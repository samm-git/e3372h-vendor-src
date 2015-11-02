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
/******************************************************************************************************
    问题单号           时间             修改人                        说明
******************************************************************************************************/

#ifndef __HW_PNP_API_H__
#define __HW_PNP_API_H__
#include "usb_debug.h"
#include "android.h"
#include "hw_pnp.h"

/*****************************************************************************
 函 数 名  :pnp_register_usb_support_function
 功能描述  :向PNP注册功能驱动
 输入参数  :struct android_usb_function* usb_func:需要注册的功能驱动
 输出参数  :
 返 回 值  :
 调用函数  :
 被调函数  :各功能驱动
 修改历史      :
  1.日    期   : 2014-06-10
    修改内容   : 添加注释
*****************************************************************************/
USB_VOID pnp_register_usb_support_function(struct android_usb_function* usb_func);

/*****************************************************************************
 函 数 名  :usb_pnp_port_style_stat
 功能描述  :
 输入参数  :NA
 输出参数  :
 返 回 值  :USB_INT:
 调用函数  :
 被调函数  :
 修改历史      :
  1.日    期   : 2014年6月10日
    修改内容   : 添加注释
*****************************************************************************/
USB_INT usb_pnp_system_type_get(USB_VOID);

/*****************************************************************************
 函 数 名  :usb_pnp_system_type_get
 功能描述  :获取当前USB连接的PC类型
 输入参数  :NA
 输出参数  :
 返 回 值  :USB_INT:
            WINDOWS_OS_FLAG 0x00
            MAC_OS_FLAG     0x10
            LINUX_OS_FLAG   0x20
            GATEWAY_OS_FLAG 0x30
 调用函数  :
 被调函数  :
 修改历史      :
  1.日    期   : 2014年6月10日
    修改内容   : 添加注释
*****************************************************************************/
USB_INT  usb_pnp_port_style_stat(USB_VOID);
/****************************************************************
 函 数 名  : pnp_if_gateway_mode
 功能描述  : 查询是否网关模式。
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NA
*****************************************************************/
USB_INT  pnp_if_gateway_mode(USB_VOID);
/*****************************************************************************
 函 数 名  :get_gateway_mode
 功能描述  :获取网关当前上报的端口模式
 输入参数  :NA
 输出参数  :USB_INT
 返 回 值  :GATEWAY_NDIS_MODE 支持NDIS工作模式
            ；GATEWAT_MODEM_MODE:支持modem工作模式
 调用函数  :
 被调函数  :
 修改历史      :
  1.日    期   : 2014年6月10日
    修改内容   : 添加注释
*****************************************************************************/
USB_INT get_gateway_mode(USB_VOID);
/****************************************************************
 函 数 名  : debug_init
 功能描述  : debug初始化。
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NA
*****************************************************************/
USB_VOID pnp_probe(USB_VOID);
/****************************************************************
 函 数 名  : pnp_remove
 功能描述  : pnp移除接口
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NA
*****************************************************************/
USB_VOID pnp_remove(USB_VOID);

/****************************************************************
 函 数 名  : pnp_set_rewind_param
 功能描述  : 设置切换命令
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NA
*****************************************************************/
USB_VOID pnp_set_rewind_param(USB_UINT8* cmnd);
/****************************************************************
 函 数 名  : pnp_is_service_switch
 功能描述  : 是否服务切换(RNDIS MASS GET MAX LUN切换)
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NA
*****************************************************************/
USB_INT pnp_is_service_switch(USB_VOID);
/****************************************************************
 函 数 名  : pnp_is_rewind_before_mode
 功能描述  : 是否切换前(是否满足切换条件)
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NA
*****************************************************************/
USB_INT pnp_is_rewind_before_mode(USB_VOID);
/****************************************************************
 函 数 名  : pnp_is_rewind_before_mode
 功能描述  : 是否服务multi lun条件
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NA
*****************************************************************/
USB_INT pnp_is_multi_lun_mode(USB_VOID);
/****************************************************************
 函 数 名  : pnp_switch_normal_work_mode
 功能描述  : 切换到正常模式(相当于第一次连接单板到PC)
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NA
*****************************************************************/
USB_VOID pnp_switch_normal_work_mode(USB_VOID);
/****************************************************************
 函 数 名  : pnp_switch_mbim_debug_mode
 功能描述  : 切换到MBIM调试模式
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NA
*****************************************************************/
USB_VOID pnp_switch_mbim_debug_mode(USB_VOID);
/****************************************************************
 函 数 名  : pnp_switch_rndis_debug_mode
 功能描述  : 切换到RNDIS调试模式
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NA
*****************************************************************/
USB_VOID pnp_switch_rndis_debug_mode(USB_VOID);
/****************************************************************
 函 数 名  : pnp_switch_rndis_project_mode
 功能描述  : 切换到RNDIS工程模式
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NA
*****************************************************************/
USB_VOID pnp_switch_rndis_project_mode(USB_VOID);
/****************************************************************
 函 数 名  : pnp_switch_rewind_after_mode
 功能描述  : 切换到工作模式
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NA
*****************************************************************/
USB_VOID pnp_switch_rewind_after_mode(USB_VOID);
/****************************************************************
 函 数 名  : usb_notify_syswatch
 功能描述  : USB事件
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NA
*****************************************************************/
void usb_notify_syswatch(int deviceid, int eventcode);
/****************************************************************
 函 数 名  : pnp_switch_autorun_port
 功能描述  : pnp重新运行接口
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NA
*****************************************************************/
USB_VOID pnp_switch_autorun_port(USB_VOID);

/****************************************************************
 函 数 名  : huawei_set_usb_enum_state
 功能描述  : 设置枚举状态
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NA
*****************************************************************/
USB_VOID huawei_set_usb_enum_state(usb_enum_state state);

/****************************************************************
 函 数 名  : huawei_get_usb_enum_state
 功能描述  : 查询枚举状态
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 枚举状态值
*****************************************************************/
usb_enum_state huawei_get_usb_enum_state( USB_VOID );

/****************************************************************
 函 数 名  : usb_power_off_chg_stat
 功能描述  : 查询关机状态
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NA
*****************************************************************/
USB_INT usb_power_off_chg_stat(USB_VOID);
/****************************************************************
 函 数 名  : pnp_set_ctl_app_flag
 功能描述  : 设置app是否启动标志
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NA
*****************************************************************/
USB_VOID pnp_set_ctl_app_flag(USB_INT flag);
/*****************************************************************************
 函 数 名  : pnp_switch_charge_only_port
 功能描述  : 切换功能函数:切换到仅充电端口状态
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NA
*****************************************************************************/
USB_VOID pnp_switch_charge_only_port(USB_VOID);
/*****************************************************************************
 函 数 名  : pnp_switch_charge_only_port
 功能描述  : 查询当前端口形态是否静态端口
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NA
*****************************************************************************/
USB_INT  pnp_if_static_port_mode(USB_VOID);
/*****************************************************************************
 函 数 名  : pnp_if_cdrom_can_open
 功能描述  : 查询当前cdrom是否可读
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NA
*****************************************************************************/
USB_INT pnp_if_cdrom_can_open(USB_VOID);

/*****************************************************************************
 函 数 名  : get_nv_backup_flag
 功能描述  : 获取NV备份恢复阶段的共享内存变量
 输入参数  : 无
 输出参数  : 无
 返 回 值  : USB_INT
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年9月17日
    作    者   : liulimin
    修改内容   : 新生成函数

*****************************************************************************/
USB_INT get_nv_backup_flag(USB_VOID);

#ifdef CONFIG_NCM_MBIM_SUPPORT
USB_VOID pnp_switch_mbim_mode(USB_INT mode);
#endif

USB_VOID huawei_set_adress_flag(USB_INT state);

USB_INT huawei_get_adress_flag(USB_VOID);

#endif
