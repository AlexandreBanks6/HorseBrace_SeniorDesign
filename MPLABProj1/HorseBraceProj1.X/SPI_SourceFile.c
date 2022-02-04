
#include <proc/p32mm0064gpl036.h>
#include "SPI_HeaderFile.h"

//-----------------------<Accelerometer Register Addresses>-------------------
#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define CTRL_REG4 0x23
#define CTRL_REG5 0x24
#define HP_FILTER_RESET 0x25
#define STATUS_REG 0x27
#define OUT_X_L 0x28
#define OUT_X_H 0x29
#define OUT_Y_L 0x2A
#define OUT_Y_H 0x2B
#define OUT_Z_L 0x2C
#define OUT_Z_H 0x2D



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

void WriteACC_basic(int DataTX){
    while(SPI2STATbits.SPITBE~=1); //Loops while the transmit buffer is not empty (waits for it to be empty)
    SPI2BUF=DataTX; //Data to be transmitted is written to the SPI2BUF register, automatically cleared in hardware
    
}

int ReadACC_basic(void){
    int DataRX;
    
    while(SPI2STATbits.SPIRBF~=1); //Loops while the receive buffer full status bit is 0 (empty)
    DataRX=SPI2BUF; //Reads the SPI2 buffer and clears the SPIRBF bit
    return(DataRX);
}

void ACC_Write_Protocol(int Address, int data){
    int RW=0; //Writing to Accelerometer
    int MS=0; //Does not increment address
    int TransData;
    data=data&0x00FF; //Keeps only first 8 bits of data
    TransData=(RW<<15)|(MS<<14)|(Address<<8)|(data); //16 bit data to be transmitted
    
    WriteACC_basic(TransData);
    
    int DataRX; //Used to clear receive buffer
    DataRX=ReadACC_basic();
    
}

int ACC_Read_Protocol(int Address){
    int data; //Data to be read from the device
    int RXData;
    int RW=1; //Reading from Accelerometer
    int MS=0; //Does not increment address
    int TransData; //Must first transmit data to accelerometer to notify which buffer is being read
    
    TransData=(RW<<15)|(MS<<14)|(Address<<8)|(0b00000000); //16 bit data to be transmitted
    
    WriteACC_basic(TransData); //16-bit Data is transmitted
                                //16-bit data is received simultaneously
                                //Only last 8 bits of received data is what we care about
    
    RXData=ReadACC_basic(); //Reads 16-bit data
    data=RXData&0x00FF; //Only keeps last 8 bits
    return(data);
}


void ConfigureAccelerometer(void){
    //Control Register 1
    ACC_Write_Protocol(CTRL_REG1,0b00111111); //Normal Mode,1000 Hz data rate (low-pass filter of 780), enable all axis
    ACC_Write_Protocol(CTRL_REG2,0b00010000); //Data from internal filter sent to output register, high-pass filter set to 20 Hz
                                              //Do not need CTR_REG3 (not using interrupt pins)
    ACC_Write_Protocol(CTRL_REG4,0b10010000); //Setting BDU means output registers are not updated between MSB and LSB readings, 200g setting
                                              //Do not need CTRL_REG5, sleep-to-wake function is disabled
    return;
}

int * Read_Acc_XYZ(void){
    //To access the three accelerometer values use the following bit of code:
    /*
     * int *AccResult
     *  AccResult=Read_Acc_XYZ()
     *  XDat=*AccResult;
     *  YDat=*(AccResult+1);
     *  ZDat=*(AccResult+2);
     */
    //~~~~~~~~~~~~~~~~~~~~~~<Init Variables>~~~~~~~~~~~~~~~~~~~
    static int AccResult[3]; //Interger array containing accelerometer readings in 3 axes
    int X_L,X_H,Y_L,Y_H,Z_L,Z_H; //Variables to store three axes
    
    int DatAvailable=0; //Used to check if new data is available
    int StatusReg;
    
    
    
    //~~~~~~~~~~~~~~~~~~~~~<Waits for New Data>~~~~~~~~~~~~~~~~~~~~~~~~~~
    while(DatAvailable~=1){ //Loops while the data available status pin is not 1
        StatusReg=ACC_Read_Protocol(STATUS_REG); //Reads the status register
        DatAvailable=StatusReg&0b00001000; //Waits until the data available bit is set        
    }
    
    
    //~~~~~~~~~~~~~~~~~~~~~~~<Reading Data>~~~~~~~~~~~~~~~~~~~~~~~~
    X_L=ACC_Read_Protocol(OUT_X_L); 
    X_H=ACC_Read_Protocol(OUT_X_H);
    AccResult[0]=(X_H<<8)|X_L; //Stores the two's complement accelerometer data
    
    Y_L=ACC_Read_Protocol(OUT_Y_L); 
    Y_H=ACC_Read_Protocol(OUT_Y_H);
    AccResult[1]=(Y_H<<8)|Y_L;
    
    Z_L=ACC_Read_Protocol(OUT_Z_L); 
    Z_H=ACC_Read_Protocol(OUT_Z_H);
    AccResult[2]=(Z_H<<8)|Z_L;
    
    return(AccResult);
}