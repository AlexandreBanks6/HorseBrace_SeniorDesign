
#include <xc.h>
#include "SDCardUART_Header.h"

void init_SD_UART(unsigned long BaudRate, unsigned long FPB) //Initializes the UART Module for 8N1 UART
{
    U2MODEbits.BRGH=0; //Standard speed mode (16x baud clock is enabled)
    unsigned long BRGPrescalar;   //BRGPrescalar for configuring baud rate
    BRGPrescalar=FPB/(16*BaudRate)-1;
    U2BRG=BRGPrescalar; //Sets the prescalar for the baud rate generator
    U2MODEbits.SIDL=0; //Continues operation in idle mode
    U2MODEbits.IREN=0;  //Disables IrDA
    U2MODEbits.RTSMD=0; //U2RTS pin is in flow contol mode (not used in this application)

    U2MODEbits.SLPEN=1; //UART2 clock runs during sleep
    U2MODEbits.WAKE=0;  //Wake-up on start bit detection
    U2MODEbits.LPBACK=0;    //Loopback mode is disabled
    U2MODEbits.RXINV=0; //U2RX idle state is "1"
    
    U2MODEbits.PDSEL=0b00; //8-bit data with no parity
    U2MODEbits.STSEL=0; //One stop bit
    U2STAbits.UTXINV=0; //U2TX Idle state is "1"
    U2MODEbits.UEN=0b00; //U2TX and U2RX are enabled
    
    //Turning on UART1 module
    U2MODEbits.ON=1; //UART Peripheral is enabled
    U2STAbits.URXEN=1;  //UART2 receiver is enabled
    U2STAbits.UTXEN=1;  //UART2 transmitter is enabled
    
    //-----------<Interrupts for UART2 Module>--------------------
    //U2STAbits.URXISEL=0b00; //Interrupt flag bit is asserted when the receive 
                            //buffer is not empty 
    //Enabling interrupt for the receiver-data-available interrupt
    
    /*
    IPC5bits.U1RXIP=0b111; //Top priority for the UART1RX module
    IPC5bits.U1RXIS=0b11; //Top subpriority for the UART1RX module
    IFS0bits.U1RXIF=0; //Clears the interrupt flag (just to ensure)
    IEC0bits.U1RXIE=1; //Enables the interrupts for the UART module
    */
    
}


void SendSDChar(char data){
    U2STAbits.UTXEN=1;  //Ensures that the transmitter is enabled
    while(U2STAbits.UTXBF); //Waits until the transmit buffer is empty
    U2TXREG=data;  //Transmits the character
    
}