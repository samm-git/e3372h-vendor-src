/*
 * f_mass_storage.c -- Mass Storage USB Composite Function
 *
 * Copyright (C) 2003-2008 Alan Stern
 * Copyright (C) 2009 Samsung Electronics
 *                    Author: Michal Nazarewicz <mina86@mina86.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions, and the following disclaimer,
 *    without modification.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The names of the above-listed copyright holders may not be used
 *    to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * ALTERNATIVELY, this software may be distributed under the terms of the
 * GNU General Public License ("GPL") as published by the Free Software
 * Foundation, either version 2 of that License or (at your option) any
 * later version.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * The Mass Storage Function acts as a USB Mass Storage device,
 * appearing to the host as a disk drive or as a CD-ROM drive.  In
 * addition to providing an example of a genuinely useful composite
 * function for a USB device, it also illustrates a technique of
 * double-buffering for increased throughput.
 *
 * Function supports multiple logical units (LUNs).  Backing storage
 * for each LUN is provided by a regular file or a block device.
 * Access for each LUN can be limited to read-only.  Moreover, the
 * function can indicate that LUN is removable and/or CD-ROM.  (The
 * later implies read-only access.)
 *
 * MSF is configured by specifying a fsg_config structure.  It has the
 * following fields:
 *
 *	nluns		Number of LUNs function have (anywhere from 1
 *				to FSG_MAX_LUNS which is 8).
 *	luns		An array of LUN configuration values.  This
 *				should be filled for each LUN that
 *				function will include (ie. for "nluns"
 *				LUNs).  Each element of the array has
 *				the following fields:
 *	->filename	The path to the backing file for the LUN.
 *				Required if LUN is not marked as
 *				removable.
 *	->ro		Flag specifying access to the LUN shall be
 *				read-only.  This is implied if CD-ROM
 *				emulation is enabled as well as when
 *				it was impossible to open "filename"
 *				in R/W mode.
 *	->removable	Flag specifying that LUN shall be indicated as
 *				being removable.
 *	->cdrom		Flag specifying that LUN shall be reported as
 *				being a CD-ROM.
 *	->nofua		Flag specifying that FUA flag in SCSI WRITE(10,12)
 *				commands for this LUN shall be ignored.
 *
 *	lun_name_format	A printf-like format for names of the LUN
 *				devices.  This determines how the
 *				directory in sysfs will be named.
 *				Unless you are using several MSFs in
 *				a single gadget (as opposed to single
 *				MSF in many configurations) you may
 *				leave it as NULL (in which case
 *				"lun%d" will be used).  In the format
 *				you can use "%d" to index LUNs for
 *				MSF's with more than one LUN.  (Beware
 *				that there is only one integer given
 *				as an argument for the format and
 *				specifying invalid format may cause
 *				unspecified behaviour.)
 *	thread_name	Name of the kernel thread process used by the
 *				MSF.  You can safely set it to NULL
 *				(in which case default "file-storage"
 *				will be used).
 *
 *	vendor_name
 *	product_name
 *	release		Information used as a reply to INQUIRY
 *				request.  To use default set to NULL,
 *				NULL, 0xffff respectively.  The first
 *				field should be 8 and the second 16
 *				characters or less.
 *
 *	can_stall	Set to permit function to halt bulk endpoints.
 *				Disabled on some USB devices known not
 *				to work correctly.  You should set it
 *				to true.
 *
 * If "removable" is not set for a LUN then a backing file must be
 * specified.  If it is set, then NULL filename means the LUN's medium
 * is not loaded (an empty string as "filename" in the fsg_config
 * structure causes error).  The CD-ROM emulation includes a single
 * data track and no audio tracks; hence there need be only one
 * backing file per LUN.
 *
 *
 * MSF includes support for module parameters.  If gadget using it
 * decides to use it, the following module parameters will be
 * available:
 *
 *	file=filename[,filename...]
 *			Names of the files or block devices used for
 *				backing storage.
 *	ro=b[,b...]	Default false, boolean for read-only access.
 *	removable=b[,b...]
 *			Default true, boolean for removable media.
 *	cdrom=b[,b...]	Default false, boolean for whether to emulate
 *				a CD-ROM drive.
 *	nofua=b[,b...]	Default false, booleans for ignore FUA flag
 *				in SCSI WRITE(10,12) commands
 *	luns=N		Default N = number of filenames, number of
 *				LUNs to support.
 *	stall		Default determined according to the type of
 *				USB device controller (usually true),
 *				boolean to permit the driver to halt
 *				bulk endpoints.
 *
 * The module parameters may be prefixed with some string.  You need
 * to consult gadget's documentation or source to verify whether it is
 * using those module parameters and if it does what are the prefixes
 * (look for FSG_MODULE_PARAMETERS() macro usage, what's inside it is
 * the prefix).
 *
 *
 * Requirements are modest; only a bulk-in and a bulk-out endpoint are
 * needed.  The memory requirement amounts to two 16K buffers, size
 * configurable by a parameter.  Support is included for both
 * full-speed and high-speed operation.
 *
 * Note that the driver is slightly non-portable in that it assumes a
 * single memory/DMA buffer will be useable for bulk-in, bulk-out, and
 * interrupt-in endpoints.  With most device controllers this isn't an
 * issue, but there may be some with hardware restrictions that prevent
 * a buffer from being used by more than one endpoint.
 *
 *
 * The pathnames of the backing files and the ro settings are
 * available in the attribute files "file" and "ro" in the lun<n> (or
 * to be more precise in a directory which name comes from
 * "lun_name_format" option!) subdirectory of the gadget's sysfs
 * directory.  If the "removable" option is set, writing to these
 * files will simulate ejecting/loading the medium (writing an empty
 * line means eject) and adjusting a write-enable tab.  Changes to the
 * ro setting are not allowed when the medium is loaded or if CD-ROM
 * emulation is being used.
 *
 * When a LUN receive an "eject" SCSI request (Start/Stop Unit),
 * if the LUN is removable, the backing file is released to simulate
 * ejection.
 *
 *
 * This function is heavily based on "File-backed Storage Gadget" by
 * Alan Stern which in turn is heavily based on "Gadget Zero" by David
 * Brownell.  The driver's SCSI command interface was based on the
 * "Information technology - Small Computer System Interface - 2"
 * document from X3T9.2 Project 375D, Revision 10L, 7-SEP-93,
 * available at <http://www.t10.org/ftp/t10/drafts/s2/s2-r10l.pdf>.
 * The single exception is opcode 0x23 (READ FORMAT CAPACITIES), which
 * was based on the "Universal Serial Bus Mass Storage Class UFI
 * Command Specification" document, Revision 1.0, December 14, 1998,
 * available at
 * <http://www.usb.org/developers/devclass_docs/usbmass-ufi10.pdf>.
 */

/*
 *				Driver Design
 *
 * The MSF is fairly straightforward.  There is a main kernel
 * thread that handles most of the work.  Interrupt routines field
 * callbacks from the controller driver: bulk- and interrupt-request
 * completion notifications, endpoint-0 events, and disconnect events.
 * Completion events are passed to the main thread by wakeup calls.  Many
 * ep0 requests are handled at interrupt time, but SetInterface,
 * SetConfiguration, and device reset requests are forwarded to the
 * thread in the form of "exceptions" using SIGUSR1 signals (since they
 * should interrupt any ongoing file I/O operations).
 *
 * The thread's main routine implements the standard command/data/status
 * parts of a SCSI interaction.  It and its subroutines are full of tests
 * for pending signals/exceptions -- all this polling is necessary since
 * the kernel has no setjmp/longjmp equivalents.  (Maybe this is an
 * indication that the driver really wants to be running in userspace.)
 * An important point is that so long as the thread is alive it keeps an
 * open reference to the backing file.  This will prevent unmounting
 * the backing file's underlying filesystem and could cause problems
 * during system shutdown, for example.  To prevent such problems, the
 * thread catches INT, TERM, and KILL signals and converts them into
 * an EXIT exception.
 *
 * In normal operation the main thread is started during the gadget's
 * fsg_bind() callback and stopped during fsg_unbind().  But it can
 * also exit when it receives a signal, and there's no point leaving
 * the gadget running when the thread is dead.  At of this moment, MSF
 * provides no way to deregister the gadget when thread dies -- maybe
 * a callback functions is needed.
 *
 * To provide maximum throughput, the driver uses a circular pipeline of
 * buffer heads (struct fsg_buffhd).  In principle the pipeline can be
 * arbitrarily long; in practice the benefits don't justify having more
 * than 2 stages (i.e., double buffering).  But it helps to think of the
 * pipeline as being a long one.  Each buffer head contains a bulk-in and
 * a bulk-out request pointer (since the buffer can be used for both
 * output and input -- directions always are given from the host's
 * point of view) as well as a pointer to the buffer and various state
 * variables.
 *
 * Use of the pipeline follows a simple protocol.  There is a variable
 * (fsg->next_buffhd_to_fill) that points to the next buffer head to use.
 * At any time that buffer head may still be in use from an earlier
 * request, so each buffer head has a state variable indicating whether
 * it is EMPTY, FULL, or BUSY.  Typical use involves waiting for the
 * buffer head to be EMPTY, filling the buffer either by file I/O or by
 * USB I/O (during which the buffer head is BUSY), and marking the buffer
 * head FULL when the I/O is complete.  Then the buffer will be emptied
 * (again possibly by USB I/O, during which it is marked BUSY) and
 * finally marked EMPTY again (possibly by a completion routine).
 *
 * A module parameter tells the driver to avoid stalling the bulk
 * endpoints wherever the transport specification allows.  This is
 * necessary for some UDCs like the SuperH, which cannot reliably clear a
 * halt on a bulk endpoint.  However, under certain circumstances the
 * Bulk-only specification requires a stall.  In such cases the driver
 * will halt the endpoint and set a flag indicating that it should clear
 * the halt in software during the next device reset.  Hopefully this
 * will permit everything to work correctly.  Furthermore, although the
 * specification allows the bulk-out endpoint to halt when the host sends
 * too much data, implementing this would cause an unavoidable race.
 * The driver will always use the "no-stall" approach for OUT transfers.
 *
 * One subtle point concerns sending status-stage responses for ep0
 * requests.  Some of these requests, such as device reset, can involve
 * interrupting an ongoing file I/O operation, which might take an
 * arbitrarily long time.  During that delay the host might give up on
 * the original ep0 request and issue a new one.  When that happens the
 * driver should not notify the host about completion of the original
 * request, as the host will no longer be waiting for it.  So the driver
 * assigns to each ep0 request a unique tag, and it keeps track of the
 * tag value of the request associated with a long-running exception
 * (device-reset, interface-change, or configuration-change).  When the
 * exception handler is finished, the status-stage response is submitted
 * only if the current ep0 request tag is equal to the exception request
 * tag.  Thus only the most recently received ep0 request will get a
 * status-stage response.
 *
 * Warning: This driver source file is too long.  It ought to be split up
 * into a header file plus about 3 separate .c files, to handle the details
 * of the Gadget, USB Mass Storage, and SCSI protocols.
 */
/******************************************************************************************************
    问题单号          时间            修改人                      说明
*****************************************************************************************************/

/* #define VERBOSE_DEBUG */
/* #define DUMP_MSGS */

#include <linux/module.h>
#include <linux/blkdev.h>
#include <linux/completion.h>
#include <linux/dcache.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/fcntl.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/kref.h>
#include <linux/kthread.h>
#include <linux/limits.h>
#include <linux/rwsem.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/string.h>
#include <linux/freezer.h>
#include <linux/utsname.h>
#include "f_mass_storage_api.h"

#ifdef MBB_USB_UNITARY_Q
#else
#include <linux/usb/ch9.h>
#include <linux/usb/gadget.h>
#include <linux/usb/composite.h>
#include <mbb_config.h>
#endif
#include "hw_pnp_api.h"
#include "gadget_chips.h"
#include "usb_nv_get.h"
#include "hw_pnp.h"
#include "android.h"
#include "mbb_fast_on_off.h"

#define FSG_DRIVER_DESC		"Mass Storage Function"
#define FSG_DRIVER_VERSION	"GADGET 2.0"

static const USB_CHAR fsg_string_interface[] = "Mass Storage";

#include "storage_common.c"

#define MAX_HDLEN                   20
#define MAX_INQUIRY_STRING          29 //8+16+4+1

#define MASS_TIMER_FIVE_SECOND      5000
#define MASS_TIMER_TEN_SECOND       10000

/*支持的mass storage enabled_function list的数量*/
#define SUPPORT_MASS_FUNCTION_NUM   2
/*mass enabled_function list 是mass storage 模块用来管理内部信息的表。
 *由此表为头可以得到所有关于enabled 的mass storage function的信息及数据
 *此表记录的是处于enabled的function 的指针
 */
struct mass_storage_status_info
{
    mass_lun_info mass_luns[FSG_MAX_LUNS];
    USB_CHAR* mass_func_name;
};

static struct android_usb_function* g_enabled_mass_function_list[SUPPORT_MASS_FUNCTION_NUM] = {NULL};
static struct mass_storage_status_info g_support_mass_functions_lun_info[SUPPORT_MASS_FUNCTION_NUM] =
{
    {{LUN_NONE}, "mass"},
    {{LUN_NONE}, "mass_two"}
};

#define SC_READ_DISC_INFORMATION        0x51

sd_card_info g_sd_card_info =
{
    .sd_card_status     = SD_CARD_NOT_PRESENT,
    .sd_card_workmode   = SD_WORKMODE_MASS,
};

USB_INT g_scsi_stat = 0;  // 0表示定时器到期后不做处理
static struct timer_list g_soft_mass_timer ;

/* 从闪电卡8200平台移植，Code Checker不修改 */

//解决linux上光盘不能自动映射到桌面
/* 根据SCSI协议规范， 0x46 get configuration命令回复内容
前四个字节数据长度字段，
   该数据长度指的是除去数据长度字段剩下的内容长度 */
/* The Data Length field indicates the amount of data available given a
   sufficient allocation length following this field.*/
/* Code Checker 不修改说明，该数组内容是SCSI交互需要返回的数据内容，
   使用宏替换回复数据没有实际意义，Code Checker 出现的警告不修改*/
static USB_UINT8  pnp_get_configration_respond[] =
{
    0x00, 0x00, 0x00, 0x44, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x03, 0x04, 0x00, 0x08, 0x01, 0x00,
    0x00, 0x01, 0x07, 0x08, 0x00, 0x00, 0x00, 0x08, 0x01, 0x00, 0x00, 0x00, 0x00, 0x02, 0x07, 0x04,
    0x02, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x04, 0x40, 0x00, 0x00, 0x00, 0x00, 0x10, 0x01, 0x08,
    0x00, 0x00, 0x08, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x1E, 0x09, 0x04, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x03, 0x00, 0x01, 0x05, 0x07, 0x04
};

static USB_INT do_rewind(struct fsg_common* common, struct fsg_buffhd* bh)
{
    USB_INT is_rewind_before = pnp_is_rewind_before_mode();
    USB_INT power_off   = MBB_USB_NO;
    USB_INT fast_off   = MBB_USB_NO;
    power_off   = usb_power_off_chg_stat();
    fast_off    = usb_fast_on_off_stat();

    if ( !is_rewind_before || MBB_USB_YES == power_off || MBB_USB_YES == fast_off )
    {
        DBG_W(MBB_MASS, "can't rewind\n");
        return -1;
    }

    DBG_T(MBB_MASS, "do_rewind command");

    print_hex_dump(KERN_ERR, "", DUMP_PREFIX_OFFSET, MAX_COMMAND_SIZE, 1,
                   common->cmnd, MAX_COMMAND_SIZE, 0);

    pnp_set_rewind_param(&(common->cmnd[1]));

    pnp_switch_rewind_after_mode();

    return 0;
}

static inline USB_INT __fsg_is_set(struct fsg_common* common,
                                   USB_CHAR const* func, USB_UINT line)
{
    struct mass_storage_function_config_data* configdata = common->private_data;
    struct fsg_state_count* fsg_state_for_common = &configdata->fsg_state;

    if (common->fsg)
    { return 1; }

    DBG_E(MBB_MASS, "common->fsg is NULL in %s at %u\n", func, line);
    fsg_state_for_common->fsg_free++;
    return 0;
}

#define fsg_is_set(common) likely(__fsg_is_set(common, __func__, __LINE__))

static inline struct fsg_dev* fsg_from_func(struct usb_function* f)
{
    return container_of(f, struct fsg_dev, function);
}

typedef void (*fsg_routine_t)(struct fsg_dev*);

static USB_INT exception_in_progress(struct fsg_common* common)
{
    return (common->state) > FSG_STATE_IDLE;
}

/* Make bulk-out requests be divisible by the maxpacket size */
static USB_VOID set_bulk_out_req_length(struct fsg_common* common,
                                        struct fsg_buffhd* bh, USB_UINT length)
{
    USB_UINT	rem;

    bh->bulk_out_intended_length = length;
    rem = length % common->bulk_out_maxpacket;

    if (rem > 0)
    { length += common->bulk_out_maxpacket - rem; }

    bh->outreq->length = length;
}

/*-------------------------------------------------------------------------*/

static USB_INT fsg_set_halt(struct fsg_dev* fsg, struct usb_ep* ep)
{
    USB_CHAR* name;
    struct mass_storage_function_config_data* configdata = fsg->common->private_data;
    struct fsg_state_count* fsg_state_for_common = &configdata->fsg_state;

    if (ep == fsg->bulk_in)
    {
        name = "bulk-in";
        fsg_state_for_common->bulk_in_set_halt++;
    }
    else if (ep == fsg->bulk_out)
    {
        name = "bulk-out";
        fsg_state_for_common->bulk_out_set_halt++;
    }
    else
    { name = (USB_CHAR*)ep->name; }

#ifdef USB_DEBUG
#else
    DBG_I(MBB_MASS, "%s set halt\n", name);
#endif
    return usb_ep_set_halt(ep);
}


/*-------------------------------------------------------------------------*/

/* These routines may be called in process context or in_irq */

/* Caller must hold fsg->lock */
static USB_VOID wakeup_thread(struct fsg_common* common)
{
    /* Tell the main thread that something has happened */
    common->thread_wakeup_needed = 1;

    if (common->thread_task)
    { wake_up_process(common->thread_task); }
}

static USB_VOID raise_exception(struct fsg_common* common, enum fsg_state new_state)
{
    USB_UINT32		flags;

    /*
     * Do nothing if a higher-priority exception is already in progress.
     * If a lower-or-equal priority exception is in progress, preempt it
     * and notify the main thread by sending it a signal.
     */
    spin_lock_irqsave(&common->lock, flags);

    if (common->state <= new_state)
    {
        common->exception_req_tag = common->ep0_req_tag;
        common->state = new_state;

        if (common->thread_task)
            send_sig_info(SIGUSR1, SEND_SIG_FORCED,
                          common->thread_task);
    }

    spin_unlock_irqrestore(&common->lock, flags);
}

/*-------------------------------------------------------------------------*/

static USB_INT ep0_queue(struct fsg_common* common)
{
    USB_INT	rc;

    rc = usb_ep_queue(common->ep0, common->ep0req, GFP_ATOMIC);
    common->ep0->driver_data = common;

    if (rc != 0 && rc != -ESHUTDOWN)
    {
        /* We can't do much more than wait for a reset */
        DBG_W(MBB_MASS, "error in submission: %s --> %d\n",
              common->ep0->name, rc);
    }

    return rc;
}

/*-------------------------------------------------------------------------*/

/* Completion handlers. These always run in_irq. */

static USB_VOID bulk_in_complete(struct usb_ep* ep, struct usb_request* req)
{
    struct fsg_common*	common = ep->driver_data;
    struct fsg_buffhd*	bh = req->context;

    if (req->status || req->actual != req->length)
        DBG_E(MBB_MASS, "%d, %u/%u\n",
              req->status, req->actual, req->length);

    if (req->status == -ECONNRESET)		/* Request was cancelled */
    { usb_ep_fifo_flush(ep); }

    /* Hold the lock while we update the request and buffer states */
    smp_wmb();
    spin_lock(&common->lock);
    bh->inreq_busy = 0;
    bh->state = BUF_STATE_EMPTY;
    wakeup_thread(common);
    spin_unlock(&common->lock);
}

static USB_VOID bulk_out_complete(struct usb_ep* ep, struct usb_request* req)
{
    struct fsg_common*	common = ep->driver_data;
    struct fsg_buffhd*	bh = req->context;
    struct bulk_cb_wrap*	cbw = bh->outreq->buf;

    cbw = req->buf;

    if (req->actual == US_BULK_CB_WRAP_LEN &&
        cbw->Signature != cpu_to_le32(US_BULK_CB_SIGN))
    {
        dump_msg(common, "bulk-out", req->buf, req->actual);
    }

    if (req->status || req->actual != bh->bulk_out_intended_length)
    {
        DBG_W(MBB_MASS, "%d, %u/%u\n",
              req->status, req->actual, bh->bulk_out_intended_length);

    }

    if (req->status == -ECONNRESET)		/* Request was cancelled */
    { usb_ep_fifo_flush(ep); }

    /* Hold the lock while we update the request and buffer states */
    smp_wmb();
    spin_lock(&common->lock);
    bh->outreq_busy = 0;
    bh->state = BUF_STATE_FULL;

    wakeup_thread(common);
    spin_unlock(&common->lock);
}

/* 为了规避在某些系统的bioss对scsi的处理不标准，进不了系统
   当scsi 在 Get Max Lun 之后超过2秒没有scsi交互，则做拉低
   D+，10秒后再拉高的操作。
*/
USB_VOID soft_mass_handler( USB_ULONG data )
{
    struct fsg_common* mass_common = NULL;
    struct mass_storage_function_config_data* configdata = NULL;

    if ((0 == data) && (1 == g_scsi_stat)) // 如果已经有scsi交互,且D+不为拉低状态时返回
    {
        DBG_I(MBB_PNP, " soft_mass_handler  scsi ok \n");
        return;
    }

    if (g_enabled_mass_function_list[0])
    {
        configdata = g_enabled_mass_function_list[0]->config;
        mass_common =  configdata->common;
    }

    if (NULL != mass_common)
    {
        if ((NULL != mass_common->gadget) && (0 == data))
        {
            usb_gadget_disconnect(mass_common->gadget);
            DBG_I(MBB_MASS, " soft_mass_handler usb_gadget_disconnect! \n");
            g_soft_mass_timer.data = 1;
            g_scsi_stat = 0;
            mod_timer(&g_soft_mass_timer, jiffies + msecs_to_jiffies(MASS_TIMER_TEN_SECOND));
        }

        if ((NULL != mass_common->gadget) && (1 == data))
        {
            DBG_I(MBB_MASS, " usb_gadget_connect! \n");
            usb_gadget_connect(mass_common->gadget);
            g_soft_mass_timer.data = 0;
        }
    }

    DBG_I(MBB_MASS, " soft_mass_handler process! \n");
}


static USB_INT fsg_setup(struct usb_function* f,
                         const struct usb_ctrlrequest* ctrl)
{
    struct fsg_dev*		fsg = fsg_from_func(f);
    struct usb_request*	req = fsg->common->ep0req;
    USB_UINT16			w_index = le16_to_cpu(ctrl->wIndex);
    USB_UINT16			w_value = le16_to_cpu(ctrl->wValue);
    USB_UINT16			w_length = le16_to_cpu(ctrl->wLength);

    if (!fsg_is_set(fsg->common))
    { return -EOPNOTSUPP; }

    ++fsg->common->ep0_req_tag;	/* Record arrival of a new request */
    req->context = NULL;
    req->length = 0;
    dump_msg(fsg, "ep0-setup", (USB_PUINT8) ctrl, sizeof(*ctrl));

    switch (ctrl->bRequest)
    {

        case US_BULK_RESET_REQUEST:
            if (ctrl->bRequestType !=
                (USB_DIR_OUT | USB_TYPE_CLASS | USB_RECIP_INTERFACE))
            { break; }

            if (w_index != fsg->interface_number || w_value != 0 ||
                w_length != 0)
            { return -EDOM; }

            /*
             * Raise an exception to stop the current operation
             * and reinitialize our state.
             */
            DBG_W(MBB_MASS, "bulk reset request\n");
            raise_exception(fsg->common, FSG_STATE_RESET);
            if (fsg->common->cdev)
            { return USB_GADGET_DELAYED_STATUS; }
            else
            { return DELAYED_STATUS; }

        case US_BULK_GET_MAX_LUN:
            if (ctrl->bRequestType !=
                (USB_DIR_IN | USB_TYPE_CLASS | USB_RECIP_INTERFACE))
            { break; }

            if (w_index != fsg->interface_number || w_value != 0 ||
                w_length != 1)
            { return -EDOM; }

            DBG_T(MBB_MASS, "get max LUN %d\n", fsg->common->nluns);
            *(USB_PUINT8)req->buf = fsg->common->nluns - 1;

            /* Respond with data/status */
            req->length = min((USB_UINT16)1, w_length);

            /*  HILINK 切换  */
#ifdef USB_RNDIS

            if (pnp_is_rewind_before_mode() && pnp_is_service_switch())
            {
                DBG_T(MBB_MASS, "GET MAX LUN REWIND\n");
                pnp_switch_rewind_after_mode();
            }

#endif
            g_scsi_stat = 0;
            /*2s没有SCSI交互规避bios卡住问题*/
            mod_timer(&g_soft_mass_timer, jiffies + msecs_to_jiffies(MASS_TIMER_FIVE_SECOND));
            return ep0_queue(fsg->common);
    }

    DBG_I(MBB_MASS,
          "unknown class-specific control req %02x.%02x v%04x i%04x l%u\n",
          ctrl->bRequestType, ctrl->bRequest,
          le16_to_cpu(ctrl->wValue), w_index, w_length);
    return -EOPNOTSUPP;
}

/*-------------------------------------------------------------------------*/

/* All the following routines run in process context */

/* Use this for bulk or interrupt transfers, not ep0 */
static USB_VOID start_transfer(struct fsg_dev* fsg, struct usb_ep* ep,
                               struct usb_request* req, USB_INT* pbusy,
                               enum fsg_buffer_state* state)
{
    USB_INT	rc;
    spin_lock_irq(&fsg->common->lock);

    if (ep == fsg->bulk_in && !req->length)
    {
        *pbusy = 0;
        *state = BUF_STATE_EMPTY;
        spin_unlock_irq(&fsg->common->lock);
        return;
    }

    *pbusy = 1;
    *state = BUF_STATE_BUSY;
    spin_unlock_irq(&fsg->common->lock);
    rc = usb_ep_queue(ep, req, GFP_KERNEL);

    if (rc != 0)
    {
        *pbusy = 0;
        *state = BUF_STATE_EMPTY;
        /* We can't do much more than wait for a reset */

        /*
         * Note: currently the net2280 driver fails zero-length
         * submissions if DMA is enabled.
         */
        if (rc != -ESHUTDOWN &&
            !(rc == -EOPNOTSUPP && req->length == 0))
            DBG_E(MBB_MASS, "error in submission: %s --> %d\n",
                  ep->name, rc);
    }
}

static bool start_in_transfer(struct fsg_common* common, struct fsg_buffhd* bh)
{
    if (!fsg_is_set(common))
    { return false; }

    bh->inreq->zero = 0;
    start_transfer(common->fsg, common->fsg->bulk_in,
                   bh->inreq, &bh->inreq_busy, &bh->state);
    return true;
}

static bool start_out_transfer(struct fsg_common* common, struct fsg_buffhd* bh)
{
    if (!fsg_is_set(common))
    { return false; }

    start_transfer(common->fsg, common->fsg->bulk_out,
                   bh->outreq, &bh->outreq_busy, &bh->state);
    return true;
}
static USB_INT sleep_thread(struct fsg_common* common)
{
    USB_INT	rc = 0;

    /* Wait until a signal arrives or we are woken up */
    for (;;)
    {
        try_to_freeze();
        set_current_state(TASK_INTERRUPTIBLE);

        if (signal_pending(current))
        {
            rc = -EINTR;
            break;
        }

        if (common->thread_wakeup_needed)
        { break; }

        schedule();
    }

    __set_current_state(TASK_RUNNING);
    common->thread_wakeup_needed = 0;

    return rc;
}

/*-------------------------------------------------------------------------*/
static USB_INT do_read(struct fsg_common* common)
{
    struct fsg_lun*		curlun = common->curlun;
    USB_UINT32			lba;
    struct fsg_buffhd*	bh;
    USB_INT			rc;
    USB_UINT		amount_left;
    loff_t			file_offset, file_offset_tmp;
    USB_UINT		amount;
    ssize_t			nread;
    struct mass_storage_function_config_data* configdata = common->private_data;
    struct fsg_state_count* fsg_state_for_common = &configdata->fsg_state;

    /*
     * Get the starting Logical Block Address and check that it's
     * not too big.
     */
    if (common->cmnd[0] == READ_6)
    { lba = get_unaligned_be24(&common->cmnd[1]); }
    else
    {
        lba = get_unaligned_be32(&common->cmnd[2]);

        /*
         * We allow DPO (Disable Page Out = don't save data in the
         * cache) and FUA (Force Unit Access = don't read from the
         * cache), but we don't implement them.
         */
        if ((common->cmnd[1] & ~0x18) != 0)
        {
            curlun->sense_data = SS_INVALID_FIELD_IN_CDB;
            return -EINVAL;
        }
    }

    if (lba >= curlun->num_sectors)
    {
        curlun->sense_data = SS_LOGICAL_BLOCK_ADDRESS_OUT_OF_RANGE;
        return -EINVAL;
    }

    file_offset = ((loff_t) lba) << curlun->blkbits;

    /* Carry out the file reads */
    amount_left = common->data_size_from_cmnd;

    if (unlikely(amount_left == 0))
    { return -EIO; }		/* No default reply */

    for (;;)
    {
        /*
         * Figure out how much we need to read:
         * Try to read the remaining amount.
         * But don't read more than the buffer size.
         * And don't try to read past the end of the file.
         */
        amount = min(amount_left, FSG_BUFLEN);
        amount = min((loff_t)amount,
                     curlun->file_length - file_offset);

        /* Wait for the next buffer to become available */
        bh = common->next_buffhd_to_fill;

        while (bh->state != BUF_STATE_EMPTY)
        {

            rc = sleep_thread(common);

            if (rc)
            {
                fsg_state_for_common->signal_pending++;
                fsg_state_for_common->signal_pending_line = __LINE__;
                return rc;
            }
        }

        /*
         * If we were asked to read past the end of file,
         * end with an empty buffer.
         */
        if (amount == 0)
        {
            curlun->sense_data =
                SS_LOGICAL_BLOCK_ADDRESS_OUT_OF_RANGE;
            curlun->sense_data_info =
                file_offset >> curlun->blkbits;
            curlun->info_valid = 1;
            bh->inreq->length = 0;
            bh->state = BUF_STATE_FULL;
            break;
        }

        /* Perform the read */
        file_offset_tmp = file_offset;

        /*file removed randomly*/
        if ( NULL == curlun->filp )
        {
            DBG_E(MBB_MASS, "curlun->filp null stop to read!\n");
            curlun->sense_data = SS_UNRECOVERED_READ_ERROR;
            curlun->sense_data_info =
                file_offset >> curlun->blkbits;
            curlun->info_valid = 1;
            bh->state = BUF_STATE_FULL;
            bh->inreq->length = 0;
            break;
        }

        nread = vfs_read(curlun->filp,
                         (char __user*)bh->buf,
                         amount, &file_offset_tmp);
#ifdef USB_SD

        /*SD_CARD removed randomly*/
        if (!curlun->cdrom)
        {
            if ( SD_CARD_NOT_PRESENT == g_sd_card_info.sd_card_status
                 || SD_WORKMODE_MASS != g_sd_card_info.sd_card_workmode)
            {
                DBG_E(MBB_MASS, "SD_CARD_NOT_PRESENT! stop to read!\n");
                curlun->sense_data = SS_UNRECOVERED_READ_ERROR;
                curlun->sense_data_info =
                    file_offset >> curlun->blkbits;
                curlun->info_valid = 1;
                bh->state = BUF_STATE_FULL;
                bh->inreq->length = 0;
                mass_storage_close_usb_sd();
                break;
            }
        }

#endif

        if (signal_pending(current))
        {

            fsg_state_for_common->signal_pending++;
            fsg_state_for_common->signal_pending_line = __LINE__;
            return -EINTR;
        }

        if (nread < 0)
        {
            DBG_E(MBB_MASS, "error in file read: %d\n", (USB_INT)nread);
            nread = 0;
        }
        else if (nread < amount)
        {
            DBG_E(MBB_MASS, "partial file read: %d/%u\n",
                  (USB_INT)nread, amount);
            nread = round_down(nread, curlun->blksize);
        }

        file_offset  += nread;
        amount_left  -= nread;
        common->residue -= nread;

        /*
         * Except at the end of the transfer, nread will be
         * equal to the buffer size, which is divisible by the
         * bulk-in maxpacket size.
         */
        bh->inreq->length = nread;
        bh->state = BUF_STATE_FULL;

        /* If an error occurred, report it and its position */
        if (nread < amount)
        {
            curlun->sense_data = SS_UNRECOVERED_READ_ERROR;
            curlun->sense_data_info =
                file_offset >> curlun->blkbits;
            curlun->info_valid = 1;
            break;
        }

        if (amount_left == 0)
        { break; }		/* No more left to read */

        /* Send this buffer and go read some more */
        bh->inreq->zero = 0;

        if (!start_in_transfer(common, bh))
            /* Don't know what to do if common->fsg is NULL */
        { return -EIO; }

        common->next_buffhd_to_fill = bh->next;
    }

    return -EIO;		/* No default reply */
}


/*-------------------------------------------------------------------------*/

static USB_INT do_write(struct fsg_common* common)
{
    struct fsg_lun*		curlun = common->curlun;
    USB_UINT32			lba;
    struct fsg_buffhd*	bh;
    USB_INT			get_some_more;
    USB_UINT			amount_left_to_req, amount_left_to_write;
    loff_t			usb_offset, file_offset, file_offset_tmp;
    USB_UINT		amount;
    ssize_t			nwritten;
    USB_INT			rc;
    struct mass_storage_function_config_data* configdata = common->private_data;
    struct fsg_state_count* fsg_state_for_common = &configdata->fsg_state;

    if (curlun->ro)
    {
        curlun->sense_data = SS_WRITE_PROTECTED;
        return -EINVAL;
    }

    spin_lock(&curlun->filp->f_lock);
    curlun->filp->f_flags &= ~O_SYNC;	/* Default is not to wait */
    spin_unlock(&curlun->filp->f_lock);

    /*
     * Get the starting Logical Block Address and check that it's
     * not too big
     */
    if (common->cmnd[0] == WRITE_6)
    { lba = get_unaligned_be24(&common->cmnd[1]); }
    else
    {
        lba = get_unaligned_be32(&common->cmnd[2]);

        /*
         * We allow DPO (Disable Page Out = don't save data in the
         * cache) and FUA (Force Unit Access = write directly to the
         * medium).  We don't implement DPO; we implement FUA by
         * performing synchronous output.
         */
        if (common->cmnd[1] & ~0x18)
        {
            curlun->sense_data = SS_INVALID_FIELD_IN_CDB;
            return -EINVAL;
        }

        if (!curlun->nofua && (common->cmnd[1] & 0x08))   /* FUA */
        {
            spin_lock(&curlun->filp->f_lock);
            curlun->filp->f_flags |= O_SYNC;
            spin_unlock(&curlun->filp->f_lock);
        }
    }

    if (lba >= curlun->num_sectors)
    {
        curlun->sense_data = SS_LOGICAL_BLOCK_ADDRESS_OUT_OF_RANGE;
        return -EINVAL;
    }

    /* Carry out the file writes */
    get_some_more = 1;
    file_offset = usb_offset = ((loff_t) lba) << curlun->blkbits;
    amount_left_to_req = common->data_size_from_cmnd;
    amount_left_to_write = common->data_size_from_cmnd;

    while (amount_left_to_write > 0)
    {

        /* Queue a request for more data from the host */
        bh = common->next_buffhd_to_fill;

        if (bh->state == BUF_STATE_EMPTY && get_some_more)
        {

            /*
             * Figure out how much we want to get:
             * Try to get the remaining amount,
             * but not more than the buffer size.
             */
            amount = min(amount_left_to_req, FSG_BUFLEN);

            /* Beyond the end of the backing file? */
            if (usb_offset >= curlun->file_length)
            {
                get_some_more = 0;
                curlun->sense_data =
                    SS_LOGICAL_BLOCK_ADDRESS_OUT_OF_RANGE;
                curlun->sense_data_info =
                    usb_offset >> curlun->blkbits;
                curlun->info_valid = 1;
                continue;
            }

            /* Get the next buffer */
            usb_offset += amount;
            common->usb_amount_left -= amount;
            amount_left_to_req -= amount;

            if (amount_left_to_req == 0)
            { get_some_more = 0; }

            /*
             * Except at the end of the transfer, amount will be
             * equal to the buffer size, which is divisible by
             * the bulk-out maxpacket size.
             */
            set_bulk_out_req_length(common, bh, amount);

            if (!start_out_transfer(common, bh))
                /* Dunno what to do if common->fsg is NULL */
            { return -EIO; }

            common->next_buffhd_to_fill = bh->next;
            continue;
        }

        /* Write the received data to the backing file */
        bh = common->next_buffhd_to_drain;

        if (bh->state == BUF_STATE_EMPTY && !get_some_more)
        { break; }			/* We stopped early */

        if (bh->state == BUF_STATE_FULL)
        {
            smp_rmb();
            common->next_buffhd_to_drain = bh->next;
            bh->state = BUF_STATE_EMPTY;

            /* Did something go wrong with the transfer? */
            if (bh->outreq->status != 0)
            {
                curlun->sense_data = SS_COMMUNICATION_FAILURE;
                curlun->sense_data_info =
                    file_offset >> curlun->blkbits;
                curlun->info_valid = 1;
                break;
            }

            amount = bh->outreq->actual;

            if (curlun->file_length - file_offset < amount)
            {
                DBG_E(MBB_MASS,
                      "write %u @ %llu beyond end %llu\n",
                      amount, (USB_UINT64)file_offset,
                      (USB_UINT64)curlun->file_length);
                amount = curlun->file_length - file_offset;
            }

            /* Don't accept excess data.  The spec doesn't say
             * what to do in this case.  We'll ignore the error.
             */
            amount = min(amount, bh->bulk_out_intended_length);

            /* Don't write a partial block */
            amount = round_down(amount, curlun->blksize);

            if (amount == 0)
            { goto empty_write; }

            /* Perform the write */
            file_offset_tmp = file_offset;

            /*file removed randomly*/
            if ( NULL == curlun->filp )
            {
                DBG_E(MBB_MASS, "curlun->filp null stop to write!\n");
                curlun->sense_data = SS_WRITE_ERROR;
                curlun->sense_data_info =
                    file_offset >> curlun->blkbits;
                curlun->info_valid = 1;
                break;
            }

            nwritten = vfs_write(curlun->filp,
                                 (char __user*)bh->buf,
                                 amount, &file_offset_tmp);
#ifdef USB_SD

            /*SD_CARD removed randomly*/
            if (!curlun->cdrom)
            {
                if ( SD_CARD_NOT_PRESENT == g_sd_card_info.sd_card_status
                     || SD_WORKMODE_MASS != g_sd_card_info.sd_card_workmode)
                {
                    DBG_E(MBB_MASS, "SD_CARD_NOT_PRESENT! stop to write!\n");
                    curlun->sense_data = SS_WRITE_ERROR;
                    curlun->sense_data_info =
                        file_offset >> curlun->blkbits;
                    curlun->info_valid = 1;
                    mass_storage_close_usb_sd();
                    break;
                }
            }

#endif
            DBG_I(MBB_MASS, "file write %u @ %llu -> %d\n", amount,
                  (USB_UINT64)file_offset, (USB_INT)nwritten);

            if (signal_pending(current))
            {

                fsg_state_for_common->signal_pending++;
                fsg_state_for_common->signal_pending_line = __LINE__;
                return -EINTR;		/* Interrupted! */

            }

            if (nwritten < 0)
            {
                DBG_E(MBB_MASS, "error in file write: %d\n",
                      (USB_INT)nwritten);
                nwritten = 0;
            }
            else if (nwritten < amount)
            {
                DBG_E(MBB_MASS, "partial file write: %d/%u\n",
                      (USB_INT)nwritten, amount);
                nwritten = round_down(nwritten, curlun->blksize);
            }

            file_offset += nwritten;
            amount_left_to_write -= nwritten;
            common->residue -= nwritten;

            /* If an error occurred, report it and its position */
            if (nwritten < amount)
            {
                curlun->sense_data = SS_WRITE_ERROR;
                curlun->sense_data_info =
                    file_offset >> curlun->blkbits;
                curlun->info_valid = 1;
                break;
            }

        empty_write:

            /* Did the host decide to stop early? */
            if (bh->outreq->actual < bh->bulk_out_intended_length)
            {
                common->short_packet_received = 1;
                break;
            }

            continue;
        }

        /* Wait for something to happen */
        rc = sleep_thread(common);

        if (rc)
        {

            fsg_state_for_common->signal_pending++;
            fsg_state_for_common->signal_pending_line = __LINE__;
            return rc;
        }
    }

    return -EIO;		/* No default reply */
}


/*-------------------------------------------------------------------------*/

static USB_INT do_synchronize_cache(struct fsg_common* common)
{
    struct fsg_lun*	curlun = common->curlun;
    USB_INT		rc;

    /* We ignore the requested LBA and write out all file's
     * dirty data buffers. */
    rc = fsg_lun_fsync_sub(curlun);

    if (rc)
    { curlun->sense_data = SS_WRITE_ERROR; }

    return 0;
}


/*-------------------------------------------------------------------------*/

static USB_VOID invalidate_sub(struct fsg_lun* curlun)
{
    struct file*	filp = curlun->filp;
    struct inode*	inode = filp->f_path.dentry->d_inode;
    USB_UINT32	rc;

    rc = invalidate_mapping_pages(inode->i_mapping, 0, -1);
    DBG_T(MBB_MASS, "invalidate_mapping_pages -> %ld\n", rc);
}

static USB_INT do_verify(struct fsg_common* common)
{
    struct fsg_lun*		curlun = common->curlun;
    USB_UINT32			lba;
    USB_UINT32			verification_length;
    loff_t			file_offset;
    USB_UINT			amount_left;
    struct mass_storage_function_config_data* configdata = common->private_data;
    struct fsg_state_count* fsg_state_for_common = &configdata->fsg_state;

    /*
     * Get the starting Logical Block Address and check that it's
     * not too big.
     */
    lba = get_unaligned_be32(&common->cmnd[2]);

    if (lba >= curlun->num_sectors)
    {
        curlun->sense_data = SS_LOGICAL_BLOCK_ADDRESS_OUT_OF_RANGE;
        return -EINVAL;
    }

    /*
     * We allow DPO (Disable Page Out = don't save data in the
     * cache) but we don't implement it.
     */
    if (common->cmnd[1] & ~0x10)
    {
        curlun->sense_data = SS_INVALID_FIELD_IN_CDB;
        return -EINVAL;
    }

    verification_length = get_unaligned_be16(&common->cmnd[7]);

    if (unlikely(verification_length == 0))
    { return -EIO; }		/* No default reply */

    /* Prepare to carry out the file verify */
    amount_left = verification_length << curlun->blkbits;
    file_offset = ((loff_t) lba) << curlun->blkbits;

    /* Write out all the dirty buffers before invalidating them */
    fsg_lun_fsync_sub(curlun);

    if (signal_pending(current))
    {

        fsg_state_for_common->signal_pending++;
        fsg_state_for_common->signal_pending_line = __LINE__;

        return -EINTR;

    }

    invalidate_sub(curlun);

    if (signal_pending(current))
    {

        fsg_state_for_common->signal_pending++;
        fsg_state_for_common->signal_pending_line = __LINE__;

        return -EINTR;
    }

    if (file_offset + amount_left > curlun->file_length)
    {
        DBG_E(MBB_MASS, "verfiy err bad read parametes\n");
        return -EINTR;
    }
    else
    {
        return 0;
    }

    /* Just try to read the requested blocks */
#if 0

    while (amount_left > 0)
    {
        /*
         * Figure out how much we need to read:
         * Try to read the remaining amount, but not more than
         * the buffer size.
         * And don't try to read past the end of the file.
         */
        amount = min(amount_left, FSG_BUFLEN);
        amount = min((loff_t)amount,
                     curlun->file_length - file_offset);

        if (amount == 0)
        {
            curlun->sense_data =
                SS_LOGICAL_BLOCK_ADDRESS_OUT_OF_RANGE;
            curlun->sense_data_info =
                file_offset >> curlun->blkbits;
            curlun->info_valid = 1;
            break;
        }

        /* Perform the read */
        file_offset_tmp = file_offset;
        nread = vfs_read(curlun->filp,
                         (char __user*) bh->buf,
                         amount, &file_offset_tmp);
        DBG_E(MBB_MASS, ( "file read %u @ %llu -> %d\n", amount,
                          (USB_UINT64) file_offset,
                          (USB_INT) nread));

        if (signal_pending(current))
        {

            fsg_state_for_common->signal_pending++;
            fsg_state_for_common->signal_pending_line = __LINE__;
            return -EINTR;
        }

        if (nread < 0)
        {
            DBG_E(MBB_MASS, "error in file verify: %d\n", (USB_INT)nread);
            nread = 0;
        }
        else if (nread < amount)
        {
            DBG_E(MBB_MASS, ("partial file verify: %d/%u\n",
                             (USB_INT)nread, amount));
            nread = round_down(nread, curlun->blksize);
        }

        if (nread == 0)
        {
            curlun->sense_data = SS_UNRECOVERED_READ_ERROR;
            curlun->sense_data_info =
                file_offset >> curlun->blkbits;
            curlun->info_valid = 1;
            break;
        }

        file_offset += nread;
        amount_left -= nread;
    }

#endif
    return 0;
}

/*------------------------------------------------------------
  名称     : void mmc_set_addr(mmc_addr_t *addr, u32 lba, int is_msf)
  描述     : 计算msf参数
  输入     : addr 数据存放地址  lab 逻辑地址 is_msf 编码格式
  输出     : 无
  返回值   : 无
-------------------------------------------------------------*/
static void mmc_set_addr(mmc_addr_t* addr, u32 lba, int is_msf)
{
    if (!is_msf)
    {
        put_unaligned_be32( lba, &addr->lba );
        return;
    }

    /* Calculate MSF */
    addr->msf.reserved = 0;
    addr->msf.frame = lba % 75;
    lba /= 75;  /* Got the frames. Now convert to seconds */
    lba += 2;   /* LBA 0 is always mapped to MSF 0/2/0 */
    addr->msf.min = lba / 60;
    addr->msf.sec = lba % 60;
}

/*------------------------------------------------------------
  名称     : static int do_read_toc(struct fsg_dev *fsg, struct fsg_buffhd *bh)
  描述     : 回复read toc scsi命令
  输入     : struct fsg_dev *fsg, struct fsg_buffhd *bh
  输出     : 无
  返回值   : 数据长度
-------------------------------------------------------------*/
static USB_INT do_read_toc(struct fsg_common* common, struct fsg_buffhd* bh)
{
    struct fsg_lun*  curlun = common->curlun;
    int msf = 0;
    u8 mmc_format = 0;
    u8 sff8020_format = 0;
    u16 data_len = 4;
    signed int  i = 0;
    scsi_toc_t* toc = (scsi_toc_t*)(bh->buf);
    track_param_t* track = NULL;

    if (!curlun)
    {
        return -1;
    }

    memset(bh->buf, 0, ((common->data_size_from_cmnd == 0xFFFE) ?
                        512 : common->data_size_from_cmnd));

    /* need to be verified in further */
    /* modifieid end by mabinjie at 2010-12-06 */

    mmc_format = common->cmnd[2] & 0xf;
    msf = (common->cmnd[1] & 0x2) ? 1 : 0;

    /* SFF8020, version 1.2 specification define READ_TOC in a different way,
     * and it is relevant only when MMC format is zero */
    if (!mmc_format)
    {
        sff8020_format = (common->cmnd[9] & 0xC0) >> 6;
    }

    if (mmc_format > 2 || sff8020_format > 2)
    {
        DBG_E(MBB_MASS, "unsupported format [%u/%u]\n",
              mmc_format, sff8020_format);
        return -1 ;
    }

    /* First and last session / track number - relevant for all response
     * formats */
    toc->first_track = toc->last_track = 1;

    if (sff8020_format == 0x2 || mmc_format == 0x2)
    {
        session_param_t* session = (session_param_t*)toc->params;

        for (i = 0; i < 4; i++)
        {
            session[i].session_num = 1;
            session[i].adr_control = (0x1 << 4) | 0x4;
        }

        session[0].point = 0xA0;
        session[0].addr.msf.min = 1;

        session[1].point = 0xA1;
        session[1].addr.msf.min = 1;

        session[2].point = 0xA2;
        mmc_set_addr(&session[2].addr, curlun->num_sectors, 1);

        session[3].point = 0x01;

        data_len += 4 * SESSION_PARAM_SIZE;
        goto Done;
    }

    /* Fill the response format as specified in MMC6-r00, Table 482 */
    track = (track_param_t*)toc->params;

    track[0].adr_control = (0x1 << 4) | 0x4; /* Mode-1, Copy permitted */
    track[0].track_number = 1;
    mmc_set_addr(&track[0].addr,  0 , msf );

    data_len += sizeof(track_param_t);

    if (!sff8020_format || !mmc_format)
    {
        track[1].adr_control = (0x1 << 4) | 0x4; /* Mode-1, Copy permitted */
        track[1].track_number = 0xAA;            /* 0xAA == Lead out Area */
        mmc_set_addr(&track[1].addr, curlun->num_sectors, msf);

        data_len += sizeof(track_param_t);
    }

Done:
    /* MMC-r10a states that the data_len field size should be excluded */
    put_unaligned_be16(data_len - sizeof(toc->data_len), &toc->data_len);

    if ( data_len < common->data_size_from_cmnd )
    {
        common->data_size_from_cmnd = data_len;
    }

    common->data_size = common->data_size_from_cmnd;

    common->residue = common->data_size;
    common->usb_amount_left = common->data_size;

    return common->data_size_from_cmnd;
}

static USB_INT do_get_configuration(struct fsg_common* common, struct fsg_buffhd *
                                    bh)
{
    USB_PUINT8    buf = (USB_PUINT8) bh->buf;
    usbsdms_get_configuration_cmd_type* get_configuration_cmd = NULL;
    USB_INT def_configuration_len  = sizeof(pnp_get_configration_respond);
    USB_UINT32 response_length = 0;

    get_configuration_cmd = (usbsdms_get_configuration_cmd_type*) & (common->cmnd[0]);

    if (get_configuration_cmd->allocation_length >= def_configuration_len)
    {
        response_length = def_configuration_len;
    }
    else
    {
        response_length = get_configuration_cmd->allocation_length;
    }

    memcpy(buf, pnp_get_configration_respond, response_length);

    if (response_length < common->data_size_from_cmnd)
    {
        common->data_size_from_cmnd = response_length;
    }

    common->data_size = common->data_size_from_cmnd;

    common->residue =  common->data_size;
    common->usb_amount_left = common->data_size;
    return response_length;

}
static USB_INT do_test_unit(struct fsg_common* common, struct fsg_buffhd* bh)
{
    common->residue = 0x00000000;//for mbb and pc interface 2.3 pnp
    return 1;
}

/*-------------------------------------------------------------------------*/
USB_VOID set_inquiry_unit_serial_number_page(USB_PUINT8 serialnumberpage)
{
    USB_PUINT8 serial_number = NULL;

    //serial_number = get_serial_number();
    if ( serial_number )
    {
        memcpy(serialnumberpage, serial_number, INQUIRY_PAGE_LENGTH);
    }
    else
    {
        memset(serialnumberpage, 0x20, INQUIRY_PAGE_LENGTH);
    }
}

static USB_INT do_inquiry(struct fsg_common* common, struct fsg_buffhd* bh)
{
    struct fsg_lun* curlun = common->curlun;
    USB_PUINT8 buf = (USB_PUINT8) bh->buf;

    if (!curlun)  		/* Unsupported LUNs are okay */
    {
        common->bad_lun_okay = 1;
        memset(buf, 0, 36);
        buf[0] = 0x7f;		/* Unsupported, no device-type */
        buf[4] = 31;		/* Additional length */
        return 36;
    }

    /*Vital product data parameters return*/
    if ( Enable_Vital_Product_Data == common->cmnd[1] && Unit_Serial_Number_Page == common->cmnd[2])
    {
        buf[0] = (Currentlun_Connect_OR_Unable_To_Determine) && (curlun->cdrom ? TYPE_ROM : TYPE_DISK);
        buf[1] = Unit_Serial_Number_Page;
        buf[2] = RESERVED;
        buf[3] = INQUIRY_PAGE_LENGTH;
        set_inquiry_unit_serial_number_page(&buf[4]);
        return Unit_Serial_Number_Page_Length;
    }

    buf[0] = curlun->cdrom ? TYPE_ROM : TYPE_DISK;
    buf[1] = curlun->removable ? 0x80 : 0;
    buf[2] = 2;		/* ANSI SCSI level 2 */
    buf[3] = 2;		/* SCSI-2 INQUIRY data format */
    buf[4] = 31;		/* Additional length */
    buf[5] = 0;		/* No special options */
    buf[6] = 0;
    buf[7] = 0;
    memcpy(buf + USB_NUM_8, curlun->inquiry_string, sizeof curlun->inquiry_string);

    return 36;
}

static USB_INT do_request_sense(struct fsg_common* common, struct fsg_buffhd* bh)
{
    struct fsg_lun*	curlun = common->curlun;
    USB_PUINT8 buf = (USB_PUINT8) bh->buf;
    USB_UINT32		sd, sdinfo;
    USB_INT		valid;

    /*
     * From the SCSI-2 spec., section 7.9 (Unit attention condition):
     *
     * If a REQUEST SENSE command is received from an initiator
     * with a pending unit attention condition (before the target
     * generates the contingent allegiance condition), then the
     * target shall either:
     *   a) report any pending sense data and preserve the unit
     *	attention condition on the logical unit, or,
     *   b) report the unit attention condition, may discard any
     *	pending sense data, and clear the unit attention
     *	condition on the logical unit for that initiator.
     *
     * FSG normally uses option a); enable this code to use option b).
     */
#if 0

    if (curlun && curlun->unit_attention_data != SS_NO_SENSE)
    {
        curlun->sense_data = curlun->unit_attention_data;
        curlun->unit_attention_data = SS_NO_SENSE;
    }

#endif

    if (!curlun)  		/* Unsupported LUNs are okay */
    {
        common->bad_lun_okay = 1;
        sd = SS_LOGICAL_UNIT_NOT_SUPPORTED;
        sdinfo = 0;
        valid = 0;
    }
    else
    {
        sd = curlun->sense_data;
        sdinfo = curlun->sense_data_info;
        valid = curlun->info_valid << 7;
        curlun->sense_data = SS_NO_SENSE;
        curlun->sense_data_info = 0;
        curlun->info_valid = 0;
    }

    memset(buf, 0, 18);
    buf[0] = valid | 0x70;			/* Valid, current error */
    buf[2] = SK(sd);
    put_unaligned_be32(sdinfo, &buf[3]);	/* Sense information */
    buf[7] = 18 - 8;			/* Additional sense length */
    buf[12] = ASC(sd);
    buf[13] = ASCQ(sd);
    return 18;
}

static int do_XPWrite(struct fsg_common* common, struct fsg_buffhd* bh)
{
    struct fsg_lun*	curlun = common->curlun;

    common->bad_lun_okay = 1;
    curlun->sense_data = SS_NO_SENSE;
    common->phase_error = 0;
    common->cmnd_return_zero = 1;

    return common->data_size_from_cmnd;
}

static USB_INT do_read_capacity(struct fsg_common* common, struct fsg_buffhd* bh)
{
    struct fsg_lun*	curlun = common->curlun;
    USB_UINT32		lba = get_unaligned_be32(&common->cmnd[2]);
    USB_INT		pmi = common->cmnd[8];
    USB_PUINT8 buf = (USB_PUINT8)bh->buf;

    /* Check the PMI and LBA fields */
    if (pmi > 1 || (pmi == 0 && lba != 0))
    {
        curlun->sense_data = SS_INVALID_FIELD_IN_CDB;
        return -EINVAL;
    }

    put_unaligned_be32(curlun->num_sectors - 1, &buf[0]);
    /* Max logical block */
    put_unaligned_be32(curlun->blksize, &buf[4]);/* Block length */
    return 8;
}

static USB_INT do_read_header(struct fsg_common* common, struct fsg_buffhd* bh)
{
    struct fsg_lun*	curlun = common->curlun;
    USB_INT		msf = common->cmnd[1] & 0x02;
    USB_UINT32		lba = get_unaligned_be32(&common->cmnd[2]);
    USB_PUINT8 buf = (USB_PUINT8)bh->buf;

    if (common->cmnd[1] & ~0x02)  		/* Mask away MSF */
    {
        curlun->sense_data = SS_INVALID_FIELD_IN_CDB;
        return -EINVAL;
    }

    if (lba >= curlun->num_sectors)
    {
        curlun->sense_data = SS_LOGICAL_BLOCK_ADDRESS_OUT_OF_RANGE;
        return -EINVAL;
    }

    memset(buf, 0, 8);
    buf[0] = 0x01;		/* 2048 bytes of user data, rest is EC */
    store_cdrom_address(&buf[4], msf, lba);
    return 8;
}

static USB_INT do_mode_sense(struct fsg_common* common, struct fsg_buffhd* bh)
{
    struct fsg_lun*	curlun = common->curlun;
    USB_INT		mscmnd = common->cmnd[0];
    USB_PUINT8 buf = (USB_PUINT8) bh->buf;
    USB_PUINT8 buf0 = buf;
    USB_INT		pc, page_code;
    USB_INT		changeable_values, all_pages;
    USB_INT		valid_page = 0;
    USB_INT		len, limit;

    if ((common->cmnd[1] & ~0x08) != 0)  	/* Mask away DBD */
    {
        curlun->sense_data = SS_INVALID_FIELD_IN_CDB;
        return -EINVAL;
    }

    pc = common->cmnd[2] >> 6;
    page_code = common->cmnd[2] & 0x3f;

    if (pc == 3)
    {
        curlun->sense_data = SS_SAVING_PARAMETERS_NOT_SUPPORTED;
        return -EINVAL;
    }

    changeable_values = (pc == 1);
    all_pages = (page_code == 0x3f);

    /*
     * Write the mode parameter header.  Fixed values are: default
     * medium type, no cache control (DPOFUA), and no block descriptors.
     * The only variable value is the WriteProtect bit.  We will fill in
     * the mode data length later.
     */
    memset(buf, 0, 8);

    if (mscmnd == MODE_SENSE)
    {
        buf[2] = (curlun->ro ? 0x80 : 0x00);		/* WP, DPOFUA */
        buf += 4;
        limit = 255;
    }
    else  			/*MODE_SENSE_10 */
    {
        buf[3] = (curlun->ro ? 0x80 : 0x00);		/* WP, DPOFUA */
        buf += 8;
        limit = 65535;		/* Should really be FSG_BUFLEN */
    }

    /* No block descriptors */

    /*
     * The mode pages, in numerical order.  The only page we support
     * is the Caching page.
     */
    if (page_code == 0x08 || all_pages)
    {
        valid_page = 1;
        buf[0] = 0x08;		/* Page code */
        buf[1] = 10;		/* Page length */
        memset(buf + 2, 0, 10);	/* None of the fields are changeable */

        if (!changeable_values)
        {
            buf[2] = 0x04;	/* Write cache enable, */
            /* Read cache not disabled */
            /* No cache retention priorities */
            put_unaligned_be16(0xffff, &buf[4]);
            /* Don't disable prefetch */
            /* Minimum prefetch = 0 */
            put_unaligned_be16(0xffff, &buf[8]);
            /* Maximum prefetch */
            put_unaligned_be16(0xffff, &buf[10]);
            /* Maximum prefetch ceiling */
        }

        buf += 12;
    }

    /*
     * Check that a valid page was requested and the mode data length
     * isn't too long.
     */
    len = buf - buf0;

    if (!valid_page || len > limit)
    {
        curlun->sense_data = SS_INVALID_FIELD_IN_CDB;
        return -EINVAL;
    }

    /*  Store the mode data length */
    if (mscmnd == MODE_SENSE)
    { buf0[0] = len - 1; }
    else
    { put_unaligned_be16(len - 2, buf0); }

    return len;
}

static USB_INT do_start_stop(struct fsg_common* common)
{
    struct fsg_lun*	curlun = common->curlun;
    USB_INT		loej, start;

    if (!curlun)
    {
        return -EINVAL;
    }
    else if (!curlun->removable)
    {
        curlun->sense_data = SS_INVALID_COMMAND;
        return -EINVAL;
    }
    else if ((common->cmnd[1] & ~0x01) != 0 ||   /* Mask away Immed */
             (common->cmnd[4] & ~0x03) != 0)   /* Mask LoEj, Start */
    {
        curlun->sense_data = SS_INVALID_FIELD_IN_CDB;
        return -EINVAL;
    }

    loej  = common->cmnd[4] & 0x02;
    start = common->cmnd[4] & 0x01;

    /*
     * Our emulation doesn't support mounting; the medium is
     * available for use as soon as it is loaded.
     */
    if (start)
    {
        if (!fsg_lun_is_open(curlun))
        {
            curlun->sense_data = SS_MEDIUM_NOT_PRESENT;
            return -EINVAL;
        }

        return 0;
    }

    /* Are we allowed to unload the media? */
    if (curlun->prevent_medium_removal)
    {
        DBG_E(MBB_MASS, "unload attempt prevented\n");
        curlun->sense_data = SS_MEDIUM_REMOVAL_PREVENTED;
        return -EINVAL;
    }

    if (!loej)
    { return 0; }

    /* Simulate an unload/eject */
    if (common->ops && common->ops->pre_eject)
    {
        USB_INT r = common->ops->pre_eject(common, curlun,
                                           curlun - common->luns);

        if (unlikely(r < 0))
        { return r; }
        else if (r)
        { return 0; }
    }

    up_read(&common->filesem);
    down_write(&common->filesem);
    fsg_lun_close(curlun);
    up_write(&common->filesem);
    down_read(&common->filesem);

    return common->ops && common->ops->post_eject
           ? min(0, common->ops->post_eject(common, curlun,
                                            curlun - common->luns))
           : 0;
}

static USB_INT do_prevent_allow(struct fsg_common* common)
{
    struct fsg_lun*	curlun = common->curlun;
    USB_INT		prevent;

    if (!common->curlun)
    {
        return -EINVAL;
    }
    else if (!common->curlun->removable)
    {
        common->curlun->sense_data = SS_INVALID_COMMAND;
        return -EINVAL;
    }

    prevent = common->cmnd[4] & 0x01;

    if (prevent)
    {
        curlun->sense_data = SS_INVALID_FIELD_IN_CDB;
        return -EINVAL;
    }

    if ((common->cmnd[4] & ~0x01) != 0)  	/* Mask away Prevent */
    {
        curlun->sense_data = SS_INVALID_FIELD_IN_CDB;
        return -EINVAL;
    }

    if (curlun->prevent_medium_removal && !prevent)
    { fsg_lun_fsync_sub(curlun); }

    curlun->prevent_medium_removal = prevent;
    return 0;
}

static USB_INT do_read_format_capacities(struct fsg_common* common,
        struct fsg_buffhd* bh)
{
    struct fsg_lun*	curlun = common->curlun;
    USB_PUINT8 buf = (USB_PUINT8) bh->buf;

    buf[0] = buf[1] = buf[2] = 0;
    buf[3] = 8;	/* Only the Current/Maximum Capacity Descriptor */
    buf += 4;

    put_unaligned_be32(curlun->num_sectors, &buf[0]);
    /* Number of blocks */
    put_unaligned_be32(curlun->blksize, &buf[4]);/* Block length */
    buf[4] = 0x02;				/* Current capacity */
    return 12;
}

static USB_INT do_mode_select(struct fsg_common* common, struct fsg_buffhd* bh)
{
    struct fsg_lun*	curlun = common->curlun;

    /* We don't support MODE SELECT */
    if (curlun)
    { curlun->sense_data = SS_INVALID_COMMAND; }

    return -EINVAL;
}


/*-------------------------------------------------------------------------*/

static USB_INT halt_bulk_in_endpoint(struct fsg_dev* fsg)
{
    USB_INT	rc;

    rc = fsg_set_halt(fsg, fsg->bulk_in);

    if (rc == -EAGAIN)
    { DBG_E(MBB_MASS, "delayed bulk-in endpoint halt\n"); }

    while (rc != 0)
    {
        if (rc != -EAGAIN)
        {
            DBG_W(MBB_MASS, "usb_ep_set_halt -> %d\n", rc);
            rc = 0;
            break;
        }

        /* Wait for a short time and then try again */
        if (msleep_interruptible(100) != 0)
        { return -EINTR; }

        rc = usb_ep_set_halt(fsg->bulk_in);
    }

    return rc;
}

static USB_INT wedge_bulk_in_endpoint(struct fsg_dev* fsg)
{
    USB_INT	rc;
    struct mass_storage_function_config_data* configdata = fsg->common->private_data;
    struct fsg_state_count* fsg_state_for_common = &configdata->fsg_state;

    DBG_T(MBB_MASS, "bulk-in set wedge\n");
    rc = usb_ep_set_wedge(fsg->bulk_in);

    if (rc == -EAGAIN)
    { DBG_I(MBB_MASS, "delayed bulk-in endpoint wedge\n"); }

    while (rc != 0)
    {
        if (rc != -EAGAIN)
        {
            DBG_W(MBB_MASS, "usb_ep_set_wedge -> %d\n", rc);
            rc = 0;
            break;
        }

        /* Wait for a short time and then try again */
        if (msleep_interruptible(100) != 0)
        { return -EINTR; }

        rc = usb_ep_set_wedge(fsg->bulk_in);
    }

    fsg_state_for_common->bulk_in_set_wedge++;
    return rc;
}

static USB_INT throw_away_data(struct fsg_common* common)
{
    struct fsg_buffhd*	bh;
    USB_UINT32			amount;
    USB_INT			rc;
    struct mass_storage_function_config_data* configdata = common->private_data;
    struct fsg_state_count* fsg_state_for_common = &configdata->fsg_state;

    for (bh = common->next_buffhd_to_drain;
         bh->state != BUF_STATE_EMPTY || common->usb_amount_left > 0;
         bh = common->next_buffhd_to_drain)
    {

        /* Throw away the data in a filled buffer */
        if (bh->state == BUF_STATE_FULL)
        {
            smp_rmb();
            bh->state = BUF_STATE_EMPTY;
            common->next_buffhd_to_drain = bh->next;

            /* A short packet or an error ends everything */
            if (bh->outreq->actual < bh->bulk_out_intended_length ||
                bh->outreq->status != 0)
            {
                raise_exception(common,
                                FSG_STATE_ABORT_BULK_OUT);
                fsg_state_for_common->throw_away_data_err++;
                return -EINTR;
            }

            continue;
        }

        /* Try to submit another request if we need one */
        bh = common->next_buffhd_to_fill;

        if (bh->state == BUF_STATE_EMPTY
            && common->usb_amount_left > 0)
        {
            amount = min(common->usb_amount_left, FSG_BUFLEN);

            /*
             * Except at the end of the transfer, amount will be
             * equal to the buffer size, which is divisible by
             * the bulk-out maxpacket size.
             */
            set_bulk_out_req_length(common, bh, amount);

            if (!start_out_transfer(common, bh))
                /* Dunno what to do if common->fsg is NULL */
            { return -EIO; }

            common->next_buffhd_to_fill = bh->next;
            common->usb_amount_left -= amount;
            continue;
        }

        /* Otherwise wait for something to happen */
        rc = sleep_thread(common);

        if (rc)
        {
            fsg_state_for_common->signal_pending++;
            fsg_state_for_common->signal_pending_line = __LINE__;
            return rc;
        }
    }

    return 0;
}
static int pad_with_zeros(struct fsg_dev* fsg)
{
    struct fsg_buffhd*	bh = fsg->common->next_buffhd_to_fill;
    u32			nkeep = bh->inreq->length;
    u32			nsend;
    int			rc;
    bh->state = BUF_STATE_EMPTY;		/* For the first iteration */
    fsg->common->usb_amount_left = nkeep + fsg->common->residue;

    while (fsg->common->usb_amount_left > 0)
    {

        /* Wait for the next buffer to be free */
        while (bh->state != BUF_STATE_EMPTY)
        {
            rc = sleep_thread(fsg->common);

            if (rc)
            { return rc; }
        }

        nsend = min(fsg->common->usb_amount_left, FSG_BUFLEN);
        memset(bh->buf + nkeep, 0, nsend - nkeep);
        bh->inreq->length = nsend;
        bh->inreq->zero = 0;
        start_transfer(fsg, fsg->bulk_in, bh->inreq,
                       &bh->inreq_busy, &bh->state);
        bh = fsg->common->next_buffhd_to_fill = bh->next;
        fsg->common->usb_amount_left -= nsend;
        nkeep = 0;
    }

    return 0;
}

static USB_INT finish_reply(struct fsg_common* common)
{
    struct fsg_buffhd*	bh = common->next_buffhd_to_fill;
    USB_INT			rc = 0;
    struct mass_storage_function_config_data* configdata = common->private_data;
    struct fsg_state_count* fsg_state_for_common = &configdata->fsg_state;

    switch (common->data_dir)
    {
        case DATA_DIR_NONE:
            break;			/* Nothing to send */

            /*
             * If we don't know whether the host wants to read or write,
             * this must be CB or CBI with an unknown command.  We mustn't
             * try to send or receive any data.  So stall both bulk pipes
             * if we can and wait for a reset.
             */
        case DATA_DIR_UNKNOWN:
            if (!common->can_stall)
            {
                /* Nothing */
            }
            else if (fsg_is_set(common))
            {
                fsg_set_halt(common->fsg, common->fsg->bulk_out);
                rc = halt_bulk_in_endpoint(common->fsg);
            }
            else
            {
                /* Don't know what to do if common->fsg is NULL */
                rc = -EIO;
            }

            break;

            /* All but the last buffer of data must have already been sent */
        case DATA_DIR_TO_HOST:
            if (0 == common->data_size )
            {
                /* Nothing to send */
            }
            /* Don't know what to do if common->fsg is NULL */
            else if (!fsg_is_set(common))
            {
                rc = -EIO;

                /* If there's no residue, simply send the last buffer */
            }
            /* If there is a cmd we need to return,but wo don't know how to return,
             * we return zero.
             */
            else if ( 1 == common->cmnd_return_zero)
            {
                common->cmnd_return_zero = 0;
                rc = pad_with_zeros(common->fsg);
            }
            else if ( 0 == common->residue )
            {
                bh->inreq->zero = 0;

                if (!start_in_transfer(common, bh))
                { return -EIO; }

                common->next_buffhd_to_fill = bh->next;

                /*
                 * For Bulk-only, mark the end of the data with a short
                 * packet.  If we are allowed to stall, halt the bulk-in
                 * endpoint.  (Note: This violates the Bulk-Only Transport
                 * specification, which requires us to pad the data if we
                 * don't halt the endpoint.  Presumably nobody will mind.)
                 */
            }
            else
            {
                bh->inreq->zero = 1;

                if (!start_in_transfer(common, bh))
                { rc = -EIO; }

                common->next_buffhd_to_fill = bh->next;

                if (common->can_stall && !bh->inreq->length)
                { rc = halt_bulk_in_endpoint(common->fsg); }
            }

            break;

            /*
             * We have processed all we want from the data the host has sent.
             * There may still be outstanding bulk-out requests.
             */
        case DATA_DIR_FROM_HOST:
            if (common->residue == 0)
            {
                /* Nothing to receive */

                /* Did the host stop sending unexpectedly early? */
            }
            else if (common->short_packet_received)
            {
                raise_exception(common, FSG_STATE_ABORT_BULK_OUT);
                rc = -EINTR;
                fsg_state_for_common->short_packet_received++;
                /*
                 * We haven't processed all the incoming data.  Even though
                 * we may be allowed to stall, doing so would cause a race.
                 * The controller may already have ACK'ed all the remaining
                 * bulk-out packets, in which case the host wouldn't see a
                 * STALL.  Not realizing the endpoint was halted, it wouldn't
                 * clear the halt -- leading to problems later on.
                 */
#if 0
            }
            else if (common->can_stall)
            {
                if (fsg_is_set(common))
                    fsg_set_halt(common->fsg,
                                 common->fsg->bulk_out);

                raise_exception(common, FSG_STATE_ABORT_BULK_OUT);
                rc = -EINTR;
#endif

                /*
                 * We can't stall.  Read in the excess data and throw it
                 * all away.
                 */
            }
            else
            {
                rc = throw_away_data(common);
            }

            break;
    }

    return rc;
}

static USB_INT send_status(struct fsg_common* common)
{
    struct fsg_lun*		curlun = common->curlun;
    struct fsg_buffhd*	bh;
    struct bulk_cs_wrap*	csw;
    USB_INT			rc;
    USB_UINT8			status = US_BULK_STAT_OK;
    USB_UINT32			sd;
    struct mass_storage_function_config_data* configdata = common->private_data;
    struct fsg_state_count* fsg_state_for_common = &configdata->fsg_state;

    /* Wait for the next buffer to become available */
    bh = common->next_buffhd_to_fill;

    while (bh->state != BUF_STATE_EMPTY)
    {
        rc = sleep_thread(common);

        if (rc)
        {
            fsg_state_for_common->signal_pending++;
            fsg_state_for_common->signal_pending_line = __LINE__;
            return rc;
        }
    }

    if (curlun)
    {
        sd = curlun->sense_data;
    }
    else if (common->bad_lun_okay)
    { sd = SS_NO_SENSE; }
    else
    { sd = SS_LOGICAL_UNIT_NOT_SUPPORTED; }

    if (common->phase_error)
    {
        /*DBG(common, "sending phase-error status\n");*/
        status = US_BULK_STAT_PHASE;
        fsg_state_for_common->phase_error++;
    }
    else if (sd != SS_NO_SENSE)
    {
        /*DBG(common, "sending command-failure status\n");*/
        status = US_BULK_STAT_FAIL;
        fsg_state_for_common->command_failure++;
    }

    /* Store and send the Bulk-only CSW */
    csw = (USB_PVOID)bh->buf;

    csw->Signature = cpu_to_le32(US_BULK_CS_SIGN);
    csw->Tag = common->tag;
    csw->Residue = cpu_to_le32(common->residue);
    csw->Status = status;

    bh->inreq->length = US_BULK_CS_WRAP_LEN;
    bh->inreq->zero = 0;

    if (!start_in_transfer(common, bh))
        /* Don't know what to do if common->fsg is NULL */
    { return -EIO; }

    common->next_buffhd_to_fill = bh->next;
    return 0;
}


/*-------------------------------------------------------------------------*/

/*
 * Check whether the command is properly formed and whether its data size
 * and direction agree with the values we already have.
 */

static USB_INT check_command(struct fsg_common* common, USB_INT cmnd_size,
                             enum data_direction data_dir, USB_UINT mask,
                             USB_INT needs_medium, USB_PCHAR name)
{
    USB_INT			lun = common->cmnd[1] >> 5;
    static const USB_CHAR	dirletter[4] = {'u', 'o', 'i', 'n'};
    USB_CHAR			hdlen[MAX_HDLEN];
    struct fsg_lun*		curlun;
    struct mass_storage_function_config_data* configdata = common->private_data;
    struct fsg_state_count* fsg_state_for_common = &configdata->fsg_state;

    hdlen[0] = 0;

    if (common->data_dir != DATA_DIR_UNKNOWN)
        snprintf(hdlen, MAX_HDLEN, "H%c=%u", dirletter[(USB_INT) common->data_dir],
                 common->data_size);

    /*
    VDBG(common, "SCSI command: %s;  Dc=%d, D%c=%u;  Hc=%d%s\n",
         name, cmnd_size, dirletter[(USB_INT) data_dir],
         common->data_size_from_cmnd, common->cmnd_size, hdlen);
    */
    /*
     * We can't reply at all until we know the correct data direction
     * and size.
     */
    if (common->data_size_from_cmnd == 0)
    { data_dir = DATA_DIR_NONE; }

    if (common->data_size < common->data_size_from_cmnd)
    {
        /*
         * Host data size < Device data size is a phase error.
         * Carry out the command, but only transfer as much as
         * we are allowed.
         */
        common->data_size_from_cmnd = common->data_size;
        common->phase_error = 1;
    }

    common->residue = common->data_size;
    common->usb_amount_left = common->data_size;

    /* Conflicting data directions is a phase error */
    if (common->data_dir != data_dir && common->data_size_from_cmnd > 0)
    {
        common->phase_error = 1;
        return -EINVAL;
    }

    /* Verify the length of the command itself */
    if (cmnd_size != common->cmnd_size)
    {

        /*
         * Special case workaround: There are plenty of buggy SCSI
         * implementations. Many have issues with cbw->Length
         * field passing a wrong command size. For those cases we
         * always try to work around the problem by using the length
         * sent by the host side provided it is at least as large
         * as the correct command length.
         * Examples of such cases would be MS-Windows, which issues
         * REQUEST SENSE with cbw->Length == 12 where it should
         * be 6, and xbox360 issuing INQUIRY, TEST UNIT READY and
         * REQUEST SENSE with cbw->Length == 10 where it should
         * be 6 as well.
         */
        if (cmnd_size <= common->cmnd_size)
        {
            /*
            DBG(common, "%s is buggy! Expected length %d "
                "but we got %d\n", name,
                cmnd_size, common->cmnd_size);
            */
            cmnd_size = common->cmnd_size;
            DBG_I(MBB_MASS, "cmnd_size %d\n", cmnd_size);
        }
        else
        {
            common->phase_error = 1;
            return -EINVAL;
        }
    }

    /* Check that the LUN values are consistent */
    if (common->lun != lun)
    {
        /*
        DBG(common, "using LUN %d from CBW, not LUN %d from CDB\n",
            common->lun, lun);
        */
        fsg_state_for_common->lun_values_not_consistent++;
    }

    /* Check the LUN */
    curlun = common->curlun;

    if (curlun)
    {
        if (common->cmnd[0] != REQUEST_SENSE)
        {
            curlun->sense_data = SS_NO_SENSE;
            curlun->sense_data_info = 0;
            curlun->info_valid = 0;
        }
    }
    else
    {
        common->bad_lun_okay = 0;

        /*
         * INQUIRY and REQUEST SENSE commands are explicitly allowed
         * to use unsupported LUNs; all others may not.
         */
        if (common->cmnd[0] != INQUIRY &&
            common->cmnd[0] != REQUEST_SENSE)
        {
            DBG_E(MBB_MASS, "unsupported LUN %d\n", common->lun);
            return -EINVAL;
        }
    }

    /*
     * If a unit attention condition exists, only INQUIRY and
     * REQUEST SENSE commands are allowed; anything else must fail.
     */
    if (curlun && curlun->unit_attention_data != SS_NO_SENSE &&
        common->cmnd[0] != INQUIRY &&
        common->cmnd[0] != REQUEST_SENSE)
    {
        curlun->sense_data = curlun->unit_attention_data;
        curlun->unit_attention_data = SS_NO_SENSE;
        return -EINVAL;
    }

    /* Check that only command bytes listed in the mask are non-zero */
    common->cmnd[1] &= 0x1f;			/* Mask away the LUN */

    /*
    for (i = 1; i < cmnd_size; ++i) {
    	if (common->cmnd[i] && !(mask & (1 << i))) {
    		if (curlun)
    			curlun->sense_data = SS_INVALID_FIELD_IN_CDB;
    		return -EINVAL;
    	}
    }*/
    /* If the medium isn't mounted and the command needs to access
     * it, return an error. */
    if (curlun && !fsg_lun_is_open(curlun) && needs_medium)
    {
        curlun->sense_data = SS_MEDIUM_NOT_PRESENT;
        return -EINVAL;
    }

    return 0;
}

/* wrapper of check_command for data size in blocks handling */
static USB_INT check_command_size_in_blocks(struct fsg_common* common,
        USB_INT cmnd_size, enum data_direction data_dir,
        USB_UINT mask, USB_INT needs_medium, USB_PCHAR name)
{
    if (common->curlun)
    { common->data_size_from_cmnd <<= common->curlun->blkbits; }

    return check_command(common, cmnd_size, data_dir,
                         mask, needs_medium, name);
}

static USB_INT do_scsi_command(struct fsg_common* common)
{
    struct fsg_buffhd*	bh;
    USB_INT			rc;
    USB_INT			reply = -EINVAL;
    USB_INT			i;
    static USB_CHAR		unknown[MAX_COMMAND_SIZE];
    struct mass_storage_function_config_data* configdata = common->private_data;
    struct fsg_state_count* fsg_state_for_common = &configdata->fsg_state;

    /*dump_cdb(common);*/

    /* Wait for the next buffer to become available for data or status */
    bh = common->next_buffhd_to_fill;
    common->next_buffhd_to_drain = bh;

    while (bh->state != BUF_STATE_EMPTY)
    {
        rc = sleep_thread(common);

        if (rc)
        {
            fsg_state_for_common->signal_pending++;
            fsg_state_for_common->signal_pending_line = __LINE__;
            return rc;
        }
    }

    common->phase_error = 0;
    common->short_packet_received = 0;

    down_read(&common->filesem);	/* We're using the backing file */
    /*存在scsi交互*/
    g_scsi_stat = 1;

    switch (common->cmnd[0])
    {

        case INQUIRY:

            /*(sizeof(common->inquiry_string)-1)为减去字符串结束标志*/
            if (common->cmnd[4] > ((sizeof(common->inquiry_string) - 1) + 8))
            {
                common->cmnd[4] = (sizeof(common->inquiry_string) - 1) + 8;
            }

            /*bit 7 6 5 means lun*/
            common->cmnd[1] &= 0xe0;
            /*Page code*/
            common->cmnd[2] = 0;
            /*Reserved*/
            common->cmnd[3] = 0;

            common->data_size = common->cmnd[4];
            common->data_size_from_cmnd = common->cmnd[4];

            reply = check_command(common, 6, DATA_DIR_TO_HOST,
                                  (0x1f << 1), 0,
                                  "INQUIRY");

            if (reply == 0)
            { reply = do_inquiry(common, bh); }

            break;

        case MODE_SELECT:
            common->data_size_from_cmnd = common->cmnd[4];
            reply = check_command(common, 6, DATA_DIR_FROM_HOST,
                                  (1 << 1) | (1 << 4), 0,
                                  "MODE SELECT(6)");

            if (reply == 0)
            { reply = do_mode_select(common, bh); }

            break;

        case MODE_SELECT_10:
            common->data_size_from_cmnd =
                get_unaligned_be16(&common->cmnd[7]);
            reply = check_command(common, 10, DATA_DIR_FROM_HOST,
                                  (1 << 1) | (3 << 7), 0,
                                  "MODE SELECT(10)");

            if (reply == 0)
            { reply = do_mode_select(common, bh); }

            break;

        case MODE_SENSE:
            common->data_size_from_cmnd = common->cmnd[4];
            reply = check_command(common, 6, DATA_DIR_TO_HOST,
                                  (1 << 1) | (1 << 2) | (1 << 4), 0,
                                  "MODE SENSE(6)");

            if (reply == 0)
            { reply = do_mode_sense(common, bh); }

            break;

        case MODE_SENSE_10:
            common->data_size_from_cmnd =
                get_unaligned_be16(&common->cmnd[7]);
            reply = check_command(common, 10, DATA_DIR_TO_HOST,
                                  (1 << 1) | (1 << 2) | (3 << 7), 0,
                                  "MODE SENSE(10)");

            if (reply == 0)
            { reply = do_mode_sense(common, bh); }

            break;

        case ALLOW_MEDIUM_REMOVAL:
            common->data_size_from_cmnd = 0;
            reply = check_command(common, 6, DATA_DIR_NONE,
                                  (1 << 4), 0,
                                  "PREVENT-ALLOW MEDIUM REMOVAL");

            if (reply == 0)
            { reply = do_prevent_allow(common); }

            break;

        case READ_6:
            i = common->cmnd[4];
            common->data_size_from_cmnd = (i == 0) ? 256 : i;
            reply = check_command_size_in_blocks(common, 6,
                                                 DATA_DIR_TO_HOST,
                                                 (7 << 1) | (1 << 4), 1,
                                                 "READ(6)");

            if (reply == 0)
            { reply = do_read(common); }

            break;

        case READ_10:
            common->data_size_from_cmnd =
                get_unaligned_be16(&common->cmnd[7]);
            reply = check_command_size_in_blocks(common, 10,
                                                 DATA_DIR_TO_HOST,
                                                 (1 << 1) | (0xf << 2) | (3 << 7), 1,
                                                 "READ(10)");

            if (reply == 0)
            { reply = do_read(common); }

            break;

        case READ_12:
            common->data_size_from_cmnd =
                get_unaligned_be32(&common->cmnd[6]);
            reply = check_command_size_in_blocks(common, 12,
                                                 DATA_DIR_TO_HOST,
                                                 (1 << 1) | (0xf << 2) | (0xf << 6), 1,
                                                 "READ(12)");

            if (reply == 0)
            { reply = do_read(common); }

            break;

        case READ_CAPACITY:
            common->data_size_from_cmnd = 8;
            reply = check_command(common, 10, DATA_DIR_TO_HOST,
                                  (0xf << 2) | (1 << 8), 1,
                                  "READ CAPACITY");

            if (reply == 0)
            { reply = do_read_capacity(common, bh); }

            break;

        case READ_HEADER:
            if (!common->curlun || !common->curlun->cdrom)
            { goto unknown_cmnd; }

            common->data_size_from_cmnd =
                get_unaligned_be16(&common->cmnd[7]);
            reply = check_command(common, 10, DATA_DIR_TO_HOST,
                                  (3 << 7) | (0x1f << 1), 1,
                                  "READ HEADER");

            if (reply == 0)
            { reply = do_read_header(common, bh); }

            break;

        case READ_TOC:
            if (!common->curlun || !common->curlun->cdrom)
            { goto unknown_cmnd; }

            common->data_size_from_cmnd =
                get_unaligned_be16(&common->cmnd[7]);
            /* 根据SCSI协议，Read Toc 命令的有效字段是1、6、7、8、9，
            mask掩码的值应该是1111000010 */
            /* Code Checker结果说明，此处修改的mask掩码的值是通过十六
            进制数据进行移位获取的，并且为了和原代码格式保持一致
            ，Code Checker警告不处理 */
            reply = check_command(common, 10, DATA_DIR_TO_HOST,
                                  (3 << 1) | (0xF << 6), 1,
                                  "READ TOC");

            if (reply == 0)
            { reply = do_read_toc(common, bh); }

            break;

        case READ_FORMAT_CAPACITIES:
            common->data_size_from_cmnd =
                get_unaligned_be16(&common->cmnd[7]);
            reply = check_command(common, 10, DATA_DIR_TO_HOST,
                                  (3 << 7), 1,
                                  "READ FORMAT CAPACITIES");

            if (reply == 0)
            { reply = do_read_format_capacities(common, bh); }

            break;

        case REQUEST_SENSE:
            common->data_size_from_cmnd = common->cmnd[4];
            reply = check_command(common, 6, DATA_DIR_TO_HOST,
                                  (1 << 4), 0,
                                  "REQUEST SENSE");

            if (reply == 0)
            { reply = do_request_sense(common, bh); }

            break;

        case START_STOP:
            common->data_size_from_cmnd = 0;
            reply = check_command(common, 6, DATA_DIR_NONE,
                                  (1 << 1) | (1 << 4), 0,
                                  "START-STOP UNIT");

            if (reply == 0)
            { reply = do_start_stop(common); }

            break;

        case SYNCHRONIZE_CACHE:
            common->data_size_from_cmnd = 0;
            reply = check_command(common, 10, DATA_DIR_NONE,
                                  (0xf << 2) | (3 << 7), 1,
                                  "SYNCHRONIZE CACHE");

            if (reply == 0)
            { reply = do_synchronize_cache(common); }

            break;

        case TEST_UNIT_READY:
            common->data_size_from_cmnd = 0;
            reply = check_command(common, 6, DATA_DIR_NONE,
                                  0, 1,
                                  "TEST UNIT READY");

            if (0 == reply)
            {
                reply = do_test_unit(common, bh);
            }

            break;

            /*
             * Although optional, this command is used by MS-Windows.  We
             * support a minimal version: BytChk must be 0.
             */
        case VERIFY:
            common->data_size_from_cmnd = 0;
            reply = check_command(common, 10, DATA_DIR_NONE,
                                  (1 << 1) | (0xf << 2) | (3 << 7), 1,
                                  "VERIFY");

            if (reply == 0)
            { reply = do_verify(common); }

            break;

        case WRITE_6:
            i = common->cmnd[4];
            common->data_size_from_cmnd = (i == 0) ? 256 : i;
            reply = check_command_size_in_blocks(common, 6,
                                                 DATA_DIR_FROM_HOST,
                                                 (7 << 1) | (1 << 4), 1,
                                                 "WRITE(6)");

            if (reply == 0)
            { reply = do_write(common); }

            break;

        case WRITE_10:
            common->data_size_from_cmnd =
                get_unaligned_be16(&common->cmnd[7]);
            reply = check_command_size_in_blocks(common, 10,
                                                 DATA_DIR_FROM_HOST,
                                                 (1 << 1) | (0xf << 2) | (3 << 7), 1,
                                                 "WRITE(10)");

            if (reply == 0)
            { reply = do_write(common); }

            break;

        case SC_READ_DISC_INFORMATION:
            common->data_size_from_cmnd = 0;
            reply = check_command(common, 10, DATA_DIR_FROM_HOST,
                                  0, 0,
                                  "XPWRITE(10)");

            if (0 == reply)
            {
                reply = do_XPWrite(common, bh);
            }

            break;

        case WRITE_12:
            common->data_size_from_cmnd =
                get_unaligned_be32(&common->cmnd[6]);
            reply = check_command_size_in_blocks(common, 12,
                                                 DATA_DIR_FROM_HOST,
                                                 (1 << 1) | (0xf << 2) | (0xf << 6), 1,
                                                 "WRITE(12)");

            if (reply == 0)
            { reply = do_write(common); }

            break;

            /*
             * Some mandatory commands that we recognize but don't implement.
             * They don't mean much in this setting.  It's left as an exercise
             * for anyone interested to implement RESERVE and RELEASE in terms
             * of Posix locks.
             */
        case GET_CONFIGURATION_SCSI:
            common->data_size_from_cmnd = get_unaligned_be16(&common->cmnd[USB_NUM_7]);
            reply = do_get_configuration(common, bh);
            break;

        case SC_REWIND_11:
            DBG_T(MBB_MASS, "do rewind receive!\n");
            reply = do_rewind(common, bh);
            break;

        case FORMAT_UNIT:
        case RELEASE:
        case RESERVE:
        case SEND_DIAGNOSTIC:

            /* Fall through */

        default:
        unknown_cmnd:
            common->data_size_from_cmnd = 0;
            snprintf(unknown, MAX_COMMAND_SIZE, "Unknown x%02x", common->cmnd[0]);
            reply = check_command(common, common->cmnd_size,
                                  DATA_DIR_UNKNOWN, ~0, 0, unknown);

            if (reply == 0)
            {
                common->curlun->sense_data = SS_INVALID_COMMAND;
                reply = -EINVAL;
            }

            fsg_state_for_common->unknown_cbw++;
            break;
    }

    up_read(&common->filesem);

    if (reply == -EINTR || signal_pending(current))
    {
        if (reply != -EINTR)
        {
            fsg_state_for_common->signal_pending++;
            fsg_state_for_common->signal_pending_line = __LINE__;
        }

        return -EINTR;
    }

    /* Set up the single reply buffer for finish_reply() */
    if (reply == -EINVAL)
    { reply = 0; }		/* Error reply length */

    if (reply >= 0 && common->data_dir == DATA_DIR_TO_HOST)
    {
        reply = min((USB_UINT)reply, common->data_size_from_cmnd);
        bh->inreq->length = reply;
        bh->state = BUF_STATE_FULL;
        common->residue -= reply;
    }				/* Otherwise it's already set */

    return 0;
}


/*-------------------------------------------------------------------------*/

static USB_INT received_cbw(struct fsg_dev* fsg, struct fsg_buffhd* bh)
{
    struct usb_request*	req = bh->outreq;
    struct bulk_cb_wrap*	cbw = req->buf;
    struct fsg_common*	common = fsg->common;
    struct mass_storage_function_config_data* configdata = fsg->common->private_data;
    struct fsg_state_count* fsg_state_for_common = &configdata->fsg_state;

    /* Was this a real packet?  Should it be ignored? */
    if (req->status || test_bit(IGNORE_BULK_OUT, &fsg->atomic_bitflags))
    { return -EINVAL; }

    /* Is the CBW valid? */
    if (req->actual != US_BULK_CB_WRAP_LEN ||
        cbw->Signature != cpu_to_le32(
            US_BULK_CB_SIGN))
    {
        DBG_E(MBB_MASS, "invalid CBW: len %u sig 0x%x\n",
              req->actual,
              le32_to_cpu(cbw->Signature));

        fsg_state_for_common->invalid_cbw++;

        /*
         * The Bulk-only spec says we MUST stall the IN endpoint
         * (6.6.1), so it's unavoidable.  It also says we must
         * retain this state until the next reset, but there's
         * no way to tell the controller driver it should ignore
         * Clear-Feature(HALT) requests.
         *
         * We aren't required to halt the OUT endpoint; instead
         * we can simply accept and discard any data received
         * until the next reset.
         */
        wedge_bulk_in_endpoint(fsg);
        set_bit(IGNORE_BULK_OUT, &fsg->atomic_bitflags);
        return -EINVAL;
    }

    /* Is the CBW meaningful? */
    if (cbw->Lun >= FSG_MAX_LUNS || cbw->Flags & ~US_BULK_FLAG_IN ||
        cbw->Length < 0 || cbw->Length > MAX_COMMAND_SIZE)
    {
        DBG_E(MBB_MASS, "non-meaningful CBW: lun = %u, flags = 0x%x, "
              "cmdlen %u\n",
              cbw->Lun, cbw->Flags, cbw->Length);

        fsg_state_for_common->non_meaningful_cbw++;

        /*
         * We can do anything we want here, so let's stall the
         * bulk pipes if we are allowed to.
         */
        if (common->can_stall)
        {
            fsg_set_halt(fsg, fsg->bulk_out);
            halt_bulk_in_endpoint(fsg);
        }

        return -EINVAL;
    }

    /* Save the command for later */
    common->cmnd_size = MAX_COMMAND_SIZE;
    memcpy(common->cmnd, cbw->CDB, common->cmnd_size);

    if (cbw->Flags & US_BULK_FLAG_IN)
    { common->data_dir = DATA_DIR_TO_HOST; }
    else
    { common->data_dir = DATA_DIR_FROM_HOST; }

    common->data_size = le32_to_cpu(cbw->DataTransferLength);

    if (common->data_size == 0)
    { common->data_dir = DATA_DIR_NONE; }

    common->lun = cbw->Lun;

    if (common->lun < common->nluns)
    { common->curlun = &common->luns[common->lun]; }
    else
    { common->curlun = NULL; }

    common->tag = cbw->Tag;
    return 0;
}

static USB_INT get_next_command(struct fsg_common* common)
{
    struct fsg_buffhd*	bh;
    USB_INT			rc = 0;
    struct mass_storage_function_config_data* configdata = common->private_data;
    struct fsg_state_count* fsg_state_for_common = &configdata->fsg_state;
    /* Wait for the next buffer to become available */
    bh = common->next_buffhd_to_fill;

    while (bh->state != BUF_STATE_EMPTY)
    {
        rc = sleep_thread(common);

        if (rc)
        {
            fsg_state_for_common->signal_pending++;
            fsg_state_for_common->signal_pending_line = __LINE__;
            return rc;
        }
    }

    /* Queue a request to read a Bulk-only CBW */
    set_bulk_out_req_length(common, bh, US_BULK_CB_WRAP_LEN);

    if (!start_out_transfer(common, bh))
        /* Don't know what to do if common->fsg is NULL */
    { return -EIO; }

    /*
     * We will drain the buffer in software, which means we
     * can reuse it for the next filling.  No need to advance
     * next_buffhd_to_fill.
     */

    /* Wait for the CBW to arrive */
    while (bh->state != BUF_STATE_FULL)
    {
        rc = sleep_thread(common);

        if (rc)
        {
            fsg_state_for_common->signal_pending++;
            fsg_state_for_common->signal_pending_line = __LINE__;
            return rc;
        }
    }

    smp_rmb();
    rc = fsg_is_set(common) ? received_cbw(common->fsg, bh) : -EIO;
    bh->state = BUF_STATE_EMPTY;
    return rc;
}


/*-------------------------------------------------------------------------*/

static USB_INT alloc_request(struct fsg_common* common, struct usb_ep* ep,
                             struct usb_request** preq)
{
    *preq = usb_ep_alloc_request(ep, GFP_ATOMIC);

    if (*preq)
    { return 0; }

    DBG_E(MBB_MASS, "can't allocate request for %s\n", ep->name);
    return -ENOMEM;
}

/* Reset interface setting and re-init endpoint state (toggle etc). */
static USB_INT do_set_interface(struct fsg_common* common, struct fsg_dev* new_fsg)
{
    struct fsg_dev* fsg;
    USB_INT i, rc = 0;

    if (common->running)
    { DBG_I(MBB_MASS, "common->running need reset interface\n"); }

reset:

    /* Deallocate the requests */
    if (common->fsg)
    {
        fsg = common->fsg;

        for (i = 0; i < fsg_num_buffers; ++i)
        {
            struct fsg_buffhd* bh = &common->buffhds[i];

            if (bh->inreq)
            {
                usb_ep_free_request(fsg->bulk_in, bh->inreq);
                bh->inreq = NULL;
            }

            if (bh->outreq)
            {
                usb_ep_free_request(fsg->bulk_out, bh->outreq);
                bh->outreq = NULL;
            }
        }

        /* Disable the endpoints */
        if (fsg->bulk_in_enabled)
        {
            usb_ep_disable(fsg->bulk_in);
            fsg->bulk_in_enabled = 0;
        }

        if (fsg->bulk_out_enabled)
        {
            usb_ep_disable(fsg->bulk_out);
            fsg->bulk_out_enabled = 0;
        }

        common->fsg = NULL;
        wake_up(&common->fsg_wait);
    }

    common->running = 0;

    if (!new_fsg || rc)
    {

        return rc;
    }

    common->fsg = new_fsg;
    fsg = common->fsg;

    /* Enable the endpoints */
    rc = config_ep_by_speed(common->gadget, &(fsg->function), fsg->bulk_in);

    if (rc)
    { goto reset; }

    rc = usb_ep_enable(fsg->bulk_in);

    if (rc)
    { goto reset; }

    fsg->bulk_in->driver_data = common;
    fsg->bulk_in_enabled = 1;

    rc = config_ep_by_speed(common->gadget, &(fsg->function),
                            fsg->bulk_out);

    if (rc)
    { goto reset; }

    rc = usb_ep_enable(fsg->bulk_out);

    if (rc)
    { goto reset; }

    fsg->bulk_out->driver_data = common;
    fsg->bulk_out_enabled = 1;
    common->bulk_out_maxpacket = usb_endpoint_maxp(fsg->bulk_out->desc);
    clear_bit(IGNORE_BULK_OUT, &fsg->atomic_bitflags);

    /* Allocate the requests */
    for (i = 0; i < fsg_num_buffers; ++i)
    {
        struct fsg_buffhd*	bh = &common->buffhds[i];

        rc = alloc_request(common, fsg->bulk_in, &bh->inreq);

        if (rc)
        { goto reset; }

        rc = alloc_request(common, fsg->bulk_out, &bh->outreq);

        if (rc)
        { goto reset; }

        bh->inreq->buf = bh->outreq->buf = bh->buf;
        bh->inreq->context = bh->outreq->context = bh;
        bh->inreq->complete = bulk_in_complete;
        bh->outreq->complete = bulk_out_complete;
    }

    common->running = 1;

    for (i = 0; i < common->nluns; ++i)
    { common->luns[i].unit_attention_data = SS_RESET_OCCURRED; }

    return rc;
}


/****************************** ALT CONFIGS ******************************/

static USB_INT fsg_set_alt(struct usb_function* f, USB_UINT intf, USB_UINT alt)
{
    struct fsg_dev* fsg = fsg_from_func(f);
    fsg->common->new_fsg = fsg;
    raise_exception(fsg->common, FSG_STATE_CONFIG_CHANGE);
    return USB_GADGET_DELAYED_STATUS;
}

static USB_VOID fsg_disable(struct usb_function* f)
{
    struct fsg_dev* fsg = fsg_from_func(f);
    fsg->common->new_fsg = NULL;
    raise_exception(fsg->common, FSG_STATE_CONFIG_CHANGE);
}


/*-------------------------------------------------------------------------*/

static USB_VOID handle_exception(struct fsg_common* common)
{
    siginfo_t		info;
    USB_INT			i;
    struct fsg_buffhd*	bh;
    enum fsg_state		old_state;
    struct fsg_lun*		curlun;
    USB_UINT		exception_req_tag;
    struct mass_storage_function_config_data* configdata = common->private_data;
    struct fsg_state_count* fsg_state_for_common = &configdata->fsg_state;

    /*
     * Clear the existing signals.  Anything but SIGUSR1 is converted
     * into a high-priority EXIT exception.
     */
    for (;;)
    {
        USB_INT sig =
            dequeue_signal_lock(current, &current->blocked, &info);

        if (!sig)
        { break; }

        if (sig != SIGUSR1)
        {
            if (common->state < FSG_STATE_EXIT)
            { DBG_E(MBB_MASS, "Main thread exiting on signal\n"); }

            raise_exception(common, FSG_STATE_EXIT);
        }
    }

    /* Cancel all the pending transfers */
    if (likely(common->fsg))
    {
        for (i = 0; i < fsg_num_buffers; ++i)
        {
            bh = &common->buffhds[i];

            if (bh->inreq_busy)
            { usb_ep_dequeue(common->fsg->bulk_in, bh->inreq); }

            if (bh->outreq_busy)
                usb_ep_dequeue(common->fsg->bulk_out,
                               bh->outreq);
        }

        /* Wait until everything is idle */
        for (;;)
        {
            USB_INT num_active = 0;
            spin_lock_irq(&common->lock);
            for (i = 0; i < fsg_num_buffers; ++i)
            {
                bh = &common->buffhds[i];
                num_active += bh->inreq_busy + bh->outreq_busy;
            }
            spin_unlock_irq(&common->lock);
            if (num_active == 0)
            { break; }

            if (sleep_thread(common))
            { return; }

        }

        /* Clear out the controller's fifos */
        if (common->fsg->bulk_in_enabled)
        { usb_ep_fifo_flush(common->fsg->bulk_in); }

        if (common->fsg->bulk_out_enabled)
        { usb_ep_fifo_flush(common->fsg->bulk_out); }
    }

    /*
     * Reset the I/O buffer states and pointers, the SCSI
     * state, and the exception.  Then invoke the handler.
     */
    spin_lock_irq(&common->lock);

    for (i = 0; i < fsg_num_buffers; ++i)
    {
        bh = &common->buffhds[i];
        bh->state = BUF_STATE_EMPTY;
    }

    common->next_buffhd_to_fill = &common->buffhds[0];
    common->next_buffhd_to_drain = &common->buffhds[0];
    exception_req_tag = common->exception_req_tag;
    old_state = common->state;

    if (old_state == FSG_STATE_ABORT_BULK_OUT)
    { common->state = FSG_STATE_STATUS_PHASE; }

    else
    {
        for (i = 0; i < common->nluns; ++i)
        {
            curlun = &common->luns[i];
            curlun->prevent_medium_removal = 0;
            curlun->sense_data = SS_NO_SENSE;
            curlun->unit_attention_data = SS_NO_SENSE;
            curlun->sense_data_info = 0;
            curlun->info_valid = 0;
        }

        common->state = FSG_STATE_IDLE;
    }

    spin_unlock_irq(&common->lock);

    /* Carry out any extra actions required for the exception */
    switch (old_state)
    {
        case FSG_STATE_ABORT_BULK_OUT:
            send_status(common);
            spin_lock_irq(&common->lock);

            if (common->state == FSG_STATE_STATUS_PHASE)
            { common->state = FSG_STATE_IDLE; }

            spin_unlock_irq(&common->lock);
            break;

        case FSG_STATE_RESET:

            /*
             * In case we were forced against our will to halt a
             * bulk endpoint, clear the halt now.  (The SuperH UDC
             * requires this.)
             */
            if (!fsg_is_set(common))
            { break; }

            if (test_and_clear_bit(IGNORE_BULK_OUT,
                                   &common->fsg->atomic_bitflags))
            { usb_ep_clear_halt(common->fsg->bulk_in); }

            if (common->ep0_req_tag == exception_req_tag)
            {
                if (common->cdev)
                {
                    usb_composite_setup_continue(common->cdev);
                }
                else
                {
                    ep0_queue(common);
                }
            }    /* Complete the status stage */

            /*
             * Technically this should go here, but it would only be
             * a waste of time.  Ditto for the INTERFACE_CHANGE and
             * CONFIG_CHANGE cases.
             */
            /* for (i = 0; i < common->nluns; ++i) */
            /*	common->luns[i].unit_attention_data = */
            /*		SS_RESET_OCCURRED;  */
            break;

        case FSG_STATE_CONFIG_CHANGE:
            do_set_interface(common, common->new_fsg);

            if (common->new_fsg)
            { usb_composite_setup_continue(common->cdev); }

            break;

        case FSG_STATE_EXIT:
        case FSG_STATE_TERMINATED:
            do_set_interface(common, NULL);		/* Free resources */
            spin_lock_irq(&common->lock);
            common->state = FSG_STATE_TERMINATED;	/* Stop the thread */
            fsg_state_for_common->terminated++;
            spin_unlock_irq(&common->lock);
            break;

        case FSG_STATE_INTERFACE_CHANGE:
        case FSG_STATE_DISCONNECT:
        case FSG_STATE_COMMAND_PHASE:
        case FSG_STATE_DATA_PHASE:
        case FSG_STATE_STATUS_PHASE:
        case FSG_STATE_IDLE:
            break;
    }
}


/*-------------------------------------------------------------------------*/

static USB_INT fsg_main_thread(USB_PVOID common_)
{
    struct fsg_common*	common = common_;
    struct mass_storage_function_config_data* configdata = common->private_data;
    struct fsg_state_count* fsg_state_for_common = &configdata->fsg_state;

    /*
     * Allow the thread to be killed by a signal, but set the signal mask
     * to block everything but INT, TERM, KILL, and USR1.
     */
    allow_signal(SIGINT);
    allow_signal(SIGTERM);
    allow_signal(SIGKILL);
    allow_signal(SIGUSR1);

    /* Allow the thread to be frozen */
    set_freezable();

    /*
     * Arrange for userspace references to be interpreted as kernel
     * pointers.  That way we can pass a kernel pointer to a routine
     * that expects a __user pointer and it will work okay.
     */
    set_fs(get_ds());

    /* The main loop */
    while (common->state != FSG_STATE_TERMINATED)
    {
        //DBG_T(MBB_MASS, "main thread\n");
        if (exception_in_progress(common) || signal_pending(current))
        {
            handle_exception(common);
            continue;
        }

        if (!common->running)
        {
            (USB_VOID)sleep_thread(common);
            continue;
        }

        if (get_next_command(common))
        { continue; }

        //DBG_T(MBB_MASS, "get_next_command succ\n");
        spin_lock_irq(&common->lock);

        if (!exception_in_progress(common))
        { common->state = FSG_STATE_DATA_PHASE; }

        fsg_state_for_common->cbw++;
        fsg_state_for_common->data_phase++;
        spin_unlock_irq(&common->lock);

#ifdef USB_SD
        if ( SD_CARD_NOT_PRESENT == g_sd_card_info.sd_card_status
             || SD_WORKMODE_MASS != g_sd_card_info.sd_card_workmode)
        {
            mass_storage_close_usb_sd();
        }

#endif

        if (do_scsi_command(common) || finish_reply(common))
        { continue; }

        spin_lock_irq(&common->lock);

        if (!exception_in_progress(common))
        { common->state = FSG_STATE_STATUS_PHASE; }

        fsg_state_for_common->status_phase++;
        fsg_state_for_common->data++;

        spin_unlock_irq(&common->lock);

        if (send_status(common))
        { continue; }

        spin_lock_irq(&common->lock);

        if (!exception_in_progress(common))
        { common->state = FSG_STATE_IDLE; }

        fsg_state_for_common->idle++;
        fsg_state_for_common->csw++;

        spin_unlock_irq(&common->lock);
    }

    spin_lock_irq(&common->lock);
    common->thread_task = NULL;
    spin_unlock_irq(&common->lock);

    if (!common->ops || !common->ops->thread_exits
        || common->ops->thread_exits(common) < 0)
    {
        struct fsg_lun* curlun = common->luns;
        USB_UINT i = common->nluns;

        down_write(&common->filesem);

        for (; i--; ++curlun)
        {
            if (!fsg_lun_is_open(curlun))
            { continue; }

            fsg_lun_close(curlun);
            curlun->unit_attention_data = SS_MEDIUM_NOT_PRESENT;
        }

        up_write(&common->filesem);
    }

    /* Let fsg_unbind() know the thread has exited */
    complete_and_exit(&common->thread_notifier, 0);
}


static ssize_t fsg_show_state(struct device* dev, struct device_attribute* attr,
                              USB_CHAR* buf)
{
    size_t count = 0;
    struct fsg_common* common = NULL;
    struct mass_storage_function_config_data* configdata = NULL;
    struct fsg_state_count* fsg_state_for_common = NULL;
    if (dev->platform_data)
    {
        configdata = dev->platform_data;
        fsg_state_for_common = &configdata->fsg_state;
        common = configdata->common;
    }
    else
    {
        DBG_E(MBB_MASS, "dev/%p, dev->platform_data/%p\n", dev, dev->platform_data);
        return 0;
    }
    count = snprintf(buf, MAX_SHOW_LEN, "\n");
    count += snprintf(buf, MAX_SHOW_LEN - count, "| |--file storage dump:common for %s\n", dev->kobj.name);

    count += snprintf(buf + count, MAX_SHOW_LEN - count, "| |--bulk in name-------------%s\n",
                      common->fsg ?
                      common->fsg->bulk_in->name : "");
    count += snprintf(buf + count, MAX_SHOW_LEN - count, "| |--bulk out name------------%s\n",
                      common->fsg ?
                      common->fsg->bulk_out->name : "");

    count += snprintf(buf + count, MAX_SHOW_LEN - count, "| |--fsg command phase--------%d\n", fsg_state_for_common->command_phase);
    count += snprintf(buf + count, MAX_SHOW_LEN - count, "| |--fsg data phase-----------%d\n", fsg_state_for_common->data_phase);
    count += snprintf(buf + count, MAX_SHOW_LEN - count, "| |--fsg status phase---------%d\n", fsg_state_for_common->status_phase);
    count += snprintf(buf + count, MAX_SHOW_LEN - count, "| |--fsg idle-----------------%d\n", fsg_state_for_common->idle);
    count += snprintf(buf + count, MAX_SHOW_LEN - count, "| |--fsg abort_bulk_out-------%d\n", fsg_state_for_common->abort_bulk_out);
    count += snprintf(buf + count, MAX_SHOW_LEN - count, "| |--fsg reset----------------%d\n", fsg_state_for_common->reset);
    count += snprintf(buf + count, MAX_SHOW_LEN - count, "| |--fsg interface change-----%d\n", fsg_state_for_common->interface_change);
    count += snprintf(buf + count, MAX_SHOW_LEN - count, "| |--fsg config change--------%d\n", fsg_state_for_common->config_change);
    count += snprintf(buf + count, MAX_SHOW_LEN - count, "| |--fsg disconnect-----------%d\n", fsg_state_for_common->disconnect);
    count += snprintf(buf + count, MAX_SHOW_LEN - count, "| |--fsg exit-----------------%d\n", fsg_state_for_common->exit);
    count += snprintf(buf + count, MAX_SHOW_LEN - count, "| |--fsg terminated-----------%d\n", fsg_state_for_common->terminated);

    count += snprintf(buf + count, MAX_SHOW_LEN - count, "| |--fsg phase error----------%d\n", fsg_state_for_common->phase_error);
    count += snprintf(buf + count, MAX_SHOW_LEN - count, "| |--fsg command failure------%d\n", fsg_state_for_common->command_failure);
    count += snprintf(buf + count, MAX_SHOW_LEN - count, "| |--fsg cbw------------------%d\n", fsg_state_for_common->cbw);
    count += snprintf(buf + count, MAX_SHOW_LEN - count, "| |--fsg invalid cbw----------%d\n", fsg_state_for_common->invalid_cbw);
    count += snprintf(buf + count, MAX_SHOW_LEN - count, "| |--fsg non meaningful cbw---%d\n", fsg_state_for_common->non_meaningful_cbw);
    count += snprintf(buf + count, MAX_SHOW_LEN - count, "| |--fsg lun values not consistent----%d\n", fsg_state_for_common->lun_values_not_consistent);

    count += snprintf(buf + count, MAX_SHOW_LEN - count, "| |--fsg unknown cbw----------%d\n", fsg_state_for_common->unknown_cbw);
    count += snprintf(buf + count, MAX_SHOW_LEN - count, "| |--fsg data-----------------%d\n", fsg_state_for_common->data);
    count += snprintf(buf + count, MAX_SHOW_LEN - count, "| |--fsg csw------------------%d\n", fsg_state_for_common->csw);
    count += snprintf(buf + count, MAX_SHOW_LEN - count, "| |--fsg invalid csw----------%d\n", fsg_state_for_common->invalid_csw);
    count += snprintf(buf + count, MAX_SHOW_LEN - count, "| |--fsg non meaningful csw---%d\n", fsg_state_for_common->non_meaningful_csw);

    count += snprintf(buf + count, MAX_SHOW_LEN - count, "| |--fsg bulk in set halt-----%d\n", fsg_state_for_common->bulk_in_set_halt);
    count += snprintf(buf + count, MAX_SHOW_LEN - count, "| |--fsg bulk out set halt----%d\n", fsg_state_for_common->bulk_out_set_halt);
    count += snprintf(buf + count, MAX_SHOW_LEN - count, "| |--fsg bulk in set wedge----%d\n", fsg_state_for_common->bulk_in_set_wedge);
    count += snprintf(buf + count, MAX_SHOW_LEN - count, "| |--fsg signal pending-------%d\n", fsg_state_for_common->signal_pending);
    count += snprintf(buf + count, MAX_SHOW_LEN - count, "| |--fsg signal_pending_line--%d\n", fsg_state_for_common->signal_pending_line);
    count += snprintf(buf + count, MAX_SHOW_LEN - count, "| |--fsg short packet received----%d\n", fsg_state_for_common->short_packet_received);
    count += snprintf(buf + count, MAX_SHOW_LEN - count, "\n");
    return count;
}

/*************************** DEVICE ATTRIBUTES ***************************/

/* Write permission is checked per LUN in store_*() functions. */
static DEVICE_ATTR(ro, S_IRUGO | S_IWUSR, fsg_show_ro, fsg_store_ro);
static DEVICE_ATTR(nofua, S_IRUGO | S_IWUSR, fsg_show_nofua, fsg_store_nofua);
static DEVICE_ATTR(file, S_IRUGO | S_IWUSR, fsg_show_file, fsg_store_file);
static DEVICE_ATTR(mode, S_IRUGO | S_IWUSR, fsg_show_mode, fsg_store_mode);
static DEVICE_ATTR(fsg_state, S_IRUGO, fsg_show_state, NULL);
/****************************** FSG COMMON ******************************/
#ifdef USB_SD
USB_INT mass_storage_open_usb_sd(USB_VOID)
{
    struct mass_storage_function_config_data* configdata = NULL;
    struct fsg_common* common = NULL;
    struct fsg_lun*     curlun = NULL;
    USB_CHAR    filename[32];
    USB_INT     openlun = 0;
    USB_INT     ret     = -1;
    USB_INT     i       = 0;
    USB_INT     j       = 0;
    USB_INT     idex;

    for (i = 0; i < SUPPORT_MASS_FUNCTION_NUM; i++)
    {
        if (NULL != g_enabled_mass_function_list[i])
        {
            configdata = g_enabled_mass_function_list[i]->config;
            common = configdata->common;
        }
        else
        {
            continue;
        }

        for (j = 0; j < common->nluns; j++)
        {
            curlun = &common->luns[j];

            if ( NULL != curlun && !curlun->cdrom )
            {
                DBG_T(MBB_MASS, "sd_card_workmode %s\n",
                      curlun->mode ? "usb" : "web");
                DBG_T(MBB_MASS, "sd_card_status %s\n",
                      g_sd_card_info.sd_card_status ? "PRESENT" : "NOT_PRESENT");
            }

            if ( NULL != curlun && !curlun->cdrom
                 && SD_WORKMODE_MASS == curlun->mode
                 && SD_CARD_PRESENT == g_sd_card_info.sd_card_status)
            {
                idex =  usb_get_mmc_dev_idex();

                if (idex > -1)
                {
                    snprintf(filename, 30, "/dev/block/mmcblk%d", idex);
                }
                else
                {
                    DBG_E(MBB_MASS, "usb_get_mmc_dev_idex return err\n");
                    snprintf(filename, 30, SDCARD_MNT_DIR);
                }

                if (fsg_lun_is_open(curlun))
                {
                    DBG_T(MBB_MASS, "%s already open file: %s\n",
                          curlun->dev.kobj.name, filename);
                    ret = 0;
                }
                else
                {
                    openlun = fsg_lun_open(curlun, filename);

                    if ( 0 == openlun )
                    {
                        curlun->unit_attention_data =
                            SS_NOT_READY_TO_READY_TRANSITION;
                    }
                    else
                    {
                        DBG_T(MBB_MASS, "usb open sd\n");
                        ret = 0;
                    }
                }
            }
        }
    }

    return ret;
}

USB_INT mass_storage_close_usb_sd(USB_VOID)
{
    struct fsg_lun* curlun = NULL;
    USB_INT ret = -1;
    USB_INT i = 0;
    USB_INT j = 0;
    struct fsg_common* common = NULL;

    for (i = 0; i < SUPPORT_MASS_FUNCTION_NUM; i++)
    {
        struct mass_storage_function_config_data* configdata = NULL;

        if (NULL != g_enabled_mass_function_list[i])
        {
            configdata = g_enabled_mass_function_list[i]->config;
            common = configdata->common;
        }
        else
        {
            continue;
        }

        for (j = 0; j < common->nluns; j++)
        {
            curlun = &common->luns[j];

            if (NULL != curlun && !curlun->cdrom)
            {
                if (fsg_lun_is_open(curlun))
                {
                    fsg_lun_close(curlun);
                    DBG_T(MBB_MASS, "usb close sd\n");
                    ret = 0;
                    return ret;
                }
            }
        }
    }

    return ret;
}

USB_INT mass_storage_usb_sd_is_open(USB_VOID)
{
    struct fsg_lun* curlun = NULL;
    USB_INT ret = -1;
    USB_INT i = 0;
    USB_INT j = 0;
    struct fsg_common* common = NULL;

    DBG_T(MBB_MASS, "mass_storage_usb_sd_is_open ?\n");
    DBG_T(MBB_MASS, "sd_card_status %s\n", g_sd_card_info.sd_card_status ? "PRESENT " : "NOT_PRESENT ");

    for (i = 0; i < SUPPORT_MASS_FUNCTION_NUM; i++)
    {
        struct mass_storage_function_config_data* configdata = NULL;

        if (NULL != g_enabled_mass_function_list[i])
        {
            configdata = g_enabled_mass_function_list[i]->config;
            common = configdata->common;
        }
        else
        {
            continue;
        }

        for (j = 0; j < common->nluns; j++)
        {
            curlun = &common->luns[j];

            if ( NULL != curlun && !curlun->cdrom )
            {
                if (fsg_lun_is_open(curlun))
                {
                    DBG_T(MBB_MASS, "usb_sd_is_open\n");
                    ret = 0;
                    return ret;
                }
            }
        }
    }

    DBG_T(MBB_MASS, "usb sd is not open\n");
    return ret;
}

USB_VOID mass_storage_set_sd_card_workmode(USB_INT mode)
{
    struct fsg_lun* curlun = NULL;
    USB_INT i = 0;
    USB_INT j = 0;
    struct fsg_common* common = NULL;

    DBG_T(MBB_MASS, "%s\n", mode ? "USB" : "WEB");
    switch (mode)
    {
        case SD_CARD_WEB_MODE:
            g_sd_card_info.sd_card_workmode = SD_WORKMODE_WEBNAS;
            break;
        case SD_CARD_USB_MODE:
            g_sd_card_info.sd_card_workmode = SD_WORKMODE_MASS;
            break;
        default :
            DBG_E(MBB_MASS, "err mode %d\n", mode);
    }

    for (i = 0; i < SUPPORT_MASS_FUNCTION_NUM; i++)
    {
        struct mass_storage_function_config_data* configdata = NULL;

        if (NULL != g_enabled_mass_function_list[i])
        {
            configdata = g_enabled_mass_function_list[i]->config;
            common = configdata->common;
        }
        else
        {
            continue;
        }

        for (j = 0; j < common->nluns; j++)
        {
            curlun = &common->luns[j];

            if ( NULL != curlun && !curlun->cdrom )
            {
                if ( SD_WORKMODE_WEBNAS == mode)
                {
                    curlun->mode = SD_CARD_WEB_MODE;
                }
                else if ( SD_WORKMODE_MASS == mode)
                {
                    curlun->mode = SD_CARD_USB_MODE;
                }
            }
        }
    }
}
USB_VOID mass_storage_set_sd_card_status(USB_INT sd_removed)
{
    /* SD卡拔出，将SD卡的filp指针赋空 */
    if (sd_removed)
    {
        DBG_T(MBB_MASS, "sd_card removed usb need close sd\n");
        g_sd_card_info.sd_card_status = SD_CARD_NOT_PRESENT;
    }
    else
    {
        /* SD卡插入，重新open sd卡分区 */
        DBG_T(MBB_MASS, "sd_card attach usb can open sd\n");
        g_sd_card_info.sd_card_status = SD_CARD_PRESENT;
        mass_storage_open_usb_sd();
    }
}

#endif

USB_INT mass_save_enabled_function_to_list(struct android_usb_function* enabled_function)
{
    USB_INT i = 0;

    for (i = 0; i < SUPPORT_MASS_FUNCTION_NUM; i++)
    {
        if (NULL == g_enabled_mass_function_list[i])
        {
            g_enabled_mass_function_list[i] = enabled_function;
            break;
        }
    }

    if (SUPPORT_MASS_FUNCTION_NUM == i)
    {
        DBG_E(MBB_MASS, "function_list IS FULL\n");
        return MBB_USB_ERROR;
    }

    return MBB_USB_OK;
}

USB_VOID mass_del_enabled_function_from_list(struct android_usb_function* del_function)
{
    USB_INT i = 0;

    for (i = 0; i < SUPPORT_MASS_FUNCTION_NUM; i++)
    {
        if (del_function == g_enabled_mass_function_list[i])
        {
            g_enabled_mass_function_list[i] = NULL;
        }
    }
}

static USB_VOID fsg_common_release(struct kref* ref);

static USB_VOID fsg_lun_release(struct device* dev)
{
    /* Nothing needs to be done */
}

static inline USB_VOID fsg_common_get(struct fsg_common* common)
{
    kref_get(&common->ref);
}

static inline USB_VOID fsg_common_put(struct fsg_common* common)
{
    kref_put(&common->ref, fsg_common_release);
}

static struct fsg_common* fsg_common_init(struct fsg_common* common,
        struct usb_composite_dev* cdev,
        struct fsg_config* cfgconfig)
{
    struct mass_storage_function_config_data*    configdata = cfgconfig->private_data;
    struct usb_gadget*       gadget = cdev->gadget;
    struct fsg_buffhd*       bh;
    struct fsg_lun*          curlun;
    struct fsg_lun_config*   lcfg;
    struct fsg_state_count*  fsg_state_for_common = NULL;

    struct android_usb_function* lunparent   = NULL;
    struct device*               mass_dev    = NULL;
    mbb_usb_nv_info_st*          usb_nv_info = NULL;

    USB_INT     nluns, i, rc;
    USB_PCHAR   pathbuf;
    USB_CHAR    filename[32];
    USB_INT     idex;
    USB_CHAR    version[USB_NUM_4] = {0};

    DBG_I(MBB_MASS, "in\n");
    lunparent = configdata->parent;
    usb_nv_info = usb_nv_get_ctx();
    rc = fsg_num_buffers_validate();

    if (rc != 0)
    { return ERR_PTR(rc); }

    /* Find out how many LUNs there should be */
    nluns = cfgconfig->nluns;

    if (nluns < 1 || nluns > FSG_MAX_LUNS)
    {
        DBG_E(MBB_MASS, "invalid number of LUNs: %u\n", nluns);
        return ERR_PTR(-EINVAL);
    }

    /* Allocate? */
    if (!common)
    {
        common = kzalloc(sizeof * common, GFP_KERNEL);

        if (!common)
        {
            DBG_E(MBB_MASS, "fail to kzalloc common \n");
            return ERR_PTR(-ENOMEM);
        }

        common->free_storage_on_release = 1;
    }
    else
    {
        memset(common, 0, sizeof * common);
        common->free_storage_on_release = 0;
    }

    common->buffhds = kcalloc(fsg_num_buffers,
                              sizeof * (common->buffhds), GFP_KERNEL);

    if (!common->buffhds)
    {
        if (common->free_storage_on_release)
        { kfree(common); }

        DBG_E(MBB_MASS, "fail to kzalloc common->buffhds \n");
        return ERR_PTR(-ENOMEM);
    }

    /*for sysfile*/

    common->ops     = cfgconfig->ops;
    common->gadget  = gadget;
    common->ep0     = gadget->ep0;
    common->ep0req  = cdev->req;
    common->cdev    = cdev;
    common->private_data = configdata;

    /* Maybe allocate device-global string IDs, and patch descriptors */
    if (fsg_strings[FSG_STRING_INTERFACE].id == 0)
    {
        rc = usb_string_id(cdev);

        if (unlikely(rc < 0))
        { goto error_release; }

        fsg_strings[FSG_STRING_INTERFACE].id = rc;
        fsg_intf_desc.iInterface = rc;
    }

    /*create sysfs mass dev for multi interface*/
    mass_dev = kzalloc(sizeof (struct device), GFP_KERNEL);
    if (!mass_dev)
    {
        DBG_E(MBB_MASS, "fail to kzalloc mass_dev \n");
        goto error_release;
    }
    DBG_I(MBB_MASS, "mass_dev %p\n", mass_dev);
    mass_dev->parent = &gadget->dev;
    mass_dev->release = fsg_lun_release;
    mass_dev->platform_data = configdata;
    dev_set_name(mass_dev, "%s", lunparent->name);

    rc = device_register(mass_dev);
    DBG_I(MBB_MASS, "register device %s\n", mass_dev->kobj.name);

    if (rc)
    {
        DBG_E(MBB_MASS, "failed to register device %s %d\n", mass_dev->kobj.name, rc);
        put_device(mass_dev);
        goto error_release;
    }

    /*create fsg_state att for mass dev*/
    rc = device_create_file(mass_dev, &dev_attr_fsg_state);
    DBG_I(MBB_MASS, "create_file fsg_state for %s \n", mass_dev->kobj.name);

    if (rc)
    {
        DBG_E(MBB_MASS, "fail to create_file fsg_state for %s \n", mass_dev->kobj.name);
        goto error_release;
    }

    /*create fsg_state att for android_usb_function*/
    /*rc = device_create_file(f->dev, &dev_attr_fsg_state);
    if (rc)
    {
    	DBG_E(MBB_MASS, "device_create_file fsg_state for %s failed \n",f->name);
    	goto error_luns;
    }*/

    /*
     * Create the LUNs, open their backing files, and register the
     * LUN devices in sysfs.
     */
    curlun = kcalloc(nluns, sizeof(*curlun), GFP_KERNEL);

    if (unlikely(!curlun))
    {
        DBG_E(MBB_MASS, "fail to kzalloc curlun\n");
        rc = -ENOMEM;
        goto error_release;
    }
    DBG_I(MBB_MASS, "curlun %p\n", curlun);

    common->luns = curlun;
    init_rwsem(&common->filesem);

    for (i = 0, lcfg = cfgconfig->luns; i < nluns; ++i, ++curlun, ++lcfg)
    {
        curlun->cdrom = !!lcfg->cdrom;
        curlun->ro = lcfg->cdrom || lcfg->ro;
        curlun->initially_ro = curlun->ro;
        curlun->removable = lcfg->removable;
        curlun->mode = lcfg->mode;
        curlun->dev.release = fsg_lun_release;
        curlun->dev.parent = mass_dev;
        dev_set_drvdata(&curlun->dev, &common->filesem);
        dev_set_name(&curlun->dev,
                     cfgconfig->lun_name_format
                     ? cfgconfig->lun_name_format
                     : "lun%d_%s",
                     i, (curlun->cdrom ? "CD" : "SD"));

        rc = device_register(&curlun->dev);
        DBG_I(MBB_MASS, "register device LUN%d: %s\n", i, curlun->dev.kobj.name);

        if (rc)
        {
            DBG_E(MBB_MASS, "failed to register device %s: %d\n", curlun->dev.kobj.name, rc);
            common->nluns = i;
            put_device(&curlun->dev);
            goto error_release;
        }

        rc = device_create_file(&curlun->dev, &dev_attr_ro);
        DBG_I(MBB_MASS, "create_file ro for %s \n", curlun->dev.kobj.name);

        if (rc)
        {
            DBG_E(MBB_MASS, "fail to create_file ro for %s \n", curlun->dev.kobj.name);
            goto error_luns;
        }

        rc = device_create_file(&curlun->dev, &dev_attr_file);
        DBG_I(MBB_MASS, "create_file file for %s \n", curlun->dev.kobj.name);

        if (rc)
        {
            DBG_E(MBB_MASS, "fail to create_file file for %s \n", curlun->dev.kobj.name);
            goto error_luns;
        }

        rc = device_create_file(&curlun->dev, &dev_attr_nofua);
        DBG_I(MBB_MASS, "create_file nofua for %s \n", curlun->dev.kobj.name);

        if (rc)
        {
            DBG_E(MBB_MASS, "fail to create_file nofua for %s \n", curlun->dev.kobj.name);
            goto error_luns;
        }

        rc = device_create_file(&curlun->dev, &dev_attr_mode);
        DBG_I(MBB_MASS, "create_file mode for %s \n", curlun->dev.kobj.name);

        if (rc)
        {
            DBG_E(MBB_MASS, "fail to create_file mode for %s \n", curlun->dev.kobj.name);
            goto error_luns;
        }

        /*link luns infomations to android_usb_functions like  'f_mass'*/
        rc = sysfs_create_link(&lunparent->dev->kobj, &curlun->dev.kobj, curlun->dev.kobj.name);
        DBG_I(MBB_MASS, "create_link 'lun' sysfs  for %s \n", curlun->dev.kobj.name);

        if (rc)
        {
            DBG_E(MBB_MASS, "fail to create_link 'lun' sysfs  for %s \n", curlun->dev.kobj.name);
            goto error_luns;
        }

        memset(filename, 0, 32);

        idex = 0;

        if (curlun->cdrom && pnp_if_cdrom_can_open() )
        {
            /*USB init 比 sysfs init早，数据卡和E5插USB开机.靠sysfs init调用脚本
             *mass_boot.sh打开光盘
             *E5非插USB开机fsg_lun_open 会返回成功*/
            (USB_VOID)fsg_lun_open(curlun, CDROM_MNT_DIR);
        }

#ifdef USB_SD
        else if (1 == curlun->mode)
        {
            DBG_T(MBB_MASS, "usb have condition to open sd\n");
            idex =  usb_get_mmc_dev_idex();

            if (idex > -1)
            {
                snprintf(filename, 30, "dev/block/mmcblk%d", idex);
            }
            else
            {
                DBG_T(MBB_MASS, "usb_get_mmc_dev_idex return err\n");
                snprintf(filename, 30, "dev/block/mmcblk0");
            }
        }

        if (filename[0])
        {
            rc = fsg_lun_open(curlun, filename);

            if (rc && !lcfg->cdrom)
            {
                if (idex > -1)
                {
                    snprintf(filename, 30, "dev/block/mmcblk%dp1", idex);
                    (USB_VOID)fsg_lun_open(curlun, filename);
                }
                else
                {
                    snprintf(filename, 30, "dev/block/mmcblk0p1");
                    (USB_VOID)fsg_lun_open(curlun, filename);
                }

            }
        }

#endif

        if (lcfg->filename)
        {
            /*usb not conect*/

        }
        else if (!curlun->removable)
        {
            DBG_E(MBB_MASS, "you configure removable for LUN%d not correct\n", i);
            rc = -EINVAL;
            goto error_luns;
        }

        if (curlun->cdrom)
        {
            cfgconfig->vendor_name = usb_nv_info->mass_dynamic_name.huawei_cdrom_dynamic_name;
            cfgconfig->product_name = usb_nv_info->mass_dynamic_name.huawei_cdrom_dynamic_name + USB_NUM_8;
            cfgconfig->release = get_unaligned_be16(usb_nv_info->mass_dynamic_name.huawei_cdrom_dynamic_name + USB_NUM_24);
            memcpy(version,
                   usb_nv_info->mass_dynamic_name.huawei_cdrom_dynamic_name + USB_NUM_24,
                   USB_NUM_4);
        }
        else
        {
            cfgconfig->vendor_name = usb_nv_info->mass_dynamic_name.huawei_sd_dynamic_name;
            cfgconfig->product_name = usb_nv_info->mass_dynamic_name.huawei_sd_dynamic_name + USB_NUM_8;
            cfgconfig->release = get_unaligned_be16(usb_nv_info->mass_dynamic_name.huawei_sd_dynamic_name + USB_NUM_24);
            memcpy(version,
                   usb_nv_info->mass_dynamic_name.huawei_sd_dynamic_name + USB_NUM_24,
                   USB_NUM_4);
        }

        snprintf(curlun->inquiry_string, sizeof curlun->inquiry_string,
                 "%-8s%-16s%-4s\n", cfgconfig->vendor_name ? : "Linux",
                 /* Assume product name dependent on the curlun */
                 cfgconfig->product_name ? : (curlun->cdrom
                                              ? "File-Stor Gadget"
                                              : "File-CD Gadget"),
                 version);


    }

    common->nluns = nluns;

    /* Data buffers cyclic list */
    bh = common->buffhds;
    i = fsg_num_buffers;
    goto buffhds_first_it;

    do
    {
        bh->next = bh + 1;
        ++bh;
    buffhds_first_it:
        bh->buf = kmalloc(FSG_BUFLEN, GFP_KERNEL);

        if (unlikely(!bh->buf))
        {
            rc = -ENOMEM;
            goto error_release;
        }
    }
    while (--i);

    bh->next = common->buffhds;

    /*
     * Some peripheral controllers are known not to be able to
     * halt bulk endpoints correctly.  If one of them is present,
     * disable stalls.
     */
    common->can_stall = cfgconfig->can_stall &&
                        !(gadget_is_at91(common->gadget));

    spin_lock_init(&common->lock);
    kref_init(&common->ref);

    /* Tell the thread to start working */
    common->thread_task =
        kthread_create(fsg_main_thread, common,
                       cfgconfig->thread_name ? : "file-storage");

    if (IS_ERR(common->thread_task))
    {
        rc = PTR_ERR(common->thread_task);
        goto error_release;
    }

    init_completion(&common->thread_notifier);
    init_waitqueue_head(&common->fsg_wait);

    /* Information */
    INFO(common, FSG_DRIVER_DESC ", version: " FSG_DRIVER_VERSION "\n");
    INFO(common, "Number of LUNs=%d\n", common->nluns);

    pathbuf = kmalloc(PATH_MAX, GFP_KERNEL);

    for (i = 0, nluns = common->nluns, curlun = common->luns;
         i < nluns;
         ++curlun, ++i)
    {
        USB_PCHAR p = "(no medium)";

        if (fsg_lun_is_open(curlun))
        {
            p = "(medium)";

            if (pathbuf)
            {
                p = d_path(&curlun->filp->f_path,
                           pathbuf, PATH_MAX);

                if (IS_ERR(p))
                { p = "(error)"; }
            }
        }

        DBG_I(MBB_MASS, "LUN%d: %s%s%s%sfile: %s\n", i,
              curlun->removable ? "removable;" : "",
              curlun->ro ? "read only;" : "RW;",
              curlun->cdrom ? "CD-ROM;" : "SD;",
              curlun->mode ? "usb mode;" : "web mode;",
              p);
    }

    kfree(pathbuf);

    DBG_I(MBB_MASS, "I/O thread pid: %d\n", task_pid_nr(common->thread_task));

    wake_up_process(common->thread_task);

    return common;

error_luns:
    common->nluns = i + 1;
error_release:
    fsg_state_for_common = &(configdata->fsg_state);
    common->state = FSG_STATE_TERMINATED;	/* The thread is dead */
    fsg_state_for_common->terminated++;
    /* Call fsg_common_release() directly, ref might be not initialised. */
    fsg_common_release(&common->ref);
    DBG_E(MBB_MASS, "fsg_common_init error\n");
    return ERR_PTR(rc);
}

static USB_VOID fsg_common_release(struct kref* ref)
{
    struct fsg_common* common = container_of(ref, struct fsg_common, ref);
    struct mass_storage_function_config_data* configdata = NULL;
    struct android_usb_function* f = NULL;
    USB_INT i;

    if (likely(common->luns))
    {
        struct fsg_lun* lun = common->luns;
        struct device* luns_parent = common->luns->dev.parent;
        i = common->nluns;

        if (likely(common->private_data))
        {
            configdata = common->private_data;
            f = configdata->parent;
        }

        /* In error recovery common->nluns may be zero. */
        for (; i; --i, ++lun)
        {
            if (NULL != f)
            {
                DBG_I(MBB_MASS, "%s remove_link%s\n", f->name, lun->dev.kobj.name);
                sysfs_remove_link(&f->dev->kobj, lun->dev.kobj.name);
            }

            DBG_I(MBB_MASS, "%s remove_file nofua\n", lun->dev.kobj.name);
            device_remove_file(&lun->dev, &dev_attr_nofua);

            DBG_I(MBB_MASS, "%s remove_file ro\n", lun->dev.kobj.name);
            device_remove_file(&lun->dev, &dev_attr_ro);

            DBG_I(MBB_MASS, "%s remove_file file\n", lun->dev.kobj.name);
            device_remove_file(&lun->dev, &dev_attr_file);

            DBG_I(MBB_MASS, "%s remove_file mode\n", lun->dev.kobj.name);
            device_remove_file(&lun->dev, &dev_attr_mode);

            fsg_lun_close(lun);

            DBG_I(MBB_MASS, "unregister%s\n", lun->dev.kobj.name);
            device_unregister(&lun->dev);
        }

        DBG_I(MBB_MASS, "%s remove_file  fsg_state\n", luns_parent->kobj.name);
        device_remove_file(luns_parent, &dev_attr_fsg_state);

        DBG_I(MBB_MASS, "unregister %s\n", luns_parent->kobj.name);
        device_unregister(luns_parent);

        kfree(common->luns);
        kfree(luns_parent);
    }

    {
        struct fsg_buffhd* bh = common->buffhds;
        USB_UINT i = fsg_num_buffers;

        do
        {
            kfree(bh->buf);
        }
        while (++bh, --i);
    }

    kfree(common->buffhds);

    if (common->free_storage_on_release)
    {
        kfree(common);
    }

}


/*-------------------------------------------------------------------------*/

static USB_VOID fsg_unbind(struct usb_configuration* c, struct usb_function* f)
{
    struct fsg_dev*		fsg = fsg_from_func(f);
    struct fsg_common*	common = fsg->common;
    USB_INT ret = 0;

    DBG_I(MBB_MASS, "\n");

    if (fsg->common->fsg == fsg)
    {
        fsg->common->new_fsg = NULL;
        raise_exception(fsg->common, FSG_STATE_CONFIG_CHANGE);
        /* FIXME: make interruptible or killable somehow? */
        wait_event(common->fsg_wait, common->fsg != fsg);
    }

    /* If the thread isn't already dead, tell it to exit now */
    if (common->state != FSG_STATE_TERMINATED)
    {
        raise_exception(common, FSG_STATE_EXIT);
        wait_for_completion(&common->thread_notifier);
    }


    if (NULL != common->thread_task)
    {
        ret = kthread_stop(common->thread_task);
        DBG_E(MBB_MASS, "thread function has run %ds\n", ret);
    }
    else
    {
        common->thread_task = NULL;
    }

    if (0 != ret)
    {
        DBG_E(MBB_MASS, "thread function has run %ds\n", ret);
    }
#ifdef MBB_USB_UNITARY_Q
    usb_free_descriptors(fsg->function.fs_descriptors);
#else
    usb_free_descriptors(fsg->function.descriptors);
#endif
    usb_free_descriptors(fsg->function.hs_descriptors);
    usb_free_descriptors(fsg->function.ss_descriptors);
    DBG_I(MBB_MASS, "fsg_unbind success \n");
}

static USB_INT fsg_bind(struct usb_configuration* c, struct usb_function* f)
{
    struct fsg_dev*		fsg = fsg_from_func(f);
    struct usb_gadget*	gadget = c->cdev->gadget;
    USB_INT			i;
    struct usb_ep*		ep;

    fsg->gadget = gadget;

    /* New interface */
    i = usb_interface_id(c, f);

    if (i < 0)
    { return i; }

    fsg_intf_desc.bInterfaceNumber = i;
    fsg->interface_number = i;

    /* Find all the endpoints we will use */
    ep = usb_ep_autoconfig(gadget, &fsg_fs_bulk_in_desc);

    if (!ep)
    { goto autoconf_fail; }

    ep->driver_data = fsg->common;	/* claim the endpoint */
    fsg->bulk_in = ep;

    ep = usb_ep_autoconfig(gadget, &fsg_fs_bulk_out_desc);

    if (!ep)
    { goto autoconf_fail; }

    ep->driver_data = fsg->common;	/* claim the endpoint */
    fsg->bulk_out = ep;
#ifdef MBB_USB_UNITARY_Q
    /* Copy descriptors */
    f->fs_descriptors = usb_copy_descriptors(fsg_fs_function);
    if (unlikely(!f->fs_descriptors))
    { return -ENOMEM; }

#else
    f->descriptors = usb_copy_descriptors(fsg_fs_function);
    if (unlikely(!f->descriptors))
    { return -ENOMEM; }
#endif

    if (gadget_is_dualspeed(gadget))
    {
        /* Assume endpoint addresses are the same for both speeds */
        fsg_hs_bulk_in_desc.bEndpointAddress =
            fsg_fs_bulk_in_desc.bEndpointAddress;
        fsg_hs_bulk_out_desc.bEndpointAddress =
            fsg_fs_bulk_out_desc.bEndpointAddress;
        f->hs_descriptors = usb_copy_descriptors(fsg_hs_function);

        if (unlikely(!f->hs_descriptors))
        {
#ifdef MBB_USB_UNITARY_Q
            usb_free_descriptors(f->fs_descriptors);
#else
            usb_free_descriptors(f->descriptors);
#endif
            return -ENOMEM;
        }
    }

    if (gadget_is_superspeed(gadget))
    {
        USB_UINT	max_burst;

        /* Calculate bMaxBurst, we know packet size is 1024 */
        max_burst = min_t(USB_UINT, FSG_BUFLEN / 1024, 15);

        fsg_ss_bulk_in_desc.bEndpointAddress =
            fsg_fs_bulk_in_desc.bEndpointAddress;
        fsg_ss_bulk_in_comp_desc.bMaxBurst = max_burst;

        fsg_ss_bulk_out_desc.bEndpointAddress =
            fsg_fs_bulk_out_desc.bEndpointAddress;
        fsg_ss_bulk_out_comp_desc.bMaxBurst = max_burst;

        f->ss_descriptors = usb_copy_descriptors(fsg_ss_function);

        if (unlikely(!f->ss_descriptors))
        {
            usb_free_descriptors(f->hs_descriptors);
#ifdef MBB_USB_UNITARY_Q
            usb_free_descriptors(f->fs_descriptors);
#else
            usb_free_descriptors(f->descriptors);
#endif
            return -ENOMEM;
        }
    }

    //open_nluns(&fsg->common->open_nluns_work);

    return 0;

autoconf_fail:
    DBG_E(MBB_MASS, "unable to autoconfigure all endpoints\n");
    return -ENOTSUPP;
}


/****************************** ADD FUNCTION ******************************/

static struct usb_gadget_strings* fsg_strings_array[] =
{
    &fsg_stringtab,
    NULL,
};

static USB_INT fsg_bind_config(struct usb_composite_dev* cdev,
                               struct usb_configuration* c,
                               struct fsg_common* common)
{
    struct fsg_dev* fsg;
    USB_INT rc;
    struct mass_storage_function_config_data* configdata = common->private_data;
    struct android_usb_function* fsgparent = configdata->parent;
    fsg = kzalloc(sizeof * fsg, GFP_KERNEL);

    if (unlikely(!fsg))
    {
        DBG_E(MBB_MASS, "fail to kzalloc fsg \n");
        return -ENOMEM;
    }

    fsg->function.name        = FSG_DRIVER_DESC;
    fsg->function.strings     = fsg_strings_array;
    fsg->function.bind        = fsg_bind;
    fsg->function.unbind      = fsg_unbind;
    fsg->function.setup       = fsg_setup;
    fsg->function.set_alt     = fsg_set_alt;
    fsg->function.disable     = fsg_disable;

    fsg->common               = common;
    /*
     * Our caller holds a reference to common structure so we
     * don't have to be worry about it being freed until we return
     * from this function.  So instead of incrementing counter now
     * and decrement in error recovery we increment it only when
     * call to usb_add_function() was successful.
     */

    rc = usb_add_function(c, &fsg->function);

    if (unlikely(rc))
    {
        DBG_E(MBB_MASS, "fail to add_function %s type %s\n", fsgparent->name, fsg->function.name);
        kfree(fsg);
    }
    else
    { fsg_common_get(fsg->common); }

    return rc;
}

static USB_INT fsg_unbind_config(struct usb_composite_dev* cdev,
                                 struct usb_configuration* c,
                                 struct fsg_common* common)
{
    struct fsg_dev* fsg = common->fsg;

    kfree(fsg);
    common->fsg = NULL;
    DBG_I(MBB_MASS, "fsg_unbind_config success\n");

    return 0;
}

static inline USB_INT __deprecated __maybe_unused
fsg_add(struct usb_composite_dev* cdev, struct usb_configuration* c,
        struct fsg_common* common)
{
    return fsg_bind_config(cdev, c, common);
}

static USB_VOID
fsg_config_from_params(struct fsg_config* cfgconfig,
                       const struct fsg_module_parameters* params,
                       struct mass_storage_function_config_data* privatedata)
{
    struct fsg_lun_config* lun;
    USB_UINT i;
    /* Configure LUNs */
    cfgconfig->nluns =
        min(params->luns ? : (params->file_count ? : 1u),
            (USB_UINT)FSG_MAX_LUNS);

    for (i = 0, lun = cfgconfig->luns; i < cfgconfig->nluns; ++i, ++lun)
    {
        lun->ro = !!params->ro[i];
        lun->cdrom = !!params->cdrom[i];
        lun->mode = !!params->mode[i];
        lun->removable = /* Removable by default */
            params->removable_count <= i || params->removable[i];
        lun->filename =
            params->file_count > i && params->file[i][0]
            ? params->file[i]
            : 0;
    }

    /* Let MSF use defaults */
    cfgconfig->lun_name_format = 0;
    cfgconfig->thread_name = 0;
    cfgconfig->vendor_name = 0;
    cfgconfig->product_name = 0;
    cfgconfig->release = 0xffff;

    cfgconfig->ops = NULL;
    cfgconfig->private_data = privatedata;

    /* Finalise */
    cfgconfig->can_stall = params->stall;
}

static inline struct fsg_common*
fsg_common_from_params(struct fsg_common* common,
                       struct usb_composite_dev* cdev,
                       const struct fsg_module_parameters* params,
                       struct mass_storage_function_config_data* configdata)
__attribute__((unused));

static inline struct fsg_common*
fsg_common_from_params(struct fsg_common* common,
                       struct usb_composite_dev* cdev,
                       const struct fsg_module_parameters* params,
                       struct mass_storage_function_config_data* configdata)
{
    struct fsg_config* cfgconfig = &(configdata->fsgconfig);
    fsg_config_from_params(cfgconfig, params, configdata);
    return fsg_common_init(common, cdev, cfgconfig);
}


static USB_INT mass_storage_function_init(struct android_usb_function* f, struct usb_composite_dev* cdev)
{
    struct mass_storage_function_config_data* config;

    config = kzalloc(sizeof(struct mass_storage_function_config_data), GFP_KERNEL);

    if (!config)
    {
        return -ENOMEM;
    }

    f->config = config;
    config->parent = f;
    return 0;
}

static USB_VOID mass_storage_function_cleanup(struct android_usb_function* f)
{
    kfree(f->config);
    f->config = NULL;
}

/*add a new lun info to func*/
USB_VOID mass_add_lun_name_to_mass_func(mass_lun_info* mass_luns, USB_CHAR* addname)
{
    USB_INT i = 0;

    for (i = 0; i < FSG_MAX_LUNS; i++)
    {
        if (LUN_NONE == mass_luns[i])
        {
            if (!strcmp("CD", addname))
            {
                DBG_I(MBB_MASS, "add %s\n", addname);
                mass_luns[i] = LUN_CD;
            }
            else if (!strcmp("SD", addname))
            {
                DBG_I(MBB_MASS, "add %s\n", addname);
                mass_luns[i] = LUN_SD;
            }
            else
            {
                DBG_E(MBB_MASS, "err lun name %s\n", addname);
            }

            break;
        }
    }
}

/*search the support MASS functions*/
struct mass_storage_status_info* mass_find_function_by_name(USB_CHAR* function_name)
{
    USB_INT i = 0;
    USB_CHAR* mass_func_name = NULL;

    DBG_I(MBB_MASS, "\n");

    for (i = 0; i < SUPPORT_MASS_FUNCTION_NUM; i++)
    {
        mass_func_name =
            g_support_mass_functions_lun_info[i].mass_func_name;

        if ( NULL != mass_func_name)
        {
            if (!strcmp(mass_func_name, function_name))
            {
                DBG_I(MBB_MASS, "find function:%s \n", function_name);
                return &(g_support_mass_functions_lun_info[i]);
            }
        }
    }

    if ( SUPPORT_MASS_FUNCTION_NUM == i)
    {
        DBG_E(MBB_MASS, " can not find function:%s \n", function_name);
    }
    return NULL;
}

/*if the PNP module do not config the lun info ,usb the default one*/
USB_VOID mass_function_default_lun_info(USB_CHAR* function_name)
{
    struct mass_storage_status_info* mass_function = NULL;
    USB_INT j = 0;

    DBG_I(MBB_MASS, "\n");

    mass_function = mass_find_function_by_name(function_name);
    if (NULL != mass_function)
    {
        if (!strcmp("mass", mass_function->mass_func_name))
        {
            mass_function->mass_luns[0] = LUN_CD;
            for (j = 1; j < FSG_MAX_LUNS; j++)
            {
                mass_function->mass_luns[j] = LUN_NONE;
            }
        }
        else if (!strcmp("mass_two", mass_function->mass_func_name))
        {
            mass_function->mass_luns[0] = LUN_SD;
            for (j = 1; j < FSG_MAX_LUNS; j++)
            {
                mass_function->mass_luns[j] = LUN_NONE;
            }
        }
    }
}

USB_INT mass_function_have_no_lun_info(USB_CHAR* function_name)
{
    USB_INT ret = MBB_USB_FALSE;
    mass_lun_info lun_type = LUN_NONE;
    struct mass_storage_status_info* mass_function = NULL;

    DBG_I(MBB_MASS, "\n");
    mass_function = mass_find_function_by_name(function_name);
    lun_type = (USB_INT)mass_function->mass_luns[0];
    if ( LUN_NONE == lun_type)
    {
        ret = MBB_USB_TRUE;
    }
    return ret;
}

/*cleanup the func lun info when unbind func*/
USB_VOID mass_function_cleanup_lun_info(USB_CHAR* function_name)
{
    USB_INT j = 0;
    struct mass_storage_status_info* mass_function = NULL;
    DBG_I(MBB_MASS, "\n");

    mass_function = mass_find_function_by_name(function_name);

    if ( NULL != mass_function)
    {
        for (j = 0; j < FSG_MAX_LUNS; j++)
        {
            mass_function->mass_luns[j] = LUN_NONE;
        }
    }
}

USB_VOID mass_function_cleanup_alllun_info(USB_VOID)
{
    USB_INT i = 0;
    USB_CHAR* mass_func_name = NULL;
    for (i = 0; i < SUPPORT_MASS_FUNCTION_NUM; i++)
    {
        mass_func_name = g_support_mass_functions_lun_info[i].mass_func_name;
        mass_function_cleanup_lun_info(mass_func_name);
    }
}

/*the PNP module to add lun info*/
USB_VOID mass_function_add_lun(USB_CHAR* function_name, USB_CHAR* addlunname)
{
    struct mass_storage_status_info* mass_function = NULL;

    DBG_I(MBB_MASS, "\n");

    mass_function = mass_find_function_by_name(function_name);

    if ( NULL != mass_function && NULL != addlunname)
    {
        mass_add_lun_name_to_mass_func(mass_function->mass_luns, addlunname);
    }
    else
    {
        DBG_E(MBB_MASS, " function name || or lunname err \n");
    }
}

/*use the lun info*/
USB_VOID mass_function_config_lun_info(USB_CHAR* function_name,
                                       struct fsg_module_parameters* fsg_mod_data)
{
    USB_INT j = 0;
    USB_INT lun_type = 0;
    mbb_usb_nv_info_st* usb_nv_info = usb_nv_get_ctx();
    struct mass_storage_status_info* mass_function = NULL;

    DBG_I(MBB_MASS, "\n");
    mass_function = mass_find_function_by_name(function_name);

    if ( NULL != mass_function)
    {
        fsg_mod_data->luns  = 0;
        fsg_mod_data->stall = MBB_USB_YES;
        fsg_mod_data->ro_count = 0;
        fsg_mod_data->file_count = 0;
        fsg_mod_data->cdrom_count = 0;
        fsg_mod_data->nofua_count = 0;
        fsg_mod_data->removable_count = 0;

        for (j = 0; j < FSG_MAX_LUNS; j++)
        {
            lun_type = (USB_INT)mass_function->mass_luns[j];
            DBG_I(MBB_MASS, "%s config lun[%d]\n", function_name, j);

            switch (lun_type)
            {
                case LUN_CD:
                    DBG_I(MBB_MASS, "lun[%d] CD\n", j);
                    fsg_mod_data->luns++;
                    fsg_mod_data->ro_count++;
                    fsg_mod_data->cdrom_count++;
                    fsg_mod_data->nofua_count++;
                    fsg_mod_data->removable_count++;
                    /*if the lun is cdrom*/
                    fsg_mod_data->cdrom[j]     = MBB_USB_YES;
                    /*if the lun can be removed*/
                    fsg_mod_data->removable[j] = MBB_USB_YES;
                    /*the wokemode of the lun*/
                    fsg_mod_data->mode[j]      = SD_WORKMODE_MASS;
                    /*if the lun is read-only*/
                    fsg_mod_data->ro[j]        = MBB_USB_YES;
                    /*if the lun is noFUA*/
                    fsg_mod_data->nofua[j]     = MBB_USB_NO;
                    break;
                case LUN_SD:
                    DBG_I(MBB_MASS, "lun[%d] SD\n", j);
                    fsg_mod_data->luns++;
                    fsg_mod_data->nofua_count++;
                    fsg_mod_data->removable_count++;
                    /*if the lun is cdrom*/
                    fsg_mod_data->cdrom[j]     = MBB_USB_NO;
                    /*if the lun can be removed*/
                    fsg_mod_data->removable[j] = MBB_USB_YES;
                    /*the wokemode of the lun*/
                    fsg_mod_data->mode[j]      = usb_nv_info->ulSDWorkMode;
                    /*if the lun is read-only*/
                    fsg_mod_data->ro[j]        = MBB_USB_NO;
                    /*if the lun is noFUA*/
                    fsg_mod_data->nofua[j]     = MBB_USB_NO;
                    break;
                default:
                    break;
            }
        }
    }
}


static USB_INT mass_storage_function_bind_config(struct android_usb_function* f,
        struct usb_configuration* c)
{
    struct mass_storage_function_config_data* configdata = f->config;
    struct fsg_common* common = NULL;
    struct fsg_module_parameters fsg_mod_data;
    USB_INT status = 0;
    USB_PVOID retp;
    USB_INT is_multi_lun = pnp_is_multi_lun_mode();

    DBG_I(MBB_MASS, "\n");
    if (NULL == configdata || NULL != configdata->common)
    {
        DBG_E(MBB_MASS, "configdata NULL OR configdata->common not NULL configdata=%p common=%p\n",
              configdata , configdata->common);
        DBG_E(MBB_MASS, "bind --%-10s to config --%-25s failed\n", f->name, c->label);
        return 0;
    }

    memset(&fsg_mod_data, 0, sizeof (struct fsg_module_parameters));
    /*rewind_before use multi lun, no rewind_before use default*/
    if (!is_multi_lun || mass_function_have_no_lun_info(f->name))
    {
        mass_function_default_lun_info(f->name);
    }

    mass_function_config_lun_info(f->name, &fsg_mod_data);

    retp = fsg_common_from_params(common, c->cdev, &fsg_mod_data, configdata);

    if (IS_ERR(retp))
    {
        DBG_E(MBB_MASS, "fsg_common return error retp=%p\n", retp);
        DBG_E(MBB_MASS, "bind --%-10s to config --%-25s failed\n", f->name, c->label);
        status = PTR_ERR(retp);

        if (status)
        {
            kfree(configdata);
            return status;
        }
    }

    configdata->common = retp;
    mass_save_enabled_function_to_list(f);

    return fsg_bind_config(c->cdev, c, configdata->common);
}

static USB_VOID mass_function_unbind_config(struct android_usb_function* f, struct usb_configuration* c)
{
    struct mass_storage_function_config_data* configdata = f->config;
    struct fsg_lun* curlun;

    /* clear the string id index */
    if (fsg_strings[FSG_STRING_INTERFACE].id)
    {
        fsg_strings[FSG_STRING_INTERFACE].id = 0;
    }

    curlun = configdata->common->luns;
    fsg_unbind_config(c->cdev, c, configdata->common);

    fsg_common_put(configdata->common);
    /* Call fsg_common_release() directly, ref might be not initialised. */
    fsg_common_release(&configdata->common->ref);

    memset(&configdata->fsgconfig, 0, sizeof (struct fsg_config));
    memset(&configdata->fsg_state, 0, sizeof (struct fsg_state_count));
    configdata->common = NULL;

    mass_del_enabled_function_from_list(f);
    mass_function_cleanup_lun_info(f->name);
}
USB_VOID mass_storage_dump(USB_VOID);
static ssize_t mass_storage_dump_show(struct device* dev, struct device_attribute* attr, USB_CHAR* buf)
{
    mass_storage_dump();
    return 1;
}
static DEVICE_ATTR(mass_dump, S_IRUGO, mass_storage_dump_show, NULL);

USB_VOID mass_init_support_func(USB_VOID)
{
    USB_INT i = 0;
    USB_INT j = 0;

    for (i = 0; i < SUPPORT_MASS_FUNCTION_NUM; i++)
    {
        g_support_mass_functions_lun_info[i].mass_func_name = NULL;

        for (j = 0; j < FSG_MAX_LUNS; j++)
        {
            g_support_mass_functions_lun_info[i].mass_luns[j] = LUN_NONE;
        }
    }
}

USB_VOID mass_add_to_support_func(USB_CHAR* function_name)
{
    USB_INT i = 0;
    USB_CHAR* mass_func_name = NULL;

    for (i = 0; i < SUPPORT_MASS_FUNCTION_NUM; i++)
    {
        mass_func_name = g_support_mass_functions_lun_info[i].mass_func_name;

        if (!mass_func_name)
        {
            DBG_I(MBB_MASS, "add support func %s\n", function_name);
            g_support_mass_functions_lun_info[i].mass_func_name = function_name;
            return;
        }
    }

    DBG_E(MBB_MASS, "can not add support func %s\n", function_name);
}

static struct device_attribute* mass_storage_function_attributes[] =
{
    &dev_attr_fsg_state,
    &dev_attr_mass_dump,
    NULL
};

static struct android_usb_function mass_storage_function_for_config1 =
{
    .name           = "mass",
    .init           = mass_storage_function_init,
    .cleanup        = mass_storage_function_cleanup,
    .bind_config    = mass_storage_function_bind_config,
    .unbind_config  = mass_function_unbind_config,
    .attributes     = mass_storage_function_attributes,
};
static struct android_usb_function mass_storage_function_for_config2 =
{
    .name           = "mass_two",
    .init           = mass_storage_function_init,
    .cleanup        = mass_storage_function_cleanup,
    .bind_config    = mass_storage_function_bind_config,
    .unbind_config  = mass_function_unbind_config,
    .attributes     = mass_storage_function_attributes,
};
USB_VOID usb_mass_storage_init(USB_VOID)
{
    DBG_I(MBB_MASS, "enter\n");
    mass_init_support_func();
    pnp_register_usb_support_function(&mass_storage_function_for_config1);
    mass_add_to_support_func(mass_storage_function_for_config1.name);
    pnp_register_usb_support_function(&mass_storage_function_for_config2);
    mass_add_to_support_func(mass_storage_function_for_config2.name);
    setup_timer(&g_soft_mass_timer , soft_mass_handler, (USB_ULONG)0);
}

USB_VOID mass_storage_dump(USB_VOID)
{
    struct fsg_lun* curlun = NULL;
    USB_INT i = 0;
    USB_INT j = 0;
    USB_PCHAR p = "no medium";
    USB_PCHAR pathbuf = NULL;

    struct fsg_common* common = NULL;
    struct mass_storage_function_config_data* configdata = NULL;
    struct android_usb_function* f = NULL;

    for (i = 0; i < SUPPORT_MASS_FUNCTION_NUM; i++)
    {
        if (NULL != g_enabled_mass_function_list[i])
        {
            configdata = g_enabled_mass_function_list[i]->config;
            f = g_enabled_mass_function_list[i];
            common = configdata->common;
        }
        else
        {
            DBG_I(MBB_MASS, "g_enabled_mass_function_list[%d] %p \n", i, g_enabled_mass_function_list[i]);
            break;
        }

        DBG_T(MBB_DEBUG, "|--------------------------------------------------------------\n");
        DBG_T(MBB_DEBUG, "|--------mass_storage func '%s' dump msg--------------\n", f->name);
        DBG_T(MBB_DEBUG, "|--------------------------------------------------------------\n");
        DBG_T(MBB_DEBUG, "|The name of---------- mass_storage is '%s'.\n", common->curlun->dev.parent->kobj.name);
        DBG_T(MBB_DEBUG, "|The nluns of--------- mass_storage is %d.\n", common->nluns);
        pathbuf = kmalloc(PATH_MAX, GFP_KERNEL);

        for (j = 0; j < common->nluns; j++)
        {
            curlun = &common->luns[j];

            if (fsg_lun_is_open(curlun))
            {
                p = "medium";

                if (pathbuf)
                {
                    p = d_path(&curlun->filp->f_path,
                               pathbuf, PATH_MAX);

                    if (IS_ERR(p))
                    { p = "error"; }
                }
            }

            DBG_T(MBB_DEBUG, "|--------------------------------------------------------------\n");
            DBG_T(MBB_DEBUG, "|The string of lun%d is '%s'.\n", j, curlun->inquiry_string);
            DBG_T(MBB_DEBUG, "|The type of------------------ lun%d is '%s'.\n", j, (curlun->cdrom ? "CD" : "SD"));
            DBG_T(MBB_DEBUG, "|The filp of------------------ lun%d is %p.\n", j, curlun->filp);
            DBG_T(MBB_DEBUG, "|The file of------------------ lun%d is '%s'.\n", j, p);
            DBG_T(MBB_DEBUG, "|The file_length of----------- lun%d is %d.\n", j, (USB_INT)curlun->file_length);
            DBG_T(MBB_DEBUG, "|The num_sectors of----------- lun%d is %d.\n", j, (USB_INT)curlun->num_sectors);
            DBG_T(MBB_DEBUG, "|The read only of------------- lun%d is '%s'.\n", j, curlun->ro ? "Read-only" : "RW");
            DBG_T(MBB_DEBUG, "|The removable of------------- lun%d is '%s'.\n", j, curlun->removable ? "Yes" : "No");
            DBG_T(MBB_DEBUG, "|The prevent_medium_removal of lun%d is %d.\n", j, curlun->prevent_medium_removal);
            DBG_T(MBB_DEBUG, "|The mode of------------------ lun%d is '%s'.\n", j, (curlun->mode ? "usb mode" : "web mode"));
            DBG_T(MBB_DEBUG, "|The keep_eject of------------ lun%d is %d.\n", j, curlun->keep_eject);
            DBG_T(MBB_DEBUG, "|The keep_no_medium_state of-- lun%d is %d.\n", j, curlun->keep_no_medium_state);
            DBG_T(MBB_DEBUG, "|The blkbits of--------------- lun%d is %d.\n", j, curlun->blkbits);
            DBG_T(MBB_DEBUG, "|The blksize of--------------- lun%d is %d.\n", j, curlun->blksize);
            DBG_T(MBB_DEBUG, "|--------------------------------------------------------------\n");
        }

        kfree(pathbuf);
    }
}
