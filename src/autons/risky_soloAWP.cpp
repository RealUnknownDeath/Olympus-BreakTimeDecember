#include "autons.h"
#include "globals.h"
#include "vex.h"

void risky_soloAWP(){
  finalSortStatus = BLUE;
  defaultSortStatus = NONE;
  //sorter.startColorSort();
  default_constants();
  chassis.swing_settle_time = 50;
  chassis.swing_timeout = 500;
  chassis.drive_settle_time = 75;
  intakeMotor.setVelocity(100, percent);
  middleTravel.setVelocity(92.5, percent); //was 85
  scoreMotor.setVelocity(100, percent);

  chassis.set_coordinates(96-7.25, 18.5, 270);
  toggleMatchLoad();
  toggleAligner();
  big_drive_constants_forwards();
  chassis.drive_timeout = 1500;
  chassis.drive_settle_time = 315;
  chassis.drive_max_voltage = 12;
  chassis.drive_distance(-30.075, 270); //was -30.25
  intakeInterface.startIntakeThread();

  intakeIn();
  intakeInterface.travel_status = IN_CHECK;

  //chassis.set_turn_constants(12, 0.56525, 0.005475, 4.625,4); old
  chassis.set_turn_constants(12, 0.56525, 0.005475, 5.835,4); //new
  chassis.set_turn_exit_conditions(1, 172.5, 975);
  //small_turn_constants_12v();
  
  chassis.turn_to_angle(180);


  chassis.drive_timeout = 645.5; //was 900.5
  chassis.drive_settle_time = 350;
  chassis.drive_distance(7.225, 180);
  wait(0.275, seconds);
  //chassis.drive_timeout = 560.5; //was 662.5
  //chassis.drive_settle_time = 350;
  //chassis.drive_max_voltage = 3.25; //was 2.25
  //wait(0.025, seconds);
  chassis.drive_timeout = 1750;
  chassis.drive_settle_time = 0; //was 350
  chassis.drive_max_voltage = 15.5; //was 12.5
  chassis.drive_distance(-25.45, 182.5);
  chassis.drive_settle_time = 150;
  highGoal();
  chassis.drive_timeout = 250;
  chassis.drive_max_voltage = 12.5; //was 9.5
  chassis.drive_distance(-7.5, 180); //was -5
  toggleMatchLoad();

  wait(0.615,seconds);
  intakeInterface.travel_status = OFF_travel;
  wait(0.15,seconds);

  chassis.set_coordinates(119, (48-8.5), chassis.get_absolute_heading());

  stopSecondStage();
  chassis.drive_timeout = 1750;
  chassis.drive_max_voltage = 10.5; //was 10.5
  chassis.drive_distance(10,180);


  chassis.set_turn_constants(12, 0.56625, 0.0055625, 5.975,4);
  chassis.set_turn_exit_conditions(1, 223.5, 1075);
  scoreMotor.spin(forward);
  chassis.turn_to_angle(308);
  scoreMotor.stop();

  intakeIn();
  intakeInterface.travel_status = IN_CHECK;

  
  chassis.drive_settle_time = 150;
  chassis.drive_max_voltage = 12;
  chassis.drive_distance(17.105,305.5); //was 307.5
  chassis.drive_settle_time = 150;
  chassis.drive_max_voltage = 8.875; //was 5.875 
  chassis.drive_distance(11.25,296.125);
  chassis.drive_max_voltage = 12;
  chassis.drive_distance(35.24, 255.5);
  chassis.drive_settle_time = 150;
  chassis.drive_max_voltage = 8.875; //was 3.875
  //chassis.drive_distance(12.75, 270); //keep this more angled and acount for it later
  chassis.drive_to_point(45.995, 49.465);
  
  //will make 3rd ball much more consistent
  chassis.turn_timeout = 640;
  chassis.turn_to_angle(221.5); //was 225, then 215
  
  chassis.drive_max_voltage = 10.5; //was 10
  chassis.drive_timeout = 750;
  chassis.drive_settle_time = 350;

  scoreMotor.setVelocity(48.5, percent); //was 42.5
  //chassis.drive_distance(-9.175, 225.85);
  middleGoal();
  //chassis.drive_distance(-5.8, 225.85);
  chassis.drive_max_voltage = 12;
  chassis.drive_distance(-16.2,223.85);

  //toggleColorSort();


  wait((0.365), seconds);
  //scoreMotor.setVelocity(-50, percent);
  //wait((1.175/2), seconds);
  scoreMotor.setVelocity(100, percent);
  scoreMotor.spin(reverse);
  intakeMotor.stop();
  middleTravel.stop();
  intakeInterface.travel_status = OFF_travel;
  intakeInterface.intake_status = OFF_intake;


  chassis.drive_timeout = 1950;
  chassis.drive_settle_time = 750;
  chassis.drive_max_voltage = 12;
  toggleMatchLoad();
  //chassis.drive_distance(52.955, 222.5);
  chassis.drive_to_point(21.145, 23.835);

  stopSecondStage();
  intakeIn();

  small_turn_constants_12v();
  chassis.turn_timeout = 650;
  intakeInterface.travel_status = IN_BLIND;
  chassis.turn_to_angle(179.5);
  intakeInterface.travel_status = IN_CHECK;
  
  chassis.drive_timeout = 500;
  chassis.drive_settle_time = 45;
  chassis.drive_max_voltage = 7.925; //was 8.125
  chassis.drive_distance(8.895, 180); //was 7.995
  wait(0.445, seconds);

  chassis.drive_max_voltage = 12.625;
  chassis.drive_timeout = 595;
  chassis.drive_settle_time = 0;
  chassis.drive_max_voltage = 5000;
  chassis.drive_distance(-24.45, 177.565); //was 178.95
  highGoal();
  toggleMatchLoad();
  chassis.drive_distance(-15.5, 179.65);
  chassis.drive_max_voltage = 11.25;
  chassis.drive_distance(-4.5, 180);
  wait(2.35,seconds);
  //stopSecondStage();

}