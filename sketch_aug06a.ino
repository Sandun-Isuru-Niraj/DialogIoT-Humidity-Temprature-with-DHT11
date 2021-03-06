#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "dht.h"
#define dht_apin 2
#include <ArduinoJson.h>

dht DHT;
void setup() {
  Serial.begin(115200);                                  //Serial connection

  WiFi.begin("Your SSID", "Your Password");   //WiFi connection
 
  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion
 
    delay(500);
    Serial.println("Waiting for connection");
 
  }

}

void loop() {
  DHT.read11(dht_apin);
  int t = DHT.temperature;
  int h = DHT.humidity;

   if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
    
  StaticJsonBuffer<300> JSONbuffer;   //Declaring static JSON buffer
  JsonObject& JSONencoder = JSONbuffer.createObject();

  JSONencoder["eventName"] = <<Your Event Name>>;
  JSONencoder["status"] = "none";
  JSONencoder["temp"] = String(t);
  JSONencoder["hum"] = String(h);
  JSONencoder["mac"] = <<Your Device MAC Address>>; //Obtain from DialogIoT Developer Account
  char JSONmessageBuffer[300];
  JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
  //Serial.println(JSONmessageBuffer);
  
 
   HTTPClient http;    //Declare object of class HTTPClient
 
   http.begin("http://52.221.141.22:8080/api/pcs/Generic_brand_628GENERIC_DEVICE3");      //Specify request destination
   http.addHeader("Content-Type", "application/json");  //Specify content-type header
 
   int httpCode = http.POST(JSONmessageBuffer);//Send the request
   String payload = http.getString();                  //Get the response payload


    Serial.println(payload);
   http.end();  //Close connection
 
 }else{
 
    Serial.println("Error in WiFi connection");   
 
 }
 
  delay(200);  //Send a request every 30 seconds
}
