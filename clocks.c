#include <msp430f2003.h>
#include "clocks.h"

// Config clock's:
// MCLK = DCO = 1MHz
// SMCLK = DCO = 1MHz
// ACLK = VLO = 12kHz
void config_clocks(void)
{
	if (CALBC1_1MHZ == 0xFF || CALDCO_1MHZ == 0xFF) {
		// fDCO(7,3)DCO frequency (7, 3)
		// RSELx = 7, DCOx = 3, MODx = 0
		// 2.2 V, 3 V F = 0.80 .. 1.50 MHz
		BCSCTL1 = RSEL2 + RSEL1 + RSEL0;
		DCOCTL = DCO1 + DCO0;
	} else {
		BCSCTL1 = CALBC1_1MHZ;
		DCOCTL = CALDCO_1MHZ;
	}

	// LFXT1 = VLO.
	BCSCTL3 |= LFXT1S_2;
	// Clear OSCFault flag
	//IFG1 &= ~OFIFG;
	// ACLK = VLO.
	//BCSCTL1 |= DIVA_0;

	// MCLK = DCO, SMCLK = DCO.
	//BCSCTL2 |= SELM_0 + DIVM_0 + DIVS_0;
}
