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
#ifndef __HARDWARE_TEST_H__
#define __HARDWARE_TEST_H__

#ifdef __cplusplus
extern "C"
{
#endif

/**************************************************************************
  头文件包含                            
**************************************************************************/
#include "bsp_sram.h"
/**************************************************************************
  宏定义 
**************************************************************************/

#ifndef TRUE
#define TRUE                  1
#endif
#ifndef FALSE
#define FALSE                 0
#endif

/**************************************************************************
  变量、结构定义 
**************************************************************************/
typedef enum
{
    HARDWARE_TEST_MODE_SET = 3,     /*设置模式*/
    HARDWARE_TEST_MODE_GET,         /*获取模式*/
    HARDWARE_TEST_BITFLIP_GET,      /*获取bitflip info*/
    HARDWARE_TEST_BITFLIP_CLR,      /*清除bitflip info*/
    HARDWARE_TEST_SD_DET_GET,       /*获取sd detect 引脚状态*/
    HARDWARE_TEST_SIM_DET_GET,      /*获取sim detect 引脚状态*/
    HARDWARE_TEST_CHG_STATUS_GET,   /*获取充电芯片通信状态*/
    HARDWARE_TEST_NAND_INFO_GET,    /*获取flash codewore、spare、ecc_correct大小*/    
    HARDWARE_TEST_MODE_MAX
}HARDWARE_TEST_CMD;

typedef enum
{
    HARDWARE_TEST_OPT_MEM = 0,
    HARDWARE_TEST_OPT_FLASH,
    HARDWARE_TEST_OPT_PERI,
    HARDWARE_TEST_OPT_ALL,
    FLASH_BITFLIP_OPT = 4,
    DDRTEST_OPT = 5,
    HARDWARE_TEST_OPT_MAX,
}HARDWARE_TEST_OPT;
#if 0
typedef  struct
{
    unsigned int smem_hw_mode;             /* 自检模式*/
    unsigned int smem_hw_option;           /* 测试项选择*/
    unsigned int smem_reserved;            /* 保留*/
}hwtest_smem_info;
#endif
typedef  struct
{
    unsigned int codewore_size;
    unsigned int spare_size;
    unsigned int ecc_correct_num;
}hwtest_flash_info;
/**************************************************************************
  函数定义 
**************************************************************************/

/********************************************************************
*Function    : set_hardware_test_mode
*Description : 设置硬件自检模式标记
*Input       : 
*Output      : none
*Return      : 0 :successed
               other:failed
********************************************************************/
extern int set_hardware_test_mode(hwtest_smem_info *hwtest_info);

/********************************************************************
*Function    : get_hardware_test_mode
*Description : 获取硬件自检模式标记
*Input       : 
*Output      : none
*Return      : 0 :not
               1:in hardware test mode
********************************************************************/
extern int get_hardware_test_mode(hwtest_smem_info *hwtest_info);

/********************************************************************
*Function    : bit_flip_info_record
*Description : 记录bit flip信息
*Input       : cw_id，corrected
*Output      : none
*Return      : 0 :successed
               other:failed
********************************************************************/
extern int bit_flip_info_record(unsigned int cw_id,unsigned int corrected);

extern void init_hardware_flash_info(unsigned int codewore_size, unsigned int spare_size, unsigned int ecc_correct_num);
#ifdef __cplusplus
}
#endif

#endif

