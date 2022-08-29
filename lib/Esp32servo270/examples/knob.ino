/* 
 * considerable power, we will connect servo power to the VBat pin of the ESP32 (located
 * near the USB connector). THIS IS ONLY APPROPRIATE FOR SMALL SERVOS. 
 * 
 * We could also connect servo power to a separate external
 * power source (as long as we connect all of the grounds (ESP32, servo, and external power).
 * In this example, we just connect ESP32 ground to servo ground. The servo signal pins
 * connect to any available GPIO pins on the ESP32 (in this example, we use pin 18.
 * 
 * In this example, we assume a Tower Pro SG90 small servo connected to VBat.
 * The published min and max for this servo are 500 and 2400, respectively.
 * These values actually drive the servos a little past 0 and 180, so
 * if you are particular, adjust the min and max values to match your needs.
*/

// Include the ESP32 Arduino Servo Library instead of the original Arduino Servo Library
<include <arrduino.h>
#include "Esp32servo270.h" 

Esp32servo270 g_servo;                // create servo object to control a servo
const int g_AttachEsp32Pin      = 4;    // Pin on es32 wher servo is attached 
                                      // Possible PWM GPIO pins on the ESP32: 0(used by on-board button)
                                      // ,2,4,5(used by  dev kit esp32 ),12-19,21-23,25-27,32-33 



int g_attachDatasheetUsMin =500;           // Micropulse for go to 0 degree 
int g_attachDatasheetUsMax =2400;          // Micropulse for go Maximus degree
                                      // using SG90 servo min/max of 500us and 2400us
                                      // for MG995 large servo, use 1000us and 2000us,
int g_attachDatasheetDegreeMax      =180;  // Max degree where servo can go.

int g_ATTACH_DATASHEET_DEGREE_min =20 ;               // optional parameter for limitation of movement by sofware
int g_attachDatasheetDegreeMax =90;                // optional parameter for limitation of movement by sofware

int g_PotValue =-1;                   // for store readed potetiometre value
int g_Degree   =-1;                   // for store degree move in fuction of potentiometre value

int g_AttachEsp32Pin = 18;      // GPIO pin used to connect the servo control (digital out)


const int g_PinPotentiometer  = 34;   // Possible ADC pins on the ESP32: 0,2,4,12-15,32-39; 34-39 
                                      // are recommended for analog input
const int g_ADC_Max           = 4096; // This is the default ADC max value on the ESP32 (12 bit ADC width);
                                      // this width can be set (in low-level oode) from 9-12 bits, for a
                                      // a range of max values of 512-4096

int g_potPin = 34;        // GPIO pin used to connect the potentiometer (analog in)
int g_ADC_Max = 4096;     
  
int g_delayLoop=300;    // variable to read the value from the analog pin

void setup()
{
    g_servo.attach(g_AttachEsp32Pin, g_attachDatasheetUsMin, g_attachDatasheetUsMax,g_attachDatasheetDegreeMax);

    / readme attach detail uses
   // setup the servo 
   // you can call the attach function in three different ways, in case default values ​​are taken
   // ........
   // first one, the most simple
   // g_servo.attach(g_PinServo);
   // in this case :
   // g_Servo_minMicroPulse = #define DEFAULT_MIN_PULSE_WIDTH 500 
   // g_Servo_maxMicroPulse = #define PULSE_WIDTH_DEFAULT 1400
   // g_Servo_MaxDegre  = #define ATTACH_DATASHEET_DEGREE_MAX_DEFAULT 180
   // g_ATTACH_DATASHEET_DEGREE_min =0;      
   // g_attachDatasheetDegreeMax = g_Servo_MaxDegre
   // IN ANY CASE THERE ARE SOME LIMITS FOR THIS VALUES
   // #define DEGREE_DATASHEET_MinLimit 90  ( for servos can go 0 t0 90 degree)
   // #define DEGREE_DATASHEET_MaxLimit 270  ( for servos can go 0 t0 270 degree)
   // #define US_DATASHEET_MinLimit 500   the shortest pulse sent to a servo
   // #define US_DATASHEET_MaxLimit 2500  the longest pulse sent to a servo
   // ........
   // Second is most acurate, and has this explicit parametrers according to hardware datasheet 
   //g_servo.attach(g_AttachEsp32Pin, g_attachDatasheetUsMin, g_attachDatasheetUsMax,g_attachDatasheetDegreeMax);
   // if in one paremeter you use values greater  of limit maximum, the library overwrite your value
   // with the maxim valid value, and reverse in case min values.
   // ........


   g_servo.attach(g_AttachEsp32Pin, g_attachDatasheetUsMin, g_attachDatasheetUsMax,g_attachDatasheetDegreeMax, g_ATTACH_DATASHEET_DEGREE_min, g_attachDatasheetDegreeMax); 
   // ATTACH_DATASHEET_DEGREE_min by default is cero, 
   // attachDatasheetDegreeMax by default is equal to g_Servo_MaxDegre and maximun equal g_Servo_MaxDegre
   // The use is for limit movement inside de full range of movement, 
   // is used for limit momement according the use in installatin


                                         
                                         // which are the defaults, so this line could be
                                         // "myservo.attach(servoPin);"
}

void loop() {
  g_PotValue = analogRead(g_potPin);                       // read the value of the potentiometer (value between 0 and 1023)
  g_Degree = map(g_PotValue, 0, g_ADC_Max,0, g_servo.g_attachDatasheetDegreeMax());   // scale it to use it with the servo attached values
  g_servo.write(g_Degree);                               // set the servo position according to the scaled value
  delay(g_delayLoop);                                    // wait for the servo to get there
}

