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
    问题单号           时间            修改人                      说明
******************************************************************************************************/

#ifndef __HW_PNP_H__
#define __HW_PNP_H__

#include "usb_debug.h"

/*-------------------------------------------------------------------------*/

/* Default vendor and product IDs*/
#define VENDOR_ID    0x12D1    /* NetChip */

#define USB_CONFIG_PATH "/data/usb_config"
#define USB_CONFIG_HILINK_MODEM_PATH "/data/usb_config/hilink_modem"

#define USB_DRIVER_NAME		"android_usb"
#define DIAL_MODE_MODEM         0
#define DIAL_MODE_NDIS          1
#define DIAL_MODE_MODEM_N_NDIS  2
#define DIAL_MODE_NONE          3

#define CDC_SPEC_NONE           0
#define CDC_SPEC_MODEM          1
#define CDC_SPEC_NDIS           2
#define CDC_SPEC_MODEM_N_NDIS   3

/* string IDs are assigned dynamically */
#define STRING_MANUFACTURER_IDX 0
#define STRING_PRODUCT_IDX      1
#define STRING_SERIAL_IDX       2
#define STRING_MSOS_IDX         3

#define ANDROID_DEVICE_NODE_NAME_LENGTH 11

#define POWER_OFF_CHARGER    0x59455353  /*关机魔术字*/

#define USB_LOW_MASK    0x00FF

#define USB_HIGH_MASK   0xFF00

#define MULTI_CONFIG_ONE      1
#define MULTI_CONFIG_TWO      2
#define MULTI_CONFIG_THR      3

#define USB_CONFIG_MBIM       MULTI_CONFIG_TWO

#define CTL_APP_START      0
#define CTL_APP_DISABLE    1
#define CTL_APP_WAIT       9

#define CTL_APP_START_FALG      '0'
#define CTL_APP_DISABLE_FALG    '1'
#define CTL_APP_WAIT_FALG       '9'

#define GATEWAT_MODEM_MODE    0
#define GATEWAY_NDIS_MODE     1

#define APP_MODE_FLAG_LEN     2
#define APP_WRITE_DATA_SITE   0

#define MAX_SHOW_LEN          4096
#define MAX_STRING_DESC_NUM   256

#define DYNAMIC_REWIND_BEFORE_dynamic_port
//#define CONFIG_BALONG_RNDIS
//#define CONFIG_USB_GADGET_DUALSPEED

#define TCM_FOR_ISO_LOAD_FLAG_VAL          0x87654321

#define HUAWEI_CONFIG_DYNAMIC_PID_CONTINUE 0
#define HUAWEI_CONFIG_DYNAMIC_PID_COMPLETE 1

#define DYNAMIC_PID_MAX_PORT_NUM     17
#define MAX_SUPPORT_PORT_NAME_STRING 255
#define MAX_TOTAL_PORT_NUM           255

#define CONFIG_TWO_MAX_PORT_NUM      4 //JUST FOR WIN8,do not need driver
#define USB_NV_NEW_PID_INFO_ID       31
#define DYNAMIC_CD_NAME_CHAR_NUM     28
#define DYNAMIC_INFO_NAME_CHAR_NUM   40

#define MAX_REWIND_PARAM_NUM    15

#define MANUFACTURER_BUFFER_SIZE 256
#define MAX_SPEED_NAME_LEN       16

#define WINDOWS_OS_FLAG    0x00
#define MAC_OS_FLAG        0x10
#define LINUX_OS_FLAG      0x20
#define GATEWAY_OS_FLAG    0x30

#define SC_REWIND_11       0x11
#define SC_REWIND_11_LEN   31

#define VENDOR_ID_HUAWEI      0x12D1
#define VENDOR_PRO            0xFF
#define PNP_PRODUCT_ID        0x14fe  /*pnp pid*/
#define CDROM_SCENE_PID       0x1f01
#define PNP_PRODUCT_ID        0x14fe  /*pnp pid*/

#define FACTORY_SCENE_PID     0x1003

#define DOWNLOAD_SCENE_PID_E5 0x1c05
#define MBIM_DOWNLOAD_PID     0x1568

#define POWER_OFF_CDROM_PID   0x1C20
#define DEBUG_PID             0x1565

#define USER_SCENE_PID        0x14db  /* RNDIS   */
#define USER_SCENE_SD_PID     0x14dc  /* RNDIS+SD*/

#define RNDIS_DEBUG_PID       0x1566  /* RNDIS Debug mode balong */

#define RNDIS_DEBUG           0x1441  /* RNDIS Debug mode qc */

#define DOWNLOAD_SCENE_PID    0x1442

#define DEFAULT_PID           0x1446
#define LOW_LINUX_PID         0x1001
#define GATEWAY_NDIS_PID      0x155E

#define DYNAMIC_PID           0x1506

//#define PRODUCT_ID              0x1506  /* Linux-USB Ethernet Gadget */
#ifdef USB_RNDIS
#define  PRODUCT_ID_CONFIG2   0x157D
#else
#define  PRODUCT_ID_CONFIG2   0x15CD
#endif/*USB_RNDIS*/


#ifdef USB_RNDIS
#define PRODUCT_ID            0x1F01
#ifdef USB_SD
#define PID_USER              0x14DC/* RNDIS+SD*/
#else
#define PID_USER              0x14db/* RNDIS   */
#endif
#else
#define PRODUCT_ID            0x15CA
#define PID_USER              0x1506
#endif/*USB_RNDIS*/

#define MBIM_DEBUG_MODE_PID   0X1567

#define DYNAMIC_DEBUG_PID     0x1506

/* 如果是多配置则通知PC取config 2，如果是单配置则通知PC取config 1 */
#define ALTRCFG_2 0x32
#define ALTRCFG_1 0x31

#define EXISTS     1
#define NON_EXISTS 0

#define TIME_OFF_MS 50

#define USB_BCDDEVICE_BALONG  0x0102
#define USB_BCDDEVICE_QC 0x0000

#define USB_SUBCLASS_BALONG              0x03  //BALONG 平台huawei usb interface subclass 
#define USB_SUBCLASS_JUNGO               0x02  //huawei usb in linux os subclass 
#define USB_SUBCLASS_LINUX               0x02  //linux下必须报0x02 否则后台不能识别
#define USB_SUBCLASS_GAOTONG             0x01  //高通平台   huawei usb interface subclass 

#define USB_REQ_TYPE_MSOS_IN             0xC0   //vendor customize request type
#define USB_REQ_TYPE_MSOS_OUT            0x40   //vendor customize request type
#define USB_REQ_GET_MSOS_DEF             0xFE   //windows not install service
#define USB_REQ_GET_MSOS_CUSTOM          0xEE   //windows  installed service
#define USB_REQ_GET_MSOS_CUSTOM_MAC      0xA1  //mac  installed service

#define OS_TYPE_MASK       0xf0 //OS高位
#define OS_TYPE_WINDOWS    0x00 //OS windos
#define OS_TYPE_MAC        0x10 //mac
#define OS_TYPE_LINUX      0x20 //linux 
#define OS_TYPE_GATEWAY    0x30 //网关类包含android chrome
#define WORK_MODE_EXT      0x01
#define DOWNLOAD_MODE_EXT  0x00

#define USB_TYPE_HUAWEI    0xc0
#define USB_REQ_HUAWEI_EXT 0x9a
#define USB_REQ_HILINK_MODEM_SWITCH 0x20

#define USB_VALUE_SWITCH   0x01
#define USB_TYPE_CRADLE    0x0120
#define USB_CRADLE_EXT     0x0100

/* MS OS String descriptor */
#define MSOS_DESC_SIZE         18
#define MSOS_STRING_INDEX      0xEE
#define MSOS_VENDOR_CODE       0x01
#define MSOS_SIGNATURE         "M\0S\0F\0T\0""1\0""0\0""0\0"
#define MS_EXTEND_COMPAT_ID    0x04
#define MS_HUAWEI_VENDOR_CODE  0xDE
#define MS_EXTEND_CONTAINER_ID 0x06

#define USB_MBB_DEVICE_INSERT    1
#define USB_MBB_DEVICE_IDEN      2
#define USB_MBB_ENUM_DONE        3
#define USB_MBB_DEVICE_REMOVE    0

#define PNP_CHECK_DEVICE_STATUS_COMPLETE 1
#define PNP_CHECK_DEVICE_STATUS_CONTINUE 0

#define SET_ADD_NONE  0
#define IS_SET_ADD    1

/*MSOS命令 begin*/
/* Microsoft Extended Configuration Descriptor Header Section */
typedef struct _ms_extend_compact_id_header
{
    USB_UINT    dwLength;
    USB_UINT16  bcdVersion;
    USB_UINT16  wIndex;
    USB_UINT8   bCount;
    USB_UINT8   Reserved[USB_NUM_7];
} ms_extend_compact_id_header;

typedef struct _ms_extend_compact_id_function
{
    USB_UINT8 bFirstInterfaceNumber;
    USB_UINT8 bInterfaceCount;
    USB_UINT8 compatibleID[USB_NUM_8];
    USB_UINT8 subCompatibleID[USB_NUM_8];
    USB_UINT8 Reserved[USB_NUM_6];
} ms_extend_compact_id_function;

typedef struct _ms_extend_container_id
{
    USB_UINT    dwLength;
    USB_UINT16  bcdVersion;
    USB_UINT16  wIndex;
    USB_UINT8   containerID[16];
} ms_extend_container_id;

/*MSOS命令 end*/

enum android_device_state
{
    USB_DISCONNECTED,
    USB_CONNECTED,
    USB_CONFIGURED,
    USB_SUSPENDED,
    USB_RESUMED
};

#define HILINK_MODEM_DEBUG_MODE_FALG    '1'
#define HILINK_MODEM_MODE_FALG          '2'
#define HILINK_RNDIS_MODE_FALG          '3'

typedef enum hilink_modem_mode
{
    HILINK_MODEM_DEBUG_MODE = 1,
    HILINK_MODEM_MODE,
    HILINK_RNDIS_MODE,
} hilink_work_mode;

typedef enum _usb_enum_state
{
    USB_ENUM_NONE = 0,
    USB_ENUM_START,
    USB_ENUM_DONE
} usb_enum_state;

typedef enum _usb_device_node_switch
{
    APP_SETMODE_NORMAL_WORK = 0,
    APP_SETMODE_NORMAL1,/*1*/
    APP_SETMODE_NORMAL2,/*2*/
    APP_SETMODE_NORMAL3,/*3*/
    APP_SETMODE_RNDIS_DEBUG,/*4*/
    APP_SETMODE_RNDIS_DLOAD,/*5*/
    APP_SETMODE_MBIM_DEBUG,  /*6*/
    APP_SETMODE_CHARGE_ONLY  /*7*/
} usb_device_node_switch;

typedef enum tagUSB_PID_UNIFY_IF_PROT_T
{
    /*通用版Protocol*/
    USB_IF_PROTOCOL_VOID         = 0x00,/*0x00*/
    USB_IF_PROTOCOL_3G_MODEM     = 0x01,/*0x01*/
    USB_IF_PROTOCOL_3G_PCUI      = 0x02,/*0x02*/
    USB_IF_PROTOCOL_3G_DIAG      = 0x03,/*0x03*/
    USB_IF_PROTOCOL_PCSC         = 0x04,/*0x04*/
    USB_IF_PROTOCOL_3G_GPS       = 0x05,/*0x05*/
    USB_IF_PROTOCOL_CTRL         = 0x06,/*0x06*/
    USB_IF_PROTOCOL_3G_NDIS      = 0x07,/*0x07*/
    USB_IF_PROTOCOL_NDISDATA     = 0x08,/*0x08*/
    USB_IF_PROTOCOL_NDISCTRL     = 0x09,/*0x09*/
    USB_IF_PROTOCOL_BLUETOOTH    = 0x0A,/*0x0A*/
    USB_IF_PROTOCOL_FINGERPRINT  = 0x0B,/*0x0B*/
    USB_IF_PROTOCOL_ACMCTRL      = 0x0C,/*0x0C*/
    USB_IF_PROTOCOL_MMS          = 0x0D,/*0x0D*/
    USB_IF_PROTOCOL_3G_PCVOICE   = 0x0E,/*0x0E*/
    USB_IF_PROTOCOL_DVB          = 0x0F,/*0x0F*/
    USB_IF_PROTOCOL_MODEM        = 0x10,/*0x10*/
    USB_IF_PROTOCOL_NDIS         = 0x11,/*0x11*/
    USB_IF_PROTOCOL_PCUI         = 0x12,/*0x12*/
    USB_IF_PROTOCOL_DIAG         = 0x13,/*0x13*/
    USB_IF_PROTOCOL_GPS          = 0x14,/*0x14*/
    USB_IF_PROTOCOL_PCVOICE      = 0x15,/*0x15*/
    USB_IF_PROTOCOL_NCM          = 0x16,/*0x16*/
    USB_IF_PROTOCOL_MODEM_ORANGE = 0x17,/*0x17*/
    USB_IF_PROTOCOL_SHEEL_A      = 0x18,/*0x18*/
    USB_IF_PROTOCOL_SHEEL_B      = 0x19,/*0x19*/
    USB_IF_PROTOCOL_COMM_A       = 0x1A,/*0x1A*/
    USB_IF_PROTOCOL_COMM_B       = 0x1B,/*0x1B*/
    USB_IF_PROTOCOL_COMM_C       = 0x1C,/*0x1C*/
    USB_IF_PROTOCOL_COMM_GNSS    = 0x1D,/*0x1D*/
    USB_IF_PROTOCOL_CDROM        = 0xA1,/*0xA1*/
    USB_IF_PROTOCOL_SDRAM        = 0xA2,/*0xA2*/
    USB_IF_PROTOCOL_RNDIS        = 0xA3,/*0xA3*/
    /*VDF定制版Protocol*/
    VDF_USB_IF_PROTOCOL_MODEM          = 0x31,/*0x31*/
    VDF_USB_IF_PROTOCOL_PCUI           = 0x32,/*0x32*/
    VDF_USB_IF_PROTOCOL_DIAG           = 0x33,/*0x33*/
    VDF_USB_IF_PROTOCOL_PCSC           = 0x34,/*0x34*/
    VDF_USB_IF_PROTOCOL_GPS            = 0x35,/*0x35*/
    VDF_USB_IF_PROTOCOL_CTRL           = 0x36,/*0x36*/
    VDF_USB_IF_PROTOCOL_NDIS           = 0x37,/*0x37*/
    VDF_USB_IF_PROTOCOL_NDISDATA       = 0x38,/*0x38*/
    VDF_USB_IF_PROTOCOL_NDISCTRL       = 0x39,/*0x39*/
    VDF_USB_IF_PROTOCOL_BLUETOOTH      = 0x3A,/*0x3A*/
    VDF_USB_IF_PROTOCOL_FINGERPRINT    = 0x3B,/*0x3B*/
    VDF_USB_IF_PROTOCOL_ACMCTRL        = 0x3C,/*0x3C*/
    VDF_USB_IF_PROTOCOL_MMS            = 0x3D,/*0x3D*/
    VDF_USB_IF_PROTOCOL_PCVOICE        = 0x3E,/*0x3E*/
    VDF_USB_IF_PROTOCOL_DVB            = 0x3F,/*0x3F*/
    VDF_USB_IF_PROTOCOL_NCM            = 0x46,/*0x46*/
    VDF_USB_IF_PROTOCOL_SHEEL_A        = 0x48,/*0x48*/
    VDF_USB_IF_PROTOCOL_SHEEL_B        = 0x49,/*0x49*/
    VDF_USB_IF_PROTOCOL_COMM_A         = 0x4A,/*0x4A*/
    VDF_USB_IF_PROTOCOL_COMM_B         = 0x4B,/*0x4B*/
    VDF_USB_IF_PROTOCOL_COMM_C         = 0x4C,/*0x4C*/
    VDF_USB_IF_PROTOCOL_COMM_GNSS      = 0x4D,/*0x4D*/
    /*无Huawei 字样*/
    NO_HW_USB_IF_PROTOCOL_MODEM           = 0x61,/*0x61*/
    NO_HW_USB_IF_PROTOCOL_PCUI            = 0x62,/*0x62*/
    NO_HW_USB_IF_PROTOCOL_DIAG            = 0x63,/*0x63*/
    NO_HW_USB_IF_PROTOCOL_PCSC            = 0x64,/*0x64*/
    NO_HW_USB_IF_PROTOCOL_GPS             = 0x65,/*0x65*/
    NO_HW_USB_IF_PROTOCOL_CTRL            = 0x66,/*0x66*/
    NO_HW_USB_IF_PROTOCOL_NDIS            = 0x67,/*0x67*/
    NO_HW_USB_IF_PROTOCOL_NDISDATA        = 0x68,/*0x68*/
    NO_HW_USB_IF_PROTOCOL_NDISCTRL        = 0x69,/*0x69*/
    NO_HW_USB_IF_PROTOCOL_BLUETOOTH       = 0x6A,/*0x6A*/
    NO_HW_USB_IF_PROTOCOL_FINGERPRINT     = 0x6B,/*0x6B*/
    NO_HW_USB_IF_PROTOCOL_ACMCTRL         = 0x6C,/*0x6C*/
    NO_HW_USB_IF_PROTOCOL_MMS             = 0x6D,/*0x6D*/
    NO_HW_USB_IF_PROTOCOL_PCVOICE         = 0x6E,/*0x6E*/
    NO_HW_USB_IF_PROTOCOL_DVB             = 0x6F,/*0x6F*/
    /*驱动端口名称带3G  无且华为字样*/
    NO_HW_USB_IF_PROTOCOL_3G_PCUI           = 0x72,/*0x72*/
    NO_HW_USB_IF_PROTOCOL_3G_DIAG           = 0x73,/*0x73*/
    NO_HW_USB_IF_PROTOCOL_3G_GPS            = 0x74,/*0x74*/
    NO_HW_USB_IF_PROTOCOL_3G_PCVOICE        = 0x75,/*0x75*/
    NO_HW_USB_IF_PROTOCOL_NCM               = 0x76,/*0x76*/
    NO_HW_USB_IF_PROTOCOL_SHEEL_A           = 0x78,/*0x78*/
    NO_HW_USB_IF_PROTOCOL_SHEEL_B           = 0x79,/*0x79*/
    NO_HW_USB_IF_PROTOCOL_COMM_A            = 0x7A,/*0x7A*/
    NO_HW_USB_IF_PROTOCOL_COMM_B            = 0x7B,/*0x7B*/
    NO_HW_USB_IF_PROTOCOL_COMM_C            = 0x7C,/*0x7C*/
    NO_HW_USB_IF_PROTOCOL_COMM_GNSS         = 0x7D,/*0x7D*/

    USB_IF_PROTOCOL_NOPNP            = 0xFF/*0xFF*/
} USB_PID_UNIFY_IF_PROT_T;

typedef enum
{
    CDROM_SCENE,
    USER_SCENE,
    FULL_SCENE,
    DOWNLOAD_SCENE,
    MODEM_FULL_SCENE,   //add by huqiao
    FACTORY_SCENE,
    CPE_SCENE,
    PWOER_OFF_SCENE,
    GATEWAY_SCENE,
    DYNAMIC_PID_SCENE,
} work_enum_scene;

/* 固定端口形态索引*/
typedef enum
{
    CDROM_MODE = 1,                  /*1*/
    RNDIS_MODE,                      /*2*/
    RNDIS_SD_MODE,                   /*3*/
    ECM_MODE,                        /*4*/
    ECM_SD_MODE,                     /*5*/
    RNDIS_DIAG_PCUI_COM_MODE,        /*6*/
    RNDIS_DIAG_PCUI_COM_SD_MODE,     /*7*/
    ECM_DIAG_PCUI_COM_MODE,          /*8*/
    ECM_DIAG_PCUI_COM_SD_MODE,       /*9*/
    MODEM_DIAG_PCUI_NDIS_CD_MODE,    /*10*/
    PCUI_ECM_DIAG_CRADLE_MODE,       /*11*/
    PCUI_DIAG_MODE,                  /*12*/
    DIAG_PCUI_MODE_EX,               /*13*/  /*升级时用的端口形态*/
    MBIM_DOWNLOAD_MODE,              /*14*/
    PCUI_DIAG_MODE_FACTORY,          /*15*/
    MBIM_SWITCH_DEBUG_MODE,          /*16*/
    MBIM_SWITCH_NORMAL_MODE,         /*17*/
    NCM_PCUI_GPS_4GDIAG_3GDIAG_CPE,  /*18*/
    RNDIS_DEBUG_MODE,                /*19*/
    RNDIS_DEBUG_MODE_SD,             /*20*/
    ECM_DEBUG_MODE,                  /*21*/
    ECM_DEBUG_MODE_SD,               /*22*/
    MODEM_DIAG_PCUI_GATEWAY,         /*23*/
    MODEM_DIAG_PCUI_NCM_GATEWAY,     /*24*/
    POWER_OFF_MODE,                  /*25*/
    /**/
    DYNAMIC_REWIND_BEFORE_MODE,      /*26*/
    DYNAMIC_REWIND_AFTER_MODE,       /*27*/
    DYNAMIC_DEBUG_MODE,              /*28*/
    HILINK_PORT_MODEM_MODE,          /*29*/
    HILINK_PORT_MODEM_DEBUG_MODE,    /*30*/
    INVALID_MODE,                    /*31*/
} ports_enum_mode;

enum port_style
{
    pnp_port_style     = 0,
    rewind_port_style,
    download_port_style,
    factory_port_style,
    charge_port_style,
};

/*  RNDIS work mode */
enum rewind_hilink_mode
{
    product_port_mode = 0,  /*  升级模式  */
    work_port_mode,         /*  工作模式  */
    debug_port_mode,        /*  调试模式  */
};

//光盘SD 支持情况
typedef enum
{
    CDROM_ONLY = 1,
    SD_ONLY,
    CDROM_SD,
    CDROM_SD_INVALID,
} cdrom_sd_style;

typedef struct  hw_usb_mode_st
{
    USB_UCHAR   port_index;  /*端口形态索引*/
    USB_CHAR*   mode_buf[DYNAMIC_PID_MAX_PORT_NUM];
    USB_INT     PID;
    USB_INT     bDeviceClass;
    USB_UINT    cdrom;
    USB_UINT    sd;
} hw_usb_mode;

typedef struct  hw_usb_dynamic_mode_st
{
    USB_UCHAR   port_index;  /*端口形态索引*/
    USB_CHAR*   mode_buf[MAX_TOTAL_PORT_NUM];
    USB_UINT8   port_protocol[MAX_TOTAL_PORT_NUM];/*串口*/
    USB_INT     PID;
} pnp_dynamic_mode;

/*rewind 切换命令*/
struct rewind_cmd_param
{
    USB_UINT8 bCmdReserved;
    USB_UINT8 bPcType;
    USB_UINT8 bTimeOut;
    USB_UINT8 bPID;
    USB_UINT8 bNewPID;
    USB_UINT8 bSupportCD;
    USB_UINT8 bProFile;
    USB_UINT8 bGreenMode;
    USB_UINT8 reserved[USB_NUM_7];
};

/*PNP 状态机定义*/
typedef struct huawei_dynamic_info
{
    USB_INT     portModeIndex;
    USB_INT     is_daynamic_sn;
    USB_INT     current_port_style;
    USB_INT     is_service_switch;
    USB_INT     app_start_mode;     // app禁止启动标志
    USB_INT     is_mbim_enable;     // 配置信息中是否包含MBIM端口
    hilink_work_mode     hilink_mode;     // hilink modem模式切换
    struct rewind_cmd_param rewind_param;
} huawei_dynamic_info_st;

typedef struct huawei_interface_info_st
{
    USB_UINT    huawei_interface_protocol_num;
    USB_CHAR*   huawei_interface_name;
} huawei_interface_info;


#endif
