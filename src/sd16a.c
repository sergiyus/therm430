#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>
#include "sd16a.h"

bool voltage_sd16a_measure;
bool temperature_sd16a_measure;

bool voltage_sd16a_result;
bool temperature_sd16a_result;

bool low_battery;

void start_measurement(const uint8_t sd16a_channel)
{
	// Ref - On
	 SD16CTL |= SD16REFON;

	// A5 - voltage devider (AVcc - AVss) / 11
	// A6 - temperature sensor
	SD16INCTL0 = sd16a_channel;

	// SD16_A Start
	SD16CCTL0 |= SD16SC;

	if (sd16a_channel == VOLTAGE) {
		voltage_sd16a_measure = true;
	} else if (sd16a_channel == TEMPERATURE) {
		temperature_sd16a_measure = true;
	}
}

