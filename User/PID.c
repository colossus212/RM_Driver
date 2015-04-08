#include "main.h"

int Kp = 1;
int Ki = 1;
int Uk = 0;
int Uk_1 = 0;
int Error_k = 0;           //偏差
int Error_k_1 = 0;         //上一次的偏差

int PID_Speed(void){
	
	Error_k_1 = Error_k;
	Error_k = Target_Speed-Encoder_Speed;
	Uk_1 = Uk;
	Uk = Kp*(Error_k-Error_k_1) + Ki*Error_k + Uk_1;   //增量式的PID
	if(Uk>4500){
		Uk = 4500;
	}
	if(Uk<1000&&Uk>0){
		Uk = 1000;
	}
	if(Uk<-4500){
		Uk = -4500;
	}
	if(Uk>-1000&&Uk<0){
		Uk = -1000;
	}
	return Uk;
}
