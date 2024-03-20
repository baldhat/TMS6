//#include <ESP8266WiFi.h>
//#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <stdint.h>
#include <Adafruit_NeoPixel.h>

#define NEO_PIXEL_PIN D5
#define PIXELCOUNT 6

#define TAG_REMOVED_TRESHOLD 10000

// Network settings
const char* ssid = "BaldhatsG8";
const char* password = "tms18team6";

const char* serverName = "http://192.168.238.80:8080/scanners/1/present";
WiFiClient client;
HTTPClient http;

// keeping track of tags[]
const int MAX_TAGS = 20;             // Maximum number of unique tags to store
String uniqueTags[MAX_TAGS];         // Array to store unique tags
uint32_t tagTime[MAX_TAGS] = { 0 };  // Latest time Tag was scanned
int tagCount = 0;                    // Counter for unique tags
uint8_t currentMaxTags = 0;          // keeps track of the number of tags there should be in the bag

String incomingTag = "";
String tagList = "";

// LED Strip
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXELCOUNT, NEO_PIXEL_PIN, NEO_GRB+NEO_KHZ400);

// connect to WiFi
void setup() {
  Serial.begin(115200);
  delay(200);

  WiFi.mode(WIFI_STA); //Optional
  // LED strip setup
  strip.begin();            
  strip.setBrightness(20);  // set the maximum LED intensity down to 20
  strip.show();             // Initialize all pixels to 'off'

  WiFi.begin(ssid, password);
  ledShowConnecting();
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  ledOff();

  Serial.println("Connected");
  http.begin(client, serverName);

  Serial2.begin(115200,SERIAL_8N1,16,17, true);
}

// main loop
void loop() {
  bool tagListChanged = false;
  if(Serial2.available() >= 10) {
    uint32_t arrivalTime = millis();
    incomingTag = Serial2.readStringUntil('\x03');
    
    String number = String();
    for (int i = 0; i < incomingTag.length(); i++) {
      if (incomingTag[i] >= 48 && incomingTag[i] <= 57) {
        number += incomingTag[i];
      }
    }
   
    if (number.length() != 10) {
      return;
    }

    incomingTag = number;
    int tagIndex = isTagAlreadyKnown(incomingTag);
    if (tagIndex == -1) {
      tagListChanged = true;
      if (tagCount < MAX_TAGS) {
        uniqueTags[tagCount] = incomingTag;
        tagTime[tagCount] = arrivalTime;
        tagCount++;
      } else {
        // If the array is full, remove the oldest tag and add the new one
        for (int i = 0; i < MAX_TAGS - 1; i++) {
          uniqueTags[i] = uniqueTags[i + 1];
          tagTime[i] = tagTime[i + 1];
        }
        uniqueTags[MAX_TAGS - 1] = incomingTag;
        tagTime[MAX_TAGS - 1] = arrivalTime;
      }

    } else {
      tagTime[tagIndex] = arrivalTime;
    }
  }

  if (checkIfTagsRemoved()) {
    tagListChanged = true;
  }

  if (tagListChanged) {
    tagList = tagListToJSON();

    Serial.println(tagList);

    http.addHeader("Content-Type", "application/json");
    http.POST(tagList);
  }
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


/**
* @brief checks if any previously present tag is not there anymore.
*        removes missing tags from uniqueTags array.
* @return true if tags are missing
**/
bool checkIfTagsRemoved() {
  uint32_t currentTime = millis();
  bool tagRemoved = false;
  for (int i = 0; i < tagCount; i++) {
    if (currentTime - tagTime[i] > TAG_REMOVED_TRESHOLD) {
      tagRemoved = true;
      tagCount--;
      for (int j = i; j < MAX_TAGS - 1; j++) {
        uniqueTags[j] = uniqueTags[j + 1];
        tagTime[j] = tagTime[j + 1];
      }
    }
  }
  return tagRemoved;
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