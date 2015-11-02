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
                                                                         单板收到拔出中断，销毁gadget，引起空指针
******************************************************************************************************/
#include "product_config.h"
#include "drv_usb.h"
#include "bsp_usb.h"
#include "bsp_sram.h"
#include "hi_uart.h"
#include "bsp_pmu.h"
#include "usb_vendor.h"

#if USB_IS_SUPPORT_OM_DUMP
#include "bsp_dump.h"
#endif

#if USB_IS_SUPPORT_NV
#include "drv_nvim.h"
#include "bsp_nvim.h"
#include "drv_nv_id.h"
#endif

#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/module.h>
#include <linux/export.h>
#include <linux/delay.h>
#include <linux/fcntl.h>
#include <linux/namei.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/syscalls.h>
#include <linux/wakelock.h>
#include <linux/regulator/consumer.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>

#include <asm/mach/irq.h>
#include "usb_charger_manager.h"
#include "usb_platform_comm.h"
#include <linux/mlog_lib.h>
#include "usb_hotplug.h"
#if (MBB_CHG_EXTCHG == FEATURE_ON)
#include "usb_otg_dev_detect.h"
#endif

#define USBID_GPIO_NO   (GPIO_0_17)

#define GPIO_OTG_ID_DET      OTG_ID_DET_GPIO
#define GPIO_OTG_ID_SET      OTG_ID_SET_GPIO
#define GPIO_DMDP_CONNECT    DMDP_CONNECT_GPIO



#define GPIO_HIGH 1
#define GPIO_LOW 0


usb_dbg_info_t g_usb_dbg_info = {{0}, 0, 0};
//static unsigned g_usb_enum_done_cur = 0;
//static unsigned g_usb_enum_dis_cur = 0;
#define MAX_EPIN_NUM            15
#define MAX_EPOUT_NUM           15

#define PMU_DETECT_DELAY        150/*PMU中断检测延时*/

#ifdef CONFIG_USB_OTG_USBID_BYGPIO
extern int otg_set_usbid(int usb_id);
#else
#define otg_set_usbid
#endif
extern int bsp_usb_is_support_usbid_bygpio(void);
extern int bsp_usb_is_support_vbus_bycharger(void);
extern int usb_prealloc_eth_rx_mem(void);
extern int is_in_dload ;

int recv_vbus_on_intr = 0;

/*
 * usb adapter for charger
 */
typedef struct usb_notifer_ctx
{
    int usb_start_inboot;
    int usbid_gpio_no;
    int usbid_gpio_state;
    char last_cdev_name[USB_CDEV_NAME_MAX];
    unsigned stat_reg_pmu_cb_fail;
    unsigned stat_usb_id_init_fail;
    unsigned stat_wait_cdev_created;
    struct workqueue_struct* usb_notify_wq;
    struct delayed_work usb_notify_wk;
} usb_notifer_ctx_t;

static usb_notifer_ctx_t g_usb_notifier;

/*
 * usb enum done management
 */

static usb_enum_stat_t g_usb_devices_enum_stat[USB_ENABLE_CB_MAX];
static unsigned g_usb_dev_enum_done_num;
static unsigned g_usb_dev_setup_num;
//static spinlock_t g_usb_dev_enum_lock;


/*
 * usb enum done management implement
 */
void bsp_usb_init_enum_stat(void)
{
    //spin_lock_init(&g_usb_dev_enum_lock);
    g_usb_dev_enum_done_num = 0;
    g_usb_dev_setup_num = 0;
    memset(g_usb_devices_enum_stat, 0, sizeof(g_usb_devices_enum_stat));
}

void bsp_usb_add_setup_dev_fdname(unsigned intf_id, char* fd_name)
{
    if (g_usb_dev_setup_num >= USB_ENABLE_CB_MAX)
    {
        printk("%s error, setup_num:%d, USB_ENABLE_CB_MAX:%d\n",
               __FUNCTION__, g_usb_dev_setup_num, USB_ENABLE_CB_MAX);
        return;
    }
    g_usb_devices_enum_stat[g_usb_dev_setup_num].usb_intf_id = intf_id;
    g_usb_devices_enum_stat[g_usb_dev_setup_num].fd_name = fd_name;
    g_usb_dev_setup_num++;
}

int bsp_usb_is_all_enum(void)
{
    return (g_usb_dev_setup_num != 0 && g_usb_dev_setup_num == g_usb_dev_enum_done_num);
}

int bsp_usb_is_all_disable(void)
{
    return (g_usb_dev_setup_num != 0 && 0 == g_usb_dev_enum_done_num);
}

void bsp_usb_set_enum_stat(unsigned intf_id, int enum_stat)
{
    unsigned int i;
    usb_enum_stat_t* find_dev = NULL;

    if (enum_stat)
    {
        /* if all dev is already enum, do nothing */
        if (bsp_usb_is_all_enum())
        {
            return;
        }

        for (i = 0; i < g_usb_dev_setup_num; i++)
        {
            if (g_usb_devices_enum_stat[i].usb_intf_id == intf_id)
            {
                find_dev = &g_usb_devices_enum_stat[i];
            }
        }
        if (find_dev)
        {
            /* if the dev is already enum, do nothing */
            if (find_dev->is_enum)
            {
                return;
            }
            find_dev->is_enum = enum_stat;
            g_usb_dev_enum_done_num++;

            /* after change stat, if all dev enum done, notify callback */
            if (bsp_usb_is_all_enum())
            {
                bsp_usb_status_change(USB_BALONG_ENUM_DONE);
            }
        }
    }
    else
    {
        if (bsp_usb_is_all_disable())
        {
            return;
        }

        for (i = 0; i < g_usb_dev_setup_num; i++)
        {
            if (g_usb_devices_enum_stat[i].usb_intf_id == intf_id)
            {
                find_dev = &g_usb_devices_enum_stat[i];
            }
        }
        if (find_dev)
        {
            /* if the dev is already disable, do nothing */
            if (!find_dev->is_enum)
            {
                return;
            }
            find_dev->is_enum = enum_stat;

            /* g_usb_dev_enum_done_num is always > 0,
             * we protect it in check bsp_usb_is_all_disable
             */
            if (g_usb_dev_enum_done_num > 0)
            {
                g_usb_dev_enum_done_num--;
            }

            /* if the version is not support pmu detect
             * and all the device is disable, we assume that the usb is remove,
             * so notify disable callback, tell the other modules
             * else, we use the pmu remove detect.
             */
            if (!(usb_get_hotplug_status() == USB_BALONG_DEVICE_REMOVE ||
                  usb_get_hotplug_status() == USB_BALONG_DEVICE_DISABLE)
                && bsp_usb_is_all_disable())
            {
                bsp_usb_status_change(USB_BALONG_DEVICE_DISABLE);
            }
        }
    }

    return;
}

void bsp_usb_enum_info_dump(void)
{
    unsigned int i;

    printk("balong usb is enum done:%d\n", bsp_usb_is_all_enum());
    printk("setup_num:%d, enum_done_num:%d\n",
           g_usb_dev_setup_num, g_usb_dev_enum_done_num);
    printk("device enum info details:\n\n");
    for (i = 0; i < g_usb_dev_setup_num; i++)
    {
        printk("enum dev:%d\n", i);
        printk("usb_intf_id:%d\n", g_usb_devices_enum_stat[i].usb_intf_id);
        printk("is_enum:%d\n", g_usb_devices_enum_stat[i].is_enum);
        printk("fd_name:%s\n\n", g_usb_devices_enum_stat[i].fd_name);
    }
}


static struct regulator* dwc3_vcc;
int power_on_dwc3_usb(void)
{
    return regulator_enable(dwc3_vcc);
}

int power_off_dwc3_usb(void)
{
    return regulator_disable(dwc3_vcc);
}

bool bsp_usb_host_state_get(void)
{
    usb_notifer_ctx_t* usb_notifier = &g_usb_notifier;
    unsigned int value;

    if (!bsp_usb_is_support_usbid_bygpio())
    {
        return 0;
    }

    value = gpio_get_value(usb_notifier->usbid_gpio_no);
    return (0 == value);
}


static irqreturn_t bsp_usb_id_irq(int irq, void* data)
{
    usb_notifer_ctx_t* usb_notifier = (usb_notifer_ctx_t*)data;

    gpio_int_mask_set(usb_notifier->usbid_gpio_no);
    gpio_int_state_clear(usb_notifier->usbid_gpio_no);

    return IRQ_WAKE_THREAD;
}

static irqreturn_t bsp_usb_id_thread(int irq, void* data)
{
    usb_notifer_ctx_t* usb_notifier = (usb_notifer_ctx_t*)data;
    unsigned int value = 0;
    unsigned int changed = 0;

    value = gpio_get_value(usb_notifier->usbid_gpio_no);
    changed = (unsigned int)(value != usb_notifier->usbid_gpio_state);

    usb_notifier->usbid_gpio_state = value;
    gpio_int_trigger_set(usb_notifier->usbid_gpio_no, value ? IRQ_TYPE_LEVEL_LOW : IRQ_TYPE_LEVEL_HIGH);
    gpio_int_state_clear(usb_notifier->usbid_gpio_no);
    gpio_int_unmask_set(usb_notifier->usbid_gpio_no);

    if (changed)
    {
        USB_DBG_VENDOR("%s:gpio value %d,%s\n", __FUNCTION__, value, changed ? "changed" : "no change");
        if (!value)
        {
            usb_wake_lock();
        }

        bsp_usb_status_change_ex(value ? USB_BALONG_PERIP_REMOVE : USB_BALONG_PERIP_INSERT);
    }

    return IRQ_HANDLED;
}

static int bsp_usb_id_register(void)
{
    usb_notifer_ctx_t* usb_notifier  = &g_usb_notifier;
    int ret;
    int gpio = USBID_GPIO_NO;

    if (!bsp_usb_is_support_usbid_bygpio())
    {
        return 0;
    }

    usb_notifier->usbid_gpio_no = USBID_GPIO_NO;

    gpio_int_mask_set(gpio);

    ret = gpio_get_value(gpio);
    if (ret)
    {
        gpio_int_trigger_set(gpio, IRQ_TYPE_LEVEL_LOW);
        usb_notifier->usbid_gpio_state = 1;
    }
    else
    {
        gpio_int_trigger_set(gpio, IRQ_TYPE_LEVEL_HIGH);
        usb_notifier->usbid_gpio_state = 0;
    }

    gpio_set_function(gpio, GPIO_INTERRUPT);

    ret = request_threaded_irq(gpio_to_irq(gpio), bsp_usb_id_irq, bsp_usb_id_thread,
                               IRQF_SHARED, "usbid_task", usb_notifier);
    if (ret)
    {
        return ret;
    }

    gpio_int_state_clear(gpio);
    gpio_int_unmask_set(gpio);

    return 0;
}

static void bsp_usb_id_unregister(void)
{
    int gpio = USBID_GPIO_NO;
    usb_notifer_ctx_t* usb_notifier  = &g_usb_notifier;

    if (!bsp_usb_is_support_usbid_bygpio())
    {
        return ;
    }

    free_irq(gpio_to_irq(gpio), usb_notifier);
    gpio_int_state_clear(gpio);
    gpio_int_unmask_set(gpio);

    return ;
}

static void bsp_usb_pmu_insert_detect(void* ctx)
{

    recv_vbus_on_intr = 1;

    /* when otg-host supply vbus,pmu intr is obmit */
    if (bsp_usb_is_support_vbus_bycharger())
    {
        if (USB_BALONG_PERIP_INSERT == usb_get_hotplug_old_status())
        {
            return ;
        }
    }

    /* if peripheral is inserted in otg mode,pmu intr is unexpected. */
    if (bsp_usb_host_state_get())
    {
        printk("pmu insert event in otg host mode.\n");
        return ;
    }
#ifdef CONFIG_USB_OTG
#if (MBB_CHG_EXTCHG == FEATURE_ON)
    if(GPIO_LOW == otg_dev_id_state())
    {
        printk("it is in otg host mode.\n");
        return ;
    }
#endif	
#endif


    /* need to check whether usb is connected, as the PMU could
     * report the false event during rapid inserting & pluging
     */
    if (!bsp_pmu_usb_state_get())
    {
        printk(KERN_WARNING "false insert event from PMU detected\n");
        return;
    }

    printk(KERN_DEBUG "DWC3 USB: pmu insert event received\n");
    if (bsp_usb_is_support_pmu_detect())
    {
        /* usb gadget driver will catch the insert event if USB_PMU_DETECT not defined */
        bsp_usb_status_change(USB_BALONG_DEVICE_INSERT);
    }
}

static void bsp_usb_pmu_remove_detect(void* ctx)
{
    recv_vbus_on_intr = 0;
    /* when otg-host supply vbus,pmu intr is obmit */
    if (bsp_usb_is_support_vbus_bycharger())
    {
        if (USB_BALONG_PERIP_INSERT == usb_get_hotplug_old_status())
        {
            return ;
        }
    }

    /* if peripheral is inserted in otg mode,pmu intr is unexpected. */
    if (bsp_usb_host_state_get())
    {
        printk("pmu remove event in otg host mode.\n");
        return ;
    }
#ifdef CONFIG_USB_OTG
#if (MBB_CHG_EXTCHG == FEATURE_ON)
    if (GPIO_LOW == otg_dev_id_state())
    {
        printk("-----otg online-----\n");
        return ;
    }
#endif
#endif
    /* need to check whether usb is connected, as the PMU could
     * report the false event during rapid inserting & pluging
     */
    if (bsp_pmu_usb_state_get())
    {
        printk(KERN_WARNING "false remove event from PMU detected");
        return;
    }

    printk(KERN_DEBUG "DWC3 USB: pmu remove event received\n");
    if (bsp_usb_is_support_pmu_detect())
    {
        /* usb gadget driver will catch the remove event if USB_PMU_DETECT not defined */
        bsp_usb_status_change(USB_BALONG_DEVICE_REMOVE);
    }
}

static int bsp_usb_detect_init(void)
{
    int ret = 0;

    recv_vbus_on_intr = 0;
    /* reg usb insert/remove detect callback */
    if (bsp_pmu_irq_callback_register(PMU_INT_USB_IN,
                                      bsp_usb_pmu_insert_detect, &g_usb_notifier))
    {
        g_usb_notifier.stat_reg_pmu_cb_fail++;
    }
    if (bsp_pmu_irq_callback_register(PMU_INT_USB_OUT,
                                      bsp_usb_pmu_remove_detect, &g_usb_notifier))
    {
        g_usb_notifier.stat_reg_pmu_cb_fail++;
    }

    if (bsp_usb_id_register())
    {
        g_usb_notifier.stat_usb_id_init_fail++;
        bsp_usb_id_unregister();
    }

    /* check whether the usb is connected in boot time */
    if (bsp_pmu_usb_state_get()
        || bsp_usb_host_state_get())
    {
        USB_DBG_VENDOR("usb is connect in boot\n");
        /* pmu would not report insert event when board reset with power on */
        usb_wake_unlock();
        ret = bsp_usb_host_state_get() ? 2 : 1;
    }
    else
    {
        USB_DBG_VENDOR("usb is not connect in boot\n");
        ret = 0;
    }

    if (!bsp_usb_is_support_pmu_detect())
    {
        printk("usb is always init in boot\n");
        ret = 1;
    }

    /* get the usb supply to prepare to power on */
    dwc3_vcc = regulator_get(NULL, "dwc3_usb-vcc");
    if (IS_ERR(dwc3_vcc))
    {
        printk(KERN_ERR "dwc3_usb-vcc regulator_get error:%d!/n", (int)PTR_ERR(dwc3_vcc));
        return 0;
    }

    if (power_on_dwc3_usb())
    {
        printk(KERN_ERR "failed to try to enable dwc3_usb-vcc error!/n");
        return 0;
    }

    power_off_dwc3_usb();

    return ret;
}



void bsp_usb_set_last_cdev_name(char* cdev_name)
{
    if (cdev_name)
    {
        strncpy(g_usb_notifier.last_cdev_name, cdev_name, USB_CDEV_NAME_MAX);
    }
}

void bsp_usb_clear_last_cdev_name(void)
{
    g_usb_notifier.last_cdev_name[0] = 0;
}

void bsp_usb_wait_cdev_created(void)
{
    mm_segment_t old_fs;

    /* if there are no cdev, do nothing */
    if (strlen(g_usb_notifier.last_cdev_name) == 0)
    {
        return;
    }
    old_fs = get_fs();
    set_fs(KERNEL_DS);
    while (sys_faccessat(AT_FDCWD, g_usb_notifier.last_cdev_name, O_RDWR))
    {
        g_usb_notifier.stat_wait_cdev_created++;
        msleep(100);
    }
    set_fs(old_fs);
}



int bsp_usb_is_support_charger(void)
{
#if defined(CONFIG_USB_CHARGER_DETECT)
    return 1;
#else
    return 0;
#endif
}

int bsp_usb_is_support_pmu_detect(void)
{
#if defined(CONFIG_USB_PMU_DETECT)
    if (is_in_dload) //进入升级模式不检测PMU
    {
        return 0;
    }
    else
    {
        return 1;
    }
#else
    return 0;
#endif
}

int bsp_usb_is_support_vbus_bycharger(void)
{
#if defined(CONFIG_USB_OTG_VBUS_BYCHARGER)
    return 1;
#else
    return 0;
#endif
}

int bsp_usb_is_support_usbid_bygpio(void)
{
#if defined(CONFIG_USB_OTG_USBID_BYGPIO)
    return 1;
#else
    return 0;
#endif
}
int  bsp_usb_usbid_proc()
{
#ifdef CONFIG_USB_OTG_USBID_BYGPIO
    if (bsp_usb_is_support_usbid_bygpio())
    {
        otg_set_usbid(0);
    }
#endif
}

void bsp_usb_notifier_dump(void)
{

    printk("usb_start_inboot:           %d\n", g_usb_notifier.usb_start_inboot);
    printk("stat_wait_cdev_created:     %d\n", g_usb_notifier.stat_wait_cdev_created);
    printk("last_cdev_name:             %s\n", g_usb_notifier.last_cdev_name);
    printk("stat_reg_pmu_cb_fail:       %d\n", g_usb_notifier.stat_reg_pmu_cb_fail);
}

int usb_notifier_mem_dump(char* buffer, unsigned int buf_size)
{
    unsigned int need_size = sizeof(g_usb_notifier);

    if (need_size > buf_size)
    {
        return -1;
    }
    memcpy(buffer, &g_usb_notifier, need_size);
    return (int)need_size;
}



void bsp_usb_dbg_init(void)
{
#if USB_IS_SUPPORT_NV
    g_usb_dbg_info.nv_key = USB_DBG_NV;

    if (NVM_Read(USB_DBG_NV,
                 &g_usb_dbg_info.dbg_info, sizeof(g_usb_dbg_info.dbg_info)))
    {
        g_usb_dbg_info.stat_nv_read_fail++;
    }
#endif
}

void bsp_usb_dbg_dump(void)
{
#if USB_IS_SUPPORT_NV
    printk("usb dbg dump info       :\n");
    printk("usb dbg module          :0x%08x\n", g_usb_dbg_info.dbg_info.dbg_module);
    printk("usb dbg nv_key          :0x%08x\n", g_usb_dbg_info.nv_key);
    printk("usb nv read fail count  :%d\n", g_usb_dbg_info.stat_nv_read_fail);
#endif
}

void bsp_usb_dbg_set(u32 value)
{
    g_usb_dbg_info.dbg_info.dbg_module = value;
}
#if USB_IS_SUPPORT_NV
/*
 * usb adapter for NV
 */

/* NV structure */
typedef struct
{
    char wwan_flag;
    char phy_auto_powerdown_flag;
    char reserved2;
    char reserved3;
    char reserved4;
    char reserved5;
    char reserved6;
    char reserved7;
} win7_feature_t;


typedef struct
{
    /* usb nv structure ... */
    win7_feature_t win7_feature;
    unsigned int   shell_lock;

    /* stat counter */
    unsigned int stat_nv_read_fail;
} usb_nv_t;

/* USB NV ID */
#define USB_WIN7_FEATURE_NV                 NV_ID_DRV_WIN7_FEATURE
#define USB_ITEM_AT_SHELL_OPEN_FLAG         NV_ID_DRV_AT_SHELL_OPNE

static usb_nv_t g_usb_nv;

static void bsp_usb_nv_default_init(void)
{
    memset(&g_usb_nv, 0, sizeof(usb_nv_t));
}

int bsp_usb_is_support_wwan(void)
{
    return (int)(!!g_usb_nv.win7_feature.wwan_flag);
}

int bsp_usb_is_support_phy_apd(void)
{
    return (int)(!!g_usb_nv.win7_feature.phy_auto_powerdown_flag);
}

int bsp_usb_is_support_shell(void)
{
    int ret;
    if ((g_usb_nv.shell_lock == 0) && (SRAM_SUPPORT_ASHELL_ADDR == PRT_FLAG_EN_MAGIC_A))
    {
        ret = 1;
    }
    else
    {
        ret = 0;
    }
    return ret;


}

void bsp_usb_nv_dump(void)
{
    printk("usb read nv fail count: %d\n", g_usb_nv.stat_nv_read_fail);
    printk("is support wwan:        %d\n", g_usb_nv.win7_feature.wwan_flag);
    printk("is shell lock:          %d\n", g_usb_nv.shell_lock);
    return;
}

void bsp_usb_nv_init(void)
{
    /* init the default value */
    bsp_usb_nv_default_init();

    /*
     * read needed usb nv items from nv
     * ignore the nv_read return value,
     * if fail, use the default value
     */
    if (NVM_Read(USB_WIN7_FEATURE_NV,
                 &g_usb_nv.win7_feature, sizeof(g_usb_nv.win7_feature)))
    {
        g_usb_nv.stat_nv_read_fail++;
    }
    if (NVM_Read(USB_ITEM_AT_SHELL_OPEN_FLAG,
                 &g_usb_nv.shell_lock, sizeof(g_usb_nv.shell_lock)))
    {
        g_usb_nv.stat_nv_read_fail++;
    }
}
#else
int bsp_usb_is_support_wwan(void)
{
    return 0;
}

int bsp_usb_is_support_shell(void)
{
    return 0;
}

void bsp_usb_nv_init(void)
{
    return;
}
#endif /* USB_IS_SUPPORT_NV */

#if 0
void usb_test_enable_cb(void)
{
    struct file* filp;

    filp = filp_open("/dev/ttyGS0", O_RDWR, 0);
    if (IS_ERR(filp))
    {
        printk("open dev fail: %d\n", (u32)filp);
        return;
    }
    printk("open dev ok\n");
}
#endif

#if USB_IS_SUPPORT_OM_DUMP
/*
 * for usb om
 */
extern int perf_caller_mem_dump(char* buffer, unsigned int buf_size);
extern int acm_cdev_mem_dump(char* buffer, unsigned int buf_size);
extern int acm_serial_mem_dump(char* buffer, unsigned int buf_size);
extern int ncm_eth_mem_dump(char* buffer, unsigned int buf_size);


typedef int(*usb_adp_dump_func_t)(char* buffer, unsigned int buf_size);

struct usb_dump_func_info
{
    char* mod_name;
    usb_adp_dump_func_t dump_func;
};

struct usb_dump_info_ctx
{
    char* dump_buffer;
    char* cur_pos;
    unsigned int buf_size;
    unsigned int left;
};


/* mod_name must be 4 bytes, we just copy 4 bytes to dump buffer */
struct usb_dump_func_info usb_dump_func_tb[] =
{
    {"perf", perf_caller_mem_dump},
    {"ntfy", usb_notifier_mem_dump},
    {"acmc", acm_cdev_mem_dump},
    {"acms", acm_serial_mem_dump},
    {"ncm_", ncm_eth_mem_dump},

    {"last", NULL}
};

#define USB_ADP_DUMP_FUNC_NUM \
    (sizeof(usb_dump_func_tb) / sizeof(struct usb_dump_func_info) - 1)

#define USB_ADP_GET_4BYTE_ALIGN(size) (((size) + 3) & (~0x3))

struct usb_dump_info_ctx usb_dump_ctx = {0};


void bsp_usb_adp_dump_hook(void)
{
    unsigned int i;
    int ret;
    char* cur_pos = usb_dump_ctx.dump_buffer;
    unsigned int left = usb_dump_ctx.buf_size;

    /* do nothing, if om init fail */
    if (0 == left || NULL == cur_pos)
    { return; }

    for (i = 0; i < USB_ADP_DUMP_FUNC_NUM; i++)
    {
        if (left <= 4)
        {
            break;
        }
        /* mark the module name, just copy 4 bytes to dump buffer */
        memcpy(cur_pos, usb_dump_func_tb[i].mod_name, 4);
        cur_pos += 4;
        left -= 4;

        if (usb_dump_func_tb[i].dump_func)
        {
            ret = usb_dump_func_tb[i].dump_func(cur_pos, left);
            /* may be no room left */
            if (ret < 0)
            {
                break;
            }

            /* different module recorder addr must be 4bytes align */
            ret = USB_ADP_GET_4BYTE_ALIGN(ret);
            cur_pos += ret;
            left -= ret;
        }
        /* NULL func is the last */
        else
        {
            break;
        }
    }

    usb_dump_ctx.cur_pos = cur_pos;
    usb_dump_ctx.left = left;
    return;
}

void bsp_usb_om_dump_init(void)
{
    /* reg the dump callback to om */
    if (bsp_dump_register_hook(DUMP_SAVE_MOD_USB,
                               (dump_save_hook)bsp_usb_adp_dump_hook))
    {
        goto err_ret;
    }

    if (bsp_dump_get_buffer(DUMP_SAVE_MOD_USB,
                            &usb_dump_ctx.dump_buffer, &usb_dump_ctx.buf_size))
    {
        goto err_ret;
    }

    return;

err_ret:
    usb_dump_ctx.dump_buffer = NULL;
    usb_dump_ctx.buf_size = 0;
    return;
}

void bsp_usb_om_dump_info(void)
{
    printk("buf_size:               %d\n", usb_dump_ctx.buf_size);
    printk("left:                   %d\n", usb_dump_ctx.left);
    printk("dump_buffer:            0x%x\n", (unsigned)usb_dump_ctx.dump_buffer);
    printk("cur_pos:                0x%x\n", (unsigned)usb_dump_ctx.cur_pos);
    return;
}
#else /* USB_IS_SUPPORT_OM_DUMP */
void bsp_usb_om_dump_init(void)
{
    return;
}
#endif /* USB_IS_SUPPORT_OM_DUMP */

/* we don't need exit for adapter module */
//int __init bsp_usb_adapter_init(void)
int bsp_usb_adapter_init(void)
{
    int ret = 0;
    int need_init = 0;
#if (MBB_CHG_EXTCHG == FEATURE_ON)
    int id_state = 0;
#endif

    /* init usb needed nv */
    bsp_usb_nv_init();

    (void)dwc3_phy_auto_powerdown((int)(g_usb_nv.win7_feature.phy_auto_powerdown_flag));

    /* init om dump for usb */
    bsp_usb_om_dump_init();

    /* init detect */
    need_init = bsp_usb_detect_init();
#if (MBB_CHG_EXTCHG == FEATURE_ON)
    id_state = otg_dev_id_state();
#endif

    /* if usb is connect, init the usb core */
    if (need_init)
    {
        g_usb_notifier.usb_start_inboot = 1;
        recv_vbus_on_intr = 1;
#if (MBB_CHG_EXTCHG == FEATURE_ON)
        if (GPIO_HIGH == id_state )
        {
            bsp_usb_status_change(
                1 == need_init ? USB_BALONG_DEVICE_INSERT : USB_BALONG_PERIP_INSERT);
        }
#else
        bsp_usb_status_change(
            1 == need_init ? USB_BALONG_DEVICE_INSERT : USB_BALONG_PERIP_INSERT);
#endif
    }

    /* test for enable cb */
    /* BSP_USB_RegUdiEnableCB(usb_test_enable_cb); */
    /* usb console init */
    //bsp_usb_console_init();
    return ret;
}
//module_init(bsp_usb_adapter_init);


