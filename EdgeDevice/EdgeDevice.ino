#include <Arduino.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <stdint.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#include "Adafruit_VL53L0X.h"

#define PIN 12
#define PIXELCOUNT 22

#define SCAN_DURATION 10000

// Network settings
const char* ssid = "BaldhatsG8";
const char* password = "tms18team6";

const char* serverName = "http://192.168.20.80:8080/scanners/1/present";
WiFiClient client;
HTTPClient http;

// keeping track of tags[]
const int MAX_TAGS = 31;             // Maximum number of unique tags to store
String uniqueTags[MAX_TAGS];         // Array to store unique tags
int tagCount = 0;                    // Counter for unique tags
uint32_t lastSent;
String tagList = "";


bool sendingNeeded = false;

// LED Strip
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXELCOUNT, PIN, NEO_GRB+NEO_KHZ800);
Adafruit_VL53L0X lox = Adafruit_VL53L0X();


// connect to WiFi
void setup() {
  Serial.begin(115200);
  delay(200);

  WiFi.mode(WIFI_STA); //Optional
  // LED strip setup
  strip.begin();            
  strip.setBrightness(255);  // set the maximum LED intensity down to 20
  strip.clear();
  strip.show();             // Initialize all pixels to 'off'
  delay(200);

  ledShowConnecting();


  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }
  ledShowScanning();

  Serial.println("Connected");
  http.begin(client, serverName);

  Serial2.begin(115200,SERIAL_8N1,16,17, true);
  lox.begin();
}

// main loop
void loop() {
  int dist = getDist();
  ledShowWaiting();
  while (dist > 300 || dist < 0) {
    dist = getDist();
    delay(50);
  }

  performScanning();

  dist = getDist();
  while (dist < 300) {
    dist = getDist();
    delay(50);
  }
}

void performScanning() {
  ledShowScanning();
  resetTagList();
  bool complete = false;
  uint32_t startTime = millis();
  lastSent = millis();
  sendingNeeded = false;
  while (millis() - startTime < SCAN_DURATION) {
    bool tagListChanged = false;
    if(Serial2.available() >= 10) {
      String incomingTag = readTag();
      if (incomingTag.length() != 10) {
        continue;
      }
      int tagIndex = isTagAlreadyKnown(incomingTag);
      if (tagIndex == -1) {
        tagListChanged = true;
        startTime = millis()-1000;
        if (tagCount < MAX_TAGS) {
          uniqueTags[tagCount] = incomingTag;
          tagCount++;
        }
      }

      if (tagListChanged) {
        if ( millis() - lastSent > 1500) {
          int isComplete = sendTags();
          if (isComplete) {
            complete = true;
            break;
          }
        } else {
          sendingNeeded = true;
        }
        
      }
    }
  }
  if (sendingNeeded) {
    int isComplete = sendTags();
    if (isComplete) {
      complete = true;
    }
  }
  if (complete) {
    ledShowComplete();
  } else {
    ledShowMissing();
  }
}

int sendTags() {
  int isComplete = 0;
  lastSent = millis();
  tagList = tagListToJSON();
  sendingNeeded = false;

  Serial.println(tagList);

  http.addHeader("Content-Type", "application/json");
  if (http.POST(tagList) == 200) {
    String response = http.getString();
    Serial.println(response);
    isComplete = atoi(response.c_str());
  }
  return isComplete;
}

void resetTagList() {
  tagCount = 0;
  for (int i = 0; i < MAX_TAGS; i++) {
    uniqueTags[i] = "";
  }
}

String readTag() {
  String incomingTag = Serial2.readStringUntil('\x03');
  
  String number = String();
  for (int i = 0; i < incomingTag.length(); i++) {
    if (incomingTag[i] >= 48 && incomingTag[i] <= 57) {
      number += incomingTag[i];
    }
  }
  return number;
}

// Function to check if the tag is already present in the array
/**
* @brief Function to check if a tag is already present in the array
* @param tag : the tag to be checked
* @return index of the tag, if known. -1 otherwise
*/
int isTagAlreadyKnown(String tag) {
  for (int i = 0; i < tagCount; i++) {
    if (uniqueTags[i] == tag) {
      return i;
    }
  }
  return -1;
}

int getDist() {
  VL53L0X_RangingMeasurementData_t measurement;
  lox.rangingTest(&measurement, false);
  if (measurement.RangeStatus != 4) {
    return measurement.RangeMilliMeter;
  } else {
    return -1;
  }
}

/**
* @brief converts the uniqueTags array to a JSON object that can be sent to the server
* @return JSON object
**/
String tagListToJSON() {
  String jsonTagList = "";
  for (int i = 0; i < tagCount; i++) {
    jsonTagList = jsonTagList + "\"" + uniqueTags[i] + "\", ";
  }
  jsonTagList.remove(jsonTagList.length() - 2, 2);
  return "\t[" + jsonTagList + "]";
}

void ledShowWaiting() {
  for (int i = 0; i < PIXELCOUNT; i++){
    strip.setPixelColor(i, 0, 0, 255); // color channels are r g b
  }
  strip.show();
}

void ledShowMissing() {
  for (int i = 0; i < PIXELCOUNT; i++){
    strip.setPixelColor(i, 255, 0, 0); // color channels are r g b
  }
  strip.show();
}

void ledShowComplete() {
  for (int i = 0; i < PIXELCOUNT; i++){
    strip.setPixelColor(i, 0, 255, 0); // color channels are r g b
  }
  strip.show();
}

void ledShowScanning() {
  for (int i = 0; i < PIXELCOUNT; i++){
    strip.setPixelColor(i, 255, 255, 0); // color channels are r g b
  }
  strip.show();
}


void ledShowConnecting() {
  for (int i = 0; i < PIXELCOUNT; i++){
    strip.setPixelColor(i, 255, 0, 255); // color channels are r g b
  }
  strip.show();
}

void ledOff() {
  for (int i = 0; i < PIXELCOUNT; i++){
    strip.setPixelColor(i, 0, 0, 0); // color channels are r g b
  }
  strip.show();
}