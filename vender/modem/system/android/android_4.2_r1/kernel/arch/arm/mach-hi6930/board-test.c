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
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
#include <linux/interrupt.h>
#include <linux/amba/bus.h>
#include <linux/io.h> 
#include <linux/gfp.h>
#include <linux/clkdev.h>

#include <asm/system.h>
#include <asm/irq.h>
#include <asm/mach-types.h>
#include <asm/pgtable.h>
#include <asm/hardware/gic.h>
#include <asm/mach/arch.h>
#include <asm/mach/irq.h>
#include <asm/mach/map.h>
#include <asm/mach/time.h>
#include <asm/hardware/gic.h>

#include <mach/hardware.h>
#include <mach/board.h>
#include <mach/irqs.h>
#include <mach/timer.h>
#include <mach/balongv7r2_iomap.h>

/* used by entry-macro.S */


/*
 * Balong devices
 */

static struct amba_device uart0_device = {
	.dev = 
	{
		.coherent_dma_mask = ~0,	
		.init_name = "dev:uart0",
		.platform_data = NULL,	
	},
	.res = 
	{
		.start	= BALONG_UART0_PHY_BASE,
		.end	= (BALONG_UART0_PHY_BASE) + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},	
	.dma_mask	= ~0,
	.irq		= { BALONG_UART0_IRQ, NO_IRQ },
    .periphid = 0x000c21c0,
};


static struct amba_device *amba_devs[] __initdata = {
	&uart0_device,
};

static void __init gic_init_irq(void)
{
	/* board GIC, primary */
	gic_init(0, GIC_PPI_START, (void __iomem *)GIC_DIST_VIRT_BASE, (void __iomem *)GIC_CPU_VIRT_BASE);
}

static void __init balongv7r2_init(void)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(amba_devs); i++) {
		struct amba_device *d = amba_devs[i];
		amba_device_register(d, &iomem_resource);
	}	
}

static void __init balongv7r2_reserve(void)
{
}

static void __init balongv7r2_init_early(void)
{
}

/* restart.c  */
void balongv7r2_restart(char mode, const char *cmd)
{
}

extern int map_io_finished;
static void __init balongv7r2_map_io(void)
{
	balong_map_io();
    map_io_finished = 1;//indicate to init;
}

MACHINE_START(HI6930, "Hisilicon hi6930")
	.atag_offset  = 0x100,
	.map_io		  = balongv7r2_map_io,
	.reserve      = balongv7r2_reserve,
	.init_irq	  = gic_init_irq,	
	.timer		  = &balongv7r2_timer,
	.init_early	  = balongv7r2_init_early,	
	.init_machine = balongv7r2_init,
	.handle_irq   = gic_handle_irq,
	.restart      = balongv7r2_restart,
MACHINE_END

