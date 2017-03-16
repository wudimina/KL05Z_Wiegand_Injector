/*
 * Copyright (c) 2017, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * [File Name]     main.c
 * [Platform]      FRDM-KL02Z
 * [Project]       myProject
 * [Version]       1.00
 * [Author]        towa2
 * [Date]          01/03/2017
 * [Language]      'C'
 * [History]       1.00 - Original Release
 *
 */

//-----------------------------------------------------------------------
// Standard C/C++ Includes
//-----------------------------------------------------------------------

#include <stdio.h>
//-----------------------------------------------------------------------
// KSDK Includes
//-----------------------------------------------------------------------
#include "main.h"
//-----------------------------------------------------------------------
// Application Includes
//-----------------------------------------------------------------------
#include "wiegand.h"
//-----------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------
tWiegandData tempwiegand;

//-----------------------------------------------------------------------
// Macros
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
// Main Function
//-----------------------------------------------------------------------

void senddata()
{
	static uint8_t counter=7;
	tWiegandData data;

	if(counter)
	{
		counter--;
		if(counter==0)
		{
			counter=7;
			data.bitlength=32;
			data.data[0]=0x55;
			data.data[1]=0xaa;
			data.data[2]=0x55;
			data.data[3]=0xaa;
			SendWiegand(eWiegandPort1, &data);
		}
	}
}

unsigned char AsciitoHex (char ch)
{
	char c = ch;

	if(c>'9')
		c = c-'A'+10;
	else
		c = c - '0';

	return c;
}

unsigned int HexStrtoChar (uint8_t *buf)
{
	unsigned int dat=0;
	dat += (AsciitoHex(*buf++) * 10);
	dat += AsciitoHex(*buf++);
	return dat;
}

void ProcessSerialCommand(uint8_t *dat)
{
	enum eWiegandPort portselection;
	uint8_t mode;
	uint8_t result;

	if(*dat == 'S')
	{
		dat++;
		if(*dat == 'W')
		{
			uint8_t i;
			dat++;

			mode = AsciitoHex(*dat);

			dat++;
			portselection = HexStrtoChar(dat);
			if(portselection>=NUM_WIEGAND_PORT)
			{
				return ;
			}
			dat+=2;
			tempwiegand.bitlength = HexStrtoChar(dat);

			dat+=2;

			if(tempwiegand.bitlength%8)
			{
				result=1;
			}
			else
			{
				result=0;
			}

			for(i=0;i<((tempwiegand.bitlength/8)+result);i++)
			{
				tempwiegand.data[i]=AsciitoHex(*dat)<<4;
				dat++;
				tempwiegand.data[i]|=AsciitoHex(*dat);
				dat++;
			}
			if(mode==1)
			{
				for(i=0;i<NUM_WIEGAND_PORT;i++)
				{
					SendWiegand(i, &tempwiegand);
				}
			}
			else
			{
				SendWiegand(portselection, &tempwiegand);
			}
		}
	}
}

int main(void)
{
	uint8_t data[50];
	uint8_t i=0;


    // Configure board specific pin muxing
    hardware_init();

    // Initialize UART terminal
    dbg_uart_init();

    PRINTF("\r\nRunning the myProject project.\r\n");

	InitWiegandPort(eWiegandPort1, eGPIOPORTB8, eGPIOPORTB9);
	InitWiegandPort(eWiegandPort2, eGPIOPORTB6, eGPIOPORTB7);
	InitWiegandPort(eWiegandPort3, eGPIOPORTB10, eGPIOPORTB11);
	InitWiegandPort(eWiegandPort4, eGPIOPORTB5, eGPIOPORTB13);
    InitWiegandProcess();

    for (;;)                                         // Forever loop
    {
        //__asm("NOP");
		//data[i]=in_char();
    	data[i]=GETCHAR();
		if(data[i]==13)
		{
			ProcessSerialCommand(data);
			i=0;
		}
		else
		{
			//printf("%i",data[i]);
			i++;
		}
    }


}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
