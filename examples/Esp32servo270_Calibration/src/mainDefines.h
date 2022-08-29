// ==========================================================================
// 16 servo objects can be created on the ESP32
// prefix g_ is used for global variables and objects
// prefix g_attach are prefix for attach servo parameters
// prefix g_wifi are for config wifi acces point parameters
// Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33 
// ==========================================================================

#ifndef MAINDEFINES_H
#define MAINDEFINES_H
#include <Arduino.h>
#include <WebServer.h>
#include <Esp32servo270.h>
const char *g_WifiSsid = "servo_calibration";
const char *g_WifiPwd = "123456789";
IPAddress g_WifiIpLocalP(192, 168, 1, 1);
IPAddress g_WifiIpGateway(192, 168, 1, 1);
IPAddress g_WifiIpMask(255, 255, 255, 0);
// int attach(int attachEsp32Pin, int attachDatasheetUsMin, int attachDatasheetUsMax, int attachDatasheetDegreeMax, int attachMoveDegreeMin, int attachMoveDegreeMax); // as above but also sets min and max values for writes. and max hardware degree admisible

int g_pin_ledblue = 2;

int g_AttachEsp32Pin = 4;
int g_attachDatasheetUsMin = 600;
int g_attachDatasheetUsMax = 2000;
int g_attachDatasheetDegreeMax = 180;
int g_AttachMoveDegreeMin = 0;
int g_AttachMoveDegreeMax = 180;

bool g_IsTurnLoop = false;
Esp32servo270 g_servo;
WebServer g_WebServer(80);

const byte g_DNS_PORT = 53;

#endif