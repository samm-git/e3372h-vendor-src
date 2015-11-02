/***************************************************************************************************
  Copyright, 1988-2013, Huawei Tech. Co., Ltd.
  File name:      mbb_adapt.h
  Author:         xuchao x00202188
  Version:        V1.0 
  Date:           2014-07-20
  Description:    软硬件配置在此文件中统一配置
  Others: 
                  1：按键GPIO配置
                  2：LED的GPIO配置
                  3：LED的DR和名字配置
  Warning:        
                  1: 软硬件联调前，请根据硬件提供的接口文档适配此头文件
******************************************************************************************************/
#ifndef __MBB_ADAPT_H__
#define __MBB_ADAPT_H__

#include <product_config.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define GPIO_NULL               -1          /* 无此GPIO时取-1 */

/*  按键GPIO配置，如果无此按键则配为-1 */
#define MENU_KEY_GPIO          (GPIO_NULL)  /* MENU/WPS键*/
#define RESET_KEY_GPIO         (GPIO_2_16)   /* RESET键*/
#define WIFI_KEY_GPIO          (GPIO_NULL)  /* WIFI/WLAN键*/

/*  LED灯GPIO配置请手动修改led_dr.c 和 leds-gpio.c */

#define WIFI_ACTIVE_GPIO     (GPIO_2_21)  /*wifi enable*/
#define WIFI_WAKEUP_BB_GPIO     (GPIO_NULL)  /*wifi wakeup bb*/

#define CHG_ENABLE_GPIO       (GPIO_NULL)  /*charge enable*/
#define CHG_BATT_LOW_INT      (GPIO_NULL)  /*batt low int*/
#define CHG_BATT_ID_CHAN      (0)  /*batt id hkadc channel*/
#define CHG_BATT_TEMP_CHAN    (2)  /*batt temp hkadc channel*/
#define CHG_BATT_VOLT_CHAN    (8)  /*batt volt hkadc channel*/
#define CHG_VBUS_VOLT_CHAN    (-1)  /*vbus volt hkadc channel*/

#define LCD_BACKLIGHT_GPIO   (GPIO_NULL)  /*lcd backlight*/
#define LCD_ID00_GPIO         (GPIO_NULL)  /*lcd id0*/
#define LCD_ID01_GPIO         (GPIO_NULL)  /*lcd id1*/

#define OLED_ID0_GPIO        (GPIO_NULL)  /*oled id0*/
#define OLED_ID1_GPIO        (GPIO_NULL)  /*oled id1*/
#define OLED_CD_GPIO         (GPIO_NULL)  /*oled cd*/

#define OTG_ID_DET_GPIO      (GPIO_NULL)  /*otg detect*/
#define OTG_ID_SET_GPIO      (GPIO_NULL)  /*otg set*/
#define DMDP_CONNECT_GPIO    (GPIO_NULL)  /*dmdp connect*/

#define SD_DETECT_GPIO      (GPIO_NULL)  /*sd detect*/

#define SIM0_DETECT_GPIO     (GPIO_0_5)  /*sim detect*/


#ifdef __cplusplus
}
#endif

#endif /*__MBB_ADAPT_H__ */
