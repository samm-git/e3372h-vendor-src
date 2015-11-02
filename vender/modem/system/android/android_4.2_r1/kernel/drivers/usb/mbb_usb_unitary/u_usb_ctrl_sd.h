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
#include <linux/module.h>  
#include <linux/kernel.h>  
#include <linux/types.h>  
#include <linux/fs.h>  
#include <linux/init.h>  
#include <linux/delay.h>  
#include <asm/uaccess.h>  
#include <asm/irq.h>  
#include <asm/io.h>  
#include <linux/miscdevice.h> 
#include <linux/ioctl.h>
#include "usb_debug.h"

#define SDDEV_MAJOR 10   /*预设的sd的主设备号,写0表示动态分配，写其他值可以静态申请
                          *我们注册的是一个混杂设备，混杂设备的主设备号固定是10
                          */
/* 定义cmd命令 */
#define IOCTL_DISK_BLOCK_NUM _IOR( SDDEV_MAJOR, 0, USB_INT)
#define IOCTL_MOUNT_DISK _IO( SDDEV_MAJOR, 1 )
#define IOCTL_UNMOUNT_DISK _IO( SDDEV_MAJOR, 2 )
#define IOCTL_IS_MOUNTED_DISK _IO( SDDEV_MAJOR, 3 )

#define SDDEV_IOC_MAXNR 3
/*设备名称*/
#define SDDEV_NAME "usbware_disk_0"	

#define WEB_MODE 0
#define USB_MODE 1
