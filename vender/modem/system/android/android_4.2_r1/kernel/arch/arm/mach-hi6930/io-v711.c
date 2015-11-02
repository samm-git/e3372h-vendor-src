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
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/export.h>

#include <mach/hardware.h>
#include <asm/page.h>
#include <asm/mach/map.h>
#include <linux/dma-mapping.h>

#include <mach/balongv7r2_iomap.h>
#include "bsp_memmap.h"
#include "product_config.h"

static struct map_desc boardp531_io_desc[] __initdata = {
	{
		.virtual	= DDR_SHARED_MEM_VIRT_ADDR,
		.pfn		= __phys_to_pfn(DDR_SHARED_MEM_ADDR),
		.length		= DDR_SHARED_MEM_SIZE,
		.type		= MT_DEVICE,
	},
	{/* GU reserve */
		.virtual	= DDR_GU_ADDR_VIRT,
		.pfn		= __phys_to_pfn(DDR_GU_ADDR),
		.length		= DDR_GU_SIZE,
		.type		= MT_DEVICE,
	},
	{
		.virtual	= HI_APP_GIC_BASE_ADDR_VIRT,
		.pfn		= __phys_to_pfn(HI_APP_GIC_BASE_ADDR),
		.length		= HI_APP_GIC_SIZE,
		.type		= MT_DEVICE,
	},
	{
		.virtual	= HI_SRAM_MEM_ADDR_VIRT,
		.pfn		= __phys_to_pfn(HI_SRAM_MEM_ADDR),
		.length 	= HI_SRAM_MEM_SIZE,
		.type		= MT_DEVICE,
	},
	{
		.virtual	= DDR_MNTN_ADDR_VIRT,
		.pfn		= __phys_to_pfn(DDR_MNTN_ADDR),
		.length		= DDR_MNTN_SIZE,
		.type		= MT_DEVICE,
	},
	{
		.virtual	= HI_SYSCTRL_BASE_ADDR_VIRT,
		.pfn		= __phys_to_pfn(HI_SYSCTRL_BASE_ADDR),
		.length 	= HI_SYSCTRL_REG_SIZE,
		.type		= MT_DEVICE,
	},
	{
		.virtual	= HI_SYSREG_PD_BASE_ADDR_VIRT,
		.pfn		= __phys_to_pfn(HI_SYSREG_PD_BASE_ADDR),
		.length 	= HI_SYSREG_PD_REG_SIZE,
		.type		= MT_DEVICE,
	},
	{
		.virtual	= HI_UART0_REGBASE_ADDR_VIRT,
		.pfn		= __phys_to_pfn(HI_UART0_REGBASE_ADDR),
		.length		= HI_UART0_REG_SIZE,
		.type		= MT_DEVICE,
	},
	{
		.virtual	= HI_UART1_REGBASE_ADDR_VIRT,
		.pfn		= __phys_to_pfn(HI_UART1_REGBASE_ADDR),
		.length		= HI_UART1_REG_SIZE,
		.type		= MT_DEVICE,
	},
	{
		.virtual	= HI_UART2_REGBASE_ADDR_VIRT,
		.pfn		= __phys_to_pfn(HI_UART2_REGBASE_ADDR),
		.length		= HI_UART2_REG_SIZE,
		.type		= MT_DEVICE,
	},
	{
		.virtual	= HI_TIMER_16TO23_REGBASE_ADDR_VIRT,
		.pfn		= __phys_to_pfn(HI_TIMER_16TO23_REGBASE_ADDR),
		.length		= HI_TIMER_16TO23_REG_SIZE,
		.type		= MT_DEVICE,
	},
	{
		.virtual	= HI_TIMER_08TO15_REGBASE_ADDR_VIRT,
		.pfn		= __phys_to_pfn(HI_TIMER_08TO15_REGBASE_ADDR),
		.length		= HI_TIMER_08TO15_REG_SIZE,
		.type		= MT_DEVICE,
	},
	{
		.virtual	= HI_TIMER_00TO07_REGBASE_ADDR_VIRT,
		.pfn		= __phys_to_pfn(HI_TIMER_00TO07_REGBASE_ADDR),
		.length		= HI_TIMER_00TO07_REG_SIZE,
		.type		= MT_DEVICE,
	},
	{
		.virtual	= IO_ADDRESS(HI_BC_CTRL_REGBASE_ADDR),
		.pfn		= __phys_to_pfn(HI_BC_CTRL_REGBASE_ADDR),
		.length		= HI_BC_CTRL_REG_SIZE,
		.type		= MT_DEVICE,
	},
	{

		.virtual	= HI_WDT_BASE_ADDR_VIRT,
		.pfn		= __phys_to_pfn(HI_WDT_BASE_ADDR),
		.length		= HI_WDT_REG_SIZE,
		.type		= MT_DEVICE,
	},
	{
		.virtual    = HI_RTC_REGBASE_ADDR_VIRT,
		.pfn        = __phys_to_pfn(HI_RTC_REGBASE_ADDR),
		.length     = HI_RTC_REG_SIZE,
		.type       = MT_DEVICE,
	},
    {
    	.virtual	= HI_SPI_MST0_REGBASE_ADDR_VIRT,
		.pfn		= __phys_to_pfn(HI_SPI_MST0_REGBASE_ADDR),
		.length		= HI_SPI_MST0_REG_SIZE,
		.type		= MT_DEVICE,
	},
	{

		.virtual	= HI_SPI_MST1_REGBASE_ADDR_VIRT,
		.pfn		= __phys_to_pfn(HI_SPI_MST1_REGBASE_ADDR),
		.length		= HI_SPI_MST1_REG_SIZE,
		.type		= MT_DEVICE,
	},
#ifdef HI_APP_VIC_BASE_ADDR
	{
		.virtual	= HI_APP_VIC_BASE_ADDR_VIRT,
		.pfn		= __phys_to_pfn(HI_APP_VIC_BASE_ADDR),
		.length		= HI_APP_VIC_SIZE,
		.type		= MT_DEVICE,
	},
#endif
#ifdef CONFIG_IPF
	{
		.virtual	= HI_IPF_REGBASE_ADDR_VIRT,
		.pfn		= __phys_to_pfn(HI_IPF_REGBASE_ADDR),
		.length		= HI_IPF_REG_SIZE,
		.type		= MT_DEVICE,
	},
#endif
    {
        .virtual    = HI_SOCP_REGBASE_ADDR_VIRT,
        .pfn        = __phys_to_pfn(HI_SOCP_REGBASE_ADDR),
        .length     = HI_SOCP_REG_SIZE,
        .type       = MT_DEVICE,
    },
    {
        .virtual    = HI_APPA9_L2_REGBASE_ADDR_VIRT,
        .pfn        = __phys_to_pfn(HI_APPA9_L2_REGBASE_ADDR),
        .length     = HI_APPA9_L2_REG_SIZE,
        .type       = MT_DEVICE,
    },

#ifdef HI_FPGA_RESERVED_SPACE_FOR_MODE_SW_BASE_ADDR
    {
        .virtual    = HI_FPGA_RESERVED_SPACE_FOR_MODE_SW_BASE_ADDR_VIRT,
        .pfn        = __phys_to_pfn(HI_FPGA_RESERVED_SPACE_FOR_MODE_SW_BASE_ADDR),
        .length     = HI_FPGA_RESERVED_SPACE_FOR_MODE_SW_SIZE,
        .type       = MT_DEVICE,
    },
#endif
    {
        .virtual    = HI_IOS_REGBASE_ADDR_VIRT,
        .pfn        = __phys_to_pfn(HI_IOS_REGBASE_ADDR),
        .length     = HI_IOS_REG_SIZE,
        .type       = MT_DEVICE,
    },

#ifdef HI_PCIE_PHY_BASE_ADDR
    {
        .virtual    = HI_PCIE_PHY_BASE_ADDR_VIRT,
        .pfn        = __phys_to_pfn(HI_PCIE_PHY_BASE_ADDR),
        .length     = HI_PCIE_PHY_SIZE,
        .type       = MT_DEVICE,
    },
#endif
    {
	 	.virtual	= HI_IPCM_REGBASE_ADDR_VIRT,
	 	.pfn		= __phys_to_pfn(HI_IPCM_REGBASE_ADDR),
	 	.length		= HI_IPCM_REG_SIZE,
	 	.type		= MT_DEVICE,
    },

    {
	 	.virtual	= HI_GPIO0_REGBASE_ADDR_VIRT,
	 	.pfn		= __phys_to_pfn(HI_GPIO0_REGBASE_ADDR),
	 	.length		= HI_GPIO0_REG_SIZE,
	 	.type		= MT_DEVICE,
    },
    {
	 	.virtual	= HI_GPIO1_REGBASE_ADDR_VIRT,
	 	.pfn		= __phys_to_pfn(HI_GPIO1_REGBASE_ADDR),
	 	.length		= HI_GPIO1_REG_SIZE,
	 	.type		= MT_DEVICE,
    },
    {
	 	.virtual	= HI_GPIO2_REGBASE_ADDR_VIRT,
	 	.pfn		= __phys_to_pfn(HI_GPIO2_REGBASE_ADDR),
	 	.length		= HI_GPIO2_REG_SIZE,
	 	.type		= MT_DEVICE,
    },
    {
	 	.virtual	= HI_GPIO3_REGBASE_ADDR_VIRT,
	 	.pfn		= __phys_to_pfn(HI_GPIO3_REGBASE_ADDR),
	 	.length		= HI_GPIO3_REG_SIZE,
	 	.type		= MT_DEVICE,
    },
    {
	 	.virtual	= HI_EDMA_REGBASE_ADDR_VIRT,
	 	.pfn		= __phys_to_pfn(HI_EDMA_REGBASE_ADDR),
	 	.length		= HI_EDMA_REG_SIZE,
	 	.type		= MT_DEVICE,
    },
    #ifdef HI_EDMA_CH4_REGBASE_ADDR
    {
	 	.virtual	= HI_EDMA_CH4_REGBASE_ADDR_VIRT,
	 	.pfn		= __phys_to_pfn(HI_EDMA_CH4_REGBASE_ADDR),
	 	.length		= HI_EDMA_CH4_REG_SIZE,
	 	.type		= MT_DEVICE,
    },
    #endif
    {
	 .virtual	= HI_AMON_CPUFAST_REGBASE_ADDR_VIRT,
	 .pfn		= __phys_to_pfn(HI_AMON_CPUFAST_REGBASE_ADDR),
	 .length		= HI_AMON_CPUFAST_REG_SIZE,
	 .type		= MT_DEVICE,
    },
    {
	 .virtual	= HI_AMON_SOC_REGBASE_ADDR_VIRT,
	 .pfn		= __phys_to_pfn(HI_AMON_SOC_REGBASE_ADDR),
	 .length		= HI_AMON_SOC_REG_SIZE,
	 .type		= MT_DEVICE,
    },
    {
     .virtual   = HI_EMI_REGBASE_ADDR_VIRT,
	 .pfn		= __phys_to_pfn(HI_EMI_REGBASE_ADDR),
	 .length		= HI_EMI_REG_SIZE,
	 .type		= MT_DEVICE,
    },
    {
	 .virtual	= HI_EMIMEM_REGBASE_ADDR_VIRT,
	 .pfn		= __phys_to_pfn(HI_EMIMEM_REGBASE_ADDR),
	 .length		= HI_EMIMEM_REG_SIZE,
	 .type		= MT_DEVICE,
    },
    {
	 .virtual	= HI_LTESIO_REGBASE_ADDR_VIRT,
	 .pfn		= __phys_to_pfn(HI_LTESIO_REGBASE_ADDR),
	 .length		= HI_LTESIO_REG_SIZE,
	 .type		= MT_DEVICE,
    },
    /* STM */
    {
        .virtual    = HI_CORESIGHT_STM_BASE_ADDR_VIRT,
        .pfn        = __phys_to_pfn(HI_CORESIGHT_STM_BASE_ADDR),
        .length     = HI_CORESIGHT_STM_SIZE,
        .type       = MT_DEVICE,
    },
    /* ETF */
    {
        .virtual    = HI_CORESIGHT_ETF_BASE_ADDR_VIRT,
        .pfn        = __phys_to_pfn(HI_CORESIGHT_ETF_BASE_ADDR),
        .length     = HI_CORESIGHT_ETF_SIZE,
        .type       = MT_DEVICE,
    },
    /* CTI */
    {
        .virtual    = HI_CORESIGHT_CTI_BASE_ADDR_VIRT,
        .pfn        = __phys_to_pfn(HI_CORESIGHT_CTI_BASE_ADDR),
        .length     = HI_CORESIGHT_CTI_SIZE,
        .type       = MT_DEVICE,
    },
    /* FUNNEL */
    {
        .virtual    = HI_CORESIGHT_FUNNEL_BASE_ADDR_VIRT,
        .pfn        = __phys_to_pfn(HI_CORESIGHT_FUNNEL_BASE_ADDR),
        .length     = HI_CORESIGHT_FUNNEL_SIZE,
        .type       = MT_DEVICE,
    },
    /* ETR */
    {
        .virtual    = HI_CORESIGHT_ETR_BASE_ADDR_VIRT,
        .pfn        = __phys_to_pfn(HI_CORESIGHT_ETR_BASE_ADDR),
        .length     = HI_CORESIGHT_ETR_SIZE,
        .type       = MT_DEVICE,
    },
    /* PTM0 */
    {
        .virtual    = HI_CORESIGHT_PTM0_BASE_ADDR_VIRT,
        .pfn        = __phys_to_pfn(HI_CORESIGHT_PTM0_BASE_ADDR),
        .length     = HI_CORESIGHT_PTM0_SIZE,
        .type       = MT_DEVICE,
    },
	/* PTM1 */
    {
        .virtual    = HI_CORESIGHT_PTM1_BASE_ADDR_VIRT,
        .pfn        = __phys_to_pfn(HI_CORESIGHT_PTM1_BASE_ADDR),
        .length     = HI_CORESIGHT_PTM1_SIZE,
        .type       = MT_DEVICE,
    },
    /* CIPHER */
    {
        .virtual    = HI_CIPHER_BASE_ADDR_VIRT,
        .pfn        = __phys_to_pfn(HI_CIPHER_BASE_ADDR),
        .length     = HI_CIPHER_REG_SIZE,
        .type       = MT_DEVICE,
    },
	{
	 .virtual	= HI_HDLC_REGBASE_ADDR_VIRT,
	 .pfn		= __phys_to_pfn(HI_HDLC_REGBASE_ADDR),
	 .length		= HI_HDLC_REG_SIZE,
	 .type		= MT_DEVICE,
    },
    /* EFUSE */
	{
	 .virtual	= HI_EFUSE_BASE_ADDR_VIRT,
	 .pfn		= __phys_to_pfn(HI_EFUSE_REGBASE_ADDR),
	 .length		= HI_EFUSE_REG_SIZE,
	 .type		= MT_DEVICE,
    },    
};

void __init balong_map_io(void)
{
	iotable_init(boardp531_io_desc, ARRAY_SIZE(boardp531_io_desc));

#if defined(CONFIG_BALONG_PCIE_DOCKING_TEST)
    /*PCIE DOCKING TEST need 8MB dma coherent buffer*/
    init_consistent_dma_size(SZ_2M + SZ_8M);
#endif
}
