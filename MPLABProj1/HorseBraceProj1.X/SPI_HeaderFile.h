/* 
 * File:   SPI_HeaderFile.h
 * Author: playf
 *
 * Created on February 4, 2022, 3:53 PM
 */

#ifndef SPI_HEADERFILE_H
#define	SPI_HEADERFILE_H
#include <xc.h>
void Configure_SPI2(void);
void WriteACC_basic(int DataTX);
int ReadACC_basic(void);
void ACC_Write_Protocol(int Address, int data);
int ACC_Read_Protocol(int Address);
void ConfigureAccelerometer(void);
int * Read_Acc_XYZ(void);

#endif	/* SPI_HEADERFILE_H */

