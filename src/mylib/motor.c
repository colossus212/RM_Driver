#include "stm32f10x.h"
#include "delay.h"
#include <stdint.h>
#include "main.h"
#include "util.h"
#include "motor.h"

//PA8----EN
//PA9----PWM---TIM1_CH2
//PA10---EN
//PA11---PWM---TIM1_CH4

#define LEFT_LOW_ON()   GPIO_SetBits(GPIOA,GPIO_Pin_8)
#define LEFT_LOW_OFF()  GPIO_ResetBits(GPIOA,GPIO_Pin_8)
#define RIGHT_LOW_ON()  GPIO_SetBits(GPIOA,GPIO_Pin_10)
#define RIGHT_LOW_OFF() GPIO_ResetBits(GPIOA,GPIO_Pin_10)

uint16_t Max_Speed = 4500;
int8_t vP = 11, vI = 2, vD = 0;
uint16_t Speed_Step = 20;

uint8_t M_Enable = 0;

void Motor_Init(void)
{
	TIM_TimeBaseInitTypeDef tim;
	TIM_OCInitTypeDef oc;
	GPIO_InitTypeDef gpio;

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA |
			RCC_APB2Periph_TIM1, ENABLE);

	gpio.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_10;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio);
	GPIO_ResetBits(GPIOA, GPIO_Pin_8 | GPIO_Pin_10);

	gpio.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11;
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio);

	tim.TIM_Period = 5000;
	tim.TIM_Prescaler = 0;
	tim.TIM_ClockDivision = TIM_CKD_DIV1;
	tim.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1, &tim);

	oc.TIM_OCMode = TIM_OCMode_PWM2;
	oc.TIM_OutputState = TIM_OutputState_Enable;
	oc.TIM_OutputNState = TIM_OutputNState_Disable;
	oc.TIM_Pulse = 0;
	oc.TIM_OCPolarity = TIM_OCPolarity_Low;
	oc.TIM_OCNPolarity = TIM_OCPolarity_High;
	oc.TIM_OCIdleState = TIM_OCIdleState_Reset;
	oc.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	TIM_OC2Init(TIM1, &oc);
	TIM_OC4Init(TIM1, &oc);

	TIM_ARRPreloadConfig(TIM1, ENABLE);

	TIM_CtrlPWMOutputs(TIM1, ENABLE);

	TIM_Cmd(TIM1, ENABLE);
}

void Motor_Enable(uint8_t men)
{
	M_Enable = men;
}

void Motor_Configuration(Motor_Config_Type mct, void* value)
{
	switch(mct)
	{
	case MCT_Max_Speed:
		Max_Speed = *(uint16_t*)value;
		break;
	case MCT_PID:
		vP = ((int8_t*)value)[0];
		vI = ((int8_t*)value)[1];
		vD = ((int8_t*)value)[2];
		break;
	case MCT_Speed_Step:
		Speed_Step = *(uint16_t*)value;
		break;
	}
}

void Motor_Speed(int32_t speed)
{
	static int32_t last_speed = 0;

	if (!M_Enable)
		speed = 0;
	if (abs(speed) > Max_Speed)
	{
		if (speed < 0)
			speed = -Max_Speed;
		else if (speed > 0)
			speed = Max_Speed;
	}

	if (speed > 0)
	{
		if (last_speed <= 0)
		{
			TIM1->CCR4 = 0;
			delay_us(100);
			LEFT_LOW_OFF();
			delay_us(100);
			RIGHT_LOW_ON();
			delay_us(100);
			TIM1->CCR2 = speed;
		}
		else
		{
			LEFT_LOW_OFF();
			RIGHT_LOW_ON();
			TIM1->CCR2 = speed;
		}
	}

	else if (speed < 0)
	{
		if (last_speed >= 0)
		{
			TIM1->CCR2 = 0;
			delay_us(100);
			RIGHT_LOW_OFF();
			delay_us(100);
			LEFT_LOW_ON();
			delay_us(100);
			TIM1->CCR4 = -speed;
		}
		else
		{
			RIGHT_LOW_OFF();
			LEFT_LOW_ON();
			TIM1->CCR4 = -speed;
		}
	}
	else
	{
		TIM1->CCR2 = 0;
		TIM1->CCR4 = 0;
		LEFT_LOW_OFF();
		RIGHT_LOW_OFF();
	}

	last_speed = speed;
}

float Get_MxMi(int num, int max, int min)
{
	if (num > max)
		return max;
	else if (num < min)
		return min;
	else
		return num;
}

/* test
 * in: vP(8bit), vI(8bit), vD(8bit), want_speed(16bit)
 * out: tick, current_speed (16bit), error_i(16bit),error_d(16bit)
*/


int32_t Motor_velocity_control(int current_speed, int want_speed)
{
	int error, error_d;
	int32_t output;
	static int error_i = 0, last_error = 0;

	error = want_speed - current_speed;

	if(abs(error) >= Speed_Step)
	{
		if(error > 0) error = Speed_Step;
		else error = -Speed_Step;
	}

	error_i += error;
	error_i = Get_MxMi(error_i, 1000.0, -1000.0);
	error_d = error - last_error;
	last_error = error;
	output = vP * error + vI * error_i + vD * error_d;
	output = Get_MxMi(output, Max_Speed, -Max_Speed);
	Motor_Speed(output);


	return output;
}
