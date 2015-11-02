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
#ifndef _NANDC_INC_H_
#define _NANDC_INC_H_

#ifdef __cplusplus
extern "C"
{
#endif
#include "nandc_port.h" /* 该文件包含了product_config.h */

#include "nandc_mco.h"
#include "nandc_def.h"
#include "nandc_ctrl.h"

#ifdef __KERNEL__
#include <linux/mtd/mtd.h>
#else
#include "nandc_linux.h"
#endif

extern u32 nandc_trace_mask;

u32     nandc_data_transmit_raw(struct nandc_host *host, u32 datasize, u32 bufoffset, u32 access);
u32     nandc_data_transmit_page(struct nandc_host *host, struct nandc_pagemap *pagemap, u32 times, u32 access);
u32     nandc_native_nand_prob(struct nandc_host* host);
void    nandc_native_host_delete(struct nandc_host* host);
u32     nandc_native_location_by_pagenum(struct nandc_host* host, u32 pagenum);
u32     nandc_native_location_by_address(struct nandc_host* host, FSZ address);
u32     nandc_native_location_by_blocknum(struct nandc_host* host, u32 blknum);
u32     nandc_ctrl_entry(struct nandc_host *host);
u32     nandc_io_bitwidth_set(u32 bitwidth);
u32     nandc_nand_set_address(struct nandc_host *host);
u32     nandc_bbm_real_addr(FSZ addr_virtual, FSZ *addr_real);
u32     nandc_host_set_define(struct nandc_host * host, struct mtd_partition* ptable, u32 nr_parts);
u32     nandc_init_mtd_partition(struct nandc_host* host);
u32     nandc_host_add_nand_chip(struct nandc_host * host,  u32 chipselect, struct nandc_spec *spec);
u32     nandc_host_init_cluster(struct nandc_host *host, struct nandc_reg_desc* reg_tbl, u8 *bitstbl);
u32     nandc_host_set_chipparam(struct nandc_host * host);
u32     nandc_host_init_regfield(struct nandc_host *host, u32 regsize);
u32     nandc_nand_create_host(struct nandc_host  **host);
int     nand_spec_info_resume(struct mtd_info *mtd, struct nand_spec_shared_mem *spec_shmem);

struct nandc_host*  nandc_native_host_create(struct nandc_init_info* init_info);

extern struct nandc_init_info   nandc4_init_info;
extern struct nandc_init_info   nandc6_init_info;
extern struct nand_interface    nandc_nand_funcs;
extern struct nandc_init_info  *nandc_init_seed;
extern int *nandc_block_buffer;

u32 nandc_bbm_real_addr(FSZ addr_virtual, FSZ *addr_real);

#ifdef __cplusplus
}
#endif

#endif  /*_NANDC_INC_H_*/


