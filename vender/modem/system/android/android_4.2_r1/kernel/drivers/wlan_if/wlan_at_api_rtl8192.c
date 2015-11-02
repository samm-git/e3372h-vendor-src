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
/*===========================================================================
                       linux系统头文件
===========================================================================*/
#ifdef WLAN_AT_API_APP_BUILD
    #include <unistd.h>
    #include <sys/wait.h>
#else
    #include <linux/module.h>
    #include <linux/kernel.h>
    #include <linux/delay.h>
#endif /*WLAN_AT_API_APP_BUILD*/

/*===========================================================================
                       平台头文件
===========================================================================*/
#include "wlan_at.h"
#include "wlan_utils.h"
#include "DrvInterface.h"
#include "product_nv_id.h"
#include "bsp_nvim.h"

/*===========================================================================
                       宏定义
===========================================================================*/
#define WLAN_AT_SSID_SUPPORT    1           /* 支持的SSID组数 */
#define WLAN_AT_KEY_SUPPORT     5           /* 支持的分组数 */
#define WLAN_AT_MODE_SUPPORT    "0,2,3,4"   /* 支持的模式(cw:0/a:1/b:2/g:3/n:4/ac:5) */
#define WLAN_AT_BAND_SUPPORT    "0,1"       /* 支持的带宽(20M:0/40M:1/80M:2/160M:3) */
#define WLAN_AT_TSELRF_SUPPORT  "0,1"         /* 支持的天线索引序列 */

/* 最大支持信道数量 */
#define WLAN_RTL_CHANNEL_24G_MAX (14)

/*WIFI功率的上下限*/
#define WLAN_AT_WIFI_POWER_MIN           (-15)
#define WLAN_AT_WIFI_POWER_MAX           (30)

/*cmd字符串长度*/
#define WIFI_CMD_MAX_SIZE   256
#define WIFI_AP_INTERFACE   "wlan0"

/* 过CE认证，配置TX占空比100% */
#define WLAN_AT_TX_FULL_DUTY_RATIO 255

/*WiFi芯片使能断言检测*/
#define IS_CW_MODE(wifi_mode) ((AT_WIMODE_80211b == wifi_mode) ? AT_FEATURE_ENABLE : AT_FEATURE_DISABLE)

/*向WiFi芯片下发配置命令*/
#define WIFI_TEST_CMD(cmd) do { \
    if (WLAN_SUCCESS != wlan_run_cmd(cmd)) \
    { \
        PLAT_WLAN_INFO("Run CMD Error!!!"); \
        return AT_RETURN_FAILURE; \
    } \
    if (NULL != strstr(cmd, " down")) \
    { \
        PLAT_WLAN_INFO("RX TX Status Clear!!!"); \
        g_wlan_at_data.WiTX = AT_FEATURE_DISABLE; \
        g_wlan_at_data.WiRX.onoff = AT_FEATURE_DISABLE; \
    } \
}while(0)

#define WIFI_SHELL_CMD(shell) do { \
    if (WLAN_SUCCESS != wlan_run_shell(shell)) \
    { \
        PLAT_WLAN_INFO("Run SHELL Error!!!"); \
        return AT_RETURN_FAILURE; \
    } \
}while(0)

/*WiFi芯片使能断言检测*/
#define ASSERT_WiFi_OFF(ret) do { \
    if (AT_WIENABLE_OFF == g_wlan_at_data.WiEnable) \
    { \
        PLAT_WLAN_INFO("Exit on WiFi OFF"); \
        return ret; \
    } \
}while(0)

#define WIFI_TOOL_IWPRIV     "/system/bin/iwpriv"                /* iwpriv工具 */
#define WIFI_TOOL_IFCONFIG   "/system/bin/busybox ifconfig"      /* ifconfig工具 */

#define WIFI_POWER_DIFF_MIN (-7) 
#define WIFI_POWER_DIFF_MAX (8)

#define IS_TSELRF_VAILD(tselrf) ((AT_TSELRF_A == tselrf) || (AT_TSELRF_B == tselrf))

#ifdef MBB_ROUTER_QCT
    #define WLAN_CURRENT_TSELRF (IS_TSELRF_VAILD(WlanATGetTSELRF())? WlanATGetTSELRF() : AT_TSELRF_A)
#else
    #define WLAN_CURRENT_TSELRF (IS_TSELRF_VAILD(g_wlan_at_data.WiTselrf)? g_wlan_at_data.WiTselrf : AT_TSELRF_A)
#endif /* MBB_ROUTER_QCT */

/* 配置命令前保证RX/TX处于停止状态 */
#define WLAN_SET_RXTX_STOP() do { \
    if (AT_FEATURE_DISABLE != g_wlan_at_data.WiRX.onoff) \
    { \
        WIFI_TEST_CMD(WIFI_TOOL_IWPRIV" "WIFI_AP_INTERFACE" mp_arx stop"); \
        g_wlan_at_data.WiRX.onoff = AT_FEATURE_DISABLE; \
    } \
    if (AT_FEATURE_DISABLE != g_wlan_at_data.WiTX) \
    { \
        WIFI_TEST_CMD(WIFI_TOOL_IWPRIV" "WIFI_AP_INTERFACE" mp_ctx stop"); \
        g_wlan_at_data.WiTX = AT_FEATURE_DISABLE; \
    } \
}while(0)

/*===========================================================================
                       类型定义
===========================================================================*/
typedef enum
{
    AT_TSELRF_A   = 0,  /*射频A 路*/
    AT_TSELRF_B   = 1,  /*射频B 路*/
    AT_TSELRF_MAX      /*最大值*/
}WLAN_AT_TSELRF_TYPE;
typedef struct _AT_WIMODE_GLOBAL
{
    WLAN_AT_FEATURE_TYPE    cw_enable;  /* cw模式使能 */
    WLAN_AT_WIMODE_TYPE     mode;       /*wifi协议模式*/
}AT_WIMODE_GLOBAL_TYPE;
typedef struct _AT_WIFREQ_GLOBAL
{
    uint32               channel;       /*wifi频点对应信道*/
    WLAN_AT_WIFREQ_STRU  freq;          /*wifi频点信息*/
}AT_WIFREQ_GLOBAL_TYPE;
typedef struct _AT_WICAL_CH_POWER
{
    uint8 ch;
    uint8 data;
}AT_WICAL_CH_POWER_TYPE;
typedef struct _AT_WICAL_POWER
{
    AT_WICAL_CH_POWER_TYPE   cck[3];  /* 802.11b, 产线校准低中高信道 */
    AT_WICAL_CH_POWER_TYPE   ht40[3]; /* 40M, 产线校准低中高信道 */
    uint8   diffht20;                 /* 20M  diff*/
    uint8   diffofdm;                 /* ofdm  diff*/
    uint8   diffht40_2s;              /* 40M  2s  diff */
}AT_WICAL_POWER_TYPE;
typedef struct _AT_WICAL_GLOBAL
{
    WLAN_AT_FEATURE_TYPE    onoff;      /* 校准切换开关 */
    
    AT_WICAL_POWER_TYPE    power[AT_TSELRF_MAX];
    uint8 power_onoff[AT_TSELRF_MAX];   /* 功率efuse 可写标志位 */
    
    uint8   crystal;                    /* 频率补偿 */
    uint8 crystal_onoff;                /* 频率 efuse 可写标志位 */
    
    uint8   thermal;                    /* 温度补偿 */
    uint8 thermal_onoff;                /* 温度efuse 可写标志位 */
}AT_WICAL_GLOBAL_TYPE;
typedef struct tagWlanATGlobal
{
    WLAN_AT_WIENABLE_TYPE   WiEnable;       /*默认加载测试模式*/
    AT_WIMODE_GLOBAL_TYPE   WiMode;
    WLAN_AT_WIBAND_TYPE     WiBand;         /* wifi协议制式*/
    AT_WIFREQ_GLOBAL_TYPE   WiFreq;         /* 频率 */
    uint32                  WiDataRate;     /* wifi发射速率*/
    int32                   WiPow;          /* wifi发射功率*/
    WLAN_AT_FEATURE_TYPE    WiTX;           /* wifi发射机状态*/
    WLAN_AT_WIRX_STRU       WiRX;           /* wifi接收机状态*/
    WLAN_AT_WiPARANGE_TYPE  WiParange;      /*wifi功率增益模式*/

    WLAN_AT_WICALTEMP_STRU  WiCalTemp;      /*wifi温度校准*/
    /* 校准数据信息*/
    AT_WICAL_GLOBAL_TYPE WiCal;
    WLAN_AT_WICALDATA_STRU WiCalData;
    
    WLAN_AT_TSELRF_TYPE  WiTselrf;          /* 天线索引 */
    
    WLAN_AT_WICALFREQ_STRU  WiCalFreq;      /* 校准频率 */
    WLAN_AT_WICALPOW_STRU   WiCalPow;       /* 校准功率 */
    
} WLAN_AT_GLOBAL_ST;

typedef struct _WLAN_AT_WICAL_PWR_PARAM_SET
{
    const char * cck;
    const char * diff_th20;
    const char * diff_th20_ofdm;
    const char * diff_th40;
    const char * th40_1s;
    const char * diff_th40_2s;
}WLAN_AT_WICAL_PWR_PARAM_SET_TYPE;

/*===========================================================================
                       全局信息
===========================================================================*/
/*开启WiFi的默认参数*/
STATIC WLAN_AT_GLOBAL_ST g_wlan_at_data =
{   .WiEnable   = AT_WIENABLE_TEST,
    .WiMode     = {AT_FEATURE_DISABLE, AT_WIMODE_80211n},
    .WiBand     = AT_WIBAND_20M,
    .WiFreq     = {1, {2412, 0}}, /* 初始化频点 */
    .WiDataRate = 5850,            /* 初始化速率 */
    .WiPow      = 3175,            /* 初始化功率 */
    .WiTX       = AT_FEATURE_DISABLE,
    .WiRX       = {AT_FEATURE_DISABLE, {0}, {0}},
    .WiParange  = AT_WiPARANGE_HIGH,
    .WiCalTemp  = {0, 0},
    .WiCal      = {   .onoff = AT_FEATURE_DISABLE            /* 校准切换开关 */
                    , .power_onoff = {AT_FEATURE_DISABLE}    /* 功率efuse 可写标志位 */
                    , .crystal = 0                           /* 频率补偿 */
                    , .crystal_onoff = AT_FEATURE_DISABLE    /* 频率 efuse 可写标志位 */
                    , .thermal = 0                           /* 温度补偿 */
                    , .thermal_onoff = AT_FEATURE_DISABLE    /* 温度efuse 可写标志位 */
                },
    .WiCalData  = {AT_WICALDATA_REL_IDX_POW, 0, AT_WIMODE_80211n, AT_WIFREQ_24G, AT_WIBAND_20M, 1, {0}},
    .WiTselrf   = AT_TSELRF_A,
    .WiCalFreq  = {AT_WICALFREQ_INDEX, 32},  /* 初始化频偏初始值 */
    .WiCalPow   = {AT_WICALPOW_POWER, 3175}, /* 初始化校准功率 */
};

/*信道列表*/
const uint16 g_ausChannels[] = {2412,2417,2422,2427,2432,2437,2442,2447,2452,2457,2462,2467,2472,2484};

const WLAN_AT_WICAL_PWR_PARAM_SET_TYPE g_wical_pwr_param_set[] =
{
    {.cck            = "HW_TX_POWER_CCK_A",
     .diff_th20      = "HW_TX_POWER_DIFF_HT20_A",
     .diff_th20_ofdm = "HW_TX_POWER_DIFF_HT20_OFDM_A",
     .th40_1s        = "HW_TX_POWER_HT40_1S_A",
     .diff_th40_2s   = "HW_TX_POWER_DIFF_HT40_2S_A"
    },
    {.cck            = "HW_TX_POWER_CCK_B",
     .diff_th20      = "HW_TX_POWER_DIFF_HT20_B",
     .diff_th20_ofdm = "HW_TX_POWER_DIFF_HT20_OFDM_B",
     .th40_1s        = "HW_TX_POWER_HT40_1S_B",
     .diff_th40_2s   = "HW_TX_POWER_DIFF_HT40_2S_B"
    },
};

/*===========================================================================
                       函数实现
===========================================================================*/
/*****************************************************************************
 函数名称  : RTL_POW_DIFF
 功能描述  : 读取power值
 输入参数  : 
 输出参数  : NA
 返 回 值  : WLAN_AT_RETURN_TYPE
*****************************************************************************/
inline int32 RTL_POW_DIFF(int wtselrf, uint8 reg_idx)
{
    int pow_diff = ((reg_idx & (0x0F << (wtselrf * 4))) >> (wtselrf * 4)); /* 天线取reg_idx值: A路取低4位，B路取高4位 */
    return (pow_diff > 8 ? (pow_diff - 16) : pow_diff); /* diff取值范围为 -7 ~ 8 */
}

/*****************************************************************************
 函数名称  : WlanReadPow
 功能描述  :读取power值
 输入参数  : 
 输出参数  : NA
 返 回 值  : WLAN_AT_RETURN_TYPE
*****************************************************************************/
STATIC int32 WlanReadPow(uint32 *reg_val, WLAN_AT_WIMODE_TYPE mode
                    , WLAN_AT_WIBAND_TYPE band, uint32 channel, WLAN_AT_TSELRF_TYPE wtselrf)
{
    char buf[WIFI_CMD_MAX_SIZE] = {0};
    char *p_buf = NULL;
    const char *info_key   = "=";
    
    ASSERT_NULL_POINTER(reg_val, AT_RETURN_FAILURE);
    PLAT_WLAN_INFO("Entry, mode = %d, band = %d , channel = %d, wtselrf =%d", mode, band, channel, wtselrf);

    if (!IS_TSELRF_VAILD(wtselrf))
    {
        PLAT_WLAN_INFO("Error  wtselrf =%d",  wtselrf);
        return AT_RETURN_FAILURE;
    }

    switch (mode)
    {
        case AT_WIMODE_80211b:
        {
            SIZEOF_SNPRINTF(buf, WIFI_TOOL_IWPRIV" "WIFI_AP_INTERFACE
                " efuse_get %s >/var/staInfo 2>&1"
                , g_wical_pwr_param_set[(int)wtselrf].cck);
            break;
        }
        case AT_WIMODE_80211g:
        {
            SIZEOF_SNPRINTF(buf, WIFI_TOOL_IWPRIV" "WIFI_AP_INTERFACE
                " efuse_get %s >/var/staInfo 2>&1"
                , g_wical_pwr_param_set[(int)wtselrf].diff_th20_ofdm);
            break;
        }
        case AT_WIMODE_80211n:
        {
            switch (band)
            {
                case AT_WIBAND_20M:
                {
                    SIZEOF_SNPRINTF(buf, WIFI_TOOL_IWPRIV" "WIFI_AP_INTERFACE
                        " efuse_get %s >/var/staInfo 2>&1"
                        , g_wical_pwr_param_set[(int)wtselrf].diff_th20);
                    break;
                }
                case AT_WIBAND_40M:
                {
                    SIZEOF_SNPRINTF(buf, WIFI_TOOL_IWPRIV" "WIFI_AP_INTERFACE
                        " efuse_get %s >/var/staInfo 2>&1"
                        , g_wical_pwr_param_set[(int)wtselrf].th40_1s);
                    break;
                }
                default:
                    PLAT_WLAN_ERR("Error band = %d", band);
                    break;
            }
            break;
        }
        default:
        {
            PLAT_WLAN_ERR("Error mode = %d", mode);
            break;
        }
    }

    WIFI_SHELL_CMD(buf);

    memset(buf, 0, sizeof(buf));
    if(0 == wlan_read_file("/var/staInfo", buf, sizeof(buf)))
    {
        PLAT_WLAN_ERR("read_from_file fail");
        return AT_RETURN_FAILURE;
    }
    
    p_buf = strstr(buf, info_key);
    if (NULL == p_buf)
    {
        return AT_RETURN_FAILURE;
    }
    p_buf += (strlen(info_key) + (channel - 1) * 2);  /* 根据信道查询power值 */
    p_buf[2] = '\0';  /* 添加字符串结束符 */
    PLAT_WLAN_INFO("channel = %d , buf:%s ", channel, p_buf);

    *reg_val = (uint32)wlan_strtoi(p_buf, NULL, WLAN_BASE16); /*获取16进制数*/

    PLAT_WLAN_INFO("Exit, reg_value = %d", (int)*reg_val);
    return AT_RETURN_SUCCESS;
}

/*===========================================================================
 (1)^WIENABLE 设置WiFi模块使能
===========================================================================*/
/*****************************************************************************
 函数名称  : int32 WlanATSetWifiEnable(WLAN_AT_WIENABLE_TYPE onoff)
 功能描述  : 设置WiFi模块使能
 输入参数  : onoff:使能开关
 输出参数  : NA
 返 回 值  : int32
*****************************************************************************/
STATIC int32 ATSetWifiEnable(WLAN_AT_WIENABLE_TYPE onoff)
{
    PLAT_WLAN_INFO("Enter [onoff=%d]", onoff);

    switch (onoff)
    {
    case AT_WIENABLE_OFF:
        WIFI_TEST_CMD(WIFI_TOOL_IWPRIV" "WIFI_AP_INTERFACE" mp_stop");
        WIFI_TEST_CMD(WIFI_TOOL_IFCONFIG" "WIFI_AP_INTERFACE" down");
        break;
    case AT_WIENABLE_TEST:
        WIFI_TEST_CMD(WIFI_TOOL_IFCONFIG" "WIFI_AP_INTERFACE" down");
        //屏蔽掉与8192d相关的命令
        //WIFI_TEST_CMD(WIFI_TOOL_IWPRIV" "WIFI_AP_INTERFACE" set_mib trsw_pape_C9=0");
        //WIFI_TEST_CMD(WIFI_TOOL_IWPRIV" "WIFI_AP_INTERFACE" set_mib trsw_pape_CC=0");
        //WIFI_TEST_CMD(WIFI_TOOL_IWPRIV" "WIFI_AP_INTERFACE" set_mib macPhyMode=0");
        //WIFI_TEST_CMD(WIFI_TOOL_IWPRIV" "WIFI_AP_INTERFACE" set_mib phyBandSelect=1");
        WIFI_TEST_CMD(WIFI_TOOL_IWPRIV" "WIFI_AP_INTERFACE" set_mib mp_specific=1");
        WIFI_TEST_CMD(WIFI_TOOL_IWPRIV" "WIFI_AP_INTERFACE" set_mib channel=1"); /*避免进入自动信道选择*/
        WIFI_TEST_CMD(WIFI_TOOL_IFCONFIG" "WIFI_AP_INTERFACE" up");
        WIFI_TEST_CMD(WIFI_TOOL_IWPRIV" "WIFI_AP_INTERFACE" mp_start");
        break;
    default:
        PLAT_WLAN_INFO("Exit on PARAMS FAILED");
        return (AT_RETURN_FAILURE);
    }

    g_wlan_at_data.WiEnable = onoff;
    return (AT_RETURN_SUCCESS);
}

/*****************************************************************************
 函数名称  : WLAN_AT_WIENABLE_TYPE WlanATGetWifiEnable(void)
 功能描述  : 获取当前的WiFi模块使能状态
 输入参数  : NA
 输出参数  : NA
 返 回 值  : WLAN_AT_WIENABLE_TYPE
*****************************************************************************/
STATIC WLAN_AT_WIENABLE_TYPE ATGetWifiEnable(void)
{
    PLAT_WLAN_INFO("Enter [status=%d]", g_wlan_at_data.WiEnable);
    if (WLAN_SUCCESS != wlan_run_cmd(WIFI_TOOL_IFCONFIG" "WIFI_AP_INTERFACE))   //check  wlan0
    {
        PLAT_WLAN_ERR(" wifi driver status error!!!");
        return AT_WIENABLE_OFF;
    }
    
    return (g_wlan_at_data.WiEnable);
}

/*===========================================================================
 (2)^WIMODE 设置WiFi模式参数 目前均为单模式测试
===========================================================================*/
/*****************************************************************************
 函数名称  : int32 WlanATSetWifiMode(WLAN_AT_WIMODE_TYPE mode)
 功能描述  : 设置WiFi AP支持的连接模式
 输入参数  : mode:连接模式
 输出参数  : NA
 返 回 值  : WLAN_AT_RETURN_TYPE
*****************************************************************************/
STATIC int32 ATSetWifiMode(WLAN_AT_WIMODE_TYPE mode)
{

    ASSERT_WiFi_OFF(AT_RETURN_FAILURE);

    PLAT_WLAN_INFO("Enter [cur_mode=%d set_mode=%d]", g_wlan_at_data.WiMode.mode, mode);

    WLAN_SET_RXTX_STOP();

    switch (mode)
    {
        case AT_WIMODE_CW:
        {
            g_wlan_at_data.WiMode.cw_enable = AT_FEATURE_ENABLE;
            g_wlan_at_data.WiMode.mode = AT_WIMODE_80211b;
            g_wlan_at_data.WiBand = AT_WIBAND_20M;
            WIFI_TEST_CMD(WIFI_TOOL_IWPRIV" "WIFI_AP_INTERFACE" mp_rate 22");
            WIFI_TEST_CMD(WIFI_TOOL_IWPRIV" "WIFI_AP_INTERFACE" mp_bandwidth 40M=0,shortGI=0");
            break;
        }
        case AT_WIMODE_80211b:
        case AT_WIMODE_80211g:
        case AT_WIMODE_80211n:
        {
            g_wlan_at_data.WiMode.cw_enable = AT_FEATURE_DISABLE;
            g_wlan_at_data.WiMode.mode = mode;
            break;
        }
        default:
            PLAT_WLAN_INFO("Error mode = %d", mode);
            return (AT_RETURN_FAILURE);
    }

    return (AT_RETURN_SUCCESS);
}

/*****************************************************************************
 函数名称  : int32 WlanATGetWifiMode(WLAN_AT_BUFFER_STRU *strBuf)
 功能描述  : 获取当前WiFi支持的连接模式
 输入参数  : NA
 输出参数  : strBuf:当前模式，以字符串形式返回eg: 2
 返 回 值  : WLAN_AT_RETURN_TYPE
*****************************************************************************/
STATIC int32 ATGetWifiMode(WLAN_AT_BUFFER_STRU *strBuf)
{
    ASSERT_NULL_POINTER(strBuf, AT_RETURN_FAILURE);
    ASSERT_WiFi_OFF(AT_RETURN_FAILURE);

    PLAT_WLAN_INFO("Enter [cw_enable = %d, cur_mode = %d]"
        , g_wlan_at_data.WiMode.cw_enable, g_wlan_at_data.WiMode.mode);

    SIZEOF_SNPRINTF(strBuf->buf, "%d"
        , ((AT_FEATURE_ENABLE == g_wlan_at_data.WiMode.cw_enable) ? AT_WIMODE_CW : g_wlan_at_data.WiMode.mode));
    PLAT_WLAN_INFO(" return  %s  ", strBuf->buf);
    return (AT_RETURN_SUCCESS);
}

/*****************************************************************************
 函数名称  : int32 WlanATGetWifiModeSupport(WLAN_AT_BUFFER_STRU *strBuf)
 功能描述  : 获取WiFi AP芯片支持的所有连接模式
 输入参数  : NA
 输出参数  : strBuf:支持的所有模式，以字符串形式返回eg: 2,3,4
 返 回 值  : WLAN_AT_RETURN_TYPE
*****************************************************************************/
STATIC int32 ATGetWifiModeSupport(WLAN_AT_BUFFER_STRU *strBuf)
{
    ASSERT_NULL_POINTER(strBuf, AT_RETURN_FAILURE);

    PLAT_WLAN_INFO("Enter [mode=%s]", WLAN_AT_MODE_SUPPORT);

    SIZEOF_SNPRINTF(strBuf->buf, "%s", WLAN_AT_MODE_SUPPORT);
    return (AT_RETURN_SUCCESS);
}

/*===========================================================================
 (3)^WIBAND 设置WiFi带宽参数
===========================================================================*/
/*****************************************************************************
 函数名称  : int32 WlanATSetWifiBand(WLAN_AT_WIBAND_TYPE width)
 功能描述  : 设置WiFi带宽参数
 输入参数  : width:带宽
 输出参数  : NA
 返 回 值  : WLAN_AT_RETURN_TYPE
*****************************************************************************/
STATIC int32 ATSetWifiBand(WLAN_AT_WIBAND_TYPE width)
{
    ASSERT_WiFi_OFF(AT_RETURN_FAILURE);
    PLAT_WLAN_INFO("Enter [band=%d]", width);

    WLAN_SET_RXTX_STOP();

    switch(width)
    {
    case AT_WIBAND_20M:
        WIFI_TEST_CMD(WIFI_TOOL_IWPRIV" "WIFI_AP_INTERFACE" mp_bandwidth 40M=0,shortGI=0");
        break;
    case AT_WIBAND_40M:
        WIFI_TEST_CMD(WIFI_TOOL_IWPRIV" "WIFI_AP_INTERFACE" mp_bandwidth 40M=1,shortGI=0");
        break;
    default:
        PLAT_WLAN_INFO("Exit on PARAMS FAILED");
        return AT_RETURN_FAILURE;
    }

    g_wlan_at_data.WiBand = width;
    return (AT_RETURN_SUCCESS);
}

/*****************************************************************************
 函数名称  : int32 WlanATGetWifiBand(WLAN_AT_BUFFER_STRU *strBuf)
 功能描述  : 获取当前带宽配置
 输入参数  : NA
 输出参数  : strBuf:当前带宽，以字符串形式返回eg: 0
 返 回 值  : WLAN_AT_RETURN_TYPE
*****************************************************************************/
STATIC int32 ATGetWifiBand(WLAN_AT_BUFFER_STRU *strBuf)
{
    ASSERT_NULL_POINTER(strBuf, AT_RETURN_FAILURE);
    ASSERT_WiFi_OFF(AT_RETURN_FAILURE);

    PLAT_WLAN_INFO("Enter [band=%d]", g_wlan_at_data.WiBand);

    SIZEOF_SNPRINTF(strBuf->buf, "%d", g_wlan_at_data.WiBand);
    return (AT_RETURN_SUCCESS);
}

/*****************************************************************************
 函数名称  : int32 WlanATGetWifiBandSupport(WLAN_AT_BUFFER_STRU *strBuf)
 功能描述  : 获取WiFi AP支持的带宽配置
 输入参数  : NA
 输出参数  : strBuf:支持带宽，以字符串形式返回eg: 0,1
 返 回 值  : WLAN_AT_RETURN_TYPE
*****************************************************************************/
STATIC int32 ATGetWifiBandSupport(WLAN_AT_BUFFER_STRU *strBuf)
{
    ASSERT_NULL_POINTER(strBuf, AT_RETURN_FAILURE);

    PLAT_WLAN_INFO("Enter [band=%s]", WLAN_AT_BAND_SUPPORT);

    SIZEOF_SNPRINTF(strBuf->buf, "%s", WLAN_AT_BAND_SUPPORT);
    return (AT_RETURN_SUCCESS);
}

/*===========================================================================
 (4)^WIFREQ 设置WiFi频点
===========================================================================*/
/*****************************************************************************
 函数名称  : int32 WlanATSetWifiFreq(WLAN_AT_WIFREQ_STRU *pFreq)
 功能描述  : 设置WiFi频点
 输入参数  : pFreq:频点信息
 输出参数  : NA
 返 回 值  : WLAN_AT_RETURN_TYPE
*****************************************************************************/
STATIC int32 ATSetWifiFreq(WLAN_AT_WIFREQ_STRU *pFreq)
{
    int8 acCmd[WIFI_CMD_MAX_SIZE] = {0};
    uint32 channel = 0;

    ASSERT_NULL_POINTER(pFreq, AT_RETURN_FAILURE);
    ASSERT_WiFi_OFF(AT_RETURN_FAILURE);

    for (channel = 0; channel < ARRAY_SIZE(g_ausChannels); channel++)
    {
        if (pFreq->value == g_ausChannels[channel])
        {
            break;
        }
    }

    PLAT_WLAN_INFO("Enter [freq=%d idx=%d<%d]", pFreq->value, (int)(channel + 1), ARRAY_SIZE(g_ausChannels));

    if (ARRAY_SIZE(g_ausChannels) <= channel)
    {
        return (AT_RETURN_FAILURE);
    }

    WLAN_SET_RXTX_STOP();

    channel++;
    SIZEOF_SNPRINTF(acCmd, WIFI_TOOL_IWPRIV" "WIFI_AP_INTERFACE" mp_channel %d", (int)channel);
    WIFI_TEST_CMD(acCmd);

    g_wlan_at_data.WiFreq.channel = channel;
    g_wlan_at_data.WiFreq.freq.value = pFreq->value;
    g_wlan_at_data.WiFreq.freq.offset = pFreq->offset;
    return (AT_RETURN_SUCCESS);
}

/*****************************************************************************
 函数名称  : int32 WlanATGetWifiFreq(WLAN_AT_WIFREQ_STRU *pFreq)
 功能描述  : 获取WiFi频点
 输入参数  : NA
 输出参数  : pFreq:当前频点
 返 回 值  : WLAN_AT_RETURN_TYPE
*****************************************************************************/
STATIC int32 ATGetWifiFreq(WLAN_AT_WIFREQ_STRU *pFreq)
{
    ASSERT_NULL_POINTER(pFreq, AT_RETURN_FAILURE);
    ASSERT_WiFi_OFF(AT_RETURN_FAILURE);

    PLAT_WLAN_INFO("Enter [channel = %d, freq = %d, offset = %d]"
        , (int)g_wlan_at_data.WiFreq.channel
        , (int)g_wlan_at_data.WiFreq.freq.value
        , (int)g_wlan_at_data.WiFreq.freq.offset
        );

    memcpy(pFreq, &(g_wlan_at_data.WiFreq.freq), sizeof(*pFreq));
    return (AT_RETURN_SUCCESS);
}

/*===========================================================================
 (5)^WIDATARATE 设置和查询当前WiFi速率集速率
  WiFi速率，单位为0.01Mb/s，取值范围为0～65535
===========================================================================*/
/*****************************************************************************
 函数名称  : int32 WlanATSetWifiDataRate(uint32 rate)
 功能描述  : 设置WiFi速率集速率
 输入参数  : rate:速率值
 输出参数  : NA
 返 回 值  : WLAN_AT_RETURN_TYPE
*****************************************************************************/
STATIC int32 ATSetWifiDataRate(uint32 rate)
{
    int8    acCmd[WIFI_CMD_MAX_SIZE] = {0};
    uint32  ulNRate = 0;
    const uint32  aucAtWifiNRate_Table[] = {650, 1300, 1950, 2600, 3900, 5200, 5850, 6500};          /*20M DataRate*/
    const uint32  aucAtWifiNRate_40M_Table[] = {1350, 2700, 4050, 5400, 8100, 10800, 12150, 13500}; /*40M DataRate*/

    ASSERT_WiFi_OFF(AT_RETURN_FAILURE);
    PLAT_WLAN_INFO("Enter [mode=%d rate=%d]", g_wlan_at_data.WiMode.mode, (int)rate);

    WLAN_SET_RXTX_STOP();

    switch (g_wlan_at_data.WiMode.mode)
    {
    case AT_WIMODE_80211b:
    case AT_WIMODE_80211g:
        SIZEOF_SNPRINTF(acCmd, WIFI_TOOL_IWPRIV" "WIFI_AP_INTERFACE" mp_rate %d", (int)(rate / 50)); /*DataRate*/
        break;
    case AT_WIMODE_80211n:
        for (ulNRate = 0; ulNRate < ARRAY_SIZE(aucAtWifiNRate_Table); ulNRate++)
        {
            if (aucAtWifiNRate_Table[ulNRate] == rate)
            {
                break;
            }
            if (aucAtWifiNRate_40M_Table[ulNRate] == rate)
            {
                break;
            }
        }
        if (ARRAY_SIZE(aucAtWifiNRate_Table) <= ulNRate)
        {
            return (AT_RETURN_FAILURE);
        }
        SIZEOF_SNPRINTF(acCmd, WIFI_TOOL_IWPRIV" "WIFI_AP_INTERFACE" mp_rate %d", (int)(128 + ulNRate));/* 速率 */
        break;
    default:
        return (AT_RETURN_FAILURE);
    }
    WIFI_TEST_CMD(acCmd);

    g_wlan_at_data.WiDataRate = rate;

    return (AT_RETURN_SUCCESS);
}

/*****************************************************************************
 函数名称  : uint32 WlanATGetWifiDataRate(void)
 功能描述  : 查询当前WiFi速率集速率
 输入参数  : NA
 输出参数  : NA
 返 回 值  : uint32:速率值
*****************************************************************************/
STATIC uint32 ATGetWifiDataRate(void)
{
    PLAT_WLAN_INFO("Enter [mode=%d rate=%d]", g_wlan_at_data.WiMode.mode, (int)g_wlan_at_data.WiDataRate);

    return g_wlan_at_data.WiDataRate;
}

/*===========================================================================
 (6)^WIPOW 来设置WiFi发射功率
   WiFi发射功率，单位为0.01dBm，取值范围为 -32768～32767
===========================================================================*/
/*****************************************************************************
 函数名称  : int32 WlanATSetWifiPOW(int32 power_dBm_percent)
 功能描述  : 设置WiFi发射功率
 输入参数  : power_dBm_percent:功率值
 输出参数  : NA
 返 回 值  : WLAN_AT_RETURN_TYPE
*****************************************************************************/
STATIC int32 ATSetWifiPOW(int32 power_dBm_percent)
{
    int8   acCmd[WIFI_CMD_MAX_SIZE] = {0};
    int32  lWifiPower = (power_dBm_percent / 100); /* dBm */
    uint32  pow_index = 0;
    uint32  curr_index = 0;
    uint32  ht40_index  = 0;
    uint32  curr_tselrf = WLAN_CURRENT_TSELRF;
    WLAN_AT_WIINFO_POWER_STRU stGetPwrWiinfo = {0};
    char sPower_24G_bMode[MAX_PWR_SIZE ] = "0";
    char sPower_24G_gMode[MAX_PWR_SIZE ] = "0";
    char sPower_24G_nMode[MAX_PWR_SIZE ] = "0";
    uint32 target_power_11b = 0;
    uint32 target_power_11g = 0;
    uint32 target_power_11n = 0;

    PLAT_WLAN_INFO("Enter [power=%d]", (int)power_dBm_percent);
    ASSERT_WiFi_OFF(AT_RETURN_FAILURE);

    if (lWifiPower < WLAN_AT_WIFI_POWER_MIN)
    {
        lWifiPower = WLAN_AT_WIFI_POWER_MIN;
    }

    if (lWifiPower > WLAN_AT_WIFI_POWER_MAX)
    {
        lWifiPower = WLAN_AT_WIFI_POWER_MAX;
    }

    if(AT_RETURN_SUCCESS != NV_Read(NV_ID_GET_PWRWIINFO, &stGetPwrWiinfo, sizeof(WLAN_AT_WIINFO_POWER_STRU)))
    {
        PLAT_WLAN_ERR("Read target wifi power failed!!");
        return AT_RETURN_FAILURE;
    }

    memcpy(sPower_24G_bMode,stGetPwrWiinfo.bMode_24G_pwr,sizeof(stGetPwrWiinfo.bMode_24G_pwr));
            memcpy(sPower_24G_gMode,stGetPwrWiinfo.gMode_24G_pwr,sizeof(stGetPwrWiinfo.gMode_24G_pwr));
            memcpy(sPower_24G_nMode,stGetPwrWiinfo.nMode_24G_pwr,sizeof(stGetPwrWiinfo.nMode_24G_pwr));
            PLAT_WLAN_INFO("NV_Read PWRWIINFO SUCCESS, sPower_24G_bMode=%s,sPower_24G_gMode=%s,sPower_24G_nMode=%s\n"
                , sPower_24G_bMode
                , sPower_24G_gMode
                , sPower_24G_nMode);

    target_power_11b = wlan_strtoi(sPower_24G_bMode, NULL, WLAN_BASE10);
    target_power_11g = wlan_strtoi(sPower_24G_gMode, NULL, WLAN_BASE10);
    target_power_11n = wlan_strtoi(sPower_24G_nMode, NULL, WLAN_BASE10);

    PLAT_WLAN_INFO("target_power_11b = %d, target_power_11g = %d target_power_11g = %d\n", target_power_11b, target_power_11g
        , target_power_11n);

    //读取40M数据
    if (AT_RETURN_SUCCESS != WlanReadPow(&ht40_index, AT_WIMODE_80211n, AT_WIBAND_40M
                                , g_wlan_at_data.WiFreq.channel, WLAN_CURRENT_TSELRF))
    {
        PLAT_WLAN_INFO("ERROR: N40 WlanReadPow");
        return AT_RETURN_FAILURE;
    }
    //读取实际结果
    PLAT_WLAN_INFO("wifi_mode = %d, wifi_band = %d, wifi_channel = %d , WiTselrf =  %d", g_wlan_at_data.WiMode.mode
        , g_wlan_at_data.WiBand, (int)g_wlan_at_data.WiFreq.channel, WLAN_CURRENT_TSELRF);
    if (AT_RETURN_SUCCESS != WlanReadPow(&curr_index, g_wlan_at_data.WiMode.mode
                                    , g_wlan_at_data.WiBand, g_wlan_at_data.WiFreq.channel, WLAN_CURRENT_TSELRF))
    {
        PLAT_WLAN_ERR("ERROR: get this mode reg value");
        return AT_RETURN_FAILURE;
    }

    PLAT_WLAN_INFO("efuse_get rmap index is 0x%02x,  ht40_index is 0x%02x", (unsigned int)curr_index, (unsigned int)ht40_index);
    switch (g_wlan_at_data.WiMode.mode)
    {
        case AT_WIMODE_80211b:
        {
            if (0 == lWifiPower)
            {
                pow_index = 50; /* 802.11b默认值 */
            }
            else
            {
                pow_index = curr_index + (lWifiPower - (target_power_11b / 10)) * 2; /* 参见MP文档 */
            }
            break;
        }
        case AT_WIMODE_80211g:
        {
            if (0 == lWifiPower)
            {
                pow_index = 41; /* 802.11g默认值 */
            }
            else if (IS_TSELRF_VAILD(curr_tselrf))
            {
                pow_index = ht40_index + RTL_POW_DIFF(curr_tselrf, curr_index) 
                    + ((lWifiPower - (target_power_11g / 10)) * 2); /*MP*/
            }
            else
            {
                PLAT_WLAN_INFO("Error  wtselrf =%d", curr_tselrf);
                return AT_RETURN_FAILURE;
            }
            break;
        }
        case AT_WIMODE_80211n:
        {
            if (0 == lWifiPower)
            {
                pow_index = 40; /* 802.11n默认值 */
            }
            else
            {
                switch (g_wlan_at_data.WiBand)
                {
                    case AT_WIBAND_20M:
                    {
                        if (IS_TSELRF_VAILD(curr_tselrf))
                        {
                            pow_index = ht40_index + RTL_POW_DIFF(curr_tselrf, curr_index) 
                                        + ((lWifiPower - (target_power_11n / 10)) * 2); /*MP*/
                        }
                        else
                        {
                            PLAT_WLAN_INFO("Error  wtselrf =%d", curr_tselrf);
                            return AT_RETURN_FAILURE;
                        }
                        break;
                    }
                    case AT_WIBAND_40M:
                        pow_index = curr_index + (lWifiPower - (target_power_11n / 10)) * 2; /* MP 40M */
                        break;
                    default:
                        PLAT_WLAN_ERR("ERROR: WiBand = %d", g_wlan_at_data.WiBand);
                        return AT_RETURN_FAILURE;
                }
            }
            break;
        }
        default:
            PLAT_WLAN_ERR("ERROR: mode = %d", g_wlan_at_data.WiMode.mode);
            return AT_RETURN_FAILURE;
    }

    WLAN_SET_RXTX_STOP();

    PLAT_WLAN_INFO("power_index = %d, mode = %d, band = %d"
        , (int)pow_index, g_wlan_at_data.WiMode.mode, g_wlan_at_data.WiBand);
    SIZEOF_SNPRINTF(acCmd, WIFI_TOOL_IWPRIV" "WIFI_AP_INTERFACE" mp_txpower patha=%d,pathb=%d", (int)pow_index, (int)pow_index);
    WIFI_TEST_CMD(acCmd);

    g_wlan_at_data.WiPow = power_dBm_percent;
    return (AT_RETURN_SUCCESS);
}

/*****************************************************************************
 函数名称  : int32 WlanATGetWifiPOW(void)
 功能描述  : 获取WiFi当前发射功率
 输入参数  : NA
 输出参数  : NA
 返 回 值  : int32:功率值
*****************************************************************************/
STATIC int32 ATGetWifiPOW(void)
{
    ASSERT_WiFi_OFF(AT_RETURN_FAILURE);
    PLAT_WLAN_INFO("Enter [power=%d]", (int)g_wlan_at_data.WiPow);

    return g_wlan_at_data.WiPow;
}

/*===========================================================================
 (7)^WITX 设置WiFi发射机状态
===========================================================================*/
/*****************************************************************************
 函数名称  : int32 WlanATSetWifiTX(WLAN_AT_FEATURE_TYPE onoff)
 功能描述  : 设置WiFi发射机状态
 输入参数  : NA
 输出参数  : NA
 返 回 值  : WLAN_AT_RETURN_TYPE
*****************************************************************************/
STATIC int32 ATSetWifiTX(WLAN_AT_FEATURE_TYPE onoff)
{
    int tx_flag = (int)onoff;
    
    ASSERT_WiFi_OFF(AT_RETURN_FAILURE);
    PLAT_WLAN_INFO("Enter onoff = %d, tselrf = %d", onoff, WLAN_CURRENT_TSELRF);

    WLAN_SET_RXTX_STOP();

    switch (tx_flag)
    {
    case AT_FEATURE_DISABLE:
        break;
    case AT_FEATURE_ENABLE:
    case WLAN_AT_TX_FULL_DUTY_RATIO:
    {
        switch (WLAN_CURRENT_TSELRF)
        {
        case AT_TSELRF_A:
            WIFI_TEST_CMD(WIFI_TOOL_IWPRIV" "WIFI_AP_INTERFACE" mp_ant_tx a");
            break;
        case AT_TSELRF_B:
            WIFI_TEST_CMD(WIFI_TOOL_IWPRIV" "WIFI_AP_INTERFACE" mp_ant_tx b");
            break;
        default:
            PLAT_WLAN_ERR("error group = %d", WLAN_CURRENT_TSELRF);
            return AT_RETURN_FAILURE;
        }

        msleep(100); /* 等待天线切换 */
        
        if (WLAN_AT_TX_FULL_DUTY_RATIO == tx_flag)
        {
            WIFI_TEST_CMD(WIFI_TOOL_IWPRIV" "WIFI_AP_INTERFACE" mp_ctx background"); /*占空比100%*/
        }
        else if (AT_FEATURE_ENABLE == g_wlan_at_data.WiMode.cw_enable)
        {
            WIFI_TEST_CMD(WIFI_TOOL_IWPRIV" "WIFI_AP_INTERFACE" mp_ctx background,stone");
        }
        else
        {
            WIFI_TEST_CMD(WIFI_TOOL_IWPRIV" "WIFI_AP_INTERFACE" mp_ctx background,pkt");
        }
        onoff = AT_FEATURE_ENABLE;
        
        msleep(100); /* 等待TX准备好 */
        break;
    }
    default:
        return (AT_RETURN_FAILURE);
    }

    g_wlan_at_data.WiTX = onoff;

    return (AT_RETURN_SUCCESS);
}

/*****************************************************************************
 函数名称  : WLAN_AT_FEATURE_TYPE WlanATGetWifiTX(void)
 功能描述  : 获取WiFi发射机状态
 输入参数  : NA
 输出参数  : NA
 返 回 值  : WLAN_AT_FEATURE_TYPE
*****************************************************************************/
STATIC WLAN_AT_FEATURE_TYPE ATGetWifiTX(void)
{
    ASSERT_WiFi_OFF(AT_FEATURE_DISABLE);
    PLAT_WLAN_INFO("Enter [tx_onoff=%d]", g_wlan_at_data.WiTX);

    return g_wlan_at_data.WiTX;
}

/*===========================================================================
 (8)^WIRX 设置WiFi接收机开关
===========================================================================*/
/*****************************************************************************
 函数名称  : int32 WlanATSetWifiRX(WLAN_AT_WIRX_STRU *params)
 功能描述  : 设置WiFi接收机开关
 输入参数  : params:当前需要配置的RX参数
 输出参数  : NA
 返 回 值  : WLAN_AT_RETURN_TYPE
*****************************************************************************/
STATIC int32 ATSetWifiRX(WLAN_AT_WIRX_STRU *params)
{
    int8   acCmd[WIFI_CMD_MAX_SIZE] = {0};
    ASSERT_NULL_POINTER(params, AT_RETURN_FAILURE);
    ASSERT_WiFi_OFF(AT_RETURN_FAILURE);
    PLAT_WLAN_INFO("Enter, onoff = %d, tselrf = %d", params->onoff, WLAN_CURRENT_TSELRF);

    WLAN_SET_RXTX_STOP();

    switch (params->onoff)
    {
    case AT_FEATURE_DISABLE:
        break;
    case AT_FEATURE_ENABLE:
        switch (WLAN_CURRENT_TSELRF)
        {
        case AT_TSELRF_A:
            WIFI_TEST_CMD(WIFI_TOOL_IWPRIV" "WIFI_AP_INTERFACE" mp_ant_rx a");
            break;
        case AT_TSELRF_B:
            WIFI_TEST_CMD(WIFI_TOOL_IWPRIV" "WIFI_AP_INTERFACE" mp_ant_rx b");
            break;
        default:
            PLAT_WLAN_ERR("error tselrf = %d", WLAN_CURRENT_TSELRF);
            return AT_RETURN_FAILURE;
        }
        PLAT_WLAN_INFO("src_mac=%s\n", params->src_mac);  /*MAC*/
        SIZEOF_SNPRINTF(acCmd, WIFI_TOOL_IWPRIV" "WIFI_AP_INTERFACE" mp_arx filter_SA=%s", params->src_mac);  /*MAC*/
        WIFI_TEST_CMD(acCmd);
        WIFI_TEST_CMD(WIFI_TOOL_IWPRIV" "WIFI_AP_INTERFACE" mp_arx start");
        break;
    default:
        return (AT_RETURN_FAILURE);
    }

    memcpy(&g_wlan_at_data.WiRX, params, sizeof(*params));
    return (AT_RETURN_SUCCESS);
}

/*****************************************************************************
 函数名称  : int32 WlanATGetWifiRX(WLAN_AT_WIRX_STRU *params)
 功能描述  : 获取WiFi接口机设置状态
 输入参数  : NA
 输出参数  : params:当前配置的RX参数
 返 回 值  : WLAN_AT_RETURN_TYPE
*****************************************************************************/
STATIC int32 ATGetWifiRX(WLAN_AT_WIRX_STRU *params)
{
    ASSERT_NULL_POINTER(params, AT_RETURN_FAILURE);
    ASSERT_WiFi_OFF(AT_RETURN_FAILURE);

    memcpy(params, &g_wlan_at_data.WiRX, sizeof(*params));

    PLAT_WLAN_INFO("Exit [onoff=%d]", params->onoff);

    return (AT_RETURN_SUCCESS);
}

/*===========================================================================
 (9)^WIRPCKG 查询WiFi接收机误包码，上报接收到的包的数量
===========================================================================*/
/*****************************************************************************
 函数名称  : int32 WlanATSetWifiRPCKG(int32 flag)
 功能描述  : WiFi上报接收到的包的数量清零
 输入参数  : flag:发包统计清零
 输出参数  : NA
 返 回 值  : WLAN_AT_RETURN_TYPE
*****************************************************************************/
STATIC int32 ATSetWifiRPCKG(int32 flag)
{
    ASSERT_WiFi_OFF(AT_RETURN_FAILURE);

    if (0 != flag)
    {
        PLAT_WLAN_INFO("Exit on flag=%d", (int)flag);
        return (AT_RETURN_FAILURE);
    }

    WIFI_TEST_CMD(WIFI_TOOL_IWPRIV" "WIFI_AP_INTERFACE" mp_reset_stats");
    return (AT_RETURN_SUCCESS);
}

/*****************************************************************************
 函数名称  : int32 WlanATGetWifiRPCKG(WLAN_AT_WIRPCKG_STRU *params)
 功能描述  : 查询WiFi接收机误包码，上报接收到的包的数量
 输入参数  : NA
 输出参数  : params:数据包统计信息
 返 回 值  : WLAN_AT_RETURN_TYPE
*****************************************************************************/
STATIC int32 ATGetWifiRPCKG(WLAN_AT_WIRPCKG_STRU *params)
{

    char buf[WIFI_CMD_MAX_SIZE] = {0};
    char *p_buf = NULL;
    const char *info_key = "Rx OK:";
    int staPkgs = 0;

    ASSERT_NULL_POINTER(params, AT_RETURN_FAILURE);
    ASSERT_WiFi_OFF(AT_RETURN_FAILURE);

    if (AT_FEATURE_ENABLE != g_wlan_at_data.WiRX.onoff)
    {
        PLAT_WLAN_ERR("Exit on RX OFF");
        return (AT_RETURN_FAILURE);
    }

    WIFI_SHELL_CMD(WIFI_TOOL_IWPRIV" "WIFI_AP_INTERFACE" mp_query >/var/staInfo 2>&1");
    
    memset(buf, 0, sizeof(buf));
    if (0 == wlan_read_file("/var/staInfo", buf, sizeof(buf)))
    {
        PLAT_WLAN_ERR("Exit on read file failed");
        return (AT_RETURN_FAILURE);
    }
    
    p_buf = strstr(buf, info_key);
    if(NULL == p_buf) 
    {
        return AT_RETURN_FAILURE;
    }
    p_buf += strlen(info_key);
    staPkgs = wlan_strtoi(p_buf, NULL, WLAN_BASE10); /* 转换为十进制数 */

    params->good_result = staPkgs;
    params->bad_result = 0;

    PLAT_WLAN_INFO("Exit [good=%d,bad=%d]", (int)params->good_result, (int)params->bad_result);
    return (AT_RETURN_SUCCESS);
}

/*===========================================================================
 (10)^WIINFO 查询WiFi的相关信息
===========================================================================*/
/* 说明: WlanATGetWifiInfo 已在平台通过读nv实现，wifi不再实现此接口 */

/*===========================================================================
 (11)^WIPLATFORM 查询WiFi方案平台供应商信息
===========================================================================*/
/*****************************************************************************
 函数名称  : WLAN_AT_WIPLATFORM_TYPE WlanATGetWifiPlatform(void)
 功能描述  : 查询WiFi方案平台供应商信息
 输入参数  : NA
 输出参数  : NA
 返 回 值  : WLAN_AT_WIPLATFORM_TYPE
*****************************************************************************/
STATIC WLAN_AT_WIPLATFORM_TYPE ATGetWifiPlatform(void)
{
    PLAT_WLAN_INFO("Enter [plat=%d]", AT_WIPLATFORM_REALTEK);
    return (AT_WIPLATFORM_REALTEK);
}

/*===========================================================================
 (12)^TSELRF 查询设置单板的WiFi射频通路
===========================================================================*/
/*****************************************************************************
 函数名称  : int32 WlanATSetTSELRF(uint32 group)
 功能描述  : 设置天线，非多通路传0
 输入参数  : group:天线索引
 输出参数  : NA
 返 回 值  : int32
*****************************************************************************/
STATIC int32 ATSetTSELRF(uint32 group)
{
    PLAT_WLAN_INFO("Enter [plat=%d]", (int)group);

    if (!IS_TSELRF_VAILD(group))
    {
        return (AT_RETURN_FAILURE);
    }

    g_wlan_at_data.WiTselrf = (WLAN_AT_TSELRF_TYPE)group;
    return (AT_RETURN_SUCCESS);
}

/*****************************************************************************
 函数名称  : int32 ATGetTSELRFSupport(WLAN_AT_BUFFER_STRU *strBuf)
 功能描述  : 支持的天线索引序列
 输入参数  : NA
 输出参数  : strBuf:天线索引，以字符串形式返回eg: 0,1,2,3
 返 回 值  : WLAN_AT_RETURN_TYPE
*****************************************************************************/
STATIC int32 ATGetTSELRFSupport(WLAN_AT_BUFFER_STRU *strBuf)
{
    ASSERT_NULL_POINTER(strBuf, AT_RETURN_FAILURE);
    PLAT_WLAN_INFO("Enter [TSELRF=%s]", WLAN_AT_TSELRF_SUPPORT);

    SIZEOF_SNPRINTF(strBuf->buf, "%s", WLAN_AT_TSELRF_SUPPORT);
    return (AT_RETURN_SUCCESS);
}

/*===========================================================================
 (13)^WiPARANGE设置、读取WiFi PA的增益情况
===========================================================================*/
/*****************************************************************************
 函数名称  : int32 WlanATSetWifiParange(WLAN_AT_WiPARANGE_TYPE pa_type)
 功能描述  : 设置WiFi PA的增益情况
 输入参数  : pa_type:增益值类型
 输出参数  : NA
 返 回 值  : WLAN_AT_RETURN_TYPE
*****************************************************************************/
STATIC int32 ATSetWifiParange(WLAN_AT_WiPARANGE_TYPE pa_type)
{
    ASSERT_WiFi_OFF(AT_RETURN_FAILURE);
    PLAT_WLAN_INFO("Enter [pa_type=%c]", pa_type);

    if (AT_WiPARANGE_HIGH != pa_type) /*仅支持内置PA*/
    {
        return AT_RETURN_FAILURE;
    }

    g_wlan_at_data.WiParange = pa_type;

    return (AT_RETURN_SUCCESS);
}

/*****************************************************************************
 函数名称  : WLAN_AT_WiPARANGE_TYPE WlanATGetWifiParange(void)
 功能描述  : 读取WiFi PA的增益情况
 输入参数  : NA
 输出参数  : NA
 返 回 值  : WLAN_AT_WiPARANGE_TYPE
*****************************************************************************/
STATIC WLAN_AT_WiPARANGE_TYPE ATGetWifiParange(void)
{
    ASSERT_WiFi_OFF(AT_WiPARANGE_BUTT);

    PLAT_WLAN_INFO("Exit [pa_mode=%c]", g_wlan_at_data.WiParange);

    return g_wlan_at_data.WiParange;
}

/*****************************************************************************
 函数名称  : int32 WlanATGetWifiParangeSupport(WLAN_AT_BUFFER_STRU *strBuf)
 功能描述  : 支持的pa模式序列
 输入参数  : NA
 输出参数  : strBuf:支持的增益序列，以字符串形式返回eg: l,h
 返 回 值  : WLAN_AT_RETURN_TYPE
*****************************************************************************/
STATIC int32 ATGetWifiParangeSupport(WLAN_AT_BUFFER_STRU *strBuf)
{
    ASSERT_NULL_POINTER(strBuf, AT_RETURN_FAILURE);

    SIZEOF_SNPRINTF(strBuf->buf, "%c", AT_WiPARANGE_HIGH);

    PLAT_WLAN_INFO("Exit [support=%s]", strBuf->buf);

    return (AT_RETURN_SUCCESS);
}

/*===========================================================================
 (14)^WICALTEMP设置、读取WiFi的温度补偿值
===========================================================================*/
/*****************************************************************************
 函数名称  : int32 WlanATGetWifiCalTemp(WLAN_AT_WICALTEMP_STRU *params)
 功能描述  : 设置WiFi的温度补偿值
 输入参数  : NA
 输出参数  : params:温度补偿参数
 返 回 值  : WLAN_AT_RETURN_TYPE
*****************************************************************************/
STATIC int32 ATGetWifiCalTemp(WLAN_AT_WICALTEMP_STRU *params)
{
    char buf[WIFI_CMD_MAX_SIZE] = {0};
    char *p_buf = NULL;
    const char *info_key = "mp_ther:";
    
    ASSERT_NULL_POINTER(params, AT_RETURN_FAILURE);

    WIFI_SHELL_CMD(WIFI_TOOL_IWPRIV" "WIFI_AP_INTERFACE" mp_ther >/var/staInfo 2>&1");

    memset(buf, 0, sizeof(buf));
    if(0 == wlan_read_file("/var/staInfo", buf, sizeof(buf)))
    {
        PLAT_WLAN_ERR("read_from_file fail");
        return AT_RETURN_FAILURE;
    }
    PLAT_WLAN_INFO("get_mib ther: %s", buf);
    p_buf = strstr(buf, info_key);
    ASSERT_NULL_POINTER(p_buf, AT_RETURN_FAILURE);

    p_buf += strlen(info_key);
    params->value = wlan_strtoi(p_buf, NULL, WLAN_BASE10);
    PLAT_WLAN_INFO("index: %d, staTemp: %d", (int)params->index, (int)params->value);

    return AT_RETURN_SUCCESS;
}

/*****************************************************************************
 函数名称  : int32 WlanATGetWifiCalTemp(WLAN_AT_WICALTEMP_STRU *params)
 功能描述  : 设置WiFi的温度补偿值
 输入参数  : params:温度补偿参数
 输出参数  : NA
 返 回 值  : WLAN_AT_RETURN_TYPE
*****************************************************************************/
STATIC int32 ATSetWifiCalTemp(WLAN_AT_WICALTEMP_STRU *params)
{
    char acCmd[WIFI_CMD_MAX_SIZE] = {0};
    char buf[WIFI_CMD_MAX_SIZE] = {0};
    char *p_buf = NULL;
    const char *info_key = "get_mib:";
    int32 tmp_data = 0;
    ASSERT_NULL_POINTER(params, AT_RETURN_FAILURE);
    PLAT_WLAN_INFO("index = %d, staTemp = %d", (int)params->index, (int)params->value);

    WLAN_SET_RXTX_STOP();

    if (255 == params->index)/* 最大索引值，表示读取寄存器中的配置信息下发 */
    {
        WIFI_SHELL_CMD(WIFI_TOOL_IWPRIV" "WIFI_AP_INTERFACE" get_mib ther >/var/staInfo 2>&1");
    
        memset(buf, 0, sizeof(buf));
        if(0 == wlan_read_file("/var/staInfo", buf, sizeof(buf)))
        {
            PLAT_WLAN_ERR("read_from_file fail");
            return AT_RETURN_FAILURE;
        }
        PLAT_WLAN_INFO("get_mib ther: %s", buf);
        p_buf = strstr(buf, info_key);
        ASSERT_NULL_POINTER(p_buf, AT_RETURN_FAILURE);
    
        p_buf += strlen(info_key);
        tmp_data = wlan_strtoi(p_buf, NULL, WLAN_BASE10);
        SIZEOF_SNPRINTF(acCmd, WIFI_TOOL_IWPRIV" "WIFI_AP_INTERFACE" mp_pwrtrk ther=%d", tmp_data);
        WIFI_TEST_CMD(acCmd);
    }
    else
    {
        SIZEOF_SNPRINTF(acCmd, WIFI_TOOL_IWPRIV" "WIFI_AP_INTERFACE" mp_pwrtrk ther=%d", (int)params->value);
        WIFI_TEST_CMD(acCmd);
    }
    return AT_RETURN_SUCCESS;
}

/*===========================================================================
 (15)^WICALDATA设置、读取指定类型的WiFi补偿数据
===========================================================================*/
/*****************************************************************************
 函数名称  : int32 WlanATGetWifiCalData(WLAN_AT_WICALDATA_STRU * params)
 功能描述  : 指定类型的WiFi补偿数据
 输入参数  : NA
 输出参数  : params:补偿数据
 返 回 值  : WLAN_AT_RETURN_TYPE
*****************************************************************************/
STATIC int32 ATGetWifiCalData(WLAN_AT_WICALDATA_STRU * params)
{
    ASSERT_NULL_POINTER(params, AT_RETURN_FAILURE);
    memcpy(params, &g_wlan_at_data.WiCalData, sizeof(g_wlan_at_data.WiCalData));
    
    return  AT_RETURN_SUCCESS;
}

/*****************************************************************************
 函数名称  : int32 WlanATSetWifiCalData(WLAN_AT_WICALDATA_STRU *params)
 功能描述  : 指定类型的WiFi补偿数据
 输入参数  : params:补偿数据
 输出参数  : NA
 返 回 值  : WLAN_AT_RETURN_TYPE
*****************************************************************************/
STATIC int32 ATSetWifiCalData(WLAN_AT_WICALDATA_STRU *params)
{
    uint32 channel = 0, idx = 0;
    int32 data = 0;

    ASSERT_NULL_POINTER(params, AT_RETURN_FAILURE);
    
    PLAT_WLAN_INFO("Entry type = %d, group = %d, mode = %d, band = %d, bandwidth = %d, freq = %d, data = %s"
        , params->type, (int)params->group, params->mode, params->band, params->bandwidth, (int)params->freq, params->data);
    
    /* 频点类型 */
    if (AT_WIFREQ_24G != params->band)
    {
        PLAT_WLAN_ERR("Error, band = %d", params->band);
        return AT_RETURN_FAILURE;
    }
    
    /* 天线检查 */
    if (!IS_TSELRF_VAILD(params->group))
    {
        PLAT_WLAN_INFO("Error  wtselrf =%d",  params->group);
        return AT_RETURN_FAILURE;
    }

    /* 信道检查 */
    for (channel = 0; channel < ARRAY_SIZE(g_ausChannels); channel++)
    {
        if (params->freq == g_ausChannels[channel])
        {
            break;
        }
    }
    if (channel == ARRAY_SIZE(g_ausChannels))
    {
        PLAT_WLAN_ERR("Error, freq = %d", (int)params->freq);
        return AT_RETURN_FAILURE;
    }
    channel++;
    data = wlan_strtoi(params->data, NULL, WLAN_BASE10);
    PLAT_WLAN_INFO("channel = %d, data = %d", (int)channel, (int)data);

    switch(params->type) 
    {
    case AT_WICALDATA_REL_IDX_POW:
    case AT_WICALDATA_ABS_IDX_POW:
    {
        switch (params->mode)
        {
            case AT_WIMODE_80211b:
            {
                for (idx = 0; idx < ARRAY_SIZE(g_wlan_at_data.WiCal.power[params->group].cck); idx++)
                {
                    if (0 == g_wlan_at_data.WiCal.power[params->group].cck[idx].ch
                        || channel == g_wlan_at_data.WiCal.power[params->group].cck[idx].ch
                        )
                    {
                        g_wlan_at_data.WiCal.power[params->group].cck[idx].ch = channel;
                        g_wlan_at_data.WiCal.power[params->group].cck[idx].data = (uint8)data;
                        break;
                    }
                }
                if (idx == ARRAY_SIZE(g_wlan_at_data.WiCal.power[params->group].cck))
                {
                    PLAT_WLAN_ERR("Error, channel = %d out of MAX ARRAY %d"
                        , channel, ARRAY_SIZE(g_wlan_at_data.WiCal.power[params->group].cck));
                    return AT_RETURN_FAILURE;
                }
                break;
            }
            case AT_WIMODE_80211g:
            {
                if (WIFI_POWER_DIFF_MIN > data || data > WIFI_POWER_DIFF_MAX)
                {
                    PLAT_WLAN_ERR("Error, data = %d", (int)data);
                    return AT_RETURN_FAILURE;
                }
                g_wlan_at_data.WiCal.power[params->group].diffofdm = (uint8)data; /* DM */
                break;
            }
            case AT_WIMODE_80211n:
            {
                switch(params->bandwidth)
                {
                    case AT_WIBAND_20M:
                    {
                        if (WIFI_POWER_DIFF_MIN > data || data > WIFI_POWER_DIFF_MAX)
                        {
                            PLAT_WLAN_ERR("Error, data = %d", (int)data);
                            return AT_RETURN_FAILURE;
                        }
                        g_wlan_at_data.WiCal.power[params->group].diffht20 = (uint8)data; /* DM */
                        break;
                    }
                    case AT_WIBAND_40M:
                    {
                        for (idx = 0; idx < ARRAY_SIZE(g_wlan_at_data.WiCal.power[params->group].ht40); idx++)
                        {
                            if (0 == g_wlan_at_data.WiCal.power[params->group].ht40[idx].ch
                                || channel == g_wlan_at_data.WiCal.power[params->group].ht40[idx].ch
                                )
                            {
                                g_wlan_at_data.WiCal.power[params->group].ht40[idx].ch = channel;
                                g_wlan_at_data.WiCal.power[params->group].ht40[idx].data = (uint8)data;
                                break;
                            }
                        }
                        if (idx == ARRAY_SIZE(g_wlan_at_data.WiCal.power[params->group].ht40))
                        {
                            PLAT_WLAN_ERR("Error, channel = %d out of MAX ARRAY %d"
                                , channel, ARRAY_SIZE(g_wlan_at_data.WiCal.power[params->group].ht40));
                            return AT_RETURN_FAILURE;
                        }
                        break;
                    }
                    default:
                        PLAT_WLAN_ERR("Error, bandwidth = %d", params->bandwidth);
                        return AT_RETURN_FAILURE;
                }
                break;
            }
            default:
                PLAT_WLAN_ERR("Error, mode = %d", params->mode);
                return AT_RETURN_FAILURE;
        }
        g_wlan_at_data.WiCal.power_onoff[params->group] = AT_FEATURE_ENABLE;
        break;
    }
    case AT_WICALDATA_ABS_IDX_FREQ:
        g_wlan_at_data.WiCal.crystal = (uint8)data;
        g_wlan_at_data.WiCal.crystal_onoff = AT_FEATURE_ENABLE;
        PLAT_WLAN_INFO("crystal_data = %d", (int)g_wlan_at_data.WiCal.crystal);
        break;
    case AT_WICALDATA_ABS_IDX_TEMP:
        g_wlan_at_data.WiCal.thermal = (uint8)data;
        g_wlan_at_data.WiCal.thermal_onoff = AT_FEATURE_ENABLE;
        PLAT_WLAN_INFO("thermal_data = %d", (int)g_wlan_at_data.WiCal.thermal);
        break;
    default:
        PLAT_WLAN_ERR("error type = %d", params->type);
        return AT_RETURN_FAILURE;
    }

    memcpy(&g_wlan_at_data.WiCalData, params, sizeof(g_wlan_at_data.WiCalData));
    
    return AT_RETURN_SUCCESS;
}

/*===========================================================================
 (16)^WICAL设置、读取校准的启动状态，是否支持补偿
===========================================================================*/
/*****************************************************************************
 函数名称  : wlan_cal_power_sprintf
 功能描述  : 将校准数据按照规则生成14个信道数据
 输入参数  : buf:缓存；size: 缓存大小；p_ch_pow: 校准数据；ch_size:校准数据大小
 输出参数  : NA
 返 回 值  : NA
*****************************************************************************/
STATIC int32 wlan_cal_power_sprintf(char *buf, int buf_size, AT_WICAL_CH_POWER_TYPE *p_ch_pow, int ch_size)
{
    int32   len = 0;
    uint8   i = 0, j = 0, ch_idx = 0, ch = 0, data = 0;
    if ((NULL == buf) || (buf_size <= 0) || (NULL == p_ch_pow) || (ch_size <= 0))
    {
        PLAT_WLAN_ERR("buf = %p, buf_size = %d, p_ch_pow = %p, ch_size = %d", buf, buf_size, p_ch_pow, ch_size);
        return len;
    }
    
    /* 校准数据按照信道排序 */
    for (i = 0; i < ch_size - 1; i++)
    {
        ch_idx = i;
        for (j = i + 1; j < ch_size; j++)
        {
            if (p_ch_pow[j].ch < p_ch_pow[ch_idx].ch)
            {
                ch_idx = j;
            }
        }
        if (ch_idx != i)
        {
            ch = p_ch_pow[i].ch;
            data = p_ch_pow[i].data;
            p_ch_pow[i].ch = p_ch_pow[ch_idx].ch;
            p_ch_pow[i].data = p_ch_pow[ch_idx].data;
            p_ch_pow[ch_idx].ch = ch;
            p_ch_pow[ch_idx].data = data;
        }
    }
    i = 0, ch_idx = 0, ch = 0, len = 0;
    do
    {
        /* 获取当前的channel段及对应的校准data */
        while(i < ch_size)
        {
            if (ch < p_ch_pow[i].ch)
            {
                if (i + 1 >= ch_size)
                {
                    ch = WLAN_RTL_CHANNEL_24G_MAX;
                }
                else
                {
                    ch = ((p_ch_pow[i].ch + p_ch_pow[i + 1].ch) / 2);
                }
                data = p_ch_pow[i].data;
                break;
            }
            i++;
        }
        PLAT_WLAN_INFO("channel %d ~ %d power = %d", (ch_idx + 1), ch, data);
        /* 生成校准数据 */
        while (ch_idx < ch)
        {
            ch_idx++;
            len += snprintf(&buf[len], (buf_size - len), "%02x", data);
        }
        /* 写入下一段 */
        i++;
    }while(ch < WLAN_RTL_CHANNEL_24G_MAX);
    
    return len;
}

/*****************************************************************************
 函数名称  : ATSetWifiCal
 功能描述  : 设置校准的启动状态
 输入参数  : onoff:0,结束校准；1,启动校准
 输出参数  : NA
 返 回 值  : WLAN_AT_FEATURE_TYPE
*****************************************************************************/
STATIC int32 ATSetWifiCal(WLAN_AT_FEATURE_TYPE onoff)
{
    int     len = 0;
    uint8   idx = 0, ch_idx = 0;
    int8    acCmd[WIFI_CMD_MAX_SIZE] = {0};
    
    /* 非结束校准状态，直接返回 */
    PLAT_WLAN_INFO("onoff = %d, crystal_onoff = %d, thermal_onoff = %d"
                    , onoff, g_wlan_at_data.WiCal.crystal_onoff, g_wlan_at_data.WiCal.thermal_onoff);
    if (AT_FEATURE_DISABLE != onoff)
    {
        g_wlan_at_data.WiCal.crystal = 0;
        g_wlan_at_data.WiCal.thermal = 0;
        memset(g_wlan_at_data.WiCal.power, 0x0, sizeof(g_wlan_at_data.WiCal.power));
        g_wlan_at_data.WiCal.onoff = onoff;
        return AT_RETURN_SUCCESS;
    }

    /* freq cal */
    if (AT_FEATURE_ENABLE == g_wlan_at_data.WiCal.crystal_onoff)
    {
        SIZEOF_SNPRINTF(acCmd, WIFI_TOOL_IWPRIV" "WIFI_AP_INTERFACE
            " efuse_set HW_11N_XCAP=%02x"
            , g_wlan_at_data.WiCal.crystal);
        WIFI_TEST_CMD(acCmd);
        g_wlan_at_data.WiCal.crystal_onoff = AT_FEATURE_DISABLE;
    }

    /* temp cal */
    if (AT_FEATURE_ENABLE == g_wlan_at_data.WiCal.thermal_onoff)
    {
        memset(acCmd,0,WIFI_CMD_MAX_SIZE);
        SIZEOF_SNPRINTF(acCmd, WIFI_TOOL_IWPRIV" "WIFI_AP_INTERFACE
            " efuse_set HW_11N_THER=%02x", 
            g_wlan_at_data.WiCal.thermal);
        WIFI_TEST_CMD(acCmd);
        g_wlan_at_data.WiCal.thermal_onoff = AT_FEATURE_DISABLE;
    }

    /* power cal */
    for (idx = AT_TSELRF_A; IS_TSELRF_VAILD(idx); idx++)
    {
        PLAT_WLAN_INFO("idx = %d, power_onoff = %d", idx, g_wlan_at_data.WiCal.power_onoff[idx]);
        if (AT_FEATURE_ENABLE != g_wlan_at_data.WiCal.power_onoff[idx])
        {
            continue;
        }
        
        /* 11b信道校准 */
        memset(acCmd,0, sizeof(acCmd));
        len = snprintf(acCmd, sizeof(acCmd), WIFI_TOOL_IWPRIV" "WIFI_AP_INTERFACE
            " efuse_set %s=" , g_wical_pwr_param_set[idx].cck);
        len += wlan_cal_power_sprintf(&acCmd[len], (sizeof(acCmd) - len)
            , g_wlan_at_data.WiCal.power[idx].cck, ARRAY_SIZE(g_wlan_at_data.WiCal.power[idx].cck));
        WIFI_TEST_CMD(acCmd);

        /* 40M带宽校准 */
        memset(acCmd,0, sizeof(acCmd));
        len = snprintf(acCmd, sizeof(acCmd), WIFI_TOOL_IWPRIV" "WIFI_AP_INTERFACE
            " efuse_set %s=" , g_wical_pwr_param_set[idx].th40_1s);
        len += wlan_cal_power_sprintf(&acCmd[len], (sizeof(acCmd) - len)
            , g_wlan_at_data.WiCal.power[idx].ht40, ARRAY_SIZE(g_wlan_at_data.WiCal.power[idx].ht40));
        WIFI_TEST_CMD(acCmd);

        /* 20M带宽校准 */
        memset(acCmd,0,WIFI_CMD_MAX_SIZE);
        len = snprintf(acCmd, sizeof(acCmd), WIFI_TOOL_IWPRIV" "WIFI_AP_INTERFACE
            " efuse_set %s=" , g_wical_pwr_param_set[idx].diff_th20);
        for (ch_idx = 0; ch_idx < WLAN_RTL_CHANNEL_24G_MAX; ch_idx++)
        {
            len += snprintf(&acCmd[len], (sizeof(acCmd) - len), "%02x", g_wlan_at_data.WiCal.power[idx].diffht20);
        }
        WIFI_TEST_CMD(acCmd);

        /* 11g信道校准 */
        memset(acCmd,0,WIFI_CMD_MAX_SIZE);
        len = snprintf(acCmd, sizeof(acCmd), WIFI_TOOL_IWPRIV" "WIFI_AP_INTERFACE
            " efuse_set %s=" , g_wical_pwr_param_set[idx].diff_th20_ofdm);
        for (ch_idx = 0; ch_idx < WLAN_RTL_CHANNEL_24G_MAX; ch_idx++)
        {
            len += snprintf(&acCmd[len], (sizeof(acCmd) - len), "%02x", g_wlan_at_data.WiCal.power[idx].diffofdm);
        }
        WIFI_TEST_CMD(acCmd);

        /* 双发校准 */
        memset(acCmd,0,WIFI_CMD_MAX_SIZE);
        SIZEOF_SNPRINTF(acCmd, WIFI_TOOL_IWPRIV" "WIFI_AP_INTERFACE
            " efuse_set %s=0000000000000000000000000000", g_wical_pwr_param_set[idx].diff_th40_2s);
        WIFI_TEST_CMD(acCmd);
        
        g_wlan_at_data.WiCal.power_onoff[idx] = AT_FEATURE_DISABLE;
    }

    memset(acCmd,0,WIFI_CMD_MAX_SIZE);
    SIZEOF_SNPRINTF(acCmd, WIFI_TOOL_IWPRIV" "WIFI_AP_INTERFACE" efuse_sync");
    WIFI_TEST_CMD(acCmd);

    g_wlan_at_data.WiCal.onoff = onoff;
    return AT_RETURN_SUCCESS;
}

/*****************************************************************************
 函数名称  : WLAN_AT_FEATURE_TYPE WlanATGetWifiCal(void)
 功能描述  : 读取校准的启动状态
 输入参数  : NA
 输出参数  : NA
 返 回 值  : WLAN_AT_FEATURE_TYPE
*****************************************************************************/
STATIC WLAN_AT_FEATURE_TYPE ATGetWifiCal(void)
{
    PLAT_WLAN_INFO("[%s in ]cal_switch = %d", __FUNCTION__, g_wlan_at_data.WiCal.onoff);

    return  g_wlan_at_data.WiCal.onoff;
}

/*****************************************************************************
 函数名称  : WLAN_AT_FEATURE_TYPE WlanATGetWifiCalSupport(void)
 功能描述  : 是否支持校准
 输入参数  : NA
 输出参数  : NA
 返 回 值  : WLAN_AT_FEATURE_TYPE
*****************************************************************************/
STATIC WLAN_AT_FEATURE_TYPE ATGetWifiCalSupport(void)
{
    return AT_FEATURE_ENABLE;
}

/*===========================================================================
 (17)^WICALFREQ 设置、查询频率补偿值
===========================================================================*/
/*****************************************************************************
 函数名称  : int32 WlanATSetWifiCalFreq(WLAN_AT_WICALFREQ_STRU *params)
 功能描述  : 设置频率补偿
 输入参数  : params:补偿参数
 输出参数  : NA
 返 回 值  : WLAN_AT_FEATURE_TYPE
*****************************************************************************/
STATIC int32 ATSetWifiCalFreq(WLAN_AT_WICALFREQ_STRU *params)
{
    int8    acCmd[WIFI_CMD_MAX_SIZE] = {0};
    
    ASSERT_NULL_POINTER(params, AT_RETURN_FAILURE);
    PLAT_WLAN_INFO("type = %d, value = %d", params->type, (int)params->value);

    WLAN_SET_RXTX_STOP();

    SIZEOF_SNPRINTF(acCmd, WIFI_TOOL_IWPRIV" "WIFI_AP_INTERFACE" mp_phypara xcap=%d", (int)params->value);
    WIFI_TEST_CMD(acCmd);

    memcpy(&g_wlan_at_data.WiCalFreq, params, sizeof(g_wlan_at_data.WiCalFreq));
    return AT_RETURN_SUCCESS;
}

/*****************************************************************************
 函数名称  : int32 WlanATGetWifiCalFreq(WLAN_AT_WICALFREQ_STRU *params)
 功能描述  : 设置频率补偿
 输入参数  : NA
 输出参数  : params:补偿参数
 返 回 值  : WLAN_AT_FEATURE_TYPE
*****************************************************************************/
STATIC int32 ATGetWifiCalFreq(WLAN_AT_WICALFREQ_STRU *params)
{
    ASSERT_NULL_POINTER(params, AT_RETURN_FAILURE);
    PLAT_WLAN_INFO("type = %d, value = %d ", g_wlan_at_data.WiCalFreq.type, (int)g_wlan_at_data.WiCalFreq.value);
    memcpy(params, &g_wlan_at_data.WiCalFreq, sizeof(g_wlan_at_data.WiCalFreq));
    return AT_RETURN_SUCCESS;
}

/*===========================================================================
 (18)^WICALPOW 设置、查询功率补偿值
===========================================================================*/
/*****************************************************************************
 函数名称  : int32 WlanATGetWifiCalFreq(WLAN_AT_WICALFREQ_STRU *params)
 功能描述  : 设置功率补偿
 输入参数  : NA
 输出参数  : params:补偿参数
 返 回 值  : WLAN_AT_FEATURE_TYPE
*****************************************************************************/
STATIC int32 ATSetWifiCalPOW(WLAN_AT_WICALPOW_STRU *params)
{
    int8    acCmd[WIFI_CMD_MAX_SIZE] = {0};
    
    ASSERT_NULL_POINTER(params, AT_RETURN_FAILURE);
    PLAT_WLAN_INFO("type = %d, value = %d, tselrf = %d", params->type, (int)params->value, WLAN_CURRENT_TSELRF);

    WLAN_SET_RXTX_STOP();

    SIZEOF_SNPRINTF(acCmd, WIFI_TOOL_IWPRIV" "WIFI_AP_INTERFACE" mp_txpower patha=%d,pathb=%d"
                    , (int)params->value, (int)params->value);
    WIFI_TEST_CMD(acCmd);
    memcpy(&g_wlan_at_data.WiCalPow, params, sizeof(g_wlan_at_data.WiCalPow));
    return AT_RETURN_SUCCESS;
}

/*****************************************************************************
 函数名称  : int32 WlanATGetWifiCalPOW(WLAN_AT_WICALPOW_STRU *params)
 功能描述  : 校准发射功率时，查询对应值
 输入参数  : NA
 输出参数  : NA
 返 回 值  : int32
*****************************************************************************/
STATIC int32 ATGetWifiCalPOW(WLAN_AT_WICALPOW_STRU *params)
{
    ASSERT_NULL_POINTER(params, AT_RETURN_FAILURE);
    PLAT_WLAN_INFO("Entry type = %d, value = %d", g_wlan_at_data.WiCalPow.type, (int)g_wlan_at_data.WiCalPow.value);
    params->type = g_wlan_at_data.WiCalPow.type;
    switch(g_wlan_at_data.WiMode.mode)
    {
    case AT_WIMODE_80211n:
        params->value = 32; /* 802.11n */
        break;
    case AT_WIMODE_80211g:
        params->value = 35; /* 802.11g */
        break;
    case AT_WIMODE_80211b:
        params->value = 31; /* 802.11b */
        break;
    default:
        PLAT_WLAN_ERR("error mode = %d", g_wlan_at_data.WiMode.mode);
        return AT_RETURN_FAILURE;
    }
    PLAT_WLAN_INFO("Exit type = %d, value = %d", params->type, (int)params->value);
    return AT_RETURN_SUCCESS;
}



STATIC WLAN_CHIP_OPS rtl8192_ops = 
{
    .WlanATSetWifiEnable = ATSetWifiEnable,
    .WlanATGetWifiEnable = ATGetWifiEnable,
    
    .WlanATSetWifiMode   = ATSetWifiMode,
    .WlanATGetWifiMode   = ATGetWifiMode,
    .WlanATGetWifiModeSupport = ATGetWifiModeSupport, 

    .WlanATSetWifiBand = ATSetWifiBand,
    .WlanATGetWifiBand = ATGetWifiBand,
    .WlanATGetWifiBandSupport = ATGetWifiBandSupport,

    .WlanATSetWifiFreq = ATSetWifiFreq,
    .WlanATGetWifiFreq = ATGetWifiFreq,

    .WlanATSetWifiDataRate = ATSetWifiDataRate,
    .WlanATGetWifiDataRate = ATGetWifiDataRate,

    .WlanATSetWifiPOW = ATSetWifiPOW,
    .WlanATGetWifiPOW = ATGetWifiPOW,

    .WlanATSetWifiTX = ATSetWifiTX,
    .WlanATGetWifiTX = ATGetWifiTX,

    .WlanATSetWifiRX = ATSetWifiRX,
    .WlanATGetWifiRX = ATGetWifiRX,

    .WlanATSetWifiRPCKG = ATSetWifiRPCKG,
    .WlanATGetWifiRPCKG = ATGetWifiRPCKG,
    
    .WlanATGetWifiPlatform = ATGetWifiPlatform,
    
    .WlanATGetTSELRF = NULL,
    .WlanATSetTSELRF = ATSetTSELRF,
    .WlanATGetTSELRFSupport = ATGetTSELRFSupport, 

    .WlanATSetWifiParange = ATSetWifiParange,
    .WlanATGetWifiParange = ATGetWifiParange,

    .WlanATGetWifiParangeSupport = ATGetWifiParangeSupport,
    
    .WlanATGetWifiCalTemp = ATGetWifiCalTemp,
    .WlanATSetWifiCalTemp = ATSetWifiCalTemp,
    
    .WlanATSetWifiCalData = ATSetWifiCalData,
    .WlanATGetWifiCalData = ATGetWifiCalData,
    
    .WlanATSetWifiCal = ATSetWifiCal,
    .WlanATGetWifiCal = ATGetWifiCal,
    .WlanATGetWifiCalSupport = ATGetWifiCalSupport,
    
    .WlanATSetWifiCalFreq = ATSetWifiCalFreq,
    .WlanATGetWifiCalFreq = ATGetWifiCalFreq,
    
    .WlanATSetWifiCalPOW = ATSetWifiCalPOW,
    .WlanATGetWifiCalPOW = ATGetWifiCalPOW
};

/* 向wlan at模块注册该芯片wt接口 */
void wlan_at_reg_rtl8192(WLAN_CHIP_OPS **wlan_ops)
{
    PLAT_WLAN_INFO("enter");
    wlan_ops[rtl8192] = &rtl8192_ops;
    PLAT_WLAN_INFO("exit");
}



