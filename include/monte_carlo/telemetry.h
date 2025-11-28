#pragma once
#include "vex.h"

//Struct to hold expected sensor readings for a particle
struct ExpectedSensorReadings {
  float front;
  float back;
  float left;
  float right;
};

class Telem
{
private:
  
public:

  float distanceLeftX;
  float distanceLeftY;
  
  float distanceRightX;
  float distanceRightY;

  float distanceFrontX;
  float distanceFrontY;
  
  float distanceBackX;
  float distanceBackY;

  bool intersectsSegment(const Vector2f& p1, const Vector2f& p2, const Segment& line, Vector2f& intersection);

  float averaged_beliefX;
  float averaged_beliefY;

  float distanceLeft_Ajust;
  float distanceRight_Ajust;
  float distanceFront_Ajust;
  float distanceBack_Ajust;

  float heightLevel;

  distance distanceLeft;
  float distanceLeftCenterDistance;
  float distanceLeftCenterOffset;

  distance distanceRight;
  float distanceRightCenterDistance;
  float distanceRightCenterOffset;

  distance distanceFront;
  float distanceFrontCenterDistance;
  float distanceFrontCenterOffset;

  distance distanceBack;
  float distanceBackCenterDistance;
  float distanceBackCenterOffset;

  float distanceLeftReading;
  float distanceRightReading;
  float distanceFrontReading;
  float distanceBackReading;

  float beliefX;
  float beliefY;

  float impactX;
  float impactY;

  Telem(float heightLevel,
  int distanceLeftPort, float distanceLeftCenterDistance, float distanceLeftCenterOffset,
  int distanceRightPort, float distanceRightCenterDistance, float distanceRightCenterOffset,
  int distanceFrontPort, float distanceFrontCenterDistance, float distanceFrontCenterOffset,
  int distanceBackPort, float distanceBackCenterDistance, float distanceBackCenterOffset);

  void updateReadings();

  void calculateDistances(float xPosition, float yPosition, const std::vector<Segment>& obstacleLines);

  void updatePositionBelief(int times, const std::vector<Segment>& obstacles);


  int distanceTrack();

  static int distanceTrackTask(void *param); //Static wrapper that matches the task callback signature. Vex can be so annoying at times.

  void startDistanceTracking();

  vex::task telem_task;


  float get_X();

  float get_Y();

  float predictSensorReading(float x, float y, float heading_deg, std::string sensorName);

  ExpectedSensorReadings expected_readings_for_particle(const Particle& p, const std::vector<Segment>& obstacles);

  void increase_odom_X(float value);
  
  void increase_odom_Y(float value);

};