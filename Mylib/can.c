#include "main.h"

//HVD232---CAN_TX---PB9
//HVD232---CAN_RX---PB8

// must meet bitsizeof(CAN_ADDR) <= 3 && bitsizeof(CAN_CONTROL_IDENTIFIER) <= 8


//#define DRIVER_LEFT_FRONT
//#define DRIVER_RIGHT_FRONT
#define DRIVER_LEFT_END
//#define DRIVER_RIGHT_END

#define CAN_CONTROL_IDENTIFIER 0x66
#ifdef DRIVER_LEFT_FRONT
  #define CAN_ADDR 0x1
#elif defined DRIVER_RIGHT_FRONT
  #define CAN_ADDR 0x2
#elif defined DRIVER_LEFT_END
  #define CAN_ADDR 0x3
#elif defined DRIVER_RIGHT_END
  #define CAN_ADDR 0x4
#endif

void CAN_Configuration(void)
{
		CAN_InitTypeDef        can;
		CAN_FilterInitTypeDef  can_filter;
		GPIO_InitTypeDef 	   	 gpio;
		NVIC_InitTypeDef   	   nvic;

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,  ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB |
													 RCC_APB2Periph_AFIO , ENABLE);

		GPIO_PinRemapConfig(GPIO_Remap1_CAN1,ENABLE);

		gpio.GPIO_Pin = GPIO_Pin_8;
		gpio.GPIO_Mode = GPIO_Mode_IPU;
		gpio.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &gpio);	//CAN_RX

		gpio.GPIO_Pin = GPIO_Pin_9;	   
		gpio.GPIO_Mode = GPIO_Mode_AF_PP;
		gpio.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &gpio);    //CAN_TX

		nvic.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;
		nvic.NVIC_IRQChannelPreemptionPriority = 1;
		nvic.NVIC_IRQChannelSubPriority = 0;
		nvic.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&nvic);

		nvic.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;  
		nvic.NVIC_IRQChannelPreemptionPriority = 1;
		nvic.NVIC_IRQChannelSubPriority = 1;
		nvic.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&nvic);

		CAN_DeInit(CAN1);

		can.CAN_TTCM = DISABLE;
		can.CAN_ABOM = DISABLE;
		can.CAN_AWUM = DISABLE;
		can.CAN_NART = DISABLE;	  
		can.CAN_RFLM = DISABLE;																
		can.CAN_TXFP = ENABLE;		
		can.CAN_Mode = CAN_Mode_Normal;
		
		// TODO: Set Propriate Value
		can.CAN_SJW = CAN_SJW_1tq;
		can.CAN_BS1 = CAN_BS1_2tq;
		can.CAN_BS2 = CAN_BS2_2tq;
		// TODO: Set Properiate Prescaler

		can.CAN_Prescaler = 5;     //CAN BaudRate 25/(1+2+2)/5=1Mbps
		CAN_Init(CAN1, &can);

		can_filter.CAN_FilterNumber = 0;
		//can_filter.CAN_FilterMode = CAN_FilterMode_IdMask;
		can_filter.CAN_FilterMode = CAN_FilterMode_IdList;
		can_filter.CAN_FilterScale = CAN_FilterScale_16bit;
		/* 
		   16bit structure
		   [15:5] = Stdid[10:0]
			 [4]    = RTR (Remote transmission request) 0=Data, 1=Remote
			 [3]    = IDE (Identifier extension bit) 0=11bit, 1=29bit
			 [2:0]  = Extid[17:15]
			 
			 32bit structure
		   [31:21] = Stdid[10:0]
			 [20:3]  = Extid[17:0]
			 [2]     = IDE (Identifier extension bit) 0=11bit, 1=29bit
			 [1]     = RTR (Remote transmission request) 0=Data, 1=Remote
       [0]     = Reserved
		*/
		can_filter.CAN_FilterIdHigh = 0;
		can_filter.CAN_FilterIdLow = 0xffe0 & ((CAN_CONTROL_IDENTIFIER << 8) | (CAN_ADDR << 5)); // 1111 1111 1110 0000
		//can_filter.CAN_FilterMaskIdHigh = 0xfff8; // 1111 1111 1111 1000
		// IdListMode
		can_filter.CAN_FilterMaskIdHigh = 0;
		can_filter.CAN_FilterMaskIdLow = 0;
		can_filter.CAN_FilterFIFOAssignment = 0;
		can_filter.CAN_FilterActivation = ENABLE;
		CAN_FilterInit(&can_filter);

		CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);
		CAN_ITConfig(CAN1,CAN_IT_TME,ENABLE);
}

void USB_LP_CAN1_RX0_IRQHandler(void)
{
	  int16_t temp_speed;
		CanRxMsg rx_message;
		extern int16_t dest_speed;
		if (CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET) 
		{
				CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
				CAN_Receive(CAN1, CAN_FIFO0, &rx_message);
			  
			  temp_speed = rx_message.Data[1] << 8 | rx_message.Data[0];
			 #if (defined DRIVER_RIGHT_FRONT) || (defined DRIVER_RIGHT_END) || (defined DRIVER_LEFT_END)
			 dest_speed=-temp_speed;
			 #else
			 dest_speed=temp_speed;
			 #endif
		}
}

void USB_HP_CAN1_TX_IRQHandler(void)
{
		if (CAN_GetITStatus(CAN1,CAN_IT_TME)!= RESET) 
		{
				CAN_ClearITPendingBit(CAN1,CAN_IT_TME);

		}
}
