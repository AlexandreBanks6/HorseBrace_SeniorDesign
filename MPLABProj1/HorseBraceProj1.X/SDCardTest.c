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
#include <cp0defs.h>
#include <proc/p32mm0064gpl036.h>


//Custom Libraries
#include "SDCard_HeaderFile_New.h" //Header file for SD card interface
#include "fileio.h" //Header file for FAT file system library
//-------------<Function Definitions>-----------------
void ConfigurePins(void);       //Function to configure pins


//--------------<Define Global Variables>------------

//~~~~~~~~~~~~~~~~~<SD Card Variables>~~~~~~~~~~~~~~~
#define B_SIZE 512 //Size of data block for SD card
char data[B_SIZE];
//char buffer[B_SIZE];

     

//-------------------<Main>--------------------------
void main(){
    MFILE *fd; 
    unsigned r;
    int i;
    char datpassed;
    
    //--------------------<Setting Up Interrupts>--------------------
       __builtin_enable_interrupts(); //Enables global interrupts
       INTCONbits.MVEC=1; //Interrupt controller configured for Multivectored mode
    //----------------<Initializing Variables>-----------------
   
    
    
    //---------------------<Configuring Pins>-----------------
    ConfigurePins(); // Configuring the pins
    //---------------<Initializing Pin Values>-----------------    
    Write_CS_SD(1); //Drives cs pin on SD card low
    
    //--------------<Initializes the DATA for testing>---------
    datpassed=0;
    for(i=0;i<B_SIZE;i++){
        data[i]=datpassed;
        datpassed++;       
    }
    
    
    
    
    while(!detectSD()); //Waits for the SD card to be inserted
    
    if(mount()) //Mounts (initializes) the SD card and enters the if statement if it is successful
    {
        if((fd=fopenM("HorseData.txt","w"))) //Create a file and continues if there are no errors, opens the file for writing
        {
            r=fwriteM(data,B_SIZE,fd); //Data is the data to be written, B_SIZE is the size of the data to be written, fd is the information stored in the FAT
            fcloseM(fd); //Closes the file
        }
        
    }
    
    unmount(); //Unmounts the SD card
      
      
    while(1){
                
        
    }
        
    return;
}

//-----------------------------------<Functions>--------------------------------

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
    
    
    //-------------------------<SD Card Pins>-------------------------
    //TRISBbits.TRISB9=0; //SD01 (MOSI) for SD card (don't need to initialize these because they are driven directly by peripheral)
    
    //ANSELBbits.ANSB14=0; //Digital pin
    //TRISBbits.TRISB14=1; //SDI1 (MISO) for SD card
   
    //TRISBbits.TRISB8=0; //SCK1 (clock) for SD card
    
    ANSELBbits.ANSB15=0; //Digital pin
    TRISBbits.TRISB15=0; //SS1 (chip select) for SD card
    
    ANSELAbits.ANSA0=0; //Digital pin
    TRISAbits.TRISA0=1; //CD (card detect) set as input
    
    //------------------------<Debugging Pins>-------------------------
    
    //Red LED
    ANSELBbits.ANSB3=0; //RB3 is digital
    TRISBbits.TRISB3=0; //RB3 set to output to red led
    
    //Green LEDd
    TRISBbits.TRISB7=0; //RB4 set to output to green led
    
    return;
}
