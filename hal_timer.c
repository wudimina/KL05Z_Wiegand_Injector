//timer.c

#include "hal_timer.h"

void hal_timer_start(enum eTimer timerindex, uint32_t timing_us, void callbackfunction(void))
{
	arch_timer_start(timerindex, timing_us,callbackfunction);
}

void hal_timer_stop(enum eTimer timerindex)
{
	 arch_timer_stop(timerindex);
}

