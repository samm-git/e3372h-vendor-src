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
#ifndef __USB_VENDOR_H__
#define __USB_VENDOR_H__

#ifdef __cplusplus
extern "C" { /* allow C++ to use these headers */
#endif /* __cplusplus */

#include <linux/notifier.h>

#define USB_NOTIF_PRIO_ADP      0           /* adp has lowest priority */
#define USB_NOTIF_PRIO_FD       100         /* function drvier */
#define USB_NOTIF_PRIO_CORE     200         /* usb core */
#define USB_NOTIF_PRIO_HAL      300         /* hardware has highest priority */

/*
 * usd to support usb event notify
 * connect pc:
 * insert(1) -> chargertype iden (2) -> enum_done(3) -> remove(0)/disable(-1) +
 *   ^___________________________________________________________|
 *
 * connect charger (must pmu support):
 * insert(1) -> remove(0) +
 *   ^____________________|
 *
 * connect peripheral(such as udisk):
 * perip_insert(4) -> perip_remove(5)
 *   ^_______________________^
 */
#define USB_BALONG_DEVICE_INSERT    1
#define USB_BALONG_CHARGER_IDEN     2
#define USB_BALONG_ENUM_DONE        3
#define USB_BALONG_PERIP_INSERT     4
#define USB_BALONG_PERIP_REMOVE     5
#define USB_BALONG_DEVICE_REMOVE    0
/* if the version is not support pmu detect
 * and all the device is disable, we assume that the usb is remove,
 */
#define USB_BALONG_DEVICE_DISABLE   (0xF1)

#define USB_BALONG_HOTPLUG_IDLE     0
#define USB_BALONG_HOTPLUG_INSERT   1
#define USB_BALONG_HOTPLUG_REMOVE   2

/*
 * charger type define
 */
#define USB_CHARGER_TYPE_HUAWEI         1
#define USB_CHARGER_TYPE_NOT_HUAWEI     2
#define USB_CHARGER_TYPE_INVALID        0

#define USB_IS_SUPPORT_NV               1
#define USB_IS_SUPPORT_OM_DUMP          1
#define USB_IS_SUPPORT_PMU              1
#define USB_IS_SUPPORT_VSHELL           1
#define USB_IS_SUPPORT_WAKELOCK         1
#define USB_IS_SUPPORT_DYN_INIT         1


#define USB_FD_DEVICE_MAX               32
#define USB_CDEV_NAME_MAX               64

#define GNET_MAX_NUM       (4)
#define GNET_USED_NUM      (1)


#if !defined(CONFIG_PC_USE_HUAWEI_DRIVER)
#define USB_IF_SUBCLASS_BALONG      0x02
#else /* PC_USE_HUAWEI_DRIVER */
#define USB_IF_SUBCLASS_BALONG      0x03
#endif


typedef enum tagUSB_PID_UNIFY_IF_PROT_T
{
    USB_IF_PROTOCOL_VOID             = 0x00,
    USB_IF_PROTOCOL_3G_MODEM         = 0x01,
    USB_IF_PROTOCOL_3G_PCUI          = 0x02,
    USB_IF_PROTOCOL_3G_DIAG          = 0x03,
    USB_IF_PROTOCOL_PCSC             = 0x04,
    USB_IF_PROTOCOL_3G_GPS           = 0x05,
    USB_IF_PROTOCOL_CTRL             = 0x06,
    USB_IF_PROTOCOL_3G_NDIS          = 0x07,
    USB_IF_PROTOCOL_NDISDATA         = 0x08,
    USB_IF_PROTOCOL_NDISCTRL         = 0x09,
    USB_IF_PROTOCOL_BLUETOOTH        = 0x0A,
    USB_IF_PROTOCOL_FINGERPRINT      = 0x0B,
    USB_IF_PROTOCOL_ACMCTRL          = 0x0C,
    USB_IF_PROTOCOL_MMS              = 0x0D,
    USB_IF_PROTOCOL_3G_PCVOICE       = 0x0E,
    USB_IF_PROTOCOL_DVB              = 0x0F,
    USB_IF_PROTOCOL_MODEM            = 0x10,
    USB_IF_PROTOCOL_NDIS             = 0x11,
    USB_IF_PROTOCOL_PCUI             = 0x12,
    USB_IF_PROTOCOL_DIAG             = 0x13,
    USB_IF_PROTOCOL_GPS              = 0x14,
    USB_IF_PROTOCOL_PCVOICE          = 0x15,
    USB_IF_PROTOCOL_NCM              = 0x16,
    USB_IF_PROTOCOL_CDROM            = 0xA1,
    USB_IF_PROTOCOL_SDRAM            = 0xA2,
    USB_IF_PROTOCOL_RNDIS            = 0xA3,

    USB_IF_PROTOCOL_NOPNP            = 0xFF
} USB_PID_UNIFY_IF_PROT_T;


typedef struct usb_enum_stat {
    char* fd_name;                  /* function drvier file name */
    unsigned usb_intf_id;           /* usb interface id */
    unsigned is_enum;               /* whether the dev is enum */
}usb_enum_stat_t;


/* charger type identify interface */
void bsp_usb_set_charger_type(int type);
int bsp_usb_get_charger_type(void);
int bsp_usb_is_support_charger(void);
int bsp_usb_is_support_pmu_detect(void);

/* notify interface */
void bsp_usb_register_notify(struct notifier_block *nb);
void bsp_usb_unregister_notify(struct notifier_block *nb);

/* usb status change interface*/
void bsp_usb_status_change(int status);
int usb_balong_init(void);
void usb_balong_exit(void);
int bsp_usb_console_init(void);


/* usb enum done interface */
void bsp_usb_init_enum_stat(void);
int bsp_usb_is_all_enum(void);
int bsp_usb_is_all_disable(void);
#define bsp_usb_add_setup_dev(intf_id) \
    bsp_usb_add_setup_dev_fdname(intf_id, __FILE__)
void bsp_usb_add_setup_dev_fdname(unsigned intf_id, char* fd_name);
void bsp_usb_set_enum_stat(unsigned intf_id, int enum_stat);
void bsp_usb_set_last_cdev_name(char* dev_name);


/* usb nv feature functions */
/* 1: support, 0:not support */
int bsp_usb_is_support_wwan(void);
int bsp_usb_is_support_shell(void);

/* ncm host assemble function */
s32 ncm_set_host_assemble_param(int net_id, unsigned long host_timeout);

#ifdef __cplusplus
} /* allow C++ to use these headers */
#endif /* __cplusplus */

#endif    /* End of __USB_VENDOR_H__ */

