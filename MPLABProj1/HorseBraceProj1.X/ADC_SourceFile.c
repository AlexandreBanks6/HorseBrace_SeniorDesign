
#include <proc/p32mm0064gpl036.h>

#include "ADC_HeaderFile.h"
//--------------------------<ADC Configuration>-----------------------------

void configureADC(void)
{
    /*
     * ADC Sampling frequency is set to 248.14 Hz
     * 
     * Uses Vrefp=AVdd=3.3V and Vrefn=AVss=0V
     * 
     * Uses 12-bit unsigned integer format
     * 
     * Uses auto-convresion trigger in combination with the automatic sampling
     * start which allows the ADC module to schedule acquisition/conversion
     * sequences with no intervention.
     * 
     * Scans five analogue inputs (AN3, AN4, AN5, AN6, and AN7) and 
     * sequentially stores these values in the first five buffers before
     * triggering an interrupt.
     * 
     * The frequency is set such that
     * Fs=Fpb/(10*SAMC*ADCS) where Fpb is the peripheral clock set at 8MHz
     */
    
    
    //Add the five inputs to the scan list (AN3, AN4, AN5, AN6, and AN7)
    AD1CSSbits.CSS3=1;
    AD1CSSbits.CSS4=1;
    AD1CSSbits.CSS5=1;
    AD1CSSbits.CSS6=1;
    AD1CSSbits.CSS7=1;
    
    //Enables the scan
    AD1CON2bits.CSCNA=1;
    //Set the number of conversion per intterupt to the number of inputs (5-1)
    AD1CON2bits.SMPI=4;
    //Clock from peripheral clock
    AD1CON3bits.ADRC=0;
    //ADC clock TAD=ADCS*2*Tpb where ADCS=248
    AD1CON3bits.ADCS=248; 
    
    //Set the sample time to 13*TAD (1/248.14 Hz)
    AD1CON3bits.SAMC=13;    
    
    //Enable sampling automatically after each conversion is complete
    AD1CON1bits.ASAM=1;
    
    //Set to auto conversion trigger
    AD1CON1bits.SSRC=7; 
    
    //Continues module operation in idle mode
    AD1CON1bits.SIDL=0;
    
    //Using 12bit ADC operation
    AD1CON1bits.MODE12=1;
    
    
    //Data Output format is 12 bit unsigned integer 
    AD1CON1bits.FORM=0b000;
    
    //Voltage reference uses Vr+=AVdd (3.3 V) and Vr-=AVss (0V)
    AD1CON2bits.VCFG=0b000;
    
    
    return;
}


//-----------------------------<ADC Control>-----------------------------------
void ADC_ON(void)
{
    //Turns on the ADC module
    AD1CON1bits.ON=1;
    //Starts the first sampling cycle
    AD1CON1bits.SAMP=1;
    
    //Setting the interrupts for the ADC module
    IEC0bits.AD1IE=1; //Enables interrupts from the ADC module
    
    return;
}

void ADC_OFF(void)
{
    //Turns the ADC module off
    AD1CON1bits.ON=0;
    
    //Disables interrupts from the ADC module
    IEC0bits.AD1IE=0;
}
