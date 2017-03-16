
#include "GPIO_config.h"

#ifndef _ARCH_GPIO_HEADER

#define _ARCH_GPIO_HEADER

extern void arch_GPIO_init(enum eGPIOPORT port);
extern void arch_GPIO_control(enum eGPIOPORT port, enum eGPIOControl onoff);

#endif
