#include "RingBuff.h"
#include "Uart.h"
#include "Wifi.h"
#include "cmsis_os.h"
#include "stm32f1xx_hal.h"
#include "string.h"
#include "usart.h"

unsigned char usartRxBuf[USART_RX_LEN];  //���ջ���,���USART_RX_LEN���ֽ�.

//����״̬
// bit15��	������ɱ�־
// bit14~0�����յ�����Ч�ֽ���Ŀ
uint16_t usartRxStatus = 0;  //����״̬���

void Uart_Init(void)
{
    HAL_UART_Receive_DMA(&huart2, usartRxBuf, USART_RX_LEN);  //����DMA����
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);              //�򿪿��н����ж�
}

int Uart_SendData(unsigned char *buf, unsigned int len, unsigned int timeout)
{
    int result = 0;

    result = HAL_UART_Transmit(&huart2, buf, len, timeout);

    return result;
}
