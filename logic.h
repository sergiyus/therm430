#ifndef _LOGIC_H_
#define _LOGIC_H_

#include <stdint.h>
#include <stdbool.h>
#include "7seg_lcd.h"

struct dspl_two_digit {
	uint8_t digit1;
	uint8_t digit0;
};

enum { SEG_TIME_ON = 10 };
enum { MAIN_DELAY = 14992 };

bool button_press;
bool periodic_timer;
bool check_battery;
bool show_voltage;
bool show_temp;
bool show_periodic_temp;

void delay_667mks(const uint16_t n_x667mks);

struct dspl_two_digit convert_temp_in_two_digit(const int16_t temp);
struct dspl_two_digit convert_voltage_in_two_digit(const uint16_t voltage);

void print_two_digit(const struct dspl_two_digit d, const uint16_t time);

#endif
