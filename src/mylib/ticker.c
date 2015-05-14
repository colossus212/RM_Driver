#include "ticker.h"
#include "stm32f10x_it.h"

#define TICKER_CYCLE_COUNT 0x00ffffff

uint32_t timer_high = 0;

void Ticker_Configuration(void)
{
	NVIC_InitTypeDef nvic;
	nvic.NVIC_IRQChannel = SysTick_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = ITP_SYSTICK_PREEMPTION;
	nvic.NVIC_IRQChannelSubPriority = ITP_SYSTICK_SUB;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);

	SysTick->LOAD = TICKER_CYCLE_COUNT;
	SysTick->CTRL = (SysTick->CTRL & 0xfffffff8) | 3; // div8, interrupt, enable
}

uint32_t Ticker_GetTick(uint32_t* hit)
{
	uint32_t t = SysTick->VAL;
	uint32_t th = timer_high;
	if( hit != 0 )
	{
		*hit = th;
	}
	return TICKER_CYCLE_COUNT - t;
}

void SysTick_Handler(void)
{
	timer_high++;
}

