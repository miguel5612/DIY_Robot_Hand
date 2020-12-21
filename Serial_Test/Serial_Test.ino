#include <SoftwareSerial.h>
SoftwareSerial Bluetooth(9, 8); // Arduino(RX, TX) - HC-05 Bluetooth (TX, RX) 3 4
bool first = false, salto = false;
unsigned long timeoutB = 0;
char inByte;
String dataIn;

void setup() {
  // put your setup code here, to run once:
  Bluetooth.begin(9600); // Default baud rate of the Bluetooth module
  Serial.begin(115200);
  Serial.println("Hola");
}

void loop() {
  // put your main code here, to run repeatedly:
  readBtSerial();
  
  if(dataIn.length()>2 & dataIn!="" & salto) 
  {
    Serial.println(dataIn);
    salto = false;
    dataIn = "";
  }
}

void readBtSerial()
{
  while (Bluetooth.available() > 0) {
    inByte = Bluetooth.read();
    if(inByte == 's' & !first)
    {
      first = true;
      timeoutB = millis();
    }  
    else if((first & (millis() - timeoutB > 5)) || inByte == 's' || inByte == '.')
    {
      first = false;
      salto = true;
      break;    
    }
    
    if(first) dataIn += inByte;
  }
}
