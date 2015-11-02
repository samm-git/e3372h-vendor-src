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
#ifndef __REGULATOR_BALONG_H__
#define __REGULATOR_BALONG_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include <hi_pwrctrl_interface.h>
#include <bsp_memmap.h>
#include <bsp_om.h>
#include <bsp_regulator.h>

//32
#define  regu_pr_err(fmt,...)		(bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_REGULATOR, "[regulator]: <%s> <%d>"fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__))
#define  regu_pr_info(fmt,...)		(bsp_trace(BSP_LOG_LEVEL_INFO, BSP_MODU_REGULATOR, "[regulator]: <%s> <%d>"fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__))
#define  regu_pr_debug(fmt,...)		(bsp_trace(BSP_LOG_LEVEL_DEBUG, BSP_MODU_REGULATOR, "[regulator]: <%s> <%d>"fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__))

struct regulator_id_ops {
	int (*list_voltage) (int regulator_id, unsigned selector);
	int (*set_voltage) (int regulator_id, int min_uV, int max_uV, unsigned *selector);
	int (*get_voltage) (int regulator_id);
	int (*is_enabled) (int regulator_id);
	int (*enable) (int regulator_id);
	int (*disable) (int regulator_id);
	int (*set_mode) (int regulator_id, int mode);
	unsigned int (*get_mode) (int regulator_id);
	int (*set_current) (int regulator_id, int min_uA, int max_uA, unsigned *selector);
	int (*get_current) (int regulator_id);
};
struct regulators_type {
	int base_num;
	int regulator_type;
	struct regulator_id_ops *ops;
};

struct regulator_mtcmos_ops {
	void (*mtcmos_enable) (void);
	void (*mtcmos_disable) (void);
	unsigned int  (*mtcmos_is_enable)(void);
};

#ifdef __cplusplus
}
#endif

#endif/*__REGULATOR_BALONG_H__*/