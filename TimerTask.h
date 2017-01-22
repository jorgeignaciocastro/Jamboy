/**
*	Timer Task for non-blocking task
**/

#ifndef TimerTask_h
#define TimerTask_h

#include "Arduino.h"

class TimerTask
{
  public:
    TimerTask(unsigned long delta = 1000, boolean timer_status = true);
    void tick(void (*callback)(int), unsigned long delta = 0);
    void activate();
    void desactivate();
    void setActive(boolean timer_status);
    
  private:
    unsigned long previous_millis = 0;
    unsigned long current_millis = 0;
    bool active = true;
    unsigned int count = 0;
    unsigned long delta_time;
};

#endif
