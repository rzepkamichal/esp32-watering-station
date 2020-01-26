#ifndef __HTTP_RESPONSE_UTILS_H__
#define __HTTP_RESPONSE_UTILS_H__

#include <stdint.h>
#include "menu.h"
#include "cJSON.h"
#include "esp_log.h"
#include <time.h>

#define HTTP_RESPONSE_UTIL_TAG "HTTP_RESPNSE_UTIL"
#define DATE_FIELD_NAME "datetime"
#define WEEKDAY_FIELD_NAME "day_of_week"
#define RAIN_FIELD_NAME "rain"
#define RAIN_INTERVAL_FIELD_NAME "3h"

void http_response_handle(menu_t *menu, const char *response_data)
{
    cJSON *json = cJSON_Parse(response_data);

    if (!json)
    {
        ESP_LOGI(HTTP_RESPONSE_UTIL_TAG, "Could not parse http response to json");
        return;
    }

    cJSON *datetime = cJSON_GetObjectItem(json, DATE_FIELD_NAME);
    cJSON *day_of_week = cJSON_GetObjectItem(json, WEEKDAY_FIELD_NAME);

    if (datetime && day_of_week)
    {
        struct tm tm;

        int y, M, d, h, m;
        float s;
        sscanf(datetime->valuestring, "%d-%d-%dT%d:%d:%fZ", &y, &M, &d, &h, &m, &s);

        tm.tm_year = y;
        tm.tm_mon = M;     
        tm.tm_mday = d;        
        tm.tm_hour = h;        
        tm.tm_min = m;         
        tm.tm_sec = (int)s;
        tm.tm_wday = day_of_week->valueint;

        menu_decode_time(menu, &tm);
        return;
    }

    cJSON *rain = cJSON_GetObjectItem(json, RAIN_FIELD_NAME);

    if(rain){
        cJSON *rain_last_3h = cJSON_GetObjectItem(rain, RAIN_INTERVAL_FIELD_NAME);

        if(rain_last_3h){
            menu->rain_3h_mm = rain_last_3h->valuedouble;
            printf("RAIN: %f", menu->rain_3h_mm);
        }
    }else{
        menu->rain_3h_mm = 0.0;
    }

    cJSON_Delete(json);
}

#endif