#ifndef _TIMER_H_
#define _TIMER_H_

#include <stdint.h>

// MAIN_DELAY in ms (X in ms) * 1000000 / 667
// t(sec) ~= 0.667ms * 14992 = 9999.6ms  ~= 10sec
// t(sec) ~= 0.667ms * 29985 = 19999.9ms ~= 20sec

// MAIN_DELAY = 10sec!
enum {MAIN_DELAY = 14992};

void config_timer_a2(void);

#endif
