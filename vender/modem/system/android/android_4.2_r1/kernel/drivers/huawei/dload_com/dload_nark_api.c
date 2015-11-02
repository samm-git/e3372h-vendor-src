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
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/syscalls.h>
#include <asm/uaccess.h>;
#include "dload_product.h"
#include "dload_mtd_adapter.h"
#include "dload_nark_api.h"
#include "bsp_sram.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************
  1 模块私有 (宏、枚举、结构体、自定义数据类型) 定义区:
******************************************************************************/
#define DLOAD_PRINT(level, fmt, ...)  (printk(level"[*DLOAD_NARK_API*] %s:%d: "fmt"\n", __func__, __LINE__, ##__VA_ARGS__))

/* 消息映射结构定义 */
typedef struct _IOCRL_MSG_MAP
{
    uint32      map_index;  /* 映射索引ID */
    msg_func_t  msg_func;   /* 映射处理函数 */
} ioctl_map_t;

/******************************************************************************
  2 模块私有 (全局变量) 定义区:
******************************************************************************/
/* 平台信息 */
STATIC platform_info_t      platinfo_st;
/* 共享内存 */
STATIC dload_smem_info_t    smem_info_st;


/*SD卡升级，充电提供的获取电池格数的设备节点*/
#define DLOAD_CHG_DEVNAME  "/proc/dload_chg"


/****************************************************************************
  Function:         dload_get_charge_voltage
  Description:      获取电池当前电量值
  Input:            ret_volt 获取的电池电量值
  Output:           void
  Return:           电池当前电量格数
  Others:           NA
****************************************************************************/
STATIC int32 dload_get_charge_voltage(uint32 *ret_volt)
{
    int32 fd = -1;
    int8 read_buf = '0';
    int32 len = -1;
    mm_segment_t fs_old = get_fs();

    if(NULL == ret_volt)
    {
        DLOAD_PRINT(KERN_ERR, "dload_get_charge_voltage para error.");
        return DLOAD_ERR;
    }
    
    /*改变内核访问权限 */
    set_fs(KERNEL_DS);

    fd = sys_open(DLOAD_CHG_DEVNAME, O_RDONLY, (S_IRUSR | S_IRGRP));
    if(0 > fd)
    {
        DLOAD_PRINT(KERN_ERR,"dload_get_charge_voltage open %s error.", DLOAD_CHG_DEVNAME);
        set_fs(fs_old);
        return DLOAD_ERR;
    }

    /*获取电池当前电量值 */
    len = sys_read(fd,&read_buf,sizeof(read_buf));
    if(sizeof(read_buf) != len)
    {
        DLOAD_PRINT(KERN_ERR, "read %s chg error.", DLOAD_CHG_DEVNAME);
        set_fs(fs_old);
        sys_close(fd);
        return DLOAD_ERR;
    }

    *ret_volt = (uint8)(read_buf - '0'); 
    DLOAD_PRINT(KERN_ERR, "get chg volt ret_volt = %lu !", *ret_volt);
    set_fs(fs_old);
    sys_close(fd);
    return DLOAD_OK;
}

/*******************************************************************
  函数名称  : get_platform_information
  函数描述  : 获取平台信息
  函数入参  : data_buffer : 数据缓存指针
              data_len    : 数据长度
  函数输出  : NA
  函数返回值: DLOAD_OK    : 成功
              DLOAD_ERR   : 失败
********************************************************************/
STATIC int32 get_platform_information(uint8 * data_buffer, uint32 data_len)
{    
    memcpy((void *)data_buffer, (void *)&platinfo_st, sizeof(platform_info_t));
    
    return DLOAD_OK;
}

/*******************************************************************
  函数名称  : get_battery_level
  函数描述  : 获取电池电量格数
  函数入参  : data_buffer : 数据缓存指针
              data_len    : 数据长度
  函数输出  : NA
  函数返回值: DLOAD_OK    : 成功
              DLOAD_ERR : 失败
********************************************************************/
STATIC int32 get_battery_level(uint8 * data_buffer, uint32 data_len)
{
    uint32  bat_volt_level = -1;
    int32   ret = DLOAD_ERR;
    
    ret = dload_get_charge_voltage(&bat_volt_level);
    if(DLOAD_ERR == ret)
    {
        DLOAD_PRINT(KERN_ERR, "get_battery_level error.");
        return  DLOAD_ERR;
    }
    
    memcpy((void *)data_buffer, (void *)&bat_volt_level, data_len);
    
    return DLOAD_OK;
}

/*******************************************************************
  函数名称  : get_mtd_fs_type
  函数描述  : 获取MTD设备文件系统类型
  函数入参  : data_buffer : 数据缓存指针
              data_len    : 数据长度
  函数输出  : NA
  函数返回值: DLOAD_OK    : 成功
              DLOAD_ERR   : 失败
********************************************************************/
STATIC int32 get_mtd_fs_type(uint8 * data_buffer, uint32 data_len)
{
    mtd_fs_type fs_type = MTD_UNKNOW_FS;

    DLOAD_PRINT(KERN_DEBUG, "mtd name = %s.", data_buffer);

    fs_type = check_mtdchar_fs_part(data_buffer);

    return fs_type;
}


/*******************************************************************
  函数名称  : get_show_path_info_size
  函数描述  :   获取path_info空间大小
  函数入参  : data_buffer : 数据缓存指针
              data_len    : 数据长度
  函数输出  : NA
  函数返回值: DLOAD_OK    : 成功
              DLOAD_ERR   : 失败
********************************************************************/
STATIC int32 get_show_path_info_size(uint8 * data_buffer, uint32 data_len)
{
    uint32 info_size = 0;

    DLOAD_PRINT(KERN_DEBUG, "fetch path list size.");

    if(sizeof(info_size) != data_len)
    {
        DLOAD_PRINT(KERN_ERR, "data_len = %lu not suitable.", data_len);
        return DLOAD_ERR;
    }

    get_show_path_info(NULL, &info_size);

    memcpy((void *)data_buffer, (void *)&info_size, sizeof(info_size));
    
    return DLOAD_OK;
}


/*******************************************************************
  函数名称  : get_show_information
  函数描述  : 获取path_info 
  函数入参  : data_buffer : 数据缓存指针
              data_len    : 数据长度
  函数输出  : NA
  函数返回值: DLOAD_OK    : 成功
              DLOAD_ERR   : 失败
********************************************************************/
STATIC int32 get_show_path_information(uint8 * data_buffer, uint32 data_len)
{
    path_info_t *ppath_info = NULL;

    DLOAD_PRINT(KERN_DEBUG, "fetch path list.");
    
    get_show_path_info(&ppath_info, NULL);

    memcpy((void *)data_buffer, (void *)ppath_info, data_len);
    
    return DLOAD_OK;
}


/*******************************************************************
  函数名称  : get_show_info_size
  函数描述  :   获取LCD/LED配置占的空间大小
  函数入参  : data_buffer : 数据缓存指针
              data_len    : 数据长度
  函数输出  : NA
  函数返回值: DLOAD_OK    : 成功
              DLOAD_ERR   : 失败
********************************************************************/
STATIC int32 get_show_info_size(uint8 * data_buffer, uint32 data_len)
{
    uint32 info_size = 0;

    DLOAD_PRINT(KERN_DEBUG, "fetch run state list size.");

    if(sizeof(info_size) != data_len)
    {
        DLOAD_PRINT(KERN_ERR, "data_len = %lu not suitable.", data_len);
        return DLOAD_ERR;
    }

    get_show_info(NULL, &info_size);

    memcpy((void *)data_buffer, (void *)&info_size, sizeof(info_size));
    
    return DLOAD_OK;
}


/*******************************************************************
  函数名称  : get_show_information
  函数描述  : 获取升级时 LCD/LED 的配置 
  函数入参  : data_buffer : 数据缓存指针
              data_len    : 数据长度
  函数输出  : NA
  函数返回值: DLOAD_OK    : 成功
              DLOAD_ERR   : 失败
********************************************************************/
STATIC int32 get_show_information(uint8 * data_buffer, uint32 data_len)
{
    upstate_info_t *pupstate_info = NULL;

    DLOAD_PRINT(KERN_DEBUG, "fetch run state list.");
    
    get_show_info(&pupstate_info, NULL);

    memcpy((void *)data_buffer, (void *)pupstate_info, data_len);
    
    return DLOAD_OK;
}


/*******************************************************************
  函数名称  : dload_mtd_flush
  函数描述  : 根据最新的分区表刷新MTD设备
  函数入参  : 
  函数输出  : NA
  函数返回值: DLOAD_OK    : 成功
              DLOAD_ERR : 失败
********************************************************************/
STATIC int32 dload_mtd_flush(uint8 * data_buffer, uint32 data_len)
{
    return dload_flush_partion();
}

/*******************************************************************
  函数名称  : visit_dload_smem
  函数描述  : 共享内存访问接口
  函数入参  : 
  函数输出  : NA
  函数返回值: TRUE  : 成功
              FALSE : 失败
********************************************************************/
int32 visit_dload_smem(uint8 * data_buffer, uint32 data_len)
{
    uint32 visit_flag;
    uint32 visit_value;

    memcpy((void *)&visit_flag, (void *)data_buffer, data_len);
    DLOAD_PRINT(KERN_DEBUG, "dload visit smem  vist = %lu.", visit_flag);
    
    switch(visit_flag)
    {
        case SET_SMEM_DLOAD_MODE_MAGIC:
            {
                if(NULL == smem_info_st.dload_mode_addr)
                {
                    DLOAD_PRINT(KERN_ERR, "smem_info_st.dload_mode_addr is NULL.");
                    return DLOAD_ERR;
                }
                
                *((uint32 *)(smem_info_st.dload_mode_addr)) = SMEM_DLOAD_FLAG_NUM;
            }
            break;
        case SET_SMEM_DLOAD_MODE_ZERO:
            {
                if(NULL == smem_info_st.dload_mode_addr)
                {
                    DLOAD_PRINT(KERN_ERR, "smem_info_st.dload_mode_addr is NULL.");
                    return DLOAD_ERR;
                }
                
                *((uint32 *)(smem_info_st.dload_mode_addr)) = 0;
#if ( FEATURE_ON == MBB_MLOG)  
                *((uint32 *)(smem_info_st.dload_times_addr)) = 1;
#endif
            }
            break;
        case GET_SMEM_IS_DLOAD_MODE:
            {
               if(NULL == smem_info_st.dload_mode_addr)
                {
                    DLOAD_PRINT(KERN_ERR, "smem_info_st.dload_mode_addr is NULL.");
                    return DLOAD_ERR;
                }
                
                visit_value = *((uint32 *)(smem_info_st.dload_mode_addr));
                if(SMEM_DLOAD_FLAG_NUM == visit_value)
                {
                    return DLOAD_OK;
                } 
                else
                {
                    return DLOAD_ERR;
                }
            }
            break;
        case SET_SMEM_SCARD_DLOAD_MAGIC:
            {
                if(NULL == smem_info_st.sd_upgrade_addr)
                {
                    DLOAD_PRINT(KERN_ERR, "smem_info_st.sd_upgrade_addr is NULL.");
                    return DLOAD_ERR;
                }
                
                *((uint32 *)(smem_info_st.sd_upgrade_addr)) = SMEM_SDUP_FLAG_NUM;
            }
            break;
        case SET_SMEM_SCARD_DLOAD_ZERO:
            {
                if(NULL == smem_info_st.sd_upgrade_addr)
                {
                    DLOAD_PRINT(KERN_ERR, "smem_info_st.sd_upgrade_addr is NULL.");
                    return DLOAD_ERR;
                }
                
                *((uint32 *)(smem_info_st.sd_upgrade_addr)) = 0;
            }
            break;
        case GET_SMEM_IS_SCARD_DLOAD:
            {
                if(NULL == smem_info_st.sd_upgrade_addr)
                {
                    DLOAD_PRINT(KERN_ERR, "smem_info_st.sd_upgrade_addr is NULL.");
                    return DLOAD_ERR;
                }
                
                visit_value = *((uint32 *)(smem_info_st.sd_upgrade_addr));
                if(SMEM_SDUP_FLAG_NUM == visit_value)
                {
                    return DLOAD_OK;
                }
                else
                {
                    return DLOAD_ERR;
                }
            }
            break;
        case SET_SMEM_SWITCH_PORT_MAGIC:
            {
                if(NULL == smem_info_st.usbport_switch_addr)
                {
                    DLOAD_PRINT(KERN_ERR, "smem_info_st.usbport_switch_addr is NULL.");
                    return DLOAD_ERR;
                }
                
                *((uint32 *)(smem_info_st.usbport_switch_addr)) = SMEM_SWITCH_PUCI_FLAG_NUM;
            }
            break;
        case SET_SMEM_SWITCH_PORT_ZERO:
            {
                if(NULL == smem_info_st.usbport_switch_addr)
                {
                    DLOAD_PRINT(KERN_ERR, "smem_info_st.usbport_switch_addr is NULL.");
                    return DLOAD_ERR;
                }
                
                *((uint32 *)(smem_info_st.usbport_switch_addr)) = 0;
            }
            break;
        case GET_SMEM_IS_SWITCH_PORT:
            {
                if(NULL == smem_info_st.usbport_switch_addr)
                {
                    DLOAD_PRINT(KERN_ERR, "smem_info_st.usbport_switch_addr is NULL.");
                    return DLOAD_ERR;
                }
                
                visit_value = *((uint32 *)(smem_info_st.usbport_switch_addr));
                if(SMEM_SWITCH_PUCI_FLAG_NUM == visit_value)
                {
                    return DLOAD_OK;
                }
                else
                {
                    return DLOAD_ERR;
                }
            }
            break;
        case SET_SMEM_ONL_RESTORE_REBOOT_MAGIC:
            {
                if(NULL == smem_info_st.app_inactivate_addr)
                {
                    DLOAD_PRINT(KERN_ERR, "smem_info_st.app_inactivate_addr is NULL.");
                    return DLOAD_ERR;
                }
                
                *((uint32 *)(smem_info_st.app_inactivate_addr)) = SMEM_ONNR_FLAG_NUM;
            }
            break;
        case SET_SMEM_ONL_RESTORE_REBOOT_ZERO:
            {
                if(NULL == smem_info_st.app_inactivate_addr)
                {
                    DLOAD_PRINT(KERN_ERR, "smem_info_st.app_inactivate_addr is NULL.");
                    return DLOAD_ERR;
                }
                
                *((uint32 *)(smem_info_st.app_inactivate_addr)) = 0;
            }
            break;
        case GET_SMEM_ONL_IS_RESTORE_REBOOT:
            {
                if(NULL == smem_info_st.app_inactivate_addr)
                {
                    DLOAD_PRINT(KERN_ERR, "smem_info_st.app_inactivate_addr is NULL.");
                    return DLOAD_ERR;
                }
                
                visit_value = *((uint32 *)(smem_info_st.app_inactivate_addr));
                if(SMEM_ONNR_FLAG_NUM == visit_value)
                {
                    return DLOAD_OK;
                }
                else
                {
                    return DLOAD_ERR;
                }
            }
            break;
        case SET_SMEM_SD_RESTORE_REBOOT_MAGIC:
            {
                if(NULL == smem_info_st.app_inactivate_addr)
                {
                    DLOAD_PRINT(KERN_ERR, "smem_info_st.app_inactivate_addr is NULL.");
                    return DLOAD_ERR;
                }
                
                *((uint32 *)(smem_info_st.app_inactivate_addr)) = SMEM_SDNR_FLAG_NUM;
            }
            break;
        case SET_SMEM_SD_RESTORE_REBOOT_ZERO:
            {
                if(NULL == smem_info_st.app_inactivate_addr)
                {
                    DLOAD_PRINT(KERN_ERR, "smem_info_st.app_inactivate_addr is NULL.");
                    return DLOAD_ERR;
                }
                
                *((uint32 *)(smem_info_st.app_inactivate_addr)) = 0;
            }
            break;
        case GET_SMEM_SD_IS_RESTORE_REBOOT:
            {
                if(NULL == smem_info_st.app_inactivate_addr)
                {
                    DLOAD_PRINT(KERN_ERR, "smem_info_st.app_inactivate_addr is NULL.");
                    return DLOAD_ERR;
                }
                
                visit_value = *((uint32 *)(smem_info_st.app_inactivate_addr));
                if(SMEM_SDNR_FLAG_NUM == visit_value)
                {
                    return DLOAD_OK;
                }
                else
                {
                    return DLOAD_ERR;
                }
            }
            break;
        case SET_SMEM_EXCEP_REBOOT_INTO_ONL_MAGIC:
            {
                if(NULL == smem_info_st.app_inactivate_addr)
                {
                    DLOAD_PRINT(KERN_ERR, "smem_info_st.app_inactivate_addr is NULL.");
                    return DLOAD_ERR;
                }
                
                *((uint32 *)(smem_info_st.app_inactivate_addr)) = SMEM_ONNR_FLAG_NUM;
            }
            break;
        case SET_SMEM_EXCEP_REBOOT_INTO_ONL_ZERO:
            {
                if(NULL == smem_info_st.app_inactivate_addr)
                {
                    DLOAD_PRINT(KERN_ERR, "smem_info_st.app_inactivate_addr is NULL.");
                    return DLOAD_ERR;
                }
                
                *((uint32 *)(smem_info_st.app_inactivate_addr)) = 0;
            }
            break;
        case GET_SMEM_IS_EXCEP_REBOOT_INTO_ONL:
            {
                if(NULL == smem_info_st.app_inactivate_addr)
                {
                    DLOAD_PRINT(KERN_ERR, "smem_info_st.app_inactivate_addr is NULL.");
                    return DLOAD_ERR;
                }
                
                visit_value = *((uint32 *)(smem_info_st.app_inactivate_addr));
                if(SMEM_ONNR_FLAG_NUM == visit_value)
                {
                    return DLOAD_OK;
                }
                else
                {
                    return DLOAD_ERR;
                }
            }
            break;
        case SET_SMEM_FORCE_MODE_MAGIC:
            {
                if(NULL == smem_info_st.force_mode_addr)
                {
                    DLOAD_PRINT(KERN_ERR, "smem_info_st.force_mode_addr is NULL.");
                    return DLOAD_ERR;
                }
                
                *((uint32 *)(smem_info_st.force_mode_addr)) = SMEM_FORCELOAD_FLAG_NUM;
            }
            break;
        case SET_SMEM_FORCE_MODE_ZERO:
            {
                if(NULL == smem_info_st.force_mode_addr)
                {
                    DLOAD_PRINT(KERN_ERR, "smem_info_st.force_mode_addr is NULL.");
                    return DLOAD_ERR;
                }
                
                *((uint32 *)(smem_info_st.force_mode_addr)) = 0;
            }
            break;
        case SET_SMEM_FORCE_SUCCESS_MAGIC:
            {
                if(NULL == smem_info_st.force_mode_addr)
                {
                    DLOAD_PRINT(KERN_ERR, "smem_info_st.force_mode_addr is NULL.");
                    return DLOAD_ERR;
                }
                
                *((uint32 *)(smem_info_st.force_mode_addr)) = SMEM_FORCELOAD_SUCCESS_NUM;
            }
            break;
        case GET_SMEM_IS_FORCE_MODE:
            {
                if(NULL == smem_info_st.force_mode_addr)
                {
                    DLOAD_PRINT(KERN_ERR, "smem_info_st.force_mode_addr is NULL.");
                    return DLOAD_ERR;
                }
                
                visit_value = *((uint32 *)(smem_info_st.force_mode_addr));
                if(SMEM_FORCELOAD_FLAG_NUM == visit_value)
                {
                    return DLOAD_OK;
                }
                else
                {
                    return DLOAD_ERR;
                }
            }
            break;
        case SET_SMEM_UT_DLOAD_MAGIC:
            {
                if(NULL == smem_info_st.ut_dload_addr)
                {
                    DLOAD_PRINT(KERN_ERR, "smem_info_st.ut_dload_addr is NULL.");
                    return DLOAD_ERR;
                }

                *((uint32 *)(smem_info_st.ut_dload_addr)) = SMEM_BURN_UPDATE_FLAG_NUM; 
            }
            break;
        case SET_SMEM_UT_DLOAD_ZERO:
            {
                if(NULL == smem_info_st.ut_dload_addr)
                {
                    DLOAD_PRINT(KERN_ERR, "smem_info_st.ut_dload_addr is NULL.");
                    return DLOAD_ERR;
                }

                *((uint32 *)(smem_info_st.ut_dload_addr)) = 0; 
            }
            break;
        case GET_SMEM_IS_UT_DLOAD:
            {
                if(NULL == smem_info_st.ut_dload_addr)
                {
                    DLOAD_PRINT(KERN_ERR, "smem_info_st.ut_dload_addr is NULL.");
                    return DLOAD_ERR;
                }
                
                visit_value = *((uint32 *)(smem_info_st.ut_dload_addr));
                if(SMEM_BURN_UPDATE_FLAG_NUM == visit_value)
                {
                    return DLOAD_OK;
                }
                else
                {
                    return DLOAD_ERR;
                }
            }
            break;
        case SET_SMEM_MULTIUPG_DLOAD_MAGIC:
            {
                if(NULL == smem_info_st.multiupg_dload_addr)
                {
                    DLOAD_PRINT(KERN_ERR, "smem_info_st.multiupg_dload_addr is NULL.");
                    return DLOAD_ERR;
                }

                *((uint32 *)(smem_info_st.multiupg_dload_addr)) = SMEM_MULTIUPG_FLAG_NUM; 
            }
            break;
        default:
            {
                DLOAD_PRINT(KERN_ERR, "visit smem enum ERROR.");
                return DLOAD_ERR;
            }
            break;
    }

    return DLOAD_OK;
}

/*******************************************************************
  函数名称  : dload_mtd_flush
  函数描述  : 根据最新的分区表刷新MTD设备
  函数入参  : 
  函数输出  : NA
  函数返回值: DLOAD_OK    : 成功
             DLOAD_ERR : 失败
********************************************************************/
STATIC int32 dload_get_dload_feature(uint8 * data_buffer, uint32 data_len)
{
    dload_feature_t*  local_dload_feature_st = NULL;

    local_dload_feature_st = get_dload_feature();

    if(NULL == local_dload_feature_st)
    {
        return DLOAD_ERR;
    }

    memcpy((void *)data_buffer, (void *)local_dload_feature_st, sizeof(dload_feature_t));

    return DLOAD_OK;
}

/*******************************************************************
  函数名称  : get_smem_info
  函数描述  : 对外提供获取共享内存接口
  函数入参  : NA
  函数输出  : NA
  函数返回值: 共享内存指针
********************************************************************/
dload_smem_info_t* get_smem_info(void)
{
    return &smem_info_st;
}

/*******************************************************************
  函数名称  : nark_api_init
  函数描述  : API模块初始化
  函数入参  : NA
  函数输出  : NA
  函数返回值: DLOAD_OK    : 成功
              DLOAD_ERR   : 失败
********************************************************************/
int32 nark_api_init(void)
{
    huawei_smem_info *smem_data = NULL;
    product_info_t* product_info_st = NULL;

    DLOAD_PRINT(KERN_DEBUG, "nark api module init.");
    
    smem_data = (huawei_smem_info *)SRAM_DLOAD_ADDR;
    if (NULL == smem_data)
    {
        DLOAD_PRINT(KERN_ERR, "nark api smem error.");
        return DLOAD_ERR;
    }

    /* 共享内存指针初始化 */
    smem_info_st.dload_mode_addr        = (void *)&(smem_data->smem_dload_flag);
    smem_info_st.sd_upgrade_addr        = (void *)&(smem_data->smem_sd_upgrade);
    smem_info_st.usbport_switch_addr    = (void *)&(smem_data->smem_switch_pcui_flag);
    smem_info_st.app_inactivate_addr    = (void *)&(smem_data->smem_online_upgrade_flag);
    smem_info_st.force_mode_addr        = (void *)&(smem_data->smem_forceload_flag);
    smem_info_st.ut_dload_addr          = (void *)&(smem_data->smem_burn_update_flag);
    smem_info_st.multiupg_dload_addr    = (void *)&(smem_data->smem_multiupg_flag);
    smem_info_st.dload_times_addr       = (void *)&(smem_data->smem_update_times);
    smem_info_st.smem_reserve6          = (void *)NULL;
    smem_info_st.smem_reserve5          = (void *)NULL;
    smem_info_st.smem_reserve4          = (void *)NULL;
    smem_info_st.smem_reserve3          = (void *)NULL;
    smem_info_st.smem_reserve2          = (void *)NULL;
    smem_info_st.smem_reserve1          = (void *)NULL;


    /* 平台信息初始化 */
    product_info_st = get_product_info();
    if(NULL == product_info_st)
    {
        return DLOAD_ERR;
    }

    memcpy((void *)&platinfo_st.product_info_st, (void *)product_info_st, sizeof(product_info_t));
    
#if (FEATURE_ON == MBB_DLOAD_LED)
    platinfo_st.platform_feature.led_feature  = TRUE;
#else
    platinfo_st.platform_feature.led_feature  = FALSE;
#endif

#if (FEATURE_ON == MBB_DLOAD_LCD)
    platinfo_st.platform_feature.lcd_feature  = TRUE;
#else
    platinfo_st.platform_feature.lcd_feature  = FALSE;
#endif
    
    return DLOAD_OK;
}

/* 消息函数映射表 */
STATIC ioctl_map_t nark_msg_map[] = 
{
    {
        /* 获取平台产品特性信息 */
        MAP_INDEX(DLOAD_MAIN_MODULE, GET_PLATFORMINFO_MSG_ID), 
        get_platform_information, 
    },
    {
        /* 访问共享内存 */
        MAP_INDEX(DLOAD_MAIN_MODULE, VISIT_DLOAD_SMEM_MSG_ID),
        visit_dload_smem, 
    },
    {
         /* 刷新MTD设备 */
        MAP_INDEX(DLOAD_MAIN_MODULE, MTD_FLUSH_MSG_ID),
        dload_mtd_flush, 
    },
    {
        /* 获取电池电量格数 */
        MAP_INDEX(DLOAD_MAIN_MODULE, GET_BAT_LEVEL_MSG_ID), 
        get_battery_level, 
    },
    {
        /* 获取平台特性信息 */
        MAP_INDEX(DLOAD_SHOW_MODULE, GET_PLATFORMINFO_MSG_ID), 
        get_platform_information, 
    },
    {
        /* 获取MTD设备文件系统类型 */
        MAP_INDEX(DLOAD_FLASH_MODULE, GET_MTD_FS_TYPE_MSG_ID), 
        get_mtd_fs_type, 
    },
    {
        /* 访问共享内存 */
        MAP_INDEX(DLOAD_ONLINE_MODULE, VISIT_DLOAD_SMEM_MSG_ID),
        visit_dload_smem, 
    },
    {
        /* 主模块获取升级特性信息 */
        MAP_INDEX(DLOAD_MAIN_MODULE, GET_DLOAD_FEATURE_MSG_ID),
        dload_get_dload_feature, 
    },
    {
        /* AT模块获取升级特性信息 */
        MAP_INDEX(DLOAD_AT_MODULE, GET_DLOAD_FEATURE_MSG_ID),
        dload_get_dload_feature, 
    },
    {
        /* 获取path_info_st占的空间大*/
        MAP_INDEX(DLOAD_SHOW_MODULE, GET_PATH_INFO_SIZE_MSG_ID), 
        get_show_path_info_size, 
    },
    {
        /*获取path_info_st 内容*/
        MAP_INDEX(DLOAD_SHOW_MODULE, GET_PATH_INFO_MSG_ID), 
        get_show_path_information, 
    },
    {
       /* 获取LCD/LED配置占的空间大小*/
        MAP_INDEX(DLOAD_SHOW_MODULE, GET_SHOW_INFO_SIZE_MSG_ID), 
        get_show_info_size, 
    },
    {
        /* 获取各升级状态下的LCD/LED配置*/
        MAP_INDEX(DLOAD_SHOW_MODULE, GET_SHOW_INFO_MSG_ID), 
        get_show_information, 
    },
};

/*******************************************************************
  函数名称  : search_map_callback
  函数描述  : 根据映射索引查找回调函数
  函数入参  : msg_map_index : 映射索引
  函数输出  : NV
  函数返回值: msg_func_t    : 回调函数
********************************************************************/
msg_func_t search_map_callback(uint32  msg_map_index)
{
    int32   index = 0;
    int32   total = 0;

    total = sizeof(nark_msg_map) / sizeof(ioctl_map_t);
    for(index = 0; index < total; index ++)
    {
        if(msg_map_index == nark_msg_map[index].map_index)
        {
            DLOAD_PRINT(KERN_DEBUG, "map_index = %lu : msg_func = %lu.", msg_map_index, (uint32)nark_msg_map[index].msg_func);
            return nark_msg_map[index].msg_func;
        }
    }

    if(index == total)
    {
        DLOAD_PRINT(KERN_ERR, "msg_type = %lu  Ioctl msg_type ERROR.", msg_map_index);
        return NULL;
    }

    return NULL;
}

#ifdef __cplusplus
}
#endif
