#ifndef __MOTOR_H
#define __MOTOR_H

extern int vP,vI,vD;
void Motor_Init(void);
void Motor_Speed(int speed);
void Motor_velocity_control(int current_speed,int want_speed);
 
#endif

