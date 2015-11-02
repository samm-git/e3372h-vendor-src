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
/******************************************************************************************************
    问题单号                       时间                   修改人                        说明
*****************************************************************************************************/
#ifndef __USB_CONFIG_H__
#define __USB_CONFIG_H__

#include <product_config.h>
/*===================平台适配宏 BEGIN==============*/

//#define MBB_USB_UNITARY_Q     /*适配高通平台的宏*/
#define MBB_USB_UNITARY_B     /*适配巴龙平台的宏*/
/*==================平台适配宏 END==============*/

/*===================功能宏BEGIN==============*/

// #define MBB_USB_DEBUG

#if (FEATURE_ON == MBB_USB_MULTICONFIG)
#define USB_MULTICONFIG
#endif

#if (FEATURE_ON == MBB_USB_SECURITY)
#define USB_SECURITY
#endif

#if (FEATURE_ON == MBB_USB_SD)
#define USB_SD
#endif

#if (FEATURE_ON == MBB_USB_CPE)
#define USB_CPE
#endif

#if (FEATURE_ON == MBB_USB_E5)
#define USB_E5
#endif

#if (FEATURE_ON == MBB_USB_HILINK)
#define USB_HILINK
#endif

#if (FEATURE_ON == MBB_USB_STICK)
#define USB_STICK
#endif

#if (FEATURE_ON == MBB_USB_WINGLE)
#define USB_WINGLE
#endif

#if (FEATURE_ON == MBB_USB_CAPABILITY_THREE)
#define USB_SUPER_SPEED
#endif

#if (FEATURE_ON == MBB_CHARGE)
#define USB_CHARGE

#if (FEATURE_ON == MBB_CHG_EXTCHG)
#define USB_CHARGE_EXT
#endif

#endif

#if (FEATURE_ON == MBB_FAST_ON_OFF)
#define USB_FAST_ON_OFF
#endif

#ifdef CONFIG_BALONG_RNDIS
#define USB_RNDIS
#endif

/* 双芯片CPE产品,会使用到的多网口及多串口模式 */
/* #define USB_DUAL_CORE_CPE */

/* 后续待处理
#define MBB_CHG_EXTCHG
#define CONFIG_USB_OTG
#define CONFIG_GADGET_SUPPORT_REMOTE_WAKEUP
#define CONFIG_NCM_MBIM_SUPPORT
#define MBB_CHG_WIRELESS
#define MBB_DLOAD
#define NCM_ENCAP_CMD_CID_ENABLED
#define CONFIG_USB_GADGET_SUPERSPEED
#define GNET_SINGLE_INTERFACE
#define GNET_CLASS_TYPE_HUAWEI
#define CONFIG_BUS_PCI
#define CONFIG_GNET_PREALLOC_RX_MEM
#define CONFIG_GNET_PREALLOC_TX_MEM
*/

/*===================功能宏 END==============*/
#endif
