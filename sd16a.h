#pragma once

volatile uint32_t voltage_raw;
volatile uint32_t temp_raw;

enum {
	VOLTAGE = 5,
	TEMPERATURE = 6
};

enum {DELAY_VOLTAGE_MEASURE = 12};

extern bool voltage_sd16a_measure;
extern bool temperature_sd16a_measure;

extern bool voltage_sd16a_result;
extern bool temperatrure_sd16a_result;

void config_sd16a(void);
void start_measurement(uint8_t sd16a_channel);

uint32_t convert_sd16a_raw_to_voltage(uint32_t raw_data);
uint32_t convert_sd16a_raw_to_temperature(uint32_t raw_data);
