#include "SDCard_HeaderFile_New.h"
#include "fat32.h"
unsigned char getBootSectorData(void){
    struct BS_Structure *bpb;
    struct MBRinfo_Structure *mbr;
    struct partitionInfo_Structure *partition;
    unsigned long dataSectors;
    int k;
    unusedSectors=0;
    writeSECTOR(0,);
    bpb=(struct BS_Structure *)buffer;
       
}