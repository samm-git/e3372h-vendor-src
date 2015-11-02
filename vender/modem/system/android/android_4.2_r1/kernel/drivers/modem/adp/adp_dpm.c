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
#include "drv_wakelock.h"
#include "drv_dpm.h"

/*****************************************************************************
 函 数 名  : DRV_PWRCTRL_PWRUP
 功能描述  : W模、G模、L模下PA、RF、BBP、DSP、ABB、HIFI的上电控制。
 输入参数  : enCommMode: PWC_COMM_MODE_WCDMA/PWC_COMM_MODE_GSM/PWC_COMM_MODE_LTE,
             enCommModule:PWC_COMM_MODULE_PA/PWC_COMM_MODULE_RF/PWC_COMM_MODULE_BBP/PWC_COMM_MODULE_DSP/PWC_COMM_MODULE_ABB,
 输出参数  : None
 返 回 值  : PWC_PARA_INVALID/PWC_SUCCESS
*****************************************************************************/
BSP_U32 DRV_PWRCTRL_PWRUP(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
 {
    return PWC_COMM_MODEM_ON;
 }
/*****************************************************************************
 函 数 名  : DRV_PWRCTRL_PWRDOWN
 功能描述  : W模、G模、L模下PA、RF、BBP、DSP、ABB、HIFI的下电控制。
 输入参数  : enCommMode: PWC_COMM_MODE_WCDMA/PWC_COMM_MODE_GSM/PWC_COMM_MODE_LTE,
             enCommModule:PWC_COMM_MODULE_PA/PWC_COMM_MODULE_RF/PWC_COMM_MODULE_BBP/PWC_COMM_MODULE_DSP/PWC_COMM_MODULE_ABB,
 输出参数  : None
 返 回 值  : PWC_PARA_INVALID/PWC_SUCCESS
*****************************************************************************/
 BSP_U32 DRV_PWRCTRL_PWRDOWN (PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
 {
    return PWC_COMM_MODEM_OFF;
 }
/*****************************************************************************
 函 数 名  : BSP_PWRCTRL_PwrStatusGet
 功能描述  : W模、G模、L模下PA、RF、BBP、DSP、ABB的上下电状态查询。
 输入参数  : enCommMode: PWC_COMM_MODE_WCDMA/PWC_COMM_MODE_GSM/PWC_COMM_MODE_LTE,
             enCommModule:PWC_COMM_MODULE_PA/PWC_COMM_MODULE_RF/PWC_COMM_MODULE_BBP/PWC_COMM_MODULE_BBE16/PWC_COMM_MODULE_ABB,
 输出参数  : None
 返 回 值  : PWRCTRL_GET_SUCCESS/PWRCTRL_GET_PARAINVALID/PWRCTRL_ONORLOCK/PWRCTRL_OFFORUNLOCK
*****************************************************************************/
  BSP_U32 DRV_PWRCTRL_PWRSTATUSGET(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
 {
	return 0;
 }

/*****************************************************************************
 函 数 名  : DRV_PWRCTRL_PLL_ENABLE
 功能描述  : W模、G模、L模下PA、DSP、ABB的PLL使能。
 输入参数  : enCommMode: PWC_COMM_MODE_WCDMA/PWC_COMM_MODE_GSM/PWC_COMM_MODE_LTE,
             enCommModule:PWC_COMM_MODULE_PA/PWC_COMM_MODULE_DSP/PWC_COMM_MODULE_ABB,
 输出参数  : None
 返 回 值  : PWC_PARA_INVALID/PWC_SUCCESS
*****************************************************************************/
 BSP_U32 DRV_PWRCTRL_PLLENABLE (PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enCommModem)
 {
	return PWC_COMM_MODEM_ON;
 }

/*****************************************************************************
 函 数 名  : DRV_PWRCTRL_PLL_DISABLE
 功能描述  : W模、G模、L模下PA、DSP、ABB的PLL去使能。
 输入参数  : enCommMode: PWC_COMM_MODE_WCDMA/PWC_COMM_MODE_GSM/PWC_COMM_MODE_LTE,
             enCommModule:PWC_COMM_MODULE_PA/PWC_COMM_MODULE_DSP/PWC_COMM_MODULE_ABB,
 输出参数  : None
 返 回 值  : PWC_PARA_INVALID/PWC_SUCCESS
*****************************************************************************/
 BSP_U32 DRV_PWRCTRL_PLL_DISABLE (PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enCommModem)
 {
	return PWC_COMM_MODEM_OFF;
 }
/*****************************************************************************
* 函 数 名  : DRV_PWRCTRL_DSP_RESET
* 功能描述  : L模下DSP复位接口
* 输入参数  :
* 输出参数  :
* 返 回 值  :
*****************************************************************************/
 int  DRV_PWRCTRL_DSP_RESET(void)
 {
	return 0;
 }


/*****************************************************************************
 函 数 名  : BSP_PWRCTRL_PllStatusGet
 功能描述  : W模、G模、L模下PA、DSP、ABB的PLL状态查询。
 输入参数  : enCommMode: PWC_COMM_MODE_WCDMA/PWC_COMM_MODE_GSM/PWC_COMM_MODE_LTE,
             enCommModule:PWC_COMM_MODULE_PA/PWC_COMM_MODULE_DSP/PWC_COMM_MODULE_ABB,
 输出参数  : None
 返 回 值  : PWRCTRL_GET_SUCCESS/PWRCTRL_GET_PARAINVALID/PWRCTRL_ONORLOCK/PWRCTRL_OFFORUNLOCK
*****************************************************************************/
 BSP_U32 DRV_PWRCTRL_PLLSTATUSGET (PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enCommModem)
 {
	return 0;
 }

/*****************************************************************************
 函 数 名  : DRV_PWRCTRL_SLEEPVOTE_LOCK
 功能描述  : 外设禁止睡眠投票接口。
 输入参数  : enClientId:PWC_CLIENT_ID_E
 输出参数  : None
 返 回 值  : PWC_PARA_INVALID/PWC_SUCCESS
*****************************************************************************/
 BSP_U32 DRV_PWRCTRL_SLEEPVOTE_LOCK(PWC_CLIENT_ID_E enClientId)
 {
	return 0;
 }

/*****************************************************************************
 函 数 名  : DRV_PWRCTRL_SLEEPVOTE_UNLOCK
 功能描述  : 外设允许睡眠投票接口。
 输入参数  : enClientId:PWC_CLIENT_ID_E
 输出参数  : None
 返 回 值  : PWC_PARA_INVALID/PWC_SUCCESS
*****************************************************************************/
 BSP_U32 DRV_PWRCTRL_SLEEPVOTE_UNLOCK(PWC_CLIENT_ID_E enClientId)
 {
	return 0;
 }


/*****************************************************************************
 函 数 名  : BSP_PWRCTRL_StandbyStateCcpu/BSP_PWRCTRL_StandbyStateAcpu
 功能描述  : AT^PSTANDBY
 输入参数  :
 输出参数  :
 返回值：
*****************************************************************************/
 unsigned int BSP_PWRCTRL_StandbyStateAcpu(unsigned int ulStandbyTime, unsigned int ulSwitchTime)
 {
	return 0;
 }
 unsigned int BSP_PWRCTRL_StandbyStateCcpu(unsigned int ulStandbyTime, unsigned int ulSwitchTime)
 {
	return 0;
 }

/*****************************************************************************
 函 数 名  : BSP_GUSYS_RFLdoDown
 功能描述  : 本接口实现WCDMA和GSM RF LDO下电。
 输入参数  : 无。
 输出参数  : 无。
 返 回 值  : 无。
*****************************************************************************/
void BSP_GUSYS_RFLdoDown(void)
{

}
/*****************************************************************************
 函 数 名  : BSP_GUSYS_RFLdoOn
 功能描述  : 本接口实现WCDMA和GSM RF LDO上电。
 输入参数  : 无。
 输出参数  : 无。
 返 回 值  : 无。
*****************************************************************************/
void BSP_GUSYS_RFLdoOn(void)
{

}

 /*****************************************************************************
 函 数 名  : BSP_GUSYS_BBPAutoAdjust
 功能描述  : 初始化ABB的自校准使能
 输入参数  : uiSysMode：0：WCDMA模式；1：GSM模式。
 输出参数  : 无。
 返 回 值  : 0:  操作成功；
             -1：操作失败。
*****************************************************************************/
int  BSP_GUSYS_BBPAutoAdjust(unsigned char ucAdjustMode)
{
    return OK;
}


