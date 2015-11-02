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
/**************************************************************************************************
    问题单号          时间           修改人                    说明
***************************************************************************************************/
#include "usb_debug.h"
#include "usb_platform_comm.h"

debug_mode_info debug_bank[MBB_LAST] =
{
    {U_ALL, "U_ALL:"},
    {U_ALL, ""},
    {U_ERROR, "U_ACM:"},
    {U_ERROR, "U_NET:"},
    {U_ERROR, "U_RNDIS:"},
    {U_ERROR, "U_ECM:"},
    {U_ERROR, "U_NCM:"},
    {U_ERROR, "U_CHARGER:"},
    {U_ERROR, "U_NV:"},
    {U_ERROR, "U_PNP:"},
    {U_ERROR, "U_EVENT:"},
    {U_ERROR, "U_MASS:"},
    {U_ERROR, "U_OTG_CHARGER:"},
    {U_ERROR, "U_HOTPLUG:"},

};

USB_CHAR* lev_name[U_ALL + 1] =
{
    "U_TRACE",
    "U_ERROR",
    "U_WARN",
    "U_INFO",
    "U_ALL"
};

/*****************************************************************
Parameters    :  USB_VOID
Return        :
Description   :  debug初始化
*****************************************************************/
USB_VOID usb_debug_init(USB_VOID)
{
    USB_INT i = 0;
    DBG_SET_GLOBAL_LEVEL(U_ALL);
    {
        DBG_SET_LEVEL(MBB_ALL, U_ALL);
        for ( i = 2; i < MBB_LAST; i++)
        {
            DBG_SET_LEVEL(i, U_ERROR);
        }
    }
}

/*****************************************************************
Parameters    :  type
Return        :
Description   :  类型检测
*****************************************************************/
USB_VOID valid_type(enum DEBUG_MODULES type)
{
    UNUSED_VAR(type);
}

/*****************************************************************
Parameters    :  module
              lev
Return        :
Description   :  打印所有USB的log，使用此log调整函数后，会出现海量
                 的log请谨慎使用
*****************************************************************/
USB_VOID usb_all_module_logs(USB_INT all_or_none)
{
    USB_INT i = 0;
    if (all_or_none)
    {
        DBG_T(MBB_DEBUG, "|-------------------------------------------------------|\n");
        DBG_T(MBB_DEBUG, "|          YOU WILL OUTPUT ALL USB DEBUG INFO!!!        |\n");
        DBG_T(MBB_DEBUG, "|-------------------------------------------------------|\n");
        for ( i = 0; i < MBB_LAST; i++)
        {
            DBG_SET_LEVEL(i, U_ALL);
        }
    }
    else
    {
        DBG_T(MBB_DEBUG, "|-------------------------------------------------------|\n");
        DBG_T(MBB_DEBUG, "|            YOU CLOSED  ALL USB DEBUG INFO!!!          |\n");
        DBG_T(MBB_DEBUG, "|-------------------------------------------------------|\n");
        for ( i = 0; i < MBB_LAST; i++)
        {
            DBG_SET_LEVEL(i, U_TRACE);
        }
    }
}

/*****************************************************************
Parameters    :  module
              lev
Return        :
Description   :  模块级别设置
*****************************************************************/
USB_VOID usb_debug_lev_set(USB_INT module, USB_INT lev)
{
    DBG_SET_LEVEL(module, lev);
    DBG_T(MBB_DEBUG, "|-------------------------------------------------------|\n");
    DBG_T(module,    "|        module name %s-- %d set lev to %s-- %d         |\n",
          debug_bank[module].module_name, module, lev_name[lev], lev);
    DBG_T(MBB_DEBUG, "|-------------------------------------------------------|\n");
}

USB_VOID usb_debug_show_module_lev(USB_VOID)
{
    USB_INT i = 0;
    for ( i = 2; i < MBB_LAST; i++)
    {
        DBG_T(MBB_DEBUG, "|%15.15s(%-2d)---- lev: %-10.10s |\n",
              debug_bank[i].module_name, i, lev_name[debug_bank[i].lev]);
    }
}
/*****************************************************************
Parameters    :   None
Return        :
Description   :  模块级别设置帮助
*****************************************************************/
USB_VOID usb_debug_help(USB_VOID)
{
    DBG_T(MBB_DEBUG, "|-------------------------------------------------------|\n");
    DBG_T(MBB_DEBUG, "|you can change the lev of the module to output it's log|\n");
    DBG_T(MBB_DEBUG, "|-------------------------------------------------------|\n");
    DBG_T(MBB_DEBUG, "|      use: dbg_lev_set(USB_INT module, USB_INT lev)    |\n");
    DBG_T(MBB_DEBUG, "|-------------------------------------------------------|\n");
    DBG_T(MBB_DEBUG, "|if you want to output all of the logs in module MASS   |\n");
    DBG_T(MBB_DEBUG, "|-------------------------------------------------------|\n");
    DBG_T(MBB_DEBUG, "|      you can input : ---- ecall dbg_lev_set 10 4      |\n");
    DBG_T(MBB_DEBUG, "|-------------------------------------------------------|\n");
    DBG_T(MBB_DEBUG, "|if you want to output all of the logs about usb        |\n");
    DBG_T(MBB_DEBUG, "|-------------------------------------------------------|\n");
    DBG_T(MBB_DEBUG, "|     you can input : ---- ecall usb_all_module_logs 1  |\n");
    DBG_T(MBB_DEBUG, "|-------------------------------------------------------|\n");
    DBG_T(MBB_DEBUG, "|if you use this, there will be a lage of logs to output|\n");
    DBG_T(MBB_DEBUG, "|-------------------------------------------------------|\n");
    DBG_T(MBB_DEBUG, "|---------------------|\n");
    DBG_T(MBB_DEBUG, "|the module is:       |\n");
    DBG_T(MBB_DEBUG, "|MBB_DEBUG:-------%-4d|\n", (USB_INT)MBB_DEBUG);
    DBG_T(MBB_DEBUG, "|MBB_ACM:---------%-4d|\n", (USB_INT)MBB_ACM);
    DBG_T(MBB_DEBUG, "|MBB_NET:---------%-4d|\n", (USB_INT)MBB_NET);
    DBG_T(MBB_DEBUG, "|MBB_RNDIS:-------%-4d|\n", (USB_INT)MBB_RNDIS);
    DBG_T(MBB_DEBUG, "|MBB_ECM:---------%-4d|\n", (USB_INT)MBB_ECM);
    DBG_T(MBB_DEBUG, "|MBB_NCM:---------%-4d|\n", (USB_INT)MBB_NCM);
    DBG_T(MBB_DEBUG, "|MBB_CHARGER:-----%-4d|\n", (USB_INT)MBB_CHARGER);
    DBG_T(MBB_DEBUG, "|MBB_USB_NV:------%-4d|\n", (USB_INT)MBB_USB_NV);
    DBG_T(MBB_DEBUG, "|MBB_PNP:---------%-4d|\n", (USB_INT)MBB_PNP);
    DBG_T(MBB_DEBUG, "|MBB_EVENT:-------%-4d|\n", (USB_INT)MBB_EVENT);
    DBG_T(MBB_DEBUG, "|MBB_MASS:--------%-4d|\n", (USB_INT)MBB_MASS);
    DBG_T(MBB_DEBUG, "|MBB_OTG_CHARGER:-%-4d|\n", (USB_INT)MBB_OTG_CHARGER);
    DBG_T(MBB_DEBUG, "|---------------------|\n");
    DBG_T(MBB_DEBUG, "|the lev is:          |\n");
    DBG_T(MBB_DEBUG, "|U_TRACE:---------%-4d|\n", (USB_INT)U_TRACE);
    DBG_T(MBB_DEBUG, "|U_ERROR:---------%-4d|\n", (USB_INT)U_ERROR);
    DBG_T(MBB_DEBUG, "|U_WARN:----------%-4d|\n", (USB_INT)U_WARN);
    DBG_T(MBB_DEBUG, "|U_INFO:----------%-4d|\n", (USB_INT)U_INFO);
    DBG_T(MBB_DEBUG, "|---------------------|\n");
}

/*****************************************************************
Parameters    :   None
Return        :
Description   :  LOG测试
*****************************************************************/
USB_VOID usb_debug_test(USB_VOID)
{
    DBG_T(MBB_ACM, "MBB_ACM-----ERR");
    DBG_W(MBB_ACM, "MBB_ACM-----WARING");
    DBG_I(MBB_ACM, "MBB_ACM-----INFO");
    DBG_T(MBB_NET, "MBB_NET-----ERR");
    DBG_W(MBB_NET, "MBB_NET-----WARING");
    DBG_I(MBB_NET, "MBB_NET-----INFO");
    DBG_I(MBB_DEBUG, "U_ALL:%d\n", (USB_INT)U_ALL);
}

