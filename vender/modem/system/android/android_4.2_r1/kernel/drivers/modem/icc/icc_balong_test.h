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
#ifndef __BALONG_ICC__TEST_H__
#define __BALONG_ICC__TEST_H__

#ifdef __cplusplus /* __cplusplus */
extern "C"
{
#endif /* __cplusplus */

#include "osl_types.h"
#include "icc_balong.h"

#define ICC_TEST_MCORE_CHN_SIZE          (20)
#define ICC_TEST_MCORE_START_CHN         (16)
#define ICC_TEST_CHANNEL_START           (32)
#define ICC_TAKE_SEM_TIMEOUT_JIFFIES     (3000)
#define ICC_TAKE_SEM_TIMEOUT_MS          (5000)

#if defined(__KERNEL__)
#define ICC_TEST_TASK_PRI                (25)
#define ICC_TEST_CHANNEL_SIZE            (64)
#define ICC_TEST_USE_CHN_SIZE            (4)
#define ICC_TEST_FIFO_SIZE               (1024)

#elif defined(__VXWORKS__)
#define ICC_TEST_TASK_PRI                (25)
#define ICC_TEST_CHANNEL_SIZE            (64)
#define ICC_TEST_USE_CHN_SIZE            (4)
#define ICC_TEST_FIFO_SIZE               (1024)

#elif defined(__CMSIS_RTOS)
#define ICC_TEST_TASK_PRI                (osPriorityHigh)
#define ICC_TEST_USE_CHN_SIZE            (4)
#define ICC_TEST_FIFO_SIZE               (40)

#endif

typedef s32 (*task_entry)(void *obj);

#define ICC_RECV_OK                      (0x10305070)
#define ICC_RECV_ERR                     (-1)
#define ICC_TEST_PASS                    (0)
#define ICC_TEST_FAIL                    (-1)
#define ICC_RECV_OK_TIMES                (21)
#define ICC_SEND_SYNC_RET                (0x789abcde)
#define ICC_TEST_MULTI_SEND_CHN_SIZE     (ICC_TEST_CHANNEL_SIZE/4)
#define ICC_VALIDE_CHN_NUM               (ICC_CHN_CSHELL)
#define ICC_TEST_CASE_INITILIZED         (0x1)
#define ICC_TEST_DEFAULT_ASYN_SUBCHN_NUM (32)
#define ICC_TEST_DEFAULT_SYNC_SUBCHN_NUM (33)
#define ICC_TEST_SEND_CNT                (100)
#define ICC_TEST_MUL_SUB_CHN_SEND_FLAG        (1)
#define ICC_TEST_MUL_SUB_CHN_SEND_SYNC_FLAG   (2)
#define ICC_TEST_MUL_CHN_SEND_FLAG            (3)
#define ICC_TEST_MULTI_CHN_SEND_SYNC_FLAG     (4)

/* 回调里不调用bsp_icc_read用例添加, 在自己的任务上下文中调用读接口, 利用cshell通道  y00184236/2013/11/04 */
#define ICC_CONF_MSG_TYPE1               (0x51)
#define ICC_CONF_MSG_TYPE2               (0x22)
#define SUPPORT_ICC_READ_IN_OTHER_TASK   (3)
#define ICC_NEW_START_DATA               (0)
#define ICC_NEW_DATA_LEN                 (32)

struct icc_test_send{
	u32 id;
};

struct icc_test_sub_channel
{
	u8 *wr_buf;
	u8 *rd_buf;
	osl_sem_id confirm_sem;
	s32 task_id;
	s32 success_cnt;
};

struct icc_test_channel
{
	u32 func_size_bak;
	u32 fifo_size_bak;
	struct icc_channel_vector *vec_bak;
#if defined(__KERNEL__) || defined(__VXWORKS__)
	struct icc_test_sub_channel sub_channels[ICC_TEST_CHANNEL_SIZE];
#elif defined(__CMSIS_RTOS) /* rtx资源有限 */
	struct icc_test_sub_channel sub_channels[ICC_TEST_MCORE_CHN_SIZE];
#endif
};

struct icc_test_case
{
	u32 state;
	u32 mcore_test_state;
	u32 task_id;
	u8 *rd_buf;
	u8 *wr_buf;
	osl_sem_id multi_send_confirm_sem;
#if defined(__KERNEL__) || defined(__VXWORKS__)
	struct icc_test_channel channels[ICC_CHN_ID_MAX];
#elif defined(__CMSIS_RTOS)
	struct icc_test_channel channels[2];
#endif
	/* 回调里不调用bsp_icc_read用例添加, 在自己的任务上下文中调用读接口, 利用cshell通道  y00184236/2013/11/04 */
	osl_sem_id recv_task_sem;
	s32        recv_task_id;
	u32        recv_task_flag;
};

#ifdef __cplusplus /* __cplusplus */
}
#endif /* __cplusplus */

#endif    /*  __BALONG_ICC__TEST_H__ */
