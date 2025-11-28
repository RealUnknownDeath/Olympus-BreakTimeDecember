#include "autons.h"
#include "globals.h"
#include "vex.h"

void shame(){
  default_constants();
  chassis.swing_settle_time = 50;
  chassis.swing_timeout = 500;
  chassis.drive_settle_time = 75;
  chassis.drive_timeout = 2000;
  intakeMotor.setVelocity(100, percent);
  middleTravel.setVelocity(85, percent);
  scoreMotor.setVelocity(100, percent);

  intakeInterface.startIntakeThread();


  chassis.drive_settle_time = 500;
  chassis.drive_max_voltage = 2.5;
  chassis.drive_timeout = 1000;

  chassis.drive_distance(2.85);
  chassis.DriveL.stop();
  chassis.DriveR.stop();
}