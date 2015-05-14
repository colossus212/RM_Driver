#include "stm32f10x.h"
#include "delay.h"
#include "ticker.h"

static uint8_t fac_us = 0;
static uint16_t fac_ms = 0;

void delay_init()
{
	fac_us = (uint8_t) (SystemCoreClock / 8000000);
	fac_ms = (uint16_t) (SystemCoreClock / 8000);
}

void delay_us(u32 nus)
{
	uint32_t curtick_hi;
	uint32_t curtick = Ticker_GetTick(&curtick_hi);
	uint32_t endtick_hi;
	uint32_t endtick;


	asm volatile
	(
		// calculate during time
		"ldr r0, %[us]\t\n"
		"ldrb r1, %[fus]\t\n"
		"umull r0, r1, r0, r1\t\n"
		"lsr r2, r0, #24\t\n"
		"lsl r1, #8\t\n"
		"orrs r1, r2\t\n"
		"mov r2, #0x00ffffff\t\n"
		"ands r0, r2\t\n"
		// calculate end time
		"ldr r3, %[ctl]\t\n"
		"add r3, r0\t\n"
		"ldr r4, %[cth]\t\n"
		"add r4, r1\t\n"
		"mov r2, 0x01000000\t\n"
		"tst r3, r2\t\n"
		"it ne\t\n"
		"addne r4, #1\t\n"
		"mov r2, 0x00ffffff\t\n"
		"ands r3, r2\t\n"
		"str r4, %[eth]\t\n"
		"str r3, %[etl]\t\n"
		:[eth] "=m" (endtick_hi), [etl] "=m" (endtick)
		:[cth] "m" (curtick_hi), [ctl] "m" (curtick), [us] "m" (nus), [fus] "m" (fac_us)
		:"r0", "r1", "r2", "r3", "r4"
	);

	while(1)
	{
		if((Ticker_GetTick(&curtick_hi) >= endtick && curtick_hi == endtick_hi) || curtick_hi > endtick_hi)
			break;
	}

}

void delay_ms(u16 nms)
{
	uint32_t curtick_hi;
	uint32_t curtick = Ticker_GetTick(&curtick_hi);
	uint32_t endtick_hi;
	uint32_t endtick;


	asm volatile
	(
		// calculate during time
		"ldrh r0, %[ms]\t\n"
		"ldrh r1, %[fms]\t\n"
		"umull r0, r1, r0, r1\t\n"
		"lsr r2, r0, #24\t\n"
		"lsl r1, #8\t\n"
		"orrs r1, r2\t\n"
		"mov r2, #0x00ffffff\t\n"
		"ands r0, r2\t\n"
		// calculate end time
		"ldr r3, %[ctl]\t\n"
		"add r3, r0\t\n"
		"ldr r4, %[cth]\t\n"
		"add r4, r1\t\n"
		"mov r2, 0x01000000\t\n"
		"tst r3, r2\t\n"
		"it ne\t\n"
		"addne r4, #1\t\n"
		"mov r2, 0x00ffffff\t\n"
		"ands r3, r2\t\n"
		"str r4, %[eth]\t\n"
		"str r3, %[etl]\t\n"
		:[eth] "=m" (endtick_hi), [etl] "=m" (endtick)
		:[cth] "m" (curtick_hi), [ctl] "m" (curtick), [ms] "m" (nms), [fms] "m" (fac_ms)
		:"r0", "r1", "r2", "r3", "r4"
	);

	while(1)
	{
		if((Ticker_GetTick(&curtick_hi) >= endtick && curtick_hi == endtick_hi) || curtick_hi > endtick_hi)
			break;
	}
}

