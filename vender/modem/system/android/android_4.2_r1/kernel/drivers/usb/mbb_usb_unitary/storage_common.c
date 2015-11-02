/*
 * storage_common.c -- Common definitions for mass storage functionality
 *
 * Copyright (C) 2003-2008 Alan Stern
 * Copyeight (C) 2009 Samsung Electronics
 * Author: Michal Nazarewicz (mina86@mina86.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */


/*
 * This file requires the following identifiers used in USB strings to
 * be defined (each of type pointer to char):
 *  - fsg_string_manufacturer -- name of the manufacturer
 *  - fsg_string_product      -- name of the product
 *  - fsg_string_config       -- name of the configuration
 *  - fsg_string_interface    -- name of the interface
 * The first four are only needed when FSG_DESCRIPTORS_DEVICE_STRINGS
 * macro is defined prior to including this file.
 */

/*
 * When FSG_NO_INTR_EP is defined fsg_fs_intr_in_desc and
 * fsg_hs_intr_in_desc objects as well as
 * FSG_FS_FUNCTION_PRE_EP_ENTRIES and FSG_HS_FUNCTION_PRE_EP_ENTRIES
 * macros are not defined.
 *
 * When FSG_NO_DEVICE_STRINGS is defined FSG_STRING_MANUFACTURER,
 * FSG_STRING_PRODUCT, FSG_STRING_SERIAL and FSG_STRING_CONFIG are not
 * defined (as well as corresponding entries in string tables are
 * missing) and FSG_STRING_INTERFACE has value of zero.
 *
 * When FSG_NO_OTG is defined fsg_otg_desc won't be defined.
 */

/*
 * When FSG_BUFFHD_STATIC_BUFFER is defined when this file is included
 * the fsg_buffhd structure's buf field will be an array of FSG_BUFLEN
 * characters rather then a pointer to void.
 */

/*
 * When USB_GADGET_DEBUG_FILES is defined the module param num_buffers
 * sets the number of pipeline buffers (length of the fsg_buffhd array).
 * The valid range of num_buffers is: num >= 2 && num <= 4.
 */
/**************************************************************************************************
    问题单号            时间             修改人                      说明
***************************************************************************************************/

#include <linux/usb/storage.h>
#include <scsi/scsi.h>
#include <asm/unaligned.h>

#include "f_mbb_storage.h"
#include "usb_debug.h"
#include "usb_platform_comm.h"
#include "mbb_usb_adp.h"

static struct fsg_lun *fsg_lun_from_dev(struct device *dev)
{
	return container_of(dev, struct fsg_lun, dev);
}

#ifdef CONFIG_USB_GADGET_DEBUG_FILES

static unsigned int fsg_num_buffers = CONFIG_USB_GADGET_STORAGE_NUM_BUFFERS;
//module_param_named(num_buffers, fsg_num_buffers, uint, S_IRUGO);
//MODULE_PARM_DESC(num_buffers, "Number of pipeline buffers");

#else

/*
 * Number of buffers we will use.
 * 2 is usually enough for good buffering pipeline
 */
#define fsg_num_buffers	CONFIG_USB_GADGET_STORAGE_NUM_BUFFERS

#endif /* CONFIG_USB_DEBUG */

/* check if fsg_num_buffers is within a valid range */
static inline int fsg_num_buffers_validate(void)
{
	if (fsg_num_buffers >= 2 && fsg_num_buffers <= 4)
		return 0;
    DBG_E(MBB_MASS, "fsg_num_buffers %u is out of range (%d to %d)\n",
        fsg_num_buffers, 2 ,4);
	return -EINVAL;
}

static inline u32 get_unaligned_be24(u8 *buf)
{
	return 0xffffff & (u32) get_unaligned_be32(buf - 1);
}

enum {
#ifndef FSG_NO_DEVICE_STRINGS
	FSG_STRING_MANUFACTURER	= 1,
	FSG_STRING_PRODUCT,
	FSG_STRING_SERIAL,
	FSG_STRING_CONFIG,
#endif
	FSG_STRING_INTERFACE
};

#ifndef FSG_NO_OTG
static struct usb_otg_descriptor
fsg_otg_desc = {
	.bLength =		sizeof fsg_otg_desc,
	.bDescriptorType =	USB_DT_OTG,

	.bmAttributes =		USB_OTG_SRP,
};
#endif

/* There is only one interface. */

static struct usb_interface_descriptor
fsg_intf_desc = {
	.bLength =		sizeof fsg_intf_desc,
	.bDescriptorType =	USB_DT_INTERFACE,

	.bNumEndpoints =	2,		/* Adjusted during fsg_bind() */
	.bInterfaceClass =	USB_CLASS_MASS_STORAGE,
	.bInterfaceSubClass =	USB_SC_SCSI,	/* Adjusted during fsg_bind() */
	.bInterfaceProtocol =	USB_PR_BULK,	/* Adjusted during fsg_bind() */
	.iInterface =		FSG_STRING_INTERFACE,
};

/*
 * Three full-speed endpoint descriptors: bulk-in, bulk-out, and
 * interrupt-in.
 */

static struct usb_endpoint_descriptor
fsg_fs_bulk_in_desc = {
	.bLength =		USB_DT_ENDPOINT_SIZE,
	.bDescriptorType =	USB_DT_ENDPOINT,

	.bEndpointAddress =	USB_DIR_IN,
	.bmAttributes =		USB_ENDPOINT_XFER_BULK,
	/* wMaxPacketSize set by autoconfiguration */
};

static struct usb_endpoint_descriptor
fsg_fs_bulk_out_desc = {
	.bLength =		USB_DT_ENDPOINT_SIZE,
	.bDescriptorType =	USB_DT_ENDPOINT,

	.bEndpointAddress =	USB_DIR_OUT,
	.bmAttributes =		USB_ENDPOINT_XFER_BULK,
	/* wMaxPacketSize set by autoconfiguration */
};

#ifndef FSG_NO_INTR_EP

static struct usb_endpoint_descriptor
fsg_fs_intr_in_desc = {
	.bLength =		USB_DT_ENDPOINT_SIZE,
	.bDescriptorType =	USB_DT_ENDPOINT,

	.bEndpointAddress =	USB_DIR_IN,
	.bmAttributes =		USB_ENDPOINT_XFER_INT,
	.wMaxPacketSize =	cpu_to_le16(2),
	.bInterval =		32,	/* frames -> 32 ms */
};

#ifndef FSG_NO_OTG
#  define FSG_FS_FUNCTION_PRE_EP_ENTRIES	2
#else
#  define FSG_FS_FUNCTION_PRE_EP_ENTRIES	1
#endif

#endif

static struct usb_descriptor_header *fsg_fs_function[] = {
#ifndef FSG_NO_OTG
	(struct usb_descriptor_header *) &fsg_otg_desc,
#endif
	(struct usb_descriptor_header *) &fsg_intf_desc,
	(struct usb_descriptor_header *) &fsg_fs_bulk_in_desc,
	(struct usb_descriptor_header *) &fsg_fs_bulk_out_desc,
#ifndef FSG_NO_INTR_EP
	(struct usb_descriptor_header *) &fsg_fs_intr_in_desc,
#endif
	NULL,
};


/*
 * USB 2.0 devices need to expose both high speed and full speed
 * descriptors, unless they only run at full speed.
 *
 * That means alternate endpoint descriptors (bigger packets)
 * and a "device qualifier" ... plus more construction options
 * for the configuration descriptor.
 */
static struct usb_endpoint_descriptor
fsg_hs_bulk_in_desc = {
	.bLength =		USB_DT_ENDPOINT_SIZE,
	.bDescriptorType =	USB_DT_ENDPOINT,

	/* bEndpointAddress copied from fs_bulk_in_desc during fsg_bind() */
	.bmAttributes =		USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize =	cpu_to_le16(512),
};

static struct usb_endpoint_descriptor
fsg_hs_bulk_out_desc = {
	.bLength =		USB_DT_ENDPOINT_SIZE,
	.bDescriptorType =	USB_DT_ENDPOINT,

	/* bEndpointAddress copied from fs_bulk_out_desc during fsg_bind() */
	.bmAttributes =		USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize =	cpu_to_le16(512),
	.bInterval =		1,	/* NAK every 1 uframe */
};

#ifndef FSG_NO_INTR_EP

static struct usb_endpoint_descriptor
fsg_hs_intr_in_desc = {
	.bLength =		USB_DT_ENDPOINT_SIZE,
	.bDescriptorType =	USB_DT_ENDPOINT,

	/* bEndpointAddress copied from fs_intr_in_desc during fsg_bind() */
	.bmAttributes =		USB_ENDPOINT_XFER_INT,
	.wMaxPacketSize =	cpu_to_le16(2),
	.bInterval =		9,	/* 2**(9-1) = 256 uframes -> 32 ms */
};

#ifndef FSG_NO_OTG
#  define FSG_HS_FUNCTION_PRE_EP_ENTRIES	2
#else
#  define FSG_HS_FUNCTION_PRE_EP_ENTRIES	1
#endif

#endif

static struct usb_descriptor_header *fsg_hs_function[] = {
#ifndef FSG_NO_OTG
	(struct usb_descriptor_header *) &fsg_otg_desc,
#endif
	(struct usb_descriptor_header *) &fsg_intf_desc,
	(struct usb_descriptor_header *) &fsg_hs_bulk_in_desc,
	(struct usb_descriptor_header *) &fsg_hs_bulk_out_desc,
#ifndef FSG_NO_INTR_EP
	(struct usb_descriptor_header *) &fsg_hs_intr_in_desc,
#endif
	NULL,
};

static struct usb_endpoint_descriptor
fsg_ss_bulk_in_desc = {
	.bLength =		USB_DT_ENDPOINT_SIZE,
	.bDescriptorType =	USB_DT_ENDPOINT,

	/* bEndpointAddress copied from fs_bulk_in_desc during fsg_bind() */
	.bmAttributes =		USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize =	cpu_to_le16(1024),
};

static struct usb_ss_ep_comp_descriptor fsg_ss_bulk_in_comp_desc = {
	.bLength =		sizeof(fsg_ss_bulk_in_comp_desc),
	.bDescriptorType =	USB_DT_SS_ENDPOINT_COMP,

	/*.bMaxBurst =		DYNAMIC, */
};

static struct usb_endpoint_descriptor
fsg_ss_bulk_out_desc = {
	.bLength =		USB_DT_ENDPOINT_SIZE,
	.bDescriptorType =	USB_DT_ENDPOINT,

	/* bEndpointAddress copied from fs_bulk_out_desc during fsg_bind() */
	.bmAttributes =		USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize =	cpu_to_le16(1024),
};

static struct usb_ss_ep_comp_descriptor fsg_ss_bulk_out_comp_desc = {
	.bLength =		sizeof(fsg_ss_bulk_in_comp_desc),
	.bDescriptorType =	USB_DT_SS_ENDPOINT_COMP,

	/*.bMaxBurst =		DYNAMIC, */
};

#ifndef FSG_NO_INTR_EP

static struct usb_endpoint_descriptor
fsg_ss_intr_in_desc = {
	.bLength =		USB_DT_ENDPOINT_SIZE,
	.bDescriptorType =	USB_DT_ENDPOINT,

	/* bEndpointAddress copied from fs_intr_in_desc during fsg_bind() */
	.bmAttributes =		USB_ENDPOINT_XFER_INT,
	.wMaxPacketSize =	cpu_to_le16(2),
	.bInterval =		9,	/* 2**(9-1) = 256 uframes -> 32 ms */
};

static struct usb_ss_ep_comp_descriptor fsg_ss_intr_in_comp_desc = {
	.bLength =		sizeof(fsg_ss_bulk_in_comp_desc),
	.bDescriptorType =	USB_DT_SS_ENDPOINT_COMP,

	.wBytesPerInterval =	cpu_to_le16(2),
};

#ifndef FSG_NO_OTG
#  define FSG_SS_FUNCTION_PRE_EP_ENTRIES	2
#else
#  define FSG_SS_FUNCTION_PRE_EP_ENTRIES	1
#endif

#endif

static __maybe_unused struct usb_ext_cap_descriptor fsg_ext_cap_desc = {
	.bLength =		USB_DT_USB_EXT_CAP_SIZE,
	.bDescriptorType =	USB_DT_DEVICE_CAPABILITY,
	.bDevCapabilityType =	USB_CAP_TYPE_EXT,

	.bmAttributes =		cpu_to_le32(USB_LPM_SUPPORT),
};

static __maybe_unused struct usb_ss_cap_descriptor fsg_ss_cap_desc = {
	.bLength =		USB_DT_USB_SS_CAP_SIZE,
	.bDescriptorType =	USB_DT_DEVICE_CAPABILITY,
	.bDevCapabilityType =	USB_SS_CAP_TYPE,

	/* .bmAttributes = LTM is not supported yet */

	.wSpeedSupported =	cpu_to_le16(USB_LOW_SPEED_OPERATION
		| USB_FULL_SPEED_OPERATION
		| USB_HIGH_SPEED_OPERATION
		| USB_5GBPS_OPERATION),
	.bFunctionalitySupport = USB_LOW_SPEED_OPERATION,
	.bU1devExitLat =	USB_DEFAULT_U1_DEV_EXIT_LAT,
	.bU2DevExitLat =	cpu_to_le16(USB_DEFAULT_U2_DEV_EXIT_LAT),
};

static __maybe_unused struct usb_bos_descriptor fsg_bos_desc = {
	.bLength =		USB_DT_BOS_SIZE,
	.bDescriptorType =	USB_DT_BOS,

	.wTotalLength =		cpu_to_le16(USB_DT_BOS_SIZE
				+ USB_DT_USB_EXT_CAP_SIZE
				+ USB_DT_USB_SS_CAP_SIZE),

	.bNumDeviceCaps =	2,
};

static struct usb_descriptor_header *fsg_ss_function[] = {
#ifndef FSG_NO_OTG
	(struct usb_descriptor_header *) &fsg_otg_desc,
#endif
	(struct usb_descriptor_header *) &fsg_intf_desc,
	(struct usb_descriptor_header *) &fsg_ss_bulk_in_desc,
	(struct usb_descriptor_header *) &fsg_ss_bulk_in_comp_desc,
	(struct usb_descriptor_header *) &fsg_ss_bulk_out_desc,
	(struct usb_descriptor_header *) &fsg_ss_bulk_out_comp_desc,
#ifndef FSG_NO_INTR_EP
	(struct usb_descriptor_header *) &fsg_ss_intr_in_desc,
	(struct usb_descriptor_header *) &fsg_ss_intr_in_comp_desc,
#endif
	NULL,
};

/* Maxpacket and other transfer characteristics vary by speed. */
static __maybe_unused struct usb_endpoint_descriptor *
fsg_ep_desc(struct usb_gadget *g, struct usb_endpoint_descriptor *fs,
		struct usb_endpoint_descriptor *hs,
		struct usb_endpoint_descriptor *ss)
{
	if (gadget_is_superspeed(g) && g->speed == USB_SPEED_SUPER)
		return ss;
	else if (gadget_is_dualspeed(g) && g->speed == USB_SPEED_HIGH)
		return hs;
	return fs;
}


/* Static strings, in UTF-8 (for simplicity we use only ASCII characters) */
static struct usb_string		fsg_strings[] = {
#ifndef FSG_NO_DEVICE_STRINGS
	{FSG_STRING_MANUFACTURER,	fsg_string_manufacturer},
	{FSG_STRING_PRODUCT,		fsg_string_product},
	{FSG_STRING_SERIAL,		""},
	{FSG_STRING_CONFIG,		fsg_string_config},
#endif
	{FSG_STRING_INTERFACE,		fsg_string_interface},
	{}
};

static struct usb_gadget_strings	fsg_stringtab = {
	.language	= 0x0409,		/* en-us */
	.strings	= fsg_strings,
};

 /*-------------------------------------------------------------------------*/

/*
 * If the next two routines are called while the gadget is registered,
 * the caller must own fsg->filesem for writing.
 */

extern u32 ptable_get_cdromiso_capacity(void);

static int fsg_lun_open(struct fsg_lun *curlun, const char *filename)
{
	int				ro;
	struct file			*filp = NULL;
	int				rc = -EINVAL;
	struct inode			*inode = NULL;
	loff_t				size;
	loff_t				num_sectors;
	loff_t				min_sectors;

	/* R/W if we can, R/O if we must */
	ro = curlun->initially_ro;
	if (!ro) {
		filp = filp_open(filename, O_RDWR | O_LARGEFILE, 0);
		if (PTR_ERR(filp) == -EROFS || PTR_ERR(filp) == -EACCES)
			ro = 1;
	}
	if (ro)
		filp = filp_open(filename, O_RDONLY | O_LARGEFILE, 0);
	if (IS_ERR(filp)) {
        DBG_W(MBB_MASS, "%s unable to open backing file: %s\n",
            curlun->dev.kobj.name, filename);
		return PTR_ERR(filp);
	}

	if (!(filp->f_mode & FMODE_WRITE))
		ro = 1;

	if (filp->f_path.dentry)
		inode = filp->f_path.dentry->d_inode;
	if (!inode || (!S_ISREG(inode->i_mode) && !S_ISBLK(inode->i_mode))) {
        DBG_W(MBB_MASS, "%s invalid file type: %s\n",
            curlun->dev.kobj.name, filename);
		goto out;
	}

	/*
	 * If we can't read the file, it's no good.
	 * If we can't write the file, use it read-only.
	 */
	if (!filp->f_op || !(filp->f_op->read || filp->f_op->aio_read)) {
        DBG_W(MBB_MASS, "%s file not readable: %s\n",
            curlun->dev.kobj.name, filename);
		goto out;
	}
	if (!(filp->f_op->write || filp->f_op->aio_write))
		ro = 1;

	size = i_size_read(inode->i_mapping->host);
	if (size < 0) {
        DBG_W(MBB_MASS, "%s unable to find file size: %s\n",
            curlun->dev.kobj.name, filename);

		rc = (int) size;
		goto out;
	}

	if (curlun->cdrom) {
        size = ptable_get_cdromiso_capacity();
		curlun->blksize = 2048;
		curlun->blkbits = 11;
	} else if (inode->i_bdev) {
		curlun->blksize = bdev_logical_block_size(inode->i_bdev);
		curlun->blkbits = blksize_bits(curlun->blksize);
	} else {
		curlun->blksize = 512;
		curlun->blkbits = 9;
	}

	num_sectors = size >> curlun->blkbits; /* File size in logic-block-size blocks */
	min_sectors = 1;
	if (curlun->cdrom) {
		min_sectors = 300;	/* Smallest track is 300 frames */
		if (num_sectors >= 256*60*75) {
			num_sectors = 256*60*75 - 1;
            DBG_W(MBB_MASS, "%s file too big: %s\n",
                curlun->dev.kobj.name, filename);
            DBG_W(MBB_MASS, "%s using only first %d blocks\n",
                curlun->dev.kobj.name, (int) num_sectors);
		}
	}
	if (num_sectors < min_sectors) {
        DBG_E(MBB_MASS, "%s file too small: %s\n",
            curlun->dev.kobj.name, filename);

		rc = -ETOOSMALL;
		goto out;
	}

	get_file(filp);
	curlun->ro = ro;
	curlun->filp = filp;
	curlun->file_length = size;
	curlun->num_sectors = num_sectors;
    DBG_I(MBB_MASS, "%s open backing file: %s\n",
        curlun->dev.kobj.name, filename);
	rc = 0;

out:
	filp_close(filp, current->files);
	return rc;
}

static void fsg_lun_close(struct fsg_lun *curlun)
{
	if (curlun->filp) {
        DBG_I(MBB_MASS, "%s close backing file\n",
            curlun->dev.kobj.name);
		fput(curlun->filp);
		curlun->filp = NULL;
	}
}

/*-------------------------------------------------------------------------*/

/*
 * Sync the file data, don't bother with the metadata.
 * This code was copied from fs/buffer.c:sys_fdatasync().
 */
static int fsg_lun_fsync_sub(struct fsg_lun *curlun)
{
	struct file	*filp = curlun->filp;

	if (curlun->ro || !filp)
		return 0;
	return vfs_fsync(filp, 1);
}

static void store_cdrom_address(u8 *dest, int msf, u32 addr)
{
	if (msf) {
		/* Convert to Minutes-Seconds-Frames */
		addr >>= 2;		/* Convert to 2048-byte frames */
		addr += 2*75;		/* Lead-in occupies 2 seconds */
		dest[3] = addr % 75;	/* Frames */
		addr /= 75;
		dest[2] = addr % 60;	/* Seconds */
		addr /= 60;
		dest[1] = addr;		/* Minutes */
		dest[0] = 0;		/* Reserved */
	} else {
		/* Absolute sector */
		put_unaligned_be32(addr, dest);
	}
}

/*-------------------------------------------------------------------------*/

static ssize_t fsg_show_ro(struct device *dev, struct device_attribute *attr,
			   char *buf)
{
	struct fsg_lun	*curlun = fsg_lun_from_dev(dev);

	return snprintf(buf,MAX_SHOW_LEN, "%d\n", fsg_lun_is_open(curlun)
				  ? curlun->ro
				  : curlun->initially_ro);
}

static ssize_t fsg_show_nofua(struct device *dev, struct device_attribute *attr,
			      char *buf)
{
	struct fsg_lun	*curlun = fsg_lun_from_dev(dev);

	return snprintf(buf,MAX_SHOW_LEN, "%u\n", curlun->nofua);
}

static ssize_t fsg_show_file(struct device *dev, struct device_attribute *attr,
			     char *buf)
{
	struct fsg_lun	*curlun = fsg_lun_from_dev(dev);
	struct rw_semaphore	*filesem = dev_get_drvdata(dev);
	char		*p;
	ssize_t		rc;

	down_read(filesem);
	if (fsg_lun_is_open(curlun)) {	/* Get the complete pathname */
		p = d_path(&curlun->filp->f_path, buf, PAGE_SIZE - 1);
		if (IS_ERR(p))
			rc = PTR_ERR(p);
		else {
			rc = strlen(p);
			memmove(buf, p, rc);
			buf[rc] = '\n';		/* Add a newline */
			buf[++rc] = 0;
		}
	} else {				/* No file, return 0 bytes */
		*buf = 0;
		rc = 0;
	}
	up_read(filesem);
	return rc;
}

static ssize_t fsg_show_mode(struct device *dev, struct device_attribute *attr,
			   char *buf)
{
	struct fsg_lun	*curlun = fsg_lun_from_dev(dev);

	return snprintf(buf,MAX_SHOW_LEN, "%d\n", curlun->mode);
}

static ssize_t fsg_store_ro(struct device *dev, struct device_attribute *attr,
			    const char *buf, size_t count)
{
	ssize_t		rc;
	struct fsg_lun	*curlun = fsg_lun_from_dev(dev);
	struct rw_semaphore	*filesem = dev_get_drvdata(dev);
	unsigned	ro;

	rc = kstrtouint(buf, 2, &ro);
	if (rc)
		return rc;

	/*
	 * Allow the write-enable status to change only while the
	 * backing file is closed.
	 */
	down_read(filesem);
	if (fsg_lun_is_open(curlun)) {
        DBG_T(MBB_MASS, "%s read-only status change prevented\n",
            curlun->dev.kobj.name);
		rc = -EBUSY;
	} else {
		curlun->ro = ro;
		curlun->initially_ro = ro;
        DBG_T(MBB_MASS, "%s read-only status set to %d\n",
            curlun->dev.kobj.name, curlun->ro);
		rc = count;
	}
	up_read(filesem);
	return rc;
}

static ssize_t fsg_store_nofua(struct device *dev,
			       struct device_attribute *attr,
			       const char *buf, size_t count)
{
	struct fsg_lun	*curlun = fsg_lun_from_dev(dev);
	unsigned	nofua;
	int		ret;

	ret = kstrtouint(buf, 2, &nofua);
	if (ret)
		return ret;

	/* Sync data when switching from async mode to sync */
	if (!nofua && curlun->nofua)
		fsg_lun_fsync_sub(curlun);

	curlun->nofua = nofua;

	return count;
}

static ssize_t fsg_store_file(struct device *dev, struct device_attribute *attr,
			      const char *buf, size_t count)
{
	struct fsg_lun	*curlun = fsg_lun_from_dev(dev);
	struct rw_semaphore	*filesem = dev_get_drvdata(dev);
	int		rc = 0;
    USB_INT power_off   = MBB_USB_NO;
    USB_INT fast_off    = MBB_USB_NO;

    power_off   = usb_power_off_chg_stat();
    fast_off    = usb_fast_on_off_stat();

    if (MBB_USB_YES == power_off || MBB_USB_YES == fast_off)//如果是假关机或者关机状态，直接返回
    {
        DBG_T(MBB_MASS, "power_off status\n");
        return -EBUSY;
    }
	if (curlun->prevent_medium_removal && fsg_lun_is_open(curlun)) {
        DBG_I(MBB_MASS, "%s eject attempt prevented\n\n",
            curlun->dev.kobj.name);
		return -EBUSY;				/* "Door is locked" */
	}

	/* Remove a trailing newline */
	if (count > 0 && buf[count-1] == '\n')
		((char *) buf)[count-1] = 0;		/* Ugh! */

	/* Eject current medium */
	down_write(filesem);
	if (fsg_lun_is_open(curlun)) {
		fsg_lun_close(curlun);
		curlun->unit_attention_data = SS_MEDIUM_NOT_PRESENT;
	}

	/* Load new medium */
	if (count > 0 && buf[0]) {
		rc = fsg_lun_open(curlun, buf);
		if (rc == 0)
			curlun->unit_attention_data =
					SS_NOT_READY_TO_READY_TRANSITION;
	}
	up_write(filesem);
	return (rc < 0 ? rc : count);
}

static ssize_t fsg_store_mode(struct device *dev,
			       struct device_attribute *attr,
			       const char *buf, size_t count)
{
	struct fsg_lun	*curlun = fsg_lun_from_dev(dev);
	struct rw_semaphore	*filesem = dev_get_drvdata(dev);
	char sdpath[30];
	unsigned	mode;
	int status;
	int idex;
	int		ret;

	ret = kstrtouint(buf, 2, &mode);
	if (ret)
		return ret;
	/* Eject current medium */
	if (curlun->cdrom) {
		goto out;
	}
	down_write(filesem);
	
	/*web mode close mass storage*/
	if (mode) {
		if (fsg_lun_is_open(curlun)) {
			d_path(&curlun->filp->f_path,
				   sdpath, 30);
			fsg_lun_close(curlun);
			curlun->unit_attention_data = SS_MEDIUM_NOT_PRESENT;
		}
	} else {
		status = usb_get_mmc_status();
		if (status) {
			idex = usb_get_mmc_dev_idex();
            if(idex > -1)
            {
    			snprintf(sdpath, 30, "/dev/block/mmcblk%dp1", idex);
            }
            else
            {
                DBG_T(MBB_MASS, "%s usb_get_mmc_dev_idex return err\n",__func__);
            }
			ret = fsg_lun_open(curlun, sdpath);
			if (ret) {
				snprintf(sdpath, 30, "/dev/block/mmcblk%d", idex);
				ret = fsg_lun_open(curlun, sdpath);
			}
			if (ret == 0)
				curlun->unit_attention_data =
						SS_NOT_READY_TO_READY_TRANSITION;	
		}
	}
	
	up_write(filesem);

out:
	curlun->mode = mode;

	return count;
}
