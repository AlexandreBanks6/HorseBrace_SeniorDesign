/*
Luc Bouvier
Ds18b20 Temperature Sensor Reading
2022-03-08

*/

#include <DS18B20.h>

#define TEMP_PIN 2

DS18B20 ds(TEMP_PIN);


void setup() {

Serial.begin(9600);

//Serial.print("Devices: ");
//Serial.println(ds.getNumberOfDevices());
}

void loop() {

//loop through all available ds18b20 sensors
  while(ds.selectNext()) {

      uint8_t address[8];
      ds.getAddress(address);

      float temp;
      temp = ds.getTempC();

  }
    
}
