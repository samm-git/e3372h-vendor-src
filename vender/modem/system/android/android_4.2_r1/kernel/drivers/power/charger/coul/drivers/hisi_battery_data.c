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
#include "hisi_battery_data.h"
#include "auto-generate/hisi_battery_data_array.c"

#define BATTERY_DATA_ARRY_SIZE sizeof(battery_data_array)/sizeof(battery_data_array[0])

struct hisi_smartstar_coul_battery_data *get_battery_data(unsigned int id_voltage)
{
    int i;

    for (i=(BATTERY_DATA_ARRY_SIZE - 1); i>0; i--){
        if ((id_voltage >= battery_data_array[i]->id_voltage_min)
            && (id_voltage <= battery_data_array[i]->id_voltage_max)){
            break;
        }
    }

    return battery_data_array[i];
}


