/******************************************************************************

                  版权所有 (C), 2001-2014, 华为技术有限公司

*******************************************************************************
  文 件 名   : dload_nark_core.h
  版 本 号   : 初稿
  作    者   : y00216577
  生成日期   : 2014年03月01日
  最近修改   :
  功能描述   : 对外信息声明文件
  函数列表   :
  修改历史   :
*********************************问题单修改记录******************************
  问题单号               修改人         修改时间           修改说明
  DTS2014090106291      c00227249      2014-09-02         同步升级归一化修改


******************************************************************************/
#ifndef _DLOAD_IOCTRL_H
#define _DLOAD_IOCTRL_H
/******************************************************************************
  1 其他头文件包含
******************************************************************************/
#include "dload_comm.h"

#ifdef  __cplusplus
extern "C" {
#endif

/******************************************************************************
  2 模块对外 (宏、枚举、结构体、自定义数据类型) 定义区:
******************************************************************************/
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


typedef struct _IOCRL_MSG
{
    dload_module_t       module_index;  /* 升级子模块ID */
    uint32               msg_index;     /* 子模块消息ID */
    uint32               data_len;      /* 数据缓存长度 */
    void*                data_buffer;   /* 数据缓存指针 */
} ioctl_msg_t;

/*******************************************************************
  函数名称  : dload_set_data
  函数描述  : 用户程序请求向内核侧设置数据接口
  函数入参  : module_index: 模块索引
              msg_index   : 消息索引
              in_buffer   : 数据缓存地址
              in_len      : 数据缓存长度
  函数输出  : NA
  函数返回值: TRUE  : 成功
              FALSE : 失败
********************************************************************/
int32 dload_set_data(dload_module_t module_index, int32 msg_index, void* in_buffer, uint32 in_len);

/*******************************************************************
  函数名称  : dload_get_data
  函数描述  : 用户程序请求从内核侧获取数据接口
  函数入参  : module_index: 模块ID
              msg_index   : 消息ID
              out_buffer  : 数据缓存地址
              out_len     : 数据缓存长度
  函数输出  : out_buffer  : 数据缓存地址
  函数返回值: TRUE  : 成功
              FALSE : 失败
********************************************************************/
int32 dload_get_data(dload_module_t module_index, int32 msg_index, void* out_buffer, uint32 out_len);

/*******************************************************************
  函数名称  : dload_ioctl_init
  函数描述  : DLOAD IOCTL模块init函数
  函数入参  :
  函数输出  : NA
  函数返回值: DLOAD_OK    : 成功
              DLOAD_ERR : 失败
********************************************************************/
int32 dload_ioctl_init(void);

/*******************************************************************
  函数名称  : dload_ioctl_exit
  函数描述  : DLOAD IOCTL模块destory函数
  函数入参  :
  函数输出  : NA
  函数返回值: DLOAD_OK    : 成功
              DLOAD_ERR : 失败
********************************************************************/
int32 dload_ioctl_exit(void);

#ifdef __cplusplus
}
#endif
#endif /* _DLOAD_IOCTRL_H */
