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

//��ǰ����������ֵ ../10ms
int16_t Encoder_Speed = 0;
//����ת�� r/10ms
int16_t Target_Speed = 0;

int16_t motor_dest_speed = 0;

int main(void)
{
	//LED��ʼ��...
	LED_Configuration();
	//���ڳ�ʼ��
	USART2_Configuration();
	//PWM��ʼ��...
	PWM_Configuration();
	CAN_Configuration();
	//delay()��ʼ��...
	delay_init();
	//������ʼ��
	Encoder_Configuration();
	Encoder_Start();
	//��ʱ����ʼ��
	TIM2_Configuration(10);
	TIM2_Start();

	//�����ʼ��
	Motor_Init();

	while (1)
	{
		delay_ms(1000);

		Target_Speed = motor_dest_speed;

		printf("%d, %d\r\n", Target_Speed, Encoder_Speed);

		LED_RED_TOGGLE();
	}
}
