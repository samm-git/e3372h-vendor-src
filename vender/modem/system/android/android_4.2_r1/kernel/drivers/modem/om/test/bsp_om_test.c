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

/*****************************************************************************
  1 头文件包含
*****************************************************************************/

#include "bsp_om_api.h"
#include "bsp_om.h"

#include <linux/syscalls.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/timer.h>
#include <linux/thread_info.h>
#include <linux/semaphore.h>
#include <linux/delay.h>
#include <linux/kthread.h>


#define    THIS_MODU_ID        BSP_MODU_TEST

#define bsp_om_test_log(log_level,mod_id,fmt, ...) \
bsp_trace(log_level,mod_id,"<%s>: "fmt"\n", __FUNCTION__, ##__VA_ARGS__)

int g_test_om_init = 0;
int g_test_task_001_times = 0;
int g_test_task_002_times = 10;

u32 *p_test01 = 0;
u32 *p_test02 = 0;
u32 *p_test03 = 0;
u32 addr_test03 = 0;


void test_socp_coder_dest_rcv(void)
{

}

void bsp_om_init_test(void)
{
    if(0 == g_test_om_init)
    {
        /* register socp coder dest recv func*/

    }
}


/*LOG_TEST*/
u32 bsp_log_level_set(bsp_log_level_e log_level);


void log_test_task001(void)
{
    u32 start_time = 0;
    u32 end_time = 0;

    u32 start_time1 = 0;
    u32 end_time1 = 0;

    msleep(60000);

    bsp_log_level_set(0);

    start_time  = om_timer_get();
    for(;;)
    {
         start_time1  = om_timer_get();

        bsp_trace(BSP_LOG_LEVEL_DEBUG,BSP_MODU_LOG,"log_test1_debug\n");
        bsp_trace(BSP_LOG_LEVEL_INFO,BSP_MODU_LOG,"log_test1_info\n");
        bsp_trace(BSP_LOG_LEVEL_NOTICE,BSP_MODU_LOG,"log_test1_notic\n");
        bsp_trace(BSP_LOG_LEVEL_WARNING,BSP_MODU_LOG,"log_test1_warning\n");
        bsp_trace(BSP_LOG_LEVEL_ERROR,BSP_MODU_LOG,"log_test1_error\n");
        bsp_trace(BSP_LOG_LEVEL_CRIT,BSP_MODU_LOG,"log_test1_crit\n");
        bsp_trace(BSP_LOG_LEVEL_ALERT,BSP_MODU_LOG,"log_test1_alert\n");
        bsp_trace(BSP_LOG_LEVEL_FATAL,BSP_MODU_LOG,"log_test1_fatal\n");

        bsp_trace(BSP_LOG_LEVEL_DEBUG,BSP_MODU_LOG,"log_test1_debug\n");
        bsp_trace(BSP_LOG_LEVEL_INFO,BSP_MODU_LOG,"log_test1_info\n");
        bsp_trace(BSP_LOG_LEVEL_NOTICE,BSP_MODU_LOG,"log_test1_notic\n");
        bsp_trace(BSP_LOG_LEVEL_WARNING,BSP_MODU_LOG,"log_test1_warning\n");
        bsp_trace(BSP_LOG_LEVEL_ERROR,BSP_MODU_LOG,"log_test1_error\n");
        bsp_trace(BSP_LOG_LEVEL_CRIT,BSP_MODU_LOG,"log_test1_crit\n");
        bsp_trace(BSP_LOG_LEVEL_ALERT,BSP_MODU_LOG,"log_test1_alert\n");
        bsp_trace(BSP_LOG_LEVEL_FATAL,BSP_MODU_LOG,"log_test1_fatal\n");

        bsp_trace(BSP_LOG_LEVEL_DEBUG,BSP_MODU_LOG,"log_test1_debug\n");
        bsp_trace(BSP_LOG_LEVEL_INFO,BSP_MODU_LOG,"log_test1_info\n");
        bsp_trace(BSP_LOG_LEVEL_NOTICE,BSP_MODU_LOG,"log_test1_notic\n");
        bsp_trace(BSP_LOG_LEVEL_WARNING,BSP_MODU_LOG,"log_test1_warning\n");
        bsp_trace(BSP_LOG_LEVEL_ERROR,BSP_MODU_LOG,"log_test1_error\n");
        bsp_trace(BSP_LOG_LEVEL_CRIT,BSP_MODU_LOG,"log_test1_crit\n");
        bsp_trace(BSP_LOG_LEVEL_ALERT,BSP_MODU_LOG,"log_test1_alert\n");
        bsp_trace(BSP_LOG_LEVEL_FATAL,BSP_MODU_LOG,"log_test1_fatal\n");

        bsp_trace(BSP_LOG_LEVEL_DEBUG,BSP_MODU_LOG,"log_test1_debug\n");
        bsp_trace(BSP_LOG_LEVEL_INFO,BSP_MODU_LOG,"log_test1_info\n");
        bsp_trace(BSP_LOG_LEVEL_NOTICE,BSP_MODU_LOG,"log_test1_notic\n");
        bsp_trace(BSP_LOG_LEVEL_WARNING,BSP_MODU_LOG,"log_test1_warning\n");
        bsp_trace(BSP_LOG_LEVEL_ERROR,BSP_MODU_LOG,"log_test1_error\n");
        bsp_trace(BSP_LOG_LEVEL_CRIT,BSP_MODU_LOG,"log_test1_crit\n");
        bsp_trace(BSP_LOG_LEVEL_ALERT,BSP_MODU_LOG,"log_test1_alert\n");
        bsp_trace(BSP_LOG_LEVEL_FATAL,BSP_MODU_LOG,"log_test1_fatal\n");

        end_time1  = om_timer_get();

        g_test_task_001_times++;

        //msleep(10);

        if(g_test_task_001_times >=128)
        {
            end_time  = om_timer_get();

            bsp_om_debug(BSP_LOG_LEVEL_DEBUG,"&& log tast end: start_time = 0x%x,end_time = 0x%x",start_time,end_time);
            bsp_om_debug(BSP_LOG_LEVEL_DEBUG,"&& log tast end: start_time1 = 0x%x,end_time1 = 0x%x",start_time1,end_time1);
            return;
        }

    }
}

int log_test_task002(void * para)
{
    unsigned long value = 0;
    u32 temp = 0;

    for(;;)
    {
        local_irq_save(value);

        temp = temp*3;

        if(temp >= 0x10000)
        {
            temp = temp/3;
        }

        local_irq_restore(value);

        msleep(10);
    }

    return 0;
}


void log_test1(u32 times)
{
    u32 i;

    for(i = 0;i< times;i++)
    {
        bsp_om_test_log(BSP_LOG_LEVEL_DEBUG,BSP_MODU_LOG,"log_test1_debug : %d \n",BSP_LOG_LEVEL_DEBUG);
        bsp_om_test_log(BSP_LOG_LEVEL_INFO,BSP_MODU_LOG,"log_test1_info : %d \n",BSP_LOG_LEVEL_INFO);
        bsp_om_test_log(BSP_LOG_LEVEL_NOTICE,BSP_MODU_LOG,"log_test1_notic : %d \n",BSP_LOG_LEVEL_NOTICE);
        bsp_om_test_log(BSP_LOG_LEVEL_WARNING,BSP_MODU_LOG,"log_test1_warning: %d \n",BSP_LOG_LEVEL_WARNING);
        bsp_om_test_log(BSP_LOG_LEVEL_ERROR,BSP_MODU_LOG,"log_test1_error: %d \n",BSP_LOG_LEVEL_ERROR);
        bsp_om_test_log(BSP_LOG_LEVEL_CRIT,BSP_MODU_LOG,"log_test1_crit : %d \n",BSP_LOG_LEVEL_CRIT);
        bsp_om_test_log(BSP_LOG_LEVEL_ALERT,BSP_MODU_LOG,"log_test1_alert : %d \n",BSP_LOG_LEVEL_ALERT);
        bsp_om_test_log(BSP_LOG_LEVEL_FATAL,BSP_MODU_LOG,"log_test1_fatal : %d \n",BSP_LOG_LEVEL_FATAL);

        bsp_om_test_log(BSP_LOG_LEVEL_DEBUG,BSP_MODU_LOG,"log_test1_debug\n");
        bsp_om_test_log(BSP_LOG_LEVEL_INFO,BSP_MODU_LOG,"log_test1_info\n");
        bsp_om_test_log(BSP_LOG_LEVEL_NOTICE,BSP_MODU_LOG,"log_test1_notic\n");
        bsp_om_test_log(BSP_LOG_LEVEL_WARNING,BSP_MODU_LOG,"log_test1_warning\n");
        bsp_om_test_log(BSP_LOG_LEVEL_ERROR,BSP_MODU_LOG,"log_test1_error\n");
        bsp_om_test_log(BSP_LOG_LEVEL_CRIT,BSP_MODU_LOG,"log_test1_crit\n");
        bsp_om_test_log(BSP_LOG_LEVEL_ALERT,BSP_MODU_LOG,"log_test1_alert\n");
        bsp_om_test_log(BSP_LOG_LEVEL_FATAL,BSP_MODU_LOG,"log_test1_fatal\n");
    }



}


void log_test3(bsp_log_level_e log_level)
{
    bsp_om_test_log(BSP_LOG_LEVEL_DEBUG,BSP_MODU_ICC,"log_test1_debug\n");
}


void log_test2(void)
{

    //kthread_run(log_test_task001, NULL, "log_task001");
    (void)kthread_run(log_test_task002, NULL, "log_task002");

}


void mem_test3(u32 test_type)
{
    if(0== test_type)
    {

    }
    else if(1 == test_type)
    {

    }
    else if(2 == test_type)
    {

    }
}

void error_log_test(void)
{
    error_log("error_log_test: %d %d %d %s %d\n", 0xaa, 0xbb, 0xcc, __FUNCTION__, __LINE__);
}

void bsp_ind_test(void)
{
    u8 array[20] = {1,2,3,4,5,6,7,8,};

    bsp_log_bin_ind(0x5220,array,8);
}





