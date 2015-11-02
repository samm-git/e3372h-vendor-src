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

#include "drv_comm.h"
#include "led_comm.h"
#include "led_cpe.h"
#include <bsp_softtimer.h>

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
 全局变量申明

*****************************************************************************/
/*定义LED 任务ID */
struct task_struct *g_ledTaskID = NULL;
/*控制LED 闪烁任务监控闪烁事件的信号量*/
struct semaphore g_ledAllBlinkTaskSemId;
/*定义LED 事件*/
int g_event_recv = 0;
/*记录9 个灯对应的定时器*/
struct softtimer_list g_kerdevTimer ;
/*记录9 个灯对应的GPIO 位*/
unsigned int LED_GPIO[LED_ID_MAX] =
    {GPIO_INVALID,GPIO_INVALID,GPIO_INVALID,GPIO_INVALID,
    GPIO_3_0,GPIO_3_1,GPIO_3_2,GPIO_2_31,
    GPIO_INVALID};
/*记录9 个LED 灯的当前时间点的状态: 亮或者灭*/
/* 状态与下面g_current_kerdev_state[] 对应*/
unsigned int g_led_current_state[LED_ID_MAX] = 
    {LED_OFF, LED_OFF, LED_OFF, LED_OFF,
    LED_OFF, LED_OFF, LED_OFF, LED_OFF,
    LED_OFF};
/* LED 灯初始状态，并且 记录8 个灯的当前状态*/
LED_IOCTL_ARG g_current_kerdev_state[LED_ID_MAX] = 
{ 
    {
        LED_STATE_OFF,
        LED_ID_WIFI,
        LED_COLOR_GREEN,
        LED_COLOR_GREEN,
        KERDEV_BLINK_TIME,
        KERDEV_BLINK_TIME,
        LED_CURRENT_LEVEL3,
        LED_MODE_NORMAL
    },
    {
        LED_STATE_OFF,
        LED_ID_BAT,
        LED_COLOR_GREEN,
        LED_COLOR_GREEN,
        KERDEV_BLINK_TIME,
        KERDEV_BLINK_TIME,
        LED_CURRENT_LEVEL3,
        LED_MODE_NORMAL
    },
    {
        LED_STATE_OFF,
        LED_ID_SMS,
        LED_COLOR_GREEN,
        LED_COLOR_GREEN,
        KERDEV_BLINK_TIME,
        KERDEV_BLINK_TIME,
        LED_CURRENT_LEVEL3,
        LED_MODE_NORMAL
    },
    {
        LED_STATE_OFF,
        LED_ID_CIRCLE,
        LED_COLOR_GREEN,
        LED_COLOR_GREEN,
        KERDEV_BLINK_TIME,
        KERDEV_BLINK_TIME,
        LED_CURRENT_LEVEL3,
        LED_MODE_NORMAL
    },
    {
        LED_STATE_OFF,
        LED_ID_SIGNAL1,
        LED_COLOR_GREEN,
        LED_COLOR_GREEN,
        KERDEV_BLINK_TIME,
        KERDEV_BLINK_TIME,
        LED_CURRENT_LEVEL3,
        LED_MODE_NORMAL
    },
    {
        LED_STATE_OFF,
        LED_ID_SIGNAL2,
        LED_COLOR_GREEN,
        LED_COLOR_GREEN,
        KERDEV_BLINK_TIME,
        KERDEV_BLINK_TIME,
        LED_CURRENT_LEVEL3,
        LED_MODE_NORMAL
    },
    {
        LED_STATE_OFF,
        LED_ID_SIGNAL3,
        LED_COLOR_GREEN,
        LED_COLOR_GREEN,
        KERDEV_BLINK_TIME_1500,
        KERDEV_BLINK_TIME_3000,
        LED_CURRENT_LEVEL3,
        LED_MODE_NORMAL
    },
    {
        LED_STATE_OFF,
        LED_ID_SLIC,
        LED_COLOR_YELLOW,
        LED_COLOR_YELLOW,
        KERDEV_BLINK_TIME,
        KERDEV_BLINK_TIME,
        LED_CURRENT_LEVEL3,
        LED_MODE_NORMAL
    },
    {
        LED_STATE_OFF,
        LED_ID_MODE,
        LED_COLOR_RED,
        LED_COLOR_RED,
        KERDEV_BLINK_TIME,
        KERDEV_BLINK_TIME,
        LED_CURRENT_LEVEL3,
        LED_MODE_NORMAL
    }
};


/****************************************************************************
 内部函数原型说明

*****************************************************************************/


/****************************************************************************
 外部函数原型说明

*****************************************************************************/
extern int kernelDeviceIoctl(struct file *file, unsigned int cmd, unsigned long arg);


/****************************************************************************
 函数定义

*****************************************************************************/
/*****************************************************************************
函 数 名  : kernelDeviceStateShow
功能描述  : Led 状态展示
输入参数  : 
输出参数  : 
调用函数  : 
被调函数  : 
History     :
1.日    期  :2013-11-19
  作    者  : 王丽00249966
  修改内容  : Create
*****************************************************************************/
int kernelDeviceStateShow(LED_IOCTL_ARG stCtrlParms)
{
    int ret = BSP_ERROR;
    
    /*更新LED 灯的状态表*/
    memcpy((LED_IOCTL_ARG *)&g_current_kerdev_state[stCtrlParms.led_id],
        &stCtrlParms,sizeof(LED_IOCTL_ARG));  
    
    switch (stCtrlParms.led_dev_state)
    {
        case LED_STATE_ON:
        {
            printk(KERN_ERR "[LED] kernelDeviceStateShow: LED_STATE_ON\n");
             
            g_led_current_state[stCtrlParms.led_id] = LED_ON;
            ret = ledStateWrite(stCtrlParms, LED_ON);
            
            break;
        }
        case LED_STATE_OFF:
        {
            printk(KERN_ERR "[LED] kernelDeviceStateShow: LED_STATE_OFF\n");
            g_led_current_state[stCtrlParms.led_id] = LED_OFF;
            ret = ledStateWrite(stCtrlParms, LED_OFF);
            
            break;
        }
        case LED_STATE_BLINK:
        {

            printk(KERN_ERR "[LED] kernelDeviceStateShow: LED_STATE_BLINK\n");
            ret = ledStateBlink(stCtrlParms);
            
            break;
        }
        default:
        {
            printk(KERN_ERR "[LED] kernelDeviceStateShow: LED state ERROR!\n");
            break;
        }
    }

    return ret;
}



/*****************************************************************************
函 数 名  : kernelDeviceIoctl_Test
功能描述  : Led IOCTL 内核态测试函数
输入参数  : 
输出参数  : 
调用函数  : 
被调函数  : 
History     :
1.日    期  :2013-11-14
  作    者  : 王丽00249966
  修改内容  : Create
*****************************************************************************/
int kernelDeviceIoctl_Test(int index)
{
    int ret = BSP_ERROR;
    /* 实参1234 为IOCTL 的形参文件描述符，无用，随便取值 */
    int pf = NULL;
    LED_IOCTL_ARG stCtrlParms;
    memset((void *)&stCtrlParms,0,sizeof(LED_IOCTL_ARG));

    stCtrlParms.led_id = LED_ID_MODE;  /* 操作第8 号LED 灯*/
    stCtrlParms.led_dev_state = LED_STATE_BLINK; /* LED 灯状态*/
    stCtrlParms.led_color = LED_COLOR_YELLOW;
    stCtrlParms.second_led_color = LED_COLOR_RED;
    stCtrlParms.duration = KERDEV_BLINK_TIME_1500;
    stCtrlParms.second_duration = KERDEV_BLINK_TIME_3000;
    stCtrlParms.led_current = LED_CURRENT_LEVEL3;
    stCtrlParms.sleep_mode = LED_MODE_NORMAL;

    switch (index)
    {
        /* LED_DEV_READ 命令: 读取LED 灯当前状态*/
        case 1:
            ret = kernelDeviceIoctl(pf, LED_IOCTL_SET, &stCtrlParms);
            break;
        /* LED_DEV_WRITE 命令: 使LED 灯亮*/
        case 2:
            stCtrlParms.led_dev_state = LED_STATE_ON;
            ret = kernelDeviceIoctl(pf, LED_IOCTL_SET, &stCtrlParms);
            break;
        /* LED_DEV_BLINK 命令: 使LED 灯闪烁*/
        case 3:
            stCtrlParms.led_dev_state = LED_STATE_BLINK;
            ret = kernelDeviceIoctl(pf, LED_IOCTL_SET, &stCtrlParms);
            break;
        default:
            printk(KERN_ERR "[LED] kernelDeviceIoctl_Test: CMD ERROR!\n");
            break;
    }

    if (BSP_OK != ret)
    {
        printk(KERN_ERR "[LED] kernelDeviceIoctl_Test: FAIL!\n");
        return BSP_ERROR;
    }

    printk(KERN_INFO "[LED] kernelDeviceIoctl_Test: SUCCESS!\n");
    return BSP_OK;
}



/*****************************************************************************
函 数 名  : eventSend
功能描述  : 发送LED 事件到闪烁监控任务
输入参数  : struct inode *inode, struct file *file
输出参数  : 无
调用函数  : 无
被调函数  : 外部接口
History     :
1.日    期  :
  作    者  : 王丽00249966
  修改内容  : Create
*****************************************************************************/
int eventSend(int event_id)
{    
    unsigned long flags = 0;
    
    spin_lock_irqsave(&led_event_lock, flags);
    g_event_recv = event_id;
    spin_unlock_irqrestore(&led_event_lock, flags);

    /*释放信号量，停止闪烁监控任务*/
    up(&g_ledAllBlinkTaskSemId);

    return BSP_OK;
}

/*****************************************************************************
函 数 名  : eventReceive
功能描述  : LED 闪烁监控任务接收到LED 事件
输入参数  : struct inode *inode, struct file *file
输出参数  : 无
调用函数  : 无
被调函数  : 外部接口
History     :
1.日    期  :
  作    者  : 王丽00249966
  修改内容  : Create
*****************************************************************************/
int eventReceive(int *get_event_id)
{    
    /*获取信号量，起闪烁监控任务*/
    down(&g_ledAllBlinkTaskSemId);

    unsigned long flags = 0;
    spin_lock_irqsave(&led_event_lock, flags);
    /*接收到事件以后，取出事件记录*/
    *get_event_id = g_event_recv;
    g_event_recv = BSP_ERROR;
    spin_unlock_irqrestore(&led_event_lock, flags);

    return BSP_OK;
}

/*****************************************************************************
函 数 名  : kernelDeviceBlinkTask
功能描述  : Led字符设备闪烁操作的任务处理函数
输入参数  : 
输出参数  :
调用函数  : 
被调函数  : 外部接口
History     :
1.日    期  :2013-11-09
  作    者  : 王丽00249966
  修改内容  : Create
*****************************************************************************/
static int kernelDeviceBlinkTask(void)
{
    printk(KERN_INFO "[LED] kernelDeviceBlinkTask() IN\n");

    int eventRec = BSP_ERROR;
    int led_id = GPIO_INVALID;
    int blinkTime = 0;
    int currentState = LED_OFF;    /*假设灯初始状态为灭，可能今后出错的地方*/
    int nextState = LED_STATE_MAX;

    for ( ; ; )
    {  
        /*接收点灯事件eventReceive*/
        eventReceive(&eventRec);
        
        if (BSP_ERROR == eventRec)
        {
            continue;
        }

        printk(KERN_INFO "[LED] kernelDeviceBlinkTask: eventRec = %d\n", eventRec);

        if ((LED_TIMER_EVENT == eventRec) || (APP_LED_SET == eventRec))
        {
            printk(KERN_INFO "[LED] kernelDeviceBlinkTask: 1\n");
            /*检查是哪一个led 灯发送的事件*/
            for (led_id = LED_ID_SIGNAL1; led_id <= LED_ID_MODE; led_id++)
            {
                printk(KERN_INFO "[LED] kernelDeviceBlinkTask: 2\n");
                printk(KERN_INFO "[LED] kernelDeviceBlinkTask: global led_state = %d\n", 
                    led_id,g_current_kerdev_state[led_id].led_dev_state);
                
                if (LED_STATE_BLINK == g_current_kerdev_state[led_id].led_dev_state)
                {  
                    /*从全局变量中读取LED 灯当前时间点状态*/
                    if (BSP_OK != ledStateRead(g_current_kerdev_state[led_id], &currentState))  /*待重写*/
                    {
                        printk(KERN_ERR "[LED] kernel_led_read FAIL!\n");
                    }
                    printk(KERN_INFO "[LED] kernelDeviceBlinkTask: currentState = %d\n", 
                        currentState);

                    /* LED 灯状态转换仲裁*/
                    if (BSP_OK != kernelDeviceStateSwitch(led_id, currentState,
                        &nextState, &blinkTime))
                    {
                        printk(KERN_ERR "[LED] kernelDeviceStateSwitch FAIL!\n");
                    }

                     /*记录当前时间点LED 灯状态*/
                    g_led_current_state[led_id] = nextState;
                    
                    /*写待转换状态到led 灯GPIO*/
                    if (BSP_OK != ledStateWrite(g_current_kerdev_state[led_id], nextState))
                    {
                        printk(KERN_ERR "[LED] kernel_led_write fail!\n");
                    }
                    printk(KERN_INFO "[LED] kernelDeviceBlinkTask: nextState=%d\n", 
                        nextState);

                    /*开始创建指定LED 灯的定时器，或者更新定时器时间*/
                    if (APP_LED_SET == eventRec)
                    {
                        /*第一次创建和启动闪烁定时器*/                      
                        //struct softtimer_info kerdevTime;
                        g_kerdevTimer.func = kerneldevBlinkCallbackFunc;
                        g_kerdevTimer.para = LED_TIMER_EVENT;
                        g_kerdevTimer.timeout = blinkTime; /*闪烁的时间*/
                        g_kerdevTimer.wake_type = SOFTTIMER_WAKE;
                        KERNELDEV_TIMER_CREAT(&g_kerdevTimer);

                        printk(KERN_INFO "[LED] g_kerdevTimer ID = %d\n", led_id,
                            g_kerdevTimer.timer_id);
                    }
                    else if (LED_TIMER_EVENT == eventRec)
                    {
                        /*启动软定时器进行闪烁时间控制*/
                        KERNELDEV_TIMER_MODIFY(&g_kerdevTimer, blinkTime);
                        printk(KERN_INFO "[LED] bsp_softtimer_modify EXECUTE!\n");
                    }
                    else
                    {
                        //do nothing    
                    }
                    KERNELDEV_TIMER_ADD(&g_kerdevTimer);
                    printk(KERN_INFO "[LED] bsp_softtimer_add EXECUTE!\n");                 
                }
                else
                {
                     //do nothing
                }
            }     
        }
        else
        {
            //do nothing
        }
    }
}

/*****************************************************************************
函 数 名  : kernelDeviceStateSwitch
功能描述  : Led  灯状态转换仲裁函数
输入参数  : 
输出参数  :
调用函数  : 
被调函数  : 
History     :
1.日    期  :2013-11-09
  作    者  : 王丽00249966
  修改内容  : Create
*****************************************************************************/
static int kernelDeviceStateSwitch(int led_id, int currentState, int *nextState,
    int *blinkTime)
{
    /*判断灯亮或者灯灭*/
    if (LED_ON == currentState)
    {
        *nextState = LED_OFF;
        *blinkTime = g_current_kerdev_state[led_id].second_duration;
        printk("[LED] led state is changed from ON to OFF!\n");
    }
    else if (LED_OFF == currentState)
    {
        *nextState = LED_ON;
        *blinkTime = g_current_kerdev_state[led_id].duration;
        printk( "[LED] led state is changed from OFF to ON!\n");
    }
    else
    {
        printk(KERN_ERR "[LED]led state ERROR!\n");
        return BSP_ERROR;
    }    
    
    return BSP_OK;
}


/*****************************************************************************
函 数 名  :kerneldevBlinkCallbackFunc
功能描述  :灯亮或者灯灭定时器到时的回调函数
输入参数  :
输出参数  : 
调用函数  : 
被调函数  : 外部接口
History     :
1.日    期  : 2013-11-09
  作    者  : 王丽00249966
  修改内容  : Create
*****************************************************************************/
static int kerneldevBlinkCallbackFunc(int event_id)
{
    if (BSP_OK != eventSend(event_id))
    {
        printk(KERN_ERR "[LED] kerneldevBlinkCallbackFunc: Send event FAIL!\n");
        return BSP_ERROR;
    }
    return BSP_OK;
}

/*****************************************************************************
函 数 名  :kernel_led_read
功能描述  :读取GPIO取值获得 LED 灯当前状态
输入参数  :
输出参数  : 
调用函数  : 
被调函数  :内部接口
History     :
1.日    期  : 
  修改内容  : Create
*****************************************************************************/
int ledStateRead(LED_IOCTL_ARG stCtrlParms, int *current_state)
{
    printk(KERN_INFO "ledStateRead IN\n");
    
    if((LED_ID_SIGNAL1 > stCtrlParms.led_id) || (LED_ID_MODE < stCtrlParms.led_id))
    {
        printk(KERN_ERR "Led type error,LED ID=%d\n", stCtrlParms.led_id);
        return BSP_ERROR;
    }
    
    if (current_state == NULL)
    {
        printk(KERN_ERR "[LED] current_state is NULL!\n");
        return BSP_ERROR;
    }

    *current_state = g_led_current_state[stCtrlParms.led_id];
    
    return BSP_OK;
}

/*****************************************************************************
函 数 名  :kernel_led_read
功能描述  :读取LED 灯当前状态，亮或者灭

输入参数  :
输出参数  : 
调用函数  : 
被调函数  : 外部接口
History     :
1.日    期  : 2013-11-09
  作    者  : 王丽00249966
  修改内容  : Create
*****************************************************************************/
int kernel_led_read(LED_ID led_id, int *current_state)
{
    if((LED_ID_SIGNAL1 > led_id) || (LED_ID_MODE < led_id))
    {
        printk(KERN_ERR "[LED] kernel_led_read: Led ID error,LED ID=%d\n",led_id);
        return BSP_ERROR;
    }
    
    if (current_state == NULL)
    {
        printk(KERN_ERR "[LED] kernel_led_read: current_state is NULL!\n");
        return BSP_ERROR;
    }

    if (BSP_OK != ledStateRead(g_current_kerdev_state[led_id], current_state))
    {
        printk(KERN_ERR "[LED] kernel_led_read:FAIL!\n");
        return BSP_ERROR;
    }

    return BSP_OK;
}

/*****************************************************************************
函 数 名  :ledStateWrite
功能描述  :设置LED 灯状态
输入参数  :无
输出参数  : 无
调用函数  : 无
被调函数  : 外部接口
History     :
1.日    期  :
  修改内容  : Create
*****************************************************************************/
int ledStateWrite(LED_IOCTL_ARG stCtrlParms, int next_state)
{
    printk(KERN_INFO "[LED] ledStateWrite: IN,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
        stCtrlParms.led_dev_state,stCtrlParms.led_id,stCtrlParms.led_color,
        stCtrlParms.second_led_color,stCtrlParms.duration,stCtrlParms.second_duration,
        stCtrlParms.led_current,stCtrlParms.sleep_mode,next_state);
    
    if((LED_ID_SIGNAL1 > stCtrlParms.led_id) || (LED_ID_MODE < stCtrlParms.led_id))
    {
        printk(KERN_ERR "[LED] Led type error,LED ID=%d\n",stCtrlParms.led_id);
        return BSP_ERROR;
    }

    if ((next_state != LED_ON) && (next_state != LED_OFF))
    {
        printk(KERN_ERR "[LED] Led state error,LED state=%d\n",next_state);
        return BSP_ERROR;
    }

    /*普通灯的亮或灭设置*/
    if (LED_ID_MODE != stCtrlParms.led_id)
    {
        if (GPIO_INVALID == LED_GPIO[stCtrlParms.led_id])
        {
            printk(KERN_ERR "[LED] Led GPIO ERROR\r\n");
            return BSP_ERROR;
        }

        gpio_direction_output(LED_GPIO[stCtrlParms.led_id],next_state);
    }
    /*模式灯的亮或灭设置: 需要配色*/
    else
    {
        switch (stCtrlParms.led_color)
        {
                case LED_COLOR_RED:
                {
                    gpio_direction_output(GPIO_MODE_R_LED, next_state);
                    gpio_direction_output(GPIO_MODE_G_LED, LED_OFF);
                    gpio_direction_output(GPIO_MODE_B_LED, LED_OFF);
                    break;
                }
                case LED_COLOR_GREEN:
                {
                    gpio_direction_output(GPIO_MODE_G_LED, next_state);
                    gpio_direction_output(GPIO_MODE_R_LED, LED_OFF);
                    gpio_direction_output(GPIO_MODE_B_LED, LED_OFF);
                    break;
                }
                case LED_COLOR_YELLOW:
                {
                    gpio_direction_output(GPIO_MODE_R_LED, next_state);
                    gpio_direction_output(GPIO_MODE_G_LED, next_state);
                    gpio_direction_output(GPIO_MODE_B_LED, LED_OFF);
                    break;
                }
                case LED_COLOR_BLUE:
                {
                    gpio_direction_output(GPIO_MODE_B_LED, next_state);
                    gpio_direction_output(GPIO_MODE_R_LED, LED_OFF);
                    gpio_direction_output(GPIO_MODE_G_LED, LED_OFF);
                    break;
                }
                case LED_COLOR_WHITE:
                {
                    gpio_direction_output(GPIO_MODE_R_LED, next_state);
                    gpio_direction_output(GPIO_MODE_G_LED, next_state);
                    gpio_direction_output(GPIO_MODE_B_LED, next_state);
                    break;
                }
                case LED_COLOR_CYAN:
                {
                    gpio_direction_output(GPIO_MODE_G_LED, next_state);
                    gpio_direction_output(GPIO_MODE_B_LED, next_state);
                    gpio_direction_output(GPIO_MODE_R_LED, LED_OFF);
                    break;
                }
                case LED_COLOR_PURPLE:
                {
                    gpio_direction_output(GPIO_MODE_R_LED, next_state);
                    gpio_direction_output(GPIO_MODE_B_LED, next_state);
                    gpio_direction_output(GPIO_MODE_G_LED, LED_OFF);
                    break;
                }
                default:
                {
                    printk(KERN_ERR "[LED] ledStateWrite: LED color ERROR!\n");
                    return BSP_ERROR;
                }
        }
    }
    
    printk(KERN_INFO "[LED] ledStateWrite: OUT, LED_GPIO[%d] = %d, next_state= %d\n",
        stCtrlParms.led_id,LED_GPIO[stCtrlParms.led_id], next_state);

    return BSP_OK;
}

/*****************************************************************************
函 数 名  :kernel_led_write
功能描述  :设置LED 灯状态
输入参数  : LED 灯ID，亮或者灭状态LED_ON / LED_OFF 而非LED_STATE_ON/LED_STATE_OFF
输出参数  : 无
调用函数  : 无
被调函数  : 外部接口
History     :
1.日    期  :
  修改内容  : Create
*****************************************************************************/
int kernel_led_write(LED_IOCTL_ARG stCtrlParms, int next_state)
{
    if((LED_ID_SIGNAL1 > stCtrlParms.led_id) || (LED_ID_MODE < stCtrlParms.led_id))
    {
        printk(KERN_ERR "[LED] kernel_led_write: Led type error,LED ID=%d\n",stCtrlParms.led_id);
        return BSP_ERROR;
    }

    if ((next_state != LED_ON) && (next_state != LED_OFF))
    {
        printk(KERN_ERR "[LED] kernel_led_write: Led state error,LED state=%d\n",
            next_state);
        return BSP_ERROR;
    }

    /*更新LED 灯全局变量状态*/
    memcpy((LED_IOCTL_ARG *)&g_current_kerdev_state[stCtrlParms.led_id],
        &stCtrlParms,sizeof(LED_IOCTL_ARG));   
    
    g_led_current_state[stCtrlParms.led_id] = next_state;
    
    if (BSP_OK != ledStateWrite(g_current_kerdev_state[stCtrlParms.led_id], next_state))
    {
        printk(KERN_ERR "[LED] kernel_led_write: ERROR, LED ID=%d, LED state=%d\n",
            stCtrlParms.led_id, next_state);
        return BSP_ERROR;
    }

    return BSP_OK;
}

/*****************************************************************************
函 数 名  :ledStateBlink
功能描述  : 设置LED 灯使其闪烁
输入参数  :KERNELDEV_IOCTL_PARMS_ST stCtrlParms
输出参数  : BSP_ERROR,BSP_OK
调用函数  : 
被调函数  :内部接口
History     :
1.日    期  : 2013-11-13
  作    者  : 王丽00249966
  修改内容  : Create
*****************************************************************************/
int ledStateBlink(LED_IOCTL_ARG stCtrlParms)
{
    printk(KERN_INFO "ledStateBlink IN\n");

    int event_id = APP_LED_SET;
    
    if((LED_ID_SIGNAL1 > stCtrlParms.led_id) || (LED_ID_MODE < stCtrlParms.led_id))
    {
        printk(KERN_ERR "[LED] ledStateBlink: Led type error,LED ID=%d\n",stCtrlParms.led_id);
        return BSP_ERROR;
    }

    /*发送事件给任务处理函数*/
    if (BSP_OK != eventSend(event_id)) /*时间发送函数待修改*/
    {
        printk(KERN_ERR "[LED] ledStateBlink: Send event FAIL!\n");
        return BSP_ERROR;
    }

    return BSP_OK;
}


/*****************************************************************************
函 数 名  :kernel_led_write
功能描述  :设置LED 灯使其闪烁
输入参数  :无
输出参数  : 无
调用函数  : 无
被调函数  : 内部接口
History     :
1.日    期  :2013-11-13
  作    者  : 王丽00249966
  修改内容  : Create
*****************************************************************************/
int kernel_led_blink(LED_IOCTL_ARG stCtrlParms)
{
    if((LED_ID_SIGNAL1 > stCtrlParms.led_id) || (LED_ID_MODE < stCtrlParms.led_id))
    {
        printk(KERN_ERR "[LED] kernel_led_blink: Led type error,LED ID=%d\n",stCtrlParms.led_id);
        return BSP_ERROR;
    }

    if ((stCtrlParms.duration < 0) || (stCtrlParms.second_duration < 0))
    {
        printk(KERN_ERR "[LED] kernel_led_blink: Timeout ERROR\n");
        return BSP_ERROR;
    }

    /*更新LED 灯全局变量状态*/
    memcpy((LED_IOCTL_ARG *)&g_current_kerdev_state[stCtrlParms.led_id],
        &stCtrlParms,sizeof(LED_IOCTL_ARG));      


    if (BSP_OK != ledStateBlink(g_current_kerdev_state[stCtrlParms.led_id]))
    {
        printk(KERN_ERR "[LED] kernel_led_blink: ERROR\n");
        return BSP_ERROR;
    }

    return BSP_OK;
}

/*****************************************************************************
函 数 名  :resource_release
功能描述  :为CPE上LED灯释放资源
输入参数  :无
输出参数  : BSP_ERROR,BSP_OK
调用函数  : 无
被调函数  : 内部接口
History     :
1.日    期  :2014-01-15
  作    者  : 王丽00249966
  修改内容  : Create
*****************************************************************************/
int resource_release(void)
{
    int ret = BSP_ERROR;
    ret = kthread_stop(g_ledTaskID);
    KERNELDEV_TIMER_FREE(&g_kerdevTimer);

    return ret;
}

/*****************************************************************************
函 数 名  :resource_release
功能描述  :为CPE上LED灯分配资源
输入参数  :无
输出参数  : BSP_ERROR,BSP_OK
调用函数  : 无
被调函数  : 内部接口
History     :
1.日    期  :2014-01-15
  作    者  : 王丽00249966
  修改内容  : Create
*****************************************************************************/
int resource_get(void)
{   
    printk(KERN_ERR "[LED] %s: IN\r\n", __func__);
    int i = 0;
    int j = 0;
    int ret = BSP_OK;

    /*LED GPIO 申请(非模式灯)*/
    for (i = LED_ID_SIGNAL1; i <= LED_ID_SLIC; i++)
    {
        if(gpio_request(LED_GPIO[i], ""))
        {
            printk(KERN_ERR "[LED] %s: request gpio%d FAIL!\n", __func__, LED_GPIO[i]);
            ret = BSP_ERROR;
        }
    }

    /*申请模式绿灯GPIO */
    if (gpio_request(GPIO_MODE_G_LED,""))
    {
        printk(KERN_ERR "[LED] %s: request gpio%d FAIL!\n", __func__, GPIO_MODE_G_LED);
        ret = BSP_ERROR;
    }

    /*申请模式红灯GPIO */
    if (gpio_request(GPIO_MODE_R_LED,""))
    {
        printk(KERN_ERR "[LED] %s: request gpio%d FAIL!\n", __func__,GPIO_MODE_R_LED);
        ret = BSP_ERROR;
    }

    /*申请模式蓝灯GPIO */
    if (gpio_request(GPIO_MODE_B_LED,""))
    {
        printk(KERN_ERR "[LED] resource_get: request gpio%d FAIL!\n", GPIO_MODE_B_LED);
        ret = BSP_ERROR;
    }


    /* 按照实际灯数量初始化LED 灯状态*/ 
    for (j = LED_ID_SIGNAL1; j < LED_ID_MAX; j++)
    {
        if (BSP_OK != kernel_led_write(g_current_kerdev_state[j],LED_OFF))
        {
            ret = BSP_ERROR;
        }
    }

    /*初始化使能闪烁监控任务的信号量*/
    sema_init(&g_ledAllBlinkTaskSemId, 0);
    g_ledTaskID = kthread_run(kernelDeviceBlinkTask, NULL, LED_BLINK_TASK_NAME);
    
    printk(KERN_ERR "[LED] %s: OUT!\r\n", __func__);
    return ret;
}


#ifdef __cplusplus
}
#endif

