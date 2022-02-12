/* 
 * File:   fileio.h
 * Author: playf
 *
 * Created on February 11, 2022, 10:41 AM
 */

#ifndef FILEIO_HEADER_H
#define	FILEIO_HEADER_H
#include <xc.h>
#include <proc/p32mm0064gpl036.h>

extern char FError; // mailbox for error reporting

// FILEIO ERROR CODES
#define FE_IDE_ERROR        1 // IDE command execution error
#define FE_NOT_PRESENT      2 // CARD not present
#define FE_PARTITION_TYPE   3 // WRONG partition type, not FAT12
#define FE_INVALID_MBR      4 // MBR sector invalid signature 
#define FE_INVALID_BR       5 // Boot Record invalid signature 
#define FE_MEDIA_NOT_MNTD   6 // Media not mounted
#define FE_FILE_NOT_FOUND   7 // File not found in open for read
#define FE_INVALID_FILE     8 // File not open
#define FE_FAT_EOF          9 // Fat attempt to read beyond EOF
#define FE_EOF              10 // Reached the end of file
#define FE_INVALID_CLUSTER  11 // Invalid cluster value > maxcls 
#define FE_DIR_FULL         12 // All root dir entry are taken
#define FE_MEDIA_FULL       13 // All clusters in partition are taken
#define FE_FILE_OVERWRITE   14 // A file with same name exists already 
#define FE_CANNOT_INIT      15 // Cannot init the CARD
#define FE_CANNOT_READ_MBR  16 // Cannot read the MBR 
#define FE_MALLOC_FAILED    17 // Malloc could not allocate the MFILE struct
#define FE_INVALID_MODE     18 // Mode was not r.w.
#define FE_FIND_ERROR       19 // Failure during FILE search
typedef struct { 
 LBA fat; // lba of FAT
 LBA root; // lba of root directory
 LBA data; // lba of the data area 
 unsigned maxroot; // max number of entries in root dir
 unsigned maxcls; // max number of clusters in partition
 unsigned fatsize; // number of sectors
 unsigned char fatcopy; // number of copies
 unsigned char sxc; // number of sectors per cluster (!=0 flags media mounted)
 } MEDIA;
typedef struct {
 MEDIA * mda; // media structure pointer
 unsigned char * buffer; // sector buffer 
 unsigned cluster; // first cluster
 unsigned ccls; // current cluster in file
 unsigned sec; // sector in current cluster
 unsigned pos; // position in current sector
 unsigned top; // number of data bytes in the buffer
 long seek; // position in the file
 long size; // file size
 unsigned time; // last update time
 unsigned date; // last update date
 char name[11]; // file name
 char chk; // checksum = ~( entry + name[0])
 unsigned entry; // entry position in cur directory
 char mode; // mode ?r?, ?w?, ?a? 
 } MFILE;

 // file attributes 
#define ATT_RO 1 // attribute read only
#define ATT_HIDE 2 // attribute hidden
#define ATT_SYS 4 // ? system file
#define ATT_VOL 8 // ? volume label
#define ATT_DIR 0x10 // ? sub-directory
#define ATT_ARC 0x20 // ? (to) archive 
#define ATT_LFN 0x0f // mask for Long File Name records
#define FOUND 2 // directory entry match
#define NOT_FOUND 1 // directory entry not found
 
 
#define FO_SIGN 0x1FE //MBR signature location (55,AA)
#define FO_FIRST_P 0x1BE // offset of first partition table
#define FO_FIRST_TYPE 0x1C2 // offset of first partition type
#define FO_FIRST_SECT 0x1C6 // first sector of first partition offset
#define FO_FIRST_SIZE 0x1CA // number of sectors in partition
 
 // Partition Boot Record key fields offsets
#define BR_SXC 0xd // (byte) number of secotrs per cluster 
#define BR_RES 0xe // (word) number of reserved sectors for the boot record
#define BR_FAT_SIZE 0x16 // (word) FAT size in number of sectors 
#define BR_FAT_CPY 0x10 // (byte) number of FAT copies
#define BR_MAX_ROOT 0x11 // (odd word) max number of entries in root dir
// macros to extract words and longs from a byte array 
// watch out, a processor trap will be generated if the address is not word aligned

#define DIR_SIZE 28
#define DIR_ESIZE BR_FAT_SIZE
#define DIR_DATE 24
#define DIR_TIME 22
#define DIR_CLST 26
#define DIR_ATTRIB 11
 
#define ReadW( a, f) *(unsigned *)(a+f) 
#define ReadL( a, f) *(unsigned long *)(a+f)
// this is a ?safe? version of ReadW to be used on odd address fields
#define ReadOddW( a, f) (*(a+f) + ( *(a+f+1) << 8))
// prototypes
unsigned nextFAT( MFILE * fp, unsigned n);
unsigned newFAT( MFILE * fp);
unsigned readDIR( MFILE *fp, unsigned entry);
unsigned findDIR( MFILE *fp);
unsigned newDIR ( MFILE *fp);
unsigned fcloseM2( MFILE *fp);
unsigned readDATA(MFILE *fp);

//Most important functions 
MEDIA * mount( void);
void unmount( void);
MFILE * fopenM ( const char *name, const char *mode);
unsigned freadM ( void * dest, unsigned count, MFILE *);
unsigned fwriteM ( void * src, unsigned count, MFILE *);
unsigned fcloseM ( MFILE *fp);

MEDIA *D;
#endif	/* FILEIO_H */

