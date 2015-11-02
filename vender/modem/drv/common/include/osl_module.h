/*
 *  Copyright (C), 2011-2013, Hisilicon Technologies Co., Ltd. 
 *
 * File name     : osl_module.h 
 * Author        :  luting 00168886
 * Created       : 2012-12-26
 * Last Modified : 
 * Description   :  The base type definition for os layer.
 * Modification  : Create file 
 *
 */

#ifndef __OSL_MODULE_H
#define __OSL_MODULE_H

#ifdef __KERNEL__
#include <linux/module.h>

#elif defined(__VXWORKS__)


#define pure_initcall(x)
#define core_initcall(x)
#define core_initcall_sync(x)
#define postcore_initcall(x)
#define postcore_initcall_sync(x)
#define arch_initcall(x)
#define arch_initcall_sync(x)
#define subsys_initcall(x)
#define subsys_initcall_sync(x)
#define fs_initcall(x)
#define fs_initcall_sync(x)
#define rootfs_initcall(x)
#define device_initcall(x)
#define device_initcall_sync(x)
#define late_initcall(x)
#define late_initcall_sync(x)

#define module_init(x)
#define module_exit(x)
#define MODULE_AUTHOR(x)
#define MODULE_DESCRIPTION(x)
#define MODULE_LICENSE(x)

#define EXPORT_SYMBOL(x)
#define __init

#elif defined(__CMSIS_RTOS)

#define module_init(x)
#define core_initcall(x)
#define arch_initcall(x)
#define EXPORT_SYMBOL(x)
#define __init
#define module_exit(x)

#else

#endif /* __KERNEL__ */

#endif

