#pragma once

#include <msp430f2003.h>
#include <stdint.h>
#include <stdbool.h>

#define DIG0	BIT7
#define	DIG1	BIT6

#define seg_A	BIT7
#define seg_B	BIT6
#define seg_C	BIT5
#define seg_D	BIT4
#define seg_E	BIT3
#define seg_F	BIT2
#define seg_G	BIT1
#define DP	BIT0

extern const uint8_t SEVENSEG_OUTPUT[];

extern bool dot_off;
extern bool dot_on;

void print_2digit(uint8_t digit1, uint8_t digit0,
		  uint16_t delay_on, uint16_t delay_off,
		  bool dot);

void clear_digit(void);
