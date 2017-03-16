
#include "MKL02Z4.h"
#include "timer_config.h"

#ifndef _ARCH_TIMER_HEADER

#define _ARCH_TIMER_HEADER

#define SYS_FREQ	48000000

#define MAX_TIMER_TICK		(65535)
#define MAX_PRESCALAR_VALUE	(128)
#define US_CONVERSION		(1000000)

extern void arch_timer_start(enum eTimer timerindex, uint32_t timing_us, void callbackfunction(void));
extern void arch_timer_stop(enum eTimer timerindex);

#endif
