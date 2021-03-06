#include <stdint.h>
#include <stdio.h>
#include "stm32f10x.h"
#include "delay.h"
#include "motor.h"
#include "usart2.h"
#include "main.h"
#include "debug.h"
#include "led.h"


//float Rotate_Speed = 0;
//TX数据包
char Tx_Data[] =
{ '*', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
		'0', '0', '#', '\r', '\n', 0 };
//10ms 时钟计数
uint16_t Num_10ms = 0;

void Do_Pid_Debug()
{
	int enc_spd;
	while (1)
	{
		delay_ms(100);
		//enc_spd = Encoder_Speed; //保存现场
		//Motor_velocity_control(Encoder_Speed,Target_Speed);
		if (Num_10ms > 10)
		{	                                            //  5ms*10=50ms		
/*
			Tx_Data[1] = enc_spd / 10000 + 0x30;
			Tx_Data[2] = enc_spd % 10000 / 1000 + 0x30;
			Tx_Data[3] = enc_spd % 1000 / 100 + 0x30;
			Tx_Data[4] = enc_spd % 100 / 10 + 0x30;
			Tx_Data[5] = enc_spd % 10 + 0x30;

			Tx_Data[7] = Target_Speed / 10000 + 0x30;
			Tx_Data[8] = Target_Speed % 10000 / 1000 + 0x30;
			Tx_Data[9] = Target_Speed % 1000 / 100 + 0x30;
			Tx_Data[10] = Target_Speed % 100 / 10 + 0x30;
			Tx_Data[11] = Target_Speed % 10 + 0x30;
			//Send...
			USART2_SendStr(Tx_Data);

			Num_10ms = 0;
			*/
		}
	}
}

void Do_Duty_Encoder_Test()
{
	int start_duty = 100;
	int end_duty = 4800;
	int step = 100;
	int i = 0, j = 0;
	int total_spd;
	printf("[\r\n");
	for (i = start_duty; i <= end_duty; i += step)
	{
		Motor_Set_Current(i);
		for (j = 0; j < 5; j++)
		{
			delay_ms(1000);
		}
		total_spd = 0;
		for (j = 0; j < 10; j++)
		{
			//total_spd += Encoder_Speed;
			delay_ms(300);
		}
		printf("  %d, %d;\r\n", i, total_spd / 10);
	}
	printf("]\r\n");
}

void Do_Loop_LED_Test(void)
{
	LED_GREEN_TOGGLE();
	while(1)
	{
		delay_ms(1000);
		LED_RED_TOGGLE();
		LED_GREEN_TOGGLE();
	}
}

void Do_Loop_Motor_Test(void)
{
	//电机初始化
	Motor_Init();
	Motor_Enable(1);

	while(1)
	{

		Motor_Set_Current(1000);
		LED_RED_ON();
		LED_GREEN_OFF();
		delay_ms(1000);
		Motor_Set_Current(-1000);
		LED_RED_OFF();
		LED_GREEN_ON();
		delay_ms(1000);

	}
}

void Do_Loop_Stabled_Motor_Test(void)
{
	extern int16_t Target_Speed;
	Motor_Enable(1);

	while(1)
	{
		Target_Speed = 50;
		LED_RED_ON();
		LED_GREEN_OFF();
		delay_ms(1000);
		Target_Speed = -50;
		LED_RED_OFF();
		LED_GREEN_ON();
		delay_ms(1000);
	}
}
