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

#include "drv_ncm.h"
#include "bsp_ncm.h"
#include "drv_udi.h"
#include "ncm_balong.h"

/*****************************************************************************
* 函 数 名  : bsp_ncm_open
*
* 功能描述  : 用来获取可以使用NCM设备ID,数据通道类型，PS使用;控制通道类型，
*              MSP传输AT命令使用
*
* 输入参数  : NCM_DEV_TYPE_E enDevType  设备类型
* 输出参数  : pu32NcmDevId              NCM 设备ID
*
* 返 回 值  : BSP_ERR_NET_NOIDLEDEV
*             OK
*
* 修改记录  : 2010年3月17日   liumengcun  creat
*****************************************************************************/
BSP_U32 bsp_ncm_open(NCM_DEV_TYPE_E enDevType, BSP_U32 pu32NcmDevId)
{
    return (BSP_U32)ncm_vendor_open(enDevType, pu32NcmDevId);
}

/*****************************************************************************
* 函 数 名  : bsp_ncm_write
*
* 功能描述  : 发送数据
*
* 输入参数  : BSP_U32 u32NcmDevId  NCM设备ID
                             void *     net_priv tcp/ip传来的参数，不和tcp/ip对接时该参数设置为NULL
* 输出参数  : BSP_VOID *pPktEncap       包封装首地址
*
* 返 回 值  : BSP_OK
*             BSP_ERR_NET_INVALID_PARA
*             BSP_ERR_NET_BUF_ALLOC_FAILED
*
* 修改记录  : 2010年3月17日   liumengcun  creat
*****************************************************************************/
BSP_U32 bsp_ncm_write(BSP_U32 u32NcmDevId, BSP_VOID *pPktEncap, void *net_priv)
{
    return (BSP_U32)ncm_vendor_write(u32NcmDevId, pPktEncap, net_priv);
}

/*****************************************************************************
* 函 数 名  : bsp_ncm_ioctl
*
* 功能描述  : 配置NCM设备属性
*
* 输入参数  : 无
* 输出参数  : 无
*
* 返 回 值  : 
*
* 修改记录  : 2010年3月17日   liumengcun  creat
*****************************************************************************/
BSP_U32 bsp_ncm_ioctl(BSP_U32 u32NcmDevId, NCM_IOCTL_CMD_TYPE_E enNcmCmd, BSP_VOID *param)
{
    return ncm_vendor_ioctl(u32NcmDevId, enNcmCmd, param);/* [false alarm]:fortify disable */
}

/*****************************************************************************
* 函 数 名  : bsp_ncm_close
*
* 功能描述  : 关闭NCM设备
*
* 输入参数  : NCM_DEV_TYPE_E enDevType   设备ID类型
*             BSP_U32 u32NcmDevId        NCM设备ID
* 输出参数  : 无
*
* 返 回 值  : OK
*
* 修改记录  : 2010年3月17日   liumengcun  creat
*****************************************************************************/
BSP_U32 bsp_ncm_close(NCM_DEV_TYPE_E enDevType, BSP_U32 u32NcmDevId)
{
    return ncm_vendor_close(enDevType, u32NcmDevId);/* [false alarm]:fortify disable */
}
