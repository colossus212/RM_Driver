
#include "main.h"

int main(void)
{
		int32_t i = 0;
	  //SystemInit();
		LED_Configuration();
		PWM_Configuration();
	  Motor_Init();
		LED_RED_ON();

		for(i = 4900; i >= -4900; i-=100)
	  {
			  Motor_Speed(i);
			  LED_GREEN_TOGGLE();
				delay_ms(100);
		}

    while(1)
    {
        LED_GREEN_TOGGLE();
				delay_ms(2000);
    }
}
