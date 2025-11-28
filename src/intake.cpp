#include "vex.h"

intake::intake(int colorSensor1Port, intakeStatus intake_status, travelStatus travel_status, int intake_PORT, int travel_PORT):
  colorSensor1(optical(colorSensor1Port)),
  intake_status(intake_status),
  travel_status(travel_status),
  intakeMotorInstance(abs(intake_PORT), is_reversed(intake_PORT)),
  travelMotorInstance(abs(travel_PORT), is_reversed(travel_PORT)),
  intakeVel(100),
  travelVel(100),
  middleActivate(-1),
  highActivate(-1),
  checkFrames(0),
  descoreActivate(-1),
  highA(false)
{
  colorSensor1.objectDetectThreshold(185);
}

void intake::middleGoalDelay(double delay, timeUnits units){
  this->middleActivate = Brain.Timer.time(units) + delay;
}
void intake::highGoalDelay(double delay, timeUnits units){
  this->highActivate = Brain.Timer.time(units) + delay;
}

void intake::descoreDelay(double delay, timeUnits units){
  this->descoreActivate = Brain.Timer.time(units) + delay;
}

void intake::setIntakeVelocity(double num){
  this->intakeVel = num;
  this->intakeMotorInstance.setVelocity(num, percent);

}
void intake::setTravelVelocity(double num){
  this->travelVel = num;
  this->travelMotorInstance.setVelocity(num, percent);
}

void intake::intakeLoop(){ //the loop for intake
  colorSensor1.setLightPower(100);
  while (1){
  
    if(Brain.Timer.time(msec) >= this->middleActivate && this->middleActivate != -1){
      middleGoal();
      this->middleActivate = -1;
    }

    if(Brain.Timer.time(msec) >= this->highActivate && this->highActivate != -1){
      highGoal();
      this->highActivate = -1;
    }

    if(Brain.Timer.time(msec) >= this->descoreActivate && this->descoreActivate != -1){
      toggleDescore();
      this->descoreActivate = -1;
    }

    if(this->intake_status == NONE_intake){

    }
    else if(this->intake_status == IN){
      this->intakeMotorInstance.spin(forward);
      this->intakeMotorInstance.setVelocity(intakeVel, percent);
    }
    else if(this->intake_status == OUT){
      this->intakeMotorInstance.setVelocity(intakeVel, percent);
      this->intakeMotorInstance.spin(reverse);
      
    }
    else if(this->intake_status == OFF_intake){
      this->intakeMotorInstance.stop();
    }
    if (this->travel_status == NONE_travel){
    }
    else if(this->travel_status == IN_CHECK){
      if(colorSensor1.isNearObject()){
        //this->travelMotorInstance.stop();
        this->checkFrames = 0;
        this->travel_status = IN_HOLD;
      }
      else{
        this->travelMotorInstance.setVelocity(travelVel, percent);
        this->travelMotorInstance.spin(forward);
  
      }
      
    }
    else if(this->travel_status == IN_HOLD){
      if(!colorSensor1.isNearObject()){
        this->checkFrames++;
      }
      if(this->checkFrames > 4){
        setTravelVelocity(100);
        this->travelMotorInstance.setVelocity(100, percent);
        this->travelMotorInstance.spin(forward);
        this->travelMotorInstance.setVelocity(travelVel, percent);
        this->travel_status = IN_CHECK;
        this->checkFrames = 0;
        scoreMotor.setVelocity(100, percent);
        scoreMotor.stop();
      }
      else{
        //this->travelMotorInstance.stop();
        setTravelVelocity(5);
        this->travelMotorInstance.setVelocity(5, percent);
        this->travelMotorInstance.spin(forward);
        if(highA){
          scoreMotor.setVelocity(0, percent);
          setTravelVelocity(0);
          this->travelMotorInstance.setVelocity(0, percent);
          this->travelMotorInstance.spin(forward);
        }
        else{
          scoreMotor.setVelocity(0.5, percent);
        }
        scoreMotor.spin(forward);
      }
    }
    else if(this->travel_status == IN_BLIND){
      setTravelVelocity(100);
      this->travelMotorInstance.setVelocity(100, percent);
      this->travelMotorInstance.spin(forward);
      this->travelMotorInstance.setVelocity(travelVel, percent);
    }
    else if(this->travel_status == REVERSE){
      this->travelMotorInstance.setVelocity(travelVel, percent);
      this->travelMotorInstance.spin(reverse);
      middleTravel.spin(reverse);
    }
    else if(this->travel_status == OFF_travel){
      this->travelMotorInstance.stop();
      this->travelMotorInstance.setVelocity(travelVel, percent);
    }
    task::sleep(2);
  }
}
/*
void intake::activateCapacity(){
  this->status1 =
}
*/
int intake::intake_loop_task(void *instance){ //the "vex task" (basically a thread) for the intake
  intake* self = static_cast<intake*>(instance);
  task::sleep(10);
  self->intakeLoop();
  return 0;
}

void intake::startIntakeThread(){ //starts up the intake loop
  intake_task = task(intake_loop_task,this);
}
