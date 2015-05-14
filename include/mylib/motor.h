#ifndef __MOTOR_H__
#define __MOTOR_H__

typedef enum {
	MCT_Max_Speed = 0,
	MCT_PID = 1,
	MCT_Speed_Step = 2
} Motor_Config_Type;

void Motor_Enable(uint8_t men);
void Motor_Configuration(Motor_Config_Type mct, void* value);

void Motor_Init(void);
void Motor_Speed(int32_t speed);
int32_t Motor_velocity_control(int current_speed, int want_speed);

#endif

