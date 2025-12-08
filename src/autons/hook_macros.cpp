#include "autons.h"
#include "globals.h"
#include "vex.h"

void leftHookPushFromGoal(double force){

  default_constants();
  chassis.swing_settle_time = 0;
  chassis.swing_timeout = 5000;
  chassis.drive_settle_time = 75;
  intakeMotor.setVelocity(100, percent);
  middleTravel.setVelocity(92.5, percent);
  scoreMotor.setVelocity(100, percent);

  chassis.set_coordinates(24, (48-8.5), 180);
  chassis.set_heading(180);
  stopSecondStage();
  intakeIn();
  intakeInterface.travel_status = IN_CHECK;

  toggleDescore();
  toggleDescore();
  
  chassis.drive_distance(1.25, 180);

  chassis.swing_max_voltage = 9.45;


  chassis.swing_settle_time = 0;
  chassis.right_swing_to_angle(27.5, false);

  chassis.swing_settle_time = 125;
  //chassis.right_swing_to_angle(310, false);
  chassis.right_swing_to_angle(313.25, false);


  
  chassis.DriveL.stop();
  chassis.DriveR.stop();



  chassis.swing_max_voltage = 3.85*force;

  chassis.right_hook(349.5,(0.6235));

  chassis.swing_timeout = 1750;

  chassis.swing_max_voltage = 6.85*force;
  chassis.right_hook(354.5,(0.4125));
  chassis.DriveL.stop();
  chassis.DriveR.stop();
  //chassis.swing_kd = 5;

  //chassis.left_hook(180,2/3);


}


void oldHook_leftHookPushFromGoal(double force){

  default_constants();
  chassis.swing_settle_time = 50;
  chassis.swing_timeout = 500;
  chassis.drive_settle_time = 75;
  intakeMotor.setVelocity(100, percent);
  middleTravel.setVelocity(92.5, percent);
  scoreMotor.setVelocity(100, percent);

  chassis.set_coordinates(24, (48-8.5), 180);
  chassis.set_heading(180);
  stopSecondStage();
  intakeIn();
  intakeInterface.travel_status = IN_CHECK;
  chassis.left_swing_to_angle(225);
  chassis.drive_distance(2.58,225);

  //chassis.set_turn_constants(12, 0.56525, 0.005475, 4.625,4);
  //chassis.set_turn_exit_conditions(1, 172.5, 975);
  chassis.set_turn_constants(12, 0.59625, 0.0059625, 5.8125,4);//test
  chassis.set_turn_exit_conditions(1, 58.5, 750);//test
  chassis.turn_to_angle(128.5); //was 128.5



  chassis.drive_max_voltage = 10;
  chassis.drive_min_voltage = 0;
  chassis.drive_distance(-10.45); //was -10.65
  
  //toggleDescore();
  intakeInterface.descoreDelay(72, msec);
  chassis.set_turn_constants(12, 0.56525, 0.005775, 4.625,4);
  chassis.set_turn_exit_conditions(1, 62.5, 975);
  chassis.turn_to_angle(196.215); //was 193.5

  chassis.right_hook(176.5, 2/3); //NEW

  chassis.drive_max_voltage = 4.8525 * force;
  
  chassis.drive_distance(-14.15,185.55); //was 184.5
  
  /*
  chassis.drive_distance(-8.5,185.55);
  toggleDescore();
  chassis.drive_distance(10,185.55);
  */

  /*
  chassis.drive_stop(hold);
  chassis.drive_max_voltage = 5.95 * force;
  chassis.drive_distance(-2.75,192.5); //was 182.5
  */

}

/*
void leftHookPushFromGoal(double force){

  default_constants();
  chassis.swing_settle_time = 50;
  chassis.swing_timeout = 500;
  chassis.drive_settle_time = 75;
  intakeMotor.setVelocity(100, percent);
  middleTravel.setVelocity(92.5, percent);
  scoreMotor.setVelocity(100, percent);

  chassis.set_coordinates(24, (48-8.5), 180);
  chassis.set_heading(180);
  stopSecondStage();
  intakeIn();
  intakeInterface.travel_status = IN_CHECK;
  chassis.left_swing_to_angle(225);
  chassis.drive_distance(2.58,225);

  //chassis.set_turn_constants(12, 0.56525, 0.005475, 4.625,4);
  //chassis.set_turn_exit_conditions(1, 172.5, 975);
  chassis.set_turn_constants(12, 0.59625, 0.0059625, 5.8125,4);//test
  chassis.set_turn_exit_conditions(1, 58.5, 750);//test
  chassis.turn_to_angle(128.5); //was 128.5



  chassis.drive_max_voltage = 10;
  chassis.drive_min_voltage = 0;
  chassis.drive_distance(-11.15); //was -10.65
  //toggleDescore();
  intakeInterface.descoreDelay(67, msec);
  chassis.set_turn_constants(12, 0.56525, 0.005775, 4.625,4);
  chassis.set_turn_exit_conditions(1, 62.5, 975);
  chassis.turn_to_angle(197.315); //was 193.5

  chassis.drive_max_voltage = 4.8525 * force;
  chassis.drive_distance(-14.15,186.95); //was 184.5

  chassis.drive_stop(hold);
  chassis.drive_max_voltage = 5.95 * force;
  chassis.drive_distance(-2.75,192.5); //was 182.5
}
*/