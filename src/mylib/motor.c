#include "stm32f10x.h"
#include "delay.h"
#include <stdint.h>
#include "main.h"
#include "util.h"
#include "motor.h"
#include "pid.h"

//PA8----EN
//PA9----PWM---TIM1_CH2
//PA10---EN
//PA11---PWM---TIM1_CH4

#define LEFT_LOW_ON()   GPIO_SetBits(GPIOA,GPIO_Pin_8)
#define LEFT_LOW_OFF()  GPIO_ResetBits(GPIOA,GPIO_Pin_8)
#define RIGHT_LOW_ON()  GPIO_SetBits(GPIOA,GPIO_Pin_10)
#define RIGHT_LOW_OFF() GPIO_ResetBits(GPIOA,GPIO_Pin_10)

static uint16_t Max_Current = 4500;
static int8_t vP = 11, vI = 2, vD = 0;
static uint16_t Speed_Step = 20;

static uint8_t M_Enable = 0;

static int16_t Target_Speed = 0;
static int16_t Last_Real_Target_Speed = 0;

static PID_Controller pid_motor_ctrl;

void Motor_Init(void)
{

	TIM_TimeBaseInitTypeDef tim;
	TIM_OCInitTypeDef oc;
	GPIO_InitTypeDef gpio;

    Motor_Speed_Control_Configuration();

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
		Max_Current = *(uint16_t*)value;
        Motor_Speed_Control_Configuration();
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

void Motor_Set_Current(int16_t current_pwm)
{
	static int16_t last_speed = 0;
	if (!M_Enable)
		current_pwm = 0;
	if (abs(current_pwm) > Max_Current)
	{
		if (current_pwm < 0)
			current_pwm = -Max_Current;
		else if (current_pwm > 0)
			current_pwm = Max_Current;
	}

	if (current_pwm > 0)
	{
		if (last_speed <= 0)
		{
			TIM1->CCR4 = 0;
			delay_us(100);
			LEFT_LOW_OFF();
			delay_us(100);
			RIGHT_LOW_ON();
			delay_us(100);
			TIM1->CCR2 = current_pwm;
		}
		else
		{
			LEFT_LOW_OFF();
			RIGHT_LOW_ON();
			TIM1->CCR2 = current_pwm;
		}
	}

	else if (current_pwm < 0)
	{
		if (last_speed >= 0)
		{
			TIM1->CCR2 = 0;
			delay_us(100);
			RIGHT_LOW_OFF();
			delay_us(100);
			LEFT_LOW_ON();
			delay_us(100);
			TIM1->CCR4 = -current_pwm;
		}
		else
		{
			RIGHT_LOW_OFF();
			LEFT_LOW_ON();
			TIM1->CCR4 = -current_pwm;
		}
	}
	else
	{
		TIM1->CCR2 = 0;
		TIM1->CCR4 = 0;
		LEFT_LOW_OFF();
		RIGHT_LOW_OFF();
	}

	last_speed = current_pwm;
}

void Motor_Set_Target_Speed(int16_t target_speed)
{
    Target_Speed = target_speed;
}

void Motor_Speed_Control_Configuration(void)
{
    PID_Controller_Configuration pcc;
    //pcc.mode = PID_Controller_Mode_Incremental;
	pcc.mode = PID_Controller_Mode_Absolute;
    pcc.kp = 11;
    pcc.ki = 2;
    pcc.kd = 8;
    pcc.ko = 0;
    pcc.max_output = Max_Current;
    pcc.min_output = -Max_Current;
    pcc.max_integral = 1000;
    pcc.min_integral = -1000;
    PID_Controller_Init(&pid_motor_ctrl, &pcc);
}

void Motor_Speed_Control(int16_t current_speed)
{
	if (!M_Enable)
		return;
    int16_t output;
    int16_t real_target_speed;
    int16_t target_speed_distance = Target_Speed - Last_Real_Target_Speed;
    //Last_Real_Target_Speed = Target_Speed;

    if(Speed_Step > 0 && abs(target_speed_distance) > Speed_Step)
    {
        real_target_speed = (int16_t) (Last_Real_Target_Speed + (target_speed_distance < 0? -1 : 1) * Speed_Step);
    }
    else
        real_target_speed = Target_Speed;

    Last_Real_Target_Speed = real_target_speed;

    output = (int16_t) PID_Controller_Calc(&pid_motor_ctrl, current_speed, Target_Speed, 0, 0);
    Motor_Set_Current(output);
}
