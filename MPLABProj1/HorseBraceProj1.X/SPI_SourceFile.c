
#include <proc/p32mm0064gpl036.h>




//-------------------------<SPI2 for Accelerometer>----------------------------
void Configure_SPI2(void){
    //Setting up the SPI2 module for master mode operation
    int RXData; //Used to clear the SPI2 interrupt
    
    //Disable interrupts 
    
    //Stop and Reset the SPI2 module
    SPI2CONbits.ON=0;
    //Clear the receive buffer
    RXData=SPI2BUF;
    
    //Clear ENHBUF to use standard buffer mode
    SPI1CONbits.ENHBUF=0; //Enhanced buffer mode is disabled
    
    //Write the Baud Rate Register
    SPI2BRG=0x1; //Use Fpb/4=2MHz for communication
    //Clear the Receive Overflow Flag Bit
    SPI2STATbits.SPIROV=0;
    
    //~~~~~~~~~~~~~~~~<Writing the Control Register>~~~~~~~~~~~~~~~~~
    SPI2CONbits.MSTEN=1; //Master Mode    
    // Framed SPI Disabled
    SPI2CONbits.FRMEN=0; 
    //Master Mode Slave Select Disabled
    SPI2CONbits.MSSEN=0;
    
    //Master Clock Enable Bit
    SPI2CONbits.MCLKSEL=0; //Uses Fpb=8MHz
    
    //Disable Audio Codec Support Bit
    SPI2CON2bits.AUDEN=0; //Audio protocol is disabled
    
    //Selecting 16-bit mode
    SPI2CONbits.MODE16=1;
    SPI2CONbits.MODE32=0; 
    
    
    //Enable the SPI operation
    SPI2CONbits.ON=1;
    
    
    return;
}

void WriteACC(int DataTX){
    while(SPI2STATbits.SPITBE~=1); //Loops while the transmit buffer is not empty (waits for it to be empty)
    SPI2BUF=DataTX; //Data to be transmitted is written to the SPI2BUF register, automatically cleared in hardware
    
}

int ReadACC(void){
    int DataRX;
    
    while(SPI2STATbits.SPIRBF~=1); //Loops while the receive buffer full status bit is 0 (empty)
    DataRX=SPI2BUF; //Reads the SPI2 buffer and clears the SPIRBF bit
    return(DataRX);
}
