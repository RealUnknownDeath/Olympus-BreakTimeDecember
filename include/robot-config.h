using namespace vex;

extern brain Brain;

// VEXcode devices
extern motor LeftMotor1;
extern motor LeftMotor2;
extern motor LeftMotor3;
extern motor RightMotor1;
extern motor RightMotor2;
extern motor RightMotor3;
extern controller Controller1;
extern inertial Inertial;
extern motor intakeMotor;
extern motor middleTravel;
extern motor scoreMotor;
extern digital_out outtakeRaise;
extern optical Optical21;
extern digital_out descorePiston;
extern digital_out matchLoadPiston;
extern distance DistanceRight;
extern distance DistanceBack;
extern distance DistanceLeft;
extern distance DistanceFront;
extern line LineTracker;
extern gps GPS7;
extern digital_out Aligner;
extern rotation odom;
extern optical MatchloadSensor;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );