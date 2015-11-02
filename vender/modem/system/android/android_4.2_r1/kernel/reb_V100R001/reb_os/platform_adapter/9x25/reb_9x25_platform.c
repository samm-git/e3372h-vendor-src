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
/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "reb_9x25_platform.h"
#include "../../reb_config.h"
#include "../../reb_mod/reb_func.h"
#include "../../os_adapter/reb_os_adapter.h"
#include <mach/huawei_smem.h>
#include <linux/export.h>
/*----------------------------------------------*
 * 外部变量声明                                 *
 *----------------------------------------------*/
/*----------------------------------------------*
 * 变量定义                                     *
 *----------------------------------------------*/
#if (FEATURE_ON == MBB_FAST_ON_OFF)
extern volatile int  g_fast_on_off;
#endif
extern void huawei_kernel_power_off(void);
/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
#ifdef  CONFIG_MBB_KERNEL_NOTIFY
/*定时器通道为2*/
#define  REB_TIMER  2
extern int huawei_notify_kernel_A2M(unsigned int notify_id);
#endif

/*****************************************************************************
 函数名称  : void reb_nv_kernel_read()
 功能描述  : 根据特性宏初始化nv结构的初值
 输入参数  : No
 输出参数  : No
 返 回 值  : No
 修改历史  : 
             1. 2013-12-30 :  00206465 qiaoyichuan created
*****************************************************************************/
int reb_nv_kernel_read(void)
{
/*系统启动过程不对文件系统操作功能使能*/
#if (YES == MBB_REB_WRITE_PROTECT)
    g_kernel_soft_reliable_info.BootNotOperaFileSysEnable = 1;
#else
    g_kernel_soft_reliable_info.BootNotOperaFileSysEnable = 0;
#endif
/*假关机失败进入真关机功能使能*/
#if (YES == MBB_REB_FAST_POWEROFF)
    g_kernel_soft_reliable_info.FastOffFailTrueShutdownEnable = 1;
#else
    g_kernel_soft_reliable_info.FastOffFailTrueShutdownEnable = 0;
#endif
/* Flash病危块扫描功能使能*/
#if (YES == MBB_REB_BADZONE_SCAN)
    g_kernel_soft_reliable_info.BadZoneScanEnable = 1;
#else
    g_kernel_soft_reliable_info.BadZoneScanEnable = 0;
#endif
/* Flash病危块回写功能使能*/
#if (YES == MBB_REB_BADZONE_REWRITE)
    g_kernel_soft_reliable_info.BadZoneReWriteEnable = 1;
#else
    g_kernel_soft_reliable_info.BadZoneReWriteEnable = 0;
#endif
/*分区写越界保护使能*/
#if (YES == MBB_REB_ZONE_WRITE_PROTECT)
    g_kernel_soft_reliable_info.ZoneWriteProtectEnable = 1;
#else
    g_kernel_soft_reliable_info.ZoneWriteProtectEnable = 0;
#endif
    /*上电危险时间设置，单位S*/
    g_kernel_soft_reliable_info.DangerTime = 5;
    return 0;
}

/*****************************************************************************
 函 数 名  :hard_timer_creat(unsigned int time_in_ms, unsigned int input_para )
 功能描述  : 创建能够唤醒操作系统的定时
 输入参数  : none
 输出参数  : none
 返 回 值  : none
*****************************************************************************/
void hard_timer_creat(unsigned int time_in_ms, unsigned int input_para )
{
    huawei_smem_info *smem_data = NULL;
#ifdef  CONFIG_MBB_KERNEL_NOTIFY
    int ret = -1 ;
#endif
    smem_data = (huawei_smem_info *)SMEM_HUAWEI_ALLOC(
                            SMEM_ID_VENDOR0, sizeof(huawei_smem_info));
    /*申请内存失败不启动定时器*/
    if(NULL == smem_data)
    {
        reb_msg(REB_MSG_ERR, "REB_DFT: get smem_data error\n");
        return;
    }
    /*更新共享内存，标记为启动定时器*/
    smem_data->smem_reb_timer = SMEM_REB_TIMER_CREAT;
    /*调用内核接口发给Q6侧启动定时器*/
#ifdef  CONFIG_MBB_KERNEL_NOTIFY
    ret = huawei_notify_kernel_A2M(REB_TIMER);
    if ( ret < 0 )
    {
        reb_msg(REB_MSG_ERR, "%s: REB_DFT: huawei_notify_kernel_A2M(0) fail \n",__func__);
    }
#endif
}

/*****************************************************************************
 函 数 名  : hard_timer_set(unsigned int time_in_ms)
 功能描述  :重新设置定时时间
 输入参数  : time_in_ms: 单位是ms的定时间隔
 输出参数  : none
 返 回 值  : none
*****************************************************************************/
void hard_timer_reset( unsigned int time_in_ms )
{
    huawei_smem_info *smem_data = NULL;
#ifdef  CONFIG_MBB_KERNEL_NOTIFY
    int ret = -1 ;
#endif
    /*申请内存失败不启动定时器*/
    smem_data = (huawei_smem_info *)SMEM_HUAWEI_ALLOC(
                            SMEM_ID_VENDOR0, sizeof(huawei_smem_info));

    if(NULL == smem_data)
    {
        reb_msg(REB_MSG_ERR, "REB_DFT: get smem_data error\n");
        return;
    }

    if ( 0 == time_in_ms )
    {
        reb_msg( REB_MSG_ERR, "REB_DFT: time_in_ms is 0\r\n " );
        return;
    }
    /*根据参数更新共享内存*/
    if (FAST_ON_OFF_FST_CYCLE == time_in_ms)
    {
        smem_data->smem_reb_timer = SMEM_REB_TIMER_SET_FAST;
    }
    else if (FAST_ON_OFF_CYCLE == time_in_ms)
    {
        smem_data->smem_reb_timer = SMEM_REB_TIMER_SET_SLOW;
    }
    else
    {
        return;
    }
    /*调用内核接口发给Q6侧启动定时器*/
#ifdef  CONFIG_MBB_KERNEL_NOTIFY
    ret = huawei_notify_kernel_A2M(REB_TIMER);
    if ( ret < 0 )
    {
        reb_msg(REB_MSG_ERR, "%s: REB_DFT: huawei_notify_kernel_A2M(2) fail \n", __func__);
    }
#endif

    return;
}

/*****************************************************************************
 函 数 名  : hard_timer_delete(void)
 功能描述  : 停止能够唤醒操作系统的定时器
 输入参数  : none
 输出参数  : none
 返 回 值  : none
*****************************************************************************/
void hard_timer_delete(void)
{
    huawei_smem_info *smem_data = NULL;
#ifdef  CONFIG_MBB_KERNEL_NOTIFY
    int ret = -1 ;
#endif
    /*申请内存失败不启动定时器*/
    smem_data = (huawei_smem_info *)SMEM_HUAWEI_ALLOC(
                            SMEM_ID_VENDOR0, sizeof(huawei_smem_info));

    if(NULL == smem_data)
    {
        reb_msg(REB_MSG_ERR ,"REB_DFT: get smem_data error\n");
        return;
    }
    /*更新共享内存标记为停止*/
    smem_data->smem_reb_timer = SMEM_REB_TIMER_DELETE;
    /*调用内核接口发给Q6侧停止定时器*/
#ifdef  CONFIG_MBB_KERNEL_NOTIFY
    ret = huawei_notify_kernel_A2M(REB_TIMER);
    if ( ret < 0 )
    {
        reb_msg(REB_MSG_ERR, "%s: REB_DFT: huawei_notify_kernel_A2M(2) fail \n",__func__);
    }
#endif

}



/*****************************************************************************
 函数名称  : FASTONOFF_MODE fastOnOffGetFastOnOffMode(void)
 功能描述  : 获取假关机状态接口
 输入参数  : None
 输出参数  : None
 返 回 值  : g_fast_on_off
 修改历史  : 
             1. 2014-1-3 :  00206465 qiaoyichuan created
*****************************************************************************/
FASTONOFF_MODE fastOnOffGetFastOnOffMode(void)
{
#if (FEATURE_ON == MBB_FAST_ON_OFF)
    return g_fast_on_off;
#endif
}
/*****************************************************************************
 函数名称  : int reb_is_factory_mode(void)
 功能描述  : 
 输入参数  : No
 输出参数  : No
 返 回 值  : No
 修改历史  : 
             1. 2013-12-2 :  00206465 qiaoyichuan created
*****************************************************************************/
int reb_is_factory_mode(void)
{
    static int reb_ftm_mod = -1;
    if (-1 == reb_ftm_mod)
    {
#ifdef MBB_FACTORY_FEATURE
        reb_ftm_mod = 0;
#else
        reb_ftm_mod = 1;
#endif
    }
    return reb_ftm_mod;
}

/**********************************************************************
函 数 名  :is_dload_mod
功能描述  :查看是否是下载模式
输入参数  :none
输出参数  :none
返 回 值  :reb_true-是下载模式 reb_false-正常模式 reb_err-执行错误
***********************************************************************/
reb_ret_val is_dload_mod(void)
{
#if (FEATURE_ON == MBB_DLOAD)
    huawei_smem_info *smem_data = NULL;

    smem_data = (huawei_smem_info *)SMEM_HUAWEI_ALLOC(SMEM_ID_VENDOR0, sizeof(huawei_smem_info));
    if ( NULL == smem_data )
    {
        reb_msg( REB_MSG_ERR, "DFT: Invalid Dload flag addr\r\n" );
        return reb_err;
    }

    if( SMEM_DLOAD_FLAG_NUM == smem_data->smem_dload_flag )
    {
        /*下载模式*/
        reb_msg( REB_MSG_DEBUG, "DFT: It's in Dload mode\r\n");
        return reb_true;  
    }
    else
    {
        reb_msg( REB_MSG_DEBUG, "DFT: It's NOT in Dload mode\r\n");
        return reb_false;  
    }
#else
    return reb_false;
#endif
}
EXPORT_SYMBOL(is_dload_mod);
/*****************************************************************************
 函数名称  : int reb_get_current_systime(void)
 功能描述  : 用于获取当前系统开机时间
 输入参数  :none
 输出参数  :none
 返 回 值  :time 当前系统开机时间，精确到秒
*****************************************************************************/
unsigned long  reb_get_current_systime(void)
{
    unsigned long time = 0;
    int this_cpu;
    unsigned long long t;
    unsigned long nanosec_rem;

    this_cpu = smp_processor_id();
    t = cpu_clock(this_cpu);
    nanosec_rem = do_div(t, 1000000000);/*换算处理*/
    time = (unsigned long)t;

    return time;
}
/*****************************************************************************
 函 数 名  : reb_for_power_off
 功能描述  : sys_reboot系统调用直接关机,带参数为:LINUX_REBOOT_CMD_POWER_OFF
 输入参数  : none
 输出参数  : none
 返 回 值  : none
*****************************************************************************/
void reb_for_power_off(void)
{
    /*调用平台关机接口*/
#if (FEATURE_ON == MBB_FAST_ON_OFF)
#if (MBB_CHARGE == FEATURE_ON)
    huawei_kernel_power_off();
#endif
#endif    
    return;
}
