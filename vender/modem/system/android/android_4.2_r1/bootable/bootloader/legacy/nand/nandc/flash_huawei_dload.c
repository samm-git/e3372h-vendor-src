/******************************************************************************************************
                                                                            
                Copyright (C), 2010, Huawei Tech. Co., Ltd.                
                            ALL RIGHTS RESERVED                             
                                                                            
------------------------------------------------------------------------------
 
 
FileName     : flash_huawei_dload.c
Version      : 1.0
Date         : 2013-04-19
Description  : This file Security Upgrade Solution Interface Fuctions
Function List: 
 
          History:
                    <author>   <date>          <desc>
----------------------------------------------------------------------------
****************************************************************************************************/
/*************************************问题单修改记录**********************************************
日    期       修改人         问题单号                   修改内容
************************************************************************************************/
/*---------------------------------------------------------------------------
  Include file define region
---------------------------------------------------------------------------*/
#include "flash_huawei_dload.h"
#if (FEATURE_ON == MBB_DLOAD)
#include "ptable_com.h"
#include "ptable_inc.h"
#include "nandc_balong.h"
/*---------------------------------------------------------------------------
  Macro define region
---------------------------------------------------------------------------*/
#define  MBB_DLOAD_OEMINFO_PARTI_NAME  "oeminfo"
#define HUAWEI_DLOAD_XML_MAGIC_NUM 0x454D5045
#define HUAWEI_RESTORE_XML_MAGIC_NUM 0x45504D45

#define     TRUE                  1
#define     FALSE                 0
/*---------------------------------------------------------------------------
  Cust type define region
---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------
  Global Variable define region
---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------
  Local Function decleare region
---------------------------------------------------------------------------*/

oper_region_struct_t   operation_region[REGION_MAX];
uint8 iso_header_buffer[MAX_PAGE_SIZE];

/*---------------------------------------------------------------------------
  Global table define region
---------------------------------------------------------------------------*/
child_region_t  child_region[] =
{
    /* SHARE子分区参数 */
    {MBB_DLOAD_OEMINFO_PARTI_NAME, MAGIC_DEFAULT,MAGIC_DEFAULT, \
     0, 4, NULL, 0},  /*0：起始block  4 总block 数*/
};


/*===========================================================================

                   FUNCTION DECLARATIONS

===========================================================================*/

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
=========================================================================== */

boolean flash_get_share_region_info(rgn_pos_e_type region_type,void* data,uint32 data_len)
{
    uint32    page_index = 0;
    uint32    blk_size = 0;
    uint32    blk_count = 0;
    uint32 blk_index = INVALID_BLOCK_ID;
    rgn_hd_type* rgn_hd;
    uint32   offset = 0;
    uint32   start_offset = 0;
    uint32   page_size = 0;
    uint8 *   read_buf = NULL;
    uint32   is_bad = 0;
    
    if(NULL == flash_nand_oper_region_init(SHARE_REGION))
    {
        cprintf("flash_get_share_region_info init fail  \n");
        return FALSE;
    }
    
    /*Get Partition and Device Info*/
    blk_count = child_region[SHARE_REGION].length;
    blk_size = operation_region[SHARE_REGION].block_size;
    start_offset = operation_region[SHARE_REGION].start_addr;
    page_size = operation_region[SHARE_REGION].page_size;
    read_buf = operation_region[SHARE_REGION].buffer;

    for (blk_index = 0; blk_index < blk_count; blk_index++)
    {
        offset = start_offset + blk_size * blk_index;
        /*If the blk if bad blk or it is erase the*/
        if (!nand_isbad((offset / blk_size),(unsigned int*)&is_bad))
        {
            if(is_bad)
            {
                cprintf("flash_get_share_region_info Block ID = %x is bad block \n",(offset / blk_size));
                continue;
            }
        }
        
        /*If current blk is not erased, then read the first page and check its magic*/
        /*lint -e64*/
        if(!nand_read(offset, (uint32)read_buf, page_size, NULL)) /*lint !e119 */
        /*lint +e64*/
        {
           /*Firstly we need to check the Share Region Info,If it was a valid block
            then read corresponding page*/
            rgn_hd = (rgn_hd_type*)read_buf;
            if(SHARE_RGN_MAGIC == rgn_hd->magic)
            {
                /*Read Spec Region Flag Info*/
                page_index += region_type;
                offset = offset + page_index * page_size;

                memset((void *)read_buf, NAND_FILL_CHAR_APP, page_size);
                /*lint -e64*/
                if(!nand_read(offset, (uint32)read_buf ,page_size, NULL)) /*lint !e119*/
                /*lint +e64*/
                {
                    memcpy(data,read_buf,data_len);
                    flash_nand_oper_region_close(SHARE_REGION);
                    return TRUE;
                }  
                cprintf("flash_get_share_region_info fail  \n");
            }
        }
    }
    cprintf("flash_get_share_region_info error  \n");
    flash_nand_oper_region_close(SHARE_REGION);
    return FALSE;
}

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
boolean flash_update_share_region_info(rgn_pos_e_type region_type,void* data)
{
    uint32    block_index = 0;
    uint32    start_index = 0;
    uint32    blk_size = 0;
    uint32    blk_count = 0;
    uint32    max_rgn_num = 0;
    uint32    dest_block = 0;
    uint32    current_block = INVALID_BLOCK_ID;
    uint32    current_page = INVALID_PAGE_ID;
    uint32    dest_page = INVALID_PAGE_ID;
    rgn_hd_type* rgn_hd = NULL;
    uint32   offset = 0;
    uint32   page_size = 0;
    uint8 *   cache_buf = NULL;
    uint32   is_bad = 0;
    uint32   start_offset = 0;

    /*入参条件检测*/
    if (region_type >= RGN_MAX_NUM || NULL == data)
    {
        cprintf("Input parm error \n");
        return FALSE;
    }

    if(NULL == flash_nand_oper_region_init(SHARE_REGION))
    {
        cprintf("flash_update_share_region_info init error \n");
        return FALSE;
    }

    /*Get Partition and Device Info*/
    blk_count = child_region[SHARE_REGION].length;
    blk_size = operation_region[SHARE_REGION].block_size;
    start_offset = operation_region[SHARE_REGION].start_addr;
    page_size = operation_region[SHARE_REGION].page_size;
    cache_buf = operation_region[SHARE_REGION].buffer;
 
    for (block_index = 0; block_index < blk_count; block_index++)
    {
        offset = start_offset + blk_size * block_index;
        /*If the blk if bad blk or it is erase the*/
        if (!nand_isbad((offset / blk_size), (unsigned int*)&is_bad))
        {
            if(1 == is_bad)
            {
                cprintf("Find Current Block ID = %x is bad block \n",(offset / blk_size));
                continue;
            }
        }
        /*If the blk if bad blk or it is erase the*/
        /*lint -e64*/
        if(!nand_read(offset,(uint32)cache_buf ,page_size,NULL)) /*lint !e119*/
        /*lint +e64*/
        {
            /*Firstly we need to check the Share Region Info,If it was a valid block
            then read corresponding page*/
            rgn_hd = (rgn_hd_type*)cache_buf;
            
            if(SHARE_RGN_MAGIC == rgn_hd->magic)
            {
                current_page = block_index * blk_size;
                max_rgn_num = rgn_hd->sub_rgn_num;
                current_block = block_index;
                cprintf("Print entery current page = %x  max_rgn_num = %x  \n",current_page,max_rgn_num);
                break;
            }  
            else
            {
                if (nand_erase(offset / blk_size))
                {
                    cprintf("Current Erase error ,Mark bad block \n");
                    (void)nand_bad(offset / blk_size);
                }
                cprintf("Print nand_erase offset = %x  \n",offset);
                continue;
            }
        }
        else
        {
            cprintf("Nand_read  error  !\n");
            return FALSE;
        }
    }


    cprintf("Print first current page = %d  \n",current_page);

    if(block_index != blk_count)
    {
        /*从 当前block 的 下一个block 分区开始寻找下一个 没使用的BLOCK*/
        dest_block = (block_index + 1) % blk_count;
    }

    /*Find the first unused block*/
    for(block_index  = 0; block_index  < blk_count; block_index++)
    {
        offset = start_offset + dest_block * blk_size;
        if (!nand_isbad((offset / blk_size),(unsigned int*)&is_bad))
        {
            if(1 == is_bad)
            {
                cprintf("Find Dest Block ID =%x is bad block \n",(offset / blk_size));
                continue;
            }
            else
            {
                if(current_block != dest_block)
                {
                    if (nand_erase(offset / blk_size))
                    {
                        (void)nand_bad(offset / blk_size);
                        dest_block = (dest_block + 1) % blk_count;
                        cprintf("Dest Erase error ,Mark bad block \n");
                        continue;
                    }
                    dest_page = dest_block * blk_size;
                    break;
                }
            }
        }
        dest_block = (dest_block + 1) % blk_count;
    }

    /*将当前页的数据和更新的数据分别取出写入目标BLOCK*/
    if (INVALID_PAGE_ID != dest_page)
    {   
        start_index = 0;
        if((uint32)region_type >= max_rgn_num )//第一次升级SHARE RGN分区  或者RGN_MAX_NUM有变化
        {
            offset = start_offset;
            memset(cache_buf,0xFF, page_size);  /*flash 操作buf 清为0XFF*/
            rgn_hd_type rgn_hd_1;
            rgn_hd_1.magic = SHARE_RGN_MAGIC;
            rgn_hd_1.sub_rgn_num = RGN_MAX_NUM;
            memcpy(cache_buf,(void *)&rgn_hd_1,sizeof(rgn_hd_type));
            cprintf("Print First Dest page = %d  \n",dest_page);
            /*
            rgn_hd->magic = SHARE_RGN_MAGIC;
            rgn_hd->sub_rgn_num = RGN_MAX_NUM;
            memcpy(cache_buf,(void *)rgn_hd,sizeof(rgn_hd));*/

            /*擦除当前BLOCK*/
            if((INVALID_BLOCK_ID == current_block) || (0 < max_rgn_num))
            {
                offset = offset + dest_page;
            }
            else
            {
                offset = offset + current_page;
            }

            /*lint -e64*/
            if (nand_write(offset,(uint32)cache_buf,page_size,NULL)) /*lint !e119*/
            /*lint +e64*/
            {
                cprintf("nand_write rgn_hd error  \n");
                goto FalseQuit;
            }

            memset(cache_buf,0xFF, page_size); /*flash 操作buf 清为0XFF*/
            memcpy(cache_buf,data,page_size);
            /*lint -e64*/
            if (nand_write((offset  + region_type * page_size),(uint32)cache_buf,page_size,NULL)) /*lint !e119*/
            /*lint +e64*/
            {
                cprintf("nand_write region page error  \n");
                goto FalseQuit;
            }
            start_index = 1;
        }

        
        if(INVALID_PAGE_ID != current_page && dest_page != current_page)
        {  
            offset = start_offset;
            cprintf("Print not First Dest page = %d  \n",dest_page);
            cprintf("Print last current page = %d  \n",current_page);
            for(; start_index < max_rgn_num; start_index++)            
            {
                if(start_index != region_type)
                {
                    memset(cache_buf,0xFF, page_size); /*flash 操作buf 清为0XFF*/
                    /*lint -e64*/
                    if(!nand_read((offset + current_page + start_index * page_size),(uint32)cache_buf ,page_size,NULL)) /*lint !e119*/
                    /*lint +e64*/
                    {
                        if(0 == start_index)
                        {
                            rgn_hd_type *temp_hd = NULL;
                            temp_hd = (rgn_hd_type *)cache_buf;
                            cprintf("magic = %x   max_num = %x \n",temp_hd->magic,temp_hd->sub_rgn_num);  
                            
                        }
                        /*lint -e64*/
                        if (!nand_write((offset + dest_page + start_index * page_size),(uint32)cache_buf,page_size,NULL)) /*lint !e119*/
                        /*lint -e64*/
                        {
                            cprintf("nand_write start_index = %d  offset = %x\n",
                                                    start_index,(offset + dest_page + region_type * page_size));
                            continue;
                        }
                    }
                }
                else
                {
                    memset(cache_buf,0xFF, page_size); /*flash 操作buf 清为0XFF*/
                    memcpy(cache_buf,data,page_size);
                    /*lint -e64*/
                    if (nand_write((offset + dest_page + region_type * page_size),(uint32)cache_buf,page_size,NULL)) /*lint !e119*/
                    /*lint -e64*/
                    {
                        cprintf("nand_write error \n");
                        goto FalseQuit;
                    }
                   cprintf("Write offset = %x ! \n",(offset + dest_page + region_type * page_size)); 
                }
            }

            if (!nand_erase((offset + current_page) / blk_size))
            {
                cprintf("nand_erase offset = %x ! \n",(offset + current_page)); 
            }
            else
            {
                cprintf("Last Erase current error ,Mark bad block \n");
                (void)nand_bad((offset + current_page) / blk_size);
                goto FalseQuit;
            } 

        }
        else if(dest_page == current_page)
        {
            cprintf("Error dest_page == current_page offset = %x  \n",current_page);
            goto FalseQuit;
        }
        flash_nand_oper_region_close(SHARE_REGION);
        return TRUE;
    }
FalseQuit:
    cprintf("flash_update_share_region_info error \n");
    flash_nand_oper_region_close(SHARE_REGION);
    return FALSE;

}

/*===========================================================================
 
FUNCTION  FLASH_NAND_OPER_REGION_INIT
 
DESCRIPTION
    Initilize user partition info,and get the handle of nand flash
RETURN VALUE
    TRUE if Op Succeed
    FALSE if Op Failure
 
SIDE EFFECTS
  None
 
===========================================================================*/
oper_region_struct_t *flash_nand_oper_region_init(  oper_region_type  oper_region_idx)
{
    uint8                idx              = (uint8)oper_region_idx;
    char                 *parent_name_ptr = NULL;
    uint32               offset           = 0;
    struct nand_spec *spec = NULL;
    uint32 ret = 0;
    struct ST_PART_TBL * ptable = NULL;
    
    /* 范围保护 */
    if (idx >= REGION_MAX)
    {
        cprintf("Parm error: idx = %d max_ids = %d\n",idx,REGION_MAX);
        return NULL;
    }
    /* 调用高通函数获取设备指针 */

    spec = (struct nand_spec *)alloc(sizeof(struct nand_spec));
    if(!spec)
    {
        cprintf("flash_nand_oper_region_init Malloc memory failed\n");
        return NULL;
    }

    ret = bsp_get_nand_info(spec); /*lint !e628*/
    if(ret)
    {
       cprintf("bsp_get_nand_info error\n");
       return NULL;
    }
 
    /*lint -e662*/
    parent_name_ptr = (char *)child_region[idx].parent_name;
    /*lint +e662*/
    /*lint -e661*/
    offset          = child_region[idx].offset;
    /*lint +e661*/

    /* get the partition table */
    if(0 != ptable_ensure()) /*lint !e628*/
    {
        cprintf("ptable_ensure  error\n");
        return NULL;
    }

    ptable = ptable_get_ram_data();
    if(!ptable)
    {
        cprintf("ptable_get_ram_data  error\n");
        return NULL;
    }

    /* find the partition */
    while(0 != strcmp(PTABLE_END_STR, ptable->name))
    {
        if(0 == strcmp(parent_name_ptr, ptable->name))
        {
            break;
        }
        ptable++;
    }
    if(!strcmp(PTABLE_END_STR, ptable->name))
    {
        cprintf("ptable not find %s \n",parent_name_ptr);
        return NULL;
    }
    //cprintf("blk_size = %x , page_size =%x page_mun \n",spec->blocksize,spec->pagesize,spec->pagenumperblock);
    /* get the flash address */
    operation_region[idx].block_size = spec->blocksize;
    operation_region[idx].page_size = spec->pagesize;
    operation_region[idx].start_addr = (ptable->offset + 
                                       (offset * operation_region[idx].block_size));
    operation_region[idx].buffer = himalloc(operation_region[idx].page_size); 
    if(NULL == operation_region[idx].buffer)
    {
        return NULL;
    }

    /* 初始化读写flash时使用的缓冲区 */
    memset((void *)operation_region[idx].buffer, NAND_FILL_CHAR_APP,
           operation_region[idx].page_size);

    return &operation_region[idx];
}/* flash_nand_oper_region_init() */

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
void flash_nand_oper_region_close(oper_region_type  oper_region_idx)
{

   (void) free(operation_region[oper_region_idx].buffer);
    operation_region[oper_region_idx].buffer = NULL;

}
#endif

