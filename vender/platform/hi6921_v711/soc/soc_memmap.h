/*************************************************************************
*   版权所有(C) 2008-2013 深圳华为技术有限公司.
*
*   文 件 名 :  soc_memmap_comm.h
*
*   作    者 :  y00184236
*
*   描    述 :  k3v3 v7r2 common memory map
*
*************************************************************************/


#ifndef __SOC_MEMMAP_H__
#define __SOC_MEMMAP_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "product_config.h"
#ifdef __KERNEL__
#include <soc_memmap_app.h>
#elif defined(__FASTBOOT__)
#include <soc_memmap_app.h>
#elif defined(__VXWORKS__)
#include <soc_memmap_mdm.h>
#elif defined(__CMSIS_RTOS)
#include <soc_memmap_m3.h>
#else
#include <soc_memmap_comm.h>
#endif

#ifdef __cplusplus
}
#endif

#endif    /* __SOC_MEMMAP_H__ */

