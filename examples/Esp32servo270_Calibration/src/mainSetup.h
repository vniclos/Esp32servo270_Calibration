#ifndef MYSETUP_H
#define MYSETUP_H
#include <Arduino.h>
#include "Esp32servo270.h"
#include "mainDefines.h"
#include <WiFi.h>
#include "web_handle.h"
void fncSetupWifi()
{
  digitalWrite(g_pin_ledblue, 1);
  Serial.println();

  Serial.print("Setting soft-AP configuration ... ");
  WiFi.disconnect();   // added to start with the wifi off, avoid crashing
  WiFi.mode(WIFI_OFF); // added to start with the wifi off, avoid crashing
  WiFi.mode(WIFI_AP);

  Serial.println(WiFi.softAPConfig(g_WifiIpLocalP, g_WifiIpGateway, g_WifiIpMask) ? "Ready" : "Failed!");

  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP(g_WifiSsid, g_WifiPwd) ? "Ready" : "Failed!");
  // WiFi.softAP(ssid);
  // WiFi.softAP(ssid, passphrase, channel, ssdi_hidden, max_connection)

  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());

  digitalWrite(g_pin_ledblue, 0);
}

void fncSetupWebserver()
{
  digitalWrite(g_pin_ledblue, HIGH);
  Serial.println("fnc_Web_Setup in");
  // g_WebServer.on("/fncStatusGet", handleSTATUSGET);
  g_WebServer.on("/", handleROOT);
  g_WebServer.on("/js.js", handleJS);
  g_WebServer.on("/css.css", handleCSS);
  g_WebServer.on("/atach", handleAtach);
  g_WebServer.on("/detach", handleDetach);
  g_WebServer.on("/move", handleMove);
  g_WebServer.on("/status", handleStatus);

  g_WebServer.on("/turnmicro", handleTurnMicro);
  g_WebServer.on("/turndegree", handleTurnDegree);
  g_WebServer.on("/template", handleTemplate);

  g_WebServer.onNotFound(handleNotFound);
  g_WebServer.begin();
  Serial.println("fnc_Web_Setup out");
  digitalWrite(g_pin_ledblue, LOW);
}

#endif