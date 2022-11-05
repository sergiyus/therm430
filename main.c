/*
 * therm430
 * The two digit 7 segment thermometer.
 * Original idea from http://www.technoblogy.com/show?2G8T
 *
 * Thu 03 Nov 2022 23:26:16 EET
 *
 * Programming...
 * Writing  826 bytes at fc00 [section: .text]...
 * Writing   10 bytes at ff3a [section: .rodata]...
 * Writing   32 bytes at ffe0 [section: .vectors]...
 * Done, 868 bytes total*
 */

#include <msp430f2003.h>
#include <stdint.h>
#include <stdbool.h>
#include "logic.h"
#include "clocks.h"
#include "ports.h"
#include "timer.h"
#include "sd16a.h"
#include "7seg_lcd.h"

int main(void)
{
	WDTCTL = WDTPW + WDTHOLD;		// Stop watchdog timer.

	config_clocks();
	config_ports();
	config_timer_a2();
	config_sd16a();

	_BIS_SR(GIE);

	uint16_t loopcounter = DELAY_VOLTAGE_MEASURE;
	struct dspl_two_digit dspl;

	while(1) {
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
