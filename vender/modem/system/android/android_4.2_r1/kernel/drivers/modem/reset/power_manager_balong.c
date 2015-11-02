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
/*lint --e{537} */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/platform_device.h>
#include "reset_balong.h"

/*lint --e{26, 665, 745} */

struct balong_power_plat_data {
    u32 modem_state;
};

enum modem_state_index {
    MODEM_NOT_READY = 0,
    MODEM_READY,
    MODEM_INVALID,
};

static struct balong_power_plat_data* balong_driver_plat_data = NULL;
static const char* const modem_state_str[] = {
    "MODEM_STATE_OFF\n",
    "MODEM_STATE_READY\n",
    "MODEM_STATE_INVALID\n",
};
static const char* const modem_switch_str[] = {
    "1",  /* MODEM_UP */
    "2",  /* MODEM_DOWN */
    "10",  /* MODEM_RESET */
    "-1", /* MODEM_INVALID_OPS */
};

/*To make modem poweroff called only once when there are two rilds.*/
static int modem_power_off_flag = 0;

int modem_state_get(void)
{
	if (!balong_driver_plat_data)
	{
		printk("Balong_power %s:%d not init.\n", __FUNCTION__, __LINE__);
		return -EINVAL;
	}
	return (int)balong_driver_plat_data->modem_state;
}

int modem_state_set(unsigned int state)
{
    if (!balong_driver_plat_data){
        printk("Balong_power %s:%d not init.\n", __FUNCTION__, __LINE__);
        return -EINVAL;
    }
    if (state >= MODEM_INVALID){
        printk("Balong_power %s:%d invalid state 0x%x.\n", __FUNCTION__, __LINE__, state);
        return -EINVAL;
    }

    balong_driver_plat_data->modem_state = state;

    return 0;
}

static ssize_t balong_power_get(struct device *dev, struct device_attribute *attr, char *buf)
{
    int state;
    ssize_t len;
    state = modem_state_get();
    if (state < 0) {
        printk("Balong_power %s:%d not init.\n", __FUNCTION__, __LINE__);
        return 0;
    }
    if (balong_driver_plat_data->modem_state >= MODEM_INVALID){
        printk("Balong_power : %s:%d Invalid state 0x%x now is set.\n", __FUNCTION__, __LINE__, state);
        return 0;
    }

    len = snprintf(buf, strlen(modem_state_str[balong_driver_plat_data->modem_state]) + 1,
        "%s\n", modem_state_str[balong_driver_plat_data->modem_state]);

    return len;
}

static ssize_t modem_reset_set(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	int state;
	char* endp;
	dev_info(dev, "Power set to %s\n", buf);
	state = simple_strtol(buf, &endp, 10); /*10 means read as dec*/
	pr_err("endp = 0x%x\n", *(int*)endp);
	pr_err("count = %d\n", count);
	if (*buf == '\0' || *buf == *endp)/* return 0 means match failed */
	{
		return count;
	}
	if (state == BALONG_MODEM_RESET)
	{
		bsp_modem_reset();
		pr_err("modem reset %d\n", BALONG_MODEM_RESET);
		return count;
	} 
	else if(state == BALONG_MODEM_OFF)
	{
		/*To make modem poweroff called only once when there are two rilds.*/
		if(modem_power_off_flag )
		{
			pr_err("Balong_power: modem power off has been called! \n");
			return count;
		}
		bsp_modem_power_off();
		pr_err("modem power off %d\n", BALONG_MODEM_OFF);
		modem_power_off_flag = 1;
		return count;
	}
	else if(state == BALONG_MODEM_ON) /* TODO: 是否需要上电，根HIFI什么关系 */
	{
		bsp_modem_reset();
		pr_err("modem power on %d\n", BALONG_MODEM_ON);
	}
	else
	{
		pr_err("Balong_power : invalid code to balong power !!!!\n");
		return count;
	}
	return count;

}

static DEVICE_ATTR(state, S_IRUGO , balong_power_get, NULL);
static DEVICE_ATTR(modem_switch, S_IRUGO | S_IWUSR | S_IWGRP, NULL, modem_reset_set);

static int __init bsp_power_probe(struct platform_device *pdev)
{
    int ret = 0;

    balong_driver_plat_data = pdev->dev.platform_data;

    ret = device_create_file(&(pdev->dev), &dev_attr_state);
    if (ret)
    {
        printk("fail to creat modem boot sysfs\n");
        return ret;
    }
     ret = device_create_file(&(pdev->dev), &dev_attr_modem_switch);
    if (ret)
    {
        printk("fail to creat dandufuwei_state boot sysfs\n");
        return ret;
    }

    return ret;
}

static struct balong_power_plat_data  balong_power_plat_data = {
    .modem_state = MODEM_NOT_READY,
};

static struct platform_device balong_power_device = {
    .name = "balong_power",
    .id = -1,
    .dev = {
        .platform_data = &balong_power_plat_data,
    },
};

static struct platform_driver balong_power_drv = {
    .probe      = bsp_power_probe,
    .driver     = {
        .name     = "balong_power",
        .owner    = THIS_MODULE,
    },
};

static int bsp_modem_power_init(void);
static void bsp_modem_power_exit(void);

static int __init bsp_modem_power_init(void)
{
    int ret = 0;

    ret = platform_device_register(&balong_power_device);
    if(ret)
    {
        printk("register hisi balong boot device failed. \r\n");
        return ret;
    }

    ret = platform_driver_register(&balong_power_drv);
    if(ret)
    {
        printk("register his_modem boot driver failed. \r\n");
        platform_device_unregister(&balong_power_device);
    }

    return ret;
}

static void __exit bsp_modem_power_exit(void)
{
    platform_driver_unregister(&balong_power_drv);
    platform_device_unregister(&balong_power_device);
}

module_init(bsp_modem_power_init);
module_exit(bsp_modem_power_exit);

MODULE_AUTHOR("y00184236@huawei.com");
MODULE_DESCRIPTION("HIS Balong Modem Power");
MODULE_LICENSE("GPL");

