/*************************************************************************
CAUTION : This file is Auto Generated by VBA based on 《V711 MBB PRODUCT_auto_adaptor.xlsm》.
          So, don't modify this file manually!
****************************************************************************/
#ifndef __MBB_PRODUCT_ADAPT_H__
#define __MBB_PRODUCT_ADAPT_H__


#ifdef __cplusplus
extern "C"
{
#endif


/****************************************************************************/
/*************************** MBB 自适配文件开始 *****************************/
/****************************************************************************/
#define    GPIO_NULL                           (-1)               /* 无此GPIO时取-1 */
#define    MENU_KEY_GPIO                       (GPIO_NULL)        /* MENU/WPS键 */
#define    RESET_KEY_GPIO                      (GPIO_NULL)        /* RESET键 */
#define    WIFI_KEY_GPIO                       (GPIO_NULL)        /* WIFI/WLAN键 */
#define    WIFI_ACTIVE_GPIO                    (GPIO_NULL)        /* wifi enable */
#define    WIFI_WAKEUP_BB_GPIO                 (GPIO_NULL)        /* wifi wakeup bb */
#define    CHG_ENABLE_GPIO                     (GPIO_NULL)        /* charge enable */
#define    CHG_BATT_LOW_INT                    (GPIO_NULL)        /* batt low int */
#define    CHG_BATT_ID_CHAN                    (0)                /* batt id hkadc channel */
#define    CHG_BATT_TEMP_CHAN                  (2)                /* batt temp hkadc channel */
#define    CHG_BATT_VOLT_CHAN                  (8)                /* batt volt hkadc channel */
#define    CHG_VBUS_VOLT_CHAN                  (-1)               /* vbus volt hkadc channel */
#define    LCD_BACKLIGHT_GPIO                  (GPIO_NULL)        /* lcd backlight */
#define    LCD_ID00_GPIO                       (GPIO_NULL)        /* lcd id0 */
#define    LCD_ID01_GPIO                       (GPIO_NULL)        /* lcd id1 */
#define    OLED_ID0_GPIO                       (GPIO_NULL)        /* oled id0 */
#define    OLED_ID1_GPIO                       (GPIO_NULL)        /* oled id1 */
#define    OLED_CD_GPIO                        (GPIO_NULL)        /* oled cd */
#define    OTG_ID_DET_GPIO                     (GPIO_NULL)        /* otg detect */
#define    OTG_ID_SET_GPIO                     (GPIO_NULL)        /* otg set */
#define    DMDP_CONNECT_GPIO                   (GPIO_NULL)        /* dmdp connect */
#define    SD_DETECT_GPIO                      (GPIO_NULL)        /* sd detect */
#define    SIM0_DETECT_GPIO                    (GPIO_0_5)         /* sim detect */

/******************************************************************************/
/***************************** MBB 自适配文件结束 *****************************/
/******************************************************************************/


#ifdef __cplusplus
}
#endif
#endif /*__MBB_PRODUCT_ADAPT_H__ */
