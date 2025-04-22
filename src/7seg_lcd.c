#include "7seg_lcd.h"

/* 7-seg LED display from "0" to "9". */
const uint8_t SEVENSEG_OUTPUT[] = {
	seg_A + seg_B + seg_C + seg_D + seg_E + seg_F,		/* 0 */
	seg_B + seg_C,						/* 1 */
	seg_A + seg_B + seg_G + seg_E + seg_D,			/* 2 */
	seg_A + seg_B + seg_G + seg_C + seg_D,			/* 3 */
	seg_F + seg_G + seg_B + seg_C,				/* 4 */
	seg_A + seg_F + seg_G + seg_C + seg_D,			/* 5 */
	seg_A + seg_F + seg_G + seg_C + seg_D + seg_E,		/* 6 */
	seg_A + seg_B + seg_C,					/* 7 */
	seg_A + seg_B + seg_C + seg_D + seg_E + seg_F + seg_G,	/* 8 */
	seg_A + seg_B + seg_C + seg_D + seg_G + seg_F		/* 9 */
	/*seg_A + seg_B + seg_C + seg_E + seg_F + seg_G, */	/* A */
	/*seg_C + seg_D + seg_E + seg_G + seg_F,	 */	/* b */
	/*seg_A + seg_D + seg_E + seg_F,		 */	/* C */
	/*seg_B + seg_C + seg_D + seg_E + seg_G,	 */	/* d */
	/*seg_A + seg_D + seg_E + seg_F + seg_G,	 */	/* E */
	/*seg_A + seg_E + seg_F + seg_G,		 */	/* F */
	/*DP,						 */	/* DP */
	/*0						 */	/* nothing */
};

