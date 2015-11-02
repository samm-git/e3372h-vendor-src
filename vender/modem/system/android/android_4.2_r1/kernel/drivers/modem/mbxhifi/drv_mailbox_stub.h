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

#ifndef __DRV_MAILBOX_STUB_H__
#define __DRV_MAILBOX_STUB_H__


/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 邮箱占用memory基地址 */
#define MAILBOX_MEM_BASEADDR            ((int)&g_MAILBOX_BST_HEAD0[0])

/* 邮箱占用memory预留总长度, 包括邮箱控制头和邮箱队列缓存 */
#define MAILBOX_MEM_LENGTH              mailbox_memory_length()

/*PC侧把邮箱的共享内存通道空间映射到静态数组空间*/
extern  int g_MAILBOX_BST_HEAD0[];                      
#define MAILBOX_HEAD_ADDR_MCU2ACPU_MSG           MAILBOX_MEM_BASEADDR                                         
#define MAILBOX_HEAD_ADDR_ACPU2MCU_MSG          (MAILBOX_HEAD_ADDR_MCU2ACPU_MSG +  MAILBOX_HEAD_LEN)
                                                                                                                
#define MAILBOX_HEAD_ADDR_ACPU2HIFI_MSG         (MAILBOX_HEAD_ADDR_ACPU2MCU_MSG +  MAILBOX_HEAD_LEN)
#define MAILBOX_HEAD_ADDR_HIFI2ACPU_MSG         (MAILBOX_HEAD_ADDR_ACPU2HIFI_MSG + MAILBOX_HEAD_LEN)
                                                                                                                                                                                                    
#define MAILBOX_HEAD_ADDR_MCU2CCPU_MSG          (MAILBOX_HEAD_ADDR_HIFI2ACPU_MSG +  MAILBOX_HEAD_LEN)
#define MAILBOX_HEAD_ADDR_CCPU2MCU_MSG          (MAILBOX_HEAD_ADDR_MCU2CCPU_MSG +  MAILBOX_HEAD_LEN)
                                                                                                                
#define MAILBOX_HEAD_ADDR_CCPU2HIFI_MSG         (MAILBOX_HEAD_ADDR_CCPU2MCU_MSG +  MAILBOX_HEAD_LEN)
#define MAILBOX_HEAD_ADDR_HIFI2CCPU_MSG         (MAILBOX_HEAD_ADDR_CCPU2HIFI_MSG +  MAILBOX_HEAD_LEN)

#define MAILBOX_HEAD_ADDR_CCPU2ACPU_MSG             (MAILBOX_HEAD_ADDR_HIFI2CCPU_MSG +  MAILBOX_HEAD_LEN)
#define MAILBOX_HEAD_ADDR_ACPU2CCPU_MSG             (MAILBOX_HEAD_ADDR_CCPU2ACPU_MSG +  MAILBOX_HEAD_LEN)

#define MAILBOX_HEAD_ADDR_CCPU2ACPU_IFC             (MAILBOX_HEAD_ADDR_ACPU2CCPU_MSG +  MAILBOX_HEAD_LEN)
#define MAILBOX_HEAD_ADDR_ACPU2CCPU_IFC             (MAILBOX_HEAD_ADDR_CCPU2ACPU_IFC +  MAILBOX_HEAD_LEN)

#define MAILBOX_HEAD_ADDR_CCPU2MCU_IFC             (MAILBOX_HEAD_ADDR_ACPU2CCPU_IFC +  MAILBOX_HEAD_LEN)
#define MAILBOX_HEAD_ADDR_MCU2CCPU_IFC             (MAILBOX_HEAD_ADDR_CCPU2MCU_IFC +  MAILBOX_HEAD_LEN)

#define MAILBOX_HEAD_ADDR_ACPU2MCU_IFC             (MAILBOX_HEAD_ADDR_MCU2CCPU_IFC +  MAILBOX_HEAD_LEN)
#define MAILBOX_HEAD_ADDR_MCU2ACPU_IFC             (MAILBOX_HEAD_ADDR_ACPU2MCU_IFC +  MAILBOX_HEAD_LEN)

#define MAILBOX_HEAD_BOTTOM_ADDR                    (MAILBOX_HEAD_ADDR_MCU2ACPU_IFC +  MAILBOX_HEAD_LEN)    

#define MAILBOX_QUEUE_ADDR_MCU2ACPU_MSG         (MAILBOX_MEM_BASEADDR + MAILBOX_MEM_HEAD_LEN)
#define MAILBOX_QUEUE_ADDR_ACPU2MCU_MSG         (MAILBOX_QUEUE_ADDR_MCU2ACPU_MSG +  MAILBOX_QUEUE_SIZE(MCU,  ACPU ,MSG))
                                                                                                                
#define MAILBOX_QUEUE_ADDR_ACPU2HIFI_MSG        (MAILBOX_QUEUE_ADDR_ACPU2MCU_MSG +  MAILBOX_QUEUE_SIZE(ACPU, MCU  ,MSG))
#define MAILBOX_QUEUE_ADDR_HIFI2ACPU_MSG        (MAILBOX_QUEUE_ADDR_ACPU2HIFI_MSG + MAILBOX_QUEUE_SIZE(ACPU ,HIFI ,MSG))
                                                                                                                
#define MAILBOX_QUEUE_ADDR_MCU2CCPU_MSG         (MAILBOX_QUEUE_ADDR_HIFI2ACPU_MSG +  MAILBOX_QUEUE_SIZE(HIFI ,ACPU  ,MSG) )
#define MAILBOX_QUEUE_ADDR_CCPU2MCU_MSG         (MAILBOX_QUEUE_ADDR_MCU2CCPU_MSG +  MAILBOX_QUEUE_SIZE(MCU  ,CCPU ,MSG) )
                                                                                                                
#define MAILBOX_QUEUE_ADDR_CCPU2HIFI_MSG        (MAILBOX_QUEUE_ADDR_CCPU2MCU_MSG +  MAILBOX_QUEUE_SIZE(CCPU ,MCU  ,MSG) )
#define MAILBOX_QUEUE_ADDR_HIFI2CCPU_MSG        (MAILBOX_QUEUE_ADDR_CCPU2HIFI_MSG+  MAILBOX_QUEUE_SIZE(CCPU ,HIFI ,MSG))

#define MAILBOX_QUEUE_ADDR_CCPU2ACPU_MSG            (MAILBOX_QUEUE_ADDR_HIFI2CCPU_MSG +  MAILBOX_QUEUE_SIZE(HIFI, CCPU, MSG) )
#define MAILBOX_QUEUE_ADDR_ACPU2CCPU_MSG            (MAILBOX_QUEUE_ADDR_CCPU2ACPU_MSG +  MAILBOX_QUEUE_SIZE(CCPU ,ACPU ,MSG))

#define MAILBOX_QUEUE_ADDR_CCPU2ACPU_IFC            (MAILBOX_QUEUE_ADDR_ACPU2CCPU_MSG +  MAILBOX_QUEUE_SIZE(ACPU, CCPU, MSG) )
#define MAILBOX_QUEUE_ADDR_ACPU2CCPU_IFC            (MAILBOX_QUEUE_ADDR_CCPU2ACPU_IFC+  MAILBOX_QUEUE_SIZE(CCPU, ACPU, IFC))

#define MAILBOX_QUEUE_ADDR_CCPU2MCU_IFC            (MAILBOX_QUEUE_ADDR_ACPU2CCPU_IFC +  MAILBOX_QUEUE_SIZE(ACPU, CCPU, IFC) )
#define MAILBOX_QUEUE_ADDR_MCU2CCPU_IFC            (MAILBOX_QUEUE_ADDR_CCPU2MCU_IFC+  MAILBOX_QUEUE_SIZE(CCPU, MCU, IFC))

#define MAILBOX_QUEUE_ADDR_ACPU2MCU_IFC            (MAILBOX_QUEUE_ADDR_MCU2CCPU_IFC +  MAILBOX_QUEUE_SIZE(MCU, CCPU, IFC) )
#define MAILBOX_QUEUE_ADDR_MCU2ACPU_IFC            (MAILBOX_QUEUE_ADDR_ACPU2MCU_IFC+  MAILBOX_QUEUE_SIZE(ACPU, MCU, IFC))
  
#define MAILBOX_MEMORY_BOTTOM_ADDR                  (MAILBOX_QUEUE_ADDR_MCU2CCPU_IFC + MAILBOX_QUEUE_SIZE(MCU, ACPU, IFC))

int mailbox_memory_length(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of __DRV_MAILBOX_STUB_H__.h */

