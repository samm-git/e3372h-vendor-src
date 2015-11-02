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
问题单号                时间            修改人              说明
*****************************************************************************************************/

#include <linux/usb/gadget.h>
#include <drv_fastOnOff.h>
#include <linux/notifier.h>
#include <linux/usb/composite.h>
#include <mbb_config.h>
#include <linux/usb/usb_interface_external.h>
#include "usb_debug.h"
#include "usb_platform_comm.h"
#include "usb_notify.h"
#include "hw_pnp.h"
#include "mbb_fast_on_off.h"

static USB_INT fast_off_status = false;

extern USB_VOID mass_storage_open(USB_VOID);
extern USB_VOID mass_storage_close(USB_VOID);

/*****************************************************************
Parameters    : fastMode :假关机模式
Return        :    无
Description   :  假关机模式设置
*****************************************************************/
USB_INT usb_fast_on_off_mode_set( FASTONOFF_MODE fastMode)
{
    DBG_T(MBB_CHARGER, "%s Entery mode %d!\n", __func__, fastMode);
    switch (fastMode)
    {
            /*退出假关机状态*/
        case FASTONOFF_MODE_CLR:
            fast_off_status = false;
            DBG_T(MBB_CHARGER, "EXIT fast off status:fastMode:%d\n", fastMode);
#ifdef USB_CHARGE_EXT
            usb_notify_event(USB_OTG_ENABLE_ID_IRQ, NULL);
#endif
            break;
            /*进入假关机状态*/
        case FASTONOFF_MODE_SET:
            fast_off_status = true;
            DBG_T(MBB_CHARGER, "enter fast off status:fastMode:%d\n", fastMode);
#ifdef USB_CHARGE_EXT
            usb_notify_event(USB_OTG_DISABLE_ID_IRQ, NULL);
#endif
            break;
        default:
            DBG_E(MBB_CHARGER, "defalut fast power off :fastMode:%d\n", fastMode);
            break;
    }

    pnp_switch_autorun_port();

    return 0;
}

/*****************************************************************
Parameters    :  无
Return        :  1:假关机  0: 非假关机
Description   :  获取假关机状态
*****************************************************************/
USB_INT usb_fast_on_off_stat(USB_VOID)
{
    return fast_off_status ;
}

/*****************************************************************
Parameters    :
Return        :
Description   :
*****************************************************************/
static USB_INT usb_monitor_charger_event(struct notifier_block* nb,
        USB_ULONG val, USB_PVOID data)
{
    USB_INT ret = 0;

    DBG_I(MBB_CHARGER, "%s +++usb_monitor_charger_event+++ %ld \n", __func__, val);
    ret = usb_fast_on_off_mode_set(val);
    return ret;
}

static struct notifier_block usb_monitor_charger_block =
{
    .notifier_call = usb_monitor_charger_event
};

/*****************************************************************
Parameters    :  无
Return        :    无
Description   :  USB  归一化假关机模块初始化
*****************************************************************/
USB_VOID fast_on_off_init(USB_VOID)
{
    blocking_notifier_chain_register(&g_fast_on_off_notifier_list,
                                     &usb_monitor_charger_block);

}

/*****************************************************************
Parameters    :  无
Return        :    无
Description   :  USB  归一化假关机模块退出
*****************************************************************/
USB_VOID fast_on_off_exit(USB_VOID)
{
    blocking_notifier_chain_unregister(&g_fast_on_off_notifier_list,
                                       &usb_monitor_charger_block);
}


