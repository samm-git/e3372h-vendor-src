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
#include "chg_hardware_data.h"

static const chg_hw_param_t chg_std_chgr_hw_paras[CHG_STM_MAX_ST] =
{
    /*0:-Transition State*/
    {
        1200,  /*mA, Power Supply front-end Current limit.*/
         576,  /*mA, Charge Current limit.*/
        4208,  /*mV, CV Voltage setting.*/
         128,  /*mA, Taper(Terminate) current.*/
        FALSE  /*If charge enabled: FALSE:-Disable, TRUE:-Enable.*/
    },
    /*1:-Fast Charge State*/
    {
        1200,  /*mA, Power Supply front-end Current limit.*/
        1024,  /*mA, Charge Current limit.*/
        4208,  /*mV, CV Voltage setting.*/
         128,  /*mA, Taper(Terminate) current.*/
        TRUE   /*If charge enabled: FALSE:-Disable, TRUE:-Enable.*/
    },
    /*2:-Maintenance State*/
    {
        1200,  /*mA, Power Supply front-end Current limit.*/
         576,  /*mA, Charge Current limit.*/
        4208,  /*mV, CV Voltage setting.*/
         128,  /*mA, Taper(Terminate) current.*/
        FALSE  /*If charge enabled: FALSE:-Disable, TRUE:-Enable.*/
    },
    /*3:-Invalid Charge Temperature State*/
    {
        1200,  /*mA, Power Supply front-end Current limit.*/
         576,  /*mA, Charge Current limit.*/
        4200,  /*mV, CV Voltage setting.*/
         128,  /*mA, Taper(Terminate) current.*/
        FALSE  /*If charge enabled: FALSE:-Disable, TRUE:-Enable.*/
    },

    /*4:-Battery Only State*/
    {
       1200,  /*mA, Power Supply front-end Current limit.*/
        576,  /*mA, Charge Current limit.*/
       4200,  /*mV, CV Voltage setting.*/
        256,  /*mA, Taper(Terminate) current.*/
        FALSE  /*If charge enabled: FALSE:-Disable, TRUE:-Enable.*/

    },

    #if defined(MBB_CHG_WARM_CHARGE)
    /*5:-Warm charge State*/
    {
        500,  /*mA, Power Supply front-end Current limit.*/
        512,  /*mA, Charge Current limit.*/
        4144,  /*mV, CV Voltage setting.*/
         128,  /*mA, Taper(Terminate) current.*/
        TRUE   /*If charge enabled: FALSE:-Disable, TRUE:-Enable.*/
    },
    #endif /* MBB_CHG_WARM_CHARGE */
};
static const chg_hw_param_t chg_usb_chgr_hw_paras[CHG_STM_MAX_ST] =
{
    /*0:-Transition State*/
    {
         500,  /*mA, Power Supply front-end Current limit.*/
         576,  /*mA, Charge Current limit.*/
        4208,  /*mV, CV Voltage setting.*/
         128,  /*mA, Taper(Terminate) current.*/
        FALSE  /*If charge enabled: FALSE:-Disable, TRUE:-Enable.*/
    },
    /*1:-Fast Charge State*/
    {
         500,  /*mA, Power Supply front-end Current limit.*/
         512,  /*mA, Charge Current limit.*/
        4208,  /*mV, CV Voltage setting.*/
         128,  /*mA, Taper(Terminate) current.*/
         TRUE  /*If charge enabled: FALSE:-Disable, TRUE:-Enable.*/
    },
    /*2:-Maintenance State*/
    {
         500,  /*mA, Power Supply front-end Current limit.*/
         512,  /*mA, Charge Current limit.*/
        4208,  /*mV, CV Voltage setting.*/
         128,  /*mA, Taper(Terminate) current.*/
        FALSE  /*If charge enabled: FALSE:-Disable, TRUE:-Enable.*/
    },
    /*3:-Invalid Charge Temperature State*/
    {
         500,  /*mA, Power Supply front-end Current limit.*/
         512,  /*mA, Charge Current limit.*/
        4208,  /*mV, CV Voltage setting.*/
         128,  /*mA, Taper(Terminate) current.*/
        FALSE  /*If charge enabled: FALSE:-Disable, TRUE:-Enable.*/
    },

    /*4:-Battery Only State*/
    {
         500,  /*mA, Power Supply front-end Current limit.*/
         512,  /*mA, Charge Current limit.*/
        4208,  /*mV, CV Voltage setting.*/
         128,  /*mA, Taper(Terminate) current.*/
        FALSE  /*If charge enabled: FALSE:-Disable, TRUE:-Enable.*/
    },

    #if defined(MBB_CHG_WARM_CHARGE)
    /*5:-Warm charge State*/
    {
         500,  /*mA, Power Supply front-end Current limit.*/
         512,  /*mA, Charge Current limit.*/
        4144,  /*mV, CV Voltage setting.*/
         128,  /*mA, Taper(Terminate) current.*/
        TRUE   /*If charge enabled: FALSE:-Disable, TRUE:-Enable.*/
    },
    #endif /* MBB_CHG_WARM_CHARGE */
};
static const chg_hw_param_t chg_usb3_chgr_hw_paras[CHG_STM_MAX_ST] =
{
    /*0:-Transition State*/
    {
         900,  /*mA, Power Supply front-end Current limit.*/
         576,  /*mA, Charge Current limit.*/
        4208,  /*mV, CV Voltage setting.*/
         128,  /*mA, Taper(Terminate) current.*/
        FALSE  /*If charge enabled: FALSE:-Disable, TRUE:-Enable.*/
    },
    /*1:-Fast Charge State*/
    {
         900,  /*mA, Power Supply front-end Current limit.*/
         1024,  /*mA, Charge Current limit.*/
        4200,  /*mV, CV Voltage setting.*/
         128,  /*mA, Taper(Terminate) current.*/
         TRUE  /*If charge enabled: FALSE:-Disable, TRUE:-Enable.*/
    },
    /*2:-Maintenance State*/
    {
         900,  /*mA, Power Supply front-end Current limit.*/
         512,  /*mA, Charge Current limit.*/
        4208,  /*mV, CV Voltage setting.*/
         128,  /*mA, Taper(Terminate) current.*/
        FALSE  /*If charge enabled: FALSE:-Disable, TRUE:-Enable.*/
    },
    /*3:-Invalid Charge Temperature State*/
    {
         900,  /*mA, Power Supply front-end Current limit.*/
         512,  /*mA, Charge Current limit.*/
        4208,  /*mV, CV Voltage setting.*/
         128,  /*mA, Taper(Terminate) current.*/
        FALSE  /*If charge enabled: FALSE:-Disable, TRUE:-Enable.*/
    },

    /*4:-Battery Only State*/
    {
         900,  /*mA, Power Supply front-end Current limit.*/
         512,  /*mA, Charge Current limit.*/
        4208,  /*mV, CV Voltage setting.*/
         128,  /*mA, Taper(Terminate) current.*/
        FALSE  /*If charge enabled: FALSE:-Disable, TRUE:-Enable.*/
    },

    #if defined(MBB_CHG_WARM_CHARGE)
    /*5:-Warm charge State*/
    {
         900,  /*mA, Power Supply front-end Current limit.*/
         576,  /*mA, Charge Current limit.*/
        4100,  /*mV, CV Voltage setting.*/
         128,  /*mA, Taper(Terminate) current.*/
        TRUE   /*If charge enabled: FALSE:-Disable, TRUE:-Enable.*/
    },
    #endif /* MBB_CHG_WARM_CHARGE */
};

static CHG_TEMP_ADC_TYPE  g_adc_batt_therm_map[] =
{
//该表由硬件提供
    {-30,      2327},   /*vol to temp*/
    {-29,      2319},   /*vol to temp*/
    {-28,      2311},   /*vol to temp*/
    {-27,      2302},   /*vol to temp*/
    {-26,      2293},   /*vol to temp*/
    {-25,      2283},   /*vol to temp*/
    {-24,      2274},   /*vol to temp*/
    {-23,      2263},   /*vol to temp*/
    {-22,      2252},   /*vol to temp*/
    {-21,      2241},   /*vol to temp*/
    {-20,      2229},   /*vol to temp*/
    {-19,      2217},   /*vol to temp*/
    {-18,      2204},   /*vol to temp*/
    {-17,      2191},   /*vol to temp*/
    {-16,      2177},   /*vol to temp*/
    {-15,      2163},    /*vol to temp*/
    {-14,      2148},    /*vol to temp*/
    {-13,      2133},    /*vol to temp*/
    {-12,      2117},    /*vol to temp*/
    {-11,      2101},    /*vol to temp*/
    {-10,      2084},    /*vol to temp*/
    {-9 ,       2066},    /*vol to temp*/
    {-8 ,       2048},    /*vol to temp*/
    {-7 ,       2030},    /*vol to temp*/
    {-6 ,       2010},    /*vol to temp*/
    {-5 ,       1991},    /*vol to temp*/
    {-4 ,       1971},    /*vol to temp*/
    {-3 ,       1950},    /*vol to temp*/
    {-2 ,       1929},    /*vol to temp*/
    {-1 ,       1908},    /*vol to temp*/
    {0     ,    1886},    /*vol to temp*/
    {1     ,    1863},    /*vol to temp*/
    {2     ,    1840},    /*vol to temp*/
    {3     ,    1817},    /*vol to temp*/
    {4     ,    1793},    /*vol to temp*/
    {5     ,    1769},    /*vol to temp*/
    {6     ,    1744},    /*vol to temp*/
    {7     ,    1719},    /*vol to temp*/
    {8     ,    1694},    /*vol to temp*/
    {9     ,    1668},    /*vol to temp*/
    {10 ,       1643},    /*vol to temp*/
    {11 ,       1617},    /*vol to temp*/
    {12 ,       1590},    /*vol to temp*/
    {13 ,       1564},    /*vol to temp*/
    {14 ,       1537},    /*vol to temp*/
    {15 ,       1510},    /*vol to temp*/
    {16 ,       1483},    /*vol to temp*/
    {17 ,       1456},    /*vol to temp*/
    {18 ,       1429},    /*vol to temp*/
    {19 ,       1402},    /*vol to temp*/
    {20 ,       1375},    /*vol to temp*/
    {21 ,       1348},    /*vol to temp*/
    {22 ,       1320},    /*vol to temp*/
    {23 ,       1293},    /*vol to temp*/
    {24 ,       1267},    /*vol to temp*/
    {25 ,       1240},    /*vol to temp*/
    {26 ,       1213},    /*vol to temp*/
    {27 ,       1187},    /*vol to temp*/
    {28 ,       1160},    /*vol to temp*/
    {29 ,       1134},    /*vol to temp*/
    {30 ,       1108},    /*vol to temp*/
    {31 ,       1083},    /*vol to temp*/
    {32 ,       1058},    /*vol to temp*/
    {33 ,       1033},    /*vol to temp*/
    {34 ,       1008},    /*vol to temp*/
    {35 ,        984},    /*vol to temp*/
    {36 ,        959},    /*vol to temp*/
    {37 ,        936},    /*vol to temp*/
    {38 ,        912},    /*vol to temp*/
    {39 ,        890},    /*vol to temp*/
    {40 ,        867},    /*vol to temp*/
    {41 ,        845},    /*vol to temp*/
    {42 ,        823},    /*vol to temp*/
    {43 ,        802},    /*vol to temp*/
    {44 ,        781},    /*vol to temp*/
    {45 ,        760},    /*vol to temp*/
    {46 ,        740},    /*vol to temp*/
    {47 ,        720},    /*vol to temp*/
    {48 ,        701},    /*vol to temp*/
    {49 ,        682},    /*vol to temp*/
    {50 ,        664},    /*vol to temp*/
    {51 ,        646},    /*vol to temp*/
    {52 ,        628},    /*vol to temp*/
    {53 ,        611},    /*vol to temp*/
    {54 ,        594},    /*vol to temp*/
    {55 ,        578},     /*vol to temp*/
    {56 ,        562},     /*vol to temp*/
    {57 ,        546},     /*vol to temp*/
    {58 ,        531},     /*vol to temp*/
    {59 ,        516},     /*vol to temp*/
    {60 ,        502},     /*vol to temp*/
    {61 ,        488},     /*vol to temp*/
    {62 ,        474},     /*vol to temp*/
    {63 ,        461},     /*vol to temp*/
    {64 ,        448},     /*vol to temp*/
    {65 ,        435},     /*vol to temp*/
    {66 ,        423},     /*vol to temp*/
    {67 ,        411},     /*vol to temp*/
    {68 ,        399},     /*vol to temp*/
    {69 ,        388},     /*vol to temp*/
    {70 ,        377},     /*vol to temp*/
    {71 ,        366},     /*vol to temp*/
    {72 ,        356},     /*vol to temp*/
    {73 ,        346},     /*vol to temp*/
    {74 ,        337},     /*vol to temp*/
    {75 ,        327},     /*vol to temp*/
    {76 ,        318},     /*vol to temp*/
    {77 ,        309},     /*vol to temp*/
    {78 ,        300},     /*vol to temp*/
    {79 ,        292},     /*vol to temp*/
    {80 ,        284},     /*vol to temp*/
    {81 ,        276},     /*vol to temp*/
    {82 ,        268},     /*vol to temp*/
    {83 ,        261},     /*vol to temp*/
    {84 ,        254},     /*vol to temp*/
    {85 ,        247},     /*vol to temp*/

};

static CHG_TEMP2ADC adc2temp_map = {
    .adc_batt_therm_map = &g_adc_batt_therm_map,
    .size = sizeof(g_adc_batt_therm_map) / sizeof(g_adc_batt_therm_map[0]),
};

static const uint32_t chg_fastchg_timeout_value_in_sec[2][2] =
{
    /*Power-off charge.*/
    {
        CHG_POWEROFF_FAST_USB_TIMER_VALUE,           /*USB/NoStd Chgr*/
        CHG_POWEROFF_FAST_CHG_TIMER_VALUE,           /*Wall/Standard Chgr*/
    },
    /*Normal charge.*/
    {
        CHG_FAST_USB_TIMER_VALUE,                    /*USB/NoStd Chgr*/
        CHG_FAST_CHG_TIMER_VALUE,                    /*Wall/Standard Chgr*/
    },
};

static struct chg_hardware_data chg_hardware_data_default = {
        .id_voltage_min = 666,
        .id_voltage_max = 888,
        .chg_std_chgr_hw_paras        = &chg_std_chgr_hw_paras,
        .chg_usb_chgr_hw_paras        = &chg_usb_chgr_hw_paras,
        .chg_usb3_chgr_hw_paras       = &chg_usb3_chgr_hw_paras,
        .adc2temp_map    = &adc2temp_map,
		.batt_volt_hkadc_id = 1,
		.vbus_volt_hkadc_id = 2,
		.batt_temp_hkadc_id = 10,
		.batt_cali_volt_min = 3400,
		.batt_cali_volt_max = 4200,
		.batt_bad_volt = 2700,
		.batt_valid_temp = -30,
		.extchg_stop_temp = 55,
		.extchg_resume_temp = 51,    
		.chg_fastchg_timeout_value_in_sec    = &chg_fastchg_timeout_value_in_sec,
		.chg_ic_en_gpio = 92,

};
