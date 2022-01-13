
/* 
 * File:   MainSource.c
 * Author: Alexandre Banks
 * Date: January 4, 2022
 */

//----------------------<Configuration Bits>---------------------------------

// FDEVOPT
#pragma config SOSCHP = OFF             // Secondary Oscillator High Power Enable bit (SOSC oprerates in normal power mode.)
#pragma config USERID = 0xFFFF          // User ID bits (Enter Hexadecimal value)

// FICD
#pragma config JTAGEN = ON              // JTAG Enable bit (JTAG is enabled)
#pragma config ICS = PGx3               // ICE/ICD Communication Channel Selection bits (Communicate on PGEC3/PGED3)

// FPOR
#pragma config BOREN = BOR3             // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware; SBOREN bit disabled)
#pragma config RETVR = OFF              // Retention Voltage Regulator Enable bit (Retention regulator is disabled)
#pragma config LPBOREN = ON             // Low Power Brown-out Enable bit (Low power BOR is enabled, when main BOR is disabled)

// FWDT
#pragma config SWDTPS = PS1048576       // Sleep Mode Watchdog Timer Postscale Selection bits (1:1048576)
#pragma config FWDTWINSZ = PS25_0       // Watchdog Timer Window Size bits (Watchdog timer window size is 25%)
#pragma config WINDIS = OFF             // Windowed Watchdog Timer Disable bit (Watchdog timer is in non-window mode)
#pragma config RWDTPS = PS1048576       // Run Mode Watchdog Timer Postscale Selection bits (1:1048576)
#pragma config RCLKSEL = LPRC           // Run Mode Watchdog Timer Clock Source Selection bits (Clock source is LPRC (same as for sleep mode))
#pragma config FWDTEN = ON              // Watchdog Timer Enable bit (WDT is enabled)

// FOSCSEL (the system clock (Fsys) and peripheral clock (Fpb) are set to 8MHz)
#pragma config FNOSC = FRCDIV           // Oscillator Selection bits (Fast RC oscillator (FRC) with divide-by-N)
#pragma config PLLSRC = FRC             // System PLL Input Clock Selection bit (FRC oscillator is selected as PLL reference input on device reset)
#pragma config SOSCEN = ON              // Secondary Oscillator Enable bit (Secondary oscillator is enabled)
#pragma config IESO = OFF               // Two Speed Startup Enable bit (Two speed startup is disabled)
#pragma config POSCMOD = OFF            // Primary Oscillator Selection bit (Primary oscillator is disabled)
#pragma config OSCIOFNC = OFF           // System Clock on CLKO Pin Enable bit (OSCO pin operates as a normal I/O)
#pragma config SOSCSEL = OFF            // Secondary Oscillator External Clock Enable bit (SOSC pins configured for Crystal mode)
#pragma config FCKSM = CSDCMD           // Clock Switching and Fail-Safe Clock Monitor Enable bits (Clock switching is disabled; Fail-safe clock monitor is disabled)

// FSEC
#pragma config CP = OFF                 // Code Protection Enable bit (Code protection is disabled)

#include <xc.h>
#include <stdio.h>       //Standard library
#include <string.h>      //Library for string comparisons
#include <stdlib.h>     
#include <sys/attribs.h> //Library for interrupt macros

//Custom Libraries
#include "UART_HeaderFile.h"    //Header file for source file with UART functions
#include "ADC_HeaderFile.h"
//-------------<Function Definitions>-----------------
void ConfigurePins(void);       //Function to configure pins


//-------------------<Main>--------------------------
int main(int argc, char** argv) {
    
    //----------------<Initializing Local Variables>-----------------
    long BaudRate = 9600; 
    long FPB=8000000;   //Peripheral clock speed is 32 MHz 
    
    //---------------------<Configuring Pins>-----------------
    ConfigurePins(); // Configuring the pins
    //---------------<Initializing Peripherals>----------------
    
    initUART(BaudRate,FPB); //Initializes the UART Module
    configureADC(); //configures the ADC
    ADC_ON();
    //---------------<Initializing Pin Values>-----------------    
    WriteKey(0); //The Bluetooth module is set to data mode (0)
    
    while(1){
        
        
       
                
        
    }

    return(EXIT_SUCCESS);
}


//---------------------------------<Interrupts>---------------------------------

//~~~~~~~~~~~<ADC Interrupt>~~~~~~~~~~~~
void __ISR(_ADC_VECTOR,IPL6SOFT) ADCHandler(void)
{
    //This sets the ADC Interrupt to have a priority of 6
    //"Soft" means that context is preserved using software instructions
    
    /*
     * Put the interrupt handler here
     * You can read the ADC with the following buffers (uncomment the following)
     * ADCResultAN3=ADC1BUF0;
     * ADCResultAN4=ADC1BUF1;
     * ADCResultAN5=ADC1BUF2;
     * ADCResultAN6=ADC1BUF3;
     * ADCResultAN7=ADC1BUF4;
     */
       
    
    
    IFS0bits.AD1IF=0; //Clears the intterupt flag
}



//------------------------------<Pin Configuration>-----------------------------
void ConfigurePins(void)
{
    //---------------------<Bluetooth Module Pins (UART1)>---------------------
    //KEY Pin For Bluetooth Module (data mode or AT Command) - default is low (data mode)
    ANSELBbits.ANSB0=0; //RB0 is set as digital pin
    TRISBbits.TRISB0=0; //RB0 set as output
    //STATE Pin for bluetooth module (checks if bluetooth is working
    TRISBbits.TRISB8=1; //RB8 set as input
    //RX Pin for bluetooth module (on micros side) (does not need to be programmed because it is built in in hardware) 
    ANSELBbits.ANSB15=0; //SETS RB15 to digital I/O
    TRISBbits.TRISB15=1;    //U1RX (RB15) is set to input
    //TX Pin for bluetooth module (one micros side) (does not need to be programmed because it is built in in hardware)
    ANSELBbits.ANSB14=0; //Sets RB14 to digital I/O
    TRISBbits.TRISB14=0; //U1TX is an output
    
    //------------------------------<ADC Pins>----------------------------------
    
    //Flex Sensor 1 Input
    ANSELBbits.ANSB1=1; //RB1 (AN3) set to analogue input
    TRISBbits.TRISB1=1;
    
    //Flex Sensor 2 Input
    ANSELBbits.ANSB2=1; //RB2 (AN4) set to analogue input
    TRISBbits.TRISB2=1;
    //Accelerometer x-channel Input
    ANSELAbits.ANSA2=1; //RA2 (AN5) set to analogue input
    TRISAbits.TRISA2=1;
    //Accelerometer y-channel Input
    ANSELAbits.ANSA3=1; //RA3 (AN6) set to analogue input
    TRISAbits.TRISA3=1;
    //Accelerometer z-channel Input
    ANSELBbits.ANSB12=1; //RB12 (AN7) set to analogue input
    TRISBbits.TRISB12=1;
    
    return;
}


