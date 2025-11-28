#include "vex.h"
#include "globals.h"
#include "auton_constants.h"

void default_constants(){
  // Each constant set is in the form of (maxVoltage, kP, kI, kD, startI).
  //chassis.set_drive_constants(12, 1.5, 0, 12, 0);
  chassis.set_drive_constants(10, 1.75, 0, 14, 0);
  chassis.set_heading_constants(12, .2, .01, 2, 0);
  //chassis.set_turn_constants(12, 0.4, 0.0125, 4,5);
  //chassis.set_turn_constants(10, 0.5, 0.0025, 4,5); //start at 0.4 P before october 8th
  chassis.set_turn_constants(10, 0.53125, 0.0040, 4,4);

  chassis.set_swing_constants(12, .3, .001, 2, 15);

  // Each exit condition set is in the form of (settle_error, settle_time, timeout).
  chassis.set_drive_exit_conditions(1, 1600, 5000);
  //chassis.set_turn_exit_conditions(2, 1500, 3000);//before october 8th
  chassis.set_turn_exit_conditions(1, 500, 1250);
  chassis.set_swing_exit_conditions(3, 600, 3000);
}

//DRIVE CONSTANTS:
void small_drive_constants(){
  chassis.set_drive_constants(10, 1.75, 0, 14, 0);
  chassis.set_drive_exit_conditions(1, 350, 1250);
}

void medium_drive_constants_forwards(){
  //chassis.set_drive_constants(12, 1.75, 0, 14, 0); before october 9th
  //chassis.set_drive_exit_conditions(1, 1400, 5000); before october 9th
  chassis.set_drive_constants(12, 1.75, 0.0025, 14.0, 0);
  chassis.set_drive_exit_conditions(1.0, 1400, 5000);
}

void medium_drive_constants_reverse(){
  //chassis.set_drive_constants(12, 1.75, 0, 14, 0); before october 9th
  //chassis.set_drive_exit_conditions(1, 1400, 5000); before october 9th
  chassis.set_drive_constants(10, 1.75, 0.0012, 16.0, 0);
  chassis.set_drive_exit_conditions(1.0, 1400, 5000);
}

void big_drive_constants_forwards(){
  //chassis.set_drive_constants(12, 1.75, 0, 14, 0); before october 9th
  //chassis.set_drive_exit_conditions(1, 1400, 5000); before october 9th
  chassis.set_drive_constants(12, 1.75, 0.0025, 14.0, 0);
  chassis.set_drive_exit_conditions(1.0, 1400, 5000);
}

void big_drive_constants_reverse(){
  //chassis.set_drive_constants(12, 1.75, 0, 14, 0); before october 9th
  //chassis.set_drive_exit_conditions(1, 1400, 5000); before october 9th
  chassis.set_drive_constants(10, 1.55, 0.0012, 16.0, 0);
  chassis.set_drive_exit_conditions(1.0, 1400, 5000);
}


//TURN CONSTANTS:
void small_turn_constants_10v(){
  chassis.set_turn_constants(10, 0.54525, 0.00475, 4.325,4);
  chassis.set_turn_exit_conditions(1, 207.5, 975);
}

void big_turn_medium_speed_constants_10v(){
  chassis.set_turn_constants(10, 0.53125, 0.0040, 4,4);
  chassis.set_turn_exit_conditions(1, 500, 1250);
}

void big_turn_fast_speed_constants_10v(){
  chassis.set_turn_constants(10, 0.53125, 0.00475, 4.4975,4);
  chassis.set_turn_exit_conditions(1, 297.5, 1075);
}

void small_turn_constants_12v(){
  chassis.set_turn_constants(12, 0.56525, 0.005475, 4.625,4);
  chassis.set_turn_exit_conditions(1, 172.5, 975);
}

void big_turn_fast_speed_constants_12v(){
  chassis.set_turn_constants(12, 0.56625, 0.0055625, 5.1625,4);
  chassis.set_turn_exit_conditions(1, 253.5, 1075);
}

void big_turn_fast_speed_constants_15v(){
  chassis.set_turn_constants(15, 0.56625, 0.0055625, 5.5925,4);
  chassis.set_turn_exit_conditions(1, 253.5, 1075);
}

void odom_constants(){
  default_constants();
  chassis.heading_max_voltage = 4;
  chassis.drive_max_voltage = 8;
  chassis.drive_settle_error = 1;
  chassis.boomerang_lead = .5;
  chassis.drive_min_voltage = 0;
}