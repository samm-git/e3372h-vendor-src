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
#include <osl_bio.h>
#include <bsp_ipc.h>
#include <hi_pmu.h>
#include <bsp_pmu.h>
#include <osl_sem.h>
#include <pmu_balong.h>
#include <osl_spinlock.h>
#if defined(__KERNEL__)
#include <osl_irq.h>
#endif
#include "common_hi6451.h"

struct hi6551_common_data{
    spinlock_t      lock;
};
struct hi6551_common_data g_hi6451_comm_st;
#if defined(__KERNEL__)
/*32K时钟偏移*/
/*para check func*/
static __inline__ int hi6451_32k_para_check(pmu_clk_e clk_id)
{
    if(PMU_32K_CLK_B < clk_id || PMU_32K_CLK_A > clk_id)
        return BSP_PMU_ERROR;
    else
        return BSP_PMU_OK;
}
/*****************************************************************************
 函 数 名  : hi6451_32k_clk_enable
 功能描述  : 开启对应路32k时钟
 输入参数  : pmu 32k 时钟枚举值
 输出参数  : 无
 返 回 值  : 操作成功或失败
 调用函数  :
 被调函数  : pmu adp文件调用
*****************************************************************************/
int hi6451_32k_clk_enable(pmu_clk_e clk_id)
{
    s32 iRetVal = 0;
    u8 iregVal = 0;
    hi6451_commflags_t flags;

    if(hi6451_32k_para_check(clk_id))
        return BSP_PMU_ERROR;

    /*核内互斥*/
    spin_lock_irqsave(&g_hi6451_comm_st.lock,flags);
    iRetVal |= bsp_hi6451_reg_read(HI_PMU_ONOFF7_OFFSET,&iregVal);
    iregVal |= 0x02;
    iRetVal |= bsp_hi6451_reg_write(HI_PMU_ONOFF7_OFFSET, iregVal);
    spin_unlock_irqrestore(&g_hi6451_comm_st.lock,flags);

    return iRetVal;
}
/*****************************************************************************
 函 数 名  : hi6451_32k_clk_disable
 功能描述  : 关闭对应路32k时钟
 输入参数  : pmu 32k 时钟枚举值
 输出参数  : 无
 返 回 值  : 操作成功或失败
 调用函数  :
 被调函数  : pmu adp文件调用
*****************************************************************************/
int hi6451_32k_clk_disable(pmu_clk_e clk_id)
{
    s32 iRetVal = 0;
    u8 iregVal = 0;
    hi6451_commflags_t flags;

    if(hi6451_32k_para_check(clk_id))
        return BSP_PMU_ERROR;

    if(PMU_32K_CLK_A == clk_id)
    {
        pmic_print_info("soc use,can not be closed!\n");
        return BSP_PMU_ERROR;
    }

    /*核内互斥*/
    spin_lock_irqsave(&g_hi6451_comm_st.lock,flags);
    iRetVal |= bsp_hi6451_reg_read(HI_PMU_ONOFF7_OFFSET,&iregVal);
    iregVal &= ~0x02;
    iRetVal |= bsp_hi6451_reg_write(HI_PMU_ONOFF7_OFFSET, iregVal);
    spin_unlock_irqrestore(&g_hi6451_comm_st.lock,flags);

    return iRetVal;
}
/*****************************************************************************
 函 数 名  : hi6451_32k_clk_is_enabled
 功能描述  : 查询对应路32k时钟是否开启
 输入参数  : pmu 32k 时钟枚举值
 输出参数  : 无
 返 回 值  : 操作成功或失败
 调用函数  :
 被调函数  : pmu adp文件调用
*****************************************************************************/
int hi6451_32k_clk_is_enabled(pmu_clk_e clk_id)
{
    s32 iRetVal = 0;
    u8 iregVal = 0;

    if(hi6451_32k_para_check(clk_id))
        return BSP_PMU_ERROR;

    iRetVal = bsp_hi6451_reg_read(HI_PMU_ONOFF7_OFFSET,&iregVal);
    return (int)(iregVal & (0x1 << 1));
}

/*****************************************************************************
 函 数 名  : hi6451_power_key_state_get
 功能描述  : 获取usb是否插拔状态
 输入参数  : void
 输出参数  : 无
 返 回 值  : power键是否按下：1:按下；0:未按下)
 调用函数  :
 被调函数  : 开关机模块
*****************************************************************************/
bool hi6451_power_key_state_get(void)
{
    u8 regval = 0;

    bsp_hi6451_reg_read(HI_PMU_STATUS4_OFFSET, &regval);
    return  (regval & PMU_HI6451_POWER_KEY_MASK) ? 1 : 0;
}
/*****************************************************************************
 函 数 名  : bsp_hi6451_usb_state_get
 功能描述  : 获取usb是否插拔状态
 输入参数  : void
 输出参数  : 无
 返 回 值  : usb插入或拔出(1:插入；0:拔出)
 调用函数  :
 被调函数  : 开关机模块
*****************************************************************************/
bool hi6451_usb_state_get(void)
{

    u8 regval = 0;

    bsp_hi6451_reg_read(HI_PMU_STATUS5_OFFSET, &regval);
    return  (regval & PMU_HI6451_USB_STATE_MASK) ? 1 : 0;

}
#endif

/*****************************************************************************
 函 数 名  : hi6451_version_get
 功能描述  : 获取pmu的版本号
 输入参数  : void
 输出参数  : 无
 返 回 值  : pmu版本号
 调用函数  :
 被调函数  : HSO集成,MSP调用
*****************************************************************************/
u8  hi6451_version_get(void)
{
    u8 regval = 0;

    bsp_hi6451_reg_read(HI_PMU_VERSION_OFFSET, &regval);
    return  regval;
}
/*****************************************************************************
 函 数 名  : bsp_hi6451_common_init
 功能描述  : PMIC HI6451 common模块初始化
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 备注说明  : 初始化app互斥信号量
*****************************************************************************/
void bsp_hi6451_common_init(void)
{
    spin_lock_init(&g_hi6451_comm_st.lock);/*中断只在Acore实现，多core*/
}
