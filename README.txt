therm430
Two-digit seven-segment thermometer on MSP430F2003 + 2 digit 7 segment LED.
original idea from http://www.technoblogy.com/show?2G8T

The range of measured temperature: from -19C to +99C.

Temperature display in the range from < -19C 		- lo
					-19C...-10C 	- two-digit decimal with "-" in the upper digit
					-9С...-1С	- senior digit "-", junior decimal number
					 0С 		- the highest digit is not filled, the lowest is 0
					+1С to +9С 	- the highest digit is not filled,
					+10C to 99C 	- a two-digit decimal number,
					> 99C 		- hi
Indication of battery discharge - lb.

Work format
Temperature indication ~ 200ms, once every ~ 10 seconds.
If the voltage of the power element is lower than 2.2V - before displaying the temperature - indicate lb.

An internal temperature sensor and an ADC are used to measure the temperature (it is not very accurate).

The code was compiled using the compiler from energia.nu with the -Os optimization flag.
