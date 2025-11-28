#pragma once
#include <vector>
#include <functional>
#include <cmath>
#include <algorithm>

struct TrajState {
  double t;            //Time in seconds
  double x;            //XPOS of the robot on the field in inches (from blue left corner)
  double y;            //YPOS of the robot on the field in inches (from blue left corner)
  double heading_deg;  //Heading of the robot in degrees
  double v;            //Velocity in inches per second
  double w;            //rad/s
};

class RamseteFollower {
public:
  void set_track_width(double inches);
  void set_constants(double b, double z);
  void set_wheel_velocity_pid(double kp, double ki, double kd);

  //Core follow (blocking). Callbacks come from Drive.
  void follow(const std::vector<TrajState>& traj,
              std::function<double(void)> getX_in,
              std::function<double(void)> getY_in,
              std::function<double(void)> getHeading_deg,
              std::function<double(void)> getLeftSpeed_ips,
              std::function<double(void)> getRightSpeed_ips,
              std::function<void(double,double)> driveWithVoltage,
              int timeout_ms = 0,
              double pos_tol_in = 1.0,
              double ang_tol_deg = 3.0);

  //Utility: simple straight line generator (constant velocity)
  static std::vector<TrajState> make_simple_line(double x0, double y0, double heading_deg,
                                                 double dist_in, double T_sec, double dt=0.01);

private:
  struct PIDv { double kp=0, ki=0, kd=0, i=0, prev_e=0; };
  static double step_pid(PIDv& p, double err, double dt);

  //Config & state
  PIDv pidL, pidR;
  double track_width_in = 12.0;
  double b_gain = 2.0;
  double zeta   = 0.7;
};
