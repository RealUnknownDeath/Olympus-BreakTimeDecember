#pragma once
#include "vex.h"

enum intakeStatus {IN, OUT, OFF_intake, NONE_intake};
enum travelStatus {IN_CHECK, IN_BLIND, IN_HOLD, REVERSE, OFF_travel, NONE_travel};

class intake
{
  private:
    motor intakeMotorInstance;
    motor travelMotorInstance;

    double intakeVel;
    double travelVel;

    double middleActivate;
    double highActivate;

    double descoreActivate;

    int checkFrames;
  public:

    //variables:
    
    intakeStatus intake_status;
    travelStatus travel_status;

    bool highA;
    

    //sensors
    optical colorSensor1;
    //optical colorSensor2;
    
    //functions
    void startIntakeThread();
    static int intake_loop_task(void *instance);
    void intakeLoop();

    void setIntakeVelocity(double num);
    void setTravelVelocity(double num);

    void middleGoalDelay(double delay, timeUnits units);
    void highGoalDelay(double delay, timeUnits units);

    void descoreDelay(double delay, timeUnits units);

    //the intake task (thread)
    vex::task intake_task;

    //constructor:

    intake(int colorSensor1Port, intakeStatus intake_status, travelStatus traval_status, int intake_PORT, int travel_PORT);

};
