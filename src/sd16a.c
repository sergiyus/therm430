#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>
#include "sd16a.h"

/**
 * @brief This is flag is used to check if the sd16a data is ready.
 */
static volatile bool sd16a_redy;

/**
 * @brief Variable for storing the result from sd16a.
 */
static volatile uint16_t sd16a_raw_data;

void sd16a_init(void)
{
	/* config_sd16a(); */
	// This is specific magic number for SD16_A
	// msp430f20x3 rev. B only!!! From errata SLA153I.pdf
	*(uint8_t *const)(0xBF) = 0x61;

	// Clock source - used VLO (~12kHz)
	// SD16DIV_0 - Clock divider /1
	// SD16REFON - Ref used only during measurement (for power safe)
	// SD16CTL = SD16LP + SD16DIV_0 + SD16SSEL_2 + SD16REFON;
	SD16CTL = SD16LP + SD16DIV_0 + SD16SSEL_2;

	// Unipolar, single mode and IE.
	SD16CCTL0 = SD16UNI + SD16SNGL + SD16IE;
}

uint16_t get_sd16a_raw(const uint8_t sd16a_channel)
{
    sd16a_redy = false;
	// Ref - On
	 SD16CTL |= SD16REFON;
	// A5 - voltage devider (AVcc - AVss) / 11
	// A6 - temperature sensor
	SD16INCTL0 = sd16a_channel;
	// SD16_A Start
	SD16CCTL0 |= SD16SC;

    while (!sd16a_redy) {
        LPM3;
    }
    return sd16a_raw_data;
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = SD16_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__ ((interrupt(SD16_VECTOR)))
#else
#error Compiler not supported!
#endif
void SD16ISR(void)
{
	/* Ref - OFF, for power safe */
	SD16CTL &= (uint16_t) ~SD16REFON;
    sd16a_raw_data = SD16MEM0;
    sd16a_redy = true;

	LPM3_EXIT;
}

