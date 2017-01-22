#include "Arduino.h"
#include "TimerTask.h"

/**
*  Consturctor
**/

TimerTask::TimerTask(unsigned long delta, boolean timer_status) {
  delta_time = delta;
  active = timer_status;
}

// activate method
void TimerTask::activate() {
  active = true;
}

// desactivate method
void TimerTask::desactivate() {
  active = false;
}
// single line active method
void TimerTask::setActive(boolean timer_status) {
  active = timer_status;
}

// main method, run soft timer
void TimerTask::tick(void (*callback)(int count), unsigned long delta) {

  if(delta > 0 ) {
    delta_time = delta;
  }
  
  //check if it's active
  if(active) {
    
    //start counting
    current_millis = millis();

    if(current_millis - previous_millis >= delta_time) {
      //start new cycle
      previous_millis = current_millis;
      count ++;

      //call callback
      callback(count);
         
      }
      
    }
}


