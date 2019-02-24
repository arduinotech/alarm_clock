#ifndef Display_h
#define Display_h

#include <Arduino.h>
#include <TM1637Display.h>

class Display
{
public:
  Display(int clk_pin, int dio_pin);
  void init();
  void showTime(uint8_t hour, uint8_t minute);
  void showTimeSettingHours(uint8_t hour, uint8_t minute, bool blink);
  void showTimeSettingMinutes(uint8_t hour, uint8_t minute, bool blink);
  void showAlarmSettingHours(uint8_t hour, uint8_t minute, bool blink);
  void showAlarmSettingMinutes(uint8_t hour, uint8_t minute, bool blink);

private:
  int _clk_pin;
  int _dio_pin;
  uint8_t _dots;
  bool _show;
  TM1637Display *_display;
  uint8_t EMPTY[2];
  uint8_t DASH[2];
};

#endif