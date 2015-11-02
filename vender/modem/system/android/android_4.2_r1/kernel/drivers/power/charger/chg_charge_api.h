#ifndef CHG_CHARGE_API_H
#define CHG_CHARGE_API_H

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
 * 外部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/
#include "chg_config.h"


#if (MBB_CHG_PLATFORM_V7R2 == FEATURE_ON)
/**********************************************************************
函 数 名  : chg_tbat_read
功能描述  : AT读接口
输入参数  : 操作方式:读(校准前/后)电池电量还是读电压校准值
输出参数  : 电池电量/电池电压校准值
返 回 值  : 是否操作成功
注意事项  : 
***********************************************************************/
int chg_tbat_read(ENUM_CHG_ATCM_TYPE atID, void *pItem);

/**********************************************************************
函 数 名  : chg_tbat_write
功能描述  : AT写接口:设置电池电量校准值
输入参数  : 4.2V和3.4V对应的ADC采样值
输出参数  : 无
返 回 值  : 是否操作成功
注意事项  : 无
***********************************************************************/
int chg_tbat_write(ENUM_CHG_ATCM_TYPE atID, void *pItem);

/**********************************************************************
函 数 名  : chg_tbat_charge_mode_set
功能描述  : 设置电池充电或者放电
输入参数  : 命令参数
输出参数  : none
返 回 值  : CHG_OK:success,CHG_ERROR:fail
注意事项  : 
***********************************************************************/
int chg_tbat_charge_mode_set(ENUM_CHG_MODE_TYPE cmd, unsigned int arg);

/**********************************************************************
函 数 名  :  chg_tbat_status_get
功能描述  :  TBAT AT^TCHRENABLE?是否需要补电

输入参数  : 无
输出参数  : 无
返 回 值      : 1:需要补电
			    0:不需要补电
注意事项  : 无
***********************************************************************/
int chg_tbat_is_batt_status_ok(void);
/**********************************************************************
函 数 名  : chg_tbat_get_charging_status
功能描述  : 查询当前充电状态
输入参数  : none
输出参数  : none
返 回 值  : 1:charging,0:no
注意事项  : 
***********************************************************************/
int chg_tbat_get_charging_status(void);
/**********************************************************************
函 数 名  : chg_tbat_get_charging_status
功能描述  : 查询当前充电模式
输入参数  : none
输出参数  : none
返 回 值  : 0:非充电；1：涓充；2：快充
注意事项  : 
***********************************************************************/
int chg_tbat_get_charging_mode(void);
/**********************************************************************
函 数 名  : chg_tbat_get_discharging_status
功能描述  : 查询当前是否是suspend模式
输入参数  : none
输出参数  : none
返 回 值  : 1:yes,0:no
注意事项  : 
***********************************************************************/
int chg_tbat_get_discharging_status(void);
/**********************************************************************
函 数 名  : chg_tbat_sfeature_inquiry_extchg
功能描述  : 查询是否支持对外充电
输入参数  : none
输出参数  : none
返 回 值  : 1:yes,0:no
注意事项  : 
***********************************************************************/
int chg_tbat_sfeature_inquiry_extchg(void);
/**********************************************************************
函 数 名  : chg_tbat_sfeature_inquiry_battery
功能描述  : 查询电池电压典型值
输入参数  : none
输出参数  : none
返 回 值  : 
注意事项  : 
***********************************************************************/
int chg_tbat_sfeature_inquiry_battery(void);
/**********************************************************************
函 数 名  : chg_MNTN_get_batt_state
功能描述  : AT读接口
输入参数  : 查询是否是可拆卸电池
输出参数  : none
返 回 值  : 1:可拆卸，2：不可拆卸
注意事项  : 
***********************************************************************/
int chg_MNTN_get_batt_state(void);
/**********************************************************************
函 数 名  : chg_wireless_mmi_test
功能描述  : AT读接口，无线充电电路检测
输入参数  : none
输出参数  : none
返 回 值  : 
注意事项  : 
***********************************************************************/
int chg_wireless_mmi_test(void);
/**********************************************************************
函 数 名  : chg_extchg_mmi_test
功能描述  : AT读接口，对外充电电路检测
输入参数  : none
输出参数  : none
返 回 值  : 
注意事项  : 
***********************************************************************/
int chg_extchg_mmi_test(void);
/**********************************************************************
函 数 名  : chg_extchg_mmi_test
功能描述  : AT读接口，获取AT^TCHRENABLE设置的结果
输入参数  : none
输出参数  : none
返 回 值  : 
注意事项  : 
***********************************************************************/
int chg_tbat_get_tchrenable_status(CHG_TCHRENABLE_TYPE *tchrenable_state);
/************************************************************************  
函 数 名  : chg_getCbcState
功能描述  : 在线升级AT^CBC
         pucBcs
            0: 电池在供电
            1: 与电池连接电池未供电
            2: 没有与电池连接
         pucBcl
            0:电量不足或没有与电池连接
             100: 剩余电量百分比
输入参数  : none
输出参数  : pucBcs，pucBcl
返 回 值  : 
注意事项  : 0 Success, -1 failed  
 ***********************************************************************/
int chg_getCbcState(unsigned char  *pucBcs ,unsigned char   *pucBcl);

/************************************************************************  
函 数 名  : chg_get_coulometer_support
功能描述  : 查询当前产品是否支持库仑计
输入参数  : none
输出参数  : none
返 回 值  : NO-0 不支持，YES-1 支持
注意事项  : 0 Success, -1 failed  
 ***********************************************************************/
int chg_get_coulometer_support(void);

#endif
/*****************************************************************************
 函 数 名  : app_get_battery_state
 功能描述  : Oled调用此函数获取充电状态以及电池电量,使用power supply机制时此函数不在对应用开放
 输入参数  : 无
 输出参数  : BATT_STATE_T *battery_state，输出电池格数和充电状态
 返 回 值  : 0 成功, -1 失败
 调用函数  :
 被调函数  :
 注      意: 使用power supply机制时此函数不在对应用开放,非LINUX系统需要打开此函数，同时在定义的地方
             去掉static声明；

 修改历史      :
  1.日    期   : 2012年10月30日
    修改内容   : 新生成函数

*****************************************************************************/
#if 0 /*使用power supply机制时此函数不在对应用开放,非LINUX系统需要打开此函数，同时在定义的地方去掉static声明；*/
int app_get_battery_state(BATT_STATE_T *battery_state);
#endif

/**********************************************************************
函 数 名      : chg_usb_state_callback
功能描述  : 用于获取核间调用函数:usb_enum_completed()和usb_charge_type()
                             的返回值。
输入参数  : STA:返回的状态值。
输出参数  : 无。
返 回 值      : 无。
注意事项  : 无。
**********************************************************************/
extern int32_t chg_usb_state_callback(int32_t STA, int32_t Msg_Error);

#endif

