
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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "UART_HeaderFile.h"    //Header file for source file with UART functions

//-------------<Function Definitions>-----------------
void ConfigurePins(void);       //Function to configure pins
void writeGreen(char state);
void writeRed(char state);

int main(int argc, char** argv) {
    long BaudRate = 9600; 
    long FPB=8000000;   //Peripheral clock speed is 32 MHz 
    
    //---------------------<Configuring Pins>-----------------
    ConfigurePins(); // Configuring the pinse
    //---------------<Initializing Peripherals>----------------
    
    initUART(BaudRate,FPB); //Initializes the UART Module
    
    //----------------<Initializing Variables>-----------------
    char DataUART[200]; //String that will contain the data from the UART module
    int length; //Length of the string returned by the UART module
    //---------------<Initializing Pin Values>-----------------
    writeGreen(0);  //Green led is off
    writeRed(0);    //Red LED is off
    
    WriteKey(0); //The Bluetooth module is set to data mode (0)
    
    while(1){
        ANSELBbits.
        char dataChar;  //Char Read From Bluetooth Module
        //while(CheckBluetoothStatus()!=0); //Loops until the bluetooth module is ready for use (0==ready for use)
        //WriteChar('G');
        
        dataChar=ReadChar();    //Reads Character from bluetooth module
        if(dataChar=='G')
        {
            writeGreen(1);  //Turns green light on 
            writeRed(0);    //Turns red light off
        }
        else if(dataChar=='R')
        {
            writeGreen(0);  //Turns green light off 
            writeRed(1);    //Turns red light on
        }
        else
        {
            writeGreen(0);  //Turns green light off 
            writeRed(0);    //Turns red light off
        }   
        
        
        
        //The following is preliminary code to read a string
        /* length=ReadString(DataUART);   //Waits until a string is read by the UART module and then string is stored in "DataUART"
        
        if(strcmp(DataUART,"Green"))
        {
            writeGreen(1);  //Turns green light on 
            writeRed(0);    //Turns red light off
        }
        else if(strcmp(DataUART,"Red"))
        {
            writeGreen(0);  //Turns green light off 
            writeRed(1);    //Turns red light on
        }
        else
        {
            writeGreen(0);  //Turns green light off 
            writeRed(0);    //Turns red light off
        }
        */
       
                
        
    }

    return(EXIT_SUCCESS);
}


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

void writeGreen(char state)
{
    LATBbits.LATB12=state; //Writes "state" value (0 or 1) to green LED
    return;
}

void writeRed(char state)
{
    LATAbits.LATA3=state; //Writes "state" value (0 or 1) to red LED
}



