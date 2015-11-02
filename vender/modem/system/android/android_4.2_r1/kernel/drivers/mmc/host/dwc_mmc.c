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
/*lint -save -e19 -e30 -e64 -e84 -e123 -e438 -e529 -e530 -e537 -e701 -e713 -e718 -e746 -e730 -e732 -e737 -e767 -e813 -e830 -e958 -e959*/
/*************************************************************************
  修改日期           修改人                问题单号            问题简要描述
*************************************************************************/
#include "hi_base.h"
#include "hi_syscrg_interface.h"
#include "hi_syssc_interface.h"
#include <linux/platform_device.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/interrupt.h>
#include <linux/dma-mapping.h>
#include <linux/scatterlist.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/irq.h>
#include <linux/ioport.h>
#include <linux/device.h>
#include <linux/switch.h>
#include <linux/delay.h>
#include <linux/dma-mapping.h>
#include <linux/kthread.h>
#include <linux/workqueue.h>
#include <linux/wakelock.h>
#include <linux/freezer.h>
#include <linux/io.h>
#include <linux/clk.h>
#include <linux/pm.h>
#include <linux/uaccess.h>
#include <linux/dma-mapping.h>
#include <linux/regulator/consumer.h>
#include <mach/gpio.h>
#include <linux/mmc/sdio_ids.h>
#include <linux/mmc/sdio_func.h>
#include <linux/mmc/card.h>
#include <linux/mmc/host.h>
#include <linux/mmc/mmc.h>
#include <linux/mmc/mshci.h>
#include <linux/ios/ios_balong.h>
#include "bsp_memmap.h"
#include "dwc_mmc.h"
#include "bsp_pmu.h"
#include "bsp_regulator.h"
#include "bsp_nvim.h"
#include "drv_nv_id.h"



#define MMC_FEATURE_NV      NV_ID_DRV_MMC_FEATURE
#include "bsp_version.h"

#include <linux/netlink.h>
#include <product_config.h>

extern void usb_notify_syswatch(int deviceid,int eventcode);

#if (FEATURE_ON == MBB_FAST_ON_OFF)
#define CONFIG_MBB_FAST_ON_OFF 
#endif



#if (FEATURE_ON == MBB_FAST_ON_OFF)
#include <linux/notifier.h>
#include "../../../../../../include/drv/drv_fastOnOff.h"
#endif 

#if (FEATURE_ON == MBB_FAST_ON_OFF)
extern struct blocking_notifier_head g_fast_on_off_notifier_list;
extern int card_detect_onoff_event(struct notifier_block *nb,
                                    unsigned long val, void *data);
#endif/*CONFIG_MBB_FAST_ON_OFF*/

#if defined(BSP_CONFIG_BOARD_WINGLE)
extern void sdio_current_set(void);
#endif

struct balong_mmc_platform_data {
	/* available voltages */
	unsigned int ocr_mask;
	/* available capabilities */
	unsigned long caps;
	unsigned long caps2;
	
	/* clock name */
	char *cclk_name;
	char *clk_name;
	char *refclk_name;
	char *phase_clk_name;
	unsigned cclk_value;
	/* deviations from spec. */
	unsigned int quirks;
	/* external detect card gpio */
	unsigned int cd_gpio;
	/* iomux name */
	char *iomux_name;
	
	char *regulator_mtcmos;	

	/* regulator name */
	char *regulator_sd;	
	/* signal voltage regulator name */
	char *regulator_io;
	char *regulator_io_m;
	char *dr1;
	int (*ext_cd_init)(void (*notify_func)(struct platform_device *,
		int state));
	int (*ext_cd_cleanup)(void (*notify_func)(struct platform_device *,
		int state));
	void (*set_power)(struct platform_device *, int val);
	int *timing_config;
	int *init_tuning_config;
	int suspend_timing_config;
	int allow_switch_signal_voltage;
	int default_signal_voltage;
};

struct hi_mci_debug_state {
	unsigned int sd_insert_times;
	unsigned int sd_remove_times;
	unsigned int pclk_enable_times;
	unsigned int pclk_disable_times;
	unsigned int refclk_enable_times;
	unsigned int refclk_disable_times;
	unsigned int phaseclk_enable_times;
	unsigned int phaseclk_disable_times;
	unsigned int cmd_times;

	unsigned int sd_mtcmos_vcc_power_up_times;
	unsigned int sd_vcc_power_up_times;
	unsigned int io_m_vcc_power_up_times;
	unsigned int io_vcc_power_up_times;
	unsigned int dr_power_up_times;
	unsigned int resister_enable_times;	
	
	unsigned int sd_mtcmos_vcc_power_off_times;
	unsigned int sd_vcc_power_off_times;
	unsigned int io_m_vcc_power_off_times;
	unsigned int io_vcc_power_off_times;
	unsigned int dr_power_off_times;
	unsigned int resister_disable_times;	
	unsigned int mmc_to_gpio;	
	unsigned int gpio_to_mmc;	
	unsigned int io_vcc_1_8v;
};
struct himci_host {
	struct mshci_host       *ms_host;
	struct device           *dev;
	struct platform_device  *pdev;
	struct clk              *pclk;				/* source clock */
	struct clk              *pcclk;				/* card clock interface */
	struct clk              *phase_clk;				/* phase clock interface */
	struct clk              *refclk;				/* ref clock interface */
	unsigned  int            clk_value;				/* clock interface */
	struct balong_mmc_platform_data *plat;
	struct regulator	    *sd_mtcmos_vcc;		/*for mtcmos*/
	struct regulator	    *vcc;				/*for LDO10*/
	struct regulator        *signal_vcc;		/*for LDO7*/
	struct regulator        *signal_m_vcc;		/*for LDO22*/
	struct regulator        *dr1; 				/*for DR1*/
	unsigned int			dr1_power_on;
	unsigned int			resister_enable;
	unsigned int			signal_m_vcc_power_on;
	unsigned int			signal_vcc_power_on;
	unsigned int			sd_mtcmos_vcc_power_on;
	unsigned int			sd_vcc_power_on;
	unsigned int			signal_vcc_1_8v;
	unsigned int			signal_m_vcc_1_425v;
	unsigned int			tuning_sample_flag;			/* record the sample OK or NOT */
	unsigned char           old_power_mode;     /* record power mode */
	unsigned char           old_sig_voltage;
	unsigned char			old_timing;

};

/* mmc nv structure ... */
struct himci_support_nv {
    unsigned int   support;
};


static struct himci_support_nv g_mmc_nv = {
	.support = 0,
};


static struct mmc_host *g_mmc_host;
#if (FEATURE_ON == MBB_COMMON)
static struct regulator *g_wifi_vcc = NULL;
#endif

static struct hi_mci_debug_state g_hi_mci_debug_state;

#ifdef CONFIG_MMC_V7R2
static struct balong_mmc_platform_data mmc0_pltfm_data = {
	.ocr_mask = MMC_VDD_27_28 | MMC_VDD_28_29 | MMC_VDD_29_30 |
				MMC_VDD_30_31 | MMC_VDD_31_32 | MMC_VDD_32_33 |
				MMC_VDD_34_35 | MMC_VDD_35_36,
	.caps = MMC_CAP_4_BIT_DATA | MMC_CAP_SD_HIGHSPEED |
		MMC_CAP_MMC_HIGHSPEED  | MMC_CAP_DRIVER_TYPE_A |MMC_CAP_UHS_SDR50,
	.quirks = MSHCI_QUIRK_EXTERNAL_CARD_DETECTION | 
		MSHCI_QUIRK_ALWAYS_WRITABLE,
	.clk_name = "mmc0_clk",
	.cclk_name = "mmc0_cclk",
	.cclk_value = MMC0_CCLK,
	.refclk_name = "mmc0_refclk",
	.phase_clk_name = "mmc0_phase_clk",
	.regulator_mtcmos = "sd_mmc0_mtcmos-vcc",
#ifndef CONFIG_MMC_SDIO_LOOP
	.caps2 = MMC_CAP2_DETECT_ON_ERR,
	.regulator_io_m = "SD_IO_M-vcc",
	.dr1 = HI6551_SD_DR,
#if(FEATURE_ON == MBB_FEATURE_SD_POLLING)  /*SD卡轮询检测*/
    .cd_gpio = 0,
#else
    .cd_gpio = SD_DETECT_GPIO,
#endif 
	.regulator_sd = "SD_POWER-vcc",
	.regulator_io = "SD_IO-vcc",
#endif
};

#else
static struct balong_mmc_platform_data mmc0_pltfm_data = {
	.ocr_mask = MMC_VDD_27_28 | MMC_VDD_28_29 | MMC_VDD_29_30 |
				MMC_VDD_30_31 | MMC_VDD_31_32 | MMC_VDD_32_33 |
				MMC_VDD_34_35 | MMC_VDD_35_36,
#if (FEATURE_ON == MBB_WIFI)
    .caps = MMC_CAP_4_BIT_DATA | MMC_CAP_SD_HIGHSPEED  | MMC_CAP_UHS_SDR50 | MMC_CAP_SDIO_IRQ,
#else
	.caps = MMC_CAP_4_BIT_DATA | MMC_CAP_SD_HIGHSPEED  | MMC_CAP_UHS_SDR104 | MMC_CAP_SDIO_IRQ,
#endif
	.quirks = MSHCI_QUIRK_WLAN_DETECTION |MSHCI_QUIRK_ALWAYS_WRITABLE | MSHCI_QUIRK_TUNING_ENABLE |
			MMC_QUIRK_BROKEN_BYTE_MODE_512,
	.clk_name = "mmc0_clk",
	.cclk_name = "mmc0_cclk",
	.cclk_value = MMC0_CCLK,
	.refclk_name = "mmc0_refclk",
	.phase_clk_name = "mmc0_phase_clk",
	.regulator_mtcmos = "sd_mmc0_mtcmos-vcc",
	.caps2 = MMC_CAP2_DETECT_ON_ERR,
};
#endif
/* mmc resource : mmc0 controller & interrupt */
static struct resource mmc0_resources[] = {
	[0] = {
		.start          = HI_MMC_HOST0_REGBASE_ADDR,
		.end            = HI_MMC_HOST0_REGBASE_ADDR + HI_MMC_HOST0_REG_SIZE - 1,
		.flags          = IORESOURCE_MEM,
	},
	[1] = {
		.start          = INT_LVL_MMC0,
		.end            = INT_LVL_MMC0,
		.flags          = IORESOURCE_IRQ,
	},
};

static struct platform_device mmc0_pltfm_device= {
	.name           = "hi_mci",
	.id             = 0,
	.num_resources  = ARRAY_SIZE(mmc0_resources),
	.resource       = mmc0_resources,
	.dev = {
		.coherent_dma_mask      = 0xffffffff,
		.platform_data          = &mmc0_pltfm_data,
	},
};

#ifdef CONFIG_MMC_V7R2
static struct balong_mmc_platform_data mmc1_pltfm_data = {
	.ocr_mask = MMC_VDD_27_28 | MMC_VDD_28_29 | MMC_VDD_29_30 |
				MMC_VDD_30_31 | MMC_VDD_31_32 | MMC_VDD_32_33 |
				MMC_VDD_34_35 | MMC_VDD_35_36,
	.caps = MMC_CAP_4_BIT_DATA | MMC_CAP_SD_HIGHSPEED |	
		MMC_CAP_NONREMOVABLE |MMC_CAP_DRIVER_TYPE_A |
		MMC_CAP_UHS_SDR104 | MMC_CAP_SDIO_IRQ,
	.clk_name = "mmc1_clk",
	.cclk_name = "mmc1_cclk",
	.cclk_value = MMC1_CCLK,
	.refclk_name = "mmc1_refclk",
	.phase_clk_name = "mmc1_phase_clk",
	.regulator_mtcmos = "sd_mtcmos-vcc",

	.quirks = MSHCI_QUIRK_WLAN_DETECTION |MSHCI_QUIRK_ALWAYS_WRITABLE |
			MMC_QUIRK_BROKEN_BYTE_MODE_512,
};
#else
/*for sdcard*/
static struct balong_mmc_platform_data mmc1_pltfm_data = {
	.ocr_mask = MMC_VDD_27_28 | MMC_VDD_28_29 | MMC_VDD_29_30 |
				MMC_VDD_30_31 | MMC_VDD_31_32 | MMC_VDD_32_33 |
				MMC_VDD_34_35 | MMC_VDD_35_36,
#if (FEATURE_ON == MBB_COMMON)
	.caps = MMC_CAP_4_BIT_DATA | MMC_CAP_SD_HIGHSPEED |
		MMC_CAP_MMC_HIGHSPEED  | MMC_CAP_DRIVER_TYPE_A | MMC_CAP_ERASE,  
#else
    	.caps = MMC_CAP_4_BIT_DATA | MMC_CAP_SD_HIGHSPEED |
		MMC_CAP_MMC_HIGHSPEED  | MMC_CAP_DRIVER_TYPE_A |MMC_CAP_UHS_SDR104 | MMC_CAP_ERASE,
#endif
	.caps2 = MMC_CAP2_DETECT_ON_ERR,
	.quirks = MSHCI_QUIRK_TUNING_ENABLE | MSHCI_QUIRK_ALWAYS_WRITABLE,
	.clk_name = "mmc1_clk",
	.cclk_name = "mmc1_cclk",
	.cclk_value = MMC1_CCLK,
	.refclk_name = "mmc1_refclk",
	.phase_clk_name = "mmc1_phase_clk",
	.regulator_mtcmos = "sd_mtcmos-vcc",
	.regulator_sd = "SD_POWER-vcc",
	.regulator_io = "SD_IO-vcc",

	.cd_gpio = GPIO_2_19 ,

};

#endif

/* 
  *mmc1 resource : mmc1 controller & interrupt
  * sdcc
  */
static struct resource mmc1_resources[] = {
	[0] = {
		.start          = HI_MMC_HOST1_REGBASE_ADDR,
		.end            = HI_MMC_HOST1_REGBASE_ADDR + HI_MMC_HOST1_REG_SIZE - 1,
		.flags          = IORESOURCE_MEM,
	},
	[1] = {
		.start          = INT_LVL_MMC1,
		.end            = INT_LVL_MMC1,
		.flags          = IORESOURCE_IRQ,
	},
};

struct platform_device mmc1_pltfm_device= {
	.name           = "hi_mci",
	.id             = 1,
	.num_resources  = ARRAY_SIZE(mmc1_resources),
	.resource       = mmc1_resources,
	.dev = {
		.coherent_dma_mask      = 0xffffffff,
		.platform_data          = &mmc1_pltfm_data,
	},
};

#if (FEATURE_ON == MBB_FAST_ON_OFF)
static struct notifier_block card_detect_block = {
    .notifier_call = card_detect_onoff_event
};
#endif

static void mmc_set_host(struct mmc_host *host)
{
	g_mmc_host = host;
}

struct mmc_host* mmc_get_host(void)
{
	return g_mmc_host;
}


static ssize_t mshci_hi_show_debug_state(struct device *dev, struct device_attribute *attr,
			     char *buf)
{
	size_t count = 0;

	count = snprintf(buf, PAGE_SIZE, "|-+dw mmc debug info dump:\n");
	count += snprintf(buf+count, PAGE_SIZE, "| |--pclk enable times		%d\n", g_hi_mci_debug_state.pclk_enable_times);
	count += snprintf(buf+count, PAGE_SIZE, "| |--pclk disable times		%d\n", g_hi_mci_debug_state.pclk_disable_times);
	count += snprintf(buf+count, PAGE_SIZE, "| |--refclk enable times		%d\n", g_hi_mci_debug_state.refclk_enable_times);
	count += snprintf(buf+count, PAGE_SIZE, "| |--refclk disable times		%d\n", g_hi_mci_debug_state.refclk_disable_times);
	count += snprintf(buf+count, PAGE_SIZE, "| |--phase clk enable times		%d\n", g_hi_mci_debug_state.phaseclk_enable_times);
	count += snprintf(buf+count, PAGE_SIZE, "| |--phase clk disable times		%d\n", g_hi_mci_debug_state.phaseclk_disable_times);
	count += snprintf(buf+count, PAGE_SIZE, "| |--mmc0 ctmos power up times		%d\n", g_hi_mci_debug_state.sd_mtcmos_vcc_power_up_times);
	count += snprintf(buf+count, PAGE_SIZE, "| |--mmc0 ctmos power off times		%d\n", g_hi_mci_debug_state.sd_mtcmos_vcc_power_off_times);
	count += snprintf(buf+count, PAGE_SIZE, "| |--sd vcc power up times		%d\n", g_hi_mci_debug_state.sd_vcc_power_up_times);
	count += snprintf(buf+count, PAGE_SIZE, "| |--sd vcc power off times		%d\n", g_hi_mci_debug_state.sd_vcc_power_off_times);
	//count += snprintf(buf+count, PAGE_SIZE, "| |--io vcc power up times		%d\n", g_hi_mci_debug_state.io_vcc_power_up_times);
	//count += snprintf(buf+count, PAGE_SIZE, "| |--io vcc power off times		%d\n", g_hi_mci_debug_state.io_vcc_power_off_times);
	count += snprintf(buf+count, PAGE_SIZE, "| |--io m vcc power up times		%d\n", g_hi_mci_debug_state.io_m_vcc_power_up_times);
	count += snprintf(buf+count, PAGE_SIZE, "| |--io m vcc power off times		%d\n", g_hi_mci_debug_state.io_m_vcc_power_off_times);
	count += snprintf(buf+count, PAGE_SIZE, "| |--dr  power up times		%d\n", g_hi_mci_debug_state.dr_power_up_times);
	count += snprintf(buf+count, PAGE_SIZE, "| |--dr  power off times		%d\n", g_hi_mci_debug_state.dr_power_off_times);
	count += snprintf(buf+count, PAGE_SIZE, "| |--resister enable times		%d\n", g_hi_mci_debug_state.resister_enable_times);
	count += snprintf(buf+count, PAGE_SIZE, "| |--resister disable times		%d\n", g_hi_mci_debug_state.resister_disable_times);
	count += snprintf(buf+count, PAGE_SIZE, "| |--sd insert times		%d\n", g_hi_mci_debug_state.sd_insert_times);
	count += snprintf(buf+count, PAGE_SIZE, "| |--sd remove times		%d\n", g_hi_mci_debug_state.sd_remove_times);
	count += snprintf(buf+count, PAGE_SIZE, "| |--switch mmc io to gpio		%d\n", g_hi_mci_debug_state.mmc_to_gpio);
	count += snprintf(buf+count, PAGE_SIZE, "| |--switch gpio to mmc io		%d\n", g_hi_mci_debug_state.gpio_to_mmc);
	count += snprintf(buf+count, PAGE_SIZE, "| |--cmd times		%d\n", g_hi_mci_debug_state.cmd_times);
	count += snprintf(buf+count, PAGE_SIZE, "| |--io vcc voltage		%d\n", g_hi_mci_debug_state.io_vcc_1_8v);
	return count;
}


static ssize_t mshci_hi_store_debug_state(struct device *dev, struct device_attribute *attr,
			      const char *buf, size_t count)
{
	return count;
}

static DEVICE_ATTR(debug_state, 0644, mshci_hi_show_debug_state, mshci_hi_store_debug_state);

unsigned int mshci_hi_get_sd_status(unsigned int gpio)
{
	unsigned int ret;
	ret = gpio_get_value(gpio);
	return ret;
}

static void  mshci_hi_smaple_drv_init(unsigned int pid, unsigned int timing)
{
	unsigned int smaple = 0;
	unsigned int drv = 0;
	/*0-400k 12M 25M, 1-50M, 2-104M*/
	if (pid == 0) {
#ifdef CONFIG_MMC_V7R2
		if (timing == 0) {
			smaple = 0;
			drv = 34;
		} else {
			smaple = 2;
			drv = 2;
		}
#else
		if (timing == 0) {
			smaple = 0x22;
			drv = 0x5;
		} else if (timing == 1){
			smaple = 0x24;
			drv = 0x4;
		} else if (timing == 2){
			drv = 0x4;
		}
#endif
	} else if (pid == 1)  {
#ifdef CONFIG_MMC_V7R2
		smaple = 0;
		drv = 3;
#else
		if (timing == 0) {
			smaple = 0x42;
			drv = 0x5;
		} else if (timing == 1)  {
			smaple = 0x45;
			drv = 0x4;
		} else if (timing == 2){
			drv = 0x4;
		} 
#endif
	}

	if (pid == 0) {
		hi_syscrg_mmc_ctrl_mmc0_sample_sel(smaple);
		hi_syscrg_mmc_ctrl_mmc0_drv_sel(drv);
	} else if (pid == 1) {
		hi_syscrg_mmc_ctrl_mmc1_sample_sel(smaple);
		hi_syscrg_mmc_ctrl_mmc1_drv_sel(drv);
	} else {

	}
}
#ifdef CONFIG_MMC_V711
static int mshci_hi_refclk_sel(struct clk* refclk, int div)
{
	struct clk *paclk = NULL;
	int ret;
	
	if (0 == div) {
		paclk = clk_get(NULL, "peripll_fout1");
	} else if (1 == div) {
		paclk = clk_get(NULL, "dsp_pll");
	}
	if (IS_ERR(paclk)) {
		ret = PTR_ERR(paclk); 
		return ret;
	}
	
	ret = clk_set_parent(refclk, paclk);

	return ret;

}
#else
static int mshci_hi_refclk_sel(struct clk* refclk, int pid, int div)
{
	return 0;
}
#endif

#if (FEATURE_ON == MBB_COMMON)
static int  mshci_hi_clk_div_init(struct himci_host *hi_host, 
	unsigned int timing, unsigned int max_dtr)
{
	unsigned int phase_clk_source = MMC_CLOCK_SOURCE_480M;
	unsigned int ref_clk_source = MMC_CLOCK_SOURCE_480M;
	int ret = 0;
#ifdef CONFIG_MMC_V711
	/*ref clk choose 600MHz*/
	if (hi_host->refclk) {
		ref_clk_source = MMC_CLOCK_SOURCE_600M;
		ret = mshci_hi_refclk_sel(hi_host->refclk, 1);
	}

	/*400k sdr 12/sdr25 use 300MHz, sdr50 sdr104 use 600MHz*/
	if ((timing == 1) || (timing == 2)) {
		phase_clk_source = MMC_CLOCK_SOURCE_600M;
	} else {
	    phase_clk_source = MMC_CLOCK_SOURCE_600M/2;
	}
#else
	/*sdr 12, sdr25 use 240MHz, sdr50 use 480MHz*/
	phase_clk_source = timing ? (MMC_CLOCK_SOURCE_480M / 2) : MMC_CLOCK_SOURCE_480M;
#endif
	if (!ret && hi_host->phase_clk) {
		ret = clk_set_rate(hi_host->phase_clk, phase_clk_source);
	}
	/*not use ip clk div, set card clock max_dtr*/
	hi_host->ms_host->max_clk = min(max_dtr,hi_host->clk_value);
	
	if (!ret && hi_host->pcclk) {
		ret = clk_set_rate(hi_host->pcclk, hi_host->ms_host->max_clk);
	}
	return ret;
}
#else
static int  mshci_hi_clk_div_init(struct himci_host *hi_host, 
	unsigned int timing, unsigned int max_dtr)
{
	unsigned int phase_clk_source = MMC_CLOCK_SOURCE_480M;
	unsigned int ref_clk_source = MMC_CLOCK_SOURCE_480M;
	int ret = 0;
#ifdef CONFIG_MMC_V711
	/*ref clk 400k choose 480MHz, others choose 600MHz*/
	if (hi_host->refclk) {
		#if 0
		if (timing == ENUM_SPEED_BUS_SPEED) {
			ref_clk_source = MMC_CLOCK_SOURCE_480M;
			ret = mshci_hi_refclk_sel(hi_host->refclk, 0);
		} else {
		#endif
			ref_clk_source = MMC_CLOCK_SOURCE_600M;
			ret = mshci_hi_refclk_sel(hi_host->refclk, 1);
		#if 0
		}
		#endif
	}

	/*400k 60MHz sdr 12, sdr25 use 300MHz, sdr50 sdr104 use 600MHz*/
	if ((timing == 1) || (timing == 2)) {
		phase_clk_source = MMC_CLOCK_SOURCE_600M;
	} else {
		#if 0
		phase_clk_source = (ref_clk_source == MMC_CLOCK_SOURCE_600M) \
				? (ref_clk_source / 2) : (ref_clk_source / 8);
		#else
		phase_clk_source =(ref_clk_source / 2);
		#endif
	}
#else
	/*sdr 12, sdr25 use 240MHz, sdr50 use 480MHz*/
	phase_clk_source = timing ? (MMC_CLOCK_SOURCE_480M / 2) : MMC_CLOCK_SOURCE_480M;
#endif
	if (!ret && hi_host->phase_clk) {
		ret = clk_set_rate(hi_host->phase_clk, phase_clk_source);
	}
	/*not use ip clk div, set card clock max_dtr*/
	hi_host->ms_host->max_clk = min(max_dtr,hi_host->clk_value);
	
	if (!ret && hi_host->pcclk) {
		ret = clk_set_rate(hi_host->pcclk, hi_host->ms_host->max_clk);
	}
	return ret;
}
#endif
static int  mshci_hi_get_vcc(struct himci_host *hi_host)
{
	struct balong_mmc_platform_data *plat = hi_host->plat;
	int ret = 0;
	
	if (plat->regulator_sd) {
		hi_host->vcc = regulator_get(NULL, plat->regulator_sd);
		if (IS_ERR(hi_host->vcc)) {
			ret = PTR_ERR(hi_host->vcc);
			goto out;
		}
	}
	
	if (plat->regulator_mtcmos) {
		hi_host->sd_mtcmos_vcc = regulator_get(NULL, plat->regulator_mtcmos);
		if (IS_ERR(hi_host->sd_mtcmos_vcc)) {
			ret = PTR_ERR(hi_host->sd_mtcmos_vcc);
			goto put_sd;
		}
	}

	if (plat->regulator_io) {
		hi_host->signal_vcc = regulator_get(NULL, plat->regulator_io);
		if (IS_ERR(hi_host->signal_vcc)) {
			ret = PTR_ERR(hi_host->signal_vcc);
			goto put_mtcmos;
		}
	}
	
	if (plat->regulator_io_m) {
		hi_host->signal_m_vcc = regulator_get(NULL, plat->regulator_io_m);
		if (IS_ERR(hi_host->signal_m_vcc)) {
			ret = PTR_ERR(hi_host->signal_m_vcc);
			goto put_io;
		}
	}

	if (plat->dr1) {
		hi_host->dr1= regulator_get(NULL, plat->dr1);
		if (IS_ERR(hi_host->dr1)) {
			ret = PTR_ERR(hi_host->dr1);
			goto put_dr1;
		}
	}

	return 0;
put_dr1:
	if (plat->regulator_io_m) {
		regulator_put(hi_host->signal_m_vcc);	
	}	
put_io:
	if (plat->regulator_io) {
		regulator_put(hi_host->signal_vcc);	
	}
put_mtcmos:
	if (plat->regulator_mtcmos) {
		regulator_put(hi_host->sd_mtcmos_vcc);	
	}
put_sd:
	if (plat->regulator_sd) {
		regulator_put(hi_host->vcc);
	}
out:
	return ret;
}


static int  mshci_hi_get_clk(struct himci_host *hi_host)
{
	struct balong_mmc_platform_data *plat = hi_host->plat;
	int ret = 0;
	
	if(plat->clk_name) {
		hi_host->pclk = clk_get(NULL, plat->clk_name);
		if (IS_ERR(hi_host->pclk)) {
			ret = PTR_ERR(hi_host->pclk);
			goto out;
		}
	}
	
	if(plat->refclk_name) {
		hi_host->refclk= clk_get(NULL, plat->refclk_name);
		if (IS_ERR(hi_host->refclk)) {
			ret = PTR_ERR(hi_host->refclk);
			goto put_clk;
		}
	}
	
	if(plat->cclk_name) {
		hi_host->pcclk = clk_get(NULL, plat->cclk_name);
		if (IS_ERR(hi_host->pcclk)) {
			ret = PTR_ERR(hi_host->pcclk);
			goto put_refclk;
		}
	}
	
	if(plat->phase_clk_name) {
		hi_host->phase_clk = clk_get(NULL, plat->phase_clk_name);
		if (IS_ERR(hi_host->phase_clk)) {
			ret = PTR_ERR(hi_host->phase_clk);
			goto put_cclk;
		}
	}

	return 0;
put_cclk:
	if (plat->cclk_name) {
		clk_put(hi_host->pcclk);
	}
put_refclk:
	if (plat->refclk_name) {
		clk_put(hi_host->refclk);	
	}
put_clk:
	if (plat->clk_name) {
		clk_put(hi_host->pclk);
	}
out:
	return ret;
}

static void  mshci_hi_put_clk(struct himci_host *hi_host)
{
	if (hi_host->pcclk) {
		clk_put(hi_host->pcclk);
	}
	if (hi_host->refclk) {
		clk_put(hi_host->refclk);
	}
	if (hi_host->pclk) {
		clk_put(hi_host->pclk);
	}
	if (hi_host->phase_clk) {
		clk_put(hi_host->phase_clk);
	}
}

static int  mshci_hi_sd_vcc_power_up(struct himci_host *hi_host)
{
	int ret = 0;
	
	if (hi_host->vcc && !hi_host->sd_vcc_power_on) {
		ret = regulator_enable(hi_host->vcc);
		if (!ret) {
			g_hi_mci_debug_state.sd_vcc_power_up_times++;
			hi_host->sd_vcc_power_on = 1;		
		}
	}
	
	return ret;
}

static int  mshci_hi_sdctmos_vcc_power_up(struct himci_host *hi_host)
{
	int ret = 0;
	
	if (hi_host->sd_mtcmos_vcc && !hi_host->sd_mtcmos_vcc_power_on) {
		ret = regulator_enable(hi_host->sd_mtcmos_vcc);
		if (!ret) {
			g_hi_mci_debug_state.sd_mtcmos_vcc_power_up_times++;
			hi_host->sd_mtcmos_vcc_power_on= 1;
		}
	}
	
	return ret;
}

static int  mshci_hi_sdctmos_vcc_power_off(struct himci_host *hi_host)
{
	int ret = 0;
	
	if (hi_host->sd_mtcmos_vcc && hi_host->sd_mtcmos_vcc_power_on) {
		ret = regulator_disable(hi_host->sd_mtcmos_vcc);
		if (!ret) {
			g_hi_mci_debug_state.sd_mtcmos_vcc_power_off_times++;
			hi_host->sd_mtcmos_vcc_power_on = 0;
		}
	}
	
	return ret;
}

static int  mshci_hi_io_vcc_to_1_8v(struct himci_host *hi_host)
{
	int ret = 0;

	if (!hi_host->signal_vcc/* || !hi_host->signal_m_vcc*/) {
		return ret;
	}
#ifndef CONFIG_MMC_V711
	/*disable resistor*/
	if (hi_host->resister_enable && 0 == hi_host->pdev->id) {
		bsp_pmu_ldo22_res_disable();
		hi_host->resister_enable = 0;
		g_hi_mci_debug_state.resister_disable_times++;
	}	
#endif

	ret = regulator_set_voltage(hi_host->signal_vcc, 1800000, 1800000);
	if (!ret) {
		g_hi_mci_debug_state.io_vcc_1_8v = 1;
		hi_host->signal_vcc_1_8v = 1;
		/*disable ldo22*/
#ifdef CONFIG_MMC_V711
		hi_syssc_mmc_voltage_18v(1);
		
#else
		ret = regulator_disable(hi_host->signal_m_vcc);
		if (!ret) {
			/*dr only for 1.8v*/
			if (hi_host->dr1 && !hi_host->dr1_power_on) {
				ret = regulator_enable(hi_host->dr1);
				if (!ret) {
					hi_host->dr1_power_on = 1;
					g_hi_mci_debug_state.dr_power_up_times++;
					udelay(20);
				}
			}
			hi_host->signal_m_vcc_power_on = 0;
			g_hi_mci_debug_state.io_m_vcc_power_off_times++;
#endif
		} else {
			ret = regulator_set_voltage(hi_host->signal_vcc, 2850000, 2850000);
			if (!ret) {
				g_hi_mci_debug_state.io_vcc_1_8v = 0;
				hi_host->signal_vcc_1_8v = 0;
			}
		}
#ifdef CONFIG_MMC_V711
#else
	}
#endif
	return ret;
}

static int  mshci_hi_io_vcc_to_2_85v(struct himci_host *hi_host)
{
	int ret = 0;
	if (!hi_host->signal_vcc/* || !hi_host->signal_m_vcc*/) {
		return ret;
	}
#ifdef CONFIG_MMC_V711
	hi_syssc_mmc_voltage_18v(0);
#else	
	/*dr only for 1.8v*/
	if (hi_host->dr1 && hi_host->dr1_power_on) {
		ret = regulator_disable(hi_host->dr1);
		if (!ret) {
			hi_host->dr1_power_on = 0;
			g_hi_mci_debug_state.dr_power_off_times++;
		}
	}
#endif

	ret = regulator_set_voltage(hi_host->signal_vcc, 2850000, 2850000);
	if (!ret) {
		hi_host->signal_vcc_1_8v = 0;
		g_hi_mci_debug_state.io_vcc_1_8v = 0;
		/*enable ldo22*/
#ifndef CONFIG_MMC_V711
		ret = regulator_enable(hi_host->signal_m_vcc);
		if (!ret) {
			/*enable resistor*/
			if (!hi_host->resister_enable && 0 == hi_host->pdev->id) {
				bsp_pmu_ldo22_res_enable();
				hi_host->resister_enable = 1;
				g_hi_mci_debug_state.resister_enable_times++;
			}
			hi_host->signal_m_vcc_power_on = 1;
			g_hi_mci_debug_state.io_m_vcc_power_up_times++;	
#endif
		} else {
			ret = regulator_set_voltage(hi_host->signal_vcc, 1800000, 1800000);
			if (!ret) {
				g_hi_mci_debug_state.io_vcc_1_8v = 1;
				hi_host->signal_vcc_1_8v = 1;
			}
		}
#ifdef CONFIG_MMC_V711
#else
	}
#endif
	return ret;
}


static void  mshci_hi_put_vcc(struct himci_host *hi_host)
{
	if (hi_host->vcc) {
		regulator_put(hi_host->vcc);
	}
	if (hi_host->signal_vcc) {
		regulator_put(hi_host->signal_vcc);
	}
	if (hi_host->signal_m_vcc) {
		regulator_put(hi_host->signal_m_vcc);
	}
	if (hi_host->sd_mtcmos_vcc) {
		regulator_put(hi_host->sd_mtcmos_vcc);
	}
}


static int  mshci_hi_clk_enable(struct himci_host *hi_host)
{
	int ret = 0;
	if (!hi_host->ms_host->clock_gate) {
		return ret;
	}
	if (hi_host->pclk) {
		ret = clk_enable(hi_host->pclk);
		if (!ret) {
			g_hi_mci_debug_state.pclk_enable_times++;
		} else {
			goto out;
		}
	}
	if (hi_host->refclk) {
		ret = clk_enable(hi_host->refclk);
		if (!ret) {
			g_hi_mci_debug_state.refclk_enable_times++;
		} else {
			goto out;
		}
	}
	if (hi_host->phase_clk) {
		ret = clk_enable(hi_host->phase_clk);
		if (!ret) {
			g_hi_mci_debug_state.phaseclk_enable_times++;
		} 
	}
	if (!ret) {
		hi_host->ms_host->clock_gate = 0;
	}
	
out:
	return ret;
}


#ifdef CONFIG_MMC_CLKGATE

static int  mshci_hi_clk_disable(struct himci_host *hi_host)
{
	if (hi_host->ms_host->clock_gate) {
		return 0;
	}
	if (hi_host->pclk) {
		clk_disable(hi_host->pclk);
		g_hi_mci_debug_state.pclk_disable_times++;
	}
	if (hi_host->refclk) {
		clk_disable(hi_host->refclk);
		g_hi_mci_debug_state.refclk_disable_times++;
	}
	if (hi_host->phase_clk) {
		clk_disable(hi_host->phase_clk);
		g_hi_mci_debug_state.phaseclk_disable_times++;
	}

	hi_host->ms_host->clock_gate = 1;
	
	return 0;
}


static void  mshci_hi_enable_off_clock(struct mshci_host *ms_host, unsigned int enable)
{
	struct himci_host *hi_host = mshci_priv(ms_host);
	if (enable) {
		mshci_hi_clk_enable(hi_host);
	} else {
		mshci_hi_clk_disable(hi_host);
	}
}
#else

static int  mshci_hi_clk_disable(struct himci_host *hi_host)
{
	return 0;
}
static void  mshci_hi_enable_off_clock(struct mshci_host *ms_host,
	unsigned int enable){}

#endif
static irqreturn_t mshci_hi_card_detect_gpio(int irq, void *data)
{
	struct mshci_host *ms_host;
	struct himci_host *hi_host;
	unsigned int old_gpio_value;
	unsigned int gpio_state;
	
	ms_host = (struct mshci_host *)data;
	if (!ms_host) {
		return IRQ_HANDLED;
	}
	
	hi_host = mshci_priv(ms_host);
	irq = hi_host->plat->cd_gpio;
	gpio_state = gpio_int_state_get(irq);
	if(!gpio_state) {
		return IRQ_NONE;
	}
	gpio_int_mask_set(irq);
	gpio_int_state_clear(irq);
	
	old_gpio_value = gpio_get_value((unsigned)irq);
	if (old_gpio_value) {
#if (FEATURE_ON == MBB_USB_SD)
        usb_notify_syswatch(DEVICE_ID_SD, SD_REMOVE);
#endif
		gpio_int_trigger_set(irq, IRQ_TYPE_LEVEL_LOW);
		g_hi_mci_debug_state.sd_remove_times++;
	} else {
		gpio_int_trigger_set(irq, IRQ_TYPE_LEVEL_HIGH);
		g_hi_mci_debug_state.sd_insert_times++;
	}
	gpio_int_unmask_set(irq);
	
	tasklet_schedule(&ms_host->card_tasklet);
	
	return IRQ_HANDLED;
}

static unsigned int mshci_hi_get_present_status(struct mshci_host *ms_host)
{
	unsigned int status;
	struct himci_host *hi_host = mshci_priv(ms_host);
	if (ms_host->quirks & MSHCI_QUIRK_WLAN_DETECTION) {
		if (ms_host->flags & MSHCI_DEVICE_DEAD) {
			status = 0;
		} else {
			status = 1;
		}
		DBG(hi_host, "embedded card %s", 
			!status ? "inserted" : "removed\n");
		return status;
    } 
#if(FEATURE_ON == MBB_FEATURE_SD_POLLING)  /*SD卡轮询检测*/
    else if ((ms_host->quirks & MSHCI_QUIRK_EXTERNAL_CARD_DETECTION) 
               && hi_host->plat->cd_gpio)
    {
        /* GPIO Low mean SD Card insert; GPIO Hight means no SD card */
        status = mshci_hi_get_sd_status(hi_host->plat->cd_gpio);
        return !status;
    }
    else if (ms_host->quirks && !(hi_host->plat->cd_gpio))
    {
        return 1;
    }
#else
    else if (ms_host->quirks & MSHCI_QUIRK_EXTERNAL_CARD_DETECTION) {
		/* GPIO Low mean SD Card insert; GPIO Hight means no SD card */
		status = mshci_hi_get_sd_status(hi_host->plat->cd_gpio);
		DBG(hi_host, "extern detect card %s", 
			!status ? "inserted" : "removed\n");
		return !status;
    }
#endif
    else {
		return 0;
	}
}

static int mshci_hi_start_signal_voltage_switch(struct mshci_host *ms_host,
				struct mmc_ios *ios)
{
	int ret = 0;
	unsigned int io_voltage;
	struct himci_host *hi_host = mshci_priv(ms_host);
	if (ios->signal_voltage == MMC_SIGNAL_VOLTAGE_180) {
		if (!hi_host->signal_vcc_1_8v) {
			ret = mshci_hi_io_vcc_to_1_8v(hi_host);
		}
	}
	else if (ios->signal_voltage == MMC_SIGNAL_VOLTAGE_330) {
		io_voltage = 2850000;
		if (hi_host->vcc) {
			ret = regulator_set_voltage(hi_host->vcc, io_voltage, io_voltage);
			if (ret) {
				return ret;
			}
		}
		if (hi_host->signal_vcc_1_8v) {
			ret = mshci_hi_io_vcc_to_2_85v(hi_host);
		}
	}
	else {
	}
	return ret;
}


int mshci_hi_tuning_find_condition(struct mshci_host *host)
{
	int i;
	int j;
	int delay_chain;
	int phase_shift_step;
	unsigned int sample;
	unsigned int tuning[24];
	struct mmc_request mrq = {NULL};
	struct mmc_command cmd = {0};
	struct mmc_command stop = {0};
	struct mmc_data data = {0};
	struct scatterlist sg;
	void *data_buf;
	struct himci_host *hi_host = mshci_priv(host);
	data_buf = kmalloc(64, GFP_KERNEL);
	if (data_buf == NULL) {
		return -ENOMEM;
	}
	for (i = 0;i < 24; i++) {
		tuning[i] = 0;
	}
#ifdef CONFIG_MMC_V711
	if (hi_host->pdev->id == 1) {
		hi_syssc_ctrl3_mmc1_sample_tuning_enable(1);
	} else if (hi_host->pdev->id == 0) {
		hi_syssc_ctrl3_mmc0_sample_tuning_enable(1);
	}
#else
	hi_syssc_ctrl3_mmc1_sample_tuning_enable(1);
#endif

#ifdef CONFIG_MMC_V711
	if (hi_host->pdev->id == 1) {
		hi_crg_mmc_ctrl_mmc1_sample_ctrl(0);
	}else if (hi_host->pdev->id == 0) {
		hi_crg_mmc_ctrl_mmc0_sample_ctrl(0);
	}
#else
	hi_syscrg_mmc_ctrl_mmc1_drv_sel(2);
#endif

#ifdef CONFIG_MMC_V711
	if (hi_host->pdev->id == 1) {
		hi_syscrg_mmc_ctrl_mmc1_clk_bypass(0);
	} else if (hi_host->pdev->id == 0) {
		hi_syscrg_mmc_ctrl_mmc0_clk_bypass(0);
	}
#else
	hi_syscrg_mmc_ctrl_mmc1_clk_bypass(0);
#endif

	j = 0;
	for (i = 0; i < 24; i++) {
		delay_chain = i / 6;
		phase_shift_step = i % 6;
		/**/
		sample = delay_chain << 4 | phase_shift_step;
		#ifdef CONFIG_MMC_V711
		if (hi_host->pdev->id == 1) {
			hi_syscrg_mmc_ctrl_mmc1_sample_sel(sample);
		} else if (hi_host->pdev->id == 0) {
			hi_syscrg_mmc_ctrl_mmc0_sample_sel(sample);
		}
		#else
		hi_syscrg_mmc_ctrl_mmc1_sample_sel(sample);
		#endif
		
		/*send cmd19*/

		/* dma onto stack is unsafe/nonportable, but callers to this
		 * routine normally provide temporary on-stack buffers ...
		 */

		mrq.cmd = &cmd;
		mrq.stop = &stop;
		mrq.data = &data;

		cmd.opcode = MMC_SEND_TUNING_BLOCK;
		cmd.arg = 0;
		cmd.flags = MMC_RSP_R1 | MMC_CMD_ADTC;

		stop.opcode = MMC_STOP_TRANSMISSION;
		stop.arg = 0;
		stop.flags = MMC_RSP_R1B | MMC_CMD_AC;	
		
		data.blksz = 64;
		data.blocks = 1;
		data.flags = MMC_DATA_READ;
		data.sg = &sg;
		data.sg_len = 1;

		sg_init_one(&sg, data_buf, 64);
		

		mmc_wait_for_req(host->mmc, &mrq);

		if (cmd.error || data.error) {
			j = 0;
		} else {
			j++;
			if (j == 3) {
				break;
			}
		}
		if (i < 2 || i > 21) {
			if (j) {
				tuning[i] = 1;
			}
		}

	}
	kfree(data_buf);
	if (j == 3) { 
		return (i - 1);
	} else {
		if (tuning[0] * tuning[1] * tuning[23]) {
			return 0;
		} else if (tuning[0] * tuning[22] * tuning[23]) {
			return 23;
		} else {
			return -1;
		}
	}
}
void mshci_hi_tuning_set_current_state(struct mshci_host *host, int clk)
{
	struct himci_host *hi_host = mshci_priv(host);
#ifdef CONFIG_MMC_V711
	if (hi_host->pdev->id == 1) {
		hi_syscrg_mmc_ctrl_mmc1_sample_sel(clk);
	} else if (hi_host->pdev->id == 0) {
		hi_syscrg_mmc_ctrl_mmc0_sample_sel(clk);
	}
#else
	hi_syscrg_mmc_ctrl_mmc1_sample_sel(clk);
#endif
	
}
static int mshci_hi_vcc_power_off(struct himci_host *hi_host);
static void mshci_hi_set_ios(struct mshci_host *host, struct mmc_ios *ios)
{
	struct himci_host *hi_host = (struct himci_host *)mshci_priv(host);

	switch (ios->power_mode) {
		case MMC_POWER_UP:
			if (hi_host->ms_host->quirks 
				& MSHCI_QUIRK_EXTERNAL_CARD_DETECTION) {
				(void)mshci_hi_sd_vcc_power_up(hi_host);
			}
			
			mshci_hi_clk_enable(hi_host);
			break;
		case MMC_POWER_OFF:
			mshci_hi_clk_disable(hi_host);
			if (hi_host->ms_host->quirks 
				& MSHCI_QUIRK_EXTERNAL_CARD_DETECTION) {
				mshci_hi_vcc_power_off(hi_host);
			}
			break;
		default:
			if (host->voltage_int_count == 1) {
				break;
			}
			if (ios->clock == 0) {
				mshci_hi_clk_disable(hi_host);
			} else if (host->clock != ios->clock) {
				switch (ios->timing) {
					int timing;
					int max_dtr;

					case MMC_TIMING_UHS_SDR12:
						/*400k chose 600M*/
						timing = (ios->clock < UHS_SDR12_MAX_DTR )
							? ENUM_SPEED_BUS_SPEED : 0;
						max_dtr = (ios->clock < UHS_SDR12_MAX_DTR )
							? MMC_CLOCK_SOURCE_15M : UHS_SDR12_MAX_DTR;
						mshci_hi_smaple_drv_init(hi_host->pdev->id, 0);
						mshci_hi_clk_div_init(hi_host, timing, max_dtr);
						break;

					case MMC_TIMING_UHS_SDR25:
						mshci_hi_smaple_drv_init(hi_host->pdev->id, 0);
						mshci_hi_clk_div_init(hi_host, 0, UHS_SDR25_MAX_DTR);
						break;
					case MMC_TIMING_UHS_SDR50:
						mshci_hi_smaple_drv_init(hi_host->pdev->id, 1);
						mshci_hi_clk_div_init(hi_host, 1, UHS_SDR50_MAX_DTR);
						break;
					case MMC_TIMING_UHS_SDR104:
						mshci_hi_smaple_drv_init(hi_host->pdev->id, 2);
						mshci_hi_clk_div_init(hi_host, 2, UHS_SDR104_MAX_DTR);
						break;
					default:
						break;
					}
			} else {
					mshci_hi_clk_enable(hi_host);	
				}
		break;	
	}
}

static struct mshci_ops mshci_hi_ops = {
	/* return: 0 -- present; 1 -- not present */
	.get_present_status = mshci_hi_get_present_status,
	.start_signal_voltage_switch
						= mshci_hi_start_signal_voltage_switch,
	.tuning_find_condition = mshci_hi_tuning_find_condition,
	.tuning_set_current_state = mshci_hi_tuning_set_current_state,
	.set_ios = mshci_hi_set_ios,
	.enable_clock = mshci_hi_enable_off_clock,
};

static void mshci_hi_notify_change(struct platform_device *dev, int state)
{
	unsigned long flags;
	struct mshci_host *host = platform_get_drvdata(dev);
	if (!host) {
		return;
	}
	spin_lock_irqsave(&host->lock, flags);
	if (state) {
		host->flags &= ~MSHCI_DEVICE_DEAD;
	} else {
		host->flags |= MSHCI_DEVICE_DEAD;
	}
	spin_unlock_irqrestore(&host->lock, flags);
	tasklet_schedule(&host->card_tasklet);
}

static int mshci_hi_nv_init(void)
{
	int support;
	
    support = bsp_nvm_read(MMC_FEATURE_NV, (u8*)&g_mmc_nv, sizeof(g_mmc_nv));
	
	return support;
	
}

#if (FEATURE_ON == MBB_FAST_ON_OFF)
int card_detect_onoff_event(struct notifier_block *nb,
                                            unsigned long val, void *data)
{
    unsigned int ret = 0;
    struct balong_mmc_platform_data *plat = NULL;
    struct mshci_host *ms_host = NULL;
    struct platform_device *pdev = NULL;
    printk(KERN_ERR "%s %ld \n", __func__, __LINE__);
    pdev = &mmc0_pltfm_device;
    plat = pdev->dev.platform_data;
    if (!plat) 
	    {
		    dev_err(&pdev->dev, "%s platform data not available\n", __func__);
		    return -ENOENT;
	    }
	
    ms_host = platform_get_drvdata(pdev);
	
    pr_info("%s +++card_detect_onoff_event+++ %ld \n", __func__, val);
    printk(KERN_ERR "%s +++card_detect_onoff_event+++ %ld \n", __func__, val);
	  switch (val)
    {
     
        case FASTONOFF_MODE_SET:
            
	           if (plat->cd_gpio) 
	              {
		       gpio_int_mask_set(plat->cd_gpio);
		       free_irq(gpio_to_irq(plat->cd_gpio), ms_host);
		       gpio_free(plat->cd_gpio);
	             }
    	     
            break;

        case FASTONOFF_MODE_CLR:
            
           ret = gpio_request_one(plat->cd_gpio, GPIOF_IN, "DETECT_SD");
	        if (ret) 
			    {
			        pr_err("Request sd detected gpio:%d fail\n", plat->cd_gpio);
			        return ret;
		        }
		    gpio_int_mask_set(plat->cd_gpio);
		    ret = gpio_get_value(plat->cd_gpio);
		    if (ret) 
		        {
                    usb_notify_syswatch(DEVICE_ID_SD, SD_REMOVE);
			        gpio_int_trigger_set(plat->cd_gpio, IRQ_TYPE_LEVEL_LOW);
		        } 
			else 
			    {
                    usb_notify_syswatch(DEVICE_ID_SD, SD_ATTATCH);
			        gpio_int_trigger_set(plat->cd_gpio, IRQ_TYPE_LEVEL_HIGH);
		        }
		    gpio_set_function(plat->cd_gpio, GPIO_INTERRUPT);
		    ret = request_irq(gpio_to_irq(plat->cd_gpio), mshci_hi_card_detect_gpio,
				 IRQF_SHARED, "sd detected gpio", ms_host);
		    if (ret) 
			    {
			        pr_err("Request sd detected gpio irq: %d fail\n", plat->cd_gpio);
			        gpio_free(plat->cd_gpio);
			        return ret;
		        }
		    gpio_int_state_clear(plat->cd_gpio);
		    gpio_int_unmask_set(plat->cd_gpio);
	        break;
        default:
            break;
    }

	return ret ;
}
#endif/*CONFIG_MBB_FAST_ON_OFF*/

static int __devinit hi_mci_probe(struct platform_device *pdev)
{
	int ret = 0;
	int irq;
	void __iomem		*ioaddr;	
	struct mshci_host *ms_host = NULL;
	struct himci_host *hi_host = NULL;
	struct resource *memres = NULL;
	struct balong_mmc_platform_data *plat = NULL;
	
	/* must have platform data */


	plat = pdev->dev.platform_data;
	if (!plat) {
		dev_err(&pdev->dev, "%s platform data not available\n", __func__);
		return -ENOENT;
	}

	irq = platform_get_irq(pdev, 0);
	memres = platform_get_resource(pdev, IORESOURCE_MEM, 0);

	if (!irq || !memres) {
		dev_err(&pdev->dev, "%s get resource error\n", __func__);
		return -ENOENT;
	}
	ioaddr = ioremap_nocache(memres->start, resource_size(memres));
	
	if (!ioaddr) {
		dev_err(&pdev->dev, "%s io remap failed\n", __func__);
		return -ENXIO;
	}

	ms_host = mshci_alloc_host(&pdev->dev, sizeof(struct himci_host));
	if (IS_ERR(ms_host)) {
		dev_err(&pdev->dev, "%s alloc mshci failed\n", __func__);
		return PTR_ERR(ms_host);

	}
	platform_set_drvdata(pdev, ms_host);	
	hi_host = mshci_priv(ms_host);
	hi_host->ms_host = ms_host;
	hi_host->pdev = pdev;
	hi_host->dev = &pdev->dev;
	hi_host->plat = plat;
	hi_host->clk_value = plat->cclk_value;
	ms_host->ioaddr = ioaddr;
	ms_host->hw_name = "hi_mci";
	ms_host->ops = &mshci_hi_ops;
	ms_host->quirks = 0;
	ms_host->irq = irq;
	ms_host->max_clk = plat->cclk_value;
	/* Setup quirks for the controller */
	
	ms_host->quirks |= plat->quirks;
	ms_host->mmc->caps |= plat->caps;
	ms_host->mmc->caps2 |= plat->caps2;
	ms_host->mmc->ocr_avail |= plat->ocr_mask;
	
	ms_host->pclk = hi_host->pclk;
	/*defualt statust: clock disable; io vcc 1.8v*/
	ms_host->clock_gate = 1;
	hi_host->signal_vcc_1_8v = 1;

#if defined (BSP_CONFIG_BOARD_WINGLE)
    sdio_current_set();
#endif

	/*get sd/sdio/emmc clk*/
	ret = mshci_hi_get_clk(hi_host);
	if (ret) {
		ERROR(hi_host, "get clks fail \n");
		goto err_get_clks;
	}
	ret = mshci_hi_get_vcc(hi_host);
	if (ret) {
		ERROR(hi_host, "get vccs fail \n");
		goto err_get_vccs;
	}
	
	ret = mshci_hi_sdctmos_vcc_power_up(hi_host);
	if (ret) {
		ERROR(hi_host, "enable sdctmos fail \n");
		goto err_sd_mtcmos;
	}
	/*
	*data3 detect should enable ld10 first, 
	*but not for gpio card detect
	*/
	if (!(hi_host->ms_host->quirks 
		& MSHCI_QUIRK_EXTERNAL_CARD_DETECTION)) {
		ret = mshci_hi_sd_vcc_power_up(hi_host);
		if (ret) {
			ERROR(hi_host, "enable sdctmos fail \n");
			goto err_sd_vcc;
		}	
	}


	ret = mshci_hi_clk_enable(hi_host);
	if (ret) {
		ERROR(hi_host, "enable clks fail \n");
		goto err_enale_clks;
	}

	if (ms_host->quirks & MSHCI_QUIRK_WLAN_DETECTION) {
		if (plat->ext_cd_init) {
			plat->ext_cd_init(mshci_hi_notify_change);
			dev_dbg(&pdev->dev, "add a notify change\n");
		}
	}
	/*sd card*/
	if (!(ms_host->mmc->caps & MMC_CAP_SDIO_IRQ)) {
		
		mmc_set_host(ms_host->mmc);
		
		if (ms_host->quirks & MSHCI_QUIRK_EXTERNAL_CARD_DETECTION) {
			ret = gpio_request_one(plat->cd_gpio, GPIOF_IN, "DETECT_SD");
			if (ret) {
				ERROR(hi_host, "Request sd detected gpio:%d fail\n", plat->cd_gpio);
				goto no_card_detect_pin;
			}
			gpio_int_mask_set(plat->cd_gpio);
			
			ret = gpio_get_value(plat->cd_gpio);
			if (ret) {
				gpio_int_trigger_set(plat->cd_gpio, IRQ_TYPE_LEVEL_LOW);
			} else {
				gpio_int_trigger_set(plat->cd_gpio, IRQ_TYPE_LEVEL_HIGH);
			}
			gpio_set_function(plat->cd_gpio, GPIO_INTERRUPT);
			ret = request_irq(gpio_to_irq(plat->cd_gpio), mshci_hi_card_detect_gpio,
					 IRQF_SHARED, "sd detected gpio", ms_host);
			if (ret) {
				ERROR(hi_host, "Request sd detected gpio irq: %d fail\n", plat->cd_gpio);
				goto no_card_detect_irq;
			}
			gpio_int_state_clear(plat->cd_gpio);
			gpio_int_unmask_set(plat->cd_gpio);
#if (FEATURE_ON == MBB_USB_SD)
            usb_notify_syswatch(DEVICE_ID_SD, SD_REMOVE);
#endif
		} else {
#if(FEATURE_ON == MBB_FEATURE_SD_POLLING) 
        ms_host->mmc->caps |= MMC_CAP_NEEDS_POLL;   
#endif

#if (FEATURE_ON == MBB_USB_SD)
            usb_notify_syswatch(DEVICE_ID_SD, SD_REMOVE);
#endif
		}
    }

#if(FEATURE_ON == MBB_FEATURE_SD_POLLING)  /*SD卡轮询检测*/
    else
    {
#if (FEATURE_ON == MBB_WIFI)
        if (1 == pdev->id) /* 0:mmc0; 1:mmc1 */
        {
            mmc_set_host(ms_host->mmc);
            ms_host->mmc->caps |= MMC_CAP_NEEDS_POLL;
        }
#else
        mmc_set_host(ms_host->mmc);
        ms_host->mmc->caps |= MMC_CAP_NEEDS_POLL;
#endif
    }
#endif
	ret = mshci_add_host(ms_host);
	if (ret) {
		ERROR(hi_host, "Hi DWC mmc%d probe failed\n", pdev->id);
		goto err_add_host;
	}
	printk("Hi DWC mmc probe ok\n");
#if (FEATURE_ON == MBB_FAST_ON_OFF)
    printk(KERN_ERR "register g_fasi_on_off_notifier_list\n");
    ret = blocking_notifier_chain_register(&g_fast_on_off_notifier_list,&card_detect_block);
    if ( ret < 0 )
        {
            pr_err("register g_fast_on_off_notifier fail \n");
            printk(KERN_ERR "register g_fast_on_off_notifier fail \n");
        }
#endif/*CONFIG_MBB_FAST_ON_OFF*/

	ret = device_create_file(&hi_host->pdev->dev, &dev_attr_debug_state);
	
	printk("Hi DWC mmc%d probe ok\n", pdev->id);

	return ret;
	
err_add_host:
	free_irq(gpio_to_irq(hi_host->plat->cd_gpio), ms_host);
no_card_detect_irq:
	gpio_free(plat->cd_gpio);
no_card_detect_pin:
	mshci_hi_clk_disable(hi_host);
err_enale_clks:
	mshci_hi_sdctmos_vcc_power_off(hi_host);
err_sd_vcc:

if (!(hi_host->ms_host->quirks 
	& MSHCI_QUIRK_EXTERNAL_CARD_DETECTION)) {
	mshci_hi_vcc_power_off(hi_host);
}
err_sd_mtcmos:	
	mshci_hi_put_vcc(hi_host);
err_get_vccs:	
	mshci_hi_put_clk(hi_host);
err_get_clks:
	iounmap(ms_host->ioaddr);
	ms_host->ioaddr = NULL;	
	return ret;
}

static int __devexit hi_mci_remove(struct platform_device *pdev)
{
	struct mshci_host *ms_host = NULL;
	struct himci_host *hi_host = NULL;
	struct balong_mmc_platform_data *plat = NULL;
	int ret = 0;
	
	WARN_ON(!pdev);
	if (!pdev) {
		return -ENODEV;
	}
	ms_host = platform_get_drvdata(pdev);
	if (!ms_host) {
		return -ENODEV;
	}
	hi_host = mshci_priv(ms_host);
	plat = pdev->dev.platform_data;

	if (plat->cd_gpio) {
		gpio_int_mask_set(plat->cd_gpio);
		free_irq(gpio_to_irq(plat->cd_gpio), ms_host);
		gpio_free(plat->cd_gpio);
	}
	mshci_remove_host(ms_host, 1);

	iounmap(ms_host->ioaddr);
	
	ms_host->ioaddr = NULL;

	mshci_hi_sdctmos_vcc_power_off(hi_host);
	
	mshci_hi_put_clk(hi_host);
	
	mshci_hi_put_vcc(hi_host);
	
#if (FEATURE_ON == MBB_COMMON)
    device_remove_file(&hi_host->pdev->dev, &dev_attr_debug_state);
#else
    if (ms_host->quirks & MSHCI_QUIRK_EXTERNAL_CARD_DETECTION) {
        device_remove_file(&hi_host->pdev->dev, &dev_attr_debug_state);
    }
#endif

	mshci_free_host(ms_host);
	platform_set_drvdata(pdev, NULL);

	printk("Hi DWC mmc removed \n");
	return ret;
}

#ifdef CONFIG_PM

static int  mshci_hi_vcc_power_off(struct himci_host *hi_host)
{
	int ret = 0;
	if (hi_host->vcc && hi_host->sd_vcc_power_on) {
		ret = regulator_disable(hi_host->vcc);
		if (!ret) {
			g_hi_mci_debug_state.sd_vcc_power_off_times++;
			hi_host->sd_vcc_power_on = 0;		
		} else {
			goto out;
		}
	}
	
	if (!hi_host->signal_vcc_1_8v) {
		ret = mshci_hi_io_vcc_to_1_8v(hi_host);
	}
out:
	return ret;
}

static int hi_mci_suspend(struct device *dev)
{
	struct platform_device *pdev =to_platform_device(dev);
	struct mshci_host *ms_host = NULL;
	struct himci_host *hi_host = NULL;

	int ret = 0;

	ms_host = platform_get_drvdata(pdev);
	if (!ms_host) {
		return -ENODEV;
	}
	hi_host = mshci_priv(ms_host);
	
	if (ms_host->mmc->power_notify_type != MMC_HOST_PW_NOTIFY_SHORT) {
		ERROR(hi_host, "pm notify not exec, suspend host failed\n");
		return -ENODEV;
	}

	ret = mshci_suspend_host(ms_host);
	if (ret) {
		goto out;
	}
	ms_host->clock = 0;
	if (0 == hi_host->pdev->id) {	
		mmc0_to_gpio_mux();
		g_hi_mci_debug_state.mmc_to_gpio++;
	}
	ret = mshci_hi_sdctmos_vcc_power_off(hi_host);
	if (ret) {
		goto out;
	}
	if (!(hi_host->ms_host->quirks 
		& MSHCI_QUIRK_EXTERNAL_CARD_DETECTION)) {
		ret = mshci_hi_vcc_power_off(hi_host);
	}	
out:
	return ret;
}

static int hi_mci_resume(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct mshci_host *ms_host = NULL;
	struct himci_host *hi_host = NULL;
	int ret = 0;
	

	ms_host = platform_get_drvdata(pdev);
	if (!ms_host) {
		return -ENODEV;
	}
	hi_host = mshci_priv(ms_host);
	
	ret = mshci_hi_sdctmos_vcc_power_up(hi_host);
	if (ret) {
		goto out;
	}
	/*data3 detect should enable ld10 first*/
	if (!(hi_host->ms_host->quirks 
		& MSHCI_QUIRK_EXTERNAL_CARD_DETECTION)) {
		ret = mshci_hi_sd_vcc_power_up(hi_host);
		if (ret) {
			goto out;
		}
	}

	if (0 == hi_host->pdev->id) {	
		gpio_to_mmc0_mux();
		g_hi_mci_debug_state.gpio_to_mmc++;
	}
	ret = mshci_resume_host(ms_host);
out:
	return ret;
}

static int hi_mci_prepare(struct device *dev)
{
	return 0;
}
static void hi_mci_complete(struct device *dev)
{
}
int mshci_hi_mci_shutdown(int arg)
{
	int ret;
	struct mmc_host *host = NULL;
	host = mmc_get_host();
	if (!host) {
		return -ENOMEDIUM;
	}
	ret = hi_mci_suspend(host->parent);
	if (ret) {
		dev_err(host->parent, "shut down failed\n");
	}
	return ret;
}

int mshci_hi_mci_resume(int arg)
{
	int ret;
	struct mmc_host *host = NULL;
	host = mmc_get_host();

	if (!host) {
		return -ENOMEDIUM;
	}
	ret = hi_mci_resume(host->parent);
	if (ret) {
		dev_err(host->parent, "resume failed\n");
	}
	return ret;
}

#else
static int  mshci_hi_vcc_power_off(struct himci_host *hi_host)
{
	return 0;
}
#define hi_mci_suspend  NULL
#define hi_mci_resume   NULL
#define hi_mci_prepare   NULL
#define hi_mci_complete   NULL
#define mshci_hi_mci_shutdown   NULL
#define mshci_hi_mci_resume   NULL
#endif

static const struct dev_pm_ops hi_mci_pm_ops ={
	.suspend		= hi_mci_suspend,
	.resume		= hi_mci_resume,
	.prepare 		= hi_mci_prepare,
	.complete 	= hi_mci_complete
};


static struct platform_driver hi_mci_driver = {
	.probe		= hi_mci_probe,
	.remove		= hi_mci_remove,
	.driver		= {
		.name	= "hi_mci",
		.pm 		= &hi_mci_pm_ops
	},
};

#if (FEATURE_ON == MBB_COMMON)
/*****************************************************************************
 函数名称  : static unsigned mshci_get_wifi_gpio(void)
 功能描述  : 根据硬件版本号获取wifi使能的GPIO
 输入参数  : NA
 输出参数  : NA
 返 回 值  : wifi使能GPIO
*****************************************************************************/
static unsigned mshci_get_wifi_gpio(void)
{
    
    return WIFI_ACTIVE_GPIO;
}

/*****************************************************************************
 函数名称  : static void mshci_wifi_GPIO_init(void)
 功能描述  : 初始化wifi
 输入参数  : NA
 输出参数  : NA
 返 回 值  : 无
*****************************************************************************/
static void mshci_wifi_GPIO_init(void)
{   
    unsigned wifi_gpio_addr = mshci_get_wifi_gpio(); /* for wifi CHIP-EN */

    /* GPIO init */
    if(!gpio_is_valid(wifi_gpio_addr))
    {
        printk("%s:fail to get wifi gpio!\n",__FUNCTION__);
        return;
    }
    
    if(gpio_request(wifi_gpio_addr,"hi-mci-wifi"))
    {
        printk("%s:gpio_request wifi_gpio_addr fail!\n",__FUNCTION__);
        return;
    }
    
    return;
}

/*****************************************************************************
 函数名称  : static void mshci_wifi_power_on(void)
 功能描述  : wifi 电源开启
 输入参数  : NA
 输出参数  : NA
 返 回 值  : 无
*****************************************************************************/
int mshci_wifi_power_on(void)
{   
    int ret = -1;
    unsigned wifi_gpio_addr = 0;
    printk("%s:enter!\n",__FUNCTION__);
    /* 3.3v电源 */
    /* WIFI_PA-VCC 为平台提供，不可修改 */
    if (NULL != g_wifi_vcc)
    {
        printk("g_wifi_vcc already on!\n");
        return ret;
    }

    
    g_wifi_vcc = regulator_get(NULL, "WIFI_PA-VCC");
    if (NULL == g_wifi_vcc)
    {
        printk("regulator_get WIFI_PA_VCC error!\n");
        return ret;
    }

    ret = regulator_set_voltage(g_wifi_vcc, 3300000, 3300000);
    if (0 != ret)
    {
        printk("regulator_set_voltage WIFI_PA_VCC error!\n");
        return ret;
    }

    ret = regulator_enable(g_wifi_vcc);
    if (0 != ret)
    {
        printk("regulator_enable WIFI_PA_VCC error!\n");  
        return ret;
    }

    /* 1.8v电源 */
    wifi_gpio_addr = mshci_get_wifi_gpio();
    ret = gpio_is_valid(wifi_gpio_addr);
    if (0 == ret)
    {
        printk("%s:fail to get wifi gpio!",__FUNCTION__);
        return -1;
    }
    else
    {
        ret = 0; /* 0: no error */
    }
    
    gpio_direction_output(wifi_gpio_addr, 1); 
    
    return ret;
}
EXPORT_SYMBOL(mshci_wifi_power_on);

/*****************************************************************************
 函数名称  : static void mshci_wifi_power_off(void)
 功能描述  : wifi 电源关闭
 输入参数  : NA
 输出参数  : NA
 返 回 值  : 无
*****************************************************************************/
int mshci_wifi_power_off(void)
{
    int ret = -1;
    unsigned wifi_gpio_addr = 0;
    printk("%s:enter!\n",__FUNCTION__);
    if (NULL == g_wifi_vcc)
    {
        printk("g_wifi_vcc already off!\n");
        return ret;
    }
    
    /* 3.3v电源 */
    ret = regulator_disable(g_wifi_vcc);
    if (0 != ret)
    {
        printk("regulator_disable WIFI_PA_VCC error!\n");
        return ret;
    }
    
    regulator_put(g_wifi_vcc);
    
    g_wifi_vcc = NULL;
 
    /* 1.8v电源 */
    wifi_gpio_addr = mshci_get_wifi_gpio();

    ret = gpio_is_valid(wifi_gpio_addr);
	if (0 == ret)
    {
        printk("%s:fail to get wifi gpio!",__FUNCTION__);
        return -1;
    }
    else
    {
         ret = 0; /* 0: no error */
    }
    
    gpio_direction_output(wifi_gpio_addr, 0); 
    
    return ret;
}
EXPORT_SYMBOL(mshci_wifi_power_off);
#endif
 
static int __init hi_mci_init(void)
{
	int ret = 0;

	ret = platform_driver_register(&hi_mci_driver);
	if (ret) {
		goto out;
	}
	if (mshci_hi_nv_init() || !g_mmc_nv.support) {
		printk("Hi DWC mmc not init\n");
		return ret;
	}
#if (FEATURE_ON == MBB_WIFI)
    hi_syscrg_mmc_mmc0_unreset();
    ret = platform_device_register(&mmc0_pltfm_device);
#else
#if 0
	hi_syscrg_mmc_mmc0_unreset();
	ret = platform_device_register(&mmc0_pltfm_device);
#endif
#endif	
    hi_syscrg_mmc_mmc1_unreset();
	ret |= platform_device_register(&mmc1_pltfm_device);
#if (FEATURE_ON == MBB_WIFI)
#if ((rtl8192 == MBB_WIFI_CHIP1) || (rtl8192 == MBB_WIFI_CHIP2))  
    {
        mshci_wifi_GPIO_init();
        ret = mshci_wifi_power_on();
    }
#endif
#endif

      out:
	return ret;
}

static void __exit hi_mci_exit(void)
{
	platform_driver_unregister(&hi_mci_driver);
	platform_device_unregister(&mmc0_pltfm_device);
	platform_device_unregister(&mmc1_pltfm_device);
}

module_init(hi_mci_init);
module_exit(hi_mci_exit);

#ifdef MODULE
MODULE_AUTHOR("Hisilicon Driver Group");
MODULE_DESCRIPTION("MMC/SD driver for the Hisilicon MMC/SD Host Controller");
MODULE_LICENSE("GPL");
#endif
/*lint -restore*/
