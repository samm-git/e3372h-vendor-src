/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2013-2015. All rights reserved.
 *
 * mobile@huawei.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
/******************************问题单修改记录**********************************
    日期          修改人              问题单号                   修改内容
******************************************************************************/

#ifndef REB_CONFIG_H
#define REB_CONFIG_H

#define YES  1
#define NO   0

#define Reb_Platform_9x25 (NO)
#define Reb_Platform_V7R2 (YES)
#define MBB_FEATURE_REB_DFT (YES)
/*系统启动过程不对文件系统操作功能*/
#define MBB_REB_WRITE_PROTECT   (YES)
/*假关机失败进入真关机功能*/
#define MBB_REB_FAST_POWEROFF   (YES)
/*Flash病危块扫描功能*/
#define MBB_REB_BADZONE_SCAN  (NO)
/*Flash病危块回写功能*/
#define MBB_REB_BADZONE_REWRITE (NO)
/*分区写越界保护使能*/
#define MBB_REB_ZONE_WRITE_PROTECT (YES)

#define REB_OS_DEBUG
#if ( YES == Reb_Platform_9x25)
#include <linux/huawei_feature.h>
#ifdef  CONFIG_MBB_FAST_ON_OFF
#define MBB_FAST_ON_OFF FEATURE_ON
#endif
#endif
#if ( YES == Reb_Platform_V7R2)
#include <product_config.h>
#endif
#endif
