#ifndef __MAIN_H__
#define __MAIN_H__



#include "stm32f10x.h"
#include <stdio.h>
#include <stdlib.h>

#include "encoder.h"
#include "led.h"
#include "pwm.h"
#include "tim2.h"
#include "delay.h"
#include "usart2.h"
#include "can.h"
#include "ad.h"
#include "flash.h"
#include "key.h"
#include "motor.h"   //转速调控
#include "PID.h"
#include "debug.h"

// 注意：abs的x参量不要使用执行过程中可能发生改变的参量
#define abs(x) ((x)>0? (x):(-(x)))

//variables...
extern int Encoder_Speed;
extern unsigned int Num_10ms;
extern float Rotate_Speed;
extern int Target_Speed;

#endif
