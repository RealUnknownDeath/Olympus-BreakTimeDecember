#pragma once
#include "colorSort.h"
#include "intake.h"
#include "AutonUI.h"          // NEW: to see AutonUI::Selection
extern AutonUI::Selection g_autonSel;  // NEW: global selection


// Booleans
extern bool descoreStatus;
extern bool descoreLiftStatus;
extern bool matchLoadStatus;
extern bool scoreOverride;
extern bool alignerStatus;
extern bool Aactive;

extern bool driverOveride;

// Sort status
extern sortType defaultSortStatus;
extern sortType finalSortStatus;

// Global sorter object (declared here, defined exactly once in a .cpp)
extern colorSort sorter;
extern intake intakeInterface;

extern SavedModel g_telemModel;
extern bool g_telemModelLoaded;