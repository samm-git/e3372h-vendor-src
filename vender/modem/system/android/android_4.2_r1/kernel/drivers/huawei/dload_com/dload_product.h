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
#ifndef _DLOAD_PRODUCT_H
#define _DLOAD_PRODUCT_H
/******************************************************************************
  1 其他头文件包含:
******************************************************************************/
#include "dload_comm.h"

#ifdef  __cplusplus
    extern "C"{
#endif

/******************************************************************************
  2 模块对外 (宏、枚举、结构体、自定义数据类型) 定义区:
******************************************************************************/


/*******************************************************************
  函数名称  : uint32  get_product_mark(void)
  函数描述  : 获取产品配置信息
  函数入参  : NA
  函数输出  : NA
  函数返回值: 产品信息结构
********************************************************************/
product_info_t*  get_product_info(void);

/*******************************************************************
  函数名称  : get_show_path_info
  函数描述  : 获取path 和path_index转换的数组
  函数入参  : NA
  函数输出  : pppath_info_st : pathinfo_st 数组的地址
                            info_size: pathinfo_st的size
  函数返回值: DLOAD_OK    : 成功
              DLOAD_ERR   : 失败
********************************************************************/
uint32  get_show_path_info(path_info_t **pppath_info_st, uint32 *info_size);


/*******************************************************************
  函数名称  : get_show_info
  函数描述  : 获取lcd/led配置信息
  函数入参  : NA
  函数输出  : ppupstate_info : upstateinfo_st 数组的地址
                            info_size: upstateinfo_st的size
  函数返回值:DLOAD_OK    : 成功
              DLOAD_ERR   : 失败
********************************************************************/
uint32  get_show_info(upstate_info_t **ppupstate_info, uint32 *info_size);

/*******************************************************************
  函数名称  : get_dload_feature
  函数描述  : 获取升级特性配置信息
  函数入参  : NA
  函数输出  : NA
  函数返回值: 产品信息结构
********************************************************************/
dload_feature_t*  get_dload_feature(void);


#ifdef  __cplusplus
    }
#endif
#endif /*_DLOAD_PRODUCT_H */
