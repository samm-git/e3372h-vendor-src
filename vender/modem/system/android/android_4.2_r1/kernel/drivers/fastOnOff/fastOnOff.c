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

#include <linux/errno.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <drv_fastOnOff.h>
#include "fastOnOff.h"

#if (FEATURE_ON == MBB_FAST_ON_OFF)

/*假关机标志，默认为正常模式*/
FASTONOFF_MODE gFastOnOffMode = FASTONOFF_MODE_CLR;
/* 假关机内核通知链头结点初始化 */
BLOCKING_NOTIFIER_HEAD(g_fast_on_off_notifier_list);

/********************************************************
*函数名   : fastOnOffSetFastOnOffMode
*函数功能 : 设置快速开关机标志位，用于在底软判断是否进入假关机状态
*输入参数 : FASTONOFF_MODE fastOnOffMode
*输出参数 : 无
*返回值   : 执行成功返回0，失败返回-1
*修改历史 :
*           2013-5-25 徐超 初版作成
********************************************************/
static int fastOnOffSetFastOnOffMode(FASTONOFF_MODE fastOnOffMode)
{
    int ret = 0;

    switch(fastOnOffMode)
    {
        case FASTONOFF_MODE_CLR:
			if( FASTONOFF_MODE_CLR == gFastOnOffMode )
            {
                printk(KERN_ERR "\r\n [FAST ON OFF DRV] Already in normal mode!");
                return ret;
            }
            /*退出假关机，进入正常状态*/
            gFastOnOffMode = FASTONOFF_MODE_CLR;
            break;
        case FASTONOFF_MODE_SET:
			if( FASTONOFF_MODE_SET == gFastOnOffMode )
            {
                printk(KERN_ERR "\r\n [FAST ON OFF DRV] Already in fast off mode!");
                return ret;
            }
            /*进入假关机状态*/
            gFastOnOffMode = FASTONOFF_MODE_SET;
            break;
        default:
            printk(KERN_ERR "\r\n [FAST ON OFF DRV] fastOnOffSetFastOnOffMode error! fastOnOffMode=%d!\r\n", fastOnOffMode);
            ret = -EINVAL;
            return ret;
    }

	/* 使用内核通知链通知各驱动进入或者退出假关机 */
	ret = blocking_notifier_call_chain(&g_fast_on_off_notifier_list,(unsigned long)gFastOnOffMode,NULL);
	return ret;
}

/********************************************************
*函数名   : fastOnOffGetFastOnOffMode
*函数功能 : 获取快速开关机标志位，用于在底软判断是否进入假关机状态
*输入参数 : 无
*输出参数 : 无
*返回值   : 执行成功返回假关机标志，失败返回-1
*修改历史 :
*           2013-5-25 徐超 初版作成
********************************************************/
FASTONOFF_MODE fastOnOffGetFastOnOffMode(void)
{
    printk(KERN_ERR "\r\n [FAST ON OFF DRV] fastOnOffGetFastOnOffMode! fastOnOffMode=%d!\r\n", gFastOnOffMode);
    return gFastOnOffMode;
}

/********************************************************
*函数名   : fastOnOffIoctl
*函数功能 : 快速开关机驱动ioctl函数，用于处理用户态程序下发的指令
*输入参数 : 
*输出参数 : 无
*返回值   : 执行成功返回0，失败返回非0值
*修改历史 :
*           2013-5-25 徐超 初版作成
********************************************************/
long fastOnOffIoctl(struct file *file, unsigned int cmd, unsigned long data)
{
    int ret = 0;

    printk(KERN_ERR "\r\n [FAST ON OFF DRV] fastOnOffIoctl: cmd=%u, data = %lu!\r\n",cmd,data);
    
    if(NULL == file )
    {
        printk(KERN_ERR "\r\n [FAST ON OFF DRV]fastOnOffIoctl: file is NULL!\r\n");
        return -1;
    }

    switch(cmd)
    {
        case FASTONOFF_FAST_OFF_MODE:
            /*进入假关机模式*/
            ret = fastOnOffSetFastOnOffMode(FASTONOFF_MODE_SET);
            break;
        case FASTONOFF_FAST_ON_MODE:
            /*退出假关机，进入假开机模式*/
            ret = fastOnOffSetFastOnOffMode(FASTONOFF_MODE_CLR);
            break;
        default:
            /*驱动不支持该命令*/
            return -ENOTTY;
    }
    
    return ret;
}

static const struct file_operations fastOnOffFops = {
    .owner         = THIS_MODULE,
    .unlocked_ioctl = fastOnOffIoctl,
};

static struct miscdevice fastOnOffMiscdev = {
    .minor    = MISC_DYNAMIC_MINOR,
    .name    = "fastOnOff",
    .fops    = &fastOnOffFops
};

static int __init fastOnOffInit(void)
{
    int ret = 0;
    
    ret = misc_register(&fastOnOffMiscdev);
    if (0 > ret)
    {
        printk(KERN_ERR "\r\n [FAST ON OFF DRV] misc_register [fastOnOff module] failed.\r\n");
        return ret;
    }
    
    return ret;
}

static void __exit fastOnOffExit(void)
{
    int ret = 0;
    ret = misc_deregister(&fastOnOffMiscdev);
    if (0 > ret)
    {
        printk(KERN_ERR "\r\n [FAST ON OFF DRV] misc_deregister [fastOnOff module] failed.\r\n");
    }
}

module_init(fastOnOffInit);
module_exit(fastOnOffExit);

MODULE_AUTHOR("MBB.Huawei Device");
MODULE_DESCRIPTION("Fast On Off Driver");
MODULE_LICENSE("GPL");

#endif /* #if (FEATURE_ON == MBB_FAST_ON_OFF) */
