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
#ifndef __F_MASS_STORAGE_API_H__
#define __F_MASS_STORAGE_API_H__
#include "usb_debug.h"
#include "android.h"

/*****************************************************************************
 函 数 名  :usb_mass_storage_init
 功能描述  :mass storage FD 初始化函数，由PNP模块调用
 输入参数  :NA
 输出参数  :
 返 回 值  :USB_INT:
 调用函数  :
 被调函数  :init()
 修改历史      :
  1.日    期   : 2014年6月10日
    修改内容   : 添加注释
*****************************************************************************/
USB_VOID usb_mass_storage_init(USB_VOID);
/*****************************************************************************
 函 数 名  :usb_mass_storage_init
 功能描述  :mass storage 端口增加lun
 输入参数  :NA
 输出参数  :
 返 回 值  :USB_INT:
 调用函数  :
 被调函数  :
 修改历史      :
  1.日    期   : 2014年6月10日
    修改内容   : 添加注释
*****************************************************************************/
USB_VOID mass_function_add_lun(USB_CHAR* function_name, USB_CHAR* addname);
/*****************************************************************************
 函 数 名  :usb_mass_storage_init
 功能描述  :设置SD卡工作模式
 输入参数  :NA
 输出参数  :
 返 回 值  :USB_INT:
 调用函数  :
 被调函数  :
 修改历史      :
  1.日    期   : 2014年6月10日
    修改内容   : 添加注释
*****************************************************************************/
USB_VOID mass_storage_set_sd_card_workmode(USB_INT mode);
/*****************************************************************************
 函 数 名  :usb_mass_storage_init
 功能描述  :USB打开SD卡
 输入参数  :NA
 输出参数  :
 返 回 值  :USB_INT:
 调用函数  :
 被调函数  :
 修改历史      :
  1.日    期   : 2014年6月10日
    修改内容   : 添加注释
*****************************************************************************/
USB_INT  mass_storage_open_usb_sd(USB_VOID);

/*****************************************************************************
 函 数 名  :mass_storage_close_usb_sd
 功能描述  :USB关闭SD卡
 输入参数  :NA
 输出参数  :
 返 回 值  :USB_INT:
 调用函数  :
 被调函数  :
 修改历史      :
  1.日    期   : 2014年6月10日
    修改内容   : 添加注释
*****************************************************************************/
USB_INT mass_storage_close_usb_sd(USB_VOID);

/*****************************************************************************
 函 数 名  :usb_mass_storage_init
 功能描述  :查询是否USB打开SD卡
 输入参数  :NA
 输出参数  :
 返 回 值  :USB_INT:
 调用函数  :
 被调函数  :
 修改历史      :
  1.日    期   : 2014年6月10日
    修改内容   : 添加注释
*****************************************************************************/
USB_INT mass_storage_usb_sd_is_open(USB_VOID);

/*****************************************************************************
 函 数 名  :mass_storage_set_sd_card_status
 功能描述  :设置SD卡插入或者拔出的状态
 输入参数  :NA
 输出参数  :
 返 回 值  :USB_VOID:
 调用函数  :
 被调函数  :
 修改历史      :
  1.日    期   : 2014年6月10日
    修改内容   : 添加注释
*****************************************************************************/
USB_VOID mass_storage_set_sd_card_status(USB_INT sd_removed);

/*****************************************************************************
 函 数 名  :mass_function_cleanup_lun_info
 功能描述  :清空lun信息
 输入参数  :NA
 输出参数  :
 返 回 值  :USB_INT:
 调用函数  :
 被调函数  :
 修改历史      :
  1.日    期   : 2014年6月10日
    修改内容   : 添加注释
*****************************************************************************/
USB_VOID mass_function_cleanup_lun_info(USB_CHAR* function_name);

/*****************************************************************************
 函 数 名  :mass_function_cleanup_alllun_info
 功能描述  :清除lun info
 返 回 值  :USB_INT:
 修改历史      :
  1.日    期   : 2014年6月10日
    修改内容   : 添加注释
*****************************************************************************/
USB_VOID mass_function_cleanup_alllun_info(USB_VOID);

#endif
