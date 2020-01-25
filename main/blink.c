// /* WiFi station Example
//    This example code is in the Public Domain (or CC0 licensed, at your option.)
//    Unless required by applicable law or agreed to in writing, this
//    software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
//    CONDITIONS OF ANY KIND, either express or implied.
// */
// #include <string.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "freertos/event_groups.h"
// #include "esp_system.h"
// #include "esp_wifi.h"
// #include "esp_event.h"
// #include "esp_log.h"
// #include "nvs_flash.h"

// #include "lwip/err.h"
// #include "lwip/sys.h"

// /* The examples use WiFi configuration that you can set via project configuration menu
//    If you'd rather not, just change the below entries to strings with
//    the config you want - ie #define EXAMPLE_WIFI_SSID "mywifissid"
// */
// #define EXAMPLE_ESP_WIFI_SSID      "Michal_Rz"
// #define EXAMPLE_ESP_WIFI_PASS      "6d6d6d6d72727a65706b613939"
// #define EXAMPLE_ESP_MAXIMUM_RETRY  5

// /* FreeRTOS event group to signal when we are connected*/
// static EventGroupHandle_t s_wifi_event_group;

// /* The event group allows multiple bits for each event, but we only care about two events:
//  * - we are connected to the AP with an IP
//  * - we failed to connect after the maximum amount of retries */
// #define WIFI_CONNECTED_BIT BIT0
// #define WIFI_FAIL_BIT      BIT1

// static const char *TAG = "wifi station";

// static int s_retry_num = 0;

// static void event_handler(void* arg, esp_event_base_t event_base,
//                                 int32_t event_id, void* event_data)
// {
//     if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
//         esp_wifi_connect();
//     } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
//         if (s_retry_num < EXAMPLE_ESP_MAXIMUM_RETRY) {
//             esp_wifi_connect();
//             s_retry_num++;
//             ESP_LOGI(TAG, "retry to connect to the AP");
//         } else {
//             xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
//         }
//         ESP_LOGI(TAG,"connect to the AP fail");
//     } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
//         ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
//         ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
//         s_retry_num = 0;
//         xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
//     }
// }

// void wifi_init_sta(void)
// {
//     s_wifi_event_group = xEventGroupCreate();

//     ESP_ERROR_CHECK(esp_netif_init());

//     ESP_ERROR_CHECK(esp_event_loop_create_default());
//     esp_netif_create_default_wifi_sta();

//     wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
//     ESP_ERROR_CHECK(esp_wifi_init(&cfg));

//     ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
//     ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL));

//     wifi_config_t wifi_config = {
//         .sta = {
//             .ssid = EXAMPLE_ESP_WIFI_SSID,
//             .password = EXAMPLE_ESP_WIFI_PASS
//         },
//     };
//     ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
//     ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
//     ESP_ERROR_CHECK(esp_wifi_start() );

//     ESP_LOGI(TAG, "wifi_init_sta finished.");

//     /* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for the maximum
//      * number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler() (see above) */
//     EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
//             WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
//             pdFALSE,
//             pdFALSE,
//             portMAX_DELAY);

//     /* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
//      * happened. */
//     if (bits & WIFI_CONNECTED_BIT) {
//         ESP_LOGI(TAG, "connected to ap SSID:%s password:%s",
//                  EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
//     } else if (bits & WIFI_FAIL_BIT) {
//         ESP_LOGI(TAG, "Failed to connect to SSID:%s, password:%s",
//                  EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
//     } else {
//         ESP_LOGE(TAG, "UNEXPECTED EVENT");
//     }

//     ESP_ERROR_CHECK(esp_event_handler_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler));
//     ESP_ERROR_CHECK(esp_event_handler_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler));
//     vEventGroupDelete(s_wifi_event_group);
// }

// void app_main(void)
// {
//     //Initialize NVS
//     esp_err_t ret = nvs_flash_init();
//     if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
//       ESP_ERROR_CHECK(nvs_flash_erase());
//       ret = nvs_flash_init();
//     }
//     ESP_ERROR_CHECK(ret);

//     ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");
//     wifi_init_sta();
// }

// void app_main()
// {
//     xTaskCreate(lcd_test, "lcd_test", configMINIMAL_STACK_SIZE * 3, NULL, 5, NULL);
// }

// #include <stdio.h>
// #include <freertos/FreeRTOS.h>
// #include <freertos/task.h>
// #include <ds1307.h>
// #include <string.h>

// #if defined(CONFIG_IDF_TARGET_ESP8266)
// #define SDA_GPIO 4
// #define SCL_GPIO 5
// #else
// #define SDA_GPIO 22
// #define SCL_GPIO 23
// #endif

// #define CONFIG_FREERTOS_HZ 100

// void ds1307_test(void *pvParameters)
// {
//     i2c_dev_t dev;
//     memset(&dev, 0, sizeof(i2c_dev_t));

//     ESP_ERROR_CHECK(ds1307_init_desc(&dev, 0, SDA_GPIO, SCL_GPIO));

//     // setup datetime: 2018-04-11 00:52:10
//     struct tm time = {
//         .tm_year = 2018,
//         .tm_mon  = 3,  // 0-based
//         .tm_mday = 11,
//         .tm_hour = 0,
//         .tm_min  = 52,
//         .tm_sec  = 10
//     };
//     ESP_ERROR_CHECK(ds1307_set_time(&dev, &time));

//     uint8_t buff[2];
//     buff[0] = 12;
//     buff[1] = 15;

//     ds1307_write_ram(&dev, 0, buff, 2);

//     uint8_t buffer[2];

//     while (1)
//     {
//         ds1307_get_time(&dev, &time);

//         printf("%04d-%02d-%02d %02d:%02d:%02d\n", time.tm_year, time.tm_mon + 1,
//             time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);

//             ds1307_read_ram(&dev, 0, buffer, 2);

//         printf("buffer:%d:%d\n", buffer[0], buffer[1]);

//         vTaskDelay(1000 / portTICK_PERIOD_MS);
//     }
// }

// void app_main()
// {
//     ESP_ERROR_CHECK(i2cdev_init());

//     xTaskCreate(ds1307_test, "ds1307_test", configMINIMAL_STACK_SIZE * 8, NULL, 5, NULL);
// }

#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <hd44780.h>
#include <menu.h>
#include <timer_setup.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

#define GPIO_BTN_BACK 27
#define GPIO_BTN_OK 14
#define GPIO_BTN_CON 12
#define GPIO_INPUT_PIN_SEL ((1ULL << GPIO_BTN_BACK) | (1ULL << GPIO_BTN_OK) | (1ULL << GPIO_BTN_CON))
#define ESP_INTR_FLAG_DEFAULT 0

static xQueueHandle gpio_evt_queue = NULL;

static void IRAM_ATTR gpio_isr_handler(void *arg)
{

    uint32_t gpio_num = (uint32_t)arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

static void gpio_task_example(void *arg)
{

    timer_setup_t zone0_timer_setup = {

        .interval_0_on_hour = 0,
        .interval_0_on_minute = 0,
        .interval_0_weekday_config = 127,
        .interval_0_off_hour = 0,
        .interval_0_off_minute = 0,

        .interval_1_on_hour = 0,
        .interval_1_on_minute = 0,
        .interval_1_weekday_config = 127,
        .interval_1_off_hour = 0,
        .interval_1_off_minute = 0,

        .interval_2_on_hour = 0,
        .interval_2_on_minute = 0,
        .interval_2_weekday_config = 127,
        .interval_2_off_hour = 0,
        .interval_2_off_minute = 0};

    timer_setup_t zone1_timer_setup = {

        .interval_0_on_hour = 0,
        .interval_0_on_minute = 0,
        .interval_0_weekday_config = 127,
        .interval_0_off_hour = 0,
        .interval_0_off_minute = 0,

        .interval_1_on_hour = 0,
        .interval_1_on_minute = 0,
        .interval_1_weekday_config = 127,
        .interval_1_off_hour = 0,
        .interval_1_off_minute = 0,

        .interval_2_on_hour = 0,
        .interval_2_on_minute = 0,
        .interval_2_weekday_config = 127,
        .interval_2_off_hour = 0,
        .interval_2_off_minute = 0};

    hd44780_t lcd = {
        .font = HD44780_FONT_5X8,
        .lines = 2,
        .pins = {
            .rs = GPIO_NUM_19,
            .e = GPIO_NUM_18,
            .d4 = GPIO_NUM_5,
            .d5 = GPIO_NUM_17,
            .d6 = GPIO_NUM_16,
            .d7 = GPIO_NUM_4,
            .bl = HD44780_NOT_USED}};

    ESP_ERROR_CHECK(hd44780_init(&lcd));

    menu_t menu = {
        .state = IDLE,
        .time_selection_state = HOUR,
        .weekday_selection_state = MONDAY,
        .continue_count = 0,
        .selected_zone = 0,
        .selected_interval = 0,
        .selected_weekday_configuration = 127,
        .BTN_BACK_PIN = GPIO_BTN_BACK,
        .BTN_OK_PIN = GPIO_BTN_OK,
        .BTN_CON_PIN = GPIO_BTN_CON,
        .zone0_timer_setup = &zone0_timer_setup,
        .zone1_timer_setup = &zone1_timer_setup};

    menu_flush_display(&menu, &lcd);
    uint32_t io_num;
    for (;;)
    {
        if (xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY))
        {

            printf("GPIO[%d] intr, val: %d\n", io_num, gpio_get_level(io_num));

            menu_handle_btn(&menu, &lcd, io_num);
            menu_flush_display(&menu, &lcd);

            //debounce
            vTaskDelay(300 / portTICK_PERIOD_MS);
            xQueueReset(gpio_evt_queue);
        }
    }
}

void app_main(void)
{

    gpio_config_t io_conf;

    //interrupt of rising edge
    io_conf.intr_type = GPIO_INTR_NEGEDGE;
    //bit mask of the pins, use GPIO4/5 here
    io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
    //set as input mode
    io_conf.mode = GPIO_MODE_INPUT;
    //enable pull-up mode
    io_conf.pull_up_en = 1;
    io_conf.pull_down_en = 0;
    gpio_config(&io_conf);

    //change gpio intrrupt type for one pin
    // gpio_set_intr_type(GPIO_BTN_BACK, GPIO_INTR_HIGH_LEVEL);
    // gpio_set_intr_type(GPIO_BTN_OK, GPIO_INTR_NEGEDGE);
    // gpio_set_intr_type(GPIO_BTN_CON, GPIO_INTR_NEGEDGE);

    //create a queue to handle gpio event from isr
    gpio_evt_queue = xQueueCreate(1, sizeof(uint32_t));
    //start gpio task
    xTaskCreate(gpio_task_example, "gpio_task_example", 2048, NULL, 10, NULL);

    //install gpio isr service
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);

    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(GPIO_BTN_BACK, gpio_isr_handler, (void *)GPIO_BTN_BACK);
    gpio_isr_handler_add(GPIO_BTN_OK, gpio_isr_handler, (void *)GPIO_BTN_OK);
    gpio_isr_handler_add(GPIO_BTN_CON, gpio_isr_handler, (void *)GPIO_BTN_CON);
}
