#include "stm32f10x.h"
#include <stdio.h>
#include "main.h"
#include "motor.h"
#include "stm32f10x_it.h"
#include "led.h"
#include "can1.h"
#include "can_packet_queue.h"

//HVD232---CAN_TX---PB9
//HVD232---CAN_RX---PB8

void (*rh)(CanRxMsg*);
void (*sh)(uint16_t, int8_t);

#define PACKET_QUEUE_SIZE 10

uint16_t mailbox0_id=0, mailbox1_id=0, mailbox2_id=0;

CAN_Packet_Queue pq[PACKET_QUEUE_SIZE];
uint8_t pq_start = 0;
uint8_t pq_end = 0;
int8_t pq_full = 0;

uint32_t async_transmit_times = 0;
uint32_t queue_full_times = 0;


int8_t mutex_transmit = 0;
int8_t mutex_queue = 0;


void CAN1_Configuration(void (*send_handler)(uint16_t, int8_t), void (*receive_handler)(CanRxMsg*), uint16_t IdHigh, uint16_t IdHighMask, uint16_t IdLow, uint16_t IdLowMask)
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

	rh = receive_handler;
	sh = send_handler;
	nvic.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = ITP_CAN1_TX_PREEMPTION;
	nvic.NVIC_IRQChannelSubPriority = ITP_CAN1_TX_SUB;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);

	nvic.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = ITP_CAN1_RX_PREEMPTION;
	nvic.NVIC_IRQChannelSubPriority = ITP_CAN1_RX_SUB;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);

	CAN_DeInit(CAN1);
	CAN_StructInit(&can);

	can.CAN_TTCM = DISABLE;
	can.CAN_ABOM = DISABLE;
	can.CAN_AWUM = DISABLE;
	can.CAN_NART = DISABLE;
	can.CAN_RFLM = DISABLE;
	can.CAN_TXFP = ENABLE;
	can.CAN_Mode = CAN_Mode_Normal;

#if defined HSE_25MHZ
	// TODO: Set Propriate Value
	can.CAN_SJW = CAN_SJW_1tq;
	can.CAN_BS1 = CAN_BS1_2tq;
	can.CAN_BS2 = CAN_BS2_2tq;
	// TODO: Set Properiate Prescaler

	can.CAN_Prescaler = 5;     //CAN BaudRate 25/(1+2+2)/5=1Mbps
#else
	// TODO: Set Propriate Value
	can.CAN_SJW = CAN_SJW_1tq;
	can.CAN_BS1 = CAN_BS1_8tq;
	can.CAN_BS2 = CAN_BS2_3tq;
	// TODO: Set Properiate Prescaler

	can.CAN_Prescaler = 3;     //CAN BaudRate 36/(1+8+3)/3=1Mbps
#endif

	CAN_Init(CAN1, &can);

	can_filter.CAN_FilterNumber = 0;
	can_filter.CAN_FilterMode = CAN_FilterMode_IdMask;
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
	can_filter.CAN_FilterIdHigh = IdHigh << 5;
	can_filter.CAN_FilterIdLow = IdLow << 5;
	can_filter.CAN_FilterMaskIdHigh = IdHighMask << 5;
	can_filter.CAN_FilterMaskIdLow = IdLowMask << 5;
	can_filter.CAN_FilterFIFOAssignment = 0;
	can_filter.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&can_filter);

	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
	CAN_ITConfig(CAN1, CAN_IT_TME, ENABLE);
}


int8_t CAN1_Transmit(uint16_t id, uint16_t addr, char* data, uint8_t size)
{
	CanTxMsg Tx_message;
	uint8_t used_mailbox;
	uint8_t i;
    int8_t ret;

	Tx_message.StdId = addr;
	Tx_message.IDE = CAN_Id_Standard;
	Tx_message.RTR = CAN_RTR_Data;
	Tx_message.DLC = size;
	for (i = 0; i < size; i++)
		Tx_message.Data[i] = data[i] & 0xff;

    if(mutex_transmit == 0) {
        mutex_transmit = 1;
        used_mailbox = CAN_Transmit(CAN1, &Tx_message);
        if (used_mailbox != CAN_TxStatus_NoMailBox) {
            switch (used_mailbox) {
                case 0:
                    mailbox0_id = id;
                    break;
                case 1:
                    mailbox1_id = id;
                    break;
                case 2:
                    mailbox2_id = id;
                    break;
            }
            ret = 1;
        }
        else
        {
            if (sh)
                sh(id, 0);
            printf("[CAN1_Transmit] No mailbox\r\n");
            ret = -1;
        }
        mutex_transmit = 0;
    }
    else
        ret = -1;
	return ret;
}


void USB_HP_CAN1_TX_IRQHandler(void)
{
	uint16_t id;
	int8_t code;
	if (CAN_GetITStatus(CAN1, CAN_IT_TME) != RESET)
	{
		if(sh)
		{
			if(CAN1->TSR & (CAN_TSR_TME0 | CAN_TSR_TME1))
			{
				if(CAN1->TSR & CAN_TSR_TME0)
				{
					id = mailbox0_id;
					code = ((CAN1->TSR & CAN_TSR_TXOK0) == CAN_TSR_TXOK0);
				}
				else
				{
					id = mailbox1_id;
					code = ((CAN1->TSR & CAN_TSR_TXOK1) == CAN_TSR_TXOK1);
				}
			}
			else
			{
				id = mailbox2_id;
				code = ((CAN1->TSR & CAN_TSR_TXOK2) == CAN_TSR_TXOK2);
			}
		}
		CAN_ClearITPendingBit(CAN1, CAN_IT_TME);

		if(sh)
			sh(id, code);

		if(mutex_transmit == 0 && mutex_queue == 0 && (pq_full == 1 || pq_end != pq_start))
		{
            mutex_queue = 1;
			CAN1_Transmit(pq[pq_start].id, pq[pq_start].addr, pq[pq_start].data, pq[pq_start].size);
			pq_full = 0;
			pq_start = (pq_start + 1) % PACKET_QUEUE_SIZE;
            mutex_queue = 0;
		}
	}
}

void USB_LP_CAN1_RX0_IRQHandler(void)
{
	CanRxMsg rx_message;

	if (CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET)
	{
		CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
		CAN_Receive(CAN1, CAN_FIFO0, &rx_message);
		if(rh)
			rh(&rx_message);
	}


}

int8_t CAN1_AsyncTransmit(uint16_t id, uint16_t addr, char* data, uint8_t size)
{
	int8_t ret;
	if((CAN1->TSR & (CAN_TSR_TME0 | CAN_TSR_TME1 | CAN_TSR_TME2))
        && (CAN1_Transmit(id, addr, data, size) == 1))
	{
		return 1;
	}

    if(mutex_queue == 0) {
        mutex_queue = 1;
        async_transmit_times++;
        if (pq_full == 0) {
            pq[pq_end].id = id;
            pq[pq_end].addr = addr;
            pq[pq_end].size = size;
            pq[pq_end].data[0] = data[0];
            pq[pq_end].data[1] = data[1];
            pq[pq_end].data[2] = data[2];
            pq[pq_end].data[3] = data[3];
            pq[pq_end].data[4] = data[4];
            pq[pq_end].data[5] = data[5];
            pq[pq_end].data[6] = data[6];
            pq[pq_end].data[7] = data[7];
            pq_end = (pq_end + 1) % PACKET_QUEUE_SIZE;
            if (pq_start == pq_end) pq_full = 1;
            ret = 0;
        }
        else {
            queue_full_times++;
            if (sh)
                sh(id, 0);
            printf("[CAN1_AsyncTransmit] Queue Full\r\n");
            ret = -1;
        }
        mutex_queue = 0;
    }
    else
        ret = -1;
	return ret;

}
