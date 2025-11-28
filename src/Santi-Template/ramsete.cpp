#include "Santi-Template/ramsete.h"
#include "vex.h"

//Local helpers (implementation-only; no header dependency on vex)
namespace {
  inline double rad(double deg) { return deg * M_PI / 180.0; }
  inline double wrap_deg180(double deg) {
    while (deg <= -180.0) deg += 360.0;
    while (deg >   180.0) deg -= 360.0;
    return deg;
  }
  inline double clampd(double x, double lo, double hi) {
    return (x < lo) ? lo : (x > hi) ? hi : x;
  }
}

//---- Public setters -------------------------------------------------
void RamseteFollower::set_track_width(double inches){ track_width_in = inches; }
void RamseteFollower::set_constants(double b, double z){ b_gain = b; zeta = z; }
void RamseteFollower::set_wheel_velocity_pid(double kp, double ki, double kd){
  pidL.kp=kp; pidL.ki=ki; pidL.kd=kd; pidL.i=0; pidL.prev_e=0;
  pidR.kp=kp; pidR.ki=ki; pidR.kd=kd; pidR.i=0; pidR.prev_e=0;
}

//---- PID utility ----------------------------------------------------
double RamseteFollower::step_pid(PIDv& p, double err, double dt){
  p.i += err * dt;
  const double d = (err - p.prev_e) / std::max(dt, 1e-3);
  p.prev_e = err;
  return p.kp*err + p.ki*p.i + p.kd*d;
}

//---- Trajectory helper ----------------------------------------------
std::vector<TrajState> RamseteFollower::make_simple_line(double x0, double y0, double heading_deg,
                                                         double dist_in, double T_sec, double dt){
  const int N = (int)std::round(T_sec / dt);
  std::vector<TrajState> v; v.reserve(N+1);
  const double th = rad(heading_deg);
  for (int i=0;i<=N;++i){
    const double t = i*dt;
    const double s = dist_in * (t / T_sec);
    const double x = x0 + s * std::sin(th);
    const double y = y0 + s * std::cos(th);
    const double vlin = dist_in / T_sec;
    v.push_back({t, x, y, heading_deg, vlin, 0.0});
  }
  return v;
}

//---- Core RAMSETE follower ------------------------------------------
void RamseteFollower::follow(const std::vector<TrajState>& traj,
                             std::function<double(void)> getX_in,
                             std::function<double(void)> getY_in,
                             std::function<double(void)> getHeading_deg,
                             std::function<double(void)> getLeftSpeed_ips,
                             std::function<double(void)> getRightSpeed_ips,
                             std::function<void(double,double)> driveWithVoltage,
                             int timeout_ms,
                             double pos_tol_in,
                             double ang_tol_deg)
{
  if (traj.empty()) return;

  vex::timer tmr; tmr.clear();

  const double ang_tol_rad = rad(ang_tol_deg);
  const double half_track  = track_width_in * 0.5;

  int settled_count = 0;
  double last_time_s = 0.0;

  while (true) {
    const double t = tmr.time(vex::timeUnits::msec) / 1000.0;
    if (timeout_ms > 0 && tmr.time(vex::timeUnits::msec) > timeout_ms) break;

    //interpolate state
    const TrajState* s = &traj.back();
    for (size_t i=1;i<traj.size();++i){
      if (t <= traj[i].t){
        const auto& a = traj[i-1]; const auto& b = traj[i];
        const double u = (t - a.t) / std::max(b.t - a.t, 1e-9);
        static TrajState tmp;
        tmp.t = t;
        tmp.x = a.x + u*(b.x - a.x);
        tmp.y = a.y + u*(b.y - a.y);
        tmp.heading_deg = a.heading_deg + u*wrap_deg180(b.heading_deg - a.heading_deg);
        tmp.v = a.v + u*(b.v - a.v);
        tmp.w = a.w + u*(b.w - a.w);
        s = &tmp;
        break;
      }
    }

    //current pose
    const double xr = getX_in();
    const double yr = getY_in();
    const double th_r = rad(getHeading_deg());

    //target pose
    const double xd = s->x, yd = s->y;
    const double th_d = rad(s->heading_deg);
    const double vd = s->v;
    const double wd = s->w;

    //pose error in robot frame
    const double dx = xd - xr;
    const double dy = yd - yr;
    const double c = std::cos(th_r), sR = std::sin(th_r);
    const double ex  =  c*dx + sR*dy;
    const double ey  = -sR*dx + c*dy;
    const double eth = rad(wrap_deg180(s->heading_deg - getHeading_deg()));

    //RAMSETE gains
    const double k = 2.0 * zeta * std::sqrt(wd*wd + b_gain*vd*vd);

    //sinc
    const double sinc = (std::fabs(eth) < 1e-6) ? (1.0 - (eth*eth)/6.0) : (std::sin(eth)/eth);

    //target chassis speeds
    const double v_cmd = vd * std::cos(eth) + k * ex;
    const double w_cmd = wd + k * eth + b_gain * vd * sinc * ey;

    //tank wheel speeds
    const double vL_cmd = v_cmd - w_cmd * half_track;
    const double vR_cmd = v_cmd + w_cmd * half_track;

    //measured
    const double vL_meas = getLeftSpeed_ips();
    const double vR_meas = getRightSpeed_ips();

    //wheel PID (no FF)
    const double now_s = t;
    const double dt = std::max(now_s - last_time_s, 1e-3);
    last_time_s = now_s;

    const double uL = clampd(step_pid(pidL, (vL_cmd - vL_meas), dt), -12.0, 12.0);
    const double uR = clampd(step_pid(pidR, (vR_cmd - vR_meas), dt), -12.0, 12.0);

    driveWithVoltage(uL, uR);

    //settle near end
    const bool at_end_time = (tmr.time(vex::timeUnits::msec) >= (int)(traj.back().t*1000.0 - 1));
    if (at_end_time) {
      const bool pos_ok = std::hypot(dx, dy) <= pos_tol_in;
      const bool ang_ok = std::fabs(eth) <= ang_tol_rad;
      if (pos_ok && ang_ok) {
        if (++settled_count >= 10) break;
      } else {
        settled_count = 0;
      }
    }

    vex::this_thread::sleep_for(10);
  }

  //stop (caller can also choose hold separately)
  driveWithVoltage(0, 0);
}
