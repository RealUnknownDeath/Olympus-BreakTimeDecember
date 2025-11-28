//Santi Utils:
#include "vex.h"
#include "santis_eigen.h"

float distanceFrom_0(float angle);

bool within_range(float value1, float value2, float range);

float random_float(int min, int max, float seed);

int random_integer(int min, int max, float seed);

Vector2f rotate_vector(float x, float y, float heading_deg);

bool point_on_vertical_line(float x, float y, float x_line, float y_min, float y_max);

bool point_on_horizontal_line(float x, float y, float y_line, float x_min, float x_max);

//Jar Template Utils

float reduce_0_to_360(float angle);

float reduce_negative_180_to_180(float angle);

float reduce_negative_90_to_90(float angle);

float to_rad(float angle_deg);

float to_deg(float angle_rad);

float clamp(float input, float min, float max);

bool is_reversed(double input);

double to_volt(double percent);

int to_port(int port);

float deadband(float input, float width);

bool is_line_settled(float desired_X, float desired_Y, float desired_angle, float current_X, float current_Y);

float left_voltage_scaling(float drive_output, float heading_output);

float right_voltage_scaling(float drive_output, float heading_output);

float clamp_min_voltage(float drive_output, float drive_min_voltage);