### mqtt移植STM32F103 Freertos过程

#### 前期准备

1. 使用CubeMX生成Keil工程；

2. Github下载最新mqtt源码，<https://github.com/jiejieTop/mqttclient>

3. mqtt源码目录：

   ![image-20220612125852546](https://gcltypora.oss-cn-guangzhou.aliyuncs.com/img/202206121258686.png)

#### 合并代码

1. 工程目录下新建mqttclient文件夹，将mqtt源码拖进mqttclient文件夹；

2. Keil工程下新建目录

   * mqttclient/common
   * mqttclient/log
   * mqttclient/mqttclient
   * mqttclient/mqtt
   * mqttclient/network
   * mqttclient/platform
   * mqttclient/test

3. 添加代码

   ![image-20220627225532717](https://gcltypora.oss-cn-guangzhou.aliyuncs.com/img/202206272255823.png)

4. 添加头文件路径

![image-20220627225805398](https://gcltypora.oss-cn-guangzhou.aliyuncs.com/img/202206272258513.png)

#### 编译调试

```c
..\mqttclient\common\log\salof_defconfig.h(105): error:  #5: cannot open source input file "pthread.h": No such file or directory
```

![image-20220612130109494](https://gcltypora.oss-cn-guangzhou.aliyuncs.com/img/202206121301624.png)

mqttclient/log目录下.c文件暂时不用，删除，同步移除头文件路径。

---

```c
..\mqttclient\mqttclient\mqtt_defconfig.h(77): error:  #5: cannot open source input file "mbedtls/config.h": No such file or directory
```

![image-20220612131058933](https://gcltypora.oss-cn-guangzhou.aliyuncs.com/img/202206121310022.png)

没有使用TLS传输，在mqtt_config.h文件中开启`MQTT_NETWORK_TYPE_NO_TLS`定义。

![image-20220612131313412](https://gcltypora.oss-cn-guangzhou.aliyuncs.com/img/202206121313504.png)

---

```c
..\mqttclient\platform\FreeRTOS\platform_net_socket.h(14): error:  #5: cannot open source input file "lwip/opt.h": No such file or directory
```

没有找到lwip/opt.h头文件，lwip协议未用到，用宏定义开关屏蔽。

![image-20220612131553904](https://gcltypora.oss-cn-guangzhou.aliyuncs.com/img/202206121315985.png)

---

```c
..\mqttclient\common\mqtt_log.h(24): error:  #5: cannot open source input file "salof.h": No such file or directory
```

![image-20220612131719563](https://gcltypora.oss-cn-guangzhou.aliyuncs.com/img/202206121317650.png)

错误信息打印，未用到，屏蔽。

![image-20220612131842069](https://gcltypora.oss-cn-guangzhou.aliyuncs.com/img/202206121318142.png)

---

```c
..\mqttclient\platform\FreeRTOS\platform_net_socket.h(26): error:  #20: identifier "size_t" is undefined
```

![image-20220612132247573](https://gcltypora.oss-cn-guangzhou.aliyuncs.com/img/202206121322655.png)

```c
nettype_tcp.c --> #include "nettype_tcp.h" --> #include "platform_net_socket.h" //缺少size_t定义
```

```c
nettype_tcp.c --> #include "mqtt_log.h" --> #include <stdio.h> //定义size_t
```

调换`#include "nettype_tcp.h"`和`#include "mqtt_log.h"`定义。

![image-20220612133106527](https://gcltypora.oss-cn-guangzhou.aliyuncs.com/img/202206121331613.png)

---

```c
..\mqttclient\platform\FreeRTOS\platform_timer.c(15): error:  #18: expected a ")"
```

![image-20220612133235418](https://gcltypora.oss-cn-guangzhou.aliyuncs.com/img/202206121332510.png)

![image-20220612133319443](https://gcltypora.oss-cn-guangzhou.aliyuncs.com/img/202206121333529.png)

`TickType_t`未定义导致，屏蔽，直接使用1000。

![image-20220612133502083](https://gcltypora.oss-cn-guangzhou.aliyuncs.com/img/202206121335170.png)

---

```c
..\mqttclient\platform\FreeRTOS\platform_net_socket.h(33): error:  #20: identifier "socklen_t" is undefined
```

`socklen_t`博主在lwip协议中定义，我未使用lwip协议，需要补`socklen_t`定义。

![image-20220612133735052](https://gcltypora.oss-cn-guangzhou.aliyuncs.com/img/202206121337128.png)

---

```c
..\mqttclient\network\network.c(78): warning:  #223-D: function "memset" declared implicitly
```

`network.c`缺少`memset()`定义，添加`#include <string.h> `包含。

![image-20220612134012998](https://gcltypora.oss-cn-guangzhou.aliyuncs.com/img/202206121340070.png)

---

```c
compiling platform_net_socket.c...
..\mqttclient\platform\FreeRTOS\platform_net_socket.h(28): error:  #20: identifier "size_t" is undefined
```

编译`platform_net_socket.c`时提示缺少`size_t`定义，添加`#include "mqtt_log.h"`头文件包含。

![image-20220612135101867](https://gcltypora.oss-cn-guangzhou.aliyuncs.com/img/202206121351030.png)

---

```c
..\mqttclient\platform\FreeRTOS\platform_net_socket.c(14): error:  #70: incomplete type is not allowed
```

`platform_net_socket.c`中的函数是跟硬件平台相关的API，先将内部逻辑删除，所有函数返回0。

---

```c
..\mqttclient\mqttclient\mqttclient.h(79): error:  #3093: anonymous structs are only supported in --gnu mode, or when enabled with #pragma anon_unions
```

错误提示大致的意思是匿名联合只在**gnu mode**，或者在联合体有**#pragma anon_unions**下支持。

![image-20220627225102229](https://gcltypora.oss-cn-guangzhou.aliyuncs.com/img/202206272251442.png)

#### 编译成功

![image-20220612135359670](https://gcltypora.oss-cn-guangzhou.aliyuncs.com/img/202206121353760.png)

#### 调试
1. mqtt连接失败
    ```c
   if (0 != mqtt_connect(client))
    {
        printf("mqtt_connect err\r\n");
        vTaskDelete(NULL);
    }
    ```
调试时发现，mqtt默认线程`mqtt_yield_thread`总是无法被创建，`xTaskCreate`时返回`errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY`空间不足。
是因为`#define configTOTAL_HEAP_SIZE                    ((size_t)3478)`，FreeRTOS默认的堆空间不足，拓展到`0x4000`即可。

2. 系统卡死
```c
static void mqtt_yield_thread(void *arg)
{
    int rc;
    client_state_t state;
    mqtt_client_t *c = (mqtt_client_t *)arg;
    platform_thread_t *thread_to_be_destoried = NULL;
    
    state = mqtt_get_client_state(c);
        if (CLIENT_STATE_CONNECTED !=  state) {
            MQTT_LOG_W("%s:%d %s()..., mqtt is not connected to the server...",  __FILE__, __LINE__, __FUNCTION__);
            platform_thread_stop(c->mqtt_thread);    /* mqtt is not connected to the server, stop thread */
    }

    while (1) {
        rc = mqtt_yield(c, c->mqtt_cmd_timeout);
        if (MQTT_CLEAN_SESSION_ERROR == rc) {
            MQTT_LOG_W("%s:%d %s()..., mqtt clean session....", __FILE__, __LINE__, __FUNCTION__);
            network_disconnect(c->mqtt_network);
            mqtt_clean_session(c);
            goto exit;
        } else if (MQTT_RECONNECT_TIMEOUT_ERROR == rc) {
            MQTT_LOG_W("%s:%d %s()..., mqtt reconnect timeout....", __FILE__, __LINE__, __FUNCTION__);
        }
    }
    
exit:
    thread_to_be_destoried = c->mqtt_thread;
    c->mqtt_thread = (platform_thread_t *)0;
    platform_thread_destroy(thread_to_be_destoried);
}
```
`mqtt_yield_thread`创建后立即被执行，先获取mqtt连接状态，不等于`CLIENT_STATE_CONNECTED `时挂起`mqtt_yield_thread`时导致系统卡死。
系统是在`MQTT_Client_Task`(优先级0)任务中调用`mqtt_connect(client)`,并创建`mqtt_connect(client)`(优先级5)，把`MQTT_Client_Task`任务优先级由0提升到6后即解决该问题，具体原因暂不详。
修改前：`xTaskCreate(MQTT_Client_Task, "MQTT_Client_Task", 128, NULL, 0, NULL);`
修改后：`xTaskCreate(MQTT_Client_Task, "MQTT_Client_Task", 128, NULL, 6, NULL);`

3. 连接成功publish几次后，提示发送错误，断开连接
