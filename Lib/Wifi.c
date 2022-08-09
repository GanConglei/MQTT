#include <stdio.h>
#include <string.h>

#include "RingBuff.h"
#include "Uart.h"
#include "Wifi.h"
#include "cmsis_os.h"
#include "gpio.h"
#include "mqttclient.h"

#define AT_OK      0
#define AT_ERR     -1
#define AT_TIMEOUT -2

RingBuff_t RecBuff = {0};

unsigned char ReplyBuf[USART_RX_LEN] = {0};
int8_t        wifiStatus             = 0;

SemaphoreHandle_t sendSemBinary;
SemaphoreHandle_t rxSemBinary;

void AT_Init(void)
{
    sendSemBinary = xSemaphoreCreateBinary();
    rxSemBinary   = xSemaphoreCreateBinary();
}

// void SetWifiStatus(int8_t status)
// {
//     wifiStatus = status;
// }

// int8_t GetWifiStatus(void)
// {
//     return wifiStatus;
// }

void SaveSpecialChar(void)
{
}

int AT_SendCmd(unsigned char *buf, char *reply, unsigned int timeout)
{
    int result = 0;

    result = Uart_SendData(buf, strlen((char *)buf), timeout);
    if (0 == result)  //发送成功
    {
        memset(ReplyBuf, 0, USART_RX_LEN);
        memcpy(ReplyBuf, reply, strlen(reply));
        result = xSemaphoreTake(sendSemBinary, timeout);
        if (pdPASS == result)
        {
            result = AT_OK;
        }
        else
        {
            result = AT_TIMEOUT;
        }
    }
    else
    {
        result = AT_TIMEOUT;
    }

    return result;
}

int AT_SendData(unsigned char *buf, unsigned int len, unsigned int timeout)
{
    int result = 0;

    result = Uart_SendData(buf, len, timeout);

    if (result)
    {
        result = AT_TIMEOUT;
    }
    return result;
}

int AT_ReadData(unsigned char *buf, unsigned int timeout)
{
    int result = 0;

    do
    {
        if (0 == RingBuff_Read(buf, &RecBuff))
        {
            return AT_OK;
        }
        else
        {
            result = xSemaphoreTake(rxSemBinary, portMAX_DELAY);
            if (pdFAIL == result)
            {
                return AT_TIMEOUT;
            }
        }
    } while (pdTRUE == result);

    return 0;
}

void AT_Rx_Task(void *par)
{
    uint8_t len = 0;
    uint8_t i   = 0;

    while (1)
    {
        if (usartRxStatus & 0x8000)
        {
            len = usartRxStatus & 0x7fff;  //得到此次接收到的数据长度
            if (strstr((char *)usartRxBuf, (char *)ReplyBuf))
            {
                xSemaphoreGive(sendSemBinary);
            }

            /* 特殊字符存入RecBuff */
            if (strstr((char *)usartRxBuf, "IPD"))
            {
                for (i = 20; i < len; i++)
                {
                    RingBuff_Write(usartRxBuf[i], &RecBuff);
                }
                xSemaphoreGive(rxSemBinary);
            }
            memset(usartRxBuf, 0, USART_RX_LEN);
            usartRxStatus = 0;
        }
    }
}

static void topic1_handler(void *client, message_data_t *msg)
{
    (void)client;
    MQTT_LOG_I(
        "----------------------------------------------------------------------"
        "-------------");
    MQTT_LOG_I("%s:%d %s()...\r\ntopic: %s\r\nmessage:%s", __FILE__, __LINE__, __FUNCTION__,
               msg->topic_name, (char *)msg->message->payload);
    MQTT_LOG_I(
        "----------------------------------------------------------------------"
        "-------------");
}

void MQTT_Client_Task(void *par)
{
    int err;

    mqtt_client_t *client = NULL;
    mqtt_message_t msg;

    memset(&msg, 0, sizeof(msg));

    mqtt_log_init();

    client = mqtt_lease();

    mqtt_set_port(client, "1883");

    mqtt_set_host(client, "172.20.10.12");

    mqtt_set_client_id(client, random_string(10));
    mqtt_set_user_name(client, random_string(10));
    mqtt_set_password(client, random_string(10));
    mqtt_set_clean_session(client, 1);

    if (0 != mqtt_connect(client))
    {
        printf("mqtt_connect err\r\n");
        vTaskDelete(NULL);
    }

    err = mqtt_subscribe(client, "topic1", QOS0, topic1_handler);
    if (err)
    {
        printf("mqtt_subscribe topic1 err\r\n");
    }

    err = mqtt_subscribe(client, "topic2", QOS1, NULL);
    if (err)
    {
        printf("mqtt_subscribe topic2 err\r\n");
    }

    err = mqtt_subscribe(client, "topic3", QOS2, NULL);
    if (err)
    {
        printf("mqtt_subscribe topic3 err\r\n");
    }

    msg.payload    = "I am weidongshan";
    msg.qos        = QOS0;
    msg.payloadlen = strlen(msg.payload);

    while (1)
    {
        mqtt_publish(client, "mcu_test", &msg);
        vTaskDelay(5000);
    }
}
