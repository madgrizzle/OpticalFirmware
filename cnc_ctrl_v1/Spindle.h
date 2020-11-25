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

#ifndef spindle_h
#define spindle_h

void  setSpindlePower(bool powerState);
void  laserOn();
void  laserOff();
int  setSpindleSpeed(int spindleSpd);
//extern int SpindlePowerControlPin;
//extern int SpindleSpeedPin;
//static bool SpindlePower;
//static int SpindleSpeed;
/* spindle speed is scaled from 0 to max with a start speed of the minimum
  S command will set the speed constrained between min and max
  M3 turns on
  M5 turns off
  spindle speed control pin is 45 by default
  spindle min is $60
  spindle max is $61
  in sysSettings
*/

#endif
