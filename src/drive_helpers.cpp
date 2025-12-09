#include "vex.h"
#include "globals.h"
#include "drive_helpers.h"


void loaderMacro(){
  if(DistanceRight.objectDistance(inches) >= DistanceLeft.objectDistance(inches)){
    //Right matchloader
    double heading_target = 180;
    chassis.ml_max_forward_voltage = 12;
    chassis.ml_slow_down_factor = 0.325; //was 0.95

    if(within_range(chassis.get_absolute_heading(), 180, 90)){
      heading_target = 180;
    }
    else{
      heading_target = 0;
    }
    default_constants();

    chassis.ml_hold_time_ms = 1250;
    chassis.ml_align_phase_end = 0.65;
    chassis.ml_slow_down_start = 0.785; //was 0.95
    chassis.ml_kAlign = 3.85;
    chassis.ml_kF = 0.865; //was 0.50
    chassis.ml_hold_pressure = 2.05;
    
    
    chassis.left_front_sensor_drive_distance(3.65, 17.95, heading_target);
  }

  
  else if(DistanceRight.objectDistance(inches) < DistanceLeft.objectDistance(inches)){
    //Left matchloader

    
  }
  
}

void intakeIn(){
  intakeInterface.intake_status = IN;
  intakeMotor.spin(forward);
}
void intakeOut(){
  intakeInterface.intake_status = OUT;
  intakeMotor.spin(reverse);
  intakeInterface.setTravelVelocity(100);
  intakeInterface.travel_status = REVERSE;
  middleTravel.spin(reverse);
}

void intakeOff(){
  if(!Aactive && !Controller1.ButtonL1.pressing()){
    intakeInterface.intake_status = OFF_intake;
    intakeMotor.stop();
    intakeInterface.travel_status = OFF_travel;
    
  }
  if(Aactive){
    intakeMotor.spin(forward);
    middleTravel.spin(forward);
    intakeInterface.travel_status = IN_CHECK;
    intakeInterface.intake_status = IN;
  }
}

void raise(){
  outtakeRaise.set(false);
}

void lower(){
  outtakeRaise.set(true);
}

void lowerTotal(){
  if(matchLoadStatus){toggleMatchLoad();}
  outtakeRaise.set(true);
  descoreLift.set(false);
  descoreLiftStatus = false;
  descorePiston.set(false);
  descoreStatus = false;
}

void raiseTotal(){
  if(!descoreLiftStatus){
    descoreLift.set(true);
    descoreLiftStatus = true;
  }
  outtakeRaise.set(false);
}

void highGoal(){
  scoreOverride = true;
  sorter.sortStatus = defaultSortStatus;
  descoreStatus = false;
  descorePiston = false;
  scoreMotor.setVelocity(100, percent);
  descorePiston.set(false);
  descoreLift.set(true);
  descoreLiftStatus = true;
  intakeInterface.intake_status = IN;
  intakeInterface.travel_status = NONE_travel;
  middleTravel.spin(forward);
  scoreMotor.spin(forward);
  intakeMotor.spin(forward);
}

void middleGoal(){
  scoreOverride = true;
  sorter.sortStatus = NONE;
  raise();
  descoreLift.set(true);
  descoreLiftStatus = true;
  intakeInterface.intake_status = IN;
  intakeInterface.travel_status = IN_BLIND;
  scoreMotor.setVelocity(42.5, percent);
  middleTravel.spin(forward);
  scoreMotor.spin(reverse);
  intakeMotor.spin(forward);
}

void stopSecondStage(){
  if(!Controller1.ButtonR1.pressing() && !Controller1.ButtonR2.pressing()){
    scoreOverride = false;
    sorter.sortStatus = NONE;
    raise();
    scoreMotor.stop();
    if(!Aactive){
      intakeInterface.intake_status = OFF_intake;
      intakeInterface.travel_status = OFF_travel;
      middleTravel.stop();
      intakeMotor.stop();
    }
  }
  else if(Controller1.ButtonR1.pressing()){
    scoreOverride = false;
    sorter.sortStatus = NONE;
    raise();
    scoreMotor.stop();
    intakeInterface.travel_status = OFF_travel;
    middleTravel.stop();
    intakeInterface.intake_status = IN;
  }
  else if(Controller1.ButtonR2.pressing()){
    scoreOverride = false;
    sorter.sortStatus = NONE;
    raise();
    scoreMotor.stop();
    intakeInterface.travel_status = OFF_travel;
    middleTravel.stop();
    intakeInterface.intake_status = OUT;
  }
}

void aPressed(){

  if(Aactive){
    if(!(Controller1.ButtonL1.pressing()||Controller1.ButtonL2.pressing())){
      scoreMotor.stop();
    }
    intakeInterface.intake_status = OFF_intake;
    intakeInterface.travel_status = OFF_travel;
    intakeMotor.stop();
    middleTravel.stop();
    Aactive = false;
  }
  else{
    intakeInterface.intake_status = IN;
    intakeInterface.travel_status = IN_CHECK;
    intakeMotor.spin(forward);
    middleTravel.spin(forward);
    Aactive = true;
  }

  /*
  while(1){
    if(Controller1.ButtonA.pressing()){
      sorter.sortStatus = NONE;
      intakeMotor.spin(forward);
      scoreMotor.stop();
      if((!Optical21.isNearObject()) && !(Optical21.hue() < 10) && !(70 < Optical21.hue() && Optical21.hue() < 230)){
        middleTravel.spin(forward);
      }
      else{
        middleTravel.stop();
      }
    }
  }
  */
}

void aReleased(){
  intakeMotor.stop();
  //while(!Optical12.isNearObject()){
    //task::sleep(5);
    //middleTravel.spinFor(reverse, 5, degrees);
  //}
  middleTravel.stop();
}

void toggleDescore(){
  if(!descoreLiftStatus){
    descoreLift.set(true);
    descoreLiftStatus = true;
  }
  if(descoreStatus){
    descorePiston.set(false);
    descoreStatus = false;
  }
  else{
    descorePiston.set(true);
    descoreStatus = true;
  }
}

void toggleMatchLoad(){
  //descorePiston.set(true);
  //descoreStatus = true;
  if(matchLoadStatus){ 
    matchLoadPiston.set(false);
    matchLoadStatus = false;
  }
  else{
    //if(descoreStatus){raiseTotal();}
    if(alignerStatus){toggleAligner();}
    matchLoadPiston.set(true);
    matchLoadStatus = true;
  }
}

void toggleColorSort(){
  if(defaultSortStatus == finalSortStatus){
    defaultSortStatus = NONE;
  }
  else{
    defaultSortStatus = finalSortStatus;
  }
}

void toggleAligner(){
  //descorePiston.set(false);
  //descoreStatus = false;
  alignerStatus = !alignerStatus;
  if(alignerStatus){
    if(matchLoadStatus){toggleMatchLoad();}
    Aligner.set(true);
  }
  else{
    Aligner.set(false);
  }
}