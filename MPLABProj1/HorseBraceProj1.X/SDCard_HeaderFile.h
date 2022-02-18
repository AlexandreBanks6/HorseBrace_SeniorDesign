/* 
 * File:   SDCard_HeaderFile.h
 * Author: playf
 *
 * Created on February 18, 2022, 4:43 AM
 */

#ifndef SDCARD_HEADERFILE_H
#define	SDCARD_HEADERFILE_H
#include <xc.h>

//Definitions
#define GO_IDLE_STATE 0
#define SEND_OP_COND 1
#define SEND_IF_COND 8
#define SEND_CSD 9
#define STOP_TRANSMISSION 12
#define SEND_STATUS 13
#define SET_BLOCK_LEN 16
#define READ_SINGLE_BLOCK 17
#define READ_MULTIPLE_BLOCKS 18
#define WRITE_SINGLE_BLOCK 24
#define WRITE_MULTIPLE_BLOCKS 25
#define ERASE_BLOCK_START_ADDR 32
#define ERASE_BLOCK_END_ADDR 33
#define ERASE_SELECTED_BLOCKS 38
#define SD_SEND_OP_COND 41
#define APP_CMD 55
#define READ_OCR 58
#define CRC_ON_OFF 59


//Variable definitions
volatile unsigned long startBlock,totalBlocks;
volatile unsigned char SDHC_flag,cardType,buffer[512];

//Function Prototypes
void setupSPI(int BRGDiv);
unsigned char SPI_transmit(unsigned char DataTX);
unsigned char SPI_receive(void);
void Write_CS_SD(int data);
unsigned char SD_sendCommand(unsigned char cmd, unsigned long arg);
unsigned char SD_init(void);
unsigned char SD_erase(unsigned long startBlock,unsigned long totalBlocks);
unsigned char SD_readSingleBlock(unsigned long startBlock);
unsigned char SD_writeSingleBlock(unsigned long startBlock);



#endif	/* SDCARD_HEADERFILE_H */

