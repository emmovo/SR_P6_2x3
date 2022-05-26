<!--
 * @Author: emmovo
 * @Date: 2022-05-03 08:51:54
 * @LastEditors: emmovo
 * @LastEditTime: 2022-05-25 14:25:35
 * @FilePath: \skippingRope_phy6222\README.md
 * @Description: 
 * 
 * Copyright (c) 2022 by mingjkl@live.com/emmovo.com, All Rights Reserved. 
-->
# skipping_rope_phy6222

## 1. 概述

该项目需开发一智能跳绳设备，设备上有4个按键进行操作输入，1个段码屏完成数据显示，显示内容分别为：蓝牙连接状态、运动模式、电池电量显示、跳绳个数和跳绳时间；设备通过锂电池供电，带USB充电功能；设备通过霍尔传感器捕获跳绳动作。

设备设置三个运动模式:

    a.自由跳模式
        该模式下，只通过已跳绳个数；
    b.计时跳模式
        用户设定一定的时间值，以倒计时的值完成一组运动；
    c.计数跳模式
        用户设定一定的运动目标值，以到达该值为一组运动；

具体的软件设计逻辑以设计需求书为准，细节可以和客户共同协商对开发需求书动态变更。

上位机使用涂鸦平台开发，设备需要运行涂鸦的API完成与涂鸦APP进行BLE数据交互和OTA等功能。

## 2. 硬件设计需求

- 主控IC为phy6222  √     
- 4个按键                              √

        电源键 / 设定键 / up键 / down键

- 4 x 16段码屏                         √

    段码屏驱动IC VK1088 (客户指定)      √
	段码屏定制
    段码屏背光      √

- USB充电检测      √
- 电池电量估算     √
- 锂电池供电       √
- 2个霍尔传感器    √

        SV229A(客户指定)

- isr烧录          √
- 1个蜂鸣器        √

## 3.软件开发计划

### 3.1 基础外设驱动程序编写

基于example\ble_peripheral\simpleBlePeripheral进行以下功能程序编写：

- IO读写
- UART通讯
- ADC数据采集
- PWM
- FLASH读写
- 休眠管理
- Watchdog
- Timer

### 3.2 段码屏驱动程序编写

基于VK1088的段码屏驱动程序编写

### 3.3 BLE基础功能实现

基于example\ble_peripheral\simpleBlePeripheral进行以下功能程序编写：
- 设备GAP信息广播
- GATT程序编写
- 数据交互服务定义
- OTA服务程序
- OTA Bootloader程序
- 匹配信息重置功能程序
- 设备电量上报程序

### 3.4 Tuya代码移植和功能实现

基于example\ble_peripheral\simpleBlePeripheral进行以下功能程序编写：
- tuya_ble_api移植
- tuya service task建立
- tuya产测功能移植
- tuya ota功能移植

### 3.5 应用代码编写移植和调试

- 按键功能移植和逻辑实现
- 段码屏驱动代码移植和逻辑实现
- USB充电检测和电池电量采集
- 段码屏背光亮度调节功能实现
- 蜂鸣器驱动程序编写
- FLASH数据读写
- 休眠和唤醒逻辑移植
- Watchdog功能移植
- Timer驱动程序移植
- 运动逻辑代码移植



