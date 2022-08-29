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
String htmlGetAtach()
{
  String htmReply="";
htmReply += "<br/>getAttachEsp32Pin= " + String(g_servo.getAttachEsp32Pin());
  htmReply += "<br/>getAttachDatasheetDegreeMax= " + String(g_servo.getAttachDatasheetDegreeMax());
  htmReply += "<br/>getAttachDatasheetUsMin= " + String(g_servo.getAttachDatasheetUsMin());
  htmReply += "<br/>getAttachDatasheetUsMax= " + String(g_servo.getAttachDatasheetUsMax());
  htmReply += "<br/>getAttachMoveDegreeMin= " + String(g_servo.getAttachMoveDegreeMin());
  htmReply += "<br/>getAttachMoveDegreeMax= " + String(g_servo.getAttachMoveDegreeMax());
  htmReply += "<br/>getAttachMoveUsMin= " + String(g_servo.getAttachMoveUsMin());
  htmReply += "<br/>getAttachMoveUsMax= " + String(g_servo.getAttachMoveUsMax());
  return htmReply;
}
void handleAtach()
{
  Serial.println("handleAtach");
  digitalWrite(g_pin_ledblue, 1);

  // URl  parameters =Hwpin, DataSheetMaxD, DataSheetMinUs, DataSheetMaxUs, SoMin, SoMax
  String sValue = "";
  int iValue = -1;
  int Hwpin = -1;
  int DataSheetMaxD = -1;
  int DataSheetMinUs = -1;
  int DataSheetMaxUs = -1;
  int MoveMinD = -1;
  int MoveMaxD = -1;
  int iParameters = -1;
  String name = "";
  String htmReply = "";
  bool bParameterOk = true;

  iParameters = g_WebServer.args();
  if (g_WebServer.hasArg("Hwpin"))
  {
    sValue = g_WebServer.arg("Hwpin");
    Hwpin = sValue.toInt();
  }
  if (g_WebServer.hasArg("DataSheetMaxD"))
  {
    sValue = g_WebServer.arg("DataSheetMaxD");
    DataSheetMaxD = sValue.toInt();
  }
  if (g_WebServer.hasArg("DataSheetMinUs"))
  {
    sValue = g_WebServer.arg("DataSheetMinUs");
    DataSheetMinUs = sValue.toInt();
  }
  if (g_WebServer.hasArg("DataSheetMaxUs"))
  {
    sValue = g_WebServer.arg("DataSheetMaxUs");
    DataSheetMaxUs = sValue.toInt();
  }
  if (g_WebServer.hasArg("MoveMinD"))
  {
    sValue = g_WebServer.arg("MoveMinD");
    MoveMinD = sValue.toInt();
  }
  if (g_WebServer.hasArg("MoveMaxD"))
  {
    sValue = g_WebServer.arg("MoveMaxD");
    MoveMaxD = sValue.toInt();
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

  if (DataSheetMaxD > g_servo.getattachDatasheetDegreeMaxLimit())
  {
    htmReply += "<br/>Hardware Max degree=" + String(DataSheetMaxD) + "Maximum are " + String(g_servo.getattachDatasheetDegreeMaxLimit());
    bParameterOk = false;
  }
  if (DataSheetMinUs < g_servo.getattachDatasheetUsMinLimit() || DataSheetMinUs > DataSheetMaxUs)
  {
    htmReply += "<br/>Hardware Min microsecconds=" + String(DataSheetMinUs) + " Range valid are [" + String(DataSheetMinUs) + "-" + String(g_servo.getattachDatasheetUsMaxLimit()) + "]";
    bParameterOk = false;
  }

  if (DataSheetMaxUs < DataSheetMinUs + 1 || DataSheetMaxUs > g_servo.getattachDatasheetUsMaxLimit())
  {
    htmReply += "<br/>Hardware Max microsecconds=" + String(DataSheetMaxUs) + " Range valid are [" + String(DataSheetMinUs + 1) + "-" + String(g_servo.getattachDatasheetUsMaxLimit()) + "]";
    bParameterOk = false;
  }
  if (MoveMinD < 0 || MoveMinD >= DataSheetMaxD)
  {
    htmReply += "<br/>Software Min degree=" + String(MoveMinD) + " Range valid are [0-" + String(DataSheetMaxD - 1) + "]";
    bParameterOk = false;
  }
  if (MoveMaxD < MoveMinD || MoveMaxD > DataSheetMaxD)
  {
    htmReply += "<br/Software Max degree =" + String(MoveMaxD) + " Range valid are [" + String(MoveMinD) + "-" + String(DataSheetMaxD) + "]";
    bParameterOk = false;
  }
  if (!bParameterOk)
  {
    htmReply = "<span class='red'>Somethings) paramerter are invalid" + htmReply + "</span>";
  }
  else
  {

    if (g_servo.getIsAtached())
    {
      g_servo.detach();
    }
    g_servo.attach(Hwpin, DataSheetMinUs, DataSheetMaxUs, DataSheetMaxD, MoveMinD, MoveMaxD);
  }
  htmReply = "Ok Servo atached with this values:";
 htmReply+=htmlGetAtach();

  htmReply += "<br/>A bit of code : servoAtachValues[6] ={" + String(Hwpin) + ", " + String(DataSheetMinUs) + ", " + String(DataSheetMaxUs) + ", " + String(DataSheetMaxD) + ", " + String(MoveMinD) + ", " + String(MoveMaxD) + "}";

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
    htmReply += "<br/><span class='red'>Please need only degree o microseccons parameter</span>";
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
    htmReply += "<br/><span class='red'>Move in degree should be in range [ " + String(g_servo.getAttachMoveDegreeMin()) + " to " + String(g_servo.getAttachMoveDegreeMax()) + "] if move is in micro secconds range should be [" + String(g_servo.getAttachDatasheetUsMin()) + "-" + String(g_servo.getAttachDatasheetUsMax()) + "]";
  }

  if (!g_servo.getIsAtached())
  {
    bool bParameterOk = false;
    htmReply += "<span class='red'>Oops, I can`t move, I'mot atached. Please configure before.</span>";
  }
  if (bParameterOk)
  {
    g_servo.write(iMoveTo);
    htmReply = "Ok, Servo moved to " + String(g_servo.getPositionDegree()) + " degrees this is equivalent to " + String(g_servo.getPositionMicroseconds()) + " microseconds";
  }

  g_WebServer.send(200, "text/html", htmReply);
  digitalWrite(g_pin_ledblue, 0);
}
void handleStatus()
{
  digitalWrite(g_pin_ledblue, 1);
  String htmReply = "";
  htmReply += "<b>Status</b>";
  htmReply += htmlGetAtach();
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
  myCode.replace("#g_attachDatasheetUsMin#", String(g_servo.getAttachDatasheetUsMin()));
  myCode.replace("#g_attachDatasheetUsMax#", String(g_servo.getAttachDatasheetUsMax()));
  myCode.replace("#g_attachDatasheetDegreeMax#", String(g_servo.getAttachDatasheetDegreeMax()));
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

  for (int i = g_servo.getAttachDatasheetUsMin(); i < g_servo.getAttachDatasheetUsMax(); i++)
  {
    digitalWrite(g_pin_ledblue, ledFlashing);
    ledFlashing = !ledFlashing;
    g_servo.write(i);
    delay(100);
  }
  for (int i = g_servo.getAttachDatasheetUsMax(); i > g_servo.getAttachDatasheetUsMin(); i--)
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