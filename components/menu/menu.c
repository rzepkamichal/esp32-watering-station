#include "menu.h"

esp_err_t handle_idle_state(menu_t *menu, const hd44780_t *lcd, uint8_t gpio_btn)
{

    if (menu->BTN_OK_PIN == gpio_btn || menu->BTN_CON_PIN == gpio_btn)
    {
        menu->state = ZONE_SELECTION;
        menu->continue_count = 0;
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
        menu->continue_count = 0;
    }
    else if (menu->BTN_CON_PIN == gpio_btn)
    {
        menu->continue_count = (menu->continue_count + 1) % 2;
    }

    return ESP_OK;
}

esp_err_t handle_interval_selection_state(menu_t *menu, const hd44780_t *lcd, uint8_t gpio_btn)
{
    if (menu->BTN_BACK_PIN == gpio_btn)
    {
        menu->state = ZONE_SELECTION;
        menu->continue_count = 0;
        menu->selected_zone = -1;
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
    }

    return ESP_OK;
}

esp_err_t handle_on_time_selection_state(menu_t *menu, const hd44780_t *lcd, uint8_t gpio_btn)
{
    if (menu->BTN_BACK_PIN == gpio_btn)
    {
        menu->state = ZONE_SELECTION;
        menu->continue_count = 0;
        menu->selected_zone = -1;
    }
    else if (menu->BTN_OK_PIN == gpio_btn)
    {
        menu->state = INTERVAL_SELECTION;
    }
    else if (menu->BTN_CON_PIN == gpio_btn)
    {
        menu->continue_count = (menu->continue_count + 1) % 3;
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
    else if (ON_TIME_SELECTION == menu->state)
    {
    }
    else if (OFF_TIME_SELECTION == menu->state)
    {
    }

    return ESP_OK;
}

esp_err_t display_idle(menu_t *menu, const hd44780_t *lcd)
{
    hd44780_clear(lcd);
    hd44780_gotoxy(lcd, 0, 0);
    hd44780_puts(lcd, "IDLE");

    return ESP_OK;
}

esp_err_t display_zone_selection(menu_t *menu, const hd44780_t *lcd)
{
    hd44780_clear(lcd);
    hd44780_gotoxy(lcd, 0, 0);
    hd44780_puts(lcd, ZONE_SELECTION_MSG);
    hd44780_gotoxy(lcd, 0, 1);

    char display_data[9];
    snprintf(display_data, 9, "Zone: %d", menu->continue_count % 2);
    hd44780_puts(lcd, display_data);

    return ESP_OK;
}

esp_err_t display_interval_selection(menu_t *menu, const hd44780_t *lcd)
{
    hd44780_clear(lcd);
    hd44780_gotoxy(lcd, 0, 0);
    hd44780_puts(lcd, INTERVAL_SELECTION_MSG);
    hd44780_gotoxy(lcd, 0, 1);

    char display_data[13];
    snprintf(display_data, 13, "Interval: %d", menu->continue_count % 3);
    hd44780_puts(lcd, display_data);

    return ESP_OK;
}

esp_err_t menu_flush_display(menu_t *menu, const hd44780_t *lcd)
{

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
    }
    else if (OFF_TIME_SELECTION == menu->state)
    {
    }

    return ESP_OK;
}