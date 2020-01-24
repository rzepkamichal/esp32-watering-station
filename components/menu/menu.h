#ifndef __MENU_H__
#define __MENU_H__

#include <stdint.h>
#include <hd44780.h>
#include <timer_setup.h>
#include <weekday_conversion.h>

#define ZONE_SELECTION_MSG "Choose zone"
#define INTERVAL_SELECTION_MSG "Choose interval"
#define ON_TIME_SELECTION_MSG "Set ON time"
#define OFF_TIME_SELECTION_MSG "Set OFF time"
#define DAY_SELECTION_MSG "Set days"

typedef enum MenuState
{
    IDLE = 0,
    ZONE_SELECTION = 1,
    INTERVAL_SELECTION = 2,
    ON_TIME_SELECTION = 3,
    OFF_TIME_SELECTION = 4,
    DAY_SELECTION = 5
} menu_state;

typedef enum TimeSelectionState
{
    HOUR = 0,
    MINUTE = 1
} time_selection_state;

typedef enum WeekDaySelectionState
{
    MONDAY = 0,
    TUESDAY = 1,
    WEDNESDAY = 2,
    THURSDAY = 3,
    FRIDAY = 4,
    SATURDAY = 5,
    SUNDAY = 6,
    WEEKDAY_COUNT = 7
} week_day_selection_state;

typedef struct
{
    uint8_t state;
    uint8_t time_selection_state;
    uint8_t weekday_selection_state;
    int8_t continue_count;
    int8_t selected_zone;
    int8_t selected_interval;
    const uint8_t BTN_BACK_PIN;
    const uint8_t BTN_OK_PIN;
    const uint8_t BTN_CON_PIN;
    timer_setup_t *timer_setup;

} menu_t;

esp_err_t menu_handle_btn(menu_t *menu, const hd44780_t *lcd, uint8_t gpio_btn);
esp_err_t menu_flush_display(menu_t *menu, const hd44780_t *lcd);

#endif