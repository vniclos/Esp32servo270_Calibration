#include <Arduino.h>
#include <WiFi.h>
#include "Esp32servo270.h"
#include "mainDefines.h"
#include "mainSetup.h"


// create servo object to control a servo, remember you need use 
Esp32servo270 myservo;  
void setup() {
  Serial.begin(115200);
  pinMode(g_pin_ledblue,OUTPUT); // pin blue on devkit
  fncSetupWifi();
  fncSetupWebserver();
}
void loop() {

  g_WebServer.handleClient();
 
}

