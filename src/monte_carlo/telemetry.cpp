#include "vex.h"
extern class monte_carlo monte_carlo;

using namespace vex;

Telem::Telem(float heightLevel, //average height level of the distance sensors (should have a +- of no more than an inch)
  int distanceLeftPort, float distanceLeftCenterDistance, float distanceLeftCenterOffset, //the port of the sensor, and then its offset from both the horizontal, and vertical centers of the bot
  int distanceRightPort, float distanceRightCenterDistance, float distanceRightCenterOffset, //the port of the sensor, and then its offset from both the horizontal, and vertical centers of the bot
  int distanceFrontPort, float distanceFrontCenterDistance, float distanceFrontCenterOffset, //the port of the sensor, and then its offset from both the horizontal, and vertical centers of the bot
  int distanceBackPort, float distanceBackCenterDistance, float distanceBackCenterOffset): //the port of the sensor, and then its offset from both the horizontal, and vertical centers of the bot
  
  heightLevel(heightLevel),

  distanceLeft(distanceLeftPort),
  distanceLeftCenterDistance(distanceLeftCenterDistance),
  distanceLeftCenterOffset(distanceLeftCenterOffset),

  distanceRight(distanceRightPort),
  distanceRightCenterDistance(distanceRightCenterDistance),
  distanceRightCenterOffset(distanceRightCenterOffset),

  distanceFront(distanceFrontPort),
  distanceFrontCenterDistance(distanceFrontCenterDistance),
  distanceFrontCenterOffset(distanceFrontCenterOffset),

  distanceBack(distanceBackPort),
  distanceBackCenterDistance(distanceBackCenterDistance),
  distanceBackCenterOffset(distanceBackCenterOffset),

  //Attempt to create points for all the sensor on the robot from a 2D top down perspective
  //The center of the robot is 0,0

  
  distanceLeftX(distanceLeftCenterDistance),
  distanceLeftY(distanceLeftCenterOffset),

  distanceRightX(distanceRightCenterDistance),
  distanceRightY(distanceRightCenterOffset),

  distanceFrontX(distanceFrontCenterOffset),
  distanceFrontY(distanceFrontCenterDistance),

  distanceBackX(distanceBackCenterOffset),
  distanceBackY(distanceBackCenterDistance),

  beliefX(0),
  beliefY(0)

{};

bool Telem::intersectsSegment(const Vector2f& p1, const Vector2f& p2, const Segment& line, Vector2f& intersection) {
  Vector2f d1 = p2 - p1;
  Vector2f d2(line.x2 - line.x1, line.y2 - line.y1);
  float denom = d1.x * d2.y - d1.y * d2.x;
  if (std::abs(denom) < 1e-5f) return false;

  float dx = line.x1 - p1.x;
  float dy = line.y1 - p1.y;
  float t = (dx * d2.y - dy * d2.x) / denom;
  float u = (dx * d1.y - dy * d1.x) / denom;

  if (t >= 0.0f && t <= 1.0f && u >= 0.0f && u <= 1.0f) {
    intersection = p1 + d1 * t;
    return true;
  }
  return false;
}


//At a given pose (X, Y, Heading), returns the sesnor value
float Telem::predictSensorReading(float x, float y, float heading_deg, std::string sensorName) {
    float WALL_LEFT_X   = 0.0f;
    float WALL_RIGHT_X  = 144.0f;
    float WALL_BOTTOM_Y = 0.0f;
    float WALL_TOP_Y    = 144.0f;

    float predicted = 50.0f;  // default max cap
    float angle_rad = heading_deg * M_PI / 180.0f;

    if (sensorName == "front") {
        if (std::cos(angle_rad) > 0.001f)
            predicted = std::min(predicted, (WALL_TOP_Y - y) / std::cos(angle_rad));
    } else if (sensorName == "back") {
        if (std::cos(angle_rad) < -0.001f)
            predicted = std::min(predicted, (y - WALL_BOTTOM_Y) / -std::cos(angle_rad));
    } else if (sensorName == "left") {
        if (std::sin(angle_rad) < -0.001f)
            predicted = std::min(predicted, (x - WALL_LEFT_X) / -std::sin(angle_rad));
    } else if (sensorName == "right") {
        if (std::sin(angle_rad) > 0.001f)
            predicted = std::min(predicted, (WALL_RIGHT_X - x) / std::sin(angle_rad));
    }

    return predicted;
}


// Helpers
static inline float deg2rad(float d) { return d * (M_PI / 180.0f); }

//Rotate a local vector (lx, ly) by heading_deg into world
static inline Vector2f rot_local(float lx, float ly, float heading_deg) {
  float r = deg2rad(heading_deg);
  float c = cosf(r), s = sinf(r);
  return Vector2f(lx * c - ly * s, lx * s + ly * c);
}

static inline float cross(const Vector2f& a, const Vector2f& b){ return a.x*b.y - a.y*b.x; }

//Raycast from origin o along direction d to axis-aligned walls of a 144x144 field
//Returns the smallest positive distance (inches); huge value if no hit.
static inline float ray_to_walls(const Vector2f& o, const Vector2f& d) {
  const float Xmin = 0.0f, Xmax = 144.0f, Ymin = 0.0f, Ymax = 144.0f;
  float best = 1e9f;
  //x = Xmin
  if (fabsf(d.x) > 1e-6f) {
    float t = (Xmin - o.x) / d.x;
    if (t > 0) {
      float y = o.y + t * d.y;
      if (y >= Ymin - 1e-4f && y <= Ymax + 1e-4f) best = fminf(best, t);
    }
    //x = Xmax
    t = (Xmax - o.x) / d.x;
    if (t > 0) {
      float y = o.y + t * d.y;
      if (y >= Ymin - 1e-4f && y <= Ymax + 1e-4f) best = fminf(best, t);
    }
  }
  //y = Ymin
  if (fabsf(d.y) > 1e-6f) {
    float t = (Ymin - o.y) / d.y;
    if (t > 0) {
      float x = o.x + t * d.x;
      if (x >= Xmin - 1e-4f && x <= Xmax + 1e-4f) best = fminf(best, t);
    }
    //y = Ymax
    t = (Ymax - o.y) / d.y;
    if (t > 0) {
      float x = o.x + t * d.x;
      if (x >= Xmin - 1e-4f && x <= Xmax + 1e-4f) best = fminf(best, t);
    }
  }
  return best;
}

//Raycast to ANY internal segment (generic line with two endpoints)
static inline float ray_to_segments(const Vector2f& o, const Vector2f& d, const std::vector<Segment>& obstacles) {
  float best = 1e9f;
  for (const auto& seg : obstacles) {
    Vector2f p(seg.x1, seg.y1);
    Vector2f q(seg.x2, seg.y2);
    Vector2f s = q - p;

    float denom = cross(d, s);
    if (fabsf(denom) < 1e-6f) continue; //parallel

    Vector2f po = p - o;
    float t = cross(po, s) / denom; //ray parameter
    float u = cross(po, d) / denom; //segment parameter

    if (t > 0.0f && u >= 0.0f && u <= 1.0f) {
      best = fminf(best, t);
    }
  }
  return best;
}

//Returns the expected telemetry sensor readings for any given particle (which has an x, y and a heading)
//Used to compare actual readings for the bot with particle's expected readings, which gets fed to the weighing system
ExpectedSensorReadings Telem::expected_readings_for_particle(const Particle& p, const std::vector<Segment>& obstacles){
  //Robot heading
  float heading = reduce_0_to_360(p.heading_deg);

  //How to calculate the position of a sensor:
  //#1: Start at the origin of the robot, which is the x and y positions of our hypothetical particle
  //#2: Take the vector of the x and y offsets, and rotate it to the heading of the bot
  //#3: Add the rotated offset to the origin
  Vector2f leftO  = p.position + rot_local(distanceLeftX,  distanceLeftY,  heading);
  Vector2f rightO = p.position + rot_local(distanceRightX, distanceRightY, heading);
  Vector2f frontO = p.position + rot_local(distanceFrontX, distanceFrontY, heading);
  Vector2f backO  = p.position - rot_local(distanceBackX,  distanceBackY,  heading);

  //#4 Rotate the offset sensor around the bot to the correct side.
  Vector2f dFront = rot_local(0.0f,  1.0f, heading);
  Vector2f dBack  = rot_local(0.0f, -1.0f, heading);
  Vector2f dLeft  = rot_local(-1.0f, 0.0f, heading);
  Vector2f dRight = rot_local( 1.0f, 0.0f, heading);

  //Ta-dah! You're done!


  //Predict a sensor output
  auto predict_one = [&](const Vector2f& o, const Vector2f& d) -> float {
    float tWalls = ray_to_walls(o, d);
    float tDiag  = ray_to_segments(o, d, obstacles);
    float t = fminf(tWalls, tDiag);
    return (t < 1e8f) ? t : 393.661f; //cap at 393.661, as it is the max range for reliable sensor output, and if it goes past that, the sensor will just return the 393.661
  };

  return {
    predict_one(frontO, dFront),
    predict_one(backO,  dBack),
    predict_one(leftO,  dLeft),
    predict_one(rightO, dRight)
  };
}



// Add these members to Telem (e.g., in Telem.h)

float lastLeft = 0, lastRight = 0, lastFront = 0, lastBack = 0;

static inline bool telemValid(float d) { return d > 1 && d < 390; }

void Telem::updateReadings(){
  float L = distanceLeft.objectDistance(inches);
  float R = distanceRight.objectDistance(inches);
  float F = distanceFront.objectDistance(inches);
  float B = distanceBack.objectDistance(inches);

  if (telemValid(L)) { distanceLeftReading  = lastLeft  = L; }
  else               { distanceLeftReading  = lastLeft;      }

  if (telemValid(R)) { distanceRightReading = lastRight = R; }
  else               { distanceRightReading = lastRight;     }

  if (telemValid(F)) { distanceFrontReading = lastFront = F; }
  else               { distanceFrontReading = lastFront;     }

  if (telemValid(B)) { distanceBackReading  = lastBack  = B; }
  else               { distanceBackReading  = lastBack;      }
}




static inline float clampf(float v, float lo, float hi){ return v < lo ? lo : (v > hi ? hi : v); }

void Telem::calculateDistances(float /*xPosition*/, float /*yPosition*/,
                               const std::vector<Segment>& obstacleLines) {
  //Tunables:
  const float XMIN = 0.0f, XMAX = 144.0f, YMIN = 0.0f, YMAX = 144.0f;
  const float MIN_COMP = 0.10f;   // require at least this |component| to use a sensor
  const float LPF      = 0.30f;   // low-pass factor (0.2–0.35 good)
  const float STEP_CAP = 1.0f;    // max inches per update (pre-LPF)

  //Robot pose from odometry readings
  Vector2f robotPos(chassis.get_X_position(), chassis.get_Y_position());
  float heading = reduce_0_to_360(chassis.get_absolute_heading());

  //Create and rotate sensor offsets with heading of robot
  Vector2f offL  = rot_local(distanceLeftX,  distanceLeftY,  heading);
  Vector2f offR  = rot_local(distanceRightX, distanceRightY, heading);
  Vector2f offF  = rot_local(distanceFrontX, distanceFrontY, heading);
  Vector2f offB  = rot_local(distanceBackX,  distanceBackY,  heading);

  //Rotate the sensor offsets to match their respective sides of the robot
  Vector2f dL = rot_local(-1.0f,  0.0f, heading);
  Vector2f dR = rot_local( 1.0f,  0.0f, heading);
  Vector2f dF = rot_local( 0.0f,  1.0f, heading);
  Vector2f dB = rot_local( 0.0f, -1.0f, heading);

  //X Calculations From Left Right Sensors:
  float xNum = 0.0f, xDen = 0.0f;

  if (telemValid(distanceLeftReading) && fabsf(dL.x) >= MIN_COMP) {
    float Xw = (dL.x >= 0.0f) ? XMAX : XMIN;                 //wall in ray direction
    float xL = (Xw - distanceLeftReading * dL.x) - offL.x;   //back-project to center X
    float wL = fabsf(dL.x);                                  //more perpendicular => higher weight
    xNum += wL * xL; xDen += wL;
  }

  if (telemValid(distanceRightReading) && fabsf(dR.x) >= MIN_COMP) {
    float Xw = (dR.x >= 0.0f) ? XMAX : XMIN;
    float xR = (Xw - distanceRightReading * dR.x) - offR.x;
    float wR = fabsf(dR.x);
    xNum += wR * xR; xDen += wR;
  }

  if (xDen > 1e-6f) {
    float xNew = xNum / xDen;
    xNew = clampf(xNew, XMIN, XMAX);
    float dx = xNew - beliefX;
    if (fabsf(dx) > STEP_CAP) xNew = beliefX + (dx > 0 ? STEP_CAP : -STEP_CAP);
    beliefX = (1.0f - LPF) * beliefX + LPF * xNew;
  }

  //Y From new Back/Front Sensors
  float yNum = 0.0f, yDen = 0.0f;

  if (telemValid(distanceBackReading) && fabsf(dB.y) >= MIN_COMP) {
    float Yw = (dB.y >= 0.0f) ? YMAX : YMIN;
    float yB = (Yw - distanceBackReading * dB.y) - offB.y;
    float wB = fabsf(dB.y);
    yNum += wB * yB; yDen += wB;
  }

  if (telemValid(distanceFrontReading) && fabsf(dF.y) >= MIN_COMP) {
    float Yw = (dF.y >= 0.0f) ? YMAX : YMIN;
    float yF = (Yw - distanceFrontReading * dF.y) - offF.y;
    float wF = fabsf(dF.y);
    yNum += wF * yF; yDen += wF;
  }

  if (yDen > 1e-6f) {
    float yNew = yNum / yDen;
    yNew = clampf(yNew, YMIN, YMAX);
    float dy = yNew - beliefY;
    if (fabsf(dy) > STEP_CAP) yNew = beliefY + (dy > 0 ? STEP_CAP : -STEP_CAP);
    beliefY = (1.0f - LPF) * beliefY + LPF * yNew;
  }

  //Closest-hit override using internal segments:
  
  //Create sensor origins in world
  Vector2f leftOrigin  = robotPos + offL;
  Vector2f rightOrigin = robotPos + offR;
  Vector2f frontOrigin = robotPos + offF;
  Vector2f backOrigin  = robotPos + offB;

  //Endpoints at the *measured* hit locations along each ray (only for valid readings)
  Vector2f leftImpact  = leftOrigin  + dL * (telemValid(distanceLeftReading)  ? distanceLeftReading  : 0.0f);
  Vector2f rightImpact = rightOrigin + dR * (telemValid(distanceRightReading) ? distanceRightReading : 0.0f);
  Vector2f frontImpact = frontOrigin + dF * (telemValid(distanceFrontReading) ? distanceFrontReading : 0.0f);
  Vector2f backImpact  = backOrigin  + dB * (telemValid(distanceBackReading)  ? distanceBackReading  : 0.0f);

  //Collect only rays with valid distances
  std::vector<std::pair<Vector2f, Vector2f>> rays;
  if (telemValid(distanceLeftReading))  rays.push_back({leftOrigin,  leftImpact});
  if (telemValid(distanceRightReading)) rays.push_back({rightOrigin, rightImpact});
  if (telemValid(distanceFrontReading)) rays.push_back({frontOrigin, frontImpact});
  if (telemValid(distanceBackReading))  rays.push_back({backOrigin,  backImpact});

  float closestDistSq = 1e9f;
  Vector2f closestImpact = Vector2f(beliefX, beliefY);

  for (const auto& ray : rays) {
    for (const auto& seg : obstacleLines) {
      Vector2f hit;
      if (intersectsSegment(ray.first, ray.second, seg, hit)) {
        float dx = hit.x - ray.first.x;
        float dy = hit.y - ray.first.y;
        float d2 = dx*dx + dy*dy;
        if (d2 < closestDistSq) {
          closestDistSq = d2;
          closestImpact = hit;
        }
      }
    }
  }

  if (closestDistSq < 1e9f) {
    //blend a bit to avoid sudden jumps
    const float OVERRIDE_LPF = 0.5f; //0.3–0.6 is reasonable
    beliefX = (1.0f - OVERRIDE_LPF) * beliefX + OVERRIDE_LPF * closestImpact.x;
    beliefY = (1.0f - OVERRIDE_LPF) * beliefY + OVERRIDE_LPF * closestImpact.y;
  }

  //Done!
  impactX = beliefX;
  impactY = beliefY;
}


























void Telem::updatePositionBelief(int times, const std::vector<Segment>& obstacles) {
  float sumX = 0.0f, sumY = 0.0f;
  int count = 0;

  while (times-- > 0) {
    updateReadings();

    // skip if all four are invalid this sub-iteration
    bool anyValid =
      telemValid(distanceLeftReading)  ||
      telemValid(distanceRightReading) ||
      telemValid(distanceFrontReading) ||
      telemValid(distanceBackReading);

    if (!anyValid) continue;

    calculateDistances(chassis.get_X_position(), chassis.get_Y_position(), obstacles);
    sumX += beliefX;
    sumY += beliefY;
    ++count;
  }

  if (count > 0) {
    // Low-pass to calm jitter (α ~ 0.2 works well)
    float newAvgX = sumX / count;
    float newAvgY = sumY / count;
    averaged_beliefX = 0.8f * averaged_beliefX + 0.2f * newAvgX;
    averaged_beliefY = 0.8f * averaged_beliefY + 0.2f * newAvgY;
  }
  // else: keep previous averaged_beliefX/Y
}

void Telem::startDistanceTracking() { //the function that starts the telemetry position tracking
  averaged_beliefX = chassis.get_X_position();
  averaged_beliefY = chassis.get_Y_position();
  telem_task = task(&Telem::distanceTrackTask, this); //creates the thread
}

void distanceTrack(Telem* telemetry){ //the actual loop that runs the telemetry tracking
  while (1) {
    telemetry->updatePositionBelief(3, monte_carlo.obstacleLines);
    task::sleep(5);
  }
}

int Telem::distanceTrackTask(void *param) { //the thread that can be created to run the loop
  Telem* telemetry = static_cast<Telem*>(param);
  ::distanceTrack(telemetry);
  return 0;
}



float Telem::get_X(){
  return this-> averaged_beliefX;
}
float Telem::get_Y(){
  return this-> averaged_beliefY;
}

void Telem::increase_odom_X(float value){

  float constant = 0;

  if (value <= chassis.get_X_position()){
    constant = ((chassis.get_X_position() / (value))/100)  * chassis.get_X_position();
  }
  else if (value > chassis.get_X_position()){
    constant = -((value / (chassis.get_Y_position()))/100)  * chassis.get_X_position();
  }

  

  chassis.set_coordinates(chassis.get_X_position() + constant, chassis.get_Y_position(), chassis.get_absolute_heading());

}

void Telem::increase_odom_Y(float value){
  float constant = 0;

  if (value <= chassis.get_Y_position()){
    constant = ((chassis.get_Y_position() / (value))/100)  * chassis.get_Y_position();
  }
  else if (value > chassis.get_Y_position()){
    constant = -((value / (chassis.get_Y_position()))/100)  * chassis.get_Y_position();
  }

  

  chassis.set_coordinates(chassis.get_X_position(), chassis.get_Y_position() + constant, chassis.get_absolute_heading());
}