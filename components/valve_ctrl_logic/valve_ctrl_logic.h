#ifndef __VALVE_VTRL_LOGIC_H__
#define __VALVE_VTRL_LOGIC_H__

#include <stdint.h>
#include "menu.h"
#include "weekday_conversion.h"

#define RAIN_MM_3h_HEAVY 20
#define RAIN_MM_3h_MODERATE 10

uint8_t should_open(current_time_t *current_time, int checked_on_hour, int checked_on_minute, int checked_weekday_config, int checked_off_hour, int checked_off_minute)
{

    
    int current_hour = current_time->hour;
    int current_minute = current_time->minute;
    int current_weekday = current_time->weekday;

    uint8_t weekday_setup[7];
    decode(checked_weekday_config, weekday_setup);

    int on_time_val = checked_on_hour * 60 + checked_on_minute;
    int off_time_val = checked_off_hour * 60 + checked_off_minute;
    int current_time_val = current_hour * 60 + current_minute;
    uint8_t today_config = weekday_setup[current_weekday];

    if (current_time_val >= on_time_val && current_time_val < off_time_val && today_config == 1)
    {
        return 1;
    }

    return 0;
}

uint8_t should_open_zone0(menu_t *menu)
{


    int checked_on_hour = menu->zone0_timer_setup->interval_0_on_hour;
    int checked_on_minute = menu->zone0_timer_setup->interval_0_on_minute;
    int checked_weekday_config = menu->zone0_timer_setup->interval_0_weekday_config;
    int checked_off_hour = menu->zone0_timer_setup->interval_0_off_hour;
    int checked_off_minute = menu->zone0_timer_setup->interval_0_off_minute;

    if (should_open(menu->current_time, checked_on_hour, checked_on_minute,
                    checked_weekday_config, checked_off_hour, checked_off_minute))
    {
        return 1;
    }

    checked_on_hour = menu->zone0_timer_setup->interval_1_on_hour;
    checked_on_minute = menu->zone0_timer_setup->interval_1_on_minute;
    checked_weekday_config = menu->zone0_timer_setup->interval_1_weekday_config;
    checked_off_hour = menu->zone0_timer_setup->interval_1_off_hour;
    checked_off_minute = menu->zone0_timer_setup->interval_1_off_minute;

    if (should_open(menu->current_time, checked_on_hour, checked_on_minute,
                    checked_weekday_config, checked_off_hour, checked_off_minute))
    {
        return 1;
    }

    checked_on_hour = menu->zone0_timer_setup->interval_2_on_hour;
    checked_on_minute = menu->zone0_timer_setup->interval_2_on_minute;
    checked_weekday_config = menu->zone0_timer_setup->interval_2_weekday_config;
    checked_off_hour = menu->zone0_timer_setup->interval_2_off_hour;
    checked_off_minute = menu->zone0_timer_setup->interval_2_off_minute;

    if (should_open(menu->current_time, checked_on_hour, checked_on_minute,
                    checked_weekday_config, checked_off_hour, checked_off_minute))
    {
        return 1;
    }

    return 0;
}

uint8_t should_open_zone1(menu_t *menu)
{
    int checked_on_hour = menu->zone1_timer_setup->interval_0_on_hour;
    int checked_on_minute = menu->zone1_timer_setup->interval_0_on_minute;
    int checked_weekday_config = menu->zone1_timer_setup->interval_0_weekday_config;
    int checked_off_hour = menu->zone1_timer_setup->interval_0_off_hour;
    int checked_off_minute = menu->zone1_timer_setup->interval_0_off_minute;

    if (should_open(menu->current_time, checked_on_hour, checked_on_minute,
                    checked_weekday_config, checked_off_hour, checked_off_minute))
    {
        return 1;
    }

    checked_on_hour = menu->zone1_timer_setup->interval_1_on_hour;
    checked_on_minute = menu->zone1_timer_setup->interval_1_on_minute;
    checked_weekday_config = menu->zone1_timer_setup->interval_1_weekday_config;
    checked_off_hour = menu->zone1_timer_setup->interval_1_off_hour;
    checked_off_minute = menu->zone1_timer_setup->interval_1_off_minute;

    if (should_open(menu->current_time, checked_on_hour, checked_on_minute,
                    checked_weekday_config, checked_off_hour, checked_off_minute))
    {
        return 1;
    }

    checked_on_hour = menu->zone1_timer_setup->interval_2_on_hour;
    checked_on_minute = menu->zone1_timer_setup->interval_2_on_minute;
    checked_weekday_config = menu->zone1_timer_setup->interval_2_weekday_config;
    checked_off_hour = menu->zone1_timer_setup->interval_2_off_hour;
    checked_off_minute = menu->zone1_timer_setup->interval_2_off_minute;

    if (should_open(menu->current_time, checked_on_hour, checked_on_minute,
                    checked_weekday_config, checked_off_hour, checked_off_minute))
    {
        return 1;
    }

    return 0;
}

#endif
