/******************************************************************************************************
                                                                            
                Copyright (C), 2010, Huawei Tech. Co., Ltd.                
                            ALL RIGHTS RESERVED                             
                                                                            
------------------------------------------------------------------------------
 
 
FileName     : flash_huawei_dload.c
Version      : 1.0
Date         : 2013-07-19
Description  : This file Security Upgrade Solution Interface Fuctions
Function List: 
 
          History:
                    <author>   <date>          <desc>
----------------------------------------------------------------------------
****************************************************************************************************/
/********************************问题单修改记录******************************************
日    期       修改人         问题单号                   修改内容
************************************************************************************************/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#ifndef FLASH_DLOAD_H
#define FLASH_DLOAD_H

 /*because of nand_core.h need to define MAX_PAGE_SIZE
  So we need to define before all of include file*/
#include "mbb_config.h"
#if (FEATURE_ON == MBB_DLOAD)
//#include "nandc_balong.h"
/*===========================================================================

                           MACRO DEFINITIONS

===========================================================================*/
#define MAX_SOFTWARE_VERSION_LENGTH       32
#define MAGIC_DEFAULT                         0x0
#define SHARE_RGN_MAGIC                       0x41454F4C
#define USER_INFO_FLAG_MAGIC_NUM              0x55534552   /* "USER" */

#ifndef FLASH_OEM_REGION_IN_USE_FLAG
#define FLASH_OEM_REGION_IN_USE_FLAG          0x59455320  
#endif
#ifndef ISO_INFO_DATA_MAGIC
#define ISO_INFO_DATA_MAGIC                   0x204F5349
#endif
#ifndef NAND_FILL_CHAR_APP
#define NAND_FILL_CHAR_APP                    0xFF
#endif

#ifndef MSG_ERROR
#define MSG_ERROR(x,a,b,c)
#endif

#define FLASH_NUM_PART_ENTRIE                 16         

#define INVALID_PAGE_ID                       0xFFFFFFFFU
#define INVALID_BLOCK_ID                      0xFFFFFFFFU
#define INVALID_CLUSTER_ID                    0xFFFFFFFFU
#define HUAWEI_NO_CDROM_MAGIC_NUM             0x47569842


#define  FS_HANDLE_ERR                       (-1)
#define  EFS_LOG_SAVE_SIZE                   80
#define  MAX_EFS_LOG_SAVE_SIZE               100
#define  SIG_UIM_TASK_STOP                   0x00004000
#define  OFFLINE_SLEEP_TIME                  3000
#define  MAX_PAGE_SIZE                       4096

#define DLOAD_FLASH_OEM_REGION_USE_FLAG                  0x59455320          /*OEM分区使用魔术字*/
#define DLOAD_ISO_VERSION_MAGIC                          0x204F5349
#define DLOAD_WEB_VERSION_MAGIC                          0x20534F49


typedef int int32;
typedef short int16;
typedef char int8;
typedef unsigned long long uint64;
typedef long long int64;
typedef unsigned long uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;
typedef unsigned char boolean;

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/
typedef struct
{
  uint8  parent_name[FLASH_NUM_PART_ENTRIE];       /* 父分区名 */
  uint32 magic_dload;                                /* 分区魔数 */
  uint32 magic_restore;
  uint32 offset;                                   /* 子分区与父分区首地址的偏移量 */ 
  uint32 length;                                   /* 子分区长度 */
  void   *content_ptr;
  uint16 content_size;
} child_region_t;

/* 子分区结构体 */

typedef struct 
{
  uint32 in_use_flag;
  uint32 nv_autorestore_magic;
  uint8  nv_autorestore_flag;
} nv_autorestore_type;

  
typedef enum
{
  AUTO_RESTORE_NONE = 0xFF,  /*空值*/
  AUTO_RESTORE_ALLOW = 0,
  AUTO_RESTORE_FORBID,
  AUTO_RESTORE_FAIL,
  /* Newly introduced flag value to additionally indicate failure because of inexistent backup */
  AUTO_RESTORE_FORBID_NO_BACKUP,   
} nv_auto_restore_enum_t;

typedef enum                            
{
  SHARE_REGION = 0,
  REGION_MAX
} oper_region_type;


/* 分区操作结构体 */
typedef struct
{

  uint32 start_addr;
  uint32 block_size;
  uint32 page_size;
  uint8        *buffer;
} oper_region_struct_t;

/* 本结构是ISO信息块的存储头，包括本blck是否使用以及标志该blck的魔数 */
#define ISO_INFO_FLAG_LEN  9  /* 定义出了对应于下面这个结构体的长度的宏，因为sizeof(iso_info_flag_type)的结果为12 */
typedef struct 
{
  uint32 in_use_flag;
  uint32 iso_info_magic;
  uint8  iso_info_flag;
} iso_info_flag_type;

typedef struct rgn_hd_struct
{
    uint32 magic;
    uint32   sub_rgn_num;
}rgn_hd_type;

typedef enum
{
    RGN_HD_INFO_FLAG = 0,
    RGN_RESTORE_FLAG,
    RGN_NVMBN_FLAG,
    RGN_ISOHD_FLAG,
    RGN_SWVER_FLAG,
    RGN_WEBHD_FLAG,
    RGN_PART_COMPLETE_FLAG,
    RGN_DLOAD_RECOVERY_FLAG,
    RGN_EFS_WHOLE_BACKUP_RESTORE_FLAG,
    RGN_TOUCHSCREEN_INFO,
    RGN_PRL_NAM0_FLAG,
    RGN_PRL_NAM1_FLAG,
    RGN_RECORD_UPINFO_FLAG,
    RESERVE1_FLAG,
    RESERVE2_FLAG,
    RESERVE3_FLAG,
    RESERVE4_FLAG,
    RESERVE5_FLAG,
    RGN_MAX_NUM
}rgn_pos_e_type;

#define NV_SWVER_FLAG_MAGIC_NUM  0x9F8E7D6C

typedef   struct
{  
   uint32  magic_number;  
   uint8   version_page_buffer[MAX_SOFTWARE_VERSION_LENGTH];  
} version_buffer_page;

extern child_region_t  child_region[];
extern oper_region_struct_t   operation_region[];


#define PART_COMPLETE_FLAG_MAGIC_NUM 0x50434D4E   /*PCMN*/
typedef   struct
{  
   uint32   magic_number;  
   uint32   complete_flag;      /*0:下载模式，1: 正常模式*/
   uint32   dload_times;       /*记录升级次数*/
} part_complete_flag;

#define DLOAD_RECOVERY_FLAG_MAGIC_NUM 0x44524543   /*DREC*/
typedef   struct
{  
   uint32   magic_number;  
   uint32   new_recovery_flag;      /*0: RECOVERYA   1: RECOVERYB*/
} recovery_type_flag;
/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/************************************************************************
DESCRIPTION： 初始化用户操作分区信息，取得NAND Flash设备操作函数指针等
CALLED BY  :  ui_nv_restore
ui_nv_backup
CALLS      :  flash_nand_device_probe
              flash_nand_open_partition
RETURN     :  
*************************************************************************/
oper_region_struct_t *flash_nand_oper_region_init(  oper_region_type  oper_region_idx);



/*===========================================================================
FUNCTION FLASH_NAND_OPER_REGION_CLOSE
 
DESCRIPTION
  close flash device and release flash handle
 
DEPENDENCIES
  None.
 
RETURN VALUE
  TRUE:  valid
  FALSE: invalid
 
SIDE EFFECTS
  None.
 
===========================================================================*/
void flash_nand_oper_region_close(oper_region_type  oper_region_idx);

/*===========================================================================
 
FUNCTION  FLASH_UPDATE_SHARE_REGION_INFO
 
DESCRIPTION
    update user flag,nv_mbn,and iso header info in the share region.
    This function use page to store these info, 

    Page 0: Share region Info:
           Magic:
           Sub region len:
    Page 1: Nv restore flag:
           In Used Flag Magic: 4 Byte
           Auto Restore Magic: 4Byte
           Auto Restore Flag: 
    Page 2:
          In User Flag Magic:
          MBN Magic:
          MBN Info Num:
          MBN: Version:
          MBN nv data
    Page 3:
          ISO Header Info;
          
    We use two block to implement this feature, when the first time to update this feature directly write it to the 
first available block,and then we update this info then firstly read the page info to cache, update the cache value
and then write it to another block, erase lasted block.

Author: ChenFeng 2010-3-20

RETURN VALUE
    TRUE if Op Succeed
    FALSE if Op Failure
 
SIDE EFFECTS
  None
 
===========================================================================*/
boolean flash_update_share_region_info(rgn_pos_e_type region_type,void* data);
/*===========================================================================
 
FUNCTION  flash_get_share_region_info
 
DESCRIPTION
    Get Correspoding field info
Author: ChenFeng 2010-3-20

RETURN VALUE
    TRUE if Op Succeed
    FALSE if Op Failure
 
SIDE EFFECTS
  None
 
===========================================================================*/
boolean flash_get_share_region_info(rgn_pos_e_type region_type,void* data,uint32 data_len);
#endif
#endif/*FLASH_DLOAD_H*/
