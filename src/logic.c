#include "logic.h"

bool button_press;
bool periodic_timer = true;
bool check_battery;
bool show_voltage;
bool show_temp;
bool show_periodic_temp;

struct dspl_two_digit convert_temp_in_two_digit(int16_t temp)
{
	struct dspl_two_digit dspl;

	if (temp > HIGHT_TEMP) {
		dspl.digit1 = H;
		dspl.digit0 = i;
	} else if (temp < LOW_TEMP) {
		dspl.digit1 = L;
		dspl.digit0 = o;
	} else {
		dspl.digit1 = 0;
		if (temp < 0) {
			dspl.digit1 = MINUS;
            temp = -temp;
		}
		uint16_t d;

		d = (uint16_t)temp / 10;
		if (d > 0) {
			dspl.digit1 |= SEVENSEG_OUTPUT[d];
		}
		d = (uint16_t)temp % 10;
		dspl.digit0 = SEVENSEG_OUTPUT[d];
	}
	return dspl;
}

struct dspl_two_digit convert_voltage_in_two_digit(const uint16_t voltage)
{
	struct dspl_two_digit dspl;
	uint16_t d;

	d = voltage / 10;
	dspl.digit1 = SEVENSEG_OUTPUT[d] | DP;
	d = voltage % 10;
	dspl.digit0 = SEVENSEG_OUTPUT[d];
	return dspl;
}

void print_two_digit(const struct dspl_two_digit dspl,
		     const uint16_t time_seg_on)
{
	/* time for output = loop * 2* (0.667ms * time_seg_on)	*/
	/* time = 20 * 2 * 6.67 ~= 0.266 sec			*/
	enum { loop = 20 };

	for (uint16_t t = loop; t > 0; --t) {
		P1OUT = dspl.digit1;
		P2OUT = DIG1;
		delay_667mks(time_seg_on);
		P2OUT = 0;

		P1OUT = dspl.digit0;
		P2OUT = DIG0;
		delay_667mks(time_seg_on);
		P2OUT = 0;
	}
	P1OUT = 0;
}

void delay_667mks(const uint16_t n_x667mks)
{
	// time delay ~ 0.667ms * n_x667mks.
	TACCR1 = TAR + n_x667mks;

	if (TACCR1 > TACCR0) {
		TACCR1 -= TACCR0;
	}
	// enable interupt
	TACCTL1 = CCIE;
	LPM3;
}

