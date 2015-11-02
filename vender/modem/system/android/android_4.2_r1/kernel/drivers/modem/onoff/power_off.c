 /*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
 * foss@huawei.com
 *
 * If distributed as part of the Linux kernel, the following license terms
 * apply:
 *
 * * This program is free software; you can redistribute it and/or modify
 * * it under the terms of the GNU General Public License version 2 and 
 * * only version 2 as published by the Free Software Foundation.
 * *
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * * GNU General Public License for more details.
 * *
 * * You should have received a copy of the GNU General Public License
 * * along with this program; if not, write to the Free Software
 * * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Otherwise, the following license terms apply:
 *
 * * Redistribution and use in source and binary forms, with or without
 * * modification, are permitted provided that the following conditions
 * * are met:
 * * 1) Redistributions of source code must retain the above copyright
 * *    notice, this list of conditions and the following disclaimer.
 * * 2) Redistributions in binary form must reproduce the above copyright
 * *    notice, this list of conditions and the following disclaimer in the
 * *    documentation and/or other materials provided with the distribution.
 * * 3) Neither the name of Huawei nor the names of its contributors may 
 * *    be used to endorse or promote products derived from this software 
 * *    without specific prior written permission.
 * 
 * * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */
/*lint --e{537} */
#include <hi_onoff.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <product_config.h>
#include <osl_list.h>
#include <osl_types.h>
#include <osl_spinlock.h>
#include <drv_chg.h>
#include <drv_onoff.h>
#include <bsp_pmu.h>
#include <hi_syssc_interface.h>
#include <power_com.h>
#include "power_inner.h"
#include "power_exchange.h"

#if(MBB_REB == FEATURE_ON)
#include "../../../reb_V100R001/reb_os/reb_mod/reb_func.h"
#endif

#ifdef FEATURE_E5_ONOFF
#include <bsp_sram.h>
#endif
#include <bsp_reset.h>

#if (MBB_SOFT_POWER_OFF == FEATURE_ON)    
#include <bsp_icc.h>
#include <hi_gpio.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#endif /* MBB_SOFT_POWER_OFF == FEATURE_ON */

#if ( FEATURE_ON == MBB_MLOG )
#include <linux/mlog_lib.h>
#endif

#if (MBB_OLED == FEATURE_ON)
extern void MainOLED_DispENoff(void);
#endif
static LIST_HEAD(list_callback);
static DEFINE_SPINLOCK(list_spinlock); //lint !e43 !e64 !e120

#ifndef HI_ONOFF_PHONE

#ifdef FEATURE_E5_ONOFF
LOCAL_1 bool is_power_off_charge_in( void );
#endif

LOCAL_1 void power_off_set_flag_and_reboot( power_off_reboot_flag enRebootFlag );

#if (MBB_CHG_EXTCHG == FEATURE_ON)
extern unsigned char chg_stm_get_extchg_online_st(void);
extern void chg_set_extchg_chg_enable(unsigned char);
#endif
#if (MBB_CHARGE == FEATURE_ON)

extern DRV_START_MODE chg_get_start_mode();/*lint !e10*/

#endif

power_off_ctrl_s power_off_ctrl = {DRV_SHUTDOWN_BUTT, {0,0,0,0,0,0,0,0,0}};


/* stub */
#if (MBB_CHARGE == FEATURE_OFF)
BATT_LEVEL_ENUM chg_get_batt_level(void)
{
    return BATT_LEVEL_4;
}
#endif


#if defined(FEATURE_E5_ONOFF) || (MBB_SOFT_POWER_OFF == FEATURE_ON) 
void power_off_clear_dump(void)
{
    unsigned long flags;
    struct bsp_onoff_callback *hook = NULL;

    spin_lock_irqsave(&list_spinlock, flags);

    list_for_each_entry(hook, &list_callback, node)
    if ((hook) && (hook->fn))
    {
        hook->fn();
    }
    spin_unlock_irqrestore(&list_spinlock, flags);
}
#endif

#if (MBB_SOFT_POWER_OFF == FEATURE_ON)    
/*****************************************************************************
 函 数 名  : void power_off_close_bb(void)
 功能描述  : 关闭基带电源
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 无
*****************************************************************************/
void power_off_close_bb(void)
{
    int volt_id = PMIC_HI6559_BUCK0;
    int dr_id = PMIC_HI6559_DR01;
    s32 ret = 0;
    
    for(volt_id = PMIC_HI6559_VOLT_MIN; volt_id <= PMIC_HI6559_VOLT_MAX; volt_id++)
    {
        if(    (PMIC_HI6559_BUCK3 != volt_id)
            && (PMIC_HI6559_BUCK4 != volt_id)
            && (PMIC_HI6559_BUCK6 != volt_id)
            && (PMIC_HI6559_LDO03 != volt_id)
          //&& (PMIC_HI6559_LDO06 != volt_id) tcxo hi6361
          //&& (PMIC_HI6559_LDO07 != volt_id) close MMC
            && (PMIC_HI6559_LDO08 != volt_id) //peri_EN
            && (PMIC_HI6559_LDO12 != volt_id) //close usb
          //&& (PMIC_HI6559_LDO22 != volt_id) close hsic phy
            && (PMIC_HI6559_LVS09 != volt_id))
        {
            ret = bsp_hi6559_volt_disable(volt_id);
            if (BSP_PMU_OK != ret)
            {
                printk(KERN_ALERT "power_off_close_bb disable %d failed, ret=%d \n", 
                        volt_id, ret);
            }
        }
    }
    
    /* 
    *  to make sure, in case applications failed to turn off all dr led
    *  in cpe b310, dr01~dr04 is for signal led 1,2,3  
    *  and eth port led, dr05 is not used.
    */
    for(dr_id = PMIC_HI6559_DR01; dr_id < PMIC_HI6559_DR05; dr_id++)
    {
        /*lint -save -e64 -e506*/
        ret = bsp_hi6559_dr_disable(dr_id);
        if (BSP_PMU_OK != ret)
        {
            printk(KERN_ALERT "power_off_close_bb disable dr %d failed, ret=%d \n", 
                dr_id, ret);
        }
        /*lint -restore +e64 +e506*/
    }
    
    printk(KERN_ALERT "power_off_close_bb ok\n");

}

void power_off_close_peripheral(void)
{
#define GPIO_PERI_POWER GPIO_1_16
#define GPIO_MODE_LED_R GPIO_1_23
#define GPIO_MODE_LED_G GPIO_2_10
#define GPIO_MODE_LED_B GPIO_2_11

    if(gpio_request(GPIO_PERI_POWER, "GPIO_PERI_PWR"))
    {
        printk(KERN_ALERT "request interface power gpio failed.\n");
        return;
    }

    gpio_direction_output(GPIO_PERI_POWER, 0);
    gpio_direction_output(GPIO_MODE_LED_R, 0);
    gpio_direction_output(GPIO_MODE_LED_G, 0);
    gpio_direction_output(GPIO_MODE_LED_B, 0);
}

void power_off_notify_mcore(void)
{
#define ICC_CHANNEL_NUM_MASK        16
#define ICC_CHANNEL_ACORE_NOTIFY_MCORE \
        ((ICC_CHN_MCORE_ACORE << ICC_CHANNEL_NUM_MASK) | MCORE_ACORE_FUNC_POWEROFF_NOTIFY)

    int ret = 0;
    u32 data = 0;
    u32 data_len = sizeof(data);

    ret = bsp_icc_send(ICC_CPU_MCU, ICC_CHANNEL_ACORE_NOTIFY_MCORE, &data, data_len);
    if(data_len != (u32)ret)
    {
        printk(KERN_ALERT "--->power_off_notify_mcore send icc error..\n");        
    }

    printk(KERN_ALERT "--->power_off_notify_mcore send icc ok..\n");  
}
#endif /* MBB_SOFT_POWER_OFF == FEATURE_ON */

/*****************************************************************************
 函 数 名  : power_off_down_operation
 功能描述  : pmu hold 系统下电
 输入参数  :
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :
*****************************************************************************/
LOCAL_1 void power_off_down_operation( void )
{
    pr_dbg( "system power down!\r\n" );
#if (FEATURE_ON == MBB_COMMON)
    /* 清除重启命令字 */
    power_reboot_cmd_set(POWER_REBOOT_CMD_BUTT);
#endif

#ifdef FEATURE_E5_ONOFF
    *(u32*)SRAM_MBB_PD_CHARGE_ADDR = SRAM_MBB_PD_CHARGE_OFF;
#endif

#if defined(FEATURE_E5_ONOFF) || (MBB_SOFT_POWER_OFF == FEATURE_ON) 
    power_off_clear_dump();
	power_on_wdt_cnt_set();
#endif 

#ifdef FEATURE_E5_ONOFF
    if(is_power_off_charge_in())    
    {         
        power_off_set_flag_and_reboot( POWER_OFF_REASON_BY_CHARGE );
    }    
    else    
    {    
#if (MBB_CHG_EXTCHG == FEATURE_ON)
        /* 根据硬件建议，在下电前若对外充电线在位则需要停止对外充电 */
        if(TRUE == chg_stm_get_extchg_online_st())
        {
            pr_dbg( "[ ON OFF ] Stop ext charging!\r\n" );
            chg_set_extchg_chg_enable(FALSE);
        }	
#endif
#if (MBB_OLED == FEATURE_ON)
        MainOLED_DispENoff();
#endif
        /* shutdown the system. */        
        hi_syssc_pmu_hold_down();
    }
#elif (MBB_SOFT_POWER_OFF == FEATURE_ON)    
    power_off_close_bb();
    power_off_close_peripheral();
    power_off_notify_mcore();
#else
    hi_syssc_pmu_hold_down();
#endif

    for(;;) ;
}

/*****************************************************************************
 函 数 名  : power_off_reboot_operation
 功能描述  : pmu rst 系统重启
 输入参数  :
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :
*****************************************************************************/
LOCAL_1 void power_off_reboot_operation( void )
{
#if (FEATURE_ON == MBB_COMMON)
    /* 设置重启命令字为热启动 */
    power_reboot_cmd_set(POWER_REBOOT_CMD_WARMRESET);
#endif

/* p531 FPGA复位需要同时复位F1/BBP/RF */
#ifdef BSP_CONFIG_P531_FPGA
extern void p531_fpga_reset(void);
	p531_fpga_reset();
#endif

    pr_dbg( "system power reboot!\r\n" );


#ifdef FEATURE_E5_ONOFF
    *(u32*)SRAM_MBB_PD_CHARGE_ADDR = SRAM_MBB_PD_CHARGE_OFF;
    hi_syssc_wdt_reboot();
#else
	hi_syssc_pmu_reboot();
#endif

    for(;;) ;
}

/*****************************************************************************
 函 数 名  : is_power_key_pressed
 功能描述  : 关机时按键是否按下
 输入参数  :
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :
*****************************************************************************/
LOCAL_1 bool is_power_key_pressed( void )
{
    return bsp_pmu_key_state_get();
}

/*****************************************************************************
 函 数 名  : is_power_off_charge_in
 功能描述  : 关机时充电器是否插入
 输入参数  :
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :
*****************************************************************************/
LOCAL_1 bool is_power_off_charge_in( void )
{
#if (FEATURE_ON == MBB_COMMON)
    bool pmu_usb_state = false;

	pmu_usb_state = bsp_pmu_usb_state_get();
    printk(KERN_INFO "[ ON OFF] pmu_usb_state is %d.\n", pmu_usb_state);
#if (MBB_CHG_EXTCHG == FEATURE_ON)
    /* 对外充电线不属于充电器，要返回false，避免单板在关机时重启 */
	if(TRUE == chg_stm_get_extchg_online_st())
	{
        printk(KERN_INFO "[ ON OFF] extchg state is true.\n");
	    pmu_usb_state = false;
	}
#endif

    return pmu_usb_state;
#else
    return bsp_pmu_usb_state_get();
#endif
}
/*****************************************************************************
 函 数 名  : power_off_operation
 功能描述  : 执行关机操作
 输入参数  :
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :
*****************************************************************************/
LOCAL_1 void power_off_operation( void )
{
    /* 按键不松开情况下，不允许关机 */
    while( is_power_key_pressed() )
    {
        /* avoid the watchdog reset while polling the power key */
        /* TO DO*/
    }

    pr_dbg("\r\n power_off_operation : the power key is release\r\n" );

    /* shutdown the system. */
    power_off_down_operation();
}

/*****************************************************************************
 函 数 名  : power_off_set_flag_and_reboot
 功能描述  : 执行关机操作
 输入参数  :
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :
*****************************************************************************/
LOCAL_1 void power_off_set_flag_and_reboot( power_off_reboot_flag enRebootFlag )
{
    /*lint --e{*} */
    unsigned long irq_flags = 0;
    struct bsp_onoff_callback *hook = NULL;

    spin_lock_irqsave(&list_spinlock, irq_flags);
    /*coverity[var_deref_op] */
    list_for_each_entry(hook, &list_callback, node)
        /*coverity[var_compare_op] */
        if ((hook) && (hook->fn))
            hook->fn();
    spin_unlock_irqrestore(&list_spinlock, irq_flags);

    power_on_reboot_flag_set( enRebootFlag );
    power_off_reboot_operation();
}

/*****************************************************************************
 函 数 名  : power_off_battery_error_handler
 功能描述  : 电池错误关机处理
 输入参数  :
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :
*****************************************************************************/
LOCAL_1  void power_off_battery_error_handler( void )
{
    if( is_power_off_charge_in() )
    {
        power_off_set_flag_and_reboot( POWER_OFF_REASON_BAD_BATTERY );
    }
    else
    {
        power_off_operation();
    }
}

/*****************************************************************************
 函 数 名  : power_off_battery_low_handler
 功能描述  : 电池低电处理
 输入参数  :
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :
*****************************************************************************/
LOCAL_1 void power_off_battery_low_handler( void )
{
    if( is_power_off_charge_in() )
    {
        power_off_set_flag_and_reboot( POWER_OFF_REASON_LOW_BATTERY );
    }
    else
    {
        power_off_operation();
    }
}

/*****************************************************************************
 函 数 名  : power_off_temp_protect_handler
 功能描述  : 温度保护处理
 输入参数  :
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :
*****************************************************************************/
LOCAL_1 void power_off_temp_protect_handler( void )
{
    if( is_power_off_charge_in() )
    {
        /* The charge is in, can't power off, restart and show the warning */
        power_off_set_flag_and_reboot(POWER_OFF_REASON_OVER_TEMP);
    }
    else
    {
        /* No warning message, power off directly */
        power_off_operation();
    }
}

/*****************************************************************************
 函 数 名  : power_off_power_key_handler
 功能描述  : 按键关机处理, 在关机充电模式下按键，进入正常工作模式
 输入参数  :
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :
*****************************************************************************/
LOCAL_1 void power_off_power_key_handler( void )
{
    if( DRV_START_MODE_CHARGING == bsp_start_mode_get() )
    {
        /* reboot and enter normal mode; */
        power_off_set_flag_and_reboot( POWER_OFF_REASON_NORMAL );
    }
    else /* normal mode */
    {
        /* if charge is in, will power on again and enter charge mode. */
        power_off_operation();
    }
}

/*****************************************************************************
 函 数 名  : power_off_update_handler
 功能描述  : 升级关机处理，软复位进入recovery模式
 输入参数  :
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :
*****************************************************************************/
LOCAL_1 void power_off_update_handler( void )
{
    /* reboot and enter update mode */
    power_off_set_flag_and_reboot( POWER_OFF_REASON_UPDATE );
}

/*****************************************************************************
 函 数 名  : power_off_charge_remove_handler
 功能描述  : 充电器移除处理
 输入参数  :
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :
*****************************************************************************/
LOCAL_1 void power_off_charge_remove_handler( void )
{
    if( DRV_START_MODE_CHARGING == bsp_start_mode_get() )
    {

#if (MBB_CHARGE == FEATURE_OFF)
        power_off_set_flag_and_reboot( POWER_OFF_REASON_RM_CHARGE );
#endif

        power_off_operation();  /* 关机充电时，拔除充电器，单板直接下电 */
    }
    else
    {
        /* Normal mode, nothing need to do. */
    }
}

/*****************************************************************************
 函 数 名  : power_off_reset_handler
 功能描述  : 系统复位操作，直接复位，不用检查charger状态
 输入参数  :
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :
*****************************************************************************/
void power_off_reset_handler( void )
{
    /* Needn't check charge status */
    power_off_set_flag_and_reboot( POWER_OFF_REASON_NORMAL );
}

/*****************************************************************************
 函 数 名  : power_off_reset_handler
 功能描述  : 系统复位操作，直接复位，不用检查charger状态
 输入参数  :
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :
*****************************************************************************/
LOCAL_1 void power_off_excecute( DRV_SHUTDOWN_REASON_ENUM enShutdownReason )
{
	unsigned long flags = 0;

#if(MBB_REB == FEATURE_ON)
    reb_file_sync();
#endif


	local_irq_save(flags);

    switch( enShutdownReason )
    {
    case DRV_SHUTDOWN_RESET:
        power_off_set_flag_and_reboot( POWER_OFF_REASON_NORMAL );
        break;
    case DRV_SHUTDOWN_BATTERY_ERROR:
        power_off_battery_error_handler();
        break;
    case DRV_SHUTDOWN_LOW_BATTERY:
        power_off_battery_low_handler();
        break;
    case DRV_SHUTDOWN_TEMPERATURE_PROTECT:
        power_off_temp_protect_handler();
        break;
    case DRV_SHUTDOWN_POWER_KEY:
        power_off_power_key_handler();
        break;
    case DRV_SHUTDOWN_UPDATE:
        power_off_update_handler();
        break;
    case DRV_SHUTDOWN_CHARGE_REMOVE:
        power_off_charge_remove_handler( );
        break;
    default:
        power_off_operation();
        break;
    }

    /* Woulnd't reach here ! */
    local_irq_restore( flags );
}

/*****************************************************************************
 函 数 名  : power_on_mode_get
 功能描述  : 获取开机模式
 输入参数  :
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :
*****************************************************************************/
unsigned int power_off_reason_get( void )
{
    power_off_reboot_flag reboot_reason = (power_off_reboot_flag)(power_off_ctrl.reason) ;
    DRV_SHUTDOWN_REASON_ENUM rb = DRV_SHUTDOWN_BUTT;

    switch(reboot_reason)
    {
    case POWER_OFF_REASON_NORMAL:
        rb = DRV_SHUTDOWN_POWER_KEY;
        break;
    case POWER_OFF_REASON_BAD_BATTERY:
        rb = DRV_SHUTDOWN_BATTERY_ERROR;
        break;
    case POWER_OFF_REASON_LOW_BATTERY:
        rb = DRV_SHUTDOWN_LOW_BATTERY;
        break;
    case POWER_OFF_REASON_OVER_TEMP:
        rb = DRV_SHUTDOWN_TEMPERATURE_PROTECT;
        break;
    case POWER_OFF_REASON_RM_CHARGE:
        rb = DRV_SHUTDOWN_CHARGE_REMOVE;
        break;
    case POWER_OFF_REASON_UPDATE:
        rb = DRV_SHUTDOWN_UPDATE;
        break;
    case POWER_OFF_REASON_RESET:
        rb = DRV_SHUTDOWN_RESET;
        break;
    default:
        rb = DRV_SHUTDOWN_BUTT;
        break;
    }

    return rb;

}

/******************************************************************************
*  Function:  power_off_log_save
*  Description: save the power off log( reason and battery voltage ).
*  Input:
*         None
*  Output:
*         None
*  Return:
*         None
*  Note  :
********************************************************************************/
LOCAL_1 int power_off_log_save( void )
{
    long pos;
    unsigned int pf;
    mm_segment_t old_fs;
    struct rtc_time tm;
    struct timespec ts;
    int     rt;
    char    buf[128];
#if (MBB_CHARGE == FEATURE_ON)
    char    *reboot_reason[] = {"LOWBATTERY", "BAD BATTERY", "NORMAL", "OVERTEMP", \
                                    "LOWTEMP", "REBOOT", "RM_CHARGER", "UPDATE", "INVALID"};
#else
    char    *reboot_reason[] = {"NORMAL", "BAD BATTERY", "LOWBATTERY", "OVERTEMP", \
                                    "RM_CHARGER", "UPDATE", "REBOOT", "INVALID"};
#endif
    BATT_LEVEL_ENUM                 battery_level = chg_get_batt_level();
    DRV_SHUTDOWN_REASON_ENUM        rb;

    getnstimeofday(&ts);

    rtc_time_to_tm((unsigned long)ts.tv_sec, &tm);

    power_off_ctrl.time = tm;
    rb = power_off_ctrl.reason;

    pr_dbg("%4d-%02d-%02d %02d:%02d:%02d\n",tm.tm_year, tm.tm_mon, \
        tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

#if ( FEATURE_ON == MBB_MLOG )
    if( DRV_SHUTDOWN_RESET == power_off_ctrl.reason )
    {
       mlog_print(MLOG_POWER, mlog_lv_info, "system will go to reboot\n" );
       mlog_set_statis_info("reboot_times",1); //normal reboot statistic   
    }
    else
    {
        mlog_print(MLOG_POWER, mlog_lv_info, "SHUTDOWN due to %s\n" , reboot_reason[rb]);
        mlog_set_statis_info("off_times",1);    
    }  
    sys_shutdown_mog_report();

#endif


    /* 记录关机信息(时间、次数、关机原因) */
    snprintf(buf, sizeof(buf) - 1, "system close reason(E5): %s, current battery voltage: %d, current time: %4d-%02d-%02d %02d:%02d:%02d\n", \
        reboot_reason[rb], battery_level, tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

    old_fs = get_fs(); //lint !e63

    set_fs(KERNEL_DS);

    /*coverity[check_return] */
    pf = (unsigned int)sys_open(EXCH_RESET_LOG_PATH, O_RDWR | O_CREAT, 0666);
    /*coverity[unchecked_value] */
    if(IS_ERR((const void*)pf))
    {
        pr_dbg( "error occured happened when open file %s, exiting.\n", EXCH_RESET_LOG_PATH);
        return (int)pf;
    }

    /*coverity[unchecked_value] */
    pos = sys_lseek(pf, 0, SEEK_END);
    if(pos > EXCH_ONOFF_LOG_MAX){
        /* 文件超过 16k,删除重新打开 */
        sys_rmdir(EXCH_RESET_LOG_PATH);
        /*coverity[check_return] */
        pf = (unsigned int)sys_open(EXCH_RESET_LOG_PATH, O_RDWR | O_CREAT, 0666);
        /*coverity[unchecked_value] */
        if(IS_ERR((const void*)pf))
        {
            pr_dbg( "error occured happened when open file %s, exiting.\n", EXCH_RESET_LOG_PATH);
            return (int)pf;
        }
    }
    else{
        /*coverity[unchecked_value] */
        sys_lseek(pf, pos, SEEK_SET);
    }

    /*coverity[unchecked_value] */
    rt = sys_write(pf, (const char*)buf, strlen(buf));
    if(rt<0)
    {
        pr_dbg("error occured happened when write file %s, exiting.\n", EXCH_RESET_LOG_PATH);
        /*coverity[unchecked_value] */
        sys_close( pf );
        set_fs(old_fs);
        return rt;
    }

    pr_dbg(KERN_DEBUG "power off log save.");

    /*coverity[unchecked_value] */
    sys_close( pf );
    set_fs(old_fs);

    return rt;
}

/******************************************************************************
*  Function:  power_off_timeout_isr
*  Description: 如果应用没有一定时间内关机，底层直接关机
*  Input:
*         None
*  Output:
*         None
*  Return:
*         None
*  Note  :
********************************************************************************/
static int power_off_timeout_isr( void )
{
    pr_dbg("power_off_timeout_isr, reason :%d\n ",power_off_ctrl.reason);

    (void)bsp_softtimer_free(&power_off_ctrl.off_timer);

    power_off_excecute(power_off_ctrl.reason);

    return 0;
}

/******************************************************************************
*  Function:  drv_power_off
*  Description: 直接关机接口，不记录关机事件
*  Input:
*         None
*  Output:
*         None
*  Return:
*         None
*  Note  :
********************************************************************************/
LOCAL_1 void drv_power_off( void )
{
    /*
       If the shutdown reason has been set, do it.
       If not, it is invalid one and will shutdown
    */
    power_off_excecute(power_off_ctrl.reason);
}

#endif

void bsp_reboot_callback_register(void (*hook)(void))
{
    /*lint --e{*} */
    unsigned long flags = 0;

    struct bsp_onoff_callback *callback =
        (struct bsp_onoff_callback *)kmalloc(sizeof(struct bsp_onoff_callback), GFP_KERNEL);
    if (NULL == callback)
    {
        pr_dbg("fail to malloc struct bsp_onoff_callback \n");
        return;
    }

    callback->fn = hook;

    spin_lock_irqsave(&list_spinlock, flags);
    list_add(&callback->node, &list_callback);
    spin_unlock_irqrestore(&list_spinlock, flags);
    /*coverity[leaked_storage] */
}

/******************************************************************************
*  Function:  drv_shut_down
*  Description: start the power off process.
*  Input:
*         eReason : shutdown reason.
*  Output:
*         None
*  Return:
*         None
*  Note  : 底层调用关机接口，启用定时器，上报事件给应用。
*          超时时间内应用不关机，由底层强制关机。
********************************************************************************/
void drv_shut_down( DRV_SHUTDOWN_REASON_ENUM enReason )
{
#ifndef HI_ONOFF_PHONE
#if (FEATURE_ON == MBB_COMMON)
    static unsigned long drv_shut_down_set_flag = false;
#endif

	power_off_ctrl.off_timer.func = (softtimer_func)power_off_timeout_isr;
    if( DRV_SHUTDOWN_CHARGE_REMOVE == enReason )
    {
        if( DRV_START_MODE_NORMAL == bsp_start_mode_get() )
        {
            /* In normal mode, don't handle the charge remove message */
            return ;
        }
    }
#if (MBB_CHARGE == FEATURE_ON)

    /*关机充电的情况下，温保过温之后不需要执行关机动作*/
    if(DRV_START_MODE_CHARGING == chg_get_start_mode() && DRV_SHUTDOWN_TEMPERATURE_PROTECT == enReason)
    {
        pr_dbg("NOW is poweroff state no need to shutdown.\n");
        return ;
    }

#endif

    if( DRV_SHUTDOWN_BUTT == enReason )
    {
        return ; /* the shutdown been handled, ignore this call */
    }

#if(FEATURE_ON == MBB_COMMON)
    if( DRV_SHUTDOWN_RESET == enReason )
    {
        pr_dbg("system will go to reboot!! \n");
        bsp_drv_power_reboot();
        return;
    }
#endif

    /* save the reason and drv_power_off need it */
#if (FEATURE_ON == MBB_COMMON)
    if (false == drv_shut_down_set_flag)
    {
#endif
        power_off_ctrl.reason = enReason;

        power_off_ctrl.off_timer.para = (u32)0;
        power_off_ctrl.off_timer.timeout = POWER_OFF_MONOTER_TIMEROUT;
        power_off_ctrl.off_timer.wake_type = SOFTTIMER_WAKE;

        if (bsp_softtimer_create(&power_off_ctrl.off_timer))
        {
            pr_dbg("create softtimer failed \n");
        }

        bsp_softtimer_add(&power_off_ctrl.off_timer);

        /* 针对由底层上报关机事件，由应用执行关机的场景 */
        /* TO DO */
#if (FEATURE_ON == MBB_COMMON)
        drv_shut_down_set_flag = true;
#endif
        power_off_log_save();
#if (FEATURE_ON == MBB_COMMON)
    }
    else
    {
        return ;
    }
#endif
#else
    if (DRV_SHUTDOWN_RESET == enReason)
    {
        pr_dbg("drv_shut_down is called, modem reset...");

        bsp_modem_reset();
    }
#endif
    /* Notify the monitor task */
}

/******************************************************************************
*  Function:  bsp_drv_power_off
*  Description: same as drv_power_off, the public API
*  Input:
*         None
*  Output:
*         None
*  Return:
*         None
*  Note  : 应用调用关机时调用，at+cfun=8
********************************************************************************/
void bsp_drv_power_off( void )
{
#ifndef HI_ONOFF_PHONE
    if(power_off_ctrl.reason == DRV_SHUTDOWN_BUTT)
    {
        /*Set shutdown reason to Power_key.*/
        power_off_ctrl.reason = DRV_SHUTDOWN_POWER_KEY;

        pr_dbg( "power off process triggered by a-cpu with power_key\n");
#if( FEATURE_OFF == MBB_COMMON)
        power_off_log_save();
#endif
    }
#if( FEATURE_ON == MBB_COMMON)
    power_off_log_save();
#endif
    power_on_wdt_cnt_set();
    drv_power_off();
#else
    printk(KERN_ERR"we will do nothing...\n");
#endif
}

/******************************************************************************
*  Function:  bsp_drv_power_reboot
*  Description: same as drv_power_off, the public API
*  Input:
*         None
*  Output:
*         None
*  Return:
*         None
*  Note  : 应用调用重启时调用，at+cfun=6
********************************************************************************/
void bsp_drv_power_reboot( void )
{
#ifndef HI_ONOFF_PHONE
    /*Set shutdown reason to reset.*/
    power_off_ctrl.reason = DRV_SHUTDOWN_RESET;

    pr_dbg( "power reboot process triggered \n");
    power_off_log_save();


#if(MBB_REB == FEATURE_ON)
    reb_file_sync();
#endif

    power_on_wdt_cnt_set();


    power_off_set_flag_and_reboot((power_off_reboot_flag)(power_off_ctrl.reason));
#else
    printk(KERN_ERR"bsp_drv_power_reboot_direct is called, modem reset...\n");

    bsp_modem_reset();
#endif
}

/******************************************************************************
*  Function:  bsp_drv_power_reboot_direct
*  Description:
*  Input:
*         None
*  Output:
*         None
*  Return:
*         None
*  Note  : 直接重启
********************************************************************************/
void bsp_drv_power_reboot_direct( void )
{
#ifndef HI_ONOFF_PHONE
    power_off_reboot_operation();
#else
    printk(KERN_ERR"bsp_drv_power_reboot_direct is called, modem reset...\n");

    bsp_modem_reset();
#endif
}


#if (MBB_CHARGE == FEATURE_ON)
void bsp_drv_set_power_off_reason(DRV_SHUTDOWN_REASON_ENUM reason)
{
    power_off_ctrl.reason = reason;
}
#endif



/******************************************************************************
*  Function:  balong_power_restart
*  Description: same as bsp_drv_power_reboot, 系统调用时使用
*  Input:
*         None
*  Output:
*         None
*  Return:
*         None
*  Note  :android_reboot(ANDROID_RB_RESTART, 0, 0);
********************************************************************************/
/*lint -save -e958 */
void balong_power_restart(char mode, const char *cmd)
{
    bsp_drv_power_reboot();
}
/*lint -restore */

/******************************************************************************
*  Function:  balong_power_off
*  Description: same as bsp_drv_power_off, 系统调用时使用
*  Input:
*         None
*  Output:
*         None
*  Return:
*         None
*  Note  :android_reboot(ANDROID_RB_POWER_OFF, 0, 0);
********************************************************************************/
void balong_power_off( void )
{
	bsp_drv_power_off();
}

#ifndef HI_ONOFF_PHONE
/******************************************************************************
*  Function:  debug命令
*  Description: 内部调试时使用
*  Input:
*         None
*  Output:
*         None
*  Return:
*         None
*  Note  :
********************************************************************************/
void bsp_power_reboot_boot( void )
{
   power_reboot_cmd_set(POWER_REBOOT_CMD_BOOTLOADER);
   power_off_set_flag_and_reboot(POWER_OFF_REASON_RESET);
}

void bsp_power_reboot_recovery( void )
{
   power_reboot_cmd_set(POWER_REBOOT_CMD_RECOVERY);
   power_off_set_flag_and_reboot(POWER_OFF_REASON_RESET);
}

void bsp_power_reboot_warm( void )
{
   power_reboot_cmd_set(POWER_REBOOT_CMD_WARMRESET);
   power_off_set_flag_and_reboot(POWER_OFF_REASON_RESET);
}
#endif

