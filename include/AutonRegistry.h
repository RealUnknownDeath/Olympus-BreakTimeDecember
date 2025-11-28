#pragma once
#include <string>
#include <vector>

// A function pointer to one of your real autons in autons.cpp
typedef void (*AutonFn)();

struct MatchAuton {
  std::string label;  // e.g. "Left 6+1 Quals"
  AutonFn runRed;     // e.g. redLeftSafeQuals
  AutonFn runBlue;    // e.g. blueLeftSafeQuals
};

struct SkillsAuton {
  std::string label;  // e.g. "Programming Skills — Safe"
  AutonFn run;        // e.g. safeProgSkills
};

// Helpers the selector will call after you confirm:
void runMatchByIndex(const std::vector<MatchAuton>& match, int idx, bool isRed);
void runSkillsByIndex(const std::vector<SkillsAuton>& skills, int idx);
