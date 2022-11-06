#include <msp430f2003.h>
#include <stdint.h>
#include <stdbool.h>
#include "sd16a.h"

bool voltage_sd16a_measure = false;
bool temperature_sd16a_measure = false;
bool voltage_sd16a_result = false;
bool temperatrure_sd16a_result = false;
bool low_battery = false;

void config_sd16a(void)
{
	// This is specific magic number for SD16_A only msp430f20x3
	// msp430f20x3 rev. B only!! From errata SLA153I.pdf
	*(uint8_t *const)(0xBF) = 0x61;

	//uint8_t *ptr;
	//ptr = (char *const)(0xBF);
	//*ptr = 0x61;

	// Clock source - used VLO (~12kHz)
	// SD16DIV_0 - Clock divider /1 
	// SD16REFON - Ref used only during measurement (for power safe)
	// SD16CTL = SD16LP + SD16DIV_0 + SD16SSEL_2 + SD16REFON;
	SD16CTL = SD16LP + SD16DIV_0 + SD16SSEL_2;

	// Unipolar, single mode and IE.
	SD16CCTL0 = SD16UNI + SD16SNGL + SD16IE;
}

void start_measurement(const uint8_t sd16a_channel)
{
	// Ref - On
	 SD16CTL |= SD16REFON;

	// A5 - voltage devider (AVcc - AVss) / 11
	// A6 - temperatrure sensor
	SD16INCTL0 = sd16a_channel;

	// SD16_A Start
	SD16CCTL0 |= SD16SC;

	if (sd16a_channel == VOLTAGE) {
		voltage_sd16a_measure = true;
	} else if (sd16a_channel == TEMPERATURE) {
		temperature_sd16a_measure = true;
	}
}

uint16_t simply_convert_voltage(const uint16_t raw_data)
{
	return raw_data / 1000;
}

/*
uint16_t convert_sd16a_raw_to_voltage(const uint32_t raw_data)
{
	return (uint16_t)((raw_data * 33) / 32768);
}
*/

int16_t convert_sd16a_raw_to_temperature(const uint16_t raw_data)
{
	//int_deg_c = ((temp_raw - 39758) * 455) / 65536;
	return ((int16_t)(raw_data - 39768) / 144);
}
