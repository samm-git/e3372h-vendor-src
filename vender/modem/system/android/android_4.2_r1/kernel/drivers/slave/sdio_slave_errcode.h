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

#ifndef __SLAVE_ERRCODE_H__
#define __SLAVE_ERRCODE_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "drv_comm.h"
#include "bsp_om.h"
#define BSP_MODU_SLAVE 11
/* 通用错误码，从0x1001开始 */
#define BSP_ERR_COMMON (0x1000)
#define BSP_ERR_MODULE_NOT_INITED (BSP_ERR_COMMON + 1)
#define BSP_ERR_NULL_PTR (BSP_ERR_COMMON + 2)
#define BSP_ERR_INVALID_PARA (BSP_ERR_COMMON + 3)
#define BSP_ERR_RETRY_TIMEOUT (BSP_ERR_COMMON + 4)
#define BSP_ERR_BUF_ALLOC_FAILED (BSP_ERR_COMMON + 5)
#define BSP_ERR_BUF_FREE_FAILED (BSP_ERR_COMMON + 6)

/* 各模块专用错误码，从0x1开始  */
#define BSP_ERR_SPECIAL (0x0)
/* 通用错误码 */
#define SLAVE_ERR_MODULE_NOT_INITED     BSP_DEF_ERR(BSP_MODU_SLAVE,BSP_ERR_MODULE_NOT_INITED)
#define SLAVE_ERR_NULL_POINTER          BSP_DEF_ERR(BSP_MODU_SLAVE,BSP_ERR_NULL_PTR)
#define SLAVE_ERR_INVALID_PARA          BSP_DEF_ERR(BSP_MODU_SLAVE,BSP_ERR_INVALID_PARA)
#define SLAVE_ERR_MALLOC_FAILED         BSP_DEF_ERR(BSP_MODU_SLAVE,BSP_ERR_BUF_ALLOC_FAILED)
#define SLAVE_ERR_FREE_FAILED           BSP_DEF_ERR(BSP_MODU_SLAVE,BSP_ERR_BUF_FREE_FAILED)
#define SLAVE_ERR_RETRY_TIMEOUT         BSP_DEF_ERR(BSP_MODU_SLAVE,BSP_ERR_RETRY_TIMEOUT)

/* 专用错误码 */
#define SLAVE_ERR_UNKNOWN               BSP_DEF_ERR(BSP_MODU_SLAVE,(BSP_ERR_SPECIAL + 0x1))
#define SLAVE_ERR_INVAL_ENUMCFG         BSP_DEF_ERR(BSP_MODU_SLAVE,(BSP_ERR_SPECIAL + 0x2))
#define SLAVE_ERR_TASK_CREATE           BSP_DEF_ERR(BSP_MODU_SLAVE,(BSP_ERR_SPECIAL + 0x3))
#define SLAVE_ERR_MSG_CREATE            BSP_DEF_ERR(BSP_MODU_SLAVE,(BSP_ERR_SPECIAL + 0x4))
#define SLAVE_ERR_SEM_CREATE            BSP_DEF_ERR(BSP_MODU_SLAVE,(BSP_ERR_SPECIAL + 0x5))
#define SLAVE_ERR_FIFO_FULL             BSP_DEF_ERR(BSP_MODU_SLAVE,(BSP_ERR_SPECIAL + 0x6))
#define SLAVE_ERR_INVAL_CONDITION       BSP_DEF_ERR(BSP_MODU_SLAVE,(BSP_ERR_SPECIAL + 0x7))
#define SLAVE_ERR_BUFF_OVERFLOW         BSP_DEF_ERR(BSP_MODU_SLAVE,(BSP_ERR_SPECIAL + 0x8))

/* Defines for Success and Fail*/
#define SLAVE_OK	    (BSP_OK)
#define SLAVE_ERROR     (BSP_ERROR)

#ifdef __cplusplus
}
#endif

#endif /* end #define _BSP_GLOBAL_H_*/
