#ifndef __HISI_SMARTSTAR_COUL_H__
#define __HISI_SMARTSTAR_COUL_H__
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
/*DEBUG LEVEL*/
typedef enum
{
    COUL_MSG_ERR = 0,
    COUL_MSG_DEBUG,
    COUL_MSG_INFO,
    COUL_MSG_MAX
}COUL_MSG_TYPE;
/*******************************************************************
Function:      smartstar_battery_capacity
Description:   get the battery soc
Calls:         Battery management module
Input:         none
Output:        none
Return:        return the capacity of battery.
*******************************************************************/
int smartstar_battery_capacity(void);

#endif
