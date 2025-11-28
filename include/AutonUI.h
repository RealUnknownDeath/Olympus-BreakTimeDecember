#pragma once
#include <vector>
#include <string>
#include "vex.h"
#include "AutonRegistry.h"

class AutonUI {
public:
  enum Mode { MATCH = 0, SKILLS = 1, STATS = 2 };

  struct Selection {
    Mode mode = MATCH;
    int  index = 0;     // which item in the current list
    bool isRed = true;  // team color
    bool confirmed = false;
  };

  AutonUI(vex::brain& brain, vex::controller& controller);

  // Blocks until the user confirms; returns the selection
  Selection runBlocking(const std::vector<MatchAuton>& match,
                        const std::vector<SkillsAuton>& skills);

private:
  vex::brain&      Brain;
  vex::controller& Controller;

  // State
  Mode mode_ = MATCH;
  int  selectedIndex_ = 0;
  bool isRed_ = true;

  // STATS sub-tabs
  enum StatsTab { STATS_MOT = 0, STATS_SENS = 1, STATS_TOG = 2 };
  int statsTab_ = STATS_MOT;

  // Touch handling
  bool touching_ = false;
  int  lastX_ = 0, lastY_ = 0;

  // Layout constants
  struct R {
    // Whole screen: 480x240 on V5 Brain
    static constexpr int W = 480;
    static constexpr int H = 240;

    // Top tab bar
    static constexpr int TAB_H = 42;

    // STATS sub-tab bar
    static constexpr int STAB_H = 34;
    static constexpr int STAB_Y = TAB_H;

    // Color toggle pill
    static constexpr int COLOR_H = 38;
    static constexpr int COLOR_Y = H - COLOR_H - 8;
    static constexpr int COLOR_W = 220;
    static constexpr int COLOR_X = 12;

    // Confirm button
    static constexpr int OK_W = 140;
    static constexpr int OK_H = 44;
    static constexpr int OK_X = W - OK_W - 12;
    static constexpr int OK_Y = H - OK_H - 8;

    // List box (default for MATCH/SKILLS)
    static constexpr int LIST_X = 12;
    static constexpr int LIST_Y = TAB_H + 10;
    static constexpr int LIST_W = W - 24;
    static constexpr int LIST_H = H - TAB_H - COLOR_H - 10 - 16 - 8; // leave room for bottom row
    static constexpr int ITEM_H = 32;

    // List box for STATS (account for STATS sub-tabs)
    static constexpr int LIST_Y_STATS = STAB_Y + STAB_H + 8;
    static constexpr int LIST_H_STATS = H - LIST_Y_STATS - COLOR_H - 16 - 8;

    // Tabs: 3 equal segments
    static constexpr int TAB_W = W / 3;
    // STATS subtabs: also 3 equal segments
    static constexpr int STAB_W = W / 3;
  };

  // Render helpers
  void drawTabs();
  void drawList(const std::vector<std::string>& labels);
  void drawColorToggle();
  void drawConfirm();

  // STATS helpers
  void drawStatsSubtabs();
  std::vector<std::string> buildMotorLines();
  std::vector<std::string> buildSensorLines();
  std::vector<std::string> buildToggleLines();
  void handleToggleTap(int idx);

  // UI helpers
  void fillRect(int x, int y, int w, int h, vex::color c);
  void strokeRect(int x, int y, int w, int h, vex::color c, int pen = 2);
  void roundedPanel(int x, int y, int w, int h, vex::color fill, vex::color border);

  // Input helpers
  void handleTouchOnce(const std::vector<std::string>& labels);
  int  listItemAt(int y, int itemCount) const;
  void clampSelection(int itemCount);
};
