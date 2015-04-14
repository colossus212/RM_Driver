#include "stm32f10x.h"
#include "main.h"
#include "motor.h"

//HVD232---CAN_TX---PB9
//HVD232---CAN_RX---PB8

// 数据包类型
#define CAN_ENABLE_TYPE 0x65 // 使能电机
#define CAN_CONTROL_TYPE 0x66 // 底盘主控控制四个电机
#define CAN_DATA_TYPE 0x67 // 驱动数据返回

// For right wheel, the prefix should be '-' considered the opposite
// direction of motor from left side
#if defined DRIVER_LEFT_FRONT
#define CAN_ADDR 0x1
#define CAN_CONTROL_DATA_OFFSET 0
#define MOTOR_WISE_PREFIX +
#elif defined DRIVER_RIGHT_FRONT
#define CAN_ADDR 0x2
#define CAN_CONTROL_DATA_OFFSET 1
#define MOTOR_WISE_PREFIX -
#elif defined DRIVER_LEFT_END
#define CAN_ADDR 0x3
#define CAN_CONTROL_DATA_OFFSET 2
#define MOTOR_WISE_PREFIX - // Left end motor is an exception due to the motor problem(?)
#elif defined DRIVER_RIGHT_END
#define CAN_ADDR 0x4
#define CAN_CONTROL_DATA_OFFSET 3
#define MOTOR_WISE_PREFIX -
#endif

void CAN_Configuration(void)
{
	CAN_InitTypeDef can;
	CAN_FilterInitTypeDef can_filter;
	GPIO_InitTypeDef gpio;
	NVIC_InitTypeDef nvic;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB |
	RCC_APB2Periph_AFIO, ENABLE);

	GPIO_PinRemapConfig(GPIO_Remap1_CAN1, ENABLE);

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
	// TODO: Review All Interrupt Priority Settings
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
	can_filter.CAN_FilterMode = CAN_FilterMode_IdMask; // CAN_FilterMode_IdList; 
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
	can_filter.CAN_FilterIdHigh = ((0xff & CAN_CONTROL_TYPE) << 8)
			| ((0x7 & CAN_ADDR) << 5); // 1111 1111 1110 0000
	can_filter.CAN_FilterIdLow = ((0xff & CAN_ENABLE_TYPE) << 8)
			| ((0x7 & CAN_ADDR) << 5);
	can_filter.CAN_FilterMaskIdHigh = 0xff00; // 全局性数据包模式
	can_filter.CAN_FilterMaskIdLow = 0xff00;
	can_filter.CAN_FilterFIFOAssignment = 0;
	can_filter.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&can_filter);

	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
	CAN_ITConfig(CAN1, CAN_IT_TME, ENABLE);
}

void USB_LP_CAN1_RX0_IRQHandler(void)
{
	CanRxMsg rx_message;

	int16_t type;
	if (CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET)
	{
		CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
		CAN_Receive(CAN1, CAN_FIFO0, &rx_message);
		type = rx_message.StdId >> 3;
		// 全局性数据包
		if (type == CAN_ENABLE_TYPE)
		{
			motor_enable =
					(int8_t) (((int16_t*) (rx_message.Data))[CAN_CONTROL_DATA_OFFSET]);
		}
		else if (type == CAN_CONTROL_TYPE)
		{
			motor_dest_speed =
					MOTOR_WISE_PREFIX(((int16_t*) (rx_message.Data))[CAN_CONTROL_DATA_OFFSET]);
		}

	}
}

void USB_HP_CAN1_TX_IRQHandler(void)
{
	if (CAN_GetITStatus(CAN1, CAN_IT_TME) != RESET)
	{
		CAN_ClearITPendingBit(CAN1, CAN_IT_TME);

	}
}
