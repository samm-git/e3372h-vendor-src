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
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#include "drv_pcsc.h"



/*****************************************************************************
* 函 数 名  : pcsc_usim_ctrl_cmd
*
* 功能描述  : USIMM模块处理PCSC下发命令，完成后，调此函数通知PCSC
*
* 输入参数  : cmd_type:USIMM处理命令
*             status:  处理的处理结果，是否成功
*             buf:   命令处理后的回应数据
*             length:回应数据的长度
* 输出参数  : 无
* 返 回 值  : 0:命令处理完成; <0:命令处理错误
*****************************************************************************/
unsigned long pcsc_usim_ctrl_cmd(unsigned long cmd_type, unsigned long status, 
                unsigned char *buf, unsigned long length)
{
    int ret;
    ret = gusim_ctrl_cmd(0, (u32)cmd_type, (u32)status, (u8*)buf, (u32)length);

    return (unsigned long)ret;
}


/*****************************************************************************
* 函 数 名  : pcsc_usim_int
*
* 功能描述  : USIM模块注册回调函数接口
*
* 输入参数  : pFun: USIMM模块处理函数指针
* 输出参数  : 无
* 返 回 值  : 无
*****************************************************************************/
void pcsc_usim_int(pFunAPDUProcess pFun1, GetCardStatus pFun2)
{
    gusim_reg_cb(0, pFun1, pFun2);
}


