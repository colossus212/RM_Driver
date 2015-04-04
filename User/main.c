#include "main.h"
int Encoder_Speed = 0;

char Tx_Data[] = {'*','0','0','0','0','0','0','0','0','0','0','0','0','0','0','\r','\n',0};
int main(void)
{
		//串口初始化
	  USART2_Configuration();
	  int enc_spd;
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
		TIM2_Configuration(10);
	  TIM2_Start();

		Motor_Init();

    while(1)
    {
			 delay_ms(500);
			 Motor_Speed(2000);		 
       enc_spd = Encoder_Speed; //保存现场
			 enc_spd = abs(enc_spd); //取绝对值（abs宏使用两次被计算值，保存现场以保证前后一致）
			 Tx_Data[1] = enc_spd/10000+0x30;
			 Tx_Data[2] = enc_spd%10000/1000+0x30;
			 Tx_Data[3] = enc_spd%1000/100+0x30;
			 Tx_Data[4] = enc_spd%100/10+0x30;
			 Tx_Data[5] = enc_spd%10+0x30; 
			 
			 USART2_SendStr(Tx_Data);
			 LED_RED_TOGGLE();
    }
}
