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
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/module.h>
#include <linux/export.h>
#include <linux/delay.h>
#include <linux/fcntl.h>
#include <linux/namei.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/syscalls.h>
#include <linux/wakelock.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <asm/mach/irq.h>
#include <linux/wait.h>

#include "usb_config.h"
#include "usb_debug.h"
#ifdef MBB_USB_UNITARY_Q
#include "hw_pnp.h"
#include "hw_pnp_api.h"
#else
#include "usb_vendor.h"
#include "mbb_config.h"
#include "product_config.h"
#endif
#include "usb_charger_manager.h"
#include <linux/mlog_lib.h>


#include "usb_hotplug.h"

#define USB_DETECT_DELAY        200/*USB 中断防抖中断检测延时*/

extern USB_INT recv_vbus_on_intr;
/*拔出解锁控制，拔出时判断是否插入状态，选择是否解锁*/
static struct timer_list g_wake_unlock_timer ;

/*wake unlock锁定时器，默认3s*/
static USB_INT g_wake_unlockt_loop = 3000;

/*
 * usb adapter for charger
 */
typedef struct usb_hotplug_datamodel
{
    USB_INT charger_type;                                          /*USB充电类型*/
    USB_INT usb_status;                                              /*USB当前状态*/
    USB_INT usb_old_status;                                       /*USB上次状态*/
    int usb_host_status;
    int usb_host_old_status;
    USB_INT usb_hotplub_state;                                  /*记录USB插拔状态只有插拔动作才会更新此状态*/
    USB_UINT stat_usb_insert;                            /*防抖前插入记数*/
    USB_UINT stat_usb_insert_proc;                   /*防抖后 处理前插入记数*/
    USB_UINT stat_usb_insert_proc_end;          /*防抖后 处完成后插入记数*/
    USB_ULONG stat_usb_insert_timestamp;                   /*插入时间戳*/
    USB_UINT stat_usb_enum_done;                  /*防抖前枚举记数*/
    USB_UINT stat_usb_enum_done_proc;         /*防抖后处理前枚举记数*/
    USB_UINT stat_usb_enum_done_proc_end; /*防抖后处理后枚举记数*/
    USB_UINT stat_usb_remove;                         /*防抖前拔出记数*/
    USB_UINT stat_usb_remove_proc;               /*防抖后处理前拔出记数*/
    USB_UINT stat_usb_remove_proc_end;        /*防抖后处理后拔出记数*/
    USB_ULONG stat_usb_remove_timestamp;      /*拔出时间戳*/
    USB_UINT stat_usb_disable;
    USB_UINT stat_usb_disable_proc;
    USB_UINT stat_usb_disable_proc_end;
    USB_UINT stat_usb_no_need_notify;             /*抖动计数*/
    USB_UINT stat_usb_perip_insert;                   /*HOST防抖前插入记数*/
    USB_UINT stat_usb_perip_insert_proc;           /*HOST防抖前处理前插入记数*/
    USB_UINT stat_usb_perip_insert_proc_end;  /*HOST防抖前处理后插入记数*/
    USB_UINT stat_usb_perip_remove;                 /*HOST防抖前拔出记数*/
    USB_UINT stat_usb_perip_remove_proc;        /*HOST防抖后处理前拔出记数*/
    USB_UINT stat_usb_perip_remove_proc_end; /*HOST防抖后处理后拔出记数*/
    USB_UINT stat_usb_poweroff_fail;                   /*上电失败计数*/
    USB_UINT stat_usb_poweron_fail;                   /*下电失败计数*/
    USB_UINT delta_time;
    //unsigned stat_wait_cdev_created;
    struct workqueue_struct* usb_notify_wq;       /*USB work queue*/
    struct delayed_work usb_notify_wk;               /*USB work */
    struct delayed_work usb_notify_host_wk;      /*USB work for 作为主机时插拔U盘处理 */
    void* private;                                                    /*USB私有数据指针*/
    struct mutex mutex;
} usb_hotplug_datamodel_t;

BLOCKING_NOTIFIER_HEAD(usb_notifier_list);/*lint !e34 !e110 !e156 !e651 !e43*/
static usb_hotplug_datamodel_t  g_usb_notifier_h;
static struct wake_lock g_dwc_wakelock;


/**注册外部接口函数**/
#ifdef MBB_USB_UNITARY_Q
USB_INT hw_usb_entry(USB_VOID)
{
    pnp_probe();
    return 0;
}
USB_VOID hw_usb_exit(USB_VOID)
{
    pnp_remove();
}

static usb_hotplug_hanlder_t usb_hotplug_ctx =
{
    .usb_poweroff_cb =  0x00,
    .usb_poweron_cb = 0x00,
    .usb_entry_cb =       hw_usb_entry,
    .usb_exit_cb =          hw_usb_exit,
    .usbid_proc_cb = 0x00,
    .usb_wait_enumdone_cb = 0x00,
    .usb_check_wireless_chg_cb = 0x00,
    .usb_wireless_chg_remove_cb = 0x00,
    .usb_notify_wq_extern = 0x00,/*在init函数赋值不，在此初始化不符合C标准，编译错误*/
};

#else
static usb_hotplug_hanlder_t usb_hotplug_ctx =
{
    .usb_poweron_cb =                          power_on_dwc3_usb,
    .usb_poweroff_cb =                          power_off_dwc3_usb,
    .usb_entry_cb =                                usb_balong_init,
    .usb_exit_cb =                                   usb_balong_exit,
    .usbid_proc_cb =                               bsp_usb_usbid_proc,
    .usb_wait_enumdone_cb =               bsp_usb_wait_cdev_created,
    .usb_clear_enumdone_cb  =              bsp_usb_clear_last_cdev_name,
    .usb_check_wireless_chg_cb =          usb_chg_wireless_detect,
    .usb_wireless_chg_remove_cb =       usb_chg_wireless_remove,
    .usb_notify_wq_extern = 0x00,
};
#endif

/*****************************************************************
Parameters    :  USB_INT loop 定时器时间
Return        :    无
Description   :  设置wake unlock的定时器时间
*****************************************************************/
USB_VOID usb_set_wake_unlockt_loop( USB_INT loop)
{
    DBG_T(MBB_HOTPLUG, "set wake_unlockt_loop to : %d\n", loop);
    g_wake_unlockt_loop = loop;
}

/*****************************************************************
Parameters    :  USB_INT action 插拔动作
Return        :    无
Description   :  通知USB插拔事件
*****************************************************************/
void usb_broadcast_event(USB_INT action)
{
    blocking_notifier_call_chain(&usb_notifier_list, action, (void*)&g_usb_notifier_h.charger_type);
}

/*****************************************************************
Parameters    :
Return        :    无
Description   :  停止USB的工作队列
*****************************************************************/
void usb_stop_work(struct delayed_work* wk )
{
    /*cancel_work_sync会取消相应的work，但是如果这个
    work已经在运行那么cancel_work_sync会阻塞，直到
    work完成并取消相应的work  */
    if (NULL != wk && delayed_work_pending(wk))
    {
        cancel_delayed_work(wk);
    }

}
/*****************************************************************
Parameters    :
Return        :    无
Description   :  启动USB的工作队列
*****************************************************************/
void  usb_start_work(struct delayed_work* wk, USB_INT timeout)
{
    if (NULL != usb_hotplug_ctx.usb_notify_wq_extern)
    {
        if (NULL  != wk)
        {
            queue_delayed_work(usb_hotplug_ctx.usb_notify_wq_extern, wk, msecs_to_jiffies(timeout));
        }

    }
    else
    {
        if (NULL  != wk)
        {
            queue_delayed_work(g_usb_notifier_h.usb_notify_wq, wk, msecs_to_jiffies(timeout));
        }
    }
}
/*****************************************************************
Parameters    :
Return        :    无
Description   :  USB插拔入口
*****************************************************************/
void bsp_usb_status_change(USB_INT status)
{
    USB_ULONG timeout = 0;

    DBG_T(MBB_HOTPLUG, "status %d\n",   status);
    switch (status)
    {
        case USB_BALONG_DEVICE_INSERT:
            timeout = USB_DETECT_DELAY;
            usb_wake_lock();
            g_usb_notifier_h.stat_usb_insert++;
            g_usb_notifier_h.stat_usb_insert_timestamp = jiffies;
            break;
        case USB_BALONG_DEVICE_REMOVE :
            mod_timer(&g_wake_unlock_timer, jiffies + msecs_to_jiffies(g_wake_unlockt_loop));
            timeout = USB_DETECT_DELAY;
            g_usb_notifier_h.stat_usb_remove++;
            g_usb_notifier_h.stat_usb_remove_timestamp = jiffies;
            break;
        case USB_BALONG_DEVICE_DISABLE:
            g_usb_notifier_h.stat_usb_disable++;
            break;
        case USB_BALONG_ENUM_DONE :
            g_usb_notifier_h.stat_usb_enum_done++;
            break;
        case USB_BALONG_PERIP_INSERT:
            timeout = USB_DETECT_DELAY;
            usb_wake_lock();
            g_usb_notifier_h.stat_usb_perip_insert++;
            break;
        case USB_BALONG_PERIP_REMOVE :
            mod_timer(&g_wake_unlock_timer, jiffies + msecs_to_jiffies(g_wake_unlockt_loop));
            timeout = USB_DETECT_DELAY;
            g_usb_notifier_h.stat_usb_perip_remove++;
            break;
        default:
            DBG_T(MBB_HOTPLUG, "error status:%d\n",   status);
    }

    /*防抖处理*/
    /*增加为两个work u盘和pc分开*/
    /*pmu在电池满时有150ms 产生异常插拔的bug 200ms防抖作为限制*/
    if (USB_BALONG_PERIP_INSERT == status || USB_BALONG_PERIP_REMOVE == status)
    {
        /*作为主机时*/
        usb_stop_work(&g_usb_notifier_h.usb_notify_host_wk);
        usb_start_work(&g_usb_notifier_h.usb_notify_host_wk, timeout);
    }
    else
    {
        /*作为设备时*/
        usb_stop_work(&g_usb_notifier_h.usb_notify_wk);
        usb_start_work(&g_usb_notifier_h.usb_notify_wk, timeout);

    }

    g_usb_notifier_h.usb_status = status;

}
/*****************************************************************
Parameters    :
Return        :    无
Description   :  对外接口，USB注册work.
*****************************************************************/
USB_INT adp_usb_queue_delay_work(struct delayed_work* dwork, USB_ULONG delay)
{
    USB_INT ret = 0;

    if (NULL != usb_hotplug_ctx.usb_notify_wq_extern)
    {
        ret = queue_delayed_work(usb_hotplug_ctx.usb_notify_wq_extern,
                                 dwork, msecs_to_jiffies(delay));
    }
    else
    {
        ret = queue_delayed_work(g_usb_notifier_h.usb_notify_wq,
                                 dwork, msecs_to_jiffies(delay));
    }

    return ret;

}
/*****************************************************************
Parameters    :
Return        :    无
Description   :  USB加系统锁
*****************************************************************/
void usb_wake_lock()
{
    /*非e5类不用加锁*/
#ifdef  USB_E5
    USB_ULONG flags = 0;
    local_irq_save(flags);
    wake_lock(&g_dwc_wakelock);
    local_irq_restore(flags);
#endif
}
/*****************************************************************
Parameters    :
Return        :    无
Description   :  USB系统解锁
*****************************************************************/
void usb_wake_unlock()
{
#ifdef USB_E5
    USB_ULONG flags = 0;
    local_irq_save(flags);
    wake_unlock(&g_dwc_wakelock);
    local_irq_restore(flags);
#endif
}

/**
 * usb_register_notify - register a notifier callback whenever a usb change happens
 * @nb: pointer to the notifier block for the callback events.
 *
 * These changes are either USB devices or busses being added or removed.
 */
void bsp_usb_register_notify(struct notifier_block* nb)
{
    blocking_notifier_chain_register(&usb_notifier_list, nb);
}

/**
 * usb_unregister_notify - unregister a notifier callback
 * @nb: pointer to the notifier block for the callback events.
 *
 * usb_register_notify() must have been previously called for this function
 * to work properly.
 */
void bsp_usb_unregister_notify(struct notifier_block* nb)
{
    blocking_notifier_chain_unregister(&usb_notifier_list, nb);
}


/*****************************************************************
Parameters    :
Return        :    无
Description   :  获取usb通知链句柄
*****************************************************************/
void* usb_get_notifier_handle()
{
    return &usb_notifier_list;
}

/*****************************************************************
Parameters    :
Return        :    无
Description   :  获取USB vbus 是否在位状态
*****************************************************************/
USB_INT usb_get_vbus_status(void)
{
    return recv_vbus_on_intr;
}
/*****************************************************************
Parameters    :
Return        :    无
Description   :  设置USB vbus 是否在位状态
*****************************************************************/
USB_INT usb_set_vbus_status(USB_INT value)
{
    recv_vbus_on_intr = value;
    return 0;
}
/*****************************************************************
Parameters    :
Return        :    无
Description   :  获取USB vbus 是否在位状态
*****************************************************************/
USB_INT usb_get_hotplug_status()
{
    return g_usb_notifier_h.usb_status;
}
/*****************************************************************
Parameters    :
Return        :    无
Description   :  获取上一次USB动作
*****************************************************************/
USB_INT usb_get_hotplug_old_status()
{
    return g_usb_notifier_h.usb_host_status;
}
/*****************************************************************
Parameters    :
Return        :    无
Description   :  获取插入时间戳
*****************************************************************/
USB_ULONG usb_get_insert_timestamp()
{
    return g_usb_notifier_h.stat_usb_insert_timestamp;
}
/*****************************************************************
Parameters    :
Return        :    无
Description   :  获取拔出时间戳
*****************************************************************/
USB_ULONG usb_get_remove_timestamp()
{
    return g_usb_notifier_h.stat_usb_remove_timestamp;
}

EXPORT_SYMBOL_GPL(bsp_usb_unregister_notify);
EXPORT_SYMBOL_GPL(bsp_usb_register_notify);
EXPORT_SYMBOL_GPL(bsp_usb_status_change);
EXPORT_SYMBOL_GPL(adp_usb_queue_delay_work);
EXPORT_SYMBOL_GPL(usb_wake_lock);
EXPORT_SYMBOL_GPL(usb_wake_unlock);
EXPORT_SYMBOL_GPL(usb_get_vbus_status);
EXPORT_SYMBOL_GPL(usb_set_vbus_status);
EXPORT_SYMBOL_GPL(usb_get_hotplug_status);
EXPORT_SYMBOL_GPL(usb_get_hotplug_old_status);
EXPORT_SYMBOL_GPL(usb_get_insert_timestamp);
EXPORT_SYMBOL_GPL(usb_get_remove_timestamp);



/*****************************************************************
Parameters    :
Return        :    无
Description   :  USB插入处理
*****************************************************************/
static void bsp_usb_insert_process(void)
{
    USB_ULONG flags = 0;

    mutex_lock(&g_usb_notifier_h.mutex);
    
    /*maybe changed the HW status so need save the irq */
    local_irq_save(flags);
    g_usb_notifier_h.stat_usb_insert_proc++;
    mlog_print("USB", mlog_lv_info, "stat_usb_insert_proc:%d\n", g_usb_notifier_h.stat_usb_insert_proc);
    usb_broadcast_event(USB_BALONG_DEVICE_INSERT);
    /*USB上电*/
    if (NULL != usb_hotplug_ctx.usb_poweron_cb)
    {
        if (usb_hotplug_ctx.usb_poweron_cb())
        {
            DBG_E(MBB_HOTPLUG, "power on dwc3 usb failed!\n");
            g_usb_notifier_h.stat_usb_poweron_fail++;
            return;
        }

    }
    /*GPIO控制制ID*/
    if (NULL != usb_hotplug_ctx.usbid_proc_cb)
    {
        usb_hotplug_ctx.usbid_proc_cb();
    }
    local_irq_restore(flags);

    /* init the usb driver */
    if (NULL != usb_hotplug_ctx.usb_entry_cb)
    {
        (void)usb_hotplug_ctx.usb_entry_cb();
    }

    /*通知充电类型*/
    usb_broadcast_event(USB_BALONG_CHARGER_IDEN);

    g_usb_notifier_h.stat_usb_insert_proc_end++;
    mlog_print("USB", mlog_lv_info, "stat_usb_insert_proc_end:%d\n", g_usb_notifier_h.stat_usb_insert_proc_end);

    mutex_unlock(&g_usb_notifier_h.mutex);
    
    return;
}
/*****************************************************************
Parameters    :
Return        :    无
Description   :  U盘插入处理
*****************************************************************/
static void bsp_usb_perip_insert_process(void)
{
    USB_INT status = -1;

    mutex_lock(&g_usb_notifier_h.mutex);
    
    g_usb_notifier_h.stat_usb_perip_insert_proc++;
    mlog_print("USB", mlog_lv_info, "stat_usb_perip_insert_proc:%d\n", g_usb_notifier_h.stat_usb_perip_insert_proc);
    /* set usb notifier status in advance to evade unexpect pmu insert intr. */
    status = g_usb_notifier_h.usb_host_old_status;
    g_usb_notifier_h.usb_host_old_status = USB_BALONG_PERIP_INSERT;

    /*usb 上电*/
    if (NULL !=  usb_hotplug_ctx.usb_poweron_cb)
    {
        if (usb_hotplug_ctx.usb_poweron_cb())
        {
            DBG_E(MBB_HOTPLUG, "power on dwc3 usb failed!\n");
            /* restore usb notifier status set in advance */
            g_usb_notifier_h.usb_host_old_status = status;
            g_usb_notifier_h.stat_usb_poweron_fail++;
            return;
        }
    }
 
    if (NULL !=  usb_hotplug_ctx.usbid_proc_cb)
    {
        (void)usb_hotplug_ctx.usbid_proc_cb();
    }

    /* init the usb driver */
    (void)usb_hotplug_ctx.usb_entry_cb();

    g_usb_notifier_h.stat_usb_perip_insert_proc_end++;
    mlog_print("USB", mlog_lv_info, "stat_usb_perip_insert_proc_end:%d\n"
               , g_usb_notifier_h.stat_usb_perip_insert_proc_end);

    mutex_unlock(&g_usb_notifier_h.mutex);
    
    return;
}


/*****************************************************************
Parameters    :
Return        :    无
Description   :  枚举完成处理
*****************************************************************/
static void bsp_usb_enum_done_process(void)
{

    DBG_T(MBB_HOTPLUG, "balong usb enum done,it works well.\n");
    g_usb_notifier_h.stat_usb_enum_done_proc++;

    /*
     * wait for usb enum_done
     */
    if (NULL != usb_hotplug_ctx.usb_wait_enumdone_cb)
    {
        usb_hotplug_ctx.usb_wait_enumdone_cb();
    }

    /* notify kernel notifier */
    usb_broadcast_event(USB_BALONG_ENUM_DONE);
    g_usb_notifier_h.stat_usb_enum_done_proc_end++;
}

/*****************************************************************
Parameters    :
Return        :    无
Description   :  USB设备拔出处理
*****************************************************************/
static void bsp_usb_remove_device_process(void)
{
    DBG_T(MBB_HOTPLUG, "balong usb remove.\n");

    mutex_lock(&g_usb_notifier_h.mutex);
    
    /*清空枚举信息*/
    if (NULL != usb_hotplug_ctx.usb_clear_enumdone_cb)
    {
        usb_hotplug_ctx.usb_clear_enumdone_cb();
    }
    g_usb_notifier_h.stat_usb_remove_proc++;
    mlog_print("USB", mlog_lv_info, "stat_usb_remove_proc:%d\n", g_usb_notifier_h.stat_usb_remove_proc);
    /* notify kernel notifier,
     * we must call notifier list before disable callback,
     * there are something need to do before user
     */
    usb_broadcast_event(USB_BALONG_DEVICE_REMOVE);
    g_usb_notifier_h.charger_type = USB_CHARGER_TYPE_INVALID;

    /* exit the usb driver */
    if (NULL != usb_hotplug_ctx.usb_exit_cb)
    {
        (void)usb_hotplug_ctx.usb_exit_cb();
    }

    mlog_print("USB", mlog_lv_info, "stat_usb_remove_proc_end:%d\n", g_usb_notifier_h.stat_usb_remove_proc_end);

    if (NULL != usb_hotplug_ctx.usb_poweroff_cb)
    {
        if (usb_hotplug_ctx.usb_poweroff_cb())
        {
            DBG_E(MBB_HOTPLUG, KERN_ERR "fail to disable the dwc3 usb regulator\n");
            g_usb_notifier_h.stat_usb_poweroff_fail++;
        }
    }

    usb_wake_unlock();
    g_usb_notifier_h.stat_usb_remove_proc_end++;

    mutex_unlock(&g_usb_notifier_h.mutex);
    
    return;
}
/*****************************************************************
Parameters    :
Return        :    无
Description   :  U盘等设备拔出处理
*****************************************************************/
static void bsp_usb_remove_perip_process(void)
{
    DBG_T(MBB_HOTPLUG, "balong usb peripheral remove.\n");

    mutex_lock(&g_usb_notifier_h.mutex);
    
    g_usb_notifier_h.stat_usb_perip_remove_proc++;
    mlog_print("USB", mlog_lv_info, "stat_usb_perip_remove_proc:%d\n", g_usb_notifier_h.stat_usb_perip_remove_proc);
    usb_broadcast_event(USB_BALONG_PERIP_REMOVE);

    /* exit the usb driver */
    if (NULL != usb_hotplug_ctx.usb_exit_cb)
    {
        (void)usb_hotplug_ctx.usb_exit_cb();
    }

    /* power-off usb hardware */
    if (NULL != usb_hotplug_ctx.usb_poweroff_cb)
    {
        if (usb_hotplug_ctx.usb_poweroff_cb())
        {
            DBG_E(MBB_HOTPLUG, KERN_ERR "fail to disable the dwc3 usb regulator\n");
            g_usb_notifier_h.stat_usb_poweroff_fail++;
        }
    }

    //usb_wake_unlock();
    g_usb_notifier_h.stat_usb_perip_remove_proc_end++;
    g_usb_notifier_h.delta_time = 0;

    mlog_print("USB", mlog_lv_info, "stat_usb_perip_remove_proc_end:%d\n"
               , g_usb_notifier_h.stat_usb_perip_remove_proc_end);

    mutex_unlock(&g_usb_notifier_h.mutex);
    
    return;
}
/*****************************************************************
Parameters    :
Return        :    无
Description   :  设备禁用处理
*****************************************************************/
static void bsp_usb_disable_device_process(void)
{

    DBG_T(MBB_HOTPLUG, "balong usb disable.\n");
    g_usb_notifier_h.stat_usb_disable_proc++;

    /*清空枚举信息*/
    if (NULL != usb_hotplug_ctx.usb_clear_enumdone_cb)
    {
        usb_hotplug_ctx.usb_clear_enumdone_cb();
    }

    /* notify kernel notifier */
    usb_broadcast_event( USB_BALONG_DEVICE_DISABLE);
    g_usb_notifier_h.charger_type = USB_CHARGER_TYPE_INVALID;

    g_usb_notifier_h.stat_usb_disable_proc_end++;


    return;
}

/*****************************************************************
Parameters    :
Return        :    无
Description   :  设置充电类型
*****************************************************************/
USB_VOID bsp_usb_set_charger_type(USB_INT type)
{
    g_usb_notifier_h.charger_type = type;
    return;
}
/*****************************************************************
Parameters    :
Return        :    无
Description   :  获取充电类型
*****************************************************************/
int bsp_usb_get_charger_type(void)
{
    return g_usb_notifier_h.charger_type;
}
/*****************************************************************
Parameters    :
Return        :    无
Description   :  可维可测拔插设备信息
*****************************************************************/
void usb_hotplug_dump(void)
{
    if (USB_CHARGER_TYPE_HUAWEI == g_usb_notifier_h.charger_type)
    {
        DBG_T(MBB_HOTPLUG, "balong usb CHARGER_TYPE: HuaWei     :) \n");
    }
    else if (USB_CHARGER_TYPE_NOT_HUAWEI  == g_usb_notifier_h.charger_type)
    {
        DBG_T(MBB_HOTPLUG, "balong usb CHARGER_TYPE: Not HuaWei :( \n");
    }
    else
    {
        DBG_T(MBB_HOTPLUG, "balong usb CHARGER_TYPE: Invalid    +_+ \n");
    }
    DBG_T(MBB_HOTPLUG, "stat_usb_insert:            %d\n", g_usb_notifier_h.stat_usb_insert);
    DBG_T(MBB_HOTPLUG, "stat_usb_insert_proc:       %d\n", g_usb_notifier_h.stat_usb_insert_proc);
    DBG_T(MBB_HOTPLUG, "stat_usb_insert_proc_end:   %d\n", g_usb_notifier_h.stat_usb_insert_proc_end);
    DBG_T(MBB_HOTPLUG, "stat_usb_enum_done:         %d\n", g_usb_notifier_h.stat_usb_enum_done);
    DBG_T(MBB_HOTPLUG, "stat_usb_enum_done_proc:    %d\n", g_usb_notifier_h.stat_usb_enum_done_proc);
    DBG_T(MBB_HOTPLUG, "stat_usb_enum_done_proc_end:%d\n", g_usb_notifier_h.stat_usb_enum_done_proc_end);
    DBG_T(MBB_HOTPLUG, "stat_usb_remove:            %d\n", g_usb_notifier_h.stat_usb_remove);
    DBG_T(MBB_HOTPLUG, "stat_usb_remove_proc:       %d\n", g_usb_notifier_h.stat_usb_remove_proc);
    DBG_T(MBB_HOTPLUG, "stat_usb_remove_proc_end:   %d\n", g_usb_notifier_h.stat_usb_remove_proc_end);
    DBG_T(MBB_HOTPLUG, "stat_usb_disable:           %d\n", g_usb_notifier_h.stat_usb_disable);
    DBG_T(MBB_HOTPLUG, "stat_usb_disable_proc:      %d\n", g_usb_notifier_h.stat_usb_disable_proc);
    DBG_T(MBB_HOTPLUG, "stat_usb_disable_proc_end:  %d\n", g_usb_notifier_h.stat_usb_disable_proc_end);
    DBG_T(MBB_HOTPLUG, "usb_status:                 %d\n", g_usb_notifier_h.usb_status);
    DBG_T(MBB_HOTPLUG, "usb_old_status:             %d\n", g_usb_notifier_h.usb_old_status);
    DBG_T(MBB_HOTPLUG, "usb_hotplub_state:          %d\n", g_usb_notifier_h.usb_hotplub_state);
    DBG_T(MBB_HOTPLUG, "stat_usb_no_need_notify:    %d\n", g_usb_notifier_h.stat_usb_no_need_notify);
    DBG_T(MBB_HOTPLUG, "stat_usb_poweron_fail:    %d\n", g_usb_notifier_h.stat_usb_poweron_fail);
    DBG_T(MBB_HOTPLUG, "stat_usb_poweroff_fail:    %d\n", g_usb_notifier_h.stat_usb_poweroff_fail);

}

void set_id_delta_time(int mstime)
{
    g_usb_notifier_h.delta_time = mstime;
}
/*
 * usb charger adapter implement
 */
void bsp_usb_status_change_ex(int status)
{
    unsigned long timeout = USB_DETECT_DELAY;
    USB_DBG_VENDOR("%s:status %d\n", __FUNCTION__, status);

    if (USB_BALONG_PERIP_INSERT == status)
    {
        g_usb_notifier_h.stat_usb_perip_insert++;
    }
    else if (USB_BALONG_PERIP_REMOVE == status)
    {
        g_usb_notifier_h.stat_usb_perip_remove++;
    }
    else
    {
        USB_DBG_VENDOR("%s: error status:%d\n", __FUNCTION__, status);
    }

    g_usb_notifier_h.usb_host_status = status;

    if (USB_BALONG_PERIP_INSERT == status || USB_BALONG_PERIP_REMOVE == status)
    {
        cancel_delayed_work(&g_usb_notifier_h.usb_notify_host_wk);
        queue_delayed_work(g_usb_notifier_h.usb_notify_wq,
                           &g_usb_notifier_h.usb_notify_host_wk, msecs_to_jiffies(USB_DETECT_DELAY));

    }


}

void bsp_usb_status_update_ex(int status)
{
    g_usb_notifier_h.usb_host_status = status;
}

/*****************************************************************
Parameters    :
Return        :    无
Description   :  usb插拔工作队列
*****************************************************************/
static void usb_notify_handler(struct work_struct* work)
{
    USB_INT cur_status = g_usb_notifier_h.usb_old_status;
    USB_INT status = 0;
    USB_ULONG flags = 0;
    DBG_T(MBB_HOTPLUG, "old_status %d,status%d\n",
          g_usb_notifier_h.usb_old_status, g_usb_notifier_h.usb_status);

recheck:
    status =  g_usb_notifier_h.usb_status;
    if (g_usb_notifier_h.usb_old_status == status)
    {
        g_usb_notifier_h.stat_usb_no_need_notify++;
    }
    DBG_T(MBB_HOTPLUG, "task of cur_status %d status:%d:start.\n",   cur_status, status);

    if (status != cur_status)
    {
        local_irq_save(flags);
        cur_status = status;
        g_usb_notifier_h.usb_old_status = status;
        local_irq_restore( flags);
        switch (status)
        {
            case USB_BALONG_DEVICE_INSERT:

                /*判断是否无线充电功能*/
                if (NULL  != usb_hotplug_ctx.usb_check_wireless_chg_cb)
                {
                    if (1 == usb_hotplug_ctx.usb_check_wireless_chg_cb())
                    {
                        DBG_T(MBB_HOTPLUG, "wireless charger detected\n");
                        break;
                    }
                }
                if (USB_BALONG_HOTPLUG_INSERT
                    == g_usb_notifier_h.usb_hotplub_state)
                {
                    DBG_T(MBB_HOTPLUG, "task of %d cancel.\n",   cur_status);
                    return ;
                }
                g_usb_notifier_h.usb_hotplub_state = USB_BALONG_HOTPLUG_INSERT;
                //local_irq_save(flags);
                bsp_usb_insert_process();
                //local_irq_restore(flags);

                break;
            case USB_BALONG_PERIP_INSERT:
                local_irq_save(flags);
                bsp_usb_perip_insert_process();
                local_irq_restore(flags);
                break;
            case USB_BALONG_ENUM_DONE:
                bsp_usb_enum_done_process();
                break;
            case USB_BALONG_DEVICE_REMOVE:
                /*判断是否无线充电功能*/
                if (NULL  != usb_hotplug_ctx.usb_wireless_chg_remove_cb)
                {
                    if (1 == usb_hotplug_ctx.usb_wireless_chg_remove_cb())
                    {
                        DBG_T(MBB_HOTPLUG, "wireless charger remove\n");
                        break;
                    }
                }

                if (USB_BALONG_HOTPLUG_REMOVE
                    == g_usb_notifier_h.usb_hotplub_state)
                {
                    DBG_T(MBB_HOTPLUG, "task of %d cancel.\n",   cur_status);
                    return ;
                }
                g_usb_notifier_h.usb_hotplub_state = USB_BALONG_HOTPLUG_REMOVE;
                //local_irq_save(flags);
                bsp_usb_remove_device_process();
                //local_irq_restore(flags);

                break;
            case USB_BALONG_PERIP_REMOVE:
                local_irq_save(flags);
                bsp_usb_remove_perip_process();
                local_irq_restore(flags);
                break;
            case USB_BALONG_DEVICE_DISABLE:
                bsp_usb_disable_device_process();
                break;
            default:
                DBG_T(MBB_HOTPLUG, "invalid status:%d\n",
                      cur_status);
                return;
        }
        DBG_T(MBB_HOTPLUG, "recheck\n");
        goto recheck;
    }

    DBG_T(MBB_HOTPLUG, "task of %d end.\n",   cur_status);

    g_usb_notifier_h.usb_old_status = cur_status;
    return;
}

static void usb_notify_handler_ex(struct work_struct* work)
{
    int cur_status = g_usb_notifier_h.usb_host_status;

    USB_DBG_VENDOR("%s:old_status %d,cur_status%d\n",
                   __FUNCTION__, g_usb_notifier_h.usb_old_status, cur_status);

    if (g_usb_notifier_h.usb_host_old_status == cur_status)
    {
        g_usb_notifier_h.stat_usb_no_need_notify++;
        return;
    }

    USB_DBG_VENDOR("%s:task of %d start.\n", __FUNCTION__, cur_status);

    switch (cur_status)
    {
        case USB_BALONG_PERIP_REMOVE:
            bsp_usb_remove_perip_process();
            break;
        case USB_BALONG_PERIP_INSERT:
            bsp_usb_perip_insert_process();
            break;
        default:
            USB_DBG_VENDOR("%s, invalid status:%d\n",
                           __FUNCTION__, cur_status);
            return;
    }

    USB_DBG_VENDOR("%s:task of %d end.\n", __FUNCTION__, cur_status);

    g_usb_notifier_h.usb_host_old_status = cur_status;
    return;
}

/*****************************************************************
Parameters    :
Return        :    无
Description   :  USB hotplug解wakelock锁
*****************************************************************/
USB_VOID wake_unlock_handler( USB_ULONG data )
{
    USB_ULONG flags = 0;

    local_irq_save(flags);
    DBG_T(MBB_HOTPLUG, "usb_status : %d.\n",   g_usb_notifier_h.usb_status);
    if ( USB_BALONG_DEVICE_REMOVE == g_usb_notifier_h.usb_status)
    {
        wake_unlock(&g_dwc_wakelock);
    }
    local_irq_restore(flags);
}


/*****************************************************************
Parameters    :
Return        :    无
Description   :  USB hotplug模块初始化
*****************************************************************/

USB_INT usb_hotplug_init(USB_VOID)
{
    USB_INT ret = 0;

    /*创建通知*/
    /* init local ctx resource */
    /*c标准：全局变量(external variable)和静态变量 (static variable)的初始化式必须为常量表达式*/
    usb_hotplug_ctx.usb_notify_wq_extern = system_nrt_wq;
    g_usb_notifier_h.charger_type = USB_CHARGER_TYPE_INVALID;
    mutex_init(&g_usb_notifier_h.mutex);
    g_usb_notifier_h.usb_notify_wq = create_singlethread_workqueue("usb_notify");
    if (!g_usb_notifier_h.usb_notify_wq)
    {
        DBG_T(MBB_HOTPLUG, "create_singlethread_workqueue fail\n");
        ret = -1;
    }
    INIT_DELAYED_WORK(&g_usb_notifier_h.usb_notify_wk, (void*)usb_notify_handler);
    INIT_DELAYED_WORK(&g_usb_notifier_h.usb_notify_host_wk, (void*)usb_notify_handler_ex);

    /*创建usb 的wakelock锁*/
    wake_lock_init(&g_dwc_wakelock, WAKE_LOCK_SUSPEND, "dwc3-wakelock");

    setup_timer(&g_wake_unlock_timer , wake_unlock_handler, (USB_ULONG)0);

    /*初始化回调*/
    return ret;
}



