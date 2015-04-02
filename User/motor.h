#ifndef __MOTOR_H
#define __MOTOR_H

extern float vP,vI,vD;
void Motor_Init(void);
void Motor_Speed(int speed);
void Motor_velocity_control(float current_speed,float want_speed);
 
#endif

