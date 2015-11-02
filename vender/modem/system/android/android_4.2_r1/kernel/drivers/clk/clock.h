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
#ifndef __CLOCK_H__
#define __CLOCK_H__

#include <bsp_clk.h>

/************************************************************************
*打印时钟ID和可用的调试函数
************************************************************************/
void debug_clock(void);

/************************************************************************
*可用的调试函数
************************************************************************/
void debug_clk_enable(unsigned int num);
void debug_clk_disable(unsigned int num);
void debug_clk_status(unsigned int num);
void debug_clk_set_rate(unsigned int num, unsigned long rate);
void debug_clk_get_rate(unsigned int num);
void debug_clk_set_parent(unsigned int num, unsigned int parent_num);

/************************************************************************
*时钟OPS
************************************************************************/
int hi6930_clk_enable (struct clk *clk);
void hi6930_clk_disable (struct clk *clk);
int hi6930_clk_round_rate(struct clk *clk, unsigned long rate);
int hi6930_clk_set_rate(struct clk *clk, unsigned long rate);
int hi6930_clk_set_rate_n(struct clk *clk, unsigned long rate);
int hi6930_clk_set_parent(struct clk *clk, struct clk *parent);
int hi6930_clk_isenable(struct clk *clk);
/************************************************************************
*时钟模块自动化测试函数
************************************************************************/
#ifdef __KERNEL__
int clk_get_test_case0(void);
int clk_enable_test_case0(void);
int clk_disable_test_case0(void);
int clk_get_parent_test_case0(void);
int clk_get_rate_test_case0(void);
int clk_set_parent_test_case0(void);
int clk_set_rate_test_case0(void);
#else
int clk_get_test_case1(void);
int clk_enable_test_case1(void);
int clk_disable_test_case1(void);
int clk_get_parent_test_case1(void);
int clk_get_rate_test_case1(void);
int clk_set_parent_test_case1(void);
int clk_set_rate_test_case1(void);
#endif

#endif

