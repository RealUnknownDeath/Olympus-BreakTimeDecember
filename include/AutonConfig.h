#pragma once
#include <vector>
#include "AutonRegistry.h"
#include "autons.h"

// ===== Match list (color-agnostic labels). Toggle Red/Blue in the UI. =====
static inline std::vector<MatchAuton> MATCH_AUTONS = {
  // label,               Red function            , Blue function
  {"QUALS: 3+4+Hook",      redLeftDiddler,         blueLeftDiddler},
  {"ELIMS: 3+4+Hook",      redLeftDiddlerElims,         blueLeftDiddlerElims},
  {"Left 9 Elims",        leftRedElims,             leftBlueElims},
  {"Right 9 Elims",       rightBlueElims,            rightRedElims},
  {"13 Ball Carry",     ball13,        ball13},
  {"Solo AWP",       soloAWP,            ball13} //FIX SOLO AWP ON BLUE SIDE SANTI DON'T FORGET THIS PLEASE
  // Add more rows here if you need them
};

// ===== Skills list (programming skills variants, etc.) =====
static inline std::vector<SkillsAuton> SKILLS_AUTONS = {
  {"Programming Skills — Safe", safeProgSkills},
  // {"Programming Skills — Max",  maxProgSkills}, // if you have one
};
