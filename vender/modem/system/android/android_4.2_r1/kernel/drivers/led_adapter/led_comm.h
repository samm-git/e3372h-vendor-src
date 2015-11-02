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

#ifdef __cplusplus
extern "C" {
#endif


#define MEM_IOC_MAGIC    'x' //定义类型
#define MEMDEV_IOC_MAXNR 2
typedef enum _LED_IOCTL_CMD
{
    LED_IOCTL_SET      = _IO(MEM_IOC_MAGIC, 0),  /*设置LED灯的状态*/
    LED_IOCTL_GET      = _IO(MEM_IOC_MAGIC, 1),  /*获取LED灯的状态*/
    LED_IOCTL_CMD_MAX = _IO(MEM_IOC_MAGIC, 2)   /*目前支持的命令个数*/
}LED_IOCTL_CMD;

typedef enum _LED_ID
{
    LED_ID_FIRST = 0,
    LED_ID_WIFI = LED_ID_FIRST,  /* WIFI灯 */
    LED_ID_BAT,                  /* 电池灯 */
    LED_ID_SMS,                  /* 短信灯 */
    LED_ID_CIRCLE,               /* 装饰灯 */
    LED_ID_SIGNAL1,              /* 信号灯1 */
    LED_ID_SIGNAL2,              /* 信号灯2 */
    LED_ID_SIGNAL3,              /* 信号灯3 */
    LED_ID_SLIC,                 /* 语音指示灯 */
    LED_ID_MODE,                 /* 网络制式指示灯 */
    LED_ID_MAX
} LED_ID;

typedef enum _LED_STATE
{
    LED_STATE_ON = 0,
    LED_STATE_OFF,
    LED_STATE_BLINK,
    LED_STATE_COLOR_BLINK,
    LED_STATE_ALL_BLINK,
    LED_STATE_MAX,
}LED_DEV_STATE;

typedef enum _LED_COLOR
{
    LED_COLOR_RED = 0,
    LED_COLOR_GREEN,
    LED_COLOR_YELLOW,
    LED_COLOR_BLUE,
    LED_COLOR_WHITE,
    LED_COLOR_CYAN,
    LED_COLOR_PURPLE,
    LED_COLOR_MAX
}LED_COLOR;

/*指示灯电流档*/
typedef enum
{
    LED_CURRENT_LEVEL0 = 0,
    LED_CURRENT_LEVEL1,
    LED_CURRENT_LEVEL2,
    LED_CURRENT_LEVEL3,
    LED_CURRENT_LEVEL4,
    LED_CURRENT_LEVELMAX
}LED_CURRENT;

/*LED的模式 */
typedef enum
{
    LED_MODE_NORMAL = 0,
    LED_MODE_SLEEP,
    LED_MODE_MAX
}LED_MODE;

typedef struct
{
    LED_DEV_STATE led_dev_state;
    LED_ID led_id;
    LED_COLOR led_color;
    LED_COLOR second_led_color;
    unsigned int duration;
    unsigned int second_duration;
    LED_CURRENT led_current;
    LED_MODE sleep_mode;
}LED_IOCTL_ARG;




#ifdef __cplusplus
}
#endif

#endif /* _KERNELDEV_H */
