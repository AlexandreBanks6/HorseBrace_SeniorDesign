
#include <proc/p32mm0064gpl036.h>


//--------------------------<ADC Init>-----------------------------

void initADC(void)
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
    
    return;
}
