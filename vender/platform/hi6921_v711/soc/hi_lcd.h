/*************************************************************************
*   版权所有(C) 2008-2013, 深圳华为技术有限公司.
*
*   文 件 名 :  bsp_lcd.h
*
*
*   描    述 :
*
*************************************************************************/
#ifndef __HI_LCD_H__
#define __HI_LCD_H__

#include <osl_bio.h>
#include <bsp_memmap.h>
#include <hi_gpio.h>
#include <product_config.h>


#ifdef __cplusplus
extern "C" {
#endif
#if(FEATURE_ON == MBB_COMMON)
#define LCD_BL_GPIO LCD_BACKLIGHT_GPIO
#define LCD_ID0_GPIO LCD_ID00_GPIO
#define LCD_ID1_GPIO LCD_ID01_GPIO
#else
#define LCD_BL_GPIO GPIO_2_26
#define LCD_ID0_GPIO GPIO_2_0
#define LCD_ID1_GPIO GPIO_2_1
#endif



#ifdef __cplusplus
}
#endif

#endif
