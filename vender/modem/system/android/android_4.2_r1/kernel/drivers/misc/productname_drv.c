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
/*lint -save -e7 -e10 -e63 -e64  -e115 -e132 -e533 -e539*/
#include <linux/delay.h>
#include <linux/proc_fs.h>
#include <product_config.h>
#include <DrvInterface.h>
#include <asm/uaccess.h>

/*宏定义 节点名称*/
#define DRV_PRODUCTNAME_PROC_FILE "productname"


/*全局变量 节点可操作的数据结构*/
static struct proc_dir_entry *g_product_name_proc_file = NULL;

/*************************************************************************
* 函数名     :  drv_product_name_proc_read
* 功能描述   :  查询产品名称
* 输入参数   :  void
* 输出参数   :  buffer : 从内核将版本号传给用户态
* 返回值     :  VER_RET_OK : 操作成功
*               VER_RET_ERROR : 操作失败
**************************************************************************/
static ssize_t drv_product_name_proc_read(struct file *filp, char *buffer, size_t length, loff_t *offset)
{
    void __user *buf_usr = (void __user *)buffer;
    static size_t  len_left = 0;
    size_t  len;

    int ret = 0;
    /*产品名称长度*/
    BSP_U32 name_length = 31;
    /*存放产品名称字符串*/
    char product_name[40] = {0};
    
    /*读取产品名称信息*/
    ret = BSP_GetProductInnerName(product_name, name_length);

    if (0 != ret)
    {
        pr_err("drv_product_name_proc_read : BSP_GetProductInnerName is failed.\n");
        return -1;
    }
    //strcat(product_name, "\n");

    /*第一次读取*/
    if (0 == *offset)
    {
        len_left = strlen(product_name);
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
        return -1;
    }

    if(copy_to_user(buf_usr, product_name + (strlen(product_name) - len_left), len))
    {
        pr_err("%s: copy_to_user failed, nothing copied\n", __FUNCTION__);
        return -1;
    }

    *offset  += len;
    len_left -= len;

    return len;
}

/*节点的操作函数数据结构*/
static struct file_operations drv_product_name_proc_ops = {
    .read  = drv_product_name_proc_read,
};

/*************************************************************************
* 函数名     :  create_product_name_proc_file
* 功能描述   :  创建节点
* 输入参数   :  void
* 输出参数   :  void
* 返回值     :  void
**************************************************************************/
static void create_product_name_proc_file(void)
{
    g_product_name_proc_file = create_proc_entry(DRV_PRODUCTNAME_PROC_FILE,
                                /*权限设置*/0444, NULL);
        
    if(g_product_name_proc_file)
    {
        g_product_name_proc_file->proc_fops = &drv_product_name_proc_ops;
    }
    else
    {
        pr_warning("%s: create proc entry for productname failed\n", __FUNCTION__);
    }
}

/*************************************************************************
* 函数名     :  remove_drv_version_proc_file
* 功能描述   :  删除节点
* 输入参数   :  void
* 输出参数   :  void
* 返回值     :  void
**************************************************************************/
static void remove_product_name_proc_file(void)
{
    remove_proc_entry(DRV_PRODUCTNAME_PROC_FILE, NULL);
}

static int __init product_name_drv_init(void)
{
    create_product_name_proc_file();
    return 0;
}

static void __exit product_name_drv_exit(void)
{
    remove_product_name_proc_file();
}

module_init(product_name_drv_init);

module_exit(product_name_drv_exit);

/*lint -restore +e7 +e10 +e63 +e64  +e115 +e132 +e533 +e539*/
