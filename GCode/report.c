/*
  report.c - reporting and messaging methods
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

/* 
  This file functions as the primary feedback interface for Grbl. Any outgoing data, such 
  as the protocol status messages, feedback messages, and status reports, are stored here.
  For the most part, these functions primarily are called from protocol.c methods. If a 
  different style feedback is desired (i.e. JSON), then a user can change these following 
  methods to accomodate their needs.
*/

//aa#include <avr/pgmspace.h>
#include "Config.h"
#include "config.h"
#include "report.h"
#include "print.h"
#include "settings.h"
#include "nuts_bolts.h"
#include "gcode.h"
#include "coolant_control.h"



// Handles the primary confirmation protocol response for streaming interfaces and human-feedback.
// For every incoming line, this method responds with an 'ok' for a successful command or an 
// 'error:'  to indicate some error event with the line or some critical system error during 
// operation. Errors events can originate from the g-code parser, settings module, or asynchronously
// from a critical error, such as a triggered hard limit. Interface should always monitor for these
// responses.
// NOTE: In silent mode, all error codes are greater than zero.
// TODO: Install silent mode to return only numeric values, primarily for GUIs.
void report_status_message(uint8_t status_code) 
{
  if (status_code == 0) { // STATUS_OK
    DebugPf("ok\r\n");
  } else {
    DebugPf("error: ");
    switch(status_code) {          
      case STATUS_BAD_NUMBER_FORMAT:
      DebugPf("Bad number format"); break;
      case STATUS_EXPECTED_COMMAND_LETTER:
      DebugPf("Expected command letter"); break;
      case STATUS_UNSUPPORTED_STATEMENT:
      DebugPf("Unsupported statement"); break;
      case STATUS_ARC_RADIUS_ERROR:
      DebugPf("Invalid radius"); break;
      case STATUS_MODAL_GROUP_VIOLATION:
      DebugPf("Modal group violation"); break;
      case STATUS_INVALID_STATEMENT:
      DebugPf("Invalid statement"); break;
      case STATUS_SETTING_DISABLED:
      DebugPf("Setting disabled"); break;
      case STATUS_SETTING_VALUE_NEG:
      DebugPf("Value < 0.0"); break;
      case STATUS_SETTING_STEP_PULSE_MIN:
      DebugPf("Value < 3 usec"); break;
      case STATUS_SETTING_READ_FAIL:
      DebugPf("EEPROM read fail. Using defaults"); break;
      case STATUS_IDLE_ERROR:
      DebugPf("Busy or queued"); break;
      case STATUS_ALARM_LOCK:
      DebugPf("Alarm lock"); break;
      case STATUS_OVERFLOW:
      DebugPf("Line overflow"); break;
    }
    DebugPf("\r\n");
  }
}

// Prints alarm messages.
void report_alarm_message(int8_t alarm_code)
{
  DebugPf("ALARM: ");
  switch (alarm_code) {
    case ALARM_HARD_LIMIT: 
    DebugPf("Hard limit"); break;
    case ALARM_ABORT_CYCLE: 
    DebugPf("Abort during cycle"); break;
  }
  DebugPf(". MPos?\r\n");
  _delay_ms(500); // Force delay to ensure message clears serial write buffer.
}

// Prints feedback messages. This serves as a centralized method to provide additional
// user feedback for things that are not of the status/alarm message protocol. These are
// messages such as setup warnings, switch toggling, and how to exit alarms.
// NOTE: For interfaces, messages are always placed within brackets. And if silent mode
// is installed, the message number codes are less than zero.
// TODO: Install silence feedback messages option in settings
void report_feedback_message(uint8_t message_code)
{
  DebugPf("[");
  switch(message_code) {
    case MESSAGE_CRITICAL_EVENT:
    DebugPf("Reset to continue"); break;
    case MESSAGE_ALARM_LOCK:
    DebugPf("'$H'|'$X' to unlock"); break;
    case MESSAGE_ALARM_UNLOCK:
    DebugPf("Caution: Unlocked"); break;
    case MESSAGE_ENABLED:
    DebugPf("Enabled"); break;
    case MESSAGE_DISABLED:
    DebugPf("Disabled"); break;    
  }
  DebugPf("]\r\n");
}


// Welcome message
void report_init_message()
{
  DebugPf("\r\nGrbl " GRBL_VERSION " ['$' for help]\r\n");
}

// Grbl help message
void report_grbl_help() {
  DebugPf("$$ (view Grbl settings)\r\n"
                      "$# (view # parameters)\r\n"
                      "$G (view parser state)\r\n"
                      "$N (view startup blocks)\r\n"
                      "$x=value (save Grbl setting)\r\n"
                      "$Nx=line (save startup block)\r\n"
                      "$C (check gcode mode)\r\n"
                      "$X (kill alarm lock)\r\n"
                      "$H (run homing cycle)\r\n"
                      "~ (cycle start)\r\n"
                      "! (feed hold)\r\n"
                      "? (current status)\r\n"
                      "ctrl-x (reset Grbl)\r\n");
}

// Grbl global settings print out.
// NOTE: The numbering scheme here must correlate to storing in settings.c
void report_grbl_settings() {
  DebugPf("$0="); printFloat(settings.steps_per_mm[X_AXIS]);
  DebugPf(" (x, step/mm)\r\n$1="); printFloat(settings.steps_per_mm[Y_AXIS]);
  DebugPf(" (y, step/mm)\r\n$2="); printFloat(settings.steps_per_mm[Z_AXIS]);
  DebugPf(" (z, step/mm)\r\n$3="); printInteger(settings.pulse_microseconds);
  DebugPf(" (step pulse, usec)\r\n$4="); printFloat(settings.default_feed_rate);
  DebugPf(" (default feed, mm/min)\r\n$5="); printFloat(settings.default_seek_rate);
  DebugPf(" (default seek, mm/min)\r\n$6="); printInteger(settings.invert_mask); 
  DebugPf(" (step port invert mask, int:"); print_uint8_base2(settings.invert_mask);  
  DebugPf(")\r\n$7="); printInteger(settings.stepper_idle_lock_time);
  DebugPf(" (step idle delay, msec)\r\n$8="); printFloat(settings.acceleration/(60*60)); // Convert from mm/min^2 for human readability
  DebugPf(" (acceleration, mm/sec^2)\r\n$9="); printFloat(settings.junction_deviation);
  DebugPf(" (junction deviation, mm)\r\n$10="); printFloat(settings.mm_per_arc_segment);
  DebugPf(" (arc, mm/segment)\r\n$11="); printInteger(settings.n_arc_correction);
  DebugPf(" (n-arc correction, int)\r\n$12="); printInteger(settings.decimal_places);
  DebugPf(" (n-decimals, int)\r\n$13="); printInteger(bit_istrue(settings.flags,BITFLAG_REPORT_INCHES));
  DebugPf(" (report inches, bool)\r\n$14="); printInteger(bit_istrue(settings.flags,BITFLAG_AUTO_START));
  DebugPf(" (auto start, bool)\r\n$15="); printInteger(bit_istrue(settings.flags,BITFLAG_INVERT_ST_ENABLE));
  DebugPf(" (invert step enable, bool)\r\n$16="); printInteger(bit_istrue(settings.flags,BITFLAG_HARD_LIMIT_ENABLE));
  DebugPf(" (hard limits, bool)\r\n$17="); printInteger(bit_istrue(settings.flags,BITFLAG_HOMING_ENABLE));
  DebugPf(" (homing cycle, bool)\r\n$18="); printInteger(settings.homing_dir_mask);
  DebugPf(" (homing dir invert mask, int:"); print_uint8_base2(settings.homing_dir_mask);  
  DebugPf(")\r\n$19="); printFloat(settings.homing_feed_rate);
  DebugPf(" (homing feed, mm/min)\r\n$20="); printFloat(settings.homing_seek_rate);
  DebugPf(" (homing seek, mm/min)\r\n$21="); printInteger(settings.homing_debounce_delay);
  DebugPf(" (homing debounce, msec)\r\n$22="); printFloat(settings.homing_pulloff);
  DebugPf(" (homing pull-off, mm)\r\n"); 
}


// Prints gcode coordinate offset parameters
void report_gcode_parameters()
{
  float coord_data[N_AXIS];
  uint8_t coord_select, i;
  for (coord_select = 0; coord_select <= SETTING_INDEX_NCOORD; coord_select++) { 
    if (!(settings_read_coord_data(coord_select,coord_data))) { 
      report_status_message(STATUS_SETTING_READ_FAIL); 
      return;
    } 
    DebugPf("[G");
    switch (coord_select) {
      case 0: DebugPf("54:"); break;
      case 1: DebugPf("55:"); break;
      case 2: DebugPf("56:"); break;
      case 3: DebugPf("57:"); break;
      case 4: DebugPf("58:"); break;
      case 5: DebugPf("59:"); break;
      case 6: DebugPf("28:"); break;
      case 7: DebugPf("30:"); break;
      // case 8: DebugPf("92:")); break; // G92.2, G92.3 not supported. Hence not stored.  
    }           
    for (i=0; i<N_AXIS; i++) {
      if (bit_istrue(settings.flags,BITFLAG_REPORT_INCHES)) { printFloat(coord_data[i]*INCH_PER_MM); }
      else { printFloat(coord_data[i]); }
      if (i < (N_AXIS-1)) { DebugPf(","); }
      else { DebugPf("]\r\n"); }
    } 
  }
  DebugPf("[G92:"); // Print G92,G92.1 which are not persistent in memory
  for (i=0; i<N_AXIS; i++) {
    if (bit_istrue(settings.flags,BITFLAG_REPORT_INCHES)) { printFloat(gc.coord_offset[i]*INCH_PER_MM); }
    else { printFloat(gc.coord_offset[i]); }
    if (i < (N_AXIS-1)) { DebugPf(","); }
    else { DebugPf("]\r\n"); }
  } 
}


// Print current gcode parser mode state
void report_gcode_modes()
{
  switch (gc.motion_mode) {
    case MOTION_MODE_SEEK : DebugPf("[G0"); break;
    case MOTION_MODE_LINEAR : DebugPf("[G1"); break;
    case MOTION_MODE_CW_ARC : DebugPf("[G2"); break;
    case MOTION_MODE_CCW_ARC : DebugPf("[G3"); break;
    case MOTION_MODE_CANCEL : DebugPf("[G80"); break;
  }

  DebugPf(" G");
  printInteger(gc.coord_select+54);
  
  if (gc.plane_axis_0 == X_AXIS) {
    if (gc.plane_axis_1 == Y_AXIS) { DebugPf(" G17"); }
    else { DebugPf(" G18"); }
  } else { DebugPf(" G19"); }
  
  if (gc.inches_mode) { DebugPf(" G20"); }
  else { DebugPf(" G21"); }
  
  if (gc.absolute_mode) { DebugPf(" G90"); }
  else { DebugPf(" G91"); }
  
  if (gc.inverse_feed_rate_mode) { DebugPf(" G93"); }
  else { DebugPf(" G94"); }
    
  switch (gc.program_flow) {
    case PROGRAM_FLOW_RUNNING : DebugPf(" M0"); break;
    case PROGRAM_FLOW_PAUSED : DebugPf(" M1"); break;
    case PROGRAM_FLOW_COMPLETED : DebugPf(" M2"); break;
  }

  switch (gc.spindle_direction) {
    case 1 : DebugPf(" M3"); break;
    case -1 : DebugPf(" M4"); break;
    case 0 : DebugPf(" M5"); break;
  }
  
  switch (gc.coolant_mode) {
    case COOLANT_DISABLE : DebugPf(" M9"); break;
    case COOLANT_FLOOD_ENABLE : DebugPf(" M8"); break;
    #ifdef ENABLE_M7
      case COOLANT_MIST_ENABLE : DebugPf(" M7"); break;
    #endif
  }
  
  DebugPf(" T");
  printInteger(gc.tool);
  
  DebugPf(" F");
  if (gc.inches_mode) { printFloat(gc.feed_rate*INCH_PER_MM); }
  else { printFloat(gc.feed_rate); }

  DebugPf("]\r\n");
}

// Prints specified startup line
void report_startup_line(uint8_t n, char *line)
{
  DebugPf("$N"); printInteger(n);
  DebugPf("="); DebugPf(line);
  DebugPf("\r\n");
}

 // Prints real-time data. This function grabs a real-time snapshot of the stepper subprogram 
 // and the actual location of the CNC machine. Users may change the following function to their
 // specific needs, but the desired real-time data report must be as short as possible. This is
 // requires as it minimizes the computational overhead and allows grbl to keep running smoothly, 
 // especially during g-code programs with fast, short line segments and high frequency reports (5-20Hz).
void report_realtime_status()
{
  // **Under construction** Bare-bones status report. Provides real-time machine position relative to 
  // the system power on location (0,0,0) and work coordinate position (G54 and G92 applied). Eventually
  // to be added are distance to go on block, processed block id, and feed rate. Also a settings bitmask
  // for a user to select the desired real-time data.
  uint8_t i;
  float print_position[3];
  int32_t current_position[3]; // Copy current state of the system position variable
  memcpy(current_position,sys.position,sizeof(sys.position));
 
  // Report current machine state
  switch (sys.state) {
    case STATE_IDLE: DebugPf("<Idle"); break;
//    case STATE_INIT: DebugPf("[Init")); break; // Never observed
    case STATE_QUEUED: DebugPf("<Queue"); break;
    case STATE_CYCLE: DebugPf("<Run"); break;
    case STATE_HOLD: DebugPf("<Hold"); break;
    case STATE_HOMING: DebugPf("<Home"); break;
    case STATE_ALARM: DebugPf("<Alarm"); break;
    case STATE_CHECK_MODE: DebugPf("<Check"); break;
  }
 
  // Report machine position
  DebugPf(",MPos:"); 
  for (i=0; i<= 2; i++) {
    print_position[i] = current_position[i]/settings.steps_per_mm[i];
    if (bit_istrue(settings.flags,BITFLAG_REPORT_INCHES)) { print_position[i] *= INCH_PER_MM; }
    printFloat(print_position[i]);
    DebugPf(",");
  }
  
  // Report work position
  DebugPf("WPos:"); 
  for (i=0; i<= 2; i++) {
    if (bit_istrue(settings.flags,BITFLAG_REPORT_INCHES)) {
      print_position[i] -= (gc.coord_system[i]+gc.coord_offset[i])*INCH_PER_MM;
    } else {
      print_position[i] -= gc.coord_system[i]+gc.coord_offset[i];
    }
    printFloat(print_position[i]);
    if (i < 2) { DebugPf(","); }
  }
    
  DebugPf(">\r\n");
}
