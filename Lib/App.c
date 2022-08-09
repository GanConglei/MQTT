#include "App.h"
#include "stm32f1xx_hal.h"
#include "usart.h"
#include "Uart.h"
#include "Wifi.h"
#include "cmsis_os.h"
#include <stdio.h>

int fputc(int ch, FILE *file)
{
    return HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
}

void Bsp_Init(void)
{
    Uart_Init();
    AT_Init();
}

void TaskCreat(void)
{
    xTaskCreate(AT_Rx_Task, "AT_Rx_Task", 128, NULL, 0, NULL);
    xTaskCreate(MQTT_Client_Task, "MQTT_Client_Task", 128, NULL, 6, NULL);
}
