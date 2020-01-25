#include "rtc_persistence.h"

esp_err_t load_configuration(i2c_dev_t *dev, menu_t *menu)
{

    uint8_t buffer[RTC_PERSISTED_VALUES_NUM];

    ds1307_read_ram(dev, 0, buffer, RTC_PERSISTED_VALUES_NUM);

    menu->zone0_timer_setup->interval_0_on_hour = buffer[0];
    menu->zone0_timer_setup->interval_0_on_minute = buffer[1];
    menu->zone0_timer_setup->interval_0_weekday_config = buffer[2];
    menu->zone0_timer_setup->interval_0_off_hour = buffer[3];
    menu->zone0_timer_setup->interval_0_off_minute = buffer[4];

    menu->zone0_timer_setup->interval_1_on_hour = buffer[5];
    menu->zone0_timer_setup->interval_1_on_minute = buffer[6];
    menu->zone0_timer_setup->interval_1_weekday_config = buffer[7];
    menu->zone0_timer_setup->interval_1_off_hour = buffer[8];
    menu->zone0_timer_setup->interval_1_off_minute = buffer[9];

    menu->zone0_timer_setup->interval_2_on_hour = buffer[10];
    menu->zone0_timer_setup->interval_2_on_minute = buffer[11];
    menu->zone0_timer_setup->interval_2_weekday_config = buffer[12];
    menu->zone0_timer_setup->interval_2_off_hour = buffer[13];
    menu->zone0_timer_setup->interval_2_off_minute = buffer[14];

    menu->zone1_timer_setup->interval_0_on_hour = buffer[15];
    menu->zone1_timer_setup->interval_0_on_minute = buffer[16];
    menu->zone1_timer_setup->interval_0_weekday_config = buffer[17];
    menu->zone1_timer_setup->interval_0_off_hour = buffer[18];
    menu->zone1_timer_setup->interval_0_off_minute = buffer[19];

    menu->zone1_timer_setup->interval_1_on_hour = buffer[20];
    menu->zone1_timer_setup->interval_1_on_minute = buffer[21];
    menu->zone1_timer_setup->interval_1_weekday_config = buffer[22];
    menu->zone1_timer_setup->interval_1_off_hour = buffer[23];
    menu->zone1_timer_setup->interval_1_off_minute = buffer[24];

    menu->zone1_timer_setup->interval_2_on_hour = buffer[25];
    menu->zone1_timer_setup->interval_2_on_minute = buffer[26];
    menu->zone1_timer_setup->interval_2_weekday_config = buffer[27];
    menu->zone1_timer_setup->interval_2_off_hour = buffer[28];
    menu->zone1_timer_setup->interval_2_off_minute = buffer[29];

    return ESP_OK;
}

esp_err_t save_configuration(i2c_dev_t *dev, menu_t *menu)
{

    uint8_t buffer[RTC_PERSISTED_VALUES_NUM];

    buffer[0] = menu->zone0_timer_setup->interval_0_on_hour;
    buffer[1] = menu->zone0_timer_setup->interval_0_on_minute;
    buffer[2] = menu->zone0_timer_setup->interval_0_weekday_config;
    buffer[3] = menu->zone0_timer_setup->interval_0_off_hour;
    buffer[4] = menu->zone0_timer_setup->interval_0_off_minute;

    buffer[5] = menu->zone0_timer_setup->interval_1_on_hour;
    buffer[6] = menu->zone0_timer_setup->interval_1_on_minute;
    buffer[7] = menu->zone0_timer_setup->interval_1_weekday_config;
    buffer[8] = menu->zone0_timer_setup->interval_1_off_hour;
    buffer[9] = menu->zone0_timer_setup->interval_1_off_minute;

    buffer[10] = menu->zone0_timer_setup->interval_2_on_hour;
    buffer[11] = menu->zone0_timer_setup->interval_2_on_minute;
    buffer[12] = menu->zone0_timer_setup->interval_2_weekday_config;
    buffer[13] = menu->zone0_timer_setup->interval_2_off_hour;
    buffer[14] = menu->zone0_timer_setup->interval_2_off_minute;

    buffer[15] = menu->zone1_timer_setup->interval_0_on_hour;
    buffer[16] = menu->zone1_timer_setup->interval_0_on_minute;
    buffer[17] = menu->zone1_timer_setup->interval_0_weekday_config;
    buffer[18] = menu->zone1_timer_setup->interval_0_off_hour;
    buffer[19] = menu->zone1_timer_setup->interval_0_off_minute;

    buffer[20] = menu->zone1_timer_setup->interval_1_on_hour;
    buffer[21] = menu->zone1_timer_setup->interval_1_on_minute;
    buffer[22] = menu->zone1_timer_setup->interval_1_weekday_config;
    buffer[23] = menu->zone1_timer_setup->interval_1_off_hour;
    buffer[24] = menu->zone1_timer_setup->interval_1_off_minute;

    buffer[25] = menu->zone1_timer_setup->interval_2_on_hour;
    buffer[26] = menu->zone1_timer_setup->interval_2_on_minute;
    buffer[27] = menu->zone1_timer_setup->interval_2_weekday_config;
    buffer[28] = menu->zone1_timer_setup->interval_2_off_hour;
    buffer[29] = menu->zone1_timer_setup->interval_2_off_minute;

    ds1307_write_ram(dev, 0, buffer, RTC_PERSISTED_VALUES_NUM);

    return ESP_OK;
}