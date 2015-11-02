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
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/string.h>
#include <product_config.h>

#include "wlan_at.h"
#include "wlan_utils.h"
#include "drv_version.h"
#include <bsp_shared_ddr.h>

#define WLAN_AT_SSID_SUPPORT            2                  /*支持的SSID组数*/
#define WLAN_AT_KEY_SUPPORT             5                  /*支持的分组数*/

#ifdef BSP_CONFIG_BOARD_E5_E5578
#define WLAN_AT_MODE_SUPPORT            "2,3,4"          /*支持的模式(b/g/n)*/
#else
#define WLAN_AT_MODE_SUPPORT            "1,2,3,4"          /*支持的模式(a/b/g/n)*/
#endif

#define WLAN_AT_BAND_SUPPORT            "0,1"              /*支持的带宽(20M/40M/80M/160M)*/

#ifdef BSP_CONFIG_BOARD_E5_E5578
#define WLAN_AT_TSELRF_SUPPORT          "0,1"          /*支持的天线索引序列*/
#else
#define WLAN_AT_TSELRF_SUPPORT          "0,1,2,3"          /*支持的天线索引序列*/
#endif

#define WLAN_AT_GROUP_MAX               4                  /*支持的最大天线索引*/
#define WLAN_AT_TYPE_MAX                2                  /*支持获取的最大信息类型*/

/*WIFI功率的上下限*/
#define WLAN_AT_POWER_MIN               (-15)
#define WLAN_AT_POWER_MAX               (30)

/*WIFI发射机接收机模式*/
#define WLAN_AT_WIFI_TX_MODE            (17)
#define WLAN_AT_WIFI_RX_MODE            (18)

/*WiFi增益模式*/
#define AT_WIFI_MODE_ONLY_PA            0x00                /*WIFI只支持PA模式*/
#define AT_WIFI_MODE_ONLY_NOPA          0x01                /*WIFI只支持NO PA模式*/
#define AT_WIFI_MODE_PA_NOPA            0x02                /*WIFI同时支持PA模式和NO PA模式*/

#define RX_PACKET_SIZE                  1000                /*装备每次发包数*/

#define WLAN_CHANNEL_2G_MIN             1                   /*2.4G信道最小值*/
#define WLAN_CHANNEL_5G_MIN             36                  /*5G信道最小值*/
#define WLAN_CHANNEL_2G_MAX             14                  /*2.4G信道最大*/
#define WLAN_CHANNEL_2G_MIDDLE          6
#define WLAN_CHANNEL_5G_MAX             165                 /*5G信道最大*/

#define WLAN_CHANNEL_5G_W52_START       36
#define WLAN_CHANNEL_5G_W52_END         48
#define WLAN_CHANNEL_5G_W53_START       52
#define WLAN_CHANNEL_5G_W53_END         64
#define WLAN_CHANNEL_5G_W57_START       149
#define WLAN_CHANNEL_5G_W57_END         161

#define WLAN_CHANNEL_5G_INTERVAL        4                     /*5G信道间隔*/
#define WLAN_CHANNEL_5G_40M_INTERVAL    8                     /*5G 40M信道间隔*/

#define WLAN_FREQ_2G_MAX                2484                  /*2.4G最大频点*/

#define WLAN_FREQ_5G_W52_MIN            5180                  /*W52最小频点*/
#define WLAN_FREQ_5G_W53_MAX            5320                  /*W53最大频点*/

#define WLAN_FREQ_5G_W52_40M_MIN        5190                  /*W52 40M最小频点*/
#define WLAN_FREQ_5G_W53_40M_MAX        5310                  /*W53 40M最大频点*/

#define WLAN_FREQ_5G_W56_MIN            5500                  /*W56最小频点*/
#define WLAN_FREQ_5G_W56_MAX            5700                  /*W56最大频点*/

#define WLAN_FREQ_5G_W56_40M_MIN        5510                  /*W56 40M最小频点*/
#define WLAN_FREQ_5G_W56_40M_MAX        5670                  /*W56 40M最大频点*/

#define WLAN_FREQ_5G_W57_MIN            5745                  /*W57最小频点*/
#define WLAN_FREQ_5G_W57_MAX            5825                  /*W57最大频点*/

#define WLAN_FREQ_5G_W57_40M_MIN        5755                  /*W57最小频点*/
#define WLAN_FREQ_5G_W57_40M_MAX        5795                  /*W57最大频点*/

#define WIFI_CMD_MAX_SIZE               256                   /*cmd字符串256长度*/
#define WIFI_CMD_8_SIZE                 8                     /*cmd字符串8长度*/
#define HUNDRED                         100

#define WIFI_MIMO_MODE                  4                     /*MIMO状态时，WIFI的天线工作模式是4*/
#define WIFI_SISO_MODE                  3                     /*SISO状态时，WIFI的天线工作模式小于3*/

#define DALEY_100_TIME  100
#define DALEY_500_TIME  500
#define DALEY_1000_TIME 1000
#define DALEY_5000_TIME 5000
#define DALEY_2000_TIME 2000

/*向WiFi芯片下发配置命令*/
#define WIFI_TEST_CMD(cmd) do {\
    char temp_cmd[WIFI_CMD_MAX_SIZE] = {0};\
    int cmd_ret = 0;\
    snprintf(temp_cmd, WIFI_CMD_MAX_SIZE, "/system/bin/wifi_brcm/exe/%s", cmd);\
    PLAT_WLAN_INFO("[ret=%d]%s\n", cmd_ret, temp_cmd);\
    cmd_ret = wlan_run_cmd(temp_cmd);\
    if (0 != cmd_ret)\
    {\
        PLAT_WLAN_ERR("Run CMD Error");\ 
        return AT_RETURN_FAILURE;\
    }\
    msleep(DALEY_100_TIME);\
}while(0)


/*WiFi芯片使能断言检测*/
#define ASSERT_WiFi_OFF(ret)                    \
if (AT_WIENABLE_OFF == g_wlan_at_data.wifiStatus) \
{                                               \
    PLAT_WLAN_INFO("Exit on WiFi OFF\n");        \
    return ret;                                 \
}

/*WiFi全局变量结构体 */
typedef struct tagWlanATGlobal
{
    WLAN_AT_WIENABLE_TYPE   wifiStatus;    /*默认加载测试模式*/
    WLAN_AT_WIMODE_TYPE     wifiMode;      /*wifi协议模式*/
    WLAN_AT_WIBAND_TYPE     wifiBand;      /*wifi协议制式*/
    WLAN_AT_WIFREQ_STRU     wifiFreq;      /*wifi频点信息*/
    uint32                  wifiChannel;   /*wifi信道信息*/
    uint32                  wifiRate;      /*wifi发射速率*/
    int32                   wifiPower;     /*wifi发射功率*/
    WLAN_AT_FEATURE_TYPE    wifiTX;        /*wifi发射机状态*/
    WLAN_AT_WIRX_STRU       wifiRX;        /*wifi接收机状态*/
    WLAN_AT_WIRPCKG_STRU    wifiPckg;      /*wifi误包码*/  
    uint32                  wifiGroup;     /*wifi天线模式*/
}WLAN_AT_GLOBAL_ST;

typedef struct tagWlanATPacketREP
{
    unsigned int TotalRxPkts;
    unsigned int BadRxPkts;
    unsigned int UcastRxPkts;
    unsigned int McastRxPkts;
}WLAN_AT_PACK_REP_ST;

typedef struct
{
    uint brate;
    int8 rate_str[WIFI_CMD_8_SIZE];
}BRATE_ST;

/*记录当前的WiFi模式，带宽，频率，速率等参数*/
STATIC WLAN_AT_GLOBAL_ST g_wlan_at_data = {AT_WIENABLE_TEST, AT_WIMODE_80211n, AT_WIBAND_20M
         , {2412, 0}, 1, 6500, 3175, AT_FEATURE_DISABLE, {AT_FEATURE_DISABLE, {0}, {0}}, AT_WiPARANGE_HIGH};/*开启WiFi的默认参数*/

/*WiFi数据包统计信息*/
STATIC WLAN_AT_PACK_REP_ST g_wifi_packet_report = {0};
STATIC WLAN_AT_PACK_REP_ST g_wifi_packet_new_rep = {0};/*实时信息*/
STATIC int g_rx = 0;

/*WiFi有效性校验*/
STATIC int32 check_wifi_valid()
{        
    char *pValStr = "wl ver";    
    
    WIFI_TEST_CMD(pValStr);
   
    return AT_RETURN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/*(1)^WIENABLE 设置WiFi模块使能 */
//////////////////////////////////////////////////////////////////////////
/*****************************************************************************
 函数名称  : int32 WlanATSetWifiEnable(WLAN_AT_WIENABLE_TYPE onoff)
 功能描述  : 用于wifi 进入测试模式，正常模式，关闭wifi
 输入参数  :  0  关闭
              1  打开正常模式
              2  打开测试模式
 输出参数  : NA
 返 回 值  : 0 成功
             1 失败
 其他说明  : 
*****************************************************************************/
STATIC int32 ATSetWifiEnable(WLAN_AT_WIENABLE_TYPE onoff)
{
    int32 ret = AT_RETURN_SUCCESS;

    ret = check_wifi_valid();
    if (AT_RETURN_SUCCESS != ret)
    {       
        PLAT_WLAN_INFO("check_wifi_valid failed!\n");
        return ret;
    }
    
    switch (onoff)
    {
        case AT_WIENABLE_OFF:
            {                
                PLAT_WLAN_INFO("Set wifi to off mode\n");
                
                WIFI_TEST_CMD("wl down");
                msleep(DALEY_2000_TIME);
                g_wlan_at_data.wifiStatus = AT_WIENABLE_OFF;
            }            
            break;
        /* 产测时，如果查询到wienable不能为2，则表示驱动加载出问题
         * 多是第一次上电异常(基本上不会发生)，则下电，上电恢复下
        */    
        case AT_WIENABLE_ON:
            {
                PLAT_WLAN_INFO("Set wifi to normal mode\n");
                
                WIFI_TEST_CMD("wifi_poweroff_43241.sh");
                WIFI_TEST_CMD("wifi_poweron_factory_43241.sh");
                WIFI_TEST_CMD("wl down");

                g_wlan_at_data.wifiStatus = AT_WIENABLE_ON;
            }
            break;
        case AT_WIENABLE_TEST:
            {                
                PLAT_WLAN_INFO("Set wifi to test mode\n");    
                
                WIFI_TEST_CMD("wifi_poweroff_43241.sh");
                WIFI_TEST_CMD("wifi_poweron_factory_43241.sh");
                WIFI_TEST_CMD("wl down");
                
                g_wlan_at_data.wifiStatus = AT_WIENABLE_TEST;
            }         
            break;
        default: 
            ret = AT_RETURN_FAILURE;
            break;
    }   
    
    return ret;
}
/*****************************************************************************
 函数名称  : WLAN_AT_WIENABLE_TYPE WlanATGetWifiEnable()
 功能描述  : 获取当前的WiFi模块使能状态
 输入参数  : NA
 输出参数  : NA
 返 回 值  : 0  关闭
             1  正常模式(信令模式)
             2  测试模式(非信令模式)
 其他说明  : 
*****************************************************************************/
STATIC WLAN_AT_WIENABLE_TYPE ATGetWifiEnable(void)
{    
    int32 ret = AT_RETURN_SUCCESS;
    
    ret = check_wifi_valid();
    if (AT_RETURN_SUCCESS != ret)
    {       
        PLAT_WLAN_INFO("check_wifi_valid failed!\n");        
        return AT_WIENABLE_OFF;
    }

    WIFI_TEST_CMD("wl down");
    
    return g_wlan_at_data.wifiStatus;
}

//////////////////////////////////////////////////////////////////////////
/*(2)^WIMODE 设置WiFi模式参数 目前均为单模式测试*/
//////////////////////////////////////////////////////////////////////////
/*****************************************************************************
 函数名称  : int32 WlanATSetWifiMode(WLAN_AT_WIMODE_TYPE mode)
 功能描述  : 设置WiFi AP支持的制式
 输入参数  : 0,  CW模式
             1,  802.11a制式
             2,  802.11b制式
             3,  802.11g制式
             4,  802.11n制式
             5,  802.11ac制式
 输出参数  : NA
 返 回 值  : 0 成功
             1 失败
 其他说明  : 
*****************************************************************************/
STATIC int32 ATSetWifiMode(WLAN_AT_WIMODE_TYPE mode)
{
    PLAT_WLAN_INFO("enter,mode = %d",mode);
    if (mode > AT_WIMODE_80211ac)
    {
        return (AT_RETURN_FAILURE);
    }   

    if (AT_WIMODE_80211a == mode)
    {
        WIFI_TEST_CMD("wl band a");
        WIFI_TEST_CMD("wl nmode 0");  
    }
    else if (AT_WIMODE_80211b == mode)
    {
        WIFI_TEST_CMD("wl nmode 0");
        WIFI_TEST_CMD("wl gmode 0");
        
    }
    else if (AT_WIMODE_80211g == mode)
    {
        WIFI_TEST_CMD("wl nmode 0");
        WIFI_TEST_CMD("wl gmode 2");        
    }
    else
    {
        WIFI_TEST_CMD("wl nmode 1");
        WIFI_TEST_CMD("wl gmode 1");        
    }
    
    g_wlan_at_data.wifiMode = mode;
    return (AT_RETURN_SUCCESS);
}

/*****************************************************************************
 函数名称  : int32 WlanATGetWifiMode(WLAN_AT_BUFFER_STRU *strBuf)
 功能描述  : 获取当前WiFi支持的制式
             当前模式，以字符串形式返回eg: 2
 输入参数  : NA
 输出参数  : NA
 返 回 值  : NA
 其他说明  : 
*****************************************************************************/
STATIC int32 ATGetWifiMode(WLAN_AT_BUFFER_STRU *strBuf)
{
    if (NULL == strBuf)
    {
        return (AT_RETURN_FAILURE);
    }

    OSA_SNPRINTF(strBuf->buf, WLAN_AT_BUFFER_SIZE, "%d", g_wlan_at_data.wifiMode);
    return (AT_RETURN_SUCCESS);
}

/*****************************************************************************
 函数名称  : int32 WlanATGetWifiModeSupport(WLAN_AT_BUFFER_STRU *strBuf)
 功能描述  : 获取WiFi芯片支持的所有协议模式
             支持的所有模式，以字符串形式返回eg: 2,3,4
 输入参数  : NA
 输出参数  : NA
 返 回 值  : 0 成功
             1 失败
 其他说明  : 
*****************************************************************************/
STATIC int32 ATGetWifiModeSupport(WLAN_AT_BUFFER_STRU *strBuf)
{
    if (NULL == strBuf)
    {
        return (AT_RETURN_FAILURE);
    }
    /*begin add by yangzhiyong for E5573S-607 antenna*/
    uint32  *pu32HwId = (u32 *)SHM_MEM_HW_VER_ADDR;
    printk("the hardid is %x",*pu32HwId);

    if(HW_VER_PRODUCT_E5573S_607  ==  *pu32HwId)
    {
        OSA_SNPRINTF(strBuf->buf, WLAN_AT_BUFFER_SIZE, "%s", "2,3,4");
        return (AT_RETURN_SUCCESS);
    }
    /*end add by yangzhiyong for E5573S-607 antenna*/
    OSA_SNPRINTF(strBuf->buf, WLAN_AT_BUFFER_SIZE, "%s", WLAN_AT_MODE_SUPPORT);
    return (AT_RETURN_SUCCESS);
}

//////////////////////////////////////////////////////////////////////////
/*(3)^WIBAND 设置WiFi带宽参数 */
//////////////////////////////////////////////////////////////////////////
/*****************************************************************************
 函数名称  : int32 WlanATSetWifiBand(WLAN_AT_WIBAND_TYPE width)
 功能描述  : 用于设置wifi带宽
 输入参数  : 0 20M
             1 40M
 输出参数  : NA
 返 回 值  : 0 成功
             1 失败
 其他说明  : 只有在n模式下才可以设置40M带宽
*****************************************************************************/
STATIC int32 ATSetWifiBand(WLAN_AT_WIBAND_TYPE band)
{
    int32 ret = AT_RETURN_SUCCESS;
    
    PLAT_WLAN_INFO("enter,band = %d\n",band);
    
    switch(band)
    {
        case AT_WIBAND_20M: 
            {
                g_wlan_at_data.wifiBand = AT_WIBAND_20M;
                break;
            }         
        case AT_WIBAND_40M:
            {                
                if(AT_WIMODE_80211n == g_wlan_at_data.wifiMode)
                {
                    g_wlan_at_data.wifiBand = AT_WIBAND_40M;                    
                }
                else
                {   
                    PLAT_WLAN_ERR("Error wifi mode,must in n mode");
                    ret = AT_RETURN_FAILURE;
                }
                break;
            }             
        default:
            ret = AT_RETURN_FAILURE;
            break;
    }
     
    return ret;
}

/*****************************************************************************
 函数名称  : int32 WlanATGetWifiBand(WLAN_AT_BUFFER_STRU *strBuf)
 功能描述  :获取当前带宽配置 
            当前带宽，以字符串形式返回eg: 0
 输入参数  : NA
 输出参数  : NA
 返 回 值  : 0 成功
             1 失败
 其他说明  : 
*****************************************************************************/
STATIC int32 ATGetWifiBand(WLAN_AT_BUFFER_STRU *strBuf)
{
    if (NULL == strBuf)
    {
        return (AT_RETURN_FAILURE);
    }

    OSA_SNPRINTF(strBuf->buf, WLAN_AT_BUFFER_SIZE, "%d", g_wlan_at_data.wifiBand);
    return (AT_RETURN_SUCCESS);
}

/*****************************************************************************
 函数名称  : int32 WlanATGetWifiBandSupport(WLAN_AT_BUFFER_STRU *strBuf)
 功能描述  :获取WiFi支持的带宽配置 
            支持带宽，以字符串形式返回eg: 0,1
 输入参数  : NA
 输出参数  : NA
 返 回 值  : 0 成功
             1 失败
 其他说明  : 
*****************************************************************************/
STATIC int32 ATGetWifiBandSupport(WLAN_AT_BUFFER_STRU *strBuf)
{
    if (NULL == strBuf)
    {
        return (AT_RETURN_FAILURE);
    }

    OSA_SNPRINTF(strBuf->buf, WLAN_AT_BUFFER_SIZE, "%s", WLAN_AT_BAND_SUPPORT);
    return (AT_RETURN_SUCCESS);
}

//////////////////////////////////////////////////////////////////////////
/*(4)^WIFREQ 设置WiFi频点 */
//////////////////////////////////////////////////////////////////////////
/*****************************************************************************
 函数名称  : int32 WlanATSetWifiFreq(WLAN_AT_WIFREQ_STRU *pFreq)
 功能描述  : 设置WiFi频点
 输入参数  : NA
 输出参数  : NA
 返 回 值  : 0 成功
             1 失败
 其他说明  : 
*****************************************************************************/
STATIC int32 ATSetWifiFreq(WLAN_AT_WIFREQ_STRU *pFreq)
{
    /*2.4G频点集合*/
    const uint16   ausChannels[] = {2412,2417,2422,2427,2432,2437,2442,2447,2452,2457,2462,2467,2472,2484};/*2.4G*/
    /*5G频点集合*/
    const uint16 aulChannel036[] = {5180, 5200, 5220, 5240, 5260, 5280, 5300, 5320};/*w52和w53*/  
    const uint16 aulChannel100[] = {5500,5520,5540,5560,5580,5600,5620,5640,5660,5680,5700};/*w56*/
    const uint16 aulChannel149[] = {5745,5765,5785,5805,5825};/*w57*/
  
    /*2.4G 40M频点集合*/
    const uint16   ausChannels_40M[] = {2422, 2427, 2432, 2437, 2442, 2447, 2452, 2457, 2462};/*2.4G 40M*/
    /*5G 40M频点集合*/
    const uint16 aulChannel036_40M[] = {5190, 5230, 5270, 5310};/*5G 40M*/
    const uint16 aulChannel100_40M[] = {5510, 5550, 5590, 5630, 5670};/*5G 40M*/
    const uint16 aulChannel149_40M[] = {5755, 5795};/*5G 40M*/
   
    /*5G 40M带宽需要加u的信道*/
    const uint16 channel_5g_40M_u[] = {40,48,56,64,104,112,120,128,136,153,161};    
    /*5G 40M带宽需要加l的信道*/
    const uint16 channel_5g_40M_l[] = {36,44,52,60,100,108,116,124,132,149,157};
    
    int8 acCmd[WIFI_CMD_MAX_SIZE] = {0};
    int8 channeStr[WIFI_CMD_8_SIZE] = {0}; 
    uint16 ulWifiFreq = 0;
    uint16 i = 0;    
    int32 ret = AT_RETURN_SUCCESS;
    
    if (NULL == pFreq)
    {
        return (AT_RETURN_FAILURE);
    }

    /*20M带宽*/
    if (AT_WIBAND_20M == g_wlan_at_data.wifiBand)
    {  
        if (pFreq->value <= WLAN_FREQ_2G_MAX)
        {
            for (i = 0; i < (sizeof(ausChannels) / sizeof(uint16)); i++)
            {
                if (pFreq->value == ausChannels[i])
                {
                    ulWifiFreq = (i + 1);
                    break;
                }
            }
        }
        /*WIFI 5G 信道算法如下，频点对照上面的信道号，具体对应对应表为

        case 36:
            iWIFIchannel = 5180;
            break;
        case 40: 
            iWIFIchannel = 5200;
            break;
        case 44:    
            iWIFIchannel = 5220;
            break;
        case 48: 
            iWIFIchannel = 5240;
            break;
        case 52:    
            iWIFIchannel = 5260;
            break;
        case 56: 
            iWIFIchannel = 5280;
            break;
        case 60:    
            iWIFIchannel = 5300;
            break;
        case 64: 
            iWIFIchannel = 5320;
            break;
        */
        else if ((pFreq->value >= WLAN_FREQ_5G_W52_MIN) && (pFreq->value <= WLAN_FREQ_5G_W53_MAX))
        {
            for (i = 0; i < (sizeof(aulChannel036) / sizeof(uint16)); i++)
            {
                if (pFreq->value == aulChannel036[i])
                {
                    ulWifiFreq = (i * WLAN_CHANNEL_5G_INTERVAL + WLAN_CHANNEL_5G_MIN);
                    break;
                }
            }
        }
        /*WIFI 5G 信道算法如下，频点对照上面的信道号，具体对应对应表为

        case 100:
            iWIFIchannel = 5500;
            break;
        case 104: 
            iWIFIchannel = 5520;
            break;
        case 108:
            iWIFIchannel = 5540;
            break;
        case 112: 
            iWIFIchannel = 5560;
            break;
        case 116:
            iWIFIchannel = 5580;
            break;
        case 120: 
            iWIFIchannel = 5600;
            break;
        case 124:
            iWIFIchannel = 5620;
            break;
        case 128: 
            iWIFIchannel = 5640;
            break;
        case 132: 
            iWIFIchannel = 5660;
            break;
        case 136:
            iWIFIchannel = 5680;
            break;
        case 140: 
            iWIFIchannel = 5700;
            break;
        */
        else if ((pFreq->value >= WLAN_FREQ_5G_W56_MIN) && (pFreq->value <= WLAN_FREQ_5G_W56_MAX))
        {
            for (i = 0; i < (sizeof(aulChannel100) / sizeof(uint16)); i++)
            {
                if (pFreq->value == aulChannel100[i])
                {
                    ulWifiFreq = (i * WLAN_CHANNEL_5G_INTERVAL + HUNDRED);
                    break;
                }
            }

        }
        /*WIFI 5G 信道算法如下，频点对照上面的信道号，具体对应对应表为
        case 149: 
            iWIFIchannel = 5745;
            break;
        case 153:
            iWIFIchannel = 5765;
            break;
        case 157: 
            iWIFIchannel = 5785;
            break;
        case 161: 
            iWIFIchannel = 5805;
            break;
        case 165:
            iWIFIchannel = 5825;
            break;
        */
        else if ((pFreq->value >= WLAN_FREQ_5G_W57_MIN) && (pFreq->value <= WLAN_FREQ_5G_W57_MAX))
        {
            for (i = 0; i < (sizeof(aulChannel149) / sizeof(uint16)); i++)
            {
                if (pFreq->value == aulChannel149[i])
                {
                    ulWifiFreq = (i * WLAN_CHANNEL_5G_INTERVAL + WLAN_CHANNEL_5G_W57_START);
                    break;
                }
            }
        }
        else
        {        
            PLAT_WLAN_INFO("Error 20M wifiFreq parameters\n");      
            return AT_RETURN_FAILURE;
        }

        PLAT_WLAN_INFO("Target Channel = %d\n", ulWifiFreq);
    
        if (!(((WLAN_CHANNEL_2G_MIN <= ulWifiFreq) && (WLAN_CHANNEL_2G_MAX >= ulWifiFreq))
            || ((WLAN_CHANNEL_5G_MIN <= ulWifiFreq) && (WLAN_CHANNEL_5G_MAX >= ulWifiFreq))))
        {
            PLAT_WLAN_INFO("Target Channel ERROR,ulWifiFreq = %u!\n", ulWifiFreq);
            return AT_RETURN_FAILURE;
        }

        if(1 == g_rx)
        {
            WIFI_TEST_CMD("wl down");
        }

        WIFI_TEST_CMD("wl phy_watchdog 0");
        WIFI_TEST_CMD("wl mpc 0");
        WIFI_TEST_CMD("wl country ALL");
        WIFI_TEST_CMD("wl scansuppress 1");
        WIFI_TEST_CMD("wl mimo_bw_cap 0");
        if(ulWifiFreq <= WLAN_CHANNEL_2G_MAX)
        {
            WIFI_TEST_CMD("wl band b");    
        }
        else
        {
            WIFI_TEST_CMD("wl band a");
        }
        OSA_SNPRINTF(acCmd, sizeof(acCmd), "wl channel %d", ulWifiFreq);       
        WIFI_TEST_CMD(acCmd);  
        msleep(DALEY_100_TIME*3); /* 延时300ms */
    }
    else if(AT_WIBAND_40M == g_wlan_at_data.wifiBand)
    {
        if (pFreq->value <= WLAN_FREQ_2G_MAX)
        {
            for (i = 0; i < (sizeof(ausChannels_40M) / sizeof(uint16)); i++)
            {
                if (pFreq->value == ausChannels_40M[i])
                {
                    ulWifiFreq = (i + 1);
                    break;
                }
            }
        }       
        else if ((pFreq->value >= WLAN_FREQ_5G_W52_40M_MIN) && (pFreq->value <= WLAN_FREQ_5G_W53_40M_MAX))
        {
            for (i = 0; i < (sizeof(aulChannel036_40M) / sizeof(uint16)); i++)
            {
                if (pFreq->value == aulChannel036_40M[i])
                {
                    ulWifiFreq = (i * WLAN_CHANNEL_5G_40M_INTERVAL + WLAN_CHANNEL_5G_MIN);
                    break;
                }
            }
        }        
        else if ((pFreq->value >= WLAN_FREQ_5G_W56_40M_MIN) && (pFreq->value <= WLAN_FREQ_5G_W56_40M_MAX))
        {
            for (i = 0; i < (sizeof(aulChannel100_40M) / sizeof(uint16)); i++)
            {
                if (pFreq->value == aulChannel100_40M[i])
                {
                    ulWifiFreq = (i * WLAN_CHANNEL_5G_40M_INTERVAL + HUNDRED);
                    break;
                }
            }

        }        
        else if ((pFreq->value >= WLAN_FREQ_5G_W57_40M_MIN) && (pFreq->value <= WLAN_FREQ_5G_W57_40M_MAX))
        {
            for (i = 0; i < (sizeof(aulChannel149_40M) / sizeof(uint16)); i++)
            {
                if (pFreq->value == aulChannel149_40M[i])
                {
                    ulWifiFreq = (i * WLAN_CHANNEL_5G_40M_INTERVAL + WLAN_CHANNEL_5G_W57_START);
                    break;
                }
            }
        }
        else
        {        
            PLAT_WLAN_INFO("Error 40M wifiFreq parameters\n");      
            return AT_RETURN_FAILURE;
        }

        PLAT_WLAN_INFO("Target Channel = %d\n", ulWifiFreq);
    
        if (!(((WLAN_CHANNEL_2G_MIN <= ulWifiFreq) && (WLAN_CHANNEL_2G_MAX >= ulWifiFreq))
            || ((WLAN_CHANNEL_5G_MIN <= ulWifiFreq) && (WLAN_CHANNEL_5G_MAX >= ulWifiFreq))))
        {
            PLAT_WLAN_INFO("Target Channel ERROR!\n");
            return AT_RETURN_FAILURE;
        }

        /*40M信道加U/L处理*/
        if ((ulWifiFreq > 0) 
                && (ulWifiFreq <= WLAN_CHANNEL_2G_MIDDLE))
        {
            OSA_SNPRINTF(channeStr, sizeof(channeStr), "%dl", ulWifiFreq);             
        }
        else if ((ulWifiFreq > WLAN_CHANNEL_2G_MIDDLE)
                && (ulWifiFreq <= WLAN_CHANNEL_2G_MAX))
        {
            OSA_SNPRINTF(channeStr, sizeof(channeStr), "%dl", ulWifiFreq);
        }
        else if ((ulWifiFreq >= WLAN_CHANNEL_5G_MIN)
                && (ulWifiFreq <= WLAN_CHANNEL_5G_MAX))
        {            
            for (i = 0;i < (sizeof(channel_5g_40M_l) / sizeof(uint16));i++)
            {
                if(ulWifiFreq == channel_5g_40M_l[i])
                {
                    OSA_SNPRINTF(channeStr, sizeof(channeStr), "%dl", ulWifiFreq);                     
                    break;
                }                
            }

            if(i == (sizeof(channel_5g_40M_l) / sizeof(uint16)))
            {
                for (i = 0;i < (sizeof(channel_5g_40M_u) / sizeof(uint16));i++)
                {
                    if(ulWifiFreq == channel_5g_40M_u[i])
                    {
                        OSA_SNPRINTF(channeStr, sizeof(channeStr), "%du", ulWifiFreq);
                        break;
                    }                 
                } 
            }
            
            if(i == (sizeof(channel_5g_40M_u) / sizeof(uint16)))
            {
                OSA_SNPRINTF(channeStr, sizeof(channeStr), "%d", ulWifiFreq); 
            }
        }
        else
        {
            PLAT_WLAN_INFO("40M Channel Process ERROR!\n");
            return AT_RETURN_FAILURE;
        }       

        if ( 1 == g_rx)
        {
            WIFI_TEST_CMD("wl down");
        }
        WIFI_TEST_CMD("wl mpc 0");
        WIFI_TEST_CMD("wl country ALL");
        WIFI_TEST_CMD("wl scansuppress 1");
        WIFI_TEST_CMD("wl mimo_bw_cap 1");
        WIFI_TEST_CMD("wl mimo_txbw 4");

        if(ulWifiFreq <= WLAN_CHANNEL_2G_MAX)
        {
            WIFI_TEST_CMD("wl band b");    
        }
        else
        {
            WIFI_TEST_CMD("wl band a");
        }    
        OSA_SNPRINTF(acCmd, sizeof(acCmd), "wl chanspec %s", channeStr);        
        PLAT_WLAN_INFO("Ready to execute command string:%s\n", acCmd);
        WIFI_TEST_CMD(acCmd);
        WIFI_TEST_CMD("wl status");
    }

    /* 保存全局变量里，以备查询 */
    g_wlan_at_data.wifiFreq.value = pFreq->value;
    g_wlan_at_data.wifiFreq.offset = pFreq->offset;
    g_wlan_at_data.wifiChannel = ulWifiFreq;
    return ret;
}

/*****************************************************************************
 函数名称  : int32 WlanATGetWifiFreq(WLAN_AT_WIFREQ_STRU *pFreq)
 功能描述  : 获取WiFi频点
 输入参数  : NA
 输出参数  : NA
 返 回 值  : 0 成功
             1 失败
 其他说明  : 
*****************************************************************************/
STATIC int32 ATGetWifiFreq(WLAN_AT_WIFREQ_STRU *pFreq)
{   
    if (NULL == pFreq)
    {
        return (AT_RETURN_FAILURE);
    }    
    
    memcpy(pFreq, &(g_wlan_at_data.wifiFreq), sizeof(WLAN_AT_WIFREQ_STRU));
      
    return (AT_RETURN_SUCCESS);
}

//////////////////////////////////////////////////////////////////////////
/*(5)^WIDATARATE 设置和查询当前WiFi速率集速率
  WiFi速率，单位为0.01Mb/s，取值范围为0～65535 */
//////////////////////////////////////////////////////////////////////////
/*****************************************************************************
 函数名称  : int32 WlanATSetWifiDataRate(uint32 rate)
 功能描述  : 设置WiFi发射速率
 输入参数  : NA
 输出参数  : NA
 返 回 值  : 0 成功
             1 失败
 其他说明  : 
*****************************************************************************/
STATIC int32 ATSetWifiDataRate(uint32 rate)
{    
    int8    acCmd[WIFI_CMD_MAX_SIZE] = {0};
    uint32  ulWifiRate = rate / HUNDRED;
    uint32  ulNRate = 0;  
    
    const BRATE_ST wifi_brates_table[] = {{100, "1"}, {200, "2"}, {550, "5.5"}, {1100, "11"}};//b 
    const uint32 wifi_20m_nrates_table[] = {650, 1300, 1950, 2600, 3900, 5200, 5850, 6500};//n ht20
    const uint32 wifi_40m_nrates_table[] = {1350, 2700, 4050, 5400, 8100, 10800, 12150, 13500};//n ht40 
    const uint32 wifi_20m_nrates_table_mimo[] = {1300, 2600, 3900, 5200, 7800, 10400, 11700, 13000};//n mimo ht20
    const uint32 wifi_40m_nrates_table_mimo[] = {2700, 5400, 8100, 10800, 16200, 21600, 24300, 27000};//n mimo ht40}

    #define WIFI_BRATES_TABLE_SIZE (sizeof(wifi_brates_table) / sizeof(BRATE_ST))
    #define WIFI_20M_NRATES_TABLE_SIZE (sizeof(wifi_20m_nrates_table) / sizeof(uint32))
    #define WIFI_40M_NRATES_TABLE_SIZE (sizeof(wifi_40m_nrates_table) / sizeof(uint32))
    #define WIFI_20M_NRATES_TABLE_MIMO_SIZE (sizeof(wifi_20m_nrates_table_mimo) / sizeof(uint32))
    #define WIFI_40M_NRATES_TABLE_MIMO_SIZE (sizeof(wifi_40m_nrates_table_mimo) / sizeof(uint32))

    
    PLAT_WLAN_INFO("WifiRate = %u\n", ulWifiRate);
    
    switch (g_wlan_at_data.wifiMode)
    {
        case AT_WIMODE_CW:
            PLAT_WLAN_INFO("AT_WIMODE_CW\n");
            return (AT_RETURN_FAILURE);            
        case AT_WIMODE_80211a:
            OSA_SNPRINTF(acCmd, sizeof(acCmd), "wl nrate -r %u", ulWifiRate);
            break;           
        case AT_WIMODE_80211b:
            for (ulNRate = 0; ulNRate < WIFI_BRATES_TABLE_SIZE; ulNRate++)
            {
                if (wifi_brates_table[ulNRate].brate == rate)
                {
                    PLAT_WLAN_INFO("bRate Index = %u\n", ulNRate);                    
                    PLAT_WLAN_INFO("bRate Str = %s\n", wifi_brates_table[ulNRate].rate_str);
                    break;
                }                
            }

            if (WIFI_BRATES_TABLE_SIZE == ulNRate)
            {
                WLAN_TRACE_ERROR("bRate Error!\n");
                return (AT_RETURN_FAILURE);
            }
            OSA_SNPRINTF(acCmd, sizeof(acCmd), "wl bg_rate %s", wifi_brates_table[ulNRate].rate_str);
            break;
        case AT_WIMODE_80211g:  
            OSA_SNPRINTF(acCmd, sizeof(acCmd), "wl bg_rate %u", ulWifiRate);
            break;
        case AT_WIMODE_80211n:
            if(g_wlan_at_data.wifiGroup <= WIFI_SISO_MODE)
            {
                if (AT_WIBAND_20M == g_wlan_at_data.wifiBand)
                {
                    /* WIFI 20M n模式WL命令的速率值为0~7，共8个 */
                    for (ulNRate = 0; ulNRate < WIFI_20M_NRATES_TABLE_SIZE; ulNRate++)
                    {
                        if (wifi_20m_nrates_table[ulNRate] == rate)
                        {
                            PLAT_WLAN_INFO("20M NRate Index = %u\n", ulNRate);                        
                            break;
                        }
                    }

                    if (WIFI_20M_NRATES_TABLE_SIZE == ulNRate)
                    {
                        WLAN_TRACE_ERROR("20M NRate Error!\n");
                        return (AT_RETURN_FAILURE);
                    }
                }
                else if (AT_WIBAND_40M == g_wlan_at_data.wifiBand)
                {
                    for (ulNRate = 0; ulNRate < WIFI_40M_NRATES_TABLE_SIZE; ulNRate++)
                    {
                        if (wifi_40m_nrates_table[ulNRate] == rate)
                        {
                            PLAT_WLAN_INFO("40M NRate Index = %u\n", ulNRate);
                            break;
                        }
                    }

                    if (WIFI_40M_NRATES_TABLE_SIZE == ulNRate)
                    {
                        WLAN_TRACE_ERROR("40M NRate Error!\n");
                        return (AT_RETURN_FAILURE);
                    }          
                }            
                OSA_SNPRINTF(acCmd, sizeof(acCmd), "wl nrate -m %u -s 0", ulNRate);           
            }
            else if(g_wlan_at_data.wifiGroup == WIFI_MIMO_MODE)
            {
                if (AT_WIBAND_20M == g_wlan_at_data.wifiBand)
                {
                    /* WIFI 20M mimo n模式WL命令的速率值为8~15，共8个 */
                    for (ulNRate = 0; ulNRate < WIFI_20M_NRATES_TABLE_MIMO_SIZE; ulNRate++)
                    {
                        if (wifi_20m_nrates_table_mimo[ulNRate] == rate)
                        {
                            PLAT_WLAN_INFO("20M Mimo NRate Index = %d\n", ulNRate + WIFI_20M_NRATES_TABLE_MIMO_SIZE);
                            break;
                        }
                    }

                    if (WIFI_20M_NRATES_TABLE_MIMO_SIZE == ulNRate)
                    {
                        WLAN_TRACE_ERROR("20M Mimo NRate Error!\n");
                        return (AT_RETURN_FAILURE);
                    }
                    ulNRate += WIFI_20M_NRATES_TABLE_MIMO_SIZE;
                }
                else if (AT_WIBAND_40M == g_wlan_at_data.wifiBand)
                {
                    for (ulNRate = 0; ulNRate < WIFI_40M_NRATES_TABLE_MIMO_SIZE; ulNRate++)
                    {
                        if (wifi_40m_nrates_table_mimo[ulNRate] == rate)
                        {
                            PLAT_WLAN_INFO("40M Mimo NRate Index = %d\n", ulNRate + WIFI_40M_NRATES_TABLE_MIMO_SIZE);
                            break;
                        }
                    }

                    if (WIFI_40M_NRATES_TABLE_MIMO_SIZE == ulNRate)
                    {
                        WLAN_TRACE_ERROR("40M Mimo NRate Error!\n");
                        return (AT_RETURN_FAILURE);
                    }          
                    ulNRate += WIFI_40M_NRATES_TABLE_MIMO_SIZE;
                }            
                OSA_SNPRINTF(acCmd, sizeof(acCmd), "wl nrate -m %d -s 3", ulNRate);
            }
            else
            {
                WLAN_TRACE_ERROR("Group Error!\n");
            }
            break;
        default:
            return (AT_RETURN_FAILURE);            
    }    
    WIFI_TEST_CMD(acCmd);    
    /*保存全局变量里，以备查询*/
    g_wlan_at_data.wifiRate = rate;
       
    return (AT_RETURN_SUCCESS);    
}
/*****************************************************************************
 函数名称  : uint32 WlanATGetWifiDataRate()
 功能描述  : 查询当前WiFi速率设置
 输入参数  : NA
 输出参数  : NA
 返 回 值  : wifi速率
 其他说明  : 
*****************************************************************************/
STATIC uint32 ATGetWifiDataRate(void)
{
    return g_wlan_at_data.wifiRate;
}

//////////////////////////////////////////////////////////////////////////
/*(6)^WIPOW 来设置WiFi发射功率 
   WiFi发射功率，单位为0.01dBm，取值范围为 -32768～32767 */
//////////////////////////////////////////////////////////////////////////
/*****************************************************************************
 函数名称  : int32 WlanATSetWifiPOW(int32 power_dBm_percent)
 功能描述  : 设置WiFi发射功率
 输入参数  : NA
 输出参数  : NA
 返 回 值  : 0 成功
             1 失败
 其他说明  : 
*****************************************************************************/
STATIC int32 ATSetWifiPOW(int32 power_dBm_percent)
{
    int8    acCmd[WIFI_CMD_MAX_SIZE] = {0};
    int32 lWifiPower = power_dBm_percent / HUNDRED;

    if ((lWifiPower >= WLAN_AT_POWER_MIN) && (lWifiPower <= WLAN_AT_POWER_MAX))
    {
        OSA_SNPRINTF(acCmd, sizeof(acCmd), "wl txpwr1 -d -o %u", lWifiPower);
        WIFI_TEST_CMD(acCmd);

        /*保存全局变量里，以备查询*/
        g_wlan_at_data.wifiPower = power_dBm_percent;
        return (AT_RETURN_SUCCESS);
    }
    else
    {
        PLAT_WLAN_INFO("Invalid argument\n");
        return (AT_RETURN_FAILURE);
    }    
}

/*****************************************************************************
 函数名称  : int32 WlanATGetWifiPOW()
 功能描述  : 获取WiFi当前发射功率
 输入参数  : NA
 输出参数  : NA
 返 回 值  : NA
 其他说明  : 
*****************************************************************************/
STATIC int32 ATGetWifiPOW(void)
{
    return g_wlan_at_data.wifiPower;
}

//////////////////////////////////////////////////////////////////////////
/*(7)^WITX 来设置WiFi发射机开关 */
//////////////////////////////////////////////////////////////////////////
/*****************************************************************************
 函数名称  : int32 WlanATSetWifiTX(WLAN_AT_FEATURE_TYPE onoff)
 功能描述  : 打开或关闭wifi发射机
 输入参数  : 0 关闭
             1 打开
 输出参数  : NA
 返 回 值  : 0 成功
             1 失败
 其他说明  : 
*****************************************************************************/
STATIC int32 ATSetWifiTX(WLAN_AT_FEATURE_TYPE onoff)
{
    int8    tcmd[WIFI_CMD_MAX_SIZE] = {0};
    
    if (AT_WIMODE_CW == g_wlan_at_data.wifiMode)
    {
        if (AT_FEATURE_DISABLE == onoff)
        {
            WIFI_TEST_CMD("wl fqacurcy 0");
        }
        else
        {
            WIFI_TEST_CMD("wl down");
            WIFI_TEST_CMD("wl mpc 0");
            WIFI_TEST_CMD("wl mimo_bw_cap 0");
            WIFI_TEST_CMD("wl mimo_txbw -1");
            WIFI_TEST_CMD("wl txant 1");
            WIFI_TEST_CMD("wl antdiv 1");
            if(WLAN_CHANNEL_2G_MAX < g_wlan_at_data.wifiChannel)
            {
                WIFI_TEST_CMD("wl band a");
            }
            else
            {
                WIFI_TEST_CMD("wl band b");
            }
            WIFI_TEST_CMD("wl up");
            WIFI_TEST_CMD("wl phy_txpwrctrl 0");
            WIFI_TEST_CMD("wl phy_txpwrindex 0 127");
            WIFI_TEST_CMD("wl phy_txpwrctrl 1");
            WIFI_TEST_CMD("wl out");
            OSA_SNPRINTF(tcmd, sizeof(tcmd), "wl fqacurcy %d", g_wlan_at_data.wifiChannel);
            WIFI_TEST_CMD(tcmd);
        }
    }
    else 
    {
        if(AT_FEATURE_DISABLE == onoff)
        {
            WIFI_TEST_CMD("wl pkteng_stop tx");
            WIFI_TEST_CMD("wl down");
        }
        else
        {
            /* 调用底软接口 */
            WIFI_TEST_CMD("wl mpc 0");
            WIFI_TEST_CMD("wl country ALL");
            WIFI_TEST_CMD("wl frameburst 1");
            WIFI_TEST_CMD("wl scansuppress 1");
            WIFI_TEST_CMD("wl up");
            msleep(DALEY_100_TIME*4); /* 延时400ms */

            WIFI_TEST_CMD("wl pkteng_start 00:11:22:33:44:55 tx 100 1500 0"); 
            msleep(DALEY_100_TIME*4); /* 延时400ms */
        } 
    }
    
    /*保存全局变量里，已备查询*/
    g_wlan_at_data.wifiTX = onoff;
     
    return (AT_RETURN_SUCCESS);
}
/*****************************************************************************
 函数名称  : WLAN_AT_FEATURE_TYPE WlanATGetWifiTX()
 功能描述  : 查询当前WiFi发射机状态信息
 输入参数  : NA
 输出参数  : NA
 返 回 值  : 0 关闭发射机
             1 打开发射机
 其他说明  : 
*****************************************************************************/
STATIC WLAN_AT_FEATURE_TYPE ATGetWifiTX(void)
{
    return g_wlan_at_data.wifiTX;
}

//////////////////////////////////////////////////////////////////////////
/*(8)^WIRX 设置WiFi接收机开关 */
//////////////////////////////////////////////////////////////////////////
STATIC int32 ATSetWifiRX(WLAN_AT_WIRX_STRU *params)
{
    int8    acCmd[WIFI_CMD_MAX_SIZE] = {0};
    if (NULL == params)
    {
        return (AT_RETURN_FAILURE);
    }
    

    PLAT_WLAN_INFO("WlanATSetWifiRX in, g_rx = %d\n", g_rx);
    if (WLAN_AT_GROUP_MAX == g_wlan_at_data.wifiGroup)
    {  
        g_rx = 0; // 0 表示当前Tx模式开启
    }
    else
    {
        g_rx = 1; // 1 表示当前rx模式开启
    }    
    PLAT_WLAN_INFO("WlanATSetWifiRX, g_rx = %d\n", g_rx);
     
    switch (params->onoff)
    {
        case AT_FEATURE_DISABLE:
            WIFI_TEST_CMD("wl pkteng_stop rx");
            break;
            
        case AT_FEATURE_ENABLE:
            PLAT_WLAN_INFO("src mac is %s\n",params->src_mac);
            WIFI_TEST_CMD("wl up");
            OSA_SNPRINTF(acCmd, sizeof(acCmd), "wl pkteng_start %s rx", params->src_mac);
            WIFI_TEST_CMD(acCmd);
            WIFI_TEST_CMD("wl counters"); 
            memcpy(&g_wifi_packet_report, &g_wifi_packet_new_rep, sizeof(g_wifi_packet_report));
            break;
            
        default:
            return (AT_RETURN_FAILURE);
    }
    
    memcpy(&g_wlan_at_data.wifiRX, params, sizeof(WLAN_AT_WIRX_STRU));
    
    return (AT_RETURN_SUCCESS);
}

/*****************************************************************************
 函数名称  : int32 WlanATGetWifiRX(WLAN_AT_WIRX_STRU *params)
 功能描述  : 获取wifi接收机的状态
 输入参数  : NA
 输出参数  : NA
 返 回 值  : NA
 其他说明  : 
*****************************************************************************/
STATIC int32 ATGetWifiRX(WLAN_AT_WIRX_STRU *params)
{
    if (NULL == params)
    {
        return (AT_RETURN_FAILURE);
    }    
    
    memcpy(params, &g_wlan_at_data.wifiRX, sizeof(WLAN_AT_WIRX_STRU));
    
    return (AT_RETURN_SUCCESS);
}

//////////////////////////////////////////////////////////////////////////
/*(9)^WIRPCKG 查询WiFi接收机误包码，上报接收到的包的数量*/
//////////////////////////////////////////////////////////////////////////
/*****************************************************************************
 函数名称  : int32 WlanATSetWifiRPCKG(int32 flag)
 功能描述  : 清除Wifi接收统计包为零
 输入参数  : 0 清除wifi统计包
             非0 无效参数
 输出参数  : NA
 返 回 值  : 0 成功
             1 失败
 其他说明  : 
*****************************************************************************/
STATIC int32 ATSetWifiRPCKG(int32 flag)
{
    ASSERT_WiFi_OFF(AT_RETURN_FAILURE);

    if (0 != flag)
    {
        PLAT_WLAN_INFO("Exit on flag = %d\n", flag);
        return (AT_RETURN_FAILURE);
    }

    WIFI_TEST_CMD("wl counters");
    memcpy(&g_wifi_packet_report, &g_wifi_packet_new_rep, sizeof(g_wifi_packet_report));
    
    return (AT_RETURN_SUCCESS);
}

/*****************************************************************************
 函数名称  : wlan_at_get_packet_report
 功能描述  : 获取wifi接收机接收包个数
 输入参数  : char * pValueStr
 输出参数  : NA             
 返 回 值  : uiRetPcktsNumBuf
 其他说明  : 
*****************************************************************************/
void wlan_at_get_packet_report(unsigned int total, unsigned int bad, unsigned int ucast, unsigned int mcast)
{
    PLAT_WLAN_INFO("Enter [Total=%d,Bad=%d,Ucast=%d,Mcast=%d]\n", total, bad, ucast, mcast);
    g_wifi_packet_new_rep.TotalRxPkts = total;
    g_wifi_packet_new_rep.BadRxPkts = bad;
    g_wifi_packet_new_rep.UcastRxPkts = ucast;
    g_wifi_packet_new_rep.McastRxPkts = mcast;
}

/*****************************************************************************
 函数名称  : int32 WlanATSetWifiRPCKG(int32 flag)
 功能描述  : 查询WiFi接收机误包码，上报接收到的包的数量
 输入参数  : WLAN_AT_WIRPCKG_STRU *params
 输出参数  : uint16 good_result; //单板接收到的好包数，取值范围为0~65535
             uint16 bad_result;  //单板接收到的坏包数，取值范围为0~65535
 返 回 值  : NA
 其他说明  : 
*****************************************************************************/
STATIC int32 ATGetWifiRPCKG(WLAN_AT_WIRPCKG_STRU *params)
{
    int32 ret = AT_RETURN_SUCCESS; 
    if (NULL == params)
    {        
        WLAN_TRACE_ERROR("%s:POINTER_NULL!\n", __FUNCTION__);
        ret = AT_RETURN_FAILURE;
        return ret;
    }
    /* 判断接收机是否打开 */
    if(AT_FEATURE_DISABLE == g_wlan_at_data.wifiRX.onoff)
    {
        WLAN_TRACE_ERROR("%s:Not Rx Mode.\n", __FUNCTION__);
        ret = AT_RETURN_FAILURE;
        return ret;
    }   

    WIFI_TEST_CMD("wl counters");
    PLAT_WLAN_INFO("Enter [old = %d, new = %d]\n", g_wifi_packet_report.UcastRxPkts, g_wifi_packet_new_rep.UcastRxPkts);      
    
    params->good_result = (uint16)(g_wifi_packet_new_rep.UcastRxPkts - g_wifi_packet_report.UcastRxPkts);
    params->bad_result = 0;   
    
    PLAT_WLAN_INFO("Exit [good = %d, bad = %d]\n", params->good_result, params->bad_result);   
    
    return (AT_RETURN_SUCCESS);
}

//////////////////////////////////////////////////////////////////////////
/*(10)^WIINFO 查询WiFi的相关信息*/
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/*(11)^WIPLATFORM 查询WiFi方案平台供应商信息 */
//////////////////////////////////////////////////////////////////////////
/*****************************************************************************
 函数名称  : WLAN_AT_WIPLATFORM_TYPE WlanATGetWifiPlatform()
 功能描述  : 查询WiFi方案平台供应商信息
 输入参数  : NA
 输出参数  : NA
 返 回 值  : NA
 其他说明  : 
*****************************************************************************/
STATIC WLAN_AT_WIPLATFORM_TYPE ATGetWifiPlatform(void)
{
    return (AT_WIPLATFORM_BROADCOM);
}

//////////////////////////////////////////////////////////////////////////
/*(12)^TSELRF 查询设置单板的WiFi射频通路*/
//////////////////////////////////////////////////////////////////////////
/*****************************************************************************
 函数名称  : int32 WlanATSetTSELRF(uint32 group)
 功能描述  : 设置天线，非多通路传0
 输入参数  : NA
 输出参数  : NA
 返 回 值  : NA
 其他说明  : 
*****************************************************************************/
STATIC int32 ATSetTSELRF(uint32 group)
{
    int8 acCmd[WIFI_CMD_MAX_SIZE] = {0};
    
    if(WLAN_AT_GROUP_MAX < group)
    {
        return AT_RETURN_FAILURE;
    }
    
    g_wlan_at_data.wifiGroup = group;

    PLAT_WLAN_INFO("Enter,group = %u\n", group); 
    
    if(group <= WIFI_SISO_MODE)
    {
        WIFI_TEST_CMD("wl sgi_tx 0");

        OSA_SNPRINTF(acCmd, sizeof(acCmd), "wl txchain %u", group % 2 + 1);   /*天线模式*/
        WIFI_TEST_CMD(acCmd);
    
        OSA_SNPRINTF(acCmd, sizeof(acCmd), "wl rxchain %u", group % 2 + 1);   /*天线模式*/
        WIFI_TEST_CMD(acCmd);

        OSA_SNPRINTF(acCmd, sizeof(acCmd), "wl txant %u", group % 2);   /*天线模式*/
        WIFI_TEST_CMD(acCmd);
    
        OSA_SNPRINTF(acCmd, sizeof(acCmd), "wl antdiv %u", group % 2);  /*天线模式*/
        WIFI_TEST_CMD(acCmd);
    }
    else if(group == WIFI_MIMO_MODE)
    {
        OSA_SNPRINTF(acCmd, sizeof(acCmd), "wl txchain 3");  /*天线模式*/
        WIFI_TEST_CMD(acCmd);
        
        OSA_SNPRINTF(acCmd, sizeof(acCmd), "wl rxchain 3");  /*天线模式*/
        WIFI_TEST_CMD(acCmd);
    }   
    return (AT_RETURN_SUCCESS);
}

/*****************************************************************************
 函数名称  : int32 WlanATGetTSELRFSupport(WLAN_AT_BUFFER_STRU *strBuf)
 功能描述  : 支持的天线索引序列，以字符串形式返回eg: 0,1,2,3
 输入参数  : NA
 输出参数  : NA
 返 回 值  : NA
 其他说明  : 
*****************************************************************************/
STATIC int32 ATGetTSELRFSupport(WLAN_AT_BUFFER_STRU *strBuf)
{
    if (NULL == strBuf)
    {
        return (AT_RETURN_FAILURE);
    }

    /*begin add by yangzhiyong for E5573S-607 antenna*/
    uint32  *pu32HwId = (u32 *)SHM_MEM_HW_VER_ADDR;
    if(HW_VER_PRODUCT_E5573S_607  ==  *pu32HwId)
    {
        OSA_SNPRINTF(strBuf->buf, WLAN_AT_BUFFER_SIZE, "%s", "0,1");
        return (AT_RETURN_SUCCESS);
    }
    /*end add by yangzhiyong for E5573S-607 antenna*/

    OSA_SNPRINTF(strBuf->buf, sizeof(strBuf->buf), "%s", WLAN_AT_TSELRF_SUPPORT);
    return (AT_RETURN_SUCCESS);
}

//////////////////////////////////////////////////////////////////////////
/*(13)^WiPARANGE设置、读取WiFi PA的增益情况*/
//////////////////////////////////////////////////////////////////////////
/*****************************************************************************
 函数名称  : int wifi_set_pa_mode(int wifiPaMode)
 功能描述  : 设置WiFi PA的增益情况
 输入参数  : 增益模式
 输出参数  : NA
 返 回 值  : NA
 其他说明  : 目前仅只支持NO PA模式
*****************************************************************************/
STATIC int wifi_set_pa_mode(int wifiPaMode)
{
    if (AT_WIFI_MODE_ONLY_PA == wifiPaMode)
    {
        return AT_RETURN_SUCCESS;
    }
    else
    {
        return AT_RETURN_FAILURE;
    }
}

/*****************************************************************************
 函数名称  : int wifi_set_pa_mode(int wifiPaMode)
 功能描述  : 获取支持的WiFi PA的增益情况
 输入参数  : NA
 输出参数  : NA
 返 回 值  : NA
 其他说明  : 目前仅只支持NO PA模式
*****************************************************************************/
STATIC int wifi_get_pa_mode(void)
{
    return AT_WIFI_MODE_ONLY_PA;
}
/*****************************************************************************
 函数名称  : int32 WlanATSetWifiParange(WLAN_AT_WiPARANGE_TYPE pa_type)
 功能描述  : 设置WiFi PA的增益情况
 输入参数  : NA
 输出参数  : NA
 返 回 值  : NA
 其他说明  : 
*****************************************************************************/
STATIC int32 ATSetWifiParange(WLAN_AT_WiPARANGE_TYPE pa_type)
{
    int32 ret = AT_RETURN_FAILURE;
    switch (pa_type)
    {
        case AT_WiPARANGE_LOW:
            ret = wifi_set_pa_mode(AT_WIFI_MODE_ONLY_NOPA);
            break;
        case AT_WiPARANGE_HIGH:
            ret = wifi_set_pa_mode(AT_WIFI_MODE_ONLY_PA);
            break;
        default:
            break;
    }
    return ret;
}

/*****************************************************************************
 函数名称  : WLAN_AT_WiPARANGE_TYPE WlanATGetWifiParange()
 功能描述  : 读取WiFi PA的增益情况
 输入参数  : NA
 输出参数  : NA
 返 回 值  : NA
 其他说明  : 
*****************************************************************************/
STATIC WLAN_AT_WiPARANGE_TYPE ATGetWifiParange(void)
{
    WLAN_AT_WiPARANGE_TYPE lWifiPAMode = AT_WiPARANGE_BUTT;
    switch (wifi_get_pa_mode())
    {
        case AT_WIFI_MODE_ONLY_NOPA:
            lWifiPAMode = AT_WiPARANGE_LOW;
            break;
        case AT_WIFI_MODE_ONLY_PA:
            lWifiPAMode = AT_WiPARANGE_HIGH;
            break;
        default:
            break;
    }
    return lWifiPAMode;
}

/*****************************************************************************
 函数名称  : int32 WlanATGetWifiParangeSupport(WLAN_AT_BUFFER_STRU *strBuf)
 功能描述  : 支持的pa模式序列，以字符串形式返回eg: l,h
 输入参数  : NA
 输出参数  : NA
 返 回 值  : NA
 其他说明  : 
*****************************************************************************/
STATIC int32 ATGetWifiParangeSupport(WLAN_AT_BUFFER_STRU *strBuf)
{
    if (NULL == strBuf)
    {
        return (AT_RETURN_FAILURE);
    }
    switch (wifi_get_pa_mode())
    {
        case AT_WIFI_MODE_ONLY_NOPA:
            OSA_SNPRINTF(strBuf->buf, sizeof(strBuf->buf), "%c", AT_WiPARANGE_LOW);
            break;
        case AT_WIFI_MODE_ONLY_PA:
            OSA_SNPRINTF(strBuf->buf, sizeof(strBuf->buf), "%c", AT_WiPARANGE_HIGH);
            break;
        case AT_WIFI_MODE_PA_NOPA:
            OSA_SNPRINTF(strBuf->buf, sizeof(strBuf->buf), "%c, %c"
                                                 , AT_WiPARANGE_LOW ,AT_WiPARANGE_HIGH);
            break;
        default:
            return (AT_RETURN_FAILURE);
    }
    
    return (AT_RETURN_SUCCESS);
}


/* 以下241芯片不支持 */
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
    return  AT_RETURN_FAILURE;
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
    return  AT_RETURN_FAILURE;
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
    return  AT_RETURN_FAILURE;
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
    return  AT_RETURN_FAILURE;
}

/*===========================================================================
 (16)^WICAL设置、读取校准的启动状态，是否支持补偿
===========================================================================*/
/*****************************************************************************
 函数名称  : int32 WlanATSetWifiCal(WLAN_AT_FEATURE_TYPE onoff)
 功能描述  : 设置校准的启动状态
 输入参数  : onoff:0,结束校准；1,启动校准
 输出参数  : NA
 返 回 值  : WLAN_AT_FEATURE_TYPE
*****************************************************************************/
STATIC int32 ATSetWifiCal(WLAN_AT_FEATURE_TYPE onoff)
{
    return  AT_RETURN_FAILURE;
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
    return  AT_RETURN_FAILURE;
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
    return  AT_FEATURE_DISABLE;
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
    return  AT_RETURN_FAILURE;
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
    return  AT_RETURN_FAILURE;
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
    return  AT_RETURN_FAILURE;
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
    return  AT_RETURN_FAILURE;
}



STATIC WLAN_CHIP_OPS bcm43241_ops = 
{
    .WlanATSetWifiEnable = ATSetWifiEnable,
    .WlanATGetWifiEnable = ATGetWifiEnable,
    .WlanATSetWifiMode   = ATSetWifiMode,
    .WlanATGetWifiMode   = ATGetWifiMode,
    .WlanATGetWifiModeSupport   = ATGetWifiModeSupport,

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
    .WlanATGetTSELRFSupport =  ATGetTSELRFSupport,

    .WlanATSetWifiParange = ATSetWifiParange,
    .WlanATGetWifiParange = ATGetWifiParange,

    .WlanATGetWifiParangeSupport = ATGetWifiParangeSupport,
    
    .WlanATGetWifiCalTemp = NULL,
    .WlanATSetWifiCalTemp = NULL,
    .WlanATSetWifiCalData = NULL,
    .WlanATGetWifiCalData = NULL,
    .WlanATSetWifiCal = NULL,
    .WlanATGetWifiCal = NULL,
    .WlanATGetWifiCalSupport = NULL,
    .WlanATSetWifiCalFreq = NULL,
    .WlanATGetWifiCalFreq = NULL,
    .WlanATSetWifiCalPOW = NULL,
    .WlanATGetWifiCalPOW = NULL
};

/* 注册bcm43241到wlan at模块 */
void wlan_at_reg_bcm43241(WLAN_CHIP_OPS **wlan_ops)
{
    PLAT_WLAN_INFO("enter");
    wlan_ops[bcm43241] = &bcm43241_ops;
    PLAT_WLAN_INFO("exit");
}


//////////////////////////////////////////////////////////////////////////
EXPORT_SYMBOL(wlan_at_get_packet_report);
