#ifndef REB_OS_ADAPTER_H
#define REB_OS_ADAPTER_H
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
    日期         修改人              问题单号                 修改内容
******************************************************************************/
#include <linux/slab.h> /*kmalloc/kfree*/
#include <linux/sched.h>
/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/
typedef void (*reb_poll_timer_callback)(void);
typedef int (*reb_task_fun)(void *para);
typedef void (*reb_hard_timer_callback)(unsigned int);
/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/


extern  void reb_msg(int print_level, const char *fmt, ...);
extern int reb_task_create( int (*Pfunc)( void * ) );
extern void reb_task_delete( void );
extern void reb_delay_ms( unsigned int delay_time_ms );
extern void reb_timer_set( unsigned int cycle_in_ms, reb_poll_timer_callback func);
extern void reb_send_msg_to_main_task( unsigned int set_event_id );
extern void reb_task_wait_for_msg( unsigned int *get_event_id,
                            const unsigned int want_event_ids );
extern void reb_lock_sleep( unsigned int sleep_id );
extern void reb_unlock_sleep( unsigned int sleep_id );

/*----------------------------------------------*
 * 函数声明                                      *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
#ifndef NULL
#define NULL (void *)0
#endif

typedef enum
{
    reb_err   = -1,
    reb_false = 0,
    reb_true  = 1
}reb_ret_val;

typedef enum
{
    REB_MSG_ERR = 0,
    REB_MSG_INFO = 1,
    REB_MSG_DEBUG = 2 /*use for debug*/
}reb_msg_log;

/*底软可靠性任务名*/
#define REB_TASK_NAME "RebTask"

/*可靠性事件ID列表*/
/*系统空闲事件*/
#define     REB_SYS_IDLE_EVENT             (1 << 0)
/*分区写越界事件*/
#define     REB_PRT_BYND_EVENT             (1 << 1)
/*病危块扫描模块初始化完成事件*/
#define     REB_NAND_SCAN_INIT_EVENT       (1 << 2)
/*电池在假关机下过放*/
#define     REB_FAST_OFF_BAT_OVERUSE       (1 << 3)


/*硬件定时器超时事件*/
#define     REB_HARD_TIMER_OUT_EVENT       (1 << 4)
/*开机启动文件系统写事件*/
#define     REB_FILE_WRITE_EVENT         (1 << 5)
/*硬件定时器停止事件*/
#define     REB_HARD_TIMER_STOP_EVENT       (1 << 6)
/*可靠性有效事件*/
#define     REB_ALL_VALID_EVENT \
(\
      REB_SYS_IDLE_EVENT  \
    | REB_PRT_BYND_EVENT \
    | REB_NAND_SCAN_INIT_EVENT \
    | REB_FAST_OFF_BAT_OVERUSE \
    | REB_HARD_TIMER_OUT_EVENT \
    | REB_FILE_WRITE_EVENT \
    | REB_HARD_TIMER_STOP_EVENT )

/*激活任务事件集,需要所有事件才能激活任务*/
#define     REB_ALL_ACTIVE_EVENT              ( REB_SYS_IDLE_EVENT )

/*内存释放的魔数字*/
#define MBB_FREE_MAGIC ( 0x5a5a5a5a )
/*内存有效地址空间*/
#define MBB_FREE_MASK ( 0xC0000000 )

#define MBB_KFREE( mem_addr )  \
    {\
        if ( MBB_FREE_MAGIC == (unsigned int) mem_addr )\
        {\
            reb_msg( REB_MSG_ERR, "REB_DFT:one memory free twice\r\n");\
        }\
        else if ( MBB_FREE_MASK != ( MBB_FREE_MASK & (unsigned int)mem_addr ) ) \
        {\
            reb_msg( REB_MSG_ERR, "REB_DFT:Invalid memory free\r\n");\
        }\
        else\
        {\
            kfree((void *)mem_addr);\
            mem_addr = (void *) MBB_FREE_MAGIC;\
        }\
    }

#ifndef TRUE
#define TRUE                  1
#endif
#ifndef FALSE
#define FALSE                 0
#endif
/*----------------------------------------------*
 * 枚举定义                                       *
 *----------------------------------------------*/
#endif
