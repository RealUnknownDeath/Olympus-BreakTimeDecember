#pragma once
#include "vex.h"

enum sortType {RED, BLUE, NONE};
enum sortAction {SPIT, SCORE, HOLD};

class colorSort
{
  private:
    std::list<sortAction> toDo;

    void blueDetected();
    void redDetected();
  public:

    //variables:
    
    sortType sortStatus;
    sortType lastSortStatus;

    //sensors
    optical colorSensor1;
    //optical colorSensor2;
    
    //functions
    void startColorSort();
    static int color_sort_check_task(void *instance);
    static int color_sort_action_task(void *instance);
    void colorSortCheckLoop();
    void colorSortActionLoop();

    //the colorsorting task (thread)
    vex::task color_check_task;
    vex::task color_action_task;

    //constructor:

    colorSort(int colorSensor1Port, sortType sortStatus);

};