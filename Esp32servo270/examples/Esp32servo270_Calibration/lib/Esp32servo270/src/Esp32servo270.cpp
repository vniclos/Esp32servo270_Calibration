/*
Copyright (c) 2021 .

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

* Notes on the implementation:
* The ESP32 supports 16 hardware LED PWM channels that are intended
* to be used for LED brightness control. The low level ESP32 code
* (esp32-hal-ledc.*) allows us to set the PWM frequency and bit-depth,
* and then manipulate them by setting bits in the relevant control
* registers.
*
* Different servos require different pulse widths to vary servo angle, but the range is
* an approximately 500-2500 microsecond pulse every 20ms (50Hz). In general, hobbyist servos
* sweep 180 degrees, so the lowest number in the published range for a particular servo
* represents an angle of 0 degrees, the middle of the range represents 90 degrees, and the top
* of the range represents 180 degrees. So for example, if the range is 1000us to 2000us,
* 1000us would equal an angle of 0, 1500us would equal 90 degrees, and 2000us would equal 180
* degrees. We vary pulse width (recall that the pulse period is already set to 20ms) as follows:
*
* The ESP32 PWM timers allow us to set the timer width (attachDatasheetUsMax 20 bits). Thus
* the timer "tick" length is (pulse_period/2**timer_width), and the equation for pulse_high_width
* (the portion of the 20ms cycle that the signal is high) becomes:
*
*                  pulse_high_width  = count * tick_length
*                                    = count * (pulse_period/2**timer_width)
*
*            and   count = (pulse_high_width / (pulse_period/2**timer_width))
*
* So, for example, if I want a 1500us pulse_high_width, I set pulse_period to 20ms (20000us)attachDatasheetUsMax
* (this value is set in the ledcSetup call), and count (used in the ledcWrite call) to
* 1500/(20000/65536), or 4924. This is the value we write to the timer in the ledcWrite call.
* If we increase the timer_width, the timer_count values need to be adjusted.
*
* The servo signal pins connect to any available GPIO pins on the ESP32, but not all pins are
* GPIO pins.
*
* The ESP32 is a 32 bit processor that includes FP support; this code reflects that fact.
*/

#include "Esp32servo270.h"
#include "esp32-hal-ledc.h"
#include "Arduino.h"

// initialize the class variable ServoCount
int Esp32servo270::ServoCount = 0;

// The ChannelUsed array elements are 0 if never used, 1 if in use, and -1 if used and disposed
// (i.e., available for reuse)
int Esp32servo270::ChannelUsed[SERVOS_COUNT_LIMIT + 1] = {0}; // we ignore the zeroth element

Esp32servo270::Esp32servo270()
{
    this->servoChannel = 0;
    // see if there is a servo channel available for reuse
    bool foundChannelForReuse = false;
    for (int i = 1; i < SERVOS_COUNT_LIMIT + 1; i++)
    {
        if (ChannelUsed[i] == -1)
        {
            // reclaim this channel
            ChannelUsed[i] = 1;
            this->servoChannel = i;
            foundChannelForReuse = true;
            break;
        }
    }
    if (!foundChannelForReuse)
    {
        // no channels available for reuse; get a new one if we can
        if (ServoCount < SERVOS_COUNT_LIMIT)
        {
            this->servoChannel = ++ServoCount; // assign a servo channel number to this instance
            ChannelUsed[this->servoChannel] = 1;
        }
        else
        {
            this->servoChannel = 0; // too many servos in use
        }
    }
    // if we got a channel either way, finish initializing it
    if (this->servoChannel > 0)
    {
        // initialize this channel with plausible values, except pin # (we set pin # when getIsAtached)
        this->ticks = PULSE_WIDTH_TICKS_DEFAULT;
        this->timer_width = TIMER_WIDTH_DEFAULT;
        this->attachEsp32Pin = -1; // make it clear that we haven't getIsAtached a pin to this channel
        this->attachDatasheetUsMin = ATTACH_DATASHEET_US_MIN_DEFAULT;
        this->attachDatasheetUsMax = ATTACH_DATASHEET_US_MAX_DEFAULT;
        this->timer_width_ticks = pow(2, this->timer_width);
    }
}

int Esp32servo270::attach(int pin)
{
    return (this->attach(attachEsp32Pin, ATTACH_DATASHEET_US_MIN_DEFAULT, ATTACH_DATASHEET_US_MAX_DEFAULT, ATTACH_DATASHEET_DEGREE_MAX_DEFAULT));
}
int Esp32servo270::attach(int attachEsp32Pin, int attachDatasheetUsMin, int attachDatasheetUsMax, int attachDatasheetDegreeMax)
{
    return (this->attach(attachEsp32Pin, ATTACH_DATASHEET_US_MIN_DEFAULT, ATTACH_DATASHEET_US_MAX_DEFAULT, ATTACH_DATASHEET_DEGREE_MAX_DEFAULT, 0, ATTACH_DATASHEET_DEGREE_MAX_DEFAULT));
} // attachEsp32Pin       = Pin on ESP32 where  data servo pin is atached
// attachDatasheetUsMin     =  microseconds for go to cero degree limited by the hardware
// attachDatasheetUsMax     =  microsecond for go to max degree limited by the hardware
// attachDatasheetDegreeMax =  Degree max where the servo hardware can rotate
// attachMoveDegreeMin =  Softare limit for degree min, may be great or ecqual to 0 and lesthan attach_degree max
// attachMoveDegreeMin =  Softare limit for degree max, may be lesthan  or ecqual toATTACH_DATASHEET_DEGREE_max and great than 0
// g_servo.attach(Hwpin, HwMaxD, HwMin, HwMax, SoMin, SoMax);
int Esp32servo270::attach(int attachEsp32Pin, int attachDatasheetUsMin, int attachDatasheetUsMax, int attachDatasheetDegreeMax, int attachMoveDegreeMin, int attachMoveDegreeMax)
{
    if ((this->servoChannel <= SERVOS_COUNT_LIMIT) && (this->servoChannel > 0))
    {
        

        if (attachDatasheetDegreeMax > DEGREE_DATASHEET_MaxLimit)
        {
            attachDatasheetDegreeMax = ATTACH_DATASHEET_DEGREE_MAX_DEFAULT;
        }
        this->attachDatasheetDegreeMax = attachDatasheetDegreeMax;
        //...................................
        if (attachMoveDegreeMin < 0)
        {
            attachMoveDegreeMin = 0;
        }
        if (attachMoveDegreeMin > attachDatasheetDegreeMax)
        {
            attachMoveDegreeMin = 0;
        }
        this->attachMoveDegreeMin = attachMoveDegreeMin;
        //...................................
        if (attachMoveDegreeMax > attachDatasheetDegreeMax)
        {
            attachMoveDegreeMax = attachDatasheetDegreeMax;
        }
        if (attachMoveDegreeMax < 0)
        {
            attachMoveDegreeMax = attachDatasheetDegreeMax;
        }
        this->attachMoveDegreeMax = attachMoveDegreeMax;

        //..............................................

        // Recommend only the following pins 2,4,12-19,21-23,25-27,32-33 (enforcement commented out)
        // if ((pin == 2) || (pin ==4) || ((pin >= 12) && (pin <= 19)) || ((pin >= 21) && (pin <= 23)) ||
        //        ((pin >= 25) && (pin <= 27)) || (pin == 32) || (pin == 33))
        //{
        // OK to proceed; first check for new/reuse
        if (this->attachEsp32Pin < 0) // we are attaching to a new or previously detached pin; we need to initialize/reinitialize
        {
            // claim/reclaim this channel
            ChannelUsed[this->servoChannel] = 1;
            this->ticks = PULSE_WIDTH_TICKS_DEFAULT;
            this->timer_width = TIMER_WIDTH_DEFAULT;
            this->timer_width_ticks = pow(2, this->timer_width);
        }
        this->attachEsp32Pin = attachEsp32Pin;
        //}
        // else
        //{
        //    return 0;
        //}

        // attachDatasheetUsMin/attachDatasheetUsMax checks
        if (attachDatasheetUsMin < US_DATASHEET_MinLimit) // ensure pulse width is valid
        {
            attachDatasheetUsMin = US_DATASHEET_MinLimit;
        }
        if (attachDatasheetUsMax > US_DATASHEET_MaxLimit)
        {
            attachDatasheetUsMax = US_DATASHEET_MaxLimit;
        }
        this->attachDatasheetUsMin = attachDatasheetUsMin; // store this value in uS
        this->attachDatasheetUsMax = attachDatasheetUsMax; // store this value in 
        
        // convert degregre move limitis to US move Limitis
        attachMoveUsMin= map(attachMoveDegreeMin, 0, this->attachDatasheetDegreeMax, this->attachDatasheetUsMin, this->attachDatasheetUsMax);
        attachMoveUsMax= map(attachMoveDegreeMax, 0, this->attachDatasheetDegreeMax, this->attachDatasheetUsMin, this->attachDatasheetUsMax);
        // Set up this channel
        // if you want anything other than default timer width, you must call setTimerWidth() before attach
        ledcSetup(this->servoChannel, REFRESH_CPS, this->timer_width); // channel #, 50 Hz, timer width
        
        ledcAttachPin(this->attachEsp32Pin, this->servoChannel);           // GPIO pin assigned to channel
        Serial.print("Servo setup as: attachEsp32Pin= ");
        Serial.print(this->attachEsp32Pin);
        Serial.print(" attachDatasheetUsMin=");
        Serial.print(this->attachDatasheetUsMin);
        Serial.print(" attachDatasheetUsMax=");
        Serial.print(this->attachDatasheetUsMax);
        Serial.print(" attachDatasheetDegreeMax=");
        Serial.println(this->attachDatasheetDegreeMax);
        Serial.println("------------");
        return 1;
    }

    return 0;
}
void Esp32servo270::detach()
{
    if (this->getIsAtached())
    {
        ledcDetachPin(this->attachEsp32Pin);
        // keep track of detached servos channels so we can reuse them if needed
        ChannelUsed[this->servoChannel] = -1;
        this->attachEsp32Pin = -1;
    }
}



bool Esp32servo270::IsValidMoveTo(int iMoveTo)
{
    if (iMoveTo<attachDatasheetUsMin) // move to in degree
    {
    if (iMoveTo >= getAttachMoveDegreeMin() && iMoveTo <= getAttachMoveDegreeMax())
    {

        return true;
    }
    }
    else // move to in microsecconds
    if (iMoveTo >= attachDatasheetUsMin && iMoveTo <= attachDatasheetUsMax)
    {

        return true;
    }
    return false;
}

int Esp32servo270::write(int value)
{
    // if value lesthan 270
    // treat values less than US_DATASHEET_MinLimit (500) as angles in degrees with maximun DEGREE_DATASHEET_MaxLimit=270
    //(valid values in microseconds are handled as microseconds)
    if (value < US_DATASHEET_MinLimit) // case parameter are degree
    {
        if (value < 0)
        {
            value = 0;
        }
        else if (value > this->attachMoveDegreeMax)
        {
            value = this->attachMoveDegreeMax;
        }
         else if (value < this->attachMoveDegreeMin)
        {
            value = this->attachMoveDegreeMin;
        }

        value = map(value, 0, this->attachDatasheetDegreeMax, this->attachDatasheetUsMin, this->attachDatasheetUsMax);
    }
    else
    {
        
        if (value < this->attachMoveUsMin)

        {
            value = attachMoveUsMin;
        }
        else if (value > this->attachMoveUsMax )
        {
            value = attachMoveUsMax;
        }
    }

    this->writeMicroseconds(value);
    return value;
}

void Esp32servo270::writeMicroseconds(int value)
{


    // calculate and store the values for the given channel
    if ((this->servoChannel <= SERVOS_COUNT_LIMIT) && (this->getIsAtached())) // ensure channel is valid
    {
        if (value < this->attachMoveUsMin) // ensure pulse width is valid
            value = this->attachMoveUsMin;
        else if (value > this->attachMoveUsMax)
            value = this->attachMoveUsMax;

        value = usToTicks(value); // convert to ticks
        this->ticks = value;
        // do the actual write
        ledcWrite(this->servoChannel, this->ticks);
    }
}

int Esp32servo270::getPositionDegree() // return the value as degrees
{
    return (map(getPositionMicroseconds() + 1, this->attachDatasheetUsMin, this->attachDatasheetUsMax, 0, this->attachMoveDegreeMax));
}

int Esp32servo270::getPositionMicroseconds()
{
    int pulsewidthUsec;
    if ((this->servoChannel <= SERVOS_COUNT_LIMIT) && (this->getIsAtached()))
    {
        pulsewidthUsec = ticksToUs(this->ticks);
    }
    else
    {
        pulsewidthUsec = 0;
    }

    return (pulsewidthUsec);
}

bool Esp32servo270::getIsAtached() { return (ChannelUsed[this->servoChannel]); }
int Esp32servo270::getAttachEsp32Pin() { return this->attachEsp32Pin; }
int Esp32servo270::getattachDatasheetUsMin() { return this->attachDatasheetUsMin; }
int Esp32servo270::getattachDatasheetUsMax() { return this->attachDatasheetUsMax; }
int Esp32servo270::getattachDatasheetDegreeMax() { return this->attachDatasheetDegreeMax; }
int Esp32servo270::getAttachMoveDegreeMin() { return this->attachMoveDegreeMin; }
int Esp32servo270::getAttachMoveDegreeMax() { return this->attachMoveDegreeMax; }

int Esp32servo270::getattachDatasheetUsMinLimit() { return US_DATASHEET_MinLimit; }
int Esp32servo270::getattachDatasheetUsMaxLimit() { return US_DATASHEET_MaxLimit; }

int Esp32servo270::getattachDatasheetDegreeMaxLimit() { return DEGREE_DATASHEET_MaxLimit; }
int Esp32servo270::getAttachDegreeMinLimit() { return DEGREE_DATASHEET_MinLimit; }
void Esp32servo270::setTimerWidth(int value)
{
    // only allow values between 16 and 20
    if (value < 16)
        value = 16;
    else if (value > 20)
        value = 20;

    // Fix the current ticks value after timer width change
    // The user can reset the tick value with a write() or writeUs()
    int widthDifference = this->timer_width - value;
    // if positive multiply by diff; if neg, divide
    if (widthDifference > 0)
    {
        this->ticks << widthDifference;
    }
    else
    {
        this->ticks >> widthDifference;
    }

    this->timer_width = value;
    this->timer_width_ticks = pow(2, this->timer_width);

    // If this is an getIsAtached servo, clean up
    if ((this->servoChannel <= SERVOS_COUNT_LIMIT) && (this->getIsAtached()))
    {
        // detach, setup and attach again to reflect new timer width
        ledcDetachPin(this->attachEsp32Pin);
        ledcSetup(this->servoChannel, REFRESH_CPS, this->timer_width);
        ledcAttachPin(this->attachEsp32Pin, this->servoChannel);
    }
}

int Esp32servo270::getTimerWidth()
{
    return (this->timer_width);
}

int Esp32servo270::usToTicks(int usec)
{
    return (int)((float)usec / ((float)REFRESH_USEC / (float)this->timer_width_ticks));
}

int Esp32servo270::ticksToUs(int ticks)
{
    return (int)((float)ticks * ((float)REFRESH_USEC / (float)this->timer_width_ticks));
}