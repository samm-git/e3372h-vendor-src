 /*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
 * foss@huawei.com
 *
 * If distributed as part of the Linux kernel, the following license terms
 * apply:
 *
 * * This program is free software; you can redistribute it and/or modify
 * * it under the terms of the GNU General Public License version 2 and 
 * * only version 2 as published by the Free Software Foundation.
 * *
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * * GNU General Public License for more details.
 * *
 * * You should have received a copy of the GNU General Public License
 * * along with this program; if not, write to the Free Software
 * * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Otherwise, the following license terms apply:
 *
 * * Redistribution and use in source and binary forms, with or without
 * * modification, are permitted provided that the following conditions
 * * are met:
 * * 1) Redistributions of source code must retain the above copyright
 * *    notice, this list of conditions and the following disclaimer.
 * * 2) Redistributions in binary form must reproduce the above copyright
 * *    notice, this list of conditions and the following disclaimer in the
 * *    documentation and/or other materials provided with the distribution.
 * * 3) Neither the name of Huawei nor the names of its contributors may 
 * *    be used to endorse or promote products derived from this software 
 * *    without specific prior written permission.
 * 
 * * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */
#include <linux/spi/spi.h>
#include <linux/spi/spidev.h>
#include "osl_types.h"
#include <osl_sem.h>
#include <bsp_om.h>

#ifdef __cplusplus
extern "C" {
#endif


#ifndef __LCDFB_BALONG__
#define __LCDFB_BALONG__

#define LCD_SEM_FULL (1)
#define LCD_SEM_EMPTY (0)

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

#define LCD_OK 0
#define LCD_ERROR 1

#ifdef CONFIG_FB_2_4_INCH_BALONG

#define  LCD_HIGHT   (320 - 1)
#define  LCD_WIDTH   (240 - 1)



#elif defined (CONFIG_FB_1_4_5_INCH_BALONG)
#define  LCD_HIGHT   (128 - 1)
#define  LCD_WIDTH   (128 - 1)

#elif defined CONFIG_FB_0_9_6_INCH_BALONG
#define  LCD_HIGHT   (64 - 1)
#define  LCD_WIDTH   (128 - 1)

#endif


#define BPP     2


#define LCD_PWR_ON_STATE      1
#define LCD_PWR_OFF_STATE      0

/*LCD命令字*/
#define COLUMN_ADDRESS_SET 0x2a
#define PAGE_ADDRESS_SET   0x2b
#define MEMORY_WRITE       0x2c
#define SLEEP_IN           0x10
#define SLEEP_OUT          0x11
#define DISPLAY_ON           0x29
#define DISPLAY_OFF          0x28

#define FB_NUM 2

#define MAX_BACKLIGHT_INTENSITY 16
#define MAX_BACKLIGHT_BRIGNTNESS 255

/*IOCTRL命令字*/
typedef enum
{
    OLED_LIGHT = 3,
    OLED_SLEEP,
    OLED_RESET,
    OLED_CONTRAST,
    OLED_POWER_ON,
    OLED_SYS_POWER_DOWN,
    OLED_INIT_IMAGE,
    OLED_WRITE_IMAGE,
    OLED_ALL_WHITE,
    OLED_ALL_BLACK,
    OLED_CMD_MAX
}OLED_IOCTL_CMD;

enum {
    MDP_RGB_565,        /* RGB 565 planar */
    MDP_BGR_565,

    MDP_XRGB_8888,      /* RGB 888 padded */
    MDP_Y_CBCR_H2V2,    /* Y and CbCr, pseudo planar w/ Cb is in MSB */
    MDP_ARGB_8888,      /* ARGB 888 */
    MDP_RGB_888,        /* RGB 888 planar */
    MDP_Y_CRCB_H2V2,    /* Y and CrCb, pseudo planar w/ Cr is in MSB */
    MDP_YCRYCB_H2V1,    /* YCrYCb interleave */
    MDP_Y_CRCB_H2V1,    /* Y and CrCb, pseduo planar w/ Cr is in MSB */
    MDP_Y_CBCR_H2V1,    /* Y and CrCb, pseduo planar w/ Cr is in MSB */

    MDP_RGBA_8888,      /* ARGB 888 */
    MDP_BGRA_8888,      /* ABGR 888 */
    MDP_RGBX_8888,      /* RGBX 888 */
    MDP_IMGTYPE_LIMIT   /* Non valid image type after this enum */
};

typedef enum{
    GPIO_DOWN = 0,
    GPIO_UP,
}gpio_status;

struct lcd_panel_info {

    u32 xres;
    u32 yres;
    u32 pixclock;
    u32 is_3d_panel;

};

struct balong_bl {
    struct regulator *bl_vcc;
    unsigned gpio;
    int  gpio_on;
    int  intensity_cur;
    int  intensity_past;
    struct semaphore    g_backlightsemmux;
    bool flag;/*lcd未resume时不可操作背光，否则会出现白屏*/
};
struct lcd_id {
    unsigned id0_gpio;
    unsigned id1_gpio;  
};


typedef struct tag_lcd_write_data {
    u16 x_start;
    u16 y_start;
    u16 x_offset;
    u16 y_offset;
    u16 *image;
    u32 size;
}lcd_write_data;

/*LCD初始化命令序列格式结构体*/
struct balong_lcd_seq {
        u8      format;
        u8      value;
};


#define  hilcd_error(fmt, ...)    (bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_LCD, "[lcd]: <%s> "fmt"\n", __FUNCTION__, ##__VA_ARGS__))
#define  hilcd_trace(fmt, ...)    (bsp_trace(BSP_LOG_LEVEL_INFO, BSP_MODU_LCD, "[lcd]: <%s> "fmt"\n", __FUNCTION__, ##__VA_ARGS__))

#endif

#ifdef __cplusplus
}
#endif
