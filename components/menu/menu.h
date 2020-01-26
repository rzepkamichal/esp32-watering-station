#ifndef __MENU_H__
#define __MENU_H__

#include <stdint.h>
#include <hd44780.h>
#include <timer_setup.h>
#include <weekday_conversion.h>
#include <time.h>

#define ZONE_SELECTION_MSG "Choose zone"
#define INTERVAL_SELECTION_MSG "Choose interval"
#define ON_TIME_SELECTION_MSG "Set ON time"
#define OFF_TIME_SELECTION_MSG "Set OFF time"
#define DAY_SELECTION_MSG "Set active days"

#define LIGHT_RAIN_MSG "Light"
#define MODERATE_RAIN_MSG "Moderate"
#define HEAVY_RAIN_MSG "Heavy"
#define NO_RAIN_MSG "None"


typedef enum MenuState
{
    IDLE = 0,
    ZONE_SELECTION = 1,
    INTERVAL_SELECTION = 2,
    ON_TIME_SELECTION = 3,
    DAY_SELECTION = 4,
    OFF_TIME_SELECTION = 5
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

static char weekday_names[7][2] = {
    {'M', '\0'},
    {'T', '\0'},
    {'W', '\0'},
    {'T', '\0'},
    {'F', '\0'},
    {'S', '\0'},
    {'S', '\0'},
};

typedef struct
{

    int day;
    int month;
    int year;
    int hour;
    int minute;
    int weekday;

} current_time_t;

typedef struct
{
    uint8_t state;
    uint8_t time_selection_state;
    uint8_t weekday_selection_state;
    int8_t continue_count;
    int8_t selected_zone;
    int8_t selected_interval;
    uint8_t selected_weekday_configuration;
    const uint8_t BTN_BACK_PIN;
    const uint8_t BTN_OK_PIN;
    const uint8_t BTN_CON_PIN;
    timer_setup_t *zone0_timer_setup;
    timer_setup_t *zone1_timer_setup;
    uint8_t current_weekday;
    current_time_t *current_time;
    double rain_3h_mm;

} menu_t;

esp_err_t menu_handle_btn(menu_t *menu, const hd44780_t *lcd, uint8_t gpio_btn);
esp_err_t menu_flush_display(menu_t *menu, const hd44780_t *lcd);
void menu_decode_time(menu_t *menu, const struct tm *tm);
void menu_encode_time(menu_t *menu, struct tm *tm);

#endif