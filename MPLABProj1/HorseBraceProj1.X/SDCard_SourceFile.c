#include <proc/p32mm0064gpl036.h>


//~~~~~~~~~~~~~~<Constant Definitions>~~~~~~~~~~~~~~~~~~
#define CMD0 0 

//-------------------------<SPI1 for Accelerometer>----------------------------
void Configure_SPI1(int BRGDiv){
    //Setting up the SPI2 module for master mode operation
    //BRGDiv=15 (for 250 KHz transmission)
    //BRGDiv=0 (for 4 MHz transmission)
    char RXData; //Used to clear the SPI2 interrupt
    
    //Disable interrupts 
    
    //Stop and Reset the SPI1 module
    SPI1CONbits.ON=0;
    //Clear the receive buffer
    RXData=SPI1BUF;
    
        
    //Write the Baud Rate Register
    SPI1BRG=BRGDiv; //Use Fpb/2*(BRGDiv+1) for communication
    //Clear the Receive Overflow Flag Bit
    SPI1STATbits.SPIROV=0;
    
    //~~~~~~~~~~~~~~~~<Writing the Control Register>~~~~~~~~~~~~~~~~~
    SPI1CONbits.MSTEN=1; //Master Mode    
    // Framed SPI Disabled
    SPI1CONbits.FRMEN=0; 
    
    //Standard Buffer Mode (Enhanced Buffer Mode Disabled)
    SPI1CONbits.ENHBUF=0;
    //Master Mode Slave Select Disabled
    SPI1CONbits.MSSEN=0;
    //Falling Edge of Clock for data transmission
    SPI1CONbits.CKP=0; //Idle clock state = low level (0)
    SPI1CONbits.CKE=1; //Transitions from active clock state (1) to idle clock state (0)
    
    //Master Clock Enable Bit
    SPI1CONbits.MCLKSEL=0; //Uses Fpb=8MHz
    
    //Disable Audio Codec Support Bit
    SPI1CON2bits.AUDEN=0; //Audio protocol is disabled
    
    //Selecting 8-bit mode
    SPI1CONbits.MODE16=0;
    SPI1CONbits.MODE32=0; 
    
    
    //Enable the SPI operation
    SPI1CONbits.ON=1;
    
    
    return;
}

unsigned char SPI_transmit(unsigned char DataTX){
    //Transmits a byte
    unsigned char dataRX;
    SPI1BUF=DataTX; //Data to be transmitted is written to the SPI2BUF register, automatically cleared in hardware
    while(SPI1STATbits.SPITBE~=1); //Loops while the transmit buffer is not empty (waits for it to be empty)
    dataRX=SPI1BUF;
    return(dataRX);
    
}

unsigned char SPI_receive(void){
    unsigned char DataRX;
    SPI1BUF=0xff; //Transmits dummy byte
    while(SPI1STATbits.SPIRBF~=1); //Loops while the receive buffer full status bit is 0 (empty)
    DataRX=SPI1BUF; //Reads the SPI2 buffer and clears the SPIRBF bit
    return(DataRX);
}

void Write_DO_SD(int data){
    
    LATBbits.LATB14=(0x0001&data); 
    return;
}

void Write_CS_SD(int data){
    LATBbits.LATB8=data;
    return;
}



//~~~~~~~~~~~~~~~~~~~<Send an SD Command>~~~~~~~~~~~~~~~~
unsigned char SD_sendCommand(unsigned char cmd, unsigned long arg)
{
    unsigned char response, retry=0, status;
    
    if(SDHC_flag==0){ //Convert the block address into a corresponding byte address by shifting left by 9 bits
        if(cmd==READ_SINGLE_BLOCK ||
           cmd==READ_MULTIPLE_BLOCKS ||
           cmd==WRITE_SINGLE_BLOCK ||
           cmd==WRITE_MULTIPLE_BLOCKS ||
           cmd==ERASE_BLOCK_START_ADDR||
           cmd==ERASE_BLOCK_END_ADDR ||)
        {
            arg=arg<<9;
        }
    }
    
    Write_CS_SD(0); //Drives CS pin from high to low
    SPI_transmit(cmd+0x40); //Send command, first two bits are '01'
    SPI_transmit(arg>>24);
    SPI_transmit(arg>>16);
    SPI_transmit(arg>>8);
    SPI_transmit(arg);
    if(cmd==SEND_IF_COND){
        SPI_transmit(0x87);
    }
    else{
        SPI_transmit(0x95);
    }
    
    while((response=SPI_receive())==0xff){ //Getting the response from the SD card
        if(retry++>0xfe) break; //used as a time-out error
        
    }
    if(response==0x00&&cmd==58){ //THis is to check the response of CMD58
        status=SPI_receive()& 0x40; //This is to take the first byte of the OCR register in the SD card
        if(status==0x40) SDHC_flag=1; //SDHC card needs to be verified
        else SDHC_flag=0;
        SPI_receive(); //Remaining 3 bytes of the OCR register are ignored
        SPI_receive();
        SPI_receive();
        
    }
    SPI_receive(); //Extra 8 clock pulses
    Write_CS_SD(1); //Drives CS pin from low to high
    return response; //Returns the response from the SD card 
}


//~~~~~~~~~~~~~~~~~~~~~~<Init SD Card>~~~~~~~~~~~~~~~~~~~

unsigned char SD_init(void){
    
    unsigned char i, response,SD_version; //Initializing variables
    unsigned int retry=0; 
    
    for(i=0;i<10;i++){
        SPI_transmit(0xff); //Writes 10 dummy bytes to the SD card
    }

    
Write_CS_SD(0); //Drives the CS pin from high to low
do
{
    response=SD_sendCommand(GO_IDLE_STATE,0); //Sends the CMD0 command which is a software reset
    retry++;
    if(retry>0x20){
        return 1; //There was a timeout when initializing (no card is detected)
    }
}while(response!=0x01); //Loops until the card is in the idle state and we are good to go
Write_CS_SD(1); //Drives CS pin high
//Clocks through 16 bits
SPI_transmit(0xff);
SPI_transmit(0xff);
retry=0; //Resets the counter

//Now we send the CMD8 command next to check the cersion of the SD card
SD_version=2; //Default set to SD compliance
do
{
    response=SD_sendCommand(SEND_IF_COND,0x000001AA); //Checks only for version 2 SD card, and also checks the voltage range.
    retry++;
    if(retry>0xfe){ //The card is not a version 2 card or there is an error
        SD_version=1;
        cardType=1;
        break;
        
    }
}while(response!=0x01);
retry=0; //Resets the counter

do{
    response=SD_sendCommand(APP_CMD,0); //CMD55, this must be sent before sending any ACMD command
    response=SD_sendCommand(SD_SEND_OP_COND,0x40000000); //Sends command to initiate SD
    retry++;
    if(retry>0xfe){
        return(2); //time out, the card initialization failed
    }
    
    
}while(response!=0x00); //Loops until we get the response that the command has been accepted

retry=0;
SDHC_flag=0; //No issue with SD card
if(SD_version==2) //enters if the SD card version is 2
{
    do
    {
        response=SD_sendCommand(READ_OCR,0); //Reads the OCR to make sure SD card is in correct voltage operating range
        retry++;
        if(retry>0xfe)
        {
            cardType=0; //Version two
            break;
        } //Time out
    }while(response!=0x00);
    if(SDHC_flag==1) cardType=2;
    else cardType=3;
}

return(0); //Means a successful return    
    
}

unsigned char SD_erase(unsigned long startBlock,unsigned long totalBlocks)
{
    unsigned char response;
    response=SD_sendCommand(ERASE_BLOCK_START_ADDR,startBlock); //Sends the starting address of where to delete
    if(response!=0x00)
        return response;
    response=SD_sendCommand(ERASE_SELECTED_BLOCKS,0); //Erases all the selected blocks
    if(response!=0x00) //error
        return response;
    return(0); //0 is the normal return
}

unsigned char SD_writeSingleBlock(unsigned long startBlock) //This is to read a single 512 bit block from SD card
{
    //Init variables
    unsigned char response;
    unsigned int i, retry=0;
    
    response=SD_sendCommand(WRITE_SINGLE_BLOCK,startBlock); //write a block to the SD card
    if(response !=0x00) //Checks that the card acknowledges the command
    {
        return(response); //Error in writing to the block
        
    }
    Write_CS_SD(0); //Drives CS pin from high to low
    SPI_transmit(0xfe); //Sends the start block token
    for(i=0;i<512;i++){
        SPI_transmit(buffer[i]); //Transmits the data to be sent (sends 512 bytes)
    }
    SPI_transmit(0xff); //sends 2 dummy bytes
    SPI_transmit(0xff); 
    
    response=SPI_receive();
    response=SPI_receive();
    
    if((response&0x1f)!=0x05) //response=0x05 data is accepted, response=0x0B (data rejected due to CRC error), response=0x0D (data rejected due to write error)
    {
        Write_CS_SD(1);
        return(response);
    }
    while(!SPI_receive()){ //Waits for the SD card to complete writing and go to idle state
        if(retry++>0xfffe) //Error in writing to SD card
            { 
             Write_CS_SD(1);
             return(1);
            }
    }
    
    Write_CS_SD(1); //Brings line from low to high
    
    SPI_transmit(0xff); //Delay before reasserting the CS line
    Write_CS_SD(0); //Re-asserts the CS line
    
    while()
    
    
}




