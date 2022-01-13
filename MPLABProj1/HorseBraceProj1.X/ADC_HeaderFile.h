/* 
 * File:   ADC_HeaderFile.h
 * Author: playf
 *
 * Created on January 13, 2022, 2:20 PM
 */

#ifndef ADC_HEADERFILE_H   //Guard condition so that contents of this file are not included more than once
#define	UART_HEADERFILE_H
#include <xc.h>

void configureADC(void); //Configures the ADC
void ADC_ON(void); //Turns on the ADC (and interrupts)
void ADC_OFF(void); //Turns off the ADC (and interrupts)



#endif	/* ADC_HEADERFILE_H */

