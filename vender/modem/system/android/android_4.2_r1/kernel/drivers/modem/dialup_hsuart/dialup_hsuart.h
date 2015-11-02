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
#ifndef    _HSUART_UDI_H_
#define    _HSUART_UDI_H_

#ifdef __cplusplus
extern "C" {
#endif
/**************************************************************************
  头文件包含                            
**************************************************************************/
#include <linux/semaphore.h>
#include <mach/gpio.h>
#include <osl_sem.h>
#include "bsp_softtimer.h"
#include "bsp_om.h"
#include "bsp_nvim.h"
#include "drv_dpm.h"
#include "drv_usb.h"
#include "drv_edma.h"
#include "drv_nv_id.h"
#include "drv_nv_def.h"
#include "drv_hsuart.h"

#ifndef OK
#define OK 0
#endif
#ifndef ERROR
#define ERROR (-1)
#endif


#ifdef _DRV_LLT_
#define memset
#endif

#define HSUART_TRUE 	1
#define HSUART_FALSE 	0
#define HSUART_ENABLE 	1
#define hsuart_error(fmt, ...)    (bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_HSUART, "[HSUART]: <%s> "fmt, __FUNCTION__, ##__VA_ARGS__))
#define hsuart_debug(fmt, ...)    (bsp_trace(BSP_LOG_LEVEL_DEBUG, BSP_MODU_HSUART, "[HSUART]: "fmt, ##__VA_ARGS__))

/*****************************************************************************
  2 宏定义
*****************************************************************************/
#define SWITCH_TIMER_LENGTH 	1000
#define SWITCH_TWO_TIMER_LENGTH 	500
#define ADAPT_TIMER_LENGTH 		15000
#define RX_TIMER_LENGTH 		10
#define DEFAULT_BAUD_IDX		5
#define DEFAULT_BAUD			115200

#define SYS_TICK_32K			32768
/* buffer define */
#define UART_READ_BUF_SIZE 		1536
#define UART_READ_BUF_NUM 		16
#define UART_WRITE_BUF_NUM 		64

#define SIZE_4K 				(4096)

#define HSUART_DCD    			GPIO_2_4
#define HSUART_DSR        		GPIO_2_5
#define HSUART_DTR        		GPIO_2_16
#define HSUART_RING        		GPIO_2_26

#define HSUART_RX_TASK_PRI		81
#define HSUART_TX_TASK_PRI		81
#define HSUART_RX_TASK_SIZE		0x1000
#define HSUART_TX_TASK_SIZE		0x1000

/* DMA Mode */
#define UART_DEF_NO_DMA_MODE  	0x00   /* Mode 0 */
#define UART_DEF_DMA_MODE     	0x08   /* Mode 1 */
#define UART_LCR_DLAB			0x80
#define UART_TX_FIFO_RESET		0x04
#define UART_RX_FIFO_RESET		0x02
/* 设置接收FIFO水线 */
#define UART_DEF_RT_1CHAR     	0x00 /* FIFO 中有1 个字符*/
#define UART_DEF_RT_2CHAR     	0xc0 /* FIFO 差2 个字符就满*/
#define UART_DEF_RT_ONEFOUR   	0x40 /* FIFO 四分之一满*/
#define UART_DEF_RT_ONETWO    	0x80 /* FIFO 二分之一满*/
#define UART_DEF_TET_ONETWO  	0x30 /* FIFO 二分之一满*/

#define UART_REG_OFFSET_MCR		0x10 /* uart modem控制器 */
#define UART_REG_OFFSET_TX_FF_DEPTH 0x2C /* 发送FIFO深度寄存器 */

#define UART_AFCE_ENABLE 		0x20 /* 自动流控 */
#define UART_RTS_ENABLE			0x02
#define UART_DEF_TX_DEPTH		0x40
typedef u32    	HSUART_TASK_ID;
typedef int* 	HTIMER;
/*****************************************************************************
  3 枚举定义
*****************************************************************************/
/* +++检测阶段*/
typedef enum
{
    NO_DETECT_STAGE,
    DETECT_STAGE_1,
    DETECT_STAGE_2,
    DETECT_STAGE_3,
    DETECT_STAGE_MAX,
}detect_stage;

/*

typedef struct tagUART_TIMER
{
    HTIMER id;
    BSP_BOOL flag;
}uart_timer;
*/
typedef enum tagUART_TRANSFER_MODE
{
    CPU_TRANSFER,
    DMA_TRANSFER,
    MODE_MAX,
}uart_transfer_mode;

typedef enum tagUART_MEM_STATE
{
    UART_MEM_NOT_IN_LIST = 0,
    UART_MEM_IN_FREELIST = 1,
    UART_MEM_IN_DONELIST = 2
}uart_mem_state;

/*****************************************************************************
  3 结构体定义
*****************************************************************************/
typedef u32 phys_addr;

/* Transfer block descriptor */
typedef struct mem_desc_s {
    void           *vaddr;
    phys_addr      paddr;
} mem_desc_t;

typedef mem_desc_t *mem_desc_h;

/* uart stat info */
typedef struct tagUART_STAT {
    BSP_U32 write_not_free_cnt;
    BSP_U32 lost_data_cnt;
    BSP_U32 syn_write_cnt;
    BSP_U32 burst_dma_done;
    BSP_U32 single_dma_done;
    BSP_U32 single_time_out;
    BSP_U32 alloc_skb_fail;
    BSP_U32 sleep_delay_cnt;
} hsuart_stat;

typedef struct tagHSUART_DEV_S
{
    int irq;
    unsigned int clock;
    struct spinlock lock_irq;
    unsigned int phy_addr_base;
    volatile void __iomem * vir_addr_base;
    BALONG_DMA_REQ dma_req_rx;
    BALONG_DMA_REQ dma_req_tx;
}hsuart_dev;

typedef int (*hsuart_set_baud_cb)(hsuart_dev *uart_dev, int baud);
typedef int (*hsuart_set_wlen_cb)(hsuart_dev *uart_dev, int w_len);
typedef int (*hsuart_set_stp2_cb)(hsuart_dev *uart_dev, int stp2);
typedef int (*hsuart_set_eps_cb)(hsuart_dev *uart_dev, int eps);

typedef struct tagUART_UDI_CALLBACKS_S
{
    /* General Handlers */
    hsuart_free_cb_t free_cb;
    hsuart_read_cb_t read_cb;
    hsuart_msc_read_cb_t msc_read_cb;
    hsuart_switch_mode_cb_t switch_mode_cb;
    
    hsuart_set_baud_cb set_baud_cb;
    hsuart_set_wlen_cb set_wlen_cb;
    hsuart_set_stp2_cb set_stp2_cb;
    hsuart_set_eps_cb set_eps_cb;
}hsuart_udi_cbs;

typedef struct tagUART_WR_ASYNC_INFO
{
    char* pBuffer;
    unsigned int u32Size;
    void* pDrvPriv;
}hsuart_wr_async_info;

typedef struct tagUART_FEATURE
{
    int baud;
    int wlen;
    int stp2;
    int eps;
	u32 lcr;
	u32 mcr;
    volatile BSP_BOOL rts;
    volatile BSP_BOOL afce;
}hsuart_feature;

/* UART 内存信息 */
typedef struct tagUART_MEM_NODE
{
    struct list_head list_node;
    uart_mem_state state;
    mem_desc_t mem;
    BSP_S32 valid_size;

    struct sk_buff *pSkb;
}uart_mem_node;

/* UART数据队列结构体*/
typedef struct tagUART_TRANS_INFO_S
{
    struct spinlock list_lock;
    struct list_head free_list;
    struct list_head done_list;
    uart_mem_node *pmem_start;                   /* MemNode数组首地址 */
    uart_mem_node *pmem_end;
    uart_mem_node *pcur_pos;                   /* 当前使用节点*/
    BSP_U32  node_size;
    BSP_U32  node_num;
    volatile BSP_U32  done_cnt;
    BSP_U32 total_bytes;                      /* 总计数 */
}hsuart_trans_info;
#if 0
/* dma 信息结构体*/
typedef struct tagUART_DMA_INFO_S
{
    volatile BSP_BOOL rx_dma_done;
 //   volatile BSP_U32  burst_delay;
}hsuart_dma_info;
#endif
typedef struct tagHSUART_CTX_S
{  
    hsuart_dev dev;
    UDI_DEVICE_ID udi_device_id;
    osl_sem_id tx_sema;
    osl_sem_id dma_sema;
    BSP_BOOL init;
    BSP_BOOL open;
    BSP_BOOL baud_adapt;
    BSP_U32 baud_idx;
    volatile detect_stage stage;
    //uart_timer switch_timer;
    volatile BSP_BOOL rts;
    //volatile BSP_BOOL tx_running;
    uart_transfer_mode transfer_mode;
    hsuart_trans_info read_info;                     /* 读信息*/
    hsuart_trans_info write_info;                    /* 写信息*/
    hsuart_feature feature;
    //hsuart_dma_info dma_info;
    hsuart_udi_cbs cbs;
    hsuart_stat stat;	
}hsuart_ctx;

struct hsuart_ctrl
{
	struct softtimer_list switch_state3_timer;
    struct softtimer_list adapt_timer;
	HSUART_TASK_ID rx_task_id;
	HSUART_TASK_ID tx_task_id;
	u32 ACSHELL_FLAG;
};

struct hsuart_dma_ctrl
{
	volatile u32 channel_id;
	volatile u32 irq_flag;
	volatile u32 total_size;
	volatile u32 pMemNode_startaddr;
	volatile u32 pMemNode_viraddr;
	volatile u32 dma_done_flag;
	volatile u32 dma_run_flag;
	volatile BSP_BOOL rx_dma_done;
	osl_sem_id edma_rx_sem;
};


void switch_stage_three(u32 arg);
extern int BSP_UDI_SetPrivate(UDI_DEVICE_ID devId, VOID* pPrivate);
extern int BSP_UDI_SetCapability(UDI_DEVICE_ID devId, BSP_U32 u32Capability);
extern int BSP_UDI_SetInterfaceTable(UDI_DEVICE_ID devId, UDI_DRV_INTEFACE_TABLE *pDrvInterface);
extern unsigned int TTF_VIRT_TO_PHY(unsigned int ulVAddr);
extern s32 hsuart_shell_init(DRV_DIALUP_HSUART_STRU acshell_nv_stru);
extern void hsuart_acshell_resume(void);
extern hsuart_ctx m2m_hsuart_ctx;
#ifdef __cplusplus /* __cplusplus */
}
#endif /* __cplusplus */
#endif

