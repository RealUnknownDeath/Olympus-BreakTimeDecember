#include "vex.h"
#include "globals.h"
#include "drive_helpers.h"

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
  descorePiston.set(true);
  descoreStatus = true;
  descorePiston = true;
}

void raiseTotal(){
  if(descoreStatus){
    descorePiston.set(false);
    descoreStatus = false;
    descorePiston = false;
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
  if(descoreStatus){
    if(matchLoadStatus){toggleMatchLoad();}
    descorePiston.set(false);
    descoreStatus = false;
  }
  else{
    descorePiston.set(true);
    descoreStatus = true;
  }
}

void toggleMatchLoad(){
  if(matchLoadStatus){ 
    matchLoadPiston.set(false);
    matchLoadStatus = false;
  }
  else{
    if(descoreStatus){raiseTotal();}
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
  alignerStatus = !alignerStatus;
  if(alignerStatus){
    if(matchLoadStatus){toggleMatchLoad();}
    Aligner.set(true);
  }
  else{
    Aligner.set(false);
  }
}