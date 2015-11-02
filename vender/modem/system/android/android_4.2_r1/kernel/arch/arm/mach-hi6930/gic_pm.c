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
#include <osl_bio.h>
#include <bsp_gic_pm.h>
#include "gic_pm.h"
static u32 gic_reg_need_bak[GIC_REG_BAK_NUM] = {
	GIC_CPUICR,
	GIC_CPUPMR,
	GIC_CPUBPR,
	GIC_ICABPR,
	GIC_ICDDCR,
	GIC_ICDISR,
	GIC_ICDISR1,
	GIC_ICDISR1_OFFSET(1),
	GIC_ICDISR1_OFFSET(2),
	GIC_ICDISR1_OFFSET(3),
	GIC_ICDISR1_OFFSET(4),
	GIC_ICDISR1_OFFSET(5),
	GIC_ICDISR1_OFFSET(6),
	GIC_ICDIPR,
	GIC_ICDIPR_OFFSET(1) ,
	GIC_ICDIPR_OFFSET(2) ,
	GIC_ICDIPR_OFFSET(3) ,
	GIC_ICDIPR_OFFSET(4) ,
	GIC_ICDIPR_OFFSET(5) ,
	GIC_ICDIPR_OFFSET(6) ,
	GIC_ICDIPR_OFFSET(7) ,
	GIC_ICDIPR_OFFSET(8) ,
	GIC_ICDIPR_OFFSET(9) ,
	GIC_ICDIPR_OFFSET(10) ,
	GIC_ICDIPR_OFFSET(11) ,
	GIC_ICDIPR_OFFSET(12) ,
	GIC_ICDIPR_OFFSET(13) ,
	GIC_ICDIPR_OFFSET(14) ,
	GIC_ICDIPR_OFFSET(15) ,
	GIC_ICDIPR_OFFSET(16) ,
	GIC_ICDIPR_OFFSET(17) ,
	GIC_ICDIPR_OFFSET(18) ,
	GIC_ICDIPR_OFFSET(19) ,
	GIC_ICDIPR_OFFSET(20) ,
	GIC_ICDIPR_OFFSET(21) ,
	GIC_ICDIPR_OFFSET(22) ,
	GIC_ICDIPR_OFFSET(23) ,
	GIC_ICDIPR_OFFSET(24) ,
	GIC_ICDIPR_OFFSET(25) ,
	GIC_ICDIPR_OFFSET(26) ,
	GIC_ICDIPR_OFFSET(27) ,
	GIC_ICDIPR_OFFSET(28) ,
	GIC_ICDIPR_OFFSET(29) ,
	GIC_ICDIPR_OFFSET(30) ,
	GIC_ICDIPR_OFFSET(31) ,
	GIC_ICDIPR_OFFSET(32) ,
	GIC_ICDIPR_OFFSET(33) ,
	GIC_ICDIPR_OFFSET(34) ,
	GIC_ICDIPR_OFFSET(35) ,
	GIC_ICDIPR_OFFSET(36) ,
	GIC_ICDIPR_OFFSET(37) ,
	GIC_ICDIPR_OFFSET(38) ,
	GIC_ICDIPR_OFFSET(39) ,
	GIC_ICDIPR_OFFSET(40) ,
	GIC_ICDIPR_OFFSET(41) ,
	GIC_ICDIPR_OFFSET(42) ,
	GIC_ICDIPR_OFFSET(43) ,
	GIC_ICDIPR_OFFSET(44) ,
	GIC_ICDIPR_OFFSET(45) ,
	GIC_ICDIPR_OFFSET(46) ,
	GIC_ICDIPR_OFFSET(47) ,
	GIC_ICDIPR_OFFSET(48) ,
	GIC_ICDIPR_OFFSET(49) ,
	GIC_ICDIPR_OFFSET(50) ,
	GIC_ICDIPR_OFFSET(51) ,
	GIC_ICDIPR_OFFSET(52) ,
	GIC_ICDIPR_OFFSET(53) ,
	GIC_ICDIPR_OFFSET(54) ,
	GIC_ICDIPR_OFFSET(55) ,
	GIC_ICDIPR_OFFSET(56) ,
	GIC_ICDICFR,
	GIC_ICDSGIR
	};
static u32 suspend_addr[GIC_REG_BAK_NUM];
void gic_suspend(void)
{
	u32 i = 0;
	for(i=0;i<GIC_REG_BAK_NUM;i++)
	{
		suspend_addr[i] = readl(gic_reg_need_bak[i]);
	}
}
void gic_resume(void)
{
	u32 i = 0;
	for(i=0;i<GIC_REG_BAK_NUM;i++)
	{
		 writel(suspend_addr[i],gic_reg_need_bak[i]);
	}
}
