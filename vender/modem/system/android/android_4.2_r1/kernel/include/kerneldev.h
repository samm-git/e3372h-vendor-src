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
#ifndef _KERNELDEV_H
#define _KERNELDEV_H

#include <linux/kthread.h>
#include <linux/sched.h>

#include "platform_api_cpe.h"

#ifdef __cplusplus
extern "C" {
#endif


/* LED 灯当前状态: 亮或者灭*/
#define LED_ON   (1)
#define LED_OFF (0)

int kernel_led_write(LED_IOCTL_ARG stCtrlParms, int next_state);
int kernel_led_blink(LED_IOCTL_ARG stCtrlParms);


#ifdef __cplusplus
}
#endif

#endif /* _KERNELDEV_H */
