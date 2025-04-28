# therm430

**Two-digit seven-segment thermometer on MSP430F2003 + 2-digit 7-segment LED.**

Original idea from [Technoblogy](http://www.technoblogy.com/show?2G8T).

## Temperature Range

*   Measured temperature range: from **\-19°C** to **+99°C**.

### Temperature Display:

*   **< -19°C**: Display "lo".
*   **\-19°C to -10°C**: Two-digit decimal with a "-" in the upper digit.
*   **\-9°C to -1°C**: Upper digit "-", lower digit is the decimal number.
*   **0°C**: The upper digit is not filled, the lower digit is 0.
*   **+1°C to +9°C**: The upper digit is not filled.
*   **+10°C to +99°C**: A two-digit decimal number.
*   **> 99°C**: Display "hi".

### Battery Discharge Indicator

*   Display "lb" when the battery voltage is low.

## Work Format

*   The temperature is displayed for **~200ms**, approximately every **10 seconds**.
*   If the voltage of the power source is lower than **2.2V**, the thermometer will indicate "lb" before showing the temperature.

## Hardware

*   An internal temperature sensor and an ADC are used to measure the temperature (note that the accuracy is limited).
*   **Button for Power Source Voltage Indication**: A button is included in the project to indicate the power source voltage. When pressed, it displays the voltage level of the power supply, providing an easy way to monitor the system's power status.
*   **New Option**: On the first press, the system displays the voltage level of the power supply. On the next press, it shows the current temperature. This alternates with each subsequent press, allowing for easy monitoring of both parameters.

## Important Note on Temperature Range

*   The theoretical maximum temperature that can be displayed is **+99°C**. However, this value is practically unreachable, as the components of the system are unlikely to withstand such high temperatures. The temperature measurement range is designed with typical operating conditions in mind, and exceeding these limits may damage the hardware.

## Compilation and Toolchain

*   The code was originally compiled using the **energia.nu** compiler with the **\-Os** optimization flag.
*   **New Option**: The project now also supports compilation with the **msp430-gcc-opensource** toolchain from [Texas Instruments](https://www.ti.com/tool/MSP430-GCC-OPENSOURCE).
*   Ensure the correct toolchain is selected in the respective Makefile (choose the variable in the Makefile: COMPILER = energia for the energia.nu compiler or COMPILER = gcc for the MSP430-GCC-OPENSOURCE compiler). Alternatively, you can use `make COMPILER=gcc` (for energia.nu).

## MSP430F20x3 Microcontroller

*   The microcontroller used is **MSP430F2003**. All available MSP430 microcontrollers in my project are sourced from old Apple MagSafe 1 power supplies (45W, 60W, or 85W).
*   These power supplies often contain microcontrollers labeled **v125d** or **v361**, which behave similarly to the **MSP430F2003** when read with **MSP430Flasher**.
*   Interestingly, the **MSP430F2003** is specified to have **1KB Flash**, but I have been able to read and write **2KB**. The **MSP430Flasher** tool recognizes these microcontrollers as **MSP430F20x3**, thus providing an additional **1KB** of Flash memory for use in the project.

## Debugging and Flashing

*   The project uses the **mspdebug** tool (available from [GitHub](https://github.com/dlbeer/mspdebug)) for debugging and flashing when compiled with the **energia.nu** toolchain.
*   The flashing tool used in Makefile configurations is **MSP430Flasher**, which supports both reading and writing firmware.
*   An alternative Makefile, **Makefile_elf**, uses the **msp430-gcc-opensource** toolchain from [Texas Instruments](https://www.ti.com/tool/MSP430-GCC-OPENSOURCE).

