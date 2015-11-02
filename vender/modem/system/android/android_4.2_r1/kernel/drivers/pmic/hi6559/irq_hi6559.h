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
#ifndef __HI6559_IRQ_H
#define __HI6559_IRQ_H

#include <osl_types.h>
#include <linux/workqueue.h>
#include "osl_spinlock.h"

#define INT_LVL_SMART   31
/* number of interrupt*/
#define HI6559_IRQ_NR   21

#define IRQ_HI6559_NR_BASE                      0
#define IRQ_HI6559_OTMP                         (IRQ_HI6559_NR_BASE + 0)    /* 0x08寄存器 */
#define IRQ_HI6559_VSYS_UNDER_2P5               (IRQ_HI6559_NR_BASE + 1)
#define IRQ_HI6559_VSYS_UNDER_2P85              (IRQ_HI6559_NR_BASE + 2)
#define IRQ_HI6559_VSYS_OVER_6P0                (IRQ_HI6559_NR_BASE + 3)
#define IRQ_HI6559_POWER_KEY_4S_PRESS           (IRQ_HI6559_NR_BASE + 4)
#define IRQ_HI6559_POWER_KEY_20MS_RELEASE       (IRQ_HI6559_NR_BASE + 5)
#define IRQ_HI6559_POWER_KEY_20MS_PRESS         (IRQ_HI6559_NR_BASE + 6)
#define IRQ_HI6559_OCP_SCP                      (IRQ_HI6559_NR_BASE + 7)    /* 0x09寄存器 */
#define IRQ_HI6559_VBUS_DET_1P375_90US          (IRQ_HI6559_NR_BASE + 8)
#define IRQ_HI6559_VBUS_DET_UNDER_0P9_3MS       (IRQ_HI6559_NR_BASE + 9)
#define IRQ_HI6559_VBUS_DET_OVER_0P9_3MS        (IRQ_HI6559_NR_BASE + 10)
#define IRQ_HI6559_ALARM_ON_A                   (IRQ_HI6559_NR_BASE + 11)   /* 0x0A寄存器 */
#define IRQ_HI6559_ALARM_ON_B                   (IRQ_HI6559_NR_BASE + 12)
#define IRQ_HI6559_ALARM_ON_C                   (IRQ_HI6559_NR_BASE + 13)
#define IRQ_HI6559_ALARM_ON_D                   (IRQ_HI6559_NR_BASE + 14)
#define IRQ_HI6559_ALARM_ON_E                   (IRQ_HI6559_NR_BASE + 15)
#define IRQ_HI6559_ALARM_ON_F                   (IRQ_HI6559_NR_BASE + 16)
#define IRQ_HI6559_SIM_HPD_OUT_FALL             (IRQ_HI6559_NR_BASE + 17)    /* 0x0B寄存器 */
#define IRQ_HI6559_SIM_HPD_OUT_RAISE            (IRQ_HI6559_NR_BASE + 18)
#define IRQ_HI6559_SIM_HPD_IN_FALL              (IRQ_HI6559_NR_BASE + 19)
#define IRQ_HI6559_SIM_HPD_IN_RAISE             (IRQ_HI6559_NR_BASE + 20)

#define HI6559_IRQM2_BASE_OFFSET 7
#define HI6559_IRQM3_BASE_OFFSET 11
#define HI6559_IRQM4_BASE_OFFSET 17

#define HI6559_IRQ_REG_NUM 4

struct hi6559_irq_data
{
    struct workqueue_struct *hi6559_irq_wq;
    struct work_struct hi6559_irq_wk;
    struct clk *clk_hi6559;
    struct resource *res;
    spinlock_t      irq_lock;
    u32             irq;
};

typedef unsigned long  hi6559_irqflags_t;
typedef void (*hi6559funcptr)(void *);

/* 中断处理回调函数的结构体 */
typedef struct 
{
    hi6559funcptr   routine;
    void *  data;
    u32  cnt;               /* 中断引用计数 */
} hi6559_irq_st;

/* 错误码 */
#define HI6559_IRQ_OK       0
#define HI6559_IRQ_ERROR    -1

/* 函数声明 */
void hi6559_irq_mask(u32 irq);
void hi6559_irq_unmask(u32 irq);
s32 hi6559_irq_is_masked(u32 irq);
s32 hi6559_irq_callback_register(u32 irq, hi6559funcptr routine, void *data);
#endif
