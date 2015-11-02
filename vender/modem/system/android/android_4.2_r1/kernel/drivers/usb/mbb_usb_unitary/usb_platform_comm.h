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
    问题单号           时间              修改人                       说明
*****************************************************************************************************/
#ifndef __USB_PLATFORM_COMM_H__
#define __USB_PLATFORM_COMM_H__

#include "usb_config.h"
#include <linux/types.h>

#ifndef MBB_NV_ENABLE
#define MBB_NV_ENABLE             (1)
#endif

#ifndef MBB_NV_DISENABLE
#define MBB_NV_DISENABLE             (0)
#endif


#ifndef MBB_USB_OK
#define MBB_USB_OK              (0)
#endif

#ifndef MBB_USB_ERROR
#define MBB_USB_ERROR           (-1)
#endif

#ifndef MBB_USB_TRUE
#define MBB_USB_TRUE            (1)
#endif

#ifndef MBB_USB_FALSE
#define MBB_USB_FALSE           (0)
#endif

#ifndef MBB_USB_NULL
#define MBB_USB_NULL            (void*)0
#endif

#ifndef MBB_USB_YES
#define MBB_USB_YES             (1)
#endif

#ifndef MBB_USB_NO
#define MBB_USB_NO              (0)
#endif

#define USB_NUM_0    0 //USB自定义数字
#define USB_NUM_1    1
#define USB_NUM_2    2
#define USB_NUM_3    3
#define USB_NUM_4    4
#define USB_NUM_5    5
#define USB_NUM_6    6
#define USB_NUM_7    7
#define USB_NUM_8    8
#define USB_NUM_9    9
#define USB_NUM_10    10
#define USB_NUM_11    11
#define USB_NUM_12    12
#define USB_NUM_13    13
#define USB_NUM_14    14
#define USB_NUM_15    15
#define USB_NUM_16    16
#define USB_NUM_17    17
#define USB_NUM_18    18
#define USB_NUM_19    19
#define USB_NUM_20    20
#define USB_NUM_21    21
#define USB_NUM_22    22
#define USB_NUM_23    23
#define USB_NUM_24    24
#define USB_NUM_25    25
#define USB_NUM_26    26
#define USB_NUM_27    27
#define USB_NUM_28    28
#define USB_NUM_29    29
#define USB_NUM_30    30
#define USB_NUM_31    31
#define USB_NUM_32    32
#define USB_NUM_33    33
#define USB_NUM_34    34
#define USB_NUM_35    35
#define USB_NUM_36    36
#define USB_NUM_37    37
#define USB_NUM_38    38
#define USB_NUM_39    39
#define USB_NUM_40    40
#define USB_NUM_41    41
#define USB_NUM_42    42
#define USB_NUM_43    43
#define USB_NUM_44    44
#define USB_NUM_45    45
#define USB_NUM_46    46
#define USB_NUM_47    47
#define USB_NUM_48    48
#define USB_NUM_49    49
#define USB_NUM_50    50
#define USB_NUM_51    51
#define USB_NUM_52    52
#define USB_NUM_53    53
#define USB_NUM_54    54
#define USB_NUM_55    55
#define USB_NUM_56    56
#define USB_NUM_57    57
#define USB_NUM_58    58
#define USB_NUM_59    59
#define USB_NUM_60    60
#define USB_NUM_61    61
#define USB_NUM_62    62
#define USB_NUM_63    63
#define USB_NUM_64    64
#define USB_NUM_65    65
#define USB_NUM_66    66
#define USB_NUM_67    67
#define USB_NUM_68    68
#define USB_NUM_69    69
#define USB_NUM_70    70
#define USB_NUM_71    71
#define USB_NUM_72    72
#define USB_NUM_73    73
#define USB_NUM_74    74
#define USB_NUM_75    75
#define USB_NUM_76    76
#define USB_NUM_77    77
#define USB_NUM_78    78
#define USB_NUM_79    79
#define USB_NUM_80    80
#define USB_NUM_81    81
#define USB_NUM_82    82
#define USB_NUM_83    83
#define USB_NUM_84    84
#define USB_NUM_85    85
#define USB_NUM_86    86
#define USB_NUM_87    87
#define USB_NUM_88    88
#define USB_NUM_89    89
#define USB_NUM_90    90
#define USB_NUM_91    91
#define USB_NUM_92    92
#define USB_NUM_93    93
#define USB_NUM_94    94
#define USB_NUM_95    95
#define USB_NUM_96    96
#define USB_NUM_97    97
#define USB_NUM_98    98
#define USB_NUM_99    99
#define USB_NUM_100   100
#define USB_NUM_256   256
#define USB_NUM_0x01 0x01
#define USB_NUM_0x03 0x03
#define USB_NUM_0x0200 0x0200
#define USB_NUM_0x9999 0x9999
#define USB_NUM_0xffff 0xffff

/* 用于测试的魔鬼数字 */
#define USB_NUM_AA    0xAA
#define USB_NUM_BB    0xBB
#define USB_NUM_CC    0xCC
#define USB_NUM_DD    0xDD

/* 用于测试的数据包长度上下限 */
#define USB_TEST_PKT_LEN_MIN  14
#define USB_TEST_PKT_LEN_MAX  1514

typedef  unsigned int         USB_SIZE_T;	
typedef  unsigned char        USB_BOOL;
typedef  unsigned int         USB_UINT;      /* Unsigned int value */
typedef  int                  USB_INT;         /* int value */
typedef  unsigned long long   USB_UINT64;      /* Unsigned 64 bit value */
typedef  long long            USB_INT64;       /* Signed 64 bit value */
typedef  unsigned long int    USB_UINT32;      /* Unsigned 32 bit value */
typedef  unsigned short       USB_UINT16;      /* Unsigned 16 bit value */
typedef  unsigned char        USB_UINT8;       /* Unsigned 8 bit value */
typedef  unsigned char*       USB_PUINT8;       /* Unsigned 8 bit value */
typedef  unsigned char        USB_UCHAR;       /* Unsigned 8  bit charcter */
typedef  unsigned char*       USB_PUCHAR;
typedef  signed long int      USB_INT32;       /* Signed 32 bit value */
typedef  signed short         USB_INT16;       /* Signed 16 bit value */
typedef  char                 USB_CHAR;        /* Signed 8  bit character value */
typedef  signed char*         USB_PCHAR;
typedef  long                 USB_LONG;
typedef  unsigned long        USB_ULONG;
typedef  void                 USB_VOID;
typedef  void*                USB_PVOID;
//typedef  const signed char*   USB_PCSTR;
//typedef  signed char*         USB_PSTR;
//typedef  unsigned char        USB_BYTE;         /* Unsigned 8  bit value type. */
//typedef  signed long int      USB_STATUS;      /* signed 32 bit */
//typedef  signed short         USB_SHORT;
//typedef  unsigned short       USB_USHORT;
//typedef  signed short         USB_WCHAR;       /* Signed 16 bit character value */
//typedef  signed short*        USB_PWCHAR;
//typedef  unsigned short       USB_UWCHAR;      /* Unsigned 16 bit CHARCTER value */
//typedef  unsigned short*      USB_PUWCHAR;
//typedef  long long int*       USB_PINT64;
//typedef  unsigned int*        USB_PUINT;     /* Unsigned int * value */
//typedef  unsigned int         USB_SIZE_T;//del

#define GNET_MAX_NUM       (4)

#ifdef USB_DUAL_CORE_CPE
#define GNET_USED_NUM      (4)
#elif defined(CONFIG_NCM_MBIM_SUPPORT)
#define GNET_USED_NUM      GNET_MAX_NUM   /* M-PDP */
#else
#define GNET_USED_NUM      (1)
#endif
/*****************************************************************************
* 函 数 名  : usb_read_nv
*
* 功能描述  : A核读NV接口函数
*
* 输入参数  : usID -> NV ID
              *pItem -> Save NV data buffer
              ulLength -> buffer length
* 输出参数  : 无
*
* 返 回 值  : 0:    操作成功
*                       -1: 操作失败
*
* 其它说明  : 无
*
*****************************************************************************/

USB_INT usb_read_nv(USB_UINT16 usID, USB_PVOID pItem, USB_UINT ulLength);

#endif
