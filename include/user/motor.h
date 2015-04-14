#ifndef __MOTOR_H__
#define __MOTOR_H__
extern int8_t motor_enable;
extern int16_t motor_dest_speed;

extern int vP, vI, vD;
void Motor_Init(void);
void Motor_Speed(int speed);
void Motor_velocity_control(int current_speed, int want_speed);

#endif

