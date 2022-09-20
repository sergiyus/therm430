#include <msp430f2003.h>
#include <stdint.h>
#include <stdbool.h>
#include "clocks.h"
#include "ports.h"
#include "timer.h"
#include "sd16a.h"
#include "7seg_lcd.h"

volatile uint8_t t1 = 0 , t2 = 0;
//, loopcounter = 0;
uint8_t loopcounter = 0;

int main(void)
{
	WDTCTL = WDTPW + WDTHOLD;		// Stop watchdog timer.

	config_clocks();
	config_ports();
	config_timer_a2();
	TACCR1 = 0;
	config_sd16a();

	_BIS_SR(GIE);

	while(1) {

		++loopcounter;
		if (loopcounter > DELAY_VOLTAGE_MEASURE) {
			start_measurement(VOLTAGE);
			loopcounter = 0;
			LPM3;
		}

		if (voltage_sd16a_result) {
			voltage_sd16a_result = false;
			t2 = convert_sd16a_raw_to_voltage(voltage_raw) / 100 % 10;
			t1 = convert_sd16a_raw_to_voltage(voltage_raw) / 10 % 10;
			//uint8_t t0 = convert_sd16a_raw_to_voltage(voltage_raw) % 10;
			//if (t0 > 5) {
			//	++t1;
			//}

			uint8_t i;
			for (i = 10; i > 0; --i) {

				print_2digit(t2, t1, 5, 5, dot_on);
			}
			delay_667mks(1000);
		}

		start_measurement(TEMPERATURE);
		LPM3;

		if (temperatrure_sd16a_result) {
			temperatrure_sd16a_result = false;

			t2 = convert_sd16a_raw_to_temperature(temp_raw) / 10;
			t1 = convert_sd16a_raw_to_temperature(temp_raw) % 10;

			uint8_t i;
			for (i = 10; i > 0; --i) {
				print_2digit(t2, t1, 5, 5, dot_off);
			}
		} else {

			P1OUT = SEVENSEG_OUTPUT[17];
			P2OUT = DIG1;
			delay_667mks(750);
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
	// Ref - OFF, for power safe
	SD16CTL &= ~SD16REFON;

	if (voltage_sd16a_measure == true) {
		voltage_raw = SD16MEM0;
		voltage_sd16a_measure = false;
		voltage_sd16a_result = true;
	} else if (temperature_sd16a_measure) {
		temp_raw = SD16MEM0;
		temperature_sd16a_measure = false;
		temperatrure_sd16a_result = true;
	}

	LPM3_EXIT;
}
