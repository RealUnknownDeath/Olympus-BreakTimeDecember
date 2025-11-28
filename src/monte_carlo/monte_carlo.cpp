#include "vex.h"
#include <iostream>
#include <map>
#include <string>
#include <random>
#include <cmath>
using namespace vex;

monte_carlo::monte_carlo(

  float gps_importance, //How important the GPS is relative to the other metrics
  float telemetry_importance, //How important the telemetry is relative to the other metrics
  float odom_importance, //How important the odom is relative to the other metrics

  //Drivetrain config:

  float drivetrain_noise, //How much noise to add to the DT movements (when the are applied to all of the particles)
  float heading_noise, //How much noise to add to the heading changes to all the particles

  //GPS parameters:
  int gps_Port,
  float gps_X_Offset,
  float gps_Y_Offset,
  float gps_Orientation,

  //Telemetry parameters:
  float heightLevel,
  int distanceLeftPort, float leftXOffset, float leftYOffset,
  int distanceRightPort, float rightXOffset, float rightYOffset,
  int distanceFrontPort, float frontXOffset, float frontYOffset,
  int distanceBackPort, float backXOffset, float backYOffset
  ):

  //Inner Classes
  gps_system(gps_Port, gps_X_Offset, gps_Y_Offset, gps_Orientation), //GPS instance
  telemetry(heightLevel,
  distanceLeftPort, leftXOffset, leftYOffset,
  distanceRightPort, rightXOffset, rightYOffset,
  distanceFrontPort, frontXOffset, frontYOffset,
  distanceBackPort, backXOffset, backYOffset), //Telemetry Instance

  //init config

  drivetrain_noise(drivetrain_noise),

  heading_noise(heading_noise),
  
  average_position(Vector2f(0,0), 0.0f), //just set the average position to be (0,0) for now

  gps_importance(gps_importance),
  telemetry_importance(telemetry_importance),
  odom_importance(odom_importance)

    
{}

void monte_carlo::innit() {
    //start the last positions measurements:
    last_Left_Drivetrain = chassis.get_left_position_in();
    last_Right_Drivetrain = chassis.get_right_position_in();


    /*
    obstacleLines.clear(); //old objects from testfield at school
    obstacleLines.emplace_back(ObstacleLine::VERTICAL, 32.0f, 0.0f, 32.0f, 20.0f);
    obstacleLines.emplace_back(ObstacleLine::HORIZONTAL, 10.0f, 122.0f, 30.0f, 122.0f);
    obstacleLines.emplace_back(ObstacleLine::VERTICAL, 14.5f, 70.0f, 14.5f, 93.5f);
    obstacleLines.emplace_back(ObstacleLine::HORIZONTAL, 0.0f, 70.0f, 14.5f, 70.0f);
    */

    //Telemetry Prediction Obstacle Approximations (come from the desmos graph)
    this->obstacleLines.clear();
    // Left Long goal
    this->obstacleLines.push_back(Segment(20.0f, 46.0f, 23.0f, 49.0f)); //L1
    this->obstacleLines.push_back(Segment(23.0f, 49.0f, 26.0f, 46.0f)); //L2

    this->obstacleLines.push_back(Segment(26.0f, 98.0f, 23.0f, 95.0f)); //L3
    this->obstacleLines.push_back(Segment(23.0f, 95.0f, 20.0f, 98.0f)); //L4

    // Right Long goal
    this->obstacleLines.push_back(Segment(124.0f, 46.0f, 121.0f, 49.0f)); //R1
    this->obstacleLines.push_back(Segment(121.0f, 49.0f, 118.0f, 46.0f)); //R2

    this->obstacleLines.push_back(Segment(124.0f, 98.0f, 121.0f, 95.0f)); //R3
    this->obstacleLines.push_back(Segment(121.0f, 95.0f, 118.0f, 98.0f)); //R4

    //Middle Supports
    this->obstacleLines.push_back(Segment(64.75f, 72.0f, 69.5f, 72.0f)); //L9
    this->obstacleLines.push_back(Segment(74.5f, 72.0f, 79.25f, 72.0f)); //L10

    this->obstacleLines.push_back(Segment(72.0f, 64.75f, 72.0f, 69.5f)); //L11
    this->obstacleLines.push_back(Segment(72.5f, 74.5f, 72.0f, 79.25f)); //L12

    //Middle Goal
    this->obstacleLines.push_back(Segment(62.5f, 79.0f, 65.0f, 81.5f)); //L13
    this->obstacleLines.push_back(Segment(81.5f, 65.0f, 79.0f, 62.5f)); //L14

    

    this -> resample_counter = 0; //start the resample counter at 0

}

bool monte_carlo::out_of_field(Particle p) { ///checks if a particle is inside the field boundaries
    return !(p.position.x >= 0 && p.position.x <= 144 and p.position.y >= 0 && p.position.y <= 144);
}

void monte_carlo::stratified_resample() {
    int N = std::distance(particles.begin(), particles.end());
    if (N == 0) return;

    std::vector<float> cumulative(N);
    float sum = 0.0f;
    int i = 0;
    for (const auto& p : particles) {
        sum += p.weight;
        cumulative[i++] = sum;
    }

    if (sum <= 1e-8f) return;

    static std::default_random_engine rng(std::random_device{}());
    std::uniform_real_distribution<float> uniform(0.0f, 1.0f / N);

    float step = 1.0f / N;
    float u = uniform(rng);

    std::vector<Particle> new_particles;
    int index = 0;

    std::normal_distribution<float> jitter_pos(0.0f, 0.5f);     //Jitter in inches
    std::normal_distribution<float> jitter_heading(0.0f, 2.0f); //Jitter in degrees

    for (int j = 0; j < N; ++j) {
        float threshold = u + j * step;
        while (index < N - 1 && cumulative[index] < threshold) {
            ++index;
        }

        if (index >= N) index = N - 1;

        auto it = particles.begin();
        std::advance(it, index);
        Particle sampled = *it;

        //Add jitter
        sampled.position.x += jitter_pos(rng);
        sampled.position.y += jitter_pos(rng);
        sampled.heading_deg += jitter_heading(rng);
        sampled.weight = 1.0f / N;

        new_particles.push_back(sampled);
    }

    particles.assign(new_particles.begin(), new_particles.end());
}





void monte_carlo::create_Particles(int number, bool in_Field) { //creates a certain number of particles that can either be inside the field, or a field's room around the bot's position
  static std::default_random_engine rng(std::random_device{}());
  float minusRange = 0;
  float plusRange = 144;
  if (!in_Field){
    //float averagePositionofXY = (chassis.get_X_position() + chassis.get_Y_position())/2; //odom
    float averagePositionofXY = (gps_system.get_X() + gps_system.get_Y())/2; //odom
    minusRange = averagePositionofXY - 72.0;
    plusRange = averagePositionofXY + 72.0;
  }
  std::uniform_real_distribution<float> dist(minusRange, plusRange); //Distribute the particles in a field size range around the robot (so that when the ones that are out of the field get put back later, more particles  will end up closer to the robot in total)
  std::uniform_real_distribution<float> heading_dist(0.0f, 360.0f); //Distribute the particles around with different headings

  while (number > 0) {
    Vector2f pos(dist(rng), dist(rng));
    float heading = heading_dist(rng);
    Particle add(pos, heading);
    particles.push_front(add);
    number--;
  }
}

void monte_carlo::update_Particles(std::default_random_engine& rng, std::uniform_real_distribution<float>& avgDistribution, std::uniform_real_distribution<float>& angleDistribution) {
  for (auto it = particles.begin(); it != particles.end(); ) {
    float noisy_avg = avgDistribution(rng);
    float noisy_heading = angleDistribution(rng);
  
    it->heading_deg += noisy_heading; //update heading

    Rotation2Df rot(it->heading_deg);
    Vector2f movement = rot(Vector2f(noisy_avg, 0));
    //float total_difference = abs(it->position.x-chassis.get_X_position()) + abs(it->position.y-chassis.get_Y_position()); //MAKE THIS TELEMETRY SOON
    float total_difference = std::abs(it->position.x - gps_system.get_X()) + std::abs(it->position.y - gps_system.get_Y());
    float chance = random_integer(0, total_difference, Brain.timer(msec));
    if (chance > 15){
      it = particles.erase(it);  //erase returns next valid iterator
      create_Particles(1, false);
    }
        
    else {
        it->position.x += movement.x;
        it->position.y += movement.y;
        ++it;
    }
  }
}

void monte_carlo::run_Predictions() { //predicts and moves particles with the desired amount of positional and heading noise
  for (int i = 0; i < 2; i++) {
      static std::default_random_engine rng(std::random_device{}());
      std::uniform_real_distribution<float> dist_x(0.0f, 144.0f);
      std::uniform_real_distribution<float> dist_y(0.0f, 144.0f);
      std::uniform_real_distribution<float> heading(0.0f, 360.0f);
      Vector2f randPos(dist_x(rng), dist_y(rng));
      float randHeading = heading(rng);
      Particle p(randPos, randHeading);
      resample_particle(p);
      auto it = particles.begin();
      std::advance(it, rand() % particles.size());
      *it = p;
    }
  //Change in both sides of the drivetrain since last update
  float leftChange = chassis.get_left_position_in() - this-> last_Left_Drivetrain;
  float rightChange = chassis.get_right_position_in() - this-> last_Right_Drivetrain;

  //update the past inputs with new inputs for drivetrain
  this->last_Left_Drivetrain = chassis.get_left_position_in();
  this->last_Right_Drivetrain = chassis.get_right_position_in();

  auto avg = (leftChange + rightChange) / 2; //NOTE TO SANTI MAKE THIS A SECANT LINE APPROXIMATION
  avg = clamp(avg, -17.5f, 17.5f); // inches per loop cap (makes sure that the thing doens't do some crazy jump if something happens like a robot tilt (which would throw off telem) so things are smooth)
  
  std::uniform_real_distribution<float> avgDistribution
  (avg - avg * this -> drivetrain_noise, //max negative noise
  avg + avg * this -> drivetrain_noise); //max positive noise

  std::uniform_real_distribution<float> angleDistribution //Should double check
  (chassis.get_absolute_heading() - this -> heading_noise, //max negative noise
  chassis.get_absolute_heading() + this -> heading_noise); //max positive noise

  //Create RNG engine
  static std::default_random_engine rng(std::random_device{}());

  //Pass rng AND distributions to update_Particles
  update_Particles(rng, avgDistribution, angleDistribution);

}

//Actually starts the monte_carlo
void monte_carlo::start_monte_carlo() {
  //task::sleep(1000);
  innit();
  telemetry.startDistanceTracking();
  create_Particles(500);
  //task::sleep(100);
  monte_Task = task(monte_carlo_Task, this);
}

//Creates the thread-like-thing that runs the MCL loop alongside stuff like the drive code loop
int monte_carlo::monte_carlo_Task(void* instance) { //some weird stuff (meaning passing the instance as a param) is done here to allow for this static function to run something instance based
  monte_carlo* self = static_cast<monte_carlo*>(instance);
  task::sleep(100);
  self->monte_carlo_loop();
  return 0;
}

//The actual loop for the MCL process:
void monte_carlo::monte_carlo_loop() {

  //TPS STUFF: (to measure how efficently the program is running)
  int tick_count = 0;
  int tps_update_interval = 1000; //in milliseconds
  int last_tps_update = Brain.timer(msec);

  while (true) {
    run_Predictions();

    weigh_particles();

    this-> average_position = average_Particle(); //Calculates and updates the average particle location (stored as an actual particle)
    display_particles(300, 100); //Displays the particles on the screen for easy visualization of particle sizes and diversity

    tick_count++;

    int now = Brain.timer(msec);
    int elapsed = now - last_tps_update;
    if (elapsed >= tps_update_interval) {
      this->tps = tick_count / (elapsed / 1000.0f);  //ticks per second (from now on will be refered to as TPS)
      tick_count = 0;
      last_tps_update = now;
    }


    task::sleep(10);
  }
}

void monte_carlo::calibrate_Odom(){ //doesn't actually get used because it wasn't stable enough
  float Xchange = 0;
  float Ychange = 0;

  //TBH this was a really dumb idea I had at 3am
  //I meant to further flesh this out, as this version was obviously a non-working proof of concept, but then I realized that this just goes against everything that the MCL stands for lmao
  if (gps_system.get_certainty() > 95 and !within_range(gps_system.get_X(), chassis.get_X_position(), 3)){ //if GPS is confident and close to odom than smoothly correct the odom to the GPS
    Xchange = (gps_system.get_X() - chassis.get_X_position()) * 0.05f;
  }
  if (gps_system.get_certainty() > 95 and !within_range(gps_system.get_Y(), chassis.get_Y_position(), 3)){ //if GPS is confident and close to odom than smoothly correct the odom to the GPS
    Ychange = (gps_system.get_Y() - chassis.get_Y_position()) * 0.05f;
  }
  chassis.set_coordinates(chassis.get_X_position() + Xchange, chassis.get_Y_position() + Ychange, chassis.get_absolute_heading());
}

void monte_carlo::display_particles(int origin_X, int origin_Y) {
    Brain.Screen.clearScreen();

    //Field walls for screen
    Brain.Screen.setFillColor(black);
    Brain.Screen.drawRectangle(origin_X, origin_Y, 144, 1);           //Top Wall
    Brain.Screen.drawRectangle(origin_X, origin_Y + 144, 144, 1);     //Bottom Wall
    Brain.Screen.drawRectangle(origin_X, origin_Y, 1, 144);           //Left Wall
    Brain.Screen.drawRectangle(origin_X + 144, origin_Y, 1, 144);     //Right wall

    //Use safe average to avoid division by zero
    float safe_avg = this->average_weight > 1e-8f ? this->average_weight : 1e-8f;

    //Draw average particle
    Brain.Screen.setFillColor(red);
    Brain.Screen.drawCircle(
        average_position.position.x + origin_X, //X
        average_position.position.y + origin_Y, //Y
        8 //radius
    );

    int i = 0;
    for (const auto& particle : particles) {
        if (i++ % 2 != 0) continue; //Skips drawing half the particles for performance

        float ratio = particle.weight / safe_avg;

        //Dramatic visual scaling of size
        float radius = clamp(0.2f * (2.0f + logf(ratio + 1.0f) * 4.0f), 1.0f, 6.0f); //Stops the particles from being invisibly small or mind-bendingly gigantic

        Brain.Screen.setFillColor(white);
        Brain.Screen.drawCircle(
            particle.position.x + origin_X, //X
            particle.position.y + origin_Y, //Y
            radius //radius
        );
    }

}





void monte_carlo::resample_particle(Particle& p) {
    //Moves particles that are outside of the field to a random point inside the field
    static std::default_random_engine rng(std::random_device{}());
    std::uniform_real_distribution<float> dist_x(0.0f, 144.0f);
    std::uniform_real_distribution<float> dist_y(0.0f, 144.0f);
    std::uniform_real_distribution<float> heading(0.0f, 360.0f);

    p.position = Vector2f(dist_x(rng), dist_y(rng));
    p.heading_deg = heading(rng);
    p.weight = 1.0f; //Resets a particle's weight to zero
}


Particle monte_carlo::average_Particle() { //weighted median average of all the particles (mostly, kinda, its lowk complicated)
    std::vector<const Particle*> vp;
    vp.reserve(particles.size());
    float totalW = 0.0f;
    for (const auto& p : particles) { vp.push_back(&p); totalW += p.weight; }
    /*
    if (totalW < 1e-6f) { //would protect the MCL in extreme situations, but I found it just unnecessary so its removed for now
        //Vector2f telem_belief(telemetry.get_X(), telemetry.get_Y());
        Vector2f odom_belief(chassis.get_X_position(), chassis.get_Y_position());
        Vector2f gps_belief(gps_system.get_X(), gps_system.get_Y());
        float alpha = (gps_system.get_certainty() > 80) ? 0.5f : 0.0f;
        //Vector2f blended = telem_belief * (1 - alpha) + gps_belief * alpha;
        Vector2f blended = odom_belief * (1 - alpha) + gps_belief * alpha;
        return Particle(blended, chassis.get_absolute_heading());
    }
    */

    std::sort(vp.begin(), vp.end(),
              [](const Particle* a, const Particle* b){ return a->weight > b->weight; });

    //Pass 1: center from top mass
    const float tau = 0.60f;
    float cum = 0.0f;
    float wsum = 0.0f;
    Vector2f c(0.0f, 0.0f);
    float c_heading = 0.0f; //I will calculate the heading, although I don't use it, this is only internal.
    for (const Particle* pp : vp) {
        if (cum >= tau) break;
        float w = pp->weight;
        if (cum + w > tau) w = tau - cum;
        cum += w;
        c.x += pp->position.x * w;
        c.y += pp->position.y * w;
        c_heading += pp->heading_deg * w;
        wsum += w;
    }
    if (wsum < 1e-6f) {
        Vector2f odom_belief(chassis.get_X_position(), chassis.get_Y_position());
        return Particle(odom_belief, chassis.get_absolute_heading());
    }
    c.x /= wsum; c.y /= wsum;
    float heading_c = c_heading / wsum;

    //Estimate robust scale: weighted median absolute deviation of distances
    std::vector<std::pair<float,float>> dists; dists.reserve(vp.size());
    float sumW = 0.0f;
    for (const Particle* pp : vp) {
        float dx = pp->position.x - c.x;
        float dy = pp->position.y - c.y;
        float d = std::sqrt(dx*dx + dy*dy);
        dists.push_back({d, pp->weight});
        sumW += pp->weight;
    }
    //Weighted median (better than the average from my tests)
    std::sort(dists.begin(), dists.end(),
              [](auto& a, auto& b){ return a.first < b.first; });
    float half = 0.5f * sumW, accW = 0.0f, mad = 0.0f;
    for (auto& pr : dists) { accW += pr.second; if (accW >= half) { mad = pr.first; break; } }
    float radius = std::max(3.0f, 2.0f * mad);  //at least 3", else 2×MAD

    //Pass 2: re-average only particles within radius of c (can re-introduce center bias)
    Vector2f acc(0.0f, 0.0f);
    float acc_h = 0.0f, w_in = 0.0f;
    for (const Particle* pp : vp) {
        float dx = pp->position.x - c.x;
        float dy = pp->position.y - c.y;
        if ((dx*dx + dy*dy) <= radius*radius) {
            acc.x += pp->position.x * pp->weight;
            acc.y += pp->position.y * pp->weight;
            acc_h += pp->heading_deg * pp->weight;
            w_in += pp->weight;
        }
    }
    if (w_in < 1e-6f) return Particle(c, heading_c);

    Vector2f avg(acc.x / w_in, acc.y / w_in);
    float avg_heading = acc_h / w_in;
    return Particle(avg, avg_heading);
}

double minZero(double number){
  if (number<0){return 0;}
  return number;
}


float monte_carlo::weight_particle(Particle p) {
  //GPS Weight:
  //gps_importance = gps_importance * (minZero(gps_system.get_certainty()-97.5)/2.5);

  const float gps_sigma = 2.0f;
  float gps_weight = 1.0f;  //neutral
  float dx_gps = p.position.x - gps_system.get_X();
  float dy_gps = p.position.y - gps_system.get_Y();
  float gps_dist_sq = dx_gps * dx_gps + dy_gps * dy_gps;

  //sanity check vs odom
  float odom_dx = chassis.get_X_position() - gps_system.get_X();
  float odom_dy = chassis.get_Y_position() - gps_system.get_Y();
  float odom_dist = sqrtf(odom_dx*odom_dx + odom_dy*odom_dy);

  if (odom_dist < 12.0f) {  //only trust GPS if within 1ft of odom belief
    const float gps_sigma = 2.0f;
    gps_weight = expf(-gps_dist_sq / (2.0f * gps_sigma * gps_sigma));
  }



  //ODOM Weight:
  static Vector2f smoothedOdom = { chassis.get_X_position(), chassis.get_Y_position() };
  smoothedOdom.x = 0.9f * smoothedOdom.x + 0.1f * chassis.get_X_position();
  smoothedOdom.y = 0.9f * smoothedOdom.y + 0.1f * chassis.get_Y_position();
  //then compute dx, dy using smoothedOdom instead of raw chassis pos

  float dx_odom = p.position.x - smoothedOdom.x;
  float dy_odom = p.position.y - smoothedOdom.y;
  float odom_dist_sq = dx_odom * dx_odom + dy_odom * dy_odom;
  //Vector2f combined_belief((gps_system.get_X() + telemetry.get_X())/2,
                         //(gps_system.get_Y() + telemetry.get_Y())/2);
  Vector2f combined_belief((gps_system.get_X()), (gps_system.get_Y()));
  float dx_belief = chassis.get_X_position() - combined_belief.x;
  float dy_belief = chassis.get_Y_position() - combined_belief.y;
  float belief_dist = sqrtf(dx_belief*dx_belief + dy_belief*dy_belief);

  //Scale trust between 0.0 and 1.0 with a smooth curve
  //float odom_trust = expf(-(belief_dist*belief_dist)/(2*12.0f*12.0f)); 
  float odom_trust = 1;
  // near → ~1, far → ~0

  //Apply odom_importance * odom_trust instead of fixed power
  float odom_weight = expf(-odom_dist_sq / (2.0f * 6.0f * 6.0f));
  odom_weight = powf(odom_weight, odom_importance * odom_trust);
  odom_weight = std::min(odom_weight, 0.4f); // never let odom > 40% of total weight
  if (!std::isfinite(odom_weight)) odom_weight = 1e-5f;

  //TELEM Weight:


  ExpectedSensorReadings expected = telemetry.expected_readings_for_particle(p, obstacleLines); //get the expected sensor readings for this particle

  float measured[4] = {
    telemetry.distanceFrontReading,
    telemetry.distanceBackReading,
    telemetry.distanceLeftReading,
    telemetry.distanceRightReading
  }; //the 4 measured values

  float expectedVals[4] = {
    expected.front,
    expected.back,
    expected.left,
    expected.right}; //the 4 expected values

  const float sensor_sigma = 6.0f; //how sharp the weighing is going to be
  float log_sum = 0.0f;
  int count = 0;

  for (int i = 0; i < 4; i++) {
    if (measured[i] > 1 && measured[i] < 390){ //only calculate difference if reading is valid
      if (expectedVals[i] < 390){
        float diff = expectedVals[i] - measured[i];
        log_sum += -(diff * diff) / (2.0f * sensor_sigma * sensor_sigma);
        count++;
      } //calculate the difference between the two
    }
    if (expectedVals[i] > 390 && measured[i] > 390){
      float diff = 0; //no difference they are both invalid (this is good for particle, so give it some weight for this)
      log_sum += -(diff * diff) / (2.0f * sensor_sigma * sensor_sigma);
      count++;
    }
  }

  float telem_weight = (count > 0) ? expf(log_sum / count) : 1.0f; //the total weight provided by the telemetry
  if (!std::isfinite(telem_weight)) telem_weight = 1e-5f; //guard rail to protect against ridiculously high weight (maybe all readings were invalid for both or smth)

  //Total Weight:
  float total_weight = (
    gps_importance * gps_weight +
    telemetry_importance * telem_weight +
    odom_importance * odom_weight
) / (gps_importance + telemetry_importance + odom_importance);

  if (!std::isfinite(total_weight)) total_weight = 1e-5f;

  return total_weight;
}






void monte_carlo::weigh_particles() {
  float total_weight = 0.0f;

  if (total_weight > 0.01f && resample_counter % 10 == 0) {
    //If we are moving, resample the particles
    if (chassis.Gyro.acceleration(xaxis) > 0.05 or chassis.Gyro.acceleration(xaxis) > 0.05 or chassis.Gyro.acceleration(xaxis) > 0.05){
      stratified_resample();
    }
    //add in 2 completely random particles for diversity's sake
    for (int i = 0; i < 3; i++) {
      static std::default_random_engine rng(std::random_device{}());
      std::uniform_real_distribution<float> dist_x(0.0f, 144.0f);
      std::uniform_real_distribution<float> dist_y(0.0f, 144.0f);
      std::uniform_real_distribution<float> heading(0.0f, 360.0f);
      Vector2f randPos(dist_x(rng), dist_y(rng));
      float randHeading = heading(rng);
      Particle p(randPos, randHeading);
      resample_particle(p);
      auto it = particles.begin();
      std::advance(it, rand() % particles.size());
      *it = p;
    }
  }

  for (auto& p : particles) {
    p.weight = weight_particle(p);

    //Avoid the three horsemen of these types of errors: NaN, inf, or negative values
    if (!std::isfinite(p.weight) || p.weight < 0.0f || p.weight > 1e6f) {
      p.weight = 0.0f;
    }

    //Replace particle if extremely weak
    if (p.weight < 1e-8f) {
      resample_particle(p);
      p.weight = 0.0f;
    }

    if (out_of_field(p)) {
      resample_particle(p);
    }

    total_weight += p.weight;
  }

  if (total_weight < 1e-8f) total_weight = 1e-8f;

  for (auto& p : particles) {
    p.weight /= total_weight;
  }

  this->average_weight = 1.0f / particles.size();
  this->total_weight = total_weight;

  this -> resample_counter++;
  if (total_weight > 0.01f && this -> resample_counter % 5 == 0) { //resample every 5th loop if we aren't moving, or give an extra resample every 5 loops if we are moving
      stratified_resample();
  }
}
