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

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/fb.h>
#include <linux/leds.h>
#include <linux/err.h>
#include <linux/pwm.h>
#include <linux/leds_pwm.h>
#include <linux/slab.h>

struct led_pwm_data {
    struct led_classdev cdev;
    struct pwm_device   *pwm;
    unsigned int        active_low;
    unsigned int        period;
};

static void led_pwm_set(struct led_classdev *led_cdev,
    enum led_brightness brightness)
{
    struct led_pwm_data *led_dat =
        container_of(led_cdev, struct led_pwm_data, cdev);
    unsigned int max = led_dat->cdev.max_brightness;
    unsigned int period =  led_dat->period;

    if (brightness == 0) {
        pwm_config(led_dat->pwm, 0, period);
        pwm_disable(led_dat->pwm);
    } else {
        pwm_config(led_dat->pwm, brightness * period / max, period);
        pwm_enable(led_dat->pwm);
    }
}

static int balong_led_pwm_probe(struct platform_device *pdev)
{
    struct led_pwm_platform_data *pdata = pdev->dev.platform_data;
    struct led_pwm *cur_led;
    struct led_pwm_data *leds_data, *led_dat;
    int i, ret = 0;

    if (!pdata)
        return -EBUSY;

    leds_data = kzalloc(sizeof(struct led_pwm_data) * pdata->num_leds,
                GFP_KERNEL);
    if (!leds_data)
        return -ENOMEM;

    for (i = 0; i < pdata->num_leds; i++) {
        cur_led = &pdata->leds[i];
        led_dat = &leds_data[i];

        led_dat->pwm = pwm_request(cur_led->pwm_id,
                cur_led->name);
        if (IS_ERR(led_dat->pwm)) {
            ret = PTR_ERR(led_dat->pwm);
            dev_err(&pdev->dev, "unable to request PWM %d\n",
                    cur_led->pwm_id);
            goto err;
        }

        led_dat->cdev.name = cur_led->name;
        led_dat->cdev.default_trigger = cur_led->default_trigger;
        led_dat->active_low = cur_led->active_low;
        led_dat->period = cur_led->pwm_period_ns;
        led_dat->cdev.brightness_set = led_pwm_set;
        led_dat->cdev.brightness = LED_OFF;
        led_dat->cdev.max_brightness = cur_led->max_brightness;
        led_dat->cdev.flags |= LED_CORE_SUSPENDRESUME;

        ret = led_classdev_register(&pdev->dev, &led_dat->cdev);
        if (ret < 0) {
            pwm_free(led_dat->pwm);
            goto err;
        }
    }

    platform_set_drvdata(pdev, leds_data);

    dev_err(&pdev->dev, "balong_led_pwm_probe ok!\n");

    return 0;

err:
    if (i > 0) {
        for (i = i - 1; i >= 0; i--) {
            led_classdev_unregister(&leds_data[i].cdev);
            pwm_free(leds_data[i].pwm);
        }
    }

    kfree(leds_data);

    return ret;
}

static int __devexit balong_led_pwm_remove(struct platform_device *pdev)
{
    int i;
    struct led_pwm_platform_data *pdata = pdev->dev.platform_data;
    struct led_pwm_data *leds_data;

    leds_data = platform_get_drvdata(pdev);

    for (i = 0; i < pdata->num_leds; i++) {
        led_classdev_unregister(&leds_data[i].cdev);
        pwm_free(leds_data[i].pwm);
    }

    kfree(leds_data);

    return 0;
}
static struct led_pwm balong_leds[] = {
    [0] = {
        .name       = "pwm_led0",
        .pwm_id     = 2,
        .max_brightness = 1023,
        .pwm_period_ns  = 4877,/*53333*/
    }
};
static struct led_pwm_platform_data balong_leds_pwm_data = {
    .num_leds   = ARRAY_SIZE(balong_leds),
    .leds       = balong_leds,
};
static struct platform_device balong_led_pwm_device = {
    .name       = "balong_leds_pwm",
    .dev        = {
        //.parent = &balong_device_pwm1.dev,
        .platform_data = &balong_leds_pwm_data,
    },
};
static struct platform_driver balong_led_pwm_driver = {
    .probe      = balong_led_pwm_probe,
    .remove     = __devexit_p(balong_led_pwm_remove),
    .driver     = {
        .name   = "balong_leds_pwm",
        .owner  = THIS_MODULE,
    },
};

static int __init balong_led_pwm_init(void)
{
    int ret = 0;

    ret = platform_device_register(&balong_led_pwm_device);
    if(ret)
        printk(KERN_ERR "spi0:platform_device_register err! \n");
    ret = platform_driver_register(&balong_led_pwm_driver);
    if(ret)
        printk(KERN_ERR "spi0:platform_driver_register err! \n");

    return ret;
}
module_init(balong_led_pwm_init);

static void __exit balong_led_pwm_exit(void)
{
    platform_driver_unregister(&balong_led_pwm_driver);
}
module_exit(balong_led_pwm_exit);
//module_platform_driver(balong_led_pwm_driver);

MODULE_AUTHOR("Luotao Fu <l.fu@pengutronix.de>");
MODULE_DESCRIPTION("PWM LED driver for PXA");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:leds-pwm");
