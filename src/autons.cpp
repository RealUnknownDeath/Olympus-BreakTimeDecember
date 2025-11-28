#include "vex.h"
#include "globals.h"


void testRickyCool() {
  default_constants();
  chassis.Gyro.calibrate();
  while (chassis.Gyro.isCalibrating()) wait(20, msec);

  chassis.drive_forward_spin180(
    /*distance*/   10.0f,
    /*clockwise*/  true,
    /*fwd_V*/      7.0f,   // up to 8.0 if your drive can handle it
    /*turn_V_max*/ 10.5f,  // give the spin more bite
    /*kP_head*/    0.14f,
    /*kD_head*/    0.10f,
    /*ease*/       0.60f,  // more front-load than 0.75 (spins earlier)
    /*settle_deg*/ 4,
    /*settle_ms*/  150,
    /*timeout_ms*/ 6000
  );

  // small nudge if needed
  chassis.drive_distance(2.0f, chassis.get_absolute_heading());
}


void dragon(){
  intakeMotor.setVelocity(100, percent);
  middleTravel.setVelocity(100, percent);
  scoreMotor.setVelocity(100, percent);
  big_drive_constants_reverse();
  chassis.drive_settle_time = 150;
  chassis.drive_distance(-46.5,0);
  highGoal();
  /*
  lowerTotal();
  toggleDescore();
  wait(1.5,seconds);
  
  raise();
  toggleDescore();
  wait(1.05,seconds);
  chassis.turn_to_angle(180);
  toggleMatchLoad();
  */
  /*
  raise();
  //wait(0.185,seconds);
  toggleDescore();
  //wait(0.495,seconds);
  toggleMatchLoad();
  */
}


void tuneTurnPID(){
  /*
  default_constants();

  chassis.turn_to_angle(300);
  chassis.set_turn_constants(10, 0.45, 0.0030, 4,5);
  chassis.set_turn_exit_conditions(1, 500, 1250);
  wait(1,seconds);
  chassis.turn_to_angle(180);
  */
  default_constants();
  small_drive_constants();
  chassis.drive_max_voltage = 4;
  chassis.drive_distance(10,0);

  big_turn_fast_speed_constants_15v();
  chassis.turn_to_angle(180);

  chassis.turn_to_angle(0);

  small_turn_constants_12v();
  chassis.turn_to_angle(90);

  big_turn_fast_speed_constants_15v();
  chassis.turn_to_angle(270);

  small_turn_constants_12v();
  chassis.turn_to_angle(180);

  big_turn_fast_speed_constants_15v();
  chassis.turn_to_angle(0);
  
  chassis.drive_distance(-10,0);
  /*
  chassis.turn_to_angle(180);
  chassis.turn_to_angle(0);
  chassis.turn_to_angle(90);
  chassis.turn_to_angle(270);
  chassis.turn_to_angle(180);
  chassis.turn_to_angle(0);
  */

  while(true){
    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(3,3);
    Brain.Screen.print(chassis.get_absolute_heading());
    task::sleep(10);
  }
}

void tuneDrivePID(){
  default_constants();
  chassis.drive_max_voltage = 4;
  chassis.drive_distance(22,0);

  small_turn_constants_12v();
  chassis.turn_to_angle(270);

  small_drive_constants();
  chassis.drive_distance(24,270);

  chassis.turn_to_angle(0);

  small_drive_constants();
  
  chassis.drive_distance(-24,0);

  big_drive_constants_forwards();
  chassis.drive_distance(96,0);

  big_drive_constants_reverse();
  chassis.drive_distance(-72,0);

  medium_drive_constants_forwards();
  chassis.drive_distance(48,0);

  big_drive_constants_reverse();
  chassis.drive_distance(-72,0);

  small_drive_constants();
  chassis.drive_distance(24,0);
  
  chassis.turn_to_angle(90);

  small_drive_constants();
  chassis.drive_distance(24,90);

  chassis.turn_to_angle(0);

  small_drive_constants();
  chassis.drive_max_voltage = 4;
  chassis.drive_distance(-22,0);


  while(true){
    Brain.Screen.clearScreen();
    Brain.Screen.print(chassis.get_X_position());
    Brain.Screen.print(chassis.get_Y_position());
    task::sleep(10);
  }
}

void testHook(){
  intakeMotor.setVelocity(100, percent);
  middleTravel.setVelocity(85, percent);
  scoreMotor.setVelocity(100, percent);
  chassis.set_coordinates(108, 60, 270);
  raise();
  chassis.swing_timeout = 1500;
  chassis.swing_max_voltage = 12;
  chassis.left_swing_to_angle(170);
  chassis.swing_timeout = 225;
  chassis.right_swing_to_angle(180);
  chassis.drive_timeout = 50;
  chassis.drive_distance(-2, 180);
  highGoal();
}

void descoreMacro(){
  
}

void rightBlueElims(){ //WORKED ON
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
  toggleMatchLoad();
  //wait(0.075,seconds); //let the matchload hit the other color balls
  chassis.drive_max_voltage = 7.5; //was 7.5
  //chassis.drive_distance(-6.25, 45); //7.25 and before that 10.5
  chassis.drive_settle_time = 175; //was 175 and also 125
  //raise();
  //chassis.drive_to_point(113.75,54.6425);
  chassis.drive_to_point(113.35,53.1925);
  chassis.drive_settle_time = 75;
  chassis.drive_max_voltage = 12; //was 10
  //toggleDescore();

  chassis.swing_settle_time = 515; //was 685 then 585
  chassis.left_swing_to_angle(267.85); //was 265.85

  intakeIn();
  intakeInterface.travel_status = IN_CHECK;

  //wait(2,seconds);
  chassis.swing_timeout = 1547; //was 1587
  chassis.swing_max_voltage = 15;

  chassis.left_swing_to_angle(162.8685); //was 170
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
  //toggleMatchLoad();
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

  chassis.drive_distance(12.45,180); //was 181.65 and 9.95
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
  wait(0.5585, seconds); // was 0.2285 seconds
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
  chassis.drive_distance(-24.75,180);

  chassis.drive_max_voltage = 12;
  highGoal();
  toggleMatchLoad();
  chassis.drive_timeout = 250;
  chassis.drive_distance(-8.85,179.25);
  chassis.drive_timeout = 75;
  chassis.drive_distance(-3.85);
  //wait(1.85,seconds);
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


}

void rightRedElims(){ //WORKED ON
  //sorter.startColorSort();
  finalSortStatus = RED;
  defaultSortStatus = RED;
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
  toggleMatchLoad();
  //wait(0.075,seconds); //let the matchload hit the other color balls
  chassis.drive_max_voltage = 7.5; //was 7.5
  //chassis.drive_distance(-6.25, 45); //7.25 and before that 10.5
  chassis.drive_settle_time = 175; //was 175 and also 125
  //raise();
  //chassis.drive_to_point(113.15,53.1925);
  chassis.drive_to_point(113.205,52.98525);
  chassis.drive_settle_time = 75;
  chassis.drive_max_voltage = 12; //was 10
  //toggleDescore();

  chassis.swing_settle_time = 515; //was 685 then 585
  chassis.left_swing_to_angle(267.85); //was 265.85
  //wait(2,seconds);
  chassis.swing_timeout = 1587; //was 1547
  chassis.swing_max_voltage = 5000; //was 15

  chassis.left_swing_to_angle(162.8685); //was 170
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
  //toggleMatchLoad();
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
  chassis.drive_max_voltage = 5.5; //was 6.5 and 8.5
  //scoreMotor.spin(reverse);
  //middleTravel.spin(reverse);
  chassis.drive_timeout = 575; //was 2000
  chassis.drive_settle_time = 250;

  chassis.drive_distance(12.45,180); //was 181.65 and 9.95
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
  wait(0.5585, seconds); // was 0.2285 seconds
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
  chassis.drive_distance(-24.75,180);

  chassis.drive_max_voltage = 12;
  highGoal();
  toggleMatchLoad();
  chassis.drive_timeout = 250;
  chassis.drive_distance(-8.85,179.25);
  chassis.drive_timeout = 75;
  chassis.drive_distance(-3.85);
  //wait(1.85,seconds);
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


}

void leftRedElims(){ //WORKED ON
  //sorter.startColorSort();
  finalSortStatus = RED;
  defaultSortStatus = RED;
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


  chassis.left_swing_to_angle(-25.5); //was 25.5
  chassis.swing_timeout = 1000;

  chassis.set_coordinates(60, 22.5, chassis.get_absolute_heading());
  toggleDescore();

  chassis.drive_max_voltage = 12; //was 10
  chassis.drive_distance(16.65,-20);
  toggleAligner();
  chassis.drive_max_voltage = 1.875; //was 2.25
  chassis.drive_distance(5.325,-20);
  chassis.drive_distance(5.7, -28.5); //was 26.5
  chassis.drive_max_voltage = 12;
  //chassis.drive_distance(8.125);
  //chassis.swing_max_voltage = 12;
  //chassis.left_swing_to_angle(359.5); //was 0
  chassis.left_swing_to_angle(-52.565); //was -49.565
  //chassis.right_swing_to_angle(52.5);
  chassis.drive_max_voltage = 12; //was 10
  chassis.drive_distance(22.55);
  chassis.drive_max_voltage = 2.575; //was 2.875
  chassis.drive_distance(9.256); //was 9.456 but was sometimes hitting the bar
  //toggleMatchLoad();
  //wait(0.075,seconds); //let the matchload hit the other color balls
  chassis.drive_max_voltage = 7.5; //was 7.5
  //chassis.drive_distance(-6.25, 45); //7.25 and before that 10.5
  chassis.swing_timeout = 1550;
  chassis.drive_settle_time = 175; //was 175 and also 125
  //raise();
  //chassis.drive_to_point(113.75,54.6425);
  toggleDescore();
  chassis.drive_to_point(24+(120-112.925),51.7225);
  chassis.drive_settle_time = 75;
  chassis.drive_max_voltage = 12; //was 10

  chassis.swing_settle_time = 515; //was 685 then 585
  chassis.right_swing_to_angle(-267.85); //was 265.85
  //wait(2,seconds);
  chassis.swing_timeout = 1547; //was 1587
  chassis.swing_max_voltage = 15;

  chassis.right_swing_to_angle(-162.8685); //was -162.8685
  chassis.swing_timeout = 265; //was 425
  chassis.drive_timeout = 25;
  highGoal();
  chassis.drive_timeout = 500;
  chassis.drive_distance(-2, 180);
  wait(0.50,seconds);
  intakeInterface.travel_status = OFF_travel;
  wait(0.05,seconds);
  chassis.set_coordinates(23, (48-8.5), chassis.get_absolute_heading());
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
  chassis.drive_max_voltage = 9.5; //was 6.5
  //scoreMotor.spin(reverse);
  //middleTravel.spin(reverse);
  chassis.drive_timeout = 575; //was 2000
  chassis.drive_settle_time = 75;

  chassis.drive_distance(12.45,180); //was 181.65 and 9.95
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
  wait(0.5585, seconds); // was 0.2285 seconds
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
  chassis.drive_distance(-24.75,180);
  toggleMatchLoad();

  chassis.drive_max_voltage = 12;
  highGoal();
  toggleMatchLoad();
  chassis.drive_timeout = 250;
  chassis.drive_distance(-8.85,-179.25);
  chassis.drive_timeout = 75;
  chassis.drive_distance(-3.85);
  //wait(1.85,seconds);
  wait(0.95, seconds);

  chassis.drive_timeout = 350;
  toggleDescore();
  scoreMotor.stop();
  intakeInterface.travel_status = IN_CHECK;
  chassis.drive_distance(8.5);
  chassis.drive_max_voltage = 15;
  chassis.drive_distance(-17.5);
  scoreMotor.spin(forward);
  intakeInterface.travel_status = IN_BLIND;
  toggleDescore();


}

void leftBlueElims(){ //WORKED ON
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


  chassis.left_swing_to_angle(-25.5); //was 25.5
  chassis.swing_timeout = 1000;

  chassis.set_coordinates(60, 22.5, chassis.get_absolute_heading());
  toggleDescore();

  chassis.drive_max_voltage = 12; //was 10
  chassis.drive_distance(16.65,-20);
  toggleAligner();
  chassis.drive_max_voltage = 1.875; //was 2.25
  chassis.drive_distance(5.325,-20);
  chassis.drive_distance(5.7, -28.5); //was 26.5
  chassis.drive_max_voltage = 12;
  //chassis.drive_distance(8.125);
  //chassis.swing_max_voltage = 12;
  //chassis.left_swing_to_angle(359.5); //was 0
  chassis.left_swing_to_angle(-52.565); //was -49.565
  //chassis.right_swing_to_angle(52.5);
  chassis.drive_max_voltage = 12; //was 10
  chassis.drive_distance(22.55);
  chassis.drive_max_voltage = 2.575; //was 2.875
  chassis.drive_distance(9.256); //was 9.456 but was sometimes hitting the bar
  //toggleMatchLoad();
  //wait(0.075,seconds); //let the matchload hit the other color balls
  chassis.drive_max_voltage = 7.5; //was 7.5
  //chassis.drive_distance(-6.25, 45); //7.25 and before that 10.5
  chassis.swing_timeout = 1550;
  chassis.drive_settle_time = 175; //was 175 and also 125
  //raise();
  //chassis.drive_to_point(113.75,54.6425);
  toggleDescore();
  chassis.drive_to_point(24+(120-112.925),51.7225);
  chassis.drive_settle_time = 75;
  chassis.drive_max_voltage = 12; //was 10

  chassis.swing_settle_time = 515; //was 685 then 585
  chassis.right_swing_to_angle(-267.85); //was 265.85
  //wait(2,seconds);
  chassis.swing_timeout = 1547; //was 1587
  chassis.swing_max_voltage = 15;

  chassis.right_swing_to_angle(-162.8685); //was -162.8685
  chassis.swing_timeout = 265; //was 425
  chassis.drive_timeout = 25;
  highGoal();
  chassis.drive_timeout = 500;
  chassis.drive_distance(-2, 180);
  wait(0.50,seconds);
  intakeInterface.travel_status = OFF_travel;
  wait(0.05,seconds);
  chassis.set_coordinates(23, (48-8.5), chassis.get_absolute_heading());
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
  chassis.drive_max_voltage = 9.5; //was 6.5
  //scoreMotor.spin(reverse);
  //middleTravel.spin(reverse);
  chassis.drive_timeout = 575; //was 2000
  chassis.drive_settle_time = 75;

  chassis.drive_distance(12.45,180); //was 181.65 and 9.95
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
  wait(0.5585, seconds); // was 0.2285 seconds
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
  chassis.drive_distance(-24.75,180);
  toggleMatchLoad();

  chassis.drive_max_voltage = 12;
  highGoal();
  toggleMatchLoad();
  chassis.drive_timeout = 250;
  chassis.drive_distance(-8.85,-179.25);
  chassis.drive_timeout = 75;
  chassis.drive_distance(-3.85);
  //wait(1.85,seconds);
  wait(0.95, seconds);

  chassis.drive_timeout = 350;
  toggleDescore();
  scoreMotor.stop();
  intakeInterface.travel_status = IN_CHECK;
  chassis.drive_distance(8.5);
  chassis.drive_max_voltage = 15;
  chassis.drive_distance(-17.5);
  scoreMotor.spin(forward);
  intakeInterface.travel_status = IN_BLIND;
  toggleDescore();


}

void driveleftBlueElims(){ //WORKED ON
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


  chassis.left_swing_to_angle(-25.5); //was 25.5
  chassis.swing_timeout = 1000;

  chassis.set_coordinates(60, 22.5, chassis.get_absolute_heading());
  toggleDescore();

  chassis.drive_max_voltage = 12; //was 10
  chassis.drive_distance(16.65,-20);
  toggleAligner();
  chassis.drive_max_voltage = 1.875; //was 2.25
  chassis.drive_distance(5.325,-20);
  chassis.drive_distance(5.7, -28.5); //was 26.5
  chassis.drive_max_voltage = 12;
  //chassis.drive_distance(8.125);
  //chassis.swing_max_voltage = 12;
  //chassis.left_swing_to_angle(359.5); //was 0
  chassis.left_swing_to_angle(-52.565); //was -49.565
  //chassis.right_swing_to_angle(52.5);
  chassis.drive_max_voltage = 12; //was 10
  chassis.drive_distance(22.55);
  chassis.drive_max_voltage = 2.575; //was 2.875
  chassis.drive_distance(9.256); //was 9.456 but was sometimes hitting the bar
  //toggleMatchLoad();
  //wait(0.075,seconds); //let the matchload hit the other color balls
  chassis.drive_max_voltage = 7.5; //was 7.5
  //chassis.drive_distance(-6.25, 45); //7.25 and before that 10.5
  chassis.swing_timeout = 1550;
  chassis.drive_settle_time = 175; //was 175 and also 125
  //raise();
  //chassis.drive_to_point(113.75,54.6425);
  toggleDescore();
  //chassis.drive_to_point(24+(120-112.925),51.7225);
  chassis.drive_to_point(24+(120-112.925),48.7225);
  chassis.drive_settle_time = 75;
  chassis.drive_max_voltage = 12; //was 10

  chassis.swing_timeout = 1547;
  chassis.swing_settle_time = 250;
  chassis.right_swing_to_angle(180); //was 265.85
  //wait(2,seconds);
  highGoal();
  chassis.drive_timeout = 500;
  chassis.drive_distance(-2, 180);
  wait(0.50,seconds);
  intakeInterface.travel_status = OFF_travel;
  wait(0.05,seconds);
  chassis.set_coordinates(23, (48-8.5), chassis.get_absolute_heading());
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
  chassis.drive_max_voltage = 9.5; //was 6.5
  //scoreMotor.spin(reverse);
  //middleTravel.spin(reverse);
  chassis.drive_timeout = 575; //was 2000
  chassis.drive_settle_time = 75;

  chassis.drive_distance(12.45,180); //was 181.65 and 9.95
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
  wait(0.5585, seconds); // was 0.2285 seconds
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
  chassis.drive_distance(-24.75,180);
  toggleMatchLoad();

  chassis.drive_max_voltage = 12;
  highGoal();
  toggleMatchLoad();
  chassis.drive_timeout = 250;
  chassis.drive_distance(-8.85,-179.25);
  chassis.drive_timeout = 75;
  chassis.drive_distance(-3.85);
  //wait(1.85,seconds);
  wait(0.95, seconds);

  chassis.drive_timeout = 350;
  toggleDescore();
  scoreMotor.stop();
  intakeInterface.travel_status = IN_CHECK;
  chassis.drive_distance(8.5);
  chassis.drive_max_voltage = 15;
  chassis.drive_distance(-17.5);
  scoreMotor.spin(forward);
  intakeInterface.travel_status = IN_BLIND;
  toggleDescore();


}


void redRightSafeQuals(){ //WORKED ON
  sorter.startColorSort();
  finalSortStatus = RED;
  defaultSortStatus = NONE;
  default_constants();
  chassis.swing_settle_time = 50;
  chassis.swing_timeout = 500;
  chassis.drive_settle_time = 75;
  chassis.drive_timeout = 2000;
  intakeMotor.setVelocity(100, percent);
  middleTravel.setVelocity(85, percent);
  scoreMotor.setVelocity(100, percent);

  intakeMotor.spin(forward);


  chassis.right_swing_to_angle(25);
  chassis.swing_timeout = 1000;

  chassis.set_coordinates(84, 24.5, chassis.get_absolute_heading());

  
  chassis.drive_max_voltage = 10;
  chassis.drive_distance(17.55);
  chassis.drive_max_voltage = 2;
  chassis.drive_distance(10.625);
  chassis.drive_max_voltage = 10;
  chassis.turn_to_angle(315);
  chassis.drive_max_voltage = 10;
  chassis.drive_distance(9.5);
  chassis.drive_max_voltage = 5;
  intakeMotor.spin(reverse);
  chassis.drive_distance(1.35);
  wait(0.50, seconds);
  intakeMotor.stop();
  toggleMatchLoad();


  chassis.drive_max_voltage = 8;
  chassis.drive_timeout = 2500;
  chassis.drive_settle_time = 250;

  //chassis.drive_to_point(120, 32);
  chassis.drive_to_point(118.5, 32);
  intakeMotor.spin(forward);

  chassis.drive_timeout = 200;
  chassis.drive_settle_time = 75;

  chassis.turn_to_angle(180);

  chassis.drive_max_voltage = 8;
  chassis.drive_distance(5.5);
  chassis.drive_max_voltage = 3.25;
  chassis.drive_distance(11.225);
  wait(0.15, seconds);
  chassis.drive_distance(1.465);
  chassis.drive_max_voltage = 7;
  chassis.drive_distance(2);
  wait(0.525, seconds);
  //chassis.drive_distance(-18,180);
  chassis.drive_to_point(120, 38);
  chassis.drive_max_voltage = 4;
  chassis.drive_timeout = 1500;
  chassis.drive_distance(-28, 180);
  highGoal();

}


void soloAWP(){
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
void ball13(){
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
  chassis.drive_timeout = 1650;
  chassis.drive_settle_time = 215;
  chassis.drive_max_voltage = 12;
  chassis.drive_distance(-32.125, 270); //was -30.25
  intakeInterface.startIntakeThread();

  intakeIn();
  intakeInterface.travel_status = IN_CHECK;

  //chassis.set_turn_constants(12, 0.56525, 0.005475, 4.625,4); old
  chassis.set_turn_constants(12, 0.57825, 0.006225, 5.905,4); //new
  chassis.set_turn_exit_conditions(1, 136.5, 975);
  //small_turn_constants_12v();
  
  chassis.turn_to_angle(180);


  chassis.drive_timeout = 645.5; //was 900.5
  chassis.drive_settle_time = 350;

  //chassis.drive_distance(7.225, 180);
  //wait(0.275, seconds);
  chassis.drive_distance(5.215, 180); //was 5.075
  chassis.DriveL.setVelocity(0.25, percent);
  chassis.DriveL.spin(forward);
  chassis.DriveR.setVelocity(0.25, percent);
  chassis.DriveR.spin(forward);
  int count = 0;
  while(!(40 < MatchloadSensor.hue()) && count < 518){
    count++;
    task::sleep(2);
  }

  chassis.DriveL.setVelocity(0, percent);
  chassis.DriveR.setVelocity(0, percent);
  chassis.DriveL.stop();
  chassis.DriveR.stop();
  //intakeInterface.intake_status = OFF_intake;

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

  wait(0.915,seconds);
  intakeInterface.travel_status = OFF_travel;
  wait(0.15,seconds);

  chassis.set_coordinates(119, (48-8.5), chassis.get_absolute_heading());

  stopSecondStage();
  chassis.drive_timeout = 1750;
  chassis.drive_max_voltage = 10.5; //was 10.5
  chassis.drive_distance(10,180);


  chassis.set_turn_constants(12, 0.57625, 0.0055625, 6.075,4);
  chassis.set_turn_exit_conditions(1, 76.5, 1075);
  //scoreMotor.spin(forward);
  chassis.turn_to_angle(308);
  //scoreMotor.stop();

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
  chassis.turn_to_angle(237.5); //was 225, then 215
  
  //CUT WAS HERE

  scoreMotor.setVelocity(100, percent);
  scoreMotor.stop();
  intakeMotor.stop();
  middleTravel.stop();
  intakeInterface.travel_status = OFF_travel;
  intakeInterface.intake_status = OFF_intake;


  chassis.drive_timeout = 1950;
  chassis.drive_settle_time = 255;
  chassis.drive_max_voltage = 10;
  //toggleMatchLoad();
  chassis.drive_distance(26.285, 237.5);
  //chassis.drive_to_point(24.615, 25.835);
  /*
  stopSecondStage();
  intakeIn();

  small_turn_constants_12v();
  chassis.turn_timeout = 650;
  intakeInterface.travel_status = IN_BLIND;

  chassis.set_turn_constants(12, 0.56625, 0.0055625, 5.975,4);
  chassis.set_turn_exit_conditions(1, 98.5, 1075);
  chassis.turn_to_angle(179.5);
  intakeInterface.travel_status = IN_CHECK;
  
  chassis.drive_timeout = 500;
  chassis.drive_settle_time = 45;

  chassis.drive_max_voltage = 7.925; //was 8.125

  //chassis.drive_distance(8.895, 180); //was 7.995
  //wait(0.445, seconds);


  chassis.drive_distance(8.845);//was 7.7225
  //wait(0.5, seconds);
  chassis.DriveL.setVelocity(7.5, percent);
  chassis.DriveL.spin(forward);
  chassis.DriveR.setVelocity(7.5, percent);
  chassis.DriveR.spin(forward);
  count = 0;
  while(!(40 < MatchloadSensor.hue()) && count < 516){
    count++;
    task::sleep(2);
  }

  chassis.DriveL.setVelocity(0, percent);
  chassis.DriveR.setVelocity(0, percent);
  chassis.DriveL.stop();
  chassis.DriveR.stop();
  intakeInterface.intake_status = OFF_intake;
  chassis.drive_max_voltage = 12.625;
  chassis.drive_timeout = 595;
  chassis.drive_settle_time = 0;
  chassis.drive_max_voltage = 5000;
  */
  //chassis.drive_distance(-24.45, 177.565); //was 178.95

  chassis.set_turn_constants(12, 0.57625, 0.0067625, 5.525,4);
  chassis.set_turn_exit_conditions(1.25, 54.5, 1075);
  chassis.turn_to_angle(179.25);
  intakeInterface.travel_status = IN_CHECK;
  intakeInterface.highGoalDelay(150, msec);
  chassis.drive_distance(-15.95, 179.75); //was 178.95
  highGoal();
  //toggleMatchLoad();
  chassis.drive_distance(-15.5, 180);
  chassis.drive_max_voltage = 11.25;
  chassis.drive_distance(-4.5, 180);
  wait(2.35,seconds);
  //stopSecondStage();

}
//PRE PLANE VERSION:
void preplanesafeProgSkills(){
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

  chassis.set_coordinates(96-7.25, 18.5, 270);
  toggleMatchLoad();
  toggleAligner();
  big_drive_constants_forwards();
  chassis.drive_timeout = 1500;
  chassis.drive_settle_time = 350;
  chassis.drive_max_voltage = 12;
  chassis.drive_distance(-30.25, 270);
  small_turn_constants_12v();
  intakeIn();
  intakeInterface.travel_status = IN_CHECK;
  chassis.turn_to_angle(180);
  chassis.drive_timeout = 900.5;
  chassis.drive_distance(6.125, 180);
  wait(0.515,seconds); //was 1.5 seconds
  chassis.drive_max_voltage = 3.25;
  chassis.drive_distance(1.656, 180);
  wait(0.55, seconds);
  //chassis.drive_timeout = 560.5; //was 662.5
  //chassis.drive_settle_time = 350;
  //chassis.drive_max_voltage = 3.25; //was 2.25
  //wait(0.025, seconds);
  chassis.drive_timeout = 1750;
  chassis.drive_settle_time = 0; //was 350
  chassis.drive_max_voltage = 11.5;
  chassis.drive_distance(-23.5, 181.5);
  chassis.drive_settle_time = 150;
  highGoal();
  chassis.drive_timeout = 250;
  chassis.drive_max_voltage = 9.5;
  chassis.drive_distance(-8, 180);
  toggleMatchLoad();

  wait(1.275,seconds); //was 1.85 seconds
  stopSecondStage();
  wait(0.315,seconds);
  highGoal();
  wait(1.25,seconds);
  
  stopSecondStage();
  chassis.drive_timeout = 1750;
  chassis.drive_max_voltage = 10.5; //was 10.5
  chassis.drive_distance(10,180);
  big_turn_fast_speed_constants_12v();


  chassis.turn_to_angle(-308.85);
  chassis.drive_max_voltage = 9.5;
  chassis.drive_distance(15.125,-329.75);
  chassis.turn_to_angle(10);
  chassis.drive_distance(60.85,359);
  chassis.drive_max_voltage = 8.125;
  
  chassis.drive_distance(6,355.25);

  chassis.turn_to_angle(307);

  //chassis.drive_to_point(133.25, 114.5);
  toggleMatchLoad();
  chassis.drive_max_voltage = 9.5;
  chassis.drive_distance(16.475); //was 17.25
  //chassis.drive_to_point(116.65, 115.5);
  chassis.turn_to_angle(359.5);
  chassis.drive_max_voltage = 9.5;
  intakeIn();
  intakeInterface.travel_status = IN_CHECK;
  chassis.drive_distance(11.487,0);
  chassis.drive_max_voltage = 3.5;
  chassis.drive_distance(3.767,0);
  wait(1, seconds);
  chassis.drive_max_voltage = 8;
  chassis.drive_distance(-24, 357.105);
  toggleMatchLoad();
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
  chassis.drive_distance(15.0955,0);

  chassis.turn_to_angle(225);

  intakeIn();
  intakeInterface.travel_status = IN_CHECK;

  chassis.drive_settle_time = 150;
  chassis.drive_max_voltage = 8;
  chassis.drive_distance(18.25, 225);
  chassis.drive_max_voltage = 2.75;
  chassis.drive_distance(18.5, 225);

  chassis.set_turn_constants(12, 0.57125, 0.005575, 5.6175,4);
  chassis.set_turn_exit_conditions(1, 151.5, 975);
  chassis.turn_to_angle(46.15); //was 225

  scoreMotor.setVelocity(48.5, percent);
  intakeInterface.middleGoalDelay(205.5, msec);

  chassis.drive_max_voltage = 6.15;

  chassis.drive_distance(-11.915); //was -15.05
  chassis.DriveL.setVelocity(0, percent);
  chassis.DriveR.setVelocity(0, percent);
  
  //chassis.DriveL.setStopping(hold);
  //chassis.DriveR.setStopping(hold);
  wait(1.085, seconds);
  //chassis.DriveL.setStopping(coast);
  //chassis.DriveR.setStopping(coast);

  chassis.drive_max_voltage = 9.5;
  toggleMatchLoad();
  stopSecondStage();
  scoreMotor.stop();

  chassis.drive_distance(23);

  chassis.turn_to_angle(281.5);

  chassis.swing_settle_time = 550;

  chassis.drive_distance(79.14265); //was 76.265
  
  chassis.turn_to_angle(0);
  
  intakeIn();
  intakeInterface.intake_status = IN;
  intakeInterface.travel_status = IN_CHECK;

  chassis.drive_timeout = 900.5;
  chassis.swing_settle_time = 150;
  chassis.drive_distance(14.925, 0); //was 11.925
  wait(0.75,seconds); //was 1.5 seconds
  chassis.drive_max_voltage = 4.25;
  chassis.drive_distance(1.996, 0);
  wait(0.55, seconds);
  //chassis.drive_timeout = 560.5; //was 662.5
  //chassis.drive_settle_time = 350;
  //chassis.drive_max_voltage = 3.25; //was 2.25
  //wait(0.025, seconds);
  chassis.drive_timeout = 1750;
  chassis.drive_settle_time = 0; //was 350
  chassis.drive_max_voltage = 11.5;
  chassis.drive_distance(-23.5, 356.55);
  chassis.drive_settle_time = 150;
  highGoal();
  chassis.drive_timeout = 250;
  chassis.drive_max_voltage = 9.5;
  chassis.drive_distance(-8, 0.75);
  toggleMatchLoad();

  wait(1.475,seconds); //was 1.85 seconds
  
  stopSecondStage();
  chassis.drive_timeout = 1750;
  chassis.drive_max_voltage = 10.5; //was 10.5
  chassis.drive_distance(10,0);
  

  //GO PARK SAFETY
  chassis.turn_to_angle(315);
  chassis.drive_max_voltage = 10.5;
  chassis.drive_distance(-22.5);
  chassis.turn_to_angle(0);
  chassis.drive_max_voltage = 10.5;
  chassis.drive_distance(-85.55);
  
  chassis.turn_to_angle(90);
  chassis.drive_max_voltage = 15.5;
  chassis.drive_settle_time=0;
  chassis.drive_distance(10.5, 85.5);
  toggleMatchLoad();
  chassis.drive_settle_time=500;
  chassis.drive_max_voltage = 5000.5;
  chassis.drive_distance(35.5, 88.25);
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

//POST PLANE EDITS:
void safeProgSkills(){
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

  chassis.set_coordinates(96-7.25, 18.5, 270);
  toggleMatchLoad();
  toggleAligner();
  big_drive_constants_forwards();
  chassis.drive_timeout = 1500;
  chassis.drive_settle_time = 350;
  chassis.drive_max_voltage = 12;
  chassis.drive_distance(-30.25, 270);
  small_turn_constants_12v();
  intakeIn();
  intakeInterface.travel_status = IN_CHECK;
  chassis.turn_to_angle(180);
  chassis.drive_timeout = 900.5;
  chassis.drive_distance(6.125, 180);
  wait(0.515,seconds); //was 1.5 seconds
  chassis.drive_max_voltage = 3.15;
  chassis.drive_distance(1.616, 180);
  wait(0.55, seconds);
  //chassis.drive_timeout = 560.5; //was 662.5
  //chassis.drive_settle_time = 350;
  //chassis.drive_max_voltage = 3.25; //was 2.25
  //wait(0.025, seconds);
  chassis.drive_timeout = 1750;
  chassis.drive_settle_time = 0; //was 350
  chassis.drive_max_voltage = 11.5;
  chassis.drive_distance(-23.5, 181.5);
  chassis.drive_settle_time = 150;
  highGoal();
  chassis.drive_timeout = 250;
  chassis.drive_max_voltage = 9.5;
  chassis.drive_distance(-8, 180);
  chassis.DriveL.stop();
  chassis.DriveR.stop();
  toggleMatchLoad();

  wait(1.275,seconds); //was 1.85 seconds
  stopSecondStage();
  wait(0.315,seconds);
  highGoal();
  wait(1.25,seconds);
  
  stopSecondStage();
  chassis.drive_timeout = 1750;
  chassis.drive_max_voltage = 10.5; //was 10.5
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
  chassis.drive_distance(16.475); //was 17.25
  //chassis.drive_to_point(116.65, 115.5);
  chassis.turn_to_angle(359.5);
  chassis.drive_max_voltage = 9.5;
  intakeIn();
  intakeInterface.travel_status = IN_CHECK;
  chassis.drive_distance(11.487,0);
  chassis.drive_max_voltage = 3.5;
  chassis.drive_distance(3.767,0);
  wait(1, seconds);
  chassis.drive_distance(-3.767,0);
  chassis.drive_distance(3.767,0);
  wait(0.5, seconds);
  chassis.drive_max_voltage = 8;
  chassis.drive_distance(-24, 357.105);
  toggleMatchLoad();
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

  chassis.drive_distance(-11.915); //was -15.05
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

  chassis.drive_distance(80.214265); //was 80.014265 could be also 76 depending on whether or not it was actually just timing out earlier
  
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
  chassis.drive_distance(12.925, 0); //was 11.925
  wait(0.75,seconds); //was 1.5 seconds
  chassis.drive_max_voltage = 4.25;
  chassis.drive_distance(2.996, 0);
  wait(0.55, seconds);
  //chassis.drive_timeout = 560.5; //was 662.5
  //chassis.drive_settle_time = 350;
  //chassis.drive_max_voltage = 3.25; //was 2.25
  //wait(0.025, seconds);
  chassis.drive_timeout = 1750;
  chassis.drive_settle_time = 0; //was 350
  chassis.drive_max_voltage = 11.5;
  chassis.drive_distance(-23.5, 356.55);
  chassis.drive_settle_time = 150;
  highGoal();
  chassis.drive_timeout = 250;
  chassis.drive_max_voltage = 9.5;
  chassis.drive_distance(-8, 0.75);
  toggleMatchLoad();

  wait(1.475,seconds); //was 1.85 seconds
  
  stopSecondStage();
  chassis.drive_timeout = 1750;
  chassis.drive_max_voltage = 10.5; //was 10.5
  chassis.drive_distance(10,0);
  

  //GO PARK SAFETY

  chassis.set_turn_constants(12, 0.57125, 0.005575, 5.6175,4);
  chassis.set_turn_exit_conditions(1, 151.5, 975);
  chassis.turn_to_angle(315);

  chassis.drive_max_voltage = 10.5;
  chassis.drive_distance(-22.5);

  chassis.set_turn_constants(12, 0.57125, 0.005575, 5.6175,4);
  chassis.set_turn_exit_conditions(1, 151.5, 975);
  chassis.turn_to_angle(0);

  chassis.drive_max_voltage = 10.5;

  chassis.drive_timeout = 2500;
  chassis.drive_settle_time=500;
  chassis.drive_distance(-87.35);
  
  chassis.set_turn_constants(12, 0.57125, 0.005575, 5.6175,4);
  chassis.set_turn_exit_conditions(1, 151.5, 975);
  chassis.turn_to_angle(90);
  chassis.drive_max_voltage = 15.5;
  chassis.drive_settle_time=0;
  chassis.drive_distance(10.5, 95.5);
  toggleMatchLoad();
  chassis.drive_settle_time=500;
  chassis.drive_max_voltage = 5000.5;
  chassis.drive_distance(45.5, 91.75);
  toggleMatchLoad();
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

void overide(){
  driverOveride = true;
}
void checkButton(){
  if(driverOveride){
    Brain.Screen.clearScreen();


    odom_constants();
    chassis.set_coordinates(72, 25, 0);
    float tps = 0;
    int tick_count = 0;
    int total_tick_count = 0;
    int tps_update_interval = 1000; // in milliseconds
    int last_tps_update = Brain.timer(msec);
    Controller1.Screen.clearScreen();
    

    Controller1.ButtonR2.pressed(intakeOut);
    Controller1.ButtonR1.pressed(intakeIn);

    Controller1.ButtonR2.released(intakeOff);
    Controller1.ButtonR1.released(intakeOff);

    Controller1.ButtonL1.pressed(highGoal);
    Controller1.ButtonL2.pressed(middleGoal);

    Controller1.ButtonL1.released(stopSecondStage);
    Controller1.ButtonL2.released(stopSecondStage);

    Controller1.ButtonB.pressed(lowerTotal);
    Controller1.ButtonX.pressed(toggleColorSort);

    Controller1.ButtonA.released(aPressed);
    Controller1.ButtonA.released(aReleased);


    Controller1.ButtonY.pressed(toggleDescore);

    Controller1.ButtonDown.pressed(toggleMatchLoad);

    Controller1.ButtonRight.pressed(toggleAligner);


    intakeMotor.setVelocity(100, percent);
    middleTravel.setVelocity(85, percent);
    scoreMotor.setVelocity(100, percent);

    intakeInterface.startIntakeThread();
    intakeInterface.intake_status = OFF_intake;
    intakeInterface.travel_status = OFF_travel;

    intakeMotor.stop();
    middleTravel.stop();
    scoreMotor.stop();

    sorter.startColorSort();
    sorter.sortStatus = NONE;
    
    chassis.drive_stop(coast);
    while (1) {
      //chassis.control_arcade_left();
      

      chassis.control_tank_ricky();
      //chassis.control_arcade();

      /*
      if(Brain.Battery.current() < 10){
        Controller1.rumble(".");
      }
      */
      /*
      if(Controller1.ButtonA.pressing()){
        sorter.sortStatus = NONE;
        intakeMotor.spin(forward);
        scoreMotor.stop();
        if((!Optical12.isNearObject()) && !(Optical12.hue() < 10) && !(70 < Optical12.hue() && Optical12.hue() < 230)){
          middleTravel.spin(forward);
        }
        else{
          middleTravel.stop();
        }
      }
      */


      tick_count++;
      total_tick_count++;

      int now = Brain.timer(msec);



      task::sleep(2); // Sleep the task for a short amount of time to
                      // prevent wasted resources.
    }
  }
}

void driverProgSkills(){
  Controller1.ButtonA.pressed(overide);
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

  chassis.set_coordinates(96-7.25, 18.5, 270);
  toggleMatchLoad();
  toggleAligner();
  big_drive_constants_forwards();
  chassis.drive_timeout = 1500;
  chassis.drive_settle_time = 350;
  chassis.drive_max_voltage = 12;
  chassis.drive_distance(-30.25, 270);

  checkButton();

  small_turn_constants_12v();
  intakeIn();
  intakeInterface.travel_status = IN_CHECK;
  chassis.turn_to_angle(180);

  checkButton();

  chassis.drive_timeout = 900.5;

  chassis.drive_distance(6.125, 180);

  checkButton();

  wait(0.515,seconds); //was 1.5 seconds
  chassis.drive_max_voltage = 3.25;
  chassis.drive_distance(1.656, 180);
  checkButton();
  wait(0.55, seconds);
  //chassis.drive_timeout = 560.5; //was 662.5
  //chassis.drive_settle_time = 350;
  //chassis.drive_max_voltage = 3.25; //was 2.25
  //wait(0.025, seconds);
  chassis.drive_timeout = 1750;
  chassis.drive_settle_time = 0; //was 350
  chassis.drive_max_voltage = 11.5;
  chassis.drive_distance(-23.5, 181.5);
  checkButton();
  chassis.drive_settle_time = 150;
  highGoal();
  chassis.drive_timeout = 250;
  chassis.drive_max_voltage = 9.5;
  chassis.drive_distance(-8, 180);
  checkButton();
  toggleMatchLoad();
  chassis.DriveL.stop();
  chassis.DriveR.stop();
  wait(1.275,seconds); //was 1.85 seconds
  checkButton();
  stopSecondStage();
  checkButton();
  wait(0.315,seconds);
  checkButton();
  highGoal();
  wait(1.25,seconds);
  checkButton();
  
  stopSecondStage();
  chassis.drive_timeout = 1750;
  chassis.drive_max_voltage = 10.5; //was 10.5
  chassis.drive_distance(10,180);
  checkButton();


  big_turn_fast_speed_constants_12v();

  chassis.turn_to_angle(-308.85);
  checkButton();
  chassis.drive_max_voltage = 9.5;
  chassis.drive_distance(15.125,-329.75);
  checkButton();

  chassis.set_turn_constants(12, 0.56625, 0.0058625, 5.1625,4);
  chassis.set_turn_exit_conditions(1, 128.5, 1075); //settle time could even go as low as 108 maybe, but I don't want to risk it on what should be a safe route
  chassis.turn_to_angle(10);
  checkButton();

  chassis.drive_distance(60.85,359);
  checkButton();
  chassis.drive_max_voltage = 8.125;
  
  chassis.drive_distance(6,355.25);
  checkButton();

  chassis.set_turn_constants(12, 0.56625, 0.0055625, 5.1625,4);
  chassis.set_turn_exit_conditions(1, 253.5, 1075);
  chassis.turn_to_angle(307);
  checkButton();

  //chassis.drive_to_point(133.25, 114.5);
  toggleMatchLoad();
  chassis.drive_max_voltage = 9.5;
  chassis.drive_distance(16.475); //was 17.25
  checkButton();
  //chassis.drive_to_point(116.65, 115.5);
  chassis.turn_to_angle(359.5);
  checkButton();
  chassis.drive_max_voltage = 9.5;
  intakeIn();
  intakeInterface.travel_status = IN_CHECK;
  chassis.drive_distance(11.487,0);
  checkButton();
  chassis.drive_max_voltage = 3.5;
  chassis.drive_distance(3.767,0);
  checkButton();
  wait(1, seconds);
  checkButton();
  chassis.drive_max_voltage = 8;
  chassis.drive_distance(-24, 357.105);
  checkButton();
  toggleMatchLoad();
  highGoal();
  chassis.drive_distance(-9, 0.65);
  checkButton();


  wait(1.325,seconds); //was 1.85 seconds
  checkButton();
  stopSecondStage();
  wait(0.25,seconds);
  checkButton();
  highGoal();
  wait(0.95,seconds);
  checkButton();


  stopSecondStage();
  //chassis.drive_distance(8.5, 0);
  chassis.drive_max_voltage = 7.5;
  chassis.drive_settle_time = 500;
  //chassis.drive_to_point(118.75, (96+10.855+8)); //96 + 10.855 inches back + botlength/2
  chassis.drive_distance(15.0955,0);
  checkButton();

  //chassis.set_turn_constants(12, 0.56625, 0.0055625, 5.1625,4); //old
  //chassis.set_turn_exit_conditions(1, 253.5, 1075); //old
  chassis.set_turn_constants(12, 0.56625, 0.0055625, 5.2825,4); //new
  chassis.set_turn_exit_conditions(1, 223.5, 1075); //new
  chassis.turn_to_angle(225);
  checkButton();

  intakeIn();
  intakeInterface.travel_status = IN_CHECK;

  chassis.drive_settle_time = 150;
  chassis.drive_max_voltage = 8;
  chassis.drive_distance(18.25, 225);
  checkButton();
  chassis.drive_max_voltage = 2.75;
  chassis.drive_distance(18.5, 225);
  checkButton();

  chassis.set_turn_constants(12, 0.57125, 0.005575, 5.6875,4);
  chassis.set_turn_exit_conditions(1, 151.5, 975);
  chassis.turn_to_angle(46.15); //was 225
  checkButton();

  scoreMotor.setVelocity(48.5, percent);
  intakeInterface.middleGoalDelay(205.5, msec);

  chassis.drive_max_voltage = 6.15;

  chassis.drive_distance(-11.915); //was -15.05
  checkButton();
  chassis.DriveL.setVelocity(0, percent);
  chassis.DriveR.setVelocity(0, percent);
  
  //chassis.DriveL.setStopping(hold);
  //chassis.DriveR.setStopping(hold);
  wait(1.085, seconds);
  checkButton();
  //chassis.DriveL.setStopping(coast);
  //chassis.DriveR.setStopping(coast);

  chassis.drive_max_voltage = 9.5;
  toggleMatchLoad();
  stopSecondStage();
  scoreMotor.stop();

  chassis.drive_distance(23);
  checkButton();

  chassis.turn_to_angle(281.5);
  checkButton();

  chassis.drive_timeout = 1750;
  chassis.swing_settle_time = 550;

  chassis.drive_distance(80.014265); //was 79.14265 could be also 76 depending on whether or not it was actually just timing out earlier
  checkButton();
  
  //chassis.set_turn_constants(12, 0.57125, 0.005575, 5.6175,4); //old
  //chassis.set_turn_exit_conditions(1, 151.5, 975); //old
  chassis.set_turn_constants(12, 0.57625, 0.006275, 5.6175,4);
  chassis.set_turn_exit_conditions(1, 151.5, 975);
  chassis.turn_to_angle(0);
  checkButton();
  
  intakeIn();
  intakeInterface.intake_status = IN;
  intakeInterface.travel_status = IN_CHECK;

  chassis.drive_timeout = 900.5;
  chassis.swing_settle_time = 150;
  chassis.drive_distance(14.925, 0); //was 11.925
  checkButton();
  wait(0.75,seconds); //was 1.5 seconds
  checkButton();
  chassis.drive_max_voltage = 4.25;
  chassis.drive_distance(1.996, 0);
  checkButton();
  wait(0.55, seconds);
  checkButton();
  //chassis.drive_timeout = 560.5; //was 662.5
  //chassis.drive_settle_time = 350;
  //chassis.drive_max_voltage = 3.25; //was 2.25
  //wait(0.025, seconds);
  chassis.drive_timeout = 1750;
  chassis.drive_settle_time = 0; //was 350
  chassis.drive_max_voltage = 11.5;
  chassis.drive_distance(-23.5, 356.55);
  checkButton();
  chassis.drive_settle_time = 150;
  highGoal();
  chassis.drive_timeout = 250;
  chassis.drive_max_voltage = 9.5;
  chassis.drive_distance(-8, 0.75);
  checkButton();
  toggleMatchLoad();

  wait(1.475,seconds); //was 1.85 seconds
  checkButton();
  
  stopSecondStage();
  chassis.drive_timeout = 1750;
  chassis.drive_max_voltage = 10.5; //was 10.5
  chassis.drive_distance(10,0);
  checkButton();
  

  //GO PARK SAFETY

  chassis.set_turn_constants(12, 0.57125, 0.005575, 5.6175,4);
  chassis.set_turn_exit_conditions(1, 151.5, 975);
  chassis.turn_to_angle(315);
  checkButton();

  chassis.drive_max_voltage = 10.5;
  chassis.drive_distance(-22.5);
  checkButton();

  chassis.set_turn_constants(12, 0.57125, 0.005575, 5.6175,4);
  chassis.set_turn_exit_conditions(1, 151.5, 975);
  chassis.turn_to_angle(0);
  checkButton();

  chassis.drive_max_voltage = 10.5;

  chassis.drive_timeout = 2500;
  chassis.drive_settle_time=500;
  chassis.drive_distance(-85.55);
  checkButton();
  
  chassis.set_turn_constants(12, 0.57125, 0.005575, 5.6175,4);
  chassis.set_turn_exit_conditions(1, 151.5, 975);
  chassis.turn_to_angle(90);
  checkButton();
  chassis.drive_max_voltage = 15.5;
  chassis.drive_settle_time=0;
  chassis.drive_distance(10.5, 95.5);
  checkButton();
  toggleMatchLoad();
  chassis.drive_settle_time=500;
  chassis.drive_max_voltage = 5000.5;
  chassis.drive_distance(35.5, 91.75);
  checkButton();
  toggleMatchLoad();
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

void rightDiddler(){ //WORKED ON
  //sorter.startColorSort();
  finalSortStatus = RED;
  defaultSortStatus = RED;
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
  intakeInterface.travel_status = REVERSE;


  chassis.right_swing_to_angle(25.5);
  chassis.swing_timeout = 1000;

  chassis.set_coordinates(84, 22.5, chassis.get_absolute_heading());

  chassis.drive_max_voltage = 12;
  chassis.drive_distance(17.25,20);
  toggleAligner();
  chassis.drive_max_voltage = 1.875;
  chassis.drive_distance(5.325,20);
  chassis.drive_distance(5.7, 28.5);
  chassis.drive_max_voltage = 12;


  chassis.set_turn_constants(12, 0.56525, 0.005475, 4.7275,4);
  chassis.set_turn_exit_conditions(1, 172.5, 975);
  chassis.turn_to_angle(315);

  chassis.drive_distance(15.95);
  intakeInterface.intake_status = OUT;
  wait(0.65, seconds);

  chassis.drive_max_voltage = 9.5;
  toggleMatchLoad();
  //chassis.drive_distance(-53.95);
  chassis.drive_to_point(119, 24);

  chassis.set_turn_constants(12, 0.56625, 0.0055625, 5.1625,4);
  chassis.set_turn_exit_conditions(1, 253.5, 1075);
  chassis.turn_to_angle(180);
  intakeIn();
  intakeInterface.travel_status = IN_CHECK;

  chassis.drive_max_voltage = 7.5;
  chassis.drive_settle_time = 400;
  chassis.drive_settle_time = 75;
  chassis.drive_distance(7.45);

  wait(0.5, seconds);

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

  chassis.set_coordinates(24, (48-8.5), chassis.get_absolute_heading());

  stopSecondStage();

}

void redHook(){

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
  intakeInterface.descoreDelay(50, msec);
  chassis.set_turn_constants(12, 0.56525, 0.005775, 4.625,4);
  chassis.set_turn_exit_conditions(1, 62.5, 975);
  chassis.turn_to_angle(197.315); //was 193.5

  chassis.drive_max_voltage = 4.8525;
  chassis.drive_distance(-14.15,186.95); //was 184.5

  chassis.drive_stop(hold);
  chassis.drive_max_voltage = 5.95;
  chassis.drive_distance(-2.75,192.5); //was 182.5
}

void redLeftDiddler(){ //WORKED ON
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


  chassis.left_swing_to_angle(0-25.5);
  chassis.swing_timeout = 1000;

  chassis.set_coordinates(60, 22.5, chassis.get_absolute_heading());

  chassis.drive_max_voltage = 12;
  chassis.drive_distance(17.25,340);
  toggleAligner();
  chassis.drive_max_voltage = 1.875;
  chassis.drive_distance(5.325,340);
  //chassis.drive_distance(7.965, 331.5); //old
  chassis.drive_settle_time = 150;
  chassis.drive_distance(7.265, 331.5); //new
  chassis.drive_max_voltage = 12;


  //chassis.set_turn_constants(12, 0.57125, 0.005575, 5.6175,4); //old
  //chassis.set_turn_exit_conditions(1, 151.5, 975); //old
  chassis.set_turn_constants(12, 0.57125, 0.006775, 5.90775,4); //new
  chassis.set_turn_exit_conditions(1, 151.5, 975); //new
  chassis.turn_to_angle(222.5); //was 225

  scoreMotor.setVelocity(48.5, percent);
  intakeInterface.middleGoalDelay(152.5, msec); //was 108.5

  chassis.drive_settle_time = 75;
  chassis.drive_distance(-14.905); //was -15.05
  chassis.DriveL.setVelocity(0, percent);
  chassis.DriveR.setVelocity(0, percent);
  
  //chassis.DriveL.setStopping(hold);
  //chassis.DriveR.setStopping(hold);
  wait(0.4945, seconds); //was 0.455
  //chassis.DriveL.setStopping(coast);
  //chassis.DriveR.setStopping(coast);

  chassis.drive_max_voltage = 9.5;
  toggleMatchLoad();
  stopSecondStage();
  scoreMotor.stop();
  //scoreMotor.spin(reverse);
  chassis.drive_timeout = 1500;
  chassis.drive_settle_time = 415;
  chassis.drive_settle_error = 1.05;
  chassis.drive_distance(49.48795);//was 48.32795
  //chassis.drive_to_point(26.075, 28);
  stopSecondStage();
  scoreMotor.setVelocity(100, percent);

  //chassis.set_turn_constants(12, 0.62625, 0.0068625, 5.5025,4); //d was 5.3625, p was 0.0060625 //old
  //chassis.set_turn_exit_conditions(1, 126.5, 1075); //old
  chassis.set_turn_constants(12, 0.66625, 0.0070625, 6.6225,4);
  chassis.set_turn_exit_conditions(1, 126.5, 1075);
  chassis.turn_to_angle(180);

  intakeIn();
  intakeInterface.travel_status = IN_CHECK;

  chassis.drive_max_voltage = 7.5;
  chassis.drive_settle_error = 1;
  chassis.drive_timeout = 400;
  chassis.drive_settle_time = 0;
  chassis.drive_stop(hold);
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
  /*
  while(!(MatchloadSensor.hue() < 11) && count < 444){ //was 458
    count++;
    task::sleep(2);
  }
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
  chassis.drive_distance(-26.45, 178.085); //was 178.85
  chassis.drive_settle_time = 150;
  highGoal();
  chassis.drive_timeout = 250;
  chassis.drive_max_voltage = 10; //was 9.5
  chassis.drive_distance(-8.5, 179.25); //was -5
  toggleMatchLoad();
  count = 0;
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
    
  //wait(1.915,seconds);


  //intakeInterface.travel_status = OFF_travel;
  //wait(0.15,seconds);

  //chassis.set_coordinates(24, (48-8.5), chassis.get_absolute_heading());

  stopSecondStage();
  
  redHook();


}

void redLeftDiddlerElims(){ //WORKED ON
  //sorter.startColorSort();
  finalSortStatus = RED;
  defaultSortStatus = RED;
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


  chassis.left_swing_to_angle(0-25.5);
  chassis.swing_timeout = 1000;

  chassis.set_coordinates(60, 22.5, chassis.get_absolute_heading());

  chassis.drive_max_voltage = 12;
  chassis.drive_distance(17.25,340);
  toggleAligner();
  chassis.drive_max_voltage = 1.875;
  chassis.drive_distance(5.325,340);
  //chassis.drive_distance(7.965, 331.5); //old
  chassis.drive_settle_time = 150;
  chassis.drive_distance(7.265, 331.5); //new
  chassis.drive_max_voltage = 12;


  //chassis.set_turn_constants(12, 0.57125, 0.005575, 5.6175,4); //old
  //chassis.set_turn_exit_conditions(1, 151.5, 975); //old
  chassis.set_turn_constants(12, 0.57125, 0.006175, 5.8175,4); //new
  chassis.set_turn_exit_conditions(1, 151.5, 975); //new
  chassis.turn_to_angle(225);

  scoreMotor.setVelocity(48.5, percent);
  intakeInterface.middleGoalDelay(141.5, msec); //was 108.5
  chassis.drive_distance(-14.855); //was -15.05
  chassis.DriveL.setVelocity(0, percent);
  chassis.DriveR.setVelocity(0, percent);
  
  //chassis.DriveL.setStopping(hold);
  //chassis.DriveR.setStopping(hold);
  wait(0.40, seconds);
  //chassis.DriveL.setStopping(coast);
  //chassis.DriveR.setStopping(coast);

  chassis.drive_max_voltage = 9.5;
  toggleMatchLoad();
  stopSecondStage();
  scoreMotor.stop();
  //scoreMotor.spin(reverse);
  chassis.drive_timeout = 1500;
  chassis.drive_settle_time = 415;
  chassis.drive_settle_error = 1.05;
  chassis.drive_distance(48.32795);//was 48.2395
  //chassis.drive_to_point(26.075, 28);
  stopSecondStage();
  scoreMotor.setVelocity(100, percent);

  //chassis.set_turn_constants(12, 0.62625, 0.0068625, 5.5025,4); //d was 5.3625, p was 0.0060625 //old
  //chassis.set_turn_exit_conditions(1, 126.5, 1075); //old
  chassis.set_turn_constants(12, 0.63625, 0.0070625, 5.9025,4);
  chassis.set_turn_exit_conditions(1, 126.5, 1075);
  chassis.turn_to_angle(180);

  intakeIn();
  intakeInterface.travel_status = IN_CHECK;

  chassis.drive_max_voltage = 7.5;
  chassis.drive_settle_error = 1;
  chassis.drive_timeout = 400;
  chassis.drive_settle_time = 0;
  chassis.drive_stop(hold);
  chassis.drive_max_voltage = 5.225;
  //chassis.drive_distance(8);//was 11.475 and then 12.775
  //wait(0.5, seconds);

  //chassis.drive_max_voltage = 3.5;
  //chassis.drive_distance(4.645); //was 5.145
  chassis.drive_distance(13.085); //was 12.775
  wait(0.05,seconds);
  LeftMotor1.setVelocity(7.5, percent);
  LeftMotor2.setVelocity(7.5, percent);
  LeftMotor3.setVelocity(7.5, percent);
  RightMotor1.setVelocity(7.5, percent);
  RightMotor2.setVelocity(7.5, percent);
  RightMotor3.setVelocity(7.5, percent);
  LeftMotor1.spin(forward);
  LeftMotor2.spin(forward);
  LeftMotor3.spin(forward);
  RightMotor1.spin(forward);
  RightMotor2.spin(forward);
  RightMotor3.spin(forward);
  int count = 0;
  while(!(40 < MatchloadSensor.hue()) && count < 444){ //was 112
    count++;
    task::sleep(2);
  }
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
  chassis.drive_distance(-26.45, 178.085); //was 178.85
  chassis.drive_settle_time = 150;
  highGoal();
  chassis.drive_timeout = 250;
  chassis.drive_max_voltage = 10; //was 9.5
  chassis.drive_distance(-8.5, 180); //was -5
  toggleMatchLoad();
  count = 0;
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
    
  //wait(1.915,seconds);


  //intakeInterface.travel_status = OFF_travel;
  //wait(0.15,seconds);

  //chassis.set_coordinates(24, (48-8.5), chassis.get_absolute_heading());

  stopSecondStage();
  
  redHook();


}

void blueHook(){

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
  intakeInterface.descoreDelay(50, msec);
  chassis.set_turn_constants(12, 0.56525, 0.005775, 4.625,4);
  chassis.set_turn_exit_conditions(1, 62.5, 975);
  chassis.turn_to_angle(197.315); //was 193.5

  chassis.drive_max_voltage = 4.8525;
  chassis.drive_distance(-14.15,186.95); //was 184.5

  chassis.drive_stop(hold);
  chassis.drive_max_voltage = 5.95;
  chassis.drive_distance(-2.75,192.5); //was 182.5
}

void blueLeftDiddler(){ //WORKED ON
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


  chassis.left_swing_to_angle(0-25.5);
  chassis.swing_timeout = 1000;

  chassis.set_coordinates(60, 22.5, chassis.get_absolute_heading());

  chassis.drive_max_voltage = 12;
  chassis.drive_distance(17.25,340);
  toggleAligner();
  chassis.drive_max_voltage = 1.875;
  chassis.drive_distance(5.325,340);
  //chassis.drive_distance(7.965, 331.5); //old
  chassis.drive_settle_time = 150;
  chassis.drive_distance(7.265, 331.5); //new
  chassis.drive_max_voltage = 12;


  //chassis.set_turn_constants(12, 0.57125, 0.005575, 5.6175,4); //old
  //chassis.set_turn_exit_conditions(1, 151.5, 975); //old
  chassis.set_turn_constants(12, 0.57125, 0.006775, 5.90775,4); //new
  chassis.set_turn_exit_conditions(1, 151.5, 975); //new
  chassis.turn_to_angle(222.5); //was 225

  scoreMotor.setVelocity(48.5, percent);
  intakeInterface.middleGoalDelay(152.5, msec); //was 108.5

  chassis.drive_settle_time = 75;
  chassis.drive_distance(-14.905); //was -15.05
  chassis.DriveL.setVelocity(0, percent);
  chassis.DriveR.setVelocity(0, percent);
  
  //chassis.DriveL.setStopping(hold);
  //chassis.DriveR.setStopping(hold);
  wait(0.4945, seconds); //was 0.455
  //chassis.DriveL.setStopping(coast);
  //chassis.DriveR.setStopping(coast);

  chassis.drive_max_voltage = 9.5;
  toggleMatchLoad();
  stopSecondStage();
  scoreMotor.stop();
  //scoreMotor.spin(reverse);
  chassis.drive_timeout = 1500;
  chassis.drive_settle_time = 415;
  chassis.drive_settle_error = 1.05;
  chassis.drive_distance(49.48795);//was 48.32795
  //chassis.drive_to_point(26.075, 28);
  stopSecondStage();
  scoreMotor.setVelocity(100, percent);

  //chassis.set_turn_constants(12, 0.62625, 0.0068625, 5.5025,4); //d was 5.3625, p was 0.0060625 //old
  //chassis.set_turn_exit_conditions(1, 126.5, 1075); //old
  chassis.set_turn_constants(12, 0.66625, 0.0070625, 6.6225,4);
  chassis.set_turn_exit_conditions(1, 126.5, 1075);
  chassis.turn_to_angle(180);

  intakeIn();
  intakeInterface.travel_status = IN_CHECK;

  chassis.drive_max_voltage = 7.5;
  chassis.drive_settle_error = 1;
  chassis.drive_timeout = 400;
  chassis.drive_settle_time = 0;
  chassis.drive_stop(hold);
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
  /*
  while(!(MatchloadSensor.hue() < 11) && count < 444){ //was 458
    count++;
    task::sleep(2);
  }
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
  chassis.drive_distance(-26.45, 178.085); //was 178.85
  chassis.drive_settle_time = 150;
  highGoal();
  chassis.drive_timeout = 250;
  chassis.drive_max_voltage = 10; //was 9.5
  chassis.drive_distance(-8.5, 179.25); //was -5
  toggleMatchLoad();
  count = 0;
  while(count < 1914/2){
    count++;
    task::sleep(2);
    if(Optical21.hue() < 11){
      wait(0.175,seconds);
      middleTravel.stop();
      intakeInterface.travel_status = OFF_travel;
      wait(0.075,seconds);
      break;
    }
  }
    
  //wait(1.915,seconds);


  //intakeInterface.travel_status = OFF_travel;
  //wait(0.15,seconds);

  //chassis.set_coordinates(24, (48-8.5), chassis.get_absolute_heading());

  stopSecondStage();
  
  blueHook();


}

void blueLeftDiddlerElims(){ //WORKED ON
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


  chassis.left_swing_to_angle(0-25.5);
  chassis.swing_timeout = 1000;

  chassis.set_coordinates(60, 22.5, chassis.get_absolute_heading());

  chassis.drive_max_voltage = 12;
  chassis.drive_distance(17.25,340);
  toggleAligner();
  chassis.drive_max_voltage = 1.875;
  chassis.drive_distance(5.325,340);
  //chassis.drive_distance(7.965, 331.5); //old
  chassis.drive_settle_time = 150;
  chassis.drive_distance(7.265, 331.5); //new
  chassis.drive_max_voltage = 12;


  //chassis.set_turn_constants(12, 0.57125, 0.005575, 5.6175,4); //old
  //chassis.set_turn_exit_conditions(1, 151.5, 975); //old
  chassis.set_turn_constants(12, 0.57125, 0.006375, 5.8175,4); //new
  chassis.set_turn_exit_conditions(1, 151.5, 975); //new
  chassis.turn_to_angle(225);

  scoreMotor.setVelocity(48.5, percent);
  intakeInterface.middleGoalDelay(141.5, msec); //was 108.5

  chassis.drive_settle_time = 75;
  chassis.drive_distance(-14.855); //was -15.05
  chassis.DriveL.setVelocity(0, percent);
  chassis.DriveR.setVelocity(0, percent);
  
  //chassis.DriveL.setStopping(hold);
  //chassis.DriveR.setStopping(hold);
  wait(0.40, seconds);
  //chassis.DriveL.setStopping(coast);
  //chassis.DriveR.setStopping(coast);

  chassis.drive_max_voltage = 9.5;
  toggleMatchLoad();
  stopSecondStage();
  scoreMotor.stop();
  //scoreMotor.spin(reverse);
  chassis.drive_timeout = 1500;
  chassis.drive_settle_time = 415;
  chassis.drive_settle_error = 1.05;
  chassis.drive_distance(48.12795);//was 48.32795
  //chassis.drive_to_point(26.075, 28);
  stopSecondStage();
  scoreMotor.setVelocity(100, percent);

  //chassis.set_turn_constants(12, 0.62625, 0.0068625, 5.5025,4); //d was 5.3625, p was 0.0060625 //old
  //chassis.set_turn_exit_conditions(1, 126.5, 1075); //old
  chassis.set_turn_constants(12, 0.69625, 0.0070625, 5.8225,4);
  chassis.set_turn_exit_conditions(1, 126.5, 1075);
  chassis.turn_to_angle(180);

  intakeIn();
  intakeInterface.travel_status = IN_CHECK;

  chassis.drive_max_voltage = 7.5;
  chassis.drive_settle_error = 1;
  chassis.drive_timeout = 400;
  chassis.drive_settle_time = 0;
  chassis.drive_stop(hold);
  chassis.drive_max_voltage = 5.225; //was 7.5
  //chassis.drive_distance(8);//was 11.475 and then 12.775
  //wait(0.5, seconds);

  //chassis.drive_max_voltage = 3.5;
  //chassis.drive_distance(4.645); //was 5.145
  //chassis.drive_distance(13.085); //was 12.775
  chassis.drive_distance(13.285);
  wait(0.05,seconds);
  LeftMotor1.setVelocity(7.5, percent);
  LeftMotor2.setVelocity(7.5, percent);
  LeftMotor3.setVelocity(7.5, percent);
  RightMotor1.setVelocity(7.5, percent);
  RightMotor2.setVelocity(7.5, percent);
  RightMotor3.setVelocity(7.5, percent);
  LeftMotor1.spin(forward);
  LeftMotor2.spin(forward);
  LeftMotor3.spin(forward);
  RightMotor1.spin(forward);
  RightMotor2.spin(forward);
  RightMotor3.spin(forward);
  int count = 0;
  while(!(MatchloadSensor.hue() < 11) && count < 444){ //was 458
    count++;
    task::sleep(2);
  }
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
  chassis.drive_distance(-26.45, 178.085); //was 178.85
  chassis.drive_settle_time = 150;
  highGoal();
  chassis.drive_timeout = 250;
  chassis.drive_max_voltage = 10; //was 9.5
  chassis.drive_distance(-8.5, 180); //was -5
  toggleMatchLoad();
  count = 0;
  while(count < 1914/2){
    count++;
    task::sleep(2);
    if(Optical21.hue() < 11){
      wait(0.175,seconds);
      middleTravel.stop();
      intakeInterface.travel_status = OFF_travel;
      wait(0.075,seconds);
      break;
    }
  }
    
  //wait(1.915,seconds);


  //intakeInterface.travel_status = OFF_travel;
  //wait(0.15,seconds);

  //chassis.set_coordinates(24, (48-8.5), chassis.get_absolute_heading());

  stopSecondStage();
  
  blueHook();


}

/**
 * The expected behavior is to return to the start position.
 */

void drive_test(){
  default_constants();
  chassis.drive_distance(20);
  chassis.turn_to_angle(180);
  chassis.drive_distance(20);
  chassis.turn_to_angle(0);

  //chassis.drive_distance(15, 90);

}

/**
 * The expected behavior is to return to the start angle, after making a complete turn.
 */

void turn_test(){
  chassis.turn_to_angle(5);
  chassis.turn_to_angle(30);
  chassis.turn_to_angle(90);
  chassis.turn_to_angle(225);
  chassis.turn_to_angle(0);
  chassis.turn_to_angle(180);
  chassis.turn_to_angle(0);
}

/**
 * Should swing in a fun S shape.
 */

void swing_test(){
  chassis.left_swing_to_angle(90);
  chassis.right_swing_to_angle(0);
}

void hell_naw(){
  default_constants();
  chassis.set_coordinates(84, 48, 135);
  chassis.drive_settle_time = 100;
  chassis.drive_to_pose(104, 35, 0);
  chassis.drive_settle_time = 500;
  chassis.drive_to_pose(104,46,0);
  chassis.drive_settle_time = 1600;
}

/**
 * A little of this, a little of that; it should end roughly where it started.
 */

void full_test(){
  chassis.drive_distance(24);
  chassis.turn_to_angle(-45);
  chassis.drive_distance(-36);
  chassis.right_swing_to_angle(-90);
  chassis.drive_distance(24);
  chassis.turn_to_angle(0);
}


void odom_test(){
  odom_constants();
  chassis.set_coordinates(72, 48, 0);
  while (1){
    Brain.Screen.clearScreen();
    Brain.Screen.printAt(5,20, "OdomX: %f", chassis.get_X_position());
    Brain.Screen.printAt(5,40, "OdomY: %f", chassis.get_Y_position());
    Brain.Screen.printAt(5,60, "Heading: %f", chassis.get_absolute_heading());
    task::sleep(10);
  }
}