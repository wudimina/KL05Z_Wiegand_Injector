//Wiegand.c

#include "wiegand.h"
#include <string.h>

tWiegandControl v_WiegandControl[NUM_WIEGAND_PORT];

void InitWiegandPort(enum eWiegandPort port, enum eGPIOPORT d0port,enum eGPIOPORT d1port)
{
	tWiegandControl* pthiswiegand = &v_WiegandControl[port];
	pthiswiegand->portd0 = d0port;
	pthiswiegand->portd1 = d1port;
	pthiswiegand->state = eWiegandState_Idle;
	pthiswiegand->bits_no = 0;
	pthiswiegand->wait_counter = 0;
	pthiswiegand->lastbit = 0;
	
	hal_GPIO_init(pthiswiegand->portd0);
	hal_GPIO_control(pthiswiegand->portd0,eGPIOControlON);
	hal_GPIO_init(pthiswiegand->portd1);
	hal_GPIO_control(pthiswiegand->portd1,eGPIOControlON);
}

void ProcessWiegandPort(tWiegandControl* pthiswiegand)
{
	uint8_t tempdata;
	if(pthiswiegand->state==eWiegandState_Sending)
	{
		if(pthiswiegand->lastbit)
		{
			hal_GPIO_control(pthiswiegand->portd1,eGPIOControlON);
		}
		else
		{
			hal_GPIO_control(pthiswiegand->portd0,eGPIOControlON);
		}
		
		if(pthiswiegand->bits_no==pthiswiegand->wiegand_data.bitlength)
		{
			pthiswiegand->state = eWiegandState_Idle;
			pthiswiegand->bits_no = 0;
			pthiswiegand->wait_counter = 0;
			pthiswiegand->lastbit = 0;
		}
		else
		{
			pthiswiegand->state = eWiegandState_Wait;
			pthiswiegand->wait_counter = WIEGAND_WAIT_COUNT;
		}

	}
	else if(pthiswiegand->state==eWiegandState_Wait)
	{
		if(pthiswiegand->wait_counter)
		{
			pthiswiegand->wait_counter--;
			if(pthiswiegand->wait_counter==0)
			{
				tempdata=pthiswiegand->wiegand_data.data[pthiswiegand->bits_no/8];
				pthiswiegand->lastbit = ((tempdata>>(7-pthiswiegand->bits_no%8))&0x01);
				
				if(pthiswiegand->lastbit)
				{
					hal_GPIO_control(pthiswiegand->portd1,eGPIOControlOFF);
#ifdef WIEGAND_DEBUG
					printf("1");
#endif
				}
				else
				{
					hal_GPIO_control(pthiswiegand->portd0,eGPIOControlOFF);
#ifdef WIEGAND_DEBUG
					printf("0");
#endif

				}
				
				pthiswiegand->state=eWiegandState_Sending;
				pthiswiegand->bits_no++;
				
			}
		}
	}
}

void SendWiegand(enum eWiegandPort port, tWiegandData *wieganddata)
{
	tWiegandControl* pthiswiegand = &v_WiegandControl[port];
	
	if(pthiswiegand->state==eWiegandState_Idle)
	{
		pthiswiegand->state = eWiegandState_Wait;
		pthiswiegand->wait_counter = 1;
		pthiswiegand->bits_no = 0;
		pthiswiegand->lastbit = 0;
		//pthiswiegand->wiegand_data.bitlength=wieganddata->bitlength;
		//pthiswiegand->wiegand_data.data[0]=wieganddata->data[0];
		memcpy(&pthiswiegand->wiegand_data,wieganddata,sizeof(tWiegandData));
	}
	else
	{
		//error, still sending wiegand
		printf("error");
	}
}

void ProcessWiegand()
{
	uint8_t i;
	for(i=0;i<NUM_WIEGAND_PORT;i++)
	{	
		ProcessWiegandPort(&v_WiegandControl[i]);
	}
}

void InitWiegandProcess()
{
	hal_timer_start(eTimer1,WIEGAND_TIMER_RESOLUTION,ProcessWiegand);
}
