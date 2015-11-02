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
/*lint --e{537}*/
#include "drv_comm.h"
#include "bsp_bbp.h"
#include "drv_bbp.h"
#include "product_config.h"

#ifdef CONFIG_BBP_INT
/*****************************************************************************
* 函数  : BSP_BBPGetCurTime
* 功能  : 被PS调用，用来获取系统精确时间
* 输入  : void
* 输出  : u64 *pcurtime
* 返回  : u32
*****************************************************************************/
BSP_U32 BSP_BBPGetCurTime(BSP_U64 *pCurTime)
{
    return bsp_bbp_getcurtime(pCurTime);
}

/*****************************************************************************
* 函数  : BSP_BBPGetCurTime
* 功能  : 获取BBP定时器的值。用于OAM 时戳
* 输入  : void
* 输出  :
			pulLow32bitValue指针参数不能为空，否则会返回失败。
			pulHigh32bitValue如果为空， 则只返回低32bit的值。
* 返回  : int
*****************************************************************************/
int DRV_GET_BBP_TIMER_VALUE(unsigned int  *pulHigh32bitValue,  unsigned int  *pulLow32bitValue)
{
	BSP_U64 CurTime;

	if(BSP_NULL == pulLow32bitValue)
	{
		return -1;
	}

	bsp_bbp_getcurtime(&CurTime);

	if(BSP_NULL != pulHigh32bitValue)
	{
		*pulHigh32bitValue = (CurTime>>32);
	}

	*pulLow32bitValue = CurTime & 0xffffffff;

	return 0;
}
#else
BSP_U32 BSP_BBPGetCurTime(BSP_U64 *pCurTime)
{
    return (BSP_U32)0;
}

/*****************************************************************************
* 函数  : BSP_BBPGetCurTime
* 功能  : 获取BBP定时器的值。用于OAM 时戳
* 输入  : void
* 输出  :
			pulLow32bitValue指针参数不能为空，否则会返回失败。
			pulHigh32bitValue如果为空， 则只返回低32bit的值。
* 返回  : int
*****************************************************************************/
int DRV_GET_BBP_TIMER_VALUE(unsigned int  *pulHigh32bitValue,  unsigned int  *pulLow32bitValue)
{
	return (int)0;
}
/*****************************************************************************
* 函 数 : bsp_bbp_getcurtime
* 功 能 : bbp 初始化
* 输 入 : void
* 输 出 : void
* 返 回 : 模块裁剪时，打桩
* 说 明 :
*****************************************************************************/
u32  bsp_bbp_getcurtime(u64 *pcurtime)
{
	return 0;
}
#endif
