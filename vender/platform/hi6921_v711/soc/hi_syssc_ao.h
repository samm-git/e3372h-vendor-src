/******************************************************************************/
/*  Copyright (C), 2007-2014, Hisilicon Technologies Co., Ltd. */
/******************************************************************************/
/* File name     : hi_syssc_ao.h */
/* Version       : 2.0 */
/* Author        : xxx*/
/* Created       : 2014-02-18*/
/* Last Modified : */
/* Description   :  The C union definition file for the module syssc_ao*/
/* Function List : */
/* History       : */
/* 1 Date        : */
/* Author        : xxx*/
/* Modification  : Create file */
/******************************************************************************/

#ifndef __HI_SYSSC_AO_H__
#define __HI_SYSSC_AO_H__

/*
 * Project: hi
 * Module : syssc_ao
 */

#ifndef HI_SET_GET
#define HI_SET_GET(a0,a1,a2,a3,a4)
#endif

/********************************************************************************/
/*    syssc_ao 寄存器偏移定义（项目名_模块名_寄存器名_OFFSET)        */
/********************************************************************************/
#define    HI_SC_CTRL0_OFFSET                                (0x400) /* Remap控制寄存器。 */
#define    HI_SC_CTRL1_OFFSET                                (0x404) /* PMU控制/EFUSE JTAG位寄存器。 */
#define    HI_SC_CTRL2_OFFSET                                (0x408) /* 看门狗配置寄存器。 */
#define    HI_SC_CTRL3_OFFSET                                (0x40C) /* 外设配置寄存器。 */
#define    HI_SC_CTRL4_OFFSET                                (0x410) /* CICOM加解密模式配置寄存器。 */
#define    HI_SC_CTRL5_OFFSET                                (0x414) /* App A9启动地址配置寄存器。 */
#define    HI_SC_CTRL6_OFFSET                                (0x418) /* Mod A9启动地址配置寄存器。 */
#define    HI_SC_CTRL7_OFFSET                                (0x41C) /* a9 热启动地址控制 */
#define    HI_SC_CTRL8_OFFSET                                (0x420) /* HIFI2启动地址配置寄存器。 */
#define    HI_SC_CTRL9_OFFSET                                (0x424) /* dsp0启动地址配置寄存器。 */
#define    HI_SC_CTRL10_OFFSET                               (0x428) /* App A9配置寄存器。 */
#define    HI_SC_CTRL11_OFFSET                               (0x42C) /* Modem A9配置寄存器。 */
#define    HI_SC_CTRL12_OFFSET                               (0x430) /* HIFI2 DSP CORE 配置寄存器。 */
#define    HI_SC_CTRL13_OFFSET                               (0x434) /* dsp0 DSP CORE 配置寄存器。 */
#define    HI_SC_CTRL14_OFFSET                               (0x438) /* 保留寄存器。 */
#define    HI_SC_CTRL15_OFFSET                               (0x43C) /* M3配置寄存器0。 */
#define    HI_SC_CTRL16_OFFSET                               (0x440) /* M3配置寄存器1。 */
#define    HI_SC_CTRL17_OFFSET                               (0x444) /* HPM控制寄存器。 */
#define    HI_SC_CTRL18_OFFSET                               (0x448) /* coresight event寄存器。 */
#define    HI_SC_CTRL19_OFFSET                               (0x44C) /* 保留寄存器。 */
#define    HI_SC_CTRL20_OFFSET                               (0x450) /* 外设自动门控时钟控制寄存器0。 */
#define    HI_SC_CTRL21_OFFSET                               (0x454) /* 外设自动门控时钟控制寄存器1。 */
#define    HI_SC_CTRL22_OFFSET                               (0x458) /* memory 时序配置寄存器0。 */
#define    HI_SC_CTRL23_OFFSET                               (0x45C) /* memory 时序配置寄存器1。 */
#define    HI_SC_CTRL24_OFFSET                               (0x460) /* memory 时序配置寄存器2。 */
#define    HI_SC_CTRL25_OFFSET                               (0x464) /* SOC 测试信号选择寄存器 */
#define    HI_SC_CTRL26_OFFSET                               (0x468) /* SOC 测试信号bypass寄存器。 */
#define    HI_SC_CTRL27_OFFSET                               (0x46C) /* Coresight Debug配置寄存器。 */
#define    HI_SC_CTRL28_OFFSET                               (0x470) /* 地址过滤首地址。 */
#define    HI_SC_CTRL29_OFFSET                               (0x474) /* 地址过滤尾地址。 */
#define    HI_SC_CTRL30_OFFSET                               (0x478) /* SCI配置寄存器。 */
#define    HI_SC_CTRL31_OFFSET                               (0x47C) /* 保留寄存器。 */
#define    HI_SC_CTRL32_OFFSET                               (0x480) /* EBI控制寄存器。 */
#define    HI_SC_CTRL33_OFFSET                               (0x484) /* dram_test测试模式寄存器。 */
#define    HI_SC_CTRL34_OFFSET                               (0x488) /* HSICPHY/HSIC控制器配置寄存器。 */
#define    HI_SC_CTRL35_OFFSET                               (0x48C) /* HSICPHY测试控制寄存器。 */
#define    HI_SC_CTRL36_OFFSET                               (0x490) /* USBPHY控制寄存器0。 */
#define    HI_SC_CTRL37_OFFSET                               (0x494) /* USBPHY控制寄存器1。 */
#define    HI_SC_CTRL38_OFFSET                               (0x498) /* USBPHY控制寄存器2。 */
#define    HI_SC_CTRL39_OFFSET                               (0x49C) /* USBPHY控制寄存器3。 */
#define    HI_SC_CTRL40_OFFSET                               (0x4A0) /* USBPHY控制寄存器4。 */
#define    HI_SC_CTRL41_OFFSET                               (0x4A4) /* USB Controller 控制寄存器0 */
#define    HI_SC_CTRL42_OFFSET                               (0x4A8) /* USB Controller 控制寄存器1 */
#define    HI_SC_CTRL43_OFFSET                               (0x4AC) /* USB Controller 控制寄存器2 */
#define    HI_SC_CTRL44_OFFSET                               (0x4B0) /* SSIC PHY控制寄存器。 */
#define    HI_SC_CTRL45_OFFSET                               (0x4B4) /* uicc、中断合并控制寄存器。 */
#define    HI_SC_CTRL46_OFFSET                               (0x4B8) /* 中断合并超时寄存器0。 */
#define    HI_SC_CTRL47_OFFSET                               (0x4BC) /* 中断合并超时寄存器1。 */
#define    HI_SC_CTRL48_OFFSET                               (0x4C0) /* CPU软中断置位寄存器。 */
#define    HI_SC_CTRL49_OFFSET                               (0x4C4) /* CPU软中断清零寄存器。 */
#define    HI_SC_CTRL50_OFFSET                               (0x4C8) /* CPU软中断状态寄存器。 */
#define    HI_SC_CTRL51_OFFSET                               (0x4CC) /* M3不可屏蔽中断寄存器。 */
#define    HI_SC_CTRL52_OFFSET                               (0x4D0) /* dsp0不可屏蔽中断寄存器。 */
#define    HI_SC_CTRL53_OFFSET                               (0x4D4) /* HIFI DSP不可屏蔽中断寄存器。 */
#define    HI_SC_CTRL54_OFFSET                               (0x4D8) /* DMA选择控制寄存器。 */
#define    HI_SC_CTRL55_OFFSET                               (0x4DC) /* memory低功耗配置寄存器0。 */
#define    HI_SC_CTRL56_OFFSET                               (0x4E0) /* BBP LBUS控制寄存器。 */
#define    HI_SC_CTRL57_OFFSET                               (0x4E4) /* BBP保留控制寄存器。 */
#define    HI_SC_CTRL68_OFFSET                               (0x510) /* memory低功耗配置寄存器2。 */
#define    HI_SC_CTRL69_OFFSET                               (0x514) /* 系统错误地址访问功能寄存器。 */
#define    HI_SC_CTRL70_OFFSET                               (0x518) /* 互联优先级配置寄存器0。 */
#define    HI_SC_CTRL71_OFFSET                               (0x51C) /* 互联优先级配置寄存器1。 */
#define    HI_SC_CTRL72_OFFSET                               (0x520) /* 互联优先级配置寄存器2。 */
#define    HI_SC_CTRL81_OFFSET                               (0x544) /* appa9 gpio0中断按位使能寄存器。 */
#define    HI_SC_CTRL85_OFFSET                               (0x554) /* mdma9 gpio0中断按位使能寄存器。 */
#define    HI_SC_CTRL98_OFFSET                               (0x588) /* M3 GPIO0中断使能寄存器。 */
#define    HI_SC_CTRL103_OFFSET                              (0x59C) /* slave active屏蔽寄存器0。 */
#define    HI_SC_CTRL104_OFFSET                              (0x5A0) /* slave active屏蔽寄存器1。 */
#define    HI_SC_CTRL105_OFFSET                              (0x5A4) /* slave active屏蔽寄存器2。 */
#define    HI_SC_CTRL121_OFFSET                              (0x5E4) /* 保留寄存器。 */
#define    HI_SC_CTRL122_OFFSET                              (0x5E8) /* 保留寄存器。 */
#define    HI_SC_CTRL123_OFFSET                              (0x5EC) /* 保留寄存器。 */
#define    HI_SC_CTRL124_OFFSET                              (0x5F0) /* 保留寄存器。 */
#define    HI_SC_CTRL125_OFFSET                              (0x5F4) /* 保留寄存器。 */
#define    HI_SC_CTRL126_OFFSET                              (0x5F8) /* 保留寄存器。 */
#define    HI_SC_CTRL127_OFFSET                              (0x5FC) /* 保留寄存器。 */
#define    HI_SC_STAT0_OFFSET                                (0x600) /* 复位记录/启动/调试模式状态寄存器。 */
#define    HI_SC_STAT1_OFFSET                                (0x604) /* App/Modem A9状态寄存器。 */
#define    HI_SC_STAT2_OFFSET                                (0x608) /* HIFI/dsp0/BBE16 DSP CORE状态寄存器。 */
#define    HI_SC_STAT3_OFFSET                                (0x60C) /* hmp状态寄存器0。 */
#define    HI_SC_STAT4_OFFSET                                (0x610) /* hmp状态寄存器1。 */
#define    HI_SC_STAT5_OFFSET                                (0x614) /* 保留寄存器 */
#define    HI_SC_STAT6_OFFSET                                (0x618) /* App/Modem A9校验状态寄存器。 */
#define    HI_SC_STAT7_OFFSET                                (0x61C) /* 保留寄存器。 */
#define    HI_SC_STAT8_OFFSET                                (0x620) /* LTE BBP可维可测读计数器。 */
#define    HI_SC_STAT9_OFFSET                                (0x624) /* AXI桥锁死状态寄存器0。 */
#define    HI_SC_STAT10_OFFSET                               (0x628) /* AXI桥锁死状态寄存器1。 */
#define    HI_SC_STAT11_OFFSET                               (0x62C) /* SDCC上拉状态寄存器 */
#define    HI_SC_STAT12_OFFSET                               (0x630) /* SDCC上拉变化中断状态寄存器 */
#define    HI_SC_STAT13_OFFSET                               (0x634) /* 保留寄存器 */
#define    HI_SC_STAT14_OFFSET                               (0x638) /* usb3PHY状态寄存器1。 */
#define    HI_SC_STAT15_OFFSET                               (0x63C) /* slave active状态寄存器。 */
#define    HI_SC_STAT16_OFFSET                               (0x640) /* lte drx abs_timer状态寄存器0 */
#define    HI_SC_STAT17_OFFSET                               (0x644) /* lte drx abs_timer状态寄存器1 */
#define    HI_SC_STAT18_OFFSET                               (0x648) /* gsm deepsleep状态寄存器 */
#define    HI_SC_STAT19_OFFSET                               (0x64C) /* 保留寄存器。 */
#define    HI_SC_STAT20_OFFSET                               (0x650) /* 保留寄存器。 */
#define    HI_SC_STAT21_OFFSET                               (0x654) /* usb_pd状态寄存器。 */
#define    HI_SC_STAT22_OFFSET                               (0x658) /* AXI桥锁死状态寄存器2。 */
#define    HI_SC_STAT23_OFFSET                               (0x65C) /* func mbist fail状态寄存器。 */
#define    HI_SC_STAT24_OFFSET                               (0x660) /* func mbist done状态寄存器0。 */
#define    HI_SC_STAT25_OFFSET                               (0x664) /* func mbist done状态寄存器1。 */
#define    HI_SC_STAT26_OFFSET                               (0x668) /* master状态寄存器。 */
#define    HI_SC_STAT27_OFFSET                               (0x66C) /* slave active状态寄存器0。 */
#define    HI_SC_STAT28_OFFSET                               (0x670) /* slave active状态寄存器1。 */
#define    HI_SC_STAT29_OFFSET                               (0x674) /* slave active状态寄存器2。 */
#define    HI_SC_STAT31_OFFSET                               (0x67C) /* bbp_sys_1control状态寄存器1。 */
#define    HI_SC_STAT32_OFFSET                               (0x680) /* bbp保留状态寄存器。 */
#define    HI_SC_STAT33_OFFSET                               (0x684) /* 保留寄存器 */
#define    HI_SC_STAT34_OFFSET                               (0x688) /* 保留寄存器 */
#define    HI_SC_STAT35_OFFSET                               (0x68C) /* func mbist done状态寄存器2。 */
#define    HI_SC_STAT36_OFFSET                               (0x690) /* func mbist done状态寄存器3。 */
#define    HI_SC_STAT37_OFFSET                               (0x694) /* func mbist done状态寄存器4。 */
#define    HI_SC_STAT38_OFFSET                               (0x698) /* func mbist done状态寄存器5。 */
#define    HI_SC_STAT39_OFFSET                               (0x69C) /* func mbist done状态寄存器6。 */
#define    HI_SC_STAT40_OFFSET                               (0x6A0) /* func mbist done状态寄存器7。 */
#define    HI_SC_STAT41_OFFSET                               (0x6A4) /* master访问错误状态寄存器0。 */
#define    HI_SC_STAT42_OFFSET                               (0x6A8) /* master访问错误状态寄存器1。 */
#define    HI_SC_STAT43_OFFSET                               (0x6AC) /* ahb外设访问错误地址寄存器。 */
#define    HI_SC_STAT44_OFFSET                               (0x6B0) /* soc外设掉电域apb外设访问错误地址寄存器。 */
#define    HI_SC_STAT45_OFFSET                               (0x6B4) /* soc外设常开区apb外设访问错误地址寄存器。 */
#define    HI_SC_STAT46_OFFSET                               (0x6B8) /* glb桥m1口（dw_axi_bbphy）写访问错误地址寄存器。 */
#define    HI_SC_STAT47_OFFSET                               (0x6BC) /* glb桥m1口（dw_axi_bbphy）读访问错误地址寄存器。 */
#define    HI_SC_STAT48_OFFSET                               (0x6C0) /* glb桥m2口（dw_axi_mst）写访问错误地址寄存器。 */
#define    HI_SC_STAT49_OFFSET                               (0x6C4) /* glb桥m2口（dw_axi_mst）读访问错误地址寄存器。 */
#define    HI_SC_STAT50_OFFSET                               (0x6C8) /* glb桥m3口（app a9 m0口）写访问错误地址寄存器。 */
#define    HI_SC_STAT51_OFFSET                               (0x6CC) /* glb桥m3口（app a9 m0口）读访问错误地址寄存器。 */
#define    HI_SC_STAT52_OFFSET                               (0x6D0) /* glb桥m4口（mdm a9 m0口）写访问错误地址寄存器。 */
#define    HI_SC_STAT53_OFFSET                               (0x6D4) /* glb桥m4口（mdm a9 m0口）读访问错误地址寄存器。 */
#define    HI_SC_STAT54_OFFSET                               (0x6D8) /* glb桥m5口（reserved）写访问错误地址寄存器。 */
#define    HI_SC_STAT55_OFFSET                               (0x6DC) /* glb桥m5口（reserved）读访问错误地址寄存器。 */
#define    HI_SC_STAT56_OFFSET                               (0x6E0) /* glb桥m6口（hifi）写访问错误地址寄存器。 */
#define    HI_SC_STAT57_OFFSET                               (0x6E4) /* glb桥m6口（hifi）读访问错误地址寄存器。 */
#define    HI_SC_STAT58_OFFSET                               (0x6E8) /* cpu桥m1口（app a9 m1口）读访问错误地址寄存器。 */
#define    HI_SC_STAT59_OFFSET                               (0x6EC) /* cpu桥m2口（mdm a9 m1口）读访问错误地址寄存器。 */
#define    HI_SC_STAT60_OFFSET                               (0x6F0) /* cpu桥m1口（app a9 m1口）写访问错误地址寄存器。 */
#define    HI_SC_STAT61_OFFSET                               (0x6F4) /* cpu桥m2口（mdm a9 m1口）写访问错误地址寄存器。 */
#define    HI_SC_STAT62_OFFSET                               (0x6F8) /* bbphy桥m1口（dw_axi_glb）写访问错误地址寄存器。 */
#define    HI_SC_STAT63_OFFSET                               (0x6FC) /* bbphy桥m1口（dw_axi_glb）读访问错误地址寄存器。 */
#define    HI_SC_STAT64_OFFSET                               (0x700) /* bbphy桥m2口（dsp0）写访问错误地址寄存器。 */
#define    HI_SC_STAT65_OFFSET                               (0x704) /* bbphy桥m2口（dsp0）读访问错误地址寄存器。 */
#define    HI_SC_STAT66_OFFSET                               (0x708) /* bbphy桥m3口（bbp）写访问错误地址寄存器。 */
#define    HI_SC_STAT67_OFFSET                               (0x70C) /* bbphy桥m3口（bbp）读访问错误地址寄存器。 */
#define    HI_SC_STAT68_OFFSET                               (0x710) /* axi_amon桥m1写访问错误地址寄存器。 */
#define    HI_SC_STAT69_OFFSET                               (0x714) /* axi_amon桥m1读访问错误地址寄存器。 */
#define    HI_SC_STAT70_OFFSET                               (0x718) /* PCIE 控制器状态寄存器0 */
#define    HI_SC_STAT71_OFFSET                               (0x71C) /* PCIE 控制器状态寄存器1 */
#define    HI_SC_STAT72_OFFSET                               (0x720) /* PCIE 控制器状态寄存器2 */
#define    HI_SC_STAT73_OFFSET                               (0x724) /* PCIE 控制器状态寄存器3 */
#define    HI_SC_STAT74_OFFSET                               (0x728) /* PCIE 控制器状态寄存器4 */
#define    HI_SC_STAT75_OFFSET                               (0x72C) /* PCIE PHY状态寄存器 */
#define    HI_SC_STAT76_OFFSET                               (0x730) /* PCIE 控制器状态寄存器5 */
#define    HI_SC_STAT77_OFFSET                               (0x734) /* PCIE 控制器状态寄存器6 */
#define    HI_SC_STAT78_OFFSET                               (0x738) /* PCIE 控制器状态寄存器7 */
#define    HI_SC_STAT79_OFFSET                               (0x73C) /* PCIE 控制器状态寄存器8 */
#define    HI_SC_STAT80_OFFSET                               (0x740) /* m3状态寄存器0。 */
#define    HI_SC_STAT81_OFFSET                               (0x744) /* m3状态寄存器1。 */
#define    HI_SC_STAT82_OFFSET                               (0x748) /* m3状态寄存器2。 */
#define    HI_SC_STAT83_OFFSET                               (0x74C) /* m3状态寄存器3。 */
#define    HI_SC_STAT84_OFFSET                               (0x750) /* m3状态寄存器4。 */
#define    HI_SC_STAT85_OFFSET                               (0x754) /* m3状态寄存器5。 */
#define    HI_SC_STAT86_OFFSET                               (0x758) /* tsensor trim寄存器 */
#define    HI_SC_STAT87_OFFSET                               (0x75C) /* PCIE 控制器状态寄存器9 */
#define    HI_VERSION_ID_OFFSET                              (0xF00) /* 芯片版本寄存器。 */


#ifndef __ASSEMBLY__

/********************************************************************************/
/*    syssc_ao 寄存器定义（项目名_模块名_寄存器名_T)        */
/********************************************************************************/
typedef union
{
    struct
    {
        unsigned int    remap_clear                : 1; /* [0..0] 串行Flash片选1或ROM对应的静态Boot Memory地址重映射清除请求。0：保持重映射状态；（默认值）1：清除重映射。 */
        unsigned int    m3_remap_clear             : 1; /* [1..1] M3地址重映射清除 */
        unsigned int    reserved_1                 : 2; /* [3..2] 保留。 */
        unsigned int    remap_stat                 : 1; /* [4..4] remap状态指示。0：remap_clear有效；1：remap_clear无效（默认值）。 */
        unsigned int    cm3_remap_stat             : 1; /* [5..5] cm3_remap状态指示。0：cm3_remap_clear有效；1：cm3_remap_clear无效（默认值）。 */
        unsigned int    reserved_0                 : 26; /* [31..6] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL0_T;    /* Remap控制寄存器。 */

typedef union
{
    struct
    {
        unsigned int    pmu_rstout_n               : 1; /* [0..0] PMU复位软件控制信号。0：复位；1：不复位（默认值）。该控制位与看门狗复位信号相与，任一有效即复位PMU。 */
        unsigned int    pmu_hold                   : 1; /* [1..1] 外部PMU状态控制信号。0：PMU下电（默认值）；1：PMU开启。 */
        unsigned int    pmu_irq_mask               : 1; /* [2..2] PMU中断屏蔽0：不屏蔽；1：屏蔽。 */
        unsigned int    pmu_irq_n_pos_sel          : 1; /* [3..3] 外部PMU中断极性配置。0：极性为低（默认值）；1：极性为高。 */
        unsigned int    efuse_sys_ctrl_en          : 1; /* [4..4] efuse控制JTAG访问控制保护。0：强制允许访问JTAG；1：按照efuse烧写值确定JTAG是否允许访问。（默认值） */
        unsigned int    wdt_reset_sel              : 1; /* [5..5] watchdog复位时，复位PMU还是只复位芯片。0：复位PMU；1：只复位芯片； */
        unsigned int    reserved_1                 : 2; /* [7..6] 保留。 */
        unsigned int    usbphy_tcxo_buff_ctrl      : 6; /* [13..8] usbphy tcxo buffer控制.             [5]：选择软件/pwrctrl控制         0：usbphy tcxo buffer 使能由pwrctrl控制                        1：由usbphy_tcxo_buff_ctrl[2]控制[4]：pwrctrl控制时，选择 ch0_tcxoen/ch1_tcxoen作为en控制；                        0=：选择ch0_tcxoen                  1=：选择ch1_tcxoen[3]：reserved。[2]：软件使能usbphy tcxo buffer控制；                                0：禁止                              1：使能[1:0]：in_dr */
        unsigned int    reserved_0                 : 18; /* [31..14] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL1_T;    /* PMU控制/EFUSE JTAG位寄存器。 */

typedef union
{
    struct
    {
        unsigned int    wdt_clk_en                 : 1; /* [0..0] 看门狗时钟使能控制。0：不使能；1：使能（默认值）。注意：对该位进行写0操作时，需要先将该寄存器高16位值写为0x6920（即将wdt_en_ctrl位写为0x6920）打开保护，然后再对此比特写数值才可以有效，避免误操作。 */
        unsigned int    reserved_1                 : 2; /* [2..1] 保留（数据可写入）。 */
        unsigned int    wdt_en_ov                  : 1; /* [3..3] WDT时钟强制使能控制位。0：不使能（默认值）；1：使能。当wdt_en_ov使能时，看门狗以总线时钟（pclk）计数，否则以32K时钟计数。该位一般情况下，不需要使用，一直配置为0。 */
        unsigned int    reserved_0                 : 12; /* [15..4] 保留（数据可写入）。 */
        unsigned int    wdt_en_ctrl                : 16; /* [31..16] wdt_en写使能控制。该控制位是为了防止软件误操作看门狗而设立的保护逻辑，看门狗时钟使能开关只有在该位配置为6920后，才可更改。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL2_T;    /* 看门狗配置寄存器。 */

typedef union
{
    struct
    {
        unsigned int    lcd_rst_n                  : 1; /* [0..0] LCD复位控制位。0：复位（默认值）；1：不复位。 */
        unsigned int    emi_sel                    : 1; /* [1..1] LCD接口时序选择。0：Intel 8080（默认值）；1：Motorola。 */
        unsigned int    zsi_en                     : 1; /* [2..2]  */
        unsigned int    sio_master_mode            : 1; /* [3..3] sio 模式选择：0：slave模式（默认值）；1：master模式； */
        unsigned int    mmc0_sample_tuning_enable  : 1; /* [4..4] MMC0 接口时序优化时能。0：不使能时序优化1：使能时序优化 */
        unsigned int    mmc1_sample_tuning_enable  : 1; /* [5..5] MMC1 接口时序优化时能。0：不使能时序优化1：使能时序优化 */
        unsigned int    mmc_msc                    : 1; /* [6..6] MMC IO电压控制。0：1.8v；1：3.3v。 */
        unsigned int    ssp0_mode                  : 1; /* [7..7] ssp0模式控制。0：三线模式；1：四线模式； */
        unsigned int    ssp1_mode                  : 1; /* [8..8] ssp1模式控制。0：三线模式；1：四线模式； */
        unsigned int    amon_cpufast_monitor_start : 1; /* [9..9] 监控Cpu_fast AXI总线axi monitor启动停止信号。0：停止；1：启动。 */
        unsigned int    amon_soc_monitor_start     : 1; /* [10..10] 监控Global AXI总线axi monitor启动停止信号。0：停止；1：启动。 */
        unsigned int    pmussi0_mst_cnt            : 3; /* [13..11] pmussi0采样延时配置 */
        unsigned int    reserved_2                 : 1; /* [14..14] 保留。 */
        unsigned int    dsp0_uart_en               : 5; /* [19..15] LTEDSP UART选择[4]：uart3[3]：uart2（默认值）[2]：uart1[1]：uart0[0]：hs uart每比特值为1表示使用对应uart，0表示不使用 */
        unsigned int    hifi_uart_en               : 4; /* [23..20] VDSP UART选择[3]：uart3（默认值）[2]：uart2[1]：uart1[0]：uart0每比特值为1表示使用对应uartx，0表示不使用 */
        unsigned int    reserved_1                 : 4; /* [27..24] 保留。 */
        unsigned int    reserved_0                 : 3; /* [30..28] 保留 */
        unsigned int    test_clk_en                : 1; /* [31..31] 测试时钟使能信号，用来门控高频时钟的分频电路。0：不使能；（默认值）1：使能。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL3_T;    /* 外设配置寄存器。 */

typedef union
{
    struct
    {
        unsigned int    cicom0_sel_mod             : 1; /* [0..0] 模式选择位。0：WCDMA加解密模式（默认值）；1：GSM加解密模式。 */
        unsigned int    cicom1_sel_mod             : 1; /* [1..1] 模式选择位。0：WCDMA加解密模式（默认值）；1：GSM加解密模式。 */
        unsigned int    reserved                   : 30; /* [31..2] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL4_T;    /* CICOM加解密模式配置寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 13; /* [12..0] 保留。 */
        unsigned int    sw_appa9_boot_addr         : 19; /* [31..13] App A9启动地址。地址空间大小64/32/16KB/8KB，该地址需要64/32/16KB/8KB对齐，具体由SCTRL7控制 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL5_T;    /* App A9启动地址配置寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 13; /* [12..0] 保留。 */
        unsigned int    sw_modema9_boot_addr       : 19; /* [31..13] Modem A9启动地址。地址空间大小64/32/16KB/8KB，该地址需要64/32/16KB/8KB对齐，具体由SCTRL7控制 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL6_T;    /* Mod A9启动地址配置寄存器。 */

typedef union
{
    struct
    {
        unsigned int    appa9_remap_size           : 2; /* [1..0] appa9热启动地址控制。00：地址空间大小64KB，该地址需要64KB对齐；01：地址空间大小32KB，该地址需要32KB对齐；10：地址空间大小16KB，该地址需要16KB对齐；11：地址空间大小8KB，该地址需要8KB对齐； */
        unsigned int    mdma9_remap_size           : 2; /* [3..2] mdm9热启动地址控制。00：地址空间大小64KB，该地址需要64KB对齐；01：地址空间大小32KB，该地址需要32KB对齐；10：地址空间大小16KB，该地址需要16KB对齐；11：地址空间大小8KB，该地址需要8KB对齐； */
        unsigned int    reserved                   : 28; /* [31..4] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL7_T;    /* a9 热启动地址控制 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 16; /* [15..0] 保留。 */
        unsigned int    sw_hifi_boot_addr          : 16; /* [31..16] HIFI启动地址配置。地址空间大小64KB，该地址需要64KB对齐 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL8_T;    /* HIFI2启动地址配置寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 16; /* [15..0] 保留。 */
        unsigned int    sw_dsp0_boot_addr          : 16; /* [31..16] dsp0热启动地址配置。地址空间大小64KB，该地址需要64KB对齐 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL9_T;    /* dsp0启动地址配置寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved_3                 : 1; /* [0..0] 保留。 */
        unsigned int    appa9_cfgnmfi              : 1; /* [1..1] 快速中断不可屏蔽控制位。0：清除；1：设置（默认值）。只在CPU reset时被采样。 */
        unsigned int    appa9_cfgsdisable          : 1; /* [2..2] 处理器部分重要配置寄存器写保护。0：不使能；1：使能。该比特使能后可以防止distributor、A9处理器接口、LSPIs的部分寄存器被修改。如果需要使用此功能，ARM建议用户在boot阶段软件配置完成相关寄存器后使能该功能。理论上，系统只有在硬复位时需要关闭该保护功能。 */
        unsigned int    appa9_cp15sdisable         : 1; /* [3..3] secure register写保护。0：不使能；1：使能。reset 0， then 1 by software。保护CPU内部寄存器设置，由系统寄存器逻辑保护此值只可改变一次。 */
        unsigned int    appa9_pwrctli0             : 2; /* [5..4] CPU0状态寄存器[1:0]复位值。如无电源管理，必须接0。 */
        unsigned int    appa9_spiden               : 1; /* [6..6] secure privilege invasive Debug使能控制。0：不使能；1：使能。 */
        unsigned int    appa9_spniden              : 1; /* [7..7] secure privilege Non-invasive Debug使能控制。0：不使能；1：使能。 */
        unsigned int    appa9_niden                : 1; /* [8..8] Non-invasive Debug使能控制。0：不使能；1：使能。 */
        unsigned int    appa9_dbgen                : 1; /* [9..9] Debug使能控制。0：不使能；1：使能。 */
        unsigned int    appa9_eventi_cfg           : 2; /* [11..10] APPA9选通mdma9_evento、cm3_evento控制。[10]:选通mdma9_evento控制，高有效；[11]:选通cm3_evento控制，高有效； */
        unsigned int    reserved_2                 : 2; /* [13..12] 保留。 */
        unsigned int    appa9_l2_spniden           : 1; /* [14..14]  */
        unsigned int    reserved_1                 : 1; /* [15..15] 保留。 */
        unsigned int    appa9_l2_waysize           : 3; /* [18..16] appa9 L2 cache way size。000：reserved001：16KB010：32KBothers：reserved */
        unsigned int    reserved_0                 : 1; /* [19..19] 保留。 */
        unsigned int    appa9_l2_regfilebase       : 12; /* [31..20] appa9 L2 cache 寄存器基地址。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL10_T;    /* App A9配置寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved_3                 : 1; /* [0..0] 保留。 */
        unsigned int    moda9_cfgnmfi              : 1; /* [1..1] 快速中断不可屏蔽控制位。0：清除；1：设置（默认值）。只在CPU reset时被采样。 */
        unsigned int    moda9_cfgsdisable          : 1; /* [2..2] 处理器部分重要配置寄存器写保护。0：不使能；1：使能。该比特使能后可以防止distributor、A9处理器接口、LSPIs的部分寄存器被修改。如果需要使用此功能，ARM建议用户在boot阶段软件配置完成相关寄存器后使能该功能。理论上，系统只有在硬复位时需要关闭该保护功能。 */
        unsigned int    moda9_cp15sdisable         : 1; /* [3..3] secure register写保护。0：不使能；1：使能。reset 0， then 1 by software。保护CPU内部寄存器设置，由系统寄存器逻辑保护此值只可改变一次。 */
        unsigned int    moda9_pwrctli0             : 2; /* [5..4] CPU0状态寄存器[1:0]复位值。如无电源管理，必须接0。 */
        unsigned int    moda9_spiden               : 1; /* [6..6] secure privilege invasive Debug使能控制。0：不使能；1：使能。 */
        unsigned int    moda9_spniden              : 1; /* [7..7] secure privilege Non-invasive Debug使能控制。0：不使能；1：使能。 */
        unsigned int    moda9_niden                : 1; /* [8..8] Non-invasive Debug使能控制。0：不使能；1：使能。 */
        unsigned int    moda9_dbgen                : 1; /* [9..9] Debug使能控制。0：不使能；1：使能。 */
        unsigned int    moda9_eventi_cfg           : 2; /* [11..10] MDMA9选通APPA9_evento、cm3_evento控制。[10]:选通APPA9_evento控制，高有效；[11]:选通cm3_evento控制，高有效； */
        unsigned int    reserved_2                 : 2; /* [13..12]  */
        unsigned int    moda9_l2_spniden           : 1; /* [14..14]  */
        unsigned int    reserved_1                 : 1; /* [15..15] 保留。 */
        unsigned int    moda9_l2_waysize           : 3; /* [18..16] moda9 L2 cache way size。000：reserved001：16KB010：32KBothers：reserved */
        unsigned int    reserved_0                 : 1; /* [19..19] 保留。 */
        unsigned int    moda9_l2_regfilebase       : 12; /* [31..20] moda9 L2 cache 寄存器基地址。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL11_T;    /* Modem A9配置寄存器。 */

typedef union
{
    struct
    {
        unsigned int    hifi_ocdhaltonreset        : 1; /* [0..0] 在解复位时强行进入OCDHalt模式。0：正常模式（默认值）；1：OCDHalt模式。该bit在解复位前6个周期及后10个周期不能修改。在解复位完成后，改变该bit没有影响。 */
        unsigned int    hifi_runstall              : 1; /* [1..1] 运行/停止。0：运行；1：停止。（默认值） */
        unsigned int    hifi_statvectorsel         : 1; /* [2..2] 启动地址选择0：内部TCM启动（默认值）1：外部启动，启动地址由外部启动地址寄存器配置 */
        unsigned int    hifi_breaksync_en          : 3; /* [5..3]  */
        unsigned int    hifi_crosstrig_en          : 3; /* [8..6]  */
        unsigned int    reserved_1                 : 1; /* [9..9] 保留。 */
        unsigned int    reserved_0                 : 22; /* [31..10] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL12_T;    /* HIFI2 DSP CORE 配置寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 16; /* [15..0] 保留。 */
        unsigned int    dsp0_ocdhaltonreset        : 1; /* [16..16] dsp0在解复位时强行进入OCDHalt模式。0：正常模式（默认值）；1：OCDHalt模式。该bit在解复位前6个周期及后10个周期不能修改。在解复位完成后，改变该bit没有影响。 */
        unsigned int    dsp0_runstall              : 1; /* [17..17] dsp0运行/停止。0：运行；1：停止。（默认值） */
        unsigned int    dsp0_statvectorsel         : 1; /* [18..18] dsp0启动地址选择0：内部TCM启动（默认值）1：外部启动，启动地址由外部启动地址寄存器配置 */
        unsigned int    dsp0_breaksync_en          : 3; /* [21..19]  */
        unsigned int    dsp0_crosstrig_en          : 3; /* [24..22]  */
        unsigned int    reserved_0                 : 7; /* [31..25] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL13_T;    /* dsp0 DSP CORE 配置寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 32; /* [31..0] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL14_T;    /* 保留寄存器。 */

typedef union
{
    struct
    {
        unsigned int    cm3_fix_mst_type           : 1; /* [0..0] 0：ahb-ap hmaster值由内部MasterType寄存器决定；1：ahb-ap hmaster值固定为1 */
        unsigned int    cm3_dap_en                 : 1; /* [1..1] dap时钟使能 */
        unsigned int    cm3_ts_clk_chg             : 1; /* [2..2] timestamp时钟改变指示 */
        unsigned int    cm3_mpu_disable            : 1; /* [3..3] 1：disable mcu */
        unsigned int    cm3_dbgen                  : 1; /* [4..4] debug使能 */
        unsigned int    cm3_eventi_cfg             : 3; /* [7..5] eventi源选择。Bit[12]：选通appa9_eventoBit[11]：选通mdma9_eventoBit[10]：选通a15_evento */
        unsigned int    reserved                   : 24; /* [31..8] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL15_T;    /* M3配置寄存器0。 */

typedef union
{
    struct
    {
        unsigned int    cm3_stcalib                : 26; /* [25..0] [25]：cm3 timer 是否有外部参考时钟STCLK。0：没有外部参考时钟；1：有外部参考时钟；[24]：参考时钟频率是否10ms的倍数。0：参考时钟频率是10ms的倍数；1：参考时钟频率不是10ms的倍数；[23:0]：timer参考时钟相对于10ms(100Hz)延时的整数值； */
        unsigned int    reserved                   : 6; /* [31..26] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL16_T;    /* M3配置寄存器1。 */

typedef union
{
    struct
    {
        unsigned int    hpm0_clk_div               : 6; /* [5..0] hpm0分频系数。 */
        unsigned int    reserved_3                 : 1; /* [6..6] 保留。 */
        unsigned int    hpm0_en                    : 1; /* [7..7] hpm0使能控制。 0：不使能；1：使能。 */
        unsigned int    hpm1_clk_div               : 6; /* [13..8] hpm1分频系数。 */
        unsigned int    reserved_2                 : 1; /* [14..14] 保留。 */
        unsigned int    hpm1_en                    : 1; /* [15..15] hpm1使能控制。 0：不使能；1：使能。 */
        unsigned int    hpm2_clk_div               : 6; /* [21..16] hpm2分频系数。 */
        unsigned int    reserved_1                 : 1; /* [22..22] 保留。 */
        unsigned int    hpm2_en                    : 1; /* [23..23] hpm2使能控制。 0：不使能；1：使能。 */
        unsigned int    reserved_0                 : 8; /* [31..24] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL17_T;    /* HPM控制寄存器。 */

typedef union
{
    struct
    {
        unsigned int    cs_event_hwe               : 32; /* [31..0] 外部事件。送给coresight HWEVENT口。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL18_T;    /* coresight event寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 32; /* [31..0] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL19_T;    /* 保留寄存器。 */

typedef union
{
    struct
    {
        unsigned int    axi_mem_ret_gatedclock_en  : 1; /* [0..0] axi_mem自动门控使能。0：不使能（默认值）；1：使能。 */
        unsigned int    cicom0_auto_clk_gate_en    : 1; /* [1..1] cicom0自动门控使能。0：不使能（默认值）；1：使能。 */
        unsigned int    cicom0_soft_gate_clk_en    : 1; /* [2..2] cicom0时钟门控控制(在自动门控使能为0时有效)。0：不关断（默认值）；1：关断时钟。 */
        unsigned int    cicom1_auto_clk_gate_en    : 1; /* [3..3] cicom1自动门控使能。0：不使能（默认值）；1：使能。 */
        unsigned int    cicom1_soft_gate_clk_en    : 1; /* [4..4] cicom1时钟门控控制(在自动门控使能为0时有效)。0：不关断（默认值）；1：关断时钟。 */
        unsigned int    hs_uart_gatedclock_en      : 1; /* [5..5] hs_uart自动门控使能。0：不使能（默认值）；1：使能。 */
        unsigned int    uart_gatedclock_en         : 1; /* [6..6] 非掉电区uart0, 掉电区uart1/2/3自动门控使能1：使能0：不使能（默认值） */
        unsigned int    sdcc_hclk_wkup_en          : 1; /* [7..7] sdcc时钟唤醒使能（sdcc_hclk_soft_en配置为0时有效）。0：常关hclk时钟；1：使能时钟唤醒功能（默认值）。此功能配置在sdcc使能自动时钟门控（sdcc_gatedclock_en）配置时有效。 */
        unsigned int    sdcc_hclk_soft_en          : 1; /* [8..8] sdcc时钟门控软件配置。0：时钟唤醒，时钟开关状态由sdcc_hclk_wkup_en决定；1：常开hclk时钟。（默认值）此功能配置在sdcc使能自动时钟门控（sdcc_gatedclock_en）配置时有效。 */
        unsigned int    dw_ssi_gatedclock_en       : 1; /* [9..9] spi0、spi1自动门控使能1：使能0：不使能（默认值） */
        unsigned int    usb3_gatedclock_en         : 1; /* [10..10] USB3 controller自动门控使能。0：不使能（默认值）；1：使能。 */
        unsigned int    hdlc_soft_gate_clk_en      : 1; /* [11..11] hdlc时钟门控控制。0：自动门控（默认值）；1：关断时钟。 */
        unsigned int    reseved                    : 1; /* [12..12] 保留。 */
        unsigned int    cs_cg_en                   : 1; /* [13..13] CoreSight自动门控使能。0：不使能（默认值）；1：使能。 */
        unsigned int    reserved_1                 : 5; /* [18..14] 保留。 */
        unsigned int    hsic_gatedclock_en         : 1; /* [19..19] hsic和uicc自动门控使能。0：不使能（默认值）；1：使能。此成员同时控制hsic_gatedclock_en、uicc_gatedclock_en。 */
        unsigned int    hsic_ss_scaledown_mode     : 2; /* [21..20] hsic和uicc仿真加速控制。00：加速关闭（默认值）；01：使能除speed enumeration,HNP/SRP,Host mode suspend nad resume之外的timingvalue；10：仅使能device mode suspend and resume timing value；11：使能比特0、1 scale-down timing values。综合时必须为00。此成员同时控制hsic_ss_scaledown_mode、uicc_ss_scaledown_mode。 */
        unsigned int    upacc_auto_clk_gate_en     : 1; /* [22..22] upacc自动门控使能。0：不使能（默认值）；1：使能。 */
        unsigned int    upacc_soft_gate_clk_en     : 1; /* [23..23] upacc时钟门控控制(在自动门控使能为0时有效)。0：不关断（默认值）；1：关断时钟。 */
        unsigned int    reserved_0                 : 3; /* [26..24]  */
        unsigned int    ios_gatedclock_en          : 1; /* [27..27] ioshare自动门控使能。0：不使能（默认值）；1：使能。 */
        unsigned int    ipcm_auto_clk_gate_en      : 1; /* [28..28] ipcm自动门控使能。0：不使能（默认值）；1：使能。 */
        unsigned int    ipcm_soft_gate_clk_en      : 1; /* [29..29] ipcm时钟门控控制(在自动门控使能为0时有效)。0：不关断（默认值）；1：关断时钟。 */
        unsigned int    hdlc_auto_clk_gate_en      : 1; /* [30..30] hdlc自动门控使能。0：不使能（默认值）；1：使能。 */
        unsigned int    sysctrl_cg_en              : 1; /* [31..31] sysctrl自动门控使能。0：不使能（默认值）；1：使能。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL20_T;    /* 外设自动门控时钟控制寄存器0。 */

typedef union
{
    struct
    {
        unsigned int    reserved_4                 : 3; /* [2..0] 保留。 */
        unsigned int    sdcc_gatedclock_en         : 1; /* [3..3] sdcc自动门控使能。0：不使能（默认值）；1：使能。 */
        unsigned int    timer_gatedclock_en        : 1; /* [4..4] timer0-23自动门控使能。0：不使能（默认值）；1：使能。 */
        unsigned int    reserved_3                 : 1; /* [5..5] tsensor自动门控使能。0：不使能（默认值）；1：使能。 */
        unsigned int    reserved_2                 : 1; /* [6..6] 保留。 */
        unsigned int    mddrc_ddrphy_ac_pdd        : 1; /* [7..7] 固定为0，实际不用 */
        unsigned int    ddrc_apb_gt_en             : 1; /* [8..8] mddrc寄存器配置电路时钟控制1：打开时钟（默认值）0：关闭时钟 */
        unsigned int    dw_axi_cpu_cg_en           : 1; /* [9..9] cpu axi自动门控使能。0：不使能（默认值）；1：使能。 */
        unsigned int    dw_axi_glb_cg_en           : 1; /* [10..10] global axi自动门控使能。0：不使能（默认值）；1：使能。 */
        unsigned int    dw_axi_mst_cg_en           : 1; /* [11..11] master axi自动门控使能。0：不使能（默认值）；1：使能。 */
        unsigned int    dw_axi_amon_gatedclock_en  : 1; /* [12..12] amon自动门控使能。0：不使能（默认值）；1：使能。 */
        unsigned int    dw_axi_bbphy_cg_en         : 1; /* [13..13] bbphy axi自动门控使能。0：不使能（默认值）；1：使能。 */
        unsigned int    dw_rs_cg_en                : 1; /* [14..14] rs axi自动门控使能。0：不使能（默认值）；1：使能。Rs位于PCIE里，2个 */
        unsigned int    dw_x2x_async_cg_en         : 1; /* [15..15] async x2x自动门控使能。0：不使能（默认值）；1：使能。async x2x分布在：DSP0两个和HIFI2个 */
        unsigned int    dw_x2x_qsync_cg_en         : 1; /* [16..16] qsync x2x自动门控使能。0：不使能（默认值）；1：使能。qsync x2x 目前没有。 */
        unsigned int    dw_x2h_qsync_cg_en         : 1; /* [17..17] rs axi自动门控使能。0：不使能（默认值）；1：使能。 */
        unsigned int    dw_hmx_cg_en               : 1; /* [18..18] hmx自动门控使能。0：不使能（默认值）；1：使能。位于m3和ahb2axi_mst里各一个 */
        unsigned int    dw_x2p_cg_en               : 1; /* [19..19] x2p自动门控使能。0：不使能（默认值）；1：使能。 */
        unsigned int    dw_gs_cg_en                : 1; /* [20..20] gs自动门控使能。0：不使能（默认值）；1：使能。gs位于axi mem里 */
        unsigned int    ashb_gatedclock_en         : 1; /* [21..21] h2x自动门控使能。0：不使能（默认值）；1：使能。位于m3和ahb2axi_mst里各一个 */
        unsigned int    dw_ahb_mst_gatedclock_en   : 1; /* [22..22] mst ahb自动门控使能。0：不使能（默认值）；1：使能。 */
        unsigned int    axi_guacc_gatedclock_en    : 1; /* [23..23] guacc互联自动门控使能。0：不使能（默认值）；1：使能。 */
        unsigned int    edmac_autogated_clk_en     : 1; /* [24..24] edmac自动门控使能。0：不使能（默认值）；1：使能。 */
        unsigned int    reserved_1                 : 6; /* [30..25] 保留。 */
        unsigned int    reserved_0                 : 1; /* [31..31] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL21_T;    /* 外设自动门控时钟控制寄存器1。 */

typedef union
{
    struct
    {
        unsigned int    hifi_ema                   : 3; /* [2..0] extra margin adjustment ,[0] = LSB */
        unsigned int    hifi_emaw                  : 2; /* [4..3] extra margin adjustment write,[0] = LSB */
        unsigned int    hifi_emas                  : 1; /* [5..5] extra margin adjustment sense amplifier pulse */
        unsigned int    reserved_1                 : 10; /* [15..6] 保留。 */
        unsigned int    dsp0_ema                   : 3; /* [18..16] extra margin adjustment ,[0] = LSB */
        unsigned int    dsp0_emaw                  : 2; /* [20..19] extra margin adjustment write,[0] = LSB */
        unsigned int    dsp0_emas                  : 1; /* [21..21] extra margin adjustment sense amplifier pulse */
        unsigned int    reserved_0                 : 10; /* [31..22] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL22_T;    /* memory 时序配置寄存器0。 */

typedef union
{
    struct
    {
        unsigned int    axi_mem_ema                : 3; /* [2..0] extra margin adjustment ,[0] = LSB */
        unsigned int    axi_mem_emaw               : 2; /* [4..3] extra margin adjustment write,[0] = LSB */
        unsigned int    axi_mem_emas               : 1; /* [5..5] extra margin adjustment sense amplifier pulse */
        unsigned int    reserved_2                 : 2; /* [7..6] 保留。 */
        unsigned int    soc_mem_ema                : 3; /* [10..8] extra margin adjustment ,[0] = LSB；控制soc spram ，dpram，以及tpram的A口。对soc_mem的注解：soc spram包括：socp，coresight，hs_uart，nandc，ipf,cipher,hsic,uicc,usb3模块内的spram；soc dpram包括：mmc0，mmc1，mmc2，ssdma，sdcc模块内的dpram；soc tpram包括：socp，edmac，ssdmac，amon，gmac，ipf，cipher，mipi模块内的tpram； */
        unsigned int    soc_mem_spram_emaw         : 2; /* [12..11] extra margin adjustment write,[0] = LSB */
        unsigned int    soc_mem_spram_emas         : 1; /* [13..13] extra margin adjustment sense amplifier pulse */
        unsigned int    soc_mem_tpram_emab         : 3; /* [16..14] extra margin adjustment ,[0] = LSB；控制soc tpram的B口。 */
        unsigned int    soc_mem_tpram_emasa        : 1; /* [17..17] extra margin adjustment sense amplifier pulse */
        unsigned int    soc_mem_dpram_emaw         : 2; /* [19..18] extra margin adjustment write,[0] = LSB */
        unsigned int    soc_mem_dpram_emas         : 1; /* [20..20] extra margin adjustment sense amplifier pulse */
        unsigned int    reserved_1                 : 3; /* [23..21] 保留。 */
        unsigned int    bootrom_ema                : 3; /* [26..24] ema */
        unsigned int    bootrom_pgen               : 1; /* [27..27] pgen 高有效。power_down mode Enable */
        unsigned int    bootrom_ken                : 1; /* [28..28] ken 低有效。0：the keepers on the bitlines are enabled.this keeps the functional state of the bitlines by holding the bitline values, but slows down the access time by 1-2%.1：keepers disabled. */
        unsigned int    reserved_0                 : 3; /* [31..29] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL23_T;    /* memory 时序配置寄存器1。 */

typedef union
{
    struct
    {
        unsigned int    appa9_hs_mem_adjust        : 10; /* [9..0] AppA9 memory 时序/低功耗控制。[9:7]：保留[6]：ret1n[5]：EMAS[4:3]：EMAW[2:0]：EMA */
        unsigned int    moda9_hs_mem_adjust        : 10; /* [19..10] ModA9 memory 时序/低功耗控制。[9:7]：保留[6]：ret1n[5]：EMAS[4:3]：EMAW[2:0]：EMA */
        unsigned int    appa9_l2_ema               : 3; /* [22..20] extra margin adjustment ,[0] = LSB */
        unsigned int    appa9_l2_emaw              : 2; /* [24..23] extra margin adjustment write,[0] = LSB */
        unsigned int    appa9_l2_emas              : 1; /* [25..25] extra margin adjustment sense amplifier pulse */
        unsigned int    moda9_l2_ema               : 3; /* [28..26] extra margin adjustment ,[0] = LSB */
        unsigned int    moda9_l2_emaw              : 2; /* [30..29] extra margin adjustment write,[0] = LSB */
        unsigned int    moda9_l2_emas              : 1; /* [31..31] extra margin adjustment sense amplifier pulse */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL24_T;    /* memory 时序配置寄存器2。 */

typedef union
{
    struct
    {
        unsigned int    test_sys0_sel              : 1; /* [0..0] SOC test信号输出。0：当ioshare信号选择SOC 测试信号输出时，把各个PD域的mtcmos_ctrl输出到PAD；1：当ioshare信号选择SOC 测试信号输出时，把glb桥的dlock信号输出到PAD。 */
        unsigned int    test_sys1_sel              : 1; /* [1..1] SOC test信号输出。0：当glb桥死锁时，且test_sys_bypass信号为0时，自动把glb桥的dlock信号输出到PAD；1：当glb桥死锁时，且test_sys_bypass信号为0时，自动把各个PD域的mtcmos_ctrl输出到PAD； */
        unsigned int    reserved                   : 30; /* [31..2] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL25_T;    /* SOC 测试信号选择寄存器 */

typedef union
{
    struct
    {
        unsigned int    test_sys0_bypass           : 32; /* [31..0] SOC 测试信号bypass寄存器，当glb桥死锁时起作用。0：非bypass； 1：bypass */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL26_T;    /* SOC 测试信号bypass寄存器。 */

typedef union
{
    struct
    {
        unsigned int    cs_spiden                  : 1; /* [0..0] CoreSight secure privilege invasive Debug使能控制。0：不使能；1：使能（默认值）。 */
        unsigned int    cs_spniden                 : 1; /* [1..1] CoreSight secure privilege Non-invasive Debug使能控制。0：不使能；1：使能（默认值）。 */
        unsigned int    cs_dbgen                   : 1; /* [2..2] Debug使能控制。0：不使能；1：使能（默认值）。 */
        unsigned int    cs_niden                   : 1; /* [3..3] Non-invasive Debug使能控制。0：不使能；1：使能（默认值）。 */
        unsigned int    cs_tpctl                   : 1; /* [4..4] 指示tracectl信号是否存在。0：不使能；1：使能（默认值）。 */
        unsigned int    cs_deviceen                : 1; /* [5..5] 设备使能。0：不使能；1：使能（默认值）。 */
        unsigned int    reserved_1                 : 2; /* [7..6] 保留。 */
        unsigned int    cs_tpmaxdataasize          : 5; /* [12..8] TRACEDATA最大数据管脚配置。 */
        unsigned int    cs_nsguaren                : 1; /* [13..13] 非安全的axi访问控制：0：不使能；1：使能； */
        unsigned int    cs_tsmaxwidth              : 1; /* [14..14] 最大timestamp位宽：0：48bit1：64bit */
        unsigned int    cs_tsnatural               : 1; /* [15..15] timestamp编码方式：0：gray1：bin */
        unsigned int    cs_timestamp_en            : 1; /* [16..16] coresight timestamp 计数使能。 */
        unsigned int    reserved_0                 : 15; /* [31..17] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL27_T;    /* Coresight Debug配置寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 20; /* [19..0] 保留。 */
        unsigned int    filter_start               : 12; /* [31..20] 地址过滤首地址。即C段起始地址为0x5000_0000。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL28_T;    /* 地址过滤首地址。 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 20; /* [19..0] 保留。 */
        unsigned int    filter_end                 : 12; /* [31..20] 地址过滤尾地址。即C段结束地址为0x7FEF_FFFF。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL29_T;    /* 地址过滤尾地址。 */

typedef union
{
    struct
    {
        unsigned int    sci0_clk_od_en             : 1; /* [0..0] SCI时钟输出模式控制信号。0：CMOS输出模式（默认值）；1：Opern-drain输出模式。 */
        unsigned int    sci0_data_od_en            : 1; /* [1..1] SCI数据输出模式配置。0:CMOS输出模式（默认值）；1:Opern-drain输出模式。 */
        unsigned int    reserved_1                 : 2; /* [3..2] 保留。 */
        unsigned int    sci1_clk_od_en             : 1; /* [4..4] SCI时钟输出模式控制信号。0：CMOS输出模式（默认值）；1：Opern-drain输出模式。 */
        unsigned int    sci1_data_od_en            : 1; /* [5..5] SCI数据输出模式配置。0：CMOS输出模式（默认值）；1：Opern-drain输出模式。 */
        unsigned int    reserved_0                 : 26; /* [31..6] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL30_T;    /* SCI配置寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 32; /* [31..0] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL31_T;    /* 保留寄存器。 */

typedef union
{
    struct
    {
        unsigned int    ebi_normal_mode            : 1; /* [0..0] 停靠模式选择。0：停靠模式；1：非停靠模式。停靠模式下，如果没有控制器申请EBI，则会仲裁给通道1（nandc）。非停靠模式下，如果没有申请，则不会仲裁给任一个通道。 */
        unsigned int    reserved_2                 : 3; /* [3..1] 保留。 */
        unsigned int    ebi_timeoutvalue1          : 10; /* [13..4] nandc通道timeout值。 */
        unsigned int    reserved_1                 : 2; /* [15..14] 保留。 */
        unsigned int    ebi_timeoutvalue2          : 10; /* [25..16] emi通道timeout值。 */
        unsigned int    reserved_0                 : 6; /* [31..26] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL32_T;    /* EBI控制寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved_2                 : 1; /* [0..0] 保留。 */
        unsigned int    func_ddr_testmode          : 1; /* [1..1] DramTest测试通路控制。0：不打开测试通路（默认）；1：打开测试通路。 */
        unsigned int    reserved_1                 : 6; /* [7..2] 保留。 */
        unsigned int    reserved_0                 : 24; /* [31..8] tsensor模拟IP测试 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL33_T;    /* dram_test测试模式寄存器。 */

typedef union
{
    struct
    {
        unsigned int    refclk_div                 : 7; /* [6..0] 参考时钟频率选择。0010100：20MHz；0010101：19.2MHz；0011010：16MHz；0011100：15MHz；0011100：13MHz；0100100：12MHz；其他值：保留。必须在power-on reset前配置完成，并且在normal下不可配。 */
        unsigned int    txrputune                  : 2; /* [8..7] 输出上拉阻抗调节，调节DATA和STROBE单端源阻抗。00：+11%；01：+5%；10：默认值；11：-5%。 */
        unsigned int    txrpdtune                  : 2; /* [10..9] 输出下拉阻抗调节，调节DATA和STROBE单端源阻抗。00：-11%；01：-5%；10：默认值；11：+5%。 */
        unsigned int    txsrtune                   : 4; /* [14..11] 输出slew rate调节，调节DATA和STROBE输出斜率。0000：-20%；0001：-10%；0011：默认值；0111：+10%；1111：+20%。 */
        unsigned int    loopbackenb                : 1; /* [15..15] Loopback Test Enable。0：数据传输期间，接收逻辑不使能；1：数据传输期间，接收逻辑使能。在run BIST期间无需设置；正常模式下也不使用。 */
        unsigned int    siddq                      : 1; /* [16..16] IDDQ Test Enable，可使用户在掉电所有模拟block时进行IDDQ测试。0：模拟block不掉电；1：模拟block掉电。 */
        unsigned int    txbitstuffen               : 1; /* [17..17] Low-byte transmit Bit-Stuffing Enable. 可在OPMODE为2'b11时控制DATAIN[7:0]位填充。0：不使能；1：使能。 */
        unsigned int    txbitstuffenh              : 1; /* [18..18] High-byte transmit Bit-Stuffing Enable. 可在OPMODE为2'b11时控制DATAINH[7:0]位填充。0：不使能；1：使能。 */
        unsigned int    hsic_portreset             : 1; /* [19..19] Per-Port Reset。可在没有关闭HSIC Transceiver PHY的时钟下复位发送接收逻辑。0：发送接收状态机正常工作，and the line_state logic becomes sequential after 11 PHYCLK cycles；1：发送接收状态机复位。 */
        unsigned int    reserved_1                 : 1; /* [20..20] 保留寄存器。 */
        unsigned int    dmpulldown                 : 1; /* [21..21] HSIC PHY输入信号dmpulldown配置。0：device模式；1：host模式。 */
        unsigned int    dppulldown                 : 1; /* [22..22] HSIC PHY输入信号dppulldown配置。0：device模式；1：host模式。 */
        unsigned int    reserved_0                 : 7; /* [29..23] 保留。 */
        unsigned int    utmi_hostdisconnect        : 1; /* [30..30]  */
        unsigned int    utmiotg_iddig              : 1; /* [31..31] mini A/B plgy indicator。0：Mini-A connected，host模式；1：Mini-B connected，device模式。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL34_T;    /* HSICPHY/HSIC控制器配置寄存器。 */

typedef union
{
    struct
    {
        unsigned int    test_addr                  : 4; /* [3..0] HSIC PHY测试接口寄存器地址，如果不用需要接为0。 */
        unsigned int    test_datain                : 8; /* [11..4] HSIC PHY测试数据总线。低4bit代表测试数据，高4bit为相应的写使能。如果不用接为0。 */
        unsigned int    test_clk                   : 1; /* [12..12] HSIC PHY测试时钟，不用需要接为0。 */
        unsigned int    test_dataoutsel            : 1; /* [13..13] HSIC PHY测试数据输出选择。0：内部输出；1：测试输出。如果不用接为0。 */
        unsigned int    reserved                   : 14; /* [27..14] 保留。 */
        unsigned int    test_dataout               : 4; /* [31..28] HSIC PHY测试数据输出总线，数据由test_dataoutsel选择。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL35_T;    /* HSICPHY测试控制寄存器。 */

typedef union
{
    struct
    {
        unsigned int    fsel                       : 3; /* [2..0] 参考时钟选择：000：9.6MHz；001：10MHz；010：12MHz；011：19.2MHz（默认值）；100：20MHz；101：24MHz；110：reserved；111：50MHz。 */
        unsigned int    refclk_sel                 : 2; /* [4..3] PLL模块参考时钟源选择。此成员必须在上电复位前配置并在正常工作时不更改。00：晶体；01：XO管脚上的外部2.5V时钟；10：CLKCORE管脚输入时钟（可从core出）；11：保留。 */
        unsigned int    common_on_n                : 1; /* [5..5] Common Block（XO,Bias,PLL掉电控制信号。该信号必须上电复位前配好。0：suspend模式下，XO,Bias,PLL模块掉电；sleep模式下，如果crystal作为参考时钟，XO模块一直不掉电；1：suspend模式下，XO,Bias,PLL模块掉电；sleep模式下，Bias,PLL模块掉电。 */
        unsigned int    port_reset                 : 1; /* [6..6] 端口复位控制。可在没有关闭HSIC PHY的时钟下复位发送接收逻辑。0：不复位；1：复位。 */
        unsigned int    vatestenb                  : 2; /* [8..7] 模拟测试管脚选择，使能模拟测试电压传到ANALOGTEST或ID0管脚00：不供给；01：供给ID0；10：供给ANALOGTEST；11：保留。 */
        unsigned int    txvreftune                 : 4; /* [12..9] 高速DC电压调节。0000：-10%；0001：-8.75%；0010：-7.5%；0011：-6.25%；0100：-5%；0101：-3.75%；0110：-2.5%；0111：-1.25%；1000：Design default%；1001：+1.25%；1010：+2.5%；1011：+3.75%；1100：+5%；1101：+6.25%；1110：+7.5%；1111：+8.75%。 */
        unsigned int    txfslstune                 : 4; /* [16..13] 全速/低速源端阻抗调整。0000：+5%；0001：+2.5%；0011：默认值；0111：-2.5%；1111：-5%；others：reserved。 */
        unsigned int    txhsxvtune                 : 2; /* [18..17] 高速发送信号电压交叉点调整。00：reserved；01：-15mV；10：+15mV；11：默认值。 */
        unsigned int    otgtune                    : 3; /* [21..19] VBUS有效阈值电压调整。000：-12%；001：-9%；010：-6%；011：-3%；100：默认值；101：+3%；110：+6%；111：+9%。 */
        unsigned int    compdistune                : 3; /* [24..22] 断开连接阈值电压调整。000：-6%；001：-4.5%；010：-3%；011：-1.5%100：默认值；101：+1.5%；110：+3%；111：+4.5%。 */
        unsigned int    sqrxtune                   : 3; /* [27..25] Squelch阈值电压调整。000：+15%；001：+10%；010：+5%；011：默认值；100：-5%；101：-10%；110：-15%；111：-20%。 */
        unsigned int    txrisetune                 : 2; /* [29..28] 高速发送上升/下降沿时间调整。00：+20%；01：+15%；10：Design default；11：-10%。 */
        unsigned int    txpreempamptune            : 2; /* [31..30] 高速发送预加重电流控制。00：不使能；01：1倍预加重电流；10：2倍预加重电流；11：3倍预加重电流。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL36_T;    /* USBPHY控制寄存器0。 */

typedef union
{
    struct
    {
        unsigned int    test_addr                  : 4; /* [3..0] 测试接口寄存器地址，如果不用需要接为0 */
        unsigned int    test_clk                   : 1; /* [4..4] 测试时钟，不用需要接为0 */
        unsigned int    test_dataout_sel           : 1; /* [5..5] 测试数据输出选择0：内部输出；1：测试输出。如果不用接为0 */
        unsigned int    phy_clk_sel                : 1; /* [6..6] 输出utmi_clk时钟选择。0：输出FREECLK；1：输出PHYCLOCK0。 */
        unsigned int    phy_id_sel                 : 1; /* [7..7] mul_id_bc_val输出选择。0：输出iddig状态；1：输出{rid_float,rid_gnd,rid_a,rid_b,rid_c}。 */
        unsigned int    test_datain                : 8; /* [15..8] 测试数据总线，低4bit代表测试数据，高4bit为相应的写使能。如果不用接为0 */
        unsigned int    test_dataout               : 4; /* [19..16] 测试数据输出总线，数据由test_dataoutsel选择 */
        unsigned int    acaenb                     : 1; /* [20..20] ACA使能。0：不使能；1：使能。 */
        unsigned int    retenablen                 : 1; /* [21..21] retention模式使能，低有效。正常工作模式下，该bit必须配置为1。进入retention模式支持PHY的0.9v和3.3v电源单独掉电。0：使能；1：不使能。 */
        unsigned int    reserved                   : 2; /* [23..22] 保留 */
        unsigned int    txpreemppulsetune          : 1; /* [24..24] 高速发送预加重时间控制。0：2倍预加重时间，默认值；1: 1倍预加重时间。 */
        unsigned int    otgdisable                 : 1; /* [25..25] OTG功能使能。 0：使能；1：不使能。 */
        unsigned int    loopbackenb                : 1; /* [26..26] 环回测试使能。0：不使能；1：使能。 */
        unsigned int    vbusvldextsel              : 1; /* [27..27] 外部Vbus有效选择信号。0：使用内部session valid comparator(默认配置)；1：使用vbusvldext信号。 */
        unsigned int    vbusvldext                 : 1; /* [28..28] Vbus有效指示信号。该信号在vbusvldextsel信号配置为1并且device模式时有效。0：vbus无效，不使能D+上的上拉电阻；1：vbus信号有效，使能D+上的上拉电阻。 */
        unsigned int    txrestune                  : 2; /* [30..29] 源端阻抗调整。00: 增大1.5Ω；01: 默认值；10: 减小2 Ω；11: 减小4 Ω。 */
        unsigned int    siddq                      : 1; /* [31..31] IDDQ测试模式使能。进入SIDDQ模式，所有模拟模块掉电。0：不使能；1：使能。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL37_T;    /* USBPHY控制寄存器1。 */

typedef union
{
    struct
    {
        unsigned int    compdistune                : 3; /* [2..0]  */
        unsigned int    otgtune                    : 3; /* [5..3]  */
        unsigned int    sqrxtune                   : 3; /* [8..6]  */
        unsigned int    txfslstune                 : 4; /* [12..9]  */
        unsigned int    txhsxvtune                 : 2; /* [14..13]  */
        unsigned int    txpreempamptune            : 2; /* [16..15]  */
        unsigned int    txpreemppulsetune          : 1; /* [17..17]  */
        unsigned int    txrestune                  : 2; /* [19..18]  */
        unsigned int    txrisetune                 : 2; /* [21..20]  */
        unsigned int    txvreftune                 : 4; /* [25..22]  */
        unsigned int    los_bias                   : 3; /* [28..26]  */
        unsigned int    los_level2_0               : 3; /* [31..29] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL38_T;    /* USBPHY控制寄存器2。 */

typedef union
{
    struct
    {
        unsigned int    los_level4_3               : 2; /* [1..0]  */
        unsigned int    pcs_tx_deemph_3p5db        : 6; /* [7..2]  */
        unsigned int    pcs_tx_deemph_6db          : 6; /* [13..8]  */
        unsigned int    pcs_tx_swing_full          : 7; /* [20..14]  */
        unsigned int    lane0_tx_term_offset       : 5; /* [25..21]  */
        unsigned int    tx_vboost_lvl              : 3; /* [28..26]  */
        unsigned int    reserved                   : 3; /* [31..29] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL39_T;    /* USBPHY控制寄存器3。 */

typedef union
{
    struct
    {
        unsigned int    cr_data_in                 : 16; /* [15..0]  */
        unsigned int    cr_cap_addr                : 1; /* [16..16]  */
        unsigned int    cr_cap_data                : 1; /* [17..17]  */
        unsigned int    cr_write                   : 1; /* [18..18]  */
        unsigned int    cr_read                    : 1; /* [19..19]  */
        unsigned int    rtune_req                  : 1; /* [20..20]  */
        unsigned int    reserved                   : 11; /* [31..21] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL40_T;    /* USBPHY控制寄存器4。 */

typedef union
{
    struct
    {
        unsigned int    usb3_bus_filter_bypass     : 4; /* [3..0]  */
        unsigned int    usb3_fladj_30mhz_reg       : 6; /* [9..4]  */
        unsigned int    usb3_host_u2_port_disable  : 1; /* [10..10]  */
        unsigned int    usb3_host_u3_port_disable  : 1; /* [11..11]  */
        unsigned int    usb3_host_port_pwrctl      : 1; /* [12..12]  */
        unsigned int    usb3_host_msi_enable       : 1; /* [13..13]  */
        unsigned int    usb3_pm_power_state_request : 2; /* [15..14]  */
        unsigned int    usb3_xhci_revision         : 1; /* [16..16]  */
        unsigned int    usb3_xhc_bme               : 1; /* [17..17]  */
        unsigned int    reserved_1                 : 1; /* [18..18]  */
        unsigned int    reserved_0                 : 1; /* [19..19]  */
        unsigned int    usb3_pmu_iddig_override    : 1; /* [20..20]  */
        unsigned int    usb3_pmu_avalid_override   : 1; /* [21..21]  */
        unsigned int    usb3_pmu_bvalid_override   : 1; /* [22..22]  */
        unsigned int    usb3_pmu_vbusvalid_override : 1; /* [23..23]  */
        unsigned int    usb3_pmu_sessend_override  : 1; /* [24..24]  */
        unsigned int    usb3_pmu_powerpresent_override : 1; /* [25..25]  */
        unsigned int    usb3_pmu_iddig_override_en : 1; /* [26..26]  */
        unsigned int    usb3_pmu_avalid_override_en : 1; /* [27..27]  */
        unsigned int    usb3_pmu_bvalid_override_en : 1; /* [28..28]  */
        unsigned int    usb3_pmu_vbusvalid_override_en : 1; /* [29..29]  */
        unsigned int    usb3_pmu_sessend_override_en : 1; /* [30..30]  */
        unsigned int    usb3_pmu_powerpresent_override_en : 1; /* [31..31]  */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL41_T;    /* USB Controller 控制寄存器0 */

typedef union
{
    struct
    {
        unsigned int    usb3_pmu_pm_pmu_config_strap : 30; /* [29..0]  */
        unsigned int    usb3_pmu_port_overcurrent  : 2; /* [31..30]  */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL42_T;    /* USB Controller 控制寄存器1 */

typedef union
{
    struct
    {
        unsigned int    usb3_hub_port_perm_attach  : 2; /* [1..0]  */
        unsigned int    usb3_pme_en                : 1; /* [2..2]  */
        unsigned int    reserved                   : 29; /* [31..3] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL43_T;    /* USB Controller 控制寄存器2 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 32; /* [31..0] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL44_T;    /* SSIC PHY控制寄存器。 */

typedef union
{
    struct
    {
        unsigned int    uicc_ic_usb_vbusvalid      : 1; /* [0..0] VBUS有效控制。0：无效；1：有效。 */
        unsigned int    reserved                   : 7; /* [7..1] 保留。 */
        unsigned int    intr_merge0_en             : 1; /* [8..8] 中断合并控制器0使能控制。1：使能； 0：禁止。 */
        unsigned int    intr_merge0_mode           : 1; /* [9..9] 中断合并控制器0模式选择。0：模式0；1：模式1。 */
        unsigned int    intr_merge1_en             : 1; /* [10..10] 中断合并控制器1使能控制。1：使能； 0：禁止。 */
        unsigned int    intr_merge1_mode           : 1; /* [11..11] 中断合并控制器1模式选择。0：模式0；1：模式1。 */
        unsigned int    intr_merge2_en             : 1; /* [12..12] 中断合并控制器2使能控制。1：使能； 0：禁止。 */
        unsigned int    intr_merge2_mode           : 1; /* [13..13] 中断合并控制器2模式选择。0：模式0；1：模式1。 */
        unsigned int    intr_merge3_en             : 1; /* [14..14] 中断合并控制器3使能控制。1：使能； 0：禁止。 */
        unsigned int    intr_merge3_mode           : 1; /* [15..15] 中断合并控制器3模式选择。0：模式0；1：模式1。 */
        unsigned int    intr_merge0_sel            : 4; /* [19..16] 选择哪个中断进行merge             12：uicc_intr                         11~10：ipfilter[1:0]                     9：edmac_intr                        8：hsic_intr                         7：cipher_intr                       6：mmc1_intr                         5：mmc0_intr                             4：sdcc_intr                         3：pcie_edma_int                     2：pcie_pm_int                       1：usb3_otg_intr                     0：usb3_intr */
        unsigned int    intr_merge1_sel            : 4; /* [23..20] 选择哪个中断进行merge             12：uicc_intr                         11~10：ipfilter[1:0]                     9：edmac_intr                        8：hsic_intr                         7：cipher_intr                       6：mmc1_intr                         5：mmc0_intr                             4：sdcc_intr                         3：pcie_edma_int                     2：pcie_pm_int                       1：usb3_otg_intr                     0：usb3_intr */
        unsigned int    intr_merge2_sel            : 4; /* [27..24] 选择哪个中断进行merge             12：uicc_intr                         11~10：ipfilter[1:0]                     9：edmac_intr                        8：hsic_intr                         7：cipher_intr                       6：mmc1_intr                         5：mmc0_intr                             4：sdcc_intr                         3：pcie_edma_int                     2：pcie_pm_int                       1：usb3_otg_intr                     0：usb3_intr */
        unsigned int    intr_merge3_sel            : 4; /* [31..28] 选择哪个中断进行merge             12：uicc_intr                         11~10：ipfilter[1:0]                     9：edmac_intr                        8：hsic_intr                         7：cipher_intr                       6：mmc1_intr                         5：mmc0_intr                             4：sdcc_intr                         3：pcie_edma_int                     2：pcie_pm_int                       1：usb3_otg_intr                     0：usb3_intr */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL45_T;    /* uicc、中断合并控制寄存器。 */

typedef union
{
    struct
    {
        unsigned int    intr_merge0_timer0         : 16; /* [15..0] 中断合并控制器0超时设置，单位是48MHz时钟的一个周期。 */
        unsigned int    intr_merge0_timer1         : 16; /* [31..16] 中断合并控制器1超时设置，单位是48MHz时钟的一个周期。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL46_T;    /* 中断合并超时寄存器0。 */

typedef union
{
    struct
    {
        unsigned int    int2_merge0_timer2         : 16; /* [15..0] 中断合并控制器2超时设置，单位是48MHz时钟的一个周期。 */
        unsigned int    intr_merge0_timer3         : 16; /* [31..16] 中断合并控制器3超时设置，单位是48MHz时钟的一个周期。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL47_T;    /* 中断合并超时寄存器1。 */

typedef union
{
    struct
    {
        unsigned int    sysctrl_intr_appa9         : 2; /* [1..0] 系统控制器ARM软件中断。[1]：AppA9 101号中断  1：使能  0：不使能[0]：AppA9 100号中断  1：使能  0：不使能 */
        unsigned int    sysctrl_intr_mdma9         : 2; /* [3..2] 系统控制器ARM软件中断。[1]：MDMA9 101号中断  1：使能  0：不使能[0]：MDMA9 100号中断  1：使能  0：不使能 */
        unsigned int    sysctrl_intr_cm3           : 2; /* [5..4] 系统控制器ARM软件中断。[1]：CM3 74号中断  1：使能  0：不使能[0]：CM3 75号中断  1：使能  0：不使能 */
        unsigned int    reserved                   : 26; /* [31..6] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL48_T;    /* CPU软中断置位寄存器。 */

typedef union
{
    struct
    {
        unsigned int    sysctrl_intr_appa9         : 2; /* [1..0] 系统控制器ARM软件中断。[1]：AppA9 101号中断  1：清除  0：不清除[0]：AppA9 100号中断  1：清除  0：不清除 */
        unsigned int    sysctrl_intr_mdma9         : 2; /* [3..2] 系统控制器ARM软件中断。[1]：MDMA9 101号中断  1：清除  0：不清除[0]：MDMA9 100号中断  1：清除  0：不清除 */
        unsigned int    sysctrl_intr_cm3           : 2; /* [5..4] 系统控制器ARM软件中断。[1]：CM3 75号中断  1：清除  0：不清除[0]：Cm3 740号中断  1：清除  0：不清除 */
        unsigned int    reserved                   : 26; /* [31..6] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL49_T;    /* CPU软中断清零寄存器。 */

typedef union
{
    struct
    {
        unsigned int    sysctrl_intr_appa9         : 2; /* [1..0] 系统控制器ARM软件中断。[1]：AppA9 101号中断  1：使能  0：不使能[0]：AppA9 100号中断  1：使能  0：不使能 */
        unsigned int    sysctrl_intr_mdma9         : 2; /* [3..2] 系统控制器ARM软件中断。[1]：MDMA9 101号中断  1：使能  0：不使能[0]：MDMA9 100号中断  1：使能  0：不使能 */
        unsigned int    sysctrl_intr_cm3           : 2; /* [5..4] 系统控制器ARM软件中断。[1]：CM3 75号中断  1：使能  0：不使能[0]：CM3 74号中断  1：使能  0：不使能 */
        unsigned int    reserved                   : 26; /* [31..6] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL50_T;    /* CPU软中断状态寄存器。 */

typedef union
{
    struct
    {
        unsigned int    cm3_nmi                    : 16; /* [15..0] cm3不可屏蔽中断，任一bit为高即有效。 */
        unsigned int    reserved                   : 16; /* [31..16] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL51_T;    /* M3不可屏蔽中断寄存器。 */

typedef union
{
    struct
    {
        unsigned int    dsp0_nmi                   : 16; /* [15..0] dsp0不可屏蔽中断，任一bit为高即有效。 */
        unsigned int    reserved                   : 16; /* [31..16] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL52_T;    /* dsp0不可屏蔽中断寄存器。 */

typedef union
{
    struct
    {
        unsigned int    hifi_nmi                   : 16; /* [15..0] HIFI2不可屏蔽中断，任一bit为高即有效。 */
        unsigned int    reserved                   : 16; /* [31..16] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL53_T;    /* HIFI DSP不可屏蔽中断寄存器。 */

typedef union
{
    struct
    {
        unsigned int    dw_ssi0_dma_sel            : 1; /* [0..0] DW_SSI0 DMA选择。0：选择EDAM；1：选择EDMA_CH4。 */
        unsigned int    dw_ssi1_dma_sel            : 1; /* [1..1] DW_SSI1 DMA选择。0：选择EDAM；1：选择EDMA_CH4。 */
        unsigned int    sio_dma_sel                : 1; /* [2..2] SIO DMA选择。0：选择EDAM；1：选择EDMA_CH4。 */
        unsigned int    hs_uart_dma_sel            : 1; /* [3..3] HS_UART DMA选择。0：选择EDAM；1：选择EDMA_CH4。 */
        unsigned int    uart0_dma_sel              : 1; /* [4..4] UART0 DMA选择。0：选择EDAM；1：选择EDMA_CH4。 */
        unsigned int    uart1_dma_sel              : 1; /* [5..5] UART1 DMA选择。0：选择EDAM；1：选择EDMA_CH4。 */
        unsigned int    uart2_dma_sel              : 1; /* [6..6] UART2 DMA选择。0：选择EDAM；1：选择EDMA_CH4。 */
        unsigned int    sci0_dma_sel               : 1; /* [7..7] SCI0 DMA选择。0：选择EDAM；1：选择EDMA_CH4。 */
        unsigned int    sci1_dma_sel               : 1; /* [8..8] SCI1 DMA选择。0：选择EDAM；1：选择EDMA_CH4。 */
        unsigned int    bbp_dbg_dma_sel            : 1; /* [9..9] bbp dbg DMA选择。0：选择EDAM；1：选择EDMA_CH4。 */
        unsigned int    bbp_grif_dma_sel           : 1; /* [10..10] bbp grif DMA选择。0：选择EDAM；1：选择EDMA_CH4。 */
        unsigned int    amon_soc_dma_sel           : 1; /* [11..11] axi monitor soc DMA选择。0：选择EDAM；1：选择EDMA_CH4。 */
        unsigned int    amon_fast_dma_sel          : 1; /* [12..12] axi monitor fast DMA选择。0：选择EDAM；1：选择EDMA_CH4。 */
        unsigned int    i2c0_dma_sel               : 1; /* [13..13] I2C0 DMA选择。0：选择EDAM；1：选择EDMA_CH4。 */
        unsigned int    i2c1_dma_sel               : 1; /* [14..14] I2C1 DMA选择。0：选择EDAM；1：选择EDMA_CH4。 */
        unsigned int    reserved                   : 17; /* [31..15] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL54_T;    /* DMA选择控制寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 1; /* [0..0] 保留。 */
        unsigned int    cs_ret1n                   : 1; /* [1..1] memory retention mode1控制。0：进入retention；1：不进入retention。 */
        unsigned int    gmac_ret1n                 : 1; /* [2..2] memory retention mode1控制。0：进入retention；1：不进入retention。 */
        unsigned int    usb3_ret1n                 : 1; /* [3..3] memory retention mode1控制。0：进入retention；1：不进入retention。 */
        unsigned int    reserved_0                 : 1; /* [4..4] 保留。 */
        unsigned int    uicc_ret1n                 : 1; /* [5..5] memory retention mode1控制。0：进入retention；1：不进入retention。 */
        unsigned int    nandc_ret1n                : 1; /* [6..6] memory retention mode1控制。0：进入retention；1：不进入retention。 */
        unsigned int    sdcc_ret1n                 : 1; /* [7..7] memory retention mode1控制。0：进入retention；1：不进入retention。 */
        unsigned int    hsic_ret1n                 : 1; /* [8..8] memory retention mode1控制。0：进入retention；1：不进入retention。 */
        unsigned int    amon_soc_ret1n             : 1; /* [9..9] memory retention mode1控制。0：进入retention；1：不进入retention。 */
        unsigned int    amon_cpufast_ret1n         : 1; /* [10..10] memory retention mode1控制。0：进入retention；1：不进入retention。 */
        unsigned int    pcie_colldisn              : 1; /* [11..11] memory读写冲突电路disable控制。0：disable 读写冲突电路；1：enable 读写冲突电路。 */
        unsigned int    mmc_colldisn               : 1; /* [12..12] memory读写冲突电路disable控制。0：disable 读写冲突电路；1：enable 读写冲突电路。 */
        unsigned int    edmac_colldisn             : 1; /* [13..13] memory读写冲突电路disable控制。0：disable 读写冲突电路；1：enable 读写冲突电路。 */
        unsigned int    ssdma_colldisn             : 1; /* [14..14] memory读写冲突电路disable控制。0：disable 读写冲突电路；1：enable 读写冲突电路。 */
        unsigned int    sdcc_colldisn              : 1; /* [15..15] memory读写冲突电路disable控制。0：disable 读写冲突电路；1：enable 读写冲突电路。 */
        unsigned int    socp_colldisn              : 1; /* [16..16] memory读写冲突电路disable控制。0：disable 读写冲突电路；1：enable 读写冲突电路。 */
        unsigned int    amon_colldisn              : 1; /* [17..17] memory读写冲突电路disable控制。0：disable 读写冲突电路；1：enable 读写冲突电路。 */
        unsigned int    gmac_colldisn              : 1; /* [18..18] memory读写冲突电路disable控制。0：disable 读写冲突电路；1：enable 读写冲突电路。 */
        unsigned int    ipf_ret1n                  : 1; /* [19..19] memory retention mode1控制。0：进入retention；1：不进入retention。 */
        unsigned int    ipf_colldisn               : 1; /* [20..20] memory读写冲突电路disable控制。0：disable 读写冲突电路；1：enable 读写冲突电路。 */
        unsigned int    cipher_ret1n               : 1; /* [21..21] memory retention mode1控制。0：进入retention；1：不进入retention。 */
        unsigned int    cipher_colldisn            : 1; /* [22..22] memory读写冲突电路disable控制。0：disable 读写冲突电路；1：enable 读写冲突电路。 */
        unsigned int    socp_ret1n                 : 1; /* [23..23] memory retention mode1控制。0：进入retention；1：不进入retention。 */
        unsigned int    mipi_ret1n                 : 1; /* [24..24] memory retention mode1控制。0：进入retention；1：不进入retention。 */
        unsigned int    hs_uart_ret1n              : 1; /* [25..25] memory retention mode1控制。0：进入retention；1：不进入retention。 */
        unsigned int    mmc0_ret1n                 : 1; /* [26..26] memory retention mode1控制。0：进入retention；1：不进入retention。 */
        unsigned int    mmc1_ret1n                 : 1; /* [27..27] memory retention mode1控制。0：进入retention；1：不进入retention。 */
        unsigned int    pcie_ret1n                 : 1; /* [28..28] memory retention mode1控制。0：进入retention；1：不进入retention。 */
        unsigned int    edmac_ret1n                : 1; /* [29..29] memory retention mode1控制。0：进入retention；1：不进入retention。 */
        unsigned int    ssdma_ret1n                : 1; /* [30..30] memory retention mode1控制。0：进入retention；1：不进入retention。 */
        unsigned int    mipi_colldisn              : 1; /* [31..31] memory读写冲突电路disable控制。0：disable 读写冲突电路；1：enable 读写冲突电路。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL55_T;    /* memory低功耗配置寄存器0。 */

typedef union
{
    struct
    {
        unsigned int    prior_level                : 2; /* [1..0] 总线优先级设置。00：优先级dma > axi;01：优先级axi > dma. */
        unsigned int    gatedclock_en              : 1; /* [2..2] lbus总线自动时钟门控使能。1：使能；0：禁止。 */
        unsigned int    dw_axi_rs_gatedclock_en    : 1; /* [3..3] axi rs自动门控使能。1：使能；0：禁止。 */
        unsigned int    dw_axi_gs_gatedclock_en    : 1; /* [4..4] axi gs自动门控使能。1：使能；0：禁止。 */
        unsigned int    overf_prot                 : 2; /* [6..5] 总线防挂死保护选项。00：关闭防挂死保护；01：使能防挂死保护，超时时间：512 cycles；10：使能防挂死保护，超时时间：1024 cycles；11：使能防挂死保护，超时时间：2048 cycles */
        unsigned int    reserved                   : 9; /* [15..7] 保留。 */
        unsigned int    atpram_ctrl                : 16; /* [31..16] memory低功耗控制。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL56_T;    /* BBP LBUS控制寄存器。 */

typedef union
{
    struct
    {
        unsigned int    soc2lte_tbd                : 16; /* [15..0]  */
        unsigned int    reserved                   : 16; /* [31..16] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL57_T;    /* BBP保留控制寄存器。 */

typedef union
{
    struct
    {
        unsigned int    moda9_l2_ret1n             : 1; /* [0..0] memory retention mode1控制。0：进入retention；1：不进入retention。 */
        unsigned int    moda9_l2_ret2n             : 1; /* [1..1] memory retention mode2控制。0：进入retention；1：不进入retention。 */
        unsigned int    moda9_l2_pgen              : 1; /* [2..2] memory power down控制。0：不进入power down；1：进入power down。 */
        unsigned int    reserved_5                 : 1; /* [3..3] 保留。 */
        unsigned int    appa9_l2_ret1n             : 1; /* [4..4] memory retention mode1控制。0：进入retention；1：不进入retention。 */
        unsigned int    appa9_l2_ret2n             : 1; /* [5..5] memory retention mode2控制。0：进入retention；1：不进入retention。 */
        unsigned int    appa9_l2_pgen              : 1; /* [6..6] memory power down控制。0：不进入power down；1：进入power down。 */
        unsigned int    reserved_4                 : 1; /* [7..7] 保留。 */
        unsigned int    dsp0_ret1n                 : 1; /* [8..8] memory retention mode1控制。0：进入retention；1：不进入retention。 */
        unsigned int    dsp0_ret2n                 : 1; /* [9..9] memory retention mode2控制。V7R2中memory不支持此bit控制0：进入retention；1：不进入retention。 */
        unsigned int    dsp0_pgen                  : 1; /* [10..10] memory power down控制。V7R2中memory不支持此bit控制0：不进入power down；1：进入power down。 */
        unsigned int    reserved_3                 : 1; /* [11..11] 保留。 */
        unsigned int    hifi_ret1n                 : 1; /* [12..12] memory retention mode1控制。0：进入retention；1：不进入retention。 */
        unsigned int    hifi_ret2n                 : 1; /* [13..13] memory retention mode2控制。V7R2中memory不支持此bit控制0：进入retention；1：不进入retention。 */
        unsigned int    hifi_pgen                  : 1; /* [14..14] memory power down控制。 V7R2中memory不支持此bit控制0：不进入power down；1：进入power down。 */
        unsigned int    reserved_2                 : 1; /* [15..15] 保留。 */
        unsigned int    pd_axi_mem_ret1n           : 1; /* [16..16] memory retention mode1控制。0：进入retention；1：不进入retention。 */
        unsigned int    pd_axi_mem_ret2n           : 1; /* [17..17] memory retention mode2控制。0：进入retention；1：不进入retention。 */
        unsigned int    pd_axi_mem_pgen            : 1; /* [18..18] memory power down控制。0：不进入power down；1：进入power down。 */
        unsigned int    reserved_1                 : 1; /* [19..19] 保留。 */
        unsigned int    ret_axi_mem_ret1n          : 1; /* [20..20] memory retention mode1控制。0：进入retention；1：不进入retention。 */
        unsigned int    ret_axi_mem_ret2n          : 1; /* [21..21] memory retention mode2控制。0：进入retention；1：不进入retention。 */
        unsigned int    ret_axi_mem_pgen           : 1; /* [22..22] memory power down控制。0：不进入power down；1：进入power down。 */
        unsigned int    reserved_0                 : 9; /* [31..23] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL68_T;    /* memory低功耗配置寄存器2。 */

typedef union
{
    struct
    {
        unsigned int    axi_bbphy_xdcdr_sel        : 1; /* [0..0] 0：访问关钟、复位、掉电模块仅报错不改变译码逻辑，不返回DECERR；1：访问关钟、复位、掉电模块报错且译码到default slave，返回DECERR； */
        unsigned int    axi_amon_xdcdr_sel         : 1; /* [1..1] 0：访问关钟、复位、掉电模块仅报错不改变译码逻辑，不返回DECERR；1：访问关钟、复位、掉电模块报错且译码到default slave，返回DECERR； */
        unsigned int    axi_cpu_xdcdr_sel          : 1; /* [2..2] 0：访问关钟、复位、掉电模块仅报错不改变译码逻辑，不返回DECERR；1：访问关钟、复位、掉电模块报错且译码到default slave，返回DECERR； */
        unsigned int    axi_glb_xdcdr_sel          : 1; /* [3..3] 0：访问关钟、复位、掉电模块仅报错不改变译码逻辑，不返回DECERR；1：访问关钟、复位、掉电模块报错且译码到default slave，返回DECERR； */
        unsigned int    sysapb_pslverr_sel         : 1; /* [4..4] 0：访问关钟、复位、掉电模块仅报错不改变译码逻辑，不返回DECERR；1：访问关钟、复位、掉电模块报错且译码到default slave，返回DECERR； */
        unsigned int    socapb_pslverr_sel         : 1; /* [5..5] 0：访问关钟、复位、掉电模块仅报错不改变译码逻辑，不返回DECERR；1：访问关钟、复位、掉电模块报错且译码到default slave，返回DECERR； */
        unsigned int    x2h_hslverr_sel            : 1; /* [6..6] x2h桥译码逻辑：0：访问关钟、复位、掉电模块仅报错不改变译码逻辑，系统可能挂死；1：访问关钟、复位、掉电模块报错且译码到default slave，系统不挂死，default slave返回ok； */
        unsigned int    mst_err_srst_req           : 1; /* [7..7] 所有错误信息软复位请求。0：不复位。1：复位所有mst_err中断和状态。 */
        unsigned int    reserved                   : 24; /* [31..8] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL69_T;    /* 系统错误地址访问功能寄存器。 */

typedef union
{
    struct
    {
        unsigned int    axi_bbphy_priority_m1      : 2; /* [1..0] dw_axi_bbphy m1口（dw_axi_glb桥）优先级配置。值越大优先级越高。 */
        unsigned int    axi_bbphy_priority_m2      : 2; /* [3..2] dw_axi_bbphy m2口（dsp0）优先级配置。值越大优先级越高。 */
        unsigned int    axi_bbphy_priority_m3      : 2; /* [5..4] dw_axi_bbphy m3口（LTEBBP/TDSBBP）优先级配置。值越大优先级越高。 */
        unsigned int    axi_bbphy_priority_m4      : 2; /* [7..6] dw_axi_bbphy m4口（WBBP）优先级配置。 */
        unsigned int    axi_guacc_priority_m1      : 2; /* [9..8] dw_axi_guacc m1口（cicom0）优先级配置。值越大优先级越高。相同值的端口号小的优先级高。 */
        unsigned int    axi_guacc_priority_m2      : 2; /* [11..10] dw_axi_guacc m2口（cicom1）优先级配置。值越大优先级越高。相同值的端口号小的优先级高。 */
        unsigned int    axi_guacc_priority_m3      : 2; /* [13..12] dw_axi_guacc m3口（hdlc）优先级配置。值越大优先级越高。相同值的端口号小的优先级高。 */
        unsigned int    axi_guacc_priority_m4      : 2; /* [15..14] dw_axi_guacc m4口（upacc）优先级配置。值越大优先级越高。相同值的端口号小的优先级高。 */
        unsigned int    axi_mst_priority_m9        : 4; /* [19..16] dw_axi_mxt m9口（CM3）优先级配置。值越大优先级越高。 */
        unsigned int    axi_mst_priority_m10       : 4; /* [23..20] dw_axi_mxt m10口（usb3）优先级配置。值越大优先级越高。 */
        unsigned int    reserved_1                 : 4; /* [27..24] 保留。 */
        unsigned int    axi_cpu_priority_m1        : 1; /* [28..28] dw_axi_cpu桥m1口（App A9 M1口）优先级配置。值越大优先级越高。 */
        unsigned int    axi_cpu_priority_m2        : 1; /* [29..29] dw_axi_cpu桥m2口（Modem A9 M1口）优先级配置。值越大优先级越高。 */
        unsigned int    axi_amon_mst_priority_m1   : 1; /* [30..30] axi_amon m1口（axi_glb）优先级配置。值越大优先级越高。 */
        unsigned int    reserved_0                 : 1; /* [31..31] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL70_T;    /* 互联优先级配置寄存器0。 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 4; /* [3..0] 保留 */
        unsigned int    axi_glb_priority_m1        : 3; /* [6..4] dw_axi_glb m1口（dw_axi_bbphy）优先级配置。值越大优先级越高。 */
        unsigned int    axi_glb_priority_m2        : 3; /* [9..7] dw_axi_glb m2口（dw_axi_mst）优先级配置。值越大优先级越高。 */
        unsigned int    axi_glb_priority_m3        : 3; /* [12..10] dw_axi_glb m3口（appa9）优先级配置。值越大优先级越高。 */
        unsigned int    axi_glb_priority_m4        : 3; /* [15..13] dw_axi_glb m4口（mdma9）优先级配置。值越大优先级越高。 */
        unsigned int    axi_glb_priority_m5        : 3; /* [18..16] dw_axi_glb m5口（reserved）优先级配置。值越大优先级越高。 */
        unsigned int    axi_glb_priority_m6        : 3; /* [21..19] dw_axi_glb m6口（hifi）优先级配置。值越大优先级越高。 */
        unsigned int    axi_mst_cache_cfg_en       : 1; /* [22..22] APP与MOD CPU cache功能使能。0：使用外设master cache（默认）；1：使用axi_mst_cache配置值。 */
        unsigned int    axi_mst_sideband           : 5; /* [27..23] 由GLB AXI桥传入到ACP AW/ARUSERS端的控制信号。[24:21]内部属性0000：strongly-ordered；0001：device；0011：normal memory non-cacheable；0110：write-through；0111：write-back no write allocate；1111：write-back write allocate。[20]共享，AWIDMx[2]=0时0：non-coherent request；1：coherent request。 */
        unsigned int    axi_mst_cache              : 4; /* [31..28] APP与MOD CPU cache值。0000：noncacheable，nonbufferable0001：bufferable only0010：cacheable，but do not allocate0011：cacheable，bufferable，but do not allocate0100：reserved0101：reserved0110：cachable write-through, allocate on reads only0111：cachable write-back, allocate on reads only1000：reserved1001：reserved1010：cachable write-through, allocate on writes only1011：cachable write-back, allocate on writes only1100：reserved1101：reserved1110：cachable write-through, allocate on write & reads 1111：cachable write-back, allocate on write &reads */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL71_T;    /* 互联优先级配置寄存器1。 */

typedef union
{
    struct
    {
        unsigned int    axi_mst_priority_m1        : 4; /* [3..0] dw_axi_mxt m1口（dw_ahb）优先级配置。值越大优先级越高。 */
        unsigned int    axi_mst_priority_m2        : 4; /* [7..4] dw_axi_mxt m2口（edmac_ch4）优先级配置。值越大优先级越高。 */
        unsigned int    axi_mst_priority_m3        : 4; /* [11..8] dw_axi_mxt m3口（cipher）优先级配置。值越大优先级越高。 */
        unsigned int    axi_mst_priority_m4        : 4; /* [15..12] dw_axi_mxt m4口（ipf）优先级配置。值越大优先级越高。 */
        unsigned int    axi_mst_priority_m5        : 4; /* [19..16] dw_axi_mxt m5口（dw_axi_guacc）优先级配置。值越大优先级越高。 */
        unsigned int    axi_mst_priority_m6        : 4; /* [23..20] dw_axi_mxt m6口（cs_tmc）优先级配置。值越大优先级越高。 */
        unsigned int    axi_mst_priority_m7        : 4; /* [27..24] dw_axi_mxt m7口（socp）优先级配置。值越大优先级越高。 */
        unsigned int    axi_mst_priority_m8        : 4; /* [31..28] dw_axi_mxt m8口（edmac）优先级配置。值越大优先级越高。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL72_T;    /* 互联优先级配置寄存器2。 */

typedef union
{
    struct
    {
        unsigned int    appa9_gpio0_en             : 32; /* [31..0] 每1bit控制1bit中断。0：中断使能；1：中断不使能； */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL81_T;    /* appa9 gpio0中断按位使能寄存器。 */

typedef union
{
    struct
    {
        unsigned int    mdma9_gpio0_en             : 32; /* [31..0] 每1bit控制1bit中断。0：中断使能；1：中断不使能； */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL85_T;    /* mdma9 gpio0中断按位使能寄存器。 */

typedef union
{
    struct
    {
        unsigned int    cm3_gpio0_en               : 32; /* [31..0] M3 GPIO0中断使能寄存器。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL98_T;    /* M3 GPIO0中断使能寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved_6                 : 1; /* [0..0] 保留 。 */
        unsigned int    bbphy_dsp0_axislv_active   : 1; /* [1..1] dw_axi_bbyphy桥上dsp0 axi slave口active信号屏蔽寄存器。 */
        unsigned int    bbphy_bbp_axislv_active    : 1; /* [2..2] dw_axi_bbyphy桥上bbp axi slave口active信号屏蔽寄存器。 */
        unsigned int    reserved_5                 : 1; /* [3..3] 保留。 */
        unsigned int    amon_soc_axislv_active     : 1; /* [4..4] dw_axi_amon桥上amon_soc slave口active信号屏蔽寄存器。 */
        unsigned int    amon_cpufast_axislv_active : 1; /* [5..5] dw_axi_amon桥上amon_cpufast slave口active信号屏蔽寄存器。 */
        unsigned int    cs_axislv_active           : 1; /* [6..6] dw_axi_amon桥上coresight slave口active信号屏蔽寄存器。 */
        unsigned int    reserved_4                 : 1; /* [7..7] 保留。 */
        unsigned int    mdmacp_axislv_active       : 1; /* [8..8] dw_axi_glb桥上mdma9 slave口active信号屏蔽寄存器。 */
        unsigned int    appacp_axislv_active       : 1; /* [9..9] dw_axi_glb桥上mappa9 slave口active信号屏蔽寄存器。 */
        unsigned int    ddrdata_axislv_active      : 1; /* [10..10] dw_axi_glb桥上mddrc slave口active信号屏蔽寄存器。 */
        unsigned int    axi_mem_axislv_active      : 1; /* [11..11] dw_axi_glb桥上axi mem slave口active信号屏蔽寄存器。 */
        unsigned int    x2h_peri_axislv_active     : 1; /* [12..12] dw_axi_glb桥上x2p桥 slave口active信号屏蔽寄存器。 */
        unsigned int    bbphy_axislv_active        : 1; /* [13..13] dw_axi_glb桥上bbephy桥 slave口active信号屏蔽寄存器。 */
        unsigned int    x2p_axislv_active          : 1; /* [14..14] dw_axi_glb桥上x2p桥 slave口active信号屏蔽寄存器 */
        unsigned int    hifi_axislv_active         : 1; /* [15..15] dw_axi_glb桥上hifi slave口active信号屏蔽寄存器。 */
        unsigned int    dw_axi_amon_axislv_active  : 1; /* [16..16] dw_axi_glb桥上dw_axi_amon slave口active信号屏蔽寄存器。 */
        unsigned int    cpu_slv_active             : 1; /* [17..17] dw_axi_cpu桥slave口active屏蔽寄存器。（ddr data actvie）0：不屏蔽；1：屏蔽active逻辑；slave固定为可访问状态； */
        unsigned int    reserved_3                 : 1; /* [18..18] 保留。 */
        unsigned int    wdog_apbslv_active         : 1; /* [19..19] wdog apb slave口active信号屏蔽寄存器。 */
        unsigned int    timer0_7_apbslv_active     : 1; /* [20..20] timer0_7 apb slave口active信号屏蔽寄存器。 */
        unsigned int    timer8_15_apbslv_active    : 1; /* [21..21] timer8_15 apb slave口active信号屏蔽寄存器。 */
        unsigned int    timer16_23_apbslv_active   : 1; /* [22..22] timer16_23 apb slave口active信号屏蔽寄存器。 */
        unsigned int    reserved_2                 : 1; /* [23..23] 保留。 */
        unsigned int    gpio0_apbslv_active        : 1; /* [24..24] gpio0 apb slave口active信号屏蔽寄存器。 */
        unsigned int    uart0_apbslv_active        : 1; /* [25..25] uart0 apb slave口active信号屏蔽寄存器。 */
        unsigned int    reserved_1                 : 1; /* [26..26] 保留。 */
        unsigned int    sci0_apbslv_active         : 1; /* [27..27] sci0 apb slave口active信号屏蔽寄存器。 */
        unsigned int    sci1_apbslv_apbactive      : 1; /* [28..28] sci1 apb slave口active信号屏蔽寄存器。 */
        unsigned int    tsensor_apbslv_active      : 1; /* [29..29] tsensor apb slave口active信号屏蔽寄存器。 */
        unsigned int    usbotg_apbslv_active       : 1; /* [30..30] usbotg apb slave口active信号屏蔽寄存器。 */
        unsigned int    reserved_0                 : 1; /* [31..31] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL103_T;    /* slave active屏蔽寄存器0。 */

typedef union
{
    struct
    {
        unsigned int    cs_slv_active              : 1; /* [0..0] coresight apb slave口active信号屏蔽寄存器 */
        unsigned int    pmussi0_slv_active         : 1; /* [1..1] pmussi0 apb slave口active信号屏蔽寄存器 */
        unsigned int    reserved_8                 : 1; /* [2..2] 保留 */
        unsigned int    usb_otg_slv_active         : 1; /* [3..3] usb_otg apb slave口active信号屏蔽寄存器 */
        unsigned int    reserved_7                 : 1; /* [4..4] 保留 */
        unsigned int    reserved_6                 : 1; /* [5..5] 保留 */
        unsigned int    dwssi0_slv_active          : 1; /* [6..6] dw_ssi0 apb slave口active信号屏蔽寄存器 */
        unsigned int    gpio1_slv_active           : 1; /* [7..7] gpio1 apb slave口active信号屏蔽寄存器 */
        unsigned int    gpio2_slv_active           : 1; /* [8..8] gpio2 apb slave口active信号屏蔽寄存器 */
        unsigned int    gpio3_slv_active           : 1; /* [9..9] gpio3 apb slave口active信号屏蔽寄存器 */
        unsigned int    reserved_5                 : 1; /* [10..10] 保留 */
        unsigned int    uart1_slv_active           : 1; /* [11..11] uart1 apb slave口active信号屏蔽寄存器 */
        unsigned int    uart2_slv_active           : 1; /* [12..12] uart2 apb slave口active信号屏蔽寄存器 */
        unsigned int    uart3_slv_active           : 1; /* [13..13] uart3 apb slave口active信号屏蔽寄存器 */
        unsigned int    i2c0_slv_active            : 1; /* [14..14] i2c0 apb slave口active信号屏蔽寄存器 */
        unsigned int    i2c1_slv_active            : 1; /* [15..15] i2c1 apb slave口active信号屏蔽寄存器 */
        unsigned int    mddrc_slv_active           : 1; /* [16..16] mddrc 配置口 apb slave口active信号屏蔽寄存器 */
        unsigned int    efusec_slv_active          : 1; /* [17..17] efusec apb slave口active信号屏蔽寄存器。 */
        unsigned int    reserved_4                 : 1; /* [18..18] 保留。 */
        unsigned int    dwssi1_slv_active          : 1; /* [19..19] dw_ssi1 apb slave口active信号屏蔽寄存器。 */
        unsigned int    edam_slv_active            : 1; /* [20..20] edma apb slave口active信号屏蔽寄存器。 */
        unsigned int    dt_slv_active              : 1; /* [21..21] dt apb slave口active信号屏蔽寄存器。 */
        unsigned int    edmac_ch4_slv_active       : 1; /* [22..22] edma_ch4 apb slave口active信号屏蔽寄存器。 */
        unsigned int    reserved_3                 : 1; /* [23..23] 保留。 */
        unsigned int    pcie_phy_slv_active        : 1; /* [24..24] pcie_phy apb slave口active信号屏蔽寄存器。 */
        unsigned int    reserved_2                 : 1; /* [25..25] 保留。 */
        unsigned int    mipi_slv_active            : 1; /* [26..26] mipi apb slave口active信号屏蔽寄存器。 */
        unsigned int    reserved_1                 : 1; /* [27..27] 保留。 */
        unsigned int    bbp_ao_slv_active          : 1; /* [28..28] bbp_ao apb slave口active信号屏蔽寄存器。 */
        unsigned int    reserved_0                 : 1; /* [29..29] 保留。 */
        unsigned int    pwm0_slv_active            : 1; /* [30..30] pwm0 apb slave口active信号屏蔽寄存器。 */
        unsigned int    pwm1_slv_active            : 1; /* [31..31] pwm1 apb slave口active信号屏蔽寄存器0：不屏蔽；1：屏蔽active逻辑；slave固定为可访问状态； */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL104_T;    /* slave active屏蔽寄存器1。 */

typedef union
{
    struct
    {
        unsigned int    uicc                       : 1; /* [0..0] uicc ahb slave口active信号屏蔽寄存器。0：不屏蔽；1：屏蔽active逻辑；slave固定为可访问状态； */
        unsigned int    hsic                       : 1; /* [1..1] hsic ahb slave口active信号屏蔽寄存器。0：不屏蔽；1：屏蔽active逻辑；slave固定为可访问状态； */
        unsigned int    reserved_8                 : 1; /* [2..2] 保留 */
        unsigned int    emi_reg                    : 1; /* [3..3] emi ahb slave口active信号屏蔽寄存器。0：不屏蔽；1：屏蔽active逻辑；slave固定为可访问状态； */
        unsigned int    emi_mem                    : 1; /* [4..4] emi ahb slave口active信号屏蔽寄存器。0：不屏蔽；1：屏蔽active逻辑；slave固定为可访问状态； */
        unsigned int    nandc_reg                  : 1; /* [5..5] nandc ahb slave口active信号屏蔽寄存器。0：不屏蔽；1：屏蔽active逻辑；slave固定为可访问状态； */
        unsigned int    ipcm                       : 1; /* [6..6] ipcm ahb slave口active信号屏蔽寄存器。0：不屏蔽；1：屏蔽active逻辑；slave固定为可访问状态； */
        unsigned int    sio                        : 1; /* [7..7] sio ahb slave口active信号屏蔽寄存器。0：不屏蔽；1：屏蔽active逻辑；slave固定为可访问状态； */
        unsigned int    hs_uart                    : 1; /* [8..8] hs uart ahb slave口active信号屏蔽寄存器。0：不屏蔽；1：屏蔽active逻辑；slave固定为可访问状态； */
        unsigned int    socp                       : 1; /* [9..9] soc ahb slave口active信号屏蔽寄存器。0：不屏蔽；1：屏蔽active逻辑；slave固定为可访问状态； */
        unsigned int    reserved_7                 : 1; /* [10..10] 保留 */
        unsigned int    cipher                     : 1; /* [11..11] cipher ahb slave口active信号屏蔽寄存器。0：不屏蔽；1：屏蔽active逻辑；slave固定为可访问状态； */
        unsigned int    ipf                        : 1; /* [12..12] ipf ahb slave口active信号屏蔽寄存器。0：不屏蔽；1：屏蔽active逻辑；slave固定为可访问状态； */
        unsigned int    mmc0                       : 1; /* [13..13] mmc0 ahb slave口active信号屏蔽寄存器。0：不屏蔽；1：屏蔽active逻辑；slave固定为可访问状态； */
        unsigned int    mmc1                       : 1; /* [14..14] mmc1 ahb slave口active信号屏蔽寄存器。0：不屏蔽；1：屏蔽active逻辑；slave固定为可访问状态； */
        unsigned int    reserved_6                 : 1; /* [15..15] 保留 */
        unsigned int    sdcc                       : 1; /* [16..16] sdcc ahb slave口active信号屏蔽寄存器。0：不屏蔽；1：屏蔽active逻辑；slave固定为可访问状态； */
        unsigned int    reserved_5                 : 1; /* [17..17] 保留 */
        unsigned int    reserved_4                 : 1; /* [18..18] 保留 */
        unsigned int    cicom0                     : 1; /* [19..19] cicom0 ahb slave口active信号屏蔽寄存器。0：不屏蔽；1：屏蔽active逻辑；slave固定为可访问状态； */
        unsigned int    cicom1                     : 1; /* [20..20] cicom1 ahb slave口active信号屏蔽寄存器。0：不屏蔽；1：屏蔽active逻辑；slave固定为可访问状态； */
        unsigned int    hdlc                       : 1; /* [21..21] hdlc ahb slave口active信号屏蔽寄存器。0：不屏蔽；1：屏蔽active逻辑；slave固定为可访问状态； */
        unsigned int    usb3                       : 1; /* [22..22] usb3控制器 ahb slave口active信号屏蔽寄存器。0：不屏蔽；1：屏蔽active逻辑；slave固定为可访问状态； */
        unsigned int    nandc_mem                  : 1; /* [23..23] nandc ahb slave口active信号屏蔽寄存器。0：不屏蔽；1：屏蔽active逻辑；slave固定为可访问状态； */
        unsigned int    reserved_3                 : 1; /* [24..24] 保留 */
        unsigned int    vic1                       : 1; /* [25..25] vic1 ahb slave口active信号屏蔽寄存器。0：不屏蔽；1：屏蔽active逻辑；slave固定为可访问状态； */
        unsigned int    cm3                        : 1; /* [26..26] cm3 ahb slave口active信号屏蔽寄存器。0：不屏蔽；1：屏蔽active逻辑；slave固定为可访问状态； */
        unsigned int    reserved_2                 : 1; /* [27..27] 保留 */
        unsigned int    reserved_1                 : 1; /* [28..28] 保留 */
        unsigned int    upacc                      : 1; /* [29..29] upacc ahb slave口active信号屏蔽寄存器。0：不屏蔽；1：屏蔽active逻辑；slave固定为可访问状态； */
        unsigned int    reserved_0                 : 1; /* [30..30] 保留 */
        unsigned int    tcssi1                     : 1; /* [31..31] tcss1 控制器 ahb slave口active信号屏蔽寄存器。0：不屏蔽；1：屏蔽active逻辑；slave固定为可访问状态； */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL105_T;    /* slave active屏蔽寄存器2。 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 32; /* [31..0]  */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL121_T;    /* 保留寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 32; /* [31..0]  */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL122_T;    /* 保留寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 32; /* [31..0]  */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL123_T;    /* 保留寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 32; /* [31..0]  */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL124_T;    /* 保留寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 32; /* [31..0]  */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL125_T;    /* 保留寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 32; /* [31..0]  */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL126_T;    /* 保留寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 32; /* [31..0]  */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL127_T;    /* 保留寄存器。 */

typedef union
{
    struct
    {
        unsigned int    boot_mode2_0               : 3; /* [2..0] 启动模式选择：000：片外nandflash启动001：onchiprom nand/usb启动010：onchiprom emmac/usb启动011：onchiprom HS_UART启动100：onchiprom HSIC启动其他值：保留                                              说明：efuse使用1bit控制启动模式，具体含义请参照efuse寄存器手册。    0：根据boot_mode[3:0]指示启动       1：固定从onchiprom启动 */
        unsigned int    boot_mode3                 : 1; /* [3..3] USB强制自举指示                   0：onchiprom启动，强制USB自举     1：onchiprom启动，正常启动，需根据boot_mode2_0配置判断启动方式 */
        unsigned int    reserved_2                 : 4; /* [7..4] 保留。 */
        unsigned int    jtag_mode0                 : 1; /* [8..8]  */
        unsigned int    jtag_mode1                 : 1; /* [9..9]  */
        unsigned int    reserved_1                 : 1; /* [10..10] 保留。 */
        unsigned int    jtag_dft_mode              : 1; /* [11..11] JTAG DFT模式。0：正常状态（默认）；1：JTAG0：BSD，JTAG1：NO USE。 */
        unsigned int    wdt_rst_n_store            : 1; /* [12..12] 看门狗复位控制状态0：看门狗复位过PMU1：无此bit写1清除；该寄存器无异步复位功能，上电后初始值不确定，需要软件在上电后对该位写1，复位记录功能才可正常使用 */
        unsigned int    pmu_rst_n_store            : 1; /* [13..13] 系统软件复位控制状态0：软件复位过PMU1：无此bit写1清除；该寄存器无异步复位功能，上电后初始值不确定，需要软件在上电后对该位写1，复位记录功能才可正常使用 */
        unsigned int    reserved_0                 : 18; /* [31..14] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT0_T;    /* 复位记录/启动/调试模式状态寄存器。 */

typedef union
{
    struct
    {
        unsigned int    appa9_pmupriv              : 1; /* [0..0] AppA9状态指示。0：用户模式；1：特权模式。该比特信息在CoreSight中不提供。 */
        unsigned int    appa9_pmusecure            : 1; /* [1..1] AppA9安全状态。0：非安全状态；1：安全状态。该比特信息在CoreSight中不提供。 */
        unsigned int    appa9_smpnamp              : 1; /* [2..2] AppA9 AMP/SMP模式。0：不对称；1：对称。 */
        unsigned int    appa9_scuevabort           : 1; /* [3..3] 异常标志信号，指示在一致性逐出期间外部发生了终止操作，用于CPU性能监控，不关心下可以悬空 */
        unsigned int    appa9_pwrctlo0             : 2; /* [5..4] 指示CPU当前工作状态。0x：CPU0必须上电；10：CPU0可以进入dormant mode；11：CPU0可以进入powered-off mode。 */
        unsigned int    appa9_l2_tagclkouten       : 1; /* [6..6] l2 tagclkouten状态。 */
        unsigned int    appa9_l2_dataclkouten      : 4; /* [10..7] l2 dataclkouten状态。 */
        unsigned int    appa9_l2_idle              : 1; /* [11..11] l2 idle状态。 */
        unsigned int    appa9_l2_clkstopped        : 1; /* [12..12] l2 clk_stopped 状态。 */
        unsigned int    reserved_1                 : 3; /* [15..13] 保留。 */
        unsigned int    moda9_pmupriv              : 1; /* [16..16] ModA9状态指示。0：用户模式；1：特权模式。该比特信息在CoreSight中不提供。 */
        unsigned int    moda9_pmusecure            : 1; /* [17..17] ModA9安全状态。0：非安全状态；1：安全状态。该比特信息在CoreSight中不提供。 */
        unsigned int    moda9_smpnamp              : 1; /* [18..18] ModA9 AMP/SMP模式。0：不对称；1：对称。 */
        unsigned int    moda9_scuevabort           : 1; /* [19..19] 异常标志信号，指示在一致性逐出期间外部发生了终止操作，用于CPU性能监控，不关心下可以悬空。0：未发生1：已发生 */
        unsigned int    moda9_pwrctlo0             : 2; /* [21..20] CPU当前工作状态。0x：CPU0必须上电；10：CPU0可以进入dormant mode；11：CPU0可以进入powered-off mode。 */
        unsigned int    moda9_l2_tagclkouten       : 1; /* [22..22] l2 tagclkouten状态。 */
        unsigned int    moda9_l2_dataclkouten      : 4; /* [26..23] l2 dataclkouten状态。 */
        unsigned int    moda9_l2_idle              : 1; /* [27..27] l2 idle状态。 */
        unsigned int    moda9_l2_clkstopped        : 1; /* [28..28] l2 clk_stopped 状态。 */
        unsigned int    reserved_0                 : 3; /* [31..29] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT1_T;    /* App/Modem A9状态寄存器。 */

typedef union
{
    struct
    {
        unsigned int    hifi_pwaitmode             : 1; /* [0..0] 处理器睡眠状态指示。0：正常；1：睡眠。当DSP执行WAITI指令等待中断时，此信号置起。任何没有disable的中断都会唤醒处理器。 */
        unsigned int    hifi_xocdmode              : 1; /* [1..1] 指示处理器处于OCD Halt模式。0：正常；1：OCD Halt模式。 */
        unsigned int    appa9_deflags              : 7; /* [8..2] A核deflags状态。 */
        unsigned int    reserved_1                 : 7; /* [15..9] 保留。 */
        unsigned int    dsp0_pwaitmode             : 1; /* [16..16] 处理器睡眠状态指示。0：正常；1：睡眠。当DSP执行WAITI指令等待中断时，此信号置起。任何没有disable的中断都会唤醒处理器。 */
        unsigned int    dsp0_xocdmode              : 1; /* [17..17] 指示处理器处于OCD Halt模式。0：正常；1：OCD Halt模式。 */
        unsigned int    reserved_0                 : 14; /* [31..18] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT2_T;    /* HIFI/dsp0/BBE16 DSP CORE状态寄存器。 */

typedef union
{
    struct
    {
        unsigned int    hpm0_opc                   : 10; /* [9..0] hpm0的OPC值。 */
        unsigned int    hpm0_opc_vld               : 1; /* [10..10] hpm0的opc有效信号。 */
        unsigned int    reserved_1                 : 1; /* [11..11] 保留。 */
        unsigned int    hpm1_opc                   : 10; /* [21..12] hpm1的OPC值。 */
        unsigned int    hpm1_opc_vld               : 1; /* [22..22] hpm1的opc有效信号。 */
        unsigned int    reserved_0                 : 9; /* [31..23] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT3_T;    /* hmp状态寄存器0。 */

typedef union
{
    struct
    {
        unsigned int    hpm2_opc                   : 10; /* [9..0] hpm2的OPC值。 */
        unsigned int    hpm2_opc_vld               : 1; /* [10..10] hpm2的opc有效信号。 */
        unsigned int    reserved                   : 21; /* [31..11] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT4_T;    /* hmp状态寄存器1。 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 32; /* [31..0] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT5_T;    /* 保留寄存器 */

typedef union
{
    struct
    {
        unsigned int    appa9_parityfail0          : 8; /* [7..0] AppA9 RAM阵列校验结果。0：校验成功；1：校验失败。[7] BTAC parity error[6] GHB[5] Instruction tag RAM[4] Instruction data RAM[3] Main TLB[2] D outer RAM[1] Data tag RAM[0] Data data RAM */
        unsigned int    appa9_parityfailscu        : 1; /* [8..8] AppA9 SCU tag RAM校验结果。0：校验成功；1：校验失败。 */
        unsigned int    reserved_1                 : 3; /* [11..9] 保留。 */
        unsigned int    moda9_parityfail0          : 8; /* [19..12] A9 RAM阵列校验结果。0：校验成功；1：校验失败。[7] BTAC parity error[6] GHB[5] Instruction tag RAM[4] Instruction data RAM[3] Main TLB[2] D outer RAM[1] Data tag RAM[0] Data data RAM */
        unsigned int    moda9_parityfailscu        : 1; /* [20..20] SCU tag RAM校验结果。0：校验成功；1：校验失败。 */
        unsigned int    reserved_0                 : 11; /* [31..21] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT6_T;    /* App/Modem A9校验状态寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 32; /* [31..0]  */
    } bits;
    unsigned int    u32;
}HI_SC_STAT7_T;    /* 保留寄存器。 */

typedef union
{
    struct
    {
        unsigned int    bbp_timer_readcnt          : 32; /* [31..0] BBP可维可测读次数计数器，每次对此寄存器进行读操作数值加1（读加1）。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT8_T;    /* LTE BBP可维可测读计数器。 */

typedef union
{
    struct
    {
        unsigned int    dw_axi_mst_dlock_wr        : 1; /* [0..0] 死锁传输读写指示。0：读；1：写。 */
        unsigned int    dw_axi_mst_dlock_slv       : 1; /* [1..1] 跟随死锁master的slave端口号。 */
        unsigned int    reserved_3                 : 2; /* [3..2] 保留。 */
        unsigned int    dw_axi_mst_dlock_mst       : 4; /* [7..4] 死锁的master最小端口号。 */
        unsigned int    dw_axi_cpu_dlock_wr        : 1; /* [8..8] 死锁传输读写指示。0：读；1：写。 */
        unsigned int    dw_axi_cpu_dlock_slv       : 1; /* [9..9] 跟随死锁master的slave端口号。 */
        unsigned int    dw_axi_cpu_dlock_mst       : 1; /* [10..10] 死锁的master最小端口号。 */
        unsigned int    reserved_2                 : 1; /* [11..11] 保留。 */
        unsigned int    axi_amon_dlock_wr          : 1; /* [12..12] 死锁传输读写指示。0：读；1：写。 */
        unsigned int    axi_amon_dlock_slv         : 3; /* [15..13] 跟随死锁master的slave端口号。 */
        unsigned int    axi_amon_dlock_mst         : 3; /* [18..16] 死锁的master最小端口号。 */
        unsigned int    reserved_1                 : 1; /* [19..19] 保留。 */
        unsigned int    dw_axi_glb_dlock_wr        : 1; /* [20..20] 死锁传输读写指示。0：读；1：写。 */
        unsigned int    dw_axi_glb_dlock_slv       : 4; /* [24..21] 跟随死锁master的slave端口号。 */
        unsigned int    dw_axi_glb_dlock_mst       : 3; /* [27..25] 死锁的master最小端口号。 */
        unsigned int    reserved_0                 : 4; /* [31..28] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT9_T;    /* AXI桥锁死状态寄存器0。 */

typedef union
{
    struct
    {
        unsigned int    dw_axi_glb_dlock_id        : 8; /* [7..0] GLB AXI桥死锁ID号 */
        unsigned int    dw_axi_amon_dlock_id       : 12; /* [19..8] AMON AXI桥死锁ID号 */
        unsigned int    dw_axi_cpu_dlock_id        : 8; /* [27..20] CPU AXI桥死锁ID号 */
        unsigned int    dw_axi_mst_dlock_id        : 4; /* [31..28] MST AXI桥死锁ID号 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT10_T;    /* AXI桥锁死状态寄存器1。 */

typedef union
{
    struct
    {
        unsigned int    sdcc_pull_stat             : 1; /* [0..0] sdcc控制器上拉请求状态0：无1：请求上拉（默认值） */
        unsigned int    reserved                   : 31; /* [31..1] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT11_T;    /* SDCC上拉状态寄存器 */

typedef union
{
    struct
    {
        unsigned int    sdcc_pull_raw              : 1; /* [0..0] sdcc控制器上拉请求变化原始中断状态0：上拉请求状态无变化1：上拉请求状态有变化此bit写1清除 */
        unsigned int    reserved                   : 31; /* [31..1] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT12_T;    /* SDCC上拉变化中断状态寄存器 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 32; /* [31..0]  */
    } bits;
    unsigned int    u32;
}HI_SC_STAT13_T;    /* 保留寄存器 */

typedef union
{
    struct
    {
        unsigned int    cr_data_out                : 16; /* [15..0]  */
        unsigned int    cr_ack                     : 1; /* [16..16]  */
        unsigned int    reserved_2                 : 3; /* [19..17]  */
        unsigned int    rtune_ack                  : 1; /* [20..20]  */
        unsigned int    reserved_1                 : 3; /* [23..21]  */
        unsigned int    ref_clkreq_n               : 1; /* [24..24]  */
        unsigned int    reserved_0                 : 7; /* [31..25]  */
    } bits;
    unsigned int    u32;
}HI_SC_STAT14_T;    /* usb3PHY状态寄存器1。 */

typedef union
{
    struct
    {
        unsigned int    sysapb_pslv_active0        : 14; /* [13..0] 常开区apb slave口active信号状态0：bit[0]：sysctrl（常值1）bit[1]：wdogbit[2]：timer0～7bit[3]：timer8～15bit[4]：timer16～23bit[5]：rtc（常值1）bit[6]：gpio0bit[7]：uart0bit[8]：synopsys spi0bit[9]：sci0bit[10]：sci1bit[11]：tsensorbit[12]：bc_ctrl                                         bit[13]：crg(常值1) */
        unsigned int    reserved_1                 : 2; /* [15..14]  */
        unsigned int    sysapb_pslv_active1        : 3; /* [18..16] 常开区apb slave口active信号状态1：bit[0]：ios_pd（常值1）bit[1]：ios_ao（常值1）bit[2]：efusec（常值1） */
        unsigned int    reserved_0                 : 13; /* [31..19]  */
    } bits;
    unsigned int    u32;
}HI_SC_STAT15_T;    /* slave active状态寄存器。 */

typedef union
{
    struct
    {
        unsigned int    ldrx2dbg_abs_timer_31_0    : 32; /* [31..0]  */
    } bits;
    unsigned int    u32;
}HI_SC_STAT16_T;    /* lte drx abs_timer状态寄存器0 */

typedef union
{
    struct
    {
        unsigned int    ldrx2dbg_abs_timer_63_32   : 32; /* [31..0]  */
    } bits;
    unsigned int    u32;
}HI_SC_STAT17_T;    /* lte drx abs_timer状态寄存器1 */

typedef union
{
    struct
    {
        unsigned int    wdrx_deepsleep_flag        : 1; /* [0..0]  */
        unsigned int    g1_gdrx_deepsleep_falg     : 1; /* [1..1]  */
        unsigned int    g2_gdrx_deepsleep_falg     : 1; /* [2..2]  */
        unsigned int    reserved                   : 29; /* [31..3] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT18_T;    /* gsm deepsleep状态寄存器 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 32; /* [31..0] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT19_T;    /* 保留寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 32; /* [31..0] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT20_T;    /* 保留寄存器。 */

typedef union
{
    struct
    {
        unsigned int    usb3_connect_state_u2pmu   : 1; /* [0..0]  */
        unsigned int    usb3_connect_state_u3pmu   : 1; /* [1..1]  */
        unsigned int    usb3_host_current_belt     : 12; /* [13..2]  */
        unsigned int    reserved_1                 : 2; /* [15..14]  */
        unsigned int    usb3_pmu_current_power_state_u2pmu : 2; /* [17..16]  */
        unsigned int    usb3_pmu_current_power_state_u3pmu : 2; /* [19..18]  */
        unsigned int    usb3_pmu_phy_u2dsport_vbus_ctrl : 1; /* [20..20]  */
        unsigned int    usb3_pmu_phy_u3dsport_vbus_ctrl : 1; /* [21..21]  */
        unsigned int    reserved_0                 : 10; /* [31..22] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT21_T;    /* usb_pd状态寄存器。 */

typedef union
{
    struct
    {
        unsigned int    dw_axi_bbphy_dlock_mst     : 2; /* [1..0]  */
        unsigned int    dw_axi_bbphy_dlock_slv     : 3; /* [4..2]  */
        unsigned int    dw_axi_bbphy_dlock_wr      : 1; /* [5..5]  */
        unsigned int    reserved_1                 : 2; /* [7..6]  */
        unsigned int    dw_axi_bbphy_dlock_id      : 12; /* [19..8]  */
        unsigned int    reserved_0                 : 12; /* [31..20]  */
    } bits;
    unsigned int    u32;
}HI_SC_STAT22_T;    /* AXI桥锁死状态寄存器2。 */

typedef union
{
    struct
    {
        unsigned int    func_mbist_fail            : 32; /* [31..0] func mbist fail状态寄存器。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT23_T;    /* func mbist fail状态寄存器。 */

typedef union
{
    struct
    {
        unsigned int    func_mbist_done            : 32; /* [31..0] func mbist done状态寄存器。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT24_T;    /* func mbist done状态寄存器0。 */

typedef union
{
    struct
    {
        unsigned int    func_mbist_done            : 32; /* [31..0] func mbist done状态寄存器。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT25_T;    /* func mbist done状态寄存器1。 */

typedef union
{
    struct
    {
        unsigned int    socp_idle                  : 1; /* [0..0] socp是否空闲状态 */
        unsigned int    ipf_idle                   : 1; /* [1..1] ipf是否空闲状态 */
        unsigned int    cicom0_clk_state           : 1; /* [2..2] cicom0是否空闲状态 */
        unsigned int    cicom1_clk_state           : 1; /* [3..3] cicom1是否空闲状态 */
        unsigned int    hdlc_clk_state             : 1; /* [4..4] hdlc是否空闲状态 */
        unsigned int    reserved                   : 27; /* [31..5]  */
    } bits;
    unsigned int    u32;
}HI_SC_STAT26_T;    /* master状态寄存器。 */

typedef union
{
    struct
    {
        unsigned int    bbphy_slv_active           : 4; /* [3..0] dw_axi_bbphy桥slave口active信号状态：bit[0]：dw_axi_dspbit[1]：dsp0bit[2]：bbpbit[3]：reserved */
        unsigned int    amon_slv_active            : 3; /* [6..4] axi_amon桥slave口active信号状态：bit[4]：socbit[5]：cpu fastbit[6]：core sight */
        unsigned int    reserved_1                 : 1; /* [7..7] 保留。 */
        unsigned int    glb_slv_active             : 9; /* [16..8] dw_axi_glb桥slave口active信号状态：bit[8]：mdm acpbit[9]：app acpbit[10]：ddr databit[11]：axi_membit[12]：x2h_peribit[13]：dw_axi_dspbit[14]：x2p（常量1）bit[15]：hifibit[16]：stm */
        unsigned int    cpu_slv_active             : 1; /* [17..17] dw_axi_cpu桥slave口active状态。（ddr data actvie状态） */
        unsigned int    reserved_0                 : 14; /* [31..18] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT27_T;    /* slave active状态寄存器0。 */

typedef union
{
    struct
    {
        unsigned int    socapb_pslv_active         : 32; /* [31..0] 掉电区apb slave口active信号状态：bit[0]：coresightbit[1]：pmussi0bit[2]： （常值1）bit[3]：usb othphybit[5:4]：（常值11）bit[6]：ssi0bit[7]：gpio1bit[8]：gpio2bit[9]：gpio3bit[10]：sys_peri（常值1）bit[11]：uart1bit[12]：uart2bit[13]：uart3bit[14]：i2c0bit[15]：i2c1                                       bit[16]：mddrcbit[17]：efusebit[18]：reservedbit[19]：spi_fastbit[20]：edmabit[21]：ashbbit[22]：edma_ch4bit[23]：cm3 ashb（常值1）bit[24]：pcie_phybit[25]：reservedbit[26]：mipibit[27]：reservedbit[28]：bbpbit[29]：reservedbit[30]：pwm0                                        bit[31]：pwm1 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT28_T;    /* slave active状态寄存器1。 */

typedef union
{
    struct
    {
        unsigned int    x2h_peri_slv_active        : 32; /* [31..0] bit[0]：uiccbit[1]：hsicbit[2]：gmacbit[3]：emi配置口bit[4]：emi数据口bit[5]：nandc配置口bit[6]：ipcmbit[7]：siobit[8]：hs_uartbit[9]：socpbit[10]：reservedbit[11]：cipherbit[12]：ipfbit[13]：mmc0bit[14]：mmc1bit[15]：reservedbit[16]：sdccbit[17]：reservedbit[18]：reservedbit[19]：cicom0bit[20]：cicom1bit[21]：hdlcbit[22]：usb3bit[23]：nandc数据口bit[24]：reservedbit[25]：vic1bit[26]：cm3bit[27]：reservedbit[28]：reservedbit[29]：upacc                                          bit[30]：reservedbit[31]：reserved */
    } bits;
    unsigned int    u32;
}HI_SC_STAT29_T;    /* slave active状态寄存器2。 */

typedef union
{
    struct
    {
        unsigned int    bbp_sys_1control           : 32; /* [31..0]  */
    } bits;
    unsigned int    u32;
}HI_SC_STAT31_T;    /* bbp_sys_1control状态寄存器1。 */

typedef union
{
    struct
    {
        unsigned int    lte2soc_tbd                : 16; /* [15..0]  */
        unsigned int    reserved                   : 16; /* [31..16] 保留 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT32_T;    /* bbp保留状态寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 32; /* [31..0]  */
    } bits;
    unsigned int    u32;
}HI_SC_STAT33_T;    /* 保留寄存器 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 32; /* [31..0]  */
    } bits;
    unsigned int    u32;
}HI_SC_STAT34_T;    /* 保留寄存器 */

typedef union
{
    struct
    {
        unsigned int    func_mbist_done_2          : 32; /* [31..0] func mbist done状态寄存器 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT35_T;    /* func mbist done状态寄存器2。 */

typedef union
{
    struct
    {
        unsigned int    func_mbist_done_3          : 32; /* [31..0] func mbist done状态寄存器 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT36_T;    /* func mbist done状态寄存器3。 */

typedef union
{
    struct
    {
        unsigned int    func_mbist_done_4          : 32; /* [31..0] func mbist done状态寄存器 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT37_T;    /* func mbist done状态寄存器4。 */

typedef union
{
    struct
    {
        unsigned int    func_mbist_done_5          : 32; /* [31..0] func mbist done状态寄存器 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT38_T;    /* func mbist done状态寄存器5。 */

typedef union
{
    struct
    {
        unsigned int    func_mbist_done_6          : 32; /* [31..0] func mbist done状态寄存器 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT39_T;    /* func mbist done状态寄存器6。 */

typedef union
{
    struct
    {
        unsigned int    func_mbist_done_7          : 32; /* [31..0]  */
    } bits;
    unsigned int    u32;
}HI_SC_STAT40_T;    /* func mbist done状态寄存器7。 */

typedef union
{
    struct
    {
        unsigned int    bbphy_mst_err              : 8; /* [7..0] dw_axi_bbphy桥访问错误寄存器。Bit[0]：m1口（dw_axi_glb）写访问错误。Bit[1]：m1口（dw_axi_glb）读访问错误。Bit[2]：m2口（dsp0）写访问错误。Bit[3]：m2口（dsp0）读访问错误。Bit[4]：m3口（bbp）写访问错误。Bit[5]：m3口（bbp）读访问错误。Bit[6]：m4口（reserved）写访问错误。Bit[7]：m4口（reserved）读访问错误。 */
        unsigned int    axi_amon_mst_err           : 1; /* [8..8] axi_amon mst访问错误寄存器。 */
        unsigned int    reserved                   : 7; /* [15..9] 保留。 */
        unsigned int    cpu_mst_err                : 4; /* [19..16] dw_axi_cpu桥访问错误寄存器。Bit[0]：app a9 m1口写访问错误Bit[1]：app a9 m1口读访问错误地址Bit[2]：mdm a9 m1口写访问错误Bit[3]：mdm a9 m1口读访问错误 */
        unsigned int    glb_mst_err                : 12; /* [31..20] dw_axi_glb桥访问错误寄存器。Bit[0]：m1口（dw_axi_bbphy）写访问错误Bit[1]：m1口（dw_axi_bbphy）读访问错误Bit[2]：m2口（dw_axi_mst）写访问错误Bit[3]：m2口（dw_axi_mst）读访问错误Bit[4]：m3口（app a9 m0口）写访问错误Bit[5]：m3口（app a9 m0口）读访问错误Bit[6]：m4口（mdm a9 m0口）写访问错误Bit[7]：m4口（mdm a9 m0口）读访问错误Bit[8]：m5口（reserved）写访问错误Bit[9]：m5口（reserved）读访问错误Bit[10]：m6口（hifi）写访问错误Bit[11]：m6口（hifi）读访问错误 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT41_T;    /* master访问错误状态寄存器0。 */

typedef union
{
    struct
    {
        unsigned int    sysapb_psel_err            : 1; /* [0..0] soc外设常开区apb外设访问错误状态。 */
        unsigned int    socapb_psel_err            : 1; /* [1..1] soc外设掉电域apb外设访问错误状态。 */
        unsigned int    x2h_peri_mst_err           : 1; /* [2..2] ahb外设访问错误状态。 */
        unsigned int    reserved                   : 29; /* [31..3] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT42_T;    /* master访问错误状态寄存器1。 */

typedef union
{
    struct
    {
        unsigned int    x2h_peri_addr_err          : 32; /* [31..0] ahb外设访问错误地址寄存器。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT43_T;    /* ahb外设访问错误地址寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 12; /* [11..0] 保留。 */
        unsigned int    addr_err                   : 20; /* [31..12] soc外设掉电域apb外设访问错误地址寄存器。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT44_T;    /* soc外设掉电域apb外设访问错误地址寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 12; /* [11..0] 保留。 */
        unsigned int    reserved_0                 : 20; /* [31..12] soc外设常开区apb外设访问错误地址寄存器。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT45_T;    /* soc外设常开区apb外设访问错误地址寄存器。 */

typedef union
{
    struct
    {
        unsigned int    addr_err                   : 32; /* [31..0] glb桥m1口（dw_axi_bbphy）写访问错误地址寄存器。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT46_T;    /* glb桥m1口（dw_axi_bbphy）写访问错误地址寄存器。 */

typedef union
{
    struct
    {
        unsigned int    addr_err                   : 32; /* [31..0] glb桥m1口（dw_axi_bbphy）读访问错误地址寄存器。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT47_T;    /* glb桥m1口（dw_axi_bbphy）读访问错误地址寄存器。 */

typedef union
{
    struct
    {
        unsigned int    addr_err                   : 32; /* [31..0] glb桥m2口（dw_axi_mst）写访问错误地址寄存器。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT48_T;    /* glb桥m2口（dw_axi_mst）写访问错误地址寄存器。 */

typedef union
{
    struct
    {
        unsigned int    addr_err                   : 32; /* [31..0] glb桥m2口（dw_axi_mst）读访问错误地址寄存器。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT49_T;    /* glb桥m2口（dw_axi_mst）读访问错误地址寄存器。 */

typedef union
{
    struct
    {
        unsigned int    addr_err                   : 32; /* [31..0] glb桥m3口（app a9 m0口）写访问错误地址寄存器。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT50_T;    /* glb桥m3口（app a9 m0口）写访问错误地址寄存器。 */

typedef union
{
    struct
    {
        unsigned int    addr_err                   : 32; /* [31..0] glb桥m3口（app a9 m0口）读访问错误地址寄存器。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT51_T;    /* glb桥m3口（app a9 m0口）读访问错误地址寄存器。 */

typedef union
{
    struct
    {
        unsigned int    addr_err                   : 32; /* [31..0] glb桥m4口（mdm a9 m0口）写访问错误地址寄存器。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT52_T;    /* glb桥m4口（mdm a9 m0口）写访问错误地址寄存器。 */

typedef union
{
    struct
    {
        unsigned int    addr_err                   : 32; /* [31..0] glb桥m4口（mdm a9 m0口）读访问错误地址寄存器。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT53_T;    /* glb桥m4口（mdm a9 m0口）读访问错误地址寄存器。 */

typedef union
{
    struct
    {
        unsigned int    addr_err                   : 32; /* [31..0] glb桥m5口（reserved）写访问错误地址寄存器。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT54_T;    /* glb桥m5口（reserved）写访问错误地址寄存器。 */

typedef union
{
    struct
    {
        unsigned int    addr_err                   : 32; /* [31..0] glb桥m5口（reserved）读访问错误地址寄存器。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT55_T;    /* glb桥m5口（reserved）读访问错误地址寄存器。 */

typedef union
{
    struct
    {
        unsigned int    addr_err                   : 32; /* [31..0] glb桥m6口（hifi）写访问错误地址寄存器。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT56_T;    /* glb桥m6口（hifi）写访问错误地址寄存器。 */

typedef union
{
    struct
    {
        unsigned int    addr_err                   : 32; /* [31..0] glb桥m6口（hifi）读访问错误地址寄存器。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT57_T;    /* glb桥m6口（hifi）读访问错误地址寄存器。 */

typedef union
{
    struct
    {
        unsigned int    addr_err                   : 32; /* [31..0] cpu桥m1口（app a9 m1口）读访问错误地址寄存器。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT58_T;    /* cpu桥m1口（app a9 m1口）读访问错误地址寄存器。 */

typedef union
{
    struct
    {
        unsigned int    addr_err                   : 32; /* [31..0] cpu桥m2口（mdm a9 m1口）读访问错误地址寄存器。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT59_T;    /* cpu桥m2口（mdm a9 m1口）读访问错误地址寄存器。 */

typedef union
{
    struct
    {
        unsigned int    addr_err                   : 32; /* [31..0] cpu桥m1口（app a9 m1口）写访问错误地址寄存器。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT60_T;    /* cpu桥m1口（app a9 m1口）写访问错误地址寄存器。 */

typedef union
{
    struct
    {
        unsigned int    addr_err                   : 32; /* [31..0] cpu桥m2口（mdm a9 m1口）写访问错误地址寄存器。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT61_T;    /* cpu桥m2口（mdm a9 m1口）写访问错误地址寄存器。 */

typedef union
{
    struct
    {
        unsigned int    addr_err                   : 32; /* [31..0] bbphy桥m1口（dw_axi_glb）写访问错误地址寄存器。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT62_T;    /* bbphy桥m1口（dw_axi_glb）写访问错误地址寄存器。 */

typedef union
{
    struct
    {
        unsigned int    addr_err                   : 32; /* [31..0] bbphy桥m1口（dw_axi_glb）读访问错误地址寄存器。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT63_T;    /* bbphy桥m1口（dw_axi_glb）读访问错误地址寄存器。 */

typedef union
{
    struct
    {
        unsigned int    addr_err                   : 32; /* [31..0] bbphy桥m2口（dsp0）写访问错误地址寄存器。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT64_T;    /* bbphy桥m2口（dsp0）写访问错误地址寄存器。 */

typedef union
{
    struct
    {
        unsigned int    addr_err                   : 32; /* [31..0] bbphy桥m2口（dsp0）读访问错误地址寄存器。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT65_T;    /* bbphy桥m2口（dsp0）读访问错误地址寄存器。 */

typedef union
{
    struct
    {
        unsigned int    addr_err                   : 32; /* [31..0] bbphy桥m3口（bbp）写访问错误地址寄存器。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT66_T;    /* bbphy桥m3口（bbp）写访问错误地址寄存器。 */

typedef union
{
    struct
    {
        unsigned int    addr_err                   : 32; /* [31..0] bbphy桥m3口（bbp）读访问错误地址寄存器。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT67_T;    /* bbphy桥m3口（bbp）读访问错误地址寄存器。 */

typedef union
{
    struct
    {
        unsigned int    addr_err                   : 32; /* [31..0] axi_amon_m1写访问错误地址寄存器。？ */
    } bits;
    unsigned int    u32;
}HI_SC_STAT68_T;    /* axi_amon桥m1写访问错误地址寄存器。 */

typedef union
{
    struct
    {
        unsigned int    addr_err                   : 32; /* [31..0] axi_amon_m1读访问错误地址寄存器。？ */
    } bits;
    unsigned int    u32;
}HI_SC_STAT69_T;    /* axi_amon桥m1读访问错误地址寄存器。 */

typedef union
{
    struct
    {
        unsigned int    pcie_gm_cmposer_lookup_err : 1; /* [0..0] PCIe（X1）控制器。Master Response Composer Lookup Error.Indicates that an overflow that occurred in a lookup table ofthe outbound responses. This indicates that there was a violation for thenumber of outstanding Non-Posted requests issued for the inbounddirection.0：无效；1：有效。 */
        unsigned int    pcie_radmx_cmposer_lookup_err : 1; /* [1..1] PCIe（X1）控制器。Slave Response Composer Lookup Error。Indicates that an overflow that occurred in a lookup table ofthe inbound responses. This indicates that there was a violation for thenumber of outstanding Non-Posted requests issued for the outbounddirection。0：无效；1：有效。 */
        unsigned int    pcie_pm_xtlh_block_tlp     : 1; /* [2..2] PCIe（X1）控制器。由于当前的低功耗状态，通知应用层必须停止产生新的TLPs请求。应用层可以继续产生Completion TLPs。0：无效；1：有效。 */
        unsigned int    pcie_cfg_mem_space_en      : 1; /* [3..3] PCIe（X1）控制器。PCI-compatible命令寄存器中的Memory Space Enable bit位的状态。0：无效；1：有效。 */
        unsigned int    pcie_cfg_rcb               : 1; /* [4..4] PCIe（X1）控制器。Link控制寄存器中的RCB bit位。0：无效；1：有效。 */
        unsigned int    pcie_rdlh_link_up          : 1; /* [5..5] PCIe（X1）控制器。数据Link层 up/down 指示。0：link down；1：link up。 */
        unsigned int    pcie_pm_curnt_state        : 3; /* [8..6] PCIe（X1）控制器。表明当前的电源状态。 */
        unsigned int    pcie_cfg_aer_rc_err_int    : 1; /* [9..9] PCIe（X1）控制器当一个错误产生，导致Root Error Status寄存器中的某一bit置位，并且对应的Root Error Command寄存器中的使能bit有效，那么就有效这个信号；0：有效；1：无效。 */
        unsigned int    pcie_cfg_aer_int_msg_num   : 5; /* [14..10] PCIe（X1）控制器当MSI或者MSI-X使能的时候才有效，来自于Root Error Status的[31：27]bit。 */
        unsigned int    pcie_xmlh_link_up          : 1; /* [15..15] PCIe（X1）控制器。PHY Link up/down 指示。0：link down；1：link up。 */
        unsigned int    pcie_wake                  : 1; /* [16..16] PCIe（X1）控制器。唤醒信号，用于从低功耗的电源状态 */
        unsigned int    pcie_cfg_eml_control       : 1; /* [17..17] PCIe（X1）控制器。Electromechanical Interlock Control. The state ofthe Electromechanical Interlock Control bit in the SlotControl register。 */
        unsigned int    pcie_hp_pme                : 1; /* [18..18] PCIe（X1）控制器。当下面条件有效的时候，Core有效hp_pme。The PME Enable bit in the Power Management Controland Status register is set to 1.Any bit in the Slot Status register transitions from 0 to 1and the associated event notification is enabled in theSlot Control register。 */
        unsigned int    pcie_hp_int                : 1; /* [19..19] PCIe（X1）控制器。当下面条件满足的时候，此信号有效。The INTx Assertion Disable bit in the Command registeris 0.Hot-Plug interrupts are enabled in the Slot Controlregister.Any bit in the Slot Status register is equal to 1, and theassociated event notification is enabled in the SlotControl register。 */
        unsigned int    pcie_hp_msi                : 1; /* [20..20] PCIe（X1）控制器。当下面的条件都有效的时候，此信号有效（一个cycle的脉冲）MSI or MSI-X is enabled。Hot-Plug interrupts are enabled in the Slot Controlregister.Any bit in the Slot Status register transitions from 0 to 1and the associated event notification is enabled in theSlot Control register。 */
        unsigned int    pcie_pm_status             : 1; /* [21..21] PCIe1（X2）控制器。PMCSR的PME状态位。0：无效；1：有效。 */
        unsigned int    pcie_ref_clk_req_n         : 1; /* [22..22] PCIe1（X2）控制器。参考时钟移除请求信号。0：有效；1：无效。 */
        unsigned int    mac_phy_powerdown_p2_exit_reg : 1; /* [23..23] mac_phy_powerdown_p2_exit中断状态 */
        unsigned int    radm_msg_req_id_low        : 8; /* [31..24] radm_msg_req_id[7:0]； */
    } bits;
    unsigned int    u32;
}HI_SC_STAT70_T;    /* PCIE 控制器状态寄存器0 */

typedef union
{
    struct
    {
        unsigned int    axi_parity_errs_reg        : 4; /* [3..0] 指示当前PCIe控制器的AXI bridge的地址总线上检测到一个奇偶错误[3]：在client1_addr通道上检测到一个奇偶错误[2]：在radmx_addr通道上检测到一个奇偶错误[1]：在slv_req_addr通道上检测到一个奇偶错误[0]：在mstr_req_addr通道上检测到一个奇偶错误 */
        unsigned int    app_parity_errs_reg        : 3; /* [6..4] 指示当前PCIe控制器内部检测到一个奇偶错误[6]：在XTLH通道上检测到一个奇偶错误[5]：在XADM通道上检测到一个奇偶错误[4]：在RADM通道上检测到一个奇偶错误 */
        unsigned int    pm_linkst_in_l1            : 1; /* [7..7] 指示当前处于电源L1状态 */
        unsigned int    pm_linkst_in_l2            : 1; /* [8..8] 指示当前处于电源L2状态 */
        unsigned int    pm_linkst_l2_exit          : 1; /* [9..9] 指示电源正在退出L2状态 */
        unsigned int    mac_phy_power_down         : 2; /* [11..10] 指示PCIe控制器当前控制PHY的PM状态00：P0（L0，正常状态，全功耗状态）01：P0s（L0s，节电状态，恢复到正常状态时间短）10：P1（L1，更低节电状态，恢复到正常状态时间长）11：P2（L2，最低节电状态） */
        unsigned int    radm_correctabl_err_reg    : 1; /* [12..12] 指示PCIe控制器当前收到一个ERR_COR消息 */
        unsigned int    radm_nonfatal_err_reg      : 1; /* [13..13] 指示PCIe控制器当前收到一个ERR_NONFATAL消息 */
        unsigned int    radm_fatal_err_reg         : 1; /* [14..14] 指示PCIe控制器当前收到一个ERR_FATAL消息 */
        unsigned int    radm_pm_to_pme_reg         : 1; /* [15..15] 指示PCIe控制器当前收到一个PME_PM消息 */
        unsigned int    radm_pm_to_ack_reg         : 1; /* [16..16] 指示PCIe控制器当前收到一个PME_TO_ACK消息 */
        unsigned int    radm_cpl_timeout_reg       : 1; /* [17..17] 指示PCIe控制器当前检测到cpl_timeout */
        unsigned int    radm_msg_unlock_reg        : 1; /* [18..18] 指示PCIe控制器当前收到一个MSG_UNLOCK消息 */
        unsigned int    cfg_pme_msi_reg            : 1; /* [19..19] 当下面条件成立：MSI/MSI-X有效Root Control寄存器相应PME Enable bit有效Root Status寄存器相应PME bit有效 */
        unsigned int    bridge_flush_not_reg       : 1; /* [20..20] 指示PCIe控制器的处于graceful复位状态 */
        unsigned int    link_req_rst_not_reg       : 1; /* [21..21] 指示PCIe控制器检测到链路异常后，进入link down，申请自动复位状态 */
        unsigned int    pcie_cfg_aer_rc_err_msi    : 1; /* [22..22] PCIe（X1）控制器当下面的条件都满足的时候，有效这个信号一个时钟cycleMSI or MSI-X is enabled.A reported error condition causes a bit to be set in theRoot Error Status register.The associated error message reporting enable bit isset in the Root Error Command register. */
        unsigned int    pcie_cfg_sys_err_rc        : 1; /* [23..23] PCIe（X1）控制器。表明侦测到系统错误。一个时钟cycle的脉冲。0：无效；1：有效。 */
        unsigned int    radm_msg_req_id_high       : 8; /* [31..24] radm_msg_req_id[15:8]； */
    } bits;
    unsigned int    u32;
}HI_SC_STAT71_T;    /* PCIE 控制器状态寄存器1 */

typedef union
{
    struct
    {
        unsigned int    pcie_stat2                 : 32; /* [31..0] 该寄存器根据PCIE_CTRL8[6:5](pcie_stat2_sel)的不同值给出不同的状态信息。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT72_T;    /* PCIE 控制器状态寄存器2 */

typedef union
{
    struct
    {
        unsigned int    pcie_stat3                 : 32; /* [31..0] 该寄存器根据PCIE_CTRL8[8:7](pcie_stat3_sel)的不同值给出不同的状态信息。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT73_T;    /* PCIE 控制器状态寄存器3 */

typedef union
{
    struct
    {
        unsigned int    ltssm_state                : 6; /* [5..0] 指示当前PCIe控制器ltssm状态link up成功的状态是：0x11 */
        unsigned int    mac_phy_rate               : 2; /* [7..6] 指示当前PCIe控制器的速率00：2.5Gbps01:5.0Gbps10:8.0Gbps */
        unsigned int    pcie_slv_err_int           : 1; /* [8..8] PCIe控制器AXI slave错误中断 */
        unsigned int    trgt_lookup_empty          : 1; /* [9..9] TARGET查找表空指示 */
        unsigned int    trgt_cpl_timeout_reg       : 1; /* [10..10] TARGET完成包超时中断状态。 */
        unsigned int    reserved                   : 21; /* [31..11] 保留 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT74_T;    /* PCIE 控制器状态寄存器4 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 32; /* [31..0] PCIE PHY状态寄存器 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT75_T;    /* PCIE PHY状态寄存器 */

typedef union
{
    struct
    {
        unsigned int    obff_dec_idle_intr_status  : 1; /* [0..0] obff_dec_idle中断状态位。0:无中断；1:有中断。 */
        unsigned int    obff_dec_obff_intr_status  : 1; /* [1..1] obff_dec_obff中断状态位。0:无中断；1:有中断。 */
        unsigned int    obff_dec_cpu_active_intr_status : 1; /* [2..2] obff_dec_cpu_active中断状态位。0:无中断；1:有中断。 */
        unsigned int    obff_dec_err_intr_status   : 1; /* [3..3] obff_dec_err中断状态位。0:无中断；1:有中断。 */
        unsigned int    obff_msg_idle_intr_status  : 1; /* [4..4] obff_msg_idle中断状态位。0:无中断；1:有中断。 */
        unsigned int    obff_msg_obff_intr_status  : 1; /* [5..5] obff_msg_obff中断状态位。0:无中断；1:有中断。 */
        unsigned int    obff_msg_cpu_active_intr_status : 1; /* [6..6] obff_msg_cpu_active中断状态位。0:无中断；1:有中断。 */
        unsigned int    radm_msg_ltr_intr_status   : 1; /* [7..7] radm_msg_ltr中断状态位。0:无中断；1:有中断。 */
        unsigned int    cfg_msi_mask_chg_intr_status : 1; /* [8..8] cfg_msi_mask_chg中断状态位。0:无中断；1:有中断。 */
        unsigned int    radm_qoverflow_intr_status : 1; /* [9..9] radm_qoverflow中断状态位。0:无中断；1:有中断。 */
        unsigned int    amba_ordr_intr_status      : 1; /* [10..10] amba_ordr中断状态位。0:无中断；1:有中断。 */
        unsigned int    pcie_clkreq_in_n_intr_status : 1; /* [11..11] pcie_clkreq_in_n中断状态位。0:无中断；1:有中断。 */
        unsigned int    pcie_clkreq_in_n_inv_intr_status : 1; /* [12..12] pcie_clkreq_in_n_inv中断状态位。0:无中断；1:有中断。 */
        unsigned int    radm_vendor_msg_intr_status : 1; /* [13..13] radm_vendor_msg中断状态位。0:无中断；1:有中断。 */
        unsigned int    mac_phy_rxelecidle_disable : 1; /* [14..14] PIPE接口信号mac_phy_rxelecidle_disable实时状态。 */
        unsigned int    mac_phy_txcommonmode_disable : 1; /* [15..15] PIPE接口信号mac_phy_txcommonmode_disable实时状态。 */
        unsigned int    reserved                   : 16; /* [31..16] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT76_T;    /* PCIE 控制器状态寄存器5 */

typedef union
{
    struct
    {
        unsigned int    obff_dec_owrd_cur_state    : 4; /* [3..0] OBFF解码器接口信号obff_dec_owrd_cur_state实时状态。 */
        unsigned int    obff_enc_owre_cur_state    : 4; /* [7..4] OBFF编码器接口信号obff_enc_owre_cur_state实时状态。 */
        unsigned int    cfg_obff_en                : 2; /* [9..8] DM接口信号cfg_obff_en实时状态。 */
        unsigned int    cfg_ltr_m_en               : 1; /* [10..10] DM接口信号cfg_ltr_m_en实时状态。 */
        unsigned int    cfg_pwr_budget_sel         : 1; /* [11..11] DM接口信号cfg_pwr_budget_sel实时状态。 */
        unsigned int    smlh_in_rl0s               : 1; /* [12..12] DM接口信号smlh_in_rl0s实时状态。 */
        unsigned int    radm_qoverflow             : 1; /* [13..13] DM接口信号radm_qoverflow实时状态。 */
        unsigned int    amba_ordr_mgr_wdt_int      : 1; /* [14..14] DM接口信号amba_ordr_mgr_wdt_int实时状态。 */
        unsigned int    reserved                   : 17; /* [31..15] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT77_T;    /* PCIE 控制器状态寄存器6 */

typedef union
{
    struct
    {
        unsigned int    cfg_msi_mask               : 32; /* [31..0] DM接口信号cfg_msi_mask实时状态。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT78_T;    /* PCIE 控制器状态寄存器7 */

typedef union
{
    struct
    {
        unsigned int    cfg_ltr_max_latency        : 32; /* [31..0] DM接口信号cfg_ltr_max_latency实时状态。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT79_T;    /* PCIE 控制器状态寄存器8 */

typedef union
{
    struct
    {
        unsigned int    cm3_brch_stat              : 4; /* [3..0] branch state。 */
        unsigned int    cm3_core_halted            : 1; /* [4..4] m3 core 处于debug状态。 */
        unsigned int    cm3_lockup                 : 1; /* [5..5] m3 core 处于lockup状态。 */
        unsigned int    reserved_1                 : 2; /* [7..6] 保留。 */
        unsigned int    cm3_cur_int_priority       : 8; /* [15..8] nvic current int priority。 */
        unsigned int    reserved_0                 : 16; /* [31..16] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT80_T;    /* m3状态寄存器0。 */

typedef union
{
    struct
    {
        unsigned int    m3_stat1                   : 32; /* [31..0] m3内部状态寄存器1。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT81_T;    /* m3状态寄存器1。 */

typedef union
{
    struct
    {
        unsigned int    m3_stat2                   : 32; /* [31..0] m3内部状态寄存器2。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT82_T;    /* m3状态寄存器2。 */

typedef union
{
    struct
    {
        unsigned int    m3_stat3                   : 32; /* [31..0] m3内部状态寄存器3。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT83_T;    /* m3状态寄存器3。 */

typedef union
{
    struct
    {
        unsigned int    m3_stat4                   : 32; /* [31..0] m3内部状态寄存器4。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT84_T;    /* m3状态寄存器4。 */

typedef union
{
    struct
    {
        unsigned int    m3_stat5                   : 21; /* [20..0] m3内部状态寄存器5。 */
        unsigned int    reserved                   : 11; /* [31..21] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT85_T;    /* m3状态寄存器5。 */

typedef union
{
    struct
    {
        unsigned int    efuse_tsensor_trim         : 32; /* [31..0]  */
    } bits;
    unsigned int    u32;
}HI_SC_STAT86_T;    /* tsensor trim寄存器 */

typedef union
{
    struct
    {
        unsigned int    app_ltr_latency            : 32; /* [31..0] DM接口信号app_ltr_latency实时状态。 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT87_T;    /* PCIE 控制器状态寄存器9 */

typedef union
{
    struct
    {
        unsigned int    version_id                 : 32; /* [31..0] 芯片版本寄存器。 */
    } bits;
    unsigned int    u32;
}HI_VERSION_ID_T;    /* 芯片版本寄存器。 */


/********************************************************************************/
/*    syssc_ao 函数（项目名_模块名_寄存器名_成员名_set)        */
/********************************************************************************/
HI_SET_GET(hi_sc_ctrl0_remap_clear,remap_clear,HI_SC_CTRL0_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL0_OFFSET)
HI_SET_GET(hi_sc_ctrl0_m3_remap_clear,m3_remap_clear,HI_SC_CTRL0_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL0_OFFSET)
HI_SET_GET(hi_sc_ctrl0_reserved_1,reserved_1,HI_SC_CTRL0_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL0_OFFSET)
HI_SET_GET(hi_sc_ctrl0_remap_stat,remap_stat,HI_SC_CTRL0_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL0_OFFSET)
HI_SET_GET(hi_sc_ctrl0_cm3_remap_stat,cm3_remap_stat,HI_SC_CTRL0_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL0_OFFSET)
HI_SET_GET(hi_sc_ctrl0_reserved_0,reserved_0,HI_SC_CTRL0_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL0_OFFSET)
HI_SET_GET(hi_sc_ctrl1_pmu_rstout_n,pmu_rstout_n,HI_SC_CTRL1_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL1_OFFSET)
HI_SET_GET(hi_sc_ctrl1_pmu_hold,pmu_hold,HI_SC_CTRL1_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL1_OFFSET)
HI_SET_GET(hi_sc_ctrl1_pmu_irq_mask,pmu_irq_mask,HI_SC_CTRL1_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL1_OFFSET)
HI_SET_GET(hi_sc_ctrl1_pmu_irq_n_pos_sel,pmu_irq_n_pos_sel,HI_SC_CTRL1_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL1_OFFSET)
HI_SET_GET(hi_sc_ctrl1_efuse_sys_ctrl_en,efuse_sys_ctrl_en,HI_SC_CTRL1_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL1_OFFSET)
HI_SET_GET(hi_sc_ctrl1_wdt_reset_sel,wdt_reset_sel,HI_SC_CTRL1_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL1_OFFSET)
HI_SET_GET(hi_sc_ctrl1_reserved_1,reserved_1,HI_SC_CTRL1_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL1_OFFSET)
HI_SET_GET(hi_sc_ctrl1_usbphy_tcxo_buff_ctrl,usbphy_tcxo_buff_ctrl,HI_SC_CTRL1_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL1_OFFSET)
HI_SET_GET(hi_sc_ctrl1_reserved_0,reserved_0,HI_SC_CTRL1_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL1_OFFSET)
HI_SET_GET(hi_sc_ctrl2_wdt_clk_en,wdt_clk_en,HI_SC_CTRL2_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL2_OFFSET)
HI_SET_GET(hi_sc_ctrl2_reserved_1,reserved_1,HI_SC_CTRL2_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL2_OFFSET)
HI_SET_GET(hi_sc_ctrl2_wdt_en_ov,wdt_en_ov,HI_SC_CTRL2_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL2_OFFSET)
HI_SET_GET(hi_sc_ctrl2_reserved_0,reserved_0,HI_SC_CTRL2_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL2_OFFSET)
HI_SET_GET(hi_sc_ctrl2_wdt_en_ctrl,wdt_en_ctrl,HI_SC_CTRL2_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL2_OFFSET)
HI_SET_GET(hi_sc_ctrl3_lcd_rst_n,lcd_rst_n,HI_SC_CTRL3_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL3_OFFSET)
HI_SET_GET(hi_sc_ctrl3_emi_sel,emi_sel,HI_SC_CTRL3_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL3_OFFSET)
HI_SET_GET(hi_sc_ctrl3_zsi_en,zsi_en,HI_SC_CTRL3_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL3_OFFSET)
HI_SET_GET(hi_sc_ctrl3_sio_master_mode,sio_master_mode,HI_SC_CTRL3_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL3_OFFSET)
HI_SET_GET(hi_sc_ctrl3_mmc0_sample_tuning_enable,mmc0_sample_tuning_enable,HI_SC_CTRL3_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL3_OFFSET)
HI_SET_GET(hi_sc_ctrl3_mmc1_sample_tuning_enable,mmc1_sample_tuning_enable,HI_SC_CTRL3_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL3_OFFSET)
HI_SET_GET(hi_sc_ctrl3_mmc_msc,mmc_msc,HI_SC_CTRL3_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL3_OFFSET)
HI_SET_GET(hi_sc_ctrl3_ssp0_mode,ssp0_mode,HI_SC_CTRL3_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL3_OFFSET)
HI_SET_GET(hi_sc_ctrl3_ssp1_mode,ssp1_mode,HI_SC_CTRL3_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL3_OFFSET)
HI_SET_GET(hi_sc_ctrl3_amon_cpufast_monitor_start,amon_cpufast_monitor_start,HI_SC_CTRL3_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL3_OFFSET)
HI_SET_GET(hi_sc_ctrl3_amon_soc_monitor_start,amon_soc_monitor_start,HI_SC_CTRL3_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL3_OFFSET)
HI_SET_GET(hi_sc_ctrl3_pmussi0_mst_cnt,pmussi0_mst_cnt,HI_SC_CTRL3_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL3_OFFSET)
HI_SET_GET(hi_sc_ctrl3_reserved_2,reserved_2,HI_SC_CTRL3_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL3_OFFSET)
HI_SET_GET(hi_sc_ctrl3_dsp0_uart_en,dsp0_uart_en,HI_SC_CTRL3_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL3_OFFSET)
HI_SET_GET(hi_sc_ctrl3_hifi_uart_en,hifi_uart_en,HI_SC_CTRL3_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL3_OFFSET)
HI_SET_GET(hi_sc_ctrl3_reserved_1,reserved_1,HI_SC_CTRL3_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL3_OFFSET)
HI_SET_GET(hi_sc_ctrl3_reserved_0,reserved_0,HI_SC_CTRL3_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL3_OFFSET)
HI_SET_GET(hi_sc_ctrl3_test_clk_en,test_clk_en,HI_SC_CTRL3_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL3_OFFSET)
HI_SET_GET(hi_sc_ctrl4_cicom0_sel_mod,cicom0_sel_mod,HI_SC_CTRL4_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL4_OFFSET)
HI_SET_GET(hi_sc_ctrl4_cicom1_sel_mod,cicom1_sel_mod,HI_SC_CTRL4_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL4_OFFSET)
HI_SET_GET(hi_sc_ctrl4_reserved,reserved,HI_SC_CTRL4_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL4_OFFSET)
HI_SET_GET(hi_sc_ctrl5_reserved,reserved,HI_SC_CTRL5_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL5_OFFSET)
HI_SET_GET(hi_sc_ctrl5_sw_appa9_boot_addr,sw_appa9_boot_addr,HI_SC_CTRL5_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL5_OFFSET)
HI_SET_GET(hi_sc_ctrl6_reserved,reserved,HI_SC_CTRL6_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL6_OFFSET)
HI_SET_GET(hi_sc_ctrl6_sw_modema9_boot_addr,sw_modema9_boot_addr,HI_SC_CTRL6_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL6_OFFSET)
HI_SET_GET(hi_sc_ctrl7_appa9_remap_size,appa9_remap_size,HI_SC_CTRL7_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL7_OFFSET)
HI_SET_GET(hi_sc_ctrl7_mdma9_remap_size,mdma9_remap_size,HI_SC_CTRL7_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL7_OFFSET)
HI_SET_GET(hi_sc_ctrl7_reserved,reserved,HI_SC_CTRL7_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL7_OFFSET)
HI_SET_GET(hi_sc_ctrl8_reserved,reserved,HI_SC_CTRL8_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL8_OFFSET)
HI_SET_GET(hi_sc_ctrl8_sw_hifi_boot_addr,sw_hifi_boot_addr,HI_SC_CTRL8_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL8_OFFSET)
HI_SET_GET(hi_sc_ctrl9_reserved,reserved,HI_SC_CTRL9_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL9_OFFSET)
HI_SET_GET(hi_sc_ctrl9_sw_dsp0_boot_addr,sw_dsp0_boot_addr,HI_SC_CTRL9_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL9_OFFSET)
HI_SET_GET(hi_sc_ctrl10_reserved_3,reserved_3,HI_SC_CTRL10_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL10_OFFSET)
HI_SET_GET(hi_sc_ctrl10_appa9_cfgnmfi,appa9_cfgnmfi,HI_SC_CTRL10_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL10_OFFSET)
HI_SET_GET(hi_sc_ctrl10_appa9_cfgsdisable,appa9_cfgsdisable,HI_SC_CTRL10_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL10_OFFSET)
HI_SET_GET(hi_sc_ctrl10_appa9_cp15sdisable,appa9_cp15sdisable,HI_SC_CTRL10_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL10_OFFSET)
HI_SET_GET(hi_sc_ctrl10_appa9_pwrctli0,appa9_pwrctli0,HI_SC_CTRL10_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL10_OFFSET)
HI_SET_GET(hi_sc_ctrl10_appa9_spiden,appa9_spiden,HI_SC_CTRL10_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL10_OFFSET)
HI_SET_GET(hi_sc_ctrl10_appa9_spniden,appa9_spniden,HI_SC_CTRL10_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL10_OFFSET)
HI_SET_GET(hi_sc_ctrl10_appa9_niden,appa9_niden,HI_SC_CTRL10_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL10_OFFSET)
HI_SET_GET(hi_sc_ctrl10_appa9_dbgen,appa9_dbgen,HI_SC_CTRL10_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL10_OFFSET)
HI_SET_GET(hi_sc_ctrl10_appa9_eventi_cfg,appa9_eventi_cfg,HI_SC_CTRL10_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL10_OFFSET)
HI_SET_GET(hi_sc_ctrl10_reserved_2,reserved_2,HI_SC_CTRL10_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL10_OFFSET)
HI_SET_GET(hi_sc_ctrl10_appa9_l2_spniden,appa9_l2_spniden,HI_SC_CTRL10_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL10_OFFSET)
HI_SET_GET(hi_sc_ctrl10_reserved_1,reserved_1,HI_SC_CTRL10_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL10_OFFSET)
HI_SET_GET(hi_sc_ctrl10_appa9_l2_waysize,appa9_l2_waysize,HI_SC_CTRL10_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL10_OFFSET)
HI_SET_GET(hi_sc_ctrl10_reserved_0,reserved_0,HI_SC_CTRL10_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL10_OFFSET)
HI_SET_GET(hi_sc_ctrl10_appa9_l2_regfilebase,appa9_l2_regfilebase,HI_SC_CTRL10_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL10_OFFSET)
HI_SET_GET(hi_sc_ctrl11_reserved_3,reserved_3,HI_SC_CTRL11_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL11_OFFSET)
HI_SET_GET(hi_sc_ctrl11_moda9_cfgnmfi,moda9_cfgnmfi,HI_SC_CTRL11_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL11_OFFSET)
HI_SET_GET(hi_sc_ctrl11_moda9_cfgsdisable,moda9_cfgsdisable,HI_SC_CTRL11_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL11_OFFSET)
HI_SET_GET(hi_sc_ctrl11_moda9_cp15sdisable,moda9_cp15sdisable,HI_SC_CTRL11_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL11_OFFSET)
HI_SET_GET(hi_sc_ctrl11_moda9_pwrctli0,moda9_pwrctli0,HI_SC_CTRL11_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL11_OFFSET)
HI_SET_GET(hi_sc_ctrl11_moda9_spiden,moda9_spiden,HI_SC_CTRL11_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL11_OFFSET)
HI_SET_GET(hi_sc_ctrl11_moda9_spniden,moda9_spniden,HI_SC_CTRL11_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL11_OFFSET)
HI_SET_GET(hi_sc_ctrl11_moda9_niden,moda9_niden,HI_SC_CTRL11_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL11_OFFSET)
HI_SET_GET(hi_sc_ctrl11_moda9_dbgen,moda9_dbgen,HI_SC_CTRL11_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL11_OFFSET)
HI_SET_GET(hi_sc_ctrl11_moda9_eventi_cfg,moda9_eventi_cfg,HI_SC_CTRL11_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL11_OFFSET)
HI_SET_GET(hi_sc_ctrl11_reserved_2,reserved_2,HI_SC_CTRL11_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL11_OFFSET)
HI_SET_GET(hi_sc_ctrl11_moda9_l2_spniden,moda9_l2_spniden,HI_SC_CTRL11_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL11_OFFSET)
HI_SET_GET(hi_sc_ctrl11_reserved_1,reserved_1,HI_SC_CTRL11_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL11_OFFSET)
HI_SET_GET(hi_sc_ctrl11_moda9_l2_waysize,moda9_l2_waysize,HI_SC_CTRL11_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL11_OFFSET)
HI_SET_GET(hi_sc_ctrl11_reserved_0,reserved_0,HI_SC_CTRL11_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL11_OFFSET)
HI_SET_GET(hi_sc_ctrl11_moda9_l2_regfilebase,moda9_l2_regfilebase,HI_SC_CTRL11_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL11_OFFSET)
HI_SET_GET(hi_sc_ctrl12_hifi_ocdhaltonreset,hifi_ocdhaltonreset,HI_SC_CTRL12_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL12_OFFSET)
HI_SET_GET(hi_sc_ctrl12_hifi_runstall,hifi_runstall,HI_SC_CTRL12_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL12_OFFSET)
HI_SET_GET(hi_sc_ctrl12_hifi_statvectorsel,hifi_statvectorsel,HI_SC_CTRL12_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL12_OFFSET)
HI_SET_GET(hi_sc_ctrl12_hifi_breaksync_en,hifi_breaksync_en,HI_SC_CTRL12_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL12_OFFSET)
HI_SET_GET(hi_sc_ctrl12_hifi_crosstrig_en,hifi_crosstrig_en,HI_SC_CTRL12_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL12_OFFSET)
HI_SET_GET(hi_sc_ctrl12_reserved_1,reserved_1,HI_SC_CTRL12_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL12_OFFSET)
HI_SET_GET(hi_sc_ctrl12_reserved_0,reserved_0,HI_SC_CTRL12_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL12_OFFSET)
HI_SET_GET(hi_sc_ctrl13_reserved_1,reserved_1,HI_SC_CTRL13_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL13_OFFSET)
HI_SET_GET(hi_sc_ctrl13_dsp0_ocdhaltonreset,dsp0_ocdhaltonreset,HI_SC_CTRL13_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL13_OFFSET)
HI_SET_GET(hi_sc_ctrl13_dsp0_runstall,dsp0_runstall,HI_SC_CTRL13_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL13_OFFSET)
HI_SET_GET(hi_sc_ctrl13_dsp0_statvectorsel,dsp0_statvectorsel,HI_SC_CTRL13_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL13_OFFSET)
HI_SET_GET(hi_sc_ctrl13_dsp0_breaksync_en,dsp0_breaksync_en,HI_SC_CTRL13_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL13_OFFSET)
HI_SET_GET(hi_sc_ctrl13_dsp0_crosstrig_en,dsp0_crosstrig_en,HI_SC_CTRL13_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL13_OFFSET)
HI_SET_GET(hi_sc_ctrl13_reserved_0,reserved_0,HI_SC_CTRL13_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL13_OFFSET)
HI_SET_GET(hi_sc_ctrl14_reserved,reserved,HI_SC_CTRL14_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL14_OFFSET)
HI_SET_GET(hi_sc_ctrl15_cm3_fix_mst_type,cm3_fix_mst_type,HI_SC_CTRL15_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL15_OFFSET)
HI_SET_GET(hi_sc_ctrl15_cm3_dap_en,cm3_dap_en,HI_SC_CTRL15_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL15_OFFSET)
HI_SET_GET(hi_sc_ctrl15_cm3_ts_clk_chg,cm3_ts_clk_chg,HI_SC_CTRL15_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL15_OFFSET)
HI_SET_GET(hi_sc_ctrl15_cm3_mpu_disable,cm3_mpu_disable,HI_SC_CTRL15_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL15_OFFSET)
HI_SET_GET(hi_sc_ctrl15_cm3_dbgen,cm3_dbgen,HI_SC_CTRL15_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL15_OFFSET)
HI_SET_GET(hi_sc_ctrl15_cm3_eventi_cfg,cm3_eventi_cfg,HI_SC_CTRL15_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL15_OFFSET)
HI_SET_GET(hi_sc_ctrl15_reserved,reserved,HI_SC_CTRL15_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL15_OFFSET)
HI_SET_GET(hi_sc_ctrl16_cm3_stcalib,cm3_stcalib,HI_SC_CTRL16_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL16_OFFSET)
HI_SET_GET(hi_sc_ctrl16_reserved,reserved,HI_SC_CTRL16_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL16_OFFSET)
HI_SET_GET(hi_sc_ctrl17_hpm0_clk_div,hpm0_clk_div,HI_SC_CTRL17_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL17_OFFSET)
HI_SET_GET(hi_sc_ctrl17_reserved_3,reserved_3,HI_SC_CTRL17_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL17_OFFSET)
HI_SET_GET(hi_sc_ctrl17_hpm0_en,hpm0_en,HI_SC_CTRL17_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL17_OFFSET)
HI_SET_GET(hi_sc_ctrl17_hpm1_clk_div,hpm1_clk_div,HI_SC_CTRL17_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL17_OFFSET)
HI_SET_GET(hi_sc_ctrl17_reserved_2,reserved_2,HI_SC_CTRL17_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL17_OFFSET)
HI_SET_GET(hi_sc_ctrl17_hpm1_en,hpm1_en,HI_SC_CTRL17_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL17_OFFSET)
HI_SET_GET(hi_sc_ctrl17_hpm2_clk_div,hpm2_clk_div,HI_SC_CTRL17_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL17_OFFSET)
HI_SET_GET(hi_sc_ctrl17_reserved_1,reserved_1,HI_SC_CTRL17_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL17_OFFSET)
HI_SET_GET(hi_sc_ctrl17_hpm2_en,hpm2_en,HI_SC_CTRL17_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL17_OFFSET)
HI_SET_GET(hi_sc_ctrl17_reserved_0,reserved_0,HI_SC_CTRL17_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL17_OFFSET)
HI_SET_GET(hi_sc_ctrl18_cs_event_hwe,cs_event_hwe,HI_SC_CTRL18_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL18_OFFSET)
HI_SET_GET(hi_sc_ctrl19_reserved,reserved,HI_SC_CTRL19_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL19_OFFSET)
HI_SET_GET(hi_sc_ctrl20_axi_mem_ret_gatedclock_en,axi_mem_ret_gatedclock_en,HI_SC_CTRL20_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL20_OFFSET)
HI_SET_GET(hi_sc_ctrl20_cicom0_auto_clk_gate_en,cicom0_auto_clk_gate_en,HI_SC_CTRL20_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL20_OFFSET)
HI_SET_GET(hi_sc_ctrl20_cicom0_soft_gate_clk_en,cicom0_soft_gate_clk_en,HI_SC_CTRL20_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL20_OFFSET)
HI_SET_GET(hi_sc_ctrl20_cicom1_auto_clk_gate_en,cicom1_auto_clk_gate_en,HI_SC_CTRL20_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL20_OFFSET)
HI_SET_GET(hi_sc_ctrl20_cicom1_soft_gate_clk_en,cicom1_soft_gate_clk_en,HI_SC_CTRL20_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL20_OFFSET)
HI_SET_GET(hi_sc_ctrl20_hs_uart_gatedclock_en,hs_uart_gatedclock_en,HI_SC_CTRL20_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL20_OFFSET)
HI_SET_GET(hi_sc_ctrl20_uart_gatedclock_en,uart_gatedclock_en,HI_SC_CTRL20_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL20_OFFSET)
HI_SET_GET(hi_sc_ctrl20_sdcc_hclk_wkup_en,sdcc_hclk_wkup_en,HI_SC_CTRL20_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL20_OFFSET)
HI_SET_GET(hi_sc_ctrl20_sdcc_hclk_soft_en,sdcc_hclk_soft_en,HI_SC_CTRL20_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL20_OFFSET)
HI_SET_GET(hi_sc_ctrl20_dw_ssi_gatedclock_en,dw_ssi_gatedclock_en,HI_SC_CTRL20_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL20_OFFSET)
HI_SET_GET(hi_sc_ctrl20_usb3_gatedclock_en,usb3_gatedclock_en,HI_SC_CTRL20_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL20_OFFSET)
HI_SET_GET(hi_sc_ctrl20_hdlc_soft_gate_clk_en,hdlc_soft_gate_clk_en,HI_SC_CTRL20_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL20_OFFSET)
HI_SET_GET(hi_sc_ctrl20_reseved,reseved,HI_SC_CTRL20_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL20_OFFSET)
HI_SET_GET(hi_sc_ctrl20_cs_cg_en,cs_cg_en,HI_SC_CTRL20_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL20_OFFSET)
HI_SET_GET(hi_sc_ctrl20_reserved_1,reserved_1,HI_SC_CTRL20_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL20_OFFSET)
HI_SET_GET(hi_sc_ctrl20_hsic_gatedclock_en,hsic_gatedclock_en,HI_SC_CTRL20_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL20_OFFSET)
HI_SET_GET(hi_sc_ctrl20_hsic_ss_scaledown_mode,hsic_ss_scaledown_mode,HI_SC_CTRL20_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL20_OFFSET)
HI_SET_GET(hi_sc_ctrl20_upacc_auto_clk_gate_en,upacc_auto_clk_gate_en,HI_SC_CTRL20_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL20_OFFSET)
HI_SET_GET(hi_sc_ctrl20_upacc_soft_gate_clk_en,upacc_soft_gate_clk_en,HI_SC_CTRL20_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL20_OFFSET)
HI_SET_GET(hi_sc_ctrl20_reserved_0,reserved_0,HI_SC_CTRL20_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL20_OFFSET)
HI_SET_GET(hi_sc_ctrl20_ios_gatedclock_en,ios_gatedclock_en,HI_SC_CTRL20_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL20_OFFSET)
HI_SET_GET(hi_sc_ctrl20_ipcm_auto_clk_gate_en,ipcm_auto_clk_gate_en,HI_SC_CTRL20_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL20_OFFSET)
HI_SET_GET(hi_sc_ctrl20_ipcm_soft_gate_clk_en,ipcm_soft_gate_clk_en,HI_SC_CTRL20_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL20_OFFSET)
HI_SET_GET(hi_sc_ctrl20_hdlc_auto_clk_gate_en,hdlc_auto_clk_gate_en,HI_SC_CTRL20_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL20_OFFSET)
HI_SET_GET(hi_sc_ctrl20_sysctrl_cg_en,sysctrl_cg_en,HI_SC_CTRL20_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL20_OFFSET)
HI_SET_GET(hi_sc_ctrl21_reserved_4,reserved_4,HI_SC_CTRL21_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL21_OFFSET)
HI_SET_GET(hi_sc_ctrl21_sdcc_gatedclock_en,sdcc_gatedclock_en,HI_SC_CTRL21_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL21_OFFSET)
HI_SET_GET(hi_sc_ctrl21_timer_gatedclock_en,timer_gatedclock_en,HI_SC_CTRL21_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL21_OFFSET)
HI_SET_GET(hi_sc_ctrl21_reserved_3,reserved_3,HI_SC_CTRL21_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL21_OFFSET)
HI_SET_GET(hi_sc_ctrl21_reserved_2,reserved_2,HI_SC_CTRL21_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL21_OFFSET)
HI_SET_GET(hi_sc_ctrl21_mddrc_ddrphy_ac_pdd,mddrc_ddrphy_ac_pdd,HI_SC_CTRL21_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL21_OFFSET)
HI_SET_GET(hi_sc_ctrl21_ddrc_apb_gt_en,ddrc_apb_gt_en,HI_SC_CTRL21_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL21_OFFSET)
HI_SET_GET(hi_sc_ctrl21_dw_axi_cpu_cg_en,dw_axi_cpu_cg_en,HI_SC_CTRL21_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL21_OFFSET)
HI_SET_GET(hi_sc_ctrl21_dw_axi_glb_cg_en,dw_axi_glb_cg_en,HI_SC_CTRL21_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL21_OFFSET)
HI_SET_GET(hi_sc_ctrl21_dw_axi_mst_cg_en,dw_axi_mst_cg_en,HI_SC_CTRL21_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL21_OFFSET)
HI_SET_GET(hi_sc_ctrl21_dw_axi_amon_gatedclock_en,dw_axi_amon_gatedclock_en,HI_SC_CTRL21_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL21_OFFSET)
HI_SET_GET(hi_sc_ctrl21_dw_axi_bbphy_cg_en,dw_axi_bbphy_cg_en,HI_SC_CTRL21_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL21_OFFSET)
HI_SET_GET(hi_sc_ctrl21_dw_rs_cg_en,dw_rs_cg_en,HI_SC_CTRL21_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL21_OFFSET)
HI_SET_GET(hi_sc_ctrl21_dw_x2x_async_cg_en,dw_x2x_async_cg_en,HI_SC_CTRL21_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL21_OFFSET)
HI_SET_GET(hi_sc_ctrl21_dw_x2x_qsync_cg_en,dw_x2x_qsync_cg_en,HI_SC_CTRL21_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL21_OFFSET)
HI_SET_GET(hi_sc_ctrl21_dw_x2h_qsync_cg_en,dw_x2h_qsync_cg_en,HI_SC_CTRL21_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL21_OFFSET)
HI_SET_GET(hi_sc_ctrl21_dw_hmx_cg_en,dw_hmx_cg_en,HI_SC_CTRL21_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL21_OFFSET)
HI_SET_GET(hi_sc_ctrl21_dw_x2p_cg_en,dw_x2p_cg_en,HI_SC_CTRL21_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL21_OFFSET)
HI_SET_GET(hi_sc_ctrl21_dw_gs_cg_en,dw_gs_cg_en,HI_SC_CTRL21_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL21_OFFSET)
HI_SET_GET(hi_sc_ctrl21_ashb_gatedclock_en,ashb_gatedclock_en,HI_SC_CTRL21_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL21_OFFSET)
HI_SET_GET(hi_sc_ctrl21_dw_ahb_mst_gatedclock_en,dw_ahb_mst_gatedclock_en,HI_SC_CTRL21_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL21_OFFSET)
HI_SET_GET(hi_sc_ctrl21_axi_guacc_gatedclock_en,axi_guacc_gatedclock_en,HI_SC_CTRL21_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL21_OFFSET)
HI_SET_GET(hi_sc_ctrl21_edmac_autogated_clk_en,edmac_autogated_clk_en,HI_SC_CTRL21_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL21_OFFSET)
HI_SET_GET(hi_sc_ctrl21_reserved_1,reserved_1,HI_SC_CTRL21_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL21_OFFSET)
HI_SET_GET(hi_sc_ctrl21_reserved_0,reserved_0,HI_SC_CTRL21_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL21_OFFSET)
HI_SET_GET(hi_sc_ctrl22_hifi_ema,hifi_ema,HI_SC_CTRL22_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL22_OFFSET)
HI_SET_GET(hi_sc_ctrl22_hifi_emaw,hifi_emaw,HI_SC_CTRL22_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL22_OFFSET)
HI_SET_GET(hi_sc_ctrl22_hifi_emas,hifi_emas,HI_SC_CTRL22_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL22_OFFSET)
HI_SET_GET(hi_sc_ctrl22_reserved_1,reserved_1,HI_SC_CTRL22_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL22_OFFSET)
HI_SET_GET(hi_sc_ctrl22_dsp0_ema,dsp0_ema,HI_SC_CTRL22_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL22_OFFSET)
HI_SET_GET(hi_sc_ctrl22_dsp0_emaw,dsp0_emaw,HI_SC_CTRL22_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL22_OFFSET)
HI_SET_GET(hi_sc_ctrl22_dsp0_emas,dsp0_emas,HI_SC_CTRL22_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL22_OFFSET)
HI_SET_GET(hi_sc_ctrl22_reserved_0,reserved_0,HI_SC_CTRL22_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL22_OFFSET)
HI_SET_GET(hi_sc_ctrl23_axi_mem_ema,axi_mem_ema,HI_SC_CTRL23_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL23_OFFSET)
HI_SET_GET(hi_sc_ctrl23_axi_mem_emaw,axi_mem_emaw,HI_SC_CTRL23_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL23_OFFSET)
HI_SET_GET(hi_sc_ctrl23_axi_mem_emas,axi_mem_emas,HI_SC_CTRL23_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL23_OFFSET)
HI_SET_GET(hi_sc_ctrl23_reserved_2,reserved_2,HI_SC_CTRL23_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL23_OFFSET)
HI_SET_GET(hi_sc_ctrl23_soc_mem_ema,soc_mem_ema,HI_SC_CTRL23_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL23_OFFSET)
HI_SET_GET(hi_sc_ctrl23_soc_mem_spram_emaw,soc_mem_spram_emaw,HI_SC_CTRL23_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL23_OFFSET)
HI_SET_GET(hi_sc_ctrl23_soc_mem_spram_emas,soc_mem_spram_emas,HI_SC_CTRL23_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL23_OFFSET)
HI_SET_GET(hi_sc_ctrl23_soc_mem_tpram_emab,soc_mem_tpram_emab,HI_SC_CTRL23_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL23_OFFSET)
HI_SET_GET(hi_sc_ctrl23_soc_mem_tpram_emasa,soc_mem_tpram_emasa,HI_SC_CTRL23_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL23_OFFSET)
HI_SET_GET(hi_sc_ctrl23_soc_mem_dpram_emaw,soc_mem_dpram_emaw,HI_SC_CTRL23_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL23_OFFSET)
HI_SET_GET(hi_sc_ctrl23_soc_mem_dpram_emas,soc_mem_dpram_emas,HI_SC_CTRL23_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL23_OFFSET)
HI_SET_GET(hi_sc_ctrl23_reserved_1,reserved_1,HI_SC_CTRL23_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL23_OFFSET)
HI_SET_GET(hi_sc_ctrl23_bootrom_ema,bootrom_ema,HI_SC_CTRL23_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL23_OFFSET)
HI_SET_GET(hi_sc_ctrl23_bootrom_pgen,bootrom_pgen,HI_SC_CTRL23_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL23_OFFSET)
HI_SET_GET(hi_sc_ctrl23_bootrom_ken,bootrom_ken,HI_SC_CTRL23_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL23_OFFSET)
HI_SET_GET(hi_sc_ctrl23_reserved_0,reserved_0,HI_SC_CTRL23_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL23_OFFSET)
HI_SET_GET(hi_sc_ctrl24_appa9_hs_mem_adjust,appa9_hs_mem_adjust,HI_SC_CTRL24_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL24_OFFSET)
HI_SET_GET(hi_sc_ctrl24_moda9_hs_mem_adjust,moda9_hs_mem_adjust,HI_SC_CTRL24_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL24_OFFSET)
HI_SET_GET(hi_sc_ctrl24_appa9_l2_ema,appa9_l2_ema,HI_SC_CTRL24_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL24_OFFSET)
HI_SET_GET(hi_sc_ctrl24_appa9_l2_emaw,appa9_l2_emaw,HI_SC_CTRL24_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL24_OFFSET)
HI_SET_GET(hi_sc_ctrl24_appa9_l2_emas,appa9_l2_emas,HI_SC_CTRL24_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL24_OFFSET)
HI_SET_GET(hi_sc_ctrl24_moda9_l2_ema,moda9_l2_ema,HI_SC_CTRL24_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL24_OFFSET)
HI_SET_GET(hi_sc_ctrl24_moda9_l2_emaw,moda9_l2_emaw,HI_SC_CTRL24_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL24_OFFSET)
HI_SET_GET(hi_sc_ctrl24_moda9_l2_emas,moda9_l2_emas,HI_SC_CTRL24_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL24_OFFSET)
HI_SET_GET(hi_sc_ctrl25_test_sys0_sel,test_sys0_sel,HI_SC_CTRL25_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL25_OFFSET)
HI_SET_GET(hi_sc_ctrl25_test_sys1_sel,test_sys1_sel,HI_SC_CTRL25_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL25_OFFSET)
HI_SET_GET(hi_sc_ctrl25_reserved,reserved,HI_SC_CTRL25_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL25_OFFSET)
HI_SET_GET(hi_sc_ctrl26_test_sys0_bypass,test_sys0_bypass,HI_SC_CTRL26_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL26_OFFSET)
HI_SET_GET(hi_sc_ctrl27_cs_spiden,cs_spiden,HI_SC_CTRL27_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL27_OFFSET)
HI_SET_GET(hi_sc_ctrl27_cs_spniden,cs_spniden,HI_SC_CTRL27_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL27_OFFSET)
HI_SET_GET(hi_sc_ctrl27_cs_dbgen,cs_dbgen,HI_SC_CTRL27_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL27_OFFSET)
HI_SET_GET(hi_sc_ctrl27_cs_niden,cs_niden,HI_SC_CTRL27_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL27_OFFSET)
HI_SET_GET(hi_sc_ctrl27_cs_tpctl,cs_tpctl,HI_SC_CTRL27_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL27_OFFSET)
HI_SET_GET(hi_sc_ctrl27_cs_deviceen,cs_deviceen,HI_SC_CTRL27_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL27_OFFSET)
HI_SET_GET(hi_sc_ctrl27_reserved_1,reserved_1,HI_SC_CTRL27_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL27_OFFSET)
HI_SET_GET(hi_sc_ctrl27_cs_tpmaxdataasize,cs_tpmaxdataasize,HI_SC_CTRL27_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL27_OFFSET)
HI_SET_GET(hi_sc_ctrl27_cs_nsguaren,cs_nsguaren,HI_SC_CTRL27_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL27_OFFSET)
HI_SET_GET(hi_sc_ctrl27_cs_tsmaxwidth,cs_tsmaxwidth,HI_SC_CTRL27_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL27_OFFSET)
HI_SET_GET(hi_sc_ctrl27_cs_tsnatural,cs_tsnatural,HI_SC_CTRL27_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL27_OFFSET)
HI_SET_GET(hi_sc_ctrl27_cs_timestamp_en,cs_timestamp_en,HI_SC_CTRL27_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL27_OFFSET)
HI_SET_GET(hi_sc_ctrl27_reserved_0,reserved_0,HI_SC_CTRL27_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL27_OFFSET)
HI_SET_GET(hi_sc_ctrl28_reserved,reserved,HI_SC_CTRL28_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL28_OFFSET)
HI_SET_GET(hi_sc_ctrl28_filter_start,filter_start,HI_SC_CTRL28_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL28_OFFSET)
HI_SET_GET(hi_sc_ctrl29_reserved,reserved,HI_SC_CTRL29_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL29_OFFSET)
HI_SET_GET(hi_sc_ctrl29_filter_end,filter_end,HI_SC_CTRL29_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL29_OFFSET)
HI_SET_GET(hi_sc_ctrl30_sci0_clk_od_en,sci0_clk_od_en,HI_SC_CTRL30_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL30_OFFSET)
HI_SET_GET(hi_sc_ctrl30_sci0_data_od_en,sci0_data_od_en,HI_SC_CTRL30_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL30_OFFSET)
HI_SET_GET(hi_sc_ctrl30_reserved_1,reserved_1,HI_SC_CTRL30_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL30_OFFSET)
HI_SET_GET(hi_sc_ctrl30_sci1_clk_od_en,sci1_clk_od_en,HI_SC_CTRL30_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL30_OFFSET)
HI_SET_GET(hi_sc_ctrl30_sci1_data_od_en,sci1_data_od_en,HI_SC_CTRL30_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL30_OFFSET)
HI_SET_GET(hi_sc_ctrl30_reserved_0,reserved_0,HI_SC_CTRL30_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL30_OFFSET)
HI_SET_GET(hi_sc_ctrl31_reserved,reserved,HI_SC_CTRL31_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL31_OFFSET)
HI_SET_GET(hi_sc_ctrl32_ebi_normal_mode,ebi_normal_mode,HI_SC_CTRL32_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL32_OFFSET)
HI_SET_GET(hi_sc_ctrl32_reserved_2,reserved_2,HI_SC_CTRL32_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL32_OFFSET)
HI_SET_GET(hi_sc_ctrl32_ebi_timeoutvalue1,ebi_timeoutvalue1,HI_SC_CTRL32_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL32_OFFSET)
HI_SET_GET(hi_sc_ctrl32_reserved_1,reserved_1,HI_SC_CTRL32_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL32_OFFSET)
HI_SET_GET(hi_sc_ctrl32_ebi_timeoutvalue2,ebi_timeoutvalue2,HI_SC_CTRL32_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL32_OFFSET)
HI_SET_GET(hi_sc_ctrl32_reserved_0,reserved_0,HI_SC_CTRL32_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL32_OFFSET)
HI_SET_GET(hi_sc_ctrl33_reserved_2,reserved_2,HI_SC_CTRL33_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL33_OFFSET)
HI_SET_GET(hi_sc_ctrl33_func_ddr_testmode,func_ddr_testmode,HI_SC_CTRL33_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL33_OFFSET)
HI_SET_GET(hi_sc_ctrl33_reserved_1,reserved_1,HI_SC_CTRL33_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL33_OFFSET)
HI_SET_GET(hi_sc_ctrl33_reserved_0,reserved_0,HI_SC_CTRL33_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL33_OFFSET)
HI_SET_GET(hi_sc_ctrl34_refclk_div,refclk_div,HI_SC_CTRL34_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL34_OFFSET)
HI_SET_GET(hi_sc_ctrl34_txrputune,txrputune,HI_SC_CTRL34_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL34_OFFSET)
HI_SET_GET(hi_sc_ctrl34_txrpdtune,txrpdtune,HI_SC_CTRL34_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL34_OFFSET)
HI_SET_GET(hi_sc_ctrl34_txsrtune,txsrtune,HI_SC_CTRL34_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL34_OFFSET)
HI_SET_GET(hi_sc_ctrl34_loopbackenb,loopbackenb,HI_SC_CTRL34_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL34_OFFSET)
HI_SET_GET(hi_sc_ctrl34_siddq,siddq,HI_SC_CTRL34_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL34_OFFSET)
HI_SET_GET(hi_sc_ctrl34_txbitstuffen,txbitstuffen,HI_SC_CTRL34_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL34_OFFSET)
HI_SET_GET(hi_sc_ctrl34_txbitstuffenh,txbitstuffenh,HI_SC_CTRL34_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL34_OFFSET)
HI_SET_GET(hi_sc_ctrl34_hsic_portreset,hsic_portreset,HI_SC_CTRL34_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL34_OFFSET)
HI_SET_GET(hi_sc_ctrl34_reserved_1,reserved_1,HI_SC_CTRL34_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL34_OFFSET)
HI_SET_GET(hi_sc_ctrl34_dmpulldown,dmpulldown,HI_SC_CTRL34_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL34_OFFSET)
HI_SET_GET(hi_sc_ctrl34_dppulldown,dppulldown,HI_SC_CTRL34_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL34_OFFSET)
HI_SET_GET(hi_sc_ctrl34_reserved_0,reserved_0,HI_SC_CTRL34_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL34_OFFSET)
HI_SET_GET(hi_sc_ctrl34_utmi_hostdisconnect,utmi_hostdisconnect,HI_SC_CTRL34_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL34_OFFSET)
HI_SET_GET(hi_sc_ctrl34_utmiotg_iddig,utmiotg_iddig,HI_SC_CTRL34_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL34_OFFSET)
HI_SET_GET(hi_sc_ctrl35_test_addr,test_addr,HI_SC_CTRL35_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL35_OFFSET)
HI_SET_GET(hi_sc_ctrl35_test_datain,test_datain,HI_SC_CTRL35_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL35_OFFSET)
HI_SET_GET(hi_sc_ctrl35_test_clk,test_clk,HI_SC_CTRL35_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL35_OFFSET)
HI_SET_GET(hi_sc_ctrl35_test_dataoutsel,test_dataoutsel,HI_SC_CTRL35_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL35_OFFSET)
HI_SET_GET(hi_sc_ctrl35_reserved,reserved,HI_SC_CTRL35_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL35_OFFSET)
HI_SET_GET(hi_sc_ctrl35_test_dataout,test_dataout,HI_SC_CTRL35_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL35_OFFSET)
HI_SET_GET(hi_sc_ctrl36_fsel,fsel,HI_SC_CTRL36_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL36_OFFSET)
HI_SET_GET(hi_sc_ctrl36_refclk_sel,refclk_sel,HI_SC_CTRL36_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL36_OFFSET)
HI_SET_GET(hi_sc_ctrl36_common_on_n,common_on_n,HI_SC_CTRL36_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL36_OFFSET)
HI_SET_GET(hi_sc_ctrl36_port_reset,port_reset,HI_SC_CTRL36_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL36_OFFSET)
HI_SET_GET(hi_sc_ctrl36_vatestenb,vatestenb,HI_SC_CTRL36_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL36_OFFSET)
HI_SET_GET(hi_sc_ctrl36_txvreftune,txvreftune,HI_SC_CTRL36_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL36_OFFSET)
HI_SET_GET(hi_sc_ctrl36_txfslstune,txfslstune,HI_SC_CTRL36_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL36_OFFSET)
HI_SET_GET(hi_sc_ctrl36_txhsxvtune,txhsxvtune,HI_SC_CTRL36_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL36_OFFSET)
HI_SET_GET(hi_sc_ctrl36_otgtune,otgtune,HI_SC_CTRL36_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL36_OFFSET)
HI_SET_GET(hi_sc_ctrl36_compdistune,compdistune,HI_SC_CTRL36_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL36_OFFSET)
HI_SET_GET(hi_sc_ctrl36_sqrxtune,sqrxtune,HI_SC_CTRL36_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL36_OFFSET)
HI_SET_GET(hi_sc_ctrl36_txrisetune,txrisetune,HI_SC_CTRL36_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL36_OFFSET)
HI_SET_GET(hi_sc_ctrl36_txpreempamptune,txpreempamptune,HI_SC_CTRL36_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL36_OFFSET)
HI_SET_GET(hi_sc_ctrl37_test_addr,test_addr,HI_SC_CTRL37_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL37_OFFSET)
HI_SET_GET(hi_sc_ctrl37_test_clk,test_clk,HI_SC_CTRL37_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL37_OFFSET)
HI_SET_GET(hi_sc_ctrl37_test_dataout_sel,test_dataout_sel,HI_SC_CTRL37_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL37_OFFSET)
HI_SET_GET(hi_sc_ctrl37_phy_clk_sel,phy_clk_sel,HI_SC_CTRL37_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL37_OFFSET)
HI_SET_GET(hi_sc_ctrl37_phy_id_sel,phy_id_sel,HI_SC_CTRL37_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL37_OFFSET)
HI_SET_GET(hi_sc_ctrl37_test_datain,test_datain,HI_SC_CTRL37_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL37_OFFSET)
HI_SET_GET(hi_sc_ctrl37_test_dataout,test_dataout,HI_SC_CTRL37_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL37_OFFSET)
HI_SET_GET(hi_sc_ctrl37_acaenb,acaenb,HI_SC_CTRL37_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL37_OFFSET)
HI_SET_GET(hi_sc_ctrl37_retenablen,retenablen,HI_SC_CTRL37_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL37_OFFSET)
HI_SET_GET(hi_sc_ctrl37_reserved,reserved,HI_SC_CTRL37_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL37_OFFSET)
HI_SET_GET(hi_sc_ctrl37_txpreemppulsetune,txpreemppulsetune,HI_SC_CTRL37_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL37_OFFSET)
HI_SET_GET(hi_sc_ctrl37_otgdisable,otgdisable,HI_SC_CTRL37_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL37_OFFSET)
HI_SET_GET(hi_sc_ctrl37_loopbackenb,loopbackenb,HI_SC_CTRL37_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL37_OFFSET)
HI_SET_GET(hi_sc_ctrl37_vbusvldextsel,vbusvldextsel,HI_SC_CTRL37_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL37_OFFSET)
HI_SET_GET(hi_sc_ctrl37_vbusvldext,vbusvldext,HI_SC_CTRL37_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL37_OFFSET)
HI_SET_GET(hi_sc_ctrl37_txrestune,txrestune,HI_SC_CTRL37_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL37_OFFSET)
HI_SET_GET(hi_sc_ctrl37_siddq,siddq,HI_SC_CTRL37_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL37_OFFSET)
HI_SET_GET(hi_sc_ctrl38_compdistune,compdistune,HI_SC_CTRL38_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL38_OFFSET)
HI_SET_GET(hi_sc_ctrl38_otgtune,otgtune,HI_SC_CTRL38_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL38_OFFSET)
HI_SET_GET(hi_sc_ctrl38_sqrxtune,sqrxtune,HI_SC_CTRL38_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL38_OFFSET)
HI_SET_GET(hi_sc_ctrl38_txfslstune,txfslstune,HI_SC_CTRL38_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL38_OFFSET)
HI_SET_GET(hi_sc_ctrl38_txhsxvtune,txhsxvtune,HI_SC_CTRL38_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL38_OFFSET)
HI_SET_GET(hi_sc_ctrl38_txpreempamptune,txpreempamptune,HI_SC_CTRL38_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL38_OFFSET)
HI_SET_GET(hi_sc_ctrl38_txpreemppulsetune,txpreemppulsetune,HI_SC_CTRL38_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL38_OFFSET)
HI_SET_GET(hi_sc_ctrl38_txrestune,txrestune,HI_SC_CTRL38_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL38_OFFSET)
HI_SET_GET(hi_sc_ctrl38_txrisetune,txrisetune,HI_SC_CTRL38_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL38_OFFSET)
HI_SET_GET(hi_sc_ctrl38_txvreftune,txvreftune,HI_SC_CTRL38_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL38_OFFSET)
HI_SET_GET(hi_sc_ctrl38_los_bias,los_bias,HI_SC_CTRL38_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL38_OFFSET)
HI_SET_GET(hi_sc_ctrl38_los_level2_0,los_level2_0,HI_SC_CTRL38_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL38_OFFSET)
HI_SET_GET(hi_sc_ctrl39_los_level4_3,los_level4_3,HI_SC_CTRL39_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL39_OFFSET)
HI_SET_GET(hi_sc_ctrl39_pcs_tx_deemph_3p5db,pcs_tx_deemph_3p5db,HI_SC_CTRL39_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL39_OFFSET)
HI_SET_GET(hi_sc_ctrl39_pcs_tx_deemph_6db,pcs_tx_deemph_6db,HI_SC_CTRL39_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL39_OFFSET)
HI_SET_GET(hi_sc_ctrl39_pcs_tx_swing_full,pcs_tx_swing_full,HI_SC_CTRL39_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL39_OFFSET)
HI_SET_GET(hi_sc_ctrl39_lane0_tx_term_offset,lane0_tx_term_offset,HI_SC_CTRL39_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL39_OFFSET)
HI_SET_GET(hi_sc_ctrl39_tx_vboost_lvl,tx_vboost_lvl,HI_SC_CTRL39_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL39_OFFSET)
HI_SET_GET(hi_sc_ctrl39_reserved,reserved,HI_SC_CTRL39_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL39_OFFSET)
HI_SET_GET(hi_sc_ctrl40_cr_data_in,cr_data_in,HI_SC_CTRL40_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL40_OFFSET)
HI_SET_GET(hi_sc_ctrl40_cr_cap_addr,cr_cap_addr,HI_SC_CTRL40_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL40_OFFSET)
HI_SET_GET(hi_sc_ctrl40_cr_cap_data,cr_cap_data,HI_SC_CTRL40_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL40_OFFSET)
HI_SET_GET(hi_sc_ctrl40_cr_write,cr_write,HI_SC_CTRL40_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL40_OFFSET)
HI_SET_GET(hi_sc_ctrl40_cr_read,cr_read,HI_SC_CTRL40_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL40_OFFSET)
HI_SET_GET(hi_sc_ctrl40_rtune_req,rtune_req,HI_SC_CTRL40_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL40_OFFSET)
HI_SET_GET(hi_sc_ctrl40_reserved,reserved,HI_SC_CTRL40_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL40_OFFSET)
HI_SET_GET(hi_sc_ctrl41_usb3_bus_filter_bypass,usb3_bus_filter_bypass,HI_SC_CTRL41_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL41_OFFSET)
HI_SET_GET(hi_sc_ctrl41_usb3_fladj_30mhz_reg,usb3_fladj_30mhz_reg,HI_SC_CTRL41_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL41_OFFSET)
HI_SET_GET(hi_sc_ctrl41_usb3_host_u2_port_disable,usb3_host_u2_port_disable,HI_SC_CTRL41_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL41_OFFSET)
HI_SET_GET(hi_sc_ctrl41_usb3_host_u3_port_disable,usb3_host_u3_port_disable,HI_SC_CTRL41_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL41_OFFSET)
HI_SET_GET(hi_sc_ctrl41_usb3_host_port_pwrctl,usb3_host_port_pwrctl,HI_SC_CTRL41_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL41_OFFSET)
HI_SET_GET(hi_sc_ctrl41_usb3_host_msi_enable,usb3_host_msi_enable,HI_SC_CTRL41_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL41_OFFSET)
HI_SET_GET(hi_sc_ctrl41_usb3_pm_power_state_request,usb3_pm_power_state_request,HI_SC_CTRL41_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL41_OFFSET)
HI_SET_GET(hi_sc_ctrl41_usb3_xhci_revision,usb3_xhci_revision,HI_SC_CTRL41_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL41_OFFSET)
HI_SET_GET(hi_sc_ctrl41_usb3_xhc_bme,usb3_xhc_bme,HI_SC_CTRL41_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL41_OFFSET)
HI_SET_GET(hi_sc_ctrl41_reserved_1,reserved_1,HI_SC_CTRL41_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL41_OFFSET)
HI_SET_GET(hi_sc_ctrl41_reserved_0,reserved_0,HI_SC_CTRL41_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL41_OFFSET)
HI_SET_GET(hi_sc_ctrl41_usb3_pmu_iddig_override,usb3_pmu_iddig_override,HI_SC_CTRL41_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL41_OFFSET)
HI_SET_GET(hi_sc_ctrl41_usb3_pmu_avalid_override,usb3_pmu_avalid_override,HI_SC_CTRL41_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL41_OFFSET)
HI_SET_GET(hi_sc_ctrl41_usb3_pmu_bvalid_override,usb3_pmu_bvalid_override,HI_SC_CTRL41_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL41_OFFSET)
HI_SET_GET(hi_sc_ctrl41_usb3_pmu_vbusvalid_override,usb3_pmu_vbusvalid_override,HI_SC_CTRL41_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL41_OFFSET)
HI_SET_GET(hi_sc_ctrl41_usb3_pmu_sessend_override,usb3_pmu_sessend_override,HI_SC_CTRL41_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL41_OFFSET)
HI_SET_GET(hi_sc_ctrl41_usb3_pmu_powerpresent_override,usb3_pmu_powerpresent_override,HI_SC_CTRL41_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL41_OFFSET)
HI_SET_GET(hi_sc_ctrl41_usb3_pmu_iddig_override_en,usb3_pmu_iddig_override_en,HI_SC_CTRL41_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL41_OFFSET)
HI_SET_GET(hi_sc_ctrl41_usb3_pmu_avalid_override_en,usb3_pmu_avalid_override_en,HI_SC_CTRL41_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL41_OFFSET)
HI_SET_GET(hi_sc_ctrl41_usb3_pmu_bvalid_override_en,usb3_pmu_bvalid_override_en,HI_SC_CTRL41_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL41_OFFSET)
HI_SET_GET(hi_sc_ctrl41_usb3_pmu_vbusvalid_override_en,usb3_pmu_vbusvalid_override_en,HI_SC_CTRL41_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL41_OFFSET)
HI_SET_GET(hi_sc_ctrl41_usb3_pmu_sessend_override_en,usb3_pmu_sessend_override_en,HI_SC_CTRL41_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL41_OFFSET)
HI_SET_GET(hi_sc_ctrl41_usb3_pmu_powerpresent_override_en,usb3_pmu_powerpresent_override_en,HI_SC_CTRL41_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL41_OFFSET)
HI_SET_GET(hi_sc_ctrl42_usb3_pmu_pm_pmu_config_strap,usb3_pmu_pm_pmu_config_strap,HI_SC_CTRL42_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL42_OFFSET)
HI_SET_GET(hi_sc_ctrl42_usb3_pmu_port_overcurrent,usb3_pmu_port_overcurrent,HI_SC_CTRL42_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL42_OFFSET)
HI_SET_GET(hi_sc_ctrl43_usb3_hub_port_perm_attach,usb3_hub_port_perm_attach,HI_SC_CTRL43_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL43_OFFSET)
HI_SET_GET(hi_sc_ctrl43_usb3_pme_en,usb3_pme_en,HI_SC_CTRL43_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL43_OFFSET)
HI_SET_GET(hi_sc_ctrl43_reserved,reserved,HI_SC_CTRL43_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL43_OFFSET)
HI_SET_GET(hi_sc_ctrl44_reserved,reserved,HI_SC_CTRL44_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL44_OFFSET)
HI_SET_GET(hi_sc_ctrl45_uicc_ic_usb_vbusvalid,uicc_ic_usb_vbusvalid,HI_SC_CTRL45_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL45_OFFSET)
HI_SET_GET(hi_sc_ctrl45_reserved,reserved,HI_SC_CTRL45_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL45_OFFSET)
HI_SET_GET(hi_sc_ctrl45_intr_merge0_en,intr_merge0_en,HI_SC_CTRL45_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL45_OFFSET)
HI_SET_GET(hi_sc_ctrl45_intr_merge0_mode,intr_merge0_mode,HI_SC_CTRL45_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL45_OFFSET)
HI_SET_GET(hi_sc_ctrl45_intr_merge1_en,intr_merge1_en,HI_SC_CTRL45_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL45_OFFSET)
HI_SET_GET(hi_sc_ctrl45_intr_merge1_mode,intr_merge1_mode,HI_SC_CTRL45_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL45_OFFSET)
HI_SET_GET(hi_sc_ctrl45_intr_merge2_en,intr_merge2_en,HI_SC_CTRL45_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL45_OFFSET)
HI_SET_GET(hi_sc_ctrl45_intr_merge2_mode,intr_merge2_mode,HI_SC_CTRL45_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL45_OFFSET)
HI_SET_GET(hi_sc_ctrl45_intr_merge3_en,intr_merge3_en,HI_SC_CTRL45_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL45_OFFSET)
HI_SET_GET(hi_sc_ctrl45_intr_merge3_mode,intr_merge3_mode,HI_SC_CTRL45_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL45_OFFSET)
HI_SET_GET(hi_sc_ctrl45_intr_merge0_sel,intr_merge0_sel,HI_SC_CTRL45_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL45_OFFSET)
HI_SET_GET(hi_sc_ctrl45_intr_merge1_sel,intr_merge1_sel,HI_SC_CTRL45_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL45_OFFSET)
HI_SET_GET(hi_sc_ctrl45_intr_merge2_sel,intr_merge2_sel,HI_SC_CTRL45_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL45_OFFSET)
HI_SET_GET(hi_sc_ctrl45_intr_merge3_sel,intr_merge3_sel,HI_SC_CTRL45_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL45_OFFSET)
HI_SET_GET(hi_sc_ctrl46_intr_merge0_timer0,intr_merge0_timer0,HI_SC_CTRL46_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL46_OFFSET)
HI_SET_GET(hi_sc_ctrl46_intr_merge0_timer1,intr_merge0_timer1,HI_SC_CTRL46_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL46_OFFSET)
HI_SET_GET(hi_sc_ctrl47_int2_merge0_timer2,int2_merge0_timer2,HI_SC_CTRL47_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL47_OFFSET)
HI_SET_GET(hi_sc_ctrl47_intr_merge0_timer3,intr_merge0_timer3,HI_SC_CTRL47_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL47_OFFSET)
HI_SET_GET(hi_sc_ctrl48_sysctrl_intr_appa9,sysctrl_intr_appa9,HI_SC_CTRL48_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL48_OFFSET)
HI_SET_GET(hi_sc_ctrl48_sysctrl_intr_mdma9,sysctrl_intr_mdma9,HI_SC_CTRL48_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL48_OFFSET)
HI_SET_GET(hi_sc_ctrl48_sysctrl_intr_cm3,sysctrl_intr_cm3,HI_SC_CTRL48_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL48_OFFSET)
HI_SET_GET(hi_sc_ctrl48_reserved,reserved,HI_SC_CTRL48_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL48_OFFSET)
HI_SET_GET(hi_sc_ctrl49_sysctrl_intr_appa9,sysctrl_intr_appa9,HI_SC_CTRL49_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL49_OFFSET)
HI_SET_GET(hi_sc_ctrl49_sysctrl_intr_mdma9,sysctrl_intr_mdma9,HI_SC_CTRL49_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL49_OFFSET)
HI_SET_GET(hi_sc_ctrl49_sysctrl_intr_cm3,sysctrl_intr_cm3,HI_SC_CTRL49_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL49_OFFSET)
HI_SET_GET(hi_sc_ctrl49_reserved,reserved,HI_SC_CTRL49_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL49_OFFSET)
HI_SET_GET(hi_sc_ctrl50_sysctrl_intr_appa9,sysctrl_intr_appa9,HI_SC_CTRL50_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL50_OFFSET)
HI_SET_GET(hi_sc_ctrl50_sysctrl_intr_mdma9,sysctrl_intr_mdma9,HI_SC_CTRL50_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL50_OFFSET)
HI_SET_GET(hi_sc_ctrl50_sysctrl_intr_cm3,sysctrl_intr_cm3,HI_SC_CTRL50_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL50_OFFSET)
HI_SET_GET(hi_sc_ctrl50_reserved,reserved,HI_SC_CTRL50_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL50_OFFSET)
HI_SET_GET(hi_sc_ctrl51_cm3_nmi,cm3_nmi,HI_SC_CTRL51_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL51_OFFSET)
HI_SET_GET(hi_sc_ctrl51_reserved,reserved,HI_SC_CTRL51_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL51_OFFSET)
HI_SET_GET(hi_sc_ctrl52_dsp0_nmi,dsp0_nmi,HI_SC_CTRL52_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL52_OFFSET)
HI_SET_GET(hi_sc_ctrl52_reserved,reserved,HI_SC_CTRL52_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL52_OFFSET)
HI_SET_GET(hi_sc_ctrl53_hifi_nmi,hifi_nmi,HI_SC_CTRL53_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL53_OFFSET)
HI_SET_GET(hi_sc_ctrl53_reserved,reserved,HI_SC_CTRL53_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL53_OFFSET)
HI_SET_GET(hi_sc_ctrl54_dw_ssi0_dma_sel,dw_ssi0_dma_sel,HI_SC_CTRL54_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL54_OFFSET)
HI_SET_GET(hi_sc_ctrl54_dw_ssi1_dma_sel,dw_ssi1_dma_sel,HI_SC_CTRL54_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL54_OFFSET)
HI_SET_GET(hi_sc_ctrl54_sio_dma_sel,sio_dma_sel,HI_SC_CTRL54_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL54_OFFSET)
HI_SET_GET(hi_sc_ctrl54_hs_uart_dma_sel,hs_uart_dma_sel,HI_SC_CTRL54_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL54_OFFSET)
HI_SET_GET(hi_sc_ctrl54_uart0_dma_sel,uart0_dma_sel,HI_SC_CTRL54_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL54_OFFSET)
HI_SET_GET(hi_sc_ctrl54_uart1_dma_sel,uart1_dma_sel,HI_SC_CTRL54_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL54_OFFSET)
HI_SET_GET(hi_sc_ctrl54_uart2_dma_sel,uart2_dma_sel,HI_SC_CTRL54_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL54_OFFSET)
HI_SET_GET(hi_sc_ctrl54_sci0_dma_sel,sci0_dma_sel,HI_SC_CTRL54_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL54_OFFSET)
HI_SET_GET(hi_sc_ctrl54_sci1_dma_sel,sci1_dma_sel,HI_SC_CTRL54_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL54_OFFSET)
HI_SET_GET(hi_sc_ctrl54_bbp_dbg_dma_sel,bbp_dbg_dma_sel,HI_SC_CTRL54_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL54_OFFSET)
HI_SET_GET(hi_sc_ctrl54_bbp_grif_dma_sel,bbp_grif_dma_sel,HI_SC_CTRL54_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL54_OFFSET)
HI_SET_GET(hi_sc_ctrl54_amon_soc_dma_sel,amon_soc_dma_sel,HI_SC_CTRL54_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL54_OFFSET)
HI_SET_GET(hi_sc_ctrl54_amon_fast_dma_sel,amon_fast_dma_sel,HI_SC_CTRL54_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL54_OFFSET)
HI_SET_GET(hi_sc_ctrl54_i2c0_dma_sel,i2c0_dma_sel,HI_SC_CTRL54_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL54_OFFSET)
HI_SET_GET(hi_sc_ctrl54_i2c1_dma_sel,i2c1_dma_sel,HI_SC_CTRL54_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL54_OFFSET)
HI_SET_GET(hi_sc_ctrl54_reserved,reserved,HI_SC_CTRL54_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL54_OFFSET)
HI_SET_GET(hi_sc_ctrl55_reserved_1,reserved_1,HI_SC_CTRL55_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL55_OFFSET)
HI_SET_GET(hi_sc_ctrl55_cs_ret1n,cs_ret1n,HI_SC_CTRL55_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL55_OFFSET)
HI_SET_GET(hi_sc_ctrl55_gmac_ret1n,gmac_ret1n,HI_SC_CTRL55_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL55_OFFSET)
HI_SET_GET(hi_sc_ctrl55_usb3_ret1n,usb3_ret1n,HI_SC_CTRL55_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL55_OFFSET)
HI_SET_GET(hi_sc_ctrl55_reserved_0,reserved_0,HI_SC_CTRL55_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL55_OFFSET)
HI_SET_GET(hi_sc_ctrl55_uicc_ret1n,uicc_ret1n,HI_SC_CTRL55_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL55_OFFSET)
HI_SET_GET(hi_sc_ctrl55_nandc_ret1n,nandc_ret1n,HI_SC_CTRL55_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL55_OFFSET)
HI_SET_GET(hi_sc_ctrl55_sdcc_ret1n,sdcc_ret1n,HI_SC_CTRL55_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL55_OFFSET)
HI_SET_GET(hi_sc_ctrl55_hsic_ret1n,hsic_ret1n,HI_SC_CTRL55_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL55_OFFSET)
HI_SET_GET(hi_sc_ctrl55_amon_soc_ret1n,amon_soc_ret1n,HI_SC_CTRL55_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL55_OFFSET)
HI_SET_GET(hi_sc_ctrl55_amon_cpufast_ret1n,amon_cpufast_ret1n,HI_SC_CTRL55_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL55_OFFSET)
HI_SET_GET(hi_sc_ctrl55_pcie_colldisn,pcie_colldisn,HI_SC_CTRL55_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL55_OFFSET)
HI_SET_GET(hi_sc_ctrl55_mmc_colldisn,mmc_colldisn,HI_SC_CTRL55_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL55_OFFSET)
HI_SET_GET(hi_sc_ctrl55_edmac_colldisn,edmac_colldisn,HI_SC_CTRL55_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL55_OFFSET)
HI_SET_GET(hi_sc_ctrl55_ssdma_colldisn,ssdma_colldisn,HI_SC_CTRL55_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL55_OFFSET)
HI_SET_GET(hi_sc_ctrl55_sdcc_colldisn,sdcc_colldisn,HI_SC_CTRL55_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL55_OFFSET)
HI_SET_GET(hi_sc_ctrl55_socp_colldisn,socp_colldisn,HI_SC_CTRL55_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL55_OFFSET)
HI_SET_GET(hi_sc_ctrl55_amon_colldisn,amon_colldisn,HI_SC_CTRL55_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL55_OFFSET)
HI_SET_GET(hi_sc_ctrl55_gmac_colldisn,gmac_colldisn,HI_SC_CTRL55_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL55_OFFSET)
HI_SET_GET(hi_sc_ctrl55_ipf_ret1n,ipf_ret1n,HI_SC_CTRL55_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL55_OFFSET)
HI_SET_GET(hi_sc_ctrl55_ipf_colldisn,ipf_colldisn,HI_SC_CTRL55_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL55_OFFSET)
HI_SET_GET(hi_sc_ctrl55_cipher_ret1n,cipher_ret1n,HI_SC_CTRL55_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL55_OFFSET)
HI_SET_GET(hi_sc_ctrl55_cipher_colldisn,cipher_colldisn,HI_SC_CTRL55_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL55_OFFSET)
HI_SET_GET(hi_sc_ctrl55_socp_ret1n,socp_ret1n,HI_SC_CTRL55_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL55_OFFSET)
HI_SET_GET(hi_sc_ctrl55_mipi_ret1n,mipi_ret1n,HI_SC_CTRL55_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL55_OFFSET)
HI_SET_GET(hi_sc_ctrl55_hs_uart_ret1n,hs_uart_ret1n,HI_SC_CTRL55_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL55_OFFSET)
HI_SET_GET(hi_sc_ctrl55_mmc0_ret1n,mmc0_ret1n,HI_SC_CTRL55_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL55_OFFSET)
HI_SET_GET(hi_sc_ctrl55_mmc1_ret1n,mmc1_ret1n,HI_SC_CTRL55_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL55_OFFSET)
HI_SET_GET(hi_sc_ctrl55_pcie_ret1n,pcie_ret1n,HI_SC_CTRL55_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL55_OFFSET)
HI_SET_GET(hi_sc_ctrl55_edmac_ret1n,edmac_ret1n,HI_SC_CTRL55_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL55_OFFSET)
HI_SET_GET(hi_sc_ctrl55_ssdma_ret1n,ssdma_ret1n,HI_SC_CTRL55_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL55_OFFSET)
HI_SET_GET(hi_sc_ctrl55_mipi_colldisn,mipi_colldisn,HI_SC_CTRL55_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL55_OFFSET)
HI_SET_GET(hi_sc_ctrl56_prior_level,prior_level,HI_SC_CTRL56_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL56_OFFSET)
HI_SET_GET(hi_sc_ctrl56_gatedclock_en,gatedclock_en,HI_SC_CTRL56_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL56_OFFSET)
HI_SET_GET(hi_sc_ctrl56_dw_axi_rs_gatedclock_en,dw_axi_rs_gatedclock_en,HI_SC_CTRL56_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL56_OFFSET)
HI_SET_GET(hi_sc_ctrl56_dw_axi_gs_gatedclock_en,dw_axi_gs_gatedclock_en,HI_SC_CTRL56_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL56_OFFSET)
HI_SET_GET(hi_sc_ctrl56_overf_prot,overf_prot,HI_SC_CTRL56_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL56_OFFSET)
HI_SET_GET(hi_sc_ctrl56_reserved,reserved,HI_SC_CTRL56_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL56_OFFSET)
HI_SET_GET(hi_sc_ctrl56_atpram_ctrl,atpram_ctrl,HI_SC_CTRL56_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL56_OFFSET)
HI_SET_GET(hi_sc_ctrl57_soc2lte_tbd,soc2lte_tbd,HI_SC_CTRL57_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL57_OFFSET)
HI_SET_GET(hi_sc_ctrl57_reserved,reserved,HI_SC_CTRL57_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL57_OFFSET)
HI_SET_GET(hi_sc_ctrl68_moda9_l2_ret1n,moda9_l2_ret1n,HI_SC_CTRL68_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL68_OFFSET)
HI_SET_GET(hi_sc_ctrl68_moda9_l2_ret2n,moda9_l2_ret2n,HI_SC_CTRL68_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL68_OFFSET)
HI_SET_GET(hi_sc_ctrl68_moda9_l2_pgen,moda9_l2_pgen,HI_SC_CTRL68_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL68_OFFSET)
HI_SET_GET(hi_sc_ctrl68_reserved_5,reserved_5,HI_SC_CTRL68_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL68_OFFSET)
HI_SET_GET(hi_sc_ctrl68_appa9_l2_ret1n,appa9_l2_ret1n,HI_SC_CTRL68_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL68_OFFSET)
HI_SET_GET(hi_sc_ctrl68_appa9_l2_ret2n,appa9_l2_ret2n,HI_SC_CTRL68_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL68_OFFSET)
HI_SET_GET(hi_sc_ctrl68_appa9_l2_pgen,appa9_l2_pgen,HI_SC_CTRL68_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL68_OFFSET)
HI_SET_GET(hi_sc_ctrl68_reserved_4,reserved_4,HI_SC_CTRL68_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL68_OFFSET)
HI_SET_GET(hi_sc_ctrl68_dsp0_ret1n,dsp0_ret1n,HI_SC_CTRL68_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL68_OFFSET)
HI_SET_GET(hi_sc_ctrl68_dsp0_ret2n,dsp0_ret2n,HI_SC_CTRL68_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL68_OFFSET)
HI_SET_GET(hi_sc_ctrl68_dsp0_pgen,dsp0_pgen,HI_SC_CTRL68_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL68_OFFSET)
HI_SET_GET(hi_sc_ctrl68_reserved_3,reserved_3,HI_SC_CTRL68_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL68_OFFSET)
HI_SET_GET(hi_sc_ctrl68_hifi_ret1n,hifi_ret1n,HI_SC_CTRL68_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL68_OFFSET)
HI_SET_GET(hi_sc_ctrl68_hifi_ret2n,hifi_ret2n,HI_SC_CTRL68_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL68_OFFSET)
HI_SET_GET(hi_sc_ctrl68_hifi_pgen,hifi_pgen,HI_SC_CTRL68_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL68_OFFSET)
HI_SET_GET(hi_sc_ctrl68_reserved_2,reserved_2,HI_SC_CTRL68_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL68_OFFSET)
HI_SET_GET(hi_sc_ctrl68_pd_axi_mem_ret1n,pd_axi_mem_ret1n,HI_SC_CTRL68_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL68_OFFSET)
HI_SET_GET(hi_sc_ctrl68_pd_axi_mem_ret2n,pd_axi_mem_ret2n,HI_SC_CTRL68_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL68_OFFSET)
HI_SET_GET(hi_sc_ctrl68_pd_axi_mem_pgen,pd_axi_mem_pgen,HI_SC_CTRL68_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL68_OFFSET)
HI_SET_GET(hi_sc_ctrl68_reserved_1,reserved_1,HI_SC_CTRL68_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL68_OFFSET)
HI_SET_GET(hi_sc_ctrl68_ret_axi_mem_ret1n,ret_axi_mem_ret1n,HI_SC_CTRL68_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL68_OFFSET)
HI_SET_GET(hi_sc_ctrl68_ret_axi_mem_ret2n,ret_axi_mem_ret2n,HI_SC_CTRL68_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL68_OFFSET)
HI_SET_GET(hi_sc_ctrl68_ret_axi_mem_pgen,ret_axi_mem_pgen,HI_SC_CTRL68_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL68_OFFSET)
HI_SET_GET(hi_sc_ctrl68_reserved_0,reserved_0,HI_SC_CTRL68_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL68_OFFSET)
HI_SET_GET(hi_sc_ctrl69_axi_bbphy_xdcdr_sel,axi_bbphy_xdcdr_sel,HI_SC_CTRL69_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL69_OFFSET)
HI_SET_GET(hi_sc_ctrl69_axi_amon_xdcdr_sel,axi_amon_xdcdr_sel,HI_SC_CTRL69_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL69_OFFSET)
HI_SET_GET(hi_sc_ctrl69_axi_cpu_xdcdr_sel,axi_cpu_xdcdr_sel,HI_SC_CTRL69_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL69_OFFSET)
HI_SET_GET(hi_sc_ctrl69_axi_glb_xdcdr_sel,axi_glb_xdcdr_sel,HI_SC_CTRL69_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL69_OFFSET)
HI_SET_GET(hi_sc_ctrl69_sysapb_pslverr_sel,sysapb_pslverr_sel,HI_SC_CTRL69_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL69_OFFSET)
HI_SET_GET(hi_sc_ctrl69_socapb_pslverr_sel,socapb_pslverr_sel,HI_SC_CTRL69_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL69_OFFSET)
HI_SET_GET(hi_sc_ctrl69_x2h_hslverr_sel,x2h_hslverr_sel,HI_SC_CTRL69_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL69_OFFSET)
HI_SET_GET(hi_sc_ctrl69_mst_err_srst_req,mst_err_srst_req,HI_SC_CTRL69_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL69_OFFSET)
HI_SET_GET(hi_sc_ctrl69_reserved,reserved,HI_SC_CTRL69_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL69_OFFSET)
HI_SET_GET(hi_sc_ctrl70_axi_bbphy_priority_m1,axi_bbphy_priority_m1,HI_SC_CTRL70_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL70_OFFSET)
HI_SET_GET(hi_sc_ctrl70_axi_bbphy_priority_m2,axi_bbphy_priority_m2,HI_SC_CTRL70_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL70_OFFSET)
HI_SET_GET(hi_sc_ctrl70_axi_bbphy_priority_m3,axi_bbphy_priority_m3,HI_SC_CTRL70_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL70_OFFSET)
HI_SET_GET(hi_sc_ctrl70_axi_bbphy_priority_m4,axi_bbphy_priority_m4,HI_SC_CTRL70_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL70_OFFSET)
HI_SET_GET(hi_sc_ctrl70_axi_guacc_priority_m1,axi_guacc_priority_m1,HI_SC_CTRL70_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL70_OFFSET)
HI_SET_GET(hi_sc_ctrl70_axi_guacc_priority_m2,axi_guacc_priority_m2,HI_SC_CTRL70_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL70_OFFSET)
HI_SET_GET(hi_sc_ctrl70_axi_guacc_priority_m3,axi_guacc_priority_m3,HI_SC_CTRL70_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL70_OFFSET)
HI_SET_GET(hi_sc_ctrl70_axi_guacc_priority_m4,axi_guacc_priority_m4,HI_SC_CTRL70_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL70_OFFSET)
HI_SET_GET(hi_sc_ctrl70_axi_mst_priority_m9,axi_mst_priority_m9,HI_SC_CTRL70_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL70_OFFSET)
HI_SET_GET(hi_sc_ctrl70_axi_mst_priority_m10,axi_mst_priority_m10,HI_SC_CTRL70_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL70_OFFSET)
HI_SET_GET(hi_sc_ctrl70_reserved_1,reserved_1,HI_SC_CTRL70_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL70_OFFSET)
HI_SET_GET(hi_sc_ctrl70_axi_cpu_priority_m1,axi_cpu_priority_m1,HI_SC_CTRL70_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL70_OFFSET)
HI_SET_GET(hi_sc_ctrl70_axi_cpu_priority_m2,axi_cpu_priority_m2,HI_SC_CTRL70_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL70_OFFSET)
HI_SET_GET(hi_sc_ctrl70_axi_amon_mst_priority_m1,axi_amon_mst_priority_m1,HI_SC_CTRL70_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL70_OFFSET)
HI_SET_GET(hi_sc_ctrl70_reserved_0,reserved_0,HI_SC_CTRL70_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL70_OFFSET)
HI_SET_GET(hi_sc_ctrl71_reserved,reserved,HI_SC_CTRL71_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL71_OFFSET)
HI_SET_GET(hi_sc_ctrl71_axi_glb_priority_m1,axi_glb_priority_m1,HI_SC_CTRL71_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL71_OFFSET)
HI_SET_GET(hi_sc_ctrl71_axi_glb_priority_m2,axi_glb_priority_m2,HI_SC_CTRL71_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL71_OFFSET)
HI_SET_GET(hi_sc_ctrl71_axi_glb_priority_m3,axi_glb_priority_m3,HI_SC_CTRL71_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL71_OFFSET)
HI_SET_GET(hi_sc_ctrl71_axi_glb_priority_m4,axi_glb_priority_m4,HI_SC_CTRL71_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL71_OFFSET)
HI_SET_GET(hi_sc_ctrl71_axi_glb_priority_m5,axi_glb_priority_m5,HI_SC_CTRL71_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL71_OFFSET)
HI_SET_GET(hi_sc_ctrl71_axi_glb_priority_m6,axi_glb_priority_m6,HI_SC_CTRL71_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL71_OFFSET)
HI_SET_GET(hi_sc_ctrl71_axi_mst_cache_cfg_en,axi_mst_cache_cfg_en,HI_SC_CTRL71_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL71_OFFSET)
HI_SET_GET(hi_sc_ctrl71_axi_mst_sideband,axi_mst_sideband,HI_SC_CTRL71_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL71_OFFSET)
HI_SET_GET(hi_sc_ctrl71_axi_mst_cache,axi_mst_cache,HI_SC_CTRL71_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL71_OFFSET)
HI_SET_GET(hi_sc_ctrl72_axi_mst_priority_m1,axi_mst_priority_m1,HI_SC_CTRL72_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL72_OFFSET)
HI_SET_GET(hi_sc_ctrl72_axi_mst_priority_m2,axi_mst_priority_m2,HI_SC_CTRL72_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL72_OFFSET)
HI_SET_GET(hi_sc_ctrl72_axi_mst_priority_m3,axi_mst_priority_m3,HI_SC_CTRL72_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL72_OFFSET)
HI_SET_GET(hi_sc_ctrl72_axi_mst_priority_m4,axi_mst_priority_m4,HI_SC_CTRL72_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL72_OFFSET)
HI_SET_GET(hi_sc_ctrl72_axi_mst_priority_m5,axi_mst_priority_m5,HI_SC_CTRL72_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL72_OFFSET)
HI_SET_GET(hi_sc_ctrl72_axi_mst_priority_m6,axi_mst_priority_m6,HI_SC_CTRL72_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL72_OFFSET)
HI_SET_GET(hi_sc_ctrl72_axi_mst_priority_m7,axi_mst_priority_m7,HI_SC_CTRL72_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL72_OFFSET)
HI_SET_GET(hi_sc_ctrl72_axi_mst_priority_m8,axi_mst_priority_m8,HI_SC_CTRL72_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL72_OFFSET)
HI_SET_GET(hi_sc_ctrl81_appa9_gpio0_en,appa9_gpio0_en,HI_SC_CTRL81_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL81_OFFSET)
HI_SET_GET(hi_sc_ctrl85_mdma9_gpio0_en,mdma9_gpio0_en,HI_SC_CTRL85_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL85_OFFSET)
HI_SET_GET(hi_sc_ctrl98_cm3_gpio0_en,cm3_gpio0_en,HI_SC_CTRL98_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL98_OFFSET)
HI_SET_GET(hi_sc_ctrl103_reserved_6,reserved_6,HI_SC_CTRL103_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL103_OFFSET)
HI_SET_GET(hi_sc_ctrl103_bbphy_dsp0_axislv_active,bbphy_dsp0_axislv_active,HI_SC_CTRL103_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL103_OFFSET)
HI_SET_GET(hi_sc_ctrl103_bbphy_bbp_axislv_active,bbphy_bbp_axislv_active,HI_SC_CTRL103_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL103_OFFSET)
HI_SET_GET(hi_sc_ctrl103_reserved_5,reserved_5,HI_SC_CTRL103_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL103_OFFSET)
HI_SET_GET(hi_sc_ctrl103_amon_soc_axislv_active,amon_soc_axislv_active,HI_SC_CTRL103_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL103_OFFSET)
HI_SET_GET(hi_sc_ctrl103_amon_cpufast_axislv_active,amon_cpufast_axislv_active,HI_SC_CTRL103_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL103_OFFSET)
HI_SET_GET(hi_sc_ctrl103_cs_axislv_active,cs_axislv_active,HI_SC_CTRL103_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL103_OFFSET)
HI_SET_GET(hi_sc_ctrl103_reserved_4,reserved_4,HI_SC_CTRL103_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL103_OFFSET)
HI_SET_GET(hi_sc_ctrl103_mdmacp_axislv_active,mdmacp_axislv_active,HI_SC_CTRL103_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL103_OFFSET)
HI_SET_GET(hi_sc_ctrl103_appacp_axislv_active,appacp_axislv_active,HI_SC_CTRL103_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL103_OFFSET)
HI_SET_GET(hi_sc_ctrl103_ddrdata_axislv_active,ddrdata_axislv_active,HI_SC_CTRL103_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL103_OFFSET)
HI_SET_GET(hi_sc_ctrl103_axi_mem_axislv_active,axi_mem_axislv_active,HI_SC_CTRL103_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL103_OFFSET)
HI_SET_GET(hi_sc_ctrl103_x2h_peri_axislv_active,x2h_peri_axislv_active,HI_SC_CTRL103_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL103_OFFSET)
HI_SET_GET(hi_sc_ctrl103_bbphy_axislv_active,bbphy_axislv_active,HI_SC_CTRL103_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL103_OFFSET)
HI_SET_GET(hi_sc_ctrl103_x2p_axislv_active,x2p_axislv_active,HI_SC_CTRL103_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL103_OFFSET)
HI_SET_GET(hi_sc_ctrl103_hifi_axislv_active,hifi_axislv_active,HI_SC_CTRL103_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL103_OFFSET)
HI_SET_GET(hi_sc_ctrl103_dw_axi_amon_axislv_active,dw_axi_amon_axislv_active,HI_SC_CTRL103_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL103_OFFSET)
HI_SET_GET(hi_sc_ctrl103_cpu_slv_active,cpu_slv_active,HI_SC_CTRL103_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL103_OFFSET)
HI_SET_GET(hi_sc_ctrl103_reserved_3,reserved_3,HI_SC_CTRL103_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL103_OFFSET)
HI_SET_GET(hi_sc_ctrl103_wdog_apbslv_active,wdog_apbslv_active,HI_SC_CTRL103_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL103_OFFSET)
HI_SET_GET(hi_sc_ctrl103_timer0_7_apbslv_active,timer0_7_apbslv_active,HI_SC_CTRL103_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL103_OFFSET)
HI_SET_GET(hi_sc_ctrl103_timer8_15_apbslv_active,timer8_15_apbslv_active,HI_SC_CTRL103_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL103_OFFSET)
HI_SET_GET(hi_sc_ctrl103_timer16_23_apbslv_active,timer16_23_apbslv_active,HI_SC_CTRL103_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL103_OFFSET)
HI_SET_GET(hi_sc_ctrl103_reserved_2,reserved_2,HI_SC_CTRL103_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL103_OFFSET)
HI_SET_GET(hi_sc_ctrl103_gpio0_apbslv_active,gpio0_apbslv_active,HI_SC_CTRL103_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL103_OFFSET)
HI_SET_GET(hi_sc_ctrl103_uart0_apbslv_active,uart0_apbslv_active,HI_SC_CTRL103_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL103_OFFSET)
HI_SET_GET(hi_sc_ctrl103_reserved_1,reserved_1,HI_SC_CTRL103_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL103_OFFSET)
HI_SET_GET(hi_sc_ctrl103_sci0_apbslv_active,sci0_apbslv_active,HI_SC_CTRL103_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL103_OFFSET)
HI_SET_GET(hi_sc_ctrl103_sci1_apbslv_apbactive,sci1_apbslv_apbactive,HI_SC_CTRL103_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL103_OFFSET)
HI_SET_GET(hi_sc_ctrl103_tsensor_apbslv_active,tsensor_apbslv_active,HI_SC_CTRL103_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL103_OFFSET)
HI_SET_GET(hi_sc_ctrl103_usbotg_apbslv_active,usbotg_apbslv_active,HI_SC_CTRL103_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL103_OFFSET)
HI_SET_GET(hi_sc_ctrl103_reserved_0,reserved_0,HI_SC_CTRL103_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL103_OFFSET)
HI_SET_GET(hi_sc_ctrl104_cs_slv_active,cs_slv_active,HI_SC_CTRL104_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL104_OFFSET)
HI_SET_GET(hi_sc_ctrl104_pmussi0_slv_active,pmussi0_slv_active,HI_SC_CTRL104_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL104_OFFSET)
HI_SET_GET(hi_sc_ctrl104_reserved_8,reserved_8,HI_SC_CTRL104_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL104_OFFSET)
HI_SET_GET(hi_sc_ctrl104_usb_otg_slv_active,usb_otg_slv_active,HI_SC_CTRL104_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL104_OFFSET)
HI_SET_GET(hi_sc_ctrl104_reserved_7,reserved_7,HI_SC_CTRL104_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL104_OFFSET)
HI_SET_GET(hi_sc_ctrl104_reserved_6,reserved_6,HI_SC_CTRL104_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL104_OFFSET)
HI_SET_GET(hi_sc_ctrl104_dwssi0_slv_active,dwssi0_slv_active,HI_SC_CTRL104_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL104_OFFSET)
HI_SET_GET(hi_sc_ctrl104_gpio1_slv_active,gpio1_slv_active,HI_SC_CTRL104_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL104_OFFSET)
HI_SET_GET(hi_sc_ctrl104_gpio2_slv_active,gpio2_slv_active,HI_SC_CTRL104_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL104_OFFSET)
HI_SET_GET(hi_sc_ctrl104_gpio3_slv_active,gpio3_slv_active,HI_SC_CTRL104_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL104_OFFSET)
HI_SET_GET(hi_sc_ctrl104_reserved_5,reserved_5,HI_SC_CTRL104_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL104_OFFSET)
HI_SET_GET(hi_sc_ctrl104_uart1_slv_active,uart1_slv_active,HI_SC_CTRL104_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL104_OFFSET)
HI_SET_GET(hi_sc_ctrl104_uart2_slv_active,uart2_slv_active,HI_SC_CTRL104_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL104_OFFSET)
HI_SET_GET(hi_sc_ctrl104_uart3_slv_active,uart3_slv_active,HI_SC_CTRL104_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL104_OFFSET)
HI_SET_GET(hi_sc_ctrl104_i2c0_slv_active,i2c0_slv_active,HI_SC_CTRL104_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL104_OFFSET)
HI_SET_GET(hi_sc_ctrl104_i2c1_slv_active,i2c1_slv_active,HI_SC_CTRL104_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL104_OFFSET)
HI_SET_GET(hi_sc_ctrl104_mddrc_slv_active,mddrc_slv_active,HI_SC_CTRL104_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL104_OFFSET)
HI_SET_GET(hi_sc_ctrl104_efusec_slv_active,efusec_slv_active,HI_SC_CTRL104_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL104_OFFSET)
HI_SET_GET(hi_sc_ctrl104_reserved_4,reserved_4,HI_SC_CTRL104_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL104_OFFSET)
HI_SET_GET(hi_sc_ctrl104_dwssi1_slv_active,dwssi1_slv_active,HI_SC_CTRL104_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL104_OFFSET)
HI_SET_GET(hi_sc_ctrl104_edam_slv_active,edam_slv_active,HI_SC_CTRL104_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL104_OFFSET)
HI_SET_GET(hi_sc_ctrl104_dt_slv_active,dt_slv_active,HI_SC_CTRL104_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL104_OFFSET)
HI_SET_GET(hi_sc_ctrl104_edmac_ch4_slv_active,edmac_ch4_slv_active,HI_SC_CTRL104_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL104_OFFSET)
HI_SET_GET(hi_sc_ctrl104_reserved_3,reserved_3,HI_SC_CTRL104_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL104_OFFSET)
HI_SET_GET(hi_sc_ctrl104_pcie_phy_slv_active,pcie_phy_slv_active,HI_SC_CTRL104_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL104_OFFSET)
HI_SET_GET(hi_sc_ctrl104_reserved_2,reserved_2,HI_SC_CTRL104_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL104_OFFSET)
HI_SET_GET(hi_sc_ctrl104_mipi_slv_active,mipi_slv_active,HI_SC_CTRL104_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL104_OFFSET)
HI_SET_GET(hi_sc_ctrl104_reserved_1,reserved_1,HI_SC_CTRL104_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL104_OFFSET)
HI_SET_GET(hi_sc_ctrl104_bbp_ao_slv_active,bbp_ao_slv_active,HI_SC_CTRL104_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL104_OFFSET)
HI_SET_GET(hi_sc_ctrl104_reserved_0,reserved_0,HI_SC_CTRL104_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL104_OFFSET)
HI_SET_GET(hi_sc_ctrl104_pwm0_slv_active,pwm0_slv_active,HI_SC_CTRL104_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL104_OFFSET)
HI_SET_GET(hi_sc_ctrl104_pwm1_slv_active,pwm1_slv_active,HI_SC_CTRL104_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL104_OFFSET)
HI_SET_GET(hi_sc_ctrl105_uicc,uicc,HI_SC_CTRL105_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL105_OFFSET)
HI_SET_GET(hi_sc_ctrl105_hsic,hsic,HI_SC_CTRL105_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL105_OFFSET)
HI_SET_GET(hi_sc_ctrl105_reserved_8,reserved_8,HI_SC_CTRL105_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL105_OFFSET)
HI_SET_GET(hi_sc_ctrl105_emi_reg,emi_reg,HI_SC_CTRL105_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL105_OFFSET)
HI_SET_GET(hi_sc_ctrl105_emi_mem,emi_mem,HI_SC_CTRL105_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL105_OFFSET)
HI_SET_GET(hi_sc_ctrl105_nandc_reg,nandc_reg,HI_SC_CTRL105_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL105_OFFSET)
HI_SET_GET(hi_sc_ctrl105_ipcm,ipcm,HI_SC_CTRL105_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL105_OFFSET)
HI_SET_GET(hi_sc_ctrl105_sio,sio,HI_SC_CTRL105_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL105_OFFSET)
HI_SET_GET(hi_sc_ctrl105_hs_uart,hs_uart,HI_SC_CTRL105_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL105_OFFSET)
HI_SET_GET(hi_sc_ctrl105_socp,socp,HI_SC_CTRL105_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL105_OFFSET)
HI_SET_GET(hi_sc_ctrl105_reserved_7,reserved_7,HI_SC_CTRL105_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL105_OFFSET)
HI_SET_GET(hi_sc_ctrl105_cipher,cipher,HI_SC_CTRL105_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL105_OFFSET)
HI_SET_GET(hi_sc_ctrl105_ipf,ipf,HI_SC_CTRL105_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL105_OFFSET)
HI_SET_GET(hi_sc_ctrl105_mmc0,mmc0,HI_SC_CTRL105_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL105_OFFSET)
HI_SET_GET(hi_sc_ctrl105_mmc1,mmc1,HI_SC_CTRL105_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL105_OFFSET)
HI_SET_GET(hi_sc_ctrl105_reserved_6,reserved_6,HI_SC_CTRL105_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL105_OFFSET)
HI_SET_GET(hi_sc_ctrl105_sdcc,sdcc,HI_SC_CTRL105_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL105_OFFSET)
HI_SET_GET(hi_sc_ctrl105_reserved_5,reserved_5,HI_SC_CTRL105_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL105_OFFSET)
HI_SET_GET(hi_sc_ctrl105_reserved_4,reserved_4,HI_SC_CTRL105_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL105_OFFSET)
HI_SET_GET(hi_sc_ctrl105_cicom0,cicom0,HI_SC_CTRL105_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL105_OFFSET)
HI_SET_GET(hi_sc_ctrl105_cicom1,cicom1,HI_SC_CTRL105_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL105_OFFSET)
HI_SET_GET(hi_sc_ctrl105_hdlc,hdlc,HI_SC_CTRL105_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL105_OFFSET)
HI_SET_GET(hi_sc_ctrl105_usb3,usb3,HI_SC_CTRL105_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL105_OFFSET)
HI_SET_GET(hi_sc_ctrl105_nandc_mem,nandc_mem,HI_SC_CTRL105_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL105_OFFSET)
HI_SET_GET(hi_sc_ctrl105_reserved_3,reserved_3,HI_SC_CTRL105_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL105_OFFSET)
HI_SET_GET(hi_sc_ctrl105_vic1,vic1,HI_SC_CTRL105_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL105_OFFSET)
HI_SET_GET(hi_sc_ctrl105_cm3,cm3,HI_SC_CTRL105_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL105_OFFSET)
HI_SET_GET(hi_sc_ctrl105_reserved_2,reserved_2,HI_SC_CTRL105_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL105_OFFSET)
HI_SET_GET(hi_sc_ctrl105_reserved_1,reserved_1,HI_SC_CTRL105_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL105_OFFSET)
HI_SET_GET(hi_sc_ctrl105_upacc,upacc,HI_SC_CTRL105_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL105_OFFSET)
HI_SET_GET(hi_sc_ctrl105_reserved_0,reserved_0,HI_SC_CTRL105_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL105_OFFSET)
HI_SET_GET(hi_sc_ctrl105_tcssi1,tcssi1,HI_SC_CTRL105_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL105_OFFSET)
HI_SET_GET(hi_sc_ctrl121_reserved,reserved,HI_SC_CTRL121_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL121_OFFSET)
HI_SET_GET(hi_sc_ctrl122_reserved,reserved,HI_SC_CTRL122_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL122_OFFSET)
HI_SET_GET(hi_sc_ctrl123_reserved,reserved,HI_SC_CTRL123_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL123_OFFSET)
HI_SET_GET(hi_sc_ctrl124_reserved,reserved,HI_SC_CTRL124_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL124_OFFSET)
HI_SET_GET(hi_sc_ctrl125_reserved,reserved,HI_SC_CTRL125_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL125_OFFSET)
HI_SET_GET(hi_sc_ctrl126_reserved,reserved,HI_SC_CTRL126_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL126_OFFSET)
HI_SET_GET(hi_sc_ctrl127_reserved,reserved,HI_SC_CTRL127_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_CTRL127_OFFSET)
HI_SET_GET(hi_sc_stat0_boot_mode2_0,boot_mode2_0,HI_SC_STAT0_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT0_OFFSET)
HI_SET_GET(hi_sc_stat0_boot_mode3,boot_mode3,HI_SC_STAT0_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT0_OFFSET)
HI_SET_GET(hi_sc_stat0_reserved_2,reserved_2,HI_SC_STAT0_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT0_OFFSET)
HI_SET_GET(hi_sc_stat0_jtag_mode0,jtag_mode0,HI_SC_STAT0_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT0_OFFSET)
HI_SET_GET(hi_sc_stat0_jtag_mode1,jtag_mode1,HI_SC_STAT0_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT0_OFFSET)
HI_SET_GET(hi_sc_stat0_reserved_1,reserved_1,HI_SC_STAT0_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT0_OFFSET)
HI_SET_GET(hi_sc_stat0_jtag_dft_mode,jtag_dft_mode,HI_SC_STAT0_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT0_OFFSET)
HI_SET_GET(hi_sc_stat0_wdt_rst_n_store,wdt_rst_n_store,HI_SC_STAT0_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT0_OFFSET)
HI_SET_GET(hi_sc_stat0_pmu_rst_n_store,pmu_rst_n_store,HI_SC_STAT0_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT0_OFFSET)
HI_SET_GET(hi_sc_stat0_reserved_0,reserved_0,HI_SC_STAT0_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT0_OFFSET)
HI_SET_GET(hi_sc_stat1_appa9_pmupriv,appa9_pmupriv,HI_SC_STAT1_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT1_OFFSET)
HI_SET_GET(hi_sc_stat1_appa9_pmusecure,appa9_pmusecure,HI_SC_STAT1_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT1_OFFSET)
HI_SET_GET(hi_sc_stat1_appa9_smpnamp,appa9_smpnamp,HI_SC_STAT1_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT1_OFFSET)
HI_SET_GET(hi_sc_stat1_appa9_scuevabort,appa9_scuevabort,HI_SC_STAT1_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT1_OFFSET)
HI_SET_GET(hi_sc_stat1_appa9_pwrctlo0,appa9_pwrctlo0,HI_SC_STAT1_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT1_OFFSET)
HI_SET_GET(hi_sc_stat1_appa9_l2_tagclkouten,appa9_l2_tagclkouten,HI_SC_STAT1_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT1_OFFSET)
HI_SET_GET(hi_sc_stat1_appa9_l2_dataclkouten,appa9_l2_dataclkouten,HI_SC_STAT1_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT1_OFFSET)
HI_SET_GET(hi_sc_stat1_appa9_l2_idle,appa9_l2_idle,HI_SC_STAT1_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT1_OFFSET)
HI_SET_GET(hi_sc_stat1_appa9_l2_clkstopped,appa9_l2_clkstopped,HI_SC_STAT1_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT1_OFFSET)
HI_SET_GET(hi_sc_stat1_reserved_1,reserved_1,HI_SC_STAT1_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT1_OFFSET)
HI_SET_GET(hi_sc_stat1_moda9_pmupriv,moda9_pmupriv,HI_SC_STAT1_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT1_OFFSET)
HI_SET_GET(hi_sc_stat1_moda9_pmusecure,moda9_pmusecure,HI_SC_STAT1_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT1_OFFSET)
HI_SET_GET(hi_sc_stat1_moda9_smpnamp,moda9_smpnamp,HI_SC_STAT1_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT1_OFFSET)
HI_SET_GET(hi_sc_stat1_moda9_scuevabort,moda9_scuevabort,HI_SC_STAT1_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT1_OFFSET)
HI_SET_GET(hi_sc_stat1_moda9_pwrctlo0,moda9_pwrctlo0,HI_SC_STAT1_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT1_OFFSET)
HI_SET_GET(hi_sc_stat1_moda9_l2_tagclkouten,moda9_l2_tagclkouten,HI_SC_STAT1_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT1_OFFSET)
HI_SET_GET(hi_sc_stat1_moda9_l2_dataclkouten,moda9_l2_dataclkouten,HI_SC_STAT1_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT1_OFFSET)
HI_SET_GET(hi_sc_stat1_moda9_l2_idle,moda9_l2_idle,HI_SC_STAT1_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT1_OFFSET)
HI_SET_GET(hi_sc_stat1_moda9_l2_clkstopped,moda9_l2_clkstopped,HI_SC_STAT1_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT1_OFFSET)
HI_SET_GET(hi_sc_stat1_reserved_0,reserved_0,HI_SC_STAT1_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT1_OFFSET)
HI_SET_GET(hi_sc_stat2_hifi_pwaitmode,hifi_pwaitmode,HI_SC_STAT2_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT2_OFFSET)
HI_SET_GET(hi_sc_stat2_hifi_xocdmode,hifi_xocdmode,HI_SC_STAT2_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT2_OFFSET)
HI_SET_GET(hi_sc_stat2_appa9_deflags,appa9_deflags,HI_SC_STAT2_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT2_OFFSET)
HI_SET_GET(hi_sc_stat2_reserved_1,reserved_1,HI_SC_STAT2_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT2_OFFSET)
HI_SET_GET(hi_sc_stat2_dsp0_pwaitmode,dsp0_pwaitmode,HI_SC_STAT2_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT2_OFFSET)
HI_SET_GET(hi_sc_stat2_dsp0_xocdmode,dsp0_xocdmode,HI_SC_STAT2_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT2_OFFSET)
HI_SET_GET(hi_sc_stat2_reserved_0,reserved_0,HI_SC_STAT2_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT2_OFFSET)
HI_SET_GET(hi_sc_stat3_hpm0_opc,hpm0_opc,HI_SC_STAT3_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT3_OFFSET)
HI_SET_GET(hi_sc_stat3_hpm0_opc_vld,hpm0_opc_vld,HI_SC_STAT3_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT3_OFFSET)
HI_SET_GET(hi_sc_stat3_reserved_1,reserved_1,HI_SC_STAT3_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT3_OFFSET)
HI_SET_GET(hi_sc_stat3_hpm1_opc,hpm1_opc,HI_SC_STAT3_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT3_OFFSET)
HI_SET_GET(hi_sc_stat3_hpm1_opc_vld,hpm1_opc_vld,HI_SC_STAT3_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT3_OFFSET)
HI_SET_GET(hi_sc_stat3_reserved_0,reserved_0,HI_SC_STAT3_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT3_OFFSET)
HI_SET_GET(hi_sc_stat4_hpm2_opc,hpm2_opc,HI_SC_STAT4_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT4_OFFSET)
HI_SET_GET(hi_sc_stat4_hpm2_opc_vld,hpm2_opc_vld,HI_SC_STAT4_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT4_OFFSET)
HI_SET_GET(hi_sc_stat4_reserved,reserved,HI_SC_STAT4_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT4_OFFSET)
HI_SET_GET(hi_sc_stat5_reserved,reserved,HI_SC_STAT5_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT5_OFFSET)
HI_SET_GET(hi_sc_stat6_appa9_parityfail0,appa9_parityfail0,HI_SC_STAT6_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT6_OFFSET)
HI_SET_GET(hi_sc_stat6_appa9_parityfailscu,appa9_parityfailscu,HI_SC_STAT6_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT6_OFFSET)
HI_SET_GET(hi_sc_stat6_reserved_1,reserved_1,HI_SC_STAT6_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT6_OFFSET)
HI_SET_GET(hi_sc_stat6_moda9_parityfail0,moda9_parityfail0,HI_SC_STAT6_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT6_OFFSET)
HI_SET_GET(hi_sc_stat6_moda9_parityfailscu,moda9_parityfailscu,HI_SC_STAT6_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT6_OFFSET)
HI_SET_GET(hi_sc_stat6_reserved_0,reserved_0,HI_SC_STAT6_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT6_OFFSET)
HI_SET_GET(hi_sc_stat7_reserved,reserved,HI_SC_STAT7_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT7_OFFSET)
HI_SET_GET(hi_sc_stat8_bbp_timer_readcnt,bbp_timer_readcnt,HI_SC_STAT8_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT8_OFFSET)
HI_SET_GET(hi_sc_stat9_dw_axi_mst_dlock_wr,dw_axi_mst_dlock_wr,HI_SC_STAT9_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT9_OFFSET)
HI_SET_GET(hi_sc_stat9_dw_axi_mst_dlock_slv,dw_axi_mst_dlock_slv,HI_SC_STAT9_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT9_OFFSET)
HI_SET_GET(hi_sc_stat9_reserved_3,reserved_3,HI_SC_STAT9_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT9_OFFSET)
HI_SET_GET(hi_sc_stat9_dw_axi_mst_dlock_mst,dw_axi_mst_dlock_mst,HI_SC_STAT9_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT9_OFFSET)
HI_SET_GET(hi_sc_stat9_dw_axi_cpu_dlock_wr,dw_axi_cpu_dlock_wr,HI_SC_STAT9_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT9_OFFSET)
HI_SET_GET(hi_sc_stat9_dw_axi_cpu_dlock_slv,dw_axi_cpu_dlock_slv,HI_SC_STAT9_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT9_OFFSET)
HI_SET_GET(hi_sc_stat9_dw_axi_cpu_dlock_mst,dw_axi_cpu_dlock_mst,HI_SC_STAT9_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT9_OFFSET)
HI_SET_GET(hi_sc_stat9_reserved_2,reserved_2,HI_SC_STAT9_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT9_OFFSET)
HI_SET_GET(hi_sc_stat9_axi_amon_dlock_wr,axi_amon_dlock_wr,HI_SC_STAT9_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT9_OFFSET)
HI_SET_GET(hi_sc_stat9_axi_amon_dlock_slv,axi_amon_dlock_slv,HI_SC_STAT9_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT9_OFFSET)
HI_SET_GET(hi_sc_stat9_axi_amon_dlock_mst,axi_amon_dlock_mst,HI_SC_STAT9_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT9_OFFSET)
HI_SET_GET(hi_sc_stat9_reserved_1,reserved_1,HI_SC_STAT9_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT9_OFFSET)
HI_SET_GET(hi_sc_stat9_dw_axi_glb_dlock_wr,dw_axi_glb_dlock_wr,HI_SC_STAT9_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT9_OFFSET)
HI_SET_GET(hi_sc_stat9_dw_axi_glb_dlock_slv,dw_axi_glb_dlock_slv,HI_SC_STAT9_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT9_OFFSET)
HI_SET_GET(hi_sc_stat9_dw_axi_glb_dlock_mst,dw_axi_glb_dlock_mst,HI_SC_STAT9_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT9_OFFSET)
HI_SET_GET(hi_sc_stat9_reserved_0,reserved_0,HI_SC_STAT9_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT9_OFFSET)
HI_SET_GET(hi_sc_stat10_dw_axi_glb_dlock_id,dw_axi_glb_dlock_id,HI_SC_STAT10_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT10_OFFSET)
HI_SET_GET(hi_sc_stat10_dw_axi_amon_dlock_id,dw_axi_amon_dlock_id,HI_SC_STAT10_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT10_OFFSET)
HI_SET_GET(hi_sc_stat10_dw_axi_cpu_dlock_id,dw_axi_cpu_dlock_id,HI_SC_STAT10_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT10_OFFSET)
HI_SET_GET(hi_sc_stat10_dw_axi_mst_dlock_id,dw_axi_mst_dlock_id,HI_SC_STAT10_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT10_OFFSET)
HI_SET_GET(hi_sc_stat11_sdcc_pull_stat,sdcc_pull_stat,HI_SC_STAT11_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT11_OFFSET)
HI_SET_GET(hi_sc_stat11_reserved,reserved,HI_SC_STAT11_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT11_OFFSET)
HI_SET_GET(hi_sc_stat12_sdcc_pull_raw,sdcc_pull_raw,HI_SC_STAT12_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT12_OFFSET)
HI_SET_GET(hi_sc_stat12_reserved,reserved,HI_SC_STAT12_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT12_OFFSET)
HI_SET_GET(hi_sc_stat13_reserved,reserved,HI_SC_STAT13_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT13_OFFSET)
HI_SET_GET(hi_sc_stat14_cr_data_out,cr_data_out,HI_SC_STAT14_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT14_OFFSET)
HI_SET_GET(hi_sc_stat14_cr_ack,cr_ack,HI_SC_STAT14_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT14_OFFSET)
HI_SET_GET(hi_sc_stat14_reserved_2,reserved_2,HI_SC_STAT14_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT14_OFFSET)
HI_SET_GET(hi_sc_stat14_rtune_ack,rtune_ack,HI_SC_STAT14_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT14_OFFSET)
HI_SET_GET(hi_sc_stat14_reserved_1,reserved_1,HI_SC_STAT14_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT14_OFFSET)
HI_SET_GET(hi_sc_stat14_ref_clkreq_n,ref_clkreq_n,HI_SC_STAT14_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT14_OFFSET)
HI_SET_GET(hi_sc_stat14_reserved_0,reserved_0,HI_SC_STAT14_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT14_OFFSET)
HI_SET_GET(hi_sc_stat15_sysapb_pslv_active0,sysapb_pslv_active0,HI_SC_STAT15_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT15_OFFSET)
HI_SET_GET(hi_sc_stat15_reserved_1,reserved_1,HI_SC_STAT15_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT15_OFFSET)
HI_SET_GET(hi_sc_stat15_sysapb_pslv_active1,sysapb_pslv_active1,HI_SC_STAT15_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT15_OFFSET)
HI_SET_GET(hi_sc_stat15_reserved_0,reserved_0,HI_SC_STAT15_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT15_OFFSET)
HI_SET_GET(hi_sc_stat16_ldrx2dbg_abs_timer_31_0,ldrx2dbg_abs_timer_31_0,HI_SC_STAT16_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT16_OFFSET)
HI_SET_GET(hi_sc_stat17_ldrx2dbg_abs_timer_63_32,ldrx2dbg_abs_timer_63_32,HI_SC_STAT17_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT17_OFFSET)
HI_SET_GET(hi_sc_stat18_wdrx_deepsleep_flag,wdrx_deepsleep_flag,HI_SC_STAT18_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT18_OFFSET)
HI_SET_GET(hi_sc_stat18_g1_gdrx_deepsleep_falg,g1_gdrx_deepsleep_falg,HI_SC_STAT18_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT18_OFFSET)
HI_SET_GET(hi_sc_stat18_g2_gdrx_deepsleep_falg,g2_gdrx_deepsleep_falg,HI_SC_STAT18_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT18_OFFSET)
HI_SET_GET(hi_sc_stat18_reserved,reserved,HI_SC_STAT18_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT18_OFFSET)
HI_SET_GET(hi_sc_stat19_reserved,reserved,HI_SC_STAT19_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT19_OFFSET)
HI_SET_GET(hi_sc_stat20_reserved,reserved,HI_SC_STAT20_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT20_OFFSET)
HI_SET_GET(hi_sc_stat21_usb3_connect_state_u2pmu,usb3_connect_state_u2pmu,HI_SC_STAT21_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT21_OFFSET)
HI_SET_GET(hi_sc_stat21_usb3_connect_state_u3pmu,usb3_connect_state_u3pmu,HI_SC_STAT21_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT21_OFFSET)
HI_SET_GET(hi_sc_stat21_usb3_host_current_belt,usb3_host_current_belt,HI_SC_STAT21_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT21_OFFSET)
HI_SET_GET(hi_sc_stat21_reserved_1,reserved_1,HI_SC_STAT21_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT21_OFFSET)
HI_SET_GET(hi_sc_stat21_usb3_pmu_current_power_state_u2pmu,usb3_pmu_current_power_state_u2pmu,HI_SC_STAT21_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT21_OFFSET)
HI_SET_GET(hi_sc_stat21_usb3_pmu_current_power_state_u3pmu,usb3_pmu_current_power_state_u3pmu,HI_SC_STAT21_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT21_OFFSET)
HI_SET_GET(hi_sc_stat21_usb3_pmu_phy_u2dsport_vbus_ctrl,usb3_pmu_phy_u2dsport_vbus_ctrl,HI_SC_STAT21_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT21_OFFSET)
HI_SET_GET(hi_sc_stat21_usb3_pmu_phy_u3dsport_vbus_ctrl,usb3_pmu_phy_u3dsport_vbus_ctrl,HI_SC_STAT21_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT21_OFFSET)
HI_SET_GET(hi_sc_stat21_reserved_0,reserved_0,HI_SC_STAT21_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT21_OFFSET)
HI_SET_GET(hi_sc_stat22_dw_axi_bbphy_dlock_mst,dw_axi_bbphy_dlock_mst,HI_SC_STAT22_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT22_OFFSET)
HI_SET_GET(hi_sc_stat22_dw_axi_bbphy_dlock_slv,dw_axi_bbphy_dlock_slv,HI_SC_STAT22_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT22_OFFSET)
HI_SET_GET(hi_sc_stat22_dw_axi_bbphy_dlock_wr,dw_axi_bbphy_dlock_wr,HI_SC_STAT22_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT22_OFFSET)
HI_SET_GET(hi_sc_stat22_reserved_1,reserved_1,HI_SC_STAT22_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT22_OFFSET)
HI_SET_GET(hi_sc_stat22_dw_axi_bbphy_dlock_id,dw_axi_bbphy_dlock_id,HI_SC_STAT22_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT22_OFFSET)
HI_SET_GET(hi_sc_stat22_reserved_0,reserved_0,HI_SC_STAT22_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT22_OFFSET)
HI_SET_GET(hi_sc_stat23_func_mbist_fail,func_mbist_fail,HI_SC_STAT23_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT23_OFFSET)
HI_SET_GET(hi_sc_stat24_func_mbist_done,func_mbist_done,HI_SC_STAT24_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT24_OFFSET)
HI_SET_GET(hi_sc_stat25_func_mbist_done,func_mbist_done,HI_SC_STAT25_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT25_OFFSET)
HI_SET_GET(hi_sc_stat26_socp_idle,socp_idle,HI_SC_STAT26_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT26_OFFSET)
HI_SET_GET(hi_sc_stat26_ipf_idle,ipf_idle,HI_SC_STAT26_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT26_OFFSET)
HI_SET_GET(hi_sc_stat26_cicom0_clk_state,cicom0_clk_state,HI_SC_STAT26_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT26_OFFSET)
HI_SET_GET(hi_sc_stat26_cicom1_clk_state,cicom1_clk_state,HI_SC_STAT26_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT26_OFFSET)
HI_SET_GET(hi_sc_stat26_hdlc_clk_state,hdlc_clk_state,HI_SC_STAT26_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT26_OFFSET)
HI_SET_GET(hi_sc_stat26_reserved,reserved,HI_SC_STAT26_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT26_OFFSET)
HI_SET_GET(hi_sc_stat27_bbphy_slv_active,bbphy_slv_active,HI_SC_STAT27_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT27_OFFSET)
HI_SET_GET(hi_sc_stat27_amon_slv_active,amon_slv_active,HI_SC_STAT27_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT27_OFFSET)
HI_SET_GET(hi_sc_stat27_reserved_1,reserved_1,HI_SC_STAT27_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT27_OFFSET)
HI_SET_GET(hi_sc_stat27_glb_slv_active,glb_slv_active,HI_SC_STAT27_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT27_OFFSET)
HI_SET_GET(hi_sc_stat27_cpu_slv_active,cpu_slv_active,HI_SC_STAT27_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT27_OFFSET)
HI_SET_GET(hi_sc_stat27_reserved_0,reserved_0,HI_SC_STAT27_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT27_OFFSET)
HI_SET_GET(hi_sc_stat28_socapb_pslv_active,socapb_pslv_active,HI_SC_STAT28_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT28_OFFSET)
HI_SET_GET(hi_sc_stat29_x2h_peri_slv_active,x2h_peri_slv_active,HI_SC_STAT29_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT29_OFFSET)
HI_SET_GET(hi_sc_stat31_bbp_sys_1control,bbp_sys_1control,HI_SC_STAT31_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT31_OFFSET)
HI_SET_GET(hi_sc_stat32_lte2soc_tbd,lte2soc_tbd,HI_SC_STAT32_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT32_OFFSET)
HI_SET_GET(hi_sc_stat32_reserved,reserved,HI_SC_STAT32_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT32_OFFSET)
HI_SET_GET(hi_sc_stat33_reserved,reserved,HI_SC_STAT33_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT33_OFFSET)
HI_SET_GET(hi_sc_stat34_reserved,reserved,HI_SC_STAT34_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT34_OFFSET)
HI_SET_GET(hi_sc_stat35_func_mbist_done_2,func_mbist_done_2,HI_SC_STAT35_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT35_OFFSET)
HI_SET_GET(hi_sc_stat36_func_mbist_done_3,func_mbist_done_3,HI_SC_STAT36_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT36_OFFSET)
HI_SET_GET(hi_sc_stat37_func_mbist_done_4,func_mbist_done_4,HI_SC_STAT37_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT37_OFFSET)
HI_SET_GET(hi_sc_stat38_func_mbist_done_5,func_mbist_done_5,HI_SC_STAT38_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT38_OFFSET)
HI_SET_GET(hi_sc_stat39_func_mbist_done_6,func_mbist_done_6,HI_SC_STAT39_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT39_OFFSET)
HI_SET_GET(hi_sc_stat40_func_mbist_done_7,func_mbist_done_7,HI_SC_STAT40_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT40_OFFSET)
HI_SET_GET(hi_sc_stat41_bbphy_mst_err,bbphy_mst_err,HI_SC_STAT41_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT41_OFFSET)
HI_SET_GET(hi_sc_stat41_axi_amon_mst_err,axi_amon_mst_err,HI_SC_STAT41_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT41_OFFSET)
HI_SET_GET(hi_sc_stat41_reserved,reserved,HI_SC_STAT41_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT41_OFFSET)
HI_SET_GET(hi_sc_stat41_cpu_mst_err,cpu_mst_err,HI_SC_STAT41_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT41_OFFSET)
HI_SET_GET(hi_sc_stat41_glb_mst_err,glb_mst_err,HI_SC_STAT41_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT41_OFFSET)
HI_SET_GET(hi_sc_stat42_sysapb_psel_err,sysapb_psel_err,HI_SC_STAT42_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT42_OFFSET)
HI_SET_GET(hi_sc_stat42_socapb_psel_err,socapb_psel_err,HI_SC_STAT42_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT42_OFFSET)
HI_SET_GET(hi_sc_stat42_x2h_peri_mst_err,x2h_peri_mst_err,HI_SC_STAT42_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT42_OFFSET)
HI_SET_GET(hi_sc_stat42_reserved,reserved,HI_SC_STAT42_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT42_OFFSET)
HI_SET_GET(hi_sc_stat43_x2h_peri_addr_err,x2h_peri_addr_err,HI_SC_STAT43_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT43_OFFSET)
HI_SET_GET(hi_sc_stat44_reserved,reserved,HI_SC_STAT44_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT44_OFFSET)
HI_SET_GET(hi_sc_stat44_addr_err,addr_err,HI_SC_STAT44_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT44_OFFSET)
HI_SET_GET(hi_sc_stat45_reserved_1,reserved_1,HI_SC_STAT45_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT45_OFFSET)
HI_SET_GET(hi_sc_stat45_reserved_0,reserved_0,HI_SC_STAT45_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT45_OFFSET)
HI_SET_GET(hi_sc_stat46_addr_err,addr_err,HI_SC_STAT46_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT46_OFFSET)
HI_SET_GET(hi_sc_stat47_addr_err,addr_err,HI_SC_STAT47_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT47_OFFSET)
HI_SET_GET(hi_sc_stat48_addr_err,addr_err,HI_SC_STAT48_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT48_OFFSET)
HI_SET_GET(hi_sc_stat49_addr_err,addr_err,HI_SC_STAT49_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT49_OFFSET)
HI_SET_GET(hi_sc_stat50_addr_err,addr_err,HI_SC_STAT50_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT50_OFFSET)
HI_SET_GET(hi_sc_stat51_addr_err,addr_err,HI_SC_STAT51_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT51_OFFSET)
HI_SET_GET(hi_sc_stat52_addr_err,addr_err,HI_SC_STAT52_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT52_OFFSET)
HI_SET_GET(hi_sc_stat53_addr_err,addr_err,HI_SC_STAT53_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT53_OFFSET)
HI_SET_GET(hi_sc_stat54_addr_err,addr_err,HI_SC_STAT54_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT54_OFFSET)
HI_SET_GET(hi_sc_stat55_addr_err,addr_err,HI_SC_STAT55_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT55_OFFSET)
HI_SET_GET(hi_sc_stat56_addr_err,addr_err,HI_SC_STAT56_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT56_OFFSET)
HI_SET_GET(hi_sc_stat57_addr_err,addr_err,HI_SC_STAT57_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT57_OFFSET)
HI_SET_GET(hi_sc_stat58_addr_err,addr_err,HI_SC_STAT58_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT58_OFFSET)
HI_SET_GET(hi_sc_stat59_addr_err,addr_err,HI_SC_STAT59_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT59_OFFSET)
HI_SET_GET(hi_sc_stat60_addr_err,addr_err,HI_SC_STAT60_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT60_OFFSET)
HI_SET_GET(hi_sc_stat61_addr_err,addr_err,HI_SC_STAT61_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT61_OFFSET)
HI_SET_GET(hi_sc_stat62_addr_err,addr_err,HI_SC_STAT62_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT62_OFFSET)
HI_SET_GET(hi_sc_stat63_addr_err,addr_err,HI_SC_STAT63_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT63_OFFSET)
HI_SET_GET(hi_sc_stat64_addr_err,addr_err,HI_SC_STAT64_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT64_OFFSET)
HI_SET_GET(hi_sc_stat65_addr_err,addr_err,HI_SC_STAT65_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT65_OFFSET)
HI_SET_GET(hi_sc_stat66_addr_err,addr_err,HI_SC_STAT66_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT66_OFFSET)
HI_SET_GET(hi_sc_stat67_addr_err,addr_err,HI_SC_STAT67_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT67_OFFSET)
HI_SET_GET(hi_sc_stat68_addr_err,addr_err,HI_SC_STAT68_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT68_OFFSET)
HI_SET_GET(hi_sc_stat69_addr_err,addr_err,HI_SC_STAT69_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT69_OFFSET)
HI_SET_GET(hi_sc_stat70_pcie_gm_cmposer_lookup_err,pcie_gm_cmposer_lookup_err,HI_SC_STAT70_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT70_OFFSET)
HI_SET_GET(hi_sc_stat70_pcie_radmx_cmposer_lookup_err,pcie_radmx_cmposer_lookup_err,HI_SC_STAT70_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT70_OFFSET)
HI_SET_GET(hi_sc_stat70_pcie_pm_xtlh_block_tlp,pcie_pm_xtlh_block_tlp,HI_SC_STAT70_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT70_OFFSET)
HI_SET_GET(hi_sc_stat70_pcie_cfg_mem_space_en,pcie_cfg_mem_space_en,HI_SC_STAT70_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT70_OFFSET)
HI_SET_GET(hi_sc_stat70_pcie_cfg_rcb,pcie_cfg_rcb,HI_SC_STAT70_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT70_OFFSET)
HI_SET_GET(hi_sc_stat70_pcie_rdlh_link_up,pcie_rdlh_link_up,HI_SC_STAT70_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT70_OFFSET)
HI_SET_GET(hi_sc_stat70_pcie_pm_curnt_state,pcie_pm_curnt_state,HI_SC_STAT70_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT70_OFFSET)
HI_SET_GET(hi_sc_stat70_pcie_cfg_aer_rc_err_int,pcie_cfg_aer_rc_err_int,HI_SC_STAT70_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT70_OFFSET)
HI_SET_GET(hi_sc_stat70_pcie_cfg_aer_int_msg_num,pcie_cfg_aer_int_msg_num,HI_SC_STAT70_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT70_OFFSET)
HI_SET_GET(hi_sc_stat70_pcie_xmlh_link_up,pcie_xmlh_link_up,HI_SC_STAT70_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT70_OFFSET)
HI_SET_GET(hi_sc_stat70_pcie_wake,pcie_wake,HI_SC_STAT70_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT70_OFFSET)
HI_SET_GET(hi_sc_stat70_pcie_cfg_eml_control,pcie_cfg_eml_control,HI_SC_STAT70_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT70_OFFSET)
HI_SET_GET(hi_sc_stat70_pcie_hp_pme,pcie_hp_pme,HI_SC_STAT70_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT70_OFFSET)
HI_SET_GET(hi_sc_stat70_pcie_hp_int,pcie_hp_int,HI_SC_STAT70_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT70_OFFSET)
HI_SET_GET(hi_sc_stat70_pcie_hp_msi,pcie_hp_msi,HI_SC_STAT70_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT70_OFFSET)
HI_SET_GET(hi_sc_stat70_pcie_pm_status,pcie_pm_status,HI_SC_STAT70_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT70_OFFSET)
HI_SET_GET(hi_sc_stat70_pcie_ref_clk_req_n,pcie_ref_clk_req_n,HI_SC_STAT70_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT70_OFFSET)
HI_SET_GET(hi_sc_stat70_mac_phy_powerdown_p2_exit_reg,mac_phy_powerdown_p2_exit_reg,HI_SC_STAT70_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT70_OFFSET)
HI_SET_GET(hi_sc_stat70_radm_msg_req_id_low,radm_msg_req_id_low,HI_SC_STAT70_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT70_OFFSET)
HI_SET_GET(hi_sc_stat71_axi_parity_errs_reg,axi_parity_errs_reg,HI_SC_STAT71_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT71_OFFSET)
HI_SET_GET(hi_sc_stat71_app_parity_errs_reg,app_parity_errs_reg,HI_SC_STAT71_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT71_OFFSET)
HI_SET_GET(hi_sc_stat71_pm_linkst_in_l1,pm_linkst_in_l1,HI_SC_STAT71_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT71_OFFSET)
HI_SET_GET(hi_sc_stat71_pm_linkst_in_l2,pm_linkst_in_l2,HI_SC_STAT71_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT71_OFFSET)
HI_SET_GET(hi_sc_stat71_pm_linkst_l2_exit,pm_linkst_l2_exit,HI_SC_STAT71_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT71_OFFSET)
HI_SET_GET(hi_sc_stat71_mac_phy_power_down,mac_phy_power_down,HI_SC_STAT71_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT71_OFFSET)
HI_SET_GET(hi_sc_stat71_radm_correctabl_err_reg,radm_correctabl_err_reg,HI_SC_STAT71_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT71_OFFSET)
HI_SET_GET(hi_sc_stat71_radm_nonfatal_err_reg,radm_nonfatal_err_reg,HI_SC_STAT71_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT71_OFFSET)
HI_SET_GET(hi_sc_stat71_radm_fatal_err_reg,radm_fatal_err_reg,HI_SC_STAT71_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT71_OFFSET)
HI_SET_GET(hi_sc_stat71_radm_pm_to_pme_reg,radm_pm_to_pme_reg,HI_SC_STAT71_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT71_OFFSET)
HI_SET_GET(hi_sc_stat71_radm_pm_to_ack_reg,radm_pm_to_ack_reg,HI_SC_STAT71_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT71_OFFSET)
HI_SET_GET(hi_sc_stat71_radm_cpl_timeout_reg,radm_cpl_timeout_reg,HI_SC_STAT71_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT71_OFFSET)
HI_SET_GET(hi_sc_stat71_radm_msg_unlock_reg,radm_msg_unlock_reg,HI_SC_STAT71_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT71_OFFSET)
HI_SET_GET(hi_sc_stat71_cfg_pme_msi_reg,cfg_pme_msi_reg,HI_SC_STAT71_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT71_OFFSET)
HI_SET_GET(hi_sc_stat71_bridge_flush_not_reg,bridge_flush_not_reg,HI_SC_STAT71_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT71_OFFSET)
HI_SET_GET(hi_sc_stat71_link_req_rst_not_reg,link_req_rst_not_reg,HI_SC_STAT71_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT71_OFFSET)
HI_SET_GET(hi_sc_stat71_pcie_cfg_aer_rc_err_msi,pcie_cfg_aer_rc_err_msi,HI_SC_STAT71_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT71_OFFSET)
HI_SET_GET(hi_sc_stat71_pcie_cfg_sys_err_rc,pcie_cfg_sys_err_rc,HI_SC_STAT71_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT71_OFFSET)
HI_SET_GET(hi_sc_stat71_radm_msg_req_id_high,radm_msg_req_id_high,HI_SC_STAT71_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT71_OFFSET)
HI_SET_GET(hi_sc_stat72_pcie_stat2,pcie_stat2,HI_SC_STAT72_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT72_OFFSET)
HI_SET_GET(hi_sc_stat73_pcie_stat3,pcie_stat3,HI_SC_STAT73_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT73_OFFSET)
HI_SET_GET(hi_sc_stat74_ltssm_state,ltssm_state,HI_SC_STAT74_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT74_OFFSET)
HI_SET_GET(hi_sc_stat74_mac_phy_rate,mac_phy_rate,HI_SC_STAT74_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT74_OFFSET)
HI_SET_GET(hi_sc_stat74_pcie_slv_err_int,pcie_slv_err_int,HI_SC_STAT74_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT74_OFFSET)
HI_SET_GET(hi_sc_stat74_trgt_lookup_empty,trgt_lookup_empty,HI_SC_STAT74_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT74_OFFSET)
HI_SET_GET(hi_sc_stat74_trgt_cpl_timeout_reg,trgt_cpl_timeout_reg,HI_SC_STAT74_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT74_OFFSET)
HI_SET_GET(hi_sc_stat74_reserved,reserved,HI_SC_STAT74_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT74_OFFSET)
HI_SET_GET(hi_sc_stat75_reserved,reserved,HI_SC_STAT75_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT75_OFFSET)
HI_SET_GET(hi_sc_stat76_obff_dec_idle_intr_status,obff_dec_idle_intr_status,HI_SC_STAT76_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT76_OFFSET)
HI_SET_GET(hi_sc_stat76_obff_dec_obff_intr_status,obff_dec_obff_intr_status,HI_SC_STAT76_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT76_OFFSET)
HI_SET_GET(hi_sc_stat76_obff_dec_cpu_active_intr_status,obff_dec_cpu_active_intr_status,HI_SC_STAT76_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT76_OFFSET)
HI_SET_GET(hi_sc_stat76_obff_dec_err_intr_status,obff_dec_err_intr_status,HI_SC_STAT76_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT76_OFFSET)
HI_SET_GET(hi_sc_stat76_obff_msg_idle_intr_status,obff_msg_idle_intr_status,HI_SC_STAT76_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT76_OFFSET)
HI_SET_GET(hi_sc_stat76_obff_msg_obff_intr_status,obff_msg_obff_intr_status,HI_SC_STAT76_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT76_OFFSET)
HI_SET_GET(hi_sc_stat76_obff_msg_cpu_active_intr_status,obff_msg_cpu_active_intr_status,HI_SC_STAT76_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT76_OFFSET)
HI_SET_GET(hi_sc_stat76_radm_msg_ltr_intr_status,radm_msg_ltr_intr_status,HI_SC_STAT76_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT76_OFFSET)
HI_SET_GET(hi_sc_stat76_cfg_msi_mask_chg_intr_status,cfg_msi_mask_chg_intr_status,HI_SC_STAT76_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT76_OFFSET)
HI_SET_GET(hi_sc_stat76_radm_qoverflow_intr_status,radm_qoverflow_intr_status,HI_SC_STAT76_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT76_OFFSET)
HI_SET_GET(hi_sc_stat76_amba_ordr_intr_status,amba_ordr_intr_status,HI_SC_STAT76_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT76_OFFSET)
HI_SET_GET(hi_sc_stat76_pcie_clkreq_in_n_intr_status,pcie_clkreq_in_n_intr_status,HI_SC_STAT76_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT76_OFFSET)
HI_SET_GET(hi_sc_stat76_pcie_clkreq_in_n_inv_intr_status,pcie_clkreq_in_n_inv_intr_status,HI_SC_STAT76_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT76_OFFSET)
HI_SET_GET(hi_sc_stat76_radm_vendor_msg_intr_status,radm_vendor_msg_intr_status,HI_SC_STAT76_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT76_OFFSET)
HI_SET_GET(hi_sc_stat76_mac_phy_rxelecidle_disable,mac_phy_rxelecidle_disable,HI_SC_STAT76_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT76_OFFSET)
HI_SET_GET(hi_sc_stat76_mac_phy_txcommonmode_disable,mac_phy_txcommonmode_disable,HI_SC_STAT76_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT76_OFFSET)
HI_SET_GET(hi_sc_stat76_reserved,reserved,HI_SC_STAT76_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT76_OFFSET)
HI_SET_GET(hi_sc_stat77_obff_dec_owrd_cur_state,obff_dec_owrd_cur_state,HI_SC_STAT77_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT77_OFFSET)
HI_SET_GET(hi_sc_stat77_obff_enc_owre_cur_state,obff_enc_owre_cur_state,HI_SC_STAT77_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT77_OFFSET)
HI_SET_GET(hi_sc_stat77_cfg_obff_en,cfg_obff_en,HI_SC_STAT77_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT77_OFFSET)
HI_SET_GET(hi_sc_stat77_cfg_ltr_m_en,cfg_ltr_m_en,HI_SC_STAT77_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT77_OFFSET)
HI_SET_GET(hi_sc_stat77_cfg_pwr_budget_sel,cfg_pwr_budget_sel,HI_SC_STAT77_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT77_OFFSET)
HI_SET_GET(hi_sc_stat77_smlh_in_rl0s,smlh_in_rl0s,HI_SC_STAT77_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT77_OFFSET)
HI_SET_GET(hi_sc_stat77_radm_qoverflow,radm_qoverflow,HI_SC_STAT77_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT77_OFFSET)
HI_SET_GET(hi_sc_stat77_amba_ordr_mgr_wdt_int,amba_ordr_mgr_wdt_int,HI_SC_STAT77_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT77_OFFSET)
HI_SET_GET(hi_sc_stat77_reserved,reserved,HI_SC_STAT77_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT77_OFFSET)
HI_SET_GET(hi_sc_stat78_cfg_msi_mask,cfg_msi_mask,HI_SC_STAT78_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT78_OFFSET)
HI_SET_GET(hi_sc_stat79_cfg_ltr_max_latency,cfg_ltr_max_latency,HI_SC_STAT79_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT79_OFFSET)
HI_SET_GET(hi_sc_stat80_cm3_brch_stat,cm3_brch_stat,HI_SC_STAT80_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT80_OFFSET)
HI_SET_GET(hi_sc_stat80_cm3_core_halted,cm3_core_halted,HI_SC_STAT80_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT80_OFFSET)
HI_SET_GET(hi_sc_stat80_cm3_lockup,cm3_lockup,HI_SC_STAT80_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT80_OFFSET)
HI_SET_GET(hi_sc_stat80_reserved_1,reserved_1,HI_SC_STAT80_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT80_OFFSET)
HI_SET_GET(hi_sc_stat80_cm3_cur_int_priority,cm3_cur_int_priority,HI_SC_STAT80_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT80_OFFSET)
HI_SET_GET(hi_sc_stat80_reserved_0,reserved_0,HI_SC_STAT80_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT80_OFFSET)
HI_SET_GET(hi_sc_stat81_m3_stat1,m3_stat1,HI_SC_STAT81_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT81_OFFSET)
HI_SET_GET(hi_sc_stat82_m3_stat2,m3_stat2,HI_SC_STAT82_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT82_OFFSET)
HI_SET_GET(hi_sc_stat83_m3_stat3,m3_stat3,HI_SC_STAT83_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT83_OFFSET)
HI_SET_GET(hi_sc_stat84_m3_stat4,m3_stat4,HI_SC_STAT84_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT84_OFFSET)
HI_SET_GET(hi_sc_stat85_m3_stat5,m3_stat5,HI_SC_STAT85_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT85_OFFSET)
HI_SET_GET(hi_sc_stat85_reserved,reserved,HI_SC_STAT85_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT85_OFFSET)
HI_SET_GET(hi_sc_stat86_efuse_tsensor_trim,efuse_tsensor_trim,HI_SC_STAT86_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT86_OFFSET)
HI_SET_GET(hi_sc_stat87_app_ltr_latency,app_ltr_latency,HI_SC_STAT87_T,HI_SYSSC_AO_BASE_ADDR, HI_SC_STAT87_OFFSET)
HI_SET_GET(hi_version_id_version_id,version_id,HI_VERSION_ID_T,HI_SYSSC_AO_BASE_ADDR, HI_VERSION_ID_OFFSET)

#endif

#endif // __HI_SYSSC_AO_H__

