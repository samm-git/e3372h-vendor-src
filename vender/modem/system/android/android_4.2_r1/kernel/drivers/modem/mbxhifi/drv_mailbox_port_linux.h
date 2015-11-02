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


#ifndef _DRV_MAILBOX_PORT_LINUX_H_
#define _DRV_MAILBOX_PORT_LINUX_H_


/*****************************************************************************
  1 头文件包含
*****************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include <linux/kernel.h>

/*虚实地址转换接口定义*/
#ifndef _DRV_LLT_

/* Modified by c64416 for hifi mailbox, 2013/09/24, begin */
/*ACPU实地址向虚拟地址映射的转换接口*/
#define MEM_CORE_SHARE_PHY2VIRT(phy)         (((unsigned int)phy)  - DDR_SHARED_MEM_ADDR + DDR_SHARED_MEM_VIRT_ADDR)
#define MEM_CORE_SHARE_VIRT2PHY(virt)        (((unsigned int)virt) - DDR_SHARED_MEM_VIRT_ADDR + DDR_SHARED_MEM_ADDR)
/* Modified by c64416 for hifi mailbox, 2013/09/24, end */

#else

/*ACPU实地址向虚拟地址映射的转换接口*/
#define MEM_CORE_SHARE_PHY2VIRT(phy)         (phy)
#define MEM_CORE_SHARE_VIRT2PHY(virt)        (virt)
#endif

/*邮箱通道，功能，核等相关配置接口定义*/
/*表明此CPU的ID*/
#define MAILBOX_LOCAL_CPUID                         MAILBOX_CPUID_ACPU


/* Modified by c64416 for hifi mailbox, 2013/09/24, begin */
/*定义C核平台相关的 邮箱通道 总数，关心收发通道 */
#define MAILBOX_CHANNEL_NUM \
    ( MAILBOX_CHANNEL_BUTT(ACPU, HIFI)   \
    + MAILBOX_CHANNEL_BUTT(HIFI, ACPU)   \
    )

/*定义C核平台相关的 邮件 总数 , 只关心接收通道*/
#define MAILBOX_USER_NUM    \
    ( MAILBOX_USER_BUTT(HIFI, ACPU, MSG) \
    )
/* Modified by c64416 for hifi mailbox, 2013/09/24, end */

/*打印输出相关配置定义*/
/*控制台打印输出接口*/
#ifdef _DRV_LLT_
#define mailbox_out(p)                              (printf p)
#else
#define mailbox_out(p)                              (printk p)
#endif

/*定义宏控制是否打印行数*/
#define _MAILBOX_LINE_                              __LINE__

/*定义宏控制是否打印文件名,(不打印文件名可节省代码空间)*/
#define _MAILBOX_FILE_                              (void*)(0) /*__FILE__*/

#define MAILBOX_LOG_LEVEL                           MAILBOX_LOG_INFO 


/*邮箱调试功能相关配置定义*/
/*表明在此CPU上打开可维可测功能*/
#ifndef MAILBOX_OPEN_MNTN
#define MAILBOX_OPEN_MNTN
#endif

/*可维可测记录的最大传送ID数*/
#define MAILBOX_RECORD_USEID_NUM                    (64)

#define RT  "\n"   /*打印回车符号*/

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif  /*_DRV_MAILBOX_LINUX_H_*/

