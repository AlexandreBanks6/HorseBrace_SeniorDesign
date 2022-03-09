//--------------------------<Pin Value Initialize>-------------------
int FLEX1_pin=A0; //Flex sensor 1 attached to A0
int FLEX2_pin=A1; //Flex sensor 2 attached to A1



//-------------------------<Initializing Variables>------------------
int Baud_BT=57600;
int BluetoothByte;
int StartRec=0; //boolean toggled by bluetooth module (1=read data from sensors)
int SampleTimePast=0, SampleTimeNew=0; //Used to store the time betwee micros() reads (sensors are sampled every 2000 micros)
int Flex1_ValRaw, Flex2_ValRaw; //These are the variables to store the analogue to digital converter values

void setup() {

//----------------------<Bluetooth Serial Interface>--------------------
Serial1.begin(Baud_BT); //Sets the bluetooth UART baud rate

//-----------------------------<Flex Sensor Pins>-----------------------
pinMode(FLEX1_pin,INPUT);
pinMode(FLEX2_pin,INPUT);

//---------------------------<Setup Sample Time>------------------------
SampleTimePast=micros(); //Reads the microseconds since the Arduino Board began running

}

void loop() {
//---------------------<Check for data from bluetooth>------------------
if(Serial1.available()>0){
  BluetoothByte=Serial1.read(); //reads the byte from the bluetooth module

  if(BluetoothByte=="I"){ //Toggles global boolean to start recording data from the sensors
    StartRec=1;
    
  }
  else if(BluetoothByte=="O"){
    StartRec=0;
  }
  
}


//--------------------------<Sampling the Sensor Data>---------------------
SampleTimeNew=micros(); //Gets the current microseconds since Arduino began

if(((SampleTimeNew-SampleTimePast)>=2000)&&(StartRec==1)){
  //~~~~records data

  //Flex sensors
  Flex1_ValRaw=analogRead(FLEX1_pin); //Reads the raw binary conversion of the analogue channel
  Flex2_ValRaw=analogRead(FLEX2_pin);


  //Updates the past sample time to be the new sample time
  SampleTimePast=SampleTimeNew;


  //~~~~Stores Data

  
}
else if((SampleTimeNew-SampleTimePast)>=2000){ //Updates the past sample time, even if we aren't reading the sensors
  SampleTimePast=SampleTimeNew;  
}






 

}
