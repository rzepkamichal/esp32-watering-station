#include "menu.h"

esp_err_t handle_idle_state(menu_t *menu, const hd44780_t *lcd, uint8_t gpio_btn)
{

    if (menu->BTN_OK_PIN == gpio_btn || menu->BTN_CON_PIN == gpio_btn)
    {
        menu->state = ZONE_SELECTION;
        menu->continue_count = 0;
        menu->selected_zone = 0;
    }

    return ESP_OK;
}

esp_err_t handle_zone_selection_state(menu_t *menu, const hd44780_t *lcd, uint8_t gpio_btn)
{

    if (menu->BTN_BACK_PIN == gpio_btn)
    {
        menu->state = IDLE;
        menu->continue_count = -1;
    }
    else if (menu->BTN_OK_PIN == gpio_btn)
    {
        menu->state = INTERVAL_SELECTION;
        menu->selected_zone = menu->continue_count % 2;
        menu->selected_interval = 0;
        menu->continue_count = 0;
    }
    else if (menu->BTN_CON_PIN == gpio_btn)
    {
        menu->continue_count = (menu->continue_count + 1) % 2;
        menu->selected_zone = menu->continue_count;
    }

    return ESP_OK;
}

esp_err_t handle_interval_selection_state(menu_t *menu, const hd44780_t *lcd, uint8_t gpio_btn)
{
    if (menu->BTN_BACK_PIN == gpio_btn)
    {
        menu->state = ZONE_SELECTION;
        menu->continue_count = menu->selected_zone;
    }
    else if (menu->BTN_OK_PIN == gpio_btn)
    {
        menu->state = ON_TIME_SELECTION;
        menu->selected_interval = menu->continue_count % 3;
        menu->continue_count = 0;
        menu->time_selection_state = HOUR;
    }
    else if (menu->BTN_CON_PIN == gpio_btn)
    {
        menu->continue_count = (menu->continue_count + 1) % 3;
        menu->selected_interval = menu->continue_count;
    }

    return ESP_OK;
}

esp_err_t handle_on_hour_selection_state(menu_t *menu, const hd44780_t *lcd, uint8_t gpio_btn)
{
    if (menu->BTN_OK_PIN == gpio_btn)
    {
        menu->time_selection_state = MINUTE;
    }
    else if (menu->BTN_CON_PIN == gpio_btn)
    {
        if (menu->selected_zone == 0)
        {

            if (menu->selected_interval == 0)
            {
                menu->zone0_timer_setup->interval_0_on_hour++;
                menu->zone0_timer_setup->interval_0_on_hour %= 24;
            }
            else if (menu->selected_interval == 1)
            {
                menu->zone0_timer_setup->interval_1_on_hour++;
                menu->zone0_timer_setup->interval_1_on_hour %= 24;
            }
            else if (menu->selected_interval == 2)
            {
                menu->zone0_timer_setup->interval_2_on_hour++;
                menu->zone0_timer_setup->interval_2_on_hour %= 24;
            }
        }
        else if (menu->selected_zone == 1)
        {
            if (menu->selected_interval == 0)
            {
                menu->zone1_timer_setup->interval_0_on_hour++;
                menu->zone1_timer_setup->interval_0_on_hour %= 24;
            }
            else if (menu->selected_interval == 1)
            {
                menu->zone1_timer_setup->interval_1_on_hour++;
                menu->zone1_timer_setup->interval_1_on_hour %= 24;
            }
            else if (menu->selected_interval == 2)
            {
                menu->zone1_timer_setup->interval_2_on_hour++;
                menu->zone1_timer_setup->interval_2_on_hour %= 24;
            }
        }
    }
    return ESP_OK;
}

uint8_t resolve_weekday_configuration(menu_t *menu)
{
    uint8_t weekday_configuration = 0;

    if (menu->selected_zone == 0)
    {

        if (menu->selected_interval == 0)
        {
            weekday_configuration = menu->zone0_timer_setup->interval_0_weekday_config;
        }
        else if (menu->selected_interval == 1)
        {
            weekday_configuration = menu->zone0_timer_setup->interval_1_weekday_config;
        }
        else if (menu->selected_interval == 2)
        {
            weekday_configuration = menu->zone0_timer_setup->interval_2_weekday_config;
        }
    }
    else if (menu->selected_zone == 1)
    {
        if (menu->selected_interval == 0)
        {
            weekday_configuration = menu->zone1_timer_setup->interval_0_weekday_config;
        }
        else if (menu->selected_interval == 1)
        {
            weekday_configuration = menu->zone1_timer_setup->interval_1_weekday_config;
        }
        else if (menu->selected_interval == 2)
        {
            weekday_configuration = menu->zone1_timer_setup->interval_2_weekday_config;
        }
    }

    return weekday_configuration;
}

esp_err_t handle_on_minute_selection_state(menu_t *menu, const hd44780_t *lcd, uint8_t gpio_btn)
{
    if (menu->BTN_OK_PIN == gpio_btn)
    {
        menu->state = DAY_SELECTION;
        menu->weekday_selection_state = MONDAY;
        menu->selected_weekday_configuration = resolve_weekday_configuration(menu);
    }
    else if (menu->BTN_CON_PIN == gpio_btn)
    {

        if (menu->selected_zone == 0)
        {

            if (menu->selected_interval == 0)
            {
                menu->zone0_timer_setup->interval_0_on_minute++;
                menu->zone0_timer_setup->interval_0_on_minute %= 60;
            }
            else if (menu->selected_interval == 1)
            {
                menu->zone0_timer_setup->interval_1_on_minute++;
                menu->zone0_timer_setup->interval_1_on_minute %= 60;
            }
            else if (menu->selected_interval == 2)
            {
                menu->zone0_timer_setup->interval_2_on_minute++;
                menu->zone0_timer_setup->interval_2_on_minute %= 60;
            }
        }
        else if (menu->selected_zone == 1)
        {
            if (menu->selected_interval == 0)
            {
                menu->zone1_timer_setup->interval_0_on_minute++;
                menu->zone1_timer_setup->interval_0_on_minute %= 60;
            }
            else if (menu->selected_interval == 1)
            {
                menu->zone1_timer_setup->interval_1_on_minute++;
                menu->zone1_timer_setup->interval_1_on_minute %= 60;
            }
            else if (menu->selected_interval == 2)
            {
                menu->zone1_timer_setup->interval_2_on_minute++;
                menu->zone1_timer_setup->interval_2_on_minute %= 60;
            }
        }
    }
    return ESP_OK;
}

esp_err_t handle_off_hour_selection_state(menu_t *menu, const hd44780_t *lcd, uint8_t gpio_btn)
{
    if (menu->BTN_OK_PIN == gpio_btn)
    {
        menu->time_selection_state = MINUTE;
    }
    else if (menu->BTN_CON_PIN == gpio_btn)
    {
        if (menu->selected_zone == 0)
        {

            if (menu->selected_interval == 0)
            {
                menu->zone0_timer_setup->interval_0_off_hour++;
                menu->zone0_timer_setup->interval_0_off_hour %= 24;
            }
            else if (menu->selected_interval == 1)
            {
                menu->zone0_timer_setup->interval_1_off_hour++;
                menu->zone0_timer_setup->interval_1_off_hour %= 24;
            }
            else if (menu->selected_interval == 2)
            {
                menu->zone0_timer_setup->interval_2_off_hour++;
                menu->zone0_timer_setup->interval_2_off_hour %= 24;
            }
        }
        else if (menu->selected_zone == 1)
        {
            if (menu->selected_interval == 0)
            {
                menu->zone1_timer_setup->interval_0_off_hour++;
                menu->zone1_timer_setup->interval_0_off_hour %= 24;
            }
            else if (menu->selected_interval == 1)
            {
                menu->zone1_timer_setup->interval_1_off_hour++;
                menu->zone1_timer_setup->interval_1_off_hour %= 24;
            }
            else if (menu->selected_interval == 2)
            {
                menu->zone1_timer_setup->interval_2_off_hour++;
                menu->zone1_timer_setup->interval_2_off_hour %= 24;
            }
        }
    }
    return ESP_OK;
}

esp_err_t handle_off_minute_selection_state(menu_t *menu, const hd44780_t *lcd, uint8_t gpio_btn)
{

    if (menu->BTN_OK_PIN == gpio_btn)
    {
        menu->state = INTERVAL_SELECTION;
    }
    else if (menu->BTN_CON_PIN == gpio_btn)
    {
        if (menu->selected_zone == 0)
        {

            if (menu->selected_interval == 0)
            {
                menu->zone0_timer_setup->interval_0_off_minute++;
                menu->zone0_timer_setup->interval_0_off_minute %= 60;
            }
            else if (menu->selected_interval == 1)
            {
                menu->zone0_timer_setup->interval_1_off_minute++;
                menu->zone0_timer_setup->interval_1_off_minute %= 60;
            }
            else if (menu->selected_interval == 2)
            {
                menu->zone0_timer_setup->interval_2_off_minute++;
                menu->zone0_timer_setup->interval_2_off_minute %= 60;
            }
        }
        else if (menu->selected_zone == 1)
        {
            if (menu->selected_interval == 0)
            {
                menu->zone1_timer_setup->interval_0_off_minute++;
                menu->zone1_timer_setup->interval_0_off_minute %= 60;
            }
            else if (menu->selected_interval == 1)
            {
                menu->zone1_timer_setup->interval_1_off_minute++;
                menu->zone1_timer_setup->interval_1_off_minute %= 60;
            }
            else if (menu->selected_interval == 2)
            {
                menu->zone1_timer_setup->interval_2_off_minute++;
                menu->zone1_timer_setup->interval_2_off_minute %= 60;
            }
        }
    }
    return ESP_OK;
}

esp_err_t handle_any_time_selection_state(menu_t *menu, const hd44780_t *lcd, uint8_t gpio_btn)
{
    if (menu->BTN_BACK_PIN == gpio_btn)
    {

        if (ON_TIME_SELECTION == menu->state)
        {
            menu->state = INTERVAL_SELECTION;
        }
        else if (OFF_TIME_SELECTION == menu->state)
        {
            menu->state = DAY_SELECTION;
            menu->weekday_selection_state = MONDAY;
        }
        menu->continue_count = menu->selected_interval;
    }

    if (ON_TIME_SELECTION == menu->state)
    {
        if (HOUR == menu->time_selection_state)
        {
            handle_on_hour_selection_state(menu, lcd, gpio_btn);
        }
        else if (MINUTE == menu->time_selection_state)
        {
            handle_on_minute_selection_state(menu, lcd, gpio_btn);
        }
    }
    else if (OFF_TIME_SELECTION == menu->state)
    {
        if (HOUR == menu->time_selection_state)
        {
            handle_off_hour_selection_state(menu, lcd, gpio_btn);
        }
        else if (MINUTE == menu->time_selection_state)
        {
            handle_off_minute_selection_state(menu, lcd, gpio_btn);
        }
    }

    return ESP_OK;
}

esp_err_t menu_handle_weekday_state_toggle(menu_t *menu)
{
    uint8_t weekday_config[7];
    uint8_t encoded_value = 0;

    if (menu->selected_zone == 0)
    {
        if (menu->selected_interval == 0)
        {
            decode(menu->zone0_timer_setup->interval_0_weekday_config, weekday_config);
            weekday_config[menu->weekday_selection_state] =
                (weekday_config[menu->weekday_selection_state] + 1) % 2;
            encoded_value = encode(weekday_config);
            menu->zone0_timer_setup->interval_0_weekday_config = encoded_value;
        }
        else if (menu->selected_interval == 1)
        {
            decode(menu->zone0_timer_setup->interval_1_weekday_config, weekday_config);
            weekday_config[menu->weekday_selection_state] =
                (weekday_config[menu->weekday_selection_state] + 1) % 2;
            encoded_value = encode(weekday_config);
            menu->zone0_timer_setup->interval_1_weekday_config = encode(weekday_config);
        }
        else if (menu->selected_interval == 2)
        {
            decode(menu->zone0_timer_setup->interval_2_weekday_config, weekday_config);
            weekday_config[menu->weekday_selection_state] =
                (weekday_config[menu->weekday_selection_state] + 1) % 2;
            encoded_value = encode(weekday_config);
            menu->zone0_timer_setup->interval_2_weekday_config = encode(weekday_config);
        }
    }
    else if (menu->selected_zone == 1)
    {
        if (menu->selected_interval == 0)
        {
            decode(menu->zone1_timer_setup->interval_0_weekday_config, weekday_config);
            weekday_config[menu->weekday_selection_state] =
                (weekday_config[menu->weekday_selection_state] + 1) % 2;
            encoded_value = encode(weekday_config);
            menu->zone1_timer_setup->interval_0_weekday_config = encode(weekday_config);
        }
        else if (menu->selected_interval == 1)
        {
            decode(menu->zone1_timer_setup->interval_1_weekday_config, weekday_config);
            weekday_config[menu->weekday_selection_state] =
                (weekday_config[menu->weekday_selection_state] + 1) % 2;
            encoded_value = encode(weekday_config);
            menu->zone1_timer_setup->interval_1_weekday_config = encode(weekday_config);
        }
        else if (menu->selected_interval == 2)
        {
            decode(menu->zone1_timer_setup->interval_2_weekday_config, weekday_config);
            weekday_config[menu->weekday_selection_state] =
                (weekday_config[menu->weekday_selection_state] + 1) % 2;
            encoded_value = encode(weekday_config);
            menu->zone1_timer_setup->interval_2_weekday_config = encode(weekday_config);
        }
    }

    menu->selected_weekday_configuration = encoded_value;

    return ESP_OK;
}

esp_err_t menu_handle_weekday_selection_state(menu_t *menu, const hd44780_t *lcd, uint8_t gpio_btn)
{
    if (menu->BTN_BACK_PIN == gpio_btn)
    {
        menu->state = ON_TIME_SELECTION;
        menu->time_selection_state = HOUR;
    }
    else if (menu->BTN_OK_PIN == gpio_btn)
    {
        menu_handle_weekday_state_toggle(menu);
    }
    else if (menu->BTN_CON_PIN == gpio_btn)
    {
        if (menu->weekday_selection_state == 6)
        {
            menu->state = OFF_TIME_SELECTION;
            menu->time_selection_state = HOUR;
        }
        else
        {
            menu->weekday_selection_state = (menu->weekday_selection_state + 1) % 7;
        }
    }

    return ESP_OK;
}

esp_err_t menu_handle_btn(menu_t *menu, const hd44780_t *lcd, uint8_t gpio_btn)
{

    if (IDLE == menu->state)
    {
        handle_idle_state(menu, lcd, gpio_btn);
    }
    else if (ZONE_SELECTION == menu->state)
    {
        handle_zone_selection_state(menu, lcd, gpio_btn);
    }
    else if (INTERVAL_SELECTION == menu->state)
    {
        handle_interval_selection_state(menu, lcd, gpio_btn);
    }
    else if (DAY_SELECTION == menu->state)
    {
        menu_handle_weekday_selection_state(menu, lcd, gpio_btn);
    }
    else if (ON_TIME_SELECTION == menu->state || OFF_TIME_SELECTION == menu->state)
    {
        handle_any_time_selection_state(menu, lcd, gpio_btn);
    }

    return ESP_OK;
}

esp_err_t display_idle(menu_t *menu, const hd44780_t *lcd)
{
    hd44780_clear(lcd);
    hd44780_gotoxy(lcd, 0, 0);
    char display_data[17];

    current_time_t *tm = menu->current_time;
    snprintf(display_data, 17, "%02d.%02d.%04d %02d:%02d",
             tm->day, tm->month, tm->year, tm->hour, tm->minute);
    hd44780_puts(lcd, display_data);
    hd44780_gotoxy(lcd, 0, 1);
    snprintf(display_data, 17, "RAIN: %02.2f mm", menu->rain_3h_mm);
    hd44780_puts(lcd, display_data);

    return ESP_OK;
}

esp_err_t display_zone_selection(menu_t *menu, const hd44780_t *lcd)
{
    hd44780_clear(lcd);
    hd44780_gotoxy(lcd, 0, 0);
    hd44780_puts(lcd, ZONE_SELECTION_MSG);
    hd44780_gotoxy(lcd, 0, 1);

    char display_data[9];
    snprintf(display_data, 9, "Zone: %d", menu->selected_zone % 2);
    hd44780_puts(lcd, display_data);

    return ESP_OK;
}

esp_err_t display_interval_selection(menu_t *menu, const hd44780_t *lcd)
{
    hd44780_clear(lcd);
    hd44780_gotoxy(lcd, 0, 0);
    hd44780_puts(lcd, INTERVAL_SELECTION_MSG);
    hd44780_gotoxy(lcd, 0, 1);

    char display_data[15];
    snprintf(display_data, 15, "Interval: %d", menu->selected_interval);
    hd44780_puts(lcd, display_data);

    return ESP_OK;
}

esp_err_t display_any_time_selection(menu_t *menu, const hd44780_t *lcd)
{
    const uint8_t buffer_size = 10;
    char display_data[buffer_size];

    hd44780_clear(lcd);
    hd44780_gotoxy(lcd, 0, 0);

    uint8_t hour = 0;
    uint8_t minute = 0;

    if (ON_TIME_SELECTION == menu->state)
    {
        hd44780_puts(lcd, ON_TIME_SELECTION_MSG);
        hd44780_gotoxy(lcd, 0, 1);

        if (menu->selected_zone == 0)
        {
            if (menu->selected_interval == 0)
            {
                hour = menu->zone0_timer_setup->interval_0_on_hour;
                minute = menu->zone0_timer_setup->interval_0_on_minute;
            }
            else if (menu->selected_interval == 1)
            {
                hour = menu->zone0_timer_setup->interval_1_on_hour;
                minute = menu->zone0_timer_setup->interval_1_on_minute;
            }
            else if (menu->selected_interval == 2)
            {
                hour = menu->zone0_timer_setup->interval_2_on_hour;
                minute = menu->zone0_timer_setup->interval_2_on_minute;
            }
        }
        else if (menu->selected_zone == 1)
        {
            if (menu->selected_interval == 0)
            {
                hour = menu->zone1_timer_setup->interval_0_on_hour;
                minute = menu->zone1_timer_setup->interval_0_on_minute;
            }
            else if (menu->selected_interval == 1)
            {
                hour = menu->zone1_timer_setup->interval_1_on_hour;
                minute = menu->zone1_timer_setup->interval_1_on_minute;
            }
            else if (menu->selected_interval == 2)
            {
                hour = menu->zone1_timer_setup->interval_2_on_hour;
                minute = menu->zone1_timer_setup->interval_2_on_minute;
            }
        }
    }
    else if (OFF_TIME_SELECTION == menu->state)
    {
        hd44780_puts(lcd, OFF_TIME_SELECTION_MSG);
        hd44780_gotoxy(lcd, 0, 1);

        if (menu->selected_zone == 0)
        {

            if (menu->selected_interval == 0)
            {
                hour = menu->zone0_timer_setup->interval_0_off_hour;
                minute = menu->zone0_timer_setup->interval_0_off_minute;
            }
            else if (menu->selected_interval == 1)
            {
                hour = menu->zone0_timer_setup->interval_1_off_hour;
                minute = menu->zone0_timer_setup->interval_1_off_minute;
            }
            else if (menu->selected_interval == 2)
            {
                hour = menu->zone0_timer_setup->interval_2_off_hour;
                minute = menu->zone0_timer_setup->interval_2_off_minute;
            }
        }
        else if (menu->selected_zone == 1)
        {
            if (menu->selected_interval == 0)
            {
                hour = menu->zone1_timer_setup->interval_0_off_hour;
                minute = menu->zone1_timer_setup->interval_0_off_minute;
            }
            else if (menu->selected_interval == 1)
            {
                hour = menu->zone1_timer_setup->interval_1_off_hour;
                minute = menu->zone1_timer_setup->interval_1_off_minute;
            }
            else if (menu->selected_interval == 2)
            {
                hour = menu->zone1_timer_setup->interval_2_off_hour;
                minute = menu->zone1_timer_setup->interval_2_off_minute;
            }
        }
    }

    snprintf(display_data, buffer_size, "%02d:%02d", hour, minute);

    hd44780_puts(lcd, display_data);
    if (HOUR == menu->time_selection_state)
    {
        hd44780_gotoxy(lcd, 1, 1);
    }
    else if (MINUTE)
    {
        hd44780_gotoxy(lcd, 4, 1);
    }

    hd44780_control(lcd, true, true, true);
    return ESP_OK;
}

esp_err_t display_weekday_selecion(menu_t *menu, const hd44780_t *lcd)
{
    hd44780_clear(lcd);
    hd44780_gotoxy(lcd, 0, 0);
    hd44780_puts(lcd, DAY_SELECTION_MSG);

    uint8_t weekday_configuration[7];
    decode(menu->selected_weekday_configuration, weekday_configuration);

    for (uint8_t i = 0; i < WEEKDAY_COUNT; i++)
    {
        hd44780_gotoxy(lcd, 2 * i + 1, 1);

        if (weekday_configuration[i] == 1)
        {
            hd44780_puts(lcd, weekday_names[i]);
        }
        else
        {
            hd44780_puts(lcd, "*");
        }
    }

    hd44780_gotoxy(lcd, 2 * (menu->weekday_selection_state) + 1, 1);
    hd44780_control(lcd, true, true, true);

    return ESP_OK;
}

esp_err_t menu_flush_display(menu_t *menu, const hd44780_t *lcd)
{

    hd44780_control(lcd, true, false, false);

    if (IDLE == menu->state)
    {
        display_idle(menu, lcd);
    }
    else if (ZONE_SELECTION == menu->state)
    {
        display_zone_selection(menu, lcd);
    }
    else if (INTERVAL_SELECTION == menu->state)
    {
        display_interval_selection(menu, lcd);
    }
    else if (ON_TIME_SELECTION == menu->state)
    {
        display_any_time_selection(menu, lcd);
    }
    else if (DAY_SELECTION == menu->state)
    {
        display_weekday_selecion(menu, lcd);
    }
    else if (OFF_TIME_SELECTION == menu->state)
    {
        display_any_time_selection(menu, lcd);
    }

    return ESP_OK;
}

void menu_decode_time(menu_t *menu, const struct tm *tm)
{
    menu->current_time->day = tm->tm_mday;
    menu->current_time->month = tm->tm_mon;
    menu->current_time->year = tm->tm_year;
    menu->current_time->hour = tm->tm_hour;
    menu->current_time->minute = tm->tm_min;
    menu->current_time->weekday = (tm->tm_wday - 1 + 7) % 7;
}

void menu_encode_time(menu_t *menu, struct tm *tm)
{

    tm->tm_mday = menu->current_time->day;
    tm->tm_mon = menu->current_time->month;
    tm->tm_year = menu->current_time->year;
    tm->tm_hour = menu->current_time->hour;
    tm->tm_min = menu->current_time->minute;
    tm->tm_wday = (menu->current_time->weekday + 1) % 7;
    tm->tm_sec = 0;
}