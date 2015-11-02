/******************************************************************************/
/*  Copyright (C), 2007-2013, Hisilicon Technologies Co., Ltd. */
/******************************************************************************/
/* File name     : hi_efuse.h */
/* Version       : 2.0 */
/* Author        : x00195528*/
/* Created       : 2013-02-27*/
/* Last Modified : */
/* Description   :  The C union definition file for the module efuse*/
/* Function List : */
/* History       : */
/* 1 Date        : */
/* Author        : x00195528*/
/* Modification  : Create file */
/******************************************************************************/

#ifndef __HI_EFUSE_H__
#define __HI_EFUSE_H__

/*
 * Project: hi
 * Module : efuse
 */
#include <hi_base.h>

#ifndef HI_SET_GET
#define HI_SET_GET(a0,a1,a2,a3,a4)
#endif

/* efuse_nandc support 2 group nandc info */
#define CONFIG_EFUSE_NANDC_GROUP2


#define EFUSE_MAX_SIZE          (16)

#define EFUSE_GRP_DIEID         (8)
#define EFUSE_DIEID_SIZE        (5)
#define EFUSE_DIEID_BIT         (27)
#define EFUSE_DIEID_LEN         (EFUSE_DIEID_SIZE * EFUSE_GROUP_SIZE)

#define EFUSE_GRP_HUK           (4)
#define EFUSE_HUK_SIZE          (4)
#define EFUSE_HUK_LEN           (EFUSE_HUK_SIZE * EFUSE_GROUP_SIZE)

#define EFUSE_COUNT_CFG     (5)
#define PGM_COUNT_CFG       (HI_APB_CLK / 1000000 * 12 - EFUSE_COUNT_CFG)

#define EFUSE_GROUP_ID_FOR_NANDC    (15)
/********************************************************************************/
/*    efuse 寄存器偏移定义（项目名_模块名_寄存器名_OFFSET)        */
/********************************************************************************/
#define    HI_EFUSEC_CFG_OFFSET                              (0x0) /* 配置寄存器，用于使能读/烧写流程，当读完成时，逻辑自动将RDn清除为0。当烧写完成时，逻辑自动将PGEn清除为0。. */
#define    HI_EFUSEC_STATUS_OFFSET                           (0x4) /* 状态寄存器，用于表述读/烧写状态 */
#define    HI_EFUSE_GROUP_OFFSET                             (0x8) /* 读取/烧写地址寄存器。将eufse分组，每组为32bit，对efuse进行烧写或者读取的时候，以一个group为单位 */
#define    HI_PG_VALUE_OFFSET                                (0xC) /* 每次的烧写值寄存器 */
#define    HI_EFUSEC_COUNT_OFFSET                            (0x10) /* efuse内部状态跳转计数器值寄存器。同时该值乘以4作为efuse读操作期间strobe信号脉冲宽度计数值 */
#define    HI_PGM_COUNT_OFFSET                               (0x14) /* 一次烧写期间strobe信号拉高时间计数器 */
#define    HI_EFUSEC_DATA_OFFSET                             (0x18) /* 存放软件从efuse读取的数据寄存器 */
#define    HI_HW_CFG_OFFSET                                  (0x1C) /* 存放efuse上电解复位后从group==127读取的数据寄存器，用于启动判断等。初始值由用户通过efuse烧写确定 */

/********************************************************************************/
/*    efuse 寄存器定义（项目名_模块名_寄存器名_T)        */
/********************************************************************************/
typedef union
{
    struct
    {
        unsigned int    pgen                       : 1; /* [0..0] 烧写使能信号，当一次烧写完成后，逻辑将此为自动清零。0：不使能1：使能 */
        unsigned int    pre_pg                     : 1; /* [1..1] 预烧写使能信号，使能后硬件拉低PGENB信号。0：不使能1：使能 */
        unsigned int    rden                       : 1; /* [2..2] 读使能信号，当一次读完成后，逻辑将此为自动清零。0：不使能1：使能 */
        unsigned int    aib_sel                    : 1; /* [3..3] AIB接口选择信号。0：选择AIB操作efuse控制器1：选择APB操作efuse控制器 */
        unsigned int    rr_en                      : 1; /* [4..4] efsue冗余控制标志。0：正常模式1：冗余模式 */
        unsigned int    pd_en                      : 1; /* [5..5] efuse power-down控制。0：不使能1：使能 */
        unsigned int    mr_en                      : 1; /* [6..6] efuse Margin读模式控制。0：不使能1：使能 */
        unsigned int    undefined                  : 25; /* [31..7]  */
    } bits;
    unsigned int    u32;
}HI_EFUSEC_CFG_T;    /* 配置寄存器，用于使能读/烧写流程，当读完成时，逻辑自动将RDn清除为0。当烧写完成时，逻辑自动将PGEn清除为0。. */

typedef union
{
    struct
    {
        unsigned int    pg_status                  : 1; /* [0..0] 烧写状态。0：未完成1：完成 */
        unsigned int    rd_status                  : 1; /* [1..1] efuse读状态0：未完成1：完成一次读操作 */
        unsigned int    pgenb_status               : 1; /* [2..2] 预烧写置位完成状态。0：未完成1：完成预烧写置位状态 */
        unsigned int    rd_error                   : 1; /* [3..3] efuse当前group地址是否是禁止的。0：否1：是 */
        unsigned int    pd_status                  : 1; /* [4..4] power-down状态。0：正常状态1：power-down状态 */
        unsigned int    undefined                  : 27; /* [31..5]  */
    } bits;
    unsigned int    u32;
}HI_EFUSEC_STATUS_T;    /* 状态寄存器，用于表述读/烧写状态 */

typedef union
{
    struct
    {
        unsigned int    efuse_group                : 7; /* [6..0] 读取/烧写地址group, */
        unsigned int    undefined                  : 25; /* [31..7]  */
    } bits;
    unsigned int    u32;
}HI_EFUSE_GROUP_T;    /* 读取/烧写地址寄存器。将eufse分组，每组为32bit，对efuse进行烧写或者读取的时候，以一个group为单位 */

typedef union
{
    struct
    {
        unsigned int    pg_value                   : 32; /* [31..0] 一组32bit的烧写信息0：不烧写该bit；1：烧写该bit； */
    } bits;
    unsigned int    u32;
}HI_PG_VALUE_T;    /* 每次的烧写值寄存器 */

typedef union
{
    struct
    {
        unsigned int    efusec_count               : 8; /* [7..0] EFUSE内部状态跳转使用的计数值。 */
        unsigned int    undefined                  : 24; /* [31..8]  */
    } bits;
    unsigned int    u32;
}HI_EFUSEC_COUNT_T;    /* efuse内部状态跳转计数器值寄存器。同时该值乘以4作为efuse读操作期间strobe信号脉冲宽度计数值 */

typedef union
{
    struct
    {
        unsigned int    pgm_count                  : 16; /* [15..0] 一次烧写期间strobe信号拉高时间计数器（以参考时钟为计数时钟） */
        unsigned int    undefined                  : 16; /* [31..16]  */
    } bits;
    unsigned int    u32;
}HI_PGM_COUNT_T;    /* 一次烧写期间strobe信号拉高时间计数器 */

typedef union
{
    struct
    {
        unsigned int    efusec_data                : 32; /* [31..0] 存放软件从efuse读取的数据 */
    } bits;
    unsigned int    u32;
}HI_EFUSEC_DATA_T;    /* 存放软件从efuse读取的数据寄存器 */

typedef union
{
    struct
    {
        unsigned int    pgm_disable                : 1; /* [0..0] 0：64页1：128页 */
        unsigned int    pad_disable                : 1; /* [1..1] 0：64页1：128页 */
        unsigned int    jtag_en                    : 1; /* [2..2] 0：64页1：128页 */
        unsigned int    boot_sel                   : 1; /* [3..3] 0：64页1：128页 */
        unsigned int    secboot_en                 : 1; /* [4..4] 0：64页1：128页 */
        unsigned int    nf_ctrl_ena0               : 1; /* [5..5] 控制第一组nand信息从管脚还是efuse中读取0：nandc从管脚读取nand flash配置参数1：nandc从efuse中读取nand falsh配置参数 */
        unsigned int    nf_block_size1             : 1; /* [6..6] 0：64页1：128页 */
        unsigned int    nf_ecc_type1               : 2; /* [8..7] 00：无ECC01：1bitECC/4bitECC11：8bitECC10：24bitECC */
        unsigned int    nf_page_size1              : 2; /* [10..9] 00：page size 512字节01：page size 2K字节11：page size 4K字节10：page size 8K字节 */
        unsigned int    nf_addr_num1               : 1; /* [11..11] 0：4周期地址1：5周期地址 */
        unsigned int    nf_ctrl_ena1               : 1; /* [12..12] 控制第二组nand信息从管脚还是efuse中读取0：nandc从管脚读取nand flash配置参数1：nandc从efuse中读取nand falsh配置参数 */
        unsigned int    reserved_2                 : 3; /* [15..13]  */
        unsigned int    nf_block_size              : 1; /* [16..16] 0：64页1：128页 */
        unsigned int    nf_ecc_type                : 2; /* [18..17] 00：无ECC01：1bitECC/4bitECC11：8bitECC10：24bitECC */
        unsigned int    reserved_1                 : 1; /* [19..19]  */
        unsigned int    nf_page_size               : 2; /* [21..20] 00：page size 512字节01：page size 2K字节11：page size 4K字节10：page size 8K字节 */
        unsigned int    nf_addr_num                : 1; /* [22..22] 0：4周期地址1：5周期地址 */
        unsigned int    reserved_0                 : 2; /* [24..23] 0：nandc从管脚读取nand flash配置参数1：nandc从efuse中读取nand falsh配置参数 */
        unsigned int    nf_ctrl_ena                : 1; /* [25..25] V721中不使用0：nandc从管脚读取nand flash配置参数1：nandc从efuse中读取nand falsh配置参数 */
        unsigned int    undefined                  : 6; /* [31..26]  */
    } bits;
    unsigned int    u32;
}HI_HW_CFG_T;    /* 存放efuse上电解复位后从group==127读取的数据寄存器，用于启动判断等。初始值由用户通过efuse烧写确定 */


/********************************************************************************/
/*    efuse 函数（项目名_模块名_寄存器名_成员名_set)        */
/********************************************************************************/
HI_SET_GET(hi_efusec_cfg_pgen,pgen,HI_EFUSEC_CFG_T,HI_EFUSE_BASE_ADDR, HI_EFUSEC_CFG_OFFSET)
HI_SET_GET(hi_efusec_cfg_pre_pg,pre_pg,HI_EFUSEC_CFG_T,HI_EFUSE_BASE_ADDR, HI_EFUSEC_CFG_OFFSET)
HI_SET_GET(hi_efusec_cfg_rden,rden,HI_EFUSEC_CFG_T,HI_EFUSE_BASE_ADDR, HI_EFUSEC_CFG_OFFSET)
HI_SET_GET(hi_efusec_cfg_aib_sel,aib_sel,HI_EFUSEC_CFG_T,HI_EFUSE_BASE_ADDR, HI_EFUSEC_CFG_OFFSET)
HI_SET_GET(hi_efusec_cfg_rr_en,rr_en,HI_EFUSEC_CFG_T,HI_EFUSE_BASE_ADDR, HI_EFUSEC_CFG_OFFSET)
HI_SET_GET(hi_efusec_cfg_pd_en,pd_en,HI_EFUSEC_CFG_T,HI_EFUSE_BASE_ADDR, HI_EFUSEC_CFG_OFFSET)
HI_SET_GET(hi_efusec_cfg_mr_en,mr_en,HI_EFUSEC_CFG_T,HI_EFUSE_BASE_ADDR, HI_EFUSEC_CFG_OFFSET)
HI_SET_GET(hi_efusec_cfg_undefined,undefined,HI_EFUSEC_CFG_T,HI_EFUSE_BASE_ADDR, HI_EFUSEC_CFG_OFFSET)
HI_SET_GET(hi_efusec_status_pg_status,pg_status,HI_EFUSEC_STATUS_T,HI_EFUSE_BASE_ADDR, HI_EFUSEC_STATUS_OFFSET)
HI_SET_GET(hi_efusec_status_rd_status,rd_status,HI_EFUSEC_STATUS_T,HI_EFUSE_BASE_ADDR, HI_EFUSEC_STATUS_OFFSET)
HI_SET_GET(hi_efusec_status_pgenb_status,pgenb_status,HI_EFUSEC_STATUS_T,HI_EFUSE_BASE_ADDR, HI_EFUSEC_STATUS_OFFSET)
HI_SET_GET(hi_efusec_status_rd_error,rd_error,HI_EFUSEC_STATUS_T,HI_EFUSE_BASE_ADDR, HI_EFUSEC_STATUS_OFFSET)
HI_SET_GET(hi_efusec_status_pd_status,pd_status,HI_EFUSEC_STATUS_T,HI_EFUSE_BASE_ADDR, HI_EFUSEC_STATUS_OFFSET)
HI_SET_GET(hi_efusec_status_undefined,undefined,HI_EFUSEC_STATUS_T,HI_EFUSE_BASE_ADDR, HI_EFUSEC_STATUS_OFFSET)
HI_SET_GET(hi_efuse_group_efuse_group,efuse_group,HI_EFUSE_GROUP_T,HI_EFUSE_BASE_ADDR, HI_EFUSE_GROUP_OFFSET)
HI_SET_GET(hi_efuse_group_undefined,undefined,HI_EFUSE_GROUP_T,HI_EFUSE_BASE_ADDR, HI_EFUSE_GROUP_OFFSET)
HI_SET_GET(hi_pg_value_pg_value,pg_value,HI_PG_VALUE_T,HI_EFUSE_BASE_ADDR, HI_PG_VALUE_OFFSET)
HI_SET_GET(hi_efusec_count_efusec_count,efusec_count,HI_EFUSEC_COUNT_T,HI_EFUSE_BASE_ADDR, HI_EFUSEC_COUNT_OFFSET)
HI_SET_GET(hi_efusec_count_undefined,undefined,HI_EFUSEC_COUNT_T,HI_EFUSE_BASE_ADDR, HI_EFUSEC_COUNT_OFFSET)
HI_SET_GET(hi_pgm_count_pgm_count,pgm_count,HI_PGM_COUNT_T,HI_EFUSE_BASE_ADDR, HI_PGM_COUNT_OFFSET)
HI_SET_GET(hi_pgm_count_undefined,undefined,HI_PGM_COUNT_T,HI_EFUSE_BASE_ADDR, HI_PGM_COUNT_OFFSET)
HI_SET_GET(hi_efusec_data_efusec_data,efusec_data,HI_EFUSEC_DATA_T,HI_EFUSE_BASE_ADDR, HI_EFUSEC_DATA_OFFSET)
HI_SET_GET(hi_hw_cfg_pgm_disable,pgm_disable,HI_HW_CFG_T,HI_EFUSE_BASE_ADDR, HI_HW_CFG_OFFSET)
HI_SET_GET(hi_hw_cfg_pad_disable,pad_disable,HI_HW_CFG_T,HI_EFUSE_BASE_ADDR, HI_HW_CFG_OFFSET)
HI_SET_GET(hi_hw_cfg_jtag_en,jtag_en,HI_HW_CFG_T,HI_EFUSE_BASE_ADDR, HI_HW_CFG_OFFSET)
HI_SET_GET(hi_hw_cfg_boot_sel,boot_sel,HI_HW_CFG_T,HI_EFUSE_BASE_ADDR, HI_HW_CFG_OFFSET)
HI_SET_GET(hi_hw_cfg_secboot_en,secboot_en,HI_HW_CFG_T,HI_EFUSE_BASE_ADDR, HI_HW_CFG_OFFSET)
HI_SET_GET(hi_hw_cfg_nf_ctrl_ena0,nf_ctrl_ena0,HI_HW_CFG_T,HI_EFUSE_BASE_ADDR, HI_HW_CFG_OFFSET)
HI_SET_GET(hi_hw_cfg_nf_block_size1,nf_block_size1,HI_HW_CFG_T,HI_EFUSE_BASE_ADDR, HI_HW_CFG_OFFSET)
HI_SET_GET(hi_hw_cfg_nf_ecc_type1,nf_ecc_type1,HI_HW_CFG_T,HI_EFUSE_BASE_ADDR, HI_HW_CFG_OFFSET)
HI_SET_GET(hi_hw_cfg_nf_page_size1,nf_page_size1,HI_HW_CFG_T,HI_EFUSE_BASE_ADDR, HI_HW_CFG_OFFSET)
HI_SET_GET(hi_hw_cfg_nf_addr_num1,nf_addr_num1,HI_HW_CFG_T,HI_EFUSE_BASE_ADDR, HI_HW_CFG_OFFSET)
HI_SET_GET(hi_hw_cfg_nf_ctrl_ena1,nf_ctrl_ena1,HI_HW_CFG_T,HI_EFUSE_BASE_ADDR, HI_HW_CFG_OFFSET)
HI_SET_GET(hi_hw_cfg_reserved_2,reserved_2,HI_HW_CFG_T,HI_EFUSE_BASE_ADDR, HI_HW_CFG_OFFSET)
HI_SET_GET(hi_hw_cfg_nf_block_size,nf_block_size,HI_HW_CFG_T,HI_EFUSE_BASE_ADDR, HI_HW_CFG_OFFSET)
HI_SET_GET(hi_hw_cfg_nf_ecc_type,nf_ecc_type,HI_HW_CFG_T,HI_EFUSE_BASE_ADDR, HI_HW_CFG_OFFSET)
HI_SET_GET(hi_hw_cfg_reserved_1,reserved_1,HI_HW_CFG_T,HI_EFUSE_BASE_ADDR, HI_HW_CFG_OFFSET)
HI_SET_GET(hi_hw_cfg_nf_page_size,nf_page_size,HI_HW_CFG_T,HI_EFUSE_BASE_ADDR, HI_HW_CFG_OFFSET)
HI_SET_GET(hi_hw_cfg_nf_addr_num,nf_addr_num,HI_HW_CFG_T,HI_EFUSE_BASE_ADDR, HI_HW_CFG_OFFSET)
HI_SET_GET(hi_hw_cfg_reserved_0,reserved_0,HI_HW_CFG_T,HI_EFUSE_BASE_ADDR, HI_HW_CFG_OFFSET)
HI_SET_GET(hi_hw_cfg_nf_ctrl_ena,nf_ctrl_ena,HI_HW_CFG_T,HI_EFUSE_BASE_ADDR, HI_HW_CFG_OFFSET)
HI_SET_GET(hi_hw_cfg_undefined,undefined,HI_HW_CFG_T,HI_EFUSE_BASE_ADDR, HI_HW_CFG_OFFSET)

/* 检查efuse是否允许被烧写bit位 */
static __inline__ unsigned int hi_efuse_get_pgm_disable_flag(void)
{
    return get_hi_hw_cfg_pgm_disable();
}

#endif // __HI_EFUSE_H__

