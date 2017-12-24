/*
  HTTP Post
*/

#include <ESP8266WiFi.h>                          // WiFi Header  
#include <PubSubClient.h>                         // MQTT Library                         
#include <ESP8266HTTPClient.h>                    // HTTP Header
#include <ArduinoJson.h>                          // JSON formatter header | https://bblanchon.github.io/ArduinoJson/
WiFiClient espClient;                             // Initiate WiFiClient Library

const char* ssid = "ZTEH108N_3C01E";
const char* password =  "X9425TE9";
char serialValue = 0;
int pinOut04 = D4;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);                     // Initialize WiFi Connection 
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
 
}

void loop() {
   
  if(WiFi.status()== WL_CONNECTED)  {            
    
    digitalWrite(pinOut04, HIGH);                 // Power Indicator
    serialValue = Serial.read();                  // Serial Input
      Serial.print("From Serial: ");
      Serial.println(serialValue);
 
    delay(1000);        
  }
  
  else  {
    Serial.println("Error in WiFi connection");   // Throw error in connection
  }
}
