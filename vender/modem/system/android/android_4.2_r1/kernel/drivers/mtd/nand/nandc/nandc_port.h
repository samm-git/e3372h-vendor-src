 /*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
 * foss@huawei.com
 *
 * If distributed as part of the Linux kernel, the following license terms
 * apply:
 *
 * * This program is free software; you can redistribute it and/or modify
 * * it under the terms of the GNU General Public License version 2 and
 * * only version 2 as published by the Free Software Foundation.
 * *
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * * GNU General Public License for more details.
 * *
 * * You should have received a copy of the GNU General Public License
 * * along with this program; if not, write to the Free Software
 * * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Otherwise, the following license terms apply:
 *
 * * Redistribution and use in source and binary forms, with or without
 * * modification, are permitted provided that the following conditions
 * * are met:
 * * 1) Redistributions of source code must retain the above copyright
 * *    notice, this list of conditions and the following disclaimer.
 * * 2) Redistributions in binary form must reproduce the above copyright
 * *    notice, this list of conditions and the following disclaimer in the
 * *    documentation and/or other materials provided with the distribution.
 * * 3) Neither the name of Huawei nor the names of its contributors may
 * *    be used to endorse or promote products derived from this software
 * *    without specific prior written permission.
 *
 * * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */
#ifndef _NANDC_PORT_H_
#define _NANDC_PORT_H_
    
#ifdef __cplusplus
extern "C"
{
#endif

#include "product_config.h"

#ifndef __KERNEL__
/* keep them for compatibility */
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long long s64;
typedef unsigned long long u64;

#ifndef FLASHLESS_MACRO
#define FLASHLESS_MACRO
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
#endif

#endif

#define NANDC_DEBUG                                 (0x00000001)
#define NANDC_USE_ASSERT                            (0x00000008)
#define NANDC_USE_MEMPOOL                           (0x00000010)
#define NANDC_READ_ONLY                             (0x00000020)
#define NANDC_LOW_POWER                             (0x00000040)
#define NANDC_USE_MTD                               (0x00000080)

extern void put_str(char *pt,...);

#define NANDC_REG_READ32(addr)                              (*(volatile u32*)(addr) )
#define NANDC_REG_WRITE32(addr, val)                        (*(volatile u32 *)(addr) = (val))

/* register set and get */
#define NANDC_REG_SETBIT32(addr, pos, bits, val)       \
                (*(volatile u32 *)(addr) = (*(volatile u32 *)(addr) & \
                (~((((u32)1 << (bits)) - 1) << (pos)))) \
                 | ((u32)((val) & (((u32)1 << (bits)) - 1)) << (pos)))

#define NANDC_REG_WRITEBIT32(addr, pos, bits, val)       \
                (*(volatile u32 *)(addr) =  ((u32)((val) & (((u32)1 << (bits)) - 1)) << (pos)))



#define NANDC_REG_GETBIT32(addr, pos, bits)     \
                 ((*(volatile u32 *)(addr)  >> (pos)) & \
                 (((u32)1 << (bits)) - 1))

#ifdef __KERNEL__ 
#include <linux/version.h>
#include <asm/io.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/partitions.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <bsp_om.h>

#define STU_SET(element)                            element = 

#define NANDC_COMPILE_FLAG                          (NANDC_DEBUG | NANDC_USE_ASSERT | NANDC_LOW_POWER)
#define NANDC_POOL_SIZE_MAX                         (10400 + 1024)

#define __iomem
#define himalloc(p)                                 kmalloc(p, GFP_KERNEL)
#define hifree(p)                                   kfree(p)
#define hiassert(p)                                 BUG_ON(1)
#define hiout(p)                                    printk p 

#elif defined(__FASTBOOT__)
#include "string.h"
#include "config.h"
#include <boot/flash.h>

#define NANDC_COMPILE_FLAG                          (NANDC_USE_ASSERT | NANDC_DEBUG | NANDC_LOW_POWER)
#define NANDC_POOL_SIZE_MAX                         (4)

#define STU_SET(element)                            element = 
#define __iomem
#define hiout(p)                                    cprintf p
#define himalloc(p)                                 alloc(p)
#define hiassert(p)                                 (*(volatile int*)(0xffffffff) = 0)
#define hifree(p)                                   free(p)

#else
#error no plant macor defined
#endif


#ifdef __cplusplus
}
#endif

#endif  /*_NANDC_PORT_H_*/

