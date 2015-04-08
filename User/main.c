#include "main.h"

//当前编码器计数值 ../10ms
int Encoder_Speed = 0;
//给定转速 r/10ms
int Target_Speed = -800; 
float Rotate_Speed = 0;
//TX数据包
char Tx_Data[] = {'*','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','#','\r','\n',0};
//10ms 时钟计数
unsigned int Num_10ms = 0;

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
	
    while(1)
    {
			 delay_ms(10);
			 //Motor_velocity_control(Encoder_Speed,Target_Speed);
			 if(Num_10ms>10){	                                            //  5ms*10=50ms		
				 enc_spd = Encoder_Speed; //保存现场
				 enc_spd = abs(enc_spd); //取绝对值（abs宏使用两次被计算值，保存现场以保证前后一致）
				 
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
