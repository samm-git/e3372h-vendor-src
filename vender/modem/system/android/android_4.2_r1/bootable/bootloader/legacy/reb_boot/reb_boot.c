/******************************************************************************

                   版权所有 (C), 2013-2023,华为终端有限公司

 ******************************************************************************
  文 件 名   : reb_func.c
  版 本 号   : 初稿
  作    者   : gongashi 00175336
  生成日期   : 2013年11月08日
  最近修改   :
  函数列表   :
  修改历史   :
  1.日    期   : 2013年11月08日
    作    者   : gongashi 00175336
    修改内容   : 创建文件

******************************************************************************/
/******************************问题单修改记录**********************************
    日期         修改人         问题单号                 修改内容
******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "types.h"
#include "reb_boot.h"
#include <bsp_nandc.h>
#include "ptable_com.h"
#include "time.h"
#include <bsp_nvim.h>

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
/*病危块数据\信息备份区*/
#define PART_WB_DATA "wbdata"

/*备份区I(数据备份)blk数据*/
#define REB_DATA_BAK_BLK_SUM   (5)

/*备份区II(回写信息备份区blk数据)*/
#define REB_INFO_BAK_BLK_SUM   (3)

/*魔数,用于判断回写信息是否有效*/
#define OUR_MAGIC_NUM (0x19830609)

/*NV 50442 ID*/
#define   NV_ID_SOFT_RELIABLE_CFG  (50442)

/*----------------------------------------------*
 * 全局变量说明                                 *
 *----------------------------------------------*/
/*boot 阶段读取的NV 50442的值*/
SOFT_RELIABLE_CFG_STRU g_boot_soft_reliable_info = {0};
extern struct ST_PART_TBL ptable_product[];

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/
typedef struct __nand_wb_info
{
    unsigned int magic;                 /*魔数字*/
    unsigned int sick_mtd_id;          /*当前需回写的代码分区id*/

    /*当前需回写的代码分区block id, 注意是sick_mtd_id分区的blk偏移*/
    unsigned int sick_blk_id;           
    
    /*备份代码的block id-从wbdata分区起始地址的偏移*/
    unsigned int backup_blk_id;         

    /*备份回写信息的block id-从wbdata分区起始地址的偏移*/
    unsigned int wb_info_blk_id;    
}nand_wb_info;


/*----------------------------------------------*
 * 外部函数原型声明                             *
 *----------------------------------------------*/
extern int bsp_nand_isbad(const char *partition_name, u32 partition_offset);
extern int bsp_nand_write(const char *partition_name, u32 partition_offset, void* ptr_ram_addr, u32 length);

extern int bsp_get_nand_info(struct nand_spec *spec);
extern int bsp_nand_read(const char *partition_name, u32 partition_offset, void* ptr_ram_addr, u32 length, u32 *skip_len);

//extern unsigned int bsp_nvm_read(unsigned int itemid, unsigned char * pdata,
//                                        unsigned int datalen);
extern int bsp_nand_erase(const char *partition_name, u32 partition_offset);

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/
/*****************************************************************************
 函数名称  :  void reb_nv_boot_read()
 功能描述  : 根据不同平台读取NV
 输入参数  : No
 输出参数  : No
 返 回 值  : No
 修改历史  : 
             1. 2013-11-28 :  00206465 qiaoyichuan created
*****************************************************************************/
 void reb_nv_boot_read()
{
    int ret = 0;
#if ( YES == Reb_Platform_V7R2 )
    ret = bsp_nvm_read(NV_ID_SOFT_RELIABLE_CFG, (unsigned char *)&g_boot_soft_reliable_info,
                       sizeof(SOFT_RELIABLE_CFG_STRU));
#else
    /*其他平台扩展*/
#endif

    if (0 != ret)
    {
        cprintf( "REB_DFT:boot get g_boot_soft_reliable_info fail\r\n" );
    }
}
/*************************************************************
 函数名称  : reb_reliability_nv_cfg_boot_init
 功能描述  : 读取nv 50442 的值，获取可靠性配置信息
 输入参数  : 无
 输出参数  : 无
 返 回 值  :无
*************************************************************/
void  reb_reliability_nv_cfg_boot_init(void)
{
    reb_nv_boot_read();
}
/*****************************************************************************
 函 数 名  : wb_info_valid_check
 功能描述  : 检测回写信息的有效性
 输入参数  : pt_wb_info-回写信息\offset-回写信息在分区中的偏移位置
             器件大小\blk大小
 输出参数  : none
 返 回 值  : 1-有效 0-无效
*****************************************************************************/
unsigned int wb_info_valid_check( nand_wb_info* pt_wb_info, unsigned int offset,
    unsigned int chip_size, unsigned int blk_size )
{
    unsigned int blk_sum = chip_size / blk_size;

    /*查看魔数字是否存在*/
    if ( OUR_MAGIC_NUM != pt_wb_info->magic )
    {
        cprintf( "REB_DFT:Invalid write back info\r\n" );
        return 0;
    }

    if ( offset != pt_wb_info->wb_info_blk_id )
    {
        cprintf( "REB_DFT:Invalid write back info\r\n" );
        return 0;
    }

    /*查看块信息是否有效*/
    if ( pt_wb_info->sick_blk_id >= blk_sum )
    {
        cprintf( "REB_DFT:Invalid sick blk info\r\n" );
        return 0;
    }

    /*查看块信息是否有效*/
    if ( pt_wb_info->backup_blk_id >= blk_sum )
    {
        cprintf( "REB_DFT:Invalid back blk info\r\n" );
        return 0;
    }
#ifdef REB_BOOT_DEBUG
    /*lint -e515*/
    cprintf( "\r\nREB_DFT:correct write back info:\r\n" );
    cprintf( "REB_DFT:sick mtd is: %d\r\n", (int)pt_wb_info->sick_mtd_id );
    cprintf( "REB_DFT:sick blk is: %d\r\n", (int)pt_wb_info->sick_blk_id );
    cprintf( "REB_DFT:back blk is: %d\r\n", (int)pt_wb_info->backup_blk_id );
    cprintf( "REB_DFT:wb info blk is: %d\r\n", (int)pt_wb_info->wb_info_blk_id );
    /*lint +e515*/
#endif
    return 1;
}

/*****************************************************************************
 函 数 名  : get_good_blk_offset
 功能描述  : boot阶段查找好块偏移量
 输入参数  : part_name-分区名称 blk_size-分区尺寸 
 输出参数  : pt_blk_offset-该分区中第一个好块地址(从分区起始地址开始)
 返 回 值  : -1:没有找到 0-找到
*****************************************************************************/
int get_good_blk_offset( char* part_name, unsigned int blk_size, unsigned int* pt_blk_offset )
{
    int ret_val = 0;
    unsigned int idx = 0;
    unsigned int offset_byte = 0; /*分区中偏移地址*/

    if ( ( NULL == part_name ) || ( NULL == pt_blk_offset ) )
    {
        cprintf( "REB_DFT:No invalid blk fail\r\n" );
        return -1;
    }

    for ( idx = 0; idx < REB_INFO_BAK_BLK_SUM; idx++ )
    {
        offset_byte = ( idx + REB_DATA_BAK_BLK_SUM ) * blk_size;

        /*查看是否坏块,注意偏移量要blk对齐*/
        ret_val = bsp_nand_isbad( PART_WB_DATA, offset_byte );
        if ( 0 == ret_val )
        {
            *pt_blk_offset = REB_DATA_BAK_BLK_SUM + idx;
            /*lint -e515*/
            cprintf( "REB_DFT:Find good blk:%d\r\n", (int)(* pt_blk_offset ) );
            /*lint +e515*/
            return 0; /*找到好块,跳出*/
        }
    }

    /*已经没有好块,退出*/
    cprintf( "REB_DFT:No invalid blk fail\r\n" );
    return -1; /*返回错误*/

}

/*****************************************************************************
 函 数 名  : reb_boot_wb_check
 功能描述  : boot阶段检测回写信息
 输入参数  : 控制器地址
 输出参数  : none
 返 回 值  : none
*****************************************************************************/
void reb_boot_wb_check(void)
{
    struct nand_spec nand_info = {0};
    unsigned int idx = 0;
    int ret_val = 0;
    unsigned int blk_offset = 0, byte_offset = 0;
    char* pt_page = NULL;
    nand_wb_info wb_info = {0};
    unsigned int read_and_write_sum = 0; 
    unsigned int sick_base_offset = 0;
    unsigned int bak_base_offset = 0;
    unsigned int alloc_mem_size = 0; /*pagesize or blocksize*/

    /*获取flash信息*/
    ret_val = bsp_get_nand_info( &nand_info );
    if ( 0 != ret_val )
    {
        cprintf( "REB_DFT:get nand info fail\r\n" );
        return;
    }

    /*获取最早的一个好块*/
    ret_val = get_good_blk_offset( PART_WB_DATA, nand_info.blocksize, &blk_offset );
    if ( ret_val < 0 )
    {
        cprintf( "REB_DFT:Maybe no good blk for wb info\r\n" );
        return;
    }

    alloc_mem_size = nand_info.blocksize;
    pt_page = (char *)alloc( alloc_mem_size );
    if ( NULL == pt_page )
    {
        /*没有申请到内存128k*/
        /*lint -e515*/
        cprintf( "REB_DFT:No enough memory for %d\r\n", alloc_mem_size );
        /*lint +e515*/
        alloc_mem_size = nand_info.pagesize;
        pt_page = (char *)alloc( alloc_mem_size );
        if ( NULL == pt_page )
        {
            /*没有申请到内存2k*/
            /*lint -e515*/
            cprintf( "REB_DFT:No enough memory for %d\r\n", alloc_mem_size );
            /*lint +e515*/
            return;
        }
    }


    /*读取回写信息到pt_page*/
    byte_offset = blk_offset * nand_info.blocksize;
    ret_val = bsp_nand_read( PART_WB_DATA, byte_offset, pt_page, nand_info.pagesize, NULL );
    if ( 0 != ret_val )
    {
        /*读取回写信息失败*/
        cprintf( "REB_DFT:Read wb info fail\r\n" );
        goto free_and_ret;
    }
    /*lint -e18 -e119 -e419*/
    memcpy(&wb_info, pt_page, sizeof(nand_wb_info) );
    /*lint +e18 +e119 +e419*/
    /*检测回写信息的有效性*/
    ret_val = wb_info_valid_check( &wb_info, blk_offset, nand_info.chipsize, nand_info.blocksize );
    if ( 0 == ret_val )
    {
        cprintf( "REB_DFT:No invalid wb info magic\r\n" );
        goto free_and_ret;
    }

    /*先擦除需要回写的病危块*/
    sick_base_offset = wb_info.sick_blk_id * nand_info.blocksize;
    ret_val = bsp_nand_erase( ptable_product[wb_info.sick_mtd_id].name, sick_base_offset );
    if ( 0 != ret_val )
    {
        cprintf( "REB_DFT:Oh God! erase sick blk err\r\n" );
        goto free_and_ret;
    }

    /*进行读取备份数据*/
    read_and_write_sum = ( unsigned int )nand_info.blocksize / alloc_mem_size;
    
    bak_base_offset  = wb_info.backup_blk_id * nand_info.blocksize;
    sick_base_offset = wb_info.sick_blk_id * nand_info.blocksize;
    for ( idx = 0; idx < read_and_write_sum; idx++ )
    {
        /*从数据备份块读取数据*/
        /*lint -e119*/
        ret_val = bsp_nand_read( PART_WB_DATA, bak_base_offset,
                       pt_page, alloc_mem_size, NULL );
        /*lint +e119*/
        if ( 0 != ret_val )
        {
            /*lint -e515*/
            cprintf( "REB_DFT:Oh God! read bak blk err, idx=%d\r\n", (int)idx );
            /*lint +e515*/
            goto free_and_ret;
        }
        bak_base_offset += alloc_mem_size; /*page写偏移累加*/

        /*往病危块回写数据*/
        /*lint -e119*/
        ret_val = bsp_nand_write( ptable_product[wb_info.sick_mtd_id].name,
            sick_base_offset, pt_page, alloc_mem_size );
        /*lint +e119*/
        if ( 0 != ret_val )
        {
            /*lint -e515*/
            cprintf( "REB_DFT:Oh God! write sick blk err, idx=%d\r\n", (int)idx );
            /*lint +e515*/
            goto free_and_ret;
        }
        sick_base_offset += alloc_mem_size; /*page写偏移累加*/
    }

    /*擦除wbdata分区回写信息所在的块*/
    bak_base_offset  = wb_info.wb_info_blk_id * nand_info.blocksize;
    ret_val = bsp_nand_erase( PART_WB_DATA, bak_base_offset );
    if ( 0 != ret_val )
    {
        /*todo:标坏块*/
        cprintf( "REB_DFT:Oh God! erase wb blk err\r\n" );
        goto free_and_ret;
    }

free_and_ret:
    /*boot无需释放内存*/
    return;
}
/*****************************************************************************
 函 数 名  : reb_boot_init
 功能描述  : boot阶段初始化
 输入参数  : 控制器地址
 输出参数  : none
 返 回 值  : none
*****************************************************************************/
void reb_boot_init(void)
{
    volatile unsigned int start = 0;
    volatile unsigned int end = 0;
    unsigned int delt = 0;

    reb_reliability_nv_cfg_boot_init();

    start = get_timer_value();
#ifdef REB_BOOT_DEBUG
    cprintf( "REB_DFT: check wb info begin......\r\n" );
#endif
    /*在boot阶段不使用宏控制*/
    reb_boot_wb_check();

    end = get_timer_value();
    if ( start >= end )
    {
        delt = start - end; /*获取运行时间tick*/
    }
    else
    {
        delt = end - start; /*获取运行时间tick*/
    }
    delt = (delt * 1000) >> 15; /*timer clock is 32768 per second*/

#ifdef REB_BOOT_DEBUG
    /*lint -e515*/
    cprintf( "REB_DFT: it takes %dms, check wb info end...\r\n", (int)( delt ) );
    /*lint +e515*/
#endif
    return;
}
