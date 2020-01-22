#ifndef __MENU_H__
#define __MENU_H__

#include <stdint.h>
#include <hd44780.h>

#define ZONE_SELECTION_MSG "Choose zone"
#define INTERVAL_SELECTION_MSG "Choose interval"
#define ON_TIME_SELECTION_MSG "Choose ON time"
#define OFF_TIME_SELECTION_MSG "Choose OFF time"
#define DAY_SELECTION_MSG "Choose weekday"

typedef enum MenuState
{
    IDLE = 0,
    ZONE_SELECTION = 1,
    INTERVAL_SELECTION = 2,
    ON_TIME_SELECTION = 3,
    OFF_TIME_SELECTION = 4
} menu_state;

typedef struct
{
    uint8_t state;
    uint8_t continue_count;
    const uint8_t BTN_BACK_PIN;
    const uint8_t BTN_OK_PIN;
    const uint8_t BTN_CON_PIN;
} menu_t;

esp_err_t menu_handle_btn(menu_t *menu, const hd44780_t *lcd, uint8_t gpio_btn);

#endif