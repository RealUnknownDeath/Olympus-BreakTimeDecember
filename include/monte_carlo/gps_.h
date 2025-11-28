#pragma once
#include "vex.h"

class gps_
{
private:

public:

  //Vex default GPS configurations
  int gps_Port;
  float gps_X_Offset;
  float gps_Y_Offset;
  float gps_Orientation;

  gps gps_sensor; //the sensor itself

  //constructor:
  gps_(
    int gps_Port,
    float gps_X_Offset,
    float gps_Y_Offset,
    float gps_Orientation
  );

  //getters:
  float get_X();
  float get_Y();
  float get_Heading();
  int get_certainty();

  //setters:
  void set_X(float x);
  void set_Y(float y);
  void set_Heading(float heading, rotationUnits units);
  void set_coordinates(float xpos, float ypos, float angle = -1);

  void calibrate();
  

};