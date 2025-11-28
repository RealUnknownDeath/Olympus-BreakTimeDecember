#include "autons.h"
#include "globals.h"
#include "vex.h"

void slamFromGoal(){
  default_constants();
  chassis.swing_settle_time = 50;
  chassis.swing_timeout = 500;
  chassis.drive_settle_time = 75;
  chassis.drive_timeout = 2000;
  intakeMotor.setVelocity(100, percent);
  middleTravel.setVelocity(85, percent);
  scoreMotor.setVelocity(100, percent);

  intakeInterface.startIntakeThread();


  chassis.drive_settle_time = 250;
  chassis.drive_max_voltage = 15;
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
}