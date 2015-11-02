/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2013-2015. All rights reserved.
 *
 * mobile@huawei.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#ifndef __OLED_SPI_H__
#define __OLED_SPI_H__


#define ECS_SPI0_BASE HI_SPI_MST0_REGBASE_ADDR_VIRT /*SPI0控制器基址*/
#define ECS_SPI1_BASE HI_SPI_MST1_REGBASE_ADDR_VIRT /*SPI1控制器基址*/
extern const u32 spiBase[2];/*lint !e10*/
#define SPI_BASE(x)        (spiBase[x])   /* SPI基地址 */
#define SPI_CTRL0(x)       (SPI_BASE(x) + HI_SPI_CTRLR0_OFFSET)

/********************************************************************************/
/*    spi 寄存器偏移定义（项目名_模块名_寄存器名_OFFSET)        */
/********************************************************************************/
#define    HI_SPI_CTRLR0_OFFSET             (0x0) /* 控制寄存器0。 */
#define    HI_SPI_CTRLR1_OFFSET             (0x4) /* 控制寄存器1。 */
#define    HI_SPI_SSIENR_OFFSET             (0x8) /* SSI使能寄存器。 */
#define    HI_SPI_MWCR_OFFSET               (0xC) /* Microwire控制寄存器。 */
#define    HI_SPI_SER_OFFSET                (0x10) /* slave使能寄存器。 */
#define    HI_SPI_BAUDR_OFFSET              (0x14) /* 波特率选择。 */
#define    HI_SPI_TXFTLR_OFFSET             (0x18) /* 发送FIFO阈值。 */
#define    HI_SPI_RXFTLR_OFFSET             (0x1C) /* 接收FIFO阈值。 */
#define    HI_SPI_TXFLR_OFFSET              (0x20) /* 发送FIFO水线寄存器。 */
#define    HI_SPI_RXFLR_OFFSET              (0x24) /* 接收FIFO水线寄存器。 */
#define    HI_SPI_SR_OFFSET                 (0x28) /* 状态寄存器。 */
#define    HI_SPI_IMR_OFFSET                (0x2C) /* 中断屏蔽寄存器。 */
#define    HI_SPI_ISR_OFFSET                (0x30) /* 中断状态寄存器。 */
#define    HI_SPI_RISR_OFFSET               (0x34) /* 原始中断寄存器。 */
#define    HI_SPI_TXOICR_OFFSET             (0x38) /* 发送FIFO溢出中断清除寄存器。 */
#define    HI_SPI_RXOICR_OFFSET             (0x3C) /* 接收FIFO溢出中断清除寄存器。 */
#define    HI_SPI_RXUICR_OFFSET             (0x40) /* 接收FIFO下溢中断清除寄存器。 */
#define    HI_SPI_MSTICR_OFFSET             (0x44) /* 多master冲突中断清除寄存器。 */
#define    HI_SPI_ICR_OFFSET                (0x48) /* 中断清除寄存器。 */
#define    HI_SPI_DMACR_OFFSET              (0x4C) /* DMA控制寄存器。 */
#define    HI_SPI_DMATDLR_OFFSET            (0x50) /* DMA发送数据水线。 */
#define    HI_SPI_DMARDLR_OFFSET            (0x54) /* DMA接收数据水线。 */
#define    HI_SPI_IDR_OFFSET                (0x58) /* ID寄存器。 */
#define    HI_SPI_SSI_COMP_VERSION_OFFSET   (0x5C) /* 版本寄存器。 */
#define    HI_SPI_DR_OFFSET                 (0x60) /* 数据寄存器。 */
#define    HI_SPI_RX_SAMPLE_DLY_OFFSET      (0xF0) /* 采样时间延迟寄存器 */

/*配置Ctrl0寄存器时，各成员的偏移*/
#define SPI_CTRL0_DFS_BITPOS        0   /* 数据长度 */
#define SPI_CTRL0_FRF_BITPOS        4   /* 帧类型 */
#define SPI_CTRL0_SCPH_BITPOS       6   /* 时钟相位 */
#define SPI_CTRL0_SCPOL_BITPOS      7   /* 时钟极性 */

#define SPI_CTRL0_TMOD_BITPOS       8   /* 传输模式设置 */
#define SPI_CTRL0_TMOD_BITWIDTH     2
#define SPI_CTRL0_TMOD_BITMASK    (((1 << SPI_CTRL0_TMOD_BITWIDTH)-1) << SPI_CTRL0_TMOD_BITPOS)
#define SPI_CTRL0_TMOD_SEND_RECV    (0x00 << SPI_CTRL0_TMOD_BITPOS)   /* 收发模式 */
#define SPI_CTRL0_TMOD_SEND         (0x01 << SPI_CTRL0_TMOD_BITPOS)   /* 发送模式 */
#define SPI_CTRL0_TMOD_RECV         (0x02 << SPI_CTRL0_TMOD_BITPOS)   /* 接收模式 */
#define SPI_CTRL0_TMOD_EEPROM_READ  (0x03 << SPI_CTRL0_TMOD_BITPOS)   /* EEPROM读模式 */

#define SPI_CFS_BITPOS              12  /* 控制帧大小 */

#define SPI_CTRL1(x)             (SPI_BASE(x) + HI_SPI_CTRLR1_OFFSET)
#define SPI_EN(x)                (SPI_BASE(x) + HI_SPI_SSIENR_OFFSET)
#define SPI_MWCTRL(x)            (SPI_BASE(x) + HI_SPI_MWCR_OFFSET  )
#define SPI_SLAVE_EN(x)          (SPI_BASE(x) + HI_SPI_SER_OFFSET   )
#define SPI_BAUD(x)              (SPI_BASE(x) + HI_SPI_BAUDR_OFFSET )
#define SPI_TXFTL(x)             (SPI_BASE(x) + HI_SPI_TXFTLR_OFFSET)
#define SPI_RXFTL(x)             (SPI_BASE(x) + HI_SPI_RXFTLR_OFFSET)
#define SPI_TXFL(x)              (SPI_BASE(x) + HI_SPI_TXFLR_OFFSET )
#define SPI_RXFL(x)              (SPI_BASE(x) + HI_SPI_RXFLR_OFFSET )
#define SPI_STATUS(x)            (SPI_BASE(x) + HI_SPI_SR_OFFSET    )
#define SPI_IMR(x)               (SPI_BASE(x) + HI_SPI_IMR_OFFSET   )
#define SPI_INT_STATUS(x)        (SPI_BASE(x) + HI_SPI_ISR_OFFSET   )
#define SPI_RX_SAMPLE_DLY(x)     (SPI_BASE(x) + HI_SPI_RX_SAMPLE_DLY_OFFSET)

#define SPI_STATUS_RXFULL         (1 << 4)
#define SPI_STATUS_RXNOTEMPTY     (1 << 3)
#define SPI_STATUS_TXEMPTY        (1 << 2)
#define SPI_STATUS_TXNOTFULL      (1 << 1)
#define SPI_STATUS_BUSY           (1 << 0)

#define SPI_RAW_INT_STATUS(x)  (SPI_BASE(x) + HI_SPI_RISR_OFFSET            )
#define SPI_TXO_INT_CLEAR(x)   (SPI_BASE(x) + HI_SPI_TXOICR_OFFSET          )
#define SPI_RXO_INT_CLEAR(x)   (SPI_BASE(x) + HI_SPI_RXOICR_OFFSET          )
#define SPI_RXU_INT_CLEAR(x)   (SPI_BASE(x) + HI_SPI_RXUICR_OFFSET          )
#define SPI_MST_INT_CLEAR(x)   (SPI_BASE(x) + HI_SPI_MSTICR_OFFSET          )
#define SPI_INT_CLEAR(x)       (SPI_BASE(x) + HI_SPI_ICR_OFFSET             )
#define SPI_DMAC(x)            (SPI_BASE(x) + HI_SPI_DMACR_OFFSET           )
#define SPI_DMATDL(x)          (SPI_BASE(x) + HI_SPI_DMATDLR_OFFSET         )
#define SPI_DMARDL(x)          (SPI_BASE(x) + HI_SPI_DMARDLR_OFFSET         )
#define SPI_ID(x)              (SPI_BASE(x) + HI_SPI_IDR_OFFSET             )
#define SPI_COMP_VERSION(x)    (SPI_BASE(x) + HI_SPI_SSI_COMP_VERSION_OFFSET)
#define SPI_DR(x)              (SPI_BASE(x) + HI_SPI_DR_OFFSET              )

/* for porting test */
/* Porting E2PROM使用SPI0 CS0 ; SFLASH使用SPI0 CS1*/
#define SPI_NO_DEV          0        /*SPI0*/
#define SPI_CS_EEPROM       (1 << 0)   /* EEPROM使用SSI0 CS0 */
#define SPI_CS_SFLASH       (1 << 1)   /* SFLASH使用SSI0 CS1 */
#define SPI_CS_DEV          SPI_CS_SFLASH/*硬件实际连接的设备*/
/* 状态寄存器状态位 */
#define SPI_DEV_STATUS_WIP  (1 << 0)  /* Write In Progress Bit */
#define SPI_DEV_STATUS_WEL  (1 << 1)  /* Write Enable Latch Bit */
#define SPI_DEV_STATUS_BP0  (1 << 2)  /* Block Protect Bits */
#define SPI_DEV_STATUS_BP1  (1 << 3)  /* Block Protect Bits */
#define SPI_DEV_STATUS_BP2  (1 << 4)  /* Block Protect Bits */
#define SPI_DEV_STATUS_SRWD  (1 << 7)  /* Status Register Write Protect */

#define SPI_MAX_DELAY_TIMES 0x1000 /*最大延迟时间*/

/*错误码*/
#define OK              (0)
#define ERROR          (-1)
#define MDELAY_10_MS          (10)

#endif


