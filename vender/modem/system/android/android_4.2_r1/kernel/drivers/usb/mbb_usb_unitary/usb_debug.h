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
#ifndef __USB_DEBUG_H__
#define __USB_DEBUG_H__

#include "usb_config.h"
#include <linux/types.h>
#include <linux/kernel.h>
#include "usb_platform_comm.h"
#include <linux/mlog_lib.h>

/*codecheck bug*/
#define UNUSED_VAR(x) (x) = (x)

enum DEBUG_LEVELS
{
    U_TRACE = 0,
    U_ERROR, //错误级别
    U_WARN, //警告级别
    U_INFO,  //描述信息级别
    U_ALL,   //所有级别
};

enum DEBUG_MODULES
{
    MBB_ALL       =       0,
    MBB_DEBUG,
    MBB_ACM,
    MBB_NET,
    MBB_RNDIS,
    MBB_ECM,
    MBB_NCM,
    MBB_CHARGER,
    MBB_USB_NV,
    MBB_PNP,
    MBB_EVENT,
    MBB_MASS,
    MBB_OTG_CHARGER,
    MBB_HOTPLUG,
    MBB_LAST,
} ;

typedef struct DEBUG_MODE_INFO
{
    enum DEBUG_LEVELS lev;
    const USB_CHAR* module_name;
} debug_mode_info;

#define mbbprintk(module_name, fmt, lev_name,args...) \
    printk( KERN_ERR "%s(%s)%s():" fmt , module_name, lev_name, __func__, ## args);

#define mbbdumpprintk(fmt, args...) \
    printk( KERN_ERR ""fmt , ## args);

/*
 *定义log打印函数
 */

/*打印调试使用的log，不代表意义，使用此log打印函数打印的log，始终会被打印*/
#define DBG_T(type, fmt, args...) PRINT_DBG((type), U_TRACE,fmt, ## args)
/*打印err级别的log，每个模块默认的log级别*/
#define DBG_E(type, fmt, args...) PRINT_DBG((type), U_ERROR,fmt, ## args)
/*打印warning级别的log*/
#define DBG_W(type, fmt, args...) PRINT_DBG((type), U_WARN,fmt, ## args)
/*打印info级别的log*/
#define DBG_I(type, fmt, args...) PRINT_DBG((type), U_INFO,fmt, ## args)

#ifdef USB_DEBUG
#define PRINT_DBG(type, level, fmt, args...) \
    /*lint -e40 -e58 */ \
    do { \
        if ((MBB_DEBUG) == (type))\
        {\
            mbbdumpprintk(fmt , ## args);\
        }\
        else \
        { \
            mbbprintk(debug_bank[(int)(type)].module_name, fmt ,lev_name[level] ,## args);\
        } \
    } while (0)
#else
#define PRINT_DBG(type, level, fmt, args...) \
    /*lint -e40 -e58 */ \
    do { \
        if ((MBB_DEBUG) == (type))\
        {\
            mbbdumpprintk(fmt , ## args);\
        }\
        else if ((level) <= debug_bank[(int)(type)].lev)\
        { \
            mbbprintk(debug_bank[(int)(type)].module_name, fmt ,lev_name[level], ## args);\
        } \
    } while (0)
/*lint +e40 +e58 */
#endif
void valid_type(enum DEBUG_MODULES type);
#define DBG_SET_LEVEL(type, level) \
    do { \
        valid_type(type); \
        debug_bank[(int)(type)].lev = (level); \
    } while (0)

#define DBG_SET_GLOBAL_LEVEL(level) \
    do { \
        int i; \
        for (i = 0; i < MBB_LAST; i++) \
        {\
            debug_bank[i].lev = (level); \
        }\
    } while(0)

#define DBG_GET_LEVEL(type) debug_bank[(USB_INT)(type)].lev

extern debug_mode_info debug_bank[MBB_LAST];

extern USB_CHAR* lev_name[U_ALL + 1];

/*==============================================================================
 *===============================API声明=================================
 *============================================================================*/
/****************************************************************
 函 数 名  : debug_init
 功能描述  : debug初始化。
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NA
*****************************************************************/
USB_VOID usb_debug_init(USB_VOID);
/****************************************************************
 函 数 名  : debug_init
 功能描述  : debug初始化。
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NA
*****************************************************************/
USB_VOID dbg_lev_set(USB_INT module, USB_INT lev);
/****************************************************************
 函 数 名  : debug_init
 功能描述  : debug初始化。
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NA
*****************************************************************/
USB_VOID usb_all_module_logs(USB_INT all_or_none);

USB_VOID usb_debug_show_module_lev(USB_VOID);

#endif
