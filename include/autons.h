#pragma once
#include "colorSort.h"
#include "drive_helpers.h"
#include "auton_constants.h"
#include "Santi-Template/drive.h"
#include "vex.h"


class Drive;

extern Drive chassis;

void default_constants();

void odom_constants();

void driveToMatchloader();
void testRamsete();

void qualsHookRoute(bool allienceColor); //red true & blue false
void leftHookPushFromGoal(double force);
void slamFromGoal();
void redQualsHook();
void blueQualsHook();

void rightElims9Ball(bool allienceColor); //red true & blue false
void redRightElims9Ball();
void blueRightElims9Ball();

void safe_progSkills();
void risky_progSkills();

void safe_soloAWP();
void risky_soloAWP();

void shame();


void lastBit();





void drive_test();
void turn_test();
void swing_test();
void full_test();
void odom_test();
void tank_odom_test();
void holonomic_odom_test();

void hell_naw();

void soloAWP();

//ELIMS:
void rightBlueElims();
void driveleftBlueElims();
void leftBlueElims();
void rightRedElims();
void leftRedElims();

void redLeftDiddlerElims();
void blueLeftDiddlerElims();

//QUALS:
void redLeftDiddler();
void redHook();
void blueLeftDiddler();
void blueHook();



void redRightSafeQuals();

//not done:
void redLeftSafeQuals();
void blueRightSafeQuals();
void blueLeftSafeQuals();

void leftRedElims();


void safeProgSkills();

void testHook();

void tuneTurnPID();
void tuneDrivePID();
void testRickyCool();

void rightDiddler();


void dragon();

void ball13();


void driverProgSkills();

