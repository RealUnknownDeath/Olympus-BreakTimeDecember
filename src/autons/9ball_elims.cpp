#include "autons.h"
#include "globals.h"
#include "vex.h"

void blueRightElims9Ball(){
  rightElims9Ball(false);
}
void redRightElims9Ball(){
  rightElims9Ball(true);
}

void rightElims9Ball(bool allienceColor){ //WORKED ON
  //sorter.startColorSort();
  finalSortStatus = BLUE;
  defaultSortStatus = BLUE;
  default_constants();
  chassis.swing_settle_time = 50;
  chassis.swing_timeout = 500;
  chassis.drive_settle_time = 75;
  chassis.drive_timeout = 2000;
  intakeMotor.setVelocity(100, percent);
  middleTravel.setVelocity(85, percent);
  scoreMotor.setVelocity(100, percent);

  intakeInterface.startIntakeThread();

  intakeIn();
  intakeInterface.travel_status = IN_CHECK;


  chassis.right_swing_to_angle(25.5); //was 25.5
  chassis.swing_timeout = 1000;

  chassis.set_coordinates(84, 22.5, chassis.get_absolute_heading());
  //toggleDescore();

  chassis.drive_max_voltage = 12; //was 10
  chassis.drive_distance(16.65,20);
  toggleAligner();
  chassis.drive_max_voltage = 1.875; //was 2.25
  chassis.drive_distance(5.325,20);
  chassis.drive_distance(5.7, 28.5); //was 26.5
  chassis.drive_max_voltage = 12;
  //chassis.drive_distance(8.125);
  //chassis.swing_max_voltage = 12;
  //chassis.left_swing_to_angle(359.5); //was 0
  chassis.right_swing_to_angle(49.565); //was 47.565
  //chassis.right_swing_to_angle(52.5);
  chassis.drive_max_voltage = 12; //was 10
  chassis.drive_distance(22.55);
  chassis.drive_max_voltage = 2.575; //was 2.875
  chassis.drive_distance(9.256); //was 9.456 but was sometimes hitting the bar
  //toggleMatchLoad();
  //wait(0.075,seconds); //let the matchload hit the other color balls
  chassis.drive_max_voltage = 7.5; //was 7.5
  //chassis.drive_distance(-6.25, 45); //7.25 and before that 10.5
  chassis.drive_settle_time = 200; //was 175 and also 125
  //raise();
  //chassis.drive_to_point(113.75,54.6425);
  //chassis.drive_to_point(113.35,53.1925);
  chassis.drive_timeout = 3000;
  chassis.drive_to_point(110.55,54.6925);

  //Controller1.rumble(".");

  chassis.drive_settle_time = 75;
  chassis.drive_timeout = 2000;
  chassis.drive_max_voltage = 12; //was 10
  //toggleDescore();

  chassis.swing_timeout = 285;
  chassis.swing_settle_time = 85; //was 685 then 585 then 515
  //chassis.left_swing_to_angle(267.85, true); //was 265.85
  chassis.left_swing_to_angle(267.85);
  chassis.drive_distance(-2.25);

  intakeIn();
  intakeInterface.travel_status = IN_CHECK;

  //wait(2,seconds);
  chassis.swing_timeout = 1547; //was 1587
  chassis.swing_max_voltage = 15;

  chassis.left_swing_to_angle(162.8685, true); //was 170
  chassis.swing_timeout = 265; //was 425
  chassis.drive_timeout = 25;
  highGoal();
  chassis.drive_timeout = 500;
  chassis.drive_distance(-2, 180);
  wait(0.50,seconds);
  intakeInterface.travel_status = OFF_travel;
  wait(0.05,seconds);
  chassis.set_coordinates(119, (48-8.5), chassis.get_absolute_heading());
  stopSecondStage();
  toggleMatchLoad();
  intakeIn();
  intakeInterface.travel_status = IN_CHECK;
  chassis.drive_timeout = 2000;
  chassis.drive_max_voltage = 12; //was 10
  //chassis.drive_timeout = 2000;
  //chassis.drive_settle_error = 1.5;
  //chassis.drive_settle_time = 750;
  //chassis.drive_to_point(120,25.5);
  chassis.drive_distance(18.225,180); //was 181.65
  //wait(10,seconds);
  //small_turn_constants_12v();
  chassis.drive_max_voltage = 5.5; //was 6.5 and also 8.5
  //scoreMotor.spin(reverse);
  //middleTravel.spin(reverse);
  chassis.drive_timeout = 575; //was 2000
  chassis.drive_settle_time = 250;

  chassis.drive_distance(11.85,180); //was 181.65 and 9.95
  //toggleAligner();
  //stopSecondStage();
  intakeIn();
  intakeInterface.travel_status = IN_CHECK;
  chassis.drive_timeout = 585;
  chassis.drive_settle_time = 225;
  //wait(0.35, seconds);
  //chassis.drive_distance(-4.75, 180);
  //wait(0.2,seconds);
  //chassis.drive_distance(8.55, 180);
  //chassis.drive_distance(2.945,180); //was 2.745 inches
  wait(0.5485, seconds); //was 0.5585 seconds
  //chassis.left_swing_to_angle(180);
  //wait(20, seconds);
  //wait(0.32765, seconds);

  /*
  chassis.drive_distance(-28.235, 179.75); //was 172.125 then 176.625 also the inches was -24
  toggleMatchLoad();
  chassis.drive_timeout = 5000;
  highGoal();
  chassis.drive_max_voltage = 8;
  chassis.drive_distance(-5, 180);
  */
  chassis.drive_timeout = 1750;
  chassis.drive_max_voltage = 12; //was 10
  chassis.drive_settle_time = 250;
  chassis.drive_settle_error = 2;
  //chassis.drive_to_point(119.2175, 32.15);
  intakeInterface.highGoalDelay(475, msec);
  toggleAligner();
  chassis.drive_distance(-30.75,180);

  chassis.drive_max_voltage = 12;
  highGoal();

  chassis.drive_timeout = 250;
  chassis.drive_timeout = 75;
  chassis.drive_distance(-3.85);
  //wait(1.85,seconds);
  //NEW SCORE ALL THEN PUSH AND HOLD CODE:
  wait(1.15, seconds);
  
  chassis.drive_timeout = 350;
  intakeInterface.travel_status = IN_CHECK;
  scoreMotor.stop();
  toggleDescore();
  chassis.drive_distance(8.5);
  chassis.drive_max_voltage = 15;
  chassis.drive_distance(-17.5);

  //OLD PUSH THEN SCORE 1 MORE CODE:
  /*
  wait(0.95, seconds);
  
  chassis.drive_timeout = 350;
  intakeInterface.travel_status = IN_CHECK;
  scoreMotor.stop();
  toggleDescore();
  chassis.drive_distance(8.5);
  chassis.drive_max_voltage = 15;
  chassis.drive_distance(-17.5);
  toggleDescore();
  scoreMotor.spin(forward);
  intakeInterface.travel_status = IN_BLIND;
  */


}