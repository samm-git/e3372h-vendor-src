/*************************************************************************
*   版权所有(C) 1987-2012, 深圳华为技术有限公司.
*
*   文 件 名 :  lcd.h
*
*
*   描    述 :  lcd.c的头文件
*
*
*************************************************************************/
#include <balongv7r2/types.h>
#include "product_config.h"
#include <boot/boot.h>
#include <balongv7r2/pmu.h>
#include <balongv7r2/parameters.h>
#include <boot/flash.h>
#include "bsp_nandc.h"
#include "ptable_com.h"
#include <balongv7r2/gpio.h>

#ifndef _LCD_H_
#define _LCD_H_

#define LCD_ERROR  -1
#define LCD_OK 0

#define LCD_POWEROFF LCD_ERROR
#define LCD_POWERON LCD_OK

/*系统控制器 xuc*/

#define V7R2_SC_VA_BASE      0x90000000


#ifdef CONFIG_FB_2_4_INCH_BALONG
#define LCD_ILITEK              0x0     /*天马LCD*/
#define LCD_CHIMEI              0x1     /*奇美LCD*/
#elif defined CONFIG_FB_1_4_5_INCH_BALONG
#define LCD_ILITEK              0x2     /*天马LCD*/
#define LCD_CHIMEI              0x0     /*奇美LCD*/
#elif defined CONFIG_FB_0_9_6_INCH_BALONG
#define LCD_ILITEK              0x0     /*信利OLED*/
#define LCD_CHIMEI              0x1     /*Futuba OLED*/
#endif
#ifdef CONFIG_FB_2_4_INCH_BALONG
#define  LCD_HIGHT   (320 - 1)
#define  LCD_WIDTH   (240 - 1)

#define LCD_INIT_X0 0
#define LCD_INIT_Y0 0
#define LCD_INIT_X1 127
#define LCD_INIT_Y1 63

#define CHAR_WIDTH (8)
#define CHAR_HIGHT (16)


#elif defined CONFIG_FB_1_4_5_INCH_BALONG
#define  LCD_HIGHT   (128 - 1)
#define  LCD_WIDTH   (128 - 1)

#define LCD_INIT_X0 0
#define LCD_INIT_Y0 32
#define LCD_INIT_X1 127
#define LCD_INIT_Y1 95

#define CHAR_WIDTH (8)
#define CHAR_HIGHT (16)

#elif defined CONFIG_FB_0_9_6_INCH_BALONG
#define  LCD_HIGHT   (128 - 1)
#define  LCD_WIDTH   (64 - 1)

#define LCD_INIT_X0 0
#define LCD_INIT_Y0 0
#define LCD_INIT_X1 127
#define LCD_INIT_Y1 63
#define CHAR_WIDTH (8)
#define CHAR_HIGHT (16)

#endif

#define BPP 2


#define INIT_LOGO_OFFSET    0    /*开机logo在flash分区中的位置偏移量*/
#define INIT_LOGO_SIZE     ((LCD_WIDTH + 1) * (LCD_HIGHT + 1) *2)

/*LCD命令字*/
#define COLUMN_ADDRESS_SET 0x2a
#define PAGE_ADDRESS_SET   0x2b
#define MEMORY_WRITE       0x2c


struct balong_lcd_seq {
	unsigned char		format;
	unsigned char		value;
};


#if (MBB_LCD == FEATURE_ON)
s32 lcd_warm_up(void);
#elif (MBB_OLED == FEATURE_ON)
s32 oled_warm_up(void);
#endif
#if (MBB_COMMON == FEATURE_ON)
#if (MBB_LCD == FEATURE_ON)
s32 lcd_power_on(char* string);
#elif (MBB_OLED == FEATURE_ON)
s32 oled_power_on(char* string);
#endif
#else
s32 lcd_power_on(void);
#endif
#if (MBB_OLED == FEATURE_ON)
void MainOLED_DispENoff(void);
#endif

s32 lcd_init_image(int offset);
void lcd_show_string(unsigned char * str);
void lcd_string_display(u16 ucX, u16 ucY, unsigned char *pucStr);
s32 balong_lcd_display_allblack(void);


#endif

