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
    日期       修改人       问题单号                 修改内容
******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/

#include "../../chg_config.h"
#if (MBB_CHG_PLATFORM_V7R2 == FEATURE_ON)
#include <linux/wakelock.h>
#endif
/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/



/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/
/*打印通道上限，数值比这个上限低的能够打印出来*/

//uint32_t chg_print_level = CHG_MSG_DEBUG;
extern int32_t g_huawei_drv_debug;



#if (MBB_CHG_PLATFORM_V7R2 == FEATURE_ON)

/*充电轮询定时器(不唤醒系统,有回调)*/
struct softtimer_list g_chg_poll_timer;

/*用于充电器类型检测定时器(可以设置是否唤醒系统,有回调)*/
struct softtimer_list g_chg_sta_timer ;

/*过温/低电关机定时(可以设置是否唤醒系统,有回调,带参数)*/
struct softtimer_list g_chg_bat_timer;

/*充电轮询定时器，休眠之后检测电池状态(需要唤醒系统,有回调)*/
struct softtimer_list g_chg_sleep_timer ;
#else
/*充电轮询定时器(不能唤醒系统,有回调)*/
struct timer_list g_chg_poll_timer;

/*用于充电器类型检测定时器(不能唤醒系统,有回调)*/
static struct timer_list g_chg_sta_timer;

/*过温/低电关机定时(不能唤醒系统,有回调,带参数)*/
static struct timer_list g_chg_bat_timer;

#endif/*MBB_CHG_PLATFORM_V7R2*/


/*Linux中接收到的事件记录*/
static uint32_t g_event_recv = 0;
/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*充电任务的ID*/
struct task_struct *g_chgTaskID = NULL;

struct wake_lock g_sply_lock;

/*充电模块轮询周期记录(ms)*/
uint32_t chg_poll_time_bak = FAST_POLL_CYCLE;
/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
/*充电任务和补电任务的优先级*/
#define CHG_TASK_PRI  (150)

/* 目前使用的延时时间的最小精度(10ms)*/
#define CHG_DELAY_COUNTER_SIZE (10)
/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/
extern void chg_sleep_batt_check_timer(void);
/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/
static DEFINE_SPINLOCK(chg_event_lock);

/*****************************************************************************
 函数名称  : chg_sply_lock
 功能描述  : 对系统补电时加wake锁，防止系统睡眠
 输入参数  : No
 输出参数  : No
 返 回 值  : No
 其他说明  :无

 修改历史      :
  1.日    期   : 2013年8月21日
    作    者   : libaojun
    修改内容   : 新生成函数
*****************************************************************************/
void chg_sply_lock(void)
{
    wake_lock(&g_sply_lock);
    chg_print_level_message( CHG_MSG_INFO,"--->Begin keep system wakeup for supply charge!\r\n");
}
/*****************************************************************************
 函数名称  : chg_sply_unlock
 功能描述  : 对系统补电完成释放wake锁
 输入参数  : No
 输出参数  : No
 返 回 值  : No
 其他说明  :无

 修改历史      :
  1.日    期   : 2013年8月21日
    作    者   : libaojun
    修改内容   : 新生成函数
*****************************************************************************/
void chg_sply_unlock(void)
{
    wake_unlock(&g_sply_lock);
    chg_print_level_message( CHG_MSG_INFO,"--->Release sply wake lock ,supply success!\r\n");
}

/*****************************************************************************
 函 数 名  : chg_print_level_message.fmt
 功能描述  : LINUX平台通用的打印函数
 输入参数  : int print_level
             const char *fmt...
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年5月8日
    作    者   : chendongguo
    修改内容   : 新生成函数

*****************************************************************************/
asmlinkage int chg_print_level_message(int print_level,const char *fmt,...)
{
    va_list args;
    int rc = 0;

    /*小于chg_print_level级别的LOG可以被打印*/
    if((uint32_t)print_level < g_huawei_drv_debug)
    {
        va_start(args, fmt);
        rc = vprintk(fmt, args);
        va_end(args);
    }
    return rc;
}

/**********************************************************************
函 数 名: chg_task_create
功能描述: 创建归一化充电任务
输入参数: None
输出参数: None
返 回 值    : None
注意事项: Vxworks的实现
          因为有系统调用,因此注意包含相应的库文件
***********************************************************************/
void chg_task_create( void )
{
    /*创建充电轮训定时器*/
#if (MBB_CHG_PLATFORM_V7R2 == FEATURE_ON)
    s32 ret = 0;

    g_chg_poll_timer.func = chg_send_msg_to_main;
    g_chg_poll_timer.para = CHG_TIMER_EVENT;
    g_chg_poll_timer.timeout = FAST_POLL_CYCLE;
    /* fix HISI Softtimer B170 bug 2013-09-17 */
    g_chg_poll_timer.wake_type = SOFTTIMER_NOWAKE;

    g_chg_sleep_timer.func = (softtimer_func)chg_sleep_batt_check_timer;
    g_chg_sleep_timer.para = 0;
    g_chg_sleep_timer.timeout = SLEEP_POLL_CYCLE;
    g_chg_sleep_timer.wake_type = SOFTTIMER_WAKE;

    ret = bsp_softtimer_create(&g_chg_poll_timer);
    if(ret)
    {
        chg_print_level_message( CHG_MSG_ERR," %s: g_chg_poll_timer create error!!!\r\n ", __func__);
        return;
    }

    ret = bsp_softtimer_create(&g_chg_sleep_timer);
    if(ret)
    {
        chg_print_level_message( CHG_MSG_ERR," %s: chg_sleep_timer create error!!!\r\n ", __func__);
        return;
    }
#else
    if ( NULL == g_chg_poll_timer.function )
    {
        setup_timer(&g_chg_poll_timer,(void *)chg_send_msg_to_main,CHG_TIMER_EVENT);
    }
#endif/*MBB_CHG_PLATFORM_V7R2*/
    /* 创建充电线程 */
    g_chgTaskID = kthread_run( chg_main_task, NULL, CHG_TASK_NAME);
    if ( IS_ERR(g_chgTaskID) )
    {
        chg_print_level_message( CHG_MSG_ERR," %s: Thread create error!!!\r\n ", __func__);
    }
    else
    {
        chg_print_level_message( CHG_MSG_ERR," %s: Thread create success!!!\r\n ", __func__);
    }

#if (MBB_CHG_PLATFORM_V7R2 == FEATURE_ON)
    /*添加启动定时器*/
    bsp_softtimer_add(&g_chg_poll_timer);
    bsp_softtimer_add(&g_chg_sleep_timer);
#endif/*MBB_CHG_PLATFORM_V7R2*/

    return;
}


/**********************************************************************
函 数 名: chg_task_delete
功能描述: 删除归一化充电任务
输入参数: None
输出参数: None
返 回 值: None
注意事项: 涵盖了Linux和Vxworks的实现
          因为有系统调用,因此注意包含相应的库文件
***********************************************************************/

void chg_task_delete( void )
{
    int32_t ret = 0;

    if (!!g_chgTaskID)
    {
        chg_print_level_message( CHG_MSG_INFO," %s: Thread start to stop!!!\r\n ", __func__);
        ret = kthread_stop(g_chgTaskID);
        chg_print_level_message( CHG_MSG_INFO," %s: Thread stop success ret %d!!!\r\n ", __func__, ret);
    }
}

#if (MBB_CHG_COMPENSATE == FEATURE_ON)
/**********************************************************************
函 数 名: chg_sply_task_create
功能描述: 创建归一化补电任务
输入参数: None
输出参数: None
返 回 值: None
注意事项: 涵盖了Linux和Vxworks的实现
          因为有系统调用,因此注意包含相应的库文件
***********************************************************************/
void chg_sply_task_create( void )
{
    struct task_struct *tsk = NULL;
    static boolean flag_task_init = FALSE;

    wake_lock_init( &g_sply_lock, WAKE_LOCK_SUSPEND, "sply_lock" );
    if (FALSE == flag_task_init)
    {
        /* 创建补电线程 */
        tsk = kthread_run( chg_batt_supply_proc, NULL, CHG_SPLY_TASK_NAME );

        /*如果创建任务失败*/
        if ( IS_ERR(tsk) )
        {
            chg_print_level_message( CHG_MSG_ERR," %s: Thread create error!!!\r\n ", __func__);
        }
        else
        {
            flag_task_init = TRUE;
            chg_print_level_message( CHG_MSG_ERR," %s: Thread create success!!!\r\n ", __func__);
        }

    }
    else
    {
        chg_print_level_message( CHG_MSG_ERR," %s: Thread already create !!!\r\n ", __func__);
    }

    return;
}
#endif/*MBB_CHG_COMPENSATE*/
/**********************************************************************
函 数 名: chg_delay_ms
功能描述: 等待延时的时间（单位毫秒）
输入参数: 需要延时的时间（单位毫秒）
输出参数: None
返 回 值: None
注意事项: vxworks
***********************************************************************/
void chg_delay_ms( uint32_t delay_time_ms )
{
    /*  因为目前的时间处理以10ms作为最小单位
    因此对输入数据区整处理*/
    msleep(delay_time_ms);
}

/*************************************************************
  函数名称: chg_poll_timer_set
  功能描述: 提供充电轮询定时器
  输入参数: 轮询周期(以ms为单位)
  输出参数: None
  返 回 值: None
  注意事项:  1\这个定时器用来作为充电模块定时轮询
                          2\时间到后，需重新定时
*************************************************************/
void chg_poll_timer_set( uint32_t cycle_in_ms )
{
    /*如果timer更新了周期*/
    if ( ( cycle_in_ms != chg_poll_time_bak ) )
    {
        /*更新充电模块轮询周期(ms)*/
        chg_poll_time_bak = cycle_in_ms;
    }
    chg_print_level_message( CHG_MSG_INFO," start poll chg state timer hz=%d!\r\n ",cycle_in_ms);

#if (MBB_CHG_PLATFORM_V7R2 == FEATURE_ON)
    (void)bsp_softtimer_delete_sync(&g_chg_poll_timer);
    bsp_softtimer_modify(&g_chg_poll_timer,cycle_in_ms);
    bsp_softtimer_add(&g_chg_poll_timer);
#else
    mod_timer( &g_chg_poll_timer, jiffies + msecs_to_jiffies( cycle_in_ms ) );
#endif/*MBB_CHG_PLATFORM_V7R2*/
}

/*************************************************************
  函数名称: chg_poll_timer_get
  功能描述: 提供充电轮询定时器查询
  输入参数: None
  输出参数: None
  返 回 值: -1:错误(可能是还没有设置过),
                      2000/20000:轮询周期(以ms为单位)
  注意事项: 请注意,读取之前一定要保证调过chg_poll_timer_set
*************************************************************/
int32_t chg_poll_timer_get( void )
{
    /*返回充电模块定时轮询周期*/
    if ( ( FAST_POLL_CYCLE == chg_poll_time_bak )
        || ( SLOW_POLL_CYCLE == chg_poll_time_bak ) )
    {
        return ( int32_t )chg_poll_time_bak;
    }
    else
    {
        return ( int32_t )(-1);
    }

}
/*************************************************************
  函数名称: chg_sta_timer_set
  功能描述: 提供给状态机轮询模块的定时器,定时到后执行回调函数
  输入参数: 定时时间(以ms为单位), 回调函数
  输出参数: None
  返 回 值: None
  注意事项: None
*************************************************************/
void chg_sta_timer_set( uint32_t cycle_in_ms, chg_poll_timer_callback func)
{

#if (MBB_CHG_PLATFORM_V7R2 == FEATURE_ON)
    s32 ret = 0;
    g_chg_sta_timer.func = (softtimer_func)func;
    g_chg_sta_timer.para = 0;
    g_chg_sta_timer.timeout = cycle_in_ms;
    /* fix HISI Softtimer B170 bug 2013-09-17 */
    g_chg_sta_timer.wake_type = SOFTTIMER_WAKE;

    if(g_chg_sta_timer.init_flags == TIMER_INIT_FLAG )
    {
        (void)bsp_softtimer_delete(&g_chg_sta_timer);
        bsp_softtimer_modify(&g_chg_sta_timer,cycle_in_ms);
        bsp_softtimer_add(&g_chg_sta_timer);
    }
    else
    {
        ret = bsp_softtimer_create(&g_chg_sta_timer);
        if(ret)
        {
            chg_print_level_message( CHG_MSG_ERR," %s: g_chg_sta_timer create error!!!\r\n ", __func__);
            return;
        }
        bsp_softtimer_add(&g_chg_sta_timer);
    }
#else
    if ( NULL == g_chg_sta_timer.function )
    {
        /*为充电器类型检测提供的定时器*/
        setup_timer( &g_chg_sta_timer, func, 0 );
    }

    mod_timer( &g_chg_sta_timer, jiffies + msecs_to_jiffies(cycle_in_ms) );
#endif/*MBB_CHG_PLATFORM_V7R2*/
}

/*************************************************************
  函数名称: chg_bat_timer_set
  功能描述: 提供给电池电压/温度监测模块的定时器,定时到时执行回调函数
  输入参数:
                        cycle_in_ms: 定时时间(ms)
                        func: 回调函数,回调函数的入参
                        para: func的入参
  输出参数: None
  返 回 值: None
  注意事项: None
*************************************************************/
void chg_bat_timer_set( uint32_t cycle_in_ms, void (*func)(uint32_t), uint32_t para )
{
#if (MBB_CHG_PLATFORM_V7R2 == FEATURE_ON)
#if 0
    struct softtimer_info chg_bat_emergency_shutoff_timer;
    chg_bat_emergency_shutoff_timer.func = func;
    chg_bat_emergency_shutoff_timer.para = para;
    chg_bat_emergency_shutoff_timer.timeout = cycle_in_ms;
    chg_bat_emergency_shutoff_timer.wake_type = SOFTTIMER_WAKE;

    g_chg_bat_timer = bsp_softtimer_create(chg_bat_emergency_shutoff_timer);
    if(g_chg_bat_timer == NULL)
    {
        chg_print_level_message( CHG_MSG_ERR," %s: g_chg_bat_timer create error!!!\r\n ", __func__);
        return;
    }
#endif
    drvShutdown(para);

#else
    if ( NULL == g_chg_bat_timer.function )
    {
        /*为状态机模块提供的定时器不需要参数*/
        setup_timer( &g_chg_bat_timer, (void *)func, para );
    }
    mod_timer( &g_chg_bat_timer, jiffies + msecs_to_jiffies(cycle_in_ms) );
#endif/*MBB_CHG_PLATFORM_V7R2*/
}

/**********************************************************************
函 数 名  : chg_send_msg_to_main
功能描述  : 定时器到达后向充电任务发送事件
输入参数  : param 固定格式参数
输出参数  : 无。
返 回 值  : 无。
注意事项  : 无。
***********************************************************************/
void chg_send_msg_to_main(uint32_t event_id)
{
    unsigned long flags = 0;

    if ( 0 == ( CHG_ALL_EVENT & event_id ) )
    {
        chg_print_level_message( CHG_MSG_ERR," Invalid event id!!!\r\n ");
    }
    else
    {
        spin_lock_irqsave(&chg_event_lock, flags);
        g_event_recv |= event_id;
        spin_unlock_irqrestore(&chg_event_lock, flags);
        chg_print_level_message( CHG_MSG_DEBUG,"send_msg_to_main g_event_recv=%x\n ", g_event_recv);
        chg_print_level_message( CHG_MSG_DEBUG,"send_msg_to_main event_id=%x\n ", event_id);
        wake_up_process(g_chgTaskID);
    }
#if (MBB_CHG_PLATFORM_V7R2 == FEATURE_ON)
    if(CHG_TIMER_EVENT == event_id)
    {
        bsp_softtimer_modify(&g_chg_poll_timer,chg_poll_time_bak);
        bsp_softtimer_add(&g_chg_poll_timer);
    }
#else
    mod_timer( &g_chg_poll_timer, jiffies + msecs_to_jiffies( chg_poll_time_bak ) );
#endif/*MBB_CHG_PLATFORM_V7R2*/
}

/**********************************************************************
函 数 名  : this_kthread_should_stop
功能描述  : When someone calls kthread_stop() on your kthread, it will be woken
and this will return true. You should then return, and your return value will
be passed through to kthread_stop()
输入参数  : 无
输出参数  : 无。
返 回 值  : TRUE:表示有调用kthread_stop()；FALSE:表示没有；
注意事项  : 无。
***********************************************************************/
boolean this_kthread_should_stop(void)
{
    if (kthread_should_stop())
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
/**********************************************************************
函 数 名: chg_main_wait_for_msg
功能描述: 充电任务处于休眠状态并等待被事件唤醒
输入参数: None
输出参数: 接收的事件
返 回 值: 无。
注意事项: Linux系统中，调用该函数时须判断是否还有事件没有处理完成
***********************************************************************/
int32_t chg_main_wait_for_msg( uint32_t* get_event_id )
{
    unsigned long flags = 0;
    if ( NULL == get_event_id )
    {
        chg_print_level_message( CHG_MSG_ERR, " Invalid address!!!\n ");
        return -1;
    }

    if (0 == (CHG_ALL_EVENT & g_event_recv))
    {
        /*充电任务进入休眠状态*/
        set_current_state(TASK_INTERRUPTIBLE);
        schedule();
    }

    chg_print_level_message( CHG_MSG_INFO," ****g_event_recv %d!!!***\n ", g_event_recv);

    spin_lock_irqsave(&chg_event_lock, flags);
    /*接收到事件以后，取出事件记录*/
    *get_event_id = g_event_recv;
    g_event_recv = 0;
    spin_unlock_irqrestore(&chg_event_lock, flags);
    return 0;
}


