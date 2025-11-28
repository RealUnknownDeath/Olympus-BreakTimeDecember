#include "vex.h"
void auton_from_Json(const char * filepath){
  pathio::Config cfg;
  std::vector<pathio::Action> actions;

  if (!pathio::load_path_file(filepath, cfg, actions)) {
    Brain.Screen.print("Failed to load path.json");
    while (true) this_thread::sleep_for(20);
  }

  Brain.Screen.clearScreen();
  Brain.Screen.setCursor(1,1);
  Brain.Screen.print("Start(%.2f, %.2f)", cfg.startX, cfg.startY);
  chassis.set_coordinates(cfg.startX, cfg.startY, cfg.startHeading-90);
  chassis.drive_max_voltage = 5;

  int row = 2;
  for (size_t i = 0; i < actions.size() && row < 12; ++i, ++row) {
    const auto& a = actions[i];
    Brain.Screen.setCursor(row,1);
    switch (a.type) {
      case pathio::ActionType::Waypoint:
        if (!(a.x == cfg.startX and a.y == cfg.startY)){
          Brain.Screen.print("WPT %.1f,%.1f h=%.2f", a.x,a.y,a.heading);
          Brain.Screen.print("CURRENT %.1f,%.1f h=%.2f", chassis.get_X_position(),chassis.get_Y_position(),chassis.get_absolute_heading());
          chassis.turn_to_point(a.x,a.y);
          wait(10, seconds);
          //chassis.drive_to_pose(a.x,a.y,a.heading-90);
          chassis.drive_to_point(a.x,a.y);
          Brain.Screen.clearScreen();
        }
        break;
      case pathio::ActionType::Turn:
        Brain.Screen.print("TURN h=%.2f t=%d s=%d", a.heading,a.time,a.speed); break;
        chassis.turn_to_angle(a.heading-90 ,chassis.turn_max_voltage, chassis.turn_settle_error, a.time, chassis.turn_timeout); //add speed
        Brain.Screen.clearScreen();
        break;
      case pathio::ActionType::Wait:
        Brain.Screen.print("WAIT %.2fs", a.duration); break;
        wait(a.duration, seconds);
        Brain.Screen.clearScreen();
        break;
      case pathio::ActionType::Speed:
        chassis.drive_max_voltage = a.speed * 12.0;
        Brain.Screen.print("SPEED %d%%", a.scale); break;
    }
  }

  while (true) this_thread::sleep_for(20);
}