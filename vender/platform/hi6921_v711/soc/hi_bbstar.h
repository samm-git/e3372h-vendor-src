/******************************************************************************/
/*  Copyright (C), 2007-2014, Hisilicon Technologies Co., Ltd. */
/******************************************************************************/
/* File name     : hi_bbstar.h */
/* Version       : 2.0 */
/* Author        : m00125050*/
/* Created       : 2014-05-09*/
/* Last Modified : */
/* Description   :  The C union definition file for the module bbstar*/
/* Function List : */
/* History       : */
/* 1 Date        : */
/* Author        : m00125050*/
/* Modification  : Create file */
/******************************************************************************/

#ifndef __HI_BBSTAR_H__
#define __HI_BBSTAR_H__

/*
 * Project: hi
 * Module : bbstar
 */

/********************************************************************************/
/*    bbstar 寄存器偏移定义（项目名_模块名_寄存器名_OFFSET)        */
/********************************************************************************/
#define    HI6559_STATUS1_OFFSET                                 (0x1) /* 状态标志1寄存器。 */
#define    HI6559_STATUS2_OFFSET                                 (0x2) /* 状态标志2寄存器。 */
#define    HI6559_IRQ1_OFFSET                                    (0x3) /* 中断1寄存器。 */
#define    HI6559_IRQ2_OFFSET                                    (0x4) /* 中断2寄存器。 */
#define    HI6559_IRQ3_OFFSET                                    (0x5) /* 中断3寄存器。 */
#define    HI6559_IRQ4_OFFSET                                    (0x6) /* 中断4寄存器。 */
#define    HI6559_IRQ1_MASK_OFFSET                               (0x8) /* 中断屏蔽1寄存器。 */
#define    HI6559_IRQ2_MASK_OFFSET                               (0x9) /* 中断屏蔽2寄存器。 */
#define    HI6559_IRQ3_MASK_OFFSET                               (0xA) /* 中断屏蔽3寄存器。 */
#define    HI6559_IRQ4_MASK_OFFSET                               (0xB) /* 中断屏蔽4寄存器。 */
#define    HI6559_SCP_RECORD1_OFFSET                             (0xD) /* 短路保护记录1寄存器。 */
#define    HI6559_OCP_RECORD1_OFFSET                             (0xE) /* 过流保护记录1寄存器。 */
#define    HI6559_OCP_RECORD2_OFFSET                             (0xF) /* 过流保护记录2寄存器。 */
#define    HI6559_OCP_RECORD3_OFFSET                             (0x10) /* 过流保护记录3寄存器。 */
#define    HI6559_OCP_RECORD4_OFFSET                             (0x11) /* 过流保护记录4寄存器。 */
#define    HI6559_OCP_RECORD5_OFFSET                             (0x12) /* 过流保护记录5寄存器。 */
#define    HI6559_OCP_RECORD6_OFFSET                             (0x13) /* 过流保护记录6寄存器。 */
#define    HI6559_NP_IRQ1_RECORD_OFFSET                          (0x15) /* 中断1非下电记录寄存器。 */
#define    HI6559_NP_IRQ2_RECORD_OFFSET                          (0x16) /* 中断2非下电记录寄存器。 */
#define    HI6559_NP_IRQ3_RECORD_OFFSET                          (0x17) /* 中断3非下电记录寄存器。 */
#define    HI6559_NP_SCP_RECORD1_OFFSET                          (0x18) /* 短路保护非下电记录1寄存器。 */
#define    HI6559_NP_OCP_RECORD1_OFFSET                          (0x19) /* 过流保护非下电记录1寄存器。 */
#define    HI6559_NP_OCP_RECORD2_OFFSET                          (0x1A) /* 过流保护非下电记录2寄存器。 */
#define    HI6559_NP_OCP_RECORD3_OFFSET                          (0x1B) /* 过流保护非下电记录3寄存器。 */
#define    HI6559_NP_OCP_RECORD4_OFFSET                          (0x1C) /* 过流保护非下电记录4寄存器。 */
#define    HI6559_NP_OCP_RECORD5_OFFSET                          (0x1D) /* 过流保护非下电记录5寄存器。 */
#define    HI6559_NP_OCP_RECORD6_OFFSET                          (0x1E) /* 过流保护非下电记录6寄存器。 */
#define    HI6559_OCP_DEB_OFFSET                                 (0x20) /* OCP去抖动调节寄存器。 */
#define    HI6559_ONOFF8_OFFSET                                  (0x21) /* 开关控制8寄存器。 */
#define    HI6559_OCP_SCP_MOD_CTRL1_OFFSET                       (0x22) /* BUCK0-3过流和短路保护模式控制寄存器。 */
#define    HI6559_OCP_SCP_MOD_CTRL2_OFFSET                       (0x23) /* BUCK4-7过流和短路保护模式控制寄存器。 */
#define    HI6559_OCP_MOD_CTRL3_OFFSET                           (0x24) /* LDO1-4过流保护模式控制寄存器。 */
#define    HI6559_OCP_MOD_CTRL4_OFFSET                           (0x25) /* LDO5-8过流保护模式控制寄存器。 */
#define    HI6559_OCP_MOD_CTRL5_OFFSET                           (0x26) /* LDO9-12过流保护模式控制寄存器。 */
#define    HI6559_OCP_MOD_CTRL6_OFFSET                           (0x27) /* LDO13-16过流保护模式控制寄存器。 */
#define    HI6559_OCP_MOD_CTRL7_OFFSET                           (0x29) /* LDO21-ldo23过流保护模式控制寄存器。 */
#define    HI6559_OCP_MOD_CTRL8_OFFSET                           (0x2B) /* LVS7-9过流保护模式控制寄存器。 */
#define    HI6559_ENABLE1_OFFSET                                 (0x2D) /* 电源模块使能1寄存器。 */
#define    HI6559_DISABLE1_OFFSET                                (0x2E) /* 电源模块关闭1寄存器。 */
#define    HI6559_ONOFF_STATUS1_OFFSET                           (0x2F) /* 电源模块开关状态1寄存器。 */
#define    HI6559_ENABLE2_OFFSET                                 (0x30) /* 电源模块使能2寄存器。 */
#define    HI6559_DISABLE2_OFFSET                                (0x31) /* 电源模块关闭2寄存器。 */
#define    HI6559_ONOFF_STATUS2_OFFSET                           (0x32) /* 电源模块开关状态2寄存器。 */
#define    HI6559_ENABLE3_OFFSET                                 (0x33) /* 电源模块使能3寄存器。 */
#define    HI6559_DISABLE3_OFFSET                                (0x34) /* 电源模块关闭3寄存器。 */
#define    HI6559_ONOFF_STATUS3_OFFSET                           (0x35) /* 电源模块开关状态3寄存器。 */
#define    HI6559_ENABLE4_OFFSET                                 (0x36) /* 电源模块使能4寄存器。 */
#define    HI6559_DISABLE4_OFFSET                                (0x37) /* 电源模块关闭4寄存器。 */
#define    HI6559_ONOFF_STATUS4_OFFSET                           (0x38) /* 电源模块开关状态4寄存器。 */
#define    HI6559_ENABLE5_OFFSET                                 (0x39) /* 电源模块使能5寄存器。 */
#define    HI6559_DISABLE5_OFFSET                                (0x3A) /* 电源模块关闭5寄存器。 */
#define    HI6559_ONOFF_STATUS5_OFFSET                           (0x3B) /* 电源模块开关状态5寄存器。 */
#define    HI6559_SIM_LDO9_LDO11_EN_OFFSET                       (0x40) /* SIM卡/LDO9/LDO11使能寄存器。 */
#define    HI6559_SIM_CFG_OFFSET                                 (0x41) /* SIM卡配置寄存器。 */
#define    HI6559_SIM_DEB_OFFSET                                 (0x43) /* SIM卡中断去抖寄调节寄存器。 */
#define    HI6559_ECO_MOD_CFG1_OFFSET                            (0x44) /* 电源模块是否跟随PMU进出ECO模式配置1寄存器。 */
#define    HI6559_ECO_MOD_CFG2_OFFSET                            (0x45) /* 电源模块是否跟随PMU进出ECO模式配置2寄存器。 */
#define    HI6559_ECO_MOD_CFG3_OFFSET                            (0x46) /* 电源是否强制随PMU进出ECO模式配置3寄存器。 */
#define    HI6559_ECO_MOD_CFG4_OFFSET                            (0x47) /* 电源是否强制随PMU进出ECO模式配置4寄存器。 */
#define    HI6559_ENB3_ECO_MARK_OFFSET                           (0x48) /* 与ENB3硬线和ECO功能相关的屏蔽信号寄存器。 */
#define    HI6559_BUCK0_REG1_ADJ_OFFSET                          (0x4A) /* BUCK0调节1寄存器。 */
#define    HI6559_BUCK0_REG2_ADJ_OFFSET                          (0x52) /* BUCK0调节2寄存器。 */
#define    HI6559_BUCK0_REG3_ADJ_OFFSET                          (0x56) /* BUCK0调节3寄存器。 */
#define    HI6559_BUCK0_REG4_ADJ_OFFSET                          (0x57) /* BUCK0调节4寄存器。 */
#define    HI6559_BUCK0_REG5_ADJ_OFFSET                          (0x58) /* BUCK0调节5寄存器。 */
#define    HI6559_BUCK0_REG6_ADJ_OFFSET                          (0x59) /* BUCK0调节6寄存器。 */
#define    HI6559_BUCK0_REG7_ADJ_OFFSET                          (0x5A) /* BUCK0调节7寄存器。 */
#define    HI6559_BUCK0_REG8_ADJ_OFFSET                          (0x5C) /* BUCK0调节8寄存器。 */
#define    HI6559_BUCK0_REG9_ADJ_OFFSET                          (0x5D) /* BUCK0调节9寄存器。 */
#define    HI6559_BUCK0_REG10_ADJ_OFFSET                         (0x5E) /* BUCK0调节10寄存器。 */
#define    HI6559_BUCK0_REG11_ADJ_OFFSET                         (0x5F) /* BUCK0调节11寄存器。 */
#define    HI6559_BUCK0_REG12_ADJ_OFFSET                         (0x60) /* BUCK0调节12寄存器。 */
#define    HI6559_BUCK0_REG13_ADJ_OFFSET                         (0x61) /* BUCK0调节13寄存器。 */
#define    HI6559_BUCK0_REG14_ADJ_OFFSET                         (0x62) /* BUCK0调节14寄存器。 */
#define    HI6559_BUCK0_REG15_ADJ_OFFSET                         (0x63) /* BUCK0调节15寄存器。 */
#define    HI6559_BUCK0_REG16_ADJ_OFFSET                         (0x65) /* BUCK0调节16寄存器。 */
#define    HI6559_BUCK3_REG1_ADJ_OFFSET                          (0x66) /* BUCK3调节1寄存器。 */
#define    HI6559_BUCK3_REG2_ADJ_OFFSET                          (0x67) /* BUCK3调节2寄存器。 */
#define    HI6559_BUCK3_REG3_ADJ_OFFSET                          (0x68) /* BUCK3调节3寄存器。 */
#define    HI6559_BUCK3_REG4_ADJ_OFFSET                          (0x69) /* BUCK3调节4寄存器。 */
#define    HI6559_BUCK3_REG5_ADJ_OFFSET                          (0x6A) /* BUCK3调节5寄存器。 */
#define    HI6559_BUCK3_REG6_ADJ_OFFSET                          (0x6B) /* BUCK3调节6寄存器。 */
#define    HI6559_BUCK3_REG7_ADJ_OFFSET                          (0x6C) /* BUCK3调节7寄存器。 */
#define    HI6559_BUCK3_REG8_ADJ_OFFSET                          (0x6D) /* BUCK3调节8寄存器。 */
#define    HI6559_BUCK3_REG9_ADJ_OFFSET                          (0x6E) /* BUCK3调节9寄存器。 */
#define    HI6559_BUCK3_REG10_ADJ_OFFSET                         (0x6F) /* BUCK3调节10寄存器。 */
#define    HI6559_BUCK5_REG0_ADJ_OFFSET                          (0x71) /* BUCK3调节12寄存器。 */
#define    HI6559_BUCK4_REG1_ADJ_OFFSET                          (0x72) /* BUCK4调节1寄存器。 */
#define    HI6559_BUCK4_REG2_ADJ_OFFSET                          (0x73) /* BUCK4调节2寄存器。 */
#define    HI6559_BUCK4_REG3_ADJ_OFFSET                          (0x74) /* BUCK4调节3寄存器。 */
#define    HI6559_BUCK4_REG4_ADJ_OFFSET                          (0x75) /* BUCK4调节4寄存器。 */
#define    HI6559_BUCK4_REG5_ADJ_OFFSET                          (0x76) /* BUCK4调节5寄存器。 */
#define    HI6559_BUCK4_REG6_ADJ_OFFSET                          (0x77) /* BUCK4调节6寄存器。 */
#define    HI6559_BUCK4_REG7_ADJ_OFFSET                          (0x78) /* BUCK4调节7寄存器。 */
#define    HI6559_BUCK4_REG8_ADJ_OFFSET                          (0x79) /* BUCK4调节8寄存器。 */
#define    HI6559_BUCK4_REG9_ADJ_OFFSET                          (0x7A) /* BUCK4调节9寄存器。 */
#define    HI6559_BUCK4_REG10_ADJ_OFFSET                         (0x7B) /* BUCK4调节10寄存器。 */
#define    HI6559_BUCK4_REG12_ADJ_OFFSET                         (0x7C) /* BUCK4调节12寄存器。 */
#define    HI6559_BUCK5_REG1_ADJ_OFFSET                          (0x7D) /* BUCK5调节1寄存器。 */
#define    HI6559_BUCK5_REG2_ADJ_OFFSET                          (0x7E) /* BUCK5调节2寄存器。 */
#define    HI6559_BUCK5_REG3_ADJ_OFFSET                          (0x7F) /* BUCK5调节3寄存器。 */
#define    HI6559_BUCK5_REG4_ADJ_OFFSET                          (0x80) /* BUCK5调节4寄存器。 */
#define    HI6559_BUCK5_REG5_ADJ_OFFSET                          (0x81) /* BUCK5调节5寄存器。 */
#define    HI6559_BUCK5_REG6_ADJ_OFFSET                          (0x83) /* BUCK5调节6寄存器。 */
#define    HI6559_BUCK5_REG7_ADJ_OFFSET                          (0x84) /* BUCK5调节7寄存器。 */
#define    HI6559_BUCK5_REG8_ADJ_OFFSET                          (0x85) /* BUCK5调节8寄存器。 */
#define    HI6559_BUCK5_REG9_ADJ_OFFSET                          (0x87) /* BUCK5调节9寄存器。 */
#define    HI6559_BUCK5_REG10_ADJ_OFFSET                         (0x88) /* BUCK5调节10寄存器。 */
#define    HI6559_BUCK6_REG1_ADJ_OFFSET                          (0x8A) /* BUCK6调节1寄存器。 */
#define    HI6559_BUCK6_REG2_ADJ_OFFSET                          (0x8B) /* BUCK6调节2寄存器。 */
#define    HI6559_BUCK6_REG3_ADJ_OFFSET                          (0x8C) /* BUCK6调节3寄存器。 */
#define    HI6559_BUCK6_REG4_ADJ_OFFSET                          (0x8D) /* BUCK6调节4寄存器。 */
#define    HI6559_BUCK6_REG5_ADJ_OFFSET                          (0x8E) /* BUCK6调节5寄存器。 */
#define    HI6559_BUCK6_REG6_ADJ_OFFSET                          (0x8F) /* BUCK6调节6寄存器。 */
#define    HI6559_BUCK6_REG7_ADJ_OFFSET                          (0x90) /* BUCK6调节7寄存器。 */
#define    HI6559_BUCK6_REG8_ADJ_OFFSET                          (0x91) /* BUCK6调节8寄存器。 */
#define    HI6559_BUCK6_REG9_ADJ_OFFSET                          (0x92) /* BUCK6调节9寄存器。 */
#define    HI6559_BUCK6_REG10_ADJ_OFFSET                         (0x93) /* BUCK6调节10寄存器。 */
#define    HI6559_BUCK6_REG12_ADJ_OFFSET                         (0x95) /* BUCK6调节12寄存器。 */
#define    HI6559_VSET_BUCK0_ADJ_OFFSET                          (0x98) /* BUCK0电压调节寄存器。 */
#define    HI6559_VSET_BUCK3_ADJ_OFFSET                          (0x99) /* BUCK3电压调节寄存器。 */
#define    HI6559_VSET_BUCK4_ADJ_OFFSET                          (0x9A) /* BUCK4电压调节寄存器。 */
#define    HI6559_VSET_BUCK5_ADJ_OFFSET                          (0x9B) /* BUCK5电压调节寄存器。 */
#define    HI6559_VSET_BUCK6_ADJ_OFFSET                          (0x9C) /* BUCK6电压调节寄存器。 */
#define    HI6559_LDO1_REG_ADJ_OFFSET                            (0x9D) /* LDO1调节寄存器。 */
#define    HI6559_LDO3_REG_ADJ_OFFSET                            (0x9F) /* LDO3调节寄存器。 */
#define    HI6559_LDO6_REG_ADJ_OFFSET                            (0xA2) /* LDO6调节寄存器。 */
#define    HI6559_LDO7_REG_ADJ_OFFSET                            (0xA3) /* LDO7调节寄存器。 */
#define    HI6559_LDO8_REG_ADJ_OFFSET                            (0xA4) /* LDO8调节寄存器。 */
#define    HI6559_LDO9_REG_ADJ_OFFSET                            (0xA5) /* LDO9调节寄存器。 */
#define    HI6559_LDO10_REG_ADJ_OFFSET                           (0xA6) /* LDO10调节寄存器。 */
#define    HI6559_LDO11_REG_ADJ_OFFSET                           (0xA7) /* LDO11调节寄存器。 */
#define    HI6559_LDO12_REG_ADJ_OFFSET                           (0xA8) /* LDO12调节寄存器。 */
#define    HI6559_LDO13_REG_ADJ_OFFSET                           (0xA9) /* LDO13调节寄存器。 */
#define    HI6559_LDO14_REG_ADJ_OFFSET                           (0xAA) /* LDO14调节寄存器。 */
#define    HI6559_LDO22_REG_ADJ_OFFSET                           (0xB2) /* LDO22调节寄存器。 */
#define    HI6559_LDO23_REG_ADJ_OFFSET                           (0xB3) /* LDO23调节寄存器。 */
#define    HI6559_LDO24_REG_ADJ_OFFSET                           (0xB4) /* LDO24调节寄存器。 */
#define    HI6559_PMUA_REG_ADJ_OFFSET                            (0xB5) /* PMUA调节寄存器。 */
#define    HI6559_LVS_ADJ1_OFFSET                                (0xB7) /* LVS调节1寄存器。 */
#define    HI6559_BANDGAP_THSD_ADJ1_OFFSET                       (0xC8) /* BANDGAP和THSD调节1寄存器。 */
#define    HI6559_BANDGAP_THSD_ADJ2_OFFSET                       (0xC9) /* BANDGAP和THSD调节2寄存器。 */
#define    HI6559_DR_FLA_CTRL1_OFFSET                            (0xCA) /* DR闪烁模式控制1寄存器。 */
#define    HI6559_DR_FLA_CTRL2_OFFSET                            (0xCB) /* DR闪烁模式控制2寄存器。 */
#define    HI6559_FLASH_PERIOD_OFFSET                            (0xCC) /* DR闪烁模式周期调节寄存器。 */
#define    HI6559_FLASH_ON_OFFSET                                (0xCD) /* DR闪烁模式点亮时间调节寄存器。 */
#define    HI6559_DR_MODE_SEL_OFFSET                             (0xCE) /* DR灯模式选择寄存器。 */
#define    HI6559_DR_BRE_CTRL_OFFSET                             (0xCF) /* DR1/2呼吸模式控制寄存器。 */
#define    HI6559_DR1_TIM_CONF0_OFFSET                           (0xD0) /* DR1常亮常暗时间配置寄存器。 */
#define    HI6559_DR1_TIM_CONF1_OFFSET                           (0xD1) /* DR1渐亮渐暗时间配置寄存器。 */
#define    HI6559_DR1_ISET_OFFSET                                (0xD2) /* DR1电流选择寄存器。 */
#define    HI6559_DR2_TIM_CONF0_OFFSET                           (0xD3) /* DR2常亮常暗时间配置寄存器。 */
#define    HI6559_DR2_TIM_CONF1_OFFSET                           (0xD4) /* DR2渐亮渐暗时间配置寄存器。 */
#define    HI6559_DR2_ISET_OFFSET                                (0xD5) /* DR2电流选择寄存器。 */
#define    HI6559_DR_LED_CTRL_OFFSET                             (0xD6) /* DR3/4/5控制寄存器。 */
#define    HI6559_DR_OUT_CTRL_OFFSET                             (0xD7) /* DR3/4/5输出控制寄存器。 */
#define    HI6559_DR3_ISET_OFFSET                                (0xD8) /* DR3电流选择寄存器。 */
#define    HI6559_DR3_START_DEL_OFFSET                           (0xD9) /* DR3启动延时配置寄存器。 */
#define    HI6559_DR3_TIM_CONF0_OFFSET                           (0xDA) /* DR3常亮常暗时间配置寄存器。 */
#define    HI6559_DR3_TIM_CONF1_OFFSET                           (0xDB) /* DR3渐亮渐暗时间配置寄存器。 */
#define    HI6559_DR4_ISET_OFFSET                                (0xDC) /* DR4电流选择寄存器。 */
#define    HI6559_DR4_START_DEL_OFFSET                           (0xDD) /* DR4启动延时配置寄存器 */
#define    HI6559_DR4_TIM_CONF0_OFFSET                           (0xDE) /* DR4常亮常暗时间配置寄存器。 */
#define    HI6559_DR4_TIM_CONF1_OFFSET                           (0xDF) /* DR4渐亮渐暗时间配置寄存器。 */
#define    HI6559_DR5_ISET_OFFSET                                (0xE0) /* DR5电流选择寄存器。 */
#define    HI6559_DR5_START_DEL_OFFSET                           (0xE1) /* DR5启动延时配置寄存器。 */
#define    HI6559_DR5_TIM_CONF0_OFFSET                           (0xE2) /* DR5常亮常暗时间配置寄存器。 */
#define    HI6559_DR5_TIM_CONF1_OFFSET                           (0xE3) /* DR5渐亮渐暗时间配置寄存器。 */
#define    HI6559_OTP_CTRL1_OFFSET                               (0xE4) /* OTP控制1信号寄存器。 */
#define    HI6559_OTP_CTRL2_OFFSET                               (0xE5) /* OTP控制2信号寄存器。 */
#define    HI6559_OTP_PDIN_OFFSET                                (0xE6) /* OTP写入值寄存器。 */
#define    HI6559_OTP_PDOB0_OFFSET                               (0xE7) /* OTP读出值0寄存器。 */
#define    HI6559_OTP_PDOB1_OFFSET                               (0xE8) /* OTP读出值1寄存器。 */
#define    HI6559_OTP_PDOB2_OFFSET                               (0xE9) /* OTP读出值2寄存器。 */
#define    HI6559_OTP_PDOB3_OFFSET                               (0xEA) /* OTP读出值3寄存器。 */
#define    HI6559_RTCCR_A0_OFFSET                                (0xEB) /* RTC_A数据0寄存器。 */
#define    HI6559_RTCCR_A1_OFFSET                                (0xEC) /* RTC_A数据1寄存器。 */
#define    HI6559_RTCCR_A2_OFFSET                                (0xED) /* RTC_A数据2寄存器。 */
#define    HI6559_RTCCR_A3_OFFSET                                (0xEE) /* RTC_A数据3寄存器。 */
#define    HI6559_RTCLR_A0_OFFSET                                (0xEF) /* RTCLR_A加载0寄存器。 */
#define    HI6559_RTCLR_A1_OFFSET                                (0xF0) /* RTCLR_A加载1寄存器。 */
#define    HI6559_RTCLR_A2_OFFSET                                (0xF1) /* RTCLR_A加载2寄存器。 */
#define    HI6559_RTCLR_A3_OFFSET                                (0xF2) /* RTCLR_A加载3寄存器。 */
#define    HI6559_RTCCTRL_A_OFFSET                               (0xF3) /* RTCCTRL_A控制寄存器。 */
#define    HI6559_RTCMR_A_A0_OFFSET                              (0xF4) /* RTC_A_A比较0寄存器。 */
#define    HI6559_RTCMR_A_A1_OFFSET                              (0xF5) /* RTC_A_A比较1寄存器。 */
#define    HI6559_RTCMR_A_A2_OFFSET                              (0xF6) /* RTC_A_A比较2寄存器。 */
#define    HI6559_RTCMR_A_A3_OFFSET                              (0xF7) /* RTC_A_A比较3寄存器。 */
#define    HI6559_RTCMR_A_B0_OFFSET                              (0xF8) /* RTC_A_B比较0寄存器。 */
#define    HI6559_RTCMR_A_B1_OFFSET                              (0xF9) /* RTC_A_B比较1寄存器。 */
#define    HI6559_RTCMR_A_B2_OFFSET                              (0xFA) /* RTC_A_B比较2寄存器。 */
#define    HI6559_RTCMR_A_B3_OFFSET                              (0xFB) /* RTC_A_B比较3寄存器。 */
#define    HI6559_RTCMR_A_C0_OFFSET                              (0xFC) /* RTC_A_C比较0寄存器。 */
#define    HI6559_RTCMR_A_C1_OFFSET                              (0xFD) /* RTC_A_C比较1寄存器。 */
#define    HI6559_RTCMR_A_C2_OFFSET                              (0xFE) /* RTC_A_C比较2寄存器。 */
#define    HI6559_RTCMR_A_C3_OFFSET                              (0xFF) /* RTC_A_C比较3寄存器。 */
#define    HI6559_RTCMR_A_D0_OFFSET                              (0x100) /* RTC_A_D比较0寄存器。 */
#define    HI6559_RTCMR_A_D1_OFFSET                              (0x101) /* RTC_A_D比较1寄存器。 */
#define    HI6559_RTCMR_A_D2_OFFSET                              (0x102) /* RTC_A_D比较2寄存器。 */
#define    HI6559_RTCMR_A_D3_OFFSET                              (0x103) /* RTC_A_D比较3寄存器。 */
#define    HI6559_RTCMR_A_E0_OFFSET                              (0x104) /* RTC_A_E比较0寄存器。 */
#define    HI6559_RTCMR_A_E1_OFFSET                              (0x105) /* RTC_A_E比较1寄存器。 */
#define    HI6559_RTCMR_A_E2_OFFSET                              (0x106) /* RTC_A_E比较2寄存器。 */
#define    HI6559_RTCMR_A_E3_OFFSET                              (0x107) /* RTC_A_E比较3寄存器。 */
#define    HI6559_RTCMR_A_F0_OFFSET                              (0x108) /* RTC_A_F比较0寄存器。 */
#define    HI6559_RTCMR_A_F1_OFFSET                              (0x109) /* RTC_A_F比较1寄存器。 */
#define    HI6559_RTCMR_A_F2_OFFSET                              (0x10A) /* RTC_A_F比较2寄存器。 */
#define    HI6559_RTCMR_A_F3_OFFSET                              (0x10B) /* RTC_A_F比较3寄存器。 */
#define    HI6559_RTCCR_NA0_OFFSET                               (0x10C) /* RTC_NA数据0寄存器。 */
#define    HI6559_RTCCR_NA1_OFFSET                               (0x10D) /* RTC_NA数据1寄存器。 */
#define    HI6559_RTCCR_NA2_OFFSET                               (0x10E) /* RTC_NA数据2寄存器。 */
#define    HI6559_RTCCR_NA3_OFFSET                               (0x10F) /* RTC_NA数据3寄存器。 */
#define    HI6559_RTCLR_NA0_OFFSET                               (0x110) /* RTCLR_NA加载0寄存器。 */
#define    HI6559_RTCLR_NA1_OFFSET                               (0x111) /* RTCLR_NA加载1寄存器。 */
#define    HI6559_RTCLR_NA2_OFFSET                               (0x112) /* RTCLR_NA加载2寄存器。 */
#define    HI6559_RTCLR_NA3_OFFSET                               (0x113) /* RTCLR_NA加载3寄存器。 */
#define    HI6559_RTCCTRL_NA_OFFSET                              (0x114) /* RTCCTRL_NA控制寄存器。 */
#define    HI6559_VERSION_OFFSET                                 (0x115) /* 版本寄存器。 */
#define    HI6559_RESERVED0_OFFSET                               (0x116) /* 保留0寄存器。 */
#define    HI6559_RESERVED1_OFFSET                               (0x117) /* 保留1寄存器。 */
#define    HI6559_RESERVED2_OFFSET                               (0x118) /* 保留2寄存器。 */
#define    HI6559_RESERVED3_OFFSET                               (0x119) /* 保留3寄存器。 */
#define    HI6559_RESERVED4_OFFSET                               (0x11A) /* 保留4寄存器。 */
#define    HI6559_HTOL_MODE_OFFSET                               (0x11B) /* 老化测试模式配置寄存器。 */
#define    HI6559_DAC_CTRL_OFFSET                                (0x11C) /* DAC控制寄存器 */
#define    HI6559_NP_REG_ADJ_OFFSET                              (0x11E) /* 非下电调节寄存器。 */
#define    HI6559_NP_REG_CHG_OFFSET                              (0x11F) /* 备用电池非下电调节寄存器。 */
#define    HI6559_NP_RSVED1_OFFSET                               (0x120) /* 非下电保留1寄存器。 */
#define    HI6559_NP_RSVED2_OFFSET                               (0x121) /* 非下电保留2寄存器。 */
#define    HI6559_NP_RSVED3_OFFSET                               (0x122) /* 非下电保留3寄存器。 */
#define    HI6559_NP_RSVED4_OFFSET                               (0x123) /* 非下电保留4寄存器。 */
#define    HI6559_RTC_ADJ1_OFFSET                                (0x124) /* rtc_adj[7:0]寄存器。 */
#define    HI6559_RTC_ADJ2_OFFSET                                (0x125) /* rtc_adj[15:8]寄存器。 */

/********************************************************************************/
/*    bbstar 寄存器定义（项目名_模块名_寄存器名_T)        */
/********************************************************************************/
typedef union
{
    struct
    {
        unsigned int    otmp_d1r                   : 1; /* [0..0] 0： 当前温度低于设定阈值（默认125℃）；1： 当前温度高于设定阈值。 */
        unsigned int    otmp150_d1r                : 1; /* [1..1] 0： 当前温度低于150℃；1： 当前温度高于150℃。 */
        unsigned int    osc_state                  : 1; /* [2..2] 0：OSC晶体时钟已经就绪，时钟切换完成;1：OSC晶体时钟没有就绪。 */
        unsigned int    vsys_uv_d2f                : 1; /* [3..3] 0：vsys<2.85V/3.0V；1：vsys>2.85V/3.0V； */
        unsigned int    vsys_6p0_d200ur            : 1; /* [4..4] 当前vsys电压：0：vsys<6.0V；1：vsys>6.0V； */
        unsigned int    pwron_d20r                 : 1; /* [5..5] 0：PWRON键当前没有被按下；1:PWRON键当前被按下(pwron为pwron_n的反向)。 */
        unsigned int    reserved                   : 2; /* [7..6] 保留。 */
    } bits;
    unsigned int    u32;
}HI6559_STATUS1_T;    /* 状态标志1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    alarm_on_a                 : 1; /* [0..0] 闹钟A:0:闹钟时间没到；1：闹钟时间到。 */
        unsigned int    alarm_on_b                 : 1; /* [1..1] 闹钟B:0:闹钟时间没到；1：闹钟时间到。 */
        unsigned int    alarm_on_c                 : 1; /* [2..2] 闹钟C:0:闹钟时间没到；1：闹钟时间到。 */
        unsigned int    alarm_on_d                 : 1; /* [3..3] 闹钟D:0:闹钟时间没到；1：闹钟时间到。 */
        unsigned int    alarm_on_e                 : 1; /* [4..4] 闹钟E:0:闹钟时间没到；1：闹钟时间到。 */
        unsigned int    vbus_det_0p9_d3r           : 1; /* [5..5] vbus_det_0p9：0: 没有插入；1：插入。 */
        unsigned int    reserved                   : 2; /* [7..6] 保留。 */
    } bits;
    unsigned int    u32;
}HI6559_STATUS2_T;    /* 状态标志2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    otmp_d1r                   : 1; /* [0..0] 0:无此中断；1：otmp 报温度超过设定阈值（默认125度）中断。 */
        unsigned int    vsys_2p5_f                 : 1; /* [1..1] 0:无此中断；1:vsys电压低于2.5V中断。 */
        unsigned int    vsys_uv_d2f                : 1; /* [2..2] 0:无此中断；1:vsys电压低于2.85V/3.0V 2ms中断。(默认2.85V，非下电寄存器vsys_uv_adj可调) */
        unsigned int    vsys_6p0_d200ur            : 1; /* [3..3] 0：无此中断；1:vsys报电压高于6.0V 200us中断。 */
        unsigned int    pwron_d4sr                 : 1; /* [4..4] 0:无此中断；1:PWRON按键按下4s中断(该中断只在正常工作时按下才上报) 。 */
        unsigned int    pwron_d20f                 : 1; /* [5..5] 0:无此中断；1:PWRON按键释放20ms中断 。 */
        unsigned int    pwron_d20r                 : 1; /* [6..6] 0:无此中断；1:PWRON按键按下20ms中断。 */
        unsigned int    reserved                   : 1; /* [7..7] 保留。 */
    } bits;
    unsigned int    u32;
}HI6559_IRQ1_T;    /* 中断1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    ocp_scp_r                  : 1; /* [0..0] 0:无此中断；1:表示至少有一个BUCK发生过流保护或者短路保护，或者LDO/LVS发生过流保护，CPU查询到后要去查看下面的过流保存寄存器查看是哪一个发生过流，并将其写1清后，中断消除。 */
        unsigned int    vbus_det_1p375_d90ur       : 1; /* [1..1] 0:无此中断；1:vbus_det>1.375V 90us中断 。 */
        unsigned int    vbus_det_0p9_d3f           : 1; /* [2..2] 0:无此中断；1:vbus_det<0.9 V 3ms中断 。 */
        unsigned int    vbus_det_0p9_d3r           : 1; /* [3..3] 0:无此中断；1:vbus_det>0.9 V 3ms中断 。 */
        unsigned int    reserved                   : 4; /* [7..4] 保留。 */
    } bits;
    unsigned int    u32;
}HI6559_IRQ2_T;    /* 中断2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    alarm_on_a                 : 1; /* [0..0] ALARM_ON_A中断：0:无此中断；1：当RTC时间与alarm时间相同时,上报中断。 */
        unsigned int    alarm_on_b                 : 1; /* [1..1] ALARM_ON_B中断：0:无此中断；1：当RTC时间与alarm时间相同时,上报中断。 */
        unsigned int    alarm_on_c                 : 1; /* [2..2] ALARM_ON_C中断：0:无此中断；1：当RTC时间与alarm时间相同时,上报中断。 */
        unsigned int    alarm_on_d                 : 1; /* [3..3] ALARM_ON_D中断：0:无此中断；1：当RTC时间与alarm时间相同时,上报中断。 */
        unsigned int    alarm_on_e                 : 1; /* [4..4] ALARM_ON_E中断：0:无此中断；1：当RTC时间与alarm时间相同时,上报中断。 */
        unsigned int    alarm_on_f                 : 1; /* [5..5] ALARM_ON_F中断：0:无此中断；1：当RTC时间与alarm时间相同时,上报中断。 */
        unsigned int    reserved                   : 2; /* [7..6] 保留。 */
    } bits;
    unsigned int    u32;
}HI6559_IRQ3_T;    /* 中断3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    sim_hpd_out_f              : 1; /* [0..0] SIM卡不在位发生SIM_HPD中断：0： 无此中断 ；  1： 发生下降沿跳变  。 */
        unsigned int    sim_hpd_out_r              : 1; /* [1..1] SIM卡不在位发生SIM_HPD中断：0： 无此中断 ；  1： 发生上升沿跳变  。 */
        unsigned int    sim_hpd_in_f               : 1; /* [2..2] SIM卡在位发生SIM_HPD中断：0： 无此中断 ；  1： 发生下降沿跳变  。 */
        unsigned int    sim_hpd_in_r               : 1; /* [3..3] SIM卡在位发生SIM_HPD中断：0： 无此中断 ；  1： 发生上升沿跳变  。 */
        unsigned int    reserved                   : 4; /* [7..4] 保留。 */
    } bits;
    unsigned int    u32;
}HI6559_IRQ4_T;    /* 中断4寄存器。 */

typedef union
{
    struct
    {
        unsigned int    otmp_d1r_mk                : 1; /* [0..0] 0:使能该中断；1：屏蔽该中断。 */
        unsigned int    vsys_2p5_f_mk              : 1; /* [1..1] 0:使能该中断；1：屏蔽该中断。 */
        unsigned int    vsys_uv_d2f_mk             : 1; /* [2..2] 0:使能该中断；1：屏蔽该中断。 */
        unsigned int    vsys_6p0_d200ur_mk         : 1; /* [3..3] 0:使能该中断；1：屏蔽该中断。 */
        unsigned int    pwron_d4sr_mk              : 1; /* [4..4] 0:使能该中断；1：屏蔽该中断。 */
        unsigned int    pwron_d20f_mk              : 1; /* [5..5] 0:使能该中断；1：屏蔽该中断。 */
        unsigned int    pwron_d20r_mk              : 1; /* [6..6] 0:使能该中断；1：屏蔽该中断。 */
        unsigned int    reserved                   : 1; /* [7..7] 保留。 */
    } bits;
    unsigned int    u32;
}HI6559_IRQ1_MASK_T;    /* 中断屏蔽1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    ocp_scp_r_mk               : 1; /* [0..0] 0:使能该中断；1：屏蔽该中断。 */
        unsigned int    vbus_det_1p375_mk          : 1; /* [1..1] 0:使能该中断；1：屏蔽该中断。 */
        unsigned int    vbus_det_0p9_d3f_mk        : 1; /* [2..2] 0:使能该中断；1：屏蔽该中断。 */
        unsigned int    vbus_det_0p9_d3r_mk        : 1; /* [3..3] 0:使能该中断；1：屏蔽该中断。 */
        unsigned int    reserved                   : 4; /* [7..4] 保留。 */
    } bits;
    unsigned int    u32;
}HI6559_IRQ2_MASK_T;    /* 中断屏蔽2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    alarm_on_a_mk              : 1; /* [0..0] ALARM_ON_A中断：0:使能该中断；1：屏蔽该中断。 */
        unsigned int    alarm_on_b_mk              : 1; /* [1..1] ALARM_ON_B中断：0:使能该中断；1：屏蔽该中断。 */
        unsigned int    alarm_on_c_mk              : 1; /* [2..2] ALARM_ON_C中断：0:使能该中断；1：屏蔽该中断。 */
        unsigned int    alarm_on_d_mk              : 1; /* [3..3] ALARM_ON_D中断：0:使能该中断；1：屏蔽该中断。 */
        unsigned int    alarm_on_e_mk              : 1; /* [4..4] ALARM_ON_E中断：0:使能该中断；1：屏蔽该中断。 */
        unsigned int    alarm_on_f_mk              : 1; /* [5..5] ALARM_ON_F中断：0:使能该中断；1：屏蔽该中断。 */
        unsigned int    reserved                   : 2; /* [7..6] 保留。 */
    } bits;
    unsigned int    u32;
}HI6559_IRQ3_MASK_T;    /* 中断屏蔽3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    sim_hpd_out_f_mk           : 1; /* [0..0] SIM卡不在位发生SIM_HPD中断：0:使能该中断；1：屏蔽该中断。 */
        unsigned int    sim_hpd_out_r_mk           : 1; /* [1..1] SIM卡不在位发生SIM_HPD中断：0:使能该中断；1：屏蔽该中断。 */
        unsigned int    sim_hpd_in_f_mk            : 1; /* [2..2] SIM卡在位发生SIM_HPD中断：0:使能该中断；1：屏蔽该中断。 */
        unsigned int    sim_hpd_in_r_mk            : 1; /* [3..3] SIM卡在位发生SIM_HPD中断：0:使能该中断；1：屏蔽该中断。 */
        unsigned int    reserved                   : 4; /* [7..4] 保留。 */
    } bits;
    unsigned int    u32;
}HI6559_IRQ4_MASK_T;    /* 中断屏蔽4寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved_2                 : 2; /* [1..0] 保留。 */
        unsigned int    scp_buck3                  : 1; /* [2..2] 0：BUCK3输出不短路；1: BUCK3输出短路。 */
        unsigned int    scp_buck4                  : 1; /* [3..3] 0：BUCK4输出不短路；1: BUCK4输出短路。 */
        unsigned int    reserved_1                 : 1; /* [4..4] 保留。 */
        unsigned int    scp_buck6                  : 1; /* [5..5] 0：BUCK6输出不短路；1: BUCK6输出短路。 */
        unsigned int    reserved_0                 : 2; /* [7..6] 保留。 */
    } bits;
    unsigned int    u32;
}HI6559_SCP_RECORD1_T;    /* 短路保护记录1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 1; /* [0..0] 保留。 */
        unsigned int    ocp_buck0                  : 1; /* [1..1] 0：BUCK0输出不过载；1: BUCK0输出过载。 */
        unsigned int    ocp_buck3                  : 1; /* [2..2] 0：BUCK3输出不过载；1: BUCK3输出过载。 */
        unsigned int    ocp_buck4                  : 1; /* [3..3] 0：BUCK4输出不过载；1: BUCK4输出过载。 */
        unsigned int    ocp_buck5                  : 1; /* [4..4] 0：BUCK5输出不过载；1: BUCK5输出过载。 */
        unsigned int    ocp_buck6                  : 1; /* [5..5] 0：BUCK6输出不过载；1: BUCK6输出过载。 */
        unsigned int    reserved_0                 : 2; /* [7..6] 保留。 */
    } bits;
    unsigned int    u32;
}HI6559_OCP_RECORD1_T;    /* 过流保护记录1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    ocp_ldo1                   : 1; /* [0..0] 0：LDO1输出不过载；1: LDO1输出过载。 */
        unsigned int    reserved_1                 : 1; /* [1..1] 保留。 */
        unsigned int    ocp_ldo3                   : 1; /* [2..2] 0：LDO3输出不过载；1: LDO3输出过载。 */
        unsigned int    reserved_0                 : 2; /* [4..3] 保留。 */
        unsigned int    ocp_ldo6                   : 1; /* [5..5] 0：LDO6输出不过载；1: LDO6输出过载。 */
        unsigned int    ocp_ldo7                   : 1; /* [6..6] 0：LDO7输出不过载；1: LDO7输出过载。 */
        unsigned int    ocp_ldo8                   : 1; /* [7..7] 0：LDO8输出不过载；1: LDO8输出过载。 */
    } bits;
    unsigned int    u32;
}HI6559_OCP_RECORD2_T;    /* 过流保护记录2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    ocp_ldo9                   : 1; /* [0..0] 0：LDO9输出不过载；1: LDO9输出过载。 */
        unsigned int    ocp_ldo10                  : 1; /* [1..1] 0：LDO10输出不过载；1: LDO10输出过载。 */
        unsigned int    ocp_ldo11                  : 1; /* [2..2] 0：LDO11输出不过载；1: LDO11输出过载。 */
        unsigned int    ocp_ldo12                  : 1; /* [3..3] 0：LDO12输出不过载；1: LDO12输出过载。 */
        unsigned int    ocp_ldo13                  : 1; /* [4..4] 0：LDO13输出不过载；1: LDO13输出过载。 */
        unsigned int    ocp_ldo14                  : 1; /* [5..5] 0：LDO14输出不过载；1: LDO14输出过载。 */
        unsigned int    reserved                   : 2; /* [7..6] 保留。 */
    } bits;
    unsigned int    u32;
}HI6559_OCP_RECORD3_T;    /* 过流保护记录3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 5; /* [4..0] 保留。 */
        unsigned int    ocp_ldo22                  : 1; /* [5..5] 0：LDO22输出不过载；1: LDO22输出过载。 */
        unsigned int    ocp_ldo23                  : 1; /* [6..6] 0：LDO23输出不过载；1: LDO23输出过载。 */
        unsigned int    ocp_ldo24                  : 1; /* [7..7] 0：LDO24输出不过载；1: LDO24输出过载。 */
    } bits;
    unsigned int    u32;
}HI6559_OCP_RECORD4_T;    /* 过流保护记录4寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 5; /* [4..0] 保留。 */
        unsigned int    ocp_lvs7                   : 1; /* [5..5] 0：LVS7输出不过载；1: LVS7输出过载。 */
        unsigned int    reserved_0                 : 2; /* [7..6] 保留。 */
    } bits;
    unsigned int    u32;
}HI6559_OCP_RECORD5_T;    /* 过流保护记录5寄存器。 */

typedef union
{
    struct
    {
        unsigned int    ocp_lvs9                   : 1; /* [0..0] 0：LVS9输出不过载；1: LVS9输出过载。 */
        unsigned int    reserved                   : 7; /* [7..1] 保留。 */
    } bits;
    unsigned int    u32;
}HI6559_OCP_RECORD6_T;    /* 过流保护记录6寄存器。 */

typedef union
{
    struct
    {
        unsigned int    np_otmp_d1r                : 1; /* [0..0] 温度报警(默认125°)：0:没有发生过该事件；1:发生该事件。 */
        unsigned int    np_hreset_n_f              : 1; /* [1..1] 热复位：0:没有发生过该事件；1:发生该事件。 */
        unsigned int    np_vsys_2p5_f              : 1; /* [2..2] vsys < 2.5V：0:没有发生过该事件；1:发生该事件。 */
        unsigned int    np_vsys_uv_d2f             : 1; /* [3..3] vsys < 2.85V/3.0V 2ms:0:没有发生过该事件；1:发生该事件。 */
        unsigned int    np_vsys_6p0_d200ur         : 1; /* [4..4] vsys> 6.0V 200us:0:没有发生过该事件；1:发生该事件。 */
        unsigned int    np_pwron_d4sr              : 1; /* [5..5] 在正常工作状态下PWRON按下4s:0:没有发生过该事件；1:发生该事件。 */
        unsigned int    np_pwron_d20f              : 1; /* [6..6] PWRON按键释放20ms：0:没有发生过该事件；1:发生该事件。 */
        unsigned int    np_pwron_d20r              : 1; /* [7..7] PWRON按键按下20ms：0:没有发生过该事件；1:发生该事件。 */
    } bits;
    unsigned int    u32;
}HI6559_NP_IRQ1_RECORD_T;    /* 中断1非下电记录寄存器。 */

typedef union
{
    struct
    {
        unsigned int    np_vbus_det_0p9_d150r      : 1; /* [0..0] vbus_det> 0.9V 150ms：     0:没有发生过该事件；1:发生该事件。 */
        unsigned int    np_vbus_det_1p375          : 1; /* [1..1] vbus_det>1.375V 90us：     0:没有发生过该事件；1:发生该事件。 */
        unsigned int    np_vbus_det_0p9_d3f        : 1; /* [2..2] vbus_det< 0.9V 3ms： 0:没有发生过该事件；1:发生该事件。 */
        unsigned int    np_vbus_det_0p9_d3r        : 1; /* [3..3] vbus_det> 0.9V 3ms： 0:没有发生过该事件；1:发生该事件。 */
        unsigned int    np_pwron_restart           : 1; /* [4..4] 通过pwron按下N+5S开机:0:没有发生过该事件；1:发生该事件。 */
        unsigned int    np_pwron_dnsr              : 1; /* [5..5] 在正常工作下pwron按下Ns关机:0:没有发生过该事件；1:发生该事件。 */
        unsigned int    np_pwron_d500r             : 1; /* [6..6] 关机状态下PWRON按下500ms:0:没有发生过该事件；1:发生该事件。 */
        unsigned int    np_otmp150_d1r             : 1; /* [7..7] 温度 > 150℃：0:没有发生过该事件；1:发生该事件。 */
    } bits;
    unsigned int    u32;
}HI6559_NP_IRQ2_RECORD_T;    /* 中断2非下电记录寄存器。 */

typedef union
{
    struct
    {
        unsigned int    np_alarm_on_a              : 1; /* [0..0] 触发闹钟a：0:没有发生过该事件；1:发生该事件。 */
        unsigned int    np_alarm_on_b              : 1; /* [1..1] 触发闹钟b：0:没有发生过该事件；1:发生该事件。 */
        unsigned int    np_alarm_on_c              : 1; /* [2..2] 触发闹钟c：0:没有发生过该事件；1:发生该事件。 */
        unsigned int    np_alarm_on_d              : 1; /* [3..3] 触发闹钟d：0:没有发生过该事件；1:发生该事件。 */
        unsigned int    np_alarm_on_e              : 1; /* [4..4] 触发闹钟e：0:没有发生过该事件；1:发生该事件。 */
        unsigned int    np_alarm_on_f              : 1; /* [5..5] 触发闹钟f：0:没有发生过该事件；1:发生该事件。 */
        unsigned int    np_hold_pwron_r            : 1; /* [6..6] pwr_hold上电记录：0：没有发生该事件；1：发生该事件； */
        unsigned int    np_hold_pwrdw_r            : 1; /* [7..7] pwr_hold下电记录：0：没有发生该事件；1：发生该事件； */
    } bits;
    unsigned int    u32;
}HI6559_NP_IRQ3_RECORD_T;    /* 中断3非下电记录寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved_2                 : 2; /* [1..0] 保留 */
        unsigned int    np_scp_buck3               : 1; /* [2..2] 0：仅记录BUCK3输出不短路；1: 仅记录BUCK3输出短路。 */
        unsigned int    np_scp_buck4               : 1; /* [3..3] 0：仅记录BUCK4输出不短路；1: 仅记录BUCK4输出短路。 */
        unsigned int    reserved_1                 : 1; /* [4..4] 保留 */
        unsigned int    np_scp_buck6               : 1; /* [5..5] 0：仅记录BUCK6输出不短路；1: 仅记录BUCK6输出短路。 */
        unsigned int    reserved_0                 : 2; /* [7..6] 保留。 */
    } bits;
    unsigned int    u32;
}HI6559_NP_SCP_RECORD1_T;    /* 短路保护非下电记录1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 3; /* [2..0] 保留 */
        unsigned int    np_ocp_buck0               : 1; /* [3..3] 0：仅记录BUCK0输出不过载；1: 仅记录BUCK0输出过载。 */
        unsigned int    np_ocp_buck3               : 1; /* [4..4] 0：仅记录BUCK3输出不过载；1: 仅记录BUCK3输出过载。 */
        unsigned int    np_ocp_buck4               : 1; /* [5..5] 0：仅记录BUCK4输出不过载；1: 仅记录BUCK4输出过载。 */
        unsigned int    np_ocp_buck5               : 1; /* [6..6] 0：仅记录BUCK5输出不过载；1: 仅记录BUCK5输出过载。 */
        unsigned int    np_ocp_buck6               : 1; /* [7..7] 0：仅记录BUCK6输出不过载；1: 仅记录BUCK6输出过载。 */
    } bits;
    unsigned int    u32;
}HI6559_NP_OCP_RECORD1_T;    /* 过流保护非下电记录1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    np_ocp_ldo1                : 1; /* [0..0] 0：仅记录LDO1输出不过载；1: 仅记录LDO1输出过载。 */
        unsigned int    reserved_1                 : 1; /* [1..1] 保留 */
        unsigned int    np_ocp_ldo3                : 1; /* [2..2] 0：仅记录LDO3输出不过载；1: 仅记录LDO3输出过载 。 */
        unsigned int    reserved_0                 : 2; /* [4..3] 保留 */
        unsigned int    np_ocp_ldo6                : 1; /* [5..5] 0：仅记录LDO6输出不过载；1: 仅记录LDO6输出过载。 */
        unsigned int    np_ocp_ldo7                : 1; /* [6..6] 0：仅记录LDO7输出不过载；1: 仅记录LDO7输出过载。 */
        unsigned int    np_ocp_ldo8                : 1; /* [7..7] 0：仅记录LDO8输出不过载；1: 仅记录LDO8输出过载。 */
    } bits;
    unsigned int    u32;
}HI6559_NP_OCP_RECORD2_T;    /* 过流保护非下电记录2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    np_ocp_ldo9                : 1; /* [0..0] 0：仅记录LDO9输出不过载；1: 仅记录LDO9输出过载。 */
        unsigned int    np_ocp_ldo10               : 1; /* [1..1] 0：仅记录LDO10输出不过载；1: 仅记录LDO10输出过载。 */
        unsigned int    np_ocp_ldo11               : 1; /* [2..2] 0：仅记录LDO11输出不过载；1: 仅记录LDO11输出过载。 */
        unsigned int    np_ocp_ldo12               : 1; /* [3..3] 0：仅记录LDO12输出不过载；1: 仅记录LDO12输出过载。 */
        unsigned int    np_ocp_ldo13               : 1; /* [4..4] 0：仅记录LDO13输出不过载；1: 仅记录LDO13输出过载。 */
        unsigned int    np_ocp_ldo14               : 1; /* [5..5] 0：仅记录LDO14输出不过载；1: 仅记录LDO14输出过载。 */
        unsigned int    reserved                   : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_NP_OCP_RECORD3_T;    /* 过流保护非下电记录3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 5; /* [4..0] 保留 */
        unsigned int    np_ocp_ldo22               : 1; /* [5..5] 0：仅记录LDO22输出不过载；1: 仅记录LDO22输出过载。 */
        unsigned int    np_ocp_ldo23               : 1; /* [6..6] 0：仅记录LDO23输出不过载；1: 仅记录LDO23输出过载。 */
        unsigned int    np_ocp_ldo24               : 1; /* [7..7] 0：仅记录LDO24输出不过载；1: 仅记录LDO24输出过载。 */
    } bits;
    unsigned int    u32;
}HI6559_NP_OCP_RECORD4_T;    /* 过流保护非下电记录4寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 5; /* [4..0] 保留 */
        unsigned int    np_ocp_lvs7                : 1; /* [5..5] 0：仅记录LVS7输出不过载；1: 仅记录LVS7输出过载。 */
        unsigned int    reserved_0                 : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_NP_OCP_RECORD5_T;    /* 过流保护非下电记录5寄存器。 */

typedef union
{
    struct
    {
        unsigned int    np_ocp_lvs9                : 1; /* [0..0] 0：仅记录LVS9输出不过载；1: 仅记录LVS9输出过载。 */
        unsigned int    reserved                   : 7; /* [7..1] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_NP_OCP_RECORD6_T;    /* 过流保护非下电记录6寄存器。 */

typedef union
{
    struct
    {
        unsigned int    ocp_ldo_deb_sel            : 2; /* [1..0] ocp去抖时间：00：500us；01：1ms；10：2ms；11：4ms。 */
        unsigned int    en_ldo_ocp_deb             : 1; /* [2..2] 0：LDO OCP上升沿不去抖；1：LDO OCP上升沿去抖；注：LDO与LVS共用。 */
        unsigned int    ocp_buck_deb_sel           : 2; /* [4..3] BUCK 的ocp和scp去抖时间：00：500us；01：1ms；10：2ms；11：4ms。 */
        unsigned int    en_buck_ocp_deb            : 1; /* [5..5] 0：BUCK OCP上升沿不去抖；1：BUCK OCP上升沿去抖。 */
        unsigned int    en_buck_scp_deb            : 1; /* [6..6] 0：BUCK SCP上升沿不去抖；1：BUCK SCP上升沿去抖； */
        unsigned int    reserved                   : 1; /* [7..7] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_OCP_DEB_T;    /* OCP去抖动调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    vbus_det_0p9_ctrl          : 1; /* [0..0] 0：屏蔽vbus_det<0.9V检测；1:不屏蔽vbus_det<0.9V检测。 */
        unsigned int    vsys_2p5_ctrl              : 1; /* [1..1] 0：屏蔽vsys< 2.5V检测；1:不屏蔽vsys<2.5V检测。 */
        unsigned int    vsys_uv_ctrl               : 1; /* [2..2] 0：屏蔽vsys< 2.85V/3.0V检测；1:不屏蔽vsys<2.85V/3.0V检测。 */
        unsigned int    vsys_6p0_ctrl              : 1; /* [3..3] 0：屏蔽vsys>6.0V检测；1:不屏蔽vsys>6.0检测。 */
        unsigned int    otmp150_ctrl               : 1; /* [4..4] 0：高温（150℃）不关机；1：高温(150℃)关机。 */
        unsigned int    reserved                   : 1; /* [5..5] 保留 */
        unsigned int    en_32kb                    : 1; /* [6..6] 0： 关闭 CLK_32B 时钟输出；1：  CLK_32B 时钟输出使能。 */
        unsigned int    en_32ka                    : 1; /* [7..7] 0： 关闭 CLK_32A 时钟输出；1：  CLK_32A 时钟输出使能。 */
    } bits;
    unsigned int    u32;
}HI6559_ONOFF8_T;    /* 开关控制8寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 2; /* [1..0] 保留 */
        unsigned int    ocp_scp_ctrl_buck0         : 1; /* [2..2] ocp_scp_off_buck0配置为1时：ocp_scp_ctrl_buck0:0:工作在过流模式2；1:工作在过流模式3； */
        unsigned int    ocp_scp_off_buck0          : 1; /* [3..3] 0：过流或短路后不支持BUCK0模块自动关闭；1：过流或短路后支持BUCK0模块自动关闭 。 */
        unsigned int    ocp_scp_ctrl_buck3         : 1; /* [4..4] ocp_scp_off_buck3配置为1时：ocp_scp_ctrl_buck3:0:工作在过流模式2；1:工作在过流模式3； */
        unsigned int    ocp_scp_off_buck3          : 1; /* [5..5] 0：过流或短路后不支持BUCK3模块自动关闭；1：过流或短路后支持BUCK3模块自动关闭 。 */
        unsigned int    ocp_scp_ctrl_buck4         : 1; /* [6..6] ocp_scp_off_buck4配置为1时：ocp_scp_ctrl_buck4:0:工作在过流模式2；1:工作在过流模式3； */
        unsigned int    ocp_scp_off_buck4          : 1; /* [7..7] 0：过流或短路后不支持BUCK4模块自动关闭；1：过流或短路后支持BUCK4模块自动关闭 。 */
    } bits;
    unsigned int    u32;
}HI6559_OCP_SCP_MOD_CTRL1_T;    /* BUCK0-3过流和短路保护模式控制寄存器。 */

typedef union
{
    struct
    {
        unsigned int    ocp_scp_ctrl_buck5         : 1; /* [0..0] ocp_scp_off_buck5配置为1时：ocp_scp_ctrl_buck5:0:工作在过流模式2；1:工作在过流模式3； */
        unsigned int    ocp_scp_off_buck5          : 1; /* [1..1] 0：过流或短路后不支持BUCK5模块自动关闭；1：过流或短路后支持BUCK5模块自动关闭 。 */
        unsigned int    ocp_scp_ctrl_buck6         : 1; /* [2..2] ocp_scp_off_buck6配置为1时：ocp_scp_ctrl_buck6:0:工作在过流模式2；1:工作在过流模式3； */
        unsigned int    ocp_scp_off_buck6          : 1; /* [3..3] 0：过流或短路后不支持BUCK6模块自动关闭；1：过流或短路后支持BUCK6模块自动关闭 。 */
        unsigned int    reserved                   : 4; /* [7..4] 保留。 */
    } bits;
    unsigned int    u32;
}HI6559_OCP_SCP_MOD_CTRL2_T;    /* BUCK4-7过流和短路保护模式控制寄存器。 */

typedef union
{
    struct
    {
        unsigned int    ocp_ctrl_ldo1              : 1; /* [0..0] ocp_off_ldo1配置为1时：ocp_ctrl_ldo1:0:工作在过流模式2；1:工作在过流模式3； */
        unsigned int    ocp_off_ldo1               : 1; /* [1..1] 0：过流后不支持LDO1模块自动关闭；1：过流后支持LDO1模块自动关闭 。 */
        unsigned int    reserved_1                 : 2; /* [3..2] 保留 */
        unsigned int    ocp_ctrl_ldo3              : 1; /* [4..4] ocp_off_ldo3配置为1时：ocp_ctrl_ldo3:0：过流模式为模式2；1：过流模式为模式3。 */
        unsigned int    ocp_off_ldo3               : 1; /* [5..5] 0：过流后不支持LDO3模块自动关闭；1：过流后支持LDO3模块自动关闭 。 */
        unsigned int    reserved_0                 : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_OCP_MOD_CTRL3_T;    /* LDO1-4过流保护模式控制寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 2; /* [1..0] 保留 */
        unsigned int    ocp_ctrl_ldo6              : 1; /* [2..2] ocp_off_ldo6配置为1时：ocp_ctrl_ldo6:0:工作在过流模式2；1:工作在过流模式3； */
        unsigned int    ocp_off_ldo6               : 1; /* [3..3] 0：过流后不支持LDO6模块自动关闭；1：过流后支持LDO6模块自动关闭 。 */
        unsigned int    ocp_ctrl_ldo7              : 1; /* [4..4] ocp_off_ldo7配置为1时：ocp_ctrl_ldo7:0:工作在过流模式2；1:工作在过流模式3； */
        unsigned int    ocp_off_ldo7               : 1; /* [5..5] 0：过流后不支持LDO7模块自动关闭；1：过流后支持LDO7模块自动关闭 。 */
        unsigned int    ocp_ctrl_ldo8              : 1; /* [6..6] ocp_off_ldo8配置为1时：ocp_ctrl_ldo8:0:工作在过流模式2；1:工作在过流模式3； */
        unsigned int    ocp_off_ldo8               : 1; /* [7..7] 0：过流后不支持LDO8模块自动关闭；1：过流后支持LDO8模块自动关闭 。 */
    } bits;
    unsigned int    u32;
}HI6559_OCP_MOD_CTRL4_T;    /* LDO5-8过流保护模式控制寄存器。 */

typedef union
{
    struct
    {
        unsigned int    ocp_ctrl_ldo9              : 1; /* [0..0] ocp_off_ldo9配置为1时：ocp_ctrl_ldo9:0:工作在过流模式2；1:工作在过流模式3； */
        unsigned int    ocp_off_ldo9               : 1; /* [1..1] 0：过流后不支持LDO9模块自动关闭；1：过流后支持LDO9模块自动关闭 。 */
        unsigned int    ocp_ctrl_ldo10             : 1; /* [2..2] ocp_off_ldo10配置为1时：ocp_ctrl_ldo10:0:工作在过流模式2；1:工作在过流模式3； */
        unsigned int    ocp_off_ldo10              : 1; /* [3..3] 0：过流后不支持LDO10模块自动关闭；1：过流后支持LDO10模块自动关闭 。 */
        unsigned int    ocp_ctrl_ldo11             : 1; /* [4..4] ocp_off_ldo11配置为1时：ocp_ctrl_ldo11:0:工作在过流模式2；1:工作在过流模式3； */
        unsigned int    ocp_off_ldo11              : 1; /* [5..5] 0：过流后不支持LDO11模块自动关闭；1：过流后支持LDO11模块自动关闭 。 */
        unsigned int    ocp_ctrl_ldo12             : 1; /* [6..6] ocp_off_ldo12配置为1时：ocp_ctrl_ldo12:0:工作在过流模式2；1:工作在过流模式3； */
        unsigned int    ocp_off_ldo12              : 1; /* [7..7] 0：过流后不支持LDO12模块自动关闭；1：过流后支持LDO12模块自动关闭 。 */
    } bits;
    unsigned int    u32;
}HI6559_OCP_MOD_CTRL5_T;    /* LDO9-12过流保护模式控制寄存器。 */

typedef union
{
    struct
    {
        unsigned int    ocp_ctrl_ldo13             : 1; /* [0..0] ocp_off_ldo13配置为1时：ocp_ctrl_ldo13:0:工作在过流模式2；1:工作在过流模式3； */
        unsigned int    ocp_off_ldo13              : 1; /* [1..1] 0：过流后不支持LDO13模块自动关闭；1：过流后支持LDO13模块自动关闭 。 */
        unsigned int    ocp_ctrl_ldo14             : 1; /* [2..2] ocp_off_ldo14配置为1时：ocp_ctrl_ldo14:0:工作在过流模式2；1:工作在过流模式3； */
        unsigned int    ocp_off_ldo14              : 1; /* [3..3] 0：过流后不支持LDO14模块自动关闭；1：过流后支持LDO14模块自动关闭 。 */
        unsigned int    reserved                   : 4; /* [7..4] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_OCP_MOD_CTRL6_T;    /* LDO13-16过流保护模式控制寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 2; /* [1..0] 保留 */
        unsigned int    ocp_ctrl_ldo22             : 1; /* [2..2] ocp_off_ldo22配置为1时：ocp_ctrl_ldo22:0:工作在过流模式2；1:工作在过流模式3； */
        unsigned int    ocp_off_ldo22              : 1; /* [3..3] 0：过流后不支持LDO22模块自动关闭；1：过流后支持LDO22模块自动关闭 。 */
        unsigned int    ocp_ctrl_ldo23             : 1; /* [4..4] ocp_off_ldo23配置为1时：ocp_ctrl_ldo23:0:工作在过流模式2；1:工作在过流模式3； */
        unsigned int    ocp_off_ldo23              : 1; /* [5..5] 0：过流后不支持LDO23模块自动关闭；1：过流后支持LDO23模块自动关闭 。 */
        unsigned int    ocp_ctrl_ldo24             : 1; /* [6..6] ocp_off_ldo24配置为1时：ocp_ctrl_ldo24:0:工作在过流模式2；1:工作在过流模式3； */
        unsigned int    ocp_off_ldo24              : 1; /* [7..7] 0：过流后不支持LDO24模块自动关闭；1：过流后支持LDO24模块自动关闭 。 */
    } bits;
    unsigned int    u32;
}HI6559_OCP_MOD_CTRL7_T;    /* LDO21-ldo23过流保护模式控制寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 2; /* [1..0] 保留 */
        unsigned int    ocp_ctrl_lvs7              : 1; /* [2..2] ocp_off_lvs7配置为1时：ocp_ctrl_lvs7:0：过流模式为模式2；1：过流模式为模式3。 */
        unsigned int    ocp_off_lvs7               : 1; /* [3..3] 0：过流后不支持lvs7模块自动关闭；1：过流后支持lvs7模块自动关闭 。 */
        unsigned int    reserved_0                 : 2; /* [5..4] 保留 */
        unsigned int    ocp_ctrl_lvs9              : 1; /* [6..6] ocp_off_lvs9配置为1时：ocp_ctrl_lvs9:0：过流模式为模式2；1：过流模式为模式3。 */
        unsigned int    ocp_off_lvs9               : 1; /* [7..7] 0：过流后不支持LVS9模块自动关闭；1：过流后支持LVS9模块自动关闭 。 */
    } bits;
    unsigned int    u32;
}HI6559_OCP_MOD_CTRL8_T;    /* LVS7-9过流保护模式控制寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 2; /* [1..0] 保留 */
        unsigned int    en_buck0_int               : 1; /* [2..2] BUCK0开关使能：写1开启，自动清零。 */
        unsigned int    en_buck3_int               : 1; /* [3..3] BUCK3开关使能：写1开启，自动清零。 */
        unsigned int    en_buck4_int               : 1; /* [4..4] BUCK4开关使能：写1开启，自动清零。 */
        unsigned int    en_buck5_int               : 1; /* [5..5] BUCK5开关使能：写1开启，自动清零。 */
        unsigned int    en_buck6_int               : 1; /* [6..6] BUCK6开关使能：写1开启，自动清零。 */
        unsigned int    reserved_0                 : 1; /* [7..7] 保留。 */
    } bits;
    unsigned int    u32;
}HI6559_ENABLE1_T;    /* 电源模块使能1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 2; /* [1..0] 保留 */
        unsigned int    dis_buck0_int              : 1; /* [2..2] BUCK0开关禁止：写1关闭，自动清零。 */
        unsigned int    dis_buck3_int              : 1; /* [3..3] BUCK3开关禁止：写1关闭，自动清零。 */
        unsigned int    dis_buck4_int              : 1; /* [4..4] BUCK4开关禁止：写1关闭，自动清零。 */
        unsigned int    dis_buck5_int              : 1; /* [5..5] BUCK5开关禁止：写1关闭，自动清零。 */
        unsigned int    dis_buck6_int              : 1; /* [6..6] BUCK6开关禁止：写1关闭，自动清零。 */
        unsigned int    reserved_0                 : 1; /* [7..7] 保留。 */
    } bits;
    unsigned int    u32;
}HI6559_DISABLE1_T;    /* 电源模块关闭1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 2; /* [1..0] 保留 */
        unsigned int    st_buck0_int               : 1; /* [2..2] BUCK0开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
        unsigned int    st_buck3_int               : 1; /* [3..3] BUCK3开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
        unsigned int    st_buck4_int               : 1; /* [4..4] BUCK4开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
        unsigned int    st_buck5_int               : 1; /* [5..5] BUCK5开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
        unsigned int    st_buck6_int               : 1; /* [6..6] BUCK6开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
        unsigned int    reserved_0                 : 1; /* [7..7] 保留。 */
    } bits;
    unsigned int    u32;
}HI6559_ONOFF_STATUS1_T;    /* 电源模块开关状态1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    en_ldo1_int                : 1; /* [0..0] LDO1开关使能：写1开启，自动清零。 */
        unsigned int    reserved_1                 : 1; /* [1..1] 保留 */
        unsigned int    en_ldo3_buck_int           : 1; /* [2..2] BUCK下LDO3使能：写1开启，自动清零。 */
        unsigned int    reserved_0                 : 2; /* [4..3] 保留 */
        unsigned int    en_ldo6_int                : 1; /* [5..5] LDO6开关使能：写1开启，自动清零。 */
        unsigned int    en_ldo7_int                : 1; /* [6..6] LDO7开关使能：写1开启，自动清零。 */
        unsigned int    en_ldo8_int                : 1; /* [7..7] LDO8开关使能：写1开启，自动清零。 */
    } bits;
    unsigned int    u32;
}HI6559_ENABLE2_T;    /* 电源模块使能2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    dis_ldo1_int               : 1; /* [0..0] LDO1开关禁止：写1关闭，自动清零。 */
        unsigned int    reserved_1                 : 1; /* [1..1] 保留 */
        unsigned int    dis_ldo3_buck_int          : 1; /* [2..2] BUCK下LDO3禁止：写1关闭，自动清零。 */
        unsigned int    reserved_0                 : 2; /* [4..3] 保留 */
        unsigned int    dis_ldo6_int               : 1; /* [5..5] LDO6开关禁止：写1关闭，自动清零。 */
        unsigned int    dis_ldo7_int               : 1; /* [6..6] LDO7开关禁止：写1关闭，自动清零。 */
        unsigned int    dis_ldo8_int               : 1; /* [7..7] LDO8开关禁止：写1关闭，自动清零。 */
    } bits;
    unsigned int    u32;
}HI6559_DISABLE2_T;    /* 电源模块关闭2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    st_ldo1_int                : 1; /* [0..0] LDO1开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
        unsigned int    reserved_1                 : 1; /* [1..1] 保留 */
        unsigned int    st_ldo3_buck_int           : 1; /* [2..2] BUCK下LDO3时能开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
        unsigned int    reserved_0                 : 2; /* [4..3] 保留 */
        unsigned int    st_ldo6_int                : 1; /* [5..5] LDO6开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
        unsigned int    st_ldo7_int                : 1; /* [6..6] LDO7开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
        unsigned int    st_ldo8_int                : 1; /* [7..7] LDO8开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
    } bits;
    unsigned int    u32;
}HI6559_ONOFF_STATUS2_T;    /* 电源模块开关状态2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    en_ldo10_int               : 1; /* [0..0] LDO10开关使能：写1开启，自动清零。 */
        unsigned int    en_ldo12_int               : 1; /* [1..1] LDO12开关使能：写1开启，自动清零。 */
        unsigned int    en_ldo13_int               : 1; /* [2..2] LDO13开关使能：写1开启，自动清零。 */
        unsigned int    en_ldo14_int               : 1; /* [3..3] LDO14开关使能：写1开启，自动清零。 */
        unsigned int    reserved                   : 4; /* [7..4] 保留。 */
    } bits;
    unsigned int    u32;
}HI6559_ENABLE3_T;    /* 电源模块使能3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    dis_ldo10_int              : 1; /* [0..0] LDO10开关禁止：写1关闭，自动清零。 */
        unsigned int    dis_ldo12_int              : 1; /* [1..1] LDO12开关禁止：写1关闭，自动清零。 */
        unsigned int    dis_ldo13_int              : 1; /* [2..2] LDO13开关禁止：写1关闭，自动清零。 */
        unsigned int    dis_ldo14_int              : 1; /* [3..3] LDO14开关禁止：写1关闭，自动清零。 */
        unsigned int    reserved                   : 4; /* [7..4] 保留。 */
    } bits;
    unsigned int    u32;
}HI6559_DISABLE3_T;    /* 电源模块关闭3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    st_ldo10_int               : 1; /* [0..0] LDO10开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
        unsigned int    st_ldo12_int               : 1; /* [1..1] LDO12开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
        unsigned int    st_ldo13_int               : 1; /* [2..2] LDO13开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
        unsigned int    st_ldo14_int               : 1; /* [3..3] LDO14开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
        unsigned int    reserved                   : 4; /* [7..4] 保留。 */
    } bits;
    unsigned int    u32;
}HI6559_ONOFF_STATUS3_T;    /* 电源模块开关状态3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 3; /* [2..0] 保留 */
        unsigned int    en_ldo22_int               : 1; /* [3..3] LDO22开关使能：写1开启，自动清零。 */
        unsigned int    en_ldo23_int               : 1; /* [4..4] LDO23开关使能：写1开启，自动清零。 */
        unsigned int    en_ldo24_int               : 1; /* [5..5] LDO24开关使能：写1开启，自动清零。 */
        unsigned int    en_ldo3_batt_int           : 1; /* [6..6] BATT下LDO3使能：写1开启，自动清零。 */
        unsigned int    reserved_0                 : 1; /* [7..7] 保留。 */
    } bits;
    unsigned int    u32;
}HI6559_ENABLE4_T;    /* 电源模块使能4寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 3; /* [2..0] 保留 */
        unsigned int    dis_ldo22_int              : 1; /* [3..3] LDO22开关禁止：写1关闭，自动清零。 */
        unsigned int    dis_ldo23_int              : 1; /* [4..4] LDO23开关禁止：写1关闭，自动清零。 */
        unsigned int    dis_ldo24_int              : 1; /* [5..5] LDO24开关禁止：写1关闭，自动清零。 */
        unsigned int    dis_ldo3_batt_int          : 1; /* [6..6] BATT下LDO3禁止：写1关闭，自动清零。 */
        unsigned int    reserved_0                 : 1; /* [7..7] 保留。 */
    } bits;
    unsigned int    u32;
}HI6559_DISABLE4_T;    /* 电源模块关闭4寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 3; /* [2..0] 保留 */
        unsigned int    st_ldo22_int               : 1; /* [3..3] LDO22开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
        unsigned int    st_ldo23_int               : 1; /* [4..4] LDO23开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
        unsigned int    st_ldo24_int               : 1; /* [5..5] LDO24开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
        unsigned int    st_ldo3_batt_int           : 1; /* [6..6] BATT下LDO3开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
        unsigned int    reserved_0                 : 1; /* [7..7] 保留。 */
    } bits;
    unsigned int    u32;
}HI6559_ONOFF_STATUS4_T;    /* 电源模块开关状态4寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 5; /* [4..0] 保留 */
        unsigned int    en_lvs7_int                : 1; /* [5..5] LVS7开关使能：写1开启，自动清零。 */
        unsigned int    reserved_0                 : 1; /* [6..6] 保留 */
        unsigned int    en_lvs9_int                : 1; /* [7..7] LVS9开关使能：写1开启，自动清零。 */
    } bits;
    unsigned int    u32;
}HI6559_ENABLE5_T;    /* 电源模块使能5寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 5; /* [4..0] 保留 */
        unsigned int    dis_lvs7_int               : 1; /* [5..5] LVS7开关禁止：写1关闭，自动清零。 */
        unsigned int    reserved_0                 : 1; /* [6..6] 保留 */
        unsigned int    dis_lvs9_int               : 1; /* [7..7] LVS9开关禁止：写1关闭，自动清零。 */
    } bits;
    unsigned int    u32;
}HI6559_DISABLE5_T;    /* 电源模块关闭5寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 5; /* [4..0] 保留 */
        unsigned int    st_lvs7_int                : 1; /* [5..5] LVS7开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
        unsigned int    reserved_0                 : 1; /* [6..6] 保留 */
        unsigned int    st_lvs9_int                : 1; /* [7..7] LVS9开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
    } bits;
    unsigned int    u32;
}HI6559_ONOFF_STATUS5_T;    /* 电源模块开关状态5寄存器。 */

typedef union
{
    struct
    {
        unsigned int    en_ldo9_int                : 1; /* [0..0] 0：关闭LDO9 ；1：开启LDO9 。 */
        unsigned int    sim_en_int                 : 1; /* [1..1] simcard转换电路的使能信号：0：关闭simcard转换电路 ；1：开启simcard转换电路 。 */
        unsigned int    en_ldo11_int               : 1; /* [2..2] 0：关闭LDO11 ；1：开启LDO11 。 */
        unsigned int    reserved                   : 5; /* [7..3] 保留。 */
    } bits;
    unsigned int    u32;
}HI6559_SIM_LDO9_LDO11_EN_T;    /* SIM卡/LDO9/LDO11使能寄存器。 */

typedef union
{
    struct
    {
        unsigned int    simcard_resv               : 2; /* [1..0] 控制CLK输出级的驱动能力，simcard_resv[1]控制P管的驱动能力，simcard_resv[0]控制N管的驱动能力。0：不增加驱动能力；1：增加驱动能力。 */
        unsigned int    simcard_bbrp               : 1; /* [2..2] simcard的DATAIO BB测上拉电阻使能信号：0：开启上拉电阻；1：关闭上拉电阻 。 */
        unsigned int    simcard_simrp              : 1; /* [3..3] simcard的DATAIO SIM卡测上拉电阻使能信号：0：开启上拉电阻；1：关闭上拉电阻 。 */
        unsigned int    simcard_200ns_en           : 1; /* [4..4] simcard 200ns快速上拉时间的使能信号：0：关闭快速上拉功能 ；1：开始快速上拉功能 。 */
        unsigned int    reserved                   : 3; /* [7..5] 保留。 */
    } bits;
    unsigned int    u32;
}HI6559_SIM_CFG_T;    /* SIM卡配置寄存器。 */

typedef union
{
    struct
    {
        unsigned int    simcard_deb_sel            : 5; /* [4..0] SIMCARD上下沿去抖时间可调：0x3 : 120us0x4 : 150us0x5 : 180us0x6 : 210us0x7 : 240us0x8 : 270us0x9 : 300us0xa : 330us0xb : 360us0xc : 390us0xd : 420us0xe : 450us0xf : 480us0x10: 510us0x11: 540us0x12: 570us0x13: 600us */
        unsigned int    reserved                   : 3; /* [7..5] 保留。 */
    } bits;
    unsigned int    u32;
}HI6559_SIM_DEB_T;    /* SIM卡中断去抖寄调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    en_eco_buck3_int           : 1; /* [0..0] 设置BUCK3是否随PMU进出ECO模式:0:不跟随；1：跟随。 */
        unsigned int    en_eco_buck4_int           : 1; /* [1..1] 设置BUCK4是否随PMU进出ECO模式:0:不跟随；1：跟随。 */
        unsigned int    reserved_1                 : 1; /* [2..2] 保留 */
        unsigned int    en_eco_buck6_int           : 1; /* [3..3] 设置BUCK6是否随PMU进出ECO模式:0:不跟随；1：跟随。 */
        unsigned int    reserved_0                 : 4; /* [7..4] 保留。 */
    } bits;
    unsigned int    u32;
}HI6559_ECO_MOD_CFG1_T;    /* 电源模块是否跟随PMU进出ECO模式配置1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    en_eco_ldo3_int            : 1; /* [0..0] 设置LDO3是否随PMU进出ECO模式:0:不跟随；1：跟随。 */
        unsigned int    en_eco_ldo7_int            : 1; /* [1..1] 设置LDO7是否随PMU进出ECO模式:0:不跟随；1：跟随。 */
        unsigned int    en_eco_ldo9_int            : 1; /* [2..2] 设置LDO9是否随PMU进出ECO模式:0:不跟随；1：跟随。 */
        unsigned int    en_eco_ldo10_int           : 1; /* [3..3] 设置LDO10是否随PMU进出ECO模式:0:不跟随；1：跟随。 */
        unsigned int    reserved                   : 1; /* [4..4] 保留 */
        unsigned int    en_eco_ldo12_int           : 1; /* [5..5] 设置LDO12是否随PMU进出ECO模式:0:不跟随；1：跟随。 */
        unsigned int    en_eco_ldo22_int           : 1; /* [6..6] 设置LDO22是否随PMU进出ECO模式:0:不跟随；1：跟随。 */
        unsigned int    en_eco_ldo24_int           : 1; /* [7..7] 设置LDO24是否随PMU进出ECO模式:0:不跟随；1：跟随。 */
    } bits;
    unsigned int    u32;
}HI6559_ECO_MOD_CFG2_T;    /* 电源模块是否跟随PMU进出ECO模式配置2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    force_eco_buck3_int        : 1; /* [0..0] 设置是否强行使BUCK3进入ECO模式：0:不进入；1：进入。 */
        unsigned int    force_eco_buck4_int        : 1; /* [1..1] 设置是否强行使BUCK4进入ECO模式：0:不进入；1：进入。 */
        unsigned int    reserved_1                 : 1; /* [2..2] 保留 */
        unsigned int    force_eco_buck6_int        : 1; /* [3..3] 设置是否强行使BUCK6进入ECO模式：0:不进入；1：进入。 */
        unsigned int    reserved_0                 : 4; /* [7..4] 保留。 */
    } bits;
    unsigned int    u32;
}HI6559_ECO_MOD_CFG3_T;    /* 电源是否强制随PMU进出ECO模式配置3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    force_eco_ldo3_int         : 1; /* [0..0] 设置是否强行使LDO3进入ECO模式：0:不进入；1：进入。 */
        unsigned int    force_eco_ldo7_int         : 1; /* [1..1] 设置是否强行使LDO7进入ECO模式：0:不进入；1：进入。 */
        unsigned int    force_eco_ldo9_int         : 1; /* [2..2] 设置是否强行使LDO9进入ECO模式：0:不进入；1：进入。 */
        unsigned int    force_eco_ldo10_int        : 1; /* [3..3] 设置是否强行使LDO10进入ECO模式：0:不进入；1：进入。 */
        unsigned int    reserved                   : 1; /* [4..4] 保留 */
        unsigned int    force_eco_ldo12_int        : 1; /* [5..5] 设置是否强行使LDO12进入ECO模式：0:不进入；1：进入。 */
        unsigned int    force_eco_ldo22_int        : 1; /* [6..6] 设置是否强行使LDO22进入ECO模式：0:不进入；1：进入。 */
        unsigned int    force_eco_ldo24_int        : 1; /* [7..7] 设置是否强行使LDO24进入ECO模式：0:不进入；1：进入。 */
    } bits;
    unsigned int    u32;
}HI6559_ECO_MOD_CFG4_T;    /* 电源是否强制随PMU进出ECO模式配置4寄存器。 */

typedef union
{
    struct
    {
        unsigned int    eco_pwrsel                 : 2; /* [1..0] PMU进出低功耗时序选择：00：PMU进出低功耗时序图组合1；01：PMU进出低功耗时序图组合2；10：PMU进出低功耗时序图组合1；11：PMU进出低功耗时序图组合1； */
        unsigned int    peri_en_ctrl_ldo12         : 1; /* [2..2] LDO12是否受PERI_EN控制上下电:0： 不受控 ；1： 受控 。 */
        unsigned int    peri_en_ctrl_lvs9          : 1; /* [3..3] LVS9是否受PERI_EN控制上下电:0： 不受控 ；1： 受控 。 */
        unsigned int    peri_en_ctrl_ldo8          : 1; /* [4..4] LDO8是否受PERI_EN控制上下电:0： 不受控 ；1： 受控 。 */
        unsigned int    peri_en_ctrl_buck3         : 1; /* [5..5] BUCK3是否受PERI_EN控制上下电:0： 不受控 ；1： 受控 。 */
        unsigned int    reserved                   : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_ENB3_ECO_MARK_T;    /* 与ENB3硬线和ECO功能相关的屏蔽信号寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 5; /* [4..0] 保留 */
        unsigned int    buck0_nmos_ocp_sel         : 1; /* [5..5] buck0 nmos ocp选择信号                                                                   0：选择nmos ocp                                                                               1：屏蔽nmos ocp */
        unsigned int    buck0_ocp_tran_enhance     : 1; /* [6..6] buck0 ocp 后关闭时间调节                                                               0：默认                                                                                             1：由环路决定 */
        unsigned int    buck0_apt_leak_sel         : 1; /* [7..7] buck0 apt调压快速下拉功能选择                                                   0：开启                                                                                                1：关闭 */
    } bits;
    unsigned int    u32;
}HI6559_BUCK0_REG1_ADJ_T;    /* BUCK0调节1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck_osc                   : 5; /* [4..0] 控制buck频率,默认频率为2.5MHz */
        unsigned int    reserved                   : 3; /* [7..5] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_BUCK0_REG2_ADJ_T;    /* BUCK0调节2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 3; /* [2..0] 保留 */
        unsigned int    buck0_leak_off_sel         : 1; /* [3..3] buck0快速下拉是否强制关闭P管选择                                                     0：不强制关闭                                                                                     1：强制关闭 */
        unsigned int    reserved_0                 : 4; /* [7..4] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_BUCK0_REG3_ADJ_T;    /* BUCK0调节3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck0_comp_adj             : 8; /* [7..0] BUCK0稳定型调节寄存器<7>: OCP新旧方案选择寄存器0：老方案1：新方案<6：0>：用于调整EA的跨导及调零点的电阻 */
    } bits;
    unsigned int    u32;
}HI6559_BUCK0_REG4_ADJ_T;    /* BUCK0调节4寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck0_pdm_sel              : 8; /* [7..0] 使能APT功能信号00000000：APT功能无效00000001：APT功能有效 */
    } bits;
    unsigned int    u32;
}HI6559_BUCK0_REG5_ADJ_T;    /* BUCK0调节5寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck_osc_random            : 8; /* [7..0] <7>:buck0 ocp关闭控制  0：不关闭ocp模块  1：关闭ocp模块<6>:buck5 ocp关闭控制  0：不关闭ocp模块  1：关闭ocp模块<5>:振荡器脉宽控制  0：默认脉宽；  1：脉宽减小；<4>:振荡器比较器选择  0：1.8V比较器；  1：5V比较器；<3:2>:buck5 p管驱动调节<1:0>:buck5 n管驱动调节 */
    } bits;
    unsigned int    u32;
}HI6559_BUCK0_REG6_ADJ_T;    /* BUCK0调节6寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck0_ocp_adj_old          : 4; /* [3..0] 老ocp点调节寄存器 */
        unsigned int    buck0_ocp_n_sel            : 4; /* [7..4] buck0 nocp点调节 */
    } bits;
    unsigned int    u32;
}HI6559_BUCK0_REG7_ADJ_T;    /* BUCK0调节7寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck0_dt_sel               : 2; /* [1..0] BUCK0死区时间调节<0>0：增加死区时间1：减小死区时间<1>0：减小死区时间1：增加死区时间 */
        unsigned int    buck0_apt_ss               : 1; /* [2..2] buck0使能延时调节寄存器 */
        unsigned int    buck0_sl_sel               : 1; /* [3..3] buck0斜坡电压斜率调节寄存器                                                               0：默认                                                                                                    1：加大斜率 */
        unsigned int    buck0_cs_os_sel            : 1; /* [4..4] buck0 W/L模下电流采样offset调节寄存器 */
        unsigned int    buck0_ocp_mode_sel         : 1; /* [5..5] buck0 ocp模式选择                                                                             0：p管和n管同时检测                                                                           1：检测p管，延时600ns */
        unsigned int    buck0_vodet_en             : 1; /* [6..6] vo检测功能；1：默认，无vo检测；0：有vo检测； */
        unsigned int    buck0_dmd_sHI6559eld           : 1; /* [7..7] 屏蔽dmd；0：不屏蔽；1：屏蔽 */
    } bits;
    unsigned int    u32;
}HI6559_BUCK0_REG8_ADJ_T;    /* BUCK0调节8寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck0_eacap_sel            : 2; /* [1..0] 调节EA输入对管处的电容                                                                      10: defaut  2.8pf */
        unsigned int    reserved_1                 : 2; /* [3..2] 保留 */
        unsigned int    buck0_dmd_sel              : 3; /* [6..4] buck0 dmd点选择寄存器 */
        unsigned int    reserved_0                 : 1; /* [7..7] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_BUCK0_REG9_ADJ_T;    /* BUCK0调节9寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck0_ocp_sel_old          : 1; /* [0..0] buck0老ocp点设置1：限流点提高0：限流点减低 */
        unsigned int    reserved_1                 : 1; /* [1..1] 保留 */
        unsigned int    buck0_sft_sel              : 1; /* [2..2] buck0软启动时间选择 */
        unsigned int    buck0_ocp_delay_sel        : 1; /* [3..3] buck0 ocp滤波时间选择                                                                       0：默认                                                                                               1：延时增加一倍 */
        unsigned int    reserved_0                 : 4; /* [7..4] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_BUCK0_REG10_ADJ_T;    /* BUCK0调节10寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck0_sc_sel               : 2; /* [1..0] 斜坡补偿电阻调节 */
        unsigned int    reserved_1                 : 2; /* [3..2] 保留 */
        unsigned int    buck0_mod_sel              : 2; /* [5..4] buck0模式选择；X1:PWM模式；10:PFM模式；00:freed； */
        unsigned int    reserved_0                 : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_BUCK0_REG11_ADJ_T;    /* BUCK0调节11寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck0_leak_mos_sel         : 2; /* [1..0] buck0快速下拉管尺寸选择                                                                    00~11逐渐增大 */
        unsigned int    reserved_1                 : 2; /* [3..2] 保留 */
        unsigned int    buck0_sg_sel               : 2; /* [5..4] 斜坡补偿充电电流调节00~11:充电电流逐渐增大； */
        unsigned int    reserved_0                 : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_BUCK0_REG12_ADJ_T;    /* BUCK0调节12寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved_2                 : 2; /* [1..0] 保留 */
        unsigned int    buck0_ocp_sHI6559eld           : 1; /* [2..2] buck0 ocp保护功能设置0：有过流保护功能1：无过流保护功能 */
        unsigned int    reserved_1                 : 1; /* [3..3] 保留 */
        unsigned int    buck0_ton                  : 2; /* [5..4] buck0最小ton时间调节；00：5ns；01：15ns；10：25ns default；11：35ns； */
        unsigned int    reserved_0                 : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_BUCK0_REG13_ADJ_T;    /* BUCK0调节13寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck0_ccm_ctrl             : 1; /* [0..0] buck0的强制连续模式功能1：强制连续模式功能关闭0：强制连续模式功能开启。 */
        unsigned int    buck0_cmp_i_sel            : 1; /* [1..1] PWM比较器偏置电流调节寄存器                                                       0:1uA                                                                                                     1:2uA */
        unsigned int    buck0_shortn               : 1; /* [2..2] buck0的短路保护功能信号0：有短路保护1：无短路保护 */
        unsigned int    buck0_pfm_sleep            : 1; /* [3..3] pwm模式屏蔽寄存器                                                                               0：不屏蔽                                                                                             1：屏蔽 */
        unsigned int    buck0_drv_adj              : 2; /* [5..4] 不同模式下驱动和电流采样offset调节寄存器                                    <1>: 驱动调节                                                                                            <0>: 电流采样offset调节 */
        unsigned int    buck0_clamp_en             : 1; /* [6..6] VZ箝位功能开关0：关闭箝位功能1：开启箝位功能 */
        unsigned int    reserved                   : 1; /* [7..7] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_BUCK0_REG14_ADJ_T;    /* BUCK0调节14寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck0_ocp_sel              : 3; /* [2..0] buck0 ocp点调节寄存器 */
        unsigned int    buck0_apt_res              : 1; /* [3..3] EA补偿电阻钳位使能寄存器                                                               <0>:                                                                                               0：补偿电阻不随VO变化                                                                         1：补偿电阻是否随VO变化有APT决定 */
        unsigned int    buck0_hyctrl               : 1; /* [4..4] APT工作时输出译码比较器迟滞控制0：默认；1：加大迟滞； */
        unsigned int    reserved                   : 3; /* [7..5] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_BUCK0_REG15_ADJ_T;    /* BUCK0调节15寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck0_reserve              : 8; /* [7..0] <7:6>:设置APT工作时输入参考电压的RC常数00：默认<5>:电流检测的delay值cs_sel1：默认0：加大delay<4>:APT工作时的驱动分块device_sel1：默认，不分块0：分块<3>:PMOS驱动上升时间调节PR1:默认0：加大上升时间<2>:PMOS驱动下降时间调节PF0:默认1：加大下降时间<1>:NMOS驱动上升时间调节NR1:默认0：加大上升时间<0>:NMOS驱动下降时间调节NF0:默认1：加大下降时间 */
    } bits;
    unsigned int    u32;
}HI6559_BUCK0_REG16_ADJ_T;    /* BUCK0调节16寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck3_adj_rlx              : 4; /* [3..0] buck3 LX反馈滤波电阻调节（每bit单独控制）:1：电阻不接入 ；0：电阻接入。 */
        unsigned int    buck3_dbias                : 4; /* [7..4] buck3 <1:0>比较器偏置电流调节（00~11增大） buck3<3:2> 比较器主偏置电流调节（00~11增大） */
    } bits;
    unsigned int    u32;
}HI6559_BUCK3_REG1_ADJ_T;    /* BUCK3调节1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck3_dmd_sel              : 5; /* [4..0] buck3 dmd点选择（000~111增大） */
        unsigned int    buck3_ocp_sel              : 2; /* [6..5] buck3 OCP点调整： */
        unsigned int    reserved                   : 1; /* [7..7] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_BUCK3_REG2_ADJ_T;    /* BUCK3调节2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck3_ocp_toff             : 2; /* [1..0] buck3发生ocp时最小关断时间量 （00~11增大） */
        unsigned int    buck3_regop_clamp          : 1; /* [2..2] regulator 嵌位电路使能：1：使能；0：不使能。 */
        unsigned int    buck3_dmd_clamp            : 1; /* [3..3] 老dmd嵌位使能信号：0：加上；1：不加上。 */
        unsigned int    buck3_ocp_delay            : 1; /* [4..4] ocp屏蔽时间延长20ns信号：0：不延长；1：延长。 */
        unsigned int    buck3_cmp_filter           : 1; /* [5..5] 比较器滤毛刺功能（0：不滤毛刺；1：滤毛刺）。 */
        unsigned int    buck3_sleep_dmd            : 1; /* [6..6] 在正常模式下强制NMOS管关闭：0：不强制；1：强制。 */
        unsigned int    buck3_ton_dmd              : 1; /* [7..7] buck3发生dmd时屏蔽控制最小导通时间 （0：不屏蔽） */
    } bits;
    unsigned int    u32;
}HI6559_BUCK3_REG3_ADJ_T;    /* BUCK3调节3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck3_ng_dt_sel            : 1; /* [0..0] buck3 P管到N管死区模式选择（每bit单独控制）：0：不并入新死区控制模式；1：并入新死区控制模式。 */
        unsigned int    buck3_pg_dt_sel            : 1; /* [1..1] buck3 N管到P管死区模式选择（每bit单独控制）：0：不并入新死区控制模式；1：并入新死区控制模式。 */
        unsigned int    buck3_nmos_switch          : 1; /* [2..2] buck3 sleep到normal切换时NMOS开启切换方式：（0：和dmd同时切换，1：在dmd之后切换） */
        unsigned int    reserved                   : 5; /* [7..3] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_BUCK3_REG4_ADJ_T;    /* BUCK3调节4寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck3_pg_n_sel             : 2; /* [1..0] buck3 P power管驱动对应NMOS驱动能力调节（0x0~0x3减小驱动能力）。 */
        unsigned int    reserved_1                 : 2; /* [3..2] 保留 */
        unsigned int    buck3_pg_p_sel             : 2; /* [5..4] buck3 P power管驱动对应PMOS驱动能力调节（0x0~0x3增加驱动能力） */
        unsigned int    reserved_0                 : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_BUCK3_REG5_ADJ_T;    /* BUCK3调节5寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck3_ng_n_sel             : 2; /* [1..0] buck3 N power管驱动对应NMOS驱动能力调节（00~11增加驱动能力） */
        unsigned int    reserved_1                 : 2; /* [3..2] 保留 */
        unsigned int    buck3_ng_p_sel             : 2; /* [5..4] buck3 N power管驱动对应PMOS驱动能力调节（0x0~0x3减小驱动能力）。 */
        unsigned int    reserved_0                 : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_BUCK3_REG6_ADJ_T;    /* BUCK3调节6寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck3_adj_clx              : 2; /* [1..0] buck3 LX反馈滤波电容调节（11：电容全接入，00：电容全不接入） */
        unsigned int    buck3_ocp_dis              : 1; /* [2..2] buck3 内部OCP环路屏蔽信号：0：不屏蔽；1：屏蔽 。 */
        unsigned int    reserved_1                 : 1; /* [3..3] 保留 */
        unsigned int    buck3_ton                  : 2; /* [5..4] buck3最小导通时间。 */
        unsigned int    reserved_0                 : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_BUCK3_REG7_ADJ_T;    /* BUCK3调节7寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck3_sleep_ng             : 1; /* [0..0] buck3用于关闭NMOS power管。0：不强制关闭，1：强制关闭 */
        unsigned int    buck3_bias_reg             : 1; /* [1..1] buck3内部regulator电流调节（0:0.5uA，1:1uA） */
        unsigned int    buck3_short_pdp            : 1; /* [2..2] 短路保护屏蔽寄存器：0:使能短路保护；1:不使能短路保护。 */
        unsigned int    buck3_reg_ss               : 1; /* [3..3] buck3软启动时内部regulator状态：0：正常状态；1：单位增益反馈。 */
        unsigned int    buck3_dt_sel               : 2; /* [5..4] buck3死区时间调节（每bit单独控制）：1：不额外延迟 ；0：额外延迟5ns 。 */
        unsigned int    buck3_regop_c              : 1; /* [6..6] buck3内部regulator OP输出电容，用于调节OP的slew-rate。 */
        unsigned int    reserved                   : 1; /* [7..7] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_BUCK3_REG8_ADJ_T;    /* BUCK3调节8寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck3_reg_dr               : 3; /* [2..0] buck3内部regulator电阻，用于调节regulator调整精度范围。 */
        unsigned int    buck3_en_reg               : 1; /* [3..3] buck3内部精度调整器使能信号：0：使能；1：不使能。 */
        unsigned int    buck3_dmd_ton              : 3; /* [6..4] buck3发生dmd时的加入导通时间量（000~111增大） */
        unsigned int    reserved                   : 1; /* [7..7] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_BUCK3_REG9_ADJ_T;    /* BUCK3调节9寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck3_filter_ton           : 2; /* [1..0] 比较器滤毛刺（00~11增大） */
        unsigned int    reserved_1                 : 2; /* [3..2] 保留 */
        unsigned int    buck3_reg_r                : 2; /* [5..4] buck3内部regulator电阻，用于调节regulator带宽。 */
        unsigned int    reserved_0                 : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_BUCK3_REG10_ADJ_T;    /* BUCK3调节10寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck5_ocp_tran_enhance     : 8; /* [7..0] buck5 ocp 后关闭时间调节                                                               00000000：默认                                                                                             00000001：由环路决定 */
    } bits;
    unsigned int    u32;
}HI6559_BUCK5_REG0_ADJ_T;    /* BUCK3调节12寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck4_adj_rlx              : 4; /* [3..0] buck4 LX反馈滤波电阻调节（每bit单独控制）:1：电阻不接入 ；0：电阻接入。 */
        unsigned int    buck4_dbias                : 4; /* [7..4] buck4 <1:0>比较器偏置电流调节（00~11增大） buck4<3:2> 比较器主偏置电流调节（00~11增大） */
    } bits;
    unsigned int    u32;
}HI6559_BUCK4_REG1_ADJ_T;    /* BUCK4调节1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck4_dmd_sel              : 5; /* [4..0] buck4 dmd点选择（000~111增大） */
        unsigned int    buck4_ocp_sel              : 2; /* [6..5] buck4 OCP点调整 */
        unsigned int    reserved                   : 1; /* [7..7] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_BUCK4_REG2_ADJ_T;    /* BUCK4调节2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck4_ocp_toff             : 2; /* [1..0] buck4发生ocp时最小关断时间量 （00~11增大） */
        unsigned int    buck4_regop_clamp          : 1; /* [2..2] regulator 嵌位电路使能：1：使能；0：不使能。 */
        unsigned int    buck4_dmd_clamp            : 1; /* [3..3] 老dmd嵌位使能信号：0：加上；1：不加上。 */
        unsigned int    buck4_ocp_delay            : 1; /* [4..4] ocp屏蔽时间延长20ns信号：0：不延长；1：延长。 */
        unsigned int    buck4_cmp_filter           : 1; /* [5..5] 比较器滤毛刺功能（0：不滤毛刺；1：滤毛刺） */
        unsigned int    buck4_sleep_dmd            : 1; /* [6..6] 在正常模式下强制NMOS管关闭：0：不强制；1：强制。 */
        unsigned int    buck4_ton_dmd              : 1; /* [7..7] buck4发生dmd时屏蔽控制最小导通时间 （0：不屏蔽） */
    } bits;
    unsigned int    u32;
}HI6559_BUCK4_REG3_ADJ_T;    /* BUCK4调节3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck4_ng_dt_sel            : 1; /* [0..0] buck4 P管到N管死区模式选择（每bit单独控制）：0：不并入新死区控制模式；1：并入新死区控制模式。 */
        unsigned int    buck4_pg_dt_sel            : 1; /* [1..1] buck4 N管到P管死区模式选择（每bit单独控制）：0：不并入新死区控制模式；1：并入新死区控制模式。 */
        unsigned int    buck4_nmos_switch          : 1; /* [2..2] buck4 sleep到normal切换时NMOS开启切换方式：（0：和dmd同时切换，1：在dmd之后切换） */
        unsigned int    reserved                   : 5; /* [7..3] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_BUCK4_REG4_ADJ_T;    /* BUCK4调节4寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck4_pg_n_sel             : 2; /* [1..0] buck4 P power管驱动对应NMOS驱动能力调节（0x0~0x3减小驱动能力）。 */
        unsigned int    reserved_1                 : 2; /* [3..2] 保留 */
        unsigned int    buck4_pg_p_sel             : 2; /* [5..4] buck4 P power管驱动对应PMOS驱动能力调节（0x0~0x3增加驱动能力） */
        unsigned int    reserved_0                 : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_BUCK4_REG5_ADJ_T;    /* BUCK4调节5寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck4_ng_n_sel             : 2; /* [1..0] buck4 N power管驱动对应NMOS驱动能力调节（0x0~0x3增加驱动能力)。 */
        unsigned int    reserved_1                 : 2; /* [3..2] 保留 */
        unsigned int    buck4_ng_p_sel             : 2; /* [5..4] buck4 N power管驱动对应PMOS驱动能力调节（0x0~0x3减小驱动能力）。 */
        unsigned int    reserved_0                 : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_BUCK4_REG6_ADJ_T;    /* BUCK4调节6寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck4_adj_clx              : 2; /* [1..0] buck4 LX反馈滤波电容调节（11：电容全接入，00：电容全不接入） */
        unsigned int    buck4_ocp_dis              : 1; /* [2..2] buck4 内部OCP环路屏蔽信号：0：不屏蔽；1：屏蔽 。 */
        unsigned int    reserved_1                 : 1; /* [3..3] 保留 */
        unsigned int    buck4_ton                  : 2; /* [5..4] buck4最小导通时间。 */
        unsigned int    reserved_0                 : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_BUCK4_REG7_ADJ_T;    /* BUCK4调节7寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck4_sleep_ng             : 1; /* [0..0] buck4用于关闭NMOS power管。0：不强制关闭，1：强制关闭 */
        unsigned int    buck4_bias_reg             : 1; /* [1..1] buck4内部regulator电流调节（0:0.5uA，1:1uA） */
        unsigned int    buck4_short_pdp            : 1; /* [2..2] 短路保护屏蔽寄存器：0:使能短路保护；1:不使能短路保护。 */
        unsigned int    buck4_reg_ss               : 1; /* [3..3] buck4软启动时内部regulator状态：0：正常状态；1：单位增益反馈。 */
        unsigned int    buck4_dt_sel               : 2; /* [5..4] buck4死区时间调节（每bit单独控制）：1：不额外延迟 ；0：额外延迟5ns 。 */
        unsigned int    buck4_regop_c              : 1; /* [6..6] buck4内部regulator OP输出电容，用于调节OP的slew-rate。 */
        unsigned int    reserved                   : 1; /* [7..7] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_BUCK4_REG8_ADJ_T;    /* BUCK4调节8寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck4_reg_dr               : 3; /* [2..0] buck4内部regulator电阻，用于调节regulator调整精度范围。 */
        unsigned int    buck4_en_reg               : 1; /* [3..3] buck4内部精度调整器使能信号：0：使能；1：不使能。 */
        unsigned int    buck4_dmd_ton              : 3; /* [6..4] buck4发生dmd时的加入导通时间量（000~111增大） */
        unsigned int    reserved                   : 1; /* [7..7] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_BUCK4_REG9_ADJ_T;    /* BUCK4调节9寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck4_filter_ton           : 2; /* [1..0] 比较器滤毛刺（00~11增大） */
        unsigned int    reserved_1                 : 2; /* [3..2] 保留 */
        unsigned int    buck4_reg_r                : 2; /* [5..4] buck4内部regulator电阻，用于调节regulator带宽。 */
        unsigned int    reserved_0                 : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_BUCK4_REG10_ADJ_T;    /* BUCK4调节10寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck4_reserve              : 8; /* [7..0] buck4备用寄存器 */
    } bits;
    unsigned int    u32;
}HI6559_BUCK4_REG12_ADJ_T;    /* BUCK4调节12寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck5_ocp_n_sel            : 4; /* [3..0] buck5 nocp点调节 */
        unsigned int    reserved                   : 4; /* [7..4] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_BUCK5_REG1_ADJ_T;    /* BUCK5调节1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 5; /* [4..0] 保留 */
        unsigned int    buck5_ocp_sel              : 2; /* [6..5] buck5 老ocp档位调节寄存器 */
        unsigned int    reserved_0                 : 1; /* [7..7] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_BUCK5_REG2_ADJ_T;    /* BUCK5调节2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck5_dmd_sel              : 2; /* [1..0] 00:67mA01:101mA10:-50mA11:34mA  Defaut */
        unsigned int    reserved                   : 6; /* [7..2] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_BUCK5_REG3_ADJ_T;    /* BUCK5调节3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck5_vz_sel               : 1; /* [0..0] EA输出钳位电压调节寄存器 */
        unsigned int    buck5_ocp_sHI6559eld           : 1; /* [1..1] buck5 ocp保护功能设置0：有过流保护功能1：无过流保护功能 */
        unsigned int    buck5_sft_sel              : 1; /* [2..2] buck5软启动时间选择寄存器 */
        unsigned int    reserved                   : 5; /* [7..3] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_BUCK5_REG4_ADJ_T;    /* BUCK5调节4寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck5_sg_sel               : 2; /* [1..0] 斜坡补偿充电电流调节00~11:充电电流逐渐增大； */
        unsigned int    reserved_1                 : 2; /* [3..2] 保留 */
        unsigned int    buck5_sc_sel               : 2; /* [5..4] 斜坡补偿电阻调节 */
        unsigned int    reserved_0                 : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_BUCK5_REG5_ADJ_T;    /* BUCK5调节5寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 2; /* [1..0] 保留 */
        unsigned int    buck5_shortn               : 1; /* [2..2] buck5的短路保护功能信号0：有短路保护1：无短路保护 */
        unsigned int    reserved_0                 : 5; /* [7..3] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_BUCK5_REG6_ADJ_T;    /* BUCK5调节6寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck5_ccm_ctrl             : 1; /* [0..0] buck5的强制连续模式功能1：强制连续模式功能关闭0：强制连续模式功能开启 */
        unsigned int    buck5_ocp_delay_sel        : 1; /* [1..1] buck5 ocp滤波时间选择                                                                       0：默认                                                                                               1：延时增加一倍 */
        unsigned int    buck5_ocp_mode_sel         : 1; /* [2..2] buck5 ocp模式选择                                                                             0：p管和n管同时检测                                                                           1：检测p管，延时600ns */
        unsigned int    buck5_ocp_set              : 1; /* [3..3] buck5 ocp新老方案选择                                                                     0：老方案                                                                                             1：新方案 */
        unsigned int    reserved_1                 : 2; /* [5..4] 保留 */
        unsigned int    buck5_cs_lxb               : 1; /* [6..6] current sense开启信号选择寄存器                                                      0：LX上升沿开启current sense                                                         1：屏蔽LX上升沿开启current sense */
        unsigned int    reserved_0                 : 1; /* [7..7] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_BUCK5_REG7_ADJ_T;    /* BUCK5调节7寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck5_ocp_p_sel            : 3; /* [2..0] buck5 pocp点调节 */
        unsigned int    buck5_dt_sel               : 1; /* [3..3] BUCK5死区时间调节0：默认1：增大死区时间（5ns） */
        unsigned int    buck5_nmos_ocp_sel         : 1; /* [4..4] buck5 nmos ocp选择信号                                                                   0：选择nmos ocp                                                                               1：屏蔽nmos ocp */
        unsigned int    reserved                   : 3; /* [7..5] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_BUCK5_REG8_ADJ_T;    /* BUCK5调节8寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck5_comp_adj             : 8; /* [7..0] BUCK5稳定性调节寄存器<7>: 设置设置GATE_DRV1：减弱0：加强<6>: EA输出钳位使能                                                                0：关闭钳位功能                                                                             1：开启钳位功能 <5>: 软启动过程中OCP是否上报                                                            0：允许上报                                                                            1：不允许上报<4：0>：用于调整EA的跨导及调零点的电阻 */
    } bits;
    unsigned int    u32;
}HI6559_BUCK5_REG9_ADJ_T;    /* BUCK5调节9寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck5_reserve1             : 6; /* [5..0] <7> : VO输出配置                                                                                      写1输出配置为1.2V                                                                              <6>：isens 电流补偿寄存器                                                             0：开启                                                                                                      1：关闭                                                                                              <5>：PWM比较器电流调节寄存器                                                        0：1uA                                                                                                 1：2uA                                                                                                         <4>：老ocp档位调节寄存器                                                                      0：默认值                                                                                              1：增加350mA                                                                                         <3>：current sense运放采样调节寄存器                                                0：精度和速度增强                                                                               1：常规采样                                                                                      <2>：isens输出滤波电容选择寄存器                                                             0：无滤波电容                                                                                    1：有滤波电容                                                                                         <1>：current sense 运放栅极滤波电容选择寄存器                          0：默认滤波电容                                                                                    1：增大滤波电容                                                                                      <0>：current sense 延时选择寄存器                                                  0：无延时                                                                                                1：增加5ns延时 */
        unsigned int    reserved                   : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_BUCK5_REG10_ADJ_T;    /* BUCK5调节10寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck6_adj_rlx              : 4; /* [3..0] buck6 LX反馈滤波电阻调节（每bit单独控制）:1：电阻不接入 ；0：电阻接入。 */
        unsigned int    buck6_dbias                : 4; /* [7..4] buck6 <1:0>比较器偏置电流调节（00~11增大） buck6<3:2> 比较器主偏置电流调节（00~11增大） */
    } bits;
    unsigned int    u32;
}HI6559_BUCK6_REG1_ADJ_T;    /* BUCK6调节1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck6_dmd_sel              : 5; /* [4..0] buck6 dmd点选择（000~111增大） */
        unsigned int    buck6_ocp_sel              : 2; /* [6..5] buck6 OCP点调整 */
        unsigned int    reserved                   : 1; /* [7..7] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_BUCK6_REG2_ADJ_T;    /* BUCK6调节2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck6_ocp_toff             : 2; /* [1..0] buck6发生ocp时最小关断时间量 （00~11增大） */
        unsigned int    buck6_regop_clamp          : 1; /* [2..2] regulator 嵌位电路使能：1：使能；0：不使能。 */
        unsigned int    buck6_dmd_clamp            : 1; /* [3..3] dmd嵌位使能信号（0：加上；1：不加上） */
        unsigned int    buck6_ocp_delay            : 1; /* [4..4] ocp屏蔽时间延长20ns信号：0：不延长；1：延长。 */
        unsigned int    buck6_cmp_filter           : 1; /* [5..5] 比较器滤毛刺功能（0：不滤毛刺；1：滤毛刺） */
        unsigned int    buck6_sleep_dmd            : 1; /* [6..6] 在正常模式下强制NMOS管关闭：0：不强制；1：强制。 */
        unsigned int    buck6_ton_dmd              : 1; /* [7..7] buck6发生dmd时屏蔽控制最小导通时间 （0：不屏蔽） */
    } bits;
    unsigned int    u32;
}HI6559_BUCK6_REG3_ADJ_T;    /* BUCK6调节3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck6_ng_dt_sel            : 1; /* [0..0] buck6 P管到N管死区模式选择（每bit单独控制）：0：不并入新死区控制模式；1：并入新死区控制模式。 */
        unsigned int    buck6_pg_dt_sel            : 1; /* [1..1] buck6 N管到P管死区模式选择（每bit单独控制）：0：不并入新死区控制模式；1：并入新死区控制模式。 */
        unsigned int    buck6_nmos_switch          : 1; /* [2..2] buck6 sleep到normal切换时NMOS开启切换方式：（0：和dmd同时切换，1：在dmd之后切换） */
        unsigned int    reserved                   : 5; /* [7..3] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_BUCK6_REG4_ADJ_T;    /* BUCK6调节4寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck6_pg_n_sel             : 2; /* [1..0] buck6 P power管驱动对应NMOS驱动能力调节（0x0~0x3减小驱动能力）。 */
        unsigned int    reserved_1                 : 2; /* [3..2] 保留 */
        unsigned int    buck6_pg_p_sel             : 2; /* [5..4] buck6 P power管驱动对应PMOS驱动能力调节（0x0~0x3增加驱动能力） */
        unsigned int    reserved_0                 : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_BUCK6_REG5_ADJ_T;    /* BUCK6调节5寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck6_ng_n_sel             : 2; /* [1..0] buck6 N power管驱动对应NMOS驱动能力调节（0x0~0x3增加驱动能力)。 */
        unsigned int    reserved_1                 : 2; /* [3..2] 保留 */
        unsigned int    buck6_ng_p_sel             : 2; /* [5..4] buck6 N power管驱动对应PMOS驱动能力调节（0x0~0x3减小驱动能力）。 */
        unsigned int    reserved_0                 : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_BUCK6_REG6_ADJ_T;    /* BUCK6调节6寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck6_adj_clx              : 2; /* [1..0] buck6 LX反馈滤波电容调节（11：电容全接入，00：电容全不接入） */
        unsigned int    buck6_ocp_dis              : 1; /* [2..2] buck6 内部OCP环路屏蔽信号：0：不屏蔽；1：屏蔽 。 */
        unsigned int    reserved_1                 : 1; /* [3..3] 保留 */
        unsigned int    buck6_ton                  : 2; /* [5..4] buck6最小导通时间。 */
        unsigned int    reserved_0                 : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_BUCK6_REG7_ADJ_T;    /* BUCK6调节7寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck6_sleep_ng             : 1; /* [0..0] buck6用于关闭NMOS power管。0：不强制关闭，1：强制关闭 */
        unsigned int    buck6_bias_reg             : 1; /* [1..1] buck6内部regulator电流调节（0:0.5uA，1:1uA） */
        unsigned int    buck6_short_pdp            : 1; /* [2..2] 短路保护屏蔽寄存器：0:使能短路保护；1:不使能短路保护。 */
        unsigned int    buck6_reg_ss               : 1; /* [3..3] buck6软启动时内部regulator状态：0：正常状态；1：单位增益反馈。 */
        unsigned int    buck6_dt_sel               : 2; /* [5..4] buck6死区时间调节（每bit单独控制）：1：不额外延迟 ；0：额外延迟5ns 。 */
        unsigned int    buck6_regop_c              : 1; /* [6..6] buck6内部regulator OP输出电容，用于调节OP的slew-rate。 */
        unsigned int    reserved                   : 1; /* [7..7] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_BUCK6_REG8_ADJ_T;    /* BUCK6调节8寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck6_reg_dr               : 3; /* [2..0] buck6内部regulator电阻，用于调节regulator带宽。 */
        unsigned int    buck6_en_reg               : 1; /* [3..3] buck6内部精度调整器使能信号：0：使能；1：不使能。 */
        unsigned int    buck6_dmd_ton              : 3; /* [6..4] buck6发生dmd时的加入导通时间量（000~111增大） */
        unsigned int    reserved                   : 1; /* [7..7] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_BUCK6_REG9_ADJ_T;    /* BUCK6调节9寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck6_filter_ton           : 2; /* [1..0] 比较器滤毛刺（00~11增大） */
        unsigned int    reserved_1                 : 2; /* [3..2] 保留 */
        unsigned int    buck6_reg_r                : 2; /* [5..4] buck6内部regulator电阻，用于调节regulator带宽。 */
        unsigned int    reserved_0                 : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_BUCK6_REG10_ADJ_T;    /* BUCK6调节10寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck6_reserve              : 8; /* [7..0] buck6备用寄存器 */
    } bits;
    unsigned int    u32;
}HI6559_BUCK6_REG12_ADJ_T;    /* BUCK6调节12寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck0_dv                   : 8; /* [7..0] BUCK0输出电压设置                                                                              默认输出1.5V  00001100                    00000000～001111110.9V~4.05V64steps，50mV/Step  */
    } bits;
    unsigned int    u32;
}HI6559_VSET_BUCK0_ADJ_T;    /* BUCK0电压调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck3_dv                   : 6; /* [5..0] buck3输出电压调节默认输出0.9V  011001   000000～111111对应0.7V～1.204V64steps，8mV/Step011001 (OTP=0)100110 (OTP=1) */
        unsigned int    reserved                   : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_VSET_BUCK3_ADJ_T;    /* BUCK3电压调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck4_dv                   : 4; /* [3..0] buck4输出电压调节:    默认输出1.225V  0001                    0000～1111对应1.2V～1.575V16steps，25mV/Step */
        unsigned int    reserved                   : 4; /* [7..4] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_VSET_BUCK4_ADJ_T;    /* BUCK3电压调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck5_dv                   : 8; /* [7..0] BUCK5输出电压设置                                                                              默认输出2.2V  00001000                    00000000～00001111对应1.5/1.6/1.7/1.8/1.85/1.9/2.0/2.12.2/2.25/2.3/2.35/2.5/2.8/2.85/3.3 */
    } bits;
    unsigned int    u32;
}HI6559_VSET_BUCK5_ADJ_T;    /* BUCK5电压调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck6_dv                   : 4; /* [3..0] buck6输出电压调节:默认输出1.825V  00010000～1111对应1.8V~2.175V16steps，25mV/Step */
        unsigned int    reserved                   : 4; /* [7..4] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_VSET_BUCK6_ADJ_T;    /* BUCK6电压调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    vset_ldo1                  : 3; /* [2..0] LDO1电压调节：0x0: 1.80V ；0x1: 2.50V ；0x2: 2.60V ；0x3: 2.70V ；0x4: 2.80V ；0x5: 2.85V ； 0x6: 2.90V ；0x7: 3.00V。 */
        unsigned int    reserved                   : 5; /* [7..3] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_LDO1_REG_ADJ_T;    /* LDO1调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    vset_ldo3                  : 3; /* [2..0] LDO3输出电压设置0.825/0.85/0.875/0.9/0.925/0.95/0.975/1.0 */
        unsigned int    reserved_1                 : 1; /* [3..3] 保留 */
        unsigned int    vrset_ldo3                 : 3; /* [6..4] LDO3负载调整率补偿调整，5mV / Step */
        unsigned int    reserved_0                 : 1; /* [7..7] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_LDO3_REG_ADJ_T;    /* LDO3调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    vset_ldo6                  : 3; /* [2..0] LDO6电压调节：0x0: 1.50V ；0x1: 1.80V ；0x2: 1.85V ；0x3: 2.20V ；0x4: 2.50V ；0x5: 2.80V 。 */
        unsigned int    reserved                   : 5; /* [7..3] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_LDO6_REG_ADJ_T;    /* LDO6调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    vset_ldo7                  : 3; /* [2..0] LDO7输出电压设置1.8/1.85/2.8/2.85/2.9/2.95/3.0/3.3 */
        unsigned int    reserved                   : 5; /* [7..3] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_LDO7_REG_ADJ_T;    /* LDO7调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    vset_ldo8                  : 3; /* [2..0] LDO8电压调节：0x0: 1.10V ；0x1: 1.20V ；0x2: 1.25V ；0x3: 1.275V ；0x4: 1.30V ；0x5: 1.325V ；0x6: 1.35V ；0x7：1.375V 。 */
        unsigned int    reserved_1                 : 1; /* [3..3] 保留 */
        unsigned int    vrset_ldo8                 : 3; /* [6..4] LDO8负载调整率补偿调整，5mV / Step */
        unsigned int    reserved_0                 : 1; /* [7..7] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_LDO8_REG_ADJ_T;    /* LDO8调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    vset_ldo9                  : 3; /* [2..0] LDO9电压调节：0x0: 1.80V ；0x1: 1.85V ；0x2: 2.80V ；0x3: 2.85V ；0x4: 2.90V ；0x5: 2.95V ；0x6: 3.00V ；0x7: 3.05V 。 */
        unsigned int    reserved_1                 : 1; /* [3..3] 保留 */
        unsigned int    vrset_ldo9                 : 3; /* [6..4] LDO9负载调整率补偿调整，5mV / Step */
        unsigned int    reserved_0                 : 1; /* [7..7] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_LDO9_REG_ADJ_T;    /* LDO9调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    vset_ldo10                 : 3; /* [2..0] LDO10输出电压设置2.8/2.85/2.9/2.95/3.0/3.05/3.1/3.3 */
        unsigned int    reserved_1                 : 1; /* [3..3] 保留 */
        unsigned int    vrset_ldo10                : 3; /* [6..4] LDO10负载调整率补偿调整，5mV / Step */
        unsigned int    reserved_0                 : 1; /* [7..7] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_LDO10_REG_ADJ_T;    /* LDO10调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    vset_ldo11                 : 3; /* [2..0] LDO11输出电压设置1.8/1.85/2.2/2.8/2.85/2.9/3.0/3.3。 */
        unsigned int    reserved_1                 : 1; /* [3..3] 保留 */
        unsigned int    vrset_ldo11                : 3; /* [6..4] LDO11负载调整率补偿调整，5mV / Step */
        unsigned int    reserved_0                 : 1; /* [7..7] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_LDO11_REG_ADJ_T;    /* LDO11调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    vset_ldo12                 : 3; /* [2..0] LDO12电压调节：0x0: 3.00V ；0x1: 3.05V ；0x2: 3.10V ；0x3: 3.15V ；0x4: 3.20V ；0x5: 3.25V ；0x6: 3.30V ；0x7: 3.35V 。 */
        unsigned int    reserved_1                 : 1; /* [3..3] 保留 */
        unsigned int    vrset_ldo12                : 3; /* [6..4] LDO12负载调整率补偿调整。 */
        unsigned int    reserved_0                 : 1; /* [7..7] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_LDO12_REG_ADJ_T;    /* LDO12调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    vset_ldo13                 : 3; /* [2..0] LDO13电压调节：0x0: 1.80V ；0x1: 2.50V ；0x2: 2.60V ；0x3: 2.70V ；0x4: 2.80V ；0x5: 2.85V ； 0x6: 2.90V ；0x7: 3.00V。 */
        unsigned int    reserved                   : 5; /* [7..3] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_LDO13_REG_ADJ_T;    /* LDO13调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    vset_ldo14                 : 3; /* [2..0] LDO14电压调节：0x0: 1.80V ；0x1: 2.20V ；0x2: 2.60V ；0x3: 2.70V ；0x4: 2.80V ；0x5: 2.85V ； 0x6: 2.90V ；0x7: 3.00V。 */
        unsigned int    reserved_1                 : 1; /* [3..3] 保留 */
        unsigned int    vrset_ldo14                : 3; /* [6..4] LDO14负载调整率补偿调整，5mV / Step */
        unsigned int    reserved_0                 : 1; /* [7..7] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_LDO14_REG_ADJ_T;    /* LDO14调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    vset_ldo22                 : 3; /* [2..0] LDO22输出电压设置1.2/1.225/1.25/1.3/1.425/1.5 */
        unsigned int    reserved                   : 5; /* [7..3] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_LDO22_REG_ADJ_T;    /* LDO22调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    vset_ldo23                 : 3; /* [2..0] LDO23电压调节：0x0: 1.50V ；0x1: 1.80V ；0x2: 1.85V ；0x3: 1.90V ；0x4: 1.95V ； 0x5: 2.50V ；0x6: 2.80V ；0x7: 2.85V 。 */
        unsigned int    reserved_1                 : 1; /* [3..3] 保留 */
        unsigned int    vrset_ldo23                : 3; /* [6..4] LDO23负载调整率补偿调整，5mV / Step */
        unsigned int    reserved_0                 : 1; /* [7..7] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_LDO23_REG_ADJ_T;    /* LDO23调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    vset_ldo24                 : 3; /* [2..0] LDO24电压调节：0x0: 2.80V ；0x1: 2.85V ；0x2: 2.90V ；0x3: 2.95V ；0x4: 3.00V ；0x5: 3.05V ；0x6: 3.10V ；0x7: 3.15V 。 */
        unsigned int    reserved                   : 5; /* [7..3] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_LDO24_REG_ADJ_T;    /* LDO24调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    vset_pmua                  : 3; /* [2..0] PMUA电压调节：0x0: 2.90V ；0x1: 3.00V ；0x2: 3.05V ；0x3: 3.10V ；0x4: 3.15V ； 0x5: 3.20V ；0x6: 3.25V ；0x7: 3.30V。 */
        unsigned int    eco_pmua                   : 1; /* [3..3] PMUA进入ECO模式控制信号:0：代表normal模式；1：代表eco模式。 */
        unsigned int    reserved                   : 4; /* [7..4] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_PMUA_REG_ADJ_T;    /* PMUA调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    sstset_lvs9                : 2; /* [1..0] LVS9缓启速度调节，11最快，00最慢 */
        unsigned int    reserved_1                 : 2; /* [3..2] 保留 */
        unsigned int    sstset_lvs7                : 2; /* [5..4] LVS7缓启速度调节，11最快，00最慢 */
        unsigned int    reserved_0                 : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_LVS_ADJ1_T;    /* LVS调节1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    bg_test                    : 4; /* [3..0] 基准测试模式预留 */
        unsigned int    bg_sleep                   : 1; /* [4..4] 0：ref_top正常工作 ；1：ref_top进入sleep模式。 */
        unsigned int    reserved                   : 3; /* [7..5] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_BANDGAP_THSD_ADJ1_T;    /* BANDGAP和THSD调节1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    en_tmp_int                 : 2; /* [1..0] 0x:THSD过温保护模块关闭；10:THSD过温保护模块开启。11:THSD的低功耗模式。高电平为10ms,周期为1s的周期信号 */
        unsigned int    reserved_1                 : 2; /* [3..2] 保留 */
        unsigned int    thsd_set_tmp               : 2; /* [5..4] 温度保护模块报警温度设置。00:  105℃；01:  115℃；10:  125℃； 11:   135℃；                     默认设置为125℃。 */
        unsigned int    reserved_0                 : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_BANDGAP_THSD_ADJ2_T;    /* BANDGAP和THSD调节2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    en_dr3_int                 : 1; /* [0..0] 0:  关闭DR3；1: 开启DR3。 */
        unsigned int    dr3_mode                   : 1; /* [1..1] 0: 关闭闪动模式(此时表现为持续输出)；1:  开启DR3闪动模式。    */
        unsigned int    en_dr4_int                 : 1; /* [2..2] 0:  关闭DR4；1: 开启DR4。 */
        unsigned int    dr4_mode                   : 1; /* [3..3] 0: 关闭闪动模式(此时表现为持续输出)；1:  开启DR4闪动模式。    */
        unsigned int    en_dr5_int                 : 1; /* [4..4] 0:  关闭DR5；1: 开启DR5。 */
        unsigned int    dr5_mode                   : 1; /* [5..5] 0: 关闭闪动模式(此时表现为持续输出)；1:  开启DR5闪动模式。    */
        unsigned int    reserved                   : 2; /* [7..6] 保留。 */
    } bits;
    unsigned int    u32;
}HI6559_DR_FLA_CTRL1_T;    /* DR闪烁模式控制1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    en_dr1_int                 : 1; /* [0..0] 0:  关闭DR1；1: 开启DR1  。 */
        unsigned int    dr1_mode                   : 1; /* [1..1] 0: 关闭闪动模式(此时表现为持续输出)；1:  开启DR1闪动模式。 */
        unsigned int    reserved_1                 : 2; /* [3..2] 保留 */
        unsigned int    en_dr2_int                 : 1; /* [4..4] 0:  关闭DR2；1: 开启DR2。 */
        unsigned int    dr2_mode                   : 1; /* [5..5] 0: 关闭闪动模式(此时表现为持续输出)；1:  开启DR2闪动模式。    */
        unsigned int    reserved_0                 : 2; /* [7..6] 保留。 */
    } bits;
    unsigned int    u32;
}HI6559_DR_FLA_CTRL2_T;    /* DR闪烁模式控制2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    flash_period               : 8; /* [7..0] 闪烁周期T = 配置值 *31.25ms。 */
    } bits;
    unsigned int    u32;
}HI6559_FLASH_PERIOD_T;    /* DR闪烁模式周期调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    flash_on                   : 8; /* [7..0] 点亮时间t =配置值 *7.8125ms。 */
    } bits;
    unsigned int    u32;
}HI6559_FLASH_ON_T;    /* DR闪烁模式点亮时间调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    dr1_mode_sel               : 1; /* [0..0] 0: DR1工作在呼吸功能模式；1：DR1工作在闪烁模式； */
        unsigned int    dr2_mode_sel               : 1; /* [1..1] 0: DR2工作在呼吸功能模式；1：DR2工作在闪烁模式； */
        unsigned int    dr3_mode_sel               : 1; /* [2..2] 0: DR3工作在呼吸功能模式；1：DR3工作在闪烁模式； */
        unsigned int    dr4_mode_sel               : 1; /* [3..3] 0: DR4工作在呼吸功能模式；1：DR4工作在闪烁模式； */
        unsigned int    dr5_mode_sel               : 1; /* [4..4] 0: DR5工作在呼吸功能模式；1：DR5工作在闪烁模式； */
        unsigned int    reserved                   : 3; /* [7..5] 保留。 */
    } bits;
    unsigned int    u32;
}HI6559_DR_MODE_SEL_T;    /* DR灯模式选择寄存器。 */

typedef union
{
    struct
    {
        unsigned int    dr1_en                     : 1; /* [0..0] 0：dr1关闭 ； 1：dr1使能：dr1_flash_en = 1,闪烁 ；dr1_flash_en = 0,常亮 。 */
        unsigned int    dr1_flash_en               : 1; /* [1..1] 0：dr1不闪烁。1：dr1闪烁 */
        unsigned int    reserved_1                 : 2; /* [3..2] 保留 */
        unsigned int    dr2_en                     : 1; /* [4..4] 0：dr2关闭 ； 1：dr2使能：dr2_flash_en = 1,闪烁 ；dr2_flash_en = 0,常亮 。 */
        unsigned int    dr2_flash_en               : 1; /* [5..5] 0：dr2不闪烁。1：dr2闪烁 */
        unsigned int    reserved_0                 : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_DR_BRE_CTRL_T;    /* DR1/2呼吸模式控制寄存器。 */

typedef union
{
    struct
    {
        unsigned int    dr1_t_off                  : 3; /* [2..0] 000：1ms001：0.25s010：0.5s011：1s100：2s101：4s110：4s111：长暗 */
        unsigned int    reserved_1                 : 1; /* [3..3] 保留 */
        unsigned int    dr1_t_on                   : 3; /* [6..4] 000：1ms001：0.25s010：0.5s011：1s100：2s101：4s110：4s111：长亮 */
        unsigned int    reserved_0                 : 1; /* [7..7] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_DR1_TIM_CONF0_T;    /* DR1常亮常暗时间配置寄存器。 */

typedef union
{
    struct
    {
        unsigned int    dr1_t_rise                 : 3; /* [2..0] 000：0s001：0.25s010：0.5s011：1s100：2s101：2.5s110：3s111：4s */
        unsigned int    reserved_1                 : 1; /* [3..3]  */
        unsigned int    dr1_t_fall                 : 3; /* [6..4] 000：0s001：0.25s010：0.5s011：1s100：2s101：2.5s110：3s111：4s */
        unsigned int    reserved_0                 : 1; /* [7..7] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_DR1_TIM_CONF1_T;    /* DR1渐亮渐暗时间配置寄存器。 */

typedef union
{
    struct
    {
        unsigned int    iset_dr1                   : 3; /* [2..0] DR1电流调节。                                    000: 3  mA；001: 6  mA；010: 9  mA；011: 12 mA；100: 15 mA；101: 18 mA；110: 21 mA；111: 24 mA； */
        unsigned int    reserved                   : 5; /* [7..3] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_DR1_ISET_T;    /* DR1电流选择寄存器。 */

typedef union
{
    struct
    {
        unsigned int    dr2_t_off                  : 3; /* [2..0] 000：1ms001：0.25s010：0.5s011：1s100：2s101：4s110：4s111：长暗 */
        unsigned int    reserved_1                 : 1; /* [3..3] 保留 */
        unsigned int    dr2_t_on                   : 3; /* [6..4] 000：1ms001：0.25s010：0.5s011：1s100：2s101：4s110：4s111：长亮 */
        unsigned int    reserved_0                 : 1; /* [7..7] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_DR2_TIM_CONF0_T;    /* DR2常亮常暗时间配置寄存器。 */

typedef union
{
    struct
    {
        unsigned int    dr2_t_rise                 : 3; /* [2..0] 000：0s001：0.25s010：0.5s011：1s100：2s101：2.5s110：3s111：4s */
        unsigned int    reserved_1                 : 1; /* [3..3]  */
        unsigned int    dr2_t_fall                 : 3; /* [6..4] 000：0s001：0.25s010：0.5s011：1s100：2s101：2.5s110：3s111：4s */
        unsigned int    reserved_0                 : 1; /* [7..7] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_DR2_TIM_CONF1_T;    /* DR2渐亮渐暗时间配置寄存器。 */

typedef union
{
    struct
    {
        unsigned int    iset_dr2                   : 3; /* [2..0] DR2电流调节。                                    000: 3  mA；001: 6  mA；010: 9  mA；011: 12 mA；100: 15 mA；101: 18 mA；110: 21 mA；111: 24 mA； */
        unsigned int    reserved                   : 5; /* [7..3] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_DR2_ISET_T;    /* DR2电流选择寄存器。 */

typedef union
{
    struct
    {
        unsigned int    dr3_en                     : 1; /* [0..0] DR3使能。0：不使能；1：使能。 */
        unsigned int    dr4_en                     : 1; /* [1..1] DR4使能。0：不使能；1：使能。 */
        unsigned int    dr5_en                     : 1; /* [2..2] DR5使能。0：不使能；1：使能。 */
        unsigned int    dr_eco_en                  : 1; /* [3..3] 控制呼吸灯常暗节拍时自动关闭使能。0：不使能；1：使能。 */
        unsigned int    reserved                   : 4; /* [7..4] 保留。 */
    } bits;
    unsigned int    u32;
}HI6559_DR_LED_CTRL_T;    /* DR3/4/5控制寄存器。 */

typedef union
{
    struct
    {
        unsigned int    dr3_out_ctrl               : 2; /* [1..0] DR3输出控制。00：DR3的配置项输出到DR3管脚；01：DR3与DR4相或后的配置项输出到DR3管脚；10：DR3与DR4、DR5相或后的配置项输出到DR3管脚；11：DR4与DR5相或后的配置项输出到DR3管脚。 */
        unsigned int    dr4_out_ctrl               : 2; /* [3..2] DR4输出控制。00：DR4的配置项输出到DR4管脚；01：DR3与DR4相或后的配置项输出到DR4管脚；10：DR3与DR4、DR5相或后的配置项输出到DR4管脚；11：DR4与DR5相或后的配置项输出到DR4管脚。 */
        unsigned int    dr5_out_ctrl               : 2; /* [5..4] DR5输出控制。00：DR5的配置项输出到DR5管脚；01：DR3与DR4相或后的配置项输出到DR5管脚；10：DR3与DR4、DR5相或后的配置项输出到DR5管脚；11：DR4与DR5相或后的配置项输出到DR5管脚。 */
        unsigned int    reserved                   : 2; /* [7..6] 保留。 */
    } bits;
    unsigned int    u32;
}HI6559_DR_OUT_CTRL_T;    /* DR3/4/5输出控制寄存器。 */

typedef union
{
    struct
    {
        unsigned int    iset_dr3                   : 3; /* [2..0] R3输出电流调节（mA）：0x0 : 1.0；0x1 : 1.5；0x2 : 2.0；0x3 : 2.5；0x4 : 3.0；0x5 : 3.5；0x6 : 4.0；0x7 : 4.5。 */
        unsigned int    reserved                   : 5; /* [7..3] 保留。 */
    } bits;
    unsigned int    u32;
}HI6559_DR3_ISET_T;    /* DR3电流选择寄存器。 */

typedef union
{
    struct
    {
        unsigned int    dr3_start_delay            : 8; /* [7..0] DR3启动延时范围：0～32768 ms，step：256ms。 */
    } bits;
    unsigned int    u32;
}HI6559_DR3_START_DEL_T;    /* DR3启动延时配置寄存器。 */

typedef union
{
    struct
    {
        unsigned int    dr3_t_off                  : 4; /* [3..0] DR3常暗时间配置。0000：0s；0001：0.5s；0010：1s；0011：2s；0100：4s；0101：6s；0110：8s；0111：12s；1000：14s；1001：16s；1111：长暗；其他值：保持配置发生时刻的LED亮度（电流）不变。 */
        unsigned int    dr3_t_on                   : 4; /* [7..4] DR3常亮时间配置。0000：0s；0001：0.5s；0010：1s；0011：2s；0100：4s；0101：6s；0110：8s；0111：12s；1000：14s；1001：16s；1111：长亮；其他值：保持配置发生时刻的LED亮度（电流）不变。 */
    } bits;
    unsigned int    u32;
}HI6559_DR3_TIM_CONF0_T;    /* DR3常亮常暗时间配置寄存器。 */

typedef union
{
    struct
    {
        unsigned int    dr3_t_rise                 : 3; /* [2..0] DR3渐亮时间配置。000：0s；001：0.25s；010：0.5s；011：1s；100：2s；101：4s；其他值：保持配置发生时刻的LED亮度（电流）不变。 */
        unsigned int    reserved_1                 : 1; /* [3..3] 保留。 */
        unsigned int    dr3_t_fall                 : 3; /* [6..4] DR3渐暗时间配置。000：0s；001：0.25s；010：0.5s；011：1s；100：2s；101：4s；其他值：保持配置发生时刻的LED亮度（电流）不变。 */
        unsigned int    reserved_0                 : 1; /* [7..7] 保留。 */
    } bits;
    unsigned int    u32;
}HI6559_DR3_TIM_CONF1_T;    /* DR3渐亮渐暗时间配置寄存器。 */

typedef union
{
    struct
    {
        unsigned int    iset_dr4                   : 3; /* [2..0] DR4输出电流调节（mA）：0x0 : 1.0；0x1 : 1.5；0x2 : 2.0；0x3 : 2.5；0x4 : 3.0；0x5 : 3.5；0x6 : 4.0；0x7 : 4.5。 */
        unsigned int    reserved                   : 5; /* [7..3] 保留。 */
    } bits;
    unsigned int    u32;
}HI6559_DR4_ISET_T;    /* DR4电流选择寄存器。 */

typedef union
{
    struct
    {
        unsigned int    dr4_start_delay            : 8; /* [7..0] DR4启动延时范围：0～32768 ms，step：256ms。 */
    } bits;
    unsigned int    u32;
}HI6559_DR4_START_DEL_T;    /* DR4启动延时配置寄存器 */

typedef union
{
    struct
    {
        unsigned int    dr4_t_off                  : 4; /* [3..0] DR4常暗时间配置。0000：0s；0001：0.5s；0010：1s；0011：2s；0100：4s；0101：6s；0110：8s；0111：12s；1000：14s；1001：16s；1111：长暗；其他值：保持配置发生时刻的LED亮度（电流）不变。 */
        unsigned int    dr4_t_on                   : 4; /* [7..4] DR4常亮时间配置。0000：0s；0001：0.5s；0010：1s；0011：2s；0100：4s；0101：6s；0110：8s；0111：12s；1000：14s；1001：16s；1111：长亮；其他值：保持配置发生时刻的LED亮度（电流）不变。 */
    } bits;
    unsigned int    u32;
}HI6559_DR4_TIM_CONF0_T;    /* DR4常亮常暗时间配置寄存器。 */

typedef union
{
    struct
    {
        unsigned int    dr4_t_rise                 : 3; /* [2..0] DR4渐亮时间配置。000：0s；001：0.25s；010：0.5s；011：1s；100：2s；101：4s；其他值：保持配置发生时刻的LED亮度（电流）不变。 */
        unsigned int    reserved_1                 : 1; /* [3..3] 保留。 */
        unsigned int    dr4_t_fall                 : 3; /* [6..4] DR4渐暗时间配置。000：0s；001：0.25s；010：0.5s；011：1s；100：2s；101：4s；其他值：保持配置发生时刻的LED亮度（电流）不变。 */
        unsigned int    reserved_0                 : 1; /* [7..7] 保留。 */
    } bits;
    unsigned int    u32;
}HI6559_DR4_TIM_CONF1_T;    /* DR4渐亮渐暗时间配置寄存器。 */

typedef union
{
    struct
    {
        unsigned int    iset_dr5                   : 3; /* [2..0] DR5输出电流调节（mA）：0x0 : 1.0；0x1 : 1.5；0x2 : 2.0；0x3 : 2.5；0x4 : 3.0；0x5 : 3.5；0x6 : 4.0；0x7 : 4.5。 */
        unsigned int    reserved                   : 5; /* [7..3] 保留。 */
    } bits;
    unsigned int    u32;
}HI6559_DR5_ISET_T;    /* DR5电流选择寄存器。 */

typedef union
{
    struct
    {
        unsigned int    dr5_start_delay            : 8; /* [7..0] DR5启动延时范围：0～32768 ms，step：256ms。 */
    } bits;
    unsigned int    u32;
}HI6559_DR5_START_DEL_T;    /* DR5启动延时配置寄存器。 */

typedef union
{
    struct
    {
        unsigned int    dr5_t_off                  : 4; /* [3..0] DR5常暗时间配置。0000：0s；0001：0.5s；0010：1s；0011：2s；0100：4s；0101：6s；0110：8s；0111：12s；1000：14s；1001：16s；1111：长暗；其他值：保持配置发生时刻的LED亮度（电流）不变。 */
        unsigned int    dr5_t_on                   : 4; /* [7..4] DR5常亮时间配置。0000：0s；0001：0.5s；0010：1s；0011：2s；0100：4s；0101：6s；0110：8s；0111：12s；1000：14s；1001：16s；1111：长亮；其他值：保持配置发生时刻的LED亮度（电流）不变。 */
    } bits;
    unsigned int    u32;
}HI6559_DR5_TIM_CONF0_T;    /* DR5常亮常暗时间配置寄存器。 */

typedef union
{
    struct
    {
        unsigned int    dr5_t_rise                 : 3; /* [2..0] DR5渐亮时间配置。000：0s；001：0.25s；010：0.5s；011：1s；100：2s；101：4s；其他值：保持配置发生时刻的LED亮度（电流）不变。 */
        unsigned int    reserved_1                 : 1; /* [3..3] 保留。 */
        unsigned int    dr5_t_fall                 : 3; /* [6..4] DR5渐暗时间配置。000：0s；001：0.25s；010：0.5s；011：1s；100：2s；101：4s；其他值：保持配置发生时刻的LED亮度（电流）不变。 */
        unsigned int    reserved_0                 : 1; /* [7..7] 保留。 */
    } bits;
    unsigned int    u32;
}HI6559_DR5_TIM_CONF1_T;    /* DR5渐亮渐暗时间配置寄存器。 */

typedef union
{
    struct
    {
        unsigned int    otp_pwe_int                : 1; /* [0..0] OTP写控制信号。 */
        unsigned int    otp_pwe_pulse              : 1; /* [1..1] 0：不发起脉冲；1：当otp_write_mask=1时，发起一个310us的高电平脉冲。 */
        unsigned int    otp_write_mask             : 1; /* [2..2] 0:由otp_pwe信号控制OTP的写操作。1:由otp_pwe_pulse信号控制OTP的写操作； */
        unsigned int    otp_por_int                : 1; /* [3..3] OTP读控制信号。0: otp不读；1：自动将32bits otp值读回到非下电寄存器保存。 */
        unsigned int    reserved                   : 4; /* [7..4] 保留。 */
    } bits;
    unsigned int    u32;
}HI6559_OTP_CTRL1_T;    /* OTP控制1信号寄存器。 */

typedef union
{
    struct
    {
        unsigned int    otp_pa                     : 2; /* [1..0] OTP的地址信号。 */
        unsigned int    otp_ptm                    : 2; /* [3..2] 测试模式使能信号。 */
        unsigned int    otp_pprog                  : 1; /* [4..4] 编程模式使能信号。 */
        unsigned int    reserved                   : 3; /* [7..5] 保留。 */
    } bits;
    unsigned int    u32;
}HI6559_OTP_CTRL2_T;    /* OTP控制2信号寄存器。 */

typedef union
{
    struct
    {
        unsigned int    otp_pdin                   : 8; /* [7..0] OTP的输入值寄存器。 */
    } bits;
    unsigned int    u32;
}HI6559_OTP_PDIN_T;    /* OTP写入值寄存器。 */

typedef union
{
    struct
    {
        unsigned int    otp_pdob0                  : 8; /* [7..0] OTP_PDOB[7:0] */
    } bits;
    unsigned int    u32;
}HI6559_OTP_PDOB0_T;    /* OTP读出值0寄存器。 */

typedef union
{
    struct
    {
        unsigned int    otp_pdob1                  : 8; /* [7..0] OTP_PDOB[15:8] */
    } bits;
    unsigned int    u32;
}HI6559_OTP_PDOB1_T;    /* OTP读出值1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    otp_pdob2                  : 8; /* [7..0] OTP_PDOB[23:16] */
    } bits;
    unsigned int    u32;
}HI6559_OTP_PDOB2_T;    /* OTP读出值2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    otp_pdob3                  : 8; /* [7..0] OTP_PDOB[31:24] */
    } bits;
    unsigned int    u32;
}HI6559_OTP_PDOB3_T;    /* OTP读出值3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtccr_a0                   : 8; /* [7..0] 读此寄存器，返回当前RTC值（32bit）中的bit[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6559_RTCCR_A0_T;    /* RTC_A数据0寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtccr_a1                   : 8; /* [7..0] 读此寄存器，返回当前RTC值（32bit）中的bit[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6559_RTCCR_A1_T;    /* RTC_A数据1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtccr_a2                   : 8; /* [7..0] 读此寄存器，返回当前RTC值（32bit）中的bit[23:16]。 */
    } bits;
    unsigned int    u32;
}HI6559_RTCCR_A2_T;    /* RTC_A数据2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtccr_a3                   : 8; /* [7..0] 读此寄存器，返回当前RTC值（32bit）中的bit[31:24]。 */
    } bits;
    unsigned int    u32;
}HI6559_RTCCR_A3_T;    /* RTC_A数据3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtclr_a0                   : 8; /* [7..0] 可设置当前时间值（32bit）中的bit[7:0]。读此寄存器，返回最后一次当前时间设置值的bit[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6559_RTCLR_A0_T;    /* RTCLR_A加载0寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtclr_a1                   : 8; /* [7..0] 可设置当前时间值（32bit）中的bit[15:8]。读此寄存器，返回最后一次当前时间设置值的bit[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6559_RTCLR_A1_T;    /* RTCLR_A加载1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtclr_a2                   : 8; /* [7..0] 可设置当前时间值（32bit）中的bit[23:16]。读此寄存器，返回最后一次当前时间设置值的bit[23:16]。 */
    } bits;
    unsigned int    u32;
}HI6559_RTCLR_A2_T;    /* RTCLR_A加载2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtclr_a3                   : 8; /* [7..0] 可设置当前时间值（32bit）中的bit[31:24]。读此寄存器，返回最后一次当前时间设置值的bit[31:24]。 */
    } bits;
    unsigned int    u32;
}HI6559_RTCLR_A3_T;    /* RTCLR_A加载3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtccr_a                    : 1; /* [0..0] 读RTC控制位0：读RTC当前值，返回值全部为0；1：读RTC当前值，返回值为实际当前值； */
        unsigned int    reserved                   : 7; /* [7..1] 保留。 */
    } bits;
    unsigned int    u32;
}HI6559_RTCCTRL_A_T;    /* RTCCTRL_A控制寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtcmr_a_a0                 : 8; /* [7..0] 可设置闹铃时间值（32bit）中的bit[7:0]。读此寄存器，返回最后一次闹铃时间设置值的bit[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6559_RTCMR_A_A0_T;    /* RTC_A_A比较0寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtcmr_a_a1                 : 8; /* [7..0] 可设置闹铃时间值（32bit）中的bit[15:8]。读此寄存器，返回最后一次闹铃时间设置值的bit[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6559_RTCMR_A_A1_T;    /* RTC_A_A比较1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtcmr_a_a2                 : 8; /* [7..0] 可设置闹铃时间值（32bit）中的bit[23:16]。读此寄存器，返回最后一次闹铃时间设置值的bit[23:16]。 */
    } bits;
    unsigned int    u32;
}HI6559_RTCMR_A_A2_T;    /* RTC_A_A比较2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtcmr_a_a3                 : 8; /* [7..0] 可设置闹铃时间值（32bit）中的bit[31:24]。读此寄存器，返回最后一次闹铃时间设置值的bit[31:24]。 */
    } bits;
    unsigned int    u32;
}HI6559_RTCMR_A_A3_T;    /* RTC_A_A比较3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtcmr_a_b0                 : 8; /* [7..0] 可设置闹铃时间值（32bit）中的bit[7:0]。读此寄存器，返回最后一次闹铃时间设置值的bit[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6559_RTCMR_A_B0_T;    /* RTC_A_B比较0寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtcmr_a_b1                 : 8; /* [7..0] 可设置闹铃时间值（32bit）中的bit[15:8]。读此寄存器，返回最后一次闹铃时间设置值的bit[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6559_RTCMR_A_B1_T;    /* RTC_A_B比较1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtcmr_a_b2                 : 8; /* [7..0] 可设置闹铃时间值（32bit）中的bit[23:16]。读此寄存器，返回最后一次闹铃时间设置值的bit[23:16]。 */
    } bits;
    unsigned int    u32;
}HI6559_RTCMR_A_B2_T;    /* RTC_A_B比较2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtcmr_a_b3                 : 8; /* [7..0] 可设置闹铃时间值（32bit）中的bit[31:24]。读此寄存器，返回最后一次闹铃时间设置值的bit[31:24]。 */
    } bits;
    unsigned int    u32;
}HI6559_RTCMR_A_B3_T;    /* RTC_A_B比较3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtcmr_a_c0                 : 8; /* [7..0] 可设置闹铃时间值（32bit）中的bit[7:0]。读此寄存器，返回最后一次闹铃时间设置值的bit[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6559_RTCMR_A_C0_T;    /* RTC_A_C比较0寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtcmr_a_c1                 : 8; /* [7..0] 可设置闹铃时间值（32bit）中的bit[15:8]。读此寄存器，返回最后一次闹铃时间设置值的bit[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6559_RTCMR_A_C1_T;    /* RTC_A_C比较1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtcmr_a_c2                 : 8; /* [7..0] 可设置闹铃时间值（32bit）中的bit[23:16]。读此寄存器，返回最后一次闹铃时间设置值的bit[23:16]。 */
    } bits;
    unsigned int    u32;
}HI6559_RTCMR_A_C2_T;    /* RTC_A_C比较2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtcmr_a_c3                 : 8; /* [7..0] 可设置闹铃时间值（32bit）中的bit[31:24]。读此寄存器，返回最后一次闹铃时间设置值的bit[31:24]。 */
    } bits;
    unsigned int    u32;
}HI6559_RTCMR_A_C3_T;    /* RTC_A_C比较3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtcmr_a_d0                 : 8; /* [7..0] 可设置闹铃时间值（32bit）中的bit[7:0]。读此寄存器，返回最后一次闹铃时间设置值的bit[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6559_RTCMR_A_D0_T;    /* RTC_A_D比较0寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtcmr_a_d1                 : 8; /* [7..0] 可设置闹铃时间值（32bit）中的bit[15:8]。读此寄存器，返回最后一次闹铃时间设置值的bit[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6559_RTCMR_A_D1_T;    /* RTC_A_D比较1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtcmr_a_d2                 : 8; /* [7..0] 可设置闹铃时间值（32bit）中的bit[23:16]。读此寄存器，返回最后一次闹铃时间设置值的bit[23:16]。 */
    } bits;
    unsigned int    u32;
}HI6559_RTCMR_A_D2_T;    /* RTC_A_D比较2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtcmr_a_d3                 : 8; /* [7..0] 可设置闹铃时间值（32bit）中的bit[31:24]。读此寄存器，返回最后一次闹铃时间设置值的bit[31:24]。 */
    } bits;
    unsigned int    u32;
}HI6559_RTCMR_A_D3_T;    /* RTC_A_D比较3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtcmr_a_e0                 : 8; /* [7..0] 可设置闹铃时间值（32bit）中的bit[7:0]。读此寄存器，返回最后一次闹铃时间设置值的bit[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6559_RTCMR_A_E0_T;    /* RTC_A_E比较0寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtcmr_a_e1                 : 8; /* [7..0] 可设置闹铃时间值（32bit）中的bit[15:8]。读此寄存器，返回最后一次闹铃时间设置值的bit[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6559_RTCMR_A_E1_T;    /* RTC_A_E比较1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtcmr_a_e2                 : 8; /* [7..0] 可设置闹铃时间值（32bit）中的bit[23:16]。读此寄存器，返回最后一次闹铃时间设置值的bit[23:16]。 */
    } bits;
    unsigned int    u32;
}HI6559_RTCMR_A_E2_T;    /* RTC_A_E比较2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtcmr_a_e3                 : 8; /* [7..0] 可设置闹铃时间值（32bit）中的bit[31:24]。读此寄存器，返回最后一次闹铃时间设置值的bit[31:24]。 */
    } bits;
    unsigned int    u32;
}HI6559_RTCMR_A_E3_T;    /* RTC_A_E比较3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtcmr_a_f0                 : 8; /* [7..0] 可设置闹铃时间值（32bit）中的bit[7:0]。读此寄存器，返回最后一次闹铃时间设置值的bit[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6559_RTCMR_A_F0_T;    /* RTC_A_F比较0寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtcmr_a_f1                 : 8; /* [7..0] 可设置闹铃时间值（32bit）中的bit[15:8]。读此寄存器，返回最后一次闹铃时间设置值的bit[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6559_RTCMR_A_F1_T;    /* RTC_A_F比较1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtcmr_a_f2                 : 8; /* [7..0] 可设置闹铃时间值（32bit）中的bit[23:16]。读此寄存器，返回最后一次闹铃时间设置值的bit[23:16]。 */
    } bits;
    unsigned int    u32;
}HI6559_RTCMR_A_F2_T;    /* RTC_A_F比较2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtcmr_a_f3                 : 8; /* [7..0] 可设置闹铃时间值（32bit）中的bit[31:24]。读此寄存器，返回最后一次闹铃时间设置值的bit[31:24]。 */
    } bits;
    unsigned int    u32;
}HI6559_RTCMR_A_F3_T;    /* RTC_A_F比较3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtccr_na0                  : 8; /* [7..0] 读此寄存器，返回当前RTC值（32bit）中的bit[7:0]。注： 改组RTC仅做计数用，无ALARM功能，不对用户开放。 */
    } bits;
    unsigned int    u32;
}HI6559_RTCCR_NA0_T;    /* RTC_NA数据0寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtccr_na1                  : 8; /* [7..0] 读此寄存器，返回当前RTC值（32bit）中的bit[15:8]。注： 改组RTC仅做计数用，无ALARM功能，不对用户开放。 */
    } bits;
    unsigned int    u32;
}HI6559_RTCCR_NA1_T;    /* RTC_NA数据1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtccr_na2                  : 8; /* [7..0] 读此寄存器，返回当前RTC值（32bit）中的bit[23:16]。注： 改组RTC仅做计数用，无ALARM功能，不对用户开放。 */
    } bits;
    unsigned int    u32;
}HI6559_RTCCR_NA2_T;    /* RTC_NA数据2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtccr_na3                  : 8; /* [7..0] 读此寄存器，返回当前RTC值（32bit）中的bit[31:24]。注： 改组RTC仅做计数用，无ALARM功能，不对用户开放。 */
    } bits;
    unsigned int    u32;
}HI6559_RTCCR_NA3_T;    /* RTC_NA数据3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtclr_na0                  : 8; /* [7..0] 可设置当前时间值（32bit）中的bit[7:0]。读此寄存器，返回最后一次当前时间设置值的bit[7:0]。注： 改组RTC仅做计数用，无ALARM功能，不对用户开放。 */
    } bits;
    unsigned int    u32;
}HI6559_RTCLR_NA0_T;    /* RTCLR_NA加载0寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtclr_na1                  : 8; /* [7..0] 可设置当前时间值（32bit）中的bit[15:8]。读此寄存器，返回最后一次当前时间设置值的bit[15:8]。注： 改组RTC仅做计数用，无ALARM功能，不对用户开放。 */
    } bits;
    unsigned int    u32;
}HI6559_RTCLR_NA1_T;    /* RTCLR_NA加载1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtclr_na2                  : 8; /* [7..0] 可设置当前时间值（32bit）中的bit[23:16]。读此寄存器，返回最后一次当前时间设置值的bit[23:16]。注： 改组RTC仅做计数用，无ALARM功能，不对用户开放。 */
    } bits;
    unsigned int    u32;
}HI6559_RTCLR_NA2_T;    /* RTCLR_NA加载2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtclr_na3                  : 8; /* [7..0] 可设置当前时间值（32bit）中的bit[31:24]。读此寄存器，返回最后一次当前时间设置值的bit[31:24]。注： 改组RTC仅做计数用，无ALARM功能，不对用户开放。 */
    } bits;
    unsigned int    u32;
}HI6559_RTCLR_NA3_T;    /* RTCLR_NA加载3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtccr_na                   : 1; /* [0..0] 读RTC控制位0：读RTC当前值，返回值全部为0；1：读RTC当前值，返回值为实际当前值；注： 改组RTC仅做计数用，无ALARM功能，不对用户开放。 */
        unsigned int    reserved                   : 7; /* [7..1] 保留。 */
    } bits;
    unsigned int    u32;
}HI6559_RTCCTRL_NA_T;    /* RTCCTRL_NA控制寄存器。 */

typedef union
{
    struct
    {
        unsigned int    version                    : 8; /* [7..0] 版本寄存器，表示版本为V100。 */
    } bits;
    unsigned int    u32;
}HI6559_VERSION_T;    /* 版本寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved0                  : 8; /* [7..0] reserved0<7>HKADC强制复位reserved0<6:0>基准修调寄存器 */
    } bits;
    unsigned int    u32;
}HI6559_RESERVED0_T;    /* 保留0寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved1                  : 8; /* [7..0] reserved1<7:0>:测试用 及备用未定； */
    } bits;
    unsigned int    u32;
}HI6559_RESERVED1_T;    /* 保留1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved2                  : 8; /* [7..0] 测试用 及备用未定。 */
    } bits;
    unsigned int    u32;
}HI6559_RESERVED2_T;    /* 保留2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved3                  : 8; /* [7..0] 测试用 及备用未定。 */
    } bits;
    unsigned int    u32;
}HI6559_RESERVED3_T;    /* 保留3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved4                  : 8; /* [7..0] 测试用 及备用未定。 */
    } bits;
    unsigned int    u32;
}HI6559_RESERVED4_T;    /* 保留4寄存器。 */

typedef union
{
    struct
    {
        unsigned int    htol_mode                  : 4; /* [3..0] 0101：不进入老化模式 ；1010：进入老化测试模式 ；其他：无效 。注：老化测试专用。 */
        unsigned int    reserved                   : 4; /* [7..4] 保留。 */
    } bits;
    unsigned int    u32;
}HI6559_HTOL_MODE_T;    /* 老化测试模式配置寄存器。 */

typedef union
{
    struct
    {
        unsigned int    dac_on_sel                 : 1; /* [0..0] 1: DAC的开关由DAC_SSI空间的dac_reg_onoff控制；0：保持DAC关闭。 */
        unsigned int    aux_ibias_cfg              : 2; /* [2..1] AUXDAC偏置电流调节，11最大，00最小 */
        unsigned int    reserved                   : 5; /* [7..3] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_DAC_CTRL_T;    /* DAC控制寄存器 */

typedef union
{
    struct
    {
        unsigned int    pdns_sel                   : 3; /* [2..0] 长按键Ns关机可配寄存器：000:6s;001:7s;010:8s;011:9s;100:10s;101:11s;110:8s;111:9s。 */
        unsigned int    pdns_mk                    : 1; /* [3..3] 长按键关机屏蔽寄存器：0：不屏蔽长按键关机；1：屏蔽长按键关机；注：在正常工作中，一直按下pwron_n ，此时才可以通过SSI将长按键关机屏蔽位置一，当pwron_n松开20ms后该位自动清除。 */
        unsigned int    vsys_uv_adj                : 1; /* [4..4] 0：vsys低压检测点设为2.85V；1：vsys低压检测点设为3.0V； */
        unsigned int    nopwr_rc_off               : 1; /* [5..5] 关闭内部256K RC时钟:0:使能;1:关闭。 */
        unsigned int    pdns_restar_mk             : 1; /* [6..6] 长按键关机后重启屏蔽寄存器：0：不屏蔽长按键关机后N+5S重启；1：屏蔽长按键关机后N+5S重启； */
        unsigned int    reserved                   : 1; /* [7..7] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_NP_REG_ADJ_T;    /* 非下电调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    nopwr_vcoinsl              : 2; /* [1..0] 备用电池充电电压选择：00：2.5V；01：3.0V；1X：3.3V。 */
        unsigned int    nopwr_en_backup_chg        : 1; /* [2..2] 备用电池充电使能：0：不使能；1：使能。 */
        unsigned int    reserved                   : 5; /* [7..3] 保留 */
    } bits;
    unsigned int    u32;
}HI6559_NP_REG_CHG_T;    /* 备用电池非下电调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    no_pwr_resved1             : 8; /* [7..0] 测试用 及备用未定 */
    } bits;
    unsigned int    u32;
}HI6559_NP_RSVED1_T;    /* 非下电保留1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    no_pwr_resved2             : 8; /* [7..0] no_pwr_resved2<7:4>: 测试用 及备用未定no_pwr_resved2<3:0>: LVS offset修调寄存器； */
    } bits;
    unsigned int    u32;
}HI6559_NP_RSVED2_T;    /* 非下电保留2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    no_pwr_resved3             : 8; /* [7..0] 产品预留：测试用及备用，不对PMU内部工作状态产生任何作用，仅用于记录信息。 */
    } bits;
    unsigned int    u32;
}HI6559_NP_RSVED3_T;    /* 非下电保留3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    no_pwr_resved4             : 8; /* [7..0] 产品预留：测试用及备用，不对PMU内部工作状态产生任何作用，仅用于记录信息。 */
    } bits;
    unsigned int    u32;
}HI6559_NP_RSVED4_T;    /* 非下电保留4寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtc_clk_step_adj1          : 8; /* [7..0] 计时精度可以调节，调节范围+/-192ppm（63拍时钟），调节步进3.05ppm（1拍时钟），默认为0。 */
    } bits;
    unsigned int    u32;
}HI6559_RTC_ADJ1_T;    /* rtc_adj[7:0]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtc_clk_step_adj2          : 8; /* [7..0] 计时精度可以调节，调节范围+/-192ppm（63拍时钟），调节步进3.05ppm（1拍时钟），默认为0。 */
    } bits;
    unsigned int    u32;
}HI6559_RTC_ADJ2_T;    /* rtc_adj[15:8]寄存器。 */

#endif // __HI_BBSTAR_H__

