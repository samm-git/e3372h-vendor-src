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
 * * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS 
IS"
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
/*lint -save -e34 -e537 -e737*/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/io.h> 
#include <linux/gfp.h>
#include <linux/delay.h>
#include <linux/clk.h>
#if (FEATURE_ON == MBB_USB_CPE)
#include <linux/gpio.h>
#endif
#include <asm/mach/irq.h>
#include "bsp_memmap.h"
#include "bsp_usb.h"
#include "bsp_clk.h"
#if (FEATURE_ON == MBB_USB)
#include "../mbb_usb_unitary/usb_vendor.h"
#else
#include "../gadget/usb_vendor.h"
#endif
#include "hi_syssc_interface.h"
#include "hi_syscrg_interface.h"

#if (FEATURE_ON == MBB_USB)
#define IO_REGISTER_ADDRESS 0x90000498
#define TXPPREEMPAMPTUNE  (0x6<<14)
#endif
#if defined(BSP_CONFIG_BOARD_WINGLE)
#define MMC1_CLK_DRV_ADD 0x90010854
#define MMC1_CLK_DRV_CURRENT_32MA 0x000000D8
#endif

static struct resource dwc3_platform_resource[] =
{
    [0]={
        .start = HI_USB3_REGBASE_ADDR,
        .end = HI_USB3_REGBASE_ADDR + HI_USB3_REG_SIZE - 1,
        .flags = IORESOURCE_MEM
    },
    [1]={
        .start = INT_LVL_USB3,
        .end = INT_LVL_USB3,
        .flags = IORESOURCE_IRQ
    }
};

u64 dwc3_dma_mask = 0xffffffffUL;

static struct platform_device* dwc3_platform_dev_ptr;

#if 0
static struct platform_device dwc3_platform_dev = {
	.name = "dwc3",
    .id = -1,
    .num_resources = ARRAY_SIZE(dwc3_platform_resource),
    .resource = dwc3_platform_resource,
    .dev = {
        .coherent_dma_mask = 0xffffffffUL,
        .dma_mask = &dwc3_dma_mask,
        /* if no release setting kernel/drivers/base/core.c:196 will print WARNNING */
        .release = dwc3_release,
    }
};
#endif

/*
 * bc(battery charger) interfaces
 */
static void bc_set_soft_mode(void)
{
    unsigned reg;

    reg = readl(HI_BC_CTRL_REGBASE_ADDR_VIRT + 0x0);
    reg &= ~0x3;
    reg |= 0x1;/* [false alarm]:Disable fortify false alarm */
    writel(reg, HI_BC_CTRL_REGBASE_ADDR_VIRT + 0x0);
    return;
}/*lint !e550*/

static void bc_set_bypass_mode(void)
{
    unsigned reg;

    reg = readl(HI_BC_CTRL_REGBASE_ADDR_VIRT + 0x0);
    reg &= ~0x03;
    reg |=  0x02;/* [false alarm]:Disable fortify false alarm */
    writel(reg, HI_BC_CTRL_REGBASE_ADDR_VIRT + 0x0);

    return;
}/*lint !e550*/

static unsigned bc_set_test_volt(void)
{
    unsigned reg;
    unsigned record_reg;

    reg = readl(HI_BC_CTRL_REGBASE_ADDR_VIRT + 0x4);
    record_reg = reg;
    reg |= 0xC;
    reg &= ~0x10;/* [false alarm]:Disable fortify false alarm */
    writel(reg, HI_BC_CTRL_REGBASE_ADDR_VIRT + 0x4);

    return record_reg;
}

static void bc_set_record_volt(unsigned record_reg)
{
    writel(record_reg, HI_BC_CTRL_REGBASE_ADDR_VIRT + 0x4);
    return;
}

/*
 * 0:huawei charger , 1:non-huawei charger, -1:invalid charger
 */
static int bc_get_charger_type(void)
{
    unsigned reg;

    reg = readl(HI_BC_CTRL_REGBASE_ADDR_VIRT + 0xc);
    if (0 != (0x2000 & reg)) {
        return USB_CHARGER_TYPE_HUAWEI;
    }

    return USB_CHARGER_TYPE_NOT_HUAWEI;
}



/*
 * note: the function will sleep in long time !!!
 * 0:huawei charger , 1:non-huawei charger, -1:invalid charger
 */
int bc_charger_type_identify(void)
{
    unsigned record_reg;
    int type = USB_CHARGER_TYPE_INVALID;

    /* setup to soft mode */
    bc_set_soft_mode();

    /*
    VBUS ------
    D+     ----
    D-     ----
    RND  ------
    when usb insert into host, VBUS and RND connect first, then D+/D- signal on
    we need msleep to wait the signal ok.
    */
    msleep(2000);

    /*
    bit[2]=1
    bit[3]=1
    bit[4]=0
    supply D+ 0.6V volt(normal volt is 0.8V)for charger identify
    */
    record_reg = bc_set_test_volt();

    /* wait 50ms for D+ signal go to D- */
    mdelay(50);/*lint !e62*/

    /*
    get D- signal, and detect the charger type
    bit[13]==0 : SDP (PC)
    bit[13]==1 : DCP (CHARGER)
    */
    type = bc_get_charger_type();

    /* restore the default setting */
    bc_set_record_volt(record_reg);

    mdelay(20);/*lint !e62*/

    /* restore to bypass mode */
    bc_set_bypass_mode();

    mdelay(2);/*lint !e62*/

    return type;
}

/* Attention: make sure the clock name is consistent with the name defined in clock_balong_hi6930.c */
#define USB_OTG_CLK_NAME "usbotg_clk"
#define USB_BC_CLK_NAME "usb_bc_clk"
static struct clk *usb3_otg_clk, *usb3_bc_clk;
static void usb3_clk_get(void)
{
    usb3_otg_clk = clk_get(NULL, USB_OTG_CLK_NAME);
	if (IS_ERR(usb3_otg_clk)) {
		printk(KERN_ERR "dwc usb3.0: failed to get the usb otg clk %d\n",(int)PTR_ERR(usb3_otg_clk));
		usb3_otg_clk = NULL;
	}
	usb3_bc_clk = clk_get(NULL, USB_BC_CLK_NAME);
	if (IS_ERR(usb3_bc_clk)) {
		printk(KERN_ERR "dwc usb3.0: failed to get the usb otg clk %d\n",(int)PTR_ERR(usb3_bc_clk));
		usb3_bc_clk = NULL;
	}
}
static void usb3_clk_put(void)
{
    if(usb3_otg_clk)
       clk_put(usb3_otg_clk);

    if (usb3_bc_clk)
        clk_put(usb3_bc_clk);
}

static void usb3_otg_clk_enable(void)
{
    int ret;
    if (!usb3_otg_clk) {
        printk(KERN_ERR "dwc usb3.0: invalid otg clk\n");
        return;
    }
    ret = clk_enable(usb3_otg_clk);
    if(ret) {
        printk(KERN_ERR "dwc usb3.0: failed to enable the otg clk(err code:%d)\n",ret);
    }
}
static void usb3_otg_clk_disable(void)
{
    if (!usb3_otg_clk) {
        printk(KERN_ERR "dwc usb3.0: invalid otg clk\n");
        return;
    }
    clk_disable(usb3_otg_clk);
}
static void usb3_bc_clk_enable(void)
{
    int ret;
    if (!usb3_bc_clk) {
        printk(KERN_ERR "dwc usb3.0: invalid bc clk\n");
        return;
    }
    ret = clk_enable(usb3_bc_clk);
    if(ret) {
        printk(KERN_ERR "dwc usb3.0: failed to enable the bc clk(err code:%d)\n",ret);
    }
}
void usb3_bc_clk_disable(void)
{
    if (!usb3_bc_clk) {
        printk(KERN_ERR "dwc usb3.0: invalid bc clk\n");
        return;
    }
    clk_disable(usb3_bc_clk);
}

#if (FEATURE_ON == MBB_USB_CPE)

/*
* CPE采用默认HOST的模式，需要在USB上电时拉
* 低PHY ID(通过设置GPIO 1-9 完成),在DT工位
* 升级时，需要反向拉高进入Device模式
*/ 
void usb3_otg_cpe_init_mode(void)
{
    static int inited = 0;
    int ret = 0;
    int gpio_val = 1;

    if (inited)
    {
        return;
    }

    ret = gpio_request(GPIO_1_9, "GPIO_OTG_CPE_MODE");
    if (ret < 0)
    {
        printk(KERN_ERR "usb3_otg_cpe_init_mode: req gpio failed %d\n", ret);
        gpio_free(GPIO_1_9);
    }
    
#if (FEATURE_ON == MBB_DT_USBLOADER)
    gpio_direction_output(GPIO_1_9, 0);    /* Device mode */
#else
    if (get_nv_backup_flag())
    {
        gpio_val = 0;
        printk("usb3_otg_cpe_init_mode: dload mode\n");        
    }
    gpio_direction_output(GPIO_1_9, gpio_val);      /* Host mode or dload mode */
#endif   

    inited = 1;
}

int usb3_otg_cpe_set_device(int enable)
{ 
    if (enable)
    {
        gpio_set_value(GPIO_1_9, 0);  /* Device mode */
    }
    else
    {
        gpio_set_value(GPIO_1_9, 1);  /* Host mode */
    }
}

#endif /* MBB_USB_CPE mode set */

void usb3_sysctrl_init(void)
{
#if (FEATURE_ON == MBB_USB)
    u32 value = 0;
#endif

#if (FEATURE_ON == MBB_USB_CPE) 
    usb3_otg_cpe_init_mode();
#endif

    usb3_clk_get();
    /* enable the usb-bc clk at first */
    usb3_bc_clk_enable();
    /* power up the usb2.0 and usb3.0 phy */
    hi_syssc_usb_powerdown_hsp(0);
    hi_syssc_usb_powerdown_ssp(0);

    /* ref_ssp_en */
    hi_syssc_usb_ref_ssp(1);

#ifdef CONFIG_USB_DWC3_VBUS_DISCONNECT
    /* vbusvldext and vbusvldextsel */
    hi_syssc_usb_vbusvldext(1);

    /* override_en and override_value*/
    hi_syssc_usb_override(1);
    //hi_syssc_usb_override_value(1);
#else
    /* vbusvldext and vbusvldextsel */
    hi_syssc_usb_vbusvldext(0);

    /* override_en and override_value*/
    hi_syssc_usb_override(0);
#endif

#ifdef CONFIG_USB_OTG_USBID_BYGPIO
    hi_syssc_usb_iddig_en(1);
#endif

    hi_syssc_usb_txpreempamptune(3);

    hi_syssc_usb_phy3_init();

    /* set bc_mode to bypass */
    bc_set_bypass_mode();

    /* enable the otg clock */
    usb3_otg_clk_enable();

    /* release controller and PHY */
    hi_syscrg_usb_release();
#if (FEATURE_ON == MBB_USB)
    /*眼图修改,TXPPREEMPAMPTUNE由原来的00改为11，即寄存器0x90000498由原来的 0x34E466E4 改为 0x34E5E6E4*/
    value = readl(IO_ADDRESS(IO_REGISTER_ADDRESS));        
    value |= TXPPREEMPAMPTUNE;
    writel(value,IO_ADDRESS(IO_REGISTER_ADDRESS));
#endif
    mdelay(10);/*lint !e62*/
}

#if (FEATURE_ON == MBB_CHARGE)
extern int dp_dm_connected;
extern int chg_en_flag;

int bc_get_dp_dm_status(void)
{
    unsigned record_reg;
    int type = USB_CHARGER_TYPE_INVALID;

    /* setup to soft mode */
    bc_set_soft_mode();

    msleep(50);

    record_reg = bc_set_test_volt();

    /* wait 50ms for D+ signal go to D- */
    mdelay(50);

    type = bc_get_charger_type();
    printk(KERN_ERR "bc_get_dp_dm_status chg_type %d.\n", type);
    /* restore the default setting */
    bc_set_record_volt(record_reg);

    mdelay(20);

    /* restore to bypass mode */
    bc_set_bypass_mode();

    mdelay(2);

    return type;
}
#endif /* MBB_CHARGE */

void usb3_sysctrl_exit(void)
{
    /* enable the usb bc clock to change the bc mode */
    usb3_bc_clk_enable();

    /* reset controller and phy */
    hi_syscrg_usb_reset();

    /* disable the usb otg clock */
    usb3_otg_clk_disable();

    /* reset vbusvldext and override_en */
    hi_syssc_usb_override(0);
    //hi_syssc_usb_override_value(0);
    hi_syssc_usb_vbusvldext(0);

    /* reset ref_ssp_en */
    hi_syssc_usb_ref_ssp(0);

#if (FEATURE_ON == MBB_CHARGE)
    if(1 == bc_get_dp_dm_status() && 1 == chg_en_flag) {
        dp_dm_connected = 1;
    } else {
        dp_dm_connected = 0;
    }
    printk(KERN_ERR "dp_dm_connected %d.\n", dp_dm_connected);
#endif /* MBB_CHARGE */

    /* reset bc_mode to soft modes */
    bc_set_soft_mode();

    /* power down the usb2.0 and usb3.0 phy */
    hi_syssc_usb_powerdown_hsp(1);
    hi_syssc_usb_powerdown_ssp(1);

    mdelay(2);/*lint !e62*/
    usb3_bc_clk_disable();

	/* put the usb-otg & usb-bc clock */
    usb3_clk_put();
}


int usb_dwc3_platform_dev_init(void)
{
    int ret;

#ifndef CONFIG_USB_OTG_DWC
    /* init otg controller and phy */
    usb3_sysctrl_init();

    /* if the version support charger, identify the charger type
     * we must detect charger type before usb core init
     */
    if (bsp_usb_is_support_charger()) {
        bsp_usb_set_charger_type(bc_charger_type_identify());
    }

    /* clk off the bc controller, when we never use it */
    usb3_bc_clk_disable();

#endif

    dwc3_platform_dev_ptr = platform_device_alloc("dwc3", -1);
    if (!dwc3_platform_dev_ptr) {
        printk("%s:platform_device_alloc fail.\n",__FUNCTION__);
        return -ENOMEM;
    }

    ret = platform_device_add_resources(dwc3_platform_dev_ptr, dwc3_platform_resource, 2);
    if (ret) {
        printk("%s:platform_device_add_resources fail: %d.\n",__FUNCTION__, ret);
        goto fail_put_dev;
    }

    dwc3_platform_dev_ptr->dev.coherent_dma_mask = 0xffffffffUL;
    dwc3_platform_dev_ptr->dev.dma_mask = &dwc3_dma_mask;
	ret = platform_device_add(dwc3_platform_dev_ptr);
    if (ret) {
        printk("%s:platform_device_register fail: %d.\n",__FUNCTION__, ret);
        goto fail_put_dev;
    }
    return 0;

fail_put_dev:
    platform_device_put(dwc3_platform_dev_ptr);
    return ret;
}


void usb_dwc3_platform_dev_exit(void)
{
    platform_device_unregister(dwc3_platform_dev_ptr);

#ifndef CONFIG_USB_OTG_DWC
    /* reset the otg controller and phy */
    usb3_sysctrl_exit();
#endif
}

#if defined(BSP_CONFIG_BOARD_WINGLE)
void sdio_current_set(void)
{
    writel(MMC1_CLK_DRV_CURRENT_32MA,IO_ADDRESS(MMC1_CLK_DRV_ADD));
    printk(KERN_ERR " MMC1 CLK courrent set \n");
}
#endif

MODULE_AUTHOR("BALONG USBNET GROUP");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("BALONG USB3 DRD Controller Driver");
/*lint -restore*/
