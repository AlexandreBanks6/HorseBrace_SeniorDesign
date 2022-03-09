/*
 * Title: TekBoot Control Algorithm
 * Authors: TekBoot gang
 * Date: March 9th, 2022
 * 
 * 
 */



//-------------------------------<Libraries>-------------------------
#include <SPI.h> //Library for SPI interfaces
#include <SD.h> //Library for SD card
#include <DS18B20.h> //Library for ds18b20 temperature sensors
#include <OneWire.h> //Library for OneWire
#include "SparkFun_LIS331.h" //Accelerometer 
//--------------------------<Pin Value Initialize>-------------------
int FLEX1_pin=A0; //Flex sensor 1 attached to A0
int FLEX2_pin=A1; //Flex sensor 2 attached to A1
int KeyPin=A2; //Pin for controlling the bluetooth KEY
int CS_SD=9; //SD Card CS
int TEMP_pin=7; //pin for temperature sensors

int CS_ACC=A10;
int MOSI_ACC=16;
int MISO_ACC=14;
int SCK_ACC=15;
LIS331 xl; //Variable that is the accelerometer
DS18B20 ds(TEMP_pin); //establish onewire connection

//-------------------------<Initializing Variables>------------------
int Baud_BT=9600;
int BluetoothByte;
int StartRec=0; //boolean toggled by bluetooth module (1=read data from sensors)
//unsigned long SampleTimePast=0, SampleTimeNew=0; //Used to store the time betwee micros() reads (sensors are sampled every 2000 micros)
int Flex1_ValRaw, Flex2_ValRaw; //These are the variables to store the analogue to digital converter values

File myFile; //Used for SD Card files

int16_t x,y,z; //Accelerometer data
//char buf[40];
//int n; //Total number of characters stored in the buffer


int tempSensorCount=7; //Number of temperature sensors used
uint8_t addressBook[7][8];  //Change "7" to number of temp sensors used

float tempValue; //Temperature Values
uint8_t tempAddress[8];
int i; //Used as a counter for shifting through the temperature sensors
int TempBoolean=0;
unsigned int FlagCount=0;
unsigned int TempReadingNumber=9600;
int PrintTemp=0;

void setup() {

//---------------------<Serial Monitor For Debugging>-------------------
Serial.begin(Baud_BT);

//----------------------<Bluetooth Serial Interface>--------------------
pinMode(KeyPin,OUTPUT);
digitalWrite(KeyPin,LOW); //For data mode
Serial1.begin(Baud_BT); //Baud rate on the micro side


//-----------------------------<Flex Sensor Pins>-----------------------
pinMode(FLEX1_pin,INPUT);
pinMode(FLEX2_pin,INPUT);
pinMode(TEMP_pin, INPUT);

//--------------------------<Accelerometer Setup>-----------------------
pinMode(CS_ACC,OUTPUT);
digitalWrite(CS_ACC,HIGH); //Writes High to the CS pin
pinMode(MOSI_ACC,OUTPUT); //MOSI for SPI
pinMode(MISO_ACC,INPUT); //MISO for SPI
pinMode(SCK_ACC,OUTPUT); //SCK for SPI

SPI.begin(); 
xl.setSPICSPin(CS_ACC); //Sets the accelerometer to use the chip select pin
xl.begin(LIS331::USE_SPI);
xl.setODR(LIS331::DR_1000HZ); //Sets the data rate to 1000Hz
xl.setFullScale(LIS331::MED_RANGE); //Sets the accelerometer to work at +-200g
xl.setHighPassCoeff(LIS331::HPC_64); //Sets the high pass filter cutoff to 2.6 Hz
//-------------------------------<SD Card Setup>------------------------
if(!SD.begin(CS_SD)){
  Serial.println("SD Initialization Failed");
  while(1);
}
Serial.println("SD Initialization Completed");
//Open The File where data will be saved for writing and add the headers
myFile=SD.open("TekBoot.csv",FILE_WRITE);
if(myFile){
  myFile.println("Flex1,Flex2,x,y,z,tempAddress,tempValue,time");
}
myFile.close();


}


//------------------------<Void Loop>------------------------------------
void loop() {
  
static unsigned long oldtime=micros(); //Initializes the variable for the old time 

//---------------------------<Temp Sensor Setup>-----------------------
if(TempBoolean==0){ //The temperature sensor addresses need to be set up
  for(i=0; i<tempSensorCount; i++){

  ds.selectNext();
  ds.getAddress(addressBook[i]); //Getting address pf successive temperature sensors

  }

  i=0;
  TempBoolean=1;
  
}

//---------------------<Check for data from bluetooth>------------------
if(Serial1.available()>0){
  BluetoothByte=Serial1.read(); //reads the byte from the bluetooth module
  if(BluetoothByte=='I'){ //Toggles global boolean to start recording data from the sensors
    StartRec=1;
    myFile=SD.open("TekBoot.csv",FILE_WRITE); //Opens SD card file for writing
    if(myFile){
      Serial.print("File Opened Correctly");        
    }
    else{
      Serial.print("File Open Failed"); 
      myFile.close();
    }
  }
  else if(BluetoothByte=='O'){
    StartRec=0;
    myFile.close();
    Serial.print("File Closed");  
  }
  
}



//--------------------------<Sampling the Sensor Data>---------------------
//SampleTimeNew=micros(); //Gets the current microseconds since Arduino began

if(((micros()-oldtime)>=2000)&&(StartRec==1)){
  oldtime=micros(); //Updates the old sample time
  //~~~~records data
  
  //Flex sensors
  Flex1_ValRaw=analogRead(FLEX1_pin); //Reads the raw binary conversion of the analogue channel
  Flex2_ValRaw=analogRead(FLEX2_pin);
  
  //Read The Accelerometer
  xl.readAxes(x,y,z);
  //~~~~Stores Data on SD card

  //Read temperature sensors
  /*
  Serial.print(FlagCount);
  Serial.print("|");
  Serial.println(i);
  */
  if(FlagCount>=TempReadingNumber){
    if(i>=tempSensorCount){
      i = 0;
    }
  
    ds.select(addressBook[i]);
    tempValue = ds.getTempC(); //Reads the temperatre for selected address
    ds.getAddress(tempAddress);
    i++;
    FlagCount=0;
    PrintTemp=1;
  }
  
  /*
  n=sprintf(buf,"%d,%d,%hi,%hi,%hi,%lu\r",Flex1_ValRaw,Flex2_ValRaw,x,y,z,oldtime);
  
  myFile.write(buf,n); //Writes data as one string 
  */ 
  myFile.print(Flex1_ValRaw);
  myFile.print(",");
  myFile.print(Flex2_ValRaw);
  myFile.print(",");
  myFile.print(x);
  myFile.print(",");
  myFile.print(y);
  myFile.print(",");
  myFile.print(z);
  myFile.print(",");
  
  if(PrintTemp==1){
    myFile.print(tempAddress[7]);
    myFile.print(",");
    myFile.print(tempValue);
    myFile.print(",");
    PrintTemp=0;
  }
  else{
    myFile.print(",,");
  }
  
  myFile.println(oldtime);  

  //Increment Flag Count
  FlagCount++;
}

else if((micros()-oldtime)>=2000){ //Updates the past sample time, even if we aren't reading the sensors
  oldtime=micros(); //Updates the old sample time  
}

}
