/******************************************************************************

                  版权所有 (C), 2001-2011,华为终端有限公司

 ******************************************************************************
  文 件 名   : chg_charge_adc.h
  版 本 号   : 初稿
  生成日期   : 2012年10月31日
  最近修改   :
  功能描述   : 头文件
  函数列表   :
******************************************************************************/
#ifndef CHG_BOOT_CHIP_PLATFORM_H
#define CHG_BOOT_CHIP_PLATFORM_H

/******************************问题单修改记录**********************************
    日期       修改人       问题单号                 修改内容

******************************************************************************/

/*----------------------------------------------*
* 包含头文件                                   *
*----------------------------------------------*/
#include "types.h"
#include <product_config.h>
/*************************Note:产品特性宏定义***********************/

 /*************************Note:产品特性宏定义***********************/



/********Note:以下头文件跟平台强相关，移植人员根据平台需要添加*******/
#if (MBB_CHG_PLATFORM_V7R2 == FEATURE_ON)
#include <balongv7r2/gpio.h>
#include "drv_onoff.h"
#endif

/********Note:以下头文件跟平台强相关，移植人员根据平台需要添加*******/

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
 * 全局变量                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/
/*----------------------------------------------*
 * 枚举定义                                       *
 *----------------------------------------------*/
/* Begin daizhicheng 2013-07-01 added */
/* 增加BOOT阶段LOG等级定义 */
typedef enum
{
    CHG_MSG_ERR = 0,
    CHG_MSG_INFO,
    CHG_MSG_DEBUG,
    CHG_MSG_MAX
}CHG_MSG_TYPE;
/* End daizhicheng 2013-07-01 added */

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/

//#define chg_print_level_message( print_level, tring, arg1, arg2, arg3)

/* BEGIN: PN:dongle121,chendongguo, 2013/6/28 added/deleted/modified*/
typedef unsigned long           ulong64_t;
//typedef unsigned int            uint32_t;
typedef signed int              int32_t;
typedef unsigned short          uint16_t;
typedef signed short            int16_t;
typedef unsigned char           uint8_t;
typedef signed char             int8_t;
typedef uint8_t                 boolean;
/* END: PN:dongle121,chendongguo, 2013/6/28 added/deleted/modified*/


#define TRUE 1
#define FALSE 0

/* Begin daizhicheng 2013-07-01 added */
/* BOOT阶段LOG默认等级----CHG_MSG_INFO */
#define CHG_BOOT_LOG_LEVEL CHG_MSG_INFO
/* End daizhicheng 2013-07-01 added */
#if (MBB_CHG_PLATFORM_V7R2 == FEATURE_ON)
#define CHG_BQ24192_CD_GPIO    CHG_ENABLE_GPIO
#else
#define CHG_BQ24192_CD_GPIO 31
#endif
/*----------------------------------------------*
 * 枚举定义                                       *
 *----------------------------------------------*/
/****Note:以下枚举变量在不同的平台中在不同的文件中有定义，如果平台其他文件中有定义本文件中的以下定义可以注掉******/
#if (MBB_CHG_PLATFORM_V7R2 == FEATURE_ON)
#define  NV_BOOT_POWERUP_MODE                  (50364)
#define  NV_BOOT_FACTORY_MODE                   (36)
/* NV50364电池参数数据结构**/
typedef struct
{
    /*非工厂模式下电池部在位开机使能标志*/
    uint8_t no_battery_powerup_enable;
    /*异常关机后，下次插入电源进入开机模式使能标志*/
    uint8_t exception_poweroff_poweron_enable;
    /*低电关机禁止标志*/
    uint8_t low_battery_poweroff_disable;
    /*保留*/
    uint8_t reserved;
}BOOT_POWERUP_MODE_TYPE;
#endif
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
#if (MBB_CHG_PLATFORM_V7R2 == FEATURE_ON)
typedef DRV_START_MODE DRV_START_MODE_ENUM;
#else
typedef enum DRV_START_MODE_tag
{
     DRV_START_MODE_EXCEPTION,  /* 异常开机模式，此时大部分业务都不启动, 仅提示信息 */
     DRV_START_MODE_NORMAL,   /*正常开机*/
     DRV_START_MODE_CHARGING,   /* 充电开机模式, ps 等业务不启动  */
     DRV_START_MODE_NOAPP,      /*SD卡升级第二次重启后不启动APP*/
     DRV_START_MODE_BUTT
}DRV_START_MODE_ENUM;
#endif

 /*chg模块用到的ADC采集通道类型枚举*/
typedef enum CHG_PARAMETER_ENUM_tag
{
    CHG_PARAMETER__BATTERY_VOLT,        /* 电池电压 */
    CHG_PARAMETER__BATT_THERM_DEGC,     /* 电池温度 */
    CHG_PARAMETER__VBUS_VOLT,           /*vbus电压*/
    CHG_PARAMETER__BATT_BOT_THERM_DEGC,  /*板级电池温度*/
    CHG_PARAMETER__BATTERY_ID_VOLT,  /*电池ID电压*/ 
    CHG_PARAMETER__INVALID
} CHG_PARAMETER_ENUM;

/****Note:以上枚举变量在不同的平台中在不同的文件中有定义，如果平台其他文件中有定义本文件中的以下定义可以注掉******/
/* BEGIN: PN:dongle121,chendongguo, 2013/6/28 added/deleted/modified*/
typedef enum
{
    CHG_DISP_OK,
    CHG_DISP_FAIL,
    CHG_DISP_BATTERY_LOWER,  /* LOW BATTERY   */
    CHG_DISP_BATTERY_BAD,  /*BATTERY ERROR*/
    CHG_DISP_OVER_HEATED,/* Over-heated */
    CHG_DISP_TEMP_LOW, /* Temp. Too Low */
    CHG_DISP_CHARGING, /*charging*/
    CHG_DISP_MAX
}CHG_BATT_DISPLAY_TYPE;

/* END: PN:dongle121,chendongguo, 2013/6/28 added/deleted/modified*/

/*----------------------------------------------*
 * 结构定义                                       *
 *----------------------------------------------*/
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
}CHG_TEMP2ADC_TYPE;
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
#endif
/**********************************************************************
FUNCTION:    chg_boot_set_power_off
DESCRIPTION: The poweroff func of CHG module, all the power-off operation
             except at boot phase MUST be performed via calling this.
INPUT:       The shutdown reason which triggered system poweroff.
             All VALID REASON:
             DRV_SHUTDOWN_BATTERY_ERROR --BATTERY ERROR;
             DRV_SHUTDOWN_TEMPERATURE_PROTECT --EXTREAM HIGH TEMPERATURE.
             DRV_SHUTDOWN_LOW_TEMP_PROTECT --EXTREAM LOW TEMPERATURE
             DRV_SHUTDOWN_CHARGE_REMOVE --CHGR REMOVAL WHILE POWEROFF CHG
             DRV_SHUTDOWN_LOW_BATTERY --LOW BATTERY
OUTPUT:      None.
RETURN:      None.
NOTE:        When this function get called to power system off, it record
             the shutdown reason, then simulate POWER_KEY event to APP to
             perform the real system shutdown process.
             THUS, THIS FUNCTION DOESN'T TAKE AFFECT IF APP DIDN'T STARTUP.
***********************************************************************/
void chg_boot_set_power_off(DRV_SHUTDOWN_REASON_ENUM real_reason);

/* Begin daizhicheng 2013-07-01 added */
/*****************************************************************************
 函 数 名  : chg_print_level_message.fmt
 功能描述  : LINUX平台通用的打印函数
 输入参数  : int print_level
             const char *fmt...
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年5月8日
    作    者   : chendongguo
    修改内容   : 新生成函数

*****************************************************************************/
extern void chg_print_level_message(int print_level,const char *fmt,...);
/* End daizhicheng 2013-07-01 added */

/*****************************************************************************
 函 数 名  : chg_boot_get_batt_volt_value
 功能描述  : 获取电池电压
 输入参数  : 无
 输出参数  : 无
 返 回 值  : int32_t 电压值
 调用函数  : 无
 被调函数  :

 修改历史      :
  1.日    期   : 2012年11月1日
    修改内容   : 新生成函数

*****************************************************************************/
extern int32_t chg_boot_get_batt_volt_value(void);

/*****************************************************************************
 函 数 名  : chg_boot_get_temp_value
 功能描述  : 获取温度参数
 输入参数  : param_type，参数类型
 输出参数  : 无
 返 回 值  : 获取的温度值
 调用函数  : 无
 被调函数  :

 修改历史      :
  1.日    期   : 2012年11月2日
    修改内容   : 新生成函数

*****************************************************************************/
extern int32_t chg_boot_get_temp_value(CHG_PARAMETER_ENUM param_type);

/**********************************************************************
函 数 名      : chg_get_powdown_charging
功能描述  :  判断当前是否为关机充电模式
输入参数  : 无。
输出参数  : 无。
返 回 值      :     TRUE:  关机充电模式
                               FALSE:  非关机充电模式
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年10月30日
    修改内容   : 新生成函数

***********************************************************************/
extern boolean chg_boot_is_powdown_charging (void);
/**********************************************************************
函 数 名  : chg_boot_set_powdown_charging
功能描述  : 设置当前为关机充电模式
输入参数  : 无。
输出参数  : 无。
返 回 值  : 无
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年10月30日
    修改内容   : 新生成函数

***********************************************************************/
void chg_boot_set_powdown_charging (void);

/*****************************************************************************
 函 数 名  : chg_boot_display_interface
 功能描述  : 封装底软统一显示接口
 输入参数  : CHG_BATT_DISPLAY_TYPE disp_type显示类型
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年10月30日
    修改内容   : 新生成函数

*****************************************************************************/
void chg_boot_display_interface(CHG_BATT_DISPLAY_TYPE disp_type);

extern void kernel_power_off(void);

/**********************************************************************
函 数 名  : chg_boot_is_ftm_mode
功能描述  : 判断目前是否为ftm模式
输入参数  : 无。
输出参数  : 无。
返 回 值  : 目前是否为工厂模式
           TRUE : 工厂模式
           FALSE: 非工厂模式
注意事项  : 无。
***********************************************************************/
boolean chg_boot_is_ftm_mode(void);
/**********************************************************************
函 数 名  : chg_boot_is_no_battery_powerup_enable
功能描述  : 判断目前是否为非工厂模式下的电池不在位开机使能模式
输入参数  : 无。
输出参数  : 无。
返 回 值  : 目前是否为非工厂模式电池不在位开机使能
                           TRUE : 使能
                           FALSE: 非使能
注意事项  : 无。
***********************************************************************/
extern boolean chg_boot_is_no_battery_powerup_enable(void);

/**********************************************************************
函 数 名  : chg_boot_is_exception_poweroff_poweron_enable
功能描述  : 判断目前是否为插入USB强制开机使能模式
输入参数  : 无。
输出参数  : 无。
返 回 值  : 目前是否为插入USB强制开机使能模式
                           TRUE : 使能
                           FALSE: 非使能
注意事项  : 无。
***********************************************************************/
extern boolean chg_boot_is_exception_poweroff_poweron_enable(void);

/*****************************************************************************
 函 数 名  : chg_boot_delay_ms
 功能描述  : boot阶段的延时函数，不同平台需要进行适配
 输入参数  : unsigned int microsecond
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年6月27日
    作    者   : chendongguo
    修改内容   : 新生成函数

*****************************************************************************/
/* modify by daizhicheng */
void chg_boot_delay_ms (uint32_t microsecond);

/*****************************************************************************
 函 数 名  : chg_boot_get_volt_from_adc
 功能描述  : 从ADC通道获取电压值
 输入参数  : param_type通道类型
 输出参数  : 无
 返 回 值  : 读取的电压值
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年10月31日
    修改内容   : 新生成函数

*****************************************************************************/
int32_t chg_boot_get_volt_from_adc(CHG_PARAMETER_ENUM param_type);

#endif/*CHG_BOOT_CHIP_PLATFORM_H*/

