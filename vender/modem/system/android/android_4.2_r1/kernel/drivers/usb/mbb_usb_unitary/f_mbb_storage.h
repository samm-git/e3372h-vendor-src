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
/**************************************************************************************************
    问题单号          时间           修改人                      说明
***************************************************************************************************/

#ifndef _F_MASS_STORAGE_MBB_H
#define _F_MASS_STORAGE_MBB_H
/*
 * Thanks to NetChip Technologies for donating this product ID.
 *
 * DO NOT REUSE THESE IDs with any other driver!!  Ever!!
 * Instead:  allocate your own, using normal USB-IF procedures.
 */

#include <linux/usb/composite.h>

#define FSG_VENDOR_ID	0x0525	/* NetChip */
#define FSG_PRODUCT_ID	0xa4a5	/* Linux-USB File-backed Storage Gadget */

/*-------------------------------------------------------------------------*/

#ifndef DEBUG
#undef VERBOSE_DEBUG
#undef DUMP_MSGS
#endif /* !DEBUG */

#ifdef VERBOSE_DEBUG
#define VLDBG	LDBG
#endif /* VERBOSE_DEBUG */
#define LDBG(lun, fmt, args...)   dev_dbg (&(lun)->dev, fmt, ## args)
#define LERROR(lun, fmt, args...) dev_err (&(lun)->dev, fmt, ## args)
#define LWARN(lun, fmt, args...)  dev_warn(&(lun)->dev, fmt, ## args)
#define LINFO(lun, fmt, args...)  dev_info(&(lun)->dev, fmt, ## args)

/*
 * Keep those macros in sync with those in
 * include/linux/usb/composite.h or else GCC will complain.  If they
 * are identical (the same names of arguments, white spaces in the
 * same places) GCC will allow redefinition otherwise (even if some
 * white space is removed or added) warning will be issued.
 *
 * Those macros are needed here because File Storage Gadget does not
 * include the composite.h header.  For composite gadgets those macros
 * are redundant since composite.h is included any way.
 *
 * One could check whether those macros are already defined (which
 * would indicate composite.h had been included) or not (which would
 * indicate we were in FSG) but this is not done because a warning is
 * desired if definitions here differ from the ones in composite.h.
 *
 * We want the definitions to match and be the same in File Storage
 * Gadget as well as Mass Storage Function (and so composite gadgets
 * using MSF).  If someone changes them in composite.h it will produce
 * a warning in this file when building MSF.
 */

#define ERROR(d, fmt, args...)   dev_err(&(d)->gadget->dev , fmt , ## args)
#define WARNING(d, fmt, args...) dev_warn(&(d)->gadget->dev , fmt , ## args)
#define INFO(d, fmt, args...)    dev_info(&(d)->gadget->dev , fmt , ## args)

#ifdef DUMP_MSGS

#  define dump_msg(fsg, /* const char * */ label,			\
                   /* const u8 * */ buf, /* unsigned */ length) do {	\
    if (length < 512) {						\
        DBG(fsg, "%s, length %u:\n", label, length);		\
        print_hex_dump(KERN_DEBUG, "", DUMP_PREFIX_OFFSET,	\
                       16, 1, buf, length, 0);			\
    }								\
} while (0)

#  define dump_cdb(fsg) do { } while (0)

#else

#  define dump_msg(fsg, /* const char * */ label, \
                   /* const u8 * */ buf, /* unsigned */ length) do { } while (0)

#  ifdef VERBOSE_DEBUG

#    define dump_cdb(fsg)						\
    print_hex_dump(KERN_DEBUG, "SCSI CDB: ", DUMP_PREFIX_NONE,	\
                   16, 1, (fsg)->cmnd, (fsg)->cmnd_size, 0)		\
     
#  else

#    define dump_cdb(fsg) do { } while (0)

#  endif /* VERBOSE_DEBUG */

#endif /* DUMP_MSGS */

/*-------------------------------------------------------------------------*/

/* CBI Interrupt data structure */
struct interrupt_data
{
    u8	bType;
    u8	bValue;
};

#define CBI_INTERRUPT_DATA_LEN		2

/* CBI Accept Device-Specific Command request */
#define USB_CBI_ADSC_REQUEST		0x00

/* Length of a SCSI Command Data Block */
#define MAX_COMMAND_SIZE	16

/* SCSI Sense Key/Additional Sense Code/ASC Qualifier values */
#define SS_NO_SENSE				0
#define SS_COMMUNICATION_FAILURE		            0x040800
#define SS_INVALID_COMMAND			                0x052000
#define SS_INVALID_FIELD_IN_CDB			            0x052400
#define SS_LOGICAL_BLOCK_ADDRESS_OUT_OF_RANGE	    0x052100
#define SS_LOGICAL_UNIT_NOT_SUPPORTED		        0x052500
#define SS_MEDIUM_NOT_PRESENT			            0x023a00
#define SS_MEDIUM_REMOVAL_PREVENTED		            0x055302
#define SS_NOT_READY_TO_READY_TRANSITION	        0x062800
#define SS_RESET_OCCURRED			                0x062900
#define SS_SAVING_PARAMETERS_NOT_SUPPORTED	        0x053900
#define SS_UNRECOVERED_READ_ERROR		            0x031100
#define SS_WRITE_ERROR				                0x030c02
#define SS_WRITE_PROTECTED			                0x072700

#define SK(x)		((u8) ((x) >> 16))	/* Sense Key byte, etc. */
#define ASC(x)		((u8) ((x) >> 8))
#define ASCQ(x)		((u8) (x))


#define Unit_Serial_Number_Page         0x80
#define Enable_Vital_Product_Data       0X01
#define Currentlun_Connect_OR_Unable_To_Determine 000<<5
#define RESERVED 0X00
#define INQUIRY_PAGE_LENGTH 16
#define Unit_Serial_Number_Page_Length 21
/*-------------------------------------------------------------------------*/
typedef enum _sd_card_remove_attach
{
    SD_CARD_NOT_PRESENT = 0,
    SD_CARD_PRESENT ,
} sd_card_remove_attach;

typedef enum _webnas_sd_workmode_
{
    SD_WORKMODE_WEBNAS = 0,      //WEB共享模式
    SD_WORKMODE_MASS = 1,        //U 盘模式
    SD_WORKMODE_SAMBA ,          //SAMBA模式
    SD_WORKMODE_DLNA ,           //DLNA 模式
    SD_WORKMODE_BUFF,
} WEBNAS_SD_WORKMODE;

typedef enum _mass_lun_info
{
    /*lun type*/
    LUN_NONE    = 0x00,/*0x00*/
    LUN_CD      = 0x01,/*0x01*/
    LUN_SD      = 0x02,/*0x02*/
    LUN_INVALID = 0x03,/*0x03*/
} mass_lun_info;

typedef struct _sd_card_info
{
    sd_card_remove_attach   sd_card_status;
    WEBNAS_SD_WORKMODE      sd_card_workmode;
} sd_card_info;

#define SD_CARD_WEB_MODE    (0)
#define SD_CARD_USB_MODE    (1)

struct fsg_lun
{
    struct file*	filp;
    loff_t		file_length;
    loff_t		num_sectors;

    unsigned int	initially_ro: 1;
    unsigned int	ro: 1;
    unsigned int	removable: 1;
    unsigned int	cdrom: 1;
    unsigned int	prevent_medium_removal: 1;
    unsigned int	registered: 1;
    unsigned int	info_valid: 1;
    unsigned int	nofua: 1;
    unsigned int 	mode; /*for web or usb*/
    unsigned int    keep_eject: 1;
    unsigned int    keep_no_medium_state: 1;
    u32		sense_data;
    u32		sense_data_info;
    u32		unit_attention_data;

    unsigned int	blkbits;	/* Bits of logical block size of bound block device */
    unsigned int	blksize;	/* logical block size of bound block device */
    struct device	dev;

    char inquiry_string[USB_NUM_8 + USB_NUM_16 + USB_NUM_4 + 1];
};

#define fsg_lun_is_open(curlun)	((curlun)->filp != NULL)

/* Big enough to hold our biggest descriptor */
#define EP0_BUFSIZE	256
#define DELAYED_STATUS	(EP0_BUFSIZE + 999)	/* An impossibly large value */
/* Default size of buffer length. */
#define FSG_BUFLEN	((u32)16384)

/* Maximal number of LUNs supported in mass storage function */
#define FSG_MAX_LUNS	8

enum fsg_buffer_state
{
    BUF_STATE_EMPTY = 0,
    BUF_STATE_FULL,
    BUF_STATE_BUSY
};

struct fsg_buffhd
{
#ifdef FSG_BUFFHD_STATIC_BUFFER
    char				buf[FSG_BUFLEN];
#else
    void*				buf;
#endif
    enum fsg_buffer_state		state;
    struct fsg_buffhd*		next;

    /*
     * The NetChip 2280 is faster, and handles some protocol faults
     * better, if we don't submit any short bulk-out read requests.
     * So we will record the intended request length here.
     */
    unsigned int			bulk_out_intended_length;

    struct usb_request*		inreq;
    int				inreq_busy;
    struct usb_request*		outreq;
    int				outreq_busy;
};

enum fsg_state
{
    /* This one isn't used anywhere */
    FSG_STATE_COMMAND_PHASE = -10,
    FSG_STATE_DATA_PHASE,
    FSG_STATE_STATUS_PHASE,

    FSG_STATE_IDLE = 0,
    FSG_STATE_ABORT_BULK_OUT,
    FSG_STATE_RESET,
    FSG_STATE_INTERFACE_CHANGE,
    FSG_STATE_CONFIG_CHANGE,
    FSG_STATE_DISCONNECT,
    FSG_STATE_EXIT,
    FSG_STATE_TERMINATED
};

enum data_direction
{
    DATA_DIR_UNKNOWN = 0,
    DATA_DIR_FROM_HOST,
    DATA_DIR_TO_HOST,
    DATA_DIR_NONE
};

/*------------------------------------------------------------------------*/
#define FSG_NO_DEVICE_STRINGS    1
#define FSG_NO_OTG               1
#define FSG_NO_INTR_EP           1

#define CDROM_MNT_DIR    "/root/ISO"
#define SDCARD_MNT_DIR   "/dev/block/mmcblk0"

/*-------------------------------------------------------------------------*/

struct fsg_dev;
struct fsg_common;

struct fsg_state_count
{
    unsigned int command_phase;
    unsigned int data_phase;
    unsigned int status_phase;
    unsigned int idle;
    unsigned int abort_bulk_out;
    unsigned int reset;
    unsigned int interface_change;
    unsigned int config_change;
    unsigned int disconnect;
    unsigned int exit;
    unsigned int terminated;
    unsigned int fsg_free;
    unsigned int phase_error;
    unsigned int command_failure;
    unsigned int cbw;
    unsigned int invalid_cbw;
    unsigned int non_meaningful_cbw;
    unsigned int lun_values_not_consistent;
    unsigned int unknown_cbw;
    unsigned int invalid_csw;
    unsigned int non_meaningful_csw;
    unsigned int data;
    unsigned int csw;
    unsigned int bulk_in_set_halt;
    unsigned int bulk_out_set_halt;
    unsigned int bulk_in_set_wedge;
    unsigned int signal_pending;
    unsigned int signal_pending_line;
    unsigned int short_packet_received;
    unsigned int throw_away_data_err;
};

/* FSF callback functions */
struct fsg_operations
{
    /*
     * Callback function to call when thread exits.  If no
     * callback is set or it returns value lower then zero MSF
     * will force eject all LUNs it operates on (including those
     * marked as non-removable or with prevent_medium_removal flag
     * set).
     */
    int (*thread_exits)(struct fsg_common* common);

    /*
     * Called prior to ejection.  Negative return means error,
     * zero means to continue with ejection, positive means not to
     * eject.
     */
    int (*pre_eject)(struct fsg_common* common,
                     struct fsg_lun* lun, int num);
    /*
     * Called after ejection.  Negative return means error, zero
     * or positive is just a success.
     */
    int (*post_eject)(struct fsg_common* common,
                      struct fsg_lun* lun, int num);
};

/* Data shared by all the FSG instances. */
struct fsg_common
{
    struct usb_gadget*	gadget;
    struct usb_composite_dev* cdev;
    struct fsg_dev*		fsg, *new_fsg;
    wait_queue_head_t	fsg_wait;
    /* filesem protects: backing files in use */
    struct rw_semaphore	filesem;

    /* lock protects: state, all the req_busy's */
    spinlock_t		lock;

    struct usb_ep*		ep0;		/* Copy of gadget->ep0 */
    struct usb_request*	ep0req;	/* Copy of cdev->req */
    unsigned int		ep0_req_tag;

    struct fsg_buffhd*	next_buffhd_to_fill;
    struct fsg_buffhd*	next_buffhd_to_drain;
    struct fsg_buffhd*	buffhds;

    int			cmnd_size;
    u8			cmnd[MAX_COMMAND_SIZE];
    USB_INT         cmnd_return_zero;
    unsigned int		nluns;
    unsigned int		lun;
    struct fsg_lun*		luns;
    struct fsg_lun*		curlun;

    unsigned int		bulk_out_maxpacket;
    enum fsg_state		state;		/* For exception handling */
    unsigned int		exception_req_tag;

    enum data_direction	data_dir;
    u32			data_size;
    u32			data_size_from_cmnd;
    u32			tag;
    u32			residue;
    u32			usb_amount_left;

    unsigned int		can_stall: 1;
    unsigned int		free_storage_on_release: 1;
    unsigned int		phase_error: 1;
    unsigned int		short_packet_received: 1;
    unsigned int		bad_lun_okay: 1;
    unsigned int		running: 1;

    int			thread_wakeup_needed;
    struct completion	thread_notifier;
    struct task_struct*	thread_task;

    struct work_struct open_nluns_work;

    /* Callback functions. */
    const struct fsg_operations*	ops;
    /* Gadget's private data. */
    void*			private_data;

    /*
     * Vendor (8 chars), product (16 chars), release (4
     * hexadecimal digits) and NUL byte
     */
    char inquiry_string[8 + 16 + 4 + 1];

    struct kref		ref;
};

struct fsg_config
{
    unsigned nluns;
    struct fsg_lun_config
    {
        const char* filename;
        char ro;
        char removable;
        char cdrom;
        char nofua;
        char mode;
    } luns[FSG_MAX_LUNS];

    const char*		lun_name_format;
    const char*		thread_name;

    /* Callback functions. */
    const struct fsg_operations*	ops;
    /* Gadget's private data. */
    void*			private_data;

    const char* vendor_name;		/*  8 characters or less */
    const char* product_name;		/* 16 characters or less */
    u16 release;

    char			can_stall;
};

struct fsg_dev
{
    struct usb_function	function;
    struct usb_gadget*	gadget;	/* Copy of cdev->gadget */
    struct fsg_common*	common;

    u16			interface_number;

    unsigned int		bulk_in_enabled: 1;
    unsigned int		bulk_out_enabled: 1;

    unsigned long		atomic_bitflags;
#define IGNORE_BULK_OUT		0

    struct usb_ep*		bulk_in;
    struct usb_ep*		bulk_out;
};

/*mass 可维可测数据结构*/
struct mass_storage_function_config_data
{
    USB_VOID* parent;
    struct fsg_config       fsgconfig;
    struct fsg_state_count  fsg_state;
    struct fsg_common*      common;
};


#define GET_CONFIGURATION_SCSI 0x46
#define MSF_ENABLE        2

/* 对READ_TOC这个SCSI命令类型的定义 */
typedef  struct _usbsdms_read_toc_cmd_type
{
    u8  op_code;
    u8  msf;
    u8  format;
    u8  reserved1;
    u8  reserved2;
    u8  reserved3;
    u8  session_num;
    u8  allocation_length_msb;
    u8  allocation_length_lsb;
    u8  control;
} __attribute__ ((packed))usbsdms_read_toc_cmd_type;

typedef  struct _usbsdms_get_configuration_cmd_type
{
    u8    op_code;
    u8    rt;
    u16  feature_num;
    u8    reserved1;
    u8    reserved2;
    u8    reserved3;
    u16  allocation_length;
    u8    control;
} __attribute__ ((packed))usbsdms_get_configuration_cmd_type;

/*
 * MAC系统上read toc lead-out 地址计算错误， 从之前代码中移植相应数据结构及计算方法
 * 来解决mac系统上scsi命令错误的问题。
 */
#define SESSION_PARAM_SIZE 11

typedef union mmc_addr
{
    u32 lba;
    struct
    {
        u8 reserved;
        u8 min;
        u8 sec;
        u8 frame;
    } __attribute__ ((packed)) msf;
} __attribute__ ((packed)) mmc_addr_t;

typedef struct scsi_toc
{
    u16 data_len;
    u8 first_track;
    u8 last_track;
    u8 params[1];
} __attribute__ ((packed)) scsi_toc_t;

typedef struct track_param
{
    u8 reserved_1;
    u8 adr_control;
    u8 track_number;
    u8 reserved_2;
    mmc_addr_t addr;
} __attribute__ ((packed)) track_param_t;

typedef struct
{
    u8 session_num;
    u8 adr_control;
    u8 tno;
    u8 point;
    u8 min;
    u8 sec;
    u8 frame;
    mmc_addr_t addr;
} __attribute__ ((packed)) session_param_t;

/************************* Module parameters *************************/

struct fsg_module_parameters
{
    char*		file[FSG_MAX_LUNS];
    bool		ro[FSG_MAX_LUNS];
    bool		removable[FSG_MAX_LUNS];
    bool		cdrom[FSG_MAX_LUNS];
    bool		nofua[FSG_MAX_LUNS];
    bool		mode[FSG_MAX_LUNS];
    unsigned int	file_count, ro_count, removable_count, cdrom_count;
    unsigned int	nofua_count;
    unsigned int	luns;	/* nluns */
    bool		stall;	/* can_stall */
};

#define _FSG_MODULE_PARAM_ARRAY(prefix, params, name, type, desc)	\
    module_param_array_named(prefix ## name, params.name, type,	\
                             &prefix ## params.name ## _count,	\
                             S_IRUGO);				\
    MODULE_PARM_DESC(prefix ## name, desc)

#define _FSG_MODULE_PARAM(prefix, params, name, type, desc)		\
    module_param_named(prefix ## name, params.name, type,		\
                       S_IRUGO);					\
    MODULE_PARM_DESC(prefix ## name, desc)

#define FSG_MODULE_PARAMETERS(prefix, params)				\
    _FSG_MODULE_PARAM_ARRAY(prefix, params, file, charp,		\
                            "names of backing files or devices");	\
    _FSG_MODULE_PARAM_ARRAY(prefix, params, ro, bool,		\
                            "true to force read-only");		\
    _FSG_MODULE_PARAM_ARRAY(prefix, params, removable, bool,	\
                            "true to simulate removable media");	\
    _FSG_MODULE_PARAM_ARRAY(prefix, params, cdrom, bool,		\
                            "true to simulate CD-ROM instead of disk"); \
    _FSG_MODULE_PARAM_ARRAY(prefix, params, nofua, bool,		\
                            "true to ignore SCSI WRITE(10,12) FUA bit"); \
    _FSG_MODULE_PARAM(prefix, params, luns, uint,			\
                      "number of LUNs");				\
    _FSG_MODULE_PARAM(prefix, params, stall, bool,			\
                      "false to prevent bulk stalls")

#endif /* _F_MASS_STORAGE_MBB_H */

