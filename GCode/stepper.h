/*
  stepper.h - stepper motor driver: executes motion plans of planner.c using the stepper motors
  Part of Grbl

  Copyright (c) 2009-2011 Simen Svale Skogsrud
  Copyright (c) 2011 Sungeun K. Jeon

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
#include <stdint.h>



#ifndef stepper_h
#define stepper_h 


typedef struct {
  // Used by the bresenham line algorithm
  int32_t counter_x,        // Counter variables for the bresenham line tracer
          counter_y, 
          counter_z;
  uint32_t event_count;
  uint32_t step_events_completed;  // The number of step events left in current motion

  // Used by the trapezoid generator
  uint32_t cycles_per_step_event;        // The number of machine cycles between each step event
  uint32_t trapezoid_tick_cycle_counter; // The cycles since last trapezoid_tick. Used to generate ticks at a steady
                                              // pace without allocating a separate timer
  uint32_t trapezoid_adjusted_rate;      // The current rate of step_events according to the trapezoid generator
  uint32_t min_safe_rate;  // Minimum safe rate for full deceleration rate reduction step. Otherwise halves step_rate.
} stepper_t;


extern stepper_t st;
extern char ShowPos;
extern long int x_pos;
extern long int y_pos;
extern long int z_pos;
//aa#include <avr/io.h>

// Initialize and setup the stepper motor subsystem
void st_init(void);

// Enable steppers, but cycle does not start unless called by motion control or runtime command.
void st_wake_up(void);

// Immediately disables steppers
void st_go_idle(void);

// Reset the stepper subsystem variables       
void st_reset(void);
             
// Notify the stepper subsystem to start executing the g-code program in buffer.
void st_cycle_start(void);

// Reinitializes the buffer after a feed hold for a resume.
void st_cycle_reinitialize(void); 

// Initiates a feed hold of the running program
void st_feed_hold(void);
void DoOutPort(char out, char cmd);
#endif
