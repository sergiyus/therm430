#include "logic.h"

struct dspl_two_digit convert_temp_in_two_digit(const int16_t temperature)
{
	struct dspl_two_digit dspl;
	int16_t temp = temperature;

	if (temp > HIGHT_TEMP) {
		dspl.digit1 = H;
		dspl.digit0 = i;
	} else if (temp < LOW_TEMP) {
		dspl.digit1 = L;
		dspl.digit0 = o;
	} else {
		uint8_t sign = 0;
		if (temp < 0) {
			sign = MINUS;
			/* convert to positive */
			temp = ~temp + 1;
		}

		uint8_t d1 = temp / 10;
		uint8_t d0 = temp % 10;
		if (d1 == 0) {
			/* mask the significant zero */
			dspl.digit1 = sign;
		} else {
			dspl.digit1 = sign + SEVENSEG_OUTPUT[d1];
		}
		dspl.digit0 = SEVENSEG_OUTPUT[d0];
	}
	return dspl;
}

struct dspl_two_digit convert_voltage_in_two_digit(const uint16_t voltage)
{
	struct dspl_two_digit dspl;

	uint8_t d1 = voltage / 10;
	uint8_t d0 = voltage % 10;
	dspl.digit1 = SEVENSEG_OUTPUT[d1] + DP;
	dspl.digit0 = SEVENSEG_OUTPUT[d0];
	return dspl;
}

/*
void convert_voltage_in_two_digit(struct dspl_two_digit *dspl, uint16_t voltage)
{
	uint8_t d;

	d = voltage / 10;
	dspl->digit1 = SEVENSEG_OUTPUT[d] + DP;

	d = voltage % 10;
	dspl->digit0 = SEVENSEG_OUTPUT[d];
}
*/

void print_two_digit(const struct dspl_two_digit dspl,
		     const uint16_t time_seg_on)
{
	/* time for output = loop * 2* (0.667ms * time_seg_on)	*/
	/* time = 10 * 2 * 6.67 ~= 0.133 sec			*/
	enum {loop = 20};

	for (uint16_t i = 0; i < loop; ++i) {
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

//void convert_temp_in_two_digit(struct dspl_two_digit *dspl, int16_t temp)
//{
//	if (temp > HIGHT_TEMP) {
//		dspl->digit1 = H;
//		dspl->digit0 = i;
//	} else if (temp < LOW_TEMP) {
//		dspl->digit1 = L;
//		dspl->digit0 = o;
//	} else {
//		uint8_t sign = 0;
//		if (temp < 0) {
//			sign = MINUS;
//			/* convert temperature to positive */
//			temp = ~temp + 1;
//		}
//
//		uint8_t d1 = temp / 10;
//		uint8_t d0 = temp % 10;
//		if (d1 == 0) {
//			/* mask the significant zero */
//			dspl->digit1 = sign;
//		} else {
//			dspl->digit1 = sign + SEVENSEG_OUTPUT[d1];
//		}
//		dspl->digit0 = SEVENSEG_OUTPUT[d0];
//	}
//
//}

