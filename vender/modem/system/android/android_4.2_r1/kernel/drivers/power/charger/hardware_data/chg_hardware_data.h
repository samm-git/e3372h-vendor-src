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
#ifndef __CHG_HARDWARE_DATA__
#define __CHG_HARDWARE_DATA__

#include "../chg_config.h"

typedef struct
{
    CHG_TEMP_ADC_TYPE    *adc_batt_therm_map;
    int32_t   size;
}CHG_TEMP2ADC;

struct chg_hardware_data {
    unsigned int      id_voltage_min;
    unsigned int      id_voltage_max;
    chg_hw_param_t    *chg_std_chgr_hw_paras;
    chg_hw_param_t    *chg_usb_chgr_hw_paras;
    chg_hw_param_t    *chg_usb3_chgr_hw_paras;
    CHG_TEMP2ADC      *adc2temp_map;
    unsigned int      batt_volt_hkadc_id;
    unsigned int      vbus_volt_hkadc_id;
    unsigned int      batt_temp_hkadc_id;
    unsigned int      batt_cali_volt_min;
    unsigned int      batt_cali_volt_max;
    unsigned int      batt_bad_volt;
    unsigned int      batt_valid_temp;
    unsigned int      extchg_stop_temp;
    unsigned int      extchg_resume_temp;    
    unsigned int      chg_fastchg_timeout_value_in_sec[3][3];
    unsigned int      chg_ic_en_gpio;
    char *batt_brand;
};

extern struct chg_hardware_data *chg_get_hardware_data(unsigned int id);

extern int chg_get_hardware_data_id(void);

#endif
