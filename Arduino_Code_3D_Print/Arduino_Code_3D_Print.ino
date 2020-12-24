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
int speedDelay = 30;
int index = 0;
bool first = false, salto = false;
unsigned long timeoutB = 0;
char inByte;
String dataIn;


void setup() {
  servo01.attach(2); //5
  servo02.attach(5); //6
  servo03.attach(4); //7
  servo04.attach(7); //8
  servo05.attach(3); //9
  servo06.attach(6); //10
  Bluetooth.begin(9600); // Default baud rate of the Bluetooth module
  Bluetooth.setTimeout(1);
  delay(speedDelay);
  // Robot arm initial position
  servo1PPos = 90;
  servo01.write(servo1PPos);
  servo2PPos = 150;
  servo02.write(servo2PPos);
  servo3PPos = 35;
  servo03.write(servo3PPos);
  servo4PPos = 140;
  servo04.write(servo4PPos);
  servo5PPos = 85;
  servo05.write(servo5PPos);
  servo6PPos = 135;
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
      Serial.print("Data: "); Serial.println(dataIn);
      // If "Waist" slider has changed value - Move Servo 1 to position
      if (dataIn.startsWith("s1")) {
        Serial.println("Moviendo servo 1");
        String dataInS = dataIn.substring(2, dataIn.length()); // Extract only the number. E.g. from "s1120" to "120"
        servo1Pos = dataInS.toInt();  // Convert the string into integer
        // We use for loops so we can control the speed of the servo
        // If previous position is bigger then current position
        if (servo1PPos > servo1Pos) {
          for ( int j = servo1PPos; j >= servo1Pos; j--) {   // Run servo down
            servo01.write(j);
            delay(speedDelay);    // defines the speed at which the servo rotates
          }
        }
        // If previous position is smaller then current position
        if (servo1PPos < servo1Pos) {
          for ( int j = servo1PPos; j <= servo1Pos; j++) {   // Run servo up
            servo01.write(j);
            delay(speedDelay);
          }
        }
        servo1PPos = servo1Pos;   // set current position as previous position
        Serial.println(servo01.read());
      }
      
      // Move Servo 2
      if (dataIn.startsWith("s2")) {
        Serial.println("Moviendo servo 2");
        String dataInS = dataIn.substring(2, dataIn.length());
        servo2Pos = dataInS.toInt();
  
        if (servo2PPos > servo2Pos) {
          for ( int j = servo2PPos; j >= servo2Pos; j--) {
            servo02.write(j);
            delay(speedDelay);
          }
        }
        if (servo2PPos < servo2Pos) {
          for ( int j = servo2PPos; j <= servo2Pos; j++) {
            servo02.write(j);
            delay(speedDelay);
          }
        }
        servo2PPos = servo2Pos;
        Serial.println(servo02.read());
      }
      // Move Servo 3
      if (dataIn.startsWith("s3")) {
        Serial.println("Moviendo servo 3");
        String dataInS = dataIn.substring(2, dataIn.length());
        servo3Pos = dataInS.toInt();
        if (servo3PPos > servo3Pos) {
          for ( int j = servo3PPos; j >= servo3Pos; j--) {
            servo03.write(j);
            delay(speedDelay);
          }
        }
        if (servo3PPos < servo3Pos) {
          for ( int j = servo3PPos; j <= servo3Pos; j++) {
            servo03.write(j);
            delay(speedDelay);
          }
        }
        servo3PPos = servo3Pos;
        Serial.println(servo03.read());
      }
      // Move Servo 4
      if (dataIn.startsWith("s4")) {
        Serial.println("Moviendo servo 4");
        String dataInS = dataIn.substring(2, dataIn.length());
        servo4Pos = dataInS.toInt();
        if (servo4PPos > servo4Pos) {
          for ( int j = servo4PPos; j >= servo4Pos; j--) {
            servo04.write(j);
            delay(speedDelay);
          }
        }
        if (servo4PPos < servo4Pos) {
          for ( int j = servo4PPos; j <= servo4Pos; j++) {
            servo04.write(j);
            delay(speedDelay);
          }
        }
        servo4PPos = servo4Pos;
        Serial.println(servo04.read());
      }
      // Move Servo 5
      if (dataIn.startsWith("s5")) {
        Serial.println("Moviendo servo 5");
        String dataInS = dataIn.substring(2, dataIn.length());
        servo5Pos = dataInS.toInt();
        if (servo5PPos > servo5Pos) {
          for ( int j = servo5PPos; j >= servo5Pos; j--) {
            servo05.write(j);
            delay(speedDelay);
          }
        }
        if (servo5PPos < servo5Pos) {
          for ( int j = servo5PPos; j <= servo5Pos; j++) {
            servo05.write(j + 40);
            delay(speedDelay);
          }
        }
        servo5PPos = servo5Pos;
        Serial.println(servo05.read());
      }
      // Move Servo 6
      if (dataIn.startsWith("s6")) {
        Serial.println("Moviendo servo 6");
        String dataInS = dataIn.substring(2, dataIn.length());
        servo6Pos = corregirServo06(dataInS.toInt());
        if (servo6PPos > servo6Pos) {
          for ( int j = servo6PPos; j >= servo6Pos; j--) {
            servo06.write(j);
            delay(speedDelay);
          }
        }
        if (servo6PPos < servo6Pos) {
          for ( int j = servo6PPos; j <= servo6Pos; j++) {
            servo06.write(j);
            delay(speedDelay);
          }
        }
        servo6PPos = servo6Pos; 
        Serial.println(servo05.read());
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
      if (servo06SP[i] < servo06SP[i + 1]) {
        for ( int j = servo06SP[i]; j <= servo06SP[i + 1]; j++) {
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
void readBtSerial()
{
  while (Bluetooth.available() > 0) {
    inByte = Bluetooth.read();
    if((inByte == 's' || inByte == 'R' || inByte == 'S') & !first)
    {
      delay(1);
      dataIn += inByte; delay(1);
      first = true;
      timeoutB = millis();
    }  
    else if((first & (millis() - timeoutB > 6)) || (inByte == 's' || inByte == 'R' || inByte == 'S') || inByte == '.')
    {
      first = false;
      salto = true;
      break;    
    }else if(first)
    { 
      dataIn += inByte; delay(1);
    }else
    {
      /*
      Serial.println("****************Status*******************");
      Serial.print("Servo 1:"); Serial.println(servo01.read());
      Serial.print("Servo 2:"); Serial.println(servo02.read());
      Serial.print("Servo 3:"); Serial.println(servo03.read());
      Serial.print("Servo 4:"); Serial.println(servo04.read());
      Serial.print("Servo 5:"); Serial.println(servo05.read());
      Serial.print("Servo 6:"); Serial.println(servo06.read());
      Serial.println("****************************************"); 
      */
    }
    /*else
    { 
      Serial.print("Inbyte no valido: ");Serial.println(inByte);
    }*/
    
  }
}

int corregirServo06(int pos)
{
  return (pos-85)*5;
}
