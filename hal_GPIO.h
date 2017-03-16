
#include "GPIO_config.h"
#include "arch_GPIO.h"

#ifndef _HAL_GPIO_HEADER

#define _HAL_GPIO_HEADER

extern void hal_GPIO_init(enum eGPIOPORT port);
extern void hal_GPIO_control(enum eGPIOPORT port, enum eGPIOControl onoff);


#endif
