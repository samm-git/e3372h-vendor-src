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
#ifndef BALONG_SPI_HEADER_H
#define BALONG_SPI_HEADER_H

#include <linux/io.h>
#include <linux/irq.h>
#include <linux/workqueue.h>
#include <linux/interrupt.h>
#include <linux/scatterlist.h>
#include <hi_spi.h>

/* Bit fields in CTRLR0 */
#define SPI_DFS_OFFSET			0

#define SPI_FRF_OFFSET			4
#define SPI_FRF_SPI				0x0
#define SPI_FRF_SSP				0x1
#define SPI_FRF_MICROWIRE		0x2
#define SPI_FRF_RESV			0x3

#define SPI_MODE_OFFSET		6
#define SPI_SCPH_OFFSET			6
#define SPI_SCOL_OFFSET			7

#define SPI_TMOD_OFFSET		8
#define SPI_TMOD_MASK			(0x3 << SPI_TMOD_OFFSET)
#define	SPI_TMOD_TR			0x0		/* xmit & recv */
#define SPI_TMOD_TO			0x1		/* xmit only */
#define SPI_TMOD_RO			0x2		/* recv only */
#define SPI_TMOD_EPROMREAD	0x3		/* eeprom read mode */

#define SPI_SLVOE_OFFSET		10
#define SPI_SRL_OFFSET			11
#define SPI_CFS_OFFSET			12

/* Bit fields in SR, 7 bits */
#define SR_MASK				0x7f		/* cover 7 bits */
#define SR_BUSY				(1 << 0)
#define SR_TF_NOT_FULL		(1 << 1)
#define SR_TF_EMPT			(1 << 2)
#define SR_RF_NOT_EMPT		(1 << 3)
#define SR_RF_FULL			(1 << 4)
#define SR_TX_ERR			(1 << 5)
#define SR_DCOL				(1 << 6)

/* Bit fields in ISR, IMR, RISR, 7 bits */
#define SPI_INT_TXEI			(1 << 0)
#define SPI_INT_TXOI		(1 << 1)
#define SPI_INT_RXUI		(1 << 2)
#define SPI_INT_RXOI		(1 << 3)
#define SPI_INT_RXFI			(1 << 4)
#define SPI_INT_MSTI		(1 << 5)

/* TX RX interrupt level threshold, max can be 256 */
#define SPI_INT_THRESHOLD		32

enum balong_ssi_type {
	SSI_MOTO_SPI = 0,
	SSI_TI_SSP,
	SSI_NS_MICROWIRE,
};

struct balong_spi;
struct balong_spi_dma_ops {
	int (*dma_init)(struct balong_spi *balongs);
	void (*dma_exit)(struct balong_spi *balongs);
	int (*dma_transfer)(struct balong_spi *balongs);
};

struct balong_spi {
	struct spi_master	*master;
	struct spi_device	*cur_dev;
	struct device		*parent_dev;
	enum balong_ssi_type	type;
	char			name[16];

	void __iomem		*regs;
	unsigned long		paddr;
	u32			iolen;
	int			irq;
	u32			fifo_len;	/* depth of the FIFO buffer */
	u32			max_freq;	/* max bus freq supported */

	u16			bus_num;
	u16			num_cs;		/* supported slave numbers */

	/* Driver message queue */
	struct workqueue_struct	*workqueue;
	struct work_struct	pump_messages;
	spinlock_t		lock;
	struct list_head	queue;
	int			busy;
	int			run;

	/* Message Transfer pump */
	struct tasklet_struct	pump_transfers;

	/* Current message transfer state info */
	struct spi_message	*cur_msg;
	struct spi_transfer	*cur_transfer;
	struct chip_data	*cur_chip;
	struct chip_data	*prev_chip;
	size_t			len;
	void			*tx;
	void			*tx_end;
	void			*rx;
	void			*rx_end;
	int			dma_mapped;
	dma_addr_t		rx_dma;
	dma_addr_t		tx_dma;
	size_t			rx_map_len;
	size_t			tx_map_len;
	u8			n_bytes;	/* current is a 1/2 bytes op */
	u8			max_bits_per_word;	/* maxim is 16b */
	u32			dma_width;
	int			cs_change;
	/*add begin*/
	int			(*write)(struct balong_spi *balongs);
	int			(*read)(struct balong_spi *balongs);
	/*add end*/
	irqreturn_t		(*transfer_handler)(struct balong_spi *balongs);
	void			(*cs_control)(u32 command);

	/* Dma info */
	int			dma_inited;
	struct dma_chan		*txchan;
	struct scatterlist	tx_sgl;
	struct dma_chan		*rxchan;
	struct scatterlist	rx_sgl;
	int			dma_chan_done;
	struct device		*dma_dev;
	dma_addr_t		dma_addr; /* phy address of the Data register */
	struct balong_spi_dma_ops	*dma_ops;
	void			*dma_priv; /* platform relate info */
	struct pci_dev		*dmac;

	/* Bus interface info */
	void			*priv;
#ifdef CONFIG_DEBUG_FS
	struct dentry *debugfs;
#endif
int dma_channel;
};

static inline u32 balong_readl(struct balong_spi *balongs, u32 offset)
{
	return __raw_readl(balongs->regs + offset);
}

static inline void balong_writel(struct balong_spi *balongs, u32 offset, u32 val)
{
	__raw_writel(val, balongs->regs + offset);
}

static inline u16 balong_readw(struct balong_spi *balongs, u32 offset)
{
	//return __raw_readw(balongs->regs + offset);
	return (__raw_readl(balongs->regs + offset)&0xffff);/*modified by x00195528,不能用非对齐方式访问*/
}

static inline void balong_writew(struct balong_spi *balongs, u32 offset, u16 val)
{
	//__raw_writew(val, balongs->regs + offset);
	__raw_writel(((val)&0xffff),balongs->regs + offset);/*modified by x00195528,不能用非对齐方式访问*/
}

static inline void spi_enable_chip(struct balong_spi *balongs, int enable)
{
	balong_writel(balongs, HI_SPI_SSIENR_OFFSET, (enable ? 1 : 0));
}

static inline void spi_set_clk(struct balong_spi *balongs, u16 div)
{
	balong_writel(balongs, HI_SPI_BAUDR_OFFSET, div);
}

static inline void spi_chip_sel(struct balong_spi *balongs, u16 cs)
{
	if (cs > balongs->num_cs)
		return;

	if (balongs->cs_control)
		balongs->cs_control(1);

	balong_writel(balongs, HI_SPI_SER_OFFSET, 1 << cs);
}

/* Disable IRQ bits */
static inline void spi_mask_intr(struct balong_spi *balongs, u32 mask)
{
	u32 new_mask;

	new_mask = balong_readl(balongs, HI_SPI_IMR_OFFSET) & ~mask;
	balong_writel(balongs, HI_SPI_IMR_OFFSET, new_mask);
}

/* Enable IRQ bits */
static inline void spi_umask_intr(struct balong_spi *balongs, u32 mask)
{
	u32 new_mask;

	new_mask = balong_readl(balongs, HI_SPI_IMR_OFFSET) | mask;
	balong_writel(balongs, HI_SPI_IMR_OFFSET, new_mask);
}

/*
 * Each SPI slave device to work with balong_api controller should
 * has such a structure claiming its working mode (PIO/DMA etc),
 * which can be save in the "controller_data" member of the
 * struct spi_device
 */
struct balong_spi_chip {
	u8 poll_mode;	/* 0 for contoller polling mode */
	u8 type;	/* SPI/SSP/Micrwire */
	u8 enable_dma;
	void (*cs_control)(u32 command);
};

extern int balong_spi_add_host(struct balong_spi *balongs);
extern void balong_spi_remove_host(struct balong_spi *balongs);
extern int balong_spi_suspend_host(struct balong_spi *balongs);
extern int balong_spi_resume_host(struct balong_spi *balongs);
extern void balong_spi_xfer_done(struct balong_spi *balongs);

/* platform related setup */
extern int balong_spi_mid_init(struct balong_spi *balongs); /* Intel MID platforms */
#endif /* balong_SPI_HEADER_H */
