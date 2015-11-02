
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
#include <linux/irq.h>
#include <linux/irqreturn.h>
#include <linux/interrupt.h>
#include <linux/usb/usb_interface_external.h>

#include "usb_otg_extern_charge.h"
#include "usb_debug.h"
#include "usb_notify.h"
#include "mbb_fast_on_off.h"
#include "mbb_usb_adp.h"
#include "usb_charger_manager.h"

/************************** 宏定义 **********************************/

#define DWC_OTG_AB          0xAB

#define OTG_DETECT_OVERTIME 2000

#define INVALID_CMD         (-1)

/* 各种对外充电模式所对应的GPIO状态 */
enum ec_gpio_state {
	EC_GPIO_1A_CHARGE,
	EC_GPIO_OTG_CHARGE,
	EC_GPIO_RESET,
	EC_GPIO_STATE_INVALID = 0xFFFFFFFF,
};


/************************** 全局变量 **********************************/

otg_charger_st g_otg_extern_charge;

extern struct blocking_notifier_head usb_balong_notifier_list;


/************************** 内部函数声明 **********************************/

static USB_VOID usb_otg_ec_reset_sys_cmd(USB_VOID);

static USB_VOID usb_otg_ec_id_notify_handler(USB_VOID);

/************************** 内部函数定义 **********************************/

/* 单板休眠投票操作函数 */

static USB_VOID usb_otg_ec_init_lpm(USB_VOID)
{
    wake_lock_init(&g_otg_extern_charge.otg_ec_wakelock, WAKE_LOCK_SUSPEND, "otg-wakelock");
}

static USB_VOID usb_otg_ec_uninit_lpm(USB_VOID)
{
    wake_lock_destroy(&g_otg_extern_charge.otg_ec_wakelock);
}

static USB_VOID usb_otg_ec_set_lpm_lock(USB_INT state)
{
    DBG_T(MBB_OTG_CHARGER, "state=%d\n", state);
    
    if (state)
    {
        wake_lock(&g_otg_extern_charge.otg_ec_wakelock);
    }
    else
    {
        wake_unlock(&g_otg_extern_charge.otg_ec_wakelock);
    }
}


/* gpio 操作函数定义 */

static USB_VOID usb_otg_ec_fresh_gpio_status(USB_VOID)
{
    USB_INT is_fast_off = usb_fast_on_off_stat();
    USB_INT det_gpio_value = gpio_get_value(GPIO_OTG_ID_DET);
    
    DBG_T(MBB_OTG_CHARGER, "fast_off=%d, det_val=%d\n", is_fast_off, det_gpio_value);

    g_otg_extern_charge.det_id_gpio_value = det_gpio_value;

    if (det_gpio_value)
    {
        gpio_int_trigger_set(GPIO_OTG_ID_DET, IRQ_TYPE_LEVEL_LOW);
    }
    else
    {
        gpio_int_trigger_set(GPIO_OTG_ID_DET, IRQ_TYPE_LEVEL_HIGH);
    }
    
    if (is_fast_off)
    {
        DBG_E(MBB_OTG_CHARGER, "in fast off state\n");
        return;
    } 

    schedule_work(&g_otg_extern_charge.otg_ec_id_work);
}

static irqreturn_t usb_otg_id_det_irq(USB_INT irq, USB_PVOID dev_id)
{
    DBG_T(MBB_OTG_CHARGER, "\n");
    usb_otg_ec_fresh_gpio_status();
    return IRQ_HANDLED;
}

static USB_INT usb_otg_register_id_det_irq(USB_VOID)
{
    const USB_CHAR *desc = "OTG_DET_GPIO";
    irq_handler_t isr = NULL;
    USB_ULONG irqflags = 0;
    USB_INT irq = -1;
    USB_INT error = -1;
    static USB_INT extern_otg_dev = DWC_OTG_AB;
    
    if (!gpio_is_valid(GPIO_OTG_ID_DET))
    {
        DBG_T(MBB_OTG_CHARGER, "gipo %u is invalid\n", GPIO_OTG_ID_DET);
        goto fail;
    }
        
    error = gpio_request(GPIO_OTG_ID_DET, "otg_int");
    if (error < 0)
    {
        DBG_E(MBB_OTG_CHARGER, "Request GPIO %d error %d\n", GPIO_OTG_ID_DET, error);
        goto fail;
    }

    gpio_int_mask_set(GPIO_OTG_ID_DET);
    gpio_int_state_clear(GPIO_OTG_ID_DET);
    
    error = gpio_direction_input(GPIO_OTG_ID_DET);
    if (error < 0)
    {
        DBG_E(MBB_OTG_CHARGER, "config GPIO %d error %d\n", GPIO_OTG_ID_DET, error);
        goto fail;
    }

    gpio_set_function(GPIO_OTG_ID_DET, GPIO_INTERRUPT);

    /*这里是处理，插着usb id线上电时，需要判断一下当前的id状态，通知充电模块是否充电*/
    usb_otg_ec_fresh_gpio_status();

    irq = gpio_to_irq(GPIO_OTG_ID_DET);
    if (irq < 0)
    {
        error = irq;
        DBG_E(MBB_OTG_CHARGER, "get irq number for GPIO %d error %d\n", GPIO_OTG_ID_DET, error);
        goto fail;
    }

    isr = usb_otg_id_det_irq;
    irqflags = IRQF_NO_SUSPEND | IRQF_SHARED;

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

static USB_VOID usb_otg_ec_gpio_work(struct work_struct *work)
{
    usb_otg_ec_id_notify_handler();
}

static USB_VOID usb_otg_ec_set_gpio_state(enum ec_gpio_state state)
{
    DBG_T(MBB_OTG_CHARGER, "state=%u\n", state);
    
    switch (state)
    {
        case EC_GPIO_1A_CHARGE :
            gpio_set_value(GPIO_DMDP_CONNECT, GPIO_HIGH);
            break;
            
        case EC_GPIO_OTG_CHARGE :
            gpio_set_value(GPIO_DMDP_CONNECT, GPIO_LOW);
            gpio_set_value(GPIO_OTG_ID_SET, GPIO_LOW);
            break;

        case EC_GPIO_RESET :
            gpio_set_value(GPIO_DMDP_CONNECT, GPIO_LOW);
            gpio_set_value(GPIO_OTG_ID_SET, GPIO_HIGH);
            break;
            
        default :
            break;
    }
}

static USB_INT usb_otg_ec_get_id_insert_state(USB_VOID)
{
    /* ID线插入状态值与gpio值相反 */
    return !g_otg_extern_charge.det_id_gpio_value; 
}

static USB_VOID usb_otg_ec_init_gpio(USB_VOID)
{
    USB_INT ret = 0;
    
    ret = gpio_request(GPIO_DMDP_CONNECT, "OTG_DP_DM_CONNECT_GPIO");
    if (ret < 0)
    {
        DBG_E(MBB_OTG_CHARGER, "gpio request failed for OTG_DP_DM_CONNECT_GPIO\n");
        gpio_free(GPIO_DMDP_CONNECT);
    }
    gpio_direction_output(GPIO_DMDP_CONNECT, 0);
    
    ret = gpio_request(GPIO_OTG_ID_SET, "GPIO_OTG_ID_SET");
    if (ret < 0)
    {
        DBG_E(MBB_OTG_CHARGER, "gpio request failed for GPIO_OTG_ID_SET\n");
        gpio_free(GPIO_OTG_ID_SET);
    }  
    gpio_direction_output(GPIO_OTG_ID_SET, 1);

    INIT_WORK(&g_otg_extern_charge.otg_ec_id_work, usb_otg_ec_gpio_work);
    g_otg_extern_charge.det_id_gpio_value = GPIO_HIGH;
    g_otg_extern_charge.last_det_id_gpio_value = GPIO_HIGH;
    
    ret = usb_otg_register_id_det_irq();
    if (ret < 0)
    {
        DBG_E(MBB_OTG_CHARGER, "reg id det irq failed error %d\n", ret);
    } 
}

static USB_VOID usb_otg_ec_reset_gpio_value(USB_VOID)
{
    g_otg_extern_charge.det_id_gpio_value = GPIO_HIGH;
    g_otg_extern_charge.last_det_id_gpio_value = GPIO_HIGH;    
}

static USB_INT usb_otg_ec_is_repeat_gpio_value(USB_VOID)
{
    USB_INT is_repeat = 0;
    
    if (g_otg_extern_charge.last_det_id_gpio_value == g_otg_extern_charge.det_id_gpio_value)
    {
        is_repeat = 1;
    }

    g_otg_extern_charge.last_det_id_gpio_value = g_otg_extern_charge.det_id_gpio_value;

    return is_repeat;
}

static USB_VOID usb_otg_ec_uninit_gpio(USB_VOID)
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

    cancel_work_sync(&g_otg_extern_charge.otg_ec_id_work);

    usb_otg_ec_reset_gpio_value();
}


/* 系统事件接口函数 */

static USB_VOID usb_otg_ec_reset_sys_cmd(USB_VOID)
{
    g_otg_extern_charge.sys_cmd = INVALID_CMD;
    pr_err("reset sys_cmd = %x\n", g_otg_extern_charge.sys_cmd);
}

static USB_INT usb_otg_ec_is_repeat_sys_cmd(USB_INT cmd)
{
    USB_INT  ret = 0;

    pr_err("check sys_cmd = %x, cmd=%u\n", g_otg_extern_charge.sys_cmd, cmd);
    
    if (cmd < 0)
    {
        DBG_E(MBB_OTG_CHARGER ," illegal cmd=%d < 0 \n", cmd);
    }
    else
    {
        ret = (g_otg_extern_charge.sys_cmd == cmd);
        g_otg_extern_charge.sys_cmd = cmd;
    }

    pr_err("check sys_cmd = %x, ret=%u\n", g_otg_extern_charge.sys_cmd, ret);
    
    return ret;   
}

static USB_VOID usb_otg_ec_process_remove_cmd(USB_VOID)
{
    USB_INT charger_type = USB_CHARGER_TYPE_INVALID;

    DBG_T(MBB_OTG_CHARGER, "is_host_mode=%d\n", g_otg_extern_charge.is_host_mode);
    
    if (g_otg_extern_charge.is_host_mode)
    {
	    /*通知关闭OTG 线程*/
        blocking_notifier_call_chain(&usb_balong_notifier_list,
            USB_BALONG_PERIP_REMOVE, (USB_VOID *)&charger_type);
            
        usb_balong_exit();
        
        if (power_off_dwc3_usb())
        {
            DBG_E(MBB_OTG_CHARGER , "fail to disable the dwc3 usb regulator!\n");
        }
    }
}

static USB_VOID usb_otg_switch_to_host_mode(USB_VOID)
{
    DBG_T(MBB_OTG_CHARGER, "enter\n");
    
    /*初始化 USB 驱动，进入  OTG  host mode*/
    if (power_on_dwc3_usb())
    {
        DBG_E(MBB_OTG_CHARGER, "---power on dwc3 usb failed!---\n");
        return;
    }
    
    usb_balong_init();
    g_otg_extern_charge.is_host_mode = 1;
    
    usb_otg_ec_set_gpio_state(EC_GPIO_OTG_CHARGE);
}

static USB_VOID usb_otg_ec_id_remove(USB_VOID)
{
    DBG_T(MBB_OTG_CHARGER, "enter\n");

    /*断开D+,D-,拉高 phy ID */
    usb_otg_ec_set_gpio_state(EC_GPIO_RESET);

    usb_otg_ec_process_remove_cmd();
    
    g_otg_extern_charge.is_host_mode = 0;
}

static USB_VOID usb_otg_ec_status_clean(USB_VOID)
{
    DBG_T(MBB_OTG_CHARGER, "enter\n");
    
    usb_otg_ec_id_remove();

    usb_chg_set_work_mode(USB_CHG_NON);
    
    usb_otg_ec_set_lpm_lock(0);

    usb_otg_ec_reset_gpio_value();
    usb_otg_ec_reset_sys_cmd();
}

static USB_INT usb_otg_ec_sys_notify_handler
    (struct notifier_block* self, USB_ULONG action, USB_PVOID dev)
{
    USB_BOOL is_ec_mode = usb_chg_check_current_mode(USB_CHG_EXTERN);
    USB_INT  is_repeat_cmd = usb_otg_ec_is_repeat_sys_cmd(action);
    
    DBG_T(MBB_OTG_CHARGER, "ex-chg=%d, cmd=%u\n", is_ec_mode, action);

    /*判断充电模式，是否可执行对外充电*/
    if (!is_ec_mode)
    {
        DBG_E(MBB_OTG_CHARGER, "charge mode is mutually exclusive \n");
        return;
    }

    if (is_repeat_cmd)
    {
        DBG_E(MBB_OTG_CHARGER, "repeated action = %ld ,do nothing.\n", action);
        return MBB_USB_ERROR;
    }

    switch (action)
    {
        /*短接 D+ D- , 进行 1 A 充电*/
        case USB_OTG_CONNECT_DP_DM:
            usb_otg_ec_set_gpio_state(EC_GPIO_1A_CHARGE);
            break;
            
        /* 断开D+ D-连接以后，拉低 ID 线 进入Host 模式，进行otg充电 */
        case USB_OTG_DISCONNECT_DP_DM:
            usb_otg_switch_to_host_mode();
            break;
            
        /*对外充电线，拔出*/
        case USB_OTG_ID_PULL_OUT:
            usb_otg_ec_id_remove();
            break;
            
        /*进入假关机处理流程*/
        case USB_OTG_DISABLE_ID_IRQ:
            /*模拟真关机流程，使假关机恢复到初始状态 */
            usb_otg_ec_status_clean();
            break;
            
        /*退出假关机处理流程*/
        case USB_OTG_ENABLE_ID_IRQ:
            /* 获取当前ID GPIO 状态，如果是低ID 在位上报ID插入事件,重新进入对外充电流程 */
            usb_otg_ec_fresh_gpio_status();
            break;
            
        default:
            break;
    }
  
    return 0;
}

static USB_VOID usb_otg_ec_id_notify_handler(USB_VOID)
{
    usb_adp_ctx_t* ctx = usb_get_adp_ctx();
    USB_INT     fast_on_off = usb_fast_on_off_stat();
    USB_BOOL    is_ec_mode = usb_chg_check_current_mode(USB_CHG_EXTERN);
    USB_INT     id_insert = usb_otg_ec_get_id_insert_state();    

    static USB_INT first_work = 1;

    DBG_T(MBB_OTG_CHARGER, "fast_off=%d, is_ec=%d, insert=%d\n", 
          fast_on_off, is_ec_mode, id_insert);
                            
    if (NULL == ctx)
    {
        DBG_E(MBB_OTG_CHARGER, "ctx NULL\n");
        return;
    }
    
    /*判断充电模式，是否可执行对外充电*/
    if (!is_ec_mode)
    {
        DBG_E(MBB_OTG_CHARGER, "not extern charge mode\n");
        return;
    }
    
    /*判断是否为假关机，如果是，单板可休眠*/
    if (fast_on_off)
    {
        DBG_E(MBB_OTG_CHARGER, "in fast off mode\n");
        usb_otg_ec_set_lpm_lock(0);
        return;
    }

    /*判断当前上报状态与之前相同，不上报消息，返回*/
    if (usb_otg_ec_is_repeat_gpio_value())
    {
        DBG_E(MBB_OTG_CHARGER, "same val\n");
        return;    
    }

    /*第一次上报，直接置充电类型，防止充电没起*/
    if (ctx->stm_set_chg_type_cb && first_work && id_insert)
    {
        DBG_I(MBB_OTG_CHARGER, "-----first notify exchg----\n");
        ctx->stm_set_chg_type_cb(CHG_EXGCHG_CHGR);
        first_work = 0;
    }
    
    if (id_insert)  /*对外充电线插入*/
    {   
        usb_otg_ec_set_lpm_lock(1);
        
        usb_chg_set_work_mode(USB_CHG_EXTERN); /*设置当前充电工作模式为对外充电*/
        usb_otg_ec_reset_sys_cmd();
        
        ctx->battery_notifier_call_chain_cb(1, CHG_EXGCHG_CHGR);
        
        msleep(OTG_DETECT_OVERTIME);
    }
    else    /*对外充电线拔出*/
    {
        ctx->battery_notifier_call_chain_cb(0, CHG_EXGCHG_CHGR);
        
        usb_notify_event(USB_OTG_ID_PULL_OUT, NULL);
        
        msleep(OTG_DETECT_OVERTIME);
        
        usb_chg_set_work_mode(USB_CHG_NON); /*退出对外充电，清空充电工作模式*/
        usb_otg_ec_reset_sys_cmd();
        
        usb_otg_ec_set_lpm_lock(0);
    }
}

static USB_VOID usb_otg_ec_init_sys_notify(USB_VOID)
{
    usb_otg_ec_reset_sys_cmd();
    
    g_otg_extern_charge.is_host_mode = 0;
    
    g_otg_extern_charge.otg_ec_notifier_block.notifier_call = usb_otg_ec_sys_notify_handler;
    usb_register_otg_notify(&g_otg_extern_charge.otg_ec_notifier_block);
}

static USB_VOID usb_otg_ec_uninit_sys_notify(USB_VOID)
{
    usb_unregister_otg_notify(&g_otg_extern_charge.otg_ec_notifier_block);
}


/************************** 外部函数定义 **********************************/

USB_VOID usb_otg_extern_charge_init(USB_VOID)
{
    DBG_T(MBB_OTG_CHARGER, "enter\n");
    
    usb_otg_ec_init_gpio();

    usb_otg_ec_init_lpm();
    
    usb_otg_ec_init_sys_notify();
}

USB_VOID usb_otg_extern_charge_remove(USB_VOID)
{
    DBG_T(MBB_OTG_CHARGER, "enter\n");
    
    usb_otg_ec_uninit_gpio();

    usb_otg_ec_uninit_lpm();
    
    usb_otg_ec_uninit_sys_notify();
}

