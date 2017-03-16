
#include "MKL02Z4.h"
#include "arch_GPIO.h"

void arch_GPIO_MUX(enum eGPIOPORT port)
{
	if(port<32)
	{
		PORTA_BASE_PTR->PCR[port]=(PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK);
	}
	else if(port>=32)
	{
		PORTB_BASE_PTR->PCR[port-32]=(PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK);
	}
}

void arch_GPIO_direction(enum eGPIOPORT port, enum eGPIOdirection direction)
{
	if(port<32)
	{
		//PTA_BASE_PTR->PDDR|=((direction&0x01)<<(port));
		GPIOA_BASE_PTR->PDDR|=((direction&0x01)<<(port));
	}
	else if(port>=32)
	{
		//PTB_BASE_PTR->PDDR|=((direction&0x01)<<(port-32));
		GPIOB_BASE_PTR->PDDR|=((direction&0x01)<<(port-32));
	}
}



void arch_GPIO_init(enum eGPIOPORT port)
{
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTA_MASK;
	arch_GPIO_MUX(port);
	arch_GPIO_direction(port, eGPIOdirection_output);
}

void arch_GPIO_control(enum eGPIOPORT port, enum eGPIOControl onoff)
{
	if(onoff)
	{
		if(port<32)
		{
			//PTA_BASE_PTR->PSOR=((0x01)<<(port));
			GPIOA_BASE_PTR->PSOR=((0x01)<<(port));
		}
		else if(port>=32)
		{
			//PTB_BASE_PTR->PSOR=((0x01)<<(port-32));
			GPIOB_BASE_PTR->PSOR=((0x01)<<(port-32));
		}
	}
	else
	{
		if(port<32)
		{
			//PTA_BASE_PTR->PCOR=((0x01)<<(port));
			GPIOA_BASE_PTR->PCOR=((0x01)<<(port));
		}
		else if(port>=32)
		{
			//PTB_BASE_PTR->PCOR=((0x01)<<(port-32));
			GPIOB_BASE_PTR->PCOR=((0x01)<<(port-32));
		}
	}
}


