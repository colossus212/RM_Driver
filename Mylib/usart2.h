#ifndef __USART_H__
#define __USART_H__

extern unsigned char rx_buffer[10];

void USART2_Configuration(void);
void USART2_SendChar(char b);
void USART2_SendStr(char *str);


#endif
