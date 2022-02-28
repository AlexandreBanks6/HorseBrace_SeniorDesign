/* 
 * File:   SDCardUART_Header.h
 * Author: playf
 *
 * Created on February 23, 2022, 2:41 PM
 */

#ifndef SDCARDUART_HEADER_H
#define	SDCARDUART_HEADER_H
//Function Prototypes
void init_SD_UART(unsigned long BaudRate, unsigned long FPB); 
void SendSDChar(char data);


#endif	/* SDCARDUART_HEADER_H */

