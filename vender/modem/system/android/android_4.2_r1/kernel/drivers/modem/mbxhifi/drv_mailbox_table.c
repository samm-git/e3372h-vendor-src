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



/*****************************************************************************
  1 头文件包含
*****************************************************************************/
#include "drv_mailbox.h"
#include "drv_mailbox_cfg.h"
#include "drv_mailbox_gut.h"

#if defined(_DRV_LLT_)
#include "drv_mailbox_stub.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifdef CONFIG_HIFI

/*****************************************************************************
    可维可测信息中包含的C文件编号宏定义
*****************************************************************************/
#undef  _MAILBOX_FILE_
#define _MAILBOX_FILE_   "table"
/*****************************************************************************
  2 全局变量定义
*****************************************************************************/
/*定义所有已存在的共享内存通道的物理内存及硬件资源配置，全局定义*/
MAILBOX_EXTERN struct mb_cfg g_mailbox_global_cfg_tbl[] =
{
   
#if 0
    /*CCPU到其他核的通道*/
    MAILBOX_CHANNEL_COMPOSE(CCPU, MCU,  MSG),
//    MAILBOX_CHANNEL_COMPOSE(CCPU, MCU,  IFC),
    MAILBOX_CHANNEL_COMPOSE(CCPU, HIFI, MSG),
//    MAILBOX_CHANNEL_COMPOSE(CCPU, ACPU, IFC),
//    MAILBOX_CHANNEL_COMPOSE(CCPU, ACPU, MSG),

    /*ACPU到其他核的通道*/
    MAILBOX_CHANNEL_COMPOSE(ACPU, MCU,  MSG),
//    MAILBOX_CHANNEL_COMPOSE(ACPU, MCU,  IFC),
    MAILBOX_CHANNEL_COMPOSE(ACPU, HIFI, MSG),
//    MAILBOX_CHANNEL_COMPOSE(ACPU, CCPU, IFC),
//    MAILBOX_CHANNEL_COMPOSE(ACPU, CCPU, MSG),
    
    /*MCU到其他核的通道*/
    MAILBOX_CHANNEL_COMPOSE(MCU, CCPU,  MSG),
//    MAILBOX_CHANNEL_COMPOSE(MCU, CCPU,  IFC),
    MAILBOX_CHANNEL_COMPOSE(MCU, ACPU,  MSG),
//    MAILBOX_CHANNEL_COMPOSE(MCU, ACPU,  IFC),
    
    /*HIFI到其他核的通道*/
    MAILBOX_CHANNEL_COMPOSE(HIFI, CCPU, MSG),
    MAILBOX_CHANNEL_COMPOSE(HIFI, ACPU, MSG),
#else
    MAILBOX_CHANNEL_COMPOSE(ACPU, HIFI, MSG),
    MAILBOX_CHANNEL_COMPOSE(HIFI, ACPU, MSG),
    MAILBOX_CHANNEL_COMPOSE(CCPU, HIFI, MSG),
    MAILBOX_CHANNEL_COMPOSE(HIFI, CCPU, MSG),
#endif


    /*结束标志*/
    {MAILBOX_MAILCODE_INVALID,  0,  0, 0}

};

/*平台邮箱通道句柄的内存池空间*/
MAILBOX_EXTERN struct mb_buff     g_mailbox_channel_handle_pool[MAILBOX_CHANNEL_NUM]; 
 
/*平台邮箱用户回调句柄的内存池空间*/
MAILBOX_EXTERN struct mb_cb    g_mailbox_user_cb_pool[MAILBOX_USER_NUM];

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

