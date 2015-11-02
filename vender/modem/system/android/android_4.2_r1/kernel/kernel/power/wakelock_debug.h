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
#ifndef __WAKELOCK_DEBUG_H
#define __WAKELOCK_DEBUG_H

#include <linux/wakelock.h>
#include <bsp_om.h>
#include <bsp_hardtimer.h>
#include <product_config.h>

#define  vote_printf(fmt, ...)    (bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_WAKELOCK, "[wakelock]: <%s> "fmt"", __FUNCTION__, ##__VA_ARGS__))


#if (FEATURE_ON == MBB_COMMON)
/*A core record lock num*/
#define  MAX_LOCK_NUM  36
#else
#define  MAX_LOCK_NUM  32
#endif /*(FEATURE_ON == MBB_COMMON)*/

struct wakelock_lookup{
    int lock_id;
    struct wake_lock *lock;
};

struct suspend_info{
    unsigned int enter_suspend_count;
    unsigned int enter_suspend_time_stamp;    
    unsigned int quit_suspend_count;
    unsigned int quit_suspend_time_stamp;    
    unsigned int enter_early_suspend_count;
    unsigned int enter_early_suspend_time_stamp;    
    unsigned int quit_early_suspend_count;    
    unsigned int quit_early_suspend_time_stamp;    
    unsigned int enter_late_resume_count;
    unsigned int enter_late_resume_time_stamp;    
    unsigned int quit_late_resume_count;    
    unsigned int quit_late_resume_time_stamp;
};
extern struct suspend_info suspend_info_record;
extern struct wakelock_lookup hi6930_wakelock[MAX_LOCK_NUM];
extern int wakelock_count;

/************************************************************************
*wakelock模块调试接口
************************************************************************/
void debug_wake_lock(int id);
void debug_wake_unlock(int id);
void debug_wakelock(void);
void debug_wake_lock_destroy(int id);

/************************************************************************
*wakelock模块自动化测试函数
************************************************************************/
int wake_lock_init_case0(void);
int wake_lock_case0(void);
int wake_unlock_case0(void);

/************************************************************************
*wakelock模块MNTN功能测试函数
************************************************************************/
void test_wakelock(int num);
void test_wakelock32(int num);

#endif

