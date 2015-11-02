/******************************************************************************/
/*  Copyright (C), 2007-2013, Hisilicon Technologies Co., Ltd. */
/******************************************************************************/
/* File name     : hi_pwm.h */
/* Version       : 2.0 */
/* Author        : x00195528*/
/* Created       : 2013-09-30*/
/* Last Modified : */
/* Description   :  The C union definition file for the module pwm*/
/* Function List : */
/* History       : */
/* 1 Date        : */
/* Author        : x00195528*/
/* Modification  : Create file */
/******************************************************************************/

#ifndef __HI_PWM_H__
#define __HI_PWM_H__

/*
 * Project: hi
 * Module : pwm
 */

#ifndef HI_SET_GET
#define HI_SET_GET(a0,a1,a2,a3,a4)
#endif

/********************************************************************************/
/*    pwm 寄存器偏移定义（项目名_模块名_寄存器名_OFFSET)        */
/********************************************************************************/
#define    HI_PWM_OUTA_EN_OFFSET                             (0x0) /* PWM输出1使能信号配置寄存器 */
#define    HI_PWM_OUTB_EN_OFFSET                             (0x4) /* PWM输出2使能信号配置寄存器。 */
#define    HI_PWM_OUTA_DIV_OFFSET                            (0x8) /* PWM输出1分频比指示信号寄存器。 */
#define    HI_PWM_OUTB_DIV_OFFSET                            (0xC) /* PWM输出2分频比指示信号寄存器。 */
#define    HI_PWM_OUTA_WIDE_OFFSET                           (0x10) /* PWM输出1占空比调制寄存器 */
#define    HI_PWM_OUTB_WIDE_OFFSET                           (0x14) /* PWM输出2占空比调制寄存器 */
#define    HI_PWM_OUTA_WARN_OFFSET                           (0x18) /* PWM输出1模式警告寄存器。 */
#define    HI_PWM_OUTB_WARN_OFFSET                           (0x1C) /* PWM输出2模式警告寄存器。 */

/********************************************************************************/
/*    pwm 寄存器定义（项目名_模块名_寄存器名_T)        */
/********************************************************************************/
typedef union
{
    struct
    {
        unsigned int    OUTA_EN                    : 1; /* [0..0] PWM输出1使能:1’b0：输出为低电平；1’b1：输出为调制后信号 */
        unsigned int    reserved                   : 31; /* [31..1] 保留位。读时返回0。写时无影响。 */
    } bits;
    unsigned int    u32;
}HI_PWM_OUTA_EN_T;    /* PWM输出1使能信号配置寄存器 */

typedef union
{
    struct
    {
        unsigned int    OUTB_EN                    : 1; /* [0..0] PWM输出2使能:1’b0：输出为低电平；1’b1：输出为调制后信号 */
        unsigned int    reserved                   : 31; /* [31..1] 保留位。读时返回0。写时无影响。 */
    } bits;
    unsigned int    u32;
}HI_PWM_OUTB_EN_T;    /* PWM输出2使能信号配置寄存器。 */

typedef union
{
    struct
    {
        unsigned int    OUTA_DIV                   : 11; /* [10..0] 指示分频系数。11’b00000000001：分频比1：1；11’b00000000010：分频比2：1；11’b00000000011：分频比3：1；11’b00000000100：分频比4：1；…… ……11’b10000000000：分频比1024：1；其他值：无意义。 */
        unsigned int    reserved                   : 21; /* [31..11] 保留位。读时返回0。写时无影响。 */
    } bits;
    unsigned int    u32;
}HI_PWM_OUTA_DIV_T;    /* PWM输出1分频比指示信号寄存器。 */

typedef union
{
    struct
    {
        unsigned int    OUTB_DIV                   : 11; /* [10..0] 指示分频系数。11’b00000000001：分频比1：1；11’b00000000010：分频比2：1；11’b00000000011：分频比3：1；11’b00000000100：分频比4：1；…… ……11’b10000000000：分频比1024：1；其他值：无意义。 */
        unsigned int    reserved                   : 21; /* [31..11] 保留位。读时返回0。写时无影响。 */
    } bits;
    unsigned int    u32;
}HI_PWM_OUTB_DIV_T;    /* PWM输出2分频比指示信号寄存器。 */

typedef union
{
    struct
    {
        unsigned int    OUTA_WIDE                  : 11; /* [10..0] 指示占空比，假设分频比为N：0：占空比为0（始终为低电平）；1：占空比为1/N；2：占空比为2/N；…… ……N：占空比为1（始终为高电平）；特殊格式指示：当PWM_OUTA_DIV寄存器值为0而PWM_OUTA_WIDE寄存器值为0时，占空比为1/2，即输出脉冲为参考时钟本身。 */
        unsigned int    reserved                   : 21; /* [31..11] 保留位。读时返回0。写时无影响。 */
    } bits;
    unsigned int    u32;
}HI_PWM_OUTA_WIDE_T;    /* PWM输出1占空比调制寄存器 */

typedef union
{
    struct
    {
        unsigned int    OUTB_WIDE                  : 11; /* [10..0] 指示占空比，假设分频比为N：0：占空比为0（始终为低电平）；1：占空比为1/N；2：占空比为2/N；…… ……N：占空比为1（始终为高电平）；特殊格式指示：当PWM_OUTA_DIV寄存器值为0而PWM_OUTA_WIDE寄存器值为0时，占空比为1/2，即输出脉冲为参考时钟本身。 */
        unsigned int    reserved                   : 21; /* [31..11] 保留。 */
    } bits;
    unsigned int    u32;
}HI_PWM_OUTB_WIDE_T;    /* PWM输出2占空比调制寄存器 */

typedef union
{
    struct
    {
        unsigned int    OUTA_WARN                  : 1; /* [0..0] 1’b1：PWM_OUTA_WIDE中值大于PWM_OUTA_DIV中值，同时在无参考时钟输入时指示输出高电平；1’b0：PWM_OUTA_WIDE中值小于或等于PWM_OUTA_DIV中值，同时在无参考时钟输入时指示输出低电平。 */
        unsigned int    reserved                   : 31; /* [31..1] 保留位。读时返回0。写时无影响。 */
    } bits;
    unsigned int    u32;
}HI_PWM_OUTA_WARN_T;    /* PWM输出1模式警告寄存器。 */

typedef union
{
    struct
    {
        unsigned int    OUTB_WARN                  : 1; /* [0..0] 1’b1：PWM_OUTB_WIDE中值大于PWM_OUTB_DIV中值，同时在无参考时钟输入时指示输出高电平；1’b0：PWM_OUTB_WIDE中值小于或等于PWM_OUTB_DIV中值，同时在无参考时钟输入时指示输出低电平。 */
        unsigned int    reserved                   : 31; /* [31..1] 保留位。读时返回0。写时无影响。 */
    } bits;
    unsigned int    u32;
}HI_PWM_OUTB_WARN_T;    /* PWM输出2模式警告寄存器。 */


/********************************************************************************/
/*    pwm 函数（项目名_模块名_寄存器名_成员名_set)        */
/********************************************************************************/
HI_SET_GET(hi_pwm_outa_en_outa_en,outa_en,HI_PWM_OUTA_EN_T,HI_PWM_BASE_ADDR, HI_PWM_OUTA_EN_OFFSET)
HI_SET_GET(hi_pwm_outa_en_reserved,reserved,HI_PWM_OUTA_EN_T,HI_PWM_BASE_ADDR, HI_PWM_OUTA_EN_OFFSET)
HI_SET_GET(hi_pwm_outb_en_outb_en,outb_en,HI_PWM_OUTB_EN_T,HI_PWM_BASE_ADDR, HI_PWM_OUTB_EN_OFFSET)
HI_SET_GET(hi_pwm_outb_en_reserved,reserved,HI_PWM_OUTB_EN_T,HI_PWM_BASE_ADDR, HI_PWM_OUTB_EN_OFFSET)
HI_SET_GET(hi_pwm_outa_div_outa_div,outa_div,HI_PWM_OUTA_DIV_T,HI_PWM_BASE_ADDR, HI_PWM_OUTA_DIV_OFFSET)
HI_SET_GET(hi_pwm_outa_div_reserved,reserved,HI_PWM_OUTA_DIV_T,HI_PWM_BASE_ADDR, HI_PWM_OUTA_DIV_OFFSET)
HI_SET_GET(hi_pwm_outb_div_outb_div,outb_div,HI_PWM_OUTB_DIV_T,HI_PWM_BASE_ADDR, HI_PWM_OUTB_DIV_OFFSET)
HI_SET_GET(hi_pwm_outb_div_reserved,reserved,HI_PWM_OUTB_DIV_T,HI_PWM_BASE_ADDR, HI_PWM_OUTB_DIV_OFFSET)
HI_SET_GET(hi_pwm_outa_wide_outa_wide,outa_wide,HI_PWM_OUTA_WIDE_T,HI_PWM_BASE_ADDR, HI_PWM_OUTA_WIDE_OFFSET)
HI_SET_GET(hi_pwm_outa_wide_reserved,reserved,HI_PWM_OUTA_WIDE_T,HI_PWM_BASE_ADDR, HI_PWM_OUTA_WIDE_OFFSET)
HI_SET_GET(hi_pwm_outb_wide_outb_wide,outb_wide,HI_PWM_OUTB_WIDE_T,HI_PWM_BASE_ADDR, HI_PWM_OUTB_WIDE_OFFSET)
HI_SET_GET(hi_pwm_outb_wide_reserved,reserved,HI_PWM_OUTB_WIDE_T,HI_PWM_BASE_ADDR, HI_PWM_OUTB_WIDE_OFFSET)
HI_SET_GET(hi_pwm_outa_warn_outa_warn,outa_warn,HI_PWM_OUTA_WARN_T,HI_PWM_BASE_ADDR, HI_PWM_OUTA_WARN_OFFSET)
HI_SET_GET(hi_pwm_outa_warn_reserved,reserved,HI_PWM_OUTA_WARN_T,HI_PWM_BASE_ADDR, HI_PWM_OUTA_WARN_OFFSET)
HI_SET_GET(hi_pwm_outb_warn_outb_warn,outb_warn,HI_PWM_OUTB_WARN_T,HI_PWM_BASE_ADDR, HI_PWM_OUTB_WARN_OFFSET)
HI_SET_GET(hi_pwm_outb_warn_reserved,reserved,HI_PWM_OUTB_WARN_T,HI_PWM_BASE_ADDR, HI_PWM_OUTB_WARN_OFFSET)

#endif // __HI_PWM_H__

