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
#ifndef _NANDC_CFG_H_
#define _NANDC_CFG_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <bsp_memmap.h>
#include <hi_base.h>
#include <hi_syssc.h>
#include <hi_nandc.h>
#include "nandc_v400.h"
#include "nandc_v600.h"
#include <bsp_nandc.h>

/* keep them for compatibility */
#define NANDC_STATUS_TIMEOUT	                (10)
#define NANDC_MAX_PAGE_SWAP                     (8)

#define NANDC_MAX_CHIPS                         (1)

#define NANDC_BAD_FLAG_FIRSTPAGE_OFFSET         (0)
#define NANDC_BAD_FLAG_SECONDPAGE_OFFSET        (1)
#define NANDC_BAD_FLAG_BYTE_SIZE                (4)

#define  NANDC_NULL_ASSERT_FORMAT               "0x%x,file:%s; line:%d \n"
#define  NANDC_NULL_PARAM_FORMAT                "NULL parameter\n "

#define NANDC_RD_RESULT_TIMEOUT                 (100000)
#define NANDC_PRG_RESULT_TIMEOUT                (1000000)
#define NANDC_ERASE_RESULT_TIMEOUT              (10000000)
#define NANDC_RESULT_TIME_OUT                   (10000000)

#define NANDC_RD_RETRY_CNT                      (3)

#define NANDC_MARK_SIZE                         (64)             /*in byte*/
#define NANDC_BADBLOCK_FLAG_SIZE                NANDC_MARK_SIZE  /*in byte*/
#define NANDC_ERASED_FLAG_SIZE                  NANDC_MARK_SIZE  /*in byte*/

#define NANDC_MAX_BLOCK_MASK                    (0xFFFFF)  /*(1MB)*/
#define NANDC_MAX_PAGE_MASK                     (0x1FFF)  /*(8kB)*/

#ifndef CONFIG_YAFFS_NO_YAFFS1
#define YAFFS_BYTES_PER_SPARE                   16
#endif

#define NANDC_REG_BASE_ADDR                    (HI_NANDC_REGBASE_ADDR)
#define NANDC_BUFFER_BASE_ADDR                 HI_NAND_MEM_BUFFER_ADDR
#define RVDS_TRACE_WRITE_BUFF                  (0x33f00000)
#define NANDC_SKIP_DEBUG
#define NANDC_RANDOM_ACCESS_THRESHOLD          (512)
#define NANDC_ECC_ACCESS_THRESHOLD             (NANDC_BUFSIZE_BASIC)
#define NANDC_READ_RESULT_DELAY

#if defined(NANDC_USE_V400)
#define NANDC_NAME                  NANDC4_NAME
#define NANDC_REG_SIZE              NANDC4_REG_SIZE
#define NANDC_BUFSIZE_BASIC			NANDC4_BUFSIZE_BASIC
#define NANDC_BUFSIZE_EXTERN        NANDC4_BUFSIZE_EXTERN
#define NANDC_BUFSIZE_TOTAL         NANDC4_BUFSIZE_TOTAL
#define NANDC_SLEEP_BUFFER_SIZE     (20)
#define NANDC_INT_CLEAR_ALL          nandc4_int_clear_all

#elif defined(NANDC_USE_V600)
#define NANDC_NAME                  NANDC6_NAME
#define NANDC_REG_SIZE              NANDC6_REG_SIZE
#define NANDC_BUFSIZE_BASIC			NANDC6_BUFSIZE_BASIC
#define NANDC_BUFSIZE_EXTERN        NANDC6_BUFSIZE_EXTERN
#define NANDC_BUFSIZE_TOTAL         NANDC6_BUFSIZE_TOTAL
#define NANDC_SLEEP_BUFFER_SIZE     (20)
#define NANDC_IRQ                   NANDC6_IRQ
#define NANDC_INT_CLEAR_ALL         nandc6_int_clear_all

/* #define FEATURE_NANDC_DMA_USE_INTERRUPT */  /* A core will use interrupt if this macro opens */
#else
#error no nand controller defined!!
#endif

#ifdef __cplusplus
}
#endif

#endif  /*_NANDC_DEF_H_*/
