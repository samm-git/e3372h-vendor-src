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
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <asm/uaccess.h>

#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/ioctl.h>

#include "led_comm.h"
#include <bsp_softtimer.h>
//#include "led_comm.h"

#ifdef __cplusplus
extern "C" {
#endif


/****************************************************************************
宏声明

*****************************************************************************/
#define  LED_BLINK_TASK_NAME    "tLedAllBlink"

/* LED 灯事件*/
#define APP_LED_SET   (1)
#define LED_TIMER_EVENT (2)

/* LED 灯当前状态: 1 亮或者0 灭*/
#define LED_ON   (1)
#define LED_OFF (0)

/* LED 灯定时器超时时长*/
#define KERDEV_BLINK_TIME (3000)
#define KERDEV_BLINK_TIME_1500 (1500)
#define KERDEV_BLINK_TIME_3000 (3000)

/*三色灯对应的GPIO */
#define GPIO_MODE_R_LED GPIO_1_23
#define GPIO_MODE_G_LED GPIO_1_24
#define GPIO_MODE_B_LED GPIO_1_22

/*未分配给led灯使用的无效GPIO*/
#define GPIO_INVALID (-1)

#define KERNELDEV_TIMER_CREAT(softtimer_info) \
    bsp_softtimer_create(softtimer_info)

#define KERNELDEV_TIMER_ADD(softtimer_info_list) \
    bsp_softtimer_add(softtimer_info_list)

#define KERNELDEV_TIMER_DELETE(softtimer_info_list) \
    bsp_softtimer_delete(softtimer_info_list)

#define KERNELDEV_TIMER_MODIFY(softtimer_info_list, timeout) \
    bsp_softtimer_modify(softtimer_info_list, timeout)

#define KERNELDEV_TIMER_FREE(softtimer_info_list) \
    bsp_softtimer_free(softtimer_info_list)




/****************************************************************************
 外部函数原型说明

*****************************************************************************/
int kernel_led_write(LED_IOCTL_ARG stCtrlParms, int next_state);
int kernel_led_read(LED_ID led_id, int *current_state);   
int kernel_led_blink(LED_IOCTL_ARG stCtrlParms);

int resource_release(void);
int resource_get(void);

int ledStateRead(LED_IOCTL_ARG stCtrlParms, int *current_state);
int ledStateWrite(LED_IOCTL_ARG stCtrlParms, int next_state);
int ledStateBlink(LED_IOCTL_ARG stCtrlParms);

static int kerneldevBlinkCallbackFunc(int event_id);
static DEFINE_SPINLOCK(led_event_lock);

int eventSend(int event_id);
int eventReceive(int *get_event_id);

/*LED 灯状态转换*/
static int kernelDeviceStateSwitch(int led_id, int currentState,
    int *nextState, int *blinkTime);
/*按照LED 灯状态进行点灯*/
int kernelDeviceStateShow(LED_IOCTL_ARG stCtrlParms);
int kernelDeviceIoctl_Test(int index); /*IOCTRL测试函数*/

#ifdef __cplusplus
}
#endif

