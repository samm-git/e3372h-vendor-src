/*******************************************************************************
* Copyright (C), 2008-2011, HUAWEI Tech. Co., Ltd.
*
* Module name: Boot Loader
*
* Version: v1.0
*
* Filename:    parttable.c
* Description:  Balong plantform boot loadr partition image(s) loading function
*
* Function List:
*
* History:
1.date:2011-11-19
 question number:
 modify reasion:         create
*******************************************************************************/
/******************************************************************************
*    Copyright (c) 2009-2011 by  Hisilicon Tech. Co., Ltd.
*    All rights reserved.
* ***
*
******************************************************************************/
/*******************************问题单修改记录********************************
日期            问题单号            修改人          修改内容
******************************************************************************/

#ifdef __cplusplus
extern "C"
{
#endif

#include "ptable_def.h"

/*----------------------- |  0 byte
 |"pTableHead"            |
 *----------------------- | 16 byte (partition head flag string)
 | the property of table  |
 *----------------------- | 20 byte (partition head flag string)
 |"V7R2_FPGA" (example.)  |
 *----------------------- | 48 byte (partition table version name)
 | <partition info>       |
 |  (size 32byte)         |
 *----------------------- | 96 byte
 | < partition info >     |
 |  (size 32byte)         |
 |------------------------| 144 byte
 :    .............       :
 :    .............       :
 |------------------------| 48 x N byte
 | < partition info >     |
 |  (size 32byte)         |
 |------------------------| 48 x (N+1) byte
 | "T"  (table end flag)  |
 |                        |
 |------------------------| */

//#pragma GCC push_options
//#pragma GCC optimize("O0")

char ptable_tail[PTABLE_TAIL_SIZE] = {PTABLE_TAIL_STR};
/***********************************************************************
 * FLASH空间分配(产品线分区表)
 ***********************************************************************/
#if(FEATURE_ON == MBB_DLOAD)
/* M3boot */
#define MBB_PTABLE_M3BOOT_START             (0)
#define MBB_PTABLE_M3BOOT_END               (MBB_PTABLE_M3BOOT_START + MBB_PTABLE_M3BOOT_LEN)

/* Fastboot */
#define MBB_PTABLE_FASTBOOT_START           MBB_PTABLE_M3BOOT_END
#define MBB_PTABLE_FASTBOOT_END             (MBB_PTABLE_FASTBOOT_START + MBB_PTABLE_FASTBOOT_LEN)

/*nvback*/
#define MBB_PTABLE_NVBACK_START             MBB_PTABLE_FASTBOOT_END
#define MBB_PTABLE_NVBACK_END               (MBB_PTABLE_NVBACK_START + MBB_PTABLE_NVBACK_LEN)

/*nvdefault*/
#define MBB_PTABLE_NVDEFAULT_START          MBB_PTABLE_NVBACK_END
#define MBB_PTABLE_NVDEFAULT_END            (MBB_PTABLE_NVDEFAULT_START + MBB_PTABLE_NVDEFAULT_LEN)

/*nvimage*/
#define MBB_PTABLE_NVIMAGE_START            MBB_PTABLE_NVDEFAULT_END
#define MBB_PTABLE_NVIMAGE_END              (MBB_PTABLE_NVIMAGE_START + MBB_PTABLE_NVIMAGE_LEN)

/*nvdload*/
#define MBB_PTABLE_NVDLOAD_START            MBB_PTABLE_NVIMAGE_END
#define MBB_PTABLE_NVDLOAD_END              (MBB_PTABLE_NVDLOAD_START + MBB_PTABLE_NVDLOAD_LEN)

/*nvupdate*/
#define MBB_PTABLE_NVUPDATE_START           MBB_PTABLE_NVDLOAD_END
#define MBB_PTABLE_NVUPDATE_END             (MBB_PTABLE_NVUPDATE_START + MBB_PTABLE_NVUPDATE_LEN)

/*oeminfo*/
#define MBB_PTABLE_OEMINFO_START            MBB_PTABLE_NVUPDATE_END
#define MBB_PTABLE_OEMINFO_END              (MBB_PTABLE_OEMINFO_START + MBB_PTABLE_OEMINFO_LEN)

/*change usrdata and online partitions sequence in stick and hilink product currently*/
/*kernel*/
#define MBB_PTABLE_KERNEL_START             MBB_PTABLE_OEMINFO_END
#define MBB_PTABLE_KERNEL_END               (MBB_PTABLE_KERNEL_START + MBB_PTABLE_KERNEL_LEN)

/*kernelbk*/
#define MBB_PTABLE_KERNELBK_START           MBB_PTABLE_KERNEL_END
#define MBB_PTABLE_KERNELBK_END             (MBB_PTABLE_KERNELBK_START + MBB_PTABLE_KERNELBK_LEN)

/*logo*/
#define MBB_PTABLE_LOGO_START               MBB_PTABLE_KERNELBK_END
#define MBB_PTABLE_LOGO_END                 (MBB_PTABLE_LOGO_START + MBB_PTABLE_LOGO_LEN)

/*m3image*/
#define MBB_PTABLE_M3IMAGE_START            MBB_PTABLE_LOGO_END
#define MBB_PTABLE_M3IMAGE_END              (MBB_PTABLE_M3IMAGE_START + MBB_PTABLE_M3IMAGE_LEN)

/*dsp*/
#define MBB_PTABLE_DSP_START                MBB_PTABLE_M3IMAGE_END
#define MBB_PTABLE_DSP_END                  (MBB_PTABLE_DSP_START + MBB_PTABLE_DSP_LEN)

/*misc*/
#define MBB_PTABLE_MISC_START               MBB_PTABLE_DSP_END
#define MBB_PTABLE_MISC_END                 (MBB_PTABLE_MISC_START + MBB_PTABLE_MISC_LEN)

/*Vxwork*/
#define MBB_PTABLE_VXWORK_START             MBB_PTABLE_MISC_END
#define MBB_PTABLE_VXWORK_END               (MBB_PTABLE_VXWORK_START + MBB_PTABLE_VXWORK_LEN)

/*reserve1*/
#define MBB_PTABLE_RESERVE1_START           MBB_PTABLE_VXWORK_END
#define MBB_PTABLE_RESERVE1_END             (MBB_PTABLE_RESERVE1_START + MBB_PTABLE_RESERVE1_LEN)

/*reserve2*/
#define MBB_PTABLE_RESERVE2_START           MBB_PTABLE_RESERVE1_END
#define MBB_PTABLE_RESERVE2_END             (MBB_PTABLE_RESERVE2_START + MBB_PTABLE_RESERVE2_LEN)

/*reserve3*/
#define MBB_PTABLE_RESERVE3_START           MBB_PTABLE_RESERVE2_END
#define MBB_PTABLE_RESERVE3_END             (MBB_PTABLE_RESERVE3_START + MBB_PTABLE_RESERVE3_LEN)

/*om*/
#define MBB_PTABLE_OM_START                 MBB_PTABLE_RESERVE3_END
#define MBB_PTABLE_OM_END                   (MBB_PTABLE_OM_START + MBB_PTABLE_OM_LEN)

/*websdk --  app*/
#define MBB_PTABLE_WEBSDK_START             MBB_PTABLE_OM_END
#define MBB_PTABLE_WEBSDK_END               (MBB_PTABLE_WEBSDK_START + MBB_PTABLE_WEBSDK_LEN)

/*webui*/
#define MBB_PTABLE_WEBUI_START              MBB_PTABLE_WEBSDK_END
#define MBB_PTABLE_WEBUI_END                (MBB_PTABLE_WEBUI_START + MBB_PTABLE_WEBUI_LEN)

/*system*/
#define MBB_PTABLE_SYSTEM_START             MBB_PTABLE_WEBUI_END
#define MBB_PTABLE_SYSTEM_END               (MBB_PTABLE_SYSTEM_START + MBB_PTABLE_SYSTEM_LEN)

/*change usrdata and online partitions sequence in stick and hilink product currently*/
/*userdata*/
#define MBB_PTABLE_USERDATA_START           MBB_PTABLE_SYSTEM_END
#define MBB_PTABLE_USERDATA_END             (MBB_PTABLE_USERDATA_START + MBB_PTABLE_USERDATA_LEN)

/*bbou  -- online*/
#define MBB_PTABLE_BBOU_START               MBB_PTABLE_USERDATA_END
#define MBB_PTABLE_BBOU_END                 (MBB_PTABLE_BBOU_START + MBB_PTABLE_BBOU_LEN)

/*mmc*/
#define MBB_PTABLE_MMC_START                MBB_PTABLE_BBOU_END
#define MBB_PTABLE_MMC_END                  (MBB_PTABLE_MMC_START + MBB_PTABLE_MMC_LEN)

/*cache*/
#define MBB_PTABLE_CACHE_START              MBB_PTABLE_MMC_END
#define MBB_PTABLE_CACHE_END                (MBB_PTABLE_CACHE_START + MBB_PTABLE_CACHE_LEN)

/*recovery-a*/
#define MBB_PTABLE_RECVA_START              MBB_PTABLE_CACHE_END
#define MBB_PTABLE_RECVA_END                (MBB_PTABLE_RECVA_START + MBB_PTABLE_RECVA_LEN)

/*recovery-b*/
#define MBB_PTABLE_RECVB_START              MBB_PTABLE_RECVA_END
#define MBB_PTABLE_RECVB_END                (MBB_PTABLE_RECVB_START + MBB_PTABLE_RECVB_LEN)


/*kernel entry Address*/
#define MBB_PTABLE_KERNEL_ENTRY             (PTABLE_FASTBOOT_ENTRY + MBB_PTABLE_FASTBOOT_LEN)
/*table partition begin*/
/*
 * Size of ptable.bin is set 2KB, so max number of partitions is 42, if you have to use more than 42 partitions, you must modify somewhere,
 * e.g. fastboot(load ptable, burn ptable.bin to nand), t32 script(nandwrite.cmm).
 */
struct ST_PART_TBL ptable_product[PTABLE_PARTITION_MAX] =
{
    /*name*/         /*offset*/                 /*loadsize*/ /*capacity*/              /*loadaddr*/                /*entry*/                   /*type*/          /*property*/                                  /*count*/
#if (0 != MBB_PTABLE_M3BOOT_LEN)
    {"m3boot"       ,MBB_PTABLE_M3BOOT_START    ,0x00000000  ,MBB_PTABLE_M3BOOT_LEN    ,0x00000000                 ,0x00000000                ,IMAGE_M3BOOT     ,DATA_NORMAL | PTABLE_MOUNT_MTD                      ,0},
#endif
#if (0 != MBB_PTABLE_FASTBOOT_LEN)
    {"fastboot"     ,MBB_PTABLE_FASTBOOT_START  ,0x00000000  ,MBB_PTABLE_FASTBOOT_LEN  ,PTABLE_FASTBOOT_ENTRY      ,PTABLE_FASTBOOT_ENTRY     ,IMAGE_FASTBOOT   ,DATA_NORMAL | PTABLE_MOUNT_MTD                      ,0},
#endif
#if (0 != MBB_PTABLE_NVBACK_LEN)
    {"nvbacklte"    ,MBB_PTABLE_NVBACK_START    ,0x00000000  ,MBB_PTABLE_NVBACK_LEN    ,0x00000000                 ,0x00000000                ,IMAGE_NVBACKLTE  ,DATA_NORMAL | PTABLE_PROTECTED | PTABLE_MOUNT_MTD   ,0},
#endif
#if (0 != MBB_PTABLE_NVDEFAULT_LEN)
    {"nvdefault"    ,MBB_PTABLE_NVDEFAULT_START ,0x00000000  ,MBB_PTABLE_NVDEFAULT_LEN ,0x00000000                 ,0x00000000                ,IMAGE_NVFACTORY  ,DATA_NORMAL | PTABLE_MOUNT_MTD                      ,0},
#endif
#if (0 != MBB_PTABLE_NVIMAGE_LEN)
    {"nvimg"        ,MBB_PTABLE_NVIMAGE_START   ,0x00000000  ,MBB_PTABLE_NVIMAGE_LEN   ,0x00000000                 ,0x00000000                ,IMAGE_NVIMG      ,DATA_NORMAL | PTABLE_MOUNT_MTD                      ,0},
#endif
#if (0 != MBB_PTABLE_NVDLOAD_LEN)
    {"nvdload"      ,MBB_PTABLE_NVDLOAD_START   ,0x00000000  ,MBB_PTABLE_NVDLOAD_LEN   ,0x00000000                 ,0x00000000                ,IMAGE_NVDLD      ,DATA_NORMAL | PTABLE_MOUNT_MTD                      ,0},
#endif
#if (0 != MBB_PTABLE_NVUPDATE_LEN)
    {"nvupdate"     ,MBB_PTABLE_NVUPDATE_START  ,0x00000000  ,MBB_PTABLE_NVUPDATE_LEN  ,0x00000000                 ,0x00000000                ,IMAGE_NVDLD      ,DATA_NORMAL | PTABLE_MOUNT_MTD                      ,0},
#endif
#if (0 != MBB_PTABLE_OEMINFO_LEN)
    {"oeminfo"      ,MBB_PTABLE_OEMINFO_START   ,0x00000000  ,MBB_PTABLE_OEMINFO_LEN   ,0x00000000                 ,0x00000000                ,IMAGE_OEMINFO    ,DATA_NORMAL | PTABLE_MOUNT_MTD                      ,0},
#endif
#if (0 != MBB_PTABLE_KERNEL_LEN)
    {"kernel"       ,MBB_PTABLE_KERNEL_START    ,0x00000000  ,MBB_PTABLE_KERNEL_LEN    ,MBB_PTABLE_KERNEL_ENTRY    ,MBB_PTABLE_KERNEL_ENTRY   ,IMAGE_KERNER     ,DATA_NORMAL | PTABLE_MOUNT_MTD                      ,0},
#endif
#if (0 != MBB_PTABLE_KERNELBK_LEN)
    {"kernelbk"     ,MBB_PTABLE_KERNELBK_START  ,0x00000000  ,MBB_PTABLE_KERNELBK_LEN  ,MBB_PTABLE_KERNEL_ENTRY    ,MBB_PTABLE_KERNEL_ENTRY   ,IMAGE_KERNELBK   ,DATA_NORMAL | PTABLE_MOUNT_MTD                      ,0},
#endif
#if (0 != MBB_PTABLE_LOGO_LEN)
    {"logo"         ,MBB_PTABLE_LOGO_START      ,0x00000000  ,MBB_PTABLE_LOGO_LEN      ,0x00000000                 ,0x00000000                ,IMAGE_LOGO       ,DATA_NORMAL | PTABLE_MOUNT_MTD                      ,0},
#endif
#if (0 != MBB_PTABLE_M3IMAGE_LEN)
    {"m3image"      ,MBB_PTABLE_M3IMAGE_START   ,0x00000000  ,MBB_PTABLE_M3IMAGE_LEN   ,0x00000000                 ,0x00000000                ,IMAGE_M3IMAGE    ,DATA_NORMAL | PTABLE_MOUNT_MTD                      ,0},
#endif
#if (0 != MBB_PTABLE_DSP_LEN)
    {"dsp"          ,MBB_PTABLE_DSP_START       ,0x00000000  ,MBB_PTABLE_DSP_LEN       ,0x00000000                 ,0x00000000                ,IMAGE_DSP        ,DATA_NORMAL | PTABLE_MOUNT_MTD                      ,0},
#endif
#if (0 != MBB_PTABLE_MISC_LEN)
    {"misc"         ,MBB_PTABLE_MISC_START      ,0x00000000  ,MBB_PTABLE_MISC_LEN      ,0x00000000                 ,0x00000000                ,IMAGE_MISC       ,DATA_NORMAL | PTABLE_MOUNT_MTD                      ,0},
#endif
#if (0 != MBB_PTABLE_VXWORK_LEN)
    {"vxworks"      ,MBB_PTABLE_VXWORK_START    ,0x00000000  ,MBB_PTABLE_VXWORK_LEN    ,PTABLE_VXWORKS_CCORE_ENTRY ,PTABLE_VXWORKS_CCORE_ENTRY,IMAGE_CCORE      ,DATA_NORMAL | PTABLE_MOUNT_MTD                      ,0}, 
#endif
#if (0 != MBB_PTABLE_RESERVE1_LEN)
    {"wbdata"     ,MBB_PTABLE_RESERVE1_START  ,0x00000000  ,MBB_PTABLE_RESERVE1_LEN  ,0x00000000                 ,0x00000000                ,IMAGE_RESERVE1   ,DATA_NORMAL | PTABLE_MOUNT_MTD                      ,0},
#endif
#if (0 != MBB_PTABLE_RESERVE2_LEN)
    {"reserve2"     ,MBB_PTABLE_RESERVE2_START  ,0x00000000  ,MBB_PTABLE_RESERVE2_LEN  ,0x00000000                 ,0x00000000                ,IMAGE_RESERVE2   ,DATA_NORMAL | PTABLE_MOUNT_MTD                      ,0},
#endif
#if (0 != MBB_PTABLE_RESERVE3_LEN)
    {"reserve3"     ,MBB_PTABLE_RESERVE3_START  ,0x00000000  ,MBB_PTABLE_RESERVE3_LEN  ,0x00000000                 ,0x00000000                ,IMAGE_RESERVE3   ,DATA_YAFFS | PTABLE_MOUNT_MTD                       ,0},
#endif
#if (0 != MBB_PTABLE_OM_LEN)
    {"om"           ,MBB_PTABLE_OM_START        ,0x00000000  ,MBB_PTABLE_OM_LEN        ,0x00000000                 ,0x00000000                ,IMAGE_OM         ,DATA_YAFFS | PTABLE_MOUNT_MTD                       ,0},
#endif
#if (0 != MBB_PTABLE_WEBSDK_LEN)
#if (FEATURE_ON == MBB_DLOAD_JFFS2)
    {"app"          ,MBB_PTABLE_WEBSDK_START    ,0x00000000  ,MBB_PTABLE_WEBSDK_LEN    ,0x00000000                 ,0x00000000                ,IMAGE_WEBSDK     ,DATA_NORMAL | PTABLE_MOUNT_MTD                       ,0},
#else
    {"app"          ,MBB_PTABLE_WEBSDK_START    ,0x00000000  ,MBB_PTABLE_WEBSDK_LEN    ,0x00000000                 ,0x00000000                ,IMAGE_WEBSDK     ,DATA_YAFFS | PTABLE_MOUNT_MTD                       ,0},
#endif
#endif
#if (0 != MBB_PTABLE_WEBUI_LEN)
#if (FEATURE_ON == MBB_DLOAD_JFFS2)
/*webui partition will suport jffs2 filesystem*/
    {"webui"        ,MBB_PTABLE_WEBUI_START     ,0x00000000  ,MBB_PTABLE_WEBUI_LEN     ,0x00000000                 ,0x00000000                ,IMAGE_WEBUI      ,DATA_NORMAL | PTABLE_MOUNT_MTD                       ,0},
#else
    {"webui"        ,MBB_PTABLE_WEBUI_START     ,0x00000000  ,MBB_PTABLE_WEBUI_LEN     ,0x00000000                 ,0x00000000                ,IMAGE_WEBUI      ,DATA_YAFFS | PTABLE_MOUNT_MTD                       ,0},
#endif
#endif
#if (0 != MBB_PTABLE_SYSTEM_LEN)
#if (FEATURE_ON == MBB_DLOAD_JFFS2)
    {"system"       ,MBB_PTABLE_SYSTEM_START    ,0x00000000  ,MBB_PTABLE_SYSTEM_LEN    ,0x00000000                 ,0x00000000                ,IMAGE_SYSTEM     ,DATA_NORMAL | PTABLE_MOUNT_MTD                       ,0},
#else
    {"system"       ,MBB_PTABLE_SYSTEM_START    ,0x00000000  ,MBB_PTABLE_SYSTEM_LEN    ,0x00000000                 ,0x00000000                ,IMAGE_SYSTEM     ,DATA_YAFFS | PTABLE_MOUNT_MTD                       ,0},
#endif
#endif
/*change usrdata and online partitions sequence in stick and hilink product currently*/
#if (0 != MBB_PTABLE_USERDATA_LEN)
    {"userdata"     ,MBB_PTABLE_USERDATA_START  ,0x00000000  ,MBB_PTABLE_USERDATA_LEN  ,0x00000000                 ,0x00000000                ,IMAGE_USERDATA   ,DATA_YAFFS | PTABLE_MOUNT_MTD                       ,0},
#endif
#if (0 != MBB_PTABLE_BBOU_LEN)
    {"online"       ,MBB_PTABLE_BBOU_START      ,0x00000000  ,MBB_PTABLE_BBOU_LEN      ,0x00000000                 ,0x00000000                ,IMAGE_BBOU       ,DATA_YAFFS | PTABLE_MOUNT_MTD                       ,0},
#endif
#if (0 != MBB_PTABLE_MMC_LEN)
    {"cdromiso"     ,MBB_PTABLE_MMC_START       ,0x00000000  ,MBB_PTABLE_MMC_LEN       ,0x00000000                 ,0x00000000                ,IMAGE_MMC        ,DATA_YAFFS | PTABLE_MOUNT_MTD                       ,0},
#endif
#if (0 != MBB_PTABLE_CACHE_LEN)
    {"cache"        ,MBB_PTABLE_CACHE_START     ,0x00000000  ,MBB_PTABLE_CACHE_LEN     ,0x00000000                 ,0x00000000                ,IMAGE_CACHE      ,DATA_NORMAL | PTABLE_MOUNT_MTD                      ,0},
#endif 
#if (0 != MBB_PTABLE_RECVA_LEN)
    {"recovery-a"   ,MBB_PTABLE_RECVA_START     ,0x00000000  ,MBB_PTABLE_RECVA_LEN     ,0x00000000                 ,0x00000000                ,IMAGE_RECOVERY_A ,DATA_NORMAL | PTABLE_MOUNT_MTD                      ,0},
#endif
#if (0 != MBB_PTABLE_RECVB_LEN)
    {"recovery-b"   ,MBB_PTABLE_RECVB_START     ,0x00000000  ,MBB_PTABLE_RECVB_LEN     ,0x00000000                 ,0x00000000                ,IMAGE_RECOVERY_B ,DATA_NORMAL | PTABLE_MOUNT_MTD                      ,0},
#endif
    {PTABLE_END_STR ,0x00000000                 ,0x00000000  ,0x00000000               ,0x00000000                 ,0x00000000                ,IMAGE_PART_TOP   ,DATA_YAFFS                                          ,0}
};
#else
/*table partition begin*/
struct ST_PART_TBL ptable_product[PTABLE_PARTITION_MAX] =
{
    /*name*/        /*offset*/      /*loadsize*/    /*capacity*/   /*loadaddr*/   /*entry*/     /*type*/            /*property*/
#if (PTABLE_M3_BOOT_LEN != 0)
    {PTABLE_M3BOOT_NM       ,PTABLE_M3_BOOT_START   , 0x00000000,PTABLE_M3_BOOT_LEN,  0x00000000,  0x00000000   ,IMAGE_M3BOOT ,DATA_NORMAL | PTABLE_MOUNT_MTD ,   0},
#endif
#if (PTABLE_FASTBOOT_LEN != 0)
    {PTABLE_FASTBOOT_NM     ,PTABLE_FASTBOOT_START    , 0x00000000,PTABLE_FASTBOOT_LEN,  PTABLE_FASTBOOT_ENTRY,  PTABLE_FASTBOOT_ENTRY   ,IMAGE_FASTBOOT  ,DATA_NORMAL | PTABLE_MOUNT_MTD   ,   0},
#endif
#if (PTABLE_NV_LTE_LEN != 0)
    {PTABLE_NVBACK_NM    ,PTABLE_NV_LTE_START   , 0x00000000,PTABLE_NV_LTE_LEN,  0x00000000,  0x00000000   ,IMAGE_NVBACKLTE ,DATA_NORMAL | PTABLE_PROTECTED | PTABLE_MOUNT_MTD   ,   0},
#endif
#if (PTABLE_NVIMG_LEN != 0)
    {PTABLE_NVIMG_NM        ,PTABLE_NVIMG_START   , 0x00000000  ,PTABLE_NVIMG_LEN,  0x00000000,  0x00000000 ,IMAGE_NVIMG   ,DATA_NORMAL   | PTABLE_MOUNT_MTD  ,   0},
#endif
#if (PTABLE_NVDLD_LEN != 0)
    {PTABLE_NVDLOAD_NM      ,PTABLE_NVDLD_START   , 0x00000000   ,PTABLE_NVDLD_LEN,  0x00000000,  0x00000000   ,IMAGE_NVDLD   ,DATA_NORMAL | PTABLE_MOUNT_MTD     ,   0},
#endif
#if (PTABLE_NVFACTORY_LEN != 0)
    {PTABLE_NVDEFAULT_NM    ,PTABLE_NVFACTORY_START   , 0x00000000   ,PTABLE_NVFACTORY_LEN,   0x00000000,  0x00000000   ,IMAGE_NVFACTORY   ,DATA_NORMAL | PTABLE_MOUNT_MTD    ,   0},
#endif
#if (PTABLE_OEMINFO_LEN != 0)
    {PTABLE_OEMINFO_NM      ,PTABLE_OEMINFO_START   ,0x00000000 ,PTABLE_OEMINFO_LEN,   0x00000000,  0x00000000   ,IMAGE_OEMINFO    ,DATA_NORMAL | PTABLE_MOUNT_MTD,   0},
#endif
#if (PTABLE_YAFFS_USERDATA_LEN != 0)
    {PTABLE_USERDATA_NM     ,PTABLE_YAFFS_USERDATA_START   ,0x00000000   ,PTABLE_YAFFS_USERDATA_LEN,   0x00000000,  0x00000000   ,IMAGE_USERDATA    ,DATA_YAFFS | PTABLE_MOUNT_MTD       ,   0},
#endif
#if (PTABLE_YAFFS_ONLINE_LEN != 0)
    {PTABLE_ONLINE_NM       ,PTABLE_YAFFS_ONLINE_START   ,0x00000000   ,PTABLE_YAFFS_ONLINE_LEN,   0x00000000,  0x00000000   ,IMAGE_ONLINE, DATA_YAFFS | PTABLE_MOUNT_MTD      ,   0},
#endif
#if (PTABLE_BOOTIMAGE_LEN != 0)
    {PTABLE_KERNEL_NM       ,PTABLE_BOOTIMAGE_START ,0x00000000   ,PTABLE_BOOTIMAGE_LEN, (PTABLE_FASTBOOT_ENTRY + PRODUCT_CFG_FLASH_FASTBOOT_LEN), (PTABLE_FASTBOOT_ENTRY + PRODUCT_CFG_FLASH_FASTBOOT_LEN), IMAGE_KERNEL    ,DATA_NORMAL | PTABLE_MOUNT_MTD,   0},
#endif
#if (PTABLE_BOOTIMAGEBAK_LEN != 0)    
    {PTABLE_KERNELBK_NM       ,PTABLE_BOOTIMAGEBAK_START ,0x00000000   ,PTABLE_BOOTIMAGEBAK_LEN, (PTABLE_FASTBOOT_ENTRY + PRODUCT_CFG_FLASH_FASTBOOT_LEN), (PTABLE_FASTBOOT_ENTRY + PRODUCT_CFG_FLASH_FASTBOOT_LEN), IMAGE_KERNELBK    ,DATA_NORMAL | PTABLE_MOUNT_MTD,   0},
#endif
#if (PTABLE_LOGO_LEN != 0)
    {PTABLE_LOGO_NM       ,PTABLE_LOGO_START ,0x00000000   ,PTABLE_LOGO_LEN, 0x00000000, 0x00000000, IMAGE_LOGO, DATA_NORMAL | PTABLE_MOUNT_MTD,   0},
#endif
#if (PTABLE_M3IMAGE_LEN != 0)
    {PTABLE_M3IMG_NM      ,PTABLE_M3IMAGE_START   ,0x00000000   ,PTABLE_M3IMAGE_LEN,   0x00000000,  0x00000000   ,IMAGE_M3IMAGE    ,DATA_NORMAL | PTABLE_MOUNT_MTD       ,   0},
#endif
#if (PTABLE_DSP_LEN != 0)
    {PTABLE_DSP_NM          ,PTABLE_DSP_START ,0x00000000   ,PTABLE_DSP_LEN,   0x00000000,  0x00000000   ,IMAGE_DSP    ,DATA_NORMAL | PTABLE_MOUNT_MTD,   0},
#endif
#if (PTABLE_MISC_LEN != 0)
    {PTABLE_MISC_NM         ,PTABLE_MISC_START   ,0x00000000   ,PTABLE_MISC_LEN,   0x00000000,  0x00000000   ,IMAGE_MISC    ,DATA_NORMAL | PTABLE_MOUNT_MTD       ,   0},
#endif
#if (PTABLE_CCORE_LEN != 0)
    {PTABLE_VXWORKS_NM      ,PTABLE_CCORE_START   ,0x00000000   ,PTABLE_CCORE_LEN,   PTABLE_VXWORKS_CCORE_ENTRY,  PTABLE_VXWORKS_CCORE_ENTRY   ,IMAGE_CCORE     ,DATA_NORMAL | PTABLE_MOUNT_MTD                       ,   0}, 
#endif
#if (PTABLE_RESERVE1_LEN != 0)
    {PTABLE_RESERVE1_NM      ,PTABLE_RESERVE1_START   ,0x00000000   ,PTABLE_RESERVE1_LEN,   0x00000000,  0x00000000   ,IMAGE_RESERVE1     ,DATA_NORMAL | PTABLE_MOUNT_MTD                       ,   0}, 
#endif
#if (PTABLE_RESERVE2_LEN != 0)
    {PTABLE_RESERVE2_NM      ,PTABLE_RESERVE2_START   ,0x00000000   ,PTABLE_RESERVE2_LEN,   0x00000000,  0x00000000   ,IMAGE_RESERVE2     ,DATA_NORMAL | PTABLE_MOUNT_MTD                       ,   0}, 
#endif
#if (PTABLE_RESERVE3_LEN != 0)    
    {PTABLE_RESERVE3_NM      ,PTABLE_RESERVE3_START   ,0x00000000   ,PTABLE_RESERVE3_LEN,  0x00000000,  0x00000000   ,IMAGE_RESERVE3     ,DATA_YAFFS | PTABLE_MOUNT_MTD                       ,   0}, 
#endif
#if (PTABLE_OM_LEN != 0)    
    {PTABLE_OM_NM           ,PTABLE_OM_START   ,0x00000000   ,PTABLE_OM_LEN,   0x00000000,  0x00000000   ,IMAGE_OM    ,DATA_YAFFS | PTABLE_MOUNT_MTD       ,   0},
#endif
#if (PTABLE_YAFFS_APP_LEN != 0)    
    {PTABLE_APP_NM          ,PTABLE_YAFFS_APP_START  ,0x00000000   ,PTABLE_YAFFS_APP_LEN,   0x00000000,  0x00000000   ,IMAGE_APP    ,DATA_YAFFS |  PTABLE_MOUNT_MTD       ,   0},
#endif
#if (PTABLE_YAFFS_WEBUI_LEN != 0)
    {PTABLE_WEBUI_NM        ,PTABLE_YAFFS_WEBUI_START   ,0x00000000   ,PTABLE_YAFFS_WEBUI_LEN,   0x00000000,  0x00000000   ,IMAGE_WEBUI    ,DATA_YAFFS | PTABLE_MOUNT_MTD      ,   0},
#endif
#if (PTABLE_YAFFS_SYSIMG_LEN != 0)
    {PTABLE_SYSTEM_NM       ,PTABLE_YAFFS_SYSIMG_START   ,0x00000000   ,PTABLE_YAFFS_SYSIMG_LEN,   0x00000000,  0x00000000   ,IMAGE_SYSTEM    ,DATA_YAFFS | PTABLE_MOUNT_MTD       ,   0},
#endif
#if (PTABLE_CACHE_LEN != 0)
    {PTABLE_CACHE_NM        ,PTABLE_CACHE_START   ,0x00000000   ,PTABLE_CACHE_LEN,   0x00000000,  0x00000000   ,IMAGE_CACHE    ,DATA_NORMAL | PTABLE_MOUNT_MTD       ,   0},
#endif
#if (PTABLE_RECOVERY_A_LEN != 0)    
    {PTABLE_RECOVERYA_NM   ,PTABLE_RECOVERY_A_START   ,0x00000000   ,PTABLE_RECOVERY_A_LEN,   0x00000000,  0x00000000   ,IMAGE_RECOVERY_A     ,DATA_NORMAL | PTABLE_MOUNT_MTD    ,   0},
#endif
#if (PTABLE_RECOVERY_B_LEN != 0)
    {PTABLE_RECOVERYB_NM   ,PTABLE_RECOVERY_B_START   ,0x00000000   ,PTABLE_RECOVERY_B_LEN,   0x00000000,  0x00000000   ,IMAGE_RECOVERY_B     ,DATA_NORMAL | PTABLE_MOUNT_MTD     ,   0},
#endif
#if (PTABLE_ISO_LEN != 0)
    {PTABLE_CDROMISO_NM     ,PTABLE_ISO_START   ,0x00000000   ,PTABLE_ISO_LEN,   0x00000000,  0x00000000   ,IMAGE_CDROMISO    ,DATA_YAFFS | PTABLE_MOUNT_MTD       ,   0},
#endif
    {PTABLE_END_STR ,0x00000000   ,0x00000000   ,0x00000000,   0x00000000,  0x00000000   ,IMAGE_PART_TOP  ,DATA_YAFFS                        ,   0}
};
#endif /*MBB_DLOAD*/

struct ST_PTABLE_HEAD ptable_head =
{
	PTABLE_HEAD_STR,
	0x80000000,
	PTABLE_VER_ID_STR,
	PTABLE_VER_STR
};
//#pragma GCC pop_options

#ifdef __cplusplus
}
#endif

