#ifndef _SD16A_H_
#define _SD16A_H_

/**
 * @brief The SD16A channels:
 */
enum {
	VOLTAGE = 5,
	TEMPERATURE = 6
};

/* N = V * 32768 / 3.3	*/
/* 29789 <-> 3.0V	*/
/* 27306 <-> 2.75V	*/
/* 21845 <-> 2.2V	*/
enum { LOW_BATTERY = 21845 };

/**
 * @brief Initializes the SD16A on MSP430.
 *
 * This function configures the SD16A ADC for operation. It sets specific
 * control registers and parameters required for the ADC to function correctly.
 * The function includes a specific magic number for the SD16A, which is
 * applicable only to the MSP430F20x3 revision B, as noted in the errata SLA153I.pdf.
 *
 * The ADC is configured to use the VLO (Very Low Frequency Oscillator) as the
 * clock source, with a clock divider of 1. The reference voltage is enabled
 * only during measurements to ensure power efficiency. The ADC is set to
 * operate in unipolar, single conversion mode with interrupts enabled.
 *
 * @note This function should be called before using the SD16A ADC for
 * conversions.
 * @note Ensure that the specific magic number is applicable to your device
 * revision to avoid unexpected behavior.
 * @note This implementation is specific to the MSP430F20x3 (rev. B).
 *       Ensure to refer to SLA153I.pdf for additional details.
 *
 */
void sd16a_init(void);

/**
 * @brief Get the raw data from the specified sd16a channel.
 *
 * @param sd16a_channel The channel to select for measurement.
 *
 * @return Raw data as a 16-bit unsigned integer.
 */
uint16_t get_sd16a_raw(const uint8_t sd16a_channel);

#endif /* _SD16A_H_ */
