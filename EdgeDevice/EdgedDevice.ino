#include <SoftwareSerial.h>

#define BAUD_RATE 9600

String incomingString = "";
String tagID = "0000000000";
SoftwareSerial softSer = SoftwareSerial(13, 12, 1);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(BAUD_RATE);
  softSer.begin(BAUD_RATE);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (softSer.available() > 0) {
    incomingString = softSer.readStringUntil('\x03');
    /*
    incomingString = softSer.readStringUntil('\n');
    if (softSer.available() == 1) {
      softSer.read();
    }
    incomingString.remove(0, 2);
    incomingString.remove(10, 1);
    */
    
    // removes the uneccessary leading and trailing special characters
    incomingString.remove(0, 1);
    incomingString.remove(10, 3);
    if (incomingString.length() == 10) {
      Serial.println(incomingString);
    }
  }
}
