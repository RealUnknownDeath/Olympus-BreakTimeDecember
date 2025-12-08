// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// LeftMotor1           motor         8               
// LeftMotor2           motor         9               
// LeftMotor3           motor         10              
// RightMotor1          motor         1               
// RightMotor2          motor         2               
// RightMotor3          motor         3               
// Controller1          controller                    
// Inertial             inertial      20              
// intakeMotor          motor         14              
// middleTravel         motor         13              
// scoreMotor           motor         15              
// outtakeRaise         digital_out   C               
// Optical21            optical       21              
// descorePiston        digital_out   E               
// matchLoadPiston      digital_out   F               
// DistanceRight        distance      5               
// DistanceBack         distance      19              
// DistanceLeft         distance      6               
// DistanceFront        distance      18              
// LineTracker          line          B               
// GPS7                 gps           7               
// Aligner              digital_out   H               
// odom                 rotation      4               
// MatchloadSensor      optical       17              
// descoreLift          digital_out   D               
// ---- END VEXCODE CONFIGURED DEVICES ----
// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// LeftMotor1           motor         8               
// LeftMotor2           motor         9               
// LeftMotor3           motor         10              
// RightMotor1          motor         1               
// RightMotor2          motor         2               
// RightMotor3          motor         3               
// Controller1          controller                    
// Inertial             inertial      20              
// intakeMotor          motor         14              
// middleTravel         motor         13              
// scoreMotor           motor         15              
// outtakeRaise         digital_out   C               
// Optical21            optical       21              
// descorePiston        digital_out   G               
// matchLoadPiston      digital_out   F               
// DistanceRight        distance      5               
// DistanceBack         distance      19              
// DistanceLeft         distance      6               
// DistanceFront        distance      18              
// LineTracker          line          B               
// GPS7                 gps           7               
// Aligner              digital_out   H               
// odom                 rotation      4               
// MatchloadSensor      optical       17              
// descoreLift          digital_out   D               
// ---- END VEXCODE CONFIGURED DEVICES ----
// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// LeftMotor1           motor         8               
// LeftMotor2           motor         9               
// LeftMotor3           motor         10              
// RightMotor1          motor         1               
// RightMotor2          motor         2               
// RightMotor3          motor         3               
// Controller1          controller                    
// Inertial             inertial      20              
// intakeMotor          motor         14              
// middleTravel         motor         13              
// scoreMotor           motor         15              
// outtakeRaise         digital_out   C               
// Optical21            optical       21              
// descorePiston        digital_out   G               
// matchLoadPiston      digital_out   F               
// DistanceRight        distance      5               
// DistanceBack         distance      19              
// DistanceLeft         distance      6               
// DistanceFront        distance      18              
// LineTracker          line          B               
// GPS7                 gps           7               
// Aligner              digital_out   H               
// odom                 rotation      4               
// MatchloadSensor      optical       17              
// descoreUp            digital_out   D               
// ---- END VEXCODE CONFIGURED DEVICES ----
// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// LeftMotor1           motor         8               
// LeftMotor2           motor         9               
// LeftMotor3           motor         10              
// RightMotor1          motor         1               
// RightMotor2          motor         2               
// RightMotor3          motor         3               
// Controller1          controller                    
// Inertial             inertial      20              
// intakeMotor          motor         14              
// middleTravel         motor         13              
// scoreMotor           motor         15              
// outtakeRaise         digital_out   C               
// Optical21            optical       21              
// descorePiston        digital_out   G               
// matchLoadPiston      digital_out   F               
// DistanceRight        distance      5               
// DistanceBack         distance      19              
// DistanceLeft         distance      6               
// DistanceFront        distance      18              
// LineTracker          line          B               
// GPS7                 gps           7               
// Aligner              digital_out   H               
// odom                 rotation      4               
// MatchloadSensor      optical       17              
// ---- END VEXCODE CONFIGURED DEVICES ----
// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// LeftMotor1           motor         8               
// LeftMotor2           motor         9               
// LeftMotor3           motor         10              
// RightMotor1          motor         1               
// RightMotor2          motor         2               
// RightMotor3          motor         3               
// Controller1          controller                    
// Inertial             inertial      20              
// intakeMotor          motor         14              
// middleTravel         motor         13              
// scoreMotor           motor         15              
// outtakeRaise         digital_out   C               
// Optical21            optical       21              
// descorePiston        digital_out   G               
// matchLoadPiston      digital_out   A               
// DistanceRight        distance      5               
// DistanceBack         distance      19              
// DistanceLeft         distance      6               
// DistanceFront        distance      18              
// LineTracker          line          B               
// GPS7                 gps           7               
// Aligner              digital_out   H               
// odom                 rotation      4               
// MatchloadSensor      optical       17              
// ---- END VEXCODE CONFIGURED DEVICES ----
#include "vex.h"
#include "globals.h"
#include "autons.h"
#include "AutonUI.h"
#include "AutonRegistry.h"
#include "AutonConfig.h"

//Hi Gavin!
//wassup santi
//wsg

using namespace vex;
competition Competition;

bool REDTeam;
bool auton_running = true;

class monte_carlo monte_carlo(

  2.5f, //gps importance
  
  1.5f, //telemetry importance

  3.5f, //odom importance

  //Drivetrain config

  0.0125, //drivetrain noise (%)
  0.125, //heading noise (degrees)

  //GPS CONFIG

  PORT7, //gps port (PORT#)
  6.5, //gps x offset (inches)
  -1, //gps y offset (inches)
  90, //gps orientation (degrees)


  //TELEMETRY CONFIG:

  //Height of the sensors: (all should be the same)
  7,

  PORT6, // left port 
  -6, // X
  -5.35, // Y

  PORT4, // right port 
  6, // X
  -5.35, // Y

  PORT18, // front port 
  4, // X
  8, // Y

  PORT19, // back port
  -4, // X
  -5.125 //Y

);


ControllerDisplay controllerDisplay;
Drive chassis(


//what type of odom setup we are running
TANK_ONE_SIDEWAYS_ROTATION,

//Left Motors:
motor_group(LeftMotor1, LeftMotor2, LeftMotor3),

//Right Motors:
motor_group(RightMotor1, RightMotor2, RightMotor3),

//Inertial Port
PORT20,

//Wheel Diameter
3.25,

//Gear Ratio
0.75,

//Inertial scalar
354.25,

//Holonomic:
//LF:      //RF:    
PORT1,     -PORT2,

//LB:      //RB: 
PORT3,     -PORT4,

//vertical odom wheel port
3,

//Vertical odom wheel diameter (-# makes direction switch)
2.75,

//Vertical odom wheel center distance
5.7,

//Sideways tracker port:
PORT4,

//Sideways tracker diameter (-# makes direction switch):
-2,

//Sideways tracker center distance (positive distance is behind center, negative is in front):
1.55

);

CsvLogger telem_logger;
#include <vector>
#include <string>
#include <cstdio>
#include <vector>
#include <string>
#include <cstdio>   // for snprintf

// Append one CSV line to telem_irl_data.csv on the SD card.
// Creates the file with header if it doesn't exist yet.
static bool append_telem_csv_line(const std::string& line) {
  const char* kName = "inertial_chart.csv";
  if (!Brain.SDcard.isInserted()) return false;

  // If missing, create with header.
  if (!Brain.SDcard.exists(kName)) {
    const char header[] =
      "t_ms,heading,pitch,roll\n";
    // make a writable buffer (non-const)
    std::vector<uint8_t> hbuf(header, header + sizeof(header) - 1);
    if (!Brain.SDcard.savefile(kName, hbuf.data(), (int)hbuf.size())) return false;
  }

  // Load existing file (may be empty)
  const int sz = Brain.SDcard.size(kName);
  std::vector<uint8_t> existing;
  if (sz > 0) {
    existing.resize(sz);
    if (!Brain.SDcard.loadfile(kName, existing.data(), sz)) return false;
  }

  // Build new content = existing + new line
  std::vector<uint8_t> out;
  out.reserve(sz + (int)line.size());
  if (sz > 0) out.insert(out.end(), existing.begin(), existing.end());
  out.insert(out.end(), line.begin(), line.end());

  // Overwrite file with appended content
  return Brain.SDcard.savefile(kName, out.data(), (int)out.size());
}

void log_snapshot_immediate() {
  if (!Brain.SDcard.isInserted()) {
    Controller1.Screen.clearScreen();
    Controller1.Screen.setCursor(1, 1);
    Controller1.Screen.print("Insert SD card");
    
    return;
  }

  const uint32_t t = Brain.timer(msec);

  double heading = chassis.get_absolute_heading();
  if(heading > 180){
    heading = 0-(360-heading);
  }
  double pitch   = Inertial.pitch();
  if(pitch > 180){
    pitch = 0-(360-pitch);
  }
  double roll   = Inertial.roll();
  if(roll > 180){
    roll = 0-(360-roll);
  }

  // Format CSV line (use ::snprintf, not std::snprintf)
  char line[160];
  ::snprintf(line, sizeof(line),
             "%u,%.3f,%.3f,%.3f\n",
             (unsigned)t, heading, pitch, roll);

  const bool ok = append_telem_csv_line(std::string(line));
  if (ok) {
  } else {
    Controller1.rumble("-");
    Controller1.Screen.clearScreen();
    Controller1.Screen.setCursor(1, 1);
    Controller1.Screen.print("CSV append failed");
  }
}

int current_auton_selection = 0;
int current_color_selection = 0; //0 = red, 1 = blue, 2 = no color
bool auton_confirmed = false;


bool colorSelected = false;
bool runProgram = false;

void autonConfirm() {
    if(!colorSelected){
      colorSelected = true;
    }
    else{
      auton_confirmed = true;
    }
    
}
std::vector<std::string> colorOptions = {
  "Color --> Red",
  "Color --> Blue",
  "Colorsort --> Off"
};
std::vector<std::string> autonNames = {
  "Left 6+1 Quals",
  "Right 6+1 Quals",
  "Left 9 Elims",
  "Right 9 Elims"
};
bool auto_started = false;

void toggleDisplay(){
  controllerDisplay.toggle();
}
void scrollDown(){
  if(!colorSelected){
    colorSelected -= 1;
  }
  else{
    controllerDisplay.scrollDown();
    current_auton_selection -= 1;
    if (current_auton_selection > 0) current_auton_selection--;
  }
}
void scrollUp(){
  if(!colorSelected){
    colorSelected += 1;
  }
  else{
    controllerDisplay.scrollUp();
    current_auton_selection +=1;
    if (current_auton_selection < current_auton_selection) current_auton_selection++;
  }
  
}
void showColorMenu() {
    controllerDisplay.start([] {
        for (int i = 0; i < (int)autonNames.size(); i++) {
            std::string line;
            if (i == current_auton_selection) {
                line = "> " + autonNames[i];  // highlight
            } else {
                line = "  " + autonNames[i];
            }
            controllerDisplay.addLine(line);
        }

        //controllerDisplay.addLine("Press A to confirm");
    });
}
void showAutonMenu() {
    controllerDisplay.start([] {
        for (int i = 0; i < (int)autonNames.size(); i++) {
            std::string line;
            if (i == current_auton_selection) {
                line = "> " + autonNames[i];  // highlight
            } else {
                line = "  " + autonNames[i];
            }
            controllerDisplay.addLine(line);
        }

        //controllerDisplay.addLine("Press A to confirm");
    });
}

// Helper: build feature vector and run inference
bool telem_predict_xy(double& outX, double& outY) {
  if (!g_telemModelLoaded) {
    outX = outY = 0.0;
    return false;
  }

  // Build the 7-feature input vector
  double xis = (chassis.get_X_position() > 72) ? 1.0 : 0.0;
  double yis = (chassis.get_Y_position() > 72) ? 1.0 : 0.0;

  std::vector<double> x = {
    DistanceFront.objectDistance(inches),
    DistanceLeft.objectDistance(inches),
    DistanceBack.objectDistance(inches),
    DistanceRight.objectDistance(inches),
    chassis.get_absolute_heading(),
    xis,
    yis
  };

  InferenceResult r = infer(g_telemModel, x);

  // Multi-output regression: we expect 2 outputs (X, Y)
  if (r.y_vec.size() >= 2) {
    outX = r.y_vec[0];
    outY = r.y_vec[1];
  } else if (r.y_vec.size() == 1) {
    outX = r.y_vec[0];
    outY = 0.0;
  } else {
    outX = outY = 0.0;
  }

  return true;
}


void pre_auton() {
  vexcodeInit();
  default_constants();
  outtakeRaise.set(false);


  int len = Brain.SDcard.size("telem_#100_best.txt");
  Brain.Screen.clearScreen();
  Brain.Screen.setCursor(1,1);
  Brain.Screen.print("Size = %d", len);
  Brain.Screen.newLine();

  if (Brain.SDcard.exists("telem_#100_best.txt")) {
    Brain.Screen.print("File found!");
  } else {
    Brain.Screen.print("File missing!");
  }
  Brain.Screen.newLine();

  // Actually load the model
  g_telemModelLoaded = load_model("telem_#100_best.txt", g_telemModel);

  if (!g_telemModelLoaded) {
    Brain.Screen.print("Model load FAILED");
    Brain.Screen.newLine();
  } else {
    Brain.Screen.print("Model OK: D=%d O=%d",
                       (int)g_telemModel.fshape.D,
                       (int)g_telemModel.fshape.O);
    Brain.Screen.newLine();
  }


  // Launch the pretty selector
  AutonUI ui(Brain, Controller1);
  g_autonSel = ui.runBlocking(MATCH_AUTONS, SKILLS_AUTONS);

  // Build: COLOR | MODE | AUTON LABEL
  std::string colorTxt = g_autonSel.isRed ? "RED" : "BLUE";
  std::string modeTxt  = (g_autonSel.mode == AutonUI::MATCH) ? "MATCH" : "SKILLS";

  std::string labelTxt = "-";
  if (g_autonSel.mode == AutonUI::MATCH) {
    if (g_autonSel.index >= 0 && g_autonSel.index < (int)MATCH_AUTONS.size()) {
      labelTxt = MATCH_AUTONS[g_autonSel.index].label; // works for std::string or const char*
    }
  } else { // SKILLS
    if (g_autonSel.index >= 0 && g_autonSel.index < (int)SKILLS_AUTONS.size()) {
      labelTxt = SKILLS_AUTONS[g_autonSel.index].label;
    }
  }

  // Pretty confirmation flash
  Brain.Screen.clearScreen();
  Brain.Screen.setPenColor(vex::white);
  Brain.Screen.setFont(vex::fontType::mono20); //should be 28
  Brain.Screen.printAt(10, 60, false, "Selected:");
  Brain.Screen.setFont(vex::fontType::mono30); //should be 32
  Brain.Screen.printAt(10, 100, false, "%s | %s | %s",
                       colorTxt.c_str(), modeTxt.c_str(), labelTxt.c_str());
  vex::wait(600, vex::msec);
}



void autonomous(void) {
  //safeProgSkills();
  //soloAWP();
  //rightRedElims();
  //leftRedElims();
  //rightBlueElims();
  //leftBlueElims();
  //blueLeftDiddler();
  //driveleftBlueElims();
  //redLeftDiddler();
  //safe_soloAWP();
  //redQualsHook();
  driveToMatchloader();
  //testRamsete();
  //blueRightElims9Ball();
  //redQualsHook();
  //testHookMove();
  //leftHookPushFromGoal(1.5);
  //risky_soloAWP();
  /*
  chassis.set_heading(90);
  toggleAligner();
  chassis.swing_max_voltage = 10;
  chassis.swing_settle_error = 1;
  chassis.swing_settle_time = 500;

  chassis.goal_swing_to_angle(180);
  
  highGoal();
  */
  //blueQualsHook();
  
  //safe_progSkills();
  //risky_progSkills();
  //lastBit();
  
  //redRightElims9Ball();

  //rightRedElims();
  //driverProgSkills();
  //shame();
  wait(60, seconds);
  //ball13();

  //If they tapped Stats tab and confirmed, fall back to a safe choice (skills safe),
  if (g_autonSel.mode == AutonUI::STATS) {
    // Placeholder: do something safe
    redLeftDiddler();
    //ball13();
    //safeProgSkills();
    return;
  }

  if (g_autonSel.mode == AutonUI::SKILLS) {
    runSkillsByIndex(SKILLS_AUTONS, g_autonSel.index);
    return;
  }

  // MATCH:
  runMatchByIndex(MATCH_AUTONS, g_autonSel.index, /*isRed=*/g_autonSel.isRed);
}


void alignForAuton(void) {

}

void usercontrol(void) {

  /*
  if (!colorSelected){
    showColorMenu();

    Controller1.ButtonUp.released(scrollUp);
    Controller1.ButtonDown.released(scrollDown);
    Controller1.ButtonA.released(autonConfirm);
    while(!colorSelected){
      task::sleep(10);
    }
  }
  if (!runProgram){
    showAutonMenu();

    Controller1.ButtonUp.released(scrollUp);
    Controller1.ButtonDown.released(scrollDown);
    Controller1.ButtonA.released(autonConfirm);
    while(!auton_confirmed){
      task::sleep(10);
    }
    
    alignForAuton();
  }
  */
  Brain.Screen.clearScreen();
  runProgram = true;

  // User control code here, inside the loop
  auton_running = false;


  
  float tps = 0;
  int tick_count = 0;
  int total_tick_count = 0;
  int tps_update_interval = 1000; // in milliseconds
  int last_tps_update = Brain.timer(msec);
  Controller1.Screen.clearScreen();
  
  controllerDisplay.start([] {
    controllerDisplay.addFormattedLine("Colorsort: %.1f", static_cast<int>(defaultSortStatus==finalSortStatus));
    controllerDisplay.addFormattedLine("A Toggle: %.1f", static_cast<int>(Aactive));
    controllerDisplay.addFormattedLine("H: %.1f", chassis.get_absolute_heading());
  });
  Controller1.ButtonA.pressed(toggleDisplay);

  Controller1.ButtonUp.pressed(scrollUp);
  Controller1.ButtonDown.pressed(scrollDown);

  Controller1.ButtonR2.pressed(intakeOut);
  Controller1.ButtonR1.pressed(intakeIn);

  Controller1.ButtonR2.released(intakeOff);
  Controller1.ButtonR1.released(intakeOff);

  Controller1.ButtonL1.pressed(highGoal);
  Controller1.ButtonL2.pressed(middleGoal);

  Controller1.ButtonL1.released(stopSecondStage);
  Controller1.ButtonL2.released(stopSecondStage);

  Controller1.ButtonB.pressed(lowerTotal);
  Controller1.ButtonX.pressed(toggleColorSort);

  Controller1.ButtonA.released(aPressed);
  Controller1.ButtonA.released(aReleased);


  Controller1.ButtonY.pressed(toggleDescore);

  Controller1.ButtonDown.pressed(toggleMatchLoad);

  Controller1.ButtonRight.pressed(toggleAligner);

  //Controller1.ButtonUp.pressed(loaderMacro);

  intakeMotor.setVelocity(100, percent);
  middleTravel.setVelocity(100, percent);
  scoreMotor.setVelocity(100, percent);

  intakeInterface.startIntakeThread();
  intakeInterface.intake_status = OFF_intake;
  intakeInterface.travel_status = OFF_travel;
  
  intakeInterface.highA = true;

  intakeMotor.stop();
  middleTravel.stop();
  scoreMotor.stop();

  //sorter.startColorSort();
  //sorter.sortStatus = NONE;
  
  chassis.drive_stop(coast);
  double x_pred = 0.0;
  double y_pred = 0.0;

  odom_constants();
  chassis.set_coordinates(72, 25, 0);


  if (!Brain.SDcard.isInserted()) {
    Controller1.Screen.clearScreen();
    Controller1.Screen.setCursor(1, 1);
    Controller1.Screen.print("Insert SD card");
  } else {
    telem_logger.set_precision(3);
    telem_logger.set_buffered_flush_ms(0);
    const bool csv_ok = telem_logger.open(
        "telem_irl_data.csv",
        {"t_ms","d_front_in","d_left_in","d_back_in","d_right_in","heading_deg","odom_x_in","odom_y_in"}
    );

    if (!csv_ok) {
      Controller1.Screen.clearScreen();
      Controller1.Screen.setCursor(1, 1);
      Controller1.Screen.print("CSV open failed");
    }
  }

  while (1) {

    telem_predict_xy(x_pred, y_pred);  // fills x_pred, y_pred

    // Debug: show inputs and outputs (optional but super helpful)
    double f = DistanceFront.objectDistance(inches);
    double l = DistanceLeft.objectDistance(inches);
    double b = DistanceBack.objectDistance(inches);
    double r = DistanceRight.objectDistance(inches);
    double h = chassis.get_absolute_heading();

    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print("f=%.1f l=%.1f", f, l);
    Brain.Screen.newLine();
    Brain.Screen.print("b=%.1f r=%.1f", b, r);
    Brain.Screen.newLine();
    Brain.Screen.print("h=%.1f", h);
    Brain.Screen.newLine();
    Brain.Screen.print("Pred X=%.2f Pred Y=%.2f", x_pred, y_pred);
    Brain.Screen.newLine();
    Brain.Screen.print("Odom X=%.2f Odom Y=%.2f", chassis.get_X_position(), chassis.get_Y_position());

    //chassis.control_arcade_left();
    
    log_snapshot_immediate();

    chassis.control_tank_ricky();

    if(Controller1.ButtonUp.pressing()){
      loaderMacro();
    }

    //chassis.control_arcade();

    /*
    if(Brain.Battery.current() < 10){
      Controller1.rumble(".");
    }
    */
    /*
    if(Controller1.ButtonA.pressing()){
      sorter.sortStatus = NONE;
      intakeMotor.spin(forward);
      scoreMotor.stop();
      if((!Optical12.isNearObject()) && !(Optical12.hue() < 10) && !(70 < Optical12.hue() && Optical12.hue() < 230)){
        middleTravel.spin(forward);
      }
      else{
        middleTravel.stop();
      }
    }
    */


    tick_count++;
    total_tick_count++;

    int now = Brain.timer(msec);



    task::sleep(2); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and calLadyBrownMotoracks.
//
int main(){
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);
  //Competition.drivercontrol(pre_auton);


  // Run the pre-autonomous function.
  pre_auton();
  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
