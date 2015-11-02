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
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/syscalls.h>
#include <asm/unistd.h>
#include <asm/uaccess.h>
#include <linux/errno.h>
#include <linux/miscdevice.h>
#include "bsp_nvim.h"
#include "bsp_pmu.h"
#include "led_dr.h"

#include <linux/err.h>
#include <linux/leds.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/types.h>
#include "bsp_leds.h"
#include "bsp_regulator.h"
#include "bsp_icc.h"
#include "bsp_softtimer.h"
#include <../leds/leds_balong.h>
#include "product_config.h"
/*******************************类型定义区**********************************/
/* DR相关 */

typedef enum
{
    BREATH_DR01 = 1,
    BREATH_DR02 ,
    BREATH_DR03 ,
    BREATH_DR04 ,
    BREATH_DR05 ,
    BREATH_DR_BOTTOM
}breathled_id_e;

/* 呼吸灯使能开关 */
typedef enum
{
    BREATH_LED_BREATH    = 0,  /* LED灯呼吸模式打开 */
    BREATH_LED_TIMER     = 1,  /* LED灯闪烁模式打开 */
    BREATH_LED_NONE      = 2,  /* LED灯关闭 */
    BREATH_LED_BOTTOM, 
}breath_led_ctrl;

typedef struct
{
    unsigned char led_enable;        /* LED灯使能开关 */
    unsigned char led_dr;            /* LED灯所使用的DR */
    unsigned char led_mode;          /* LED灯模式标志 */
    unsigned char led_reserve;        /* LED灯模式标志 */
    unsigned int  full_on;           /* LED灯呼吸模式稳定亮的持续时间 */
    unsigned int  full_off;          /* LED灯呼吸模式稳定暗的持续时间 */
    unsigned int  fade_on;           /* LED灯呼吸模式从暗到亮的持续时间 */
    unsigned int  fade_off;          /* LED灯呼吸模式从亮到暗的持续时间 */
    unsigned int  delay_on;          /* LED灯闪烁模式亮的时间 */
    unsigned int  delay_period;     /* LED灯闪烁模式的闪烁周期时间*/
    unsigned int  full_long_on;     /* LED灯长亮的持续时间 */
    unsigned int  full_long_off;    /* LED灯长暗的持续时间 */
    unsigned int  brightness;       /* LED灯亮度电流值 */
}NV_LED_PARA_STRU;

/* 属性 */
typedef enum
{
    BRT_ATTR_FULL_ON = 1,    /* LED灯稳定亮持续时间属性 */
    BRT_ATTR_FULL_OFF,       /* LED灯稳定灭持续时间属性 */
    BRT_ATTR_FADE_ON,        /* LED灯由暗变亮持续时间属性 */
    BRT_ATTR_FADE_OFF,       /* LED灯由亮变暗持续时间属性 */
    BRT_ATTR_DELAY_ON,       /* LED灯闪烁亮持续时间属性 */
    BRT_ATTR_DELAY_OFF,      /* LED灯闪烁灭持续时间属性 */
    BRT_ATTR_BRIGHTNESS,     /* LED灯闪亮度值属性 */
    BRT_ATTR_B0TTOM
}breathled_attr_e;
/*******************************宏定义区**********************************/

#define ZERO_TIME  0

/* 呼吸灯使能 */
#define BREATH_LED_ENABLE   1   /* 呼吸灯使能 */
#define BREATH_LED_DISABLE  0   /* 呼吸灯非使能 */

/* 呼吸灯模式 */
#define LED_MODE_BREATH     0   /* 呼吸灯呼吸模式 */

#define LED_MODE_BLINK      1   /* 呼吸灯闪烁模式 */

#define NV_ID_LED_PARA  50455
/* 呼吸灯当前状态 */
PRIVATE breathled_state breath_led_curr_state  = BREATH_LED_STATE_SPARK;

#define ATTR_INFO_LEN 20
#define CONTAINER_LEN 200
#define LED_MAX   32           /*最大可增加的led 节点数*/

/* 呼吸灯各属性名称 */
#define BREATH_TRIGGER_BREATH     "breath"
#define BREATH_TRIGGER_TIMER      "timer"
#define BREATH_TRIGGER_NONE       "none"

#define BREATH_TRIGGER            "trigger"
#define BREATH_FULL_ON            "full_on"
#define BREATH_FULL_OFF           "full_off"
#define BREATH_FADE_ON            "fade_on"
#define BREATH_FADE_OFF           "fade_off"
#define BREATH_DELAY_ON           "delay_on"
#define BREATH_DELAY_OFF          "delay_off"
#define BREATH_BRIGHTNESS         "brightness"
#define LED_LONG_TIME             5000000    /*长亮、长暗寄存器固定配置值*/

#define DR12_BRIGHTNESS_3MA    31       /*DR12输出3mA电流对应的亮度值*/
#define DR345_BRIGHTNESS_3MA    170     /*DR345输出3mA电流对应的亮度值*/

#ifdef BSP_CONFIG_BOARD_E5
#define DR345_BRIGHTNESS_4_5MA    255     /*DR345输出4.5mA电流对应的亮度值*/
#endif

#define LED_NAME_1  1      /*海思提供的Balong_dr1节点*/
#define LED_NAME_2  2      /*海思提供的Balong_dr2节点*/
#define LED_NAME_3  3      /*海思提供的Balong_dr3节点*/
#define LED_NAME_4  4      /*海思提供的Balong_dr4节点*/
#define LED_NAME_5  5      /*海思提供的Balong_dr5节点*/

PRIVATE char path_container[CONTAINER_LEN];
PRIVATE const char* container_format   = "/sys/devices/platform/balong_led/leds/Balong_dr%d/%s";
int  g_dr_name = 0;   
int g_dr_brightness[5] = {-1, -1, -1, -1, -1};     /*当前的亮度标识*/
extern struct workqueue_struct *led_workqueue_brightness;    /*亮灭控制队列*/

/*******************************函数声明区**********************************/
extern int led_dr_regulator_enable(int led_id);

PUBLIC int breathled_controller(breathled_state led_state);

PRIVATE int breathled_hold(breath_led_ctrl led_ctrl);
PRIVATE int set_breathled_attr(breathled_attr_e br_attr, unsigned long attr_value);
PRIVATE int assemble_container(const char * cmd_obj);
PRIVATE int breathled_breath(void);
PRIVATE int breathled_shine(void);
PRIVATE int breathled_sleep(void);
PRIVATE int breathled_disable(void);
PRIVATE int breathled_nv_init(void);
PRIVATE int breathled_nv_save(void);

/*******************************变量定义区**********************************/

/* 呼吸灯各属性值说明 */
PRIVATE NV_LED_PARA_STRU led_attr_default =
{
    .led_enable    = 1,          /* LED灯使能开关 ，默认使能*/
    .led_dr        = 2,          /* LED灯所使用的DR，默认Balong_dr2 */
    .led_mode      = 0,          /* LED灯模式标志，默认呼吸模式 */
    .full_on       = 500000,     /* LED灯稳定亮的持续时间 */
    .full_off      = 500000,     /* LED灯稳定暗的持续时间 */
    .fade_on       = 2000000,    /* LED灯从暗到亮的持续时间 */
    .fade_off      = 2000000,    /* LED灯从亮到暗的持续时间 */
    .delay_on      = 101569,     /* LED灯闪烁亮的持续时间 */
    .delay_period  = 218750,     /* LED灯闪烁周期时间 */
    .full_long_on  = 5000000,    /* LED灯长亮 */
    .full_long_off = 5000000,    /* LED灯长暗 */
    .brightness    = 30,         /* LED灯亮度电流值，此属性值对应 3mA */
};

/*lint -e54 -e119 -e30 -e84 -e514 -e18 -e628*/
 struct led_platform_data led_dr_plat_data = {
    .num_leds   = ARRAY_SIZE(led_dr),
    .leds       = (struct led_info *)led_dr,
};
/*lint +e54 +e119 +e30 +e84 +e514 -e18 +628*/

static struct platform_device led_dr_dev = {    
    .name      = "led_dr",   
    .id             = -1,    
    .dev ={       
                  .platform_data =  &led_dr_plat_data,
        },
    };

static DEFINE_MUTEX(led_dr_mutex); /* LED操作锁 */

/*******************************函数实现区**********************************/

/************************************************************************
 *函数原型 ： PRIVATE int breathled_nv_save(void)
 *描述     ： 呼吸灯控制参数NV读取初始化
 *输入     ： 无
 *输出     ： 无
 *返回值   ： BR_OK OR BR_ERRO
 *修改历史     :
 1.日    期   : 20131205
   修改内容   : 新生成函数
*************************************************************************/
PRIVATE int breathled_nv_save(void)
{   
    int ret = 0;

    printk(KERN_DEBUG"[*Breath-Led*] %s: led parameter nv save.\n", __func__);
    
    ret = bsp_nvm_write(NV_ID_LED_PARA, (unsigned char *)(&led_attr_default), sizeof(led_attr_default));
    if(NV_OK != ret)
    {
        printk(KERN_ERR"[*Breath-Led*] %s: led parameter nv write fail.\n", __func__);
        return BR_ERROR;
    }

    return BR_OK;
}

/************************************************************************
 *函数原型 ： PRIVATE int breathled_nv_init(void)
 *描述     ： 呼吸灯控制参数NV读取初始化
 *输入     ： 无
 *输出     ： 无
 *返回值   ： BR_OK OR BR_ERRO
 *修改历史     :
 1.日    期   : 20131205
   修改内容   : 新生成函数
*************************************************************************/
PRIVATE int breathled_nv_init(void)
{   
    int ret = 0;

    printk(KERN_DEBUG"[*Breath-Led*] %s: led parameter nv init.\n", __func__);
    
    ret = bsp_nvm_read(NV_ID_LED_PARA, (unsigned char *)(&led_attr_default), sizeof(led_attr_default));
    if(NV_OK != ret)
    {
        printk(KERN_ERR"[*Breath-Led*] %s: led parameter nv read fail.\n", __func__);
        return BR_ERROR;
    }

    printk(KERN_DEBUG"[*Breath-Led*] %s: led_attr_default.led_enable = %d.\n", __func__, led_attr_default.led_enable);
    printk(KERN_DEBUG"[*Breath-Led*] %s: led_attr_default.led_dr = %d.\n", __func__, led_attr_default.led_dr);
    printk(KERN_DEBUG"[*Breath-Led*] %s: led_attr_default.led_mode = %d.\n", __func__, led_attr_default.led_mode);
    printk(KERN_DEBUG"[*Breath-Led*] %s: led_attr_default.full_on = %u.\n", __func__, led_attr_default.full_on);
    printk(KERN_DEBUG"[*Breath-Led*] %s: led_attr_default.full_off = %u.\n", __func__, led_attr_default.full_off);
    printk(KERN_DEBUG"[*Breath-Led*] %s: led_attr_default.fade_on = %u.\n", __func__, led_attr_default.fade_on);
    printk(KERN_DEBUG"[*Breath-Led*] %s: led_attr_default.fade_off = %u.\n", __func__, led_attr_default.fade_off);
    printk(KERN_DEBUG"[*Breath-Led*] %s: led_attr_default.delay_on = %u.\n", __func__, led_attr_default.delay_on);
    printk(KERN_DEBUG"[*Breath-Led*] %s: led_attr_default.delay_period = %u.\n", __func__, led_attr_default.delay_period);
    printk(KERN_DEBUG"[*Breath-Led*] %s: led_attr_default.full_long_on = %u.\n", __func__, led_attr_default.full_long_on);
    printk(KERN_DEBUG"[*Breath-Led*] %s: led_attr_default.full_long_off = %u.\n", __func__, led_attr_default.full_long_off);
    printk(KERN_DEBUG"[*Breath-Led*] %s: led_attr_default.brightness = %u.\n", __func__, led_attr_default.brightness);
    
    return BR_OK;
}

/************************************************************************
 *函数原型 ： static int assemble_container(const char * cmd_obj)
 *描述     ： 属性路径容器填充函数
 *输入     ： 无
 *输出     ： 无
 *返回值   ： BR_OK OR BR_ERRO
 *修改历史     :
 1.日    期   : 20131205
   修改内容   : 新生成函数
*************************************************************************/
PRIVATE int assemble_container(const char * cmd_obj)
{   
    int len = 0;
    
    if(!cmd_obj)
    {
        LED_TRACE(LED_DEBUG_LEVEL(ERROR),"[%s] arguments is NULL\n", __FUNCTION__);
        return BR_ERROR;
    }
    /* 置空容器 */
    (void)memset(path_container, 0x0, CONTAINER_LEN);/*lint !e506*/

    if(BREATH_DR_BOTTOM <= led_attr_default.led_dr || BREATH_DR01 > led_attr_default.led_dr)
    {
        printk(KERN_ERR"[*Breath-Led*] %s: PMU DR selected %d error.\n", __func__, led_attr_default.led_dr);
        return BR_ERROR;
    }
    
    /* 填充容器 */
    len = snprintf(path_container, CONTAINER_LEN, container_format, led_attr_default.led_dr, cmd_obj);
    if(len < 0)
    {
        printk(KERN_ERR"[*Breath-Led*] %s: assemble %s failed.\n", __func__, cmd_obj);
        return BR_ERROR;
    }

    return BR_OK;
}

/************************************************************************
 *函数原型 ： static int breathled_hold()
 *描述     ： 呼吸灯 打开函数
 *输入     ： 无
 *输出     ： 无
 *返回值   ： BR_OK OR BR_ERRO
 *修改历史     :
 1.日    期   : 20131205
   修改内容   : 新生成函数
*************************************************************************/
PRIVATE int breathled_hold(breath_led_ctrl led_ctrl)
{
    struct file *attr_handle = NULL;
    int ret = BR_ERROR;
    int len = -1;
    mm_segment_t old_fs = get_fs();

    if(BREATH_LED_BOTTOM <= led_ctrl)
    {
        printk(KERN_ERR"[*Breath-Led*] %s: para %d invalid.\n", __func__, led_ctrl);
        return BR_ERROR;
    }
    
    /* 初始化属性路径 */
    ret = assemble_container(BREATH_TRIGGER);
    if(BR_ERROR == ret)
    {
        return BR_ERROR;
    }

    set_fs(KERNEL_DS);

    /* 打开属性文件 */
    attr_handle = filp_open(path_container, O_RDWR, S_IRWXU);
    if(IS_ERR(attr_handle))
    {
        printk(KERN_ERR"[*Breath-Led*] %s: sys_open \"%s\" failed.\n", __func__,path_container);
        set_fs(old_fs);
        return BR_ERROR;
    }

    /* 写入属性值 */
    switch( led_ctrl )
    {
        case BREATH_LED_BREATH:
            /* 使能呼吸灯呼吸 */
            len = attr_handle->f_op->write(attr_handle, 
                                           BREATH_TRIGGER_BREATH, 
                                           sizeof(BREATH_TRIGGER_BREATH), 
                                           &attr_handle->f_pos);
            break;
        case BREATH_LED_TIMER:
            /* 使能呼吸灯闪烁 */
            len = attr_handle->f_op->write(attr_handle, 
                                           BREATH_TRIGGER_TIMER, 
                                           sizeof(BREATH_TRIGGER_TIMER), 
                                           &attr_handle->f_pos);
            break;
        case BREATH_LED_NONE:
            /* 关闭呼吸灯 */
            len = attr_handle->f_op->write(attr_handle, 
                                           BREATH_TRIGGER_NONE, 
                                           sizeof(BREATH_TRIGGER_NONE), 
                                           &attr_handle->f_pos);
            break;
        default:
            break;
    }
    
    if(len < 0) 
    {
        printk(KERN_ERR"[*Breath-Led*] %s: sys_write \"%s\" failed.\n", __func__,path_container);
        (void)filp_close(attr_handle, NULL);
        set_fs(old_fs);
        return BR_ERROR;
    }
    
    (void)filp_close(attr_handle, NULL);
    set_fs(old_fs);
    
    return BR_OK;
}


/************************************************************************
 *函数原型 ： PRIVATE int set_breathled_attr(breathled_attr_e br_attr, unsigned long attr_value)
 *描述     ： 呼吸灯属性设置函数
 *输入     ： 无
 *输出     ： 无
 *返回值   ： BR_OK OR BR_ERRO
 *修改历史     :
 1.日    期   : 20131205
   修改内容   : 新生成函数
*************************************************************************/
PRIVATE int set_breathled_attr(breathled_attr_e br_attr, unsigned long attr_value)
{
    struct file *attr_handle = NULL;
    int ret = BR_ERROR;
    int len = 0;
    /* 定义属性值数组 */
    char value_buf[ATTR_INFO_LEN];
    /* 定义属性名数组 */
    char attr_buf[ATTR_INFO_LEN];
    mm_segment_t old_fs = get_fs();
    
    (void)memset(value_buf, 0x0, sizeof(value_buf));/*lint !e506*/
    (void)memset(attr_buf, 0x0, sizeof(attr_buf));/*lint !e506*/
    switch(br_attr)
    {
        case BRT_ATTR_FULL_ON:
            /* 属性信息填充 */
            strncpy(attr_buf, BREATH_FULL_ON, sizeof(BREATH_FULL_ON));
            break;
        case BRT_ATTR_FULL_OFF:
            /* 属性信息填充 */
            strncpy(attr_buf, BREATH_FULL_OFF, sizeof(BREATH_FULL_OFF));
            break;
        case BRT_ATTR_FADE_ON:
            /* 属性信息填充 */
            strncpy(attr_buf, BREATH_FADE_ON, sizeof(BREATH_FADE_ON));
            break;
        case BRT_ATTR_FADE_OFF:
            /* 属性信息填充 */
            strncpy(attr_buf, BREATH_FADE_OFF, sizeof(BREATH_FADE_OFF));
            break;
        case BRT_ATTR_DELAY_ON:
            /* 属性信息填充 */
            strncpy(attr_buf, BREATH_DELAY_ON, sizeof(BREATH_DELAY_ON));
            break;
        case BRT_ATTR_DELAY_OFF:
            /* 属性信息填充 */
            strncpy(attr_buf, BREATH_DELAY_OFF, sizeof(BREATH_DELAY_OFF));
            break;
        case BRT_ATTR_BRIGHTNESS:
            /* 属性信息填充 */
            strncpy(attr_buf, BREATH_BRIGHTNESS, sizeof(BREATH_BRIGHTNESS));
            break;
        default:
            /* 属性设置错误 */
            printk(KERN_ERR"%s: BREATH ATTR error.\n", __func__);
            return BR_ERROR;
    }

    /* 属性信息组装 */
    ret = assemble_container(attr_buf);
    if(BR_ERROR == ret)
    {
        return BR_ERROR;
    }

    /* 属性值填充 */
    len = snprintf(value_buf, sizeof(value_buf), "%ld", attr_value);
    if(len < 0) 
    {
        printk(KERN_ERR"[*Breath-Led*] %s: fill value buffer failded.\n", __func__);
        set_fs(old_fs);
        return BR_ERROR;
    }

    set_fs(KERNEL_DS);

    /* 打开属性文件 */
    attr_handle = filp_open(path_container, O_RDWR, S_IRWXU);
    if(IS_ERR(attr_handle))
    {
        printk(KERN_ERR"[*Breath-Led*] %s: sys_open \"%s\" failed.\n", __func__,path_container);
        set_fs(old_fs);
        return BR_ERROR;
    }

    /* 写入属性值 */
    len = attr_handle->f_op->write(attr_handle, value_buf, len, &attr_handle->f_pos);
    if(len < 0) 
    {
        printk(KERN_ERR"[*Breath-Led*] %s: sys_write \"%s\" failed.\n", __func__,path_container);
        (void)filp_close(attr_handle, NULL);
        set_fs(old_fs);
        return BR_ERROR;
    }
    
    (void)filp_close(attr_handle, NULL);
    set_fs(old_fs);
    
    return BR_OK;
}

/************************************************************************
 *函数原型 ： static int breathled_breath()
 *描述     ： 呼吸灯 开始呼吸函数
 *输入     ： 无
 *输出     ： 无
 *返回值   ： BR_OK OR BR_ERRO
 *修改历史     :
 1.日    期   : 20131205
   修改内容   : 新生成函数
*************************************************************************/
PRIVATE int breathled_breath(void)
{
    int ret = BR_ERROR;
    
    /* 开始呼吸 */
    ret = breathled_hold (BREATH_LED_BREATH);
    if(BR_ERROR == ret)
    {
        return ret;
    }
    ret = set_breathled_attr(BRT_ATTR_FULL_ON, led_attr_default.full_on);
    if(BR_ERROR == ret)
    {
        return ret;
    }
    ret = set_breathled_attr(BRT_ATTR_FULL_OFF, led_attr_default.full_off);
    if(BR_ERROR == ret)
    {
        return ret;
    }
    ret = set_breathled_attr(BRT_ATTR_FADE_ON, led_attr_default.fade_on);
    if(BR_ERROR == ret)
    {
        return ret;
    }
    ret = set_breathled_attr(BRT_ATTR_FADE_OFF, led_attr_default.fade_off);
    if(BR_ERROR == ret)
    {
        return ret;
    }
    ret = set_breathled_attr(BRT_ATTR_BRIGHTNESS, led_attr_default.brightness);
    if(BR_ERROR == ret)
    {
        return ret;
    }
    
    return BR_OK;
}

/* 保留接口，清除编译告警 */
#if 0
/************************************************************************
 *函数原型 ： static int breathled_blink()
 *描述     ： 呼吸灯 开始闪烁函数
 *输入     ： 无
 *输出     ： 无
 *返回值   ： BR_OK OR BR_ERRO
 *修改历史     :
 1.日    期   : 20131205
   修改内容   : 新生成函数
*************************************************************************/
PRIVATE int breathled_blink(void)
{
    int ret = BR_ERROR;
    
    /* 开始闪烁 */
    ret = breathled_hold (BREATH_LED_TIMER);
    if(BR_ERROR == ret)
    {
        return ret;
    }
    ret = set_breathled_attr(BRT_ATTR_DELAY_ON, led_attr_default.delay_on);
    if(BR_ERROR == ret)
    {
        return ret;
    }
    if(led_attr_default.delay_on > led_attr_default.delay_period)
    {
        printk(KERN_ERR"[*Breath-Led*] %s: blink on time greater than blink period time.\n", __func__);
        return BR_ERROR;
    }
    ret = set_breathled_attr(BRT_ATTR_DELAY_OFF, led_attr_default.delay_period - led_attr_default.delay_on);
    if(BR_ERROR == ret)
    {
        return ret;
    }
    ret = set_breathled_attr(BRT_ATTR_BRIGHTNESS);
    if(BR_ERROR == ret)
    {
        return ret;
    }
    
    return BR_OK;
}
#endif

/************************************************************************
 *函数原型 ： static int breathled_shine()
 *描述     ： 呼吸灯 长亮函数
 *输入     ： 无
 *输出     ： 无
 *返回值   ： BR_OK OR BR_ERRO
 *修改历史     :
 1.日    期   : 20131205
   修改内容   : 新生成函数
*************************************************************************/
PRIVATE int breathled_shine(void)
{
    int ret = BR_ERROR;

    /* 开始长亮 */
    ret = breathled_hold (BREATH_LED_BREATH);
    if(BR_ERROR == ret)
    {
        return ret;
    }
    ret = set_breathled_attr(BRT_ATTR_FULL_ON, led_attr_default.full_long_on);
    if(BR_ERROR == ret)
    {
        return ret;
    }
    ret = set_breathled_attr(BRT_ATTR_FULL_OFF, ZERO_TIME);
    if(BR_ERROR == ret)
    {
        return ret;
    }
    ret = set_breathled_attr(BRT_ATTR_FADE_ON, ZERO_TIME);
    if(BR_ERROR == ret)
    {
        return ret;
    }
    ret = set_breathled_attr(BRT_ATTR_FADE_OFF, ZERO_TIME);
    if(BR_ERROR == ret)
    {
        return ret;
    }
    ret = set_breathled_attr(BRT_ATTR_BRIGHTNESS, led_attr_default.brightness);
    if(BR_ERROR == ret)
    {
        return ret;
    }
    
    return BR_OK;
}

/************************************************************************
 *函数原型 ： static int breathled_sleep()
 *描述     ： 呼吸灯 长灭函数
 *输入     ： 无
 *输出     ： 无
 *返回值   ： BR_OK OR BR_ERRO
 *修改历史     :
 1.日    期   : 20131205
   修改内容   : 新生成函数
*************************************************************************/
PRIVATE int breathled_sleep(void)
{
    int ret = BR_ERROR;

    /* 开始长暗 */
    ret = breathled_hold (BREATH_LED_BREATH);
    if(BR_ERROR == ret)
    {
        return ret;
    }
    ret = set_breathled_attr(BRT_ATTR_FULL_ON, ZERO_TIME);
    if(BR_ERROR == ret)
    {
        return ret;
    }
    ret = set_breathled_attr(BRT_ATTR_FULL_OFF, led_attr_default.full_long_off);
    if(BR_ERROR == ret)
    {
        return ret;
    }
    ret = set_breathled_attr(BRT_ATTR_FADE_ON, ZERO_TIME);
    if(BR_ERROR == ret)
    {
        return ret;
    }
    ret = set_breathled_attr(BRT_ATTR_FADE_OFF, ZERO_TIME);
    if(BR_ERROR == ret)
    {
        return ret;
    }
    ret = set_breathled_attr(BRT_ATTR_BRIGHTNESS, led_attr_default.brightness);
    if(BR_ERROR == ret)
    {
        return ret;
    }
    
    return BR_OK;
}

/************************************************************************
 *函数原型 ： static int breathled_disable()
 *描述     ： 呼吸灯 关闭函数
 *输入     ： 无
 *输出     ： 无
 *返回值   ： BR_OK OR BR_ERRO
 *修改历史     :
 1.日    期   : 20131205
   修改内容   : 新生成函数
*************************************************************************/
PRIVATE int breathled_disable(void)
{
    int ret = BR_ERROR;

    /* 关闭呼吸灯 */
    ret = breathled_hold (BREATH_LED_NONE);
    if(BR_ERROR == ret)
    {
        return ret;
    }

    return BR_OK;
}

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
PUBLIC int breathled_controller(breathled_state led_state)
{
    int ret = BR_ERROR;
    
    printk(KERN_DEBUG"[*Breath-Led*] %s: breathle curr state = %d.\n", __func__, breath_led_curr_state);
    printk(KERN_DEBUG"[*Breath-Led*] %s: breathle next state = %d.\n", __func__, led_state);
    
    switch( led_state )
    {
        case BREATH_LED_STATE_ENABLE:
            /* 呼吸灯使能 */
            led_attr_default.led_enable = BREATH_LED_ENABLE;
            /* 呼吸灯使能状态固化 */
            ret = breathled_nv_save();
            if(BR_ERROR == ret)
            {
                return ret;
            }
            
            ret = breathled_controller(breath_led_curr_state);
            if(BR_ERROR == ret)
            {
                return ret;
            }
            break;
        case BREATH_LED_STATE_DISABLE:
            /* 呼吸灯关闭 */
            led_attr_default.led_enable = BREATH_LED_DISABLE;
            /* 呼吸灯使能状态固化 */
            ret = breathled_nv_save();
            if(BR_ERROR == ret)
            {
                return ret;
            }
            
            ret = breathled_disable();
            if(BR_ERROR == ret)
            {
                return ret;
            }
            break;
        case BREATH_LED_STATE_SPARK:
            /* 呼吸模式 */
            if(LED_MODE_BREATH != led_attr_default.led_mode)
            {
                /* 呼吸灯配置非呼吸模式直接返回 */
                return BR_ERROR;
            }
            
            breath_led_curr_state = BREATH_LED_STATE_SPARK;
            if(BREATH_LED_DISABLE == led_attr_default.led_enable)
            {
                /* 呼吸灯关闭时直接返回 */
                return BR_OK;
            }
            
            ret = breathled_breath();
            if(BR_ERROR == ret)
            {
                return ret;
            }
            break;
        case BREATH_LED_STATE_LIGHT:
            /* 开始长亮 */
            breath_led_curr_state = BREATH_LED_STATE_LIGHT;
            if(BREATH_LED_DISABLE == led_attr_default.led_enable)
            {
                /* 呼吸灯关闭时直接返回 */
                return BR_OK;
            }
            
            ret = breathled_shine();
            if(BR_ERROR == ret)
            {
                return ret;
            }
            break;
        case BREATH_LED_STATE_SLEEP:
            /* 开始长暗 */
            breath_led_curr_state = BREATH_LED_STATE_SLEEP;
            if(BREATH_LED_DISABLE == led_attr_default.led_enable)
            {
                /* 呼吸灯关闭时直接返回 */
                return BR_OK;
            }
            
            ret = breathled_sleep();
            if(BR_ERROR == ret)
            {
                return ret;
            }
            break;
        default:
            printk(KERN_ERR"[*Breath-Led*] %s: breathle state cmd is invalid.\n", __func__);
            return BR_ERROR;
            break;
    }

    return BR_OK;
}

EXPORT_SYMBOL (breathled_controller);

PRIVATE int __init breathled_init(void)
{
    int ret = 0;
    
    ret = breathled_nv_init();

    return ret;
}

PRIVATE void __exit breathled_exit(void)
{
    int ret = 0;
    
    ret = breathled_controller (BREATH_LED_STATE_DISABLE);
    if(BR_ERROR == ret)
    {
        printk(KERN_ERR"[*Breath-Led*] %s: breathled_controller failed.\n", __func__);
    }
}

/************************************************************************
 *函数原型 ： int change_name(struct led_classdev *led_cdev)
 *描述     ： 将用户态操作的节点名称转换成海思注册的Balong_dr*的名称,用于访问海思注册的dr灯节点
 *输入     ： 用户态操作的节点信息
 *输出     ： 无
 *返回值   ： BR_OK OR BR_ERRO
 *修改历史     :
 1.日    期   : 20140620
   修改内容   : 新生成函数
*************************************************************************/
int change_name(struct led_classdev *led_cdev)
{
    /* reject null */
    if(!led_cdev || !led_cdev->name)
    {
        LED_TRACE(LED_DEBUG_LEVEL(ERROR),"[%s] arguments is NULL\n", __FUNCTION__);
        return BR_ERROR;
    }

    LED_TRACE(LED_DEBUG_LEVEL(INFO),"[%s] led name %s\n", __FUNCTION__, led_cdev->name);

    if(!strcmp(led_cdev->name, LED_NODE_1))
    {
        g_dr_name = LED_NAME_1;
        return BR_OK;
    }
    else if(!strcmp(led_cdev->name, LED_NODE_2))
    {
        g_dr_name = LED_NAME_2;
        return BR_OK;
    }
    else if(!strcmp(led_cdev->name, LED_NODE_3))
    {
        g_dr_name = LED_NAME_3;
        return BR_OK;
    }
    else if(!strcmp(led_cdev->name, LED_NODE_4))
    {
        g_dr_name = LED_NAME_4;
        return BR_OK;
    }
    else if(!strcmp(led_cdev->name, LED_NODE_5))
    {
        g_dr_name = LED_NAME_5;
        return BR_OK;
    }
    else
    {
        LED_TRACE(LED_DEBUG_LEVEL(ERROR),"[%s] match the led name failedn", __FUNCTION__);
        return BR_ERROR;
    }
    
}

/************************************************************************
 *函数原型 ： static int assemble_container_dr(const char * cmd_obj)
 *描述     ： 属性路径容器填充函数
 *输入     ： cmd_obj   需要填充的属性
 *输出     ： 无
 *返回值   ： BR_OK OR BR_ERRO
 *修改历史     :
 1.日    期   : 20140620
   修改内容   : 新生成函数
*************************************************************************/
PRIVATE int assemble_container_dr(const char * cmd_obj)
{   
    int len = 0;

    if(!cmd_obj)
    {
        LED_TRACE(LED_DEBUG_LEVEL(ERROR),"[%s] arguments is NULL\n", __FUNCTION__);
        return BR_ERROR;
    }
    
    /* 置空容器 */
    (void)memset(path_container,0x0, CONTAINER_LEN);/*lint !e160 !e506*/

    /* 填充容器 */
    len = snprintf(path_container, CONTAINER_LEN, container_format, g_dr_name, cmd_obj);
    if(len < 0)
    {
        printk(KERN_ERR"[*Breath-Led*] %s: assemble %s failed.\n", __func__, cmd_obj);
        return BR_ERROR;
    }

    return BR_OK;
}
/************************************************************************
 *函数原型 ： PRIVATE int set_led_dr_attr(breathled_attr_e br_attr, unsigned long attr_value)
 *描述     ： 用户态使用的DR灯属性设置函数
 *输入     ： br_attr        节点属性
 *输出     ： attr_value      属性值
 *返回值   ： BR_OK OR BR_ERRO
 *修改历史     :
 1.日    期   : 20140620
   修改内容   : 新生成函数
*************************************************************************/
PRIVATE int set_led_dr_attr(breathled_attr_e br_attr, unsigned long attr_value)
{
    struct file *attr_handle = NULL;
    int ret = BR_ERROR;
    int len = 0;
   
    char value_buf[ATTR_INFO_LEN];
    
    char attr_buf[ATTR_INFO_LEN];
    mm_segment_t old_fs = get_fs();
    /*lint -e160 -e506*/
    (void)memset(value_buf,0x0, sizeof(value_buf));
    (void)memset(attr_buf,0x0, sizeof(attr_buf));
    /*lint +e160 +e506*/
    switch(br_attr)
    {
        case BRT_ATTR_FULL_ON:
         
            strncpy(attr_buf, BREATH_FULL_ON, sizeof(BREATH_FULL_ON));
            break;
        case BRT_ATTR_FULL_OFF:
          
            strncpy(attr_buf, BREATH_FULL_OFF, sizeof(BREATH_FULL_OFF));
            break;
        case BRT_ATTR_FADE_ON:
          
            strncpy(attr_buf, BREATH_FADE_ON, sizeof(BREATH_FADE_ON));
            break;
        case BRT_ATTR_FADE_OFF:
            
            strncpy(attr_buf, BREATH_FADE_OFF, sizeof(BREATH_FADE_OFF));
            break;
        case BRT_ATTR_DELAY_ON:
           
            strncpy(attr_buf, BREATH_DELAY_ON, sizeof(BREATH_DELAY_ON));
            break;
        case BRT_ATTR_DELAY_OFF:
           
            strncpy(attr_buf, BREATH_DELAY_OFF, sizeof(BREATH_DELAY_OFF));
            break;
        case BRT_ATTR_BRIGHTNESS:
            
            strncpy(attr_buf, BREATH_BRIGHTNESS, sizeof(BREATH_BRIGHTNESS));
            break;
        default:
           
            printk(KERN_ERR"%s: BREATH ATTR error.\n", __func__);
            return BR_ERROR;
    }


    ret = assemble_container_dr(attr_buf);
    if(BR_ERROR == ret)
    {
        return BR_ERROR;
    }

    len = snprintf(value_buf, sizeof(value_buf), "%ld", attr_value);
    if(len < 0) 
    {
        printk(KERN_ERR"[*Breath-Led*] %s: fill value buffer failded.\n", __func__);
        set_fs(old_fs);
        return BR_ERROR;
    }

    set_fs(KERNEL_DS);

    attr_handle = filp_open(path_container, O_RDWR, S_IRWXU);
    if(IS_ERR(attr_handle))
    {
        printk(KERN_ERR"[*Breath-Led*] %s: sys_open \"%s\" failed.\n", __func__,path_container);
        set_fs(old_fs);
        return BR_ERROR;
    }

    len = attr_handle->f_op->write(attr_handle, value_buf, len, &attr_handle->f_pos);
    if(len < 0) 
    {
        printk(KERN_ERR"[*Breath-Led*] %s: sys_write \"%s\" failed.\n", __func__,path_container);
        (void)filp_close(attr_handle, NULL);
        set_fs(old_fs);
        return BR_ERROR;
    }
    
    (void)filp_close(attr_handle, NULL);
    set_fs(old_fs);
    
    return BR_OK;
}
/************************************************************************
 *函数原型 ： int led_dr_trigger_hold()
 *描述     ： 设置dr灯的属性为呼吸模式
 *输入     ： 无
 *输出     ： 无
 *返回值   ： BR_OK OR BR_ERRO
 *修改历史     :
 1.日    期   : 20140620
   修改内容   : 新生成函数
*************************************************************************/
int led_dr_trigger_hold(void)
{
    struct file *attr_handle = NULL;
    int ret = BR_ERROR;
    int len = -1;
    mm_segment_t old_fs = get_fs();

    ret = assemble_container_dr(BREATH_TRIGGER);
    if(BR_ERROR == ret)
    {
        return BR_ERROR;
    }

    set_fs(KERNEL_DS);

     attr_handle = filp_open(path_container, O_RDWR, S_IRWXU);
    if(IS_ERR(attr_handle))
    {
        printk(KERN_ERR"[*Breath-Led*] %s: sys_open \"%s\" failed.\n", __func__,path_container);
        set_fs(old_fs);
        return BR_ERROR;
    }
    len = attr_handle->f_op->write(attr_handle, BREATH_TRIGGER_BREATH, 
                                                sizeof(BREATH_TRIGGER_BREATH), &attr_handle->f_pos);           
    if(len < 0) 
    {
        printk(KERN_ERR"[*Breath-Led*] %s: sys_write \"%s\" failed.\n", __func__,path_container);
        (void)filp_close(attr_handle, NULL);
        set_fs(old_fs);
        return BR_ERROR;
    }
    
    (void)filp_close(attr_handle, NULL);
    set_fs(old_fs);
    
    return BR_OK;
}
/************************************************************************
 *函数原型 ： int led_dr_breath_set(struct led_classdev *led_cdev, unsigned long *full_on, unsigned long *full_off, unsigned long *fade_on, unsigned long *fade_off)
 *描述     ： 用户态操作DR灯开始呼吸的接口
 *输入     ： 无
 *输出     ： 无
 *返回值   ： BR_OK OR BR_ERRO
 *修改历史     :
 1.日    期   : 20140620
   修改内容   : 新生成函数
*************************************************************************/
int led_dr_breath_set(struct led_classdev *led_cdev, unsigned long *full_on, unsigned long *full_off, unsigned long *fade_on, unsigned long *fade_off)
{   
    int ret = BR_ERROR;

    /* argument check */
    if((!led_cdev) || (!full_on) || (!full_off) || (!fade_on) || (!fade_off))
    {
        LED_TRACE(LED_DEBUG_LEVEL(ERROR),"[%s] arguments is NULL\n", __FUNCTION__);
        return BR_ERROR;
    }
    led_cdev->breath_full_on = *full_on;
    led_cdev->breath_full_off = *full_off;
    led_cdev->breath_fade_on = *fade_on;
    led_cdev->breath_fade_off = *fade_off;
    
    ret = change_name(led_cdev);
    if(BR_ERROR == ret)
    {
        return ret;
    }
    
    g_dr_brightness[g_dr_name - 1] = -1;        /*亮度值复位*/
    
    ret = led_dr_trigger_hold();
    if(BR_ERROR == ret)
    {
        return ret;
    }
    /*应用层下发的时间参数是毫秒级的，而底层定义的参数是微秒级的，在用户态参数传入的时候需要转换*/
    ret = set_led_dr_attr(BRT_ATTR_FULL_ON, (led_cdev->breath_full_on)*1000);
    if(BR_ERROR == ret)
    {
        return ret;
    }
    ret = set_led_dr_attr(BRT_ATTR_FULL_OFF, (led_cdev->breath_full_off)*1000);
    if(BR_ERROR == ret)
    {
        return ret;
    }
    ret = set_led_dr_attr(BRT_ATTR_FADE_ON, (led_cdev->breath_fade_on)*1000);
    if(BR_ERROR == ret)
    {
        return ret;
    }
    ret = set_led_dr_attr(BRT_ATTR_FADE_OFF, (led_cdev->breath_fade_off)*1000);
    if(BR_ERROR == ret)
    {
        return ret;
    }
    
     /*时间赋值完毕进入亮度设置*/
    if ((led_cdev->breath_full_on)&&(led_cdev->breath_full_off)&&(led_cdev->breath_fade_on)&&(led_cdev->breath_fade_off))
    {
        if ((0 == led_cdev->brightness) || (1 == led_cdev->brightness))
        {               
            if(LED_NAME_2 < g_dr_name)
            {
                ret = set_led_dr_attr(BRT_ATTR_BRIGHTNESS, DR345_BRIGHTNESS_3MA);
            }
            else
            {
                ret = set_led_dr_attr(BRT_ATTR_BRIGHTNESS, DR12_BRIGHTNESS_3MA);
            }
        }
        else 
        {
            ret = set_led_dr_attr(BRT_ATTR_BRIGHTNESS, led_cdev->brightness);
        }    

        if(BR_ERROR == ret)
        {
            return ret;
        }
    }
    return BR_OK;
}

/************************************************************************
 *函数原型 ： void led_dr_work(struct work_struct *work)
 *描述     ： DR灯亮灭接口工作队列
 *输入     ： 无
 *输出     ： 无
 *返回值   ： 无
 *修改历史     :
 1.日    期   : 20140809
   修改内容   : 新生成函数
*************************************************************************/
static void led_dr_work(struct work_struct *work)
{
    int ret = BR_ERROR;
    unsigned long time_local = 0;
    int brightness = 0;
    
    struct balong_led_device	*led_dat = NULL;
    struct led_classdev *led_cdev = NULL;
	
    mutex_lock(&led_dr_mutex);
    led_dat = container_of(work, struct balong_led_device, work);
    led_cdev = &(led_dat->cdev);
    brightness = led_dat->brightness;

    if(!led_cdev)
    {
        LED_TRACE(LED_DEBUG_LEVEL(ERROR),"[%s] arguments is NULL\n", __FUNCTION__);
        goto out;
    }
    
    ret = change_name(led_cdev);    
    if(BR_ERROR == ret)
    {
        goto out;
    }
    
    if (brightness >= LED_FULL)
    {
        brightness = LED_FULL;
    }
    
    if(1 == brightness)
    {
        if(LED_NAME_2 < g_dr_name)
        {
#ifdef BSP_CONFIG_BOARD_E5
            brightness = DR345_BRIGHTNESS_4_5MA;
#else
            brightness = DR345_BRIGHTNESS_3MA;
#endif
        }
        else
        {
            brightness = DR12_BRIGHTNESS_3MA;
        }
    }
        
    if(g_dr_brightness[g_dr_name - 1] == brightness)    /*重复设置亮度*/
    {
        goto out;
    }
    g_dr_brightness[g_dr_name - 1] = brightness;    /*亮度值更新*/

    ret = led_dr_trigger_hold();
    if(BR_ERROR == ret)
    {
        goto out;
    }
    /*设置灯的状态为长灭*/
    if (LED_OFF == brightness)
    {
        ret = set_led_dr_attr(BRT_ATTR_FULL_ON, time_local);
        if(BR_ERROR == ret)
        {
            printk(KERN_ERR"[*Breath-Led*] %s: set_led_dr_attr(BRT_ATTR_FULL_ON,time_local) failded.\n", __func__);
            goto out;
        }
        ret = set_led_dr_attr(BRT_ATTR_FULL_OFF, LED_LONG_TIME);
        if(BR_ERROR == ret)
        {
            printk(KERN_ERR"[*Breath-Led*] %s: set_led_dr_attr(BRT_ATTR_FULL_OFF, LED_LONG_TIME) failded.\n", __func__);
            goto out;
        }
    }
    /*设置灯的状态为长亮*/
    else
    {
        ret = set_led_dr_attr(BRT_ATTR_FULL_ON, LED_LONG_TIME);
        if(BR_ERROR == ret)
        {
            printk(KERN_ERR"[*Breath-Led*] %s: set_led_dr_attr(BRT_ATTR_FULL_ON, LED_LONG_TIME) failded.\n", __func__);
            goto out;
        }
        ret = set_led_dr_attr(BRT_ATTR_FULL_OFF, time_local);
        if(BR_ERROR == ret)
        {
            printk(KERN_ERR"[*Breath-Led*] %s: set_led_dr_attr(BRT_ATTR_FULL_OFF, time_local) failded.\n", __func__);
            goto out;
        }
    }
    ret = set_led_dr_attr(BRT_ATTR_FADE_ON, time_local);
    if(BR_ERROR == ret)
    {
        goto out;
    }
    ret = set_led_dr_attr(BRT_ATTR_FADE_OFF, time_local);
    if(BR_ERROR == ret)
    {
        goto out;
    }
    
    ret = set_led_dr_attr(BRT_ATTR_BRIGHTNESS, brightness);
    if(BR_ERROR == ret)
    {
        goto out;
    }

out:
    mutex_unlock(&led_dr_mutex);
}

/************************************************************************
 *函数原型 ： void led_dr_brightness_set(struct led_classdev *led_cdev, enum led_brightness brightness)
 *描述     ： 用户态操作DR灯为长亮、长灭的接口
 *输入     ： 无
 *输出     ： 无
 *返回值   ： BR_OK OR BR_ERRO
 *修改历史     :
 1.日    期   : 20140620
   修改内容   : 新生成函数
*************************************************************************/
void led_dr_brightness_set(struct led_classdev *led_cdev, enum led_brightness brightness)
{
    struct balong_led_device    *led_dat =
        container_of(led_cdev, struct balong_led_device, cdev);
    led_dat->brightness = brightness;
    queue_work(led_workqueue_brightness, &(led_dat->work));
}
/************************************************************************
 *函数原型 ： enum led_brightness led_dr_brightness_get(struct led_classdev *led_cdev)
 *描述     ： 用户态操作DR灯开始闪烁的接口
 *输入     ： 无
 *输出     ： 无
 *返回值   ： BR_OK OR BR_ERRO
 *修改历史     :
 1.日    期   : 20140620
   修改内容   : 新生成函数
*************************************************************************/
enum led_brightness led_dr_brightness_get(struct led_classdev *led_cdev)
{
    if(!led_cdev)
    {
        LED_TRACE(LED_DEBUG_LEVEL(ERROR),"[%s] arguments is NULL\n", __FUNCTION__);
        return (enum led_brightness)BR_ERROR;  
    }
    
    return (enum led_brightness)led_cdev->brightness;   
}
/************************************************************************
 *函数原型 ： int led_dr_led_blink_set(struct led_classdev *led_cdev, unsigned long *delay_on, unsigned long *delay_off)
 *描述     ： 用户态灯的亮度值的接口
 *输入     ： 无
 *输出     ： 无
 *返回值   ： BR_OK OR BR_ERRO
 *修改历史     :
 1.日    期   : 20140620
   修改内容   : 新生成函数
*************************************************************************/
int led_dr_led_blink_set(struct led_classdev *led_cdev, unsigned long *delay_on, unsigned long *delay_off)
{
    return BR_OK;
}

    /*lint -e54 -e119 -e30 -e84 -e514 */
static int led_dr_probe(struct platform_device *dev)
{
    int ret = 0;
    unsigned int i = 0; 
    struct led_platform_data *pdata = dev->dev.platform_data;
    struct balong_led_device *led=  NULL;
    
    led = (struct balong_led_device *)kzalloc(sizeof(struct balong_led_device)*ARRAY_SIZE(led_dr) , GFP_KERNEL);
    if(!led)
    {
        LED_TRACE(LED_DEBUG_LEVEL(ERROR),"[%s] get buffer failed\n", __FUNCTION__);
        goto ERRO;
    }

    for(i = 0; i < (int)ARRAY_SIZE(led_dr); i++)
    {

        led[i].pdata =(struct balong_led_platdata*)kzalloc(sizeof(struct balong_led_platdata), GFP_KERNEL);
        if(!led[i].pdata)
        {
            LED_TRACE(LED_DEBUG_LEVEL(ERROR),"[%s] get pdata buffer failed\n", __FUNCTION__);
            goto ERRO;
        }

        led[i].cdev.name = pdata->leds[i].name;
        led[i].pdata->led_breath_set      = led_dr_breath_set;
        led[i].cdev.brightness_set        = led_dr_brightness_set;
        led[i].cdev.brightness_get        = led_dr_brightness_get;
        led[i].cdev.blink_set             = led_dr_led_blink_set;
              
        led[i].cdev.default_trigger       = pdata->leds[i].default_trigger;
        led[i].cdev.flags                 = 0;
        
        INIT_WORK(&(led[i].work), led_dr_work);

        ret = led_classdev_register(&dev->dev, &(led[i].cdev));
        if(ret)
        {
            LED_TRACE(LED_DEBUG_LEVEL(ERROR),"[%s]led%d sysfs register failed\n", __FUNCTION__,i + 1);
        }
        
    }

    platform_set_drvdata(dev, led);/*lint !e534*/
    return ret;
    
ERRO:
    for(i = 0; i < ARRAY_SIZE(led_dr); i++)  
    {
        if(!led)
        {
            continue;
        }
        if(led[i].pdata)
        {
            kfree(led[i].pdata);
            led[i].pdata = NULL;
        }
    }
    if(led)
    {
        kfree(led);
        led = NULL;
    }
    LED_TRACE(LED_DEBUG_LEVEL(ALWAYS),"LED init failed\n");
    return BR_ERROR; 
}

static int led_dr_remove(struct platform_device *dev)
{

    unsigned int i;
    struct balong_led_device *led = platform_get_drvdata(dev);
    if(!led)
    {
        return BR_ERROR;
    }
    
    if(&led->cdev)
    {
        led_classdev_unregister(&led->cdev);
    }
    for(i = 0; i < (int)(ARRAY_SIZE(led_dr)); i++)
    {
        if(!led[i].pdata)
        {
            continue;
        }
        led_classdev_unregister(&(led[i].cdev));
        if(led[i].pdata)
        {
            kfree(led[i].pdata);
            led[i].pdata = NULL;
        }
    }

    kfree(led);
    led = NULL;

    return BR_OK;
}
    /*lint +e54 +e119 +e30 +e84 +e514 */



static struct platform_driver led_dr_drv = {
    .probe =led_dr_probe,
    .remove = led_dr_remove,
    .driver = {  
        .name  = "led_dr",  
        .owner = THIS_MODULE, 
        .bus        = &platform_bus_type,
        .pm    = NULL,
    },  
};

static int __init led_dr_init(void)
{
    int ret = 0;
#if(FEATURE_ON == MBB_LED_GPIO)
    /* 解决变量未使用编译问题 */
    UNUSED_PARAMETER(gpio_exp_leds_config);
#endif
    ret = platform_device_register(&led_dr_dev);
    if (ret < 0)
    {
        printk(KERN_ERR "\r\n [led_dr_init] platform_device_register the led_dr failed.\r\n");
        return ret;
    }

    ret = platform_driver_register(&led_dr_drv);
    if (0 > ret)
    {
        platform_device_unregister(&led_dr_dev);
        printk(KERN_ERR "\r\n [led_dr_init] platform_device_unregister the led_dr failed.\r\n");
        return ret;
    }
    return ret;
}

static void __exit led_dr_exit(void)
{
    platform_driver_unregister(&led_dr_drv);

    platform_device_unregister(&led_dr_dev);
}

/*lint -e629*/
module_init(led_dr_init);
module_exit(led_dr_exit);
late_initcall(breathled_init);
module_exit(breathled_exit);
/*lint +e629*/
MODULE_AUTHOR("MBB.Huawei Device");
MODULE_DESCRIPTION("Breath Led Driver");
MODULE_LICENSE("GPL");
