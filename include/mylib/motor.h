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
void Motor_Set_Current(int16_t current_pwm);
void Motor_Set_Target_Speed(int16_t target_speed);
void Motor_Speed_Control_Configuration(void);
void Motor_Speed_Control(int16_t current_speed);

#endif

