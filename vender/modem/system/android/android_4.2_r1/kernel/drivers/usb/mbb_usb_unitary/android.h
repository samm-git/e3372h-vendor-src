/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2013-2015. All rights reserved.
 *
 * mobile@huawei.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#ifndef __ANDROID_H__
#define __ANDROID_H__
#include "usb_debug.h"
#include <linux/usb/composite.h>
#include <linux/pm_qos.h>

struct android_usb_function
{
    USB_CHAR* name;
    USB_VOID* config;
    USB_INT   property;/*´®¿Úprotocol*/
    struct device* dev;
    USB_CHAR* dev_name;
    struct device_attribute** attributes;

    struct android_dev* android_dev;

    /* for android_dev.enabled_functions */
    struct list_head enabled_list;

    /* Optional: initialization during gadget bind */
    USB_INT (*init)(struct android_usb_function*, struct usb_composite_dev*);
    /* Optional: cleanup during gadget unbind */
    USB_VOID (*cleanup)(struct android_usb_function*);
    /* Optional: called when the function is added the list of
     *    enabled functions */
    USB_VOID (*enable)(struct android_usb_function*);
    /* Optional: called when it is removed */
    USB_VOID (*disable)(struct android_usb_function*);

    USB_INT (*bind_config)(struct android_usb_function*,
                           struct usb_configuration*);

    /* Optional: called when the configuration is removed */
    USB_VOID (*unbind_config)(struct android_usb_function*,
                              struct usb_configuration*);
    /* Optional: handle ctrl requests before the device is configured */
    USB_INT (*ctrlrequest)(struct android_usb_function*,
                           struct usb_composite_dev*,
                           const struct usb_ctrlrequest*);
};

struct android_configuration
{
    struct usb_configuration usb_config;

    /* A list of the functions will enabled by this config */
    struct list_head enabled_functions;

    /* A list node inside the struct android_dev.configs list */
    struct list_head list_item;
};

/**
* struct android_dev - represents android USB gadget device
* @name: device name.
* @functions: an array of all the supported USB function
*    drivers that this gadget support but not necessarily
*    added to one of the gadget configurations.
* @cdev: The internal composite device. Android gadget device
*    is a composite device, such that it can support configurations
*    with more than one function driver.
* @dev: The kernel device that represents this android device.
* @enabled: True if the android gadget is enabled, means all
*    the configurations were set and all function drivers were
*    bind and ready for USB enumeration.
* @disable_depth: Number of times the device was disabled, after
*    symmetrical number of enables the device willl be enabled.
*    Used for controlling ADB userspace disable/enable requests.
* @mutex: Internal mutex for protecting device member fields.
* @pdata: Platform data fetched from the kernel device platfrom data.
* @connected: True if got connect notification from the gadget UDC.
*    False if got disconnect notification from the gadget UDC.
* @sw_connected: Equal to 'connected' only after the connect
*    notification was handled by the android gadget work function.
* @suspended: True if got suspend notification from the gadget UDC.
*    False if got resume notification from the gadget UDC.
* @sw_suspended: Equal to 'suspended' only after the susped
*    notification was handled by the android gadget work function.
* @pm_qos: An attribute string that can be set by user space in order to
*    determine pm_qos policy. Set to 'high' for always demand pm_qos
*    when USB bus is connected and resumed. Set to 'low' for disable
*    any setting of pm_qos by this driver. Default = 'high'.
* @work: workqueue used for handling notifications from the gadget UDC.
* @configs: List of configurations currently configured into the device.
*    The android gadget supports more than one configuration. The host
*    may choose one configuration from the suggested.
* @configs_num: Number of configurations currently configured and existing
*    in the android_configs list.
* @list_item: This driver supports more than one android gadget device (for
*    example in order to support multiple USB cores), therefore this is
*    a item in a linked list of android devices.
*/
struct android_dev
{
    const char* name;
    struct android_usb_function** functions;
    struct usb_composite_dev* cdev;
    struct device* dev;

    void (*setup_complete)(struct usb_ep* ep,
                           struct usb_request* req);

    bool enabled;
    int irq_ctl_port;
    int disable_depth;
    struct mutex mutex;
    struct android_usb_platform_data* pdata;

    bool connected;
    bool sw_connected;
    bool suspended;
    bool sw_suspended;
    char pm_qos[5];
    struct pm_qos_request pm_qos_req_dma;
    struct work_struct work;
    USB_CHAR ffs_aliases[USB_NUM_256];

    /* A list of struct android_configuration */
    struct list_head android_configs;
    int configs_num;
};

#endif
