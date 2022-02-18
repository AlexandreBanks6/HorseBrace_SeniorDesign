
// Sound Recorder using SD/SDHC card and PIC16F876A, by www.moty22.co.uk
//
// File will compile with the free Hi-Tech C compiler or MPLABX with XC8.
// Recording is done by the ADC using only the LSB. Bytes are written to the SD
// at the rate of 20KHz. 
// For playback CCP is used as a DAC (Digital to Analogue Converter).
   
#include <htc.h>

#if defined(__XC8)
  #pragma config LVP=OFF, FOSC=HS, CP=OFF, CPD=OFF, WDTE=OFF
#else defined(COMPILER_MPLAB_PICC)
  __CONFIG(LVPDIS & UNPROTECT & WDTDIS & HS & UNPROTECT);
#endif

#define CS RC0		//chip select input
#define Stop RB0	//stop pushbutton
#define Play RB1	//play PB
#define Rec RB2		//record PB
#define Pause RB3	//pause PB
#define RecLED RB5
#define errorLED RB4
#define sdLED RC6	//SD or SDHC optional LED

//prototypes
unsigned char SPI(unsigned char data);
char Command(unsigned char frame1, unsigned long adrs, unsigned char frame2 );
void InitSD(void);
void main(void);
void WriteSD(void);
void ReadSD(void);


unsigned long loc;	//pause location
unsigned char sdhc=0;	//standard sd

unsigned char SPI(unsigned char data)		// send character over SPI
{
	SSPBUF = data;			// load character
	while (!BF);		// sent
	return SSPBUF;		// received character
}

char Command(unsigned char frame1, unsigned long adrs, unsigned char frame2 )
{	
	unsigned char i, res;
	SPI(0xFF);
	SPI((frame1 | 0x40) & 0x7F);	//first 2 bits are 01
	SPI((adrs & 0xFF000000) >> 24);		//first of the 4 bytes address
	SPI((adrs & 0x00FF0000) >> 16);
	SPI((adrs & 0x0000FF00) >> 8);
	SPI(adrs & 0x000000FF);	
	SPI(frame2 | 1);				//CRC and last bit 1

	for(i=0;i<10;i++)	// wait for received character
	{
		res = SPI(0xFF);
		if(res != 0xFF)break;
	}
	return res;	  
}

void InitSD(void)
{
	unsigned char i,r[4];
	
	CS=1;
	for(i=0; i < 10; i++)SPI(0xFF);		// min 74 clocks
	CS=0;			// Enabled for SPI mode
	
	i=100;	//try enter idle state for up to 100 times
	while(Command(0x00,0,0x95) !=1 && i!=0)
	{ 
		CS=1;
		SPI(0xFF);
		CS=0;
		i--;
	}
	if(i==0)	errorLED = 1;	//idle failed
	
	if (Command(8,0x01AA,0x87)==1){					//check card is 3.3V
		r[0]=SPI(0xFF); r[1]=SPI(0xFF); r[2]=SPI(0xFF); r[3]=SPI(0xFF);		//rest of R7
		if ( r[2] == 0x01 && r[3] == 0xAA ){ 		//Vdd OK (3.3V)
			
			//Command(59,0,0xFF);		//CRC off
			Command(55,0,0xFF);
			while(Command(41,0x40000000,0xFF)){Command(55,0,0xFF);} 	//ACMD41 with HCS bit
			}
	}else{errorLED = 1;} 
	
	if (Command(58,0,0xFF)==0){		//read CCS in the OCR - SD or SDHC
		r[0]=SPI(0xFF); r[1]=SPI(0xFF); r[2]=SPI(0xFF); r[3]=SPI(0xFF);		//rest of R3
		sdhc=r[0] & 0x40;
		if(r[0] & 0x40)sdLED=1;  
	}
	
	SSPM1 = 0;	// full speed 2MHz
	CS = 1;
}

void WriteSD(void)	
{
	unsigned int r,i;
	CS = 0;	
	ADON = 1;
	RecLED = 1;
	
	r = Command(25,loc,0xFF);	//multi sector write
	if(r != 0)
	{
		errorLED = 1;
		ADON = 0;
		RecLED = 0;
	}
	SPI(0xFF);
	SPI(0xFF);
	SPI(0xFF);
	
	while(Stop && Pause)
	{
		SPI(0xFC);	//multi sector token byte
		for(i=0;i<512;i++)
		{
			#if defined(__XC8)
            GO_DONE = 1;
            #else defined(COMPILER_MPLAB_PICC)
            GODONE=1;
            #endif
			while(!TMR2IF){}	//20KHz clock
			SPI(ADRESL);		//send analogue byte
			TMR2IF = 0;
				//play while record
			CCP1X = ADRESL & 1;	//shift byte to get the required PWM duty cycle 
			CCPR1L = (ADRESL >> 1);
		}
 		SPI(0xFF);	// CRC
		SPI(0xFF);	// CRC
	
	if((r=SPI(0xFF) & 0x0F) == 0x05){	//data accepted	= 0101		
		for(i=10000;i>0;i--){				
			if(r=SPI(0xFF))	break;
		}	
	}
	else{
		errorLED = 1;
	}
	while(SPI(0xFF) != 0xFF);	// while busy
	
	if(sdhc){loc += 1;}else{loc += 512;}	//SD or SDHC 		
	}
	SPI(0xFD);	//stop transfer	token byte
			
	SPI(0xFF);
	SPI(0xFF);
	while(SPI(0xFF) != 0xFF);	// while busy
	
	CS = 1;
	ADON = 0;
	RecLED = 0;
}

void ReadSD(void)
{
	unsigned int i,r;
	unsigned char data;
	CS = 0;
	r = Command(18,loc,0xFF);	//read multi-sector
	if(r != 0)errorLED = 1;			//if command failed

	while(Stop && Pause)
	{
		while(SPI(0xFF) != 0xFE);	// wait for first byte
		for(i=0;i<512;i++){
			while(!TMR2IF){}
			data = SPI(0xFF);
			CCP1X = data & 1;	//shift byte to get the required PWM duty cycle 
			CCPR1L = (data >> 1);
			TMR2IF = 0;
		}
		SPI(0xFF);	//discard of CRC
		SPI(0xFF);
		
		if(sdhc){loc += 1;}else{loc += 512;}	//SD or SDHC	
	}
	
	Command(12,0x00,0xFF);	//stop transmit
	SPI(0xFF);
	SPI(0xFF);
	CS = 1;
}

void main(void)
{
	
	// PIC I/O init
	TRISC = 0b10010000;		// sck rc3, sdo rc5, sdi rc4, CS rc0, rc2 CPP1.
	TRISB = 0b1111;   	// switches
        //pullup
	#if defined(__XC8)
    nRBPU = 0;
    #else defined(COMPILER_MPLAB_PICC)
    RBPU = 0;
    #endif
	RecLED = 0;
	errorLED = 0;
	sdLED = 0;
		
	//analogue init
	CCP1CON = 0B1100;	//PWM mode
	PR2 = 100;	//20KHz
	T2CON = 0B100;	//prescale 1, post scale 1, timer2 on
	ADCON1 = 0B11001110;		// Fosc/16, ch1, ref=Vdd, right just. 
	ADCON0 = 0B1000000;	// Fosc/16.
	
	//SPI init
	SSPCON = 0B110010;	//low speed osc/64(125kHz),enabled,clock idle=H
	CS = 1; 		// disable SD	

	InitSD();
	if(sdhc){loc = 10;}else{loc = 5120;}	//SD or SDHC
	while(1) {
	if(!Rec) WriteSD();	
	if(!Play) ReadSD();
	if(!Stop) {
		if(sdhc){loc = 10;}else{loc = 5120;}
	}	

	}		
}

