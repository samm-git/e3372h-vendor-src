/******************************************************************************/
/*  Copyright (C), 2007-2014, Hisilicon Technologies Co., Ltd. */
/******************************************************************************/
/* File name     : hi_syscrg.h */
/* Version       : 2.0 */
/* Author        : xxx*/
/* Created       : 2014-03-17*/
/* Last Modified : */
/* Description   :  The C union definition file for the module syscrg*/
/* Function List : */
/* History       : */
/* 1 Date        : */
/* Author        : xxx*/
/* Modification  : Create file */
/******************************************************************************/

#ifndef __HI_SYSCRG_H__
#define __HI_SYSCRG_H__

/*
 * Project: hi
 * Module : syscrg
 */

#ifndef HI_SET_GET
#define HI_SET_GET(a0,a1,a2,a3,a4)
#endif

/********************************************************************************/
/*    syscrg 寄存器偏移定义（项目名_模块名_寄存器名_OFFSET)        */
/********************************************************************************/
#define    HI_CRG_CLKEN1_OFFSET                              (0x0) /* 时钟使能1 */
#define    HI_CRG_CLKDIS1_OFFSET                             (0x4) /* 时钟关闭1 */
#define    HI_CRG_CLKSTAT1_OFFSET                            (0x8) /* 时钟控制状态1 */
#define    HI_CRG_CLKEN2_OFFSET                              (0xC) /* 时钟使能2 */
#define    HI_CRG_CLKDIS2_OFFSET                             (0x10) /* 时钟关闭2 */
#define    HI_CRG_CLKSTAT2_OFFSET                            (0x14) /* 时钟控制状态2 */
#define    HI_CRG_CLKEN3_OFFSET                              (0x18) /* 时钟使能3 */
#define    HI_CRG_CLKDIS3_OFFSET                             (0x1C) /* 时钟关闭3 */
#define    HI_CRG_CLKSTAT3_OFFSET                            (0x20) /* 时钟控制状态3 */
#define    HI_CRG_CLKEN4_OFFSET                              (0x24) /* 时钟使能4(MCPU控制) */
#define    HI_CRG_CLKDIS4_OFFSET                             (0x28) /* 时钟关闭4(MCPU控制) */
#define    HI_CRG_CLKSTAT4_OFFSET                            (0x2C) /* 时钟控制状态4(MCPU控制) */
#define    HI_CRG_CLKEN5_OFFSET                              (0x30) /* 时钟使能5（BBP） */
#define    HI_CRG_CLKDIS5_OFFSET                             (0x34) /* 时钟关闭5 */
#define    HI_CRG_CLKSTAT5_OFFSET                            (0x38) /* 时钟控制状态5 */
#define    HI_CRG_SRSTEN1_OFFSET                             (0x60) /* 软复位请求使能1 */
#define    HI_CRG_SRSTDIS1_OFFSET                            (0x64) /* 软复位请求关闭1 */
#define    HI_CRG_SRSTSTAT1_OFFSET                           (0x68) /* 软复位请求状态1 */
#define    HI_CRG_SRSTEN2_OFFSET                             (0x6C) /* 软复位请求使能2 */
#define    HI_CRG_SRSTDIS2_OFFSET                            (0x70) /* 软复位请求关闭2 */
#define    HI_CRG_SRSTSTAT2_OFFSET                           (0x74) /* 软复位请求状态2 */
#define    HI_CRG_SRSTEN3_OFFSET                             (0x78) /* 软复位请求使能3（ABB+BBP） */
#define    HI_CRG_SRSTDIS3_OFFSET                            (0x7C) /* 软复位请求关闭3（ABB+BBP） */
#define    HI_CRG_SRSTSTAT3_OFFSET                           (0x80) /* 软复位请求状态3（ABB+BBP） */
#define    HI_CRG_CLKDIV1_OFFSET                             (0x100) /* 时钟分频控制1(SIO PCM时钟分频） */
#define    HI_CRG_CLKDIV2_OFFSET                             (0x104) /* 时钟分频控制2(总线分频） */
#define    HI_CRG_CLKDIV3_OFFSET                             (0x108) /* 时钟分频控制3(MMC0/1) */
#define    HI_CRG_CLKDIV4_OFFSET                             (0x10C) /* 时钟分频控制4(HIFI时钟分频) */
#define    HI_CRG_CLKDIV5_OFFSET                             (0x110) /* 时钟分频控制5(BBE/SCI时钟分频) */
#define    HI_CRG_MMC_CTRL_OFFSET                            (0x114) /* MMC0/1调相控制 */
#define    HI_CRG_PLLLOCK_CTRL_OFFSET                        (0x118) /* pll锁定控制寄存器 */
#define    HI_CRG_CLK_SEL1_OFFSET                            (0x140) /* 时钟源选择控制1 */
#define    HI_CRG_CLK_SEL2_OFFSET                            (0x144) /* 时钟源选择控制2(TIMER) */
#define    HI_CRG_CLK_SEL3_OFFSET                            (0x148) /* 时钟源选择控制3（BBP） */
#define    HI_CRG_DFS1_CTRL1_OFFSET                          (0x200) /* A9 DFS配置1（1.2GHz) */
#define    HI_CRG_DFS1_CTRL2_OFFSET                          (0x204) /* A9 DFS配置2 */
#define    HI_CRG_DFS1_CTRL3_OFFSET                          (0x208) /* A9 DFS配置3 */
#define    HI_CRG_DFS2_CTRL1_OFFSET                          (0x20C) /* DSP DFS配置1(1200MHz) */
#define    HI_CRG_DFS2_CTRL2_OFFSET                          (0x210) /* DSP DFS配置2 */
#define    HI_CRG_DFS2_CTRL3_OFFSET                          (0x214) /* DSP DFS配置3 */
#define    HI_CRG_DFS3_CTRL1_OFFSET                          (0x218) /* HIFI DFS配置1（1.6GHz) */
#define    HI_CRG_DFS3_CTRL2_OFFSET                          (0x21C) /* HIFI DFS配置2 */
#define    HI_CRG_DFS3_CTRL3_OFFSET                          (0x220) /* HIFI DFS配置3 */
#define    HI_CRG_DFS4_CTRL1_OFFSET                          (0x224) /* BBP DFS配置1(491.52MHz) */
#define    HI_CRG_DFS4_CTRL2_OFFSET                          (0x228) /* BBP DFS配置2 */
#define    HI_CRG_DFS4_CTRL3_OFFSET                          (0x22C) /* BBP DFS配置3 */
#define    HI_CRG_DFS5_CTRL1_OFFSET                          (0x230) /* PERI DFS配置1(960MHz) */
#define    HI_CRG_DFS5_CTRL2_OFFSET                          (0x234) /* PERI DFS配置2 */
#define    HI_CRG_DFS5_CTRL3_OFFSET                          (0x238) /* PERI DFS配置3 */
#define    HI_CRG_DFS6_CTRL1_OFFSET                          (0x23C) /* PERI USB DFS配置1(500MHz) */
#define    HI_CRG_DFS6_CTRL2_OFFSET                          (0x240) /* PERI USB DFS配置2 */
#define    HI_CRG_DFS6_CTRL3_OFFSET                          (0x244) /* PERI USB DFS配置3 */
#define    HI_CRG_CLKSW_STAT1_OFFSET                         (0x260) /* 时钟切换状态1 */
#define    HI_CRG_CLKDIV6_OFFSET                             (0x264) /* 时钟分频控制6（CM3时钟分频） */


#ifndef __ASSEMBLY__

/********************************************************************************/
/*    syscrg 寄存器定义（项目名_模块名_寄存器名_T)        */
/********************************************************************************/
typedef union
{
    struct
    {
        unsigned int    mst_aclk_en                : 1; /* [0..0] AXI master总线时钟使能。0：无影响；1：时钟使能 */
        unsigned int    dsp_aclk_en                : 1; /* [1..1] DSP总线时钟使能。0：无影响；1：时钟使能。该bit主要用于功耗测试，正常工作时该时钟一直为1 */
        unsigned int    soc_hclk_en                : 1; /* [2..2] SOC_HCLK/SOC_ASHB_PCLK时钟使能。0：无影响；1：时钟使能 该bit主要用于功耗测试，正常工作时该时钟一直为1 */
        unsigned int    amon_cpufast_clk_en        : 1; /* [3..3] AMON_CPUFAST模块时钟使能。0：无影响；1：时钟使能。 */
        unsigned int    amon_soc_clk_en            : 1; /* [4..4] AMON_SOC时钟使能。0：无影响；1：时钟使能 */
        unsigned int    tsensor_clk_en             : 1; /* [5..5] TSENSOR时钟使能控制。0：无影响；1：时钟使能。 */
        unsigned int    reserved_2                 : 2; /* [7..6] 保留 */
        unsigned int    ipf_clk_en                 : 1; /* [8..8] IPF时钟使能控制。0：无影响；1：时钟使能。 */
        unsigned int    reserved_1                 : 1; /* [9..9] 保留 */
        unsigned int    edmac_ch4_clk_en           : 1; /* [10..10] EDMAC_CH4时钟使能控制。0：无影响；1：时钟使能。 */
        unsigned int    socp_clk_en                : 1; /* [11..11] SOCP时钟使能。0：无影响；1：时钟使能 */
        unsigned int    mipi_clk_en                : 1; /* [12..12] MIPI时钟使能。0：无影响；1：时钟使能 */
        unsigned int    pmussi0_clk_en             : 1; /* [13..13] PMU_SSI0时钟使能控制。0：无影响；1：时钟使能 */
        unsigned int    pmussi1_clk_en             : 1; /* [14..14] PMUSSI1时钟使能控制。0：无影响；1：时钟使能 */
        unsigned int    dw_ssi0_clk_en             : 1; /* [15..15] DW SSI0时钟使能控制。0：无影响；1：时钟使能 */
        unsigned int    efuse_clk_en               : 1; /* [16..16] eFUSE时钟使能。0：无影响；1：时钟使能 */
        unsigned int    nandc_clk_en               : 1; /* [17..17] NANDC时钟使能。                              0：无影响；1：时钟使能 */
        unsigned int    hs_uart_clk_en             : 1; /* [18..18] HS_UART时钟使能。0：无影响；1：时钟使能 */
        unsigned int    emi_clk_en                 : 1; /* [19..19] EMI时钟使能。0：无影响；1：时钟使能。 */
        unsigned int    edmac_clk_en               : 1; /* [20..20] EDMAC_CH16时钟使能控制。0：无影响；1：时钟使能。 */
        unsigned int    bootrom_clk_en             : 1; /* [21..21] BOOTROM时钟使能控制。0：无影响；1：时钟使能.安全启动后，该时钟可以关闭。 */
        unsigned int    pwm1_clk_en                : 1; /* [22..22] PWM1时钟使能。0：无影响；1：时钟使能 */
        unsigned int    pwm0_clk_en                : 1; /* [23..23] PWM0时钟使能。0：无影响；1：时钟使能 */
        unsigned int    dt_clk_en                  : 1; /* [24..24] DramTest时钟使能。0：无影响；1：时钟使能。系统启动后关闭该时钟。 */
        unsigned int    ipcm_clk_en                : 1; /* [25..25] IPCM时钟使能。                            0：无影响；1：时钟使能；该时钟系统启动后一直使能。 */
        unsigned int    dw_ssi1_clk_en             : 1; /* [26..26] DW SSI1时钟使能。                           0：无影响；1：时钟使能 */
        unsigned int    i2c1_clk_en                : 1; /* [27..27] I2C1时钟使能。                             0：无影响；                                 1：时钟使能 */
        unsigned int    gpio1_clk_en               : 1; /* [28..28] GPIO1总线时钟使能。0：无影响；1：时钟使能 */
        unsigned int    i2c0_clk_en                : 1; /* [29..29] I2C0时钟使能。                             0：无影响；                                 1：时钟使能 */
        unsigned int    gpio3_clk_en               : 1; /* [30..30] GPIO3总线时钟使能。0：无影响；1：时钟使能 */
        unsigned int    reserved_0                 : 1; /* [31..31] 保留 */
    } bits;
    unsigned int    u32;
}HI_CRG_CLKEN1_T;    /* 时钟使能1 */

typedef union
{
    struct
    {
        unsigned int    mst_aclk_dis               : 1; /* [0..0] AXI master总线时钟关闭控制。                        0：无影响 ；                                 1：时钟关闭 */
        unsigned int    dsp_aclk_dis               : 1; /* [1..1] DSP总线时钟关闭控制。                         0：无影响 ；                                 1：时钟关闭 */
        unsigned int    soc_hclk_dis               : 1; /* [2..2] SOC_HCLK/SOC_ASHB_PCLK时钟关闭控制。（AHB）                     0：无影响 ；                                 1：时钟关闭 */
        unsigned int    amon_cpufast_clk_dis       : 1; /* [3..3] AMON_CPUFAST模块时钟关闭控制。                     0：无影响 ；                                 1：时钟关闭 */
        unsigned int    amon_soc_clk_dis           : 1; /* [4..4] 保留 */
        unsigned int    tsensor_clk_dis            : 1; /* [5..5] TSENSOR时钟关闭控制。                       0：无影响 ；                                 1：时钟关闭 */
        unsigned int    reserved_2                 : 2; /* [7..6] 保留 */
        unsigned int    ipf_clk_dis                : 1; /* [8..8] IPF时钟关闭控制。                              0：无影响 ；                                 1：时钟关闭 */
        unsigned int    reserved_1                 : 1; /* [9..9] 保留 */
        unsigned int    edmac_ch4_clk_dis          : 1; /* [10..10] EDAMC_CH4时钟关闭控制。                              0：无影响 ；                                 1：时钟关闭 */
        unsigned int    socp_clk_dis               : 1; /* [11..11] SOCP时钟关闭控制。0：无影响； 1：时钟关闭 */
        unsigned int    mipi_clk_dis               : 1; /* [12..12] MIPI时钟关闭控制。0：无影响； 1：时钟关闭 */
        unsigned int    pmussi0_clk_dis            : 1; /* [13..13] PMUSSI0时钟关闭控制。                           0：无影响 ；                                 1：时钟关闭 */
        unsigned int    pmussi1_clk_dis            : 1; /* [14..14] PMUSSI1时钟关闭控制。                           0：无影响 ；                                 1：时钟关闭 */
        unsigned int    dw_ssi0_clk_dis            : 1; /* [15..15] DW SSI0时钟关闭控制。                           0：无影响 ；                                 1：时钟关闭 */
        unsigned int    efuse_clk_dis              : 1; /* [16..16] EFUSE时钟关闭控制。                              0：无影响 ；                                 1：时钟关闭 */
        unsigned int    nandc_clk_dis              : 1; /* [17..17] NANDC时钟关闭控制。                              0：无影响 ；                                 1：时钟关闭 */
        unsigned int    hs_uart_clk_dis            : 1; /* [18..18] HS_UART时钟关闭控制。                              0：无影响 ；                                 1：时钟关闭 */
        unsigned int    emi_clk_dis                : 1; /* [19..19] EMI时钟关闭控制。                              0：无影响 ；                                 1：时钟关闭 */
        unsigned int    edmac_clk_dis              : 1; /* [20..20] EDMAC_CH4时钟关闭控制。                              0：无影响 ；                                 1：时钟关闭 */
        unsigned int    bootrom_clk_dis            : 1; /* [21..21] BOOTROM时钟关闭控制。                              0：无影响 ；                                 1：时钟关闭 */
        unsigned int    pwm2_clk_dis               : 1; /* [22..22] PWM2时钟关闭控制。                              0：无影响 ；                                 1：时钟关闭 */
        unsigned int    pwm1_clk_dis               : 1; /* [23..23] PWM1时钟关闭控制。                              0：无影响 ；                                 1：时钟关闭 */
        unsigned int    dt_clk_dis                 : 1; /* [24..24] DT时钟关闭控制。                              0：无影响 ；                                 1：时钟关闭 */
        unsigned int    ipcm_clk_dis               : 1; /* [25..25] IPCM时钟关闭控制。                            0：无影响 ；                                 1：时钟关闭 */
        unsigned int    dw_ssi1_clk_dis            : 1; /* [26..26] DW SSI1时钟关闭控制。                           0：无影响 ；                                 1：时钟关闭 */
        unsigned int    i2c1_clk_dis               : 1; /* [27..27] I2C时钟关闭控制。                             0：无影响 ；                                 1：时钟关闭 */
        unsigned int    gpio1_clk_dis              : 1; /* [28..28] GPIO1时钟关闭控制。                           0：无影响 ；                                 1：时钟关闭 */
        unsigned int    i2c0_clk_dis               : 1; /* [29..29] I2C0时钟关闭控制。                           0：无影响 ；                                 1：时钟关闭 */
        unsigned int    gpio3_clk_dis              : 1; /* [30..30] GPIO3时钟关闭控制。                           0：无影响 ；                                 1：时钟关闭 */
        unsigned int    reserved_0                 : 1; /* [31..31] 保留 */
    } bits;
    unsigned int    u32;
}HI_CRG_CLKDIS1_T;    /* 时钟关闭1 */

typedef union
{
    struct
    {
        unsigned int    mst_aclk_status            : 1; /* [0..0] AXI master时钟控制状态。                            0：时钟关闭；                                 1：时钟使能 */
        unsigned int    dsp_aclk_status            : 1; /* [1..1] DSP总线时钟控制状态。                            0：时钟关闭；                                 1：时钟使能 */
        unsigned int    soc_hclk_status            : 1; /* [2..2] SOC_HCLK/SOC_ASHB_PCLK时钟控制状态。                            0：时钟关闭；                                 1：时钟使能 */
        unsigned int    amon_cpufast_clk_status    : 1; /* [3..3] AMON_CPUFAST时钟控制状态。                            0：时钟关闭；                                 1：时钟使能 */
        unsigned int    amon_soc_clk_status        : 1; /* [4..4] AMON_SOC时钟控制状态。                            0：时钟关闭；                                 1：时钟使能 */
        unsigned int    reserved_2                 : 3; /* [7..5] 保留 */
        unsigned int    ipf_clk_status             : 1; /* [8..8] IPF时钟控制状态。                            0：时钟关闭；                                 1：时钟使能 */
        unsigned int    reserved_1                 : 1; /* [9..9] 保留 */
        unsigned int    edmac_ch4_clk_status       : 1; /* [10..10] EDMAC_CH4时钟控制状态。                            0：时钟关闭；                                 1：时钟使能 */
        unsigned int    socp_clk_status            : 1; /* [11..11] SOCP时钟控制状态。                            0：时钟关闭；                                 1：时钟使能 */
        unsigned int    mipi_clk_status            : 1; /* [12..12] MIPI时钟控制状态。                            0：时钟关闭；                                 1：时钟使能 */
        unsigned int    pmussi0_clk_status         : 1; /* [13..13] PMUSSI1时钟控制状态。                            0：时钟关闭；                                 1：时钟使能 */
        unsigned int    pmussi1_clk_status         : 1; /* [14..14] PMUSSI1时钟控制状态。                            0：时钟关闭；                                 1：时钟使能 */
        unsigned int    dw_ssi0_clk_status         : 1; /* [15..15] DWSSI0时钟控制状态。                            0：时钟关闭；                                 1：时钟使能 */
        unsigned int    efuse_clk_status           : 1; /* [16..16] EFUSE时钟控制状态。                            0：时钟关闭；                                 1：时钟使能 */
        unsigned int    nandc_clk_status           : 1; /* [17..17] NANDC时钟控制状态。                            0：时钟关闭；                                 1：时钟使能 */
        unsigned int    hs_uart_clk_status         : 1; /* [18..18] HS_UART时钟控制状态。                            0：时钟关闭；                                 1：时钟使能 */
        unsigned int    emi_clk_status             : 1; /* [19..19] EMI时钟控制状态。                            0：时钟关闭；                                 1：时钟使能 */
        unsigned int    edmac_clk_status           : 1; /* [20..20] EDMAC时钟控制状态。                            0：时钟关闭；                                 1：时钟使能 */
        unsigned int    bootrom_clk_status         : 1; /* [21..21] BOOTROM时钟控制状态。                            0：时钟关闭；                                 1：时钟使能 */
        unsigned int    pwm2_clk_status            : 1; /* [22..22] PWM2时钟控制状态。                            0：时钟关闭；                                 1：时钟使能 */
        unsigned int    pwm1_clk_status            : 1; /* [23..23] PWM1时钟控制状态。                            0：时钟关闭；                                 1：时钟使能 */
        unsigned int    dt_clk_status              : 1; /* [24..24] DT时钟控制状态。                            0：时钟关闭；                                 1：时钟使能 */
        unsigned int    ipcm_clk_status            : 1; /* [25..25] IPCM时钟控制状态。                            0：时钟关闭；                                 1：时钟使能 */
        unsigned int    dw_ssi1_clk_status         : 1; /* [26..26] DWSSI1时钟控制状态。                            0：时钟关闭；                                 1：时钟使能 */
        unsigned int    i2c1_clk_status            : 1; /* [27..27] I2C1时钟控制状态。                            0：时钟关闭；                                 1：时钟使能 */
        unsigned int    gpio1_clk_status           : 1; /* [28..28] GPIO1时钟控制状态。                            0：时钟关闭；                                 1：时钟使能 */
        unsigned int    i2c0_clk_status            : 1; /* [29..29] I2C0时钟控制状态。                            0：时钟关闭；                                 1：时钟使能 */
        unsigned int    gpio3_clk_status           : 1; /* [30..30] GPIO3时钟控制状态。                            0：时钟关闭；                                 1：时钟使能 */
        unsigned int    reserved_0                 : 1; /* [31..31] 保留 */
    } bits;
    unsigned int    u32;
}HI_CRG_CLKSTAT1_T;    /* 时钟控制状态1 */

typedef union
{
    struct
    {
        unsigned int    uart3_clk_en               : 1; /* [0..0] UART3工作时钟使能控制。0：无影响；1：时钟使能 */
        unsigned int    uart2_clk_en               : 1; /* [1..1] UART2工作时钟使能控制。0：无影响；1：时钟使能 */
        unsigned int    gpio2_clk_en               : 1; /* [2..2] GPIO2时钟使能控制。                         0：无影响1：时钟使能 */
        unsigned int    cs_clk_en                  : 1; /* [3..3] CORESIGHT模块时钟使能控制。0：无影响；1：时钟使能 */
        unsigned int    appa9_mbist_clk_en         : 1; /* [4..4] appa9_mbist_clk时钟使能控制。0：无影响；1：时钟使能。正常工作时关闭该时钟。 */
        unsigned int    moda9_mbist_clk_en         : 1; /* [5..5] moda9_mbist_clk时钟使能控制。0：无影响；1：时钟使能。正常工作时关闭该时钟。 */
        unsigned int    cpu_aclk_en                : 1; /* [6..6] CPU总线时钟使能控制。0：无影响；1：时钟使能。该bit用于测试目的，正常工作时一直使能。 */
        unsigned int    mddrc_clk_en               : 1; /* [7..7] MDDRC模块时钟使能。0：无影响；1：时钟使能。正常工作时，该时钟需要一直使能。 */
        unsigned int    ddrphy_clk_en              : 1; /* [8..8] DDRPHY时钟使能控制。0：无影响；1：时钟使能 正常工作时，该时钟需要一直使能。 */
        unsigned int    ddrphy_ddr_clk_en          : 1; /* [9..9] DDRPHY_2XCLK时钟使能控制。0：无影响；1：时钟使能 当DDRPHY工作在PLL bypass模式时，该bit必须一直使能，否则可以关闭该时钟 */
        unsigned int    axi_mem_clk_en             : 1; /* [10..10] AXI_MEM_CLK时钟使能控制。0：无影响；1：时钟使能 */
        unsigned int    func_wakeup_pd_clk_en      : 1; /* [11..11] func_wakeup时钟使能控制。                              0：无影响；1：时钟使能 */
        unsigned int    soc_peri_usb_pd_clk_en     : 1; /* [12..12] SOC_PERI_USB电压域时钟使能控制。0：无影响；1：时钟使能 */
        unsigned int    usbotg_clk_en              : 1; /* [13..13] USBOTG时钟使能控制。0：无影响；1：时钟使能 */
        unsigned int    usb_bc_clk_en              : 1; /* [14..14] USBOTG BC时钟使能控制。 0：无影响；1：时钟使能 */
        unsigned int    ssic_host_clk_en           : 1; /* [15..15] SSIC HOST控制器时钟使能控制。0：无影响；1：时钟使能 */
        unsigned int    ssic_dev_clk_en            : 1; /* [16..16] SSIC Device控制器时钟使能控制。0：无影响；1：时钟使能 */
        unsigned int    mmc0_clk_en                : 1; /* [17..17] MMC0时钟使能控制。                              0：无影响；1：时钟使能 */
        unsigned int    mmc1_clk_en                : 1; /* [18..18] MMC1时钟使能控制。0：无影响；1：时钟使能 */
        unsigned int    mmc0_phase_clk_en          : 1; /* [19..19] MMC0调相时钟使能控制。                              0：无影响；1：时钟使能 */
        unsigned int    mmc1_phase_clk_en          : 1; /* [20..20] MMC1调相时钟使能控制。0：无影响；1：时钟使能 */
        unsigned int    mmc0_refclk_en             : 1; /* [21..21] MMC0调相输入时钟使能控制。                              0：无影响；1：时钟使能 */
        unsigned int    mmc1_refclk_en             : 1; /* [22..22] MMC1调相输入时钟使能控制。                              0：无影响；1：时钟使能 */
        unsigned int    usbhsic_clk_en             : 1; /* [23..23] USBHSIC时钟使能。0：无影响；1：时钟使能 */
        unsigned int    sdcc_clk_en                : 1; /* [24..24] SDCC时钟使能。0：无影响；1：时钟使能 */
        unsigned int    hpm_clk_en                 : 1; /* [25..25] HPM0-2时钟使能控制。0：无影响；1：时钟使能。该时钟可以非用即关。 */
        unsigned int    sha1_clk_en                : 1; /* [26..26] SHA1时钟使能控制。0：无影响；1：时钟使能。该时钟可以非用即关。 */
        unsigned int    pcie_ctrl_clk_en           : 2; /* [28..27] PCIE控制器时钟使能控制。0：无影响；1：时钟使能。Bit[27] - 控制总线接口时钟；bit[28] - 控制PHY接口时钟；(core_clk和aux_clk） */
        unsigned int    pcie_phy_clk_en            : 1; /* [29..29] PCIEPHY时钟使能控制。0：无影响；1：时钟使能。 */
        unsigned int    reserved                   : 1; /* [30..30] 保留 */
        unsigned int    wdog_clk_en                : 1; /* [31..31] WDOG时钟使能控制。0：无影响；1：时钟使能。该时钟系统启动后一直使能。 */
    } bits;
    unsigned int    u32;
}HI_CRG_CLKEN2_T;    /* 时钟使能2 */

typedef union
{
    struct
    {
        unsigned int    uart3_clk_dis              : 1; /* [0..0] UART3工作时钟关闭控制。                        0：无影响；                                  1：时钟关闭 */
        unsigned int    uart2_clk_dis              : 1; /* [1..1] UART2工作时钟关闭控制。                         0：无影响；                                  1：时钟关闭 */
        unsigned int    gpio2_clk_dis              : 1; /* [2..2] GPIO2时钟关闭控制。                          0：无影响；                                  1：时钟关闭 */
        unsigned int    cs_clk_dis                 : 1; /* [3..3] CORESIGHT模块时钟关闭控制。                              0：无影响 ；                                 1：时钟关闭 */
        unsigned int    appa9_mbist_clk_dis        : 1; /* [4..4] appa9_mbist_clk时钟关闭控制。                              0：无影响 ；                                 1：时钟关闭 */
        unsigned int    moda9_mbist_clk_dis        : 1; /* [5..5] moda9_mbist_clk时钟关闭控制。                              0：无影响 ；                                 1：时钟关闭 */
        unsigned int    cpu_aclk_dis               : 1; /* [6..6] CPU总线时钟关闭控制。                          0：无影响；                                  1：时钟关闭 */
        unsigned int    mddrc_clk_dis              : 1; /* [7..7] MDDRC控制器时钟关闭控制。                          0：无影响；                                  1：时钟关闭 */
        unsigned int    ddrphy_clk_dis             : 1; /* [8..8] DDRPHY时钟关闭控制。                          0：无影响；                                  1：时钟关闭 */
        unsigned int    ddrphy_ddr_clk_dis         : 1; /* [9..9] DDRPHY_2XCLK时钟关闭控制。                          0：无影响；                                  1：时钟关闭 */
        unsigned int    axi_mem_clk_dis            : 1; /* [10..10] AXI_MEM_CLK时钟关闭控制。                          0：无影响；                                  1：时钟关闭 */
        unsigned int    func_wake_pd_clk_dis       : 1; /* [11..11] func_wakeup时钟关闭控制。                          0：无影响；                                  1：时钟关闭 */
        unsigned int    soc_usb_pd_clk_dis         : 1; /* [12..12] SOC_USB电压域时钟关闭控制。                          0：无影响；                                  1：时钟关闭 */
        unsigned int    usbotg_clk_dis             : 1; /* [13..13] USBOTG时钟关闭控制。                          0：无影响；                                  1：时钟关闭 */
        unsigned int    usb_bc_clk_dis             : 1; /* [14..14] USBOTG BC时钟关闭控制。                          0：无影响；                                  1：时钟关闭 */
        unsigned int    ssic_host_clk_dis          : 1; /* [15..15] SSIC HOST控制器时钟关闭控制。                          0：无影响；                                  1：时钟关闭 */
        unsigned int    ssic_dev_clk_dis           : 1; /* [16..16] SSIC Device控制器时钟关闭控制。                          0：无影响；                                  1：时钟关闭 */
        unsigned int    mmc0_clk_dis               : 1; /* [17..17] MMC0时钟关闭控制。                          0：无影响；                                  1：时钟关闭 */
        unsigned int    mmc1_clk_dis               : 1; /* [18..18] MMC1时钟关闭控制。                          0：无影响；                                  1：时钟关闭 */
        unsigned int    mmc0_phase_clk_dis         : 1; /* [19..19] MMC0调相时钟关闭控制。                          0：无影响；                                  1：时钟关闭 */
        unsigned int    mmc1_phase_clk_dis         : 1; /* [20..20] MMC1调相时钟关闭控制。                          0：无影响；                                  1：时钟关闭 */
        unsigned int    mmc0_refclk_dis            : 1; /* [21..21] MMC0调相输入参考时钟关闭控制。                          0：无影响；                                  1：时钟关闭 */
        unsigned int    mmc1_refclk_dis            : 1; /* [22..22] MMC1调相输入参考时钟关闭控制。                          0：无影响；                                  1：时钟关闭 */
        unsigned int    usbhsic_clk_dis            : 1; /* [23..23] USBHSIC时钟关闭控制。                          0：无影响；                                  1：时钟关闭 */
        unsigned int    sdcc_clk_dis               : 1; /* [24..24] SDCC时钟关闭控制。                          0：无影响；                                  1：时钟关闭 */
        unsigned int    hpm_clk_dis                : 1; /* [25..25] HPM0-2时钟关闭控制。0：无影响；1：时钟股关闭。该时钟可以非用即关。 */
        unsigned int    sha1_clk_dis               : 1; /* [26..26] SHA1时钟关闭控制。0：无影响；1：时钟股关闭。该时钟可以非用即关。 */
        unsigned int    pcie_ctrl_clk_dis          : 2; /* [28..27] PCIE控制器时钟关闭控制。0：无影响；1：时钟股关闭。 */
        unsigned int    pcie_phy_clk_dis           : 1; /* [29..29] PCIEPHY时钟关闭控制。0：无影响；1：时钟股关闭。 */
        unsigned int    reserved                   : 1; /* [30..30] 保留 */
        unsigned int    wdog_clk_dis               : 1; /* [31..31] WDOG时钟关闭控制。0：无影响；1：时钟关闭 */
    } bits;
    unsigned int    u32;
}HI_CRG_CLKDIS2_T;    /* 时钟关闭2 */

typedef union
{
    struct
    {
        unsigned int    uart3_clk_status           : 1; /* [0..0] UART3工作时钟控制状态。                         0：时钟关闭；                                 1：时钟使能 */
        unsigned int    uart2_clk_status           : 1; /* [1..1] UART2工作时钟控制状态。                         0：时钟关闭；                                 1：时钟使能 */
        unsigned int    gpio2_clk_status           : 1; /* [2..2] GPIO2时钟控制状态。                         0：时钟关闭；                                 1：时钟使能 */
        unsigned int    cs_clk_status              : 1; /* [3..3] CORESIGHT模块时钟控制状态。                         0：时钟关闭；                                 1：时钟使能 */
        unsigned int    appa9_mbist_clk_status     : 1; /* [4..4] appa9_mbist_clk时钟控制状态。                         0：时钟关闭；                                 1：时钟使能 */
        unsigned int    moda9_mbist_clk_status     : 1; /* [5..5] moda9_mbist_clk时钟控制状态。                         0：时钟关闭；                                 1：时钟使能 */
        unsigned int    cpu_aclk_status            : 1; /* [6..6] CPU总线时钟控制状态。                         0：时钟关闭；                                 1：时钟使能 */
        unsigned int    mddrc_clk_status           : 1; /* [7..7] MDDRC模块时控制状态。                         0：时钟关闭；                                 1：时钟使能 */
        unsigned int    ddrphy_clk_status          : 1; /* [8..8] DDRPHY时钟使能控制。0：无影响；1：时钟使能 */
        unsigned int    ddrphy_ddr_clk_status      : 1; /* [9..9] DDRPHY_2XCLK时钟使能控制。0：无影响；1：时钟使能 */
        unsigned int    axi_mem_clk_status         : 1; /* [10..10] AXI_MEM_CLK时钟控制状态。                         0：时钟关闭；                                 1：时钟使能 */
        unsigned int    func_wake_pd_clk_status    : 1; /* [11..11] func_wakeup电压域时钟控制状态。                     0：时钟关闭；                                 1：时钟使能 */
        unsigned int    soc_usb_pd_clk_status      : 1; /* [12..12] SOC_USB电压域时钟控制状态。                         0：时钟关闭；                                 1：时钟使能 */
        unsigned int    usbotg_clk_status          : 1; /* [13..13] USBOTG时钟使控制状态。                     0：时钟关闭；                                 1：时钟使能 */
        unsigned int    usb_bc_clk_status          : 1; /* [14..14] USBOTG BC时钟控制状态。                     0：时钟关闭；                                 1：时钟使能 */
        unsigned int    ssic_host_clk_status       : 1; /* [15..15] SSIC HOST控制器时钟控制状态。                     0：时钟关闭；                                 1：时钟使能 */
        unsigned int    ssic_dev_clk_status        : 1; /* [16..16] SSIC Device控制器时钟控制状态。                     0：时钟关闭；                                 1：时钟使能 */
        unsigned int    mmc0_clk_status            : 1; /* [17..17] MMC0时钟控制状态。                     0：时钟关闭；                                 1：时钟使能 */
        unsigned int    mmc1_clk_status            : 1; /* [18..18] MMC1时钟控制状态。                     0：时钟关闭；                                 1：时钟使能 */
        unsigned int    mmc0_phase_clk_status      : 1; /* [19..19] MMC0调相时钟控制状态。                     0：时钟关闭；                                 1：时钟使能 */
        unsigned int    mmc1_phase_clk_status      : 1; /* [20..20] MMC1调相时钟控制状态。                     0：时钟关闭；                                 1：时钟使能 */
        unsigned int    mmc0_refclk_status         : 1; /* [21..21] MMC0调相输入参考时钟控制状态。                     0：时钟关闭；                                 1：时钟使能 */
        unsigned int    mmc1_refclk_status         : 1; /* [22..22] MMC1调相输入参考时钟控制状态。                     0：时钟关闭；                                 1：时钟使能 */
        unsigned int    usbhsic_clk_status         : 1; /* [23..23] USBHSIC时钟控制状态。                     0：时钟关闭；                                 1：时钟使能 */
        unsigned int    sdcc_clk_status            : 1; /* [24..24] SDCC时钟控制状态。                     0：时钟关闭；                                 1：时钟使能 */
        unsigned int    hpm_clk_status             : 1; /* [25..25] HPM0-2时钟控制状态。                     0：时钟关闭；                                 1：时钟使能该时钟可以非用即关。 */
        unsigned int    sha1_clk_status            : 1; /* [26..26] SHA1时钟控制状态。                     0：时钟关闭；                                 1：时钟使能该时钟可以非用即关。 */
        unsigned int    pcie_ctrl_clk_status       : 2; /* [28..27] PCIE控制器控制状态。                     0：时钟关闭；                                 1：时钟使能 */
        unsigned int    pcie_phy_clk_status        : 1; /* [29..29] PCIEPHY控制状态。                     0：时钟关闭；                                 1：时钟使能 */
        unsigned int    reserved                   : 1; /* [30..30] 保留 */
        unsigned int    wdog_clk_status            : 1; /* [31..31] WDOG时钟状态0：时钟关闭；1：时钟使能 */
    } bits;
    unsigned int    u32;
}HI_CRG_CLKSTAT2_T;    /* 时钟控制状态2 */

typedef union
{
    struct
    {
        unsigned int    gpio0_clk_en               : 1; /* [0..0] GPIO0时钟使能控制。                         0：无影响1：时钟使能 */
        unsigned int    uart0_clk_en               : 1; /* [1..1] UART0时钟使能控制。                         0：无影响1：时钟使能 */
        unsigned int    tm0_clk_en                 : 1; /* [2..2] TIMER0时钟使能控制。                         0：无影响1：时钟使能 */
        unsigned int    tm1_clk_en                 : 1; /* [3..3] TIMER1时钟使能控制。                         0：无影响1：时钟使能 */
        unsigned int    tm2_clk_en                 : 1; /* [4..4] TIMER2时钟使能控制。                         0：无影响1：时钟使能 */
        unsigned int    tm3_clk_en                 : 1; /* [5..5] TIMER3时钟使能控制。                         0：无影响1：时钟使能 */
        unsigned int    tm4_clk_en                 : 1; /* [6..6] TIMER4时钟使能控制。                         0：无影响1：时钟使能 */
        unsigned int    tm5_clk_en                 : 1; /* [7..7] TIMER5时钟使能控制。                         0：无影响1：时钟使能 */
        unsigned int    tm6_clk_en                 : 1; /* [8..8] TIMER6时钟使能控制。                         0：无影响1：时钟使能 */
        unsigned int    tm7_clk_en                 : 1; /* [9..9] TIMER7时钟使能控制。                         0：无影响1：时钟使能 */
        unsigned int    tm8_clk_en                 : 1; /* [10..10] TIMER8时钟使能控制。                         0：无影响1：时钟使能 */
        unsigned int    tm9_clk_en                 : 1; /* [11..11] TIMER9时钟使能控制。                         0：无影响1：时钟使能 */
        unsigned int    tm10_clk_en                : 1; /* [12..12] TIMER10时钟使能控制。                         0：无影响1：时钟使能 */
        unsigned int    tm11_clk_en                : 1; /* [13..13] TIMER11时钟使能控制。                         0：无影响1：时钟使能 */
        unsigned int    tm12_clk_en                : 1; /* [14..14] TIMER12时钟使能控制。                         0：无影响1：时钟使能 */
        unsigned int    tm13_clk_en                : 1; /* [15..15] TIMER13时钟使能控制。                         0：无影响1：时钟使能 */
        unsigned int    tm14_clk_en                : 1; /* [16..16] TIMER14时钟使能控制。                         0：无影响1：时钟使能 */
        unsigned int    tm15_clk_en                : 1; /* [17..17] TIMER15时钟使能控制。                         0：无影响1：时钟使能 */
        unsigned int    tm16_clk_en                : 1; /* [18..18] TIMER16时钟使能控制。                         0：无影响1：时钟使能 */
        unsigned int    tm17_clk_en                : 1; /* [19..19] TIMER17时钟使能控制。                         0：无影响1：时钟使能 */
        unsigned int    tm18_clk_en                : 1; /* [20..20] TIMER18时钟使能控制。                         0：无影响1：时钟使能 */
        unsigned int    tm19_clk_en                : 1; /* [21..21] TIMER19时钟使能控制。                         0：无影响1：时钟使能 */
        unsigned int    tm20_clk_en                : 1; /* [22..22] TIMER20时钟使能控制。                         0：无影响1：时钟使能 */
        unsigned int    tm21_clk_en                : 1; /* [23..23] TIMER21时钟使能控制。                         0：无影响1：时钟使能 */
        unsigned int    tm22_clk_en                : 1; /* [24..24] TIMER22时钟使能控制。                         0：无影响1：时钟使能 */
        unsigned int    tm23_clk_en                : 1; /* [25..25] TIMER23总线时钟使能控制。                         0：无影响1：时钟使能 */
        unsigned int    appa9_clk_en               : 1; /* [26..26] APPA9电压域时钟使能控制。                         0：无影响1：时钟使能 */
        unsigned int    mdma9_clk_en               : 1; /* [27..27] MDMA9电压域时钟使能控制。                         0：无影响1：时钟使能 */
        unsigned int    hifi_vic_clk_en            : 1; /* [28..28] DSP VIC中断控制器时钟使能控制。                         0：无影响1：时钟使能 */
        unsigned int    lte_sio_clk_en             : 1; /* [29..29] LTE_SIO 时钟使能控制。0：无影响；1：时钟使能 */
        unsigned int    hifi_dbg_clk_en            : 1; /* [30..30] HIFI Debug时钟使能控制。0：无影响；1：时钟使能 */
        unsigned int    hifi_clk_en                : 1; /* [31..31] HIFI子系统时钟使能控制。0：无影响；1：时钟使能 */
    } bits;
    unsigned int    u32;
}HI_CRG_CLKEN3_T;    /* 时钟使能3 */

typedef union
{
    struct
    {
        unsigned int    gpio0_clk_dis              : 1; /* [0..0] GPIO0时钟关闭控制。                         0：无影响；                                  1：时钟关闭 */
        unsigned int    uart0_clk_dis              : 1; /* [1..1] UART0时钟关闭控制。                         0：无影响；                                  1：时钟关闭 */
        unsigned int    tm0_clk_dis                : 1; /* [2..2] TIMER0时钟关闭控制。                         0：无影响；                                  1：时钟关闭 */
        unsigned int    tm1_clk_dis                : 1; /* [3..3] TIMER1时钟关闭控制。                         0：无影响；                                  1：时钟关闭 */
        unsigned int    tm2_clk_dis                : 1; /* [4..4] TIMER2时钟关闭控制。                         0：无影响；                                  1：时钟关闭 */
        unsigned int    tm3_clk_dis                : 1; /* [5..5] TIMER3时钟关闭控制。                         0：无影响；                                  1：时钟关闭 */
        unsigned int    tm4_clk_dis                : 1; /* [6..6] TIMER4时钟关闭控制。                         0：无影响；                                  1：时钟关闭 */
        unsigned int    tm5_clk_dis                : 1; /* [7..7] TIMER5时钟关闭控制。                         0：无影响；                                  1：时钟关闭 */
        unsigned int    tm6_clk_dis                : 1; /* [8..8] TIMER6时钟关闭控制。                         0：无影响；                                  1：时钟关闭 */
        unsigned int    tm7_clk_dis                : 1; /* [9..9] TIMER7时钟关闭控制。                         0：无影响；                                  1：时钟关闭 */
        unsigned int    tm8_clk_dis                : 1; /* [10..10] TIMER8时钟关闭控制。                         0：无影响；                                  1：时钟关闭 */
        unsigned int    tm9_clk_dis                : 1; /* [11..11] TIMER9时钟关闭控制。                         0：无影响；                                  1：时钟关闭 */
        unsigned int    tm10_clk_dis               : 1; /* [12..12] TIMER10时钟关闭控制。                         0：无影响；                                  1：时钟关闭 */
        unsigned int    tm11_clk_dis               : 1; /* [13..13] TIMER11时钟关闭控制。                         0：无影响；                                  1：时钟关闭 */
        unsigned int    tm12_clk_dis               : 1; /* [14..14] TIMER12时钟关闭控制。                         0：无影响；                                  1：时钟关闭 */
        unsigned int    tm13_clk_dis               : 1; /* [15..15] TIMER13时钟关闭控制。                         0：无影响；                                  1：时钟关闭 */
        unsigned int    tm14_clk_dis               : 1; /* [16..16] TIMER14时钟关闭控制。                         0：无影响；                                  1：时钟关闭 */
        unsigned int    tm15_clk_dis               : 1; /* [17..17] TIMER15时钟关闭控制。                         0：无影响；                                  1：时钟关闭 */
        unsigned int    tm16_clk_dis               : 1; /* [18..18] TIMER16时钟关闭控制。                         0：无影响；                                  1：时钟关闭 */
        unsigned int    tm17_clk_dis               : 1; /* [19..19] TIMER17时钟关闭控制。                         0：无影响；                                  1：时钟关闭 */
        unsigned int    tm18_clk_dis               : 1; /* [20..20] TIMER18时钟关闭控制。                         0：无影响；                                  1：时钟关闭 */
        unsigned int    tm19_clk_dis               : 1; /* [21..21] TIMER19时钟关闭控制。                         0：无影响；                                  1：时钟关闭 */
        unsigned int    tm20_clk_dis               : 1; /* [22..22] TIMER20关闭控制。                         0：无影响；                                  1：时钟关闭 */
        unsigned int    tm21_clk_dis               : 1; /* [23..23] TIMER21时钟关闭控制。                         0：无影响；                                  1：时钟关闭 */
        unsigned int    tm22_clk_dis               : 1; /* [24..24] TIMER22时钟关闭控制。                         0：无影响；                                  1：时钟关闭 */
        unsigned int    tm23_clk_dis               : 1; /* [25..25] TIMER23时钟关闭控制。                         0：无影响；                                  1：时钟关闭 */
        unsigned int    appa9_clk_dis              : 1; /* [26..26] APPA9电压域时钟关闭控制。                         0：无影响；                                  1：时钟关闭 */
        unsigned int    mdma9_clk_dis              : 1; /* [27..27] MDMA9电压域时钟关闭控制。                         0：无影响；                                  1：时钟关闭 */
        unsigned int    hifi_vic_clk_dis           : 1; /* [28..28] HIFI VIC中断控制器时钟关闭控制。                         0：无影响；                                  1：时钟关闭 */
        unsigned int    lte_sio_clk_dis            : 1; /* [29..29] LTE_SIO 时钟关闭控制。0：无影响；1：时钟关闭 */
        unsigned int    hifi_dbg_clk_dis           : 1; /* [30..30] HIFI Debug时钟关闭控制。0：无影响；1：时钟关闭 */
        unsigned int    hifi_clk_dis               : 1; /* [31..31] HIFI子系统时钟关闭控制。0：无影响；1：时钟关闭 */
    } bits;
    unsigned int    u32;
}HI_CRG_CLKDIS3_T;    /* 时钟关闭3 */

typedef union
{
    struct
    {
        unsigned int    gpio0_clk_status           : 1; /* [0..0] GPIO0时钟控制状态。                         0：时钟关闭；                                 1：时钟使能 */
        unsigned int    uart0_clk_status           : 1; /* [1..1] UART0时钟控制状态。                         0：时钟关闭；                                 1：时钟使能 */
        unsigned int    tm0_clk_status             : 1; /* [2..2] TIMER0时钟控制状态。                         0：时钟关闭；                                 1：时钟使能 */
        unsigned int    tm1_clk_status             : 1; /* [3..3] TIMER1时钟控制状态。                         0：时钟关闭；                                 1：时钟使能 */
        unsigned int    tm2_clk_status             : 1; /* [4..4] TIMER2时钟控制状态。                         0：时钟关闭；                                 1：时钟使能 */
        unsigned int    tm3_clk_status             : 1; /* [5..5] TIMER3时钟控制状态。                         0：时钟关闭；                                 1：时钟使能 */
        unsigned int    tm4_clk_status             : 1; /* [6..6] TIMER4时钟控制状态。                         0：时钟关闭；                                 1：时钟使能 */
        unsigned int    tm5_clk_status             : 1; /* [7..7] TIMER5时钟控制状态。                         0：时钟关闭；                                 1：时钟使能 */
        unsigned int    tm6_clk_status             : 1; /* [8..8] TIMER6时钟控制状态。                         0：时钟关闭；                                 1：时钟使能 */
        unsigned int    tm7_clk_status             : 1; /* [9..9] TIMER7时钟控制状态。                         0：时钟关闭；                                 1：时钟使能 */
        unsigned int    tm8_clk_status             : 1; /* [10..10] TIMER8时钟控制状态。                         0：时钟关闭；                                 1：时钟使能 */
        unsigned int    tm9_clk_status             : 1; /* [11..11] TIMER9时钟控制状态。                         0：时钟关闭；                                 1：时钟使能 */
        unsigned int    tm10_clk_status            : 1; /* [12..12] TIMER10时钟控制状态。                         0：时钟关闭；                                 1：时钟使能 */
        unsigned int    tm11_clk_status            : 1; /* [13..13] TIMER11时钟控制状态。                         0：时钟关闭；                                 1：时钟使能 */
        unsigned int    tm12_clk_status            : 1; /* [14..14] TIMER12时钟控制状态。                         0：时钟关闭；                                 1：时钟使能 */
        unsigned int    tm13_clk_status            : 1; /* [15..15] TIMER13时钟控制状态。                         0：时钟关闭；                                 1：时钟使能 */
        unsigned int    tm14_clk_status            : 1; /* [16..16] TIMER14时钟控制状态。                         0：时钟关闭；                                 1：时钟使能 */
        unsigned int    tm15_clk_status            : 1; /* [17..17] TIMER15时钟控制状态。                         0：时钟关闭；                                 1：时钟使能 */
        unsigned int    tm16_clk_status            : 1; /* [18..18] TIMER16时钟控制状态。                         0：时钟关闭；                                 1：时钟使能 */
        unsigned int    tm17_clk_status            : 1; /* [19..19] TIMER17时钟控制状态。                         0：时钟关闭；                                 1：时钟使能 */
        unsigned int    tm18_clk_status            : 1; /* [20..20] TIMER18时钟控制状态。                         0：时钟关闭；                                 1：时钟使能 */
        unsigned int    tm19_clk_status            : 1; /* [21..21] TIMER19时钟控制状态。                         0：时钟关闭；                                 1：时钟使能 */
        unsigned int    tm20_clk_status            : 1; /* [22..22] TIMER20时钟控制状态。                         0：时钟关闭；                                 1：时钟使能 */
        unsigned int    tm21_clk_status            : 1; /* [23..23] TIMER21时钟控制状态。                         0：时钟关闭；                                 1：时钟使能 */
        unsigned int    tm22_clk_status            : 1; /* [24..24] TIMER22时钟控制状态。                         0：时钟关闭；                                 1：时钟使能 */
        unsigned int    tm23_clk_status            : 1; /* [25..25] TIMER23时钟控制状态。                         0：时钟关闭；                                 1：时钟使能 */
        unsigned int    appa9_clk_status           : 1; /* [26..26] APPA9电压域时钟控制状态。                         0：时钟关闭；                                 1：时钟使能 */
        unsigned int    mdma9_clk_status           : 1; /* [27..27] MDMA9电压域时钟控制状态。                         0：时钟关闭；                                 1：时钟使能 */
        unsigned int    hifi_vic_clk_status        : 1; /* [28..28] HIFI VIC中断控制器时钟控制状态。                         0：时钟关闭；                                 1：时钟使能 */
        unsigned int    lte_sio_clk_status         : 1; /* [29..29] LTE_SIO 时钟控制状态。                         0：时钟关闭；                                 1：时钟使能 */
        unsigned int    hifi_dbg_clk_status        : 1; /* [30..30] HIFI Debug时钟控制状态。                         0：时钟关闭；                                 1：时钟使能 */
        unsigned int    hifi_clk_status            : 1; /* [31..31] HIFI子系统时钟控制状态。                         0：时钟关闭；                                 1：时钟使能 */
    } bits;
    unsigned int    u32;
}HI_CRG_CLKSTAT3_T;    /* 时钟控制状态3 */

typedef union
{
    struct
    {
        unsigned int    dsp0_pd_clk_en             : 1; /* [0..0] DSP子系统时钟关闭控制。0：无影响；1：时钟使能 */
        unsigned int    dsp0_core_clk_en           : 1; /* [1..1] DSP子系统CORE时钟使能控制。0：无影响；1：时钟使能 */
        unsigned int    dsp0_dbg_clk_en            : 1; /* [2..2] DSP子系统DBG时钟使能控制。0：无影响；1：时钟使能 */
        unsigned int    reserved_1                 : 4; /* [6..3] 保留 */
        unsigned int    guacc_axi_clk_en           : 1; /* [7..7] GUACC互联桥时钟使能。0：无影响；1：时钟使能 */
        unsigned int    cicom0_clk_en              : 1; /* [8..8] CICOM0时钟使能。0：无影响；1：时钟使能 */
        unsigned int    cicom1_clk_en              : 1; /* [9..9] CICOM1时钟使能。0：无影响；1：时钟使能 */
        unsigned int    hdlc_clk_en                : 1; /* [10..10] HDLC时钟使能。0：无影响；1：时钟使能 */
        unsigned int    upacc_clk_en               : 1; /* [11..11] UPACC时钟使能。0：无影响；1：时钟使能 */
        unsigned int    sci0_clk_en                : 1; /* [12..12] SCI0时钟使能控制。0：无影响；1：时钟使能 */
        unsigned int    sci1_clk_en                : 1; /* [13..13] SCI1时钟使能控制。0：无影响；1：时钟使能 */
        unsigned int    uicc_clk_en                : 1; /* [14..14] UICC模块时钟使能控制。0：无影响；1：时钟使能 */
        unsigned int    uart1_clk_en               : 1; /* [15..15] UART1工作时钟使能控制。0：无影响；1：时钟使能 */
        unsigned int    dsp_vic_clk_en             : 1; /* [16..16] DSP0 VIC中断控制器时钟使能控制。0：无影响；1：时钟使能 */
        unsigned int    cipher_clk_en              : 1; /* [17..17] CIPHER中断控制器时钟使能控制。0：无影响；1：时钟使能 */
        unsigned int    gmac_rx_clk_en             : 1; /* [18..18] GMAC接收方向的接收时钟使能控制0:无影响；1:时钟使能 */
        unsigned int    gmac_tx_clk_en             : 1; /* [19..19] GMAC发送方向的发送时钟使能控制0:无影响；1:时钟使能 */
        unsigned int    gmac_gtx_clk_en            : 1; /* [20..20] RGMII接口的发送随路时钟使能控制0:无影响；1:时钟使能 */
        unsigned int    gmac_switch_clk_en         : 1; /* [21..21] GMAC125M时钟使能控制？0：无影响；1：时钟使能 */
        unsigned int    gmac_aclk_en               : 1; /* [22..22] GMAC总线时钟使能控制0：无影响；1：时钟使能 */
        unsigned int    reserved_0                 : 9; /* [31..23] 保留 */
    } bits;
    unsigned int    u32;
}HI_CRG_CLKEN4_T;    /* 时钟使能4(MCPU控制) */

typedef union
{
    struct
    {
        unsigned int    dsp0_pd_clk_dis            : 1; /* [0..0] DSP子系统时钟关闭控制。0：无影响；1：时钟关闭 */
        unsigned int    dsp0_core_clk_dis          : 1; /* [1..1] DSP子系统CORE时钟关闭控制。0：无影响；1：时钟关闭 */
        unsigned int    dsp0_dbg_clk_dis           : 1; /* [2..2] DSP子系统DBG时钟关闭控制。0：无影响；1：时钟关闭 */
        unsigned int    reserved_1                 : 4; /* [6..3] 保留 */
        unsigned int    guacc_axi_clk_dis          : 1; /* [7..7] GUACC互联桥时钟关闭控制。0：无影响；1：时钟关闭 */
        unsigned int    cicom0_clk_dis             : 1; /* [8..8] CICOM0时钟关闭控制。0：无影响；1：时钟关闭 */
        unsigned int    cicom1_clk_dis             : 1; /* [9..9] CICOM1时钟关闭控制。0：无影响；1：时钟关闭 */
        unsigned int    hdlc_clk_dis               : 1; /* [10..10] HDLC时钟关闭控制。0：无影响；1：时钟关闭 */
        unsigned int    upacc_clk_dis              : 1; /* [11..11] UPACC时钟关闭控制。0：无影响；1：时钟关闭 */
        unsigned int    sci0_clk_dis               : 1; /* [12..12] SCI0时钟关闭控制。                            0：无影响；                                  1：时钟关闭 */
        unsigned int    sci1_clk_dis               : 1; /* [13..13] SCI1时钟关闭控制。                            0：无影响；                                  1：时钟关闭 */
        unsigned int    uicc_clk_dis               : 1; /* [14..14] UICC模块时钟关闭控制。                            0：无影响；                                  1：时钟关闭 */
        unsigned int    uart1_clk_dis              : 1; /* [15..15] UART1工作时钟关闭控制。                            0：无影响；                                  1：时钟关闭 */
        unsigned int    dsp_vic_clk_dis            : 1; /* [16..16] DSP0 VIC中断控制器时钟关闭控制。                            0：无影响；                                  1：时钟关闭 */
        unsigned int    cipher_clk_dis             : 1; /* [17..17] CIPHER中断控制器时钟关闭控制。                            0：无影响；                                  1：时钟关闭 */
        unsigned int    gmac_rx_clk_dis            : 1; /* [18..18] GMAC接收方向的接收时钟关闭控制0：无影响；                                  1：时钟关闭 */
        unsigned int    gmac_tx_clk_dis            : 1; /* [19..19] GMAC发送方向的发送时钟关闭控制0：无影响；                                  1：时钟关闭 */
        unsigned int    gmac_gtx_clk_dis           : 1; /* [20..20] RGMII接口的发送随路时钟关闭控制0：无影响；                                  1：时钟关闭 */
        unsigned int    gmac_switch_clk_dis        : 1; /* [21..21] GMAC125M时钟关闭控制？0：无影响；                                  1：时钟关闭 */
        unsigned int    gmac_aclk_dis              : 1; /* [22..22] GMAC总线时钟关闭控制0：无影响；                                  1：时钟关闭 */
        unsigned int    reserved_0                 : 9; /* [31..23] 保留 */
    } bits;
    unsigned int    u32;
}HI_CRG_CLKDIS4_T;    /* 时钟关闭4(MCPU控制) */

typedef union
{
    struct
    {
        unsigned int    dsp0_pd_clk_status         : 1; /* [0..0] DSP子系统时钟控制状态。0：时钟关闭；1：时钟使能 */
        unsigned int    dsp0_core_clk_status       : 1; /* [1..1] DSP子系统CORE时钟控制状态。0：时钟关闭；1：时钟使能 */
        unsigned int    dsp0_dbg_clk_status        : 1; /* [2..2] DSP子系统DBG时钟控制状态。0：时钟关闭；1：时钟使能 */
        unsigned int    reserved_1                 : 4; /* [6..3] 保留 */
        unsigned int    guacc_axi_clk_status       : 1; /* [7..7] GUACC互联桥时钟控制状态。0：时钟关闭；1：时钟使能 */
        unsigned int    cicom0_clk_status          : 1; /* [8..8] CICOM0时钟控制控制状态。0：时钟关闭；1：时钟使能 */
        unsigned int    cicom1_clk_status          : 1; /* [9..9] CICOM1时钟控制控制状态。0：时钟关闭；1：时钟使能 */
        unsigned int    hdlc_clk_status            : 1; /* [10..10] HDLC时钟控制控制状态。0：时钟关闭；1：时钟使能 */
        unsigned int    upacc_clk_status           : 1; /* [11..11] UPACC时钟控制控制状态。0：时钟关闭；1：时钟使能 */
        unsigned int    sci0_clk_status            : 1; /* [12..12] SCI0时钟控制控制状态。0：时钟关闭；1：时钟使能 */
        unsigned int    sci1_clk_status            : 1; /* [13..13] SCI1时钟控制控制状态。0：时钟关闭；1：时钟使能 */
        unsigned int    uicc_clk_status            : 1; /* [14..14] UICC模块时钟控制状态。0：时钟关闭；1：时钟使能 */
        unsigned int    uart1_clk_status           : 1; /* [15..15] UART1工作时钟控制状态。0：时钟关闭；1：时钟使能 */
        unsigned int    dsp_vic_clk_status         : 1; /* [16..16] DSP0 VIC中断控制器时钟控制状态。0：时钟关闭；1：时钟使能 */
        unsigned int    cipher_clk_status          : 1; /* [17..17] CIPHER控制器时钟控制状态。0：时钟关闭；1：时钟使能 */
        unsigned int    gmac_rx_clk_status         : 1; /* [18..18] GMAC接收方向的接收时钟控制状态0：时钟关闭；1：时钟使能 */
        unsigned int    gmac_tx_clk_status         : 1; /* [19..19] GMAC发送方向的发送时钟控制状态0：时钟关闭；1：时钟使能 */
        unsigned int    gmac_gtx_clk_status        : 1; /* [20..20] RGMII接口的发送随路时钟控制状态0：时钟关闭；1：时钟使能 */
        unsigned int    gmac_switch_clk_status     : 1; /* [21..21] GMAC125M时钟控制状态？0：时钟关闭；1：时钟使能 */
        unsigned int    gmac_aclk_status           : 1; /* [22..22] GMAC总线时钟控制状态0：时钟关闭；1：时钟使能 */
        unsigned int    reserved_0                 : 9; /* [31..23] 保留 */
    } bits;
    unsigned int    u32;
}HI_CRG_CLKSTAT4_T;    /* 时钟控制状态4(MCPU控制) */

typedef union
{
    struct
    {
        unsigned int    bbp_ao_clk_en              : 1; /* [0..0] BBP AO区时钟使能控制。1 - 时钟打开0 - 无影响 */
        unsigned int    bbpcommon_2a_clk_en        : 1; /* [1..1] BBPCOMMON模块时钟使能控制。1 - 时钟打开0 - 无影响 */
        unsigned int    ltebbp_pd_clk_en           : 1; /* [2..2] LTE BBP掉电区时钟使能控制。1 - 时钟打开0 - 无影响 */
        unsigned int    g1bbp_pd_clk_en            : 1; /* [3..3] G1 BBP掉电区时钟使能控制。1 - 时钟打开0 - 无影响 */
        unsigned int    g2bbp_pd_clk_en            : 1; /* [4..4] G2 BBP掉电区时钟时钟使能控制。1 - 时钟打开0 - 无影响 */
        unsigned int    twbbp_pd_clk_en            : 1; /* [5..5] TW BBP掉电区时钟使能控制。1 - 时钟打开0 - 无影响 */
        unsigned int    wbbp_pd_clk_en             : 1; /* [6..6] WBBP掉电区时钟使能控制。1 - 时钟打开0 - 无影响 */
        unsigned int    irm_pd_clk_en              : 1; /* [7..7] IRM BBP掉电区时钟时钟使能控制。1 - 时钟打开0 - 无影响 */
        unsigned int    lbbp_axi_clk_en            : 1; /* [8..8] LTE BBP掉电区AXI接口时钟使能控制。1 - 时钟打开0 - 无影响 */
        unsigned int    lbbp_pdt_clk_en            : 1; /* [9..9] LTE BBP掉电区时域部分时钟使能控制。1 - 时钟打开0 - 无影响 */
        unsigned int    lbbp_pdf_clk_en            : 1; /* [10..10] LTE BBP掉电区频域部分时钟使能控制。1 - 时钟打开0 - 无影响 */
        unsigned int    lbbp_tdl_clk_en            : 1; /* [11..11] LTE BBP掉电区TDL相关时钟使能控制。1 - 时钟打开0 - 无影响 */
        unsigned int    lbbp_vdl_clk_en            : 1; /* [12..12] LTE BBP掉电区VDL相关时钟使能控制。1 - 时钟打开0 - 无影响 */
        unsigned int    lbbp_tds_clk_en            : 1; /* [13..13] LTE BBP掉电区频域部分时钟使能控制。1 - 时钟打开0 - 无影响 */
        unsigned int    reserved_1                 : 2; /* [15..14] 保留 */
        unsigned int    irm_turbo_clk_en           : 1; /* [16..16] IRM BBP掉电区 turbo时钟使能控制。1 - 时钟打开0 - 无影响 */
        unsigned int    irm_bbc_245m_clk_en        : 1; /* [17..17] IRM BBP掉电区 245MHz时钟使能控制。1 - 时钟打开0 - 无影响 */
        unsigned int    irm_wbbp_122m_clk_en       : 1; /* [18..18] IRM BBP掉电122MHz时钟使能控制。1 - 时钟打开0 - 无影响 */
        unsigned int    twbbp_wbbp_122m_clk_en     : 1; /* [19..19] WTDS/W BBP掉电区TDS模122MHz时钟使能控制。1 - 时钟打开0 - 无影响 */
        unsigned int    twbbp_wbbp_61m_clk_en      : 1; /* [20..20] TDS/W BBP掉电区 WBBP 61MHz时钟使能控制。1 - 时钟打开0 - 无影响 */
        unsigned int    twbbp_tbbp_122m_clk_en     : 1; /* [21..21] PDTW BBP掉电区tbbp 122MHz时钟使能控制。1 - 时钟打开0 - 无影响 */
        unsigned int    twbbp_tbbp_245m_clk_en     : 1; /* [22..22] PDTW BBP掉电区tbbp 245MHz时钟使能控制。1 - 时钟打开0 - 无影响 */
        unsigned int    wbbp_61m_clk_en            : 1; /* [23..23] PDW BBP掉电区61MHz时钟使能控制。1 - 时钟打开0 - 无影响 */
        unsigned int    wbbp_122m_clk_en           : 1; /* [24..24] PDW BBP掉电区122MHz时钟使能控制。1 - 时钟打开0 - 无影响 */
        unsigned int    wbbp_245m_clk_en           : 1; /* [25..25] PDW BBP掉电区245MHz时钟使能控制。1 - 时钟打开0 - 无影响 */
        unsigned int    wbbp_axi_clk_en            : 1; /* [26..26] PDW BBP掉电区AXI时钟使能控制。1 - 时钟打开0 - 无影响 */
        unsigned int    reserved_0                 : 5; /* [31..27] 保留 */
    } bits;
    unsigned int    u32;
}HI_CRG_CLKEN5_T;    /* 时钟使能5（BBP） */

typedef union
{
    struct
    {
        unsigned int    bbp_ao_clk_dis             : 1; /* [0..0] BBP AO区时钟时钟关闭控制。1 - 时钟关闭0 - 无影响 */
        unsigned int    bbpcommon_clk_dis          : 1; /* [1..1] BBPCOMMON模块时钟时钟关闭控制。1 - 时钟关闭0 - 无影响 */
        unsigned int    ltebbp_pd_clk_dis          : 1; /* [2..2] LTE BBP掉电区时钟时钟关闭控制。1 - 时钟关闭0 - 无影响 */
        unsigned int    g1bbp_pd_clk_dis           : 1; /* [3..3] G1 BBP掉电区时钟时钟关闭控制。1 - 时钟关闭0 - 无影响 */
        unsigned int    g2bbp_pd_clk_dis           : 1; /* [4..4] G2 BBP掉电区时钟时钟关闭控制。1 - 时钟关闭0 - 无影响 */
        unsigned int    twbbp_pd_clk_dis           : 1; /* [5..5] TW BBP掉电区时钟时钟关闭控制。1 - 时钟关闭0 - 无影响 */
        unsigned int    wbbp_pd_clk_dis            : 1; /* [6..6] WBBP掉电区时钟时钟关闭控制。1 - 时钟关闭0 - 无影响 */
        unsigned int    irm_pd_clk_dis             : 1; /* [7..7] IRM BBP掉电区时钟时钟关闭控制。1 - 时钟关闭0 - 无影响 */
        unsigned int    lbbp_axi_clk_dis           : 1; /* [8..8] LTE BBP掉电区AXI接口时钟关闭控制。1 - 时钟关闭0 - 无影响 */
        unsigned int    lbbp_pdt_clk_dis           : 1; /* [9..9] LTE BBP掉电区时域部分时钟关闭控制。1 - 时钟关闭0 - 无影响 */
        unsigned int    lbbp_pdf_clk_dis           : 1; /* [10..10] LTE BBP掉电区频域部分时钟关闭控制。1 - 时钟关闭0 - 无影响 */
        unsigned int    lbbp_tdl_clk_dis           : 1; /* [11..11] LTE BBP掉电区TDL相关时钟关闭控制。1 - 时钟关闭0 - 无影响 */
        unsigned int    lbbp_vdl_clk_dis           : 1; /* [12..12] LTE BBP掉电区VDL相关时钟关闭控制。1 - 时钟关闭0 - 无影响 */
        unsigned int    lbbp_tds_clk_dis           : 1; /* [13..13] LTE BBP掉电区频域部分时钟关闭控制。1 - 时钟关闭0 - 无影响 */
        unsigned int    twbbp_tbbp_clk_dis         : 1; /* [14..14] WTDS/W BBP掉电区TDS模时钟关闭控制。1 - 时钟关闭0 - 无影响 */
        unsigned int    twbbp_wbbp_clk_dis         : 1; /* [15..15] TDS/W BBP掉电区 WBBP时钟关闭控制。1 - 时钟关闭0 - 无影响 */
        unsigned int    irm_turbo_clk_dis          : 1; /* [16..16] IRM BBP掉电区 turbo时钟关闭控制。1 - 时钟关闭0 - 无影响 */
        unsigned int    irm_bbc_245m_clk_dis       : 1; /* [17..17] IRM BBP掉电区 245MHz时钟使能控制。1 - 时钟打开0 - 无影响 */
        unsigned int    irm_wbbp_122m_clk_dis      : 1; /* [18..18] IRM BBP掉电122MHz时钟使能控制。1 - 时钟打开0 - 无影响 */
        unsigned int    pdtw_wbbp_122m_clk_dis     : 1; /* [19..19] PDTW BBP掉电区wbbp 122MHz时钟关闭控制。1 - 时钟关闭0 - 无影响 */
        unsigned int    pdtw_wbbp_61m_clk_dis      : 1; /* [20..20] PDTW BBP掉电区wbbp 61MHz时钟关闭控制。1 - 时钟关闭0 - 无影响 */
        unsigned int    pdtw_tbbp_122m_clk_dis     : 1; /* [21..21] PDTW BBP掉电区tbbp 122MHz时钟关闭控制。1 - 时钟关闭0 - 无影响 */
        unsigned int    pdtw_tbbp_245m_clk_dis     : 1; /* [22..22] PDTW BBP掉电区tbbp 245MHz时钟关闭控制。1 - 时钟关闭0 - 无影响 */
        unsigned int    pdw_61m_clk_dis            : 1; /* [23..23] PDW BBP掉电区61MHz时钟关闭控制。1 - 时钟关闭0 - 无影响 */
        unsigned int    pdw_122m_clk_dis           : 1; /* [24..24] PDW BBP掉电区122MHz时钟关闭控制。1 - 时钟关闭0 - 无影响 */
        unsigned int    pdw_245m_clk_dis           : 1; /* [25..25] PDW BBP掉电区245MHz时钟关闭控制。1 - 时钟关闭0 - 无影响 */
        unsigned int    pdw_axi_clk_dis            : 1; /* [26..26] PDW BBP掉电区AXI时钟关闭控制。1 - 时钟关闭0 - 无影响 */
        unsigned int    reserved                   : 5; /* [31..27] 保留 */
    } bits;
    unsigned int    u32;
}HI_CRG_CLKDIS5_T;    /* 时钟关闭5 */

typedef union
{
    struct
    {
        unsigned int    bbp_ao_clk_stat            : 1; /* [0..0] BBP AO区时钟时钟控制状态。0：时钟关闭；1：时钟使能 */
        unsigned int    bbpcommon_2a_clk_stat      : 1; /* [1..1] BBPCOMMON模块时钟时钟控制状态。0：时钟关闭；1：时钟使能 */
        unsigned int    ltebbp_pd_clk_stat         : 1; /* [2..2] LTE BBP掉电区时钟时钟控制状态。0：时钟关闭；1：时钟使能 */
        unsigned int    g1bbp_pd_clk_stat          : 1; /* [3..3] G1 BBP掉电区时钟时钟控制状态。0：时钟关闭；1：时钟使能 */
        unsigned int    g2bbp_pd_clk_stat          : 1; /* [4..4] G2 BBP掉电区时钟时钟控制状态。0：时钟关闭；1：时钟使能 */
        unsigned int    twbbp_pd_clk_stat          : 1; /* [5..5] TW BBP掉电区时钟时钟控制状态。0：时钟关闭；1：时钟使能 */
        unsigned int    wbbp_pd_clk_stat           : 1; /* [6..6] WBBP掉电区时钟时钟控制状态。0：时钟关闭；1：时钟使能 */
        unsigned int    irm_pd_clk_stat            : 1; /* [7..7] IRM BBP(BBPCOMMON_2B)掉电区时钟时钟控制状态。0：时钟关闭；1：时钟使能 */
        unsigned int    lbbp_axi_clk_stat          : 1; /* [8..8] LTE BBP掉电区AXI接口时钟使能控制。1 - 时钟打开0 - 无影响 */
        unsigned int    lbbp_pdt_clk_stat          : 1; /* [9..9] LTE BBP掉电区时域部分时钟控制状态。0：时钟关闭；1：时钟使能 */
        unsigned int    lbbp_pdf_clk_stat          : 1; /* [10..10] LTE BBP掉电区频域部分时钟控制状态。0：时钟关闭；1：时钟使能 */
        unsigned int    lbbp_tdl_clk_stat          : 1; /* [11..11] LTE BBP掉电区TDL相关时钟控制状态。0：时钟关闭；1：时钟使能 */
        unsigned int    lbbp_vdl_clk_stat          : 1; /* [12..12] LTE BBP掉电区VDL相关时钟控制状态。0：时钟关闭；1：时钟使能 */
        unsigned int    lbbp_tds_clk_stat          : 1; /* [13..13] LTE BBP掉电区tds部分时钟控制状态。0：时钟关闭；1：时钟使能 */
        unsigned int    twbbp_tbbp_clk_stat        : 1; /* [14..14] TDS/W BBP掉电区TDS模时钟控制状态。0：时钟关闭；1：时钟使能 */
        unsigned int    twbbp_wbbp_clk_stat        : 1; /* [15..15] TDS/W BBP掉电区 WBBP模时钟控制状态。0：时钟关闭；1：时钟使能 */
        unsigned int    irm_turbo_clk_stat         : 1; /* [16..16] IRM BBP掉电区 turbo时钟控制状态。0：时钟关闭；1：时钟使能 */
        unsigned int    irm_bbc_245m_clk_status    : 1; /* [17..17] IRM BBP掉电区 245MHz时钟控制状态。0：时钟关闭；1：时钟使能 */
        unsigned int    irm_wbbp_122m_clk_status   : 1; /* [18..18] IRM BBP掉电122MHz时钟控制状态。0：时钟关闭；1：时钟使能 */
        unsigned int    pdtw_wbbp_122m_clk_status  : 1; /* [19..19] PDTW BBP掉电区wbbp 122MHz时钟控制状态。0：时钟关闭；1：时钟使能 */
        unsigned int    pdtw_wbbp_61m_clk_status   : 1; /* [20..20] PDTW BBP掉电区wbbp 61MHz时钟控制状态。0：时钟关闭；1：时钟使能 */
        unsigned int    pdtw_tbbp_122m_clk_status  : 1; /* [21..21] PDTW BBP掉电区tbbp 122MHz时钟控制状态。0：时钟关闭；1：时钟使能 */
        unsigned int    pdtw_tbbp_245m_clk_status  : 1; /* [22..22] PDTW BBP掉电区tbbp 245MHz时钟控制状态。0：时钟关闭；1：时钟使能 */
        unsigned int    pdw_61m_clk_status         : 1; /* [23..23] PDW BBP掉电区61MHz时钟控制状态。0：时钟关闭；1：时钟使能 */
        unsigned int    pdw_122m_clk_status        : 1; /* [24..24] PDW BBP掉电区122MHz时钟控制状态。0：时钟关闭；1：时钟使能 */
        unsigned int    pdw_245m_clk_status        : 1; /* [25..25] PDW BBP掉电区245MHz时钟控制状态。0：时钟关闭；1：时钟使能 */
        unsigned int    pdw_axi_clk_status         : 1; /* [26..26] PDW BBP掉电区AXI时钟控制状态。0：时钟关闭；1：时钟使能 */
        unsigned int    reserved                   : 5; /* [31..27] 保留 */
    } bits;
    unsigned int    u32;
}HI_CRG_CLKSTAT5_T;    /* 时钟控制状态5 */

typedef union
{
    struct
    {
        unsigned int    appa9_cpu_srst_en          : 1; /* [0..0] APPA9 CPU CORE软复位请求使能控制。0：无影响1：复位请求有效 */
        unsigned int    appa9_dbg_srst_en          : 1; /* [1..1] APPA9 Debug软复位请求使能控制。0：无影响1：复位请求有效 */
        unsigned int    appa9_peri_srst_en         : 1; /* [2..2] APPA9外设软复位请求使能控制。0：无影响1：复位请求有效 */
        unsigned int    appa9_scu_srst_en          : 1; /* [3..3] APPA9 SCU软复位请求使能控制。0：无影响1：复位请求有效 */
        unsigned int    appa9_ptm_srst_en          : 1; /* [4..4] APPA9 PTM软复位请求使能控制。0：无影响1：复位请求有效 */
        unsigned int    appa9_wd_srst_en           : 1; /* [5..5] APPA9 WD软复位请求使能控制。0：无影响1：复位请求有效 */
        unsigned int    moda9_cpu_srst_en          : 1; /* [6..6] MODA9 CPU CORE软复位请求使能控制。0：无影响1：复位请求有效 */
        unsigned int    moda9_dbg_srst_en          : 1; /* [7..7] MODA9 Debug软复位请求使能控制。0：无影响1：复位请求有效 */
        unsigned int    moda9_peri_srst_en         : 1; /* [8..8] MODA9 外设软复位请求使能控制。0：无影响1：复位请求有效 */
        unsigned int    moda9_scu_srst_en          : 1; /* [9..9] MODA9 SCU软复位请求使能控制。0：无影响1：复位请求有效 */
        unsigned int    moda9_ptm_srst_en          : 1; /* [10..10] MODA9 PTM软复位请求使能控制。0：无影响1：复位请求有效 */
        unsigned int    moda9_wd_srst_en           : 1; /* [11..11] MODA9 WD软复位请求使能控制。0：无影响1：复位请求有效 */
        unsigned int    hpm_srst_en                : 1; /* [12..12] HPM0-2软复位请求使能控制。0：无影响1：复位请求有效 */
        unsigned int    hpm_clkdiv_srst_en         : 1; /* [13..13] HPM时钟分频器复位使能控制。0：无影响1：复位请求有效 */
        unsigned int    appa9_srst_en              : 1; /* [14..14] APPA9上电复位请求使能控制。0：无影响1：复位请求有效 */
        unsigned int    mdma9_srst_en              : 1; /* [15..15] MDMA9上电复位请求使能控制。0：无影响1：复位请求有效 */
        unsigned int    cm3_dbg_srst_en            : 1; /* [16..16] CM3 Debug复位请求使能控制。0：无影响1：复位请求有效 */
        unsigned int    mddrc_srst_en              : 1; /* [17..17] MDDRC软复位请求使能控制。0：无影响1：复位请求有效 */
        unsigned int    cm3_cpu_srst_en            : 1; /* [18..18] CM3 CORE复位请求使能控制。0：无影响1：复位请求有效 */
        unsigned int    cicom0_srst_en             : 1; /* [19..19] CICOM0软复位请求使能控制。0：无影响1：复位请求有效 */
        unsigned int    cicom1_srst_en             : 1; /* [20..20] CICOM1软复位请求使能控制。0：无影响1：复位请求有效 */
        unsigned int    hdlc_srst_en               : 1; /* [21..21] HDLC软复位请求使能控制。0：无影响1：复位请求有效 */
        unsigned int    upacc_srst_en              : 1; /* [22..22] UPACC软复位请求使能控制。0：无影响1：复位请求有效 */
        unsigned int    edmac_ch4_srst_en          : 1; /* [23..23] EDMAC软复位请求使能控制。0：无影响1：复位请求有效 */
        unsigned int    edmac_srst_en              : 1; /* [24..24] EDMAC软复位请求使能控制。0：无影响1：复位请求有效 */
        unsigned int    emi_srst_en                : 1; /* [25..25] EMI软复位请求使能控制。0：无影响1：复位请求有效 */
        unsigned int    uicc_srst_en               : 1; /* [26..26] UICC软复位请求使能控制。0：无影响1：复位请求有效 */
        unsigned int    reserved                   : 1; /* [27..27] 保留 */
        unsigned int    amon_cpufast_srst_en       : 1; /* [28..28] CPUFAST AMONITOR软复位请求使能控制。0：无影响1：复位请求有效 */
        unsigned int    amon_soc_srst_en           : 1; /* [29..29] SOC AMONITOR软复位请求使能控制。0：无影响1：复位请求有效 */
        unsigned int    dt_srst_en                 : 1; /* [30..30] DT软复位请求请求使能控制。0：无影响1：复位请求有效 */
        unsigned int    sha1_srst_en               : 1; /* [31..31] SHA1软复位请求请求使能控制。0：无影响1：复位请求有效 */
    } bits;
    unsigned int    u32;
}HI_CRG_SRSTEN1_T;    /* 软复位请求使能1 */

typedef union
{
    struct
    {
        unsigned int    appa9_cpu_srst_dis         : 1; /* [0..0] APPA9 CPU CORE软复位请求关闭控制。0：无影响；1：复位请求无效 */
        unsigned int    appa9_dbg_srst_dis         : 1; /* [1..1] APPA9 Debug软复位请求关闭控制。0：无影响；1：复位请求无效 */
        unsigned int    appa9_peri_srst_dis        : 1; /* [2..2] APPA9外设软复位请求关闭控制。0：无影响；1：复位请求无效 */
        unsigned int    appa9_scu_srst_dis         : 1; /* [3..3] APPA9 SCU软复位请求关闭控制。0：无影响；1：复位请求无效 */
        unsigned int    appa9_ptm_srst_dis         : 1; /* [4..4] APPA9 PTM软复位请求关闭控制。0：无影响；1：复位请求无效 */
        unsigned int    appa9_wd_srst_dis          : 1; /* [5..5] APPA9 WD软复位请求关闭控制。0：无影响；1：复位请求无效 */
        unsigned int    moda9_cpu_srst_dis         : 1; /* [6..6] MODA9 CPU CORE软复位请求关闭控制。0：无影响；1：复位请求无效 */
        unsigned int    moda9_dbg_srst_dis         : 1; /* [7..7] MODA9 Debug软复位请求关闭控制。0：无影响；1：复位请求无效 */
        unsigned int    moda9_peri_srst_dis        : 1; /* [8..8] MODA9 外设软复位请求关闭控制。0：无影响；1：复位请求无效 */
        unsigned int    moda9_scu_srst_dis         : 1; /* [9..9] MODA9 SCU软复位请求关闭控制。0：无影响；1：复位请求无效 */
        unsigned int    moda9_ptm_srst_dis         : 1; /* [10..10] MODA9 PTM软复位请求关闭控制。0：无影响；1：复位请求无效 */
        unsigned int    moda9_wd_srst_dis          : 1; /* [11..11] MODA9 WD软复位请求关闭控制。0：无影响；1：复位请求无效 */
        unsigned int    hpm_srst_dis               : 1; /* [12..12] HPM0-2软复位请求关闭控制。0：无影响；1：复位请求无效 */
        unsigned int    hpm_clkdiv_srst_dis        : 1; /* [13..13] HPM时钟分频器复位关闭控制。0：无影响；1：复位请求无效 */
        unsigned int    appa9_srst_dis             : 1; /* [14..14] APPA9上电复位请求关闭控制。0：无影响1：复位请求无效 */
        unsigned int    mdma9_srst_dis             : 1; /* [15..15] MDMA9上电复位请求关闭控制。0：无影响1：复位请求无效 */
        unsigned int    cm3_dbg_srst_dis           : 1; /* [16..16] CM3 Debug复位请求关闭控制。0：无影响1：复位请求无效 */
        unsigned int    mddrc_srst_dis             : 1; /* [17..17] MDDRC软复位请求关闭控制。0：无影响1：复位请求无效 */
        unsigned int    cm3_cpu_srst_dis           : 1; /* [18..18] CM3 COREg复位请求关闭控制。0：无影响1：复位请求无效 */
        unsigned int    cicom0_srst_dis            : 1; /* [19..19] CICOM0软复位请求关闭控制。0：无影响1：复位请求无效 */
        unsigned int    cicom1_srst_dis            : 1; /* [20..20] CICOM1软复位请求关闭控制。0：无影响1：复位请求无效 */
        unsigned int    hdlc_srst_dis              : 1; /* [21..21] HDLC软复位请求关闭控制。0：无影响1：复位请求无效 */
        unsigned int    upacc_srst_dis             : 1; /* [22..22] UPACC软复位请求关闭控制。0：无影响1：复位请求无效 */
        unsigned int    edmac_ch4_srst_dis         : 1; /* [23..23] EDMAC软复位请求关闭控制。0：无影响；1：复位请求无效 */
        unsigned int    edmac_srst_dis             : 1; /* [24..24] EDMAC软复位请求关闭控制。0：无影响；1：复位请求无效 */
        unsigned int    emi_srst_dis               : 1; /* [25..25] EMI软复位请求关闭控制。0：无影响；1：复位请求无效 */
        unsigned int    uicc_srst_dis              : 1; /* [26..26] UICC软复位请求关闭控制。0：无影响1：复位请求无效 */
        unsigned int    reserved                   : 1; /* [27..27] 保留 */
        unsigned int    amon_cpufast_srst_dis      : 1; /* [28..28] CPUFAST AMONITOR软复位请求关闭控制。0：无影响；1：复位请求无效 */
        unsigned int    amon_soc_srst_dis          : 1; /* [29..29] SOC AMONITOR软复位请求关闭控制。0：无影响；1：复位请求无效 */
        unsigned int    dt_srst_dis                : 1; /* [30..30] DT软复位请求关闭控制。0：无影响；1：复位请求无效 */
        unsigned int    sha1_srst_dis              : 1; /* [31..31] SHA1软复位请求关闭控制。0：无影响；1：复位请求无效 */
    } bits;
    unsigned int    u32;
}HI_CRG_SRSTDIS1_T;    /* 软复位请求关闭1 */

typedef union
{
    struct
    {
        unsigned int    appa9_cpu_srst_status      : 1; /* [0..0] APPA9 CPU CORE软复位请求状态控制。0：复位请求无效；1：复位请求有效 */
        unsigned int    appa9_dbg_srst_status      : 1; /* [1..1] APPA9 Debug软复位请求状态控制。0：复位请求无效；1：复位请求有效 */
        unsigned int    appa9_peri_srst_status     : 1; /* [2..2] APPA9外设软复位请求状态控制。0：复位请求无效；1：复位请求有效 */
        unsigned int    appa9_scu_srst_status      : 1; /* [3..3] APPA9 SCU软复位请求状态控制。0：复位请求无效；1：复位请求有效 */
        unsigned int    appa9_ptm_srst_status      : 1; /* [4..4] APPA9 PTM软复位请求状态控制。0：复位请求无效；1：复位请求有效 */
        unsigned int    appa9_wd_srst_status       : 1; /* [5..5] APPA9 WD软复位请求状态控制。0：复位请求无效；1：复位请求有效 */
        unsigned int    moda9_cpu_srst_status      : 1; /* [6..6] MODA9 CPU CORE软复位请求状态控制。0：复位请求无效；1：复位请求有效 */
        unsigned int    moda9_dbg_srst_status      : 1; /* [7..7] MODA9 Debug软复位请求状态控制。0：复位请求无效；1：复位请求有效 */
        unsigned int    moda9_peri_srst_status     : 1; /* [8..8] MODA9 外设软复位请求状态控制。0：复位请求无效；1：复位请求有效 */
        unsigned int    moda9_scu_srst_status      : 1; /* [9..9] MODA9 SCU软复位请求状态控制。0：复位请求无效；1：复位请求有效 */
        unsigned int    moda9_ptm_srst_status      : 1; /* [10..10] MODA9 PTM软复位请求状态控制。0：复位请求无效；1：复位请求有效 */
        unsigned int    moda9_wd_srst_status       : 1; /* [11..11] MODA9 WD软复位请求状态控制。0：复位请求无效；1：复位请求有效 */
        unsigned int    hpm_srst_status            : 1; /* [12..12] HPM0-2软复位请求状态控制。0：复位请求无效；1：复位请求有效 */
        unsigned int    hpm_clkdiv_srst_status     : 1; /* [13..13] HPM时钟分频器复位状态控制。0：复位请求无效；1：复位请求有效 */
        unsigned int    appa9_srst_status          : 1; /* [14..14] APPA9上电复位请求状态控制。0：复位请求无效；1：复位请求有效 */
        unsigned int    mdma9_srst_status          : 1; /* [15..15] MDMA9上电复位请求状态控制。0：复位请求无效；1：复位请求有效 */
        unsigned int    cm3_dbg_srst_status        : 1; /* [16..16] CM3 Debug复位请求状态控制。0：复位请求无效；1：复位请求有效 */
        unsigned int    mddrc_srst_status          : 1; /* [17..17] MDDRC软复位请求状态控制。0：复位请求无效；1：复位请求有效 */
        unsigned int    cm3_core_srst_status       : 1; /* [18..18] CM3 Debug复位请求状态控制。0：复位请求无效；1：复位请求有效 */
        unsigned int    cicom0_srst_status         : 1; /* [19..19] CICOM0软复位请求状态控制。0：复位请求无效 1：复位请求有效 */
        unsigned int    cicom1_srst_status         : 1; /* [20..20] CICOM1软复位请求状态控制。0：复位请求无效 1：复位请求有效 */
        unsigned int    hdlc_srst_status           : 1; /* [21..21] HDLC软复位请求状态控制。0：复位请求无效 1：复位请求有效 */
        unsigned int    upacc_srst_status          : 1; /* [22..22] UPACC软复位请求状态控制。0：复位请求无效 1：复位请求有效 */
        unsigned int    edmac_ch4_srst_status      : 1; /* [23..23] EDMAC软复位请求状态控制。0：复位请求无效；1：复位请求有效 */
        unsigned int    edmac_srst_status          : 1; /* [24..24] EDMAC软复位请求状态控制。0：复位请求无效；1：复位请求有效 */
        unsigned int    emi_srst_status            : 1; /* [25..25] EMI软复位请求状态控制。0：复位请求无效；1：复位请求有效 */
        unsigned int    uicc_srst_status           : 1; /* [26..26] UICC软复位请求状态控制。0：复位请求无效 1：复位请求有效 */
        unsigned int    reserved                   : 1; /* [27..27] 保留 */
        unsigned int    amon_cpufast_srst_status   : 1; /* [28..28] CPUFAST AMONITOR软复位请求状态控制。0：复位请求无效；1：复位请求有效 */
        unsigned int    amon_soc_srst_status       : 1; /* [29..29] SOC AMONITOR软复位请求状态控制。0：复位请求无效；1：复位请求有效 */
        unsigned int    dt_srst_status             : 1; /* [30..30] DT软复位请求状态控制。0：复位请求无效；1：复位请求有效 */
        unsigned int    sha1_srst_status           : 1; /* [31..31] SHA1软复位请求状态控制。0：复位请求无效；1：复位请求有效 */
    } bits;
    unsigned int    u32;
}HI_CRG_SRSTSTAT1_T;    /* 软复位请求状态1 */

typedef union
{
    struct
    {
        unsigned int    tensi_dps0_pd_srst_en      : 1; /* [0..0] Tensi dsp0子系统软复位请求使能控制。0：无影响1：复位请求有效 */
        unsigned int    tensi_dsp0_core_srst_en    : 1; /* [1..1] Tensi dsp0 core软复位请求使能控制。0：无影响1：复位请求有效 */
        unsigned int    tensi_dsp0_dbg_srst_en     : 1; /* [2..2] Tensi dsp0 DBG软复位请求使能控制。0：无影响1：复位请求有效    DSP debug相关逻辑不受bit【0】复位请求控制，DSP下电时，需要配置该bit复位debug相关逻辑 */
        unsigned int    reserved_3                 : 1; /* [3..3] 保留 */
        unsigned int    func_wakeup_pd_srst_en     : 1; /* [4..4] func_wakeup掉电复位请求使能控制。0：无影响1：复位请求有效 */
        unsigned int    soc_peri_usb_pd_srst_en    : 1; /* [5..5] SOC_PERI_USB电压域掉电复位请求使能控制。0：无影响1：复位请求有效 */
        unsigned int    mmc0_srst_en               : 1; /* [6..6] MMC0软复位请求使能控制。0：无影响1：复位请求有效 */
        unsigned int    mmc1_srst_en               : 1; /* [7..7] MMC1软复位请求使能控制。0：无影响1：复位请求有效 */
        unsigned int    sdcc_srst_en               : 1; /* [8..8] SDCC软复位请求使能控制。0：无影响1：复位请求有效 */
        unsigned int    rserved                    : 2; /* [10..9] 保留 */
        unsigned int    gmac_srst_req              : 1; /* [11..11] GMAC软复位请求使能控制。0：无影响1：复位请求有效 */
        unsigned int    reserved_2                 : 2; /* [13..12] 保留。 */
        unsigned int    pcie_ctrl_srst_req         : 1; /* [14..14] PCIE控制器软复位请求使能控制。0：无影响1：复位请求有效 */
        unsigned int    pcie_phy_srst_req          : 1; /* [15..15] PCIE PHY软复位请求使能控制。0：无影响1：复位请求有效 */
        unsigned int    ddrphy_div_srst_req        : 1; /* [16..16] ddrphy ctl clk分频器软复位请求使能控制。0：无影响1：复位请求有效 */
        unsigned int    reserved_1                 : 1; /* [17..17] 保留 */
        unsigned int    hifi_core_srst_en          : 1; /* [18..18] HIFI DSP CORE软复位请求使能控制。0：无影响1：复位请求有效 */
        unsigned int    hifi_pd_srst_en            : 1; /* [19..19] HIFI 除DBG外的软复位请求使能控制。0：无影响1：复位请求有效默认HIFI子系统复位，时钟时能。 */
        unsigned int    hifi_dbg_srst_en           : 1; /* [20..20] HIFI DBG软复位请求使能控制。0：无影响1：复位请求有效HIFI debug相关逻辑不受bit【0】控制，HIFI子系统下电时，需要配置该bit复位debug相关逻辑。 */
        unsigned int    usbotg_bc_srst_en          : 1; /* [21..21] USB_BC控制器软复位请求使能控制。0：无影响1：复位请求有效 */
        unsigned int    usbotg_phy_srst_en         : 1; /* [22..22] USBOTG PHY软复位请求使能控制。0：无影响1：复位请求有效 */
        unsigned int    usbotg_ctrl_srst_en        : 1; /* [23..23] USBOTG ctrl软复位请求使能控制。0：无影响1：复位请求有效 */
        unsigned int    usbotg_ctrl_vaux_srst_en   : 1; /* [24..24] USBOTG VAUX控制器软复位请求使能控制。0：无影响1：复位请求有效 */
        unsigned int    usbotg_ctrl_vcc_srst_en    : 1; /* [25..25] USBOTG VCC控制器软复位请求使能控制。0：无影响1：复位请求有效 */
        unsigned int    usbhsic_ctrl_srst_en       : 1; /* [26..26] USBHSIC控制器软复位请求使能控制。0：无影响1：复位请求有效 */
        unsigned int    usbhsic_phy_srst_en        : 1; /* [27..27] USBHSIC PHY软复位请求使能控制。0：无影响1：复位请求有效 */
        unsigned int    cs_srst_en                 : 1; /* [28..28] CS软复位请求使能控制。0：无影响1：复位请求有效 */
        unsigned int    tsensor_srst_en            : 1; /* [29..29] TSENSOR软复位请求使能控制。0：无影响1：复位请求有效 */
        unsigned int    reserved_0                 : 2; /* [31..30] 保留 */
    } bits;
    unsigned int    u32;
}HI_CRG_SRSTEN2_T;    /* 软复位请求使能2 */

typedef union
{
    struct
    {
        unsigned int    tensi_dps0_pd_srst_dis     : 1; /* [0..0] Tensi dsp0子系统软复位请求关闭控制。0：无影响1：复位请求无效 */
        unsigned int    tensi_dsp0_core_srst_dis   : 1; /* [1..1] Tensi dsp0 core软复位请求关闭控制。0：无影响1：复位请求无效 */
        unsigned int    tensi_dsp0_dbg_srst_dis    : 1; /* [2..2] Tensi dsp0 DBG软复位请求关闭控制。0：无影响1：复位请求无效 */
        unsigned int    reserved_3                 : 1; /* [3..3] 保留 */
        unsigned int    func_wakeup_pd_srst_dis    : 1; /* [4..4] func_wakeup掉电复位请求关闭控制。0：无影响1：复位请求无效 */
        unsigned int    soc_peri_usb_pd_srst_dis   : 1; /* [5..5] SOC_PERI_USB电压域掉电复位请求关闭控制。0：无影响1：复位请求无效 */
        unsigned int    mmc0_srst_dis              : 1; /* [6..6] MMC0软复位请求关闭控制。0：无影响1：复位请求无效 */
        unsigned int    mmc1_srst_dis              : 1; /* [7..7] MMC1软复位请求使关闭控制。0：无影响1：复位请求无效 */
        unsigned int    sdcc_srst_dis              : 1; /* [8..8] SDCC软复位请求关闭控制。0：无影响1：复位请求无效 */
        unsigned int    reserved_2                 : 2; /* [10..9] 保留 */
        unsigned int    gmac_srst_dis              : 1; /* [11..11] GMAC软复位请求关闭控制。0：无影响1：复位请求无效 */
        unsigned int    ssicdev_ctrl_srst_dis      : 1; /* [12..12] SSIC Device控制器软复位请求关闭控制。0：无影响1：复位请求无效 */
        unsigned int    ssicphy_srst_dis           : 1; /* [13..13] SSIC PHY软复位请求关闭控制。0：无影响1：复位请求无效 */
        unsigned int    pcie_ctrl_srst_dis         : 1; /* [14..14] PCIE控制器软复位请求关闭控制。0：无影响1：复位请求无效 */
        unsigned int    pcie_phy_srst_dis          : 1; /* [15..15] PCIE PHY软复位请求关闭控制。0：无影响1：复位请求无效 */
        unsigned int    ddrphy_div_srst_dis        : 1; /* [16..16] ddrphy ctl clk分频器软复位请求使能控制。0：无影响1：复位请求无效 */
        unsigned int    reserved_1                 : 1; /* [17..17] 保留 */
        unsigned int    hifi_core_srst_dis         : 1; /* [18..18] HIFI DSP CORE的软复位请求使能控制。0：无影响1：复位请求有效 */
        unsigned int    hifi_pd_srst_dis           : 1; /* [19..19] HIFI 除DBG外的软复位请求使能控制。0：无影响1：复位请求有效 */
        unsigned int    hifi_dbg_srst_dis          : 1; /* [20..20] HIFI DBG软复位请求使能控制。0：无影响1：复位请求有效 */
        unsigned int    usbotg_bc_srst_dis         : 1; /* [21..21] USB_BC控制器软复位请求关闭控制。0：无影响1：复位请求无效 */
        unsigned int    usbotg_phy_srst_dis        : 1; /* [22..22] USBOTG PHY软复位请求关闭控制。0：无影响1：复位请求无效 */
        unsigned int    usbotg_ctrl_srst_dis       : 1; /* [23..23] USBOTG ctrl软复位请求关闭控制。0：无影响1：复位请求无效 */
        unsigned int    usbotg_ctrl_vaux_srst_dis  : 1; /* [24..24] USBOTG VAUX控制器软复位请求关闭控制。0：无影响1：复位请求无效 */
        unsigned int    usbotg_ctrl_vcc_srst_dis   : 1; /* [25..25] USBOTG VCC控制器软复位请求关闭控制。0：无影响1：复位请求无效 */
        unsigned int    usbhsic_ctrl_srst_dis      : 1; /* [26..26] USBHSIC控制器软复位请求关闭控制。0：无影响1：复位请求无效 */
        unsigned int    usbhsic_phy_srst_dis       : 1; /* [27..27] USBHSIC PHY软复位请求关闭控制。0：无影响1：复位请求无效 */
        unsigned int    cs_srst_dis                : 1; /* [28..28] Coresight软复位请求关闭控制。0：无影响；1：复位请求无效 */
        unsigned int    tsensor_srst_dis           : 1; /* [29..29] TSENSOR软复位请求关闭控制。0：无影响1：复位请求无效 */
        unsigned int    reserved_0                 : 2; /* [31..30] 保留 */
    } bits;
    unsigned int    u32;
}HI_CRG_SRSTDIS2_T;    /* 软复位请求关闭2 */

typedef union
{
    struct
    {
        unsigned int    tensi_dps0_pd_srst_status  : 1; /* [0..0] Tensi dsp0子系统软复位请求状态控制。0：复位请求无效 1：复位请求有效 */
        unsigned int    tensi_dsp0_core_srst_status : 1; /* [1..1] Tensi dsp0 core软复位请求状态控制。0：复位请求无效 1：复位请求有效 */
        unsigned int    tensi_dsp0_dbg_srst_status : 1; /* [2..2] Tensi dsp0 DBG软复位请求状态控制。0：复位请求无效 1：复位请求有效 */
        unsigned int    reserved_3                 : 1; /* [3..3] 保留 */
        unsigned int    func_wakeup_pd_srst_status : 1; /* [4..4] func_wakeup掉电复位请求状态控制。0：复位请求无效1：复位请求有效 */
        unsigned int    soc_peri_usb_pd_srst_status : 1; /* [5..5] SOC_PERI_USB掉电复位请求状态控制。0：复位请求无效1：复位请求有效 */
        unsigned int    mmc0_srst_status           : 1; /* [6..6] MMC0软复位请求状态控制。0：复位请求无效1：复位请求有效 */
        unsigned int    mmc1_srst_status           : 1; /* [7..7] MMC1软复位请求状态控制。0：复位请求无效1：复位请求有效 */
        unsigned int    sdcc_srst_status           : 1; /* [8..8] SDCC软复位请求状态控制。0：复位请求无效1：复位请求有效 */
        unsigned int    reserved_2                 : 2; /* [10..9] 保留 */
        unsigned int    gmac_srst_status           : 1; /* [11..11] GMAC软复位请求状态控制。0：复位请求无效1：复位请求有效 */
        unsigned int    ssicdev_ctrl_srst_status   : 1; /* [12..12] SSIC Device控制器软复位请求状态控制。0：复位请求无效1：复位请求有效 */
        unsigned int    ssicphy_srst_status        : 1; /* [13..13] SSIC PHY软复位请求状态控制。0：复位请求无效1：复位请求有效 */
        unsigned int    pcie_ctrl_srst_status      : 1; /* [14..14] PCIE控制器软复位请求请求状态控制。0：复位请求无效1：复位请求有效 */
        unsigned int    pcie_phy_srst_status       : 1; /* [15..15] PCIE PHY软复位请求请求状态控制。0：复位请求无效1：复位请求有效 */
        unsigned int    ddrphy_div_srst_status     : 1; /* [16..16] ddrphy ctl clk分频器软复位请求状态控制。0：复位请求无效1：复位请求有效 */
        unsigned int    reserved_1                 : 1; /* [17..17] 保留 */
        unsigned int    hificore_srst_status       : 1; /* [18..18] HIFI DSP CORE软复位请求状态控制。0：复位请求无效1：复位请求有效 */
        unsigned int    hifi_pd_srst_status        : 1; /* [19..19] HIFI 除DBG外的软复位请求状态控制。0：复位请求无效1：复位请求有效 */
        unsigned int    hifi_dbg_srst_status       : 1; /* [20..20] HIFI DBG软复位请求状态控制。0：复位请求无效1：复位请求有效 */
        unsigned int    usbotg_bc_srst_status      : 1; /* [21..21] USB_BC控制器软复位请求状态控制。0：复位请求无效1：复位请求有效 */
        unsigned int    usbotg_phy_srst_status     : 1; /* [22..22] USBOTG PHY软复位请求状态控制。0：复位请求无效1：复位请求有效 */
        unsigned int    usbotg_ctrl_srst_status    : 1; /* [23..23] USBOTG ctrl软复位请求状态控制。0：复位请求无效1：复位请求有效 */
        unsigned int    usbotg_ctrl_vaux_srst_status : 1; /* [24..24] USBOTG VAUX控制器软复位请求状态控制。0：复位请求无效1：复位请求有效 */
        unsigned int    usbotg_ctrl_vcc_srst_status : 1; /* [25..25] USBOTG VCC控制器软复位请求状态控制。0：复位请求无效1：复位请求有效 */
        unsigned int    usbhsic_ctrl_srst_status   : 1; /* [26..26] USBHSIC控制器软复位请求状态控制。0：复位请求无效1：复位请求有效 */
        unsigned int    usbhsic_phy_srst_v         : 1; /* [27..27] USBHSIC PHY软复位请求状态控制。0：复位请求无效1：复位请求有效 */
        unsigned int    cs_srst_status             : 1; /* [28..28] Coresight软复位请求状态控制。0：复位请求无效；1：复位请求有效 */
        unsigned int    tsensor_srst_status        : 1; /* [29..29] TSENSOR软复位请求状态控制。0：复位请求无效1：复位请求有效 */
        unsigned int    reserved_0                 : 2; /* [31..30] 保留 */
    } bits;
    unsigned int    u32;
}HI_CRG_SRSTSTAT2_T;    /* 软复位请求状态2 */

typedef union
{
    struct
    {
        unsigned int    bbp_crg_srst_en            : 1; /* [0..0] BBP时钟复位模块软复位请求使能控制。0 - 无影响1 - 复位有效 */
        unsigned int    bbpcommon_2a_srst_en       : 1; /* [1..1] BBPCOMMON_2A软复位请求使能控制。0 - 无影响1 - 复位有效 */
        unsigned int    lbbp_pd_srst_en            : 1; /* [2..2] LTEBBP整个掉电区掉电软复位请求使能控制。0 - 无影响1 - 复位有效 */
        unsigned int    g1bbp_pd_srst_en           : 1; /* [3..3] G1BBP掉电区掉电软复位请求使能控制。0 - 无影响1 - 复位有效 */
        unsigned int    g2bbp_pd_srst_en           : 1; /* [4..4] G2BBP掉电区掉电软复位请求使能控制。0 - 无影响1 - 复位有效 */
        unsigned int    twbbp_pd_srst_en           : 1; /* [5..5] TWBBP整个掉电区掉电软复位请求使能控制。0 - 无影响1 - 复位有效 */
        unsigned int    wbbp_pd_srst_en            : 1; /* [6..6] WBBP整个掉电区掉电软复位请求使能控制。0 - 无影响1 - 复位有效 */
        unsigned int    irm_pd_srst_en             : 1; /* [7..7] IRM掉电区掉电软复位请求使能控制。0 - 无影响1 - 复位有效 */
        unsigned int    bbc_lbbp_pdt_srst_en       : 1; /* [8..8] BBPCOMMON_2A LBBP PDT软复位源控制。0 - 无影响1 - 该bit置1 */
        unsigned int    bbc_lbbp_pdf_srst_en       : 1; /* [9..9] BBPCOMMON_2A LBBP PDF软复位源控制。0 - 无影响1 - 该bit置1 */
        unsigned int    bbc_lbbp_tdl_srst_en       : 1; /* [10..10] BBPCOMMON_2A LBBP TDL软复位源控制。0 - 无影响1 - 该bit置1 */
        unsigned int    bbc_tbbp_245m_srst_en      : 1; /* [11..11] BBPCOMMON_2A TBBP 245MHz部分逻辑软复位源控制。0 - 无影响1 - 该bit置1 */
        unsigned int    bbc_twbbp_122m_srst_en     : 1; /* [12..12] BBPCOMMON_2A TWBBP 122MHz部分逻辑软复位源控制。0 - 无影响1 - 该bit置1 */
        unsigned int    bbc_g1bbp_104m_srst_en     : 1; /* [13..13] BBPCOMMON_2A G1BBP 104MHz部分逻辑软复位源控制。0 - 无影响1 - 该bit置1 */
        unsigned int    bbc_g2bbp_104m_srst_en     : 1; /* [14..14] BBPCOMMON_2A G2BBP 104MHz部分逻辑软复位源控制。0 - 无影响1 - 该bit置1 */
        unsigned int    reserved                   : 16; /* [30..15] 保留 */
        unsigned int    abb_srst_en                : 1; /* [31..31] ABB复位请求使能控制。0 - 无影响1 - 复位有效 */
    } bits;
    unsigned int    u32;
}HI_CRG_SRSTEN3_T;    /* 软复位请求使能3（ABB+BBP） */

typedef union
{
    struct
    {
        unsigned int    bbp_crg_srst_dis           : 1; /* [0..0] BBP时钟复位模块软复位请求关闭控制。0 - 无影响1 - 复位无效。 */
        unsigned int    bbpcommon_2a_pd_srst_dis   : 1; /* [1..1] BBPCOMMON_2A软复位请求关闭控制。0 - 无影响1 - 复位无效。 */
        unsigned int    lbbp_pd_srst_dis           : 1; /* [2..2] LTEBBP整个掉电区掉电软复位请求关闭控制。0 - 无影响1 - 复位无效。 */
        unsigned int    g1bbp_pd_srst_dis          : 1; /* [3..3] G1BBP掉电区掉电软复位请求关闭控制。0 - 无影响1 - 复位无效。 */
        unsigned int    g2bbp_pd_srst_dis          : 1; /* [4..4] G2BBP掉电区掉电软复位请求关闭控制。0 - 无影响1 - 复位无效。 */
        unsigned int    twbbp_pd_srst_dis          : 1; /* [5..5] TWBBP整个掉电区掉电软复位请求关闭控制。0 - 无影响1 - 复位无效。 */
        unsigned int    wbbp_pd_srst_dis           : 1; /* [6..6] WBBP整个掉电区掉电软复位请求关闭控制。0 - 无影响1 - 复位无效。 */
        unsigned int    irm_pd_srst_dis            : 1; /* [7..7] IRM掉电区掉电软复位请求关闭控制。0 - 无影响1 - 复位无效。 */
        unsigned int    bbc_lbbp_pdt_srst_dis      : 1; /* [8..8] BBPCOMMON_2A LBBP PDT软复位控制。0 - 无影响1 - 该bit清0 */
        unsigned int    bbc_lbbp_pdf_srst_dis      : 1; /* [9..9] BBPCOMMON_2A LBBP PDF软复位请求软复位控制。0 - 无影响1 - 该bit清0 */
        unsigned int    bbc_lbbp_tdl_srst_dis      : 1; /* [10..10] BBPCOMMON_2A LBBP TDL软复位请求软复位控制。0 - 无影响1 - 该bit清0 */
        unsigned int    bbc_tbbp_245m_srst_dis     : 1; /* [11..11] BBPCOMMON_2A TBBP 245MHz部分逻辑软复位控制。0 - 无影响1 - 该bit清0 */
        unsigned int    bbc_twbbp_122m_srst_dis    : 1; /* [12..12] BBPCOMMON_2A TWBBP 122MHz部分逻辑软复位控制。0 - 无影响1 - 该bit清0 */
        unsigned int    bbc_g1bbp_104m_srst_dis    : 1; /* [13..13] BBPCOMMON_2A G1BBP 104MHz部分逻辑软复位控制。0 - 无影响1 - 该bit清0 */
        unsigned int    bbc_g2bbp_104m_srst_dis    : 1; /* [14..14] BBPCOMMON_2A G2BBP 104MHz部分逻辑软复位控制。0 - 无影响1 - 该bit清0 */
        unsigned int    reserved                   : 16; /* [30..15] 保留 */
        unsigned int    abb_srst_dis               : 1; /* [31..31] ABB复位请求关闭控制。0 - 无影响1 - 复位有效 */
    } bits;
    unsigned int    u32;
}HI_CRG_SRSTDIS3_T;    /* 软复位请求关闭3（ABB+BBP） */

typedef union
{
    struct
    {
        unsigned int    bbp_crg_srst_stat          : 1; /* [0..0] BBP时钟复位模块软复位请求状态控制。0 - 复位无效1 - 复位有效 */
        unsigned int    bbpcommon_2a_srst_stat     : 1; /* [1..1] BBPCOMMON_2A软复位请求状态控制。0 - 复位无效1 - 复位有效 */
        unsigned int    lbbp_pd_srst_stat          : 1; /* [2..2] LTEBBP整个掉电区掉电软复位请求状态控制。0 - 复位无效1 - 复位有效 */
        unsigned int    g1bbp_pd_srst_stat         : 1; /* [3..3] G1BBP掉电区掉电软复位请求状态控制。0 - 复位无效1 - 复位有效 */
        unsigned int    g2bbp_pd_srst_stat         : 1; /* [4..4] G2BBP掉电区掉电软复位请求状态控制。0 - 复位无效1 - 复位有效 */
        unsigned int    twbbp_pd_srst_stat         : 1; /* [5..5] TWBBP整个掉电区掉电软复位请求状态控制。0 - 复位无效1 - 复位有效 */
        unsigned int    wbbp_pd_srst_stat          : 1; /* [6..6] WBBP整个掉电区掉电软复位请求状态控制。0 - 复位无效1 - 复位有效 */
        unsigned int    irm_pd_srst_stat           : 1; /* [7..7] IRM掉电区掉电软复位请求状态控制。0 - 复位无效1 - 复位有效 */
        unsigned int    bbc_lbbp_pdt_srstctrl_stat : 1; /* [8..8] BBPCOMMON_2A LBBP PDT软复位请求控制。0 - 复位受LTEBBP电压域软复位控制；1 - 复位受BBPCOMMON_2A软复位控制。 */
        unsigned int    bbc_lbbp_pdf_srstctrl_stat : 1; /* [9..9] BBPCOMMON_2A LBBP PDF软复位请求软复位控制。0 - 复位受LTEBBP电压域软复位控制；1 - 复位受BBPCOMMON_2A软复位控制。 */
        unsigned int    bbc_lbbp_tdl_srstctrl_stat : 1; /* [10..10] BBPCOMMON_2A LBBP TDL软复位请求软复位请求控制。0 - 复位受LTEBBP电压域软复位控制；1 - 复位受BBPCOMMON_2A软复位控制。 */
        unsigned int    bbc_tbbp_245m_srstctrl_stat : 1; /* [11..11] BBPCOMMON_2A TBBP 245MHz部分逻辑软复位请求控制。0 - 复位受TWEBBP电压域软复位控制；1 - 复位受BBPCOMMON_2A软复位控制。 */
        unsigned int    bbc_twbbp_122m_srstctrl_stat : 1; /* [12..12] BBPCOMMON_2A TWBBP 122MHz部分逻辑软复位请求控制。0 - 复位受TWEBBP电压域软复位控制；1 - 复位受BBPCOMMON_2A软复位控制。 */
        unsigned int    bbc_g1bbp_104m_srstctrl_stat : 1; /* [13..13] BBPCOMMON_2A G1BBP 104MHz部分逻辑软复位请求控制。0 - 复位受G1EBBP电压域软复位控制；1 - 复位受BBPCOMMON_2A软复位控制。 */
        unsigned int    bbc_g2bbp_104m_srstctrl_stat : 1; /* [14..14] BBPCOMMON_2A G2BBP 104MHz部分逻辑软复位软复位请求控制。0 - 复位受G2EBBP电压域软复位控制；1 - 复位受BBPCOMMON_2A软复位控制。 */
        unsigned int    reserved                   : 16; /* [30..15] 保留 */
        unsigned int    abb_srst_status            : 1; /* [31..31] ABB复位请求状态控制。0 - 复位无效1 - 复位有效 */
    } bits;
    unsigned int    u32;
}HI_CRG_SRSTSTAT3_T;    /* 软复位请求状态3（ABB+BBP） */

typedef union
{
    struct
    {
        unsigned int    pcm_bclk_div               : 16; /* [15..0] sio_bclk分频系数。 */
        unsigned int    pcm_sync_div               : 12; /* [27..16] sio_sync分频系数。 */
        unsigned int    reserved                   : 4; /* [31..28] 保留 */
    } bits;
    unsigned int    u32;
}HI_CRG_CLKDIV1_T;    /* 时钟分频控制1(SIO PCM时钟分频） */

typedef union
{
    struct
    {
        unsigned int    appa9_clk_div              : 4; /* [3..0] app A9分频系数。只要满足慢速总线及快速总线的频率比，可以是0-15的任何值；app A9最高600MHz，即A9_PLL postdiv时钟被此分频系数分频后的频率。默认A9PLL POSTDIV输出时钟频率为600MHz */
        unsigned int    mdma9_clk_div              : 4; /* [7..4] modem A9分频系数。只要满足慢速总线及快速总线的频率比，可以是0-15的任何值；modem A9最高600MHz，即A9_PLL postdiv时钟被此分频系数分频后的频率。默认A9PLL POSTDIV输出时钟频率为600MHz */
        unsigned int    soc_clk_div                : 4; /* [11..8] 总线分频系数。N - N+1分频。只要满足和A9及快速总线的频率比，可以是1-16的任何值；slow_aclk=a9pll_postdiv/(slow_clk_div+1)慢速总线最高150MHz具体支持的profile见CRG规格表 */
        unsigned int    fast_clk_div               : 4; /* [15..12] 快速总线分频系数。N - N+1分频。只要满足和A9及慢速总线的频率比，可以是1-16的任何分频；fast_aclk=a9pll_postdiv/(fast_clk_div+1)快速总线最高300MHz当bit24=0,（DDRPHY PLL bypass模式）时，该域必须配置为偶数。 */
        unsigned int    reserved_2                 : 8; /* [23..16] 保留 */
        unsigned int    ddr_rate_sel               : 1; /* [24..24] DDR速率。0：DDR速率<667MHz；1：DDR速率=667MHz。只有当DDR速率<667MHz时，DDRPHY的2倍频时钟才会打开。当该bit配置为0，则fast_clk_div必须是奇数，即偶偶数分频才能正常工作。 */
        unsigned int    reserved_1                 : 5; /* [29..25] 保留 */
        unsigned int    apb_freqmode               : 1; /* [30..30] APB总线与SlowBus总线分频比配置。0 - 1:11 - 2:1 */
        unsigned int    reserved_0                 : 1; /* [31..31] 保留 */
    } bits;
    unsigned int    u32;
}HI_CRG_CLKDIV2_T;    /* 时钟分频控制2(总线分频） */

typedef union
{
    struct
    {
        unsigned int    mmc0_cclk_div              : 5; /* [4..0] MMC0卡接口时钟分频系数，0 - 1分频；N - N+1分频默认4分频，N=3 */
        unsigned int    reserved_3                 : 2; /* [6..5] 保留 */
        unsigned int    mmc0_refclk_sel            : 1; /* [7..7] 0 - HIFIPLL（800MHz)1 - PERIPLL_480MHz */
        unsigned int    mmc0_phase_clk_div         : 3; /* [10..8] MMC0控制器调相时钟分频系数，0 - 1分频；N - N+1分频默认2分频，N=1 */
        unsigned int    reserved_2                 : 1; /* [11..11] 保留 */
        unsigned int    mmc1_cclk_div              : 5; /* [16..12] MMC1卡接口时钟分频系数，0 - 1分频；N - N+1分频默认2分频，N=1 */
        unsigned int    reserved_1                 : 2; /* [18..17] 保留 */
        unsigned int    mmc1_refclk_sel            : 1; /* [19..19] 0 - HIFIPLL800MHz)1 - PERIPLL480MHz */
        unsigned int    mmc1_phase_clk_div         : 3; /* [22..20] MMC1控制器调相时钟分频系数，0 - 1分频；N - N+1分频默认2分频，N=1 */
        unsigned int    reserved_0                 : 1; /* [23..23] 保留 */
        unsigned int    hpm_clk_div                : 4; /* [27..24] HPM工作时钟分频系数。0 - 1分频；N - N分频。 */
        unsigned int    cipher_clk_div             : 4; /* [31..28] cipher工作时钟分频系数。分频器的输入参考时钟为480MHz。N：N分频；（默认配置为5分频）cipher_clk=960MHz/5=96MHz. */
    } bits;
    unsigned int    u32;
}HI_CRG_CLKDIV3_T;    /* 时钟分频控制3(MMC0/1) */

typedef union
{
    struct
    {
        unsigned int    hifi_clk_div_num           : 4; /* [3..0] HIFI CORE时钟分频系数。HIFI CORE最高工作频率为300MHz。0 - 1分频N - N+1分频 */
        unsigned int    hifi_freqmode              : 2; /* [5..4] HIFI CORE和总线分频比。最高工作频率为150MHz0 - 1:11 - 2:1(default)2 - 3:13 - 4:1 */
        unsigned int    hifi_refclk_sel            : 2; /* [7..6] HIFI输入参考时钟选择。?0 - DSPPLL_FOUTPOST(300MHz)?1 = PERIPLL_FOUTPOST(960MHz）当不需要工作在最高性能时，可将参考时钟切换到PERIPLL的输出，同时关闭HIFIPLL从而节省功耗。切换流程：HIFIPLL -》 PERIPLL1. 配置hifi_clk_div_num到合适的分频系数2. 配置hifi_refclk_sel切换到期望的输入参考时钟3. 关闭HIFIPLL切换流程：PERIPLL -》 HIFIPLL1. 如果HIFIPLL处于PD状态，使能PLL2. 配置hifi_clk_div_num到合适的分频系数3. 配置hifi_refclk_sel切换到期望的输入参考时钟 */
        unsigned int    reserved                   : 24; /* [31..8] 保留 */
    } bits;
    unsigned int    u32;
}HI_CRG_CLKDIV4_T;    /* 时钟分频控制4(HIFI时钟分频) */

typedef union
{
    struct
    {
        unsigned int    bbe_clk_div_num            : 3; /* [2..0] BBE16 CORE时钟分频系数。最高工作频率为3000MHz。0 - 1分频N - N+1分频 */
        unsigned int    reserved_2                 : 1; /* [3..3] 保留 */
        unsigned int    bbe_freqmode               : 2; /* [5..4] DSP CORE和总线分频比。总线的最高工作频率为150MHz.0 - 1:11 - 2:1(default)10 -3:111- 4:1 */
        unsigned int    bbe_refclk_sel             : 2; /* [7..6] DSP输入参考时钟选择。?0 - DSPPLL_FOUTPOST(420MHz)?1 - PERIPLL_FOUTPOST(960MHz）当不需要工作在最高性能时，可将BBE的参考时钟切换到PERIPLL的输出。切换流程：1. 配置bbe_clk_div_num到合适的分频系数2. 配置bbe_refclk_sel切换到期望的输入参考时钟 */
        unsigned int    sci0_clk_div_num           : 3; /* [10..8] SCI0工作时钟分频系数。SCI的最高频率为60MHz时钟源PERIPLL_FOUT4=120MHzN - N分频，支持2~7分频。 */
        unsigned int    reserved_1                 : 1; /* [11..11] 保留 */
        unsigned int    sci1_clk_div_num           : 3; /* [14..12] SCI1工作时钟分频系数。SCI的最高频率为60MHz时钟源PERIPLL_FOUT4=120MHzN - N分频，只支持2~7分频。 */
        unsigned int    reserved_0                 : 17; /* [31..15] 保留 */
    } bits;
    unsigned int    u32;
}HI_CRG_CLKDIV5_T;    /* 时钟分频控制5(BBE/SCI时钟分频) */

typedef union
{
    struct
    {
        unsigned int    mmc0_drv_sel               : 7; /* [6..0] mmc0_drv_clk调相控制。Bit[6:5] - delay chain select00 - no delay chain01 - 1st delay10 - 2nd delay11 - 3rd delaybit[4:0] - phase shift step select.MMC0只支持8级调相steps，bit[4；3]必须固定配置为0。drv时钟=cclk 移相45*bit[2:0]度 */
        unsigned int    mmc0_clk_bypass            : 1; /* [7..7] mmc0_drv_clk/mmc_sampe_clk时钟源选择信号。1：mmc0_drv_clk时钟为mmc_cclk反向时钟；sample采用PAD反馈时钟。0：mmc0_drv_clk和mmc_sample_clk为mmc0_clk调相后时钟 */
        unsigned int    mmc0_sample_sel            : 7; /* [14..8] mmc0_sample_clk调相控制。Bit[14:13] - delay chain select00 - no delay chain01 - 1st delay10 - 2nd delay11 - 3rd delaybit[12:8] - phase shift step select.MMC0只支持8级调相steps，bit[12:11]必须固定配置为0。Sample时钟=cclk 移相45*bit[10:8]度此时phase_clk是cclk的2倍频 */
        unsigned int    mmc0_sample_ctrl           : 1; /* [15..15] MMC0采样sample后数据的时钟源选择信号。0：采用mmc_cclk_in的正向时钟；1：采用mmc_cclk_in的反向时钟。 */
        unsigned int    mmc1_drv_sel               : 7; /* [22..16] mmc1_drv_clk调相控制。Bit[22:21] - delay chain select00 - no delay chain01 - 1st delay10 - 2nd delay11 - 3rd delaybit[20:16] - phase shift step select.MMC0只支持16级调相steps，bit[20]必须固定配置为0。drv时钟=cclk 移相45*bit[19:16]度. */
        unsigned int    mmc1_clk_bypass            : 1; /* [23..23] mmc_drv_clk/mmc_sampe_clk时钟源选择信号。1：mmc_drv_clk时钟为mmc_cclk反向时钟；sample采用PAD反馈时钟。0：mmc_drv_clk和mmc_sample_clk为mmc_clk调相后时钟 */
        unsigned int    mmc1_sample_sel            : 7; /* [30..24] mmc1_sample_clk调相控制。Bit[30:29] - delay chain select00 - no delay chain01 - frist delay10 - 2nd delay11 - third delaybit[28:24] - phase shift step select,MC1只支持16级调相steps，bit[28]必须固定配置为0。Sample时钟=cclk 移相22.5*bit[10:8]度.此时phase_clk是cclk的4倍频。 */
        unsigned int    mmc1_sample_ctrl           : 1; /* [31..31] MMC1采样sample后数据的时钟源选择信号。0：采用mmc_cclk_in的正向时钟；1：采用mmc_cclk_in的反向时钟。 */
    } bits;
    unsigned int    u32;
}HI_CRG_MMC_CTRL_T;    /* MMC0/1调相控制 */

typedef union
{
    struct
    {
        unsigned int    pll_lock_ctrl              : 16; /* [15..0] 逻辑自动使能PLL输出时钟等待时钟周期数(tcxo_clk）。该配置域在pll_lock_mode=0时有意义。 */
        unsigned int    pll_clk_dis_cfg            : 15; /* [30..16] 软件方式pll输出时钟时能控制(pll_lock_mode=1）。[16] - dsppll foutpost clk[17] - hifipll_foutpost_clk[18] - 保留。[19] - peripll_foutpost_clk[20] - peripll_fout1_clk[21] - peripll_fout3_clk[22] - peripll_fout4_clk[23] - bbppll_foutpost_clk[24] - hifipll_fout1_clk[25] - hifipll_fout3_clk[26] - hifipll_fout4_clk[27] - peripll_fout2_clk[28] - gmacpll_foutpost_clk[29] - gmacpll_fout2_clk[30] - bbppll_fout2_clkPLL默认为PD和BYPASS模式，因此，只有*_FOUTPOST时钟有输出，其他时钟为0。配置流程 - 打开时钟1. 关闭pll_clk_en_cfg对应bit2. 配置PLL，等待PLL稳定（lock指示信号有效）；3. 配置pll_clk_en_cfg对应bit，打开时钟配置流程 - 关闭时钟1. 关闭pll_clk_en_cfg对应bit关闭时钟：2. 先配置该域对应bits关闭时钟，再配置PLL下电关钟 */
        unsigned int    pll_lock_mode              : 1; /* [31..31] pll锁定控制方式。0：pll锁定后自动打开时钟；1：pll锁定后，需要控制打开时钟； */
    } bits;
    unsigned int    u32;
}HI_CRG_PLLLOCK_CTRL_T;    /* pll锁定控制寄存器 */

typedef union
{
    struct
    {
        unsigned int    soc_tcxo_refclk_sel        : 1; /* [0..0] SOC TCXO时钟选择。动态切换，切换时必须保证都是时钟。0 - TCXO0 CLK1 - TCXO1 CLK切换需要在非Normal模式下完成,否则可能会导致PLL失锁，导致芯片工作异常。 */
        unsigned int    pmu_tcxo_clk_sel           : 1; /* [1..1] PMU TCXO时钟选择。静态切换0 - TCXO0 CLK1 - TCXO1 CLK */
        unsigned int    gps_tcxo_clk_sel           : 1; /* [2..2] GPS TCXO时钟选择。静态切换0 - TCXO0 CLK1 - TCXO1 CLK */
        unsigned int    wifi_tcxo_clk_sel          : 1; /* [3..3] WIFI TCXO时钟选择。静态切换0 - TCXO0 CLK1 - TCXO1 CLK */
        unsigned int    sleep_clk_sel              : 1; /* [4..4] sleep时钟选择。静态切换，在使用sleep时钟之前配置切换。0 - PAD sleep_clk1 - TCXO分频后得到的时钟 */
        unsigned int    uart0_clk_sel              : 1; /* [5..5] UART0工作时钟选择。动态切换，切换时必须保证都是时钟。0 - 48MHz1 - 32.768KHz切换流程：切换时，48MHz和32KHz时钟必须都有时钟输出，否则切换不成功。1.确认高频时钟打开：需要配置PERIPLL和PLLLOCK_CTRL寄存器，2. 配置时钟切换。 */
        unsigned int    sio_clk_sel                : 1; /* [6..6] SIO外部接口时钟极性选择。0 - 选择同向时钟（默认配置）1 - 选择反向时钟 */
        unsigned int    cs_clk_sel                 : 1; /* [7..7] coresight子系统时钟源选择。0 - 选择19.2MHz1 - 选择PERIPLL输出时钟切换流程：1.确认高频时钟打开：需要配置PERIPLL和PLLLOCK_CTRL寄存器，2. 配置时钟切换。 */
        unsigned int    a9pll_clk_sel              : 2; /* [9..8] A9高频时钟源选择。01 - A9PLL输出时钟；10 - PERIPLL输出时钟；其他配置不支持 */
        unsigned int    reserved                   : 2; /* [11..10] 保留 */
        unsigned int    pwm0_clk_sel               : 1; /* [12..12] PWM0工作时钟选择控制。静态切换。0 - 19.2MHz1 - 32KHz */
        unsigned int    pwm1_clk_sel               : 1; /* [13..13] PWM1工作时钟选择控制。静态切换。0 - 19.2MHz1 - 32KHz */
        unsigned int    tcssi0_clk_sel             : 1; /* [14..14] TCSSI0时钟选择。静态切换。0 - TCXO0 CLK1 - TCXO1 CLK */
        unsigned int    tcssi1_clk_sel             : 1; /* [15..15] TCSSI1时钟选择。静态切换。0 - TCXO0 CLK1 - TCXO1 CLK */
        unsigned int    ushsic_utmi_clk_dis        : 1; /* [16..16] UTMI时钟使能控制。该bit在HSIC低功耗时使用，和bit17一起使用。0 - 打开控制器的UTMI时钟；1 - 关闭控制器的UTMI时钟。当UTMI时钟切换时，首先通过bit16关闭时钟，切换完成后打开时钟。 */
        unsigned int    usbhsic_utmi_clk_sel       : 1; /* [17..17] HSIC控制器UTMI接口时钟选择。0 - 选择PHY UTMI时钟；1 - 选择32.768KHz时钟 */
        unsigned int    hpm_refclk_sel             : 2; /* [19..18] HPM输入参考时钟选择。00 - A9PLL01 - DSPPLL10 - DSPPLL11 - ABB输出245.76MHz时钟 */
        unsigned int    a9_wd_srst_en              : 1; /* [20..20] A9 WDT复位控制。0 - A9 WDT复位有效；1 - A9 WDT复位无效。 */
        unsigned int    gmac_rx_clk_sel            : 1; /* [21..21] GMAC接收方向的接收时钟选择。0 - 正向时钟1 - 反向时钟 */
        unsigned int    gmac_tx_clk_sel            : 1; /* [22..22] GMAC发送方向的发送时钟选择。0 - 正向时钟1 - 反向时钟 */
        unsigned int    gmac_rtx_clk_sel           : 1; /* [23..23] RGMII接口发送随路时钟选择。0 - 正向时钟1 - 反向时钟 */
        unsigned int    usbphy_refclk_sel          : 2; /* [25..24] USBPHY参考时钟选择。0？- USB_TCXO_CLK1？- TCXO0_CLK10 - TCXO0 CLK11 - TCXO1 CLK */
        unsigned int    peripll_refclk_sel         : 2; /* [27..26] PERIPLL参考时钟选择。0？- USB_TCXO_CLK1？- TCXO0_CLK10 - TCXO0 CLK11 - TCXO1 CLK */
        unsigned int    cs_refclk_sel              : 1; /* [28..28] CS_AT_CLK参考时钟选择0 - peripll_fout3_clk(160MHz)1 - peripll_fout4_clk(120MHz) */
        unsigned int    mmc1_refclk_sel            : 1; /* [29..29] MMC1参考时钟选择0 - PERIPLL480MHz1 - DSPPLL300MHz */
        unsigned int    sio_refclk_sel             : 1; /* [30..30] SIOmaster模式参考时钟选择。0 - 选择245.76M（默认配置）1 - 选择TCXO时钟 */
        unsigned int    mmc0_refclk_sel            : 1; /* [31..31] MMC0参考时钟选择0 - PERIPLL480MHz1 - DSPPLL300MHz */
    } bits;
    unsigned int    u32;
}HI_CRG_CLK_SEL1_T;    /* 时钟源选择控制1 */

typedef union
{
    struct
    {
        unsigned int    timer0_6_clk_sel           : 7; /* [6..0] TIMER0-6工作时钟源选择，静态切换。1 - 选择19.2MHz时钟；0 - 选择32KHz时钟Note：切换时钟时，最好先关闭对应的时钟输出，然后切换时钟，切换完毕后打开时钟 */
        unsigned int    reserved_3                 : 1; /* [7..7] 保留 */
        unsigned int    timer8_14_clk_sel          : 7; /* [14..8] TIMER8_14工作时钟源选择，静态切换。1 - 选择19.2MHz时钟；0 - 选择32KHz时钟 */
        unsigned int    reserved_2                 : 1; /* [15..15] 保留 */
        unsigned int    timer16_22_clk_sel         : 7; /* [22..16] TIMER16_22工作时钟源选择，静态切换。1 - 选择19.2MHz时钟；0 - 选择32KHz时钟 */
        unsigned int    reserved_1                 : 1; /* [23..23] 保留 */
        unsigned int    timer7_clk_sel             : 2; /* [25..24] TIMER7工作时钟源选择，静态切换。1？ - PERIPLL FOUT4/2(60MHz)00 - 选择32KHz时钟01 - 选择19.2MHz时钟； */
        unsigned int    timer15_clk_sel            : 2; /* [27..26] TIMER15工作时钟源选择，静态切换。1？ - PERIPLL FOUT4/2(60MHz)00 - 选择32KHz时钟01 - 选择19.2MHz时钟； */
        unsigned int    timer23_clk_sel            : 2; /* [29..28] TIMER23工作时钟源选择，静态切换。1？- PERIPLL FOUT3/N(160MHz)00 - 选择32KHz时钟01 - 选择19.2MHz时钟； */
        unsigned int    reserved_0                 : 2; /* [31..30] 保留 */
    } bits;
    unsigned int    u32;
}HI_CRG_CLK_SEL2_T;    /* 时钟源选择控制2(TIMER) */

typedef union
{
    struct
    {
        unsigned int    abb_tcxo_clk_sel           : 1; /* [0..0] ABB TCXO时钟源选择。0 - TCXO0时钟；1 - TCXO1时钟 */
        unsigned int    bbppll_refclk_sel          : 1; /* [1..1] BBPPLL参考时钟源选择。静态切换。0 - ABB通道0 TCXO时钟；1 - ABB通道1 TCXO时钟 */
        unsigned int    bbp_tcxo_clk_sel           : 1; /* [2..2] BBP 19.2MHz时钟源选择。静态切换。0 - ABB通道0 TCXO时钟；1 - ABB通道1 TCXO时钟 */
        unsigned int    ch0_tcxo_clk_sel           : 1; /* [3..3] 通道0 19.2MHz时钟选择。静态切换。0 - ABB通道0 TCXO时钟；1 - ABB通道1 TCXO时钟 */
        unsigned int    ch1_tcxo_clk_sel           : 1; /* [4..4] 通道1 19.2MHz时钟选择。静态切换。0 - ABB通道0 TCXO时钟；1 - ABB通道1 TCXO时钟 */
        unsigned int    g1bbp_104m_clk_sel         : 1; /* [5..5] G1BBP通道选择。静态切换。0 - 选择通道0；1 - 选择通道1 */
        unsigned int    g2bbp_104m_clk_sel         : 1; /* [6..6] G2BBP通道选择。静态切换。0 - 选择通道0；1 - 选择通道1 */
        unsigned int    tdl_clk_sel                : 1; /* [7..7] TDL时钟源选择。0 - 122MHz/245MHz；1 - 245MHz/491MHz */
        unsigned int    lbbp_vdl_clk_sel           : 1; /* [8..8] VDL时钟源选择。0 - 122MHz；1 - 245MHz */
        unsigned int    lbbp_pdf_clk_sel           : 1; /* [9..9] LTEBBP 频域时钟源选择。0 - 122MHz；1 - 245MHz */
        unsigned int    reserved                   : 22; /* [31..10] 保留 */
    } bits;
    unsigned int    u32;
}HI_CRG_CLK_SEL3_T;    /* 时钟源选择控制3（BBP） */

typedef union
{
    struct
    {
        unsigned int    dfs_frac                   : 24; /* [23..0] 频率合成器倍频小数部分。 */
        unsigned int    reserved                   : 8; /* [31..24] 保留 */
    } bits;
    unsigned int    u32;
}HI_CRG_DFS1_CTRL1_T;    /* A9 DFS配置1（1.2GHz) */

typedef union
{
    struct
    {
        unsigned int    dfs_intprog                : 12; /* [11..0] 频率合成器倍频整数部分。 */
        unsigned int    dfs_refdiv                 : 6; /* [17..12] 频率合成器输出参考时钟分频系数。 */
        unsigned int    reserved_2                 : 2; /* [19..18] 保留 */
        unsigned int    dfs_postdiv2               : 3; /* [22..20] VCO二级分频系数。 */
        unsigned int    reserved_1                 : 1; /* [23..23] 保留 */
        unsigned int    dfs_postdiv1               : 3; /* [26..24] VCO一级分频系数。 */
        unsigned int    reserved_0                 : 5; /* [31..27] 保留 */
    } bits;
    unsigned int    u32;
}HI_CRG_DFS1_CTRL2_T;    /* A9 DFS配置2 */

typedef union
{
    struct
    {
        unsigned int    pd                         : 1; /* [0..0] DFS工作模式。0：正常工作模式；1：Power Down模式。 */
        unsigned int    reserved_1                 : 1; /* [1..1] 保留。 */
        unsigned int    dsmpd                      : 1; /* [2..2] 分数分频工作模式。0：使能分数分频；1：禁止分数分频。 */
        unsigned int    phase4_pd                  : 1; /* [3..3] 4种相位时钟输出使能。0：使能输出；1：禁止输出。 */
        unsigned int    postdivpd                  : 1; /* [4..4] POSTDIV时钟输出使能。0：使能输出；1：禁止输出。 */
        unsigned int    vcopd                      : 1; /* [5..5] VCO输出使能。0：使能VCO输出；1：禁止VCO输出。 */
        unsigned int    dacpd                      : 1; /* [6..6] Cancellation DAC power down。正常工作模式，该位应该一直保持为1'b0 */
        unsigned int    bypass                     : 1; /* [7..7] DFS旁路控制。0：正常操作fOUT=fOUT；1：旁路fOUT=fREF。 */
        unsigned int    pll_ctrl                   : 1; /* [8..8] ARM PLL控制选择。0：由逻辑自动控制ARM PLL的PD状态；1：由软件控制ARM PLL工作状态，配合pd使用 */
        unsigned int    reserved_0                 : 22; /* [30..9] 保留。 */
        unsigned int    lock                       : 1; /* [31..31] DFS锁定指示，高有效。0：失锁；1：锁定。 */
    } bits;
    unsigned int    u32;
}HI_CRG_DFS1_CTRL3_T;    /* A9 DFS配置3 */

typedef union
{
    struct
    {
        unsigned int    dfs_frac                   : 24; /* [23..0] 频率合成器倍频小数部分。 */
        unsigned int    reserved                   : 8; /* [31..24] 保留 */
    } bits;
    unsigned int    u32;
}HI_CRG_DFS2_CTRL1_T;    /* DSP DFS配置1(1200MHz) */

typedef union
{
    struct
    {
        unsigned int    dfs_intprog                : 12; /* [11..0] 频率合成器倍频整数部分。 */
        unsigned int    dfs_refdiv                 : 6; /* [17..12] 频率合成器输出参考时钟分频系数。 */
        unsigned int    reserved_2                 : 2; /* [19..18] 保留 */
        unsigned int    dfs_postdiv2               : 3; /* [22..20] VCO二级分频系数。 */
        unsigned int    reserved_1                 : 1; /* [23..23] 保留 */
        unsigned int    dfs_postdiv1               : 3; /* [26..24] VCO一级分频系数。 */
        unsigned int    reserved_0                 : 5; /* [31..27] 保留 */
    } bits;
    unsigned int    u32;
}HI_CRG_DFS2_CTRL2_T;    /* DSP DFS配置2 */

typedef union
{
    struct
    {
        unsigned int    pd                         : 1; /* [0..0] DFS工作模式。0：正常工作模式；1：Power Down模式。 */
        unsigned int    reserved_1                 : 1; /* [1..1] 保留。 */
        unsigned int    dsmpd                      : 1; /* [2..2] 分数分频工作模式。0：使能分数分频；1：禁止分数分频。 */
        unsigned int    phase4_pd                  : 1; /* [3..3] 4种相位时钟输出使能。0：使能输出；1：禁止输出。 */
        unsigned int    postdivpd                  : 1; /* [4..4] POSTDIV时钟输出使能。0：使能输出；1：禁止输出。 */
        unsigned int    vcopd                      : 1; /* [5..5] VCO输出使能。0：使能VCO输出；1：禁止VCO输出。 */
        unsigned int    dacpd                      : 1; /* [6..6] Cancellation DAC power down。正常工作模式，该位应该一直保持为1'b0 */
        unsigned int    bypass                     : 1; /* [7..7] DFS旁路控制。0：正常操作fOUT=fOUT；1：旁路fOUT=fREF。 */
        unsigned int    reserved_0                 : 23; /* [30..8] 保留。 */
        unsigned int    lock                       : 1; /* [31..31] DFS锁定指示，高有效。0：失锁；1：锁定。 */
    } bits;
    unsigned int    u32;
}HI_CRG_DFS2_CTRL3_T;    /* DSP DFS配置3 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 32; /* [31..0] 保留 */
    } bits;
    unsigned int    u32;
}HI_CRG_DFS3_CTRL1_T;    /* HIFI DFS配置1（1.6GHz) */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 32; /* [31..0] 保留 */
    } bits;
    unsigned int    u32;
}HI_CRG_DFS3_CTRL2_T;    /* HIFI DFS配置2 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 32; /* [31..0] DFS锁定指示，高有效。0：失锁；1：锁定。 */
    } bits;
    unsigned int    u32;
}HI_CRG_DFS3_CTRL3_T;    /* HIFI DFS配置3 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 32; /* [31..0] 保留 */
    } bits;
    unsigned int    u32;
}HI_CRG_DFS4_CTRL1_T;    /* BBP DFS配置1(491.52MHz) */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 32; /* [31..0] 保留 */
    } bits;
    unsigned int    u32;
}HI_CRG_DFS4_CTRL2_T;    /* BBP DFS配置2 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 32; /* [31..0] DFS锁定指示，高有效。0：失锁；1：锁定。 */
    } bits;
    unsigned int    u32;
}HI_CRG_DFS4_CTRL3_T;    /* BBP DFS配置3 */

typedef union
{
    struct
    {
        unsigned int    dfs_frac                   : 24; /* [23..0] 频率合成器倍频小数部分。 */
        unsigned int    reserved                   : 8; /* [31..24] 保留 */
    } bits;
    unsigned int    u32;
}HI_CRG_DFS5_CTRL1_T;    /* PERI DFS配置1(960MHz) */

typedef union
{
    struct
    {
        unsigned int    dfs_intprog                : 12; /* [11..0] 频率合成器倍频整数部分。 */
        unsigned int    dfs_refdiv                 : 6; /* [17..12] 频率合成器输出参考时钟分频系数。 */
        unsigned int    reserved_2                 : 2; /* [19..18] 保留 */
        unsigned int    dfs_postdiv2               : 3; /* [22..20] VCO二级分频系数。 */
        unsigned int    reserved_1                 : 1; /* [23..23] 保留 */
        unsigned int    dfs_postdiv1               : 3; /* [26..24] VCO一级分频系数。 */
        unsigned int    reserved_0                 : 5; /* [31..27] 保留 */
    } bits;
    unsigned int    u32;
}HI_CRG_DFS5_CTRL2_T;    /* PERI DFS配置2 */

typedef union
{
    struct
    {
        unsigned int    pd                         : 1; /* [0..0] DFS工作模式。0：正常工作模式；1：Power Down模式。 */
        unsigned int    reserved_1                 : 1; /* [1..1] 保留。 */
        unsigned int    dsmpd                      : 1; /* [2..2] 分数分频工作模式。0：使能分数分频；1：禁止分数分频。 */
        unsigned int    phase4_pd                  : 1; /* [3..3] 4种相位时钟输出使能。0：使能输出；1：禁止输出。 */
        unsigned int    postdivpd                  : 1; /* [4..4] POSTDIV时钟输出使能。0：使能输出；1：禁止输出。 */
        unsigned int    vcopd                      : 1; /* [5..5] VCO输出使能。0：使能VCO输出；1：禁止VCO输出。 */
        unsigned int    dacpd                      : 1; /* [6..6] Cancellation DAC power down。正常工作模式，该位应该一直保持为1'b0 */
        unsigned int    bypass                     : 1; /* [7..7] DFS旁路控制。0：正常操作fOUT=fOUT；1：旁路fOUT=fREF。 */
        unsigned int    pll_ctrl                   : 1; /* [8..8] PLL控制选择。0：由逻辑自动控制PLL的PD状态；1：由软件控制PLL工作状态，配合pd使用 */
        unsigned int    reserved_0                 : 22; /* [30..9] 保留。 */
        unsigned int    lock                       : 1; /* [31..31] DFS锁定指示，高有效。0：失锁；1：锁定。 */
    } bits;
    unsigned int    u32;
}HI_CRG_DFS5_CTRL3_T;    /* PERI DFS配置3 */

typedef union
{
    struct
    {
        unsigned int    dfs_frac                   : 24; /* [23..0] 频率合成器倍频小数部分。 */
        unsigned int    reserved                   : 8; /* [31..24] 保留 */
    } bits;
    unsigned int    u32;
}HI_CRG_DFS6_CTRL1_T;    /* PERI USB DFS配置1(500MHz) */

typedef union
{
    struct
    {
        unsigned int    dfs_intprog                : 12; /* [11..0] 频率合成器倍频整数部分。 */
        unsigned int    dfs_refdiv                 : 6; /* [17..12] 频率合成器输出参考时钟分频系数。 */
        unsigned int    reserved_2                 : 2; /* [19..18] 保留 */
        unsigned int    dfs_postdiv2               : 3; /* [22..20] VCO二级分频系数。 */
        unsigned int    reserved_1                 : 1; /* [23..23] 保留 */
        unsigned int    dfs_postdiv1               : 3; /* [26..24] VCO一级分频系数。 */
        unsigned int    reserved_0                 : 5; /* [31..27] 保留 */
    } bits;
    unsigned int    u32;
}HI_CRG_DFS6_CTRL2_T;    /* PERI USB DFS配置2 */

typedef union
{
    struct
    {
        unsigned int    pd                         : 1; /* [0..0] DFS工作模式。0：正常工作模式；1：Power Down模式。 */
        unsigned int    reserved_1                 : 1; /* [1..1] 保留。 */
        unsigned int    dsmpd                      : 1; /* [2..2] 分数分频工作模式。0：使能分数分频；1：禁止分数分频。 */
        unsigned int    phase4_pd                  : 1; /* [3..3] 4种相位时钟输出使能。0：使能输出；1：禁止输出。 */
        unsigned int    postdivpd                  : 1; /* [4..4] POSTDIV时钟输出使能。0：使能输出；1：禁止输出。 */
        unsigned int    vcopd                      : 1; /* [5..5] VCO输出使能。0：使能VCO输出；1：禁止VCO输出。 */
        unsigned int    dacpd                      : 1; /* [6..6] Cancellation DAC power down。正常工作模式，该位应该一直保持为1'b0 */
        unsigned int    bypass                     : 1; /* [7..7] DFS旁路控制。0：正常操作fOUT=fOUT；1：旁路fOUT=fREF。 */
        unsigned int    pll_ctrl                   : 1; /* [8..8] PLL控制选择。0：由逻辑自动控制PLL的PD状态；1：由软件控制PLL工作状态，配合pd使用 */
        unsigned int    reserved_0                 : 22; /* [30..9] 保留。 */
        unsigned int    lock                       : 1; /* [31..31] DFS锁定指示，高有效。0：失锁；1：锁定。 */
    } bits;
    unsigned int    u32;
}HI_CRG_DFS6_CTRL3_T;    /* PERI USB DFS配置3 */

typedef union
{
    struct
    {
        unsigned int    uart0_clksw_stat           : 2; /* [1..0] UART0时钟切换状态。                           00：时钟未切换完成01: 输出48MHz时钟10: 输出32.768KHzX时钟 11：无意义 */
        unsigned int    socpll_refclksw_stat       : 2; /* [3..2] PLL输入参考时钟切换状态。00：时钟未切换完成；瞬间状态。01：选择TCXO0时钟；10: 选择TCXO1时钟。11：无意义 */
        unsigned int    dsp_ref_clksw_stat         : 2; /* [5..4] DSP输入参考时钟切换状态。00：时钟未切换完成；瞬间状态。01：选择DSPPLL_FOUTPOST时钟；10: 选择PERIPLL_FOUTPOST时钟。11：无意义 */
        unsigned int    hifi_ref_clksw_stat        : 2; /* [7..6] HIFI输入参考时钟切换状态。00：时钟未切换完成；瞬间状态。01：选择HIFIPLL_FOUTPOST时钟；10: 选择PERIPLL_FOUTPOST时钟。11：无意义 */
        unsigned int    pwm0_clksw_stat            : 2; /* [9..8] PWM0输入参考时钟切换状态。00：时钟未切换完成；瞬间状态。01：选择TCXO时钟；10: 选择32.768KHz时钟。11：无意义 */
        unsigned int    pwm1_clksw_stat            : 2; /* [11..10] PWM1输入参考时钟切换状态。00：时钟未切换完成；瞬间状态。01：选择TCXO时钟；10: 选择32.768KHz时钟。11：无意义 */
        unsigned int    ssic_pa_ref_clksw_stat     : 2; /* [13..12] SSIC控制器ssic_pa_clk输入参考时钟切换状态。00：时钟未切换完成；瞬间状态。01：选择HIFIPLL_FOUT3时钟（此时HIFIPLL_POSTDIV=750MHz）；10: 选择DSPPLL_FOUTPOST时钟。11：无意义 */
        unsigned int    a9_pll_clksw_stat          : 3; /* [16..14] A9 高频输入参考时钟切换状态。00：时钟未切换完成；瞬间状态。01：选择A9PLL_FOUTPOST时钟；10: 选择PERIPLL_FOUT1时钟(480MHz)。11：无意义 */
        unsigned int    reserved                   : 15; /* [31..17] 保留 */
    } bits;
    unsigned int    u32;
}HI_CRG_CLKSW_STAT1_T;    /* 时钟切换状态1 */

typedef union
{
    struct
    {
        unsigned int    cm3_clk_div                : 4; /* [3..0] HPM工作时钟分频系数。N -  2~15N - N分频。默认2分频（时钟源peripll_fout2 240MHz） */
        unsigned int    reserved                   : 28; /* [31..4] 保留 */
    } bits;
    unsigned int    u32;
}HI_CRG_CLKDIV6_T;    /* 时钟分频控制6（CM3时钟分频） */


/********************************************************************************/
/*    syscrg 函数（项目名_模块名_寄存器名_成员名_set)        */
/********************************************************************************/
HI_SET_GET(hi_crg_clken1_mst_aclk_en,mst_aclk_en,HI_CRG_CLKEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN1_OFFSET)
HI_SET_GET(hi_crg_clken1_dsp_aclk_en,dsp_aclk_en,HI_CRG_CLKEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN1_OFFSET)
HI_SET_GET(hi_crg_clken1_soc_hclk_en,soc_hclk_en,HI_CRG_CLKEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN1_OFFSET)
HI_SET_GET(hi_crg_clken1_amon_cpufast_clk_en,amon_cpufast_clk_en,HI_CRG_CLKEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN1_OFFSET)
HI_SET_GET(hi_crg_clken1_amon_soc_clk_en,amon_soc_clk_en,HI_CRG_CLKEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN1_OFFSET)
HI_SET_GET(hi_crg_clken1_tsensor_clk_en,tsensor_clk_en,HI_CRG_CLKEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN1_OFFSET)
HI_SET_GET(hi_crg_clken1_reserved_2,reserved_2,HI_CRG_CLKEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN1_OFFSET)
HI_SET_GET(hi_crg_clken1_ipf_clk_en,ipf_clk_en,HI_CRG_CLKEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN1_OFFSET)
HI_SET_GET(hi_crg_clken1_reserved_1,reserved_1,HI_CRG_CLKEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN1_OFFSET)
HI_SET_GET(hi_crg_clken1_edmac_ch4_clk_en,edmac_ch4_clk_en,HI_CRG_CLKEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN1_OFFSET)
HI_SET_GET(hi_crg_clken1_socp_clk_en,socp_clk_en,HI_CRG_CLKEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN1_OFFSET)
HI_SET_GET(hi_crg_clken1_mipi_clk_en,mipi_clk_en,HI_CRG_CLKEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN1_OFFSET)
HI_SET_GET(hi_crg_clken1_pmussi0_clk_en,pmussi0_clk_en,HI_CRG_CLKEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN1_OFFSET)
HI_SET_GET(hi_crg_clken1_pmussi1_clk_en,pmussi1_clk_en,HI_CRG_CLKEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN1_OFFSET)
HI_SET_GET(hi_crg_clken1_dw_ssi0_clk_en,dw_ssi0_clk_en,HI_CRG_CLKEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN1_OFFSET)
HI_SET_GET(hi_crg_clken1_efuse_clk_en,efuse_clk_en,HI_CRG_CLKEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN1_OFFSET)
HI_SET_GET(hi_crg_clken1_nandc_clk_en,nandc_clk_en,HI_CRG_CLKEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN1_OFFSET)
HI_SET_GET(hi_crg_clken1_hs_uart_clk_en,hs_uart_clk_en,HI_CRG_CLKEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN1_OFFSET)
HI_SET_GET(hi_crg_clken1_emi_clk_en,emi_clk_en,HI_CRG_CLKEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN1_OFFSET)
HI_SET_GET(hi_crg_clken1_edmac_clk_en,edmac_clk_en,HI_CRG_CLKEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN1_OFFSET)
HI_SET_GET(hi_crg_clken1_bootrom_clk_en,bootrom_clk_en,HI_CRG_CLKEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN1_OFFSET)
HI_SET_GET(hi_crg_clken1_pwm1_clk_en,pwm1_clk_en,HI_CRG_CLKEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN1_OFFSET)
HI_SET_GET(hi_crg_clken1_pwm0_clk_en,pwm0_clk_en,HI_CRG_CLKEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN1_OFFSET)
HI_SET_GET(hi_crg_clken1_dt_clk_en,dt_clk_en,HI_CRG_CLKEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN1_OFFSET)
HI_SET_GET(hi_crg_clken1_ipcm_clk_en,ipcm_clk_en,HI_CRG_CLKEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN1_OFFSET)
HI_SET_GET(hi_crg_clken1_dw_ssi1_clk_en,dw_ssi1_clk_en,HI_CRG_CLKEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN1_OFFSET)
HI_SET_GET(hi_crg_clken1_i2c1_clk_en,i2c1_clk_en,HI_CRG_CLKEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN1_OFFSET)
HI_SET_GET(hi_crg_clken1_gpio1_clk_en,gpio1_clk_en,HI_CRG_CLKEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN1_OFFSET)
HI_SET_GET(hi_crg_clken1_i2c0_clk_en,i2c0_clk_en,HI_CRG_CLKEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN1_OFFSET)
HI_SET_GET(hi_crg_clken1_gpio3_clk_en,gpio3_clk_en,HI_CRG_CLKEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN1_OFFSET)
HI_SET_GET(hi_crg_clken1_reserved_0,reserved_0,HI_CRG_CLKEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN1_OFFSET)
HI_SET_GET(hi_crg_clkdis1_mst_aclk_dis,mst_aclk_dis,HI_CRG_CLKDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS1_OFFSET)
HI_SET_GET(hi_crg_clkdis1_dsp_aclk_dis,dsp_aclk_dis,HI_CRG_CLKDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS1_OFFSET)
HI_SET_GET(hi_crg_clkdis1_soc_hclk_dis,soc_hclk_dis,HI_CRG_CLKDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS1_OFFSET)
HI_SET_GET(hi_crg_clkdis1_amon_cpufast_clk_dis,amon_cpufast_clk_dis,HI_CRG_CLKDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS1_OFFSET)
HI_SET_GET(hi_crg_clkdis1_amon_soc_clk_dis,amon_soc_clk_dis,HI_CRG_CLKDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS1_OFFSET)
HI_SET_GET(hi_crg_clkdis1_tsensor_clk_dis,tsensor_clk_dis,HI_CRG_CLKDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS1_OFFSET)
HI_SET_GET(hi_crg_clkdis1_reserved_2,reserved_2,HI_CRG_CLKDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS1_OFFSET)
HI_SET_GET(hi_crg_clkdis1_ipf_clk_dis,ipf_clk_dis,HI_CRG_CLKDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS1_OFFSET)
HI_SET_GET(hi_crg_clkdis1_reserved_1,reserved_1,HI_CRG_CLKDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS1_OFFSET)
HI_SET_GET(hi_crg_clkdis1_edmac_ch4_clk_dis,edmac_ch4_clk_dis,HI_CRG_CLKDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS1_OFFSET)
HI_SET_GET(hi_crg_clkdis1_socp_clk_dis,socp_clk_dis,HI_CRG_CLKDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS1_OFFSET)
HI_SET_GET(hi_crg_clkdis1_mipi_clk_dis,mipi_clk_dis,HI_CRG_CLKDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS1_OFFSET)
HI_SET_GET(hi_crg_clkdis1_pmussi0_clk_dis,pmussi0_clk_dis,HI_CRG_CLKDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS1_OFFSET)
HI_SET_GET(hi_crg_clkdis1_pmussi1_clk_dis,pmussi1_clk_dis,HI_CRG_CLKDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS1_OFFSET)
HI_SET_GET(hi_crg_clkdis1_dw_ssi0_clk_dis,dw_ssi0_clk_dis,HI_CRG_CLKDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS1_OFFSET)
HI_SET_GET(hi_crg_clkdis1_efuse_clk_dis,efuse_clk_dis,HI_CRG_CLKDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS1_OFFSET)
HI_SET_GET(hi_crg_clkdis1_nandc_clk_dis,nandc_clk_dis,HI_CRG_CLKDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS1_OFFSET)
HI_SET_GET(hi_crg_clkdis1_hs_uart_clk_dis,hs_uart_clk_dis,HI_CRG_CLKDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS1_OFFSET)
HI_SET_GET(hi_crg_clkdis1_emi_clk_dis,emi_clk_dis,HI_CRG_CLKDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS1_OFFSET)
HI_SET_GET(hi_crg_clkdis1_edmac_clk_dis,edmac_clk_dis,HI_CRG_CLKDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS1_OFFSET)
HI_SET_GET(hi_crg_clkdis1_bootrom_clk_dis,bootrom_clk_dis,HI_CRG_CLKDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS1_OFFSET)
HI_SET_GET(hi_crg_clkdis1_pwm2_clk_dis,pwm2_clk_dis,HI_CRG_CLKDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS1_OFFSET)
HI_SET_GET(hi_crg_clkdis1_pwm1_clk_dis,pwm1_clk_dis,HI_CRG_CLKDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS1_OFFSET)
HI_SET_GET(hi_crg_clkdis1_dt_clk_dis,dt_clk_dis,HI_CRG_CLKDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS1_OFFSET)
HI_SET_GET(hi_crg_clkdis1_ipcm_clk_dis,ipcm_clk_dis,HI_CRG_CLKDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS1_OFFSET)
HI_SET_GET(hi_crg_clkdis1_dw_ssi1_clk_dis,dw_ssi1_clk_dis,HI_CRG_CLKDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS1_OFFSET)
HI_SET_GET(hi_crg_clkdis1_i2c1_clk_dis,i2c1_clk_dis,HI_CRG_CLKDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS1_OFFSET)
HI_SET_GET(hi_crg_clkdis1_gpio1_clk_dis,gpio1_clk_dis,HI_CRG_CLKDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS1_OFFSET)
HI_SET_GET(hi_crg_clkdis1_i2c0_clk_dis,i2c0_clk_dis,HI_CRG_CLKDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS1_OFFSET)
HI_SET_GET(hi_crg_clkdis1_gpio3_clk_dis,gpio3_clk_dis,HI_CRG_CLKDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS1_OFFSET)
HI_SET_GET(hi_crg_clkdis1_reserved_0,reserved_0,HI_CRG_CLKDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS1_OFFSET)
HI_SET_GET(hi_crg_clkstat1_mst_aclk_status,mst_aclk_status,HI_CRG_CLKSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT1_OFFSET)
HI_SET_GET(hi_crg_clkstat1_dsp_aclk_status,dsp_aclk_status,HI_CRG_CLKSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT1_OFFSET)
HI_SET_GET(hi_crg_clkstat1_soc_hclk_status,soc_hclk_status,HI_CRG_CLKSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT1_OFFSET)
HI_SET_GET(hi_crg_clkstat1_amon_cpufast_clk_status,amon_cpufast_clk_status,HI_CRG_CLKSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT1_OFFSET)
HI_SET_GET(hi_crg_clkstat1_amon_soc_clk_status,amon_soc_clk_status,HI_CRG_CLKSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT1_OFFSET)
HI_SET_GET(hi_crg_clkstat1_reserved_2,reserved_2,HI_CRG_CLKSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT1_OFFSET)
HI_SET_GET(hi_crg_clkstat1_ipf_clk_status,ipf_clk_status,HI_CRG_CLKSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT1_OFFSET)
HI_SET_GET(hi_crg_clkstat1_reserved_1,reserved_1,HI_CRG_CLKSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT1_OFFSET)
HI_SET_GET(hi_crg_clkstat1_edmac_ch4_clk_status,edmac_ch4_clk_status,HI_CRG_CLKSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT1_OFFSET)
HI_SET_GET(hi_crg_clkstat1_socp_clk_status,socp_clk_status,HI_CRG_CLKSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT1_OFFSET)
HI_SET_GET(hi_crg_clkstat1_mipi_clk_status,mipi_clk_status,HI_CRG_CLKSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT1_OFFSET)
HI_SET_GET(hi_crg_clkstat1_pmussi0_clk_status,pmussi0_clk_status,HI_CRG_CLKSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT1_OFFSET)
HI_SET_GET(hi_crg_clkstat1_pmussi1_clk_status,pmussi1_clk_status,HI_CRG_CLKSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT1_OFFSET)
HI_SET_GET(hi_crg_clkstat1_dw_ssi0_clk_status,dw_ssi0_clk_status,HI_CRG_CLKSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT1_OFFSET)
HI_SET_GET(hi_crg_clkstat1_efuse_clk_status,efuse_clk_status,HI_CRG_CLKSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT1_OFFSET)
HI_SET_GET(hi_crg_clkstat1_nandc_clk_status,nandc_clk_status,HI_CRG_CLKSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT1_OFFSET)
HI_SET_GET(hi_crg_clkstat1_hs_uart_clk_status,hs_uart_clk_status,HI_CRG_CLKSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT1_OFFSET)
HI_SET_GET(hi_crg_clkstat1_emi_clk_status,emi_clk_status,HI_CRG_CLKSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT1_OFFSET)
HI_SET_GET(hi_crg_clkstat1_edmac_clk_status,edmac_clk_status,HI_CRG_CLKSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT1_OFFSET)
HI_SET_GET(hi_crg_clkstat1_bootrom_clk_status,bootrom_clk_status,HI_CRG_CLKSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT1_OFFSET)
HI_SET_GET(hi_crg_clkstat1_pwm2_clk_status,pwm2_clk_status,HI_CRG_CLKSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT1_OFFSET)
HI_SET_GET(hi_crg_clkstat1_pwm1_clk_status,pwm1_clk_status,HI_CRG_CLKSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT1_OFFSET)
HI_SET_GET(hi_crg_clkstat1_dt_clk_status,dt_clk_status,HI_CRG_CLKSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT1_OFFSET)
HI_SET_GET(hi_crg_clkstat1_ipcm_clk_status,ipcm_clk_status,HI_CRG_CLKSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT1_OFFSET)
HI_SET_GET(hi_crg_clkstat1_dw_ssi1_clk_status,dw_ssi1_clk_status,HI_CRG_CLKSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT1_OFFSET)
HI_SET_GET(hi_crg_clkstat1_i2c1_clk_status,i2c1_clk_status,HI_CRG_CLKSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT1_OFFSET)
HI_SET_GET(hi_crg_clkstat1_gpio1_clk_status,gpio1_clk_status,HI_CRG_CLKSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT1_OFFSET)
HI_SET_GET(hi_crg_clkstat1_i2c0_clk_status,i2c0_clk_status,HI_CRG_CLKSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT1_OFFSET)
HI_SET_GET(hi_crg_clkstat1_gpio3_clk_status,gpio3_clk_status,HI_CRG_CLKSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT1_OFFSET)
HI_SET_GET(hi_crg_clkstat1_reserved_0,reserved_0,HI_CRG_CLKSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT1_OFFSET)
HI_SET_GET(hi_crg_clken2_uart3_clk_en,uart3_clk_en,HI_CRG_CLKEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN2_OFFSET)
HI_SET_GET(hi_crg_clken2_uart2_clk_en,uart2_clk_en,HI_CRG_CLKEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN2_OFFSET)
HI_SET_GET(hi_crg_clken2_gpio2_clk_en,gpio2_clk_en,HI_CRG_CLKEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN2_OFFSET)
HI_SET_GET(hi_crg_clken2_cs_clk_en,cs_clk_en,HI_CRG_CLKEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN2_OFFSET)
HI_SET_GET(hi_crg_clken2_appa9_mbist_clk_en,appa9_mbist_clk_en,HI_CRG_CLKEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN2_OFFSET)
HI_SET_GET(hi_crg_clken2_moda9_mbist_clk_en,moda9_mbist_clk_en,HI_CRG_CLKEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN2_OFFSET)
HI_SET_GET(hi_crg_clken2_cpu_aclk_en,cpu_aclk_en,HI_CRG_CLKEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN2_OFFSET)
HI_SET_GET(hi_crg_clken2_mddrc_clk_en,mddrc_clk_en,HI_CRG_CLKEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN2_OFFSET)
HI_SET_GET(hi_crg_clken2_ddrphy_clk_en,ddrphy_clk_en,HI_CRG_CLKEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN2_OFFSET)
HI_SET_GET(hi_crg_clken2_ddrphy_ddr_clk_en,ddrphy_ddr_clk_en,HI_CRG_CLKEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN2_OFFSET)
HI_SET_GET(hi_crg_clken2_axi_mem_clk_en,axi_mem_clk_en,HI_CRG_CLKEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN2_OFFSET)
HI_SET_GET(hi_crg_clken2_func_wakeup_pd_clk_en,func_wakeup_pd_clk_en,HI_CRG_CLKEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN2_OFFSET)
HI_SET_GET(hi_crg_clken2_soc_peri_usb_pd_clk_en,soc_peri_usb_pd_clk_en,HI_CRG_CLKEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN2_OFFSET)
HI_SET_GET(hi_crg_clken2_usbotg_clk_en,usbotg_clk_en,HI_CRG_CLKEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN2_OFFSET)
HI_SET_GET(hi_crg_clken2_usb_bc_clk_en,usb_bc_clk_en,HI_CRG_CLKEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN2_OFFSET)
HI_SET_GET(hi_crg_clken2_ssic_host_clk_en,ssic_host_clk_en,HI_CRG_CLKEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN2_OFFSET)
HI_SET_GET(hi_crg_clken2_ssic_dev_clk_en,ssic_dev_clk_en,HI_CRG_CLKEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN2_OFFSET)
HI_SET_GET(hi_crg_clken2_mmc0_clk_en,mmc0_clk_en,HI_CRG_CLKEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN2_OFFSET)
HI_SET_GET(hi_crg_clken2_mmc1_clk_en,mmc1_clk_en,HI_CRG_CLKEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN2_OFFSET)
HI_SET_GET(hi_crg_clken2_mmc0_phase_clk_en,mmc0_phase_clk_en,HI_CRG_CLKEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN2_OFFSET)
HI_SET_GET(hi_crg_clken2_mmc1_phase_clk_en,mmc1_phase_clk_en,HI_CRG_CLKEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN2_OFFSET)
HI_SET_GET(hi_crg_clken2_mmc0_refclk_en,mmc0_refclk_en,HI_CRG_CLKEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN2_OFFSET)
HI_SET_GET(hi_crg_clken2_mmc1_refclk_en,mmc1_refclk_en,HI_CRG_CLKEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN2_OFFSET)
HI_SET_GET(hi_crg_clken2_usbhsic_clk_en,usbhsic_clk_en,HI_CRG_CLKEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN2_OFFSET)
HI_SET_GET(hi_crg_clken2_sdcc_clk_en,sdcc_clk_en,HI_CRG_CLKEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN2_OFFSET)
HI_SET_GET(hi_crg_clken2_hpm_clk_en,hpm_clk_en,HI_CRG_CLKEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN2_OFFSET)
HI_SET_GET(hi_crg_clken2_sha1_clk_en,sha1_clk_en,HI_CRG_CLKEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN2_OFFSET)
HI_SET_GET(hi_crg_clken2_pcie_ctrl_clk_en,pcie_ctrl_clk_en,HI_CRG_CLKEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN2_OFFSET)
HI_SET_GET(hi_crg_clken2_pcie_phy_clk_en,pcie_phy_clk_en,HI_CRG_CLKEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN2_OFFSET)
HI_SET_GET(hi_crg_clken2_reserved,reserved,HI_CRG_CLKEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN2_OFFSET)
HI_SET_GET(hi_crg_clken2_wdog_clk_en,wdog_clk_en,HI_CRG_CLKEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN2_OFFSET)
HI_SET_GET(hi_crg_clkdis2_uart3_clk_dis,uart3_clk_dis,HI_CRG_CLKDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS2_OFFSET)
HI_SET_GET(hi_crg_clkdis2_uart2_clk_dis,uart2_clk_dis,HI_CRG_CLKDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS2_OFFSET)
HI_SET_GET(hi_crg_clkdis2_gpio2_clk_dis,gpio2_clk_dis,HI_CRG_CLKDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS2_OFFSET)
HI_SET_GET(hi_crg_clkdis2_cs_clk_dis,cs_clk_dis,HI_CRG_CLKDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS2_OFFSET)
HI_SET_GET(hi_crg_clkdis2_appa9_mbist_clk_dis,appa9_mbist_clk_dis,HI_CRG_CLKDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS2_OFFSET)
HI_SET_GET(hi_crg_clkdis2_moda9_mbist_clk_dis,moda9_mbist_clk_dis,HI_CRG_CLKDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS2_OFFSET)
HI_SET_GET(hi_crg_clkdis2_cpu_aclk_dis,cpu_aclk_dis,HI_CRG_CLKDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS2_OFFSET)
HI_SET_GET(hi_crg_clkdis2_mddrc_clk_dis,mddrc_clk_dis,HI_CRG_CLKDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS2_OFFSET)
HI_SET_GET(hi_crg_clkdis2_ddrphy_clk_dis,ddrphy_clk_dis,HI_CRG_CLKDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS2_OFFSET)
HI_SET_GET(hi_crg_clkdis2_ddrphy_ddr_clk_dis,ddrphy_ddr_clk_dis,HI_CRG_CLKDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS2_OFFSET)
HI_SET_GET(hi_crg_clkdis2_axi_mem_clk_dis,axi_mem_clk_dis,HI_CRG_CLKDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS2_OFFSET)
HI_SET_GET(hi_crg_clkdis2_func_wake_pd_clk_dis,func_wake_pd_clk_dis,HI_CRG_CLKDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS2_OFFSET)
HI_SET_GET(hi_crg_clkdis2_soc_usb_pd_clk_dis,soc_usb_pd_clk_dis,HI_CRG_CLKDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS2_OFFSET)
HI_SET_GET(hi_crg_clkdis2_usbotg_clk_dis,usbotg_clk_dis,HI_CRG_CLKDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS2_OFFSET)
HI_SET_GET(hi_crg_clkdis2_usb_bc_clk_dis,usb_bc_clk_dis,HI_CRG_CLKDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS2_OFFSET)
HI_SET_GET(hi_crg_clkdis2_ssic_host_clk_dis,ssic_host_clk_dis,HI_CRG_CLKDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS2_OFFSET)
HI_SET_GET(hi_crg_clkdis2_ssic_dev_clk_dis,ssic_dev_clk_dis,HI_CRG_CLKDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS2_OFFSET)
HI_SET_GET(hi_crg_clkdis2_mmc0_clk_dis,mmc0_clk_dis,HI_CRG_CLKDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS2_OFFSET)
HI_SET_GET(hi_crg_clkdis2_mmc1_clk_dis,mmc1_clk_dis,HI_CRG_CLKDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS2_OFFSET)
HI_SET_GET(hi_crg_clkdis2_mmc0_phase_clk_dis,mmc0_phase_clk_dis,HI_CRG_CLKDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS2_OFFSET)
HI_SET_GET(hi_crg_clkdis2_mmc1_phase_clk_dis,mmc1_phase_clk_dis,HI_CRG_CLKDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS2_OFFSET)
HI_SET_GET(hi_crg_clkdis2_mmc0_refclk_dis,mmc0_refclk_dis,HI_CRG_CLKDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS2_OFFSET)
HI_SET_GET(hi_crg_clkdis2_mmc1_refclk_dis,mmc1_refclk_dis,HI_CRG_CLKDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS2_OFFSET)
HI_SET_GET(hi_crg_clkdis2_usbhsic_clk_dis,usbhsic_clk_dis,HI_CRG_CLKDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS2_OFFSET)
HI_SET_GET(hi_crg_clkdis2_sdcc_clk_dis,sdcc_clk_dis,HI_CRG_CLKDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS2_OFFSET)
HI_SET_GET(hi_crg_clkdis2_hpm_clk_dis,hpm_clk_dis,HI_CRG_CLKDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS2_OFFSET)
HI_SET_GET(hi_crg_clkdis2_sha1_clk_dis,sha1_clk_dis,HI_CRG_CLKDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS2_OFFSET)
HI_SET_GET(hi_crg_clkdis2_pcie_ctrl_clk_dis,pcie_ctrl_clk_dis,HI_CRG_CLKDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS2_OFFSET)
HI_SET_GET(hi_crg_clkdis2_pcie_phy_clk_dis,pcie_phy_clk_dis,HI_CRG_CLKDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS2_OFFSET)
HI_SET_GET(hi_crg_clkdis2_reserved,reserved,HI_CRG_CLKDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS2_OFFSET)
HI_SET_GET(hi_crg_clkdis2_wdog_clk_dis,wdog_clk_dis,HI_CRG_CLKDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS2_OFFSET)
HI_SET_GET(hi_crg_clkstat2_uart3_clk_status,uart3_clk_status,HI_CRG_CLKSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT2_OFFSET)
HI_SET_GET(hi_crg_clkstat2_uart2_clk_status,uart2_clk_status,HI_CRG_CLKSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT2_OFFSET)
HI_SET_GET(hi_crg_clkstat2_gpio2_clk_status,gpio2_clk_status,HI_CRG_CLKSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT2_OFFSET)
HI_SET_GET(hi_crg_clkstat2_cs_clk_status,cs_clk_status,HI_CRG_CLKSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT2_OFFSET)
HI_SET_GET(hi_crg_clkstat2_appa9_mbist_clk_status,appa9_mbist_clk_status,HI_CRG_CLKSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT2_OFFSET)
HI_SET_GET(hi_crg_clkstat2_moda9_mbist_clk_status,moda9_mbist_clk_status,HI_CRG_CLKSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT2_OFFSET)
HI_SET_GET(hi_crg_clkstat2_cpu_aclk_status,cpu_aclk_status,HI_CRG_CLKSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT2_OFFSET)
HI_SET_GET(hi_crg_clkstat2_mddrc_clk_status,mddrc_clk_status,HI_CRG_CLKSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT2_OFFSET)
HI_SET_GET(hi_crg_clkstat2_ddrphy_clk_status,ddrphy_clk_status,HI_CRG_CLKSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT2_OFFSET)
HI_SET_GET(hi_crg_clkstat2_ddrphy_ddr_clk_status,ddrphy_ddr_clk_status,HI_CRG_CLKSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT2_OFFSET)
HI_SET_GET(hi_crg_clkstat2_axi_mem_clk_status,axi_mem_clk_status,HI_CRG_CLKSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT2_OFFSET)
HI_SET_GET(hi_crg_clkstat2_func_wake_pd_clk_status,func_wake_pd_clk_status,HI_CRG_CLKSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT2_OFFSET)
HI_SET_GET(hi_crg_clkstat2_soc_usb_pd_clk_status,soc_usb_pd_clk_status,HI_CRG_CLKSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT2_OFFSET)
HI_SET_GET(hi_crg_clkstat2_usbotg_clk_status,usbotg_clk_status,HI_CRG_CLKSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT2_OFFSET)
HI_SET_GET(hi_crg_clkstat2_usb_bc_clk_status,usb_bc_clk_status,HI_CRG_CLKSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT2_OFFSET)
HI_SET_GET(hi_crg_clkstat2_ssic_host_clk_status,ssic_host_clk_status,HI_CRG_CLKSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT2_OFFSET)
HI_SET_GET(hi_crg_clkstat2_ssic_dev_clk_status,ssic_dev_clk_status,HI_CRG_CLKSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT2_OFFSET)
HI_SET_GET(hi_crg_clkstat2_mmc0_clk_status,mmc0_clk_status,HI_CRG_CLKSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT2_OFFSET)
HI_SET_GET(hi_crg_clkstat2_mmc1_clk_status,mmc1_clk_status,HI_CRG_CLKSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT2_OFFSET)
HI_SET_GET(hi_crg_clkstat2_mmc0_phase_clk_status,mmc0_phase_clk_status,HI_CRG_CLKSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT2_OFFSET)
HI_SET_GET(hi_crg_clkstat2_mmc1_phase_clk_status,mmc1_phase_clk_status,HI_CRG_CLKSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT2_OFFSET)
HI_SET_GET(hi_crg_clkstat2_mmc0_refclk_status,mmc0_refclk_status,HI_CRG_CLKSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT2_OFFSET)
HI_SET_GET(hi_crg_clkstat2_mmc1_refclk_status,mmc1_refclk_status,HI_CRG_CLKSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT2_OFFSET)
HI_SET_GET(hi_crg_clkstat2_usbhsic_clk_status,usbhsic_clk_status,HI_CRG_CLKSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT2_OFFSET)
HI_SET_GET(hi_crg_clkstat2_sdcc_clk_status,sdcc_clk_status,HI_CRG_CLKSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT2_OFFSET)
HI_SET_GET(hi_crg_clkstat2_hpm_clk_status,hpm_clk_status,HI_CRG_CLKSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT2_OFFSET)
HI_SET_GET(hi_crg_clkstat2_sha1_clk_status,sha1_clk_status,HI_CRG_CLKSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT2_OFFSET)
HI_SET_GET(hi_crg_clkstat2_pcie_ctrl_clk_status,pcie_ctrl_clk_status,HI_CRG_CLKSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT2_OFFSET)
HI_SET_GET(hi_crg_clkstat2_pcie_phy_clk_status,pcie_phy_clk_status,HI_CRG_CLKSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT2_OFFSET)
HI_SET_GET(hi_crg_clkstat2_reserved,reserved,HI_CRG_CLKSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT2_OFFSET)
HI_SET_GET(hi_crg_clkstat2_wdog_clk_status,wdog_clk_status,HI_CRG_CLKSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT2_OFFSET)
HI_SET_GET(hi_crg_clken3_gpio0_clk_en,gpio0_clk_en,HI_CRG_CLKEN3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN3_OFFSET)
HI_SET_GET(hi_crg_clken3_uart0_clk_en,uart0_clk_en,HI_CRG_CLKEN3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN3_OFFSET)
HI_SET_GET(hi_crg_clken3_tm0_clk_en,tm0_clk_en,HI_CRG_CLKEN3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN3_OFFSET)
HI_SET_GET(hi_crg_clken3_tm1_clk_en,tm1_clk_en,HI_CRG_CLKEN3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN3_OFFSET)
HI_SET_GET(hi_crg_clken3_tm2_clk_en,tm2_clk_en,HI_CRG_CLKEN3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN3_OFFSET)
HI_SET_GET(hi_crg_clken3_tm3_clk_en,tm3_clk_en,HI_CRG_CLKEN3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN3_OFFSET)
HI_SET_GET(hi_crg_clken3_tm4_clk_en,tm4_clk_en,HI_CRG_CLKEN3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN3_OFFSET)
HI_SET_GET(hi_crg_clken3_tm5_clk_en,tm5_clk_en,HI_CRG_CLKEN3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN3_OFFSET)
HI_SET_GET(hi_crg_clken3_tm6_clk_en,tm6_clk_en,HI_CRG_CLKEN3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN3_OFFSET)
HI_SET_GET(hi_crg_clken3_tm7_clk_en,tm7_clk_en,HI_CRG_CLKEN3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN3_OFFSET)
HI_SET_GET(hi_crg_clken3_tm8_clk_en,tm8_clk_en,HI_CRG_CLKEN3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN3_OFFSET)
HI_SET_GET(hi_crg_clken3_tm9_clk_en,tm9_clk_en,HI_CRG_CLKEN3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN3_OFFSET)
HI_SET_GET(hi_crg_clken3_tm10_clk_en,tm10_clk_en,HI_CRG_CLKEN3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN3_OFFSET)
HI_SET_GET(hi_crg_clken3_tm11_clk_en,tm11_clk_en,HI_CRG_CLKEN3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN3_OFFSET)
HI_SET_GET(hi_crg_clken3_tm12_clk_en,tm12_clk_en,HI_CRG_CLKEN3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN3_OFFSET)
HI_SET_GET(hi_crg_clken3_tm13_clk_en,tm13_clk_en,HI_CRG_CLKEN3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN3_OFFSET)
HI_SET_GET(hi_crg_clken3_tm14_clk_en,tm14_clk_en,HI_CRG_CLKEN3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN3_OFFSET)
HI_SET_GET(hi_crg_clken3_tm15_clk_en,tm15_clk_en,HI_CRG_CLKEN3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN3_OFFSET)
HI_SET_GET(hi_crg_clken3_tm16_clk_en,tm16_clk_en,HI_CRG_CLKEN3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN3_OFFSET)
HI_SET_GET(hi_crg_clken3_tm17_clk_en,tm17_clk_en,HI_CRG_CLKEN3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN3_OFFSET)
HI_SET_GET(hi_crg_clken3_tm18_clk_en,tm18_clk_en,HI_CRG_CLKEN3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN3_OFFSET)
HI_SET_GET(hi_crg_clken3_tm19_clk_en,tm19_clk_en,HI_CRG_CLKEN3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN3_OFFSET)
HI_SET_GET(hi_crg_clken3_tm20_clk_en,tm20_clk_en,HI_CRG_CLKEN3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN3_OFFSET)
HI_SET_GET(hi_crg_clken3_tm21_clk_en,tm21_clk_en,HI_CRG_CLKEN3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN3_OFFSET)
HI_SET_GET(hi_crg_clken3_tm22_clk_en,tm22_clk_en,HI_CRG_CLKEN3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN3_OFFSET)
HI_SET_GET(hi_crg_clken3_tm23_clk_en,tm23_clk_en,HI_CRG_CLKEN3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN3_OFFSET)
HI_SET_GET(hi_crg_clken3_appa9_clk_en,appa9_clk_en,HI_CRG_CLKEN3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN3_OFFSET)
HI_SET_GET(hi_crg_clken3_mdma9_clk_en,mdma9_clk_en,HI_CRG_CLKEN3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN3_OFFSET)
HI_SET_GET(hi_crg_clken3_hifi_vic_clk_en,hifi_vic_clk_en,HI_CRG_CLKEN3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN3_OFFSET)
HI_SET_GET(hi_crg_clken3_lte_sio_clk_en,lte_sio_clk_en,HI_CRG_CLKEN3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN3_OFFSET)
HI_SET_GET(hi_crg_clken3_hifi_dbg_clk_en,hifi_dbg_clk_en,HI_CRG_CLKEN3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN3_OFFSET)
HI_SET_GET(hi_crg_clken3_hifi_clk_en,hifi_clk_en,HI_CRG_CLKEN3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN3_OFFSET)
HI_SET_GET(hi_crg_clkdis3_gpio0_clk_dis,gpio0_clk_dis,HI_CRG_CLKDIS3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS3_OFFSET)
HI_SET_GET(hi_crg_clkdis3_uart0_clk_dis,uart0_clk_dis,HI_CRG_CLKDIS3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS3_OFFSET)
HI_SET_GET(hi_crg_clkdis3_tm0_clk_dis,tm0_clk_dis,HI_CRG_CLKDIS3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS3_OFFSET)
HI_SET_GET(hi_crg_clkdis3_tm1_clk_dis,tm1_clk_dis,HI_CRG_CLKDIS3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS3_OFFSET)
HI_SET_GET(hi_crg_clkdis3_tm2_clk_dis,tm2_clk_dis,HI_CRG_CLKDIS3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS3_OFFSET)
HI_SET_GET(hi_crg_clkdis3_tm3_clk_dis,tm3_clk_dis,HI_CRG_CLKDIS3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS3_OFFSET)
HI_SET_GET(hi_crg_clkdis3_tm4_clk_dis,tm4_clk_dis,HI_CRG_CLKDIS3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS3_OFFSET)
HI_SET_GET(hi_crg_clkdis3_tm5_clk_dis,tm5_clk_dis,HI_CRG_CLKDIS3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS3_OFFSET)
HI_SET_GET(hi_crg_clkdis3_tm6_clk_dis,tm6_clk_dis,HI_CRG_CLKDIS3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS3_OFFSET)
HI_SET_GET(hi_crg_clkdis3_tm7_clk_dis,tm7_clk_dis,HI_CRG_CLKDIS3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS3_OFFSET)
HI_SET_GET(hi_crg_clkdis3_tm8_clk_dis,tm8_clk_dis,HI_CRG_CLKDIS3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS3_OFFSET)
HI_SET_GET(hi_crg_clkdis3_tm9_clk_dis,tm9_clk_dis,HI_CRG_CLKDIS3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS3_OFFSET)
HI_SET_GET(hi_crg_clkdis3_tm10_clk_dis,tm10_clk_dis,HI_CRG_CLKDIS3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS3_OFFSET)
HI_SET_GET(hi_crg_clkdis3_tm11_clk_dis,tm11_clk_dis,HI_CRG_CLKDIS3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS3_OFFSET)
HI_SET_GET(hi_crg_clkdis3_tm12_clk_dis,tm12_clk_dis,HI_CRG_CLKDIS3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS3_OFFSET)
HI_SET_GET(hi_crg_clkdis3_tm13_clk_dis,tm13_clk_dis,HI_CRG_CLKDIS3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS3_OFFSET)
HI_SET_GET(hi_crg_clkdis3_tm14_clk_dis,tm14_clk_dis,HI_CRG_CLKDIS3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS3_OFFSET)
HI_SET_GET(hi_crg_clkdis3_tm15_clk_dis,tm15_clk_dis,HI_CRG_CLKDIS3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS3_OFFSET)
HI_SET_GET(hi_crg_clkdis3_tm16_clk_dis,tm16_clk_dis,HI_CRG_CLKDIS3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS3_OFFSET)
HI_SET_GET(hi_crg_clkdis3_tm17_clk_dis,tm17_clk_dis,HI_CRG_CLKDIS3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS3_OFFSET)
HI_SET_GET(hi_crg_clkdis3_tm18_clk_dis,tm18_clk_dis,HI_CRG_CLKDIS3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS3_OFFSET)
HI_SET_GET(hi_crg_clkdis3_tm19_clk_dis,tm19_clk_dis,HI_CRG_CLKDIS3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS3_OFFSET)
HI_SET_GET(hi_crg_clkdis3_tm20_clk_dis,tm20_clk_dis,HI_CRG_CLKDIS3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS3_OFFSET)
HI_SET_GET(hi_crg_clkdis3_tm21_clk_dis,tm21_clk_dis,HI_CRG_CLKDIS3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS3_OFFSET)
HI_SET_GET(hi_crg_clkdis3_tm22_clk_dis,tm22_clk_dis,HI_CRG_CLKDIS3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS3_OFFSET)
HI_SET_GET(hi_crg_clkdis3_tm23_clk_dis,tm23_clk_dis,HI_CRG_CLKDIS3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS3_OFFSET)
HI_SET_GET(hi_crg_clkdis3_appa9_clk_dis,appa9_clk_dis,HI_CRG_CLKDIS3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS3_OFFSET)
HI_SET_GET(hi_crg_clkdis3_mdma9_clk_dis,mdma9_clk_dis,HI_CRG_CLKDIS3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS3_OFFSET)
HI_SET_GET(hi_crg_clkdis3_hifi_vic_clk_dis,hifi_vic_clk_dis,HI_CRG_CLKDIS3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS3_OFFSET)
HI_SET_GET(hi_crg_clkdis3_lte_sio_clk_dis,lte_sio_clk_dis,HI_CRG_CLKDIS3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS3_OFFSET)
HI_SET_GET(hi_crg_clkdis3_hifi_dbg_clk_dis,hifi_dbg_clk_dis,HI_CRG_CLKDIS3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS3_OFFSET)
HI_SET_GET(hi_crg_clkdis3_hifi_clk_dis,hifi_clk_dis,HI_CRG_CLKDIS3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS3_OFFSET)
HI_SET_GET(hi_crg_clkstat3_gpio0_clk_status,gpio0_clk_status,HI_CRG_CLKSTAT3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT3_OFFSET)
HI_SET_GET(hi_crg_clkstat3_uart0_clk_status,uart0_clk_status,HI_CRG_CLKSTAT3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT3_OFFSET)
HI_SET_GET(hi_crg_clkstat3_tm0_clk_status,tm0_clk_status,HI_CRG_CLKSTAT3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT3_OFFSET)
HI_SET_GET(hi_crg_clkstat3_tm1_clk_status,tm1_clk_status,HI_CRG_CLKSTAT3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT3_OFFSET)
HI_SET_GET(hi_crg_clkstat3_tm2_clk_status,tm2_clk_status,HI_CRG_CLKSTAT3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT3_OFFSET)
HI_SET_GET(hi_crg_clkstat3_tm3_clk_status,tm3_clk_status,HI_CRG_CLKSTAT3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT3_OFFSET)
HI_SET_GET(hi_crg_clkstat3_tm4_clk_status,tm4_clk_status,HI_CRG_CLKSTAT3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT3_OFFSET)
HI_SET_GET(hi_crg_clkstat3_tm5_clk_status,tm5_clk_status,HI_CRG_CLKSTAT3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT3_OFFSET)
HI_SET_GET(hi_crg_clkstat3_tm6_clk_status,tm6_clk_status,HI_CRG_CLKSTAT3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT3_OFFSET)
HI_SET_GET(hi_crg_clkstat3_tm7_clk_status,tm7_clk_status,HI_CRG_CLKSTAT3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT3_OFFSET)
HI_SET_GET(hi_crg_clkstat3_tm8_clk_status,tm8_clk_status,HI_CRG_CLKSTAT3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT3_OFFSET)
HI_SET_GET(hi_crg_clkstat3_tm9_clk_status,tm9_clk_status,HI_CRG_CLKSTAT3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT3_OFFSET)
HI_SET_GET(hi_crg_clkstat3_tm10_clk_status,tm10_clk_status,HI_CRG_CLKSTAT3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT3_OFFSET)
HI_SET_GET(hi_crg_clkstat3_tm11_clk_status,tm11_clk_status,HI_CRG_CLKSTAT3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT3_OFFSET)
HI_SET_GET(hi_crg_clkstat3_tm12_clk_status,tm12_clk_status,HI_CRG_CLKSTAT3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT3_OFFSET)
HI_SET_GET(hi_crg_clkstat3_tm13_clk_status,tm13_clk_status,HI_CRG_CLKSTAT3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT3_OFFSET)
HI_SET_GET(hi_crg_clkstat3_tm14_clk_status,tm14_clk_status,HI_CRG_CLKSTAT3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT3_OFFSET)
HI_SET_GET(hi_crg_clkstat3_tm15_clk_status,tm15_clk_status,HI_CRG_CLKSTAT3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT3_OFFSET)
HI_SET_GET(hi_crg_clkstat3_tm16_clk_status,tm16_clk_status,HI_CRG_CLKSTAT3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT3_OFFSET)
HI_SET_GET(hi_crg_clkstat3_tm17_clk_status,tm17_clk_status,HI_CRG_CLKSTAT3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT3_OFFSET)
HI_SET_GET(hi_crg_clkstat3_tm18_clk_status,tm18_clk_status,HI_CRG_CLKSTAT3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT3_OFFSET)
HI_SET_GET(hi_crg_clkstat3_tm19_clk_status,tm19_clk_status,HI_CRG_CLKSTAT3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT3_OFFSET)
HI_SET_GET(hi_crg_clkstat3_tm20_clk_status,tm20_clk_status,HI_CRG_CLKSTAT3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT3_OFFSET)
HI_SET_GET(hi_crg_clkstat3_tm21_clk_status,tm21_clk_status,HI_CRG_CLKSTAT3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT3_OFFSET)
HI_SET_GET(hi_crg_clkstat3_tm22_clk_status,tm22_clk_status,HI_CRG_CLKSTAT3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT3_OFFSET)
HI_SET_GET(hi_crg_clkstat3_tm23_clk_status,tm23_clk_status,HI_CRG_CLKSTAT3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT3_OFFSET)
HI_SET_GET(hi_crg_clkstat3_appa9_clk_status,appa9_clk_status,HI_CRG_CLKSTAT3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT3_OFFSET)
HI_SET_GET(hi_crg_clkstat3_mdma9_clk_status,mdma9_clk_status,HI_CRG_CLKSTAT3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT3_OFFSET)
HI_SET_GET(hi_crg_clkstat3_hifi_vic_clk_status,hifi_vic_clk_status,HI_CRG_CLKSTAT3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT3_OFFSET)
HI_SET_GET(hi_crg_clkstat3_lte_sio_clk_status,lte_sio_clk_status,HI_CRG_CLKSTAT3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT3_OFFSET)
HI_SET_GET(hi_crg_clkstat3_hifi_dbg_clk_status,hifi_dbg_clk_status,HI_CRG_CLKSTAT3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT3_OFFSET)
HI_SET_GET(hi_crg_clkstat3_hifi_clk_status,hifi_clk_status,HI_CRG_CLKSTAT3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT3_OFFSET)
HI_SET_GET(hi_crg_clken4_dsp0_pd_clk_en,dsp0_pd_clk_en,HI_CRG_CLKEN4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN4_OFFSET)
HI_SET_GET(hi_crg_clken4_dsp0_core_clk_en,dsp0_core_clk_en,HI_CRG_CLKEN4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN4_OFFSET)
HI_SET_GET(hi_crg_clken4_dsp0_dbg_clk_en,dsp0_dbg_clk_en,HI_CRG_CLKEN4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN4_OFFSET)
HI_SET_GET(hi_crg_clken4_reserved_1,reserved_1,HI_CRG_CLKEN4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN4_OFFSET)
HI_SET_GET(hi_crg_clken4_guacc_axi_clk_en,guacc_axi_clk_en,HI_CRG_CLKEN4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN4_OFFSET)
HI_SET_GET(hi_crg_clken4_cicom0_clk_en,cicom0_clk_en,HI_CRG_CLKEN4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN4_OFFSET)
HI_SET_GET(hi_crg_clken4_cicom1_clk_en,cicom1_clk_en,HI_CRG_CLKEN4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN4_OFFSET)
HI_SET_GET(hi_crg_clken4_hdlc_clk_en,hdlc_clk_en,HI_CRG_CLKEN4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN4_OFFSET)
HI_SET_GET(hi_crg_clken4_upacc_clk_en,upacc_clk_en,HI_CRG_CLKEN4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN4_OFFSET)
HI_SET_GET(hi_crg_clken4_sci0_clk_en,sci0_clk_en,HI_CRG_CLKEN4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN4_OFFSET)
HI_SET_GET(hi_crg_clken4_sci1_clk_en,sci1_clk_en,HI_CRG_CLKEN4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN4_OFFSET)
HI_SET_GET(hi_crg_clken4_uicc_clk_en,uicc_clk_en,HI_CRG_CLKEN4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN4_OFFSET)
HI_SET_GET(hi_crg_clken4_uart1_clk_en,uart1_clk_en,HI_CRG_CLKEN4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN4_OFFSET)
HI_SET_GET(hi_crg_clken4_dsp_vic_clk_en,dsp_vic_clk_en,HI_CRG_CLKEN4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN4_OFFSET)
HI_SET_GET(hi_crg_clken4_cipher_clk_en,cipher_clk_en,HI_CRG_CLKEN4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN4_OFFSET)
HI_SET_GET(hi_crg_clken4_gmac_rx_clk_en,gmac_rx_clk_en,HI_CRG_CLKEN4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN4_OFFSET)
HI_SET_GET(hi_crg_clken4_gmac_tx_clk_en,gmac_tx_clk_en,HI_CRG_CLKEN4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN4_OFFSET)
HI_SET_GET(hi_crg_clken4_gmac_gtx_clk_en,gmac_gtx_clk_en,HI_CRG_CLKEN4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN4_OFFSET)
HI_SET_GET(hi_crg_clken4_gmac_switch_clk_en,gmac_switch_clk_en,HI_CRG_CLKEN4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN4_OFFSET)
HI_SET_GET(hi_crg_clken4_gmac_aclk_en,gmac_aclk_en,HI_CRG_CLKEN4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN4_OFFSET)
HI_SET_GET(hi_crg_clken4_reserved_0,reserved_0,HI_CRG_CLKEN4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN4_OFFSET)
HI_SET_GET(hi_crg_clkdis4_dsp0_pd_clk_dis,dsp0_pd_clk_dis,HI_CRG_CLKDIS4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS4_OFFSET)
HI_SET_GET(hi_crg_clkdis4_dsp0_core_clk_dis,dsp0_core_clk_dis,HI_CRG_CLKDIS4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS4_OFFSET)
HI_SET_GET(hi_crg_clkdis4_dsp0_dbg_clk_dis,dsp0_dbg_clk_dis,HI_CRG_CLKDIS4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS4_OFFSET)
HI_SET_GET(hi_crg_clkdis4_reserved_1,reserved_1,HI_CRG_CLKDIS4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS4_OFFSET)
HI_SET_GET(hi_crg_clkdis4_guacc_axi_clk_dis,guacc_axi_clk_dis,HI_CRG_CLKDIS4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS4_OFFSET)
HI_SET_GET(hi_crg_clkdis4_cicom0_clk_dis,cicom0_clk_dis,HI_CRG_CLKDIS4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS4_OFFSET)
HI_SET_GET(hi_crg_clkdis4_cicom1_clk_dis,cicom1_clk_dis,HI_CRG_CLKDIS4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS4_OFFSET)
HI_SET_GET(hi_crg_clkdis4_hdlc_clk_dis,hdlc_clk_dis,HI_CRG_CLKDIS4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS4_OFFSET)
HI_SET_GET(hi_crg_clkdis4_upacc_clk_dis,upacc_clk_dis,HI_CRG_CLKDIS4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS4_OFFSET)
HI_SET_GET(hi_crg_clkdis4_sci0_clk_dis,sci0_clk_dis,HI_CRG_CLKDIS4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS4_OFFSET)
HI_SET_GET(hi_crg_clkdis4_sci1_clk_dis,sci1_clk_dis,HI_CRG_CLKDIS4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS4_OFFSET)
HI_SET_GET(hi_crg_clkdis4_uicc_clk_dis,uicc_clk_dis,HI_CRG_CLKDIS4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS4_OFFSET)
HI_SET_GET(hi_crg_clkdis4_uart1_clk_dis,uart1_clk_dis,HI_CRG_CLKDIS4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS4_OFFSET)
HI_SET_GET(hi_crg_clkdis4_dsp_vic_clk_dis,dsp_vic_clk_dis,HI_CRG_CLKDIS4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS4_OFFSET)
HI_SET_GET(hi_crg_clkdis4_cipher_clk_dis,cipher_clk_dis,HI_CRG_CLKDIS4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS4_OFFSET)
HI_SET_GET(hi_crg_clkdis4_gmac_rx_clk_dis,gmac_rx_clk_dis,HI_CRG_CLKDIS4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS4_OFFSET)
HI_SET_GET(hi_crg_clkdis4_gmac_tx_clk_dis,gmac_tx_clk_dis,HI_CRG_CLKDIS4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS4_OFFSET)
HI_SET_GET(hi_crg_clkdis4_gmac_gtx_clk_dis,gmac_gtx_clk_dis,HI_CRG_CLKDIS4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS4_OFFSET)
HI_SET_GET(hi_crg_clkdis4_gmac_switch_clk_dis,gmac_switch_clk_dis,HI_CRG_CLKDIS4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS4_OFFSET)
HI_SET_GET(hi_crg_clkdis4_gmac_aclk_dis,gmac_aclk_dis,HI_CRG_CLKDIS4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS4_OFFSET)
HI_SET_GET(hi_crg_clkdis4_reserved_0,reserved_0,HI_CRG_CLKDIS4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS4_OFFSET)
HI_SET_GET(hi_crg_clkstat4_dsp0_pd_clk_status,dsp0_pd_clk_status,HI_CRG_CLKSTAT4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT4_OFFSET)
HI_SET_GET(hi_crg_clkstat4_dsp0_core_clk_status,dsp0_core_clk_status,HI_CRG_CLKSTAT4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT4_OFFSET)
HI_SET_GET(hi_crg_clkstat4_dsp0_dbg_clk_status,dsp0_dbg_clk_status,HI_CRG_CLKSTAT4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT4_OFFSET)
HI_SET_GET(hi_crg_clkstat4_reserved_1,reserved_1,HI_CRG_CLKSTAT4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT4_OFFSET)
HI_SET_GET(hi_crg_clkstat4_guacc_axi_clk_status,guacc_axi_clk_status,HI_CRG_CLKSTAT4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT4_OFFSET)
HI_SET_GET(hi_crg_clkstat4_cicom0_clk_status,cicom0_clk_status,HI_CRG_CLKSTAT4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT4_OFFSET)
HI_SET_GET(hi_crg_clkstat4_cicom1_clk_status,cicom1_clk_status,HI_CRG_CLKSTAT4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT4_OFFSET)
HI_SET_GET(hi_crg_clkstat4_hdlc_clk_status,hdlc_clk_status,HI_CRG_CLKSTAT4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT4_OFFSET)
HI_SET_GET(hi_crg_clkstat4_upacc_clk_status,upacc_clk_status,HI_CRG_CLKSTAT4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT4_OFFSET)
HI_SET_GET(hi_crg_clkstat4_sci0_clk_status,sci0_clk_status,HI_CRG_CLKSTAT4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT4_OFFSET)
HI_SET_GET(hi_crg_clkstat4_sci1_clk_status,sci1_clk_status,HI_CRG_CLKSTAT4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT4_OFFSET)
HI_SET_GET(hi_crg_clkstat4_uicc_clk_status,uicc_clk_status,HI_CRG_CLKSTAT4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT4_OFFSET)
HI_SET_GET(hi_crg_clkstat4_uart1_clk_status,uart1_clk_status,HI_CRG_CLKSTAT4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT4_OFFSET)
HI_SET_GET(hi_crg_clkstat4_dsp_vic_clk_status,dsp_vic_clk_status,HI_CRG_CLKSTAT4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT4_OFFSET)
HI_SET_GET(hi_crg_clkstat4_cipher_clk_status,cipher_clk_status,HI_CRG_CLKSTAT4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT4_OFFSET)
HI_SET_GET(hi_crg_clkstat4_gmac_rx_clk_status,gmac_rx_clk_status,HI_CRG_CLKSTAT4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT4_OFFSET)
HI_SET_GET(hi_crg_clkstat4_gmac_tx_clk_status,gmac_tx_clk_status,HI_CRG_CLKSTAT4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT4_OFFSET)
HI_SET_GET(hi_crg_clkstat4_gmac_gtx_clk_status,gmac_gtx_clk_status,HI_CRG_CLKSTAT4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT4_OFFSET)
HI_SET_GET(hi_crg_clkstat4_gmac_switch_clk_status,gmac_switch_clk_status,HI_CRG_CLKSTAT4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT4_OFFSET)
HI_SET_GET(hi_crg_clkstat4_gmac_aclk_status,gmac_aclk_status,HI_CRG_CLKSTAT4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT4_OFFSET)
HI_SET_GET(hi_crg_clkstat4_reserved_0,reserved_0,HI_CRG_CLKSTAT4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT4_OFFSET)
HI_SET_GET(hi_crg_clken5_bbp_ao_clk_en,bbp_ao_clk_en,HI_CRG_CLKEN5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clken5_bbpcommon_2a_clk_en,bbpcommon_2a_clk_en,HI_CRG_CLKEN5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clken5_ltebbp_pd_clk_en,ltebbp_pd_clk_en,HI_CRG_CLKEN5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clken5_g1bbp_pd_clk_en,g1bbp_pd_clk_en,HI_CRG_CLKEN5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clken5_g2bbp_pd_clk_en,g2bbp_pd_clk_en,HI_CRG_CLKEN5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clken5_twbbp_pd_clk_en,twbbp_pd_clk_en,HI_CRG_CLKEN5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clken5_wbbp_pd_clk_en,wbbp_pd_clk_en,HI_CRG_CLKEN5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clken5_irm_pd_clk_en,irm_pd_clk_en,HI_CRG_CLKEN5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clken5_lbbp_axi_clk_en,lbbp_axi_clk_en,HI_CRG_CLKEN5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clken5_lbbp_pdt_clk_en,lbbp_pdt_clk_en,HI_CRG_CLKEN5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clken5_lbbp_pdf_clk_en,lbbp_pdf_clk_en,HI_CRG_CLKEN5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clken5_lbbp_tdl_clk_en,lbbp_tdl_clk_en,HI_CRG_CLKEN5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clken5_lbbp_vdl_clk_en,lbbp_vdl_clk_en,HI_CRG_CLKEN5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clken5_lbbp_tds_clk_en,lbbp_tds_clk_en,HI_CRG_CLKEN5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clken5_reserved_1,reserved_1,HI_CRG_CLKEN5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clken5_irm_turbo_clk_en,irm_turbo_clk_en,HI_CRG_CLKEN5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clken5_irm_bbc_245m_clk_en,irm_bbc_245m_clk_en,HI_CRG_CLKEN5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clken5_irm_wbbp_122m_clk_en,irm_wbbp_122m_clk_en,HI_CRG_CLKEN5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clken5_twbbp_wbbp_122m_clk_en,twbbp_wbbp_122m_clk_en,HI_CRG_CLKEN5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clken5_twbbp_wbbp_61m_clk_en,twbbp_wbbp_61m_clk_en,HI_CRG_CLKEN5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clken5_twbbp_tbbp_122m_clk_en,twbbp_tbbp_122m_clk_en,HI_CRG_CLKEN5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clken5_twbbp_tbbp_245m_clk_en,twbbp_tbbp_245m_clk_en,HI_CRG_CLKEN5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clken5_wbbp_61m_clk_en,wbbp_61m_clk_en,HI_CRG_CLKEN5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clken5_wbbp_122m_clk_en,wbbp_122m_clk_en,HI_CRG_CLKEN5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clken5_wbbp_245m_clk_en,wbbp_245m_clk_en,HI_CRG_CLKEN5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clken5_wbbp_axi_clk_en,wbbp_axi_clk_en,HI_CRG_CLKEN5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clken5_reserved_0,reserved_0,HI_CRG_CLKEN5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_bbp_ao_clk_dis,bbp_ao_clk_dis,HI_CRG_CLKDIS5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_bbpcommon_clk_dis,bbpcommon_clk_dis,HI_CRG_CLKDIS5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_ltebbp_pd_clk_dis,ltebbp_pd_clk_dis,HI_CRG_CLKDIS5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_g1bbp_pd_clk_dis,g1bbp_pd_clk_dis,HI_CRG_CLKDIS5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_g2bbp_pd_clk_dis,g2bbp_pd_clk_dis,HI_CRG_CLKDIS5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_twbbp_pd_clk_dis,twbbp_pd_clk_dis,HI_CRG_CLKDIS5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_wbbp_pd_clk_dis,wbbp_pd_clk_dis,HI_CRG_CLKDIS5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_irm_pd_clk_dis,irm_pd_clk_dis,HI_CRG_CLKDIS5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_lbbp_axi_clk_dis,lbbp_axi_clk_dis,HI_CRG_CLKDIS5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_lbbp_pdt_clk_dis,lbbp_pdt_clk_dis,HI_CRG_CLKDIS5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_lbbp_pdf_clk_dis,lbbp_pdf_clk_dis,HI_CRG_CLKDIS5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_lbbp_tdl_clk_dis,lbbp_tdl_clk_dis,HI_CRG_CLKDIS5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_lbbp_vdl_clk_dis,lbbp_vdl_clk_dis,HI_CRG_CLKDIS5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_lbbp_tds_clk_dis,lbbp_tds_clk_dis,HI_CRG_CLKDIS5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_twbbp_tbbp_clk_dis,twbbp_tbbp_clk_dis,HI_CRG_CLKDIS5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_twbbp_wbbp_clk_dis,twbbp_wbbp_clk_dis,HI_CRG_CLKDIS5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_irm_turbo_clk_dis,irm_turbo_clk_dis,HI_CRG_CLKDIS5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_irm_bbc_245m_clk_dis,irm_bbc_245m_clk_dis,HI_CRG_CLKDIS5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_irm_wbbp_122m_clk_dis,irm_wbbp_122m_clk_dis,HI_CRG_CLKDIS5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_pdtw_wbbp_122m_clk_dis,pdtw_wbbp_122m_clk_dis,HI_CRG_CLKDIS5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_pdtw_wbbp_61m_clk_dis,pdtw_wbbp_61m_clk_dis,HI_CRG_CLKDIS5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_pdtw_tbbp_122m_clk_dis,pdtw_tbbp_122m_clk_dis,HI_CRG_CLKDIS5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_pdtw_tbbp_245m_clk_dis,pdtw_tbbp_245m_clk_dis,HI_CRG_CLKDIS5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_pdw_61m_clk_dis,pdw_61m_clk_dis,HI_CRG_CLKDIS5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_pdw_122m_clk_dis,pdw_122m_clk_dis,HI_CRG_CLKDIS5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_pdw_245m_clk_dis,pdw_245m_clk_dis,HI_CRG_CLKDIS5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_pdw_axi_clk_dis,pdw_axi_clk_dis,HI_CRG_CLKDIS5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_reserved,reserved,HI_CRG_CLKDIS5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_bbp_ao_clk_stat,bbp_ao_clk_stat,HI_CRG_CLKSTAT5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_bbpcommon_2a_clk_stat,bbpcommon_2a_clk_stat,HI_CRG_CLKSTAT5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_ltebbp_pd_clk_stat,ltebbp_pd_clk_stat,HI_CRG_CLKSTAT5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_g1bbp_pd_clk_stat,g1bbp_pd_clk_stat,HI_CRG_CLKSTAT5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_g2bbp_pd_clk_stat,g2bbp_pd_clk_stat,HI_CRG_CLKSTAT5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_twbbp_pd_clk_stat,twbbp_pd_clk_stat,HI_CRG_CLKSTAT5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_wbbp_pd_clk_stat,wbbp_pd_clk_stat,HI_CRG_CLKSTAT5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_irm_pd_clk_stat,irm_pd_clk_stat,HI_CRG_CLKSTAT5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_lbbp_axi_clk_stat,lbbp_axi_clk_stat,HI_CRG_CLKSTAT5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_lbbp_pdt_clk_stat,lbbp_pdt_clk_stat,HI_CRG_CLKSTAT5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_lbbp_pdf_clk_stat,lbbp_pdf_clk_stat,HI_CRG_CLKSTAT5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_lbbp_tdl_clk_stat,lbbp_tdl_clk_stat,HI_CRG_CLKSTAT5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_lbbp_vdl_clk_stat,lbbp_vdl_clk_stat,HI_CRG_CLKSTAT5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_lbbp_tds_clk_stat,lbbp_tds_clk_stat,HI_CRG_CLKSTAT5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_twbbp_tbbp_clk_stat,twbbp_tbbp_clk_stat,HI_CRG_CLKSTAT5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_twbbp_wbbp_clk_stat,twbbp_wbbp_clk_stat,HI_CRG_CLKSTAT5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_irm_turbo_clk_stat,irm_turbo_clk_stat,HI_CRG_CLKSTAT5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_irm_bbc_245m_clk_status,irm_bbc_245m_clk_status,HI_CRG_CLKSTAT5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_irm_wbbp_122m_clk_status,irm_wbbp_122m_clk_status,HI_CRG_CLKSTAT5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_pdtw_wbbp_122m_clk_status,pdtw_wbbp_122m_clk_status,HI_CRG_CLKSTAT5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_pdtw_wbbp_61m_clk_status,pdtw_wbbp_61m_clk_status,HI_CRG_CLKSTAT5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_pdtw_tbbp_122m_clk_status,pdtw_tbbp_122m_clk_status,HI_CRG_CLKSTAT5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_pdtw_tbbp_245m_clk_status,pdtw_tbbp_245m_clk_status,HI_CRG_CLKSTAT5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_pdw_61m_clk_status,pdw_61m_clk_status,HI_CRG_CLKSTAT5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_pdw_122m_clk_status,pdw_122m_clk_status,HI_CRG_CLKSTAT5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_pdw_245m_clk_status,pdw_245m_clk_status,HI_CRG_CLKSTAT5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_pdw_axi_clk_status,pdw_axi_clk_status,HI_CRG_CLKSTAT5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_reserved,reserved,HI_CRG_CLKSTAT5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_srsten1_appa9_cpu_srst_en,appa9_cpu_srst_en,HI_CRG_SRSTEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN1_OFFSET)
HI_SET_GET(hi_crg_srsten1_appa9_dbg_srst_en,appa9_dbg_srst_en,HI_CRG_SRSTEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN1_OFFSET)
HI_SET_GET(hi_crg_srsten1_appa9_peri_srst_en,appa9_peri_srst_en,HI_CRG_SRSTEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN1_OFFSET)
HI_SET_GET(hi_crg_srsten1_appa9_scu_srst_en,appa9_scu_srst_en,HI_CRG_SRSTEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN1_OFFSET)
HI_SET_GET(hi_crg_srsten1_appa9_ptm_srst_en,appa9_ptm_srst_en,HI_CRG_SRSTEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN1_OFFSET)
HI_SET_GET(hi_crg_srsten1_appa9_wd_srst_en,appa9_wd_srst_en,HI_CRG_SRSTEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN1_OFFSET)
HI_SET_GET(hi_crg_srsten1_moda9_cpu_srst_en,moda9_cpu_srst_en,HI_CRG_SRSTEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN1_OFFSET)
HI_SET_GET(hi_crg_srsten1_moda9_dbg_srst_en,moda9_dbg_srst_en,HI_CRG_SRSTEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN1_OFFSET)
HI_SET_GET(hi_crg_srsten1_moda9_peri_srst_en,moda9_peri_srst_en,HI_CRG_SRSTEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN1_OFFSET)
HI_SET_GET(hi_crg_srsten1_moda9_scu_srst_en,moda9_scu_srst_en,HI_CRG_SRSTEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN1_OFFSET)
HI_SET_GET(hi_crg_srsten1_moda9_ptm_srst_en,moda9_ptm_srst_en,HI_CRG_SRSTEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN1_OFFSET)
HI_SET_GET(hi_crg_srsten1_moda9_wd_srst_en,moda9_wd_srst_en,HI_CRG_SRSTEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN1_OFFSET)
HI_SET_GET(hi_crg_srsten1_hpm_srst_en,hpm_srst_en,HI_CRG_SRSTEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN1_OFFSET)
HI_SET_GET(hi_crg_srsten1_hpm_clkdiv_srst_en,hpm_clkdiv_srst_en,HI_CRG_SRSTEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN1_OFFSET)
HI_SET_GET(hi_crg_srsten1_appa9_srst_en,appa9_srst_en,HI_CRG_SRSTEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN1_OFFSET)
HI_SET_GET(hi_crg_srsten1_mdma9_srst_en,mdma9_srst_en,HI_CRG_SRSTEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN1_OFFSET)
HI_SET_GET(hi_crg_srsten1_cm3_dbg_srst_en,cm3_dbg_srst_en,HI_CRG_SRSTEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN1_OFFSET)
HI_SET_GET(hi_crg_srsten1_mddrc_srst_en,mddrc_srst_en,HI_CRG_SRSTEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN1_OFFSET)
HI_SET_GET(hi_crg_srsten1_cm3_cpu_srst_en,cm3_cpu_srst_en,HI_CRG_SRSTEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN1_OFFSET)
HI_SET_GET(hi_crg_srsten1_cicom0_srst_en,cicom0_srst_en,HI_CRG_SRSTEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN1_OFFSET)
HI_SET_GET(hi_crg_srsten1_cicom1_srst_en,cicom1_srst_en,HI_CRG_SRSTEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN1_OFFSET)
HI_SET_GET(hi_crg_srsten1_hdlc_srst_en,hdlc_srst_en,HI_CRG_SRSTEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN1_OFFSET)
HI_SET_GET(hi_crg_srsten1_upacc_srst_en,upacc_srst_en,HI_CRG_SRSTEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN1_OFFSET)
HI_SET_GET(hi_crg_srsten1_edmac_ch4_srst_en,edmac_ch4_srst_en,HI_CRG_SRSTEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN1_OFFSET)
HI_SET_GET(hi_crg_srsten1_edmac_srst_en,edmac_srst_en,HI_CRG_SRSTEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN1_OFFSET)
HI_SET_GET(hi_crg_srsten1_emi_srst_en,emi_srst_en,HI_CRG_SRSTEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN1_OFFSET)
HI_SET_GET(hi_crg_srsten1_uicc_srst_en,uicc_srst_en,HI_CRG_SRSTEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN1_OFFSET)
HI_SET_GET(hi_crg_srsten1_reserved,reserved,HI_CRG_SRSTEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN1_OFFSET)
HI_SET_GET(hi_crg_srsten1_amon_cpufast_srst_en,amon_cpufast_srst_en,HI_CRG_SRSTEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN1_OFFSET)
HI_SET_GET(hi_crg_srsten1_amon_soc_srst_en,amon_soc_srst_en,HI_CRG_SRSTEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN1_OFFSET)
HI_SET_GET(hi_crg_srsten1_dt_srst_en,dt_srst_en,HI_CRG_SRSTEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN1_OFFSET)
HI_SET_GET(hi_crg_srsten1_sha1_srst_en,sha1_srst_en,HI_CRG_SRSTEN1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN1_OFFSET)
HI_SET_GET(hi_crg_srstdis1_appa9_cpu_srst_dis,appa9_cpu_srst_dis,HI_CRG_SRSTDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS1_OFFSET)
HI_SET_GET(hi_crg_srstdis1_appa9_dbg_srst_dis,appa9_dbg_srst_dis,HI_CRG_SRSTDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS1_OFFSET)
HI_SET_GET(hi_crg_srstdis1_appa9_peri_srst_dis,appa9_peri_srst_dis,HI_CRG_SRSTDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS1_OFFSET)
HI_SET_GET(hi_crg_srstdis1_appa9_scu_srst_dis,appa9_scu_srst_dis,HI_CRG_SRSTDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS1_OFFSET)
HI_SET_GET(hi_crg_srstdis1_appa9_ptm_srst_dis,appa9_ptm_srst_dis,HI_CRG_SRSTDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS1_OFFSET)
HI_SET_GET(hi_crg_srstdis1_appa9_wd_srst_dis,appa9_wd_srst_dis,HI_CRG_SRSTDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS1_OFFSET)
HI_SET_GET(hi_crg_srstdis1_moda9_cpu_srst_dis,moda9_cpu_srst_dis,HI_CRG_SRSTDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS1_OFFSET)
HI_SET_GET(hi_crg_srstdis1_moda9_dbg_srst_dis,moda9_dbg_srst_dis,HI_CRG_SRSTDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS1_OFFSET)
HI_SET_GET(hi_crg_srstdis1_moda9_peri_srst_dis,moda9_peri_srst_dis,HI_CRG_SRSTDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS1_OFFSET)
HI_SET_GET(hi_crg_srstdis1_moda9_scu_srst_dis,moda9_scu_srst_dis,HI_CRG_SRSTDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS1_OFFSET)
HI_SET_GET(hi_crg_srstdis1_moda9_ptm_srst_dis,moda9_ptm_srst_dis,HI_CRG_SRSTDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS1_OFFSET)
HI_SET_GET(hi_crg_srstdis1_moda9_wd_srst_dis,moda9_wd_srst_dis,HI_CRG_SRSTDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS1_OFFSET)
HI_SET_GET(hi_crg_srstdis1_hpm_srst_dis,hpm_srst_dis,HI_CRG_SRSTDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS1_OFFSET)
HI_SET_GET(hi_crg_srstdis1_hpm_clkdiv_srst_dis,hpm_clkdiv_srst_dis,HI_CRG_SRSTDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS1_OFFSET)
HI_SET_GET(hi_crg_srstdis1_appa9_srst_dis,appa9_srst_dis,HI_CRG_SRSTDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS1_OFFSET)
HI_SET_GET(hi_crg_srstdis1_mdma9_srst_dis,mdma9_srst_dis,HI_CRG_SRSTDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS1_OFFSET)
HI_SET_GET(hi_crg_srstdis1_cm3_dbg_srst_dis,cm3_dbg_srst_dis,HI_CRG_SRSTDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS1_OFFSET)
HI_SET_GET(hi_crg_srstdis1_mddrc_srst_dis,mddrc_srst_dis,HI_CRG_SRSTDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS1_OFFSET)
HI_SET_GET(hi_crg_srstdis1_cm3_cpu_srst_dis,cm3_cpu_srst_dis,HI_CRG_SRSTDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS1_OFFSET)
HI_SET_GET(hi_crg_srstdis1_cicom0_srst_dis,cicom0_srst_dis,HI_CRG_SRSTDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS1_OFFSET)
HI_SET_GET(hi_crg_srstdis1_cicom1_srst_dis,cicom1_srst_dis,HI_CRG_SRSTDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS1_OFFSET)
HI_SET_GET(hi_crg_srstdis1_hdlc_srst_dis,hdlc_srst_dis,HI_CRG_SRSTDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS1_OFFSET)
HI_SET_GET(hi_crg_srstdis1_upacc_srst_dis,upacc_srst_dis,HI_CRG_SRSTDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS1_OFFSET)
HI_SET_GET(hi_crg_srstdis1_edmac_ch4_srst_dis,edmac_ch4_srst_dis,HI_CRG_SRSTDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS1_OFFSET)
HI_SET_GET(hi_crg_srstdis1_edmac_srst_dis,edmac_srst_dis,HI_CRG_SRSTDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS1_OFFSET)
HI_SET_GET(hi_crg_srstdis1_emi_srst_dis,emi_srst_dis,HI_CRG_SRSTDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS1_OFFSET)
HI_SET_GET(hi_crg_srstdis1_uicc_srst_dis,uicc_srst_dis,HI_CRG_SRSTDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS1_OFFSET)
HI_SET_GET(hi_crg_srstdis1_reserved,reserved,HI_CRG_SRSTDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS1_OFFSET)
HI_SET_GET(hi_crg_srstdis1_amon_cpufast_srst_dis,amon_cpufast_srst_dis,HI_CRG_SRSTDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS1_OFFSET)
HI_SET_GET(hi_crg_srstdis1_amon_soc_srst_dis,amon_soc_srst_dis,HI_CRG_SRSTDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS1_OFFSET)
HI_SET_GET(hi_crg_srstdis1_dt_srst_dis,dt_srst_dis,HI_CRG_SRSTDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS1_OFFSET)
HI_SET_GET(hi_crg_srstdis1_sha1_srst_dis,sha1_srst_dis,HI_CRG_SRSTDIS1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS1_OFFSET)
HI_SET_GET(hi_crg_srststat1_appa9_cpu_srst_status,appa9_cpu_srst_status,HI_CRG_SRSTSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT1_OFFSET)
HI_SET_GET(hi_crg_srststat1_appa9_dbg_srst_status,appa9_dbg_srst_status,HI_CRG_SRSTSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT1_OFFSET)
HI_SET_GET(hi_crg_srststat1_appa9_peri_srst_status,appa9_peri_srst_status,HI_CRG_SRSTSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT1_OFFSET)
HI_SET_GET(hi_crg_srststat1_appa9_scu_srst_status,appa9_scu_srst_status,HI_CRG_SRSTSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT1_OFFSET)
HI_SET_GET(hi_crg_srststat1_appa9_ptm_srst_status,appa9_ptm_srst_status,HI_CRG_SRSTSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT1_OFFSET)
HI_SET_GET(hi_crg_srststat1_appa9_wd_srst_status,appa9_wd_srst_status,HI_CRG_SRSTSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT1_OFFSET)
HI_SET_GET(hi_crg_srststat1_moda9_cpu_srst_status,moda9_cpu_srst_status,HI_CRG_SRSTSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT1_OFFSET)
HI_SET_GET(hi_crg_srststat1_moda9_dbg_srst_status,moda9_dbg_srst_status,HI_CRG_SRSTSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT1_OFFSET)
HI_SET_GET(hi_crg_srststat1_moda9_peri_srst_status,moda9_peri_srst_status,HI_CRG_SRSTSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT1_OFFSET)
HI_SET_GET(hi_crg_srststat1_moda9_scu_srst_status,moda9_scu_srst_status,HI_CRG_SRSTSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT1_OFFSET)
HI_SET_GET(hi_crg_srststat1_moda9_ptm_srst_status,moda9_ptm_srst_status,HI_CRG_SRSTSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT1_OFFSET)
HI_SET_GET(hi_crg_srststat1_moda9_wd_srst_status,moda9_wd_srst_status,HI_CRG_SRSTSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT1_OFFSET)
HI_SET_GET(hi_crg_srststat1_hpm_srst_status,hpm_srst_status,HI_CRG_SRSTSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT1_OFFSET)
HI_SET_GET(hi_crg_srststat1_hpm_clkdiv_srst_status,hpm_clkdiv_srst_status,HI_CRG_SRSTSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT1_OFFSET)
HI_SET_GET(hi_crg_srststat1_appa9_srst_status,appa9_srst_status,HI_CRG_SRSTSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT1_OFFSET)
HI_SET_GET(hi_crg_srststat1_mdma9_srst_status,mdma9_srst_status,HI_CRG_SRSTSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT1_OFFSET)
HI_SET_GET(hi_crg_srststat1_cm3_dbg_srst_status,cm3_dbg_srst_status,HI_CRG_SRSTSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT1_OFFSET)
HI_SET_GET(hi_crg_srststat1_mddrc_srst_status,mddrc_srst_status,HI_CRG_SRSTSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT1_OFFSET)
HI_SET_GET(hi_crg_srststat1_cm3_core_srst_status,cm3_core_srst_status,HI_CRG_SRSTSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT1_OFFSET)
HI_SET_GET(hi_crg_srststat1_cicom0_srst_status,cicom0_srst_status,HI_CRG_SRSTSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT1_OFFSET)
HI_SET_GET(hi_crg_srststat1_cicom1_srst_status,cicom1_srst_status,HI_CRG_SRSTSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT1_OFFSET)
HI_SET_GET(hi_crg_srststat1_hdlc_srst_status,hdlc_srst_status,HI_CRG_SRSTSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT1_OFFSET)
HI_SET_GET(hi_crg_srststat1_upacc_srst_status,upacc_srst_status,HI_CRG_SRSTSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT1_OFFSET)
HI_SET_GET(hi_crg_srststat1_edmac_ch4_srst_status,edmac_ch4_srst_status,HI_CRG_SRSTSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT1_OFFSET)
HI_SET_GET(hi_crg_srststat1_edmac_srst_status,edmac_srst_status,HI_CRG_SRSTSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT1_OFFSET)
HI_SET_GET(hi_crg_srststat1_emi_srst_status,emi_srst_status,HI_CRG_SRSTSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT1_OFFSET)
HI_SET_GET(hi_crg_srststat1_uicc_srst_status,uicc_srst_status,HI_CRG_SRSTSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT1_OFFSET)
HI_SET_GET(hi_crg_srststat1_reserved,reserved,HI_CRG_SRSTSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT1_OFFSET)
HI_SET_GET(hi_crg_srststat1_amon_cpufast_srst_status,amon_cpufast_srst_status,HI_CRG_SRSTSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT1_OFFSET)
HI_SET_GET(hi_crg_srststat1_amon_soc_srst_status,amon_soc_srst_status,HI_CRG_SRSTSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT1_OFFSET)
HI_SET_GET(hi_crg_srststat1_dt_srst_status,dt_srst_status,HI_CRG_SRSTSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT1_OFFSET)
HI_SET_GET(hi_crg_srststat1_sha1_srst_status,sha1_srst_status,HI_CRG_SRSTSTAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT1_OFFSET)
HI_SET_GET(hi_crg_srsten2_tensi_dps0_pd_srst_en,tensi_dps0_pd_srst_en,HI_CRG_SRSTEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN2_OFFSET)
HI_SET_GET(hi_crg_srsten2_tensi_dsp0_core_srst_en,tensi_dsp0_core_srst_en,HI_CRG_SRSTEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN2_OFFSET)
HI_SET_GET(hi_crg_srsten2_tensi_dsp0_dbg_srst_en,tensi_dsp0_dbg_srst_en,HI_CRG_SRSTEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN2_OFFSET)
HI_SET_GET(hi_crg_srsten2_reserved_3,reserved_3,HI_CRG_SRSTEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN2_OFFSET)
HI_SET_GET(hi_crg_srsten2_func_wakeup_pd_srst_en,func_wakeup_pd_srst_en,HI_CRG_SRSTEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN2_OFFSET)
HI_SET_GET(hi_crg_srsten2_soc_peri_usb_pd_srst_en,soc_peri_usb_pd_srst_en,HI_CRG_SRSTEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN2_OFFSET)
HI_SET_GET(hi_crg_srsten2_mmc0_srst_en,mmc0_srst_en,HI_CRG_SRSTEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN2_OFFSET)
HI_SET_GET(hi_crg_srsten2_mmc1_srst_en,mmc1_srst_en,HI_CRG_SRSTEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN2_OFFSET)
HI_SET_GET(hi_crg_srsten2_sdcc_srst_en,sdcc_srst_en,HI_CRG_SRSTEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN2_OFFSET)
HI_SET_GET(hi_crg_srsten2_rserved,rserved,HI_CRG_SRSTEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN2_OFFSET)
HI_SET_GET(hi_crg_srsten2_gmac_srst_req,gmac_srst_req,HI_CRG_SRSTEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN2_OFFSET)
HI_SET_GET(hi_crg_srsten2_reserved_2,reserved_2,HI_CRG_SRSTEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN2_OFFSET)
HI_SET_GET(hi_crg_srsten2_pcie_ctrl_srst_req,pcie_ctrl_srst_req,HI_CRG_SRSTEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN2_OFFSET)
HI_SET_GET(hi_crg_srsten2_pcie_phy_srst_req,pcie_phy_srst_req,HI_CRG_SRSTEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN2_OFFSET)
HI_SET_GET(hi_crg_srsten2_ddrphy_div_srst_req,ddrphy_div_srst_req,HI_CRG_SRSTEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN2_OFFSET)
HI_SET_GET(hi_crg_srsten2_reserved_1,reserved_1,HI_CRG_SRSTEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN2_OFFSET)
HI_SET_GET(hi_crg_srsten2_hifi_core_srst_en,hifi_core_srst_en,HI_CRG_SRSTEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN2_OFFSET)
HI_SET_GET(hi_crg_srsten2_hifi_pd_srst_en,hifi_pd_srst_en,HI_CRG_SRSTEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN2_OFFSET)
HI_SET_GET(hi_crg_srsten2_hifi_dbg_srst_en,hifi_dbg_srst_en,HI_CRG_SRSTEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN2_OFFSET)
HI_SET_GET(hi_crg_srsten2_usbotg_bc_srst_en,usbotg_bc_srst_en,HI_CRG_SRSTEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN2_OFFSET)
HI_SET_GET(hi_crg_srsten2_usbotg_phy_srst_en,usbotg_phy_srst_en,HI_CRG_SRSTEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN2_OFFSET)
HI_SET_GET(hi_crg_srsten2_usbotg_ctrl_srst_en,usbotg_ctrl_srst_en,HI_CRG_SRSTEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN2_OFFSET)
HI_SET_GET(hi_crg_srsten2_usbotg_ctrl_vaux_srst_en,usbotg_ctrl_vaux_srst_en,HI_CRG_SRSTEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN2_OFFSET)
HI_SET_GET(hi_crg_srsten2_usbotg_ctrl_vcc_srst_en,usbotg_ctrl_vcc_srst_en,HI_CRG_SRSTEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN2_OFFSET)
HI_SET_GET(hi_crg_srsten2_usbhsic_ctrl_srst_en,usbhsic_ctrl_srst_en,HI_CRG_SRSTEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN2_OFFSET)
HI_SET_GET(hi_crg_srsten2_usbhsic_phy_srst_en,usbhsic_phy_srst_en,HI_CRG_SRSTEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN2_OFFSET)
HI_SET_GET(hi_crg_srsten2_cs_srst_en,cs_srst_en,HI_CRG_SRSTEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN2_OFFSET)
HI_SET_GET(hi_crg_srsten2_tsensor_srst_en,tsensor_srst_en,HI_CRG_SRSTEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN2_OFFSET)
HI_SET_GET(hi_crg_srsten2_reserved_0,reserved_0,HI_CRG_SRSTEN2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN2_OFFSET)
HI_SET_GET(hi_crg_srstdis2_tensi_dps0_pd_srst_dis,tensi_dps0_pd_srst_dis,HI_CRG_SRSTDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS2_OFFSET)
HI_SET_GET(hi_crg_srstdis2_tensi_dsp0_core_srst_dis,tensi_dsp0_core_srst_dis,HI_CRG_SRSTDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS2_OFFSET)
HI_SET_GET(hi_crg_srstdis2_tensi_dsp0_dbg_srst_dis,tensi_dsp0_dbg_srst_dis,HI_CRG_SRSTDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS2_OFFSET)
HI_SET_GET(hi_crg_srstdis2_reserved_3,reserved_3,HI_CRG_SRSTDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS2_OFFSET)
HI_SET_GET(hi_crg_srstdis2_func_wakeup_pd_srst_dis,func_wakeup_pd_srst_dis,HI_CRG_SRSTDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS2_OFFSET)
HI_SET_GET(hi_crg_srstdis2_soc_peri_usb_pd_srst_dis,soc_peri_usb_pd_srst_dis,HI_CRG_SRSTDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS2_OFFSET)
HI_SET_GET(hi_crg_srstdis2_mmc0_srst_dis,mmc0_srst_dis,HI_CRG_SRSTDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS2_OFFSET)
HI_SET_GET(hi_crg_srstdis2_mmc1_srst_dis,mmc1_srst_dis,HI_CRG_SRSTDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS2_OFFSET)
HI_SET_GET(hi_crg_srstdis2_sdcc_srst_dis,sdcc_srst_dis,HI_CRG_SRSTDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS2_OFFSET)
HI_SET_GET(hi_crg_srstdis2_reserved_2,reserved_2,HI_CRG_SRSTDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS2_OFFSET)
HI_SET_GET(hi_crg_srstdis2_gmac_srst_dis,gmac_srst_dis,HI_CRG_SRSTDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS2_OFFSET)
HI_SET_GET(hi_crg_srstdis2_ssicdev_ctrl_srst_dis,ssicdev_ctrl_srst_dis,HI_CRG_SRSTDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS2_OFFSET)
HI_SET_GET(hi_crg_srstdis2_ssicphy_srst_dis,ssicphy_srst_dis,HI_CRG_SRSTDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS2_OFFSET)
HI_SET_GET(hi_crg_srstdis2_pcie_ctrl_srst_dis,pcie_ctrl_srst_dis,HI_CRG_SRSTDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS2_OFFSET)
HI_SET_GET(hi_crg_srstdis2_pcie_phy_srst_dis,pcie_phy_srst_dis,HI_CRG_SRSTDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS2_OFFSET)
HI_SET_GET(hi_crg_srstdis2_ddrphy_div_srst_dis,ddrphy_div_srst_dis,HI_CRG_SRSTDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS2_OFFSET)
HI_SET_GET(hi_crg_srstdis2_reserved_1,reserved_1,HI_CRG_SRSTDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS2_OFFSET)
HI_SET_GET(hi_crg_srstdis2_hifi_core_srst_dis,hifi_core_srst_dis,HI_CRG_SRSTDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS2_OFFSET)
HI_SET_GET(hi_crg_srstdis2_hifi_pd_srst_dis,hifi_pd_srst_dis,HI_CRG_SRSTDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS2_OFFSET)
HI_SET_GET(hi_crg_srstdis2_hifi_dbg_srst_dis,hifi_dbg_srst_dis,HI_CRG_SRSTDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS2_OFFSET)
HI_SET_GET(hi_crg_srstdis2_usbotg_bc_srst_dis,usbotg_bc_srst_dis,HI_CRG_SRSTDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS2_OFFSET)
HI_SET_GET(hi_crg_srstdis2_usbotg_phy_srst_dis,usbotg_phy_srst_dis,HI_CRG_SRSTDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS2_OFFSET)
HI_SET_GET(hi_crg_srstdis2_usbotg_ctrl_srst_dis,usbotg_ctrl_srst_dis,HI_CRG_SRSTDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS2_OFFSET)
HI_SET_GET(hi_crg_srstdis2_usbotg_ctrl_vaux_srst_dis,usbotg_ctrl_vaux_srst_dis,HI_CRG_SRSTDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS2_OFFSET)
HI_SET_GET(hi_crg_srstdis2_usbotg_ctrl_vcc_srst_dis,usbotg_ctrl_vcc_srst_dis,HI_CRG_SRSTDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS2_OFFSET)
HI_SET_GET(hi_crg_srstdis2_usbhsic_ctrl_srst_dis,usbhsic_ctrl_srst_dis,HI_CRG_SRSTDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS2_OFFSET)
HI_SET_GET(hi_crg_srstdis2_usbhsic_phy_srst_dis,usbhsic_phy_srst_dis,HI_CRG_SRSTDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS2_OFFSET)
HI_SET_GET(hi_crg_srstdis2_cs_srst_dis,cs_srst_dis,HI_CRG_SRSTDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS2_OFFSET)
HI_SET_GET(hi_crg_srstdis2_tsensor_srst_dis,tsensor_srst_dis,HI_CRG_SRSTDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS2_OFFSET)
HI_SET_GET(hi_crg_srstdis2_reserved_0,reserved_0,HI_CRG_SRSTDIS2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS2_OFFSET)
HI_SET_GET(hi_crg_srststat2_tensi_dps0_pd_srst_status,tensi_dps0_pd_srst_status,HI_CRG_SRSTSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT2_OFFSET)
HI_SET_GET(hi_crg_srststat2_tensi_dsp0_core_srst_status,tensi_dsp0_core_srst_status,HI_CRG_SRSTSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT2_OFFSET)
HI_SET_GET(hi_crg_srststat2_tensi_dsp0_dbg_srst_status,tensi_dsp0_dbg_srst_status,HI_CRG_SRSTSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT2_OFFSET)
HI_SET_GET(hi_crg_srststat2_reserved_3,reserved_3,HI_CRG_SRSTSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT2_OFFSET)
HI_SET_GET(hi_crg_srststat2_func_wakeup_pd_srst_status,func_wakeup_pd_srst_status,HI_CRG_SRSTSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT2_OFFSET)
HI_SET_GET(hi_crg_srststat2_soc_peri_usb_pd_srst_status,soc_peri_usb_pd_srst_status,HI_CRG_SRSTSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT2_OFFSET)
HI_SET_GET(hi_crg_srststat2_mmc0_srst_status,mmc0_srst_status,HI_CRG_SRSTSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT2_OFFSET)
HI_SET_GET(hi_crg_srststat2_mmc1_srst_status,mmc1_srst_status,HI_CRG_SRSTSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT2_OFFSET)
HI_SET_GET(hi_crg_srststat2_sdcc_srst_status,sdcc_srst_status,HI_CRG_SRSTSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT2_OFFSET)
HI_SET_GET(hi_crg_srststat2_reserved_2,reserved_2,HI_CRG_SRSTSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT2_OFFSET)
HI_SET_GET(hi_crg_srststat2_gmac_srst_status,gmac_srst_status,HI_CRG_SRSTSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT2_OFFSET)
HI_SET_GET(hi_crg_srststat2_ssicdev_ctrl_srst_status,ssicdev_ctrl_srst_status,HI_CRG_SRSTSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT2_OFFSET)
HI_SET_GET(hi_crg_srststat2_ssicphy_srst_status,ssicphy_srst_status,HI_CRG_SRSTSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT2_OFFSET)
HI_SET_GET(hi_crg_srststat2_pcie_ctrl_srst_status,pcie_ctrl_srst_status,HI_CRG_SRSTSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT2_OFFSET)
HI_SET_GET(hi_crg_srststat2_pcie_phy_srst_status,pcie_phy_srst_status,HI_CRG_SRSTSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT2_OFFSET)
HI_SET_GET(hi_crg_srststat2_ddrphy_div_srst_status,ddrphy_div_srst_status,HI_CRG_SRSTSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT2_OFFSET)
HI_SET_GET(hi_crg_srststat2_reserved_1,reserved_1,HI_CRG_SRSTSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT2_OFFSET)
HI_SET_GET(hi_crg_srststat2_hificore_srst_status,hificore_srst_status,HI_CRG_SRSTSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT2_OFFSET)
HI_SET_GET(hi_crg_srststat2_hifi_pd_srst_status,hifi_pd_srst_status,HI_CRG_SRSTSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT2_OFFSET)
HI_SET_GET(hi_crg_srststat2_hifi_dbg_srst_status,hifi_dbg_srst_status,HI_CRG_SRSTSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT2_OFFSET)
HI_SET_GET(hi_crg_srststat2_usbotg_bc_srst_status,usbotg_bc_srst_status,HI_CRG_SRSTSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT2_OFFSET)
HI_SET_GET(hi_crg_srststat2_usbotg_phy_srst_status,usbotg_phy_srst_status,HI_CRG_SRSTSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT2_OFFSET)
HI_SET_GET(hi_crg_srststat2_usbotg_ctrl_srst_status,usbotg_ctrl_srst_status,HI_CRG_SRSTSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT2_OFFSET)
HI_SET_GET(hi_crg_srststat2_usbotg_ctrl_vaux_srst_status,usbotg_ctrl_vaux_srst_status,HI_CRG_SRSTSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT2_OFFSET)
HI_SET_GET(hi_crg_srststat2_usbotg_ctrl_vcc_srst_status,usbotg_ctrl_vcc_srst_status,HI_CRG_SRSTSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT2_OFFSET)
HI_SET_GET(hi_crg_srststat2_usbhsic_ctrl_srst_status,usbhsic_ctrl_srst_status,HI_CRG_SRSTSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT2_OFFSET)
HI_SET_GET(hi_crg_srststat2_usbhsic_phy_srst_v,usbhsic_phy_srst_v,HI_CRG_SRSTSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT2_OFFSET)
HI_SET_GET(hi_crg_srststat2_cs_srst_status,cs_srst_status,HI_CRG_SRSTSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT2_OFFSET)
HI_SET_GET(hi_crg_srststat2_tsensor_srst_status,tsensor_srst_status,HI_CRG_SRSTSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT2_OFFSET)
HI_SET_GET(hi_crg_srststat2_reserved_0,reserved_0,HI_CRG_SRSTSTAT2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT2_OFFSET)
HI_SET_GET(hi_crg_srsten3_bbp_crg_srst_en,bbp_crg_srst_en,HI_CRG_SRSTEN3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN3_OFFSET)
HI_SET_GET(hi_crg_srsten3_bbpcommon_2a_srst_en,bbpcommon_2a_srst_en,HI_CRG_SRSTEN3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN3_OFFSET)
HI_SET_GET(hi_crg_srsten3_lbbp_pd_srst_en,lbbp_pd_srst_en,HI_CRG_SRSTEN3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN3_OFFSET)
HI_SET_GET(hi_crg_srsten3_g1bbp_pd_srst_en,g1bbp_pd_srst_en,HI_CRG_SRSTEN3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN3_OFFSET)
HI_SET_GET(hi_crg_srsten3_g2bbp_pd_srst_en,g2bbp_pd_srst_en,HI_CRG_SRSTEN3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN3_OFFSET)
HI_SET_GET(hi_crg_srsten3_twbbp_pd_srst_en,twbbp_pd_srst_en,HI_CRG_SRSTEN3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN3_OFFSET)
HI_SET_GET(hi_crg_srsten3_wbbp_pd_srst_en,wbbp_pd_srst_en,HI_CRG_SRSTEN3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN3_OFFSET)
HI_SET_GET(hi_crg_srsten3_irm_pd_srst_en,irm_pd_srst_en,HI_CRG_SRSTEN3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN3_OFFSET)
HI_SET_GET(hi_crg_srsten3_bbc_lbbp_pdt_srst_en,bbc_lbbp_pdt_srst_en,HI_CRG_SRSTEN3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN3_OFFSET)
HI_SET_GET(hi_crg_srsten3_bbc_lbbp_pdf_srst_en,bbc_lbbp_pdf_srst_en,HI_CRG_SRSTEN3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN3_OFFSET)
HI_SET_GET(hi_crg_srsten3_bbc_lbbp_tdl_srst_en,bbc_lbbp_tdl_srst_en,HI_CRG_SRSTEN3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN3_OFFSET)
HI_SET_GET(hi_crg_srsten3_bbc_tbbp_245m_srst_en,bbc_tbbp_245m_srst_en,HI_CRG_SRSTEN3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN3_OFFSET)
HI_SET_GET(hi_crg_srsten3_bbc_twbbp_122m_srst_en,bbc_twbbp_122m_srst_en,HI_CRG_SRSTEN3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN3_OFFSET)
HI_SET_GET(hi_crg_srsten3_bbc_g1bbp_104m_srst_en,bbc_g1bbp_104m_srst_en,HI_CRG_SRSTEN3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN3_OFFSET)
HI_SET_GET(hi_crg_srsten3_bbc_g2bbp_104m_srst_en,bbc_g2bbp_104m_srst_en,HI_CRG_SRSTEN3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN3_OFFSET)
HI_SET_GET(hi_crg_srsten3_reserved,reserved,HI_CRG_SRSTEN3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN3_OFFSET)
HI_SET_GET(hi_crg_srsten3_abb_srst_en,abb_srst_en,HI_CRG_SRSTEN3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTEN3_OFFSET)
HI_SET_GET(hi_crg_srstdis3_bbp_crg_srst_dis,bbp_crg_srst_dis,HI_CRG_SRSTDIS3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS3_OFFSET)
HI_SET_GET(hi_crg_srstdis3_bbpcommon_2a_pd_srst_dis,bbpcommon_2a_pd_srst_dis,HI_CRG_SRSTDIS3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS3_OFFSET)
HI_SET_GET(hi_crg_srstdis3_lbbp_pd_srst_dis,lbbp_pd_srst_dis,HI_CRG_SRSTDIS3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS3_OFFSET)
HI_SET_GET(hi_crg_srstdis3_g1bbp_pd_srst_dis,g1bbp_pd_srst_dis,HI_CRG_SRSTDIS3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS3_OFFSET)
HI_SET_GET(hi_crg_srstdis3_g2bbp_pd_srst_dis,g2bbp_pd_srst_dis,HI_CRG_SRSTDIS3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS3_OFFSET)
HI_SET_GET(hi_crg_srstdis3_twbbp_pd_srst_dis,twbbp_pd_srst_dis,HI_CRG_SRSTDIS3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS3_OFFSET)
HI_SET_GET(hi_crg_srstdis3_wbbp_pd_srst_dis,wbbp_pd_srst_dis,HI_CRG_SRSTDIS3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS3_OFFSET)
HI_SET_GET(hi_crg_srstdis3_irm_pd_srst_dis,irm_pd_srst_dis,HI_CRG_SRSTDIS3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS3_OFFSET)
HI_SET_GET(hi_crg_srstdis3_bbc_lbbp_pdt_srst_dis,bbc_lbbp_pdt_srst_dis,HI_CRG_SRSTDIS3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS3_OFFSET)
HI_SET_GET(hi_crg_srstdis3_bbc_lbbp_pdf_srst_dis,bbc_lbbp_pdf_srst_dis,HI_CRG_SRSTDIS3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS3_OFFSET)
HI_SET_GET(hi_crg_srstdis3_bbc_lbbp_tdl_srst_dis,bbc_lbbp_tdl_srst_dis,HI_CRG_SRSTDIS3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS3_OFFSET)
HI_SET_GET(hi_crg_srstdis3_bbc_tbbp_245m_srst_dis,bbc_tbbp_245m_srst_dis,HI_CRG_SRSTDIS3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS3_OFFSET)
HI_SET_GET(hi_crg_srstdis3_bbc_twbbp_122m_srst_dis,bbc_twbbp_122m_srst_dis,HI_CRG_SRSTDIS3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS3_OFFSET)
HI_SET_GET(hi_crg_srstdis3_bbc_g1bbp_104m_srst_dis,bbc_g1bbp_104m_srst_dis,HI_CRG_SRSTDIS3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS3_OFFSET)
HI_SET_GET(hi_crg_srstdis3_bbc_g2bbp_104m_srst_dis,bbc_g2bbp_104m_srst_dis,HI_CRG_SRSTDIS3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS3_OFFSET)
HI_SET_GET(hi_crg_srstdis3_reserved,reserved,HI_CRG_SRSTDIS3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS3_OFFSET)
HI_SET_GET(hi_crg_srstdis3_abb_srst_dis,abb_srst_dis,HI_CRG_SRSTDIS3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTDIS3_OFFSET)
HI_SET_GET(hi_crg_srststat3_bbp_crg_srst_stat,bbp_crg_srst_stat,HI_CRG_SRSTSTAT3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT3_OFFSET)
HI_SET_GET(hi_crg_srststat3_bbpcommon_2a_srst_stat,bbpcommon_2a_srst_stat,HI_CRG_SRSTSTAT3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT3_OFFSET)
HI_SET_GET(hi_crg_srststat3_lbbp_pd_srst_stat,lbbp_pd_srst_stat,HI_CRG_SRSTSTAT3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT3_OFFSET)
HI_SET_GET(hi_crg_srststat3_g1bbp_pd_srst_stat,g1bbp_pd_srst_stat,HI_CRG_SRSTSTAT3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT3_OFFSET)
HI_SET_GET(hi_crg_srststat3_g2bbp_pd_srst_stat,g2bbp_pd_srst_stat,HI_CRG_SRSTSTAT3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT3_OFFSET)
HI_SET_GET(hi_crg_srststat3_twbbp_pd_srst_stat,twbbp_pd_srst_stat,HI_CRG_SRSTSTAT3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT3_OFFSET)
HI_SET_GET(hi_crg_srststat3_wbbp_pd_srst_stat,wbbp_pd_srst_stat,HI_CRG_SRSTSTAT3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT3_OFFSET)
HI_SET_GET(hi_crg_srststat3_irm_pd_srst_stat,irm_pd_srst_stat,HI_CRG_SRSTSTAT3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT3_OFFSET)
HI_SET_GET(hi_crg_srststat3_bbc_lbbp_pdt_srstctrl_stat,bbc_lbbp_pdt_srstctrl_stat,HI_CRG_SRSTSTAT3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT3_OFFSET)
HI_SET_GET(hi_crg_srststat3_bbc_lbbp_pdf_srstctrl_stat,bbc_lbbp_pdf_srstctrl_stat,HI_CRG_SRSTSTAT3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT3_OFFSET)
HI_SET_GET(hi_crg_srststat3_bbc_lbbp_tdl_srstctrl_stat,bbc_lbbp_tdl_srstctrl_stat,HI_CRG_SRSTSTAT3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT3_OFFSET)
HI_SET_GET(hi_crg_srststat3_bbc_tbbp_245m_srstctrl_stat,bbc_tbbp_245m_srstctrl_stat,HI_CRG_SRSTSTAT3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT3_OFFSET)
HI_SET_GET(hi_crg_srststat3_bbc_twbbp_122m_srstctrl_stat,bbc_twbbp_122m_srstctrl_stat,HI_CRG_SRSTSTAT3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT3_OFFSET)
HI_SET_GET(hi_crg_srststat3_bbc_g1bbp_104m_srstctrl_stat,bbc_g1bbp_104m_srstctrl_stat,HI_CRG_SRSTSTAT3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT3_OFFSET)
HI_SET_GET(hi_crg_srststat3_bbc_g2bbp_104m_srstctrl_stat,bbc_g2bbp_104m_srstctrl_stat,HI_CRG_SRSTSTAT3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT3_OFFSET)
HI_SET_GET(hi_crg_srststat3_reserved,reserved,HI_CRG_SRSTSTAT3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT3_OFFSET)
HI_SET_GET(hi_crg_srststat3_abb_srst_status,abb_srst_status,HI_CRG_SRSTSTAT3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_SRSTSTAT3_OFFSET)
HI_SET_GET(hi_crg_clkdiv1_pcm_bclk_div,pcm_bclk_div,HI_CRG_CLKDIV1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIV1_OFFSET)
HI_SET_GET(hi_crg_clkdiv1_pcm_sync_div,pcm_sync_div,HI_CRG_CLKDIV1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIV1_OFFSET)
HI_SET_GET(hi_crg_clkdiv1_reserved,reserved,HI_CRG_CLKDIV1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIV1_OFFSET)
HI_SET_GET(hi_crg_clkdiv2_appa9_clk_div,appa9_clk_div,HI_CRG_CLKDIV2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIV2_OFFSET)
HI_SET_GET(hi_crg_clkdiv2_mdma9_clk_div,mdma9_clk_div,HI_CRG_CLKDIV2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIV2_OFFSET)
HI_SET_GET(hi_crg_clkdiv2_soc_clk_div,soc_clk_div,HI_CRG_CLKDIV2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIV2_OFFSET)
HI_SET_GET(hi_crg_clkdiv2_fast_clk_div,fast_clk_div,HI_CRG_CLKDIV2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIV2_OFFSET)
HI_SET_GET(hi_crg_clkdiv2_reserved_2,reserved_2,HI_CRG_CLKDIV2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIV2_OFFSET)
HI_SET_GET(hi_crg_clkdiv2_ddr_rate_sel,ddr_rate_sel,HI_CRG_CLKDIV2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIV2_OFFSET)
HI_SET_GET(hi_crg_clkdiv2_reserved_1,reserved_1,HI_CRG_CLKDIV2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIV2_OFFSET)
HI_SET_GET(hi_crg_clkdiv2_apb_freqmode,apb_freqmode,HI_CRG_CLKDIV2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIV2_OFFSET)
HI_SET_GET(hi_crg_clkdiv2_reserved_0,reserved_0,HI_CRG_CLKDIV2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIV2_OFFSET)
HI_SET_GET(hi_crg_clkdiv3_mmc0_cclk_div,mmc0_cclk_div,HI_CRG_CLKDIV3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIV3_OFFSET)
HI_SET_GET(hi_crg_clkdiv3_reserved_3,reserved_3,HI_CRG_CLKDIV3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIV3_OFFSET)
HI_SET_GET(hi_crg_clkdiv3_mmc0_refclk_sel,mmc0_refclk_sel,HI_CRG_CLKDIV3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIV3_OFFSET)
HI_SET_GET(hi_crg_clkdiv3_mmc0_phase_clk_div,mmc0_phase_clk_div,HI_CRG_CLKDIV3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIV3_OFFSET)
HI_SET_GET(hi_crg_clkdiv3_reserved_2,reserved_2,HI_CRG_CLKDIV3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIV3_OFFSET)
HI_SET_GET(hi_crg_clkdiv3_mmc1_cclk_div,mmc1_cclk_div,HI_CRG_CLKDIV3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIV3_OFFSET)
HI_SET_GET(hi_crg_clkdiv3_reserved_1,reserved_1,HI_CRG_CLKDIV3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIV3_OFFSET)
HI_SET_GET(hi_crg_clkdiv3_mmc1_refclk_sel,mmc1_refclk_sel,HI_CRG_CLKDIV3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIV3_OFFSET)
HI_SET_GET(hi_crg_clkdiv3_mmc1_phase_clk_div,mmc1_phase_clk_div,HI_CRG_CLKDIV3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIV3_OFFSET)
HI_SET_GET(hi_crg_clkdiv3_reserved_0,reserved_0,HI_CRG_CLKDIV3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIV3_OFFSET)
HI_SET_GET(hi_crg_clkdiv3_hpm_clk_div,hpm_clk_div,HI_CRG_CLKDIV3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIV3_OFFSET)
HI_SET_GET(hi_crg_clkdiv3_cipher_clk_div,cipher_clk_div,HI_CRG_CLKDIV3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIV3_OFFSET)
HI_SET_GET(hi_crg_clkdiv4_hifi_clk_div_num,hifi_clk_div_num,HI_CRG_CLKDIV4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIV4_OFFSET)
HI_SET_GET(hi_crg_clkdiv4_hifi_freqmode,hifi_freqmode,HI_CRG_CLKDIV4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIV4_OFFSET)
HI_SET_GET(hi_crg_clkdiv4_hifi_refclk_sel,hifi_refclk_sel,HI_CRG_CLKDIV4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIV4_OFFSET)
HI_SET_GET(hi_crg_clkdiv4_reserved,reserved,HI_CRG_CLKDIV4_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIV4_OFFSET)
HI_SET_GET(hi_crg_clkdiv5_bbe_clk_div_num,bbe_clk_div_num,HI_CRG_CLKDIV5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIV5_OFFSET)
HI_SET_GET(hi_crg_clkdiv5_reserved_2,reserved_2,HI_CRG_CLKDIV5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIV5_OFFSET)
HI_SET_GET(hi_crg_clkdiv5_bbe_freqmode,bbe_freqmode,HI_CRG_CLKDIV5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIV5_OFFSET)
HI_SET_GET(hi_crg_clkdiv5_bbe_refclk_sel,bbe_refclk_sel,HI_CRG_CLKDIV5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIV5_OFFSET)
HI_SET_GET(hi_crg_clkdiv5_sci0_clk_div_num,sci0_clk_div_num,HI_CRG_CLKDIV5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIV5_OFFSET)
HI_SET_GET(hi_crg_clkdiv5_reserved_1,reserved_1,HI_CRG_CLKDIV5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIV5_OFFSET)
HI_SET_GET(hi_crg_clkdiv5_sci1_clk_div_num,sci1_clk_div_num,HI_CRG_CLKDIV5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIV5_OFFSET)
HI_SET_GET(hi_crg_clkdiv5_reserved_0,reserved_0,HI_CRG_CLKDIV5_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIV5_OFFSET)
HI_SET_GET(hi_crg_mmc_ctrl_mmc0_drv_sel,mmc0_drv_sel,HI_CRG_MMC_CTRL_T,HI_SYSCRG_BASE_ADDR, HI_CRG_MMC_CTRL_OFFSET)
HI_SET_GET(hi_crg_mmc_ctrl_mmc0_clk_bypass,mmc0_clk_bypass,HI_CRG_MMC_CTRL_T,HI_SYSCRG_BASE_ADDR, HI_CRG_MMC_CTRL_OFFSET)
HI_SET_GET(hi_crg_mmc_ctrl_mmc0_sample_sel,mmc0_sample_sel,HI_CRG_MMC_CTRL_T,HI_SYSCRG_BASE_ADDR, HI_CRG_MMC_CTRL_OFFSET)
HI_SET_GET(hi_crg_mmc_ctrl_mmc0_sample_ctrl,mmc0_sample_ctrl,HI_CRG_MMC_CTRL_T,HI_SYSCRG_BASE_ADDR, HI_CRG_MMC_CTRL_OFFSET)
HI_SET_GET(hi_crg_mmc_ctrl_mmc1_drv_sel,mmc1_drv_sel,HI_CRG_MMC_CTRL_T,HI_SYSCRG_BASE_ADDR, HI_CRG_MMC_CTRL_OFFSET)
HI_SET_GET(hi_crg_mmc_ctrl_mmc1_clk_bypass,mmc1_clk_bypass,HI_CRG_MMC_CTRL_T,HI_SYSCRG_BASE_ADDR, HI_CRG_MMC_CTRL_OFFSET)
HI_SET_GET(hi_crg_mmc_ctrl_mmc1_sample_sel,mmc1_sample_sel,HI_CRG_MMC_CTRL_T,HI_SYSCRG_BASE_ADDR, HI_CRG_MMC_CTRL_OFFSET)
HI_SET_GET(hi_crg_mmc_ctrl_mmc1_sample_ctrl,mmc1_sample_ctrl,HI_CRG_MMC_CTRL_T,HI_SYSCRG_BASE_ADDR, HI_CRG_MMC_CTRL_OFFSET)
HI_SET_GET(hi_crg_plllock_ctrl_pll_lock_ctrl,pll_lock_ctrl,HI_CRG_PLLLOCK_CTRL_T,HI_SYSCRG_BASE_ADDR, HI_CRG_PLLLOCK_CTRL_OFFSET)
HI_SET_GET(hi_crg_plllock_ctrl_pll_clk_dis_cfg,pll_clk_dis_cfg,HI_CRG_PLLLOCK_CTRL_T,HI_SYSCRG_BASE_ADDR, HI_CRG_PLLLOCK_CTRL_OFFSET)
HI_SET_GET(hi_crg_plllock_ctrl_pll_lock_mode,pll_lock_mode,HI_CRG_PLLLOCK_CTRL_T,HI_SYSCRG_BASE_ADDR, HI_CRG_PLLLOCK_CTRL_OFFSET)
HI_SET_GET(hi_crg_clk_sel1_soc_tcxo_refclk_sel,soc_tcxo_refclk_sel,HI_CRG_CLK_SEL1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLK_SEL1_OFFSET)
HI_SET_GET(hi_crg_clk_sel1_pmu_tcxo_clk_sel,pmu_tcxo_clk_sel,HI_CRG_CLK_SEL1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLK_SEL1_OFFSET)
HI_SET_GET(hi_crg_clk_sel1_gps_tcxo_clk_sel,gps_tcxo_clk_sel,HI_CRG_CLK_SEL1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLK_SEL1_OFFSET)
HI_SET_GET(hi_crg_clk_sel1_wifi_tcxo_clk_sel,wifi_tcxo_clk_sel,HI_CRG_CLK_SEL1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLK_SEL1_OFFSET)
HI_SET_GET(hi_crg_clk_sel1_sleep_clk_sel,sleep_clk_sel,HI_CRG_CLK_SEL1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLK_SEL1_OFFSET)
HI_SET_GET(hi_crg_clk_sel1_uart0_clk_sel,uart0_clk_sel,HI_CRG_CLK_SEL1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLK_SEL1_OFFSET)
HI_SET_GET(hi_crg_clk_sel1_sio_clk_sel,sio_clk_sel,HI_CRG_CLK_SEL1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLK_SEL1_OFFSET)
HI_SET_GET(hi_crg_clk_sel1_cs_clk_sel,cs_clk_sel,HI_CRG_CLK_SEL1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLK_SEL1_OFFSET)
HI_SET_GET(hi_crg_clk_sel1_a9pll_clk_sel,a9pll_clk_sel,HI_CRG_CLK_SEL1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLK_SEL1_OFFSET)
HI_SET_GET(hi_crg_clk_sel1_reserved,reserved,HI_CRG_CLK_SEL1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLK_SEL1_OFFSET)
HI_SET_GET(hi_crg_clk_sel1_pwm0_clk_sel,pwm0_clk_sel,HI_CRG_CLK_SEL1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLK_SEL1_OFFSET)
HI_SET_GET(hi_crg_clk_sel1_pwm1_clk_sel,pwm1_clk_sel,HI_CRG_CLK_SEL1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLK_SEL1_OFFSET)
HI_SET_GET(hi_crg_clk_sel1_tcssi0_clk_sel,tcssi0_clk_sel,HI_CRG_CLK_SEL1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLK_SEL1_OFFSET)
HI_SET_GET(hi_crg_clk_sel1_tcssi1_clk_sel,tcssi1_clk_sel,HI_CRG_CLK_SEL1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLK_SEL1_OFFSET)
HI_SET_GET(hi_crg_clk_sel1_ushsic_utmi_clk_dis,ushsic_utmi_clk_dis,HI_CRG_CLK_SEL1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLK_SEL1_OFFSET)
HI_SET_GET(hi_crg_clk_sel1_usbhsic_utmi_clk_sel,usbhsic_utmi_clk_sel,HI_CRG_CLK_SEL1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLK_SEL1_OFFSET)
HI_SET_GET(hi_crg_clk_sel1_hpm_refclk_sel,hpm_refclk_sel,HI_CRG_CLK_SEL1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLK_SEL1_OFFSET)
HI_SET_GET(hi_crg_clk_sel1_a9_wd_srst_en,a9_wd_srst_en,HI_CRG_CLK_SEL1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLK_SEL1_OFFSET)
HI_SET_GET(hi_crg_clk_sel1_gmac_rx_clk_sel,gmac_rx_clk_sel,HI_CRG_CLK_SEL1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLK_SEL1_OFFSET)
HI_SET_GET(hi_crg_clk_sel1_gmac_tx_clk_sel,gmac_tx_clk_sel,HI_CRG_CLK_SEL1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLK_SEL1_OFFSET)
HI_SET_GET(hi_crg_clk_sel1_gmac_rtx_clk_sel,gmac_rtx_clk_sel,HI_CRG_CLK_SEL1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLK_SEL1_OFFSET)
HI_SET_GET(hi_crg_clk_sel1_usbphy_refclk_sel,usbphy_refclk_sel,HI_CRG_CLK_SEL1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLK_SEL1_OFFSET)
HI_SET_GET(hi_crg_clk_sel1_peripll_refclk_sel,peripll_refclk_sel,HI_CRG_CLK_SEL1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLK_SEL1_OFFSET)
HI_SET_GET(hi_crg_clk_sel1_cs_refclk_sel,cs_refclk_sel,HI_CRG_CLK_SEL1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLK_SEL1_OFFSET)
HI_SET_GET(hi_crg_clk_sel1_mmc1_refclk_sel,mmc1_refclk_sel,HI_CRG_CLK_SEL1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLK_SEL1_OFFSET)
HI_SET_GET(hi_crg_clk_sel1_sio_refclk_sel,sio_refclk_sel,HI_CRG_CLK_SEL1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLK_SEL1_OFFSET)
HI_SET_GET(hi_crg_clk_sel1_mmc0_refclk_sel,mmc0_refclk_sel,HI_CRG_CLK_SEL1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLK_SEL1_OFFSET)
HI_SET_GET(hi_crg_clk_sel2_timer0_6_clk_sel,timer0_6_clk_sel,HI_CRG_CLK_SEL2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLK_SEL2_OFFSET)
HI_SET_GET(hi_crg_clk_sel2_reserved_3,reserved_3,HI_CRG_CLK_SEL2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLK_SEL2_OFFSET)
HI_SET_GET(hi_crg_clk_sel2_timer8_14_clk_sel,timer8_14_clk_sel,HI_CRG_CLK_SEL2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLK_SEL2_OFFSET)
HI_SET_GET(hi_crg_clk_sel2_reserved_2,reserved_2,HI_CRG_CLK_SEL2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLK_SEL2_OFFSET)
HI_SET_GET(hi_crg_clk_sel2_timer16_22_clk_sel,timer16_22_clk_sel,HI_CRG_CLK_SEL2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLK_SEL2_OFFSET)
HI_SET_GET(hi_crg_clk_sel2_reserved_1,reserved_1,HI_CRG_CLK_SEL2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLK_SEL2_OFFSET)
HI_SET_GET(hi_crg_clk_sel2_timer7_clk_sel,timer7_clk_sel,HI_CRG_CLK_SEL2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLK_SEL2_OFFSET)
HI_SET_GET(hi_crg_clk_sel2_timer15_clk_sel,timer15_clk_sel,HI_CRG_CLK_SEL2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLK_SEL2_OFFSET)
HI_SET_GET(hi_crg_clk_sel2_timer23_clk_sel,timer23_clk_sel,HI_CRG_CLK_SEL2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLK_SEL2_OFFSET)
HI_SET_GET(hi_crg_clk_sel2_reserved_0,reserved_0,HI_CRG_CLK_SEL2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLK_SEL2_OFFSET)
HI_SET_GET(hi_crg_clk_sel3_abb_tcxo_clk_sel,abb_tcxo_clk_sel,HI_CRG_CLK_SEL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLK_SEL3_OFFSET)
HI_SET_GET(hi_crg_clk_sel3_bbppll_refclk_sel,bbppll_refclk_sel,HI_CRG_CLK_SEL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLK_SEL3_OFFSET)
HI_SET_GET(hi_crg_clk_sel3_bbp_tcxo_clk_sel,bbp_tcxo_clk_sel,HI_CRG_CLK_SEL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLK_SEL3_OFFSET)
HI_SET_GET(hi_crg_clk_sel3_ch0_tcxo_clk_sel,ch0_tcxo_clk_sel,HI_CRG_CLK_SEL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLK_SEL3_OFFSET)
HI_SET_GET(hi_crg_clk_sel3_ch1_tcxo_clk_sel,ch1_tcxo_clk_sel,HI_CRG_CLK_SEL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLK_SEL3_OFFSET)
HI_SET_GET(hi_crg_clk_sel3_g1bbp_104m_clk_sel,g1bbp_104m_clk_sel,HI_CRG_CLK_SEL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLK_SEL3_OFFSET)
HI_SET_GET(hi_crg_clk_sel3_g2bbp_104m_clk_sel,g2bbp_104m_clk_sel,HI_CRG_CLK_SEL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLK_SEL3_OFFSET)
HI_SET_GET(hi_crg_clk_sel3_tdl_clk_sel,tdl_clk_sel,HI_CRG_CLK_SEL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLK_SEL3_OFFSET)
HI_SET_GET(hi_crg_clk_sel3_lbbp_vdl_clk_sel,lbbp_vdl_clk_sel,HI_CRG_CLK_SEL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLK_SEL3_OFFSET)
HI_SET_GET(hi_crg_clk_sel3_lbbp_pdf_clk_sel,lbbp_pdf_clk_sel,HI_CRG_CLK_SEL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLK_SEL3_OFFSET)
HI_SET_GET(hi_crg_clk_sel3_reserved,reserved,HI_CRG_CLK_SEL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLK_SEL3_OFFSET)
HI_SET_GET(hi_crg_dfs1_ctrl1_dfs_frac,dfs_frac,HI_CRG_DFS1_CTRL1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS1_CTRL1_OFFSET)
HI_SET_GET(hi_crg_dfs1_ctrl1_reserved,reserved,HI_CRG_DFS1_CTRL1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS1_CTRL1_OFFSET)
HI_SET_GET(hi_crg_dfs1_ctrl2_dfs_intprog,dfs_intprog,HI_CRG_DFS1_CTRL2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS1_CTRL2_OFFSET)
HI_SET_GET(hi_crg_dfs1_ctrl2_dfs_refdiv,dfs_refdiv,HI_CRG_DFS1_CTRL2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS1_CTRL2_OFFSET)
HI_SET_GET(hi_crg_dfs1_ctrl2_reserved_2,reserved_2,HI_CRG_DFS1_CTRL2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS1_CTRL2_OFFSET)
HI_SET_GET(hi_crg_dfs1_ctrl2_dfs_postdiv2,dfs_postdiv2,HI_CRG_DFS1_CTRL2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS1_CTRL2_OFFSET)
HI_SET_GET(hi_crg_dfs1_ctrl2_reserved_1,reserved_1,HI_CRG_DFS1_CTRL2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS1_CTRL2_OFFSET)
HI_SET_GET(hi_crg_dfs1_ctrl2_dfs_postdiv1,dfs_postdiv1,HI_CRG_DFS1_CTRL2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS1_CTRL2_OFFSET)
HI_SET_GET(hi_crg_dfs1_ctrl2_reserved_0,reserved_0,HI_CRG_DFS1_CTRL2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS1_CTRL2_OFFSET)
HI_SET_GET(hi_crg_dfs1_ctrl3_pd,pd,HI_CRG_DFS1_CTRL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS1_CTRL3_OFFSET)
HI_SET_GET(hi_crg_dfs1_ctrl3_reserved_1,reserved_1,HI_CRG_DFS1_CTRL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS1_CTRL3_OFFSET)
HI_SET_GET(hi_crg_dfs1_ctrl3_dsmpd,dsmpd,HI_CRG_DFS1_CTRL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS1_CTRL3_OFFSET)
HI_SET_GET(hi_crg_dfs1_ctrl3_phase4_pd,phase4_pd,HI_CRG_DFS1_CTRL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS1_CTRL3_OFFSET)
HI_SET_GET(hi_crg_dfs1_ctrl3_postdivpd,postdivpd,HI_CRG_DFS1_CTRL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS1_CTRL3_OFFSET)
HI_SET_GET(hi_crg_dfs1_ctrl3_vcopd,vcopd,HI_CRG_DFS1_CTRL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS1_CTRL3_OFFSET)
HI_SET_GET(hi_crg_dfs1_ctrl3_dacpd,dacpd,HI_CRG_DFS1_CTRL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS1_CTRL3_OFFSET)
HI_SET_GET(hi_crg_dfs1_ctrl3_bypass,bypass,HI_CRG_DFS1_CTRL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS1_CTRL3_OFFSET)
HI_SET_GET(hi_crg_dfs1_ctrl3_pll_ctrl,pll_ctrl,HI_CRG_DFS1_CTRL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS1_CTRL3_OFFSET)
HI_SET_GET(hi_crg_dfs1_ctrl3_reserved_0,reserved_0,HI_CRG_DFS1_CTRL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS1_CTRL3_OFFSET)
HI_SET_GET(hi_crg_dfs1_ctrl3_lock,lock,HI_CRG_DFS1_CTRL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS1_CTRL3_OFFSET)
HI_SET_GET(hi_crg_dfs2_ctrl1_dfs_frac,dfs_frac,HI_CRG_DFS2_CTRL1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS2_CTRL1_OFFSET)
HI_SET_GET(hi_crg_dfs2_ctrl1_reserved,reserved,HI_CRG_DFS2_CTRL1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS2_CTRL1_OFFSET)
HI_SET_GET(hi_crg_dfs2_ctrl2_dfs_intprog,dfs_intprog,HI_CRG_DFS2_CTRL2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS2_CTRL2_OFFSET)
HI_SET_GET(hi_crg_dfs2_ctrl2_dfs_refdiv,dfs_refdiv,HI_CRG_DFS2_CTRL2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS2_CTRL2_OFFSET)
HI_SET_GET(hi_crg_dfs2_ctrl2_reserved_2,reserved_2,HI_CRG_DFS2_CTRL2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS2_CTRL2_OFFSET)
HI_SET_GET(hi_crg_dfs2_ctrl2_dfs_postdiv2,dfs_postdiv2,HI_CRG_DFS2_CTRL2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS2_CTRL2_OFFSET)
HI_SET_GET(hi_crg_dfs2_ctrl2_reserved_1,reserved_1,HI_CRG_DFS2_CTRL2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS2_CTRL2_OFFSET)
HI_SET_GET(hi_crg_dfs2_ctrl2_dfs_postdiv1,dfs_postdiv1,HI_CRG_DFS2_CTRL2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS2_CTRL2_OFFSET)
HI_SET_GET(hi_crg_dfs2_ctrl2_reserved_0,reserved_0,HI_CRG_DFS2_CTRL2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS2_CTRL2_OFFSET)
HI_SET_GET(hi_crg_dfs2_ctrl3_pd,pd,HI_CRG_DFS2_CTRL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS2_CTRL3_OFFSET)
HI_SET_GET(hi_crg_dfs2_ctrl3_reserved_1,reserved_1,HI_CRG_DFS2_CTRL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS2_CTRL3_OFFSET)
HI_SET_GET(hi_crg_dfs2_ctrl3_dsmpd,dsmpd,HI_CRG_DFS2_CTRL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS2_CTRL3_OFFSET)
HI_SET_GET(hi_crg_dfs2_ctrl3_phase4_pd,phase4_pd,HI_CRG_DFS2_CTRL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS2_CTRL3_OFFSET)
HI_SET_GET(hi_crg_dfs2_ctrl3_postdivpd,postdivpd,HI_CRG_DFS2_CTRL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS2_CTRL3_OFFSET)
HI_SET_GET(hi_crg_dfs2_ctrl3_vcopd,vcopd,HI_CRG_DFS2_CTRL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS2_CTRL3_OFFSET)
HI_SET_GET(hi_crg_dfs2_ctrl3_dacpd,dacpd,HI_CRG_DFS2_CTRL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS2_CTRL3_OFFSET)
HI_SET_GET(hi_crg_dfs2_ctrl3_bypass,bypass,HI_CRG_DFS2_CTRL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS2_CTRL3_OFFSET)
HI_SET_GET(hi_crg_dfs2_ctrl3_reserved_0,reserved_0,HI_CRG_DFS2_CTRL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS2_CTRL3_OFFSET)
HI_SET_GET(hi_crg_dfs2_ctrl3_lock,lock,HI_CRG_DFS2_CTRL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS2_CTRL3_OFFSET)
HI_SET_GET(hi_crg_dfs3_ctrl1_reserved,reserved,HI_CRG_DFS3_CTRL1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS3_CTRL1_OFFSET)
HI_SET_GET(hi_crg_dfs3_ctrl2_reserved,reserved,HI_CRG_DFS3_CTRL2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS3_CTRL2_OFFSET)
HI_SET_GET(hi_crg_dfs3_ctrl3_reserved,reserved,HI_CRG_DFS3_CTRL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS3_CTRL3_OFFSET)
HI_SET_GET(hi_crg_dfs4_ctrl1_reserved,reserved,HI_CRG_DFS4_CTRL1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS4_CTRL1_OFFSET)
HI_SET_GET(hi_crg_dfs4_ctrl2_reserved,reserved,HI_CRG_DFS4_CTRL2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS4_CTRL2_OFFSET)
HI_SET_GET(hi_crg_dfs4_ctrl3_reserved,reserved,HI_CRG_DFS4_CTRL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS4_CTRL3_OFFSET)
HI_SET_GET(hi_crg_dfs5_ctrl1_dfs_frac,dfs_frac,HI_CRG_DFS5_CTRL1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS5_CTRL1_OFFSET)
HI_SET_GET(hi_crg_dfs5_ctrl1_reserved,reserved,HI_CRG_DFS5_CTRL1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS5_CTRL1_OFFSET)
HI_SET_GET(hi_crg_dfs5_ctrl2_dfs_intprog,dfs_intprog,HI_CRG_DFS5_CTRL2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS5_CTRL2_OFFSET)
HI_SET_GET(hi_crg_dfs5_ctrl2_dfs_refdiv,dfs_refdiv,HI_CRG_DFS5_CTRL2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS5_CTRL2_OFFSET)
HI_SET_GET(hi_crg_dfs5_ctrl2_reserved_2,reserved_2,HI_CRG_DFS5_CTRL2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS5_CTRL2_OFFSET)
HI_SET_GET(hi_crg_dfs5_ctrl2_dfs_postdiv2,dfs_postdiv2,HI_CRG_DFS5_CTRL2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS5_CTRL2_OFFSET)
HI_SET_GET(hi_crg_dfs5_ctrl2_reserved_1,reserved_1,HI_CRG_DFS5_CTRL2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS5_CTRL2_OFFSET)
HI_SET_GET(hi_crg_dfs5_ctrl2_dfs_postdiv1,dfs_postdiv1,HI_CRG_DFS5_CTRL2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS5_CTRL2_OFFSET)
HI_SET_GET(hi_crg_dfs5_ctrl2_reserved_0,reserved_0,HI_CRG_DFS5_CTRL2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS5_CTRL2_OFFSET)
HI_SET_GET(hi_crg_dfs5_ctrl3_pd,pd,HI_CRG_DFS5_CTRL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS5_CTRL3_OFFSET)
HI_SET_GET(hi_crg_dfs5_ctrl3_reserved_1,reserved_1,HI_CRG_DFS5_CTRL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS5_CTRL3_OFFSET)
HI_SET_GET(hi_crg_dfs5_ctrl3_dsmpd,dsmpd,HI_CRG_DFS5_CTRL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS5_CTRL3_OFFSET)
HI_SET_GET(hi_crg_dfs5_ctrl3_phase4_pd,phase4_pd,HI_CRG_DFS5_CTRL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS5_CTRL3_OFFSET)
HI_SET_GET(hi_crg_dfs5_ctrl3_postdivpd,postdivpd,HI_CRG_DFS5_CTRL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS5_CTRL3_OFFSET)
HI_SET_GET(hi_crg_dfs5_ctrl3_vcopd,vcopd,HI_CRG_DFS5_CTRL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS5_CTRL3_OFFSET)
HI_SET_GET(hi_crg_dfs5_ctrl3_dacpd,dacpd,HI_CRG_DFS5_CTRL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS5_CTRL3_OFFSET)
HI_SET_GET(hi_crg_dfs5_ctrl3_bypass,bypass,HI_CRG_DFS5_CTRL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS5_CTRL3_OFFSET)
HI_SET_GET(hi_crg_dfs5_ctrl3_pll_ctrl,pll_ctrl,HI_CRG_DFS5_CTRL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS5_CTRL3_OFFSET)
HI_SET_GET(hi_crg_dfs5_ctrl3_reserved_0,reserved_0,HI_CRG_DFS5_CTRL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS5_CTRL3_OFFSET)
HI_SET_GET(hi_crg_dfs5_ctrl3_lock,lock,HI_CRG_DFS5_CTRL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS5_CTRL3_OFFSET)
HI_SET_GET(hi_crg_dfs6_ctrl1_dfs_frac,dfs_frac,HI_CRG_DFS6_CTRL1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS6_CTRL1_OFFSET)
HI_SET_GET(hi_crg_dfs6_ctrl1_reserved,reserved,HI_CRG_DFS6_CTRL1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS6_CTRL1_OFFSET)
HI_SET_GET(hi_crg_dfs6_ctrl2_dfs_intprog,dfs_intprog,HI_CRG_DFS6_CTRL2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS6_CTRL2_OFFSET)
HI_SET_GET(hi_crg_dfs6_ctrl2_dfs_refdiv,dfs_refdiv,HI_CRG_DFS6_CTRL2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS6_CTRL2_OFFSET)
HI_SET_GET(hi_crg_dfs6_ctrl2_reserved_2,reserved_2,HI_CRG_DFS6_CTRL2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS6_CTRL2_OFFSET)
HI_SET_GET(hi_crg_dfs6_ctrl2_dfs_postdiv2,dfs_postdiv2,HI_CRG_DFS6_CTRL2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS6_CTRL2_OFFSET)
HI_SET_GET(hi_crg_dfs6_ctrl2_reserved_1,reserved_1,HI_CRG_DFS6_CTRL2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS6_CTRL2_OFFSET)
HI_SET_GET(hi_crg_dfs6_ctrl2_dfs_postdiv1,dfs_postdiv1,HI_CRG_DFS6_CTRL2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS6_CTRL2_OFFSET)
HI_SET_GET(hi_crg_dfs6_ctrl2_reserved_0,reserved_0,HI_CRG_DFS6_CTRL2_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS6_CTRL2_OFFSET)
HI_SET_GET(hi_crg_dfs6_ctrl3_pd,pd,HI_CRG_DFS6_CTRL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS6_CTRL3_OFFSET)
HI_SET_GET(hi_crg_dfs6_ctrl3_reserved_1,reserved_1,HI_CRG_DFS6_CTRL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS6_CTRL3_OFFSET)
HI_SET_GET(hi_crg_dfs6_ctrl3_dsmpd,dsmpd,HI_CRG_DFS6_CTRL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS6_CTRL3_OFFSET)
HI_SET_GET(hi_crg_dfs6_ctrl3_phase4_pd,phase4_pd,HI_CRG_DFS6_CTRL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS6_CTRL3_OFFSET)
HI_SET_GET(hi_crg_dfs6_ctrl3_postdivpd,postdivpd,HI_CRG_DFS6_CTRL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS6_CTRL3_OFFSET)
HI_SET_GET(hi_crg_dfs6_ctrl3_vcopd,vcopd,HI_CRG_DFS6_CTRL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS6_CTRL3_OFFSET)
HI_SET_GET(hi_crg_dfs6_ctrl3_dacpd,dacpd,HI_CRG_DFS6_CTRL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS6_CTRL3_OFFSET)
HI_SET_GET(hi_crg_dfs6_ctrl3_bypass,bypass,HI_CRG_DFS6_CTRL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS6_CTRL3_OFFSET)
HI_SET_GET(hi_crg_dfs6_ctrl3_pll_ctrl,pll_ctrl,HI_CRG_DFS6_CTRL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS6_CTRL3_OFFSET)
HI_SET_GET(hi_crg_dfs6_ctrl3_reserved_0,reserved_0,HI_CRG_DFS6_CTRL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS6_CTRL3_OFFSET)
HI_SET_GET(hi_crg_dfs6_ctrl3_lock,lock,HI_CRG_DFS6_CTRL3_T,HI_SYSCRG_BASE_ADDR, HI_CRG_DFS6_CTRL3_OFFSET)
HI_SET_GET(hi_crg_clksw_stat1_uart0_clksw_stat,uart0_clksw_stat,HI_CRG_CLKSW_STAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSW_STAT1_OFFSET)
HI_SET_GET(hi_crg_clksw_stat1_socpll_refclksw_stat,socpll_refclksw_stat,HI_CRG_CLKSW_STAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSW_STAT1_OFFSET)
HI_SET_GET(hi_crg_clksw_stat1_dsp_ref_clksw_stat,dsp_ref_clksw_stat,HI_CRG_CLKSW_STAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSW_STAT1_OFFSET)
HI_SET_GET(hi_crg_clksw_stat1_hifi_ref_clksw_stat,hifi_ref_clksw_stat,HI_CRG_CLKSW_STAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSW_STAT1_OFFSET)
HI_SET_GET(hi_crg_clksw_stat1_pwm0_clksw_stat,pwm0_clksw_stat,HI_CRG_CLKSW_STAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSW_STAT1_OFFSET)
HI_SET_GET(hi_crg_clksw_stat1_pwm1_clksw_stat,pwm1_clksw_stat,HI_CRG_CLKSW_STAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSW_STAT1_OFFSET)
HI_SET_GET(hi_crg_clksw_stat1_ssic_pa_ref_clksw_stat,ssic_pa_ref_clksw_stat,HI_CRG_CLKSW_STAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSW_STAT1_OFFSET)
HI_SET_GET(hi_crg_clksw_stat1_a9_pll_clksw_stat,a9_pll_clksw_stat,HI_CRG_CLKSW_STAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSW_STAT1_OFFSET)
HI_SET_GET(hi_crg_clksw_stat1_reserved,reserved,HI_CRG_CLKSW_STAT1_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKSW_STAT1_OFFSET)
HI_SET_GET(hi_crg_clkdiv6_cm3_clk_div,cm3_clk_div,HI_CRG_CLKDIV6_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIV6_OFFSET)
HI_SET_GET(hi_crg_clkdiv6_reserved,reserved,HI_CRG_CLKDIV6_T,HI_SYSCRG_BASE_ADDR, HI_CRG_CLKDIV6_OFFSET)

#endif

#endif // __HI_SYSCRG_H__

