// standard C libraries used 
#include <stdlib.h> // NULL, malloc, free...
#include <ctype.h> // toupper...
#include <string.h> // memcpy...
#include <proc/p32mm0064gpl036.h>
#include "SDCard_HeaderFile_New.h" //SD Card/MMc interface
#include "fileio_header.h"

//-----------<Upgraded Functions for use with FAT16 File System>--------------

//mount initializes a MEDIA structure for FILEIO access

MEDIA*mount(void){
    LBA psize; //Number of sectors in a partition
    LBA firsts; //LBA of first sector inside the first partition
    int i;
    unsigned char *buffer;
    
    
    Write_CS_SD(1); //Drives CS pin high for the SD card
    
    initSD(15); //Configures the SPI1 peripheral for data transfer with SD card
    
    //Check if the card is in the slot
   /* if(!detectSD())
    {
        FError=FE_NOT_PRESENT;
        return(NULL);
    }
    */
    
      
    //Initializes the SD card
    if(initMedia()){ //Include action here to show there is an error in the SD card
        FError=FE_CANNOT_INIT;
        return(NULL);
        
    }
    
    //allocate space for a media structure (dyanmically add two data structures)
    D=(MEDIA*)(malloc(sizeof(MEDIA)));
    if(D==NULL) //Report an error
    {
        FError=FE_MALLOC_FAILED;
        return(NULL); 
    }
    //Allocate space for a temporary sector buffer
    buffer=(unsigned char *) malloc(512);
    if(buffer==NULL) //Report an error
    {
        FError=FE_MALLOC_FAILED;
        free(D);
        return(NULL);
    }
    
    //Get the Master Boot Record
    if(!readSECTOR(0,buffer))
    {
        FError=FE_CANNOT_READ_MBR;
        free(D); free(buffer);
        return(NULL);
    }
    
    
    //Check if the MBR sector is valid
    if((buffer[FO_SIGN]!=0x55)||(buffer[FO_SIGN+1]!=0xAA))
    {
        FError=FE_INVALID_MBR;
        free(D); free(buffer);
        return(NULL);
    }
    
    //Read te number of sectors in partition
    psize=ReadL(buffer,FO_FIRST_SIZE);
    
    //Check if the parition tupe is accetable
    i=buffer[FO_FIRST_TYPE];
    switch(i)
    {
        case 0x04:
        case 0x06:
        case 0x0E:
            //valid FAT16 options
            break;
        default:
            FError=FE_PARTITION_TYPE;
            free(D); free(buffer);
            return(NULL);
    }
    
    //Get the first partition first sector -> boot record
    firsts=ReadL(buffer,FO_FIRST_SECT);
    
    //Get the sector loaded (boot record)
    if(!readSECTOR(firsts,buffer))
    {
        free(D); free(buffer);
        return(NULL);
    }
    
    //Check if the boot record is valid
    if((buffer[FO_SIGN]!=0x55)||(buffer[FO_SIGN+1]!=0xAA))
    {
        FError=FE_INVALID_BR;
        free(D); free(buffer);
        return(NULL);
    }
    
    //Determine the size of a cluster
    D->sxc=buffer[BR_SXC];
    //This will act as a flag that the media is mounted
    
    //Determine fat, root, and data LBAs
    //FAT=first sector in partition (the boot record)+reserved records
    D->fat=firsts+ReadW(buffer,BR_RES);
    D->fatsize=ReadW(buffer,BR_FAT_SIZE);
    D->fatcopy=buffer[BR_FAT_CPY];
    //Finding position of the root directory
    //Root=FAT+(sectors per FAT*copies of FAT)
    D->root=D->fat+(D->fatsize*D->fatcopy);
    
    //MAX ROOT is the max number of entries in the root directory
    D->maxroot=ReadOddW(buffer,BR_MAX_ROOT);
    
    //DATA=ROOT+(MAXIMUM_ROOT*32/512)
    D->data=D->root+(D->maxroot>>4); //Assuming maxroot%16==0
    //Max cluster in this partition = (tot sectors-sys sectors)/sxc
    D->maxcls=(psize-(D->data-firsts))/D->sxc;
    
    
    
    
    
    //Free up the temporary buffer
    free(buffer);
    return(D);
    
    
    
 }


//----------------------------------------------------------------------
// unmount releases the space allocated for the MEDIA structure 
//
void unmount( void){ 
 free( D); 
 D = NULL; 
} // unmount


/*
MFILE *fopenM(const char *filename, const char *mode){ //Function that finds all possible information regarding a file
            //Gathering it in a new structure that is called MFILE

    char c;
    int i,r;
    unsigned char *b; //Newly allocated buffer
    MFILE *fp;        //Pointer to new allocated MFILE structure
    MEDIA *mda=D;     //Pointer to MEDIA structure
    unsigned e;
    //Check if the storage device is mounted
    if(D==NULL) //Unmounted storage device
    {
        FError=FE_MEDIA_NOT_MNTD;
        return(NULL);
    }
    
    //Allocate a buffer for the file
    b=(unsigned char*)malloc(512);
    if(b==NULL)
    {
        FError=FE_MALLOC_FAILED;
        return(NULL);
    }
    
    
    //Allocates an MFILE structure on the heap
    fp=(MFILE*)malloc(sizeof(MFILE));
    if(fp==NULL) //report an error
    {
        FError=FE_MALLOC_FAILED;
        free(b);
        return(NULL);
        
    }
    
    //The buffer and media pointers can now be recorded inside the MFILE data structure
    
    fp->mda=D;
    fp->buffer=b;
    
    
    //The file name parameters must be extracted, and each character must be translated to upper case
    
    for(i=0;i<8;i++)
    {
        c=toupper(*filename++); //Reads a char and converts it to upper case
        if((c=='.')||(c=='\0')) //Extension or short name noextension
        {
            break;
        }
        else
        {
            fp->name[i]=c;
        }
    }
    //If short fille up the rest of the name with spaces
    while(i<8) fp->name[i++]=' ';
    
    //If there is an extension
    if(c!='\0')
    {
        for(i=8;i<11;i++)
        {
            c=toupper(*filename++); //Reads char, converts it to upper case
            if(c=='.')
            {
                c=toupper(*filename++);
            }
            if(c=='\0') //Short extension
            {
                break;
            }
            else
            {
                fp->name[i]=c;
            }
        }
        
        //If it is short will up the rest with up to 3 spaces
        while(i<11) fp->name[i++]=' ';
    }
    
    //Copy the file mode character (r,w)
    if((*mode=='r')||(*mode=='w'))
    {
        fp->mode=*mode;
    }
    else
    {
        FError=FE_INVALID_MODE;
        goto ExitOpen;
    }
    
    //Search for the file in the current directory
    if((r=findDIR(fp))==FAIL)
    {
        FError=FE_FIND_ERROR;
        goto ExitOpen;
    }
    
    //Initializes all counters to the beginning of the file
    fp->seek=0; //First byte in the file
    fp->sec=0; //First sector in the cluster
    fp->pos=0; //First byte in the sector/cluster
    
    //Depenging on the mode (read or write) we will do one of the following
    
    if(fp->mode=='r')
    {
        //'r' open for reading
        if(r==NOT_FOUND)
        {
            FError=FE_FILE_NOT_FOUND;
            goto ExitOpen;
        }
        else //file is found
        {
            //Set the current cluster pointer on the first file cluster
            fp->ccls=fp->cluster; 
            //Read a sector of data from the file
            if(!readDATA(fp))
            {
                goto ExitOpen;
            }
            
            //Determine how much data is really inside the buffer
            if(fp->size-fp->seek<512)
            {
                fp->top=fp->size-fp->seek;
            }
            else
            {
                fp->top=512;
            }
        }
    }
    else //Open for writing
    {
        if(r==NOT_FOUND)
        {
           //Allocate a first cluster to it
            fp->ccls=0; //Indicates a brand new file
            if(newFAT(fp)==FAIL)
            {
                //Must be media full
                FError=FE_MEDIA_FULL;
                goto ExitOpen; 
            }
            fp->cluster=fp->ccls; 
            
            //Create a new entry
            //Search again, for an empty entry this time
            if((r=newDIR(fp))==FAIL) //report any error
            {
                FError=FE_IDE_ERROR;
                goto ExitOpen;
            }
            
            //New entry not found
            if(r==NOT_FOUND)
            {
                FError=FE_DIR_FULL;
                goto ExitOpen;
            }
            else //New Entry identified by fp->entry filled
            {
                //Init file size
                fp->size=0;
                e=(fp->entry&0xf)*DIR_ESIZE; //16 entries per sector
                //Set initial file size to 0
                fp->buffer[e+DIR_SIZE]=0;
                fp->buffer[e+DIR_SIZE+1]=0;
                fp->buffer[e+DIR_SIZE+2]=0;
                fp->buffer[e+DIR_SIZE+3]=0;
                
                //Default time and date
                fp->date=0x34FE; //July 30th, 2006
                fp->buffer[r+DIR_DATE]=fp->date;
                fp->buffer[e+DIR_DATE+1]=fp->date>>8;
                fp->buffer[e+DIR_TIME]=fp->time;
                fp->buffer[e+DIR_TIME+1]=fp->time>>8;
                
                //Set the first cluster
                fp->buffer[e+DIR_CLST]=fp->cluster;
                fp->buffer[e+DIR_CLST+1]=(fp->cluster>>8);
                
                //Set the name
                for(i=0;i<DIR_ATTRIB;i++){
                    fp->buffer[e+i]=fp->name[i];
                }
                
                //Set the attrib
                fp->buffer[e+DIR_ATTRIB]=ATT_ARC;
                
                //Update the directory sector
                if(!writeDIR(fp,fp->entry)){
                    FError=FE_IDE_ERROR;
                    goto ExitOpen;
                }
            }
        }
        
        else //File exists already, report error
        {
            FError=FE_FILE_OVERWRITE;
            goto ExitOpen
        }
    }
    
    //Compute the MFIL structure checksum
    fp->chk=~(fp->entry+fp->name[0]);
    
    return(fp);
    
    //In the case that any of the previous steps failed, we will exit the function and return a NULL pointer
    //Exit with error
    ExitOpen:
            free(fp->buffer);
            free(fp);
            return(NULL);
    
}
 */


unsigned readDATA(MFILE *fp){
    LBA 1;
    //Calculates the lba of cluster/sector
    1=fp->mda->data+(LBA)(fp->ccls-2)*fp->mda->sxc+fp->sec;
    
    return(readSECTOR(1,fp->buffer));
}

unsigned readDIR(MFILE *fp, unsigned e){
    //Loads the current entry sector in the file buffer
    //Returns FAIL/TRUE
    
    LBA l;
    //Load the root sector containing the DIR entry "e"
    l=fp->mda->root+(e>>4);
    return(readSECTOR(1,fp->buffer));
}

unsigned findDIR(MFILE *fp){
    //fp=file structure
    //return found/not_found/fail
    //Loops through all the available entries in the root directory
    unsigned eCount; //Current entry counter
    unsigned e; //Current entry offset in buffer
    int i,a,c,d;
    MEDIA *mda=fp->mda;
    
    //Start from the first entry
    eCount=0;
    
    //Load the first sector of the root
    if(!readDIR(fp,eCount))
    {
        return(FAIL);
    }
    
    //loop until you reach the end or find the file
    while(1)
    {
        
        //determine the offset in the current buffer
        e=(eCount&oxf)*DIR_ESIZE;
        //Read the first char of the file name
        a=fp->buffer[e+DIR_NAME];
        
        //Terminate if it is empty (end of the list)
        if(a==DIR_EMPTY)
        {
            return(NOT_FOUND);
        }
        //Skip erased entries if looking for a match
        if(a!=DIR_DEL)
        {
            //If not VOLume or DIR comapre the names
            a=fp->buffer[e+DIR_ATTRIB];
            if(!((a&ATT_DIR)||(a&ATT_VOL)))
            {
                //Compare file name and extension
                for(i=DIR_NAME;i<DIR_ATTRIB;i++)
                {
                    if((fp->buffer[e+i])!=(fp->name[i]))
                    {
                        break; //Difference found
                    }
                }
                if(i==DIR_ATTRIB)
                {
                    //Entry found, fill the mfile structure
                    fp->entry=eCount; //Store the entry index
                    fp->time=ReadW(fp->buffer,e+DIR_TIME);
                    fp->date=ReadW(fp->buffer,e+DIR_DATE);
                    fp->size=ReadL(fp->buffer,e+DIR_SIZE);
                    fp->cluster=ReadL(fp->buffer,e+DIR_CLST);
                    return(FOUND);
                }
            }
        }
        
        //get the next entry
        eCount++;
        if(eCount&0xf==0)
        {
            //load a new sector from the Dir
            if(!readDIR(fp,eCount))
            {
                return(FAIL);
            }
        }
        
        //Exit the loop if reached the end of error
        if(eCount>=mda->maxroot)
        {
            return(NOT_FOUND); //Last entry reached
        }
    }
}

unsigned freadM(void *dest,unsigned size,MFILE *fp){
    //Reads freely blocks of data from file
    //fp=pointer to MFILE structure
    //dest pointer to destination buffer
    //count number of bytes to transfer
    //returns number of bytes actually transferred
    MEDIA *mda=fp->mda; //Media structure
    unsigned count=size; //Counts the number of bytes to be transferred
    unsigned len;
    
    //Check if fp points to a valid open file structure
    if((fp->entry+fp->name[0]!=~fp->chk)||(fp->mode!='r'))
    {
        //Checksum fails or not open in read mode
        FError=FE_INVALID_FILE;
        return(size-count);
    }
    
    //Loop to transfer the data
    while(count>0){
        
        //Check if the EOF is reached
        if(fp->seek>=fp->size)
        {
            FError=FE_EOF; //Reached the end of the file
            break;
        }
        
        //Load a new sector if necessary
        if(fp->pos==fp->top)
        {
            fp->pos=0;
            fp->sec++;
            //Get a new cluster if necessary
            if(fp->sec==mda->sxc)
            {
                fp->sec=0;
                if(!nextFAT(fp,1))
                {
                    break;
                }
            }
            //Load a sector of data
            
            if(!readDATA(fp))
            {
                break;
            }
            //Determine how much data is really inside the buffer
            if(fp->size-fp->seek<512){
                fp->top=fp->size-fp->seek;
            }
            else{
                fp->top=512;
            }
            
            
        }
        
        //Copy as many bytes as possible in a single chunk, take as much as fits in the current sector
        
        if(fp->pos+count<fp->top){
            len=count; //Fits all in current sector
        }
        else{
            len=fp->top-fp->pos; //Take a first chunk, but there is more
        }
        
        memcpy(dest,fp->buffer+fp->pos,len);
        
        //Update all counters and pointers
        count-=len; //Compute what is left
        dest+=len; //Advance the destination pointer
        fp->pos+=len; //Advance the pointer in the current sector
        fp->seek+=len; //Advance the seek pointer
        
    }
    
    //Return the number of bytes actually transferred
    return(size-count);
    
}

unsigned fcloseM2(MFILE*fp)
{
    //Closing a file
    //Invalidate the file structure
    fp->chk=fp->entry+fp->name[0]; //Set the checksum invalid
    //Free up the buffer and the MFILE structure
    free(fp->buffer);
    free(fp);
}

unsigned fwriteM(void *src,unsigned count,MFILE*fp){
    //src=points to the source data (buffer)
    //count=number of bytes to write
    //returns the number of bytes actually written
    MEDIA *mda=fp->mda;
    unsigned len,size=count;
    
    //Check if the file is open
    if(fp->entry+fp->name[0]!=~fp->chk){
        //Checksum fails
        FError=FE_INVALID_FILE;
        return(FAIL);
    }
    
    //Loop writing count btes
    while(count>0)
    {
        //Copy as many bytes at a time as possible
        if(fp->pos+count<512)
        {
            len=count;
        }
        else
        {
            len=512-fp->pos;
        }
        memcpy(fp->buffer+fp->pos,src,len);
        
        //Update all pointers and counters
        fp->pos+=len; //Advance the buffer position
        fp->seek+=len; //Count the added bytes
        count-=len; //Update the counter
        src+=len; //Advance the source pointer
        
        //Update the file size as well
        if(fp->seek>fp->size)
        {
            fp->size=fp->seek;
        }
        //If the buffer is full, write current buffer to current sector
        if(fp->pos==512)
        {
            //Write buffer full of data
            if(!writeDATA(fp))
            {
                return(FAIL);
            }
            //Advance to next sector in cluster
            fp->pos=0;
            fp->sec++;
            
            //Get a new cluster if necessary 
            if(fp->sec==mda->sxc)
            {
                fp->sec=0;
                if(newFAT(fp)==FAIL){
                    return(FAIL);                    
                }
            }
            
            
            
        }
    }
    
    //Number of bytes actually written
    return(size-count);
    
}

unsigned fcloseM(MFILE *fp){
    unsigned e,r; //Offset of directory entry in current buffer
    r=FAIL;
    //Check if it was open for write
    if(fp->mode=='w')
    {
        //If the current buffer contains data, flush it
        if(fp->pos>0)
        {
            if(!writeDATA(fp)){
                goto ExitClose;
            }
        }
        //Finally update the dir entry
        //Retrieve the dir sector
        if(!readDIR(fp,fp->entry)){
            goto ExitClose;
        }
        //Determine position in the DIR sector
        e=(fp->entry&0xf)*DIR_ESIZE; //Entry per sector
        
        //Update file size
        fp->buffer[ e + DIR_SIZE] = fp->size;
        fp->buffer[ e + DIR_SIZE+1]= fp->size>>8;
        fp->buffer[ e + DIR_SIZE+2]= fp->size>>16;
        fp->buffer[ e + DIR_SIZE+3]= fp->size>>24;
        
        //Update the directory sector
        if(!writeDIR(fp,fp->entry)){
            goto ExitClose;
        }
    }
    //Exit with success
    r=TRUE;
    //invalidate the file structure
    fp->chk=fp->entry+fp->name[0]; //Set checksum wrong
    
    //Free up the buffer and the MFILE structure
    free(fp->buffer);
    free(fp);
    
    return(r);
    
}

unsigned newDIR(MFILE *fp)
{
    //Used to find an available spot in the root directory to create a new file
    //fp=file structure
    //return (found/fail), fp->entry filled
    unsigned eCount; //Current entry counter
    unsigned e; //Current entry offset in buffer
    
    int a;
    MEDIA *mda=fp->mda;
    //Start from the first entry
    eCount=0;
    //Load the first sector of root
    if(!readDIR(fp,eCount)){
        return(FAIL);
    }
    
    //Loop until you reach the end or find the file
    while(1)
    {
        //Determine the offset in current buffer
        e=(eCount&0xf)*DIR_ESIZE;
        //Read the first char of the file name
        a=fp->buffer[e+DIR_NAME];
        //Terminate if it is empty (end of the list) or deleted
        if((a==DIR_EMPTY)||(a==DIR_DEL))
        {
            fp->entry=eCount;
            return(FOUND);
        }
        //Get the next entry
        eCount++;
                if((eCount&0xf)==0)
                {
                    //load a new sector from the root
                    if(!readDIR(fp,eCount)){
                        return(FAIL);
                    }
                }
        //Exit loop if reached the end or error
        if(eCount>mda->maxroot){
            return(NOT_FOUND); //Last entry reached
        }
    }
    return(FAIL);
}

unsigned newFAT(MFILE*fp){
    //Used to find an available cluster to allocate for a new block of data
    //fp=file structure
    //fp->ccls==0 if first cluster to be allocated, !=0 if additional cluster
    //return TRUE/FAIL
    //fp-ccls new cluster member
    
    unsigned i, c=fp->ccls;
    
    //Sequentially scan through the FAT looking for an empty cluster
    do{
        c++; //Check next cluster in FAT
        //Check if reached last cluster in FAT, re-start from the top
        if(c>=fp->mda->maxcls){
            c=0;
        }
        
        //check if full circle done, media full
        if(c==fp->ccls)
        {
            FError=FE_MEDIA_FULL;
            return(FAIL);
        }
        //look at its value
        i=readFAT(fp,c);
    }while(i!=0); //Scanning for an empty cluster
    //mark the cluster as taken, and last in chain
    writeFAT(fp,c,FAT_EOF);
    
    //if not first cluster, link current cluster to the new one
    if(fp->ccls>0){
        writeFAT(fp,fp->ccls,c);
    }
    //update the MFILE structure
    fp->ccls=c;
    return(TRUE);
    
}

unsigned readFAT(MFILE*fp, unsigned ccls){
    //fp=MFILE structure
    //ccls=current cluster
    //return=next cluster value
    //oxffff if failed or last
    unsigned p,c;
    LBA l;
    //get address of curren cluster in fat
    p=ccls;
    //cluster=0xabcd
    //packed as: 0 | 1 | 2 | 3 |
    // word p   0 1|2 3|4 5|6 7|
    //          cd ab|cd ab|cd ab| cd ab|
    
    //Load the fat sector containing the cluster
    l=fp->mda->fat+(p>>8); //256 clusters per sector
    if(!readSECTOR(1,fp->buffer))
    {
        return(0xffff); //Failed
    }
    //get the next cluster value
    c=ReadOddW(fp->buffer,((p&0xFF)<<1));
    return(c);
    
    
    
}


unsigned writeFAT(MFILE *fp, unsigned cls, unsigned v){
    //Updates the contents of the FAT and keeps all its sopies current
    //fp=MFILE structure
    //cls=current cluster
    //v=next value
    //return TRUE if successful, or FAIL
    unsigned p;
    LBA l;
    //Get address of current cluster in fat
    p=cls*2; //always even
    //load the fat sector containg the cluster
    l=fp->mda->fat+(p>>9);
    p&=0x1fe;
    if(!readSECTOR(1,fp->buffer)){
        return(FAIL);
    }
    
    //Get the next cluster value
    fp->buffer[p]=v; //lsb
    fp->buffer[p+1]=(v>>8); //msb
    //Update all the fat copies
    for(i=0;i<fp->mda->fatcopy;i++,l+=fp->mda->fatsize)
        if(!writeSECTOR(1,fp->buffer))
            return(FAIL);
    
    return(TRUE);

}

unsigned writeDATA(MFILE*fp)
{
    //write actual sectors of data to the storage device
    LBA l;
    //Calculate lba or cluster/sector
    l=fp->mda->data+(LBA)(fp->ccls-2)*fp->mda->sxc+fp->sec;
    return(writeSECTOR(l,fp->buffer));
}
