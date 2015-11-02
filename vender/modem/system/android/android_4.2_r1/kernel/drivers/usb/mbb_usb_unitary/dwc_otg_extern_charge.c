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

*****************************************************************************************************/
#include <linux/timer.h>
#include <linux/notifier.h>
#include <linux/irqreturn.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/delay.h>
#include <linux/usb/usb_interface_external.h>
#include "usb_vendor.h"
#include "dwc_otg_extern_charge.h"
#include "usb_charger_manager.h"
#include "mbb_usb_adp.h"
#include "usb_platform_comm.h"
#include "usb_debug.h"
#include "usb_notify.h"
#include <product_config.h>
#include <mbb_config.h>
#include "mbb_fast_on_off.h"


#define DWC_OTG_AB      0xAB
#define OTG_DETECT_OVERTIME 2000
#define  INVALID_CMD  (-1)

static USB_INT  otg_old_cmd = INVALID_CMD;
static USB_INT  otg_new_cmd = INVALID_CMD;

otg_charger_st g_otg_extern_charge;
USB_INT extern_otg_dev = DWC_OTG_AB;
static USB_INT g_otg_usb2lan = MBB_USB_FALSE;


static  USB_INT otg_is_host_mode = MBB_USB_FALSE;


static USB_INT usb_host_charger_current_notify(struct notifier_block* self,
        USB_ULONG action, USB_PVOID dev);


static struct notifier_block usb_host_charger_current_nb =
{
    .notifier_call = usb_host_charger_current_notify
};

USB_INT old_otg_inserted_status = GPIO_HIGH;
USB_INT curr_otg_inserted_status = GPIO_HIGH;
USB_INT otg_det_gpio_value = 1;
USB_INT otg_is_inserted = MBB_USB_FALSE;
static USB_INT otg_first_report_chg_type = 1;


/*********************************************************************
函数  : otg_detect_id_thread
功能  : 通知充电模块，对外充电线插拔事件
参数  : w
返回值: void
*********************************************************************/
#define vbus_irq_delay 300
USB_VOID otg_detect_id_thread( USB_VOID )
{
    usb_adp_ctx_t* ctx = NULL;
    USB_INT fast_on_off = 0;
    fast_on_off = usb_fast_on_off_stat();
    ctx = usb_get_adp_ctx();
    if (NULL == ctx)
    {
        DBG_E(MBB_OTG_CHARGER, "--otg_detect_id_thread:  ctx == NULL -- \n");
        return;
    }
    /*判断充电模式，是否可执行对外充电*/
    if (!usb_chg_check_current_mode(USB_CHG_EXTERN))
    {
        DBG_E(MBB_OTG_CHARGER, "charge mode is mutually exclusive \n");
        return;
    }
    /*判断是否为假关机，如果是，单板可休眠*/
    if (fast_on_off)
    {
        wake_unlock(&g_otg_extern_charge.otg_chg_wakelock);
        return;
    }

    DBG_I(MBB_OTG_CHARGER, "old_otg_inserted_status = %d,curr_otg_inserted_status=%d", \
                            old_otg_inserted_status, curr_otg_inserted_status);

    /*判断当前上报状态与之前相同，不上报消息，返回*/
    if (old_otg_inserted_status == curr_otg_inserted_status )
    {
        DBG_E(MBB_OTG_CHARGER,"old_otg_inserted_status == curr_otg_inserted_status\n");
        return;
    }

    old_otg_inserted_status = curr_otg_inserted_status;
    DBG_I(MBB_OTG_CHARGER, "notify usb id is %8s.\n", 
        otg_is_inserted ? "inserted" : "removed");

    /*AF18为usb转网卡类产品，此处应该增加usb转网卡产品特性宏*/
    /*不支持usb转网卡特性,判断为usb转网卡时拔插 不作处理###*/
    msleep(vbus_irq_delay);
    if ((MBB_USB_TRUE == otg_is_inserted) && (MBB_USB_TRUE == usb_get_vbus_status()))
    {
        g_otg_usb2lan = MBB_USB_TRUE;
        DBG_I(MBB_OTG_CHARGER, "OTG device AF18 is inserted.\r\n");
        return;
    }
    else if (MBB_USB_FALSE == otg_is_inserted)
    {
        /*判断是否usb转网卡拨出*/
        if (g_otg_usb2lan == MBB_USB_TRUE)
        {
            /*清空usb转网卡状态*/
            g_otg_usb2lan = MBB_USB_FALSE;
            DBG_I(MBB_OTG_CHARGER, "OTG device AF18 is removed.\r\n");
            return;
        }
    }

    /*第一次上报，直接置充电类型，防止充电没起*/
    if (ctx->stm_set_chg_type_cb && otg_first_report_chg_type && otg_is_inserted)
    {
        DBG_I(MBB_OTG_CHARGER, "-----first notify exchg----\n");
        ctx->stm_set_chg_type_cb(CHG_EXGCHG_CHGR);
        otg_first_report_chg_type = 0;
    }
    /*对外充电线插入*/
    if (otg_is_inserted)
    {   
        wake_lock(&g_otg_extern_charge.otg_chg_wakelock);
        /*设置当前充电工作模式为对外充电*/
        usb_chg_set_work_mode(USB_CHG_EXTERN);
        otg_old_cmd = INVALID_CMD;
        otg_new_cmd = INVALID_CMD;
        ctx->battery_notifier_call_chain_cb(otg_is_inserted, CHG_EXGCHG_CHGR);
        msleep(OTG_DETECT_OVERTIME);
    }
    /*对外充电线拔出*/
    else
    {
        ctx->battery_notifier_call_chain_cb(otg_is_inserted, CHG_EXGCHG_CHGR);
        usb_notify_event(USB_OTG_ID_PULL_OUT, NULL);
        msleep(OTG_DETECT_OVERTIME);
        /*退出对外充电，清空充电工作模式*/
        usb_chg_set_work_mode(USB_CHG_NON);
        otg_old_cmd = INVALID_CMD;
        otg_new_cmd = INVALID_CMD;
        wake_unlock(&g_otg_extern_charge.otg_chg_wakelock);
    }
}

static irqreturn_t dwc_otg_det_irq(USB_INT irq, USB_PVOID dev_id)
{
    USB_INT is_fast_off = 0;
    DBG_I(MBB_OTG_CHARGER, "dwc_otg_det_irq:enter\n");


    otg_det_gpio_value = gpio_get_value(GPIO_OTG_ID_DET);
    /*
    如果gpio上报了多次中断，2次都是一个消息，那么认为消息
    有误，不做处理。
    */
    curr_otg_inserted_status = otg_det_gpio_value;

    /*
    如果是关机模式，不启动定时器上报消息，但是记录otg状态
    */
    is_fast_off = usb_fast_on_off_stat();
    if (otg_det_gpio_value)
    {
        gpio_int_trigger_set(GPIO_OTG_ID_DET, IRQ_TYPE_LEVEL_LOW);
        /* ID 线拔出 */
        DBG_I(MBB_OTG_CHARGER, "OTG_ID pull out!\n");
        otg_is_inserted = MBB_USB_FALSE;
    }
    else
    {
        gpio_int_trigger_set(GPIO_OTG_ID_DET, IRQ_TYPE_LEVEL_HIGH);
        /* ID 线插入*/
        DBG_I(MBB_OTG_CHARGER, "OTG_ID pull in!\n");
        otg_is_inserted = MBB_USB_TRUE;
    }
    if (!is_fast_off)
    {
        schedule_work(&g_otg_extern_charge.otg_chg_notify_work);
    }
    else
    {
        DBG_E(MBB_OTG_CHARGER, "--it is in fast_off mode--\n");

    }
    return IRQ_HANDLED;
}

static USB_INT otg_register_otg_det_irq(USB_VOID)
{
    const char* desc = "OTG_DET_GPIO";
    irq_handler_t isr = NULL;
    unsigned long irqflags = 0;
    USB_INT irq = -1;
    USB_INT error = -1;
    if (gpio_is_valid(GPIO_OTG_ID_DET))
    {
        DBG_I(MBB_OTG_CHARGER, "gipo is valid\n");
        error = gpio_request(GPIO_OTG_ID_DET, "otg_int");
        if (error < 0)
        {
            DBG_E(MBB_OTG_CHARGER, "Failed to request GPIO %d, error %d\n", GPIO_OTG_ID_DET, error);
            return error;
        }
        gpio_int_mask_set(GPIO_OTG_ID_DET);
        gpio_int_state_clear(GPIO_OTG_ID_DET);
        error = gpio_direction_input(GPIO_OTG_ID_DET);
        if (error < 0)
        {
            DBG_E(MBB_OTG_CHARGER, "Failed to configure direction for GPIO %d, error s%d\n", GPIO_OTG_ID_DET, error);
            goto fail;
        }

        gpio_set_function(GPIO_OTG_ID_DET, GPIO_INTERRUPT);

        /*这里是处理，插着usb id线上电时，需要判断一下当前的id状态，通知充电模块是否充电*/
        dwc_otg_det_irq(NULL, NULL);

        irq = gpio_to_irq(GPIO_OTG_ID_DET);
        if (irq < 0)
        {
            error = irq;
            DBG_E(MBB_OTG_CHARGER, "Unable to get irq number for GPIO %d, error %d\n", GPIO_OTG_ID_DET, error);
            goto fail;
        }

        isr = dwc_otg_det_irq;
        irqflags = IRQF_NO_SUSPEND | IRQF_SHARED;

    }


    error = request_irq(irq, isr, irqflags, desc, &extern_otg_dev);
    if (error < 0)
    {
        DBG_E(MBB_OTG_CHARGER, "Unable to claim irq %d; error %d\n", irq, error);

        goto fail;
    }
    gpio_int_state_clear(GPIO_OTG_ID_DET);
    gpio_int_unmask_set(GPIO_OTG_ID_DET);

    return MBB_USB_OK;

fail:
    if (gpio_is_valid(GPIO_OTG_ID_DET))
    {
        gpio_free(GPIO_OTG_ID_DET);
    }

    return error;
}

static USB_INT otg_unregister_otg_det_irq(USB_VOID)
{
    if (gpio_is_valid(GPIO_OTG_ID_DET))
    {
        gpio_free(GPIO_OTG_ID_DET);
    }
    if (gpio_is_valid(GPIO_DMDP_CONNECT))
    {
        gpio_free(GPIO_DMDP_CONNECT);
    }
    if (gpio_is_valid(GPIO_OTG_ID_SET))
    {
        gpio_free(GPIO_OTG_ID_SET);
    }

    old_otg_inserted_status = GPIO_HIGH;
    curr_otg_inserted_status = GPIO_HIGH;
    return MBB_USB_OK;
}

/*********************************************************************
函数  :otg_exchg_status_clean
功能  :进入假关机后，清除对外充电状态
参数  :
返回值:
*********************************************************************/
static USB_VOID otg_exchg_status_clean(USB_VOID)
{
    USB_INT charger_type = USB_CHARGER_TYPE_INVALID;
    struct blocking_notifier_head *usb_notifier_list = NULL;
    gpio_int_mask_set(GPIO_OTG_ID_DET);
    gpio_int_state_clear(GPIO_OTG_ID_DET);

    /*断开D+,D-,拉高 phy ID */
    gpio_set_value(GPIO_DMDP_CONNECT, GPIO_LOW);
    gpio_set_value(GPIO_OTG_ID_SET, GPIO_HIGH);

    if (MBB_USB_TRUE == otg_is_host_mode)
    {
        /*通知关闭OTG线程*/
	usb_notifier_list = usb_get_notifier_handle();
        blocking_notifier_call_chain(usb_notifier_list,
                             USB_BALONG_PERIP_REMOVE, (void*)&charger_type);
        usb_balong_exit();
        if (power_off_dwc3_usb())
        {
            DBG_E(MBB_OTG_CHARGER , "fail to disable the dwc3 usb regulator!\n");
        }
    }
    otg_old_cmd = INVALID_CMD;
    otg_new_cmd = INVALID_CMD;
    otg_is_host_mode = MBB_USB_FALSE;
    old_otg_inserted_status = GPIO_HIGH;
    usb_chg_set_work_mode(USB_CHG_NON);
    wake_unlock(&g_otg_extern_charge.otg_chg_wakelock);
}

/*********************************************************************
函数  :otg_switch_to_host_mode
功能  :对外充电切换到 OTG 模式充电，500mA
参数  :
返回值:
*********************************************************************/
static USB_VOID otg_switch_to_host_mode(USB_VOID)
{
    /*初始化 USB 驱动，进入  OTG  host mode*/
    if (power_on_dwc3_usb())
    {
        DBG_E(MBB_OTG_CHARGER, "---power on dwc3 usb failed!---\n");
        return;
    }
    usb_balong_init();
    otg_is_host_mode = MBB_USB_TRUE;
    /*断开 D+, D- , 拉低 ID */
    gpio_set_value(GPIO_DMDP_CONNECT, GPIO_LOW);
    gpio_set_value(GPIO_OTG_ID_SET, GPIO_LOW);
}

/*********************************************************************
函数  :otg_exchg_remove
功能  :对外充电线拔出处理
参数  :
返回值:
*********************************************************************/
static USB_VOID otg_exchg_remove(USB_VOID)
{
    USB_INT charger_type = USB_CHARGER_TYPE_INVALID;
   struct blocking_notifier_head *usb_notifier_list = NULL;
    DBG_E(MBB_OTG_CHARGER , "%s: USB_OTG_ID_PULL_OUT \n",
                             __func__  );

    /*断开D+,D-,拉高 phy ID */
    gpio_set_value(GPIO_DMDP_CONNECT, GPIO_LOW);
    gpio_set_value(GPIO_OTG_ID_SET, GPIO_HIGH);

    if (MBB_USB_TRUE == otg_is_host_mode)
    {
        /*通知关闭OTG 线程*/
        usb_notifier_list = usb_get_notifier_handle();
        blocking_notifier_call_chain(&usb_notifier_list,
                                 USB_BALONG_PERIP_REMOVE, (void*)&charger_type);
        usb_balong_exit();
        if (power_off_dwc3_usb())
        {
            DBG_E(MBB_OTG_CHARGER , "fail to disable the dwc3 usb regulator!\n");
        }
    }
    otg_is_host_mode = MBB_USB_FALSE;
    /*此处不清空usb转网卡置位###*/
}

/*********************************************************************
函数  :usb_check_repeat_cmd
功能  :检测是否重复接收到同样的命令，如果是，
               不予理会，注意( cmd >= 0)
参数  :
返回值:
*********************************************************************/
USB_BOOL  usb_check_repeat_cmd(USB_INT cmd)
{
    USB_BOOL  ret = MBB_USB_FALSE;

    if (cmd < 0)
    {
        DBG_E(MBB_OTG_CHARGER ," illegal cmd=%d < 0 \n", cmd);
    }
    otg_new_cmd = cmd;
    DBG_E(MBB_OTG_CHARGER , "recieve cmd = %d \n", cmd);
    if ( otg_old_cmd ==  otg_new_cmd)
    {
        ret = MBB_USB_FALSE;
        DBG_E(MBB_OTG_CHARGER , "recieve repeated cmd = %d , ignore it\n", otg_new_cmd);
    }
    else
    {
        otg_old_cmd =  otg_new_cmd;
        ret = MBB_USB_TRUE;
    }
    return ret;
}

/*********************************************************************
函数  :usb_otg_current_notify
功能  :根据输入参数，设置USB状态
参数  :action:
返回值:
*********************************************************************/
static USB_INT usb_host_charger_current_notify(struct notifier_block* self,
        USB_ULONG action, USB_PVOID dev)
{

    /*判断充电模式，是否可执行对外充电*/
    if (!usb_chg_check_current_mode(USB_CHG_EXTERN))
    {
        DBG_E(MBB_OTG_CHARGER, "charge mode is mutually exclusive \n");
        return;
    }

    if (!usb_check_repeat_cmd(action))
    {
        DBG_E(MBB_OTG_CHARGER, "repeated action = %ld ,do nothing.\n", action);
        return MBB_USB_ERROR;
    }

    gpio_int_mask_set(GPIO_DMDP_CONNECT);
    gpio_int_state_clear(GPIO_DMDP_CONNECT);
    gpio_direction_output(GPIO_DMDP_CONNECT, 0);


    gpio_int_mask_set(GPIO_OTG_ID_SET);
    gpio_int_state_clear(GPIO_OTG_ID_SET);
    gpio_direction_output(GPIO_OTG_ID_SET, 1);

    DBG_I(MBB_OTG_CHARGER, "usb_otg_current_notify action is %ld.\n", action);

    switch (action)
    {
        case USB_OTG_CONNECT_DP_DM:
            /*短接 D+ D- , 进行 1 A 充电*/
            gpio_set_value(GPIO_DMDP_CONNECT, GPIO_HIGH);
            break;
        /* 断开D+ D-连接以后，拉低 ID 线 进入Host 模式，进行otg充电 */
        case USB_OTG_DISCONNECT_DP_DM:
            otg_switch_to_host_mode();
            break;
            /*对外充电线，拔出*/
        case USB_OTG_ID_PULL_OUT:
            otg_exchg_remove();
            break;
            /*进入假关机处理流程*/
        case USB_OTG_DISABLE_ID_IRQ:
            /*模拟真关机流程，使假关机恢复到初始状态 */
            otg_exchg_status_clean();
            break;
            /*退出假关机处理流程*/
        case USB_OTG_ENABLE_ID_IRQ:
            /* 获取当前ID GPIO 状态，如果是低ID 在位上报ID插入事件,
            重新进入对外充电流程 */
            dwc_otg_det_irq(NULL, NULL);
            gpio_int_state_clear(GPIO_OTG_ID_DET);
            gpio_int_unmask_set(GPIO_OTG_ID_DET);
            break;
        default:
            break;
    }

    return MBB_USB_OK;
}

USB_INT chager_test(USB_ULONG action)
{
    gpio_int_mask_set(GPIO_DMDP_CONNECT);
    gpio_int_state_clear(GPIO_DMDP_CONNECT);
    gpio_direction_output(GPIO_DMDP_CONNECT, 0);

    gpio_int_mask_set(GPIO_OTG_ID_SET);
    gpio_int_state_clear(GPIO_OTG_ID_SET);
    gpio_direction_output(GPIO_OTG_ID_SET, 1);

    DBG_I(MBB_OTG_CHARGER, "usb_otg_current_notify action is %ld.\n", action);

    switch (action)
    {
        case USB_OTG_CONNECT_DP_DM:
            gpio_set_value(GPIO_DMDP_CONNECT, GPIO_HIGH);
            break;

        case USB_OTG_DISCONNECT_DP_DM:
            /* 断开D+ D-连接以后，拉低 ID 线 进入Host 模式，进行otg充电 */
            gpio_set_value(GPIO_DMDP_CONNECT, GPIO_HIGH);
            gpio_set_value(GPIO_DMDP_CONNECT, GPIO_LOW);
            gpio_set_value(GPIO_OTG_ID_SET, GPIO_LOW);
            break;

        case USB_OTG_ID_PULL_OUT:
            /* ID拔出 断开D+ D-连接，拉高 ID 线 恢复USB 模式 */
            gpio_set_value(GPIO_OTG_ID_SET, GPIO_HIGH);
            gpio_set_value(GPIO_DMDP_CONNECT, GPIO_LOW);
            break;

        default:
            break;
    }

    return MBB_USB_OK;
}

EXPORT_SYMBOL(chager_test);
/*********************************************************************
函数  : usb_otg_extern_charge_init
功能  : 对外充电事件初始化
参数  : void
返回值: void
*********************************************************************/
USB_VOID usb_otg_extern_charge_init(USB_VOID)
{
    USB_INT ret = 0;

    ret = gpio_request(GPIO_DMDP_CONNECT, "OTG_DP_DM_CONNECT_GPIO");
    if (ret < 0)
    {
        DBG_E(MBB_OTG_CHARGER, "gpio request failed for OTG_DP_DM_CONNECT_GPIO\n");
        gpio_free(GPIO_DMDP_CONNECT);
    }
    ret = gpio_request(GPIO_OTG_ID_SET, "GPIO_OTG_ID_SET");
    if (ret < 0)
    {
        DBG_E(MBB_OTG_CHARGER, "gpio request failed for GPIO_OTG_ID_SET\n");
        gpio_free(GPIO_OTG_ID_SET);
    }
    wake_lock_init(&g_otg_extern_charge.otg_chg_wakelock, WAKE_LOCK_SUSPEND, "otg-wakelock");
    INIT_WORK(&g_otg_extern_charge.otg_chg_notify_work, otg_detect_id_thread);
    init_waitqueue_head(&g_otg_extern_charge.wait_wq);
    /*初始化对外充事件电接口，供充电模块使用*/
    usb_register_otg_notify(&usb_host_charger_current_nb);
    otg_register_otg_det_irq();

}

/*********************************************************************
函数  : wakeup_wait_wq
功能  : 唤醒等待事件的接口
参数  : void
返回值: void
*********************************************************************/
USB_VOID wakeup_wait_wq(USB_VOID)
{
    wake_up_interruptible(&g_otg_extern_charge.wait_wq);
}
EXPORT_SYMBOL_GPL(wakeup_wait_wq);

/*********************************************************************
函数  : usb_otg_extern_charge_remove
功能  : 对外充电事件卸载
参数  : viod
返回值: void
*********************************************************************/
USB_VOID usb_otg_extern_charge_remove(USB_VOID)
{
    /*移除对外充事件电接口*/
    usb_unregister_otg_notify(&usb_host_charger_current_nb);
    otg_unregister_otg_det_irq();
    cancel_work_sync(&g_otg_extern_charge.otg_chg_notify_work);
    wake_lock_destroy(&g_otg_extern_charge.otg_chg_wakelock);

}

