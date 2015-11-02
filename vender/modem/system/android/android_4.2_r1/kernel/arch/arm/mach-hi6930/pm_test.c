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

#include <osl_types.h>
#include "pm.h"
#include <osl_bio.h>
#include <bsp_om.h>
#include <bsp_hardtimer.h>
#include <hi_timer.h>
#include <soc_interrupts.h>

#ifdef PM_TEST_WSRC
/* 启用 timer ( 使用 timer 作为wsrc ) */
// a-timer11 0x90004xxx   c-timer20  0x90004050
OSL_IRQ_FUNC(irqreturn_t, timer11_irq, intLvl, dev)
{
    u32 ret = 0;
    u32 t11_addr = HI_TIMER_11_REGBASE_ADDR_VIRT;
    ret = readl(t11_addr+HI_TIMER_EOI_OFFSET); /*clear int*/
    pm_printk(BSP_LOG_LEVEL_ERROR,"acore timer11 irq handled : 0x%x \n",ret);
    return IRQ_HANDLED;
}
static u32 timer_irq_requested = 0;
int timer_wsrc_test_init(u32 sec)
{
    u32 t11_addr = HI_TIMER_11_REGBASE_ADDR_VIRT;
    if(timer_irq_requested)
    {
        free_irq( INT_LVL_TIMER11 , 0);
    }
    /* 初始值 */
    *(u32*)(t11_addr+HI_TIMER_LOADCOUNT_OFFSET) = HI_TIMER11_CLK*sec;
    /*  config */
    *(u32*)(t11_addr+HI_TIMER_CONTROLREG_OFFSET) = 3;
    if(request_irq(INT_LVL_TIMER11, (irq_handler_t)timer11_irq, 0, "timer11", 0))
    {
        pm_printk(BSP_LOG_LEVEL_ERROR, "request_irq INT_LVL_TIMER11 - FAILED! \n");
        return -1;
    }
    timer_irq_requested = 1;
    return 0;
}
#endif

