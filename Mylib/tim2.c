#include "main.h"

void TIM2_Configuration(int32_t sample_interval_ms)   //sample_interval_ms ¶¨Ê±
{
    TIM_TimeBaseInitTypeDef  tim;
    NVIC_InitTypeDef         nvic;
	  RCC_ClocksTypeDef        RCC_ClocksStatus;
	  uint32_t								 TIM2_Clock;
	  int32_t									 TIM2_Clock_MHZ;
	  int32_t									 Period;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);

    nvic.NVIC_IRQChannel = TIM2_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 1;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
	  
	  
		RCC_GetClocksFreq(&RCC_ClocksStatus);
	  TIM2_Clock = RCC_ClocksStatus.PCLK1_Frequency;
	  TIM2_Clock_MHZ = TIM2_Clock / 1000000;
	  Period = sample_interval_ms * TIM2_Clock / (TIM2_Clock_MHZ * 1000);
	
	  printf("%d, %d\r\n", TIM2_Clock_MHZ, Period);
    tim.TIM_Prescaler = TIM2_Clock_MHZ - 1;
    tim.TIM_CounterMode = TIM_CounterMode_Up;
    tim.TIM_ClockDivision = TIM_CKD_DIV1;
    tim.TIM_Period = Period-1;                     

    TIM_TimeBaseInit(TIM2,&tim);
}

void TIM2_Start(void)
{
    TIM_Cmd(TIM2, ENABLE);	 
    TIM_ITConfig(TIM2, TIM_IT_Update,ENABLE);
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);	

}

void TIM2_IRQHandler(void)
{
	  
    if (TIM_GetITStatus(TIM2,TIM_IT_Update)!= RESET) 
	  {
		    Encoder_Speed = Encoder_Get_CNT();
			  Num_10ms++;
			  Motor_velocity_control(Encoder_Speed,Target_Speed);
			  LED_GREEN_TOGGLE();
        TIM_ClearFlag(TIM2, TIM_FLAG_Update);	
    }

}
