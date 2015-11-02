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
    问题单号            时间            修改人                        说明
******************************************************************************************************/
#include <linux/string.h>
#include "usb_nv_get.h"
#include "usb_debug.h"
#include "f_mbb_storage.h"
#include "hw_pnp_api.h"

static mbb_usb_nv_info_st g_usb_nv_info;/*全局变量禁止直接使用，请使用usb_nv_get_ctx()*/
static nv_protocol_base_type gstDevProtOfsDefault = { 0x00, 0x00 };
#ifdef USB_RNDIS
static nv_cust_pid_type gstDevPidDefault = { 0x00, CDROM_SCENE_PID, PID_USER };
#else
static nv_cust_pid_type gstDevPidDefault = { 0x00, PRODUCT_ID, PID_USER };
#endif

#ifdef USB_RNDIS
static uint8_t gRewindPortDefault[DYNAMIC_PID_MAX_PORT_NUM] =
{
    USB_IF_PROTOCOL_RNDIS,
#ifdef USB_SD
    USB_IF_PROTOCOL_SDRAM,
#endif
};
#else
static uint8_t gRewindPortDefault[DYNAMIC_PID_MAX_PORT_NUM] =
{
    USB_IF_PROTOCOL_SDRAM,        //SD
    USB_IF_PROTOCOL_PCUI,         //PCUI
    USB_IF_PROTOCOL_3G_DIAG,      //3G_DIAG
    USB_IF_PROTOCOL_NCM,          //NDIS
    USB_IF_PROTOCOL_DIAG,         //DIAG
    USB_IF_PROTOCOL_GPS,          //ASHELL
    USB_IF_PROTOCOL_BLUETOOTH,    //CSHELL
    USB_IF_PROTOCOL_3G_GPS,       //3G_GPS
};
#endif

static USB_UINT8 gUsbModeLogLever[MBB_LAST] =
{
    U_ERROR,
};

/*****************************************************************
Parameters    :  无
Return        :    无
Description   :  获取 shell_lock 值
*****************************************************************/
USB_INT get_shell_lock(USB_VOID)
{
    return g_usb_nv_info.shell_lock;
}

/*****************************************************************
Parameters    :  无
Return        :    无
Description   :  初始化USB 各个模块的log级别
*****************************************************************/
USB_VOID usb_mode_log_lever_init(USB_VOID)
{
    USB_INT i = 0;
    for ( ; i < MBB_LAST ; i++)
    {
        gUsbModeLogLever[i] = U_ERROR;
    }
}


/*****************************************************************
Parameters    :  无
Return        :    0  MBB_USB_OK
Description   :  读取   所有 USB NV 项
*****************************************************************/
USB_INT  usb_nv_get(USB_VOID)
{
    USB_INT ret = MBB_USB_ERROR;

    DBG_I(MBB_USB_NV , "begin!\n");
#ifdef MBB_USB_UNITARY_Q
    memset(&(g_usb_nv_info.stUSBNvSnSupp),0,sizeof(g_usb_nv_info.stUSBNvSnSupp));
    ret = usb_read_nv(USB_NV_SERIAL_NUM_ID,(USB_PVOID) &(g_usb_nv_info.stUSBNvSnSupp), 
     sizeof(g_usb_nv_info.stUSBNvSnSupp));
    if(MBB_USB_OK != ret || MBB_USB_FALSE == g_usb_nv_info.stUSBNvSnSupp.nv_status )
    {
        DBG_T(MBB_USB_NV ,"NV = %d, ret=%d nv_status=%d \r\n", USB_NV_SERIAL_NUM_ID,ret,
            g_usb_nv_info.stUSBNvSnSupp.nv_status);
        g_usb_nv_info.stUSBNvSnSupp.sn_enable = MBB_USB_FALSE;    
    }
#else
    /*USB_NV_SERIAL_NUM_ID  NV 26*/
    memset(&(g_usb_nv_info.stUSBNvSnSupp), 0, sizeof(g_usb_nv_info.stUSBNvSnSupp));
    ret = usb_read_nv(USB_NV_SERIAL_NUM_ID, (USB_PVOID) & (g_usb_nv_info.stUSBNvSnSupp),
                      sizeof(g_usb_nv_info.stUSBNvSnSupp));
    if (MBB_USB_OK != ret || MBB_USB_FALSE == g_usb_nv_info.stUSBNvSnSupp.usbSnNvStatus )
    {
        DBG_T(MBB_USB_NV , "NV = %d, ret=%d nv_status=%d \r\n", USB_NV_SERIAL_NUM_ID, ret,
              g_usb_nv_info.stUSBNvSnSupp.usbSnNvStatus);
        g_usb_nv_info.stUSBNvSnSupp.usbSnNvStatus = MBB_USB_FALSE;
        g_usb_nv_info.stUSBNvSnSupp.usbSnNvSucFlag = 0;
    }
    /*USB_NV_ITEM_AT_SHELL_OPEN NV 33*/
    ret = usb_read_nv(USB_NV_ITEM_AT_SHELL_OPEN, (USB_PVOID) & (g_usb_nv_info.shell_lock),
                      sizeof(USB_UINT32));
    if (MBB_USB_OK != ret)
    {
        DBG_T(MBB_USB_NV, "NV %d usb default!\r\n", USB_NV_ITEM_AT_SHELL_OPEN);
        g_usb_nv_info.shell_lock = 0;
    }

    /*USB_NV_FACTORY_MODE_I  NV 36*/
    ret = usb_read_nv(USB_NV_FACTORY_MODE_I, (USB_PVOID) & (g_usb_nv_info.factory_mode),
                      sizeof(USB_UINT32));
    if (MBB_USB_OK != ret)
    {
        DBG_T(MBB_USB_NV , "NV %d usb default!\r\n", USB_NV_FACTORY_MODE_I);
        g_usb_nv_info.factory_mode = 1;
    }

    /*USB_NV_SD_WORKMODE NV 51*/
    ret = usb_read_nv(USB_NV_SD_WORKMODE, &(g_usb_nv_info.ulSDWorkMode), sizeof(USB_UINT32));
    if (MBB_USB_OK != ret)
    {
        DBG_E(MBB_USB_NV , "NV %d usb default!\r\n", USB_NV_SD_WORKMODE);
        g_usb_nv_info.ulSDWorkMode = SD_WORKMODE_MASS;
    }
#endif
    /*USB_NV_PID_UNIFICATION_ID NV50071*/
    memset(&(g_usb_nv_info.pid_info), 0, sizeof(nv_cust_pid_type));
    ret = usb_read_nv(USB_NV_PID_UNIFICATION_ID, (USB_PVOID) & (g_usb_nv_info.pid_info),
                      sizeof(nv_cust_pid_type));
    if (MBB_USB_OK != ret || MBB_USB_FALSE == g_usb_nv_info.pid_info.nv_status)
    {
        DBG_T(MBB_USB_NV , "NV = %d, ret=%d nv_status=%d\r\n", USB_NV_PID_UNIFICATION_ID, ret,
              g_usb_nv_info.pid_info.nv_status);
        memcpy((USB_PVOID)(&(g_usb_nv_info.pid_info)), (USB_PVOID)(&gstDevPidDefault), sizeof(nv_cust_pid_type));
    }

    /*USB_NV_PORT_INFO_ID NV50091*/
    memset(&(g_usb_nv_info.dev_profile_info), 0, sizeof(nv_huawei_dynamic_pid_type));
    ret = usb_read_nv(USB_NV_PORT_INFO_ID, (USB_PVOID) & (g_usb_nv_info.dev_profile_info),
                      sizeof(nv_huawei_dynamic_pid_type));
    if (MBB_USB_OK != ret || MBB_USB_FALSE == g_usb_nv_info.dev_profile_info.nv_status)
    {
        DBG_T(MBB_USB_NV , "NV = %d, ret=%d nv_status=%d\r\n", USB_NV_PORT_INFO_ID, ret,
              g_usb_nv_info.dev_profile_info.nv_status);
        g_usb_nv_info.dev_profile_info.first_port_style[0] = USB_IF_PROTOCOL_CDROM;
        memcpy(g_usb_nv_info.dev_profile_info.rewind_port_style, gRewindPortDefault,
               DYNAMIC_PID_MAX_PORT_NUM);
    }

    /*USB_NV_MASS_DYNAMIC_NAME NV50108*/
    memset(&(g_usb_nv_info.mass_dynamic_name), 0, sizeof(nv_huawei_dynamic_name));
    ret = usb_read_nv(USB_NV_MASS_DYNAMIC_NAME, (USB_PVOID) & (g_usb_nv_info.mass_dynamic_name),
                      sizeof(nv_huawei_dynamic_name));
    if (MBB_USB_OK != ret || MBB_USB_FALSE == g_usb_nv_info.mass_dynamic_name.nv_status)
    {
        DBG_T(MBB_USB_NV , "NV = %d, ret=%d nv_status=%d\r\n", USB_NV_MASS_DYNAMIC_NAME, ret,
              g_usb_nv_info.mass_dynamic_name.nv_status);
        snprintf(g_usb_nv_info.mass_dynamic_name.huawei_cdrom_dynamic_name, USB_NUM_29,
                 "%-8s%-16s%-4s", "HUAWEI", "Mass Storage", "2.31");
        snprintf(g_usb_nv_info.mass_dynamic_name.huawei_sd_dynamic_name, USB_NUM_29,
                 "%-8s%-16s%-4s", "HUAWEI", "TF CARD Storage", "2.31");
    }
    /*USB_NV_DYNAMIC_INFO_NAME NV50109*/
    memset(&(g_usb_nv_info.port_dynamic_name), 0, sizeof(nv_huawei_dynamic_info_name));
    ret = usb_read_nv(USB_NV_DYNAMIC_INFO_NAME, (USB_PVOID) & (g_usb_nv_info.port_dynamic_name),
                      sizeof(nv_huawei_dynamic_info_name));
    if (MBB_USB_OK != ret || MBB_USB_FALSE == g_usb_nv_info.port_dynamic_name.nv_status)
    {
        DBG_T(MBB_USB_NV , "NV = %d, ret=%d nv_status=%d\r\n", USB_NV_DYNAMIC_INFO_NAME, ret,
              g_usb_nv_info.port_dynamic_name.nv_status);
        strncpy(g_usb_nv_info.port_dynamic_name.huawei_manufacturer_dynamic_name, "HUAWEI_MOBILE",
                (DYNAMIC_INFO_NAME_CHAR_NUM - 1));
        strncpy(g_usb_nv_info.port_dynamic_name.huawei_product_dynamic_name, "HUAWEI_MOBILE",
                (DYNAMIC_INFO_NAME_CHAR_NUM - 1));
    }
    /*USB_NV_PROT_OFFSET_ID  NV50110*/
    memset( &(g_usb_nv_info.stDevProtOfsInfo), 0, sizeof(nv_protocol_base_type));
    ret = usb_read_nv(USB_NV_PROT_OFFSET_ID, (USB_PVOID) & (g_usb_nv_info.stDevProtOfsInfo),
                      sizeof(nv_protocol_base_type));
    if (MBB_USB_OK != ret || MBB_USB_FALSE == g_usb_nv_info.stDevProtOfsInfo.nv_status)
    {
        DBG_T(MBB_USB_NV , "NV = %d, ret=%d nv_status=%d\r\n", USB_NV_PROT_OFFSET_ID, ret,
              g_usb_nv_info.stDevProtOfsInfo.nv_status);
        memcpy((USB_PVOID) & (g_usb_nv_info.stDevProtOfsInfo), (USB_PVOID) & (gstDevProtOfsDefault), sizeof(nv_protocol_base_type));
    }
    else if (g_usb_nv_info.stDevProtOfsInfo.protocol_base != 0)
    {
        DBG_T(MBB_USB_NV , "NV_%d invalid !protocol_base = %d!!\r\n", USB_NV_PROT_OFFSET_ID,
              g_usb_nv_info.stDevProtOfsInfo.protocol_base);
    }

    /*USB_NV_FEATURE_WWAN NV50115*/
    ret = usb_read_nv(USB_NV_FEATURE_WWAN, &(g_usb_nv_info.feature_wwan),
                      sizeof(g_usb_nv_info.feature_wwan));
    if (MBB_USB_OK != ret)
    {
        DBG_T(MBB_USB_NV , "NV = %d, ret=%d nv_status=%d\r\n",
              USB_NV_USB_PRIVATE_INFO, ret, g_usb_nv_info.feature_wwan.nv_status);
#if defined(USB_E5) || defined(USB_RNDIS)
        g_usb_nv_info.feature_wwan.wwan_work_flag = MBB_USB_FALSE;
#else
        g_usb_nv_info.feature_wwan.wwan_work_flag = MBB_USB_TRUE;
#endif
    }

    /*NV_WINBLUE_PRF NV50424*/
    memset(&(g_usb_nv_info.winblue_profile), 0, sizeof(nv_winblue_profile_type));
    ret = usb_read_nv(USB_NV_WINBLUE_PRF_ID,
                      &(g_usb_nv_info.winblue_profile), sizeof(nv_winblue_profile_type));
    if (MBB_USB_OK != ret)
    {
        DBG_T(MBB_USB_NV , "NV = %d, ret=%d \r\n",
              USB_NV_WINBLUE_PRF_ID, ret);
        g_usb_nv_info.winblue_profile.MBIMEnable = 0;
    }
    else
    {
        DBG_T(MBB_USB_NV, "Winblue: profile nv val: %s, %u, %u, %u, %u, %u, %u\n",
              g_usb_nv_info.winblue_profile.InterfaceName,
              g_usb_nv_info.winblue_profile.MBIMEnable,
              g_usb_nv_info.winblue_profile.CdRom,
              g_usb_nv_info.winblue_profile.TCard,
              g_usb_nv_info.winblue_profile.MaxPDPSession,
              g_usb_nv_info.winblue_profile.IPV4MTU,
              g_usb_nv_info.winblue_profile.IPV6MTU);
    }

    /*net speed NV 50456*/
    ret = usb_read_nv(USB_NV_NET_SPEED_ID,
                      &(g_usb_nv_info.net_speed_info), sizeof(usb_cdc_net_speed_type));
    if (MBB_USB_OK != ret || MBB_USB_FALSE == g_usb_nv_info.net_speed_info.nv_status)
    {
        DBG_T(MBB_USB_NV, "NV = %d, ret=%d nv_status=%ld\r\n", USB_NV_NET_SPEED_ID, ret,
              g_usb_nv_info.net_speed_info.nv_status);
        g_usb_nv_info.net_speed_info.net_speed = 0;
    }

    /*USB_NV_USB_PRIVATE_INFO NV50498*/
    ret = usb_read_nv(USB_NV_USB_PRIVATE_INFO, &(g_usb_nv_info.usb_log_ctl), sizeof(usb_private_nv_info));
    if (MBB_USB_OK != ret || MBB_USB_FALSE == g_usb_nv_info.usb_log_ctl.nv_status)
    {
        DBG_T(MBB_USB_NV , "NV = %d, ret=%d nv_status=%d\r\n",
              USB_NV_USB_PRIVATE_INFO, ret, g_usb_nv_info.usb_log_ctl.nv_status);
        memcpy(&g_usb_nv_info.usb_log_ctl.debug_mode, gUsbModeLogLever, sizeof(gUsbModeLogLever));
    }

    return MBB_USB_OK;
}


/*****************************************************************
Parameters    :  usb_nv 结构体指针
Return        :    无
Description   :  对外输出 USB NV  相关内容
*****************************************************************/

mbb_usb_nv_info_st*  usb_nv_get_ctx(USB_VOID)
{
    return &g_usb_nv_info;
}

/*****************************************************************
Parameters    :  无
Return        :    无
Description   :  USB NV 初始化为  NV  读取失败的默认值
*****************************************************************/
USB_VOID usb_nv_init(USB_VOID)
{

    /* NV 50091对应内容的 默认值*/
    g_usb_nv_info.dev_profile_info.nv_status = MBB_USB_TRUE;
    g_usb_nv_info.dev_profile_info.first_port_style[0] = USB_IF_PROTOCOL_CDROM;
    g_usb_nv_info.dev_profile_info.first_port_style[1] = USB_IF_PROTOCOL_SDRAM;
    memcpy(g_usb_nv_info.dev_profile_info.rewind_port_style, gRewindPortDefault,
           DYNAMIC_PID_MAX_PORT_NUM);
#ifdef MBB_USB_UNITARY_Q
    g_usb_nv_info.stUSBNvSnSupp.nv_status = MBB_USB_FALSE;
    g_usb_nv_info.stUSBNvSnSupp.sn_enable = MBB_USB_FALSE;
#else
    /* NV 26对应内容的 默认值*/
    g_usb_nv_info.stUSBNvSnSupp.usbSnNvStatus = 0;
    g_usb_nv_info.stUSBNvSnSupp.usbSnNvSucFlag = 0;
#endif
    g_usb_nv_info.factory_mode = -1;
    memcpy((USB_PVOID) & (g_usb_nv_info.stDevProtOfsInfo), (USB_PVOID) & (gstDevProtOfsDefault), sizeof(nv_protocol_base_type));
    g_usb_nv_info.mass_dynamic_name.nv_status = MBB_USB_TRUE;
    g_usb_nv_info.shell_lock = 0;
    g_usb_nv_info.ulSDWorkMode = SD_WORKMODE_MASS;
    usb_mode_log_lever_init();

    usb_nv_get();

}

/*USB_NV 可维可测函数*/
USB_VOID usb_nv_dump(USB_VOID)
{
    USB_INT i = 0;
    DBG_T(MBB_DEBUG, "|-------------------------------------------------------\n");
#ifdef MBB_USB_UNITARY_Q
    DBG_T(MBB_DEBUG,"|NV_50049: \n");
    DBG_T(MBB_DEBUG,"|nv_status = %d\n" ,g_usb_nv_info.stUSBNvSnSupp.nv_status);
    DBG_T(MBB_DEBUG,"|u32USBSerialNumSupp = %d\n",
        g_usb_nv_info.stUSBNvSnSupp.sn_enable);
#else
    /*26*/
    DBG_T(MBB_DEBUG, "|NV_26: \n");
    DBG_T(MBB_DEBUG , "|nv_status = %d\n" , g_usb_nv_info.stUSBNvSnSupp.usbSnNvStatus);
    DBG_T(MBB_DEBUG, "|u32USBSerialNumSupp = %d\n",
          g_usb_nv_info.stUSBNvSnSupp.usbSnNvSucFlag);

    DBG_T(MBB_DEBUG, "|-------------------------------------------------------\n");
    /*33*/
    DBG_T(MBB_DEBUG, "|NV_33:\n");
    DBG_T(MBB_DEBUG, "|shell_lock = %d \n", g_usb_nv_info.shell_lock );
    DBG_T(MBB_DEBUG, "|-------------------------------------------------------\n");
    /*36*/
    DBG_T(MBB_DEBUG, "|NV_36:\n");
    DBG_T(MBB_DEBUG, "|factory_mode = %d\n", g_usb_nv_info.factory_mode);

    DBG_T(MBB_DEBUG, "|-------------------------------------------------------\n");
    /*51*/
    DBG_T(MBB_DEBUG, "|NV_51:\n");
    DBG_T(MBB_DEBUG, "|ulSDWorkMode =%d \n", g_usb_nv_info.ulSDWorkMode);
    DBG_T(MBB_DEBUG, "|-------------------------------------------------------\n");
#endif
    /*50071*/
    DBG_T(MBB_DEBUG, "|NV_50071:\n");
    DBG_T(MBB_DEBUG, "|nv_status = %d \n", g_usb_nv_info.pid_info.nv_status);
    DBG_T(MBB_DEBUG, "|cust_before_pid = 0x%X \n", g_usb_nv_info.pid_info.cust_first_pid );
    DBG_T(MBB_DEBUG, "|cust_after_pid  = 0x%X \n", g_usb_nv_info.pid_info.cust_rewind_pid );
    DBG_T(MBB_DEBUG, "|-------------------------------------------------------\n");
    /*50091*/
    DBG_T(MBB_DEBUG, "|NV_50091:\n");
    for (i = 0; i < DYNAMIC_PID_MAX_PORT_NUM; i++)
    {
        if (g_usb_nv_info.dev_profile_info.first_port_style[i])
        {
            DBG_T(MBB_DEBUG, "| first_port_style[%2d] =0x%X\n",
                  i, g_usb_nv_info.dev_profile_info.first_port_style[i]);
        }
        else
        {
            break;
        }
    }
    for (i = 0; i < DYNAMIC_PID_MAX_PORT_NUM; i++)
    {
        if (g_usb_nv_info.dev_profile_info.rewind_port_style[i])
        {
            DBG_T(MBB_DEBUG , "|rewind_port_style[%2d] =0x%X\n",
                  i, g_usb_nv_info.dev_profile_info.rewind_port_style[i]);
        }
        else
        {
            break;
        }

    }
    DBG_T(MBB_DEBUG, "|-------------------------------------------------------\n");
    /*50108*/
    DBG_T(MBB_DEBUG, "|NV_50108: \n");
    DBG_T(MBB_DEBUG, "|nv_status = %d\n", g_usb_nv_info.mass_dynamic_name.nv_status);
    DBG_T(MBB_DEBUG, "|huawei_cdrom_dynamic_name: %28.28s \n"
          , g_usb_nv_info.mass_dynamic_name.huawei_cdrom_dynamic_name);
    DBG_T(MBB_DEBUG, "|huawei_sd_dynamic_name: %s \n"
          , g_usb_nv_info.mass_dynamic_name.huawei_sd_dynamic_name);
    DBG_T(MBB_DEBUG, "|-------------------------------------------------------\n");
    /*50109*/
    DBG_T(MBB_DEBUG, "|NV_50109:\n");
    DBG_T(MBB_DEBUG, "|nv_status = %d \n"
          , g_usb_nv_info.port_dynamic_name.nv_status);
    DBG_T(MBB_DEBUG, "|huawei_product_dynamic_name: %s\n"
          , g_usb_nv_info.port_dynamic_name.huawei_product_dynamic_name);
    DBG_T(MBB_DEBUG, "|huawei_manufacturer_dynamic_name: %s"
          , g_usb_nv_info.port_dynamic_name.huawei_manufacturer_dynamic_name);
    DBG_T(MBB_DEBUG, "|huawei_configuration_dynamic_name: %s \n"
          , g_usb_nv_info.port_dynamic_name.huawei_configuration_dynamic_name);
    DBG_T(MBB_DEBUG, "|-------------------------------------------------------\n");
    /*50110*/
    DBG_T(MBB_DEBUG, "|NV_50110: \n");
    DBG_T(MBB_DEBUG, "|nv_status = %d\n"
          , g_usb_nv_info.stDevProtOfsInfo.nv_status);
    DBG_T(MBB_DEBUG , "|protocol_base = 0x%X\n",
          g_usb_nv_info.stDevProtOfsInfo.protocol_base);
    DBG_T(MBB_DEBUG, "|-------------------------------------------------------\n");
    /*50151*/
    DBG_T(MBB_DEBUG, "|NV_50151: \n");
    DBG_T(MBB_DEBUG, "|nv_status = %d\n"
          , g_usb_nv_info.feature_wwan.nv_status);
    DBG_T(MBB_DEBUG , "|wwan_work_flag = %u\n",
          g_usb_nv_info.feature_wwan.wwan_work_flag);
    DBG_T(MBB_DEBUG, "|-------------------------------------------------------\n");
    /*50456*/
    DBG_T(MBB_DEBUG, "|NV_50456: \n");
    DBG_T(MBB_DEBUG, "|nv_status = %d\n"
          , (int)(g_usb_nv_info.net_speed_info.nv_status));
    DBG_T(MBB_DEBUG, "|net_speed = %d\n"
          , (int)(g_usb_nv_info.net_speed_info.net_speed));
    /*50498*/
    DBG_T(MBB_DEBUG, "|NV_50498: \n");
    DBG_T(MBB_DEBUG, "|nv_status = %d\n"
          , g_usb_nv_info.usb_log_ctl.nv_status);
    for ( i = 0; i < MBB_LAST; i++ )
    {
        DBG_T(MBB_DEBUG , "|debug_mode[%2d] =%u\n",
              i, g_usb_nv_info.usb_log_ctl.debug_mode[i]);
    }
    DBG_T(MBB_DEBUG, "|-------------------------------------------------------\n");
}
