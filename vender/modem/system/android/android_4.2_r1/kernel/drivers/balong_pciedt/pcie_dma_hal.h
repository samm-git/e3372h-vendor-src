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
#ifndef _PCIE_DMA_HAL_H_
#define _PCIE_DMA_HAL_H_

typedef struct _dma_chn_ctx{
    u32 ctrl_reg1;
    u32 ctrl_reg2;
    u32 trans_size;
    u32 sar_low;
    u32 sar_high;
    u32 dar_low;
    u32 dar_high;
    u32 ll_ptr_low;
    u32 ll_ptr_high;    
}dma_chn_ctx;

typedef struct _dma_data_ele{
    u32 ctrl_reg1;
    u32 trans_size;
    u32 sar_low;
    u32 sar_high;
    u32 dar_low;
    u32 dar_high;       
}dma_data_ele;

typedef struct _dma_ll_ele{
    u32 ctrl_reg1;
    u32 placeholder0;
    u32 ll_ptr_low;
    u32 ll_ptr_high; 
    u32 placeholder1;
    u32 placeholder3;
}dma_ll_ele;

#define PCIE_LOCAL (0)
#define PCIE_REMOTE (1)

#define WR    (0)
#define RD    (1)

#define CHN_RUNNING (1)
#define CHN_HALTED  (2)
#define CHN_STOPPED (3)

s32 pcie_dma_wr_engine_enable(u32 position);
s32 pcie_dma_wr_engine_disable(u32 position);
s32 pcie_dma_wr_chn_start(u32 position,u32 chnid);
s32 pcie_dma_wr_chn_stop(u32 position,u32 chnid);
s32 pcie_dma_wr_engine_arb_wet_set(u32 position,u32 chnid,s32 wet);
s32 pcie_dma_rd_engine_enable(u32 position);
s32 pcie_dma_rd_engine_disable(u32 position);
s32 pcie_dma_rd_chn_start(u32 position,u32 chnid);
s32 pcie_dma_rd_chn_stop(u32 position,u32 chnid);
s32 pcie_dma_rd_engine_arb_wet_set(u32 position,u32 chnid,s32 wet);
s32 pcie_dma_wr_int_status_get(u32 position,u32* status);
s32 pcie_dma_wr_int_mask_set(u32 position,u32 mask);
s32 pcie_dma_wr_int_clear(u32 position,u32 clear);
s32 pcie_dma_wr_done_imwr_adr_set(u32 position,u32 low_adr, u32 high_adr);
s32 pcie_dma_wr_abort_imwr_adr_set(u32 position,u32 low_adr, u32 high_adr);
s32 pcie_dma_wr_chn_imwr_data_set(u32 position,u32 chnid,u16 data);
s32 pcie_dma_ll_err_enable(u32 position, u32 wr_or_rd, u32 chnid);
s32 pcie_dma_ll_err_disable(u32 position, u32 wr_or_rd, u32 chnid);
s32 pcie_dma_rd_int_status_get(u32 position,u32* status);
s32 pcie_dma_rd_int_mask_set(u32 position,u32 mask);
s32 pcie_dma_rd_int_clear(u32 position,u32 clear);
s32 pcie_dma_rd_done_imwr_adr_set(u32 position,u32 low_adr, u32 high_adr);
s32 pcie_dma_rd_abort_imwr_adr_set(u32 position,u32 low_adr, u32 high_adr);
s32 pcie_dma_rd_chn_imwr_data_set(u32 position,u32 chnid,u16 data);
s32 pcie_dma_chn_ctx_idx_set(u32 position,u32 wr_or_rd,u32 idx);
s32 pcie_dma_chn_ctx_idx_get(u32 position,u32* idx);
s32 pcie_dma_chn_ctx_set(u32 position,dma_chn_ctx* chn_ctx);
s32 pcie_dma_chn_ctx_get(u32 position,dma_chn_ctx* chn_ctx);
#endif
