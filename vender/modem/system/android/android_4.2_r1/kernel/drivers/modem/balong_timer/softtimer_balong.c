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
/*lint --e{537,718,746,752,958}*/
/******************************************************************************************************
     问题单号                       时间                   修改人                        说明
DTS2013070904799               2013-07-09          daizhicheng 00183683        60分钟自动关机功能底软接口提供
DTS2013090208967               2013-09-02          daizhicheng 00183683        60分钟自动关机功能底软接口bug修复
DTS2013111502011               2013-11-15          徐超 00202188               将定时器改造为同时可以让应用启动多个
DTS2013112303816               2013-11-23          徐超 00202188               给应用的接口加锁保护
DTS2013111809248               2013-12-10          wangli 00249966             清零编译告警
DTS2014010303897               2014.01.03          l00212897                   修改超时锁保护时间
DTS2014080701967               2014.08.07          张祥壮00278248              删除定时器重复代码
DTS2014080804769               2014.08.26          libing00228880              底软的special timer定时器个数和上层应用不一致
*****************************************************************************************************/
#ifdef __KERNEL__
//#include <linux/init.h>
#include <linux/irq.h>
/* Begin DTS2013070904799 daizhicheng 2013-07-09 added */
#if (FEATURE_ON == MBB_COMMON)
#include <linux/fs.h>
#include <linux/errno.h>
#include <asm/uaccess.h>
#include <linux/miscdevice.h>
#include <linux/netlink.h>
#include <linux/wakelock.h>
#endif
/* End DTS2013070904799 daizhicheng 2013-07-09 added */
#elif defined(__VXWORKS__)
#include <vxWorks.h>
#include <stdio.h>
#include <stdlib.h>
#include <taskLib.h>
#include <sysLib.h>
#endif
#include <osl_module.h>
#include <osl_sem.h>
#include <osl_spinlock.h>
#include <osl_malloc.h>
#include <soc_clk.h>
#include <bsp_softtimer.h>
#include <bsp_memmap.h>
#include <bsp_om.h>
#include <osl_thread.h>
#include "softtimer_balong.h"
#include "hardtimer_balong.h"
#ifdef __cplusplus
extern "C" 
{
#endif
/*lint --e{129, 63, 64, 409, 49, 52, 502, 574, 569 ,613 , 550} */
int  softtimer_task_func_wake(void *data);
int  softtimer_task_func_normal(void *data);
s32 show_list(u32 wake);


/* Begin DTS2013070904799 daizhicheng 2013-07-09 added */
#if (FEATURE_ON == MBB_COMMON)
#ifdef __KERNEL__
#define SPECIAL_TIMER_START             (1)
#define SPECIAL_TIMER_STOP              (0)
#define SPECIAL_TIMER_VOTE_SLEEP_TIME   (1000)
#define SPECIAL_TIMER_TIME_UNIT_SECOND  (1000)
/*用于应用起休眠能唤醒的定时器*/
struct softtimer_list s_special_softtimer;
/*用于special timer超时以后投LCD的反对休眠票，和超时后投允许休眠票*/
static struct wake_lock special_timer_lock;

/*用户态接口锁*/
spinlock_t softtimer_lock;

/* 限制应用最大同时启动定时器数目，如需增加需经过评审 */
#define SPECIAL_TIMER_NUM_MAX  (6)

/* 定时器结构体 */
struct special_timer_str
{
    unsigned int uTime; /* 定时时长 */
    int iId;           /* 定时器编号，由应用指定，定时器到时后将此编号通知应用 */
    struct softtimer_list s_special_softtimer; /* 定时器 */
};

/* 定时器结构体数组，用于存储应用设置的所有定时器 */
struct special_timer_str timer_str[SPECIAL_TIMER_NUM_MAX];
#endif
#endif /* #ifdef MBB_COMMON */
/* End DTS2013070904799 daizhicheng 2013-07-09 added */
struct softtimer_ctrl
{
    unsigned char timer_id_alloc[SOFTTIMER_MAX_NUM];              /*最多支持40个softtimer在链表中,用于分配软timerId  */
    struct list_head timer_list_head;
    u32 softtimer_start_value;                         /*记录每次物理timer计数起始值                     */
    u32 hard_timer_id_addr;                            /*软timer使用的物理timer地址                     */
    u32 hard_timer_id;                                 /*软timer使用的物理timer id                      */
     /*lint -save -e43*/
    spinlock_t  timer_list_lock;                       /*互斥访问软timer链表                            */
    osl_sem_id soft_timer_sem;                         /*硬timer中断信号量                              */
    SOFTTIMER_TASK_ID softtimer_task;                  /*记录创建的任务                                 */
    u32 clk;                                           /*所使用物理timer的时钟频率                       */
};

static struct softtimer_ctrl timer_control[2];         /*timer_control[0] wake, timer_control[1] normal*/

static void start_hard_timer(u32 wake_type,u32 ulvalue )
{
    timer_control[wake_type].softtimer_start_value = ulvalue;
    bsp_hardtimer_disable(timer_control[wake_type].hard_timer_id);
    bsp_hardtimer_load_value(timer_control[wake_type].hard_timer_id,ulvalue);
    bsp_hardtimer_enable(timer_control[wake_type].hard_timer_id);
}

static void stop_hard_timer(u32 wake_type,u32 timer_id)
{
    bsp_hardtimer_disable(timer_id);
    timer_control[wake_type].softtimer_start_value = ELAPESD_TIME_INVAILD;
}
static u32 hard_timer_elapsed_time(u32 wake_type)
{
    u32 ulTempValue = 0;
    if ((u32)ELAPESD_TIME_INVAILD == timer_control[wake_type].softtimer_start_value)
    {
        return 0;
    }
    ulTempValue = bsp_get_timer_current_value(timer_control[wake_type].hard_timer_id);
    return ((timer_control[wake_type].softtimer_start_value < ulTempValue) ? 0 : (timer_control[wake_type].softtimer_start_value - ulTempValue+1));
}
/*lint --e{438,564}*/
void bsp_softtimer_add(struct softtimer_list * timer)
{
    u32 elapsed_time = 0;
    struct softtimer_list *p = NULL;
    unsigned long flags = 0;
    if (NULL == timer)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR,BSP_MODU_SOFTTIMER,"timer to be added is NULL\n");
        return;
    }
    spin_lock_irqsave(&(timer_control[timer->wake_type].timer_list_lock),flags);
    /*如果已经在超时链表中，则不插入*/
    if(!list_empty(&timer->entry))
    {
    	 spin_unlock_irqrestore(&(timer_control[timer->wake_type].timer_list_lock),flags);
    	 return;
    }
    timer->timeout = timer->count_num;
    elapsed_time = hard_timer_elapsed_time(timer->wake_type);
    timer->timeout += elapsed_time;
    /*如果timer链表为空，则插入头结点*/
    if (list_empty(&(timer_control[timer->wake_type].timer_list_head)))
    {
        list_add_tail(&(timer->entry),&(timer_control[timer->wake_type].timer_list_head));
    }
    /*如果链表不为空，则按照超时时间从小到大插入*/
    else
    {
    	  /*lint -save -e413 *//*lint -save -e613*/
        p=list_first_entry(&(timer_control[timer->wake_type].timer_list_head),struct softtimer_list,entry);
        if((TIMER_TRUE==p->is_running)&&(0==elapsed_time))
        {
            timer->timeout += p->timeout;
        }
	    if(elapsed_time!=0)
    	{
    	     timer->timeout -= 1;
	         elapsed_time     -= 1;
    	}
        list_for_each_entry(p,&(timer_control[timer->wake_type].timer_list_head),entry)
        {
            if(p->timeout <= timer->timeout)
	        {
	            timer->timeout -= p->timeout;
	        }
	        else
	        {
	            break;
	        }
	    }
	    if(&(p->entry)!=&(timer_control[timer->wake_type].timer_list_head))
	    {
	        p->timeout -= timer->timeout;
	    }
	    list_add_tail(&(timer->entry),&(p->entry));
    }
    if (timer_control[timer->wake_type].timer_list_head.next == &(timer->entry))
    {
        timer->timeout -= elapsed_time;
	 if ((timer->entry.next)!=(&(timer_control[timer->wake_type].timer_list_head)))
	 {
	     p = list_entry(timer->entry.next,struct softtimer_list,entry);
	     if(TIMER_TRUE==p->is_running)
	     {
	         p->is_running = TIMER_FALSE;
	     }
	 }
	 timer->is_running = TIMER_TRUE;
	 start_hard_timer(timer->wake_type,timer->timeout);
    }
    spin_unlock_irqrestore(&(timer_control[timer->wake_type].timer_list_lock),flags);
}
s32 bsp_softtimer_delete(struct softtimer_list * timer)
{
    struct softtimer_list * p=NULL;
    unsigned long flags;
    if (NULL == timer)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR,BSP_MODU_SOFTTIMER,"NULL pointer \n");
        return ERROR;
    }
    spin_lock_irqsave(&(timer_control[timer->wake_type].timer_list_lock),flags);
    if (list_empty(&timer->entry))
    {
        spin_unlock_irqrestore(&(timer_control[timer->wake_type].timer_list_lock),flags);
        return NOT_ACTIVE;
    }
    else
    {
        if(timer->entry.next == &(timer_control[timer->wake_type].timer_list_head))/*如果删除的是最后一个节点，直接删除*/
        {
            timer->is_running = TIMER_FALSE;
            list_del_init(&(timer->entry));
        }
	 /*如果删除的是链表中第一个结点,并且其后还有其他结点*/
        else if((timer->entry.prev == &(timer_control[timer->wake_type].timer_list_head))
			&&(timer->entry.next != &(timer_control[timer->wake_type].timer_list_head)))
        {
            timer->is_running = TIMER_FALSE;
            list_del_init(&(timer->entry));
			p=list_first_entry(&(timer_control[timer->wake_type].timer_list_head),struct softtimer_list,entry);
			p->timeout += timer->timeout - hard_timer_elapsed_time((u32)timer->wake_type);
            start_hard_timer(p->wake_type,p->timeout);
			p->is_running = TIMER_TRUE;
        }
	 /*如果删除的是中间节点*/
        else
        {
            p = list_entry(timer->entry.next,struct softtimer_list,entry);
	        p->timeout += timer->timeout;
	        timer->is_running = TIMER_FALSE;
	        list_del_init(&(timer->entry));
        }
    }
    if (list_empty(&(timer_control[timer->wake_type].timer_list_head)))/*如果删除完事后链表为空，则停止计数*/
    {
        stop_hard_timer(timer->wake_type,timer_control[timer->wake_type].hard_timer_id);
    }
    spin_unlock_irqrestore(&(timer_control[timer->wake_type].timer_list_lock),flags);
    return OK;
}

s32 bsp_softtimer_delete_sync(struct softtimer_list * timer)
{
     return bsp_softtimer_delete(timer);
}

s32 bsp_softtimer_modify(struct softtimer_list *timer,u32 new_expire_time)
{
    if((NULL == timer)||(!list_empty(&timer->entry)) )
    {
        return ERROR;
    }
    if(WAKE_SOURCE_CLK==timer_control[timer->wake_type].clk)
    {
	    if((new_expire_time) < (SOFTTIMER_MAX_LENGTH)) 
	    {
	        timer->timeout= (WAKE_SOURCE_CLK* new_expire_time)/1000;
	    }
	    else /* 防止乘法溢出 */ 
	    {
	        timer->timeout= WAKE_SOURCE_CLK * (new_expire_time/1000);
	    }
	    timer->count_num = timer->timeout;
    }
     else if(NOWAKE_SOURCE_CLK==timer_control[timer->wake_type].clk)
    {
	    timer->timeout= (NOWAKE_SOURCE_CLK/1000)* new_expire_time;
	    timer->count_num = timer->timeout;
    }
    return OK;
}

s32 bsp_softtimer_create(struct softtimer_list *sft_info)
{
    u32 i = 0;
      /*lint -save -e685*/    
    if (check_timer_type((u32)sft_info->wake_type))
	{
		bsp_trace(BSP_LOG_LEVEL_ERROR,BSP_MODU_SOFTTIMER,"wake type is error\n");
		return ERROR;
	}
	if(sft_info->init_flags==TIMER_INIT_FLAG)
		return ERROR;
      /*lint -restore +e685*/  
    INIT_LIST_HEAD(&(sft_info->entry));
    sft_info->is_running = TIMER_FALSE;
    sft_info->init_flags=TIMER_INIT_FLAG;
    if(WAKE_SOURCE_CLK==timer_control[sft_info->wake_type].clk)
    {
    	/*如果超出最大值，则返回失败,单位为ms*/
		if(sft_info->timeout>SOFTTIMER_MAX_LENGTH*1000)
		{
			bsp_trace(BSP_LOG_LEVEL_ERROR,BSP_MODU_SOFTTIMER,"time too long ,not support\n");
			return ERROR;
		}
		if((sft_info->timeout) < SOFTTIMER_MAX_LENGTH)  /* 对小于SOFTTIMER_MAX_LENGTH的计时，可以保持精度 */
		{
			sft_info->timeout= (WAKE_SOURCE_CLK*(sft_info->timeout))/1000;
		}
		else /* 防止乘法溢出 */ 
		{
			sft_info->timeout= WAKE_SOURCE_CLK* ((sft_info->timeout)/1000);
		}	
		sft_info->count_num = sft_info->timeout;
	}
	else if(NOWAKE_SOURCE_CLK==timer_control[sft_info->wake_type].clk)
	{
		/*如果超出最大值，则返回失败,单位为ms*/
		if(sft_info->timeout>SOFTTIMER_MAX_LENGTH_NORMAL*1000)
		{
			bsp_trace(BSP_LOG_LEVEL_ERROR,BSP_MODU_SOFTTIMER,"time too long ,not support\n");
			return ERROR;
		}
		sft_info->timeout= (NOWAKE_SOURCE_CLK/1000)* (sft_info->timeout);
		sft_info->count_num = sft_info->timeout;
	}
	for (i=0 ;i < SOFTTIMER_MAX_NUM; i++)
	{
		if (timer_control[sft_info->wake_type].timer_id_alloc[i] == 0)
		{
			sft_info->timer_id = i;
		timer_control[sft_info->wake_type].timer_id_alloc[i] = 1;
		break;
		}
	}
	if (SOFTTIMER_MAX_NUM == i)
	{
		bsp_trace(BSP_LOG_LEVEL_ERROR,BSP_MODU_SOFTTIMER,"error,not enough timerid for alloc, already 40 exists\n");
		return ERROR;
	}
	return OK;
}
/*lint -restore +e685*/ 

s32 bsp_softtimer_free(struct softtimer_list *p)
{
	 if ((NULL == p)||(!list_empty(&p->entry)))
	{
		return ERROR;
	}
	p->init_flags = 0;
	 timer_control[p->wake_type].timer_id_alloc[p->timer_id] = 0;
	 return OK;   
}

int  softtimer_task_func(void* data)
{
	u32  timer_id = (u32) data;
	struct softtimer_list     *p = NULL;
	unsigned long flags;
	softtimer_func func =NULL;
	u32 para = 0;

	/* coverity[no_escape] */
	for( ; ; )
	{
		/* coverity[sleep] */
		osl_sem_down(&(timer_control[timer_id].soft_timer_sem));
		 /* coverity[lock_acquire] */
		spin_lock_irqsave(&(timer_control[timer_id].timer_list_lock),flags);
		timer_control[timer_id].softtimer_start_value = ELAPESD_TIME_INVAILD;
		if (!list_empty(&(timer_control[timer_id].timer_list_head)))
		{
			p = list_first_entry(&(timer_control[timer_id].timer_list_head),struct softtimer_list,entry);
			if(p->is_running == TIMER_TRUE)
			{
				list_del_init(&p->entry);
				p->is_running = TIMER_FALSE;
				func = p->func;
				para = p->para;
				spin_unlock_irqrestore(&(timer_control[timer_id].timer_list_lock),flags); 
				func(para);
				spin_lock_irqsave(&(timer_control[timer_id].timer_list_lock),flags);
				while(!list_empty(&(timer_control[timer_id].timer_list_head)))
				{
					p=list_first_entry(&(timer_control[timer_id].timer_list_head),struct softtimer_list,entry);
					if(0==p->timeout)
					{
						list_del_init(&p->entry);
						p->is_running = TIMER_FALSE;
						func = p->func;
						para = p->para;
						spin_unlock_irqrestore(&(timer_control[timer_id].timer_list_lock),flags); 
						func(para);
						spin_lock_irqsave(&(timer_control[timer_id].timer_list_lock),flags);
					}
					else
						break;
				}
				if (!list_empty(&(timer_control[timer_id].timer_list_head)))/*如果还有未超时定时器*/
				{
					p=list_first_entry(&(timer_control[timer_id].timer_list_head),struct softtimer_list,entry);
					p->is_running = TIMER_TRUE;
					start_hard_timer(timer_id,p->timeout);
				}
				else 
				{  
					stop_hard_timer(timer_id,timer_control[timer_id].hard_timer_id);
				}
			}
			else  if (p->is_running == TIMER_FALSE)
			{
				p->is_running = TIMER_TRUE;
				start_hard_timer(timer_id,p->timeout);
			}
		}
		else
		{
			stop_hard_timer(timer_id,timer_control[timer_id].hard_timer_id);
		}
		spin_unlock_irqrestore(&(timer_control[timer_id].timer_list_lock),flags); 
	} 
	/*lint -save -e527*/ 
	return 0;
	/*lint -restore +e527*/ 
}


OSL_IRQ_FUNC(static irqreturn_t,softtimer_interrupt_call_back,irq,dev)
{    
	u32  timer_id = (u32) dev;
    u32 readValue = 0;    
    /*1、读取硬件定时器的中断状态
	 2、如果有中断，则清中断，同时释放信号量
   */
    readValue = bsp_hardtimer_int_status(timer_control[timer_id].hard_timer_id);
    if (0 != readValue)
    {
        bsp_hardtimer_int_clear(timer_control[timer_id].hard_timer_id);
        osl_sem_up(&(timer_control[timer_id].soft_timer_sem));
    }
    return IRQ_HANDLED;
}


int  bsp_softtimer_init(void)
{
    s32 ret = 0;
    u32 array_size=0 ;
    /* coverity[var_decl] */
    struct bsp_hardtimer_control timer_ctrl ;
    INIT_LIST_HEAD(&(timer_control[SOFTTIMER_WAKE].timer_list_head));
    INIT_LIST_HEAD(&(timer_control[SOFTTIMER_NOWAKE].timer_list_head));
    timer_control[SOFTTIMER_NOWAKE].hard_timer_id_addr = ST_NORMAL_HARDTIMER_ADDR;
    timer_control[SOFTTIMER_WAKE].hard_timer_id_addr     = ST_WAKE_HARDTIMER_ADDR;
    timer_control[SOFTTIMER_NOWAKE].hard_timer_id      = ST_NORMAL_HARDTIMER_ID;
    timer_control[SOFTTIMER_WAKE].hard_timer_id          = ST_WAKE_HARDTIMER_ID;
    timer_control[SOFTTIMER_NOWAKE].clk                = NOWAKE_TIMER_CLK;
    timer_control[SOFTTIMER_WAKE].clk                    = WAKE_TIMER_CLK;
    array_size = sizeof(u8)*SOFTTIMER_MAX_NUM;/*lint !e419 */
    memset(timer_control[SOFTTIMER_WAKE].timer_id_alloc,0,array_size);/*lint !e419 */
    memset(timer_control[SOFTTIMER_NOWAKE].timer_id_alloc,0,array_size);/*lint !e419 */
    timer_control[SOFTTIMER_WAKE].softtimer_start_value  = ELAPESD_TIME_INVAILD;
    timer_control[SOFTTIMER_NOWAKE].softtimer_start_value = ELAPESD_TIME_INVAILD;
    osl_sem_init(SEM_EMPTY,&(timer_control[SOFTTIMER_NOWAKE].soft_timer_sem));
    osl_sem_init(SEM_EMPTY,&(timer_control[SOFTTIMER_WAKE].soft_timer_sem));
    spin_lock_init(&(timer_control[SOFTTIMER_WAKE].timer_list_lock));
    spin_lock_init(&(timer_control[SOFTTIMER_NOWAKE].timer_list_lock));
    timer_ctrl.func=(timer_func)softtimer_interrupt_call_back;
    timer_ctrl.mode=TIMER_ONCE_COUNT;
    if (OK==check_timer_type((u32)SOFTTIMER_WAKE))
    {
        if(ERROR == osl_task_init("softtimer_wake", TIMER_TASK_WAKE_PRI, TIMER_TASK_STK_SIZE ,(void *)softtimer_task_func, (void*)SOFTTIMER_WAKE,
			&timer_control[SOFTTIMER_WAKE].softtimer_task))
        {
            bsp_trace(BSP_LOG_LEVEL_ERROR,BSP_MODU_SOFTTIMER,"softtimer_wake task create failed\n");
            return ERROR;
        }
        timer_ctrl.para=SOFTTIMER_WAKE;
        timer_ctrl.timerId =ST_WAKE_HARDTIMER_ID;
        ret =bsp_hardtimer_alloc(&timer_ctrl);
        if (ret)
        {
            bsp_trace(BSP_LOG_LEVEL_ERROR,BSP_MODU_SOFTTIMER,"bsp_hardtimer_alloc error,softtimer init failed 2\n");
            return ERROR;
        }
    }
     if (OK==check_timer_type((u32)SOFTTIMER_NOWAKE))
     {
         if(ERROR == osl_task_init("softtimer_nowake", TIMER_TASK_NOWAKE_PRI, TIMER_TASK_STK_SIZE ,(void *)softtimer_task_func, (void*)SOFTTIMER_NOWAKE,
			&timer_control[SOFTTIMER_NOWAKE].softtimer_task))
            {
                bsp_trace(BSP_LOG_LEVEL_ERROR,BSP_MODU_SOFTTIMER,"softtimer_normal task create failed\n");
                return ERROR;
            }
        timer_ctrl.para=SOFTTIMER_NOWAKE;
        timer_ctrl.timerId =ST_NORMAL_HARDTIMER_ID;
	/* coverity[uninit_use_in_call] */
         ret =bsp_hardtimer_alloc(&timer_ctrl);
        if (ret)
        {
            bsp_trace(BSP_LOG_LEVEL_ERROR,BSP_MODU_SOFTTIMER,"bsp_hardtimer_alloc error,softtimer init failed 2\n");
            return ERROR;
        }
     }
     bsp_trace(BSP_LOG_LEVEL_ERROR,BSP_MODU_SOFTTIMER,"softtimer init success\n");
    return OK;
}
/*lint -restore +e550*/

/*lint -save -e413*/

s32 show_list(u32 wake_type)
{
    struct softtimer_list * timer = NULL;
    unsigned long flags = 0;
    bsp_trace(BSP_LOG_LEVEL_ERROR,BSP_MODU_SOFTTIMER,"timer_id\t timeout \t time_count_num \t is_running\n");
    bsp_trace(BSP_LOG_LEVEL_ERROR,BSP_MODU_SOFTTIMER,"----------------------------------------------------------------------------------\n");
    spin_lock_irqsave(&(timer_control[wake_type].timer_list_lock),flags); 
    list_for_each_entry(timer,&(timer_control[wake_type].timer_list_head),entry)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR,BSP_MODU_SOFTTIMER,"%d \t %d \t\t %d \t\t %d\n",timer->timer_id,timer->timeout,timer->count_num,timer->is_running);
    }
     spin_unlock_irqrestore(&(timer_control[wake_type].timer_list_lock),flags); 
    return OK;
}
/*lint -restore +e413*/

/*lint -save -e19*/



/* Begin DTS2013070904799 daizhicheng 2013-07-09 added */
#if (FEATURE_ON == MBB_COMMON)
#ifdef __KERNEL__
/*****************************************************************************
* 函 数 名  : special_timer_id_search
* 功能描述  :  查询已有定时器的下标，如果没有找到则返回可用下标，如果都没有则返回-1
* 输入参数  : int id：需要查询的定时器ID，该ID由应用通过ioctl设置
* 输出参数  : 无
* 返 回 值  : 有可用下标则返回下标，无可用位置则返回-1
* 其它说明  : 无
*****************************************************************************/
int special_timer_id_search(int id)
{
    int i = 0;
    /* 首先找id相同的，如果找到了则说明已经有这个定时器了，返回该定时器在数组中的位置 */
    for(i = 0; i < SPECIAL_TIMER_NUM_MAX; i++)
    {
        if(id == timer_str[i].iId )
        {
            return i;
        }
    }

    /* ID没有相同的说明需要新建，找到空闲位置并返回下标 */
    for(i = 0; i < SPECIAL_TIMER_NUM_MAX; i++)
    {
        if(TIMER_INIT_FLAG != timer_str[i].s_special_softtimer.init_flags)
        {
            return i;
        }
    }

    /* 如果ID即没有在已有定时器中找到，也没有空闲的位置，则返回-1 */
    return -1;
}

/*****************************************************************************
* 函 数 名  : special_timer_cb
* 功能描述  : Special_timer 超时处理函数
*                               借用LCD投反对票，起1秒定时器，超时后投LCD允许休眠票
*                               通过netlink上报上层定时器超时事件
* 输入参数  : 
* 输出参数  : 无
* 返 回 值  : 无
* 其它说明  : 无
*****************************************************************************/
/*lint -e10 -e64 */
int special_timer_cb(int temp)
{
    int ret = -1;
    int size = -1;
    BSP_S32 timer_num = 0;
    DEVICE_EVENT *event = NULL;

    char buff[sizeof(DEVICE_EVENT) + sizeof(int)];

    /* 超时wakelock锁 */
    wake_lock_timeout(&special_timer_lock, (long)msecs_to_jiffies(SPECIAL_TIMER_VOTE_SLEEP_TIME));

    /* restart the timer */
    timer_num = special_timer_id_search(temp);
    if( 0 > timer_num)
    {
        printk(KERN_ERR "[special_timer] Special timer is not exist, id by app is %d !\n",temp);
        return -1;
    }

    if( TIMER_INIT_FLAG == timer_str[timer_num].s_special_softtimer.init_flags)
    {
        bsp_softtimer_add(&(timer_str[timer_num].s_special_softtimer));
    }

    event = (DEVICE_EVENT *)buff;
    size =  sizeof(buff);

    event->device_id = DEVICE_ID_TIMER;      /* 设备ID */
    event->event_code = DEVICE_TIMEROU_F;    /* 事件代码 */
    event->len = sizeof(int);
    memcpy(event->data, &temp, sizeof(int));

    /* 超时事件上报 */
    ret = device_event_report(event, size);
    /* Begin DTS2013090208967 daizhicheng 2013-09-02 modified */
    printk( KERN_EMERG "softtimer event: id=%d, code=%d, len=%d, data=%d\n",
        event->device_id, event->event_code, event->len, *((int *)(event->data)));
    /* End DTS2013090208967 daizhicheng 2013-09-02 modified */

    if (-1 == ret) 
    {
        printk(KERN_ERR "special_timer_cb device_event_report fail!\n");
    }

    return ret;
}

/*****************************************************************************
* 函 数 名  : special_timer_start_func
* 功能描述  : 起special timer定时器
* 输入参数  : 
* 输出参数  : 无
* 返 回 值  : BSP_OK--创建成功；
                           BSP_ERROR--创建失败；
* 其它说明  : 无
*****************************************************************************/
BSP_S32 special_timer_start_func(struct special_timer_str special_timer_par)
{
    unsigned int uTimeTemp = 0;
    BSP_S32 timer_num = 0;
    s32 ret = 0;

    uTimeTemp = special_timer_par.uTime * SPECIAL_TIMER_TIME_UNIT_SECOND;

    printk(KERN_ERR "[special_timer] Timer create start, ID  is  %d \n", special_timer_par.iId);

    timer_num = special_timer_id_search(special_timer_par.iId);
    if( 0 > timer_num)
    {
        printk(KERN_EMERG "[special_timer] Special timer is full, creat not allowed!\n");
        return -1;
    }

    if(TIMER_INIT_FLAG == timer_str[timer_num].s_special_softtimer.init_flags)
    {
        printk(KERN_EMERG "[special_timer] Special timer is already created, creat again not allowed!\n");
        return -1;
    }

    /* 填充定时器信息 */
    timer_str[timer_num].s_special_softtimer.func = (softtimer_func)special_timer_cb;     /* 超时回调 */
    timer_str[timer_num].s_special_softtimer.para = special_timer_par.iId;/* 定时器编号 */
    timer_str[timer_num].s_special_softtimer.timeout = uTimeTemp;         /* 超时时间 */
    timer_str[timer_num].s_special_softtimer.wake_type = SOFTTIMER_WAKE;                 /* 可唤醒定时器 */

    /* 创建定时器 */
    ret = bsp_softtimer_create(&(timer_str[timer_num].s_special_softtimer));
    if (ret)
    {
        printk(KERN_ERR "create vote_sleep softtimer failed \n");
        /*Begin DTS2013090208967 daizhicheng 2013-09-02 added */
        return -1;
        /*End DTS2013090208967 daizhicheng 2013-09-02 added */
    }
    /* 添加到定时器列表中，并启动该定时器 */
    timer_str[timer_num].iId = special_timer_par.iId;
    timer_str[timer_num].uTime = special_timer_par.uTime;
    bsp_softtimer_add(&(timer_str[timer_num].s_special_softtimer));
    printk(KERN_EMERG "[special_timer] Creat add start special timer sucess, timerID is %d \n",
        timer_str[timer_num].s_special_softtimer.timer_id);
    
    return 0;
}

/*****************************************************************************
* 函 数 名  : special_timer_stop_func
* 功能描述  : 停止special timer定时器
* 输入参数  : 
* 输出参数  : 无
* 返 回 值  : BSP_OK--创建成功；
                           BSP_ERROR--创建失败；
* 其它说明  : 无
*****************************************************************************/
BSP_S32 special_timer_stop_func(struct special_timer_str special_timer_par)
{
    BSP_S32 timer_num = 0;
    struct softtimer_list *  softtimer;

    timer_num = special_timer_id_search(special_timer_par.iId);
    if( 0 > timer_num )
    {
        printk(KERN_ERR "[special_timer] Timer delete end, No such ID in the list!\n");
        return -1;
    }

    softtimer = &(timer_str[timer_num].s_special_softtimer);

    printk(KERN_ERR "[special_timer] Timer delete start, ID  is  %d \n", special_timer_par.iId);

    if ( TIMER_INIT_FLAG == softtimer->init_flags )
    {
        printk(KERN_EMERG "Before delete, timerID  is  %d \n", softtimer->timer_id);
        /* 从超时列表中删除该定时器 */
        if(0 > bsp_softtimer_delete(softtimer))
        {
            /* Begin DTS2013090208967 daizhicheng 2013-09-02 modified */
            printk(KERN_EMERG "Delete special_timer fail,timerID is %d !\n", softtimer->timer_id);
            /* End DTS2013090208967 daizhicheng 2013-09-02 modified */
            return -1;
        }
        /* 释放定时器 */
        if(0 != bsp_softtimer_free(softtimer))
        {
            printk(KERN_EMERG "Free special_timer fail!\n");
            return -1;
        }
        timer_str[timer_num].iId = -1;
        timer_str[timer_num].uTime = -1;
    }
    else
    {
        printk(KERN_ERR "[special_timer] Timer delete end, already deleted!\n");
    }

    return 0;
}

/*****************************************************************************
* 函 数 名  : balong_special_timer_open
* 功能描述  : Special_timer open处理函数
                               预留，目前没使用
* 输入参数  : 
* 输出参数  : 无
* 返 回 值  : 
* 其它说明  : 无
*****************************************************************************/
BSP_S32 balong_special_timer_open(struct inode *inode, struct file *file)
{
    if (NULL != inode && NULL != file)
    {
        ;//for pclint
    }
    return 0;
}

/*****************************************************************************
* 函 数 名  : balong_special_timer_release
* 功能描述  : Special_timer release处理函数
                               预留，目前没使用
* 输入参数  : 
* 输出参数  : 无
* 返 回 值  : 
* 其它说明  : 无
*****************************************************************************/
BSP_S32 balong_special_timer_release(struct inode *inode, struct file *file)
{
    if (NULL != inode && NULL != file)
    {
        ;//for pclint
    }
    return 0;
}

/*****************************************************************************
* 函 数 名  : balong_special_timer_read
* 功能描述  : Special_timer read处理函数
                               预留，目前没使用
* 输入参数  : 
* 输出参数  : 无
* 返 回 值  : 
* 其它说明  : 无
*****************************************************************************/
BSP_S32 balong_special_timer_read(struct file *file, char __user * buffer, 
        size_t count, loff_t *ppos)
{
    if (NULL != file)
    {
        ; //for pclint
    }

    return 0;
}

/*****************************************************************************
* 函 数 名  : balong_special_timer_write
* 功能描述  :字符设备写函数，预留
* 输入参数  : 
* 输出参数  : 无
* 返 回 值  : 
* 其它说明  : 无
*****************************************************************************/
BSP_S32 balong_special_timer_write(struct file *file, const char __user *buf, 
        size_t count,loff_t *ppos)
{
    if (NULL == file && NULL == ppos)
    {
        ;//for pclint
    }

    return 0;
}

/*****************************************************************************
* 函 数 名  : balong_special_timer_ioctl
* 功能描述  : Special_timer ioctrl处理函数
* 输入参数  : 
* 输出参数  : 无
* 返 回 值  : BSP_OK:    操作成功
*             BSP_ERROR: 操作失败
* 其它说明  : 无
*****************************************************************************/
/*Begin DTS2013090208967 daizhicheng 2013-09-02 added */
long balong_special_timer_ioctl(struct file *file, unsigned int bStart, unsigned long arg)
/*End DTS2013090208967 daizhicheng 2013-09-02 added */
{
    BSP_S32 err_code = 0;
    unsigned long flag = 0;
    struct special_timer_str special_timer_par; /* 用户存储用户态传递的参数信息 */

    /*Begin DTS2013090208967 daizhicheng 2013-09-02 added */
    if (NULL == file)
    /*End DTS2013090208967 daizhicheng 2013-09-02 added */
    {
        ; //for pclint
    }
    spin_lock_irqsave(&softtimer_lock, flag);

    /* 从用户态读取参数信息 */
    if (copy_from_user(&special_timer_par, (void *)arg, sizeof(struct special_timer_str)))
    {
        printk(KERN_ERR "[special_timer] copy_from_user failed!\n");
        err_code = -1;
        goto end;
    }

    /* 启动定时器 */
    if(SPECIAL_TIMER_START == bStart)
    {
        err_code = special_timer_start_func(special_timer_par);
        if(0 != err_code)
        {
            /*Begin DTS2013090208967 daizhicheng 2013-09-02 added */
            printk(KERN_EMERG "balong_special_timer_ioctll() start timer error---!!!\n");
            /*End DTS2013090208967 daizhicheng 2013-09-02 added */
            goto end;
        }
    }
    /* 停止定时器 */
    else if(SPECIAL_TIMER_STOP == bStart)
    {
        err_code = special_timer_stop_func(special_timer_par);
        if(0 != err_code)
        {
            /*Begin DTS2013090208967 daizhicheng 2013-09-02 added */
            printk(KERN_EMERG "balong_special_timer_ioctll() stop timer error---!!!\n");
            /*End DTS2013090208967 daizhicheng 2013-09-02 added */
            goto end;
        }
    }
    else
    {
        printk(KERN_EMERG "param bStart invalid, bStart=%d!\n",bStart);
        err_code = -1;
        goto end;
    }
    /*Begin DTS2013090208967 daizhicheng 2013-09-02 modified */
    /*printk(KERN_EMERG "balong_special_timer_ioctll() leave-------!!!\n");*/
    /*End DTS2013090208967 daizhicheng 2013-09-02 modified */
end:
    spin_unlock_irqrestore(&softtimer_lock, flag);
    return (long)err_code;
}

/*special timer函数数据结构*/
/*lint -e527 */
struct file_operations balong_special_timer_fops = {
    .owner = THIS_MODULE,
    .read = balong_special_timer_read,
    .write = balong_special_timer_write,
    .unlocked_ioctl = balong_special_timer_ioctl,
    .open = balong_special_timer_open,
    .release = balong_special_timer_release,
};
/*lint +e527 */

/*balong_special_timer_miscdev作为调用misc_register函数的参数，
  用于向linux内核注册special timer(硬timer)misc设备。
*/
static struct miscdevice balong_special_timer_miscdev = {
    .name = "special_timer",
    .minor = MISC_DYNAMIC_MINOR,/*动态分配子设备号（minor）*/
    .fops = &balong_special_timer_fops,
};

/*****************************************************************************
* 函 数 名  : balong_special_timer_init
*
* 功能描述  : Special_timer A核模块初始化
*
* 输入参数  : 
* 输出参数  : 无
*
* 返 回 值  : BSP_OK:    操作成功
*             BSP_ERROR: 操作失败
*
* 其它说明  : 无
*
*****************************************************************************/
BSP_S32 balong_special_timer_init(void)
{
    int ret = 0;
    int i = 0;

    /* 初始化wakelock锁 */
    wake_lock_init(&special_timer_lock , WAKE_LOCK_SUSPEND, "special_timer_lock");
    printk("------special_timer wakelock init ok ----------!\n");
    
    /* 注册misc设备，/dev/special_timer */
    ret = misc_register(&balong_special_timer_miscdev);
    if (0 != ret)
    {
        printk("------special_timer misc register fail!\n");
        return ret;
    }

    /* 初始化定时器数组, 存储应用设置的定时器，支持同时启动多个定时器 */
    for(i = 0; i < SPECIAL_TIMER_NUM_MAX; i++)
    {
        timer_str[i].iId = -1;
        timer_str[i].uTime = -1;
    }

    spin_lock_init(&softtimer_lock);
    printk("------special_timer misc register leave!\n");
    return BSP_OK;
}
/*lint -esym(529,balong_special_timer_init,__initcall_balong_special_timer_init6)*/
module_init(balong_special_timer_init);
/*lint +e10 +e64 */
#endif
#endif
/* End DTS2013070904799 daizhicheng 2013-07-09 added */

EXPORT_SYMBOL(bsp_softtimer_create);
EXPORT_SYMBOL(bsp_softtimer_delete);
EXPORT_SYMBOL(bsp_softtimer_delete_sync);
EXPORT_SYMBOL(bsp_softtimer_modify);
EXPORT_SYMBOL(bsp_softtimer_add);
EXPORT_SYMBOL(bsp_softtimer_free);
arch_initcall(bsp_softtimer_init);

/*lint -restore +e19*/

/*lint -restore +e43*/
/*lint -restore +e527*/
/*lint -restore +e413*//*lint -restore +e613*/


#ifdef __cplusplus
}
#endif


