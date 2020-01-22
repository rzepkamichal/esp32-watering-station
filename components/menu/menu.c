#include "menu.h"

esp_err_t handle_idle_state(menu_t *menu, const hd44780_t *lcd, uint8_t gpio_btn)
{

    if (menu->BTN_OK_PIN == gpio_btn)
    {
        hd44780_gotoxy(lcd, 0, 0);
        hd44780_clear(lcd);
        hd44780_puts(lcd, ZONE_SELECTION_MSG);
        menu->state = ZONE_SELECTION;
    }
    else
    {
        hd44780_gotoxy(lcd, 0, 0);
        hd44780_clear(lcd);
        hd44780_puts(lcd, "IDLE");
    }

    return ESP_OK;
}

esp_err_t handle_zone_selection_state(menu_t *menu, const hd44780_t *lcd, uint8_t gpio_btn)
{

    if (menu->BTN_BACK_PIN == gpio_btn)
    {
        hd44780_gotoxy(lcd, 0, 0);
        hd44780_clear(lcd);
        menu->state = IDLE;
        hd44780_puts(lcd, "IDLE");
    }
    else if (menu->BTN_OK_PIN == gpio_btn)
    {
        hd44780_gotoxy(lcd, 0, 0);
        hd44780_clear(lcd);
        hd44780_puts(lcd, "OK");
        menu->state = ZONE_SELECTION;
    }else if (menu->BTN_CON_PIN == gpio_btn)
    {
        hd44780_gotoxy(lcd, 0, 0);
        hd44780_clear(lcd);
        hd44780_puts(lcd, "CON");
        menu->state = ZONE_SELECTION;
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
    else if (ON_TIME_SELECTION == menu->state)
    {
    }
    else if (OFF_TIME_SELECTION == menu->state)
    {
    }

    return ESP_OK;
}