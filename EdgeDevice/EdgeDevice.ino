#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

// Network settings
const char* ssid = "Baldhats";
const char* password = "tms18team6";

const char* serverName = "http://192.168.179.80:8080/scanners/1/present";
WiFiClient client;
HTTPClient http;

// keeping track of tags[]
const int MAX_TAGS = 10;      // Maximum number of unique tags to store
String uniqueTags[MAX_TAGS];  // Array to store unique tags
int tagCount = 0;             // Counter for unique tags

String incomingTag = "";
String tagList = "";

// connect to WiFi
void setup() {
    Serial.begin(9600);
    delay(200);
    Serial.swap();
    delay(200);
    pinMode(LED_BUILTIN, OUTPUT);
    /*
    WiFi.enableInsecureWEP();
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
    digitalWrite(LED_BUILTIN, HIGH);
    http.begin(client, serverName);
    */
}

// main loop
void loop() {
  if(Serial.available() > 10) {
    uint32_t arrivalTime = millis();
    incomingTag = Serial.readStringUntil('\x03');
    incomingTag.remove(0, 1);
    incomingTag.remove(10, 3);
    
    Serial.swap();
    delay(10);
    Serial.println(incomingTag);
    Serial.flush();
    Serial.swap();
    
    if (!isTagAlreadyKnown(incomingTag)) {
      if (tagCount < MAX_TAGS) {
        uniqueTags[tagCount] = incomingTag;
        tagCount++;
      } else {
        // If the array is full, remove the oldest tag and add the new one
        for (int i = 0; i < MAX_TAGS - 1; i++) {
          uniqueTags[i] = uniqueTags[i + 1];
        }
        uniqueTags[MAX_TAGS - 1] = incomingTag;
      }
      
      tagList = tagListToJSON();
      
      Serial.swap();
      delay(10);
      Serial1.println(tagList);
      Serial1.flush();
      Serial.swap();
      
      /* 
      http.addHeader("Content-Type", "application/json");
      if (http.POST(tagList) != 200) {
        digitalWrite(LED_BUILTIN, LOW);
        delay(40);
        digitalWrite(LED_BUILTIN, HIGH);
      }
      */
    }
  }
}

// Function to check if the tag is already present in the array
boolean isTagAlreadyKnown(String tag) {
  for (int i = 0; i < tagCount; i++) {
    if (uniqueTags[i] == tag) {
      return true;
    }
  }
  return false;
}

/**
* @brief converts the uniqueTags array to a JSON object that can be sent to the server
* @return JSON object
**/
String tagListToJSON() {
  String jsonTagList = "";
  for (int i = 0; i < tagCount; i++) {
    jsonTagList = jsonTagList + ", \"" + uniqueTags[i] + "\"";
  }

  return "\t[" + jsonTagList + "]";
}
