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
#ifndef _wlan_drv_if_h_
#define _wlan_drv_if_h_


#define  BRCM_WL_TOOL  "/system/bin/wifi_brcm/exe/wl"

typedef enum 
{
    WIFI_CHIP_NAME_43362    = 43362, /* BCM 43362 */
    WIFI_CHIP_NAME_43239    = 43239, /* BCM 43239 */
    WIFI_CHIP_NAME_43241    = 43241, /* BCM 43241 */
    WIFI_CHIP_NAME_43236    = 43236, /* BCM 43236 */
    WIFI_CHIP_NAME_43217    = 43217, /* BCM 43217 */
    WIFI_CHIP_NAME_4354     = 4354,  /* BCM 4354  */  
    WIFI_CHIP_NAME_8189     = 8189,  /* RTL 8189  */
    WIFI_CHIP_NAME_8192     = 8192,  /* RTL 8192  */ 
}WIFI_CHIP_NAME;


/* WiFi 模块自己的netlink事件号，不要和系统的冲突 */
#define NETLINK_USER_WIFI_EVENT   31



/* 和v7r1 保持一致，便于应用层处理 */
#define  WIFI_AP_SHUTDOWN_TIMEOUT_SET_STR          "9 timeout:%u"           /* wifi 自动关闭时间 */
#define  WIFI_SHUTDOWN_SET_STR                     "10 shutdown %u:%s"      /* wifi 芯片下电 */
#define  WIFI_STARTUP_SET_STR                      "11 powerup %u:%s"       /* wifi 芯片上电 */
#define  WIFI_STARTUP_VSDB_SET_STR                 "12 powerup %u:%s"
#define  WIFI_TIMER_SET_CMD_STR                    "13 timeout:%u"
#define  WIFI_SET_KERNEL_FW_TYPE                   "14 fw_type:%u"          /* 芯片启动模式 */
#define  WIFI_MONITOR_PID_SET_STR                  "15 msg_pid:%u"          /* msg_monitor pid */
#define  WIFI_LOG_LEVEL_SET_STR                    "16 setlog:%d"           /* wifi log 打印级别 */


#define  WIFI_AP_SHUTDOWN_TIMEOUT_SET       9
#define  WIFI_SHUTDOWN_SET                  10
#define  WIFI_STARTUP_SET                   11
#define  WIFI_STARTUP_VSDB_SET              12
#define  WIFI_STA_SCAN_TIMEOUT_SET          13
#define  WIFI_KERNEL_FW_TYPE_SET            14
#define  WIFI_MONITOR_PID_SET               15
#define  WIFI_LOG_LEVEL_SET                 16


#endif


