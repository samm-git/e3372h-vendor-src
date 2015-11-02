/*
 * arch/arm/mach-hi6620/dev_smartstar_coul.c
 *
 * Copyright (C) 2013 Hisilicon Co. Ltd.
 *
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#include <linux/platform_device.h>
#include <linux/io.h>
#if 0
#include <mach/system.h>
#include <mach/platform.h>
#include <mach/irqs.h>
#include <mach/io.h>
#include <mach/gpio.h>
#endif
//#include <hsad/config_mgr.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <dev_smartstar_coul.h>

#include <bsp_pmu.h>

#define SMARTSTAR_COUL_ENABLE "gas_gauge/ss_coul_enable"

static struct resource  smartstar_coul_resource[] = {

    [0] = {
            .name  = "cl_int",
            .start = PMU_INT_COUL_INT,
            .end   = PMU_INT_COUL_INT,
            .flags = IORESOURCE_IRQ,
    },
    [1] = {
            .name  = "cl_out_D_int",
            .start = PMU_INT_COUL_OUT,
            .end   = PMU_INT_COUL_OUT,
            .flags = IORESOURCE_IRQ,
    },
    [2] = {
            .name  = "cl_in_D_int",
            .start = PMU_INT_COUL_IN,
            .end   = PMU_INT_COUL_IN,
            .flags = IORESOURCE_IRQ,
    },
    [3] = {
            .name  = "chg_timer_D_int",
            .start = PMU_INT_CHG_TIMER,
            .end   = PMU_INT_CHG_TIMER,
            .flags = IORESOURCE_IRQ,
    },
    [4] = {
            .name  = "load_timer_D_int",
            .start = PMU_INT_LOAD_TIMER,
            .end   = PMU_INT_LOAD_TIMER,
            .flags = IORESOURCE_IRQ,
    },
    [5] = {
            .name  = "vbat_int",
            .start = PMU_INT_VBAT_INT,
            .end   = PMU_INT_VBAT_INT,
            .flags = IORESOURCE_IRQ,
    },

};


static struct platform_device smartstar_coul_device = {
    .name                   = "hisi_smartstar_coul",
    .id                             = -1,
    .resource                       = (struct resource	*)&smartstar_coul_resource,
    .num_resources  = ARRAY_SIZE(smartstar_coul_resource),
};


static int __init smartstar_coul_dev_init(void)
{
    int ret = 0, enable = 0;
#if 0
    if (get_hw_config_int(SMARTSTAR_COUL_ENABLE, &enable, NULL )){
        if (!enable){
            printk(KERN_ERR "smartstar coul disabled !\n");
            return 0;
        }
    }
#endif

    ret = platform_device_register(&smartstar_coul_device);
    return ret;
}

arch_initcall(smartstar_coul_dev_init);
