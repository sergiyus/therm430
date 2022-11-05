#ifndef _SD16A_H_
#define _SD16A_H_

volatile uint16_t voltage_raw;
volatile uint16_t temp_raw;

enum {
	VOLTAGE = 5,
	TEMPERATURE = 6
};

enum {DELAY_VOLTAGE_MEASURE = 12};

/* N = V * 32768 / 3.3	*/
/* 29789 <-> 3.0V	*/
enum {LOW_BATTERY = 29789};

extern bool voltage_sd16a_measure;
extern bool temperature_sd16a_measure;

extern bool voltage_sd16a_result;
extern bool temperatrure_sd16a_result;

extern bool low_battery;

void config_sd16a(void);
void start_measurement(const uint8_t sd16a_channel);

/*
uint16_t convert_sd16a_raw_to_voltage(const uint16_t raw_data);
*/

int16_t convert_sd16a_raw_to_temperature(const uint16_t raw_data);

#endif
