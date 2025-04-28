#ifndef _7SEG_LCD_H_
#define _7SEG_LCD_H_

#define DIG0	BIT7
#define	DIG1	BIT6

#define seg_A	BIT7
#define seg_B	BIT6
#define seg_C	BIT5
#define seg_D	BIT4
#define seg_E	BIT3
#define seg_F	BIT2
#define seg_G	BIT1
#define DP      BIT0

extern const uint8_t SEVENSEG_OUTPUT[];

/* limitation two-digit display */
enum {HIGHT_TEMP	=  99};
enum {LOW_TEMP		= -19};

/* hi - hight           */
/* Lo - low             */
/* Lb - low battery     */
/* Er - Error.          */
enum {
	MINUS	= seg_G,                                /* - */
	H	= seg_B + seg_C + seg_E + seg_F + seg_G,	/* H */
	i	= seg_C,					                /* i */
	L	= seg_D + seg_E + seg_F,                    /* L */
	o	= seg_C + seg_D + seg_E + seg_G,            /* o */
	b	= seg_C + seg_D + seg_E + seg_F + seg_G,    /* b */
/*	E	= seg_A + seg_D + seg_E + seg_F + seg_G, */	/* E */
/*	r	= seg_E + seg_G                          */ /* r */
};

#endif
