/*
 * therm430
 * The two digit 7 segment thermometer.
 * Original idea from http://www.technoblogy.com/show?2G8T
 *
 * Sun 06 Nov 2022 20:44:28 EET
 *
 * text    data     bss     dec     hex filename
 * 1024       0      18    1042     412 therm430.elf
 */

#include <msp430f2003.h>
#include <stdint.h>
#include <stdbool.h>
#include "clocks.h"
#include "ports.h"
#include "timer.h"
#include "sd16a.h"
#include "7seg_lcd.h"
#include "logic.h"

bool nmi_flag = false;

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

	config_clocks();
	config_ports();
	config_timer_a2();
	config_sd16a();

	_BIS_SR(GIE);

	uint16_t loopcounter = DELAY_VOLTAGE_MEASURE;
	struct dspl_two_digit dspl;

	while (1) {
		++loopcounter;
		if (loopcounter > DELAY_VOLTAGE_MEASURE) {
			start_measurement(VOLTAGE);
			loopcounter = 0;
			LPM3;
		}

		if (voltage_sd16a_result) {
			voltage_sd16a_result = false;
			if (voltage_raw < LOW_BATTERY) {
				low_battery = true;
			} else {
				low_battery = false;
			}
		}

		if (low_battery) {
			dspl.digit1 = L;
			dspl.digit0 = b;
			print_two_digit(dspl, SEG_TIME_ON);
			delay_667mks(SEG_TIME_ON);
		}

		if (nmi_flag) {
			nmi_flag = false;
			start_measurement(VOLTAGE);
			LPM3;

			if (voltage_sd16a_result) {
				voltage_sd16a_result = false;
				uint16_t volt;
				volt = simply_convert_voltage(voltage_raw);

				dspl = convert_voltage_in_two_digit(volt);
				/*
				uint8_t d1 = volt / 10;
				uint8_t d0 = volt % 10;
				dspl.digit1 = SEVENSEG_OUTPUT[d1] + DP;
				dspl.digit0 = SEVENSEG_OUTPUT[d0];
				*/
				print_two_digit(dspl, 10);
			}

		}

		start_measurement(TEMPERATURE);
		LPM3;

		if (temperatrure_sd16a_result) {
			temperatrure_sd16a_result = false;

			int16_t temp;
			temp = convert_sd16a_raw_to_temperature(temp_raw);
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
	// exit from sleep (LPM3)
	LPM3_EXIT;
}

#pragma vector = TIMERA1_VECTOR
__interrupt void Timer_A1(void)
{
	switch(TAIV) {
	case  2:
	         {
			// disable interupt
			TACCTL1 &= ~CCIE;
	         	LPM3_EXIT;
	         }
	         break;
	case  4: break;
	case 10: break;
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
		temperatrure_sd16a_result = true;
	} else if (voltage_sd16a_measure == true) {
		voltage_raw = SD16MEM0;
		voltage_sd16a_measure = false;
		voltage_sd16a_result = true;
	}

	LPM3_EXIT;
}


#pragma vector = NMI_VECTOR
__interrupt void NMI(void)
{
	if (NMIIFG) {
		nmi_flag = true;
		IFG1 &= ~NMIIFG;
		IE1 |= NMIIE;
		LPM3_EXIT;
	}
}

