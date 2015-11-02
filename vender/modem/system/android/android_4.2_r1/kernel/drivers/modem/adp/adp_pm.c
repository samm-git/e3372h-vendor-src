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
#include <drv_pm.h>
#include <bsp_pm.h>

/*
set&unset wakeup interrupt source
input : wake_src defined in drv_pm.h [DRV_PM_WAKE_SRC_ENUM]
*/
BSP_VOID DRV_PM_SET_WAKE_SRC(DRV_PM_WAKE_SRC_ENUM wake_src)
{
    pm_enable_wake_src(wake_src);
}

BSP_VOID DRV_PM_UNSET_WAKE_SRC(DRV_PM_WAKE_SRC_ENUM wake_src)
{

    pm_disable_wake_src(wake_src);
}


/*****************************************************************************
 函 数 名  : DRV_PWRCTRL_DEEPSLEEP_FOREVER
 功能描述  : AARM CARM 下电接口
 输入参数  : None
 输出参数  : None
 返 回 值  : None

*****************************************************************************/
 void DRV_PWRCTRL_DEEPSLEEP_FOREVER(void )
 {}

/*****************************************************************************
 函 数 名  : DRV_PWRCTRL_SLEEP_CALLBACK_REG
 功能描述  : 注册PWC函数给底软使用
 输入参数  : 待注册的函数
 输出参数  : None
 返 回 值  : 0:  操作成功；
             -1：操作失败。
*****************************************************************************/
 int DRV_PWRCTRL_SLEEP_CALLBACK_REG(PWC_SLEEP_CALLBACK_STRU pCallback)
 {
	return 0;
 }


/*****************************************************************************
Function:   DRV_PWRCTRL_SLEEP_IN_CB
Description:
Input:
Output:     None;
Return:
Others:
*****************************************************************************/
 int DRV_PWRCTRL_SLEEP_IN_CB(PWC_DS_SOCP_CB_STRU stFunc)
 {
	return 0;
 }



