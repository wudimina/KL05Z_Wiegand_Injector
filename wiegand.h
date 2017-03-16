//Wiegand.h

#include "stdint.h"
#include "hal_GPIO.h"
#include "hal_timer.h"

#define WIEGAND_TIMER_RESOLUTION	(50)	//50 us

#define MAX_WEIGAND_BITS (128)

#define WIEGAND_WAIT_COUNT	(20)

#define WIEGAND_DEBUG

enum eWiegandPort
{
	eWiegandPort1,
	eWiegandPort2,
	eWiegandPort3,
	eWiegandPort4,
	NUM_WIEGAND_PORT
};

enum eWiegandState
{
	eWiegandState_Idle,
	eWiegandState_Wait,
	eWiegandState_Sending
};

typedef struct
{
	uint8_t bitlength;
	uint8_t data[MAX_WEIGAND_BITS/8];
}tWiegandData;

typedef struct
{
	enum eGPIOPORT portd0;
	enum eGPIOPORT portd1;
	uint8_t lastbit;
	uint8_t state;
	uint8_t bits_no;
	uint8_t wait_counter;
	tWiegandData wiegand_data;
}tWiegandControl;

//TEST
//extern void ProcessWiegand();


extern void InitWiegandProcess(void);
extern void InitWiegandPort(enum eWiegandPort port, enum eGPIOPORT d0port,enum eGPIOPORT d1port);
extern void SendWiegand(enum eWiegandPort port, tWiegandData *wieganddata);
