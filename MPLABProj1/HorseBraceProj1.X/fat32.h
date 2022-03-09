/* 
 * File:   fat32.h
 * Author: playf
 *
 * Created on February 22, 2022, 3:44 PM
 */

#ifndef FAT32_H
#define	FAT32_H
#include <xc.h>
#include <stdlib.h>
#include <math.h>

struct MBRinfo_Structure{
    unsigned charnothing[446]; //Ignore, placed here to fill the gap in the structure
    unsigned charpartitionData[64]; //Partition records (16x4)
    unsigned shortsignature;
};

//Structure to access info of the first partition of the disk
struct partitionInfo_Structure{
    unsigned charstatus;
    unsigned char headStart;
    unsigned shortcycleSectStart;
    unsigned chartype;
    unsigned charheadEnd;
    unsigned shortcylSectEnd;
    unsigned longfirstSector;
    unsigned longsectorsTotal;
};

//Structure to access the boot sector data
struct BS_Structure{
    unsigned char jumpBoot[3];
    
    unsigned char OEMName[8];
    unsigned short bytesPerSector;
    unsigned char sectorPerCluster;
    unsigned short reservedSectorCount;
    unsigned char numberofFATs;
    unsigned short rootEntryCount;
    unsigned short totalSectors_F16;
    unsigned char mediaType;
    unsigned short FATsize_F16;
    unsigned short sectorsPerTrack;
    unsigned short numberofHeads;
    unsigned long hiddenSectors;
    unsigned long totalSectors_F32;
    unsigned long FATsize_F32;
    unsigned short extFlags;
    unsigned short FSversion;
    unsigned long rootCluster;
    unsigned short FSinfo;
    unsigned short BackupBootSector;
    unsigned char reserved[12];
    unsigned char driveNumber;
    unsigned char reserved1;
    unsigned char bootSignature;
    unsigned long volumeID;
    unsigned char volumeLabel[11];
    unsigned char fileSystemType[8];
    unsigned char bootData[420];
    unsigned short bootEndSignature;
}bpb;

//Structure to access FS info sector data

struct FSInfo_Structure{
    unsigned long leadSignature;
    unsigned char reserved1[480];
    unsigned long structureSignature;
    unsigned long freeClusterCount;
    unsigned long nextFreeCluster;
    unsigned char reserved2[12];
    unsigned long trailSignature;
};

struct dir_Structure{
    unsigned char name[11];
    unsigned char attrib;
    unsigned char NTreserved;
    unsigned char timeTenth;
    unsigned short createTime;
    unsigned short createDate;
    unsigned short lastAccessDate;
    unsigned short firstClusterHI;
    unsigned short writeTime;
    unsigned short writeDate;
    unsigned short firstClusterLO;
    unsigned long fileSize;
};

#define ATTR_READ_ONLY 0x01
#define ATTR_HIDDEN 0x02
#define ATTR_SYSTEM 0x04
#define ATTR_VOLUME_ID 0x08
#define ATTR_DIRECTORY 0x10
#define ATTR_ARCHIVE 0x20
#define ATTR_LONG_NAME 0x0f
#define DIR_ENTRY_SIZE 0x32
#define EMPTY 0x00
#define DELETED 0x35
#define GET 0
#define SET 1
#define READ 0
#define VERIFY 1
#define ADD 0
#define REMOVE 1 
#define LOW 0
#define HIGH 1
#define TOTAL_FREE 1
#define NEXT_FREE 2
#define GET_LIST 0
#define GET_FILE 1
#define DELETE 2
#define EOF 0x0fffffff

//External variables
volatile unsigned long firstDataSector, rootCluster, totalClusters;
volatile unsigned short bytesPerSector, reservedSectorCount;
volatile unsigned char sectorPerCluster;
unsigned long unusedSectors, appendFileSector, appendFileLocation, fileSize, appendStartCluster;
unsigned char freeClusterCountUpdated;

//Functions
unsigned char getBootSectorData(void);
unsigned long getFirstSector(unsigned long clusterNumber);
unsigned long getSetFreeCluster(unsigned char totOrNext, unsigned char get_set, unsigned long FSEntry);
struct dir_Structure* findFiles(unsigned char flag, unsigned char *fileName);
unsigned long getSetNextCluster(unsigned long clusterNumber, unsigned char get_set,unsigned long cluster);
unsigned char readFile(unsigned char flag, unsigned char *fileName);
unsigned char convertFileName(unsigned char *filename);
void writeFile(unsigned char *fileName);
void appendFile(void);
unsigned long searchNextFreeCluster(unsigned long startCluster);
void memoryStatistics(void);
void displayMemory(unsigned char flag, unsigned long memory);
void deleteFile(unsigned char *fileName);
void freeMemoryUpdate(unsigned char flag, unsigned long size);





#endif	/* FAT32_H */

