#include "autons.h"
#include "globals.h"
#include "vex.h"

void safe_progSkills(){
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
  chassis.drive_distance(-27.25, 270);
  small_turn_constants_12v();
  intakeIn();
  intakeInterface.travel_status = IN_CHECK;
  chassis.turn_to_angle(180);
  chassis.drive_timeout = 900.5;
  chassis.drive_distance(7.825, 180);
  wait(0.515,seconds); //was 1.5 seconds
  chassis.drive_max_voltage = 3.15;
  //chassis.drive_distance(1.616, 180);
  wait(0.55, seconds);
  //chassis.drive_timeout = 560.5; //was 662.5
  //chassis.drive_settle_time = 350;
  //chassis.drive_max_voltage = 3.25; //was 2.25
  //wait(0.025, seconds);
  chassis.drive_timeout = 1750;
  chassis.drive_settle_time = 0; //was 350
  chassis.drive_max_voltage = 11.5;
  toggleAligner();
  chassis.drive_distance(-23.5, 181.5);
  chassis.drive_settle_time = 150;
  highGoal();
  chassis.drive_timeout = 250;
  chassis.drive_max_voltage = 9.5;
  chassis.drive_distance(-8, 180);
  chassis.DriveL.stop();
  chassis.DriveR.stop();

  wait(1.275,seconds); //was 1.85 seconds
  stopSecondStage();
  wait(0.315,seconds);
  highGoal();
  wait(1.25,seconds);
  
  stopSecondStage();
  chassis.drive_timeout = 1750;
  chassis.drive_max_voltage = 10.5; //was 10.5
  toggleAligner();

  chassis.drive_distance(10,180);


  big_turn_fast_speed_constants_12v();

  chassis.turn_to_angle(-308.85);
  chassis.drive_max_voltage = 9.5;
  chassis.drive_distance(15.125,-329.75);

  chassis.set_turn_constants(12, 0.56625, 0.0058625, 5.1625,4);
  chassis.set_turn_exit_conditions(1, 128.5, 1075); //settle time could even go as low as 108 maybe, but I don't want to risk it on what should be a safe route
  chassis.turn_to_angle(10);

  chassis.drive_distance(60.85,359);
  chassis.drive_max_voltage = 8.125;
  
  chassis.drive_distance(6,355.25);

  chassis.set_turn_constants(12, 0.56625, 0.0055625, 5.1625,4);
  chassis.set_turn_exit_conditions(1, 253.5, 1075);
  chassis.turn_to_angle(307);

  //chassis.drive_to_point(133.25, 114.5);
  toggleMatchLoad();
  chassis.drive_max_voltage = 9.5;
  chassis.drive_distance(16.675); //was 16.475
  //chassis.drive_to_point(116.65, 115.5);
  chassis.turn_to_angle(359.5);
  chassis.drive_max_voltage = 9.5;
  intakeIn();
  intakeInterface.travel_status = IN_CHECK;
  chassis.drive_distance(11.487,0);
  chassis.drive_max_voltage = 3.5;
  chassis.drive_distance(3.167,0);
  wait(1, seconds);

  chassis.drive_max_voltage = 8;
  toggleAligner();
  chassis.drive_distance(-24, 357.105);
  highGoal();
  chassis.drive_distance(-9, 0.65);


  wait(1.325,seconds); //was 1.85 seconds
  stopSecondStage();
  wait(0.25,seconds);
  highGoal();
  wait(0.95,seconds);


  stopSecondStage();
  //chassis.drive_distance(8.5, 0);
  chassis.drive_max_voltage = 7.5;
  chassis.drive_settle_time = 500;
  //chassis.drive_to_point(118.75, (96+10.855+8)); //96 + 10.855 inches back + botlength/2
  chassis.drive_distance(15.2055,0);

  //chassis.set_turn_constants(12, 0.56625, 0.0055625, 5.1625,4); //old
  //chassis.set_turn_exit_conditions(1, 253.5, 1075); //old
  chassis.set_turn_constants(12, 0.56625, 0.0055625, 5.2825,4); //new
  chassis.set_turn_exit_conditions(1, 223.5, 1075); //new
  chassis.turn_to_angle(225);

  intakeIn();
  intakeInterface.travel_status = IN_CHECK;

  chassis.drive_settle_time = 150;
  chassis.drive_max_voltage = 8;
  chassis.drive_distance(18.25, 225);
  chassis.drive_max_voltage = 2.75;
  chassis.drive_distance(18.5, 225);

  chassis.set_turn_constants(12, 0.57125, 0.005575, 5.6875,4);
  chassis.set_turn_exit_conditions(1, 151.5, 975);
  chassis.turn_to_angle(46.15); //was 225

  scoreMotor.setVelocity(48.5, percent);
  intakeInterface.middleGoalDelay(205.5, msec);

  chassis.drive_max_voltage = 6.15;

  chassis.drive_distance(-11.715); //was -11.915
  chassis.DriveL.setVelocity(0, percent);
  chassis.DriveR.setVelocity(0, percent);
  
  //chassis.DriveL.setStopping(hold);
  //chassis.DriveR.setStopping(hold);
  wait(2.085, seconds); //was 1.085
  //chassis.DriveL.setStopping(coast);
  //chassis.DriveR.setStopping(coast);

  chassis.drive_max_voltage = 9.5;
  toggleMatchLoad();
  stopSecondStage();
  scoreMotor.stop();

  chassis.drive_distance(23);

  chassis.turn_to_angle(281.5);

  chassis.drive_timeout = 1750;
  chassis.swing_settle_time = 550;

  chassis.drive_distance(81.914265); //was 80.014265 could be also 76 depending on whether or not it was actually just timing out earlier
  
  //chassis.set_turn_constants(12, 0.57125, 0.005575, 5.6175,4); //old
  //chassis.set_turn_exit_conditions(1, 151.5, 975); //old
  chassis.set_turn_constants(12, 0.57625, 0.006275, 5.6175,4);
  chassis.set_turn_exit_conditions(1, 151.5, 975);
  chassis.turn_to_angle(0);
  
  intakeIn();
  intakeInterface.intake_status = IN;
  intakeInterface.travel_status = IN_CHECK;

  chassis.drive_timeout = 900.5;
  chassis.swing_settle_time = 150;
  chassis.drive_distance(11.625, 0); //was 11.925
  wait(0.75,seconds); //was 1.5 seconds
  chassis.drive_max_voltage = 4.25;
  chassis.drive_distance(1.396, 0);
  wait(0.55, seconds);
  //chassis.drive_timeout = 560.5; //was 662.5
  //chassis.drive_settle_time = 350;
  //chassis.drive_max_voltage = 3.25; //was 2.25
  //wait(0.025, seconds);
  chassis.drive_timeout = 1750;
  chassis.drive_settle_time = 0; //was 350
  chassis.drive_max_voltage = 11.5;

  toggleAligner();
  chassis.drive_distance(-23.5, 356.55);
  chassis.drive_settle_time = 150;
  highGoal();
  chassis.drive_timeout = 250;
  chassis.drive_max_voltage = 9.5;
  chassis.drive_distance(-8, 0.75);

  wait(2.475,seconds); //was 1.85 seconds
  
  stopSecondStage();
  chassis.drive_timeout = 1750;
  chassis.drive_max_voltage = 10.5; //was 10.5
  chassis.drive_distance(10,0);
  

  //GO PARK SAFETY

  chassis.set_turn_constants(12, 0.57125, 0.005575, 5.6175,4);
  chassis.set_turn_exit_conditions(1, 151.5, 975);
  chassis.turn_to_angle(315);

  chassis.drive_max_voltage = 10.5;
  chassis.drive_distance(-16.285);

  chassis.set_turn_constants(12, 0.57125, 0.005575, 5.6175,4);
  chassis.set_turn_exit_conditions(1, 151.5, 975);
  chassis.turn_to_angle(0);

  chassis.drive_max_voltage = 10.5;

  chassis.drive_timeout = 2500;
  chassis.drive_settle_time=500;
  chassis.drive_distance(-87.95, 358.05); //was -87.35
  
  chassis.set_turn_constants(12, 0.57125, 0.005575, 5.6175,4);
  chassis.set_turn_exit_conditions(1, 151.5, 975);
  chassis.turn_to_angle(102);
  chassis.drive_max_voltage = 15.5;
  chassis.drive_settle_time = 0;
  chassis.drive_distance(14.95, 105);
  toggleMatchLoad();
  chassis.turn_to_angle(95);
  wait(0.55, seconds);
  chassis.drive_settle_time=500;
  chassis.drive_max_voltage = 5000.5;
  intakeIn();
  intakeInterface.travel_status = IN_CHECK;
  chassis.drive_distance(37.25, 95.75); //was 37.5
  toggleMatchLoad();
  chassis.drive_distance(-2, 90);
  chassis.DriveL.setStopping(hold);
  chassis.DriveR.setStopping(hold);
  chassis.DriveL.stop();
  chassis.DriveR.stop();
  /*
  big_turn_fast_speed_constants_12v();

  chassis.turn_to_angle(180+51.15);
  chassis.drive_max_voltage = 9.5;
  chassis.drive_distance(15.125,180+30.25);
  chassis.turn_to_angle(10);
  chassis.drive_distance(60.85,180+1);
  chassis.drive_max_voltage = 8.125;
  
  chassis.drive_distance(6,180+4.75);

  chassis.turn_to_angle(180+53);

  //chassis.drive_to_point(133.25, 114.5);
  toggleMatchLoad();
  chassis.drive_max_voltage = 9.5;
  chassis.drive_distance(16.475); //was 17.25
  //chassis.drive_to_point(116.65, 115.5);
  chassis.turn_to_angle(180);
  chassis.drive_max_voltage = 9.5;
  intakeIn();
  intakeInterface.travel_status = IN_CHECK;
  chassis.drive_distance(11.487,180);
  chassis.drive_max_voltage = 3.5;
  chassis.drive_distance(3.967,180);
  wait(1, seconds);
  chassis.drive_max_voltage = 8;
  chassis.drive_distance(-24, 3);
  toggleMatchLoad();
  highGoal();
  chassis.drive_distance(-9, -0.65);
  wait(1.875, seconds);
  stopSecondStage();
  */

}
