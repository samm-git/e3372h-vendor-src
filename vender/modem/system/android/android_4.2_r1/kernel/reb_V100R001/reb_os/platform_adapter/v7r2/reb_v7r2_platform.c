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
    日期          修改人              问题单号                   修改内容
******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "../../reb_config.h"
#include <linux/export.h>
#include "../../reb_mod/reb_func.h"
#include "../../os_adapter/reb_os_adapter.h"
#include "bsp_hardtimer.h"
#include "bsp_nvim.h"
#include "bsp_sram.h"
#include "bsp_softtimer.h"
#include "../../../../../../../../include/drv/drv_fastOnOff.h"
#include "ptable_com.h"
/*----------------------------------------------*
 * 外部变量声明                                 *
 *----------------------------------------------*/
/*----------------------------------------------*
 * 变量定义                                     *
 *----------------------------------------------*/
struct softtimer_list g_reb_poll_timer ;
reb_hard_timer_callback g_hard_timer_call_func = NULL;
/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
#define   NV_ID_SOFT_RELIABLE_CFG  (50442)
#define   NV_ID_SOFT_FACTORY_CFG  (36)

extern void balong_power_off(void);
/*****************************************************************************
 函 数 名  : hard_timer_creat(unsigned int time_in_ms )
 功能描述  : 创建能够唤醒操作系统的定时
 输入参数  : time_in_ms: 单位是ms的定时间隔
           input_para: 输入参数,用于传给硬定时回调函数
 输出参数  : none
 返 回 值  : none
*****************************************************************************/
void hard_timer_creat(unsigned int time_in_ms, unsigned int input_para )
{
    s32 ret = 0;
    if ( 0 == time_in_ms )
    {
        reb_msg( REB_MSG_ERR,"REB_DFT: time_in_ms is 0\r\n " );
        return;
    }

    if ( NULL == g_hard_timer_call_func )
    {
        reb_msg( REB_MSG_ERR,"REB_DFT: g_hard_timer_call_func is NULL\r\n " );
        return;
    }
    
    g_reb_poll_timer.func = g_hard_timer_call_func;
    g_reb_poll_timer.para = input_para;
    g_reb_poll_timer.timeout = time_in_ms;
    g_reb_poll_timer.wake_type = SOFTTIMER_WAKE;

    if ( g_reb_poll_timer.init_flags == TIMER_INIT_FLAG )
    {
        reb_msg( REB_MSG_ERR,"REB_DFT: g_reb_poll_timer was exist\r\n " );
        return;
    }

    ret = bsp_softtimer_create(&g_reb_poll_timer);
    if(ret)
    {
        reb_msg( REB_MSG_ERR,"REB_DFT: g_reb_poll_timer create error\r\n " );
        return;
    }

    bsp_softtimer_add(&g_reb_poll_timer);

}
//EXPORT_SYMBOL(hard_timer_creat);
/*****************************************************************************
 函 数 名  : hard_timer_reset(unsigned int time_in_ms)
 功能描述  : 能够唤醒操作系统的定时重新定时
 输入参数  : time_in_ms: 单位是ms的定时间隔
             none
 输出参数  : none
 返 回 值  : none
*****************************************************************************/
void hard_timer_reset( unsigned int time_in_ms )
{
    if ( 0 == time_in_ms )
    {
        reb_msg( REB_MSG_ERR,"REB_DFT: time_in_ms is 0\r\n " );
        return;
    }

    if ( g_reb_poll_timer.init_flags != TIMER_INIT_FLAG )
    {
        reb_msg( REB_MSG_ERR,"REB_DFT: g_reb_poll_timer NOT exist\r\n " );
        return;
    }
    
    bsp_softtimer_modify( &g_reb_poll_timer, time_in_ms );
    bsp_softtimer_add(&g_reb_poll_timer);
    return;
}
/*****************************************************************************
 函 数 名  : hard_timer_delete(void)
 功能描述  : 创建能够唤醒操作系统的定时
 输入参数  : time_in_ms: 单位是ms的定时间隔
             none
 输出参数  : none
 返 回 值  : none
*****************************************************************************/
void hard_timer_delete(void)
{
    int ret = 0;
    if ( g_reb_poll_timer.init_flags != TIMER_INIT_FLAG )
    {
        reb_msg( REB_MSG_ERR,"REB_DFT: g_reb_poll_timer NOT exist\r\n " );
        return;
    }

   ret = bsp_softtimer_delete(&g_reb_poll_timer);

    if (-1 == ret)
    {
        reb_msg(REB_MSG_ERR, "REB_DFT:hard_timer_delete fail \n");
    }

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
    int ret = 0;
    static int reb_ftm_mod = -1;
    if (-1 == reb_ftm_mod)
    {
        ret = bsp_nvm_read(NV_ID_SOFT_FACTORY_CFG, (unsigned char *)(& reb_ftm_mod),
                           sizeof( unsigned int));
        if (0 != ret)
        {
            reb_msg( REB_MSG_ERR, "REB_DFT: get nv factory err \r\n" );
            return -1;
        }
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
reb_ret_val  is_dload_mod(void)
{
#if (FEATURE_ON == MBB_DLOAD)
    huawei_smem_info *smem_data = NULL;
    smem_data = (huawei_smem_info *)SRAM_DLOAD_ADDR;

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
    unsigned long long t, tmp;
    unsigned long timer_get = 0;
    timer_get = bsp_get_slice_value();
    t = (unsigned long long)(timer_get & 0xFFFFFFFF);/*换算处理*/
    tmp = t;
    tmp = (tmp * 1000) >> 15;/*timer clock is 32768 per second*/
    time = (unsigned long)tmp / 1000;/*换算成秒*/
    return time;
}

/*****************************************************************************
 函数名称  : unsigned int  get_random_num(void)
 功能描述  : 用于获取当前系统开机时间
 输入参数  :none
 输出参数  :none
 返 回 值  :time 当前系统开机时间，精确到秒
*****************************************************************************/
unsigned int  get_random_num(void)
{
    unsigned int num = bsp_get_slice_value();
    reb_msg( REB_MSG_INFO, "Reb_DFT get_random_num %u\r\n", num );
    return num;
}
EXPORT_SYMBOL(get_random_num);
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
    balong_power_off();
#endif
#endif
    return;
}
/*****************************************************************************
 函数名称  : int reb_nv_kernel_read(void)
 功能描述  : 根据不同平台读取nv值
 输入参数  : No
 输出参数  : No
 返 回 值  : No
 修改历史  : 
             1. 2013-11-28 :  00206465 qiaoyichuan created
*****************************************************************************/
int reb_nv_kernel_read(void)
{
    int ret = 0;
    ret = bsp_nvm_read(NV_ID_SOFT_RELIABLE_CFG,
            (unsigned char *)(& g_kernel_soft_reliable_info), 
            sizeof(SOFT_RELIABLE_CFG_STRU));

    if (0 != ret)
    {
        reb_msg( REB_MSG_ERR, "REB_DFT: get nv config err \r\n" );
        return -1;
    }
    return 0;
}
/*****************************************************************************
 函数名称  : void reb_get_ptable_num()
 功能描述  : 获取分区数目
 输入参数  : No
 输出参数  : No
 返 回 值  : i 分区数目
 修改历史  : 
*****************************************************************************/
int reb_get_ptable_num(void)
{
    int i = 0;
    struct ST_PART_TBL * ptable = NULL;
    ptable = ptable_get_ram_data();
    if(NULL == ptable)
    {
        return;
    }
    while (0 != strncmp(PTABLE_END_STR, ptable->name,strlen(PTABLE_END_STR)))
    {
        i++;
        ptable++;
    }
	
    return i;
}
EXPORT_SYMBOL(reb_get_ptable_num);

/*****************************************************************************
 函数名称  : ST_PART_TBL * ptalbe_get()
 功能描述  : 获取分区表内容
 输入参数  : No
 输出参数  : No
 返 回 值  : i 分区表指针
 修改历史  : 
*****************************************************************************/
struct ST_PART_TBL * ptable_get(void)
{
    struct ST_PART_TBL * ptable = NULL;
    ptable = ptable_get_ram_data();
    if(NULL == ptable)
    {
        reb_msg( REB_MSG_ERR, "REB_DFT: ptable get error \r\n" );
    }
    return ptable;
}
EXPORT_SYMBOL(ptable_get);
