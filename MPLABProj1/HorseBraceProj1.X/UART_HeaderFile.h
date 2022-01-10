/* 
 * File:   UART_HeaderFile.h
 * Author: Alexandre Banks
 * Date: January 5th, 2021 *
 */

#ifndef UART_HEADERFILE_H   //Guard condition so that contents of this file are not included more than once
#define	UART_HEADERFILE_H
#include <xc.h>
//---------------<Function Definitions>--------------
void initUART(long BaudRate, long FPB);     //Initializes the UART1 module
void WriteChar(char c);             //writes a character over UART1
int WriteString(char *string);
char ReadChar(void);             //Reads a single character from the UART1 RX
int ReadString(char *string);   //Reads a string using null '\0' termination an returns the lgnth of the character
char CheckBluetoothStatus(void); //Checks the status of the bluetooth module
void WriteKey(char KeyVal); //Sets the mode for the bluetooth module (1= AT Comman 2=data mode)


#endif	/* UART_HEADERFILE_H */

