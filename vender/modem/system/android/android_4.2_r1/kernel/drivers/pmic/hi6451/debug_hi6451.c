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
#ifdef __KERNEL__
#include "exc_hi6451.h"
#endif
#include "common_hi6451.h"

/*电源名称*/
char* hi6451_volt_name[PMIC_HI6451_VOLT_MAX]={"BUCK0","BUCK1","BUCK2","BUCK3","BUCK4","BUCK5","BUCK6", \
"LDO1","LDO2","LDO3","LDO4","LDO5","LDO6","LDO7","LDO8","LDO9","LDO10","LDO11","LDO12","LDO13","LDO14", \
"LDO15","DR1","DR2","DR3"};
char* hi6451_mode_name[PMU_MODE_BUTTOM]={"NORMAL","ECO","","","NONE_MODE"};

/*异常状态记录*/
extern u8 pmu_hi6451_om_data[6];
extern u8 pmu_hi6451_om_boot[3];

/*para check func*/
static __inline__ int hi6451_volt_para_check(int volt_id)
{
    /* 有效性检查*/
    if ((PMIC_HI6451_VOLT_MAX < volt_id) || (PMIC_HI6451_VOLT_MIN > volt_id))
    {
        pmic_print_error("PMIC doesn't have this volt ,the volt you can set is from %d to %d,please check!\n",PMIC_HI6451_VOLT_MIN,PMIC_HI6451_VOLT_MAX);
        return BSP_PMU_ERROR;
    }
}

/*****************************************************************************
 函 数 名  : bsp_hi6451_volt_show
 功能描述  : 显示当前所需路电源的使用信息(包括开关，电压，)
 输入参数  : void
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  :异常定位
*****************************************************************************/
void bsp_hi6451_volt_show(int volt_id)
{
    int sw;/*开关状态*/
    int voltage;/*电压*/
    pmu_mode_e mode;/*模式*/
    char* onoff;/*开关名称*/
    char* name;/*电压源名称*/
    char* mod_name;

    hi6451_volt_para_check(volt_id);
    sw = bsp_hi6451_volt_is_enabled(volt_id);
    voltage = bsp_hi6451_volt_get_voltage(volt_id);
    mode = bsp_hi6451_volt_get_mode(volt_id);
    #if 0
    if(volt_id <= PMIC_HI6451_BUCK_MAX)
        name = "BUCK"##volt_id;
    else if(PMIC_HI6451_LDO_MAX >= volt_id)
        name = "LDO"##(volt_id - PMIC_HI6451_LDO_MIN + 1);
    #endif

    name = hi6451_volt_name[volt_id];
    if(!sw)
        onoff = "OFF";
    else
        onoff = "ON";

    mod_name = hi6451_mode_name[mode];

    pmic_print_error("id:%d ,name:%s ,sw:%s ,voltage:%d uV,mode:%s  \n",volt_id,name,onoff,voltage,mod_name);

}
/*****************************************************************************
 函 数 名  : bsp_hi6451_volt_state
 功能描述  : 显示当前所有路电源的使用信息(包括开关，电压，)
 输入参数  : void
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  :异常定位
*****************************************************************************/
void bsp_hi6451_volt_state(void)
{
    int i = 0;

    pmic_print_error("***********bsp_hi6451_all_volt_state*****************\n");

    for(i= PMIC_HI6451_VOLT_MIN;i <= PMIC_HI6451_VOLT_MAX;i++)
    {
        bsp_hi6451_volt_show(i);
    }
}
#ifdef __KERNEL__
/*****************************************************************************
 函 数 名  : bsp_hi6451_om_data_show
 功能描述  : 显示此次运行过程中当前PMU状态的异常信息
 输入参数  : void
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  : 异常定位
*****************************************************************************/
void bsp_hi6451_om_data_show(void)
{
    int i,j = 0;

    if(pmu_hi6451_om_data[1] & (0x01 << HI6451_OCP_OFFSET))/*过流*/
    {
        /* 检查所有OCP_RECORD中的状态,安全处理 */
        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 8; j++)
            {
                if (pmu_hi6451_om_data[i + 3] & (0x1 << (7-j)))
                {
                    if(i < 2)
                    {
                        pmic_print_error("pmu_hi6451:Volt %d overflow !\n",(i*8+j+1));
                    }
                    else
                    {
                        pmic_print_error("pmu_hi6451:Volt %d overflow !\n",(i*8+j-2));
                    }
                }
            }
        }
    }
    if(pmu_hi6451_om_data[0] & (0x01 << HI6451_OTP_OFFSET))
        pmic_print_error("pmu_hi6451:temperature overflow (>135/125/115/105℃)!\n");
    if(pmu_hi6451_om_data[0] & (0x01 << HI6451_OVP_OFFSET))
        pmic_print_error("pmu_hi6451:vsys over 6.0v !\n");
    if(pmu_hi6451_om_data[0] & (0x01 << HI6451_UVP_OFFSET))
        pmic_print_error("pmu_hi6451:vsys under 2.5v !\n");
}
/*****************************************************************************
 函 数 名  : bsp_hi6451_boot_om_show
 功能描述  : 显示当前PMU启动时记录的异常信息
 输入参数  : void
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  : 异常定位
*****************************************************************************/
void bsp_hi6451_boot_om_show(void)
{
    /*欠压，过压*/
    if(pmu_hi6451_om_boot[0] & (0x01 << HI6451_VINI_UNDER_2P7_OFFSET ))
        pmic_print_error("pmu_hi6451:vsys under 2.5v last time!\n");
    if(pmu_hi6451_om_boot[0] & (0x01 << HI6451_VINI_OVER_6P0_OFFSET ))
        pmic_print_error("pmu_hi6451:vsys over 6.0v last time!\n");
    /*过流*/
    if(pmu_hi6451_om_boot[1] & (0x01 << HI6451_CORE_OVER_CUR_OFFSET ) )
        pmic_print_error("pmu_hi6451:ldo/buck overflow last time!\n");
    /*过温*/
    if(pmu_hi6451_om_boot[2] & (0x01 << HI6451_OTMP_150_OFFSET ) )
        pmic_print_error("pmu_hi6451:temperature over 150℃ last time!\n");
}
/*****************************************************************************
 函 数 名  : bsp_hi6451_exc_state
 功能描述  : 显示当前PMU的异常信息
 输入参数  : void
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  : 异常定位
*****************************************************************************/
void bsp_hi6451_exc_state(void)
{
    bsp_hi6451_boot_om_show();
    bsp_hi6451_om_data_show();

    pmic_print_error("bsp_hi6451_exc_state over!\n");
}
#endif
/*****************************************************************************
 函 数 名  : bsp_hi6451_version_get
 功能描述  : 显示当前PMIC的版本信息
 输入参数  : void
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  : 异常定位
*****************************************************************************/
void bsp_hi6451_version_get(void)
{
    u8 version = 0;

    version = hi6451_version_get();
    pmic_print_error("PMIC HI6451 version is 0x%x!\n",version);
}

/*****************************************************************************
 函 数 名  : bsp_hi6451_debug_init
 功能描述  : hi6451 debug模块的初始化
 输入参数  : void
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  : 异常定位
*****************************************************************************/
void bsp_hi6451_debug_init(void)
{
    int iret = BSP_PMU_OK;

    struct pmu_dbg_ops hi6451_dbg_ops = {
        .pmu_volt_show = bsp_hi6451_volt_show,
        .pmu_volt_state = bsp_hi6451_volt_state,
        .pmu_pmic_info = bsp_hi6451_version_get,
#ifdef __KERNEL__
        .pmu_om_data_show = bsp_hi6451_om_data_show,
        .pmu_om_data_show = bsp_hi6451_boot_om_show,
        .pmu_exc_state = bsp_hi6451_exc_state,
#endif
    };

    iret = bsp_pmu_dbg_register(PMIC_HI6451,hi6451_dbg_ops);

    if(BSP_PMU_OK != iret)
        pmic_print_error("bsp_pmu_dbg_register error!\n");
    else
        pmic_print_error("bsp_hi6451_debug_init ok!\n");
}
