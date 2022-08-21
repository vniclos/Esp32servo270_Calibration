#ifndef _WEB_HANDLE_H
#define _WEB_HANDLE_H
#include "mainDefines.h"
#include "web_root.h"
#include "web_js.h"
#include "web_css.h"
#include "web_html_template.h"

#define PIN_LEDINTBLUE2 2

//=================================================
// void handleROOT()
//=================================================
void handleROOT()
{
  // Serial.println("handleRoot");
  digitalWrite(g_pin_ledblue, 1);
  Serial.println("handleROOT");
  g_WebServer.send(200, "text/html", g_HTML_Web_root);
  digitalWrite(g_pin_ledblue, 0);
}

void handleCSS()
{
  digitalWrite(g_pin_ledblue, 1);
  Serial.println("handleCSS");
  digitalWrite(g_pin_ledblue, 1);
  ;

  g_WebServer.send(200, "text/css", g_HTML_Web_CSS);
  digitalWrite(g_pin_ledblue, 0);
}

void handleJS()
{
  Serial.println("handleJS");
  digitalWrite(g_pin_ledblue, 1);
  ;

  g_WebServer.send(200, "application/javascript", g_HTML_Web_JS);
  digitalWrite(g_pin_ledblue, 0);
}

void handleAtach()
{
  Serial.println("handleAtach");
  digitalWrite(g_pin_ledblue, 1);

  // URl  parameters =Hwpin, HwMaxD, HwMin, HwMax, SoMin, SoMax
  String sValue = "";
  int iValue = -1;
  int Hwpin = -1;
  int HwMaxD = -1;
  int HwMin = -1;
  int HwMax = -1;
  int SoMinD = -1;
  int SoMaxD = -1;
  int iParameters = -1;
  String name="";
  String htmReply = "";
  bool bParameterOk = true;

  iParameters = g_WebServer.args();
  if (g_WebServer.hasArg("Hwpin"))
  {
    sValue = g_WebServer.arg("Hwpin");
    Hwpin = sValue.toInt();
  }
  if (g_WebServer.hasArg("HwMaxD"))
  {
    sValue = g_WebServer.arg("HwMaxD");
    HwMaxD = sValue.toInt();
  }
  if (g_WebServer.hasArg("HwMin"))
  {
    sValue = g_WebServer.arg("HwMin");
    HwMin = sValue.toInt();
  }
  if (g_WebServer.hasArg("HwMax"))
  {
    sValue = g_WebServer.arg("HwMax");
    HwMax = sValue.toInt();
  }
  if (g_WebServer.hasArg("SoMinD"))
  {
    sValue = g_WebServer.arg("SoMinD");
    SoMinD = sValue.toInt();
  }
  if (g_WebServer.hasArg("SoMaxD"))
  {
    sValue = g_WebServer.arg("SoMaxD");
    SoMaxD = sValue.toInt();
  }

 

  // validate paramameters
  if (iParameters < 6)
  {
    htmReply += "<br/>Paramenters are insuficients";
    bParameterOk = false;
  }
  if (Hwpin < 1 || Hwpin > 32)
  {
    htmReply += "<br/>Hardware pin=" + String(Hwpin) + " but need range [1-32]";
    bParameterOk = false;
  }

  if (HwMaxD > g_servo.getattachDatasheetDegreeMaxLimit())
  {
    htmReply += "<br/>Hardware Max degree=" + String(HwMaxD) + "Maximum are " + String(g_servo.getattachDatasheetDegreeMaxLimit());
    bParameterOk = false;
  }
  if (HwMin < g_servo.getattachDatasheetUsMinLimit() || HwMin > HwMax)
  {
    htmReply += "<br/>Hardware Min microsecconds=" + String(HwMin) + " Range valid are [" + String(HwMin) + "-" + String(g_servo.getattachDatasheetUsMaxLimit()) + "]";
    bParameterOk = false;
  }

  if (HwMax < HwMin + 1 || HwMax > g_servo.getattachDatasheetUsMaxLimit())
  {
    htmReply += "<br/>Hardware Max microsecconds=" + String(HwMax) + " Range valid are [" + String(HwMin + 1) + "-" + String(g_servo.getattachDatasheetUsMaxLimit()) + "]";
    bParameterOk = false;
  }
  if (SoMinD < 0 || SoMinD >= HwMaxD)
  {
    htmReply += "<br/>Software Min degree=" + String(SoMinD) + " Range valid are [0-" + String(HwMaxD - 1) + "]";
    bParameterOk = false;
  }
  if (SoMaxD < SoMinD || SoMaxD > HwMaxD)
  {
    htmReply += "<br/Software Max degree =" + String(SoMaxD) + " Range valid are [" + String(SoMinD) + "-" + String(HwMaxD) + "]";
    bParameterOk = false;
  }
  if (!bParameterOk)
  {
    htmReply = "<span class='red'>Somethings) paramerter are invalid" + htmReply + "</span>";
  }
  else
  {
    htmReply = "Ok Servo atached with this values:";
    htmReply += "<br/>attachEsp32Pin =" + String(Hwpin);
    htmReply += "<br/>attachDatasheetUsMin =" + String(HwMin);
    htmReply += "<br/>attachDatasheetUsMax=" + String(HwMax);
    htmReply += "<br/attachDatasheetDegreeMax=" + String(HwMaxD);
    htmReply += "<br/attachMoveDegreeMin =" + String(SoMinD);
    htmReply += "<br/>attachMoveDegreeMax =" + String(SoMaxD);
    htmReply += "<br/>A bit of code : servoAtachValues[6] ={" +  String(Hwpin)+", "+String(HwMin) +", "+String(HwMax)+", "+String(HwMaxD)+", "+ String(SoMinD)+", "+ String(SoMaxD)+"}";

    if (g_servo.getIsAtached())
    {
      g_servo.detach();
    }
    g_servo.attach(Hwpin, HwMin, HwMax, HwMaxD, SoMinD, SoMaxD);
  }
  Serial.println(htmReply);
  g_WebServer.send(200, "text/html", htmReply);

  digitalWrite(g_pin_ledblue, 0);
}
//==================================================
//==================================================
void handleDetach()
{
  Serial.println("handleDetach");
  digitalWrite(g_pin_ledblue, 1);

  String htmReply = "";
  g_servo.detach();

  g_WebServer.send(200, "text/html", "Servo detached");
  digitalWrite(g_pin_ledblue, 0);
}
//================================================
//================================================
void handleMove()
{
    Serial.println("handleMove");
  digitalWrite(g_pin_ledblue, 1);
  if (!g_servo.getIsAtached())
  {
    g_WebServer.send(200, "text/plain", "Please, Atach servo before start move");
    digitalWrite(g_pin_ledblue, 0);
    return;
  }


  String sMoveTo = "";
  int iMoveTo = -1;
  String htmReply = "";
  bool bParameterOk = true;

  int iParameters = g_WebServer.args();
 

  if (bParameterOk != 1)
  {
    bParameterOk = false;
    htmReply += "<br/><span class='red'>Please need only degree parameter</span>";
  }
  if (g_WebServer.hasArg("moveto"))
  {

    sMoveTo = g_WebServer.arg("moveto");
    iMoveTo = sMoveTo.toInt();
  }
  else
  {
    bParameterOk = false;
    htmReply += "<br/><span class='red'>Please need degree parameter</span>";
  }
  if (!g_servo.IsValidMoveTo(iMoveTo)) // if iDegree
  {
    htmReply += "<br/><span class='red'>Move in degree should be in range [ " + String(g_servo.getAttachMoveDegreeMin()) + " to " + String(g_servo.getAttachMoveDegreeMax()) + "] if move is in micro secconds range should be ["+String(g_servo.getattachDatasheetUsMin()) +"-"+ String(g_servo.getattachDatasheetUsMax())+"]";
  }

  if (!g_servo.getIsAtached())
  {
    bool bParameterOk = false;
    htmReply += "<span class='red'>Oops, I can`t move, I'mot atached. Please configure before.</span>";
  }
  if (bParameterOk)
  {
     g_servo.write(iMoveTo);
    htmReply = "Ok, Servo moved to " +String( g_servo.getPositionDegree()) + " degrees this is equivalent to " + String(g_servo.getPositionMicroseconds()) + " microseconds";
  }
 
  g_WebServer.send(200, "text/html", htmReply);
  digitalWrite(g_pin_ledblue, 0);
}

//=================================================
//=================================================
void handleNotFound()
{
  digitalWrite(g_pin_ledblue, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += g_WebServer.uri();
  message += "\nMethod: ";
  message += (g_WebServer.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += g_WebServer.args();
  message += "\n";

  for (uint8_t i = 0; i < g_WebServer.args(); i++)
  {
    message += " " + g_WebServer.argName(i) + ": " + g_WebServer.arg(i) + "\n";
  }

  g_WebServer.send(404, "text/plain", message);
  digitalWrite(g_pin_ledblue, 0);
}

void handleTemplate()
{
  Serial.println("handleTemplate");
  digitalWrite(g_pin_ledblue, 1);
  
  String myCode = "";
  myCode = String(g_HTML_Web_template);
  myCode.replace("#g_AttachEsp32Pin#", String(g_servo.getAttachEsp32Pin()));
  myCode.replace("#g_attachDatasheetUsMin#", String(g_servo.getattachDatasheetUsMin()));
  myCode.replace("#g_attachDatasheetUsMax#", String(g_servo.getattachDatasheetUsMax()));
  myCode.replace("#g_attachDatasheetDegreeMax#", String(g_servo.getattachDatasheetDegreeMax()));
  myCode.replace("#g_AttachMoveDegreeMin#", String(g_servo.getAttachMoveDegreeMin()));
  myCode.replace("#g_AttachMoveDegreeMax#", String(g_servo.getAttachMoveDegreeMax()));

  g_WebServer.send(200, "application/javascript", myCode);
  digitalWrite(g_pin_ledblue, 0);
}
void handleTurnMicro()
{
  Serial.println("handleTurnMicro");
  bool ledFlashing = HIGH;
  digitalWrite(g_pin_ledblue, 1);
  if (!g_servo.getIsAtached())
  {
    g_WebServer.send(200, "text/plain", "Please, Atach servo before start test");
    digitalWrite(g_pin_ledblue, 0);
    return;
  }

  for (int i = g_servo.getattachDatasheetUsMin(); i < g_servo.getattachDatasheetUsMax(); i++)
  {
    digitalWrite(g_pin_ledblue, ledFlashing);
    ledFlashing = !ledFlashing;
    g_servo.write(i);
    delay(100);
  }
  for (int i = g_servo.getattachDatasheetUsMax(); i > g_servo.getattachDatasheetUsMin(); i--)
  {
    digitalWrite(g_pin_ledblue, ledFlashing);
    ledFlashing = !ledFlashing;
    g_servo.write(i);
    delay(100);
  }
  g_WebServer.send(200, "text/plain", "Please wait, this terst turn right and left one time using micro steeps");
  digitalWrite(g_pin_ledblue, 0);
}
void handleTurnDegree()
{
  Serial.println("handleTurnDegree");
  bool ledFlashing = HIGH;
  if (!g_servo.getIsAtached())
  {
    g_WebServer.send(200, "text/plain", "Please, Atach servo before start test");
    digitalWrite(g_pin_ledblue, 0);
    return;
  }
  for (int i = g_servo.getAttachMoveDegreeMin(); i < g_servo.getAttachMoveDegreeMax(); i++)
  {
    digitalWrite(g_pin_ledblue, ledFlashing);
    ledFlashing = !ledFlashing;
    g_servo.write(i);
    delay(100);
  }
  for (int i = g_servo.getAttachMoveDegreeMax(); i > g_servo.getAttachMoveDegreeMin(); i--)
  {
    digitalWrite(g_pin_ledblue, ledFlashing);
    ledFlashing = !ledFlashing;
    g_servo.write(i);
    delay(100);
  }
  g_WebServer.send(200, "text/plain", "Please wait, this terst turn right and left one time using degree steeps");
  digitalWrite(g_pin_ledblue, 0);
}
#endif