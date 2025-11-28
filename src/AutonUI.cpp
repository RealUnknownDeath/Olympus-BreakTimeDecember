#include "AutonUI.h"
#include "StatsDeviceMap.h"
#include "globals.h"
#include <algorithm>
#include <stdio.h>   // for snprintf on VEX toolchain


using namespace vex;

// (Optional) if you have this elsewhere, this extern will link to it:
extern void toggleColorSort();

static const color BG_DARK    = color(18, 18, 22);
static const color BG_PANEL   = color(30, 30, 36);
static const color BG_ACCENT  = color(50, 50, 58);
static const color FG_TEXT    = color(255, 255, 255);
static const color FG_MUTED   = color(180, 180, 190);
static const color ACCENT     = color(0, 153, 255);
static const color ACCENT_RED = color(255, 62, 62);
static const color ACCENT_BLU = color(80, 170, 255);
static const color HILITE     = color(240, 240, 255);

bool colorSortOn(){
  if(defaultSortStatus == NONE){return false;}
  return true;
}

static inline std::string fixed1(double x){
  char b[32];
  snprintf(b, sizeof(b), "%.1f", x);   // no std:: prefix
  return std::string(b);
}
static inline std::string fixed0(double x){
  char b[32];
  snprintf(b, sizeof(b), "%.0f", x);   // no std:: prefix
  return std::string(b);
}

AutonUI::AutonUI(brain& brain, controller& controller)
: Brain(brain), Controller(controller) {}

void AutonUI::fillRect(int x, int y, int w, int h, color c) {
  Brain.Screen.setFillColor(c);
  Brain.Screen.setPenColor(c);
  Brain.Screen.drawRectangle(x, y, w, h);
}
void AutonUI::strokeRect(int x, int y, int w, int h, color c, int pen) {
  Brain.Screen.setPenWidth(pen);
  Brain.Screen.setPenColor(c);
  Brain.Screen.drawRectangle(x, y, w, h);
}
void AutonUI::roundedPanel(int x, int y, int w, int h, color fill, color border) {
  fillRect(x, y, w, h, fill);
  strokeRect(x, y, w, h, border, 2);
}

void AutonUI::drawTabs() {
  // Background bar
  fillRect(0, 0, R::W, R::TAB_H, BG_PANEL);

  Brain.Screen.setFont(fontType::mono20);
  auto tabDraw = [&](int i, const char* label, bool active) {
    int x = i * R::TAB_W;
    fillRect(x, 0, R::TAB_W, R::TAB_H, active ? BG_ACCENT : BG_PANEL);
    strokeRect(x, 0, R::TAB_W, R::TAB_H, active ? ACCENT : FG_MUTED, 2);
    Brain.Screen.setPenColor(active ? HILITE : FG_MUTED);
    Brain.Screen.printAt(x + 20, 28, false, "%s", label);
  };

  tabDraw(0, "MATCH", mode_ == MATCH);
  tabDraw(1, "SKILLS", mode_ == SKILLS);
  tabDraw(2, "STATS", mode_ == STATS);
}

void AutonUI::drawStatsSubtabs() {
  // Bar under the top tabs
  int y = R::STAB_Y;
  fillRect(0, y, R::W, R::STAB_H, BG_PANEL);

  Brain.Screen.setFont(fontType::mono20);
  auto subDraw = [&](int i, const char* label, bool active) {
    int x = i * R::STAB_W;
    fillRect(x, y, R::STAB_W, R::STAB_H, active ? BG_ACCENT : BG_PANEL);
    strokeRect(x, y, R::STAB_W, R::STAB_H, active ? ACCENT : FG_MUTED, 2);
    Brain.Screen.setPenColor(active ? HILITE : FG_MUTED);
    Brain.Screen.printAt(x + 18, y + 24, false, "%s", label);
  };

  subDraw(0, "Motors",   statsTab_ == STATS_MOT);
  subDraw(1, "Sensors",  statsTab_ == STATS_SENS);
  subDraw(2, "Toggles",  statsTab_ == STATS_TOG);
}

void AutonUI::drawList(const std::vector<std::string>& labels) {
  // Box geometry: account for STATS subtabs
  const int listX = R::LIST_X;
  const int listY = (mode_ == STATS) ? R::LIST_Y_STATS : R::LIST_Y;
  const int listW = R::LIST_W;
  const int listH = (mode_ == STATS) ? R::LIST_H_STATS : R::LIST_H;

  roundedPanel(listX, listY, listW, listH, BG_PANEL, BG_ACCENT);
  Brain.Screen.setFont(fontType::mono20);

  const int visible = listH / R::ITEM_H;
  int start = 0;
  if ((int)labels.size() > visible) {
    start = std::max(0, std::min(selectedIndex_ - visible / 2, (int)labels.size() - visible));
  }

  for (int row = 0; row < visible; ++row) {
    const int idx = start + row;
    const int y = listY + row * R::ITEM_H;
    if (idx >= (int)labels.size()) break;

    const bool sel = (idx == selectedIndex_);
    if (sel) fillRect(listX + 2, y + 2, listW - 4, R::ITEM_H - 4, BG_ACCENT);

    Brain.Screen.setPenColor(sel ? HILITE : FG_TEXT);
    Brain.Screen.printAt(listX + 12, y + 22, false, "%s", labels[idx].c_str());
  }

  // Footer hint only (no color pill or confirm here)
  Brain.Screen.setPenColor(FG_MUTED);
  Brain.Screen.printAt(listX + 10, listY + listH - 6, false,
                       (mode_ == STATS)
                         ? "Tap rows to toggle"
                         : "UP/DOWN: select   A: confirm   L/R: color");
}


void AutonUI::drawColorToggle() {
  roundedPanel(R::COLOR_X, R::COLOR_Y, R::COLOR_W, R::COLOR_H, BG_PANEL, BG_ACCENT);

  int half = R::COLOR_W / 2;
  int leftX  = R::COLOR_X;
  int rightX = R::COLOR_X + half;

  // Red half
  fillRect(leftX + 2, R::COLOR_Y + 2, half - 4, R::COLOR_H - 4, isRed_ ? ACCENT_RED : BG_PANEL);
  Brain.Screen.setPenColor(isRed_ ? color::white : FG_MUTED);
  Brain.Screen.setFont(fontType::mono20);
  Brain.Screen.printAt(leftX + 14, R::COLOR_Y + 24, false, "RED");

  // Blue half
  fillRect(rightX + 2, R::COLOR_Y + 2, half - 4, R::COLOR_H - 4, !isRed_ ? ACCENT_BLU : BG_PANEL);
  Brain.Screen.setPenColor(!isRed_ ? color::white : FG_MUTED);
  Brain.Screen.printAt(rightX + 14, R::COLOR_Y + 24, false, "BLUE");
}

void AutonUI::drawConfirm() {
  roundedPanel(R::OK_X, R::OK_Y, R::OK_W, R::OK_H, ACCENT, BG_ACCENT);
  Brain.Screen.setPenColor(color::white);
  Brain.Screen.setFont(fontType::mono20);
  Brain.Screen.printAt(R::OK_X + 26, R::OK_Y + 28, false, "CONFIRM");
}

// ---------- Build lines for STATS: Motors / Sensors / Toggles ----------

std::vector<std::string> AutonUI::buildMotorLines() {
  std::vector<std::string> out;
  if (STATS_MOTORS.empty()) { out.push_back("(no motors listed)"); return out; }

  for (auto& e : STATS_MOTORS) {
    if (!e.m) continue;
    // Celsius straight from VEX API
    double tC = e.m->temperature(vex::temperatureUnits::celsius);
    out.push_back(std::string(e.name) + " — " + fixed1(tC) + "°C");
  }
  return out;
}


std::vector<std::string> AutonUI::buildSensorLines() {
  std::vector<std::string> out;

  if (STATS_IMUS.empty() && STATS_GPS.empty() && STATS_DISTANCE.empty() &&
      STATS_OPTICAL.empty() && STATS_ROTATION.empty()) {
    out.push_back("(no sensors listed)");
    return out;
  }

  for (auto& e : STATS_IMUS) if (e.imu) {
    out.push_back(std::string(e.name) + " — H:" + fixed1(e.imu->heading())
                 + "° P:" + fixed1(e.imu->pitch()) + "° R:" + fixed1(e.imu->roll()) + "°");
  }
  for (auto& e : STATS_GPS) if (e.g) {
    out.push_back(std::string(e.name) + " — X:" + fixed1(e.g->xPosition(vex::distanceUnits::in))
                 + " Y:" + fixed1(e.g->yPosition(vex::distanceUnits::in))
                 + " H:" + fixed1(e.g->heading()) + "°");
  }
  for (auto& e : STATS_DISTANCE) if (e.d) {
    bool obj = e.d->isObjectDetected();
    out.push_back(std::string(e.name) + " — " +
                  (obj ? (fixed0(e.d->objectDistance(vex::distanceUnits::mm)) + " mm")
                       : "no object"));
  }
  for (auto& e : STATS_OPTICAL) if (e.o) {
    // brightness(): 0–100, proximity(): 0–100 (approx), hue(): 0–360
    out.push_back(std::string(e.name) + " — hue:" + fixed0((double)e.o->hue()) +
                " br:" + fixed0((double)e.o->brightness()) +
                " prox:" + fixed0((double)e.o->isNearObject()) +
                (e.o->isNearObject() ? " (near)" : ""));
  }

  for (auto& e : STATS_ROTATION) if (e.r) {
    out.push_back(std::string(e.name) + " — " +
                  fixed1(e.r->position(vex::rotationUnits::deg)) + "°");
  }

  return out;
}

std::vector<std::string> AutonUI::buildToggleLines() {
  std::vector<std::string> out;
  auto onoff = [](bool b){ return b ? "ON" : "OFF"; };

  // ColorSort state display — we can't introspect easily; present as an action
  out.push_back(std::string("ColorSort — ") +
              (colorSortOn() ? "ON" : "OFF") + "  (tap to toggle)");

  out.push_back(std::string("Descore — ")     + onoff(descoreStatus));
  out.push_back(std::string("Match Load — ")  + onoff(matchLoadStatus));
  out.push_back(std::string("Aligner — ")     + onoff(alignerStatus));
  out.push_back(std::string("Score Override — ") + onoff(scoreOverride));

  return out;
}

void AutonUI::handleToggleTap(int idx) {
  // idx is relative to the TOGGLES list
  switch (idx) {
    case 0: // ColorSort
      toggleColorSort(); // uses your existing function
      break;
    case 1: descoreStatus     = !descoreStatus;     break;
    case 2: matchLoadStatus   = !matchLoadStatus;   break;
    case 3: alignerStatus     = !alignerStatus;     break;
    case 4: scoreOverride     = !scoreOverride;     break;
    default: break;
  }
}

// ---------- Input helpers ----------

int AutonUI::listItemAt(int y, int itemCount) const {
  int listY = (mode_ == STATS) ? R::LIST_Y_STATS : R::LIST_Y;
  int listH = (mode_ == STATS) ? R::LIST_H_STATS : R::LIST_H;
  if (y < listY || y > listY + listH) return -1;
  int row = (y - listY) / R::ITEM_H;

  int visible = listH / R::ITEM_H;
  int start = 0;
  if (itemCount > visible) {
    start = std::max(0, std::min(selectedIndex_ - visible / 2, itemCount - visible));
  }
  int idx = start + row;
  if (idx < 0 || idx >= itemCount) return -1;
  return idx;
}

void AutonUI::clampSelection(int itemCount) {
  if (itemCount <= 0) { selectedIndex_ = 0; return; }
  if (selectedIndex_ < 0) selectedIndex_ = 0;
  if (selectedIndex_ >= itemCount) selectedIndex_ = itemCount - 1;
}

void AutonUI::handleTouchOnce(const std::vector<std::string>& labels) {
  // Top tabs?
  if (lastY_ < R::TAB_H) {
    int tab = lastX_ / R::TAB_W;
    if      (tab == 0) mode_ = MATCH;
    else if (tab == 1) mode_ = SKILLS;
    else               mode_ = STATS;
    return;
  }

  // STATS subtabs?
  if (mode_ == STATS &&
      lastY_ >= R::STAB_Y && lastY_ <= R::STAB_Y + R::STAB_H) {
    int stab = lastX_ / R::STAB_W;
    statsTab_ = (stab == 0) ? STATS_MOT : (stab == 1) ? STATS_SENS : STATS_TOG;
    return;
  }

  // Color pill? (disabled on STATS)
  if (mode_ != STATS &&
      lastY_ >= R::COLOR_Y && lastY_ <= R::COLOR_Y + R::COLOR_H &&
      lastX_ >= R::COLOR_X && lastX_ <= R::COLOR_X + R::COLOR_W) {
    int half = R::COLOR_W / 2;
    isRed_ = (lastX_ < R::COLOR_X + half);
    return;
  }


  // Confirm?
  if (lastX_ >= R::OK_X && lastX_ <= R::OK_X + R::OK_W &&
      lastY_ >= R::OK_Y && lastY_ <= R::OK_Y + R::OK_H) {
    return; // runBlocking will catch it
  }

  // List click: in STATS/TOGGLES, act immediately
  int idx = listItemAt(lastY_, (int)labels.size());
  if (idx != -1) {
    selectedIndex_ = idx;
    if (mode_ == STATS && statsTab_ == STATS_TOG) {
      handleToggleTap(idx);
    }
  }
}

// ---------- Event-driven, no-flicker loop ----------

AutonUI::Selection AutonUI::runBlocking(const std::vector<MatchAuton>& match,
                                        const std::vector<SkillsAuton>& skills) {
  Selection out;

  auto makeMatchLabels = [&](){
    std::vector<std::string> v; v.reserve(match.size());
    for (auto& m : match) v.push_back(m.label);
    return v;
  };
  auto makeSkillsLabels = [&](){
    std::vector<std::string> v; v.reserve(skills.size());
    for (auto& s : skills) v.push_back(s.label);
    return v;
  };

  // Edge latches
  bool upPrev=false, downPrev=false, leftPrev=false, rightPrev=false;
  bool xPrev=false, yPrev=false, bPrev=false, aPrev=false;

  touching_ = false;
  bool needsRedraw = true;

  // === Stats list auto-refresh (list-only) ===
  static const int STATS_REFRESH_MS = 125;  // 8 Hz; set to 500 for 2 Hz
  int lastStatsRefresh = Brain.timer(vex::msec);

  auto repaint = [&](const std::vector<std::string>& labels){
    fillRect(0, 0, R::W, R::H, BG_DARK);
    drawTabs();
    if (mode_ == STATS) {
      drawStatsSubtabs();
    }
    drawList(labels);
    if (mode_ != STATS) {
      drawColorToggle();   // <-- now hidden on STATS
      drawConfirm();       // already hidden on STATS from earlier change
    }
  };


  while (true) {
    // Controller polling (edge-based)
    bool upNow    = Controller.ButtonUp.pressing();
    bool downNow  = Controller.ButtonDown.pressing();
    bool leftNow  = Controller.ButtonLeft.pressing();
    bool rightNow = Controller.ButtonRight.pressing();
    bool xNow     = Controller.ButtonX.pressing();
    bool yNow     = Controller.ButtonY.pressing();
    bool bNow     = Controller.ButtonB.pressing();
    bool aNow     = Controller.ButtonA.pressing();

    if (upNow   && !upPrev)   { selectedIndex_--; needsRedraw = true; }
    if (downNow && !downPrev) { selectedIndex_++; needsRedraw = true; }
    if (leftNow  && !leftPrev  && mode_ != STATS) { isRed_ = true;  needsRedraw = true; }
    if (rightNow && !rightPrev && mode_ != STATS) { isRed_ = false; needsRedraw = true; }
    if (xNow    && !xPrev)    { mode_  = MATCH;  needsRedraw = true; }
    if (yNow    && !yPrev)    { mode_  = SKILLS; needsRedraw = true; }
    if (bNow    && !bPrev)    { mode_  = STATS;  needsRedraw = true; }

    upPrev=upNow; downPrev=downNow; leftPrev=leftNow; rightPrev=rightNow;
    xPrev=xNow; yPrev=yNow; bPrev=bNow;

    // ===== Build labels for CURRENT state (pre-touch) =====
    std::vector<std::string> labels;
    if (mode_ == MATCH)       labels = makeMatchLabels();
    else if (mode_ == SKILLS) labels = makeSkillsLabels();
    else {
      if      (statsTab_ == STATS_MOT)  labels = buildMotorLines();
      else if (statsTab_ == STATS_SENS) labels = buildSensorLines();
      else                               labels = buildToggleLines();
    }

    // ===== Touch handling (use real labels so list taps map correctly) =====
    AutonUI::Mode oldMode    = mode_;
    int           oldStatsTab= statsTab_;
    bool          oldIsRed   = isRed_;

    if (Brain.Screen.pressing()) {
      if (!touching_) {
        touching_ = true;
        lastX_ = Brain.Screen.xPosition();
        lastY_ = Brain.Screen.yPosition();
        handleTouchOnce(labels);    // pass labels so row taps work
        needsRedraw = true;
      }
    } else {
      touching_ = false;
    }

    // If a tap switched tabs/subtabs/color, rebuild labels immediately
    if (mode_ != oldMode || statsTab_ != oldStatsTab || isRed_ != oldIsRed) {
      labels.clear();
      if (mode_ == MATCH)       labels = makeMatchLabels();
      else if (mode_ == SKILLS) labels = makeSkillsLabels();
      else {
        if      (statsTab_ == STATS_MOT)  labels = buildMotorLines();
        else if (statsTab_ == STATS_SENS) labels = buildSensorLines();
        else                               labels = buildToggleLines();
      }
    }

    clampSelection((int)labels.size());

    // Confirm (A edge) or Confirm button tap (ignored on STATS)
    bool aEdge = (aNow && !aPrev);
    aPrev = aNow;

    bool inConfirmBtn =
      Brain.Screen.xPosition() >= R::OK_X && Brain.Screen.xPosition() <= R::OK_X + R::OK_W &&
      Brain.Screen.yPosition() >= R::OK_Y && Brain.Screen.yPosition() <= R::OK_Y + R::OK_H;

    bool confirmRequested = (aEdge || (touching_ && inConfirmBtn));
    if (confirmRequested && mode_ != STATS) {
      out.mode      = mode_;
      out.index     = selectedIndex_;
      out.isRed     = isRed_;
      out.confirmed = true;
      repaint(labels); // final stable frame
      break;
    }

    // Full repaint only when something changed
    if (needsRedraw) {
      repaint(labels);
      needsRedraw = false;
      lastStatsRefresh = Brain.timer(vex::msec); // reset ticker after a full paint
    }

    // === Light-weight, list-only refresh on STATS (Motors/Sensors) ===
    if (mode_ == STATS) {
      int now = Brain.timer(vex::msec);
      if (now - lastStatsRefresh >= STATS_REFRESH_MS) {
        lastStatsRefresh = now;

        if (statsTab_ == STATS_MOT || statsTab_ == STATS_SENS) {
          // rebuild dynamic lines and repaint ONLY the list panel
          std::vector<std::string> dyn;
          if (statsTab_ == STATS_MOT)  dyn = buildMotorLines();
          else                          dyn = buildSensorLines();
          clampSelection((int)dyn.size());
          drawList(dyn);   // repaints just the list box area
        }
        // (Toggles page doesn’t auto-refresh; taps update it immediately)
      }
    }

    wait(25, msec); // smooth input scan; list refresh is timed separately
  }

  return out;
}


