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
#include <linux/kernel.h>
#include <linux/ctype.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/mutex.h>
#include <linux/backing-dev.h>
#include <linux/compat.h>
#include <linux/mount.h>
#include <linux/blkpg.h>
#include <linux/magic.h>
#include <linux/mtd/mtd.h>
#include <mtd/mtd-abi.h>
#include <linux/mtd/partitions.h>
#include <linux/mtd/map.h>
#include <linux/mtd/partitions.h>
#include <ptable_com.h>
#include "mtdcore.h"
#include "dload_mtd_adapter.h"
#include <linux/syscalls.h>
#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************
  1 模块私有 (宏、枚举、结构体、自定义数据类型) 定义区:
******************************************************************************/
#define DLOAD_PRINT(level, fmt, ...) (printk(level"[*DLOAD_MTD*] %s:%d: "fmt"\n", __func__, __LINE__, ##__VA_ARGS__))

#if (FEATURE_ON == MBB_DLOAD)

#if (FEATURE_ON == MBB_COMMON)
extern struct ST_PART_TBL * ptable_get_ram_data(void); 
#endif

STATIC struct mtd_info*		dload_mtd_info = (struct mtd_info*)NULL;

#define     FLASH_USER_OBB_SIZE_32              (32)
#define     FLASH_USER_OBB_SIZE_16              (16)
#define     DLOAD_PTABLE_WRITED_MAGIC   (0X5A5A5A5A)

struct mtdpart_flush
{
    int     ptable_flushed;
    char    sys_ptable[4096];
};


struct mtdpart_flush    g_mtdpart_flush;
static DEFINE_MUTEX(mtdpart_flush_mutex);

mtd_parti_fs_type g_huawei_mtd_fs_type []=
{
#if (FEATURE_ON == MBB_DLOAD_JFFS2)
#if (FEATURE_ON == MBB_DLOAD_HILINK)
    {"system",   MTD_YAFFS2_FS},
    {"webui",    MTD_JIFFS2_FS},
    {"app",      MTD_YAFFS2_FS},
#else
    {"system",   MTD_JIFFS2_FS},
    {"webui",    MTD_JIFFS2_FS},
    {"app",      MTD_JIFFS2_FS},
#endif /*MBB_DLOAD_HILINK*/
#else
    {"system",   MTD_YAFFS2_FS},
    {"webui",    MTD_YAFFS2_FS},
    {"app",      MTD_YAFFS2_FS},
#endif
    {"userdata", MTD_YAFFS2_FS},
    {"online",   MTD_YAFFS2_FS},
    {"om",       MTD_YAFFS2_FS},
    {"cdromiso", MTD_YAFFS2_FS},
    {"cache",    MTD_YAFFS2_FS},
    {"reserve3", MTD_YAFFS2_FS},
    {"",         MTD_UNKNOW_FS}
};
/*
 * This function to detect whether the m3boot partition
 * 
 * param
 *   mtd: partition mtd  
 */
static int mtdchar_ism3boot(struct mtd_info *mtd)
{
    if (!strcmp(mtd->name, "m3boot"))
    {
        return 1;
    }

    return 0;
}

/*
 * This function write m3boot partition, 
 * 
 * param
 *   mtd: partition mtd  
 *   ppos: write position
 *   buf: user ptable source buffer
 *   count: length
 */
static int mtdchar_write_m3bootpart(struct mtd_info *mtd, loff_t ppos, char *buf,
                                    size_t count)
{
    char    *buf_total = NULL;
    int     total_buf_len = 0;
    struct mtd_oob_ops ops;

    if (NULL == mtd || NULL == buf)
    {
        pr_err("mtd_read param error\n");
        return - EINVAL;
    }

    if ( ppos + count > mtd->size )
    {
        pr_err("no space ppos = %d count = %d  mtd_size = %d \n", (int)ppos,count, (int)mtd->size);
        return - ENOSPC;
    }


    if (count > mtd->erasesize)    
    {
        pr_err("usr partition table size too long\n");
        return - EINVAL;       
    }

    total_buf_len = mtd->erasesize;
    buf_total = kmalloc(total_buf_len, GFP_KERNEL);
    if (NULL == buf_total)
    {
        pr_err("get memory error\n");
        return - ENOMEM;
    }
    
    memset(buf_total, 0XFF, total_buf_len); /*0XFF μ?Flash ì?3???*/
    memcpy(buf_total, buf, count);
    
    ops.mode = MTD_OPS_AUTO_OOB;
    ops.datbuf = buf_total;
    ops.len = mtd->erasesize;
    ops.ooboffs = 0;
    ops.ooblen = 0;
    ops.oobbuf = NULL;

    if (0 != mtd_write_oob(mtd, ppos, &ops))
    {
        pr_err("mtd_write_oob error\n");
        kfree(buf_total);
        return - EINVAL;
    }
    
    /* backup sys ptable, will be used when flush mtd partitions
     */
    mutex_lock(&mtdpart_flush_mutex);
    memcpy(g_mtdpart_flush.sys_ptable, (char *)buf_total + FLASH_PTABLE_OFFSET, mtd->writesize);
    g_mtdpart_flush.ptable_flushed = DLOAD_PTABLE_WRITED_MAGIC;
    mutex_unlock(&mtdpart_flush_mutex);

    kfree(buf_total);
    return 0;
}

/*
 * This function flush all mtd partition with new ptable, 
 * 
 * param
 *   mtd: master mtd  
 *
 */
int mtdchar_flush_part(struct mtd_info *mtd)
{
    struct mtd_partition *real_parts = NULL;
    int i = 0;
    int ret = 0;
    char    *pname = NULL;
    int  ptable_count = 0;
    struct ST_PART_TBL * ptable_new = NULL;
#if (FEATURE_ON == MBB_COMMON)   
    struct ST_PART_TBL *ptable_old = NULL; 
    unsigned int  chipsize = 0;
#endif 
    mutex_lock(&mtdpart_flush_mutex);
    
    /* check new ptabel has been write to flash
     */
    if(DLOAD_PTABLE_WRITED_MAGIC == g_mtdpart_flush.ptable_flushed)
    {
        /*skip head*/
        ptable_new = (struct ST_PART_TBL *)(g_mtdpart_flush.sys_ptable + PTABLE_HEAD_SIZE);

        while(0 != strcmp(PTABLE_END_STR, ptable_new->name))
        {
           ptable_count++;
           ptable_new++;
        }
#if (FEATURE_ON == MBB_COMMON)
        /*FLASH  last partition adapt*/
        ptable_new--;
        printk("before last partition capacity = 0x%x\n", ptable_new->capacity);
        ptable_old = ptable_get_ram_data();
        while(0 != strcmp(PTABLE_END_STR, ptable_old->name))
        {
            ptable_old++;
        }
        ptable_old--;
        chipsize = ptable_old->offset + ptable_old->capacity;
        printk("chipsize = 0x%x\n", chipsize );
        if(chipsize >= ptable_new->offset)
        {
            ptable_new->capacity = chipsize - ptable_new->offset;
        }
        printk("after last partition capacity = 0x%x\n", ptable_new->capacity);
        ptable_new++;
#endif

        real_parts = (struct mtd_partition *)kcalloc(ptable_count, 
                        sizeof(struct mtd_partition), GFP_KERNEL);
        if(NULL == real_parts)
        {
            pr_err("mtd_partition malloc error\n");
            ret = - ENOMEM;
            goto out;
        }
        
        /* construct new ptable
         */
        for (i = ptable_count; i > 0 ; i--)
        {
            ptable_new--;
            
            if ('\0' == ptable_new->name)
            {
                continue;
            }

            pname = ptable_new->name;
            
            
            if (pname)
            {
                real_parts[i - 1].name = pname;
            }
            else
            {
                pr_err("mtd_partition fatmat error\n");
                ret = - EINVAL;
                goto free_mem;
            }
            
            real_parts[i - 1].offset = ptable_new->offset;
            real_parts[i - 1].size = ptable_new->capacity;
            pname = NULL;
        }
        /* 
         * unregister all partition, add all partition in new ptable
         */
        ret = mtd_device_unregister(mtd);
        if (0 != ret)
        {
            pr_err("unregister mtd partitions error\n");
            ret = - EINVAL;
            goto free_mem;
        }

        ret = add_mtd_partitions(mtd, real_parts, ptable_count);
        if (0 != ret)
        {
            pr_err("add mtd partitions error\n");
            ret = - EINVAL;
            goto free_mem;
        }
    }
    else
    {
        pr_err("please write ptable first\n");
    }

free_mem:
    kfree(real_parts);
    
out:
    memset(&g_mtdpart_flush, 0, sizeof(struct mtdpart_flush));
    mutex_unlock(&mtdpart_flush_mutex);

    return ret;
}

/*******************************************************************
  函数名称  : enum mtd_fs_type check_mtdchar_fs_part(char *part_name)
  函数描述  : 
  函数入参  : 
  函数输出  : NA
  函数返回值: 
********************************************************************/
mtd_fs_type check_mtdchar_fs_part(const char *part_name)
{
    int i = 0;

    while(1)
    {
        if(!strncmp(g_huawei_mtd_fs_type[i].parti_name, "", strlen("") + 1))
        {
            break;
        }
        
        if(!strncmp(part_name, g_huawei_mtd_fs_type[i].parti_name,
                strlen(g_huawei_mtd_fs_type[i].parti_name)))
        {
            return g_huawei_mtd_fs_type[i].parti_fs;
        }
        i++;
    }

    return MTD_NO_FS;
}

/*
 * This function read mtd partition, 
 * 
 * param
 *   mtd: partition mtd  
 *   ppos: read position
 *   buf: dest buf
 *   count: read length
 *   total_retlen: realy read length
 *   ppos_realy: realy position move
 *
 */
int mtdchar_read_dload(struct mtd_info *mtd, loff_t ppos, char *buf, 
                        size_t count, ssize_t *total_retlen, loff_t *ppos_realy)
{
    ssize_t  read_size = 0;
    struct mtd_oob_ops ops = {0};
    ssize_t  index = 0;
    ssize_t  retlen = 0;
    mtd_fs_type  isyaffspart = MTD_UNKNOW_FS;
    int     ret = 0;
    int     ecc_error = 0;
    int     sparesize = 0;
    int     blocksparesize = 0;

    if (NULL == mtd || NULL == buf)
    {
        DLOAD_PRINT(KERN_ERR, "mtd_read param error\n");
        return - EINVAL;
    }

    if (ppos >= mtd->size)
    {
        return - ENOSPC;
    }


    sparesize = FLASH_USER_OBB_SIZE_16;
    
    blocksparesize = (mtd->erasesize / mtd->writesize) * sparesize;
    isyaffspart = check_mtdchar_fs_part(mtd->name);
    if (MTD_YAFFS2_FS == isyaffspart)
    {
        if (ppos + count > mtd->size + blocksparesize)
        {
            DLOAD_PRINT(KERN_ERR, "read count over\n");
            return - EINVAL;
        }
    }
    else
    {
        if (ppos + count > mtd->size)
        {
            DLOAD_PRINT(KERN_ERR, "read count over\n");
            return - EINVAL;
        }
    }

    if (!count)
    {
        return 0;
    }


    if (MTD_YAFFS2_FS == isyaffspart)
    {
        if (count % (mtd->writesize + sparesize) != 0)
        {
            DLOAD_PRINT(KERN_ERR, "mtd_read len error\n");
            return - EINVAL;
        }
    }
    else
    {
        if (count % (mtd->writesize) != 0)
        {
            DLOAD_PRINT(KERN_ERR, "mtd_read len error\n");
            return - EINVAL;
        }
    }

    ops.mode = MTD_OPS_AUTO_OOB;
    read_size = count;

    while (read_size > 0)
    {
        ops.datbuf = buf + retlen;
        ops.len = mtd->writesize;
        if (MTD_YAFFS2_FS == isyaffspart)
        {
            ops.oobbuf = buf + retlen + mtd->writesize;
            ops.ooboffs = 0;
            ops.ooblen = YAFFS2_OOBSIZE_USED;
            memset(ops.oobbuf, 0xFF, sparesize);
        }
        else
        {
            ops.oobbuf = NULL;
            ops.ooboffs = 0;
            ops.ooblen = 0;
        }

        ret = mtd_read_oob(mtd, ppos + index, &ops);
        if (ret != 0 && !mtd_is_bitflip_or_eccerr(ret))
        {
            DLOAD_PRINT(KERN_ERR, "mtd_read_oob error\n");
            break;
        }
        else if (ret != 0 && mtd_is_eccerr(ret)) //ecc error but continue
        {
            ecc_error = ret;
        }
        else //(ret == 0) or (bitflip but corrected) is ok
        {
            ret = 0;
        }

        if (MTD_YAFFS2_FS == isyaffspart)
        {
            retlen += (ops.retlen + sparesize);
            index += ops.retlen;
            read_size -= (ops.retlen + sparesize);
        }
        else
        {
            retlen += ops.retlen;
            index += ops.retlen;
            read_size -= ops.retlen;
        }
    }

    *total_retlen = retlen;
    *ppos_realy = index;

    if (ecc_error != 0)
    {
        ret = ecc_error;
    }

    return ret;
}

/*
 * This function write mtd partition, 
 * 
 * param
 *   mtd: partition mtd  
 *   ppos: write position
 *   buf: source buf
 *   count: write length
 *   total_retlen: realy write length
 *   ppos_realy: realy position move
 *
 */
int mtdchar_write_dload(struct mtd_info *mtd, loff_t ppos, char *buf, 
                        size_t count, ssize_t *total_retlen, loff_t *ppos_realy)
{
    ssize_t  write_size = 0;
    struct mtd_oob_ops ops = {0};
    ssize_t  index = 0;
    ssize_t  retlen = 0;
    mtd_fs_type isyaffspart = MTD_UNKNOW_FS;
    int     ret = 0;
    int     sparesize = 0;

    int     blocksparesize = 0;

    unsigned char   oob[FLASH_USER_OBB_SIZE_32] = {0};
    unsigned char   oobsize = 0;

    if (NULL == mtd || NULL == buf)
    {
        DLOAD_PRINT(KERN_ERR, "mtd_write param error\n");
        return - EINVAL;
    }

    if (ppos >= mtd->size)
    {
        return - ENOSPC;
    }

    sparesize = FLASH_USER_OBB_SIZE_16;
  
    oobsize = FLASH_USER_OBB_SIZE_16;


    blocksparesize = (mtd->erasesize / mtd->writesize) * sparesize;

    isyaffspart = check_mtdchar_fs_part(mtd->name);
    if (MTD_YAFFS2_FS == isyaffspart)
    {
        if (ppos + count > mtd->size + blocksparesize)
        {
            DLOAD_PRINT(KERN_ERR, "write count over\n");
            return - EINVAL;
        }
    }
    else
    {
        if (ppos + count > mtd->size)
        {
            DLOAD_PRINT(KERN_ERR, "write count over\n");
            return - EINVAL;
        }
    }

    if (!count)
    {
        return 0;
    }

    /* write m3boot partition*/
    if (mtdchar_ism3boot(mtd))
    {
        ret = mtdchar_write_m3bootpart(mtd, ppos, buf, count);        
        if (0 != ret)
        {
            DLOAD_PRINT(KERN_ERR, "mtdchar write m3boot error\n");
            return - EINVAL;
        }

        *total_retlen = count;
        *ppos_realy = count;

        return 0;
    }

    if (MTD_YAFFS2_FS == isyaffspart)
    {
        if (count % (mtd->writesize + sparesize) != 0)
        {
            DLOAD_PRINT(KERN_ERR, "mtd_write len error\n");
            return - EINVAL;
        }
    }
    else
    {
        if (count % (mtd->writesize) != 0)
        {
            DLOAD_PRINT(KERN_ERR, "mtd_write len error\n");
            return - EINVAL;
        }
    }

    ops.mode = MTD_OPS_AUTO_OOB;
    write_size = count;

    memset(oob, 0XFF, FLASH_USER_OBB_SIZE_32);

    while (write_size > 0)
    {
        ops.datbuf = buf + retlen;
        ops.len = mtd->writesize;
        if (MTD_YAFFS2_FS == isyaffspart)
        {
            ops.oobbuf = buf + retlen + mtd->writesize;
            ops.ooboffs = 0;
            ops.ooblen = oobsize;
        }
        else
        {
            ops.oobbuf = (uint8_t *)oob;
            ops.ooboffs = 0;
            ops.ooblen = oobsize;
        }
 
        if (0 != mtd_write_oob(mtd, ppos + index, &ops))
        {
            DLOAD_PRINT(KERN_ERR, "mtd_write_oob error\n");
            return - EINVAL;
        }

        if (MTD_YAFFS2_FS == isyaffspart)
        {
            retlen += (ops.retlen + sparesize);
            index += ops.retlen;
            write_size -= (ops.retlen + sparesize);
        }
        else
        {
            retlen += ops.retlen;
            index += ops.retlen;
            write_size -= ops.retlen;
        }
    }

    *total_retlen = retlen;
    *ppos_realy = index;

    return 0;
}


/*******************************************************************
  函数名称  : void dload_set_mtd_info(struct mtd_info *mtd)
  函数描述  : 引入MTD INFO信息
  函数入参  : 
  函数输出  : NA
  函数返回值: 
********************************************************************/
void dload_set_mtd_info(struct mtd_info *mtd)
{
    dload_mtd_info = mtd;
}

/*******************************************************************
  函数名称  : int8 dload_flush_partion(void)
  函数描述  : 刷新MTD分区
  函数入参  : 
  函数输出  : NA
  函数返回值: 
********************************************************************/
int8 dload_flush_partion(void)
{
    int8 ret = DLOAD_OK;
    
    if(NULL == dload_mtd_info)
    {
        DLOAD_PRINT(KERN_ERR, "dload_mtd_info is NULL \n");
        return DLOAD_ERR;
    }

    ret = mtdchar_flush_part(dload_mtd_info);
    if(0 > ret)
    {
        DLOAD_PRINT(KERN_ERR, "mtdchar_flush_part ret = %d error.", ret);
        return DLOAD_ERR;
    }

    return DLOAD_OK;
}

/* Our partition node structure */
struct mtd_part {
	struct mtd_info mtd;
	struct mtd_info *master;
	uint64_t offset;
	struct list_head list;
};

/*******************************************************************
  函数名称  : enum mtd_fs_type huawei_scan_mtd_parti_type(const char *parti_name)
  函数描述  : 
  函数入参  : 
  函数输出  : NA
  函数返回值: 
********************************************************************/
mtd_fs_type huawei_scan_mtd_parti_type(const char *parti_name)
{
    int i = 0;
    int parti_name_len = -1;
    
    if ( NULL == parti_name )
    {
        DLOAD_PRINT(KERN_ERR, "%s : parti name is NULL \n",__FUNCTION__);
        goto scan_fail;
    }

    parti_name_len = strlen(parti_name);
    
    if ((parti_name_len > MAX_PARTI_NAME_LEN - 1) ||
        (-1 == parti_name_len ))
    {
        DLOAD_PRINT(KERN_ERR, "%s:parti name is over len \n",__FUNCTION__);
        goto scan_fail;
    }
    
    for (i = 0; i < ARRAY_SIZE(g_huawei_mtd_fs_type) ; i++)
    {
        if ( 0 != g_huawei_mtd_fs_type[i].parti_name [0])
        {
            if (0 == strncmp(g_huawei_mtd_fs_type[i].parti_name,\
                parti_name,strlen(g_huawei_mtd_fs_type[i].parti_name)))
            {
                return (g_huawei_mtd_fs_type[i].parti_fs);
            }
        }
        else
        {
            goto scan_fail;
        }
    }
scan_fail:    
    return MTD_UNKNOW_FS;
}

/*******************************************************************
  函数名称  : int fixed_fs_partitions(struct mtd_info *master, struct mtd_part *part)
  函数描述  : 
  函数入参  : 
  函数输出  : NA
  函数返回值: 
********************************************************************/
int fixed_fs_partitions(struct mtd_info *master, void *partion)
{
    unsigned int      pos     = 0;
    struct mtd_part  *part    = NULL;
    mtd_fs_type      fs_type  = MTD_UNKNOW_FS;

    part = (struct mtd_part *)partion;
    
    if( (NULL == part)  || (NULL == part->mtd.name))
    {
        DLOAD_PRINT(KERN_ERR, "fixed_mtd_partitions , mtd info is not exist!\n");
        return -1;
    }

    /*For raw and  fs partiton, find the first good block  */
    fs_type = huawei_scan_mtd_parti_type(part->mtd.name);
    if ( MTD_YAFFS2_FS == fs_type || MTD_JIFFS2_FS == fs_type )
    {
        while(pos < (MAX_TRY_BLKS * part->mtd.erasesize))
        {
            if(((pos % part->mtd.erasesize) == 0)
            && (master->_block_isbad(master, pos + part->offset)))
            {
                pos += part->mtd.erasesize;
                continue;
            }

            part->offset =  part->offset + pos;
            part->mtd.size = part->mtd.size - pos;
            DLOAD_PRINT(KERN_DEBUG, "Partition %s position fixed, Offset:%llx Size:0x%llx\n",
            part->mtd.name, part->offset, part->mtd.size);
            return 0;
        }
    }
    return -1;
}
#endif


#ifdef __cplusplus
}
#endif
