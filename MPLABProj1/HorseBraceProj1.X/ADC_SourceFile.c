
#include <proc/p32mm0064gpl036.h>

#include "ADC_HeaderFile.h"
//--------------------------<ADC Configuration>-----------------------------

void initADC(void)
{
    /*
     * ADC module is set up to have sampling triggered in software
     * The sampling frequency is set to 500 Khz by the following equation
     * Since we have two channels we are scanning the sampling frequency is divided 
     * by 2 as follows: Fs=Fpb/(2*2*SAMC*ADCS) where SAMC=2 and ADCS=2 and Fpb=8MHz.
     * 
     * To trigger ADC sampling the AD1CON1bits.SAMP=1 and it will take 1e-6 to convert the 
     * first channel (AN3) and another 1e-6 to convert the second channel so Ts=2e-6
     * 
     * Uses Vrefp=AVdd=3.3V and Vrefn=AVss=0V
     * 
     * Uses 12-bit unsigned integer format
     * 
     * Uses auto-convresion trigger so conversions occur directly after sampling
     * 
     * Scans two analogue inputs (AN3, AN4) using auto-scan
     */
    
    //-------------------<Init Variables>----------------------
    int ADCSVal;
    int SAMCVal;
    
    //Calculating values for desired sampling frequency
    SAMCVal=2;
    ADCSVal=2; //Determines the ADCS value for the desired sampling frequency of the ADC
    
    
    //~~~~~~~~~~~~~~~~~~~~~~Setting up Auto-Scan>~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Add the two inputs to the scan list (AN3, AN4)
    AD1CSSbits.CSS3=1;
    AD1CSSbits.CSS4=1;
    
    //The converted result is stored in the corresponding buffer
    //AN3 is stored in ADC1BUF3 and AN4 is stored in ADC1BUF4
    //The scan start is triggered by the autoconversion trigger which happens 1e-6 after SAMP=1
    AD1CON2bits.BUFREGEN=1; 
    AD1CON5bits.LPEN=0; //Full power is enabled after scan
    AD1CON5bits.ASEN=1; //Auto-scan is enabled
    
    
    
    
    
    //Clock from peripheral clock
    AD1CON3bits.ADRC=0;
    
    //Setting up the Sampling frequency
    
    //ADC clock TAD=2*ADCS*Tpb where ADCS=2 and Tpb=8 
    AD1CON3bits.ADCS=ADCSVal;
    //Set the sample time to 2*TAD
    AD1CON3bits.SAMC=SAMCVal;    
       
     
    //Set to auto conversion trigger (conversion automatically occurs after sampling)
    AD1CON1bits.SSRC=7; 
    
    //Continues module operation in idle mode
    AD1CON1bits.SIDL=0;
    
    //Using 12bit ADC operation
    AD1CON1bits.MODE12=1;
    
    
    //Data Output format is 12 bit unsigned integer 
    AD1CON1bits.FORM=0b000;
    
    //Voltage reference uses Vr+=AVdd (3.3 V) and Vr-=AVss (0V)
    AD1CON2bits.VCFG=0b000;
    
    //Turning on the ADC module
    AD1CON1bits.ON=1; 
    
    
    return;
}


//-----------------------------<ADC Control>-----------------------------------
void ReadADC(unsigned long* ADCResultAN3,unsigned long* ADCResultAN4)
{
    //Input is the result of the two flex sensor measurements
    //Flex sensor measurements are on the AN3 (RB1) and AN4 (RB2) pins
        
    IFS0bits.AD1IF=0; //Clears the intterupt flag for ADC just to ensure we won't generate an interrupt right away
    
    //Starts sampling and after 1e-6 the auto-scan is triggered (for AN3)
    //It takes 2e-6 to sample both channels
    AD1CON1bits.SAMP=1;
    
    //Waits for the result
    while(IFS0bits.AD1IF==0); //Loops until the autoscan is complete
    
    //Gets the ADC value
    *ADCResultAN3=ADC1BUF3;
    *ADCResultAN4=ADC1BUF4;
    
        
    /*
     * To use this function you must follow this syntax
     * unsigned int long ADCResultAN3; unsigned int long ADCResultAN4;
     *  ReadADC(&ADCResultAN3,&ADCResultAN4);
     */
    
    return;
}
