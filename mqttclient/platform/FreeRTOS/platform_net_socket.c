/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-01-10 23:45:59
 * @LastEditTime: 2020-04-25 17:50:58
 * @Description: the code belongs to jiejie, please keep the author information
 * and source code according to the license.
 */
#include "Wifi.h"
#include "mqtt_log.h"  //22.06.12 add
#include "platform_net_socket.h"

#define WIFI_SSID     "Kent"
#define WIFI_PASSWORD "88888888"

int platform_net_socket_connect(const char *host, const char *port, int proto)
{
    int           err = 0;
    unsigned char cmd[100];

    while (1)
    {
        /* WIFI设置为 STA模式*/
        err = AT_SendCmd("AT+CWMODE=1\r\n", "OK\r\n", 2000);
        if (err)
        {
            printf("AT+CWMMODE=1 err = %d\r\n", err);
        }

        /* 重启生效*/
        err = AT_SendCmd("AT+RST\r\n", "ready\r\n", 10000);
        if (err)
        {
            printf("AT+RST err = %d\r\n", err);
        }

        /* 连接路由器 */
        // err = AT_SendData("AT+CWQAP\r\n", "OK\r\n", 2000);
        // if (err)
        // {
        //     printf("disconnet AP err = %d\r\n", err);
        // }

        err = AT_SendCmd("AT+CWJAP=\"" WIFI_SSID "\",\"" WIFI_PASSWORD "\"\r\n", "OK\r\n", 20000);
        if (err)
        {
            printf("AT+CWQAP err = %d\r\n", err);
            continue;
        }

        /* 连接服务器 */
        if (PLATFORM_NET_PROTO_TCP == proto)
        {
            sprintf((char *)cmd, "AT+CIPSTART=\"TCP\",\"%s\",%s\r\n", host, port);
        }
        else
        {
            sprintf((char *)cmd, "AT+CIPSTART=\"UDP\",\"%s\",%s\r\n", host, port);
        }
        err = AT_SendCmd(cmd, "OK\r\n", 20000);
        if (err)
        {
            printf("AT+CIPSTART err = %d\r\n", err);
            continue;
        }

        /* 开启透传模式 */
        // err = AT_SendCmd("AT+CIPMODE=1\r\n", "OK\r\n", 2000);
        // if (err)
        // {
        //     printf("AT+CIPMODE=1 err = %d\r\n", err);
        // }

        if (!err)
            break;
    }

    return 0;
}

#if 0 
int platform_net_socket_recv(int fd, void *buf, size_t len, int flags)
{
    return 0;
}
#endif

int platform_net_socket_recv_timeout(int fd, unsigned char *buf, int len, int timeout)
{
    int i   = 0;
    int err = 0;

    for (i = 0; i < len; i++)
    {
        err = AT_ReadData(&buf[i], timeout);
        if (err)
        {
            return 0;
        }
    }

    return len;
}

#if 0
int platform_net_socket_write(int fd, void *buf, size_t len)
{
    return 0;
}
#endif

int platform_net_socket_write_timeout(int fd, unsigned char *buf, int len, int timeout)
{
    int           err = 0;
    unsigned char cmd[20];

    sprintf((char *)cmd, "AT+CIPSEND=%d\r\n", len);
    err = AT_SendCmd(cmd, "OK\r\n", 2000);
    if (err)
    {
        printf("%s err = %d\n", cmd, err);
    }

    err = AT_SendData(buf, len, timeout);
    if (err)
    {
        printf("SendData err = %d\n", err);
        return 0;
    }

    return len;
}

int platform_net_socket_close(int fd)
{
    return AT_SendCmd("AT+CIPCLOSE\r\n", "OK\r\n", 2000);
}

#if 0
int platform_net_socket_set_block(int fd)
{
    return 0;
}

int platform_net_socket_set_nonblock(int fd)
{
    return 0;
}

int platform_net_socket_setsockopt(int fd, int level, int optname, const void *optval, socklen_t optlen)
{
    return 0;
}
#endif
