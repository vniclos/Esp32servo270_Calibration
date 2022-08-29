//javascripts

#ifndef _WEB_TEMPLATE_H
#define _WEB_TEMPLATE_H
const char g_HTML_Web_template[] PROGMEM = R"=====(
  
//------------------------------------------------------
//----------------------  README  ----------------------
// Esp32Servo270 library, basic used sample, adapted with your  
// callibration options taked.
// be carefull with your option for not breack your servo
// develop name conventions
// prefix g_ is used for global variables and object
// prefix g_hard is used for hardware values
// prefix g_soft is used for sofware values
// (1)  When starting, use values from the datasheet, then go adjusting manually 
//------------------------------------------------------
//-----------------------  CODE  -----------------------
#include &lt;Arduino.h&gt;
#include &lt;Esp32servo270.h&gt;
 int g_pin_ledblue =2;   // In ESP32 dev kit is used for manage blue led
 int g_AttachEsp32Pin =#g_AttachEsp32Pin#;     // pin where servo is atached
 int g_attachDatasheetUsMin=#g_attachDatasheetUsMin#;      // Microsecond for go t0 0 degree. (1) 
 int g_attachDatasheetUsMax=#g_attachDatasheetUsMax#;     // Microseconds for go to max degree (1)
 int g_attachDatasheetDegreeMax=#g_attachDatasheetDegreeMax#;  // Maximum turn defined by hardware (1)
 int g_AttachMoveDegreeMin=#g_AttachMoveDegreeMin#;    // Min position in degree limited free movement in you instalation use, (default 0)
 int g_AttachMoveDegreeMax=#g_AttachMoveDegreeMax#;  // Max position in degree limited free movement in you instalation use, (defalut g_attachDatasheetDegreeMax)
 int g_delayMove=100;        // delay for do not fast movement
 Esp32servo270 g_servo;      // object for manage the servo
 //------------------------------------------------------
 void setup() 
 {

    // 1 Full parameticed atach servo
    g_servo.attach(g_AttachEsp32Pin, g_attachDatasheetUsMin, g_attachDatasheetUsMax, g_attachDatasheetDegreeMax, g_AttachMoveDegreeMin, g_AttachMoveDegreeMax);
    //.....
    // 2 Medium parameticed call to atach servo
    // g_servo.attach(g_AttachEsp32Pin, g_attachDatasheetUsMin, g_attachDatasheetUsMax, g_attachDatasheetDegreeMax);
    
    // Then the omited parameter take this values:
    // g_AttachMoveDegreeMin =0
    // g_AttachMoveDegreeMax=g_attachDatasheetDegreeMax
    //.....
    // 3 minimun parameticed call to atach servo
    // g_servo.attach(g_AttachEsp32Pin);

    // Then the omited parameter take this values
    // g_attachDatasheetUsMin     = Default value in libary = #define US_DATASHEET_MinLimit 500  (Microsenconds)
    // g_attachDatasheetUsMax     = Default value in libary = #define US_DATASHEET_MaxLimit 2500 (Microsenconds)
    // g_attachDatasheetDegreeMax = Default value in libary = #define ATTACH_DATASHEET_DEGREE_MAX_DEFAULT 180
    // g_AttachMoveDegreeMin = 0:
    // g_AttachMoveDegreeMax = attachDatasheetDegreeMax      
    //.....
 }
 //------------------------------------------------------
 void loop() 
 {
   //........................................................
  // turn in degree steeps 
  //........................................................
  // turn rigth in degree steep
  for (int iDegree=0; iDegree=&lt;g_attachDatasheetDegreeMax; iDegree++)
  {
  	 g_servo.write(iDegree);
  	 delay(g_delayMove);
  }
  // turn reverse in degree steep
  for (int iDegree=g_attachDatasheetDegreeMax; iDegree&gt;-1; iDegree--)
  {
  	 g_servo.write(iDegree);
  	 delay(g_delayMove);
  }
}
)=====";
#endif