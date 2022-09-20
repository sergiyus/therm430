#include <msp430f2003.h>
#include "ports.h"

void config_ports(void)
{
	// All pins are output
	P1SEL &= ~0xFF;
	P1DIR |= 0xFF;

	P2SEL &= ~(BIT7 + BIT6);
	P2DIR |= BIT6 + BIT7;
}
