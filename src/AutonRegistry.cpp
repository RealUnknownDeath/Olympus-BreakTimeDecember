#include "AutonRegistry.h"
#include "vex.h"

void runMatchByIndex(const std::vector<MatchAuton>& match, int idx, bool isRed) {
  if (match.empty()) return;
  if (idx < 0) idx = 0;
  if (idx >= (int)match.size()) idx = (int)match.size() - 1;

  AutonFn fn = isRed ? match[idx].runRed : match[idx].runBlue;
  if (fn) fn();
}

void runSkillsByIndex(const std::vector<SkillsAuton>& skills, int idx) {
  if (skills.empty()) return;
  if (idx < 0) idx = 0;
  if (idx >= (int)skills.size()) idx = (int)skills.size() - 1;

  AutonFn fn = skills[idx].run;
  if (fn) fn();
}
