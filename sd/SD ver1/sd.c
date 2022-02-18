
// Sound Recorder using SD card and PIC16F876A, by www.moty22.co.uk
//
// File will compile with the free Hi-Tech C compiler.
// Recording is done by the ADC using only the LSB. Bytes are written to the SD
// at the rate of 20KHz. It is similar to the format of .wav sound files.  
// For playback CCP is used as a DAC (Digital to Analogue Converter).
   
#include <htc.h>

__CONFIG(LVPDIS & WDTDIS & UNPROTECT & HS);

#define CS RC0		//chip select input
#define Stop RB0	//stop pushbutton
#define Play RB1	//play PB
#define Rec RB2		//record PB
#define Pause RB3	//pause PB
#define RecLED RB5
#define errorLED RB4

unsigned long loc;	//pause location

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
	unsigned char i;
	
	CS=1;
	for(i=0; i < 10; i++)SPI(0xFF);		// min 74 clocks
	CS=0;			// Enabled for SPI mode

	//if (Command(0x00,0,0x95) !=1) errorLED = 1;	// Reset SD command
	i=100;	//try enter idle state for up to 100 times
	while(Command(0x00,0,0x95) !=1 && i!=0)
	{ 
		CS=1;
		SPI(0xFF);
		CS=0;
		i--;
	}
	if(i==0)	errorLED = 1;	//idle failed	

	while (Command(0x01,0,0xFF) !=0) {} //wait for idle state
	
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
			GODONE = 1;			//ADC input sample
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
	loc += 512; 		
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
		loc += 512;	
	}
	
	Command(12,0x00,0xFF);	//stop transmit
	SPI(0xFF);
	SPI(0xFF);
	CS = 1;
}

void main(void)
{
//	unsigned char Pause = 0;
//	unsigned int i;
	loc = 512;
	
	// PIC I/O init
	TRISC = 0b10010000;		// sck rc3, sdo rc5, sdi rc4, CS rc0, rc2 CPP1.
	TRISB = 0b1111;   	// switches
	RBPU = 0;		//pullup on
	RecLED = 0;
	errorLED = 0;
		
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
	
	while(1) {
	if(!Rec) WriteSD();	
	if(!Play) ReadSD();
	if(!Stop) loc = 512;
//	{
//		Pause = !Pause;
//		while(!Stop)
//		{
//			for(i=0;i<5000;i++);
//		}	
//		if(!Pause && Stop) loc = 512;
//	}
	}		
}

