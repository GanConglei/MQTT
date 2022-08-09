### 1.安装软件

安装两个软件：

![image-20220628210041623](https://gcltypora.oss-cn-guangzhou.aliyuncs.com/img/202206282100815.png)

### 2.启动服务器

`WIN+R`启动DOS命令行，进入`mosquitto-2.0.14-install-windows-x64`的安装目录，执行命令：

```shell
C:\Users\44135>d:

D:\>cd mqtt

D:\mqtt>cd mosquitto

D:\mqtt\mosquitto>.\mosquitto.exe -v
```

![image-20220628210511361](https://gcltypora.oss-cn-guangzhou.aliyuncs.com/img/202206282105478.png)

保持`mosquitto-2.0.14-install-windows-x64`处在运行状态；

### 3 使用MQTTX

#### 3.1 建立连接

运行MQTTX后，如下图操作：

![image-20220224164238214](https://gcltypora.oss-cn-guangzhou.aliyuncs.com/img/202206282107343.png)

![image-20220224164453390](https://gcltypora.oss-cn-guangzhou.aliyuncs.com/img/202206282107340.png)

#### 3.2 订阅主题

建立连接后，如下图操作：

![image-20220224164541550](https://gcltypora.oss-cn-guangzhou.aliyuncs.com/img/202206282107336.png)

![image-20220224164643934](https://gcltypora.oss-cn-guangzhou.aliyuncs.com/img/202206282107345.png)


#### 3.3 发布主题

如下操作：

![image-20220224165011262](https://gcltypora.oss-cn-guangzhou.aliyuncs.com/img/202206282107348.png)

### 4 使用mosquitto

#### 4.1 发布消息

使用DOS命令行，进入mosquitto-2.0.14-install-windows-x64的安装目录，执行命令：

```shell
C:\Users\44135>d:

D:\>cd mqtt

D:\mqtt>cd mosquitto

.\mosquitto_pub.exe -h 127.0.0.1 -p 1883  -t "100ask" -m "test"
```

![image-20220224165619509](https://gcltypora.oss-cn-guangzhou.aliyuncs.com/img/202206282107369.png)

#### 4.2 订阅消息

使用DOS命令行，进入mosquitto-2.0.14-install-windows-x64的安装目录，执行命令：

```shell
cd  "c:\Program Files\mosquitto"
.\mosquitto_sub.exe -h 127.0.0.1 -p 1883  -t "100ask"
```

![image-20220224165950047](https://gcltypora.oss-cn-guangzhou.aliyuncs.com/img/202206282107647.png)

### 5.添加远程连接

以上只能使用127.0.0.1进行本地连接，如需进行远程连接，需修改配置文件。

[hMosquitto创建MQTT服务器提示Starting in local only mode的解决办法](https://www.jianshu.com/p/943c422b72d4)

![image-20220628212709674](https://gcltypora.oss-cn-guangzhou.aliyuncs.com/img/202206282127812.png)

```shell
C:\Users\44135>d:

D:\>cd mqtt

D:\mqtt>cd mosquitto

mosquitto.exe -c mosquitto.conf -v
```
