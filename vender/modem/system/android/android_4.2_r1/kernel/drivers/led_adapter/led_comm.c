/******************************************************************************

  Copyright (C), 2001-2013, Huawei Tech. Co., Ltd.

 ******************************************************************************
  File Name     : kerneldev.c
  Version       : Initial Draft
  Created       :
  Description   : kerneldev Init
  Function List :

  History        :
  1.Date         : 2013/11/26
    Author       : 王丽00249966
    Modification : Created function

******************************************************************************/
/*******************************修改记录开始**********************************
日期           修改人          问题单号                 修改内容
********************************修改记录结束**********************************/

#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <asm/uaccess.h>

#include "drv_comm.h"
#include "led_comm.h"
#include <bsp_softtimer.h>
#include "drv_leds.h"

#ifdef __cplusplus
extern "C" {
#endif


/****************************************************************************
 外部函数原型说明

*****************************************************************************/
#if ((FEATURE_ON == MBB_DLOAD_STICK) || (FEATURE_ON == MBB_DLOAD_HILINK))

extern int led_threecolor_switch(unsigned int new_state);
#endif

/****************************************************************************
 内部函数原型说明

*****************************************************************************/
static ssize_t dev_open(struct inode *inode, struct file *file);
static ssize_t dev_release(struct inode *inode, struct file *file);
int kernelDeviceIoctl(struct file *file, unsigned int cmd, unsigned long arg);

/****************************************************************************
 外部函数原型说明

*****************************************************************************/



/****************************************************************************
 全局变量申明

*****************************************************************************/
/*指示灯操作函数数据结构*/
static const struct file_operations balong_led_fops = {
    .owner = THIS_MODULE,
//    .open  = dev_open,
//    .release = dev_release,
    .unlocked_ioctl = kernelDeviceIoctl,
};


/*balong_led_miscdev作为调用misc_register函数的参数，
用于向linux内核注册指示灯misc设备。
*/
static struct miscdevice balong_led_miscdev = {
    .name = "led",
    .minor = MISC_DYNAMIC_MINOR,/*动态分配子设备号（minor）*/
    .fops = &balong_led_fops,
};


/****************************************************************************
 函数定义

*****************************************************************************/
static ssize_t dev_open(struct inode *inode, struct file *file)
{
    printk("[LED] %s\n", __func__);
    return 0;
}

static ssize_t dev_release(struct inode *inode, struct file *file)
{
    printk("[LED] %s\n", __func__);
    return 0;
}

/*****************************************************************************
函 数 名  : BipDeviceRelease
功能描述  : 关闭Led字符设备
输入参数  : struct inode *inode, struct file *file
输出参数  : 无
调用函数  : 无
被调函数  : 外部接口
History     :
1.日    期  :
  修改内容  : Create
*****************************************************************************/
static BSP_S32 kernelDeviceRelease(struct inode *inode, struct file *file)
{
    int ret = BSP_ERROR;
    
    if (NULL != inode && NULL != file)
    {
        ;
    }
    
    ret = misc_deregister(&balong_led_miscdev);
    
    if (0 > ret)
    {
        printk(KERN_ERR "[LED] kernelDeviceRelease: misc_deregister FAIL!\n");
    }
    
    return ret;
}

/*****************************************************************************
函 数 名  : LedDeviceIoctl
功能描述  : Led字符设备读写操作
输入参数  : struct inode *inode, struct file *file
输出参数  : 无
调用函数  : 无
被调函数  : 外部接口
History     :
1.日    期  :
  修改内容  : Create
2.修改: 2013-11-09
  作者: 王丽00249966
  修改内容: 增加LED 灯闪烁功能
*****************************************************************************/
int kernelDeviceIoctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    int ret = BSP_ERROR;
    LED_IOCTL_ARG stCtrlParms = {0};

    printk(KERN_ERR"[LED] %s:  cmd = %d, arg = 0x%x\n", __func__, cmd, arg);

    /*入参判断*/
    if (NULL != file)
    {
        ;
    }

    /* 检测命令的有效性 */
    if (_IOC_TYPE(cmd) != MEM_IOC_MAGIC) 
    {
        printk(KERN_ERR"[LED] kernelDeviceIoctl:CMD ERROR,file is NULL! \n");
        return -EINVAL;
    }

    if (_IOC_NR(cmd) >= MEMDEV_IOC_MAXNR) 
    {
        printk(KERN_ERR"[LED] kernelDeviceIoctl:CMD ERROR,file is NULL! \n");
        return -EINVAL;
    }

    if(NULL == arg)
    {
        printk(KERN_ERR"[LED] kernelDeviceIoctl: ARG ERROR,file is NULL! \n");
        return BSP_ERROR;
    } 

    /*内核调用*/
    if(NULL == file)
    {
        memcpy(&stCtrlParms, (void *)arg, sizeof(stCtrlParms));
    }
    else
    {
        /*应用调用*/
        if (BSP_OK != copy_from_user((LED_IOCTL_ARG *)&stCtrlParms,(LED_IOCTL_ARG *)arg,sizeof(LED_IOCTL_ARG)))
        {
            printk(KERN_ERR"[LED] kernelDeviceIoctl: copy_from_user FAIL!\n");
            return BSP_ERROR;
        }
    }

    switch (cmd)
    {
        case LED_IOCTL_GET:
        {            
            ret = BSP_OK;
            break;
        }
        case LED_IOCTL_SET:
        {
            ret = BSP_OK;
#if defined(BSP_CONFIG_BOARD_STICK) || defined(BSP_CONFIG_BOARD_HILINK)
            ret = led_threecolor_switch(stCtrlParms.led_dev_state);
#endif
            break;
        }
        default:
        {
            printk(KERN_ERR"[LED] kernelDeviceIoctl: CMD ERROR!\n");
            break;
        }
    }

    return ret;
}


/*****************************************************************************
函 数 名  :LedDeviceInit
功能描述  : 注册Led字符设备
输入参数  :无
输出参数  : 无
调用函数  : 无
被调函数  : 外部接口
History     :
1.日    期  :
  修改内容  : Create
*****************************************************************************/
static int __init kernelDeviceInit(void)
{
    int ret  = BSP_ERROR;
    int i = 0;
    int j = 0;

    ret = misc_register(&balong_led_miscdev);

    if(ret < 0)
    {
      printk(KERN_ERR "led init, register_chrdev failed.Ret=%d \n",ret);
    }
    printk("[LED] kernelDeviceInit: 1\ret = %d!\n",ret);
    
    return ret;
}

module_init(kernelDeviceInit);
module_exit(kernelDeviceRelease);

MODULE_AUTHOR("MBB.Huawei Device");
MODULE_DESCRIPTION("LED Driver");
MODULE_LICENSE("GPL");


#ifdef __cplusplus
}
#endif

