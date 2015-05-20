#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdint.h>

//variables...
extern int16_t Encoder_Speed;
extern int16_t Target_Speed;

//#define CAR_1
#define CAR_2

//#define DRIVER_LEFT_FRONT
//#define DRIVER_RIGHT_FRONT
#define DRIVER_LEFT_END
//#define DRIVER_RIGHT_END


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
#define CAN_PACKET_TYPE_MASK_DESTADDR_GREEDY (CAN_ADDR & 0xf)	/* 000 0000	xxxx */


#endif
