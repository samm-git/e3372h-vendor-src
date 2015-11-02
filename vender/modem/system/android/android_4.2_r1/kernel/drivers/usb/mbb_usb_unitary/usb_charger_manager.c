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
/******************************问题单修改记录**********************************
     问题单号           时间            修改人                     说明
******************************************************************************/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/utsname.h>
#include <linux/platform_device.h>

#include <linux/usb/ch9.h>
#include <linux/usb/composite.h>
#include <linux/usb/gadget.h>

#include "usb_charger_manager.h"
#include <product_config.h>
#include "gadget_chips.h"
#include <linux/netlink.h>
#include <linux/notifier.h>
#include "usb_notify.h"
#include "usb_debug.h"
/*对外接口*/
#include <linux/usb/usb_interface_external.h>

/*依赖外部接口*/
#include "mbb_usb_adp.h"

#if (MBB_CHG_WIRELESS == FEATURE_ON)
#include <bsp_hkadc.h>
#include <bsp_sram.h>
#define VBUS_JUDGEMENT_THRESHOLD 700 //无线充电adc判断门限值，
#endif /* MBB_CHG_WIRELESS */
enum usb_chg_action
{
    CHG_PULL_OUT,
    CHG_PLUG_IN,
};

static usb_adp_ctx_t* usb_adp_cb = NULL;
static mbb_charger_st  g_mbb_charger =
{
    .current_chg_type = CHG_CHGR_INVALID,
    .usb_chg_action_flag = CHG_PULL_OUT,
    .usb_chg_work_mode = USB_CHG_NON,
};

/* 设置定时器，在插入PC和非标充电器以后，设置10s超时，检查是否枚举成功 */
static struct timer_list chg_nonstd_det_timer ;

static USB_INT first_report_chg_type = 1;

#if ( FEATURE_ON == MBB_CHG_PLATFORM_9X25 )
extern  struct wake_lock g_charger_wake_lock;
#endif
/*****************************************************************
Parameters    :  chg_mode 当前充电模式
Return        :    无
Description   :  将充电模式设置为当前模式，以互斥其他模式
*****************************************************************/
USB_VOID usb_chg_set_work_mode(USB_INT chg_mode)
{
    g_mbb_charger.usb_chg_work_mode = chg_mode;
    DBG_I(MBB_CHARGER, "set charger_work_mode = %d \n",
          g_mbb_charger.usb_chg_work_mode);
}
/*****************************************************************
Parameters    :  chg_mode 当前充电模式
Return        :    1：可进入当前充电模式； 0：不可执行当前模式
Description   :  判断本充电模式是否可执行
*****************************************************************/
USB_BOOL usb_chg_check_current_mode(USB_INT chg_mode)
{
    if ((USB_CHG_NON == g_mbb_charger.usb_chg_work_mode)  ||
        chg_mode == g_mbb_charger.usb_chg_work_mode)
    {
        return   1;
    }
    else
    {
        return  0;
    }
}
/*****************************************************************
Parameters    :  chg_type 充电类型
Return        :    无
Description   :  将充电类型转化为字符
*****************************************************************/
static const char* chg_type_to_string(chg_chgr_type_t chg_type)
{
    switch (chg_type)
    {
        case CHG_CHGR_UNKNOWN:
            return "CHG_CHGR_UNKNOWN";
        case CHG_WALL_CHGR:
            return "CHG_WALL_CHGR";
        case CHG_USB_HOST_PC:
            return "CHG_USB_HOST_PC";
        case CHG_NONSTD_CHGR:
            return "CHG_NONSTD_CHGR";
        case CHG_WIRELESS_CHGR:
            return "CHG_WIRELESS_CHGR";
        case CHG_EXGCHG_CHGR:
            return "CHG_EXGCHG_CHGR";
        case CHG_CHGR_INVALID:
            return "CHG_CHGR_INVALID";
        default:
            return "CHG_CHGR_INVALID";
    }
}

/*****************************************************************
Parameters    :
Return        :
Description   :
*****************************************************************/
/* 形参中要加入unsigned long data，否则会出现警告1 */
static USB_VOID detect_chg_carkit_timer_handler( USB_ULONG data )
{
    mbb_charger_st* mbb_chg = ( mbb_charger_st*)data;

    if (CHG_CHGR_UNKNOWN == g_mbb_charger.current_chg_type)
    {
        g_mbb_charger.current_chg_type = CHG_NONSTD_CHGR;
        g_mbb_charger.usb_chg_action_flag = CHG_PLUG_IN;
        queue_delayed_work(g_mbb_charger.usb_chg_notify_wq,
                           &mbb_chg->usb_chg_notify_work, 0);
        usb_notify_syswatch(DEVICE_ID_CHARGER, GPIO_CHARGER_ATTATCH);
    }
}

/*****************************************************************
Parameters    :
Return        :
Description   :  个充电模块上报充电事件
*****************************************************************/
static USB_VOID usb_chg_det_notify_work( struct work_struct* w )
{
    mbb_charger_st* mbb_chg = container_of(w, mbb_charger_st, usb_chg_notify_work.work);

    DBG_I(MBB_CHARGER, "%s:current_chg_type:%s\n", __func__,
          chg_type_to_string(g_mbb_charger.current_chg_type));

    if (first_report_chg_type)
    {
        usb_adp_cb->stm_set_chg_type_cb(g_mbb_charger.current_chg_type);
        first_report_chg_type = 0;
    }
    usb_adp_cb->battery_notifier_call_chain_cb(g_mbb_charger.usb_chg_action_flag,
            g_mbb_charger.current_chg_type);

}

#define DELTA_TIME_AND_REMOVE_INSERT 200
USB_ULONG wall_chg_insert_time = 0;
USB_ULONG wall_chg_remove_time = 0;
USB_INT dp_dm_connected = 0;

/*****************************************************************
Parameters    :  nb:    val :USB设备连接状态   data:标充、非标充类型值
Return        :    0
Description   :  充电类型检测
*****************************************************************/
USB_INT usb_charger_type_detect_event(struct notifier_block* nb, USB_ULONG val, USB_PVOID data)
{
    USB_INT ret = 0;
    USB_INT charger_type = *( (USB_INT*)data);
    USB_ULONG delta_wall_chg_time = 0;

    DBG_E(MBB_CHARGER, "%s -val: %ld ,charger_type:%d\n",
          __func__, val, charger_type);
    /*判断当前充电模式是否互斥*/
    if (!usb_chg_check_current_mode(USB_CHG_LINE))
    {
        DBG_E(MBB_CHARGER, "charge mode is mutually exclusive\n");
        return ret;
    }


    if ( MBB_USB_DEVICE_IDEN == val)
    {
        /*
        * note: the function will sleep in long time !!!
        * 0:INVALID , 1:huawei charger, 2:NOT_HUAWEI  3:PC_charger
        */
        g_mbb_charger.usb_chg_action_flag = CHG_PLUG_IN;
        switch (charger_type)
        {
            case MBB_USB_CHARGER_TYPE_HUAWEI:
#if (FEATURE_ON == MBB_CHG_PLATFORM_9X25  )
                /*充电器插入加锁不让单板休眠*/
                wake_lock(&g_charger_wake_lock);
                DBG_I(MBB_CHARGER, " charger plug in locked!\n");
#endif
                delta_wall_chg_time = wall_chg_insert_time - wall_chg_remove_time;
                DBG_I(MBB_CHARGER, "delta_wall_chg_time 0x%lx!\nwall_chg_insert_time 0x%lx!\nwall_chg_remove_time 0x%lx!\n",
                      delta_wall_chg_time,
                      wall_chg_insert_time,
                      wall_chg_remove_time);
                if ((DELTA_TIME_AND_REMOVE_INSERT > delta_wall_chg_time)
                    && (1 == dp_dm_connected))
                {
                    DBG_I(MBB_CHARGER, "CHG_500MA_WALL_CHGR detected!\n");
                    g_mbb_charger.current_chg_type = CHG_500MA_WALL_CHGR;
                }
                else
                {
                    DBG_I(MBB_CHARGER, "CHG_WALL_CHGR detected!\n");
                    g_mbb_charger.current_chg_type = CHG_WALL_CHGR;
                }

                queue_delayed_work(g_mbb_charger.usb_chg_notify_wq,
                                   &g_mbb_charger.usb_chg_notify_work, 0);

                break;
            case MBB_USB_CHARGER_TYPE_NOT_HUAWEI:
                g_mbb_charger.current_chg_type = CHG_CHGR_UNKNOWN;
                /* 启动 10s 非标充电器类型检测 */
                mod_timer(&chg_nonstd_det_timer, jiffies
                          + msecs_to_jiffies(CHG_DETECT_OVERTIME));

                break;
            case  MBB_USB_CHARGER_TYPE_PC:
                g_mbb_charger.current_chg_type = CHG_USB_HOST_PC;
                DBG_E(MBB_CHARGER, "current_chg_type %d",
                      g_mbb_charger.current_chg_type);
                queue_delayed_work(g_mbb_charger.usb_chg_notify_wq,
                                   &g_mbb_charger.usb_chg_notify_work, 0);

                break;
            case MBB_USB_CHARGER_TYPE_INVALID:
                g_mbb_charger.current_chg_type = CHG_CHGR_INVALID;
                DBG_E(MBB_CHARGER, "invalid charger %s!\n", __func__);
                queue_delayed_work(g_mbb_charger.usb_chg_notify_wq,
                                   &g_mbb_charger.usb_chg_notify_work, 0);
                break;
            default:
                queue_delayed_work(g_mbb_charger.usb_chg_notify_wq,
                                   &g_mbb_charger.usb_chg_notify_work, 0);

                break;
        }
    }
    else if (MBB_USB_DEVICE_INSERT == val)
    {
        usb_chg_set_work_mode(USB_CHG_LINE);
        g_mbb_charger.current_chg_type = CHG_CHGR_UNKNOWN;
        g_mbb_charger.usb_chg_action_flag = CHG_PLUG_IN;
        usb_notify_syswatch(DEVICE_ID_CHARGER, GPIO_CHARGER_ATTATCH);
        queue_delayed_work(g_mbb_charger.usb_chg_notify_wq,
                           &g_mbb_charger.usb_chg_notify_work, 0);
    }
    else if ( MBB_USB_DEVICE_REMOVE == val)
    {
        g_mbb_charger.usb_chg_action_flag = CHG_PULL_OUT;

        DBG_E(MBB_CHARGER, "CHARGER pull out!\n");
        usb_notify_syswatch(DEVICE_ID_CHARGER, GPIO_CHARGER_REMOVE);
        queue_delayed_work(g_mbb_charger.usb_chg_notify_wq,
                           &g_mbb_charger.usb_chg_notify_work, 0);
#if (FEATURE_ON == MBB_CHG_PLATFORM_9X25  )
        /*充电器移除释放锁*/
        if ((CHG_WALL_CHGR == g_mbb_charger.current_chg_type) ||
            (CHG_500MA_WALL_CHGR == g_mbb_charger.current_chg_type))
        {
            wake_unlock(&g_charger_wake_lock);
            DBG_E(MBB_CHARGER, "charger plug out unlocked!\n");
        }
#endif
        g_mbb_charger.current_chg_type = CHG_CHGR_INVALID;
        usb_chg_set_work_mode(USB_CHG_NON);
    }
    return ret;
}

static struct notifier_block usb_charger_type_detect_block =
{
    .notifier_call = usb_charger_type_detect_event
};

#if (MBB_CHG_WIRELESS == FEATURE_ON)
/*****************************************************************
Parameters    :  无
Return        :    1: 有无线充电 0: 没有无线充电事件
Description   :  无线充电检测及消息上报
*****************************************************************/
USB_BOOL usb_chg_wireless_detect(USB_VOID)
{
    USB_INT16 adc_voltage = 0;
    USB_INT ret = 0;

    /*判断当前充电模式是否互斥*/
    if (!usb_chg_check_current_mode(USB_CHG_WIRELESS))
    {
        DBG_E(MBB_CHARGER, "%s:charge mode is mutually exclusive\n", __func__);
        return  0;
    }

#if (FEATURE_ON == MBB_DLOAD)
    huawei_smem_info* smem_data = NULL;
    smem_data = (huawei_smem_info*)SRAM_DLOAD_ADDR;

    /*该问题咱这样处理，升级时不判断无线充电*/
    adc_voltage = VBUS_JUDGEMENT_THRESHOLD;
    if (SMEM_DLOAD_FLAG_NUM != smem_data->smem_dload_flag)
    {
        ret = bsp_hkadc_convert(HKADC_CHANNEL_8, &adc_voltage);
    }
    else
    {
        return 0;
    }
#else
    ret = bsp_hkadc_convert(HKADC_CHANNEL_8, &adc_voltage);
#endif /*MBB_DLOAD*/
    DBG_I(MBB_CHARGER, "bsp_hkadc_convert ret = %d, bsp_hkadc_convert = %d.\n",
          ret, adc_voltage);
    if ((0 == ret) && (adc_voltage < VBUS_JUDGEMENT_THRESHOLD))
    {
        DBG_E(MBB_CHARGER, "CHG_WIRELESS_CHGR detected!\n");
        usb_chg_set_work_mode(USB_CHG_WIRELESS);
        g_mbb_charger.current_chg_type = CHG_WIRELESS_CHGR;
        usb_adp_cb->stm_set_chg_type_cb(g_mbb_charger.current_chg_type);
        usb_adp_cb->battery_notifier_call_chain_cb(1, g_mbb_charger.current_chg_type);
        return 1;
    }
    else
    {
        return 0;
    }
}

USB_BOOL usb_chg_wireless_remove(USB_VOID)
{
    /*判断当前充电模式是否互斥*/
    if (!usb_chg_check_current_mode(USB_CHG_WIRELESS))
    {
        DBG_E(MBB_CHARGER, "%s:charge mode is mutually exclusive\n", __func__);
        return  0;
    }

    if ( CHG_WIRELESS_CHGR == g_mbb_charger.current_chg_type)
    {
        DBG_E(MBB_CHARGER, "CHG_WIRELESS_CHGR pull out!\n");
        usb_adp_cb->battery_notifier_call_chain_cb(0,
                g_mbb_charger.current_chg_type);
        g_mbb_charger.current_chg_type = CHG_CHGR_INVALID;
        usb_chg_set_work_mode(USB_CHG_NON);
        return 1;
    }
    else
    {
        return 0;
    }
}
#endif /* MBB_CHG_WIRELESS */


/*****************************************************************
Parameters    :  无
Return        :    无
Description   :  充电模块初始化
*****************************************************************/
USB_VOID usb_charger_init(USB_VOID)
{
    usb_adp_cb = usb_get_adp_ctx();/*获取依赖的外部接口*/
    g_mbb_charger.usb_chg_notify_wq = create_singlethread_workqueue("usb_chg_notify");
    INIT_DELAYED_WORK(&g_mbb_charger.usb_chg_notify_work,
                      usb_chg_det_notify_work);
    setup_timer(&chg_nonstd_det_timer, detect_chg_carkit_timer_handler,
                (USB_ULONG)&g_mbb_charger);
    usb_adp_cb->usb_register_notify_cb(&usb_charger_type_detect_block);
}

/*****************************************************************
Parameters    :  无
Return        :    无
Description   :  充电模块退出
*****************************************************************/
USB_VOID usb_charger_exit(USB_VOID)
{
    cancel_delayed_work_sync(&g_mbb_charger.usb_chg_notify_work);
    del_timer(&chg_nonstd_det_timer); /* delete timer */
    usb_adp_cb->usb_unregister_notify_cb(&usb_charger_type_detect_block);
}

