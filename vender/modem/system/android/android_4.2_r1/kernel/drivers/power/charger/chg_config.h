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
#ifndef CHG_CONFIG_H
#define CHG_CONFIG_H
#if (MBB_CHG_PLATFORM_V7R2 == FEATURE_ON)
#include "drv_chg.h"
#include "drv_onoff.h"
#endif

/*----------------------------------------------*
 * 包含外部头文件                                   *
 *----------------------------------------------*/


/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
/*************************BEGIN:产品功能特性宏定义***********************/
/*充放电功能总控制宏,编译控制宏，外部编译如果定义此处不再定义*/
//#define    MBB_CHARGE              FEATURE_ON
#if 0
/*LCD产品特性宏*/
#define    MBB_CHG_LCD             FEATURE_OFF
/*LED产品特性宏*/
#define    MBB_CHG_LED             FEATURE_OFF
/*TOHCH产品特性宏*/
#define    MBB_CHG_TOUCH           FEATURE_ON
/*LINUX系统特性宏*/
#define    MBB_CHG_LINUX           FEATURE_ON
/*LINUX系统特性宏*/
#define    MBB_CHG_VXWORKS         FEATURE_OFF
/*REX系统特性宏*/
#define    MBB_CHG_REX             FEATURE_OFF
/*对外充电特性宏*/
#define    MBB_CHG_EXTCHG          FEATURE_ON
/*无线充电特性宏*/
#define    MBB_CHG_WIRELESS        FEATURE_ON
/*MC2.0特性宏*/
#define    MBB_CHG_MC2             FEATURE_ON
/*工厂补电特性宏*/
#define    MBB_CHG_COMPENSATE      FEATURE_ON
/*BQ24196充电芯片特性宏*/
#define    MBB_CHG_BQ24196         FEATURE_ON
/*BQ24161充电芯片特性宏*/
#define    MBB_CHG_BQ24161         FEATURE_OFF
/*MAX8903充电芯片特性宏*/
#define    MBB_CHG_MAX8903         FEATURE_OFF
/*scharger充电芯片特性宏*/
#define    MBB_CHG_SCHARGER        FEATURE_OFF
/*高温充电特性宏*/
#define    MBB_CHG_WARM_CHARGE     FEATURE_ON
/*MC2.0特性宏*/
#define    MBB_CHG_MC              FEATURE_ON
/*power supply特性宏*/
#define    MBB_CHG_POWER_SUPPLY    FEATURE_ON
/*V7R2特性宏*/
//#define    MBB_CHG_PLATFORM_V7R2   FEATURE_ON
/*9X25特性宏*/
//#define    MBB_CHG_PLATFORM_9X25   FEATURE_OFF
#endif

/*归一化代码移植打桩调试宏*/
//#define CHG_STUB

#if (FEATURE_ON == MBB_FAST_ON_OFF)
#define CONFIG_MBB_FAST_ON_OFF 
#endif

/*************************END:产品功能特性宏定义****************/

/*NV项宏定义*/
#define  NV_OVER_TEMP_SHUTOFF_PROTECT     (50016)
#define  NV_LOW_TEMP_SHUTOFF_PROTECT      (52005)
#define  NV_BATT_TEMP_PROTECT_I           (50385)
#define  NV_BATT_VOLT_PROTECT_I           (50386)
#define  NV_POWERUP_MODE                  (50364)
#define  NV_FACTORY_MODE_I                (36)


/*----------------------------------------------*
* 简单数据类型定义说明                         *
*----------------------------------------------*/

typedef unsigned long           ulong64_t;
typedef unsigned int            uint32_t;
typedef signed int              int32_t;
typedef unsigned short          uint16_t;
typedef signed short            int16_t;
typedef unsigned char           uint8_t;
typedef signed char             int8_t;
typedef uint8_t                 boolean;

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/
#ifndef YES
#define YES  1
#endif

#ifndef NO
#define NO   0
#endif

#ifndef TRUE
#define TRUE                  1
#endif
#ifndef FALSE
#define FALSE                 0
#endif

#define CHG_OK                 (0 )
#define CHG_ERROR              (-1)

/*=====================ERROR CODE===================*/
#define CHG_BUS_ERR            (-2)
#define CHG_IO_ERR             (-3)
#define CHG_NO_MEM             (-4)
#define CHG_ADC_ERR            (-5)
/**********Charge Module Return Code Macros**********/

/*电量百分比定义*/
#define    CHG_EVENT_NONEED_CARE     255

#if (MBB_CHG_COULOMETER == FEATURE_ON || MBB_CHG_BQ27510 == FEATURE_ON)
#define    BATT_CAPACITY_SHUTOFF     2
#define    BATT_CAPACITY_LEVELLOW    10
#define    BATT_CAPACITY_LEVEL1      11
#define    BATT_CAPACITY_LEVEL2      20
#define    BATT_CAPACITY_LEVEL3      30
#define    BATT_CAPACITY_LEVEL4      60
#define    BATT_CAPACITY_RECHG       95
#define    BATT_CAPACITY_FULL        100
#else
#define    BATT_CAPACITY_SHUTOFF     0
#define    BATT_CAPACITY_LEVELLOW    3
#define    BATT_CAPACITY_LEVEL1      10
#define    BATT_CAPACITY_LEVEL2      30
#define    BATT_CAPACITY_LEVEL3      50
#define    BATT_CAPACITY_LEVEL4      80
#define    BATT_CAPACITY_RECHG       95
#define    BATT_CAPACITY_FULL        100
#endif
/*----------------------------------------------*
 * 枚举定义                                       *
 *----------------------------------------------*/
typedef enum
{
    CHG_MSG_ERR = 0,
    CHG_MSG_INFO,
    CHG_MSG_DEBUG,
    CHG_MSG_MAX
}CHG_MSG_TYPE;

typedef enum
{
    /*CHG_STM_INIT_ST: NOT real state, just for initialize.*/
    CHG_STM_INIT_ST = -1,
    CHG_STM_TRANSIT_ST = 0,
    CHG_STM_FAST_CHARGE_ST,
    CHG_STM_MAINT_ST,
    CHG_STM_INVALID_CHG_TEMP_ST,
    CHG_STM_BATTERY_ONLY,
    CHG_STM_WARMCHG_ST,
    /*对外充电改为由work完成，不用状态机*/
#if 0
    CHG_STM_EXTCHG_ST,
#endif
    CHG_STM_MAX_ST,
    CHG_STM_ST_RSVD = 0xFFFF /* */
}chg_stm_state_type;

typedef enum
{
    CHG_MODE_INIT = -1,
    CHG_WIRED_MODE   = 0,
    CHG_WIRELESS_MODE,
    CHG_EXTCHG_MODE,
    CHG_BAT_ONLY_MODE,
    CHG_SUPPLY_MODE,
    CHG_MODE_UNKNOW
}CHG_MODE_ENUM;


/****Note:以下枚举变量在不同的平台中在不同的文件中有定义，如果平台其他文件中有定义本文件中的以下定义可以注掉******/

typedef enum POWER_ON_STATUS_tag
{
    POWER_ON_STATUS_INVALID         = 0x00000000, /* */
    POWER_ON_STATUS_BOOTING         = 0x00000000, /* */
    POWER_ON_STATUS_CHARGING        = 0x504F5343, /* */
    POWER_ON_STATUS_CAPACITY_LOW    = 0x504F434C, /* */
    POWER_ON_STATUS_BATTERY_ERROR   = 0x504F4245, /* */
    POWER_ON_STATUS_FINISH_NORMAL   = 0x6f656d00, /* */
    POWER_ON_STATUS_FINISH_CHARGE   = 0x59455353, /* */
    POWER_ON_STATUS_BUTT            = 0xFFFFFFFF  /* */
}POWER_ON_STATUS_ENUM;
#if (MBB_CHG_PLATFORM_V7R2 == FEATURE_OFF)
typedef enum DRV_START_MODE_tag
{
     DRV_START_MODE_EXCEPTION,  /* 异常开机模式，此时大部分业务都不启动, 仅提示信息 */
     DRV_START_MODE_NORMAL,   /*正常开机*/
     DRV_START_MODE_CHARGING,   /* 充电开机模式, ps 等业务不启动  */
     DRV_START_MODE_NOAPP,      /*SD卡升级第二次重启后不启动APP*/
     DRV_START_MODE_BUTT
}DRV_START_MODE_ENUM;
#endif

#if (MBB_CHG_PLATFORM_V7R2 == FEATURE_ON)
typedef DRV_START_MODE DRV_START_MODE_ENUM;
#endif

 /*chg模块用到的ADC采集通道类型枚举*/
typedef enum CHG_PARAMETER_ENUM_tag
{
    CHG_PARAMETER__BATTERY_VOLT,        /* 电池电压 */
    CHG_PARAMETER__BATT_THERM_DEGC,     /* 电池温度 */
    CHG_PARAMETER__VBUS_VOLT,           /*对外充电用于检测VBUS电压*/
    CHG_PARAMETER__BATT_BOT_THERM_DEGC,  /*板级电池温度*/
    CHG_PARAMETER__BATTERY_ID_VOLT,  /*电池ID电压*/   
    CHG_PARAMETER__INVALID
} CHG_PARAMETER_ENUM;

/*充电LCD显示*/
typedef enum
{
    CHG_DISP_OK,
    CHG_DISP_FAIL,
    CHG_DISP_BATTERY_LOWER,  /* LOW BATTERY   */
    CHG_DISP_BATTERY_BAD,  /*BATTERY ERROR*/
    CHG_DISP_OVER_HEATED,/* Over-heated */
    CHG_DISP_TEMP_LOW, /* Temp. Too Low */
    CHG_DISP_MAX
}CHG_BATT_DISPLAY_TYPE;


#if (MBB_CHG_PLATFORM_V7R2 == FEATURE_OFF)
typedef enum DRV_SHUTDOWN_REASON_tag_s
{
    DRV_SHUTDOWN_LOW_BATTERY,           /* 电池电量低                 */
    DRV_SHUTDOWN_BATTERY_ERROR,         /* 电池异常                   */
    DRV_SHUTDOWN_POWER_KEY,             /* 长按 Power 键关机          */
    DRV_SHUTDOWN_TEMPERATURE_PROTECT,   /* 过温保护关机               */
    DRV_SHUTDOWN_LOW_TEMP_PROTECT,
    DRV_SHUTDOWN_RESET,                 /* 系统软复位                 */
    DRV_SHUTDOWN_CHARGE_REMOVE,         /* 关机充电模式下，拔除充电器 */
    DRV_SHUTDOWN_UPDATE,                /* 关机并进入升级模式         */
    DRV_SHUTDOWN_BUTT
}DRV_SHUTDOWN_REASON_ENUM;


/****Note:以下枚举变量在不同的平台中在不同的文件中有定义，如果平台其他文件中有定义本文件中的以下定义可以注掉******/
/*电池充电状态--查询到状态以底软为准*/
typedef enum CHARGING_STATE_ENUM_tag
{
    CHARGING_INIT =  -1,
    NO_CHARGING_UP = 0,         /* 开机未充电*/
    CHARGING_UP,               /*开机正在充电 */
    NO_CHARGING_DOWN,          /* 关机未充电 */
    CHARGING_DOWN             /* 关机充电  */
}CHARGING_STATE_ENUM;

/* 电池电量*/
typedef enum BATT_LEVEL_ENUM_tag
{
    BATT_INIT =  -2,       /* */
    BATT_LOW_POWER = -1 ,  /*电池低电，*/
    BATT_LEVEL_0,                 /*0格电池电量**/
    BATT_LEVEL_1 ,                /*1格电池电量*/
    BATT_LEVEL_2 ,                 /*2格电池电量*/
    BATT_LEVEL_3 ,               /*3格电池电量*/
    BATT_LEVEL_4  ,              /*4格电池电量*/
    BATT_LEVEL_MAX
}BATT_LEVEL_ENUM;

/****Note:以上枚举变量在不同的平台中在不同的文件中有定义，如果平台其他文件中有定义本文件中的以下定义可以注掉******/

/*电池高温上报事件*/
typedef enum _TEMP_EVENT
{
   TEMP_BATT_LOW,    /* 电池低温事件 */
   TEMP_BATT_HIGH,   /* 电池高温事件 */
   TEMP_BATT_NORMAL, /* 电池高温解除事件 */
   TEMP_BATT_MAX     /* 如果事件值不小于此值，则为非法事件*/
}TEMP_EVENT;
#endif

/*BATTERY(电池)主动上报的事件*/
typedef enum _BAT_EVENT
{
    BAT_CHARGING_ON_START = 0,  /* 在开机模式，开始充电 */
    BAT_CHARGING_OFF_START,     /* 在关机模式，开始充电 */
    BAT_LOW_POWER,              /* 电池电量低时，上报此事件 */
    BAT_CHARGING_UP_STOP,       /* 开机停止充电时，上报此事件 */
    BAT_CHARGING_DOWN_STOP,     /* 关机停止充电时，上报此事件 */
    BAT_LOW_POWEROFF,           /* 电池电量低导致的关机，上报此事件*/
    BAT_EVENT_MAX               /* 如果事件值不小于此值，则为非法事件 */
}BATTERY_EVENT;

/*KEY主动上报的事件*/
typedef enum _GPIO_KEY_EVENT
{
    GPIO_KEY_WAKEUP = 0,           /* 短按任意按键，上报此事件，用于点亮屏幕 */
    GPIO_KEY_WLAN,                 /* 用于打开/关闭WLAN设备，不同的版本使用的按键不同 */
    GPIO_KEY_WPS,                  /* 长按WPS键（大于5秒）时，上报此事件，用于激活WPS功能 */
    GPIO_KEY_FACTORY_RESTORE,      /* 长按复位键时，上报此事件，用于恢复出厂设置 */
    GPIO_KEY_POWER_OFF,            /* 长按POWER键时，上报此事件，用于关机 */
    GPIO_KEY_UPDATE,               /* 当系统需要升级时，上报此事件，用于升级系统 */
    GPIO_KEY_SSID,                 /* 短按WPS键（小于5秒）时，用于显示SSID，只有部分版本需要 */
    GPIO_KEY_WPS_PROMPT,           /* 短按WPS键（大于1秒）时，用于显示WPS提示语，只有部分版本需要 */
    GPIO_KEY_POWER_SHORT,          /* 短按Power按键，500毫秒内要松开按键 */
    GPIO_KEY_WPS_SHORT,            /* 短按WPS按键，500毫秒内要松开按键 */
    GPIO_KEY_RESET_SHORT,          /* 短按Reset按键，500 毫秒内要松开按键 */
    GPIO_KEY_WPS_POWER,            /* WPS和POWER 键一起按下2秒 */
    GPIO_KEY_WPS_POWER_UP,         /* WPS和POWER 键一起按下2秒后在5秒前抬起，或者5秒后未在500毫秒内抬起按下WPS */
    GPIO_KEY_POWER_ONE_SECOND,     /* Power按下1秒 */
    GPIO_KEY_POWER_ONE_SECOND_UP,  /* Power按下1秒后在2秒前抬起 */
    GPIO_KEY_WPS_ONE_SECOND,       /*WPS 短按 1秒事件，主要用于开关 WiFi Offload*/
    GPIO_KEY_WPS_DIAL = 16,         /*WPS 短按松开上报 WPS 拨号事件，主要用于按键拨号*/
#ifdef FEATURE_FUNC_FAST_BOOT
    /*平台提供5秒快速关机事件*/
    GPIO_KEY_SHORT_POWER_OFF = 17,
    /*平台提供5秒快速开机事件*/
    GPIO_KEY_SHORT_POWER_ON = 18,
 #endif
    GPIO_KEY_EVENT_MAX             /* 如果事件值不小于此值，则为非法事件 */
}GPIO_KEY_EVENT;

typedef enum
{
    DEVICE_ID_NULL_ID = 0, /*空id，用于初始化id*/
    DEVICE_ID_USB = 1,     /*USB设备id*/
    DEVICE_ID_KEY = 2,     /*KEY按键设备id*/
    DEVICE_ID_BATTERY = 3,/*电池事件上报使用此ID*/
    DEVICE_ID_CHARGER = 4, /*充电器事件上报使用此ID*/
    DEVICE_ID_SD_CARD = 5, /* */
    DEVICE_ID_GPIO_INTER = 6, /*非按键的gpio中断上报，使用此ID*/
    DEVICE_ID_SCREEN = 7, /* */
    DEVICE_ID_WLAN = 8, /* */
    DEVICE_ID_OM = 9, /* OM上报消息 */
    DEVICE_ID_TEMP = 10,/*温度保护ID*/
    DEVICE_ID_WIRELESS = 254, /*无线充电事件*/
    DEVICE_ID_EXTCHG = 255,   /*对外充电事件*/
    DEVICE_ID_MAX_ID   /*用做边界保护*/
}DEVICE_ID_E;
#if (MBB_CHG_COULOMETER == FEATURE_ON || MBB_CHG_BQ27510 == FEATURE_ON)
/*充电模块提供给coul的事件类型*/
typedef enum
{
    VCHRG_START_USB_CHARGING_EVENT = 0,
    VCHRG_START_AC_CHARGING_EVENT,
    VCHRG_START_CHARGING_EVENT,
    VCHRG_STOP_CHARGING_EVENT,
    VCHRG_CHARGE_DONE_EVENT,
    VCHRG_NOT_CHARGING_EVENT,
    VCHRG_POWER_SUPPLY_OVERVOLTAGE,
    VCHRG_POWER_SUPPLY_WEAKSOURCE,
    VCHRG_MAX
}CHG_COUL_EVENT_TYPE;
#endif


/*----------------------------------------------*
 * 结构定义                                       *
 *----------------------------------------------*/
 /***************Note:平台相关代码，根据平台按需要添加，有的平台如V7R1需要
                  移植人员根据需要，添加或者移除下边函数调用***************************/
#if 0
    /*V7R1移植,A核通过该共享内存读取电池电压*/
    /*AXI内存结构体，共32个字节，目前有28个字节未使用*/
    typedef struct _st_batt_info_share_mem
    {
        BSP_S32 batt_current_volt; /*当前电压值*/
        BSP_S32 batt_unused[7];   /*尚未使用*/
    }st_batt_info_share_mem;
#endif

/*充电过程中关键信息结构体*/
typedef struct
{
    int32_t            battery_one_temp;        /* 电池单次采集温度**/
    int32_t            battery_temp;            /* 电池实际温度**/
    int32_t            battery_one_volt;        /* 电池单次采集电压**/
    int32_t            battery_volt;            /* 电池实际电压**/
    int32_t            bat_capacity;            /* 电池电量百分比*/
    int32_t            bat_time_to_full;        /* 电池电量距充满还有多长时间*/
    BATT_LEVEL_ENUM    bat_volt_lvl;            /* 电池电量级别,即电池格数**/
    int32_t            volt_revise_value;        /* 电池电压虚高虚低修正值*/
    int32_t            battery_id_volt;        /* 电池ID电压*/
}CHG_PROCESS_INFO;

/****Note:以下结构体在不同的平台中在不同的文件中有定义，如果平台其他文件中有定义本文件中的以下定义可以注掉******/
#if (MBB_CHG_PLATFORM_V7R2 == FEATURE_OFF)
typedef struct BATT_STATE_tag
{
    CHARGING_STATE_ENUM  charging_state;
    BATT_LEVEL_ENUM      battery_level;
    TEMP_EVENT      batt_temp_state;
}BATT_STATE_T;
#endif
/****Note:以上枚举变量在不同的平台中在不同的文件中有定义，如果平台其他文件中有定义本文件中的以下定义可以注掉******/
/*=================================================================
 CHG_CHGR_UNKNOWN: Chgr type has not been check completely from USB module.
 CHG_WALL_CHGR   : Wall standard charger, which D+/D- was short.
 CHG_USB_HOST_PC : USB HOST PC or laptop or pad, etc.
 CHG_NONSTD_CHGR : D+/D- wasn't short and USB enumeration failed.
 CHG_CHGR_INVALID: External Charger invalid or absent.
 ==================================================================*/
typedef enum
{
    /*未知类型*/
    CHG_CHGR_UNKNOWN,
    /*标准充电器*/
    CHG_WALL_CHGR,
    /*USB*/
    CHG_USB_HOST_PC,
    /*非标准充电器*/
    CHG_NONSTD_CHGR,
    /*无线充电器*/
    CHG_WIRELESS_CHGR,
    /*对外充电器*/
    CHG_EXGCHG_CHGR,
    /*弱充*/
    CHG_500MA_WALL_CHGR,
    /*cradle*/
    CHG_USB_OTG_CRADLE,
    /*充电器不可用*/
    CHG_CHGR_INVALID,
    CHG_CHGR_TYPE_RSVD = 0xFFFF /* */
}chg_chgr_type_t;



 /*通道类型对应通道号结构*/
typedef struct
{
    CHG_PARAMETER_ENUM  param_type;
    int32_t             adc_channel;
}CHG_ADC_PARAM_TO_CHANNEL_TYPE;

/*温度对应电压结构*/
typedef struct
{
    int32_t   temperature;
    int32_t   voltage;
}CHG_TEMP_ADC_TYPE;

typedef struct
{
    chg_stm_state_type cur_stm_state;     /*充电状态机当前状态*/
    chg_chgr_type_t    cur_chgr_type;     /*当前充电器类型*/
    uint32_t          charging_lasted_in_sconds; /*充电持续时间*/
    chg_stm_state_type pre_stm_state;     /*充电状态机前一个状态*/
    int32_t            bat_stat_type;     /*电池充电状态信息与power supply STATUS属性对应*/
    int32_t            bat_heath_type;    /*电池温度状态信息与power supply HEALTH属性对应*/
    int32_t            extchg_status;     /*对外充电状态信息与power supply EXTCHG_STA属性对应*/
    CHG_MODE_ENUM      cur_chg_mode;      /*目前所处的充电状态，有线/无线/对外充电状态*/
    boolean            extchg_online_st;  /*对外充电在位状态*/
    boolean            wireless_online_st;/*无线充电在位状态*/
    boolean            usb_online_st;  /*对外充电在位状态*/
    boolean            ac_online_st;/*无线充电在位状态*/
}chg_stm_state_info_t;

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/


/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "chg_charge_api.h"
#include "chg_charge_stm.h"
#include "chg_charge_task.h"
#include "huawei-charger.h"
#include "platform/hisi/chg_chip_platform.h"
#include "os_adapter/linux/chg_os_adapter.h"
#include "drv/bq24196/chg_charge_drv.h"

#if (MBB_CHG_COULOMETER == FEATURE_ON)
#include "coul/drivers/hisi_smartstar_coul.h"
#include "coul/drivers/hisi_coul_drv.h"
#endif
#include "hardware_data/chg_hardware_data.h"

#if (MBB_CHG_BQ27510 == FEATURE_ON)
#include "bq27510/bq27510_battery.h"
#include "bq27510/hisi_coul_drv.h"
#include "bq27510/bq27510_battery_monitor.h"
#endif

#endif/*CHG_CONFIG_H*/
