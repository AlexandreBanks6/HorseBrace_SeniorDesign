
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
#include "UART_HeaderFile.h"    //Header file for source file with UART functions
#include "ADC_HeaderFile.h"     //Header file for ADC functions (with flex sensors)
#include "SPI_HeaderFile.h"     //Header file for SPI interface with accelerometer
//#include "SDCard_HeaderFile_New.h" //Header file for SD card interface
//#include "fileio_header.h" //Header file for FAT file system library
//-------------<Function Definitions>-----------------
void ConfigurePins(void);       //Function to configure pins


//--------------<Define Global Variables>------------

//~~~~~~~~~~~~~~~~~<SD Card Variables>~~~~~~~~~~~~~~~
//#define B_SIZE 512 //Size of data block for SD card
//char data[B_SIZE];
//char buffer[B_SIZE];



//-------------------<Main>--------------------------
void main(){
    
    //--------------------<Setting Up Interrupts>--------------------
       __builtin_enable_interrupts(); //Enables global interrupts
       INTCONbits.MVEC=1; //Interrupt controller configured for Multivectored mode
    //----------------<Initializing Variables>-----------------
    long BaudRate = 9600; //Baud rate for UART
    long FPB=8000000;   //Peripheral clock speed is 8 MHz 
    //unsigned int long Flex1_AN3; unsigned int long Flex2_AN4; //ADC results
    
    //SD Card Variables
   // MFILE *fd; 
    //unsigned r;
    //int i;
    //char datpassed;
    
    
    //Accelerometer variables
    int *AccResult; //Will contain the accelerometer readings in three axis
                    //XDat=*AccResult; YDat=*(AccResult+1);ZDat=*(AccResult+2);
    int thresh=5; //Threshold for testing accelerometer to turn on LED (if it is greater it will turn on LED in that direction)
    int DataRX;
    //---------------------<Configuring Pins>-----------------
    ConfigurePins(); // Configuring the pins
        
    //---------------<Initializing Peripherals>----------------
    
    //UART
    initUART(BaudRate,FPB); //Initializes the UART Module, turns it on, and sets up its interrupt
    
    //ADC
    initADC(); //configures the ADC for 12-bit unsigned format where sampling is triggered when AD1CON1bits.SAMP=1
                //The ADC scans the input from both flex sensors (AN3 and AN4), 
                //It uses the Fpb for auto-scanning with 1e-6 between each scan
    
    //Accelerometer
    Configure_SPI2(); //Configures SPI2 peripheral to use 2 MHz communication with the accelerometer module, and using 16-bit communication
    ConfigureAccelerometer(); //Configures the accelerometer to function in normal mode with 1000 Hz data aquisition
    
    
      
    
    
    //---------------<Initializing Pin Values>-----------------    
    WriteKey(0); //The Bluetooth module is set to data mode (0)
    //Write_CS_SD(1); //Drives cs pin on SD card low
    
    //----------------------<SD Card Test>----------------------------
    /*
    //Initializes the DATA for testing
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
    
    unmount();
    
     * */
    
    
    while(1){
         //AccResult=Read_Acc_XYZ(); //Calls to read three axis of accelerometers
        WriteACC_basic(0xCCCC);
         //Used to clear receive buffer
        //DataRX=ReadACC_basic();
    }
    
    return;
}


//---------------------------------<Interrupts>---------------------------------

//~~~~~~~~~~~~~<UART1 Module Interrupt>~~~~~~~~~~~~~~~~

void __ISR(_UART1_RX_VECTOR,IPL7AUTO) UART1Handler(void){
    //UART1 interrupts are top priority (above the ADC)
    //This means that Bluetooth commands take precedence over all other interrupts (the ADC)
    //Interrupt for UART1 generate UART when data is received
    char dataUART1;
    
    dataUART1=ReadChar(); //Must read in the data before we clear interrupt flag
    
    if(dataUART1=='I')
    {
        LATBbits.LATB7=1; //Turns on green LED
    }
    else if(dataUART1=='O')
    {
        LATBbits.LATB3=1; //Turns on red LED
    }
   else{
        LATBbits.LATB3=0; //Turns off green LED
        LATBbits.LATB7=0; //Turns off the red LED
        
    }
   
    
    
    IFS0bits.U1RXIF=0; //Clears the interrupt flag
}



//~~~~~~~~~~~~~<Timer1 Interrupt>~~~~~~~~~~~~~~~~

void __ISR(_TIMER_1_VECTOR, IPL6AUTO) Timer1Handler(void)
{
    
    
    
    
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
    //---------------------<Accelerometer Pins>-----------------------
    //Chip select (active low)
    ANSELBbits.ANSB13=0; //Digital pin
    TRISBbits.TRISB13=0; //CS for accelerometer
    
    //Need to use peripheral pin select for the accelerometer pins
    //Data in (SDI2)
    RPINR11bits.SDI2R=18; //Connect SPI2 DI to pin 18
    
    //Outputs
    //Data out (SDO2)
    RPOR4bits.RP17R=3; //Connects SDO2 to RP17
    //Clock out (SCK2)
    RPOR2bits.RP12R=4; //Connects SCK2OUT to RP12
    
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
    ANSELAbits.ANSA1=0; //Set as digital pin
    TRISAbits.TRISA1=0; //RA1 set as output
    
    //Green LED
    ANSELAbits.ANSA3=0; //set as digital pin
    TRISAbits.TRISA3=0; //RA3 set as output
    
    //Yellow LED
    TRISCbits.TRISC9=0; //RC9 set as digital output
    
    
    return;
}


//-------------------------<Timer 1 Functions>----------------------
void StopTimer1(void)
{
    T1CONbits.ON = 0;       //Disable Timer1
}

void StartTimer1(void)
{
    T1CONbits.ON = 1;       //Enable Timer1
}

void DisableTimer1Interrupt(void)
{
    IEC0bits.T1IE = 0;
}

void EnableTimer1Interrupt(void)
{
    IEC0bits.T1IE = 1;
}

void InitializeTimer1(void)
{
    StopTimer1();
    
    TMR1 = 0x0000;      //Reset Timer 1 Value
    T1CONbits.TCKPS = 1;    //Set Prescalar to 1:8
    PR1 = 9999;         //Interrupt every 0.020 seconds
    
    IEC0bits.T1IE = 1;       //Enable Timer1 Interrupt
    IFS0bits.T1IF = 0;       //Clear Timer1 Interrupt Flag
}
      
