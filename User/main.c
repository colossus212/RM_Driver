#include "main.h"

//��ǰ����������ֵ ../10ms
int Encoder_Speed = 0;
//����ת�� r/10ms
int Target_Speed = 0; 



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

    //Motor_Speed(2000);
    while(1)
    {
			 delay_ms(100);
       Target_Speed = dest_speed;
       //Motor_Speed(dest_speed);
			 enc_spd = Encoder_Speed; //�����ֳ�

			 printf("%d\r\n", enc_spd);

			 LED_RED_TOGGLE();
    }
}
