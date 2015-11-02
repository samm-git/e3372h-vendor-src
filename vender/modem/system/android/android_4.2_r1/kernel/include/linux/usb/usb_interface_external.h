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
******************************************************************************************************/
#ifndef _usb_interface_external_h
#define _usb_interface_external_h

#define USB_OTG_CONNECT_DP_DM    0x0001
#define USB_OTG_DISCONNECT_DP_DM 0x0002  /*直连基带，拉低HS_ID*/
#define USB_OTG_ID_PULL_OUT      0x0003  /*直连基带，拉高HS_ID*/
#define USB_OTG_DISABLE_ID_IRQ   0x0004
#define USB_OTG_ENABLE_ID_IRQ    0x0005
#define USB_WIRELESS_CHGR_DET    0x0006
#define USB_OTG_MAIN_THREAD_EXIT  0x0007
enum chg_current 
{
    CHG_CURRENT_HS =0,    /*usb2.0 for 500mA*/
    CHG_CURRENT_SS ,        /*usb3.0 for 900mA */
    CHG_CURRENT_NO,        /*invalid usb for no current*/
};
int usb_speed_work_mode(void);

#endif
