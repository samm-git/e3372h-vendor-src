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
#ifndef __BALONG_CPUFREQ_H__
#define __BALONG_CPUFREQ_H__

#include <linux/io.h>
#include <linux/init.h>
#include <linux/irq.h>
#include <bsp_om.h>
#include <bsp_cpufreq.h>

#define  cpufreq_err(fmt,...)		(bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MUDU_CPUFREQ, "[cpufreq]: <%s> <%d>"fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__))
#define  cpufreq_info(fmt,...)		(bsp_trace(BSP_LOG_LEVEL_INFO, BSP_MUDU_CPUFREQ, "[cpufreq]: <%s> <%d>"fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__))
#define  cpufreq_debug(fmt,...)		(bsp_trace(BSP_LOG_LEVEL_DEBUG, BSP_MUDU_CPUFREQ, "[cpufreq]: <%s> <%d>"fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__))



#ifndef BSP_OK
#define BSP_OK              (0)
#endif

#ifndef BSP_ERROR
#define BSP_ERROR           (-1)
#endif

#ifndef BSP_TRUE
#define BSP_TRUE            (1)
#endif

#ifndef BSP_FALSE
#define BSP_FALSE           (0)
#endif

#ifndef BSP_NULL
#define BSP_NULL            (void*)0
#endif

#ifndef SEM_EMPTY
#define SEM_EMPTY 0
#endif

#define DEF_FREQUENCY_DOWN_THRESHOLD	(20)
#define DEF_FREQUENCY_UP_THRESHOLD		(40)
#define DEF_SAMPLING_DOWN_FACTOR		(1)

#define TRANSITION_LATENCY_LIMIT		(10 * 1000 * 1000)
/*任务延迟时间*/
#define CPUFREQ_DEALY_TIME_OF_TASK  4000000 /*us*/

#define CPUFREQ_DOWN_THRESHOLD_TIMES 3







/*调频策略需要信息的结构体，根据情况可增减 same with g_stDfsCcpuControl*/
struct cpu_dbs_info_s {
	s32 cpu;
	struct cpufreq_policy *cur_policy;
	struct delayed_work work;
	struct cpufreq_frequency_table *freq_table;
	cputime64_t prev_cpu_idle;
	cputime64_t prev_cpu_wall;
	u32 start_time;
	u32 cpu_down_time;/*符合下调条件次数*/
	u32 cpu_up_time;/*符合上调条件次数*/
};

/*same with g_stDfsCcpuConfigInfo*/
struct dbs_tuners {
	u32 sampling_rate;/*采样值*/
	u32 up_threshold;/*调频CPU占用上限阈值*/
	u32 down_threshold;/*调频CPU占用下限阈值*/

	u32 down_threshold_times;/*下调阈值*/
	u32 up_threshold_times;/*上调阈值*/
};




#endif /* __BALONG_CPUFREQ_H__ */
