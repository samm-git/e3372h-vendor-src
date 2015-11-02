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
/**************************问题单修改记录******************************
    日期         修改人              问题单号                   修改内容
******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "../reb_config.h"
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/export.h>
#include <linux/delay.h>
#include <linux/irq.h>

#include <linux/kthread.h>
#include <linux/timer.h>
#include <linux/wakelock.h>
#include "reb_os_adapter.h"
/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/



/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/
/*用于定时(不能唤醒系统,有回调)*/
static struct timer_list s_reb_sta_timer;

/*Linux中接收到的事件记录*/
static unsigned int s_event_recv = 0;
static reb_msg_log s_print_lvl = REB_MSG_INFO;

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*可靠性任务的ID*/
static struct task_struct *s_RebTaskID = NULL;

struct wake_lock reb_sleep_lock;
/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
 
/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/
/*****************************************************************************
 函 数 名  : void reb_lock_sleep( void* sleep_id )
 功能描述  : 投票不允许进入低功耗
 输入参数  : pt_sleep_id: wakelock or just vote id
 输出参数  : none
 返 回 值  : none
*****************************************************************************/
void reb_lock_sleep( unsigned int sleep_id )
{
    if ( 0 == sleep_id )
    {
        //for lint;
    }
    wake_lock( &reb_sleep_lock );
}

/*****************************************************************************
 函 数 名  : void reb_unlock_sleep( void* sleep_id )
 功能描述  : 投票允许进入低功耗
 输入参数  : pt_sleep_id: wakelock or just vote id
 输出参数  : none
 返 回 值  : none
*****************************************************************************/
void reb_unlock_sleep( unsigned int sleep_id )
{
    if ( 0 == sleep_id )
    {
        //for lint;
    }

    wake_unlock( &reb_sleep_lock );
}

/*****************************************************************************
 函 数 名  : reb_msg
 功能描述  : LINUX平台通用的打印函数
 输入参数  : int print_level
             const char *fmt...
 输出参数  : none
 返 回 值  : none
*****************************************************************************/
asmlinkage void reb_msg(int print_level,const char *fmt,...)
{
    va_list args;
    int ret_val = 0;

    /*小于等于REB_MSG_INFO级别的LOG可以被打印*/
    if((unsigned int)print_level <= s_print_lvl)
    {
        va_start(args, fmt);
        ret_val = vprintk(fmt, args);
        va_end(args);
    }

    if (ret_val)
    {
        //for lint
    }
        
    return;
}
/**********************************************************************
函 数 名: reb_task_create
功能描述: 可靠性模块创建任务
输入参数: 任务执行函数
输出参数: None
返 回 值: -1:创建任务失败
***********************************************************************/
int reb_task_create( reb_task_fun reb_task )
{
    if ( NULL == reb_task )
    {
        reb_msg( REB_MSG_ERR,"Reb_DFT function is NULL!\r\n");
        return -1;
    }

    if ( 0 != IS_ERR(s_RebTaskID) )
    {
        /*可靠性任务已经创建*/
        reb_msg( REB_MSG_ERR,"Reb_DFT thread EXIST!\r\n");
        return -1;
    }
    /*初始化wakelock*/
    wake_lock_init( &reb_sleep_lock, WAKE_LOCK_SUSPEND, "reb_wake_lock" );
    /*可靠性模块创建任务*/
    s_RebTaskID = kthread_run( reb_task, NULL, REB_TASK_NAME);

    if ( IS_ERR(s_RebTaskID) )
    {
        reb_msg( REB_MSG_ERR,"Reb_DFT thread create error!\r\n");
    }
    else
    {
        reb_msg( REB_MSG_INFO,"Reb_DFT thread create success!\r\n");
    }
    return 0;
}

/**********************************************************************
函 数 名: reb_task_delete
功能描述: 删除任务
输入参数: None
输出参数: None
返 回 值: None
***********************************************************************/
void reb_task_delete( void )
{
    int ret = 0;

    if (!IS_ERR(s_RebTaskID))
    {
        ret = kthread_stop(s_RebTaskID);
        if (ret)
        {
            //for lint
        }
    }
}
/**********************************************************************
函 数 名: reb_delay_ms
功能描述: 等待延时的时间（单位毫秒）
输入参数: 需要延时的时间（单位毫秒）
输出参数: None
返 回 值: None
注意事项: Linux操作系统下精度为1ms
***********************************************************************/
void reb_delay_ms( unsigned int delay_time_ms )
{
    msleep(delay_time_ms);
}

/*************************************************************
  函数名称: reb_timer_set
  功能描述: 提供给状态机轮询模块的定时器,定时到后执行回调函数
  输入参数: 定时时间(以ms为单位), 回调函数
  输出参数: None
  返 回 值: None
  注意事项: 1-不能唤醒操作系统
            2-不是循环定时器
*************************************************************/
void reb_timer_set( unsigned int cycle_in_ms, reb_poll_timer_callback func)
{
    if ( NULL == s_reb_sta_timer.function )
    {
        /*为可靠性模块提供的定时器*/
        setup_timer( &s_reb_sta_timer, (void *)func, 0 );
    }
    mod_timer( &s_reb_sta_timer, jiffies + msecs_to_jiffies(cycle_in_ms) );
}

/**********************************************************************
函 数 名: reb_send_msg_to_main_task
功能描述: 发送事件给可靠性任务
输入参数: 事件Id
输出参数: none
返 回 值: -1-任务不存在/事件ID错误
注意事项: none
***********************************************************************/
void reb_send_msg_to_main_task( unsigned int set_event_id )
{
    if ( NULL == s_RebTaskID )
    {
        reb_msg( REB_MSG_ERR, "Reb_DFT: Invalid Task!!!\n ");
        return;
    }

    if (0 == (REB_ALL_VALID_EVENT & set_event_id))
    {
        reb_msg( REB_MSG_ERR, "Reb_DFT: Invalid event id!!!\n ");
        return;
    }

    /*加锁*/    
    /*记录该事件ID*/
    s_event_recv |= set_event_id;
    /*解锁*/    
#if 1
    /*唤醒可靠性任务*/
    wake_up_process( s_RebTaskID );
#endif
    return;
}

/**********************************************************************
函 数 名: reb_task_wait_for_msg
功能描述: 可靠性任务处于挂起状态并等待被事件激活
输入参数: event_ids-激活任务需要获取的事件集,如果为0说明任何事件都可以
            激活该任务
输出参数: get_event_id-接收的事件
返 回 值: -1-数据错误 
注意事项: Linux系统中，调用该函数时须判断是否还有事件没有处理完成
***********************************************************************/
void reb_task_wait_for_msg( unsigned int *get_event_id,
                            const unsigned int want_event_ids )
{

    if ( 0 == want_event_ids )
    {
        //for lint
    }

    /*如果能够激活就会跳出该循环,否则就会继续挂起*/
#if 0
    while (1)
    {
        if ( 0 != want_event_ids )
        {
            /*能够激活可靠性任务*/
            if ( want_event_ids == ( want_event_ids & s_event_recv ) )
            {
                break;
            }
        }

        if (0 != (REB_ALL_VALID_EVENT & s_event_recv))
        {
            break;
        }

        /*可靠性任务进入挂起状态*/
        set_current_state(TASK_INTERRUPTIBLE);
        schedule();
    }
#endif
    /*接收到事件以后，取出事件记录*/
    if ( 0 != s_event_recv )
    {
        reb_msg( REB_MSG_INFO, "Reb_DFT: receive event %d\n ", *get_event_id);
        *get_event_id = s_event_recv;
        s_event_recv = 0;
    }

    return;
}
/*****************************************************************************
 函 数 名  : reb_set_print_lvl( reb_msg_log new_level )
 功能描述  : 设置打印级别
 输入参数  : set_print_lvl( reb_msg_log new_level )
           input_para: 输入参数,用于传给硬定时回调函数
 输出参数  : none
 返 回 值  : none
*****************************************************************************/
int reb_set_print_lvl( reb_msg_log new_level )
{
    if ( ( unsigned int )new_level > REB_MSG_DEBUG )
    {
        reb_msg( REB_MSG_ERR, "\r\nReb_DFT: ERR para:%u\n ", ( unsigned int )(new_level) );
        return -1;
    }

    s_print_lvl = new_level;
    return 0;

}
