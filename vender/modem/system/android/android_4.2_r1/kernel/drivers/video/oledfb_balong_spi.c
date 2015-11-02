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
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/fb.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/semaphore.h>
#include <linux/interrupt.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/kthread.h>
#include <linux/platform_device.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/dma-mapping.h>
#include <asm/uaccess.h>
#include <linux/earlysuspend.h>
#include <linux/leds.h>

#include <bsp_memmap.h>
#include <hi_base.h>
#include <linux/regulator/consumer.h>
#include <hi_syssc.h>
#include <bsp_version.h>
#include <linux/video/lcdfb_balong.h>
#include "bsp_regulator.h"
#include <hi_syscrg_interface.h>
#include "oled_spi.h"


#if (CONFIG_SPI == YES)
#error  notice: 0P96 OLED had used the spi1 ,but not use the HISI spi_balong driver!!!
#endif

unsigned char PIC00[] = {
/*--  调入了一幅图像：D:\000\桌面\1.bmp  --*/
/*--  宽度x高度=128x64  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x80,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x40,
0xC0,0x40,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0xE0,0x00,0xF0,0x00,0xFC,
0x00,0x00,0x2F,0xA2,0xEF,0x00,0xC0,0x20,0xA0,0xA0,0x00,0x00,0x00,0x00,0xFE,0xC6,
0x1E,0xE6,0x1E,0xC6,0xFE,0x0A,0xFE,0xFE,0x02,0xFA,0x2A,0x2A,0x02,0xF6,0x02,0xFE,
0x00,0x00,0x98,0xC8,0xB8,0x00,0x00,0x00,0x00,0x00,0xFC,0x92,0x93,0xFF,0x92,0x92,
0xFF,0x92,0x92,0xFF,0x93,0x92,0x7C,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x7D,0x39,
0x93,0xC7,0xCF,0xDF,0xDF,0xCF,0xC7,0x93,0x3B,0x79,0xFE,0x00,0x00,0x00,0x00,0x00,
0x00,0x78,0x78,0xFF,0x03,0xFD,0xFD,0x01,0xFD,0xFD,0x01,0xFD,0xFD,0x01,0xFD,0xFD,
0x03,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,
0x00,0x00,0x02,0x02,0x03,0x00,0x01,0x02,0x02,0x03,0x00,0x00,0x00,0x00,0x01,0x01,
0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x03,0x06,0x04,
0x07,0x04,0x06,0x03,0x02,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x03,
0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x02,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x03,0x03,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,
0x03,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x80,0xC0,0x60,0x20,0x20,0x60,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x1F,0x3F,0x60,0x40,0x40,0x60,0x3F,0x1F,0x00,0x00,0xFF,0xFF,0x22,0x41,0x7F,
0x3E,0x00,0x00,0x3E,0x7F,0x49,0x49,0x6F,0x2E,0x00,0x00,0x7F,0x7F,0x01,0x01,0x00,
0x32,0x79,0x4D,0x45,0x7F,0x7E,0x00,0x01,0x3F,0x7F,0x41,0x00,0x3E,0x7F,0x41,0x41,
0x7F,0x3E,0x00,0x00,0x7F,0x7F,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x7F,0x7F,0x02,
0x01,0x7F,0x7E,0x00,0x00,0x32,0x79,0x4D,0x45,0x7F,0x7E,0x00,0x00,0x7F,0x7F,0x02,
0x01,0x7F,0x7F,0x02,0x01,0x7F,0x7E,0x00,0x00,0x3E,0x7F,0x49,0x49,0x6F,0x2E,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x83,0x83,0x80,0x00,0x00,
0x00,0x80,0x80,0x80,0x00,0x00,0x00,0x80,0x80,0x80,0x00,0x00,0x00,0x80,0x00,0x00,
0x00,0x00,0x00,0x80,0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x00,0x80,0x00,
0x00,0x00,0x80,0x00,0x00,0x00,0x80,0x80,0x80,0x00,0x00,0x80,0x00,0x00,0x00,0x00,
0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x80,0x80,0x80,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x13,0x24,0x24,0x24,0x1F,0x00,
0x21,0x30,0x28,0x24,0x23,0x00,0x1F,0x20,0x20,0x20,0x1F,0x00,0x00,0x3F,0x03,0x0C,
0x30,0x0C,0x03,0x3F,0x00,0x00,0x3F,0x24,0x24,0x24,0x1B,0x00,0x30,0x0E,0x01,0x00,
0x02,0x01,0x3F,0x00,0x00,0x1F,0x20,0x20,0x20,0x1F,0x00,0x3F,0x02,0x02,0x02,0x3C,
0x00,0x16,0x23,0x22,0x22,0x1C,0x00,0x21,0x30,0x28,0x24,0x23,0x00,0x00,0x3E,0x02,
0x02,0x3C,0x02,0x02,0x3C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

/************************************************************
*******************宏定义********************************
*************************************************************/
#define SPIDEV_MAJOR            150/* SPI从设备主设备号 */
#define N_SPI_MINORS            32/* SPI从设备次设备号 */
/*定义一个数组，数组的维数通过N_SPI_MINORS计算得到*/
static DECLARE_BITMAP(minors, N_SPI_MINORS);
#define IMAGEBUFFER (LCD_WIDTH + 1)* (LCD_HIGHT + 1) /8
#define LCD_ID0 OLED_ID0_GPIO
#define LCD_ID1 OLED_ID1_GPIO
#define LCD_CD  OLED_CD_GPIO  /* SPI 传输数据命令连线 */

struct stlcdfb_par {
    struct platform_device *pdev;
    struct fb_info *info;
    u32 *vmem;
    struct spi_driver spidrv;
    struct regulator *dr1_vcc;
    struct regulator *lcd_vcc;

    u32 g_ulLcdPwrOnState;
    struct semaphore    g_screensemmux;

    #ifdef CONFIG_HAS_EARLYSUSPEND
        struct early_suspend earlysus_lcd;
    #endif
    u32 fb_imgType;
    u32 hw_refresh;
    u32 var_pixclock;
    u32 var_xres;
    u32 var_yres;
    u32 fb_page;
    struct lcd_panel_info panel_info;
    int ref_cnt;
    int lcd_backlight_registered;
    struct balong_bl bl;
    struct led_classdev led_cdev;
};

struct v7r2_lcd{
    struct class *lcd_class;
    struct lcd_spidev_data* g_lcd;
};

/*初始化设备链表*/
static LIST_HEAD(device_list);
/*初始化互斥信号量*/
static DEFINE_MUTEX(device_list_lock);

#define START_PAGE 0xB0
#define END_PAGE   0xB7
/* LCD使用SPI1 CS0 */
#define SPI_NO_LCD          0
#define SPI_CS_LCD          0     /* CS0,for LCD */

#ifdef CONFIG_FB_0_9_6_INCH_BALONG
struct regulator *dr1_vcc;
struct balong_lcd_seq initseq[] =
{
        {'c',0xAE}, /*唤醒LCD*/
        {'c',0X33},

        {'c',0x40},/*设置像素格式*/
        {'c',0x81},  /*65K  OR 262k selectiong  16bit control*/
        {'c',0x0c},
        {'c',0xA1},
        {'c',0xA4},
        {'c',0xA6},
        {'c',0xA8},
        {'c',0x3F},
        {'c',0xAD},
        {'c',0x8B},
        {'w', 100},
        {'c',0xC8},
        {'c',0xD3},
        {'c',0x00},
        {'c',0xD5},
        {'c',0x80},
        {'c',0xD9},
        {'c',0x1F},
        {'c',0xDA},
        {'c',0x12},
        {'c',0xDB},
        {'c',0x40},
        {'c',0xAF}, 
};
struct balong_lcd_seq initseq_futaba[] =
{
        {'c',0xAE}, /*Display OFF*/
        {'c',0xAD}, /*iNTERNAL IREF setting*/
        {'c',0x10}, 
        {'c',0x81}, /*Set Contrast Control*/
        {'c',0x40},
        {'c',0xD5}, /*Set Display Clock Drive Ratio , Oscillator Frequency*/
        {'c',0x40},
        {'c',0xD9}, /*Pre-charge Period*/
        {'c',0x24},
        {'c',0xDB}, /*VCOMH Dselect Level*/
        {'c',0x20}, /*0.77*VCC(REST)*/
        {'c',0xA8}, /*Multiplex Ratio = 63+1*/
        {'c',0x3F}, /*(63+1)3F:RESET*/
        {'c',0xA4}, /*Entire Display Off : Resume to RAM content display*/
        {'c',0xC8}, /*COM Output Scan Direction : from COM0 to COM(N-1)*/
        {'c',0xA1}, /*Set Segment Remap(column address127>>0)*/
        {'c',0x20}, /*Memory Addressing Mode: Page Addressing Mode*/
        {'c',0x00}, /*Horizontal addressing mode*/
        {'c',0x21}, /*Setup column start and end address*/
        {'c',0x00},
        {'c',0x7F},
        {'c',0x22}, /*Setup page start and end address*/
        {'c',0x00},
        {'c',0x07},        
        {'c',0x20}, /*Memory Addressing Mode: Page Addressing Mode*/
        {'c',0x10}, /*page addressing mode*/      
        {'c',0x8D}, /*Charge Pump Setting*/
        {'c',0x95}, /*A[2]=0b:Disable,1b:Enable Charge Pump ON A[7]A[0]:11 9.0V*/
        {'c',0xAF}, /*Display ON*/
};
#endif

/************************************************************
*******************全局变量********************************
*************************************************************/
struct v7r2_lcd lcd;
int oled_suspend = 0;/*屏灭时 suspend 标志 */
struct fb_info *g_fbinfo;
const u32 spiBase[2] = {ECS_SPI0_BASE, ECS_SPI1_BASE};
static int oled_MacID = 0;

/************************************************************
*******************函数声明********************************
*************************************************************/

extern void lcd_reset(void);
extern void MainOLED_DispENon(void);
extern void MainOLED_DispENoff(void);

/************************************************************
*************************************************************/

 int balong_bl_set_intensity(struct led_classdev *led_cdev,enum led_brightness value)
{
    if(0 == value)
    {
        hilcd_trace("brightness is 0,bl will be closed!\n");
        MainOLED_DispENoff();
    }
    else 
    {
        hilcd_trace("brightness is 0,bl will be open!\n");
        MainOLED_DispENon();
    }

    return LCD_OK;
}

struct fb_info g_fbinfo_get(void)
{
    return (struct fb_info )(*g_fbinfo);
}

void spi_clk_enable(u32 spiNo)
{
    printk(KERN_INFO "PS:spi_clk_enable\n");
    if (spiNo)
    {
        hi_syscrg_spi1_clk_enable();/*lint !e628*/
    }
    else
    {
        hi_syscrg_spi0_clk_enable();/*lint !e628*/
    }
    
}

void spi_clk_disable(u32 spiNo)
{
    printk(KERN_INFO "PS:spi_clk_disable \n");
    if (spiNo)
    {
        hi_syscrg_spi1_clk_disable();/*lint !e628*/
    }
    else
    {
        hi_syscrg_spi0_clk_disable();/*lint !e628*/
    }
    
}

static void wait_till_not_busy(u32 spiNo)
{
    unsigned long end = jiffies + 1 + usecs_to_jiffies(1000);

    while (time_before(jiffies, end)) {
        if (!(readl(SPI_STATUS(spiNo))& SPI_STATUS_BUSY))
            return;
    }
    printk(KERN_INFO "oled SPI: Status keeps busy for 1000us after a read/write!\n");
}

void spi_suspend(u32 spiNo)
{
    writel(0,SPI_BAUD(spiNo));
    /* 禁止SPI Slave*/
    writel(0,SPI_SLAVE_EN(spiNo));
    /* 禁止SPI Master*/
    writel(0,SPI_EN(spiNo));
    /*关spi时钟*/
    spi_clk_disable(SPI_NO_LCD);
}

void spi_resume(u32 spiNo)
{
    u32 reg;
    /*开spi时钟*/
    spi_clk_enable(SPI_NO_LCD);
    /*三线模式*/
    reg = readl(HI_SYSCTRL_BASE_ADDR_VIRT + 0x040C);
    reg &= ~0x80;
    writel(reg,HI_SYSCTRL_BASE_ADDR_VIRT + 0x040C);
    mdelay(MDELAY_10_MS);/*lint !e506*/

    /* 禁止SPI Slave*/
    writel(0,SPI_SLAVE_EN(spiNo));
    /* 禁止SPI Master*/
    writel(0,SPI_EN(spiNo));
    /*8bit数据，相位1 1*/
    writel((0x07 << SPI_CTRL0_DFS_BITPOS)    \
            | (1 << SPI_CTRL0_SCPH_BITPOS)    \
            | (1 << SPI_CTRL0_SCPOL_BITPOS)   \
            | (0 << SPI_CTRL0_FRF_BITPOS)   \
            | (0x00 << SPI_CFS_BITPOS),SPI_CTRL0(spiNo));
         /* 配置SPI波特率为SSI CLK的1/24，即48/24=2MHz */
    writel(24,SPI_BAUD(spiNo));


    /*禁止所有中断信号*/
    writel(0,SPI_IMR(spiNo));
    /*屏蔽DMA传输*/
    writel(0, SPI_DMAC(spiNo));

    /* 禁止SPI Slave*/
    writel(0,SPI_SLAVE_EN(spiNo));
    /* 禁止SPI Master*/
    writel(1,SPI_EN(spiNo));

    while(readl(SPI_STATUS(spiNo)) & SPI_STATUS_RXNOTEMPTY)
    {
        readl(SPI_DR(spiNo));
    }
    wait_till_not_busy(spiNo);
}

/*lint -e160*/
s32 spi_init(u32 spiNo)
{
    u32 reg;
    if ((0 != spiNo) && (1 != spiNo))
    {
        return ERROR;
    }

    printk(KERN_ERR "spp:spi_init\n");
    /*打开clk*/
    spi_clk_enable(SPI_NO_LCD);

    /*SPI0三线模式*/
    reg = readl(HI_SYSCTRL_BASE_ADDR_VIRT + 0x040C);
    reg &= ~0x80;
    writel(reg,HI_SYSCTRL_BASE_ADDR_VIRT + 0x040C);
    mdelay(MDELAY_10_MS);/*lint !e506*/

    /* 禁止SPI Slave*/
    writel(0,SPI_SLAVE_EN(spiNo));
    /* 禁止SPI Master*/
    writel(0,SPI_EN(spiNo));

    /* 配置ctrl0寄存器，命令字长度为1，数据宽度为8,上升沿触发,低电平有效 */
    writel((0x07 << SPI_CTRL0_DFS_BITPOS)    \
            | (1 << SPI_CTRL0_SCPH_BITPOS)    \
            | (1 << SPI_CTRL0_SCPOL_BITPOS)   \
            | (0 << SPI_CTRL0_FRF_BITPOS)   \
            | (0x00 << SPI_CFS_BITPOS),SPI_CTRL0(spiNo));
    /* 配置SPI波特率为SSI CLK的1/24，即48/24=2MHz */
    writel(24,SPI_BAUD(spiNo));

    /*禁止所有中断信号*/
    writel(0,SPI_IMR(spiNo));
    /*屏蔽DMA传输*/
    writel(0, SPI_DMAC(spiNo));
    return OK;
}

s32 spi_send (u32 spiNo, u32 cs, u16* pData, u32 ulLen)
{
    u16 *pSh;
    u32  i;
    u32 ulLoop = SPI_MAX_DELAY_TIMES;
    u32 ulVal;

    if (((0 != spiNo) && (1 != spiNo))
        || (NULL == pData) || (0 == ulLen))
    {
        return ERROR;
    }

    pSh = (u16*)pData;
    /* 禁止SPI Slave*/
    writel(0, SPI_SLAVE_EN(spiNo));
    /* 禁止SPI Master*/
    writel(0, SPI_EN(spiNo));
    /* 设置成发送模式 */
    writel(((readl(SPI_CTRL0(spiNo)) & ~SPI_CTRL0_TMOD_BITMASK) | SPI_CTRL0_TMOD_SEND),SPI_CTRL0(spiNo));
    /*使能SPI Master*/
    writel(1, SPI_EN(spiNo));
    /*使能SPI Slave*/
    writel((1<<cs), SPI_SLAVE_EN(spiNo));

    /* 发送命令 */
    for(i = 0; i < ulLen; i++)
    {
        /* 等待发送FIFO非满 */
        while(!(readl(SPI_STATUS(spiNo)) & SPI_STATUS_TXNOTFULL)
            && (0 != --ulLoop))
        {
        }

        if(0 == ulLoop)
        {
            return ERROR;
        }
        writel(*pSh++, SPI_DR(spiNo));
    }

    /*将发送FIFO中的数据全部发出,且不BUSY*/
    ulLoop = SPI_MAX_DELAY_TIMES;
    ulVal = readl(SPI_STATUS(spiNo));
    while(((!(ulVal & SPI_STATUS_TXEMPTY)) || (ulVal & SPI_STATUS_BUSY))
        && (0 != --ulLoop))
    {
        ulVal = readl(SPI_STATUS(spiNo));
    }

    if(0 == ulLoop)
    {
        return ERROR;
    }

    return OK;
}
/*lint +e160*/

static s32 spi_write_data(const unsigned char *buf, unsigned int len)
{
    u32 count = 0;
    u16 data;

    if((NULL == buf) || (0 == len))
    {
        return LCD_ERROR;
    }
    gpio_direction_output(LCD_CD, 1);
    for(count = 0;(unsigned int)count < len; count++)
    {
        data = buf[count]; /* 数据 */
        spi_send(SPI_NO_LCD,SPI_CS_LCD,&data,1);
    }
    return LCD_OK;
}

static s32 spi_write_cmd(const unsigned char *buf, unsigned int len)
{
    u32 count = 0;
    u16 cmd;

    if((NULL == buf) || (0 == len))
    {
        return LCD_ERROR;
    }
    gpio_direction_output(LCD_CD, 0);
    for(count = 0;(unsigned int)count < len; count++)
    {
        cmd = buf[count]; /* 命令 */
        spi_send(SPI_NO_LCD,SPI_CS_LCD,&cmd,1);


    }
    return LCD_OK;

}/*lint !e550*/

int balong_lcd_toggle(struct balong_lcd_seq *seq, int sz)
{
    s32 ret = 0;
    u32 count;
    u8 buf[1];

    for (count = 0; count < sz; count++)
    {
        buf[0] = seq[count].value;
        if(seq[count].format == 'c')
        {
            ret = spi_write_cmd(buf, 1);  /* command */
            if(ret)
            {
                hilcd_error("spi write command error!");
                return ret;
            }
        }
        else if(seq[count].format == 'd')
        {
            ret = spi_write_data(buf, 1); /* data */
            if(ret)
            {
                hilcd_error("spi write data error!");
                return ret;
            }
        }
        else if(seq[count].format == 'w')
        {
            mdelay(seq[count].value); /* delay */
        }
    }
    return LCD_OK;
}

s32 lcd_vectid_get(u16 *pu16VectId)
{
        u32 u32Data1 = 0;
        u32 u32Data2 = 0;
        static bool is_init = BSP_FALSE;

        if(!pu16VectId)
        {
            hilcd_error("invalid parameter\n");
            return LCD_ERROR;
        }

        if(!is_init)
        {
            if(gpio_request(LCD_ID0, "LCD_ID0"))
            {
                hilcd_error("Gpio is busy!!! \n");
            }
            /* 设置GPIO为输入 */
            (void)gpio_direction_input(LCD_ID0);

            if(gpio_request(LCD_ID1, "LCD_ID1"))
            {
                hilcd_error("Gpio is busy!!! \n");
            }
            (void)gpio_direction_input(LCD_ID1);
            is_init = BSP_TRUE;
        }

        /* 获取输入值 */
        u32Data1 = gpio_get_value(LCD_ID0);
        u32Data2 = gpio_get_value(LCD_ID1);

    hilcd_trace("LCD_ID0 %x,LCD_ID1 %x\n",u32Data1,u32Data2);
    /* 获取 LCD 厂家ID*/
    *pu16VectId = (unsigned short)((1 & u32Data1 << 0)\
                     | ((1 & u32Data2) << 1));
    hilcd_error("\r\nlcd ID=%x\n",*pu16VectId);
    return LCD_OK;
}

void lcd_init_byid(u32 ucId)
{
    int ret = 0;
    if(LCD_ILITEK == ucId)
    {
        ret = balong_lcd_toggle(initseq, ARRAY_SIZE(initseq));
        if(ret)
        {
            hilcd_error("lcd init error\n");
        }
        return;
    }
    else if(LCD_CHIMEI == ucId)
    {
        hilcd_error("lcd FUTABA init\n");
        ret = balong_lcd_toggle(initseq_futaba, ARRAY_SIZE(initseq_futaba));
        if(ret)
        {
            hilcd_error("lcd init error\n");
        }
        return;
    }
    else
    {
        hilcd_error("invalid lcd vector id:%d\n", ucId);
        return;
    }

}

/*lint -e64 -e550 -e569*/
void MainOLED_DispEntireon(void)
{
      s32 rt = 0;
      char data1 = 0xA5;
      printk(KERN_INFO "invalid lcd MainOLED_DispENon\n");
      rt = spi_write_cmd(&data1,1); 
}
void MainOLED_DispEntireoff(void)
{ 
      s32 rt = 0;
      char data1 = 0xA4;
      printk(KERN_INFO "invalid lcd MainOLED_DispENoff\n");
      rt = spi_write_cmd(&data1,1); 
}

void MainOLED_DispENon(void)
{
      s32 rt = 0;
      char data1 = 0xAF;
      printk(KERN_INFO "invalid lcd MainOLED_DispENon\n");
      rt = spi_write_cmd(&data1,1); 
}
void MainOLED_DispENoff(void)
{
      s32 rt = 0;
      char data1 = 0xAE;
      printk(KERN_INFO "invalid lcd MainOLED_DispENoff\n");
      rt = spi_write_cmd(&data1,1); 

}

/*lint +e64 +e550 +e569*/
/*****************************************************************************
* 函 数 名  : Dis_ADDRESS
*
* 功能描述  :  0.96 OLED 初始化
*
* 输入参数  :
*
* 输出参数  : 无
*
* 返 回 值  :
*****************************************************************************/
void Dis_ADDRESS(void)
{  
    char  LOW_COL,HIGH_COL;
    if(LCD_CHIMEI == oled_MacID) /*futaba OLED*/
    {
        LOW_COL = 0x00;
        HIGH_COL = 0x10;
    }
    else /*信利 OLED*/
    {
        LOW_COL = 0x02;
        HIGH_COL = 0x10;		
    }
    spi_write_cmd(&HIGH_COL,1);        //SET column low address
    spi_write_cmd(&LOW_COL,1);        //SET column higher address
}

/*****************************************************************************
* 函 数 名  : MainOLED_DispAllon
*
* 功能描述  :  LCD 显示全亮
*
* 输入参数  :
*
* 输出参数  : 无
*
* 返 回 值  :
*****************************************************************************/
void MainOLED_DispAllon(void)
{
    int i,j;
    char data1 = 0xff;
    hilcd_error("lcd MainOLED_DispAllon\n");
      
    for(i = START_PAGE;i < (END_PAGE + 1);i++)
    {   
        spi_write_cmd(&i,1);
        Dis_ADDRESS();
        for(j = 0;j < 128;j++)
        {
           spi_write_data(&data1,1); 
        }
    }   
}

void MainOLED_DispAlloff(void)
{
    int i,j;
    char data1 = 0x00;

    hilcd_error("lcd MainOLED_DispAlloff\n");
    for(i = START_PAGE;i < (END_PAGE + 1);i++)
    {   
        spi_write_cmd(&i,1);
        Dis_ADDRESS();
        for(j = 0;j < 128;j++)
        {
            spi_write_data(&data1,1); 
        }
    }  
}

void MainOLED_DispAll(char data)
{
    int i,j;
    char data1 = data;

    hilcd_error("lcd MainOLED_DispAll\n");
    for(i = START_PAGE;i < (END_PAGE + 1);i++)
    {   
        spi_write_cmd(&i,1);
        Dis_ADDRESS();
        for(j = 0;j < 128;j++)
        {
           spi_write_data(&data1,1); 
        }
    }       
}

void MainOLED_Xpoint(void)
{
    int i,j,k;
    unsigned char text01[] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
    for(i = START_PAGE;i < (END_PAGE + 1);i++)
    {  
        spi_write_cmd(&i,1);
        Dis_ADDRESS();
        for(j = 0;j < 16;j++)
        {
            for(k = 0;k < 8;k++) 
            {
                spi_write_data(&text01[k],1); 
            }
        }
    }
}
void MainOLED_Disppic(char PIC [ ])
{
    int i,j,k;
    unsigned char pixel1; 
    k = 0;
    for(i = START_PAGE;i < (END_PAGE + 1);i++)
    {
        spi_write_cmd(&i,1);
        Dis_ADDRESS();
        for(j = 0;j < 128;j++)
        {
            spi_write_data(&PIC[k++],1); 
        }
    }
}

void do_convert(unsigned char* indata, unsigned char* outdata)
{
    int h_idx = 0, v_idx = 0, bit = 0;
    unsigned char* pindata = indata;
    unsigned char* poutdata = outdata;
    
    for (h_idx = 0; h_idx < 16; h_idx++)
    {
        *poutdata = 0;
        for (v_idx = 7; v_idx >= 0; v_idx--)
        {
            for (bit = 7; bit >= 0; bit--)
            {
                *poutdata |= (*(pindata + 16 * bit) & (1 << v_idx)) >> v_idx;
                if (bit != 0)
                {
                    *poutdata <<= 1;
                }
            }
            poutdata++;
        }
        pindata++;
    }
    
}

/*将整帧的LCD数据由横向取模转换为纵向取模*/
/*len是指输入数据的长度，单位字节。目前只支持整页数据转换，即len必须为128字节的
  整数倍，否则出错。*/
/*！！此处outdata空间有调用者保证，至少为len，否则会发生内存越界！！*/
void lcd_data_format_convert(unsigned char* indata, unsigned char* outdata, int len)
{
    /*思路是，每128字节一转；如果不是128字节的整数倍，则无法转换！！*/
    int i = 0;

    if (len % 128)
    {
        return;
    }
    
    for (i = 0; i < len / 128; i++)
    {      
        do_convert(indata + 128 * i, outdata + 128 * i);
    } 
}

/*****************************************************************************
* 函 数 名  : lcdInit
*
* 功能描述  :  LCD 初始化
*
* 输入参数  :
*
* 输出参数  : 无
*
* 返 回 值  :
*****************************************************************************/
void lcd_init(void)
{
    u16 ucId = 0;
    s32 retval = 0;

    retval = lcd_vectid_get(&ucId);
    if(retval)
    {
        return;
    }

    oled_MacID = ucId;
    printk(KERN_ERR "lcd_init:oled_MacID = %d \n");

    lcd_init_byid(ucId);
    MainOLED_DispAlloff();
}

 void lcd_pwron(struct fb_info *info)
{
    struct stlcdfb_par *par = info->par;
    /*已是上电状态，无需重新上电*/
    if (LCD_PWR_ON_STATE == par->g_ulLcdPwrOnState)
    {
        /*printk("lcd has been powered on.\n");*/
        return ;
    }
    msleep(110);/*100ms以上*/

    lcd_init();
    par->g_ulLcdPwrOnState = LCD_PWR_ON_STATE;
    return ;
}

unsigned char rbuf[IMAGEBUFFER];

void print_rbuf(int index)
{
    printk("0x%x  \n",rbuf[index]);
}

s32 lcd_refresh(struct fb_info *info)
{
    struct stlcdfb_par *par = info->par;
    struct fb_var_screeninfo var = info->var;
    struct fb_fix_screeninfo fix = info->fix;
    u8 *buf = info->screen_base + var.yoffset * fix.line_length +var.xoffset ; /* 获取用户的数据  */
    u32 len = (fix.line_length) * (var.yres); /*  16*64 = 1024 */
    s32 ret = 0;

    memset(rbuf,0x00,len);
    lcd_data_format_convert(buf,rbuf,IMAGEBUFFER);

    if(oled_suspend == 1)
    {
        printk(KERN_INFO "spp :oled spi still suspend \n");
        return;
    }

    MainOLED_Disppic(rbuf);/*image*/

    return LCD_OK;
}


 s32 lcd_light(struct fb_info *info)
{
    struct stlcdfb_par *par = info->par;
    s32 ret = 0;
    u8 buf[1];

    osl_sem_down(&(par->g_screensemmux));
    buf[0] = 0xAF;
    ret = spi_write_cmd(buf, 1);
    if(ret)
    {
        hilcd_error("spi write command error!");
        return ret;
    }
    osl_sem_up(&(par->g_screensemmux));
    return LCD_OK;
}


 s32 lcd_sleep(struct fb_info *info)
{
    s32 ret = 0;
    u8 buf[1];
    buf[0] = 0xAE;
    struct stlcdfb_par *par = (struct stlcdfb_par *)info->par;

    osl_sem_down(&(par->g_screensemmux));
    ret = spi_write_cmd(buf, 1);
    if(ret)
    {
        hilcd_error("spi write command error!");
        return ret;
    }
    osl_sem_up(&(par->g_screensemmux));
    return LCD_OK;
}

s32 lcd_pwroff(struct fb_info *info)
{
    s32 ret = 0;
    struct stlcdfb_par *par = info->par;
    /*已是下电状态，无需重新下电*/
    if (LCD_PWR_OFF_STATE == par->g_ulLcdPwrOnState)
    {
        hilcd_error("lcd has been powered off");
        return LCD_OK;
    }
    /*关闭显示*/
    ret = lcd_sleep(info);
    if(ret)
    {
        hilcd_error("lcd sleepin error");
        return LCD_ERROR;
    }
    par->g_ulLcdPwrOnState = LCD_PWR_OFF_STATE;
    return LCD_OK;
}
 s32 stlcdfb_blank(int blank_mode,struct fb_info *fbinfo)
{
   /* struct platform_device *pdev = to_platform_device(fbinfo);*/
    switch (blank_mode)
    {
        case VESA_NO_BLANKING:/*lcd power 1;backlight power1*/
        (void)lcd_light(fbinfo);
        break;
        case VESA_VSYNC_SUSPEND:/*lcd on;backlight off/sleepin*/
        (void)lcd_sleep(fbinfo);
        break;
        case VESA_HSYNC_SUSPEND:
        case VESA_POWERDOWN:/*lcd off;backlight:off*/
        (void)lcd_pwroff(fbinfo);
        break;
        default:
        break;

    }
    return 0;
}


static u32 stlcd_fb_line_length(u32 fb_index, u32 xres, u32 bpp)
{   /* The adreno GPU hardware requires that the pitch be aligned to
32 pixels for color buffers, so for the cases where the GPU
is writing directly to fb0, the framebuffer pitch
also needs to be 32 pixel aligned */
    /*if (fb_index == 0)
       return ALIGN(xres, 32) * bpp;
    else  */
        return xres * bpp;
}

int stlcdfb_check_var(struct fb_var_screeninfo *var,
            struct fb_info *info)
{
    u32 len;
    if(var->rotate != FB_ROTATE_UR)
        return -EINVAL;
    if(var->grayscale != info->var.grayscale)
        return -EINVAL;
    switch(var->bits_per_pixel){
    case 16:
        if((var->green.offset != 5) ||
            !((var->blue.offset == 11) || (var->blue.offset == 0)) ||
            !((var->red.offset == 11) || (var->red.offset == 0)) ||/*suport RGB or BGR*/
            (var->blue.length != 5) ||
            (var->green.length != 6) ||
            (var->red.length != 5) ||/*support 565*/
            (var->blue.msb_right != 0) ||
            (var->green.msb_right != 0) ||
            (var->red.msb_right != 0) ||
            (var->transp.length != 0) ||
            (var->transp.length != 0))
            return -EINVAL;
        break;
    case 24:
        if ((var->blue.offset != 0) ||
            (var->green.offset != 8) ||
            (var->red.offset != 16) ||
            (var->blue.length != 8) ||
            (var->green.length != 8) ||
            (var->red.length != 8) ||
            (var->blue.msb_right != 0) ||
            (var->green.msb_right != 0) ||
            (var->red.msb_right != 0) ||
            !(((var->transp.offset == 0) &&
            (var->transp.length == 0)) ||
            ((var->transp.offset == 24) &&
            (var->transp.length == 8))))
            return -EINVAL;
        break;
    case 32:
        /* Figure out if the user meant RGBA or ARGB
        and verify the position of the RGB components */
        if (var->transp.offset == 24) {
            if ((var->blue.offset != 0) ||
                (var->green.offset != 8) ||
                (var->red.offset != 16))
                return -EINVAL;
            } else if (var->transp.offset == 0) {
            if ((var->blue.offset != 8) ||
                (var->green.offset != 16) ||
                (var->red.offset != 24))
                return -EINVAL;
            } else
            return -EINVAL;
            /* Check the common values for both RGBA and ARGB */
            if ((var->blue.length != 8) ||
                (var->green.length != 8) ||
                (var->red.length != 8) ||
                (var->transp.length != 8) ||
                (var->blue.msb_right != 0) ||
                (var->green.msb_right != 0) ||
                (var->red.msb_right != 0))
                return -EINVAL;
            break;
    default:
            return -EINVAL;

    }
    if ((var->xres_virtual <= 0) || (var->yres_virtual <= 0))
        return -EINVAL;
    len = var->xres_virtual * var->yres_virtual * (var->bits_per_pixel / 8);
    if (len > info->fix.smem_len)
        return -EINVAL;
    if ((var->xres == 0) || (var->yres == 0))
        return -EINVAL;
    /*if ((var->xres > mfd->panel_info.xres) ||
        (var->yres > mfd->panel_info.yres))
        return -EINVAL;  */
    if (var->xoffset > (var->xres_virtual - var->xres))
        return -EINVAL;
    if (var->yoffset > (var->yres_virtual - var->yres))
        return -EINVAL;
    return 0;
}

int stlcdfb_set_par(struct fb_info *info)
{
    struct stlcdfb_par *par = (struct stlcdfb_par *)(info->par);
    struct fb_var_screeninfo *var = &info->var;
    int old_imgType;
    int blank = 0;
    old_imgType = par->fb_imgType;
    switch (var->bits_per_pixel) {
        case 16:
            if (var->red.offset == 0)
                par->fb_imgType = MDP_BGR_565;
            else
                par->fb_imgType = MDP_RGB_565;
            break;
            case 24:
                if ((var->transp.offset == 0) && (var->transp.length == 0))
                    par->fb_imgType = MDP_RGB_888;
                else if ((var->transp.offset == 24) &&
                    (var->transp.length == 8)) {
                    par->fb_imgType = MDP_ARGB_8888;
                    info->var.bits_per_pixel = 32;
                    }
                break;
        case 32:
            if (var->transp.offset == 24)
                par->fb_imgType = MDP_ARGB_8888;
            else
                par->fb_imgType = MDP_RGBA_8888;
            break;
            default:
                return -EINVAL;
        }
    if ((par->var_pixclock != var->pixclock) ||
        (par->hw_refresh && ((par->fb_imgType != old_imgType) ||
        (par->var_pixclock != var->pixclock) ||
        (par->var_xres != var->xres) ||
        (par->var_yres != var->yres)))) {
            par->var_xres = var->xres;
            par->var_yres = var->yres;
            par->var_pixclock = var->pixclock;
            blank = 1;
            }
    par->info->fix.line_length = stlcd_fb_line_length(info->node, var->xres,
        var->bits_per_pixel / 8);
    if (blank) {
        stlcdfb_blank(VESA_POWERDOWN, info);
        lcd_pwron(info);
        }
    return 0;
}

int stlcdfb_pan_display( struct fb_var_screeninfo *var, struct fb_info *info)
{

    struct stlcdfb_par *par = info->par;
    unsigned long flags;
    static int is_first_dispaly = 1;
          
    if (var->xoffset > (info->var.xres_virtual - info->var.xres))
        return -EINVAL;
    if (var->yoffset > (info->var.yres_virtual - info->var.yres))
        return -EINVAL;
    if (info->fix.xpanstep)
        info->var.xoffset =
        (var->xoffset / info->fix.xpanstep) * info->fix.xpanstep;

    if (info->fix.ypanstep)
        info->var.yoffset =
        (var->yoffset / info->fix.ypanstep) * info->fix.ypanstep;

    else
        hilcd_trace("dma function not set for panel type=%d\n") ;

    if(1 == is_first_dispaly)
    {
        /*kernel 刷第一图片是初始化下oled，区别不同的厂家*/
        MainOLED_DispENoff();
        lcd_reset();
        lcd_pwron(info);
        MainOLED_DispENon();
        is_first_dispaly = 0;
    }

    osl_sem_down(&(par->g_screensemmux));
    local_irq_save(flags);
    lcd_refresh(info);
    local_irq_restore(flags);
    osl_sem_up(&(par->g_screensemmux));

    return 0;
}

int stlcdfb_open(struct fb_info *info,int user)
{
    struct stlcdfb_par *par = (struct stlcdfb_par *)info->par;
    int ret = 0;
    if(!par->ref_cnt){
            hilcd_trace("first open fb %d\n",info->node);
        }

    par->ref_cnt++;
    return LCD_OK;
}


int stlcdfb_release(struct fb_info *info,int user)
{
    struct stlcdfb_par *par = (struct stlcdfb_par *)info->par;
    int ret = 0;

    if(!par->ref_cnt){
        hilcd_error("try to close unopened fb %d\n",info->node);
        return -EINVAL;
        }
    par->ref_cnt--;
    if(par->ref_cnt){
         hilcd_trace("last close fb %d\n",info->node);
        }

    return LCD_OK;
}


static struct fb_ops stlcdfb_ops = {
    .owner   = THIS_MODULE,
    .fb_open = stlcdfb_open,
    .fb_release = stlcdfb_release,
    .fb_blank   = stlcdfb_blank,
    .fb_check_var = stlcdfb_check_var,
    .fb_set_par = stlcdfb_set_par,
    .fb_pan_display = stlcdfb_pan_display,
    /*.fb_fillrect = cfb_fillrect,*/
    /*.fb_copyarea = cfb_copyarea,*/
    /*.fb_imageblit = cfb_imageblit,*/

};


void lcd_reset(void)
{
    set_hi_sc_ctrl3_lcd_rst_n(1);
    mdelay(10);
    set_hi_sc_ctrl3_lcd_rst_n(0);
    mdelay(10);
    set_hi_sc_ctrl3_lcd_rst_n(1);
    mdelay(50);
}

#ifdef CONFIG_HAS_EARLYSUSPEND
void lcd_suspend_early(struct early_suspend *h)
{
    printk(KERN_INFO "lcd_suspend_early\r\n");
/*P711上EUAP通过brightness亮度节点控制OLED亮灭。启动时释放main lock此earlysuspend操作不操作*/
    //lcd_sleep(g_fbinfo);

}
void lcd_resume_late(struct early_suspend *h)
{
    printk(KERN_INFO "lcd_resume_late\r\n");
/*P711上EUAP通过brightness亮度节点控制OLED亮灭。启动时释放main lock此earlysuspend操作不操作*/
   // lcd_light(g_fbinfo);
}
#endif

s32 stlcdfb_fill(struct fb_info *info,struct stlcdfb_par *par)
{
    struct fb_var_screeninfo *var = (struct fb_var_screeninfo *)&info->var;

    switch (par->fb_imgType) {
        case MDP_RGB_565:
            info->fix.type = FB_TYPE_PACKED_PIXELS;
            info->fix.xpanstep = 1;
            info->fix.ypanstep = 1;
            var->vmode = FB_VMODE_NONINTERLACED;
            var->blue.offset = 0;
            var->green.offset = 5;
            var->red.offset = 11;
            var->blue.length = 5;
            var->green.length = 6;
            var->red.length = 5;
            var->blue.msb_right = 0;
            var->green.msb_right = 0;
            var->red.msb_right = 0;
            var->transp.offset = 0;
            var->transp.length = 0;
            var->bits_per_pixel = 1;
        break;
        case MDP_RGB_888:
            info->fix.type = FB_TYPE_PACKED_PIXELS;
            info->fix.xpanstep = 1;
            info->fix.ypanstep = 1;
            var->vmode = FB_VMODE_NONINTERLACED;
            var->blue.offset = 0;
            var->green.offset = 8;
            var->red.offset = 16;
            var->blue.length = 8;
            var->green.length = 8;
            var->red.length = 8;
            var->blue.msb_right = 0;
            var->green.msb_right = 0;
            var->red.msb_right = 0;
            var->transp.offset = 0;
            var->transp.length = 0;
            var->bits_per_pixel = 3 * 8;
        break;
        case MDP_ARGB_8888:
            info->fix.type = FB_TYPE_PACKED_PIXELS;
            info->fix.xpanstep = 1;
            info->fix.ypanstep = 1;
            var->vmode = FB_VMODE_NONINTERLACED;
            var->blue.offset = 0;
            var->green.offset = 8;
            var->red.offset = 16;
            var->blue.length = 8;
            var->green.length = 8;
            var->red.length = 8;
            var->blue.msb_right = 0;
            var->green.msb_right = 0;
            var->red.msb_right = 0;
            var->transp.offset = 24;
            var->transp.length = 8;
            var->bits_per_pixel = 4 * 8;
            break;
        case MDP_RGBA_8888:
            info->fix.type = FB_TYPE_PACKED_PIXELS;
            info->fix.xpanstep = 1;
            info->fix.ypanstep = 1;
            var->vmode = FB_VMODE_NONINTERLACED;
            var->blue.offset = 8;
            var->green.offset = 16;
            var->red.offset = 24;
            var->blue.length = 8;
            var->green.length = 8;
            var->red.length = 8;
            var->blue.msb_right = 0;
            var->green.msb_right = 0;
            var->red.msb_right = 0;
            var->transp.offset = 0;
            var->transp.length = 8;
            var->bits_per_pixel = 4 * 8;
        break;
        case MDP_YCRYCB_H2V1:
            info->fix.type = FB_TYPE_INTERLEAVED_PLANES;
            info->fix.xpanstep = 2;
            info->fix.ypanstep = 1;
            var->vmode = FB_VMODE_NONINTERLACED;
            /* how about R/G/B offset? */
            var->blue.offset = 0;
            var->green.offset = 5;
            var->red.offset = 11;
            var->blue.length = 5;
            var->green.length = 6;
            var->red.length = 5;
            var->blue.msb_right = 0;
            var->green.msb_right = 0;
            var->red.msb_right = 0;
            var->transp.offset = 0;
            var->transp.length = 0;
            var->bits_per_pixel = 2 * 8;
        break;
        default:
            hilcd_error("stlcd_fb_init: fb %d unkown image type!\n",
                info->node);
            return LCD_ERROR;
        }
    return LCD_OK;
}

static s32 __devinit stlcdfb_probe (struct platform_device *pdev)
{
    struct fb_info *info;
    struct stlcdfb_par *par;
    struct fb_var_screeninfo *var = NULL;
    s32 retval = -ENOMEM;
    size_t size;
    u32 phys = 0;
   // u32 product_type = 0xff;

    /*输入参数检查*/
    if(NULL == pdev)
    {
        return -ENOMEM;
    }

    /*分配一个fb_info结构体,结构体中通用指针*par为struct stlcdfb_par结构*/
    info = framebuffer_alloc(sizeof(struct stlcdfb_par), &pdev->dev);
    if (!info)
        return retval;

    /*info 作为pdev的drvdata*/
    platform_set_drvdata(pdev, info);

    /*par 挂接在info->par中*/
    par = info->par;

    /*初始化屏幕互斥信号量*/
    osl_sem_init(LCD_SEM_FULL, &(par->g_screensemmux));

    par->g_ulLcdPwrOnState = LCD_PWR_OFF_STATE;

    /*创建lcd类*/
    lcd.lcd_class = class_create(THIS_MODULE, "lcd");
    if (IS_ERR(lcd.lcd_class)) {
        goto dealloc_fb;

    }

    par->fb_page = FB_NUM;
    par->fb_imgType = MDP_RGB_565;
    par->ref_cnt = 0;
    par->lcd_backlight_registered = 0;

    /*初始化fb_info结构体*/

    strcpy(info->fix.id , "STLCD");

    info->fbops = &stlcdfb_ops;

    stlcdfb_fill(info,par);


    info->flags = FBINFO_FLAG_DEFAULT | FBINFO_VIRTFB;
    var = (struct fb_var_screeninfo *)&info->var  ;
    var->xres = LCD_WIDTH + 1;
    var->yres = LCD_HIGHT + 1;
    var->xres_virtual = LCD_WIDTH + 1;
    var->yres_virtual = var->yres * par->fb_page;


    par->var_xres = var->xres;
    par->var_yres = var->yres;
    par->var_pixclock = var->pixclock;

    //info->fix.line_length = stlcd_fb_line_length(info->node, var->xres,var->bits_per_pixel/8);
    info->fix.line_length =  var->xres /8; /*  128/8=16  */


    size = info->fix.line_length * var->yres;
    par->vmem =  dma_alloc_coherent(NULL, size,(dma_addr_t *) &phys, GFP_KERNEL);
    if (!par->vmem){
        hilcd_error("unable to alloc fbmem size = %u\n",size);
        goto class_exit;
    }

    info->screen_base = (u8 __force __iomem *)(par->vmem);/*(u8 __force __iomem *)*/
    info->fix.smem_start = phys;
    info->fix.smem_len = size;  /*16* 64 = 1024  */

    info->flags = FBINFO_FLAG_DEFAULT | FBINFO_VIRTFB;


    par->led_cdev.name = "lcd-backlight";
    par->led_cdev.brightness = MAX_BACKLIGHT_BRIGNTNESS;
    par->led_cdev.brightness_set = balong_bl_set_intensity;
    par->info = info;
    par->pdev = pdev;
    g_fbinfo = info;

    retval = register_framebuffer(info);
    if (retval < 0)
        goto release_mem;
#if 1
    if(!par->lcd_backlight_registered){
        if(led_classdev_register(&pdev->dev,&par->led_cdev))
            hilcd_error("led_classdev_register failed!");
        else
            par->lcd_backlight_registered = 1;
    }
#endif

    #ifdef CONFIG_HAS_EARLYSUSPEND

    par->earlysus_lcd.suspend = lcd_suspend_early;
    par->earlysus_lcd.resume = lcd_resume_late;
    par->earlysus_lcd.level = EARLY_SUSPEND_LEVEL_BLANK_SCREEN;

    register_early_suspend(&(par->earlysus_lcd));
    #endif
  
    hilcd_trace(
        "fb%d: %s frame buffer device,\n\tusing %d Byte of video memory\n",
        info->node, info->fix.id, info->fix.smem_len);

    (void)spi_init(SPI_NO_LCD);
    retval = gpio_request(LCD_CD, "oled_cd");
    if (retval)
    {
        printk(KERN_ERR "%s: Failed to get oled_reset gpio %d. Code: %d.",
            __func__,LCD_CD, retval);
        return retval;
    }
    gpio_direction_output(LCD_CD,1);/*lint !e534*/
    gpio_set_function(LCD_CD,GPIO_NORMAL);/*lint !e534*/
    printk("gpio_request :2-08.\n");

    return LCD_OK;

release_mem:
    kfree(par->vmem);
class_exit:
     class_destroy(lcd.lcd_class);
dealloc_fb:
    platform_set_drvdata(pdev,NULL);
    framebuffer_release(info);
    osl_sema_delete(&(par->g_screensemmux));

    return retval;
}

static s32 __devexit stlcdfb_remove(struct platform_device *pdev)
{
    struct fb_info *info = platform_get_drvdata(pdev);
    struct stlcdfb_par *par = (struct stlcdfb_par *)info->par;

    platform_set_drvdata(pdev, NULL);
    if (info) {
        unregister_framebuffer(info);
        kfree(info->screen_base);
        framebuffer_release(info);
        osl_sema_delete(&(par->g_screensemmux));
    }
    if(par->lcd_backlight_registered){
        par->lcd_backlight_registered = 0;
       // led_classdev_unregister(&par->led_cdev);

    }
    #ifdef CONFIG_HAS_EARLYSUSPEND
    unregister_early_suspend(&(par->earlysus_lcd));
    #endif
    return 0;
}

#ifdef CONFIG_PM
static int oled_spi_suspend(struct device *dev)
{
    printk(KERN_INFO "oled_spi_suspend \r\n");
    spi_suspend(SPI_NO_LCD);
    oled_suspend = 1;
    return 0;
}

static int oled_spi_resume(struct device *dev)
{
    spi_resume(SPI_NO_LCD);
    oled_suspend = 0;
    printk(KERN_INFO "oled_spi_resume \r\n");
    return 0;
}

static const struct dev_pm_ops oled_spi_dev_pm_ops ={
    .suspend  = oled_spi_suspend,
    .resume   = oled_spi_resume,
};

#define BALONG_DEV_PM_OPS (&oled_spi_dev_pm_ops)

#else
#define BALONG_DEV_PM_OPS NULL
#endif

/*驱动注册*/
static struct platform_driver stlcdfb_driver = {
    .driver = {
    .name   = "stlcd_tft_fb",
    .owner  = THIS_MODULE,
    .pm = BALONG_DEV_PM_OPS
    },

    .probe  = stlcdfb_probe,

    .remove = __devexit_p(stlcdfb_remove),
};

/*设备注册*/
static struct resource balong_lcd_resource[] = {
};
struct platform_device balong_device_lcd = {
    .name = "stlcd_tft_fb",
    .id = -1,
    .num_resources = ARRAY_SIZE(balong_lcd_resource),
    .resource = balong_lcd_resource,
};

static int __init stlcdfb_init(void)
{
    platform_device_register(&balong_device_lcd);
    platform_driver_register(&stlcdfb_driver);
    return 0;

}

static void __exit stlcdfb_exit(void)
{
    platform_driver_unregister(&stlcdfb_driver);
    platform_device_unregister(&balong_device_lcd);
}

module_init(stlcdfb_init);
module_exit(stlcdfb_exit);
