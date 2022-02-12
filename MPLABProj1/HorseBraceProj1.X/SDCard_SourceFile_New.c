#include <xc.h>
//~~~~~~~~~~~~~~<Constant/Macro Definitions Definitions>~~~~~~~~~~~~~~~~~~
#define readSPI() writeSPI(0xFF) //Function to read from the SPI (completed by writing a dummy byte)
#define clockSPI() writeSPI(0xFF)
#define disableSD() Write_CS_SD(1); clockSPI()
#define enableSD() Write_CS_SD(0)

// SD carde <proc/p32mm0064gpl036.h>
#include "SDCard_HeaderFile_New.h"

#define RESET 0 // a.k.a. GO_IDLE (CMD0)
#define INIT 1 // a.k.a SEND_OP_COND (CMD1)
#define READ_SINGLE 17 // read a block of data
#define WRITE_SINGLE 24 // write a block of data
#define DATA_START 0xFE //Specific token sent by memory card when data is being read from device
#define DATA_ACCEPT 0x05 //Response sent by SD card when data is accepted during write operations
//Define a new type that is a 32-bit memory address
typedef unsigned long LBA; //Logic block address, 32-bits
//-------------------------<SPI1 for SD Card>----------------------------
void Configure_SPI1(int BRGDiv){
    //Setting up the SPI1 module for master mode operation
    //BRGDiv=15 (for 250 KHz transmission)
    //BRGDiv=0 (for 4 MHz transmission)
    char RXData; //Used to clear the SPI2 interrupt
    
    //Disable interrupts 
    
    //Stop and Reset the SPI1 module
    SPI1CONbits.ON=0;
    //Clear the receive buffer
    RXData=SPI1BUF;
    
        
    //Write the Baud Rate Register
    SPI1BRG=BRGDiv; //Use Fpb/2*(BRGDiv+1) for communication
    //Clear the Receive Overflow Flag Bit
    SPI1STATbits.SPIROV=0;
    
    //~~~~~~~~~~~~~~~~<Writing the Control Register>~~~~~~~~~~~~~~~~~
    SPI1CONbits.MSTEN=1; //Master Mode    
    // Framed SPI Disabled
    SPI1CONbits.FRMEN=0; 
    
    //Standard Buffer Mode (Enhanced Buffer Mode Disabled)
    SPI1CONbits.ENHBUF=0;
    //Master Mode Slave Select Disabled
    SPI1CONbits.MSSEN=0;
    //Falling Edge of Clock for data transmission
    SPI1CONbits.CKP=0; //Idle clock state = low level (0)
    SPI1CONbits.CKE=1; //Transitions from active clock state (1) to idle clock state (0)
    
    //Master Clock Enable Bit
    SPI1CONbits.MCLKSEL=0; //Uses Fpb=8MHz
    
    //Disable Audio Codec Support Bit
    SPI1CON2bits.AUDEN=0; //Audio protocol is disabled
    
    //Selecting 8-bit mode
    SPI1CONbits.MODE16=0;
    SPI1CONbits.MODE32=0; 
    
    
    //Enable the SPI operation
    SPI1CONbits.ON=1;
    
    
    return;
}

//Function to change SPI1 clock speed
void Change_SPI1Clock(int BRGDiv){
    //Used to speed up the clock to 4 MHz
    SPI1CONbits.ON=0; //Disables the SPI1 module
    //Write the Baud Rate Register
    SPI1BRG=BRGDiv; //Use Fpb/2*(BRGDiv+1) for communication
    SPI1CONbits.ON=1; //Enables the SPI1 module
    return;
}


//Writing to the SPI1
unsigned char writeSPI(unsigned char DataTX){
    //Transmits a byte
    SPI1BUF=DataTX; //Data to be transmitted is written to the SPI2BUF register, automatically cleared in hardware
    while(!SPI1STATbits.SPITBE); //Loops while the transmit buffer is not empty (waits for it to be empty)
    return(SPI1BUF);
    
}


//Writing to the CS pin
void Write_CS_SD(int data){ //Writes to SD card Chip Select
    LATBbits.LATB8=data;
    return;
}



//---------------------------<SD Card Functions>------------------------

//Sending SD Commands
int sendSDCmd( unsigned char c, LBA a)
// sends a 6 byte command block to the card and leaves SDCS active
{
 int i, r;
 // enable SD card
 Write_CS_SD(0);
 // send a comand packet (6 bytes)
 writeSPI( c | 0x40); // send command + frame bit
 writeSPI( (unsigned char) a>>24); // msb of the address
 writeSPI( a>>16); 
 writeSPI( a>>8);
 writeSPI( a); // lsb
// NOTE only CMD0-RESET requires an actual CRC (once in SPI mode CRC is disabled)
 writeSPI( 0x95); // send CRC of RESET, for all other cmds it?s a don?t care
 
 //Wait for a response up to an 8 byte delay
 i=9;
 do
 {
     r=readSPI(); //Checks if the SD card has returned a response (line is no longer high)
     if(r!=0xFF) break;
 }while(--i>0);
 
 return(r);
 
 /* return response
 FF - timeout, no answer
 00 - command accepted
 01 - command received, card in idle state (after RESET)
 other errors
*/
 
 
}

//Initializing the SD card
int initSD(void)
{
    int i, r;
    
    //Sends 80 clock cycles while the card is not selected
    Write_CS_SD(1); 
    for(i=0;i<10;i++){
        clockSPI();
    }
    
    //Select the card
    Write_CS_SD(0);
    
    //Send a reset command to enter SPI mode
    r=sendSDCmd(RESET,0); Write_CS_SD(1);
    if(r!=1){
        return(0x84);
    }
    
    //Send INIT repeatedly
    i=10000; //Allows up to 0.3 seconds before timeout
    do{
        r=sendSDCmd(INIT,0); Write_CS_SD(1);
        if(r) break; //Card response not in the idle state
    }while(--i>0);
    if((i==0)||(r!=1)){
        return(0x85); //Timeout has occured
        
    }
    
    //Increase the SPI speed to 4MHz transmission
    Change_SPI1Clock(0); 
    
    return(0);
       
}

int readSECTOR(LBA a, char *p)
{
    //a=LBA requested
    //p=pointer to the data buffer
    //returns TRUE if successful
    int r,i; 
    
    r=sendSDCmd(READ_SINGLE,(a<<9));
    if(r==0) //Cheks if the command was accepted
    {
        i=10000; 
        do  //Waits for a response
        {
            r=readSPI();
            if(r==DATA_START)break; //Data will be transferred
        }while(--i>0);
        
        //If it did not timeout we will read the 512 byte sector
        if(i)
        {
            for(i=0;i<512;i++)
            {
                *p++=readSPI(); //Reads the data into the pointer
            }
            readSPI(); //Ignoring the CRC response
            readSPI();
        }
    }
    
    //Disable the card
    disableSD();
    return(r==DATA_START); //Returns TRUE if it is successful
    
    
}

int writeSECTOR(LBA a, char *p)
{
    //Writes a sector (512 bytes of data)
    //a LBA of sector request (address)
    //p pointer to the sector buffer (data to be written)
    //returns TRUE if it is successful
    unsigned r,i;
    r=sendSDCmd(WRITE_SINGLE,(a<<9));
    if(r==0) //Checks if the command was accepted
    {
        writeSPI(DATA_START);
        for(i=0;i<512;i++){
            writeSPI(*p++); //Writes the data
        }
        
        //Send two dummy CRC 
        clockSPI();
        clockSPI();
        
        //Checks if the data was accepted
        if((r=readSPI()&0xf)==DATA_ACCEPT)
        {
            for(i=10000;i>0;i--) //Wait for the end of the write operation
            {
                if(r=readSPI())
                {
                    break;
                }
            }
        }
        else
            r=FAIL;
        
    }
    //Disable the card and return
    disableSD();
    return(r); //Returns true if successful
}

//Detect the SD card
int detectSD(void)
{
    return(!PORTAbits.RA0); //Returns 1 if it is detected and 0 if it is not detected
    
}


