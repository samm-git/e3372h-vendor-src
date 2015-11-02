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
#include <osl_types.h>
#include <linux/dma-mapping.h>
#include <bsp_edma.h>

#ifdef __cplusplus
extern "C" {
#endif


#ifndef __EMI_BALONG__
#define __EMI_BALONG__

#define EMI_BUF_WRT_CMD         0           /*读出/写入命令,相对EMI_BUF_BASE_ADDR偏移地址*/
#define EMI_WRT_DATA_8BIT       0x01        /*读出/写入数据, 8-bit,相对EMI_BUF_BASE_ADDR偏移地址*/
#define EMI_WRT_DATA_16BIT      0x02        /*读出/写入数据, 16-bit,相对EMI_BUF_BASE_ADDR偏移地址*/


#if 0
#define emiCmdOut8(cmd)		(iowrite8(cmd, HI_EMIMEM_REGBASE_ADDR_VIRT + EMI_BUF_WRT_CMD))

#define emiDataOut8(data)		(iowrite8(data, HI_EMIMEM_REGBASE_ADDR_VIRT + EMI_WRT_DATA_8BIT))

#define emiCmdIn8()		(ioread8(HI_EMIMEM_REGBASE_ADDR_VIRT + EMI_BUF_WRT_CMD))

#define emiDataIn8()		(ioread8(HI_EMIMEM_REGBASE_ADDR_VIRT + EMI_WRT_DATA_8BIT))
#endif

/* 时序选择*/
typedef enum tag_emi_scdul_mode_e
{
    EMI_TFT_8BIT       = 0,         /* TFT I8080时序,8-bit数据格式*/
    EMI_TFT_9BIT,                   /* TFT I8080时序,9-bit数据格式*/
    EMI_STN_8BIT_M6800,             /* STN M6800时序,8-bit数据格式*/
    EMI_STN_8BIT_I8080,             /* LCD I8080时序,8-bit数据格式*/
    EMI_LCD_8BIT,                  /* LCD M6800时序,8-bit数据格式*/
    EMI_SCHEDULING_BUTT
}emi_scdul_mode_e;

/* 低功耗: 寄存器备份、恢复 */
typedef struct
{
    u32     smbidcyr0;
    u32     smbwst1r0;
    u32     smbwst2r0;
    u32     smbwstoenr0;
    u32     smbwstwenr0;
    u32     smbcr0;
    u32     smbsr0;
}emi_reg_t;
#if 0
typedef struct
{
    unsigned int     smbidcyr0;
    unsigned int     smbwst1r0;
    unsigned int     smbwst2r0;
    unsigned int     smbwstoenr0;
    unsigned int     smbwstwenr0;
    unsigned int     smbcr0;
    unsigned int     smbsr0;
}emi_reg_t;
#endif

void emi_reg_read(u32 reg, u32* value);
void emi_reg_write(u32 reg, u32 value);
void emi_reg_show(u32 reg);
s32 emi_config(emi_scdul_mode_e EMI_MODE);
void emi_enable_clk(void);
void emi_disable_clk(void);
void emiCmdOut8(u8 cmd);
void emiDataOut8(u8 data);
u8 emiCmdIn8(void);
u8 emiDataIn8(void);
s32 emi_edma_transfer(dma_addr_t src,unsigned long len);

#endif

#ifdef __cplusplus
}
#endif