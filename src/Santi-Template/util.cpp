#include "vex.h"
#include <iostream>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()


//Santi Utils:

//Finds the smallest signed angle (target-current) in [-180,180]
static inline double angleDiffDeg(double targetDeg, double currectDeg){
  double diff = std::fmod(targetDeg - currectDeg + 540, 360)-180;
  return diff;
}

//Correct skewed distance to perpendicular-to-wall distance
static inline double correctedPerpDistance(double measuredIn, double robotHeadingDeg, double sensorOffsetDeg, double wallNormalDeg){
  double beamHeadingDeg = robotHeadingDeg + sensorOffsetDeg;
  double delta = angleDiffDeg(beamHeadingDeg, wallNormalDeg);

  double c = std::cos(to_rad(delta));

  if(std::fabs(c) < 0.25){
    //reading is trash
    return 1e9;
  }

  return measuredIn * c;
}


//random_float made with help from AI:
int random_integer(int min, int max, float /*seed*/) {
    return min + (rand() % (max - min + 1));
}

float random_float(int min, int max, float seed){

  float first_digits = random_integer(min, max -1, seed);
  float second_digits = random_integer(0, 100, seed)/100;

  return first_digits + second_digits;

}
float distanceFrom_0(float angle){
  if (angle > 90){return std::abs(angle - 360);}
  return angle;
}

bool within_range(float value1, float value2, float range){
  if (value1 + range > value2 and value1 - range < value2){
    return true;
  }
  return false;
}

Vector2f rotate_vector(float x, float y, float heading_deg) {
  float rad = to_rad(heading_deg);
  return Vector2f(
    x * cos(rad) - y * sin(rad),
    x * sin(rad) + y * cos(rad)
  );
}

bool point_on_vertical_line(float x, float y, float x_line, float y_min, float y_max) {
    return std::fabs(x - x_line) < 1.5f && y >= y_min && y <= y_max;
}

bool point_on_horizontal_line(float x, float y, float y_line, float x_min, float x_max) {
    return std::fabs(y - y_line) < 1.5f && x >= x_min && x <= x_max;
}

/*
make this AI code into a circular mean function if needed:
float sum_sin = 0.0f;
float sum_cos = 0.0f;

for (const auto& p : particles) {
    float rad = p.heading_deg * 3.14159265359f / 180.0f;
    sum_sin += std::sin(rad);
    sum_cos += std::cos(rad);
}

float avg_heading_rad = std::atan2(sum_sin, sum_cos);
float avg_heading_deg = avg_heading_rad * 180.0f / 3.14159265359f;
if (avg_heading_deg < 0) avg_heading_deg += 360.0f;

I just got it as a bonus helper fact while doing other things, W chatGPT.
*/

//Jar Template utils:

/**
 * Converts an angle to an equivalent one in the range [0, 360).
 * 
 * @param angle The angle to be reduced in degrees.
 * @return Reduced angle.
 */

float reduce_0_to_360(float angle) {
  while(!(angle >= 0 && angle < 360)) {
    if( angle < 0 ) { angle += 360; }
    if(angle >= 360) { angle -= 360; }
  }
  return(angle);
}

/**
 * Converts an angle to an equivalent one in the range [-180, 180).
 * 
 * @param angle The angle to be reduced in degrees.
 * @return Reduced angle.
 */

float reduce_negative_180_to_180(float angle) {
  while(!(angle >= -180 && angle < 180)) {
    if( angle < -180 ) { angle += 360; }
    if(angle >= 180) { angle -= 360; }
  }
  return(angle);
}

/**
 * Converts an angle to an equivalent one in the range [-90, 90).
 * If the angle has no equivalent, then the angle halfway around
 * the circle is returned.
 * 
 * @param angle The angle to be reduced in degrees.
 * @return Reduced angle.
 */

float reduce_negative_90_to_90(float angle) {
  while(!(angle >= -90 && angle < 90)) {
    if( angle < -90 ) { angle += 180; }
    if(angle >= 90) { angle -= 180; }
  }
  return(angle);
}

/**
 * Converts an angle in degrees to radians.
 * 
 * @param angle The angle in degrees.
 * @return Angle in radians.
 */

float to_rad(float angle_deg){
  return(angle_deg/(180.0/M_PI));
}

/**
 * Converts an angle in radians to degrees.
 * 
 * @param angle The angle in radians.
 * @return Angle in degrees.
 */

float to_deg(float angle_rad){
  return(angle_rad*(180.0/M_PI));
}

/**
 * "Clamps" a number between a min and max.
 * Does no error checking and assumes min is less
 * than or equal to max.
 * 
 * @param input The number to be clamped.
 * @param min Minimum edge of the clamp.
 * @param max Maximum edge of the clamp.
 * @return Clamped number.
 */

float clamp(float input, float min, float max){
  if( input > max ){ return(max); }
  if(input < min){ return(min); }
  return(input);
}

/**
 * Checks if an input number corresponds to a reversed drive port.
 * 
 * @param input The number to be checked.
 * @return Whether the input is negative.
 */

bool is_reversed(double input){
  if(input<0) return(true);
  return(false);
}

/**
 * Scales a joystick to drive voltage scale.
 * Values get multiplied by 12 because motors can
 * output a max of 12 volts.
 * 
 * @param percent The input joystick reading.
 * @return The equivalent value in volts.
 */

double to_volt(double percent){
  return(percent*12.0/100.0);
}

/**
 * Converts an input port to a triport.
 * Does error checking to prevent invalid values from being
 * passed to the triport array.
 * 
 * @param port Input integer port.
 * @return Port-1, to convert it to a triport.
 */

int to_port(int port){
  if(port > 8 || port < 1){
    return(0);
  }
  return(port-1);
}

/**
 * Deadband function for joystick control.
 * If the joystick value is small, we should just consider it 0.
 * 
 * @param input The input joystick value.
 * @param width Minimum value to not get zeroed out.
 * @return The deadbanded value.
 */

float deadband(float input, float width){
  if (std::fabs(input)<width){
    return(0);
  }
  return(input);
}

/**
 * Settling control for odometry functions.
 * Draws a line perpendicular to the line from the robot to the desired 
 * endpoint, and checks if the robot has crossed that line. Allows for
 * very quick settling, and thereby chaining for fast motion control.
 * 
 * @param desired_X The ending X position in inches.
 * @param desired_Y The ending Y position in inches.
 * @param desired_angle_deg The direction of the line to be drawn.
 * @param current_X The robot's X position in inches.
 * @param current_Y The robot's Y position in inches.
 * @return Whether the robot can be considered settled.
 */

bool is_line_settled(float desired_X, float desired_Y, float desired_angle_deg, float current_X, float current_Y){
  return( (desired_Y-current_Y) * cos(to_rad(desired_angle_deg)) <= -(desired_X-current_X) * sin(to_rad(desired_angle_deg)) );
}

/**
 * Voltage scaling to keep from applying more than 12 volts to either side of the drive.
 * Divides both drive and heading output proportionally to get a similar result to the
 * desired one.
 * 
 * @param drive_output The forward output of the drive.
 * @param heading_output The angular output of the drive.
 * @return The scaled voltage for the left side of the robot.
 */

float left_voltage_scaling(float drive_output, float heading_output){
  float ratio = std::max(std::fabs(drive_output+heading_output), std::fabs(drive_output-heading_output))/12.0;
  if (ratio > 1) {
    return (drive_output+heading_output)/ratio;
  }
  return drive_output+heading_output;
}

/**
 * Voltage scaling to keep from applying more than 12 volts to either side of the drive.
 * Divides both drive and heading output proportionally to get a similar result to the
 * desired one.
 * 
 * @param drive_output The forward output of the drive.
 * @param heading_output The angular output of the drive.
 * @return The scaled voltage for the right side of the robot.
 */

float right_voltage_scaling(float drive_output, float heading_output){
  float ratio = std::max(std::fabs(drive_output+heading_output), std::fabs(drive_output-heading_output))/12.0;
  if (ratio > 1) {
    return (drive_output-heading_output)/ratio;
  }
  return drive_output-heading_output;
}

/**
 * Brings an output up to the minimum voltage if it's too slow.
 * Used for minimum voltage calculations for movement chaining.
 * Has no effect on 0 voltage output, because how do we know 
 * which way it's supposed to be going?
 * 
 * @param drive_output The forward output of the drive.
 * @param drive_min_voltage The minimum output of the drive.
 * @return The voltage with the minimum applied.
 */

float clamp_min_voltage(float drive_output, float drive_min_voltage){
  if(drive_output < 0 && drive_output > -drive_min_voltage){
      return -drive_min_voltage;
  }
  if(drive_output > 0 && drive_output < drive_min_voltage){
    return drive_min_voltage;
  }
  return drive_output;
}