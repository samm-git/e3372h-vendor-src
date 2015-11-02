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
#ifndef __POWER_INNER_H__
#define __POWER_INNER_H__

#include <linux/rtc.h>
#include <drv_chg.h>
#include <drv_onoff.h>
#include <power_com.h>
#include <bsp_softtimer.h>

#if (MBB_CHG_PLATFORM_V7R2 == FEATURE_ON)
#include "drv_chg.h"
#endif

/*******************************************************************************
* 开机结构体、枚举定义
********************************************************************************/
typedef enum power_on_property {
	POWER_ON_PROP_STATUS = 0,
	POWER_ON_PROP_REASON,
	POWER_ON_PROP_TIME
}power_sysinfo;

typedef struct
{
    DRV_START_MODE           mode;
    DRV_START_REASON         reason;
    struct rtc_time         time;
    struct softtimer_list   on_timer;
}power_on_ctrl_s;

/********************************************************************************
* 开机函数声明
*********************************************************************************/

/* stub */
extern BATT_LEVEL_ENUM chg_get_batt_level(void);

#if (MBB_CHG_PLATFORM_V7R2 == FEATURE_OFF)
extern int chg_get_batt_level(void);
#endif
/********************************************************************************
* 关机结构体、枚举定义
*********************************************************************************/

typedef struct
{
    DRV_SHUTDOWN_REASON_ENUM    reason;
    struct rtc_time            time;
	struct softtimer_list      off_timer;
}power_off_ctrl_s;

/* 开关机宏定义 */

#if (MBB_CHG_PLATFORM_V7R2 == FEATURE_ON)
#define POWER_OFF_MONOTER_TIMEROUT      (25 * 1000)/*超时关机时间*/
#else
#define POWER_OFF_MONOTER_TIMEROUT      (5 * 1000)
#endif

#define POWER_ON_MONITOR_TIMEROUT       (60*1000)

#define pr_dbg printk

#define EXCH_RESET_LOG_PATH     "/modem_log/poweroff.txt"
#define EXCH_POWER_LOG_PATH     "/modem_log/poweron.txt"
#define EXCH_ONOFF_LOG_MAX      (0x400*16)

/********************************************************************************
* 关机函数声明
*********************************************************************************/

#endif

