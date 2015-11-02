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
#include "reb_config.h"
#include <linux/export.h>
#include "os_adapter/reb_os_adapter.h"
#if (YES == Reb_Platform_9x25)
#include "platform_adapter/9x25/reb_9x25_platform.h"
#endif
#if (YES == Reb_Platform_V7R2)
#include "platform_adapter/v7r2/reb_v7r2_platform.h"
#endif
#include "reb_mod/reb_func.h"
/*----------------------------------------------*
 * 外部变量声明                                 *
 *----------------------------------------------*/
#if (FEATURE_ON == MBB_FAST_ON_OFF)
#if (MBB_CHARGE == FEATURE_ON)
extern void chg_send_stat_to_app(unsigned int chg_device_id, 
    unsigned int chg_event_id);
extern void reb_fast_off_inspect_init(void);
#endif
#endif
#if (YES == MBB_FEATURE_REB_DFT)   
extern  int  reb_test_moudle_init(void);
extern void  reb_test_moudle_exit(void);
#endif
extern int  reb_reliability_nv_cfg_kernel_init(void);
extern reb_ret_val is_dload_mod(void);
#if (YES == Reb_Platform_V7R2)
extern int reb_scan_flash_parts( void );
extern int reb_part_scan_init(void);
extern void show_cur_scan_pos(void);
#endif
/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/

#define FIRST_RUN_TIME (30000) /*可靠性任务30秒以后开始运行*/
#define EVERY_SCAN_ALTERNATION (3000) /*可靠性任务最短3000ms扫描flash一次*/

/*----------------------------------------------*
 * 变量定义                                     *
 *----------------------------------------------*/
/*reb_task在EVERY_SCAN_ALTERNATION秒内被调度的累计时间(us)*/
unsigned int  lately_idle_time = 0;

/*如果达到lately_idle_time低于min_idle_duration,
就认为当前系统繁忙,不进行扫描*/
unsigned int min_idle_duration = (2500000);

int reb_idle_sign_set(unsigned int idle_time_in_us)
{
    lately_idle_time += idle_time_in_us;
    return 0;
}

EXPORT_SYMBOL(reb_idle_sign_set);

void reb_idle_sign_clr(void)
{
    lately_idle_time = 0;
}

/**********************************************************************
函 数 名: reb_main_task
功能描述: 底软可靠性任务
输入参数: 暂未使用
输出参数: None
返 回 值: None
***********************************************************************/
int reb_main_task( void *task_para )
{
    unsigned int event_recv = 0;
    static unsigned int os_idle_cnt = 0;
    if ( NULL == task_para )
    {
        //for lint
    }

    /*为了不合开机搅合在一起,上电30秒以后开始工作*/
    reb_delay_ms(FIRST_RUN_TIME);

#if (YES == Reb_Platform_V7R2 )
    reb_part_scan_init(); /*初始化扫描范围*/
#endif
#if (FEATURE_ON == MBB_FAST_ON_OFF)
#if (MBB_CHARGE == FEATURE_ON)
    reb_fast_off_inspect_init();
#endif
#endif
    for ( ; ; )
    {
        reb_task_wait_for_msg( &event_recv, 0 );
        if ( event_recv & REB_FAST_OFF_BAT_OVERUSE )
        {
            event_recv &= ~REB_FAST_OFF_BAT_OVERUSE;
#if (FEATURE_ON == MBB_FAST_ON_OFF)
#if (MBB_CHARGE == FEATURE_ON)
            /*todo:上报按键真关机事件*/
            reb_msg(REB_MSG_ERR, "REB_DFT :power off for bat overuse\r\n");
            reb_for_power_off();
#endif
#endif
        }
#if (YES == Reb_Platform_9x25)
        /*处理定时器事件*/
        if ( event_recv &REB_HARD_TIMER_OUT_EVENT)
        {
            event_recv &= ~REB_HARD_TIMER_OUT_EVENT;
            hard_timer_reset(FAST_ON_OFF_CYCLE);
        }

        if ( event_recv &REB_HARD_TIMER_STOP_EVENT)
        {
            event_recv &= ~REB_HARD_TIMER_STOP_EVENT;
            hard_timer_delete();
        }
#endif
        event_recv &= ~REB_SYS_IDLE_EVENT;

        if ( lately_idle_time < min_idle_duration )
        {
            os_idle_cnt = 0;
            reb_msg( REB_MSG_DEBUG, 
                "REB_DFT :Os is busy, lately_idle_time:%u,\r\n", lately_idle_time );
        }
        else
        {   
            /*做一个平滑动作*/
            if ( ++os_idle_cnt >= 3 )
            {

#if (YES == Reb_Platform_V7R2 )
#ifdef REB_OS_DEBUG
            /*调试使用:显示当前扫描的信息*/
            show_cur_scan_pos();
#endif

            reb_scan_flash_parts();
#endif
            }
        }
        reb_msg(REB_MSG_DEBUG, "REB_DFT :lately_idle_time is:%u\r\n",
            lately_idle_time );                
        reb_idle_sign_clr();

        /*等待3秒后再次调度*/
        reb_delay_ms( EVERY_SCAN_ALTERNATION );
    }

    return 0;
}
/**********************************************************************
函 数 名  :reb_init
功能描述  :开机初始化
输入参数  :none
输出参数  :none
返 回 值  :none
***********************************************************************/
static int reb_init(void)
{
    reb_ret_val ret_val = is_dload_mod();
    if ( ( reb_err == ret_val ) || ( reb_true == ret_val ) )
    {
        /*如果是下载模式,可靠性模块不初始化*/
        return 0;        
    }
    if(0 == reb_is_factory_mode())
    {
        return 0;
    }
    reb_msg(REB_MSG_INFO, "REB_DFT:%s exec...\r\n", __func__);
    reb_reliability_nv_cfg_kernel_init();
    reb_task_create( (reb_task_fun)reb_main_task );
#if (YES == MBB_FEATURE_REB_DFT)   
    reb_test_moudle_init();
#endif
    return 0;
}

late_initcall(reb_init);

static void reb_exit(void)
{
#if (YES == MBB_FEATURE_REB_DFT)   
    reb_test_moudle_exit();
#endif
    return;
}
module_exit(reb_exit);

