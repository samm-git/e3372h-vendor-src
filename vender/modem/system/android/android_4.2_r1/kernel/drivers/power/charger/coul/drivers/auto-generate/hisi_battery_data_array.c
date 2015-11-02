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
#include "hisi_battery_data_default.c"
#include "hisi_battery_data_feimaotui_1780.c"
#include "hisi_battery_data_feimaotui_1500.c"

static struct hisi_smartstar_coul_battery_data *battery_data_array[] = {
    &default_battery_data,
    &feimaotui_1780_battery_data,
    &feimaotui_1500_battery_data,
};



