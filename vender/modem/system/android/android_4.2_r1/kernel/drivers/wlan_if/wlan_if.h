/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2013-2015. All rights reserved.
 *
 * mobile@huawei.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef _wlan_if_h_
#define _wlan_if_h_

#include <linux/signal.h>
#include "wlan_utils.h"


#define    WLAN_TRACE_INFO(fmt, ...)    printk(fmt, ##__VA_ARGS__)
#define    WLAN_TRACE_ERROR(fmt, ...)    printk(fmt, ##__VA_ARGS__)


/* signal report */
typedef enum
{
    WIFI_SIGNAL_UPDATE_STA_LIST    = SIGIO,   /* WiFi STA列表更新 */
    WIFI_SIGNAL_AUTO_SHUTDOWN      = SIGUSR2,   /* WiFi自动关闭信号 */
}WLAN_SIGNAL_EVENT_ENUM;

/* event report */
typedef enum _WLAN_EVENT_TYPE
{
    USER_WIFI_TIMEOUT_EVENT         = 1,         /* WiFi自动关闭消息 */
    USER_WIFI_UPDATE_STA_LIST       = 2,         /* STA接入个数更新事件 */
    USER_WIFI_DATA_DEAD_EVENT       = 32,        /* 控制导致FW异常上报 */
    USER_WIFI_CTRL_DEAD_EVENT       = 33,        /* 数传导致FW异常上报 */
    USER_WIFI_NULL_EVENT = ((unsigned int)-1),   /* 空事件 */
}WLAN_EVENT_TYPE;

typedef struct _wlan_user_event
{
    WLAN_EVENT_TYPE eventId;
    unsigned int eventVal;
} WLAN_USER_EVENT;

 /* WiFi芯片状态检测结构体,DHD中还有相同结构体的定义*/
 typedef struct
 {
     unsigned int rxerror;
     unsigned int txerror;
     unsigned int cmderror;
 }WLAN_STATUS_STU;


 /*===========================================================================
 
                         函数声明部分
 
 ===========================================================================*/

/*****************************************************************************
 函数名称  : WLAN_RETURN_TYPE wlan_signal_report(WiFi_SIGNAL_EVENT_ENUM signal)
 功能描述  : 向应用层发送信号
 输入参数  : NA
 输出参数  : NA
 返 回 值  : 当前加载WiFi类型值
*****************************************************************************/
WLAN_RETURN_TYPE wlan_signal_report(WLAN_SIGNAL_EVENT_ENUM signal);

/*****************************************************************************
 函数名称  : WLAN_RETURN_TYPE wlan_event_report(WLAN_USER_EVENT *event)
 功能描述  : 向应用层发送事件
 输入参数  : NA
 输出参数  : NA
 返 回 值  : 当前加载WiFi类型值
*****************************************************************************/
WLAN_RETURN_TYPE wlan_event_report(WLAN_USER_EVENT *event);


#endif


