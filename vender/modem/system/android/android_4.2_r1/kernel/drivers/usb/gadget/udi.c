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
#include "udi.h"


static char* acm_name_udi_type[] = {
    "/dev/acm_ctrl",          /*UDI_USB_ACM_CTRL*/
    "/dev/acm_at",            /*UDI_USB_ACM_AT*/
    "/dev/acm_c_shell",       /*UDI_USB_ACM_SHELL*/
    "/dev/acm_4g_diag",       /*UDI_USB_ACM_LTE_DIAG*/
    "/dev/acm_3g_diag",       /*UDI_USB_ACM_OM*/
    "/dev/acm_err",           /*UDI_USB_ACM_MODEM*/
    "/dev/acm_err",           /*UDI_USB_ACM_GPS*/
    "/dev/acm_a_shell",       /*UDI_USB_ACM_3G_GPS*/
    "/dev/acm_err",           /*UDI_USB_ACM_3G_PCVOICE*/
    "/dev/acm_err",           /*UDI_USB_ACM_PCVOICE*/
};

/*****************************************************************************
* 函 数 名  : udi_open
*
* 功能描述  : 打开设备(数据通道)
*
* 输入参数  : pParam: 设备的打开配置参数
* 输出参数  : 无
* 返 回 值  : -1:失败 / 其他:成功
*****************************************************************************/
UDI_HANDLE udi_open(UDI_OPEN_PARAM *pParam)
{
    UDI_ACM_DEV_TYPE type;
    char* name;
    struct file* filp;

    if (!pParam) {
        pr_emerg("%s, invalid param\n", __FUNCTION__);
        return (UDI_HANDLE)(BSP_ERROR);
    }
    type = (UDI_ACM_DEV_TYPE)(pParam->devid & 0xff);
    if (type > sizeof(acm_name_udi_type) / sizeof(char*)) {
        pr_emerg("%s, invalid type:%d\n", __FUNCTION__, type);
        return (UDI_HANDLE)(BSP_ERROR);
    }
    name = acm_name_udi_type[type];

    filp = filp_open(name, O_RDWR, 0);
    if (IS_ERR(filp)) {
        pr_emerg("%s, type:%d open fail:%d\n", __FUNCTION__, type, (int)filp);
        return (UDI_HANDLE)(BSP_ERROR);
    }

    return (UDI_HANDLE)filp;
}

/*****************************************************************************
* 函 数 名  : udi_close
*
* 功能描述  : 关闭设备(数据通道)
*
* 输入参数  : handle: 设备的handle
* 输出参数  : 无
* 返 回 值  : 成功/失败
*****************************************************************************/
BSP_S32 udi_close(UDI_HANDLE handle)
{
    struct file* filp = (struct file*)handle;

    if (unlikely((UDI_HANDLE)BSP_ERROR == handle || (UDI_HANDLE)0 == handle)) {
        pr_emerg("%s, invalid param\n", __FUNCTION__);
        return (UDI_HANDLE)(BSP_ERROR);
    }

    filp_close(filp, NULL);
    return 0;
}

/*****************************************************************************
* 函 数 名  : udi_write
*
* 功能描述  : 数据写
*
* 输入参数  : handle:  设备的handle
*             pMemObj: buffer内存 或 内存链表对象
*             u32Size: 数据写尺寸 或 内存链表对象可不设置
* 输出参数  :
*
* 返 回 值  : 完成字节数 或 成功/失败
*****************************************************************************/
BSP_S32 udi_write(UDI_HANDLE handle, void* pMemObj, BSP_U32 u32Size)
{
    mm_segment_t old_fs;
    struct file* filp = (struct file*)handle;
    BSP_S32 status;
    loff_t pos = 0;

    if (unlikely((UDI_HANDLE)BSP_ERROR == handle || (UDI_HANDLE)0 == handle)) {
        pr_emerg("%s, invalid param\n", __FUNCTION__);
        return (UDI_HANDLE)(BSP_ERROR);
    }

    old_fs = get_fs();
    set_fs(KERNEL_DS);
    status = vfs_write(filp, (void __force __user *)pMemObj, u32Size, &pos);
    set_fs(old_fs);
    return status;
}

/*****************************************************************************
* 函 数 名  : udi_read
*
* 功能描述  : 数据读
*
* 输入参数  : handle:  设备的handle
*             pMemObj: buffer内存 或 内存链表对象
*             u32Size: 数据读尺寸 或 内存链表对象可不设置
* 输出参数  :
*
* 返 回 值  : 完成字节数 或 成功/失败
*****************************************************************************/
BSP_S32 udi_read(UDI_HANDLE handle, void* pMemObj, BSP_U32 u32Size)
{
    mm_segment_t old_fs;
    struct file* filp = (struct file*)handle;
    BSP_S32 status;
    loff_t pos = 0;

    if (unlikely((UDI_HANDLE)BSP_ERROR == handle || (UDI_HANDLE)0 == handle)) {
        pr_emerg("%s, invalid param\n", __FUNCTION__);
        return (UDI_HANDLE)(BSP_ERROR);
    }

    old_fs = get_fs();
    set_fs(KERNEL_DS);
    status = vfs_read(filp, (void __force __user *)pMemObj, u32Size, &pos);
    set_fs(old_fs);
    return status;
}

/*****************************************************************************
* 函 数 名  : udi_ioctl
*
* 功能描述  : 数据通道属性配置
*
* 输入参数  : handle: 设备的handle
*             u32Cmd: IOCTL命令码
*             pParam: 操作参数
* 输出参数  :
*
* 返 回 值  : 成功/失败
*****************************************************************************/
BSP_S32 udi_ioctl(UDI_HANDLE handle, BSP_U32 u32Cmd, VOID* pParam)
{
    mm_segment_t old_fs;
    struct file* filp = (struct file*)handle;
    BSP_S32 status;

    if (unlikely((UDI_HANDLE)BSP_ERROR == handle || (UDI_HANDLE)0 == handle)) {
        pr_emerg("%s, invalid param\n", __FUNCTION__);
        return (UDI_HANDLE)(BSP_ERROR);
    }

    old_fs = get_fs();
    set_fs(KERNEL_DS);
    status = do_vfs_ioctl(filp, 0, (unsigned int)u32Cmd, (unsigned long)pParam);
    set_fs(old_fs);
    return status;
}


