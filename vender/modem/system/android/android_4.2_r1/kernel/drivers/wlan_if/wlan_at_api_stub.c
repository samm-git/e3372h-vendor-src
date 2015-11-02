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
#include "wlan_at.h"
#include "wlan_utils.h"


STATIC WLAN_CHIP_OPS stub_ops = 
{
    .WlanATSetWifiEnable = NULL,
    .WlanATGetWifiEnable = NULL,
    .WlanATSetWifiMode   = NULL,
    .WlanATGetWifiMode   = NULL,

    .WlanATSetWifiBand = NULL,
    .WlanATGetWifiBand = NULL,
    .WlanATGetWifiBandSupport = NULL,

    .WlanATSetWifiFreq = NULL,
    .WlanATGetWifiFreq = NULL,

    .WlanATSetWifiDataRate = NULL,
    .WlanATGetWifiDataRate = NULL,

    .WlanATSetWifiPOW = NULL,
    .WlanATGetWifiPOW = NULL,

    .WlanATSetWifiTX = NULL,
    .WlanATGetWifiTX = NULL,

    .WlanATSetWifiRX = NULL,
    .WlanATGetWifiRX = NULL,

    .WlanATSetWifiRPCKG = NULL,
    .WlanATGetWifiRPCKG = NULL,
    .WlanATGetWifiPlatform = NULL,
    .WlanATGetTSELRF = NULL,
    .WlanATSetTSELRF = NULL,
    .WlanATGetTSELRFSupport = NULL,

    .WlanATSetWifiParange = NULL,
    .WlanATGetWifiParange = NULL,

    .WlanATGetWifiParangeSupport = NULL,
    
    .WlanATGetWifiCalTemp = NULL,
    .WlanATSetWifiCalTemp = NULL,
    .WlanATSetWifiCalData = NULL,
    .WlanATGetWifiCalData = NULL,
    .WlanATSetWifiCal = NULL,
    .WlanATGetWifiCal = NULL,
    .WlanATGetWifiCalSupport = NULL,
    .WlanATSetWifiCalFreq = NULL,
    .WlanATGetWifiCalFreq = NULL,
    .WlanATSetWifiCalPOW = NULL,
    .WlanATGetWifiCalPOW = NULL
};

/* ×¢²ábcm43241µ½wlan atÄ£¿é */
void wlan_at_reg_stub(WLAN_CHIP_OPS **wlan_ops)
{
    PLAT_WLAN_INFO("enter");
    wlan_ops[chipstub] = &stub_ops;
    PLAT_WLAN_INFO("exit");
}

