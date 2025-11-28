#include "vex.h"

colorSort::colorSort(int colorSensor1Port, sortType sortStatus):
  colorSensor1(optical(colorSensor1Port)),
  sortStatus(sortStatus),
  lastSortStatus(sortStatus)
{
}

void colorSort::blueDetected(){
  if(toDo.size() < 10){
    if(this->sortStatus == RED){
      toDo.push_back(SPIT);
    }
    else if(this->sortStatus == BLUE){
      toDo.push_back(SCORE);
    }
    else{
      toDo.push_back(HOLD);
    }
  }
}

void colorSort::redDetected(){
  if(toDo.size() < 10){
    if(this->sortStatus == RED){
      toDo.push_back(SCORE);
    }
    else if(this->sortStatus == BLUE){
      toDo.push_back(SPIT);
    }
    else{
      toDo.push_back(HOLD);
    }
  }
}

void colorSort::colorSortCheckLoop(){ //the loop for colorsort
  colorSensor1.setLightPower(100);
  while (1){
    if(sortStatus != NONE){
      if(colorSensor1.hue() < 10){
        redDetected();
        //while(colorSensor1.isNearObject() or !(colorSensor1.hue() < 5)){}
        //task::sleep(35);
        task::sleep(27); //was 30
      }
      if(180 < colorSensor1.hue() && colorSensor1.hue() < 250){
        blueDetected();
        //while(colorSensor1.isNearObject() or !(180 < colorSensor1.hue() && colorSensor1.hue() < 210)){}
        //task::sleep(35);
        task::sleep(27); //was 30
      }
    }
    if(sortStatus == NONE){
      if(colorSensor1.hue() < 10){
        toDo.clear();
        if(lastSortStatus == RED){
          toDo.push_front(SCORE);
        }
        else if(lastSortStatus == BLUE){
          toDo.push_front(SPIT);
        }
        task::sleep(20);
      }
      if(180 < colorSensor1.hue() && colorSensor1.hue() < 250){
        toDo.clear();
        if(lastSortStatus == BLUE){
          toDo.push_front(SCORE);
        }
        else if(lastSortStatus == RED){
          toDo.push_front(SPIT);
        }
        task::sleep(20);
      }
    }
  
    task::sleep(2);
  }
}
void colorSort::colorSortActionLoop(){ //the loop for colorsort
  colorSensor1.setLightPower(100);
  while (1){
    /*
    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print(toDo.size());
    //Brain.Screen.newLine();
    //Brain.Screen.print(toDo);
    Brain.Screen.setCursor(3, 1);
    if(sortStatus == RED){Brain.Screen.print("RED");}
    else if(sortStatus == BLUE){Brain.Screen.print("BLUE");}
    else if(sortStatus == NONE){Brain.Screen.print("NONE");}
    Brain.Screen.setCursor(5, 1);
    if(toDo.front() == SCORE){Brain.Screen.print("SCORE");}
    else if(toDo.front() == SPIT){Brain.Screen.print("SPIT");}
    else if(toDo.front() == HOLD){Brain.Screen.print("HOLD");}
    Brain.Screen.setCursor(7, 1);
    Brain.Screen.print(colorSensor1.hue());
    */
    if(toDo.size() > 0 and sortStatus != NONE){
      if(toDo.front() == SPIT){
        if(toDo.size() == 1){
          //task::sleep(2);
        }
        
        scoreMotor.spin(reverse);
        task::sleep(37); //was 35
        scoreMotor.spin(forward);
        
        /*
        scoreMotor.spin(reverse);
        task::sleep(32);
        middleTravel.stop();
        task::sleep(5);
        scoreMotor.spin(forward);
        middleTravel.spin(forward);
        */

        toDo.pop_front();
        if(sortStatus == NONE){
          scoreMotor.stop();
        }
      }
      else if(toDo.front() == SCORE){
        
        if(toDo.size() == 1){
          //task::sleep(2);
        }
        
        
        scoreMotor.spin(forward);
        task::sleep(34); //was 115
        
        /*
        scoreMotor.spin(forward);
        task::sleep(30);
        middleTravel.stop();
        task::sleep(4);
        middleTravel.spin(forward);
        */


        toDo.pop_front();
        if(sortStatus == NONE){
          scoreMotor.stop();
        }
      }
      else if(toDo.front() == HOLD){
        scoreMotor.stop();
        //task::sleep(75);
        

        toDo.pop_front();
      }
    }
    
  
    task::sleep(2);
  }
}

int colorSort::color_sort_check_task(void *instance){ //the "vex task" (basically a thread) for the colorSort
  colorSort* self = static_cast<colorSort*>(instance);
  task::sleep(10);
  self->colorSortCheckLoop();
  return 0;
}
int colorSort::color_sort_action_task(void *instance){ //the "vex task" (basically a thread) for the colorSort
  colorSort* self = static_cast<colorSort*>(instance);
  task::sleep(10);
  self->colorSortActionLoop();
  return 0;
}

void colorSort::startColorSort(){ //starts up the colorsort loop
  color_check_task = task(color_sort_check_task,this);
  color_action_task = task(color_sort_action_task,this);
}
