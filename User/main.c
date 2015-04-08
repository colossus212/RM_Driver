#include "main.h"

//��ǰ����������ֵ ../10ms
int Encoder_Speed = 0;
//����ת�� r/10ms
int Target_Speed = -800; 
float Rotate_Speed = 0;
//TX���ݰ�
char Tx_Data[] = {'*','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','#','\r','\n',0};
//10ms ʱ�Ӽ���
unsigned int Num_10ms = 0;

int16_t dest_speed=0;
int main(void)
{
	  
	  int enc_spd;
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
	
    while(1)
    {
			 delay_ms(10);
			 //Motor_velocity_control(Encoder_Speed,Target_Speed);
			 if(Num_10ms>10){	                                            //  5ms*10=50ms		
				 enc_spd = Encoder_Speed; //�����ֳ�
				 enc_spd = abs(enc_spd); //ȡ����ֵ��abs��ʹ�����α�����ֵ�������ֳ��Ա�֤ǰ��һ�£�
				 
				 Tx_Data[1] = enc_spd/10000+0x30;
				 Tx_Data[2] = enc_spd%10000/1000+0x30;
				 Tx_Data[3] = enc_spd%1000/100+0x30;
				 Tx_Data[4] = enc_spd%100/10+0x30;
				 Tx_Data[5] = enc_spd%10+0x30;
				 
				 Tx_Data[7] = Target_Speed/10000+0x30;
				 Tx_Data[8] = Target_Speed%10000/1000+0x30;
				 Tx_Data[9] = Target_Speed%1000/100+0x30;
				 Tx_Data[10] = Target_Speed%100/10+0x30;
				 Tx_Data[11] = Target_Speed%10+0x30; 
				 //Send...
				 USART2_SendStr(Tx_Data);
				 Num_10ms = 0;
			 }
			//LED_RED_TOGGLE();
    }
}
