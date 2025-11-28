/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       vex.h                                                     */
/*    Author:       Vex Robotics                                              */
/*    Created:      1 Feb 2019                                                */
/*    Description:  Default header for V5 projects                            */
/*                                                                            */
/*----------------------------------------------------------------------------*/
//

#pragma once
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <iostream>
#include <map>
#include <list>
#include <random>
#include <cmath>   // for std::sqrt
#include <deque>   // for std::deque

#include "v5.h"
#include "v5_vcs.h"

#include "ai/vex_infer.h"


#include "robot-config.h"
#include "Santi-Template/odom.h"
#include "Santi-Template/drive.h"
#include "Santi-Template/util.h"
#include "Santi-Template/PID.h"

#include "monte_carlo/telemetry.h"
#include "monte_carlo/gps_.h"
#include "monte_carlo/monte_carlo.h"

#include "csv_logger.h"

#include "path_actions.h"
#include "auton_interpreter.h"
#include "ControllerDisplay.h"

#include "colorSort.h"
#include "intake.h"

#include "santis_eigen.h"
#include "autons.h"


#define waitUntil(condition)                                                   \
  do {                                                                         \
    wait(5, msec);                                                             \
  } while (!(condition))

#define repeat(iterations)                                                     \
  for (int iterator = 0; iterator < iterations; iterator++)