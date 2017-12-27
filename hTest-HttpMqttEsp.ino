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
int serialValue = 0;
int serialValueConv = 0;
int pinOut04 = D4;
int pinOut03 = D3;

void setup() {
  pinMode(pinOut04, OUTPUT);
  pinMode(pinOut03, OUTPUT); 
  
  Serial.begin(115200);
  WiFi.begin(ssid, password);                     // Initialize WiFi Connection 
 
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connecting to WiFi..");
  }
  
  Serial.println("Connected to the WiFi network");
 
}

void loop() {
   
  if(WiFi.status() == WL_CONNECTED)  {            
    
      /* Power and Internet Connection */
      
      digitalWrite(pinOut04, HIGH);                 
      
      /* Serial Input Tests */
      
      serialValue = Serial.read();                  // Serial Input   
      serialValueConv = serialValue - 48;           // Temporary Conversion from ASCII           
                
      Serial.print("From Serial: ");
      Serial.println(serialValueConv);
              
              if (serialValueConv == 1) {
                  digitalWrite(pinOut03, HIGH);  
                 
              }
              else if (serialValueConv == 2)  {
                  digitalWrite(pinOut03, LOW);        
              }

                          
    delay(1000);        
  }
  
  else  {
    Serial.println("Error in WiFi connection");   // Throw error in connection
  }
}
