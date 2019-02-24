#ifndef Clock_h
#define Clock_h

#include <Arduino.h>

struct timeclock_t
{
  int hour, minute;
};

class Clock
{
public:
  Clock();
  timeclock_t getCurrentTime();
  void setNewCurrentTime(int hour, int minute);
};

#endif