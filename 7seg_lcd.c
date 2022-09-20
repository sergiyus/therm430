#include "7seg_lcd.h"
#include "timer.h"

bool dot_off = false;
bool dot_on  = true;

// 7-seg LED display from "0" to "9", H, i, L, o, b, E, r.
// Hi - hight
// Lo - low
// Lb - low battery
// Er - Error.
const uint8_t SEVENSEG_OUTPUT[] =
{
	seg_A + seg_B + seg_C + seg_D + seg_E + seg_F,          // 0
        seg_B + seg_C,						// 1
        seg_A + seg_B + seg_G + seg_E + seg_D,                  // 2
        seg_A + seg_B + seg_G + seg_C + seg_D,                  // 3
        seg_F + seg_G + seg_B + seg_C,				// 4
        seg_A + seg_F + seg_G + seg_C + seg_D,                  // 5
        seg_A + seg_F + seg_G + seg_C + seg_D + seg_E,		// 6
        seg_A + seg_B + seg_C,					// 7
        seg_A + seg_B + seg_C + seg_D + seg_E + seg_F + seg_G,  // 8
        seg_A + seg_B + seg_C + seg_D + seg_G + seg_F,          // 9
	//seg_A + seg_B + seg_C + seg_E + seg_F + seg_G,	// A
	//seg_C + seg_D + seg_E + seg_G + seg_F,		// b
	//seg_A + seg_D + seg_E + seg_F,			// C
	//seg_B + seg_C + seg_D + seg_E + seg_G,		// d
	//seg_A + seg_D + seg_E + seg_F + seg_G,		// E
	//seg_A + seg_E + seg_F + seg_G,			// F
	DP,							// DP
	0,							// nothing
	seg_B + seg_C + seg_E + seg_F + seg_G,			// H
	seg_C,							// i
	seg_D + seg_E + seg_F,					// L
	seg_C + seg_D + seg_E + seg_G,				// o
	seg_C + seg_D + seg_E + seg_F + seg_G,			// b
	seg_A + seg_D + seg_E + seg_F + seg_G,			// E
	seg_E + seg_G						// r
};


void print_2digit(uint8_t digit1, uint8_t digit0,
		  uint16_t delay_on, uint16_t delay_off,
		  bool dot)
{
	if (dot) {
		P1OUT = SEVENSEG_OUTPUT[digit1] + DP;
	} else {
		P1OUT = SEVENSEG_OUTPUT[digit1];
	}
	P2OUT = DIG1;
	delay_667mks(delay_on);

	clear_digit();
	delay_667mks(delay_off);

	P1OUT = SEVENSEG_OUTPUT[digit0];
	P2OUT = DIG0;
	delay_667mks(delay_on);

	clear_digit();
	delay_667mks(delay_off);
}

void clear_digit(void)
{
	P1OUT = 0;
	P2OUT = 0;
}
