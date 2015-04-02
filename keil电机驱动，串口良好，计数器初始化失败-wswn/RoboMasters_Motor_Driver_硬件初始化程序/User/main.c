#include "main.h"
	
int Encoder_Speed = 0;
char Tx_Data[19] = {'*','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','#',0};
int main(void)
{
		//LED��ʼ��...
		LED_Configuration();
		//PWM��ʼ��...
	  PWM_Configuration();
	  //delay()��ʼ��...
	  delay_init();
	  //������ʼ��
	  Encoder_Configuration();
    Encoder_Start();
	  //��ʱ����ʼ��
		TIM2_Configuration();
	  TIM2_Start();
	  //���ڳ�ʼ��
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
