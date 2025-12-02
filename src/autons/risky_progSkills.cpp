#include "autons.h"
#include "globals.h"
#include "vex.h"
void lastBit(){
  default_constants();
  chassis.swing_settle_time = 50;
  chassis.swing_timeout = 500;
  chassis.drive_settle_time = 75;
  intakeInterface.startIntakeThread();
  intakeMotor.setVelocity(100, percent);
  middleTravel.setVelocity(92.5, percent); //was 85
  scoreMotor.setVelocity(100, percent);

  intakeInterface.setIntakeVelocity(100);
  intakeInterface.setTravelVelocity(92.5);

  //TEMPORARY:
  /*
  toggleAligner();
  wait(0.5,seconds);
  chassis.drive_max_voltage = 11.5;
  chassis.drive_distance(-22.5);
  wait(1, seconds);
  */
  
  //chassis.set_heading(0);

  chassis.drive_timeout = 1750;
  chassis.drive_max_voltage = 12.5; //was 10.5
  chassis.drive_distance(10,0);


  chassis.set_turn_constants(12, 0.57125, 0.005875, 5.6575,4);
  chassis.set_turn_exit_conditions(1, 85.5, 975);
  chassis.turn_to_angle(315);

  chassis.drive_max_voltage = 10.5;
  chassis.drive_distance(-16.485); //was -16.285

  chassis.set_turn_constants(12, 0.57125, 0.005975, 5.6875,4);
  chassis.set_turn_exit_conditions(1, 65.5, 975);
  chassis.turn_to_angle(359.85); //was 0

  chassis.drive_max_voltage = 12; //was 10.5

  chassis.drive_timeout = 2500;
  chassis.drive_settle_time=750;
  //chassis.drive_distance(-87.95, 358.05); //was -87.35
  
  //movement across the field down to the middle goal again and the 4th matchloader
  //chassis.drive_distance(-64.65, 0); //was -64.95 and 358.05

  //chassis.drive_distance(-56.65, 0);
  chassis.drive_distance(-55.45, 357.885); //was -56.65

  //chassis.drive_distance(-9.75, 3.55);
  chassis.drive_distance(-9.75, 1.85);

  intakeIn();
  intakeInterface.travel_status = IN_CHECK;
  chassis.set_turn_constants(12, 0.57125, 0.005575, 5.6175,4);
  chassis.set_turn_exit_conditions(1, 151.5, 975);
  chassis.turn_to_angle(45);
  
  chassis.drive_settle_time=250;
  //chassis.drive_to_point(44.65, 48);
  chassis.drive_max_voltage = 6.55;
  chassis.drive_distance(19.5, 45);


  //will make 3rd ball much more consistent
  chassis.turn_timeout = 640;
  chassis.set_turn_constants(12, 0.57125, 0.005575, 5.6175,4);
  chassis.set_turn_exit_conditions(1, 151.5, 975);
  chassis.turn_to_angle(223.75); //was 221.5
  
  chassis.drive_max_voltage = 10.5; //was 10
  chassis.drive_timeout = 750;
  chassis.drive_settle_time = 350;

  scoreMotor.setVelocity(42.5, percent); //was 42.5
  intakeInterface.middleGoalDelay(95, msec); //was 90
  chassis.drive_max_voltage = 10.5;
  chassis.drive_distance(-15.005,224.85); //was 223.85 and at another point -15.185

  //toggleColorSort();


  wait((0.425), seconds); //was 0.825 seconds
  //scoreMotor.setVelocity(-50, percent);
  //wait((1.175/2), seconds);
  scoreMotor.setVelocity(100, percent);
  scoreMotor.stop();
  intakeMotor.stop();
  middleTravel.stop();
  intakeInterface.travel_status = OFF_travel;
  intakeInterface.intake_status = OFF_intake;


  chassis.drive_timeout = 1950;
  chassis.drive_settle_time = 750;
  chassis.drive_max_voltage = 12;

  chassis.drive_distance(47.985, 228.5); //was 47.455 and 47.155
  //chassis.drive_distance(48.685, 228.5); //was 48.285

  intakeIn();
  intakeInterface.travel_status = IN_CHECK;

  small_turn_constants_12v();
  chassis.turn_timeout = 650;
  chassis.set_turn_constants(12, 0.57125, 0.005575, 5.6175,4);
  chassis.set_turn_exit_conditions(1, 151.5, 975);
  chassis.turn_to_angle(180); //was 179.5
  
  toggleMatchLoad();
  wait(0.45, seconds);

  /*
  chassis.drive_timeout = 875;
  chassis.drive_settle_time = 150;
  chassis.drive_distance(12.05);

  chassis.drive_max_voltage = 4.865;
  chassis.drive_distance(2.05);

  wait(0.785, seconds);
  */
  chassis.ml_hold_time_ms = 550;
  chassis.right_front_sensor_drive_distance(3.125, 18.1, 180);

  chassis.drive_max_voltage = 12.625;
  chassis.drive_timeout = 595;
  chassis.drive_settle_time = 250;
  chassis.drive_settle_time = 0;
  chassis.drive_max_voltage = 5000;

  toggleAligner();
  intakeInterface.highGoalDelay(735, msec);
  chassis.ml_hold_time_ms = 0;
  chassis.right_front_sensor_drive_distance(32.0, 18.0, 180);
  //chassis.drive_distance(-25.05, 178.35); //was -11.65
  highGoal();

  chassis.DriveR.setVelocity(0, percent);
  chassis.DriveL.setVelocity(0, percent);
  chassis.DriveL.stop();
  chassis.DriveR.stop();

  //chassis.drive_distance(-8.5, 179.65);
  wait(1.425,seconds);

  stopSecondStage();


  intakeIn();
  intakeInterface.travel_status = OFF_travel;

  chassis.drive_max_voltage = 12;
  chassis.drive_timeout = 1000;
  highGoal();
  intakeInterface.intake_status = OFF_intake;
  intakeMotor.stop();
  chassis.drive_distance(8.5);
  chassis.set_turn_constants(12, 0.57125, 0.005575, 5.6175,4);
  chassis.set_turn_exit_conditions(1, 68.5, 975);
  chassis.turn_to_angle(135);
  chassis.drive_timeout = 1500;
  middleTravel.stop();
  intakeInterface.travel_status = OFF_travel;
  chassis.drive_distance(25.45); //was 26.5
  


  chassis.set_turn_constants(12, 0.57125, 0.005575, 5.6175,4);
  chassis.set_turn_exit_conditions(1, 68.5, 975);
  stopSecondStage();
  chassis.turn_to_angle(115);
  chassis.drive_max_voltage = 15.5;
  chassis.drive_settle_time = 0;
  chassis.drive_timeout = 650;
  chassis.drive_distance(11.95, 115); //was 14.95
  toggleMatchLoad();
  chassis.set_turn_constants(12, 0.57125, 0.005575, 5.6175,4);
  chassis.set_turn_exit_conditions(1, 65.5, 975);
  chassis.turn_to_angle(97.5);
  wait(0.125, seconds);
  chassis.drive_settle_time=500;
  chassis.drive_max_voltage = 5000.5;
  intakeIn();
  intakeInterface.travel_status = OFF_travel;
  chassis.drive_timeout = 1200;
  chassis.drive_distance(39.85, 101.45); //was 37.5 and the angle was 98.75
  toggleMatchLoad();
  chassis.drive_distance(-2.05, 90);
  chassis.DriveL.setStopping(hold);
  chassis.DriveR.setStopping(hold);
  chassis.DriveL.stop();
  chassis.DriveR.stop();
}
void risky_progSkills(){
  default_constants();
  chassis.swing_settle_time = 50;
  chassis.swing_timeout = 500;
  chassis.drive_settle_time = 75;
  intakeInterface.startIntakeThread();
  intakeMotor.setVelocity(100, percent);
  middleTravel.setVelocity(92.5, percent); //was 85
  scoreMotor.setVelocity(100, percent);

  intakeInterface.setIntakeVelocity(100);
  intakeInterface.setTravelVelocity(92.5);

  //chassis.set_coordinates(96-7.25, 18.5, 270);
  chassis.set_coordinates(96-4.25, 18.5, 270);

  toggleMatchLoad();
  big_drive_constants_forwards();
  chassis.drive_timeout = 1500;
  chassis.drive_settle_time = 350;
  chassis.drive_max_voltage = 12;
  //chassis.drive_distance(-30.25, 270);
  chassis.drive_distance(-28.35, 270);
  intakeIn();
  intakeInterface.travel_status = IN_CHECK;

  chassis.set_turn_constants(12, 0.56525, 0.005475, 4.925,4);
  chassis.set_turn_exit_conditions(1, 100.5, 975);
  chassis.turn_to_angle(180);
  chassis.ml_hold_time_ms = 550;
  chassis.left_front_sensor_drive_distance(3.125, 17.6, 180); //was 6.2 front distance right matchloader

  
  /*
  chassis.drive_timeout = 900.5;
  chassis.drive_distance(7.865, 180); //was 7.265
  chassis.drive_max_voltage = 3.15;
  wait(0.8725, seconds);
  */
  //chassis.drive_timeout = 560.5; //was 662.5
  //chassis.drive_settle_time = 350;
  //chassis.drive_max_voltage = 3.25; //was 2.25
  //wait(0.025, seconds);
  chassis.drive_timeout = 1750;
  chassis.drive_settle_time = 0; //was 350
  chassis.drive_max_voltage = 11.5;
  toggleAligner();
  /*
  chassis.drive_distance(-23.5, 181.5);
  chassis.drive_settle_time = 150;
  highGoal();
  chassis.drive_timeout = 250;
  chassis.drive_max_voltage = 9.5;
  chassis.drive_distance(-8, 180);
  */

  chassis.ml_max_forward_voltage = 12;
  chassis.ml_hold_time_ms = 0;
  chassis.left_front_sensor_drive_distance(32.0, 17.6, 180); //right long goal
  highGoal();

  chassis.DriveL.stop();
  chassis.DriveR.stop();

  //chassis.set_heading(180);
  wait(1.445,seconds); //was 1.85 seconds

  //SAFETY CODE:
  /*
  stopSecondStage();
  wait(0.315,seconds);
  highGoal();
  wait(0.85,seconds); //was 1.25
  */
  
  stopSecondStage();
  chassis.drive_timeout = 1750;
  chassis.drive_max_voltage = 10.5; //was 10.5
  toggleAligner();

  chassis.drive_settle_time = 250;
  chassis.drive_distance(10,180);


  chassis.set_turn_constants(12, 0.56625, 0.0055625, 5.2325,4);
  chassis.set_turn_exit_conditions(1, 213.5, 1075);
  chassis.turn_to_angle(-308.85);
  chassis.drive_max_voltage = 9.5;
  chassis.drive_distance(17.325,-329.75); //was 15.125

  chassis.set_turn_constants(12, 0.56625, 0.0058625, 5.1625,4);
  chassis.set_turn_exit_conditions(1, 60.5, 1075); //settle time could even go as low as 108 maybe, but I don't want to risk it on what should be a safe route
  chassis.turn_to_angle(10);

  chassis.drive_max_voltage = 11.5; //was 12

  chassis.drive_settle_time = 500;
  chassis.drive_distance(56.85,1.15); //was 60.85
  chassis.drive_max_voltage = 8.125;
  
  chassis.drive_settle_time = 250;
  chassis.drive_distance(6,355.25);

  chassis.set_turn_constants(12, 0.56625, 0.0055625, 5.1625,4);
  chassis.set_turn_exit_conditions(1, 85.5, 1075);
  chassis.turn_to_angle(307);

  //chassis.drive_to_point(133.25, 114.5);
  toggleMatchLoad();
  chassis.drive_max_voltage = 9.5;
  chassis.drive_settle_time = 350;
  chassis.drive_distance(17.6225); //was 15.3025
  //chassis.drive_to_point(116.65, 115.5);
  chassis.set_turn_constants(12, 0.56625, 0.0058825, 5.1625,4);
  chassis.set_turn_exit_conditions(1, 85.5, 1075);
  chassis.turn_to_angle(359.5);
  chassis.drive_max_voltage = 9.5;
  intakeIn();
  intakeInterface.travel_status = IN_CHECK;

  /*
  chassis.drive_settle_time = 250;
  chassis.drive_distance(11.777,0); //was 12.027
  chassis.drive_max_voltage = 3.5;
  chassis.drive_distance(2.967,0);
  wait(0.85, seconds);
  */

  chassis.ml_hold_time_ms = 550;
  chassis.right_front_sensor_drive_distance(3.125, 18.1, 0); //left matchloader

  chassis.drive_max_voltage = 10.65;
  toggleAligner();

  chassis.ml_hold_time_ms = 0;
  chassis.right_front_sensor_drive_distance(32.0, 18.0, 0);
  highGoal();


  wait(0.85,seconds); //was 1.185 seconds

  //SAFTEY CODE:
  /*
  stopSecondStage();
  wait(0.25,seconds);
  highGoal();
  wait(0.75,seconds);
  */


  stopSecondStage();
  //chassis.drive_distance(8.5, 0);
  chassis.drive_max_voltage = 9.5; //was 10.5
  chassis.drive_settle_time = 500;
  //chassis.drive_to_point(118.75, (96+10.855+8)); //96 + 10.855 inches back + botlength/2
  chassis.drive_distance(15.2885,0); //was 15.2855

  //chassis.set_turn_constants(12, 0.56625, 0.0055625, 5.1625,4); //old
  //chassis.set_turn_exit_conditions(1, 253.5, 1075); //old
  chassis.set_turn_constants(12, 0.56625, 0.0055625, 5.3425,4); //new
  chassis.set_turn_exit_conditions(1, 223.5, 1075); //new
  chassis.turn_to_angle(225);

  intakeIn();
  intakeInterface.travel_status = IN_CHECK;
  scoreMotor.spin(forward);

  chassis.drive_settle_time = 150;
  chassis.drive_max_voltage = 8;
  chassis.drive_distance(18.25, 225);
  chassis.drive_max_voltage = 2.75;
  scoreMotor.stop();
  chassis.drive_distance(18.5, 225);

  chassis.set_turn_constants(12, 0.57125, 0.005575, 5.6875,4);
  chassis.set_turn_exit_conditions(1, 151.5, 975);
  chassis.turn_to_angle(46.45); //was 46.15

  scoreMotor.setVelocity(42.5, percent); //was 48.5
  intakeInterface.middleGoalDelay(215.5, msec); //was 205.5

  chassis.drive_max_voltage = 7.45; //was 6.15

  chassis.drive_distance(-11.585); //was -11.915
  chassis.DriveL.setVelocity(0, percent);
  chassis.DriveR.setVelocity(0, percent);
  
  //chassis.DriveL.setStopping(hold);
  //chassis.DriveR.setStopping(hold);
  wait(1.685, seconds); //was 0.985
  //chassis.DriveL.setStopping(coast);
  //chassis.DriveR.setStopping(coast);


  stopSecondStage();
  scoreMotor.stop();
  scoreMotor.setVelocity(100, percent);

  chassis.drive_max_voltage = 11.25;
  chassis.drive_distance(23);

  chassis.turn_to_angle(281.5);

  chassis.drive_timeout = 1750;
  chassis.swing_settle_time = 550;

  chassis.drive_max_voltage = 11.45;
  //movement across the field for 3rd matchloader
  chassis.drive_distance(80.844265); //was 80.944265
  
  //chassis.set_turn_constants(12, 0.57125, 0.005575, 5.6175,4); //old
  //chassis.set_turn_exit_conditions(1, 151.5, 975); //old
  chassis.set_turn_constants(12, 0.57625, 0.006975, 5.5575,4);
  chassis.set_turn_exit_conditions(1, 151.5, 975);
  chassis.turn_to_angle(356.85); //was 357.25
  toggleMatchLoad();
  wait(0.45, seconds);
  
  intakeIn();
  intakeInterface.intake_status = IN;
  intakeInterface.travel_status = IN_CHECK;

  /*
  chassis.drive_timeout = 900.5;
  chassis.swing_settle_time = 150;
  chassis.drive_distance(8.485, 0); //was 11.925
  chassis.drive_max_voltage = 4.25;
  chassis.drive_distance(3.646, 0); //was 3.596
  wait(1.005, seconds);
  */
  chassis.ml_hold_time_ms = 550;
  chassis.left_front_sensor_drive_distance(3.125, 17.6, 0);
  //chassis.drive_timeout = 560.5; //was 662.5
  //chassis.drive_settle_time = 350;
  //chassis.drive_max_voltage = 3.25; //was 2.25
  //wait(0.025, seconds);
  chassis.drive_timeout = 1750;
  chassis.drive_settle_time = 0; //was 350
  chassis.drive_max_voltage = 11.5;

  toggleAligner();
  /*
  chassis.drive_distance(-23.5, 356.35);
  chassis.drive_settle_time = 150;
  highGoal();
  chassis.drive_timeout = 250;
  chassis.drive_max_voltage = 9.5;
  chassis.drive_distance(-8, 359.5);
  */
  chassis.ml_hold_time_ms = 0;
  chassis.left_front_sensor_drive_distance(33.0, 18.0, 0);
  
  highGoal();
  wait(1.475,seconds); //was 1.85 seconds
  
  stopSecondStage();

  //LAST BIT:

  lastBit();
  
  //DELETED EVERYTHING AFTER:
}
