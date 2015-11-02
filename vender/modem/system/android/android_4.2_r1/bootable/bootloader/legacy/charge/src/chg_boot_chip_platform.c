/******************************************************************************

                  版权所有 (C), 2001-2011,华为终端有限公司

 ******************************************************************************
  文 件 名   : chg_chip_platform.c
  版 本 号   : 初稿
  生成日期   : 2013年03月15日
  最近修改   :
  功能描述   : 9x25充电模块平台相关接口实现
  函数列表   :
******************************************************************************/
/******************************问题单修改记录**********************************
    日期            修改人           问题单号                 修改内容
******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "chg_boot_chip_platform.h"
#include "chg_boot_charge_drv.h"

#if (MBB_CHG_PLATFORM_V7R2 == FEATURE_ON)
#include <stdarg.h>
#include <boot/boot.h>
#include "../libc/libc_private.h"
#include <bsp_nvim.h>
#if defined(CONFIG_HKADC)
#include <bsp_hkadc.h>
#endif
#ifdef CONFIG_COUL
#include <bsp_coul.h>
#endif
#include "balongv7r2/lcd.h"
#if (MBB_LED == FEATURE_ON)
#include <balongv7r2/led_boot.h>
#endif
#endif
/*9X25平台需要包含如下头文件*/
#if 0 /*PN:dongle121, chendongguo, 2013/6/27 deleted*/
#include "chg_boot_i2c_virtual.h"
#include "chg_boot_uawei_lcd.h"
#include "huawei_pwrctl.h"
#include "AdcBoot.h"
#include "pm_mpps.h"
#include "DALSys.h"
#include "smem_huawei.h"
#endif



/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/
 /*采集的参数类型和对应ADC通道，通道需要移植时根据产品情况检查及修改*/

/***************Note:平台相关代码，根据平台按需要添加，有的平台如9x15需要
              移植人员根据需要，添加或者移除下边函数调用***************************/
#define CHG_BAT_VOLT_ADC_CH    0x20

#if (MBB_CHG_BQ27510 == FEATURE_ON)
#define bq27510_SLAVE_I2C_ADDR              (0x55)
#define BQ27510_REG_TEMP                    (0x06)
#define BQ27510_REG_VOLT                    (0x08)
#define CONST_NUM_10                        (10)
#define CONST_NUM_2730                      (2730)
#endif
/* LCD 一列的像素个数 */
#define LCD_HEIGHT 320
/* LCD一行的像素个数 */
#define LCD_WIDTH  240

#define SAMPLE_BATTERY_VOLT_MULT 151 / 51
/*notes:define the ADC channel according to <bsp_hkadc.h>*/
#if (MBB_CHG_PLATFORM_V7R2 == FEATURE_ON)


#define CHG_BATT_THERM_HKADC_ID    CHG_BATT_TEMP_CHAN/*电池温度读取通道*/
#define CHG_VBUS_VOLT_HKADC_ID     CHG_VBUS_VOLT_CHAN/*VBUS电压读取通道，5578上VBUS未使用*/
#define CHG_BATT_VOLT_HKADC_ID     CHG_BATT_VOLT_CHAN/*电池电压读取通道*/
#define CHG_BATT_ID_VOLT_HKADC_ID   CHG_BATT_ID_CHAN/*电池电压读取通道*/

#endif
/*用于保存NV中读取的电池电压校准最大端*/
int32_t g_vbatt_max = 0;
/*用于保存NV中读取的电池电压校准最小端*/
int32_t g_vbatt_min = 0;
/*用于保存是否进行了校准初始化的标志*/
boolean g_is_batt_volt_calib_init = FALSE;

/*用于保存屏幕已经显示的动画*/
static CHG_BATT_DISPLAY_TYPE disp_state = CHG_DISP_MAX;
/*该表由硬件提供*/
/* 电池内部NTC实际温度校准表，7x25平台该表的值在ADC模块修改*/
#if defined(BSP_CONFIG_BOARD_E5) 
const  CHG_TEMP2ADC_TYPE  g_adc_batt_therm_map[] =
{
//该表由硬件提供
    {-30,      2304},   /*vol to temp*/
    {-29,      2295},   /*vol to temp*/
    {-28,      2284},   /*vol to temp*/
    {-27,      2274},   /*vol to temp*/
    {-26,      2263},   /*vol to temp*/
    {-25,      2252},   /*vol to temp*/
    {-24,      2240},   /*vol to temp*/
    {-23,      2228},   /*vol to temp*/
    {-22,      2215},   /*vol to temp*/
    {-21,      2202},   /*vol to temp*/
    {-20,      2189},   /*vol to temp*/
    {-19,      2175},   /*vol to temp*/
    {-18,      2161},   /*vol to temp*/
    {-17,      2146},   /*vol to temp*/
    {-16,      2131},   /*vol to temp*/
    {-15,      2116},   /*vol to temp*/
    {-14,      2100},   /*vol to temp*/
    {-13,      2084},   /*vol to temp*/
    {-12,      2067},    /*vol to temp*/
    {-11,      2050},    /*vol to temp*/
    {-10,      2032},    /*vol to temp*/
    {-9,      2014},    /*vol to temp*/
    {-8,      1996},    /*vol to temp*/
    {-7,      1977},    /*vol to temp*/
    {-6 ,       1958},    /*vol to temp*/
    {-5 ,       1939},    /*vol to temp*/
    {-4 ,       1919},    /*vol to temp*/
    {-3 ,       1898},    /*vol to temp*/
    {-2 ,       1878},    /*vol to temp*/
    {-1 ,       1857},    /*vol to temp*/
    {0 ,       1836},    /*vol to temp*/
    {1 ,       1814},    /*vol to temp*/
    {2 ,       1792},    /*vol to temp*/
    {3     ,    1770},    /*vol to temp*/
    {4     ,    1748},    /*vol to temp*/
    {5     ,    1725},    /*vol to temp*/
    {6     ,    1703},    /*vol to temp*/
    {7     ,    1680},    /*vol to temp*/
    {8     ,    1656},    /*vol to temp*/
    {9     ,    1633},    /*vol to temp*/
    {10     ,    1609},    /*vol to temp*/
    {11     ,    1586},    /*vol to temp*/
    {12     ,    1562},    /*vol to temp*/
    {13 ,       1538},    /*vol to temp*/
    {14 ,       1514},    /*vol to temp*/
    {15 ,       1489},    /*vol to temp*/
    {16 ,       1466},    /*vol to temp*/
    {17 ,       1442},    /*vol to temp*/
    {18 ,       1417},    /*vol to temp*/
    {19 ,       1393},    /*vol to temp*/
    {20 ,       1369},    /*vol to temp*/
    {21 ,       1345},    /*vol to temp*/
    {22 ,       1321},    /*vol to temp*/
    {23 ,       1297},    /*vol to temp*/
    {24 ,       1273},    /*vol to temp*/
    {25 ,       1250},    /*vol to temp*/
    {26 ,       1227},    /*vol to temp*/
    {27 ,       1203},    /*vol to temp*/
    {28 ,       1180},    /*vol to temp*/
    {29 ,       1157},    /*vol to temp*/
    {30 ,       1134},    /*vol to temp*/
    {31 ,       1112},    /*vol to temp*/
    {32 ,       1090},    /*vol to temp*/
    {33 ,       1068},    /*vol to temp*/
    {34 ,       1046},    /*vol to temp*/
    {35 ,       1024},    /*vol to temp*/
    {36 ,       1003},    /*vol to temp*/
    {37 ,       982},    /*vol to temp*/
    {38 ,       961},    /*vol to temp*/
    {39 ,       941},    /*vol to temp*/
    {40 ,        921},    /*vol to temp*/
    {41 ,        901},    /*vol to temp*/
    {42 ,        881},    /*vol to temp*/
    {43 ,        862},    /*vol to temp*/
    {44 ,        843},    /*vol to temp*/
    {45 ,        824},    /*vol to temp*/
    {46 ,        806},    /*vol to temp*/
    {47 ,        787},    /*vol to temp*/
    {48 ,        770},    /*vol to temp*/
    {49 ,        752},    /*vol to temp*/
    {50 ,        735},    /*vol to temp*/
    {51 ,        718},    /*vol to temp*/
    {52 ,        702},    /*vol to temp*/
    {53 ,        686},    /*vol to temp*/
    {54 ,        670},    /*vol to temp*/
    {55 ,        654},    /*vol to temp*/
    {56 ,        639},    /*vol to temp*/
    {57 ,        624},    /*vol to temp*/
    {58 ,        609},    /*vol to temp*/
    {59 ,        595},    /*vol to temp*/
    {60 ,        581},     /*vol to temp*/
    {61 ,        567},     /*vol to temp*/
    {62 ,        554},     /*vol to temp*/
    {63 ,        541},     /*vol to temp*/
    {64 ,        528},     /*vol to temp*/
    {65 ,        515},     /*vol to temp*/
    {66,         503},     /*vol to temp*/
    {67 ,        491},     /*vol to temp*/
    {68 ,        479},     /*vol to temp*/
    {69 ,        468},     /*vol to temp*/
    {70 ,        456},     /*vol to temp*/
    {71 ,        445},     /*vol to temp*/
    {72 ,        434},     /*vol to temp*/
    {73 ,        424},     /*vol to temp*/
    {74 ,        414},     /*vol to temp*/
    {75 ,        404},     /*vol to temp*/
    {76 ,        395},     /*vol to temp*/
    {77 ,        385},     /*vol to temp*/
    {78 ,        376},     /*vol to temp*/
    {79 ,        367},     /*vol to temp*/
    {80 ,        358},     /*vol to temp*/
    {81 ,        349},     /*vol to temp*/
    {82 ,        341},     /*vol to temp*/
    {83 ,        333},     /*vol to temp*/
    {84 ,        325},     /*vol to temp*/
    {85,         317},     /*vol to temp*/
};
#elif defined(BSP_CONFIG_BOARD_E5_E5578)
const  CHG_TEMP2ADC_TYPE  g_adc_batt_therm_map[] =
{
//该表由硬件提供
    {-30,      2302},   /*vol to temp*/
    {-29,      2293},   /*vol to temp*/
    {-28,      2283},   /*vol to temp*/
    {-27,      2274},   /*vol to temp*/
    {-26,      2263},   /*vol to temp*/
    {-25,      2252},   /*vol to temp*/
    {-24,      2241},   /*vol to temp*/
    {-23,      2229},   /*vol to temp*/
    {-22,      2217},   /*vol to temp*/
    {-21,      2204},   /*vol to temp*/
    {-20,      2191},   /*vol to temp*/
    {-19,      2177},   /*vol to temp*/
    {-18,      2163},   /*vol to temp*/
    {-17,      2148},   /*vol to temp*/
    {-16,      2133},   /*vol to temp*/
    {-15,      2117},   /*vol to temp*/
    {-14,      2101},   /*vol to temp*/
    {-13,      2084},   /*vol to temp*/
    {-12,      2066},    /*vol to temp*/
    {-11,      2048},    /*vol to temp*/
    {-10,      2030},    /*vol to temp*/
    {-9,      2010},    /*vol to temp*/
    {-8,      1991},    /*vol to temp*/
    {-7,      1971},    /*vol to temp*/
    {-6 ,       1950},    /*vol to temp*/
    {-5 ,       1929},    /*vol to temp*/
    {-4 ,       1908},    /*vol to temp*/
    {-3 ,       1886},    /*vol to temp*/
    {-2 ,       1863},    /*vol to temp*/
    {-1 ,       1840},    /*vol to temp*/
    {0 ,       1817},    /*vol to temp*/
    {1 ,       1793},    /*vol to temp*/
    {2 ,       1769},    /*vol to temp*/
    {3     ,    1744},    /*vol to temp*/
    {4     ,    1719},    /*vol to temp*/
    {5     ,    1694},    /*vol to temp*/
    {6     ,    1668},    /*vol to temp*/
    {7     ,    1643},    /*vol to temp*/
    {8     ,    1617},    /*vol to temp*/
    {9     ,    1590},    /*vol to temp*/
    {10     ,    1564},    /*vol to temp*/
    {11     ,    1537},    /*vol to temp*/
    {12     ,    1510},    /*vol to temp*/
    {13 ,       1483},    /*vol to temp*/
    {14 ,       1456},    /*vol to temp*/
    {15 ,       1429},    /*vol to temp*/
    {16 ,       1415},    /*vol to temp*/
    {17 ,       1402},    /*vol to temp*/
    {18 ,       1375},    /*vol to temp*/
    {19 ,       1348},    /*vol to temp*/
    {20 ,       1320},    /*vol to temp*/
    {21 ,       1293},    /*vol to temp*/
    {22 ,       1267},    /*vol to temp*/
    {23 ,       1240},    /*vol to temp*/
    {24 ,       1213},    /*vol to temp*/
    {25 ,       1187},    /*vol to temp*/
    {26 ,       1160},    /*vol to temp*/
    {27 ,       1134},    /*vol to temp*/
    {28 ,       1125},    /*vol to temp*/
    {29 ,       1117},    /*vol to temp*/
    {30 ,       1100},    /*vol to temp*/
    {31 ,       1075},    /*vol to temp*/
    {32 ,       1050},    /*vol to temp*/
    {33 ,       1025},    /*vol to temp*/
    {34 ,       1000},    /*vol to temp*/
    {35 ,       976},    /*vol to temp*/
    {36 ,       951},    /*vol to temp*/
    {37 ,       928},    /*vol to temp*/
    {38 ,       905},    /*vol to temp*/
    {39 ,       882},    /*vol to temp*/
    {40 ,        860},    /*vol to temp*/
    {41 ,        838},    /*vol to temp*/
    {42 ,        816},    /*vol to temp*/
    {43 ,        795},    /*vol to temp*/
    {44 ,        774},    /*vol to temp*/
    {45 ,        753},    /*vol to temp*/
    {46 ,        737},    /*vol to temp*/
    {47 ,        727},    /*vol to temp*/
    {48 ,        714},    /*vol to temp*/
    {49 ,        695},    /*vol to temp*/
    {50 ,        676},    /*vol to temp*/
    {51 ,        658},    /*vol to temp*/
    {52 ,        640},    /*vol to temp*/
    {53 ,        622},    /*vol to temp*/
    {54 ,        605},    /*vol to temp*/
    {55 ,        589},    /*vol to temp*/
    {56 ,        573},    /*vol to temp*/
    {57 ,        557},    /*vol to temp*/
    {58 ,        541},    /*vol to temp*/
    {59 ,        526},    /*vol to temp*/
    {60 ,        511},     /*vol to temp*/
    {61 ,        497},     /*vol to temp*/
    {62 ,        483},     /*vol to temp*/
    {63 ,        470},     /*vol to temp*/
    {64 ,        457},     /*vol to temp*/
    {65 ,        444},     /*vol to temp*/
    {66,         431},     /*vol to temp*/
    {67 ,        419},     /*vol to temp*/
    {68 ,        407},     /*vol to temp*/
    {69 ,        395},     /*vol to temp*/
    {70 ,        384},     /*vol to temp*/
    {71 ,        373},     /*vol to temp*/
    {72 ,        363},     /*vol to temp*/
    {73 ,        353},     /*vol to temp*/
    {74 ,        343},     /*vol to temp*/
    {75 ,        334},     /*vol to temp*/
    {76 ,        324},     /*vol to temp*/
    {77 ,        315},     /*vol to temp*/
    {78 ,        306},     /*vol to temp*/
    {79 ,        297},     /*vol to temp*/
    {80 ,        289},     /*vol to temp*/
    {81 ,        282},     /*vol to temp*/
    {82 ,        274},     /*vol to temp*/
    {83 ,        266},     /*vol to temp*/
    {84 ,        259},     /*vol to temp*/
    {85,         169},     /*vol to temp*/
};
#else
const  CHG_TEMP2ADC_TYPE  g_adc_batt_therm_map[] =
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
#endif
/*该表由硬件提供*/
/* 板级电池温度NTC实际温度校准表，
7x25平台该表的值在ADC模块修改，EC5075产品板级电池NTC没用到，预留*/
const  CHG_TEMP2ADC_TYPE  g_adc_battbtm_therm_map[] =
{
    {-30, 2416}, /* */
    {-25,2411},  /* */
    {-20,2353},  /* */
    {-15,2282},  /* */
    {-10,2197},  /* */
    {-5,2173},   /* */
    {0,2058},    /* */
    {5,1927},    /* */
    {10,1782},   /* */
    {15,1623},   /* */
    {20,1463},   /* */
    {25,1300},   /* */
    {30,1141},   /* */
    {35,991},    /* */
    {40,853},    /* */
    {45,729},    /* */
    {50,619},    /* */
    {55,523},    /* */
    {60,441},    /* */
    {65,372},    /* */
    {70,313},    /* */
    {75,263},    /* */
    {80,222},    /* */
    {85,187},    /* */
    {90,158},    /* */
    {95,134},    /* */
    {100,113},   /* */
    {105,97},    /* */
    {110,83},    /* */
    {115,71},    /* */
};

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
/* 电池校准的最小及最大电压**/
#define CHG_BATT_CALI_MIN_VOLT  (3400)
#define CHG_BATT_CALI_MAX_VOLT  (4200)

#define CHG_VBATT_CONVERT_PARA                 (3)
#define MICRO_TO_MILLI_V                   (1000)

#define MSTOUS 1000


/*****************************************************************************
 函 数 名  : chg_boot_delay_ms
 功能描述  : boot阶段的延时函数，不同平台需要进行适配
 输入参数  : unsigned int microsecond
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年6月27日
    作    者   : chendongguo
    修改内容   : 新生成函数

*****************************************************************************/
void chg_boot_delay_ms (uint32_t microsecond)
{
#if (MBB_CHG_PLATFORM_V7R2 == FEATURE_ON)
    /*lint -e18*/
    sleep(microsecond);
    /*lint +e18*/
#else
    DALSYS_BusyWait(MSTOUS * microsecond);
#endif /*MBB_CHG_PLATFORM_V7R2 == YES*/
}


#if (MBB_CHG_PLATFORM_9X25 == FEATURE_ON)
int32_t bsp_i2c_read(uint8_t reg)
{
    uint8_t error_ind = 0;
    int32_t ret = 0;
    uint8_t chg_bq_val = 0;

    chg_boot_delay_ms(1);/*wait 1 ms*/
    error_ind = i2c_write_v(&reg, sizeof(reg), SBL1_BQ24192_SLAVE_I2C_ADDR);
    if(error_ind == SBL2_I2C_V_ERROR)
    {
        ret = -1;
    }

    error_ind = i2c_read_v(&chg_bq_val, sizeof(chg_bq_val), SBL1_BQ24192_SLAVE_I2C_ADDR);

    if(error_ind == SBL2_I2C_V_ERROR)
    {
        ret = -1;
    }

    ret = (int32_t)chg_bq_val;

    return ret;
}
int32_t bsp_i2c_write(uint8_t reg, int32_t value)
{
    uint8_t error_ind = 0;
    int32_t ret = 0;
    uint8_t chg_bq_reg[I2C_ADDR_N_DATA] = {0};

    chg_bq_reg[0] = reg;
    chg_bq_reg[1] = (uint8_t)value;

    chg_boot_delay_ms(1);/*wait 1 ms*/
    error_ind = i2c_write_v(chg_bq_reg, sizeof(chg_bq_reg), SBL1_BQ24192_SLAVE_I2C_ADDR);

    if(error_ind == SBL2_I2C_V_ERROR)
    {
        ret = -1;
    }
    return ret;
}
#endif /*MBB_CHG_PLATFORM_9X25 == YES*/

/* Begin daizhicheng 2013-07-01 added */
/*****************************************************************************
 函 数 名  : chg_print_level_message.fmt
 功能描述  : LINUX平台通用的打印函数
 输入参数  : int print_level
             const char *fmt...
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年5月8日
    作    者   : chendongguo
    修改内容   : 新生成函数

*****************************************************************************/
/*lint -e530 -e611*/
void chg_print_level_message(int print_level,const char *fmt,...)
{
    va_list ap;
    
    /*小于chg_print_level级别的LOG可以被打印*/
    if((uint32_t)print_level < CHG_BOOT_LOG_LEVEL)
    {     
    #if (MBB_CHG_PLATFORM_V7R2 == FEATURE_ON)
        va_start(ap, fmt);
        __xprintf(fmt, ap, (void*) console_putc, 0);
        va_end(ap);
        console_flush();
    #endif
    }
}
/*lint +e530 +e611*/
/* End daizhicheng 2013-07-01 added */

/**********************************************************************
函 数 名  : chg_boot_is_ftm_mode
功能描述  : 判断目前是否为ftm模式
输入参数  : 无。
输出参数  : 无。
返 回 值  : 目前是否为工厂模式
           TRUE : 工厂模式
           FALSE: 非工厂模式
注意事项  : 无。
***********************************************************************/
boolean chg_boot_is_ftm_mode(void)
{
    int rt = 0;
    uint32_t is_ftm_mode = 0;

    rt = bsp_nvm_read(NV_BOOT_FACTORY_MODE, &is_ftm_mode, sizeof(uint32_t));

    if(0 != rt)
    {
        chg_print_level_message( CHG_MSG_ERR,"CHG:read ftm mode NV failed!!!\r\n ",0,0,0 );
        #ifdef MBB_FACTORY_FEATURE
            return TRUE;
        #else
            return FALSE;
        #endif
    }
    else
    {
        chg_print_level_message( CHG_MSG_ERR,"CHG:read NV success,mode=%d\r\n ",is_ftm_mode,0,0 );
        return (!is_ftm_mode);
    }
    return FALSE;
}
/**********************************************************************
函 数 名  : chg_boot_is_no_battery_powerup_enable
功能描述  : 判断目前是否为非工厂模式下的电池不在位开机使能模式
输入参数  : 无。
输出参数  : 无。
返 回 值  : 目前是否为非工厂模式电池不在位开机使能
                           TRUE : 使能
                           FALSE: 非使能
注意事项  : 无。
***********************************************************************/
boolean chg_boot_is_no_battery_powerup_enable(void)
{
    int rt = 0;
    BOOT_POWERUP_MODE_TYPE powerup_mode_value;

    memset( (void *)&powerup_mode_value, 0, sizeof(BOOT_POWERUP_MODE_TYPE) );
    rt = bsp_nvm_read(NV_BOOT_POWERUP_MODE, &powerup_mode_value, sizeof(BOOT_POWERUP_MODE_TYPE));
    if(0 != rt)
    {
        chg_print_level_message( CHG_MSG_ERR,"CHG:read NV failed!!!\r\n ",0,0,0 );

        return FALSE;
    }
    else
    {
        chg_print_level_message( CHG_MSG_ERR,"CHG:no_battery_powerup_enable=%d\r\n ",
        powerup_mode_value.no_battery_powerup_enable,0,0 );
        return powerup_mode_value.no_battery_powerup_enable;
    }   
    return FALSE;
}

/**********************************************************************
函 数 名  : chg_boot_is_exception_poweroff_poweron_enable
功能描述  : 判断目前是否为插入USB强制开机使能模式
输入参数  : 无。
输出参数  : 无。
返 回 值  : 目前是否为插入USB强制开机使能模式
                           TRUE : 使能
                           FALSE: 非使能
注意事项  : 无。
***********************************************************************/
boolean chg_boot_is_exception_poweroff_poweron_enable(void)
{
    int rt = 0;
    BOOT_POWERUP_MODE_TYPE powerup_mode_value;

    memset( (void *)&powerup_mode_value, 0, sizeof(BOOT_POWERUP_MODE_TYPE) );
    rt = bsp_nvm_read(NV_BOOT_POWERUP_MODE, &powerup_mode_value, sizeof(BOOT_POWERUP_MODE_TYPE));
    if(0 != rt)
    {
        chg_print_level_message( CHG_MSG_ERR,"CHG:read NV failed!!!\r\n ",0,0,0 );
        return FALSE;
    }
    else
    {
        chg_print_level_message( CHG_MSG_ERR,"CHG:exception_poweroff_poweron_enable=%d\r\n ",
        powerup_mode_value.exception_poweroff_poweron_enable,0,0 );
        return powerup_mode_value.exception_poweroff_poweron_enable;
    }
    
    return FALSE;
}

/**********************************************************************
FUNCTION:    CHG_SET_POWER_OFF
DESCRIPTION: The poweroff func of CHG module, all the power-off operation
             except at boot phase MUST be performed via calling this.
INPUT:       The shutdown reason which triggered system poweroff.
             All VALID REASON:
             DRV_SHUTDOWN_BATTERY_ERROR --BATTERY ERROR;
             DRV_SHUTDOWN_TEMPERATURE_PROTECT --EXTREAM HIGH TEMPERATURE.
             DRV_SHUTDOWN_LOW_TEMP_PROTECT --EXTREAM LOW TEMPERATURE
             DRV_SHUTDOWN_CHARGE_REMOVE --CHGR REMOVAL WHILE POWEROFF CHG
             DRV_SHUTDOWN_LOW_BATTERY --LOW BATTERY
OUTPUT:      None.
RETURN:      None.
NOTE:        When this function get called to power system off, it record
             the shutdown reason, then simulate POWER_KEY event to APP to
             perform the real system shutdown process.
             THUS, THIS FUNCTION DOESN'T TAKE AFFECT IF APP DIDN'T STARTUP.
***********************************************************************/
/***************Note:平台相关代码，根据平台按需要添加，有的平台如9x25需要
              移植人员根据需要，添加或者移除下边函数调用***************************/
void chg_boot_set_power_off(DRV_SHUTDOWN_REASON_ENUM real_reason)
{
    chg_print_level_message(CHG_MSG_ERR, "chg_boot_set_power_off:power off by reason = %d \n ",real_reason,0,0);
#if (MBB_CHG_PLATFORM_V7R2 == FEATURE_ON)
    /* Begin daizhicheng 2013-07-01 added*/
    /*lint -e18*/
    boot_power_off();
    /*lint +e18*/
    /* End daizhicheng 2013-07-01 added*/
#else

    sbl1_poweroff();
#endif /*MBB_CHG_PLATFORM_V7R2 == YES*/
    return;
}

/*****************************************************************************
  函 数 名  : chg_boot_get_start_mode
  功能描述  : 通过读取共享内存，获取当前开机状态
  输入参数  : 无
  输出参数  : 无
  返 回 值  : DRV_START_MODE_ENUM类型
  调用函数  :
  被调函数  :

  修改历史      :
   1.日    期   : 2012年10月30日
     修改内容   : 新生成函数

*****************************************************************************/
#if (MBB_CHG_PLATFORM_V7R2 == FEATURE_OFF)
DRV_START_MODE_ENUM chg_boot_get_start_mode(void)
{
#if (MBB_CHG_PLATFORM_V7R2 == FEATURE_ON)


    /*V7R2平台不需要用这个接口*/
    return DRV_START_MODE_BUTT;
#else
    huawei_smem_info *smem_data = NULL;

    /*通过读取共享内存，获取当前开机状态，此处由平台移植人员填写,
    根据开机状态，区分是充电开机模式，正常开机模式，异常开机模式*/
    smem_data = (huawei_smem_info *)SMEM_HUAWEI_ALLOC(SMEM_ID_VENDOR0 , sizeof(huawei_smem_info ));

    /*coverity*/
    if (NULL == smem_data)
    {
        return DRV_START_MODE_NORMAL;
    }

    /*start_mode_flag = */
    switch(smem_data->smem_huawei_poweroff_chg)
    {
        /*关机充电*/
        case POWER_ON_STATUS_FINISH_CHARGE:
        {
            /*break;*/
            return DRV_START_MODE_CHARGING;
        }
        /*正常开机*/
        case POWER_ON_STATUS_FINISH_NORMAL:
        {
            /*break;*/
            return DRV_START_MODE_NORMAL;
        }
        /*case 2:*/
        case POWER_ON_STATUS_BOOTING:
        case POWER_ON_STATUS_CHARGING:
        case POWER_ON_STATUS_CAPACITY_LOW:
        case POWER_ON_STATUS_BATTERY_ERROR:
        default:
        {
            /*break;*/
            return DRV_START_MODE_EXCEPTION;
        }
    }
#endif /*MBB_CHG_PLATFORM_V7R2 == YES*/
}
#endif
/**********************************************************************
函 数 名  : chg_boot_is_powdown_charging
功能描述  :  判断当前是否为关机充电模式
输入参数  : 无。
输出参数  : 无。
返 回 值  : TRUE:  关机充电模式
            FALSE:  非关机充电模式
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年10月30日
    修改内容   : 新生成函数

***********************************************************************/
boolean chg_boot_is_powdown_charging (void)
{
    DRV_START_MODE_ENUM start_mode = DRV_START_MODE_BUTT;

#ifdef MBB_FACTORY_FEATURE
return FALSE;
#endif
#if (MBB_CHG_PLATFORM_V7R2 == FEATURE_OFF)
    start_mode = chg_boot_get_start_mode();
    if(DRV_START_MODE_CHARGING == start_mode)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
#else
    return get_pw_charge_flag();
#endif
}

/**********************************************************************
函 数 名  : chg_boot_set_powdown_charging
功能描述  : 设置当前是否为关机充电模式
输入参数  : 无。
输出参数  : 无。
返 回 值  : 无
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年10月30日
    修改内容   : 新生成函数

***********************************************************************/
void chg_boot_set_powdown_charging (void)
{
/***************Note:平台相关代码，根据平台按需要添加，有的平台如9x25需要
              移植人员根据需要，添加或者移除下边函数调用***************************/
#if (MBB_CHG_PLATFORM_V7R2 == FEATURE_ON)
    set_pw_charge_flag(TRUE);
#else
    huawei_smem_info *smem_data = NULL;
    /*设置为关机充电模式*/
    smem_data = (huawei_smem_info *)SMEM_HUAWEI_ALLOC(SMEM_ID_VENDOR0 ,sizeof(huawei_smem_info ));
    if (NULL == smem_data)
    {
        return;
    }
    smem_data->smem_huawei_poweroff_chg = POWER_ON_STATUS_FINISH_CHARGE;
#endif /*MBB_CHG_PLATFORM_V7R2 == YES*/
}
#if (MBB_CHG_PLATFORM_V7R2 == FEATURE_ON)
void chg_boot_display_string(char* string)
{
    if(NULL == string)
    {
        return;
    }
#if (MBB_CHG_LCD == FEATURE_ON)
    (void)balong_lcd_display_allblack();
    lcd_show_string(string);
#endif
}
#endif
/*****************************************************************************
 函 数 名  : chg_boot_lcd_display
 功能描述  : 封装了img和string的显示
 输入参数  : CHG_BATT_DISPLAY_TYPE disp_type显示类型
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年10月30日
    修改内容   : 新生成函数

*****************************************************************************/
void chg_boot_lcd_display(CHG_BATT_DISPLAY_TYPE disp_type)
{
    int ret = -1;
    
    switch(disp_type)
    {
        case CHG_DISP_OK:

            //LCD驱动接口，由移植人员根据实际情况封装；
            break;
        case CHG_DISP_FAIL:

            //LCD驱动接口，由移植人员根据实际情况封装；
            break;
        case CHG_DISP_BATTERY_LOWER:
        #if (MBB_CHG_PLATFORM_V7R2 == FEATURE_ON)
            chg_boot_display_string("Low Battery");
        #else
            //LCD驱动接口，由移植人员根据实际情况封装；
            huawei_charge_lcd_print(CHG_LOW_BATTERY);
        #endif
            break;
        case CHG_DISP_BATTERY_BAD:
        #if (MBB_CHG_PLATFORM_V7R2 == FEATURE_ON)
            chg_boot_display_string("Battery Error");
        #else
            //LCD驱动接口，由移植人员根据实际情况封装；
            huawei_charge_lcd_print(CHG_BATTERY_ERROR);
        #endif
            break;
        case CHG_DISP_OVER_HEATED:
        #if (MBB_CHG_PLATFORM_V7R2 == FEATURE_ON)
            chg_boot_display_string("Battery Overtemp");
        #else
            //LCD驱动接口，由移植人员根据实际情况封装；
            huawei_charge_lcd_print(CHG_OVER_TEMPERATURE);
        #endif
            break;
        case CHG_DISP_TEMP_LOW:
        #if (MBB_CHG_PLATFORM_V7R2 == FEATURE_ON)
            chg_boot_display_string("Battery Overtemp");
        #else
            //LCD驱动接口，由移植人员根据实际情况封装；
            huawei_charge_lcd_print(CHG_OVER_TEMPERATURE);
        #endif
            break;
        case CHG_DISP_CHARGING:
        #if (MBB_CHG_PLATFORM_V7R2 == FEATURE_ON)
            chg_boot_display_string("charging");
        #else
            //LCD驱动接口，由移植人员根据实际情况封装；
            huawei_charge_lcd_print(CHG_POWER_OFF_CHARGING);
        #endif
            break;
        default:
            chg_print_level_message( CHG_MSG_ERR,"chg_boot_lcd_display: disp_type invalid!!!\r\n ",0,0,0 );
            break;
    }
    return;
}

#if (MBB_LED == FEATURE_ON)

#define LED_LTE_RED         "lte_led:red"
#define LED_LTE_GREEN       "lte_led:green"
#define LED_BREATH          "breath_led"
#define LED_BATT_RED        "bat_led:red" 
#define LED_BATT_GREEN      "bat_led:green" 

#define LED_ON     (1)
#define LED_OFF    (0)

void chg_led_batt_blink(void)
{
    static int blink_flag = 1;
    
    led_boot_status_set(LED_LTE_RED, LED_OFF);
    led_boot_status_set(LED_LTE_GREEN, LED_OFF);
    led_boot_status_set(LED_BREATH, LED_OFF);
    led_boot_status_set(LED_BATT_RED, LED_OFF);
    if(blink_flag)
    {
        blink_flag = 0;
        led_boot_status_set(LED_BATT_GREEN, LED_OFF);        
    }
    else
    {
        blink_flag = 1;        
        led_boot_status_set(LED_BATT_GREEN, LED_ON);        
    }
}
void chg_led_batt_lower(void)
{
    led_boot_status_set(LED_LTE_RED, LED_OFF);
    led_boot_status_set(LED_LTE_GREEN, LED_OFF);
    led_boot_status_set(LED_BREATH, LED_OFF);
    led_boot_status_set(LED_BATT_RED, LED_ON);
    led_boot_status_set(LED_BATT_GREEN, LED_OFF); 
        
}

void chg_led_batt_ok(void)
{
#if defined(BSP_CONFIG_BOARD_E5)
    led_boot_status_set(LED_BATT_RED, LED_OFF);
    led_boot_status_set(LED_BATT_GREEN, LED_ON);
	led_boot_status_set(LED_LTE_GREEN, LED_ON);
#endif
}

void chg_led_all_blink(void)
{
    static int blink_flag = 1;
    
    led_boot_status_set(LED_LTE_RED, LED_OFF);
    led_boot_status_set(LED_BATT_RED, LED_OFF);
    
    if(blink_flag)
    {
        blink_flag = 0;

        led_boot_status_set(LED_LTE_GREEN, LED_OFF);
        led_boot_status_set(LED_BREATH, LED_OFF);
        led_boot_status_set(LED_BATT_GREEN, LED_OFF);        
    }
    else
    {
        blink_flag = 1;        
        led_boot_status_set(LED_LTE_GREEN, LED_ON);
        led_boot_status_set(LED_BREATH, LED_ON);
        led_boot_status_set(LED_BATT_GREEN, LED_ON);        
    }
}

#else

void chg_led_batt_blink(void)
{
    return;
}
void chg_led_batt_lower(void)
{
    return;    
}
void chg_led_batt_ok(void)
{
    return;
}
void chg_led_all_blink(void)
{
    return;    
}
 
#endif
/*****************************************************************************
 函 数 名  : chg_boot_led_display
 功能描述  : 封装底软的闪灯模式
 输入参数  : CHG_BATT_DISPLAY_TYPE disp_type显示类型
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年10月30日
    修改内容   : 新生成函数

*****************************************************************************/
void chg_boot_led_display(CHG_BATT_DISPLAY_TYPE disp_type)
{


    switch(disp_type)
    {
        case CHG_DISP_OK:
            /*调用闪灯接口，用于显示电池正常情况下的led状态*/
            chg_led_batt_ok();
            break;
        case CHG_DISP_FAIL:
            //log指示，暂不支持(闪灯产品目前都是可拆卸电池);
            chg_print_level_message( CHG_MSG_ERR,"chg_boot_led_display: led product not support!!!\r\n ",0,0,0 );
            break;
        case CHG_DISP_BATTERY_LOWER:
            /*调用闪灯接口，红灯长亮，其他熄灭*/
            chg_led_batt_lower();           
            break;
        case CHG_DISP_CHARGING:     
            chg_led_batt_blink();
            break;          
        case CHG_DISP_BATTERY_BAD:
        case CHG_DISP_OVER_HEATED: /*指高温关机而非停充*/
        case CHG_DISP_TEMP_LOW: /*指低温关机而非停充*/
            /*调用绿灯全闪接口*/
            chg_led_all_blink();            
            break;
        default:
            chg_print_level_message( CHG_MSG_ERR,"chg_boot_led_display: disp_type invalid!!!\r\n ",0,0,0 );
            break;
    }
    return;
}

/*****************************************************************************
 函 数 名  : chg_boot_oled_display
 功能描述  : 0.96 OLED 屏BOOT 阶段显示接口
 输入参数  : CHG_BATT_DISPLAY_TYPE disp_type显示类型
 输出参数  : 无
 返 回 值  : 无
 说明 :  OLED 字符串显示暂无接口，使用整屏图片刷新
 调用函数  :
 被调函数  :
*****************************************************************************/
void chg_boot_oled_display(CHG_BATT_DISPLAY_TYPE disp_type)
{
#if (MBB_OLED == FEATURE_ON)
    oled_boot_chg_display(disp_type);
#else
    return;
#endif

}
/*****************************************************************************
 函 数 名  : chg_boot_display_interface
 功能描述  : 封装底软统一显示接口
 输入参数  : CHG_BATT_DISPLAY_TYPE disp_type显示类型
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年10月30日
    修改内容   : 新生成函数

*****************************************************************************/
void chg_boot_display_interface(CHG_BATT_DISPLAY_TYPE disp_type)
{

#if (MBB_CHG_LCD == FEATURE_ON)
    if (disp_state == disp_type)
    {
        return;
    }
    
    chg_boot_lcd_display(disp_type);
    disp_state = disp_type;
#elif (MBB_CHG_LED == FEATURE_ON)
    chg_boot_led_display(disp_type);
#elif (MBB_CHG_OLED == FEATURE_ON)
    if (disp_state == disp_type)
    {
        return;
    }
    chg_boot_oled_display(disp_type);
    disp_state = disp_type; 
#endif
    
    return;
}

#if (MBB_CHG_BQ27510 == FEATURE_ON)
/******************************************************************************
  Function      boot_bq27510_i2c_read
  Description   读取I2C设备指定寄存器地址的值
  Input         reg   :寄存器地址
  Output        *pval :用于将寄存器的值写入该整型数据所在地址
  Return        0     :函数执行成功
                -1    :函数执行失败
  Others        N/A
******************************************************************************/
int32_t boot_bq27510_i2c_read(uint8_t reg, uint16_t *pval)
{
    int32_t ret = 0;
    uint8_t chg_bq_reg = 0;
    chg_bq_reg = reg;

    uint8_t slave_id = bq27510_SLAVE_I2C_ADDR;
    uint16_t read_data = 0;
    ret = bsp_i2c_byte_data_receive((uint8_t)slave_id, (uint8_t)chg_bq_reg, &read_data);
    if(ret < 0)
    {
        chg_print_level_message(CHG_MSG_ERR, "bsp_i2c_byte_data_receive fail%d\n",0,0,0);
        return -1;
    }
    else
    {
        *pval = (uint16_t)read_data;
        return 0;
    }
}

/******************************************************************************
  Function      boot_bq27510_i2c_write
  Description   对I2C设备寄存器指定地址写入数值
  Input         reg  : 寄存器地址
                val  : 希望写入上述寄存器地址的值
  Output        N/A
  Return        0    : 函数执行成功
                -1   : 函数执行失败
  Others        N/A
******************************************************************************/
int32_t boot_bq27510_i2c_write(uint8_t reg, uint8_t val)
{
    int32_t ret = 0;
    uint8_t chg_bq_reg = 0;
    chg_bq_reg = reg;

    uint8_t slave_id = bq27510_SLAVE_I2C_ADDR;

    ret = bsp_i2c_byte_data_send((uint8_t)slave_id,(uint8_t)chg_bq_reg, (uint8_t)val);

    if(ret < 0)
    {
        chg_print_level_message(CHG_MSG_ERR, "bsp_i2c_byte_data_send fail%d\n",0,0,0);
        return -1;
    }
    else
    {
        return 0;
    }
}

/*****************************************************************************
 函 数 名  : chg_boot_bq27510_get_volt
 功能描述  : 从库仑计获取电池电压值
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 读取的电池电压值
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年9月3日
    作    者   : zhaochw zwx227638
    修改内容   : 新生成函数

*****************************************************************************/
int32_t chg_boot_bq27510_get_volt(void)
{
	uint8_t reg           = 0;
    uint16_t reg_val_low  = 0;
	uint16_t reg_val_high = 0;
    int32_t ret_low       = 0;
	int32_t ret_high      = 0;
	int32_t batt_volt    = 0;

    reg = BQ27510_REG_VOLT;

    ret_low = boot_bq27510_i2c_read(reg, &reg_val_low);
	ret_high = boot_bq27510_i2c_read(reg+1, &reg_val_high);
	
    if (0 != (ret_low || ret_high))
    {
        chg_print_level_message(CHG_MSG_ERR, "ERROR READING chg_boot_bq27510_get_volt!\n", 0,0,0);
        return FALSE;
    }
    else
    {
        chg_print_level_message(CHG_MSG_ERR, "[zcw_test]:boot_bq27510 read volt val_low = %x; val_high = %x!\n",
                             reg_val_low, reg_val_high,0);
		batt_volt = reg_val_low | (reg_val_high<<8);
		chg_print_level_message(CHG_MSG_ERR, "[zcw_test]:boot_bq27510 read volt batt_volt = %x!\n",
                             batt_volt, 0,0);
    }
    chg_print_level_message(CHG_MSG_DEBUG, "CHG_DRV_DBG:chg_boot_set_charge_enable() reg %d, value 0x before!\n",
                             reg, 0 ,0);
	return batt_volt;
}

/*****************************************************************************
 函 数 名  : chg_boot_bq27510_get_temp
 功能描述  : 从库仑计获取电池温度值
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 读取的电池温度值
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年9月4日
    作    者   : zhaochw zwx227638
    修改内容   : 新生成函数

*****************************************************************************/
int32_t chg_boot_bq27510_get_temp(void)
{
    uint8_t reg           = 0;
    uint16_t reg_val_low  = 0;
	uint16_t reg_val_high = 0;
    int32_t ret_low       = 0;
	int32_t ret_high      = 0;
	int32_t batt_temp    = 0;

    reg = BQ27510_REG_TEMP;

    ret_low = boot_bq27510_i2c_read(reg, &reg_val_low);
	ret_high = boot_bq27510_i2c_read(reg+1, &reg_val_high);
	
    if (0 != (ret_low || ret_high))
    {
        chg_print_level_message(CHG_MSG_ERR, "ERROR READING chg_boot_bq27510_get_temp!\n", 0,0,0);
        return FALSE;
    }
    else
    {
        chg_print_level_message(CHG_MSG_ERR, "[zcw_test]:boot_bq27510 read temp val_low = %x; val_high = %x!\n",
                             reg_val_low, reg_val_high,0);
		batt_temp = reg_val_low | (reg_val_high<<8);
		chg_print_level_message(CHG_MSG_ERR, "[zcw_test]:boot_bq27510 read temp batt_temp = %x!\n",
                             batt_temp, 0,0);
    }
    
    batt_temp = (batt_temp-CONST_NUM_2730)/CONST_NUM_10;
    chg_print_level_message(CHG_MSG_DEBUG, "CHG_DRV_DBG:chg_boot_set_charge_enable() reg %d, value 0x before!\n",
                             reg, 0 ,0);
	return batt_temp;
}
#endif

/*****************************************************************************
 函 数 名  : chg_boot_get_volt_from_adc
 功能描述  : 从ADC通道获取电压值
 输入参数  : param_type通道类型
 输出参数  : 无
 返 回 值  : 读取的电压值
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年10月31日
    修改内容   : 新生成函数

*****************************************************************************/
int32_t chg_boot_get_volt_from_adc(CHG_PARAMETER_ENUM param_type)
{
/***************Note:平台相关代码，根据平台按需要添加，有的平台如9x25需要
              移植人员根据需要，添加或者移除下边函数调用***************************/
#if defined(CONFIG_HKADC)           
    int32_t rtn_vbat_val = 0;
    int ret = 0;

    int tmp_ret = -1;
    u16 tmp_rtn_vbat_val = 0;


    /*Get the channel from where the data is needed*/
    if (CHG_PARAMETER__BATTERY_VOLT == param_type)
    {
        /*电池电压使用库仑计*/
#if (MBB_CHG_COULOMETER == FEATURE_ON)
        rtn_vbat_val = bsp_coul_voltage();
        chg_print_level_message( CHG_MSG_DEBUG,"CHG:VBAT = %d\n", rtn_vbat_val,0,0);
#elif (MBB_CHG_BQ27510 == FEATURE_ON)
        rtn_vbat_val = chg_boot_bq27510_get_volt();
	    chg_print_level_message( CHG_MSG_DEBUG,"CHG:VBAT = %d\n", rtn_vbat_val,0,0);

#else
      tmp_ret = bsp_hkadc_convert(CHG_BATT_VOLT_HKADC_ID, &tmp_rtn_vbat_val);
      rtn_vbat_val = (int32_t)(tmp_rtn_vbat_val + tmp_rtn_vbat_val);
    if (tmp_ret < 0)
        {
        chg_print_level_message( CHG_MSG_ERR,"fail to convert, return value %d\n", 
                                            tmp_ret,0,0);
        }
    else
        {
        chg_print_level_message( CHG_MSG_DEBUG,"CHG:VBAT = %d\n", 
                                            rtn_vbat_val,0,0);
        } 
#endif
    }
    else if (CHG_PARAMETER__BATT_THERM_DEGC == param_type)
    {
        ret = bsp_hkadc_convert(CHG_BATT_THERM_HKADC_ID,&rtn_vbat_val);
        if(ret)
        {
            chg_print_level_message( CHG_MSG_ERR,"chg_boot_get_volt_from_adc: get adc value failed!!!\r\n ",0,0,0 );
            return ret;
        }
        chg_print_level_message( CHG_MSG_DEBUG,"CHG:BAT_TEMP = %d\n", rtn_vbat_val,0,0);

	}
    else if (CHG_PARAMETER__VBUS_VOLT == param_type)
    {
        ret = bsp_hkadc_convert(CHG_VBUS_VOLT_HKADC_ID,&rtn_vbat_val);
        if(ret)
        {
            chg_print_level_message( CHG_MSG_ERR,"chg_boot_get_volt_from_adc: get adc value failed!!!\r\n ",0,0,0 );
            return ret;
        }
        chg_print_level_message( CHG_MSG_DEBUG,"CHG:VBUS = %d\n", rtn_vbat_val,0,0);
 
	}
    else if (CHG_PARAMETER__BATTERY_ID_VOLT == param_type)
    {
        ret = bsp_hkadc_convert(CHG_VBUS_VOLT_HKADC_ID,&rtn_vbat_val);
        if(ret)
        {
            chg_print_level_message( CHG_MSG_ERR,"chg_boot_get_volt_from_adc: get adc value failed!!!\r\n ",0,0,0 );
            return ret;
        }
        chg_print_level_message( CHG_MSG_DEBUG,"CHG:VBUS = %d\n", rtn_vbat_val,0,0);
 
    }

    else
    {
        chg_print_level_message( CHG_MSG_INFO,"VADC bad param_type %d\n", param_type,0,0);
        return -1;
    }
    
    return rtn_vbat_val;
#endif


#if 0
    AdcBootDeviceChannelType channel = {0};
    AdcBootResultType adc_result = {0};
    uint32_t error = 0x0;
    int32_t rtn_vbat_val = 0;
    pm_err_flag_type status = PM_ERR_FLAG__SUCCESS;

    /*Initialize the ADC*/
    error = AdcBoot_Init();
    if(0 != error)
    {
        return -1;
    }

    /*Get the channel from where the data is needed*/
    if (CHG_PARAMETER__BATTERY_VOLT == param_type)
    {
        status = pm_dev_mpp_config_analog_input(0, PM_MPP_6, PM_MPP__AIN__CH_AMUX6);
        if (status != PM_ERR_FLAG__SUCCESS)
        {
            return -1;
        }

        error = AdcBoot_GetChannel(ADC_INPUT_MPP6, &channel);
        if(0 != error)
        {
            return -1;
        }
    }
    else if (CHG_PARAMETER__BATT_THERM_DEGC == param_type)
    {
        error = AdcBoot_GetChannel(ADC_INPUT_BATT_ID, &channel);
        if(0 != error)
        {
            return -1;
        }
    }
    else if (CHG_PARAMETER__VBUS_VOLT == param_type)
    {
        error = AdcBoot_GetChannel(ADC_INPUT_PA_THERM1, &channel);
        if(0 != error)
        {
            return -1;
        }
    }
    else
    {
        chg_print_level_message( CHG_MSG_INFO,"VADC bad channel %d\n", param_type,0,0);
        return -1;
    }

    /*Read the Voltage of the Battery*/
    error = AdcBoot_Read(&channel, &adc_result);
    if(0 != error)
    {
        return -1;
    }

    /*Check for the result*/
    if(0 != adc_result.eStatus)
    {
        rtn_vbat_val = (int32_t)(adc_result.nMicrovolts / MICRO_TO_MILLI_V);       /* */

        if (CHG_PARAMETER__BATTERY_VOLT == param_type)
        {
            rtn_vbat_val = rtn_vbat_val * SAMPLE_BATTERY_VOLT_MULT;
        }
    }
    else
    {
        return -1;
    }
    return rtn_vbat_val;
#endif

}

/*****************************************************************************
 函 数 名  : chg_boot_get_batt_volt_value
 功能描述  : 获取电池电压
 输入参数  : 无
 输出参数  : 无
 返 回 值  : int32_t 电压值
 调用函数  : 无
 被调函数  :

 修改历史      :
  1.日    期   : 2012年11月1日
    修改内容   : 新生成函数

*****************************************************************************/
int32_t chg_boot_get_batt_volt_value(void)
{
    int32_t rtn_vbatt_value = 0;

    int32_t vbatt_from_adc = 0;
    vbatt_from_adc = chg_boot_get_volt_from_adc(CHG_PARAMETER__BATTERY_VOLT);

    /*boot充电不做电池电压校准*/
    rtn_vbatt_value = vbatt_from_adc;


    return rtn_vbatt_value;
}

/*****************************************************************************
 函 数 名  : chg_boot_volt_to_temp
 功能描述  : 电压到温度的转换
 输入参数  : int32_t volt_value, 输入的电压值；int * AdcTable，映射表
 输出参数  : 无
 返 回 值  : 转换后的温度值
 调用函数  : 无
 被调函数  :

 修改历史      :
  1.日    期   : 2012年11月2日
    修改内容   : 新生成函数

*****************************************************************************/
int32_t chg_boot_volt_to_temp (int32_t volt_value, const CHG_TEMP2ADC_TYPE * AdcTable, uint32_t table_size)
{
    boolean   desending_flag = TRUE;
    uint32_t  idx = 0;

    if(NULL == AdcTable)
    {
        chg_print_level_message( CHG_MSG_ERR,"chg_boot_volt_to_temp: invalid input para!!!\r\n ",0,0,0 );
        /*直接返回电压*/
        return volt_value;
    }

    /*判断表是按电压值升序or降序*/
    if(1 < table_size)
    {
        if(AdcTable[0].voltage < AdcTable[1].voltage)
        {
            desending_flag = FALSE;
        }
    }

    /*查找入参在表格中的位置*/
    while(idx < table_size)
    {
        if((TRUE == desending_flag) && (AdcTable[idx].voltage < volt_value))
        {
            break;
        }
        else if((FALSE == desending_flag) && (AdcTable[idx].voltage > volt_value))
        {
            break;
        }
        else
        {
            idx++;
        }
    }

    /*极限值判断，其他值公式求解*/
    if(0 == idx)
    {
        return AdcTable[0].temperature;
    }
    else if(table_size == idx)
    {
        return AdcTable[table_size - 1].temperature;
    }
    else
    {
        /*防止斜率计算非法*/
        if(AdcTable[idx - 1].voltage == AdcTable[idx].voltage)
        {
            chg_print_level_message( CHG_MSG_ERR,"chg_boot_volt_to_temp: slop invalid!!!\r\n ",0,0,0 );
            return AdcTable[idx - 1].temperature;
        }

        return (((AdcTable[idx - 1].temperature - AdcTable[idx].temperature ) * (volt_value - AdcTable[idx - 1].voltage))
            / (AdcTable[idx - 1].voltage - AdcTable[idx].voltage)
            + AdcTable[idx - 1].temperature);

    }
}

/*****************************************************************************
 函 数 名  : chg_boot_get_temp_value
 功能描述  : 获取温度参数
 输入参数  : param_type，参数类型
 输出参数  : 无
 返 回 值  : 获取的温度值
 调用函数  : 无
 被调函数  :

 修改历史      :
  1.日    期   : 2012年11月2日
    修改内容   : 新生成函数

*****************************************************************************/
int32_t chg_boot_get_temp_value(CHG_PARAMETER_ENUM param_type)
{
    int32_t rtn_temp_value = 0;

    int32_t voltage_from_adc = 0;
    voltage_from_adc = chg_boot_get_volt_from_adc(param_type);
    switch(param_type)
    {
        case CHG_PARAMETER__BATT_THERM_DEGC:
        {
		#if (MBB_CHG_BQ27510 == FEATURE_ON)
		    rtn_temp_value = chg_boot_bq27510_get_temp();
			rtn_temp_value = rtn_temp_value/100;
		#else
            rtn_temp_value = chg_boot_volt_to_temp(voltage_from_adc, g_adc_batt_therm_map,
                                        sizeof(g_adc_batt_therm_map) / sizeof(g_adc_batt_therm_map[0]));
		#endif
            break;
        }
        case CHG_PARAMETER__BATT_BOT_THERM_DEGC:
        {
            rtn_temp_value = chg_boot_volt_to_temp(voltage_from_adc, g_adc_battbtm_therm_map,
                                        sizeof(g_adc_battbtm_therm_map) / sizeof(g_adc_battbtm_therm_map[0]));
            break;
        }
        default:
        {
            /*先返回电压值*/
            rtn_temp_value = voltage_from_adc;
            break;
        }
    }


    return rtn_temp_value;
}
