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
    Serial.println();
    Serial.println(dataIn);
    salto = false;
    dataIn = "";
  }
}

const int timeoutSerial = 20;
bool debug = false;
String pal = "";
void readBtSerial()
{
  while (Bluetooth.available() > 0) {
    inByte = Bluetooth.read();
    pal += inByte;
    if(debug) Serial.print(".");
    if(debug) Serial.print(inByte); 
    if(inByte == 'A' & !first)
    {
      inByte = ""; pal = "A";
      first = true;
      timeoutB = millis();
    }  
    else if((first & (millis() - timeoutB > timeoutSerial)) || inByte == 'Z' || inByte == '.')
    {
      first = false;
      salto = true;
      if(millis() - timeoutB > timeoutSerial) {dataIn = "";} // Invalido por timeout
      if(inByte != 'Z' & inByte != '.') {dataIn = "";} // Invalido por inconsistente
      if(!debug) Serial.print("Pal: ");Serial.println(pal);  
      break;    
    }
    else if(inByte == 'A')
    {
       dataIn = ""; // Invalido por sobre-escritura
       if(debug) Serial.println("omito");
       break;
    } else if((first & dataIn.length()>0) || (first & dataIn.length() == 0 & inByte == 's')) dataIn += String(inByte);
  }
  delay(1);
}
