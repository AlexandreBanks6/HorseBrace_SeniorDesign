void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600); 

}

void loop() {
static unsigned long oldtime=micros(); //Initializes the variable for the old time 
if(((micros()-oldtime)>=2000)){
  oldtime=micros();
  Serial.println(oldtime);
}

  
  // put your main code here, to run repeatedly:

}
