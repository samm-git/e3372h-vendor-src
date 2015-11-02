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

#include "drv_comm.h"
#include "sdio_slave.h"
#include "sdio_slave_osi.h"
#include "sdio_slave_hal.h"
#include "sdio_slave_cfg.h"
#include "sdio_slave_errcode.h"
extern BSP_S32 slave_init(void);
extern BSP_S32 SLAVE_Open(void);
extern BSP_VOID SLAVE_Close(BSP_S32 u32SlaveDevId);
extern BSP_S32 SLAVE_Ioctl(BSP_S32 s32SlaveDevId, BSP_U32 cmd, BSP_U32 arg);
extern BSP_S32 SLAVE_Write(BSP_S32 s32SlaveDevId, BSP_U8 *pBuf, BSP_U8 *pBufPhy, BSP_U32 u32Size);
extern BSP_S32 SLAVE_Read(BSP_S32 u32SlaveDevId, BSP_U8 *pBuf, BSP_U32 u32Size);

/*设备打开标志*/
static BSP_BOOL bModuleOpen = BSP_FALSE;

BSP_S32 BSP_SLAVE_Init(BSP_VOID)
{
    return slave_init();
}
EXPORT_SYMBOL(BSP_SLAVE_Init);

/*****************************************************************************
* 函 数 名  : BSP_SLAVE_Open
*
* 功能描述  : SLAVE 设备打开
*
* 输入参数  : 无
*           
* 输出参数  : 无
* 返 回 值  : 设备ID
*****************************************************************************/
BSP_S32 BSP_SLAVE_Open(BSP_VOID)
{
    BSP_S32 ret = 0;

    if(bModuleOpen)
    {
        return BSP_ERROR;
    }
    
    //SLAVE_Init();
    
    /*打开设备*/
    ret = SLAVE_Open();
    if(!ret)
    {
        return BSP_ERROR;
    }
    
    bModuleOpen = BSP_TRUE;

    return ret;
}
EXPORT_SYMBOL(BSP_SLAVE_Open);

/*****************************************************************************
* 函 数 名  : BSP_SLAVE_Close
*
* 功能描述  : SLAVE 设备关闭
*
* 输入参数  : s32SlaveDevId:  设备ID
*           
* 输出参数  : 无
* 返 回 值  : 成功/失败
*****************************************************************************/
BSP_S32 BSP_SLAVE_Close(BSP_S32 s32SlaveDevId)
{
    if(BSP_FALSE ==bModuleOpen)
    {
        printk("dev was closed\n");
        return BSP_ERROR;
    }

    bModuleOpen = BSP_FALSE;
    
    SLAVE_Close(s32SlaveDevId);
    return BSP_OK;

}
EXPORT_SYMBOL(BSP_SLAVE_Close);

/*****************************************************************************
* 函 数 名  : BSP_SLAVE_Ioctl
*
* 功能描述  : SLAVE IOCTL接口
*
* 输入参数  : handle:  设备句柄
*             cmd:     命令类型
*             u32Size: 源数据的字节数
* 输出参数  : 无
* 返 回 值  : 成功/失败
*****************************************************************************/
BSP_S32 BSP_SLAVE_Ioctl(BSP_S32 handle, BSP_U32 cmd, BSP_U32 arg)
{
    if(BSP_TRUE != bModuleOpen)
    {
        return BSP_ERROR;   
    }
    else 
        return SLAVE_Ioctl(handle,cmd,arg);

}
EXPORT_SYMBOL(BSP_SLAVE_Ioctl);

/*****************************************************************************
* 函 数 名  : BSP_SLAVE_Write
*
* 功能描述  : SLAVE 写接口
*
* 输入参数  : handle:  设备句柄
*             pBuf:    源数据buffer的首地址
*             u32Size: 源数据的字节数
* 输出参数  : 无
* 返 回 值  : 状态/写入字节数
*****************************************************************************/
BSP_S32 BSP_SLAVE_Write(BSP_S32 handle, BSP_U8 *buf, BSP_U8 *bufPhy, BSP_U32 len)
{
    if(BSP_TRUE !=bModuleOpen)
    {
        return BSP_ERROR;
    }
    else 
        return SLAVE_Write(handle,buf, bufPhy, len);
}
EXPORT_SYMBOL(BSP_SLAVE_Write);

/*****************************************************************************
* 函 数 名  : BSP_SLAVE_Read
*
* 功能描述  : SLAVE 阻塞读接口
*
* 输入参数  : handle:  设备句柄
*             pBuf:    接收buffer的首地址
*             u32Size: 源数据的字节数
* 输出参数  : 无
* 返 回 值  : 状态/写入字节数
*****************************************************************************/
BSP_S32 BSP_SLAVE_Read(BSP_S32 handle, BSP_U8 *buf, BSP_U32 len)
{
    if(BSP_TRUE !=bModuleOpen)
    {
        return BSP_ERROR;
    }
    else 
        return SLAVE_Read(handle,buf,len);
}
EXPORT_SYMBOL(BSP_SLAVE_Read);
