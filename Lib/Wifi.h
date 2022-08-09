#ifndef _WIFI_H_
#define _WIFI_H_

extern void AT_Init(void);
extern int  AT_SendCmd(unsigned char *buf, char *reply, unsigned int timeout);
extern int  AT_SendData(unsigned char *buf, unsigned int len, unsigned int timeout);
extern int  AT_ReadData(unsigned char *buf, unsigned int timeout);
extern void AT_Rx_Task(void *par);
extern void MQTT_Client_Task(void *par);

#endif
