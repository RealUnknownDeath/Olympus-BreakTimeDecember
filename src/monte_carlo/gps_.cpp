#include "vex.h"
#include <iostream>
#include <map>
#include <string>

using namespace vex;

gps_::gps_(int gps_Port, float gps_X_Offset, float gps_Y_Offset, float gps_Orientation):

  gps_Port(gps_Port),
  gps_X_Offset(gps_X_Offset),
  gps_Y_Offset(gps_Y_Offset),
  gps_Orientation(gps_Orientation),
  gps_sensor(gps(gps_Port, gps_X_Offset, gps_Y_Offset, inches, gps_Orientation))

{};

void gps_::calibrate(){
  gps_sensor.calibrate(5);
  if (!gps_sensor.installed()){
    //Controller1.rumble("...");
  }
}

void gps_::set_coordinates(float xpos, float ypos, float angle){
  if (angle == -1){
    gps_sensor.setLocation(xpos, ypos);
  }
  else{
    gps_sensor.setLocation(xpos, ypos, angle);
  }
}


//Getters:
int gps_::get_certainty(){
  return gps_sensor.quality();
}

float gps_::get_X() {
  return gps_sensor.yPosition(inches) + 72;
}

float gps_::get_Y() {
  return 72-gps_sensor.xPosition(inches);
}

float gps_::get_Heading(){
  return gps_sensor.heading();
}


//Setters: (realistically going to be unused)
void gps_::set_X(float x) {
  gps_sensor.setLocation(x, get_Y());
}

void gps_::set_Y(float y) {
  gps_sensor.setLocation(get_X(), y);
}

void gps_::set_Heading(float heading, rotationUnits units) {
  gps_sensor.setHeading(heading, units);
}
