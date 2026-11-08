# IAP

先向stm32烧录boot程序，打开上位机，打开app的bin文件，串口握手成功后升级；

app有两个打印任务，通过spi协议发到esp32端（主机），再通过wifi协议发送至上位机，上位机可以选择wifi模式，在日志区检测print任务执行（printtask1和printtask2轮流执行）

