//#include <SoftwareSerial.h>

//SoftwareSerial BTSerial(0,1);


void setup() {
  // put your setup code here, to run once:
  //pinMode(KeyPin,OUTPUT);
  //digitalWrite(KeyPin,HIGH);

  Serial.begin(38400);
  delay(2000);
  Serial.println("Enter AT Commands");
  Serial1.begin(38400); //Sets the bluetooth UART baud rate for AT command mode

}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial1.available()){
    Serial.write(Serial1.read());
  }

  if(Serial.available()){
    Serial1.write(Serial.read());
  }

}
