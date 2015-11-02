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
#ifndef _BREATH_LED_H
#define _BREATH_LED_H

#ifdef __cplusplus
extern "C" {
#endif

#define PUBLIC 
#define PRIVATE static
#define BR_OK    1
#define BR_ERROR 0

/* 呼吸灯状态控制 */
typedef enum
{
    BREATH_LED_STATE_LIGHT   = 0, /* 呼吸灯常亮 */
    BREATH_LED_STATE_SLEEP   = 1, /* 呼吸灯常灭 */
    BREATH_LED_STATE_SPARK   = 2, /* 呼吸灯呼吸 */
    BREATH_LED_STATE_ENABLE  = 3, /* 呼吸灯使能 */
    BREATH_LED_STATE_DISABLE = 4, /* 呼吸灯关闭 */
    BREATH_LED_STATE_MAX
}breathled_state;

/************************************************************************
 *函数原型 ： int breathled_controller(breathled_state led_state)
 *描述     ： 呼吸灯驱动内核接口，用于处理内核态其它模块程序下发的指令
 *输入     ： BREATH_LED_STATE_LIGHT    呼吸灯常亮 
              BREATH_LED_STATE_SLEEP    呼吸灯常灭
              BREATH_LED_STATE_SPARK    呼吸灯闪烁
              BREATH_LED_STATE_ENABLE   呼吸灯打开
              BREATH_LED_STATE_DISABLE  呼吸灯关闭
 *输出     ： 无
 *返回值   ： BR_OK OR BR_ERRO
 *修改历史     :
 1.日    期   : 20131205
   修改内容   : 新生成函数
*************************************************************************/
PUBLIC int breathled_controller(breathled_state led_state);

#ifdef __cplusplus
}
#endif

#endif /* _BREATH_LED_H */
