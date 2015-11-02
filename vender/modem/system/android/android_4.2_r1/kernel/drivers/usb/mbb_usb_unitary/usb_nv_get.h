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
#ifndef __USB_NV_GET_H__
#define __USB_NV_GET_H__

#include "usb_platform_comm.h"
#include "usb_debug.h"
#include "hw_pnp.h"

#define MODE_NUM   16


typedef enum
{
    /*|==================================|*/
#ifdef MBB_USB_UNITARY_Q                /*|                                  |*/
    USB_NV_SERIAL_NUM_ID      = 50049,  /*|高通平台控制SN固定与否            |*/
#else                                   /*|                                  |*/
    USB_NV_SERIAL_NUM_ID      = 26,     /*|balong平台控制SN固定与否          |*/
#endif                                  /*|                                  |*/
    USB_NV_ITEM_AT_SHELL_OPEN = 33,     /*|balong平台 shell 端口控制         |*/
    USB_NV_FACTORY_MODE_I     = 36,     /*|balong平台 产线升级模式           |*/
    USB_NV_SD_WORKMODE        = 51,     /*|balong平台 记录SD卡工作模式       |*/
    USB_NV_PID_UNIFICATION_ID = 50071,  /*|PID可定制项                       |*/
    USB_NV_PORT_INFO_ID       = 50091,  /*|端口可配置                        |*/
    USB_NV_MASS_DYNAMIC_NAME  = 50108,  /*|光盘名称可定制                    |*/
    USB_NV_DYNAMIC_INFO_NAME  = 50109,  /*|设备名称可定制                    |*/
    USB_NV_PROT_OFFSET_ID     = 50110,  /*|VDF定制端口                       |*/
    /*|--重定义了VDF端口协议字段(废弃)   |*/
    USB_NV_FEATURE_WWAN       = 50151,  /*|WWAN功能可定制                    |*/
    USB_NV_WINBLUE_PRF_ID     = 50424,  /*|MBIM功能可定制                    |*/
    USB_NV_NET_SPEED_ID       = 50456,  /*|单板上报给PC的网卡速率            |*/
    /*|--目前规格按最大能力上报          |*/
    USB_NV_USB_PRIVATE_INFO   = 50498   /*|USB log可控制                     |*/
} usb_nv_item;                          /*|==================================|*/


typedef struct COMMON_NV_TYPE
{
    USB_UINT nv_status;
    USB_UINT8 reserved;
} nv_huawei_common_type;

/*NV 26*/
typedef struct tagUSB_NV_SERIAL_NUM_T
{
    USB_UINT usbSnNvStatus;
    USB_UINT usbSnNvSucFlag;
} USB_NV_SERIAL_NUM_T;


/* NV 50049 SN enable */
typedef struct NV_HUAWEI_SN_REPORT_ENABLE_I
{
    USB_UINT    nv_status;
    USB_UINT8   sn_enable;
} nv_huawei_sn_report_enable;

/* 自定义 PID使能 */
typedef struct PID_ENABLE
{
    USB_UINT pid_enabled;
} nv_pid_enable_type;

/* NV 50067 SD enable */
typedef struct NV_HUAWEI_FIRST_REPORT_PORT_I
{
    USB_UINT    nv_status;
    USB_UINT8   sd_enable;
} nv_huawei_first_report_port;

/* 50071 PID */
typedef struct PRODUCT_PID
{
    USB_UINT nv_status;
    USB_UINT16 cust_first_pid;
    USB_UINT16 cust_rewind_pid;
} nv_cust_pid_type;

/* NV 50080 CD enable */
typedef struct NV_HUAWEI_DISABLE_CDROM_I
{
    USB_UINT nv_status;
    USB_UINT8     cd_enable;
} nv_huawei_disable_cdrom;

/* 50091 端口形态 */
typedef struct PACKED_POST
{
    USB_UINT nv_status;
    USB_UINT8 first_port_style[DYNAMIC_PID_MAX_PORT_NUM];
    USB_UINT8 rewind_port_style[DYNAMIC_PID_MAX_PORT_NUM];
    USB_UINT8 reserved[USB_NUM_22];
} nv_huawei_dynamic_pid_type;

/* NV 50108 CD/SD name*/
typedef struct NV_HUAWEI_DYNAMIC_NAME_I
{
    USB_UINT nv_status;
    USB_UINT8 huawei_cdrom_dynamic_name[DYNAMIC_CD_NAME_CHAR_NUM];
    USB_UINT8 huawei_sd_dynamic_name[DYNAMIC_CD_NAME_CHAR_NUM];
    USB_UINT8 reserved[USB_NUM_32];
} nv_huawei_dynamic_name;

/* NV 50109 ports name*/
typedef struct NV_HUAWEI_DYNAMIC_INFO_NAME_I
{
    USB_UINT nv_status;
    USB_UINT8 huawei_product_dynamic_name[DYNAMIC_INFO_NAME_CHAR_NUM];
    USB_UINT8 huawei_manufacturer_dynamic_name[DYNAMIC_INFO_NAME_CHAR_NUM];
    USB_UINT8 huawei_configuration_dynamic_name[DYNAMIC_INFO_NAME_CHAR_NUM];
    USB_UINT8 reserved[USB_NUM_8];
} nv_huawei_dynamic_info_name;
#ifndef MBB_USB_UNITARY_Q
/* 50110 接口协议基数 */
typedef struct PROTOCOL_BASE
{
    USB_UINT nv_status;
    USB_UINT protocol_base;
} nv_protocol_base_type;
#else
typedef struct PROTOCOL_BASE
{
    USB_UINT nv_status;
    USB_UINT8 protocol_base;
    USB_UINT8 reserved[USB_NUM_20];
} nv_protocol_base_type;

#endif
/* 50115 WIN7 wwan特性 */
typedef struct NV_HUAWEI_WWAN_CUSTOMIZATION
{
    USB_UINT  nv_status;
    USB_UINT8 wwan_work_flag;
} nv_feature_wwan;

/*50424*/
typedef struct DRV_WINBLUE_PROFILE_TYPE
{
    USB_UINT8  InterfaceName[32];
    USB_UINT8  MBIMEnable;
    USB_UINT8  CdRom;
    USB_UINT8  TCard;
    USB_UINT8  MaxPDPSession;
    USB_UINT16 IPV4MTU;
    USB_UINT16 IPV6MTU;
    USB_UINT32 Reserved1;
    USB_UINT32 Reserved2;
} nv_winblue_profile_type;

/*50456*/
typedef struct USB_CDC_NET_SPEED_TYPE
{
    USB_UINT32  nv_status;
    USB_UINT32  net_speed;  //单位为bps(例如300Mbps=300*1000*1000 bps),注意单位不是1024
    USB_UINT32  reserve1;
    USB_UINT32  reserve2;
    USB_UINT32  reserve3;
} usb_cdc_net_speed_type;

/*50498*/
typedef struct USB_PRIVATE_NV_INFO
{
    USB_UINT   nv_status;
    USB_UINT8  debug_mode[MBB_LAST];
    USB_UINT32 reserve[10];
} usb_private_nv_info;

/*define struct ,list all correlative usb nv*/
typedef struct mbb_usb_nv_info
{
    nv_cust_pid_type               pid_info ;              //USB_NV_PID_UNIFICATION_ID
    nv_huawei_dynamic_pid_type     dev_profile_info ; //USB_NV_PORT_INFO_ID
#ifdef MBB_USB_UNITARY_Q
    nv_huawei_sn_report_enable     stUSBNvSnSupp ; //USB_NV_SERIAL_NUM_ID
#else
    USB_NV_SERIAL_NUM_T            stUSBNvSnSupp ; //USB_NV_SERIAL_NUM_ID
#endif
    nv_feature_wwan                feature_wwan;  //USB_NV_FEATURE_WWAN
    USB_UINT                       factory_mode ;    //USB_NV_FACTORY_MODE_I
    USB_UINT                       ulSDWorkMode;     //USB_NV_SD_WORKMODE
    USB_UINT                       shell_lock;       //USB_NV_ITEM_AT_SHELL_OPEN
    nv_huawei_dynamic_name         mass_dynamic_name; //USB_NV_MASS_DYNAMIC_NAME
    nv_huawei_dynamic_info_name    port_dynamic_name;   //USB_NV_DYNAMIC_INFO_NAME
    nv_protocol_base_type          stDevProtOfsInfo;  //USB_NV_PROT_OFFSET_ID
    nv_winblue_profile_type        winblue_profile;
    usb_cdc_net_speed_type         net_speed_info;
    usb_private_nv_info            usb_log_ctl;
} mbb_usb_nv_info_st;

/*==============================================================================
 *===============================API声明=================================
 *============================================================================*/
/****************************************************************
 函 数 名  : debug_init
 功能描述  : debug初始化。
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NA
*****************************************************************/
USB_VOID    usb_nv_init(USB_VOID);
/****************************************************************
 函 数 名  : debug_init
 功能描述  : debug初始化。
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NA
*****************************************************************/
/*读取NV文件接口*/
USB_INT     usb_nv_get(USB_VOID);
/****************************************************************
 函 数 名  : debug_init
 功能描述  : debug初始化。
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NA
*****************************************************************/
USB_INT     get_shell_lock(USB_VOID);
/****************************************************************
 函 数 名  : debug_init
 功能描述  : debug初始化。
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NA
*****************************************************************/
/*各模块获取NV值接口*/
mbb_usb_nv_info_st*  usb_nv_get_ctx(USB_VOID);
/****************************************************************
 函 数 名  : debug_init
 功能描述  : debug初始化。
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NA
*****************************************************************/
USB_VOID usb_nv_dump(USB_VOID);

#endif
