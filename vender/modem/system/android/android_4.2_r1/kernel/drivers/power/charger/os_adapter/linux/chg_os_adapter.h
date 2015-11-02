#ifndef CHG_OS_ADAPTER_H
#define CHG_OS_ADAPTER_H

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
 * 外部变量说明                                 *
 *----------------------------------------------*/


/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

extern void chg_task_create( void );
extern void chg_task_delete( void );

#if (MBB_CHG_COMPENSATE == FEATURE_ON)
extern void chg_sply_task_create( void );
#endif/*MBB_CHG_COMPENSATE*/

#define chg_timer_para unsigned long //uint32_t
typedef void (*chg_poll_timer_callback)(unsigned long);

/*提供充电轮询定时器*/
extern void chg_poll_timer_set( uint32_t cycle_in_ms );
extern int32_t chg_poll_timer_get( void );

/*提供给状态机轮询模块的定时器,定时到后执行回调函数*/
void chg_sta_timer_set( uint32_t cycle_in_ms, chg_poll_timer_callback func );

extern int32_t chg_main_wait_for_msg( uint32_t* get_event_id );

extern asmlinkage int chg_print_level_message(int print_level,const char *fmt,...);

extern void chg_delay_ms( uint32_t delay_time_ms );

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/
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
void chg_sply_lock(void);
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
void chg_sply_unlock(void);


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
void chg_bat_timer_set( uint32_t cycle_in_ms, void (*func)(uint32_t), uint32_t para );


/**********************************************************************
函 数 名  : chg_send_msg_to_main
功能描述  : 定时器到达后向充电任务发送事件
输入参数  : param 固定格式参数
输出参数  : 无。
返 回 值  : 无。
注意事项  : 无。
***********************************************************************/
extern void chg_send_msg_to_main(uint32_t event_id);

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
boolean this_kthread_should_stop(void);
/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
/*Boolean data type.*/
#if defined(TRUE)
  #if (TRUE != 1)
    #error "TRUE" defined conflicted with native OS
  #endif
#else
  #define TRUE                   1
#endif /*defined(TRUE)*/

#if defined(FALSE)
  #if (FALSE != 0)
    #error "FALSE" defined conflicted with native OS
  #endif
#else
  #define FALSE                  0
#endif /*defined(FALSE)*/

/*充电任务名*/
#define CHG_TASK_NAME "ChgTask"
#define CHG_SPLY_TASK_NAME "ChgSplyTask"

/*快慢定时器定时模式(对应ms)*/
typedef enum{
   FAST_POLL_CYCLE = 2000,  /* */
   SLOW_POLL_CYCLE = 20000, /* */
   FAST_OFF_POLL_CYCLE = 300000,/* */
   SLEEP_POLL_CYCLE = 20 * 60 * 1000 /* 休眠之后定时器时间，20分钟*/
}CHG_POLL_MODE;

/* 充电流程的各个事件*/
#define     CHG_TIMER_EVENT                 (1 << 16)
#define     CHG_CHARGER_IN_EVENT            (1 << 17)
#define     CHG_CHARGER_OUT_EVENT           (1 << 18)
#define     CHG_POLL_TIMER_CHANGE           (1 << 19)
#define     CHG_CHGR_TYPE_CHECKING_EVENT    (1 << 20)

#define     CHG_ALL_EVENT   (CHG_TIMER_EVENT | CHG_CHARGER_IN_EVENT \
    | CHG_CHARGER_OUT_EVENT | CHG_POLL_TIMER_CHANGE \
    | CHG_CHGR_TYPE_CHECKING_EVENT)

/*----------------------------------------------*
 * 枚举定义                                       *
 *----------------------------------------------*/
/*----------------------------------------------*
 * 结构定义                                       *
 *----------------------------------------------*/
 /*通道类型对应通道号结构*/
#endif

