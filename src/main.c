/*
 * therm430
 * The two digit 7 segment thermometer.
 * Original idea from http://www.technoblogy.com/show?2G8T
 */

#include <msp430.h>
#include <stdint.h>
#include "sd16a.h"
#include "7seg_lcd.h"
#include "logic.h"

#define CONVERT_VOLTAGE(VOLTAGE_RAW)    ((VOLTAGE_RAW) / 992)
#define CONVERT_TEMP(TEMP_RAW)          ((int16_t)((TEMP_RAW) - 39768) / 144)

int main(void)
{
    board_init();
    sd16a_init();

    uint16_t battery_check_interval = 0;
    struct dspl2d dspl = {0, 0};
    status = FLAG_PERIODIC;

	_BIS_SR(GIE);

	while (1) {
        /* Check Battery Voltage */
        if (battery_check_interval == 0) {
            if (get_sd16a_raw(VOLTAGE) < LOW_BATTERY) {
                status |= FLAG_BATTERY_LOW;
            } else {
                status &= ~FLAG_BATTERY_LOW;
            }
            battery_check_interval = DELAY_VOLTAGE_MEASURE - 1;
        }

        /* Show the alert 'Lb' when the battery is low */
        /* Show the Temperature */
        if (status & FLAG_PERIODIC) {
            if (status & FLAG_BATTERY_LOW) {
				dspl.digit1 = L;
				dspl.digit0 = b;
                print2d(&dspl, DIGIT_TIME_ON);
                delay_667mks(DIGIT_TIME_ON);

            }
            convert_temp2d(CONVERT_TEMP(get_sd16a_raw(TEMPERATURE)), &dspl);
            print2d(&dspl, DIGIT_TIME_ON);
            status &= ~FLAG_PERIODIC;
        }

        /* The Button Press Handler */
        /* Show the temperature of battery voltage */
        if (status & FLAG_BUTTON_PRESS) {
            if (status & FLAG_SHOW_VOLT) {
                convert_volt2d(CONVERT_VOLTAGE(get_sd16a_raw(VOLTAGE)), &dspl);
            } else {
                convert_temp2d(CONVERT_TEMP(get_sd16a_raw(TEMPERATURE)), &dspl);
            }
            print2d(&dspl, DIGIT_TIME_ON);

            status ^= FLAG_SHOW_VOLT;
            status &= ~FLAG_BUTTON_PRESS;
        }

        --battery_check_interval;

		LPM3;
	}
}

