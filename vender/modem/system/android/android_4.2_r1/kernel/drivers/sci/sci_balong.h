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


#ifndef _SCI_BALONG_H_
#define _SCI_BALONG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "soc_interrupts.h"
#include "product_config.h"
#include "bsp_memmap.h"
#include "osl_module.h"
#include "bsp_pmu.h"
#include <product_config.h>


/* add if you need */
#define BALONG_SIM_NUM  2

/* use raise int only */
#define INT_LVL_SIM0_PMU_IN      (PMU_INT_SIM0_IN_RAISE)
#define INT_LVL_SIM0_PMU_OUT     (PMU_INT_SIM0_OUT_FALL)
#define INT_LVL_SIM1_PMU_IN      (PMU_INT_SIM1_IN_RAISE)
#define INT_LVL_SIM1_PMU_OUT     (PMU_INT_SIM1_OUT_FALL)
#if(FEATURE_ON == MBB_WPG_COMMON)
#define SCI_SIMOUT_WAKELOCK_TIMEOUT_IN_MS    10000
#endif
#if(FEATURE_ON == MBB_COMMON)
#define SIM0_GPIO_DETECT         SIM0_DETECT_GPIO
#else
/* sim detect pin */
#define SIM0_GPIO_DETECT         (GPIO_0_5)  /* yangzhi modified 2014-07-17 */
#endif

#define DRIVER_NAME_SIM			"sim_io"
#define DETECT_NAME_SIM 		"sim_detect"

#define PMU_HPD_DEBOUNCE_TIME    (400)


#ifndef OK
#define OK      (0)
#endif

#ifndef ERROR
#define ERROR   (-1)
#endif

#ifdef __KERNEL__
#define SCI_PRINT   printk
#endif


typedef enum
{
    SIM0_CARD_OUT_EVENT = 0,
    SIM0_CARD_IN_EVENT  = 1,
    SIM1_CARD_OUT_EVENT = 0,
    SIM1_CARD_IN_EVENT  = 1
} sci_in_out_enum;


typedef enum
{
    SIM_CARD_STAUTS_OUT = 0,
    SIM_CARD_STAUTS_IN  = 1
} sci_status_enum;


typedef enum
{
    SIM_CARD_DETECT_LOW = 0,
    SIM_CARD_DETECT_HIGH  = 1
} sci_detect_level_enum;


typedef enum
{
    SIM_CARD_PMU_HPD_TRUE = 0,
    SIM_CARD_PMU_HPD_FALSE  = 1
} sci_pmu_hpd_enum;


typedef struct {
	bool sci_init_flag;
	u32  sci0_card_satus;
	u32  sci0_detect_level;
	bool  sci0_pmu_hpd;
	u32  sci1_card_satus;
	u32  sci1_detect_level;
	bool  sci1_pmu_hpd;
} bsp_sci_st;

typedef struct {
	u32 sci0_init_cnt;
	u32 sci0_init_err_cnt;
	u32 sci0_init_request_cnt;
	u32 sci0_detect_cnt;
	u32 sci0_detect_low_cnt;
	u32 sci0_detect_high_cnt;
    u32 sci0_pmu_hpd_in_enter_cnt;
	u32 sci0_pmu_hpd_in_cnt;
    u32 sci0_pmu_hpd_out_enter_cnt;
	u32 sci0_pmu_hpd_out_cnt;
    u32 sci0_icc_high_ok_cnt;
    u32 sci0_icc_low_ok_cnt;
    u32 sci0_detect_low_err_cnt;
	u32 sci0_detect_high_err_cnt;
	u32 sci0_pmu_hpd_in_err_cnt;
	u32 sci0_pmu_hpd_out_err_cnt;
	u32 sci0_detect_get_err_cnt;
    u32 sci0_icc_high_err_cnt;
    u32 sci0_icc_low_err_cnt;
    u32 sci0_detect_err_cnt;
    u32 sci0_hpd_err_cnt;
}sci_debug_info_st;


#ifdef __cplusplus
}
#endif

#endif /* _VIC_BALONG_H_ */

