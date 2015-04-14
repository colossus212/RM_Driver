#include <stdint.h>
#include <stdio.h>

#include "stm32f10x.h"
#include "main.h"

#include "led.h"
#include "usart2.h"
#include "pwm.h"
#include "can.h"
#include "delay.h"
#include "encoder.h"
#include "tim2.h"
#include "motor.h"
#include "debug.h"

//当前编码器计数值 ../10ms
int16_t Encoder_Speed = 0;
//给定转速 r/10ms
int16_t Target_Speed = 0;

int16_t motor_dest_speed = 0;

int main(void)
{
	//LED初始化...
	LED_Configuration();
	//串口初始化
	USART2_Configuration();
	//PWM初始化...
	PWM_Configuration();
	CAN_Configuration();
	//delay()初始化...
	delay_init();
	//计数初始化
	Encoder_Configuration();
	Encoder_Start();
	//定时器初始化
	TIM2_Configuration(10);
	TIM2_Start();

	//电机初始化
	Motor_Init();

	while (1)
	{
		delay_ms(1000);

		Target_Speed = motor_dest_speed;

		printf("%d, %d\r\n", Target_Speed, Encoder_Speed);

		LED_RED_TOGGLE();
	}
}
