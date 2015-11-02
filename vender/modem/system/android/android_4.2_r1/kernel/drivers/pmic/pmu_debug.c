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
#include "pmu_balong.h"

struct pmu_dbg g_pmu_dbg;

/*函数声明*/
void bsp_pmu_dbg_enable(void);
void bsp_pmu_dbg_disable(void);
void bsp_pmu_volt_state(void);
void bsp_pmu_om_data_show(void);
void bsp_pmu_om_boot_show(void);
void bsp_pmu_exc_state(void);
void bsp_pmu_pmic_info(void);

/*函数实现*/
/*****************************************************************************
 函 数 名  : bsp_pmu_dbg_enable
 功能描述  :打开pmu模块dbg信息
 输入参数  : void
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  : 异常定位
*****************************************************************************/
void bsp_pmu_dbg_enable(void)
{
    g_pmu_dbg.info_print_sw = 1;
}
/*****************************************************************************
 函 数 名  : bsp_pmu_dbg_disable
 功能描述  :关闭pmu模块dbg信息
 输入参数  : void
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  :异常定位
*****************************************************************************/
void bsp_pmu_dbg_disable(void)
{
    g_pmu_dbg.info_print_sw = 0;
}
/*****************************************************************************
 函 数 名  : bsp_pmu_dbg_register
 功能描述  : pmu模块dbg信息注册
 输入参数  : void
 输出参数  : 无
 返 回 值  : 注册成功或失败
 调用函数  :
 被调函数  : 异常定位
*****************************************************************************/
int bsp_pmu_dbg_register(pmic_id_e pmic_id,struct pmu_dbg_ops ops_func)
{
    if(PMIC_BUTTOM <= pmic_id)
    {
        pmic_print_info("*****bsp_pmu_all_volt_state*****\n");
        return  BSP_PMU_PARA_ERROR;
    }

    g_pmu_dbg.dbg_ops[pmic_id]= ops_func;

    return  BSP_PMU_OK;

}
/*****************************************************************************
 函 数 名  : bsp_pmu_volt_state
 功能描述  : 显示当前PMIC的所有路电源的使用信息(包括开关，电压，)
 输入参数  : void
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  :异常定位
*****************************************************************************/
void bsp_pmu_volt_state(void)
{
    int i = 0;

    pmic_print_info("*****bsp_pmu_all_volt_state*****\n");
    for(i = 0;i < PMIC_BUTTOM;i++)
    {
        if(NULL != g_pmu_dbg.dbg_ops[i].pmu_volt_state)
            g_pmu_dbg.dbg_ops[i].pmu_volt_state();
    }
}
/*****************************************************************************
 函 数 名  : bsp_pmu_om_data_show
 功能描述  : 显示此次运行过程中当前PMU状态的异常信息
 输入参数  : void
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  : 异常定位
*****************************************************************************/
void bsp_pmu_om_data_show(void)
{
    int i = 0;

    pmic_print_info("*****bsp_pmu_om_data_show*****\n");
    for(i = 0;i < PMIC_BUTTOM;i++)
    {
        if(NULL != g_pmu_dbg.dbg_ops[i].pmu_om_data_show)
            g_pmu_dbg.dbg_ops[i].pmu_om_data_show();
    }
}
/*****************************************************************************
 函 数 名  : bsp_pmu_boot_om_show
 功能描述  : 显示此次运行过程中当前PMU状态的异常信息
 输入参数  : void
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  : 异常定位
*****************************************************************************/
void bsp_pmu_om_boot_show(void)
{
    int i = 0;

    pmic_print_info("*****bsp_pmu_boot_om_show*****\n");
    for(i = 0;i < PMIC_BUTTOM;i++)
    {
        if(NULL != g_pmu_dbg.dbg_ops[i].pmu_om_boot_show)
            g_pmu_dbg.dbg_ops[i].pmu_om_boot_show();
    }
}
/*****************************************************************************
 函 数 名  : bsp_pmu_exc_state
 功能描述  : 显示当前所有PMIC的所有异常信息
 输入参数  : void
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  :异常定位
*****************************************************************************/
void bsp_pmu_exc_state(void)
{
    int i = 0;

    pmic_print_info("*****bsp_pmu_exc_state*****\n");
    for(i = 0;i < PMIC_BUTTOM;i++)
    {
        if(NULL != g_pmu_dbg.dbg_ops[i].pmu_exc_state)
            g_pmu_dbg.dbg_ops[i].pmu_exc_state();
    }
}
/*****************************************************************************
 函 数 名  : bsp_pmu_pmic_info
 功能描述  : 显示当前所有PMIC的版本信息
 输入参数  : void
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  : 异常定位
*****************************************************************************/
void bsp_pmu_pmic_info(void)
{
    int i = 0;

    pmic_print_info("*****bsp_pmu_pmic_info*****\n");
    for(i = 0;i < PMIC_BUTTOM;i++)
    {
        if(NULL != g_pmu_dbg.dbg_ops[i].pmu_pmic_info)
            g_pmu_dbg.dbg_ops[i].pmu_pmic_info();
    }
}

