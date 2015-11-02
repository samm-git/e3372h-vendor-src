/******************************************************************************

                  版权所有 (C), 2001-2014, 华为技术有限公司

*******************************************************************************
  文 件 名   : dload_nark_core.h
  版 本 号   : 初稿
  作    者   : y00216577
  生成日期   : 2014年03月01日
  最近修改   :
  功能描述   : 用户态程序与内核通信IOCTL接口实现文件，不允许修改。
  函数列表   :
  修改历史   :
*********************************问题单修改记录*******************************
  问题单号               修改人         修改时间           修改说明                             
  DTS2014090106291      c00227249      2014-09-02         同步升级归一化修改
  
                         
******************************************************************************/
#include <sys/types.h>
#include <sys/stat.h>
#include <asm/ioctl.h>
#include <linux/ioctl.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include "dload_debug.h"
#include "dload_ioctl.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************
  1 模块私有 (宏、枚举、结构体、自定义数据类型) 定义区:
******************************************************************************/
#define DLOAD_PRINT(level, fmt, ...) (dload_printf(level, "[*DLOAD_IOCTL*] %s:%d: "fmt"\n", __func__, __LINE__, ##__VA_ARGS__))
#define DLOAD_IOC_TYPE   'D'
enum
{
    DLOAD_SET_CMD  = _IOW(DLOAD_IOC_TYPE, 0, ioctl_msg_t),  /* 设置命令 */
    DLOAD_GET_CMD  = _IOR(DLOAD_IOC_TYPE, 1, ioctl_msg_t),  /* 获取命令 */
};

/******************************************************************************
  2 模块私有 (全局变量) 定义区:
******************************************************************************/
STATIC const uint8* dload_dev_name = "/dev/dload_nark";
STATIC int32        dload_nark_file = -1;

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
int32 dload_set_data(dload_module_t module_index, int32 msg_index, void* in_buffer, uint32 in_len)
{
    int32        ret = DLOAD_OK;
    ioctl_msg_t  msg_entry;

    msg_entry.module_index  = module_index;
    msg_entry.msg_index     = msg_index;
    if(NULL == in_buffer)
    {
        msg_entry.data_buffer  = NULL;
        msg_entry.data_len     = 0;
    }
    else
    {
        msg_entry.data_buffer  = in_buffer;
        msg_entry.data_len     = in_len;
    }
    
    ret = ioctl(dload_nark_file, DLOAD_SET_CMD, &msg_entry);

    return ret;
}

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
int32 dload_get_data(dload_module_t module_index, int32 msg_index, void* out_buffer, uint32 out_len)
{
    int32  ret = DLOAD_OK;
    ioctl_msg_t msg_entry;

    msg_entry.module_index   = module_index;
    msg_entry.msg_index      = msg_index;
    if(NULL == out_buffer)
    {
        msg_entry.data_buffer = NULL;
        msg_entry.data_len    = 0;
    }
    else
    {
        msg_entry.data_buffer  = out_buffer;
        msg_entry.data_len     = out_len;
    }

    if(0 > dload_nark_file)
    {
        DLOAD_PRINT(DEBUG_LEV_ERR, "file hander = 0x%x error", dload_nark_file);
        return DLOAD_ERR;
    }
    
    ret = ioctl(dload_nark_file, DLOAD_GET_CMD, &msg_entry);
    
    return ret;
}

/*******************************************************************
  函数名称  : dload_ioctl_init
  函数描述  : DLOAD IOCTL模块init函数
  函数入参  : 
  函数输出  : NA
  函数返回值: DLOAD_OK    : 成功
              DLOAD_ERR : 失败
********************************************************************/
int32 dload_ioctl_init(void)
{
    dload_nark_file = open(dload_dev_name, O_RDWR);
    if(0 > dload_nark_file)
    {
    	DLOAD_PRINT(DEBUG_LEV_ERR, "file hander = 0x%x open failed", dload_nark_file); 
        return DLOAD_ERR;
    }
    
    return DLOAD_OK;
}

/*******************************************************************
  函数名称  : dload_ioctl_exit
  函数描述  : DLOAD IOCTL模块destory函数
  函数入参  : 
  函数输出  : NA
  函数返回值: DLOAD_OK    : 成功
              DLOAD_ERR : 失败
********************************************************************/
int32 dload_ioctl_exit(void)
{
    int32 ret = DLOAD_OK;
    ret = close(dload_nark_file);
    if(0 > ret)
    {
        DLOAD_PRINT(DEBUG_LEV_ERR, "close return = %d failed", ret); 
        ret = DLOAD_ERR;
    }
    
    return ret;
}

#ifdef __cplusplus
}
#endif
