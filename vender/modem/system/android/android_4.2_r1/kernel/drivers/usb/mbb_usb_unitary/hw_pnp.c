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
/******************************************************************************************************
    问题单号                       时间                   修改人                        说明
******************************************************************************************************/
#include <linux/proc_fs.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>
#include <linux/usb/gadget.h>

#include <linux/list.h>
#include <linux/netlink.h>
#include <linux/usb/usb_interface_external.h>

#include "android.h"
#include "hw_pnp.h"
#include "hw_pnp_adapt.h"
#include "hw_pnp_api.h"

/*the string descriptor of device */
static char manufacturer_string[MAX_STRING_DESC_NUM];
static char product_string[MAX_STRING_DESC_NUM];
static char serial_string[MAX_STRING_DESC_NUM];
static char MSOS_descriptor_string[MAX_STRING_DESC_NUM] =
{
    USB_NUM_18, /* sizeof(mtp_os_string) */
    USB_DT_STRING,
    /* Signature field: "MSFT100" */
    'M', 0, 'S', 0, 'F', 0, 'T', 0, '1', 0, '0', 0, '0', 0,
    /* vendor code */
    MS_HUAWEI_VENDOR_CODE,
    /* padding */
    0
};

static struct usb_device_descriptor device_desc =
{
    .bLength              = sizeof(device_desc),
    .bDescriptorType      = USB_DT_DEVICE,
    .bcdUSB               = __constant_cpu_to_le16(0x0200),
    .bDeviceClass         = USB_CLASS_PER_INTERFACE,
    .idVendor             = __constant_cpu_to_le16(VENDOR_ID_HUAWEI),
    .idProduct            = __constant_cpu_to_le16(PRODUCT_ID),
#ifdef MBB_USB_UNITARY_Q
    .bcdDevice            = __constant_cpu_to_le16(USB_BCDDEVICE_QC),
#else
    .bcdDevice            = __constant_cpu_to_le16(USB_BCDDEVICE_BALONG),
#endif
    .bDeviceProtocol      = 0x00,
    .bDeviceSubClass      = 0x00,
    .bNumConfigurations   = 1,
};

static struct usb_otg_descriptor otg_descriptor =
{
    .bLength         = sizeof otg_descriptor,
    .bDescriptorType = USB_DT_OTG,
    .bmAttributes    = USB_OTG_SRP | USB_OTG_HNP,
    .bcdOTG          = __constant_cpu_to_le16(0x0200),
};

static const struct usb_descriptor_header* otg_desc[] =
{
    (struct usb_descriptor_header*)& otg_descriptor,
    NULL,
};

/* String Table */
static struct usb_string strings_dev[] =
{
    [STRING_MANUFACTURER_IDX].s = manufacturer_string,
    [STRING_PRODUCT_IDX].s = product_string,
    [STRING_SERIAL_IDX].s = serial_string,
    {
        .s = MSOS_descriptor_string,
        .id = MSOS_STRING_INDEX,/*STRING_MSOS_IDX*/
    },
    {}
};

static struct usb_gadget_strings stringtab_dev =
{
    .language = 0x0409,	/* en-us */
    .strings  = strings_dev,
};

static struct usb_gadget_strings* dev_strings[] =
{
    &stringtab_dev,
    NULL,
};

/*****************************************************************************
内部接口 begin{
*****************************************************************************/
USB_VOID pnp_probe(USB_VOID);
USB_VOID pnp_remove(USB_VOID);

USB_VOID pnp_switch_normal_work_mode(USB_VOID);
USB_VOID pnp_switch_mbim_debug_mode(USB_VOID);
USB_VOID pnp_switch_rndis_debug_mode(USB_VOID);
USB_VOID pnp_switch_rndis_project_mode(USB_VOID);

static USB_VOID pnp_free_android_config(struct android_dev* dev,
                                        struct android_configuration* conf);
USB_INT pnp_huawei_vendor_setup(struct usb_gadget* gadget,
                                const struct usb_ctrlrequest* ctrl);

pnp_dynamic_mode* pnp_dynamic_port_get_mode(USB_UCHAR port_index);
USB_VOID pnp_notify_to_switch(USB_VOID);
USB_VOID pnp_switch_func(struct work_struct* w);

USB_VOID pnp_dynamic_port_add_info_to_port_mode(
    pnp_dynamic_mode* port_mode, USB_CHAR* new_name, USB_UINT8 prot_procotol);
USB_VOID pnp_config_dev_by_port_index_start(USB_VOID);
USB_VOID pnp_dynamic_port_get_info_from_NV(USB_INT portindex);
USB_VOID pnp_static_port_set_mass_lun_info( hw_usb_mode* port_mode );
USB_VOID pnp_dynamic_port_clear_mode(USB_UCHAR port_index);
USB_CHAR* pnp_dynamic_port_case_port_name(USB_CHAR* case_name);
USB_VOID pnp_dynamic_port_mode_default(pnp_dynamic_mode* dynamic_port_mode);
USB_UINT8 pnp_dynamic_port_get_port_protocol(USB_CHAR* fd_name);

static int android_bind(struct usb_composite_dev* cdev);
static int android_setup(struct usb_gadget* gadget, const struct usb_ctrlrequest* c);
static USB_INT android_usb_unbind(struct usb_composite_dev* cdev);
static void android_disconnect(struct usb_composite_dev* cdev);
static void android_suspend(struct usb_composite_dev* cdev);
static void android_resume(struct usb_composite_dev* cdev);
struct android_dev*  android_get_android_dev(void);
/*****************************************************************************
}  内部接口 end
*****************************************************************************/

static struct usb_composite_driver android_usb_driver =
{
    .name       = "android_usb",
    .dev        = &device_desc,
    .strings    = dev_strings,
    .bind       = android_bind,
    .setup      = android_setup,
    .unbind     = android_usb_unbind,
    .disconnect = android_disconnect,
    .resume     = android_resume,
    .suspend    = android_suspend,
    .max_speed  = USB_SPEED_HIGH,
};

static struct class* android_class;
static struct android_dev* _android_dev  = NULL;

/* 设置定时器，检查set_config 状态 */
static struct timer_list g_soft_reconnect_timer ;
/*插拔或切换后10s没有收到set_config的最长时间*/
static USB_INT g_soft_reconnect_loop = 10000;

static struct delayed_work  pnp_switch_work;

static struct delayed_work  pnp_notify_rewind_after_work;

static struct delayed_work  pnp_remove_work;

/*按照3.0 协议要求至少30ms，为了信号稳定，默认延时100ms*/
USB_INT g_soft_delyay = 100;

static int setmode_state = 0;
struct delayed_work setmode_work;

static huawei_dynamic_info_st  g_pnp_info  = {0};
static usb_enum_state  g_usb_enum_state    = USB_ENUM_NONE;


USB_INT g_set_adress_flag = SET_ADD_NONE;

USB_INT         is_in_dload         = 0;

static int functions_store_enable = 0;

#include "hw_pnp_adapt.c"

static struct android_usb_function* supported_functions[USB_NUM_30] =
{
    NULL
};

pnp_dynamic_mode g_pnp_dynamic_port_mode_info[] =
{
    {
        DYNAMIC_REWIND_BEFORE_MODE,
        {NULL},
        {USB_IF_PROTOCOL_VOID},
        CDROM_SCENE_PID
    },
    {
        DYNAMIC_REWIND_AFTER_MODE,
        {NULL},
        {USB_IF_PROTOCOL_VOID},
        CDROM_SCENE_PID
    },
    {
        DYNAMIC_DEBUG_MODE,
        {NULL},
        {USB_IF_PROTOCOL_VOID},
        DYNAMIC_DEBUG_PID
    },
    {
        HILINK_PORT_MODEM_MODE,
        {"modem", "pcui"},
        {USB_IF_PROTOCOL_MODEM, USB_IF_PROTOCOL_PCUI},
        DYNAMIC_PID
    },

    {
        HILINK_PORT_MODEM_DEBUG_MODE,
        {"modem", "pcui", "diag", "adb"},
        {USB_IF_PROTOCOL_MODEM, USB_IF_PROTOCOL_PCUI, USB_IF_PROTOCOL_DIAG, USB_IF_PROTOCOL_VOID},
        DYNAMIC_PID
    },
};

/*****************************************************************************
android 设备驱动功能 begin{
*****************************************************************************/

struct android_dev*  android_get_android_dev(void)
{
    return _android_dev;
}

static USB_INT android_bind_enabled_functions(struct android_dev* dev,
        struct usb_configuration* c)
{
    struct android_usb_function* f;
    struct android_configuration* conf =
        container_of(c, struct android_configuration, usb_config);
    USB_INT ret;

    list_for_each_entry(f, &conf->enabled_functions, enabled_list)
    {
        DBG_I(MBB_PNP, "bind '%-10s'\n", f->name);
        ret = f->bind_config(f, c);

        if (ret)
        {
            DBG_E(MBB_PNP, "conf %d: %s failed",
                  conf->usb_config.bConfigurationValue, f->name);
            return ret;
        }
    }
    return 0;
}

static void
android_unbind_enabled_functions(struct android_dev* dev,
                                 struct usb_configuration* c)
{
    struct android_usb_function* f;
    struct android_configuration* conf =
        container_of(c, struct android_configuration, usb_config);

    list_for_each_entry(f, &conf->enabled_functions, enabled_list)
    {
        if (f->unbind_config)
        {
            DBG_T(MBB_PNP, "unbind android_usb_func '%-10s'\n", f->name);
            f->unbind_config(f, c);
        }
    }
}

static int android_bind_config(struct usb_configuration* c)
{
    struct android_dev* dev = android_get_android_dev();
    int ret = 0;

    if ( NULL == dev)
    {
        DBG_E(MBB_PNP, "android_get_android_dev NULL!\n");
        return ret;
    }

    ret = android_bind_enabled_functions(dev, c);

    if (ret)
    { return ret; }

    return 0;
}

static void android_unbind_config(struct usb_configuration* c)
{
    struct android_dev* dev = android_get_android_dev();

    if ( NULL == dev)
    {
        DBG_E(MBB_PNP, "android_get_android_dev NULL!\n");
        return;
    }

    android_unbind_enabled_functions(dev, c);
}

static int android_enable(struct android_dev* dev)
{
    struct usb_composite_dev* cdev = dev->cdev;
    struct android_configuration* conf;
    int err = -1;

    if (WARN_ON(!dev->disable_depth))
    { return err; }

    if (--dev->disable_depth == 0)
    {

        list_for_each_entry(conf, &dev->android_configs, list_item)
        {
            err = usb_add_config(cdev, &conf->usb_config,
                                 android_bind_config);

            if (err < 0)
            {
                DBG_E(MBB_PNP, "usb_add_config failed : err: %d\n", err);
                return err;
            }
        }
    }

    return err;
}

static void android_disable(struct android_dev* dev)
{
    struct usb_composite_dev* cdev = dev->cdev;
    struct android_configuration* conf;
    struct list_head* curr_conf = &dev->android_configs;

    if (dev->disable_depth++ == 0)
    {
        DBG_I(MBB_PNP, "start disable process\n");

        usb_gadget_disconnect(cdev->gadget);
        /* Cancel pending control requests */
        usb_ep_dequeue(cdev->gadget->ep0, cdev->req);

        list_for_each_entry(conf, &dev->android_configs, list_item)
        {
            DBG_I(MBB_PNP, "remove config/%p!\n", &conf->usb_config);
            usb_remove_config(cdev, &conf->usb_config);
        }

        /* Free uneeded configurations if exists */
        while (!list_empty(curr_conf))
        {
            conf = list_entry(curr_conf->next,
                              struct android_configuration, list_item);
            pnp_free_android_config(dev, conf);
        }
        dev->enabled = MBB_USB_FALSE;
    }
}

static int android_init_functions(struct android_usb_function** functions,
                                  struct usb_composite_dev* cdev)
{
    struct android_dev* dev = android_get_android_dev();
    struct android_usb_function* f;
    struct device_attribute** attrs;
    struct device_attribute* attr;
    int err = 0;
    int index = 1; /* index 0 is for android0 device */

    if ( NULL == dev)
    {
        DBG_E(MBB_PNP, "android_get_android_dev NULL!\n");
        return err;
    }

    for (; (f = *functions++); index++)
    {
        f->dev_name = kasprintf(GFP_KERNEL, "f_%s", f->name);
        f->android_dev = NULL;

        if (!f->dev_name)
        {
            err = (-ENOMEM);
            goto err_out;
        }

        f->dev = device_create(android_class, dev->dev,
                               MKDEV(0, index), f, f->dev_name);
        if (IS_ERR(f->dev))
        {
            DBG_E(MBB_PNP, "Failed to create dev %s", f->dev_name);
            err = PTR_ERR(f->dev);
            f->dev = NULL;
            goto err_create;
        }

        if (f->init)
        {
            err = f->init(f, cdev);

            if (err)
            {
                DBG_E(MBB_PNP, "Failed to init %s", f->name);
                goto err_init;
            }
        }

        attrs = f->attributes;

        if (attrs)
        {
            while ((attr = *attrs++) && !err)
            {
                err = device_create_file(f->dev, attr);
            }
        }

        if (err)
        {
            DBG_E(MBB_PNP, "Failed to create function %s attributes", f->name);
            goto err_attrs;
        }
    }

    return 0;

err_attrs:

    for (attr = *(attrs -= USB_NUM_2); attrs != f->attributes; attr = *(attrs--))
    { device_remove_file(f->dev, attr); }

    if (f->cleanup)
    { f->cleanup(f); }

err_init:
    device_destroy(android_class, f->dev->devt);
err_create:
    f->dev = NULL;
    kfree(f->dev_name);
err_out:
    android_cleanup_functions(dev->functions);
    return err;
}

static USB_INT android_enable_function(struct android_dev* dev,
                                       struct android_configuration* c,
                                       USB_CHAR* name,
                                       USB_UINT8 procotol)
{
    struct android_usb_function** functions = dev->functions;
    struct android_usb_function* f;
    struct android_usb_function* f_tmp;

    if ( NULL == c )
    {
        return (-EINVAL);
    }

    while ((f = *functions++))
    {
        if (!strcmp(name, f->name))
        {
            list_for_each_entry(f_tmp, &c->enabled_functions, enabled_list)
            {
                if (!strcmp(name, f_tmp->name))
                {
                    DBG_E(MBB_PNP, "name:%s**f_tmp->name:%s has been enabled\n",
                          name, f_tmp->name);
                    return 0;
                }
            }
            f->property = procotol;
            list_add_tail(&f->enabled_list, &c->enabled_functions);
            return 0;
        }
    }

    return (-EINVAL);
}

/*-------------------------------------------------------------------------*/
/* /sys/class/android_usb/android%d/ interface */
/*-------------------------------------------------------------------------*/

static ssize_t remote_wakeup_show(struct device* pdev,
                                  struct device_attribute* attr, char* buf)
{
    struct android_dev* dev = dev_get_drvdata(pdev);
    struct android_configuration* conf;

    /*
     * Show the wakeup attribute of the first configuration,
     * since all configurations have the same wakeup attribute
     */
    if (dev->configs_num == 0)
    { return 0; }

    conf = list_entry(dev->android_configs.next,
                      struct android_configuration,
                      list_item);

    return snprintf(buf, PAGE_SIZE, "%d\n",
                    !!(conf->usb_config.bmAttributes &
                       USB_CONFIG_ATT_WAKEUP));
}

static ssize_t remote_wakeup_store(struct device* pdev,
                                   struct device_attribute* attr, const char* buff, size_t size)
{
    struct android_dev* dev = dev_get_drvdata(pdev);
    struct android_configuration* conf;
    int enable = 0;

    sscanf(buff, "%d", &enable);

    pr_debug("android_usb: %s remote wakeup\n",
             enable ? "enabling" : "disabling");

    list_for_each_entry(conf, &dev->android_configs, list_item)
    {
        if (enable)
            conf->usb_config.bmAttributes |=
                USB_CONFIG_ATT_WAKEUP;
        else
            conf->usb_config.bmAttributes &=
                ~USB_CONFIG_ATT_WAKEUP;
    }

    return size;
}

static ssize_t
functions_store_enable_show(struct device* pdev, struct device_attribute* attr, char* buf)
{
    return snprintf(buf, PAGE_SIZE, "%d\n", functions_store_enable);
}

static ssize_t
functions_store_enable_store(struct device* pdev, struct device_attribute* attr,
                             const char* buff, size_t size)
{
    /*code cc */
    sscanf(buff, "%d", &functions_store_enable);
    return size;
}

static ssize_t
functions_show(struct device* pdev, struct device_attribute* attr, char* buf)
{
    struct android_dev* dev = dev_get_drvdata(pdev);
    struct android_configuration* conf;
    struct android_usb_function* f;
    char* buff = buf;

    mutex_lock(&dev->mutex);

    list_for_each_entry(conf, &dev->android_configs, list_item)
    {
        if (buff != buf)
        { *(buff - 1) = ':'; }

        list_for_each_entry(f, &conf->enabled_functions, enabled_list)
        buff += snprintf(buff, PAGE_SIZE, "%s,", f->name);
    }

    mutex_unlock(&dev->mutex);

    if (buff != buf)
    { *(buff - 1) = '\n'; }

    return buff - buf;
}

static ssize_t
functions_store(struct device* pdev, struct device_attribute* attr,
                const char* buff, size_t size)
{
    struct android_dev* dev = dev_get_drvdata(pdev);
    char* conf_str;
    char* stor_name;
    char* case_name;
    char buf[USB_NUM_256], *b;
    USB_INT port_protocol = 0;
    pnp_dynamic_mode* dynamic_port_mode = NULL;

    if (0 == functions_store_enable)
    {
        DBG_T(MBB_PNP, "can not store functions \n");
        return -1;
    }
    mutex_lock(&dev->mutex);

    strlcpy(buf, buff, sizeof(buf));
    b = strim(buf);

    pnp_dynamic_port_clear_mode(DYNAMIC_DEBUG_MODE);
    dynamic_port_mode = pnp_dynamic_port_get_mode(DYNAMIC_DEBUG_MODE);


    while (b)
    {
        conf_str = strsep(&b, ":");

        if (!conf_str)
        { continue; }

        while (conf_str)
        {
            DBG_T(MBB_PNP, "conf_str = %s", conf_str);
            stor_name = strsep(&conf_str, ",");
            case_name = pnp_dynamic_port_case_port_name(stor_name);
            DBG_T(MBB_PNP, "case_name = %s", case_name);
            if ( NULL != case_name)
            {
                port_protocol = pnp_dynamic_port_get_port_protocol(case_name);
                if (port_protocol > USB_IF_PROTOCOL_VOID)
                {
                    pnp_dynamic_port_add_info_to_port_mode(dynamic_port_mode,
                                                           case_name, port_protocol);
                }
                else
                {
                    pnp_dynamic_port_add_info_to_port_mode(dynamic_port_mode,
                                                           case_name, USB_IF_PROTOCOL_VOID);
                }
            }
        }
    }

    g_pnp_info.portModeIndex = DYNAMIC_DEBUG_MODE;

    pnp_notify_to_switch();

    DBG_T(MBB_PNP, "success\n");
    mutex_unlock(&dev->mutex);

    return size;
}



static ssize_t enable_show(struct device* pdev, struct device_attribute* attr,
                           char* buf)
{
    struct android_dev* dev = dev_get_drvdata(pdev);

    return snprintf(buf, PAGE_SIZE, "%d\n", dev->enabled);
}


static ssize_t pm_qos_show(struct device* pdev,
                           struct device_attribute* attr, char* buf)
{
    struct android_dev* dev = dev_get_drvdata(pdev);

    return snprintf(buf, PAGE_SIZE, "%s\n", dev->pm_qos);
}

static ssize_t pm_qos_store(struct device* pdev,
                            struct device_attribute* attr,
                            const char* buff, size_t size)
{
    struct android_dev* dev = dev_get_drvdata(pdev);

    strlcpy(dev->pm_qos, buff, sizeof(dev->pm_qos));

    return size;
}


static ssize_t state_show(struct device* pdev, struct device_attribute* attr,
                          char* buf)
{
    struct android_dev* dev = dev_get_drvdata(pdev);
    struct usb_composite_dev* cdev = dev->cdev;
    char* state = "DISCONNECTED";
    unsigned long flags;

    if (!cdev)
    { goto out; }

    spin_lock_irqsave(&cdev->lock, flags);

    if (cdev->config)
    { state = "CONFIGURED"; }
    else if (dev->connected)
    { state = "CONNECTED"; }

    spin_unlock_irqrestore(&cdev->lock, flags);
out:
    return snprintf(buf, PAGE_SIZE, "%s\n", state);
}

static void setmode_work_func(struct work_struct* work)
{
    mbb_usb_nv_info_st* usb_nv_info = usb_nv_get_ctx();
    DBG_I(MBB_PNP, "setmode_work_func  !\n");
    switch (setmode_state)
    {
        case APP_SETMODE_NORMAL_WORK:
            pnp_switch_autorun_port();
            break;
        case APP_SETMODE_RNDIS_DEBUG:
            pnp_switch_rndis_debug_mode();
            break;

        case APP_SETMODE_RNDIS_DLOAD:
            pnp_switch_rndis_project_mode();
            break;
        case APP_SETMODE_MBIM_DEBUG:
            if ( MBB_NV_ENABLE == usb_nv_info->winblue_profile.MBIMEnable)
            {
                pnp_switch_mbim_debug_mode();
            }
            break;
        case APP_SETMODE_CHARGE_ONLY:
            pnp_switch_charge_only_port();
            break;
        default:
            break;
    }
}
static ssize_t setmode_show(struct device* pdev, struct device_attribute* attr,
                            char* buf)
{
    return snprintf(buf, PAGE_SIZE, "%d\n", setmode_state);
}

static ssize_t
setmode_store(struct device* pdev, struct device_attribute* attr,
              const char* buff, size_t size)
{
    struct android_dev* dev = android_get_android_dev();
    struct usb_composite_dev* cdev = dev->cdev;
    int app_setmode = -1;

    if (1 == sscanf(buff, "%4d", &app_setmode))
    {
        DBG_T(MBB_PNP, "app_setmode : %d\n", app_setmode);
        if (app_setmode == setmode_state)
        {
            DBG_T(MBB_PNP, "setmode don't changed\n");
            return size;
        }
        /* 此处会引起Code Checker警告，但是此处代码完全符合C语言语法，编程规范*/
        setmode_state = app_setmode;
        /*先拉低D+，待IPA状态更新，否则会死机*/
        usb_gadget_disconnect(cdev->gadget);
        DBG_I(MBB_PNP, "setmode_store %d!\n", setmode_state);
        pnp_usb_queue_delay_work(&setmode_work, 1000 * HZ / 1000);
        return size;
    }
    else
    {
        DBG_E(MBB_PNP, "setmode read failed\n");
        return -ENOMEM;
    }

}

static ssize_t usb_private_show(struct device* pdev, struct device_attribute* attr,
                                char* buf)
{
    usb_nv_dump();
    pnp_dump();
    return 0;
}

static ssize_t
usb_module_log_show(struct device* pdev, struct device_attribute* attr,
                    char* buf)
{
    usb_debug_show_module_lev();
    return 0;
}

static ssize_t
usb_module_log_store(struct device* pdev, struct device_attribute* attr,
                     const char* buff, size_t size)
{
    char* change;
    USB_INT module;
    USB_INT lever;
    char buf[USB_NUM_256], *b;

    strlcpy(buf, buff, sizeof(buf));
    b = strim(buf);

    if (NULL != b)
    {
        change = strsep(&b, ",");
        if (NULL != change)
        {
            if (1 != sscanf(change, "%2d", &module))
            {
                DBG_E(MBB_PNP, "store module err\n");
            }
        }
        else
        {
            DBG_E(MBB_PNP, "echo module err\n");
        }
        change = strsep(&b, ",");
        if (NULL != change)
        {
            if (1 != sscanf(change, "%2d", &lever))
            {
                DBG_E(MBB_PNP, "store lever err\n");
            }
        }
        else
        {
            DBG_E(MBB_PNP, "echo lever err\n");
        }
        DBG_SET_LEVEL(module, lever);
        DBG_T(MBB_PNP, "success\n");
    }

    return size;
}

static ssize_t dialmode_show(struct device* pdev, struct device_attribute* attr,
                             char* buf)
{
    USB_UINT PsBuffer = 0;
    USB_UINT CdcBuffer = 0;

    if (0 != Query_USB_PortType(&PsBuffer , &CdcBuffer))
    {
        DBG_E(MBB_PNP, "dialmode_show err !\n");
        return 0;
    }
    return snprintf(buf, PAGE_SIZE, "%d,%d\n", PsBuffer, CdcBuffer);
}

#define DESCRIPTOR_ATTR(field, format_string)               \
    static ssize_t                              \
    field ## _show(struct device *dev, struct device_attribute *attr,   \
                   char *buf)                       \
    {                                   \
        return snprintf(buf, PAGE_SIZE,                 \
                        format_string, device_desc.field);      \
    }                                   \
    static ssize_t                              \
    field ## _store(struct device *dev, struct device_attribute *attr,  \
                    const char *buf, size_t size)               \
    {                                   \
        int value;                          \
        if (sscanf(buf, format_string, &value) == 1) {          \
            device_desc.field = value;              \
            return size;                        \
        }                               \
        return -1;                          \
    }                                   \
    static DEVICE_ATTR(field, S_IRUGO | S_IWUSR, field ## _show, field ## _store);

#define DESCRIPTOR_STRING_ATTR(field, buffer)               \
    static ssize_t                              \
    field ## _show(struct device *dev, struct device_attribute *attr,   \
                   char *buf)                       \
    {                                   \
        return snprintf(buf, PAGE_SIZE, "%s", buffer);          \
    }                                   \
    static ssize_t                              \
    field ## _store(struct device *dev, struct device_attribute *attr,  \
                    const char *buf, size_t size)               \
    {                                   \
        if (size >= sizeof(buffer))                 \
            return -EINVAL;                     \
        strlcpy(buffer, buf, sizeof(buffer));               \
        strim(buffer);                          \
        return size;                            \
    }                                   \
    static DEVICE_ATTR(field, S_IRUGO | S_IWUSR, field ## _show, field ## _store);


DESCRIPTOR_ATTR(idVendor, "%04x\n")
DESCRIPTOR_ATTR(idProduct, "%04x\n")
DESCRIPTOR_ATTR(bcdDevice, "%04x\n")
DESCRIPTOR_ATTR(bDeviceClass, "%d\n")
DESCRIPTOR_ATTR(bDeviceSubClass, "%d\n")
DESCRIPTOR_ATTR(bDeviceProtocol, "%d\n")
DESCRIPTOR_STRING_ATTR(iManufacturer, manufacturer_string)
DESCRIPTOR_STRING_ATTR(iProduct, product_string)
DESCRIPTOR_STRING_ATTR(iSerial, serial_string)

static DEVICE_ATTR(functions_store_enable, S_IRUGO | S_IWUSR, functions_store_enable_show,
                   functions_store_enable_store);
static DEVICE_ATTR(functions, S_IRUGO | S_IWUSR, functions_show,
                   functions_store);
static DEVICE_ATTR(enable, S_IRUGO | S_IWUSR, enable_show, enable_store);
static DEVICE_ATTR(pm_qos, S_IRUGO | S_IWUSR,
                   pm_qos_show, pm_qos_store);
static DEVICE_ATTR(state, S_IRUGO, state_show, NULL);
static DEVICE_ATTR(remote_wakeup, S_IRUGO | S_IWUSR,
                   remote_wakeup_show, remote_wakeup_store);

static DEVICE_ATTR(setmode, S_IRUGO | S_IWUSR, setmode_show, setmode_store);
static DEVICE_ATTR(usb_private, S_IRUGO, usb_private_show, NULL);
static DEVICE_ATTR(usb_module_log, S_IRUGO | S_IWUSR, usb_module_log_show, usb_module_log_store);
static DEVICE_ATTR(smode, S_IRUGO | S_IWUSR, smode_show, smode_store);
static DEVICE_ATTR(dialmode, S_IRUGO , dialmode_show, NULL);

static struct device_attribute* android_usb_attributes[] =
{
    &dev_attr_idVendor,
    &dev_attr_idProduct,
    &dev_attr_bcdDevice,
    &dev_attr_bDeviceClass,
    &dev_attr_bDeviceSubClass,
    &dev_attr_bDeviceProtocol,
    &dev_attr_iManufacturer,
    &dev_attr_iProduct,
    &dev_attr_iSerial,
    &dev_attr_functions,
    &dev_attr_enable,
    &dev_attr_pm_qos,
    &dev_attr_state,
    &dev_attr_remote_wakeup,
    &dev_attr_setmode,
    &dev_attr_usb_private,
    &dev_attr_usb_module_log,
    &dev_attr_functions_store_enable,
    &dev_attr_smode,
    &dev_attr_dialmode,
    NULL
};

static int android_create_device(struct android_dev* dev, u8 usb_core_id)
{
    struct device_attribute** attrs = android_usb_attributes;
    struct device_attribute* attr;
    char device_node_name[ANDROID_DEVICE_NODE_NAME_LENGTH];
    int err;

    /*
     * The primary usb core should always have usb_core_id=0, since
     * Android user space is currently interested in android0 events.
     */
    snprintf(device_node_name, ANDROID_DEVICE_NODE_NAME_LENGTH,
             "android%d", usb_core_id);
    dev->dev = device_create(android_class, NULL,
                             MKDEV(0, 0), NULL, device_node_name);

    if (IS_ERR(dev->dev))
    { return PTR_ERR(dev->dev); }

    dev_set_drvdata(dev->dev, dev);

    while ((attr = *attrs++))
    {
        err = device_create_file(dev->dev, attr);

        if (err)
        {
            device_destroy(android_class, dev->dev->devt);
            return err;
        }
    }

    return 0;
}

static void android_destroy_device(struct android_dev* dev)
{
    struct device_attribute** attrs = android_usb_attributes;
    struct device_attribute* attr;

    while ((attr = *attrs++))
    { device_remove_file(dev->dev, attr); }

    device_destroy(android_class, dev->dev->devt);
}

static USB_INT android_usb_unbind(struct usb_composite_dev* cdev)
{
    struct android_dev* dev = android_get_android_dev();
    int ret = -1;

    if ( NULL == dev)
    {
        DBG_E(MBB_PNP, "android_get_android_dev NULL!\n");
        return ret;
    }

    manufacturer_string[0] = '\0';
    product_string[0] = '\0';
    serial_string[0] = '0';
    cancel_work_sync(&dev->work);
    android_cleanup_functions(dev->functions);
    return 0;
}

static int android_bind(struct usb_composite_dev* cdev)
{
    struct android_dev* dev = android_get_android_dev();
    struct usb_gadget*	gadget = cdev->gadget;
    struct android_configuration* conf;
    int	id;
    int ret = -1;
    mbb_usb_nv_info_st* usb_nv_info = usb_nv_get_ctx();

    if ( NULL == dev)
    {
        DBG_E(MBB_PNP, "android_get_android_dev NULL!\n");
        return ret;
    }
    pnp_usb_init_enum_stat();
    dev->cdev = cdev;

    /* Save the default handler */
    dev->setup_complete = cdev->req->complete;

    /*
     * Start disconnected. Userspace will connect the gadget once
     * it is done configuring the functions.
     */
    usb_gadget_disconnect(gadget);

    /* Init the supported functions only once, on the first android_dev */
    ret = android_init_functions(dev->functions, cdev);

    if (ret)
    {
        DBG_E(MBB_PNP, "android_init_functions return err %d\n", ret);
        return ret;
    }

    /* Allocate string descriptor numbers ... note that string
     * contents can be overridden by the composite_dev glue.
     */
    id = usb_string_id(cdev);

    if (id < 0)
    {
        DBG_E(MBB_PNP, "usb_string_id STRING_MANUFACTURER_IDX return err %d\n", id);
        return id;
    }
    strings_dev[STRING_MANUFACTURER_IDX].id = id;
    device_desc.iManufacturer = id;

    id = usb_string_id(cdev);

    if (id < 0)
    {
        DBG_E(MBB_PNP, "usb_string_id STRING_PRODUCT_IDX return err %d\n", id);
        return id;
    }
    strings_dev[STRING_PRODUCT_IDX].id = id;
    device_desc.iProduct = id;

    /* Default strings - should be updated by userspace */

    strncpy(manufacturer_string,
            usb_nv_info->port_dynamic_name.huawei_manufacturer_dynamic_name,
            DYNAMIC_INFO_NAME_CHAR_NUM - 1);

    strncpy(product_string,
            usb_nv_info->port_dynamic_name.huawei_product_dynamic_name,
            DYNAMIC_INFO_NAME_CHAR_NUM - 1);

    strncpy(serial_string, "0123456789ABCDEF", sizeof(serial_string) - 1);

    id = usb_string_id(cdev);

    if (id < 0)
    {
        DBG_E(MBB_PNP, "usb_string_id STRING_SERIAL_IDX return err %d\n", id);
        return id;
    }
    strings_dev[STRING_SERIAL_IDX].id = id;
    device_desc.iSerialNumber = id;

    if (gadget_is_otg(cdev->gadget))
        list_for_each_entry(conf, &dev->android_configs, list_item)
        conf->usb_config.descriptors = otg_desc;

    return 0;
}

static int
android_setup(struct usb_gadget* gadget, const struct usb_ctrlrequest* c)
{
    struct usb_composite_dev*	cdev = get_gadget_data(gadget);
    struct android_dev* dev = android_get_android_dev();
    struct usb_request*		req = cdev->req;
    struct android_usb_function*	f;
    struct android_configuration*	conf;
    int value = -EOPNOTSUPP;
    unsigned long flags;
    bool do_work = false;
    int ret = -1;

    if ( NULL == dev)
    {
        DBG_E(MBB_PNP, "android_get_android_dev NULL!\n");
        return ret;
    }

    req->zero = 0;
    req->length = 0;
    req->complete = dev->setup_complete;
    gadget->ep0->driver_data = cdev;

    list_for_each_entry(conf, &dev->android_configs, list_item)
    list_for_each_entry(f,
                        &conf->enabled_functions,
                        enabled_list)
    {
        if (f->ctrlrequest)
        {
            value = f->ctrlrequest(f, cdev, c);

            if (value >= 0)
            { break; }
        }
    }

    if (value < 0 || (USB_GADGET_DELAYED_STATUS == value) )
    { value = pnp_huawei_vendor_setup(gadget, c); }

    spin_lock_irqsave(&cdev->lock, flags);

    if (!dev->connected)
    {
        dev->connected = 1;
        do_work = true;
    }
    else if (c->bRequest == USB_REQ_SET_CONFIGURATION &&
             cdev->config)
    {
        do_work = true;
    }

    spin_unlock_irqrestore(&cdev->lock, flags);

    if (do_work)
    {
        schedule_work(&dev->work);
    }

    return value;
}


static void android_disconnect(struct usb_composite_dev* cdev)
{
    struct android_dev* dev = android_get_android_dev();

    if ( NULL == dev)
    {
        DBG_E(MBB_PNP, "android_get_android_dev NULL!\n");
        return ;
    }

    /*do other disconnect if necessary*/
    pnp_accessory_disconnect();

    dev->connected = 0;
    schedule_work(&dev->work);
}

static void android_suspend(struct usb_composite_dev* cdev)
{
    usb_enum_state state = USB_ENUM_NONE;
    unsigned long flags;
    struct android_dev* dev = android_get_android_dev();

    DBG_I(MBB_PNP, " android_suspend \n");
    if ( NULL == dev)
    {
        DBG_E(MBB_PNP, "android_get_android_dev NULL!\n");
        return ;
    }
    state = huawei_get_usb_enum_state();
    if ( USB_ENUM_DONE == state )
    {
        usb_notify_syswatch(DEVICE_ID_USB, USB_SUSPEND_NOTIFY);
    }
    spin_lock_irqsave(&cdev->lock, flags);

    if (!dev->suspended)
    {
        dev->suspended = 1;
        schedule_work(&dev->work);
    }

    spin_unlock_irqrestore(&cdev->lock, flags);

}

static void android_resume(struct usb_composite_dev* cdev)
{
    usb_enum_state state = USB_ENUM_NONE;
    unsigned long flags;
    struct android_dev* dev = android_get_android_dev();
    DBG_I(MBB_PNP, " android_resume \n");

    if ( NULL == dev)
    {
        DBG_E(MBB_PNP, "android_get_android_dev NULL!\n");
        return;
    }
    state = huawei_get_usb_enum_state();
    if ( USB_ENUM_DONE == state )
    {
        usb_notify_syswatch(DEVICE_ID_USB, USB_RESUME_NOTIFY);
    }
    spin_lock_irqsave(&cdev->lock, flags);

    if (dev->suspended)
    {
        dev->suspended = 0;
        schedule_work(&dev->work);
    }

    spin_unlock_irqrestore(&cdev->lock, flags);

}

/*****************************************************************************
}  android 设备驱动功能 end
*****************************************************************************/

USB_INT pnp_status_check_download(USB_VOID)
{
    USB_INT dload_flag 		= MBB_USB_FALSE;
    USB_INT nv_backup_flag 	= MBB_USB_FALSE;
    mbb_usb_nv_info_st* usb_nv_info = usb_nv_get_ctx();

    DBG_I(MBB_PNP, "\n");
    dload_flag 		= get_dload_flag();
    /*需要增加NV备份接口*/
    nv_backup_flag 	= get_nv_backup_flag();

    /*MBIM升级报1568，增加静态端口形态*/
    if (( MBB_USB_TRUE == dload_flag ) || ( MBB_USB_TRUE == nv_backup_flag ))
    {
        g_pnp_info.current_port_style 	= download_port_style;

        /*MBIM升级*/
        if ( MBB_NV_ENABLE == usb_nv_info->winblue_profile.MBIMEnable )
        {
            g_pnp_info.portModeIndex 	= MBIM_DOWNLOAD_MODE;
        }
        else
        {
#ifdef USB_RNDIS
            g_pnp_info.portModeIndex 	= PCUI_DIAG_MODE;
#else
            g_pnp_info.portModeIndex 	= DIAG_PCUI_MODE_EX;
#endif /* #ifdef USB_RNDIS */
        }

#ifdef MBB_USB_UNITARY_Q
        /*升级的diag报假口*/
        diag_set_diag_switch(MBB_USB_FALSE);
#endif
        DBG_T(MBB_PNP, " COMPLETE\n");
        return PNP_CHECK_DEVICE_STATUS_COMPLETE;
    }

    DBG_I(MBB_PNP, "return CONTINUE\n");
    return PNP_CHECK_DEVICE_STATUS_CONTINUE;
}

USB_INT pnp_status_check_poweroff(USB_VOID)
{
    USB_INT power_off   = MBB_USB_NO;
    USB_INT fast_off    = MBB_USB_NO;
    USB_INT off_status    = MBB_USB_NO;
    struct android_dev* dev = android_get_android_dev();

    DBG_I(MBB_PNP, "\n");
    power_off   = usb_power_off_chg_stat();
    fast_off    = usb_fast_on_off_stat();

    if (MBB_USB_YES == power_off)
    {
        /*power off status enable USB port, do not ctl by app*/
        dev->irq_ctl_port = 1;
        dev->disable_depth = 1;
        off_status = MBB_USB_YES;
    }

    if ( MBB_USB_YES == fast_off)
    {
        off_status = MBB_USB_YES;
    }

    if ( MBB_USB_YES == off_status)
    {
        g_pnp_info.current_port_style   = charge_port_style;
        g_pnp_info.portModeIndex        = POWER_OFF_MODE;
        DBG_T(MBB_PNP, " COMPLETE\n");
        return PNP_CHECK_DEVICE_STATUS_COMPLETE;
    }

    DBG_I(MBB_PNP, "return CONTINUE\n");
    return PNP_CHECK_DEVICE_STATUS_CONTINUE;
}

/*****************************************************************************
 函 数 名  :
 功能描述  :
 输入参数  :
 输出参数  :
 返 回 值  :
 调用函数  :
 被调函数  :
 修改历史      :
  1.日    期   : 2013年9月17日
    作    者   :
    修改内容   : 新生成函数
*****************************************************************************/
USB_INT get_dynamic_port_nopnp_flag(USB_VOID)
{
    mbb_usb_nv_info_st* usb_nv_info = usb_nv_get_ctx();

    if (USB_IF_PROTOCOL_NOPNP == usb_nv_info->dev_profile_info.first_port_style[0])
    {
        //NoPNP 过程只允许在windows 系统使用
        g_pnp_info.rewind_param.bPcType = WINDOWS_OS_FLAG;
        return MBB_USB_TRUE;
    }
    else
    {
        return MBB_USB_FALSE;
    }
}

USB_INT pnp_rndis_full_sence_index_according_pctype( USB_UINT8  OsType )
{
    USB_INT PortModeIndex 	= INVALID_MODE;

    switch (OsType)
    {
        case OS_TYPE_WINDOWS:
            PortModeIndex = RNDIS_DEBUG_MODE;
#ifdef USB_SD
            PortModeIndex = RNDIS_DEBUG_MODE_SD;
#endif
            break;

        case OS_TYPE_MAC:
        case OS_TYPE_LINUX:
            PortModeIndex = ECM_DEBUG_MODE;
#ifdef USB_SD
            PortModeIndex = ECM_DEBUG_MODE_SD;
#endif
            break;

        default:
            break;
    }
    DBG_I(MBB_PNP, "PortModeIndex: %d\n", PortModeIndex);
    return PortModeIndex;
}


USB_INT pnp_status_check_nopnp(USB_VOID)
{
    USB_INT nopnp_mode  =  MBB_USB_FALSE;
    USB_INT PortModeIndex   = INVALID_MODE;

    DBG_I(MBB_PNP, "\n");
    nopnp_mode = get_dynamic_port_nopnp_flag();

    if ( MBB_USB_FALSE == nopnp_mode)
    {
        DBG_I(MBB_PNP, "return CONTINUE\n");
        return PNP_CHECK_DEVICE_STATUS_CONTINUE;
    }

#ifdef USB_RNDIS
    PortModeIndex = pnp_rndis_full_sence_index_according_pctype(g_pnp_info.rewind_param.bPcType);

    if ( INVALID_MODE == PortModeIndex)
    {
        DBG_E(MBB_PNP, "PortModeIndex is INVALID\n");
        /*but we don't make an error */
        PortModeIndex = CDROM_MODE;
    }
#else
    g_pnp_info.current_port_style = rewind_port_style;
    PortModeIndex = DYNAMIC_REWIND_AFTER_MODE;
#endif
    g_pnp_info.portModeIndex = PortModeIndex;
    g_pnp_info.is_mbim_enable = 0;
    pnp_set_ctl_app_flag(CTL_APP_START);

    DBG_T(MBB_PNP, " COMPLETE\n");
    return PNP_CHECK_DEVICE_STATUS_COMPLETE;
}

USB_INT pnp_status_rewind_before(USB_VOID)
{
    DBG_I(MBB_PNP, "\n");

    g_pnp_info.current_port_style = pnp_port_style;

    g_pnp_info.portModeIndex = DYNAMIC_REWIND_BEFORE_MODE;
    DBG_T(MBB_PNP, " COMPLETE\n");
    return PNP_CHECK_DEVICE_STATUS_COMPLETE;
}

/*按优先级check配置状态*/
USB_INT (* pnp_status_check_device_status_funcs[])(USB_VOID) =
{
    pnp_status_check_download,
    pnp_status_check_factory,
    pnp_status_check_poweroff,
    pnp_status_check_charge_only,
    pnp_status_check_nopnp,
    pnp_status_rewind_before,
};

USB_VOID pnp_status_check_device_status_to_select_port_index(USB_VOID)
{
    USB_INT i = 0;
    USB_INT ret = PNP_CHECK_DEVICE_STATUS_CONTINUE;

    DBG_I(MBB_PNP, "\n");

    while (NULL != pnp_status_check_device_status_funcs[i])
    {
        ret = pnp_status_check_device_status_funcs[i]();

        if (HUAWEI_CONFIG_DYNAMIC_PID_COMPLETE == ret )
        {
            /*符合端口配置的条件*/
            break;
        }

        i++;
    }

    DBG_I(MBB_PNP, "switch\n");
    pnp_notify_to_switch();
}

/*=============================================================
切换功能模块
==============================================================*/
static USB_VOID pnp_free_android_config(struct android_dev* dev,
                                        struct android_configuration* conf)
{
    DBG_I(MBB_PNP, "\n");
    list_del(&conf->list_item);
    dev->configs_num--;
    kfree(conf);
    DBG_I(MBB_PNP, "return COMPLETE\n");
}

static struct android_configuration* pnp_alloc_android_config(struct android_dev* androiddev)
{
    struct android_configuration* config;

    DBG_I(MBB_PNP, "\n");
    config = kzalloc(sizeof(*config), GFP_KERNEL);

    if (!config)
    {
        DBG_E(MBB_PNP, "Failed to alloc memory for android conf\n");
        return ERR_PTR(-ENOMEM);
    }

    androiddev->configs_num++;
    config->usb_config.label = androiddev->name;
    config->usb_config.unbind = android_unbind_config;
    config->usb_config.bConfigurationValue = androiddev->configs_num;
    config->usb_config.cdev = androiddev->cdev;

    /* MBIM强制需求REMOTE WAKEUP特性 */
    if (g_pnp_info.is_mbim_enable)
    {
        config->usb_config.bmAttributes &= ~USB_CONFIG_ATT_SELFPOWER;
        config->usb_config.bmAttributes |= USB_CONFIG_ATT_WAKEUP;
    }

    INIT_LIST_HEAD(&config->enabled_functions);

    list_add_tail(&config->list_item, &androiddev->android_configs);
    DBG_I(MBB_PNP, "return COMPLETE\n");
    return config;
}

USB_VOID pnp_select_a_new_config(struct android_dev* androiddev,
                                 struct android_configuration** config, struct list_head* curr_conf)
{
    DBG_I(MBB_PNP, "\n");

    /* If the next not equal to the head, take it */
    if ((curr_conf->next) != &(androiddev->android_configs))
    {
        *config = list_entry(curr_conf->next,
                             struct android_configuration,
                             list_item);
        INIT_LIST_HEAD(&(*config)->enabled_functions);
        DBG_E(MBB_PNP, "it's an error,ConfigurationValue=%d,check if you free_android_config when switch\n",
              (*config)->usb_config.bConfigurationValue);
    }
    else
    {
        *config = pnp_alloc_android_config(androiddev);
    }
}

hw_usb_mode* pnp_static_port_get_mode(USB_UCHAR   index )
{
    USB_INT i = 0 ;
    hw_usb_mode* port_mode = NULL;

    for ( i = 0; i < sizeof(g_usb_port_mode_info) / sizeof(hw_usb_mode) ; i++ )
    {
        if ( g_usb_port_mode_info[i].port_index == index )
        {
            port_mode = g_usb_port_mode_info + i;
            break;
        }
    }

    if (NULL == port_mode) /*如果失败，默认返回光盘端口形态*/
    {
        //如果代码走到这可能是config_static_port_index_for_scene 函数修改错误
        DBG_E(MBB_PNP, "get_hw_usb_static_mode index:%d error! return CDROM mode\n", index);
        WARN_ON(1);
        port_mode = &g_usb_port_mode_info[0];
        g_pnp_info.portModeIndex = CDROM_MODE;
    }

    return port_mode;
}

USB_VOID pnp_bind_port_group_to_dev_config(struct android_dev* androiddev)
{
    struct list_head* curr_conf = &androiddev->android_configs;
    struct android_configuration* config = NULL;
    hw_usb_mode* static_port_mode = NULL;
    pnp_dynamic_mode* dynamic_port_mode = NULL;
    USB_CHAR** port_group_buf = NULL;
    USB_UINT8* port_procotol = NULL;
    USB_CHAR* name = NULL;
    USB_INT err = -1;
    USB_INT i = 0;

    DBG_I(MBB_PNP, "\n");

    if (g_pnp_info.portModeIndex >= INVALID_MODE)
    {
        DBG_E(MBB_PNP, "PortModeIndex is INVALID\n");
        /*but we don't make an error */
        g_pnp_info.portModeIndex = CDROM_MODE;
    }
    /*动态端口*/
    if ( g_pnp_info.portModeIndex >= DYNAMIC_REWIND_BEFORE_MODE )
    {

        pnp_dynamic_port_get_info_from_NV(g_pnp_info.portModeIndex);

        dynamic_port_mode = pnp_dynamic_port_get_mode(g_pnp_info.portModeIndex);
        port_group_buf = dynamic_port_mode->mode_buf;
        port_procotol = dynamic_port_mode->port_protocol;

        if (NULL != port_group_buf)
        {
            /**/
            pnp_select_a_new_config(androiddev, &config, curr_conf);

            curr_conf = curr_conf->next;

            for (i = 0; i < MAX_TOTAL_PORT_NUM; i++)
            {
                name = port_group_buf[i];

                if (NULL == name)
                {
                    DBG_I(MBB_PNP, "port_group_buf is endd i = %d\n", i);
                    break;
                }

                if (!strcmp(name, ":"))
                {
                    /*previous config have enabled_functions*/
                    if (config->enabled_functions.next != &config->enabled_functions )
                    {
                        pnp_select_a_new_config(androiddev, &config, curr_conf);
                        curr_conf = curr_conf->next;
                    }

                    continue;
                }

                err = android_enable_function(androiddev, config, name, port_procotol[i]);

                if (err)
                {
                    DBG_E(MBB_PNP, "android_usb: Cannot enable '%s' (%d)\n"
                          , name, err);
                }
            }

            /* Free uneeded configurations if exists */
            while (curr_conf->next != &androiddev->android_configs)
            {
                config = list_entry(curr_conf->next,
                                    struct android_configuration, list_item);
                pnp_free_android_config(androiddev, config);
            }
        }
    }
    else/*静态端口*/
    {

        static_port_mode = pnp_static_port_get_mode(g_pnp_info.portModeIndex);
        /*静态端口的mass多lun OR 多interface处理*/
        pnp_static_port_set_mass_lun_info(static_port_mode);
        port_group_buf = static_port_mode->mode_buf;

        if (NULL != port_group_buf)
        {
            /**/
            pnp_select_a_new_config(androiddev, &config, curr_conf);

            curr_conf = curr_conf->next;

            for (i = 0; i < DYNAMIC_PID_MAX_PORT_NUM; i++)
            {
                name = port_group_buf[i];

                if (NULL == name)
                {
                    DBG_I(MBB_PNP, "port_group_buf is endd i = %d\n", i);
                    break;
                }

                if (!strcmp(name, ":"))
                {
                    /*previous config have enabled_functions*/
                    if (config->enabled_functions.next != &config->enabled_functions )
                    {
                        pnp_select_a_new_config(androiddev, &config, curr_conf);
                        curr_conf = curr_conf->next;
                    }

                    continue;
                }

                err = android_enable_function(androiddev, config, name, USB_IF_PROTOCOL_VOID);

                if (err)
                {
                    DBG_E(MBB_PNP, "android_usb: Cannot enable '%s' (%d)\n",
                          name, err);
                }
            }

            /* Free uneeded configurations if exists */
            while (curr_conf->next != &androiddev->android_configs)
            {
                config = list_entry(curr_conf->next,
                                    struct android_configuration, list_item);
                pnp_free_android_config(androiddev, config);
            }
        }
    }
}

USB_VOID pnp_static_port_set_mass_lun_info( hw_usb_mode* port_mode )
{
    USB_CHAR** mode_buf = NULL;
    USB_INT i = 0;
    mode_buf = port_mode->mode_buf;
    DBG_I(MBB_PNP, "port_mode %d\n", port_mode->port_index);

    for (i = 0; i < DYNAMIC_PID_MAX_PORT_NUM; i++)
    {
        if ( NULL == mode_buf[i])
        {
            DBG_I(MBB_PNP, "port_mode %d mode_buf end with%d!",
                  port_mode->port_index, i);
            break;
        }

        if (!strcmp(mode_buf[i], "mass"))
        {
            if (port_mode->cdrom)
            {
                DBG_I(MBB_PNP, "port_mode index %d add CD to mass!",
                      port_mode->port_index);
                mass_function_add_lun("mass", "CD");
            }

            if (port_mode->sd)
            {
                DBG_I(MBB_PNP, "port_mode index %d add SD to mass!",
                      port_mode->port_index);
                mass_function_add_lun("mass", "SD");
            }
        }
    }
}

USB_VOID pnp_set_serialnumber_index(struct usb_composite_dev* cdev)
{
    if (rewind_port_style == g_pnp_info.current_port_style)
    {
        /*切换后SN受NV控制*/
        if (0x01 == g_pnp_info.is_daynamic_sn )
        {
            cdev->desc.iSerialNumber = 0;
        }
        else
        {
            cdev->desc.iSerialNumber = strings_dev[STRING_SERIAL_IDX].id;
        }
    }
    else if (pnp_port_style == g_pnp_info.current_port_style)
    {
        cdev->desc.iSerialNumber = strings_dev[STRING_SERIAL_IDX].id;
    }
    else
    {
        /*download_port_style、factory_port_style无SN*/
        cdev->desc.iSerialNumber = 0;
    }

}

USB_VOID pnp_static_port_config_dev_desc_info(struct usb_composite_dev* cdev,
        hw_usb_mode* port_mode)
{
    DBG_I(MBB_PNP, "\n");
    //dev info only need to config 4 points.
    pnp_set_serialnumber_index(cdev);
    cdev->desc.idProduct = port_mode->PID;
    cdev->desc.bDeviceClass = port_mode->bDeviceClass;
    cdev->desc.bDeviceProtocol = 0x00;
}

USB_VOID pnp_dynamic_port_config_dev_desc_info(struct usb_composite_dev* cdev, pnp_dynamic_mode* port_mode)
{
    mbb_usb_nv_info_st* usb_nv_info = usb_nv_get_ctx();

    DBG_I(MBB_PNP, "\n");

    cdev->desc.idProduct = port_mode->PID;
    pnp_set_serialnumber_index(cdev);

    /*配置切换前信息*/
    if (pnp_port_style == g_pnp_info.current_port_style)
    {
        cdev->desc.bDeviceClass = 0x00;

        if ( MBB_NV_ENABLE == usb_nv_info->winblue_profile.MBIMEnable)
        {
            if ( MBB_NV_DISENABLE == usb_nv_info->pid_info.nv_status)
            {
                DBG_E(MBB_PNP, "MBIM 50071 disenabled!!\n");
                cdev->desc.idProduct = PRODUCT_ID_CONFIG2;
            }

            cdev->desc.bDeviceProtocol = VENDOR_PRO;
        }
        else
        {
            cdev->desc.bDeviceProtocol = 0x00;
        }

        DBG_I(MBB_PNP, "pnp_port_style pid %x\n", cdev->desc.idProduct);

    }
    else/*配置切换后信息*/
    {

        cdev->desc.bDeviceClass = 0x00;

#ifdef USB_RNDIS
        if ((MAC_OS_FLAG == g_pnp_info.rewind_param.bPcType)
            || (LINUX_OS_FLAG == g_pnp_info.rewind_param.bPcType))
        {
            cdev->desc.bDeviceClass = USB_CLASS_COMM;
        }
#endif
        cdev->desc.bDeviceProtocol = 0x00;
        DBG_I(MBB_PNP, "rewind_port_style pid %x\n", cdev->desc.idProduct);
    }
}

USB_VOID pnp_config_dev_desc_info(struct android_dev* androiddev)
{
    struct android_dev* dev = android_get_android_dev();
    hw_usb_mode* static_port_mode = NULL;
    pnp_dynamic_mode* dynamic_port_mode = NULL;
    USB_INT PortModeIndex = g_pnp_info.portModeIndex;
    struct usb_composite_dev* cdev = dev->cdev;

    DBG_I(MBB_PNP, "\n");

    if ( PortModeIndex < DYNAMIC_REWIND_BEFORE_MODE )
    {
        static_port_mode = pnp_static_port_get_mode(PortModeIndex);
        pnp_static_port_config_dev_desc_info(cdev, static_port_mode);
    }
    else
    {
        dynamic_port_mode = pnp_dynamic_port_get_mode(PortModeIndex);
        pnp_dynamic_port_config_dev_desc_info(cdev, dynamic_port_mode);
    }

}

USB_VOID pnp_config_dev_by_port_index_start(USB_VOID)
{
    struct android_dev* dev = android_get_android_dev();
    DBG_I(MBB_PNP, "\n");

    pnp_bind_port_group_to_dev_config(dev);

    pnp_config_dev_desc_info(dev);
}

USB_VOID pnp_switch_func(struct work_struct* w)
{
    struct android_dev* dev = NULL;
    struct usb_composite_dev* cdev = NULL;
    USB_INT err = -1;
    DBG_I(MBB_PNP, "\n");
    dev = android_get_android_dev();

    if ( NULL == dev)
    {
        DBG_E(MBB_PNP, "android_get_android_dev NULL!\n");
        return ;
    }

    if (!dev->irq_ctl_port)
    {
        DBG_E(MBB_PNP, "irq_ctl_port is not enable!\n");
        return;
    }
    /*pnp_probe compete with the pnp_remove ,
      mutex the android_dev , and g_pnp_info */
    mutex_lock(&dev->mutex);

    cdev = dev->cdev;
    if ( NULL == cdev)
    {
        DBG_E(MBB_PNP, "dev->cdev NULL!\n");
        goto switch_exit ;
    }

    /*disconnect with PC first*/
    usb_gadget_disconnect(cdev->gadget);
    /* Cancel pending control requests */
    usb_ep_dequeue(cdev->gadget->ep0, cdev->req);

    err = pnp_usb_state_get();
    if (!err)
    {
        DBG_E(MBB_PNP, "remove event from PMU detected \n");
        goto switch_exit ;
    }
    /*if dev is enabled disabled it*/
    if ( MBB_USB_TRUE == dev->enabled )
    {
        android_disable(dev);
    }

    pnp_usb_init_enum_stat();

    pnp_config_dev_by_port_index_start();

    huawei_set_usb_enum_state(USB_ENUM_NONE);

    err = android_enable(dev);
    /*让D+从拉低到拉高的时延符合USB3.0协议*/
    mdelay(g_soft_delyay);

    if (( 0 == dev->disable_depth ) && (!err))
    {
        /*the cdev maybe not the same with the start of pnp_switch_func
          because of maybe remove and reinsert.*/
        cdev = dev->cdev;

        err = pnp_usb_state_get();
        if ((0 != err) && (NULL != cdev))
        {
            usb_gadget_connect(cdev->gadget);
            dev->enabled = MBB_USB_TRUE;
        }
        else
        {
            DBG_T(MBB_PNP, "android_enable fail err=%d\n", err);
        }
    }
    else
    {
        DBG_T(MBB_PNP, "android_enable fail dev->disable_depth=%d,err=%d\n",
              dev->disable_depth, err);
    }
switch_exit:
    mutex_unlock(&dev->mutex);
    DBG_I(MBB_PNP, "pnp_switch_func success!\n");
}

USB_VOID pnp_notify_to_switch(USB_VOID)
{
    pnp_usb_queue_delay_work(&pnp_switch_work, 0);

    mod_timer(&g_soft_reconnect_timer, jiffies + msecs_to_jiffies(g_soft_reconnect_loop));
}

USB_VOID pnp_rewind_after_set_windows_mode(USB_VOID)
{
    USB_INT i = 0;
    pnp_dynamic_mode* rewind_after_mode = NULL;

    DBG_I(MBB_PNP, "\n");
    rewind_after_mode = pnp_dynamic_port_get_mode(DYNAMIC_REWIND_AFTER_MODE);

    for (i = 0; i < MAX_TOTAL_PORT_NUM; i++)
    {
        if ( NULL != rewind_after_mode->mode_buf[i] )
        {
            if (!strcmp(rewind_after_mode->mode_buf[i], "ecm"))
            {
                rewind_after_mode->mode_buf[i] = "rndis";
                break;
            }
        }
        else
        {
            break;
        }
    }
}

USB_VOID pnp_rewind_after_set_mac_linux_mode(USB_VOID)
{
    USB_INT i = 0;
    pnp_dynamic_mode* rewind_after_mode = NULL;

    DBG_I(MBB_PNP, "\n");
    rewind_after_mode = pnp_dynamic_port_get_mode(DYNAMIC_REWIND_AFTER_MODE);

    for (i = 0; i < MAX_TOTAL_PORT_NUM; i++)
    {
        if ( NULL != rewind_after_mode->mode_buf[i] )
        {
            if (!strcmp(rewind_after_mode->mode_buf[i], "rndis"))
            {
                rewind_after_mode->mode_buf[i] = "ecm";
                break;
            }
            else
            {
                break;
            }
        }
    }
}

/*****************************************************************************
 函 数 名  : pnp_dynamic_port_config_mbim_info
 功能描述  : 配置MBIM端口形态
 输入参数  : 无
 输出参数  :
 返 回 值  : 无
 被调函数  : pnp_dynamic_port_get_info_from_NV
 修改历史      :
  1.日    期   : 2014年5月15日
    作    者   : zhangkuo
    修改内容   : 新生成函数
*****************************************************************************/
USB_VOID pnp_notify_rewind_after_func(struct work_struct* w)
{
    USB_INT err = MBB_USB_ERROR;
    DBG_I(MBB_PNP, "\n");
#ifdef USB_RNDIS
#ifdef USB_HILINK_MODEM_SWITCH
    pnp_read_hilink_modem_flag();
#endif
#endif
    if (GATEWAY_OS_FLAG == g_pnp_info.rewind_param.bPcType)
    {
        err = pnp_set_gateway_mode();
        if (MBB_USB_ERROR == err)
        {
            return;
        }
        DBG_T(MBB_PNP, "go gateway mode %d\n", g_pnp_info.portModeIndex);
        goto switch_mode;
    }

#ifdef USB_RNDIS
    else if ( HILINK_MODEM_MODE == g_pnp_info.hilink_mode )
    {
        g_pnp_info.portModeIndex = HILINK_PORT_MODEM_MODE;
        DBG_T(MBB_PNP, "HILINK_PORT_MODEM_MODE\n");
    }
    else if (HILINK_MODEM_DEBUG_MODE == g_pnp_info.hilink_mode )
    {
        g_pnp_info.portModeIndex = HILINK_PORT_MODEM_DEBUG_MODE;
        DBG_T(MBB_PNP, "HILINK_PORT_MODEM_DEBUG_MODE\n");
    }
    else if ((WINDOWS_OS_FLAG == g_pnp_info.rewind_param.bPcType)
             && (product_port_mode == g_pnp_info.rewind_param.bGreenMode))
    {
        pnp_set_ctl_app_flag(CTL_APP_START);
        g_pnp_info.current_port_style = download_port_style;
        g_pnp_info.portModeIndex = PCUI_DIAG_MODE;
        //加高通宏 chenbo
        //pnp_set_diag_switch(MBB_USB_FALSE);
    }
    else if (work_port_mode == g_pnp_info.rewind_param.bGreenMode)
    {
        pnp_set_ctl_app_flag(CTL_APP_START);

        g_pnp_info.portModeIndex = DYNAMIC_REWIND_AFTER_MODE;
    }
    else
    {
        DBG_E(MBB_PNP, "unknown rewind command\n");
        return;
    }

#else
    else
    {
        g_pnp_info.portModeIndex = DYNAMIC_REWIND_AFTER_MODE;
        pnp_set_ctl_app_flag(CTL_APP_START);
    }

#endif
switch_mode:
    pnp_notify_to_switch();
}

/*****************************************************************************
 函 数 名  : pnp_dynamic_port_config_mbim_info
 功能描述  : 配置MBIM端口形态
 输入参数  : 无
 输出参数  :
 返 回 值  : 无
 被调函数  : pnp_dynamic_port_get_info_from_NV
 修改历史      :
  1.日    期   : 2014年5月15日
    作    者   : zhangkuo
    修改内容   : 新生成函数
*****************************************************************************/
USB_VOID pnp_switch_rewind_after_mode(USB_VOID)
{
    /*先更新状态，再切换*/
    g_pnp_info.is_mbim_enable = 0;  /* rewind切换后没有mbim模式 */
    g_pnp_info.current_port_style = rewind_port_style;
    pnp_usb_queue_delay_work(&pnp_notify_rewind_after_work, 0);
}

/*****************************************************************************
 函 数 名  : pnp_dynamic_port_config_mbim_info
 功能描述  : 配置MBIM端口形态
 输入参数  : 无
 输出参数  :
 返 回 值  : 无
 被调函数  : pnp_dynamic_port_get_info_from_NV
 修改历史      :
  1.日    期   : 2014年5月15日
    作    者   : zhangkuo
    修改内容   : 新生成函数
*****************************************************************************/
USB_VOID pnp_switch_mbim_debug_mode(USB_VOID)
{
    DBG_I(MBB_PNP, "\n");
    g_pnp_info.portModeIndex = MBIM_SWITCH_DEBUG_MODE;
    pnp_notify_to_switch();
}

/*****************************************************************************
 函 数 名  : pnp_switch_mbim_normal_mode
 功能描述  : 配置MBIM端口形态
 输入参数  : 无
 输出参数  :
 返 回 值  : 无
 被调函数  : pnp_dynamic_port_get_info_from_NV
 修改历史      :
  1.日    期   : 2014年5月15日
    作    者   : zhangkuo
    修改内容   : 新生成函数
*****************************************************************************/
USB_VOID pnp_switch_normal_work_mode(USB_VOID)
{
    DBG_I(MBB_PNP, "\n");
    g_pnp_info.portModeIndex = DYNAMIC_REWIND_BEFORE_MODE;
    pnp_notify_to_switch();
}

/*****************************************************************************
 函 数 名  : pnp_dynamic_port_config_mbim_info
 功能描述  : 配置MBIM端口形态
 输入参数  : 无
 输出参数  :
 返 回 值  : 无
 被调函数  : pnp_dynamic_port_get_info_from_NV
 修改历史      :
  1.日    期   : 2014年5月15日
    作    者   : zhangkuo
    修改内容   : 新生成函数
*****************************************************************************/
USB_VOID pnp_switch_rndis_debug_mode(USB_VOID)
{
    DBG_I(MBB_PNP, "\n");
    g_pnp_info.portModeIndex =
        pnp_rndis_full_sence_index_according_pctype(g_pnp_info.rewind_param.bPcType);
    pnp_notify_to_switch();
}

/*****************************************************************************
 函 数 名  : pnp_switch_rndis_project_mode
 功能描述  : 配置MBIM端口形态
 输入参数  : 无
 输出参数  :
 返 回 值  : 无
 被调函数  : pnp_dynamic_port_get_info_from_NV
 修改历史      :
  1.日    期   : 2014年5月15日
    作    者   : zhangkuo
    修改内容   : 新生成函数
*****************************************************************************/
USB_VOID pnp_switch_rndis_project_mode(USB_VOID)
{
    DBG_I(MBB_PNP, "\n");
    g_pnp_info.portModeIndex  = PCUI_DIAG_MODE;
    pnp_notify_to_switch();
}

/*****************************************************************************
 函 数 名  : pnp_switch_autorun_port
 功能描述  : 切换功能函数:切换到默认端口状态
 输入参数  : 无
 输出参数  :
 返 回 值  : 无
 被调函数  : pnp_dynamic_port_get_info_from_NV
 修改历史      :
  1.日    期   : 2014年5月15日
    作    者   : zhangkuo
    修改内容   : 新生成函数
*****************************************************************************/
USB_VOID pnp_switch_autorun_port(USB_VOID)
{
    DBG_I(MBB_PNP, "enter\n");
    pnp_probe();
}
/*****************************************************************************
 函 数 名  : pnp_switch_charge_only_port
 功能描述  : 切换功能函数:切换到仅充电端口状态
 输入参数  : 无
 输出参数  :
 返 回 值  : 无
 被调函数  : setmode_work_func
 修改历史      :
  1.日    期   : 2014年5月15日
    修改内容   : 新生成函数
*****************************************************************************/
USB_VOID pnp_switch_charge_only_port(USB_VOID)
{
    DBG_I(MBB_PNP, "\n");
    g_pnp_info.current_port_style   = charge_port_style;
    /*仅充电报关机充电口，光盘可读*/
    g_pnp_info.portModeIndex   = POWER_OFF_MODE;
    pnp_notify_to_switch();
}

#ifdef CONFIG_NCM_MBIM_SUPPORT
USB_VOID pnp_switch_mbim_mode(USB_INT mode)
{
    DBG_I(MBB_PNP, "\n");

    if (MBIM_SWITCH_DEBUG_MODE == mode)
    {
        pnp_switch_mbim_debug_mode();
    }
    else if (MBIM_SWITCH_NORMAL_MODE == mode)
    {
        pnp_switch_autorun_port();
    }
    else
    {
        DBG_E(MBB_PNP, "Invalid mode %u\n", mode);
    }
}
#endif

/*****************************************************************************
 函 数 名  : pnp_dynamic_port_config_mbim_info
 功能描述  : 配置MBIM端口形态
 输入参数  : 无
 输出参数  :
 返 回 值  : 无
 被调函数  : pnp_dynamic_port_get_info_from_NV
 修改历史      :
  1.日    期   : 2014年5月15日
    作    者   : zhangkuo
    修改内容   : 新生成函数
*****************************************************************************/
USB_VOID pnp_dynamic_port_config_mbim_info(USB_VOID)
{
    mbb_usb_nv_info_st* usb_nv_info = usb_nv_get_ctx();
    USB_INT i = 0;
    USB_INT mass_enable = 0;

    DBG_I(MBB_PNP, "enter\n");

    if ( usb_nv_info->winblue_profile.CdRom)
    {
        DBG_I(MBB_PNP, "winblue_profile.CdRom enable\n");
        mass_function_add_lun("mass_two", "CD");
        mass_enable = 1;
    }

    if (usb_nv_info->winblue_profile.TCard)
    {
        DBG_I(MBB_PNP, "winblue_profile.TCard enable\n");
        mass_function_add_lun("mass_two", "SD");
        mass_enable = 1;
    }

    if (mass_enable)
    {
        for (i = 0; i < CONFIG_TWO_MAX_PORT_NUM; i++)
        {
            if (NULL != config_funcs_win8[i])
            {
                if (!strcmp(config_funcs_win8[i], "mass_two"))
                {
                    DBG_I(MBB_PNP, "config_win8 already have mass storage\n");
                    break;
                }
            }
            else
            {
                DBG_I(MBB_PNP, "config_win8 add mass storage\n");
                config_funcs_win8[i] = "mass_two";
                break;
            }
        }

        DBG_E(MBB_PNP, "config_win8 can not add mass\n");
        return;
    }

    DBG_I(MBB_PNP, "success\n");
}

/*****************************************************************************
 函 数 名  : pnp_dynamic_port_add_info_to_port_mode
 功能描述  : 向动态端口表中加入端口名称和协议字段
 输入参数  : 无
 输出参数  :
 返 回 值  : 无
 被调函数  : pnp_dynamic_port_get_info_from_NV
 修改历史      :
  1.日    期   : 2014年5月15日
    作    者   : zhangkuo
    修改内容   : 新生成函数
*****************************************************************************/
USB_VOID pnp_dynamic_port_add_info_to_port_mode(pnp_dynamic_mode* port_mode,
        USB_CHAR* new_name, USB_UINT8 new_procotol)
{
    USB_INT i = 0;
    USB_CHAR** port_group_buf = NULL;
    USB_UINT8* port_procotol = NULL;

    if ( NULL != port_mode)
    {
        port_group_buf = port_mode->mode_buf;
        port_procotol = port_mode->port_protocol;
    }
    if ((NULL == port_group_buf) || (NULL == port_procotol))
    {
        DBG_E(MBB_PNP, "port_group_buf:%p ,port_procotol %p \n", port_group_buf, port_procotol);
        return;
    }
    for (i = 0; i < MAX_TOTAL_PORT_NUM; i++)
    {

        if ( NULL == port_group_buf[i])
        {
            port_group_buf[i] = new_name;
            port_procotol[i] =  new_procotol;

            DBG_I(MBB_PNP, "find %d to add new name:%s\n", i, new_name);
            return;
        }
        else
        {
            DBG_I(MBB_PNP, "port_group[%d] name:%s\n", i, port_group_buf[i]);
        }
    }

    DBG_E(MBB_PNP, "cannot add new name\n");

}

USB_VOID pnp_dynamic_port_clear_mode(USB_UCHAR port_index)
{
    USB_INT i = 0;

    for (i = 0; i < sizeof(g_pnp_dynamic_port_mode_info) / sizeof(pnp_dynamic_mode); i++)
    {
        if ( port_index == g_pnp_dynamic_port_mode_info[i].port_index)
        {
            DBG_I(MBB_PNP, "find port_index = %d in g_pnp_dynamic_port_mode_info[%d]\n",
                  port_index, i);
            pnp_dynamic_port_mode_default(&(g_pnp_dynamic_port_mode_info[i]));
        }
    }
    DBG_E(MBB_PNP, "can not find port_index = %d in g_pnp_dynamic_port_mode_info\n",
          port_index);
}

/*****************************************************************************
 函 数 名  : pnp_dynamic_port_get_mode
 功能描述  : 获取动态端口模式(rewind before/after)
 输入参数  : 无
 输出参数  :
 返 回 值  : 无
 被调函数  : pnp_dynamic_port_get_info_from_NV
             pnp_bind_port_group_to_dev_config
             pnp_config_dev_desc_info
 修改历史      :
  1.日    期   : 2014年5月15日
    作    者   : zhangkuo
    修改内容   : 新生成函数
*****************************************************************************/
pnp_dynamic_mode* pnp_dynamic_port_get_mode(USB_UCHAR port_index)
{
    USB_INT i = 0;

    for (i = 0; i < sizeof(g_pnp_dynamic_port_mode_info) / sizeof(pnp_dynamic_mode); i++)
    {
        if ( port_index == g_pnp_dynamic_port_mode_info[i].port_index)
        {
            DBG_I(MBB_PNP, "find port_index = %d in g_pnp_dynamic_port_mode_info[%d]\n",
                  port_index, i);
            return &(g_pnp_dynamic_port_mode_info[i]);
        }
    }

    DBG_E(MBB_PNP, "can not find port_index = %d in g_pnp_dynamic_port_mode_info\n",
          port_index);
    return NULL;
}

/*****************************************************************************
 函 数 名  : pnp_dynamic_port_get_port_name
 功能描述  :
 输入参数  : 无
 输出参数  :
 返 回 值  : 无
 被调函数  : pnp_dynamic_port_get_info_from_NV
 修改历史      :
  1.日    期   : 2014年5月15日
    作    者   : zhangkuo
    修改内容   : 新生成函数
*****************************************************************************/
USB_CHAR* pnp_dynamic_port_case_port_name(USB_CHAR* case_name)
{
    struct android_dev* dev = android_get_android_dev();
    struct android_usb_function** functions = dev->functions;
    struct android_usb_function* f;

    if (NULL == case_name)
    {
        return NULL;
    }

    while ((f = *functions++))
    {
        if (!strcmp(case_name, f->name))
        {
            DBG_I(MBB_PNP, "find name %s\n", f->name);
            return f->name;
        }
    }
    return NULL;
}

USB_UINT8 pnp_dynamic_port_get_port_protocol(USB_CHAR* fd_name)
{
    USB_UINT8 i = USB_IF_PROTOCOL_VOID;
    if (NULL == fd_name)
    {
        DBG_E(MBB_PNP, "fd_name NULL\n");
        return USB_IF_PROTOCOL_VOID;
    }
    for (i = 0; i < (sizeof(g_huawei_interface_vector) / sizeof(struct huawei_interface_info_st)); i++)
    {
        if (!strcmp(fd_name, g_huawei_interface_vector[i].huawei_interface_name))
        {
            return g_huawei_interface_vector[i].huawei_interface_protocol_num;
        }
    }
    return USB_IF_PROTOCOL_VOID;
}

/*****************************************************************************
 函 数 名  : pnp_dynamic_port_get_port_name
 功能描述  :
 输入参数  : 无
 输出参数  :
 返 回 值  : 无
 被调函数  : pnp_dynamic_port_get_info_from_NV
 修改历史      :
  1.日    期   : 2014年5月15日
    作    者   : zhangkuo
    修改内容   : 新生成函数
*****************************************************************************/
USB_CHAR* pnp_dynamic_port_get_port_name(USB_UINT8 port_index)
{
    USB_INT i = 0;

    if (USB_IF_PROTOCOL_VOID == port_index)
    {
        DBG_E(MBB_PNP, "port_index NULL\n");
        return NULL;
    }

    for (i = 0; i < (sizeof(g_huawei_interface_vector) / sizeof(struct huawei_interface_info_st)); i++)
    {
        if (port_index == g_huawei_interface_vector[i].huawei_interface_protocol_num)
        {
            return g_huawei_interface_vector[i].huawei_interface_name;
        }
    }

    return NULL;
}

USB_VOID pnp_dynamic_port_mode_default(pnp_dynamic_mode* dynamic_port_mode)
{
    USB_INT i = 0;

    for (i = 0; i < MAX_TOTAL_PORT_NUM; i++)
    {
        dynamic_port_mode->mode_buf[i] = NULL;
        dynamic_port_mode->port_protocol[i] = USB_IF_PROTOCOL_VOID;
    }
}

USB_VOID pnp_init_dynamic_port_mode(USB_VOID)
{
    pnp_dynamic_mode* port_mode = NULL;

    port_mode = pnp_dynamic_port_get_mode(DYNAMIC_REWIND_BEFORE_MODE);
    pnp_dynamic_port_mode_default(port_mode);
    port_mode = pnp_dynamic_port_get_mode(DYNAMIC_REWIND_AFTER_MODE);
    pnp_dynamic_port_mode_default(port_mode);
}

/*****************************************************************************
 函 数 名  : pnp_dynamic_port_get_info_from_NV
 功能描述  : 从50091中读取端口形态的protocol组合，并且按照切换前config1，mass
            多lun，config2，mass_two多lun，切换后mass多interface的原则配置mass
            将protocol记录，传递给acm(其他端口不需要)。
 输入参数  : 无
 输出参数  :
 返 回 值  : 无
 被调函数  : pnp_probe
 修改历史      :
  1.日    期   : 2014年5月15日
    作    者   : zhangkuo
    修改内容   : 新生成函数
*****************************************************************************/
USB_VOID pnp_dynamic_port_get_info_from_NV(USB_INT portindex)
{
    mbb_usb_nv_info_st* usb_nv_info = usb_nv_get_ctx();
    USB_INT i = 0;
    USB_INT rewind_before_mass_enable = 0;
    USB_UINT8 port_protocol = USB_IF_PROTOCOL_VOID;
    pnp_dynamic_mode* rewind_before_mode = NULL;
    pnp_dynamic_mode* rewind_after_mode = NULL;
    USB_CHAR* port_name = NULL;

    DBG_I(MBB_PNP, "enter\n");

    /*check daynamic_sn NV */
    pnp_check_daynamic_sn_flag();


    mass_function_cleanup_alllun_info();

    /*配置切换后的端口组合*/
    if (DYNAMIC_REWIND_AFTER_MODE == portindex)
    {
        rewind_after_mode = pnp_dynamic_port_get_mode(portindex);

        /*init the dynamic port mode before config it*/
        pnp_dynamic_port_mode_default(rewind_after_mode);

        rewind_after_mode->PID = usb_nv_info->pid_info.cust_rewind_pid;
        DBG_I(MBB_PNP, "rewind_after_mode  PID:%x\n", rewind_after_mode->PID);

        for (i = 0; i < DYNAMIC_PID_MAX_PORT_NUM; i++)
        {
            port_protocol = usb_nv_info->dev_profile_info.rewind_port_style[i];

            DBG_I(MBB_PNP, "rewind_port_style  %d\n", port_protocol);

            if ( USB_IF_PROTOCOL_VOID == port_protocol)
            {
                DBG_I(MBB_PNP, "rewind_port_style end with %d\n", i);
                break;
            }

            port_name = pnp_dynamic_port_get_port_name(port_protocol);
            pnp_dynamic_port_add_info_to_port_mode(rewind_after_mode, port_name, port_protocol);
        }

        /*config the eth dev according to the OS*/
        if ((MAC_OS_FLAG == g_pnp_info.rewind_param.bPcType)
            || (LINUX_OS_FLAG == g_pnp_info.rewind_param.bPcType))
        {
            pnp_rewind_after_set_mac_linux_mode();
        }
        else
        {
            pnp_rewind_after_set_windows_mode();
        }
    }
    /*配置切换前的端口组合*/
    else if (DYNAMIC_REWIND_BEFORE_MODE == portindex)
    {
        rewind_before_mode = pnp_dynamic_port_get_mode(portindex);

        /*init the dynamic port mode before config it*/
        pnp_dynamic_port_mode_default(rewind_before_mode);

        rewind_before_mode->PID = usb_nv_info->pid_info.cust_first_pid;
        DBG_I(MBB_PNP, "rewind_before_mode  PID:%x\n", rewind_before_mode->PID);

        for (i = 0; i < DYNAMIC_PID_MAX_PORT_NUM; i++)
        {
            port_protocol = usb_nv_info->dev_profile_info.first_port_style[i];

            if ( USB_IF_PROTOCOL_VOID == port_protocol)
            {
                DBG_I(MBB_PNP, "first_port_style end with %d\n", i);
                break;
            }

            switch (port_protocol)
            {
                case USB_IF_PROTOCOL_CDROM:
                    mass_function_add_lun("mass", "CD");

                    if ( 0 == rewind_before_mass_enable)
                    {
                        pnp_dynamic_port_add_info_to_port_mode(
                            rewind_before_mode, "mass", port_protocol);
                    }

                    rewind_before_mass_enable = 1;
                    break;

                case USB_IF_PROTOCOL_SDRAM:
                    mass_function_add_lun("mass", "SD");

                    if ( 0 == rewind_before_mass_enable)
                    {
                        pnp_dynamic_port_add_info_to_port_mode(
                            rewind_before_mode, "mass", port_protocol);
                    }

                    rewind_before_mass_enable = 1;
                    break;

                default:
                    port_name = pnp_dynamic_port_get_port_name(port_protocol);
                    pnp_dynamic_port_add_info_to_port_mode(
                        rewind_before_mode, port_name, port_protocol);
                    break;
            }
        }

        if ( MBB_NV_ENABLE == usb_nv_info->winblue_profile.MBIMEnable)
        {
            g_pnp_info.is_mbim_enable = 1;

            /*不通过procotol配置端口的功能不需要记录protocol*/
            pnp_dynamic_port_add_info_to_port_mode(
                rewind_before_mode, ":", USB_IF_PROTOCOL_VOID);

            pnp_dynamic_port_config_mbim_info();

            for (i = 0; i < CONFIG_TWO_MAX_PORT_NUM; i++)
            {
                if (NULL != config_funcs_win8[i])
                {
                    /*不通过procotol配置端口的功能不需要记录protocol*/
                    pnp_dynamic_port_add_info_to_port_mode(
                        rewind_before_mode, config_funcs_win8[i], USB_IF_PROTOCOL_VOID);
                }
                else
                {
                    break;
                }
            }
        }
    }
    else
    {
        DBG_I(MBB_PNP, "port index %d do not config from NV\n", portindex);
    }
    DBG_I(MBB_PNP, "success\n");
}

/*****************************************************************************
 函 数 名  :
 功能描述  :
 输入参数  :
 输出参数  :
 返 回 值  :
 调用函数  :
 被调函数  :
 修改历史      :
  1.日    期   : 2013年9月17日
    作    者   :
    修改内容   : 新生成函数
*****************************************************************************/

ssize_t pnp_ctl_app_start_flag_read( struct file* file, char __user* buf, size_t size, loff_t* ppos )
{
    ssize_t   len  = 0;
    unsigned long iRet = 0;
    USB_CHAR  mode_buf[APP_MODE_FLAG_LEN] = {0};

    if (APP_MODE_FLAG_LEN < size)
    {
        return 0;
    }

    memset( mode_buf, 0, APP_MODE_FLAG_LEN );
    snprintf( mode_buf, APP_MODE_FLAG_LEN, "%d", g_pnp_info.app_start_mode);
    len = strlen(mode_buf) + 1;

    if ( (APP_MODE_FLAG_LEN == size ) && (0 == *ppos) )
    {
        iRet = copy_to_user( buf, mode_buf, len );

        if (0 != iRet)
        {
            DBG_E(MBB_PNP, "copy_to_user failed.\n");
        }

        DBG_I(MBB_PNP, "gAppStartMode=%u\n", g_pnp_info.app_start_mode);
        *ppos = len;
        return len;
    }
    else
    {
        DBG_I(MBB_PNP, "Invalid param %u.\n", size);
        return 0;
    }
}

ssize_t pnp_ctl_app_start_flag_write( struct file* file, const char __user* buf,
                                      size_t size, loff_t* ppos )
{
    unsigned long iRet = 0;
    USB_CHAR  mode_buf[APP_MODE_FLAG_LEN] = {0};

    memset( mode_buf, 0, APP_MODE_FLAG_LEN );

    iRet = copy_from_user( mode_buf, buf, 1 );

    if (iRet)
    {
        DBG_E(MBB_PNP, "copy_from_user failed err:%ld\n", iRet);
        return -EFAULT;
    }

    DBG_E(MBB_PNP, "%s\n", mode_buf);

    switch (mode_buf[APP_WRITE_DATA_SITE])
    {
        case CTL_APP_START_FALG  :
            pnp_set_ctl_app_flag(CTL_APP_START);
            break;

        case CTL_APP_DISABLE_FALG:
            pnp_set_ctl_app_flag(CTL_APP_DISABLE);
            break;

        case CTL_APP_WAIT_FALG   :
            pnp_set_ctl_app_flag(CTL_APP_WAIT);
            break;

        default:
            return -EFAULT;
    }

    return size;
}

static const struct file_operations app_start_mode_proc_fops =
{
    .owner      = THIS_MODULE,
    .read       = pnp_ctl_app_start_flag_read,
    .write      = pnp_ctl_app_start_flag_write,
};

USB_VOID usb_ctl_app_start_flag_init( USB_VOID )
{
    static struct proc_dir_entry* entry = NULL;

#if defined(USB_E5) || defined(USB_CPE) || defined(USB_WINGLE)
    pnp_set_ctl_app_flag(CTL_APP_START);
#else
    pnp_set_ctl_app_flag(CTL_APP_WAIT);
#endif

    if (NULL == entry)
    {
        entry = proc_create( "gAppStartMode", S_IRUGO | S_IWUSR, NULL, &app_start_mode_proc_fops );
        DBG_I(MBB_PNP, "app_start_flag_proc_init. gAppStartMode=%d, entry=%p \n", g_pnp_info.app_start_mode, entry);
    }
}

/*****************************************************************************
 函 数 名  : pnp_probe
 功能描述  : PNP连接函数，将此函数连接到其他模块可调起PNP。
 输入参数  : 无
 输出参数  :
 返 回 值  : 无
 被调函数  : pnp_probe
 修改历史      :
  1.日    期   : 2014年5月15日
    作    者   : zhangkuo
    修改内容   : 新生成函数
*****************************************************************************/
USB_VOID pnp_probe(USB_VOID)
{
    struct android_dev* dev = android_get_android_dev();
    struct usb_composite_dev* cdev = NULL;
    DBG_I(MBB_PNP, "enter\n");

    /*the HW irq early wait the android_dev create */
    if ( NULL == dev)
    {
        DBG_E(MBB_PNP, "android_get_android_dev NULL!\n");
        return ;
    }

    cdev = dev->cdev;
    /*pnp_probe compete with the pnp_remove ,
      mutex the android_dev , and g_pnp_info */
    mutex_lock(&dev->mutex);
    /*读取NV*/
    usb_nv_get();
    pnp_init_device_descriptor(cdev);
    pnp_status_check_device_status_to_select_port_index();
    mutex_unlock(&dev->mutex);
    DBG_I(MBB_PNP, "success\n");
}

USB_VOID pnp_remove_func(struct work_struct* w)
{
    struct android_dev* dev = android_get_android_dev();

    DBG_I(MBB_PNP, "\n");

    if ( NULL == dev)
    {
        DBG_E(MBB_PNP, "android_get_android_dev NULL!\n");
        return ;
    }

    /*pnp_remove_func compete with the pnp_switch_func ,
      mutex the android_dev , and g_pnp_info */

    mutex_lock(&dev->mutex);
    android_disable(dev);

    pnp_usb_init_enum_stat();

    memset(&g_pnp_info, 0, sizeof(huawei_dynamic_info_st));
    huawei_set_usb_enum_state(USB_ENUM_NONE);
    mutex_unlock(&dev->mutex);
    DBG_I(MBB_PNP, "success\n");
}

/*****************************************************************************
 函 数 名  : pnp_remove
 功能描述  : PNP移除函数，将此函数连接到其他模块可调起PNP。
 输入参数  : 无
 输出参数  :
 返 回 值  : 无
 被调函数  : pnp_remove
 修改历史      :
  1.日    期   : 2014年5月15日
    作    者   : zhangkuo
    修改内容   : 新生成函数
*****************************************************************************/
USB_VOID pnp_remove(USB_VOID)
{
    struct android_dev* dev = android_get_android_dev();

    /*the HW irq early wait the android_dev create */
    if ( NULL == dev)
    {
        DBG_E(MBB_PNP, "android_get_android_dev NULL!\n");
        return ;
    }
    DBG_I(MBB_PNP, "\n");
    /*pnp_probe compete with the pnp_remove ,
      mutex the android_dev , and g_pnp_info */
    //mutex_lock(&dev->mutex);
    pnp_remove_func(&pnp_remove_work);
    //pnp_usb_queue_delay_work(&pnp_remove_work, 0);

    // mutex_unlock(&dev->mutex);

    DBG_I(MBB_PNP, "success\n");
}

/*=============================================================
PNP USB请求功能模块
==============================================================*/

/*****************************************************************************
 函 数 名  :
 功能描述  :
 输入参数  :
 输出参数  :
 返 回 值  :
 调用函数  :
 被调函数  :
 修改历史      :
  1.日    期   : 2014年9月17日
    作    者   :
    修改内容   : 新生成函数
*****************************************************************************/
USB_INT pnp_is_rndis_present(USB_VOID)
{
    struct android_dev* dev = android_get_android_dev();
    struct android_configuration* conf = NULL;
    struct android_usb_function* f;
    USB_INT ret = MBB_USB_FALSE;

    list_for_each_entry(conf, &dev->android_configs, list_item)
    {
        list_for_each_entry(f, &conf->enabled_functions, enabled_list)
        {
            if (!strcmp("rndis", f->name))
            {
                DBG_T(MBB_PNP, "enabled_function have %s", f->name);
                ret = MBB_USB_TRUE;
                break;
            }
        }
    }
    return ret;
}

/*微软文档中截取的containter_ID*/
static USB_UINT8 containter_ID[] =
{
    0x65, 0xe9, 0x36, 0x4d,
    0x25, 0xe3,
    0xce, 0x11,
    0xbf, 0xc1,
    0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18
};

static USB_INT  process_msos_request(const struct  usb_ctrlrequest* msosctrl,
                                     USB_PVOID buf,
                                     USB_INT id)
{
    ms_extend_compact_id_header header;
    ms_extend_container_id* pContainerID = NULL;
    ms_extend_compact_id_function Function;
    ms_extend_container_id containerID;
    USB_INT wLength = 0;
    mbb_usb_nv_info_st* usb_nv_info = usb_nv_get_ctx();

    switch (msosctrl->wIndex)
    {
        case MS_EXTEND_COMPAT_ID:
            DBG_I(MBB_PNP, "--->MS_EXTEND_COMPAT_ID received!\n");
            memset(&header, 0x00, sizeof(ms_extend_compact_id_header));

            header.dwLength = sizeof(ms_extend_compact_id_header);
            /*Version = 1.00*/
            header.bcdVersion = 0x0100;
            header.wIndex = 0x0004;
            header.bCount = 1;
            memset(&Function, 0, sizeof(ms_extend_compact_id_function));
            Function.bFirstInterfaceNumber = 0;
            Function.bInterfaceCount = 0x01;
            memcpy(Function.compatibleID, "ALTRCFG", 7);
            Function.subCompatibleID[0] = ALTRCFG_1;

            if (MBB_NV_ENABLE == usb_nv_info->winblue_profile.MBIMEnable)
            {
                Function.subCompatibleID[0] = ALTRCFG_2;
            }

            if (pnp_is_rndis_present())
            {
                DBG_I(MBB_PNP, "rndis is present!\n");
                memset(&Function.compatibleID, 0, 8);
                memcpy(Function.compatibleID, "RNDIS", 5);
                Function.subCompatibleID[0] = 0;
            }

            memcpy(buf + header.dwLength, &Function, sizeof(ms_extend_compact_id_function));
            header.dwLength += sizeof(ms_extend_compact_id_function);
            memcpy(buf, &header, sizeof(ms_extend_compact_id_header));
            wLength = header.dwLength;
            break;

        case MS_EXTEND_CONTAINER_ID:
            DBG_I(MBB_PNP, "--->MS_EXTEND_CONTAINER_ID received!\n");
            memset(&containerID, 0, sizeof(ms_extend_container_id));
            pContainerID = &containerID;
            memset((uint8_t*)pContainerID, 0x00, sizeof(ms_extend_container_id));
            pContainerID->dwLength = sizeof(ms_extend_container_id);
            pContainerID->bcdVersion = 0x0100;
            pContainerID->wIndex = 6;
            memcpy(pContainerID->containerID, containter_ID, sizeof(containter_ID));
            memcpy(buf, &containerID, sizeof(ms_extend_container_id));
            wLength = pContainerID->dwLength;
            break;

        default:
            break;
    }

    return wLength;
}

USB_INT pnp_huawei_vendor_setup(struct usb_gadget* gadget, const struct usb_ctrlrequest* ctrl)
{
    struct usb_composite_dev*	cdev = get_gadget_data(gadget);
    struct usb_request*		req = NULL;
    usb_enum_state  enum_state   = USB_ENUM_NONE;
    USB_INT         value    = -EOPNOTSUPP;
    USB_UINT16      w_value  = le16_to_cpu(ctrl->wValue);
    USB_UINT16      w_length = le16_to_cpu(ctrl->wLength);

    /* partial re-init of the response message; the function or the
     * gadget might need to intercept e.g. a control-OUT completion
     * when we delegate to it.
     */
    if (NULL  == cdev)
    {
        DBG_E(MBB_PNP, "get_gadget_data return null, error!\n");
        return -EOPNOTSUPP;
    }

    req = cdev->req;
    req->zero = 0;
    req->length = 0;

    switch (ctrl->bRequest)
    {
        case MS_HUAWEI_VENDOR_CODE:
            if (USB_REQ_TYPE_MSOS_IN == ctrl->bRequestType )
            {
                DBG_T(MBB_PNP, "C0 DE recive\n");
                /*此处添加上报COMID*/
                enum_state = huawei_get_usb_enum_state();

                if ( USB_ENUM_DONE != enum_state || pnp_is_rndis_present())
                {
                    value = process_msos_request( ctrl, req->buf, w_value & USB_LOW_MASK);

                    if (value >= 0)
                    {
                        value = min(w_length, (USB_UINT16) value);
                    }
                }
            }

            break;

        case USB_REQ_GET_MSOS_CUSTOM: //windows service installed
            if (USB_REQ_TYPE_MSOS_IN == ctrl->bRequestType)
            {
                DBG_T(MBB_PNP, "C0 EE recive\n");
#ifndef USB_HILINK_MODEM_SWITCH
                if (pnp_port_style == g_pnp_info.current_port_style)
                {
                    g_pnp_info.rewind_param.bCmdReserved = USB_NUM_6;
                    g_pnp_info.rewind_param.bPcType      = WINDOWS_OS_FLAG;
                    g_pnp_info.rewind_param.bTimeOut     = 0x00;
                    g_pnp_info.rewind_param.bPID         = 0x00;
                    g_pnp_info.rewind_param.bNewPID      = 0x00;
                    g_pnp_info.rewind_param.bSupportCD   = 0x00;
                    g_pnp_info.rewind_param.bProFile     = 0x00;
                    g_pnp_info.rewind_param.bGreenMode   = 0x01;
                    g_pnp_info.is_service_switch         = MBB_USB_TRUE;
                }
#endif

                /*此处添加上报COMID*/
                enum_state = huawei_get_usb_enum_state();

                if ( USB_ENUM_DONE != enum_state || pnp_is_rndis_present())
                {
                    value = process_msos_request( ctrl, req->buf, w_value & USB_LOW_MASK);

                    if (value >= 0)
                    {
                        value = min(w_length, (USB_UINT16) value);
                    }
                }
            }
            break;

        case USB_REQ_GET_MSOS_CUSTOM_MAC://mac os service installed
            if ((USB_REQ_TYPE_MSOS_OUT == ctrl->bRequestType))
            {
                DBG_T(MBB_PNP, "40 A1 recive(MAC)\n");

                if (pnp_port_style == g_pnp_info.current_port_style)
                {
                    g_pnp_info.rewind_param.bCmdReserved = USB_NUM_6;
                    g_pnp_info.rewind_param.bPcType      = MAC_OS_FLAG;
                    g_pnp_info.rewind_param.bTimeOut     = 0x00;
                    g_pnp_info.rewind_param.bPID         = 0x00;
                    g_pnp_info.rewind_param.bNewPID      = 0x00;
                    g_pnp_info.rewind_param.bSupportCD   = 0x00;
                    g_pnp_info.rewind_param.bProFile     = 0x00;
                    g_pnp_info.rewind_param.bGreenMode   = 0x01;
                    pnp_switch_rewind_after_mode();
                }
                else
                {
                    DBG_T(MBB_PNP, "can not rewind\n");
                }
            }

            break;

        case USB_REQ_HUAWEI_EXT:
            if ((USB_REQ_TYPE_MSOS_IN == ctrl->bRequestType)
                && (pnp_port_style == g_pnp_info.current_port_style)
                && (0x0100 == (ctrl->wValue & USB_HIGH_MASK)))
            {
                USB_UCHAR os = ctrl->wValue & USB_LOW_MASK;
                DBG_T(MBB_PNP, "C0 9a recive\n");
                {
                    g_pnp_info.rewind_param.bCmdReserved = USB_NUM_6;
                    g_pnp_info.rewind_param.bPcType      = os;
                    g_pnp_info.rewind_param.bTimeOut     = 0x00;
                    g_pnp_info.rewind_param.bPID         = 0x00;
                    g_pnp_info.rewind_param.bNewPID      = ctrl->wIndex & USB_LOW_MASK;
                    g_pnp_info.rewind_param.bSupportCD   = 0x00;
                    g_pnp_info.rewind_param.bProFile     = 0x00;
#ifdef USB_RNDIS
                    g_pnp_info.rewind_param.bGreenMode   = 0x01;
#else
                    g_pnp_info.rewind_param.bGreenMode   = 0x00;
#endif
                    pnp_switch_rewind_after_mode();
                }
            }
            break;
            /*特殊切换命令，支持hilink、modem双模式的产品支持,和C0 EE互斥*/
        case USB_REQ_HILINK_MODEM_SWITCH:
            if (USB_REQ_TYPE_MSOS_IN == ctrl->bRequestType)
            {
                DBG_T(MBB_PNP, "C0 20 recive\n");
#ifdef USB_HILINK_MODEM_SWITCH
                if (pnp_port_style == g_pnp_info.current_port_style)
                {
                    g_pnp_info.rewind_param.bCmdReserved = USB_NUM_6;
                    g_pnp_info.rewind_param.bPcType      = WINDOWS_OS_FLAG;
                    g_pnp_info.rewind_param.bTimeOut     = 0x00;
                    g_pnp_info.rewind_param.bPID         = 0x00;
                    g_pnp_info.rewind_param.bNewPID      = ctrl->wIndex & USB_LOW_MASK;
                    g_pnp_info.rewind_param.bSupportCD   = 0x00;
                    g_pnp_info.rewind_param.bProFile     = 0x00;
                    g_pnp_info.rewind_param.bGreenMode   = 0x01;
                    g_pnp_info.is_service_switch         = MBB_USB_TRUE;
                }
#endif
                /*此处添加上报COMID*/
                enum_state = huawei_get_usb_enum_state();

                if ( USB_ENUM_DONE != enum_state || pnp_is_rndis_present())
                {
                    value = process_msos_request( ctrl, req->buf, w_value & USB_LOW_MASK);

                    if (value >= 0)
                    {
                        value = min(w_length, (USB_UINT16) value);
                    }
                }
            }
            break;

        default:
            break;
    }

    /* respond with data transfer before status phase? */
    if (value >= 0 && value != USB_GADGET_DELAYED_STATUS)
    {
        req->length = value;
        req->zero = value < w_length;
        value = usb_ep_queue(gadget->ep0, req, GFP_ATOMIC);

        if (value < 0)
        {
            DBG_I(MBB_PNP, "ep_queue --> %d\n", value);
            req->status = 0;
            //pnp_huawei_vendor_setup_complete(gadget->ep0, req);
        }
    }
    //else if (value == USB_GADGET_DELAYED_STATUS && w_length != 0)
    //{
    //    DBG_E(MBB_PNP, "Delayed status not supported for w_length != 0");
    //}

    /* device either (value < 0) or reports success */
    return value;
}

/*****************************************************************************
 函 数 名  : set_soft_delay
 功能描述  : 设置软切换延时时间（主要用来调试的接口）
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 无
 被调函数  : ecall/可维可测试
 修改历史      :
  1.日    期   : 2014.3.17
    修改内容   : 新生成函数

*****************************************************************************/
USB_VOID set_soft_delay(int m)
{
    g_soft_delyay = m;
    DBG_E(MBB_PNP, "g_soft_delyay is %d ms\n", g_soft_delyay);
}

/*****************************************************************************
 函 数 名  :   soft_reconnect_handler
 功能描述  : 软件模拟插拔的功能
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 无
 被调函数  : time
 修改历史      :
  1.日    期   : 2014.3.17
    修改内容   : 新生成函数

*****************************************************************************/
USB_VOID soft_reconnect_handler( USB_ULONG data )
{
    struct android_dev* dev = android_get_android_dev();
    struct usb_composite_dev* cdev = NULL;
    usb_enum_state state = USB_ENUM_NONE;
    state = huawei_get_usb_enum_state();

    if ( USB_ENUM_DONE == state )
    {
        DBG_E(MBB_PNP, " soft_reconnect_handler  state:%d \n", state);
        return;
    }

    if (NULL != dev)
    {
        cdev = dev->cdev;
    }
    else
    {
        DBG_E(MBB_PNP, "android_dev NULL!\n");
        return;
    }

    if ((!cdev) || (!cdev->gadget))
    {
        DBG_E(MBB_PNP, "cdev or gadget NULL\n");
        return;
    }

    DBG_I(MBB_PNP, " soft_reconnect_handler process! \n");

#if ( FEATURE_ON == MBB_MLOG )
    mlog_print("USB", mlog_lv_info, "enum timeout\n");   /* SW reliability */
#endif
    usb_gadget_disconnect(cdev->gadget);
    mdelay(g_soft_delyay);
    usb_gadget_connect(cdev->gadget);
}

/**
   如果在setconfig2里直接调用切网关的函数，既断网函数，会导致核间通信失败
   不能成功断开网络，此work回调处理切网关回调。
*/


/****************************************************************
 函 数 名  : usb_pnp_init
 功能描述  : PNP初始化。
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NA
*****************************************************************/
USB_VOID usb_pnp_init(USB_VOID)
{
    usb_adp_ctx_t* UsbCtx = usb_get_adp_ctx();

    memset((USB_VOID*)&g_pnp_info, 0, sizeof(huawei_dynamic_info_st));
    usb_ctl_app_start_flag_init();
    INIT_DELAYED_WORK(&pnp_switch_work, pnp_switch_func);
    INIT_DELAYED_WORK(&pnp_notify_rewind_after_work, pnp_notify_rewind_after_func);
    INIT_DELAYED_WORK(&pnp_remove_work, pnp_remove_func);
    INIT_DELAYED_WORK(&setmode_work, setmode_work_func);
    INIT_DELAYED_WORK(&(UsbCtx->gatway_disconnect_work), gatway_data_disconnect);
    setup_timer(&g_soft_reconnect_timer , soft_reconnect_handler, (USB_ULONG)0);
}

/*******************************************************************************
 *对外接口定义区
 *
 *接口定义必须进行接口说明
 ******************************************************************************/

/*****************************************************************************
 函 数 名  :pnp_register_usb_support_function
 功能描述  :向PNP注册功能驱动
 输入参数  :struct android_usb_function* usb_func:需要注册的功能驱动
 输出参数  :
 返 回 值  :
 调用函数  :
 被调函数  :各功能驱动
 修改历史      :
  1.日    期   : 2014-06-10
    修改内容   : 添加注释
*****************************************************************************/
USB_VOID pnp_register_usb_support_function(struct android_usb_function* usb_func)
{
    USB_UINT index = 0;

    if (NULL == usb_func)
    {
        DBG_E(MBB_PNP, "empty func\n");
        return;
    }

    for (index = 0; index < USB_NUM_30; index++)
    {
        if (NULL == supported_functions[index])
        {
            supported_functions[index] = usb_func;
            DBG_I(MBB_PNP, "reg %s at %u\n", usb_func->name, index);
            break;
        }
    }

    if ( USB_NUM_30 == index )
    {
        DBG_W(MBB_PNP, "list is full\n");
    }
}

/*****************************************************************************
 函 数 名  : get_pnp_info
 功能描述  : 获取PNP状态机
 输入参数  : USB_VOID
 输出参数  : 无
 返 回 值  : USB_INT
 调用函数  :
 被调函数  :
 修改历史      :
  1.日    期   : 2014-06-10
    修改内容   : 添加注释
*****************************************************************************/
void*   get_pnp_info(USB_VOID)
{
    return &g_pnp_info;
}

/*****************************************************************************
 函 数 名  :
 功能描述  :
 输入参数  :
 输出参数  :
 返 回 值  :
 调用函数  :
 被调函数  :
 修改历史      :
  1.日    期   : 2013年9月17日
    作    者   :
    修改内容   : 新生成函数
*****************************************************************************/
#if 0
USB_PUINT8 get_serial_number(USB_PUINT8 serialnumber)
{
    struct android_dev* dev = android_get_android_dev();
    struct usb_composite_dev* cdev = dev->cdev;

    if (NULL != dev)
    {
        cdev = dev->cdev;
    }

    if (cdev->desc.iSerialNumber)
    {
        //memcpy(serialnumber, strings_dev[STRING_SERIAL_IDX].s, 7);
        //serialnumber = strings_dev[STRING_SERIAL_IDX].s;
    }
}
#endif
/*****************************************************************************
 函 数 名  :usb_pnp_port_style_stat
 功能描述  :
 输入参数  :NA
 输出参数  :
 返 回 值  :USB_INT:
 调用函数  :
 被调函数  :
 修改历史      :
  1.日    期   : 2014年6月10日
    修改内容   : 添加注释
*****************************************************************************/
USB_INT  usb_pnp_port_style_stat(USB_VOID)
{
    if (download_port_style == g_pnp_info.current_port_style
        || factory_port_style == g_pnp_info.current_port_style
        || (GATEWAY_OS_FLAG == g_pnp_info.rewind_param.bPcType))
    {
        return MBB_USB_TRUE;
    }
    else
    {
        return MBB_USB_FALSE;
    }
}

/*****************************************************************************
 函 数 名  :pnp_if_gateway_mode
 功能描述  :
 输入参数  :NA
 输出参数  :
 返 回 值  :USB_INT:
 调用函数  :
 被调函数  :
 修改历史      :
  1.日    期   : 2014年6月10日
    修改内容   : 添加注释
*****************************************************************************/
USB_INT  pnp_if_static_port_mode(USB_VOID)
{
    if ( g_pnp_info.portModeIndex < DYNAMIC_REWIND_BEFORE_MODE)
    {
        DBG_I(MBB_PNP, "static_port_mode\n");
        return MBB_USB_TRUE;
    }
    else
    {
        DBG_I(MBB_PNP, "dynamic_port_mode\n");
        return MBB_USB_FALSE;
    }
}

/*****************************************************************************
 函 数 名  :pnp_if_gateway_mode
 功能描述  :
 输入参数  :NA
 输出参数  :
 返 回 值  :USB_INT:
 调用函数  :
 被调函数  :
 修改历史      :
  1.日    期   : 2014年6月10日
    修改内容   : 添加注释
*****************************************************************************/
USB_INT  pnp_if_gateway_mode(USB_VOID)
{
    if ( GATEWAY_OS_FLAG == g_pnp_info.rewind_param.bPcType )
    {
        return MBB_USB_TRUE;
    }
    else
    {
        return MBB_USB_FALSE;
    }
}

/*****************************************************************************
 函 数 名  :usb_pnp_system_type_get
 功能描述  :获取当前USB连接的PC类型
 输入参数  :NA
 输出参数  :
 返 回 值  :USB_INT:
            WINDOWS_OS_FLAG 0x00
            MAC_OS_FLAG     0x10
            LINUX_OS_FLAG   0x20
            GATEWAY_OS_FLAG 0x30
 调用函数  :
 被调函数  :
 修改历史      :
  1.日    期   : 2014年6月10日
    修改内容   : 添加注释
*****************************************************************************/
USB_INT usb_pnp_system_type_get(USB_VOID)
{
    return g_pnp_info.rewind_param.bPcType;
}



/*****************************************************************
Parameters    :  pnp_style
Return        :  TRUE：动态，FALSE：静态
Description   : 判断端口形配置
*****************************************************************/
bool do_dynamic_port_config(USB_INT pnp_style)
{
    return  (pnp_style >= DYNAMIC_REWIND_BEFORE_MODE) ? MBB_USB_TRUE : MBB_USB_FALSE;
}

/*****************************************************************************
 函 数 名  :get_gateway_mode
 功能描述  :获取网关当前上报的端口模式
 输入参数  :NA
 输出参数  :USB_INT
 返 回 值  :GATEWAY_NDIS_MODE 支持NDIS工作模式
            ；GATEWAT_MODEM_MODE:支持modem工作模式
 调用函数  :
 被调函数  :
 修改历史      :
  1.日    期   : 2014年6月10日
    修改内容   : 添加注释
*****************************************************************************/
USB_INT get_gateway_mode(USB_VOID)
{
    if ((GATEWAY_NDIS_MODE == g_pnp_info.rewind_param.bProFile)
        && (OS_TYPE_GATEWAY == g_pnp_info.rewind_param.bPcType))
    {
        return GATEWAY_NDIS_MODE;
    }
    return GATEWAT_MODEM_MODE;
}

/*****************************************************************************
函数名：   BSP_USB_CapabilityThree
功能描述:  提供查询设备USB工作模式接口,AT^USBSPEED使用
输入参数： void
返回值：   OK支持USB3.0，ERROR不支持USB3.0
*****************************************************************************/
USB_INT BSP_USB_CapabilityThree( USB_VOID )
{
#ifdef USB_SUPER_SPEED
    return MBB_USB_OK;
#else
    return MBB_USB_ERROR;
#endif
}

/*****************************************************************************
 函 数 名  : pnp_get_usb_speed
 功能描述  : 获取当前USB速率
 输入参数  : 无
 输出参数  :
 返 回 值  : USB_INT
 修改历史      :
  1.日    期   : 2013年11月13日
    作    者   : zhangkuo
    修改内容   : 新生成函数

*****************************************************************************/
USB_UINT pnp_get_usb_speed(USB_VOID)
{
    struct android_dev* dev = android_get_android_dev();
    USB_UINT speed = 0;

    if (NULL == dev || NULL == dev->cdev
        || NULL == dev->cdev->gadget)
    {
        return 0;
    }

    speed = (USB_UINT)dev->cdev->gadget->speed;
    return speed;
}

/*****************************************************************************
 函 数 名  : usb_speed_work_mode
 功能描述  : 充电模块接口，区别2.0(500mA)和3.0(900mA)充电
 输入参数  : 无
 输出参数  :
 返 回 值  : USB_INT
 修改历史      :
  1.日    期   :
    作    者   : zhangkuo
    修改内容   : 新生成函数

*****************************************************************************/
USB_INT usb_speed_work_mode(USB_VOID)
{
    USB_UINT usb_speed = pnp_get_usb_speed();
    DBG_I(MBB_CHARGER, "usb_speed = 0x%X \n", usb_speed);
    /*if usb3.0 set 900mA,else if invalid usb set 0mA ,else set 500mA*/
    if (USB_SPEED_SUPER == usb_speed)
    {
        DBG_T(MBB_CHARGER, "CHG_CURRENT_SS\n");
        return CHG_CURRENT_SS;
    }
    else if (USB_SPEED_UNKNOWN == usb_speed )
    {
        DBG_T(MBB_CHARGER, "CHG_CURRENT_NO\n");
        return CHG_CURRENT_NO;
    }
    else
    {
        DBG_T(MBB_CHARGER, "CHG_CURRENT_HS\n");
        return CHG_CURRENT_HS;
    }
}

/*****************************************************************************
函数名：   BSP_USB_GetSpeed
功能描述:  提供查询设备USB速率接口
输入参数： char *
返回值：
 修改历史      :
  1.日    期   : 2013年11月13日
    作    者   : zhangkuo
    修改内容   : 新生成函数

*****************************************************************************/
USB_INT BSP_USB_GetSpeed(USB_UCHAR* buf)
{
    USB_UCHAR    speed;
    USB_INT      ret = MBB_USB_ERROR;

    speed = pnp_get_usb_speed();
    DBG_I(MBB_PNP, "USBMODE:%u", speed);

    switch (speed)
    {
        case 0:
            break;

        case 1:
        case 2:
            snprintf(buf, MAX_SPEED_NAME_LEN, "USB1.1");
            ret = MBB_USB_OK;
            break;

        case 3:
            snprintf(buf, MAX_SPEED_NAME_LEN, "USB2.0");
            ret = MBB_USB_OK;
            break;

        case 4:
            snprintf(buf, MAX_SPEED_NAME_LEN, "USB2.5");
            ret = MBB_USB_OK;
            break;

        case 5:
            snprintf(buf, MAX_SPEED_NAME_LEN, "USB3.0");
            ret = MBB_USB_OK;
            break;
    }

    return ret;
}

/*****************************************************************************
 函 数 名  : huawei_get_usb_enum_state
 功能描述  : 获取USB枚举状态
 输入参数  : 无
 输出参数  :
 返 回 值  : USB_INT
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年6月10日
    作    者   : zhangkuo
    修改内容   : 添加注释

*****************************************************************************/
usb_enum_state huawei_get_usb_enum_state( void )
{
    return   g_usb_enum_state ;
}

/*****************************************************************************
 函 数 名  : huawei_set_usb_enum_state
 功能描述  : 设置USB枚举状态
 输入参数  : 无
 输出参数  :
 返 回 值  : USB_INT
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年6月10日
    作    者   : zhangkuo
    修改内容   : 添加注释

*****************************************************************************/
USB_VOID huawei_set_usb_enum_state(usb_enum_state state)
{
    g_usb_enum_state = state;
}

USB_VOID huawei_set_adress_flag(USB_INT state)
{
    if (pnp_is_rewind_before_mode())
    {
        g_set_adress_flag = state;
    }
}

USB_INT huawei_get_adress_flag(USB_VOID)
{
    return   g_set_adress_flag ;
}

USB_INT pnp_is_rewind_before_mode(USB_VOID)
{
    USB_INT ret = MBB_USB_FALSE;
    if ( pnp_port_style == g_pnp_info.current_port_style )
    {
        ret = MBB_USB_TRUE;
    }

    return ret;
}

/*****************************************************************************
/*****************************************************************************
 函 数 名  : pnp_is_multi_lun_mode
 功能描述  : 查询是否支持multi lun
 输入参数  : 无
 输出参数  :
 返 回 值  : USB_INT
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年6月10日
    作    者   : zhangkuo
    修改内容   : 添加注释

*****************************************************************************/
USB_INT pnp_is_multi_lun_mode(USB_VOID)
{
    USB_INT ret = MBB_USB_FALSE;
    if ( DYNAMIC_REWIND_BEFORE_MODE == g_pnp_info.portModeIndex )
    {
        ret = MBB_USB_TRUE;
    }
    return ret;
}

USB_INT pnp_is_service_switch(USB_VOID)
{
    USB_INT ret = MBB_USB_FALSE;
    if ( MBB_USB_TRUE == g_pnp_info.is_service_switch )
    {
        ret = MBB_USB_TRUE;
    }

    return ret;
}

/*****************************************************************************
 函 数 名  : pnp_set_rewind_param
 功能描述  : 设置切换命令
 输入参数  : 无
 输出参数  :
 返 回 值  : USB_INT
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年6月10日
    作    者   : zhangkuo
    修改内容   : 添加注释

*****************************************************************************/
USB_VOID pnp_set_rewind_param(USB_UINT8* cmnd)
{
    memcpy(&(g_pnp_info.rewind_param), cmnd, MAX_REWIND_PARAM_NUM);
}

/*****************************************************************************
 函 数 名  : pnp_set_ctl_app_flag
 功能描述  : 设置app是否启动标志
 输入参数  : 无
 输出参数  :
 返 回 值  : USB_INT
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年6月10日
    作    者   : zhangkuo
    修改内容   : 添加注释

*****************************************************************************/
USB_VOID pnp_set_ctl_app_flag(USB_INT flag)
{
    g_pnp_info.app_start_mode = flag;
}

/*****************************************************************************
 函 数 名  : pnp_is_service_switch
 功能描述  : 查询是否服务切换
 输入参数  : 无
 返 回 值  : USB_INT

 修改历史      :
  1.日    期   : 2014年6月10日
    作    者   : zhangkuo
    修改内容   : 添加注释

*****************************************************************************/
USB_INT pnp_if_cdrom_can_open(USB_VOID)
{
    USB_INT ret = MBB_USB_FALSE;
    if ( g_pnp_info.current_port_style <= rewind_port_style)
    {
        ret = MBB_USB_TRUE;
    }

    return ret;
}

