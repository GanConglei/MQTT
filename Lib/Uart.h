#ifndef _UART_H_
#define _UART_H_

#include "main.h"

#define USART_RX_LEN 1024

extern unsigned char usartRxBuf[USART_RX_LEN];
extern uint16_t      usartRxStatus;

extern void Uart_Init(void);
extern int  Uart_SendData(unsigned char *buf, unsigned int len, unsigned int timeout);

#endif
