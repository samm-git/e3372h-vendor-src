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
/****************************头文件引用***************************************/
#include "../reb_config.h"
#include <linux/kernel.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/debugfs.h>
#if (YES == Reb_Platform_V7R2 )
#include "bsp_dump.h"
#endif
#include "../os_adapter/reb_os_adapter.h"
#if (YES == MBB_FEATURE_REB_DFT )
/****************************函数原型声明************************************/

static int rebtestdev_read (struct file *file, char __user *buf,
                            size_t count, loff_t *offset);

static int rebtestdev_write (struct file *file, const char __user *buf,
                             size_t count, loff_t *offset);

static long rebtestdev_ioctl(struct file *file, unsigned int cmd,
                            unsigned long arg);

/******************************内部宏定义***********************************/
/*内存重复释放*/
#define DRIVER_REB_MEM_DOUBLE_FREE         (0x0A)
/*内存异常释放*/
#define DRIVER_REB_MEM_ABNORMAL_FREE    (0x0B)
/*异常重启*/
#define DRIVER_REB_ABNORMAL_RESTART     (0X0C)
/*内存正常释放*/
#define DRIVER_REB_MEM_NORMAL_FREE    (0x0D)

#define DRIVER_REB_DATA_LEN     (100)
/*异常释放地址*/
#define ABNORMAL_ADDRESS          (0x10000000)
/*****************************内部变量定义**********************************/

static const struct file_operations g_rebtestdev_fops =
{
    .owner = THIS_MODULE,
    .read = rebtestdev_read,
    .write = rebtestdev_write,
    .unlocked_ioctl = rebtestdev_ioctl,
};

static struct class *g_rebtest_class;
/*******************************函数实现**************************************/

/*****************************************************************************
 函数名称  : void reb_driver_mem_double_free(void)
 功能描述  : 重复释放内存
 输入参数  : No
 输出参数  : No
 返 回 值  : No
 修改历史  :
             1. 2013-11-14 :  00206465 qiaoyichuan created
*****************************************************************************/
void reb_driver_mem_double_free(void)
{
    char *str = NULL;
    /*申请内存 */
    str = (char *)kmalloc(DRIVER_REB_DATA_LEN, GFP_KERNEL);
    if (NULL == str)
    {
        reb_msg( REB_MSG_ERR, "Reb_DFT: kmalloc failed \n");
        return;
    }
    memset(str, 0x0, DRIVER_REB_DATA_LEN);
    strncpy(str, "Hello_Reb_Test", DRIVER_REB_DATA_LEN - 1);
    /* 显示字符串 */
    reb_msg( REB_MSG_ERR, "Reb_DFT: String is %s\n", str);
    /* 重复释放内存*/
    MBB_KFREE(str);
    MBB_KFREE(str);
}

/*****************************************************************************
 函数名称  : reb_driver_mem_abnormal_free()
 功能描述  : 释放异常地址内存
 输入参数  : None
 输出参数  : None
 返 回 值  : No
 修改历史  : 
             1. 2013-11-20 :  00206465 qiaoyichuan created
*****************************************************************************/
void reb_driver_mem_abnormal_free(void)
{
    unsigned int *abnormal_addr = (unsigned int *)ABNORMAL_ADDRESS;
    /* 重复异常地址内存*/
    MBB_KFREE(abnormal_addr);
}
/*****************************************************************************
 函数名称  : void reb_driver_mem_normal_free()
 功能描述  : 正常释放内存
 输入参数  : No
 输出参数  : No
 返 回 值  : No
 修改历史  : 
             1. 2013-12-2 :  00206465 qiaoyichuan created
*****************************************************************************/
void reb_driver_mem_normal_free(void)
{
    char *str = NULL;

    str = (char *)kmalloc(DRIVER_REB_DATA_LEN, GFP_KERNEL);
    if (NULL == str)
    {
        reb_msg( REB_MSG_ERR, "Reb_DFT: kmalloc failed \n");
        return;
    }
    memset(str, 0x0, DRIVER_REB_DATA_LEN);
    strncpy(str, "Reb_Test_mem_normal_free", DRIVER_REB_DATA_LEN - 1);
    /* 显示字符串 */
    reb_msg( REB_MSG_ERR, "Reb_DFT: String is %s\n", str);
    /*释放内存*/
    MBB_KFREE(str);
}
/*****************************************************************************
 函数名称  : static int   rebtestdev_read (struct file *file, char __user *buf,
                                     size_t count,loff_t *offset)
 功能描述  : 该函数暂未使用
 输入参数  : None
 输出参数  : None
 返 回 值  : No
 修改历史  :
             1. 2013-11-14 :  00206465 qiaoyichuan created
*****************************************************************************/
static int   rebtestdev_read (struct file *file, char __user *buf,
                                                     size_t count, loff_t *offset)
{
    return 0;
}


/*****************************************************************************
 函数名称  : static int  rebtestdev_write (struct file *file, const char __user *buf,
                                          size_t count,loff_t *offset)
 功能描述  : 该函数暂未使用
 输入参数  : No
 输出参数  : No
 返 回 值  : No
 修改历史  :
             1. 2013-11-14 :  00206465 qiaoyichuan created
*****************************************************************************/
static int  rebtestdev_write (struct file *file, const char __user *buf,  
                                                     size_t count, loff_t *offset)
{
    return 0;
}


/*****************************************************************************
 函数名称  : static int rebtestdev_ioctl(struct file *file, unsigned int cmd,
                                           unsigned long arg)
 功能描述  : 用于处理上层下发的可靠性测试的命令
 输入参数  : No
 输出参数  : No
 返 回 值  : No
 修改历史  :
             1. 2013-11-14 :  00206465 qiaoyichuan created
*****************************************************************************/
static long rebtestdev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    switch (cmd)
    {
        case DRIVER_REB_MEM_DOUBLE_FREE:
        {
            reb_msg( REB_MSG_ERR, "Reb_DFT: double free\n");
            reb_driver_mem_double_free();
            break;
        }

        case DRIVER_REB_MEM_ABNORMAL_FREE:
        {
            reb_msg( REB_MSG_ERR, "Reb_DFT:  free abnormal\n");
            reb_driver_mem_abnormal_free();
            break;
        }

        case DRIVER_REB_MEM_NORMAL_FREE:
        {
            reb_msg( REB_MSG_ERR, "Reb_DFT:  free normal\n");
            reb_driver_mem_normal_free();
            break;
        }

        case DRIVER_REB_ABNORMAL_RESTART:
        {
            reb_msg( REB_MSG_ERR, "Reb_DFT:  abnormal  restart\n");
#if (YES == Reb_Platform_V7R2 )
            system_error(0,0,0,0,0);
#endif
#if (YES == Reb_Platform_9x25)
            panic("abnormal  restart test");
#endif
            break;
        }

        default:
            break;
    }

    return 0;
}

/*****************************************************************************
 函数名称  : static int __init reb_test_moudle_init(void)
 功能描述  : 建立可靠性测试模块，注册字符设备
 输入参数  : None
 输出参数  : None
 返 回 值  : No
 修改历史  :
             1. 2013-11-14 :  00206465 qiaoyichuan created
*****************************************************************************/
int reb_test_moudle_init(void)
{
    int rebtest_mager = 0;

    /*注册字符设备*/
    rebtest_mager = register_chrdev(0, "reb_test", &g_rebtestdev_fops);

    /* dev下创建设备节点*/
    if (rebtest_mager < 0)
    {
        reb_msg( REB_MSG_ERR, "Reb_DFT: REGISTER  REB TEST MOUDLE FAILED!\n");
        return -1;
    }
    else
    {
        g_rebtest_class = class_create(THIS_MODULE, "reb_test");
        device_create(g_rebtest_class, NULL, MKDEV(rebtest_mager, 0), "%s",
                      "reb_test");
    }
    reb_msg( REB_MSG_ERR, " INIT REB TEST MOUDLE !\n");

    return 0;
}

EXPORT_SYMBOL(reb_test_moudle_init);
/*****************************************************************************
 函数名称  : void __exit reb_test_moudle_exit(void)
 功能描述  : 退出可靠性测试模块
 输入参数  : No
 输出参数  : No
 返 回 值  : No
 修改历史  :
             1. 2013-11-14 :  00206465 qiaoyichuan created
*****************************************************************************/
void reb_test_moudle_exit(void)
{
    unregister_chrdev(0, "reb_test");
    reb_msg( REB_MSG_ERR, "Reb_DFT: EXIT REB TEST MOUDLE!\n");
}
EXPORT_SYMBOL(reb_test_moudle_exit);
#endif


