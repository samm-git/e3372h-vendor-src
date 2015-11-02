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
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/gfp.h>
#include <linux/delay.h>
#include <mach/lm.h>

#include <asm/mach/irq.h>
#include "bsp_memmap.h"

#include <linux/gpio.h>

#include "bsp_pmu.h"

extern int hi6451_32k_clk_enable(int clk_id);

#define DWC_SYNOP_CORE_HSIC 2
static struct lm_device dwc_usb_hsic={
	.dev={
		.init_name = "synopsys_dwc_hsic",
	},

	.resource.start = HI_HSIC_REGBASE_ADDR,
	.resource.end = HI_HSIC_REGBASE_ADDR + HI_HSIC_REG_SIZE -1,
	.resource.flags = IORESOURCE_MEM,
	.irq = INT_LVL_HSIC,
	.id = DWC_SYNOP_CORE_HSIC,
};
void dwc_wifi_unreset(void)
{
    printk("dwc_wifi_unrest: ===enter===\n");

	if (FEATURE_BCM_VERSION == 43241) {

	} else if (FEATURE_BCM_VERSION == 43239) {
	   	gpio_request(GPIO_1_19,"hisi-wifi");
   		gpio_direction_output(GPIO_1_19,1);   /* BCM43239_RST_N */
	} else {
		printk("invalid bcm version!\n");
	}
}

void dwc_hsic_unreset(void)
{
    u32 value;

    printk("dwc_hsic_unrest: ====enter===\r\n");

    /* Adjust HSIC signals */
    value = readl(IO_ADDRESS(HI_SYSCTRL_BASE_ADDR)+0x488);
	value |= (0xFF << 7);
    writel(value, IO_ADDRESS(HI_SYSCTRL_BASE_ADDR)+0x488);

    /* Force Host Mode */
	value = readl(IO_ADDRESS(HI_SYSCTRL_BASE_ADDR) + 0x488);
    value &= ~((u32)1<< 31);
    writel(value, IO_ADDRESS(HI_SYSCTRL_BASE_ADDR) + 0x488);

    /* dppulldown,dmpulldown clear */
	value = readl(IO_ADDRESS(HI_SYSCTRL_BASE_ADDR)+0x488);
	value |= (u32)1 << 22;
    writel(value, IO_ADDRESS(HI_SYSCTRL_BASE_ADDR)+0x488);

	value = readl(IO_ADDRESS(HI_SYSCTRL_BASE_ADDR)+0x488);
	value |= (u32)1 << 21;
    writel(value, IO_ADDRESS(HI_SYSCTRL_BASE_ADDR)+0x488);

    /* Activate HSIC Controller */
    value = readl(IO_ADDRESS(HI_SYSCTRL_BASE_ADDR) + 0x3C);
    value &= ~((u32)1<< 26);
    writel(value, IO_ADDRESS(HI_SYSCTRL_BASE_ADDR) + 0x3C);

    /* Release HSIC PHY Por */
    value = readl(IO_ADDRESS(HI_SYSCTRL_BASE_ADDR) + 0x3C);
    value &= ~((u32)1<< 27);
    writel(value, IO_ADDRESS(HI_SYSCTRL_BASE_ADDR) + 0x3C);

    udelay(100);
}

static int __init dwc_hsic_init(void)
{
	dwc_wifi_unreset();
	dwc_hsic_unreset();

    return lm_device_register(&dwc_usb_hsic);
}
module_init(dwc_hsic_init);

static void __init dwc_hsic_exit(void)
{
    return lm_device_unregister(&dwc_usb_hsic);
}
module_exit(dwc_hsic_exit);


MODULE_AUTHOR("BALONG USBNET GROUP");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("BALONG USB HSIC Controller Driver");
