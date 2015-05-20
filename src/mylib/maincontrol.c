#include "main.h"
#include "can_packet.h"
#include "maincontrol.h"
#include "can1.h"
#include "motor.h"
#include <stdio.h>


void Maincontrol_Can_Receive_Handler(CanRxMsg* rx_msg)
{
	uint16_t type, id;
	uint8_t motor_mask;

    type = (uint16_t) (rx_msg->StdId & CAN_PACKET_TYPE_MASK_DATATYPE);
    id = (uint16_t) (rx_msg->StdId & CAN_PACKET_TYPE_MASK_SOURCEADDR_STRICT);

	if(rx_msg->FMI == 0) // AllInOne data
	{
		if (type == CAN_PACKET_DRIVER_DATATYPE_ENABLE)
		{
			motor_mask = 1 << CAN_OFFSET;
			Motor_Enable(
                    (uint8_t) ((rx_msg->Data[0] & motor_mask) ? 1 : 0)
			);
		}
		else if (type == CAN_PACKET_DRIVER_DATATYPE_CONTROL)
		{
			// TODO: stop when speed too small
			Target_Speed =
					MOTOR_WISE_PREFIX(((int16_t*) (rx_msg->Data))[CAN_OFFSET]);
		}
	}
	else if(rx_msg->FMI == 1) // Specific data
	{

		if (type == CAN_PACKET_DRIVER_DATATYPE_CONTROL)
		{
			Target_Speed =
					MOTOR_WISE_PREFIX(((int16_t*) (rx_msg->Data))[0]);
		}
		else if (type == CAN_PACKET_DRIVER_DATATYPE_CONFIG)
		{
			Motor_Configuration((Motor_Config_Type)(rx_msg->Data[0]), rx_msg->Data + 1);
		}
	}

}


#define PACKET_SIZE_BODY_MAX 64

uint8_t packet_total_size = 0;
int8_t packet_transmit_complete = 1;
char packet_transmit_buffer[PACKET_SIZE_BODY_MAX];

void Maincontrol_Can_Send_Handler(uint16_t id, int8_t code)
{
	/*
	uint8_t cur_size;
#if !defined STDOUT_DEBUG
	if(code == 0)printf("%u send failed\r\n", id);
#endif
	if(code == 1)
	{
		if(id >= (packet_total_size + 7 / 8))
		{
			packet_transmit_complete = 1;
		}
		else
		{
			id ++;
			cur_size = packet_total_size - id * 8;
			if(cur_size > 8) cur_size = 8;
			CAN1_AsyncTransmit(id,
					CAN_PACKET_DESTTYPE_CENTER |
					CAN_PACKET_CENTER_DATATYPE_DATA |
					CAN_PACKET_PLACETYPE_SPECIFIC |
					(id - 1),
					packet_transmit_buffer + id * 8 , cur_size);
		}
	}
	else
	{
		packet_transmit_complete = 1;
	}
	*/
}

void Maincontrol_Configuration()
{
	CAN1_Configuration(Maincontrol_Can_Send_Handler, Maincontrol_Can_Receive_Handler,
			CAN_PACKET_DESTTYPE_DRIVER | CAN_PACKET_PLACETYPE_SPECIFIC | CAN_ADDR,
			CAN_PACKET_TYPE_MASK_DESTTYPE | CAN_PACKET_TYPE_MASK_PLACETYPE | CAN_PACKET_TYPE_MASK_DESTADDR_GREEDY,
			CAN_PACKET_DESTTYPE_DRIVER | CAN_PACKET_PLACETYPE_ALLINONE | CAN_ADDR,
			CAN_PACKET_TYPE_MASK_DESTTYPE | CAN_PACKET_TYPE_MASK_PLACETYPE | CAN_PACKET_TYPE_MASK_DESTADDR_GREEDY);
}

/*
int Maincontrol_Transmit(int8_t type, char* data, uint8_t size)
{

	char data_start[2];
	if(!packet_transmit_complete) return -1;
	if(size > PACKET_SIZE_BODY_MAX) return -2;
	packet_total_size = size;

	data_start[0] = type;
	data_start[1] = size;
	for(;size > 0; size--)
	{
		packet_transmit_buffer[size - 1] = data[size - 1];
	}

	packet_transmit_complete = 0;

	CAN1_AsyncTransmit(0,
			CAN_PACKET_DESTTYPE_CENTER |
			CAN_PACKET_CENTER_DATATYPE_DATA |
			CAN_PACKET_PLACETYPE_SPECIFIC,
			data_start, 2);

	return packet_total_size;
}
*/
