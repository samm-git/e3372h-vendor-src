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
#ifndef __HI6559_EXC_H
#define __HI6559_EXC_H

#include <linux/workqueue.h>
#include <bsp_om.h>

#define HI6559_OTMP_125_OFFSET                  0
#define HI6559_OTMP_150_OFFSET                  7
#define HI6559_VSYS_UNDER_2P5_OFFSET            2
#define HI6559_VSYS_UNDER_2P85_OFFSET           3
#define HI6559_VSYS_OVER_6P0_OFFSET             4
#define HI6559_POWER_KEY_4S_PRESS_OFFSET        5
#define HI6559_POWER_KEY_20MS_RELEASE_OFFSET    6
#define HI6559_POWER_KEY_20MS_PRESS_OFFSET      7
#define HI6559_NP_SCP_RECORD1_CONT_NUM          3   /* 寄存器0x18有效bit数量 */
#define HI6559_NP_OCP_RECORD1_CONT_NUM          5   /* 寄存器0x19有效bit数量 */
#define HI6559_NP_OCP_RECORD2_CONT_NUM          5   /* 寄存器0x1A有效bit数量 */
#define HI6559_NP_OCP_RECORD3_CONT_NUM          6   /* 寄存器0x1B有效bit数量 */
#define HI6559_NP_OCP_RECORD4_CONT_NUM          3   /* 寄存器0x1C有效bit数量 */
#define HI6559_LVS07_BIT_OFFSET                 5   /* LVS07过载bit在寄存器内的偏移 */
#define HI6559_LVS09_BIT_OFFSET                 0   /* LVS07过载bit在寄存器内的偏移 */
#define PMU_MODULE_ID                           BSP_MODU_PMU

typedef unsigned long hi6559_excflags_t;

struct hi6559_exc_data{
    spinlock_t      lock;
    struct workqueue_struct *hi6559_om_wq;
    struct delayed_work hi6559_om_wk;
};

/* 寄存器的内容 */
struct hi6559_reg_cont{
    u32 bit_ofs;        /* bit位偏移 */
    u32 volt_id;        /* 电压源ID */
    char *cont;           /* bit位含义 */
};

#endif
