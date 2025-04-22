/*
 * therm430
 * The two digit 7 segment thermometer.
 * Original idea from http://www.technoblogy.com/show?2G8T
 *
 * Thu 15 Dec 2022 23:33:52 EET
 *
 * text    data     bss     dec     hex filename
 * 1020       2      26    1048     418 therm430.elf
 */

#include <msp430f2003.h>
#include <stdint.h>
#include <stdbool.h>
#include "sd16a.h"
#include "7seg_lcd.h"
#include "logic.h"

#define CONVERT_VOLTAGE(VOLTAGE_RAW)	((VOLTAGE_RAW) / 1000)
#define CONVERT_TEMP(TEMP_RAW)		((int16_t)((TEMP_RAW) - 39768) / 144)

volatile uint16_t voltage_raw;
volatile uint16_t temp_raw;

int main(void)
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
	// NO FREE space!
	//P2SEL &= ~(BIT7 + BIT6);
	P2SEL = 0x00;
	//P2DIR |= BIT6 + BIT7;
	P2DIR = 0xFF;

	/* clear digig */
	//P2OUT &= ~(DIG1 + DIG0);
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

	/* config_sd16a(); */
	// This is specific magic number for SD16_A
	// msp430f20x3 rev. B only!!! From errata SLA153I.pdf
	*(uint8_t *const)(0xBF) = 0x61;

	// Clock source - used VLO (~12kHz)
	// SD16DIV_0 - Clock divider /1
	// SD16REFON - Ref used only during measurement (for power safe)
	// SD16CTL = SD16LP + SD16DIV_0 + SD16SSEL_2 + SD16REFON;
	SD16CTL = SD16LP + SD16DIV_0 + SD16SSEL_2;

	// Unipolar, single mode and IE.
	SD16CCTL0 = SD16UNI + SD16SNGL + SD16IE;

	uint16_t loopcounter = DELAY_VOLTAGE_MEASURE - 1;
	struct dspl_two_digit dspl = {0, 0};

	_BIS_SR(GIE);

	while (1) {

		if (periodic_timer) {
			periodic_timer = false;
			++loopcounter;
			start_measurement(TEMPERATURE);
			show_periodic_temp = true;

		} else if (loopcounter > DELAY_VOLTAGE_MEASURE) {
			loopcounter = 0;
			start_measurement(VOLTAGE);
			check_battery = true;

		} else if (button_press) {
			button_press = false;
			start_measurement(VOLTAGE);
			show_voltage = true;

		} else if (check_battery && voltage_sd16a_result) {
			check_battery = false;
			voltage_sd16a_result = false;
			low_battery = (voltage_raw < LOW_BATTERY) ? true : false;

		} else if (show_voltage && voltage_sd16a_result) {
			show_voltage = false;
			voltage_sd16a_result = false;
			uint16_t volt = CONVERT_VOLTAGE(voltage_raw);
			//uint16_t volt = convert_voltage(voltage_raw);
			dspl = convert_voltage_in_two_digit(volt);
			print_two_digit(dspl, SEG_TIME_ON);

		} else if (show_periodic_temp && temperature_sd16a_result) {
			show_periodic_temp = false;
			temperature_sd16a_result = false;
			if (low_battery) {
				dspl.digit1 = L;
				dspl.digit0 = b;
				print_two_digit(dspl, SEG_TIME_ON);
				delay_667mks(SEG_TIME_ON);
			}
			int16_t temp = CONVERT_TEMP(temp_raw);

			dspl = convert_temp_in_two_digit(temp);
			print_two_digit(dspl, SEG_TIME_ON);
		}

		LPM3;
	}
}

#pragma vector = TIMERA0_VECTOR
__interrupt void Timer_A0(void)
{
	TACCR1 = 0;
	periodic_timer = true;
	LPM3_EXIT;
}

#pragma vector = TIMERA1_VECTOR
__interrupt void Timer_A1(void)
{
	switch (TAIV) {
	case  2:
		// disable interupt
		TACCTL1 &= ~CCIE;
		LPM3_EXIT;
		break;
	case  4:
		break;
	case 10:
		break;
	}
}

#pragma vector = SD16_VECTOR
__interrupt void SD16ISR(void)
{
	/* Ref - OFF, for power safe */
	SD16CTL &= ~SD16REFON;
	if (temperature_sd16a_measure) {
		temp_raw = SD16MEM0;
		temperature_sd16a_measure = false;
		temperature_sd16a_result = true;
	} else if (voltage_sd16a_measure) {
		voltage_raw = SD16MEM0;
		voltage_sd16a_measure = false;
		voltage_sd16a_result = true;
	}
	LPM3_EXIT;
}

#pragma vector = NMI_VECTOR
__interrupt void NMI(void)
{
	if (IFG1 & NMIIFG) {
		button_press = true;
		IFG1 &= ~NMIIFG;
		IE1 |= NMIIE;
		LPM3_EXIT;
	}
}

