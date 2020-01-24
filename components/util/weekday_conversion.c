#include "weekday_conversion.h"

uint8_t encode(uint8_t weekday_setup[7])
{

    uint8_t encoded_value = 0;
    uint8_t power = 1;

    for (int i = 0; i < WEEKDAY_COUNT; i++)
    {
        if (1 == weekday_setup[i])
        {
            encoded_value += power;
        }

        power *= 2;
    }

    return encoded_value;
}

void decode(uint8_t encoded_value, uint8_t weekday_setup[7])
{
    for (uint8_t i = 0; i < WEEKDAY_COUNT; i++)
    {
        if (encoded_value % 2 == 1)
        {
            weekday_setup[i] = 1;
        }
        else
        {
            weekday_setup[i] = 0;
        }

        encoded_value = encoded_value >> 1;
    }
}