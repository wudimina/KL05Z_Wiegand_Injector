
//#include "common.h"	//for enable_irq
//#include "isr.h"
#include "arch_timer.h"


void (*tpm0callback)(void) = 0;
void (*tpm1callback)(void) = 0;

static void timer_clear_irq(enum eTimer timerindex)
{
	TPM_MemMapPtr pthistpm;
	switch(timerindex)
	{
		case eTimer1:
			pthistpm=TPM0_BASE_PTR;
			break;
		case eTimer2:
			pthistpm=TPM1_BASE_PTR;
			break;
		default:
			break;
	}
	pthistpm->SC |= TPM_SC_TOF_MASK;
	pthistpm->CNT = TPM_CNT_COUNT(0);
}

static void arch_timer_restart(enum eTimer timerindex)
{
	TPM_MemMapPtr pthistpm;
	switch(timerindex)
	{
		case eTimer1:
			pthistpm=TPM0_BASE_PTR;
			break;
		case eTimer2:
			pthistpm=TPM1_BASE_PTR;
			break;
		default:
			break;
	}
	pthistpm->SC |= TPM_SC_CMOD(1);
}

void arch_timer_stop(enum eTimer timerindex)
{
	TPM_MemMapPtr pthistpm;
	switch(timerindex)
	{
		case eTimer1:
			pthistpm=TPM0_BASE_PTR;
			break;
		case eTimer2:
			pthistpm=TPM1_BASE_PTR;
			break;
		default:
			break;
	}
	pthistpm->SC |= TPM_SC_CMOD(0);
}

//void tpm0_isrv(void)
void TPM0_IRQHandler(void)
{	
	timer_clear_irq(eTimer1);
	arch_timer_stop(eTimer1);
	(*tpm0callback)();
	arch_timer_restart(eTimer1);
}

//void tpm1_isrv(void)
void TPM1_IRQHandler(void)
{
	timer_clear_irq(eTimer2);
	arch_timer_stop(eTimer2);
	(*tpm1callback)();
	arch_timer_restart(eTimer2);
}

void arch_timer_start(enum eTimer timerindex, uint32_t timing_us, void callbackfunction(void))
{
	uint32_t tick_required = 0xffffffff; //maximize it
	uint32_t prescaler = 0;
	uint32_t count;
	uint32_t temp_prescalar;
	TPM_MemMapPtr pthistpm;
	
	SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1); 
	
	switch(timerindex)
	{
		case eTimer1:
			pthistpm=TPM0_BASE_PTR;
			tpm0callback = callbackfunction;
			SIM_SCGC6 |= SIM_SCGC6_TPM0_MASK;
			//enable_irq(INT_TPM0 - 16);
			NVIC_EnableIRQ(TPM0_IRQn);
			break;
		case eTimer2:
			pthistpm=TPM1_BASE_PTR;
			tpm1callback = callbackfunction;
			SIM_SCGC6 |= SIM_SCGC6_TPM1_MASK;
			//enable_irq(INT_TPM1 - 16);
			NVIC_EnableIRQ(TPM1_IRQn);
			break;
		default:
			break;
	}
	
	while(tick_required > MAX_TIMER_TICK)
	{
		if(prescaler)	
			prescaler = prescaler *2;
		else
			prescaler = 1;
			
		tick_required = (SYS_FREQ/ US_CONVERSION) * (timing_us/prescaler);
	}

	if(prescaler==1)
	{
		prescaler=0;
	}
	else
	{	
		temp_prescalar=1;
		count=0;
		
		while(temp_prescalar<=MAX_PRESCALAR_VALUE)
		{
			temp_prescalar*=2;
			count++;
			if(temp_prescalar==prescaler)
				break;
		}
		prescaler = count;
	}
	
	pthistpm->CNT = TPM_CNT_COUNT(0);
	pthistpm->MOD = TPM_MOD_MOD(tick_required);
	pthistpm->SC = TPM_SC_TOIE_MASK|TPM_SC_CMOD(1)|TPM_SC_PS(prescaler);
}








