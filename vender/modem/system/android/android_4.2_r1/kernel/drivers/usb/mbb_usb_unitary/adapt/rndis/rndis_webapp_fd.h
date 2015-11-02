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
#ifndef _RNDIS_WEBAPP_H_
#define _RNDIS_WEBAPP_H_


#define RNDIS_RET_SUCCESS   0
#define RNDIS_RETURN_FAIL   ( - 1 )

enum ioctl_type
{
    ENUM_FORCE_SWITCH_PROJECT_MODE = 0,
    ENUM_FORCE_SWITCH_DEBUG_MODE    = 1,
    ENUM_USB_RNDIS_RESERV1,
    ENUM_USB_RNDIS_SET_RATE,//         = 3,
    ENUM_APP_DHCP_TASK_START,//        = 4,
    NUM_USB_RNDIS_INVALID
};



int  rndis_webapp_init(void);
extern void pnp_switch_rndis_debug_mode(void);

extern void pnp_switch_rndis_project_mode(void);

//int rndis_rndis_set_rate(uint32_t ulRate);

//void rndis_fd_status_connect (void);


extern int rndis_set_rate(uint32_t ulRate);
extern void rndis_send_connect_event_to_host(void);

#endif/*_RNDIS_WEBAPP_H_*/
