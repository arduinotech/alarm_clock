/*
	Simple alarm clock

  This alarm clock shows the current time, plays the melody at the specified time and also saves the settings when the power is turned off.

	The circuit:
	* Arduino uno
  * 7 segment display (ТМ1637) CLK - pin 2, DIO - pin 3
  * Real time module (DS1307) SCL - pin A5, SDA - pin A4
  * Speaker - pin 5
  * Button 1 - resistor 20 ohm - pin 7
  * Button 2 - resistor 20 ohm - pin 8

	Created 24 february 2019
	By https://arduino.tech
  
*/
#include <Arduino.h>
#include <EEPROM.h>

#include "MelodyPlayer.h"
#include "Display.h"
#include "Clock.h"

#define DISPLAY_CLK_PIN 2
#define DISPLAY_DIO_PIN 3
#define SPEAKER_PIN 5
#define BUTTON_1_PIN 7
#define BUTTON_2_PIN 8

#define LOOP_MILLIS 500

#define STATE_CLOCK_IS_ON 0
#define STATE_TIME_SETTING_HOURS 1
#define STATE_TIME_SETTING_MINUTES 2
#define STATE_ALARM_SETTING_HOURS 3
#define STATE_ALARM_SETTING_MINUTES 4

Display display(DISPLAY_CLK_PIN, DISPLAY_DIO_PIN);
MelodyPlayer melodyPlayer(SPEAKER_PIN);
Clock clock;

int button1OldState = LOW;
int button2OldState = LOW;
int button1NewState = LOW;
int button2NewState = LOW;
unsigned long lastPressButton1Time = 0;
unsigned long lastPressButton2Time = 0;


byte state = 0;

void setup()
{
  pinMode(BUTTON_1_PIN, INPUT);
  pinMode(BUTTON_2_PIN, INPUT);

  melodyPlayer.init();
  display.init();
}

void loop()
{
  static unsigned long lastLoop = 0;
  static uint8_t settingHours;
  static uint8_t settingMinutes;

  

  static int alarmHours = EEPROM.read(0);
  static int alarmMinutes = EEPROM.read(1);

  unsigned long now = millis();
  if (lastLoop > now) {
    lastLoop = 0;
  }

  button1NewState = digitalRead(BUTTON_1_PIN);
  button2NewState = digitalRead(BUTTON_2_PIN);

  if (now > (lastLoop + LOOP_MILLIS)) {
    if (state == STATE_CLOCK_IS_ON) {
      timeclock_t time = clock.getCurrentTime();
      display.showTime(time.hour, time.minute);
      if ((alarmHours == time.hour) && (alarmMinutes == time.minute)) {
        melodyPlayer.playStarwars();
        melodyPlayer.playStarwars();
      }
    } else if ((state == STATE_TIME_SETTING_HOURS) && (button2NewState != HIGH)) {
      display.showTimeSettingHours(settingHours, settingMinutes, true);
    } else if ((state == STATE_TIME_SETTING_MINUTES) && (button2NewState != HIGH)) {
      display.showTimeSettingMinutes(settingHours, settingMinutes, true);
    } else if ((state == STATE_ALARM_SETTING_HOURS) && (button1NewState != HIGH)) {
      display.showAlarmSettingHours(alarmHours, alarmMinutes, true);
    } else if ((state == STATE_ALARM_SETTING_MINUTES) && (button1NewState != HIGH)) {
      display.showAlarmSettingMinutes(alarmHours, alarmMinutes, true);
    }
    lastLoop = now;
  }

  if ((((button1NewState == HIGH) && (button1OldState == LOW) && ((now - lastPressButton1Time) > 500))) ||
     ((button1NewState == HIGH) && ((now - lastPressButton1Time) > 200) && ((state == STATE_ALARM_SETTING_HOURS) || (state == STATE_ALARM_SETTING_MINUTES)))) {
    if (state == STATE_CLOCK_IS_ON) {
      state = STATE_TIME_SETTING_HOURS;
      timeclock_t time = clock.getCurrentTime();
      settingHours = time.hour;
      settingMinutes = time.minute;
    } else if (state == STATE_TIME_SETTING_HOURS) {
      state = STATE_TIME_SETTING_MINUTES;
    } else if (state == STATE_TIME_SETTING_MINUTES) {
      state = STATE_CLOCK_IS_ON;
      clock.setNewCurrentTime(settingHours, settingMinutes);
    } else if (state == STATE_ALARM_SETTING_HOURS) {
      alarmHours++;
      if (alarmHours == 25) {
        alarmHours = 0;
      }
      display.showAlarmSettingHours(alarmHours, alarmMinutes, false);
    } else if (state == STATE_ALARM_SETTING_MINUTES) {
      alarmMinutes++;
      if (alarmMinutes == 61) {
        alarmMinutes = 0;
      }
      display.showAlarmSettingMinutes(alarmHours, alarmMinutes, false);
    }
    button1OldState = HIGH;
    lastPressButton1Time = now;
  } else if ((button1NewState == LOW) && (button1OldState == HIGH)) {
    button1OldState = LOW;
  }


  if ((((button2NewState == HIGH) && (button2OldState == LOW) && ((now - lastPressButton2Time) > 500))) ||
     ((button2NewState == HIGH) && ((now - lastPressButton2Time) > 200) && ((state == STATE_TIME_SETTING_HOURS) || (state == STATE_TIME_SETTING_MINUTES)))) {
    if (state == STATE_CLOCK_IS_ON) {
      state = STATE_ALARM_SETTING_HOURS;
    } else if (state == STATE_ALARM_SETTING_HOURS) {
      state = STATE_ALARM_SETTING_MINUTES;
    } else if (state == STATE_ALARM_SETTING_MINUTES) {
      state = STATE_CLOCK_IS_ON;
      EEPROM.write(0, alarmHours);
      EEPROM.write(1, alarmMinutes);
      if ((alarmHours == 24) || (alarmMinutes == 60)) {
        alarmHours = 24;
        alarmMinutes = 60;
      }
    } else if (state == STATE_TIME_SETTING_HOURS) {
      settingHours++;
      if (settingHours == 24) {
        settingHours = 0;
      }
      display.showTimeSettingHours(settingHours, settingMinutes, false);
    } else if (state == STATE_TIME_SETTING_MINUTES) {
      settingMinutes++;
      if (settingMinutes == 60) {
        settingMinutes = 0;
      }
      display.showTimeSettingMinutes(settingHours, settingMinutes, false);
    }
    button2OldState = HIGH;
    lastPressButton2Time = now;
  } else if (button2NewState == LOW) {
    button2OldState = LOW;
  }
}


