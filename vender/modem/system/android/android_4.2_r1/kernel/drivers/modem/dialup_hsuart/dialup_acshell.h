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
#ifndef    _HSUART_ACSHELL_H_
#define    _HSUART_ACSHELL_H_

#ifdef __cplusplus
extern "C" {
#endif
/**************************************************************************
  头文件包含                            
**************************************************************************/
#include "dialup_hsuart.h"
#include "drv_hsuart.h"
#include "hi_uart.h"

/*****************************************************************************
  宏定义
*****************************************************************************/

#define ACSHELL_TX_BUFF_SIZE 	(8*1024)
#define DIAL_HSUART_CSHELL_BIT	(3)
#define HSUART_BASE_VIRT_ADDR	(0xc2000000)
/*****************************************************************************
  结构体定义
*****************************************************************************/
struct acshell_str
{
	volatile u32 ulread;
	volatile u32 ulwrite;
	u8 uldata[1024];
	u8 *ptxbuff;
	u8 shell_flag;
}acshell_recv_str;

struct  _HSUART_RX_OPS_
{
    int (*shell_rx)(u8 *buff,u32 lenth);
}hsuart_ops;

/*****************************************************************************
  函数声明
*****************************************************************************/
extern struct console* bsp_get_uart_console(void);
extern int cshell_set_bit(int num_from_zero);
extern int cshell_clear_bit(int num_from_zero);
extern int cshell_recv_from_dial(u8 *buf, u32 len);

#ifdef __cplusplus /* __cplusplus */
}
#endif /* __cplusplus */
#endif

