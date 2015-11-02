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


#include "bsp_om_api.h"
#include "bsp_om.h"
#include "drv_comm.h"
#include "osl_irq.h"


#ifdef __cplusplus
extern "C"
{
#endif

#ifndef ENABLE_BUILD_OM
/*****************************************************************************
* 函 数 名  : DRV_OM_SET_HSO_CONN_FLAG
*
* 功能描述  : flag :连接标志，1表示连接，0表示断开
*
* 输入参数  :无
*
*
* 输出参数  :无
*
* 返 回 值  :无
*****************************************************************************/

BSP_VOID DRV_OM_SET_HSO_CONN_FLAG(BSP_U32 flag)
{
    return;
}

/*****************************************************************************
* 函 数 名  : DRV_SYSVIEW_SWT_SET
*
* 功能描述  :sysview上报开关设置，封装函数，提供给MSP使用
*
* 输入参数  :tarce_type :trace类型
*                        set_swt  :开关值
*                       period  :上报周期
*
* 输出参数  : 无
*
* 返 回 值  : BSP_OK
*****************************************************************************/

BSP_U32 DRV_SYSVIEW_SWT_SET(BSP_U32 set_type,BSP_U32 set_swt,BSP_U32 period)
{
    return 0;
}

/*****************************************************************************
* 函 数 名  : DRV_GET_ALL_TASK_ID_NAME
*
* 功能描述  :获取当前所有任务的ID和名称
*
* 输入参数  :p_task_stru:任务名称和ID输出的结构体
*                       param_len:参数1的长度，单位字节
*
*
* 输出参数  : 无
*
* 返 回 值  : BSP_OK
*****************************************************************************/
BSP_U32 DRV_GET_ALL_TASK_ID_NAME(BSP_VOID *p_task_stru,u32 param_len)
{
      return 0;
}


/*****************************************************************************
* 函 数 名  : DRV_INT_LOCK
*
* 功能描述  :锁中断接口，用于锁中断统计
*
* 输入参数  :无
*
* 输出参数  : 无
*
* 返 回 值  : 锁中断后获得的key
*****************************************************************************/
BSP_S32 DRV_INT_LOCK(BSP_VOID)
{
    return 0;
}

/*****************************************************************************
* 函 数 名  : DRV_INT_UNLOCK
*
* 功能描述  : 解锁中断接口，用于锁中断统计
*
* 输入参数  : 锁中断后获得的key
*
* 输出参数  : 无
*
* 返 回 值  : 无
*****************************************************************************/
BSP_VOID DRV_INT_UNLOCK(BSP_S32 LockKey)
{
    return;
}


/*****************************************************************************
* 函 数 名  : DRV_SOCP_CHAN_ENABLE
*
* 功能描述  : 底软SOCP通道使能操作
*
* 输入参数  : 无
*
*
* 输出参数  :无
*
* 返 回 值  :无
*****************************************************************************/

BSP_VOID DRV_SOCP_CHAN_ENABLE(BSP_VOID)
{
    return;
}

/*****************************************************************************
* 函 数 名  : DRV_LOG_BIN_IND
*
* 功能描述  : 底软主动上报接口
*
* 输入参数  : 
*
*
* 输出参数  :无
*
* 返 回 值  :无
*****************************************************************************/
BSP_VOID DRV_LOG_BIN_IND(BSP_S32 str_id, BSP_VOID* ind_data, BSP_S32 ind_data_size)
{
	return;
}

/*****************************************************************************
* 函 数 名  : DRV_LOG_BIN_IND_REGISTER
*
* 功能描述  : 底软主动上报注册接口
*
* 输入参数  : 
*
*
* 输出参数  :无
*
* 返 回 值  :无
*****************************************************************************/
BSP_VOID DRV_LOG_BIN_IND_REGISTER(log_bin_ind_cb log_ind_cb)
{
	return;
}

#else
/*****************************************************************************
* 函 数 名  : DRV_OM_SET_HSO_CONN_FLAG
*
* 功能描述  : flag :连接标志，1表示连接，0表示断开
*
* 输入参数  :无
*
*
* 输出参数  :无
*
* 返 回 值  :无
*****************************************************************************/

BSP_VOID DRV_OM_SET_HSO_CONN_FLAG(BSP_U32 flag)
{
    bsp_om_set_hso_conn_flag(flag);
}

#ifndef ENABLE_BUILD_SYSVIEW
/*****************************************************************************
* 函 数 名  : DRV_SYSVIEW_SWT_SET
*
* 功能描述  :sysview上报开关设置，封装函数，提供给MSP使用
*
* 输入参数  :tarce_type :trace类型
*                        set_swt  :开关值
*                       period  :上报周期
*
* 输出参数  : 无
*
* 返 回 值  : BSP_OK
*****************************************************************************/

BSP_U32 DRV_SYSVIEW_SWT_SET(BSP_U32 set_type,BSP_U32 set_swt,BSP_U32 period)
{
    return 0;
}

/*****************************************************************************
* 函 数 名  : DRV_GET_ALL_TASK_ID_NAME
*
* 功能描述  :获取当前所有任务的ID和名称
*
* 输入参数  :p_task_stru:任务名称和ID输出的结构体
*                       param_len:参数1的长度，单位字节
*
*
* 输出参数  : 无
*
* 返 回 值  : BSP_OK
*****************************************************************************/
BSP_U32 DRV_GET_ALL_TASK_ID_NAME(BSP_VOID *p_task_stru,u32 param_len)
{
      return 0;
}

#else
/*****************************************************************************
* 函 数 名  : DRV_SYSVIEW_SWT_SET
*
* 功能描述  :sysview上报开关设置，封装函数，提供给MSP使用
*
* 输入参数  :tarce_type :trace类型
*                        set_swt  :开关值
*                       period  :上报周期
*
* 输出参数  : 无
*
* 返 回 值  : BSP_OK
*****************************************************************************/

BSP_U32 DRV_SYSVIEW_SWT_SET(BSP_U32 set_type,BSP_U32 set_swt,BSP_U32 period)
{
    return bsp_sysview_swt_set(set_type,set_swt,period);
}

/*****************************************************************************
* 函 数 名  : DRV_GET_ALL_TASK_ID_NAME
*
* 功能描述  :获取当前所有任务的ID和名称
*
* 输入参数  :p_task_stru:任务名称和ID输出的结构体
*                       param_len:参数1的长度，单位字节
*
*
* 输出参数  : 无
*
* 返 回 值  : BSP_OK
*****************************************************************************/
BSP_U32 DRV_GET_ALL_TASK_ID_NAME(BSP_VOID *p_task_stru,u32 param_len)
{
      return sysview_get_all_task_name((void *)p_task_stru,param_len );
}
#endif

/*****************************************************************************
* 函 数 名  : DRV_INT_LOCK
*
* 功能描述  :锁中断接口，用于锁中断统计
*
* 输入参数  :无
*
* 输出参数  : 无
*
* 返 回 值  : 锁中断后获得的key
*****************************************************************************/
BSP_S32 DRV_INT_LOCK(BSP_VOID)
{
    unsigned long flags;

    local_irq_save(flags);

    bsp_int_lock_enter();
    return (BSP_S32)flags;
}

/*****************************************************************************
* 函 数 名  : DRV_INT_UNLOCK
*
* 功能描述  : 解锁中断接口，用于锁中断统计
*
* 输入参数  : 锁中断后获得的key
*
* 输出参数  : 无
*
* 返 回 值  : 无
*****************************************************************************/
BSP_VOID DRV_INT_UNLOCK(BSP_S32 LockKey)
{
    unsigned long lock_key;

    lock_key = (unsigned long)LockKey;
    
    bsp_int_lock_out();

    local_irq_restore(lock_key);
}


/*****************************************************************************
* 函 数 名  : DRV_SOCP_CHAN_ENABLE
*
* 功能描述  : 底软SOCP通道使能操作
*
* 输入参数  : 无
*
*
* 输出参数  :无
*
* 返 回 值  :无
*****************************************************************************/

BSP_VOID DRV_SOCP_CHAN_ENABLE(BSP_VOID)
{
    bsp_socp_chan_enable();
}


#endif

#ifdef __cplusplus
}
#endif


