/*
 *  Copyright (C), 2011-2013, Hisilicon Technologies Co., Ltd.
 *
 * File name     : osl_malloc.h
 * Author        : luting 00168886
 * Created       : 2013-01-16
 * Last Modified :
 * Description   :  The base type definition for os layer.
 * Modification  : Create file
 *
 */

#ifndef __OSL_MALLOC_H
#define __OSL_MALLOC_H

#include "osl_common.h"

#ifdef __KERNEL__
#include <linux/slab.h>

static inline void* osl_malloc(u32 size)
{
	return kmalloc(size, GFP_KERNEL);
}

static inline void osl_free(const void *objp)
{
	return kfree(objp);
}

#elif defined(__VXWORKS__)
/*#include <memLib.h>*/
#include <stdlib.h>


static __inline__ void* osl_malloc(u32 size)
{
	return malloc(size);
}

static __inline__ void osl_free(void *objp)
{
	return free(objp);
}

#elif defined(__CMSIS_RTOS)
#include "osl_common.h"


static __inline__ void* osl_malloc(u32 size)
{
	return malloc(size);
}

static __inline__ void osl_free(void *objp)
{
	return free(objp);
}

#else

#endif /* __KERNEL__ */

#endif

