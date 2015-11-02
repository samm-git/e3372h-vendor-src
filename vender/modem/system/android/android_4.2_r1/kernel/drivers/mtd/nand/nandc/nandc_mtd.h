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
#ifndef _NANDC_MTD_H_
#define _NANDC_MTD_H_

#ifdef __cplusplus
extern "C"
{
#endif
#include "hi_nandc.h"
#include "nandc_inc.h"

#include "nandc_v400.h"
#include "nandc_v600.h"


#if defined (NANDC_USE_V400)

#define NANDC_NAME                  NANDC4_NAME
#define NANDC_REG_SIZE              NANDC4_REG_SIZE
#define NANDC_BUFSIZE_BASIC			NANDC4_BUFSIZE_BASIC
#define NANDC_BUFSIZE_EXTERN        NANDC4_BUFSIZE_EXTERN
#define NANDC_BUFSIZE_TOTAL         NANDC4_BUFSIZE_TOTAL

#elif defined (NANDC_USE_V600)

#define NANDC_NAME                  NANDC6_NAME
#define NANDC_REG_SIZE              NANDC6_REG_SIZE
#define NANDC_BUFSIZE_BASIC			NANDC6_BUFSIZE_BASIC
#define NANDC_BUFSIZE_EXTERN        NANDC6_BUFSIZE_EXTERN
#define NANDC_BUFSIZE_TOTAL         NANDC6_BUFSIZE_TOTAL

#else
#error no bsp board defined!!
#endif

#define NANDC_ADDR_CYCLE_MASK                   (0x4)
#define NANDC_CHIP_DELAY                        (25)

#ifdef CONFIG_PM 
#define END_FLAG    0xFFFFFFFF
#endif
/*****************************************************************************/

/*****************************************************************************/


u32  nandc_host_init_mtd(struct nandc_host * host,
                                    u32 pagesize,
                                    u32 sparesize,
                                    u32 chipsize,
                                    u32 erasesize,
                                    u32 buswidth,
                                    u32 numchips,
                                    struct mtd_partition  * ptable,
                                    u32 nr_parts
                                    );
/*****************************************************************************/
#ifdef CONFIG_PM 
u32 reg_offset[NANDC_SLEEP_BUFFER_SIZE / sizeof(u32)] = 
{
    HI_NFC_CON_OFFSET,
    HI_NFC_PWIDTH_OFFSET,
    HI_NFC_OPIDLE_OFFSET,
    
#ifdef NANDC_USE_V400
    HI_NFC_OP_OFFSET,
#else
    HI_NFC_DMA_CTRL_OFFSET,
#endif    

    END_FLAG,
};

#endif
/*****************************************************************************/


#ifdef __cplusplus
}
#endif

#endif  /*_NANDC_MTD_H_*/



