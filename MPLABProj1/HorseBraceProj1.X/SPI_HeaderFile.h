/* 
 * File:   SPI_HeaderFile.h
 * Author: playf
 *
 * Created on February 4, 2022, 3:53 PM
 */

#ifndef SPI_HEADERFILE_H
#define	SPI_HEADERFILE_H
#include <xc.h>
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

//----------------------------------<Prototypes>--------------------------------
void Configure_SPI2(void);
int ACC_Interface_basic(int DataTX);
void WriteCS_ACC(int data);
void ACC_Write_Protocol(int Address, int data);
int ACC_Read_Protocol(int Address);
void ConfigureAccelerometer(void);
double * Read_Acc_XYZ(void);

#endif	/* SPI_HEADERFILE_H */

