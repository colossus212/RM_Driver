/*
 * maincontrol.h
 *
 *  Created on: Apr 23, 2015
 *      Author: texot
 */

#ifndef _MAINCONTROL_H_
#define _MAINCONTROL_H_

#include "main.h"

#if defined CAR_1
// For right wheel, the prefix should be '-' considered the opposite
// direction of motor from left side
#if defined DRIVER_LEFT_FRONT

#define CAN_OFFSET 0
#define MOTOR_WISE_PREFIX +

#elif defined DRIVER_RIGHT_FRONT

#define CAN_OFFSET 1
#define MOTOR_WISE_PREFIX -

#elif defined DRIVER_LEFT_END

#define CAN_OFFSET 2
#define MOTOR_WISE_PREFIX - // Left end motor is an exception due to the motor problem(?)

#elif defined DRIVER_RIGHT_END

#define CAN_OFFSET 3
#define MOTOR_WISE_PREFIX -

#endif

#elif defined CAR_2

#if defined DRIVER_LEFT_FRONT

#define CAN_OFFSET 0
#define MOTOR_WISE_PREFIX +

#elif defined DRIVER_RIGHT_FRONT

#define CAN_OFFSET 1
#define MOTOR_WISE_PREFIX -

#elif defined DRIVER_LEFT_END

#define CAN_OFFSET 2
#define MOTOR_WISE_PREFIX +

#elif defined DRIVER_RIGHT_END

#define CAN_OFFSET 3
#define MOTOR_WISE_PREFIX -

#endif

#endif


#define CAN_ADDR (0x1 << CAN_OFFSET)

/* aaa bbbc dddd

 * aaa:  目标
 * bbb:  数据包类型
 * c:    数据放置类型
 * dddd: 目标地址
 */
#define CAN_PACKET_TYPE_MASK_DESTTYPE 0x700				/* 111 0000	0000 */
#define CAN_PACKET_TYPE_MASK_DATATYPE 0x0e0				/* 000 1110	0000 */
#define CAN_PACKET_TYPE_MASK_PLACETYPE 0x010			/* 000 0001	0000 */
#define CAN_PACKET_TYPE_MASK_DESTADDR_GREEDY (CAN_ADDR & 0xf)	/* 000 0000	xxxx */
#define CAN_PACKET_TYPE_MASK_DESTADDR_STRICT 0xf
#define CAN_PACKET_TYPE_MASK_DESTADDR_ALL 0


#define CAN_PACKET_DESTTYPE_DRIVER \
	(0x0 << 8)									/* 000 xxxx xxxx */
#define CAN_PACKET_DESTTYPE_CENTER \
	(0x1 << 8)									/* 001 xxxx xxxx */


#define CAN_PACKET_DRIVER_DATATYPE_ENABLE \
	(0x0 << 5)									/* xxx 000x xxxx */
#define CAN_PACKET_DRIVER_DATATYPE_CONFIG \
	(0x1 << 5)									/* xxx 001x xxxx */
#define CAN_PACKET_DRIVER_DATATYPE_CONTROL \
	(0x2 << 5)									/* xxx 002x xxxx */


#define CAN_PACKET_CENTER_DATATYPE_PRINTF \
	(0x0 << 5)									/* xxx 000x xxxx */
#define CAN_PACKET_CENTER_DATATYPE_STATUS \
	(0x1 << 5)									/* xxx 000x xxxx */


#define CAN_PACKET_PLACETYPE_ALLINONE \
	(0x0 << 4)									/* xxx xxx0 xxxx */
#define CAN_PACKET_PLACETYPE_SPECIFIC \
	(0x1 << 4)									/* xxx xxx1 xxxx */

void Maincontrol_Configuration(void);


#endif /* _MAINCONTROL_H_ */
