#include <stdio.h>
#include "stm32f10x.h"
#include "usart2.h"
#include "main.h"
#include "stm32f10x_it.h"


void USART2_Configuration(void)
{
	USART_InitTypeDef usart;
	GPIO_InitTypeDef gpio;
	NVIC_InitTypeDef nvic;
	DMA_InitTypeDef dma;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	gpio.GPIO_Pin = GPIO_Pin_3;
	gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio);

	gpio.GPIO_Pin = GPIO_Pin_2;
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio);

	USART_DeInit(USART2);
	usart.USART_BaudRate = 115200;
	usart.USART_WordLength = USART_WordLength_8b;
	usart.USART_StopBits = USART_StopBits_1;
	usart.USART_Parity = USART_Parity_No;
	usart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART2, &usart);


	nvic.NVIC_IRQChannel = USART2_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = ITP_USART2_GLOBAL_PREEMPTION;
	nvic.NVIC_IRQChannelSubPriority = ITP_USART2_GLOBAL_SUB;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	// DMA 接收
	dma.DMA_PeripheralBaseAddr = (uint32_t) &(USART2->DR);
	dma.DMA_MemoryBaseAddr = 0;
	dma.DMA_DIR = DMA_DIR_PeripheralSRC;
	dma.DMA_BufferSize = 0;
	dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
	dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	dma.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
	dma.DMA_Mode = DMA_Mode_Circular;
	dma.DMA_Priority = DMA_Priority_High;
	dma.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel6, &dma);
	// DMA 接收中断
	nvic.NVIC_IRQChannel = DMA1_Channel6_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = ITP_USART2_DMA_RX_PREEMPTION;
	nvic.NVIC_IRQChannelSubPriority = ITP_USART2_DMA_RX_SUB;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
	DMA_ITConfig(DMA1_Channel6, DMA_IT_TC | DMA_IT_TE, ENABLE);

	// DMA 发送
	dma.DMA_PeripheralBaseAddr = (uint32_t) &(USART2->DR);
	dma.DMA_MemoryBaseAddr = 0;
	dma.DMA_DIR = DMA_DIR_PeripheralDST;
	dma.DMA_BufferSize = 0;
	dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
	dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	dma.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
	dma.DMA_Mode = DMA_Mode_Normal;
	dma.DMA_Priority = DMA_Priority_VeryHigh;
	dma.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel7, &dma);
	// DMA 发送中断
	nvic.NVIC_IRQChannel = DMA1_Channel7_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = ITP_USART2_DMA_TX_PREEMPTION;
	nvic.NVIC_IRQChannelSubPriority = ITP_USART2_DMA_TX_SUB;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
	DMA_ITConfig(DMA1_Channel7, DMA_IT_TC | DMA_IT_TE, ENABLE);

	//USART_DMACmd(USART2, USART_DMAReq_Rx | USART_DMAReq_Tx, ENABLE);
	USART_Cmd(USART2, ENABLE);
}

int USART2_DMA_SendData(char *data, uint16_t size)
{
	if(DMA1_Channel7->CCR & DMA_CCR1_EN)
	{
		return -1;
	}

	DMA_ClearFlag(DMA1_FLAG_GL7);

	DMA_SetCurrDataCounter(DMA1_Channel7, size);
	DMA1_Channel7->CMAR = (uint32_t)data;

	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
	DMA_Cmd(DMA1_Channel7, ENABLE);

	return size;
}
// DMA TX
void DMA1_Channel7_IRQHandler()
{

	DMA_ClearITPendingBit(DMA1_IT_GL7);

	USART_DMACmd(USART2, USART_DMAReq_Tx, DISABLE);
	DMA_Cmd(DMA1_Channel7, DISABLE);

}

void (*cb_func)(int8_t);

int USART2_DMA_ReceiveData(char* data, uint16_t size, void (*callback_func)(int8_t))
{
	if(DMA1_Channel6->CCR & DMA_CCR1_EN)
	{
		return -1;
	}

	DMA_ClearFlag(DMA1_FLAG_GL6);

	DMA_SetCurrDataCounter(DMA1_Channel6, size);
	DMA1_Channel6->CMAR = (uint32_t)data;
	cb_func = callback_func;
	USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);

	USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
	DMA_Cmd(DMA1_Channel6, ENABLE);

	return size;
}

// DMA RX
void DMA1_Channel6_IRQHandler()
{
	int result = 0;
	DMA_ClearITPendingBit(DMA1_IT_GL6);

	if (DMA_GetITStatus(DMA1_IT_TC6) == SET)
	{
		result = 1;
	}

	cb_func(result);

	USART_DMACmd(USART2, USART_DMAReq_Rx, DISABLE);
	DMA_Cmd(DMA1_Channel6, DISABLE);

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
}

void USART2_SendChar(char b)
{
	while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
		;
	USART_SendData(USART2, b);
}

#if defined STDOUT_USART2

#if defined ( __CC_ARM )
int fputc(int ch, FILE *f)
{
	while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
		;
	USART_SendData(USART2, (uint8_t) ch);
	return ch;
}
#elif defined ( __GNUC__ )
int _write(int file, char* ptr, int len)
{
	int i = 0;
	for (i = 0; i< len; i++)
	{
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART2, ptr[i]);
	}
	return i;
}
#endif

#endif

void USART2_SendStr(char *str)
{                        //发送字符串
	int Num = 0;
	while (str[Num] != 0)
	{
		USART2_SendChar(str[Num]);
		Num++;
	}
}

void USART2_IRQHandler(void)
{
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);

	}
}

