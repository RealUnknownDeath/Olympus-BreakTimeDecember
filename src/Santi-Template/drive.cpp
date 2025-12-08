#include "vex.h"

static double dclamp(double v, double lo, double hi) {
  if (v < lo) return lo;
  if (v > hi) return hi;
  return v;
}


//Finds the smallest signed angle (target-current) in [-180,180]
static double angleDiffDeg(double targetDeg, double currectDeg){
  double diff = std::fmod(targetDeg - currectDeg + 540, 360)-180;
  return diff;
}

//Correct skewed distance to perpendicular-to-wall distance
static double correctedPerpDistance(double measuredIn, double robotHeadingDeg, double sensorOffsetDeg, double wallNormalDeg){
  double beamHeadingDeg = robotHeadingDeg + sensorOffsetDeg;
  double delta = angleDiffDeg(beamHeadingDeg, wallNormalDeg);

  double c = std::cos(to_rad(delta));

  if(std::fabs(c) < 0.25){
    //reading is trash
    return 1e9;
  }

  return measuredIn * c;
}

/**left_front_sensor_drive_distance
 * Drive constructor for the chassis.
 * Even though there's only one constructor, there can be
 * huge differences in implementation depending on the drive style
 * selected.
 * 
 * @param drive_setup The style of drive, such as TANK_TWO_ROTATION.
 * @param DriveL Left motor group.
 * @param DriveR Right motor group.
 * @param gyro_port IMU port.
 * @param wheel_diameter Wheel diameter in inches.
 * @param wheel_ratio External drive gear ratio.
 * @param gyro_scale Scale factor in degrees.
 * @param DriveLF_port Left front port for holonomic drives.
 * @param DriveRF_port Right front port for holonomic drives.
 * @param DriveLB_port Left back port for holonomic drives.
 * @param DriveRB_port Right back port for holonomic drives.
 * @param ForwardTracker_port Port for the forward tracker.
 * @param ForwardTracker_diameter Diameter in inches.
 * @param ForwardTracker_center_distance Horizontal distance in inches.
 * @param SidewaysTracker_port Port for the sideways tracker.
 * @param SidewaysTracker_diameter Diameter in inches.
 * @param SidewaysTracker_center_distance Vertical distance in inches.
 */

Drive::Drive(enum::drive_setup drive_setup, motor_group DriveL, motor_group DriveR, 
int gyro_port, float wheel_diameter, float wheel_ratio, float gyro_scale, 
int DriveLF_port, int DriveRF_port, int DriveLB_port, int DriveRB_port, 
int ForwardTracker_port, float ForwardTracker_diameter, float ForwardTracker_center_distance, 
int SidewaysTracker_port, float SidewaysTracker_diameter, float SidewaysTracker_center_distance) :
  wheel_diameter(wheel_diameter),
  wheel_ratio(wheel_ratio),
  gyro_scale(gyro_scale),
  drive_in_to_deg_ratio(wheel_ratio/360.0*M_PI*wheel_diameter),
  ForwardTracker_diameter(ForwardTracker_diameter),
  ForwardTracker_center_distance(ForwardTracker_center_distance),
  ForwardTracker_in_to_deg_ratio(M_PI*ForwardTracker_diameter/360.0),
  SidewaysTracker_center_distance(SidewaysTracker_center_distance),
  SidewaysTracker_diameter(SidewaysTracker_diameter),
  SidewaysTracker_in_to_deg_ratio(M_PI*SidewaysTracker_diameter/360.0),
  drive_setup(drive_setup),
  DriveL(DriveL),
  DriveR(DriveR),
  Gyro(inertial(gyro_port)),
  DriveLF(abs(DriveLF_port), is_reversed(DriveLF_port)),
  DriveRF(abs(DriveRF_port), is_reversed(DriveRF_port)),
  DriveLB(abs(DriveLB_port), is_reversed(DriveLB_port)),
  DriveRB(abs(DriveRB_port), is_reversed(DriveRB_port)),
  R_ForwardTracker(ForwardTracker_port),
  R_SidewaysTracker(SidewaysTracker_port),
  E_ForwardTracker(ThreeWire.Port[to_port(ForwardTracker_port)]),
  E_SidewaysTracker(ThreeWire.Port[to_port(SidewaysTracker_port)])
{
    if (drive_setup == TANK_ONE_FORWARD_ENCODER || drive_setup == TANK_ONE_FORWARD_ROTATION || drive_setup == ZERO_TRACKER_ODOM){
      odom.set_physical_distances(ForwardTracker_center_distance, 0);
    } 
    if (drive_setup == TANK_ONE_SIDEWAYS_ENCODER || drive_setup == TANK_ONE_SIDEWAYS_ROTATION || 
    drive_setup == TANK_TWO_ENCODER || drive_setup == TANK_TWO_ROTATION ||
    drive_setup == HOLONOMIC_TWO_ENCODER || drive_setup == HOLONOMIC_TWO_ROTATION){
      odom.set_physical_distances(ForwardTracker_center_distance, SidewaysTracker_center_distance);
    }
}

/**
 * Drives each side of the chassis at the specified voltage.
 * 
 * @param leftVoltage Voltage out of 12.
 * @param rightVoltage Voltage out of 12.
 */

void Drive::drive_with_voltage(float leftVoltage, float rightVoltage){
  DriveL.spin(fwd, leftVoltage, volt);
  DriveR.spin(fwd, rightVoltage,volt);
}

/**
 * Resets default turn constants.
 * Turning includes turn_to_angle() and turn_to_point().
 * 
 * @param turn_max_voltage Max voltage out of 12.
 * @param turn_kp Proportional constant.
 * @param turn_ki Integral constant.
 * @param turn_kd Derivative constant.
 * @param turn_starti Minimum angle in degrees for integral to begin.
 */

void Drive::set_turn_constants(float turn_max_voltage, float turn_kp, float turn_ki, float turn_kd, float turn_starti){
  this->turn_max_voltage = turn_max_voltage;
  this->turn_kp = turn_kp;
  this->turn_ki = turn_ki;
  this->turn_kd = turn_kd;
  this->turn_starti = turn_starti;
} 

/**
 * Resets default drive constants.
 * Driving includes drive_distance(), drive_to_point(), and
 * holonomic_drive_to_point().
 * 
 * @param drive_max_voltage Max voltage out of 12.
 * @param drive_kp Proportional constant.
 * @param drive_ki Integral constant.
 * @param drive_kd Derivative constant.
 * @param drive_starti Minimum distance in inches for integral to begin
 */

void Drive::set_drive_constants(float drive_max_voltage, float drive_kp, float drive_ki, float drive_kd, float drive_starti){
  this->drive_max_voltage = drive_max_voltage;
  this->drive_kp = drive_kp;
  this->drive_ki = drive_ki;
  this->drive_kd = drive_kd;
  this->drive_starti = drive_starti;
} 

/**
 * Resets default heading constants.
 * Heading control keeps the robot facing the right direction
 * and is part of drive_distance(), drive_to_point(), and
 * holonomic_drive_to_point.
 * 
 * @param heading_max_voltage Max voltage out of 12.
 * @param heading_kp Proportional constant.
 * @param heading_ki Integral constant.
 * @param heading_kd Derivative constant.
 * @param heading_starti Minimum angle in degrees for integral to begin.
 */

void Drive::set_heading_constants(float heading_max_voltage, float heading_kp, float heading_ki, float heading_kd, float heading_starti){
  this->heading_max_voltage = heading_max_voltage;
  this->heading_kp = heading_kp;
  this->heading_ki = heading_ki;
  this->heading_kd = heading_kd;
  this->heading_starti = heading_starti;
}

/**
 * Resets default swing constants.
 * Swing control holds one side of the drive still and turns with the other.
 * Only left_swing_to_angle() and right_swing_to_angle() use these constants.
 * 
 * @param swing_max_voltage Max voltage out of 12.
 * @param swing_kp Proportional constant.
 * @param swing_ki Integral constant.
 * @param swing_kd Derivative constant.
 * @param swing_starti Minimum angle in degrees for integral to begin.
 */

void Drive::set_swing_constants(float swing_max_voltage, float swing_kp, float swing_ki, float swing_kd, float swing_starti){
  this->swing_max_voltage = swing_max_voltage;
  this->swing_kp = swing_kp;
  this->swing_ki = swing_ki;
  this->swing_kd = swing_kd;
  this->swing_starti = swing_starti;
} 

/**
 * Resets default turn exit conditions.
 * The robot exits when error is less than settle_error for a duration of settle_time, 
 * or if the function has gone on for longer than timeout.
 * 
 * @param turn_settle_error Error to be considered settled in degrees.
 * @param turn_settle_time Time to be considered settled in milliseconds.
 * @param turn_timeout Time before quitting and move on in milliseconds.
 */

void Drive::set_turn_exit_conditions(float turn_settle_error, float turn_settle_time, float turn_timeout){
  this->turn_settle_error = turn_settle_error;
  this->turn_settle_time = turn_settle_time;
  this->turn_timeout = turn_timeout;
}

/**
 * Resets default drive exit conditions.
 * The robot exits when error is less than settle_error for a duration of settle_time, 
 * or if the function has gone on for longer than timeout.
 * 
 * @param drive_settle_error Error to be considered settled in inches.
 * @param drive_settle_time Time to be considered settled in milliseconds.
 * @param drive_timeout Time before quitting and move on in milliseconds.
 */

void Drive::set_drive_exit_conditions(float drive_settle_error, float drive_settle_time, float drive_timeout){
  this->drive_settle_error = drive_settle_error;
  this->drive_settle_time = drive_settle_time;
  this->drive_timeout = drive_timeout;
}

/**
 * Resets default swing exit conditions.
 * The robot exits when error is less than settle_error for a duration of settle_time, 
 * or if the function has gone on for longer than timeout.
 * 
 * @param swing_settle_error Error to be considered settled in degrees.
 * @param swing_settle_time Time to be considered settled in milliseconds.
 * @param swing_timeout Time before quitting and move on in milliseconds.
 */

void Drive::set_swing_exit_conditions(float swing_settle_error, float swing_settle_time, float swing_timeout){
  this->swing_settle_error = swing_settle_error;
  this->swing_settle_time = swing_settle_time;
  this->swing_timeout = swing_timeout;
}

/**
 * Gives the drive's absolute heading with Gyro correction.
 * 
 * @return Gyro scale-corrected heading in the range [0, 360).
 */

float Drive::get_absolute_heading(){ 
  return( reduce_0_to_360( Gyro.rotation()*360.0/gyro_scale ) ); 
}

/**
 * Gets the motor group's position and converts to inches.
 * 
 * @return Left position in inches.
 */

float Drive::get_left_position_in(){
  return( DriveL.position(deg)*drive_in_to_deg_ratio );
}

/**
 * Gets the motor group's position and converts to inches.
 * 
 * @return Right position in inches.
 */

float Drive::get_right_position_in(){
  return( DriveR.position(deg)*drive_in_to_deg_ratio );
}

/**
 * Stops both sides of the drive with the desired mode.
 * 
 * @param mode hold, brake, or stop
 */

void Drive::drive_stop(vex::brakeType mode){
  DriveL.stop(mode);
  DriveR.stop(mode);
}

/**
 * Turns the robot to a field-centric angle.
 * Optimizes direction, so it turns whichever way is closer to the 
 * current heading of the robot.
 * 
 * @param angle Desired angle in degrees.
 */

void Drive::turn_to_angle(float angle){
  turn_to_angle(angle, turn_max_voltage, turn_settle_error, turn_settle_time, turn_timeout, turn_kp, turn_ki, turn_kd, turn_starti);
}

void Drive::turn_to_angle(float angle, float turn_max_voltage){
  turn_to_angle(angle, turn_max_voltage, turn_settle_error, turn_settle_time, turn_timeout, turn_kp, turn_ki, turn_kd, turn_starti);
}

void Drive::turn_to_angle(float angle, float turn_max_voltage, float turn_settle_error, float turn_settle_time, float turn_timeout){
  turn_to_angle(angle, turn_max_voltage, turn_settle_error, turn_settle_time, turn_timeout, turn_kp, turn_ki, turn_kd, turn_starti);
}

void Drive::turn_to_angle(float angle, float turn_max_voltage, float turn_settle_error, float turn_settle_time, float turn_timeout, float turn_kp, float turn_ki, float turn_kd, float turn_starti){
  PID turnPID(reduce_negative_180_to_180(angle - get_absolute_heading()), turn_kp, turn_ki, turn_kd, turn_starti, turn_settle_error, turn_settle_time, turn_timeout);
  while( !turnPID.is_settled() ){
    float error = reduce_negative_180_to_180(angle - get_absolute_heading());
    float output = turnPID.compute(error);
    output = clamp(output, -turn_max_voltage, turn_max_voltage);
    drive_with_voltage(output, -output);
    task::sleep(10);
  }
}

/**
 * Drives the robot a given distance with a given heading.
 * Drive distance does not optimize for direction, so it won't try
 * to drive at the opposite heading from the one given to get there faster.
 * You can control the heading, but if you choose not to, it will drive with the
 * heading it's currently facing. It uses the average of the left and right
 * motor groups to calculate distance driven.
 * 
 * @param distance Desired distance in inches.
 * @param heading Desired heading in degrees.
 */

void Drive::drive_distance(float distance){
  drive_distance(distance, get_absolute_heading(), drive_max_voltage, heading_max_voltage, drive_settle_error, drive_settle_time, drive_timeout, drive_kp, drive_ki, drive_kd, drive_starti, heading_kp, heading_ki, heading_kd, heading_starti);
}

void Drive::drive_distance(float distance, float heading){
  drive_distance(distance, heading, drive_max_voltage, heading_max_voltage, drive_settle_error, drive_settle_time, drive_timeout, drive_kp, drive_ki, drive_kd, drive_starti, heading_kp, heading_ki, heading_kd, heading_starti);
}

void Drive::drive_distance(float distance, float heading, float drive_max_voltage, float heading_max_voltage){
  drive_distance(distance, heading, drive_max_voltage, heading_max_voltage, drive_settle_error, drive_settle_time, drive_timeout, drive_kp, drive_ki, drive_kd, drive_starti, heading_kp, heading_ki, heading_kd, heading_starti);
}

void Drive::drive_distance(float distance, float heading, float drive_max_voltage, float heading_max_voltage, float drive_settle_error, float drive_settle_time, float drive_timeout){
  drive_distance(distance, heading, drive_max_voltage, heading_max_voltage, drive_settle_error, drive_settle_time, drive_timeout, drive_kp, drive_ki, drive_kd, drive_starti, heading_kp, heading_ki, heading_kd, heading_starti);
}

void Drive::drive_distance(float distance, float heading, float drive_max_voltage, float heading_max_voltage, float drive_settle_error, float drive_settle_time, float drive_timeout, float drive_kp, float drive_ki, float drive_kd, float drive_starti, float heading_kp, float heading_ki, float heading_kd, float heading_starti){
  PID drivePID(distance, drive_kp, drive_ki, drive_kd, drive_starti, drive_settle_error, drive_settle_time, drive_timeout);
  PID headingPID(reduce_negative_180_to_180(heading - get_absolute_heading()), heading_kp, heading_ki, heading_kd, heading_starti);
  float start_average_position = (get_left_position_in()+get_right_position_in())/2.0;
  float average_position = start_average_position;
  while(drivePID.is_settled() == false){
    average_position = (get_left_position_in()+get_right_position_in())/2.0;
    float drive_error = distance+start_average_position-average_position;
    float heading_error = reduce_negative_180_to_180(heading - get_absolute_heading());
    float drive_output = drivePID.compute(drive_error);
    float heading_output = headingPID.compute(heading_error);

    drive_output = clamp(drive_output, -drive_max_voltage, drive_max_voltage);
    heading_output = clamp(heading_output, -heading_max_voltage, heading_max_voltage);

    drive_with_voltage(drive_output+heading_output, drive_output-heading_output);
    task::sleep(10);
  }
}

void Drive::drive_distance_sensor(float distance){
  drive_distance(distance, get_absolute_heading(), drive_max_voltage, heading_max_voltage, drive_settle_error, drive_settle_time, drive_timeout, drive_kp, drive_ki, drive_kd, drive_starti, heading_kp, heading_ki, heading_kd, heading_starti);
}

void Drive::drive_distance_sensor(float distance, float heading){
  drive_distance(distance, heading, drive_max_voltage, heading_max_voltage, drive_settle_error, drive_settle_time, drive_timeout, drive_kp, drive_ki, drive_kd, drive_starti, heading_kp, heading_ki, heading_kd, heading_starti);
}

void Drive::drive_distance_sensor(float distance, float heading, float drive_max_voltage, float heading_max_voltage){
  drive_distance(distance, heading, drive_max_voltage, heading_max_voltage, drive_settle_error, drive_settle_time, drive_timeout, drive_kp, drive_ki, drive_kd, drive_starti, heading_kp, heading_ki, heading_kd, heading_starti);
}

void Drive::drive_distance_sensor(float distance, float heading, float drive_max_voltage, float heading_max_voltage, float drive_settle_error, float drive_settle_time, float drive_timeout){
  drive_distance(distance, heading, drive_max_voltage, heading_max_voltage, drive_settle_error, drive_settle_time, drive_timeout, drive_kp, drive_ki, drive_kd, drive_starti, heading_kp, heading_ki, heading_kd, heading_starti);
}

void Drive::drive_distance_sensor(float distance, float heading, float drive_max_voltage, float heading_max_voltage, float drive_settle_error, float drive_settle_time, float drive_timeout, float drive_kp, float drive_ki, float drive_kd, float drive_starti, float heading_kp, float heading_ki, float heading_kd, float heading_starti){
  PID drivePID(distance, drive_kp, drive_ki, drive_kd, drive_starti, drive_settle_error, drive_settle_time, drive_timeout);
  PID headingPID(reduce_negative_180_to_180(heading - get_absolute_heading()), heading_kp, heading_ki, heading_kd, heading_starti);
  float start_average_position = 0;
  if(distance >=0){
    start_average_position = DistanceFront.objectDistance(inches);
  }
  else{
    start_average_position = DistanceBack.objectDistance(inches);
  }
  float average_position = start_average_position;
  while(drivePID.is_settled() == false){
    if(distance >=0){
      average_position = DistanceFront.objectDistance(inches);
    }
    else{
      average_position = DistanceBack.objectDistance(inches);
    }
    float drive_error = distance+start_average_position-average_position;
    float heading_error = reduce_negative_180_to_180(heading - get_absolute_heading());
    float drive_output = drivePID.compute(drive_error);
    float heading_output = headingPID.compute(heading_error);

    drive_output = clamp(drive_output, -drive_max_voltage, drive_max_voltage);
    heading_output = clamp(heading_output, -heading_max_voltage, heading_max_voltage);

    drive_with_voltage(drive_output+heading_output, drive_output-heading_output);
    task::sleep(10);
  }
}

/**
 * Turns to a given angle with only one side of the drivetrain.
 * Like turn_to_angle(), is optimized for turning the shorter
 * direction.
 * 
 * @param angle Desired angle in degrees.
 */

void Drive::left_swing_to_angle(float angle){
  left_swing_to_angle(angle, false, swing_max_voltage, swing_settle_error, swing_settle_time, swing_timeout, swing_kp, swing_ki, swing_kd, swing_starti);
}

void Drive::left_swing_to_angle(float angle, bool no_lock){
  left_swing_to_angle(angle, no_lock, swing_max_voltage, swing_settle_error, swing_settle_time, swing_timeout, swing_kp, swing_ki, swing_kd, swing_starti);
}

void Drive::left_swing_to_angle(float angle, bool no_lock, float swing_max_voltage, float swing_settle_error, float swing_settle_time, float swing_timeout, float swing_kp, float swing_ki, float swing_kd, float swing_starti){
  PID swingPID(reduce_negative_180_to_180(angle - get_absolute_heading()), swing_kp, swing_ki, swing_kd, swing_starti, swing_settle_error, swing_settle_time, swing_timeout);
  while(swingPID.is_settled() == false){
    float error = reduce_negative_180_to_180(angle - get_absolute_heading());
    float output = swingPID.compute(error);
    output = clamp(output, -swing_max_voltage, swing_max_voltage);
    DriveL.spin(fwd, output, volt);
    if(no_lock){
      DriveR.setVelocity(0, percent);
      DriveR.spin(forward);
    }
    else{
      DriveR.stop(hold);
    }

    task::sleep(10);
  }
}

void Drive::goal_swing_to_angle(float angle){
  float currentAngle = chassis.get_absolute_heading();
  float error = angle-currentAngle;

  if(error < 0){
    //spin left reverse fully with just the slighest bit of right going forward

    //after we pass i-gate we can use the right side to go a little backwards to finish the alignement
    bool finalize = false;

    PID swingPID(reduce_negative_180_to_180(angle - get_absolute_heading()), swing_kp, swing_ki, swing_kd, swing_starti, swing_settle_error, swing_settle_time, swing_timeout);
    while(swingPID.is_settled() == false){
      float error = reduce_negative_180_to_180(angle - get_absolute_heading());
      float output = swingPID.compute(error);
      output = clamp(output, -swing_max_voltage, swing_max_voltage);
      DriveL.spin(fwd, output, volt);

      if(error <= swingPID.starti){
        finalize = true;
      }
      if(!finalize){
        DriveR.spin(reverse, output * 0.1, volt);
      }
      else{
        DriveR.spin(fwd, output * 0.1, volt);
      }
      

      task::sleep(10);
    }
    
  }

  else if(error > 0){
    //spin right reverse fully with just the slighest bit of left going forward

    //after we pass i-gate we can use the right side to go a right backwards to finish the alignement

    bool finalize = false;

    PID swingPID(reduce_negative_180_to_180(angle - get_absolute_heading()), swing_kp, swing_ki, swing_kd, swing_starti, swing_settle_error, swing_settle_time, swing_timeout);
    while(swingPID.is_settled() == false){
      float error = reduce_negative_180_to_180(angle - get_absolute_heading());
      float output = swingPID.compute(error);
      output = clamp(output, -swing_max_voltage, swing_max_voltage);
      DriveR.spin(reverse, output, volt);

      if(error <= swingPID.starti){
        finalize = true;
      }
      if(!finalize){
        DriveL.spin(fwd, output * 0.1, volt);
      }
      else{
        DriveL.spin(reverse, output * 0.1, volt);
      }
      

      task::sleep(10);
    }
  }

}

void Drive::right_swing_to_angle(float angle){
  right_swing_to_angle(angle, false, swing_max_voltage, swing_settle_error, swing_settle_time, swing_timeout, swing_kp, swing_ki, swing_kd, swing_starti);
}

void Drive::right_swing_to_angle(float angle, bool no_lock){
  right_swing_to_angle(angle, no_lock, swing_max_voltage, swing_settle_error, swing_settle_time, swing_timeout, swing_kp, swing_ki, swing_kd, swing_starti);
}

void Drive::right_swing_to_angle(float angle, bool no_lock, float swing_max_voltage, float swing_settle_error, float swing_settle_time, float swing_timeout, float swing_kp, float swing_ki, float swing_kd, float swing_starti){
  PID swingPID(reduce_negative_180_to_180(angle - get_absolute_heading()), swing_kp, swing_ki, swing_kd, swing_starti, swing_settle_error, swing_settle_time, swing_timeout);
  while(swingPID.is_settled() == false){
    float error = reduce_negative_180_to_180(angle - get_absolute_heading());
    float output = swingPID.compute(error);
    output = clamp(output, -swing_max_voltage, swing_max_voltage);
    DriveR.spin(reverse, output, volt);
    if(no_lock){
      DriveL.setVelocity(0, percent);
      DriveL.spin(forward);
    }
    else{
      DriveL.stop(hold);
    }
    task::sleep(10);
  }
}

void Drive::left_hook(float angle, double ratio){
  left_hook(angle, ratio, swing_max_voltage, swing_settle_error, swing_settle_time, swing_timeout, swing_kp, swing_ki, swing_kd, swing_starti);
}

void Drive::left_hook(float angle, double ratio, float swing_max_voltage, float swing_settle_error, float swing_settle_time, float swing_timeout, float swing_kp, float swing_ki, float swing_kd, float swing_starti){
  PID swingPID(reduce_negative_180_to_180(angle - get_absolute_heading()), swing_kp, swing_ki, swing_kd, swing_starti, swing_settle_error, swing_settle_time, swing_timeout);
  while(swingPID.is_settled() == false){
    //float error = reduce_negative_180_to_180((angle - get_absolute_heading())*(2/(1-ratio)));
    float error = reduce_negative_180_to_180((angle - get_absolute_heading()));
    float output = swingPID.compute(error);
    output = clamp(output, -swing_max_voltage, swing_max_voltage);
    DriveR.spin(reverse, output, volt);
    DriveL.spin(reverse, (output)*ratio, volt);

    task::sleep(10);
  }
}

void Drive::right_hook(float angle, double ratio){
  right_hook(angle, ratio, swing_max_voltage, swing_settle_error, swing_settle_time, swing_timeout, swing_kp, swing_ki, swing_kd, swing_starti);
}

void Drive::right_hook(float angle, double ratio, float swing_max_voltage, float swing_settle_error, float swing_settle_time, float swing_timeout, float swing_kp, float swing_ki, float swing_kd, float swing_starti){
  PID swingPID(reduce_negative_180_to_180(angle - get_absolute_heading()), swing_kp, swing_ki, swing_kd, swing_starti, swing_settle_error, swing_settle_time, swing_timeout);
  while(swingPID.is_settled() == false){
    //float error = reduce_negative_180_to_180((angle - get_absolute_heading())*(2/(1-ratio)));
    float error = reduce_negative_180_to_180((angle - get_absolute_heading()));
    float output = swingPID.compute(error);
    output = clamp(output, -swing_max_voltage, swing_max_voltage);
    DriveL.spin(fwd, output, volt);
    DriveR.spin(fwd, (output)*ratio, volt);

    task::sleep(10);
  }
}



void Drive::left_front_sensor_drive_distance(double front_target,
                                             double left_target,
                                             double heading_target) {
  left_front_sensor_drive_distance(front_target, left_target, heading_target,
                                   ml_front_sensor_offset,
                                   ml_left_sensor_offset,
                                   ml_align_phase_end,
                                   ml_slow_down_start,
                                   ml_slow_down_factor,
                                   ml_kF,
                                   ml_kAlign,
                                   ml_max_lean,
                                   ml_max_forward_voltage,
                                   ml_heading_kP,
                                   ml_heading_kI,
                                   ml_heading_kD,
                                   ml_front_tolerance,
                                   ml_left_tolerance,
                                   ml_heading_tolerance,
                                   ml_timeout,
                                   ml_loopDt,
                                   ml_hold_time_ms);
}

void Drive::left_front_sensor_drive_distance(double front_target,
                                             double left_target,
                                             double heading_target,
                                             double front_sensor_offset,
                                             double left_sensor_offset,
                                             double align_phase_end,
                                             double slow_down_start,
                                             double slow_down_factor,
                                             double kF,
                                             double kAlign,
                                             double max_lean,
                                             double max_forward_voltage,
                                             double heading_kP,
                                             double heading_kI,
                                             double heading_kD,
                                             double front_tolerance,
                                             double left_tolerance,
                                             double heading_tolerance,
                                             double timeout,
                                             int    loopDt,
                                             double hold_time_ms) {
  // Wall normals: where beams point when squared at target
  double front_wall_normal = heading_target + front_sensor_offset;
  double left_wall_normal  = heading_target + left_sensor_offset;

  // Helper for consistent finish
  auto finish_and_hold = [&](void) {
    drive_stop(vex::brakeType::hold);
    vex::task::sleep((int)hold_time_ms);
  };

  // --- starting front distance (corrected) for progress ---
  double heading_start   = get_absolute_heading();

  double rawFrontStartIn = DistanceFront.objectDistance(vex::distanceUnits::in);
  double dF_start        = correctedPerpDistance(rawFrontStartIn,
                                                 heading_start,
                                                 front_sensor_offset,
                                                 front_wall_normal);

  double totalTravel = dF_start - front_target;

  //+1 = going forward, -1 = going backward
  double dirSign = (totalTravel >= 0.0) ? 1.0 : -1.0;

  //If already basically at front distance, just clean heading and hold.
  if (fabs(totalTravel) < 1.0) {
    PID headingPID(angleDiffDeg(heading_target, get_absolute_heading()),
                   heading_kP, heading_kI, heading_kD, heading_starti);
    while (true) {
      double theta = get_absolute_heading();
      double hErr  = angleDiffDeg(heading_target, theta);
      if (fabs(hErr) < heading_tolerance) {
        finish_and_hold();
        break;
      }
      double turn = headingPID.compute(hErr);
      drive_with_voltage(-turn, +turn);
      vex::task::sleep(loopDt);
    }
    return;
  }

  PID headingPID(0.0, heading_kP, heading_kI, heading_kD, heading_starti);

  double startTimeMs = vex::timer::system();

  while (true) {
    double nowMs = vex::timer::system();
    if (nowMs - startTimeMs > timeout) {
      // Safety timeout
      finish_and_hold();
      break;
    }

    if(!(within_range(Controller1.Axis2.position(),0,5) || within_range(Controller1.Axis3.position(),0,5))){
        break;
    }

    

    // --- Sensor reads ---
    double rawFrontIn = DistanceFront.objectDistance(vex::distanceUnits::in);
    double rawLeftIn  = DistanceLeft.objectDistance(vex::distanceUnits::in);
    double theta      = get_absolute_heading();

    // --- Correct to perpendicular distances ---
    double dF = correctedPerpDistance(rawFrontIn, theta,
                                      front_sensor_offset, front_wall_normal);
    double dL = correctedPerpDistance(rawLeftIn,  theta,
                                      left_sensor_offset,  left_wall_normal);

    // --- Errors ---
    double eF    = dF - front_target;
    double eL    = dL - left_target;
    double hErr0 = angleDiffDeg(heading_target, theta);

    // --- Stop condition: inside tolerance box ---
    if (fabs(eF) < front_tolerance &&
        fabs(eL) < left_tolerance &&
        fabs(hErr0) < heading_tolerance) {
      finish_and_hold();
      break;
    }

    // --- Progress based on front distance (0..1) ---
    double progress = (dF_start - dF) / totalTravel;
    if (progress < 0.0) progress = 0.0;
    if (progress > 1.0) progress = 1.0;

    // --- Heading bias for Phase 1 ---
    double biasDeg = 0.0;
    if (progress < align_phase_end) {
      double phaseFrac = progress / align_phase_end; // 0..1
      double taper     = 1.0 - phaseFrac;            // 1..0
      // For left wall: forward => -kAlign*eL, backward => flipped by dirSign
      biasDeg = -kAlign * eL * taper * dirSign;
      biasDeg = dclamp(biasDeg, -max_lean, max_lean);
    }
    // After align_phase_end, biasDeg = 0 → pure heading_target

    double desiredHeading = heading_target + biasDeg;

    // --- Heading PID ---
    double hErr = angleDiffDeg(desiredHeading, theta);
    double turn = headingPID.compute(hErr);

    // --- Forward from front error ---
    double forward = kF * eF;
    forward = dclamp(forward, -max_forward_voltage, max_forward_voltage);

    if (progress > slow_down_start) {
      forward *= slow_down_factor;
    }

    // If close in front but not in heading, favor turning
    if (fabs(eF) < 0.5 && fabs(hErr) > heading_tolerance) {
      forward *= 0.4;
    }

    // --- Tank outputs ---
    double leftV  = forward + turn;
    double rightV = forward - turn;
    leftV  = dclamp(leftV,  -12.0, 12.0);
    rightV = dclamp(rightV, -12.0, 12.0);

    drive_with_voltage(leftV, rightV);
    vex::task::sleep(loopDt);
  }
}

// ================== RIGHT WALL VERSION ==================

void Drive::right_front_sensor_drive_distance(double front_target,
                                              double right_target,
                                              double heading_target) {
  right_front_sensor_drive_distance(front_target, right_target, heading_target,
                                    ml_front_sensor_offset,
                                    ml_right_sensor_offset,
                                    ml_align_phase_end,
                                    ml_slow_down_start,
                                    ml_slow_down_factor,
                                    ml_kF,
                                    ml_kAlign,
                                    ml_max_lean,
                                    ml_max_forward_voltage,
                                    ml_heading_kP,
                                    ml_heading_kI,
                                    ml_heading_kD,
                                    ml_front_tolerance,
                                    ml_right_tolerance,
                                    ml_heading_tolerance,
                                    ml_timeout,
                                    ml_loopDt,
                                    ml_hold_time_ms);
}

void Drive::right_front_sensor_drive_distance(double front_target,
                                              double right_target,
                                              double heading_target,
                                              double front_sensor_offset,
                                              double right_sensor_offset,
                                              double align_phase_end,
                                              double slow_down_start,
                                              double slow_down_factor,
                                              double kF,
                                              double kAlign,
                                              double max_lean,
                                              double max_forward_voltage,
                                              double heading_kP,
                                              double heading_kI,
                                              double heading_kD,
                                              double front_tolerance,
                                              double right_tolerance,
                                              double heading_tolerance,
                                              double timeout,
                                              int    loopDt,
                                              double hold_time_ms) {
  // Wall normals: where beams point when squared at target
  double front_wall_normal = heading_target + front_sensor_offset;
  double right_wall_normal = heading_target + right_sensor_offset;

  // Helper for consistent finish
  auto finish_and_hold = [&](void) {
    drive_stop(vex::brakeType::hold);
    vex::task::sleep((int)hold_time_ms);
  };

  // --- starting front distance (corrected) for progress ---
  double heading_start   = get_absolute_heading();

  double rawFrontStartIn = DistanceFront.objectDistance(vex::distanceUnits::in);
  double dF_start        = correctedPerpDistance(rawFrontStartIn,
                                                 heading_start,
                                                 front_sensor_offset,
                                                 front_wall_normal);

  double totalTravel = dF_start - front_target;

  // +1 = going forward, -1 = going backward
  double dirSign = (totalTravel >= 0.0) ? 1.0 : -1.0;

  // If already basically at front distance, just clean heading and hold.
  if (fabs(totalTravel) < 1.0) {
    PID headingPID(angleDiffDeg(heading_target, get_absolute_heading()),
                   heading_kP, heading_kI, heading_kD, heading_starti);
    while (true) {
      double theta = get_absolute_heading();
      double hErr  = angleDiffDeg(heading_target, theta);
      if (fabs(hErr) < heading_tolerance) {
        finish_and_hold();
        break;
      }
      double turn = headingPID.compute(hErr);
      drive_with_voltage(-turn, +turn);
      vex::task::sleep(loopDt);
    }
    return;
  }

  PID headingPID(0.0, heading_kP, heading_kI, heading_kD, heading_starti);

  double startTimeMs = vex::timer::system();

  while (true) {
    double nowMs = vex::timer::system();
    if (nowMs - startTimeMs > timeout) {
      // Safety timeout
      finish_and_hold();
      break;
    }

    // --- Sensor reads ---
    double rawFrontIn = DistanceFront.objectDistance(vex::distanceUnits::in);
    double rawRightIn = DistanceRight.objectDistance(vex::distanceUnits::in);
    double theta      = get_absolute_heading();

    // --- Correct to perpendicular distances ---
    double dF = correctedPerpDistance(rawFrontIn, theta,
                                      front_sensor_offset, front_wall_normal);
    double dR = correctedPerpDistance(rawRightIn, theta,
                                      right_sensor_offset, right_wall_normal);

    // --- Errors ---
    double eF    = dF - front_target;
    double eR    = dR - right_target;          // distance to RIGHT wall error
    double hErr0 = angleDiffDeg(heading_target, theta);

    // --- Stop condition: inside tolerance box ---
    if (fabs(eF) < front_tolerance &&
        fabs(eR) < right_tolerance &&
        fabs(hErr0) < heading_tolerance) {
      finish_and_hold();
      break;
    }

    // --- Progress based on front distance (0..1) ---
    double progress = (dF_start - dF) / totalTravel;
    if (progress < 0.0) progress = 0.0;
    if (progress > 1.0) progress = 1.0;

    // --- Heading bias for Phase 1 ---
    double biasDeg = 0.0;
    if (progress < align_phase_end) {
      double phaseFrac = progress / align_phase_end; // 0..1
      double taper     = 1.0 - phaseFrac;            // 1..0
      // For right wall: forward => +kAlign*eR, backward => flipped by dirSign
      biasDeg =  kAlign * eR * taper * dirSign;
      biasDeg = dclamp(biasDeg, -max_lean, max_lean);
    }
    // After align_phase_end, biasDeg = 0 → pure heading_target

    double desiredHeading = heading_target + biasDeg;

    // --- Heading PID ---
    double hErr = angleDiffDeg(desiredHeading, theta);
    double turn = headingPID.compute(hErr);

    // --- Forward from front error ---
    double forward = kF * eF;
    forward = dclamp(forward, -max_forward_voltage, max_forward_voltage);

    if (progress > slow_down_start) {
      forward *= slow_down_factor;
    }

    // If close in front but not in heading, favor turning
    if (fabs(eF) < 0.5 && fabs(hErr) > heading_tolerance) {
      forward *= 0.4;
    }

    // --- Tank outputs ---
    double leftV  = forward + turn;
    double rightV = forward - turn;
    leftV  = dclamp(leftV,  -12.0, 12.0);
    rightV = dclamp(rightV, -12.0, 12.0);

    drive_with_voltage(leftV, rightV);
    vex::task::sleep(loopDt);
  }
}




/**
 * Depending on the drive style, gets the tracker's position.
 * 
 * @return The tracker position.
 */

float Drive::get_ForwardTracker_position(){
  if (drive_setup==ZERO_TRACKER_ODOM || drive_setup == TANK_ONE_SIDEWAYS_ENCODER || drive_setup == TANK_ONE_SIDEWAYS_ROTATION){
    return(get_right_position_in());
  }
  if (drive_setup==TANK_ONE_FORWARD_ENCODER || drive_setup == TANK_TWO_ENCODER || drive_setup == HOLONOMIC_TWO_ENCODER){
    return(E_ForwardTracker.position(deg)*ForwardTracker_in_to_deg_ratio);
  }else{
    return(R_ForwardTracker.position(deg)*ForwardTracker_in_to_deg_ratio);
  }
}

/**
 * Depending on the drive style, gets the tracker's position.
 * 
 * @return The tracker position.
 */

float Drive::get_SidewaysTracker_position(){
  if (drive_setup==TANK_ONE_FORWARD_ENCODER || drive_setup == TANK_ONE_FORWARD_ROTATION || drive_setup == ZERO_TRACKER_ODOM){
    return(0);
  }else if (drive_setup == TANK_TWO_ENCODER || drive_setup == HOLONOMIC_TWO_ENCODER || drive_setup == TANK_ONE_SIDEWAYS_ENCODER){
    return(E_SidewaysTracker.position(deg)*SidewaysTracker_in_to_deg_ratio);
  }else{
    return(R_SidewaysTracker.position(deg)*SidewaysTracker_in_to_deg_ratio);
  }
}




/**
 * Background task for updating the odometry.
*/


void Drive::position_track(){
  while(1){
    odom.update_position(get_ForwardTracker_position(), get_SidewaysTracker_position(), get_absolute_heading());
    task::sleep(5);
  }
}

/**
 * Resets the robot's heading.
 * For example, at the beginning of auton, if your robot starts at
 * 45 degrees, so set_heading(45) and the robot will know which way 
 * it's facing.
 * 
 * @param orientation_deg Desired heading in degrees.
 */

void Drive::set_heading(float orientation_deg){
  Gyro.setRotation(orientation_deg*gyro_scale/360.0, deg);
}

/**
 * Resets the robot's coordinates and heading.
 * This is for odom-using robots to specify where the bot is at the beginning
 * of the match.
 * 
 * @param X_position Robot's x in inches.
 * @param Y_position Robot's y in inches.
 * @param orientation_deg Desired heading in degrees.
 */

void Drive::set_coordinates(float X_position, float Y_position, float orientation_deg){
  odom.set_position(X_position, Y_position, orientation_deg, get_ForwardTracker_position(), get_SidewaysTracker_position());
  set_heading(orientation_deg);
  odom_task = task(position_track_task);
}

/**
 * Gets the robot's x.
 * 
 * @return The robot's x position in inches.
 */

float Drive::get_X_position(){
  return(odom.X_position);
}

/**
 * Gets the robot's y.
 * 
 * @return The robot's y position in inches.
 */

float Drive::get_Y_position(){
  return(odom.Y_position);
}

/**
 * Drives to a specified point on the field.
 * Uses the double-PID method, with one for driving and one for heading correction.
 * The drive error is the euclidean distance to the desired point, and the heading error
 * is the turn correction from the current heading to the desired point. Uses optimizations
 * like driving backwards whenever possible and scaling the drive output with the cosine
 * of the angle to the point.
 * 
 * @param X_position Desired x position in inches.
 * @param Y_position Desired y position in inches.
 */

void Drive::drive_to_point(float X_position, float Y_position){
  drive_to_point(X_position, Y_position, drive_min_voltage, drive_max_voltage, heading_max_voltage, drive_settle_error, drive_settle_time, drive_timeout, drive_kp, drive_ki, drive_kd, drive_starti, heading_kp, heading_ki, heading_kd, heading_starti);
}

void Drive::drive_to_point(float X_position, float Y_position, float drive_min_voltage, float drive_max_voltage, float heading_max_voltage){
  drive_to_point(X_position, Y_position, drive_min_voltage, drive_max_voltage, heading_max_voltage, drive_settle_error, drive_settle_time, drive_timeout, drive_kp, drive_ki, drive_kd, drive_starti, heading_kp, heading_ki, heading_kd, heading_starti);
}

void Drive::drive_to_point(float X_position, float Y_position, float drive_min_voltage, float drive_max_voltage, float heading_max_voltage, float drive_settle_error, float drive_settle_time, float drive_timeout){
  drive_to_point(X_position, Y_position, drive_min_voltage, drive_max_voltage, heading_max_voltage, drive_settle_error, drive_settle_time, drive_timeout, drive_kp, drive_ki, drive_kd, drive_starti, heading_kp, heading_ki, heading_kd, heading_starti);
}

void Drive::drive_to_point(float X_position, float Y_position, float drive_min_voltage, float drive_max_voltage, float heading_max_voltage, float drive_settle_error, float drive_settle_time, float drive_timeout, float drive_kp, float drive_ki, float drive_kd, float drive_starti, float heading_kp, float heading_ki, float heading_kd, float heading_starti){
  PID drivePID(hypot(X_position-get_X_position(),Y_position-get_Y_position()), drive_kp, drive_ki, drive_kd, drive_starti, drive_settle_error, drive_settle_time, drive_timeout);
  float start_angle_deg = to_deg(atan2(X_position-get_X_position(),Y_position-get_Y_position()));
  PID headingPID(start_angle_deg-get_absolute_heading(), heading_kp, heading_ki, heading_kd, heading_starti);
  bool line_settled = false;
  bool prev_line_settled = is_line_settled(X_position, Y_position, start_angle_deg, get_X_position(), get_Y_position());
  while(!drivePID.is_settled()){
    line_settled = is_line_settled(X_position, Y_position, start_angle_deg, get_X_position(), get_Y_position());
    if(line_settled && !prev_line_settled){ break; }
    prev_line_settled = line_settled;

    float drive_error = hypot(X_position-get_X_position(),Y_position-get_Y_position());
    float heading_error = reduce_negative_180_to_180(to_deg(atan2(X_position-get_X_position(),Y_position-get_Y_position()))-get_absolute_heading());
    float drive_output = drivePID.compute(drive_error);

    float heading_scale_factor = cos(to_rad(heading_error));
    drive_output*=heading_scale_factor;
    heading_error = reduce_negative_90_to_90(heading_error);
    float heading_output = headingPID.compute(heading_error);
    
    if (drive_error<drive_settle_error) { heading_output = 0; }

    drive_output = clamp(drive_output, -fabs(heading_scale_factor)*drive_max_voltage, fabs(heading_scale_factor)*drive_max_voltage);
    heading_output = clamp(heading_output, -heading_max_voltage, heading_max_voltage);

    drive_output = clamp_min_voltage(drive_output, drive_min_voltage);

    drive_with_voltage(left_voltage_scaling(drive_output, heading_output), right_voltage_scaling(drive_output, heading_output));
    task::sleep(10);
  }
}



/**
 * Drives to a specified point and orientation on the field.
 * Uses a boomerang controller. The carrot point is back from the target
 * by the same distance as the robot's distance to the target, times the lead. The
 * robot always tries to go to the carrot, which is constantly moving, and the
 * robot eventually gets into position. The heading correction is optimized to only
 * try to reach the correct angle when drive error is low, and the robot will drive 
 * backwards to reach a pose if it's faster. .5 is a reasonable value for the lead. 
 * The setback parameter is used to glide into position more effectively. It is
 * the distance back from the target that the robot tries to drive to first.
 * 
 * @param X_position Desired x position in inches.
 * @param Y_position Desired y position in inches.
 * @param angle Desired orientation in degrees.
 * @param lead Constant scale factor that determines how far away the carrot point is. 
 * @param setback Distance in inches from target by which the carrot is always pushed back.
 * @param drive_min_voltage Minimum voltage on the drive, used for chaining movements.
 */

void Drive::drive_to_pose(float X_position, float Y_position, float angle){
  drive_to_pose(X_position, Y_position, angle, boomerang_lead, boomerang_setback, drive_min_voltage, drive_max_voltage, heading_max_voltage, drive_settle_error, drive_settle_time, drive_timeout, drive_kp, drive_ki, drive_kd, drive_starti, heading_kp, heading_ki, heading_kd, heading_starti);
}

void Drive::drive_to_pose(float X_position, float Y_position, float angle, float lead, float setback, float drive_min_voltage){
  drive_to_pose(X_position, Y_position, angle, lead, setback, drive_min_voltage, drive_max_voltage, heading_max_voltage, drive_settle_error, drive_settle_time, drive_timeout, drive_kp, drive_ki, drive_kd, drive_starti, heading_kp, heading_ki, heading_kd, heading_starti);
}

void Drive::drive_to_pose(float X_position, float Y_position, float angle, float lead, float setback, float drive_min_voltage, float drive_max_voltage, float heading_max_voltage){
  drive_to_pose(X_position, Y_position, angle, lead, setback, drive_min_voltage, drive_max_voltage, heading_max_voltage, drive_settle_error, drive_settle_time, drive_timeout, drive_kp, drive_ki, drive_kd, drive_starti, heading_kp, heading_ki, heading_kd, heading_starti);
}


void Drive::drive_to_pose(float X_position, float Y_position, float angle, float lead, float setback, float drive_min_voltage, float drive_max_voltage, float heading_max_voltage, float drive_settle_error, float drive_settle_time, float drive_timeout){
  drive_to_pose(X_position, Y_position, angle, lead, setback, drive_min_voltage, drive_max_voltage, heading_max_voltage, drive_settle_error, drive_settle_time, drive_timeout, drive_kp, drive_ki, drive_kd, drive_starti, heading_kp, heading_ki, heading_kd, heading_starti);
}

void Drive::drive_to_pose(float X_position, float Y_position, float angle, float lead, float setback, float drive_min_voltage, float drive_max_voltage, float heading_max_voltage, float drive_settle_error, float drive_settle_time, float drive_timeout, float drive_kp, float drive_ki, float drive_kd, float drive_starti, float heading_kp, float heading_ki, float heading_kd, float heading_starti){
  float target_distance = hypot(X_position-get_X_position(),Y_position-get_Y_position());
  PID drivePID(target_distance, drive_kp, drive_ki, drive_kd, drive_starti, drive_settle_error, drive_settle_time, drive_timeout);
  PID headingPID(to_deg(atan2(X_position-get_X_position(),Y_position-get_Y_position()))-get_absolute_heading(), heading_kp, heading_ki, heading_kd, heading_starti);
  bool line_settled = is_line_settled(X_position, Y_position, angle, get_X_position(), get_Y_position());
  bool prev_line_settled = is_line_settled(X_position, Y_position, angle, get_X_position(), get_Y_position());
  bool crossed_center_line = false;
  bool center_line_side = is_line_settled(X_position, Y_position, angle+90, get_X_position(), get_Y_position());
  bool prev_center_line_side = center_line_side;
  while(!drivePID.is_settled()){
    line_settled = is_line_settled(X_position, Y_position, angle, get_X_position(), get_Y_position());
    if(line_settled && !prev_line_settled){ break; }
    prev_line_settled = line_settled;

    center_line_side = is_line_settled(X_position, Y_position, angle+90, get_X_position(), get_Y_position());
    if(center_line_side != prev_center_line_side){
      crossed_center_line = true;
    }

    target_distance = hypot(X_position-get_X_position(),Y_position-get_Y_position());

    float carrot_X = X_position - sin(to_rad(angle)) * (lead * target_distance + setback);
    float carrot_Y = Y_position - cos(to_rad(angle)) * (lead * target_distance + setback);

    float drive_error = hypot(carrot_X-get_X_position(),carrot_Y-get_Y_position());
    float heading_error = reduce_negative_180_to_180(to_deg(atan2(carrot_X-get_X_position(),carrot_Y-get_Y_position()))-get_absolute_heading());

    if (drive_error<drive_settle_error || crossed_center_line || drive_error < setback) { 
      heading_error = reduce_negative_180_to_180(angle-get_absolute_heading()); 
      drive_error = target_distance;
    }
    
    float drive_output = drivePID.compute(drive_error);

    float heading_scale_factor = cos(to_rad(heading_error));
    drive_output*=heading_scale_factor;
    heading_error = reduce_negative_90_to_90(heading_error);
    float heading_output = headingPID.compute(heading_error);

    drive_output = clamp(drive_output, -fabs(heading_scale_factor)*drive_max_voltage, fabs(heading_scale_factor)*drive_max_voltage);
    heading_output = clamp(heading_output, -heading_max_voltage, heading_max_voltage);

    drive_output = clamp_min_voltage(drive_output, drive_min_voltage);

    drive_with_voltage(left_voltage_scaling(drive_output, heading_output), right_voltage_scaling(drive_output, heading_output));
    task::sleep(10);
  }
}

/**
 * Turns to a specified point on the field.
 * Functions similarly to turn_to_angle() except with a point. The
 * extra_angle_deg parameter turns the robot extra relative to the 
 * desired target. For example, if you want the back of your robot
 * to point at (36, 42), you would run turn_to_point(36, 42, 180).
 * 
 * @param X_position Desired x position in inches.
 * @param Y_position Desired y position in inches.
 * @param extra_angle_deg Angle turned past the desired heading in degrees.
 */

void Drive::turn_to_point(float X_position, float Y_position){
  turn_to_point(X_position, Y_position, 0, turn_max_voltage, turn_settle_error, turn_settle_time, turn_timeout, turn_kp, turn_ki, turn_kd, turn_starti);
}

void Drive::turn_to_point(float X_position, float Y_position, float extra_angle_deg){
  turn_to_point(X_position, Y_position, extra_angle_deg, turn_max_voltage, turn_settle_error, turn_settle_time, turn_timeout, turn_kp, turn_ki, turn_kd, turn_starti);
}

void Drive::turn_to_point(float X_position, float Y_position, float extra_angle_deg, float turn_max_voltage, float turn_settle_error, float turn_settle_time, float turn_timeout){
  turn_to_point(X_position, Y_position, extra_angle_deg, turn_max_voltage, turn_settle_error, turn_settle_time, turn_timeout, turn_kp, turn_ki, turn_kd, turn_starti);
}

void Drive::turn_to_point(float X_position, float Y_position, float extra_angle_deg, float turn_max_voltage, float turn_settle_error, float turn_settle_time, float turn_timeout, float turn_kp, float turn_ki, float turn_kd, float turn_starti){
  PID turnPID(reduce_negative_180_to_180(to_deg(atan2(X_position-get_X_position(),Y_position-get_Y_position())) - get_absolute_heading()), turn_kp, turn_ki, turn_kd, turn_starti, turn_settle_error, turn_settle_time, turn_timeout);
  while(turnPID.is_settled() == false){
    float error = reduce_negative_180_to_180(to_deg(atan2(X_position-get_X_position(),Y_position-get_Y_position())) - get_absolute_heading() + extra_angle_deg);
    float output = turnPID.compute(error);
    output = clamp(output, -turn_max_voltage, turn_max_voltage);
    drive_with_voltage(output, -output);
    task::sleep(10);
  }
}

/**
 * Drives and turns simultaneously to a desired pose.
 * Uses two PID loops, one drive and one heading to drive and turn
 * at the same time. Optimized to turn the quicker direction and only
 * exits once both PID loops have settled. It uses the heading constants
 * for heading but the turn exit conditions to settle.
 * 
 * @param X_position Desired x position in inches.
 * @param Y_position Desired y position in inches.
 * @param angle Desired ending angle in degrees.
 */

void Drive::holonomic_drive_to_pose(float X_position, float Y_position){
  holonomic_drive_to_pose(X_position, Y_position, get_absolute_heading(), drive_max_voltage, heading_max_voltage, drive_settle_error, drive_settle_time, drive_timeout, drive_kp, drive_ki, drive_kd, drive_starti, heading_kp, heading_ki, heading_kd, heading_starti);
}

void Drive::holonomic_drive_to_pose(float X_position, float Y_position, float angle){
  holonomic_drive_to_pose(X_position, Y_position, angle, drive_max_voltage, heading_max_voltage, drive_settle_error, drive_settle_time, drive_timeout, drive_kp, drive_ki, drive_kd, drive_starti, heading_kp, heading_ki, heading_kd, heading_starti);
}

void Drive::holonomic_drive_to_pose(float X_position, float Y_position, float angle, float drive_max_voltage, float heading_max_voltage){
  holonomic_drive_to_pose(X_position, Y_position, angle, drive_max_voltage, heading_max_voltage, drive_settle_error, drive_settle_time, drive_timeout, drive_kp, drive_ki, drive_kd, drive_starti, heading_kp, heading_ki, heading_kd, heading_starti);
}

void Drive::holonomic_drive_to_pose(float X_position, float Y_position, float angle, float drive_max_voltage, float heading_max_voltage, float drive_settle_error, float drive_settle_time, float drive_timeout){
  holonomic_drive_to_pose(X_position, Y_position, angle, drive_max_voltage, heading_max_voltage, drive_settle_error, drive_settle_time, drive_timeout, drive_kp, drive_ki, drive_kd, drive_starti, heading_kp, heading_ki, heading_kd, heading_starti);
}

void Drive::holonomic_drive_to_pose(float X_position, float Y_position, float angle, float drive_max_voltage, float heading_max_voltage, float drive_settle_error, float drive_settle_time, float drive_timeout, float drive_kp, float drive_ki, float drive_kd, float drive_starti, float heading_kp, float heading_ki, float heading_kd, float heading_starti){
  PID drivePID(hypot(X_position-get_X_position(),Y_position-get_Y_position()), drive_kp, drive_ki, drive_kd, drive_starti, drive_settle_error, drive_settle_time, drive_timeout);
  PID turnPID(angle-get_absolute_heading(), heading_kp, heading_ki, heading_kd, heading_starti, turn_settle_error, turn_settle_time, turn_timeout);
  while( !(drivePID.is_settled() && turnPID.is_settled()) ){
    float drive_error = hypot(X_position-get_X_position(),Y_position-get_Y_position());
    float turn_error = reduce_negative_180_to_180(angle-get_absolute_heading());

    float drive_output = drivePID.compute(drive_error);
    float turn_output = turnPID.compute(turn_error);

    drive_output = clamp(drive_output, -drive_max_voltage, drive_max_voltage);
    turn_output = clamp(turn_output, -heading_max_voltage, heading_max_voltage);

    float heading_error = atan2(Y_position-get_Y_position(), X_position-get_X_position());

    DriveLF.spin(fwd, drive_output*cos(to_rad(get_absolute_heading()) + heading_error - M_PI/4) + turn_output, volt);
    DriveLB.spin(fwd, drive_output*cos(-to_rad(get_absolute_heading()) - heading_error + 3*M_PI/4) + turn_output, volt);
    DriveRB.spin(fwd, drive_output*cos(to_rad(get_absolute_heading()) + heading_error - M_PI/4) - turn_output, volt);
    DriveRF.spin(fwd, drive_output*cos(-to_rad(get_absolute_heading()) - heading_error + 3*M_PI/4) - turn_output, volt);
    task::sleep(10);
  }
}

/**
 * Controls a chassis with left stick throttle and right stick turning.
 * Default deadband is 5.
 */

void Drive::control_arcade(){
  float throttle = deadband(controller(primary).Axis3.value(), 5);
  float turn = deadband(controller(primary).Axis1.value(), 5);
  DriveL.spin(fwd, to_volt(throttle+turn), volt);
  DriveR.spin(fwd, to_volt(throttle-turn), volt);
}
void Drive::control_arcade_left(){
  float throttle = deadband(controller(primary).Axis3.value(), 5);
  float turn = deadband(controller(primary).Axis4.value(), 5);
  if (turn > -5 and turn < 5){
    turn = 0;
  }
  DriveL.spin(fwd, to_volt(throttle+turn), volt);
  DriveR.spin(fwd, to_volt(throttle-turn), volt);
}
void Drive::control_arcade_right(){
  float throttle = deadband(controller(primary).Axis2.value(), 5);
  float turn = deadband(controller(primary).Axis1.value(), 5);
  DriveL.spin(fwd, to_volt(throttle+turn), volt);
  DriveR.spin(fwd, to_volt(throttle-turn), volt);
}

/**
 * Controls a chassis with left stick throttle and strafe, and right stick turning.
 * Default deadband is 5.
 */

void Drive::control_holonomic(){
  float throttle = deadband(controller(primary).Axis3.value(), 5);
  float turn = deadband(controller(primary).Axis1.value(), 5);
  float strafe = deadband(controller(primary).Axis4.value(), 5);
  DriveLF.spin(fwd, to_volt(throttle+turn+strafe), volt);
  DriveRF.spin(fwd, to_volt(throttle-turn-strafe), volt);
  DriveLB.spin(fwd, to_volt(throttle+turn-strafe), volt);
  DriveRB.spin(fwd, to_volt(throttle-turn+strafe), volt);
}

/**
 * Controls a chassis with left stick left drive and right stick right drive.
 * Default deadband is 5.
 */

void Drive::control_tank(){
  float leftthrottle = deadband(controller(primary).Axis3.value(), 5);
  float rightthrottle = deadband(controller(primary).Axis2.value(), 5);
  DriveL.spin(fwd, to_volt(leftthrottle), volt);
  DriveR.spin(fwd, to_volt(rightthrottle), volt);
}

void Drive::control_tank_ricky(){
  float leftthrottle = controller(primary).Axis3.value(); //no deadzone
  float rightthrottle = controller(primary).Axis2.value(); 
  
  if(within_range(leftthrottle,rightthrottle, 4.05)){ //was 3.65
    leftthrottle = (leftthrottle + rightthrottle)/2;
    rightthrottle = leftthrottle;
  }

  DriveL.spin(fwd, to_volt(leftthrottle), volt);
  DriveR.spin(fwd, to_volt(rightthrottle), volt);
}

/**
 * Tracking task to run in the background.
 */

int Drive::position_track_task(){
  chassis.position_track();
  return(0);
}

//RAMSETE Helpers:

void Drive::set_track_width(double inches){
  track_width_in_ramsete_cache = inches;
  ramsete_.set_track_width(inches);
}
void Drive::set_ramsete_constants(double b, double zeta){
  ramsete_.set_constants(b, zeta);
}

void Drive::set_wheel_velocity_pid(double kp, double ki, double kd){
  ramsete_.set_wheel_velocity_pid(kp, ki, kd);
}

// Convert motor_group velocity -> in/s via wheel geometry
double Drive::left_speed_ips(){
  // NOTE: VEX uses rpm units here
  const double rpmL = DriveL.velocity(vex::velocityUnits::rpm);
  return rpmL * inches_per_rev() / 60.0;
}
double Drive::right_speed_ips(){
  const double rpmR = DriveR.velocity(vex::velocityUnits::rpm);
  return rpmR * inches_per_rev() / 60.0;
}

//RAMSETE Follower:

void Drive::follow_trajectory_ramsete(const std::vector<TrajState>& traj,
                                      int timeout_ms,
                                      double pos_tol_in,
                                      double ang_tol_deg)
{
  ramsete_.follow(
    traj,
    // pose getters (inches, deg)
    [this]{ return this->get_X_position(); },
    [this]{ return this->get_Y_position(); },
    [this]{ return this->get_absolute_heading(); },
    // wheel speeds (in/s)
    [this]{ return this->left_speed_ips();  },
    [this]{ return this->right_speed_ips(); },
    // drive command (volts)
    [this](double l, double r){ this->drive_with_voltage(l, r); },
    timeout_ms, pos_tol_in, ang_tol_deg
  );

  // optional: stop with hold after completion
  this->drive_stop(vex::brakeType::hold);
}
// ================== drive_forward_spin180 (field-projected, anti-stall) ==================
static inline float lerp_shortest_deg(float a, float b, float t){
  float d = reduce_negative_180_to_180(b - a);
  return reduce_0_to_360(a + d * t);
}

// ---------- convenience wrappers ----------
void Drive::drive_forward_spin180(float distance){
  drive_forward_spin180(distance, /*clockwise=*/true,
                        /*fwd_V*/ 6.5f,
                        /*turn_V_max*/ 9.0f,
                        /*kP_head*/ 0.12f,
                        /*kD_head*/ 0.05f,
                        /*ease*/ 0.70f,
                        /*settle_deg*/ 3, /*settle_ms*/ 150,
                        /*timeout_ms*/ 6000);
}

void Drive::drive_forward_spin180(float distance, bool clockwise){
  drive_forward_spin180(distance, clockwise,
                        /*fwd_V*/ 6.5f,
                        /*turn_V_max*/ 9.0f,
                        /*kP_head*/ 0.12f,
                        /*kD_head*/ 0.05f,
                        /*ease*/ 0.70f,
                        /*settle_deg*/ 3, /*settle_ms*/ 150,
                        /*timeout_ms*/ 6000);
}

// ---------- main implementation (UNWRAPPED HEADING, no 90° flip) ----------
void Drive::drive_forward_spin180(float distance, bool clockwise,
                                  float fwd_V,
                                  float turn_V_max,
                                  float kP_head, float kD_head,
                                  float ease,
                                  int settle_deg, int settle_ms,
                                  int timeout_ms)
{
  // --- safety clamps
  fwd_V       = clamp(fwd_V, -12.0f, 12.0f);
  turn_V_max  = clamp(turn_V_max, 0.0f,  12.0f);
  ease        = fmaxf(0.2f, ease);

  // ====== constants you can tune quickly ======
  const float TURN_FF_VOLTS         = 1.15f;  // constant nudge in spin direction
  const float MIN_FWD_START         = 3.2f;   // forward floor early in the spin
  const float MIN_FWD_END           = 1.1f;   // forward floor late in the spin
  const float FLOOR_ANGLE_DROP_DEG  = 140.0f; // fade the floor after this angle
  const float COS_GATE_FLOOR        = 0.35f;  // keep at least this fraction of fwd
  const int   STALL_WIN_MS          = 180;    // stall window
  const float STALL_DELTA_IN        = 0.06f;  // proj distance change threshold
  const float STALL_BOOST_V         = 1.4f;   // add push if stalled
  const float MID_PUSH_MIN_V        = 2.6f;   // guaranteed forward near 90–150°
  const float MID_PUSH_START_DEG    = 80.0f;
  const float MID_PUSH_END_DEG      = 155.0f;

  // ----- spin direction / target in UNWRAPPED space -----
  const int   TURN_SIGN = clockwise ? -1 : +1;  // CW negative, CCW positive
  const float H_TGT_U   = TURN_SIGN * 180.0f;   // target = ±180° (unwrapped)

  // ----- wheel-average for incremental inches -----
  float prev_avg = (get_left_position_in() + get_right_position_in()) * 0.5f;

  // ----- field-projected progress along the initial heading -----
  float S_proj = 0.0f;

  // ----- unwrap heading state -----
  const float h0_raw   = get_absolute_heading(); // initial raw (0–360)
  float       last_h   = h0_raw;                 // last raw reading
  float       h_now_u  = 0.0f;                   // unwrapped, start = 0

  // ----- PD derivative LPF -----
  float prev_err = H_TGT_U - 0.0f; // at start, current=0, so err = target
  float d_err_lp = 0.0f;
  const float DT    = 0.010f; // 10 ms
  const float ALPHA = 0.7f;   // LPF for derivative

  // ----- anti-stall on projected distance -----
  float last_S   = 0.0f;
  int   stall_ms = 0;

  // ----- settle/timeout -----
  int settle_timer_ms = 0;
  int total_ms        = 0;

  while (true) {
    // === inches advanced (wheel avg) ===
    const float avg_now  = (get_left_position_in() + get_right_position_in()) * 0.5f;
    const float d_inches = avg_now - prev_avg;
    prev_avg = avg_now;

    // === unwrap current heading ===
    const float h_raw  = get_absolute_heading();
    const float d_wrap = reduce_negative_180_to_180(h_raw - last_h); // [-180,180]
    h_now_u += d_wrap;  // accumulate without modulo -> monotonic
    last_h  = h_raw;

    // === project inches along ORIGINAL heading h0_raw ===
    const float rel = to_rad(reduce_negative_180_to_180(h_raw - h0_raw));
    S_proj += d_inches * cosf(rel);

    // === progress & easing ===
    float prog = (distance == 0.0f) ? 1.0f
                 : clamp(S_proj / distance,
                         distance >= 0 ? 0.0f : -1.0f,
                         distance >= 0 ? 1.0f :  0.0f);
    float prog01  = clamp((distance >= 0 ? prog : -prog), 0.0f, 1.0f);
    float t_eased = powf(prog01, ease);

    // === desired heading in unwrapped space (monotonic 0 -> ±180) ===
    const float h_des_u = H_TGT_U * t_eased;

    // === PD on UNWRAPPED error ===
    const float err   = h_des_u - h_now_u;        // no modulo => no 90° flip
    const float d_raw = (err - prev_err) / DT;
    d_err_lp  = ALPHA * d_err_lp + (1.0f - ALPHA) * d_raw;
    prev_err  = err;

    float turn_V = kP_head * err + kD_head * d_err_lp;
    turn_V += (TURN_SIGN * TURN_FF_VOLTS);        // feedforward in spin direction
    turn_V  = clamp(turn_V, -turn_V_max, turn_V_max);

    // === forward gating & floors ===
    const float ang_from_start = fabsf(h_now_u);  // 0..~180 (unwrapped)

    // fade forward as angle grows, but never below COS_GATE_FLOOR
    float cos_gate = cosf(to_rad(fminf(ang_from_start, FLOOR_ANGLE_DROP_DEG)));
    cos_gate = fmaxf(cos_gate, COS_GATE_FLOOR);

    // fade as we finish requested inches along the original line
    const float dist_left = fmaxf(0.0f, distance - S_proj);
    float dist_gate = clamp(dist_left / fmaxf(6.0f, distance), 0.0f, 1.0f);

    // floor blends: strong early, small late
    const float floor_mix = 1.0f - clamp(ang_from_start / FLOOR_ANGLE_DROP_DEG, 0.0f, 1.0f);
    const float fwd_floor = MIN_FWD_END
                          + (MIN_FWD_START - MIN_FWD_END) * floor_mix * (1.0f - t_eased);

    float fwd_cmd = (fwd_V * cos_gate * dist_gate);

    // keep some guaranteed forward near 90–150° so it doesn’t stall
    if (ang_from_start > MID_PUSH_START_DEG && ang_from_start < MID_PUSH_END_DEG) {
      fwd_cmd = fmaxf(fwd_cmd, MID_PUSH_MIN_V);
    }

    // hold the floor until we’ve rotated a lot AND are mostly done with distance
    const bool hold_floor = (ang_from_start < FLOOR_ANGLE_DROP_DEG) || (prog01 < 0.85f);
    if (hold_floor) fwd_cmd = fmaxf(fwd_cmd, fwd_floor);

    // anti-stall on projected distance progress
    if (fabsf(S_proj - last_S) < STALL_DELTA_IN) {
      stall_ms += 10;
      if (stall_ms >= STALL_WIN_MS) fwd_cmd += STALL_BOOST_V;
    } else {
      stall_ms = 0;
      last_S   = S_proj;
    }

    // === drive it ===
    float leftV  = clamp(fwd_cmd + turn_V,  -12.0f, 12.0f);
    float rightV = clamp(fwd_cmd - turn_V,  -12.0f, 12.0f);
    drive_with_voltage(leftV, rightV);

    // === exit: distance reached AND final angle settled (unwrapped) ===
    const bool  dist_done = (S_proj >= distance - 0.5f);
    const float final_err = H_TGT_U - h_now_u;      // unwrapped final error
    const bool  ang_good  = fabsf(final_err) <= (float)settle_deg;

    if (dist_done && ang_good) {
      if (settle_timer_ms >= settle_ms) break;
      settle_timer_ms += 10;
    } else {
      settle_timer_ms = 0;
    }

    if (timeout_ms > 0 && total_ms >= timeout_ms) break;

    task::sleep(10);
    total_ms += 10;
  }

  drive_stop(vex::brakeType::hold);
}
