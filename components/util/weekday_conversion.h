#ifndef __WEEKDAY_CONVERSION_H__
#define __WEEKDAY_CONVERSION_H__

#include <stdlib.h>
#include <stdint.h>
#include <menu.h>
#include <timer_setup.h>

//convert a true/false 7-element array into corresponding value
//eg. [1, 1, 1, 1, 1, 0, 1] -> 011111101
uint8_t encode(uint8_t weekday_setup[7]);

//convert a true/false 7-digit value into a corresponding array
//eg.  011111101 -> [1, 1, 1, 1, 1, 0, 1]
void decode(uint8_t encoded_value, uint8_t weekday_setup[7]);

#endif