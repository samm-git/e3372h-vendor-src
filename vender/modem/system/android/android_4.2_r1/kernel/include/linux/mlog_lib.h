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
#ifndef __MLOG_PRINT_H__
#define __MLOG_PRINT_H__
#include "product_config.h"
#ifdef __cplusplus
extern "C" {
#endif
#ifndef _UINT32_DEFINED
typedef  unsigned  int  uint32;      /* Unsigned 32 bit value */
#define _UINT32_DEFINED
#endif

#ifndef _UINT16_DEFINED
typedef  unsigned short     uint16;      /* Unsigned 16 bit value */
#define _UINT16_DEFINED
#endif

#ifndef _UINT8_DEFINED
typedef  unsigned char      uint8;       /* Unsigned 8  bit value */
#define _UINT8_DEFINED
#endif

#ifndef _INT32_DEFINED
typedef  signed  int    int32;       /* Signed 32 bit value */
#define _INT32_DEFINED
#endif

#define MAX_ITEM_LEN  256
typedef enum _mlog_lv
{
    mlog_lv_none  = 0, /* NONE */
    mlog_lv_fatal = 1, /* FATAL */
    mlog_lv_error = 2, /* ERROR */
    mlog_lv_warn  = 4, /* WARNING */
    mlog_lv_info  = 8 /* INFO */
} mlog_lv_eum;

#define  MLOG_PM    "PM"
#define  MLOG_THM   "THM"
#define  MLOG_KEY   "KEY"
#define  MLOG_CHG   "CHG"


#define APP_MBIM_MODULE "mbim"
#define APP_USB_MODULE "usb"

#define  MLOG_POWERON_RESTART  "poweron_restart"
#define  MLOG_POWER  "PWR"

#define FLASH_ERASE_WRITE_MAGIC 0x20140519
#define MAX_FS_PARTITI_COUNT 32             //yaffs max partition 
#define MAX_FS_PARTITI_NAME_LEN  128

#define MLOG_DEBUG_LEVEL 0x01
#define EVENT_DEBUG_LEVEL  (0x01 << 1)
#define YAFFS_DEBUG_LEVEL  (0x01 << 2)
#define FLASH_DEBUG_LEVEL  (0x01 << 3)

#define APP_CRADLE_MODULE "cradle"
#define APP_DEVICE_MODULE "device"
#define APP_DIALUP_MODULE "dialup"
#define APP_WIFI_MODULE "WiFi"
#define APP_POWER_MANAGEMENT "power"
#define APP_NETWORK_MODULE "network"
#define APP_WAN_MODULE "wan"
#define APP_STATUS_MODULE "status"
#define APP_WEBSERVER_MODULE "webserver"
#define APP_BBOU_MODULE  "bbou"
#define APP_SYSGUARD_MODULE "sysguard"
#define APP_SYSWATCH_MODULE "syswatch"


#define APP_NP_WIFI_MODULE  "NP_WIFI"
#define APP_TIME_MODULE  "TIME"



#define APP_NPWAN_MODULE  "NP_WAN"


#define MLOG_THM "THM"
#define MLOG_UPG "UPG"
#define MLOG_CHG "CHG"


#define MLOG_DEBUG_LEVEL 0x01
#define COMM_DEBUG_LEVEL  (0x01 << 1)
#define YAFFS_DEBUG_LEVEL  (0x01 << 2)
#define FLASH_DEBUG_LEVEL (0x01 << 3)

typedef struct
{
    uint32 blockid;
    uint32 erasetimes;
    uint32 writetimes;
    uint32   flags;          /*set the flag when erase/write*/
} flash_stat_info_t;

typedef struct yaffs_dev_stat
{
    char name[MAX_FS_PARTITI_NAME_LEN] ; /*device name*/
    uint32  n_page_writes;
    uint32  n_page_reads;
    uint32  n_erasures;
    uint32  n_erase_failures;
    uint32  n_gc_copies;
    uint32  all_gcs;
    uint32  passive_gc_count;
    uint32  oldest_dirty_gc_count;
    uint32  n_gc_blocks;
    uint32  bg_gcs;
    uint32  n_retried_writes;
    uint32  n_retired_blocks;
    uint32  n_ecc_fixed;
    uint32  n_ecc_unfixed;
    uint32  n_tags_ecc_fixed;
    uint32  n_tags_ecc_unfixed;
    uint32  n_deletions;
    uint32  n_unmarked_deletions;
    uint32  refresh_count;
    uint32  cache_hits;
    uint32  flags;  /*setting when change*/
} yaffs_dev_stat_t;

typedef struct yaffs_dev_statis_info
{
    yaffs_dev_stat_t yaffs_info_stat[MAX_FS_PARTITI_COUNT];
    unsigned long int  flags;    /*as partiton share flag*/
} yaffs_dev_statis_info_t;

#define MAX_ITEM_LEN  256
#define MSTAT_BUF_MAX_LEN  1024

typedef struct
{
    char item_name[MAX_ITEM_LEN];
    char item_value[MAX_ITEM_LEN];

    unsigned int item_flags;  /*as the shutdown flag*/

} mlog_server_stat_info;


#define MLOG_YAFFS "YAFFS2"


#if (FEATURE_OFF == MBB_MLOG)
#define  mlog_print(format, ...)
#define mlog_set_statis_info(item_name1,item_value2)
#define sys_shutdown_mog_report()
#else
void mlog_print(char* module, mlog_lv_eum loglv, const char* fmt, ...);
void mlog_set_statis_info(char* item_name, unsigned int item_value);
void  sys_shutdown_mog_report(void);
#endif

#ifdef __cplusplus
}
#endif

#endif
