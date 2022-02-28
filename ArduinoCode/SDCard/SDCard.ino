/*
    SD card read/write
    This example shows how to read and write data to and from an SD card file
    The circuit:
    SD card attached to SPI bus or SDMMC
    created   Nov 2010
    by David A. Mellis
    modified 9 Apr 2012
    by Tom Igoe
    modified 18 June 2021
    by Hongtai.liu
    This example code is in the public domain.
*/
#include <Seeed_Arduino_FS.h>


#define DEV  SD

#define SDCARD_SS_PIN 1
#define SDCARD_SPI SPI


#define LOG Serial

//Init Variables
int Bool=0;
unsigned int incomingData=0;
int count=0;

void setup() {

  
    LOG.begin(250000);
    pinMode(5, OUTPUT);
    digitalWrite(5, HIGH);
    while (!LOG) {};
 

    while (!DEV.begin(SDCARD_SS_PIN,SDCARD_SPI,4000000UL)) {
        LOG.println("Card Mount Failed");
        return;
    }  
    File RootWrite = DEV.open("/TekBoot.csv", "w");
    // File RootWrite = DEV.open("/hello.txt", FILE_WRITE);

    // if the file opened okay, write to it:
    if (RootWrite) {
        LOG.print("Writing to TekBoot.txt...");
        RootWrite.println("Welcome To Horse Data,");
        // close the file:
        RootWrite.close();
        LOG.println("done.");
    } else {
        // if the file didn't open, print an error:
        LOG.println("error opening hello.txt");
    }

  LOG.println("Receiving from Micro, sending to SD:");

    LOG.println("initialization done.");

    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.

    
    /*
    // re-open the file for reading:
    File RootRead= DEV.open("/hello.txt");
    if (RootRead) {
        LOG.println("hello.txt:");

        // read from the file until there's nothing else in it:
        while (RootRead.available()) {
            LOG.write(RootRead.read());
        }
        // close the file:
        RootRead.close();
    } else {
        // if the file didn't open, print an error:
        LOG.println("error opening hello.txt");
    }
    */
    /*
    for(Counter;Counter<6;Counter++){
    RootWrite = DEV.open("/TekBoot.csv", FILE_APPEND);
    
    RootWrite.println(Counter);
    RootWrite.println(",");
    LOG.println("Wrote Extra");
    RootWrite.close();
    }
    */


    Serial1.begin(250000,SERIAL_8N1); //Speed of UART communication

    

    
}

void loop() {

if(Serial1.available()>0){
    File RootWrite = DEV.open("/TekBoot.csv", FILE_APPEND);
    incomingData=Serial1.read();
    
    
    RootWrite.println(incomingData);
    RootWrite.print(",");
    LOG.println(incomingData);
    LOG.print(",");
    RootWrite.close();
    
}



/*
File RootWrite = DEV.open("/TekBoot.csv", FILE_APPEND);
LOG.println("Open File");

while(1){
  if(Serial1.available()>0){
       
    incomingData=Serial1.read();
    
    
    RootWrite.println(incomingData);
    RootWrite.print(",");
    LOG.println(incomingData);
    LOG.print(",");
    break;
  }

}
while(1){
  if(Serial1.available()>0){
       
    incomingData=Serial1.read();
    
    
    RootWrite.println(incomingData);
    RootWrite.print(",");
    LOG.println(incomingData);
    LOG.print(",");
    break;
  }

}
while(1){
  if(Serial1.available()>0){
       
    incomingData=Serial1.read();
    
    
    RootWrite.println(incomingData);
    RootWrite.print(",");
    LOG.println(incomingData);
    LOG.print(",");
    break;
  }

}
while(1){
  if(Serial1.available()>0){
       
    incomingData=Serial1.read();
    
    
    RootWrite.println(incomingData);
    RootWrite.print(",");
    LOG.println(incomingData);
    LOG.print(",");
    break;
  }

}
while(1){
  if(Serial1.available()>0){
       
    incomingData=Serial1.read();
    
    
    RootWrite.println(incomingData);
    RootWrite.print(",");
    LOG.println(incomingData);
    LOG.print(",");
    break;
  }

}
while(1){
  if(Serial1.available()>0){
       
    incomingData=Serial1.read();
    
    
    RootWrite.println(incomingData);
    RootWrite.print(",");
    LOG.println(incomingData);
    LOG.print(",");
    break;
  }

}
while(1){
  if(Serial1.available()>0){
       
    incomingData=Serial1.read();
    
    
    RootWrite.println(incomingData);
    RootWrite.print(",");
    LOG.println(incomingData);
    LOG.print(",");
    break;
  }

}
while(1){
  if(Serial1.available()>0){
       
    incomingData=Serial1.read();
    
    
    RootWrite.println(incomingData);
    RootWrite.print(",");
    LOG.println(incomingData);
    LOG.print(",");
    break;
  }

}
while(1){
  if(Serial1.available()>0){
       
    incomingData=Serial1.read();
    
    
    RootWrite.println(incomingData);
    RootWrite.print(",");
    LOG.println(incomingData);
    LOG.print(",");
    break;
  }

}
while(1){
  if(Serial1.available()>0){
       
    incomingData=Serial1.read();
    
    
    RootWrite.println(incomingData);
    RootWrite.print(",");
    LOG.println(incomingData);
    LOG.print(",");
    break;
  }

}
while(1){
  if(Serial1.available()>0){
       
    incomingData=Serial1.read();
    
    
    RootWrite.println(incomingData);
    RootWrite.print(",");
    LOG.println(incomingData);
    LOG.print(",");
    break;
  }

}
while(1){
  if(Serial1.available()>0){
       
    incomingData=Serial1.read();
    
    
    RootWrite.println(incomingData);
    RootWrite.print(",");
    LOG.println(incomingData);
    LOG.print(",");
    break;
  }

}

*/

/*

  while(1){

   
   if(Serial1.available()>0){
    if(count==0){
    RootWrite = DEV.open("/TekBoot.csv", FILE_APPEND);
    LOG.println("Open File");
    }
    
    incomingData=Serial1.read();
    
    
    RootWrite.println(incomingData);
    RootWrite.print(",");
    LOG.println(incomingData);
    LOG.print(",");

    count=count+1;
    
    if(count>=12){
    RootWrite.close();
    LOG.println("Close File");
    count=0;
    }
    
    
    
   
  }
  
   

  
  //Serial1.write(0b00101010);
  }

 */
 
    
/*
    // nothing happens after setup
    RootWrite.close();
    LOG.println("Close File");
    */
    
}
