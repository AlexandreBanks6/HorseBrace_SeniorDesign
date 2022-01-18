/* 
 * File:   ADC_HeaderFile.h
 * Author: playf
 *
 * Created on January 13, 2022, 2:20 PM
 */

#ifndef ADC_HEADERFILE_H   //Guard condition so that contents of this file are not included more than once
#define	UART_HEADERFILE_H
#include <xc.h>

void initADC(void); //Configures the ADC
void ReadADC(unsigned long* ADCResultAN3,unsigned long* ADCResultAN4); //Reads the ADC

#endif	/* ADC_HEADERFILE_H */

