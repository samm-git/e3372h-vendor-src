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
#include <linux/delay.h>
#include <linux/proc_fs.h>
#include <product_config.h>
#include <DrvInterface.h>
#include <asm/uaccess.h>

/*宏定义 节点名称*/
#define DRV_MHWVER_PROC_FILE "mhwver"
#define DRV_MSWVER_PROC_FILE "mswver"

#define VER_RET_OK            0
#define VER_RET_ERROR       - 1

/*全局变量 节点可操作的数据结构*/
static struct proc_dir_entry *g_drv_mhwver_proc_file = NULL;
static struct proc_dir_entry *g_drv_mswver_proc_file = NULL;

/*************************************************************************
* 函数名     :  drv_mhwver_proc_read
* 功能描述   :  modem侧硬件版本号查询
* 输入参数   :  void
* 输出参数   :  buffer : 从内核将版本号传给用户态
* 返回值     :  VER_RET_OK : 操作成功
*               VER_RET_ERROR : 操作失败
**************************************************************************/
static ssize_t drv_mhwver_proc_read(struct file *filp,
                char *buffer, size_t length, loff_t *offset)
{
    void __user *buf_usr = (void __user *)buffer;
    static size_t  len_left = 0;
    size_t  len;

    int ret = 0;
    /*版本号长度*/
    BSP_U32 mhwver_length = 31;
    /*存放版本号字符串*/
    char mhwver_version[40] = {0};
    
    /*读取版本号信息*/
    ret = BSP_HwGetHwVersion(mhwver_version, mhwver_length);
    if (0 != ret)
    {
        pr_err("drv_mhwver_proc_read : BSP_HwGetHwVersion is failed.\n");
        return VER_RET_ERROR;
    }
    strcat(mhwver_version, "\n");
    
    /*第一次读取*/
    if (0 == *offset)
    {
        len_left = strlen(mhwver_version);
    }
    len      = (length > len_left) ? (len_left) : length;
    /*读取完毕*/
    if(len_left == 0)
    {
        return 0;
    }

    if (!access_ok(VERIFY_WRITE, buf_usr, len))
    {
        pr_err("%s: Verify user buffer write fail.\n", __FUNCTION__);
        return - EFAULT;
    }

    if(copy_to_user(buf_usr, mhwver_version + (strlen(mhwver_version) - len_left), len))
    {
        pr_err("%s: copy_to_user failed, nothing copied\n", __FUNCTION__);
        return - EFAULT;
    }

    *offset  += len;
    len_left -= len;
    return len;
}

/*************************************************************************
* 函数名     :  drv_mswver_proc_read
* 功能描述   :  router侧软件版本号查询
* 输入参数   :  void
* 输出参数   :  buffer : 从内核将版本号传给用户态
* 返回值     :  VER_RET_OK : 操作成功
*               VER_RET_ERROR : 操作失败
**************************************************************************/
static ssize_t drv_mswver_proc_read(struct file *filp,
                char *buffer, size_t length, loff_t *offset)
{
    void __user *buf_usr = (void __user *)buffer;
    static size_t  len_left = 0;
    size_t  len;
    /*存放版本号字符串*/
    char mswver_version[25] = {0};
    
    /*从宏定义中取版本号*/
    strncpy(mswver_version, PRODUCT_CFG_MODEM_SOFTWARE_VER, 15);
    strcat(mswver_version, "\n");
    
    /*第一次读取*/
    if (0 == *offset)
    {
        len_left = strlen(mswver_version);
    }
    len      = (length > len_left) ? (len_left) : length;
    /*读取完毕*/
    if(len_left == 0)
    {
        return 0;
    }

    if (!access_ok(VERIFY_WRITE, buf_usr, len))
    {
        pr_err("%s: Verify user buffer write fail.\n", __FUNCTION__);
        return - EFAULT;
    }

    if(copy_to_user(buf_usr, mswver_version + (strlen(mswver_version) - len_left), len))
    {
        pr_err("%s: copy_to_user failed, nothing copied\n", __FUNCTION__);
        return - EFAULT;
    }

    *offset  += len;
    len_left -= len;

    return len;
}

/*节点的操作函数数据结构*/
static struct file_operations drv_mhwver_proc_ops = {
    .read  = drv_mhwver_proc_read,
};
static struct file_operations drv_mswver_proc_ops = {
    .read  = drv_mswver_proc_read,
};

/*************************************************************************
* 函数名     :  create_drv_version_proc_file
* 功能描述   :  创建节点
* 输入参数   :  void
* 输出参数   :  void
* 返回值     :  void
**************************************************************************/
static void create_drv_version_proc_file(void)
{
    g_drv_mhwver_proc_file = create_proc_entry(DRV_MHWVER_PROC_FILE,
                                /*权限设置*/0444, NULL);
        
    if(g_drv_mhwver_proc_file)
    {
        g_drv_mhwver_proc_file->proc_fops = &drv_mhwver_proc_ops;
    }
    else
    {
        pr_warning("%s: create proc entry for mhwver_version failed\n",
            DRV_MHWVER_PROC_FILE);
    }
    
    g_drv_mswver_proc_file = create_proc_entry(DRV_MSWVER_PROC_FILE,
                                /*权限设置*/0444, NULL);
        
    if(g_drv_mswver_proc_file)
    {
        g_drv_mswver_proc_file->proc_fops = &drv_mswver_proc_ops;
    }
    else
    {
        pr_warning("%s: create proc entry for mswver_version failed\n",
            DRV_MSWVER_PROC_FILE);
    }
}

/*************************************************************************
* 函数名     :  remove_drv_version_proc_file
* 功能描述   :  删除节点
* 输入参数   :  void
* 输出参数   :  void
* 返回值     :  void
**************************************************************************/
static void remove_drv_version_proc_file(void)
{
#if 0
    extern struct proc_dir_entry proc_root_mhwver;
    extern struct proc_dir_entry proc_root_mswver;
    
    remove_proc_entry(DRV_MHWVER_PROC_FILE, &proc_root_mhwver);
    remove_proc_entry(DRV_MSWVER_PROC_FILE, &proc_root_mswver);
#endif	
}

static int __init version_drv_init(void)
{
    create_drv_version_proc_file();
    return 0;
}

static void __exit version_drv_exit(void)
{
    remove_drv_version_proc_file();
}

module_init(version_drv_init);
module_exit(version_drv_exit);
