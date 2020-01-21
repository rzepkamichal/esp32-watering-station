#ifndef __MENU_H__
#define __MENU_H__

#define BTN_BACK_PIN 27
#define BTN_OK_PIN 14
#define BTN_CON_PIN 12

enum MenuState {IDLE = 0, ZONE_SELECTION = 1, INTERVAL_SELECTION = 2, ON_TIME_SELECTION = 3, OFF_TIME_SELECTION = 4 };

#endif