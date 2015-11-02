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
#include "chg_config.h"
#if (MBB_CHG_PLATFORM_V7R2 == FEATURE_ON)
#include <bsp_nvim.h>
#endif

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

/*Keep ret-code and data of intercore calling.*/
static int32_t chg_intercore_reply_data = 0;
static int32_t chg_intercore_reply_ret  = CHG_ERROR;

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

static const chg_hw_param_t chg_pt_chgr_hw_paras[] =
{
    /*0:PT_Enable_Charge Setting*/
    {
         500,  /*mA, Power Supply front-end Current limit.*/
         576,  /*mA, Charge Current limit.*/
        4200,  /*mV, CV Voltage setting.*/
         256,  /*mA, Taper(Terminate) current.*/
         TRUE  /*If charge enabled: FALSE:-Disable, TRUE:-Enable.*/
    },
    {

    }

};

static CHG_TCHRENABLE_TYPE g_chg_tchrenable_status = 
{
        -1,
        -1
};

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/

/*****************************************************************************
 函 数 名  : app_get_battery_state
 功能描述  : Oled调用此函数获取充电状态以及电池电量，
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
int app_get_battery_state(BATT_STATE_T *battery_state)
{
    boolean power_off_charging_flag = FALSE;
    chg_stm_state_type real_charger_state = CHG_STM_INIT_ST;

    if(NULL == battery_state)
    {
        return CHG_ERROR;
    }

    battery_state->battery_level = chg_get_batt_level();
    battery_state->batt_temp_state = chg_get_batt_temp_state();
    real_charger_state = chg_stm_get_cur_state();
    power_off_charging_flag = chg_is_powdown_charging();

    if(FALSE == power_off_charging_flag)
    {
        /*如果目前为非关机充电模式，是否充电根据实际状态
        显示**/
        if((CHG_STM_FAST_CHARGE_ST == real_charger_state) \
            || (CHG_STM_WARMCHG_ST == real_charger_state))
        {
            battery_state->charging_state = CHARGING_UP;
        }
        else
        {
            battery_state->charging_state = NO_CHARGING_UP;
        }
    }
    else
    {
        /* 目前进入关机充电后初始化状态也认为是充电
        模式此时的关机充电动画已经开启***/
        if((CHG_STM_FAST_CHARGE_ST == real_charger_state) \
           || (CHG_STM_TRANSIT_ST  == real_charger_state) \
           || (CHG_STM_WARMCHG_ST == real_charger_state))
        {
            battery_state->charging_state = CHARGING_DOWN;
        }
        else
        {
            battery_state->charging_state = NO_CHARGING_DOWN;
        }
    }

    chg_print_level_message(CHG_MSG_DEBUG, "CHG_API: app charging_state =%d !!!\r\n", battery_state->charging_state);
    chg_print_level_message(CHG_MSG_DEBUG, "CHG_API: app battery_level =%d !!!\r\n", battery_state->battery_level);
    chg_print_level_message(CHG_MSG_DEBUG, "CHG_API: app batt_temp_state =%d !!!\r\n", battery_state->batt_temp_state);
    return CHG_OK;
}

#if (MBB_CHG_PLATFORM_V7R2 == FEATURE_ON)
/**********************************************************************
函 数 名  : chg_tbat_read
功能描述  : AT读接口
输入参数  : 操作方式:读(校准前/后)电池电量还是读电压校准值
输出参数  : 电池电量/电池电压校准值
返 回 值  : 是否操作成功
注意事项  : 
***********************************************************************/
int chg_tbat_read(ENUM_CHG_ATCM_TYPE atID, void *pItem)
{
    int32_t ret = 0;

    if(NULL == pItem)
    {
        chg_print_level_message(CHG_MSG_ERR,"invalid parameter!!\n");
        return CHG_ERROR;
    }
    switch(atID)
    {
        case CHG_AT_BATTERY_CHECK:
        {
            /*读取电池电量校准值*/
            VBAT_CALIBRATION_TYPE * ptr = (VBAT_CALIBRATION_TYPE *)pItem;
            VBAT_CALIBRATION_TYPE tbat_check;

            ret = bsp_nvm_read(NV_BATT_VOLT_CALI_I, &tbat_check, sizeof(VBAT_CALIBRATION_TYPE));
            if(0 == ret)
            {
                ptr->max_value = tbat_check.max_value;
                ptr->min_value = tbat_check.min_value;
            }
            else
            {
                chg_print_level_message(CHG_MSG_ERR,"read chg nv failed when read check value\n");
                return CHG_ERROR;
            }
        }
        break;
        case CHG_AT_BATTERY_LEVEL:
        {
            /*读取校准后的电池电压*/
            *(int32_t*)pItem = chg_get_batt_volt_value();
        }
        break;

        /*校准新增AT接口*/
        case CHG_AT_BATTERY_ADC:
        {
            /*读取电池未校准的电池电压*/
            *(int32_t*)pItem = chg_get_volt_from_adc( CHG_PARAMETER__BATTERY_VOLT );
        }
        break;
        /*获取电池电量*/
        case CHG_AT_BATTERY_CAPACITY:
        {
            /*读取电池电量，暂未实现*/
            //*(int32_t*)pItem = chg_get_batt_volt_value();
        }
        break;

        default:
            return CHG_ERROR;
    }
    return  CHG_OK;
}
/**********************************************************************
函 数 名  : chg_tbat_write
功能描述  : AT写接口:设置电池电量校准值
输入参数  : 4.2V和3.4V对应的ADC采样值
输出参数  : 无
返 回 值  : 是否操作成功
注意事项  : 无
***********************************************************************/
int chg_tbat_write(ENUM_CHG_ATCM_TYPE atID, void *pItem)
{
    /*设置电池电量校准值*/
    int32_t ret = 0;
    VBAT_CALIBRATION_TYPE * ptr = (VBAT_CALIBRATION_TYPE *)pItem;
    VBAT_CALIBRATION_TYPE tbat_org = {0};

    if(NULL == pItem)
    {
        chg_print_level_message(CHG_MSG_ERR,"invalid parameter!!\n");
        return CHG_ERROR;
    }

    switch(atID)
    {
        case CHG_AT_BATTERY_CHECK:
        {
            ret = bsp_nvm_read(NV_BATT_VOLT_CALI_I, &tbat_org, sizeof(VBAT_CALIBRATION_TYPE));
            if(ret)
            {
                chg_print_level_message(CHG_MSG_ERR,"read chg nv failed!!!\n");
                return CHG_ERROR;
            }
            
            if(TBAT_CHECK_INVALID == ptr->max_value)
            {
                ptr->max_value = tbat_org.max_value;
            }
            if(TBAT_CHECK_INVALID == ptr->min_value)
            {
                ptr->min_value = tbat_org.min_value;
            }
            
            ret = bsp_nvm_write(NV_BATT_VOLT_CALI_I, ptr, sizeof(VBAT_CALIBRATION_TYPE));
            if(ret)
            {
                chg_print_level_message(CHG_MSG_ERR,"write chg nv failed when set check value\n", 0, 0, 0);
                return CHG_ERROR;
            }


        }
        break;

        default:
            return CHG_ERROR;
    }
    return  CHG_OK;
}
/**********************************************************************
函 数 名  : chg_tbat_charge_mode_set
功能描述  : 设置电池充电或者放电
输入参数  : 命令参数
输出参数  : none
返 回 值  : CHG_OK:success,CHG_ERROR:fail
注意事项  : 
***********************************************************************/
int chg_tbat_charge_mode_set(ENUM_CHG_MODE_TYPE cmd, unsigned int arg)
{
    int ret = 0;
    g_chg_tchrenable_status.chg_state = cmd;
    g_chg_tchrenable_status.chg_mode = arg;
    switch(cmd)
    {
        /*PT工位，打开充电*/
        case CHG_AT_CHARGE_ENABLE:
        {
            //BQ24192不支持涓充和快充模式的设置
            //chg_set_hardware_parameter(&chg_pt_chgr_hw_paras[0]);
            chg_pt_mmi_test_proc();
        }
        break;
        /*关闭充电*/
        case CHG_AT_CHARGE_DISALBE:
        {
            (void)chg_set_charge_enable(FALSE);
        }
        break;
        /*打开放电*/
        case CHG_AT_DISCHARGE_ENABLE:
        {
            (void)chg_set_suspend_mode(TRUE);
        }
        break;
        /*关闭放电*/
        case CHG_AT_DISCHARGE_DISALBE:
        {
            (void)chg_set_suspend_mode(FALSE);
        }
        break;
        /*打开补电*/
        case CHG_AT_START_CHARGE_SPLY:
        {
            ret = chg_tbat_chg_sply();
            return ret;
        }
        break;

        default:
            return CHG_ERROR;
    }
    return  CHG_OK;
}
/**********************************************************************
函 数 名  :  chg_tbat_status_get
功能描述  :  TBAT AT^TCHRENABLE?是否需要补电

输入参数  : 无
输出参数  : 无
返 回 值      : 1:需要补电
			    0:不需要补电
注意事项  : 无
***********************************************************************/
int chg_tbat_is_batt_status_ok(void)
{
    return chg_tbat_status_get();
}

/**********************************************************************
函 数 名  : chg_tbat_get_charging_status
功能描述  : 查询当前充电状态
输入参数  : none
输出参数  : none
返 回 值  : 1:charging,0:no
注意事项  : 
***********************************************************************/
int chg_tbat_get_charging_status(void)
{
    return chg_is_IC_charging();
}

/**********************************************************************
函 数 名  : chg_tbat_get_charging_status
功能描述  : 查询当前充电模式
输入参数  : none
输出参数  : none
返 回 值  : 0:非充电；1：涓充；2：快充
注意事项  : 
***********************************************************************/
int chg_tbat_get_charging_mode(void)
{
    chg_status_type  chg_stat = {0};
    int op_result = TRUE;

    op_result = chg_get_IC_status(&chg_stat);
    if (FALSE == op_result)
    {
        chg_print_level_message(CHG_MSG_ERR,"get IC status failed!!\n");
        return -1;
    }

    return chg_stat.chg_chrg_stat;
}
/**********************************************************************
函 数 名  : chg_tbat_get_discharging_status
功能描述  : 查询当前是否是suspend模式
输入参数  : none
输出参数  : none
返 回 值  : 1:yes,0:no
注意事项  : 
***********************************************************************/
int chg_tbat_get_discharging_status(void)
{
    return chg_get_suspend_status();
}

/**********************************************************************
函 数 名  : chg_tbat_sfeature_inquiry_extchg
功能描述  : 查询是否支持对外充电
输入参数  : none
输出参数  : none
返 回 值  : 1:yes,0:no
注意事项  : 
***********************************************************************/
int chg_tbat_sfeature_inquiry_extchg(void)
{
#if (MBB_CHG_EXTCHG == FEATURE_ON)
    return TRUE;
#else
    return FALSE;
#endif
}
/**********************************************************************
函 数 名  : chg_tbat_sfeature_inquiry_wireless
功能描述  : 查询是否支持无线充电
输入参数  : none
输出参数  : none
返 回 值  : 1:yes,0:no
注意事项  : 
***********************************************************************/
int chg_tbat_sfeature_inquiry_wireless(void)
{
#if (MBB_CHG_WIRELESS == FEATURE_ON)
    return TRUE;
#else
    return FALSE;
#endif
}
/**********************************************************************
函 数 名  : chg_tbat_sfeature_inquiry_battery
功能描述  : 查询电池电压典型值
输入参数  : none
输出参数  : none
返 回 值  : 
注意事项  : 
***********************************************************************/
int chg_tbat_sfeature_inquiry_battery(void)
{
    return -1;
}

/**********************************************************************
函 数 名  : chg_MNTN_get_batt_state
功能描述  : AT读接口
输入参数  : 查询是否是可拆卸电池
输出参数  : none
返 回 值  : 1:可拆卸，2：不可拆卸
注意事项  : 
***********************************************************************/
int chg_MNTN_get_batt_state(void)
{
    /*需要根据具体产品来确定是否是可拆卸电池*/
#if (MBB_CHG_BAT_KNOCK_DOWN == FEATURE_ON)
    return KNOCK_DOWN_BATT_MODE;
#elif (MBB_CHG_BAT_KNOCK_DOWN == FEATURE_OFF)
    return FIX_BATT_MODE;
#else
    return NO_BATT_MODE;
#endif
}
/**********************************************************************
函 数 名  : chg_wireless_mmi_test
功能描述  : AT读接口，无线充电电路检测
输入参数  : none
输出参数  : none
返 回 值  : 
注意事项  : 
***********************************************************************/
int chg_wireless_mmi_test(void)
{
    return -1;
}
/**********************************************************************
函 数 名  : chg_extchg_mmi_test
功能描述  : AT读接口，对外充电电路检测
输入参数  : none
输出参数  : none
返 回 值  : 
注意事项  : 
***********************************************************************/
int chg_extchg_mmi_test(void)
{
    return -1;
}

/**********************************************************************
函 数 名  : chg_extchg_mmi_test
功能描述  : AT读接口，获取AT^TCHRENABLE设置的结果
输入参数  : none
输出参数  : none
返 回 值  : 
注意事项  : 
***********************************************************************/
int chg_tbat_get_tchrenable_status(CHG_TCHRENABLE_TYPE *tchrenable_state)
{
    if(NULL == tchrenable_state)
    {
        return -1;
    }

    *tchrenable_state = g_chg_tchrenable_status;

    return 0;
}
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
int chg_getCbcState(unsigned char  *pucBcs ,unsigned char   *pucBcl)
{
    return 0;
}

/*****************************************************************************
 函 数 名  : chg_get_coulometer_support
 功能描述  : 
 输入参数  : 无
 输出参数  : BATT_STATE_T *battery_state，输出电池格数和充电状态
 返 回 值  : 0 不支持, 1 支持
 调用函数  :
 被调函数  :
 注      意: 
 修改历史      :
  1.日    期   : 2014年01月15日
    作    者   : 王丽 00249966
    修改内容   : 新生成函数

*****************************************************************************/
int chg_get_coulometer_support(void)
{
#if (MBB_CHG_COULOMETER == FEATURE_ON || MBB_CHG_BQ27510 == FEATURE_ON)
    return YES;
#else 
    return NO;
#endif
}

#endif
/**********************************************************************
函 数 名  : chg_usb_state_callback
功能描述  : 用于获取核间调用函数:usb_enum_completed()和usb_charge_type()
            的返回值。
输入参数  : STA:返回的状态值。
输出参数  : 无。
返 回 值  : 无。
注意事项  : 无。
**********************************************************************/
int chg_usb_state_callback(int STA, int Msg_Error)
{
    chg_intercore_reply_data = (int32_t)STA;
    chg_intercore_reply_ret =  (int32_t)Msg_Error;

    if (CHG_OK == chg_intercore_reply_ret)
    {
        return STA;
    }
    else
    {
        return Msg_Error;
    }
}

