#include "globals.h"
#include "AutonUI.h"  // safe even if already pulled via globals.h

AutonUI::Selection g_autonSel{AutonUI::MATCH, /*index*/0, /*isRed*/true, /*confirmed*/false};



// Initialize booleans
bool descoreStatus   = false;
bool matchLoadStatus = false;
bool alignerStatus = false;
bool scoreOverride   = false;
bool Aactive = false;

bool driverOveride = false;

//Initialize sort status
sortType defaultSortStatus = BLUE;
sortType finalSortStatus   = BLUE;


colorSort sorter(PORT21, defaultSortStatus);

intake intakeInterface(PORT21, OFF_intake, OFF_travel, PORT14, PORT13);


//AI stuff
SavedModel g_telemModel;
bool g_telemModelLoaded = false;

