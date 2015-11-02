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

#ifndef __DRV_RHPC_EXT_API_H__
#define __DRV_RHPC_EXT_API_H__

typedef enum
{
    GPIO_DIR_INPUT,
    GPIO_DIR_OUTPUT,
}drv_rhpc_gpio_dir_t;

typedef enum
{
    TEMP_FLAG_WIFI_2G,
    TEMP_FLAG_WIFI_5G,
    TEMP_FLAG_DDR,
    TEMP_FLAG_SIM,
    TEMP_FLAG_INVALID
}drv_rhpc_temp_flag;

typedef int (*drv_rhpc_dfou_reboot_func)(void*, int);

typedef int (*drv_rhpc_at_resume_default_func)(unsigned char*);

void drv_rhpc_dfou_reboot_hdlr_register(drv_rhpc_dfou_reboot_func func);


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
int drv_rhpc_r2m_msg_proc(void *req_buf, int req_len, void *rsp_buf, int *rsp_len);

/*****************************************************************************
 Name         : drv_rhpc_notify_upg_status_msg_proc
 Description  : The handle function when we send the modem upg status to router.
 INPUT-Parameters  : data   The current upg status. 
 OUTPUT-Parameters : NONE
                     
 Return    : CHIP_COMM_SUCC for process successful.
             CHIP_COMM_FAIL for process failure.
 NOTICE    : .
*****************************************************************************/
int drv_rhpc_notify_upg_status_msg_proc(unsigned int ststus);


/*****************************************************************************
 Name         : drv_rhpc_get_nv_value_msg_proc
 Description  : The handle function to read the nv value from router.
 INPUT-Parameters  : nvName   The name of the router nv item. 
 OUTPUT-Parameters : buff    the value of nvname
                     
 Return    : CHIP_COMM_SUCC for process successful.
                CHIP_COMM_FAIL for process failure.
 NOTICE    : .
*****************************************************************************/
int drv_rhpc_get_nv_value_msg_proc(char *nvName,char * buff);
/*****************************************************************************
 Name         : drv_rhpc_write_nv_value_msg_proc
 Description  : The handle function to set the nv item to router.
 INPUT-Parameters  : nvName   The name of the router nv item. 
                              nvValue    the value to be set
                     
 Return    : CHIP_COMM_SUCC for process successful.
                CHIP_COMM_FAIL for process failure.
 NOTICE    : .
*****************************************************************************/
int drv_rhpc_write_nv_value_msg_proc(char *nvName,char *nvValue);
/*****************************************************************************
 Name         : at_set_rhpc_datalock_status
 Description  : The function to set the datalock status called by AT moudule.
 INPUT-Parameters  : status   status of datalock. 
                             
 Return    : CHIP_COMM_SUCC for process successful.
                CHIP_COMM_FAIL for process failure.
 NOTICE    : .
*****************************************************************************/
int at_set_rhpc_datalock_status(int status);
/*****************************************************************************
 Name         : at_set_rhpc_resume_default_func
 Description  : The function to set function pointer called by AT moudule.
 INPUT-Parameters  : status   status of datalock. 
                             
 Return    : CHIP_COMM_SUCC for process successful.
                CHIP_COMM_FAIL for process failure.
 NOTICE    : .
*****************************************************************************/
int at_set_rhpc_resume_default_func(void * pfunc);


#endif /*__DRV_RHPC_EXT_API_H__*/

