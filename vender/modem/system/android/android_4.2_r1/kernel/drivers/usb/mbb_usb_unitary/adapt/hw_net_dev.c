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
/**************************************************************问题单修改记录***********************************************************************
日    期           修改人           问题单号               修改内容

*****************************************************************************************************************************************************/


#include "hw_net_dev.h"
#include <product_config.h>

/**************************************************
  Function：     net_get_connect_rate
  Description：  获取nv项中配置的速率
  Called by：    ncm_app_enable
  Input：        NA
  Output：       NA
  Return：       获取的速率值
  Others：       NA
**************************************************/

bool_t  s_EthLinkStatus = FALSE;

uint32_t net_get_connect_rate(uint32_t ratetype)
{
    /*移植代码codeck for 警告注*/
    uint32_t ratetbl[] = {DEFAULT_RATE, 53600, 236800, 384000, 1800000, 3600000, 
                    7200000,10200000, 14400000, 21600000, 28800000, 43200000};
    uint32_t dfttbl[RATE_TYPE_BUTT] = {14400, 53600, 236800, 384000, DEFAULT_RATE};
    nv_huawei_connect_display_rate_type  rateitem;
    char* prate;
    uint32_t tablelen = sizeof(ratetbl) / sizeof(ratetbl[0]);
    uint32_t value;

    /*获取NV项*/
    if (0 != usb_read_nv(en_NV_Item_Default_Rate, &rateitem, sizeof(nv_huawei_connect_display_rate_type)))
    {
        return DEFAULT_RATE;
    }

    prate = (char*) &rateitem;

    /*检查入参*/
    if ( ratetype < RATE_TYPE_BUTT)
    {
        value = *(prate + ratetype);
    }
    else
    {
        return DEFAULT_RATE;
    }

    /*value 等于0是返回默认速率*/
    if ((0 == value) && (ratetype < RATE_TYPE_BUTT))
    {
        return dfttbl[ratetype];
    }

    if (value > 0 && value < tablelen)
    {
        return ratetbl[value];
    }
    else
    {
        return DEFAULT_RATE;
    }
}


void    USB_ETH_LinkStatSet( bool_t status )
{
    s_EthLinkStatus = status;
    pr_err("U_NET: set link status = %d \n", s_EthLinkStatus);
}
bool_t USB_ETH_LinkStatGet()
{
    pr_err("U_NET: get link status = %d \n", s_EthLinkStatus);
    return s_EthLinkStatus;
}
