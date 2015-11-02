/*
 * f_ncm.c -- USB CDC Network (NCM) link function driver
 *
 * Copyright (C) 2010 Nokia Corporation
 * Contact: Yauheni Kaliuta <yauheni.kaliuta@nokia.com>
 *
 * The driver borrows from f_ecm.c which is:
 *
 * Copyright (C) 2003-2005,2008 David Brownell
 * Copyright (C) 2008 Nokia Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
/**************************************************************************************************
    问题单号          时间           修改人                      说明
***************************************************************************************************/

#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/etherdevice.h>
#include <linux/crc32.h>
#include <linux/kthread.h>
#include <linux/usb/cdc.h>
#include <net/ip.h>

#include "ncm_balong.h"

#ifdef CONFIG_NCM_MBIM_SUPPORT
#include "mbim_srvc_cmd_interface.h"
#include "mbim_core_winblue_manager.h"
#endif

#include "drv_dump.h"
#include "bsp_dump.h"
#include "f_ncm_mbb.h"
#include "adapt/hw_net_dev.h"
#include "usb_platform_comm.h"
#include "hw_pnp.h"
#include "hw_pnp_adapt.h"

/*
 * This function is a "CDC Network Control Model" (CDC NCM) Ethernet link.
 * NCM is intended to be used with high-speed network attachments.
 *
 * Note that NCM requires the use of "alternate settings" for its data
 * interface.  This means that the set_alt() method has real work to do,
 * and also means that a get_alt() method is required.
 */

/* to trigger crc/non-crc ndp signature */

#define NCM_NDP_HDR_CRC_MASK     0x01000000
#define NCM_NDP_HDR_CRC          0x01000000
#define NCM_NDP_HDR_NOCRC        0x00000000

#define USB_NOTIFY_VENDOR_NCM_ACCUMULATION_TIMEOUT  0x03

/* These definiation would be used while pad for
 a skb buffer which has not enougth tailroom  */
#define NCM_PAD_ERR_FLAG         0xffff0000
#define NCM_PAD_ERR_CHECK(_pad)  (NCM_PAD_ERR_FLAG == (NCM_PAD_ERR_FLAG & _pad))
#define NCM_PAD_TRUE_VAL(_pad)   (_pad & 0x0000ffff)

/* For IPV6 */
#define ECM_DESC_ETH_NET_STAT     0x0f
#define ECM_DESC_NUM_MC_FILTERS   0x03
#define ECM_DESC_NUM_PWR_FILTERS  0x01
#define USB_CDC_GET_IPV6_DNS      0xAC

/* For MBIM*/
typedef enum _ncm_mbim_debug_mode
{
    NCM_MBIM_MODE_NORMAL = 0,
    NCM_MBIM_MODE_DEBUG,
    NCM_MBIM_MODE_MAX = 0xff,
} ncm_mbim_debug_mode;

static ncm_mbim_debug_mode gMbimDebugMode = NCM_MBIM_MODE_MAX;

/* time delay for switch mbim mode */
#define MBIM_SWITCH_MODE_TIME_DELAY     100

#define MBIM_NCM_MSG_BUFFER_LENGTH      64

/* Recored the interface name read from nv 50424 */
#define MBIM_INTERFACE_NAME_LEN_MAX     32
static USB_CHAR gMbimInterfaceName[MBIM_INTERFACE_NAME_LEN_MAX + 1] = {0};

static struct f_ncm* the_ncm[GNET_MAX_NUM] = {0};
static USB_UINT8 hostaddr[GNET_MAX_NUM][ETH_ALEN] = {{0x00, 0x1e, 0x10, 0x1f, 0x00, 0x00}};;
static USB_UINT32 glink_idx = 0;
static USB_UINT32 n_gnet_ports = GNET_USED_NUM;

static USB_BOOL g_force_bypass_mode = false;

typedef struct
{
    struct list_head    list;
    USB_UINT8*            buf;
    USB_UINT32            length;
    USB_INT32           send;
} ncm_resp_t;

typedef struct
{
    USB_VOID            (*resp_avail)(USB_VOID* v);
    USB_VOID*            v;
    spinlock_t          resp_lock;
    struct list_head    resp_queue;
    struct semaphore    sem;
    struct timer_list   timer;
} ncm_params_t;

#ifdef NCM_ENCAP_CMD_CID_ENABLED
#define USB_CDC_REQ_VENDOR_NOTIFY_WWAN_CONTROL_TYPE 0xAE
#endif

#define DISCONNECT_TIME_DELAY               0

enum ncm_notify_state
{
    NCM_NOTIFY_NONE,        /* don't notify */
    NCM_NOTIFY_CONNECT,        /* issue CONNECT next */
    NCM_NOTIFY_SPEED,        /* issue SPEED_CHANGE next */
    NCM_NOTIFY_SET_HOST_TIMEOUT,
    NCM_NOTIFY_IPV6_DNS,
    NCM_NOTIFY_FLOW_CONTROL,
    NCM_NOTIFY_RESPONSE_AVAI,  /* response available notify */
};

typedef enum _ncm_work_mode
{
    NCM_WORK_MODE_NORMAL,         /* 正常模式 ,单接口，对接协议栈*/
    NCM_WORK_MODE_GATEWAY,        /* 适配网关，双接口 ,对接协议栈*/
    NCM_WORK_MODE_MBIM,           /* MBIM模式 ,双接口，对接协议栈*/
    NCM_WORK_MODE_ECM,            /* ECM模式 ,双接口，对接协议栈*/
    NCM_WORK_MODE_NET_BRIDGE,     /* 网桥模式，单接口，用于E5 ,对接路由*/
    NCM_WORK_MODE_INVALID = 0xFFFFFFFF,
} ncm_work_mode;

/* 检查NCM是否处于BYPASS模式(非网桥模式) */
#define VERIFY_NCM_BYPASS(_ncm)     (NCM_WORK_MODE_NET_BRIDGE != _ncm->work_mode)

/* 检查NCM是否处于mbim模式 */
#define VERIFY_NCM_MBIM(_ncm)       (NCM_WORK_MODE_MBIM == _ncm->work_mode)

struct f_ncm
{
    struct gether            port;
    USB_UINT8                ctrl_id, data_id;

    USB_CHAR                 ethaddr[14];

    struct usb_ep*           notify;
    struct usb_request*      notify_req;
    struct usb_request*      notify_req_rec;
    USB_UINT8                notify_state;
    bool                is_open;

    struct ndp_parser_opts*  parser_opts;
    USB_UINT32               host_timeout;
    USB_UINT32               stat_host_timeout;
    bool                is_crc;

    /*
     * for notification, it is accessed from both
     * callback and ethernet open/close
     */
    spinlock_t            lock;

    bool                is_single_interface;
    ncm_work_mode       work_mode;

#define GNET_NCM_PRESERVE_HEADER_LEN 16
    USB_UINT32 header_room_resved;      /* len of the reserved header room */
    USB_UINT32 curr_alt;

    struct delayed_work notify_work;
    USB_UINT8           queued_notify_state;
    USB_UINT8           connect_notify_blocked;

    struct delayed_work disconnect_work;
    ncm_params_t        params;
    ncm_vendor_ctx_t    vendor_ctx;
    USB_BOOL            EncapRspBlocked;
#ifdef CONFIG_NCM_MBIM_SUPPORT
    struct delayed_work switch_work;
    USB_BOOL            LoopBackTest;
#endif

    USB_INT protocal;
};

extern netdev_tx_t eth_vendor_start_xmit(struct sk_buff* skb,
        struct net_device* net);
extern USB_INT32 ncm_vendor_submit_skb(USB_VOID* app_ctx, struct sk_buff* skb);

extern void*   get_pnp_info(USB_VOID);

USB_INT32 ncm_add_response (USB_VOID* app_ncm, USB_UINT8* buf, USB_UINT32 len)
{
    struct f_ncm* ncm = (struct f_ncm*)app_ncm;
    ncm_resp_t* ncm_resp = NULL;

    if (!ncm || !ncm->notify)
    {
        DBG_E(MBB_NCM, "invalid param\n");
        return -EINVAL;
    }

    ncm_resp = (ncm_resp_t*)kzalloc(sizeof(ncm_resp_t) + len, GFP_ATOMIC);
    if (!ncm_resp)
    {
        DBG_E(MBB_NCM, "alloc mem failed\n");
        return -ENOMEM;
    }

    ncm_resp->buf = (USB_UINT8*)(ncm_resp + 1);
    memcpy(ncm_resp->buf, buf, len);
    ncm_resp->length = len;
    ncm_resp->send = 0;

    /* add to response queue */
    spin_lock(&(ncm->params.resp_lock));
    list_add_tail(&(ncm_resp->list), &(ncm->params.resp_queue));
    spin_unlock(&(ncm->params.resp_lock));

    /* notify pc that at response is available now */
    ncm->params.resp_avail(ncm->params.v);

    return 0;
}

USB_UINT8* ncm_get_next_response (struct f_ncm* ncm, USB_UINT32* length)
{
    ncm_resp_t*        resp, *next;

    if (!length)
    { return NULL; }

    spin_lock(&(ncm->params.resp_lock));
    list_for_each_entry_safe(resp, next, &ncm->params.resp_queue, list)
    {
        if (!resp->send)
        {
            resp->send = 1;
            *length = resp->length;
            spin_unlock(&(ncm->params.resp_lock));
            return resp->buf;
        }
    }
    spin_unlock(&(ncm->params.resp_lock));

    return NULL;
}


USB_VOID ncm_free_response (struct f_ncm* ncm, USB_UINT8* buf)
{
    ncm_resp_t*        resp, *next;

    spin_lock(&(ncm->params.resp_lock));
    list_for_each_entry_safe(resp, next, &ncm->params.resp_queue, list)
    {
        if (resp->buf == buf)
        {
            list_del (&resp->list);
            kfree(resp);
            break;
        }
    }
    spin_unlock(&(ncm->params.resp_lock));
}

USB_VOID ncm_cut_response_size(struct f_ncm* ncm, USB_UINT8* buf, USB_UINT32 size)
{
    ncm_resp_t*        resp, *next;

    spin_lock(&(ncm->params.resp_lock));
    list_for_each_entry_safe(resp, next, &ncm->params.resp_queue, list)
    {
        if (resp->buf == buf)
        {
            if (resp->length > size)
            {
                resp->length -= size;
                resp->send = 0;
                resp->buf += size;
            }
            else
            {
                DBG_E(MBB_NCM, "%s : can not cut buf %p size form %u to %u\n",
                      resp->buf, resp->length, size);
            }
            break;
        }
    }
    spin_unlock(&(ncm->params.resp_lock));
}

bool ncm_resp_queue_empty(struct f_ncm* ncm)
{
    ncm_resp_t*    resp, *next;

    spin_lock(&(ncm->params.resp_lock));
    list_for_each_entry_safe(resp, next, &ncm->params.resp_queue, list)
    {
        if (!resp->send)
        {
            spin_unlock(&(ncm->params.resp_lock));
            return false;
        }
    }

    spin_unlock(&(ncm->params.resp_lock));
    return true;
}

USB_VOID ncm_empty_resp_queue(struct f_ncm* ncm)
{
    ncm_resp_t*        resp, *next;

    spin_lock(&(ncm->params.resp_lock));
    list_for_each_entry_safe(resp, next, &ncm->params.resp_queue, list)
    {
        list_del(&resp->list);
        kfree(resp);
    }
    spin_unlock(&(ncm->params.resp_lock));
}

USB_INT32 ncm_vendor_add_response(USB_VOID* app_ncm, USB_UINT8* buf, USB_UINT32 len)
{
    struct f_ncm* ncm = (struct f_ncm*)app_ncm;
    USB_INT32 ret = 0;

    if ((NULL == ncm) || (!VERIFY_NCM_BYPASS(ncm)))
    {
        DBG_E(MBB_NCM, "invalid param\n");
        return -EIO;
    }

    NCM_DBG_PRINT_AT("block %u, data length %u\n", ncm->EncapRspBlocked, len);

    if (ncm->EncapRspBlocked)
    {
        /* 主机没有及时取走返回值，清空之前的队列 */
        ncm_empty_resp_queue(ncm);
    }

    ret = ncm_add_response(ncm, buf, len);
    if (ret)
    {
        DBG_E(MBB_NCM, "ncm_add_response ret %d\n", ret);
        return ret;
    }

    if (ncm->EncapRspBlocked)
    {
        /* 主机没有及时取走返回值，此时不做等待，直接返回，防止后续at命令卡住 */
        return 0;
    }

    ret = down_timeout(&(ncm->params.sem), USB_NUM_50);

    if (-ETIME == ret)
    {
        DBG_E(MBB_NCM, "ncm_vendor_add_response : at rsp time out\n");
        ncm->EncapRspBlocked = TRUE;
    }
    else
    {
        ncm->EncapRspBlocked = FALSE;
    }

    return ret;
}
USB_VOID ncm_data_disconnect (struct work_struct* w)
{
    struct f_ncm* ncm = container_of(w, struct f_ncm, disconnect_work.work);
    ncm_app_ctx_t* app_ctx = (ncm_app_ctx_t*)ncm->vendor_ctx.vendor_priv;

    if (NULL == ncm)
    {
        DBG_E(MBB_NCM, "ncm_switch_mbim_mode : ncm is empty\n");
        return;
    }

    if (app_ctx
        && app_ctx->bDataConnect
        && app_ctx->cbs.ndis_stat_change_cb)
    {
        app_ctx->cbs.ndis_stat_change_cb(NCM_IOCTL_STUS_BREAK, NULL);
    }
}

/*****************************************************************************
 函 数 名  : ncm_get_bypass_state
 功能描述  : 获取ncm是否处于bypass模式
 输入参数  : USB_VOID
 输出参数  : 无
 返 回 值  : USB_BOOL
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年2月24日
    作    者   : chenbo
    修改内容   : 新生成函数

*****************************************************************************/
USB_BOOL ncm_get_bypass_state(USB_VOID)
{
    USB_BOOL bRet = false;

#if defined(USB_CPE)
    bRet = false;
#elif defined(USB_E5)  /* 后续Wingle形态也需要在这里处理 */
    bRet = (GATEWAY_OS_FLAG == usb_pnp_system_type_get());  /* E5 在网关下上报bypass模式网口 */
#else
    bRet = true;
#endif

    if (g_force_bypass_mode)
    {
        bRet = true;
    }

    DBG_T(MBB_PNP, "ncm bypass state = %u, is force=%u\n", bRet, g_force_bypass_mode);
    return bRet;
}

USB_INT ncm_set_bypass_state(USB_INT mode)
{
    g_force_bypass_mode = (0 != mode);
    return mode;
}

#ifdef CONFIG_NCM_MBIM_SUPPORT
/*****************************************************************************
* 函 数 名  : ncm_mbim_send_msg_to_mbim
*
* 功能描述  : 发送NCM消息到MBIM
*
* 输入参数  : USB_UINT32 msgtype - 消息类型
*             USB_UCHAR *msgdata - 消息数据
*             USB_UINT32 msglength - 消息长度
* 输出参数  :
*
* 返 回 值  : 无
*
* 其它说明  :
*
*****************************************************************************/
USB_VOID ncm_mbim_send_msg_to_mbim(struct f_ncm* ncm,
                                   USB_UINT32 msgtype, USB_UCHAR* msgdata, USB_UINT32 msglength)
{
    USB_UINT8                databuffer[MBIM_NCM_MSG_BUFFER_LENGTH] = {0};
    PMBIM_NCM_MSG_FORMAT     mbimNcmMsgReq = NULL;
    USB_INT32                ret = 0;

    if (NULL == ncm)
    {
        DBG_E(MBB_NCM, "ncm_mbim_send_msg_to_mbim : ncm is empty\n");
        return -1;
    }

    mbimNcmMsgReq = (PMBIM_NCM_MSG_FORMAT)databuffer;

    mbimNcmMsgReq->mbimMsgHeader.MessageType = MBIM_NCM_COMMAND_MSG;
    mbimNcmMsgReq->mbimMsgHeader.MessageLength =
        sizeof(MBIM_NCM_MSG_FORMAT) + msglength;
    mbimNcmMsgReq->mbimMsgHeader.TransactionId = 0;
    mbimNcmMsgReq->mbimNcmMsg = msgtype;

    if (msglength && msgdata)
    {
        mbimNcmMsgReq->InformationBufferLength = msglength;
        memcpy((mbimNcmMsgReq + 1), msgdata, msglength);
    }

    ret = ncm_mbim_send_encap_cmd(ncm->vendor_ctx.vendor_priv,
                                  databuffer, mbimNcmMsgReq->mbimMsgHeader.MessageLength);

    return ret;
}

USB_VOID ncm_switch_mbim_mode(struct work_struct* w)
{
    USB_INT mode = 0;
    struct usb_composite_dev* cdev = NULL;
    struct f_ncm* ncm = container_of(w, struct f_ncm, switch_work.work);

    if (NULL == ncm)
    {
        DBG_E(MBB_NCM, "ncm_switch_mbim_mode : ncm is empty\n");
        return;
    }

    cdev = ncm->port.func.config->cdev;
    if ((!cdev) || (!cdev->gadget))
    {
        DBG_E(MBB_PNP, "cdev or gadget NULL\n");
        return;
    }

    DBG_T(MBB_NCM, "ncm_switch_mbim_mode: mode=%u, gIsMbimMode=%u",
          gMbimDebugMode, VERIFY_NCM_MBIM(ncm));

    if (NCM_MBIM_MODE_NORMAL == gMbimDebugMode)
    {
        mode = MBIM_SWITCH_NORMAL_MODE;
        DBG_I(MBB_NCM, "U_NCM:gMbimDebugMode:%d", gMbimDebugMode);
    }
    else if (NCM_MBIM_MODE_DEBUG == gMbimDebugMode)
    {
        mode = MBIM_SWITCH_DEBUG_MODE;
        DBG_I(MBB_NCM, "U_NCM:gMbimDebugMode:%d", gMbimDebugMode);
    }
    else
    {
        return;
    }

    ncm_mbim_send_msg_to_mbim(ncm, MBIM_NCM_RESET_MSG, NULL, 0);
    pnp_switch_mbim_mode(mode);

    return;
}

USB_VOID BSP_USB_MBIMSetMode(USB_INT switch_idex)
{
    ncm_mbim_debug_mode switchmode = NCM_MBIM_MODE_MAX;

    if ((!the_ncm[0]) || (!VERIFY_NCM_MBIM(the_ncm[0])))
    {
        return;
    }

    if (0 == switch_idex)
    {
        switchmode = NCM_MBIM_MODE_NORMAL;
    }
    else if (1 == switch_idex)
    {
        switchmode = NCM_MBIM_MODE_DEBUG;
    }
    else
    {
        return;
    }

    if (gMbimDebugMode == switchmode)
    {
        return;
    }
    else
    {
        gMbimDebugMode = switchmode;
    }

    adp_usb_queue_delay_work(&the_ncm[0]->switch_work, MBIM_SWITCH_MODE_TIME_DELAY);

    return;
}

USB_UINT BSP_USB_MBIMGetMode(USB_VOID)
{
    return (USB_UINT32)gMbimDebugMode;
}

USB_VOID ncm_mbim_enable_loopback(USB_VOID* ncm_ctx, USB_BOOL enable)
{
    struct f_ncm* ncm = (struct f_ncm*)ncm_ctx;
    ncm_app_ctx_t* app_ctx = NULL;

    if (NULL == ncm)
    {
        DBG_E(MBB_NCM, "%s empty ncm\n", __FUNCTION__);
        return;
    }

    if (enable && (ncm->vendor_ctx.connect) && (!ncm->LoopBackTest))
    {
        DBG_E(MBB_NCM, "%s eth working\n", __FUNCTION__);
        return;
    }

    app_ctx = (ncm_app_ctx_t*)ncm->vendor_ctx.vendor_priv;
    if (NULL == app_ctx)
    {
        DBG_E(MBB_NCM, "%s vendor not connect\n", __FUNCTION__);
        return;
    }

    app_ctx->bDataConnect = enable;
    ncm->LoopBackTest = enable;
    ncm->vendor_ctx.connect = enable;

    DBG_T(MBB_NCM, "%s enable=%u\n", __FUNCTION__, enable);
}

static USB_VOID ncm_mbim_loopback_swap_ip_addr(USB_UCHAR* ippktbuf)
{
    USB_UINT32  uTmp = 0;
    USB_UINT32* pSrc = NULL;
    USB_UINT32* pDst = NULL;

    if (NULL == ippktbuf)
    {
        DBG_E(MBB_NCM, "%s: empty input\n", __FUNCTION__);
        return;
    }

    pSrc = (USB_UINT32*)(ippktbuf + 12);    /* IP包第12个字节起四个字节是源IP地址 */
    pDst = (USB_UINT32*)(ippktbuf + 16);    /* IP包第16个字节起四个字节是目标IP地址 */

    uTmp = *pSrc;
    *pSrc = *pDst;
    *pDst = uTmp;
}

#endif

static inline struct f_ncm* func_to_ncm(struct usb_function* f)
{
    return container_of(f, struct f_ncm, port.func);
}

/* peak (theoretical) bulk transfer rate in bits-per-second */
static inline USB_UINT32 ncm_bitrate(struct usb_gadget* g)
{
    USB_UINT32 speed = eth_get_net_speed();
    if (0 < speed)
    {
        return speed;
    }

    return 300 * 1000 * 1000;   /* V7R2的理论峰值速率为300M */
}

USB_UINT32 gNcmDbgMask = 0;
int ncm_set_debug_mask(unsigned int mask)
{
    (void)printk("ncm_set_debug_mask: MASK BIT 0 for AT\n");
    (void)printk("ncm_set_debug_mask: MASK BIT 1 for RX\n");
    (void)printk("ncm_set_debug_mask: MASK BIT 2 for TX\n");
    (void)printk("ncm_set_debug_mask: MASK BIT 3 for Notify\n");
    (void)printk("ncm_set_debug_mask: mask=0x%08X\n", mask);
    gNcmDbgMask = mask;
    return 0;
}

/*-------------------------------------------------------------------------*/

/*
 * We cannot group frames so use just the minimal size which ok to put
 * one max-size ethernet frame.
 * If the host can group frames, allow it to do that, 16K is selected,
 * because it's used by default by the current linux host driver
 */
#define NTB_DEFAULT_IN_SIZE    USB_CDC_NCM_NTB_MIN_IN_SIZE
#define NTB_OUT_SIZE        16384

/*
 * skbs of size less than that will not be aligned
 * to NCM's dwNtbInMaxSize to save bus bandwidth
 */

#define    MAX_TX_NONFIXED        (512 * 3)

#define FORMATS_SUPPORTED    (USB_CDC_NCM_NTB16_SUPPORTED |    \
                              USB_CDC_NCM_NTB32_SUPPORTED)

static struct usb_cdc_ncm_ntb_parameters ntb_parameters =
{
    .wLength = sizeof ntb_parameters,
    .bmNtbFormatsSupported = cpu_to_le16(FORMATS_SUPPORTED),
    .dwNtbInMaxSize = cpu_to_le32(GNET_NCM_IN_MAXSIZE_PER_TRANSFER),
    .wNdpInDivisor = cpu_to_le16(USB_CDC_NCM_NDP_IN_DIVISOR),
    .wNdpInPayloadRemainder = cpu_to_le16(USB_CDC_NCM_NDP_IN_REMAINDER),
    .wNdpInAlignment = cpu_to_le16(USB_CDC_NCM_NDP_IN_ALIGNMENT),

    .dwNtbOutMaxSize = cpu_to_le32(GNET_NCM_OUT_MAXSIZE_PER_TRANSFER),
    .wNdpOutDivisor = cpu_to_le16(USB_CDC_NCM_NDP_OUT_DIVISOR),
    .wNdpOutPayloadRemainder = cpu_to_le16(USB_CDC_NCM_NDP_OUT_REMAINDER),
    .wNdpOutAlignment = cpu_to_le16(USB_CDC_NCM_NDP_OUT_ALIGNMENT),
};

/*
 * Use wMaxPacketSize big enough to fit CDC_NOTIFY_SPEED_CHANGE in one
 * packet, to simplify cancellation; and a big transfer interval, to
 * waste less bandwidth.
 */

#define LOG2_STATUS_INTERVAL_MSEC    5    /* 1 << 5 == 32 msec */
#define NCM_STATUS_BYTECOUNT        16    /* 8 byte header + data */

static struct usb_interface_assoc_descriptor ncm_iad_desc =
{
    .bLength =        sizeof ncm_iad_desc,
    .bDescriptorType =    USB_DT_INTERFACE_ASSOCIATION,

    /* .bFirstInterface =    DYNAMIC, */
    .bInterfaceCount =    2,    /* control + data */
    .bFunctionClass =    USB_CLASS_COMM,
    .bFunctionSubClass =    USB_CDC_SUBCLASS_NCM,
    .bFunctionProtocol =    USB_CDC_PROTO_NONE,
    /* .iFunction =        DYNAMIC */
};

/* interface descriptor: */

static struct usb_interface_descriptor ncm_control_intf =
{
    .bLength =        sizeof ncm_control_intf,
    .bDescriptorType =    USB_DT_INTERFACE,

    /* .bInterfaceNumber = DYNAMIC */
    .bNumEndpoints =    1,
    .bInterfaceClass =    USB_CLASS_COMM,
    .bInterfaceSubClass =    USB_CDC_SUBCLASS_NCM,
    .bInterfaceProtocol =    USB_CDC_PROTO_NONE,
    /* .iInterface = DYNAMIC */
};

static struct usb_cdc_header_desc ncm_header_desc =
{
    .bLength =        sizeof ncm_header_desc,
    .bDescriptorType =    USB_DT_CS_INTERFACE,
    .bDescriptorSubType =    USB_CDC_HEADER_TYPE,

    .bcdCDC =        cpu_to_le16(0x0110),
};

static struct usb_cdc_union_desc ncm_union_desc =
{
    .bLength =        sizeof(ncm_union_desc),
    .bDescriptorType =    USB_DT_CS_INTERFACE,
    .bDescriptorSubType =    USB_CDC_UNION_TYPE,
    /* .bMasterInterface0 =    DYNAMIC */
    /* .bSlaveInterface0 =    DYNAMIC */
};

static struct usb_cdc_ncm_mbim_fdesc ncm_mbim_desc =
{
    .bLength = sizeof(ncm_mbim_desc),
    .bType = USB_DT_CS_INTERFACE,
    .bSubtype = 0x1b,
    .bcdVersion = cpu_to_le16(0x0100),
    .wMaxCtrlMsg = cpu_to_le16(0x400),
    .bNumFilters = 0x20,
    .bMaxFilterSize = 0x80,
    .wMaxSegSize = cpu_to_le16(1500),
    .bmNetworkCapabilities = 0x20,
};

static struct usb_cdc_ncm_ex_mbim_fdesc ncm_ex_mbim_desc =
{
    .bLength = sizeof(ncm_ex_mbim_desc),
    .bType = USB_DT_CS_INTERFACE,
    .bSubtype = 0x1c,
    .bcdVersion = cpu_to_le16(0x0100),
    .bMaxOutstandingCmdMsg = 0x10,
    .wMTPU = cpu_to_le16(1500),
};

static struct usb_cdc_ether_desc ncm_ecm_desc =
{
    .bLength = sizeof ncm_ecm_desc,
    .bDescriptorType = USB_DT_CS_INTERFACE,
    .bDescriptorSubType = USB_CDC_ETHERNET_TYPE,

    /* this descriptor actually adds value, surprise! */
    /* .iMACAddress = DYNAMIC */
    .bmEthernetStatistics = cpu_to_le32(0), /* no statistics */
    .wMaxSegmentSize = cpu_to_le16(ETH_FRAME_LEN),
    .wNumberMCFilters = cpu_to_le16(0),
    .bNumberPowerFilters = 0,
};

#define NCAPS    (USB_CDC_NCM_NCAP_ETH_FILTER \
                  | USB_CDC_NCM_NCAP_NET_ADDRESS \
                  | USB_CDC_NCM_NCAP_ENCAP_COMMAND \
                  | USB_CDC_NCM_NCAP_MAX_DATAGRAM_SIZE \
                  | USB_CDC_NCM_NCAP_CRC_MODE)

static struct usb_cdc_ncm_desc ncm_desc =
{
    .bLength =        sizeof ncm_desc,
    .bDescriptorType =    USB_DT_CS_INTERFACE,
    .bDescriptorSubType =    USB_CDC_NCM_TYPE,

    .bcdNcmVersion =    cpu_to_le16(0x0100),
    /* can process SetEthernetPacketFilter */
    .bmNetworkCapabilities = NCAPS,
};

/* the default data interface has no endpoints ... */

static struct usb_interface_descriptor ncm_data_nop_intf =
{
    .bLength =        sizeof ncm_data_nop_intf,
    .bDescriptorType =    USB_DT_INTERFACE,

    .bInterfaceNumber =    1,
    .bAlternateSetting =    0,
    .bNumEndpoints =    0,
    .bInterfaceClass =    USB_CLASS_CDC_DATA,
    .bInterfaceSubClass =    0,
    .bInterfaceProtocol =    USB_CDC_NCM_PROTO_NTB,
    /* .iInterface = DYNAMIC */
};

/* ... but the "real" data interface has two bulk endpoints */

static struct usb_interface_descriptor ncm_data_intf =
{
    .bLength =        sizeof ncm_data_intf,
    .bDescriptorType =    USB_DT_INTERFACE,

    .bInterfaceNumber =    1,
    .bAlternateSetting =    1,
    .bNumEndpoints =    2,
    .bInterfaceClass =    USB_CLASS_CDC_DATA,
    .bInterfaceSubClass =    0,
    .bInterfaceProtocol =    USB_CDC_NCM_PROTO_NTB,
    /* .iInterface = DYNAMIC */
};

/* full speed support: */

static struct usb_endpoint_descriptor fs_ncm_notify_desc =
{
    .bLength =        USB_DT_ENDPOINT_SIZE,
    .bDescriptorType =    USB_DT_ENDPOINT,

    .bEndpointAddress =    USB_DIR_IN,
    .bmAttributes =        USB_ENDPOINT_XFER_INT,
    .wMaxPacketSize =    cpu_to_le16(NCM_STATUS_BYTECOUNT),
    .bInterval =        1 << LOG2_STATUS_INTERVAL_MSEC,
};

static struct usb_endpoint_descriptor fs_ncm_in_desc =
{
    .bLength =        USB_DT_ENDPOINT_SIZE,
    .bDescriptorType =    USB_DT_ENDPOINT,

    .bEndpointAddress =    USB_DIR_IN,
    .bmAttributes =        USB_ENDPOINT_XFER_BULK,
};

static struct usb_endpoint_descriptor fs_ncm_out_desc =
{
    .bLength =        USB_DT_ENDPOINT_SIZE,
    .bDescriptorType =    USB_DT_ENDPOINT,

    .bEndpointAddress =    USB_DIR_OUT,
    .bmAttributes =        USB_ENDPOINT_XFER_BULK,
};

static struct usb_descriptor_header* ncm_fs_function_single[] =
{
    //    (struct usb_descriptor_header *) &ncm_iad_desc,
    /* CDC NCM control descriptors */
    (struct usb_descriptor_header*)& ncm_control_intf,
    (struct usb_descriptor_header*)& ncm_header_desc,
    (struct usb_descriptor_header*)& ncm_desc,
    (struct usb_descriptor_header*)& ncm_ecm_desc,
    (struct usb_descriptor_header*)& ncm_union_desc,
    (struct usb_descriptor_header*)& fs_ncm_notify_desc,
    /* data interface, altsettings 0 and 1 */
    //    (struct usb_descriptor_header *) &ncm_data_nop_intf,
    (struct usb_descriptor_header*)& ncm_data_intf,
    (struct usb_descriptor_header*)& fs_ncm_notify_desc,
    (struct usb_descriptor_header*)& fs_ncm_in_desc,
    (struct usb_descriptor_header*)& fs_ncm_out_desc,
    NULL,
};

static struct usb_descriptor_header* ncm_fs_function[] =
{
    (struct usb_descriptor_header*)& ncm_iad_desc,
    /* CDC NCM control descriptors */
    (struct usb_descriptor_header*)& ncm_control_intf,
    (struct usb_descriptor_header*)& ncm_header_desc,
    (struct usb_descriptor_header*)& ncm_union_desc,
    (struct usb_descriptor_header*)& ncm_ecm_desc,
    (struct usb_descriptor_header*)& ncm_desc,
    (struct usb_descriptor_header*)& fs_ncm_notify_desc,
    /* data interface, altsettings 0 and 1 */
    (struct usb_descriptor_header*)& ncm_data_nop_intf,
    (struct usb_descriptor_header*)& ncm_data_intf,
    (struct usb_descriptor_header*)& fs_ncm_in_desc,
    (struct usb_descriptor_header*)& fs_ncm_out_desc,
    NULL,
};

#ifdef NCM_ENCAP_CMD_CID_ENABLED
static struct usb_descriptor_header* ncm_cid_fs_function[] =
{
    //    (struct usb_descriptor_header *) &ncm_iad_desc,
    /* CDC NCM control descriptors */
    (struct usb_descriptor_header*)& ncm_control_intf,
    (struct usb_descriptor_header*)& ncm_header_desc,
    (struct usb_descriptor_header*)& ncm_desc,
    (struct usb_descriptor_header*)& ncm_mbim_desc,
    (struct usb_descriptor_header*)& ncm_ecm_desc,
    (struct usb_descriptor_header*)& ncm_union_desc,
    (struct usb_descriptor_header*)& fs_ncm_notify_desc,
    /* data interface, altsettings 0 and 1 */
    //    (struct usb_descriptor_header *) &ncm_data_nop_intf,
    (struct usb_descriptor_header*)& ncm_data_intf,
    (struct usb_descriptor_header*)& fs_ncm_notify_desc,
    (struct usb_descriptor_header*)& fs_ncm_in_desc,
    (struct usb_descriptor_header*)& fs_ncm_out_desc,
    NULL,
};
#endif

static struct usb_descriptor_header* ncm_mbim_fs_function[]  =
{
    (struct usb_descriptor_header*)& ncm_iad_desc,
    /* CDC NCM control descriptors */
    (struct usb_descriptor_header*)& ncm_control_intf,
    (struct usb_descriptor_header*)& ncm_header_desc,
    (struct usb_descriptor_header*)& ncm_mbim_desc,
    (struct usb_descriptor_header*)& ncm_ex_mbim_desc,
    (struct usb_descriptor_header*)& ncm_union_desc,
    (struct usb_descriptor_header*)& fs_ncm_notify_desc,
    /* data interface, altsettings 0 and 1 */
    (struct usb_descriptor_header*)& ncm_data_nop_intf,
    (struct usb_descriptor_header*)& ncm_data_intf,
    (struct usb_descriptor_header*)& fs_ncm_in_desc,
    (struct usb_descriptor_header*)& fs_ncm_out_desc,
    NULL,
};
/* high speed support: */

static struct usb_endpoint_descriptor hs_ncm_notify_desc =
{
    .bLength =        USB_DT_ENDPOINT_SIZE,
    .bDescriptorType =    USB_DT_ENDPOINT,

    .bEndpointAddress =    USB_DIR_IN,
    .bmAttributes =        USB_ENDPOINT_XFER_INT,
    .wMaxPacketSize =    cpu_to_le16(NCM_STATUS_BYTECOUNT),
    .bInterval =        LOG2_STATUS_INTERVAL_MSEC,
};
static struct usb_endpoint_descriptor hs_ncm_in_desc =
{
    .bLength =        USB_DT_ENDPOINT_SIZE,
    .bDescriptorType =    USB_DT_ENDPOINT,

    .bEndpointAddress =    USB_DIR_IN,
    .bmAttributes =        USB_ENDPOINT_XFER_BULK,
    .wMaxPacketSize =    cpu_to_le16(512),
};

static struct usb_endpoint_descriptor hs_ncm_out_desc =
{
    .bLength =        USB_DT_ENDPOINT_SIZE,
    .bDescriptorType =    USB_DT_ENDPOINT,

    .bEndpointAddress =    USB_DIR_OUT,
    .bmAttributes =        USB_ENDPOINT_XFER_BULK,
    .wMaxPacketSize =    cpu_to_le16(512),
};

static struct usb_descriptor_header* ncm_hs_function_single[] =
{
    //    (struct usb_descriptor_header *) &ncm_iad_desc,
    /* CDC NCM control descriptors */
    (struct usb_descriptor_header*)& ncm_control_intf,
    (struct usb_descriptor_header*)& ncm_header_desc,
    (struct usb_descriptor_header*)& ncm_desc,
    (struct usb_descriptor_header*)& ncm_ecm_desc,
    (struct usb_descriptor_header*)& ncm_union_desc,
    (struct usb_descriptor_header*)& hs_ncm_notify_desc,
    /* data interface, altsettings 0 and 1 */
    //    (struct usb_descriptor_header *) &ncm_data_nop_intf,
    (struct usb_descriptor_header*)& ncm_data_intf,
    (struct usb_descriptor_header*)& hs_ncm_notify_desc,
    (struct usb_descriptor_header*)& hs_ncm_in_desc,
    (struct usb_descriptor_header*)& hs_ncm_out_desc,
    NULL,
};

static struct usb_descriptor_header* ncm_hs_function[] =
{
    (struct usb_descriptor_header*)& ncm_iad_desc,
    /* CDC NCM control descriptors */
    (struct usb_descriptor_header*)& ncm_control_intf,
    (struct usb_descriptor_header*)& ncm_header_desc,
    (struct usb_descriptor_header*)& ncm_union_desc,
    (struct usb_descriptor_header*)& ncm_ecm_desc,
    (struct usb_descriptor_header*)& ncm_desc,
    (struct usb_descriptor_header*)& hs_ncm_notify_desc,
    /* data interface, altsettings 0 and 1 */
    (struct usb_descriptor_header*)& ncm_data_nop_intf,
    (struct usb_descriptor_header*)& ncm_data_intf,
    (struct usb_descriptor_header*)& hs_ncm_in_desc,
    (struct usb_descriptor_header*)& hs_ncm_out_desc,
    NULL,
};

#ifdef NCM_ENCAP_CMD_CID_ENABLED
static struct usb_descriptor_header* ncm_cid_hs_function[]  =
{
    /* CDC NCM control descriptors */
    (struct usb_descriptor_header*)& ncm_control_intf,
    (struct usb_descriptor_header*)& ncm_header_desc,
    (struct usb_descriptor_header*)& ncm_desc,
    (struct usb_descriptor_header*)& ncm_mbim_desc,
    (struct usb_descriptor_header*)& ncm_ecm_desc,
    (struct usb_descriptor_header*)& ncm_union_desc,
    (struct usb_descriptor_header*)& hs_ncm_notify_desc,
    /* data interface, altsettings 0 and 1 */
    (struct usb_descriptor_header*)& ncm_data_intf,
    (struct usb_descriptor_header*)& hs_ncm_notify_desc,
    (struct usb_descriptor_header*)& hs_ncm_in_desc,
    (struct usb_descriptor_header*)& hs_ncm_out_desc,
    NULL,
};
#endif

static struct usb_descriptor_header* ncm_mbim_hs_function[]  =
{
    (struct usb_descriptor_header*)& ncm_iad_desc,
    /* CDC NCM control descriptors */
    (struct usb_descriptor_header*)& ncm_control_intf,
    (struct usb_descriptor_header*)& ncm_header_desc,
    (struct usb_descriptor_header*)& ncm_mbim_desc,
    (struct usb_descriptor_header*)& ncm_ex_mbim_desc,
    (struct usb_descriptor_header*)& ncm_union_desc,
    (struct usb_descriptor_header*)& hs_ncm_notify_desc,
    /* data interface, altsettings 0 and 1 */
    (struct usb_descriptor_header*)& ncm_data_nop_intf,
    (struct usb_descriptor_header*)& ncm_data_intf,
    (struct usb_descriptor_header*)& hs_ncm_in_desc,
    (struct usb_descriptor_header*)& hs_ncm_out_desc,
    NULL,
};

/* super speed support: */

static struct usb_endpoint_descriptor ss_ncm_notify_desc =
{
    .bLength =        USB_DT_ENDPOINT_SIZE,
    .bDescriptorType =    USB_DT_ENDPOINT,

    .bEndpointAddress =    USB_DIR_IN,
    .bmAttributes =        USB_ENDPOINT_XFER_INT,
    .wMaxPacketSize =    cpu_to_le16(NCM_STATUS_BYTECOUNT),
    .bInterval =        LOG2_STATUS_INTERVAL_MSEC,
};

static struct usb_ss_ep_comp_descriptor ss_ncm_intr_comp_desc =
{
    .bLength =        sizeof ss_ncm_intr_comp_desc,
    .bDescriptorType =    USB_DT_SS_ENDPOINT_COMP,

    /* the following 3 values can be tweaked if necessary */
    /* .bMaxBurst =        0, */
    /* .bmAttributes =    0, */
    .wBytesPerInterval =    cpu_to_le16(NCM_STATUS_BYTECOUNT),
};

static struct usb_endpoint_descriptor ss_ncm_in_desc =
{
    .bLength =        USB_DT_ENDPOINT_SIZE,
    .bDescriptorType =    USB_DT_ENDPOINT,

    .bEndpointAddress =    USB_DIR_IN,
    .bmAttributes =        USB_ENDPOINT_XFER_BULK,
    .wMaxPacketSize =    cpu_to_le16(1024),
};

static struct usb_endpoint_descriptor ss_ncm_out_desc =
{
    .bLength =        USB_DT_ENDPOINT_SIZE,
    .bDescriptorType =    USB_DT_ENDPOINT,

    .bEndpointAddress =    USB_DIR_OUT,
    .bmAttributes =        USB_ENDPOINT_XFER_BULK,
    .wMaxPacketSize =    cpu_to_le16(1024),
};

static struct usb_ss_ep_comp_descriptor ss_ncm_bulk_comp_desc =
{
    .bLength =        sizeof ss_ncm_bulk_comp_desc,
    .bDescriptorType =    USB_DT_SS_ENDPOINT_COMP,

    /* the following 2 values can be tweaked if necessary */
    /* .bMaxBurst =        0, */
    /* .bmAttributes =    0, */
};

static struct usb_descriptor_header* ncm_ss_function_single[] =
{
    //    (struct usb_descriptor_header *) &ncm_iad_desc,
    /* CDC NCM control descriptors */
    (struct usb_descriptor_header*)& ncm_control_intf,
    (struct usb_descriptor_header*)& ncm_header_desc,
    (struct usb_descriptor_header*)& ncm_desc,
    (struct usb_descriptor_header*)& ncm_ecm_desc,
    (struct usb_descriptor_header*)& ncm_union_desc,
    (struct usb_descriptor_header*)& ss_ncm_notify_desc,
    (struct usb_descriptor_header*)& ss_ncm_intr_comp_desc,
    /* data interface, altsettings 0 and 1 */
    //    (struct usb_descriptor_header *) &ncm_data_nop_intf,
    (struct usb_descriptor_header*)& ncm_data_intf,
    (struct usb_descriptor_header*)& ss_ncm_notify_desc,
    (struct usb_descriptor_header*)& ss_ncm_intr_comp_desc,
    (struct usb_descriptor_header*)& ss_ncm_in_desc,
    (struct usb_descriptor_header*)& ss_ncm_bulk_comp_desc,
    (struct usb_descriptor_header*)& ss_ncm_out_desc,
    (struct usb_descriptor_header*)& ss_ncm_bulk_comp_desc,
    NULL,
};

static struct usb_descriptor_header* ncm_ss_function[] =
{
    (struct usb_descriptor_header*)& ncm_iad_desc,
    /* CDC NCM control descriptors */
    (struct usb_descriptor_header*)& ncm_control_intf,
    (struct usb_descriptor_header*)& ncm_header_desc,
    (struct usb_descriptor_header*)& ncm_union_desc,
    (struct usb_descriptor_header*)& ncm_ecm_desc,
    (struct usb_descriptor_header*)& ncm_desc,
    (struct usb_descriptor_header*)& ss_ncm_notify_desc,
    (struct usb_descriptor_header*)& ss_ncm_intr_comp_desc,
    /* data interface, altsettings 0 and 1 */
    (struct usb_descriptor_header*)& ncm_data_nop_intf,
    (struct usb_descriptor_header*)& ncm_data_intf,
    (struct usb_descriptor_header*)& ss_ncm_in_desc,
    (struct usb_descriptor_header*)& ss_ncm_bulk_comp_desc,
    (struct usb_descriptor_header*)& ss_ncm_out_desc,
    (struct usb_descriptor_header*)& ss_ncm_bulk_comp_desc,
    NULL,
};

#ifdef NCM_ENCAP_CMD_CID_ENABLED
static struct usb_descriptor_header* ncm_cid_ss_function[] =
{
    //    (struct usb_descriptor_header *) &ncm_iad_desc,
    /* CDC NCM control descriptors */
    (struct usb_descriptor_header*)& ncm_control_intf,
    (struct usb_descriptor_header*)& ncm_header_desc,
    (struct usb_descriptor_header*)& ncm_desc,
    (struct usb_descriptor_header*)& ncm_mbim_desc,
    (struct usb_descriptor_header*)& ncm_ecm_desc,
    (struct usb_descriptor_header*)& ncm_union_desc,
    (struct usb_descriptor_header*)& ss_ncm_notify_desc,
    (struct usb_descriptor_header*)& ss_ncm_intr_comp_desc,
    /* data interface, altsettings 0 and 1 */
    //    (struct usb_descriptor_header *) &ncm_data_nop_intf,
    (struct usb_descriptor_header*)& ncm_data_intf,
    (struct usb_descriptor_header*)& ss_ncm_notify_desc,
    (struct usb_descriptor_header*)& ss_ncm_intr_comp_desc,
    (struct usb_descriptor_header*)& ss_ncm_in_desc,
    (struct usb_descriptor_header*)& ss_ncm_bulk_comp_desc,
    (struct usb_descriptor_header*)& ss_ncm_out_desc,
    (struct usb_descriptor_header*)& ss_ncm_bulk_comp_desc,
    NULL,
};
#endif

static struct usb_descriptor_header* ncm_mbim_ss_function[]  =
{
    (struct usb_descriptor_header*)& ncm_iad_desc,
    /* CDC NCM control descriptors */
    (struct usb_descriptor_header*)& ncm_control_intf,
    (struct usb_descriptor_header*)& ncm_header_desc,
    (struct usb_descriptor_header*)& ncm_mbim_desc,
    (struct usb_descriptor_header*)& ncm_ex_mbim_desc,
    (struct usb_descriptor_header*)& ncm_union_desc,
    (struct usb_descriptor_header*)& ss_ncm_notify_desc,
    (struct usb_descriptor_header*)& ss_ncm_intr_comp_desc,
    /* data interface, altsettings 0 and 1 */
    (struct usb_descriptor_header*)& ncm_data_nop_intf,
    (struct usb_descriptor_header*)& ncm_data_intf,
    (struct usb_descriptor_header*)& ss_ncm_in_desc,
    (struct usb_descriptor_header*)& ss_ncm_bulk_comp_desc,
    (struct usb_descriptor_header*)& ss_ncm_out_desc,
    (struct usb_descriptor_header*)& ss_ncm_bulk_comp_desc,
    NULL,
};

/* string descriptors: */

#define STRING_CTRL_IDX    0
#define STRING_MAC_IDX    1
#define STRING_DATA_IDX    2
#define STRING_IAD_IDX    3

static struct usb_string ncm_string_defs[] =
{
    [STRING_CTRL_IDX].s = "NCM Network Control Model",
    [STRING_MAC_IDX].s = "001e101f0000" /* DYNAMIC */,
    [STRING_DATA_IDX].s = "CDC Network Data",
    [STRING_IAD_IDX].s = "CDC NCM",
    {  } /* end of list */
};

static struct usb_gadget_strings ncm_string_table =
{
    .language =        0x0409,    /* en-us */
    .strings =        ncm_string_defs,
};

static struct usb_gadget_strings* ncm_strings[] =
{
    &ncm_string_table,
    NULL,
};

/*
 * Here are options for NCM Datagram Pointer table (NDP) parser.
 * There are 2 different formats: NDP16 and NDP32 in the spec (ch. 3),
 * in NDP16 offsets and sizes fields are 1 16bit word wide,
 * in NDP32 -- 2 16bit words wide. Also signatures are different.
 * To make the parser code the same, put the differences in the structure,
 * and switch pointers to the structures when the format is changed.
 */

struct ndp_parser_opts
{
    USB_UINT32        nth_sign;
    USB_UINT32        ndp_sign;
    USB_UINT32    nth_size;
    USB_UINT32    ndp_size;
    USB_UINT32    ndplen_align;
    /* sizes in USB_UINT16 units */
    USB_UINT32    dgram_item_len; /* index or length */
    USB_UINT32    block_length;
    USB_UINT32    fp_index;
    USB_UINT32    reserved1;
    USB_UINT32    reserved2;
    USB_UINT32    next_fp_index;
};

#define INIT_NDP16_OPTS {                    \
        .nth_sign = USB_CDC_NCM_NTH16_SIGN,        \
                    .ndp_sign = USB_CDC_NCM_NDP16_NOCRC_SIGN,    \
                                .nth_size = sizeof(struct usb_cdc_ncm_nth16),    \
                                            .ndp_size = sizeof(struct usb_cdc_ncm_ndp16),    \
                                                    .ndplen_align = 4,                \
                                                            .dgram_item_len = 1,                \
                                                                    .block_length = 1,                \
                                                                            .fp_index = 1,                    \
                                                                                    .reserved1 = 0,                    \
                                                                                            .reserved2 = 0,                    \
                                                                                                    .next_fp_index = 1,                \
    }


#define INIT_NDP32_OPTS {                    \
        .nth_sign = USB_CDC_NCM_NTH32_SIGN,        \
                    .ndp_sign = USB_CDC_NCM_NDP32_NOCRC_SIGN,    \
                                .nth_size = sizeof(struct usb_cdc_ncm_nth32),    \
                                            .ndp_size = sizeof(struct usb_cdc_ncm_ndp32),    \
                                                    .ndplen_align = 8,                \
                                                            .dgram_item_len = 2,                \
                                                                    .block_length = 2,                \
                                                                            .fp_index = 2,                    \
                                                                                    .reserved1 = 1,                    \
                                                                                            .reserved2 = 2,                    \
                                                                                                    .next_fp_index = 2,                \
    }

static struct ndp_parser_opts ndp16_opts = INIT_NDP16_OPTS;
static struct ndp_parser_opts ndp32_opts = INIT_NDP32_OPTS;

static inline USB_VOID put_ncm(__le16** p, USB_UINT32 size, USB_UINT32 val)
{
    switch (size)
    {
        case 1:
            put_unaligned_le16((USB_UINT16)val, *p);
            break;
        case 2:
            put_unaligned_le32((USB_UINT32)val, *p);
            break;
        default:
            BUG();
    }

    *p += size;
}

static inline USB_UINT32 get_ncm(__le16** p, USB_UINT32 size)
{
    USB_UINT32 tmp = 0;

    switch (size)
    {
        case 1:
            tmp = get_unaligned_le16(*p);
            break;
        case 2:
            tmp = get_unaligned_le32(*p);
            break;
        default:
            BUG();
    }

    *p += size;
    return tmp;
}

/*-------------------------------------------------------------------------*/

static inline USB_VOID ncm_reset_values(struct f_ncm* ncm)
{
    ncm->parser_opts = &ndp32_opts;

    ncm->is_crc = false;
    ncm->port.cdc_filter = DEFAULT_FILTER;

    /* doesn't make sense for ncm, fixed size used */
    ncm->port.header_len = 0;

    //ntb_parameters.dwNtbOutMaxSize = cpu_to_le32(GNET_NCM_OUT_MAXSIZE_PER_TRANSFER);
    ncm->port.fixed_out_len = le32_to_cpu(ntb_parameters.dwNtbOutMaxSize);
    ncm->port.fixed_in_len = le32_to_cpu(ntb_parameters.dwNtbInMaxSize);
}

/*
 * Context: ncm->lock held
 */
static USB_VOID ncm_do_notify(struct f_ncm* ncm)
{
    struct usb_request*        req = ncm->notify_req;
    struct usb_cdc_notification*    event;
    struct usb_composite_dev*    cdev = ncm->port.func.config->cdev;
    __le32*                 data;
    USB_INT32               status;
    struct eth_dev*         dev = ncm->port.ioport;
    huawei_dynamic_info_st* pnp_info = get_pnp_info();

    /* Linux/AF23 上采用轮询获取at返回值，不用报中断 */
    if ((OS_TYPE_LINUX == pnp_info->rewind_param.bPcType)
        && (NCM_NOTIFY_RESPONSE_AVAI == ncm->notify_state))
    {
        ncm->notify_state = NCM_NOTIFY_NONE;
        return;
    }

    NCM_DBG_PRINT_NTF("req %p, dev %p, state %u\n", req, dev, ncm->notify_state);

    /* notification already in flight? */
    if (!req)
    { return; }

    /* ncm not enable, reset the notify status */
    if (!dev)
    {
        ncm->notify_state = NCM_NOTIFY_NONE;
        return;
    }

    event = req->buf;
    switch (ncm->notify_state)
    {
        case NCM_NOTIFY_NONE:
            return;

        case NCM_NOTIFY_SET_HOST_TIMEOUT:
            event->bNotificationType = USB_NOTIFY_VENDOR_NCM_ACCUMULATION_TIMEOUT;
            event->wValue = cpu_to_le16(0);
            event->wLength = sizeof(USB_UINT32);
            event->bmRequestType = USB_DIR_IN | USB_TYPE_VENDOR | USB_RECIP_INTERFACE;
            req->length = sizeof(USB_UINT32) + sizeof(*event);
            data = req->buf + sizeof(*event);
            memcpy((USB_VOID*)data, &ncm->host_timeout, sizeof(USB_UINT32));
            ncm->notify_state = NCM_NOTIFY_NONE;
            break;

        case NCM_NOTIFY_CONNECT:
            event->bNotificationType = USB_CDC_NOTIFY_NETWORK_CONNECTION;

            if (VERIFY_NCM_BYPASS(ncm))
            {
                event->wValue = cpu_to_le16(ncm->vendor_ctx.connect);
            }
            else
            {
                if (ncm->is_open)
                { event->wValue = cpu_to_le16(1); }
                else
                { event->wValue = cpu_to_le16(0); }
            }

            event->wLength = 0;
            req->length = sizeof * event;

            NCM_DBG_PRINT_NTF("notify connect %d\n", event->wValue);
            ncm->notify_state = NCM_NOTIFY_NONE;
            event->bmRequestType = 0xA1;
            break;

        case NCM_NOTIFY_SPEED:
            event->bNotificationType = USB_CDC_NOTIFY_SPEED_CHANGE;
            event->wValue = cpu_to_le16(0);
            event->wLength = cpu_to_le16(8);
            req->length = NCM_STATUS_BYTECOUNT;

            /* SPEED_CHANGE data is up/down speeds in bits/sec */
            data = req->buf + sizeof * event;

            if (VERIFY_NCM_BYPASS(ncm))
            {
                data[0] = cpu_to_le32(ncm->vendor_ctx.speed.u32DownBitRate);
                data[1] = cpu_to_le32(ncm->vendor_ctx.speed.u32UpBitRate);
                ncm->notify_state = NCM_NOTIFY_NONE;
                DBG_I(MBB_NCM, "notify speed [%d:%d]\n",
                      ncm->vendor_ctx.speed.u32DownBitRate,
                      ncm->vendor_ctx.speed.u32UpBitRate);
            }
            else
            {
                data[0] = cpu_to_le32(ncm_bitrate(cdev->gadget));
                data[1] = data[0];
                ncm->notify_state = NCM_NOTIFY_CONNECT;
                DBG_I(MBB_NCM, "notify speed %ld\n", ncm_bitrate(cdev->gadget));
            }

            event->bmRequestType = 0xA1;
            break;

        case NCM_NOTIFY_RESPONSE_AVAI:
            memset(event, 0, sizeof(*event));
            event->bmRequestType = 0xA1;
            event->bNotificationType = USB_CDC_NOTIFY_RESPONSE_AVAILABLE;

            req->length = sizeof(*event);
            /* stop to notify */
            ncm->notify_state = NCM_NOTIFY_NONE;
            break;

        case NCM_NOTIFY_IPV6_DNS :
            memset(event, 0, sizeof(*event));
            event->bmRequestType = 0xC1;
            event->bNotificationType = USB_CDC_VENDOR_NTF_IPV6_DNS;

            req->length = sizeof(*event);
            /* stop to notify */
            ncm->notify_state = NCM_NOTIFY_NONE;
            break;

        case NCM_NOTIFY_FLOW_CONTROL :
            memset(event, 0, sizeof(*event));
            event->bmRequestType = 0xC1;
            event->bNotificationType = USB_CDC_VENDOR_NTF_FLOW_CONTROL;
            event->wValue = (0 != ncm->vendor_ctx.flow_control) ? cpu_to_le16(1) : 0;

            req->length = sizeof(*event);
            /* stop to notify */
            ncm->notify_state = NCM_NOTIFY_NONE;
            break;
    }
    //event->bmRequestType = 0xA1;
    event->wIndex = cpu_to_le16(ncm->ctrl_id);

    ncm->notify_req = NULL;
    /*
     * In double buffering if there is a space in FIFO,
     * completion callback can be called right after the call,
     * so unlocking
     */
    spin_unlock(&ncm->lock);

    /* check whether the gadget is suspended.
       if so, try to wake up the host */
    if (unlikely(dev->is_suspend))
    {
        if (gnet_wakeup_gadget(dev))
        {
            /* wakeup failed, reset the notify state */
            spin_lock(&ncm->lock);

            /* 由于单板suspend导致connect中断未能上报 */
            if ((0xA1 == event->bmRequestType)
                && (USB_CDC_NOTIFY_NETWORK_CONNECTION == event->bNotificationType)
                && (!(VERIFY_NCM_MBIM(ncm))))
            {
                DBG_E(MBB_NCM, "connect notify blocked\n");
                ncm->connect_notify_blocked = 1;
            }

            ncm->notify_state = NCM_NOTIFY_NONE;
            ncm->notify_req = req;
            return;
        }
    }

    status = usb_ep_queue(ncm->notify, req, GFP_ATOMIC);
    spin_lock(&ncm->lock);
    if (status < 0)
    {
        ncm->notify_req = req;
        DBG_E(MBB_NCM, "notify --> %ld\n", status);
    }
}

USB_VOID ncm_notify_work (struct work_struct* w)
{
    struct f_ncm* ncm = container_of(w, struct f_ncm, notify_work.work);
    USB_ULONG flags;

    if (NULL == ncm)
    {
        DBG_E(MBB_NCM, "ncm is empty\n");
        return;
    }
    else
    {
        DBG_T(MBB_NCM, "send notify %u in work\n", ncm->queued_notify_state);
    }
    ncm->notify_state = ncm->queued_notify_state;
    ncm->queued_notify_state = NCM_NOTIFY_NONE;
    spin_lock_irqsave(&ncm->lock, flags);
    ncm_do_notify(ncm);
    spin_unlock_irqrestore(&ncm->lock, flags);
}

/*
 * Context: ncm->lock held
 */
static USB_VOID ncm_notify(struct f_ncm* ncm)
{
    if (VERIFY_NCM_BYPASS(ncm))
    {
        /* BYPASS 模式下不允许自动上报NCM状态 */
        return;
    }

    /*
     * NOTE on most versions of Linux, host side cdc-ethernet
     * won't listen for notifications until its netdevice opens.
     * The first notification then sits in the FIFO for a long
     * time, and the second one is queued.
     *
     * If ncm_notify() is called before the second (CONNECT)
     * notification is sent, then it will reset to send the SPEED
     * notificaion again (and again, and again), but it's not a problem
     */
    ncm->notify_state = NCM_NOTIFY_SPEED;
    ncm_do_notify(ncm);
}

static USB_VOID ncm_notify_complete(struct usb_ep* ep, struct usb_request* req)
{
    struct f_ncm*            ncm = req->context;
    struct usb_cdc_notification*    event = req->buf;

    NCM_DBG_PRINT_NTF("type %x, status %d\n",
                      event->bNotificationType, req->status);

    spin_lock(&ncm->lock);
    switch (req->status)
    {
        case 0:
            break;
        case -ECONNRESET:
        case -ESHUTDOWN:
            ncm->notify_state = NCM_NOTIFY_NONE;
            break;
        default:
            break;
    }

    ncm->notify_req = req;
    ncm_do_notify(ncm);
    spin_unlock(&ncm->lock);

    if (VERIFY_NCM_BYPASS(ncm))
    {
        complete(&(ncm->vendor_ctx.notify_done));
    }
}

static USB_VOID ncm_ep0out_complete(struct usb_ep* ep, struct usb_request* req)
{
    /* now for SET_NTB_INPUT_SIZE only */
    USB_UINT32        in_size;
    struct usb_function*    f = req->context;
    struct f_ncm*        ncm = func_to_ncm(f);

    req->context = NULL;
    if (req->status || req->actual != req->length)
    {
        DBG_E(MBB_NCM, "Bad control-OUT transfer\n");
        goto invalid;
    }

    in_size = get_unaligned_le32(req->buf);
    if (in_size < USB_CDC_NCM_NTB_MIN_IN_SIZE ||
        in_size > le32_to_cpu(ntb_parameters.dwNtbInMaxSize))
    {
        DBG_E(MBB_NCM, "Got wrong INPUT SIZE (%ld) from host\n", in_size);
        goto invalid;
    }

    ncm->port.fixed_in_len = in_size;
    DBG_I(MBB_NCM, "Set NTB INPUT SIZE %ld\n", in_size);
    return;

invalid:
    usb_ep_set_halt(ep);
    return;
}

static USB_VOID ncm_encap_cmd_handle(struct f_ncm* ncm, USB_UINT8* buf, USB_UINT32 len)
{
    if ((NULL == ncm) || (!VERIFY_NCM_BYPASS(ncm)))
    {
        return;
    }

    ncm_vendor_encap_cmd(ncm->vendor_ctx.vendor_priv, buf, len);
}

static USB_VOID ncm_encap_cmd_complete(struct usb_ep* ep, struct usb_request* req)
{
    struct f_ncm* ncm = req->context;
    USB_INT32  status = req->status;

    NCM_DBG_PRINT_AT("req %p, stauts %d, length %u\n", req, status, req->length);

    if (!ncm)
    {
        return;
    }

    switch (status)
    {
        case 0:
        {
            ncm_encap_cmd_handle(ncm, req->buf, req->length);
            break;
        }

        case -ECONNRESET:           /* unlink */
        case -ESHUTDOWN:            /* disconnect etc */
        case -ECONNABORTED:         /* endpoint reset */
        case -EOVERFLOW:            /* data overrun */
        default:
        {
            DBG_E(MBB_NCM, "%s line %d: status = %d", __FUNCTION__, __LINE__, status);
            break;
        }
    }

    return;
}

static USB_VOID ncm_set_eth_mul_filters(struct usb_ep* ep, struct usb_request* req)
{
    /* Nothing to do while handling USB_CDC_SET_ETHERNET_MULTICAST_FILTERS */
    return;
}

#ifdef NCM_ENCAP_CMD_CID_ENABLED
extern USB_VOID ncm_notify_wwan_ctl_type(USB_VOID* app_ctx, USB_UCHAR ctltype);
static USB_VOID ncm_vendor_notify_ctrl_type_complete(
    struct usb_ep* ep, struct usb_request* req)
{
    USB_INT32      status = 0;
    USB_UINT32  length = 0;
    USB_UCHAR* ctltypeptr = NULL;
    ncm_app_ctx_t* app_ctx = NULL;

    if (NULL == req)
    {
        return;
    }

    status = req->status;
    ctltypeptr = (USB_UCHAR*)req->buf;
    length = req->length;

    DBG_I(MBB_NCM, "%s: status=%u, length=%u\n", __FUNCTION__, status, length);

    if ((0 == status) && (sizeof(USB_UCHAR) == length)
        && ((USB_CDC_NOTIFY_WWAN_CTL_TYPE_AT == *ctltypeptr)
            || (USB_CDC_NOTIFY_WWAN_CTL_TYPE_CID == *ctltypeptr)))
    {
        app_ctx = (ncm_app_ctx_t*)the_ncm[0]->vendor_ctx.vendor_priv;
        if (NULL != app_ctx)
        {
            DBG_I(MBB_NCM, "%s: set type %u\n", __FUNCTION__, *ctltypeptr);
            ncm_notify_wwan_ctl_type(app_ctx, *ctltypeptr);
        }
    }
}
#endif

static USB_VOID ncm_response_complete(struct usb_ep* ep, struct usb_request* req)
{
    struct f_ncm*      ncm = req->context;

    if (VERIFY_NCM_MBIM(ncm))
    {
#ifdef CONFIG_NCM_MBIM_SUPPORT
        ncm_mbim_send_encap_rsp_done(req->status);
#endif
    }
    else
    {
        up(&(ncm->params.sem));
    }

    req->length = 0;

    if (!ncm_resp_queue_empty(ncm))
    {
        mod_timer(&ncm->params.timer, (jiffies + (HZ / USB_NUM_20)));
    }

    return;
}

static USB_UINT32 ncm_encap_cmd_response(struct f_ncm* ncm, struct usb_request* req, USB_UINT32 bufLength)
{
    USB_UINT8* buf;
    USB_UINT32 n, value;

    buf = ncm_get_next_response(ncm, &n);
    if (buf)
    {
        if (bufLength < n)
        {
            memcpy(req->buf, buf, bufLength);
            ncm_cut_response_size(ncm, buf, bufLength);
            value = bufLength;
        }
        else
        {
            memcpy(req->buf, buf, n);
            ncm_free_response(ncm, buf);
            value = n;
        }

        req->complete = ncm_response_complete;
    }
    else
    {
        value = 0;
    }

    NCM_DBG_PRINT_AT("length=%u\n", value);

    req->context = ncm;

    return value;
}

/* static USB_VOID ncm_response_available_complete(struct usb_ep *ep, struct usb_request *req)
{
    if (!ep || !req)
    {
        return;
    }

    if (req->status)
    {
        DBG_E(MBB_NCM, "%s error, status=%d, ep_name=%s\n", __FUNCTION__, req->status, ep->name);
    }

    return ;
} */

static USB_VOID ncm_response_available(USB_VOID* v)
{
    struct f_ncm*         ncm = (struct f_ncm*)v;
    USB_ULONG flags = 0;

    if (!ncm)
    {
        /* In this case, ncm interface is removed by ^setport */
        return ;
    }

    spin_lock_irqsave(&ncm->lock, flags);
    ncm->notify_state = NCM_NOTIFY_RESPONSE_AVAI;
    ncm_do_notify(ncm);
    spin_unlock_irqrestore(&ncm->lock, flags);
}

static USB_VOID ncm_response_available_notify(USB_ULONG handle)
{
    struct f_ncm* ncm = (struct f_ncm*)handle;

    if (!ncm_resp_queue_empty(ncm))
    {
        if (ncm->params.resp_avail)
        { ncm->params.resp_avail(ncm->params.v); }
    }
}

static USB_INT ncm_setup(struct usb_function* f, const struct usb_ctrlrequest* ctrl)
{
    struct f_ncm*        ncm = func_to_ncm(f);
    struct usb_composite_dev* cdev = f->config->cdev;
    struct usb_request*    req = cdev->req;
    USB_INT               value = -EOPNOTSUPP;
    USB_UINT16            w_index = le16_to_cpu(ctrl->wIndex);
    USB_UINT16            w_value = le16_to_cpu(ctrl->wValue);
    USB_UINT16            w_length = le16_to_cpu(ctrl->wLength);

    /* Filter bypass request in net bridge mode */
    if (!VERIFY_NCM_BYPASS(ncm))
    {
        if ((USB_CDC_SEND_ENCAPSULATED_COMMAND == ctrl->bRequest)
            || (USB_CDC_GET_ENCAPSULATED_RESPONSE == ctrl->bRequest)
            || (USB_CDC_GET_WWAN_STATE == ctrl->bRequest))
        {
            goto invalid;
        }
    }

    /*
     * composite driver infrastructure handles everything except
     * CDC class messages; interface activation uses set_alt().
     */
    switch ((ctrl->bRequestType << 8) | ctrl->bRequest)
    {
        case (((USB_DIR_OUT | USB_TYPE_CLASS | USB_RECIP_INTERFACE) << 8)
                      | USB_CDC_SET_ETHERNET_PACKET_FILTER):
            /*
             * see 6.2.30: no data, wIndex = interface,
             * wValue = packet filter bitmap
             */
            if (w_length != 0 || w_index != ncm->ctrl_id)
            { goto invalid; }
            DBG_I(MBB_NCM, "packet filter %02x\n", w_value);
            /*
             * REVISIT locking of cdc_filter.  This assumes the UDC
             * driver won't have a concurrent packet TX irq running on
             * another CPU; or that if it does, this write is atomic...
             */
            ncm->port.cdc_filter = w_value;
            value = 0;
            break;

            /*
             * and optionally:
             * case USB_CDC_SEND_ENCAPSULATED_COMMAND:
             * case USB_CDC_GET_ENCAPSULATED_RESPONSE:
             * case USB_CDC_SET_ETHERNET_MULTICAST_FILTERS:
             * case USB_CDC_SET_ETHERNET_PM_PATTERN_FILTER:
             * case USB_CDC_GET_ETHERNET_PM_PATTERN_FILTER:
             * case USB_CDC_GET_ETHERNET_STATISTIC:
             */
        case  (((USB_DIR_OUT | USB_TYPE_CLASS | USB_RECIP_INTERFACE) << 8)
                       | USB_CDC_SEND_ENCAPSULATED_COMMAND):

            if (w_value || w_index != ncm->ctrl_id)
            { goto invalid; }
            value = w_length;
            req->complete = ncm_encap_cmd_complete;
            req->context = ncm;
            if (ncm->EncapRspBlocked)
            {
                ncm_empty_resp_queue(ncm);
            }
            ncm->EncapRspBlocked = FALSE;
            break;

        case  (((USB_DIR_IN | USB_TYPE_CLASS | USB_RECIP_INTERFACE) << 8)
                       | USB_CDC_GET_ENCAPSULATED_RESPONSE):

            if (w_value || w_index != ncm->ctrl_id)
            { goto invalid; }

            value = ncm_encap_cmd_response(ncm, req, w_length);
            ncm->EncapRspBlocked = FALSE;
            break;

        case (((USB_DIR_IN | USB_TYPE_VENDOR | USB_RECIP_INTERFACE) << 8)
                      | USB_CDC_GET_WWAN_STATE) :
            {
                if (bsp_usb_is_support_wwan())
                {
                    memcpy(req->buf, "WWAN", 4);
                }
                else
                {
                    memset(req->buf, 0, 4);
                }
                value = 4;
                break;
            }

            /*
            case ((USB_DIR_IN | USB_TYPE_VENDOR | USB_RECIP_INTERFACE) << 8)
              | CDC_REQ_GET_NCM_PACKETS_ACCUMULATION_TIME :
            {
                ncm_app_ctx_t  *app_ctx = (ncm_app_ctx_t *)ncm->vendor_ctx.ncm_priv;

                put_unaligned_le32(app_ctx->u32AccumulationTime, req->buf);
                value = 4;
                break;
            }
            */

#ifdef CONFIG_NCM_MBIM_SUPPORT
        case (((USB_DIR_OUT | USB_TYPE_CLASS | USB_RECIP_INTERFACE) << 8 )
                      | USB_REG_MBIM_RESET) :
            {
                ncm_mbim_send_msg_to_mbim(ncm, MBIM_NCM_RESET_MSG, NULL, 0);
                value = 0;
                break;
            }
#endif

#ifdef NCM_ENCAP_CMD_CID_ENABLED
        case  (((USB_DIR_OUT | USB_TYPE_VENDOR | USB_RECIP_INTERFACE) << USB_NUM_8)
                       | USB_CDC_REQ_VENDOR_NOTIFY_WWAN_CONTROL_TYPE) :
            {
                value = w_length;
                req->complete = ncm_vendor_notify_ctrl_type_complete;
                req->context = ncm;

                break;
            }
#endif

        case  (((USB_DIR_OUT | USB_TYPE_CLASS | USB_RECIP_INTERFACE) << 8)
                       | USB_CDC_SET_ETHERNET_MULTICAST_FILTERS) :
            {
                value = w_length;
                req->complete = ncm_set_eth_mul_filters;
                req->context = ncm;

                break;
            }

        case (((USB_DIR_IN | USB_TYPE_VENDOR | USB_RECIP_INTERFACE) << 8)
                      | USB_CDC_GET_IPV6_DNS) :
            {
                ncm_app_ctx_t* app_ctx = (ncm_app_ctx_t*)ncm->vendor_ctx.vendor_priv;
                if ((NULL != app_ctx) && (0 < app_ctx->u32Ipv6DnsLen))
                {
                    memcpy(req->buf, app_ctx->au8Ipv6Dns, app_ctx->u32Ipv6DnsLen);
                    value = app_ctx->u32Ipv6DnsLen;
                }

                break;
            }

        case (((USB_DIR_IN | USB_TYPE_CLASS | USB_RECIP_INTERFACE) << 8)
                      | USB_CDC_GET_NTB_PARAMETERS):

            if (w_length == 0 || w_value != 0 || w_index != ncm->ctrl_id)
            { goto invalid; }
            value = w_length > sizeof ntb_parameters ?
                    sizeof ntb_parameters : w_length;
            memcpy(req->buf, &ntb_parameters, value);
            DBG_I(MBB_NCM, "Host asked NTB parameters\n");
            break;

        case (((USB_DIR_IN | USB_TYPE_CLASS | USB_RECIP_INTERFACE) << 8)
                      | USB_CDC_GET_NTB_INPUT_SIZE):

            if (w_length < 4 || w_value != 0 || w_index != ncm->ctrl_id)
            { goto invalid; }
            put_unaligned_le32(ncm->port.fixed_in_len, req->buf);
            value = 4;
            DBG_I(MBB_NCM, "Host asked INPUT SIZE, sending %ld\n",
                  ncm->port.fixed_in_len);
            break;

        case (((USB_DIR_OUT | USB_TYPE_CLASS | USB_RECIP_INTERFACE) << 8)
                      | USB_CDC_SET_NTB_INPUT_SIZE):
            {
                if (w_length != 4 || w_value != 0 || w_index != ncm->ctrl_id)
                { goto invalid; }
                req->complete = ncm_ep0out_complete;
                req->length = w_length;
                req->context = f;

                value = req->length;
                break;
            }

        case (((USB_DIR_IN | USB_TYPE_CLASS | USB_RECIP_INTERFACE) << 8)
                      | USB_CDC_GET_NTB_FORMAT):
            {
                USB_UINT16 format;

                if (w_length < 2 || w_value != 0 || w_index != ncm->ctrl_id)
                { goto invalid; }
                format = (ncm->parser_opts == &ndp16_opts) ? 0x0000 : 0x0001;
                put_unaligned_le16(format, req->buf);
                value = 2;
                DBG_E(MBB_NCM, "Host asked NTB FORMAT, sending %d\n", format);
                break;
            }

        case (((USB_DIR_OUT | USB_TYPE_CLASS | USB_RECIP_INTERFACE) << 8)
                      | USB_CDC_SET_NTB_FORMAT):
            {
                if (w_length != 0 || w_index != ncm->ctrl_id)
                { goto invalid; }
                switch (w_value)
                {
                    case 0x0000:
                        ncm->parser_opts = &ndp16_opts;
                        DBG_I(MBB_NCM, "NCM16 selected\n");
                        break;
                    case 0x0001:
                        ncm->parser_opts = &ndp32_opts;
                        DBG_I(MBB_NCM, "NCM32 selected\n");
                        break;
                    default:
                        goto invalid;
                }
                value = 0;
                break;
            }
        case (((USB_DIR_IN | USB_TYPE_CLASS | USB_RECIP_INTERFACE) << 8)
                      | USB_CDC_GET_CRC_MODE):
            {
                USB_UINT16 is_crc;

                if (w_length < 2 || w_value != 0 || w_index != ncm->ctrl_id)
                { goto invalid; }
                is_crc = ncm->is_crc ? 0x0001 : 0x0000;
                put_unaligned_le16(is_crc, req->buf);
                value = 2;
                DBG_E(MBB_NCM, "Host asked CRC MODE, sending %d\n", is_crc);
                break;
            }

        case (((USB_DIR_OUT | USB_TYPE_CLASS | USB_RECIP_INTERFACE) << 8)
                      | USB_CDC_SET_CRC_MODE):
            {
                USB_INT32 ndp_hdr_crc = 0;

                if (w_length != 0 || w_index != ncm->ctrl_id)
                { goto invalid; }
                switch (w_value)
                {
                    case 0x0000:
                        ncm->is_crc = false;
                        ndp_hdr_crc = NCM_NDP_HDR_NOCRC;
                        DBG_I(MBB_NCM, "non-CRC mode selected\n");
                        break;
                    case 0x0001:
                        ncm->is_crc = true;
                        ndp_hdr_crc = NCM_NDP_HDR_CRC;
                        DBG_I(MBB_NCM, "CRC mode selected\n");
                        break;
                    default:
                        goto invalid;
                }

                if (VERIFY_NCM_MBIM(ncm))
                {
                    DBG_E(MBB_NCM, "MBIM not support CRC\n");
                    value = 0;
                    break;
                }

                ncm->parser_opts->ndp_sign &= ~NCM_NDP_HDR_CRC_MASK;
                ncm->parser_opts->ndp_sign |= ndp_hdr_crc;
                value = 0;
                break;
            }

            /* and disabled in ncm descriptor: */
            /* case USB_CDC_GET_NET_ADDRESS: */
            /* case USB_CDC_SET_NET_ADDRESS: */
            /* case USB_CDC_GET_MAX_DATAGRAM_SIZE: */
            /* case USB_CDC_SET_MAX_DATAGRAM_SIZE: */

            /* host request to get the reserved_bytes parameter */
        case (((USB_DIR_IN | USB_TYPE_VENDOR | USB_RECIP_INTERFACE) << 8)
                      | USB_CDC_NTFY_HUAWEI_GET_NCM_RESERVE_BYTES):
            {
                if (w_length != 4 || w_value != 0 || w_index != ncm->ctrl_id)
                { goto invalid; }

                ncm->header_room_resved = GNET_NCM_PRESERVE_HEADER_LEN;
                put_unaligned_le32(ncm->header_room_resved, req->buf);
                value = 4;

                DBG_E(MBB_NCM, "Host asked reserved header room len, sending %ld\n",
                      ncm->header_room_resved);
                break;
            }

        default:
        invalid:
            DBG_E(MBB_NCM, "invalid control req%02x.%02x v%04x i%04x l%d\n",
                  ctrl->bRequestType, ctrl->bRequest,
                  w_value, w_index, w_length);
    }

    /* respond with data transfer or status phase? */
    if (value >= 0)
    {
        DBG_I(MBB_NCM, "ncm req%02x.%02x v%04x i%04x l%d\n",
              ctrl->bRequestType, ctrl->bRequest,
              w_value, w_index, w_length);
        req->zero = 0;
        req->length = value;
        value = usb_ep_queue(cdev->gadget->ep0, req, GFP_ATOMIC);
        if (value < 0)
            DBG_E(MBB_NCM, "ncm req %02x.%02x response err %d\n",
                  ctrl->bRequestType, ctrl->bRequest,
                  value);
    }

    /* device either stalls (value < 0) or reports success */
    return value;
}

USB_INT ncm_set_host_assemble_param(USB_INT net_id, USB_ULONG host_timeout)
{
    USB_ULONG flags;
    struct f_ncm* ncm = the_ncm[net_id];

    if (unlikely(!ncm))
    {
        return -ESHUTDOWN;
    }

    spin_lock_irqsave(&ncm->lock, flags);
    ncm->host_timeout = host_timeout;
    ncm->notify_state = NCM_NOTIFY_SET_HOST_TIMEOUT;
    ncm->stat_host_timeout++;
    ncm_do_notify(ncm);
    spin_unlock_irqrestore(&ncm->lock, flags);

    return 0;
}

USB_INT32 ncm_vendor_notify(USB_VOID* app_ncm, USB_UINT32 cmd, USB_VOID* param)
{
    struct f_ncm* ncm = (struct f_ncm*)app_ncm;
    USB_INT32 ret = 0;
    USB_ULONG flags = 0;

    DBG_T(MBB_NCM, "%s cmd=%u\n", __FUNCTION__, cmd);

    if ((NULL == ncm) || (!VERIFY_NCM_BYPASS(ncm)))
    {
        return -EIO;
    }

    switch (cmd)
    {
        case NCM_IOCTL_NETWORK_CONNECTION_NOTIF :
        {
            ncm->vendor_ctx.connect = (0 != (*(USB_UINT32*)param));
            ncm->notify_state = NCM_NOTIFY_CONNECT;

            if (!ncm->notify_req && ncm->notify_req_rec)
            {
                DBG_T(MBB_NCM, "notify is busy, sending connect notify in work\n");
                usb_ep_dequeue(ncm->notify, ncm->notify_req_rec);
                ncm->queued_notify_state = NCM_NOTIFY_CONNECT;
                adp_usb_queue_delay_work(&(ncm->notify_work), 0);
                return 0;
            }

            break;
        }

        case NCM_IOCTL_CONNECTION_SPEED_CHANGE_NOTIF :
        {
            NCM_IOCTL_CONNECTION_SPEED_S* speed = (NCM_IOCTL_CONNECTION_SPEED_S*)param;
            ncm->vendor_ctx.speed.u32UpBitRate = speed->u32UpBitRate;
            ncm->vendor_ctx.speed.u32DownBitRate = speed->u32DownBitRate;
            ncm->notify_state = NCM_NOTIFY_SPEED;
            break;
        }

        case NCM_IOCTL_SET_IPV6_DNS :
        {
            ncm->notify_state = NCM_NOTIFY_IPV6_DNS;
            break;
        }

        case NCM_IOCTL_FLOW_CTRL_NOTIF :
        {
            ncm->notify_state = NCM_NOTIFY_FLOW_CONTROL;
            ncm->vendor_ctx.flow_control = *(USB_UINT32*)param;
            break;
        }

        default :
        {
            DBG_E(MBB_NCM, "%s : invalid cmd %u\n", __FUNCTION__, cmd);
            return -EINVAL;
        }
    }

    spin_lock_irqsave(&ncm->lock, flags);
    ncm_do_notify(ncm);
    spin_unlock_irqrestore(&ncm->lock, flags);

    ret = wait_for_completion_interruptible_timeout(&ncm->vendor_ctx.notify_done, 1 * HZ);
    if (0 >= ret)
    {
        DBG_E(MBB_NCM, "wait for notify_done failed: ret=%d, cmd=%u\n", ret, cmd);
        ncm->vendor_ctx.stat_notify_timeout++;
    }

    return 0;
}

USB_INT32 ncm_vendor_post_skb(USB_VOID* vendor_ncm, struct sk_buff* skb)
{
    struct f_ncm* ncm = (struct f_ncm*)vendor_ncm;
    struct eth_dev*  dev = NULL;
    ncm_app_ctx_t* app_ctx = NULL;
    netdev_tx_t  tx_ret = NETDEV_TX_OK;

    if (!ncm)
    { return -ENOMEM; }

    if (!VERIFY_NCM_BYPASS(ncm))
    {
        return -EIO;
    }

    if (gnet_dbg_level & GNET_DBG_TX2TASK)
    {
        if (skb_queue_len(&ncm->vendor_ctx.tx_frames) >= 1000)
        {
            ncm->vendor_ctx.stat_tx_drop++;
            return -EIO;
        }

        skb_queue_tail(&ncm->vendor_ctx.tx_frames, skb);
        ncm->vendor_ctx.stat_tx_post++;
        complete(&ncm->vendor_ctx.tx_kick);

        return 0;
    }

    app_ctx = (ncm_app_ctx_t*)ncm->vendor_ctx.vendor_priv;
    dev = ncm->port.ioport;
    if (!dev)
    {
        ncm->vendor_ctx.stat_tx_nodev++;
        return -ENXIO;
    }
    ncm->vendor_ctx.stat_tx_post++;
    if (skb)
    {
        ncm->vendor_ctx.stat_tx_total++;
        if (true != app_ctx->bDataConnect)
        {
            ncm->vendor_ctx.stat_tx_cancel++;
            dev_kfree_skb_any(skb);
        }
        else
        {
            ncm->vendor_ctx.stat_tx_xmit++;
            tx_ret = eth_vendor_start_xmit(skb, dev->net);
            if (NETDEV_TX_OK != tx_ret)
            {
                ncm->vendor_ctx.stat_tx_xmit_fail++;
                return tx_ret;
            }
        }
    }

    return 0;
}

static USB_INT32 ncm_vendor_rx(struct gether* port, struct sk_buff* skb)
{
    struct f_ncm*  ncm = func_to_ncm(&port->func);

    if (!ncm)
    {
        return -ENOMEM;
    }

    if (!ncm->vendor_ctx.vendor_priv)
    {
        ncm->vendor_ctx.stat_rx_einval++;
        return -ENOMEM;
    }

    ncm->vendor_ctx.stat_rx_total++;

    return ncm_vendor_submit_skb((ncm_app_ctx_t*)ncm->vendor_ctx.vendor_priv, skb);
}

static USB_INT32 ncm_vendor_tx_thread(USB_VOID* param)
{
    struct f_ncm* ncm = (struct f_ncm*)param;
    struct eth_dev*  dev = NULL;
    struct sk_buff* skb = NULL;
    ncm_app_ctx_t* app_ctx = NULL;
    netdev_tx_t  tx_ret = NETDEV_TX_OK;

    GNET_TRACE(("ncm vendor tx thread enter.\n"));

    if (!ncm || !ncm->vendor_ctx.vendor_priv)
    {
        GNET_ERR(("ncm vendor tx thread abnormal quit!\n"));
        return -ENODEV;
    }

    app_ctx = (ncm_app_ctx_t*)ncm->vendor_ctx.vendor_priv;
    dev = ncm->port.ioport;
    if (!dev)
    {
        GNET_ERR(("no eth_dev,ncm vendor tx thread abnormal quit!\n"));
        return -ENODEV;
    }

    ncm->vendor_ctx.tx_task_run = true;
    while (ncm->vendor_ctx.tx_task_run)
    {
        wait_for_completion(&ncm->vendor_ctx.tx_kick);

        do
        {
            skb = skb_dequeue(&ncm->vendor_ctx.tx_frames);
            if (skb)
            {
                ncm->vendor_ctx.stat_tx_total++;
                if (true != app_ctx->bDataConnect)
                {
                    ncm->vendor_ctx.stat_tx_cancel++;
                    dev_kfree_skb_any(skb);
                }
                else
                {
                    ncm->vendor_ctx.stat_tx_xmit++;
                    tx_ret = eth_vendor_start_xmit(skb, dev->net);
                    if (NETDEV_TX_OK != tx_ret)
                    {
                        ncm->vendor_ctx.stat_tx_xmit_fail++;
                        GNET_ERR(("vendor xmit skb failed\n"));
                    }
                }
            }
        }
        while (skb);
    }

    complete(&ncm->vendor_ctx.tx_task_stop);
    GNET_TRACE(("ncm vendor tx thread exit.\n"));
    return 0;
}

static USB_INT ncm_vendor_tx_thread_init(struct f_ncm* ncm)
{
    GNET_TRACE(("%s:enter.\n", __FUNCTION__));

    if (!ncm)
    {
        return -ENOMEM;
    }

    if (ncm->vendor_ctx.tx_task_run)
    {
        return 0;
    }

    skb_queue_head_init(&ncm->vendor_ctx.tx_frames);
    init_completion(&ncm->vendor_ctx.tx_kick);
    init_completion(&ncm->vendor_ctx.tx_task_stop);
    init_completion(&ncm->vendor_ctx.notify_done);

    /* move tx_task_run flag in thread, if the thread is wakeup after set_alt 1
       if clean up before set_alt 1, thread_uninit will wait for ever */
    //ncm->vendor_ctx.tx_task_run = true;

    ncm->vendor_ctx.tx_task = kthread_create(ncm_vendor_tx_thread, ncm,
                              "ncm_vendor_tx_thread");
    if (IS_ERR(ncm->vendor_ctx.tx_task))
    {
        GNET_ERR(("create kthread ncm_tx_thread failed!\n"));
        ncm->vendor_ctx.tx_task_run = false;
        return (USB_INT32)PTR_ERR(ncm->vendor_ctx.tx_task);
    }

    GNET_TRACE(("%s line %d:leave\n", __FUNCTION__, __LINE__));

    return 0;
}

static USB_VOID ncm_vendor_tx_thead_stop(struct f_ncm* ncm)
{
    if (ncm->vendor_ctx.tx_task)
    {
        kthread_stop(ncm->vendor_ctx.tx_task);
        ncm->vendor_ctx.tx_task = NULL;
    }
}

static USB_INT32 ncm_vendor_tx_thread_uninit(struct f_ncm* ncm)
{
    struct sk_buff* skb = NULL;

    if (!ncm)
    {
        return -ENOMEM;
    }

    if (!ncm->vendor_ctx.tx_task_run)
    {
        /* if ncm is disable, the thread will not wakeup,
         * stop it here.
         */
        ncm_vendor_tx_thead_stop(ncm);
        return 0;
    }

    ncm->vendor_ctx.tx_task_run = false;

    complete(&ncm->vendor_ctx.tx_kick);
    wait_for_completion(&ncm->vendor_ctx.tx_task_stop);

    do
    {
        skb = skb_dequeue(&ncm->vendor_ctx.tx_frames);
        if (skb)
        {
            kfree_skb(skb);
        }
    }
    while (skb);

    DBG_I(MBB_NCM, "ncm_vendor_tx_thread_uninit ok\n");
    return 0;
}

USB_VOID ncm_params_init(struct f_ncm* ncm)
{
    DBG_I(MBB_NCM, "%s:enter\n", __FUNCTION__);

    spin_lock_init(&(ncm->params.resp_lock));
    INIT_LIST_HEAD(&(ncm->params.resp_queue));
    sema_init(&(ncm->params.sem), 0);

    /* init timer */
    init_timer(&ncm->params.timer);
    ncm->params.timer.function = ncm_response_available_notify;
    ncm->params.timer.data = (USB_ULONG)ncm;

    INIT_DELAYED_WORK(&ncm->disconnect_work, ncm_data_disconnect);
    INIT_DELAYED_WORK(&ncm->notify_work, ncm_notify_work);
#ifdef CONFIG_NCM_MBIM_SUPPORT
    INIT_DELAYED_WORK(&ncm->switch_work, ncm_switch_mbim_mode);
    ncm->LoopBackTest = false;
#endif

    DBG_I(MBB_NCM, "%s:leave\n", __FUNCTION__);
}

USB_INT32 ncm_params_register(USB_VOID (*resp_avail)(USB_VOID* v), USB_VOID* v)
{
    struct f_ncm* ncm = (struct f_ncm*)v;

    DBG_I(MBB_NCM, "%s:\n", __func__);

    if (!resp_avail)
    { return -EINVAL; }

    ncm->params.resp_avail = resp_avail;
    ncm->params.v = v;

    return 0;
}

USB_VOID ncm_params_deregister(struct f_ncm* ncm)
{
    DBG_I(MBB_NCM, "%s:\n", __func__);

    ncm->params.resp_avail = NULL;
    ncm->params.v = NULL;
}

static USB_INT ncm_set_alt(struct usb_function* f, USB_UINT intf, USB_UINT alt)
{
    struct f_ncm*        ncm = func_to_ncm(f);
    struct usb_composite_dev* cdev = f->config->cdev;

    DBG_T(MBB_NCM, "ncm_set_alt: intf=%u, alt=%u, ctrl_id=%u, data_id=%u\n",
          intf, alt, ncm->ctrl_id, ncm->data_id);

    /* Control interface has only altsetting 0 */
    if (intf == ncm->ctrl_id && alt == 0 )
    {
#ifdef CONFIG_NCM_MBIM_SUPPORT
        if (VERIFY_NCM_MBIM(ncm) && (NCM_MBIM_MODE_MAX == gMbimDebugMode))
        {
            gMbimDebugMode = NCM_MBIM_MODE_NORMAL;
            DBG_I(MBB_NCM, "U_NCM:gMbimDebugMode:%d", gMbimDebugMode);
        }
#endif

        if (ncm->notify->driver_data)
        {
            DBG_I(MBB_NCM, "reset ncm control %d\n", intf);
            usb_ep_disable(ncm->notify);
        }

        if (!(ncm->notify->desc))
        {
            DBG_I(MBB_NCM, "init ncm ctrl %d\n", intf);

            if (config_ep_by_speed(cdev->gadget, f, ncm->notify))
            {
                goto fail;
            }
        }

        (USB_VOID)usb_ep_enable(ncm->notify);
        ncm->notify->driver_data = ncm;

        bsp_usb_set_enum_stat(ncm->data_id, 1);

        /* Data interface has has only altsetting 0 */
    }

    if (intf == ncm->data_id)
    {
        if (alt > 1)
        {
            goto fail;
        }

        if (VERIFY_NCM_BYPASS(ncm))
        {
            ncm_empty_resp_queue(ncm);
            adp_usb_queue_delay_work(&(ncm->disconnect_work), DISCONNECT_TIME_DELAY);
        }

        if (ncm->port.in_ep->driver_data)
        {
            DBG_I(MBB_NCM, "reset ncm\n");
            gether_disconnect(&ncm->port);
            ncm_reset_values(ncm);
        }

        /*
         * CDC Network only sends data in non-default altsettings.
         * Changing altsettings resets filters, statistics, etc.
         */
        if (alt == 1)
        {
            struct net_device*    net;

            if (!ncm->port.in_ep->desc || !ncm->port.out_ep->desc)
            {
                DBG_I(MBB_NCM, "init ncm\n");

                if (config_ep_by_speed(cdev->gadget, f, ncm->port.in_ep)
                    || config_ep_by_speed(cdev->gadget, f, ncm->port.out_ep))
                {
                    ncm->port.in_ep->desc = NULL;
                    ncm->port.out_ep->desc = NULL;
                    goto fail;
                }
            }

            /* TODO */
            /* Enable zlps by default for NCM conformance;
             * override for musb_hdrc (avoids txdma ovhead)
             */
            ncm->port.is_zlp_ok = !(gadget_is_musbhdrc(cdev->gadget));
            ncm->port.cdc_filter = DEFAULT_FILTER;
            DBG_I(MBB_NCM, "activate ncm\n");
            net = gether_connect(&ncm->port);

            if (IS_ERR(net))
            {
                return PTR_ERR(net);
            }

            if (VERIFY_NCM_BYPASS(ncm))
            {
                wake_up_process(ncm->vendor_ctx.tx_task);
            }

            ncm->port.notify = ncm->notify;
            ncm->port.header_room_resved_ptr = &ncm->header_room_resved;
        }

        spin_lock(&ncm->lock);
        ncm_notify(ncm);
        spin_unlock(&ncm->lock);
        bsp_usb_set_enum_stat(ncm->data_id, 1);
    }

    if (!VERIFY_NCM_BYPASS(ncm))
    {
        usb_notify_syswatch(DEVICE_ID_USB, USB_ENABLE_NOTIFY);
        /*设置连接状态为连接*/
        USB_ETH_LinkStatSet( TRUE );
    }

    ncm->curr_alt = alt;

    return 0;
fail:
    ncm->curr_alt = 0;
    return -EINVAL;
}

/*
 * Because the data interface supports multiple altsettings,
 * this NCM function *MUST* implement a get_alt() method.
 */
static USB_INT ncm_get_alt(struct usb_function* f, USB_UINT intf)
{
    struct f_ncm*        ncm = func_to_ncm(f);

    if (intf == ncm->ctrl_id)
    { return 0; }
    return ncm->port.in_ep->driver_data ? 1 : 0;
}

USB_VOID ncm_free_ntbnode(ntb_node_t* ntb_node)
{
    if (ntb_node)
    {
        if (ntb_node->addr)
        {
            kfree(ntb_node->addr);
        }

        kfree(ntb_node);
    }
}

ntb_node_t* ncm_alloc_ntbnode(USB_UINT32 size, gfp_t gfp_msk)
{
    ntb_node_t* ntb_node;

    ntb_node = (ntb_node_t*)kzalloc(sizeof(ntb_node_t), gfp_msk);
    if (!ntb_node)
    {
        goto fail;
    }

    ntb_node->addr = kzalloc(size, gfp_msk);
    if (!ntb_node->addr)
    {
        goto fail;
    }

    INIT_LIST_HEAD(&ntb_node->entry);

    return ntb_node;

fail:
    ncm_free_ntbnode(ntb_node);

    return NULL;
}


USB_UINT32 ncm_get_ntbnode(tx_ctx_t* ctx, USB_UINT32 size, gfp_t gfp_msk)
{
#ifdef CONFIG_GNET_PREALLOC_TX_MEM
    return 0;
#else
    ntb_node_t* ntb_node = NULL;

    ntb_node = ncm_alloc_ntbnode(size, gfp_msk);
    if (!ntb_node)
    { return -ENOMEM; }

    ctx->ntb = ntb_node;

    return 0;
#endif
}

USB_VOID ncm_put_ntbnode(tx_ctx_t* ctx)
{
#ifdef CONFIG_GNET_PREALLOC_TX_MEM
#else
    if (ctx->ntb)
    {
        ncm_free_ntbnode(ctx->ntb);
        ctx->ntb = NULL;
    }
#endif
}

USB_UINT32 ncm_pad_for_skb(struct sk_buff* skb, USB_UINT32 ofs, USB_UINT32 div, USB_UINT32 rem, bool is_crc)
{
    USB_UINT32 pad = 0;

    if (!is_crc)
    {
        ofs += skb->len;
        pad = ALIGN(ofs, div) + rem - ofs;  /* pad for ncm align */
        if (unlikely((skb->tail + pad) > skb->end))
        {
            GNET_TRACE(("ncm_pad_for_skb: pad=%lu, headroom=%u, tailroom=%u\n",
                        pad, skb_headroom(skb), skb_tailroom(skb)));

            /*if (likely(skb_headroom(skb) >= pad))
            {
                skb_push(skb, pad);
                memmove(skb->data, ((USB_PCHAR)(skb->data) + pad), (skb->len - pad));
                return pad;
            }*/

            pad = (NCM_PAD_ERR_FLAG | pad);  /*means no tailroom*/
        }
        else
        {
            (USB_VOID)skb_put(skb, pad);
        }

        return pad;
    }
    else
    {
        USB_UINT32 crc = 0;
        USB_UINT32 pad0 = 0;
        USB_UINT32 pad1 = 0;

        /* pad for 802.3 */
        pad0 = (skb->len < ETH_ZLEN) ? (ETH_ZLEN - skb->len) : 0;
        ofs += pad0;
        pad += pad0;

        ofs += 4;   /* 4 bytes for crcs32 */
        pad += 4;

        /* pad for ncm align */
        pad1 = ALIGN(ofs, div) + rem - ofs;
        ofs += pad1;
        pad += pad1;

        if (unlikely((skb->tail + pad) > skb->end))
        {
            pad |= NCM_PAD_ERR_FLAG;  /* means no tailroom */
            DBG_I(MBB_NCM, "ofs = %ld\n", ofs);
        }
        else
        {
            /* calc crc */
            crc = ~crc32_le(~0, skb->data, skb->len + pad0);
            put_unaligned_le32(crc, skb->data + skb->len + pad0);
            (void)skb_put(skb, pad);
        }

        return pad;
    }
}

struct sk_buff* ncm_skb_expand(tx_ctx_t* ctx, struct sk_buff* skb, u32 pad, u32 div)
{
    struct eth_dev* dev = ctx->dev;
    struct sk_buff*  n = NULL;

    /* alloc more div bytes */
    n = skb_copy_expand(skb, skb_headroom(skb),
                        (skb_tailroom(skb) + NCM_PAD_TRUE_VAL(pad) + div), GFP_ATOMIC);
    if (!n)
    {
        GNET_ERR(("%s line %d:no room for new skb!\n", __FUNCTION__, __LINE__));
        dev->ctx.stats.tx_skb_tailrom_lack++;
        ctx->pkt_cnt--;
    }
    else
    {
        dev->ctx.stats.tx_skb_tailrom_expand++;
        __skb_insert(n, skb->prev, skb, &ctx->pkt_list);
    }

    __skb_unlink(skb, &ctx->pkt_list);
    dev_kfree_skb_any(skb);
    return n;
}

struct usb_request* ncm_skbs_to_ntb(struct gether* port, struct usb_request* req)
{
#define TRB_CACHE_QUIRK_UPDATE_BYTES(trbs,bytes,maxpacket)\
    do\
    {\
        if((bytes) >= (maxpacket))\
        {\
            (bytes) &= ((maxpacket)-1);\
            (trbs) = (bytes)?1:0;\
        }\
    }while(0)
    struct f_ncm* ncm = func_to_ncm(&port->func);
    struct eth_dev* dev = port->ioport;
    tx_ctx_t* ctx = (tx_ctx_t*)req->context;
    struct sk_buff*    skb_loop;
    struct sk_buff*    skb_next;
    USB_UINT32 ncb_len = 0;
    USB_UINT32 ntb_len = 0;
    USB_UINT32 last_ntb_len = 0;
    USB_UINT32 last_skb_len = 0;
    USB_UINT32  pad = 0;
    __le16*    tmp;
    __le16*    tmp_ntb_length;
    USB_INT32   div = le16_to_cpu(ntb_parameters.wNdpInDivisor);
    USB_INT32   rem = le16_to_cpu(ntb_parameters.wNdpInPayloadRemainder);
    USB_INT32   ndp_align = le16_to_cpu(ntb_parameters.wNdpInAlignment);
    USB_INT32   ndp_pad;
    USB_UINT32 max_size = ncm->port.fixed_in_len;
    struct ndp_parser_opts* opts = ncm->parser_opts;
    USB_UINT32 cnt = 0;
    USB_UINT32 trbs_for_quirk = 0;
    USB_UINT32 bytes_for_quirk = 0;
    USB_UINT32 maxpacket = port->in_ep->maxpacket;
#ifdef CONFIG_NCM_MBIM_SUPPORT
    ncm_mpdp_ndp_info_t tx_pdp_info[GNET_USED_NUM] = {{0}};
    USB_UINT32          pdp_number = 0;
    USB_UINT32          tx_total_pdp = 0;
    ncm_skb_cb_s*       skb_cb = NULL;
#endif

    /* alloc scatter-gather list */
    if (gnet_get_sgnode(ctx, ctx->pkt_cnt + 1 + dev->ctx.sg_node_reserve, GFP_ATOMIC))  /* add 1 to point to ntb header */
    {
        dev->ctx.stats.tx_no_sg++;
        GNET_BUG(("can't get sgnode\n"), 1);

        return NULL;
    }

#ifdef CONFIG_NCM_MBIM_SUPPORT
    /* Scan all skb pkt, check the pdp channel */
    if (VERIFY_NCM_MBIM(ncm))
    {
        skb_queue_walk_safe(&ctx->pkt_list, skb_loop, skb_next)
        {
            skb_cb = (ncm_skb_cb_s*)skb_loop->cb;
            pdp_number = skb_cb->PdpChannel;

            if (GNET_USED_NUM <= pdp_number)
            {
                NCM_DBG_PRINT_TX("Invalid pdp nubmer %u\n", pdp_number);
                return NULL;
            }

            if (0 == tx_pdp_info[pdp_number].tx_pkt_num)
            {
                tx_total_pdp++;
                opts->ndp_sign = (opts->ndp_sign & 0x00FFFFFF) | (pdp_number << 24);
            }
            tx_pdp_info[pdp_number].tx_pkt_num++;

            NCM_DBG_PRINT_TX("pdp_number=%u, pkt_num=%u, tx_total_pdp=%u, ndp_sign=0x%08X\n",
                             pdp_number, tx_pdp_info[pdp_number].tx_pkt_num, tx_total_pdp, opts->ndp_sign);
        }
    }
#endif

    /* calc ntb header length */
    ncb_len += opts->nth_size;
    ndp_pad = ALIGN(ncb_len, ndp_align) - ncb_len;
    ncb_len += ndp_pad;

#ifdef CONFIG_NCM_MBIM_SUPPORT
    /* Calc NDP size for Multiple PDP */
    if (VERIFY_NCM_MBIM(ncm) && (MBIM_MPDP_LOW_WATERLINE < tx_total_pdp))
    {
        USB_UINT32 Index = 0;
        for (Index = 0; Index < GNET_USED_NUM; Index++)
        {
            if (0 == tx_pdp_info[Index].tx_pkt_num)
            {
                continue;
            }

            tx_pdp_info[Index].offset = ncb_len;

            ncb_len += opts->ndp_size;
            ncb_len += 2 * 2 * opts->dgram_item_len * tx_pdp_info[Index].tx_pkt_num; /* Datagram entry */
            ncb_len += 2 * 2 * opts->dgram_item_len; /* Zero datagram entry */
            tx_pdp_info[Index].next_ndp = ncb_len;

            NCM_DBG_PRINT_TX("pdp_number=%u, pkt_num=%u, Offset=%u, next=%u\n", Index,
                             tx_pdp_info[Index].tx_pkt_num, tx_pdp_info[Index].offset, tx_pdp_info[Index].next_ndp);
        }

        pad = ALIGN(ncb_len, div) + rem - ncb_len;
        ncb_len += pad;

        goto Fill_Header;
    }
#endif

    ncb_len += opts->ndp_size;
    ncb_len += 2 * 2 * opts->dgram_item_len * ctx->pkt_cnt ; /* Datagram entry */
    ncb_len += 2 * 2 * opts->dgram_item_len; /* Zero datagram entry */
    pad = ALIGN(ncb_len, div) + rem - ncb_len;
    ncb_len += pad;

#ifdef CONFIG_NCM_MBIM_SUPPORT
Fill_Header :
#endif

    /* alloc ntb header */
    if (ncm_get_ntbnode(ctx, ncb_len, GFP_ATOMIC))
    {
        dev->ctx.stats.tx_no_ntb++;
        GNET_BUG(("can't get ntbnode\n"), 1);

        return NULL;
    }

    /* fill ntb header base info
     */
    /* NTH */
    tmp = (__le16*)ctx->ntb->addr;
#ifdef CONFIG_NCM_MBIM_SUPPORT
    if (VERIFY_NCM_MBIM(ncm) && (MBIM_MPDP_LOW_WATERLINE < tx_total_pdp))
    {
        memset(tmp, 0, ncb_len);
    }
#endif
    put_unaligned_le32(opts->nth_sign, tmp); /* dwSignature */
    tmp += 2;
    put_unaligned_le16(opts->nth_size, tmp++);  /* wHeaderLength */
    tmp++; /* skip wSequence */
    tmp_ntb_length = tmp;
    tmp += opts->block_length; /* skip (d)wBlockLength */
    /* (d)wFpIndex */
    /* the first pointer is right after the NTH + align */
    put_ncm(&tmp, opts->fp_index, opts->nth_size + ndp_pad);

    /* NDP */
    tmp = (__le16*)((USB_UINT32)tmp + ndp_pad);

#ifdef CONFIG_NCM_MBIM_SUPPORT
    if (VERIFY_NCM_MBIM(ncm) && (MBIM_MPDP_LOW_WATERLINE < tx_total_pdp))
    {
        USB_UINT32 Index = 0;
        USB_UINT32 ndp_sign = 0;
        USB_UINT32 total_num = tx_total_pdp;
        for (Index = 0; Index < GNET_USED_NUM; Index++)
        {
            if (0 == tx_pdp_info[Index].tx_pkt_num)
            {
                continue;
            }
            tmp = (__le16*)((USB_PUCHAR)(ctx->ntb->addr) + tx_pdp_info[Index].offset);
            tx_pdp_info[Index].data_gram_ptr = ((USB_PUCHAR)tmp + opts->ndp_size);
            total_num--;

            NCM_DBG_PRINT_TX("pdp_number=%u, ndp_ptr=%p, data_gram_ptr=%p\n",
                             Index, tmp, tx_pdp_info[Index].data_gram_ptr);

            ndp_sign = (opts->ndp_sign & 0x00FFFFFF) | (Index << 24);
            put_unaligned_le32(ndp_sign, tmp); /* dwSignature */
            tmp += 2;

            put_unaligned_le16(tx_pdp_info[Index].next_ndp - tx_pdp_info[Index].offset, tmp++);/* wLength */

            tmp += opts->reserved1;
            if (0 < total_num)
            {
                put_ncm(&tmp, opts->dgram_item_len, tx_pdp_info[Index].next_ndp);
            }
            else
            {
                put_ncm(&tmp, opts->dgram_item_len, 0);
            }
            tmp += opts->reserved2;
        }
        goto Fill_Data;
    }
#endif

    put_unaligned_le32(opts->ndp_sign, tmp); /* dwSignature */
    tmp += 2;

    put_unaligned_le16(ncb_len - opts->nth_size - pad - ndp_pad, tmp++);/* wLength */

    tmp += opts->reserved1;
    tmp += opts->next_fp_index; /* skip reserved (d)wNextFpIndex */
    tmp += opts->reserved2;

#ifdef CONFIG_NCM_MBIM_SUPPORT
Fill_Data :
#endif

    /* fill ntb header */
    cnt = 1;
    ntb_len = ncb_len;
    ctx->sg_node->actual = 0;

    if (dev->ctx.quirks & GNET_NCM_QUIRK_DWC_TRB_CACHE)
    {
        bytes_for_quirk = ncb_len;
        trbs_for_quirk = 1;
        if (bytes_for_quirk >= maxpacket)
        {
            TRB_CACHE_QUIRK_UPDATE_BYTES(trbs_for_quirk, bytes_for_quirk, maxpacket);
        }
    }

    skb_queue_walk_safe(&ctx->pkt_list, skb_loop, skb_next)
    {
        last_ntb_len = ntb_len;
        last_skb_len = skb_loop->len;
        pad = ncm_pad_for_skb(skb_loop, ntb_len, div, rem, ncm->is_crc);

        if (unlikely(NCM_PAD_ERR_CHECK(pad)))
        {
            skb_loop = ncm_skb_expand(ctx, skb_loop, pad, div);
            if (unlikely(!skb_loop))
            {
                continue;
            }

            pad = ncm_pad_for_skb(skb_loop, ntb_len, div, rem, ncm->is_crc);
        }

        ntb_len += skb_loop->len;

        if (dev->ctx.quirks & GNET_NCM_QUIRK_DWC_TRB_CACHE)
        {
            bytes_for_quirk += ntb_len - last_ntb_len;
            if (bytes_for_quirk >= maxpacket)
            {
                TRB_CACHE_QUIRK_UPDATE_BYTES(trbs_for_quirk, bytes_for_quirk, maxpacket);
            }
            else
            {
                USB_UINT32 trbs_for_quirk_tmp = trbs_for_quirk;
                USB_UINT32 bytes_for_quirk_tmp = bytes_for_quirk;
                USB_UINT32 ntb_len_tmp = last_ntb_len;
                USB_UINT32 actual_tmp = 0;

                trbs_for_quirk++;    /* try to add current trb */
                if ((USB_CDC_NCM_DWC_CACHE_TRB_NUM - 1) == trbs_for_quirk)
                {
                    /* temp give up current skb */
                    bytes_for_quirk -= ntb_len - last_ntb_len;
                    skb_trim(skb_loop, last_skb_len);

                    ntb_len = last_ntb_len;
                    ntb_len += maxpacket - bytes_for_quirk; /* fill to maxpacket */
#if 0   /* when quirk occur,align is not necessary */
                    pad = ALIGN(ntb_len, div) + rem - ntb_len;  /* pad for ncm align */
                    ntb_len += pad;
#endif
                    GNET_TRACE(("line %d:sg[%ld]buf 0x%lx len 0x%lx\n", __LINE__, cnt, (USB_UINT32)dev->ctx.dwc_trb_cache_buf, ntb_len - last_ntb_len));
                    sg_set_buf(&ctx->sg_node->sg_tbl.sgl[cnt], (USB_VOID*)dev->ctx.dwc_trb_cache_buf, ntb_len - last_ntb_len);
                    actual_tmp = ctx->sg_node->actual;
                    ctx->sg_node->actual += ntb_len - last_ntb_len;
                    cnt++;

                    bytes_for_quirk += ntb_len - last_ntb_len;
                    TRB_CACHE_QUIRK_UPDATE_BYTES(trbs_for_quirk, bytes_for_quirk, maxpacket);

                    last_ntb_len = ntb_len;
                    last_skb_len = skb_loop->len;
                    pad = ncm_pad_for_skb(skb_loop, ntb_len, div, rem, ncm->is_crc);

                    if (unlikely(NCM_PAD_ERR_CHECK(pad)))
                    {
                        skb_loop = ncm_skb_expand(ctx, skb_loop, pad, div);
                        if (unlikely(!skb_loop))
                        {
                            trbs_for_quirk = trbs_for_quirk_tmp;
                            bytes_for_quirk = bytes_for_quirk_tmp;
                            ntb_len = ntb_len_tmp;
                            ctx->sg_node->actual = actual_tmp;
                            cnt--;
                            continue;
                        }

                        pad = ncm_pad_for_skb(skb_loop, ntb_len, div, rem, ncm->is_crc);
                    }

                    ntb_len += skb_loop->len;
                    trbs_for_quirk++;
                    bytes_for_quirk += ntb_len - last_ntb_len;
                    TRB_CACHE_QUIRK_UPDATE_BYTES(trbs_for_quirk, bytes_for_quirk, maxpacket);
                }
            }
        }

        dev->ctx.stats.tx_gathered_bytes += skb_loop->len;

#ifdef CONFIG_NCM_MBIM_SUPPORT
        /* Reset the datagram info offset */
        if (VERIFY_NCM_MBIM(ncm) && (MBIM_MPDP_LOW_WATERLINE < tx_total_pdp))
        {
            skb_cb = (ncm_skb_cb_s*)skb_loop->cb;
            pdp_number = skb_cb->PdpChannel;
            tmp = (__le16*)(tx_pdp_info[pdp_number].data_gram_ptr);
            tx_pdp_info[pdp_number].data_gram_ptr += 2 * 2 * opts->dgram_item_len;
        }
#endif

        put_ncm(&tmp, opts->dgram_item_len, last_ntb_len);          /* (d)wDatagramIndex[i] */
        put_ncm(&tmp, opts->dgram_item_len, skb_loop->len - pad);   /* (d)wDatagramLength[i] */

        GNET_TRACE(("line %d:sg[%ld]buf 0x%lx len 0x%lx\n", __LINE__, cnt, (USB_UINT32)skb_loop->data, ntb_len - last_ntb_len));
        sg_set_buf(&ctx->sg_node->sg_tbl.sgl[cnt], (const USB_VOID*)skb_loop->data, ntb_len - last_ntb_len);
        ctx->sg_node->actual += ntb_len - last_ntb_len;
        cnt++;
    }

    /* mark end */
    put_ncm(&tmp, opts->dgram_item_len, 0); /* (d)wDatagramIndex[i] */
    put_ncm(&tmp, opts->dgram_item_len, 0); /* (d)wDatagramIndex[i] */

    ctx->ntb->actual = ncb_len;
    GNET_TRACE(("line %d:sg[%d]buf 0x%lx len 0x%lx\n", __LINE__, 0, (USB_UINT32)ctx->ntb->addr, ctx->ntb->actual));
    sg_set_buf(&ctx->sg_node->sg_tbl.sgl[0], (const USB_VOID*)ctx->ntb->addr, ctx->ntb->actual);     /* let usb core flush cache */
    ctx->sg_node->sg_tbl.nents = cnt;
    if ((ctx->ntb->actual + ctx->sg_node->actual) > max_size)
    {
        dev->ctx.stats.tx_ntb_overflow++;
        GNET_BUG(("ntb overflow\n"), 1);

        return NULL;
    }
    GNET_BUG(("sg node overflow:%ld,%ld,%ld.\n", cnt, ctx->pkt_cnt, dev->ctx.sg_node_reserve),
             cnt >= (ctx->pkt_cnt + 1 + dev->ctx.sg_node_reserve));

    put_ncm(&tmp_ntb_length, opts->block_length, ntb_len); /* (d)wBlockLength */

    return req;
}

static struct sk_buff* ncm_wrap_ntb(struct gether* port,
                                    struct sk_buff* skb)
{
    struct eth_dev* dev = port->ioport;
    struct usb_request* req;
    tx_ctx_t* ctx;
    USB_ULONG flags;
    USB_UINT32 pkt_len_waterline =
        min(port->fixed_in_len - GNET_NCM_ROOM_FOR_NTB_INMAXSIZE, dev->ctx.pkt_len_waterline);

    if (skb) /* caller is xmit */
    {
        spin_lock_irqsave(&dev->tx_req_lock, flags);
        if (list_empty_careful(&dev->tx_reqs))
        {
            spin_unlock_irqrestore(&dev->tx_req_lock, flags);
            dev_kfree_skb_any(skb);
            dev->ctx.stats.tx_noreq_inwrap++;
            return NULL;
        }

        req = list_first_entry(&dev->tx_reqs, struct usb_request, list);
        ctx = (tx_ctx_t*)req->context;

        if (skb_queue_empty(&ctx->pkt_list))
        {
            /* start timer */
            dev->ctx.timer.expires = jiffies + dev->ctx.timeout;  /* jiffies unit */
            if (!timer_pending(&dev->ctx.timer))
            {
                add_timer(&dev->ctx.timer);
                dev->ctx.stats.tx_timer_activated++;
            }
        }

        /* pkt enqueue */
        skb->used_check = GNET_MAGIC_SKB_USED;
        __skb_queue_tail(&ctx->pkt_list, skb);

        /* update packets info */
        ctx->pkt_cnt++;
        ctx->pkt_len += skb->len;

        if ((!skb->psh)
            && (ctx->pkt_cnt < dev->ctx.pkt_cnt_waterline)
            && (ctx->pkt_len < pkt_len_waterline))
        {
            spin_unlock_irqrestore(&dev->tx_req_lock, flags);
            return (struct sk_buff*)GNET_MAGIC_WRAPING;
        }
        else
        {
            /* if waterline reach, construct a ntb wrap */
            del_timer(&dev->ctx.timer); /* delete timer */
            if (skb->psh)
            {
                dev->ctx.stats.tx_psh++;
            }

            dev->ctx.stats.tx_timer_activated--;
            list_del(&req->list);
            spin_unlock_irqrestore(&dev->tx_req_lock, flags);
            GNET_INFO(("%s [xmit]:waterline reach,ctx=%p\n", __FUNCTION__, ctx));
        }
    }
    else    /* caller is timeout */
    {
        GNET_INFO(("%s [timeout]:caller is timeout.\n", __FUNCTION__));

        spin_lock_irqsave(&dev->tx_req_lock, flags);
        if (!list_empty_careful(&dev->tx_done_reqs))   /* ready to send */
        {
            spin_unlock_irqrestore(&dev->tx_req_lock, flags);
            GNET_INFO(("%s line %d:ready to send.\n", __FUNCTION__, __LINE__));
            return (struct sk_buff*)GNET_MAGIC_WRAPED;
        }

        if (list_empty_careful(&dev->tx_reqs)) /* no tx req */
        {
            spin_unlock_irqrestore(&dev->tx_req_lock, flags);
            GNET_ERR(("%s line %d:no tx req.\n", __FUNCTION__, __LINE__));
            dev->ctx.stats.tx_bug_noreq++;
            return (struct sk_buff*)GNET_MAGIC_WRAPING;
        }

        req = list_first_entry(&dev->tx_reqs, struct usb_request, list);
        ctx = (tx_ctx_t*)req->context;
        if (skb_queue_empty(&ctx->pkt_list))
        {
            spin_unlock_irqrestore(&dev->tx_req_lock, flags);
            GNET_ERR(("%s line %d:no packet.\n", __FUNCTION__, __LINE__));
            dev->ctx.stats.tx_bug_nopkt++;
            return (struct sk_buff*)GNET_MAGIC_WRAPING;
        }

        list_del(&req->list);
        spin_unlock_irqrestore(&dev->tx_req_lock, flags);
    }

    if (ncm_skbs_to_ntb(port, req)) /* wrap skbs to ntb according to ncm protocol */
    {
        /* push req to done queue */
        spin_lock_irqsave(&dev->tx_req_lock, flags);
        list_add_tail(&req->list, &dev->tx_done_reqs);
        spin_unlock_irqrestore(&dev->tx_req_lock, flags);
        return (struct sk_buff*)GNET_MAGIC_WRAPED;
    }
    else
    {
        GNET_ERR(("%s line %d:skbs to ntb fail.\n", __FUNCTION__, __LINE__));

        spin_lock_irqsave(&dev->tx_req_lock, flags);
        gnet_recycle_ctx_info((tx_ctx_t*)req->context, 0);   /* memory recycle */
        list_add_tail(&req->list, &dev->tx_reqs);  /* ctx enqueue */
        spin_unlock_irqrestore(&dev->tx_req_lock, flags);

        return NULL;
    }
}

struct sk_buff* g_skb = NULL;
static USB_INT32 ncm_unwrap_ntb(struct gether* port,
                                struct sk_buff* skb,
                                struct sk_buff_head* list)
{
    struct f_ncm*    ncm = func_to_ncm(&port->func);
    struct eth_dev*  dev = port->ioport;
    __le16*        tmp = (USB_VOID*) skb->data;
    USB_UINT32    index, index2;
    USB_UINT32    dg_len, dg_len2;
    USB_UINT32    ndp_len;
    struct sk_buff*    skb2  = NULL;
    USB_INT32       ret = -EINVAL;
    USB_UINT32    max_size = le32_to_cpu(ntb_parameters.dwNtbOutMaxSize);
    struct ndp_parser_opts* opts = ncm->parser_opts;
    USB_UINT32    crc_len = ncm->is_crc ? sizeof(USB_UINT32) : 0;
    USB_INT32       dgram_counter;
    USB_INT32    doclone = port->ioport->doclone;
#ifdef CONFIG_NCM_MBIM_SUPPORT
    ncm_skb_cb_s* skb_cb = (ncm_skb_cb_s*)skb->cb;
    USB_UINT32    ndpsig = 0;       /* IPS0 or NCM0, etc */
    USB_UINT32    nextndpidx = 0;
    USB_UINT32    ndp_sign = opts->ndp_sign;
#endif

    /* dwSignature */
    if (get_unaligned_le32(tmp) != opts->nth_sign)
    {
        DBG_E(MBB_NCM, "ncm_unwrap_ntb Wrong NTH SIGN, skblen %d\n", skb->len);
        print_hex_dump(KERN_INFO, "HEAD:", DUMP_PREFIX_ADDRESS, 32, 1,
                       skb->data, 32, false);

        goto err;
    }
    tmp += 2;
    /* wHeaderLength */
    if (get_unaligned_le16(tmp++) != opts->nth_size)
    {
        DBG_E(MBB_NCM, "ncm_unwrap_ntb Wrong NTB headersize\n");
        goto err;
    }
    tmp++; /* skip wSequence */

    /* (d)wBlockLength */
    if (get_ncm(&tmp, opts->block_length) > max_size)
    {
        DBG_E(MBB_NCM, "ncm_unwrap_ntb OUT size exceeded\n");
        goto err;
    }

    index = get_ncm(&tmp, opts->fp_index);
    /* NCM 3.2 */
    if (((index % 4) != 0) && (index < opts->nth_size))
    {
        DBG_E(MBB_NCM, "Bad index: %lx\n", index);
        goto err;
    }

#ifdef CONFIG_NCM_MBIM_SUPPORT  /* M-PDP */
Walk_Ndp :
#endif

    /* walk through NDP */
    tmp = ((USB_VOID*)skb->data) + index;

#ifdef CONFIG_NCM_MBIM_SUPPORT   /* M-PDP */
    ndpsig  = get_unaligned_le32(tmp);
    skb_cb->PdpChannel = 0;
    if (VERIFY_NCM_MBIM(ncm))
    {
        /* 主机字节序，第一个byte为PDP的session id，"IPS<session id>" */
        skb_cb->PdpChannel = (USB_UINT8)(ndpsig >> 24);
        skb_cb->PktType = NCM_PKT_TYPE_RAW;
        ndpsig = (0x00FFFFFF & ndpsig);
        ndp_sign = (0x00FFFFFF & ndp_sign);
        NCM_DBG_PRINT_RX("chanel=%u, sig=0x%08X\n", skb_cb->PdpChannel, ndpsig);
    }
    else
    {
        skb_cb->PktType = NCM_PKT_TYPE_ETH;
    }

    if (ndpsig != ndp_sign)
    {
        DBG_E(MBB_NCM, "WinBlue %s : Wrong NDP SIGN %x\n", __FUNCTION__, ndpsig);
        goto err;
    }
#else
    if (get_unaligned_le32(tmp) != opts->ndp_sign)
    {
        DBG_E(MBB_NCM, "ncm_unwrap_ntb Wrong NDP SIGN\n");
        goto err;
    }
#endif

    tmp += 2;

    ndp_len = get_unaligned_le16(tmp++);
    /*
     * NCM 3.3.1
     * entry is 2 items
     * item size is 16/32 bits, opts->dgram_item_len * 2 bytes
     * minimal: struct usb_cdc_ncm_ndpX + normal entry + zero entry
     */
    if ((ndp_len < opts->ndp_size + 2 * 2 * (opts->dgram_item_len * 2))
        || (ndp_len % opts->ndplen_align != 0))
    {
        DBG_E(MBB_NCM, "ncm_unwrap_ntb Bad NDP length: %lx\n", ndp_len);
        goto err;
    }
    tmp += opts->reserved1;
#ifdef CONFIG_NCM_MBIM_SUPPORT  /* M-PDP */
    if (VERIFY_NCM_MBIM(ncm))
    {
        nextndpidx = get_ncm(&tmp, opts->dgram_item_len);
        NCM_DBG_PRINT_RX("nextndpidx=%u\n", nextndpidx);
    }
    else
    {
        tmp += opts->next_fp_index;
    }
#else
    tmp += opts->next_fp_index; /* skip reserved (d)wNextFpIndex */
#endif
    tmp += opts->reserved2;

    ndp_len -= opts->ndp_size;
    index2 = get_ncm(&tmp, opts->dgram_item_len);
    dg_len2 = get_ncm(&tmp, opts->dgram_item_len);
    dgram_counter = 0;

    do
    {
        index = index2;
        dg_len = dg_len2;
        if (dg_len < 14 + crc_len
            || index >= skb->len
            || index + dg_len > skb->len
            || dg_len > ETH_FRAME_LEN + crc_len)  /* ethernet header + crc */
        {
            DBG_E(MBB_NCM, "ncm_unwrap_ntb Bad dgram length: %lx\n", dg_len);
            GNET_ERR(("unmatched dgram item:%ld,%ld,%d\n", index, dg_len, skb->len));
            /*unsigned char log_buf[64] = {0};
            sprintf(log_buf,"unmatched dgram item:%d,%d,%d\n",index,dg_len,skb->len);
            bsp_dump_save_file(DUMP_SAVE_FILE_NCM,log_buf,sizeof(log_buf),DUMP_SAVE_FILE_MODE_CREATE);
            bsp_dump_save_file(DUMP_SAVE_FILE_NCM,skb->data,skb->len,DUMP_SAVE_FILE_MODE_APPEND);*/
            if (gnet_dbg_level & GNET_DBG_RXUNWRAP)
            {
                g_skb = skb;
                BUG();
            }

            goto err;
        }

        if (ncm->is_crc)
        {
            USB_UINT32 crc, crc2;

            crc = get_unaligned_le32(skb->data +
                                     index + dg_len - crc_len);
            crc2 = ~crc32_le(~0,
                             skb->data + index,
                             dg_len - crc_len);
            if (crc != crc2)
            {
                DBG_E(MBB_NCM, "ncm_unwrap_ntb Bad CRC\n");
                goto err;
            }
        }

        index2 = get_ncm(&tmp, opts->dgram_item_len);
        dg_len2 = get_ncm(&tmp, opts->dgram_item_len);

        if (gnet_rx_need_protect(dev, skb->data + index))
        {
            break;
        }

#if !defined(CONFIG_GNET_PREALLOC_RX_MEM)
        if (index2 == 0 || dg_len2 == 0)
        {
            skb2 = skb;
        }
        else
#endif
        {
            if (doclone)
            { skb2 = skb_clone(skb, GFP_ATOMIC); }
            else
            { skb2 = gnet_copy_skb(skb, (USB_INT32)index, (USB_INT32)(dg_len - crc_len)); }

            if (NULL == skb2 )
            { goto err; }
        }

#ifdef CONFIG_NCM_MBIM_SUPPORT    /* M-PDP */
        if (VERIFY_NCM_MBIM(ncm))
        {
            ncm_skb_cb_s* dstskbcb = (ncm_skb_cb_s*)skb2->cb;
            dstskbcb->PktType = skb_cb->PktType;
            dstskbcb->PdpChannel = skb_cb->PdpChannel;
            NCM_DBG_PRINT_RX("add skb to channel=%u\n", dstskbcb->PdpChannel);
        }
#endif

        if (doclone)
        {
            if (!skb_pull(skb2, index))
            {
                ret = -EOVERFLOW;
                dev_kfree_skb_any(skb2);
                DBG_E(MBB_NCM, "%s skb2  skb_pull err!\n\t", __FUNCTION__);
                goto err;
            }

#if defined(CONFIG_GNET_PREALLOC_RX_MEM)
            /* revise the head to point to the start of the buffer totally owned by this skb.
             * For ncm mode, 16 bytes room is preserved before the position pointed by ndpidx
             * of every ndp, so we set the head pointer 16 bytes ahead of the data pointer.
             */
            if (!VERIFY_NCM_BYPASS(ncm))
            {
                skb2->head = skb2->data - ncm->header_room_resved;
            }
#endif

            skb_trim(skb2, dg_len - crc_len);
            if (!VERIFY_NCM_BYPASS(ncm))
            {
                if ((skb2 != skb)
                    && ip_is_fragment((struct iphdr*)(skb2->data + ETH_HLEN)))
                {
                    skb2->truesize = SKB_TRUESIZE(skb2->len);
                }

                make_clone_skb_writable(skb2);
            }
        }
        /* make sure the psh is cleared */
        skb2->psh = 0;

#ifdef CONFIG_NCM_MBIM_SUPPORT
        if (unlikely(ncm->LoopBackTest))
        {
            static USB_ULONG tx_no_req = 0;

            if (tx_no_req != dev->ctx.stats.tx_no_req)
            {
                /* Send tx pkt too fast, ep req is not enough */
                tx_no_req = dev->ctx.stats.tx_no_req;
                DBG_E(MBB_NCM, "%s: loopback tx_no_req inc to %u\n", __FUNCTION__, tx_no_req);
                (USB_VOID)eth_add_tx_req(dev);
                udelay(50);
            }

            /* Swap the src ip and dst ip */
            ncm_mbim_loopback_swap_ip_addr(skb2->data);

            /* Add to tx frames list, let tx_thread send it to host */
            skb_queue_tail(&ncm->vendor_ctx.tx_frames, skb2);
            ncm->vendor_ctx.stat_tx_post++;
            complete(&ncm->vendor_ctx.tx_kick);
        }
        else /* add to rx done list */
#endif
        {
            skb_queue_tail(list, skb2);
        }

        ndp_len -= 2 * (opts->dgram_item_len * 2);

        dgram_counter++;

        if (index2 == 0 || dg_len2 == 0)
        { break; }
    }
    while (ndp_len > 2 * (opts->dgram_item_len * 2));   /* zero entry */

#ifdef CONFIG_NCM_MBIM_SUPPORT  /* M-PDP */
    if (VERIFY_NCM_MBIM(ncm) && (0 != nextndpidx))
    {
        index = nextndpidx;
        nextndpidx = 0;
        goto Walk_Ndp;
    }
#endif

    /* set the psh for the last packet in the ndp */
    if (skb2)
    {
        skb2->psh = 1;
    }
    gnet_put_rx_skb_to_done(port->ioport, skb);

    GNET_TRACE(("Parsed NTB with %ld frames\n", dgram_counter));

    return 0;
err:
    skb_queue_purge(list);
    gnet_recycle_rx_skb(port->ioport, skb);

    return ret;
}

static USB_VOID ncm_disable(struct usb_function* f)
{
    struct f_ncm*        ncm = func_to_ncm(f);

    DBG_T(MBB_NCM, "ncm_disable enter\n");

    if (VERIFY_NCM_BYPASS(ncm))
    {
        ncm_empty_resp_queue(ncm);
        adp_usb_queue_delay_work(&(ncm->disconnect_work), DISCONNECT_TIME_DELAY);
    }

    if (ncm->port.in_ep->driver_data)
    { gether_disconnect(&ncm->port); }

    if (ncm->notify->driver_data)
    {
        usb_ep_disable(ncm->notify);
        ncm->notify->driver_data = NULL;
        ncm->notify->desc = NULL;
    }
    bsp_usb_set_enum_stat(ncm->data_id, 0);
}

static USB_VOID ncm_suspend(struct usb_function* f)
{
    struct eth_dev* dev = func_to_ethdev(f);
    struct f_ncm* ncm = func_to_ncm(f);

    DBG_T(MBB_NCM, "ncm_suspend enter\n");

#ifdef CONFIG_NCM_MBIM_SUPPORT
    if (VERIFY_NCM_MBIM(ncm))
    {
        USB_UINT suspendstate = 1;
        ncm_mbim_send_msg_to_mbim(ncm, MBIM_NCM_SUSPEND_MSG, &suspendstate, sizeof(USB_UINT));
    }
#endif

    if ((!ncm->curr_alt) && (!dev))
    {
        GNET_INFO(("ncm function disabled, skip the ncm adapter suspend process\n"));
        return;
    }

    if ((!VERIFY_NCM_MBIM(ncm)) && (VERIFY_NCM_BYPASS(ncm)))
    {
        adp_usb_queue_delay_work(&(ncm->disconnect_work), DISCONNECT_TIME_DELAY);
    }

    eth_suspend(dev);
}

static USB_VOID ncm_resume(struct usb_function* f)
{
    struct eth_dev* dev = func_to_ethdev(f);
    struct f_ncm* ncm = func_to_ncm(f);

    DBG_T(MBB_NCM, "ncm_resume enter\n");

#ifdef CONFIG_NCM_MBIM_SUPPORT
    if (VERIFY_NCM_MBIM(ncm))
    {
        USB_UINT suspendstate = 0;
        ncm_mbim_send_msg_to_mbim(ncm, MBIM_NCM_SUSPEND_MSG, &suspendstate, sizeof(USB_UINT));
    }
#endif

    if ((!ncm->curr_alt) && (!dev))
    {
        GNET_INFO(("ncm function disabled, skip the ncm adapter resume process\n"));
        return;
    }

    eth_resume(dev);

    if (ncm->connect_notify_blocked)
    {
        /* 补发在sleep前被丢弃的联网状态消息 */
        DBG_T(MBB_NCM, "resume connect notify\n");

        ncm->queued_notify_state = NCM_NOTIFY_CONNECT;
        adp_usb_queue_delay_work(&(ncm->notify_work), 0);

        ncm->connect_notify_blocked = 0;
    }
}

/*-------------------------------------------------------------------------*/

/*
 * Callbacks let us notify the host about connect/disconnect when the
 * net device is opened or closed.
 *
 * For testing, note that link states on this side include both opened
 * and closed variants of:
 *
 *   - disconnected/unconfigured
 *   - configured but inactive (data alt 0)
 *   - configured and active (data alt 1)
 *
 * Each needs to be tested with unplug, rmmod, SET_CONFIGURATION, and
 * SET_INTERFACE (altsetting).  Remember also that "configured" doesn't
 * imply the host is actually polling the notification endpoint, and
 * likewise that "active" doesn't imply it's actually using the data
 * endpoints for traffic.
 */

static USB_VOID ncm_open(struct gether* geth)
{
    struct f_ncm*        ncm = func_to_ncm(&geth->func);

    DBG_T(MBB_NCM, "%s\n", __func__);

    spin_lock(&ncm->lock);
    ncm->is_open = true;
    ncm_notify(ncm);
    spin_unlock(&ncm->lock);
}

static USB_VOID ncm_close(struct gether* geth)
{
    struct f_ncm*        ncm = func_to_ncm(&geth->func);

    DBG_T(MBB_NCM, "%s\n", __func__);

    spin_lock(&ncm->lock);
    ncm->is_open = false;
    ncm_notify(ncm);
    spin_unlock(&ncm->lock);
}

/*-------------------------------------------------------------------------*/

/* 在同时支持多种形态的NCM端口时，全局描述符经
常改变，需要缓存起来，每次调用ncm_bind时进行初始化 */
static USB_VOID ncm_restore_desc(USB_VOID)
{
    static USB_BOOL inited = false;
    static struct usb_interface_descriptor ctl_intf_desc = {0};
    static struct usb_interface_descriptor data_intf_desc = {0};
    static struct usb_interface_descriptor nop_intf_desc = {0};
    static struct usb_cdc_ether_desc desc_ncm_ecm = {0};

    if (inited)
    {
        memcpy(&ncm_control_intf, &ctl_intf_desc, sizeof(struct usb_interface_descriptor));
        memcpy(&ncm_data_intf, &data_intf_desc, sizeof(struct usb_interface_descriptor));
        memcpy(&ncm_data_nop_intf, &nop_intf_desc, sizeof(struct usb_interface_descriptor));
        memcpy(&ncm_ecm_desc, &desc_ncm_ecm, sizeof(struct usb_cdc_ether_desc));
    }
    else
    {
        memcpy(&ctl_intf_desc, &ncm_control_intf, sizeof(struct usb_interface_descriptor));
        memcpy(&data_intf_desc, &ncm_data_intf, sizeof(struct usb_interface_descriptor));
        memcpy(&nop_intf_desc, &ncm_data_nop_intf, sizeof(struct usb_interface_descriptor));
        memcpy(&desc_ncm_ecm, &ncm_ecm_desc, sizeof(struct usb_cdc_ether_desc));
        inited = true;
    }
}

/* ethernet function driver setup/binding */
//static int __init
static USB_INT
ncm_bind(struct usb_configuration* c, struct usb_function* f)
{
    struct usb_composite_dev* cdev = c->cdev;
    struct f_ncm*        ncm = func_to_ncm(f);
    USB_INT           status;
    struct usb_ep*        ep;
    struct usb_descriptor_header** desc;
    mbb_usb_nv_info_st* usb_nv_info = usb_nv_get_ctx();
#ifdef NCM_ENCAP_CMD_CID_ENABLED
    huawei_dynamic_info_st* pnp_info = get_pnp_info();
#endif

    DBG_T(MBB_NCM, "config %u\n", c->bConfigurationValue);

    if (USB_NUM_2 == c->bConfigurationValue)    /* config 2 for MBIM */
    {
        ncm->work_mode = NCM_WORK_MODE_MBIM;
    }
    else if (NCM_MBIM_MODE_MAX != gMbimDebugMode)
    {
        ncm->work_mode = NCM_WORK_MODE_MBIM;
    }
    else if (get_gateway_mode())
    {
        ncm->work_mode = NCM_WORK_MODE_GATEWAY;
    }
    else if (!ncm_get_bypass_state())
    {
        ncm->work_mode = NCM_WORK_MODE_NET_BRIDGE;
    }
    else
    {
        ncm->work_mode = NCM_WORK_MODE_NORMAL;
    }

    ncm->vendor_ctx.is_mbim_mode = VERIFY_NCM_MBIM(ncm);
    ncm->EncapRspBlocked = FALSE;

    status = ncm_bind_func(ncm->vendor_ctx.vendor_priv);
    if (status < 0)
    {
        goto fail;
    }

    /* 重置全局描述符 */
    ncm_restore_desc();

    /* allocate instance-specific interface IDs */
    status = usb_interface_id(c, f);
    if (status < 0)
    { goto fail; }
    ncm->ctrl_id = status;
    ncm_iad_desc.bFirstInterface = status;

    ncm_control_intf.bInterfaceNumber = status;
    ncm_union_desc.bMasterInterface0 = status;

    if (NCM_WORK_MODE_MBIM == ncm->work_mode)
    {
        ncm_iad_desc.bFunctionSubClass = 0x0E;
        ndp16_opts.ndp_sign = USB_CDC_MBIM_NDP16_NOCRC_SIGN;
        ndp32_opts.ndp_sign = USB_CDC_MBIM_NDP32_NOCRC_SIGN;
        ncm_control_intf.bInterfaceSubClass = 0x0E;
        ncm_data_nop_intf.bInterfaceProtocol = USB_CDC_MBIM_PROTO_NTB;
        ncm_data_intf.bInterfaceProtocol = USB_CDC_MBIM_PROTO_NTB;

        ncm->is_single_interface = false;
    }
    else if (NCM_WORK_MODE_GATEWAY == ncm->work_mode)
    {
        ncm_control_intf.bInterfaceClass = USB_CLASS_COMM;
        ncm_control_intf.bInterfaceSubClass = USB_CDC_SUBCLASS_NCM;
        ncm_control_intf.bInterfaceProtocol = USB_IF_PROTOCOL_VOID;

        ncm_data_intf.bInterfaceClass = USB_CLASS_CDC_DATA;
        ncm_data_intf.bInterfaceSubClass = 0x00;
        ncm_data_intf.bInterfaceProtocol = 0x01;

        ndp16_opts.ndp_sign = USB_CDC_NCM_NDP16_NOCRC_SIGN;
        ndp32_opts.ndp_sign = USB_CDC_NCM_NDP32_NOCRC_SIGN;
        ncm->is_single_interface = false;
    }
    else
    {
        ncm_control_intf.bInterfaceClass = USB_CLASS_VENDOR_SPEC;

        ncm_control_intf.bInterfaceProtocol = USB_IF_PROTOCOL_NCM;
        ncm_data_intf.bInterfaceClass = USB_CLASS_VENDOR_SPEC;


        ncm_data_intf.bInterfaceProtocol = USB_IF_PROTOCOL_NCM;
        ncm_data_intf.bNumEndpoints = 3;  /* notify + in + out */
        ndp16_opts.ndp_sign = USB_CDC_NCM_NDP16_NOCRC_SIGN;
        ndp32_opts.ndp_sign = USB_CDC_NCM_NDP32_NOCRC_SIGN;

        if ((LINUX_OS_FLAG == usb_pnp_system_type_get())
            || (GATEWAY_OS_FLAG == usb_pnp_system_type_get()))
        {
            ncm_control_intf.bInterfaceSubClass = USB_SUBCLASS_LINUX;
            ncm_data_intf.bInterfaceSubClass = USB_SUBCLASS_LINUX;
        }
        else
        {
            ncm_control_intf.bInterfaceSubClass = USB_SUBCLASS_CODE;
            ncm_data_intf.bInterfaceSubClass = USB_SUBCLASS_CODE;
        }
        /*设置自定义协议字段*/
        if (0 != ncm->protocal)
        {
            ncm_control_intf.bInterfaceProtocol =  ncm->protocal;
            ncm_data_intf.bInterfaceProtocol = ncm->protocal;
        }
        else
        {
            ncm_control_intf.bInterfaceProtocol =  USB_IF_PROTOCOL_NCM;
            ncm_data_intf.bInterfaceProtocol =  USB_IF_PROTOCOL_NCM;
        }
        ncm->is_single_interface = true;

        if VERIFY_NCM_BYPASS(ncm)
        {
            ncm_ecm_desc.bmEthernetStatistics = ECM_DESC_ETH_NET_STAT;
            ncm_ecm_desc.wNumberMCFilters = ECM_DESC_NUM_MC_FILTERS;
            ncm_ecm_desc.bNumberPowerFilters = ECM_DESC_NUM_PWR_FILTERS;
        }
    }

    if (!ncm->is_single_interface)
    {
        status = usb_interface_id(c, f);
        if (status < 0)
        { goto fail; }
    }

    ncm->data_id = status;
    bsp_usb_add_setup_dev((USB_UINT32)ncm->data_id);

    ncm_data_nop_intf.bInterfaceNumber = status;
    ncm_data_intf.bInterfaceNumber = status;
    ncm_union_desc.bSlaveInterface0 = status;

    status = -ENODEV;

    /* allocate instance-specific endpoints */
    ep = usb_ep_autoconfig(cdev->gadget, &fs_ncm_in_desc);
    if (!ep)
    { goto fail; }
    ncm->port.in_ep = ep;
    ep->driver_data = cdev;    /* claim */

    ep = usb_ep_autoconfig(cdev->gadget, &fs_ncm_out_desc);
    if (!ep)
    { goto fail; }
    ncm->port.out_ep = ep;
    ep->driver_data = cdev;    /* claim */

    ep = usb_ep_autoconfig(cdev->gadget, &fs_ncm_notify_desc);
    if (!ep)
    { goto fail; }
    ncm->notify = ep;
    ep->driver_data = cdev;    /* claim */

    status = -ENOMEM;

    /* allocate notification request and buffer */
    ncm->notify_req = usb_ep_alloc_request(ep, GFP_KERNEL);
    if (!ncm->notify_req)
    { goto fail; }
    ncm->notify_req->buf = kmalloc(NCM_STATUS_BYTECOUNT, GFP_KERNEL);
    if (!ncm->notify_req->buf)
    { goto fail; }
    ncm->notify_req->context = ncm;
    ncm->notify_req->complete = ncm_notify_complete;
    ncm->notify_req_rec = ncm->notify_req;

    /* copy descriptors, and track endpoint copies */
    if (NCM_WORK_MODE_MBIM == ncm->work_mode)
    {
        desc = ncm_mbim_fs_function;
    }
    else if (ncm->is_single_interface)
    {
        desc = ncm_fs_function_single;
    }
    else
    {
        desc = ncm_fs_function;
    }

#ifdef NCM_ENCAP_CMD_CID_ENABLED
    if (OS_TYPE_WINDOWS == pnp_info->rewind_param.bPcType)
    {
        desc = ncm_cid_fs_function;
    }
#endif

    f->descriptors = usb_copy_descriptors(desc);
    if (!f->descriptors)
    { goto fail; }

    /*
     * support all relevant hardware speeds... we expect that when
     * hardware is dual speed, all bulk-capable endpoints work at
     * both speeds
     */
    if (gadget_is_dualspeed(c->cdev->gadget))
    {
        hs_ncm_in_desc.bEndpointAddress =
            fs_ncm_in_desc.bEndpointAddress;
        hs_ncm_out_desc.bEndpointAddress =
            fs_ncm_out_desc.bEndpointAddress;
        hs_ncm_notify_desc.bEndpointAddress =
            fs_ncm_notify_desc.bEndpointAddress;

        /* copy descriptors, and track endpoint copies */
        if (NCM_WORK_MODE_MBIM == ncm->work_mode)
        {
            desc = ncm_mbim_hs_function;
        }
        else if (ncm->is_single_interface)
        {
            desc = ncm_hs_function_single;
        }
        else
        {
            desc = ncm_hs_function;
        }

#ifdef NCM_ENCAP_CMD_CID_ENABLED
        if (OS_TYPE_WINDOWS == pnp_info->rewind_param.bPcType)
        {
            desc = ncm_cid_hs_function;
        }
#endif

        f->hs_descriptors = usb_copy_descriptors(desc);
        if (!f->hs_descriptors)
        { goto fail; }
    }

    if (gadget_is_superspeed(c->cdev->gadget))
    {
        ss_ncm_in_desc.bEndpointAddress =
            fs_ncm_in_desc.bEndpointAddress;
        ss_ncm_out_desc.bEndpointAddress =
            fs_ncm_out_desc.bEndpointAddress;
        ss_ncm_notify_desc.bEndpointAddress =
            fs_ncm_notify_desc.bEndpointAddress;

        /* copy descriptors, and track endpoint copies */
        if (NCM_WORK_MODE_MBIM == ncm->work_mode)
        {
            desc = ncm_mbim_ss_function;
        }
        else if (ncm->is_single_interface)
        {
            desc = ncm_ss_function_single;
        }
        else
        {
            desc = ncm_ss_function;
        }

#ifdef NCM_ENCAP_CMD_CID_ENABLED
        if (OS_TYPE_WINDOWS == pnp_info->rewind_param.bPcType)
        {
            desc = ncm_cid_ss_function;
        }
#endif

        f->ss_descriptors = usb_copy_descriptors(desc);
        if (!f->ss_descriptors)
        { goto fail; }
    }

    /*
     * NOTE:  all that is done without knowing or caring about
     * the network link ... which is unavailable to this code
     * until we're activated via set_alt().
     */
    ncm->port.open = ncm_open;
    ncm->port.close = ncm_close;

    DBG_I(MBB_NCM, "CDC Network: %s speed IN/%s OUT/%s NOTIFY/%s\n",
          gadget_is_superspeed(c->cdev->gadget) ? "super" :
          gadget_is_dualspeed(c->cdev->gadget) ? "dual" : "full",
          ncm->port.in_ep->name, ncm->port.out_ep->name,
          ncm->notify->name);

    if (VERIFY_NCM_BYPASS(ncm))
    {
        (USB_VOID)ncm_params_register(ncm_response_available, (USB_VOID* )ncm);

        status = ncm_vendor_tx_thread_init(ncm);
        if (status)
        {
            DBG_E(MBB_NCM, "ncm vendor tx thread init failed.\n");
            goto fail;
        }
    }

    return 0;

fail:

    if (VERIFY_NCM_BYPASS(ncm))
    {
        ncm_params_deregister(ncm);
        ncm_vendor_tx_thread_uninit(ncm);
    }

    if (f->hs_descriptors)
    { usb_free_descriptors(f->hs_descriptors); }
    if (f->ss_descriptors)
    { usb_free_descriptors(f->ss_descriptors); }
    if (f->descriptors)
    { usb_free_descriptors(f->descriptors); }

    if (ncm->notify_req)
    {
        kfree(ncm->notify_req->buf);
        usb_ep_free_request(ncm->notify, ncm->notify_req);
        ncm->notify_req_rec = NULL;
    }

    /* we might as well release our claims on endpoints */
    if (ncm->notify)
    { ncm->notify->driver_data = NULL; }
    if (ncm->port.out_ep)
    { ncm->port.out_ep->driver_data = NULL; }
    if (ncm->port.in_ep)
    { ncm->port.in_ep->driver_data = NULL; }

    DBG_E(MBB_NCM, "%s: can't bind, err %d\n", f->name, status);

    return status;
}

static USB_VOID
ncm_unbind(struct usb_configuration* c, struct usb_function* f)
{
    struct f_ncm*        ncm = func_to_ncm(f);

    GNET_BUG(("ncm_unbind:null the_ncm!\n"), (NULL == the_ncm[ncm->port.port_no]));

    DBG_T(MBB_NCM, "ncm unbind\n");

    if (VERIFY_NCM_BYPASS(ncm))
    {
        ncm_params_deregister(ncm);
        (USB_VOID)ncm_vendor_tx_thread_uninit(ncm);

        ncm_unbind_func(ncm->vendor_ctx.vendor_priv);
    }

    if (gadget_is_dualspeed(c->cdev->gadget))
    { usb_free_descriptors(f->hs_descriptors); }
    if (f->ss_descriptors)
    { usb_free_descriptors(f->ss_descriptors); }
    if (f->descriptors)
    { usb_free_descriptors(f->descriptors); }

    kfree(ncm->notify_req->buf);
    usb_ep_free_request(ncm->notify, ncm->notify_req);
    ncm->notify_req_rec = NULL;

    if (VERIFY_NCM_BYPASS(ncm))
    {
        ncm_vendor_uninit(&ncm->vendor_ctx);
    }

    the_ncm[ncm->port.port_no] = NULL;

    glink_idx--;
    kfree(ncm);
}

/**
 * ncm_bind_config - add CDC Network link to a configuration
 * @c: the configuration to support the network link
 * @ethaddr: a buffer in which the ethernet address of the host side
 *    side of the link was recorded
 * Context: single threaded during gadget setup
 *
 * Returns zero on success, else negative errno.
 *
 * Caller must have called @gether_setup().  Caller is also responsible
 * for calling @gether_cleanup() before module unload.
 */
//int __init ncm_bind_config(struct usb_configuration *c, USB_UINT8 ethaddr[ETH_ALEN])
USB_INT ncm_bind_config(struct usb_configuration* c,
                        struct android_usb_function* f, USB_UINT8 ethaddr[ETH_ALEN])
{
    struct f_ncm*    ncm;
    USB_INT       status;
#ifdef CONFIG_NCM_MBIM_SUPPORT
    mbb_usb_nv_info_st* nv_st = usb_nv_get_ctx();
#endif

    GNET_BUG(("ncm_bind_config:glink_idx(%ld) overflow!\n", glink_idx), (glink_idx >= GNET_USED_NUM));

    DBG_T(MBB_NCM, "ncm_bind_config\n");

    if (the_ncm[glink_idx])
    { return -EBUSY; }

    if (!can_support_ecm(c->cdev->gadget) || !ethaddr)
    { return -EINVAL; }

    /* maybe allocate device-global string IDs */
    if (ncm_string_defs[0].id == 0)
    {

        /* control interface label */
        status = usb_string_id(c->cdev);
        if (status < 0)
        { return status; }
        ncm_string_defs[STRING_CTRL_IDX].id = status;
        ncm_control_intf.iInterface = status;

        /* data interface label */
        status = usb_string_id(c->cdev);
        if (status < 0)
        { return status; }
        ncm_string_defs[STRING_DATA_IDX].id = status;
        ncm_data_nop_intf.iInterface = status;
        ncm_data_intf.iInterface = status;

        /* MAC address */
        status = usb_string_id(c->cdev);
        if (status < 0)
        { return status; }
        ncm_string_defs[STRING_MAC_IDX].id = status;
        ncm_ecm_desc.iMACAddress = status;

        /* IAD */
        status = usb_string_id(c->cdev);
        if (status < 0)
        { return status; }
        ncm_string_defs[STRING_IAD_IDX].id = status;
        ncm_iad_desc.iFunction = status;
    }

#ifdef CONFIG_NCM_MBIM_SUPPORT
    if ((NULL != nv_st) && (0 != nv_st->winblue_profile.MBIMEnable))
    {
        memcpy(gMbimInterfaceName, nv_st->winblue_profile.InterfaceName, MBIM_INTERFACE_NAME_LEN_MAX);
        ncm_string_defs[STRING_IAD_IDX].s = gMbimInterfaceName;
        ncm_ex_mbim_desc.wMTPU = nv_st->winblue_profile.IPV4MTU;
        mbim_core_winblue_set_profile(nv_st->winblue_profile.IPV4MTU,
                                      nv_st->winblue_profile.IPV6MTU, nv_st->winblue_profile.MaxPDPSession);
    }
#endif

    /* allocate and initialize one new instance */
    ncm = kzalloc(sizeof * ncm, GFP_KERNEL);
    if (!ncm)
    { return -ENOMEM; }

    /* export host's Ethernet address in CDC format */
    snprintf(ncm->ethaddr, sizeof ncm->ethaddr,
             "%02X%02X%02X%02X%02X%02X",
             ethaddr[0], ethaddr[1], ethaddr[2],
             ethaddr[3], ethaddr[4], ethaddr[5]);

    spin_lock_init(&ncm->lock);
    ncm_reset_values(ncm);
    ncm->port.is_fixed = true;

    ncm->port.func.name = "ncm_network";
    ncm->port.func.strings = ncm_strings;
    /* descriptors are per-instance copies */
    ncm->port.func.bind = ncm_bind;
    ncm->port.func.unbind = ncm_unbind;
    ncm->port.func.set_alt = ncm_set_alt;
    ncm->port.func.get_alt = ncm_get_alt;
    ncm->port.func.setup = ncm_setup;
    ncm->port.func.disable = ncm_disable;

    ncm->port.func.suspend = ncm_suspend;
    ncm->port.func.resume = ncm_resume;

    ncm->port.wrap = ncm_wrap_ntb;
    ncm->port.unwrap = ncm_unwrap_ntb;

    if (NULL != f)
    {
        ncm->protocal = f->property;
    }

    if (!ncm_get_bypass_state())
    {
        ncm->work_mode = NCM_WORK_MODE_NET_BRIDGE;
    }
    else
    {
        /* 实际形态要在ncm_bind中确定 */
        ncm->work_mode = NCM_WORK_MODE_NORMAL;
    }

    if (VERIFY_NCM_BYPASS(ncm))
    {
        ncm->port.handle_rx = ncm_vendor_rx;

        ncm_params_init(ncm);

        ncm->vendor_ctx.ncm_priv = ncm;
        status = ncm_vendor_init(&ncm->vendor_ctx);
        if (status)
        {
            kfree(ncm);
            return status;
        }
    }
    else
    {
        ncm->port.handle_rx = NULL;
    }

    status = usb_add_function(c, &ncm->port.func);
    if (status)
    {
        if (VERIFY_NCM_BYPASS(ncm))
        {
            ncm_vendor_uninit(&ncm->vendor_ctx);
        }

        kfree(ncm);
        return status;
    }

    ncm->port.port_no = glink_idx;
    the_ncm[glink_idx] = ncm;
    glink_idx++;
    return status;
}

void f_ncm_dump(void)
{
    int idx;
    struct f_ncm* ncm;

    for (idx = 0; idx < GNET_MAX_NUM; idx++)
    {
        ncm = the_ncm[idx];
        if (ncm)
        {
            DBG_T(MBB_NCM, "notify_state:       %u\n", ncm->notify_state);
            DBG_T(MBB_NCM, "host_timeout:       %ld\n", ncm->host_timeout);
            DBG_T(MBB_NCM, "stat_host_timeout:  %ld\n", ncm->stat_host_timeout);
        }
    }
}

USB_VOID ncm_unbind_config(struct usb_configuration* c)
{
    USB_INT32 loop;
    struct f_ncm*    ncm = NULL;

    DBG_T(MBB_NCM, "ncm ncm_unbind_config\n");

    for (loop = 0; loop < n_gnet_ports; loop++)
    {
        if (!the_ncm[loop])
        {
            continue;
        }

        ncm = the_ncm[loop];
        ncm_unbind(c, &ncm->port.func);

        glink_idx--;
        the_ncm[loop] = NULL;
    }

    GNET_BUG(("gnet_unbind_config:glink_idx (%ld) not zero!\n", glink_idx), 0 != glink_idx);
    glink_idx = 0;

    return;
}

/*ncm*/
/* Code Checker 将单目运算符"-（负号）"
作为双目运算符"-（减号）"处理，此为Code Checker问题 */
static ssize_t ncm_manufacturer_show(struct device* dev, struct device_attribute* attr, USB_CHAR* buf)
{
    return -1;
}

static ssize_t ncm_manufacturer_store(struct device* dev,
                                      struct device_attribute* attr, const USB_CHAR* buf, size_t size)
{
    return -1;
}

static DEVICE_ATTR(ncm_manufacturer, S_IRUGO | S_IWUSR, ncm_manufacturer_show,
                   ncm_manufacturer_store);


static ssize_t ncm_ethaddr_show(struct device* dev,
                                struct device_attribute* attr, USB_CHAR* buf)
{
    return -EINVAL;
}

static ssize_t ncm_ethaddr_store(struct device* dev,
                                 struct device_attribute* attr, const USB_CHAR* buf, size_t size)
{
    return -EINVAL;
}

static DEVICE_ATTR(ncm_ethaddr, S_IRUGO | S_IWUSR, ncm_ethaddr_show, ncm_ethaddr_store);

static ssize_t ncm_vendorID_show(struct device* dev,
                                 struct device_attribute* attr, USB_CHAR* buf)
{
    return -EINVAL;
}

static ssize_t ncm_vendorID_store(struct device* dev,
                                  struct device_attribute* attr, const USB_CHAR* buf, size_t size)
{
    return -EINVAL;
}

static DEVICE_ATTR(ncm_vendorID, S_IRUGO | S_IWUSR, ncm_vendorID_show,
                   ncm_vendorID_store);

static struct device_attribute* ncm_function_attributes[] =
{
    &dev_attr_ncm_manufacturer,
    &dev_attr_ncm_ethaddr,
    &dev_attr_ncm_vendorID,
    NULL
};

struct ncm_function_config
{
    USB_UINT8      ethaddr[ETH_ALEN];
    USB_UINT32     vendorID;
    USB_CHAR       manufacturer[MANUFACTURER_BUFFER_SIZE];
};

static USB_INT ncm_function_init(struct android_usb_function* f, struct usb_composite_dev* cdev)
{
    f->config = kzalloc(sizeof(struct ncm_function_config), GFP_KERNEL);
    if (!f->config)
    { return -ENOMEM; }
    return 0;
}

static USB_VOID ncm_function_cleanup(struct android_usb_function* f)
{
    kfree(f->config);
    f->config = NULL;
}

static USB_INT ncm_function_bind_config(
    struct android_usb_function* f, struct usb_configuration* c)
{
    USB_INT ret = 0;
    USB_UINT loop = 0;
    USB_UINT port_num = 1;

#ifdef USB_DUAL_CORE_CPE
    port_num = n_gnet_ports;
#endif

    /* set up network link layer */
    for (loop = 0; loop < port_num; loop++)
    {
        DBG_I(MBB_NCM, "%s: gether_setup %d\n", __func__, loop);
        ret = gether_setup(c->cdev->gadget, hostaddr[loop]);
        if (ret < 0)
        {
            DBG_E(MBB_NCM, "%s: gether_setup failed loop:%d\n", __func__, loop);
            goto fail_clean_eth;
        }
    }

    for (loop = 0; loop < port_num; loop++)
    {
        DBG_I(MBB_NCM, "%s: bind %d\n", __func__, loop);
        ret = ncm_bind_config(c, f, hostaddr[loop]);
        if (ret < 0)
        {
            DBG_E(MBB_NCM, "ncm_bind_config %d fail!\n", loop);
            goto clean_gnet_config;
        }
    }

    return 0;

clean_gnet_config:
    ncm_unbind_config(c);
fail_clean_eth:
    if (n_gnet_ports > 0)
    {
        gether_cleanup();
    }

    return ret;
}

static USB_VOID ncm_function_unbind_config(
    struct android_usb_function* f, struct usb_configuration* c)
{
    /* clear the string id index */
    if (ncm_string_defs[STRING_CTRL_IDX].id)
    {
        ncm_string_defs[STRING_CTRL_IDX].id = 0;
        ncm_string_defs[STRING_DATA_IDX].id = 0;
        ncm_string_defs[STRING_MAC_IDX].id = 0;
        ncm_string_defs[STRING_IAD_IDX].id = 0;
    }

    gether_cleanup();
}

static struct android_usb_function ncm_function =
{
    .name          = "ncm",
    .init          = ncm_function_init,
    .cleanup       = ncm_function_cleanup,
    .bind_config   = ncm_function_bind_config,
    .unbind_config = ncm_function_unbind_config,
    .attributes    = ncm_function_attributes,
};

USB_VOID usb_ncm_init(USB_VOID)
{
    USB_INT cdc_type = GNET_CDC_TYPE_NCM;

    DBG_I(MBB_NCM, "usb_ncm_init enter\n");
    pnp_register_usb_support_function(&ncm_function);

#if (defined(USB_E5) || defined(USB_WINGLE)) && (defined(USB_RNDIS))
    cdc_type = GNET_CDC_TYPE_RNDIS;
#endif

    /* NCM 的RX内存需要提前分配，避免后续分配连续大块内存失败 */
    if (eth_prealloc_rx_mem(cdc_type))
    {
        DBG_E(MBB_NCM, "usb_ncm_init init ncm memory failed:cdc_type%d\n", cdc_type);
    }
}

