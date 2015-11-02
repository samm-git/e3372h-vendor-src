#ifndef CHG_CHARGE_ADC_H
#define CHG_CHARGE_ADC_H

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
/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/

/********Note:以下头文件跟平台强相关，移植人员根据平台需要添加*******/
#include <linux/kernel.h>
#include <linux/err.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/jiffies.h>
#include <linux/timer.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/printk.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <linux/gpio.h>
#include <asm/div64.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

/*I2C操作头文件*/
#include "bsp_i2c.h"
/*NV操作头文件*/
#include "bsp_nvim.h"
/*定时器操作头文件*/
#include "bsp_softtimer.h"

#include <linux/syscalls.h>
#include <asm/unaligned.h>

#include <linux/spinlock.h>
#ifndef CHG_STUB
#include <linux/mlog_lib.h>
#else
#define  mlog_print(format, ...)
#endif
/********Note:以下头文件跟平台强相关，移植人员根据平台需要添加*******/

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/
extern void kernel_power_off(void);

/*----------------------------------------------*
 * 宏定义说明                                   *
 *----------------------------------------------*/

#if (MBB_CHG_EXTCHG == FEATURE_ON)
/*根据评审意见将所有GPIO号使用宏定义代替*/
#if (FEATURE_ON == MBB_CHG_PLATFORM_V7R2)
//#define EXTCHG_CONTROL_GPIO_EN1       GPIO_0_11 /* V7R2平台只控制EN1，EN2由硬件强制拉低*/
#else
#define EXTCHG_CONTROL_GPIO_EN1       59
#define EXTCHG_CONTROL_GPIO_EN2       60
#endif
#endif/*MBB_CHG_EXTCHG*/

#if (MBB_CHG_WIRELESS == FEATURE_ON)
/*根据评审意见将所有GPIO号使用宏定义代替*/
#if (FEATURE_ON == MBB_CHG_PLATFORM_V7R2)
#define WIRELESS_CONTROL_GPIO_EN1       GPIO_0_11 /* V7R2平台只控制EN1，EN2由硬件强制拉低*/
#else
#define WIRELESS_CONTROL_GPIO_EN1       59
#endif
#define WIRELESS_CONTROL_GPIO_EN2       60
#define USB_WIRELESS_CHGR_DET    0x0006
#endif/*MBB_CHG_WIRELESS*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/
/**********************************************************************
FUNCTION:    chg_send_stat_to_app
DESCRIPTION: Charge module sends charge state to application layer.
INPUT:       uint32_t chg_device_id,
             uint32_t chg_event_id
OUTPUT:      None.
RETURN:      None.
NOTE:        None
***********************************************************************/
void chg_send_stat_to_app(uint32_t chg_device_id, uint32_t chg_event_id);
/**********************************************************************
FUNCTION:    CHG_SET_POWER_OFF
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
void chg_set_power_off(DRV_SHUTDOWN_REASON_ENUM real_reason);

/*****************************************************************************
 函 数 名  : chg_get_volt_from_adc
 功能描述  : 从ADC通道获取电压值
 输入参数  : param_type通道类型
 输出参数  : 无
 返 回 值  : 读取的电压值
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年10月31日
    作    者   : chendongguo
    修改内容   : 新生成函数

*****************************************************************************/
extern int32_t chg_get_volt_from_adc(CHG_PARAMETER_ENUM param_type);


/*****************************************************************************
 函 数 名  : chg_get_batt_volt_value
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
extern int32_t chg_get_batt_volt_value(void);

/*****************************************************************************
 函 数 名  : chg_get_temp_value
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
extern int32_t chg_get_temp_value(CHG_PARAMETER_ENUM param_type);

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
extern boolean chg_is_powdown_charging (void);

/*****************************************************************************
 函 数 名  : get_chgr_type_from_usb
 功能描述  : Get real charger type from USB module.
             This function called the platform interfaces to obtain the real
             charger type from USB module.
             !!Platform dependent, currently we implemented as Balong V700R001
             design.
 输入参数  : VOID
 输出参数  : VOID
 返 回 值  : Charger type of state machine needed.
 CALL-WHOM : Platform interfaces.
 WHO-CALLED: chg_check_and_update_hw_param_per_chgr_type
             chg_transit_state_period_func
 NOTICE    : 1. Platform chgr_type enum may be different from chg_stm design,
             we need convert or remap them before return.
             2. Balong V3R2/V7R1 platform didn't support CHG_USB_HOST_PC,
             CHG_NONSTD_CHGR and CHG_USB_HOST_PC would all be treated as
             CHG_NONSTD_CHGR.
             3. Export Required.
*****************************************************************************/
extern chg_chgr_type_t get_chgr_type_from_usb(ulong64_t plug,int32_t chg_type);

/*****************************************************************************
 函 数 名  : chg_ display_interface
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
void chg_display_interface(CHG_BATT_DISPLAY_TYPE disp_type);

/*****************************************************************************
 函 数 名  : chg_batt_volt_calib_init
 功能描述  : 电池电压校准中参数获取
 输入参数  : uint16 min电池电压校准NV中获取的最小校准电压值, uint16 max，最大校准电压值
 输出参数  : g_vbatt_max，g_vbatt_min，g_is_batt_volt_calib_init
 返 回 值  : 读取的电压值
 调用函数  : 无
 被调函数  : 初始化时读取NV项后调用该函数，设置min和max值

 修改历史      :
  1.日    期   : 2012年11月1日
    修改内容   : 新生成函数

*****************************************************************************/
/***************Note:平台相关代码，根据平台按需要添加，有的平台如9x25需要
              移植人员根据需要，添加或者移除下边函数调用***************************/
void chg_batt_volt_calib_init(void);
/*****************************************************************************
 函 数 名  : chg_get_volt_from_adc
 功能描述  : 从ADC通道获取电压值
 输入参数  : param_type通道类型
 输出参数  : 无
 返 回 值  : 读取的电压值
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年10月31日
    作    者   : chendongguo
    修改内容   : 新生成函数

*****************************************************************************/
int32_t chg_get_volt_from_adc(CHG_PARAMETER_ENUM param_type);

#if (MBB_CHG_EXTCHG == FEATURE_ON)
/*****************************************************************************
 函 数 名  : extchg_gpio_init
 功能描述  : 对外充电控制的GPIO初始化，不同平台及硬件设计不同需要配置
 输入参数  : void
 输出参数  : 无
 返 回 值  :N/A
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年5月28日
    作    者   : chendongguo
    修改内容   : 新生成函数

*****************************************************************************/
extern int extchg_gpio_init(void);

/*****************************************************************************
 函 数 名  : extchg_gpio_control
 功能描述  : 对外充电GPIO控制函数
 输入参数  : gpio:需要控制的GPIO
             level:高低电平
 输出参数  : 无
 返 回 值  :N/A
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年5月28日
    作    者   : chendongguo
    修改内容   : 新生成函数

*****************************************************************************/
extern void extchg_gpio_control(uint32_t gpio,int32_t level);
#endif/*MBB_CHG_EXTCHG*/


#if (MBB_CHG_WIRELESS == FEATURE_ON)
/*****************************************************************************
 函 数 名  : wireless_gpio_init
 功能描述  : 初始化控制GPIO
 输入参数  : void
 输出参数  : 无
 返 回 值  :N/A
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年5月11日
    作    者   : chendongguo
    修改内容   : 新生成函数

*****************************************************************************/
extern int wireless_gpio_init(void);
/*****************************************************************************
 函 数 名  : wireless_gpio_control
 功能描述  : 初始化控制GPIO
 输入参数  : gpio:需要控制的GPIO
             level:高低电平
 输出参数  : 无
 返 回 值  :N/A
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年5月11日
    作    者   : chendongguo
    修改内容   : 新生成函数

*****************************************************************************/
extern void wireless_gpio_control(uint32_t gpio,int32_t level);
#endif/*MBB_CHG_WIRELESS*/

/*****************************************************************************
 函 数 名  : is_in_update_mode
 功能描述  : 获取当前是否在升级模式，如果是升级模式则不启动充电任务
 输入参数  : NA
             NA
 输出参数  : TURE:是升级模式
             FALSE:不是升级模式
 返 回 值  :N/A
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年5月11日
    作    者   : chendongguo
    修改内容   : 新生成函数

*****************************************************************************/
extern boolean is_in_update_mode(void);

/**********************************************************************
函 数 名  : boolean chg_config_para_read(int32_t num)
功能描述  : 读充电可配置参数
输入参数  : none
输出参数  : 无。
返 回 值  : 无。
注意事项  : 可根据平台不同修改实现方法，比如NV等。
***********************************************************************/
boolean chg_config_para_read(uint16_t nvID, void *pItem, uint32_t ulLength);

/*****************************************************************************
 函 数 名  : chg_pt_mmi_test_proc
 功能描述  : PT工位使能充电MMI检测处理函数
 输入参数  : 无
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年7月12日
    作    者   : chendongguo
    修改内容   : 新生成函数

*****************************************************************************/
void chg_pt_mmi_test_proc(void);

#if (MBB_CHG_COMPENSATE == FEATURE_ON)
/*****************************************************************************
 函 数 名  : chg_set_fact_release_mode
 功能描述  : 设置在产线充电状态利用SMEM将放电的标志位传至modem侧
 输入参数  : TRUE:开始大电流放电
             FALSE:关闭             
 输出参数  : NA
 返 回 值  : NA
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年6月20日
    修改内容   : 新生成函数

*****************************************************************************/
void chg_set_fact_release_mode(boolean on);
#endif



#endif

