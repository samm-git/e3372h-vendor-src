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
#ifndef __COMMON_HI6551_H
#define __COMMON_HI6551_H
#ifdef __cplusplus /* __cplusplus */
extern "C"
{
#endif /* __cplusplus */
#include "bsp_pmu.h"

typedef unsigned long hi6551_commflags_t;

#define PMU_HI6551_POWER_KEY_MASK         (1<<5)   /* Bit 5 for Power key */
#define PMU_HI6551_USB_STATE_MASK            (1<<5)
#define PMU_HI6551_HRESET_STATE_MASK            (1<<1)

/*º¯ÊýÉùÃ÷*/
#if defined(__KERNEL__)
int hi6551_32k_clk_enable(pmu_clk_e clk_id);
int hi6551_32k_clk_disable(pmu_clk_e clk_id);
int hi6551_32k_clk_is_enabled(pmu_clk_e clk_id);
bool hi6551_usb_state_get(void);
bool hi6551_power_key_state_get(void);
int hi6551_sim_deb_time_set(u32 uctime);
void hi6551_ldo22_res_enable(void);
void hi6551_ldo22_res_disable(void);
#endif
#if defined(__CMSIS_RTOS)
void hi6551_pastar_enable(void);
void hi6551_pastar_disable(void);
#endif
u8 hi6551_version_get(void);
void bsp_hi6551_common_init(void);
#ifdef __cplusplus /* __cplusplus */
}
#endif /* __cplusplus */
#endif
