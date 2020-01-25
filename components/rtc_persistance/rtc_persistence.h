#ifndef __RTC_PERSISTENCE_H__
#define __RTC_PERSISTENCE_H__

#include "menu.h"
#include <ds1307.h>
#include "driver/periph_ctrl.h"
#include "driver/timer.h"

//number of configuration-bytes stored in RTC ram
#define RTC_PERSISTED_VALUES_NUM 30

esp_err_t load_configuration(i2c_dev_t *dev, menu_t *menu);
esp_err_t save_configuration(i2c_dev_t *dev, menu_t *menu);

#endif