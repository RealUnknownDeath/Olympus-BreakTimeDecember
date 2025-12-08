#pragma once
#include "vex.h"
#include "Santi-Template/ramsete.h"

enum drive_setup {ZERO_TRACKER_NO_ODOM, ZERO_TRACKER_ODOM, TANK_ONE_FORWARD_ENCODER, TANK_ONE_FORWARD_ROTATION, 
TANK_ONE_SIDEWAYS_ENCODER, TANK_ONE_SIDEWAYS_ROTATION, TANK_TWO_ENCODER, TANK_TWO_ROTATION, 
HOLONOMIC_TWO_ENCODER, HOLONOMIC_TWO_ROTATION};

/**
 * Drive class supporting tank and holo drive, with or without odom.
 * Eight flavors of odom and six custom motion algorithms.
 */

class Drive
{
private:
  float wheel_diameter;
  float wheel_ratio;
  float gyro_scale;
  float drive_in_to_deg_ratio;
  float ForwardTracker_diameter;
  float ForwardTracker_center_distance;
  float ForwardTracker_in_to_deg_ratio;
  float SidewaysTracker_center_distance;
  float SidewaysTracker_diameter;
  float SidewaysTracker_in_to_deg_ratio;
  vex:: triport ThreeWire = vex::triport(vex::PORT22);

  RamseteFollower ramsete_;
  double track_width_in_ramsete_cache = 12.0; // optional cache if you want to expose later
  // helper accessors used by RAMSETE callbacks
  double left_speed_ips();
  double right_speed_ips();
  double inches_per_rev() const { return M_PI * wheel_diameter * wheel_ratio; }

public: 
  drive_setup drive_setup = ZERO_TRACKER_NO_ODOM;
  motor_group DriveL;
  motor_group DriveR;
  inertial Gyro;
  motor DriveLF;
  motor DriveRF;
  motor DriveLB;
  motor DriveRB;
  rotation R_ForwardTracker;
  rotation R_SidewaysTracker;
  encoder E_ForwardTracker;
  encoder E_SidewaysTracker;

  float turn_max_voltage;
  float turn_kp;
  float turn_ki;
  float turn_kd;
  float turn_starti;

  float turn_settle_error;
  float turn_settle_time;
  float turn_timeout;

  float drive_min_voltage;
  float drive_max_voltage;
  float drive_kp;
  float drive_ki;
  float drive_kd;
  float drive_starti;

  float drive_settle_error;
  float drive_settle_time;
  float drive_timeout;

  float heading_max_voltage;
  float heading_kp;
  float heading_ki;
  float heading_kd;
  float heading_starti;

  float swing_max_voltage;
  float swing_kp;
  float swing_ki;
  float swing_kd;
  float swing_starti;

  float swing_settle_error;
  float swing_settle_time;
  float swing_timeout;

  float boomerang_lead;
  float boomerang_setback;

  //MATCHLOADER ALIGNMENT TUNES:

  double ml_front_sensor_offset = 0.0;
  double ml_left_sensor_offset = -90.0;
  double ml_right_sensor_offset = 90.0;

  //Phases:
  double ml_align_phase_end = 0.825; ///was 0.7
  double ml_slow_down_start = 0.9;
  double ml_slow_down_factor = 0.4;
  
  //Gains:
  double ml_kF = 0.45; //was 0.35
  double ml_kAlign = 4.625; //was 2.0
  double ml_max_lean = 45.65; //was 22.65
  
  double ml_max_forward_voltage = 10.0;
  
  double ml_heading_kP = 0.9;
  double ml_heading_kI = 0.0;
  double ml_heading_kD = 3.5;

  //Stop Tolerances:
  double ml_front_tolerance = 0.5;
  double ml_left_tolerance = 0.25;
  double ml_right_tolerance = 0.25;
  double ml_heading_tolerance = 1.0;
  
  //Misc:
  double ml_timeout = 1500.0;
  double ml_loopDt = 20.0;
  double ml_hold_time_ms = 118.5;

  Drive(enum::drive_setup drive_setup, motor_group DriveL, motor_group DriveR, int gyro_port, float wheel_diameter, float wheel_ratio, float gyro_scale, int DriveLF_port, int DriveRF_port, int DriveLB_port, int DriveRB_port, int ForwardTracker_port, float ForwardTracker_diameter, float ForwardTracker_center_distance, int SidewaysTracker_port, float SidewaysTracker_diameter, float SidewaysTracker_center_distance);

  void drive_with_voltage(float leftVoltage, float rightVoltage);

  float get_absolute_heading();

  float get_left_position_in();

  float get_right_position_in();

  void set_turn_constants(float turn_max_voltage, float turn_kp, float turn_ki, float turn_kd, float turn_starti); 
  void set_drive_constants(float drive_max_voltage, float drive_kp, float drive_ki, float drive_kd, float drive_starti);
  void set_heading_constants(float heading_max_voltage, float heading_kp, float heading_ki, float heading_kd, float heading_starti);
  void set_swing_constants(float swing_max_voltage, float swing_kp, float swing_ki, float swing_kd, float swing_starti);

  void set_turn_exit_conditions(float turn_settle_error, float turn_settle_time, float turn_timeout);
  void set_drive_exit_conditions(float drive_settle_error, float drive_settle_time, float drive_timeout);
  void set_swing_exit_conditions(float swing_settle_error, float swing_settle_time, float swing_timeout);

  void turn_to_angle(float angle);
  void turn_to_angle(float angle, float turn_max_voltage);
  void turn_to_angle(float angle, float turn_max_voltage, float turn_settle_error, float turn_settle_time, float turn_timeout);
  void turn_to_angle(float angle, float turn_max_voltage, float turn_settle_error, float turn_settle_time, float turn_timeout, float turn_kp, float turn_ki, float turn_kd, float turn_starti);

  void drive_distance(float distance);
  void drive_distance(float distance, float heading);
  void drive_distance(float distance, float heading, float drive_max_voltage, float heading_max_voltage);
  void drive_distance(float distance, float heading, float drive_max_voltage, float heading_max_voltage, float drive_settle_error, float drive_settle_time, float drive_timeout);
  void drive_distance(float distance, float heading, float drive_max_voltage, float heading_max_voltage, float drive_settle_error, float drive_settle_time, float drive_timeout, float drive_kp, float drive_ki, float drive_kd, float drive_starti, float heading_kp, float heading_ki, float heading_kd, float heading_starti);

  void drive_distance_sensor(float distance);
  void drive_distance_sensor(float distance, float heading);
  void drive_distance_sensor(float distance, float heading, float drive_max_voltage, float heading_max_voltage);
  void drive_distance_sensor(float distance, float heading, float drive_max_voltage, float heading_max_voltage, float drive_settle_error, float drive_settle_time, float drive_timeout);
  void drive_distance_sensor(float distance, float heading, float drive_max_voltage, float heading_max_voltage, float drive_settle_error, float drive_settle_time, float drive_timeout, float drive_kp, float drive_ki, float drive_kd, float drive_starti, float heading_kp, float heading_ki, float heading_kd, float heading_starti);

  void left_swing_to_angle(float angle);
  void left_swing_to_angle(float angle, bool no_lock);
  void left_swing_to_angle(float angle, bool no_lock, float swing_max_voltage, float swing_settle_error, float swing_settle_time, float swing_timeout, float swing_kp, float swing_ki, float swing_kd, float swing_starti);

  void left_hook(float angle, double ratio);
  void left_hook(float angle, double ratio, float swing_max_voltage, float swing_settle_error, float swing_settle_time, float swing_timeout, float swing_kp, float swing_ki, float swing_kd, float swing_starti);

  void right_swing_to_angle(float angle);
  void right_swing_to_angle(float angle, bool no_lock);
  void right_swing_to_angle(float angle, bool no_lock, float swing_max_voltage, float swing_settle_error, float swing_settle_time, float swing_timeout, float swing_kp, float swing_ki, float swing_kd, float swing_starti);

  void right_hook(float angle, double ratio);
  void right_hook(float angle, double ratio, float swing_max_voltage, float swing_settle_error, float swing_settle_time, float swing_timeout, float swing_kp, float swing_ki, float swing_kd, float swing_starti);
  
  void goal_swing_to_angle(float angle);
  
  void left_front_sensor_drive_distance(double front_target, double left_target, double heading_target);
  void left_front_sensor_drive_distance(double front_target, double left_target, double heading_target,
                                        double front_sensor_offset, double left_sensor_offset,
                                        double align_phase_end, double slow_down_start, double slow_down_factor,
                                        double kF, double kAlign, double max_lean, double max_forward_voltage,
                                        double heading_kP, double heading_kI, double heading_kD,
                                        double front_tolerance, double left_tolerance, double heading_tolerance,
                                        double timeout, int loopDt, double hold_time_ms);

  void right_front_sensor_drive_distance(double front_target, double right_target, double heading_target);
  void right_front_sensor_drive_distance(double front_target, double right_target, double heading_target,
                                        double front_sensor_offset, double right_sensor_offset,
                                        double align_phase_end, double slow_down_start, double slow_down_factor,
                                        double kF, double kAlign, double max_lean, double max_forward_voltage,
                                        double heading_kP, double heading_kI, double heading_kD,
                                        double front_tolerance, double right_tolerance, double heading_tolerance,
                                        double timeout, int loopDt, double hold_time_ms);

  Odom odom;
  float get_ForwardTracker_position();
  float get_SidewaysTracker_position();
  void set_coordinates(float X_position, float Y_position, float orientation_deg);
  void set_heading(float orientation_deg);
  void position_track();
  static int position_track_task();
  vex::task odom_task;
  float get_X_position();
  float get_Y_position();

  void drive_stop(vex::brakeType mode);

  void drive_to_point(float X_position, float Y_position);
  void drive_to_point(float X_position, float Y_position, float drive_min_voltage, float drive_max_voltage, float heading_max_voltage);
  void drive_to_point(float X_position, float Y_position, float drive_min_voltage, float drive_max_voltage, float heading_max_voltage, float drive_settle_error, float drive_settle_time, float drive_timeout);
  void drive_to_point(float X_position, float Y_position, float drive_min_voltage, float drive_max_voltage, float heading_max_voltage, float drive_settle_error, float drive_settle_time, float drive_timeout, float drive_kp, float drive_ki, float drive_kd, float drive_starti, float heading_kp, float heading_ki, float heading_kd, float heading_starti);

  void drive_to_point_Telem(float X_position, float Y_position);
  void drive_to_point_Telem(float X_position, float Y_position, float drive_min_voltage, float drive_max_voltage, float heading_max_voltage, float drive_settle_error, float drive_settle_time, float drive_timeout, float drive_kp, float drive_ki, float drive_kd, float drive_starti, float heading_kp, float heading_ki, float heading_kd, float heading_starti);

  void drive_to_point_Monte(float X_position, float Y_position);
  void drive_to_point_Monte(float X_position, float Y_position, float drive_min_voltage, float drive_max_voltage, float heading_max_voltage, float drive_settle_error, float drive_settle_time, float drive_timeout, float drive_kp, float drive_ki, float drive_kd, float drive_starti, float heading_kp, float heading_ki, float heading_kd, float heading_starti);

  void drive_to_pose(float X_position, float Y_position, float angle);
  void drive_to_pose(float X_position, float Y_position, float angle, float lead, float setback, float drive_min_voltage);
  void drive_to_pose(float X_position, float Y_position, float angle, float lead, float setback, float drive_min_voltage, float drive_max_voltage, float heading_max_voltage);
  void drive_to_pose(float X_position, float Y_position, float angle, float lead, float setback, float drive_min_voltage, float drive_max_voltage, float heading_max_voltage, float drive_settle_error, float drive_settle_time, float drive_timeout);
  void drive_to_pose(float X_position, float Y_position, float angle, float lead, float setback, float drive_min_voltage, float drive_max_voltage, float heading_max_voltage, float drive_settle_error, float drive_settle_time, float drive_timeout, float drive_kp, float drive_ki, float drive_kd, float drive_starti, float heading_kp, float heading_ki, float heading_kd, float heading_starti);
  
  void turn_to_point(float X_position, float Y_position);
  void turn_to_point(float X_position, float Y_position, float extra_angle_deg);
  void turn_to_point(float X_position, float Y_position, float extra_angle_deg, float turn_max_voltage, float turn_settle_error, float turn_settle_time, float turn_timeout);
  void turn_to_point(float X_position, float Y_position, float extra_angle_deg, float turn_max_voltage, float turn_settle_error, float turn_settle_time, float turn_timeout, float turn_kp, float turn_ki, float turn_kd, float turn_starti);
  
  void holonomic_drive_to_pose(float X_position, float Y_position);
  void holonomic_drive_to_pose(float X_position, float Y_position, float angle);
  void holonomic_drive_to_pose(float X_position, float Y_position, float angle, float drive_max_voltage, float heading_max_voltage);
  void holonomic_drive_to_pose(float X_position, float Y_position, float angle, float drive_max_voltage, float heading_max_voltage, float drive_settle_error, float drive_settle_time, float drive_timeout);
  void holonomic_drive_to_pose(float X_position, float Y_position, float angle, float drive_max_voltage, float heading_max_voltage, float drive_settle_error, float drive_settle_time, float drive_timeout, float drive_kp, float drive_ki, float drive_kd, float drive_starti, float heading_kp, float heading_ki, float heading_kd, float heading_starti);

  void control_arcade();
  void control_arcade_left();
  void control_arcade_right();
  void control_tank();
  void control_tank_ricky();
  void control_holonomic();

  void set_track_width(double inches);
  void set_ramsete_constants(double b, double zeta);
  void set_wheel_velocity_pid(double kp, double ki, double kd);

  //Follow a time-parameterized trajectory
  void follow_trajectory_ramsete(const std::vector<TrajState>& traj,
                                 int timeout_ms = 0,
                                 double pos_tol_in = 1.0,
                                 double ang_tol_deg = 3.0);

  // Spin-while-forward (continuous)
  // 1) simple
  void drive_forward_spin180(float distance);
  // 2) choose direction
  void drive_forward_spin180(float distance, bool clockwise);
  // 3) full tuning
  void drive_forward_spin180(float distance, bool clockwise,
                           float fwd_V,
                           float turn_V_max,
                           float kP_head, float kD_head,
                           float ease,
                           int settle_deg, int settle_ms,
                           int timeout_ms);


};