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
#include "esp_types.h"
#include "driver/periph_ctrl.h"
#include "driver/timer.h"
#include <stdio.h>
#include <ds1307.h>
#include "rtc_persistence.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "esp_netif.h"
#include "esp_tls.h"
#include "esp_http_client.h"
#include <cJSON.h>
#include "http_response_utils.h"

#define SDA_GPIO 22
#define SCL_GPIO 23

#define TIMER_DIVIDER 16                             //  Hardware timer clock divider
#define TIMER_SCALE (TIMER_BASE_CLK / TIMER_DIVIDER) // convert counter value to seconds
#define TIMER_INTERVAL0_SEC (1)                      // sample test interval for the first timer
#define TEST_WITHOUT_RELOAD 0                        // testing will be done without auto reload
#define TEST_WITH_RELOAD 1                           // testing will be done with auto reload

#define GPIO_BTN_BACK 27
#define GPIO_BTN_OK 14
#define GPIO_BTN_CON 12
#define GPIO_INPUT_PIN_SEL ((1ULL << GPIO_BTN_BACK) | (1ULL << GPIO_BTN_OK) | (1ULL << GPIO_BTN_CON))
#define ESP_INTR_FLAG_DEFAULT 0

#define EXAMPLE_ESP_WIFI_SSID "M_RZ"
#define EXAMPLE_ESP_WIFI_PASS "michalek"
#define EXAMPLE_ESP_MAXIMUM_RETRY 10

/* FreeRTOS event group to signal when we are connected*/
static EventGroupHandle_t s_wifi_event_group;

/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

#define MAX_HTTP_RECV_BUFFER 4096
static const char *TAG_HTTP = "HTTP_CLIENT";

static uint8_t web_time_init_attempt_performed = 0;

/*
 * A sample structure to pass events
 * from the timer interrupt handler to the main program.
 */
typedef struct
{
    int type; // the type of timer's event
    int timer_group;
    int timer_idx;
    uint64_t timer_counter_value;
} timer_event_t;

static current_time_t current_time = {
    .day = 0,
    .month = 0,
    .year = 0,
    .hour = 0,
    .minute = 0,
    .weekday = MONDAY};

static timer_setup_t zone0_timer_setup = {

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

static timer_setup_t zone1_timer_setup = {

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

static hd44780_t lcd = {
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

static menu_t menu = {
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
    .zone1_timer_setup = &zone1_timer_setup,
    .current_time = &current_time};

static xQueueHandle gpio_evt_queue = NULL;
xQueueHandle timer_queue;

static const char *TAG = "wifi station";

static int s_retry_num = 0;

static char http_rcv_buffer[MAX_HTTP_RECV_BUFFER];

static void event_handler(void *arg, esp_event_base_t event_base,
                          int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        esp_wifi_connect();
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        if (s_retry_num < EXAMPLE_ESP_MAXIMUM_RETRY)
        {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG, "retry to connect to the AP");
        }
        else
        {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }
        ESP_LOGI(TAG, "connect to the AP fail");
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

void wifi_init_sta(void)
{
    s_wifi_event_group = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_netif_init());

    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = EXAMPLE_ESP_WIFI_SSID,
            .password = EXAMPLE_ESP_WIFI_PASS},
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "wifi_init_sta finished.");

    /* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for the maximum
     * number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler() (see above) */
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
                                           WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
                                           pdFALSE,
                                           pdFALSE,
                                           portMAX_DELAY);

    /* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
     * happened. */
    if (bits & WIFI_CONNECTED_BIT)
    {
        ESP_LOGI(TAG, "connected to ap SSID:%s password:%s",
                 EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
    }
    else if (bits & WIFI_FAIL_BIT)
    {
        web_time_init_attempt_performed = 1;
        ESP_LOGI(TAG, "Failed to connect to SSID:%s, password:%s",
                 EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
    }
    else
    {
        ESP_LOGE(TAG, "UNEXPECTED EVENT");
    }

    ESP_ERROR_CHECK(esp_event_handler_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler));
    ESP_ERROR_CHECK(esp_event_handler_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler));
    vEventGroupDelete(s_wifi_event_group);
}

/*
 * Timer group0 ISR handler
 *
 * Note:
 * We don't call the timer API here because they are not declared with IRAM_ATTR.
 * If we're okay with the timer irq not being serviced while SPI flash cache is disabled,
 * we can allocate this interrupt without the ESP_INTR_FLAG_IRAM flag and use the normal API.
 */
void IRAM_ATTR timer_group0_isr(void *para)
{
    timer_spinlock_take(TIMER_GROUP_0);
    int timer_idx = (int)para;

    /* Retrieve the interrupt status and the counter value
       from the timer that reported the interrupt */
    uint32_t timer_intr = timer_group_get_intr_status_in_isr(TIMER_GROUP_0);
    uint64_t timer_counter_value = timer_group_get_counter_value_in_isr(TIMER_GROUP_0, timer_idx);

    /* Prepare basic event data
       that will be then sent back to the main program task */
    timer_event_t evt;
    evt.timer_group = 0;
    evt.timer_idx = timer_idx;
    evt.timer_counter_value = timer_counter_value;

    /* Clear the interrupt
       and update the alarm time for the timer with without reload */
    if (timer_intr & TIMER_INTR_T0)
    {
        evt.type = TEST_WITHOUT_RELOAD;
        timer_group_clr_intr_status_in_isr(TIMER_GROUP_0, TIMER_0);
        timer_counter_value += (uint64_t)(TIMER_INTERVAL0_SEC * TIMER_SCALE);
        timer_group_set_alarm_value_in_isr(TIMER_GROUP_0, timer_idx, timer_counter_value);
    }
    else
    {
        evt.type = -1; // not supported even type
    }

    /* After the alarm has been triggered
      we need enable it again, so it is triggered the next time */
    timer_group_enable_alarm_in_isr(TIMER_GROUP_0, timer_idx);

    /* Now just send the event data back to the main program task */
    xQueueSendFromISR(timer_queue, &evt, NULL);
    timer_spinlock_give(TIMER_GROUP_0);
}

/*
 * Initialize selected timer of the timer group 0
 *
 * timer_idx - the timer number to initialize
 * auto_reload - should the timer auto reload on alarm?
 * timer_interval_sec - the interval of alarm to set
 */
static void example_tg0_timer_init(int timer_idx,
                                   bool auto_reload, double timer_interval_sec)
{
    /* Select and initialize basic parameters of the timer */
    timer_config_t config;
    config.divider = TIMER_DIVIDER;
    config.counter_dir = TIMER_COUNT_UP;
    config.counter_en = TIMER_PAUSE;
    config.alarm_en = TIMER_ALARM_EN;
    config.intr_type = TIMER_INTR_LEVEL;
    config.auto_reload = auto_reload;
#ifdef TIMER_GROUP_SUPPORTS_XTAL_CLOCK
    config.clk_src = TIMER_SRC_CLK_APB;
#endif
    timer_init(TIMER_GROUP_0, timer_idx, &config);

    /* Timer's counter will initially start from value below.
       Also, if auto_reload is set, this value will be automatically reload on alarm */
    timer_set_counter_value(TIMER_GROUP_0, timer_idx, 0x00000000ULL);

    /* Configure the alarm value and the interrupt on alarm. */
    timer_set_alarm_value(TIMER_GROUP_0, timer_idx, timer_interval_sec * TIMER_SCALE);
    timer_enable_intr(TIMER_GROUP_0, timer_idx);
    timer_isr_register(TIMER_GROUP_0, timer_idx, timer_group0_isr,
                       (void *)timer_idx, ESP_INTR_FLAG_IRAM, NULL);

    timer_start(TIMER_GROUP_0, timer_idx);
}

static void IRAM_ATTR gpio_isr_handler(void *arg)
{

    uint32_t gpio_num = (uint32_t)arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

static void gpio_task_example(void *arg)
{

    ESP_ERROR_CHECK(hd44780_init(&lcd));

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

esp_err_t _http_event_handler(esp_http_client_event_t *evt)
{
    switch (evt->event_id)
    {
    case HTTP_EVENT_ERROR:
        web_time_init_attempt_performed = 1;
        ESP_LOGD(TAG_HTTP, "HTTP_EVENT_ERROR");
        break;
    case HTTP_EVENT_ON_CONNECTED:
        ESP_LOGD(TAG_HTTP, "HTTP_EVENT_ON_CONNECTED");
        break;
    case HTTP_EVENT_HEADER_SENT:
        ESP_LOGD(TAG_HTTP, "HTTP_EVENT_HEADER_SENT");
        break;
    case HTTP_EVENT_ON_HEADER:
        ESP_LOGD(TAG_HTTP, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
        break;
    case HTTP_EVENT_ON_DATA:
        ESP_LOGD(TAG_HTTP, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
        if (!esp_http_client_is_chunked_response(evt->client))
        {

            int data_len = snprintf(http_rcv_buffer + strlen(http_rcv_buffer),
                                    evt->data_len + 1, "%s", (char *)evt->data);

            printf("%s\n\n", http_rcv_buffer);
            printf("%.*s\n\n", evt->data_len, (char *)evt->data);
        }
        break;
    case HTTP_EVENT_ON_FINISH:
        ESP_LOGD(TAG_HTTP, "HTTP_EVENT_ON_FINISH");
        printf("FINISH: %s\n\n", http_rcv_buffer);
        http_response_handle(&menu, http_rcv_buffer);
        http_rcv_buffer[0] = '\0';
        web_time_init_attempt_performed = 1;
        break;
    case HTTP_EVENT_DISCONNECTED:
        ESP_LOGI(TAG_HTTP, "HTTP_EVENT_DISCONNECTED");
        int mbedtls_err = 0;
        esp_err_t err = esp_tls_get_and_clear_last_error(evt->data, &mbedtls_err, NULL);
        if (err != 0)
        {
            ESP_LOGI(TAG_HTTP, "Last esp error code: 0x%x", err);
            ESP_LOGI(TAG_HTTP, "Last mbedtls failure: 0x%x", mbedtls_err);
        }
        break;
    }
    return ESP_OK;
}

static void http_rest_with_url(void)
{
    esp_http_client_config_t config = {
        .url = "http://worldtimeapi.org/api/timezone/Europe/Warsaw",
        .event_handler = _http_event_handler,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);

    // GET
    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK)
    {
        ESP_LOGI(TAG_HTTP, "HTTP GET Status = %d, content_length = %d",
                 esp_http_client_get_status_code(client),
                 esp_http_client_get_content_length(client));
    }
    else
    {
        ESP_LOGE(TAG_HTTP, "HTTP GET request failed: %s", esp_err_to_name(err));
    }

    esp_http_client_cleanup(client);
}

static void http_test_task(void *pvParameters)
{
    http_rest_with_url();
    ESP_LOGI(TAG_HTTP, "Finish http example");
    vTaskDelete(NULL);
}

/*
 * The main task of this example program
 */
static void timer_example_evt_task(void *arg)
{
    while(!web_time_init_attempt_performed){}

    i2c_dev_t dev;
    memset(&dev, 0, sizeof(i2c_dev_t));

    ESP_ERROR_CHECK(ds1307_init_desc(&dev, 0, SDA_GPIO, SCL_GPIO));

    struct tm time = {0};

    // printf("godzina: %d \n\n", menu.current_time->hour);
    // printf("dzien: %d \n\n", menu.current_time->day);
    if (menu.current_time->year != 0)
    {
        //if time already loaded from internet, save to RTC
        menu_encode_time(&menu, &time);
        ds1307_set_time(&dev, &time);
    }
    else
    {
        //if time not loaded from internet, read from RTC
        ds1307_get_time(&dev, &time);
        menu_decode_time(&menu, &time);
    }

    ESP_ERROR_CHECK(ds1307_set_time(&dev, &time));

    while (1)
    {
        timer_event_t evt;
        xQueueReceive(timer_queue, &evt, portMAX_DELAY);
        ds1307_get_time(&dev, &time);
        save_configuration(&dev, &menu);
        menu_decode_time(&menu, &time);
        menu_flush_display(&menu, &lcd);
    }
}

void app_main(void)
{
    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");
    wifi_init_sta();

    // ESP_ERROR_CHECK(esp_netif_init());
    // ESP_ERROR_CHECK(esp_event_loop_create_default());

    /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
     * Read "Establishing Wi-Fi or Ethernet Connection" section in
     * examples/protocols/README.md for more information about this function.
     */

    ESP_LOGI(TAG_HTTP, "START HTTP CLIENT");

    xTaskCreate(&http_test_task, "http_test_task", 8192, NULL, 5, NULL);

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

    ESP_ERROR_CHECK(i2cdev_init());

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

    timer_queue = xQueueCreate(10, sizeof(timer_event_t));
    example_tg0_timer_init(TIMER_0, TEST_WITHOUT_RELOAD, TIMER_INTERVAL0_SEC);
    xTaskCreate(timer_example_evt_task, "timer_evt_task", 2048, NULL, 5, NULL);
}
