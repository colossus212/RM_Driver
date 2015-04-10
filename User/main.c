#include "main.h"

//当前编码器计数值 ../10ms
int Encoder_Speed = 0;
//给定转速 r/10ms
int Target_Speed = 0; 



int16_t dest_speed=0;


int main(void)
{
	  
	  int enc_spd;
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

    //Motor_Speed(2000);
    while(1)
    {
			 delay_ms(100);
       Target_Speed = dest_speed;
       //Motor_Speed(dest_speed);
			 enc_spd = Encoder_Speed; //保存现场

			 printf("%d\r\n", enc_spd);

			 LED_RED_TOGGLE();
    }
}
