#include "autons.h"
#include "globals.h"
#include "vex.h"

void redQualsHook(){qualsHookRoute(true);}// gavin run da red side
void blueQualsHook(){qualsHookRoute(false);}

void qualsHookRoute(bool allienceColor){
  //sorter.startColorSort();
  if(allienceColor){
    finalSortStatus = RED;
    defaultSortStatus = RED;
  }
  else{
    finalSortStatus = BLUE;
    defaultSortStatus = BLUE;
  }
  
  default_constants();// gavin da default constants for stuff
  chassis.swing_settle_time = 50;
  chassis.swing_timeout = 500;
  chassis.drive_settle_time = 75;
  chassis.drive_timeout = 2000;
  intakeMotor.setVelocity(100, percent);
  middleTravel.setVelocity(85, percent);
  scoreMotor.setVelocity(100, percent);

  intakeInterface.startIntakeThread();

  intakeIn();
  intakeInterface.travel_status = IN_CHECK;//gav only go a certain for intake


  chassis.left_swing_to_angle(0-25.5);//turn
  chassis.swing_timeout = 1000;//turn time

  chassis.set_coordinates(60, 22.5, chassis.get_absolute_heading());//gav what degree ur turned at or smthn

  chassis.drive_max_voltage = 12;
  chassis.drive_distance(17.25,340);//gav bot go vroom

  chassis.drive_max_voltage = 1.875;//gav motor go brrrrrrrrrrr
  chassis.drive_distance(5.325,340);//gav wheels now go brrrrrrrrrr
  //chassis.drive_distance(7.965, 331.5); //old
  chassis.drive_settle_time = 150;
  chassis.drive_distance(7.265, 331.5); //new
  chassis.drive_max_voltage = 12;// gav no clue why three moves are back to back when theres no turn


  //chassis.set_turn_constants(12, 0.57125, 0.005575, 5.6175,4); //old
  //chassis.set_turn_exit_conditions(1, 151.5, 975); //old
  chassis.set_turn_constants(12, 0.57125, 0.007585, 5.93275,4); //new// gav pid turn and fancy stuff
  chassis.set_turn_exit_conditions(1, 85.5, 975); //new

  chassis.turn_to_angle(222.5); //was 225

  scoreMotor.setVelocity(42.5, percent);
  intakeInterface.middleGoalDelay(180.5, msec); //was 158.5// gav first long goal score *u check to see if ran 5 sec

  chassis.drive_settle_time = 75;//*throws hands in air bc u ran 5 sec *ricky starts yelling
  chassis.drive_distance(-15.085); //was -15.05
  chassis.DriveL.setVelocity(0, percent);//aaaaand the bot stopped im cackling on the field
  //gav yes these comments tell me where i am and they helpful
  //gav that means stop deleting them Santi
  chassis.DriveR.setVelocity(0, percent);
  
  //chassis.DriveL.setStopping(hold);
  //chassis.DriveR.setStopping(hold);
  wait(0.435, seconds); //was 0.455
  //chassis.DriveL.setStopping(coast);
  //chassis.DriveR.setStopping(coast);

  chassis.drive_max_voltage = 9.5;//gav I get told to set the field back up "FIELD MONKEY, RESET"
  toggleMatchLoad();
  stopSecondStage();//gav 2nd matchloader scored I think
  scoreMotor.stop();
  wait(0.0945, seconds);
  //scoreMotor.spin(reverse);
  chassis.drive_timeout = 1500;
  chassis.drive_settle_time = 415;
  chassis.drive_settle_error = 1.05;
  chassis.drive_distance(49.486795); //was 49.286795
  //chassis.drive_to_point(26.075, 28);
  stopSecondStage();
  scoreMotor.setVelocity(100, percent);

  //chassis.set_turn_constants(12, 0.62625, 0.0068625, 5.5025,4); //d was 5.3625, p was 0.0060625 //old
  //chassis.set_turn_exit_conditions(1, 126.5, 1075); //old
  chassis.set_turn_constants(12, 0.66625, 0.0073625, 6.6225,4);
  chassis.set_turn_exit_conditions(1, 68.5, 1075);
  chassis.turn_to_angle(180);

  intakeIn();
  intakeInterface.travel_status = IN_CHECK;//pickin up the 4 balls

  chassis.drive_max_voltage = 6.65; //was 7.5
  chassis.drive_settle_error = 1;
  chassis.drive_timeout = 400;
  chassis.drive_settle_time = 0;
  chassis.drive_stop(hold);

  //NEW:
  /*
  chassis.drive_distance(8.125);
  chassis.drive_max_voltage = 2.25;
  chassis.drive_distance(7.25);//gav brrrrrrrrrrrrrrrrrrrrrrrrrrrrrr
  //wait(0.3125, seconds);
  int count = 0;
  if(allienceColor){
    while(!(MatchloadSensor.hue() < 180 && MatchloadSensor.hue() < 250) && count < (296.5/2)){ //was 306.5
      count++;
      task::sleep(2);
    }
  }
  else{
    while(!(MatchloadSensor.hue() < 11) && count < (296.5/2)){ //was 306.5
      count++;
      task::sleep(2);//gav bunch of fancy stuff about color checking n the like to score on mid goal?
    }
  }
  */
  //NEWER:
  chassis.ml_hold_time_ms = 55;
  chassis.right_front_sensor_drive_distance(3.125, 18.1, 180);
  toggleMatchLoad();
  toggleAligner();
  //OLD:
  /*
  chassis.drive_max_voltage = 4.125; //was 5.225
  //chassis.drive_distance(8);//was 11.475 and then 12.775
  //wait(0.5, seconds);

  //chassis.drive_max_voltage = 3.5;
  //chassis.drive_distance(4.645); //was 5.145
  //chassis.drive_distance(13.085); //was 12.775
  chassis.drive_distance(12.115); //was 13.115 and 13.228
  wait(0.085,seconds);
  double pushVel = 6.05; //was 12.65
  LeftMotor1.setVelocity(pushVel, percent);
  LeftMotor2.setVelocity(pushVel, percent);
  LeftMotor3.setVelocity(pushVel, percent);
  RightMotor1.setVelocity(pushVel, percent);
  RightMotor2.setVelocity(pushVel, percent);
  RightMotor3.setVelocity(pushVel, percent);
  LeftMotor1.spin(forward);
  LeftMotor2.spin(forward);
  LeftMotor3.spin(forward);
  RightMotor1.spin(forward);
  RightMotor2.spin(forward);
  RightMotor3.spin(forward);
  int count = 0;
  wait(451*2, msec);
  */
  
  
  chassis.DriveL.setVelocity(0, percent);
  chassis.DriveR.setVelocity(0, percent);
  chassis.DriveL.stop();
  chassis.DriveR.stop();
  chassis.drive_stop(coast);

  wait(0.1,seconds);


  //intakeInterface.intake_status = OFF_intake;
  
  chassis.drive_settle_error = 0.5;
  chassis.drive_timeout = 1750;
  chassis.drive_settle_time = 0; //was 350
  chassis.drive_max_voltage = 12; //was 12.5
  chassis.ml_hold_time_ms = 0;
  chassis.right_front_sensor_drive_distance(32.0, 18.1, 180);
  highGoal();
  int count = 0;
  if(allienceColor){ //red
    while(count < 1914/2){
      count++;
      task::sleep(2);
      if((180 < Optical21.hue() && Optical21.hue() < 250)){
        wait(0.175,seconds);
        middleTravel.stop();
        intakeInterface.travel_status = OFF_travel;
        wait(0.075,seconds);
        break;
      }
    }
  }
  else{ //blue
    while(count < 1914/2){
      count++;
      task::sleep(2);
      if((Optical21.hue() < 11)){
        wait(0.175,seconds);
        middleTravel.stop();
        intakeInterface.travel_status = OFF_travel;
        wait(0.075,seconds);
        break;//gav fancy schmancy scoring on the second long goal
      }
    }
  
  }
    
  //wait(1.915,seconds);


  //intakeInterface.travel_status = OFF_travel;
  //wait(0.15,seconds);

  //chassis.set_coordinates(24, (48-8.5), chassis.get_absolute_heading());

  stopSecondStage();
  
  //leftHookPushFromGoal();
  leftHookPushFromGoal(1.5);
  //slamFromGoal();


}