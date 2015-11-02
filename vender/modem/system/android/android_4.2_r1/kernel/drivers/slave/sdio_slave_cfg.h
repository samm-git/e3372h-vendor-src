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

#ifndef __SLAVE_CFG_H__
#define __SLAVE_CFG_H__

#ifdef __cplusplus
extern "C"
{
#endif
#define CSA_BUF_SIZE            0x400   /*(1*1024)*/
/*接收buffer大小*/
//#define DATA_BUF_SIZE           0x8000  /*(32*1024)，共享内存最大只能分8K*/
#define DATA_BUF_SIZE           0x10000  /* 512K */
/* 数据最大包长度 */
#define SLAVE_TRANS_PKT_SIZE    0x80000 /*(512*1024)，由于IP限制，单次传数最大只能传512KB*/

/* 每个描述符项buffer大小 */
#define ADMA_BUF_MAXSIZE        0x2000  /*(8*1024)*/

/* 预分配描述符个数 */
#define ADMA_DESC_LINE_COUNT    (SLAVE_TRANS_PKT_SIZE/ADMA_BUF_MAXSIZE)
#define ADMA_DESC_FUN0_COUNT     2

/* 描述符存储类型 */
#define ADMA_DESC_MEM_CACHEABLE
//#define ADMA_DESC_MEM_NOCACHEABLE
//#define ADMA_DESC_MEM_AXI

/*SLAVE debug 宏*/

/* 描述符cache操作 */
#ifdef ADMA_DESC_MEM_CACHEABLE
#define ADMA_DESC_CACHE   /* if desc need synchronization cache */
#endif

/* 数据cache操作 */
#define TX_BUF_FLUSH_CACHE      /* 是否flush cache */
#define RX_BUF_INVALIDATE_CACHE /* 是否valid cache */

#define SLAVE_CATALOG_IOONLY        (0)
#define SLAVE_CATALOG_COMBO         (1)
#define SLAVE_CATALOG_MEMONLY       (2)
#define SLAVE_CATALOG_MAX           (3)

#define SLAVE_CFG_CATALOG           (SLAVE_CATALOG_IOONLY)

#define SLAVE_CFG_INT_TASK_NAME     "tSlaveIntrProc"
#define SLAVE_CFG_INT_TASK_PRI      (142)
#define SLAVE_CFG_INT_TASK_STACK    (0x1000)

#define SLAVE_CFG_MSG_NUM           (64)

/*interrupt define*/
#define INT_VEC_SDIO_SLAVE  INT_LVL_SDCC    	//INT_LVL_SDCC  
#define FUN_CNT             0x2
#define SLAVE_REG_SIZE HI_MMC_DEV_REG_SIZE		//SLAVE_REG_SIZE_1k

#define SD_DEV_REGBASE_ADR              HI_MMC_DEV_REGBASE_ADDR

#define ASIC_SYSTEM_BASE    HI_SYSCTRL_BASE_ADDR_VIRT
#define SDIO_TIMER_REG        0x454         //自动门控时钟
#define SDIO_CONTROL_REG      0x958
#define SDIO_GPIO_REG         0x988

#ifdef __cplusplus
}
#endif

#endif /* end #define _BSP_GLOBAL_H_*/

