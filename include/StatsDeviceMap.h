#pragma once
#include <vector>
#include "vex.h"
#include "robot-config.h"   // uses your existing device variables

struct MotorEntry   { const char* name; vex::motor*    m;   };
struct ImuEntry     { const char* name; vex::inertial* imu; };
struct GpsEntry     { const char* name; vex::gps*      g;   };
struct DistEntry    { const char* name; vex::distance* d;   };
struct OptEntry     { const char* name; vex::optical*  o;   };
struct RotEntry     { const char* name; vex::rotation* r;   };

// Motors
static inline std::vector<MotorEntry> STATS_MOTORS = {
  { "Left1 (P8)",         &LeftMotor1 },
  { "Left2 (P9)",         &LeftMotor2 },
  { "Left3 (P10)",        &LeftMotor3 },
  { "Right1 (P1)",        &RightMotor1 },
  { "Right2 (P2)",        &RightMotor2 },
  { "Right3 (P3)",        &RightMotor3 },
  { "Intake (P14)",       &intakeMotor },
  { "MiddleTravel (P13)", &middleTravel },
  { "Score (P15)",        &scoreMotor },
};

// IMU
static inline std::vector<ImuEntry> STATS_IMUS = {
  { "IMU (P20)", &Inertial },
};

// GPS
static inline std::vector<GpsEntry> STATS_GPS = {
  { "GPS (P7)", &GPS7 },
};

// Distance
static inline std::vector<DistEntry> STATS_DISTANCE = {
  { "RightDist (P5)",  &DistanceRight },
  { "BackDist (P19)",  &DistanceBack },
  { "LeftDist (P6)",   &DistanceLeft },
  { "FrontDist (P18)", &DistanceFront },
};

// Optical
static inline std::vector<OptEntry> STATS_OPTICAL = {
  { "Optical (P21)",      &Optical21 },
  { "MatchloadOpt (P17)", &MatchloadSensor },
};

// Rotation
static inline std::vector<RotEntry> STATS_ROTATION = {
  { "Odom (P4)", &odom },
};
