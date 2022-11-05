#include <msp430f2003.h>
#include "timer.h"

void config_timer_a2(void)
{
	// CCR0 - Interupt enable.
	TACCTL0 = CCIE;

	TACCR0 = MAIN_DELAY - 1;
	// TA2:
	//	- ACLK (VLO/8 ~12kHz/8)
	//	- CLOCK/8 (~12kHz/8 ~ 1500 -> 0.000667sec)
	//	- Up MODE (timer counts up to TACCR0).
	
	TACCR1 = 0;
	TACTL = TASSEL_1 + ID_3 + MC_1;

	// TACCR1 IE enable.
	//TACCTL1 = CCIE;
}

