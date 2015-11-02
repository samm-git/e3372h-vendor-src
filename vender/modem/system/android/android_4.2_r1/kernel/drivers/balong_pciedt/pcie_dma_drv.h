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
#ifndef _PCIE_DMA_DRV_H_
#define _PCIE_DMA_DRV_H_

#include "pcie_dma_hal.h"

/*#define MAX_CHN_NUM (8)*/
/*only 1 write chn and 1 read chn*/
#define MAX_CHN_NUM   (1)

#define EP_DMA_RISE_LOCAL_INT

typedef void (*dma_done_isr)(void* private);

typedef void (*dma_abort_isr)(u32 err_status_l,u32 err_status_h,void* private);

typedef enum _dma_chn_status {
    not_allocated = 0, /*drv status*/
    running = 1,       /*chip status*/
    halted = 2,        /*chip status*/
    stopped = 3,       /*chip status*/
    allocated,         /*drv status*/
}dma_chn_status;

typedef struct _dma_chn_info {
    u32 id;
    u32 type;
    u32 status;
    dma_chn_ctx ctx;
    dma_done_isr done_isr;
    void* done_isr_private;
    u32 done_isr_count;
    dma_abort_isr abort_isr;
    void* abort_isr_private;
    u32 abort_isr_count;
}dma_chn_info;

typedef struct _dma_blk_info {
    u32 trans_size;
    u32 sar_low;
    u32 sar_high;
    u32 dar_low;
    u32 dar_high;
    dma_done_isr done_isr;
    void* done_isr_private; 
    dma_abort_isr abort_isr;
    void* abort_isr_private;    
}dma_blk_info;

typedef struct _data_blk_info {
    u32 trans_size;
    u32 sar_low;
    u32 sar_high;
    u32 dar_low;
    u32 dar_high;
    dma_done_isr done_isr;
    void* done_isr_private; 
    dma_abort_isr abort_isr;
    void* abort_isr_private;    
}data_blk_info;

typedef struct _dma_ll_info {
    u32 ll_ptr_low;
    u32 ll_ptr_high;
    dma_done_isr done_isr;
    void* done_isr_private; 
    dma_abort_isr abort_isr;
    void* abort_isr_private;    
}dma_ll_info;

typedef struct _dma_ll {
    u32 cycled;
    u32 virt_low_adr;
    u32 virt_high_adr;
    u32 phys_low_adr;
    u32 phys_high_adr;
    u32 data_ele_num;
    dma_data_ele* data_ele;
    dma_ll_ele* ll_ele;
}dma_ll;

s32 pcie_dma_chn_allocate(u32 type, u32* chn_id);
s32 pcie_dma_chn_status_get(u32 position,u32 wr_or_rd,u32 chn_id,u32* status);
s32 pcie_dma_chn_free(u32 type, u32 chn_id);
s32 pcie_dma_single_blk_config(u32 type, u32 chn_id, dma_blk_info* single_blk);
s32 pcie_dma_ll_config(u32 type, u32 chn_id, dma_ll_info* ll_info);
s32 pcie_dma_ll_build(dma_ll* cur_ll,dma_data_ele* data_ele,dma_ll* next_ll);
s32 pcie_dma_chn_start(u32 type,u32 chn_id);
s32 pcie_dma_chn_stop(u32 type,u32 chn_id);
s32 pcie_dma_init(void);
void pcie_dma_deinit(void);
#endif
