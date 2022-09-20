#include <msp430f2003.h>
#include <stdint.h>
#include <stdbool.h>
#include "sd16a.h"

bool voltage_sd16a_measure = false;
bool temperature_sd16a_measure = false;
bool voltage_sd16a_result = false;
bool temperatrure_sd16a_result = false;

void config_sd16a(void)
{
	// This specific magic number for SD16_A only msp430f20x3
	// msp430f20x3 rev. B only!! SLA153I.pdf
	*(char *)0xBF = 0x61;

	// Clock source - used VLO (~12kHz)
	// SD16DIV_0 - Clock divider /1 
	// SD16REFON - Ref used only during measurement (for power safe)
	SD16CTL = SD16LP + SD16DIV_0 + SD16SSEL_2 + SD16REFON;
	//SD16CTL = SD16LP + SD16DIV_0 + SD16SSEL_2;

	// Unipolar, single mode and IE.
	SD16CCTL0 = SD16UNI + SD16SNGL + SD16IE;
}

void start_measurement(uint8_t sd16a_channel)
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

uint32_t convert_sd16a_raw_to_voltage(uint32_t raw_data)
{
	return ((raw_data * 330) / 32768);
}

uint32_t convert_sd16a_raw_to_temperature(uint32_t raw_data)
{
	//int_deg_c = ((temp_raw - 39758) * 455) / 65536;
	//return ((raw_data - 39800) / 144);
	return ((raw_data - 39700) / 144);
}

