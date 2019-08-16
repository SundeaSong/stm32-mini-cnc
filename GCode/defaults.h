/*
  defaults.h - defaults settings configuration file
  Part of Grbl

  Copyright (c) 2012 Sungeun K. Jeon

  Grbl is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Grbl is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Grbl.  If not, see <http://www.gnu.org/licenses/>.
*/

/* The defaults.h file serves as a central default settings file for different machine
   types, from DIY CNC mills to CNC conversions of off-the-shelf machines. The settings
   here are supplied by users, so your results may vary. However, this should give you
   a good starting point as you get to know your machine and tweak the settings for your
   our nefarious needs. */

#ifndef defaults_h
#define defaults_h

#ifdef DEFAULTS_GENERIC
  // Grbl generic default settings. Should work across different machines.
  #define DEFAULT_X_STEPS_PER_MM 600.0
  #define DEFAULT_Y_STEPS_PER_MM 600.0
  #define DEFAULT_Z_STEPS_PER_MM 600.0
  #define DEFAULT_STEP_PULSE_MICROSECONDS 60
  #define DEFAULT_MM_PER_ARC_SEGMENT 0.1
  #define DEFAULT_RAPID_FEEDRATE 100.0 // mm/min
  #define DEFAULT_FEEDRATE 300.0
  #define DEFAULT_ACCELERATION (10.0*60*60) // 10 mm/min^2
  #define DEFAULT_JUNCTION_DEVIATION 0.05 // mm
  #define DEFAULT_STEPPING_INVERT_MASK 0x00//((1<<Y_DIRECTION_BIT)|(1<<Z_DIRECTION_BIT))
  #define DEFAULT_REPORT_INCHES 0 // false
  #define DEFAULT_AUTO_START 1 // true
  #define DEFAULT_INVERT_ST_ENABLE 0 // false
  #define DEFAULT_HARD_LIMIT_ENABLE 0  // false
  #define DEFAULT_HOMING_ENABLE 0  // false
  #define DEFAULT_HOMING_DIR_MASK 0 // move positive dir
  #define DEFAULT_HOMING_RAPID_FEEDRATE 150.0 // mm/min
  #define DEFAULT_HOMING_FEEDRATE 25.0 // mm/min
  #define DEFAULT_HOMING_DEBOUNCE_DELAY 100 // msec (0-65k)
  #define DEFAULT_HOMING_PULLOFF 1.0 // mm
  #define DEFAULT_STEPPER_IDLE_LOCK_TIME 25 // msec (0-255)
  #define DEFAULT_DECIMAL_PLACES 3
  #define DEFAULT_N_ARC_CORRECTION 25
#endif


#endif
