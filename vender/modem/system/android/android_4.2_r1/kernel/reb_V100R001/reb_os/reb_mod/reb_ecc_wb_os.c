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
/******************************问题单修改记录**********************************
问题单号             修改人               修改日期          修改内容
******************************************************************************/

/*----------------------------------------------*
 * 包含头文件()                                   *
 *----------------------------------------------*/
#include "../reb_config.h"
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/export.h>
#include <linux/string.h>
#include "../os_adapter/reb_os_adapter.h"
#include <mtd/mtd-abi.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/semaphore.h>
#include <linux/syscalls.h>
#include "reb_func.h"
#if ( YES == Reb_Platform_V7R2 )
#include "ptable_com.h"
#endif
/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
#define MTD_NAME_PREFIX "/dev/mtd/mtd%d"

/*魔数,用于判断回写信息是否有效*/
#define OUR_MAGIC_NUM (0x19830609)

/*最大支持block范围对(分区数)*/
#define REB_ECC_SCAN_MAX_NUM            (16)                                         

/*最大分区名字符个数*/
#define REB_ECC_SCAN_MAX_LEN            (32)                                         

/*开机后尝试打开MTD设备的次数*/
#define MAX_TRY_TIME   (5)

/*一个Page最大支持8个section,对应8个ECC*/
#if ( YES == Reb_Platform_V7R2 )
#define REB_MAX_SECT_NUM       (4)
#else
#define REB_MAX_SECT_NUM       (4)
#endif

#define MAX_MTD_NAME_LEN       (32)

/*病危块数据备份区*/
#define PART_WB_DATA "wbdata"
/*病危块数据备份区占用的block数*/
#define WB_DATA_BLK_NUM (5)

/*回写信息备份区*/
#define PART_WB_INFO "WbInfo"
/*病危块回写信息备份区占用的block数*/
#define WB_INFO_BLK_NUM (3)

#define MS_OF_ONE_SECOND (1000)
/*产品flsh控制器ECC纠错能力*/
#define FLASH_ECC_BITS (1)

/*ECC纠错能力通过自适应获得,默认是1bit*/
unsigned int flash_ecc_bits = FLASH_ECC_BITS;

/*要扫描的分区名称配置, 移植时需填写*/
#if ( YES == Reb_Platform_V7R2 )
#define PART_NAME0 "kernel"
#define PART_NAME1 "kernelbk"
#define PART_NAME2 "m3boot" 
#define PART_NAME3 "m3image"
#define PART_NAME4 "dsp"
#define PART_NAME5 "fastboot"
#define PART_NAME6 "vxworks"
#define PART_NAME7 "nvbacklte"
#define PART_NAME8 "nvdefault"
#define PART_NAME9 "nvdload"
#define PART_NAMEA "oeminfo"
#define PART_NAMEB NULL
#define PART_NAMEC NULL
#define PART_NAMED NULL
#define PART_NAMEE NULL
#define PART_NAMEF NULL
#else /*V3R3*/
#define PART_NAME0 "BootLoad"
#define PART_NAME1 "NvBackGU"
#define PART_NAME2 "BootRomA"
#define PART_NAME3 "BootRomB"
#define PART_NAME4 "VxWorks"
#define PART_NAME5 "FastBoot"
#define PART_NAME6 "kernel"
#define PART_NAME7 NULL
#define PART_NAME8 NULL
#define PART_NAME9 NULL
#define PART_NAMEA NULL
#define PART_NAMEB NULL
#define PART_NAMEC NULL
#define PART_NAMED NULL
#define PART_NAMEE NULL
#define PART_NAMEF NULL
#endif
/*----------------------------------------------*
 * 内部数据类型定义                             
 *----------------------------------------------*/
#define MAX_SCAN_PART_SIZE (50)
typedef struct __part_info_for_scan
{
    unsigned int mtd_idx;
//    unsigned int mtd_size;
    /*对于超过50个块的分区,将该分区分为两部分
    0-上部分 非-下部分*/
    unsigned int blk_offset;
}part_info_for_scan;

/*flash控制器ECC位置信息*/
typedef struct __ecc_cfg_info
{
    unsigned char bit_offset;   /*偏移量*/
    unsigned char bit_sum;      /*作用域*/
    unsigned char rsv1;         /*作用域*/
    unsigned char rsv2;         /*作用域*/
}ecc_cfg_info;

/*(nand扫描)分区的block范围结构*/
typedef struct __nand_scan_config
{
    unsigned int start_blk_id;                               /*起始block id*/
    unsigned int end_blk_id;                                 /*结束block id*/
    unsigned int mtd_idx;                                    /*对应MTD的索引*/
}nand_scan_config;

/*flash器件信息*/
typedef struct __nand_dev_info
{
    unsigned int page_size;                               /*页大小*/
    unsigned int blk_size;                                /*块大小*/
}nand_dev_info;
 
/*扫描的位置信息*/
typedef struct __nand_scan_pos_info
{
    /*当前扫描的分区序列(对应scan_blk_scope的idx)，
      从零往上连续, 注意和mtd的分区时不一样的
      */
    unsigned int part_idx;      
    unsigned int blk_pos;                                /*块位置*/
    unsigned int page_pos;                               /*页位置*/
}nand_scan_pos_info;
/*
nand回写信息,为了防止在回写过程中掉电,采用如下流程:
扫描blk->读病危blk内容并写到备份区I(需要5个blk资源)->
nand回写信息写入备份区II(需要3个blk资源)->回写->清除
备份区I内的nand回写信息.
为防止回写过程中发生掉电,开机后在boot阶段从备份区II
中读取nand回写信息,并进行回写操作.
备份区I和备份区II以分区的形式存在,不挂载文件系统.
*/
typedef struct __nand_wb_info
{
    unsigned int magic;                 /*魔数字*/
    unsigned int sick_mtd_id;           /*当前需回写的代码分区id*/
    
    /*当前需回写的代码分区block id, 注意是sick_mtd_id分区的blk偏移*/
    unsigned int sick_blk_id;           
    
    /*备份代码的block id-从wbdata分区起始地址的偏移*/
    unsigned int backup_blk_id;         

    /*备份回写信息的block id-从wbdata分区起始地址的偏移*/
    unsigned int wb_info_blk_id;        
}nand_wb_info;

/*nand扫描信息*/
typedef struct __nand_scan_info
{
    nand_scan_pos_info current_pos;                                /*当前扫描的的位置*/
    unsigned int sec_ecc_sta[REB_MAX_SECT_NUM];                    /*保存一个page的所有section的ECC状态信息*/
    nand_dev_info scan_dev_info;                                   /*需要扫描flash器件信息*/
//#define FLASH_BLK_SIZE scan_dev_info.blk_size
    nand_scan_config scan_blk_scope[REB_ECC_SCAN_MAX_NUM];         /*需要扫描分区的block范围对*/
}nand_scan_info;

/*----------------------------------------------*
 * 内部符号说明                             *
 *----------------------------------------------*/


/*----------------------------------------------*
 * 外部符号声明                                 *
 *----------------------------------------------*/
extern void (*set_ecc_result_func)(unsigned int, unsigned int );
extern void (*get_ecc_cfg_func)( unsigned int, unsigned int );

extern unsigned int get_random_num(void);
extern int reb_get_ptable_num(void); 
extern struct ST_PART_TBL * ptable_get(void);   
/*----------------------------------------------*
 * 内部变量                                   *
 *----------------------------------------------*/
int ptable_part_num = 0; 
const ST_PART_TBL_ST *pt_part_table = NULL;

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/
 /*返回扫描结果*/
typedef enum{
    reb_invalid = -1,
    reb_ok = 0,
    reb_sick,
    reb_bad
}page_health_sta;


/*nand扫描信息*/
static nand_scan_info reb_scan_info = {0};

/*nand回写信息*/
static nand_wb_info reb_wb_info = {0, 0, 0, 0, 0};

/*一段内存地址,用于Blk信息多次搬运*/
char *mem_for_page = NULL;

/*page大小一段内存地址,前面sizeof(nand_wb_info)字节用于保存回写信息*/
char *mem_for_wb_info = NULL;
/*Nand器件需要扫描的分区总数*/
unsigned int scan_part_sum = 0;
ecc_cfg_info ecc_cfg[REB_MAX_SECT_NUM] = 
{
#if ( YES == Reb_Platform_V7R2 ) /*目前只针对4bit ECC纠错的配置*/
    {  0, 8, 0, 0 },                 /*sector 0的ECC配置*/
    {  8, 8, 0, 0 },                 /*sector 1的ECC配置*/
    { 16, 8, 0, 0 },                 /*sector 2的ECC配置*/
    { 24, 8, 0, 0 },                 /*sector 3的ECC配置*/
#else
    0
#endif
};

ecc_cfg_info ecc_cfg_bits =
{
#if ( YES == Reb_Platform_V7R2 ) 
    9, 3, 0, 0                  /*ECC位翻转位数的配置*/
#else
    0
#endif
};


/*要扫描的分区名字*/
static const char* reb_scan_part_name_array[REB_ECC_SCAN_MAX_NUM] = 
{
    PART_NAME0,
    PART_NAME1,
    PART_NAME2,
    PART_NAME3,
    PART_NAME4,
    PART_NAME5,
    PART_NAME6,
    PART_NAME7,
    PART_NAME8,
    PART_NAME9,
    PART_NAMEA,
    PART_NAMEB,
    PART_NAMEC,
    PART_NAMED,
    PART_NAMEE,
    PART_NAMEF
};


/*****************************************************************************
 函 数 名  : part_name_to_mtd_index
 功能描述  : 从要扫描MTD分区名字找到MTD的索引(从0开始)
 输入参数  : part_name-分区名字数组; 
 输出参数  : p_mtd_idx-要扫描MTD分区的索引(从0开始);
 返 回 值  : 0:没有找到或其他错误 1:找到
*****************************************************************************/
int part_name_to_mtd_index( const char* part_name, unsigned int* p_mtd_idx ) 
{
    unsigned int idx = 0;
    
    if ( ( NULL == part_name ) || ( NULL == p_mtd_idx ) )
    {
        reb_msg( REB_MSG_ERR, "Reb_DFT: addr is NULL\r\n" );
        return 0;
    }

    /*匹配分区表名称*/
    for ( idx = 0; idx < ptable_part_num; idx++ )
    {
        if ( strlen( part_name ) != strlen( pt_part_table[idx].name) )
        {
            continue;
        }
        
        /*获取分区表地址:*/
        if( 0 == strcmp(part_name, pt_part_table[idx].name) )
        {

            *p_mtd_idx = idx;
            reb_msg( REB_MSG_ERR, "Reb_DFT: find suited mtd idx\r\n" );
            return 1;
        }
    }

    return 0;
}

/*****************************************************************************
 函 数 名  : part_name_to_scope
 功能描述  : 从要扫描MTD分区名字找到其blk范围(从0开始)
 输入参数  : input_array-分区名字数组; 
 输出参数  : output_array要扫描分区的索引(从0开始);
             *p_ret_num-有效分区个数
 返 回 值  : -1:参数错误 0:执行成功
注意事项   : 目前只适用与分区block size是块对齐的情况
*****************************************************************************/
int part_name_to_scope( const char* input_array[REB_ECC_SCAN_MAX_NUM], 
    nand_scan_config scan_items[REB_ECC_SCAN_MAX_NUM], unsigned int* p_ret_num )
{
    unsigned int idx_input, idx_output = 0;
    unsigned int ret_num = 0;
    int ret_val = 0;
    
    if ( ( NULL == input_array ) || ( NULL == scan_items ) )
    {
        reb_msg( REB_MSG_ERR, "Reb_DFT: addr is NULL\r\n" );
        return -1;
    }

    /*匹配分区表名称*/
    for ( idx_input = 0; idx_input < REB_ECC_SCAN_MAX_NUM; idx_input++ )
    {
        if ( NULL == input_array[idx_input] )
        {
            break;
        }

        /*获取对应的分区索引*/
        ret_val = part_name_to_mtd_index( input_array[idx_input], &idx_output );
        if ( 0 == ret_val )
        {
            reb_msg( REB_MSG_ERR, "Reb_DFT:NOT find part:%s\r\n",input_array[idx_input] );
        }
        else
        {
            scan_items[ret_num].start_blk_id
                = pt_part_table[idx_output].offset / reb_scan_info.scan_dev_info.blk_size;

#if ( YES == Reb_Platform_V7R2 )
            scan_items[ret_num].end_blk_id = ( pt_part_table[idx_output].offset + \
                pt_part_table[idx_output].capacity ) / reb_scan_info.scan_dev_info.blk_size;
#else
            scan_items[ret_num].end_blk_id = ( pt_part_table[idx_output].offset + \
                pt_part_table[idx_output].loadsize ) / reb_scan_info.scan_dev_info.blk_size;
#endif      
            if ( scan_items[ret_num].end_blk_id > scan_items[ret_num].start_blk_id )
            {   
                scan_items[ret_num].end_blk_id -= 1;
            }
            else
            {
                reb_msg( REB_MSG_ERR, "Reb_DFT:mtd%u start and end blk equal\r\n", idx_output );
            }
            
            scan_items[ret_num].mtd_idx = idx_output;
            ret_num++;
        }
        
    }

    *p_ret_num = ret_num;
    return 0;
    
}
/*****************************************************************************
 函 数 名  : ecc_cfg_to_bits
 功能描述  : 读取flash控制器ECC配置,并根据不同平台得出ECC校验能力
 输入参数  : flash控制器ECC配置
 输出参数  : flash控制器ECC校验能力
 返 回 值  : none
*****************************************************************************/
void ecc_cfg_to_bits( unsigned int ctrl_cfg, unsigned int* pt_ecc_bits )
{
    if ( NULL == pt_ecc_bits )
    {
        reb_msg( REB_MSG_ERR, "Reb_DFT:pt_ecc_bits err\r\n");
        return;
    }

    *pt_ecc_bits = 0;
    switch ( ctrl_cfg )
    {
#if ( YES == Reb_Platform_V7R2 )
        case 0x00:  /*未使能flash控制器ECC*/
            break;
        case 0x01:  /*ECC配置为0x01*/
            *pt_ecc_bits = 1; /*flash控制器ECC:1bit 模式*/
            break;
        case 0x02:  /*ECC配置为0x02*/
            *pt_ecc_bits = 4; /*flash控制器ECC:4bit 模式*/
            break;
        case 0x03:  /*ECC配置为0x03*/
            *pt_ecc_bits = 8; /*flash控制器ECC:8bit 模式*/
            break;
        default:
            break;
#endif
    }
    
    reb_msg( REB_MSG_INFO, "Reb_DFT:pt_ecc_bits is %u\r\n", *pt_ecc_bits );
    return;
}
/*****************************************************************************
 函 数 名  : reb_get_ecc_cfg(对外接口)
 功能描述  : 从flash控制器获取ECC配置信息
             当前主要是ECC纠错能力(disable/1/4/8bit)
 输入参数  : 控制器地址
 输出参数  : none
 返 回 值  : none
*****************************************************************************/
void reb_get_ecc_cfg( unsigned int reg_base, unsigned int offset )
{
    unsigned int value  = 0;
    unsigned int temp = 0;
    static unsigned int fst_flag = 0;

    if ( 0 == reg_base )
    {
        reb_msg( REB_MSG_ERR, "Reb_DFT:flash controller reg_base err\r\n");
        return;
    }

    if ( 0 == fst_flag )
    {
#if ( YES == Reb_Platform_V7R2 )
        value = *( volatile unsigned int* )( reg_base + 0 );
        temp = ( value >> ecc_cfg_bits.bit_offset ) 
            & ( ( 1 << ecc_cfg_bits.bit_sum ) - 1 );
        reb_msg( REB_MSG_INFO, 
            "Reb_DFT:flash ctrl cfg is 0x%08x, flash ecc ctrl is 0x%08x\r\n",
            value, temp );
        if ( 0 != temp )
        {
            fst_flag = 1;
        }
#else
        fst_flag = 1;
#endif
        ecc_cfg_to_bits( temp, &flash_ecc_bits );

    }
}
/*****************************************************************************
 函 数 名  : reb_set_ecc_result(对外接口)
 功能描述  : 读page操作后,从flash控制器获取每个页内section的ECC校验结果
             并把结果读入到reb_scan_info.sec_ecc_sta字段
 输入参数  : 控制器地址
 输出参数  : none
 返 回 值  : none
*****************************************************************************/
void reb_set_ecc_result( unsigned int reg_base, unsigned int offset )
{
    unsigned int value  = 0;
    unsigned int idx = 0;
    unsigned int temp = 0;

    if ( 0 == reg_base )
    {
        reb_msg( REB_MSG_ERR, "Reb_DFT:flash controller reg_base err\r\n");
        return;
    }

    value = *( volatile unsigned int* )( reg_base + offset );
#ifdef REB_OS_DEBUG    
    reb_msg( REB_MSG_DEBUG, "Reb_DFT:flash controller ECC result: 0x%08x\r\n", value );
#endif
    for ( idx = 0; idx < REB_MAX_SECT_NUM; idx++ )
    {
        temp = ( value >> ecc_cfg[idx].bit_offset ) & ( ( 1 << ecc_cfg[idx].bit_sum ) - 1 );
        if ( temp > reb_scan_info.sec_ecc_sta[idx] )
        {
            reb_scan_info.sec_ecc_sta[idx] = temp;
        }
    }
    return;

    /*从flash控制器读取每个section的的位翻转信息*/
    /*具体操作需参考对于平台寄存器手册描述
    因为每读一个page都会获取一组位翻转信息
    我们采取这样的原则: 只有当新的翻转数目更大时,我们才往sec_ecc_sta中更新
    注意在读取下一个blk时,这个信息要进行清除*/
}

#ifdef REB_OS_DEBUG
/*****************************************************************************
 函 数 名  : set_ecc_result_test(对外接口)
 功能描述  : 读page操作后,从flash控制器获取每个页内section的ECC校验结果
             并把结果读入到reb_scan_info.sec_ecc_sta字段
 输入参数  : bit_turned-位翻转的个数
 输出参数  : none
 返 回 值  : none
*****************************************************************************/
void set_ecc_result_test( unsigned int bit_turned )
{
    reb_scan_info.sec_ecc_sta[0] = bit_turned;
    /*此处当前只考虑4bit ECC的场景*/
    /*从flash控制器读取每个section的的位翻转信息*/
    /*具体操作需参考对于平台寄存器手册描述*/
}
#endif
/*****************************************************************************
 函 数 名  : is_sick_blk
 功能描述  : 读page/blk操作后,根据reb_scan_info.sec_ecc_sta字段,查看是否
             病危
 输入参数  : none
 输出参数  : none
 返 回 值  : reb_sick-病危 reb_ok-还好 reb_bad-成为坏块
注意事项   : 检测玩成后要清除sec_ecc_sta中的信息
*****************************************************************************/
static page_health_sta is_sick_blk(void)
{
    unsigned int idx = 0;
    unsigned int max_bit_change = 0;

    for ( idx = 0; idx < REB_MAX_SECT_NUM; idx++ )
    {
        if ( max_bit_change < reb_scan_info.sec_ecc_sta[idx] )
        {
            max_bit_change = reb_scan_info.sec_ecc_sta[idx];
        }
    }
    if ( max_bit_change == flash_ecc_bits )
    {
        reb_msg( REB_MSG_ERR, "Reb_DFT:find sick page\r\n" );
        return reb_sick;
    }
    else if ( 0 == max_bit_change ) /*未发现位翻转*/
    {
#ifdef REB_OS_DEBUG
        reb_msg( REB_MSG_DEBUG, "Reb_DFT:No bit change found\r\n" );
#endif
        return reb_ok;
    }
    else if ( max_bit_change < flash_ecc_bits )
    {
        reb_msg( REB_MSG_INFO, "Reb_DFT:find bit change but not sick page\r\n" );
        return reb_ok;
    }
    else
    {
        reb_msg( REB_MSG_ERR, "Reb_DFT:Maybe find bad page\r\n" );
        return reb_bad;
    }
}
/*****************************************************************************
 函 数 名  : reb_blk_to_mtd_id
 功能描述  : 从blk定位到part_idx、mtd分区id及block偏移
            注意这里part_idx是记录扫描序列scan_blk_scope[]数组的索引
            与mtd_id有明显区别
 输入参数  : blk_id
 输出参数  : mtd_id blk_offset
 返 回 值  : false:未找到 true:找到
*****************************************************************************/
int blk_to_mtd_id(unsigned int blk_id, unsigned int *part_idx,
    unsigned int *mtd_id, unsigned int* blk_offset)
{
    unsigned int idx = 0;
    for ( idx = 0; idx < REB_ECC_SCAN_MAX_NUM; idx++ )
    {
        if ( ( blk_id >= reb_scan_info.scan_blk_scope[idx].start_blk_id ) \
            && ( blk_id <= reb_scan_info.scan_blk_scope[idx].end_blk_id ) )
        {
            if ( NULL != part_idx )
            {
                *part_idx = idx;
            }
            if ( NULL != mtd_id )
            {
                *mtd_id = reb_scan_info.scan_blk_scope[idx].mtd_idx;
            }
            if ( NULL != blk_offset )
            {
                *blk_offset = blk_id - reb_scan_info.scan_blk_scope[idx].start_blk_id;
            }
            
            return true;
        }
    }

    return false;
}
/*****************************************************************************
 函 数 名  : scan_one_page
 功能描述  : 扫描一个page
 输入参数  : blk_id-要扫描page所在的blk(0-xx), page_id-要扫描的page(0-xx)
             rd_dst-数据读取的目标地址
 输出参数  : none
 返 回 值  : reb_invalid-发生错误 reb_sick-病危 reb_ok-还好 reb_bad-成为坏块
*****************************************************************************/
page_health_sta scan_one_page(unsigned int blk_id, unsigned int page_id, char* rd_dst)
{
    int ret = true;
    page_health_sta ret_val = reb_invalid;
    unsigned int tmp_mtd_idx = 0;
    unsigned int tmp_blk_offset = 0;
    static unsigned int blk_id_bak = 0; /*为了避免每次都对blk进行坏块查询,只有blk变化时再查询*/
    char mtd_name[MAX_MTD_NAME_LEN] = {0}; 
    unsigned long fs_old = 0;        
    int fd = 0;
    long long cur_pos = 0;
    int bad_blk_flg = 0;
    ret = blk_to_mtd_id( blk_id, NULL, &tmp_mtd_idx, &tmp_blk_offset );
    if ( false == ret )
    {
        reb_msg( REB_MSG_ERR, "Reb_DFT: Invalid blk_id: %d\r\n", blk_id );
        return reb_invalid;
    }

    snprintf( mtd_name, sizeof(mtd_name),MTD_NAME_PREFIX, tmp_mtd_idx );

    /*备份内核访问权限 */
    fs_old = get_fs();
    
    /*改变内核访问权限 */
    set_fs(KERNEL_DS);

    /*打开文件,获取句柄*/
    fd = sys_open( mtd_name, O_RDONLY, 0 );
    if ( fd < 0 )
    {
        reb_msg( REB_MSG_ERR, "Reb_DFT: Open file %s fail: %d\r\n", mtd_name, fd );
        ret_val = reb_invalid;
        goto err_ret;
    }


    cur_pos = tmp_blk_offset * reb_scan_info.scan_dev_info.blk_size;

    /*是否进入一个新的blk*/
    if ( blk_id_bak != blk_id ) 
    {
        blk_id_bak = blk_id;
        memset( reb_scan_info.sec_ecc_sta, 0, sizeof(reb_scan_info.sec_ecc_sta) );
    
        /*查看是否是坏块*/
        bad_blk_flg = sys_ioctl(fd, MEMGETBADBLOCK, (unsigned long)&cur_pos);
        if ( bad_blk_flg != 0 )
        {
            reb_msg( REB_MSG_ERR, "Reb_DFT: Get bak blk info fail or bad blk: %d\r\n", bad_blk_flg );
            ret_val = reb_bad;
            goto err_ret;
        }
    }

    /*移动文件节点指针,具体到某个页*/
    cur_pos += page_id * reb_scan_info.scan_dev_info.page_size;
    sys_lseek( fd, cur_pos, SEEK_SET );

    ret = sys_read( fd, rd_dst, reb_scan_info.scan_dev_info.page_size );
    if( ret < 0 )
    {
        reb_msg( REB_MSG_ERR, "REB_DFT: %s: sys read fail,the ret is %d\n", __func__, ret );
        ret_val = reb_invalid;
        goto err_ret;
    }

    /*是否是病危块*/
    ret_val = is_sick_blk();

err_ret:
    if ( fd >= 0 )
    {
        sys_close(fd);
    }
    /*恢复内核访问权限 */
    set_fs(fs_old);
    return ret_val;

}

/*****************************************************************************
 函 数 名  : find_first_valid_blk
 功能描述  : 找到某个分区内从偏移量开始第一个正常块(非坏块)
 输入参数  : fd-打开的文件句柄, offset-文件指针的偏移量(单位是blk)
 输出参数  : first_blk的索引(从0开始)
 返 回 值  : -1-发生错误(分区不存在/都是坏块)
 注意事项  : 病危块分区结构
                    -------------------------------------
                        病      |                        |
                        危      | 病危块数据备份(5个blk) |
                        块      |                        |
                        备      --------------------------
                        分      | 回写信息备份(3个blk)   |
                        区      |                        |
                    -------------------------------------
*****************************************************************************/
static int find_first_valid_blk( unsigned int fd, unsigned int* first_blk, unsigned int offset )
{
    struct mtd_info_user as_mtd_info = {0};
    int ret = 0;
    int bad_flg = 0; /*坏块标记*/
    unsigned int blk_sum = 0; /*该分区Blk总数*/
    unsigned int idx = 0; /*该分区的blk索引,从0开始*/
    unsigned long long cur_pos =  (unsigned long long)offset *  (unsigned long long)reb_scan_info.scan_dev_info.blk_size; /*当前位置*/

    /*获取mtd分区信息*/
    ret = sys_ioctl(fd, MEMGETINFO, (unsigned long)(&as_mtd_info));
    if ( ret < 0 )
    {
        reb_msg( REB_MSG_ERR, "REB_DFT: sys ioctl get meminfo fail,the ret is %d\n", ret );
        return -1;
    }

    if ( 0 == as_mtd_info.size )
    {
        reb_msg( REB_MSG_ERR, "REB_DFT: get mtd% size fail\r\n", idx );
        return -1;
    }

    /*计算分区保护的物理block数*/    
    blk_sum = as_mtd_info.size / as_mtd_info.erasesize;
    if ( blk_sum < ( WB_DATA_BLK_NUM + WB_INFO_BLK_NUM ) )
    {
        reb_msg( REB_MSG_ERR, "REB_DFT:partition to small, size: %d\r\n", blk_sum );
        return -1;
    }

    if ( 0 == offset )
    {
        blk_sum = WB_DATA_BLK_NUM;
    }
    else if ( WB_DATA_BLK_NUM == offset )
    {
        blk_sum = WB_INFO_BLK_NUM;
    }
    else
    {
        reb_msg( REB_MSG_ERR, "REB_DFT:%s,pare offset err\r\n", __func__ );
        return -1;
    }

    for ( idx = 0; idx < blk_sum; idx++ )
    {
        /*设置文件指针*/
        sys_lseek(fd, cur_pos, SEEK_SET);
    
        bad_flg = sys_ioctl(fd, MEMGETBADBLOCK, (unsigned long)(&cur_pos));
        if( bad_flg < 0 )
        {
            reb_msg( REB_MSG_ERR, "REB_DFT: sys ioctl fail,the ret is %d\n",bad_flg );
            return -1;
        }

        if( bad_flg )
        {
            cur_pos += as_mtd_info.erasesize;
        }
        else
        {
            *first_blk = offset + idx;
            reb_msg( REB_MSG_INFO, "REB_DFT:Find first valid blk:%d success\n",idx );
            return 0;
        }
    }

    reb_msg( REB_MSG_INFO, "REB_DFT:Find first valid blk fail\n" );
    return -1;
}

/*****************************************************************************
 函 数 名  : backup_wb_data
 功能描述  : 找到数据/信息备份分区内第一个正常块(非坏块),并备份病危数据到该blk
 输入参数  : part_name-数据备份MTD分区名称 sick_blk-当前病危块 
             src_addr:数据备份要使用的内存地址
 输出参数  : pt_back_blk-数据要备份位置(wbdata分区的blk)
 返 回 值  : -1-发生错误(分区不存在/都是坏块)
                    -------------------------------------
                        病      |                        |
                        危      | 病危块数据备份(5个blk) |
                        块      |                        |
                        备      --------------------------
                        分      | 回写信息备份(3个blk)   |
                        区      |                        |
                    -------------------------------------
 *****************************************************************************/
int backup_wb_data( char* part_name, unsigned int sick_blk, unsigned int* pt_back_blk, char* src_addr )
{
    int ret_val = -1;
    int fd_sick = -1, fd_bak = -1;
    unsigned long fs_old = 0;        
    char mtd_name[MAX_MTD_NAME_LEN] = {0}; 
    unsigned int idx = 0; 
    struct erase_info_user erase_info = {0};
    unsigned int page_sum_of_blk = ( reb_scan_info.scan_dev_info.blk_size 
        / reb_scan_info.scan_dev_info.page_size );
    unsigned long long sick_fd_pos = 0, bak_fd_pos = 0;
    unsigned int sick_mtd_idx = 0, bak_mtd_idx = 0;
    unsigned int sick_blk_offset = 0;

    if ( ( NULL == part_name ) || ( NULL == pt_back_blk ) || ( NULL == src_addr ) )
    {
        reb_msg( REB_MSG_ERR, "Reb_DFT: Invalid part_name pt_back_blk or src_addr\r\n" );
        return ret_val;
    }

    /*定位病危块所在的MTD, 及相对于MTD设备文件的偏移blk*/
    ret_val = blk_to_mtd_id( sick_blk, NULL, &sick_mtd_idx, &sick_blk_offset );
    if ( false == ret_val )
    {
        reb_msg( REB_MSG_ERR, "Reb_DFT: Invalid sick_blk: %d\r\n", sick_blk );
        return ret_val;
    }

    /*获取分区MTD设备索引*/
    ret_val = part_name_to_mtd_index( part_name, &bak_mtd_idx );
    if ( 0 == ret_val )
    {
        reb_msg( REB_MSG_ERR, "REB_DFT: Invalid partition name\r\n" );
        return -1;
    }
    
    /* 改变内核访问权限 */
    fs_old = get_fs();
    set_fs(KERNEL_DS);

    /*组合病危数据备份mtd设备名称*/
    snprintf( mtd_name, sizeof(mtd_name),MTD_NAME_PREFIX, bak_mtd_idx);

    /*打开文件,获取句柄*/
    fd_bak = sys_open( mtd_name, O_RDWR, 0 );
    if ( 0 > fd_bak )
    {
        reb_msg( REB_MSG_ERR, "REB_DFT: Open mtd%d fail, maybe mtd driver not init\r\n", bak_mtd_idx );
        goto err_ret;
    }

    /*获取数据备份区第一个有效block, 并输出到*pt_back_blk*/
    ret_val = find_first_valid_blk( fd_bak, pt_back_blk, 0 );
    if( ret_val < 0 )
    {
        reb_msg( REB_MSG_ERR, "REB_DFT: No valid block\r\n" );
        goto err_ret;
    }

    /*擦除单个块(病危数据备份块)*/
    erase_info.start  = ( *pt_back_blk ) * ( reb_scan_info.scan_dev_info.blk_size );
    erase_info.length = reb_scan_info.scan_dev_info.blk_size;
    ret_val = sys_ioctl(fd_bak, MEMERASE, (unsigned long)&erase_info);
    if( ret_val < 0 )
    {
        reb_msg( REB_MSG_ERR, "REB_DFT: erase blk %d fail\r\n", erase_info.start );
        goto err_ret;
    }
    
    /*清除原有备份数据MTD设备名称,并组合病危数据所在MTD设备名称*/
    memset( mtd_name, 0, sizeof(mtd_name) );
    snprintf( mtd_name, sizeof(mtd_name),MTD_NAME_PREFIX, sick_mtd_idx );

    /*只读方式打开病危块MTD设备文件,获取句柄*/
    fd_sick = sys_open( mtd_name, O_RDONLY, 0 );
    if ( 0 > fd_sick )
    {
        reb_msg( REB_MSG_ERR, "REB_DFT: Open mtd%d fail, maybe mtd driver not init\r\n", sick_mtd_idx );
        goto err_ret;
    }
    
    /*以页为单位进行搬运:病危块->数据备份块*/
    sick_fd_pos =  (unsigned long long)sick_blk_offset *  (unsigned long long)reb_scan_info.scan_dev_info.blk_size;
    bak_fd_pos  =  (unsigned long long)( *pt_back_blk ) *  (unsigned long long)reb_scan_info.scan_dev_info.blk_size;    
    for ( idx = 0; idx < page_sum_of_blk; idx++ )
    {
        /*移动病危块文件节点指针,具体到某个页,并读取该页数据*/
        sys_lseek( fd_sick, sick_fd_pos, SEEK_SET );
        ret_val = sys_read( fd_sick, src_addr, reb_scan_info.scan_dev_info.page_size );
        if( ret_val < 0 )
        {
            reb_msg( REB_MSG_ERR, "REB_DFT: %s: sys read fail,the ret is %d\n", __func__, ret_val );
            goto err_ret;
        }
        sick_fd_pos += reb_scan_info.scan_dev_info.page_size;

        reb_delay_ms(1);

        /*移动数据备份块文件节点指针,具体到某个页,并写该页数据*/
        sys_lseek( fd_bak, bak_fd_pos, SEEK_SET );
        ret_val = sys_write( fd_bak, src_addr, reb_scan_info.scan_dev_info.page_size );
        if( ret_val < 0 )
        {
            reb_msg( REB_MSG_ERR, "REB_DFT: %s:sys write fail,the ret is %d\n", __func__, ret_val );
            goto err_ret;
        }
        bak_fd_pos += reb_scan_info.scan_dev_info.page_size;

        reb_delay_ms(1);
    }
    
err_ret:
    if ( fd_sick >= 0 )
    {
        sys_close(fd_sick);
    }
    if ( fd_bak >= 0 )
    {
        sys_close(fd_bak);
    }
    set_fs(fs_old);
    return ret_val;
}
/*****************************************************************************
 函 数 名  : record_wb_info
 功能描述  : 记录回写信息
 输入参数  : part_name-MTD分区名称; 源数据就是nand_wb_info结构地址
 输出参数  : none
 返 回 值  : -1-发生错误(分区不存在/都是坏块)
                    -------------------------------------
                        病      |                        |
                        危      | 病危块数据备份(5个blk) |
                        块      |                        |
                        备      --------------------------
                        分      | 回写信息备份(3个blk)   |
                        区      |                        |
                    -------------------------------------
 *****************************************************************************/
int record_wb_info( char* part_name, nand_wb_info* pt_wb_info, const unsigned int wr_num )
{
    int fd = 0;
    int ret_val = -1;
    unsigned long fs_old = 0;        
    char mtd_name[MAX_MTD_NAME_LEN] = {0}; 
    unsigned int idx = 0; 
    struct erase_info_user erase_info = {0};

    /*获取分区MTD设备索引*/
    ret_val = part_name_to_mtd_index( part_name, &idx );
    if ( -1 == ret_val )
    {
        reb_msg( REB_MSG_ERR, "REB_DFT: Invalid partition name\r\n" );
        return ret_val;
    }

    /* 改变内核访问权限 */
    fs_old = get_fs();
    set_fs(KERNEL_DS);

    /*操作mtdx*/
    snprintf( mtd_name,sizeof(mtd_name), MTD_NAME_PREFIX, idx );


    /*打开文件,获取句柄*/
    fd = sys_open( mtd_name, O_RDWR, 0 );
    if ( 0 > fd )
    {
        reb_msg( REB_MSG_ERR, "REB_DFT: Open mtd%d fail, maybe mtd driver not init\r\n", idx );
        goto err_ret;
    }

    /*获取第一个有效block*/
    ret_val = find_first_valid_blk( fd, &idx, WB_DATA_BLK_NUM );
    if( ret_val < 0 )
    {
        reb_msg( REB_MSG_ERR, "REB_DFT: No valid block\r\n" );
        goto err_ret;
    }

    /*要记录回写信息*/
    if ( OUR_MAGIC_NUM == pt_wb_info->magic )
    {
        pt_wb_info->wb_info_blk_id = idx;
    }

    /*擦除单个块*/
    erase_info.start  = idx * reb_scan_info.scan_dev_info.blk_size;
    erase_info.length = reb_scan_info.scan_dev_info.blk_size;
    ret_val = sys_ioctl(fd, MEMERASE, (unsigned long)&erase_info);
    if( ret_val < 0 )
    {
        reb_msg( REB_MSG_ERR, "REB_DFT: erase blk %d fail\r\n", erase_info.start );
        goto err_ret;
    }
    
    /*设置文件指针*/
    sys_lseek(fd, idx * reb_scan_info.scan_dev_info.blk_size, SEEK_SET);

    /*以页为单位进行写操作*/
    memcpy( mem_for_wb_info, pt_wb_info, wr_num );
    ret_val = sys_write( fd, mem_for_wb_info, reb_scan_info.scan_dev_info.page_size );
    if( ret_val < 0 )
    {
        reb_msg( REB_MSG_ERR, "REB_DFT: %s:sys write fail,the ret is %d\n", __func__, ret_val );
        goto err_ret;
    }
    
err_ret:
    if ( fd >= 0 )
    {
        sys_close(fd);
    }
    set_fs(fs_old);
    return ret_val;
}
/*****************************************************************************
 函 数 名  : wb_sick_blk_data
 功能描述  : (备份病危数据及回写信息后)回写病危块
 输入参数  : part_name-病危块数据备份分区名称 
             src_addr-按page进行数据搬移时的内存地址 pt_wb_info-回写信息
 输出参数  : none
 返 回 值  : -1-发生错误(分区不存在/都是坏块)
 注意事项  : 如果是在kernel调用:内存->病危块
 *****************************************************************************/
int wb_sick_blk_data( char* part_name, char* src_addr, nand_wb_info* pt_wb_info )
{
    int fd_sick = -1, fd_bak = -1;
    int ret_val = -1;
    unsigned int idx = 0; 
    unsigned long fs_old = 0;        
    char mtd_name[MAX_MTD_NAME_LEN] = {0}; 
    unsigned long long sick_fd_pos = 0, bak_fd_pos = 0;
    unsigned int sick_mtd_idx = 0, bak_mtd_idx = 0;
    struct erase_info_user erase_info = {0};
    unsigned int page_sum_of_blk = ( reb_scan_info.scan_dev_info.blk_size 
        / reb_scan_info.scan_dev_info.page_size );

    /*查看入参是否合法*/
    if ( ( NULL == src_addr ) || ( NULL == pt_wb_info ) || ( NULL == part_name ) )
    {
        reb_msg( REB_MSG_ERR, "REB_DFT:Invalid addr\r\n" );
        return ret_val;
    }

    /* 改变内核访问权限 */
    fs_old = get_fs();
    set_fs(KERNEL_DS);

    /*获取分区MTD设备索引*/
    ret_val = part_name_to_mtd_index( part_name, &bak_mtd_idx );
    if ( -1 == ret_val )
    {
        reb_msg( REB_MSG_ERR, "REB_DFT: Invalid partition name\r\n" );
        return ret_val;
    }

    /*组合备份块所在MTD设备名称*/
    snprintf( mtd_name, sizeof(mtd_name),MTD_NAME_PREFIX, bak_mtd_idx );

    /*只读方式打开文件,获取备份块所在MTD设备文件句柄*/
    fd_bak = sys_open( mtd_name, O_RDONLY, 0 );
    if ( 0 > fd_bak )
    {
        reb_msg( REB_MSG_ERR, "REB_DFT: Open mtd%d fail, maybe mtd driver not init\r\n", bak_mtd_idx );
        goto err_ret;
    }

    /*组合病危块所在MTD设备名称*/
    sick_mtd_idx = pt_wb_info->sick_mtd_id;
    memset( mtd_name, 0, sizeof(mtd_name) );
    snprintf( mtd_name, sizeof(mtd_name), MTD_NAME_PREFIX, sick_mtd_idx );

    /*打开文件,获取病危块所在MTD设备文件句柄*/
    fd_sick = sys_open( mtd_name, O_RDWR, 0 );
    if ( 0 > fd_sick )
    {
        reb_msg( REB_MSG_ERR, "REB_DFT: Open mtd%d fail, maybe mtd driver not init\r\n", sick_mtd_idx );
        goto err_ret;
    }

    /*擦除病危块数据*/
    erase_info.start  = (pt_wb_info->sick_blk_id )*reb_scan_info.scan_dev_info.blk_size;
    erase_info.length = reb_scan_info.scan_dev_info.blk_size;
    ret_val = sys_ioctl(fd_sick, MEMERASE, (unsigned long)&erase_info);
    if( ret_val < 0 )
    {
        reb_msg( REB_MSG_ERR, "REB_DFT: erase blk %d fail\r\n", erase_info.start );
        goto err_ret;
    }
    
    /*以页为单位进行回写:数据备份块->病危块*/
    bak_fd_pos  =  (unsigned long long)( pt_wb_info->backup_blk_id ) *  (unsigned long long)reb_scan_info.scan_dev_info.blk_size;    
    sick_fd_pos =  (unsigned long long)( pt_wb_info->sick_blk_id ) *  (unsigned long long)reb_scan_info.scan_dev_info.blk_size;
    for ( idx = 0; idx < page_sum_of_blk; idx++ )
    {
        /*移动数据备份块文件节点指针,具体到某个页,并读取该页数据*/
        sys_lseek( fd_bak, bak_fd_pos, SEEK_SET );
        ret_val = sys_read( fd_bak, src_addr, reb_scan_info.scan_dev_info.page_size );
        if( ret_val < 0 )
        {
            reb_msg( REB_MSG_ERR, "REB_DFT: %s: sys read fail,the ret is %d\n", __func__, ret_val );
            goto err_ret;
        }
        bak_fd_pos += reb_scan_info.scan_dev_info.page_size;

        /*移动病危块文件节点指针,具体到某个页,并回写该页数据*/
        sys_lseek( fd_sick, sick_fd_pos, SEEK_SET );
        ret_val = sys_write( fd_sick, src_addr, reb_scan_info.scan_dev_info.page_size );
        if( ret_val < 0 )
        {
            reb_msg( REB_MSG_ERR, "REB_DFT: sys write fail,the ret is %d\n",ret_val );
            goto err_ret;
        }
        sick_fd_pos += reb_scan_info.scan_dev_info.page_size;
    }
    
err_ret:
    if ( fd_sick >= 0 )
    {
        sys_close(fd_sick);
    }
    if ( fd_bak >= 0 )
    {
        sys_close(fd_bak);
    }
    set_fs(fs_old);
    return ret_val;
}
/*****************************************************************************
 函 数 名  : reb_scan_flash_parts
 功能描述  : 逐个分区扫描病危块(由main task调用)
            备份->保存回写信息->回写->清除回写信息
 输入参数  : none
 输出参数  : the_pos:病危块的位置信息
 返 回 值  : reb_invalid-发生错误 reb_sick-病危 reb_ok-还好 reb_bad-成为坏块
*****************************************************************************/
int reb_scan_flash_parts( void )
{
    int ret_val = 0;

    /*数组scan_blk_scope的序列*/
    unsigned int cur_part_idx = reb_scan_info.current_pos.part_idx; 
    unsigned int cur_blk_ops = reb_scan_info.current_pos.blk_pos;
    unsigned int cur_page_ops = reb_scan_info.current_pos.page_pos; /*例如:0~63*/
    unsigned int page_sum_of_blk = ( reb_scan_info.scan_dev_info.blk_size 
        / reb_scan_info.scan_dev_info.page_size );

    if ( 0 == scan_part_sum )
    {
        reb_msg( REB_MSG_DEBUG, "REB_DFT:No block need to scan\r\n" );
        return reb_ok;
    }
    /*扫描一个page*/
    ret_val = scan_one_page( cur_blk_ops, cur_page_ops, mem_for_page );
    if ( 0 == flash_ecc_bits )
    {
        scan_part_sum = 0;
        reb_msg( REB_MSG_INFO, "REB_DFT:Set scan_part_sum = 0\r\n" );
        return reb_ok;
    }
    /* 如果回写功能没有打开 */
    if( false == g_kernel_soft_reliable_info.BadZoneReWriteEnable )
    {
        goto next_page;
    }

    if ( reb_sick == ret_val )
    {
        /*备份病危blk数据到flash备份区I,并输出备份blk的信息*/
        ret_val = backup_wb_data( PART_WB_DATA, cur_blk_ops, &reb_wb_info.backup_blk_id, mem_for_page );
        if ( ret_val < 0 )
        {
            goto next_block;
        }

        /*整理回写信息*/
        reb_wb_info.magic = OUR_MAGIC_NUM;
        reb_wb_info.sick_mtd_id = reb_scan_info.scan_blk_scope[cur_part_idx].mtd_idx;
        reb_wb_info.sick_blk_id = ( reb_scan_info.current_pos.blk_pos \
            - reb_scan_info.scan_blk_scope[cur_part_idx].start_blk_id );

        /*记录回写信息到备份区II, 并输出回写信息blk*/
        ret_val = record_wb_info( PART_WB_DATA, &reb_wb_info, sizeof(nand_wb_info) );
        if ( ret_val < 0 )
        {
            goto next_block;
        }
        
        /*回写病危blk*/
        ret_val = wb_sick_blk_data( PART_WB_DATA, mem_for_page, &reb_wb_info );
        if ( ret_val < 0 )
        {
            goto next_block;
        }

        /*清除flash备份区II的回写信息*/
        reb_wb_info.magic = 0;
        ret_val = record_wb_info( PART_WB_DATA, &reb_wb_info, sizeof(nand_wb_info) );
        if ( ret_val < 0 )
        {
            reb_msg( REB_MSG_INFO, "REB_DFT:record_wb_info fail\r\n" );
        }
        goto next_block;
    }
    else if ( ( reb_bad == ret_val ) || ( reb_invalid == ret_val ) )
    {
        reb_msg( REB_MSG_INFO, "REB_DFT:Find a bad or invalid blk\r\n" );
        goto next_block;
    }
    
next_page:
    cur_page_ops = ++reb_scan_info.current_pos.page_pos; /*page++*/

    /*某个blk所有page已经扫描完毕,指向下一个blk*/
    if ( page_sum_of_blk == cur_page_ops )
    {
    
next_block:
        cur_blk_ops = ++reb_scan_info.current_pos.blk_pos;      /*blk++*/
        reb_scan_info.current_pos.page_pos = 0;  /*page=0*/

        /*某个part(ition)所有的blk扫描完毕, 指向下一个part*/
        if ( cur_blk_ops > reb_scan_info.scan_blk_scope[cur_part_idx].end_blk_id )
        {
            cur_part_idx = ++reb_scan_info.current_pos.part_idx;  /*part++*/
            reb_scan_info.current_pos.blk_pos = \
                reb_scan_info.scan_blk_scope[cur_part_idx].start_blk_id;
            cur_blk_ops = 0;

            /*所有分区已经扫描完毕,停止扫描*/
            if ( scan_part_sum <= cur_part_idx )
            {
                reb_msg( REB_MSG_INFO, "REB_DFT:all scan is complete\r\n" );
                scan_part_sum = 0;
                cur_part_idx = 0;
            }
        }
        
    }
    
    return ret_val;
}
EXPORT_SYMBOL(reb_scan_flash_parts);

/*****************************************************************************
 函 数 名  : get_flash_dev_info
 功能描述  : 获取flash设备的页/块信息
 输入参数  : none 
 输出参数  : nand_dev_info
 返 回 值  : -1: 操作mtd设备失败
*****************************************************************************/
int get_flash_dev_info( nand_dev_info* pt_dev_info )
{
    long fd = 0;
    int ret_val = 0;
    unsigned long fs_old = 0;        
    struct mtd_info_user as_mtd_info = {0};
    char mtd_name[MAX_MTD_NAME_LEN] = {0}; 

    /*操作mtd0*/
    snprintf( mtd_name,sizeof(mtd_name),MTD_NAME_PREFIX, ret_val );

    /* 改变内核访问权限 */
    fs_old = get_fs();
    set_fs(KERNEL_DS);

    /*打开文件,获取句柄*/
    fd = sys_open( mtd_name, O_RDONLY, 0 );
    if ( fd < 0 )
    {
        reb_msg( REB_MSG_ERR, "REB_DFT: Open mtd0 fail, maybe mtd driver not init\r\n");
        ret_val = -1;
        goto err_ret;
    }

    /*获取mtd分区信息*/
    ret_val = sys_ioctl(fd, MEMGETINFO, (unsigned long)(&as_mtd_info));
    if ( ret_val < 0 )
    {
        reb_msg( REB_MSG_ERR, "REB_DFT: sys_ioctl fails, maybe mtd driver not init\r\n");
        goto err_ret;
    }
    
    pt_dev_info->blk_size = as_mtd_info.erasesize;
    pt_dev_info->page_size = as_mtd_info.writesize;
    
err_ret:
    if ( fd >= 0 )
    {
        sys_close(fd);
    }
    set_fs(fs_old);
    return ret_val;

}
/*****************************************************************************
 函 数 名  : part_scan_pos_init
 功能描述  : 初始化(开始)扫描位置
 输入参数  : none 
 输出参数  : none
 返 回 值  : none
*****************************************************************************/
void part_scan_pos_init(void)
{
    part_info_for_scan temp_struct[32] = {0}; /*最多支持32个随机单位*/
    unsigned int idx = 0;
    unsigned int struct_idx = 0;
    unsigned int random = 0;
    unsigned int blk_sum = 0;

    for ( idx = 0; idx < scan_part_sum; idx++ )
    {
        temp_struct[struct_idx].mtd_idx = reb_scan_info.scan_blk_scope[idx].mtd_idx;
        temp_struct[struct_idx].blk_offset
            = reb_scan_info.scan_blk_scope[idx].start_blk_id;

        /*查看是否超过50个块*/
        blk_sum = ( reb_scan_info.scan_blk_scope[idx].end_blk_id + 1 )
            - reb_scan_info.scan_blk_scope[idx].start_blk_id;
        if ( blk_sum >= MAX_SCAN_PART_SIZE )
        {
            struct_idx++;
            temp_struct[struct_idx].mtd_idx = reb_scan_info.scan_blk_scope[idx].mtd_idx;
            /*记录分区的下半部*/
            temp_struct[struct_idx].blk_offset
                = reb_scan_info.scan_blk_scope[idx].start_blk_id
                + blk_sum / 2;/*计算分区下半部*/
        }
        /*累加,准备下一次记录*/
        struct_idx++;
    }

    if(!struct_idx)
    {
        return;
    }
    /*获取随机数*/
    random = get_random_num();
    random &= 0xff; /*0~255*/
    reb_msg( REB_MSG_INFO, "Reb_DFT now random is %u, struct_idx is: %u\r\n", 
        random, struct_idx );
    random %= struct_idx;
    reb_msg( REB_MSG_INFO, "Reb_DFT at last random is %u\r\n", random );

    /*初始化扫描位置*/
    reb_scan_info.current_pos.blk_pos = temp_struct[random].blk_offset;
    (void)blk_to_mtd_id( reb_scan_info.current_pos.blk_pos, 
        &reb_scan_info.current_pos.part_idx, NULL, NULL );
    reb_scan_info.current_pos.page_pos = 0;    
}
/*****************************************************************************
 函 数 名  : reb_part_scan_init
 功能描述  : 初始化(注意要在linux mtd驱动初始化完成之后)
 输入参数  : none 
 输出参数  : none
 返 回 值  : none
*****************************************************************************/
int reb_part_scan_init(void)
{
    int ret_val = 0;
    unsigned int cnt = 0;

    /*读取NV项*/
    if ( false == g_kernel_soft_reliable_info.BadZoneScanEnable )
    {
        reb_msg( REB_MSG_INFO, "REB_DFT:Nand scan not enable\r\n");
        goto notice_main_task;
    }

    pt_part_table = ptable_get();
    if(NULL == pt_part_table)
    {
        reb_msg( REB_MSG_ERR, "REB_DFT: invalid pt_part_table\n");
        return 0;
    }

    ptable_part_num = reb_get_ptable_num();
    if(0 >= ptable_part_num)
    {
        reb_msg( REB_MSG_ERR, "REB_DFT: invalid part num %d\n",ptable_part_num);
        return 0;
    }
    reb_msg( REB_MSG_ERR, "REB_DFT: ptable_part_num %d\n",ptable_part_num);

    /*获取flash器件的信息*/
    for ( cnt = 0; cnt < MAX_TRY_TIME; cnt++ )
    {
        ret_val = get_flash_dev_info( &reb_scan_info.scan_dev_info );
        if ( 0 == ret_val )
        {
            break;
        }

        /*这个时候需要等待一下,因为现在mtd设备可能还没有加载*/
        reb_delay_ms(MS_OF_ONE_SECOND);
    }
    if ( MAX_TRY_TIME == cnt )
    {
        goto notice_main_task;
    }

    /*获取要扫描的分区信息*/
    ret_val = part_name_to_scope( reb_scan_part_name_array, reb_scan_info.scan_blk_scope, &scan_part_sum );
    if ( ret_val )
    {
        scan_part_sum = 0;
        goto notice_main_task;
    }
    if(0 == scan_part_sum)
    {
        goto notice_main_task;
    }
    /*初始化扫描位置*/
    part_scan_pos_init();

    /*申请一段内存:每次扫描一个page*/
    mem_for_page = (char *)kmalloc(reb_scan_info.scan_dev_info.page_size + 16, GFP_KERNEL);
    if ( NULL == mem_for_page )
    {
        reb_msg( REB_MSG_ERR, "REB_DFT:Not enough memory\r\n");
        scan_part_sum = 0;
        goto notice_main_task;
    }

    /*在内存做一个标志,防止内存写越界*/
    *(unsigned int *)( mem_for_page ) = OUR_MAGIC_NUM;

    mem_for_wb_info = (char *)kmalloc( reb_scan_info.scan_dev_info.page_size, GFP_KERNEL );
    if ( NULL == mem_for_wb_info )
    {
        reb_msg( REB_MSG_ERR, "REB_DFT:Not enough memory for mem_for_wb_info\r\n");
        scan_part_sum = 0;
        goto notice_main_task;
    }
    set_ecc_result_func = ( void(*)( unsigned int, unsigned int ) )reb_set_ecc_result;
    get_ecc_cfg_func = ( void(*)( unsigned int, unsigned int ) )reb_get_ecc_cfg;
notice_main_task:
    /*通知主控任务NAND扫描模块初始化完毕*/
    reb_send_msg_to_main_task( REB_NAND_SCAN_INIT_EVENT );
    return ret_val;
}
#ifdef REB_OS_DEBUG
/*****************************************************************************
 函 数 名  : show_cur_scan_pos
 功能描述  : 显示当前扫描信息
 输入参数  : none
 输出参数  : none
 返 回 值  : none
*****************************************************************************/
void show_cur_scan_pos(void)
{
    unsigned int idx = reb_scan_info.current_pos.part_idx;
    reb_msg( REB_MSG_DEBUG, "\r\nREB_DFT:Current mtd: %d\r\n", reb_scan_info.scan_blk_scope[idx].mtd_idx );
    reb_msg( REB_MSG_DEBUG, "REB_DFT:Current blk: %d\r\n", reb_scan_info.current_pos.blk_pos );
    reb_msg( REB_MSG_DEBUG, "REB_DFT:Current page: %d\r\n", reb_scan_info.current_pos.page_pos );
}

/*****************************************************************************
 函 数 名  : show_all_scan_info
 功能描述  : 显示病危块扫描信息
 输入参数  : none
 输出参数  : none
 返 回 值  : none
*****************************************************************************/
void show_all_scan_info(void)
{
    unsigned int part_idx = 0;

    for ( part_idx = 0; part_idx < scan_part_sum; part_idx++ )
    {
        reb_msg( REB_MSG_DEBUG, "\r\nREB_DFT:mtd idx: mtd%d\r\n", reb_scan_info.scan_blk_scope[part_idx].mtd_idx );
        reb_msg( REB_MSG_DEBUG, "REB_DFT:start blk: %d\r\n", reb_scan_info.scan_blk_scope[part_idx].start_blk_id );
        reb_msg( REB_MSG_DEBUG, "REB_DFT:  end blk: %d\r\n", reb_scan_info.scan_blk_scope[part_idx].end_blk_id );
    }
}
#endif
//late_initcall(reb_part_scan_init);



