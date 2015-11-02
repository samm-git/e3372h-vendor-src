/*
 * LED Class Core
 *
 * Copyright 2005-2006 Openedhand Ltd.
 *
 * Author: Richard Purdie <rpurdie@openedhand.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
 /*******************************修改记录开始**********************************
日期           修改人          问题单号                 修改内容
********************************修改记录结束**********************************/

#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/rwsem.h>
#include <linux/leds.h>
#include "leds.h"
#include <product_config.h>

DECLARE_RWSEM(leds_list_lock);
EXPORT_SYMBOL_GPL(leds_list_lock);

LIST_HEAD(leds_list);
EXPORT_SYMBOL_GPL(leds_list);
#if ( FEATURE_ON == MBB_LED )
void led_stop_software_blink(struct led_classdev *led_cdev)
#else
static void led_stop_software_blink(struct led_classdev *led_cdev)
#endif
{
	/* deactivate previous settings */
	del_timer_sync(&led_cdev->blink_timer);
	led_cdev->blink_delay_on = 0;
	led_cdev->blink_delay_off = 0;
}

static void led_set_software_blink(struct led_classdev *led_cdev,
				   unsigned long delay_on,
				   unsigned long delay_off)
{
	int current_brightness;

	current_brightness = led_get_brightness(led_cdev);
	if (current_brightness)
		led_cdev->blink_brightness = current_brightness;
	if (!led_cdev->blink_brightness)
		led_cdev->blink_brightness = led_cdev->max_brightness;

	if (led_get_trigger_data(led_cdev) &&
	    delay_on == led_cdev->blink_delay_on &&
	    delay_off == led_cdev->blink_delay_off)
		return;

	led_stop_software_blink(led_cdev);

	led_cdev->blink_delay_on = delay_on;
	led_cdev->blink_delay_off = delay_off;
    
#if (FEATURE_ON == MBB_COMMON)
        if((!delay_off) && (!delay_on))
        {
            return;
        }
        /* never on - don't blink */
        if (!delay_on)
        {
            led_set_brightness(led_cdev, LED_OFF);
            return;
        }   

        /* never off - just set to brightness */
        if (!delay_off) {
            led_set_brightness(led_cdev, LED_OFF);
            return;
        }

        mod_timer(&led_cdev->blink_timer, jiffies + msecs_to_jiffies(delay_off));
#else
	/* never on - don't blink */
	if (!delay_on)
		return;

	/* never off - just set to brightness */
	if (!delay_off) {
		led_set_brightness(led_cdev, led_cdev->blink_brightness);
		return;
	}

	mod_timer(&led_cdev->blink_timer, jiffies + 1);
#endif
}


void led_blink_set(struct led_classdev *led_cdev,
		   unsigned long *delay_on,
		   unsigned long *delay_off)
{
	del_timer_sync(&led_cdev->blink_timer);

	if (led_cdev->blink_set &&
	    !led_cdev->blink_set(led_cdev, delay_on, delay_off))
		return;

	/* blink with 1 Hz as default if nothing specified */
	/* 如果在此初始化 delay_on/off 为 500 , 会在绑定默认 timer trigger 
       时在初始化后自动blink, 即使设置 brightness 默认为 0 也会 blink , 
       并且在 blink 时修改 brightness 为非 0
    */
    /* blink with 1 Hz as default if nothing specified */
    /*
    if (!*delay_on && !*delay_off)
        delay_on = *delay_off = 500;
    */

	led_set_software_blink(led_cdev, *delay_on, *delay_off);
}
EXPORT_SYMBOL(led_blink_set);

void led_brightness_set(struct led_classdev *led_cdev,
			enum led_brightness brightness)
{
	led_stop_software_blink(led_cdev);
	led_cdev->brightness_set(led_cdev, brightness);
}
EXPORT_SYMBOL(led_brightness_set);
