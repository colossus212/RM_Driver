#include "main.h"

//��ǰ����������ֵ ../10ms
int Encoder_Speed = 0;
//����ת�� r/10ms
float Rotate_Speed = 0;
//TX���ݰ�
char Tx_Data[] = {'*','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','#','\r','\n',0};
//10ms ʱ�Ӽ���
unsigned int Num_10ms = 0;
	
int main(void)
{
	  int enc_spd;
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
		TIM2_Configuration(10);
	  TIM2_Start();
    //���ڳ�ʼ��
	  USART2_Configuration();
	  //�����ʼ��
		Motor_Init();

    while(1)
    {
			 //delay_ms(500);
			 Motor_Speed(2000);	
       if(Num_10ms>5){	                                            //  5ms*10=50ms		
				 enc_spd = Encoder_Speed; //�����ֳ�
				 enc_spd = abs(enc_spd); //ȡ����ֵ��abs��ʹ�����α�����ֵ�������ֳ��Ա�֤ǰ��һ�£�
				 Tx_Data[1] = enc_spd/10000+0x30;
				 Tx_Data[2] = enc_spd%10000/1000+0x30;
				 Tx_Data[3] = enc_spd%1000/100+0x30;
				 Tx_Data[4] = enc_spd%100/10+0x30;
				 Tx_Data[5] = enc_spd%10+0x30; 
				 //Send...
				 USART2_SendStr(Tx_Data);
				 Num_10ms = 0;
			 }
			//LED_RED_TOGGLE();
    }
}
