#include <Arduino.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

#include "Clock.h"

Clock::Clock()
{
}

timeclock_t Clock::getCurrentTime()
{
  tmElements_t tm;
  RTC.read(tm);
  timeclock_t now;
  now.hour = tm.Hour;
  now.minute = tm.Minute;
  return now;
}

void Clock::setNewCurrentTime(int hour, int minute)
{
  tmElements_t tm;
  tm.Hour = hour;
  tm.Minute = minute;
  tm.Second = 0;
  RTC.write(tm);
}