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
#include <linux/sched.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/time.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/poll.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <asm/io.h>

#include <linux/mlog_lib.h>

#define BUFF_SIZE 1024

static int mlog_test_read(struct file *file, char __user *buf,size_t count, loff_t *ppos)
{
    return 0;
}

static int mlog_test_write(struct file *file,
    const char __user *user_buf, size_t count, loff_t *ppos)
{
    char buf[BUFF_SIZE];
    int buf_size;
    static int time = 0;

    buf_size = min(count, sizeof(buf) - 1);
    if (copy_from_user(buf, user_buf, buf_size))
    {
        return -EFAULT;
    }

    printk(KERN_ERR "test write  start\n");
    mlog_print("ktest", mlog_lv_warn,  "%d %s. \n", time, buf);
    printk(KERN_ERR "test write  end\n");

    time++;

    return buf_size;
}
static int mlog_test_open(struct inode * inode, struct file * file)
{
    return 0;
}

static int mlog_test_release(struct inode * inode, struct file * file)
{
    return 0;
}

static unsigned int mlog_test_poll(struct file *file, poll_table *wait)
{
    return 0;
}

static const struct file_operations proc_mlog_log_operations = {
    .read        = mlog_test_read,
    .write       = mlog_test_write,
    .poll        = mlog_test_poll,
    .open        = mlog_test_open,
    .release    = mlog_test_release,
};

/*****************************************************************************
函数名称  : proc_mlog_log_init
功能描述  :
输入参数  :
输出参数  :
返 回 值  : No
修改历史  :
*****************************************************************************/
static int __init proc_mlog_test_init(void)
{
    proc_create("mobilelog_test", S_IRUSR, NULL, &proc_mlog_log_operations);
    return 0;
}

/*****************************************************************************
函数名称  : proc_mlog_log_exit
功能描述  :
输入参数  :
输出参数  :
返 回 值  : No
修改历史  :
*****************************************************************************/
static  void __exit proc_mlog_test_exit(void)
{
    remove_proc_entry("mobilelog_test",  NULL );
}


module_init(proc_mlog_test_init);
module_exit(proc_mlog_test_exit);
