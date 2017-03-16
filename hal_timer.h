//timer.h

#include "stdint.h"
#include "arch_timer.h"
#include "timer_config.h"


#ifndef _HAL_TIMER_HEADER

#define _HAL_TIMER_HEADER

extern void hal_timer_start(enum eTimer timerindex, uint32_t timing_us, void callbackfunction(void));
extern void hal_timer_stop(enum eTimer timerindex);

#endif
