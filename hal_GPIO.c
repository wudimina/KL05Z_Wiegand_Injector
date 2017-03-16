
#include "hal_GPIO.h"

void hal_GPIO_init(enum eGPIOPORT port)
{
	arch_GPIO_init(port);
}

void hal_GPIO_control(enum eGPIOPORT port, enum eGPIOControl onoff)
{
	arch_GPIO_control(port, onoff);

}

