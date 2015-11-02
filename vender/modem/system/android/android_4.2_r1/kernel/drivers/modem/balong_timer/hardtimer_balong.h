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

#ifndef __HARDTIMER_BALONG_H__
#define __HARDTIMER_BALONG_H__
#include <osl_types.h>
#include <hi_timer.h>
#include <soc_clk_app.h>
#include <product_config.h>

#ifndef OK
#define OK 0
#endif
#ifndef ERROR
#define ERROR  (-1)
#endif
#ifndef SEM_EMPTY
#define SEM_EMPTY 0
#endif

#ifndef SEM_FULL
#define SEM_FULL 1
#endif
#define  Second_To_Millisecond                               1000

#define MAX_COUNT                                               (0xFFFFFFFF/1000)
#define TCXO_CLK_MAX_LENGTH                      (0xFFFFFFFF/HI_TCXO_CLK) 


#define TIMERS_INTSTATUS(hard_timer_module_addr)                  (hard_timer_module_addr + 0xA0)
#define TIMERS_EOI(hard_timer_module_addr)                        (hard_timer_module_addr+ 0xA4 )
#define TIMERS_RAWINTSTATUS(hard_timer_module_addr)               (hard_timer_module_addr+ 0xA8)
#define TIMER_LOADCOUNT(hard_timer_id_addr)                       (hard_timer_id_addr + HI_TIMER_LOADCOUNT_OFFSET )
#define TIMER_LOADCOUNT_H(hard_timer_id_addr)                     (hard_timer_id_addr + HI_TIMER_LOADCOUNT_H_OFFSET )
#define TIMER_CURRENTVALUE(hard_timer_id_addr)                    (hard_timer_id_addr + HI_TIMER_CURRENTVALUE_OFFSET )
#define TIMER_CURRENTVALUE_H(hard_timer_id_addr)                  (hard_timer_id_addr +  HI_TIMER_CURRENTVALUE_H_OFFSET)
#define TIMER_CONTROLREG(hard_timer_id_addr)                      (hard_timer_id_addr + HI_TIMER_CONTROLREG_OFFSET )
#define TIMER_EOI(hard_timer_id_addr)                             (hard_timer_id_addr +  HI_TIMER_EOI_OFFSET)
#define TIMER_INTSTATUS(hard_timer_id_addr)                       (hard_timer_id_addr + HI_TIMER_INTSTATUS_OFFSET )

#define TIMER_EN_ACK                                 (1<<4)
#define TIMER_INT_MASK                               (1<<2)
#define TIMER_MODE_PERIOD                            (1<<1)
#define HARD_TIMER_ENABLE                             1
#define HARD_TIMER_DISABLE                            0
  

#endif
