#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <stdint.h>

#define TAG_REMOVED_TRESHOLD 10000

// Network settings
const char* ssid = "Baldhats";
const char* password = "tms18team6";

const char* serverName = "http://169.254.81.133:8080/scanners/1/present";
WiFiClient client;
HTTPClient http;

// keeping track of tags[]
const int MAX_TAGS = 10;             // Maximum number of unique tags to store
String uniqueTags[MAX_TAGS];         // Array to store unique tags
uint32_t tagTime[MAX_TAGS] = { 0 };  // Latest time Tag was scanned
int tagCount = 0;                    // Counter for unique tags

String incomingTag = "";
String tagList = "";

// connect to WiFi
void setup() {
  Serial.begin(115200);
  delay(200);

  WiFi.enableInsecureWEP();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.swap();
  delay(200);
  http.begin(client, serverName);
}

// main loop
void loop() {
  bool tagListChanged = false;
  if(Serial.available() >= 10) {
    uint32_t arrivalTime = millis();
    incomingTag = Serial.readStringUntil('\x03');
    
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
    /*
    Serial.swap();
    delay(10);
    Serial.println(incomingTag);
    Serial.flush();
    Serial.swap();
    */
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

    Serial.swap();
    //delay(10);
    Serial.println(tagList);
    Serial.flush();
    Serial.swap();

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
