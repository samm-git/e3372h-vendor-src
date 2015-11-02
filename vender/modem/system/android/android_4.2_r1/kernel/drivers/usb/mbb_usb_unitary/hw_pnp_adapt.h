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
#ifndef __HE_PNP_ADAPT_H__
#define __HE_PNP_ADAPT_H__

#ifdef MBB_USB_UNITARY_Q
#include <soc/qcom/smem.h>
#include <mach/huawei_smem.h>
#include <linux/huawei_feature.h>
#include <linux/qcom/diag_dload.h>

#define USB_SUBCLASS_CODE   USB_SUBCLASS_GAOTONG
#define USB_MBIM_FUNC_NAME  "usb_mbim"

#else
#include "usb_vendor.h"
#include "bsp_sram.h"
#include "usb_platform_comm.h"
#include "bsp_pmu.h"

#define USB_SUBCLASS_CODE   USB_SUBCLASS_BALONG
#define USB_MBIM_FUNC_NAME  "ncm"

#endif

#endif
