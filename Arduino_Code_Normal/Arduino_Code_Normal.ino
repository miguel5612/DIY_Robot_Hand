/*        
       DIY Arduino Robot Arm Smartphone Control  
        by Dejan, www.HowToMechatronics.com  
*/

#include <SoftwareSerial.h>
#include <Servo.h>

Servo servo01;
Servo servo02;
Servo servo03;
Servo servo04;
Servo servo05;
Servo servo06;

SoftwareSerial Bluetooth(9, 8); // Arduino(RX, TX) - HC-05 Bluetooth (TX, RX) 3 4

int servo1Pos, servo2Pos, servo3Pos, servo4Pos, servo5Pos, servo6Pos; // current position
int servo1PPos, servo2PPos, servo3PPos, servo4PPos, servo5PPos, servo6PPos; // previous position
int servo01SP[50], servo02SP[50], servo03SP[50], servo04SP[50], servo05SP[50], servo06SP[50]; // for storing positions/steps
int speedDelay = 15;
int timeDelayAfterMov = 500;
int index = 0;
bool first = false, salto = false;
unsigned long timeoutB = 0;
char inByte;
String dataIn, dataInS;


void setup() {
  servo01.attach(2); //5 Girar todo el brazo
  servo02.attach(3); //6 Base
  servo03.attach(4); //7 
  servo04.attach(7); //8
  servo05.attach(6); //9 
  servo06.attach(5); //10 Base
  
  
  Bluetooth.begin(9600); // Default baud rate of the Bluetooth module
  Bluetooth.setTimeout(1);
  delay(speedDelay);
  // Robot arm initial position
  servo1PPos = 90;
  servo01.write(servo1PPos);
  servo2PPos = 150;
  servo02.write(servo2PPos);
  servo3PPos = 90;
  servo03.write(servo3PPos);
  servo4PPos = 140;
  servo04.write(servo4PPos);
  servo5PPos = 85;
  servo05.write(servo5PPos);
  servo6PPos = 180-servo2PPos;
  servo06.write(servo6PPos);
  Serial.begin(115200);
  Serial.println("Brazo robot inicializado");
}

void loop() {
  // Check for incoming data
  /*
  Serial.println("****************Status*******************");
  Serial.print("Servo 1:"); Serial.println(servo1PPos);
  Serial.print("Servo 2:"); Serial.println(servo2PPos);
  Serial.print("Servo 3:"); Serial.println(servo3PPos);
  Serial.print("Servo 4:"); Serial.println(servo4PPos);
  Serial.print("Servo 5:"); Serial.println(servo5PPos);
  Serial.print("Servo 6:"); Serial.println(servo6PPos);
  Serial.println("****************************************");
  */
  readBtSerial();
  if (dataIn.length()>2 & dataIn!="" & salto) {
      //Serial.print("Data: "); Serial.println(dataIn);
      // If "Waist" slider has changed value - Move Servo 1 to position

      
      if (dataIn.startsWith("s1")) {
        dataInS = dataIn.substring(2, dataIn.length()); // Extract only the number. E.g. from "s1120" to "120"
        servo1Pos = corregirServo01(dataInS.toInt());  // Convert the string into integer
        // We use for loops so we can control the speed of the servo
        // If previous position is bigger then current position
        if(servo1Pos != servo1PPos){
         Serial.print("Moviendo servo 1 ");
         Serial.print(servo1PPos); Serial.print(" - ");
          Serial.println(servo1Pos);
          
          if (servo1PPos > servo1Pos) {
            for ( int j = servo1PPos; j >= servo1Pos; j--) {   // Run servo down
              servo01.write(j);
              Serial.print(j);Serial.print(".");
              delay(speedDelay);    // defines the speed at which the servo rotates
            }
          }
          // If previous position is smaller then current position
          if (servo1PPos < servo1Pos) {
            for ( int j = servo1PPos; j <= servo1Pos; j++) {   // Run servo up
              servo01.write(j);
              Serial.print(j);Serial.print(".");
              delay(speedDelay);
            }
          }
        }
        servo1PPos = servo1Pos;   // set current position as previous position
        Serial.println(servo01.read());
        delay(timeDelayAfterMov);
      }
      
      // Move Servo 2
      if (dataIn.startsWith("s2")) {
        dataInS = dataIn.substring(2, dataIn.length());
        servo2Pos = corregirServo02(dataInS.toInt());
        if(servo2Pos != servo2PPos){
         Serial.print("Moviendo servo 2 ");
         Serial.print(servo2PPos); Serial.print(" - ");
         Serial.println(servo2Pos);
         
         if (servo2PPos > servo2Pos) {
            for ( int j = servo2PPos; j > servo2Pos; j--) {
              servo02.write(j);
              servo06.write(180-j);
              Serial.print(j);Serial.print(".");
              delay(speedDelay);
            }
          }
          if (servo2PPos < servo2Pos) {
            for ( int j = servo2PPos; j < servo2Pos; j++) {
              servo02.write(j);
              servo06.write(180-j);
              Serial.print(j);Serial.print(".");
              delay(speedDelay);
            }
          } 
        }
        servo2PPos = servo2Pos;
        Serial.println(servo02.read());
        delay(timeDelayAfterMov);
      }
      // Move Servo 3
      if (dataIn.startsWith("s3")) {
        dataInS = dataIn.substring(2, dataIn.length());
        servo3Pos = corregirServo03(dataInS.toInt());
        if(servo3Pos != servo3PPos){
          Serial.print("Moviendo servo 3 ");
          Serial.print(servo3PPos); Serial.print(" - ");
          Serial.println(servo3Pos);
          
          if (servo3PPos > servo3Pos) {
            for ( int j = servo3PPos; j > servo3Pos; j--) {
              servo03.write(j);
              Serial.print(j);Serial.print(".");
              delay(speedDelay);
            }
          }
          if (servo3PPos < servo3Pos) {
            for ( int j = servo3PPos; j < servo3Pos; j++) {
              servo03.write(j);
              Serial.print(j);Serial.print(".");
              delay(speedDelay);
            }
          }
        }
        servo3PPos = servo3Pos;
        Serial.println(servo03.read());
        delay(timeDelayAfterMov);
      }
      // Move Servo 4
      if (dataIn.startsWith("s4")) {
        dataInS = dataIn.substring(2, dataIn.length());
        servo4Pos = corregirServo04(dataInS.toInt());
        if(servo4Pos != servo4PPos){
          Serial.print("Moviendo servo 4 ");
          Serial.print(servo2PPos); Serial.print(" - ");
          Serial.println(servo2Pos);
          
          if (servo4PPos > servo4Pos) {
            for ( int j = servo4PPos; j >= servo4Pos; j--) {
              servo04.write(j);
              Serial.print(j);Serial.print(".");
              delay(speedDelay);
            }
          }
          if (servo4PPos < servo4Pos) {
            for ( int j = servo4PPos; j <= servo4Pos; j++) {
              servo04.write(j);
              Serial.print(j);Serial.print(".");
              delay(speedDelay);
            }
          }
        }
        servo4PPos = servo4Pos;
        Serial.println(servo04.read());
        delay(timeDelayAfterMov);
      }
      // Move Servo 5
      if (dataIn.startsWith("s5")) {
        dataInS = dataIn.substring(2, dataIn.length());
        servo5Pos = corregirServo05(dataInS.toInt());
        if(servo5Pos != servo5PPos){
          Serial.print("Moviendo servo 4 ");
          Serial.print(servo5PPos); Serial.print(" - ");
          Serial.println(servo5Pos);
        
          if (servo5PPos > servo5Pos) {
            for ( int j = servo5PPos; j >= servo5Pos; j--) {
              servo05.write(j);
              Serial.print(j);Serial.print(".");
              delay(speedDelay);
            }
          }
          if (servo5PPos < servo5Pos) {
            for ( int j = servo5PPos; j <= servo5Pos; j++) {
              servo05.write(j);
              Serial.print(j);Serial.print(".");
              delay(speedDelay);
            }
          }
        }
        servo5PPos = servo5Pos;
        Serial.println(servo05.read());
        delay(timeDelayAfterMov);
      }
      // If button "SAVE" is pressed
      if (dataIn.startsWith("SAVE")) {
        Serial.println("Guardando");
        servo01SP[index] = servo1PPos;  // save position into the array
        servo02SP[index] = servo2PPos;
        servo03SP[index] = servo3PPos;
        servo04SP[index] = servo4PPos;
        servo05SP[index] = servo5PPos;
        servo06SP[index] = servo6PPos;
        index++;                        // Increase the array index
      }
      // If button "RUN" is pressed
      if (dataIn.startsWith("RUN")) {
        Serial.println("Corriendo");
        runservo();  // Automatic mode - run the saved steps 
      }
      // If button "RESET" is pressed
      if ( dataIn == "RESET") {
        memset(servo01SP, 0, sizeof(servo01SP)); // Clear the array data to 0
        memset(servo02SP, 0, sizeof(servo02SP));
        memset(servo03SP, 0, sizeof(servo03SP));
        memset(servo04SP, 0, sizeof(servo04SP));
        memset(servo05SP, 0, sizeof(servo05SP));
        memset(servo06SP, 0, sizeof(servo06SP));
        index = 0;  // Index to 0
      }
    } 
    salto = false;
    dataIn = "";
}

// Automatic mode custom function - run the saved steps
void runservo() {
  while (dataIn != "RESET") {   // Run the steps over and over again until "RESET" button is pressed
    for (int i = 0; i <= index - 2; i++) {  // Run through all steps(index)
      if (Bluetooth.available() > 0) {      // Check for incomding data
        dataIn = Bluetooth.readString();
        if ( dataIn == "PAUSE") {           // If button "PAUSE" is pressed
          while (dataIn != "RUN") {         // Wait until "RUN" is pressed again
            if (Bluetooth.available() > 0) {
              dataIn = Bluetooth.readString();
              if ( dataIn == "RESET") {     
                break;
              }
            }
          }
        }
        // If speed slider is changed
        if (dataIn.startsWith("ss")) {
          String dataInS = dataIn.substring(2, dataIn.length());
          speedDelay = dataInS.toInt(); // Change servo speed (delay time)
        }
      }
      // Servo 1
      if (servo01SP[i] == servo01SP[i + 1]) {
      }
      if (servo01SP[i] > servo01SP[i + 1]) {
        for ( int j = servo01SP[i]; j >= servo01SP[i + 1]; j--) {
          servo01.write(j);
          delay(speedDelay);
        }
      }
      if (servo01SP[i] < servo01SP[i + 1]) {
        for ( int j = servo01SP[i]; j <= servo01SP[i + 1]; j++) {
          servo01.write(j);
          delay(speedDelay);
        }
      }

      // Servo 2
      if (servo02SP[i] == servo02SP[i + 1]) {
      }
      if (servo02SP[i] > servo02SP[i + 1]) {
        for ( int j = servo02SP[i]; j >= servo02SP[i + 1]; j--) {
          servo02.write(j);
          delay(speedDelay);
        }
      }
      if (servo02SP[i] < servo02SP[i + 1]) {
        for ( int j = servo02SP[i]; j <= servo02SP[i + 1]; j++) {
          servo02.write(j);
          delay(speedDelay);
        }
      }

      // Servo 3
      if (servo03SP[i] == servo03SP[i + 1]) {
      }
      if (servo03SP[i] > servo03SP[i + 1]) {
        for ( int j = servo03SP[i]; j >= servo03SP[i + 1]; j--) {
          servo03.write(j);
          delay(speedDelay);
        }
      }
      if (servo03SP[i] < servo03SP[i + 1]) {
        for ( int j = servo03SP[i]; j <= servo03SP[i + 1]; j++) {
          servo03.write(j);
          delay(speedDelay);
        }
      }

      // Servo 4
      if (servo04SP[i] == servo04SP[i + 1]) {
      }
      if (servo04SP[i] > servo04SP[i + 1]) {
        for ( int j = servo04SP[i]; j >= servo04SP[i + 1]; j--) {
          servo04.write(j);
          delay(speedDelay);
        }
      }
      if (servo04SP[i] < servo04SP[i + 1]) {
        for ( int j = servo04SP[i]; j <= servo04SP[i + 1]; j++) {
          servo04.write(j);
          delay(speedDelay);
        }
      }

      // Servo 5
      if (servo05SP[i] == servo05SP[i + 1]) {
      }
      if (servo05SP[i] > servo05SP[i + 1]) {
        for ( int j = servo05SP[i]; j >= servo05SP[i + 1]; j--) {
          servo05.write(j);
          delay(speedDelay);
        }
      }
      if (servo05SP[i] < servo05SP[i + 1]) {
        for ( int j = servo05SP[i]; j <= servo05SP[i + 1]; j++) {
          servo05.write(j);
          delay(speedDelay);
        }
      }

      // Servo 6
      if (servo06SP[i] == servo06SP[i + 1]) {
      }
      if (servo06SP[i] > servo06SP[i + 1]) {
        for ( int j = servo06SP[i]; j >= servo06SP[i + 1]; j--) {
          servo06.write(j);
          delay(speedDelay);
        }
      }      
    }
  }
}

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0] - 1, strIndex[1]) : "";
}

// Con estas funciones se consigue que el valor que llega en pos, vaya al servo con un rango de entre 0 y 180 grados.
int corregirServo01(int pos)
{
  return pos;
  return (pos-23)*1.2;
}


int corregirServo02(int pos)
{
  return (pos-100)*2.3;
}


int corregirServo03(int pos)
{
  return pos;
  return (pos-34)*1.7;
}


int corregirServo04(int pos)
{
  return pos;
  return (pos-4)*1.2;
}


int corregirServo05(int pos)
{
  return pos;
  return (pos-28)*1.4;
}

int corregirServo06(int pos)
{
  return pos;
  return (pos-85)*5;
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
      if(debug) {Serial.print("Pal: ");Serial.println(pal);}
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
