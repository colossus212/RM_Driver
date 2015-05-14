#ifndef __USART_H__
#define __USART_H__

extern unsigned char rx_buffer[10];

void USART2_Configuration(void);
int USART2_DMA_SendData(char *data, uint16_t size);
void USART2_SendChar(char b);
void USART2_SendStr(char *str);

#endif
