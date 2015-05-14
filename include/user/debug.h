#ifndef __DEBUG_H__
#define __DEBUG_H__

#define DISABLE_FOLLOWING_CODE(x) while(x)

extern uint16_t Num_10ms;
void Do_Pid_Debug(void);
void Do_Duty_Encoder_Test(void);
void Do_Loop_LED_Test(void);
void Do_Loop_Motor_Test(void);

#endif
