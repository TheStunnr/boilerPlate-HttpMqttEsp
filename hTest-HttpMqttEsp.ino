#include <ESP8266WiFi.h>

// MQTT Library
#include <PubSubClient.h>                         

// HTTP headers
#include <ESP8266HTTPClient.h>

// JSON formatter header 
// https://bblanchon.github.io/ArduinoJson/
#include <ArduinoJson.h>
 
const char* ssid = "ZTEH108N_3C01E";
const char* password =  "X9425TE9";
//const char* mqttServer = "m11.cloudmqtt.com";
//const int mqttPort = 19829;
//const char* mqttUser = "obzsbsrb";
//const char* mqttPassword = "5g_aGdvOLQu0";
 
WiFiClient espClient;
//PubSubClient client(espClient);

int pinOut00 = D0;
int pinOut01 = D1;
int pinOut02 = D2;
int pinOut03 = D3;
int pinOut04 = D4;
int pinOut05 = D5;
int pinOut06 = D6;
int pinOut07 = D7;
int pinOut08 = D8;
int pinInA0 = A0;
int valueInA0 = 0;
int valueInA1 = 0;
int valueInA2 = 0;
float mv, temp;
int bpm, vib;

void setup() {
  
  pinMode(pinOut00, OUTPUT); // Setting Pin 13 as output
  pinMode(pinOut01, OUTPUT); // Setting Pin 12 as output
  pinMode(pinOut02, OUTPUT);
  pinMode(pinOut03, OUTPUT);
  pinMode(pinOut04, OUTPUT);
  pinMode(pinOut05, OUTPUT);
  pinMode(pinInA0, INPUT); // Setting analog pin A0 to input 

  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
 
//  client.setServer(mqttServer, mqttPort);
// 
//  while (!client.connected()) {
//    Serial.println("Connecting to MQTT...");
// 
//    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
// 
//      Serial.println("connected to mqtt");  
// 
//    } else {
// 
//      Serial.print("failed with state ");
//      Serial.print(client.state());
//      delay(2000);
// 
//    }
//  }
}
 
void loop() {

  //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){            

  digitalWrite(pinOut04, HIGH); // Power Indicator
  digitalWrite(pinOut00, LOW); // Y0 - pulse
  digitalWrite(pinOut01, LOW);
  digitalWrite(pinOut02, LOW);

  if ( digitalRead(pinOut00) == LOW && digitalRead(pinOut01) == LOW && digitalRead(pinOut02) == LOW ) {
    valueInA0 = analogRead(pinInA0); // Sensing voltage input in pin A0 and converting to integer values (0 - 1023)
    Serial.print("pulse: ");
      Serial.println(valueInA0);
      bpm = valueInA0;
    }
 
  digitalWrite(pinOut00, HIGH); // Y1 - temperature
  digitalWrite(pinOut01, LOW);
  digitalWrite(pinOut02, LOW);
  
  if ( digitalRead(pinOut00) == HIGH && digitalRead(pinOut01) == LOW && digitalRead(pinOut02) == LOW ) {
    valueInA1 = analogRead(pinInA0); // Sensing voltage input in pin A0 and converting to integer values (0 - 1023)
    mv = ( valueInA1/1024.0)*5000; 
    temp = (mv/10)/1.7;
    Serial.print("TEMPERATURE = ");
    Serial.print(temp);
    Serial.print("*C");
    Serial.println();
  }

  digitalWrite(pinOut00, LOW); // Y2 - vibrations
  digitalWrite(pinOut01, HIGH);
  digitalWrite(pinOut02, LOW);

  if ( digitalRead(pinOut00) == LOW && digitalRead(pinOut01) == HIGH && digitalRead(pinOut02) == LOW ) {
    valueInA2 = analogRead(pinInA0); // Sensing voltage input in pin A0 and converting to integer values (0 - 1023)
    Serial.print("vibrations: ");    
    Serial.println(valueInA2);
    vib = valueInA2;
  }

    // Memory pool for JSON object tree.
    //
    // Inside the brackets, 500 is the size of the pool in bytes.
    // If the JSON object is more complex, you need to increase that value.
    StaticJsonBuffer<500> jsonBuffer;

    // Create the root of the object tree.
    //
    // It's a reference to the JsonObject, the actual bytes are inside the
    // JsonBuffer with all the other nodes of the object tree.
    // Memory is freed when jsonBuffer goes out of scope.
    JsonObject& root = jsonBuffer.createObject();    

    // Add a nested object.
    JsonObject& payload = root.createNestedObject("payload");

    // TODO: Initialize values for valueInpA0FrmD#
    
    payload["temperature"] = temp;
    payload["pulse_rate"] = bpm;
    payload["vibrations"] = vib;

    //JSON output
    //{
    //  "payload": {
    //      "temperaturemD1": "",
    //      "pulse_rate": "",
    //      "vibrations": ""
    //  }

    String data;

    //print JSON to data
    root.printTo(data);
    
    //Declare object of class HTTPClient
    HTTPClient http;

    //Specify request destination
    http.begin("https://mdm-hackathon.herokuapp.com/esp", "08 3b 71 72 02 43 6e ca ed 42 86 93 ba 7e df 81 c4 bc 62 30");

    //Specify content-type header
    http.addHeader("Content-Type", "application/json");

    //Send the request
    int httpCode = http.POST(data);
    
    //Get the response payload
    String payload1 = http.getString();
    
    Serial.println();
    
    //Print HTTP return code
    Serial.print(httpCode);
    Serial.print(" ");
    //Print request response payload
    Serial.println(payload1);
    
    //Close connection
    http.end();

    delay(100);

    payload["temperature"] = temp;

    String data2;

    //print JSON to data
    root.printTo(data2);
    
    //Specify request destination
    http.begin("https://mdm-hackathon.herokuapp.com/vibration", "08 3b 71 72 02 43 6e ca ed 42 86 93 ba 7e df 81 c4 bc 62 30");

    //Specify content-type header
    http.addHeader("Content-Type", "application/json");

    //Send the request
    httpCode = http.POST(data2);
    
    //Get the response payload
    String payload2 = http.getString();
    
    Serial.println();
    
    //Print HTTP return code
    Serial.print(httpCode);
    Serial.print(" ");
    //Print request response payload
    Serial.println(payload2);
    
    //Close connection
    http.end();
 }else{
    //throw error in connection
    Serial.println("Error in WiFi connection");
 }
 
  delay(100); 

}

