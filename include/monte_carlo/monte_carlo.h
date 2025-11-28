#pragma once
#include "santis_eigen.h"
#include "vex.h"


class monte_carlo {
private:

  float last_Left_Drivetrain;
  float last_Right_Drivetrain;

  std::list<Particle> particles = {};

  int resample_counter;

public:

  float tps;

  float gps_importance;
  float telemetry_importance;
  float odom_importance;

  std::vector<Segment> obstacleLines;

  float total_weight;

  float average_weight;

  //drivetrain config:

  float drivetrain_noise;
  float heading_noise;

  //inner classes

  gps_ gps_system;
  Telem telemetry;

  monte_carlo(

    float gps_importance,
    float telemetry_importance,
    float odom_importance,

    //drivetrain config:

    float drivetrain_noise,
    float heading_noise,

    //gps config:
    int gps_Port,
    float gps_X_Offset,
    float gps_Y_Offset,
    float gps_Orientation,


    //telem config:
    float heightLevel,
    int distanceLeftPort, float leftXOffset, float leftYOffset,
    int distanceRightPort, float rightXOffset, float rightYOffset,
    int distanceFrontPort, float frontXOffset, float frontYOffset,
    int distanceBackPort, float backXOffset, float backYOffset
  );

  void innit();

  bool out_of_field(Particle p);
    
  void start_monte_carlo();
    

  void update_Particles_simple(float x, float y, float heading);
  void update_Particles(std::default_random_engine& rng,
                        std::uniform_real_distribution<float>& avgDistribution,
                        std::uniform_real_distribution<float>& angleDistribution);

  void run_Predictions();
  void create_Particles(int number, bool in_Field = false);
  void monte_carlo_loop();
  
  static int monte_carlo_Task(void* instance);

  vex::task monte_Task;

  void calibrate_Odom();

  void display_particles(int origin_X, int origin_Y);

  Particle average_Particle();

  Particle average_position;

  void weigh_particles();
  
  float weight_particle(Particle const);

  void resample_particle(Particle& p);

  void stratified_resample();

};
