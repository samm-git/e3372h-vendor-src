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
#ifndef __COMMON_HI6559_H
#define __COMMON_HI6559_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "bsp_pmu.h"

typedef unsigned long hi6559_commflags_t;

#define PMU_HI6559_POWER_KEY_MASK                   (1 << 5)   /* Bit 5 for Power key */
#define PMU_HI6559_USB_STATE_MASK                   (1 << 5)
#define PMU_HI6559_HRESET_STATE_MASK                (1 << 1)
#define PMU_HI6559_BACKBAT_CHARGER_ENABLE_OFFSET    2       /* 0x11F寄存器bit2 */
#define PMU_HI6559_BACKBAT_CHARGER_VOLT_NUM         3       /* 0x11F寄存器bit0~bit1 */
#define PMU_HI6559_LONGPOWERKEY_TIME_MAX            11      /* 0x11E寄存器bit2~bit0 */
#define PMU_HI6559_LONGPOWERKEY_TIME_MIN            6       /* 0x11E寄存器bit2~bit0 */
#define PMU_HI6559_LONGPOWERKEY_TIME_MASK           7       /* 0x11E寄存器bit2~bit0 */
#define PMU_HI6559_LONGPOWERKEY_OFF_MASK_OFS        3       /* 0x11E寄存器bit3 */
#define PMU_HI6559_LONGPOWERKEY_REBOOT_MASK_OFS     6       /* 0x11E寄存器bit6 */
#define PMU_HI6559_SIM_DEB_SEL_MASK                 0x1F    /* 0x43寄存器bit0~bit4 */
#define PMU_HI6559_SIM_DEB_TIME_MIN                 120     /* 0x43寄存器bit0~bit4，SIM卡去抖时间最小值，单位us */
#define PMU_HI6559_SIM_DEB_TIME_MAX                 600     /* 0x43寄存器bit0~bit4，SIM卡去抖时间最大值，单位us */
#define PMU_HI6559_SIM_DEB_TIME_STEP                30      /* SIM卡去抖时间间隔，单位us */

#define HI6559_NP_OCP_SCP_REG_NUM                   7       /* 非下电过温/短路记录寄存器数量，寄存器0x18~0x1E */

/*函数声明*/
#if defined(__KERNEL__)
s32  hi6559_32k_clk_enable(pmu_clk_e clk_id);
s32  hi6559_32k_clk_disable(pmu_clk_e clk_id);
s32  hi6559_32k_clk_is_enabled(pmu_clk_e clk_id);
bool hi6559_usb_state_get(void);
bool hi6559_power_key_state_get(void);
s32  hi6559_sim_deb_time_set(u32 uctime);
void hi6559_ldo22_res_enable(void);
void hi6559_ldo22_res_disable(void);
#endif

u8 hi6559_version_get(void);
void bsp_hi6559_common_init(void);

#ifdef __cplusplus
}
#endif
#endif
