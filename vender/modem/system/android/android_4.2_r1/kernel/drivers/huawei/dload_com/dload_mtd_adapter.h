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
#ifndef _DLOAD_MTD_ADAPTER_H
#define _DLOAD_MTD_ADAPTER_H
/******************************************************************************
  1 其他头文件包含
******************************************************************************/
#include <linux/types.h>
#include "dload_comm.h"

#ifdef  __cplusplus
    extern "C"{
#endif

/******************************************************************************
  2 模块对外 (宏、枚举、结构体、自定义数据类型) 定义区:
******************************************************************************/
#if (FEATURE_ON == MBB_DLOAD)

#define MAX_TRY_BLKS 128
#define MAX_PARTI_NAME_LEN 256
	
typedef enum
{
	MTD_UNKNOW_FS,
	MTD_NO_FS,	
	MTD_YAFFS2_FS,
	MTD_JIFFS2_FS,
	MTD_FAT32_FS,
	MTD_MAX_FS
}mtd_fs_type;

typedef struct 
{
	char parti_name[MAX_PARTI_NAME_LEN];
	mtd_fs_type parti_fs;
}mtd_parti_fs_type;

#define     FLASH_CODEWORD_SHIFT    (9)
#define     FLASH_CW_OOBSIZE_4096   (20)
#define     FLASH_CW_OOBSIZE_2048   (16)

/* YAFFS2 used oob size*/
#define     YAFFS2_OOBSIZE_USED     (16)

enum flash_page_size
{
    FLASH_PAGE_SIZE_512     = 512,
    FLASH_PAGE_SIZE_2048    = 2048,
    FLASH_PAGE_SIZE_4096    = 4096,
    FLASH_PAGE_SIZE_8192    = 8192
};

int mtdchar_read_dload(struct mtd_info *mtd, loff_t ppos, char *buf,  size_t count, ssize_t *total_retlen, loff_t *ppos_realy);
int mtdchar_write_dload(struct mtd_info *mtd, loff_t ppos, char *buf, size_t count, ssize_t *total_retlen, loff_t *ppos_realy);
mtd_fs_type check_mtdchar_fs_part(const char *part_name);
void dload_set_mtd_info(struct mtd_info *mtd);
int8 dload_flush_partion(void);

int fixed_fs_partitions(struct mtd_info *master, void *partion);
#endif

#ifdef __cplusplus
}
#endif
#endif /* _DLOAD_MTD_ADAPTER_H */
