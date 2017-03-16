//GPIO_config_structure

#ifndef _GPIO_HEADER
#define _GPIO_HEADER

enum eGPIOPORT
{
	eGPIOPORTA0,
	eGPIOPORTA5 = 5,
	eGPIOPORTB5 = 37,
	eGPIOPORTB6 = 38,
	eGPIOPORTB7 = 39,
	eGPIOPORTB8 = 40,
	eGPIOPORTB9 = 41,
	eGPIOPORTB10 = 42,
	eGPIOPORTB11 = 43,
	eGPIOPORTB13 = 45,
};

enum eGPIOdirection
{
	eGPIOdirection_input,
	eGPIOdirection_output,
};

enum eGPIOControl
{
	eGPIOControlOFF,
	eGPIOControlON,
};

#endif
