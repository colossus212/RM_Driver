#include "main.h"
	
int Encoder_Speed = 0;
char Tx_Data[19] = {'*','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','#',0};
int main(void)
{
		//LED初始化...
		LED_Configuration();
		//PWM初始化...
	  PWM_Configuration();
	  //delay()初始化...
	  delay_init();
	  //计数初始化
	  Encoder_Configuration();
    Encoder_Start();
	  //定时器初始化
		TIM2_Configuration();
	  TIM2_Start();
	  //串口初始化
	  USART2_Configuration();
	
    while(1)
    {
			 delay_ms(50);
			 Motor_Speed(2500);		 
			 Tx_Data[1] = Encoder_Speed/10000+0x30;
			 Tx_Data[2] = Encoder_Speed%10000/1000+0x30;
			 Tx_Data[3] = Encoder_Speed%10000%1000/100+0x30;
			 Tx_Data[4] = Encoder_Speed%10000%1000%100/10+0x30;
			 Tx_Data[5] = Encoder_Speed%10000%1000%100%10+0x30; 
			 USART2_SendStr(Tx_Data);
			 
    }
}
