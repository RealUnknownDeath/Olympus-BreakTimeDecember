#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
motor LeftMotor1 = motor(PORT8, ratio6_1, true);
motor LeftMotor2 = motor(PORT9, ratio6_1, true);
motor LeftMotor3 = motor(PORT10, ratio6_1, true);
motor RightMotor1 = motor(PORT1, ratio6_1, false);
motor RightMotor2 = motor(PORT2, ratio6_1, false);
motor RightMotor3 = motor(PORT3, ratio6_1, false);
controller Controller1 = controller(primary);
inertial Inertial = inertial(PORT20);
motor intakeMotor = motor(PORT14, ratio18_1, false);
motor middleTravel = motor(PORT13, ratio18_1, false);
motor scoreMotor = motor(PORT15, ratio18_1, false);
digital_out outtakeRaise = digital_out(Brain.ThreeWirePort.C);
optical Optical21 = optical(PORT21);
digital_out descorePiston = digital_out(Brain.ThreeWirePort.G);
digital_out matchLoadPiston = digital_out(Brain.ThreeWirePort.F);
distance DistanceRight = distance(PORT5);
distance DistanceBack = distance(PORT19);
distance DistanceLeft = distance(PORT6);
distance DistanceFront = distance(PORT18);
line LineTracker = line(Brain.ThreeWirePort.B);
gps GPS7 = gps(PORT7, 165.10, -25.40, mm, 90);
digital_out Aligner = digital_out(Brain.ThreeWirePort.H);
rotation odom = rotation(PORT4, false);
optical MatchloadSensor = optical(PORT17);

// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}