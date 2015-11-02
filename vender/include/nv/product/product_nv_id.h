/******************************************************************************

    Copyright(C)2008,Hisilicon Co. LTD.

 ******************************************************************************
  File Name       :   product_nv_id.h
  Description     :   productNV ID枚举定义
  History         :
******************************************************************************/
#ifndef __PRODUCT_NV_ID_H__
#define __PRODUCT_NV_ID_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*
 *  NV ID 的添加按从小到大排列
 */

typedef enum
{
    NV_ID_PRODUCT_START = 0xc350,
    NV_ID_GID1_LOCK                   = 0xC3A3,    /*50083*/
    NV_ID_DYNAMIC_NAME                = 0xc3bc,     /*50108*/
    NV_ID_DYNAMIC_INFO_NAME           = 0xc3bd,     /*50109*/
    NV_ID_SPECIAL_PLMN_NOT_ROAM       = 0xC3B7,    /*50103*/
    NV_Item_HUAWEI_PLMN_MODIFY        = 0xC3C3,    /*50115*/
    NV_ID_CSIM_CUSTOMIZATION          = 0xC40D,    /*50189*/
    NV_ID_HUAWEI_SYSCFGEX_MODE_LIST   = 0xC416,    /*50198*/
    NV_TEST_POWERUP_MODE_CONTROL_FLAG = 0xC4BC,   /*50364*/
    NV_CHG_SHUTOFF_TEMP_PROTECT_I     = 0xC4D1,   /*50385*/
    NV_CHG_SHUTOFF_VOLT_PROTECT_I     = 0xC4D2,   /*50386*/
    NV_ID_CUSTOMIZED_BAND_GROUP       = 0xC4FB,   /*50427*/
    NV_ID_SOFT_RELIABLE_CFG  = 0xC50A,
    NV_ID_HPLMN_FIRST_UMTS_TO_LTE     = 0xC515,   /*50453*/
    NV_ID_EE_OPERATOR_NAME_DISPLAY    = 0xC516,   /*50454*/
    NV_ID_WEB_SITE                    = 0xCB84,
    NV_ID_WPS_PIN                     = 0xCB8D,
    NV_ID_WEB_USER_NAME               = 0xCB9C,
    NV_ID_HPLMN_SEARCH_REGARDLESS_MCC_SUPPORT_EX = 0xC4F6,
    NV_ID_DSFLOW_REPORT_MUTI_WAN = 0xC4EA,/*50410*/
    NV_ID_WINBLUE_PROFILE             = 0xC4F8, /* 50424 */
    NV_ITEM_RPLMN_ACT_DISABLE_CFG = 0xC4F9,  /*50425*/
    NV_ID_PIH_IMSI_TIMER              = 0xC4FA,   /*50426*/
    NV_START_MANUAL_TO_AUTO = 0xC4FC, /*50428 */
    NV_ID_VODAFONE_CPBS               = 0xC4FD,   /*50429*/
    NV_ID_TATA_DEVICE_LOCK            = 0xC500,   /*50432*/
    NV_Item_HPLMNWithinEPLMNNotRoam = 0XC501,/*50433 */
    NV_Item_APN_LOCK = 0xC508,/*50440*/
    NV_ID_SMS_AUTO_REG                = 0xC511,   /*50449*/
    NV_ID_TME_CPIN                    = 0xC512,   /*50450*/
    NV_ID_LED_PARA                  = 0xC517,/* 50455  */
    NV_ID_USB_CDC_NET_SPEED           = 0xC518, /* 50456 */
    NV_HUAWEI_COUL_INFO_I             = 0xC51E,  /*50462*/
    NV_ID_PLATFORM_CATEGORY_SET     = 0xC51A,   /* 50458 */
    NV_ID_AT_GET_CHANWIINFO = 0xC524,/*50468*/
    NV_ID_GET_PWRWIINFO = 0xC525,/*50469*/
    NV_TELNET_SWITCH_I             = 0xC545,  /*50501*/
    NV_HUAWEI_OEMLOCK_I   = 0xC546 ,  /*50502*/
    NV_HUAWEI_SIMLOCK_I   = 0xC547 ,  /*50503*/
    NV_ID_SIM_CUSM_FEATURE = 0xC54E,/*50510*/
    NV_HUAWEI_ROAM_WHITELIST_I         = 0xC550, /*50512*/
    NV_HUAWEI_ROAM_WHITELIST_EXTEND1_I = 0xC551, /*50513*/
    NV_DRV_MAC_NUM                  = 0xC555, /*50517*/
    NV_TEMP_PROTECT_SAR_REDUCTION      = 0xC558 ,  /*50520*/
    NV_ID_LTE_TDD_PRIOR             = 0xC55F,   /*50527*/
    NV_ID_LTE_BANDLOCK                 = 0xC560,   /*50528*/
    NV_HUAWEI_BATTERY_INFO_I     = 0xC567,  /*50535*/
    NV_ID_PRODUCT_END   = 0xcb1f
}NV_ID_PRODUCT_ENUM;


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif


