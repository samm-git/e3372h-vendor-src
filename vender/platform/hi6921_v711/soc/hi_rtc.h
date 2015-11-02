/******************************************************************************/
/*  Copyright (C), 2007-2013, Hisilicon Technologies Co., Ltd. */
/******************************************************************************/
/* File name     : hi_rtc.h */
/* Version       : 2.0 */
/* Author        : f00221597*/
/* Created       : 2013-02-02*/
/* Last Modified : */
/* Description   :  The C union definition file for the module rtc*/
/* Function List : */
/* History       : */
/* 1 Date        : */
/* Author        : f00221597*/
/* Modification  : Create file */
/******************************************************************************/

#ifndef __HI_RTC_H__
#define __HI_RTC_H__

/*
 * Project: hi
 * Module : rtc
 */

#ifndef HI_SET_GET
#define HI_SET_GET(a0,a1,a2,a3,a4)
#endif

/********************************************************************************/
/*    rtc 寄存器偏移定义（项目名_模块名_寄存器名_OFFSET)        */
/********************************************************************************/
#define    HI_RTC_CCVR_OFFSET                                (0x0) /* 当前计数值寄存器。 */
#define    HI_RTC_CMR_OFFSET                                 (0x4) /* 比较值寄存器。 */
#define    HI_RTC_CLR_OFFSET                                 (0x8) /* 载入值寄存器。 */
#define    HI_RTC_CCR_OFFSET                                 (0xC) /* 控制寄存器。 */
#define    HI_RTC_STAT_OFFSET                                (0x10) /* 中断状态寄存器。 */
#define    HI_RTC_RSTAT_OFFSET                               (0x14) /* 原始中断状态寄存器。 */
#define    HI_RTC_EOI_OFFSET                                 (0x18) /* 中断清除寄存器。 */

/********************************************************************************/
/*    rtc 寄存器定义（项目名_模块名_寄存器名_T)        */
/********************************************************************************/
typedef union
{
    struct
    {
        unsigned int    ccvr                       : 32; /* [31..0] 计数器当前计数值。 */
    } bits;
    unsigned int    u32;
}HI_RTC_CCVR_T;    /* 当前计数值寄存器。 */

typedef union
{
    struct
    {
        unsigned int    cmr                        : 32; /* [31..0] 在中断使能的前提下，当内部计数器的计数值与该寄存器相等时，产生中断。 */
    } bits;
    unsigned int    u32;
}HI_RTC_CMR_T;    /* 比较值寄存器。 */

typedef union
{
    struct
    {
        unsigned int    clr                        : 32; /* [31..0] 设置内部计数器初始值寄存器。 */
    } bits;
    unsigned int    u32;
}HI_RTC_CLR_T;    /* 载入值寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtc_ien                    : 1; /* [0..0] 中断使能控制位。0：不产生中断；1：产生中断。 */
        unsigned int    rtc_mask                   : 1; /* [1..1] 中断屏蔽控制位。0：不屏蔽中断；1：屏蔽中断。 */
        unsigned int    reserved                   : 30; /* [31..2] 保留。 */
    } bits;
    unsigned int    u32;
}HI_RTC_CCR_T;    /* 控制寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtc_stat                   : 1; /* [0..0] 经过屏蔽后的中断状态寄存器。0：中断无效；1：中断有效。 */
        unsigned int    reserved                   : 31; /* [31..1] 保留。 */
    } bits;
    unsigned int    u32;
}HI_RTC_STAT_T;    /* 中断状态寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtc_rstat                  : 1; /* [0..0] 未经屏蔽的中断状态寄存器。0：中断无效；1：中断有效。 */
        unsigned int    reserved                   : 31; /* [31..1] 保留。 */
    } bits;
    unsigned int    u32;
}HI_RTC_RSTAT_T;    /* 原始中断状态寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtc_eoi                    : 1; /* [0..0] 中断清除寄存器，通过读该寄存器来清除中断。 */
        unsigned int    reserved                   : 31; /* [31..1] 保留。 */
    } bits;
    unsigned int    u32;
}HI_RTC_EOI_T;    /* 中断清除寄存器。 */


/********************************************************************************/
/*    rtc 函数（项目名_模块名_寄存器名_成员名_set)        */
/********************************************************************************/
HI_SET_GET(hi_rtc_ccvr_ccvr,ccvr,HI_RTC_CCVR_T,HI_RTC_BASE_ADDR, HI_RTC_CCVR_OFFSET)
HI_SET_GET(hi_rtc_cmr_cmr,cmr,HI_RTC_CMR_T,HI_RTC_BASE_ADDR, HI_RTC_CMR_OFFSET)
HI_SET_GET(hi_rtc_clr_clr,clr,HI_RTC_CLR_T,HI_RTC_BASE_ADDR, HI_RTC_CLR_OFFSET)
HI_SET_GET(hi_rtc_ccr_rtc_ien,rtc_ien,HI_RTC_CCR_T,HI_RTC_BASE_ADDR, HI_RTC_CCR_OFFSET)
HI_SET_GET(hi_rtc_ccr_rtc_mask,rtc_mask,HI_RTC_CCR_T,HI_RTC_BASE_ADDR, HI_RTC_CCR_OFFSET)
HI_SET_GET(hi_rtc_ccr_reserved,reserved,HI_RTC_CCR_T,HI_RTC_BASE_ADDR, HI_RTC_CCR_OFFSET)
HI_SET_GET(hi_rtc_stat_rtc_stat,rtc_stat,HI_RTC_STAT_T,HI_RTC_BASE_ADDR, HI_RTC_STAT_OFFSET)
HI_SET_GET(hi_rtc_stat_reserved,reserved,HI_RTC_STAT_T,HI_RTC_BASE_ADDR, HI_RTC_STAT_OFFSET)
HI_SET_GET(hi_rtc_rstat_rtc_rstat,rtc_rstat,HI_RTC_RSTAT_T,HI_RTC_BASE_ADDR, HI_RTC_RSTAT_OFFSET)
HI_SET_GET(hi_rtc_rstat_reserved,reserved,HI_RTC_RSTAT_T,HI_RTC_BASE_ADDR, HI_RTC_RSTAT_OFFSET)
HI_SET_GET(hi_rtc_eoi_rtc_eoi,rtc_eoi,HI_RTC_EOI_T,HI_RTC_BASE_ADDR, HI_RTC_EOI_OFFSET)
HI_SET_GET(hi_rtc_eoi_reserved,reserved,HI_RTC_EOI_T,HI_RTC_BASE_ADDR, HI_RTC_EOI_OFFSET)

#endif // __HI_RTC_H__

