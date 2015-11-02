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
/*lint -save -e537 -e64 -e124 -e413 -e613 -e533 -e713 -e718 -e732 -e737 -e730 -e529*/
/*lint -save -e550 -e438 -e527 -e716 -e958 -e718 -e746 -e650 -e734 -e545*/
#include <linux/kernel.h>       /*kmalloc,printk*/
#include <linux/module.h>
#include <linux/kthread.h>      
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/dma-mapping.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/platform_device.h>
#include <asm/irq.h>
#include "osl_malloc.h"
#include "osl_thread.h"
#include <bsp_edma.h>
#include <bsp_hardtimer.h>
#include "drv_udi.h"
#include "drv_acm.h"
#include "hi_uart.h"
#include "dialup_hsuart.h"
/*lint -save -e110 -e82*/
hsuart_ctx  m2m_hsuart_ctx = {
    .dev = {
        .irq = INT_LVL_HSUART,
        .clock = 48000000,
        .phy_addr_base = HI_HSUART_REGBASE_ADDR,
        .dma_req_rx = EDMA_HSUART_RX,
        .dma_req_tx = EDMA_HSUART_TX
    },
    .udi_device_id = UDI_HSUART_0_ID,
    .transfer_mode = DMA_TRANSFER,
};

static int typical_baud[] = {9600, 19200, 38400, 57600, 115200, 375000, 600000, 750000, 1000000, 3000000};/*参考芯片资料*/
static const char switch_cmd[] = "+++";

struct hsuart_ctrl g_hsuart_ctrl ={{0},{0},0};
struct hsuart_dma_ctrl g_dma_ctrl ={0};

extern struct clk *clk_get(char *dev, const char *con_id);
extern int clk_enable(struct clk * c);
extern void clk_disable(struct clk * c);
extern s32 dialup_hsuart_send(u8 * pucSrcAddr, u32 u32TransLength);


/*******************************************************************************
* 函 数 名  : hsuart_fifo_reset
* 功能描述  : 复位uart fifo
* 输入参数  : uart_dev 设备指针
* 输出参数  : NA
* 返 回 值  : NA
*******************************************************************************/
static inline void hsuart_fifo_reset(hsuart_dev *uart_dev)
{
    u32 val = 0;
    val = (UART_DEF_RT_ONEFOUR|UART_DEF_TET_ONETWO|UART_DEF_DMA_MODE|UART_TX_FIFO_RESET|UART_RX_FIFO_RESET|UART_FCR_FIFO_ENABLE);
    (void)writel(val, uart_dev->vir_addr_base + UART_REGOFF_FCR);
}
/*******************************************************************************
* 函 数 名  : hsuart_set_rts
* 功能描述  : 设置流控
* 输入参数  : uart_dev 设备指针
* 输出参数  : NA
* 返 回 值  : NA
*******************************************************************************/
static inline void hsuart_set_rts(hsuart_dev *uart_dev)
{
    u32 val = 0;
    hsuart_ctx *uart_ctx = NULL;
    
    uart_ctx = container_of(uart_dev, hsuart_ctx, dev);
    val = readl(uart_dev->vir_addr_base + UART_REG_OFFSET_MCR);
    val |= (UART_RTS_ENABLE|UART_AFCE_ENABLE);
    (void)writel(val, uart_dev->vir_addr_base + UART_REG_OFFSET_MCR); 
}
/*******************************************************************************
* 函 数 名  : hsuart_clear_rts
* 功能描述  : 禁能UART
* 输入参数  : uart_dev 设备指针
* 输出参数  : NA
* 返 回 值  : NA
*******************************************************************************/
static inline void hsuart_clear_rts(hsuart_dev *uart_dev)
{
    u32 val = 0;
    hsuart_ctx *uart_ctx = NULL;
    
    uart_ctx = container_of(uart_dev, hsuart_ctx, dev);
    val = readl(uart_dev->vir_addr_base + UART_REG_OFFSET_MCR);
	val &= (~(UART_RTS_ENABLE|UART_AFCE_ENABLE));
    (void)writel(val, uart_dev->vir_addr_base + UART_REG_OFFSET_MCR);
}
/*****************************************************************************
* 函 数 名  : hsuart_set_baud
* 功能描述  : UART 设置波特率函数
* 输入参数  : uart_dev: 设备管理结构指针
*             baud: 波特率值
* 输出参数  :
* 返 回 值  : 成功/失败
*****************************************************************************/
int hsuart_set_baud(hsuart_dev *uart_dev, int baud)
{
	u32 divisor = 0;
	u32 divisor_dll = 0;
    u32 divisor_dlh = 0;
	u32 val = 0;
	u32 u32Times = 0;
	u32 u32Discard = 0;
    hsuart_ctx *uart_ctx = NULL;

    if (NULL == uart_dev || baud < 0 || (unsigned int)baud > (uart_dev->clock >> 4))
    {
        hsuart_error("uart_dev:0x%x   baud:%d\n", (unsigned int)uart_dev, baud);
        return ERROR;
    }
    
    uart_ctx = container_of(uart_dev, hsuart_ctx, dev);
    if (0 == baud)
    {
        uart_ctx->baud_adapt = HSUART_TRUE;
        uart_ctx->baud_idx = (uart_ctx->baud_idx + 1) % (sizeof(typical_baud) / sizeof(int));
        baud = typical_baud[uart_ctx->baud_idx];
		bsp_softtimer_add(&g_hsuart_ctrl.adapt_timer);
        hsuart_debug("baud adapt start");
    }
    uart_ctx->feature.baud = baud;
	
    hsuart_debug("set baud: %d\n", baud);
    
	divisor =  uart_dev->clock/ (u32)(16 * baud);
	divisor_dll = divisor & 0xFF;
	divisor_dlh = (divisor & 0xFF00) >> 8;

	/* 读取UART 状态寄存器，看UART是否处于idle状态*/
    u32Discard = readl(uart_dev->vir_addr_base + UART_REGOFF_USR);
    while((u32Discard & 0x01)&&(u32Times<1000))
    {
    	u32Discard = readl(uart_dev->vir_addr_base + UART_REGOFF_USR);
        u32Times++;
    }
    if(1000 == u32Times)
    {
		hsuart_error("UART BUSY\n");
		return ERROR;
    }	
	/* 打开 DLL/DHL */
	val = readl(uart_dev->vir_addr_base + UART_REGOFF_LCR);
    writel(UART_LCR_DLAB, uart_dev->vir_addr_base + UART_REGOFF_LCR);
    /* 配置DLH 和DLL 寄存器，设置串行通讯的波特率*/
    writel(divisor_dll, uart_dev->vir_addr_base + UART_REGOFF_DLL);
    writel(divisor_dlh, uart_dev->vir_addr_base + UART_REGOFF_DLH);
    /*禁止 DLL&DLH */
	val &= ~UART_LCR_DLAB;
    writel(val, uart_dev->vir_addr_base + UART_REGOFF_LCR); 
    return OK;
}
/*******************************************************************************
* 函 数 名  : set_next_adapt
* 功能描述  : 波特率
* 输入参数  : uart_dev 设备指针
* 输出参数  : NA
* 返 回 值  : NA
*******************************************************************************/
void set_next_adapt(u32 arg)
{
    hsuart_ctx *uart_ctx = (hsuart_ctx *)arg;
    hsuart_fifo_reset(&uart_ctx->dev);
    hsuart_set_baud(&uart_ctx->dev, 0);
}
/*****************************************************************************
* 函 数 名  : hsuart_set_wlen
* 功能描述  : UART 设置数据位长函数
* 输入参数  : uart_dev: 设备管理结构指针
*             w_len: 数据位长
* 输出参数  :
* 返 回 值  : 成功/失败
*****************************************************************************/
int hsuart_set_wlen(hsuart_dev *uart_dev, int w_len)
{
	u32 val = 0;
    u32 wlen = 0;
    
    hsuart_debug("set wlen: %d\n", w_len);    
    switch(w_len)
    {
        case WLEN_5_BITS:
            wlen = UART_LCR_DLS_5BITS;
            break;
        case WLEN_6_BITS:
            wlen = UART_LCR_DLS_6BITS;
            break;
        case WLEN_7_BITS:
            wlen = UART_LCR_DLS_7BITS;
            break;
        case WLEN_8_BITS:
            wlen = UART_LCR_DLS_8BITS;
            break;
        default:
            hsuart_debug("unsupport w_len:%d\n", w_len);
            return ERROR;
    }
 
    /*clear wlen bits*/
    val = readl(uart_dev->vir_addr_base + UART_REGOFF_LCR);
    val &= (~UART_LCR_DLS_MASK); 
    /*set new wlen*/
    val |= (unsigned int)wlen;
    (void)writel(val, uart_dev->vir_addr_base + UART_REGOFF_LCR);
    return OK;
}
/*****************************************************************************
* 函 数 名  : hsuart_set_stp2
* 功能描述  : UART 设置停止位长函数
* 输入参数  : uart_dev: 设备管理结构指针
*             stp2: 两个停止位开关
* 输出参数  :
* 返 回 值  : 成功/失败
*****************************************************************************/
int hsuart_set_stpbit(hsuart_dev *uart_dev, int stp2)
{
    u32 val = 0;  
    hsuart_debug("set stp2: %d\n", stp2);
	val = readl(uart_dev->vir_addr_base + UART_REGOFF_LCR);
    if(stp2 == STP2_OFF)
    {
		val &= ~UART_LCR_STOP_2BITS;
    }
   	else if(stp2 == STP2_ON)
   	{
        val |= UART_LCR_STOP_2BITS;
   	}
	else
	{
		hsuart_error("unsupport stp2:%d\n", stp2);
        return ERROR;
    }
	(void)writel(val, uart_dev->vir_addr_base + UART_REGOFF_LCR);
    return OK;
}
/*****************************************************************************
* 函 数 名  : hsuart_set_eps
* 功能描述  : UART 设置校验方式函数
* 输入参数  : uart_dev: 设备管理结构指针
*             eps: 校验方式
* 输出参数  :
* 返 回 值  : 成功/失败
*****************************************************************************/
int hsuart_set_eps(hsuart_dev *uart_dev, int eps)
{
    u32 val = 0;
	hsuart_debug("set eps: %d\n", eps);
	val = readl(uart_dev->vir_addr_base + UART_REGOFF_LCR);
    if(eps == PARITY_NO_CHECK)   	/*无校验位*/
    {		
        val &= ~UART_LCR_PEN;
    }	
    else if(eps == PARITY_CHECK_ODD)  	/*奇校验*/
    {
            val |= UART_LCR_PEN;
			val &= ~UART_LCR_EPS;
    }	
    else if(eps == PARITY_CHECK_EVEN)	/*偶校验*/
    {
		val |= (UART_LCR_PEN | UART_LCR_EPS);
    }		
	else
	{
		hsuart_error("unsupport eps:%d\n", eps);
        return ERROR;
    }
	(void)writel(val, uart_dev->vir_addr_base + UART_REGOFF_LCR);
    return OK;
}

/*****************************************************************************
* 函 数 名  : hsuart_alloc_skb
* 功能描述  :分配SKB
* 输入参数  : pMemNode 数据节点size 缓存大小
* 输出参数  : NA
* 返 回 值  : 失败/成功
*****************************************************************************/
s32 hsuart_alloc_skb(uart_mem_node* pMemNode, u32 size)
{
	pMemNode->pSkb = dev_alloc_skb(size);   
	if (NULL == pMemNode->pSkb)
    {
        hsuart_error("pMemNode buff alloc fail, size:%d\n", size);
        return ERROR;
    }
    pMemNode->mem.vaddr = (void*)pMemNode->pSkb->data;
  	pMemNode->mem.paddr = dma_map_single(NULL, pMemNode->mem.vaddr, size, DMA_FROM_DEVICE);
    (void)memset(pMemNode->mem.vaddr, 0, size);
    return OK;
}
/*****************************************************************************
* 函 数 名  : add_mem_to_freelist
* 功能描述  : UART UDI 添加节点到 FreeList 中
* 输入参数  : trans_info: 读写结构体
*             pMemNode: 待添加的节点
* 输出参数  : NA
* 返 回 值  : NA
*****************************************************************************/
void add_mem_to_freelist(hsuart_trans_info *trans_info, uart_mem_node* pMemNode)
{
    unsigned long flags = 0;   
    spin_lock_irqsave(&trans_info->list_lock, flags); 
    list_add_tail(&pMemNode->list_node, &(trans_info->free_list));
    /*去初始化pMemNode成员*/
    pMemNode->state = UART_MEM_IN_FREELIST;
    pMemNode->valid_size = 0;
    pMemNode->pSkb = NULL;
    pMemNode->mem.vaddr = (void*)NULL;
    pMemNode->mem.paddr = (phys_addr)NULL;  
    spin_unlock_irqrestore(&trans_info->list_lock, flags);
}
/*****************************************************************************
* 函 数 名  : get_mem_from_freelist
* 功能描述  : UART UDI 从FreeList 头取出一个节点
* 输入参数  : trans_info: 读写结构体
* 输出参数  : NA
* 返 回 值  : 返回的节点指针
*****************************************************************************/
uart_mem_node* get_mem_from_freelist(hsuart_trans_info *trans_info)
{
    struct list_head *pListNode;
    uart_mem_node* pMemNode;
    unsigned long flags = 0;  
    spin_lock_irqsave(&trans_info->list_lock, flags);  
    if (list_empty(&(trans_info->free_list)))
    {
        pMemNode = NULL;
    }
    else
    {
        pListNode = trans_info->free_list.next;
        pMemNode = list_entry(pListNode, uart_mem_node, list_node);
        
        list_del_init(pListNode);
        pMemNode->state = UART_MEM_NOT_IN_LIST;
    }   
    spin_unlock_irqrestore(&trans_info->list_lock, flags);   
    return pMemNode;
}
/*****************************************************************************
* 函 数 名  : add_mem_to_donelist
* 功能描述  : UART UDI 添加节点到 完成链表 中
* 输入参数  : trans_info: uart 读写结构体指针
*             pMemNode: 待添加的节点
* 输出参数  : NA
* 返 回 值  : NA
*****************************************************************************/
void add_mem_to_donelist(hsuart_trans_info *trans_info, uart_mem_node* pMemNode)
{
	unsigned long flags = 0;

    spin_lock_irqsave(&trans_info->list_lock, flags);
    list_add_tail(&pMemNode->list_node, &(trans_info->done_list));
    trans_info->done_cnt++;
    pMemNode->state = UART_MEM_IN_DONELIST;    
    spin_unlock_irqrestore(&trans_info->list_lock, flags);
}
/*****************************************************************************
* 函 数 名  : get_mem_from_donelist
* 功能描述  : UART UDI 从DoneList 头取出一个节点
* 输入参数  : trans_info: 读写结构体
* 输出参数  : NA
* 返 回 值  : 返回的节点指针
*****************************************************************************/
uart_mem_node* get_mem_from_donelist(hsuart_trans_info *trans_info)
{
    struct list_head *pListNode = NULL;
    uart_mem_node* pMemNode = NULL;
    unsigned long flags = 0;

    spin_lock_irqsave(&trans_info->list_lock, flags);   
    if (list_empty(&(trans_info->done_list)))
    {
		//hsuart_error("pMemNode is null!\n");
		pMemNode = NULL;
    }
    else
    {    
        pListNode = trans_info->done_list.next;
        pMemNode = list_entry(pListNode, uart_mem_node, list_node);
        list_del_init(pListNode);
        trans_info->done_cnt--;
        pMemNode->state = UART_MEM_NOT_IN_LIST;
    }    
    spin_unlock_irqrestore(&trans_info->list_lock, flags); 
    return pMemNode;
}

/*****************************************************************************
* 函 数 名  : get_list_node_num
* 功能描述  : 获取链表中的节点个数
* 输入参数  : pFreeHeader: Free List头节点
* 输出参数  : 返回的节点个数
* 返 回 值  : 节点个数
*****************************************************************************/
s32 get_list_node_num(struct list_head *pHeader)
{
    struct list_head* pCurPos = NULL;
    struct list_head* pNextPos = NULL;
    s32 s32Num = 0;
    list_for_each_safe(pCurPos, pNextPos, pHeader)
    {
        s32Num++;
    }  
    return s32Num;
}
/*****************************************************************************
* 函 数 名  : hsuart_uninit_list
* 功能描述  : 去初始化读写链表
* 输入参数  : trans_info: 读写结构体
* 输出参数  : NA
* 返 回 值  : NA
*****************************************************************************/
void hsuart_uninit_list(hsuart_trans_info *trans_info)
{
    u32 idx = 0;
    unsigned long flags = 0;

    if(NULL == trans_info)
	{
       hsuart_error("trans_info is NULL\n");
	   return;
	}
    spin_lock_irqsave(&trans_info->list_lock, flags);    
    INIT_LIST_HEAD(&(trans_info->free_list));
    INIT_LIST_HEAD(&(trans_info->done_list));
  
    if (trans_info->pmem_start)
    {
        for (idx = 0; idx < trans_info->node_num; idx++)
        {
            INIT_LIST_HEAD(&(trans_info->pmem_start[idx].list_node));
            trans_info->pmem_start[idx].state = UART_MEM_NOT_IN_LIST;
            trans_info->pmem_start[idx].mem.vaddr = (void*)NULL;
            trans_info->pmem_start[idx].mem.paddr = (phys_addr)NULL;
            trans_info->pmem_start[idx].valid_size = 0;
            if (trans_info->pmem_start[idx].pSkb)
            {
                dev_kfree_skb(trans_info->pmem_start[idx].pSkb);
                trans_info->pmem_start[idx].pSkb = NULL;
            }
        }

        kfree(trans_info->pmem_start);
        trans_info->pmem_start = NULL;
    }
    
    trans_info->pmem_end = NULL;
    trans_info->pcur_pos = NULL;
    trans_info->node_num = 0;
    trans_info->node_size = 0;
    trans_info->done_cnt = 0;
    trans_info->total_bytes = 0;
    spin_unlock_irqrestore(&trans_info->list_lock, flags);
}
/*****************************************************************************
* 函 数 名  : hsuart_init_list
* 功能描述  : 初始化读写链表
* 输入参数  : trans_info: 读写结构体
* 输出参数  : NA
* 返 回 值  : 成功/失败
*****************************************************************************/
s32 hsuart_init_list(hsuart_trans_info *trans_info)
{
    u8* pMemHead = NULL;
    u32 idx = 0;
    s32 ret = OK;
    unsigned long flags = 0;
	struct list_head *pListNode = NULL;
	
    if(NULL == trans_info)
	{
       hsuart_error("trans_info is NULL\n");
	   return ERROR;
	}
    spin_lock_irqsave(&trans_info->list_lock, flags);//阻塞信号量
    INIT_LIST_HEAD(&(trans_info->free_list));
    INIT_LIST_HEAD(&(trans_info->done_list));
    
    pMemHead = osl_malloc(trans_info->node_num*sizeof(uart_mem_node));
    if (pMemHead)
    {
        (void)memset(pMemHead, 0, (trans_info->node_num*sizeof(uart_mem_node)));
        trans_info->pmem_start = (uart_mem_node*)pMemHead;
        trans_info->pmem_end = (uart_mem_node*)(pMemHead + (trans_info->node_num*sizeof(uart_mem_node)));
        trans_info->pcur_pos = NULL;
        trans_info->done_cnt = 0;
        trans_info->total_bytes = 0;

        /* 初始化时,所有节点都在FreeList */
        for (idx = 0; idx < trans_info->node_num; idx++)
        {
            pListNode = &(trans_info->pmem_start[idx].list_node);
            list_add_tail(pListNode, &(trans_info->free_list));
            trans_info->pmem_start[idx].state = UART_MEM_IN_FREELIST;
            trans_info->pmem_start[idx].valid_size = 0;
            (void)memset(&(trans_info->pmem_start[idx].mem), 0, sizeof(mem_desc_t));
            trans_info->pmem_start[idx].pSkb = NULL;
        }
    }
    else
    {
        ret = ERROR;
    }
    spin_unlock_irqrestore(&trans_info->list_lock, flags);

    return ret;
}
/*****************************************************************************
* 函 数 名  : hsuart_buf_init
* 功能描述  :初始化读写缓存队列
* 输入参数  : uart_ctx 上下文
* 输出参数  : NA
* 返 回 值  : 成功/失败
*****************************************************************************/
s32 hsuart_buf_init(hsuart_ctx *uart_ctx)
{
    s32 ret = OK;	
    if(NULL == uart_ctx)
	{
       hsuart_error("uart_ctx is NULL\n");
	   return ERROR;
	}
    memset(&(uart_ctx->read_info), 0, sizeof(hsuart_trans_info));
    memset(&(uart_ctx->write_info), 0, sizeof(hsuart_trans_info));
    
    spin_lock_init(&(uart_ctx->read_info.list_lock));
    spin_lock_init(&(uart_ctx->write_info.list_lock));

    uart_ctx->read_info.node_num = UART_READ_BUF_NUM;   //读节点数量16
    uart_ctx->read_info.node_size = UART_READ_BUF_SIZE; //每个节点大小1536字节
    uart_ctx->write_info.node_num = UART_WRITE_BUF_NUM; //写节点数量64
    uart_ctx->write_info.node_size = 0;					//每个写节点大小0
	if (hsuart_init_list(&(uart_ctx->read_info)) || hsuart_init_list(&(uart_ctx->write_info)))
    {
        hsuart_error("hsuart_init_list error.\n");
        ret = ERROR;
    }
    return ret;
}
/*****************************************************************************
* 函 数 名  : gpio_dtr_irq
* 功能描述  : GPIO 中断处理
* 输入参数  : irq 中断号 dev_id 传参
* 输出参数  : NA
* 返 回 值  : 成功/失败bcmsdh_register_oob_intr
*****************************************************************************/
static irqreturn_t gpio_dtr_irq(int irq, void *dev_id)
{
    hsuart_ctx* uart_ctx = (hsuart_ctx*)dev_id;
    MODEM_MSC_STRU modem_msc = {0};
	if(!gpio_int_state_get(HSUART_DTR))
	{
		hsuart_error("gpio_dtr_irq:%d\n", modem_msc.ucDtr);
		return IRQ_NONE;
	}
    gpio_int_mask_set(HSUART_DTR);
	
    memset(&modem_msc, 0, sizeof(MODEM_MSC_STRU));
    modem_msc.OP_Dtr = SIGNALCH;
    modem_msc.ucDtr = !(BSP_U8)gpio_get_value(HSUART_DTR);
    hsuart_debug("gpio_dtr_irq:%d\n", modem_msc.ucDtr);
	if (uart_ctx->cbs.msc_read_cb)
    {
        uart_ctx->cbs.msc_read_cb(&modem_msc);
    }
    else
    {
        hsuart_error("value:%d  msc_read_cb is NULL.\n", modem_msc.ucDtr);
    }
	
    if(gpio_get_value(HSUART_DTR))
    {
    	gpio_int_trigger_set(HSUART_DTR , IRQ_TYPE_EDGE_FALLING);
    }
    else
    {
     	gpio_int_trigger_set(HSUART_DTR , IRQ_TYPE_EDGE_RISING); 
    }

    gpio_int_state_clear(HSUART_DTR);
    gpio_int_unmask_set(HSUART_DTR);  
    
    return IRQ_HANDLED;
}
/*****************************************************************************
* 函 数 名  : hsuart_gpio_init
* 功能描述  : uart gpio 模拟管脚初始化函数
* 输入参数  : uart_ctx 上下文
* 输出参数  : NA
* 返 回 值  : 成功/失败bcmsdh_register_oob_intr
* 备	注	:来自gpio配置手册通用流程
*****************************************************************************/
s32 hsuart_gpio_init(hsuart_ctx *uart_ctx)
{
    s32 ret;
    s32 irq_id = 0;
	hsuart_debug("hsuart_gpio_init enter...!\n");

    if (gpio_request(HSUART_RING, "GPIO_RING"))
    {
        hsuart_error("gpio=%d is busy.\n", HSUART_RING);
        return ERROR;
    }

    if (gpio_request(HSUART_DSR, "GPIO_DSR"))
    {
        hsuart_error("gpio=%d is busy.\n", HSUART_DSR);
        return ERROR;
    }

    if (gpio_request(HSUART_DCD, "GPIO_DCD"))
    {
        hsuart_error("gpio=%d is busy.\n", HSUART_DCD);
        return ERROR;
    }
  
    gpio_direction_output(HSUART_RING, HIGHLEVEL); 
    gpio_direction_output(HSUART_DSR, HIGHLEVEL); 
    gpio_direction_output(HSUART_DCD, HIGHLEVEL); 
    
    if (gpio_request(HSUART_DTR, "GPIO_DTR"))
    {
        hsuart_error("gpio=%d is busy\n", HSUART_DTR);
        return ERROR;
    }
    
    gpio_direction_input(HSUART_DTR);     
    gpio_int_mask_set(HSUART_DTR);    
    gpio_int_state_clear(HSUART_DTR); 
	if(gpio_get_value(HSUART_DTR))
    {
    	gpio_int_trigger_set(HSUART_DTR , IRQ_TYPE_EDGE_FALLING);
    }
    else
    {
     	gpio_int_trigger_set(HSUART_DTR , IRQ_TYPE_EDGE_RISING); 
    }
	gpio_set_function(HSUART_DTR, GPIO_INTERRUPT);
    hsuart_error("GPIO: %d gpio_set_function done\n", HSUART_DTR);
    
	irq_id = gpio_to_irq(HSUART_DTR);
    ret = request_irq(irq_id, gpio_dtr_irq, IRQF_NO_SUSPEND | IRQF_SHARED,"HSUART_DTR", (void*)uart_ctx);
    if (ret != 0) 
	{
        hsuart_error("failed at register gpio-oob irq\n");
        return ERROR;
    }   
    gpio_int_state_clear(HSUART_DTR);
    gpio_int_unmask_set(HSUART_DTR);    
    return OK;
}
/*****************************************************************************
* 函 数 名  : hsuart_dma_tx_complete
* 功能描述  : UART DMA 模式发送完成函数
* 输入参数  : arg: 设备管理结构指针
*             int_status: 中断状态
* 输出参数  :
* 返 回 值  : NA
*****************************************************************************/
void hsuart_dma_tx_complete(u32 arg, u32 int_status)
{
    hsuart_ctx *uart_ctx = (hsuart_ctx *)arg;
    up(&uart_ctx->dma_sema);
}
/*****************************************************************************
* 函 数 名  : hsuart_tx_dma
* 功能描述  : UART DMA 模式发送数据函数
* 输入参数  : uart_dev: 设备管理结构指针
*             dma_handle: 发送数据缓存地址
*             length: 发送数据长度
* 输出参数  :
* 返 回 值  : 发送数据长度
*****************************************************************************/
s32 hsuart_tx_dma(hsuart_dev *uart_dev, u32 dma_handle, u32 length)
{
    s32 ret_id = 0;
    hsuart_ctx *uart_ctx = NULL;

    uart_ctx = container_of(uart_dev, hsuart_ctx, dev);
    
	ret_id = bsp_edma_channel_init(uart_dev->dma_req_tx, hsuart_dma_tx_complete, (u32)uart_ctx, BALONG_DMA_INT_DONE);
	if (ret_id < 0)
    {
        hsuart_error("balong_dma_channel_init error ret_id = %d\n", ret_id);
        return ERROR;
    }
    if (bsp_edma_channel_set_config(ret_id, BALONG_DMA_M2P, EDMA_TRANS_WIDTH_32, EDMA_BUR_LEN_16))
    {
        hsuart_error("balong_dma_channel_set_config failed! ret_id = %d\n", ret_id);
        return ERROR;
    }
    if (bsp_edma_channel_async_start(ret_id, dma_handle, (u32)(uart_dev->phy_addr_base + UART_REGOFF_THR), length))
    {
        hsuart_error("balong_dma_channel_async_start transfer failed! ret_id = %d\n", ret_id);
        return ERROR;
    }
    return length;
}
/*******************************************************************************
* 函 数 名  : switch_stage_three
* 功能描述  : 模式切换第三阶段
* 输入参数  : arg 传参
* 输出参数  : NA
* 返 回 值  : NA
*******************************************************************************/
void switch_stage_three(u32 arg)
{
    hsuart_ctx *uart_ctx = (hsuart_ctx *)arg;
    uart_mem_node* pMemNode = NULL;
    
    pMemNode = uart_ctx->read_info.pcur_pos;
    pMemNode->valid_size = 0;
    uart_ctx->stage = DETECT_STAGE_1;
    
	if (uart_ctx->cbs.switch_mode_cb)
 	{
      	uart_ctx->cbs.switch_mode_cb();
		hsuart_debug("switch cmd mode.\n");
  	}
  	else
	{
        hsuart_error("switch_mode_cb is NULL.\n");
    }
}

/*****************************************************************************
* 函 数 名  : switch_mode_cmd_detect
* 功能描述  : 模式切换命令检测函数
* 输入参数  : uart_ctx 上下文 pMemNode 数据节点指针
* 输出参数  : NA
* 返 回 值  : 成功/失败
*****************************************************************************/
s32 switch_mode_cmd_detect(hsuart_ctx *uart_ctx, uart_mem_node* pMemNode)
{
    int result = 0;
    
    if (DETECT_STAGE_2 == uart_ctx->stage)
    {
		//hsuart_debug("switch_mode_cmd_detect\n"); 
        if (pMemNode)	/*该数据节点内数据为"+++"*/
        {
            result = !memcmp((void*)switch_cmd, (void*)pMemNode->mem.vaddr, strlen(switch_cmd));
        	if((*((char *)pMemNode->mem.vaddr + 3)) == '\r' || (*((char *)pMemNode->mem.vaddr + 3)) == '\n')
			{
				--pMemNode->valid_size;
				if ((*((char *)pMemNode->mem.vaddr + 4)) == '\n' || (*((char *)pMemNode->mem.vaddr + 4)) == '\r')
				{
					--pMemNode->valid_size;
		    	}
        	}
			result = result && (pMemNode->valid_size == strlen(switch_cmd));
			
        	if (result)
        	{
				uart_ctx->stage = DETECT_STAGE_3;
	            bsp_softtimer_add(&g_hsuart_ctrl.switch_state3_timer);
        	}
        	else
        	{
				//hsuart_debug("DETECT_STAGE_2 to DETECT_STAGE_1\n");    
	            uart_ctx->stage = DETECT_STAGE_1;
        	}
        }
    }
    else
    {
		uart_ctx->stage = DETECT_STAGE_1;
	}
    return result;
}
/*****************************************************************************
* 函 数 名  : wait_tx_fifo_empty
* 功能描述  : UART 等待发送完成
* 输入参数  : ctx 传参
* 输出参数  : NA
* 返 回 值  : NA
*****************************************************************************/
void wait_tx_fifo_empty(hsuart_ctx *uart_ctx)
{
    u32 cnt = 0;
    u32 status;
    
    status = readl(uart_ctx->dev.vir_addr_base + UART_REGOFF_USR);
    while (0 == (status & UART_USR_FIFO_EMP))
    {
        msleep(1);
        if (10000 < cnt++)
        {
            hsuart_error("wait time out.\n");
            break;
        }
        status = readl(uart_ctx->dev.vir_addr_base + UART_REGOFF_USR);
    }
}
/*****************************************************************************
* 函 数 名  : set_hsuart_feature
* 功能描述  : uart 设置属性
* 输入参数  : uart_ctx:uart上下文
* 输出参数  : NA
* 返 回 值  : NA
*****************************************************************************/
void set_hsuart_feature(hsuart_ctx *uart_ctx)
{
    unsigned long flags = 0;
    
    if (uart_ctx->cbs.set_baud_cb || uart_ctx->cbs.set_wlen_cb || uart_ctx->cbs.set_stp2_cb || uart_ctx->cbs.set_eps_cb)
    {
        wait_tx_fifo_empty(uart_ctx);
        hsuart_fifo_reset(&uart_ctx->dev);
		spin_lock_irqsave(&(uart_ctx->dev.lock_irq), flags);
        if (uart_ctx->cbs.set_baud_cb)
        {
            uart_ctx->cbs.set_baud_cb(&uart_ctx->dev, uart_ctx->feature.baud);
            uart_ctx->cbs.set_baud_cb = NULL;
        }
        if (uart_ctx->cbs.set_wlen_cb)
        {
            uart_ctx->cbs.set_wlen_cb(&uart_ctx->dev, uart_ctx->feature.wlen);
            uart_ctx->cbs.set_wlen_cb = NULL;
        }
        if (uart_ctx->cbs.set_stp2_cb)
        {
            uart_ctx->cbs.set_stp2_cb(&uart_ctx->dev, uart_ctx->feature.stp2);
            uart_ctx->cbs.set_stp2_cb = NULL;
        }
        if (uart_ctx->cbs.set_eps_cb)
        {
            uart_ctx->cbs.set_eps_cb(&uart_ctx->dev, uart_ctx->feature.eps);
            uart_ctx->cbs.set_eps_cb = NULL;
        }
        spin_unlock_irqrestore(&(uart_ctx->dev.lock_irq), flags);
    }
}
/*******************************************************************************
* 函 数 名  : get_read_node
* 功能描述  : UART 获取当前写地址
* 输入参数  : uart_ctx 上下文
* 输出参数  : NA
* 返 回 值  : 写地址
*******************************************************************************/
uart_mem_node* get_read_node(hsuart_ctx *uart_ctx)
{
    uart_mem_node* pMemNode;
	
    if (NULL == uart_ctx->read_info.pcur_pos)
    {
        pMemNode = get_mem_from_freelist(&(uart_ctx->read_info));
        if (pMemNode)
        {
            if (!pMemNode->pSkb)
            {
                if (OK != hsuart_alloc_skb(pMemNode, uart_ctx->read_info.node_size))
                {
                    add_mem_to_freelist(&(uart_ctx->read_info), pMemNode);
                    pMemNode = NULL;
                    uart_ctx->stat.alloc_skb_fail++;
                }
            }
            uart_ctx->read_info.pcur_pos = pMemNode;
        }
    }
    else
    {
        pMemNode = uart_ctx->read_info.pcur_pos;        
    }
    
    return pMemNode;
}
/*******************************************************************************
* 函 数 名  : check_mem_to_donelist
* 功能描述  : 接收数据检查
* 输入参数  : uart_ctx 上下文 pMemNode 内存节点
* 输出参数  : NA
* 返 回 值  : NA
*******************************************************************************/
s32 check_mem_to_donelist(hsuart_ctx *uart_ctx, uart_mem_node* pMemNode)
{
    s32 ret = ERROR;
	u32 i =0;
	char *adap_addr =NULL;
    if (uart_ctx->baud_adapt)
    {
		bsp_softtimer_delete(&g_hsuart_ctrl.adapt_timer);
		/*数据适配AT 是否成功*/
        if (memcmp("AT", pMemNode->mem.vaddr, 2) && memcmp("at", pMemNode->mem.vaddr, 2))
        {
            hsuart_debug("baud adapt fail!   idx:%d baud:%d\n", uart_ctx->baud_idx, typical_baud[uart_ctx->baud_idx]);
/*debug*/			
			adap_addr = (char *)pMemNode->mem.vaddr;
			for(i=0;i<pMemNode->valid_size;i++)
			{
				printk("adapt data 0x%x\n",*adap_addr);
				adap_addr++;
			}
           
/*debug*/			
            pMemNode->valid_size = 0;			
    		hsuart_fifo_reset(&uart_ctx->dev);
    		hsuart_set_baud(&uart_ctx->dev, 0);
        }
        else
        {
            hsuart_error("baud adapt success!   idx:%d baud:%d\n", uart_ctx->baud_idx, typical_baud[uart_ctx->baud_idx]);			
			uart_ctx->baud_adapt = HSUART_FALSE;
            add_mem_to_donelist(&uart_ctx->read_info, pMemNode);
            ret = OK;
        }
    }
    else
    {
        if (!switch_mode_cmd_detect(uart_ctx, pMemNode))
        {
            add_mem_to_donelist(&uart_ctx->read_info, pMemNode);
            ret = OK;
        }
    }
    return ret;
}
/*****************************************************************************
* 函 数 名  : hsuart_dma_tx_complete
* 功能描述  : UART DMA 模式发送完成函数
* 输入参数  : arg: 设备管理结构指针
*             int_status: 中断状态
* 输出参数  :
* 返 回 值  : NA
*****************************************************************************/
void hsuart_dma_rx_complete(u32 arg, u32 int_status)
{
    //hsuart_ctx *uart_ctx = (hsuart_ctx *)arg;
  
    hsuart_debug("UART RX DMA transfer complete.\n");
    //uart_ctx->dma_info.rx_dma_done = HSUART_TRUE;
	g_dma_ctrl.rx_dma_done = HSUART_TRUE;
	g_dma_ctrl.dma_done_flag = HSUART_TRUE;
    up(&g_dma_ctrl.edma_rx_sem);
}
/*****************************************************************************
* 函 数 名  : hsuart_rx_dma
* 功能描述  : UART DMA 模式接收数据函数
* 输入参数  : uart_dev: 设备管理结构指针
*             dma_handle: 接收数据缓存地址
*             length: 接收数据长度
* 输出参数  :
* 返 回 值  : DMA channel id
*****************************************************************************/
s32 hsuart_rx_dma(hsuart_dev *uart_dev, u32 dma_handle, u32 length)
{
    s32 ret_id = 0;
    hsuart_ctx *uart_ctx = NULL;

    uart_ctx = container_of(uart_dev, hsuart_ctx, dev);
    
    ret_id = bsp_edma_channel_init(uart_dev->dma_req_rx, hsuart_dma_rx_complete, (u32)uart_ctx, BALONG_DMA_INT_DONE);
    if (ret_id < 0)
    {
        hsuart_error("balong_dma_channel_init error ret_id = %d\n", ret_id);
        return ERROR;
    }
    if (bsp_edma_channel_set_config(ret_id, BALONG_DMA_P2M, EDMA_TRANS_WIDTH_32, EDMA_BUR_LEN_16))
    {
        hsuart_error("balong_dma_channel_set_config failed! ret_id = %d\n", ret_id);
        return ERROR;
    }  
    if (bsp_edma_channel_async_start(ret_id, (u32)(uart_dev->phy_addr_base + UART_REGOFF_RBR), dma_handle, length))
    {
        hsuart_error("balong_dma_channel_async_start transfer failed! ret_id = %d\n", ret_id);
        return ERROR;
    } 
    return ret_id;
}
/*******************************************************************************
* 函 数 名  : hsuart_rx_complete
* 功能描述  : 接收完成
* 输入参数  : uart_ctx 上下文
* 输出参数  : NA
* 返 回 值  : NA
*******************************************************************************/
void hsuart_rx_complete(hsuart_ctx *uart_ctx)
{
    uart_mem_node* pMemNode;
    
    pMemNode = uart_ctx->read_info.pcur_pos;
    if (!pMemNode)
    {
		hsuart_error("pMemNode is null!\n");
		return;
    }
    
   	if (uart_ctx->cbs.read_cb)
   	{
       	skb_put(pMemNode->pSkb, pMemNode->valid_size);		/*使用标准的API来修改len */
		if (OK == check_mem_to_donelist(uart_ctx, pMemNode))
        {
            uart_ctx->read_info.total_bytes += pMemNode->valid_size;
            uart_ctx->cbs.read_cb();
            uart_ctx->read_info.pcur_pos = NULL;
        }
    }
    else
    {
       hsuart_error("ReadCb is null.\n");
       pMemNode->valid_size = 0;
    }
}
/*****************************************************************************
* 函 数 名  : hsuart_burst_dma_transfer
* 功能描述  : UART dma数据搬移
* 输入参数  : uart_ctx ，pMemNode
* 输出参数  : NA
* 返 回 值  : NA
*****************************************************************************/
void hsuart_burst_dma_transfer(hsuart_ctx *uart_ctx, uart_mem_node* pMemNode)
{
    u32 start_addr = 0;
	hsuart_dev *uart_dev;
	u32 switch_scurrent_time = 0;
	u32 switch_delta_time = 0;
	u32 switch_last_time = 0;
	uart_dev = &uart_ctx->dev;
    //uart_ctx->dma_info.rx_dma_done
	g_dma_ctrl.rx_dma_done = HSUART_FALSE;
	g_dma_ctrl.dma_done_flag = HSUART_FALSE;
	
	pMemNode->mem.paddr = dma_map_single(NULL, pMemNode->mem.vaddr , uart_ctx->read_info.node_size,DMA_FROM_DEVICE);
	
    start_addr = (u32)pMemNode->mem.paddr;
	
	g_dma_ctrl.pMemNode_startaddr = (u32)pMemNode->mem.paddr;
	g_dma_ctrl.pMemNode_viraddr = (u32)pMemNode->mem.vaddr;
	g_dma_ctrl.channel_id = hsuart_rx_dma(&uart_ctx->dev, start_addr, uart_ctx->read_info.node_size);
	g_dma_ctrl.dma_run_flag = HSUART_TRUE;
	
	switch_scurrent_time = bsp_get_slice_value();

	(void)writel(0x10,uart_dev->vir_addr_base+ UART_REGOFF_IER);
	down(&g_dma_ctrl.edma_rx_sem);
	writel(UART_IER_IRQ_DISABLE, uart_dev->vir_addr_base + UART_REGOFF_IER);
	switch_last_time = bsp_get_slice_value();
	switch_delta_time = get_timer_slice_delta(switch_scurrent_time ,switch_last_time);
	//hsuart_debug("switch_scurrent_time %d\n",switch_delta_time);
	switch_delta_time = (switch_delta_time/SYS_TICK_32K)*1000;
	if(switch_delta_time >= SWITCH_TIMER_LENGTH && uart_ctx->stage == DETECT_STAGE_1)
	{
		uart_ctx->stage = DETECT_STAGE_2;
		//hsuart_debug("uart_ctx->stage is DETECT_STAGE_2\n");
	}
	
	else if(switch_delta_time >= SWITCH_TWO_TIMER_LENGTH && uart_ctx->stage == DETECT_STAGE_2)
	{
		uart_ctx->stage = DETECT_STAGE_1;
		//hsuart_debug("uart_ctx->stage change DETECT_STAGE_1\n");
	}
    if (g_dma_ctrl.rx_dma_done)
    {
		hsuart_error("burst dma transfer done!\n");
		pMemNode->valid_size = uart_ctx->read_info.node_size;
        uart_ctx->stat.burst_dma_done++;
		return;
    }
	else
	{
		pMemNode->valid_size =  g_dma_ctrl.total_size;
	}

	//hsuart_debug("\nrecv data %s\n",(char *)pMemNode->mem.vaddr);
	g_dma_ctrl.dma_run_flag = HSUART_FALSE;
}
/*****************************************************************************
* 函 数 名  : hsuart_rx_thread
* 功能描述  : UART 底层接收线程
* 输入参数  : ctx 传参
* 输出参数  : NA
* 返 回 值  : NA
*****************************************************************************/
s32 hsuart_rx_thread(void *ctx)
{
    hsuart_ctx *uart_ctx = (hsuart_ctx *)ctx;
    uart_mem_node* pMemNode = NULL;
    
    if(NULL == ctx)
	{
       hsuart_error("uart_ctx is NULL\n");
	   return ERROR;
	}
    while (1)
    {
		if (!uart_ctx->rts)
        {
            pMemNode = get_read_node(uart_ctx);
            if (pMemNode)
            {
                while (uart_ctx->read_info.pcur_pos)
                {
                    hsuart_burst_dma_transfer(uart_ctx, pMemNode);									
					bsp_softtimer_delete(&g_hsuart_ctrl.switch_state3_timer);
                    if (DETECT_STAGE_3 == uart_ctx->stage)
                    {
                        uart_ctx->stage = DETECT_STAGE_1;
                    }

                    if (pMemNode->valid_size)
                    {
                        hsuart_debug("valid_size:0x%x\n", (u32)pMemNode->valid_size);
                        hsuart_rx_complete(uart_ctx);
                    }
                }
				hsuart_debug("**********\n");
            }
            else
            {
                msleep(1);
				hsuart_error("pmemnode null\n");
            }
        }
        else
        {
            msleep(1);
			hsuart_error("rx thread idle\n");
        }   
  }
   
    return 0;
}
/*****************************************************************************
* 函 数 名  : hsuart_tx_thread
* 功能描述  : UART 底层发送线程
* 输入参数  : ctx 传参
* 输出参数  : NA
* 返 回 值  : NA
*****************************************************************************/
s32 hsuart_tx_thread(void *ctx)
{
    hsuart_ctx *uart_ctx = (hsuart_ctx *)ctx;
	hsuart_dev *uart_dev = &uart_ctx->dev;    

    uart_mem_node* pMemNode = NULL;
    
	if(NULL == ctx)
	{
       hsuart_error("uart_ctx is NULL\n");
	   return ERROR;
	}
    while (1)
    {
        if (!uart_ctx->baud_adapt)		/* 波特率自适应期间不上报消息*/
        {
           	//uart_ctx->tx_running = HSUART_FALSE;
            down(&(uart_ctx->tx_sema));
            //uart_ctx->tx_running = HSUART_TRUE;
            pMemNode = get_mem_from_donelist(&(uart_ctx->write_info));
            while (pMemNode)
            {
				hsuart_debug("enter. pSkb:0x%x, valid_size:0x%x\n", (unsigned int)pMemNode->pSkb, (unsigned int)pMemNode->valid_size);//debug
				hsuart_tx_dma(uart_dev, (u32)pMemNode->mem.paddr, pMemNode->valid_size); 				
				down(&uart_ctx->dma_sema);  

                if (uart_ctx->cbs.free_cb)
                {
                    uart_ctx->cbs.free_cb((char*)pMemNode->pSkb);
					if (pMemNode->pSkb)
					{
						pMemNode->pSkb = NULL;
					}
                }				
                else
                {
                    uart_ctx->stat.write_not_free_cnt++;
                    hsuart_error("no write free func register.\n");
                    kfree_skb(pMemNode->pSkb);
					pMemNode->pSkb = NULL;
                }
                add_mem_to_freelist(&(uart_ctx->write_info), pMemNode);
                pMemNode = get_mem_from_donelist(&(uart_ctx->write_info));
            }			
            set_hsuart_feature(uart_ctx);		/* NAS收到AT命令，返回给对方OK后开始配置uart属性 */
        }
        else
        {
            msleep(10);
        }
    }
    return 0;
}
/*****************************************************************************
* 函 数 名  : hsuart_irq
* 功能描述  : uart 中断服务程序
* 输入参数  : irq:中断号
*             dev_id:传参
* 输出参数  : NA
* 返 回 值  : 中断返回值
*****************************************************************************/
irqreturn_t hsuart_irq(int irq, void *ctx)
{
   	u32 ulInt = 0;
	u32 ulCharNum = 0;
	u32 Data = 0;
	u32 i = 0;
    u32 cur_addr = 0;
	u8 *recvTemp = 0;
	u32 cur_transfered = 0;
	u32 recv_addr = 0;
	hsuart_dev *uart_dev = NULL;
	uart_dev = (hsuart_dev *)ctx;
	
    ulInt = readl(uart_dev->vir_addr_base+ UART_REGOFF_IIR);
    ulInt &= UART_FIFO_MASK;
	
    if(ulInt == UART_IIR_REV_TIMEOUT && g_dma_ctrl.dma_done_flag == HSUART_FALSE) //接收超时中断
    {	
		hsuart_debug("irq\n");
		ulCharNum = readl(uart_dev->vir_addr_base + 0x84);
		if(ulCharNum == 0)
		{
			readl(uart_dev->vir_addr_base + UART_REGOFF_RBR);
			return (irqreturn_t)IRQ_NONE;
		}
		g_dma_ctrl.total_size = ulCharNum;
		cur_addr = bsp_edma_channel_stop(g_dma_ctrl.channel_id);
		cur_transfered = cur_addr - g_dma_ctrl.pMemNode_startaddr;
		recv_addr = (g_dma_ctrl.pMemNode_viraddr + cur_transfered);

		g_dma_ctrl.total_size += cur_transfered;
		while(ulCharNum)
		{
			if(ulCharNum >= 4)
			{
				Data = readl(uart_dev->vir_addr_base + UART_REGOFF_RBR);
				*(u32 *)recv_addr = Data;
				recv_addr += 4;
				ulCharNum = ulCharNum - 4;
			}
			else
			{	
				Data = readl(uart_dev->vir_addr_base + UART_REGOFF_RBR);
				recvTemp = (u8 *)&Data;
				for(i = 0; i <ulCharNum; i++)
				{		
					*(u8 *)recv_addr = recvTemp[i];
					recv_addr++;					
				}
				break;
			}	
		}
		up(&g_dma_ctrl.edma_rx_sem);
    }
	else
	{
		hsuart_error("rx edma running\n");
		if(ulCharNum == 0)
		{
			readl(uart_dev->vir_addr_base + UART_REGOFF_RBR);
			return (irqreturn_t)IRQ_NONE;
		}
	}
	//dma_ctrl.irq_flag = HSUART_TRUE;
	writel(UART_IER_IRQ_DISABLE, uart_dev->vir_addr_base + UART_REGOFF_IER);

	return (irqreturn_t)IRQ_HANDLED;
}

/*****************************************************************************
* 函 数 名  : hsuart_write_async
* 功能描述  : UART UDI 设备异步写实现
* 输入参数  : uart_ctx: 上下文
*             pWRInfo: 内存信息
* 输出参数  : NA
* 返 回 值  : 请求的 成功/失败
*****************************************************************************/
s32 hsuart_write_async(hsuart_ctx* uart_ctx, hsuart_wr_async_info* pWRInfo)
{
    struct sk_buff* tx_skb;
    uart_mem_node* pMemNode;

    if (NULL == pWRInfo)
    {
        hsuart_error("pWRInfo:0x%x\n", (unsigned int)pWRInfo);
        return ERROR;
    }
    tx_skb = (struct sk_buff*)pWRInfo->pBuffer;
    pMemNode = get_mem_from_freelist(&(uart_ctx->write_info));
    if (pMemNode)
    {
        pMemNode->pSkb = tx_skb;
        pMemNode->valid_size = pMemNode->pSkb->len;
        pMemNode->mem.vaddr = (void*)pMemNode->pSkb->data;
        pMemNode->mem.paddr = dma_map_single(NULL, pMemNode->mem.vaddr, pMemNode->valid_size, DMA_TO_DEVICE);
        add_mem_to_donelist(&(uart_ctx->write_info), pMemNode);
    }
    else
    {
        hsuart_error("no free node, tx_skb:0x%x.\n", (unsigned int)tx_skb);
        return ERROR;
    }  
    up(&(uart_ctx->tx_sema));  
    return OK;
}

/*****************************************************************************
* 函 数 名  : hsuart_get_read_buffer
* 功能描述  : 上层获取接收buf 的信息
* 输入参数  :  uart_ctx 上下文, 
* 输出参数  :  pWRInfo 读内存信息
* 返 回 值  : 成功/失败
*****************************************************************************/
s32 hsuart_get_read_buffer(hsuart_ctx* uart_ctx, hsuart_wr_async_info* pWRInfo)
{
    uart_mem_node* pMemNode = NULL;
    
    if (NULL == pWRInfo)
    {
        hsuart_error("pWRInfo:0x%x.\n", (unsigned int)pWRInfo);
        return ERROR;
    }
    
    /* 获取完成的 buffer节点 */
    pMemNode = get_mem_from_donelist(&(uart_ctx->read_info));
    if (pMemNode)
    {
        pWRInfo->pBuffer = (char *)pMemNode->pSkb;
        pWRInfo->u32Size = (u32)pMemNode->pSkb->len;
        pWRInfo->pDrvPriv = (void*)pMemNode->pSkb;      
        add_mem_to_freelist(&(uart_ctx->read_info), pMemNode);
        //(void)hsuart_alloc_skb(pMemNode, uart_ctx->read_info.node_size);
    }
    else
    {
        hsuart_error("no done mem node in list\n");
        return ERROR;
    }
    return OK;
}

/*****************************************************************************
* 函 数 名  : hsuart_return_read_buffer
* 功能描述  : 上层释放接收Buffer 的接口
* 输入参数  : uart_ctx 上下文, pWRInfo 内存信息
* 输出参数  :
* 返 回 值  : 成功/失败
*****************************************************************************/
s32 hsuart_return_read_buffer(hsuart_ctx* uart_ctx, hsuart_wr_async_info* pWRInfo)
{
    if(NULL == pWRInfo)
	{
       hsuart_error("pWRInfo is NULL\n");
	   return ERROR;
	}
    dev_kfree_skb((struct sk_buff *)pWRInfo->pBuffer);
	pWRInfo->pBuffer = NULL;
    return OK;
}
/*****************************************************************************
* 函 数 名  : hsuart_modem_write_signal
* 功能描述  :写modem 管脚信号
* 输入参数  :uart_dev 设备结构体pModemMsc 管脚信息
* 输出参数  :
* 返 回 值  : 成功/失败
*****************************************************************************/
s32 hsuart_modem_write_signal(hsuart_ctx *uart_ctx, MODEM_MSC_STRU* pModemMsc)
{    
	if(NULL == uart_ctx)
	{
       hsuart_error("uart_ctx is NULL\n");
	   return ERROR;
	}    
    if (uart_ctx->feature.rts)					/*处理DCE rts 管脚*/
    {
        if (SIGNALCH == pModemMsc->OP_Cts)
        {
            if (LOWLEVEL == pModemMsc->ucCts)	/*拉低rts 管脚*/
            {
                uart_ctx->rts = HSUART_TRUE;
                hsuart_set_rts(&uart_ctx->dev);
                hsuart_debug("rts valued:%d\n", pModemMsc->ucCts);//DEBUG
            }
            else								/*拉高rts 管脚*/
            {               
                uart_ctx->rts = HSUART_FALSE;
                hsuart_clear_rts(&uart_ctx->dev);
                hsuart_debug("rts invalued:%d\n", pModemMsc->ucCts);//DEBUG
            }
        }
    }
#ifdef HSUART_4_LINE
#else
    if (SIGNALCH == pModemMsc->OP_Ri)		/*Ring SIGNAL CHANGE*/
    {
        gpio_direction_output(HSUART_RING, !pModemMsc->ucRi); 
        hsuart_error("Ring change:%d\n", !pModemMsc->ucRi);//DEBUG
    }   
    if (SIGNALCH == pModemMsc->OP_Dsr)		/*DSR SIGNAL CHANGE*/
    {
        gpio_direction_output(HSUART_DSR, !pModemMsc->ucDsr); 
        hsuart_error("dsr change:%d\n", !pModemMsc->ucDsr);//DEBUG
    }  
    if (SIGNALCH == pModemMsc->OP_Dcd)		/* DCD SIGNAL CHANGE*/
    {
        gpio_direction_output(HSUART_DCD, !pModemMsc->ucDcd); 
        hsuart_error("dcd change:%d\n", !pModemMsc->ucDcd);//DEBUG
    }
#endif
    return OK;
}

/*****************************************************************************
* 函 数 名  : hsuart_realloc_read_buf
* 功能描述  : 重新分配读缓存
* 输入参数  : uart_ctx 上下文pReadBuffInfo 内存信息
* 输出参数  :
* 返 回 值  :失败/成功
*****************************************************************************/
s32 hsuart_realloc_read_buf(hsuart_ctx* uart_ctx, hsuart_read_buff_info* pReadBuffInfo)
{
    unsigned long flags = 0;

    if (NULL == uart_ctx || NULL == pReadBuffInfo)
    {
        hsuart_error("uart_ctx:0x%x   pReadBuffInfo:0x%x.\n", (unsigned int)uart_ctx, (unsigned int)pReadBuffInfo);
        return ERROR;
    }  
    if ((uart_ctx->read_info.node_num != pReadBuffInfo->u32BuffNum) || (uart_ctx->read_info.node_size != pReadBuffInfo->u32BuffSize))
    {
        hsuart_uninit_list(&(uart_ctx->read_info));
		spin_lock_irqsave(&(uart_ctx->dev.lock_irq), flags);
        uart_ctx->read_info.node_num = pReadBuffInfo->u32BuffNum;
        uart_ctx->read_info.node_size = pReadBuffInfo->u32BuffSize;
		spin_unlock_irqrestore(&(uart_ctx->dev.lock_irq), flags);
        if (hsuart_init_list(&(uart_ctx->read_info)))
        {
            hsuart_error("hsuart_init_rw_list error.\n");
            return ERROR;
        }       
    }
    return OK;
}

/*****************************************************************************
* 函 数 名  : hsuart_mode_switch_config
* 功能描述  : 模式切换设置
* 输入参数  : uart_ctx 上下文s32Arg 配置参数
* 输出参数  :
* 返 回 值  :NA
*****************************************************************************/
void hsuart_mode_switch_config(hsuart_ctx* uart_ctx, void* s32Arg)
{
    //unsigned long flags = 0;
    
    if (NULL == s32Arg)
    {
		bsp_softtimer_delete(&g_hsuart_ctrl.switch_state3_timer);
		
        uart_ctx->stage = NO_DETECT_STAGE;
		uart_ctx->cbs.switch_mode_cb= NULL;
        hsuart_debug("detect mode switch disable\n");
    }
    else
    {
        uart_ctx->stage = DETECT_STAGE_1;
		uart_ctx->cbs.switch_mode_cb = (hsuart_switch_mode_cb_t)s32Arg;
		hsuart_debug("detect mode switch enable\n");
    }
}

/*****************************************************************************
* 函 数 名  : hsuart_set_flow_ctrl
* 功能描述  : uart流控控制
* 输入参数  : uart_ctx 上下文s32Arg 配置参数
* 输出参数  :
* 返 回 值  :NA
*****************************************************************************/
void hsuart_set_flow_ctrl(hsuart_ctx* uart_ctx, uart_flow_ctrl_union* pParam)
{
	u32 val = 0;
    u32 mask = UART_AFCE_ENABLE | UART_RTS_ENABLE;
	u32 regval = 0;
    unsigned long flags = 0;
	if(pParam->reg.rtsen == 1)
	{
		regval |= (1<<14);
	}
	if(pParam->reg.ctsen == 1)
	{
		regval |= (1<<15);
	}
   	spin_lock_irqsave(&(uart_ctx->dev.lock_irq), flags);
   	val = readl(uart_ctx->dev.vir_addr_base + UART_REG_OFFSET_MCR);
    val &= regval | (~mask);
    val |= regval & mask;
	(void)writel(val, uart_ctx->dev.vir_addr_base + UART_REG_OFFSET_MCR);

    uart_ctx->feature.rts = pParam->reg.rtsen;
    uart_ctx->feature.afce = pParam->reg.ctsen;
    spin_unlock_irqrestore(&(uart_ctx->dev.lock_irq), flags);
}

/*****************************************************************************
* 函 数 名  : hsuart_udi_open
* 功能描述  : 打开uart udi设备
* 输入参数  : param 参数, handle 句柄
* 输出参数  :
* 返 回 值  : 打开结果
*****************************************************************************/
s32 hsuart_udi_open(UDI_OPEN_PARAM *param, UDI_HANDLE handle)
{
    hsuart_ctx *uart_ctx = &m2m_hsuart_ctx;
    uart_ctx->open = HSUART_TRUE;
    (void)BSP_UDI_SetPrivate(param->devid, (VOID*)uart_ctx);
    return OK;
}
/*****************************************************************************
* 函 数 名  : hsuart_udi_close
* 功能描述  : 关闭uart udi设备
* 输入参数  : handle
* 输出参数  :
* 返 回 值  :失败/成功
*****************************************************************************/
s32 hsuart_udi_close(BSP_S32 s32UartDevCtx)
{
    hsuart_ctx *pUartDevCtx = (hsuart_ctx *)s32UartDevCtx;   
    pUartDevCtx->open = HSUART_FALSE;
    return OK;
}

/*****************************************************************************
* 函 数 名  : hsuart_udi_write
* 功能描述  : uart校准同步发送接口
* 输入参数  : BSP_S32 s32UartDev, BSP_U8* pBuf, u32 u32Size
* 输出参数  : 实际写入的字数
* 返 回 值  :失败/实际的数字
*****************************************************************************/
s32 hsuart_udi_write(VOID* pPrivate, void* pMemObj, u32 u32Size)
{
    s32 num = 0;
	hsuart_ctx *uart_ctx = (hsuart_ctx *)pPrivate;

    if (NULL == uart_ctx || NULL == pMemObj || 0 == u32Size)
    {
        hsuart_error("Invalid params.\n");
        return ERROR;
    }
    if (!uart_ctx->open)
    {
        hsuart_error("uart is not open.\n");
        return ERROR;
    }    
    uart_ctx->stat.syn_write_cnt++;
    num = get_list_node_num(&uart_ctx->write_info.done_list);
    hsuart_debug("start addr:0x%x  len:%d  done list num:%d.\n", (unsigned int)pMemObj, u32Size, num);
    while (num)
    {
        msleep(1);
        num = get_list_node_num(&uart_ctx->write_info.done_list);
    }
    dialup_hsuart_send((u8 *)pMemObj, u32Size);  
    return OK;
}

/*****************************************************************************
* 函 数 名  : hsuart_udi_ioctl
* 功能描述  : UART 拨号业务相关 ioctl 设置
* 输入参数  : s32UartDevId: 设备管理结构指针
*             s32Cmd: 命令码
*             pParam: 命令参数
* 输出参数  :
* 返 回 值  : 成功/失败错误码
*****************************************************************************/
s32 hsuart_udi_ioctl(VOID* pPrivate, u32 u32Cmd, VOID* pParam)
{
    s32 ret = OK;
    unsigned long flags = 0;
    hsuart_ctx* uart_ctx = (hsuart_ctx*)pPrivate;
    hsuart_dev *uart_dev = NULL;
	DRV_DIALUP_HSUART_STRU dialup_acshell_nv;

	if(NULL == uart_ctx)
	{
       hsuart_error("uart_ctx is NULL\n");
	   return ERROR;
	}
    hsuart_debug("cmd:0x%x  pParam:0x%x\n", u32Cmd, (unsigned int)pParam);
    
    if (!uart_ctx->open)
    {
        hsuart_error("uart is not open.\n");
        return ERROR;
    }  
    uart_dev = &uart_ctx->dev;
    switch(u32Cmd)
    {      
        case UART_IOCTL_WRITE_ASYNC:		/* 下行异步写接口*/
        	ret = hsuart_write_async(uart_ctx, (hsuart_wr_async_info *)pParam);
        	break;
        case UART_IOCTL_SET_FREE_CB:		/* NAS注册下行数据buf 释放函数*/
        	uart_ctx->cbs.free_cb = (hsuart_free_cb_t)pParam;
        	break;
        case UART_IOCTL_SET_READ_CB:		/* 收到数据后调用此注册接口通知上层接收*/
        	uart_ctx->cbs.read_cb = (hsuart_read_cb_t)pParam;
        	break;        
        case UART_IOCTL_GET_RD_BUFF:		/* 上层调用此接口获得上行buf 地址，上层在我们的read回调函数中来实现的*/
        	ret = hsuart_get_read_buffer(uart_ctx, (hsuart_wr_async_info*)pParam);
        	break;
        case UART_IOCTL_RETURN_BUFF:		/* 上层调用此接口释放上行buf */
           	ret = hsuart_return_read_buffer(uart_ctx, (hsuart_wr_async_info*)pParam);
        	break;       
        case UART_IOCTL_RELLOC_READ_BUFF:	/* 重新分配单个buffer大小命令*/
        	ret = hsuart_realloc_read_buf(uart_ctx, (hsuart_read_buff_info*)pParam);
        	break;     
        case UART_IOCTL_SET_MSC_READ_CB:	/*注册DTR管脚变化通知回调参数为pMODEM_MSC_STRU 指向本地全局*/
        	uart_ctx->cbs.msc_read_cb = (hsuart_msc_read_cb_t)pParam;
	        break;       
        case UART_IOCTL_MSC_WRITE_CMD:		 /*写modem管脚信号，参数为pMODEM_MSC_STRU*/
        	ret = hsuart_modem_write_signal(uart_ctx, (MODEM_MSC_STRU*)(pParam));
        	break;        
        case UART_IOCTL_SWITCH_MODE_CB:		/* +++切换命令模式回调，设置NULL关闭该功能 */
        	hsuart_mode_switch_config(uart_ctx, pParam);
        	break;            
        case UART_IOCTL_SET_BAUD:			/* 设置波特率 */
            spin_lock_irqsave(&(uart_ctx->dev.lock_irq), flags);
            uart_ctx->cbs.set_baud_cb = hsuart_set_baud;
            uart_ctx->feature.baud = *(int*)pParam;
            spin_unlock_irqrestore(&(uart_ctx->dev.lock_irq), flags);
            hsuart_debug("set baud cb: %d\n", *(int*)pParam);
        	break;
        case UART_IOCTL_SET_WLEN:			/* 设置数据长度 */
            spin_lock_irqsave(&(uart_ctx->dev.lock_irq), flags);
            uart_ctx->cbs.set_wlen_cb = hsuart_set_wlen;
            uart_ctx->feature.wlen = *(int*)pParam;
            spin_unlock_irqrestore(&(uart_ctx->dev.lock_irq), flags);
            hsuart_debug("set wlen cb: %d\n", *(int*)pParam);
        	break;
        case UART_IOCTL_SET_STP2:			/* 设置停止位 */
            spin_lock_irqsave(&(uart_ctx->dev.lock_irq), flags);           
            uart_ctx->cbs.set_stp2_cb = hsuart_set_stpbit;
            uart_ctx->feature.stp2 = *(int*)pParam;            
            spin_unlock_irqrestore(&(uart_ctx->dev.lock_irq), flags);
            hsuart_debug("set stp2 cb: %d\n", *(int*)pParam);
        	break;
        case UART_IOCTL_SET_EPS:		 	/* 设置校验类型 */
            spin_lock_irqsave(&(uart_ctx->dev.lock_irq), flags);
            uart_ctx->cbs.set_eps_cb = hsuart_set_eps;
           	uart_ctx->feature.eps = *(int*)pParam;
            spin_unlock_irqrestore(&(uart_ctx->dev.lock_irq), flags);
            hsuart_debug("set eps cb: %d\n", *(int*)pParam);
        	break;
        case UART_IOCTL_SET_FLOW_CONTROL:	/* 设置流控 */
            hsuart_set_flow_ctrl(uart_ctx, (uart_flow_ctrl_union*)pParam);
        	break;
        case UART_IOCTL_SET_AC_SHELL:		 /*set A/C shell 终端需求 debug*/
			dialup_acshell_nv.DialupACShellCFG = *(unsigned int*)pParam;
			dialup_acshell_nv.DialupEnableCFG  = HSUART_ENABLE;
			ret = bsp_nvm_write(NV_ID_DRV_DIALUP_ACSHELL, (u8 *)&dialup_acshell_nv, sizeof(DRV_DIALUP_HSUART_STRU));
			if (ret != OK)
   			{
        		hsuart_error("write %d NV ERROR\n",NV_ID_DRV_DIALUP_ACSHELL);
        		return ERROR;
    		}
            break;
        default:
            hsuart_error("unknow Cmd: 0x%x\n", u32Cmd);
            ret = ERROR;
            break;
    }
    return ret;
}

/*****************************************************************************
* 函 数 名  : hsuart_timer_init
* 功能描述  : 
* 输入参数  : uart_ctx 上下文
* 输出参数  : NA
* 返 回 值  : 成功/失败bcmsdh_register_oob_intr
*****************************************************************************/
int hsuart_timer_init(hsuart_ctx *uart_ctx)
{
	struct softtimer_list *statethree_timer = &g_hsuart_ctrl.switch_state3_timer;
	struct softtimer_list *baud_adapt_timer = &g_hsuart_ctrl.adapt_timer;
	
	statethree_timer->func = switch_stage_three;
	statethree_timer->para = (u32)&m2m_hsuart_ctx;
	statethree_timer->timeout = SWITCH_TIMER_LENGTH;
	statethree_timer->wake_type = SOFTTIMER_NOWAKE;

	baud_adapt_timer->func = set_next_adapt;
	baud_adapt_timer->para = (u32)&m2m_hsuart_ctx;
	baud_adapt_timer->timeout = ADAPT_TIMER_LENGTH;
	baud_adapt_timer->wake_type = SOFTTIMER_NOWAKE;

	if(OK != bsp_softtimer_create(statethree_timer))
	{
		hsuart_error("statethree_timer create fail...\n");
	}

	if(OK != bsp_softtimer_create(baud_adapt_timer))
	{
		hsuart_error("baud_adapt_timer create fail...\n");
		goto fail_adapt_timer;
	}

	return 0;
fail_adapt_timer:
	bsp_softtimer_free(statethree_timer);
	return ERROR;	
}

#ifdef CONFIG_PM
static s32 dialup_hsuart_suspend(struct device *dev)
{
	hsuart_ctx *uart_ctx = &m2m_hsuart_ctx;
	hsuart_dev *uart_dev = &uart_ctx->dev;
	hsuart_debug("dialup hsuart suspend\n");
	
	if(g_hsuart_ctrl.ACSHELL_FLAG == HSUART_FALSE)
	{
		bsp_edma_channel_stop(g_dma_ctrl.channel_id);
		bsp_edma_channel_stop(EDMA_CH_HSUART_TX);
		disable_irq(gpio_to_irq(HSUART_DTR));
		disable_irq((unsigned int)uart_dev->irq);
		uart_ctx->feature.lcr = readl(uart_dev->vir_addr_base+ UART_REGOFF_LCR);
		uart_ctx->feature.mcr = readl(uart_dev->vir_addr_base+ UART_REG_OFFSET_MCR);
	}
	return OK;
}
extern s32 hsuart_acshell_baud(hsuart_dev *uart_dev);

static s32 dialup_hsuart_resume(struct device *dev)
{
	u32 regval = 0;
	struct clk *hsuart_clk = NULL;
	hsuart_ctx *uart_ctx = &m2m_hsuart_ctx;
	hsuart_dev *uart_dev = &uart_ctx->dev;

	hsuart_debug("dialup hsuart resume\n");

	hsuart_clk = clk_get(NULL, "hs_uart_clk");
	if(hsuart_clk == NULL)
	{
		hsuart_error("hsuart get clk fail\n");
		return ERROR;
	}
	clk_enable(hsuart_clk);
	if(g_hsuart_ctrl.ACSHELL_FLAG == HSUART_FALSE)
	{
		if (DMA_TRANSFER == uart_ctx->transfer_mode)
		{
			regval = UART_DEF_RT_ONEFOUR|UART_DEF_TET_ONETWO|UART_DEF_DMA_MODE|UART_TX_FIFO_RESET|UART_RX_FIFO_RESET|UART_FCR_FIFO_ENABLE;
			writel(regval , uart_dev->vir_addr_base+ UART_REGOFF_FCR);		
		}
		else
		{
			regval = UART_DEF_RT_ONEFOUR|UART_DEF_TET_ONETWO|UART_DEF_NO_DMA_MODE|UART_TX_FIFO_RESET|UART_RX_FIFO_RESET|UART_FCR_FIFO_ENABLE;
			writel(regval , uart_dev->vir_addr_base+ UART_REGOFF_FCR);		
		}
		
		hsuart_set_baud(uart_dev, uart_ctx->feature.baud);	
		writel(uart_ctx->feature.lcr , uart_dev->vir_addr_base+ UART_REGOFF_LCR);
		writel(UART_DEF_TX_DEPTH, uart_dev->vir_addr_base +  UART_REG_OFFSET_TX_FF_DEPTH);
		writel(uart_ctx->feature.mcr ,uart_dev->vir_addr_base+ UART_REG_OFFSET_MCR);
		writel(UART_IER_IRQ_DISABLE, uart_dev->vir_addr_base + UART_REGOFF_IER);
		readl(uart_dev->vir_addr_base + UART_REGOFF_LSR);
		readl(uart_dev->vir_addr_base + UART_REGOFF_USR);
		readl(uart_dev->vir_addr_base + UART_REGOFF_RBR);
		readl(uart_dev->vir_addr_base + UART_REGOFF_IIR);

		enable_irq(gpio_to_irq(HSUART_DTR));
		enable_irq((unsigned int)uart_dev->irq);

		{
			if(g_dma_ctrl.dma_run_flag == HSUART_TRUE)
			{
				if (bsp_edma_channel_async_start(g_dma_ctrl.channel_id, (u32)(uart_dev->phy_addr_base + UART_REGOFF_RBR), g_dma_ctrl.pMemNode_startaddr, uart_ctx->read_info.node_size))
	    		{
	        		hsuart_error("balong_dma_channel_async_start transfer failed! ret_id = %d\n", g_dma_ctrl.channel_id);
	        		return ERROR;
	    		}
				(void)writel(0x10,  uart_dev->vir_addr_base+ UART_REGOFF_IER);
			}
		}	
	}
	else
	{
		hsuart_acshell_resume();
	}
	return OK;
}
static const struct dev_pm_ops dialup_hsuart_pm_ops ={
	.suspend = dialup_hsuart_suspend,
	.resume = dialup_hsuart_resume,
};
#define BALONG_DEV_PM_OPS (&dialup_hsuart_pm_ops)
#else
#define BALONG_DEV_PM_OPS NULL
#endif

#define DRIVER_NAME "dialup_hsuart_device"
static struct platform_driver dialup_hsuart_driver = {
	.probe = NULL,
	.remove = NULL,
	.driver = {
		.name = DRIVER_NAME,
		.owner  = THIS_MODULE,
		.pm     = BALONG_DEV_PM_OPS,
	},
};

static struct platform_device dialup_hsuart_device =
{
    .name = DRIVER_NAME,
    .id       = -1,
    .num_resources = 0,
};
/*****************************************************************************
* 函 数 名  : hsuart_drv_init
* 功能描述  : uart 底层初始化函数
* 输入参数  : uart_ctx:uart上下文
* 输出参数  : NA
* 返 回 值  : 成功/失败
*****************************************************************************/
s32 hsuart_drv_init(hsuart_ctx *uart_ctx)
{
    s32 ret = ERROR;
	u32 regval = 0;
    hsuart_dev *uart_dev = NULL;

    if(NULL == uart_ctx)
	{
       hsuart_error("uart_ctx is NULL\n");
	   return ERROR;
	}
    uart_dev = &uart_ctx->dev;
    spin_lock_init(&uart_dev->lock_irq);
	
    uart_dev->vir_addr_base = ioremap(uart_dev->phy_addr_base,SIZE_4K);

	memset(&uart_ctx->cbs, 0, sizeof(hsuart_udi_cbs));
    memset(&uart_ctx->stat, 0, sizeof(hsuart_stat));
	
	uart_ctx->open = HSUART_TRUE;
    uart_ctx->baud_adapt = HSUART_FALSE;
    uart_ctx->baud_idx = DEFAULT_BAUD_IDX;
    uart_ctx->stage = DETECT_STAGE_1;
	uart_ctx->rts = HSUART_FALSE;
    uart_ctx->feature.baud = DEFAULT_BAUD;
	g_dma_ctrl.rx_dma_done = HSUART_FALSE;
    osl_sem_init(SEM_EMPTY,&(uart_ctx->tx_sema));
    osl_sem_init(SEM_EMPTY,&(uart_ctx->dma_sema));
    osl_sem_init(SEM_EMPTY,&g_dma_ctrl.edma_rx_sem);
    if (hsuart_buf_init(uart_ctx) != OK)
    {
        hsuart_error("hsuart_buf_init is error.\n");
        return ERROR;
    }

	if (DMA_TRANSFER == uart_ctx->transfer_mode)
	{
		regval = UART_DEF_RT_ONEFOUR|UART_DEF_TET_ONETWO|UART_DEF_DMA_MODE|UART_TX_FIFO_RESET|UART_RX_FIFO_RESET|UART_FCR_FIFO_ENABLE;
		writel(regval , uart_dev->vir_addr_base+ UART_REGOFF_FCR);		
	}
/*
	else
	{
		regval = UART_DEF_RT_ONEFOUR|UART_DEF_TET_ONETWO|UART_DEF_NO_DMA_MODE|UART_TX_FIFO_RESET|UART_RX_FIFO_RESET|UART_FCR_FIFO_ENABLE;
		writel(regval , uart_dev->vir_addr_base+ UART_REGOFF_FCR);
	}
*/
	/* 设置波特率 */
    hsuart_set_baud(uart_dev, uart_ctx->feature.baud);	
	
    writel(UART_LCR_DLS_8BITS | UART_LCR_STOP_1BITS |UART_LCR_PEN_NONE, uart_dev->vir_addr_base + UART_REGOFF_LCR);
	writel(UART_AFCE_ENABLE|UART_RTS_ENABLE ,uart_dev->vir_addr_base + UART_REG_OFFSET_MCR);
    writel(UART_DEF_TX_DEPTH, uart_dev->vir_addr_base +  UART_REG_OFFSET_TX_FF_DEPTH);
	writel(UART_IER_IRQ_DISABLE, uart_dev->vir_addr_base + UART_REGOFF_IER);	
	readl(uart_dev->vir_addr_base + UART_REGOFF_LSR);
	readl(uart_dev->vir_addr_base + UART_REGOFF_RBR);
	readl(uart_dev->vir_addr_base + UART_REGOFF_IIR);
	readl(uart_dev->vir_addr_base + UART_REGOFF_USR);
	
	ret = request_irq((unsigned int)uart_dev->irq, (irq_handler_t)hsuart_irq,0,"HS UART ISR", &uart_ctx->dev);
	if (ret != OK)
	{
	   	hsuart_error("request_irq is failed!\n");
	    return ERROR;
	}	
    hsuart_gpio_init(uart_ctx);
	hsuart_timer_init(uart_ctx);	
    return OK;
}

static UDI_DRV_INTEFACE_TABLE hsuart_udi = {
    .udi_open_cb = (UDI_OPEN_CB_T)hsuart_udi_open,
    .udi_close_cb = (UDI_CLOSE_CB_T)hsuart_udi_close,
    .udi_write_cb = (UDI_WRITE_CB_T)hsuart_udi_write,
    .udi_read_cb = (UDI_READ_CB_T)NULL,
    .udi_ioctl_cb = (UDI_IOCTL_CB_T)hsuart_udi_ioctl,
};
/*****************************************************************************
* 函 数 名  : hsuart_udi_init
* 功能描述  : UART UDI 初始化接口
* 输入参数  : NA
* 输出参数  : NA
* 返 回 值  : 成功/失败
*****************************************************************************/
int hsuart_udi_init(void)
{
	s32 ret = ERROR;
	struct clk *hsuart_clk = NULL;
	hsuart_ctx *uart_ctx = &m2m_hsuart_ctx;
    DRV_DIALUP_HSUART_STRU dialup_acshell_nv;
  

    uart_ctx->init = HSUART_FALSE;
	memset(&dialup_acshell_nv,0,sizeof(DRV_DIALUP_HSUART_STRU));
	ret = bsp_nvm_read(NV_ID_DRV_DIALUP_ACSHELL, (u8 *)&dialup_acshell_nv, sizeof(DRV_DIALUP_HSUART_STRU));
	if (ret != OK)
	{
	   	hsuart_error("read %d NV ERROR\n",NV_ID_DRV_DIALUP_ACSHELL);
	   	return ERROR;
	}
	if(HSUART_ENABLE == dialup_acshell_nv.DialupEnableCFG)
	{
		hsuart_error("hsuart init enable\n");
		
		hsuart_clk = clk_get(NULL, "hs_uart_clk");
		if(hsuart_clk == NULL)
		{
			hsuart_error("hsuart get clk fail\n");
			return ERROR;
		}
		clk_enable(hsuart_clk);
		
		ret = platform_device_register(&dialup_hsuart_device);
		if(ret)
		{
			hsuart_error("Platform hsuart device register is failed!\n");
			clk_disable(hsuart_clk);
	        return ret;
		}
		ret = platform_driver_register(&dialup_hsuart_driver);
		if (ret)
		{
			hsuart_error("Platform hsuart deriver register is failed!\n");
			platform_device_unregister(&dialup_hsuart_device);
			clk_disable(hsuart_clk);
			return ret;
		}
	  	/* A/C shell端口 */
		if((dialup_acshell_nv.DialupACShellCFG == UART_C_SHELL) || (dialup_acshell_nv.DialupACShellCFG == UART_A_SHELL))
		{
			g_hsuart_ctrl.ACSHELL_FLAG = HSUART_TRUE;
			ret = hsuart_shell_init(dialup_acshell_nv);
			if(ret != OK)
			{
				goto init_fail;
			}
			
			dialup_acshell_nv.DialupACShellCFG = 0;
			ret = bsp_nvm_write(NV_ID_DRV_DIALUP_ACSHELL, (u8 *)&dialup_acshell_nv, sizeof(DRV_DIALUP_HSUART_STRU));
			if (ret != OK)
	   		{
	        	hsuart_error("write %d NV ERROR\n",NV_ID_DRV_DIALUP_ACSHELL);
				goto init_fail;
	    	}
		}
		/* 数传端口 */
		else
		{	
		    if (OK != hsuart_drv_init(uart_ctx))
		    {
		        hsuart_error("hsuart_drv_init create failed!\n");
				goto init_fail;
		    }
			if (ERROR == osl_task_init("uart_recv_thread",HSUART_RX_TASK_PRI,HSUART_RX_TASK_SIZE,(void *)hsuart_rx_thread,(void *)uart_ctx,
										&g_hsuart_ctrl.rx_task_id))
			{
			  	hsuart_error("uart rx thread create failed!\n");
				goto init_fail;
			}
			if (ERROR == osl_task_init("uart_send_thread",HSUART_TX_TASK_PRI,HSUART_TX_TASK_SIZE,(void *)hsuart_tx_thread,(void *)uart_ctx,
										&g_hsuart_ctrl.tx_task_id))
			{
			   	hsuart_error("uart tx thread create failed!\n");
				goto init_fail;
			}

			/* UDI层的初始化 */
	    	(void)BSP_UDI_SetCapability(uart_ctx->udi_device_id, 0);
	    	(void)BSP_UDI_SetInterfaceTable(uart_ctx->udi_device_id, &hsuart_udi);
			uart_ctx->init = HSUART_TRUE;
		}
	}
	return OK;

init_fail:
	platform_device_unregister(&dialup_hsuart_device);
	platform_driver_unregister(&dialup_hsuart_driver);
	clk_disable(hsuart_clk);
	return ERROR;
}

module_init(hsuart_udi_init);

void hsuart_set_loglevel(u32 temp)
{
	bsp_mod_level_set(BSP_MODU_HSUART , temp);
}


