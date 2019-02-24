#include <Arduino.h>

#include "Display.h"

Display::Display(int clk_pin, int dio_pin) : EMPTY{0, 0}, DASH{SEG_G, SEG_G}
{
  _clk_pin = clk_pin;
  _dio_pin = dio_pin;
}

void Display::init()
{
  _display = new TM1637Display(_clk_pin, _dio_pin);
  _display->setBrightness(0x0f);
  _display->clear();
  _dots = 0;
  _show = true;
}

void Display::showTime(uint8_t hour, uint8_t minute)
{
  _display->showNumberDecEx(hour, _dots, true, 2);
  _display->showNumberDecEx(minute, _dots, true, 2, 2);
  if (_dots == 0b01000000) {
    _dots = 0;
  } else {
    _dots = 0b01000000;
  }
}

void Display::showTimeSettingHours(uint8_t hour, uint8_t minute, bool blink)
{
  if ((_show) || (!blink)) {
    _display->showNumberDec(hour, true, 2);
    _display->showNumberDec(minute, true, 2, 2);
  } else {
    _display->setSegments(EMPTY, 2);
    _display->showNumberDec(minute, true, 2, 2);
  }
  _show = !_show;
}

void Display::showTimeSettingMinutes(uint8_t hour, uint8_t minute, bool blink)
{
  if ((_show) || (!blink)) {
    _display->showNumberDec(hour, true, 2);
    _display->showNumberDec(minute, true, 2, 2);
  } else {
    _display->setSegments(EMPTY, 2, 2);
    _display->showNumberDec(hour, true, 2);
  }
  _show = !_show;
}

void Display::showAlarmSettingHours(uint8_t hour, uint8_t minute, bool blink)
{
  if ((_show) || (!blink)) {
    if (hour == 24) {
      _display->setSegments(DASH, 2);
    } else {
      _display->showNumberDecEx(hour, 0b01000000, true, 2);
    }
    if (minute == 60) {
      _display->setSegments(DASH, 2, 2);
    } else {
      _display->showNumberDecEx(minute, 0b01000000, true, 2, 2);
    }
  } else {
    _display->setSegments(EMPTY, 2);
    if (minute == 60) {
      _display->setSegments(DASH, 2, 2);
    } else {
      _display->showNumberDecEx(minute, 0b01000000, true, 2, 2);
    }
  }
  _show = !_show;
}

void Display::showAlarmSettingMinutes(uint8_t hour, uint8_t minute, bool blink)
{
  if ((_show) || (!blink)) {
    if (hour == 24) {
      _display->setSegments(DASH, 2);
    } else {
      _display->showNumberDecEx(hour, 0b01000000, true, 2);
    }
    if (minute == 60) {
      _display->setSegments(DASH, 2, 2);
    } else {
      _display->showNumberDecEx(minute, 0b01000000, true, 2, 2);
    }
  } else {
    _display->setSegments(EMPTY, 2, 2);
    if (hour == 24) {
      _display->setSegments(DASH, 2);
    } else {
      _display->showNumberDecEx(hour, 0b01000000, true, 2);
    }
  }
  _show = !_show;
}
