/* 
 * File:   SDCard_HeaderFile_New.h
 * Author: playf
 *
 * Created on February 10, 2022, 3:25 PM
 */

#ifndef SDCARD_HEADERFILE_NEW_H
#define	SDCARD_HEADERFILE_NEW_H
#include <xc.h>

#define TRUE 1
#define FALSE 0
#define FAIL 0

typedef unsigned long LBA; //Logic block address 
void Configure_SPI1(int BRGDiv); //Configures the SPI interface with the SD card
void Change_SPI1Clock(int BRGDiv); //Allows changing the clock speed
unsigned char writeSPI(unsigned char DataTX); //Write a byte over SPI
void Write_CS_SD(int data); //Drive the CS pin
int sendSDCmd( unsigned char c, LBA a); //Send a 6 byte command block to the card
int initSD(void); //Initialize the SD card
int readSECTOR(LBA a, char *p); //Read a sector from the SD card
int writeSECTOR(LBA a, char *p); //Write a sector to the SD card
int detectSD(void); //Used to detect if the SD card is present


#endif	/* SDCARD_HEADERFILE_NEW_H */

