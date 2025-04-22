#ifndef _SD16A_H_
#define _SD16A_H_

enum {
	VOLTAGE = 5,
	TEMPERATURE = 6
};

enum { DELAY_VOLTAGE_MEASURE = 18 };

/* N = V * 32768 / 3.3	*/
/* 29789 <-> 3.0V	*/
/* 27306 <-> 2.75V	*/
/* 21845 <-> 2.2V	*/
enum { LOW_BATTERY = 21845 };

extern bool voltage_sd16a_measure;
extern bool temperature_sd16a_measure;

extern bool voltage_sd16a_result;
extern bool temperature_sd16a_result;

extern bool low_battery;

void start_measurement(const uint8_t sd16a_channel);

#endif
