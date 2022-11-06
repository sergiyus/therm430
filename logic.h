#ifndef _LOGIC_H_
#define _LOGIC_H_

#include <stdint.h>
#include "7seg_lcd.h"
#include "timer.h"

struct dspl_two_digit {
	uint8_t digit1;
	uint8_t digit0;
};

enum {SEG_TIME_ON = 10};

void delay_667mks(const uint16_t n_x667mks);

struct dspl_two_digit convert_temp_in_two_digit(const int16_t temp);
struct dspl_two_digit convert_voltage_in_two_digit(const uint16_t voltage);

/*
void convert_temp_in_two_digit(struct dspl_two_digit *dspl, const int16_t temp);
void convert_voltage_in_two_digit(struct dspl_two_digit *dspl, const uint16_t voltage);
*/

void print_two_digit(const struct dspl_two_digit dspl,
		     const uint16_t time_seg_on);

#endif
