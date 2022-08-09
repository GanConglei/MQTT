#include "RingBuff.h"
#include "Uart.h"
#include "Wifi.h"
#include "cmsis_os.h"
#include "stm32f1xx_hal.h"
#include "string.h"
#include "usart.h"

unsigned char usartRxBuf[USART_RX_LEN];  //接收缓冲,最大USART_RX_LEN个字节.

//接收状态
// bit15，	接收完成标志
// bit14~0，接收到的有效字节数目
uint16_t usartRxStatus = 0;  //接收状态标记

void Uart_Init(void)
{
    HAL_UART_Receive_DMA(&huart2, usartRxBuf, USART_RX_LEN);  //开启DMA接收
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);              //打开空闲接收中断
}

int Uart_SendData(unsigned char *buf, unsigned int len, unsigned int timeout)
{
    int result = 0;

    result = HAL_UART_Transmit(&huart2, buf, len, timeout);

    return result;
}
