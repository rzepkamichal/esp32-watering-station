#ifndef __TIMER_SETUP_H__
#define __TIMER_SETUP_H__

#include <stdint.h>

typedef struct
{
    uint8_t interval_0_on_hour;
    uint8_t interval_0_on_minute;
    uint8_t interval_0_weekday_config;
    uint8_t interval_0_off_hour;
    uint8_t interval_0_off_minute;

    uint8_t interval_1_on_hour;
    uint8_t interval_1_on_minute;
    uint8_t interval_1_weekday_config;
    uint8_t interval_1_off_hour;
    uint8_t interval_1_off_minute;

    uint8_t interval_2_on_hour;
    uint8_t interval_2_on_minute;
    uint8_t interval_2_weekday_config;
    uint8_t interval_2_off_hour;
    uint8_t interval_2_off_minute;

} timer_setup_t;

#endif