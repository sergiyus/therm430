#include <msp430.h>
#include <stdint.h>
#include "logic.h"
#include "7seg_lcd.h"

volatile uint16_t status = 0;

void board_init(void)
{
	/*
	 * Stop watchdog timer and config nmi.
	 * WDTNMI	- NMI select function.
	 * WDTNMIES	- NMI on falling edge (1 -> 0).
	 */
	WDTCTL = WDTPW + WDTHOLD + WDTNMI + WDTNMIES;
	/* Enable nmi interupt */
	IE1 |= NMIIE;

	/* config_clocks(); */
	//BCSCTL1 = CALBC1_1MHZ;
	//DCOCTL = CALDCO_1MHZ;
	if (CALBC1_1MHZ == 0xFF || CALDCO_1MHZ == 0xFF) {
		// fDCO(7,3)DCO frequency (7, 3)
		// RSELx = 7, DCOx = 3, MODx = 0
		// 2.2 V, 3 V F = 0.80 .. 1.50 MHz
		BCSCTL1 = RSEL2 + RSEL1 + RSEL0;
		DCOCTL = DCO1 + DCO0;
	} else {
		BCSCTL1 = CALBC1_1MHZ;
		DCOCTL = CALDCO_1MHZ;
	}
	// LFXT1 = VLO.
	BCSCTL3 |= LFXT1S_2;

	/* config_ports(); */
	// All pins of P1 are output
	P1SEL = 0x00;
	P1DIR = 0xFF;
	// P2.7 & P2.6 are output
	P2SEL = 0x00;
	P2DIR = 0xFF;
	/* clear digig */
	P2OUT = 0x00;

	/* config_timer_a2(); */
	// CCR0 - Interupt enable.
	TACCTL0 = CCIE;

	TACCR0 = MAIN_DELAY - 1;
	// TA2:
	//	- ACLK (VLO/8 ~12kHz/8)
	//	- CLOCK/8 (~12kHz/8 ~ 1500 -> 0.000667sec)
	//	- Up MODE (timer counts up to TACCR0).

	//TACCR1 = 0;
	TACTL = TASSEL_1 + ID_3 + MC_1;
	// TACCR1 IE enable.
	//TACCTL1 = CCIE;
}

void convert_temp2d(int16_t temp, struct dspl2d *dspl)
{
	if (temp > HIGHT_TEMP) {
		dspl->digit1 = H;
		dspl->digit0 = i;
	} else if (temp < LOW_TEMP) {
		dspl->digit1 = L;
		dspl->digit0 = o;
	} else {
		dspl->digit1 = 0;

        if (temp < 0) {
            dspl->digit1 = MINUS;
            temp = -temp;
        }

        uint16_t abs_temp = (uint16_t) temp;
        uint16_t tmp = abs_temp / 10;

        if (tmp > 0) {
            dspl->digit1 |= SEVENSEG_OUTPUT[tmp];
        }
        dspl->digit0 = SEVENSEG_OUTPUT[abs_temp % 10];
    }
}

void convert_volt2d(const uint16_t voltage, struct dspl2d *dspl)
{
	dspl->digit1 = SEVENSEG_OUTPUT[voltage / 10] | DP;
	dspl->digit0 = SEVENSEG_OUTPUT[voltage % 10];
}

void print2d(const struct dspl2d *dspl, const uint16_t time_seg_on)
{
	/* time for output = loop * 2* (0.667ms * time_seg_on)	*/
	/* time = 20 * 2 * 6.67 ~= 0.266 sec			*/
	enum { loop = 20 };

	for (uint16_t t = loop; t > 0; --t) {
		P1OUT = dspl->digit1;
		P2OUT = DIG1;
		delay_667mks(time_seg_on);
		P2OUT = 0;

		P1OUT = dspl->digit0;
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

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER0_A0_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__ ((interrupt(TIMER0_A0_VECTOR)))
#else
#error Compiler not supported!
#endif
void Timer_A0(void)
{
	TACCR1 = 0;
    status |= FLAG_PERIODIC;
	LPM3_EXIT;
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER_A1_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__ ((interrupt(TIMER0_A1_VECTOR)))
#else
#error Compiler not supported!
#endif
void Timer_A1(void)
{
	switch (TAIV) {
	case  2:
		// disable interupt
		TACCTL1 &= (uint16_t) ~CCIE;
		LPM3_EXIT;
		break;
	case  4:
		break;
	case 10:
		break;
	}
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = NMI_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__ ((interrupt(NMI_VECTOR)))
#else
#error Compiler not supported!
#endif
void NMI(void)
{
	if (IFG1 & NMIIFG) {
        status |= FLAG_BUTTON_PRESS;
		IFG1 &= (uint8_t) ~NMIIFG;
		IE1 |= NMIIE;
		LPM3_EXIT;
	}
}

