/*This file is part of the Maslow Control Software.
    The Maslow Control Software is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Maslow Control Software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with the Maslow Control Software.  If not, see <http://www.gnu.org/licenses/>.

    Copyright 2014-2017 Bar Smith*/

// This contains all of the Spindle commands

#include "Maslow.h"
#include "Settings.h"

// the variables SpindlePowerControlPin and LaserPowerPin are assigned in setupAxes() in System.cpp

// Globals for Spindle control, both poorly named
#ifndef SPINDLE_SPEED
Servo myservo;  // create servo object to control a servo
#endif

void  setSpindlePower(bool powerState) {
    /*
     * Turn spindle on or off depending on powerState
     */
    SpindleAutomationType spindleAutomateType = sysSettings.spindleAutomateType;

    int delayAfterChange = 1000;  // milliseconds
    int servoIdle =  90;  // degrees
    int servoOn   = 180;  // degrees
    int servoOff  =   0;  // degrees
    int servoDelay = 2000;  // milliseconds

    // Now for the main code
    #if defined (verboseDebug) && verboseDebug > 1
    Serial.print(F("Spindle control uses pin "));
    Serial.print(SpindlePowerControlPin);
    Serial.print(F("Spindle automation type "));
    Serial.print(spindleAutomateType);
    #endif
    #ifndef SPINDLE_SPEED
    if (spindleAutomateType == SERVO) {   // use a servo to control a standard wall switch
        #if defined (verboseDebug) && verboseDebug > 1
        Serial.print(F(" with servo (idle="));
        Serial.print(servoIdle);
        Serial.print(F(", on="));
        Serial.print(servoOn);
        Serial.print(F(", off="));
        Serial.print(servoOff);
        Serial.println(F(")"));
        #endif
        myservo.attach(SpindlePowerControlPin); // start servo control
        myservo.write(servoIdle);   // move servo to idle position
        maslowDelay(servoDelay);    // wait for move to complete
        if(sys.stop){return;}
        if (powerState) { // turn on spindle
            Serial.println(F("Turning Spindle On"));
            myservo.write(servoOn); // move servo to turn on switch
        }
        else {            // turn off spindle
            Serial.println(F("Turning Spindle Off"));
            myservo.write(servoOff); // move servo to turn off switch
        }
        maslowDelay(servoDelay);    // wait for move to complete
        if(sys.stop){return;}
        myservo.write(servoIdle);   // return servo to idle position
        maslowDelay(servoDelay);    // wait for move to complete
        if(sys.stop){return;}
        myservo.detach();           // stop servo control
    }
    #endif
    if (spindleAutomateType == SPEED_CONTROL_RELAY_ACTIVE_HIGH){
        #if defined (verboseDebug) && verboseDebug > 1
        Serial.print(F(" as digital power and pwm speed control, active high"));
        #endif
        pinMode(SpindlePowerControlPin, OUTPUT);
        if (powerState) { // turn on spindle
            Serial.println(F("Turning Spindle On"));
            digitalWrite(SpindlePowerControlPin, HIGH);
            Serial.println(F("Adjusting Spindle Speed to Minimum"));
            setSpindleSpeed(sysSettings.spindleMin);
        }
        else {            // turn off spindle
            Serial.println(F("Turning Spindle Off"));
            digitalWrite(SpindlePowerControlPin, LOW);
            digitalWrite(SpindleSpeedControlPin, LOW);
        }
    }
    if (spindleAutomateType == SPEED_CONTROL_RELAY_ACTIVE_LOW){
        #if defined (verboseDebug) && verboseDebug > 1
        Serial.print(F(" as digital power and pwm speed control, active low"));
        #endif
        pinMode(SpindlePowerControlPin, OUTPUT);
        if (powerState) { // turn on spindle
            Serial.println(F("Turning Spindle On"));
            digitalWrite(SpindlePowerControlPin, LOW);
            Serial.println(F("Adjusting Spindle Speed to Minimum"));
            setSpindleSpeed(sysSettings.spindleMin);
        }
        else {            // turn off spindle
            Serial.println(F("Turning Spindle Off"));
            digitalWrite(SpindlePowerControlPin, HIGH);
            digitalWrite(SpindleSpeedControlPin, LOW);
        }
    }
    if (spindleAutomateType == RELAY_ACTIVE_HIGH) {
        #if defined (verboseDebug) && verboseDebug > 1
        Serial.print(F(" as digital output, active high"));
        #endif
        pinMode(SpindlePowerControlPin, OUTPUT);
        if (powerState) { // turn on spindle
            Serial.println(F("Turning Spindle On"));
            digitalWrite(SpindlePowerControlPin, HIGH);
        }
        else {            // turn off spindle
            Serial.println(F("Turning Spindle Off"));
            digitalWrite(SpindlePowerControlPin, LOW);
        }
     }          
     else if (spindleAutomateType == RELAY_ACTIVE_LOW) {            // use a digital I/O pin to control a relay
        #if defined (verboseDebug) && verboseDebug > 1
        Serial.print(F(" as digital output, active low"));
        #endif
        pinMode(SpindlePowerControlPin, OUTPUT);
        if (powerState) { // turn on spindle
            Serial.println(F("Turning Spindle On"));
            digitalWrite(SpindlePowerControlPin, LOW);
        }
        else {            // turn off spindle
            Serial.println(F("Turning Spindle Off"));
            digitalWrite(SpindlePowerControlPin, HIGH);
        }
    }
    if (spindleAutomateType != NONE) {
        maslowDelay(delayAfterChange);
    }
}

int  setSpindleSpeed(int spindleSpeed){
  // map 0-255 min spindle speed to max spindle speed
  // output 0-255 
  if (spindleSpeed < sysSettings.spindleMin){
    spindleSpeed = sysSettings.spindleMin;
  }
  if (spindleSpeed > sysSettings.spindleMax){
    spindleSpeed = sysSettings.spindleMax;
  }
  long spd = map (spindleSpeed, 0, sysSettings.spindleMax, 0, 255);
  analogWrite(SpindleSpeedControlPin,spd);
  return(1);
  //for (i=0;i<spd;i++){
    //analogWrite(SpindleSpeedControlPin,i);
    //current_spindle_speed = i;
  //}
}

void laserOn() {
    Serial.println("Laser on");
    pinMode(LaserPowerPin, OUTPUT);
    digitalWrite(LaserPowerPin, HIGH);
}

void laserOff(){
    Serial.println("Laser off");
    pinMode(LaserPowerPin, OUTPUT);
    digitalWrite(LaserPowerPin, LOW);
}
