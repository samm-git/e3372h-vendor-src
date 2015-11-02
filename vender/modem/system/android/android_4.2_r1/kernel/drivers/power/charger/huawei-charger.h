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
#ifndef HUAWEI_CHARGER_H
#define HUAWEI_CHARGER_H

/******************************问题单修改记录**********************************
    日期       修改人       问题单号                 修改内容
   2012.X.X    XXXX         DTSXXXX                  XXXXXXX
******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include <linux/power_supply.h>
#include <linux/notifier.h>
#include "product_config.h"
#include "linux/wakelock.h"

/*----------------------------------------------*
 * 结构体定义                                    *
 *----------------------------------------------*/
struct chargeIC_chip {
    struct i2c_client   *client;
    struct power_supply bat;
    struct power_supply usb;
    struct power_supply ac;
    struct power_supply extchg;
    struct power_supply wireless;

    int usb_online;                    /*USB是否在位，1表示在位，0表示不在位*/
    int ac_online;                     /*AC是否在位，1表示在位，0表示不在位*/
    int wireless_online;               /*无线充电是否在位，1表示在位，0表示不在位*/

    int extchg_online;                 /*USB ID线是否在位，1表示在位，0表示不在位*/
    int extchg_status;                 /*对外充电状态，正在对外充电还是停充等*/

    int bat_present;                   /*电池是否在位，1表示在位，0表示不在位*/
    int bat_stat;                      /*电池电压状态，是否在充电，是否需要补电等*/
    int bat_health;                    /*电池温度状态，是否过温等*/
    int bat_technology;                /*电池类型，锂电池等*/
    int bat_avg_voltage;               /*电池平均电压*/
    int bat_avg_temp;                  /*电池平均温度*/
    int bat_capacity;                  /*电池电量百分比*/
    int bat_time_to_full;              /*电池充满需要时间*/
    struct blocking_notifier_head notifier_list_bat;
    struct notifier_block nb;
    struct wake_lock alarm_wake_lock;  /* 充电上报事件时的延时锁 */
    struct wake_lock chg_wake_lock;    /* 防止任务执行中休眠用永久锁 */
	
    /*extchg monitor*/
    struct delayed_work	extchg_monitor_work;

    /*以下暂未使用为后续功能扩展留用*/
    int charge_current_limit_ua;
    int input_current_limit_ua;
    int term_current_ua;
    bool charging_enabled;
    bool otg_mode_enabled;
    bool charging_allowed;
    bool usb_suspend_enabled;
};
/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
#define CHARGEIC_PROC_FILE "driver/bq24192"
#define DLOAD_CHG_PROC_FILE "dload_chg"
#define POWERON_CHG_PROC_FILE "power_on"

#define ATOI_REG_ADDR_CNT           (0)
#define ATOI_REG_VALUE_CNT          (2)
#define ATOI_CONVERT_NUM            (10)
#define ATOI_MAX_LENGTH             (256)
#define DEV_ID_REG                  (0x0a)
#define DEV_ID_PN_MASK              (0x38)
#define DEV_ID_PN_SHIFT             (3)
#define DEV_ID_DEV_REG_MASK         (0x3)
#define DEV_ID_DEV_REG_SHIFT        (0)

#define ONLINE     1
#define OFFLINE    0

#define PRESENT    1
#define UNPRESENT  0


#endif

