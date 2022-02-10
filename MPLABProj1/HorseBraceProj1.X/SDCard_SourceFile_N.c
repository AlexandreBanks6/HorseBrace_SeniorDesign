#include <proc/p32mm0064gpl036.h>


//~~~~~~~~~~~~~~<Constant Definitions>~~~~~~~~~~~~~~~~~~


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

unsigned char writeSPI(unsigned char DataTX){
    //Transmits a byte
    SPI1BUF=DataTX; //Data to be transmitted is written to the SPI2BUF register, automatically cleared in hardware
    while(!SPI1STATbits.SPITBE); //Loops while the transmit buffer is not empty (waits for it to be empty)
    return(SPI1BUF);
    
}

unsigned char SPI_receive(void){
    unsigned char DataRX;
    SPI1BUF=0xff; //Transmits dummy byte
    while(SPI1STATbits.SPIRBF~=1); //Loops while the receive buffer full status bit is 0 (empty)
    DataRX=SPI1BUF; //Reads the SPI2 buffer and clears the SPIRBF bit
    return(DataRX);
}

void Write_DO_SD(int data){
    
    LATBbits.LATB14=(0x0001&data); 
    return;
}

void Write_CS_SD(int data){
    LATBbits.LATB8=data;
    return;
}
