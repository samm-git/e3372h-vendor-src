/******************************************************************************/
/*  Copyright (C), 2007-2014, Hisilicon Technologies Co., Ltd. */
/******************************************************************************/
/* File name     : hi_pwrctrl.h */
/* Version       : 2.0 */
/* Author        : xxx*/
/* Created       : 2014-02-18*/
/* Last Modified : */
/* Description   :  The C union definition file for the module pwrctrl*/
/* Function List : */
/* History       : */
/* 1 Date        : */
/* Author        : xxx*/
/* Modification  : Create file */
/******************************************************************************/

#ifndef __HI_PWRCTRL_H__
#define __HI_PWRCTRL_H__

/*
 * Project: hi
 * Module : pwrctrl
 */

#ifndef HI_SET_GET
#define HI_SET_GET(a0,a1,a2,a3,a4)
#endif

/********************************************************************************/
/*    pwrctrl 寄存器偏移定义（项目名_模块名_寄存器名_OFFSET)        */
/********************************************************************************/
#define    HI_PWR_CTRL0_OFFSET                               (0xC00) /* 系统模式控制 */
#define    HI_PWR_CTRL1_OFFSET                               (0xC04) /* 中断模式控制 */
#define    HI_PWR_CTRL2_OFFSET                               (0xC08) /* TCXO 控制 */
#define    HI_PWR_CTRL3_OFFSET                               (0xC0C) /* PLL 控制 */
#define    HI_PWR_CTRL4_OFFSET                               (0xC10) /* ISO cell 隔离使能 */
#define    HI_PWR_CTRL5_OFFSET                               (0xC14) /* ISO cell 隔离禁止 */
#define    HI_PWR_CTRL6_OFFSET                               (0xC18) /* MTCMOS 上电控制 */
#define    HI_PWR_CTRL7_OFFSET                               (0xC1C) /* MTCMOS 下电控制 */
#define    HI_PWR_CTRL8_OFFSET                               (0xC20) /* CM3 上下电定时控制 */
#define    HI_PWR_CTRL9_OFFSET                               (0xC24) /* 外设上下电控制 */
#define    HI_PWR_CTRL10_OFFSET                              (0xC28) /* Retention memory 上电控制 */
#define    HI_PWR_CTRL11_OFFSET                              (0xC2C) /* Retention memory 下电控制 */
#define    HI_PWR_CTRL12_OFFSET                              (0xC30) /* Retention Meory 低功耗控制 */
#define    HI_PWR_CTRL13_OFFSET                              (0xC34) /* PMU控制寄存器 */
#define    HI_PWR_CTRL14_OFFSET                              (0xC38) /* sleephold控制 */
#define    HI_PWR_CTRL15_OFFSET                              (0xC3C) /* USB唤醒控制 */
#define    HI_PWR_CTRL16_OFFSET                              (0xC40) /* Cortex_M3 唤醒中断使能0 */
#define    HI_PWR_CTRL17_OFFSET                              (0xC44) /* Cortex_M3 唤醒中断使能1 */
#define    HI_PWR_CTRL18_OFFSET                              (0xC48) /* Cortex_M3 唤醒中断使能2 */
#define    HI_PWR_CTRL19_OFFSET                              (0xC4C) /* Cortex_M3 唤醒中断使能3 */
#define    HI_PWR_CTRL20_OFFSET                              (0xC50) /* 保留寄存器 */
#define    HI_PWR_CTRL21_OFFSET                              (0xC54) /* 保留寄存器 */
#define    HI_PWR_STAT0_OFFSET                               (0xE00) /* pwrctrl 状态 */
#define    HI_PWR_STAT1_OFFSET                               (0xE04) /* MTCMOS rdy状态 */
#define    HI_PWR_STAT2_OFFSET                               (0xE08) /* PLL/XTAL 计数器 */
#define    HI_PWR_STAT3_OFFSET                               (0xE0C) /* ISO cell 控制状态 */
#define    HI_PWR_STAT4_OFFSET                               (0xE10) /* MTCMOS 控制状态 */
#define    HI_PWR_STAT5_OFFSET                               (0xE14) /* retmem_ctrl */
#define    HI_PWR_STAT6_OFFSET                               (0xE18) /* 中断位置寄存器 */
#define    HI_PWR_STAT7_OFFSET                               (0xE1C) /* CM3 唤醒中断状态0 */
#define    HI_PWR_STAT8_OFFSET                               (0xE20) /* CM3 唤醒中断状态1 */
#define    HI_PWR_STAT9_OFFSET                               (0xE24) /* CM3 唤醒中断状态2 */
#define    HI_PWR_STAT10_OFFSET                              (0xE28) /* CM3 唤醒中断状态3 */
#define    HI_PWR_STAT11_OFFSET                              (0xE2C) /* 保留寄存器 */
#define    HI_PWR_STAT12_OFFSET                              (0xE30) /* 保留寄存器 */
#define    HI_CM3_PWRCTRL0_OFFSET                            (0x44000) /* ISO cell 隔离使能 */
#define    HI_CM3_PWRCTRL1_OFFSET                            (0x44004) /* ISO cell 隔离禁止 */
#define    HI_CM3_ISO_STAT_OFFSET                            (0x44008) /* CM3 ISO cell 控制状态 */
#define    HI_CM3_PWRCTRL2_OFFSET                            (0x44010) /* MTCMOS 上电控制 */
#define    HI_CM3_PWRCTRL3_OFFSET                            (0x44014) /* MTCMOS 下电控制 */
#define    HI_CM3_MTCMOS_STAT_OFFSET                         (0x44018) /* CM3 MTCMOS 控制状态 */
#define    HI_CM3_PWRCTRL4_OFFSET                            (0x44020) /* CM3功耗控制寄存器 */
#define    HI_CM3_PWRSTAT0_OFFSET                            (0x44040) /* PWRCTRL MTCMOS 控制状态 */
#define    HI_CM3_PWRSTAT1_OFFSET                            (0x44044) /* PWRCTRL ISO cell 控制状态 */
#define    HI_CM3_DEBUG_STAT_OFFSET                          (0x44048) /* debug 状态 */


#ifndef __ASSEMBLY__

/********************************************************************************/
/*    pwrctrl 寄存器定义（项目名_模块名_寄存器名_T)        */
/********************************************************************************/
typedef union
{
    struct
    {
        unsigned int    cpumode_ctrl               : 3; /* [2..0] CPU模式控制位。定义要求系统控制器进入的操作模式。000：sleep；001：doze；010：slow；100：normal。当cortex_M3从SLEEP状态唤醒至DOZE状态时，该寄存器会被硬件置为001当cortex_M3从SLEEP状态唤醒至SLOW状态时，该寄存器会被硬件置为010，其他情况下，软件可正常读写该寄存器 */
        unsigned int    a9_cpumode_status          : 4; /* [6..3] CPU模式状态位。定义返回系统控制器状态机当前的操作模式。0000：sleep；0001：doze；0010：slow；0011：XTAL CTL；0100：normal；0101：normal WFI；1000：deep sleep；0110：PLL CTL；0111：slow WFI；1001：SW from XTAL；1010：SW from PLL；1011：SW to XTAL；1110：SW to PLL；其他：保留。 */
        unsigned int    reserved_1                 : 3; /* [9..7] 保留 */
        unsigned int    cpu_dsleep_en              : 1; /* [10..10] CPU进入SLEEP模式时是否掉电。0：CPU不掉电，保持目前电压；1：CPU掉电，进入Deep Sleep模式。 */
        unsigned int    reserved_0                 : 11; /* [21..11] 保留 */
        unsigned int    doze_es_skip               : 1; /* [22..22] 当系统进入SLEEP模式是否进入doze状态。0：经DOZE状态进入SLEEP。1：不经DOZE状态直接进入SLEEP。 */
        unsigned int    doze_wk_skip               : 1; /* [23..23] 当系统从SLEEP模式唤醒后是否进入doze状态。0：唤醒后进入DOZE状态。1：唤醒后进入SLOW。 */
        unsigned int    cm3_sleephold_en           : 1; /* [24..24] cortex_M3的sleephold功能开关。0：不使用sleephold功能1：使用sleephold功能 */
        unsigned int    cm3_sleeping_en            : 1; /* [25..25] cortex_M3的sleeping信号是否被使用0：sleeping是否有效都不会影响内部状态机的跳转；1：sleeping信号有效时，内部状态机会跳转到WFI或者SLEEP等低功耗状态。 */
        unsigned int    cm3_sleepdeep_en           : 1; /* [26..26] cortex_M3的sleepdeep信号是否被使用0：sleepdeep是否有效都不会影响内部状态机的跳转；1：sleepdeep信号有效时，内部状态机会跳转到WFI或者SLEEP等低功耗状态。 */
        unsigned int    arm_dsleep_rst_en          : 1; /* [27..27] 控制进DSLEEP时是否复位Cortex_M3。0：进DSLEEP不复位Cortex_M31：进DSLEEP复位Cortex_M3 */
        unsigned int    usb_irq_glitch_free        : 1; /* [28..28] usb中断信号滤毛刺开关信号。0：不对usb中端信号过滤1：只有一个时钟周期的usb中断信号将被过滤掉。 */
        unsigned int    io_ret_value_sel           : 1; /* [29..29] io_retention锁ISO值、锁idle态值选择信号。                             0：锁idle态值                         1：锁ISO值 */
        unsigned int    arm_boot_type              : 1; /* [30..30] 系统 冷热启动标志寄存器。0：冷启动；1：热启动。当系统状态机进入DSLEEP状态后，硬件会将该bit置1，其余时刻，软件可读可写。. */
        unsigned int    sys_sleep_flag             : 1; /* [31..31] 系统 进入sleep模式标志寄存器。0：没有进入sleep模式；1：进入过sleep模式。当系统状态机进入SLEEP状态后，硬件会将该bit置1，其余时刻，软件可读可写。. */
    } bits;
    unsigned int    u32;
}HI_PWR_CTRL0_T;    /* 系统模式控制 */

typedef union
{
    struct
    {
        unsigned int    sys_intmod_en              : 1; /* [0..0] 系统中断模式使能位。0：关闭中断模式；1：当中断有效时进入中断模式。 */
        unsigned int    sys_intmod_ctrl            : 3; /* [3..1] 设置中断模式下系统工作模式。该值和cpumode_ctrl的值相或后作为中断发生后系统所处的工作模式。 */
        unsigned int    reserved_1                 : 1; /* [4..4] 保留 */
        unsigned int    sys_intmod_stat            : 1; /* [5..5] 中断模式控制/状态位。0：当前未处于中断模式；1：当前处于中断模式。当系统进入中断模式后，硬件会将该bit置1。同时，软件可以通过该位控制进入和退出中断模式。写0：表示退出中断模式写1：表示软件触发进入中断模式 */
        unsigned int    reserved_0                 : 26; /* [31..6] 保留 */
    } bits;
    unsigned int    u32;
}HI_PWR_CTRL1_T;    /* 中断模式控制 */

typedef union
{
    struct
    {
        unsigned int    tcxo_time                  : 16; /* [15..0] TCXO TimeOut Timer初值寄存器。当Timer从初值计数到16’hFFF时，会发出TimerOut信号 */
        unsigned int    soc_tcxo_over              : 1; /* [16..16] soc tcxo控制选择1：表示soc tcxo时钟由软件控制0：表示soc tcxo时钟由逻辑控制（状态机进入DOZE、SLEEP或DEEP SLEEP时逻辑会关闭soc tcxo时钟） */
        unsigned int    reserved_1                 : 1; /* [17..17]  */
        unsigned int    sleep_tcxo_off             : 1; /* [18..18] 当soc tcxo状态由状态机控制时（soc_tcxo_over为0），系统进入SLEEP状态时，是否关断SOC TCXO。0：不关断soc tcxo；1：关断soc tcxo。 */
        unsigned int    soc_tcxo_on                : 1; /* [19..19] soc tcxo时钟是否使能。0：表示soc tcxo时钟关闭1：表示soc tcxo时钟打开 */
        unsigned int    abb_ch0_tcxo_en            : 1; /* [20..20] gul tcxo软件使能，当gul_tcxo为soc tcxo时钟时（见CRG_CTRL12[SOCPLL_REFCLK_SEL]），该比特为1或者soc_tcxo_on为1时gul tcxo时钟有效；该比特为0且soc_tcxo_on也为0时gul tcxo时钟才被关闭；当lte_tcxo为soc tcxo时钟时，gul tcxo打开与否仅由该比特控制（1表示时钟打开，0表示时钟关闭）。 */
        unsigned int    abb_ch1_tcxo_en            : 1; /* [21..21] lte tcxo软件使能，当lte_tcxo为soc tcxo时钟时（见CRG_CTRL12[SOCPLL_REFCLK_SEL]），该比特为1或者soc_tcxo_on为1时lte tcxo时钟有效；该比特为0且soc_tcxo_on也为0时lte tcxo时钟才被关闭；当gul_tcxo为soc tcxo时钟时，lte tcxo打开与否仅由该比特控制（1表示时钟打开，0表示时钟关闭）。 */
        unsigned int    pmu_ch0_tcxo_en            : 1; /* [22..22] gul tcxo软件使能，当gul_tcxo为soc tcxo时钟时（见CRG_CTRL12[SOCPLL_REFCLK_SEL]），该比特为1或者soc_tcxo_on为1时gul tcxo时钟有效；该比特为0且soc_tcxo_on也为0时gul tcxo时钟才被关闭；当lte_tcxo为soc tcxo时钟时，gul tcxo打开与否仅由该比特控制（1表示时钟打开，0表示时钟关闭）。 */
        unsigned int    pmu_ch1_tcxo_en            : 1; /* [23..23] lte tcxo软件使能，当lte_tcxo为soc tcxo时钟时（见CRG_CTRL12[SOCPLL_REFCLK_SEL]），该比特为1或者soc_tcxo_on为1时lte tcxo时钟有效；该比特为0且soc_tcxo_on也为0时lte tcxo时钟才被关闭；当gul_tcxo为soc tcxo时钟时，lte tcxo打开与否仅由该比特控制（1表示时钟打开，0表示时钟关闭）。 */
        unsigned int    pmu_abb_tcxo_time          : 4; /* [27..24] 状态机控制下电时，给pmu的tcxo_en到给abb的tcxo_en的时钟间隔数。当pmu_abb_time_en=1时起作用。                                     0：间隔1个时钟cycle                         1：间隔2个时钟cycle                         。。。                                       15：间隔16个时钟cycle */
        unsigned int    pmu_abb_time_en            : 1; /* [28..28] 状态机控制下电时，给pmu的tcxo_en到给abb的tcxo_en的时钟间隔计数使能。                  0：计数禁止，pmu、abb的tcxo_en一起拉低      1：计数使能，时钟间隔见pmu_abb_tcxo_time */
        unsigned int    pmu_tcxo_en_stat           : 1; /* [29..29] 状态机控制时pmu tcxo开关是否使能。0：表示pmu tcxo开关关闭1：表示pmu tcxo开关打开 */
        unsigned int    reserved_0                 : 2; /* [31..30] 保留 */
    } bits;
    unsigned int    u32;
}HI_PWR_CTRL2_T;    /* TCXO 控制 */

typedef union
{
    struct
    {
        unsigned int    pll_time                   : 25; /* [24..0] CPU PLL TimeOut Timer初值寄存器。当Timer从初值计数到25’h1FF_FFF时，会发出TimerOut信号。 */
        unsigned int    pll_time_en                : 1; /* [25..25] 等待PLL锁定定时器使能。0：定时器不使能；1：定时器使能； */
        unsigned int    pll_status                 : 1; /* [26..26] PLL是否锁定状态信号。0：APP A9 PLL未锁定；1：APP A9 PLL锁定。 */
        unsigned int    reserved                   : 5; /* [31..27] 保留 */
    } bits;
    unsigned int    u32;
}HI_PWR_CTRL3_T;    /* PLL 控制 */

typedef union
{
    struct
    {
        unsigned int    appa9_iso_en               : 1; /* [0..0] APPA9输出方向ISO CELL软件使能信号。0：无影响1：表示输出被钳位 */
        unsigned int    moda9_iso_en               : 1; /* [1..1] MODA9输出方向ISO CELL软件使能信号。0：无影响1：表示输出被钳位 */
        unsigned int    reserved_3                 : 1; /* [2..2] 保留 */
        unsigned int    hisc_iso_en                : 1; /* [3..3] HSIC输出方向ISO CELL控制信号。0：无影响1：表示输出被钳位 */
        unsigned int    dsp0_iso_en                : 1; /* [4..4] DSP0输出方向ISO CELL控制信号。0：无影响1：表示输出被钳位 */
        unsigned int    hifi_iso_en                : 1; /* [5..5] HIFI输出方向ISO CELL控制信号。0：无影响1：表示输出被钳位 */
        unsigned int    lbbp_iso_en                : 1; /* [6..6] LBBP输出方向ISO CELL控制信号。0：无影响1：表示输出被钳位 */
        unsigned int    twbbp_iso_en               : 1; /* [7..7] TWBBP输出方向ISO CELL控制信号。0：无影响1：表示输出被钳位 */
        unsigned int    wbbp_iso_en                : 1; /* [8..8] WBBP输出方向ISO CELL控制信号。0：无影响1：表示输出被钳位 */
        unsigned int    g1bbp_iso_en               : 1; /* [9..9] G1BBP输出方向ISO CELL控制信号。0：无影响1：表示输出被钳位 */
        unsigned int    g2bbp_iso_en               : 1; /* [10..10] G2BBP输出方向ISO CELL控制信号。0：无影响1：表示输出被钳位 */
        unsigned int    irm_iso_en                 : 1; /* [11..11] IRM输出方向ISO CELL控制信号。0：无影响1：表示输出被钳位 */
        unsigned int    usb_iso_en                 : 1; /* [12..12] USB输出方向ISO CELL控制信号。0：无影响1：表示输出被钳位 */
        unsigned int    sd_iso_en                  : 1; /* [13..13] SD 输出方向ISO CELL控制信号。0：无影响1：表示输出被钳位 */
        unsigned int    reserved_2                 : 8; /* [21..14] 保留 */
        unsigned int    io_ddr_ret                 : 1; /* [22..22] DDR 的 IO 的retention控制信号。0：无影响1：ret信号使能 */
        unsigned int    reserved_1                 : 3; /* [25..23]  */
        unsigned int    io_ret_lp_ctrl             : 4; /* [29..26] IO的retention控制信号。对应bit为0：无影响对应bit为1：ret信号使能 */
        unsigned int    reserved_0                 : 2; /* [31..30] 保留 */
    } bits;
    unsigned int    u32;
}HI_PWR_CTRL4_T;    /* ISO cell 隔离使能 */

typedef union
{
    struct
    {
        unsigned int    appa9_iso_dis              : 1; /* [0..0] APPA9输出方向ISO CELL控制信号。0：无影响1：表示输出有效 */
        unsigned int    moda9_iso_dis              : 1; /* [1..1] MODA9输出方向ISO CELL控制信号。0：无影响1：表示输出有效 */
        unsigned int    reserved_3                 : 1; /* [2..2] 保留 */
        unsigned int    hisc_iso_dis               : 1; /* [3..3] HSIC输出方向ISO CELL控制信号。0：无影响1：表示输出有效 */
        unsigned int    dsp0_iso_dis               : 1; /* [4..4] DSP0输出方向ISO CELL控制信号。0：无影响1：表示输出有效 */
        unsigned int    hifi_iso_dis               : 1; /* [5..5] HIFI输出方向ISO CELL控制信号。0：无影响1：表示输出有效 */
        unsigned int    lbbp_iso_dis               : 1; /* [6..6] LBBP输出方向ISO CELL控制信号。0：无影响1：表示输出有效 */
        unsigned int    twbbp_iso_dis              : 1; /* [7..7] TWBBP输出方向ISO CELL控制信号。0：无影响1：表示输出有效 */
        unsigned int    wbbp_iso_dis               : 1; /* [8..8] WBBP输出方向ISO CELL控制信号。0：无影响1：表示输出有效 */
        unsigned int    g1bbp_iso_dis              : 1; /* [9..9] G1BBP输出方向ISO CELL控制信号。0：无影响1：表示输出有效 */
        unsigned int    g2bbp_iso_dis              : 1; /* [10..10] G2BBP输出方向ISO CELL控制信号。0：无影响1：表示输出有效 */
        unsigned int    irm_iso_dis                : 1; /* [11..11] IRM输出方向ISO CELL控制信号。0：无影响1：表示输出有效 */
        unsigned int    usb_iso_dis                : 1; /* [12..12] USB输出方向ISO CELL控制信号。0：无影响1：表示输出有效 */
        unsigned int    sd_iso_dis                 : 1; /* [13..13] SD 输出方向ISO CELL控制信号。0：无影响1：表示输出有效 */
        unsigned int    reserved_2                 : 8; /* [21..14] 保留 */
        unsigned int    io_ddr_ret                 : 1; /* [22..22] DDR 的 IO 的retention控制信号。0：无影响1：ret信号禁止 */
        unsigned int    reserved_1                 : 3; /* [25..23]  */
        unsigned int    io_ret_lp_ctrl             : 4; /* [29..26] IO的retention控制信号。对应bit为0：无影响对应bit为1：ret信号禁止 */
        unsigned int    reserved_0                 : 2; /* [31..30] 保留 */
    } bits;
    unsigned int    u32;
}HI_PWR_CTRL5_T;    /* ISO cell 隔离禁止 */

typedef union
{
    struct
    {
        unsigned int    appa9_mtcmos_en            : 1; /* [0..0] APP A9 MTCMOS CELL控制信号。0：无影响1：表示上电 */
        unsigned int    moda9_mtcmos_en            : 1; /* [1..1] MOD A9 MTCMOS CELL控制信号。0：无影响1：表示上电 */
        unsigned int    reserved_1                 : 1; /* [2..2]  */
        unsigned int    hisc_mtcmos_en             : 1; /* [3..3] HSIC MTCMOS CELL控制信号。0：无影响1：表示上电 */
        unsigned int    dsp0_mtcmos_en             : 1; /* [4..4] DSP0 MTCMOS CELL控制信号。0：无影响1：表示上电 */
        unsigned int    hifi_mtcmos_en             : 1; /* [5..5] HIFI MTCMOS CELL控制信号。0：无影响1：表示上电 */
        unsigned int    lbbp_mtcmos_en             : 1; /* [6..6] LBBP MTCMOS CELL控制信号。0：无影响1：表示上电 */
        unsigned int    twbbp_mtcmos_en            : 1; /* [7..7] TWBBP MTCMOS CELL控制信号。0：无影响1：表示上电 */
        unsigned int    wbbp_mtcmos_en             : 1; /* [8..8] WBBP MTCMOS CELL控制信号。0：无影响1：表示上电 */
        unsigned int    g1bbp_mtcmos_en            : 1; /* [9..9] G1BBP MTCMOS CELL控制信号。0：无影响1：表示上电 */
        unsigned int    g2bbp_mtcmos_en            : 1; /* [10..10] G2BBP MTCMOS CELL控制信号。0：无影响1：表示上电 */
        unsigned int    irm_mtcmos_en              : 1; /* [11..11] IRM MTCMOS CELL控制信号。0：无影响1：表示上电 */
        unsigned int    usb_mtcmos_en              : 1; /* [12..12] USB MTCMOS CELL控制信号。0：无影响1：表示上电 */
        unsigned int    sd_mtcmos_en               : 1; /* [13..13] SD MTCMOS CELL控制信号。0：无影响1：表示上电 */
        unsigned int    reserved_0                 : 18; /* [31..14] 保留 */
    } bits;
    unsigned int    u32;
}HI_PWR_CTRL6_T;    /* MTCMOS 上电控制 */

typedef union
{
    struct
    {
        unsigned int    appa9_mtcmos_dis           : 1; /* [0..0] APP A9 MTCMOS CELL控制信号。0：无影响1：表示断电 */
        unsigned int    moda9_mtcmos_dis           : 1; /* [1..1] MOD A9 MTCMOS CELL控制信号。0：无影响1：表示断电 */
        unsigned int    reserved_1                 : 1; /* [2..2]  */
        unsigned int    hisc_mtcmos_dis            : 1; /* [3..3] HSIC MTCMOS CELL控制信号。0：无影响1：表示断电 */
        unsigned int    dsp0_mtcmos_dis            : 1; /* [4..4] DSP0 MTCMOS CELL控制信号。0：无影响1：表示断电 */
        unsigned int    hifi_mtcmos_dis            : 1; /* [5..5] HIFI MTCMOS CELL控制信号。0：无影响1：表示断电 */
        unsigned int    lbbp_mtcmos_dis            : 1; /* [6..6] LBBP MTCMOS CELL控制信号。0：无影响1：表示断电 */
        unsigned int    twbbp_mtcmos_dis           : 1; /* [7..7] TWBBP MTCMOS CELL控制信号。0：无影响1：表示断电 */
        unsigned int    wbbp_mtcmos_dis            : 1; /* [8..8] WBBP MTCMOS CELL控制信号。0：无影响1：表示断电 */
        unsigned int    g1bbp_mtcmos_dis           : 1; /* [9..9] G1BBP MTCMOS CELL控制信号。0：无影响1：表示断电 */
        unsigned int    g2bbp_mtcmos_dis           : 1; /* [10..10] G2BBP MTCMOS CELL控制信号。0：无影响1：表示断电 */
        unsigned int    irm_mtcmos_dis             : 1; /* [11..11] IRM MTCMOS CELL控制信号。0：无影响1：表示断电 */
        unsigned int    usb_mtcmos_dis             : 1; /* [12..12] USB MTCMOS CELL控制信号。0：无影响1：表示断电 */
        unsigned int    sd_mtcmos_dis              : 1; /* [13..13] SD MTCMOS CELL控制信号。0：无影响1：表示断电 */
        unsigned int    reserved_0                 : 18; /* [31..14] 保留 */
    } bits;
    unsigned int    u32;
}HI_PWR_CTRL7_T;    /* MTCMOS 下电控制 */

typedef union
{
    struct
    {
        unsigned int    cpu_pwrup_time             : 12; /* [11..0] Cortex_M3 上电定时器初值。当Timer从初值计数到12’hFFF时，会发出TimerOut信号。 */
        unsigned int    reserved_1                 : 4; /* [15..12] 保留 */
        unsigned int    cpu_pwrdn_time             : 12; /* [27..16] Cortex_M3 下电定时器初值。当Timer从初值计数到12’hFFF时，会发出TimerOut信号。 */
        unsigned int    cpu_pwrtime_en             : 1; /* [28..28] Cortex_M3 上下电定时器使能。0：Cortex_M3 上下电是否完成不以定时器timeout为标志；1：Cortex_M3 上下电是否完成靠定时器timeout指示。 */
        unsigned int    reserved_0                 : 3; /* [31..29] 保留 */
    } bits;
    unsigned int    u32;
}HI_PWR_CTRL8_T;    /* CM3 上下电定时控制 */

typedef union
{
    struct
    {
        unsigned int    peri_pwrup_time            : 12; /* [11..0] 普通外设区上电定时器初值。当Timer从初值计数到12’hFFF时，会发出TimerOut信号。 */
        unsigned int    reserved_1                 : 4; /* [15..12] 保留 */
        unsigned int    peri_pwrdn_time            : 12; /* [27..16] 普通外设区下电定时器初值。当Timer从初值计数到12’hFFF时，会发出TimerOut信号。 */
        unsigned int    peri_pwrtime_en            : 1; /* [28..28] 等待普通外设区上下电定时器使能。0：普通外设区上下电是否完成不以定时器timeout为标志；1：普通外设区上下电是否完成靠定时器timeout指示。 */
        unsigned int    dsleep_peri_pwrdn          : 1; /* [29..29] 普通外设区在DSLEEP时供电模式。0：普通外设区不掉电；1：普通外设区掉电。 */
        unsigned int    sleep_periclk_en           : 1; /* [30..30] 当Cortex_M3睡眠时，外设时钟的状态控制。0：外设时钟关闭1：外设时钟不关闭 */
        unsigned int    reserved_0                 : 1; /* [31..31] 保留 */
    } bits;
    unsigned int    u32;
}HI_PWR_CTRL9_T;    /* 外设上下电控制 */

typedef union
{
    struct
    {
        unsigned int    retmem_nor_time            : 25; /* [24..0] Retention Memory区上电定时器初值。当Timer从初值计数到25’h1FF_FFF时，会发出TimerOut信号。建议仿真时设置为一个较大的值。否则会因为计数使得仿真时间很长。 */
        unsigned int    reserved                   : 7; /* [31..25] 保留 */
    } bits;
    unsigned int    u32;
}HI_PWR_CTRL10_T;    /* Retention memory 上电控制 */

typedef union
{
    struct
    {
        unsigned int    retmem_ret_time            : 25; /* [24..0] Retention Memory区下电定时器等待时间。当Timer从初值计数到25’h1FF_FFF时，会发出TimerOut信号。建议仿真时设置为一个较大的值。否则会因为计数使得仿真时间很长。 */
        unsigned int    reserved                   : 7; /* [31..25] 保留 */
    } bits;
    unsigned int    u32;
}HI_PWR_CTRL11_T;    /* Retention memory 下电控制 */

typedef union
{
    struct
    {
        unsigned int    ema                        : 3; /* [2..0] memory读速率控制，随此值的增大，memory读延时增大。 */
        unsigned int    reserved_3                 : 1; /* [3..3] 保留 */
        unsigned int    emaw                       : 2; /* [5..4] memory写速率控制，随此值的增大，memory写延时增大。 */
        unsigned int    emas                       : 1; /* [6..6] 对memory的emas控制 */
        unsigned int    reserved_2                 : 3; /* [9..7] 保留 */
        unsigned int    retmem_ret1n_cfg           : 1; /* [10..10] retention memory 进入省电模式后RET1N 信号控制。 */
        unsigned int    retmem_ret2n_cfg           : 1; /* [11..11] retention memory 进入省电模式后RET2N 信号控制。 */
        unsigned int    retmem_pgen_cfg            : 1; /* [12..12] retention memory 进入省电模式后PGEN 信号控制。 */
        unsigned int    reserved_1                 : 3; /* [15..13] 保留 */
        unsigned int    retmem_sw_lp_ctrl          : 1; /* [16..16] 软件对retention Memory区的电源管理由软件控制使能，此bit只能控制APP 或Modem A9 处于工作状态时AXI Memory区的供电模式。写1：进入LS/DS/SD状态写0：返回Normal状态 */
        unsigned int    dsleep_retmem_ret          : 1; /* [17..17] retention Memory区在状态机进入 sleep时供电模式。0：retention Memory区保持 Normal状态；1：retention Memory区进入省电模式。 */
        unsigned int    reserved_0                 : 14; /* [31..18] 保留 */
    } bits;
    unsigned int    u32;
}HI_PWR_CTRL12_T;    /* Retention Meory 低功耗控制 */

typedef union
{
    struct
    {
        unsigned int    pmu_pwrup_time             : 12; /* [11..0] PMU上电定时器初值。PMU掉电使能（dsleep_pmu_pwrdn=1）时，当系统从dsleep唤醒，此计数器开始计数。当Timer从初值计数到12’hFFF时，会发出TimerOut信号。 */
        unsigned int    reserved_1                 : 4; /* [15..12]  */
        unsigned int    dsleep_pmu_pwrdn           : 1; /* [16..16] PMU在DSLEEP时供电模式。0：PMU不掉电；1：PMU掉电。 */
        unsigned int    sleep_pmuclk_en            : 1; /* [17..17] 当Cortex_M3睡眠时，PMU时钟的状态控制。0：PMU时钟关闭1：PMU时钟不关闭 */
        unsigned int    reserved_0                 : 14; /* [31..18]  */
    } bits;
    unsigned int    u32;
}HI_PWR_CTRL13_T;    /* PMU控制寄存器 */

typedef union
{
    struct
    {
        unsigned int    sleep_hold_cycle           : 16; /* [15..0] 表示sleephold需要延迟的cycle数。 */
        unsigned int    sleephole_cnt              : 16; /* [31..16] 表示sleephold计数值，即当前已经延迟的cycle数。 */
    } bits;
    unsigned int    u32;
}HI_PWR_CTRL14_T;    /* sleephold控制 */

typedef union
{
    struct
    {
        unsigned int    moda9_usbwkup_en           : 1; /* [0..0] USB 唤醒 Modem A9使能信号，高有效。 */
        unsigned int    appa9_usbwkup_en           : 1; /* [1..1] USB 唤醒 APP A9使能信号，高有效。 */
        unsigned int    a9_otgphy_wkup_en          : 8; /* [9..2] utmiotg_iddig 0 唤醒  A9使能信号，高有效。 */
        unsigned int    a9_otgsuspend_wkup_en      : 3; /* [12..10] utmiotg_iddig 1 唤醒  A9使能信号，高有效。 */
        unsigned int    otgawson_wpc_clk_en        : 1; /* [13..13] utmiotg_bvalid 0 唤醒  A9使能信号，高有效。 */
        unsigned int    reserved_1                 : 2; /* [15..14] 保留 */
        unsigned int    sleep_usbwkup_en           : 1; /* [16..16] USB PHY中断作为从sleep唤醒的中断源使能。         0：USB PHY 中断唤醒sleep禁止                       1：USB PHY 中断唤醒sleep使能 */
        unsigned int    reserved_0                 : 15; /* [31..17] 保留 */
    } bits;
    unsigned int    u32;
}HI_PWR_CTRL15_T;    /* USB唤醒控制 */

typedef union
{
    struct
    {
        unsigned int    cm3_irqs_en_31_0           : 32; /* [31..0] Cortex_M3 唤醒中断使能bit31-bit0。 */
    } bits;
    unsigned int    u32;
}HI_PWR_CTRL16_T;    /* Cortex_M3 唤醒中断使能0 */

typedef union
{
    struct
    {
        unsigned int    cm3_irqs_en_63_32          : 32; /* [31..0] Cortex_M3 唤醒中断使能bit63-bit32。 */
    } bits;
    unsigned int    u32;
}HI_PWR_CTRL17_T;    /* Cortex_M3 唤醒中断使能1 */

typedef union
{
    struct
    {
        unsigned int    cm3_irqs_en_95_64          : 32; /* [31..0] Cortex_M3 唤醒中断使能bit95-bit64。 */
    } bits;
    unsigned int    u32;
}HI_PWR_CTRL18_T;    /* Cortex_M3 唤醒中断使能2 */

typedef union
{
    struct
    {
        unsigned int    cm3_irqs_en_127_96         : 32; /* [31..0] Cortex_M3 唤醒中断使能bit127-bit96。 */
    } bits;
    unsigned int    u32;
}HI_PWR_CTRL19_T;    /* Cortex_M3 唤醒中断使能3 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 32; /* [31..0]  */
    } bits;
    unsigned int    u32;
}HI_PWR_CTRL20_T;    /* 保留寄存器 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 32; /* [31..0]  */
    } bits;
    unsigned int    u32;
}HI_PWR_CTRL21_T;    /* 保留寄存器 */

typedef union
{
    struct
    {
        unsigned int    cpu_volt_fsm               : 4; /* [3..0] 电压控制状态机的状态。4'h0：电压正常4'h1：CPU输出钳位4'h2：CPU掉电4'h3：外设掉电4'h4：外设上电4'h5：CPU上电4'h6：CPU输出钳位撤离4'h7：CPU和外设都掉电4'h8：RETMEM请求进入低功耗状态4'h9：RETMEM请求进入正常状态其他：无意义 */
        unsigned int    cpu_pwrup_timeout          : 1; /* [4..4] 等待Modem A9上电定时器超时。0：定时器没有超时；1：定时器超时； */
        unsigned int    cpu_pwrdn_timeout          : 1; /* [5..5] 等待Modem A9掉电定时器超时。0：定时器没有超时；1：定时器超时； */
        unsigned int    peri_pwrup_timeout         : 1; /* [6..6] 等待普通外设上电定时器超时。0：定时器没有超时；1：定时器超时； */
        unsigned int    peri_pwrdn_timeout         : 1; /* [7..7] 等待普通外设掉电定时器超时。0：定时器没有超时；1：定时器超时； */
        unsigned int    retmem_nor_timeout         : 1; /* [8..8] 等待RetMem区电压进入normal定时器超时。0：定时器没有超时；1：定时器超时； */
        unsigned int    retmem_ret_timeout         : 1; /* [9..9] 等待RetMem区电压进入retention定时器超时。0：定时器没有超时；1：定时器超时； */
        unsigned int    moda9_standbywfi           : 1; /* [10..10] Modem A9 standbywfi信号的值1：表示当前MODA9处于standbywfi状态0：表示MODA9不在standbywfi状态 */
        unsigned int    moda9_standbywfe           : 1; /* [11..11] Modem A9 standbywfe信号的值1：表示当前MODA9处于standbywfe状态0：表示MODA9不在standbywfe状态 */
        unsigned int    appa9_standbywfi           : 1; /* [12..12] App A9 standbywfi信号的值1：表示当前APPA9处于standbywfi状态0：表示APPA9不在standbywfi状态 */
        unsigned int    appa9_standbywfe           : 1; /* [13..13] App A9 standbywfe信号的值1：表示当前APPA9处于standbywfe状态0：表示APPA9不在standbywfe状态 */
        unsigned int    pmu_pwrup_timeout          : 1; /* [14..14] 等待PMU上电定时器超时。0：定时器没有超时；1：定时器超时； */
        unsigned int    reserved_1                 : 5; /* [19..15] 保留 */
        unsigned int    cm3_sleeping               : 1; /* [20..20] 表示Cortex_M3输出的sleeping信号值 */
        unsigned int    cm3_sleepdeep              : 1; /* [21..21] 表示Cortex_M3输出的sleepdeep信号值 */
        unsigned int    cm3_sleephold_ack_n        : 1; /* [22..22] 表示Cortex_M3输出的sleephold_ack_n信号值0：表示Cortex_M3接受了pwrctrl的sleephold_req信号1：表示Cortex_M3没有接受pwrctrl的sleephold_req信号 */
        unsigned int    reserved_0                 : 9; /* [31..23] 保留 */
    } bits;
    unsigned int    u32;
}HI_PWR_STAT0_T;    /* pwrctrl 状态 */

typedef union
{
    struct
    {
        unsigned int    appa9_mtcmos_rdy           : 1; /* [0..0] APPA9 CORE 电压域MTCOMS开关完成，电源稳定信号。1：表示有电0：表示掉电 */
        unsigned int    moda9_mtcmos_rdy           : 1; /* [1..1] MODA9 CORE 电压域MTCOMS开关完成，电源稳定信号。1：表示有电0：表示掉电 */
        unsigned int    peri_mtcmos_rdy            : 1; /* [2..2] SOC PERI 电压域MTCOMS开关完成，电源稳定信号。1：表示有电0：表示掉电 */
        unsigned int    hisc_mtcmos_rdy            : 1; /* [3..3] HISC 电压域MTCOMS开关完成，电源稳定信号。1：表示有电0：表示掉电 */
        unsigned int    dsp0_mtcmos_rdy            : 1; /* [4..4] DSP0 电压域MTCOMS开关完成，电源稳定信号。1：表示有电0：表示掉电 */
        unsigned int    hifi_mtcmos_rdy            : 1; /* [5..5] HIFI CORE 电压域MTCOMS开关完成，电源稳定信号。1：表示有电0：表示掉电 */
        unsigned int    lbbp_mtcmos_rdy            : 1; /* [6..6] LBBP 电压域MTCOMS开关完成，电源稳定信号。1：表示有电0：表示掉电 */
        unsigned int    twbbp_mtcmos_rdy           : 1; /* [7..7] TWBBP 电压域MTCOMS开关完成，电源稳定信号。1：表示有电0：表示掉电 */
        unsigned int    wbbp_mtcmos_rdy            : 1; /* [8..8] WBBP 电压域MTCOMS开关完成，电源稳定信号。1：表示有电0：表示掉电 */
        unsigned int    g1bbp_mtcmos_rdy           : 1; /* [9..9] G1BBP 电压域MTCOMS开关完成，电源稳定信号。1：表示有电0：表示掉电 */
        unsigned int    g2bbp_mtcmos_rdy           : 1; /* [10..10] G2BBP 电压域MTCOMS开关完成，电源稳定信号。1：表示有电0：表示掉电 */
        unsigned int    irm_mtcmos_rdy             : 1; /* [11..11] IRM 电压域MTCOMS开关完成，电源稳定信号。1：表示有电0：表示掉电 */
        unsigned int    usb_mtcmos_rdy             : 1; /* [12..12] USB 电压域MTCOMS开关完成，电源稳定信号。1：表示有电0：表示掉电 */
        unsigned int    sd_mtcmos_rdy              : 1; /* [13..13] SD 电压域MTCOMS开关完成，电源稳定信号。1：表示有电0：表示掉电 */
        unsigned int    reserved                   : 18; /* [31..14] 保留 */
    } bits;
    unsigned int    u32;
}HI_PWR_STAT1_T;    /* MTCMOS rdy状态 */

typedef union
{
    struct
    {
        unsigned int    sccnt_data                 : 25; /* [24..0] PLL/XTAL 计数器。 */
        unsigned int    reserved                   : 7; /* [31..25] 保留 */
    } bits;
    unsigned int    u32;
}HI_PWR_STAT2_T;    /* PLL/XTAL 计数器 */

typedef union
{
    struct
    {
        unsigned int    appa9_iso_ctrl             : 1; /* [0..0] APPA9输出方向ISO CELL控制信号。0：表示输出有效1：表示输出被钳位 */
        unsigned int    moda9_iso_ctrl             : 1; /* [1..1] MODA9输出方向ISO CELL控制信号。0：表示输出有效1：表示输出被钳位 */
        unsigned int    peri_iso_ctrl              : 1; /* [2..2] SOC_PERI输出方向ISO CELL控制信号。0：表示输出有效1：表示输出被钳位 */
        unsigned int    hisc_iso_ctrl              : 1; /* [3..3] HSIC输出方向ISO CELL控制信号。0：表示输出有效1：表示输出被钳位 */
        unsigned int    dsp0_iso_ctrl              : 1; /* [4..4] DSP0输出方向ISO CELL控制信号。0：表示输出有效1：表示输出被钳位 */
        unsigned int    hifi_iso_ctrl              : 1; /* [5..5] HIFI输出方向ISO CELL控制信号。0：表示输出有效1：表示输出被钳位 */
        unsigned int    lbbp_iso_ctrl              : 1; /* [6..6] LBBP输出方向ISO CELL控制信号。0：表示输出有效1：表示输出被钳位 */
        unsigned int    twbbp_iso_ctrl             : 1; /* [7..7] TWBBP输出方向ISO CELL控制信号。0：表示输出有效1：表示输出被钳位 */
        unsigned int    wbbp_iso_ctrl              : 1; /* [8..8] WBBP输出方向ISO CELL控制信号。0：表示输出有效1：表示输出被钳位 */
        unsigned int    g1bbp_iso_ctrl             : 1; /* [9..9] G1BBP输出方向ISO CELL控制信号。0：表示输出有效1：表示输出被钳位 */
        unsigned int    g2bbp_iso_ctrl             : 1; /* [10..10] G2BBP输出方向ISO CELL控制信号。0：表示输出有效1：表示输出被钳位 */
        unsigned int    irm_iso_ctrl               : 1; /* [11..11] IRM输出方向ISO CELL控制信号。0：表示输出有效1：表示输出被钳位 */
        unsigned int    usb_iso_ctrl               : 1; /* [12..12] USB输出方向ISO CELL控制信号。0：表示输出有效1：表示输出被钳位 */
        unsigned int    sd_iso_ctrl                : 1; /* [13..13] SD 输出方向ISO CELL控制信号。0：表示输出有效1：表示输出被钳位 */
        unsigned int    reserved_2                 : 8; /* [21..14] 保留 */
        unsigned int    io_ddr_ret                 : 1; /* [22..22] DDR 的 IO 的retention控制信号。0：表示ret信号禁止1：表示ret信号使能 */
        unsigned int    reserved_1                 : 3; /* [25..23]  */
        unsigned int    io_ret_lp_ctrl             : 4; /* [29..26] IO的retention控制信号。对应bit为0：ret信号禁止对应bit为1：ret信号使能 */
        unsigned int    pmu_iso_ctrl               : 1; /* [30..30] PMU 输出方向ISO CELL控制信号。0：表示输出有效1：表示输出被钳位 */
        unsigned int    reserved_0                 : 1; /* [31..31] 保留 */
    } bits;
    unsigned int    u32;
}HI_PWR_STAT3_T;    /* ISO cell 控制状态 */

typedef union
{
    struct
    {
        unsigned int    appa9_mtcmos_ctrl          : 1; /* [0..0] APP A9 MTCMOS CELL控制信号。0：表示断电1：表示上电 */
        unsigned int    moda9_mtcmos_ctrl          : 1; /* [1..1] MOD A9 MTCMOS CELL控制信号。0：表示断电1：表示上电 */
        unsigned int    peri_mtcmos_ctrl           : 1; /* [2..2] SOC_PERI MTCMOS CELL控制信号。0：表示断电1：表示上电 */
        unsigned int    hisc_mtcmos_ctrl           : 1; /* [3..3] HSIC MTCMOS CELL控制信号。0：表示断电1：表示上电 */
        unsigned int    dsp0_mtcmos_ctrl           : 1; /* [4..4] DSP0 MTCMOS CELL控制信号。0：表示断电1：表示上电 */
        unsigned int    hifi_mtcmos_ctrl           : 1; /* [5..5] HIFI MTCMOS CELL控制信号。0：表示断电1：表示上电 */
        unsigned int    lbbp_mtcmos_ctrl           : 1; /* [6..6] LBBP MTCMOS CELL控制信号。0：表示断电1：表示上电 */
        unsigned int    twbbp_mtcmos_ctrl          : 1; /* [7..7] TWBBP MTCMOS CELL控制信号。0：表示断电1：表示上电 */
        unsigned int    wbbp_mtcmos_ctrl           : 1; /* [8..8] WBBP MTCMOS CELL控制信号。0：表示断电1：表示上电 */
        unsigned int    g1bbp_mtcmos_ctrl          : 1; /* [9..9] G1BBP MTCMOS CELL控制信号。0：表示断电1：表示上电 */
        unsigned int    g2bbp_mtcmos_ctrl          : 1; /* [10..10] G2BBP MTCMOS CELL控制信号。0：表示断电1：表示上电 */
        unsigned int    irm_mtcmos_ctrl            : 1; /* [11..11] IRM MTCMOS CELL控制信号。0：表示断电1：表示上电 */
        unsigned int    usb_mtcmos_ctrl            : 1; /* [12..12] USB MTCMOS CELL控制信号。0：表示断电1：表示上电 */
        unsigned int    sd_mtcmos_ctrl             : 1; /* [13..13] SD MTCMOS CELL控制信号。0：表示断电1：表示上电 */
        unsigned int    reserved_1                 : 16; /* [29..14] 保留 */
        unsigned int    pmu_pwr_on                 : 1; /* [30..30] PMU MTCMOS CELL控制信号。0：表示断电1：表示上电 */
        unsigned int    reserved_0                 : 1; /* [31..31] 保留 */
    } bits;
    unsigned int    u32;
}HI_PWR_STAT4_T;    /* MTCMOS 控制状态 */

typedef union
{
    struct
    {
        unsigned int    retmem_ctrl                : 16; /* [15..0]  */
        unsigned int    reserved                   : 16; /* [31..16] 保留 */
    } bits;
    unsigned int    u32;
}HI_PWR_STAT5_T;    /* retmem_ctrl */

typedef union
{
    struct
    {
        unsigned int    cm3_wkup_stat0             : 1; /* [0..0] Cortex_M3 唤醒中断位置。0：Cortex_M3 唤醒中断0 中无中断产生1：Cortex_M3 唤醒中断0 中有中断产生 */
        unsigned int    cm3_wkup_stat1             : 1; /* [1..1] Cortex_M3 唤醒中断位置。0：Cortex_M3 唤醒中断1 中无中断产生1：Cortex_M3 唤醒中断1 中有中断产生 */
        unsigned int    cm3_wkup_stat2             : 1; /* [2..2] Cortex_M3 唤醒中断位置。0：Cortex_M3 唤醒中断2 中无中断产生1：Cortex_M3 唤醒中断2 中有中断产生 */
        unsigned int    cm3_wkup_stat3             : 1; /* [3..3] Cortex_M3 唤醒中断位置。0：Cortex_M3 唤醒中断3 中无中断产生1：Cortex_M3 唤醒中断3 中有中断产生 */
        unsigned int    reserved                   : 28; /* [31..4] 保留 */
    } bits;
    unsigned int    u32;
}HI_PWR_STAT6_T;    /* 中断位置寄存器 */

typedef union
{
    struct
    {
        unsigned int    app_wkup_irq0              : 32; /* [31..0]  */
    } bits;
    unsigned int    u32;
}HI_PWR_STAT7_T;    /* CM3 唤醒中断状态0 */

typedef union
{
    struct
    {
        unsigned int    app_wkup_irq1              : 32; /* [31..0]  */
    } bits;
    unsigned int    u32;
}HI_PWR_STAT8_T;    /* CM3 唤醒中断状态1 */

typedef union
{
    struct
    {
        unsigned int    app_wkup_irq2              : 32; /* [31..0]  */
    } bits;
    unsigned int    u32;
}HI_PWR_STAT9_T;    /* CM3 唤醒中断状态2 */

typedef union
{
    struct
    {
        unsigned int    app_wkup_irq3              : 32; /* [31..0]  */
    } bits;
    unsigned int    u32;
}HI_PWR_STAT10_T;    /* CM3 唤醒中断状态3 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 32; /* [31..0]  */
    } bits;
    unsigned int    u32;
}HI_PWR_STAT11_T;    /* 保留寄存器 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 32; /* [31..0]  */
    } bits;
    unsigned int    u32;
}HI_PWR_STAT12_T;    /* 保留寄存器 */

typedef union
{
    struct
    {
        unsigned int    appa9_iso_en               : 1; /* [0..0] APPA9输出方向ISO CELL软件使能信号。0：无影响1：表示输出被钳位 */
        unsigned int    moda9_iso_en               : 1; /* [1..1] MODA9输出方向ISO CELL软件使能信号。0：无影响1：表示输出被钳位 */
        unsigned int    peri_iso_en                : 1; /* [2..2] SOC_PERI 输出方向ISO CELL控制信号。0：无影响1：表示输出被钳位 */
        unsigned int    hisc_iso_en                : 1; /* [3..3] HSIC输出方向ISO CELL控制信号。0：无影响1：表示输出被钳位 */
        unsigned int    dsp0_iso_en                : 1; /* [4..4] DSP0输出方向ISO CELL控制信号。0：无影响1：表示输出被钳位 */
        unsigned int    hifi_iso_en                : 1; /* [5..5] HIFI输出方向ISO CELL控制信号。0：无影响1：表示输出被钳位 */
        unsigned int    lbbp_iso_en                : 1; /* [6..6] LBBP输出方向ISO CELL控制信号。0：无影响1：表示输出被钳位 */
        unsigned int    twbbp_iso_en               : 1; /* [7..7] TWBBP输出方向ISO CELL控制信号。0：无影响1：表示输出被钳位 */
        unsigned int    wbbp_iso_en                : 1; /* [8..8] WBBP输出方向ISO CELL控制信号。0：无影响1：表示输出被钳位 */
        unsigned int    g1bbp_iso_en               : 1; /* [9..9] G1BBP输出方向ISO CELL控制信号。0：无影响1：表示输出被钳位 */
        unsigned int    g2bbp_iso_en               : 1; /* [10..10] G2BBP输出方向ISO CELL控制信号。0：无影响1：表示输出被钳位 */
        unsigned int    irm_iso_en                 : 1; /* [11..11] IRM输出方向ISO CELL控制信号。0：无影响1：表示输出被钳位 */
        unsigned int    usb_iso_en                 : 1; /* [12..12] USB输出方向ISO CELL控制信号。0：无影响1：表示输出被钳位 */
        unsigned int    sd_iso_en                  : 1; /* [13..13] SD 输出方向ISO CELL控制信号。0：无影响1：表示输出被钳位 */
        unsigned int    reserved_2                 : 8; /* [21..14] 保留 */
        unsigned int    io_ddr_ret_sw              : 1; /* [22..22] DDR 的 IO 的retention控制信号。0：无影响1：ret信号使能 */
        unsigned int    reserved_1                 : 3; /* [25..23]  */
        unsigned int    io_ret_lp_ctrl_sw          : 4; /* [29..26] IO的retention控制信号。对应bit为0：无影响对应bit为1：ret信号使能 */
        unsigned int    pmu_iso_en                 : 1; /* [30..30] PMU 输出方向ISO CELL控制信号。0：无影响1：表示输出被钳位 */
        unsigned int    reserved_0                 : 1; /* [31..31] 以下CM3开头的寄存器是CM3的专用寄存器，只有通过CM3的专用APB接口才能访问 */
    } bits;
    unsigned int    u32;
}HI_CM3_PWRCTRL0_T;    /* ISO cell 隔离使能 */

typedef union
{
    struct
    {
        unsigned int    appa9_iso_dis              : 1; /* [0..0] APPA9输出方向ISO CELL控制信号。0：无影响1：表示输出有效 */
        unsigned int    moda9_iso_dis              : 1; /* [1..1] MODA9输出方向ISO CELL控制信号。0：无影响1：表示输出有效 */
        unsigned int    peri_iso_dis               : 1; /* [2..2] SOC_PERI 输出方向ISO CELL控制信号。0：无影响1：表示输出有效 */
        unsigned int    hisc_iso_dis               : 1; /* [3..3] HSIC输出方向ISO CELL控制信号。0：无影响1：表示输出有效 */
        unsigned int    dsp0_iso_dis               : 1; /* [4..4] DSP0输出方向ISO CELL控制信号。0：无影响1：表示输出有效 */
        unsigned int    hifi_iso_dis               : 1; /* [5..5] HIFI输出方向ISO CELL控制信号。0：无影响1：表示输出有效 */
        unsigned int    lbbp_iso_dis               : 1; /* [6..6] LBBP输出方向ISO CELL控制信号。0：无影响1：表示输出有效 */
        unsigned int    twbbp_iso_dis              : 1; /* [7..7] TWBBP输出方向ISO CELL控制信号。0：无影响1：表示输出有效 */
        unsigned int    wbbp_iso_dis               : 1; /* [8..8] WBBP输出方向ISO CELL控制信号。0：无影响1：表示输出有效 */
        unsigned int    g1bbp_iso_dis              : 1; /* [9..9] G1BBP输出方向ISO CELL控制信号。0：无影响1：表示输出有效 */
        unsigned int    g2bbp_iso_dis              : 1; /* [10..10] G2BBP输出方向ISO CELL控制信号。0：无影响1：表示输出有效 */
        unsigned int    irm_iso_dis                : 1; /* [11..11] IRM输出方向ISO CELL控制信号。0：无影响1：表示输出有效 */
        unsigned int    usb_iso_dis                : 1; /* [12..12] USB输出方向ISO CELL控制信号。0：无影响1：表示输出有效 */
        unsigned int    sd_iso_dis                 : 1; /* [13..13] SD 输出方向ISO CELL控制信号。0：无影响1：表示输出有效 */
        unsigned int    reserved_2                 : 8; /* [21..14] 保留 */
        unsigned int    io_ddr_ret                 : 1; /* [22..22] DDR 的 IO 的retention控制信号。0：无影响1：ret信号禁止 */
        unsigned int    reserved_1                 : 3; /* [25..23]  */
        unsigned int    io_ret_lp_ctrl             : 4; /* [29..26] IO的retention控制信号。对应bit为0：无影响对应bit为1：ret信号禁止 */
        unsigned int    pmu_iso_dis                : 1; /* [30..30] PMU 输出方向ISO CELL控制信号。0：无影响1：表示输出有效 */
        unsigned int    reserved_0                 : 1; /* [31..31]  */
    } bits;
    unsigned int    u32;
}HI_CM3_PWRCTRL1_T;    /* ISO cell 隔离禁止 */

typedef union
{
    struct
    {
        unsigned int    appa9_iso_ctrl             : 1; /* [0..0] APPA9输出方向ISO CELL控制信号。0：表示输出有效1：表示输出被钳位 */
        unsigned int    moda9_iso_ctrl             : 1; /* [1..1] MODA9输出方向ISO CELL控制信号。0：表示输出有效1：表示输出被钳位 */
        unsigned int    peri_iso_ctrl              : 1; /* [2..2] SOC_PERI输出方向ISO CELL控制信号。0：表示输出有效1：表示输出被钳位 */
        unsigned int    hisc_iso_ctrl              : 1; /* [3..3] HSIC输出方向ISO CELL控制信号。0：表示输出有效1：表示输出被钳位 */
        unsigned int    dsp0_iso_ctrl              : 1; /* [4..4] DSP0输出方向ISO CELL控制信号。0：表示输出有效1：表示输出被钳位 */
        unsigned int    hifi_iso_ctrl              : 1; /* [5..5] HIFI输出方向ISO CELL控制信号。0：表示输出有效1：表示输出被钳位 */
        unsigned int    lbbp_iso_ctrl              : 1; /* [6..6] LBBP输出方向ISO CELL控制信号。0：表示输出有效1：表示输出被钳位 */
        unsigned int    twbbp_iso_ctrl             : 1; /* [7..7] TWBBP输出方向ISO CELL控制信号。0：表示输出有效1：表示输出被钳位 */
        unsigned int    wbbp_iso_ctrl              : 1; /* [8..8] WBBP输出方向ISO CELL控制信号。0：表示输出有效1：表示输出被钳位 */
        unsigned int    g1bbp_iso_ctrl             : 1; /* [9..9] G1BBP输出方向ISO CELL控制信号。0：表示输出有效1：表示输出被钳位 */
        unsigned int    g2bbp_iso_ctrl             : 1; /* [10..10] G2BBP输出方向ISO CELL控制信号。0：表示输出有效1：表示输出被钳位 */
        unsigned int    irm_iso_ctrl               : 1; /* [11..11] IRM输出方向ISO CELL控制信号。0：表示输出有效1：表示输出被钳位 */
        unsigned int    usb_iso_ctrl               : 1; /* [12..12] USB输出方向ISO CELL控制信号。0：表示输出有效1：表示输出被钳位 */
        unsigned int    sd_iso_ctrl                : 1; /* [13..13] SD 输出方向ISO CELL控制信号。0：表示输出有效1：表示输出被钳位 */
        unsigned int    reserved_2                 : 8; /* [21..14] 保留 */
        unsigned int    io_ddr_ret                 : 1; /* [22..22] DDR 的 IO 的retention控制信号。0：表示ret信号禁止1：表示ret信号使能 */
        unsigned int    reserved_1                 : 3; /* [25..23]  */
        unsigned int    io_ret_lp_ctrl             : 4; /* [29..26] IO的retention控制信号。对应bit为0：ret信号禁止对应bit为1：ret信号使能 */
        unsigned int    pmu_iso_ctrl               : 1; /* [30..30] PMU 输出方向ISO CELL控制信号。0：表示输出有效1：表示输出被钳位 */
        unsigned int    reserved_0                 : 1; /* [31..31]  */
    } bits;
    unsigned int    u32;
}HI_CM3_ISO_STAT_T;    /* CM3 ISO cell 控制状态 */

typedef union
{
    struct
    {
        unsigned int    appa9_mtcmos_en            : 1; /* [0..0] APP A9 MTCMOS CELL控制信号。0：无影响1：表示上电 */
        unsigned int    moda9_mtcmos_en            : 1; /* [1..1] MOD A9 MTCMOS CELL控制信号。0：无影响1：表示上电 */
        unsigned int    peri_mtcmos_en             : 1; /* [2..2] SOC_PERI MTCMOS CELL控制信号。0：无影响1：表示上电 */
        unsigned int    hisc_mtcmos_en             : 1; /* [3..3] HSIC MTCMOS CELL控制信号。0：无影响1：表示上电 */
        unsigned int    dsp0_mtcmos_en             : 1; /* [4..4] DSP0 MTCMOS CELL控制信号。0：无影响1：表示上电 */
        unsigned int    hifi_mtcmos_en             : 1; /* [5..5] HIFI MTCMOS CELL控制信号。0：无影响1：表示上电 */
        unsigned int    lbbp_mtcmos_en             : 1; /* [6..6] LBBP MTCMOS CELL控制信号。0：无影响1：表示上电 */
        unsigned int    twbbp_mtcmos_en            : 1; /* [7..7] TWBBP MTCMOS CELL控制信号。0：无影响1：表示上电 */
        unsigned int    wbbp_mtcmos_en             : 1; /* [8..8] WBBP MTCMOS CELL控制信号。0：无影响1：表示上电 */
        unsigned int    g1bbp_mtcmos_en            : 1; /* [9..9] G1BBP MTCMOS CELL控制信号。0：无影响1：表示上电 */
        unsigned int    g2bbp_mtcmos_en            : 1; /* [10..10] G2BBP MTCMOS CELL控制信号。0：无影响1：表示上电 */
        unsigned int    irm_mtcmos_en              : 1; /* [11..11] IRM MTCMOS CELL控制信号。0：无影响1：表示上电 */
        unsigned int    usb_mtcmos_en              : 1; /* [12..12] USB MTCMOS CELL控制信号。0：无影响1：表示上电 */
        unsigned int    sd_mtcmos_en               : 1; /* [13..13] SD MTCMOS CELL控制信号。0：无影响1：表示上电 */
        unsigned int    reserved_1                 : 16; /* [29..14]  */
        unsigned int    pmu_pwr_on                 : 1; /* [30..30] PMU 上电使能控制信号              0：无影响1：表示上电 */
        unsigned int    reserved_0                 : 1; /* [31..31]  */
    } bits;
    unsigned int    u32;
}HI_CM3_PWRCTRL2_T;    /* MTCMOS 上电控制 */

typedef union
{
    struct
    {
        unsigned int    appa9_mtcmos_dis           : 1; /* [0..0] APP A9 MTCMOS CELL控制信号。0：无影响1：表示断电 */
        unsigned int    moda9_mtcmos_dis           : 1; /* [1..1] MOD A9 MTCMOS CELL控制信号。0：无影响1：表示断电 */
        unsigned int    peri_mtcmos_dis            : 1; /* [2..2] SOC_PERI MTCMOS CELL控制信号。0：无影响1：表示断电 */
        unsigned int    hisc_mtcmos_dis            : 1; /* [3..3] HSIC MTCMOS CELL控制信号。0：无影响1：表示断电 */
        unsigned int    dsp0_mtcmos_dis            : 1; /* [4..4] DSP0 MTCMOS CELL控制信号。0：无影响1：表示断电 */
        unsigned int    hifi_mtcmos_dis            : 1; /* [5..5] HIFI MTCMOS CELL控制信号。0：无影响1：表示断电 */
        unsigned int    lbbp_mtcmos_dis            : 1; /* [6..6] LBBP MTCMOS CELL控制信号。0：无影响1：表示断电 */
        unsigned int    twbbp_mtcmos_dis           : 1; /* [7..7] TWBBP MTCMOS CELL控制信号。0：无影响1：表示断电 */
        unsigned int    wbbp_mtcmos_dis            : 1; /* [8..8] WBBP MTCMOS CELL控制信号。0：无影响1：表示断电 */
        unsigned int    g1bbp_mtcmos_dis           : 1; /* [9..9] G1BBP MTCMOS CELL控制信号。0：无影响1：表示断电 */
        unsigned int    g2bbp_mtcmos_dis           : 1; /* [10..10] G2BBP MTCMOS CELL控制信号。0：无影响1：表示断电 */
        unsigned int    irm_mtcmos_dis             : 1; /* [11..11] IRM MTCMOS CELL控制信号。0：无影响1：表示断电 */
        unsigned int    usb_mtcmos_dis             : 1; /* [12..12] USB MTCMOS CELL控制信号。0：无影响1：表示断电 */
        unsigned int    sd_mtcmos_dis              : 1; /* [13..13] SD MTCMOS CELL控制信号。0：无影响1：表示断电 */
        unsigned int    reersved                   : 16; /* [29..14]  */
        unsigned int    pmu_pwr_off                : 1; /* [30..30] PMU 断电使能控制信号              0：无影响1：表示断电 */
        unsigned int    reserved                   : 1; /* [31..31]  */
    } bits;
    unsigned int    u32;
}HI_CM3_PWRCTRL3_T;    /* MTCMOS 下电控制 */

typedef union
{
    struct
    {
        unsigned int    appa9_mtcmos_ctrl          : 1; /* [0..0] APP A9 MTCMOS CELL控制信号。0：表示断电1：表示上电 */
        unsigned int    moda9_mtcmos_ctrl          : 1; /* [1..1] MOD A9 MTCMOS CELL控制信号。0：表示断电1：表示上电 */
        unsigned int    peri_mtcmos_ctrl           : 1; /* [2..2] SOC_PERI MTCMOS CELL控制信号。0：表示断电1：表示上电 */
        unsigned int    hisc_mtcmos_ctrl           : 1; /* [3..3] HSIC MTCMOS CELL控制信号。0：表示断电1：表示上电 */
        unsigned int    dsp0_mtcmos_ctrl           : 1; /* [4..4] DSP0 MTCMOS CELL控制信号。0：表示断电1：表示上电 */
        unsigned int    hifi_mtcmos_ctrl           : 1; /* [5..5] HIFI MTCMOS CELL控制信号。0：表示断电1：表示上电 */
        unsigned int    lbbp_mtcmos_ctrl           : 1; /* [6..6] LBBP MTCMOS CELL控制信号。0：表示断电1：表示上电 */
        unsigned int    twbbp_mtcmos_ctrl          : 1; /* [7..7] TWBBP MTCMOS CELL控制信号。0：表示断电1：表示上电 */
        unsigned int    wbbp_mtcmos_ctrl           : 1; /* [8..8] WBBP MTCMOS CELL控制信号。0：表示断电1：表示上电 */
        unsigned int    g1bbp_mtcmos_ctrl          : 1; /* [9..9] G1BBP MTCMOS CELL控制信号。0：表示断电1：表示上电 */
        unsigned int    g2bbp_mtcmos_ctrl          : 1; /* [10..10] G2BBP MTCMOS CELL控制信号。0：表示断电1：表示上电 */
        unsigned int    irm_mtcmos_ctrl            : 1; /* [11..11] IRM MTCMOS CELL控制信号。0：表示断电1：表示上电 */
        unsigned int    usb_mtcmos_ctrl            : 1; /* [12..12] USB MTCMOS CELL控制信号。0：表示断电1：表示上电 */
        unsigned int    sd_mtcmos_ctrl             : 1; /* [13..13] SD MTCMOS CELL控制信号。0：表示断电1：表示上电 */
        unsigned int    reserved_1                 : 16; /* [29..14] 保留 */
        unsigned int    pmu_pwr_on                 : 1; /* [30..30] PMU MTCMOS CELL控制信号。0：表示断电1：表示上电 */
        unsigned int    reserved_0                 : 1; /* [31..31]  */
    } bits;
    unsigned int    u32;
}HI_CM3_MTCMOS_STAT_T;    /* CM3 MTCMOS 控制状态 */

typedef union
{
    struct
    {
        unsigned int    peri_clk_en                : 1; /* [0..0] SOC_PERI 时钟使能控制                      0：时钟禁止                      1：时钟使能 */
        unsigned int    peri_srst_req              : 1; /* [1..1] SOC_PERI 复位请求                      0：复位请求无效                     1：复位请求有效 */
        unsigned int    pmu_clk_en                 : 1; /* [2..2] PMU 时钟使能控制                      0：时钟禁止                      1：时钟使能 */
        unsigned int    pmu_srst_req               : 1; /* [3..3] PMU 复位请求                      0：复位请求无效                     1：复位请求有效 */
        unsigned int    tcxo_en                    : 4; /* [7..4] TCXO使能控制，对应比特            0：tcxo禁止                       1：tcxo使能                       bit[7]对应pmu_ch1_tcxo_en     bit[6]对应pmu_ch0_tcxo_en     bit[5]对应abb_ch1_tcxo_en    bit[4]对应abb_ch0_tcxo_en */
        unsigned int    pwrctrl_bypass             : 1; /* [8..8] pwrctrl功耗控制旁路控制           0：功耗由pwrctrl输出信号控制      1：pwrctrl被旁路，功耗由CM3直接控制 */
        unsigned int    clk_sw_req                 : 6; /* [14..9] 时钟切换请求                     bit[14]:cm3_tcxo_clk_sw_req bit[13]:cm3_sleep_clk_sw_req bit[12]:cm3_pll_clk_sw_req bit[11]:bus_tcxo_clk_sw_req bit[10]:bus_sleep_clk_sw_req    bit[9]:bus_pll_clk_sw_req */
        unsigned int    reserved                   : 9; /* [23..15]  */
        unsigned int    debug_sel                  : 8; /* [31..24] debug状态寄存器返回值选择控制。见寄存器CM3_DEBUG_STAT */
    } bits;
    unsigned int    u32;
}HI_CM3_PWRCTRL4_T;    /* CM3功耗控制寄存器 */

typedef union
{
    struct
    {
        unsigned int    mtcmos_ctrl_stat           : 31; /* [30..0] 同寄存器PWR_STAT4 */
        unsigned int    reserved                   : 1; /* [31..31]  */
    } bits;
    unsigned int    u32;
}HI_CM3_PWRSTAT0_T;    /* PWRCTRL MTCMOS 控制状态 */

typedef union
{
    struct
    {
        unsigned int    iso_ctrl_stat              : 31; /* [30..0] 同寄存器PWR_STAT3 */
        unsigned int    reserved                   : 1; /* [31..31]  */
    } bits;
    unsigned int    u32;
}HI_CM3_PWRSTAT1_T;    /* PWRCTRL ISO cell 控制状态 */

typedef union
{
    struct
    {
        unsigned int    debug_stat                 : 32; /* [31..0] debug状态寄存器，根据debug_sel取不同的值反应不用的状态：          0：同寄存器PWR_STAT1                 1：                                bit[9]:pwrctrl输出的peri_clk_en   bit[8]:pwrctrl输出的peri_srst_req        bit[7]:pwrctrl输出的pmu_clk_en   bit[6]:pwrctrl输出的pmu_srst_req bit[5]:pwrctrl输出的cm3_clk_en bit[4]:pwrctrl输出的cm3_srst_req bit[3:0]:pwrctrl输出的tcxo_en                               2： 时钟切换完成状态                   bit[5]:cm3_tcxo_clk_sw_done bit[4]:cm3_sleep_clk_sw_done bit[3]:cm3_pll_clk_sw_done bit[2]:bus_tcxo_clk_sw_done bit[1]:bus_sleep_clk_sw_done    bit[0]:bus_pll_clk_sw_done                              3：同寄存器SC_STAT15               4：同寄存器SC_STAT9                               5：同寄存器SC_STAT10                                    6：同寄存器SC_STAT22                               7：同寄存器SC_STAT27                              8：同寄存器SC_STAT28                                9：同寄存器SC_STAT29              0xFF：DDRC的APB接口读取的数据 */
    } bits;
    unsigned int    u32;
}HI_CM3_DEBUG_STAT_T;    /* debug 状态 */


/********************************************************************************/
/*    pwrctrl 函数（项目名_模块名_寄存器名_成员名_set)        */
/********************************************************************************/
HI_SET_GET(hi_pwr_ctrl0_cpumode_ctrl,cpumode_ctrl,HI_PWR_CTRL0_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL0_OFFSET)
HI_SET_GET(hi_pwr_ctrl0_a9_cpumode_status,a9_cpumode_status,HI_PWR_CTRL0_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL0_OFFSET)
HI_SET_GET(hi_pwr_ctrl0_reserved_1,reserved_1,HI_PWR_CTRL0_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL0_OFFSET)
HI_SET_GET(hi_pwr_ctrl0_cpu_dsleep_en,cpu_dsleep_en,HI_PWR_CTRL0_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL0_OFFSET)
HI_SET_GET(hi_pwr_ctrl0_reserved_0,reserved_0,HI_PWR_CTRL0_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL0_OFFSET)
HI_SET_GET(hi_pwr_ctrl0_doze_es_skip,doze_es_skip,HI_PWR_CTRL0_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL0_OFFSET)
HI_SET_GET(hi_pwr_ctrl0_doze_wk_skip,doze_wk_skip,HI_PWR_CTRL0_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL0_OFFSET)
HI_SET_GET(hi_pwr_ctrl0_cm3_sleephold_en,cm3_sleephold_en,HI_PWR_CTRL0_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL0_OFFSET)
HI_SET_GET(hi_pwr_ctrl0_cm3_sleeping_en,cm3_sleeping_en,HI_PWR_CTRL0_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL0_OFFSET)
HI_SET_GET(hi_pwr_ctrl0_cm3_sleepdeep_en,cm3_sleepdeep_en,HI_PWR_CTRL0_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL0_OFFSET)
HI_SET_GET(hi_pwr_ctrl0_arm_dsleep_rst_en,arm_dsleep_rst_en,HI_PWR_CTRL0_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL0_OFFSET)
HI_SET_GET(hi_pwr_ctrl0_usb_irq_glitch_free,usb_irq_glitch_free,HI_PWR_CTRL0_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL0_OFFSET)
HI_SET_GET(hi_pwr_ctrl0_io_ret_value_sel,io_ret_value_sel,HI_PWR_CTRL0_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL0_OFFSET)
HI_SET_GET(hi_pwr_ctrl0_arm_boot_type,arm_boot_type,HI_PWR_CTRL0_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL0_OFFSET)
HI_SET_GET(hi_pwr_ctrl0_sys_sleep_flag,sys_sleep_flag,HI_PWR_CTRL0_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL0_OFFSET)
HI_SET_GET(hi_pwr_ctrl1_sys_intmod_en,sys_intmod_en,HI_PWR_CTRL1_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL1_OFFSET)
HI_SET_GET(hi_pwr_ctrl1_sys_intmod_ctrl,sys_intmod_ctrl,HI_PWR_CTRL1_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL1_OFFSET)
HI_SET_GET(hi_pwr_ctrl1_reserved_1,reserved_1,HI_PWR_CTRL1_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL1_OFFSET)
HI_SET_GET(hi_pwr_ctrl1_sys_intmod_stat,sys_intmod_stat,HI_PWR_CTRL1_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL1_OFFSET)
HI_SET_GET(hi_pwr_ctrl1_reserved_0,reserved_0,HI_PWR_CTRL1_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL1_OFFSET)
HI_SET_GET(hi_pwr_ctrl2_tcxo_time,tcxo_time,HI_PWR_CTRL2_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL2_OFFSET)
HI_SET_GET(hi_pwr_ctrl2_soc_tcxo_over,soc_tcxo_over,HI_PWR_CTRL2_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL2_OFFSET)
HI_SET_GET(hi_pwr_ctrl2_reserved_1,reserved_1,HI_PWR_CTRL2_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL2_OFFSET)
HI_SET_GET(hi_pwr_ctrl2_sleep_tcxo_off,sleep_tcxo_off,HI_PWR_CTRL2_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL2_OFFSET)
HI_SET_GET(hi_pwr_ctrl2_soc_tcxo_on,soc_tcxo_on,HI_PWR_CTRL2_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL2_OFFSET)
HI_SET_GET(hi_pwr_ctrl2_abb_ch0_tcxo_en,abb_ch0_tcxo_en,HI_PWR_CTRL2_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL2_OFFSET)
HI_SET_GET(hi_pwr_ctrl2_abb_ch1_tcxo_en,abb_ch1_tcxo_en,HI_PWR_CTRL2_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL2_OFFSET)
HI_SET_GET(hi_pwr_ctrl2_pmu_ch0_tcxo_en,pmu_ch0_tcxo_en,HI_PWR_CTRL2_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL2_OFFSET)
HI_SET_GET(hi_pwr_ctrl2_pmu_ch1_tcxo_en,pmu_ch1_tcxo_en,HI_PWR_CTRL2_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL2_OFFSET)
HI_SET_GET(hi_pwr_ctrl2_pmu_abb_tcxo_time,pmu_abb_tcxo_time,HI_PWR_CTRL2_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL2_OFFSET)
HI_SET_GET(hi_pwr_ctrl2_pmu_abb_time_en,pmu_abb_time_en,HI_PWR_CTRL2_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL2_OFFSET)
HI_SET_GET(hi_pwr_ctrl2_pmu_tcxo_en_stat,pmu_tcxo_en_stat,HI_PWR_CTRL2_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL2_OFFSET)
HI_SET_GET(hi_pwr_ctrl2_reserved_0,reserved_0,HI_PWR_CTRL2_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL2_OFFSET)
HI_SET_GET(hi_pwr_ctrl3_pll_time,pll_time,HI_PWR_CTRL3_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL3_OFFSET)
HI_SET_GET(hi_pwr_ctrl3_pll_time_en,pll_time_en,HI_PWR_CTRL3_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL3_OFFSET)
HI_SET_GET(hi_pwr_ctrl3_pll_status,pll_status,HI_PWR_CTRL3_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL3_OFFSET)
HI_SET_GET(hi_pwr_ctrl3_reserved,reserved,HI_PWR_CTRL3_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL3_OFFSET)
HI_SET_GET(hi_pwr_ctrl4_appa9_iso_en,appa9_iso_en,HI_PWR_CTRL4_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL4_OFFSET)
HI_SET_GET(hi_pwr_ctrl4_moda9_iso_en,moda9_iso_en,HI_PWR_CTRL4_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL4_OFFSET)
HI_SET_GET(hi_pwr_ctrl4_reserved_3,reserved_3,HI_PWR_CTRL4_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL4_OFFSET)
HI_SET_GET(hi_pwr_ctrl4_hisc_iso_en,hisc_iso_en,HI_PWR_CTRL4_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL4_OFFSET)
HI_SET_GET(hi_pwr_ctrl4_dsp0_iso_en,dsp0_iso_en,HI_PWR_CTRL4_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL4_OFFSET)
HI_SET_GET(hi_pwr_ctrl4_hifi_iso_en,hifi_iso_en,HI_PWR_CTRL4_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL4_OFFSET)
HI_SET_GET(hi_pwr_ctrl4_lbbp_iso_en,lbbp_iso_en,HI_PWR_CTRL4_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL4_OFFSET)
HI_SET_GET(hi_pwr_ctrl4_twbbp_iso_en,twbbp_iso_en,HI_PWR_CTRL4_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL4_OFFSET)
HI_SET_GET(hi_pwr_ctrl4_wbbp_iso_en,wbbp_iso_en,HI_PWR_CTRL4_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL4_OFFSET)
HI_SET_GET(hi_pwr_ctrl4_g1bbp_iso_en,g1bbp_iso_en,HI_PWR_CTRL4_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL4_OFFSET)
HI_SET_GET(hi_pwr_ctrl4_g2bbp_iso_en,g2bbp_iso_en,HI_PWR_CTRL4_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL4_OFFSET)
HI_SET_GET(hi_pwr_ctrl4_irm_iso_en,irm_iso_en,HI_PWR_CTRL4_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL4_OFFSET)
HI_SET_GET(hi_pwr_ctrl4_usb_iso_en,usb_iso_en,HI_PWR_CTRL4_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL4_OFFSET)
HI_SET_GET(hi_pwr_ctrl4_sd_iso_en,sd_iso_en,HI_PWR_CTRL4_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL4_OFFSET)
HI_SET_GET(hi_pwr_ctrl4_reserved_2,reserved_2,HI_PWR_CTRL4_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL4_OFFSET)
HI_SET_GET(hi_pwr_ctrl4_io_ddr_ret,io_ddr_ret,HI_PWR_CTRL4_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL4_OFFSET)
HI_SET_GET(hi_pwr_ctrl4_reserved_1,reserved_1,HI_PWR_CTRL4_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL4_OFFSET)
HI_SET_GET(hi_pwr_ctrl4_io_ret_lp_ctrl,io_ret_lp_ctrl,HI_PWR_CTRL4_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL4_OFFSET)
HI_SET_GET(hi_pwr_ctrl4_reserved_0,reserved_0,HI_PWR_CTRL4_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL4_OFFSET)
HI_SET_GET(hi_pwr_ctrl5_appa9_iso_dis,appa9_iso_dis,HI_PWR_CTRL5_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL5_OFFSET)
HI_SET_GET(hi_pwr_ctrl5_moda9_iso_dis,moda9_iso_dis,HI_PWR_CTRL5_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL5_OFFSET)
HI_SET_GET(hi_pwr_ctrl5_reserved_3,reserved_3,HI_PWR_CTRL5_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL5_OFFSET)
HI_SET_GET(hi_pwr_ctrl5_hisc_iso_dis,hisc_iso_dis,HI_PWR_CTRL5_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL5_OFFSET)
HI_SET_GET(hi_pwr_ctrl5_dsp0_iso_dis,dsp0_iso_dis,HI_PWR_CTRL5_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL5_OFFSET)
HI_SET_GET(hi_pwr_ctrl5_hifi_iso_dis,hifi_iso_dis,HI_PWR_CTRL5_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL5_OFFSET)
HI_SET_GET(hi_pwr_ctrl5_lbbp_iso_dis,lbbp_iso_dis,HI_PWR_CTRL5_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL5_OFFSET)
HI_SET_GET(hi_pwr_ctrl5_twbbp_iso_dis,twbbp_iso_dis,HI_PWR_CTRL5_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL5_OFFSET)
HI_SET_GET(hi_pwr_ctrl5_wbbp_iso_dis,wbbp_iso_dis,HI_PWR_CTRL5_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL5_OFFSET)
HI_SET_GET(hi_pwr_ctrl5_g1bbp_iso_dis,g1bbp_iso_dis,HI_PWR_CTRL5_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL5_OFFSET)
HI_SET_GET(hi_pwr_ctrl5_g2bbp_iso_dis,g2bbp_iso_dis,HI_PWR_CTRL5_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL5_OFFSET)
HI_SET_GET(hi_pwr_ctrl5_irm_iso_dis,irm_iso_dis,HI_PWR_CTRL5_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL5_OFFSET)
HI_SET_GET(hi_pwr_ctrl5_usb_iso_dis,usb_iso_dis,HI_PWR_CTRL5_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL5_OFFSET)
HI_SET_GET(hi_pwr_ctrl5_sd_iso_dis,sd_iso_dis,HI_PWR_CTRL5_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL5_OFFSET)
HI_SET_GET(hi_pwr_ctrl5_reserved_2,reserved_2,HI_PWR_CTRL5_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL5_OFFSET)
HI_SET_GET(hi_pwr_ctrl5_io_ddr_ret,io_ddr_ret,HI_PWR_CTRL5_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL5_OFFSET)
HI_SET_GET(hi_pwr_ctrl5_reserved_1,reserved_1,HI_PWR_CTRL5_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL5_OFFSET)
HI_SET_GET(hi_pwr_ctrl5_io_ret_lp_ctrl,io_ret_lp_ctrl,HI_PWR_CTRL5_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL5_OFFSET)
HI_SET_GET(hi_pwr_ctrl5_reserved_0,reserved_0,HI_PWR_CTRL5_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL5_OFFSET)
HI_SET_GET(hi_pwr_ctrl6_appa9_mtcmos_en,appa9_mtcmos_en,HI_PWR_CTRL6_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL6_OFFSET)
HI_SET_GET(hi_pwr_ctrl6_moda9_mtcmos_en,moda9_mtcmos_en,HI_PWR_CTRL6_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL6_OFFSET)
HI_SET_GET(hi_pwr_ctrl6_reserved_1,reserved_1,HI_PWR_CTRL6_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL6_OFFSET)
HI_SET_GET(hi_pwr_ctrl6_hisc_mtcmos_en,hisc_mtcmos_en,HI_PWR_CTRL6_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL6_OFFSET)
HI_SET_GET(hi_pwr_ctrl6_dsp0_mtcmos_en,dsp0_mtcmos_en,HI_PWR_CTRL6_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL6_OFFSET)
HI_SET_GET(hi_pwr_ctrl6_hifi_mtcmos_en,hifi_mtcmos_en,HI_PWR_CTRL6_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL6_OFFSET)
HI_SET_GET(hi_pwr_ctrl6_lbbp_mtcmos_en,lbbp_mtcmos_en,HI_PWR_CTRL6_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL6_OFFSET)
HI_SET_GET(hi_pwr_ctrl6_twbbp_mtcmos_en,twbbp_mtcmos_en,HI_PWR_CTRL6_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL6_OFFSET)
HI_SET_GET(hi_pwr_ctrl6_wbbp_mtcmos_en,wbbp_mtcmos_en,HI_PWR_CTRL6_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL6_OFFSET)
HI_SET_GET(hi_pwr_ctrl6_g1bbp_mtcmos_en,g1bbp_mtcmos_en,HI_PWR_CTRL6_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL6_OFFSET)
HI_SET_GET(hi_pwr_ctrl6_g2bbp_mtcmos_en,g2bbp_mtcmos_en,HI_PWR_CTRL6_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL6_OFFSET)
HI_SET_GET(hi_pwr_ctrl6_irm_mtcmos_en,irm_mtcmos_en,HI_PWR_CTRL6_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL6_OFFSET)
HI_SET_GET(hi_pwr_ctrl6_usb_mtcmos_en,usb_mtcmos_en,HI_PWR_CTRL6_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL6_OFFSET)
HI_SET_GET(hi_pwr_ctrl6_sd_mtcmos_en,sd_mtcmos_en,HI_PWR_CTRL6_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL6_OFFSET)
HI_SET_GET(hi_pwr_ctrl6_reserved_0,reserved_0,HI_PWR_CTRL6_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL6_OFFSET)
HI_SET_GET(hi_pwr_ctrl7_appa9_mtcmos_dis,appa9_mtcmos_dis,HI_PWR_CTRL7_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL7_OFFSET)
HI_SET_GET(hi_pwr_ctrl7_moda9_mtcmos_dis,moda9_mtcmos_dis,HI_PWR_CTRL7_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL7_OFFSET)
HI_SET_GET(hi_pwr_ctrl7_reserved_1,reserved_1,HI_PWR_CTRL7_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL7_OFFSET)
HI_SET_GET(hi_pwr_ctrl7_hisc_mtcmos_dis,hisc_mtcmos_dis,HI_PWR_CTRL7_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL7_OFFSET)
HI_SET_GET(hi_pwr_ctrl7_dsp0_mtcmos_dis,dsp0_mtcmos_dis,HI_PWR_CTRL7_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL7_OFFSET)
HI_SET_GET(hi_pwr_ctrl7_hifi_mtcmos_dis,hifi_mtcmos_dis,HI_PWR_CTRL7_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL7_OFFSET)
HI_SET_GET(hi_pwr_ctrl7_lbbp_mtcmos_dis,lbbp_mtcmos_dis,HI_PWR_CTRL7_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL7_OFFSET)
HI_SET_GET(hi_pwr_ctrl7_twbbp_mtcmos_dis,twbbp_mtcmos_dis,HI_PWR_CTRL7_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL7_OFFSET)
HI_SET_GET(hi_pwr_ctrl7_wbbp_mtcmos_dis,wbbp_mtcmos_dis,HI_PWR_CTRL7_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL7_OFFSET)
HI_SET_GET(hi_pwr_ctrl7_g1bbp_mtcmos_dis,g1bbp_mtcmos_dis,HI_PWR_CTRL7_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL7_OFFSET)
HI_SET_GET(hi_pwr_ctrl7_g2bbp_mtcmos_dis,g2bbp_mtcmos_dis,HI_PWR_CTRL7_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL7_OFFSET)
HI_SET_GET(hi_pwr_ctrl7_irm_mtcmos_dis,irm_mtcmos_dis,HI_PWR_CTRL7_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL7_OFFSET)
HI_SET_GET(hi_pwr_ctrl7_usb_mtcmos_dis,usb_mtcmos_dis,HI_PWR_CTRL7_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL7_OFFSET)
HI_SET_GET(hi_pwr_ctrl7_sd_mtcmos_dis,sd_mtcmos_dis,HI_PWR_CTRL7_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL7_OFFSET)
HI_SET_GET(hi_pwr_ctrl7_reserved_0,reserved_0,HI_PWR_CTRL7_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL7_OFFSET)
HI_SET_GET(hi_pwr_ctrl8_cpu_pwrup_time,cpu_pwrup_time,HI_PWR_CTRL8_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL8_OFFSET)
HI_SET_GET(hi_pwr_ctrl8_reserved_1,reserved_1,HI_PWR_CTRL8_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL8_OFFSET)
HI_SET_GET(hi_pwr_ctrl8_cpu_pwrdn_time,cpu_pwrdn_time,HI_PWR_CTRL8_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL8_OFFSET)
HI_SET_GET(hi_pwr_ctrl8_cpu_pwrtime_en,cpu_pwrtime_en,HI_PWR_CTRL8_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL8_OFFSET)
HI_SET_GET(hi_pwr_ctrl8_reserved_0,reserved_0,HI_PWR_CTRL8_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL8_OFFSET)
HI_SET_GET(hi_pwr_ctrl9_peri_pwrup_time,peri_pwrup_time,HI_PWR_CTRL9_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL9_OFFSET)
HI_SET_GET(hi_pwr_ctrl9_reserved_1,reserved_1,HI_PWR_CTRL9_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL9_OFFSET)
HI_SET_GET(hi_pwr_ctrl9_peri_pwrdn_time,peri_pwrdn_time,HI_PWR_CTRL9_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL9_OFFSET)
HI_SET_GET(hi_pwr_ctrl9_peri_pwrtime_en,peri_pwrtime_en,HI_PWR_CTRL9_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL9_OFFSET)
HI_SET_GET(hi_pwr_ctrl9_dsleep_peri_pwrdn,dsleep_peri_pwrdn,HI_PWR_CTRL9_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL9_OFFSET)
HI_SET_GET(hi_pwr_ctrl9_sleep_periclk_en,sleep_periclk_en,HI_PWR_CTRL9_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL9_OFFSET)
HI_SET_GET(hi_pwr_ctrl9_reserved_0,reserved_0,HI_PWR_CTRL9_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL9_OFFSET)
HI_SET_GET(hi_pwr_ctrl10_retmem_nor_time,retmem_nor_time,HI_PWR_CTRL10_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL10_OFFSET)
HI_SET_GET(hi_pwr_ctrl10_reserved,reserved,HI_PWR_CTRL10_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL10_OFFSET)
HI_SET_GET(hi_pwr_ctrl11_retmem_ret_time,retmem_ret_time,HI_PWR_CTRL11_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL11_OFFSET)
HI_SET_GET(hi_pwr_ctrl11_reserved,reserved,HI_PWR_CTRL11_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL11_OFFSET)
HI_SET_GET(hi_pwr_ctrl12_ema,ema,HI_PWR_CTRL12_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL12_OFFSET)
HI_SET_GET(hi_pwr_ctrl12_reserved_3,reserved_3,HI_PWR_CTRL12_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL12_OFFSET)
HI_SET_GET(hi_pwr_ctrl12_emaw,emaw,HI_PWR_CTRL12_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL12_OFFSET)
HI_SET_GET(hi_pwr_ctrl12_emas,emas,HI_PWR_CTRL12_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL12_OFFSET)
HI_SET_GET(hi_pwr_ctrl12_reserved_2,reserved_2,HI_PWR_CTRL12_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL12_OFFSET)
HI_SET_GET(hi_pwr_ctrl12_retmem_ret1n_cfg,retmem_ret1n_cfg,HI_PWR_CTRL12_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL12_OFFSET)
HI_SET_GET(hi_pwr_ctrl12_retmem_ret2n_cfg,retmem_ret2n_cfg,HI_PWR_CTRL12_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL12_OFFSET)
HI_SET_GET(hi_pwr_ctrl12_retmem_pgen_cfg,retmem_pgen_cfg,HI_PWR_CTRL12_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL12_OFFSET)
HI_SET_GET(hi_pwr_ctrl12_reserved_1,reserved_1,HI_PWR_CTRL12_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL12_OFFSET)
HI_SET_GET(hi_pwr_ctrl12_retmem_sw_lp_ctrl,retmem_sw_lp_ctrl,HI_PWR_CTRL12_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL12_OFFSET)
HI_SET_GET(hi_pwr_ctrl12_dsleep_retmem_ret,dsleep_retmem_ret,HI_PWR_CTRL12_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL12_OFFSET)
HI_SET_GET(hi_pwr_ctrl12_reserved_0,reserved_0,HI_PWR_CTRL12_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL12_OFFSET)
HI_SET_GET(hi_pwr_ctrl13_pmu_pwrup_time,pmu_pwrup_time,HI_PWR_CTRL13_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL13_OFFSET)
HI_SET_GET(hi_pwr_ctrl13_reserved_1,reserved_1,HI_PWR_CTRL13_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL13_OFFSET)
HI_SET_GET(hi_pwr_ctrl13_dsleep_pmu_pwrdn,dsleep_pmu_pwrdn,HI_PWR_CTRL13_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL13_OFFSET)
HI_SET_GET(hi_pwr_ctrl13_sleep_pmuclk_en,sleep_pmuclk_en,HI_PWR_CTRL13_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL13_OFFSET)
HI_SET_GET(hi_pwr_ctrl13_reserved_0,reserved_0,HI_PWR_CTRL13_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL13_OFFSET)
HI_SET_GET(hi_pwr_ctrl14_sleep_hold_cycle,sleep_hold_cycle,HI_PWR_CTRL14_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL14_OFFSET)
HI_SET_GET(hi_pwr_ctrl14_sleephole_cnt,sleephole_cnt,HI_PWR_CTRL14_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL14_OFFSET)
HI_SET_GET(hi_pwr_ctrl15_moda9_usbwkup_en,moda9_usbwkup_en,HI_PWR_CTRL15_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL15_OFFSET)
HI_SET_GET(hi_pwr_ctrl15_appa9_usbwkup_en,appa9_usbwkup_en,HI_PWR_CTRL15_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL15_OFFSET)
HI_SET_GET(hi_pwr_ctrl15_a9_otgphy_wkup_en,a9_otgphy_wkup_en,HI_PWR_CTRL15_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL15_OFFSET)
HI_SET_GET(hi_pwr_ctrl15_a9_otgsuspend_wkup_en,a9_otgsuspend_wkup_en,HI_PWR_CTRL15_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL15_OFFSET)
HI_SET_GET(hi_pwr_ctrl15_otgawson_wpc_clk_en,otgawson_wpc_clk_en,HI_PWR_CTRL15_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL15_OFFSET)
HI_SET_GET(hi_pwr_ctrl15_reserved_1,reserved_1,HI_PWR_CTRL15_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL15_OFFSET)
HI_SET_GET(hi_pwr_ctrl15_sleep_usbwkup_en,sleep_usbwkup_en,HI_PWR_CTRL15_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL15_OFFSET)
HI_SET_GET(hi_pwr_ctrl15_reserved_0,reserved_0,HI_PWR_CTRL15_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL15_OFFSET)
HI_SET_GET(hi_pwr_ctrl16_cm3_irqs_en_31_0,cm3_irqs_en_31_0,HI_PWR_CTRL16_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL16_OFFSET)
HI_SET_GET(hi_pwr_ctrl17_cm3_irqs_en_63_32,cm3_irqs_en_63_32,HI_PWR_CTRL17_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL17_OFFSET)
HI_SET_GET(hi_pwr_ctrl18_cm3_irqs_en_95_64,cm3_irqs_en_95_64,HI_PWR_CTRL18_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL18_OFFSET)
HI_SET_GET(hi_pwr_ctrl19_cm3_irqs_en_127_96,cm3_irqs_en_127_96,HI_PWR_CTRL19_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL19_OFFSET)
HI_SET_GET(hi_pwr_ctrl20_reserved,reserved,HI_PWR_CTRL20_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL20_OFFSET)
HI_SET_GET(hi_pwr_ctrl21_reserved,reserved,HI_PWR_CTRL21_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_CTRL21_OFFSET)
HI_SET_GET(hi_pwr_stat0_cpu_volt_fsm,cpu_volt_fsm,HI_PWR_STAT0_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT0_OFFSET)
HI_SET_GET(hi_pwr_stat0_cpu_pwrup_timeout,cpu_pwrup_timeout,HI_PWR_STAT0_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT0_OFFSET)
HI_SET_GET(hi_pwr_stat0_cpu_pwrdn_timeout,cpu_pwrdn_timeout,HI_PWR_STAT0_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT0_OFFSET)
HI_SET_GET(hi_pwr_stat0_peri_pwrup_timeout,peri_pwrup_timeout,HI_PWR_STAT0_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT0_OFFSET)
HI_SET_GET(hi_pwr_stat0_peri_pwrdn_timeout,peri_pwrdn_timeout,HI_PWR_STAT0_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT0_OFFSET)
HI_SET_GET(hi_pwr_stat0_retmem_nor_timeout,retmem_nor_timeout,HI_PWR_STAT0_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT0_OFFSET)
HI_SET_GET(hi_pwr_stat0_retmem_ret_timeout,retmem_ret_timeout,HI_PWR_STAT0_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT0_OFFSET)
HI_SET_GET(hi_pwr_stat0_moda9_standbywfi,moda9_standbywfi,HI_PWR_STAT0_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT0_OFFSET)
HI_SET_GET(hi_pwr_stat0_moda9_standbywfe,moda9_standbywfe,HI_PWR_STAT0_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT0_OFFSET)
HI_SET_GET(hi_pwr_stat0_appa9_standbywfi,appa9_standbywfi,HI_PWR_STAT0_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT0_OFFSET)
HI_SET_GET(hi_pwr_stat0_appa9_standbywfe,appa9_standbywfe,HI_PWR_STAT0_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT0_OFFSET)
HI_SET_GET(hi_pwr_stat0_pmu_pwrup_timeout,pmu_pwrup_timeout,HI_PWR_STAT0_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT0_OFFSET)
HI_SET_GET(hi_pwr_stat0_reserved_1,reserved_1,HI_PWR_STAT0_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT0_OFFSET)
HI_SET_GET(hi_pwr_stat0_cm3_sleeping,cm3_sleeping,HI_PWR_STAT0_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT0_OFFSET)
HI_SET_GET(hi_pwr_stat0_cm3_sleepdeep,cm3_sleepdeep,HI_PWR_STAT0_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT0_OFFSET)
HI_SET_GET(hi_pwr_stat0_cm3_sleephold_ack_n,cm3_sleephold_ack_n,HI_PWR_STAT0_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT0_OFFSET)
HI_SET_GET(hi_pwr_stat0_reserved_0,reserved_0,HI_PWR_STAT0_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT0_OFFSET)
HI_SET_GET(hi_pwr_stat1_appa9_mtcmos_rdy,appa9_mtcmos_rdy,HI_PWR_STAT1_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT1_OFFSET)
HI_SET_GET(hi_pwr_stat1_moda9_mtcmos_rdy,moda9_mtcmos_rdy,HI_PWR_STAT1_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT1_OFFSET)
HI_SET_GET(hi_pwr_stat1_peri_mtcmos_rdy,peri_mtcmos_rdy,HI_PWR_STAT1_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT1_OFFSET)
HI_SET_GET(hi_pwr_stat1_hisc_mtcmos_rdy,hisc_mtcmos_rdy,HI_PWR_STAT1_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT1_OFFSET)
HI_SET_GET(hi_pwr_stat1_dsp0_mtcmos_rdy,dsp0_mtcmos_rdy,HI_PWR_STAT1_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT1_OFFSET)
HI_SET_GET(hi_pwr_stat1_hifi_mtcmos_rdy,hifi_mtcmos_rdy,HI_PWR_STAT1_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT1_OFFSET)
HI_SET_GET(hi_pwr_stat1_lbbp_mtcmos_rdy,lbbp_mtcmos_rdy,HI_PWR_STAT1_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT1_OFFSET)
HI_SET_GET(hi_pwr_stat1_twbbp_mtcmos_rdy,twbbp_mtcmos_rdy,HI_PWR_STAT1_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT1_OFFSET)
HI_SET_GET(hi_pwr_stat1_wbbp_mtcmos_rdy,wbbp_mtcmos_rdy,HI_PWR_STAT1_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT1_OFFSET)
HI_SET_GET(hi_pwr_stat1_g1bbp_mtcmos_rdy,g1bbp_mtcmos_rdy,HI_PWR_STAT1_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT1_OFFSET)
HI_SET_GET(hi_pwr_stat1_g2bbp_mtcmos_rdy,g2bbp_mtcmos_rdy,HI_PWR_STAT1_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT1_OFFSET)
HI_SET_GET(hi_pwr_stat1_irm_mtcmos_rdy,irm_mtcmos_rdy,HI_PWR_STAT1_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT1_OFFSET)
HI_SET_GET(hi_pwr_stat1_usb_mtcmos_rdy,usb_mtcmos_rdy,HI_PWR_STAT1_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT1_OFFSET)
HI_SET_GET(hi_pwr_stat1_sd_mtcmos_rdy,sd_mtcmos_rdy,HI_PWR_STAT1_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT1_OFFSET)
HI_SET_GET(hi_pwr_stat1_reserved,reserved,HI_PWR_STAT1_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT1_OFFSET)
HI_SET_GET(hi_pwr_stat2_sccnt_data,sccnt_data,HI_PWR_STAT2_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT2_OFFSET)
HI_SET_GET(hi_pwr_stat2_reserved,reserved,HI_PWR_STAT2_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT2_OFFSET)
HI_SET_GET(hi_pwr_stat3_appa9_iso_ctrl,appa9_iso_ctrl,HI_PWR_STAT3_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT3_OFFSET)
HI_SET_GET(hi_pwr_stat3_moda9_iso_ctrl,moda9_iso_ctrl,HI_PWR_STAT3_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT3_OFFSET)
HI_SET_GET(hi_pwr_stat3_peri_iso_ctrl,peri_iso_ctrl,HI_PWR_STAT3_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT3_OFFSET)
HI_SET_GET(hi_pwr_stat3_hisc_iso_ctrl,hisc_iso_ctrl,HI_PWR_STAT3_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT3_OFFSET)
HI_SET_GET(hi_pwr_stat3_dsp0_iso_ctrl,dsp0_iso_ctrl,HI_PWR_STAT3_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT3_OFFSET)
HI_SET_GET(hi_pwr_stat3_hifi_iso_ctrl,hifi_iso_ctrl,HI_PWR_STAT3_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT3_OFFSET)
HI_SET_GET(hi_pwr_stat3_lbbp_iso_ctrl,lbbp_iso_ctrl,HI_PWR_STAT3_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT3_OFFSET)
HI_SET_GET(hi_pwr_stat3_twbbp_iso_ctrl,twbbp_iso_ctrl,HI_PWR_STAT3_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT3_OFFSET)
HI_SET_GET(hi_pwr_stat3_wbbp_iso_ctrl,wbbp_iso_ctrl,HI_PWR_STAT3_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT3_OFFSET)
HI_SET_GET(hi_pwr_stat3_g1bbp_iso_ctrl,g1bbp_iso_ctrl,HI_PWR_STAT3_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT3_OFFSET)
HI_SET_GET(hi_pwr_stat3_g2bbp_iso_ctrl,g2bbp_iso_ctrl,HI_PWR_STAT3_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT3_OFFSET)
HI_SET_GET(hi_pwr_stat3_irm_iso_ctrl,irm_iso_ctrl,HI_PWR_STAT3_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT3_OFFSET)
HI_SET_GET(hi_pwr_stat3_usb_iso_ctrl,usb_iso_ctrl,HI_PWR_STAT3_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT3_OFFSET)
HI_SET_GET(hi_pwr_stat3_sd_iso_ctrl,sd_iso_ctrl,HI_PWR_STAT3_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT3_OFFSET)
HI_SET_GET(hi_pwr_stat3_reserved_2,reserved_2,HI_PWR_STAT3_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT3_OFFSET)
HI_SET_GET(hi_pwr_stat3_io_ddr_ret,io_ddr_ret,HI_PWR_STAT3_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT3_OFFSET)
HI_SET_GET(hi_pwr_stat3_reserved_1,reserved_1,HI_PWR_STAT3_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT3_OFFSET)
HI_SET_GET(hi_pwr_stat3_io_ret_lp_ctrl,io_ret_lp_ctrl,HI_PWR_STAT3_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT3_OFFSET)
HI_SET_GET(hi_pwr_stat3_pmu_iso_ctrl,pmu_iso_ctrl,HI_PWR_STAT3_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT3_OFFSET)
HI_SET_GET(hi_pwr_stat3_reserved_0,reserved_0,HI_PWR_STAT3_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT3_OFFSET)
HI_SET_GET(hi_pwr_stat4_appa9_mtcmos_ctrl,appa9_mtcmos_ctrl,HI_PWR_STAT4_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT4_OFFSET)
HI_SET_GET(hi_pwr_stat4_moda9_mtcmos_ctrl,moda9_mtcmos_ctrl,HI_PWR_STAT4_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT4_OFFSET)
HI_SET_GET(hi_pwr_stat4_peri_mtcmos_ctrl,peri_mtcmos_ctrl,HI_PWR_STAT4_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT4_OFFSET)
HI_SET_GET(hi_pwr_stat4_hisc_mtcmos_ctrl,hisc_mtcmos_ctrl,HI_PWR_STAT4_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT4_OFFSET)
HI_SET_GET(hi_pwr_stat4_dsp0_mtcmos_ctrl,dsp0_mtcmos_ctrl,HI_PWR_STAT4_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT4_OFFSET)
HI_SET_GET(hi_pwr_stat4_hifi_mtcmos_ctrl,hifi_mtcmos_ctrl,HI_PWR_STAT4_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT4_OFFSET)
HI_SET_GET(hi_pwr_stat4_lbbp_mtcmos_ctrl,lbbp_mtcmos_ctrl,HI_PWR_STAT4_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT4_OFFSET)
HI_SET_GET(hi_pwr_stat4_twbbp_mtcmos_ctrl,twbbp_mtcmos_ctrl,HI_PWR_STAT4_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT4_OFFSET)
HI_SET_GET(hi_pwr_stat4_wbbp_mtcmos_ctrl,wbbp_mtcmos_ctrl,HI_PWR_STAT4_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT4_OFFSET)
HI_SET_GET(hi_pwr_stat4_g1bbp_mtcmos_ctrl,g1bbp_mtcmos_ctrl,HI_PWR_STAT4_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT4_OFFSET)
HI_SET_GET(hi_pwr_stat4_g2bbp_mtcmos_ctrl,g2bbp_mtcmos_ctrl,HI_PWR_STAT4_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT4_OFFSET)
HI_SET_GET(hi_pwr_stat4_irm_mtcmos_ctrl,irm_mtcmos_ctrl,HI_PWR_STAT4_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT4_OFFSET)
HI_SET_GET(hi_pwr_stat4_usb_mtcmos_ctrl,usb_mtcmos_ctrl,HI_PWR_STAT4_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT4_OFFSET)
HI_SET_GET(hi_pwr_stat4_sd_mtcmos_ctrl,sd_mtcmos_ctrl,HI_PWR_STAT4_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT4_OFFSET)
HI_SET_GET(hi_pwr_stat4_reserved_1,reserved_1,HI_PWR_STAT4_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT4_OFFSET)
HI_SET_GET(hi_pwr_stat4_pmu_pwr_on,pmu_pwr_on,HI_PWR_STAT4_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT4_OFFSET)
HI_SET_GET(hi_pwr_stat4_reserved_0,reserved_0,HI_PWR_STAT4_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT4_OFFSET)
HI_SET_GET(hi_pwr_stat5_retmem_ctrl,retmem_ctrl,HI_PWR_STAT5_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT5_OFFSET)
HI_SET_GET(hi_pwr_stat5_reserved,reserved,HI_PWR_STAT5_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT5_OFFSET)
HI_SET_GET(hi_pwr_stat6_cm3_wkup_stat0,cm3_wkup_stat0,HI_PWR_STAT6_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT6_OFFSET)
HI_SET_GET(hi_pwr_stat6_cm3_wkup_stat1,cm3_wkup_stat1,HI_PWR_STAT6_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT6_OFFSET)
HI_SET_GET(hi_pwr_stat6_cm3_wkup_stat2,cm3_wkup_stat2,HI_PWR_STAT6_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT6_OFFSET)
HI_SET_GET(hi_pwr_stat6_cm3_wkup_stat3,cm3_wkup_stat3,HI_PWR_STAT6_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT6_OFFSET)
HI_SET_GET(hi_pwr_stat6_reserved,reserved,HI_PWR_STAT6_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT6_OFFSET)
HI_SET_GET(hi_pwr_stat7_app_wkup_irq0,app_wkup_irq0,HI_PWR_STAT7_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT7_OFFSET)
HI_SET_GET(hi_pwr_stat8_app_wkup_irq1,app_wkup_irq1,HI_PWR_STAT8_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT8_OFFSET)
HI_SET_GET(hi_pwr_stat9_app_wkup_irq2,app_wkup_irq2,HI_PWR_STAT9_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT9_OFFSET)
HI_SET_GET(hi_pwr_stat10_app_wkup_irq3,app_wkup_irq3,HI_PWR_STAT10_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT10_OFFSET)
HI_SET_GET(hi_pwr_stat11_reserved,reserved,HI_PWR_STAT11_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT11_OFFSET)
HI_SET_GET(hi_pwr_stat12_reserved,reserved,HI_PWR_STAT12_T,HI_PWRCTRL_BASE_ADDR, HI_PWR_STAT12_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl0_appa9_iso_en,appa9_iso_en,HI_CM3_PWRCTRL0_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL0_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl0_moda9_iso_en,moda9_iso_en,HI_CM3_PWRCTRL0_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL0_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl0_peri_iso_en,peri_iso_en,HI_CM3_PWRCTRL0_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL0_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl0_hisc_iso_en,hisc_iso_en,HI_CM3_PWRCTRL0_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL0_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl0_dsp0_iso_en,dsp0_iso_en,HI_CM3_PWRCTRL0_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL0_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl0_hifi_iso_en,hifi_iso_en,HI_CM3_PWRCTRL0_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL0_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl0_lbbp_iso_en,lbbp_iso_en,HI_CM3_PWRCTRL0_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL0_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl0_twbbp_iso_en,twbbp_iso_en,HI_CM3_PWRCTRL0_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL0_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl0_wbbp_iso_en,wbbp_iso_en,HI_CM3_PWRCTRL0_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL0_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl0_g1bbp_iso_en,g1bbp_iso_en,HI_CM3_PWRCTRL0_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL0_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl0_g2bbp_iso_en,g2bbp_iso_en,HI_CM3_PWRCTRL0_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL0_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl0_irm_iso_en,irm_iso_en,HI_CM3_PWRCTRL0_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL0_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl0_usb_iso_en,usb_iso_en,HI_CM3_PWRCTRL0_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL0_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl0_sd_iso_en,sd_iso_en,HI_CM3_PWRCTRL0_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL0_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl0_reserved_2,reserved_2,HI_CM3_PWRCTRL0_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL0_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl0_io_ddr_ret_sw,io_ddr_ret_sw,HI_CM3_PWRCTRL0_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL0_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl0_reserved_1,reserved_1,HI_CM3_PWRCTRL0_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL0_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl0_io_ret_lp_ctrl_sw,io_ret_lp_ctrl_sw,HI_CM3_PWRCTRL0_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL0_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl0_pmu_iso_en,pmu_iso_en,HI_CM3_PWRCTRL0_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL0_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl0_reserved_0,reserved_0,HI_CM3_PWRCTRL0_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL0_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl1_appa9_iso_dis,appa9_iso_dis,HI_CM3_PWRCTRL1_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL1_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl1_moda9_iso_dis,moda9_iso_dis,HI_CM3_PWRCTRL1_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL1_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl1_peri_iso_dis,peri_iso_dis,HI_CM3_PWRCTRL1_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL1_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl1_hisc_iso_dis,hisc_iso_dis,HI_CM3_PWRCTRL1_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL1_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl1_dsp0_iso_dis,dsp0_iso_dis,HI_CM3_PWRCTRL1_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL1_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl1_hifi_iso_dis,hifi_iso_dis,HI_CM3_PWRCTRL1_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL1_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl1_lbbp_iso_dis,lbbp_iso_dis,HI_CM3_PWRCTRL1_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL1_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl1_twbbp_iso_dis,twbbp_iso_dis,HI_CM3_PWRCTRL1_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL1_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl1_wbbp_iso_dis,wbbp_iso_dis,HI_CM3_PWRCTRL1_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL1_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl1_g1bbp_iso_dis,g1bbp_iso_dis,HI_CM3_PWRCTRL1_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL1_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl1_g2bbp_iso_dis,g2bbp_iso_dis,HI_CM3_PWRCTRL1_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL1_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl1_irm_iso_dis,irm_iso_dis,HI_CM3_PWRCTRL1_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL1_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl1_usb_iso_dis,usb_iso_dis,HI_CM3_PWRCTRL1_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL1_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl1_sd_iso_dis,sd_iso_dis,HI_CM3_PWRCTRL1_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL1_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl1_reserved_2,reserved_2,HI_CM3_PWRCTRL1_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL1_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl1_io_ddr_ret,io_ddr_ret,HI_CM3_PWRCTRL1_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL1_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl1_reserved_1,reserved_1,HI_CM3_PWRCTRL1_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL1_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl1_io_ret_lp_ctrl,io_ret_lp_ctrl,HI_CM3_PWRCTRL1_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL1_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl1_pmu_iso_dis,pmu_iso_dis,HI_CM3_PWRCTRL1_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL1_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl1_reserved_0,reserved_0,HI_CM3_PWRCTRL1_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL1_OFFSET)
HI_SET_GET(hi_cm3_iso_stat_appa9_iso_ctrl,appa9_iso_ctrl,HI_CM3_ISO_STAT_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_ISO_STAT_OFFSET)
HI_SET_GET(hi_cm3_iso_stat_moda9_iso_ctrl,moda9_iso_ctrl,HI_CM3_ISO_STAT_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_ISO_STAT_OFFSET)
HI_SET_GET(hi_cm3_iso_stat_peri_iso_ctrl,peri_iso_ctrl,HI_CM3_ISO_STAT_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_ISO_STAT_OFFSET)
HI_SET_GET(hi_cm3_iso_stat_hisc_iso_ctrl,hisc_iso_ctrl,HI_CM3_ISO_STAT_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_ISO_STAT_OFFSET)
HI_SET_GET(hi_cm3_iso_stat_dsp0_iso_ctrl,dsp0_iso_ctrl,HI_CM3_ISO_STAT_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_ISO_STAT_OFFSET)
HI_SET_GET(hi_cm3_iso_stat_hifi_iso_ctrl,hifi_iso_ctrl,HI_CM3_ISO_STAT_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_ISO_STAT_OFFSET)
HI_SET_GET(hi_cm3_iso_stat_lbbp_iso_ctrl,lbbp_iso_ctrl,HI_CM3_ISO_STAT_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_ISO_STAT_OFFSET)
HI_SET_GET(hi_cm3_iso_stat_twbbp_iso_ctrl,twbbp_iso_ctrl,HI_CM3_ISO_STAT_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_ISO_STAT_OFFSET)
HI_SET_GET(hi_cm3_iso_stat_wbbp_iso_ctrl,wbbp_iso_ctrl,HI_CM3_ISO_STAT_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_ISO_STAT_OFFSET)
HI_SET_GET(hi_cm3_iso_stat_g1bbp_iso_ctrl,g1bbp_iso_ctrl,HI_CM3_ISO_STAT_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_ISO_STAT_OFFSET)
HI_SET_GET(hi_cm3_iso_stat_g2bbp_iso_ctrl,g2bbp_iso_ctrl,HI_CM3_ISO_STAT_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_ISO_STAT_OFFSET)
HI_SET_GET(hi_cm3_iso_stat_irm_iso_ctrl,irm_iso_ctrl,HI_CM3_ISO_STAT_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_ISO_STAT_OFFSET)
HI_SET_GET(hi_cm3_iso_stat_usb_iso_ctrl,usb_iso_ctrl,HI_CM3_ISO_STAT_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_ISO_STAT_OFFSET)
HI_SET_GET(hi_cm3_iso_stat_sd_iso_ctrl,sd_iso_ctrl,HI_CM3_ISO_STAT_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_ISO_STAT_OFFSET)
HI_SET_GET(hi_cm3_iso_stat_reserved_2,reserved_2,HI_CM3_ISO_STAT_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_ISO_STAT_OFFSET)
HI_SET_GET(hi_cm3_iso_stat_io_ddr_ret,io_ddr_ret,HI_CM3_ISO_STAT_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_ISO_STAT_OFFSET)
HI_SET_GET(hi_cm3_iso_stat_reserved_1,reserved_1,HI_CM3_ISO_STAT_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_ISO_STAT_OFFSET)
HI_SET_GET(hi_cm3_iso_stat_io_ret_lp_ctrl,io_ret_lp_ctrl,HI_CM3_ISO_STAT_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_ISO_STAT_OFFSET)
HI_SET_GET(hi_cm3_iso_stat_pmu_iso_ctrl,pmu_iso_ctrl,HI_CM3_ISO_STAT_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_ISO_STAT_OFFSET)
HI_SET_GET(hi_cm3_iso_stat_reserved_0,reserved_0,HI_CM3_ISO_STAT_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_ISO_STAT_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl2_appa9_mtcmos_en,appa9_mtcmos_en,HI_CM3_PWRCTRL2_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL2_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl2_moda9_mtcmos_en,moda9_mtcmos_en,HI_CM3_PWRCTRL2_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL2_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl2_peri_mtcmos_en,peri_mtcmos_en,HI_CM3_PWRCTRL2_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL2_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl2_hisc_mtcmos_en,hisc_mtcmos_en,HI_CM3_PWRCTRL2_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL2_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl2_dsp0_mtcmos_en,dsp0_mtcmos_en,HI_CM3_PWRCTRL2_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL2_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl2_hifi_mtcmos_en,hifi_mtcmos_en,HI_CM3_PWRCTRL2_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL2_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl2_lbbp_mtcmos_en,lbbp_mtcmos_en,HI_CM3_PWRCTRL2_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL2_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl2_twbbp_mtcmos_en,twbbp_mtcmos_en,HI_CM3_PWRCTRL2_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL2_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl2_wbbp_mtcmos_en,wbbp_mtcmos_en,HI_CM3_PWRCTRL2_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL2_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl2_g1bbp_mtcmos_en,g1bbp_mtcmos_en,HI_CM3_PWRCTRL2_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL2_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl2_g2bbp_mtcmos_en,g2bbp_mtcmos_en,HI_CM3_PWRCTRL2_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL2_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl2_irm_mtcmos_en,irm_mtcmos_en,HI_CM3_PWRCTRL2_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL2_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl2_usb_mtcmos_en,usb_mtcmos_en,HI_CM3_PWRCTRL2_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL2_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl2_sd_mtcmos_en,sd_mtcmos_en,HI_CM3_PWRCTRL2_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL2_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl2_reserved_1,reserved_1,HI_CM3_PWRCTRL2_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL2_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl2_pmu_pwr_on,pmu_pwr_on,HI_CM3_PWRCTRL2_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL2_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl2_reserved_0,reserved_0,HI_CM3_PWRCTRL2_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL2_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl3_appa9_mtcmos_dis,appa9_mtcmos_dis,HI_CM3_PWRCTRL3_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL3_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl3_moda9_mtcmos_dis,moda9_mtcmos_dis,HI_CM3_PWRCTRL3_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL3_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl3_peri_mtcmos_dis,peri_mtcmos_dis,HI_CM3_PWRCTRL3_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL3_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl3_hisc_mtcmos_dis,hisc_mtcmos_dis,HI_CM3_PWRCTRL3_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL3_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl3_dsp0_mtcmos_dis,dsp0_mtcmos_dis,HI_CM3_PWRCTRL3_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL3_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl3_hifi_mtcmos_dis,hifi_mtcmos_dis,HI_CM3_PWRCTRL3_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL3_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl3_lbbp_mtcmos_dis,lbbp_mtcmos_dis,HI_CM3_PWRCTRL3_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL3_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl3_twbbp_mtcmos_dis,twbbp_mtcmos_dis,HI_CM3_PWRCTRL3_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL3_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl3_wbbp_mtcmos_dis,wbbp_mtcmos_dis,HI_CM3_PWRCTRL3_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL3_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl3_g1bbp_mtcmos_dis,g1bbp_mtcmos_dis,HI_CM3_PWRCTRL3_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL3_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl3_g2bbp_mtcmos_dis,g2bbp_mtcmos_dis,HI_CM3_PWRCTRL3_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL3_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl3_irm_mtcmos_dis,irm_mtcmos_dis,HI_CM3_PWRCTRL3_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL3_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl3_usb_mtcmos_dis,usb_mtcmos_dis,HI_CM3_PWRCTRL3_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL3_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl3_sd_mtcmos_dis,sd_mtcmos_dis,HI_CM3_PWRCTRL3_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL3_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl3_reersved,reersved,HI_CM3_PWRCTRL3_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL3_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl3_pmu_pwr_off,pmu_pwr_off,HI_CM3_PWRCTRL3_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL3_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl3_reserved,reserved,HI_CM3_PWRCTRL3_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL3_OFFSET)
HI_SET_GET(hi_cm3_mtcmos_stat_appa9_mtcmos_ctrl,appa9_mtcmos_ctrl,HI_CM3_MTCMOS_STAT_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_MTCMOS_STAT_OFFSET)
HI_SET_GET(hi_cm3_mtcmos_stat_moda9_mtcmos_ctrl,moda9_mtcmos_ctrl,HI_CM3_MTCMOS_STAT_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_MTCMOS_STAT_OFFSET)
HI_SET_GET(hi_cm3_mtcmos_stat_peri_mtcmos_ctrl,peri_mtcmos_ctrl,HI_CM3_MTCMOS_STAT_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_MTCMOS_STAT_OFFSET)
HI_SET_GET(hi_cm3_mtcmos_stat_hisc_mtcmos_ctrl,hisc_mtcmos_ctrl,HI_CM3_MTCMOS_STAT_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_MTCMOS_STAT_OFFSET)
HI_SET_GET(hi_cm3_mtcmos_stat_dsp0_mtcmos_ctrl,dsp0_mtcmos_ctrl,HI_CM3_MTCMOS_STAT_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_MTCMOS_STAT_OFFSET)
HI_SET_GET(hi_cm3_mtcmos_stat_hifi_mtcmos_ctrl,hifi_mtcmos_ctrl,HI_CM3_MTCMOS_STAT_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_MTCMOS_STAT_OFFSET)
HI_SET_GET(hi_cm3_mtcmos_stat_lbbp_mtcmos_ctrl,lbbp_mtcmos_ctrl,HI_CM3_MTCMOS_STAT_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_MTCMOS_STAT_OFFSET)
HI_SET_GET(hi_cm3_mtcmos_stat_twbbp_mtcmos_ctrl,twbbp_mtcmos_ctrl,HI_CM3_MTCMOS_STAT_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_MTCMOS_STAT_OFFSET)
HI_SET_GET(hi_cm3_mtcmos_stat_wbbp_mtcmos_ctrl,wbbp_mtcmos_ctrl,HI_CM3_MTCMOS_STAT_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_MTCMOS_STAT_OFFSET)
HI_SET_GET(hi_cm3_mtcmos_stat_g1bbp_mtcmos_ctrl,g1bbp_mtcmos_ctrl,HI_CM3_MTCMOS_STAT_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_MTCMOS_STAT_OFFSET)
HI_SET_GET(hi_cm3_mtcmos_stat_g2bbp_mtcmos_ctrl,g2bbp_mtcmos_ctrl,HI_CM3_MTCMOS_STAT_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_MTCMOS_STAT_OFFSET)
HI_SET_GET(hi_cm3_mtcmos_stat_irm_mtcmos_ctrl,irm_mtcmos_ctrl,HI_CM3_MTCMOS_STAT_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_MTCMOS_STAT_OFFSET)
HI_SET_GET(hi_cm3_mtcmos_stat_usb_mtcmos_ctrl,usb_mtcmos_ctrl,HI_CM3_MTCMOS_STAT_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_MTCMOS_STAT_OFFSET)
HI_SET_GET(hi_cm3_mtcmos_stat_sd_mtcmos_ctrl,sd_mtcmos_ctrl,HI_CM3_MTCMOS_STAT_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_MTCMOS_STAT_OFFSET)
HI_SET_GET(hi_cm3_mtcmos_stat_reserved_1,reserved_1,HI_CM3_MTCMOS_STAT_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_MTCMOS_STAT_OFFSET)
HI_SET_GET(hi_cm3_mtcmos_stat_pmu_pwr_on,pmu_pwr_on,HI_CM3_MTCMOS_STAT_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_MTCMOS_STAT_OFFSET)
HI_SET_GET(hi_cm3_mtcmos_stat_reserved_0,reserved_0,HI_CM3_MTCMOS_STAT_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_MTCMOS_STAT_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl4_peri_clk_en,peri_clk_en,HI_CM3_PWRCTRL4_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL4_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl4_peri_srst_req,peri_srst_req,HI_CM3_PWRCTRL4_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL4_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl4_pmu_clk_en,pmu_clk_en,HI_CM3_PWRCTRL4_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL4_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl4_pmu_srst_req,pmu_srst_req,HI_CM3_PWRCTRL4_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL4_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl4_tcxo_en,tcxo_en,HI_CM3_PWRCTRL4_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL4_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl4_pwrctrl_bypass,pwrctrl_bypass,HI_CM3_PWRCTRL4_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL4_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl4_clk_sw_req,clk_sw_req,HI_CM3_PWRCTRL4_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL4_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl4_reserved,reserved,HI_CM3_PWRCTRL4_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL4_OFFSET)
HI_SET_GET(hi_cm3_pwrctrl4_debug_sel,debug_sel,HI_CM3_PWRCTRL4_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRCTRL4_OFFSET)
HI_SET_GET(hi_cm3_pwrstat0_mtcmos_ctrl_stat,mtcmos_ctrl_stat,HI_CM3_PWRSTAT0_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRSTAT0_OFFSET)
HI_SET_GET(hi_cm3_pwrstat0_reserved,reserved,HI_CM3_PWRSTAT0_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRSTAT0_OFFSET)
HI_SET_GET(hi_cm3_pwrstat1_iso_ctrl_stat,iso_ctrl_stat,HI_CM3_PWRSTAT1_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRSTAT1_OFFSET)
HI_SET_GET(hi_cm3_pwrstat1_reserved,reserved,HI_CM3_PWRSTAT1_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_PWRSTAT1_OFFSET)
HI_SET_GET(hi_cm3_debug_stat_debug_stat,debug_stat,HI_CM3_DEBUG_STAT_T,HI_PWRCTRL_BASE_ADDR, HI_CM3_DEBUG_STAT_OFFSET)

#endif

#endif // __HI_PWRCTRL_H__

