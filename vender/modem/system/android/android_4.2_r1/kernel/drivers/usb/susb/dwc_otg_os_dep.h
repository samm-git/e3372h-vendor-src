/*
 * Synopsys Portability Library Software and documentation
 * (hereinafter, "Software") is an Unsupported proprietary work of
 * Synopsys, Inc. unless otherwise expressly agreed to in writing
 * between Synopsys and you.
 * 
 * The Software IS NOT an item of Licensed Software or Licensed Product
 * under any End User Software License Agreement or Agreement for
 * Licensed Product with Synopsys or any supplement thereto. You are
 * permitted to use and redistribute this Software in source and binary
 * forms, with or without modification, provided that redistributions
 * of source code must retain this notice. You may not view, use,
 * disclose, copy or distribute this file or any information contained
 * herein except pursuant to this license grant from Synopsys. If you
 * do not agree with this notice, including the disclaimer below, then
 * you are not authorized to use the Software.
 * 
 * Alternatively, this software may be distributed under the terms of the
 * GNU General Public License ("GPL") version 2 as published by the Free
 * Software Foundation.
 * 
 * THIS SOFTWARE IS BEING DISTRIBUTED BY SYNOPSYS SOLELY ON AN "AS IS"
 * BASIS AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE HEREBY DISCLAIMED. IN NO EVENT SHALL
 * SYNOPSYS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 */
#ifndef _DWC_OS_DEP_H_
#define _DWC_OS_DEP_H_

/**
 * @file
 *
 * This file contains OS dependent structures.
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/interrupt.h>
#include <linux/ctype.h>
#include <linux/string.h>
#include <linux/dma-mapping.h>
#include <linux/jiffies.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include <linux/workqueue.h>
#include <linux/stat.h>
#include <linux/pci.h>

#include <linux/version.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20)
#include <linux/irq.h>
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,21)
#include <linux/usb/ch9.h>
#else
#include <linux/usb_ch9.h>
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,24)
#include <linux/usb/gadget.h>
#else
#include <linux/usb_gadget.h>
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20)
#include <asm/irq.h>
#endif

#ifdef PCI_INTERFACE
#include <asm/io.h>
#endif

#ifdef LM_INTERFACE
#include <asm/unaligned.h>
#include <asm/sizes.h>
#include <asm/param.h>
#include <asm/io.h>
#include <mach/lm.h>
#include <mach/irqs.h>
/*# include <mach/regs-irq.h>*/
#endif

/** The OS page size */
#define DWC_OS_PAGE_SIZE	PAGE_SIZE

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,14)
typedef int gfp_t;
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,18)
#define IRQF_SHARED SA_SHIRQ
#endif

typedef struct os_dependent {
	/** Base address returned from ioremap() */
	void *base;

	/** Register offset for Diagnostic API */
	uint32_t reg_offset;

#ifdef LM_INTERFACE
	struct lm_device *lmdev;
#elif  defined(PCI_INTERFACE)
	struct pci_dev *pcidev;

	/** Start address of a PCI region */
	resource_size_t rsrc_start;
	
	/** Length address of a PCI region */
	resource_size_t rsrc_len;
#endif
} os_dependent_t;

#ifdef __cplusplus
}
#endif

#endif /* _DWC_OS_DEP_H_ */
