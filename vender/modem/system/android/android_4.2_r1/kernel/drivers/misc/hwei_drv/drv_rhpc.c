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
#include <linux/jiffies.h>
#include <linux/semaphore.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/gpio.h>
#include <linux/netlink.h>
#include <linux/drv_rhpc_ext_api.h>
#include "bsp_nvim.h"
#include "drv_nvim.h"

#include "kerneldev.h"

//#include "platform_api_cpe.h"

#include "bsp_version.h"


#if 0
#include "../../led_drv/balong_led_drv.h"
#endif

#include "drv_rhpc.h"
#include "drv_sci.h"

#if 0
#include "../../../arch/arm/mach-balong/mmi.h"
#endif

#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/kthread.h>

#define AT_FACTORY_INFO_LEN             (78)
#define AT_MMI_TEST_FLAG_OFFSET         (24)
#define AT_MMI_TEST_FLAG_LEN            (4)
#define AT_MMI_TEST_FLAG_SUCC           "ST P"
#define en_NV_Item_Factory_Info			(114)

//extern BSP_U32 NVM_Write(BSP_U32 ulid, BSP_VOID * pdata, BSP_U32 usdatalen);


/*extern unsigned int update_getModemUpgFlag(void);*/

//桩函数，等待移植
unsigned int update_getModemUpgFlag(void)
{
    return 0;
}

#define DRV_RHPC_DEBUG

/*Global Variable*/
#if 0
/*wink on time*/
static unsigned int g_ledWinkOnTime = 200;
/*wink off time*/
static unsigned int g_ledWinkOffTime = 300;
#endif

static drv_rhpc_dfou_reboot_func drv_rhpc_dfou_reboot = NULL;
static drv_rhpc_at_resume_default_func drv_rhpc_at_resume_default = NULL;
static int g_drv_rhpc_datalock_status = 1;//datalock defaultValue
/*Forward Declaration.*/
static int drv_rhpc_dfu_mreboot_handle(drv_rhpc_data_t *data, void *rsp_buf, int *rsp_len);
#if 0
static int drv_rhpc_gpio_handle(drv_rhpc_data_t *data, void *rsp_buf, int *rsp_len);
#endif
static int drv_rhpc_rhwver_get_handle(drv_rhpc_data_t *data, void *rsp_buf, int *rsp_len);

static int drv_rhpc_router_get_datalock_handle(drv_rhpc_data_t *data, void *rsp_buf, int *rsp_len);
static int drv_rhpc_router_set_customize_item_to_default_handle(drv_rhpc_data_t *data, void *rsp_buf, int *rsp_len);

static int drv_rhpc_router_write_mmi_nv_handle(drv_rhpc_data_t *data, void *rsp_buf, int *rsp_len);
static int drv_rhpc_router_clear_mmi_nv_handle(drv_rhpc_data_t *data, void *rsp_buf, int *rsp_len);

static int drv_rhpc_router_get_upg_status(drv_rhpc_data_t *data, void *rsp_buf, int *rsp_len);

static int
drv_rhpc_modem_led_status_set(drv_rhpc_data_t *data, void *rsp_buf, int *rsp_len);

//extern BSP_U32 NVM_Read(BSP_U16 usID,BSP_VOID *pItem,BSP_U32 ulLength);
#define EN_NV_ITEM_IMEI    (0)
#define PH_IMEI_LEN    (16)
#define NV_OK (0)



/*Global Data*/
static struct task_struct *g_startup_status_detect_thread;

static drv_rhpc_msg_hdlr_func drv_rhpc_r2m_msg_hdlr[DRV_RHPC_REQ_R2M_CMD_MAX_IDX] =
{
    /*[DRV_RHPC_REQ_GPIO_DIR_SET_CMD ... \
    DRV_RHPC_REQ_GPIO_OUTPUT_LEVEL_SET_CMD]  = drv_rhpc_gpio_handle,*/
    [DRV_RHPC_REQ_ONLINEUPDATE_REBOOT_CMD]   = drv_rhpc_dfu_mreboot_handle,
    [DRV_RHPC_REQ_ROUTER_HWVER_GET_CMD]      = drv_rhpc_rhwver_get_handle,
    [DRV_RHPC_REQ_R2M_CMD_GET_DATALOCK_STATUS_CMD]      = drv_rhpc_router_get_datalock_handle,
    [DRV_RHPC_REQ_R2M_CMD_SET_CUSTOMIZE_ITEM_TO_DEFAULT_CMD]      = drv_rhpc_router_set_customize_item_to_default_handle,

    [DRV_RHPC_REQ_R2M_CMD_WRITE_MMI_NV_FLAG_CMD]      = drv_rhpc_router_write_mmi_nv_handle,
    [DRV_RHPC_REQ_R2M_CMD_CLEAR_MMI_NV_CMD]      = drv_rhpc_router_clear_mmi_nv_handle,

    [DRV_RHPC_REQ_R2M_CMD_GET_UPG_STATUS]         = drv_rhpc_router_get_upg_status,

    [DRV_RHPC_REQ_R2M_CMD_QUERY_TEMP_CMD]      = NULL,

    [DRV_RHPC_REQ_MODEM_LED_SET]             = drv_rhpc_modem_led_status_set,
};


#if defined(CONFIG_PROC_FS) && defined(DRV_RHPC_DEBUG)
#define DRV_RHPC_PROC_FILE "driver/drv_rhpc"
static struct proc_dir_entry *g_drv_rhpc_proc_file = NULL;

static ssize_t drv_rhpc_proc_read(struct file *filp,
     char *buffer, size_t length, loff_t *offset)
{
    void __user *buf_usr = (void __user *)buffer;
    char buf_knl[] = {"All supported commands and usage:\n"
                      "1. DRV_RHPC_REQ_DFU_NOTIFY_STATUS_CMD,"
                      "Echo notify_dfu_status [status] to execute.\n"};
    static size_t  len_left = 0;
    size_t  len;

    pr_info("%s: Show the cmds supported and usage.\n", __FUNCTION__);

    /* If first read, reset the len_left.*/
    if (0 == *offset)
    {
        len_left = strlen(buf_knl);
    }
    len      = (length > len_left) ? (len_left) : length;

    /* If all buf has been read done, return 0 to indicate read completed.*/
    if(len_left == 0)
    {
        return 0;
    }

    if (!access_ok(VERIFY_WRITE, buf_usr, len))
    {
        pr_err("%s: Verify user buffer write fail.\n", __FUNCTION__);
        return -EFAULT;
    }

    if(copy_to_user(buf_usr, buf_knl + (strlen(buf_knl) - len_left), len))
    {
        pr_err("%s: copy_to_user failed, nothing copied.\n", __FUNCTION__);
        return -EFAULT;
    }

    *offset  += len;
    len_left -= len;
    return len;
}

static ssize_t drv_rhpc_proc_write(struct file *filp,
     const char *buff, size_t length, loff_t *off)
{
    char buf_knl[128] = {0};
    void __user *buf_usr = (void __user *)buff;
    size_t len = 128;
    char *szdata = NULL;

    pr_info("%s: Echo the command to execute.\n", __FUNCTION__);

    if (length > len - 1)
    {
        pr_warning("Data length from usr more than %dBytes, truncate.\n", len - 1);
        len = len - 1;
    }
    else
    {
        len = length;
    }

    if (!access_ok(VERIFY_READ, buf_usr, len))
    {
        pr_err("%s: Verify user buffer read fail.\n", __FUNCTION__);
        return -EFAULT;
    }

    if(copy_from_user(buf_knl, buf_usr, len))
    {
        pr_err("%s: copy_from_user failed, nothing copied\n", __FUNCTION__);
        return -EFAULT;
    }


    szdata = buf_knl;
    if (0 == strncmp(szdata, "notify_dfu_status", strlen("notify_dfu_status")))
    {
        szdata = strchr(buf_knl, ' ');
        if (NULL == szdata)
        {
            pr_err("There is no parameters found, command is %s.\n", buf_knl);
        }
        else
        {
            szdata++;
            pr_info("[parameter]%s got from usr space for notify_dfu_status.\n", szdata);
            /*TODO: What's the parameter stand for ??*/
        }
    }
    else
    {
        pr_err("Unknown command found: %s.\n", szdata);
    }

    return len;
}

static struct file_operations drv_rhpc_proc_ops = {
    .read  = drv_rhpc_proc_read,
    .write = drv_rhpc_proc_write,
};

static void create_drv_rhpc_proc_file(void)
{
    g_drv_rhpc_proc_file = create_proc_entry(DRV_RHPC_PROC_FILE, 0644, NULL);

    if(g_drv_rhpc_proc_file)
    {
        g_drv_rhpc_proc_file->proc_fops = &drv_rhpc_proc_ops;
    }
    else
    {
        pr_warning("%s: create proc entry for drv_rhpc failed\n", __FUNCTION__);
    }
}

static void remove_drv_rhpc_proc_file(void)
{
    extern struct proc_dir_entry proc_root;
    remove_proc_entry(DRV_RHPC_PROC_FILE, &proc_root);
}
#endif /*defined(CONFIG_PROC_FS) && defined(DRV_RHPC_DEBUG)*/


/*Helper Functions.*/
void drv_rhpc_dfou_reboot_hdlr_register(drv_rhpc_dfou_reboot_func func)
{
    drv_rhpc_dfou_reboot = func;
}


/*****************************************************************************
 Name         : drv_rhpc_rhwver_get_handle
 Description  : The handle function when we received get router hardware version
                message from router. As our hardware design, router hardware version
                was stored at modem side.
 INPUT-Parameters  : data   The DRV_RHPC data got from router.
 OUTPUT-Parameters : rsp_buf  Response message data buffer
                     rsp_len  Response message data length
 Return    : CHIP_COMM_SUCC for process successful.
             CHIP_COMM_FAIL for process failure.
 NOTICE    : How to describe the router hardware??.
*****************************************************************************/
static int drv_rhpc_rhwver_get_handle(drv_rhpc_data_t *data, void *rsp_buf, int *rsp_len)
{
    static uint32_t rhwver  = 0;
	rhwver  = (uint32_t)bsp_version_get_hwversion_index();

	printk("begin drv_rhpc_rhwver_get_handle version=0X%X\n", rhwver);
    pr_info("The router hardware version is 0x%x.\n", rhwver);
    *(uint32_t*)rsp_buf = rhwver;
    *rsp_len            = sizeof(rhwver);
	
    return CHIP_COMM_SUCC;
}

/*****************************************************************************
 Name         : drv_rhpc_router_get_datalock_handle
 Description  : The handle function when we received get router hardware version
                message from router. As our hardware design, router hardware version
                was stored at modem side.
 INPUT-Parameters  : data   The DRV_RHPC data got from router.
 OUTPUT-Parameters : rsp_buf  Response message data buffer
                     rsp_len  Response message data length
 Return    : CHIP_COMM_SUCC for process successful.
             CHIP_COMM_FAIL for process failure.
 NOTICE    : How to describe the router hardware??.
*****************************************************************************/
static int drv_rhpc_router_get_datalock_handle(drv_rhpc_data_t *data, void *rsp_buf, int *rsp_len)
{
    char aucBuf[PH_IMEI_LEN + 1];
    unsigned int ulDataLen;
    char loop = 0;

    memset(aucBuf,0,sizeof(aucBuf));
    ulDataLen = PH_IMEI_LEN;

    pr_info("Enter drv_rhpc_router_get_datalock_handle\n");
    if ( NV_OK != NVM_Read(EN_NV_ITEM_IMEI,aucBuf,ulDataLen) )
    {
        pr_info("GetImeiValue:Read IMEI Failed!\n");
    }
    else
    {
        for ( loop = 0;loop < PH_IMEI_LEN;loop++ )
        {
            if ( 0 != aucBuf[loop] )
            {
                break;
            }
        }

        if ( loop == PH_IMEI_LEN )
        {
            pr_info("imei is 0,set datalock 0!");
            g_drv_rhpc_datalock_status = 0;
        }
    }

    *(uint32_t*)rsp_buf = g_drv_rhpc_datalock_status;
    *rsp_len            = sizeof(g_drv_rhpc_datalock_status);
    pr_info("Leave drv_rhpc_router_get_datalock_handle  status=%d\n", g_drv_rhpc_datalock_status);
    return CHIP_COMM_SUCC;
}
/*****************************************************************************
 Name         : drv_rhpc_router_set_customize_item_to_default_handle
 Description  : The handle function when we received get router hardware version
                message from router. As our hardware design, router hardware version
                was stored at modem side.
 INPUT-Parameters  : data   The DRV_RHPC data got from router.
 OUTPUT-Parameters : rsp_buf  Response message data buffer
                     rsp_len  Response message data length
 Return    : CHIP_COMM_SUCC for process successful.
             CHIP_COMM_FAIL for process failure.
 NOTICE    : How to describe the router hardware??.
*****************************************************************************/

static int drv_rhpc_router_set_customize_item_to_default_handle(drv_rhpc_data_t *data, void *rsp_buf, int *rsp_len)
{
    uint32_t ret=0;
    pr_info("Enter drv_rhpc_router_set_customize_item_to_default_handle\n");

    if(NULL != drv_rhpc_at_resume_default)
    {
        pr_info("At callback function initiate OK!address=%d\n",drv_rhpc_at_resume_default);
        ret = drv_rhpc_at_resume_default(0);
    }
    else
    {
       pr_info("At callback function initiate failed!\n");
       return CHIP_COMM_FAIL;
    }

    *(uint32_t*)rsp_buf = ret;
    *rsp_len            = sizeof(ret);
     pr_info("drv_rhpc_router_set_customize_item_to_default_handle Retval=%d\n",ret);
    return CHIP_COMM_SUCC;
}


void mmi_write_nv(int mmi_result)
{
	unsigned int                          ulResult;
    	unsigned char                        aucFacInfo[AT_FACTORY_INFO_LEN];
	memset(aucFacInfo, 0x00, AT_FACTORY_INFO_LEN);  /*pc-lint*/

/* 写入en_NV_Item_Factory_Info，需偏移24个字节，长度4为四个字节，因此需要先读 */
  	ulResult = NVM_Read( en_NV_Item_Factory_Info, aucFacInfo, AT_FACTORY_INFO_LEN);

	 if (NV_OK != ulResult)
	{
		printk("AT_SetTmmiPara: NV Read Fail!");
	  		return;
	}

/* 成功 写入"ST P" */
	if(1 == mmi_result)
	{
		memcpy(&aucFacInfo[AT_MMI_TEST_FLAG_OFFSET], AT_MMI_TEST_FLAG_SUCC, AT_MMI_TEST_FLAG_LEN);/*pc-lint*/
	}
	else if(0 == mmi_result)
	{
		memcpy(&aucFacInfo[AT_MMI_TEST_FLAG_OFFSET], "0000", AT_MMI_TEST_FLAG_LEN);
	}

	ulResult = NVM_Write(en_NV_Item_Factory_Info, aucFacInfo, AT_FACTORY_INFO_LEN);

	if (NV_OK != ulResult)
	{
		printk("AT_SetTmmiPara: NV write Fail!");
	}
	 else
	{
		printk("AT_SetTmmiPara: NV write Success!");
	 }
}

/*****************************************************************************
 Name         : drv_rhpc_router_write_mmi_nv_handle
 Description  : The handle function to write mmi NV item.
 INPUT-Parameters  : data   The DRV_RHPC data got from router.
 OUTPUT-Parameters : rsp_buf  Response message data buffer
                     rsp_len  Response message data length
 Return    : CHIP_COMM_SUCC for process successful.
             CHIP_COMM_FAIL for process failure.
 NOTICE    :
*****************************************************************************/
static int drv_rhpc_router_write_mmi_nv_handle(drv_rhpc_data_t *data, void *rsp_buf, int *rsp_len)
{
    pr_info("Enter drv_rhpc_router_write_mmi_nv_handle\n");
	//call interface
    mmi_write_nv(1);
    pr_info("Leave drv_rhpc_router_write_mmi_nv_handle  OK\n");
    return CHIP_COMM_SUCC;

}
/*****************************************************************************
 Name         : drv_rhpc_router_clear_mmi_nv_handle
 Description  : The handle function to clear mmi NV item.
 INPUT-Parameters  : data   The DRV_RHPC data got from router.
 OUTPUT-Parameters : rsp_buf  Response message data buffer
                     rsp_len  Response message data length
 Return    : CHIP_COMM_SUCC for process successful.
             CHIP_COMM_FAIL for process failure.
 NOTICE    :
*****************************************************************************/
static int drv_rhpc_router_clear_mmi_nv_handle(drv_rhpc_data_t *data, void *rsp_buf, int *rsp_len)
{
    pr_info("Enter drv_rhpc_router_write_mmi_nv_handle\n");
	//call interface
    mmi_write_nv(0);
	//mmi_clear_nv();
    pr_info("Leave drv_rhpc_router_write_mmi_nv_handle  OK\n" );
    return CHIP_COMM_SUCC;
}

int drv_UpDateInfoInit(void)
{
    UPDATE_INFOMATION stUpdateInfo;

    unsigned int fd = 0;
    mm_segment_t old_fs;

    old_fs = get_fs();
    set_fs(KERNEL_DS);

    stUpdateInfo.m_update_info_length       = sizeof(UPDATE_INFOMATION);
    stUpdateInfo.m_update_type              = HUAWEI_PRIVATE;
    stUpdateInfo.m_component_type           = FIRMWARE1;
    stUpdateInfo.m_compress_type            = ZIP;
    stUpdateInfo.m_total_components         = UPDATE_COMPONENTS_NUM;
    stUpdateInfo.m_current_component_index  = 0;
    stUpdateInfo.m_current_component_status = IDLEUP;
    stUpdateInfo.m_full                     = 0;
    stUpdateInfo.m_upgrade_mode             = UPGRADE_USB;
    stUpdateInfo.m_reserved3                = 0;
    stUpdateInfo.m_reserved4                = 0;
    stUpdateInfo.m_component_length         = 0;

    if (0 > (fd = sys_open(UPDATE_INFO_FILE_PATH_NAME, O_RDWR|O_CREAT, S_IRWXU)))
    {
		pr_err("RHPC:update_UpDateInfoInit:Err when open [%s]!\n", UPDATE_INFO_FILE_PATH_NAME);
        set_fs(old_fs);
        return CHIP_COMM_FAIL;
    }

    if (0 > sys_lseek(fd, 0, SEEK_SET))//指向文件首地址
    {
        pr_err("RHPC:update_UpDateInfoInit:Err when seek!\n");
        set_fs(old_fs);
        sys_close(fd);
        return CHIP_COMM_FAIL;
    }

    if (0 > sys_write(fd, &stUpdateInfo, sizeof(UPDATE_INFOMATION)))
    {
        pr_err("RHPC:update_UpDateInfoInit:Err when write!\n");
        set_fs(old_fs);
        sys_close(fd);
        return CHIP_COMM_FAIL;
    }

    set_fs(old_fs);
    sys_close(fd);

    pr_warning("RHPC:update_UpDateInfoInit: SUCCESS !\n");

    return CHIP_COMM_SUCC;
}

int drv_UpDateInfoGet(UPDATE_INFOMATION *pstUpdateInfo)
{
    unsigned int fd = 0;
    mm_segment_t old_fs;
    old_fs = get_fs();
    set_fs(KERNEL_DS);

    if(NULL == pstUpdateInfo)
    {
        pr_err("RHPC:update_UpDateInfoGet:input is null!\n");
		
        return CHIP_COMM_FAIL;
    }

    if (0 > (fd = sys_open(UPDATE_INFO_FILE_PATH_NAME, O_RDONLY, S_IRUSR)))
    {
		pr_err("RHPC:update_UpDateInfoGet:Err when open [%s]!\n", UPDATE_INFO_FILE_PATH_NAME);
		
		set_fs(old_fs);
        return CHIP_COMM_FAIL;
    }

    if (0 > sys_lseek(fd, 0, SEEK_SET))//指向文件首地址
    {
        pr_err("RHPC:update_UpDateInfoGet:Err when seek!\n");
		
        set_fs(old_fs);
        sys_close(fd);
        return CHIP_COMM_FAIL;
    }

    if (0 > sys_read(fd, (char*)pstUpdateInfo, sizeof(UPDATE_INFOMATION)))
    {
        pr_err("RHPC:update_UpDateInfoGet:Err when read!\n");
		
        set_fs(old_fs);
        sys_close(fd);
        return CHIP_COMM_FAIL;
    }
    set_fs(old_fs);
    sys_close(fd);

    return CHIP_COMM_SUCC;
}
int drv_rhpc_led(MODEM_UPG_STATUS emStatus)
{
    int rlt = 0;

    LED_IOCTL_ARG dataLedArg;
    memset(&dataLedArg, 0, sizeof(LED_IOCTL_ARG));
    dataLedArg.led_dev_state = LED_STATE_ON; /*容易出错led_state/led_dev_state*/
    
    switch(emStatus)
    {
        case UPG_STATUS_NONE:
        {
            dataLedArg.led_id = LED_ID_SIGNAL3;
            rlt |= kernel_led_write(dataLedArg, LED_ON);

            dataLedArg.led_id = LED_ID_MODE;
            dataLedArg.led_color = LED_COLOR_BLUE;
            rlt |= kernel_led_write(dataLedArg, LED_ON);
        }
        break;
        case UPG_STATUS_NV_RESTORE_FAIL:
        {
            dataLedArg.led_id = LED_ID_SIGNAL3;
            rlt |= kernel_led_write(dataLedArg, LED_ON);

            dataLedArg.led_id = LED_ID_SIGNAL2;
            rlt |= kernel_led_write(dataLedArg, LED_ON);

            dataLedArg.led_id = LED_ID_MODE;
            dataLedArg.led_color = LED_COLOR_RED;
            rlt |= kernel_led_write(dataLedArg, LED_ON);
        }
        break;
        case UPG_STATUS_NV_RESTORE_SUCCESS:
        {
            dataLedArg.led_id = LED_ID_SIGNAL3;
            rlt |= kernel_led_write(dataLedArg, LED_ON);
            
            dataLedArg.led_id = LED_ID_SIGNAL2;
            rlt |= kernel_led_write(dataLedArg, LED_ON);

            dataLedArg.led_id = LED_ID_MODE;
            dataLedArg.led_color = LED_COLOR_GREEN;
            rlt |= kernel_led_write(dataLedArg, LED_ON);
        }
        break;
        default:
        {
            pr_warning("RHPC:drv_rhpc_led Unkown MODEM_UPG_STATUS!\n");
            return CHIP_COMM_FAIL;
        }

    }

    if(0 != rlt)
    {
        pr_warning("RHPC:drv_rhpc_led err!\n");
        return CHIP_COMM_FAIL;
    }

    return CHIP_COMM_SUCC;
}
static int drv_rhpc_status_send_thread()
{
    static MODEM_UPG_STATUS cur_emStatus = UPG_STATUS_MAX;

    MODEM_UPG_STATUS emStatus = UPG_STATUS_NONE;
    UPDATE_INFOMATION stUpgInfo;
    memset(&stUpgInfo, 0x0, sizeof(UPDATE_INFOMATION));

    /*查询升级状态*/
    if(CHIP_COMM_SUCC != drv_UpDateInfoGet(&stUpgInfo))
    {
        pr_warning("RHPC:get update info failed, so return none!\n");
        emStatus = UPG_STATUS_NONE;
    }
    else
    {
        if((UPDATE_SUCCESSFUL_NO_DATA == stUpgInfo.m_current_component_status) \
            || (UPDATE_SUCCESSFUL_HAVE_DATA == stUpgInfo.m_current_component_status))
        {
            emStatus = UPG_STATUS_NV_RESTORE_SUCCESS;
        }
        else if ((UPDATE_FAILED_NO_DATA == stUpgInfo.m_current_component_status) \
                  || (UPDATE_FAILED_HAVE_DATA == stUpgInfo.m_current_component_status))
        {
            emStatus = UPG_STATUS_NV_RESTORE_FAIL;      /*升级状态位：升级失败，数据已删除*/
        }
        else
        {
            emStatus = UPG_STATUS_NONE;
        }
		
        pr_warning("RHPC:get upgInfo|status = [%d|%d]!\n", stUpgInfo.m_current_component_status, emStatus);
		
        if (cur_emStatus != emStatus)
        {
            cur_emStatus = emStatus;
            if (CHIP_COMM_SUCC != drv_rhpc_led(cur_emStatus))
            {
                pr_warning("RHPC:led Fail!\n");
            }
        }
    }

    drv_rhpc_notify_upg_status_msg_proc(emStatus);
	
    pr_warning("RHPC:send_thread OK, status = [%d]!\n", emStatus);
	
    return 0;
}
static int drv_rhpc_router_get_upg_status(drv_rhpc_data_t *data, void *rsp_buf, int *rsp_len)
{
    struct task_struct *th;
    /*向R侧发送消息*/

    /* Start up the thread for delayed SCSI-device scanning */
	th = kthread_create(drv_rhpc_status_send_thread, NULL, "nothing");
	if (IS_ERR(th))
    {
        pr_warning("RHPC:kthread_create Err!\n");

        return CHIP_COMM_FAIL;
	}
	wake_up_process(th);
	
	pr_warning("RHPC:kthread_create SUCC and wake_up!\n");

    return CHIP_COMM_SUCC;
}


/*****************************************************************************
 Name         : drv_rhpc_notify_upg_status_msg_proc
 Description  : The handle function when we send the modem upg status to router.
 INPUT-Parameters  : data   The current upg status.
 OUTPUT-Parameters : NONE

 Return    : CHIP_COMM_SUCC for process successful.
             CHIP_COMM_FAIL for process failure.
 NOTICE    : .
*****************************************************************************/
int drv_rhpc_notify_upg_status_msg_proc(unsigned int ststus)
{
    drv_rhpc_data_t sendbuf;
    unsigned int upgstatus = 0;
    CHIP_COMM_INFO_ST msg_info;
    int rst = 0;
    msg_info.msg_type = DRV_RHPC_M2R_REQUEST;
    msg_info.msg_flow = CHIP_COMM_SYNC;
    msg_info.msg_mode = CHIP_COMM_ACORE2BCM;

    memset(&sendbuf,0,sizeof(drv_rhpc_data_t));
    sendbuf.cmd = DRV_RHPC_REQ_M2R_CM_SET_UPG_STATUS;
    sendbuf.len = 0;
    sendbuf.idx = ststus;

    rst = chip_comm_send_msg(&msg_info, &sendbuf, sizeof(sendbuf) + sendbuf.len , &upgstatus, sizeof(upgstatus));

    return rst;

}

/*****************************************************************************
 Name         : drv_rhpc_get_nv_value_msg_proc
 Description  : The handle function to retieve nvValue by nvName from router.
 INPUT-Parameters  : nvName   The name of nv item.
                              buff        The value of the nv item.

 Return    : CHIP_COMM_SUCC for process successful.
                CHIP_COMM_FAIL for process failure.
 NOTICE    : .
*****************************************************************************/
int drv_rhpc_get_nv_value_msg_proc(char *nvName,char * buff)
{
   pr_info("Enter drv_rhpc_get_nv_value_msg_proc\n");
    int nvNameLen = strlen(nvName);
    drv_rhpc_data_t *sendbuf = (drv_rhpc_data_t *)kmalloc(sizeof(drv_rhpc_data_t) + sizeof(char)* (nvNameLen+1),GFP_KERNEL);
    char * pMacAdrress = (char *)kmalloc(32, GFP_KERNEL);
    CHIP_COMM_INFO_ST msg_info;
    int rst = 0;
    msg_info.msg_type = DRV_RHPC_M2R_REQUEST;
    msg_info.msg_flow = CHIP_COMM_SYNC;
    msg_info.msg_mode = CHIP_COMM_ACORE2BCM;

    memset(sendbuf,0,sizeof(drv_rhpc_data_t) + nvNameLen+1);
    sendbuf->cmd = DRV_RHPC_REQ_M2R_CMD_GET_NV_VALUE;
    sendbuf->len = nvNameLen+1;
    strncpy(sendbuf->data,nvName,sendbuf->len);//nvName参数放入data域中；
    rst = chip_comm_send_msg(&msg_info, sendbuf, sizeof(drv_rhpc_data_t) + sendbuf->len , pMacAdrress, 32);

    if(CHIP_COMM_SUCC == rst)//返回成功时
    {
    	 pr_info("OK drv_rhpc_get_nv_value_msg_proc pMacAdrress=%s\n",pMacAdrress);
         strncpy(buff,pMacAdrress,strlen(pMacAdrress)+1);
    }
    //释放空间
    if(NULL!=pMacAdrress)
    {
    	kfree(pMacAdrress);
    }
    if(NULL != sendbuf)
    {
    	kfree(sendbuf);
    }
    return rst;

}
/*****************************************************************************
 Name         : drv_rhpc_write_nv_value_msg_proc
 Description  : The handle function to write the value of some nv item .
 INPUT-Parameters  : nvName   The name of nv item.
                              nvValue        The value to be writed.

 Return    : CHIP_COMM_SUCC for process successful.
                CHIP_COMM_FAIL for process failure.
 NOTICE    : .
*****************************************************************************/
int drv_rhpc_write_nv_value_msg_proc(char *nvName,char *nvValue)
{
    pr_info("Enter drv_rhpc_write_nv_value_msg_proc\n");
    int nvNameLen = strlen(nvName);
    int nvValueLen = strlen(nvValue);
    drv_rhpc_data_t *sendbuf = (drv_rhpc_data_t *)kmalloc(sizeof(drv_rhpc_data_t) + sizeof(char)* (nvNameLen+nvValueLen+2),GFP_KERNEL);
    unsigned int writeStatus = 0;

    CHIP_COMM_INFO_ST msg_info;
    int rst = 0;
    msg_info.msg_type = DRV_RHPC_M2R_REQUEST;
    msg_info.msg_flow = CHIP_COMM_SYNC;
    msg_info.msg_mode = CHIP_COMM_ACORE2BCM;

    memset(sendbuf,0,sizeof(drv_rhpc_data_t) + nvNameLen + nvValueLen + 2);
    sendbuf->cmd = DRV_RHPC_REQ_M2R_CMD_WRITE_NV_VALUE;
    sendbuf->len = nvNameLen+nvValueLen+2;
    strncpy(sendbuf->data,nvName,nvNameLen);
    strcat(sendbuf->data,"=");
    strcat(sendbuf->data,nvValue);
    //sendbuf.idx = ststus;
    pr_info("drv_rhpc_write_nv_value_msg_proc retValue=%s\n",sendbuf->data);
    rst = chip_comm_send_msg(&msg_info, sendbuf, sizeof(drv_rhpc_data_t) + sendbuf->len , &writeStatus, sizeof(writeStatus));
    pr_info("drv_rhpc_write_nv_value_msg_proc retValue=%d \n",rst);
     if(NULL != sendbuf)
    	{
    		kfree(sendbuf);
    	}
    return rst;

}


 /*****************************************************************************
 Name         : drv_rhpc_gpio_handle
 Description  : The handle function when we received gpio relative message
                from router, such as gpio direction set, input level get, set
                output level, etc.
 INPUT-Parameters  : data   The DRV_RHPC data got from router.
 OUTPUT-Parameters : rsp_buf  Response message data buffer
                     rsp_len  Response message data length
 Return    : CHIP_COMM_SUCC for process successful.
             CHIP_COMM_FAIL for process failure.
 NOTICE    : GPIO number was stored at idx field.
             TODO: How to request all these gpios? and ensure which be done once?
*****************************************************************************/
#if 0
static int drv_rhpc_gpio_handle(drv_rhpc_data_t *data, void *rsp_buf, int *rsp_len)
{
    int ret = 0;
    drv_rhpc_data_t *recv_data = data;
    uint32_t gpio_gr = 0, gpio_offset = 0, gpio_nr = 0;

    /*Suppress pc-lint warning, arg not used.*/
    (void)rsp_buf;
    (void)rsp_len;

#define GET_BALONG_GPIO_NR(gr, off)  (GPIO_MAX_PINS * gr + off)
    gpio_gr = (recv_data->idx & 0xFF00) >> 8;
    gpio_offset = recv_data->idx & 0xFF;

    if (gpio_gr >= (GPIO_MAXIMUM - 1) || gpio_offset >= GPIO_MAX_PINS)
    {
        pr_err("Invalid gpio group nr %d or gpio offset %d.\n", gpio_gr, gpio_offset);
        return CHIP_COMM_FAIL;
    }

    gpio_nr = GET_BALONG_GPIO_NR(gpio_gr, gpio_offset);
    pr_info("%s: [GPIO_GR]%d, [GPIO_OFF]%d, [GPIO_NR]%d.\n", __FUNCTION__, gpio_gr, gpio_offset, gpio_nr);

    switch (recv_data->cmd)
    {
        case DRV_RHPC_REQ_GPIO_DIR_SET_CMD:
        {
            if (GPIO_DIR_INPUT == recv_data->val)
            {
                gpio_direction_input(gpio_nr);
            }
            else if (GPIO_DIR_OUTPUT == recv_data->val)
            {
                gpio_direction_output(gpio_nr, 0);
            }
            else
            {
                pr_err("Illegal GPIO direction %d got, do nothing.\n", recv_data->val);
                ret = CHIP_COMM_FAIL;
            }
        }
        break;
        case DRV_RHPC_REQ_GPIO_OUTPUT_LEVEL_SET_CMD:
        {
            /*接口保留*/
        }
        break;
        default:
        {
            pr_err("Unknown command %d got, do nothing.\n", recv_data->cmd);
            ret = CHIP_COMM_FAIL;
        }
    }

    return ret;
}
#endif

/*****************************************************************************
 Name         : drv_rhpc_modem_led_status_set
 Description  : The function is we received message to set LED status
                from router.
 INPUT-Parameters  : data   The DRV_RHPC data got from router.
 OUTPUT-Parameters : rsp_buf  Response message data buffer
                     rsp_len  Response message data length
 Return    : CHIP_COMM_SUCC for process successful.
             CHIP_COMM_FAIL for process failure.
 NOTICE    : We'll call the true api exported by updateonline to perform reboot.
*****************************************************************************/
static int drv_rhpc_modem_led_status_set(drv_rhpc_data_t *data,
                void *rsp_buf, int *rsp_len)
{
    int ret = 0;
    unsigned int nLedState = 0;
        
    drv_rhpc_data_t *recv_data = data;
    LED_IOCTL_ARG dataLedArg;

    /*Suppress pc-lint warning, arg not used.*/
    (void)rsp_buf;
    (void)rsp_len;

    memset(&dataLedArg, 0, sizeof(LED_IOCTL_ARG));
    memcpy(&dataLedArg, recv_data->data, recv_data->len);

    if (LED_STATE_ON == dataLedArg.led_dev_state)
    {
        nLedState = LED_ON;
        ret = kernel_led_write(dataLedArg, nLedState);
    }
    else if (LED_STATE_OFF == dataLedArg.led_dev_state)
    {
        nLedState = LED_OFF;
        ret = kernel_led_write(dataLedArg, nLedState);
    }
    else if (LED_STATE_BLINK == dataLedArg.led_dev_state)
    {
        ret = kernel_led_blink(dataLedArg);
    }

    if (0 != ret)
    {
        pr_err("DRV_RHCP balong_led_ioctl : Modem LED status Seting failed.\n");
        ret = CHIP_COMM_FAIL;
    }

    return ret;
}

/*****************************************************************************
 Name         : drv_rhpc_dfu_mreboot_handle
 Description  : The handle function when we received modem reboot to update message
                from router, which was triggered by UPG process.
 INPUT-Parameters  : data   The DRV_RHPC data got from router.
 OUTPUT-Parameters : rsp_buf  Response message data buffer
                     rsp_len  Response message data length
 Return    : CHIP_COMM_SUCC for process successful.
             CHIP_COMM_FAIL for process failure.
 NOTICE    : We'll call the true api exported by updateonline to perform reboot.
*****************************************************************************/
static int drv_rhpc_dfu_mreboot_handle(drv_rhpc_data_t *data, void *rsp_buf, int *rsp_len)
{
    int ret = 0;
    drv_rhpc_data_t *recv_data = data;
    DEVICE_EVENT event;

    /*Suppress pc-lint warning, arg not used.*/
    (void)rsp_buf;
    (void)rsp_len;
	
    if (CHIP_COMM_SUCC != drv_UpDateInfoInit())
    {
        pr_err("drv_rhpc_dfou_reboot UpDateInfoInit failed.\n");
        return CHIP_COMM_FAIL;
    }

    event.device_id  = (DEVICE_ID)recv_data->idx;
    event.event_code = recv_data->val;
    event.len        = 0;

    BUG_ON(DEVICE_ID_UPDATEONLINE != event.device_id);
    BUG_ON(APP_REBOOT_REQUEST     != event.event_code);
#ifdef __REBOOT__
    if (likely(NULL != drv_rhpc_dfou_reboot))
    {
        if (drv_rhpc_dfou_reboot(&event, sizeof(DEVICE_EVENT)))
        {
            pr_err("drv_rhpc_dfou_reboot failed.\n");
            ret = CHIP_COMM_FAIL;
        }
        else
        {
            pr_info("drv_rhpc_dfou_reboot successful.\n");
            ret = CHIP_COMM_SUCC;
        }
    }
    else
    {
        pr_err("drv_rhpc_dfou_reboot not registered.\n");
        ret = CHIP_COMM_FAIL;
    }
#else
    event.device_id  = DEVICE_ID_UPDATEONLINE;
    event.event_code = KNL_NOTIFY_UPDATE_START;
    event.len        = 0;

    device_event_report(&event, sizeof(DEVICE_EVENT));
#endif /*__REBOOT__*/
    return ret;
}


/*****************************************************************************
 Name         : at_set_rhpc_datalock_status
 Description  :The interface to set the g_drv_rhpc_datalock_status called by the AT moudle. when
      the datalock status of the AT module changed, called this function to update the new status.
 INPUT-Parameters  : status     current datalock status
 OUTPUT-Parameters
 Return    : CHIP_COMM_SUCC for process successful.
                CHIP_COMM_FAIL for process failure.
 NOTICE    : .
*****************************************************************************/
int at_set_rhpc_datalock_status(int status)
{
	g_drv_rhpc_datalock_status = status;
	pr_info("at_set_rhpc_datalock_status: %d.\n", g_drv_rhpc_datalock_status);
}
/*****************************************************************************
 Name         : at_set_rhpc_resume_default_func
 Description  :The interface to initiate the function pointer drv_rhpc_at_resume_default called by the
     AT moudle.
 INPUT-Parameters  : status     current datalock status
 OUTPUT-Parameters
 Return    : CHIP_COMM_SUCC for process successful.
                CHIP_COMM_FAIL for process failure.
 NOTICE    : .
*****************************************************************************/
int at_set_rhpc_resume_default_func(void * pfunc)
{
       drv_rhpc_at_resume_default = (drv_rhpc_at_resume_default_func)pfunc;
}

/*****************************************************************************
 Name         : drv_rhpc_r2m_msg_proc
 Description  : Platform driver remote host procedure call Router->Modem message
                process callback function, invoked by chip_comm module.
 INPUT-Parameters  : req_buf  Request message data buffer
                     req_len  Request message data length
 OUTPUT-Parameters : rsp_buf  Response message data buffer
                     rsp_len  Response message data length
 Return    : CHIP_COMM_SUCC for process successful.
             CHIP_COMM_FAIL for process failure.
 NOTICE    : All platform driver rhpc requests from router are both dealed with
             by this function.
*****************************************************************************/
int drv_rhpc_r2m_msg_proc(void *req_buf, int req_len, void *rsp_buf, int *rsp_len)
{
    int ret = 0;
    drv_rhpc_data_t* recv_data = (drv_rhpc_data_t*)req_buf;
    int recv_data_len = req_len;

    if (req_len != (sizeof(drv_rhpc_data_t) + recv_data->len))
    {
        pr_err("%s: Receive data len %d incorrect, %d expected.\n", __FUNCTION__,
            recv_data_len, sizeof(drv_rhpc_data_t) + recv_data->len);
        return CHIP_COMM_FAIL;
    }

    BUG_ON(NULL == recv_data);

    pr_info("Message recevied from router: [cmd]%d, [idx]%d, [val]%d.\n",
        recv_data->cmd, recv_data->idx, recv_data->val);

    /*If the cmd illegal??*/
    if (recv_data->cmd >= DRV_RHPC_REQ_R2M_CMD_MAX_IDX)
    {
        pr_err("Unknow cmd %d received, abandon..\n", recv_data->cmd);
        return CHIP_COMM_FAIL;
    }

    /*Now dispatch and handle the msg.*/
    if (unlikely(NULL == drv_rhpc_r2m_msg_hdlr[recv_data->cmd]))
    {
        pr_err("The handler function for cmd %d not registered.\n", recv_data->cmd);
        ret = CHIP_COMM_FAIL;
    }
    else
    {
        ret = drv_rhpc_r2m_msg_hdlr[recv_data->cmd](recv_data, rsp_buf, rsp_len);
        pr_info("drv_rhpc_r2m_msg_hdlr start!----------- \n");
    }

    return ret;
}

static int startup_status_detect_thread(void *data)
{
#define DETECT_ROUTER_STATUS_TIME_IN_MSEC   30000
#define DETECT_SLEEP_TIME_IN_MSEC            2000
    if (!kthread_should_stop())
    {
        int ret = 0;
        int i = 0;
        int update_ret = 0;

        for (i = 0; i < (DETECT_ROUTER_STATUS_TIME_IN_MSEC/DETECT_SLEEP_TIME_IN_MSEC); i++)
        {
            ret = chip_comm_detect_peer(CHIP_COMM_ACORE2BCM);
            if (CHIP_COMM_SUCC == ret)
            {
                pr_info("Detect router success at %d times.\n", i + 1);
                break;
            }
            msleep(DETECT_SLEEP_TIME_IN_MSEC);
        }
        update_ret = update_getModemUpgFlag();
        if(1 == update_ret)
        {
            pr_info("UpdataOnline Mode : Modem don't detect Router.\n");
            return 0;
        }
        if (CHIP_COMM_FAIL == ret)
        {
            pr_err("Detect router failed, blink MODE_RED led to indicate error.\n");
#if 0
            /*TODO: Blinking MODE_RED LED to indicate error.*/
            BSP_LED_IoctlWink(LED_STATE_BLINK, LED_ID_MODE,
                    g_ledWinkOnTime, g_ledWinkOnTime);
            BSP_LED_IoctlWink(LED_STATE_OFF, LED_ID_SIGNAL1, 0, 0);
            BSP_LED_IoctlWink(LED_STATE_OFF, LED_ID_SIGNAL2, 0, 0);
            BSP_LED_IoctlWink(LED_STATE_OFF, LED_ID_SIGNAL3, 0, 0);
            BSP_LED_IoctlWink(LED_STATE_OFF, LED_ID_SLIC, 0, 0);
#endif
        }

    }

    return 0;
}


static int __init drv_rhpc_init(void)
{
    int ret = 0;
    /*Detect whether the other side is OK.*/
    ret = chip_comm_detect_peer(CHIP_COMM_ACORE2BCM);

    if (CHIP_COMM_FAIL == ret)
    {
        pr_warning("DRV_RHPC: Detect Router fail with 0x%d, run startup status detection thread!!\n", ret);
        g_startup_status_detect_thread = kthread_run(startup_status_detect_thread, NULL, "mpwron_status_checkd");
        if (IS_ERR(g_startup_status_detect_thread))
        {
            pr_alert("DRV_RHPC: Create startup status detection thread fail!!.\n");
        }
        else
        {
            ret = CHIP_COMM_SUCC;
        }
    }
    else
    {
        pr_info("DRV_RHPC: Got init req from router successfully.\n");
    }

#if defined(CONFIG_PROC_FS) && defined(DRV_RHPC_DEBUG)
    create_drv_rhpc_proc_file();
#endif /*defined(CONFIG_PROC_FS) && defined(DRV_RHPC_DEBUG)*/

	return ret;
}

static void __exit drv_rhpc_exit(void)
{
#if defined(CONFIG_PROC_FS) && defined(DRV_RHPC_DEBUG)
    remove_drv_rhpc_proc_file();
#endif /*defined(CONFIG_PROC_FS) && defined(DRV_RHPC_DEBUG)*/

	return;
}

void mode_led_test(void)
{
    LED_IOCTL_ARG LedArg;
    drv_rhpc_data_t *send_buf = (drv_rhpc_data_t *)kmalloc(sizeof(drv_rhpc_data_t)
                                    + sizeof(LED_IOCTL_ARG), GFP_KERNEL);
    if(NULL == send_buf)
    {
        return CHIP_COMM_FAIL;
    }

    memset(&LedArg,0,sizeof(LED_IOCTL_ARG));
    LedArg.led_dev_state = LED_STATE_ON;
    LedArg.led_color = LED_COLOR_RED;

    memset(send_buf, 0, sizeof(drv_rhpc_data_t) + sizeof(LED_IOCTL_ARG));
    send_buf->cmd = DRV_RHPC_REQ_MODEM_LED_SET;
    send_buf->len = sizeof(LED_IOCTL_ARG);
    memcpy(send_buf->data, (char *)&LedArg, send_buf->len);

    drv_rhpc_modem_led_status_set(send_buf, 0, 0);

    if(NULL != send_buf)
    {
        kfree(send_buf);
    }
}

void mode_rhwver_test(void)
{
    uint32_t rhwver = 0;
	uint32_t rhwverlen = 0;
	drv_rhpc_rhwver_get_handle(0, &rhwver, &rhwverlen);
	
	printk("mode_rhwver_test rhwver=0X%X rhwverlen=0X%X\n", rhwver, rhwverlen);
}


module_init(drv_rhpc_init);
module_exit(drv_rhpc_exit);

