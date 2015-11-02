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
/******************************************************************************************************
    问题单号                       时间                   修改人                        说明
******************************************************************************************************/
#ifndef _USB_CHG_MANAGER_
#define _USB_CHG_MANAGER_

#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/utsname.h>
#include <linux/platform_device.h>
#include "usb_platform_comm.h"
typedef enum
{
    CHG_CHGR_UNKNOWN    = 0,
    CHG_WALL_CHGR       = 1,
    CHG_USB_HOST_PC     = 2,
    CHG_NONSTD_CHGR     = 3,
    CHG_WIRELESS_CHGR   = 4,
    CHG_EXGCHG_CHGR     = 5,
    CHG_500MA_WALL_CHGR = 6,
    CHG_USB_OTG_CRADLE  = 7,
    CHG_CHGR_INVALID    = 8,
    CHG_CHGR_TYPE_RSVD  = 0xFFFF /* */
} chg_chgr_type_t;
/**
*   CHARGER(充电器)主动上报的事件
*/
typedef enum _GPIO_CHARGER_EVENT
{
    GPIO_CHARGER_ATTATCH = 0,  /* 在充电器插入时上报，表示有充电器插入 */
    GPIO_CHARGER_REMOVE = 1,       /* 在充电器拔出时上报，表示充电器被拔出 */
    GPIO_WIRELESS_ATTATCH = 2,     /* 在无线充电器插入时上报，表示有无线充电器插入 */
    GPIO_WIRELESS_REMOVE = 3,      /* 在无线充电器插拔出时上报，表示有无线充电器拔出 */
    GPIO_CHARGER_EVENT_MAX     /* 如果事件值不小于此值，则为非法事件 */
} GPIO_CHARGER_EVENT;

typedef struct mbb_charger
{
    chg_chgr_type_t current_chg_type;  /* 充电类型*/
    USB_BOOL  usb_chg_action_flag;       /*充电插入，拔出*/
    USB_INT usb_chg_work_mode;       /*充电工作模式*/
    struct workqueue_struct* usb_chg_notify_wq;
    struct delayed_work usb_chg_notify_work;
} mbb_charger_st;


/*用于单板充电模式互斥 保护*/
enum usb_chg_mode
{
    USB_CHG_NON = 0, /* 不充电模式 */
    USB_CHG_LINE,       /*USB 线整机充电模式 */
    USB_CHG_WIRELESS, /*无线充电模式*/
    USB_CHG_EXTERN,     /* 对外充电模式 */
};

/* 检查充电器类型的防抖时间定义 */
#define MSM_CHG_DET_DELAY_TIME      (1000 * HZ / 1000)

#define CHG_DETECT_OVERTIME (10 * 1000)   /* 10s */
/* 全局变量记录当前的充电类型 */
/*Begin:added by wei wenwen 84014342  2013-9-22 */

#define MBB_USB_CHARGER_TYPE_HUAWEI                  1
#define MBB_USB_CHARGER_TYPE_NOT_HUAWEI          2
#define MBB_USB_CHARGER_TYPE_PC                           3
#define MBB_USB_CHARGER_TYPE_INVALID                  0

#define MBB_USB_DEVICE_INSERT                         1
#define MBB_USB_DEVICE_IDEN                             2
#define MBB_USB_ENUM_DONE                               3
#define MBB_USB_DEVICE_REMOVE                        0
USB_INT usb_charger_type_detect_event(struct notifier_block* nb, USB_ULONG val, USB_PVOID data);
USB_VOID usb_set_speed(USB_INT speed);
USB_VOID usb_charger_init(USB_VOID);
USB_VOID usb_charger_exit(USB_VOID);
USB_VOID usb_chg_set_work_mode(USB_INT chg_mode);
USB_BOOL usb_chg_check_current_mode(USB_INT chg_mode);
#if (MBB_CHG_WIRELESS == FEATURE_ON)
USB_BOOL usb_chg_wireless_detect(USB_VOID);
USB_BOOL usb_chg_wireless_remove(USB_VOID);
#else
static inline USB_BOOL usb_chg_wireless_detect(USB_VOID)
{
    return 0;
}
static inline USB_BOOL usb_chg_wireless_remove(USB_VOID)
{
    return 0;
}
#endif /*MBB_CHG_WIRELESS*/

#endif
