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
#ifndef _HI_MCI_H_
#define _HI_MCI_H_



/*for fgpa low hz*/
//#define CONFIG_MMC_V7R2_FPGA

/*for mmc0-sd else mmc0-sdio*/
/*#define CONFIG_MMC_SDIO_LOOP*/


#define MMC0_CCLK			    	UHS_SDR50_MAX_DTR /*sdio card 96MHz*/
/*wifi*/
#define MMC1_CCLK			    	UHS_SDR104_MAX_DTR
#ifdef CONFIG_MMC_V7R2
#ifdef CONFIG_MMC_V7R2_FPGA
/*sd*/
#define MMC0_CCLK			    	UHS_SDR25_MAX_DTR
/*wifi*/
#define MMC1_CCLK			    	UHS_SDR25_MAX_DTR
#else
/*sd*/
#define MMC0_CCLK			    	UHS_SDR50_MAX_DTR /*sdio card 96MHz*/
/*wifi*/
#define MMC1_CCLK			    	UHS_SDR104_MAX_DTR
#endif
#elif (defined (CONFIG_MMC_V711))
#undef MMC0_CCLK
#undef MMC1_CCLK
#define MMC0_CCLK			    	(150000000) /*sdio card 100MHz*/
/*wifi*/
#define MMC1_CCLK			    	(150000000)
#endif

#define MMC_CLOCK_SOURCE_480M	480000000
#define MMC_CLOCK_SOURCE_600M	600000000
#if (FEATURE_ON == MBB_COMMON)
#define MMC_CLOCK_SOURCE_15M    15000000
#endif
#define MMC_CLOCK_SOURCE_400K   400000
#define MMC_CLOCK_SOURCE_15M   	15000000
#define ENUM_SPEED_BUS_SPEED	0xf
#define MMC_RESOURCES_SIZE	SZ_4K

#define HI_MAX_REQ_SIZE     (128*1024)

/* maximum size of one mmc block */
#define MAX_BLK_SIZE        512

/* maximum number of bytes in one req */
#define MAX_REQ_SIZE        HI_MAX_REQ_SIZE

/* maximum number of blocks in one req */
#define MAX_BLK_COUNT       (HI_MAX_REQ_SIZE/512)

/* maximum number of segments, see blk_queue_max_phys_segments */
#define MAX_SEGS            (HI_MAX_REQ_SIZE/512)

/* maximum size of segments, see blk_queue_max_segment_size */
#define MAX_SEG_SIZE        HI_MAX_REQ_SIZE





#define DBG(host, fmt, ...)                   \
do { \
	dev_dbg(host->dev, "[%s] "fmt, __func__, ##__VA_ARGS__); \
} while(0)

#define ERROR(host, fmt, ...)                   \
do { \
	dev_err(host->dev, "[%s] "fmt, __func__, ##__VA_ARGS__); \
} while(0)
	
#define INFO(host, fmt, ...)                   \
do { \
	 dev_info(host->dev, "[%s] "fmt, __func__, ##__VA_ARGS__); \
} while(0)


#endif
