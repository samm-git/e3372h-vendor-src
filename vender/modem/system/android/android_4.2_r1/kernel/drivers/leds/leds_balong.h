 /*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
 * foss@huawei.com
 *
 * If distributed as part of the Linux kernel, the following license terms
 * apply:
 *
 * * This program is free software; you can redistribute it and/or modify
 * * it under the terms of the GNU General Public License version 2 and 
 * * only version 2 as published by the Free Software Foundation.
 * *
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * * GNU General Public License for more details.
 * *
 * * You should have received a copy of the GNU General Public License
 * * along with this program; if not, write to the Free Software
 * * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Otherwise, the following license terms apply:
 *
 * * Redistribution and use in source and binary forms, with or without
 * * modification, are permitted provided that the following conditions
 * * are met:
 * * 1) Redistributions of source code must retain the above copyright
 * *    notice, this list of conditions and the following disclaimer.
 * * 2) Redistributions in binary form must reproduce the above copyright
 * *    notice, this list of conditions and the following disclaimer in the
 * *    documentation and/or other materials provided with the distribution.
 * * 3) Neither the name of Huawei nor the names of its contributors may 
 * *    be used to endorse or promote products derived from this software 
 * *    without specific prior written permission.
 * 
 * * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */
#include <linux/leds.h>
#include <linux/mutex.h>
#include <drv_leds.h>
#include <drv_nv_def.h>
#include <drv_nv_id.h>
#include <bsp_om.h>
#include <bsp_pmu.h>
#include <bsp_regulator.h>

/* LED id */
#define LED1_ID     PMU_DR01
#define LED2_ID     PMU_DR02
#define LED3_ID     PMU_DR03
#define LED4_ID     PMU_DR04
#define LED5_ID     PMU_DR05
#define LED_ID_MIN  LED1_ID
#define LED_ID_MAX  LED5_ID

/* LED name, for balong_ledX, X must not be less than LED_ID_MIN and bigger than LED_ID_MAX */
#define LED1_NAME    H6551_DR1_NM   /* 这里的名称必须与regulator的name一致，应使用宏定义 */
#define LED2_NAME    H6551_DR2_NM
#define LED3_NAME    H6551_DR3_NM
#define LED4_NAME    H6551_DR4_NM
#define LED5_NAME    H6551_DR5_NM

/* trigger name */
#define LED_TRIGGER_TIMER       "timer"     /* defined in timer_led_trigger, ledtrig-timer.c */
#define LED_TRIGGER_BREATH      "breath"    /* defined in breath_led_trigger, ledtrig-breath.c */

/* 用于sysfs系统显示呼吸模式参数时，最大输出字符串的长度 */
#define LED_SYSFS_MAX_STR_LENTH   8

/* some important structure */
struct balong_led_platdata {
    char *name;
    unsigned long full_on, full_off, fade_on, fade_off;
    int	(*led_breath_set)(struct led_classdev *led_cdev, unsigned long *full_on, unsigned long *full_off, 
        unsigned long *fade_on, unsigned long *fade_off);
    struct regulator *pregulator;
	struct mutex		mlock;
};

struct balong_led_device{
     struct led_classdev cdev;
     struct balong_led_platdata *pdata;
#if ( FEATURE_ON == MBB_LED_DR )
    struct work_struct work;/* 亮灭操作需要使用工作队列完成 */
    int     brightness;     /* 需要被设置的亮度 */
#endif
};

/******************************************************************************************/
/* 三色灯相关的宏 */

/* 颜色值,0位表示蓝,1位表示绿,2位表示红,其他位不用
青为蓝+绿,黄为红+绿,紫为红+蓝*/

enum LED_COLOR
{
    LED_NULL    = 0x0,
    LED_BLUE    = 0x1,
    LED_GREEN   = 0x2,
    LED_CYAN    = 0x3,
    LED_RED     = 0x4,
    LED_PURPLE  = 0x5,
    LED_YELLOW  = 0x6,
    LED_WHITE   = 0x7
};

#if (FEATURE_ON == MBB_WPG_COMMON)
enum
{
    DEF = 0,                /*0:默认闪灯枚举*/
    HONGKONG,               /*1:香港PCCW闪灯枚举*/
    RUSSIA,                 /*2:俄罗斯MTS闪灯枚举*/
    VODAFONE,               /*3:vodafone闪灯枚举*/   
    CHINAUNION,             /*4:中国联通闪灯枚举*/
                            /*5:预留值*/
    SFR = 6,                /*6:法国SFR闪灯枚举*/
    BSNL,                   /*7:印度BSNL闪灯枚举*/
    KPN,                    /*8:荷兰KPN闪灯枚举*/
    SOFTBANK,               /*9:日本软银闪灯枚举*/
    EMOBILE                /*10:日本Emobile闪灯枚举*/
};
#endif

/* 三色灯默认状态序列*/
#define LED_GREEN_LIGHTING_DOUBLE               {{LED_GREEN, 1},{LED_NULL,1},{LED_GREEN, 1},{LED_NULL, 17},{0,0}}/*绿灯双闪*/
#define LED_GREEN_LIGHTING_SIGNAL               {{LED_GREEN, 1},{LED_NULL, 19},{0,0}}/*绿灯单闪*/
#define LED_GREEN_LIGHTING_FREQUENTLY           {{LED_GREEN, 1},{LED_NULL,1},{0,0}}/*绿灯频闪*/
#define LED_GREEN_LIGHTING_ALWAYS               {{LED_GREEN, 1},{0,0}}/*绿灯常亮*/

#define LED_WHITE_LIGHTING_FREQUENTLY           {{LED_WHITE, 1},{LED_NULL,1},{0,0}}/*白灯频闪*/

#define LED_BULE_LIGHTING_SIGNAL                {{LED_BLUE, 1},{LED_NULL, 19},{0,0}}/*蓝灯单闪*/
#define LED_BLUE_LIGHTING_FREQUENTLY            {{LED_BLUE, 1},{LED_NULL,1},{0,0}}/*蓝灯频闪*/
#define LED_BLUE_LIGHTING_ALWAYS                {{LED_BLUE, 1},{0,0}}/*蓝灯常亮*/

#define LED_CYAN_LIGHTING_SIGNAL                {{LED_CYAN, 1},{LED_NULL, 19},{0,0}}/*青灯单闪*/
#define LED_CYAN_LIGHTING_ALWAYS                {{LED_CYAN, 1},{0,0}}/*青灯常亮*/

#define LED_BLUE_GREEN_ALTERNATE                {{LED_BLUE, 1},{LED_NULL,1},{LED_GREEN, 1},{LED_NULL, 17},{0,0}}/*蓝绿交替双闪*/  

#define LED_RED_LIGHTING_DOUBLE                 {{LED_RED, 1},{LED_NULL,1},{LED_RED, 1},{LED_NULL, 17},{0,0}}/*红灯双闪*/
#define LED_RED_LIGHTING_SIGNAL                 {{LED_RED, 1},{LED_NULL, 19},{0,0}}/*红灯单闪*/
#define LED_RED_LIGHTING_ALWAYS                 {{LED_RED, 1},{0,0}}/*红灯常亮*/

#define LED_SHUTDOWN_DEFINE                     {{LED_NULL, 1},{0,0}}/*灯灭*/

#if (FEATURE_ON == MBB_WPG_COMMON)
#define LED_PURPLE_LIGHTING_SIGNAL                {{LED_PURPLE, 1},{LED_NULL, 19},{0,0}}/*紫灯单闪*/
#define LED_PURPLE_LIGHTING_ALWAYS                {{LED_PURPLE, 1},{0,0}}/*紫灯常亮*/
#endif

/* some macro */
#define LED_NV_ALREADY_READ                     (0x0F0F0F0F)  /* nv already read flag */
#define MNTN_LED_STATUS_FLUSH                   (0x1)
#define MNTN_LED_TIMER_OCCURE                   (0x2)

#define RED_ON                                  led_on(LED3_ID - 1)
#define RED_OFF                                 led_off(LED3_ID - 1)
#define GREEN_ON                                led_on(LED4_ID - 1)
#define GREEN_OFF                               led_off(LED4_ID - 1)
#define BLUE_ON                                 led_on(LED5_ID - 1)
#define BLUE_OFF                                led_off(LED5_ID - 1)

 /* 三色灯闪灯状态序列个数*/
#define LED_CONFIG_MAX_LTE 10

#if (FEATURE_ON == MBB_WPG_COMMON)
#define LED_OM_CONFIG {LED_GREEN_LIGHTING_DOUBLE,\
                                LED_GREEN_LIGHTING_DOUBLE,\
                                LED_GREEN_LIGHTING_SIGNAL,\
                                LED_BULE_LIGHTING_SIGNAL,\
                                LED_GREEN_LIGHTING_ALWAYS,\
                                LED_BLUE_LIGHTING_ALWAYS,\
                                LED_CYAN_LIGHTING_ALWAYS,\
                                LED_GREEN_LIGHTING_DOUBLE,\
                                LED_WHITE_LIGHTING_FREQUENTLY,\
                                LED_GREEN_LIGHTING_FREQUENTLY,\
                                LED_GREEN_LIGHTING_FREQUENTLY,\
                                LED_GREEN_LIGHTING_ALWAYS,\
                                LED_BLUE_LIGHTING_ALWAYS,\
                                LED_BLUE_GREEN_ALTERNATE,\
                                LED_BLUE_LIGHTING_FREQUENTLY,\
                                LED_GREEN_LIGHTING_DOUBLE,\
                                LED_SHUTDOWN_DEFINE,\
                                LED_GREEN_LIGHTING_DOUBLE,\
                                LED_SHUTDOWN_DEFINE,\
                                LED_CYAN_LIGHTING_SIGNAL,\
                                LED_CYAN_LIGHTING_ALWAYS,\
                                LED_GREEN_LIGHTING_DOUBLE, \
                                LED_GREEN_LIGHTING_DOUBLE}

#define LED_HONGKONG_CONFIG {LED_GREEN_LIGHTING_DOUBLE,\
                                LED_GREEN_LIGHTING_DOUBLE,\
                                LED_GREEN_LIGHTING_SIGNAL,\
                                LED_BULE_LIGHTING_SIGNAL,\
                                LED_GREEN_LIGHTING_ALWAYS,\
                                LED_BLUE_LIGHTING_ALWAYS,\
                                LED_BLUE_LIGHTING_ALWAYS,\
                                LED_GREEN_LIGHTING_DOUBLE,\
                                LED_WHITE_LIGHTING_FREQUENTLY,\
                                LED_GREEN_LIGHTING_FREQUENTLY,\
                                LED_GREEN_LIGHTING_FREQUENTLY,\
                                LED_GREEN_LIGHTING_ALWAYS,\
                                LED_BLUE_LIGHTING_ALWAYS,\
                                LED_BLUE_GREEN_ALTERNATE,\
                                LED_BLUE_LIGHTING_FREQUENTLY,\
                                LED_GREEN_LIGHTING_DOUBLE,\
                                LED_SHUTDOWN_DEFINE,\
                                LED_GREEN_LIGHTING_DOUBLE,\
                                LED_SHUTDOWN_DEFINE,\
                                LED_CYAN_LIGHTING_SIGNAL,\
                                LED_CYAN_LIGHTING_ALWAYS,\
                                LED_GREEN_LIGHTING_DOUBLE,\
                                LED_GREEN_LIGHTING_DOUBLE}

#define LED_RUSSIA_CONFIG {LED_RED_LIGHTING_DOUBLE,\
                                LED_RED_LIGHTING_DOUBLE,\
                                LED_RED_LIGHTING_SIGNAL,\
                                LED_BULE_LIGHTING_SIGNAL,\
                                LED_RED_LIGHTING_ALWAYS,\
                                LED_BLUE_LIGHTING_ALWAYS,\
                                LED_CYAN_LIGHTING_ALWAYS,\
                                LED_RED_LIGHTING_DOUBLE,\
                                LED_WHITE_LIGHTING_FREQUENTLY,\
                                LED_GREEN_LIGHTING_FREQUENTLY,\
                                LED_GREEN_LIGHTING_FREQUENTLY,\
                                LED_GREEN_LIGHTING_ALWAYS,\
                                LED_BLUE_LIGHTING_ALWAYS,\
                                LED_BLUE_GREEN_ALTERNATE,\
                                LED_BLUE_LIGHTING_FREQUENTLY,\
                                LED_RED_LIGHTING_DOUBLE,\
                                LED_SHUTDOWN_DEFINE,\
                                LED_GREEN_LIGHTING_DOUBLE,\
                                LED_SHUTDOWN_DEFINE,\
                                LED_CYAN_LIGHTING_SIGNAL,\
                                LED_CYAN_LIGHTING_ALWAYS,\
                                LED_RED_LIGHTING_DOUBLE,\
                                LED_GREEN_LIGHTING_DOUBLE}

#define LED_VODAFONE_CONFIG {LED_GREEN_LIGHTING_DOUBLE,\
                                LED_GREEN_LIGHTING_DOUBLE,\
                                LED_GREEN_LIGHTING_SIGNAL,\
                                LED_BULE_LIGHTING_SIGNAL,\
                                LED_GREEN_LIGHTING_ALWAYS,\
                                LED_BLUE_LIGHTING_ALWAYS,\
                                LED_CYAN_LIGHTING_ALWAYS,\
                                LED_GREEN_LIGHTING_DOUBLE,\
                                LED_WHITE_LIGHTING_FREQUENTLY,\
                                LED_GREEN_LIGHTING_FREQUENTLY,\
                                LED_GREEN_LIGHTING_FREQUENTLY,\
                                LED_GREEN_LIGHTING_ALWAYS,\
                                LED_BLUE_LIGHTING_ALWAYS,\
                                LED_BLUE_GREEN_ALTERNATE,\
                                LED_BLUE_LIGHTING_FREQUENTLY,\
                                LED_GREEN_LIGHTING_DOUBLE,\
                                LED_SHUTDOWN_DEFINE,\
                                LED_GREEN_LIGHTING_DOUBLE,\
                                LED_SHUTDOWN_DEFINE,\
                                LED_PURPLE_LIGHTING_SIGNAL,\
                                LED_PURPLE_LIGHTING_ALWAYS,\
                                LED_GREEN_LIGHTING_DOUBLE,\
                                LED_GREEN_LIGHTING_DOUBLE}

#define LED_CHINAUNION_CONFIG {LED_GREEN_LIGHTING_DOUBLE,\
                                LED_GREEN_LIGHTING_DOUBLE,\
                                LED_GREEN_LIGHTING_SIGNAL,\
                                LED_BULE_LIGHTING_SIGNAL,\
                                LED_GREEN_LIGHTING_ALWAYS,\
                                LED_BLUE_LIGHTING_ALWAYS,\
                                LED_BLUE_LIGHTING_ALWAYS,\
                                LED_GREEN_LIGHTING_DOUBLE,\
                                LED_WHITE_LIGHTING_FREQUENTLY,\
                                LED_GREEN_LIGHTING_FREQUENTLY,\
                                LED_GREEN_LIGHTING_FREQUENTLY,\
                                LED_GREEN_LIGHTING_ALWAYS,\
                                LED_BLUE_LIGHTING_ALWAYS,\
                                LED_BLUE_GREEN_ALTERNATE,\
                                LED_BLUE_LIGHTING_FREQUENTLY,\
                                LED_RED_LIGHTING_ALWAYS,\
                                LED_SHUTDOWN_DEFINE,\
                                LED_GREEN_LIGHTING_DOUBLE,\
                                LED_SHUTDOWN_DEFINE,\
                                LED_CYAN_LIGHTING_SIGNAL,\
                                LED_CYAN_LIGHTING_ALWAYS,\
                                LED_RED_LIGHTING_ALWAYS,\
                                LED_GREEN_LIGHTING_DOUBLE}

#define LED_FRANCE_CONFIG {LED_RED_LIGHTING_ALWAYS,\
                                LED_RED_LIGHTING_ALWAYS,\
                                LED_GREEN_LIGHTING_SIGNAL,\
                                LED_BULE_LIGHTING_SIGNAL,\
                                LED_GREEN_LIGHTING_ALWAYS,\
                                LED_BLUE_LIGHTING_ALWAYS,\
                                LED_CYAN_LIGHTING_ALWAYS,\
                                LED_RED_LIGHTING_ALWAYS,\
                                LED_WHITE_LIGHTING_FREQUENTLY,\
                                LED_GREEN_LIGHTING_FREQUENTLY,\
                                LED_GREEN_LIGHTING_FREQUENTLY,\
                                LED_GREEN_LIGHTING_ALWAYS,\
                                LED_BLUE_LIGHTING_ALWAYS,\
                                LED_BLUE_GREEN_ALTERNATE,\
                                LED_BLUE_LIGHTING_FREQUENTLY,\
                                LED_RED_LIGHTING_ALWAYS,\
                                LED_SHUTDOWN_DEFINE,\
                                LED_RED_LIGHTING_SIGNAL,\
                                LED_SHUTDOWN_DEFINE,\
                                LED_CYAN_LIGHTING_SIGNAL,\
                                LED_CYAN_LIGHTING_ALWAYS,\
                                LED_RED_LIGHTING_ALWAYS,\
                                LED_GREEN_LIGHTING_DOUBLE}

#define LED_INDIA_CONFIG {LED_RED_LIGHTING_DOUBLE,\
                                LED_RED_LIGHTING_DOUBLE,\
                                LED_GREEN_LIGHTING_SIGNAL,\
                                LED_BULE_LIGHTING_SIGNAL,\
                                LED_GREEN_LIGHTING_ALWAYS,\
                                LED_BLUE_LIGHTING_ALWAYS,\
                                LED_CYAN_LIGHTING_ALWAYS,\
                                LED_RED_LIGHTING_DOUBLE,\
                                LED_WHITE_LIGHTING_FREQUENTLY,\
                                LED_GREEN_LIGHTING_FREQUENTLY,\
                                LED_GREEN_LIGHTING_FREQUENTLY,\
                                LED_GREEN_LIGHTING_ALWAYS,\
                                LED_BLUE_LIGHTING_ALWAYS,\
                                LED_BLUE_GREEN_ALTERNATE,\
                                LED_BLUE_LIGHTING_FREQUENTLY,\
                                LED_RED_LIGHTING_DOUBLE,\
                                LED_SHUTDOWN_DEFINE,\
                                LED_GREEN_LIGHTING_DOUBLE,\
                                LED_SHUTDOWN_DEFINE,\
                                LED_CYAN_LIGHTING_SIGNAL,\
                                LED_CYAN_LIGHTING_ALWAYS,\
                                LED_RED_LIGHTING_DOUBLE,\
                                LED_GREEN_LIGHTING_DOUBLE}

#define LED_HOLAND_CONFIG {LED_GREEN_LIGHTING_DOUBLE,\
                                LED_GREEN_LIGHTING_DOUBLE,\
                                LED_GREEN_LIGHTING_SIGNAL,\
                                LED_CYAN_LIGHTING_SIGNAL,\
                                LED_GREEN_LIGHTING_ALWAYS,\
                                LED_CYAN_LIGHTING_ALWAYS,\
                                LED_CYAN_LIGHTING_ALWAYS,\
                                LED_GREEN_LIGHTING_DOUBLE,\
                                LED_WHITE_LIGHTING_FREQUENTLY,\
                                LED_GREEN_LIGHTING_FREQUENTLY,\
                                LED_GREEN_LIGHTING_FREQUENTLY,\
                                LED_GREEN_LIGHTING_ALWAYS,\
                                LED_BLUE_LIGHTING_ALWAYS,\
                                LED_BLUE_GREEN_ALTERNATE,\
                                LED_BLUE_LIGHTING_FREQUENTLY,\
                                LED_GREEN_LIGHTING_DOUBLE,\
                                LED_SHUTDOWN_DEFINE,\
                                LED_GREEN_LIGHTING_DOUBLE,\
                                LED_SHUTDOWN_DEFINE,\
                                LED_BULE_LIGHTING_SIGNAL,\
                                LED_BLUE_LIGHTING_ALWAYS,\
                                LED_GREEN_LIGHTING_DOUBLE,\
                                LED_GREEN_LIGHTING_DOUBLE}

#define LED_JAPAN_SOFTBANK_CONFIG
#define LED_JAPAN_EMOBILE_CONFIG

#else
#define LED_OM_CONFIG {LED_GREEN_LIGHTING_DOUBLE,\
                                LED_GREEN_LIGHTING_DOUBLE,\
                                LED_GREEN_LIGHTING_SIGNAL,\
                                LED_BULE_LIGHTING_SIGNAL,\
                                LED_GREEN_LIGHTING_ALWAYS,\
                                LED_BLUE_LIGHTING_ALWAYS,\
                                LED_CYAN_LIGHTING_ALWAYS,\
                                LED_GREEN_LIGHTING_DOUBLE,\
                                LED_WHITE_LIGHTING_FREQUENTLY,\
                                LED_GREEN_LIGHTING_FREQUENTLY,\
                                LED_GREEN_LIGHTING_FREQUENTLY,\
                                LED_GREEN_LIGHTING_ALWAYS,\
                                LED_BLUE_LIGHTING_ALWAYS,\
                                LED_BLUE_GREEN_ALTERNATE,\
                                LED_BLUE_LIGHTING_FREQUENTLY,\
                                LED_GREEN_LIGHTING_DOUBLE,\
                                LED_SHUTDOWN_DEFINE,\
                                LED_GREEN_LIGHTING_DOUBLE,\
                                LED_SHUTDOWN_DEFINE,\
                                LED_CYAN_LIGHTING_SIGNAL,\
                                LED_GREEN_LIGHTING_DOUBLE}
#endif

/* 三色灯当前状态参数*/
struct led_param
{
	unsigned char led_state_id;     /* 三色灯状态，对应LED_STATUS */
	unsigned char led_config_id;    /* 闪灯状态序列值 */
	unsigned char led_color_id;     /* 三色灯颜色，对应LED_COLOR的值 */
	unsigned char led_time;         /* 该配置持续的时间长度，单位100ms */
};

/*****************************************************************************
 结构名    : LED_CONTROL_NV_STRU
 结构说明  : LED_CONTROL_NV结构 ID=7
*****************************************************************************/
typedef struct
{
    unsigned char   ucLedColor;      /*三色灯颜色，对应LED_COLOR的值*/
    unsigned char   ucTimeLength;    /*该配置持续的时间长度，单位100ms*/
}LED_CONTROL_NV_STRU;

/* led nv结构体，保存nv和已读标记 */
struct nv_led{
    LED_CONTROL_NV_STRU g_led_state_str_om[LED_LIGHT_STATE_MAX][LED_CONFIG_MAX_LTE];
    int g_already_read;
};

struct led_tled_arg
{
    unsigned char ctl;
    unsigned char new_state;
};

void balong_led_brightness_set(struct led_classdev *led_cdev, enum led_brightness brightness);
int led_threecolor_flush(u32 channel_id , u32 len, void* context);
int do_led_threecolor_flush(void);
int balong_led_breath_set(struct led_classdev *led_cdev, unsigned long *full_on, unsigned long *full_off,
                                        unsigned long *fade_on, unsigned long *fade_off);

