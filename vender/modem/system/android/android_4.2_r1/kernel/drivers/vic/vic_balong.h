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


#ifndef _VIC_BALONG_H_
#define _VIC_BALONG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "soc_interrupts.h"
#include "bsp_memmap.h"
#include "osl_module.h"
#include "bsp_vic.h"

#define INT_VIC_MAX     32
#define BALONG_VIC_NUM  2

#ifdef __KERNEL__
#define INT_LVL_VIC      INT_LVL_EXT12
#define INT_GU_LVL_VIC   INT_LVL_EXT14
#define BSP_VIC_BASE     HI_APP_VIC_BASE_ADDR_VIRT
#define BSP_GU_VIC_BASE  HI_APP_GU_VIC_BASE_ADDR_VIRT
#else
#define INT_LVL_VIC      INT_LVL_EXT13
#define INT_GU_LVL_VIC   INT_LVL_EXT15
#define BSP_VIC_BASE     HI_MDM_VIC_BASE_ADDR_VIRT
#define BSP_GU_VIC_BASE  HI_MDM_GU_VIC_BASE_ADDR_VIRT
#endif

#ifndef OK
#define OK      (0)
#endif

#ifndef ERROR
#define ERROR   (-1)
#endif

typedef struct {
    vicfuncptr	routine;
    s32	arg;
    } bsp_victable_st;

typedef struct {
	u32 vic_base_addr;
	u32 vic_irq_num;
	u32 vic_inten;
	u32 vic_intdisable;
	u32 vic_final_status;
    bsp_victable_st victable[INT_VIC_MAX];
    } bsp_vic_st;


#ifdef __cplusplus
}
#endif

#endif /* _VIC_BALONG_H_ */

