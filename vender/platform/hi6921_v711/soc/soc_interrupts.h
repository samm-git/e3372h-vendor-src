/*************************************************************************
*   版权所有(C) 2008-2013 深圳华为技术有限公司.
*
*   文 件 名 :  soc_interrupts.h
*
*   作    者 :  z67193
*
*   描    述 :
*
*************************************************************************/


#ifndef __SOC_INTERRUPTS_H__
#define __SOC_INTERRUPTS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "product_config.h"
#ifdef __KERNEL__
#include <soc_interrupts_app.h>
#elif defined(__FASTBOOT__)
#include <soc_interrupts_app.h>
#elif defined(__VXWORKS__)
#include <soc_interrupts_mdm.h>
#elif defined(__CMSIS_RTOS)
#include <soc_interrupts_m3.h>
#else
#include <soc_interrupts_comm.h>
#endif

#ifdef __cplusplus
}
#endif

#endif    /* __SOC_INTERRUPTS_H__ */

