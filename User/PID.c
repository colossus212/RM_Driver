#include "main.h"

int Kp = 0;
int Ki = 0;
int Uk = 0;
int Uk_1 = 0;
int Error_k = 0;           //ƫ��
int Error_k_1 = 0;         //��һ�ε�ƫ��

int PID_Speed(void){
	Error_k_1 = Error_k;
	Error_k = Rotate_Speed*500-Encoder_Speed;
	Uk_1 = Uk;
	Uk = Kp*(Error_k-Error_k_1) + Ki*Error_k + Uk_1;   //����ʽ��PID
	return Uk;
}
