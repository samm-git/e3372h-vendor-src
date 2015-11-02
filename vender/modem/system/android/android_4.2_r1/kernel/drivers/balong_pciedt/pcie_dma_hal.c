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
#include <linux/pci.h>
#include "mach/pcie.h"
#include "pcie_dma_hal.h"

extern struct pci_dev* balong_rc;
extern struct pci_dev* balong_ep;

static __inline__ s32 pcie_cfg_reg_write(u32 position, s32 reg, u32 val)
{
    struct pci_dev* dev = (position == PCIE_LOCAL) ? balong_rc : balong_ep;       
    return pci_write_config_dword(dev, reg, val);
}

static __inline__ s32 pcie_cfg_reg_read(u32 position, s32 reg, u32* val)
{
    struct pci_dev* dev = (position == PCIE_LOCAL) ? balong_rc : balong_ep;       
    return pci_read_config_dword(dev, reg, val);
}

s32 pcie_dma_wr_engine_enable(u32 position)
{
    return pcie_cfg_reg_write(position,DMA_WR_ENGINE_ENABLE,1);
}

s32 pcie_dma_wr_engine_disable(u32 position)
{
    return pcie_cfg_reg_write(position,DMA_WR_ENGINE_ENABLE,0);
}

s32 pcie_dma_wr_chn_start(u32 position,u32 chnid)
{
    return pcie_cfg_reg_write(position,DMA_WR_DOORBELL,chnid&0x7);
}

s32 pcie_dma_wr_chn_stop(u32 position,u32 chnid)
{
    return pcie_cfg_reg_write(position,DMA_WR_DOORBELL,(1<<31)|(chnid&0x7));
}

s32 pcie_dma_wr_engine_arb_wet_set(u32 position,u32 chnid,s32 wet)
{
    u32 reg_adr;
    u32 reg_off;
    u32 val = 0;
    s32 ret = 0;

    reg_adr = (chnid < 4) ? DMA_WR_ENGINE_CHN_ARB_WT_LOW : DMA_WR_ENGINE_CHN_ARB_WT_HIGH;
    reg_off = (chnid < 4) ? chnid*5 : (chnid - 4)*5;

    ret = pcie_cfg_reg_read(position,reg_adr,&val);
    if(ret)
    {
        printk(KERN_ERR "pcie_cfg_reg_read(position:%d,reg:0x%x) fail.",position,reg_adr);
        return ret;
    }
    
    val |= wet<<reg_off;
    return pcie_cfg_reg_write(position,reg_adr,val);
}

s32 pcie_dma_rd_engine_enable(u32 position)
{
    return pcie_cfg_reg_write(position,DMA_RD_ENGINE_ENABLE,1);
}

s32 pcie_dma_rd_engine_disable(u32 position)
{
    return pcie_cfg_reg_write(position,DMA_RD_ENGINE_ENABLE,0);
}

s32 pcie_dma_rd_chn_start(u32 position,u32 chnid)
{
    return pcie_cfg_reg_write(position,DMA_RD_DOORBELL,chnid&0x7);
}

s32 pcie_dma_rd_chn_stop(u32 position,u32 chnid)
{
    return pcie_cfg_reg_write(position,DMA_RD_DOORBELL,(1<<31)|(chnid&0x7));
}

s32 pcie_dma_rd_engine_arb_wet_set(u32 position,u32 chnid,s32 wet)
{
    u32 reg_adr;
    u32 reg_off;
    u32 val = 0;
    s32 ret = 0;

    reg_adr = (chnid < 4) ? DMA_RD_ENGINE_CHN_ARB_WT_LOW : DMA_RD_ENGINE_CHN_ARB_WT_HIGH;
    reg_off = (chnid < 4) ? chnid*5 : (chnid - 4)*5;

    ret = pcie_cfg_reg_read(position,reg_adr,&val);
    if(ret)
    {
        printk(KERN_ERR "pcie_cfg_reg_read(position:%d,reg:0x%x) fail.",position,reg_adr);
        return ret;
    }
    
    val |= wet<<reg_off;
    return pcie_cfg_reg_write(position,reg_adr,val);
}

s32 pcie_dma_wr_int_status_get(u32 position,u32* status)
{
    return pcie_cfg_reg_read(position,DMA_WR_INT_STATUS,status);
}

s32 pcie_dma_wr_int_mask_set(u32 position,u32 mask)
{
    return pcie_cfg_reg_write(position,DMA_WR_INT_MASK,mask);
}

s32 pcie_dma_wr_int_clear(u32 position,u32 clear)
{
    return pcie_cfg_reg_write(position,DMA_WR_INT_CLEAR,clear);
}

s32 pcie_dma_wr_done_imwr_adr_set(u32 position,u32 low_adr, u32 high_adr)
{
    s32 ret = 0;

    ret = pcie_cfg_reg_write(position,DMA_WR_DONE_IMWR_ADDR_LOW,low_adr);
    ret |= pcie_cfg_reg_write(position,DMA_WR_DONE_IMWR_ADDR_HIGH,high_adr);
    return ret;
}

s32 pcie_dma_wr_abort_imwr_adr_set(u32 position,u32 low_adr, u32 high_adr)
{
    s32 ret = 0;

    ret = pcie_cfg_reg_write(position,DMA_WR_ABORT_IMWR_ADDR_LOW,low_adr);
    ret |= pcie_cfg_reg_write(position,DMA_WR_ABORT_IMWR_ADDR_HIGH,high_adr);
    return ret;
}

s32 pcie_dma_wr_chn_imwr_data_set(u32 position,u32 chnid,u16 data)
{
    u32 reg_adr;
    u32 reg_off;
    u32 val;
    s32 ret = 0;

    switch(chnid)
    {
    case 0:
    case 1:
        reg_adr = DMA_WR_CHN_1_0_IMWR_DATA;
        break;
    case 2:
    case 3:
        reg_adr = DMA_WR_CHN_3_2_IMWR_DATA;
        break;
    case 4:
    case 5:
        reg_adr = DMA_WR_CHN_5_4_IMWR_DATA;
        break;
    case 6:
    case 7:
        reg_adr = DMA_WR_CHN_7_6_IMWR_DATA;
        break;
    default:
        printk(KERN_ERR "chnid > 7.");
        return -1;
    }

    reg_off = (chnid%2 == 0) ? 0 : 16;

    ret = pcie_cfg_reg_read(position,reg_adr,&val);
    if(ret)
    {
        printk(KERN_ERR "pcie_cfg_reg_read(position:%d, reg_addr:0x%x fail.)",
            position,reg_adr);
        return -1;
    }
    val &= ~(0xffff<<reg_off);
    val |= (data << reg_off);

    return pcie_cfg_reg_write(position,reg_adr,val);
}

s32 pcie_dma_ll_err_enable(u32 position, u32 wr_or_rd,u32 chnid)
{
    u32 reg_off;
    u32 reg_adr = (wr_or_rd == WR) ? DMA_WR_LL_ERR_ENABLE : DMA_RD_LL_ERR_ENABLE;
    u32 val;
    s32 ret = 0;
    
    reg_off = (position == PCIE_LOCAL) ? 16 : 0;

    ret = pcie_cfg_reg_read(position, reg_adr, &val);
    if(ret)
    {
        printk(KERN_ERR "pcie_cfg_reg_read(position:%d, reg_adr:0x%x fail.)",
            position,reg_adr);
        return -1;
    }

    val |= (0x1<<chnid)<<reg_off;
    return pcie_cfg_reg_write(position,reg_adr,val);
}

s32 pcie_dma_ll_err_disable(u32 position, u32 wr_or_rd,u32 chnid)
{
    u32 reg_off;
    u32 reg_adr = (wr_or_rd == WR) ? DMA_WR_LL_ERR_ENABLE : DMA_RD_LL_ERR_ENABLE;
    u32 val;
    s32 ret = 0;
    
    reg_off = (position == PCIE_LOCAL) ? 16 : 0;

    ret = pcie_cfg_reg_read(position, reg_adr, &val);
    if(ret)
    {
        printk(KERN_ERR "pcie_cfg_reg_read(position:%d, reg_adr:0x%x fail.)",
            position,reg_adr);
        return -1;
    }

    val &= ~((0x1<<chnid)<<reg_off);
    
    return pcie_cfg_reg_write(position,reg_adr,val);
}

s32 pcie_dma_rd_int_status_get(u32 position,u32* status)
{
    return pcie_cfg_reg_read(position,DMA_RD_INT_STATUS,status);
}

s32 pcie_dma_rd_int_mask_set(u32 position,u32 mask)
{
    return pcie_cfg_reg_write(position,DMA_RD_INT_MASK,mask);
}

s32 pcie_dma_rd_int_clear(u32 position,u32 clear)
{
    return pcie_cfg_reg_write(position,DMA_RD_INT_CLEAR,clear);
}

s32 pcie_dma_rd_done_imwr_adr_set(u32 position,u32 low_adr, u32 high_adr)
{
    s32 ret = 0;

    ret = pcie_cfg_reg_write(position,DMA_RD_DONE_IMWR_ADDR_LOW,low_adr);
    ret |= pcie_cfg_reg_write(position,DMA_RD_DONE_IMWR_ADDR_HIGH,high_adr);
    return ret;
}

s32 pcie_dma_rd_abort_imwr_adr_set(u32 position,u32 low_adr, u32 high_adr)
{
    s32 ret = 0;

    ret = pcie_cfg_reg_write(position,DMA_RD_ABORT_IMWR_ADDR_LOW,low_adr);
    ret |= pcie_cfg_reg_write(position,DMA_RD_ABORT_IMWR_ADDR_HIGH,high_adr);
    return ret;
}

s32 pcie_dma_rd_chn_imwr_data_set(u32 position,u32 chnid,u16 data)
{
    u32 reg_adr;
    u32 reg_off;
    u32 val;
    s32 ret = 0;

    switch(chnid)
    {
    case 0:
    case 1:
        reg_adr = DMA_RD_CHN_1_0_IMWR_DATA;
        break;
    case 2:
    case 3:
        reg_adr = DMA_RD_CHN_3_2_IMWR_DATA;
        break;
    case 4:
    case 5:
        reg_adr = DMA_RD_CHN_5_4_IMWR_DATA;
        break;
    case 6:
    case 7:
        reg_adr = DMA_RD_CHN_7_6_IMWR_DATA;
        break;
    default:
        printk(KERN_ERR "chnid > 7.");
        return -1;
    }

    reg_off = (chnid%2 == 0) ? 0 : 16;

    ret = pcie_cfg_reg_read(position,reg_adr,&val);
    if(ret)
    {
        printk(KERN_ERR "pcie_cfg_reg_read(position:%d, reg_addr:0x%x fail.)",
            position,reg_adr);
        return -1;
    }

    val |= (data << reg_off);

    return pcie_cfg_reg_write(position,reg_adr,val);
}

s32 pcie_dma_chn_ctx_idx_set(u32 position,u32 wr_or_rd,u32 idx)
{
    return pcie_cfg_reg_write(position,DMA_CHN_CTX_IDX,(wr_or_rd<<31)|idx);
}

s32 pcie_dma_chn_ctx_idx_get(u32 position,u32* idx)
{
    return pcie_cfg_reg_read(position,DMA_CHN_CTX_IDX,idx);
}

s32 pcie_dma_chn_ctx_set(u32 position,dma_chn_ctx* chn_ctx)
{
    s32 ret =0;

    ret = pcie_cfg_reg_write(position,DMA_CHN_CTRL1_REG,chn_ctx->ctrl_reg1);
    ret |= pcie_cfg_reg_write(position,DMA_CHN_CTRL2_REG,chn_ctx->ctrl_reg2);
    ret |= pcie_cfg_reg_write(position,DMA_TRANSFER_SIZE,chn_ctx->trans_size);
    ret |= pcie_cfg_reg_write(position,DMA_SAR_LOW,chn_ctx->sar_low);
    ret |= pcie_cfg_reg_write(position,DMA_SAR_HIGH,chn_ctx->sar_high);
    ret |= pcie_cfg_reg_write(position,DMA_DAR_LOW,chn_ctx->dar_low);
    ret |= pcie_cfg_reg_write(position,DMA_DAR_HIGH,chn_ctx->dar_high); 
    ret |= pcie_cfg_reg_write(position,DMA_LL_PTR_LOW,chn_ctx->ll_ptr_low);
    ret |= pcie_cfg_reg_write(position,DMA_LL_PTR_HIGH,chn_ctx->ll_ptr_high);  

    return ret;    
}

s32 pcie_dma_chn_ctx_get(u32 position,dma_chn_ctx* chn_ctx)
{
    s32 ret =0;

    ret = pcie_cfg_reg_read(position,DMA_CHN_CTRL1_REG,&(chn_ctx->ctrl_reg1));
    ret |= pcie_cfg_reg_read(position,DMA_CHN_CTRL2_REG,&(chn_ctx->ctrl_reg2));
    ret |= pcie_cfg_reg_read(position,DMA_TRANSFER_SIZE,&(chn_ctx->trans_size));
    ret |= pcie_cfg_reg_read(position,DMA_SAR_LOW,&(chn_ctx->sar_low));
    ret |= pcie_cfg_reg_read(position,DMA_SAR_HIGH,&(chn_ctx->sar_high));
    ret |= pcie_cfg_reg_read(position,DMA_DAR_LOW,&(chn_ctx->dar_low));
    ret |= pcie_cfg_reg_read(position,DMA_DAR_HIGH,&(chn_ctx->dar_high)); 
    ret |= pcie_cfg_reg_read(position,DMA_LL_PTR_LOW,&(chn_ctx->ll_ptr_low));
    ret |= pcie_cfg_reg_read(position,DMA_LL_PTR_HIGH,&(chn_ctx->ll_ptr_high));  

    return ret;    
}

