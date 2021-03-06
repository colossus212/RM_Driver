
#include <stdint.h>
#include <stdio.h>
#include "misc.h"

#include "stm32f10x.h"
#include "main.h"

#include "led.h"
#include "usart2.h"
#include "delay.h"
#include "encoder.h"
#include "tim2.h"
#include "can1.h"
#include "motor.h"
#include "debug.h"
#include "ad.h"
#include "ticker.h"
#include "maincontrol.h"


int main(void)
{
	//设置NVIC中断分组2位抢占优先级，2位响应优先级
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2) ;

	Ticker_Configuration();

	LED_Configuration();

	USART2_Configuration();

	delay_init();

	//Do_Loop_LED_Test();
	//Do_Loop_Motor_Test();
	//DISABLE_FOLLOWING_CODE(1);

	//主控通信控制器初始化
	Maincontrol_Configuration();

	Encoder_Configuration();
	Encoder_Start();

	//速度采样控制器初始化
	TIM2_Configuration(5);
	TIM2_Start();

	//电流检测
	ADC_Configuration();

	//电机初始化
	Motor_Init();

	while (1)
	{
		delay_ms(200);

		LED_RED_TOGGLE();
		LED_GREEN_TOGGLE();
	}
}
