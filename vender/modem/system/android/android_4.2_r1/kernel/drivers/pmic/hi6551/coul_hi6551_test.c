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
#include <asm/mach/irq.h>
#include <linux/interrupt.h>
#include "bsp_pmu.h"
#include "hi_smartstar.h"
#include "irq_hi6551.h"
#include "coul_hi6551.h"

extern unsigned short coul_voltage(void);
extern unsigned short coul_current(void);
extern unsigned int coul_capacity(void);

/*****************************************************************************
 函 数 名  : coul_battery_voltage
 功能描述  : 当前电压获取，单位:uV/1000
 输入参数  : void
 输出参数  : 无
 返 回 值  : unsigned long long
 调用函数  :
 被调函数  :
*****************************************************************************/
unsigned long long coul_battery_voltage_test(void)
{
    unsigned short  val = 0;
    unsigned long long voltage = 0;

    val = coul_voltage();

    voltage = val * 225120;

    coul_dbg("coul_battery_voltage is %lld\n",voltage);

    return voltage;
}

/*****************************************************************************
 函 数 名  : coul_battery_current
 功能描述  : 当前电流获取,单位:uA/1000
 输入参数  : void
 输出参数  : 无
 返 回 值  : long long
 调用函数  :
 被调函数  :
*****************************************************************************/
signed long long  coul_battery_current_test(void)
{
    unsigned int  val = 0;
    signed long long  bat_current = 0;

    val = coul_current();

    bat_current = val * 468999;

    coul_dbg("coul_battery_current is %lld\n",bat_current);

    return bat_current;
}

/*****************************************************************************
 函 数 名  : coul_battery_capacity
 功能描述  : 电池电量获取,单位:uAh/1000
 输入参数  : void
 输出参数  : 无
 返 回 值  : long long
 调用函数  :
 被调函数  :
*****************************************************************************/
signed long long coul_battery_in_capacity_test(void)
{
    unsigned int  val = 0;
    signed long long capacity = 0;

    val = coul_in_capacity();

    capacity = val * 14330;

    coul_dbg("coul_battery_capacity is %lld\n",capacity);

    return capacity;
}
/*****************************************************************************
 函 数 名  : coul_battery_capacity
 功能描述  : 电池电量获取,单位:uAh/1000
 输入参数  : void
 输出参数  : 无
 返 回 值  : long long
 调用函数  :
 被调函数  :
*****************************************************************************/
signed long long coul_battery_out_capacity_test(void)
{
    unsigned int  val = 0;
    signed long long capacity = 0;

    val = coul_out_capacity();

    capacity = val * 14330;

    coul_dbg("coul_battery_capacity is %lld\n",capacity);

    return capacity;
}

