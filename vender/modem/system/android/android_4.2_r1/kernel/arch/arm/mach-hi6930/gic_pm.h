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
#ifndef __GIC_DRX_H__
#define __GIC_DRX_H__
#include <bsp_memmap.h>
#ifdef __KERNEL__
/*GIC base addr */
#define GIC_CPU_BASE 	           (HI_APP_GIC_BASE_ADDR_VIRT+0x100)
#define GIC_DIST_BASE              (HI_APP_GIC_BASE_ADDR_VIRT+0x1000)
#elif defined(__VXWORKS__)
/*GIC base addr*/
#define GIC_CPU_BASE               (HI_MDM_GIC_BASE_ADDR_VIRT+0x100)  /*(0x8000100)*/
#define GIC_DIST_BASE              (HI_MDM_GIC_BASE_ADDR_VIRT+0x1000)/*(0x8001000)*/
#endif
#define GIC_REG_BAK_NUM 72

/* GIC */
#define GIC_CPUICR                 (GIC_CPU_BASE + 0x000)
#define GIC_CPUPMR                 (GIC_CPU_BASE + 0x004)
#define GIC_CPUBPR                 (GIC_CPU_BASE + 0x008)
#define GIC_CPUIACK                (GIC_CPU_BASE + 0x00C)
#define GIC_CPUEOIR                (GIC_CPU_BASE + 0x010)
#define GIC_CPURPR                 (GIC_CPU_BASE + 0x014)
#define GIC_CPUHPIR                (GIC_CPU_BASE + 0x018)
#define GIC_ICABPR                 (GIC_CPU_BASE + 0x01C)

#define GIC_ICDDCR                 (GIC_DIST_BASE + 0x000)
#define GIC_ICDISR                 (GIC_DIST_BASE + 0x080)
#define GIC_ICDISER                (GIC_DIST_BASE + 0x100)
#define GIC_ICDICER                (GIC_DIST_BASE + 0x180)
#define GIC_ICDISR1                (GIC_DIST_BASE + 0x200)
#define GIC_ICDISR1_OFFSET(x)      (GIC_ICDISR1  + ((x)*0x4))

#define GIC_ICDICPR                (GIC_DIST_BASE + 0x280)

#define GIC_ICDISPR                (GIC_DIST_BASE + 0x300)
#define GIC_ICDIPR                 (GIC_DIST_BASE + 0x400)
#define GIC_ICDIPR_OFFSET(x)       (GIC_ICDIPR  + ((x)*0x4))

#define GIC_ICDTARG                (GIC_DIST_BASE + 0x800)
#define GIC_ICDICFR                (GIC_DIST_BASE + 0xC00)
#define GIC_ICDSGIR                (GIC_DIST_BASE + 0xF00)
#define GIC_ICDISPR_OFFSET(x)      (GIC_ICDISPR  + ((x)*0x4))
#endif
