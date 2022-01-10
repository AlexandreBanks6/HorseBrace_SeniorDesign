/*
 * Title: UART_SourceFile.c
 * Date: January 5th, 2022
 * Author: Alexandre Banks
 * Description: Functions to initialize the UART1 module for the PIC32MM0064GPL036,
 * write a character, write a string, read a character, and read a string 
 */

#include "UART_HeaderFile.h"    //Includes the header file with the function definitions

//~~~~~~~~~~~~~~~~~~~<UART1 Init>~~~~~~~~~~~~~~~~~~
void initUART(long BaudRate, long FPB) //Initializes the UART Module for 8N1 UART
{
    U1MODEbits.BRGH=0; //Standard speed mode (16x baud clock is enabled)
    int BRGPrescalar;   //BRGPrescalar for configuring baud rate
    BRGPrescalar=FPB/(16*BaudRate)-1;
    U1BRG=BRGPrescalar; //Sets the prescalar for the baud rate generator
    U1MODEbits.SIDL=0; //Continues operation in idle mode
    U1MODEbits.IREN=0;  //Disables IrDA
    U1MODEbits.RTSMD=0; //U1RTS pin is in flow contol mode (not used in this application)
    U1MODEbits.UEN=0b00; //U1TX and U1RX are enabled
    U1MODEbits.WAKE=1;  //Wake-up on start bit detection
    U1MODEbits.LPBACK=0;    //Loopback mode is disabled
    U1MODEbits.RXINV=0; //U1RX idle state is "1"
    U1MODEbits.PDSEL=0b00; //8-bit data with no parity
    U1MODEbits.STSEL=0; //One stop bit
    U1STAbits.UTXINV=0; //U1TX Idle state is "1"
    U1MODEbits.ON=1; //UART Peripheral is enabled
    U1STAbits.URXEN=1;  //UART1 receiver is enabled
    U1STAbits.UTXEN=1;  //UART1 transmitter is enabled
}


//~~~~~~~~~~~~~~~~<Writing Character Using UART1>-----------------------
void WriteChar(char c)  //Used to send a character over UART1
{
    U1STAbits.UTXEN=1;  //Ensures that the transmitter is enabled
    while(U1STAbits.UTXBF); //Waits until the transmit buffer is empty
    U1TXREG=c;  //Transmits the character
    
}

//~~~~~~~~~~~~~~~~<Writing String Using UART1>------------------------
int WriteString(char *string) //Function to send string over UART1
{
    int i=0;
    U1STAbits.UTXEN=1; //Ensures the transmitter is enabled
    while(*string)
    {
        while(U1STAbits.UTXBF); //Waits until the transmit buffer is empty
        U1TXREG=*string;        //Transmits one character in the string
        string++;               //Skips to address of next character in the string
        i++;
    }
    return(i); //Returns the number of characters just sent
}

//~~~~~~~~~~~~~~~~<Reading Character Using UART1>------------------------
char ReadChar(void)             //Reads a single character from the UART1 RX
{
    while(!U1STAbits.URXDA);    //Waits receive buffer has data and at least one character can be read
    return U1RXREG;
}


//~~~~~~~~~~~~~~~~<Reading String Using UART1>------------------------
int ReadString(char *string)   //Reads a string using null '\0' termination an returns the lgnth of the character
{
    int length;
    length=0;
    
    do
    {
        *string=ReadChar(); //Reads in the character into the current string character address
        if(*string=='\r') break;    //Null termination is encountered
        string++;   //Increments string pointer to the next character
        length++;   //Increments the length counter
    }while(length<100);  //Only read in max of 100 characters
        return(length); //Returns the length of the string
}

char CheckBluetoothStatus(void)
{
    char Status;
    Status=PORTBbits.RB8;       //Reads the status of the bluetooth module
    return(Status);
}

void WriteKey(char KeyVal)
{
    LATBbits.LATB0=KeyVal;  //Writes the mode of the bluetooth module
}