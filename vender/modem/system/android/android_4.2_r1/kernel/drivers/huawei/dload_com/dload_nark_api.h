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
#ifndef _DLOAD_NARK_API_H
#define _DLOAD_NARK_API_H
/******************************************************************************
  1 其他头文件包含
******************************************************************************/
#include "dload_comm.h"

#ifdef  __cplusplus
    extern "C"{
#endif

/******************************************************************************
  2 模块对外 (宏、枚举、结构体、自定义数据类型) 定义区:
******************************************************************************/
#define MAP_INDEX(X, Y)                ((0XFF00 & (X << 8)) | (0X00FF & Y)) 
typedef int32  (*msg_func_t)(uint8 *, uint32);

/* 升级通信各子模块ID定义 */
typedef enum _DLOAD_MODULE
{
    DLOAD_MAIN_MODULE,    /* 升级主模块 */
    DLOAD_SHOW_MODULE,    /* 升级显示模块 */
    DLOAD_USB_MODULE,     /* 一键升级模块 */
    DLOAD_ONLINE_MODULE,  /* 在线升级模块 */
    DLOAD_SCARD_MODULE,   /* SCARD升级模块 */
    DLOAD_HOTA_MODULE,    /* HOTA升级模块 */
    DLOAD_FOTA_MODULE,    /* FOTA升级模块 */
    DLOAD_FLASH_MODULE,   /* FLASH操作模块 */
    DLOAD_AT_MODULE,      /* 升级AT模块 */
}dload_module_t;

/* 交互消息ID定义列表 */
enum _MAIN_MODULE_MSG_ID
{
    MTD_FLUSH_MSG_ID ,           /* 刷新MTD设备 */
    GET_BAT_LEVEL_MSG_ID,           /* 获取电池电量格数 */
    GET_PLATFORMINFO_MSG_ID,    /* 获取平台信息 */
    VISIT_DLOAD_SMEM_MSG_ID,    /* 共享内存访问消息ID*/
    GET_MTD_FS_TYPE_MSG_ID,     /* 获取MTD文件系统类型 */
    GET_DLOAD_FEATURE_MSG_ID,   /* 获取升级特性信息 */
    GET_PATH_INFO_SIZE_MSG_ID,  /* 获取显示模块路径映射表大小 */
    GET_PATH_INFO_MSG_ID,        /* 获取显示模块路径映射表 */
    GET_SHOW_INFO_SIZE_MSG_ID,  /* 获取显示模块运行列表大小 */
    GET_SHOW_INFO_MSG_ID,        /* 获取显示模块运行列表 */
};


/* 升级共享内存结构体 */
typedef struct _DLOAD_SMEM_INFO
{
    void*  dload_mode_addr;           /* 标记是否进入下载模式 */
    void*  sd_upgrade_addr;           /* 记录SD卡升级标识位 */
    void*  usbport_switch_addr;      /* 标识NV自动恢复阶段，启动时USB端口形态设置*/
    void*  app_inactivate_addr;      /* 记录 在线升级、SD卡升级 NV自动恢复阶段，APP不启动 */
    void*  force_mode_addr;           /* 标记是否进入逃生模式或可靠性自动恢复模式 */
    void*  ut_dload_addr;             /* 多台加载升级标识*/        
    void*  multiupg_dload_addr;      /* 组播升级标识*/                
    void*  dload_times_addr;           /*升级次数标识*/
    void*  smem_reserve6;
    void*  smem_reserve5;
    void*  smem_reserve4;
    void*  smem_reserve3;
    void*  smem_reserve2;
    void*  smem_reserve1;
}dload_smem_info_t;
/*******************************************************************
  函数名称  : search_map_callback
  函数描述  : 根据映射索引查找回调函数
  函数入参  : msg_map_index : 映射索引
  函数输出  : NV
  函数返回值: msg_func_t    : 回调函数
********************************************************************/
msg_func_t search_map_callback(uint32  msg_map_index);

/*******************************************************************
  函数名称  : nark_api_init
  函数描述  : API模块初始化
  函数入参  : NA
  函数输出  : NA
  函数返回值: DLOAD_OK    : 成功
              DLOAD_ERR   : 失败
********************************************************************/
int32 nark_api_init(void);

/*******************************************************************
  函数名称  : visit_dload_smem
  函数描述  : 共享内存访问接口
  函数入参  : 
  函数输出  : NA
  函数返回值: TRUE  : 成功
              FALSE : 失败
********************************************************************/
int32 visit_dload_smem(uint8 * data_buffer, uint32 data_len);

/*******************************************************************
  函数名称  : get_smem_info
  函数描述  : 对外提供获取共享内存接口
  函数入参  : NA
  函数输出  : NA
  函数返回值: 共享内存指针
********************************************************************/
dload_smem_info_t* get_smem_info(void);
#ifdef __cplusplus
}
#endif
#endif /* _DLOAD_NARK_API_H */
