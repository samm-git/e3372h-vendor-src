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


#ifndef _DRV_MAILBOX_PLATFORM_H_
#define _DRV_MAILBOX_PLATFORM_H_

/*****************************************************************************
  1 头文件包含
*****************************************************************************/


/* Modified by c64416 for hifi mailbox, 2013/09/24, begin */
#if defined(__VXWORKS__) /* vxworks */
#include "drv_mailbox_port_vxworks.h"
#else /* linux */
#include "drv_mailbox_port_linux.h"
#endif
/* Modified by c64416 for hifi mailbox, 2013/09/24, end */

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  2 宏定义
*****************************************************************************/

#ifndef MAILBOX_TRUE
#define MAILBOX_TRUE                                (1)
#endif

#ifndef MAILBOX_FALSE
#define MAILBOX_FALSE                               (0)
#endif

#ifndef MAILBOX_NULL
#define MAILBOX_NULL                                (void*)(0)
#endif

/*邮箱接口类型定义*/
#ifdef _DRV_LLT_
#define MAILBOX_LOCAL                                           /*文件内部调用接口(PC工程)*/
#else
#ifndef MAILBOX_LOCAL
#define MAILBOX_LOCAL                               static      /*文件内部调用接口(单板)*/
#endif
#endif

#ifndef MAILBOX_EXTERN
#define MAILBOX_EXTERN                                          /*模块内部调用接口*/  
#endif

#ifndef MAILBOX_GLOBAL
#define MAILBOX_GLOBAL                                          /*本模块提供给外部的功能接口*/                                
#endif

/*定义邮箱通道数组数目及方向*/
#define MIALBOX_DIRECTION_INVALID                   (0)             /*表示通道方向无效*/
#define MIALBOX_DIRECTION_SEND                      (1)             /*表示当前核发送给其他核的通道*/
#define MIALBOX_DIRECTION_RECEIVE                   (2)             /*表示其他核发送给当前核的通道*/

/*无效邮件号*/
#define  MAILBOX_MAILCODE_INVALID                   (unsigned long)(0xffffffff)

/*****************************************************************************
  3 枚举定义
*****************************************************************************/

/*****************************************************************************
 实 体 名  : MAILBOX_DELAY_SCENE_E
 功能描述  : 传入给平台层延迟函数的场景号。
*****************************************************************************/
enum MAILBOX_DELAY_SCENE_E
{
    MAILBOX_DELAY_SCENE_MSG_FULL ,          /*发送msg邮件时，邮箱满场景的延时等待号*/
    MAILBOX_DELAY_SCENE_IFC_FULL            /*发送ifc邮件时，邮箱满场景的延时等待号*/
};

/*****************************************************************************
  4 消息头定义
*****************************************************************************/

/*****************************************************************************
  5 消息定义
*****************************************************************************/

/*****************************************************************************
  6 STRUCT定义
*****************************************************************************/

/*****************************************************************************
  7 UNION定义
*****************************************************************************/

/*****************************************************************************
  8 OTHERS定义
*****************************************************************************/

/*****************************************************************************
  9 全局变量声明
*****************************************************************************/

/*****************************************************************************
  10 函数声明
*****************************************************************************/
/*邮箱内部文件之间调用的接口*/
extern MAILBOX_EXTERN void  mailbox_assert(unsigned long ErroNo);

extern MAILBOX_EXTERN long mailbox_mutex_lock(void **mutexId);

extern MAILBOX_EXTERN void mailbox_mutex_unlock(void **mutexId);

extern MAILBOX_EXTERN long mailbox_int_context(void);

extern MAILBOX_EXTERN long mailbox_get_timestamp(void);

extern MAILBOX_EXTERN long mailbox_process_register(
                unsigned long channel_id, 
                 long (*cb)(unsigned long channel_id),
                 void *priv);

extern MAILBOX_EXTERN long mailbox_channel_register(
                unsigned long ChannelID, 
                unsigned long IntNum,
                unsigned long DstID,
                unsigned long Direct,
                void   **mutex);

extern MAILBOX_EXTERN void *mailbox_memcpy(void *dst, const void *src, long size);

extern MAILBOX_EXTERN void *mailbox_memset(void * m, long c, unsigned long size);

extern MAILBOX_EXTERN long mailbox_delivery(unsigned long channel_id);

extern MAILBOX_EXTERN long mailbox_init_platform(void);

extern MAILBOX_EXTERN long mailbox_scene_delay(
                unsigned long scene_id, 
                unsigned long *try_times);
                
extern MAILBOX_EXTERN void *mailbox_init_completion(void);

extern MAILBOX_EXTERN long mailbox_wait_completion(void **mutexId, unsigned long timeout);

extern MAILBOX_EXTERN void mailbox_complete(void **wait_id);

extern MAILBOX_EXTERN void mailbox_del_completion(void **wait);

extern void mailbox_ifc_test_init(void);

#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif

#endif  /*_DRV_MAILBOX_PLATFORM_H_*/

