/******************************************************************************/
/*  Copyright (C), 2007-2013, Hisilicon Technologies Co., Ltd. */
/******************************************************************************/
/* File name     : hi_hi6551.h */
/* Version       : 2.0 */
/* Author        : x00195528*/
/* Created       : 2013-06-26*/
/* Last Modified : */
/* Description   :  The C union definition file for the module hi6551*/
/* Function List : */
/* History       : */
/* 1 Date        : */
/* Author        : x00195528*/
/* Modification  : Create file */
/******************************************************************************/

#ifndef __HI6551_H__
#define __HI6551_H__

/*
 * Project: hi
 * Module : hi6551
 */


/********************************************************************************/
/*    hi6551 寄存器偏移定义（项目名_模块名_寄存器名_OFFSET)        */
/********************************************************************************/
#define    HI6551_STATUS1_OFFSET                          (0x1) /* 状态标志1寄存器。 */
#define    HI6551_STATUS2_OFFSET                          (0x2) /* 状态标志2寄存器。 */
#define    HI6551_IRQ1_OFFSET                             (0x3) /* 中断1寄存器。 */
#define    HI6551_IRQ2_OFFSET                             (0x4) /* 中断2寄存器。 */
#define    HI6551_IRQ3_OFFSET                             (0x5) /* 中断3寄存器。 */
#define    HI6551_IRQ4_OFFSET                             (0x6) /* 中断4寄存器。 */
#define    HI6551_COUL_IRQ_OFFSET                         (0x7) /* 库仑计中断寄存器。 */
#define    HI6551_IRQ1_MASK_OFFSET                        (0x8) /* 中断屏蔽1寄存器。 */
#define    HI6551_IRQ2_MASK_OFFSET                        (0x9) /* 中断屏蔽2寄存器。 */
#define    HI6551_IRQ3_MASK_OFFSET                        (0xA) /* 中断屏蔽3寄存器。 */
#define    HI6551_IRQ4_MASK_OFFSET                        (0xB) /* 中断屏蔽4寄存器。 */
#define    HI6551_COUL_IRQ_MASK_OFFSET                    (0xC) /* 库仑计中断寄存器。 */
#define    HI6551_SCP_RECORD1_OFFSET                      (0xD) /* 短路保护记录1寄存器。 */
#define    HI6551_OCP_RECORD1_OFFSET                      (0xE) /* 过流保护记录1寄存器。 */
#define    HI6551_OCP_RECORD2_OFFSET                      (0xF) /* 过流保护记录2寄存器。 */
#define    HI6551_OCP_RECORD3_OFFSET                      (0x10) /* 过流保护记录3寄存器。 */
#define    HI6551_OCP_RECORD4_OFFSET                      (0x11) /* 过流保护记录4寄存器。 */
#define    HI6551_OCP_RECORD5_OFFSET                      (0x12) /* 过流保护记录5寄存器。 */
#define    HI6551_OCP_RECORD6_OFFSET                      (0x13) /* 过流保护记录6寄存器。 */
#define    HI6551_OCP_RECORD7_OFFSET                      (0x14) /* 过流保护记录7寄存器。 */
#define    HI6551_NP_IRQ1_RECORD_OFFSET                   (0x15) /* 中断1非下电记录寄存器。 */
#define    HI6551_NP_IRQ2_RECORD_OFFSET                   (0x16) /* 中断2非下电记录寄存器。 */
#define    HI6551_NP_IRQ3_RECORD_OFFSET                   (0x17) /* 中断3非下电记录寄存器。 */
#define    HI6551_NP_SCP_RECORD1_OFFSET                   (0x18) /* 短路保护非下电记录1寄存器。 */
#define    HI6551_NP_OCP_RECORD1_OFFSET                   (0x19) /* 过流保护非下电记录1寄存器。 */
#define    HI6551_NP_OCP_RECORD2_OFFSET                   (0x1A) /* 过流保护非下电记录2寄存器。 */
#define    HI6551_NP_OCP_RECORD3_OFFSET                   (0x1B) /* 过流保护非下电记录3寄存器。 */
#define    HI6551_NP_OCP_RECORD4_OFFSET                   (0x1C) /* 过流保护非下电记录4寄存器。 */
#define    HI6551_NP_OCP_RECORD5_OFFSET                   (0x1D) /* 过流保护非下电记录5寄存器。 */
#define    HI6551_NP_OCP_RECORD6_OFFSET                   (0x1E) /* 过流保护非下电记录6寄存器。 */
#define    HI6551_NP_OCP_RECORD7_OFFSET                   (0x1F) /* 过流保护非下电记录7寄存器。 */
#define    HI6551_OCP_DEB_OFFSET                          (0x20) /* OCP去抖动调节寄存器。 */
#define    HI6551_ONOFF8_OFFSET                           (0x21) /* 开关控制8寄存器。 */
#define    HI6551_OCP_SCP_MOD_CTRL1_OFFSET                (0x22) /* BUCK0-3过流和短路保护模式控制寄存器。 */
#define    HI6551_OCP_SCP_MOD_CTRL2_OFFSET                (0x23) /* BUCK4-7过流和短路保护模式控制寄存器。 */
#define    HI6551_OCP_MOD_CTRL3_OFFSET                    (0x24) /* LDO1-4过流保护模式控制寄存器。 */
#define    HI6551_OCP_MOD_CTRL4_OFFSET                    (0x25) /* LDO5-8过流保护模式控制寄存器。 */
#define    HI6551_OCP_MOD_CTRL5_OFFSET                    (0x26) /* LDO9-12过流保护模式控制寄存器。 */
#define    HI6551_OCP_MOD_CTRL6_OFFSET                    (0x27) /* LDO13-16过流保护模式控制寄存器。 */
#define    HI6551_OCP_MOD_CTRL7_OFFSET                    (0x28) /* LDO17-20过流保护模式控制寄存器。 */
#define    HI6551_OCP_MOD_CTRL8_OFFSET                    (0x29) /* LDO21-ldo23过流保护模式控制寄存器。 */
#define    HI6551_OCP_MOD_CTRL9_OFFSET                    (0x2A) /* LVS2-5过流保护模式控制寄存器。 */
#define    HI6551_OCP_MOD_CTRL10_OFFSET                   (0x2B) /* LVS2-5过流保护模式控制寄存器。 */
#define    HI6551_OCP_MOD_CTRL11_OFFSET                   (0x2C) /* LVS9-10,BOOST过流保护模式控制寄存器。 */
#define    HI6551_ENABLE1_OFFSET                          (0x2D) /* 电源模块使能1寄存器。 */
#define    HI6551_DISABLE1_OFFSET                         (0x2E) /* 电源模块关闭1寄存器。 */
#define    HI6551_ONOFF_STATUS1_OFFSET                    (0x2F) /* 电源模块开关状态1寄存器。 */
#define    HI6551_ENABLE2_OFFSET                          (0x30) /* 电源模块使能2寄存器。 */
#define    HI6551_DISABLE2_OFFSET                         (0x31) /* 电源模块关闭2寄存器。 */
#define    HI6551_ONOFF_STATUS2_OFFSET                    (0x32) /* 电源模块开关状态2寄存器。 */
#define    HI6551_ENABLE3_OFFSET                          (0x33) /* 电源模块使能3寄存器。 */
#define    HI6551_DISABLE3_OFFSET                         (0x34) /* 电源模块关闭3寄存器。 */
#define    HI6551_ONOFF_STATUS3_OFFSET                    (0x35) /* 电源模块开关状态3寄存器。 */
#define    HI6551_ENABLE4_OFFSET                          (0x36) /* 电源模块使能4寄存器。 */
#define    HI6551_DISABLE4_OFFSET                         (0x37) /* 电源模块关闭4寄存器。 */
#define    HI6551_ONOFF_STATUS4_OFFSET                    (0x38) /* 电源模块开关状态4寄存器。 */
#define    HI6551_ENABLE5_OFFSET                          (0x39) /* 电源模块使能5寄存器。 */
#define    HI6551_DISABLE5_OFFSET                         (0x3A) /* 电源模块关闭5寄存器。 */
#define    HI6551_ONOFF_STATUS5_OFFSET                    (0x3B) /* 电源模块开关状态5寄存器。 */
#define    HI6551_ENABLE6_OFFSET                          (0x3C) /* 电源模块使能6寄存器。 */
#define    HI6551_DISABLE6_OFFSET                         (0x3D) /* 电源模块关闭6寄存器。 */
#define    HI6551_ONOFF_STATUS6_OFFSET                    (0x3E) /* 电源模块开关状态6寄存器。 */
#define    HI6551_ENABLE7_OFFSET                          (0x3F) /* 电源模块使能7寄存器。 */
#define    HI6551_SIMCARD_EN_OFFSET                       (0x40) /* SIM卡使能寄存器。 */
#define    HI6551_SIM0_CFG_OFFSET                         (0x41) /* SIM卡0配置寄存器。 */
#define    HI6551_SIM1_CFG_OFFSET                         (0x42) /* SIM卡1配置寄存器。 */
#define    HI6551_SIM_DEB_OFFSET                          (0x43) /* SIM卡中断去抖寄调节寄存器。 */
#define    HI6551_ECO_MOD_CFG1_OFFSET                     (0x44) /* 电源模块是否跟随PMU进出ECO模式配置1寄存器。 */
#define    HI6551_ECO_MOD_CFG2_OFFSET                     (0x45) /* 电源模块是否跟随PMU进出ECO模式配置2寄存器。 */
#define    HI6551_ECO_MOD_CFG3_OFFSET                     (0x46) /* 电源是否强制随PMU进出ECO模式配置3寄存器。 */
#define    HI6551_ECO_MOD_CFG4_OFFSET                     (0x47) /* 电源是否强制随PMU进出ECO模式配置4寄存器。 */
#define    HI6551_ENB3_ECO_MARK_OFFSET                    (0x48) /* 与ENB3硬线和ECO功能相关的屏蔽信号寄存器。 */
#define    HI6551_BUCK01_REG1_ADJ_OFFSET                  (0x49) /* BUCK0和BUCK1调节1寄存器。 */
#define    HI6551_BUCK01_REG2_ADJ_OFFSET                  (0x4A) /* BUCK0和BUCK1调节2寄存器。 */
#define    HI6551_BUCK01_REG3_ADJ_OFFSET                  (0x4B) /* BUCK0和BUCK1调节3寄存器。 */
#define    HI6551_BUCK01_REG4_ADJ_OFFSET                  (0x4C) /* BUCK0和BUCK1调节4寄存器。 */
#define    HI6551_BUCK01_REG5_ADJ_OFFSET                  (0x4D) /* BUCK0和BUCK1调节5寄存器。 */
#define    HI6551_BUCK01_REG6_ADJ_OFFSET                  (0x4E) /* BUCK0和BUCK1调节6寄存器。 */
#define    HI6551_BUCK01_REG7_ADJ_OFFSET                  (0x4F) /* BUCK0和BUCK1调节7寄存器。 */
#define    HI6551_BUCK01_REG8_ADJ_OFFSET                  (0x50) /* BUCK0和BUCK1调节8寄存器。 */
#define    HI6551_BUCK01_REG9_ADJ_OFFSET                  (0x51) /* BUCK0和BUCK1调节9寄存器。 */
#define    HI6551_BUCK01_REG10_ADJ_OFFSET                 (0x52) /* BUCK0和BUCK1调节10寄存器。 */
#define    HI6551_BUCK01_REG11_ADJ_OFFSET                 (0x53) /* BUCK0和BUCK1调节11寄存器。 */
#define    HI6551_BUCK01_REG12_ADJ_OFFSET                 (0x54) /* BUCK0和BUCK1调节12寄存器。 */
#define    HI6551_BUCK01_REG13_ADJ_OFFSET                 (0x55) /* BUCK0和BUCK1调节13寄存器。 */
#define    HI6551_BUCK01_REG14_ADJ_OFFSET                 (0x56) /* BUCK0和BUCK1调节14寄存器。 */
#define    HI6551_BUCK01_REG15_ADJ_OFFSET                 (0x57) /* BUCK0和BUCK1调节15寄存器。 */
#define    HI6551_BUCK01_REG16_ADJ_OFFSET                 (0x58) /* BUCK0和BUCK1调节16寄存器。 */
#define    HI6551_BUCK01_REG17_ADJ_OFFSET                 (0x59) /* BUCK0和BUCK1调节17寄存器。 */
#define    HI6551_BUCK2_REG1_ADJ_OFFSET                   (0x5A) /* BUCK2调节1寄存器。 */
#define    HI6551_BUCK2_REG2_ADJ_OFFSET                   (0x5B) /* BUCK2调节2寄存器。 */
#define    HI6551_BUCK2_REG3_ADJ_OFFSET                   (0x5C) /* BUCK2调节3寄存器。 */
#define    HI6551_BUCK2_REG4_ADJ_OFFSET                   (0x5D) /* BUCK2调节4寄存器。 */
#define    HI6551_BUCK2_REG5_ADJ_OFFSET                   (0x5E) /* BUCK2调节5寄存器。 */
#define    HI6551_BUCK2_REG6_ADJ_OFFSET                   (0x5F) /* BUCK2调节6寄存器。 */
#define    HI6551_BUCK2_REG7_ADJ_OFFSET                   (0x60) /* BUCK2调节7寄存器。 */
#define    HI6551_BUCK2_REG8_ADJ_OFFSET                   (0x61) /* BUCK2调节8寄存器。 */
#define    HI6551_BUCK2_REG9_ADJ_OFFSET                   (0x62) /* BUCK2调节9寄存器。 */
#define    HI6551_BUCK2_REG10_ADJ_OFFSET                  (0x63) /* BUCK2调节10寄存器。 */
#define    HI6551_BUCK2_REG11_ADJ_OFFSET                  (0x64) /* BUCK2调节11寄存器。 */
#define    HI6551_BUCK2_REG12_ADJ_OFFSET                  (0x65) /* BUCK2调节12寄存器。 */
#define    HI6551_BUCK3_REG1_ADJ_OFFSET                   (0x66) /* BUCK3调节1寄存器。 */
#define    HI6551_BUCK3_REG2_ADJ_OFFSET                   (0x67) /* BUCK3调节2寄存器。 */
#define    HI6551_BUCK3_REG3_ADJ_OFFSET                   (0x68) /* BUCK3调节3寄存器。 */
#define    HI6551_BUCK3_REG4_ADJ_OFFSET                   (0x69) /* BUCK3调节4寄存器。 */
#define    HI6551_BUCK3_REG5_ADJ_OFFSET                   (0x6A) /* BUCK3调节5寄存器。 */
#define    HI6551_BUCK3_REG6_ADJ_OFFSET                   (0x6B) /* BUCK3调节6寄存器。 */
#define    HI6551_BUCK3_REG7_ADJ_OFFSET                   (0x6C) /* BUCK3调节7寄存器。 */
#define    HI6551_BUCK3_REG8_ADJ_OFFSET                   (0x6D) /* BUCK3调节8寄存器。 */
#define    HI6551_BUCK3_REG9_ADJ_OFFSET                   (0x6E) /* BUCK3调节9寄存器。 */
#define    HI6551_BUCK3_REG10_ADJ_OFFSET                  (0x6F) /* BUCK3调节10寄存器。 */
#define    HI6551_BUCK3_REG11_ADJ_OFFSET                  (0x70) /* BUCK3调节11寄存器。 */
#define    HI6551_BUCK3_REG12_ADJ_OFFSET                  (0x71) /* BUCK3调节12寄存器。 */
#define    HI6551_BUCK4_REG1_ADJ_OFFSET                   (0x72) /* BUCK4调节1寄存器。 */
#define    HI6551_BUCK4_REG2_ADJ_OFFSET                   (0x73) /* BUCK4调节2寄存器。 */
#define    HI6551_BUCK4_REG3_ADJ_OFFSET                   (0x74) /* BUCK4调节3寄存器。 */
#define    HI6551_BUCK4_REG4_ADJ_OFFSET                   (0x75) /* BUCK4调节4寄存器。 */
#define    HI6551_BUCK4_REG5_ADJ_OFFSET                   (0x76) /* BUCK4调节5寄存器。 */
#define    HI6551_BUCK4_REG6_ADJ_OFFSET                   (0x77) /* BUCK4调节6寄存器。 */
#define    HI6551_BUCK4_REG7_ADJ_OFFSET                   (0x78) /* BUCK4调节7寄存器。 */
#define    HI6551_BUCK4_REG8_ADJ_OFFSET                   (0x79) /* BUCK4调节8寄存器。 */
#define    HI6551_BUCK4_REG9_ADJ_OFFSET                   (0x7A) /* BUCK4调节9寄存器。 */
#define    HI6551_BUCK4_REG10_ADJ_OFFSET                  (0x7B) /* BUCK4调节10寄存器。 */
#define    HI6551_BUCK4_REG12_ADJ_OFFSET                  (0x7C) /* BUCK4调节12寄存器。 */
#define    HI6551_BUCK5_REG1_ADJ_OFFSET                   (0x7D) /* BUCK5调节1寄存器。 */
#define    HI6551_BUCK5_REG2_ADJ_OFFSET                   (0x7E) /* BUCK5调节2寄存器。 */
#define    HI6551_BUCK5_REG3_ADJ_OFFSET                   (0x7F) /* BUCK5调节3寄存器。 */
#define    HI6551_BUCK5_REG4_ADJ_OFFSET                   (0x80) /* BUCK5调节4寄存器。 */
#define    HI6551_BUCK5_REG5_ADJ_OFFSET                   (0x81) /* BUCK5调节5寄存器。 */
#define    HI6551_BUCK5_REG6_ADJ_OFFSET                   (0x82) /* BUCK5调节6寄存器。 */
#define    HI6551_BUCK5_REG7_ADJ_OFFSET                   (0x83) /* BUCK5调节7寄存器。 */
#define    HI6551_BUCK5_REG8_ADJ_OFFSET                   (0x84) /* BUCK5调节8寄存器。 */
#define    HI6551_BUCK5_REG9_ADJ_OFFSET                   (0x85) /* BUCK5调节9寄存器。 */
#define    HI6551_BUCK5_REG10_ADJ_OFFSET                  (0x86) /* BUCK5调节10寄存器。 */
#define    HI6551_BUCK5_REG12_ADJ_OFFSET                  (0x87) /* BUCK5调节12寄存器。 */
#define    HI6551_BUCK5_REG13_ADJ_OFFSET                  (0x88) /* BUCK5调节13寄存器。 */
#define    HI6551_BUCK5_REG14_ADJ_OFFSET                  (0x89) /* BUCK5调节14寄存器。 */
#define    HI6551_BUCK6_REG1_ADJ_OFFSET                   (0x8A) /* BUCK6调节1寄存器。 */
#define    HI6551_BUCK6_REG2_ADJ_OFFSET                   (0x8B) /* BUCK6调节2寄存器。 */
#define    HI6551_BUCK6_REG3_ADJ_OFFSET                   (0x8C) /* BUCK6调节3寄存器。 */
#define    HI6551_BUCK6_REG4_ADJ_OFFSET                   (0x8D) /* BUCK6调节4寄存器。 */
#define    HI6551_BUCK6_REG5_ADJ_OFFSET                   (0x8E) /* BUCK6调节5寄存器。 */
#define    HI6551_BUCK6_REG6_ADJ_OFFSET                   (0x8F) /* BUCK6调节6寄存器。 */
#define    HI6551_BUCK6_REG7_ADJ_OFFSET                   (0x90) /* BUCK6调节7寄存器。 */
#define    HI6551_BUCK6_REG8_ADJ_OFFSET                   (0x91) /* BUCK6调节8寄存器。 */
#define    HI6551_BUCK6_REG9_ADJ_OFFSET                   (0x92) /* BUCK6调节9寄存器。 */
#define    HI6551_BUCK6_REG10_ADJ_OFFSET                  (0x93) /* BUCK6调节10寄存器。 */
#define    HI6551_BUCK6_REG11_ADJ_OFFSET                  (0x94) /* BUCK6调节11寄存器。 */
#define    HI6551_BUCK6_REG12_ADJ_OFFSET                  (0x95) /* BUCK6调节12寄存器。 */
#define    HI6551_CHG_PUMP2_ADJ_OFFSET                    (0x96) /* CHG_PUMP2调节寄存器。 */
#define    HI6551_VSET_BUCK01_ADJ_OFFSET                  (0x97) /* BUCK01电压调节寄存器。 */
#define    HI6551_VSET_BUCK2_ADJ_OFFSET                   (0x98) /* BUCK2电压调节寄存器。 */
#define    HI6551_VSET_BUCK3_ADJ_OFFSET                   (0x99) /* BUCK3电压调节寄存器。 */
#define    HI6551_VSET_BUCK4_ADJ_OFFSET                   (0x9A) /* BUCK3电压调节寄存器。 */
#define    HI6551_VSET_BUCK5_ADJ_OFFSET                   (0x9B) /* BUCK5电压调节寄存器。 */
#define    HI6551_VSET_BUCK6_ADJ_OFFSET                   (0x9C) /* BUCK6电压调节寄存器。 */
#define    HI6551_LDO1_REG_ADJ_OFFSET                     (0x9D) /* LDO1调节寄存器。 */
#define    HI6551_LDO2_REG_ADJ_OFFSET                     (0x9E) /* LDO2调节寄存器。 */
#define    HI6551_LDO3_REG_ADJ_OFFSET                     (0x9F) /* LDO3调节寄存器。 */
#define    HI6551_LDO4_REG_ADJ_OFFSET                     (0xA0) /* LDO4调节寄存器。 */
#define    HI6551_LDO5_REG_ADJ_OFFSET                     (0xA1) /* LDO5调节寄存器。 */
#define    HI6551_LDO6_REG_ADJ_OFFSET                     (0xA2) /* LDO6调节寄存器。 */
#define    HI6551_LDO7_REG_ADJ_OFFSET                     (0xA3) /* LDO7调节寄存器。 */
#define    HI6551_LDO8_REG_ADJ_OFFSET                     (0xA4) /* LDO8调节寄存器。 */
#define    HI6551_LDO9_REG_ADJ_OFFSET                     (0xA5) /* LDO9调节寄存器。 */
#define    HI6551_LDO10_REG_ADJ_OFFSET                    (0xA6) /* LDO10调节寄存器。 */
#define    HI6551_LDO11_REG_ADJ_OFFSET                    (0xA7) /* LDO11调节寄存器。 */
#define    HI6551_LDO12_REG_ADJ_OFFSET                    (0xA8) /* LDO12调节寄存器。 */
#define    HI6551_LDO13_REG_ADJ_OFFSET                    (0xA9) /* LDO13调节寄存器。 */
#define    HI6551_LDO14_REG_ADJ_OFFSET                    (0xAA) /* LDO14调节寄存器。 */
#define    HI6551_LDO15_REG_ADJ_OFFSET                    (0xAB) /* LDO15调节寄存器。 */
#define    HI6551_LDO16_REG_ADJ_OFFSET                    (0xAC) /* LDO16调节寄存器。 */
#define    HI6551_LDO17_REG_ADJ_OFFSET                    (0xAD) /* LDO17调节寄存器。 */
#define    HI6551_LDO18_REG_ADJ_OFFSET                    (0xAE) /* LDO18调节寄存器。 */
#define    HI6551_LDO19_REG_ADJ_OFFSET                    (0xAF) /* LDO19调节寄存器。 */
#define    HI6551_LDO20_REG_ADJ_OFFSET                    (0xB0) /* LDO20调节寄存器。 */
#define    HI6551_LDO21_REG_ADJ_OFFSET                    (0xB1) /* LDO21调节寄存器。 */
#define    HI6551_LDO22_REG_ADJ_OFFSET                    (0xB2) /* LDO22调节寄存器。 */
#define    HI6551_LDO23_REG_ADJ_OFFSET                    (0xB3) /* LDO23调节寄存器。 */
#define    HI6551_LDO24_REG_ADJ_OFFSET                    (0xB4) /* LDO24调节寄存器。 */
#define    HI6551_PMUA_REG_ADJ_OFFSET                     (0xB5) /* PMUA调节寄存器。 */
#define    HI6551_LVS_ADJ1_OFFSET                         (0xB6) /* LVS调节1寄存器。 */
#define    HI6551_LVS_ADJ2_OFFSET                         (0xB7) /* LVS调节2寄存器。 */
#define    HI6551_LVS_ADJ3_OFFSET                         (0xB8) /* LVS调节3寄存器。 */
#define    HI6551_BOOST_ADJ0_OFFSET                       (0xB9) /* BOOST调节0寄存器。 */
#define    HI6551_BOOST_ADJ1_OFFSET                       (0xBA) /* BOOST调节1寄存器。 */
#define    HI6551_BOOST_ADJ2_OFFSET                       (0xBB) /* BOOST调节2寄存器。 */
#define    HI6551_BOOST_ADJ3_OFFSET                       (0xBC) /* BOOST调节3寄存器。 */
#define    HI6551_BOOST_ADJ4_OFFSET                       (0xBD) /* BOOST调节4寄存器。 */
#define    HI6551_BOOST_ADJ5_OFFSET                       (0xBE) /* BOOST调节5寄存器。 */
#define    HI6551_BOOST_ADJ6_OFFSET                       (0xBF) /* BOOST调节6寄存器。 */
#define    HI6551_BOOST_ADJ7_OFFSET                       (0xC0) /* BOOST调节7寄存器。 */
#define    HI6551_BOOST_ADJ8_OFFSET                       (0xC1) /* BOOST调节8寄存器。 */
#define    HI6551_BOOST_ADJ9_OFFSET                       (0xC2) /* BOOST调节9寄存器。 */
#define    HI6551_BOOST_ADJ10_OFFSET                      (0xC3) /* BOOST调节10寄存器。 */
#define    HI6551_BOOST_ADJ11_OFFSET                      (0xC4) /* BOOST调节11寄存器。 */
#define    HI6551_CLASSD_ADJ0_OFFSET                      (0xC5) /* CLASSD调节0寄存器。 */
#define    HI6551_CLASSD_ADJ1_OFFSET                      (0xC6) /* CLASSD调节1寄存器。 */
#define    HI6551_CLASSD_ADJ2_OFFSET                      (0xC7) /* CLASSD调节2寄存器。 */
#define    HI6551_BANDGAP_THSD_ADJ1_OFFSET                (0xC8) /* BANDGAP和THSD调节1寄存器。 */
#define    HI6551_BANDGAP_THSD_ADJ2_OFFSET                (0xC9) /* BANDGAP和THSD调节2寄存器。 */
#define    HI6551_DR_FLA_CTRL1_OFFSET                     (0xCA) /* DR闪烁模式控制1寄存器。 */
#define    HI6551_DR_FLA_CTRL2_OFFSET                     (0xCB) /* DR闪烁模式控制2寄存器。 */
#define    HI6551_FLASH_PERIOD_OFFSET                     (0xCC) /* DR闪烁模式周期调节寄存器。 */
#define    HI6551_FLASH_ON_OFFSET                         (0xCD) /* DR闪烁模式点亮时间调节寄存器。 */
#define    HI6551_DR_MODE_SEL_OFFSET                      (0xCE) /* DR灯模式选择寄存器。 */
#define    HI6551_DR_BRE_CTRL_OFFSET                      (0xCF) /* DR1/2呼吸模式控制寄存器。 */
#define    HI6551_DR1_TIM_CONF0_OFFSET                    (0xD0) /* DR1常亮常暗时间配置寄存器。 */
#define    HI6551_DR1_TIM_CONF1_OFFSET                    (0xD1) /* DR1渐亮渐暗时间配置寄存器。 */
#define    HI6551_DR1_ISET_OFFSET                         (0xD2) /* DR1电流选择寄存器。 */
#define    HI6551_DR2_TIM_CONF0_OFFSET                    (0xD3) /* DR2常亮常暗时间配置寄存器。 */
#define    HI6551_DR2_TIM_CONF1_OFFSET                    (0xD4) /* DR2渐亮渐暗时间配置寄存器。 */
#define    HI6551_DR2_ISET_OFFSET                         (0xD5) /* DR2电流选择寄存器。 */
#define    HI6551_DR_LED_CTRL_OFFSET                      (0xD6) /* DR3/4/5控制寄存器。 */
#define    HI6551_DR_OUT_CTRL_OFFSET                      (0xD7) /* DR3/4/5输出控制寄存器。 */
#define    HI6551_DR3_ISET_OFFSET                         (0xD8) /* DR3电流选择寄存器。 */
#define    HI6551_DR3_START_DEL_OFFSET                    (0xD9) /* DR3启动延时配置寄存器。 */
#define    HI6551_DR3_TIM_CONF0_OFFSET                    (0xDA) /* DR3常亮常暗时间配置寄存器。 */
#define    HI6551_DR3_TIM_CONF1_OFFSET                    (0xDB) /* DR3渐亮渐暗时间配置寄存器。 */
#define    HI6551_DR4_ISET_OFFSET                         (0xDC) /* DR4电流选择寄存器。 */
#define    HI6551_DR4_START_DEL_OFFSET                    (0xDD) /* DR4启动延时配置寄存器 */
#define    HI6551_DR4_TIM_CONF0_OFFSET                    (0xDE) /* DR4常亮常暗时间配置寄存器。 */
#define    HI6551_DR4_TIM_CONF1_OFFSET                    (0xDF) /* DR4渐亮渐暗时间配置寄存器。 */
#define    HI6551_DR5_ISET_OFFSET                         (0xE0) /* DR5电流选择寄存器。 */
#define    HI6551_DR5_START_DEL_OFFSET                    (0xE1) /* DR5启动延时配置寄存器。 */
#define    HI6551_DR5_TIM_CONF0_OFFSET                    (0xE2) /* DR5常亮常暗时间配置寄存器。 */
#define    HI6551_DR5_TIM_CONF1_OFFSET                    (0xE3) /* DR5渐亮渐暗时间配置寄存器。 */
#define    HI6551_OTP_CTRL1_OFFSET                        (0xE4) /* OTP控制1信号寄存器。 */
#define    HI6551_OTP_CTRL2_OFFSET                        (0xE5) /* OTP控制2信号寄存器。 */
#define    HI6551_OTP_PDIN_OFFSET                         (0xE6) /* OTP写入值寄存器。 */
#define    HI6551_OTP_PDOB0_OFFSET                        (0xE7) /* OTP读出值0寄存器。 */
#define    HI6551_OTP_PDOB1_OFFSET                        (0xE8) /* OTP读出值1寄存器。 */
#define    HI6551_OTP_PDOB2_OFFSET                        (0xE9) /* OTP读出值2寄存器。 */
#define    HI6551_OTP_PDOB3_OFFSET                        (0xEA) /* OTP读出值3寄存器。 */
#define    HI6551_RTCCR_A0_OFFSET                         (0xEB) /* RTC_A数据0寄存器。 */
#define    HI6551_RTCCR_A1_OFFSET                         (0xEC) /* RTC_A数据1寄存器。 */
#define    HI6551_RTCCR_A2_OFFSET                         (0xED) /* RTC_A数据2寄存器。 */
#define    HI6551_RTCCR_A3_OFFSET                         (0xEE) /* RTC_A数据3寄存器。 */
#define    HI6551_RTCLR_A0_OFFSET                         (0xEF) /* RTCLR_A加载0寄存器。 */
#define    HI6551_RTCLR_A1_OFFSET                         (0xF0) /* RTCLR_A加载1寄存器。 */
#define    HI6551_RTCLR_A2_OFFSET                         (0xF1) /* RTCLR_A加载2寄存器。 */
#define    HI6551_RTCLR_A3_OFFSET                         (0xF2) /* RTCLR_A加载3寄存器。 */
#define    HI6551_RTCCTRL_A_OFFSET                        (0xF3) /* RTCCTRL_A控制寄存器。 */
#define    HI6551_RTCMR_A_A0_OFFSET                       (0xF4) /* RTC_A_A比较0寄存器。 */
#define    HI6551_RTCMR_A_A1_OFFSET                       (0xF5) /* RTC_A_A比较1寄存器。 */
#define    HI6551_RTCMR_A_A2_OFFSET                       (0xF6) /* RTC_A_A比较2寄存器。 */
#define    HI6551_RTCMR_A_A3_OFFSET                       (0xF7) /* RTC_A_A比较3寄存器。 */
#define    HI6551_RTCMR_A_B0_OFFSET                       (0xF8) /* RTC_A_B比较0寄存器。 */
#define    HI6551_RTCMR_A_B1_OFFSET                       (0xF9) /* RTC_A_B比较1寄存器。 */
#define    HI6551_RTCMR_A_B2_OFFSET                       (0xFA) /* RTC_A_B比较2寄存器。 */
#define    HI6551_RTCMR_A_B3_OFFSET                       (0xFB) /* RTC_A_B比较3寄存器。 */
#define    HI6551_RTCMR_A_C0_OFFSET                       (0xFC) /* RTC_A_C比较0寄存器。 */
#define    HI6551_RTCMR_A_C1_OFFSET                       (0xFD) /* RTC_A_C比较1寄存器。 */
#define    HI6551_RTCMR_A_C2_OFFSET                       (0xFE) /* RTC_A_C比较2寄存器。 */
#define    HI6551_RTCMR_A_C3_OFFSET                       (0xFF) /* RTC_A_C比较3寄存器。 */
#define    HI6551_RTCMR_A_D0_OFFSET                       (0x100) /* RTC_A_D比较0寄存器。 */
#define    HI6551_RTCMR_A_D1_OFFSET                       (0x101) /* RTC_A_D比较1寄存器。 */
#define    HI6551_RTCMR_A_D2_OFFSET                       (0x102) /* RTC_A_D比较2寄存器。 */
#define    HI6551_RTCMR_A_D3_OFFSET                       (0x103) /* RTC_A_D比较3寄存器。 */
#define    HI6551_RTCMR_A_E0_OFFSET                       (0x104) /* RTC_A_E比较0寄存器。 */
#define    HI6551_RTCMR_A_E1_OFFSET                       (0x105) /* RTC_A_E比较1寄存器。 */
#define    HI6551_RTCMR_A_E2_OFFSET                       (0x106) /* RTC_A_E比较2寄存器。 */
#define    HI6551_RTCMR_A_E3_OFFSET                       (0x107) /* RTC_A_E比较3寄存器。 */
#define    HI6551_RTCMR_A_F0_OFFSET                       (0x108) /* RTC_A_F比较0寄存器。 */
#define    HI6551_RTCMR_A_F1_OFFSET                       (0x109) /* RTC_A_F比较1寄存器。 */
#define    HI6551_RTCMR_A_F2_OFFSET                       (0x10A) /* RTC_A_F比较2寄存器。 */
#define    HI6551_RTCMR_A_F3_OFFSET                       (0x10B) /* RTC_A_F比较3寄存器。 */
#define    HI6551_RTCCR_NA0_OFFSET                        (0x10C) /* RTC_NA数据0寄存器。 */
#define    HI6551_RTCCR_NA1_OFFSET                        (0x10D) /* RTC_NA数据1寄存器。 */
#define    HI6551_RTCCR_NA2_OFFSET                        (0x10E) /* RTC_NA数据2寄存器。 */
#define    HI6551_RTCCR_NA3_OFFSET                        (0x10F) /* RTC_NA数据3寄存器。 */
#define    HI6551_RTCLR_NA0_OFFSET                        (0x110) /* RTCLR_NA加载0寄存器。 */
#define    HI6551_RTCLR_NA1_OFFSET                        (0x111) /* RTCLR_NA加载1寄存器。 */
#define    HI6551_RTCLR_NA2_OFFSET                        (0x112) /* RTCLR_NA加载2寄存器。 */
#define    HI6551_RTCLR_NA3_OFFSET                        (0x113) /* RTCLR_NA加载3寄存器。 */
#define    HI6551_RTCCTRL_NA_OFFSET                       (0x114) /* RTCCTRL_NA控制寄存器。 */
#define    HI6551_VERSION_OFFSET                          (0x115) /* 版本寄存器。 */
#define    HI6551_RESERVED0_OFFSET                        (0x116) /* 保留0寄存器。 */
#define    HI6551_RESERVED1_OFFSET                        (0x117) /* 保留1寄存器。 */
#define    HI6551_RESERVED2_OFFSET                        (0x118) /* 保留2寄存器。 */
#define    HI6551_RESERVED3_OFFSET                        (0x119) /* 保留3寄存器。 */
#define    HI6551_RESERVED4_OFFSET                        (0x11A) /* 保留4寄存器。 */
#define    HI6551_HTOL_MODE_OFFSET                        (0x11B) /* 老化测试模式配置寄存器。 */
#define    HI6551_DAC_CTRL_OFFSET                         (0x11C) /* DAC控制寄存器 */
#define    HI6551_CHIP_SOFT_RST_OFFSET                    (0x11D) /* 芯片模块软复位寄存器。 */
#define    HI6551_NP_REG_ADJ_OFFSET                       (0x11E) /* 非下电调节寄存器。 */
#define    HI6551_NP_REG_CHG_OFFSET                       (0x11F) /* 备用电池非下电调节寄存器。 */
#define    HI6551_NP_RSVED1_OFFSET                        (0x120) /* 非下电保留1寄存器。 */
#define    HI6551_NP_RSVED2_OFFSET                        (0x121) /* 非下电保留2寄存器。 */
#define    HI6551_NP_RSVED3_OFFSET                        (0x122) /* 非下电保留3寄存器。 */
#define    HI6551_NP_RSVED4_OFFSET                        (0x123) /* 非下电保留4寄存器。 */
#define    HI6551_RTC_ADJ1_OFFSET                         (0x124) /* rtc_adj[7:0]寄存器。 */
#define    HI6551_RTC_ADJ2_OFFSET                         (0x125) /* rtc_adj[15:8]寄存器。 */
#define    HI6551_CLJ_CTRL_REG_OFFSET                     (0x133) /* 寄存器控制寄存器。 */
#define    HI6551_ECO_REFALSH_TIME_OFFSET                 (0x134) /* ECO_REFLASH工作时间寄存器。 */
#define    HI6551_CL_OUT0_OFFSET                          (0x135) /* cl_out[7:0]寄存器。 */
#define    HI6551_CL_OUT1_OFFSET                          (0x136) /* cl_out[15:8]寄存器。 */
#define    HI6551_CL_OUT2_OFFSET                          (0x137) /* cl_out[23:16]寄存器。 */
#define    HI6551_CL_OUT3_OFFSET                          (0x138) /* cl_out[31:24]寄存器。 */
#define    HI6551_CL_IN0_OFFSET                           (0x139) /* cl_in[7:0]寄存器。 */
#define    HI6551_CL_IN1_OFFSET                           (0x13A) /* cl_in[15:8]寄存器。 */
#define    HI6551_CL_IN2_OFFSET                           (0x13B) /* cl_in[23:16]寄存器。 */
#define    HI6551_CL_IN3_OFFSET                           (0x13C) /* cl_in[31:24]寄存器。 */
#define    HI6551_CHG_TIMER0_OFFSET                       (0x13D) /* chg_timer[7:0]寄存器。 */
#define    HI6551_CHG_TIMER1_OFFSET                       (0x13E) /* chg_timer[15:8]寄存器。 */
#define    HI6551_CHG_TIMER2_OFFSET                       (0x13F) /* chg_timer[23:16]寄存器。 */
#define    HI6551_CHG_TIMER3_OFFSET                       (0x140) /* chg_timer[31:24]寄存器。 */
#define    HI6551_LOAD_TIMER0_OFFSET                      (0x141) /* load_timer[7:0]寄存器。 */
#define    HI6551_LOAD_TIMER1_OFFSET                      (0x142) /* load_timer[15:8]寄存器。 */
#define    HI6551_LOAD_TIMER2_OFFSET                      (0x143) /* load_timer[23:16]寄存器。 */
#define    HI6551_LOAD_TIMER3_OFFSET                      (0x144) /* load_timer[31:24]寄存器。 */
#define    HI6551_OFF_TIMER0_OFFSET                       (0x145) /* off_timer[7:0]寄存器。 */
#define    HI6551_OFF_TIMER1_OFFSET                       (0x146) /* off_timer[15:8]寄存器。 */
#define    HI6551_OFF_TIMER2_OFFSET                       (0x147) /* off_timer[23:16]寄存器。 */
#define    HI6551_OFF_TIMER3_OFFSET                       (0x148) /* off_timer[31:24]寄存器。 */
#define    HI6551_CL_INT0_OFFSET                          (0x149) /* cl_int[7:0]寄存器。 */
#define    HI6551_CL_INT1_OFFSET                          (0x14A) /* cl_int[15:8]寄存器。 */
#define    HI6551_CL_INT2_OFFSET                          (0x14B) /* cl_int[23:16]寄存器。 */
#define    HI6551_CL_INT3_OFFSET                          (0x14C) /* cl_int[31:24]寄存器。 */
#define    HI6551_V_INT0_OFFSET                           (0x14D) /* v_int[7:0]寄存器。 */
#define    HI6551_V_INT1_OFFSET                           (0x14E) /* v_int[15:8]寄存器。 */
#define    HI6551_OFFSET_CURRENT0_OFFSET                  (0x14F) /* offset_current[7:0]寄存器。 */
#define    HI6551_OFFSET_CURRENT1_OFFSET                  (0x150) /* offset_current[15:8]寄存器。 */
#define    HI6551_OFFSET_VOLTAGE0_OFFSET                  (0x151) /* offset_voltage[7:0]寄存器。 */
#define    HI6551_OFFSET_VOLTAGE1_OFFSET                  (0x152) /* offset_voltage[15:8]寄存器。 */
#define    HI6551_OCV_DATA1_OFFSET                        (0x153) /* ocv_data[7:0]寄存器。 */
#define    HI6551_OCV_DATA2_OFFSET                        (0x154) /* ocv_data[15:8]寄存器。 */
#define    HI6551_V_OUT0_PRE0_OFFSET                      (0x155) /* v_out_pre0[7:0]寄存器。 */
#define    HI6551_V_OUT1_PRE0_OFFSET                      (0x156) /* v_out_pre0[15:8]寄存器。 */
#define    HI6551_V_OUT0_PRE1_OFFSET                      (0x157) /* v_out_pre1[7:0]寄存器。 */
#define    HI6551_V_OUT1_PRE1_OFFSET                      (0x158) /* v_out_pre1[15:8]寄存器。 */
#define    HI6551_V_OUT0_PRE2_OFFSET                      (0x159) /* v_out_pre2[7:0]寄存器。 */
#define    HI6551_V_OUT1_PRE2_OFFSET                      (0x15A) /* v_out_pre2[15:8]寄存器。 */
#define    HI6551_V_OUT0_PRE3_OFFSET                      (0x15B) /* v_out_pre3[7:0]寄存器。 */
#define    HI6551_V_OUT1_PRE3_OFFSET                      (0x15C) /* v_out_pre3[15:8]寄存器。 */
#define    HI6551_V_OUT0_PRE4_OFFSET                      (0x15D) /* v_out_pre4[7:0]寄存器。 */
#define    HI6551_V_OUT1_PRE4_OFFSET                      (0x15E) /* v_out_pre4[15:8]寄存器。 */
#define    HI6551_V_OUT0_PRE5_OFFSET                      (0x15F) /* v_out_pre5[7:0]寄存器。 */
#define    HI6551_V_OUT1_PRE5_OFFSET                      (0x160) /* v_out_pre5[15:8]寄存器。 */
#define    HI6551_V_OUT0_PRE6_OFFSET                      (0x161) /* v_out_pre6[7:0]寄存器。 */
#define    HI6551_V_OUT1_PRE6_OFFSET                      (0x162) /* v_out_pre6[15:8]寄存器。 */
#define    HI6551_V_OUT0_PRE7_OFFSET                      (0x163) /* v_out_pre7[7:0]寄存器。 */
#define    HI6551_V_OUT1_PRE7_OFFSET                      (0x164) /* v_out_pre7[15:8]寄存器。 */
#define    HI6551_V_OUT0_PRE8_OFFSET                      (0x165) /* v_out_pre8[7:0]寄存器。 */
#define    HI6551_V_OUT1_PRE8_OFFSET                      (0x166) /* v_out_pre8[15:8]寄存器。 */
#define    HI6551_V_OUT0_PRE9_OFFSET                      (0x167) /* v_out_pre9[7:0]寄存器。 */
#define    HI6551_V_OUT1_PRE9_OFFSET                      (0x168) /* v_out_pre9[15:8]寄存器。 */
#define    HI6551_V_OUT0_PRE10_OFFSET                     (0x169) /* v_out_pre10[7:0]寄存器。 */
#define    HI6551_V_OUT1_PRE10_OFFSET                     (0x16A) /* v_out_pre10[15:8]寄存器。 */
#define    HI6551_V_OUT0_PRE11_OFFSET                     (0x16B) /* v_out_pre11[7:0]寄存器。 */
#define    HI6551_V_OUT1_PRE11_OFFSET                     (0x16C) /* v_out_pre11[15:8]寄存器。 */
#define    HI6551_V_OUT0_PRE12_OFFSET                     (0x16D) /* v_out_pre12[7:0]寄存器。 */
#define    HI6551_V_OUT1_PRE12_OFFSET                     (0x16E) /* v_out_pre12[15:8]寄存器。 */
#define    HI6551_V_OUT0_PRE13_OFFSET                     (0x16F) /* v_out_pre13[7:0]寄存器。 */
#define    HI6551_V_OUT1_PRE13_OFFSET                     (0x170) /* v_out_pre13[15:8]寄存器。 */
#define    HI6551_V_OUT0_PRE14_OFFSET                     (0x171) /* v_out_pre14[7:0]寄存器。 */
#define    HI6551_V_OUT1_PRE14_OFFSET                     (0x172) /* v_out_pre14[15:8]寄存器。 */
#define    HI6551_V_OUT0_PRE15_OFFSET                     (0x173) /* v_out_pre15[7:0]寄存器。 */
#define    HI6551_V_OUT1_PRE15_OFFSET                     (0x174) /* v_out_pre15[15:8]寄存器。 */
#define    HI6551_V_OUT0_PRE16_OFFSET                     (0x175) /* v_out_pre16[7:0]寄存器。 */
#define    HI6551_V_OUT1_PRE16_OFFSET                     (0x176) /* v_out_pre16[15:8]寄存器。 */
#define    HI6551_V_OUT0_PRE17_OFFSET                     (0x177) /* v_out_pre17[7:0]寄存器。 */
#define    HI6551_V_OUT1_PRE17_OFFSET                     (0x178) /* v_out_pre17[15:8]寄存器。 */
#define    HI6551_V_OUT0_PRE18_OFFSET                     (0x179) /* v_out_pre18[7:0]寄存器。 */
#define    HI6551_V_OUT1_PRE18_OFFSET                     (0x17A) /* v_out_pre18[15:8]寄存器。 */
#define    HI6551_V_OUT0_PRE19_OFFSET                     (0x17B) /* v_out_pre19[7:0]寄存器。 */
#define    HI6551_V_OUT1_PRE19_OFFSET                     (0x17C) /* v_out_pre19[15:8]寄存器。 */
#define    HI6551_CURRENT0_PRE0_OFFSET                    (0x17D) /* current_pre0[7:0]寄存器。 */
#define    HI6551_CURRENT1_PRE0_OFFSET                    (0x17E) /* current_pre0[15:8]寄存器。 */
#define    HI6551_CURRENT0_PRE1_OFFSET                    (0x17F) /* current_pre1[7:0]寄存器。 */
#define    HI6551_CURRENT1_PRE1_OFFSET                    (0x180) /* current_pre1[15:8]寄存器。 */
#define    HI6551_CURRENT0_PRE2_OFFSET                    (0x181) /* current_pre2[7:0]寄存器。 */
#define    HI6551_CURRENT1_PRE2_OFFSET                    (0x182) /* current_pre2[15:8]寄存器。 */
#define    HI6551_CURRENT0_PRE3_OFFSET                    (0x183) /* current_pre3[7:0]寄存器。 */
#define    HI6551_CURRENT1_PRE3_OFFSET                    (0x184) /* current_pre3[15:8]寄存器。 */
#define    HI6551_CURRENT0_PRE4_OFFSET                    (0x185) /* current_pre4[7:0]寄存器。 */
#define    HI6551_CURRENT1_PRE4_OFFSET                    (0x186) /* current_pre4[15:8]寄存器。 */
#define    HI6551_CURRENT0_PRE5_OFFSET                    (0x187) /* current_pre5[7:0]寄存器。 */
#define    HI6551_CURRENT1_PRE5_OFFSET                    (0x188) /* current_pre5[15:8]寄存器。 */
#define    HI6551_CURRENT0_PRE6_OFFSET                    (0x189) /* current_pre6[7:0]寄存器。 */
#define    HI6551_CURRENT1_PRE6_OFFSET                    (0x18A) /* current_pre6[15:8]寄存器。 */
#define    HI6551_CURRENT0_PRE7_OFFSET                    (0x18B) /* current_pre7[7:0]寄存器。 */
#define    HI6551_CURRENT1_PRE7_OFFSET                    (0x18C) /* current_pre7[15:8]寄存器。 */
#define    HI6551_CURRENT0_PRE8_OFFSET                    (0x18D) /* current_pre8[7:0]寄存器。 */
#define    HI6551_CURRENT1_PRE8_OFFSET                    (0x18E) /* current_pre8[15:8]寄存器。 */
#define    HI6551_CURRENT0_PRE9_OFFSET                    (0x18F) /* current_pre9[7:0]寄存器。 */
#define    HI6551_CURRENT1_PRE9_OFFSET                    (0x190) /* current_pre9[15:8]寄存器。 */
#define    HI6551_CURRENT0_PRE10_OFFSET                   (0x191) /* current_pre10[7:0]寄存器。 */
#define    HI6551_CURRENT1_PRE10_OFFSET                   (0x192) /* current_pre10[15:8]寄存器。 */
#define    HI6551_CURRENT0_PRE11_OFFSET                   (0x193) /* current_pre11[7:0]寄存器。 */
#define    HI6551_CURRENT1_PRE11_OFFSET                   (0x194) /* current_pre11[15:8]寄存器。 */
#define    HI6551_CURRENT0_PRE12_OFFSET                   (0x195) /* current_pre12[7:0]寄存器。 */
#define    HI6551_CURRENT1_PRE12_OFFSET                   (0x196) /* current_pre12[15:8]寄存器。 */
#define    HI6551_CURRENT0_PRE13_OFFSET                   (0x197) /* current_pre13[7:0]寄存器。 */
#define    HI6551_CURRENT1_PRE13_OFFSET                   (0x198) /* current_pre13[15:8]寄存器。 */
#define    HI6551_CURRENT0_PRE14_OFFSET                   (0x199) /* current_pre14[7:0]寄存器。 */
#define    HI6551_CURRENT1_PRE14_OFFSET                   (0x19A) /* current_pre14[15:8]寄存器。 */
#define    HI6551_CURRENT0_PRE15_OFFSET                   (0x19B) /* current_pre15[7:0]寄存器。 */
#define    HI6551_CURRENT1_PRE15_OFFSET                   (0x19C) /* current_pre15[15:8]寄存器。 */
#define    HI6551_CURRENT0_PRE16_OFFSET                   (0x19D) /* current_pre16[7:0]寄存器。 */
#define    HI6551_CURRENT1_PRE16_OFFSET                   (0x19E) /* current_pre16[15:8]寄存器。 */
#define    HI6551_CURRENT0_PRE17_OFFSET                   (0x19F) /* current_pre17[7:0]寄存器。 */
#define    HI6551_CURRENT1_PRE17_OFFSET                   (0x1A0) /* current_pre17[15:8]寄存器。 */
#define    HI6551_CURRENT0_PRE18_OFFSET                   (0x1A1) /* current_pre18[7:0]寄存器。 */
#define    HI6551_CURRENT1_PRE18_OFFSET                   (0x1A2) /* current_pre18[15:8]寄存器。 */
#define    HI6551_CURRENT0_PRE19_OFFSET                   (0x1A3) /* current_pre19[7:0]寄存器。 */
#define    HI6551_CURRENT1_PRE19_OFFSET                   (0x1A4) /* current_pre19[15:8]寄存器。 */
#define    HI6551_CLJ_DEBUG_OFFSET                        (0x1A5) /* 库仑计DEBUG专用1寄存器。 */
#define    HI6551_STATE_TEST_OFFSET                       (0x1A6) /* 库仑计DEBUG专用2寄存器。 */
#define    HI6551_CLJ_RESERVED1_OFFSET                    (0x1A7) /* 库仑计保留1寄存器。 */
#define    HI6551_CLJ_RESERVED2_OFFSET                    (0x1A8) /* 库仑计保留2寄存器。 */
#define    HI6551_CLJ_RESERVED3_OFFSET                    (0x1A9) /* 库仑计保留3寄存器。 */
#define    HI6551_CLJ_RESERVED4_OFFSET                    (0x1AA) /* 库仑计保留4寄存器。 */
#define    HI6551_CLJ_RESERVED5_OFFSET                    (0x1AB) /* 库仑计保留5寄存器。 */
#define    HI6551_CLJ_RESERVED6_OFFSET                    (0x1AC) /* 库仑计保留6寄存器。 */
#define    HI6551_CLJ_RESERVED7_OFFSET                    (0x1AD) /* 库仑计保留7寄存器。 */
#define    HI6551_CLJ_RESERVED8_OFFSET                    (0x1AE) /* 库仑计保留8寄存器。 */

/********************************************************************************/
/*    hi6551 寄存器定义（项目名_模块名_寄存器名_T)        */
/********************************************************************************/
typedef union
{
    struct
    {
        unsigned int    otmp_d1r                   : 1; /* [0..0] 0： 当前温度低于设定阈值（默认125℃）；1： 当前温度高于设定阈值。 */
        unsigned int    otmp150_d1r                : 1; /* [1..1] 0： 当前温度低于150℃；1： 当前温度高于150℃。 */
        unsigned int    osc_state                  : 1; /* [2..2] 0：OSC晶体时钟已经就绪，时钟切换完成;1：OSC晶体时钟没有就绪。 */
        unsigned int    vsys_uv_d2f                : 1; /* [3..3] 当前vsys电压：0：vsys<2.7V/3VV；1：vsys>2.7V/3.0V； */
        unsigned int    vsys_6p0_d200ur            : 1; /* [4..4] 当前vsys电压：0：vsys<6.0V；1：vsys>6.0V； */
        unsigned int    pwron_d20r                 : 1; /* [5..5] 0：PWRON键当前没有被按下；1:PWRON键当前被按下(pwron为pwron_n的反向)。 */
        unsigned int    reserved                   : 2; /* [7..6] 保留。 */
    } bits;
    unsigned int    u32;
}HI6551_STATUS1_T;    /* 状态标志1寄存器。 */

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
}HI6551_STATUS2_T;    /* 状态标志2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    otmp_d1r                   : 1; /* [0..0] 0:无此中断；1：otmp 报温度超过设定阈值（默认125度）中断。 */
        unsigned int    vsys_2p5_f                 : 1; /* [1..1] 0:无此中断；1:vsys电压低于2.5V中断。 */
        unsigned int    vsys_uv_d2f                : 1; /* [2..2] 0:无此中断；1:vsys电压低于2.7V/3.0V 2ms中断。(默认3V，非下电寄存器vsys_uv_adj可调) */
        unsigned int    vsys_6p0_d200ur            : 1; /* [3..3] 0：无此中断；1:vsys报电压高于6.0V 200us中断。 */
        unsigned int    pwron_d4sr                 : 1; /* [4..4] 0:无此中断；1:PWRON按键按下4s中断(该中断只在正常工作时按下才上报) 。 */
        unsigned int    pwron_d20f                 : 1; /* [5..5] 0:无此中断；1:PWRON按键释放20ms中断 。 */
        unsigned int    pwron_d20r                 : 1; /* [6..6] 0:无此中断；1:PWRON按键按下20ms中断。 */
        unsigned int    reserved                   : 1; /* [7..7] 保留。 */
    } bits;
    unsigned int    u32;
}HI6551_IRQ1_T;    /* 中断1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    ocp_scp_r                  : 1; /* [0..0] 0:无此中断；1:表示至少有一个BUCK/BOOST/CLASSD发生过流保护或者短路保护，或者LDO/LVS/发生过流保护，或者BOOST发生过压/欠压保护，CPU查询到后要去查看下面的过流保存寄存器查看是哪一个发生过流，并将其写1清后，中断消除。 */
        unsigned int    vbus_det_1p375_d90ur       : 1; /* [1..1] 0:无此中断；1:vbus_det>1.375V 90us中断 。 */
        unsigned int    vbus_det_0p9_d3f           : 1; /* [2..2] 0:无此中断；1:vbus_det<0.9 V 3ms中断 。 */
        unsigned int    vbus_det_0p9_d3r           : 1; /* [3..3] 0:无此中断；1:vbus_det>0.9 V 3ms中断 。 */
        unsigned int    reserved                   : 4; /* [7..4] 保留。 */
    } bits;
    unsigned int    u32;
}HI6551_IRQ2_T;    /* 中断2寄存器。 */

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
}HI6551_IRQ3_T;    /* 中断3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    sim0_hpd_out_f             : 1; /* [0..0] SIM0卡不在位发生SIM0_HPD中断：0： 无此中断 ；  1： 发生下降沿跳变  。 */
        unsigned int    sim0_hpd_out_r             : 1; /* [1..1] SIM0卡不在位发生SIM0_HPD中断：0： 无此中断 ；  1： 发生上升沿跳变  。 */
        unsigned int    sim0_hpd_in_f              : 1; /* [2..2] SIM0卡在位发生SIM0_HPD中断：0： 无此中断 ；  1： 发生下降沿跳变  。 */
        unsigned int    sim0_hpd_in_r              : 1; /* [3..3] SIM0卡在位发生SIM0_HPD中断：0： 无此中断 ；  1： 发生上升沿跳变  。 */
        unsigned int    sim1_hpd_out_f             : 1; /* [4..4] SIM1卡不在位发生SIM1_HPD中断：：0： 无此中断 ；  1： 发生下降沿跳变  。 */
        unsigned int    sim1_hpd_out_r             : 1; /* [5..5] SIM1卡不在位发生SIM1_HPD中断：0： 无此中断 ；  1： 发生上升沿跳变  。 */
        unsigned int    sim1_hpd_in_f              : 1; /* [6..6] SIM1卡在位发生SIM1_HPD中断：0： 无此中断 ；  1： 发生下降沿跳变  。 */
        unsigned int    sim1_hpd_in_r              : 1; /* [7..7] SIM1卡在位发生SIM1_HPD中断：0： 无此中断 ；  1： 发生上升沿跳变  。 */
    } bits;
    unsigned int    u32;
}HI6551_IRQ4_T;    /* 中断4寄存器。 */

typedef union
{
    struct
    {
        unsigned int    cl_int_i                   : 1; /* [0..0] cl_out>cl_int时上报中断 */
        unsigned int    cl_out_i                   : 1; /* [1..1] cl_out计数到81.25%上报中断 */
        unsigned int    cl_in_i                    : 1; /* [2..2] cl_in计数到81.25%上报中断 */
        unsigned int    chg_timer_i                : 1; /* [3..3] chg_timer计数到81.25%上报中断 */
        unsigned int    load_timer_i               : 1; /* [4..4] load_timer计数到81.25%上报中断 */
        unsigned int    vbat_int_i                 : 1; /* [5..5] vbat电压<设定的vbat_int值 */
        unsigned int    reserved                   : 2; /* [7..6] 保留。 */
    } bits;
    unsigned int    u32;
}HI6551_COUL_IRQ_T;    /* 库仑计中断寄存器。 */

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
}HI6551_IRQ1_MASK_T;    /* 中断屏蔽1寄存器。 */

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
}HI6551_IRQ2_MASK_T;    /* 中断屏蔽2寄存器。 */

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
}HI6551_IRQ3_MASK_T;    /* 中断屏蔽3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    sim0_hpd_out_f_mk          : 1; /* [0..0] SIM0卡不在位发生SIM0_HPD中断：0:使能该中断；1：屏蔽该中断。 */
        unsigned int    sim0_hpd_out_r_mk          : 1; /* [1..1] SIM0卡不在位发生SIM0_HPD中断：0:使能该中断；1：屏蔽该中断。 */
        unsigned int    sim0_hpd_in_f_mk           : 1; /* [2..2] SIM0卡在位发生SIM0_HPD中断：0:使能该中断；1：屏蔽该中断。 */
        unsigned int    sim0_hpd_in_r_mk           : 1; /* [3..3] SIM0卡在位发生SIM0_HPD中断：0:使能该中断；1：屏蔽该中断。 */
        unsigned int    sim1_hpd_out_f_mk          : 1; /* [4..4] SIM1卡不在位发生SIM1_HPD中断：：0:使能该中断；1：屏蔽该中断。 */
        unsigned int    sim1_hpd_out_r_mk          : 1; /* [5..5] SIM1卡不在位发生SIM1_HPD中断：0:使能该中断；1：屏蔽该中断。 */
        unsigned int    sim1_hpd_in_f_mk           : 1; /* [6..6] SIM1卡在位发生SIM1_HPD中断：0:使能该中断；1：屏蔽该中断。 */
        unsigned int    sim1_in_hpd_r_mk           : 1; /* [7..7] SIM1卡在位发生SIM1_HPD中断：0:使能该中断；1：屏蔽该中断。 */
    } bits;
    unsigned int    u32;
}HI6551_IRQ4_MASK_T;    /* 中断屏蔽4寄存器。 */

typedef union
{
    struct
    {
        unsigned int    cl_int_i_mk                : 1; /* [0..0] cl_int_i中断屏蔽 */
        unsigned int    cl_out_i_mk                : 1; /* [1..1] cl_out_i中断屏蔽 */
        unsigned int    cl_in_i_mk                 : 1; /* [2..2] cl_in_i中断屏蔽 */
        unsigned int    chg_timer_i_mk             : 1; /* [3..3] chg_timer_i中断屏蔽 */
        unsigned int    load_timer_i_mk            : 1; /* [4..4] load_timer_i中断屏蔽 */
        unsigned int    vbat_int_i_mk              : 1; /* [5..5] vbat_int_i中断屏蔽 */
        unsigned int    reserved                   : 2; /* [7..6] 保留。 */
    } bits;
    unsigned int    u32;
}HI6551_COUL_IRQ_MASK_T;    /* 库仑计中断寄存器。 */

typedef union
{
    struct
    {
        unsigned int    scp_buck0_1                : 1; /* [0..0] 0：BUCK0_1输出不过载；1: BUCK0_1输出过载。注：BUCK0和BUCK1共用1bits短路保护中断记录寄存器。 */
        unsigned int    scp_buck2                  : 1; /* [1..1] 0：BUCK2输出不短路；1: BUCK2输出短路。 */
        unsigned int    scp_buck3                  : 1; /* [2..2] 0：BUCK3输出不短路；1: BUCK3输出短路。 */
        unsigned int    scp_buck4                  : 1; /* [3..3] 0：BUCK4输出不短路；1: BUCK4输出短路。 */
        unsigned int    scp_buck5                  : 1; /* [4..4] 0：BUCK5输出不短路；1: BUCK5输出短路。 */
        unsigned int    scp_buck6                  : 1; /* [5..5] 0：BUCK6输出不短路；1: BUCK6输出短路。 */
        unsigned int    reserved                   : 2; /* [7..6] 保留。 */
    } bits;
    unsigned int    u32;
}HI6551_SCP_RECORD1_T;    /* 短路保护记录1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    ocp_buck0_1                : 1; /* [0..0] 0：BUCK0_1输出不过载；1: BUCK0_1输出过载。  注:用于上报中断，清除该位即可消除buck0_1过流中断。当buck0和buck1同时并联使用时，只有都发生过流后才会触发过流保护，但当单独使用某个buck时，只要发生过流就触发过流保护。 */
        unsigned int    ocp_buck2                  : 1; /* [1..1] 0：BUCK2输出不过载；1: BUCK2输出过载。 */
        unsigned int    ocp_buck3                  : 1; /* [2..2] 0：BUCK3输出不过载；1: BUCK3输出过载。 */
        unsigned int    ocp_buck4                  : 1; /* [3..3] 0：BUCK4输出不过载；1: BUCK4输出过载。 */
        unsigned int    ocp_buck5                  : 1; /* [4..4] 0：BUCK5输出不过载；1: BUCK5输出过载。 */
        unsigned int    ocp_buck6                  : 1; /* [5..5] 0：BUCK6输出不过载；1: BUCK6输出过载。 */
        unsigned int    reserved                   : 2; /* [7..6] 保留。 */
    } bits;
    unsigned int    u32;
}HI6551_OCP_RECORD1_T;    /* 过流保护记录1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    ocp_ldo1                   : 1; /* [0..0] 0：LDO1输出不过载；1: LDO1输出过载。 */
        unsigned int    ocp_ldo2                   : 1; /* [1..1] 0：LDO2输出不过载；1: LDO2输出过载。 */
        unsigned int    ocp_ldo3                   : 1; /* [2..2] 0：LDO3输出不过载；1: LDO3输出过载。 */
        unsigned int    ocp_ldo4                   : 1; /* [3..3] 0：LDO4输出不过载；1: LDO4输出过载。 */
        unsigned int    ocp_ldo5                   : 1; /* [4..4] 0：LDO5输出不过载；1: LDO5输出过载。 */
        unsigned int    ocp_ldo6                   : 1; /* [5..5] 0：LDO6输出不过载；1: LDO6输出过载。 */
        unsigned int    ocp_ldo7                   : 1; /* [6..6] 0：LDO7输出不过载；1: LDO7输出过载。 */
        unsigned int    ocp_ldo8                   : 1; /* [7..7] 0：LDO8输出不过载；1: LDO8输出过载。 */
    } bits;
    unsigned int    u32;
}HI6551_OCP_RECORD2_T;    /* 过流保护记录2寄存器。 */

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
        unsigned int    ocp_ldo15                  : 1; /* [6..6] 0：LDO15输出不过载；1: LDO15输出过载。 */
        unsigned int    ocp_ldo16                  : 1; /* [7..7] 0：LDO16输出不过载；1: LDO16输出过载。 */
    } bits;
    unsigned int    u32;
}HI6551_OCP_RECORD3_T;    /* 过流保护记录3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    ocp_ldo17                  : 1; /* [0..0] 0：LDO17输出不过载；1: LDO17输出过载。 */
        unsigned int    ocp_ldo18                  : 1; /* [1..1] 0：LDO18输出不过载；1: LDO18输出过载。 */
        unsigned int    ocp_ldo19                  : 1; /* [2..2] 0：LDO19输出不过载；1: LDO19输出过载。 */
        unsigned int    ocp_ldo20                  : 1; /* [3..3] 0：LDO20输出不过载；1: LDO20输出过载。 */
        unsigned int    ocp_ldo21                  : 1; /* [4..4] 0：LDO21输出不过载；1: LDO21输出过载。 */
        unsigned int    ocp_ldo22                  : 1; /* [5..5] 0：LDO22输出不过载；1: LDO22输出过载。 */
        unsigned int    ocp_ldo23                  : 1; /* [6..6] 0：LDO23输出不过载；1: LDO23输出过载。 */
        unsigned int    ocp_ldo24                  : 1; /* [7..7] 0：LDO24输出不过载；1: LDO24输出过载。 */
    } bits;
    unsigned int    u32;
}HI6551_OCP_RECORD4_T;    /* 过流保护记录4寄存器。 */

typedef union
{
    struct
    {
        unsigned int    ocp_lvs2                   : 1; /* [0..0] 0：LVS2输出不过载；1: LVS2输出过载。 */
        unsigned int    ocp_lvs3                   : 1; /* [1..1] 0：LVS3输出不过载；1: LVS3输出过载。 */
        unsigned int    ocp_lvs4                   : 1; /* [2..2] 0：LVS4输出不过载；1: LVS4输出过载。 */
        unsigned int    ocp_lvs5                   : 1; /* [3..3] 0：LVS5输出不过载；1: LVS5输出过载。 */
        unsigned int    ocp_lvs7                   : 1; /* [4..4] 0：LVS7输出不过载；1: LVS7输出过载。 */
        unsigned int    ocp_lvs6                   : 1; /* [5..5] 0：LVS6输出不过载；1: LVS6输出过载。 */
        unsigned int    ocp_lvs8                   : 1; /* [6..6] 0：LVS8输出不过载；1: LVS8输出过载。 */
        unsigned int    reserved                   : 1; /* [7..7] 保留。 */
    } bits;
    unsigned int    u32;
}HI6551_OCP_RECORD5_T;    /* 过流保护记录5寄存器。 */

typedef union
{
    struct
    {
        unsigned int    ocp_lvs9                   : 1; /* [0..0] 0：LVS9输出不过载；1: LVS9输出过载。 */
        unsigned int    ocp_lvs10                  : 1; /* [1..1] 0：LVS10输出不过载；1: LVS10输出过载。 */
        unsigned int    reserved                   : 6; /* [7..2] 保留。 */
    } bits;
    unsigned int    u32;
}HI6551_OCP_RECORD6_T;    /* 过流保护记录6寄存器。 */

typedef union
{
    struct
    {
        unsigned int    ocp_boost                  : 1; /* [0..0] 0：BOOOST输出不过流；1: BOOOST输出过流。 */
        unsigned int    scp_boost                  : 1; /* [1..1] 0：BOOOST输出不短路；1: BOOOST输出短路。 */
        unsigned int    ovp_boost                  : 1; /* [2..2] 0：BOOOST输出不过压；1: BOOOST输出过压。 */
        unsigned int    uvp_boost                  : 1; /* [3..3] 0：BOOOST输出不欠压；1: BOOOST输出欠压。 */
        unsigned int    ocp_classd                 : 1; /* [4..4] 0：CLASSD输出不过流；1: CLASSD输出过流。 */
        unsigned int    reserved                   : 3; /* [7..5] 保留。 */
    } bits;
    unsigned int    u32;
}HI6551_OCP_RECORD7_T;    /* 过流保护记录7寄存器。 */

typedef union
{
    struct
    {
        unsigned int    np_otmp_d1r                : 1; /* [0..0] 温度报警(默认125°)：0:没有发生过该事件；1:发生该事件。 */
        unsigned int    np_hreset_n_f              : 1; /* [1..1] 热复位：0:没有发生过该事件；1:发生该事件。 */
        unsigned int    np_vsys_2p5_f              : 1; /* [2..2] vsys < 2.5V：0:没有发生过该事件；1:发生该事件。 */
        unsigned int    np_vsys_uv_d2f             : 1; /* [3..3] vsys < 2.7V/3.0V 2ms:0:没有发生过该事件；1:发生该事件。 */
        unsigned int    np_vsys_6p0_d200ur         : 1; /* [4..4] vsys> 6.0V 200us:0:没有发生过该事件；1:发生该事件。 */
        unsigned int    np_pwron_d4sr              : 1; /* [5..5] 在正常工作状态下PWRON按下4s:0:没有发生过该事件；1:发生该事件。 */
        unsigned int    np_pwron_d20f              : 1; /* [6..6] PWRON按键释放20ms：0:没有发生过该事件；1:发生该事件。 */
        unsigned int    np_pwron_d20r              : 1; /* [7..7] PWRON按键按下20ms：0:没有发生过该事件；1:发生该事件。 */
    } bits;
    unsigned int    u32;
}HI6551_NP_IRQ1_RECORD_T;    /* 中断1非下电记录寄存器。 */

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
}HI6551_NP_IRQ2_RECORD_T;    /* 中断2非下电记录寄存器。 */

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
        unsigned int    reserved                   : 2; /* [7..6] 保留。 */
    } bits;
    unsigned int    u32;
}HI6551_NP_IRQ3_RECORD_T;    /* 中断3非下电记录寄存器。 */

typedef union
{
    struct
    {
        unsigned int    np_scp_buck0_1             : 1; /* [0..0] 0：仅记录BUCK0_1输出不短路；1: 仅记录BUCK0_1输出短路。注：BUCK0和BUCK1共用1bits短路保护中断记录寄存器。 */
        unsigned int    np_scp_buck2               : 1; /* [1..1] 0：仅记录BUCK2输出不短路；1: 仅记录BUCK2输出短路。 */
        unsigned int    np_scp_buck3               : 1; /* [2..2] 0：仅记录BUCK3输出不短路；1: 仅记录BUCK3输出短路。 */
        unsigned int    np_scp_buck4               : 1; /* [3..3] 0：仅记录BUCK4输出不短路；1: 仅记录BUCK4输出短路。 */
        unsigned int    np_scp_buck5               : 1; /* [4..4] 0：仅记录BUCK5输出不短路；1: 仅记录BUCK5输出短路。 */
        unsigned int    np_scp_buck6               : 1; /* [5..5] 0：仅记录BUCK6输出不短路；1: 仅记录BUCK6输出短路。 */
        unsigned int    reserved                   : 2; /* [7..6] 保留。 */
    } bits;
    unsigned int    u32;
}HI6551_NP_SCP_RECORD1_T;    /* 短路保护非下电记录1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    np_ocp_buck0               : 1; /* [0..0] 0：仅记录BUCK0输出不过载；1: 仅记录BUCK0输出过载。注：仅用于记录，不做它用。 */
        unsigned int    np_ocp_buck1               : 1; /* [1..1] 0：仅记录BUCK1输出不过载；1: 仅记录BUCK1输出过载。注：仅用于记录，不做它用。 */
        unsigned int    np_ocp_buck0_1             : 1; /* [2..2] 0：仅记录BUCK0_1输出不过载；1: 仅记录BUCK0_1输出过载。 */
        unsigned int    np_ocp_buck2               : 1; /* [3..3] 0：仅记录BUCK2输出不过载；1: 仅记录BUCK2输出过载。 */
        unsigned int    np_ocp_buck3               : 1; /* [4..4] 0：仅记录BUCK3输出不过载；1: 仅记录BUCK3输出过载。 */
        unsigned int    np_ocp_buck4               : 1; /* [5..5] 0：仅记录BUCK4输出不过载；1: 仅记录BUCK4输出过载。 */
        unsigned int    np_ocp_buck5               : 1; /* [6..6] 0：仅记录BUCK5输出不过载；1: 仅记录BUCK5输出过载。 */
        unsigned int    np_ocp_buck6               : 1; /* [7..7] 0：仅记录BUCK6输出不过载；1: 仅记录BUCK6输出过载。 */
    } bits;
    unsigned int    u32;
}HI6551_NP_OCP_RECORD1_T;    /* 过流保护非下电记录1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    np_ocp_ldo1                : 1; /* [0..0] 0：仅记录LDO1输出不过载；1: 仅记录LDO1输出过载。 */
        unsigned int    np_ocp_ldo2                : 1; /* [1..1] 0：仅记录LDO2输出不过载；1: 仅记录LDO2输出过载。 */
        unsigned int    np_ocp_ldo3                : 1; /* [2..2] 0：仅记录LDO3输出不过载；1: 仅记录LDO3输出过载 。 */
        unsigned int    np_ocp_ldo4                : 1; /* [3..3] 0：仅记录LDO4输出不过载；1: 仅记录LDO4输出过载。 */
        unsigned int    np_ocp_ldo5                : 1; /* [4..4] 0：仅记录LDO5输出不过载；1: 仅记录LDO5输出过载。 */
        unsigned int    np_ocp_ldo6                : 1; /* [5..5] 0：仅记录LDO6输出不过载；1: 仅记录LDO6输出过载。 */
        unsigned int    np_ocp_ldo7                : 1; /* [6..6] 0：仅记录LDO7输出不过载；1: 仅记录LDO7输出过载。 */
        unsigned int    np_ocp_ldo8                : 1; /* [7..7] 0：仅记录LDO8输出不过载；1: 仅记录LDO8输出过载。 */
    } bits;
    unsigned int    u32;
}HI6551_NP_OCP_RECORD2_T;    /* 过流保护非下电记录2寄存器。 */

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
        unsigned int    np_ocp_ldo15               : 1; /* [6..6] 0：仅记录LDO15输出不过载；1: 仅记录LDO15输出过载。 */
        unsigned int    np_ocp_ldo16               : 1; /* [7..7] 0：仅记录LDO16输出不过载；1: 仅记录LDO16输出过载。 */
    } bits;
    unsigned int    u32;
}HI6551_NP_OCP_RECORD3_T;    /* 过流保护非下电记录3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    np_ocp_ldo17               : 1; /* [0..0] 0：仅记录LDO17输出不过载；1: 仅记录LDO17输出过载。 */
        unsigned int    np_ocp_ldo18               : 1; /* [1..1] 0：仅记录LDO18输出不过载；1: 仅记录LDO18输出过载。 */
        unsigned int    np_ocp_ldo19               : 1; /* [2..2] 0：仅记录LDO19输出不过载；1: 仅记录LDO19输出过载。 */
        unsigned int    np_ocp_ldo20               : 1; /* [3..3] 0：仅记录LDO20输出不过载；1: 仅记录LDO20输出过载。 */
        unsigned int    np_ocp_ldo21               : 1; /* [4..4] 0：仅记录LDO21输出不过载；1: 仅记录LDO21输出过载。 */
        unsigned int    np_ocp_ldo22               : 1; /* [5..5] 0：仅记录LDO22输出不过载；1: 仅记录LDO22输出过载。 */
        unsigned int    np_ocp_ldo23               : 1; /* [6..6] 0：仅记录LDO23输出不过载；1: 仅记录LDO23输出过载。 */
        unsigned int    np_ocp_ldo24               : 1; /* [7..7] 0：仅记录LDO24输出不过载；1: 仅记录LDO24输出过载。 */
    } bits;
    unsigned int    u32;
}HI6551_NP_OCP_RECORD4_T;    /* 过流保护非下电记录4寄存器。 */

typedef union
{
    struct
    {
        unsigned int    np_ocp_lvs2                : 1; /* [0..0] 0：仅记录LVS2输出不过载；1: 仅记录LVS2输出过载。 */
        unsigned int    np_ocp_lvs3                : 1; /* [1..1] 0：仅记录LVS3输出不过载；1: 仅记录LVS3输出过载。 */
        unsigned int    np_ocp_lvs4                : 1; /* [2..2] 0：仅记录LVS4输出不过载；1: 仅记录LVS4输出过载。 */
        unsigned int    np_ocp_lvs5                : 1; /* [3..3] 0：仅记录LVS5输出不过载；1: 仅记录LVS5输出过载。 */
        unsigned int    np_ocp_lvs7                : 1; /* [4..4] 0：仅记录LVS7输出不过载；1: 仅记录LVS7输出过载。 */
        unsigned int    np_ocp_lvs6                : 1; /* [5..5] 0：仅记录LVS6输出不过载；1: 仅记录LVS6输出过载。 */
        unsigned int    np_ocp_lvs8                : 1; /* [6..6] 0：仅记录LVS8输出不过载；1: 仅记录LVS8输出过载。 */
        unsigned int    reserved                   : 1; /* [7..7] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_NP_OCP_RECORD5_T;    /* 过流保护非下电记录5寄存器。 */

typedef union
{
    struct
    {
        unsigned int    np_ocp_lvs9                : 1; /* [0..0] 0：仅记录LVS9输出不过载；1: 仅记录LVS9输出过载。 */
        unsigned int    np_ocp_lvs10               : 1; /* [1..1] 0：仅记录LVS10输出不过载；1: 仅记录LVS10输出过载。 */
        unsigned int    reserved                   : 6; /* [7..2] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_NP_OCP_RECORD6_T;    /* 过流保护非下电记录6寄存器。 */

typedef union
{
    struct
    {
        unsigned int    np_ocp_boost               : 1; /* [0..0] 0：仅记录BOOST输出不过流；1: 仅记录BOOST输出过流。 */
        unsigned int    np_scp_boost               : 1; /* [1..1] 0：仅记录BOOST输出不短路；1: 仅记录BOOST输出短路。 */
        unsigned int    np_ovp_boost               : 1; /* [2..2] 0：仅记录BOOST输出不过压；1: 仅记录BOOST输出过压。 */
        unsigned int    np_uvp_boost               : 1; /* [3..3] 0：仅记录BOOST输出不欠压；1: 仅记录BOOST输出欠压。 */
        unsigned int    np_ocp_classd              : 1; /* [4..4] 0：仅记录CLASSD输出不过流；1: 仅记录CLASSD输出过流。 */
        unsigned int    reserved                   : 3; /* [7..5] 保留。 */
    } bits;
    unsigned int    u32;
}HI6551_NP_OCP_RECORD7_T;    /* 过流保护非下电记录7寄存器。 */

typedef union
{
    struct
    {
        unsigned int    ocp_ldo_deb_sel            : 2; /* [1..0] ocp去抖时间：00：500us；01：1ms；10：2ms；11：4ms。 */
        unsigned int    en_ldo_ocp_deb             : 1; /* [2..2] 0：LDO OCP上升沿不去抖；1：LDO OCP上升沿去抖；注：LDO与LVS共用。 */
        unsigned int    ocp_buck_deb_sel           : 2; /* [4..3] BUCK 的ocp和scp去抖时间：00：500us；01：1ms；10：2ms；11：4ms。注：CLASSD，BOOST与BUCK共用。 */
        unsigned int    en_buck_ocp_deb            : 1; /* [5..5] 0：BUCK OCP上升沿不去抖；1：BUCK OCP上升沿去抖。注：CLASSD与BUCK共用。 */
        unsigned int    en_buck_scp_deb            : 1; /* [6..6] 0：BUCK SCP上升沿不去抖；1：BUCK SCP上升沿去抖；注：SCP表示短路保护。 */
        unsigned int    en_bst_ocp_deb             : 1; /* [7..7] 0：BOOST 保护上升沿不去抖；1：BOOST 保护上升沿去抖；注：BOOST 保护包括过流，短路，过压，欠压，与BUCK共用去抖时间调节 */
    } bits;
    unsigned int    u32;
}HI6551_OCP_DEB_T;    /* OCP去抖动调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    vbus_det_0p9_ctrl          : 1; /* [0..0] 0：屏蔽vbus_det<0.9V检测；1:不屏蔽vbus_det<0.9V检测。 */
        unsigned int    vsys_2p5_ctrl              : 1; /* [1..1] 0：屏蔽vsys< 2.5V检测；1:不屏蔽vsys<2.5V检测。 */
        unsigned int    vsys_uv_ctrl               : 1; /* [2..2] 0：屏蔽vsys< 2.7V/3.0V检测；1:不屏蔽vsys<2.7V/3.0V检测。 */
        unsigned int    vsys_6p0_ctrl              : 1; /* [3..3] 0：屏蔽vsys>6.0V检测；1:不屏蔽vsys>6.0检测。 */
        unsigned int    otmp150_ctrl               : 1; /* [4..4] 0：高温（150℃）不关机；1：高温(150℃)关机。 */
        unsigned int    en_32kc                    : 1; /* [5..5] 0： 关闭  CLK_32C时钟输出；1：  CLK_32C 时钟输出使能。 */
        unsigned int    en_32kb                    : 1; /* [6..6] 0： 关闭 CLK_32B 时钟输出；1：  CLK_32B 时钟输出使能。 */
        unsigned int    en_32ka                    : 1; /* [7..7] 0： 关闭 CLK_32A 时钟输出；1：  CLK_32A 时钟输出使能。 */
    } bits;
    unsigned int    u32;
}HI6551_ONOFF8_T;    /* 开关控制8寄存器。 */

typedef union
{
    struct
    {
        unsigned int    ocp_scp_ctrl_buck0_1       : 1; /* [0..0] ocp_scp_off_buck0_1配置为1时：ocp_scp_ctrl_buck0_1:0:工作在过流模式2；1:工作在过流模式3； */
        unsigned int    ocp_scp_off_buck0_1        : 1; /* [1..1] 0：过流后或短路后不支持BUCK1模块自动关闭；1：过流或短路后支持BUCK1模块自动关闭 。 */
        unsigned int    ocp_scp_ctrl_buck2         : 1; /* [2..2] ocp_scp_off_buck2配置为1时：ocp_scp_ctrl_buck2:0:工作在过流模式2；1:工作在过流模式3； */
        unsigned int    ocp_scp_off_buck2          : 1; /* [3..3] 0：过流或短路后不支持BUCK2模块自动关闭；1：过流或短路后支持BUCK2模块自动关闭 。 */
        unsigned int    ocp_scp_ctrl_buck3         : 1; /* [4..4] ocp_scp_off_buck3配置为1时：ocp_scp_ctrl_buck3:0:工作在过流模式2；1:工作在过流模式3； */
        unsigned int    ocp_scp_off_buck3          : 1; /* [5..5] 0：过流或短路后不支持BUCK3模块自动关闭；1：过流或短路后支持BUCK3模块自动关闭 。 */
        unsigned int    ocp_scp_ctrl_buck4         : 1; /* [6..6] ocp_scp_off_buck4配置为1时：ocp_scp_ctrl_buck4:0:工作在过流模式2；1:工作在过流模式3； */
        unsigned int    ocp_scp_off_buck4          : 1; /* [7..7] 0：过流或短路后不支持BUCK4模块自动关闭；1：过流或短路后支持BUCK4模块自动关闭 。 */
    } bits;
    unsigned int    u32;
}HI6551_OCP_SCP_MOD_CTRL1_T;    /* BUCK0-3过流和短路保护模式控制寄存器。 */

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
}HI6551_OCP_SCP_MOD_CTRL2_T;    /* BUCK4-7过流和短路保护模式控制寄存器。 */

typedef union
{
    struct
    {
        unsigned int    ocp_ctrl_ldo1              : 1; /* [0..0] ocp_off_ldo1配置为1时：ocp_ctrl_ldo1:0:工作在过流模式2；1:工作在过流模式3； */
        unsigned int    ocp_off_ldo1               : 1; /* [1..1] 0：过流后不支持LDO1模块自动关闭；1：过流后支持LDO1模块自动关闭 。 */
        unsigned int    ocp_ctrl_ldo2              : 1; /* [2..2] ocp_off_ldo2配置为1时：ocp_ctrl_ldo2:0：过流模式为模式2；1：过流模式为模式3。 */
        unsigned int    ocp_off_ldo2               : 1; /* [3..3] 0：过流后不支持LDO2模块自动关闭；1：过流后支持LDO2模块自动关闭 。 */
        unsigned int    ocp_ctrl_ldo3              : 1; /* [4..4] ocp_off_ldo3配置为1时：ocp_ctrl_ldo3:0：过流模式为模式2；1：过流模式为模式3。 */
        unsigned int    ocp_off_ldo3               : 1; /* [5..5] 0：过流后不支持LDO3模块自动关闭；1：过流后支持LDO3模块自动关闭 。 */
        unsigned int    ocp_ctrl_ldo4              : 1; /* [6..6] ocp_off_ldo4配置为1时：ocp_ctrl_ldo4:0:工作在过流模式2；1:工作在过流模式3； */
        unsigned int    ocp_off_ldo4               : 1; /* [7..7] 0：过流后不支持LDO4模块自动关闭；1：过流后支持LDO4模块自动关闭 。 */
    } bits;
    unsigned int    u32;
}HI6551_OCP_MOD_CTRL3_T;    /* LDO1-4过流保护模式控制寄存器。 */

typedef union
{
    struct
    {
        unsigned int    ocp_ctrl_ldo5              : 1; /* [0..0] ocp_off_ldo5配置为1时：ocp_ctrl_ldo5:0:工作在过流模式2；1:工作在过流模式3； */
        unsigned int    ocp_off_ldo5               : 1; /* [1..1] 0：过流后不支持LDO5模块自动关闭；1：过流后支持LDO5模块自动关闭 。 */
        unsigned int    ocp_ctrl_ldo6              : 1; /* [2..2] ocp_off_ldo6配置为1时：ocp_ctrl_ldo6:0:工作在过流模式2；1:工作在过流模式3； */
        unsigned int    ocp_off_ldo6               : 1; /* [3..3] 0：过流后不支持LDO6模块自动关闭；1：过流后支持LDO6模块自动关闭 。 */
        unsigned int    ocp_ctrl_ldo7              : 1; /* [4..4] ocp_off_ldo7配置为1时：ocp_ctrl_ldo7:0:工作在过流模式2；1:工作在过流模式3； */
        unsigned int    ocp_off_ldo7               : 1; /* [5..5] 0：过流后不支持LDO7模块自动关闭；1：过流后支持LDO7模块自动关闭 。 */
        unsigned int    ocp_ctrl_ldo8              : 1; /* [6..6] ocp_off_ldo8配置为1时：ocp_ctrl_ldo8:0:工作在过流模式2；1:工作在过流模式3； */
        unsigned int    ocp_off_ldo8               : 1; /* [7..7] 0：过流后不支持LDO8模块自动关闭；1：过流后支持LDO8模块自动关闭 。 */
    } bits;
    unsigned int    u32;
}HI6551_OCP_MOD_CTRL4_T;    /* LDO5-8过流保护模式控制寄存器。 */

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
}HI6551_OCP_MOD_CTRL5_T;    /* LDO9-12过流保护模式控制寄存器。 */

typedef union
{
    struct
    {
        unsigned int    ocp_ctrl_ldo13             : 1; /* [0..0] ocp_off_ldo13配置为1时：ocp_ctrl_ldo13:0:工作在过流模式2；1:工作在过流模式3； */
        unsigned int    ocp_off_ldo13              : 1; /* [1..1] 0：过流后不支持LDO13模块自动关闭；1：过流后支持LDO13模块自动关闭 。 */
        unsigned int    ocp_ctrl_ldo14             : 1; /* [2..2] ocp_off_ldo14配置为1时：ocp_ctrl_ldo14:0:工作在过流模式2；1:工作在过流模式3； */
        unsigned int    ocp_off_ldo14              : 1; /* [3..3] 0：过流后不支持LDO14模块自动关闭；1：过流后支持LDO14模块自动关闭 。 */
        unsigned int    ocp_ctrl_ldo15             : 1; /* [4..4] ocp_off_ldo15配置为1时：ocp_ctrl_ldo15:0:工作在过流模式2；1:工作在过流模式3； */
        unsigned int    ocp_off_ldo15              : 1; /* [5..5] 0：过流后不支持LDO15模块自动关闭；1：过流后支持LDO15模块自动关闭 。 */
        unsigned int    ocp_ctrl_ldo16             : 1; /* [6..6] ocp_off_ldo16配置为1时：ocp_ctrl_ldo16:0:工作在过流模式2；1:工作在过流模式3； */
        unsigned int    ocp_off_ldo16              : 1; /* [7..7] 0：过流后不支持LDO16模块自动关闭；1：过流后支持LDO16模块自动关闭 。 */
    } bits;
    unsigned int    u32;
}HI6551_OCP_MOD_CTRL6_T;    /* LDO13-16过流保护模式控制寄存器。 */

typedef union
{
    struct
    {
        unsigned int    ocp_ctrl_ldo17             : 1; /* [0..0] ocp_off_ldo17配置为1时：ocp_ctrl_ldo17:0:工作在过流模式2；1:工作在过流模式3； */
        unsigned int    ocp_off_ldo17              : 1; /* [1..1] 0：过流后不支持LDO17模块自动关闭；1：过流后支持LDO17模块自动关闭 。 */
        unsigned int    ocp_ctrl_ldo18             : 1; /* [2..2] ocp_off_ldo18配置为1时：ocp_ctrl_ldo18:0:工作在过流模式2；1:工作在过流模式3； */
        unsigned int    ocp_off_ldo18              : 1; /* [3..3] 0：过流后不支持LDO18模块自动关闭；1：过流后支持LDO18模块自动关闭 。 */
        unsigned int    ocp_ctrl_ldo19             : 1; /* [4..4] ocp_off_ldo19配置为1时：ocp_ctrl_ldo19:0:工作在过流模式2；1:工作在过流模式3； */
        unsigned int    ocp_off_ldo19              : 1; /* [5..5] 0：过流后不支持LDO19模块自动关闭；1：过流后支持LDO19模块自动关闭 。 */
        unsigned int    ocp_ctrl_ldo20             : 1; /* [6..6] ocp_off_ldo20配置为1时：ocp_ctrl_ldo20:0：过流模式为模式2；1：过流模式为模式3。 */
        unsigned int    ocp_off_ldo20              : 1; /* [7..7] 0：过流后不支持LDO20模块自动关闭；1：过流后支持LDO20模块自动关闭 。 */
    } bits;
    unsigned int    u32;
}HI6551_OCP_MOD_CTRL7_T;    /* LDO17-20过流保护模式控制寄存器。 */

typedef union
{
    struct
    {
        unsigned int    ocp_ctrl_ldo21             : 1; /* [0..0] ocp_off_ldo21配置为1时：ocp_ctrl_ldo21:0：过流模式为模式2；1：过流模式为模式3。 */
        unsigned int    ocp_off_ldo21              : 1; /* [1..1] 0：过流后不支持LDO21模块自动关闭；1：过流后支持LDO21模块自动关闭 。 */
        unsigned int    ocp_ctrl_ldo22             : 1; /* [2..2] ocp_off_ldo22配置为1时：ocp_ctrl_ldo22:0:工作在过流模式2；1:工作在过流模式3； */
        unsigned int    ocp_off_ldo22              : 1; /* [3..3] 0：过流后不支持LDO22模块自动关闭；1：过流后支持LDO22模块自动关闭 。 */
        unsigned int    ocp_ctrl_ldo23             : 1; /* [4..4] ocp_off_ldo23配置为1时：ocp_ctrl_ldo23:0:工作在过流模式2；1:工作在过流模式3； */
        unsigned int    ocp_off_ldo23              : 1; /* [5..5] 0：过流后不支持LDO23模块自动关闭；1：过流后支持LDO23模块自动关闭 。 */
        unsigned int    ocp_ctrl_ldo24             : 1; /* [6..6] ocp_off_ldo24配置为1时：ocp_ctrl_ldo24:0:工作在过流模式2；1:工作在过流模式3； */
        unsigned int    ocp_off_ldo24              : 1; /* [7..7] 0：过流后不支持LDO24模块自动关闭；1：过流后支持LDO24模块自动关闭 。 */
    } bits;
    unsigned int    u32;
}HI6551_OCP_MOD_CTRL8_T;    /* LDO21-ldo23过流保护模式控制寄存器。 */

typedef union
{
    struct
    {
        unsigned int    ocp_ctrl_lvs2              : 1; /* [0..0] ocp_off_lvs2配置为1时：ocp_ctrl_lvs2:0：过流模式为模式2；1：过流模式为模式3。 */
        unsigned int    ocp_off_lvs2               : 1; /* [1..1] 0：过流后不支持LVS2模块自动关闭；1：过流后支持LVS2模块自动关闭 。 */
        unsigned int    ocp_ctrl_lvs3              : 1; /* [2..2] ocp_off_lvs3配置为1时：ocp_ctrl_lvs3:0：过流模式为模式2；1：过流模式为模式3。 */
        unsigned int    ocp_off_lvs3               : 1; /* [3..3] 0：过流后不支持LVS3模块自动关闭；1：过流后支持LVS3模块自动关闭 。 */
        unsigned int    ocp_ctrl_lvs4              : 1; /* [4..4] ocp_off_lvs4配置为1时：ocp_ctrl_lvs4:0：过流模式为模式2；1：过流模式为模式3。 */
        unsigned int    ocp_off_lvs4               : 1; /* [5..5] 0：过流后不支持LVS4模块自动关闭；1：过流后支持LVS4模块自动关闭 。 */
        unsigned int    ocp_ctrl_lvs5              : 1; /* [6..6] ocp_off_lvs5配置为1时：ocp_ctrl_lvs5:0：过流模式为模式2；1：过流模式为模式3。 */
        unsigned int    ocp_off_lvs5               : 1; /* [7..7] 0：过流后不支持LVS5模块自动关闭；1：过流后支持LVS5模块自动关闭 。 */
    } bits;
    unsigned int    u32;
}HI6551_OCP_MOD_CTRL9_T;    /* LVS2-5过流保护模式控制寄存器。 */

typedef union
{
    struct
    {
        unsigned int    ocp_ctrl_lvs7              : 1; /* [0..0] ocp_off_lvs7配置为1时：ocp_ctrl_lvs7:0：过流模式为模式2；1：过流模式为模式3。 */
        unsigned int    ocp_off_lvs7               : 1; /* [1..1] 0：过流后不支持lvs7模块自动关闭；1：过流后支持lvs7模块自动关闭 。 */
        unsigned int    ocp_ctrl_lvs6              : 1; /* [2..2] ocp_off_lvs6配置为1时：ocp_ctrl_lvs6:0：过流模式为模式2；1：过流模式为模式3。 */
        unsigned int    ocp_off_lvs6               : 1; /* [3..3] 0：过流后不支持lvs6模块自动关闭；1：过流后支持lvs6模块自动关闭 。 */
        unsigned int    ocp_ctrl_lvs8              : 1; /* [4..4] ocp_off_lvs7配置为1时：ocp_ctrl_lvs7:0：过流模式为模式2；1：过流模式为模式3。 */
        unsigned int    ocp_off_lvs8               : 1; /* [5..5] 0：过流后不支持LVS8模块自动关闭；1：过流后支持LVS8模块自动关闭 。 */
        unsigned int    ocp_ctrl_lvs9              : 1; /* [6..6] ocp_off_lvs9配置为1时：ocp_ctrl_lvs9:0：过流模式为模式2；1：过流模式为模式3。 */
        unsigned int    ocp_off_lvs9               : 1; /* [7..7] 0：过流后不支持LVS9模块自动关闭；1：过流后支持LVS9模块自动关闭 。 */
    } bits;
    unsigned int    u32;
}HI6551_OCP_MOD_CTRL10_T;    /* LVS2-5过流保护模式控制寄存器。 */

typedef union
{
    struct
    {
        unsigned int    ocp_ctrl_lvs10             : 1; /* [0..0] ocp_off_lvs10配置为1时：ocp_ctrl_lvs10:0：过流模式为模式2；1：过流模式为模式3。 */
        unsigned int    ocp_off_lvs10              : 1; /* [1..1] 0：过流后不支持LVS10模块自动关闭；1：过流后支持LVS10模块自动关闭 。 */
        unsigned int    ocp_ctrl_bst               : 1; /* [2..2] ocp_off_bst配置为1时：ocp_ctrl_bst:0：保护模式为模式2；1：保护模式为模式3。 */
        unsigned int    ocp_off_bst                : 1; /* [3..3] 0：过流/短路/过压/欠压后不支持BOOST模块自动关闭；1：过流/短路/过压/欠压后支持BOOST模块自动关闭 。 */
        unsigned int    ocp_ctrl_classd            : 1; /* [4..4] ocp_off_classd配置为1时：ocp_ctrl_classd:0：保护模式为模式2；1：保护模式为模式3。 */
        unsigned int    ocp_off_classd             : 1; /* [5..5] 0：过流后不支持CLASSD模块自动关闭；1：过流后支持CLASSD模块自动关闭 。 */
        unsigned int    reserved                   : 2; /* [7..6] 保留。 */
    } bits;
    unsigned int    u32;
}HI6551_OCP_MOD_CTRL11_T;    /* LVS9-10,BOOST过流保护模式控制寄存器。 */

typedef union
{
    struct
    {
        unsigned int    en_buck0_int               : 1; /* [0..0] BUCK0开关使能：写1开启，自动清零。 */
        unsigned int    en_buck1_int               : 1; /* [1..1] BUCK1开关使能：写1开启，自动清零。 */
        unsigned int    en_buck2_int               : 1; /* [2..2] BUCK2开关使能：写1开启，自动清零。 */
        unsigned int    en_buck3_int               : 1; /* [3..3] BUCK3开关使能：写1开启，自动清零。 */
        unsigned int    en_buck4_int               : 1; /* [4..4] BUCK4开关使能：写1开启，自动清零。 */
        unsigned int    en_buck5_int               : 1; /* [5..5] BUCK5开关使能：写1开启，自动清零。 */
        unsigned int    en_buck6_int               : 1; /* [6..6] BUCK6开关使能：写1开启，自动清零。 */
        unsigned int    reserved                   : 1; /* [7..7] 保留。 */
    } bits;
    unsigned int    u32;
}HI6551_ENABLE1_T;    /* 电源模块使能1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    dis_buck0_int              : 1; /* [0..0] BUCK0开关禁止：写1关闭，自动清零。 */
        unsigned int    dis_buck1_int              : 1; /* [1..1] BUCK1开关禁止：写1关闭，自动清零。 */
        unsigned int    dis_buck2_int              : 1; /* [2..2] BUCK2开关禁止：写1关闭，自动清零。 */
        unsigned int    dis_buck3_int              : 1; /* [3..3] BUCK3开关禁止：写1关闭，自动清零。 */
        unsigned int    dis_buck4_int              : 1; /* [4..4] BUCK4开关禁止：写1关闭，自动清零。 */
        unsigned int    dis_buck5_int              : 1; /* [5..5] BUCK5开关禁止：写1关闭，自动清零。 */
        unsigned int    dis_buck6_int              : 1; /* [6..6] BUCK6开关禁止：写1关闭，自动清零。 */
        unsigned int    reserved                   : 1; /* [7..7] 保留。 */
    } bits;
    unsigned int    u32;
}HI6551_DISABLE1_T;    /* 电源模块关闭1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    st_buck0_int               : 1; /* [0..0] BUCK0开关状态：0：通过寄存器关闭； 1：通过寄存器开启。OTP_PWRSEL(OTP上电时序选择):00 ：1'b1 上电时序1 ；01 ：1'b0 上电时序2 ；10 ：1'b0 上电时序3 ；11 : 1'b0 上电时序4 。 */
        unsigned int    st_buck1_int               : 1; /* [1..1] BUCK1开关状态：0：通过寄存器关闭； 1：通过寄存器开启。OTP_PWRSEL(OTP上电时序选择):00 ：1'b1 上电时序1 ；01 ：1'b0 上电时序2 ；10 ：1'b0 上电时序3 ；11 : 1'b0 上电时序4 。 */
        unsigned int    st_buck2_int               : 1; /* [2..2] BUCK2开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
        unsigned int    st_buck3_int               : 1; /* [3..3] BUCK3开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
        unsigned int    st_buck4_int               : 1; /* [4..4] BUCK4开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
        unsigned int    st_buck5_int               : 1; /* [5..5] BUCK5开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
        unsigned int    st_buck6_int               : 1; /* [6..6] BUCK6开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
        unsigned int    reserved                   : 1; /* [7..7] 保留。 */
    } bits;
    unsigned int    u32;
}HI6551_ONOFF_STATUS1_T;    /* 电源模块开关状态1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    en_ldo1_int                : 1; /* [0..0] LDO1开关使能：写1开启，自动清零。 */
        unsigned int    en_ldo2_int                : 1; /* [1..1] LDO2开关使能：写1开启，自动清零。 */
        unsigned int    en_ldo3_buck_int           : 1; /* [2..2] BUCK下LDO3使能：写1开启，自动清零。 */
        unsigned int    en_ldo4_int                : 1; /* [3..3] LDO4开关使能：写1开启，自动清零。 */
        unsigned int    en_ldo5_int                : 1; /* [4..4] LDO5开关使能：写1开启，自动清零。 */
        unsigned int    en_ldo6_int                : 1; /* [5..5] LDO6开关使能：写1开启，自动清零。 */
        unsigned int    en_ldo7_int                : 1; /* [6..6] LDO7开关使能：写1开启，自动清零。 */
        unsigned int    en_ldo8_int                : 1; /* [7..7] LDO8开关使能：写1开启，自动清零。 */
    } bits;
    unsigned int    u32;
}HI6551_ENABLE2_T;    /* 电源模块使能2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    dis_ldo1_int               : 1; /* [0..0] LDO1开关禁止：写1关闭，自动清零。 */
        unsigned int    dis_ldo2_int               : 1; /* [1..1] LDO2开关禁止：写1关闭，自动清零。 */
        unsigned int    dis_ldo3_buck_int          : 1; /* [2..2] BUCK下LDO3禁止：写1关闭，自动清零。 */
        unsigned int    dis_ldo4_int               : 1; /* [3..3] LDO4开关禁止：写1关闭，自动清零。 */
        unsigned int    dis_ldo5_int               : 1; /* [4..4] LDO5开关禁止：写1关闭，自动清零。 */
        unsigned int    dis_ldo6_int               : 1; /* [5..5] LDO6开关禁止：写1关闭，自动清零。 */
        unsigned int    dis_ldo7_int               : 1; /* [6..6] LDO7开关禁止：写1关闭，自动清零。 */
        unsigned int    dis_ldo8_int               : 1; /* [7..7] LDO8开关禁止：写1关闭，自动清零。 */
    } bits;
    unsigned int    u32;
}HI6551_DISABLE2_T;    /* 电源模块关闭2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    st_ldo1_int                : 1; /* [0..0] LDO1开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
        unsigned int    st_ldo2_int                : 1; /* [1..1] LDO2开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
        unsigned int    st_ldo3_buck_int           : 1; /* [2..2] BUCK下LDO3时能开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
        unsigned int    st_ldo4_int                : 1; /* [3..3] LDO4开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
        unsigned int    st_ldo5_int                : 1; /* [4..4] LDO5开关状态：0：通过寄存器关闭； 1：通过寄存器开启。OTP_PWRSEL(OTP上电时序选择):00 ：1'b1 上电时序1 ；01 ：1'b0 上电时序2 ；10 ：1'b0 上电时序3 ；11 : 1'b0 上电时序4 。 */
        unsigned int    st_ldo6_int                : 1; /* [5..5] LDO6开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
        unsigned int    st_ldo7_int                : 1; /* [6..6] LDO7开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
        unsigned int    st_ldo8_int                : 1; /* [7..7] LDO8开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
    } bits;
    unsigned int    u32;
}HI6551_ONOFF_STATUS2_T;    /* 电源模块开关状态2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    en_ldo10_int               : 1; /* [0..0] LDO10开关使能：写1开启，自动清零。 */
        unsigned int    en_ldo12_int               : 1; /* [1..1] LDO12开关使能：写1开启，自动清零。 */
        unsigned int    en_ldo13_int               : 1; /* [2..2] LDO13开关使能：写1开启，自动清零。 */
        unsigned int    en_ldo14_int               : 1; /* [3..3] LDO14开关使能：写1开启，自动清零。 */
        unsigned int    en_ldo15_int               : 1; /* [4..4] LDO15开关使能：写1开启，自动清零。 */
        unsigned int    en_ldo16_int               : 1; /* [5..5] LDO16开关使能：写1开启，自动清零。 */
        unsigned int    en_ldo17_int               : 1; /* [6..6] LDO17开关使能：写1开启，自动清零。 */
        unsigned int    en_ldo18_int               : 1; /* [7..7] LDO18开关使能：写1开启，自动清零。 */
    } bits;
    unsigned int    u32;
}HI6551_ENABLE3_T;    /* 电源模块使能3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    dis_ldo10_int              : 1; /* [0..0] LDO10开关禁止：写1关闭，自动清零。 */
        unsigned int    dis_ldo12_int              : 1; /* [1..1] LDO12开关禁止：写1关闭，自动清零。 */
        unsigned int    dis_ldo13_int              : 1; /* [2..2] LDO13开关禁止：写1关闭，自动清零。 */
        unsigned int    dis_ldo14_int              : 1; /* [3..3] LDO14开关禁止：写1关闭，自动清零。 */
        unsigned int    dis_ldo15_int              : 1; /* [4..4] LDO15开关禁止：写1关闭，自动清零。 */
        unsigned int    dis_ldo16_int              : 1; /* [5..5] LDO16开关禁止：写1关闭，自动清零。 */
        unsigned int    dis_ldo17_int              : 1; /* [6..6] LDO17开关禁止：写1关闭，自动清零。 */
        unsigned int    dis_ldo18_int              : 1; /* [7..7] LDO18开关禁止：写1关闭，自动清零。 */
    } bits;
    unsigned int    u32;
}HI6551_DISABLE3_T;    /* 电源模块关闭3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    st_ldo10_int               : 1; /* [0..0] LDO10开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
        unsigned int    st_ldo12_int               : 1; /* [1..1] LDO12开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
        unsigned int    st_ldo13_int               : 1; /* [2..2] LDO13开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
        unsigned int    st_ldo14_int               : 1; /* [3..3] LDO14开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
        unsigned int    st_ldo15_int               : 1; /* [4..4] LDO15开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
        unsigned int    st_ldo16_int               : 1; /* [5..5] LDO16开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
        unsigned int    st_ldo17_int               : 1; /* [6..6] LDO17开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
        unsigned int    st_ldo18_int               : 1; /* [7..7] LDO18开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
    } bits;
    unsigned int    u32;
}HI6551_ONOFF_STATUS3_T;    /* 电源模块开关状态3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    en_ldo19_int               : 1; /* [0..0] LDO19开关使能：写1开启，自动清零。 */
        unsigned int    en_ldo20_int               : 1; /* [1..1] LDO20开关使能：写1开启，自动清零。 */
        unsigned int    en_ldo21_int               : 1; /* [2..2] LDO21开关使能：写1开启，自动清零。 */
        unsigned int    en_ldo22_int               : 1; /* [3..3] LDO22开关使能：写1开启，自动清零。 */
        unsigned int    en_ldo23_int               : 1; /* [4..4] LDO23开关使能：写1开启，自动清零。 */
        unsigned int    en_ldo24_int               : 1; /* [5..5] LDO24开关使能：写1开启，自动清零。 */
        unsigned int    en_ldo3_batt_int           : 1; /* [6..6] BATT下LDO3使能：写1开启，自动清零。 */
        unsigned int    reserved                   : 1; /* [7..7] 保留。 */
    } bits;
    unsigned int    u32;
}HI6551_ENABLE4_T;    /* 电源模块使能4寄存器。 */

typedef union
{
    struct
    {
        unsigned int    dis_ldo19_int              : 1; /* [0..0] LDO19开关禁止：写1关闭，自动清零。 */
        unsigned int    dis_ldo20_int              : 1; /* [1..1] LDO20开关禁止：写1关闭，自动清零。 */
        unsigned int    dis_ldo21_int              : 1; /* [2..2] LDO21开关禁止：写1关闭，自动清零。 */
        unsigned int    dis_ldo22_int              : 1; /* [3..3] LDO22开关禁止：写1关闭，自动清零。 */
        unsigned int    dis_ldo23_int              : 1; /* [4..4] LDO23开关禁止：写1关闭，自动清零。 */
        unsigned int    dis_ldo24_int              : 1; /* [5..5] LDO24开关禁止：写1关闭，自动清零。 */
        unsigned int    dis_ldo3_batt_int          : 1; /* [6..6] BATT下LDO3禁止：写1关闭，自动清零。 */
        unsigned int    reserved                   : 1; /* [7..7] 保留。 */
    } bits;
    unsigned int    u32;
}HI6551_DISABLE4_T;    /* 电源模块关闭4寄存器。 */

typedef union
{
    struct
    {
        unsigned int    st_ldo19_int               : 1; /* [0..0] LDO19开关状态：0：通过寄存器关闭； 1：通过寄存器开启。OTP_PWRSEL(OTP上电时序选择):00 ：1'b1 上电时序1 ；01 ：1'b0 上电时序2 ；10 ：1'b0 上电时序3 ；11 : 1'b0 上电时序4 。 */
        unsigned int    st_ldo20_int               : 1; /* [1..1] LDO20开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
        unsigned int    st_ldo21_int               : 1; /* [2..2] LDO21开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
        unsigned int    st_ldo22_int               : 1; /* [3..3] LDO22开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
        unsigned int    st_ldo23_int               : 1; /* [4..4] LDO23开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
        unsigned int    st_ldo24_int               : 1; /* [5..5] LDO24开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
        unsigned int    st_ldo3_batt_int           : 1; /* [6..6] BATT下LDO3开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
        unsigned int    reserved                   : 1; /* [7..7] 保留。 */
    } bits;
    unsigned int    u32;
}HI6551_ONOFF_STATUS4_T;    /* 电源模块开关状态4寄存器。 */

typedef union
{
    struct
    {
        unsigned int    en_lvs2_int                : 1; /* [0..0] LVS2开关使能：写1开启，自动清零。 */
        unsigned int    en_lvs3_int                : 1; /* [1..1] LVS3开关使能：写1开启，自动清零。 */
        unsigned int    en_lvs4_int                : 1; /* [2..2] LVS4开关使能：写1开启，自动清零。 */
        unsigned int    en_lvs5_int                : 1; /* [3..3] LVS5开关使能：写1开启，自动清零。 */
        unsigned int    en_lvs7_int                : 1; /* [4..4] LVS7开关使能：写1开启，自动清零。 */
        unsigned int    en_lvs6_int                : 1; /* [5..5] LVS6开关使能：写1开启，自动清零。 */
        unsigned int    en_lvs8_int                : 1; /* [6..6] LVS8开关使能：写1开启，自动清零。 */
        unsigned int    en_lvs9_int                : 1; /* [7..7] LVS9开关使能：写1开启，自动清零。 */
    } bits;
    unsigned int    u32;
}HI6551_ENABLE5_T;    /* 电源模块使能5寄存器。 */

typedef union
{
    struct
    {
        unsigned int    dis_lvs2_int               : 1; /* [0..0] LVS2开关禁止：写1关闭，自动清零。 */
        unsigned int    dis_lvs3_int               : 1; /* [1..1] LVS3开关禁止：写1关闭，自动清零。 */
        unsigned int    dis_lvs4_int               : 1; /* [2..2] LVS4开关禁止：写1关闭，自动清零。 */
        unsigned int    dis_lvs5_int               : 1; /* [3..3] LVS5开关禁止：写1关闭，自动清零。 */
        unsigned int    dis_lvs7_int               : 1; /* [4..4] LVS7开关禁止：写1关闭，自动清零。 */
        unsigned int    dis_lvs6_int               : 1; /* [5..5] LVS6开关禁止：写1关闭，自动清零。 */
        unsigned int    dis_lvs8_int               : 1; /* [6..6] LVS8开关禁止：写1关闭，自动清零。 */
        unsigned int    dis_lvs9_int               : 1; /* [7..7] LVS9开关禁止：写1关闭，自动清零。 */
    } bits;
    unsigned int    u32;
}HI6551_DISABLE5_T;    /* 电源模块关闭5寄存器。 */

typedef union
{
    struct
    {
        unsigned int    st_lvs2_int                : 1; /* [0..0] LVS2开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
        unsigned int    st_lvs3_int                : 1; /* [1..1] LVS3开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
        unsigned int    st_lvs4_int                : 1; /* [2..2] LVS4开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
        unsigned int    st_lvs5_int                : 1; /* [3..3] LVS5开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
        unsigned int    st_lvs7_int                : 1; /* [4..4] LVS7开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
        unsigned int    st_lvs6_int                : 1; /* [5..5] LVS6开关状态：0：通过寄存器关闭； 1：通过寄存器开启。OTP_PWRSEL(OTP上电时序选择):00 ：1'b1 上电时序1 ；01 ：1'b0 上电时序2 ；10 ：1'b0 上电时序3 ；11 : 1'b0 上电时序4 。 */
        unsigned int    st_lvs8_int                : 1; /* [6..6] LVS8开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
        unsigned int    st_lvs9_int                : 1; /* [7..7] LVS9开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
    } bits;
    unsigned int    u32;
}HI6551_ONOFF_STATUS5_T;    /* 电源模块开关状态5寄存器。 */

typedef union
{
    struct
    {
        unsigned int    en_lvs10_int               : 1; /* [0..0] LVS10开关使能：写1开启，自动清零。 */
        unsigned int    reserved                   : 7; /* [7..1] 保留。 */
    } bits;
    unsigned int    u32;
}HI6551_ENABLE6_T;    /* 电源模块使能6寄存器。 */

typedef union
{
    struct
    {
        unsigned int    dis_lvs10_int              : 1; /* [0..0] LVS10开关禁止：写1关闭，自动清零。 */
        unsigned int    reserved                   : 7; /* [7..1] 保留。 */
    } bits;
    unsigned int    u32;
}HI6551_DISABLE6_T;    /* 电源模块关闭6寄存器。 */

typedef union
{
    struct
    {
        unsigned int    st_lvs10_int               : 1; /* [0..0] LVS10开关状态：0：通过寄存器关闭； 1：通过寄存器开启。 */
        unsigned int    reserved                   : 7; /* [7..1] 保留。 */
    } bits;
    unsigned int    u32;
}HI6551_ONOFF_STATUS6_T;    /* 电源模块开关状态6寄存器。 */

typedef union
{
    struct
    {
        unsigned int    en_cp2_int                 : 1; /* [0..0] cp2_always_on_int = 1'b0 :usb_chg_pump 开启设置：0：关闭；1：开启。PMU集成USB PHY过压保护电路，当VBUS_DET>1.375V时，上报中断，并通过关闭en_cp2来切断VBUS到USB PHy的通路。 */
        unsigned int    cp2_always_on_int          : 1; /* [1..1] usb_chg_pump常开设置，当设置为1时，usb_chg_pump的开启只跟该位有关，当为0时，根据en_cp2_int打开或者关闭：0：usb_chg_pump关闭；1：usb_chg_pump打开。 */
        unsigned int    reserved                   : 6; /* [7..2] 保留。 */
    } bits;
    unsigned int    u32;
}HI6551_ENABLE7_T;    /* 电源模块使能7寄存器。 */

typedef union
{
    struct
    {
        unsigned int    en_ldo9_int                : 1; /* [0..0] 0：关闭LDO9 ；1：开启LDO9 。 */
        unsigned int    sim0_en_int                : 1; /* [1..1] simcard0转换电路的使能信号：0：关闭simcard0转换电路 ；1：开启simcard0转换电路 。 */
        unsigned int    en_ldo11_int               : 1; /* [2..2] 0：关闭LDO11 ；1：开启LDO11 。 */
        unsigned int    sim1_en_int                : 1; /* [3..3] simcard1转换电路的使能信号：0：关闭simcard1转换电路 ；1：开启simcard1转换电路 。 */
        unsigned int    reserved                   : 4; /* [7..4] 保留。 */
    } bits;
    unsigned int    u32;
}HI6551_SIMCARD_EN_T;    /* SIM卡使能寄存器。 */

typedef union
{
    struct
    {
        unsigned int    simcard0_resv              : 2; /* [1..0] 控制CLK输出级的驱动能力，simcard0_resv[1]控制P管的驱动能力，simcard0_resv[0]控制N管的驱动能力。0：不增加驱动能力；1：增加驱动能力。 */
        unsigned int    simcard0_bbrp              : 1; /* [2..2] simcard0的DATAIO BB测上拉电阻使能信号：0：开启上拉电阻；1：关闭上拉电阻 。 */
        unsigned int    simcard0_simrp             : 1; /* [3..3] simcard0的DATAIO SIM卡测上拉电阻使能信号：0：开启上拉电阻；1：关闭上拉电阻 。 */
        unsigned int    simcard0_200ns_en          : 1; /* [4..4] simcard0 200ns快速上拉时间的使能信号：0：关闭快速上拉功能 ；1：开始快速上拉功能 。 */
        unsigned int    reserved                   : 3; /* [7..5] 保留。 */
    } bits;
    unsigned int    u32;
}HI6551_SIM0_CFG_T;    /* SIM卡0配置寄存器。 */

typedef union
{
    struct
    {
        unsigned int    simcard1_resv              : 2; /* [1..0] 控制CLK输出级的驱动能力，simcard1_resv[1]控制P管的驱动能力，simcard1_resv[0]控制N管的驱动能力。0：不增加驱动能力；1：增加驱动能力。 */
        unsigned int    simcard1_bbrp              : 1; /* [2..2] simcard1的DATAIO BB测上拉电阻使能信号：0：开启上拉电阻；1：关闭上拉电阻 。 */
        unsigned int    simcard1_simrp             : 1; /* [3..3] simcard1的DATAIO SIM卡测上拉电阻使能信号：0：开启上拉电阻；1：关闭上拉电阻 。 */
        unsigned int    simcard1_200ns_en          : 1; /* [4..4] simcard1 200ns快速上拉时间的使能信号：0：关闭快速上拉功能 ；1：开始快速上拉功能 。 */
        unsigned int    reserved                   : 3; /* [7..5] 保留。 */
    } bits;
    unsigned int    u32;
}HI6551_SIM1_CFG_T;    /* SIM卡1配置寄存器。 */

typedef union
{
    struct
    {
        unsigned int    simcard_deb_sel            : 5; /* [4..0] SIMCARD0和SIMCARD1上下沿去抖时间可调：0x3 : 120us0x4 : 150us0x5 : 180us0x6 : 210us0x7 : 240us0x8 : 270us0x9 : 300us0xa : 330us0xb : 360us0xc : 390us0xd : 420us0xe : 450us0xf : 480us0x10: 510us0x11: 540us0x12: 570us0x13: 600us */
        unsigned int    reserved                   : 3; /* [7..5] 保留。 */
    } bits;
    unsigned int    u32;
}HI6551_SIM_DEB_T;    /* SIM卡中断去抖寄调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    en_eco_buck3_int           : 1; /* [0..0] 设置BUCK3是否随PMU进出ECO模式:0:不跟随；1：跟随。 */
        unsigned int    en_eco_buck4_int           : 1; /* [1..1] 设置BUCK4是否随PMU进出ECO模式:0:不跟随；1：跟随。 */
        unsigned int    en_eco_buck5_int           : 1; /* [2..2] 设置BUCK5是否随PMU进出ECO模式:0:不跟随；1：跟随。 */
        unsigned int    en_eco_buck6_int           : 1; /* [3..3] 设置BUCK6是否随PMU进出ECO模式:0:不跟随；1：跟随。 */
        unsigned int    reserved                   : 4; /* [7..4] 保留。 */
    } bits;
    unsigned int    u32;
}HI6551_ECO_MOD_CFG1_T;    /* 电源模块是否跟随PMU进出ECO模式配置1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    en_eco_ldo3_int            : 1; /* [0..0] 设置LDO3是否随PMU进出ECO模式:0:不跟随；1：跟随。 */
        unsigned int    en_eco_ldo7_int            : 1; /* [1..1] 设置LDO7是否随PMU进出ECO模式:0:不跟随；1：跟随。 */
        unsigned int    en_eco_ldo9_int            : 1; /* [2..2] 设置LDO9是否随PMU进出ECO模式:0:不跟随；1：跟随。 */
        unsigned int    en_eco_ldo10_int           : 1; /* [3..3] 设置LDO10是否随PMU进出ECO模式:0:不跟随；1：跟随。 */
        unsigned int    en_eco_ldo11_int           : 1; /* [4..4] 设置LDO11是否随PMU进出ECO模式:0:不跟随；1：跟随。 */
        unsigned int    en_eco_ldo12_int           : 1; /* [5..5] 设置LDO12是否随PMU进出ECO模式:0:不跟随；1：跟随。 */
        unsigned int    en_eco_ldo22_int           : 1; /* [6..6] 设置LDO22是否随PMU进出ECO模式:0:不跟随；1：跟随。 */
        unsigned int    en_eco_ldo24_int           : 1; /* [7..7] 设置LDO24是否随PMU进出ECO模式:0:不跟随；1：跟随。 */
    } bits;
    unsigned int    u32;
}HI6551_ECO_MOD_CFG2_T;    /* 电源模块是否跟随PMU进出ECO模式配置2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    force_eco_buck3_int        : 1; /* [0..0] 设置是否强行使BUCK3进入ECO模式：0:不进入；1：进入。 */
        unsigned int    force_eco_buck4_int        : 1; /* [1..1] 设置是否强行使BUCK4进入ECO模式：0:不进入；1：进入。 */
        unsigned int    force_eco_buck5_int        : 1; /* [2..2] 设置是否强行使BUCK5进入ECO模式：0:不进入；1：进入。 */
        unsigned int    force_eco_buck6_int        : 1; /* [3..3] 设置是否强行使BUCK6进入ECO模式：0:不进入；1：进入。 */
        unsigned int    reserved                   : 4; /* [7..4] 保留。 */
    } bits;
    unsigned int    u32;
}HI6551_ECO_MOD_CFG3_T;    /* 电源是否强制随PMU进出ECO模式配置3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    force_eco_ldo3_int         : 1; /* [0..0] 设置是否强行使LDO3进入ECO模式：0:不进入；1：进入。 */
        unsigned int    force_eco_ldo7_int         : 1; /* [1..1] 设置是否强行使LDO7进入ECO模式：0:不进入；1：进入。 */
        unsigned int    force_eco_ldo9_int         : 1; /* [2..2] 设置是否强行使LDO9进入ECO模式：0:不进入；1：进入。 */
        unsigned int    force_eco_ldo10_int        : 1; /* [3..3] 设置是否强行使LDO10进入ECO模式：0:不进入；1：进入。 */
        unsigned int    force_eco_ldo11_int        : 1; /* [4..4] 设置是否强行使LDO11进入ECO模式：0:不进入；1：进入。 */
        unsigned int    force_eco_ldo12_int        : 1; /* [5..5] 设置是否强行使LDO12进入ECO模式：0:不进入；1：进入。 */
        unsigned int    force_eco_ldo22_int        : 1; /* [6..6] 设置是否强行使LDO22进入ECO模式：0:不进入；1：进入。 */
        unsigned int    force_eco_ldo24_int        : 1; /* [7..7] 设置是否强行使LDO24进入ECO模式：0:不进入；1：进入。 */
    } bits;
    unsigned int    u32;
}HI6551_ECO_MOD_CFG4_T;    /* 电源是否强制随PMU进出ECO模式配置4寄存器。 */

typedef union
{
    struct
    {
        unsigned int    eco_pwrsel                 : 2; /* [1..0] PMU进出低功耗时序选择：00：PMU进出低功耗时序图组合1；01：PMU进出低功耗时序图组合2；10：PMU进出低功耗时序图组合1；11：PMU进出低功耗时序图组合1； */
        unsigned int    peri_en_ctrl_ldo12         : 1; /* [2..2] LDO12是否受Peri_EN控制上下电:0： 不受控 ；1： 受控 。 */
        unsigned int    peri_en_ctrl_lvs9          : 1; /* [3..3] LVS9是否受Peri_EN控制上下电:0： 不受控 ；1： 受控 。 */
        unsigned int    peri_en_ctrl_ldo8          : 1; /* [4..4] LDO8是否受Peri_EN控制上下电:0： 不受控 ；1： 受控 。 */
        unsigned int    peri_en_ctrl_buck3         : 1; /* [5..5] BUCK3是否受Peri_EN控制上下电:0： 不受控 ；1： 受控 。 */
        unsigned int    reserved                   : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_ENB3_ECO_MARK_T;    /* 与ENB3硬线和ECO功能相关的屏蔽信号寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck01_osc_d               : 4; /* [3..0] 控制buck01频率,默认频率为1.2MHz（频率可调范围约为300KHz~3MHz） */
        unsigned int    buck0_pfm_vth_sel          : 1; /* [4..4] 自由切换pfm是否选择用NMOS阈值自动切换PWM：0：PFM时不会自动切换强制PWM ；1：PFM时自动切换强制PWM 。 */
        unsigned int    buck0_sc_sel               : 2; /* [6..5] 斜坡补偿电阻，SENSE电阻，斜坡补偿电容调节 */
        unsigned int    reserved                   : 1; /* [7..7] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK01_REG1_ADJ_T;    /* BUCK0和BUCK1调节1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck01_mod_sel             : 2; /* [1..0] BUCK01的模式选择：00：自由切换；X1：强制PWM; */
        unsigned int    buck0_bt_sel               : 1; /* [2..2] 开关管blanking time调节：0:没有延时；1:增加10ns延时。 */
        unsigned int    buck0_pd_sel               : 1; /* [3..3] DMD状态驱动能力全开功能使能:0：屏蔽该功能 ；1：启用该功能 。 */
        unsigned int    buck0_burst_drv_ctr        : 1; /* [4..4] 进入自由切换PFM时，选择burst对功率管的控制：0：进入自由切换PFM时，选择burst控制P/N功率管 ；1：进入自由切换PFM时，选择burst只控制P功率管。 */
        unsigned int    buck01_ocp_mod_sel         : 1; /* [5..5] OCP 检测模式选择的开关：0：开 ；1：关 。 */
        unsigned int    buck01_ocp_clamp_sel       : 1; /* [6..6] OCP clamp功能的开关：0：开 ；1：关 。 */
        unsigned int    buck01_ocp_shied_sel       : 1; /* [7..7] OCP功能屏蔽功能的开关：0：开 ；1：关 。 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK01_REG2_ADJ_T;    /* BUCK0和BUCK1调节2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck0_vc_sel               : 2; /* [1..0] buck01 自由切换pfm EA 阈值电压设置电阻调节：00:45k；01:60k；10:75k；11:90k。 */
        unsigned int    reserved_1                 : 2; /* [3..2] 保留 */
        unsigned int    buck01_auto_pfm_ctr        : 2; /* [5..4] buck01 自由切换pfm辅助控制；00：自由切换不受PFM电路影响；其他：保持原有自由切换模式。 */
        unsigned int    reserved_0                 : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK01_REG3_ADJ_T;    /* BUCK0和BUCK1调节3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck0_isc_sel              : 2; /* [1..0] buck01 自由切换pfm时电感峰值电流控制增加量(00~11增大)。 */
        unsigned int    reserved_1                 : 2; /* [3..2] 保留 */
        unsigned int    buck0_ipk_sel              : 2; /* [5..4] buck01 自由切换pfm时电感峰值电流控制电阻增加量：00:21k；01:28k；10:35k；11:41k。 */
        unsigned int    reserved_0                 : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK01_REG4_ADJ_T;    /* BUCK0和BUCK1调节4寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck01_dt_sel              : 2; /* [1..0] buck01死区时间调节（每bit单独控制）:0：额外延迟5ns；1：不额外延迟。 */
        unsigned int    buck01_pg_dt_sel           : 1; /* [2..2] buck01 N管到P管死区模式选择：0：不并入新死区控制模式 ；1：并入新死区控制模式。 */
        unsigned int    buck01_ng_dt_sel           : 1; /* [3..3] buck01 P管到N管死区模式选择：0：不并入新死区控制模式；1：并入新死区控制模式。 */
        unsigned int    buck01_sft_sel             : 1; /* [4..4] buck01软启动方式选择。 */
        unsigned int    reserved                   : 3; /* [7..5] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK01_REG5_ADJ_T;    /* BUCK0和BUCK1调节5寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck01_pg_p_sel            : 2; /* [1..0] buck01 P power管驱动对应PMOS驱动能力调节（00~11减小驱动能力） */
        unsigned int    reserved_1                 : 2; /* [3..2] 保留 */
        unsigned int    buck01_pg_n_sel            : 2; /* [5..4] buck01 P power管驱动对应NMOS驱动能力调节（00~11减小驱动能力）。 */
        unsigned int    reserved_0                 : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK01_REG6_ADJ_T;    /* BUCK0和BUCK1调节6寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck01_ng_p_sel            : 2; /* [1..0] buck01 N power管驱动对应PMOS驱动能力调节（00~11减小驱动能力）。 */
        unsigned int    reserved_1                 : 2; /* [3..2] 保留 */
        unsigned int    buck01_ng_n_sel            : 2; /* [5..4] buck01 N power管驱动对应NMOS驱动能力调节（00~11减小驱动能力）。 */
        unsigned int    reserved_0                 : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK01_REG7_ADJ_T;    /* BUCK0和BUCK1调节7寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck0_ccm_sel              : 2; /* [1..0] buck0调节并联均流电流大小（00~11增大sens电流）。 */
        unsigned int    buck1_ccm_sel              : 2; /* [3..2] buck1调节并联均流电流大小（00~11增大sens电流）。 */
        unsigned int    buck0_ccc_sel              : 2; /* [5..4] 调节isns处防开关误动作的电容：0：不接电容 ；1：接电容 。 */
        unsigned int    buck0_cs_sel               : 2; /* [7..6] 调节sense管处的电容：00:0p；01:0.2p；10:0.4p；11:0.6p。 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK01_REG8_ADJ_T;    /* BUCK0和BUCK1调节8寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck0_dmd_sel              : 2; /* [1..0] BUCK DMD点设置 。 */
        unsigned int    buck01_ton_off             : 1; /* [2..2] 最小占空比调节控制位：0：关闭调节功能；1：开启调节功能。 */
        unsigned int    buck01_pfm_sleep           : 1; /* [3..3] 控制PFM工作时进入SLEEP模式信号BUCK01进出sleep模式选择:0：BUCK退出sleep模式；1：BUCK进入sleep模式，buck1_enp=buck1_en_ss=0 */
        unsigned int    buck01_ton_on              : 2; /* [5..4] 最小占空比调节位(00~11增大)。 */
        unsigned int    reserved                   : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK01_REG9_ADJ_T;    /* BUCK0和BUCK1调节9寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck01_new_dmd_sel         : 5; /* [4..0] 新dmd结构dmd电流调节 */
        unsigned int    reserved                   : 3; /* [7..5] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK01_REG10_ADJ_T;    /* BUCK0和BUCK1调节10寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck0_ocp_sel              : 4; /* [3..0] OCP 触发点选择位:0x3 :系统启动开始时或者PVDD下降到3.15V时；0xb :系统启动后正常工作时 。 */
        unsigned int    buck01_ocp_en              : 1; /* [4..4] 选择OCP功能的开关：0：开 ；1：不开。 */
        unsigned int    buck01_dmd_en              : 1; /* [5..5] 选择DMD和OCP功能的开关 ：0：开 ；1：不开。 */
        unsigned int    buck01_ea_sel              : 1; /* [6..6] 并联模式控制信号：0：分别用各自PWM信号；1：共用buck0 PWM信号。 */
        unsigned int    buck01_clk_sel             : 1; /* [7..7] 并联时钟选在信号。0：相同clk;1：两buck clk相位差180deg。 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK01_REG11_ADJ_T;    /* BUCK0和BUCK1调节11寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck0_sleep_depth_adj      : 2; /* [1..0] BUCK0 SLEEP模式调节：bit[1]: 睡眠深度控制调节:0：加深睡眠；1：减轻睡眠。Bit[0]:PFM比较器偏置电流调节:0：加大电流；1：减小电流。 */
        unsigned int    reserved_1                 : 2; /* [3..2] 保留 */
        unsigned int    buck0_ea_comp_cap          : 2; /* [5..4] BUCK0 EA内部环路补偿电容调节00到11补偿电容从小到大。 */
        unsigned int    reserved_0                 : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK01_REG12_ADJ_T;    /* BUCK0和BUCK1调节12寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck01_slope_gen           : 3; /* [2..0] buck0斜坡补偿变化：（000-111逐渐增加） */
        unsigned int    buck01_pdn_lx_det          : 1; /* [3..3] 使能LX检测的死区时间控制电路：0：不使能；1：使能。 */
        unsigned int    buck01_sleep_dmd           : 1; /* [4..4] 在正常模式下强制NMOS管关闭：0：不强制；1：强制。 */
        unsigned int    buck01_dmd_shield_n        : 1; /* [5..5] 大电流下dmd屏蔽信号：0:屏蔽；1：不屏蔽。 */
        unsigned int    buck01_ocp_delay_sel       : 1; /* [6..6] ocp屏蔽时间延长20ns信号：0：不延长；1：延长。 */
        unsigned int    buck01_dmd_clamp           : 1; /* [7..7] 老dmd嵌位使能信号:0：加上；1：不加上。 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK01_REG13_ADJ_T;    /* BUCK0和BUCK1调节13寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck01_slope_dc            : 2; /* [1..0] buck0 slope dc值设定选择。 */
        unsigned int    buck01_unsleep             : 1; /* [2..2] buck01 ECO 屏蔽选择；0： 屏蔽sleep；1：不屏蔽sleep； */
        unsigned int    buck01_dmd_float           : 1; /* [3..3] buck01 dmd比较器clamp选择0：使用clamp电路；1：不使用clamp电路； */
        unsigned int    reserved                   : 4; /* [7..4] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK01_REG14_ADJ_T;    /* BUCK0和BUCK1调节14寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck0_comp_adj1            : 8; /* [7..0] BUCK稳定性调节寄存器1:bit[7]选择不同的斜坡补偿量：0：加大补偿量；1：减小补偿量。bit[6]设置运放高频噪声抑制电容的大小：0：加大补偿量；1：减小补偿量。bit[6:3]：与输出相关的补偿零点调节电阻选择位。Bit[2:0] ：误差放大器跨导调整。 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK01_REG15_ADJ_T;    /* BUCK0和BUCK1调节15寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck01_reserved1           : 8; /* [7..0] 预留寄存器1。 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK01_REG16_ADJ_T;    /* BUCK0和BUCK1调节16寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck01_reserved0           : 8; /* [7..0] 预留寄存器0。 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK01_REG17_ADJ_T;    /* BUCK0和BUCK1调节17寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck2_adj_rlx              : 4; /* [3..0] buck2 LX反馈滤波电阻调节（每bit单独控制）:0：电阻不接入 ；1：电阻接入。 */
        unsigned int    buck2_adj_clx              : 4; /* [7..4] buck2 LX反馈滤波电容调节（每bit单独控制）:0：电容不接入 ；1：电容接入。 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK2_REG1_ADJ_T;    /* BUCK2调节1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck2_new_dmd_sel          : 5; /* [4..0] 新dmd结构dmd电流调节 */
        unsigned int    reserved                   : 3; /* [7..5] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK2_REG2_ADJ_T;    /* BUCK2调节2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck2_shield_i             : 2; /* [1..0] dmd屏蔽信号对应电流调节（00~11增大） */
        unsigned int    buck2_en_regop_clamp       : 1; /* [2..2] regulator 嵌位电路使能：1：使能；0：不使能。 */
        unsigned int    buck2_dmd_clamp            : 1; /* [3..3] 老dmd嵌位使能信号：0：加上；1：不加上。 */
        unsigned int    buck2_ocp_delay_sel        : 1; /* [4..4] ocp屏蔽时间延长20ns信号：0：不延长；1：延长。 */
        unsigned int    buck2_dmd_shield_n         : 1; /* [5..5] 大电流下dmd屏蔽信号：0:屏蔽；1：不屏蔽。 */
        unsigned int    buck2_sleep_dmd            : 1; /* [6..6] 在正常模式下强制NMOS管关闭：0：不强制；1：强制。 */
        unsigned int    buck2_pdn_lx_det           : 1; /* [7..7] 使能LX检测的死区时间控制电路：0：不使能；1：使能。 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK2_REG3_ADJ_T;    /* BUCK2调节3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck2_ocp_sel              : 2; /* [1..0] buck2 OCP点调整：0x0: 2.95A ；0x1: 3.74A ；0x2: 4.12A ；0x3: 4.72A 。 */
        unsigned int    reserved_1                 : 2; /* [3..2] 保留 */
        unsigned int    buck2_dmd_sel              : 3; /* [6..4] buck2 dmd点选择（0x0~0x7增大）。 */
        unsigned int    reserved_0                 : 1; /* [7..7] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK2_REG4_ADJ_T;    /* BUCK2调节4寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck2_ng_dt_sel            : 1; /* [0..0] buck2 P管到N管死区模式选择（每bit单独控制）：0：不并入新死区控制模式；1：并入新死区控制模式。 */
        unsigned int    buck2_pg_dt_sel            : 1; /* [1..1] buck2 N管到P管死区模式选择（每bit单独控制）：0：不并入新死区控制模式；1：并入新死区控制模式。 */
        unsigned int    buck2_sft_sel              : 1; /* [2..2] buck2软启动方式选择 */
        unsigned int    buck2_sleep                : 1; /* [3..3] buck2 sleep模式选择：0：正常 ；1：sleep 。 */
        unsigned int    reserved                   : 4; /* [7..4] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK2_REG5_ADJ_T;    /* BUCK2调节5寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck2_pg_n_sel             : 2; /* [1..0] buck2 P power管驱动对应NMOS驱动能力调节（0x0~0x3减小驱动能力）。 */
        unsigned int    reserved_1                 : 2; /* [3..2] 保留 */
        unsigned int    buck2_pg_p_sel             : 2; /* [5..4] buck2 P power管驱动对应PMOS驱动能力调节（0x0~0x3增加驱动能力） */
        unsigned int    reserved_0                 : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK2_REG6_ADJ_T;    /* BUCK2调节6寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck2_ng_n_sel             : 2; /* [1..0] buck2 N power管驱动对应NMOS驱动能力调节（0x0~0x3减小驱动能力)。 */
        unsigned int    reserved_1                 : 2; /* [3..2] 保留 */
        unsigned int    buck2_ng_p_sel             : 2; /* [5..4] buck2 N power管驱动对应PMOS驱动能力调节（0x0~0x3减小驱动能力）。 */
        unsigned int    reserved_0                 : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK2_REG7_ADJ_T;    /* BUCK2调节7寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck2_dbias                : 2; /* [1..0] buck2 比较器偏置电流调节（0x0~0x3增大）。 */
        unsigned int    buck2_ocp_d                : 1; /* [2..2] buck2 内部OCP环路屏蔽信号：0：不屏蔽；1：屏蔽 。 */
        unsigned int    reserved_1                 : 1; /* [3..3] 保留 */
        unsigned int    buck2_ton                  : 2; /* [5..4] buck2最小导通时间。 */
        unsigned int    reserved_0                 : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK2_REG8_ADJ_T;    /* BUCK2调节8寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck2_nmos_off             : 1; /* [0..0] buck2用于关闭NMOS power管：0：强制关闭；1：不强制关闭 。 */
        unsigned int    buck2_reg_c                : 1; /* [1..1] buck2内部regulator电容，用于调节regulator带宽 */
        unsigned int    buck2_short_pdp            : 1; /* [2..2] 短路保护屏蔽寄存器：0:使能短路保护；1:不使能短路保护。 */
        unsigned int    buck2_reg_ss_d             : 1; /* [3..3] buck2软启动时内部regulator状态：0：正常状态；1：单位增益反馈。 */
        unsigned int    buck2_dt_sel               : 2; /* [5..4] buck2死区时间调节（每bit单独控制）：1：不额外延迟 ；0：额外延迟5ns 。 */
        unsigned int    buck2_regop_c              : 1; /* [6..6] buck2内部regulator OP输出电容，用于调节OP的slew-rate。 */
        unsigned int    reserved                   : 1; /* [7..7] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK2_REG9_ADJ_T;    /* BUCK2调节9寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck2_reg_dr               : 3; /* [2..0] buck2内部regulator电阻，用于调节regulator调整精度范围。 */
        unsigned int    buck2_en_reg               : 1; /* [3..3] buck2内部精度调整器使能信号：0：使能；1：不使能。 */
        unsigned int    buck2_ocp_clamp_sel        : 1; /* [4..4] OCP clamp功能的开关:0:开；1：关。 */
        unsigned int    reserved                   : 3; /* [7..5] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK2_REG10_ADJ_T;    /* BUCK2调节10寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck2_reg_idr              : 2; /* [1..0] buck2内部regulator电流，用于调节regulator调整精度范围。 */
        unsigned int    reserved_1                 : 2; /* [3..2] 保留 */
        unsigned int    buck2_reg_r                : 2; /* [5..4] buck2内部regulator电阻，用于调节regulator调整精度范围。 */
        unsigned int    reserved_0                 : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK2_REG11_ADJ_T;    /* BUCK2调节11寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck2_reserve              : 8; /* [7..0] buck2备用寄存器 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK2_REG12_ADJ_T;    /* BUCK2调节12寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck3_adj_rlx              : 4; /* [3..0] buck3 LX反馈滤波电阻调节（每bit单独控制）:0：电阻不接入 ；1：电阻接入。 */
        unsigned int    buck3_adj_clx              : 4; /* [7..4] buck3 LX反馈滤波电容调节（每bit单独控制）:0：电容不接入 ；1：电容接入。 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK3_REG1_ADJ_T;    /* BUCK3调节1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck3_new_dmd_sel          : 5; /* [4..0] 新dmd结构dmd电流调节 */
        unsigned int    buck3_ocp_sel              : 2; /* [6..5] buck3 OCP点调整：0x0: 2.63A ；0x1: 3.08A ；0x2: 3.52A ；0x3: 3.92A 。 */
        unsigned int    reserved                   : 1; /* [7..7] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK3_REG2_ADJ_T;    /* BUCK3调节2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck3_dmd_sel              : 2; /* [1..0] buck3 dmd点选择（0x0~0x3增大）。 */
        unsigned int    buck3_en_regop_clamp       : 1; /* [2..2] regulator 嵌位电路使能：1：使能；0：不使能。 */
        unsigned int    buck3_dmd_clamp            : 1; /* [3..3] 老dmd嵌位使能信号：0：加上；1：不加上。 */
        unsigned int    buck3_ocp_delay_sel        : 1; /* [4..4] ocp屏蔽时间延长20ns信号：0：不延长；1：延长。 */
        unsigned int    buck3_dmd_shield_n         : 1; /* [5..5] 大电流下dmd屏蔽信号：0:屏蔽；1：不屏蔽。 */
        unsigned int    buck3_sleep_dmd            : 1; /* [6..6] 在正常模式下强制NMOS管关闭：0：不强制；1：强制。 */
        unsigned int    buck3_pdn_lx_det           : 1; /* [7..7] 使能LX检测的死区时间控制电路：0：不使能；1：使能。 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK3_REG3_ADJ_T;    /* BUCK3调节3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck3_ng_dt_sel            : 1; /* [0..0] buck3 P管到N管死区模式选择（每bit单独控制）：0：不并入新死区控制模式；1：并入新死区控制模式。 */
        unsigned int    buck3_pg_dt_sel            : 1; /* [1..1] buck3 N管到P管死区模式选择（每bit单独控制）：0：不并入新死区控制模式；1：并入新死区控制模式。 */
        unsigned int    buck3_sft_sel              : 1; /* [2..2] buck3软启动方式选择 */
        unsigned int    reserved_1                 : 1; /* [3..3] 保留 */
        unsigned int    buck3_shield_i             : 2; /* [5..4] dmd屏蔽信号对应电流调节（00~11增大） */
        unsigned int    reserved_0                 : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK3_REG4_ADJ_T;    /* BUCK3调节4寄存器。 */

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
}HI6551_BUCK3_REG5_ADJ_T;    /* BUCK3调节5寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck3_ng_n_sel             : 2; /* [1..0] buck3 N power管驱动对应NMOS驱动能力调节（0x0~0x3减小驱动能力)。 */
        unsigned int    reserved_1                 : 2; /* [3..2] 保留 */
        unsigned int    buck3_ng_p_sel             : 2; /* [5..4] buck3 N power管驱动对应PMOS驱动能力调节（0x0~0x3减小驱动能力）。 */
        unsigned int    reserved_0                 : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK3_REG6_ADJ_T;    /* BUCK3调节6寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck3_dbias                : 2; /* [1..0] buck3 比较器偏置电流调节（0x0~0x3增大）。 */
        unsigned int    buck3_ocp_d                : 1; /* [2..2] buck3 内部OCP环路屏蔽信号：0：不屏蔽；1：屏蔽 。 */
        unsigned int    reserved_1                 : 1; /* [3..3] 保留 */
        unsigned int    buck3_ton                  : 2; /* [5..4] buck3最小导通时间。 */
        unsigned int    reserved_0                 : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK3_REG7_ADJ_T;    /* BUCK3调节7寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck3_nmos_off             : 1; /* [0..0] buck3用于关闭NMOS power管：0：强制关闭；1：不强制关闭 。 */
        unsigned int    buck3_reg_c                : 1; /* [1..1] buck3内部regulator电容，用于调节regulator带宽 */
        unsigned int    buck3_short_pdp            : 1; /* [2..2] 短路保护屏蔽寄存器：0:使能短路保护；1:不使能短路保护。 */
        unsigned int    buck3_reg_ss_d             : 1; /* [3..3] buck3软启动时内部regulator状态：0：正常状态；1：单位增益反馈。 */
        unsigned int    buck3_dt_sel               : 2; /* [5..4] buck3死区时间调节（每bit单独控制）：1：不额外延迟 ；0：额外延迟5ns 。 */
        unsigned int    buck3_regop_c              : 1; /* [6..6] buck3内部regulator OP输出电容，用于调节OP的slew-rate。 */
        unsigned int    reserved                   : 1; /* [7..7] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK3_REG8_ADJ_T;    /* BUCK3调节8寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck3_reg_dr               : 3; /* [2..0] buck3内部regulator电阻，用于调节regulator调整精度范围。 */
        unsigned int    buck3_en_reg               : 1; /* [3..3] buck3内部精度调整器使能信号：0：使能；1：不使能。 */
        unsigned int    buck3_ocp_clamp_sel        : 1; /* [4..4] OCP clamp功能的开关:0:开；1：关。 */
        unsigned int    reserved                   : 3; /* [7..5] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK3_REG9_ADJ_T;    /* BUCK3调节9寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck3_reg_idr              : 2; /* [1..0] buck3内部regulator电流，用于调节regulator调整精度范围。 */
        unsigned int    reserved_1                 : 2; /* [3..2] 保留 */
        unsigned int    buck3_reg_r                : 2; /* [5..4] buck3内部regulator电阻，用于调节regulator调整精度范围。 */
        unsigned int    reserved_0                 : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK3_REG10_ADJ_T;    /* BUCK3调节10寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck3_mos_sel              : 2; /* [1..0] buck3 power管大小选择:11表示power管全部导通 。 */
        unsigned int    reserved                   : 6; /* [7..2] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK3_REG11_ADJ_T;    /* BUCK3调节11寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck3_reserve              : 8; /* [7..0] buck3备用寄存器 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK3_REG12_ADJ_T;    /* BUCK3调节12寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck4_adj_rlx              : 4; /* [3..0] buck4 LX反馈滤波电阻调节（每bit单独控制）:0：电阻不接入 ；1：电阻接入。 */
        unsigned int    buck4_adj_clx              : 4; /* [7..4] buck4 LX反馈滤波电容调节（每bit单独控制）:0：电容不接入 ；1：电容接入。 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK4_REG1_ADJ_T;    /* BUCK4调节1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck4_new_dmd_sel          : 5; /* [4..0] 新dmd结构dmd电流调节 */
        unsigned int    buck4_ocp_sel              : 2; /* [6..5] buck4 OCP点调整：0x0: 1.03A ；0x1: 1.48A ；0x2: 1.92A ；0x3: 2.35A 。 */
        unsigned int    reserved                   : 1; /* [7..7] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK4_REG2_ADJ_T;    /* BUCK4调节2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck4_dmd_sel              : 2; /* [1..0] buck4 dmd点选择（0x0~0x3增大）。 */
        unsigned int    buck4_en_regop_clamp       : 1; /* [2..2] regulator 嵌位电路使能：1：使能；0：不使能。 */
        unsigned int    buck4_dmd_clamp            : 1; /* [3..3] 老dmd嵌位使能信号：0：加上；1：不加上。 */
        unsigned int    buck4_ocp_delay_sel        : 1; /* [4..4] ocp屏蔽时间延长20ns信号：0：不延长；1：延长。 */
        unsigned int    buck4_dmd_shield_n         : 1; /* [5..5] 大电流下dmd屏蔽信号：0:屏蔽；1：不屏蔽。 */
        unsigned int    buck4_sleep_dmd            : 1; /* [6..6] 在正常模式下强制NMOS管关闭：0：不强制；1：强制。 */
        unsigned int    buck4_pdn_lx_det           : 1; /* [7..7] 使能LX检测的死区时间控制电路：0：不使能；1：使能。 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK4_REG3_ADJ_T;    /* BUCK4调节3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck4_ng_dt_sel            : 1; /* [0..0] buck4 P管到N管死区模式选择（每bit单独控制）：0：不并入新死区控制模式；1：并入新死区控制模式。 */
        unsigned int    buck4_pg_dt_sel            : 1; /* [1..1] buck4 N管到P管死区模式选择（每bit单独控制）：0：不并入新死区控制模式；1：并入新死区控制模式。 */
        unsigned int    buck4_sft_sel              : 1; /* [2..2] buck4软启动方式选择 */
        unsigned int    reserved                   : 5; /* [7..3] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK4_REG4_ADJ_T;    /* BUCK4调节4寄存器。 */

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
}HI6551_BUCK4_REG5_ADJ_T;    /* BUCK4调节5寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck4_ng_n_sel             : 2; /* [1..0] buck4 N power管驱动对应NMOS驱动能力调节（0x0~0x3减小驱动能力)。 */
        unsigned int    reserved_1                 : 2; /* [3..2] 保留 */
        unsigned int    buck4_ng_p_sel             : 2; /* [5..4] buck4 N power管驱动对应PMOS驱动能力调节（0x0~0x3减小驱动能力）。 */
        unsigned int    reserved_0                 : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK4_REG6_ADJ_T;    /* BUCK4调节6寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck4_dbias                : 2; /* [1..0] buck4 比较器偏置电流调节（0x0~0x3增大）。 */
        unsigned int    buck4_ocp_d                : 1; /* [2..2] buck4 内部OCP环路屏蔽信号：0：不屏蔽；1：屏蔽 。 */
        unsigned int    reserved_1                 : 1; /* [3..3] 保留 */
        unsigned int    buck4_ton                  : 2; /* [5..4] buck4最小导通时间。 */
        unsigned int    reserved_0                 : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK4_REG7_ADJ_T;    /* BUCK4调节7寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck4_nmos_off             : 1; /* [0..0] buck4用于关闭NMOS power管：0：强制关闭；1：不强制关闭 。 */
        unsigned int    buck4_reg_c                : 1; /* [1..1] buck4内部regulator电容，用于调节regulator带宽 */
        unsigned int    buck4_short_pdp            : 1; /* [2..2] 短路保护屏蔽寄存器：0:使能短路保护；1:不使能短路保护。 */
        unsigned int    buck4_reg_ss_d             : 1; /* [3..3] buck4软启动时内部regulator状态：0：正常状态；1：单位增益反馈。 */
        unsigned int    buck4_dt_sel               : 2; /* [5..4] buck4死区时间调节（每bit单独控制）：1：不额外延迟 ；0：额外延迟5ns 。 */
        unsigned int    buck4_regop_c              : 1; /* [6..6] buck4内部regulator OP输出电容，用于调节OP的slew-rate。 */
        unsigned int    reserved                   : 1; /* [7..7] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK4_REG8_ADJ_T;    /* BUCK4调节8寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck4_reg_dr               : 3; /* [2..0] buck4内部regulator电阻，用于调节regulator调整精度范围。 */
        unsigned int    buck4_en_reg               : 1; /* [3..3] buck4内部精度调整器使能信号：0：使能；1：不使能。 */
        unsigned int    buck4_ocp_clamp_sel        : 1; /* [4..4] OCP clamp功能的开关:0:开；1：关。 */
        unsigned int    reserved                   : 3; /* [7..5] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK4_REG9_ADJ_T;    /* BUCK4调节9寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck4_reg_idr              : 2; /* [1..0] buck4内部regulator电流，用于调节regulator调整精度范围。 */
        unsigned int    reserved_1                 : 2; /* [3..2] 保留 */
        unsigned int    buck4_reg_r                : 2; /* [5..4] buck4内部regulator电阻，用于调节regulator调整精度范围。 */
        unsigned int    reserved_0                 : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK4_REG10_ADJ_T;    /* BUCK4调节10寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck4_reserve              : 8; /* [7..0] buck4备用寄存器 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK4_REG12_ADJ_T;    /* BUCK4调节12寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck5_adj_rlx              : 4; /* [3..0] buck5 LX反馈滤波电阻调节（每bit单独控制）:0：电阻不接入 ；1：电阻接入。 */
        unsigned int    buck5_adj_clx              : 4; /* [7..4] buck5 LX反馈滤波电容调节（每bit单独控制）:0：电容不接入 ；1：电容接入。 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK5_REG1_ADJ_T;    /* BUCK5调节1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck5_new_dmd_sel          : 5; /* [4..0] 新dmd结构dmd电流调节 */
        unsigned int    buck5_ocp_sel              : 2; /* [6..5] buck5 OCP点调整：0x0: 0.57A ；0x1: 0.79A ；0x2: 1.02A ；0x3: 1.22A 。 */
        unsigned int    reserved                   : 1; /* [7..7] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK5_REG2_ADJ_T;    /* BUCK5调节2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck5_dmd_sel              : 2; /* [1..0] buck5 dmd点选择（0x0~0x3增大）。 */
        unsigned int    buck5_en_regop_clamp       : 1; /* [2..2] regulator 嵌位电路使能：1：使能；0：不使能。 */
        unsigned int    buck5_dmd_clamp            : 1; /* [3..3] 老dmd嵌位使能信号：0：加上；1：不加上。 */
        unsigned int    buck5_ocp_delay_sel        : 1; /* [4..4] ocp屏蔽时间延长20ns信号：0：不延长；1：延长。 */
        unsigned int    buck5_dmd_shield_n         : 1; /* [5..5] 大电流下dmd屏蔽信号：0:屏蔽；1：不屏蔽。 */
        unsigned int    buck5_sleep_dmd            : 1; /* [6..6] 在正常模式下强制NMOS管关闭：0：不强制；1：强制。 */
        unsigned int    buck5_pdn_lx_det           : 1; /* [7..7] 使能LX检测的死区时间控制电路：0：不使能；1：使能。 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK5_REG3_ADJ_T;    /* BUCK5调节3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck5_ng_dt_sel            : 1; /* [0..0] buck5 P管到N管死区模式选择（每bit单独控制）：0：不并入新死区控制模式；1：并入新死区控制模式。 */
        unsigned int    buck5_pg_dt_sel            : 1; /* [1..1] buck5 N管到P管死区模式选择（每bit单独控制）：0：不并入新死区控制模式；1：并入新死区控制模式。 */
        unsigned int    buck5_sft_sel              : 1; /* [2..2] buck5软启动方式选择 */
        unsigned int    reserved                   : 5; /* [7..3] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK5_REG4_ADJ_T;    /* BUCK5调节4寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck5_pg_n_sel             : 2; /* [1..0] buck5 P power管驱动对应NMOS驱动能力调节（0x0~0x3减小驱动能力）。 */
        unsigned int    reserved_1                 : 2; /* [3..2] 保留 */
        unsigned int    buck5_pg_p_sel             : 2; /* [5..4] buck5 P power管驱动对应PMOS驱动能力调节（0x0~0x3增加驱动能力） */
        unsigned int    reserved_0                 : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK5_REG5_ADJ_T;    /* BUCK5调节5寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck5_ng_n_sel             : 2; /* [1..0] buck5 N power管驱动对应NMOS驱动能力调节（0x0~0x3减小驱动能力)。 */
        unsigned int    reserved_1                 : 2; /* [3..2] 保留 */
        unsigned int    buck5_ng_p_sel             : 2; /* [5..4] buck5 N power管驱动对应PMOS驱动能力调节（0x0~0x3减小驱动能力）。 */
        unsigned int    reserved_0                 : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK5_REG6_ADJ_T;    /* BUCK5调节6寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck5_dbias                : 2; /* [1..0] buck5 比较器偏置电流调节（0x0~0x3增大）。 */
        unsigned int    buck5_ocp_d                : 1; /* [2..2] buck5 内部OCP环路屏蔽信号：0：不屏蔽；1：屏蔽 。 */
        unsigned int    reserved_1                 : 1; /* [3..3] 保留 */
        unsigned int    buck5_ton                  : 2; /* [5..4] buck5最小导通时间。 */
        unsigned int    reserved_0                 : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK5_REG7_ADJ_T;    /* BUCK5调节7寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck5_nmos_off             : 1; /* [0..0] buck5用于关闭NMOS power管：0：强制关闭；1：不强制关闭 。 */
        unsigned int    buck5_reg_c                : 1; /* [1..1] buck5内部regulator电容，用于调节regulator带宽 */
        unsigned int    buck5_short_pdp            : 1; /* [2..2] 短路保护屏蔽寄存器：0:使能短路保护；1:不使能短路保护。 */
        unsigned int    buck5_reg_ss_d             : 1; /* [3..3] buck5软启动时内部regulator状态：0：正常状态；1：单位增益反馈。 */
        unsigned int    buck5_dt_sel               : 2; /* [5..4] buck5死区时间调节（每bit单独控制）：1：不额外延迟 ；0：额外延迟5ns 。 */
        unsigned int    buck5_regop_c              : 1; /* [6..6] buck5内部regulator OP输出电容，用于调节OP的slew-rate。 */
        unsigned int    reserved                   : 1; /* [7..7] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK5_REG8_ADJ_T;    /* BUCK5调节8寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck5_reg_dr               : 3; /* [2..0] buck5内部regulator电阻，用于调节regulator调整精度范围。 */
        unsigned int    buck5_en_reg               : 1; /* [3..3] buck5内部精度调整器使能信号：0：使能；1：不使能。 */
        unsigned int    buck5_ocp_clamp_sel        : 1; /* [4..4] OCP clamp功能的开关:0:开；1：关。 */
        unsigned int    reserved                   : 3; /* [7..5] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK5_REG9_ADJ_T;    /* BUCK5调节9寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck5_reg_idr              : 2; /* [1..0] buck5内部regulator电流，用于调节regulator调整精度范围。 */
        unsigned int    reserved_1                 : 2; /* [3..2] 保留 */
        unsigned int    buck5_reg_r                : 2; /* [5..4] buck5内部regulator电阻，用于调节regulator调整精度范围。 */
        unsigned int    reserved_0                 : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK5_REG10_ADJ_T;    /* BUCK5调节10寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck5_reserve0             : 8; /* [7..0] buck5备用寄存器 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK5_REG12_ADJ_T;    /* BUCK5调节12寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck5_reserve1             : 8; /* [7..0] buck5备用寄存器 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK5_REG13_ADJ_T;    /* BUCK5调节13寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck5_reserve2             : 8; /* [7..0] buck5备用寄存器 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK5_REG14_ADJ_T;    /* BUCK5调节14寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck6_adj_rlx              : 4; /* [3..0] buck6 LX反馈滤波电阻调节（每bit单独控制）:0：电阻不接入 ；1：电阻接入。 */
        unsigned int    buck6_adj_clx              : 4; /* [7..4] buck6 LX反馈滤波电容调节（每bit单独控制）:0：电容不接入 ；1：电容接入。 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK6_REG1_ADJ_T;    /* BUCK6调节1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck6_new_dmd_sel          : 5; /* [4..0] 新dmd结构dmd电流调节 */
        unsigned int    buck6_ocp_sel              : 2; /* [6..5] buck6 OCP点调整：0x0: 1.09A ；0x1: 1.58A ；0x2: 1.92A ；0x3: 2.40A 。 */
        unsigned int    reserved                   : 1; /* [7..7] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK6_REG2_ADJ_T;    /* BUCK6调节2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck6_dmd_sel              : 2; /* [1..0] buck6 dmd点选择（0x0~0x3增大）。 */
        unsigned int    buck6_en_regop_clamp       : 1; /* [2..2] regulator 嵌位电路使能：1：使能；0：不使能。 */
        unsigned int    buck6_dmd_clamp            : 1; /* [3..3] 老dmd嵌位使能信号：0：加上；1：不加上。 */
        unsigned int    buck6_ocp_delay_sel        : 1; /* [4..4] ocp屏蔽时间延长20ns信号：0：不延长；1：延长。 */
        unsigned int    buck6_dmd_shield_n         : 1; /* [5..5] 大电流下dmd屏蔽信号：0:屏蔽；1：不屏蔽。 */
        unsigned int    buck6_sleep_dmd            : 1; /* [6..6] 在正常模式下强制NMOS管关闭：0：不强制；1：强制。 */
        unsigned int    buck6_pdn_lx_det           : 1; /* [7..7] 使能LX检测的死区时间控制电路：0：不使能；1：使能。 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK6_REG3_ADJ_T;    /* BUCK6调节3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck6_ng_dt_sel            : 1; /* [0..0] buck6 P管到N管死区模式选择（每bit单独控制）：0：不并入新死区控制模式；1：并入新死区控制模式。 */
        unsigned int    buck6_pg_dt_sel            : 1; /* [1..1] buck6 N管到P管死区模式选择（每bit单独控制）：0：不并入新死区控制模式；1：并入新死区控制模式。 */
        unsigned int    buck6_sft_sel              : 1; /* [2..2] buck6软启动方式选择 */
        unsigned int    reserved                   : 5; /* [7..3] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK6_REG4_ADJ_T;    /* BUCK6调节4寄存器。 */

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
}HI6551_BUCK6_REG5_ADJ_T;    /* BUCK6调节5寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck6_ng_n_sel             : 2; /* [1..0] buck6 N power管驱动对应NMOS驱动能力调节（0x0~0x3减小驱动能力)。 */
        unsigned int    reserved_1                 : 2; /* [3..2] 保留 */
        unsigned int    buck6_ng_p_sel             : 2; /* [5..4] buck6 N power管驱动对应PMOS驱动能力调节（0x0~0x3减小驱动能力）。 */
        unsigned int    reserved_0                 : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK6_REG6_ADJ_T;    /* BUCK6调节6寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck6_dbias                : 2; /* [1..0] buck6 比较器偏置电流调节（0x0~0x3增大）。 */
        unsigned int    buck6_ocp_d                : 1; /* [2..2] buck6 内部OCP环路屏蔽信号：0：不屏蔽；1：屏蔽 。 */
        unsigned int    reserved_1                 : 1; /* [3..3] 保留 */
        unsigned int    buck6_ton                  : 2; /* [5..4] buck6最小导通时间。 */
        unsigned int    reserved_0                 : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK6_REG7_ADJ_T;    /* BUCK6调节7寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck6_nmos_off             : 1; /* [0..0] buck6用于关闭NMOS power管：0：强制关闭；1：不强制关闭 。 */
        unsigned int    buck6_reg_c                : 1; /* [1..1] buck6内部regulator电容，用于调节regulator带宽 */
        unsigned int    buck6_short_pdp            : 1; /* [2..2] 短路保护屏蔽寄存器：0:使能短路保护；1:不使能短路保护。 */
        unsigned int    buck6_reg_ss_d             : 1; /* [3..3] buck6软启动时内部regulator状态：0：正常状态；1：单位增益反馈。 */
        unsigned int    buck6_dt_sel               : 2; /* [5..4] buck6死区时间调节（每bit单独控制）：1：不额外延迟 ；0：额外延迟5ns 。 */
        unsigned int    buck6_regop_c              : 1; /* [6..6] buck6内部regulator OP输出电容，用于调节OP的slew-rate。 */
        unsigned int    reserved                   : 1; /* [7..7] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK6_REG8_ADJ_T;    /* BUCK6调节8寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck6_reg_dr               : 3; /* [2..0] buck6内部regulator电阻，用于调节regulator调整精度范围。 */
        unsigned int    buck6_en_reg               : 1; /* [3..3] buck6内部精度调整器使能信号：0：使能；1：不使能。 */
        unsigned int    buck6_ocp_clamp_sel        : 1; /* [4..4] OCP clamp功能的开关:0:开；1：关。 */
        unsigned int    reserved                   : 3; /* [7..5] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK6_REG9_ADJ_T;    /* BUCK6调节9寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck6_reg_idr              : 2; /* [1..0] buck6内部regulator电流，用于调节regulator调整精度范围。 */
        unsigned int    reserved_1                 : 2; /* [3..2] 保留 */
        unsigned int    buck6_reg_r                : 2; /* [5..4] buck6内部regulator电阻，用于调节regulator调整精度范围。 */
        unsigned int    reserved_0                 : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK6_REG10_ADJ_T;    /* BUCK6调节10寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck6_mos_sel              : 2; /* [1..0] buck6 power管大小选择:11表示power管全部导通 。 */
        unsigned int    reserved                   : 6; /* [7..2] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK6_REG11_ADJ_T;    /* BUCK6调节11寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck6_reserve              : 8; /* [7..0] buck6备用寄存器 */
    } bits;
    unsigned int    u32;
}HI6551_BUCK6_REG12_ADJ_T;    /* BUCK6调节12寄存器。 */

typedef union
{
    struct
    {
        unsigned int    cp2_vout_sel               : 2; /* [1..0] CHARGE PUMP2的输出电平选择信号0x0 : 7.00V ；0x1 ：6.82V ；0x2 ：6.66V ；0x3 ：6.350V 。 */
        unsigned int    reserved                   : 6; /* [7..2] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_CHG_PUMP2_ADJ_T;    /* CHG_PUMP2调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck0_vck                  : 6; /* [5..0] buck01输出控制默认输出0.9V64Steps7.9365mV/step要求电压曲线必须单调。0x19: 0.90V；0x26: 1.00V；OTP可编程默认电压：otp_vset:0x0 : 0.90V ；0x1 : 1.00V 。 */
        unsigned int    reserved                   : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_VSET_BUCK01_ADJ_T;    /* BUCK01电压调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck2_dv                   : 8; /* [7..0] buck2输出电压调节.0x00: 0.70V；0x3f: 1.20V；8mv/step。0x7f：1.6V ；0xff:1.8V ；0x19: 0.90V；0x26: 1.00V；OTP可编程默认电压：otp_vset:0x0 : 0.90V ；0x1 :1.00V 。 */
    } bits;
    unsigned int    u32;
}HI6551_VSET_BUCK2_ADJ_T;    /* BUCK2电压调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck3_dv                   : 4; /* [3..0] BUCK3输出电压调节信号：0x6: 0.90V；0xa: 1.00V；OTP可编程默认电压：otp_vset:0x0 : 0.90V ；0x1 : 1.00V 。 */
        unsigned int    reserved                   : 4; /* [7..4] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_VSET_BUCK3_ADJ_T;    /* BUCK3电压调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck4_dv                   : 4; /* [3..0] buck4输出电压调节:      0x0:1.20V;                   0xF:1.575V               25mv/step       默认输出电压：1.225V */
        unsigned int    reserved                   : 4; /* [7..4] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_VSET_BUCK4_ADJ_T;    /* BUCK3电压调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck5_dv                   : 8; /* [7..0] buck5输出电压调节: 0x0:0.7V;                   0x63:1.204V;0x127:1.65V; 0x255:1.8V8mv/step默认输出电压：0.9V */
    } bits;
    unsigned int    u32;
}HI6551_VSET_BUCK5_ADJ_T;    /* BUCK5电压调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    buck6_dv                   : 4; /* [3..0] buck6输出电压调节:0x0:1.8V;0xF:2.175V;                        25mv/step 默认输出电压：1.825V */
        unsigned int    reserved                   : 4; /* [7..4] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_VSET_BUCK6_ADJ_T;    /* BUCK6电压调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    vset_ldo1                  : 3; /* [2..0] LDO1电压调节：0x0: 1.80V ；0x1: 2.50V ；0x2: 2.60V ；0x3: 2.70V ；0x4: 2.80V ；0x5: 2.85V ； 0x6: 2.90V ；0x7: 3.00V。 */
        unsigned int    reserved                   : 5; /* [7..3] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_LDO1_REG_ADJ_T;    /* LDO1调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    vset_ldo2                  : 3; /* [2..0] LDO2电压调节：0x0: 2.50V ；0x1: 2.60V ；0x2: 2.80V ；0x3: 2.90V ；0x4: 3.00V ；0x5: 3.10V ；0x6: 3.20V ；0x7: 3.30V 。 */
        unsigned int    reserved                   : 5; /* [7..3] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_LDO2_REG_ADJ_T;    /* LDO2调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    vset_ldo3                  : 3; /* [2..0] LDO3电压调节：0x0:0.80V ；0x1: 0.85V ；0x2: 0.90V ；0x3: 0.95V ；0x4: 1.00V 。OTP可编程默认电压：otp_vset:0x0 : 0.9V ；0x1 : 1.00V 。Pmu上电时ldo3_batt为0.85V，当ldo3_buck上电时vset_ldo3 otp没烧写为0.9V，烧写后为1.0V。 */
        unsigned int    reserved_1                 : 1; /* [3..3] 保留 */
        unsigned int    vrset_ldo3                 : 3; /* [6..4] LDO3负载调整率补偿调整，5mV / Step */
        unsigned int    reserved_0                 : 1; /* [7..7] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_LDO3_REG_ADJ_T;    /* LDO3调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    vset_ldo4                  : 3; /* [2..0] LDO4电压调节：0x0: 1.50V ；0x1: 1.80V ；0x2: 1.85V ；0x3: 2.20V ；0x4: 2.50V ；0x5: 2.80V 。 */
        unsigned int    reserved                   : 5; /* [7..3] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_LDO4_REG_ADJ_T;    /* LDO4调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    vset_ldo5                  : 3; /* [2..0] LDO5电压调节：0x0: 2.85V ；0x1: 2.90V ；0x2: 2.95V ；0x3: 3.00V ；0x4: 3.05V ；0x5: 3.10V ；0x6: 3.15V ；0x7: 3.15V 。 */
        unsigned int    reserved_1                 : 1; /* [3..3] 保留 */
        unsigned int    vrset_ldo5                 : 3; /* [6..4] LDO3负载调整率补偿调整，5mV / Step */
        unsigned int    reserved_0                 : 1; /* [7..7] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_LDO5_REG_ADJ_T;    /* LDO5调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    vset_ldo6                  : 3; /* [2..0] LDO6电压调节：0x0: 1.50V ；0x1: 1.80V ；0x2: 1.85V ；0x3: 2.20V ；0x4: 2.50V ；0x5: 2.80V 。 */
        unsigned int    reserved                   : 5; /* [7..3] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_LDO6_REG_ADJ_T;    /* LDO6调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    vset_ldo7                  : 3; /* [2..0] LDO7电压调节：0x0: 1.80V ；0x1: 1.85V ；0x2: 2.80V ；0x3: 2.85V ；0x4: 2.90V ；0x5: 2.95V ；0x6: 3.00V ；0x7: 3.05V 。 */
        unsigned int    reserved                   : 5; /* [7..3] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_LDO7_REG_ADJ_T;    /* LDO7调节寄存器。 */

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
}HI6551_LDO8_REG_ADJ_T;    /* LDO8调节寄存器。 */

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
}HI6551_LDO9_REG_ADJ_T;    /* LDO9调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    vset_ldo10                 : 3; /* [2..0] LDO10电压调节：0x0: 2.80V ；0x1: 2.85V ；0x2: 2.90V ；0x3: 2.95V ；0x4: 3.00V ；0x5: 3.05V ；0x6: 3.10V ；0x7: 3.15V 。 */
        unsigned int    reserved_1                 : 1; /* [3..3] 保留 */
        unsigned int    vrset_ldo10                : 3; /* [6..4] LDO10负载调整率补偿调整，5mV / Step */
        unsigned int    reserved_0                 : 1; /* [7..7] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_LDO10_REG_ADJ_T;    /* LDO10调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    vset_ldo11                 : 3; /* [2..0] LDO11电压调节：0x0: 1.80V ；0x1: 1.85V ；0x2: 2.80V ；0x3: 2.85V ；0x4: 2.90V ；0x5: 2.95V ；0x6: 3.00V ；0x7: 3.05V 。 */
        unsigned int    reserved_1                 : 1; /* [3..3] 保留 */
        unsigned int    vrset_ldo11                : 3; /* [6..4] LDO11负载调整率补偿调整，5mV / Step */
        unsigned int    reserved_0                 : 1; /* [7..7] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_LDO11_REG_ADJ_T;    /* LDO11调节寄存器。 */

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
}HI6551_LDO12_REG_ADJ_T;    /* LDO12调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    vset_ldo13                 : 3; /* [2..0] LDO13电压调节：0x0: 1.80V ；0x1: 2.50V ；0x2: 2.60V ；0x3: 2.70V ；0x4: 2.80V ；0x5: 2.85V ； 0x6: 2.90V ；0x7: 3.00V。 */
        unsigned int    reserved                   : 5; /* [7..3] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_LDO13_REG_ADJ_T;    /* LDO13调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    vset_ldo14                 : 3; /* [2..0] LDO14电压调节：0x0: 1.80V ；0x1: 2.50V ；0x2: 2.60V ；0x3: 2.70V ；0x4: 2.80V ；0x5: 2.85V ； 0x6: 2.90V ；0x7: 3.00V。 */
        unsigned int    reserved_1                 : 1; /* [3..3] 保留 */
        unsigned int    vrset_ldo14                : 3; /* [6..4] LDO14负载调整率补偿调整，5mV / Step */
        unsigned int    reserved_0                 : 1; /* [7..7] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_LDO14_REG_ADJ_T;    /* LDO14调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    vset_ldo15                 : 3; /* [2..0] LDO15电压调节：0x0: 1.80V ；0x1: 2.50V ；0x2: 2.60V ；0x3: 2.70V ；0x4: 2.80V ；0x5: 2.85V ； 0x6: 2.90V ；0x7: 3.00V。 */
        unsigned int    reserved_1                 : 1; /* [3..3] 保留 */
        unsigned int    vrset_ldo15                : 3; /* [6..4] LDO15负载调整率补偿调整，5mV / Step */
        unsigned int    reserved_0                 : 1; /* [7..7] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_LDO15_REG_ADJ_T;    /* LDO15调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    vset_ldo16                 : 3; /* [2..0] LDO16电压调节：0x0: 1.80V ；0x1: 2.50V ；0x2: 2.60V ；0x3: 2.70V ；0x4: 2.80V ；0x5: 2.85V ； 0x6: 2.90V ；0x7: 3.00V。 */
        unsigned int    reserved_1                 : 1; /* [3..3] 保留 */
        unsigned int    vrset_ldo16                : 3; /* [6..4] LDO16负载调整率补偿调整，5mV / Step */
        unsigned int    reserved_0                 : 1; /* [7..7] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_LDO16_REG_ADJ_T;    /* LDO16调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    vset_ldo17                 : 3; /* [2..0] LDO17电压调节：0x0: 0.90V ；0x1: 0.95V ；0x2: 1.00V ；0x3: 1.05V ；0x4: 1.10V ；0x5: 1.15V ； 0x6: 1.20V ；0x7: 1.25V。 */
        unsigned int    reserved_1                 : 1; /* [3..3] 保留 */
        unsigned int    vrset_ldo17                : 3; /* [6..4] LDO17负载调整率补偿调整，5mV / Step */
        unsigned int    reserved_0                 : 1; /* [7..7] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_LDO17_REG_ADJ_T;    /* LDO17调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    vset_ldo18                 : 3; /* [2..0] LDO18电压调节：0x0: 1.80V ；0x1: 2.50V ；0x2: 2.60V ；0x3: 2.70V ；0x4: 2.80V ；0x5: 2.85V ； 0x6: 2.90V ；0x7: 3.00V 。 */
        unsigned int    reserved_1                 : 1; /* [3..3] 保留 */
        unsigned int    vrset_ldo18                : 3; /* [6..4] LDO18负载调整率补偿调整，5mV / Step */
        unsigned int    reserved_0                 : 1; /* [7..7] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_LDO18_REG_ADJ_T;    /* LDO18调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    vset_ldo19                 : 3; /* [2..0] LDO19电压调节：0x0: 0.900V ；0x1: 0.925V ；0x2: 0.950V ；0x3: 0.975V ；0x4: 1.000V ；0x5: 1.025V ；0x6: 1.050V ；0x7: 1.075V 。 */
        unsigned int    reserved_1                 : 1; /* [3..3] 保留 */
        unsigned int    vrset_ldo19                : 3; /* [6..4] LDO19负载调整率补偿调整，5mV / Step */
        unsigned int    reserved_0                 : 1; /* [7..7] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_LDO19_REG_ADJ_T;    /* LDO19调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    vset_ldo20                 : 3; /* [2..0] LDO20电压调节：0x0: 2.50V ；0x1: 2.60V ；0x2: 2.70V ；0x3: 2.75V ；0x4: 2.80V ；0x5: 2.85V ； 0x6: 3.00V；0x7: 3.10V。 */
        unsigned int    reserved_1                 : 1; /* [3..3] 保留 */
        unsigned int    vrset_ldo20                : 3; /* [6..4] LDO20负载调整率补偿调整，5mV / Step */
        unsigned int    reserved_0                 : 1; /* [7..7] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_LDO20_REG_ADJ_T;    /* LDO20调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    vset_ldo21                 : 3; /* [2..0] LDO21电压调节：0x0: 1.20V ；0x1: 1.25V ；0x2: 1.275V ；0x3: 1.30V ；0x4: 1.325V ；0x5: 1.35V ；0x6: 1.50V ；0x7：1.80V 。 */
        unsigned int    reserved_1                 : 1; /* [3..3] 保留 */
        unsigned int    vrset_ldo21                : 3; /* [6..4] LDO21负载调整率补偿调整，5mV / Step */
        unsigned int    reserved_0                 : 1; /* [7..7] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_LDO21_REG_ADJ_T;    /* LDO21调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    vset_ldo22                 : 3; /* [2..0] LDO22电压调节：0x0: 1.40V ；0x1: 1.425V ；0x2: 1.45V ；0x3: 1.475V ；0x4: 1.50V ； 0x5: 1.525V。 */
        unsigned int    en_sink_ldo22              : 1; /* [3..3] 使能LDO22电阻下拉：0:代表没有电阻下拉；1：代表有电阻下拉 。 */
        unsigned int    rset_sink_ldo22            : 2; /* [5..4] 设置LDO22下拉电阻：00电阻最大，11电阻最小。默认值10。 */
        unsigned int    reserved                   : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_LDO22_REG_ADJ_T;    /* LDO22调节寄存器。 */

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
}HI6551_LDO23_REG_ADJ_T;    /* LDO23调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    vset_ldo24                 : 3; /* [2..0] LDO24电压调节：0x0: 2.80V ；0x1: 2.85V ；0x2: 2.90V ；0x3: 2.95V ；0x4: 3.00V ；0x5: 3.05V ；0x6: 3.10V ；0x7: 3.15V 。 */
        unsigned int    reserved                   : 5; /* [7..3] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_LDO24_REG_ADJ_T;    /* LDO24调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    vset_pmua                  : 3; /* [2..0] PMUA电压调节：0x0: 2.90V ；0x1: 3.00V ；0x2: 3.05V ；0x3: 3.10V ；0x4: 3.15V ； 0x5: 3.20V ；0x6: 3.25V ；0x7: 3.30V。 */
        unsigned int    eco_pmua                   : 1; /* [3..3] PMUA进入ECO模式控制信号:0：代表normal模式；1：代表eco模式。 */
        unsigned int    reserved                   : 4; /* [7..4] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_PMUA_REG_ADJ_T;    /* PMUA调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    sstset_lvs5                : 2; /* [1..0] LVS5缓启速度调节，11最快，00最慢 */
        unsigned int    sstset_lvs4                : 2; /* [3..2] LVS4缓启速度调节，11最快，00最慢 */
        unsigned int    sstset_lvs3                : 2; /* [5..4] LVS3缓启速度调节，11最快，00最慢 */
        unsigned int    sstset_lvs2                : 2; /* [7..6] LVS2缓启速度调节，11最快，00最慢 */
    } bits;
    unsigned int    u32;
}HI6551_LVS_ADJ1_T;    /* LVS调节1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    sstset_lvs9                : 2; /* [1..0] LVS9缓启速度调节，11最快，00最慢 */
        unsigned int    sstset_lvs8                : 2; /* [3..2] LVS8缓启速度调节，11最快，00最慢 */
        unsigned int    sstset_lvs7                : 2; /* [5..4] LVS7缓启速度调节，11最快，00最慢 */
        unsigned int    sstset_lvs6                : 2; /* [7..6] LVS6缓启速度调节，11最快，00最慢 */
    } bits;
    unsigned int    u32;
}HI6551_LVS_ADJ2_T;    /* LVS调节2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    sstset_lvs10               : 2; /* [1..0] LVS10缓启速度调节，11最快，00最慢 */
        unsigned int    reserved                   : 6; /* [7..2] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_LVS_ADJ3_T;    /* LVS调节3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    en_bst_int                 : 1; /* [0..0] 0: BOOST关闭 ；1: BOOST开启 。 */
        unsigned int    bst_pm_th                  : 1; /* [1..1] 0: PMOS不被此寄存器控制 ；1: PMOS强行直通，所有内部电路关闭。 */
        unsigned int    bst_pm_cut                 : 1; /* [2..2] 0: PMOS不被此寄存器控制 ；1: PMOS强行关闭 。 */
        unsigned int    bst_reserved0              : 5; /* [7..3] 测试用 及备用未定 */
    } bits;
    unsigned int    u32;
}HI6551_BOOST_ADJ0_T;    /* BOOST调节0寄存器。 */

typedef union
{
    struct
    {
        unsigned int    bst_sel_scp                : 1; /* [0..0] 0: 1ms内VOUT不足0.85BAT，上报短路；1: 2ms内VOUT不足0.85BAT，上报短路。 */
        unsigned int    bst_sel_pd                 : 1; /* [1..1] 0: 关闭下电PMOS控制电路 ；1: 使能下电PMOS控制电路 。 */
        unsigned int    bst_en_uvp                 : 1; /* [2..2] 0: 关闭欠压电路（不上报）；1: 使能欠压电路（输出电压低于设定20%）。 */
        unsigned int    bst_en_scp                 : 1; /* [3..3] 0: 关闭短路系统自动关闭（仍上报）；1: 使能短路系统自动关闭。 */
        unsigned int    bst_en_ovp                 : 1; /* [4..4] 0: 关闭过压电路（不上报）；1: 使能过压电路（输出电压超过设定20%）。 */
        unsigned int    bst_en_ocp                 : 1; /* [5..5] 0: 关闭过流环路控制（仍然上报）；1: 使能过流环路控制。 */
        unsigned int    bst_reserved1              : 2; /* [7..6] 测试用 及备用未定 */
    } bits;
    unsigned int    u32;
}HI6551_BOOST_ADJ1_T;    /* BOOST调节1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    bst_drv_mode               : 1; /* [0..0] 0: 最小死区模式；1: 传统死区检测模式。 */
        unsigned int    bst_loop_mode              : 1; /* [1..1] 0:电流模式；1:电压模式。 */
        unsigned int    bst_en_pfm                 : 1; /* [2..2] 0: 关闭输出电压方式的PFM模式；1: 使能输出电压方式的PFM模式。 */
        unsigned int    bst_en_nring               : 1; /* [3..3] 0: 关闭短振铃；1: 使能防振铃。 */
        unsigned int    bst_en_clp_os              : 1; /* [4..4] 0: 关闭钳位电路失调电压；1: 使能钳位电路失调电压。 */
        unsigned int    bst_en_slop                : 1; /* [5..5] 0: 关闭斜坡补偿；1: 使能斜坡补偿。 */
        unsigned int    bst_en_dmd                 : 1; /* [6..6] 0: 关闭过零检测；1: 使能过零检测。 */
        unsigned int    bst_reserved2              : 1; /* [7..7] 测试用 及备用未定。 */
    } bits;
    unsigned int    u32;
}HI6551_BOOST_ADJ2_T;    /* BOOST调节2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    bst_clp                    : 3; /* [2..0] EA输出钳位值和VRAMP直流电平值的失调000: 8mV001: 12mV010: 16mV011: 20mV100: 24mV101: 28mV110: 32mV111: 36mV */
        unsigned int    bst_itail                  : 1; /* [3..3] EA输出级静态电流控制0: 2.5uA ；1:  5.5uA 。 */
        unsigned int    bst_gm                     : 3; /* [6..4] EA中gm电阻设定(gm=2/R)000: 160kΩ001: 140kΩ010: 120kΩ011: 100kΩ100: 40kΩ101: 30kΩ110: 20kΩ111: 10kΩ */
        unsigned int    bst_reserved3              : 1; /* [7..7] 测试用 及备用未定 */
    } bits;
    unsigned int    u32;
}HI6551_BOOST_ADJ3_T;    /* BOOST调节3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    bst_ccom2                  : 2; /* [1..0] EA输出滤波电容00: 0.8pF01: 1.1pF10: 1.4pF11: 1.7pF */
        unsigned int    bst_ccom1                  : 2; /* [3..2] 主环路补偿电容00: 20pF01: 40pF10: 50pF11: 60pF */
        unsigned int    bst_rcom                   : 2; /* [5..4] 主环路补偿电阻00: 140kΩ01: 120kΩ10: 100kΩ11: 80kΩ */
        unsigned int    bst_reserved4              : 2; /* [7..6] 测试用 及备用未定 */
    } bits;
    unsigned int    u32;
}HI6551_BOOST_ADJ4_T;    /* BOOST调节4寄存器。 */

typedef union
{
    struct
    {
        unsigned int    bst_slop                   : 2; /* [1..0] 斜坡补偿斜率调节（电容调节）00: 2.0pF01: 2.5pF10: 3.0pF11: 3.5pF */
        unsigned int    bst_ri                     : 2; /* [3..2] 电感电流转电压阻抗00: 0.75Ω（实际120kΩ）01: 0.625Ω（实际100kΩ）10: 0.5Ω（实际80kΩ）11: 0.375Ω（实际60kΩ） */
        unsigned int    bst_reserved5              : 4; /* [7..4] 测试用 及备用未定 */
    } bits;
    unsigned int    u32;
}HI6551_BOOST_ADJ5_T;    /* BOOST调节5寄存器。 */

typedef union
{
    struct
    {
        unsigned int    bst_nsn                    : 2; /* [1..0] NMOS栅控电压下降斜率00: 67ns01: 35ns10: 25ns11: 11ns */
        unsigned int    bst_nsp                    : 2; /* [3..2] NMOS栅控电压上升斜率00: 50ns01: 33ns10: 23ns11: 11ns */
        unsigned int    bst_psn                    : 2; /* [5..4] PMOS栅控电压下降斜率00:10ns01: 6ns10: 4ns11:3ns */
        unsigned int    bst_psp                    : 2; /* [7..6] PMOS栅控电压上升斜率00: 10ns01: 6ns10: 4ns11: 2ns */
    } bits;
    unsigned int    u32;
}HI6551_BOOST_ADJ6_T;    /* BOOST调节6寄存器。 */

typedef union
{
    struct
    {
        unsigned int    bst_dt_nm                  : 2; /* [1..0] NMOS关闭后开启PMOS的延时00: 001: 2ns10: 4ns11: 6ns */
        unsigned int    bst_dt_pm                  : 2; /* [3..2] PMOS关闭后开启NMOS的延时00: 001: 2ns10: 4ns11: 6ns */
        unsigned int    bst_lxde                   : 2; /* [5..4] 判定开启PMOS，LX判断电路保留寄存器LXDE<1>：保留LXDE<0>1：开启DMD关断逻辑0：关闭DMD关断逻辑（即DMD之后有漏电） */
        unsigned int    bst_reserved6              : 2; /* [7..6] 测试用 及备用未定 */
    } bits;
    unsigned int    u32;
}HI6551_BOOST_ADJ7_T;    /* BOOST调节7寄存器。 */

typedef union
{
    struct
    {
        unsigned int    bst_ckmin                  : 2; /* [1..0] NMOS最小导通时间00: 160ns01: 120ns10: 80ns11: 40ns */
        unsigned int    bst_osc                    : 4; /* [5..2] 开关频率（MHz）0000: 2.9910001: 2.8520010: 2.7120011: 2.5680100: 2.4230101: 2.2740110: 2.1220111: 1.9681000: 1.8131001: 1.6541010: 1.4921011: 1.3251100: 1.1551101: 0.9811110: 0.8021111: 0.617 */
        unsigned int    bst_reserved7              : 2; /* [7..6] 测试用 及备用未定 */
    } bits;
    unsigned int    u32;
}HI6551_BOOST_ADJ8_T;    /* BOOST调节8寄存器。 */

typedef union
{
    struct
    {
        unsigned int    vo                         : 3; /* [2..0] 输出电压000: 5.5V001: 5.4V010: 5.2V011: 5.0V100: 4.2V101~111: 3.8V */
        unsigned int    sel_ocp                    : 2; /* [4..3] NMOS限流值00: 1.7A01: 2.0A10: 2.3A11: 2.6A */
        unsigned int    bst_start                  : 3; /* [7..5] PMOS直通时栅电流控制（影响启动时间和短路电流）000: 0.3uA001: 0.4uA010: 0.5uA011: 0.6uA100: 0.7uA101: 0.8uA110: 0.9uA111: 1.4uA */
    } bits;
    unsigned int    u32;
}HI6551_BOOST_ADJ9_T;    /* BOOST调节9寄存器。 */

typedef union
{
    struct
    {
        unsigned int    bst_dmd_ofs                : 4; /* [3..0] DMD失调0000: 20mV (300mA)0001: 16mV (240mA)0010: 14mV (210mA)0011: 12mV (180mA)0100: 10mV (150mA)0101: 8mV (120mA)0110: 6mV (90mA)0111: 4mV (60mA)1000: 2mV (30mA)1001:01010: -16mV (-180mA).1011: -32mV (-360mA).1100: -48mV (-540mA).1101: -64mV (-720mA)..1110: -128mV (-900mA).1111: -256mV (-1080mA). */
        unsigned int    bst_reserved10             : 4; /* [7..4] 测试用 及备用未定 */
    } bits;
    unsigned int    u32;
}HI6551_BOOST_ADJ10_T;    /* BOOST调节10寄存器。 */

typedef union
{
    struct
    {
        unsigned int    bst_v2clp                  : 2; /* [1..0] EA输出高端钳位点（对应放大比例0.4情况）00: 0.7*VOUT01: 0.6*VOUT10: 0.5*VOUT11: 0.4*VOUT */
        unsigned int    bst_v2div                  : 2; /* [3..2] 三角波被缩小的比例，电阻取值。与V2RAMP寄存器配合使用。若取同样的值，放大比例为0.400: 455kΩ01: 380kΩ10: 320kΩ11: 245kΩ */
        unsigned int    bst_v2ramp                 : 2; /* [5..4] 产生三角波RC网络中R取值00: 705kΩ01: 590kΩ10: 485kΩ11: 370kΩ */
        unsigned int    bst_reserved11             : 2; /* [7..6] 测试用 及备用未定 */
    } bits;
    unsigned int    u32;
}HI6551_BOOST_ADJ11_T;    /* BOOST调节11寄存器。 */

typedef union
{
    struct
    {
        unsigned int    classd_mute_sel            : 1; /* [0..0] classd_mute_sel控制。    0：延时mute模式；1：正常mute模式。 */
        unsigned int    classd_mute                : 1; /* [1..1] classd_mute控制。    0：正常工作；1：Mute。 */
        unsigned int    classd_gain                : 2; /* [3..2] classD增益控制。   00:12db01:15db10:18db11:21db其它：mute。 */
        unsigned int    en_classd_int              : 1; /* [4..4] classD使能控制。1：开启；0：关闭。 */
        unsigned int    classd_i_ocp               : 2; /* [6..5] classd 过流阈值调节；00:1.5A01:2A10:2.5A11:3A */
        unsigned int    reserved                   : 1; /* [7..7] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_CLASSD_ADJ0_T;    /* CLASSD调节0寄存器。 */

typedef union
{
    struct
    {
        unsigned int    classd_n_sel               : 2; /* [1..0] 驱动级nmos控制。00：开一路nmos；01：开两路nmos；10：开三路nmos；11：开四路nmos。 */
        unsigned int    classd_p_sel               : 2; /* [3..2] 驱动级pmos控制。00：开一路pmos；01：开两路pmos；10：开三路pmos；11：开四路pmos。 */
        unsigned int    classd_i_ramp              : 2; /* [5..4] Ramp波中，比较器电流控制。00：1μA；01：3μA；10：4μA；11：5μA。 */
        unsigned int    classd_i_pump              : 2; /* [7..6] Ramp波中，pump电流控制。00：6μA；01：7μA；10：7μA；11：8μA。 */
    } bits;
    unsigned int    u32;
}HI6551_CLASSD_ADJ1_T;    /* CLASSD调节1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    classd_ocp_bps             : 1; /* [0..0] 过流保护控制。0：开启过流保护；1：bypass过流保护电路。 */
        unsigned int    classd_fx_bps              : 1; /* [1..1] 扩频模式控制。0：开启扩频模式；1：bypass扩频模式（固定频率）。 */
        unsigned int    classd_dt_sel              : 1; /* [2..2] dead time控制。0：2ns；1：7ns。 */
        unsigned int    classd_pls_byp             : 1; /* [3..3] 最小脉宽门限：0：正常工作1：bypass最小脉宽限制 */
        unsigned int    classd_reserved            : 4; /* [7..4] 测试用 及备用未定 */
    } bits;
    unsigned int    u32;
}HI6551_CLASSD_ADJ2_T;    /* CLASSD调节2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    bg_test                    : 4; /* [3..0] 基准测试模式预留 */
        unsigned int    bg_sleep                   : 1; /* [4..4] 0：ref_top正常工作 ；1：ref_top进入sleep模式。 */
        unsigned int    reserved                   : 3; /* [7..5] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_BANDGAP_THSD_ADJ1_T;    /* BANDGAP和THSD调节1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    en_tmp_int                 : 2; /* [1..0] 0x:THSD过温保护模块关闭；10:THSD过温保护模块开启。11:THSD的低功耗模式。高电平为10ms,周期为1s的周期信号 */
        unsigned int    reserved_1                 : 2; /* [3..2] 保留 */
        unsigned int    thsd_set_tmp               : 2; /* [5..4] 温度保护模块报警温度设置。00:  105℃；01:  115℃；10:  125℃；                      默认设置为125℃。 */
        unsigned int    reserved_0                 : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_BANDGAP_THSD_ADJ2_T;    /* BANDGAP和THSD调节2寄存器。 */

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
}HI6551_DR_FLA_CTRL1_T;    /* DR闪烁模式控制1寄存器。 */

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
}HI6551_DR_FLA_CTRL2_T;    /* DR闪烁模式控制2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    flash_period               : 8; /* [7..0] 闪烁周期T = 配置值 *31.25ms。 */
    } bits;
    unsigned int    u32;
}HI6551_FLASH_PERIOD_T;    /* DR闪烁模式周期调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    flash_on                   : 8; /* [7..0] 点亮时间t =配置值 *7.8125ms。 */
    } bits;
    unsigned int    u32;
}HI6551_FLASH_ON_T;    /* DR闪烁模式点亮时间调节寄存器。 */

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
}HI6551_DR_MODE_SEL_T;    /* DR灯模式选择寄存器。 */

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
}HI6551_DR_BRE_CTRL_T;    /* DR1/2呼吸模式控制寄存器。 */

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
}HI6551_DR1_TIM_CONF0_T;    /* DR1常亮常暗时间配置寄存器。 */

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
}HI6551_DR1_TIM_CONF1_T;    /* DR1渐亮渐暗时间配置寄存器。 */

typedef union
{
    struct
    {
        unsigned int    iset_dr1                   : 3; /* [2..0] DR1电流调节。                                    000: 3  mA；001: 6  mA；010: 9  mA；011: 12 mA；100: 15 mA；101: 18 mA；110: 21 mA；111: 24 mA； */
        unsigned int    reserved                   : 5; /* [7..3] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_DR1_ISET_T;    /* DR1电流选择寄存器。 */

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
}HI6551_DR2_TIM_CONF0_T;    /* DR2常亮常暗时间配置寄存器。 */

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
}HI6551_DR2_TIM_CONF1_T;    /* DR2渐亮渐暗时间配置寄存器。 */

typedef union
{
    struct
    {
        unsigned int    iset_dr2                   : 3; /* [2..0] DR2电流调节。                                    000: 3  mA；001: 6  mA；010: 9  mA；011: 12 mA；100: 15 mA；101: 18 mA；110: 21 mA；111: 24 mA； */
        unsigned int    reserved                   : 5; /* [7..3] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_DR2_ISET_T;    /* DR2电流选择寄存器。 */

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
}HI6551_DR_LED_CTRL_T;    /* DR3/4/5控制寄存器。 */

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
}HI6551_DR_OUT_CTRL_T;    /* DR3/4/5输出控制寄存器。 */

typedef union
{
    struct
    {
        unsigned int    iset_dr3                   : 3; /* [2..0] R3输出电流调节（mA）：0x0 : 1.0；0x1 : 1.5；0x2 : 2.0；0x3 : 2.5；0x4 : 3.0；0x5 : 3.5；0x6 : 4.0；0x7 : 4.5。 */
        unsigned int    reserved                   : 5; /* [7..3] 保留。 */
    } bits;
    unsigned int    u32;
}HI6551_DR3_ISET_T;    /* DR3电流选择寄存器。 */

typedef union
{
    struct
    {
        unsigned int    dr3_start_delay            : 8; /* [7..0] DR3启动延时范围：0～32768 ms，step：256ms。 */
    } bits;
    unsigned int    u32;
}HI6551_DR3_START_DEL_T;    /* DR3启动延时配置寄存器。 */

typedef union
{
    struct
    {
        unsigned int    dr3_t_off                  : 4; /* [3..0] DR3常暗时间配置。0000：0s；0001：0.5s；0010：1s；0011：2s；0100：4s；0101：6s；0110：8s；0111：12s；1000：14s；1001：16s；1111：长暗；其他值：保持配置发生时刻的LED亮度（电流）不变。 */
        unsigned int    dr3_t_on                   : 4; /* [7..4] DR3常亮时间配置。0000：0s；0001：0.5s；0010：1s；0011：2s；0100：4s；0101：6s；0110：8s；0111：12s；1000：14s；1001：16s；1111：长亮；其他值：保持配置发生时刻的LED亮度（电流）不变。 */
    } bits;
    unsigned int    u32;
}HI6551_DR3_TIM_CONF0_T;    /* DR3常亮常暗时间配置寄存器。 */

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
}HI6551_DR3_TIM_CONF1_T;    /* DR3渐亮渐暗时间配置寄存器。 */

typedef union
{
    struct
    {
        unsigned int    iset_dr4                   : 3; /* [2..0] DR4输出电流调节（mA）：0x0 : 1.0；0x1 : 1.5；0x2 : 2.0；0x3 : 2.5；0x4 : 3.0；0x5 : 3.5；0x6 : 4.0；0x7 : 4.5。 */
        unsigned int    reserved                   : 5; /* [7..3] 保留。 */
    } bits;
    unsigned int    u32;
}HI6551_DR4_ISET_T;    /* DR4电流选择寄存器。 */

typedef union
{
    struct
    {
        unsigned int    dr4_start_delay            : 8; /* [7..0] DR4启动延时范围：0～32768 ms，step：256ms。 */
    } bits;
    unsigned int    u32;
}HI6551_DR4_START_DEL_T;    /* DR4启动延时配置寄存器 */

typedef union
{
    struct
    {
        unsigned int    dr4_t_off                  : 4; /* [3..0] DR4常暗时间配置。0000：0s；0001：0.5s；0010：1s；0011：2s；0100：4s；0101：6s；0110：8s；0111：12s；1000：14s；1001：16s；1111：长暗；其他值：保持配置发生时刻的LED亮度（电流）不变。 */
        unsigned int    dr4_t_on                   : 4; /* [7..4] DR4常亮时间配置。0000：0s；0001：0.5s；0010：1s；0011：2s；0100：4s；0101：6s；0110：8s；0111：12s；1000：14s；1001：16s；1111：长亮；其他值：保持配置发生时刻的LED亮度（电流）不变。 */
    } bits;
    unsigned int    u32;
}HI6551_DR4_TIM_CONF0_T;    /* DR4常亮常暗时间配置寄存器。 */

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
}HI6551_DR4_TIM_CONF1_T;    /* DR4渐亮渐暗时间配置寄存器。 */

typedef union
{
    struct
    {
        unsigned int    iset_dr5                   : 3; /* [2..0] DR5输出电流调节（mA）：0x0 : 1.0；0x1 : 1.5；0x2 : 2.0；0x3 : 2.5；0x4 : 3.0；0x5 : 3.5；0x6 : 4.0；0x7 : 4.5。 */
        unsigned int    reserved                   : 5; /* [7..3] 保留。 */
    } bits;
    unsigned int    u32;
}HI6551_DR5_ISET_T;    /* DR5电流选择寄存器。 */

typedef union
{
    struct
    {
        unsigned int    dr5_start_delay            : 8; /* [7..0] DR5启动延时范围：0～32768 ms，step：256ms。 */
    } bits;
    unsigned int    u32;
}HI6551_DR5_START_DEL_T;    /* DR5启动延时配置寄存器。 */

typedef union
{
    struct
    {
        unsigned int    dr5_t_off                  : 4; /* [3..0] DR5常暗时间配置。0000：0s；0001：0.5s；0010：1s；0011：2s；0100：4s；0101：6s；0110：8s；0111：12s；1000：14s；1001：16s；1111：长暗；其他值：保持配置发生时刻的LED亮度（电流）不变。 */
        unsigned int    dr5_t_on                   : 4; /* [7..4] DR5常亮时间配置。0000：0s；0001：0.5s；0010：1s；0011：2s；0100：4s；0101：6s；0110：8s；0111：12s；1000：14s；1001：16s；1111：长亮；其他值：保持配置发生时刻的LED亮度（电流）不变。 */
    } bits;
    unsigned int    u32;
}HI6551_DR5_TIM_CONF0_T;    /* DR5常亮常暗时间配置寄存器。 */

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
}HI6551_DR5_TIM_CONF1_T;    /* DR5渐亮渐暗时间配置寄存器。 */

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
}HI6551_OTP_CTRL1_T;    /* OTP控制1信号寄存器。 */

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
}HI6551_OTP_CTRL2_T;    /* OTP控制2信号寄存器。 */

typedef union
{
    struct
    {
        unsigned int    otp_pdin                   : 8; /* [7..0] OTP的输入值寄存器。 */
    } bits;
    unsigned int    u32;
}HI6551_OTP_PDIN_T;    /* OTP写入值寄存器。 */

typedef union
{
    struct
    {
        unsigned int    otp_pdob0                  : 8; /* [7..0] OTP_PDOB[7:0] */
    } bits;
    unsigned int    u32;
}HI6551_OTP_PDOB0_T;    /* OTP读出值0寄存器。 */

typedef union
{
    struct
    {
        unsigned int    otp_pdob1                  : 8; /* [7..0] OTP_PDOB[15:8] */
    } bits;
    unsigned int    u32;
}HI6551_OTP_PDOB1_T;    /* OTP读出值1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    otp_pdob2                  : 8; /* [7..0] OTP_PDOB[23:16] */
    } bits;
    unsigned int    u32;
}HI6551_OTP_PDOB2_T;    /* OTP读出值2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    otp_pdob3                  : 8; /* [7..0] OTP_PDOB[31:24] */
    } bits;
    unsigned int    u32;
}HI6551_OTP_PDOB3_T;    /* OTP读出值3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtccr_a0                   : 8; /* [7..0] 读此寄存器，返回当前RTC值（32bit）中的bit[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_RTCCR_A0_T;    /* RTC_A数据0寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtccr_a1                   : 8; /* [7..0] 读此寄存器，返回当前RTC值（32bit）中的bit[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_RTCCR_A1_T;    /* RTC_A数据1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtccr_a2                   : 8; /* [7..0] 读此寄存器，返回当前RTC值（32bit）中的bit[23:16]。 */
    } bits;
    unsigned int    u32;
}HI6551_RTCCR_A2_T;    /* RTC_A数据2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtccr_a3                   : 8; /* [7..0] 读此寄存器，返回当前RTC值（32bit）中的bit[31:24]。 */
    } bits;
    unsigned int    u32;
}HI6551_RTCCR_A3_T;    /* RTC_A数据3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtclr_a0                   : 8; /* [7..0] 可设置当前时间值（32bit）中的bit[7:0]。读此寄存器，返回最后一次当前时间设置值的bit[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_RTCLR_A0_T;    /* RTCLR_A加载0寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtclr_a1                   : 8; /* [7..0] 可设置当前时间值（32bit）中的bit[15:8]。读此寄存器，返回最后一次当前时间设置值的bit[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_RTCLR_A1_T;    /* RTCLR_A加载1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtclr_a2                   : 8; /* [7..0] 可设置当前时间值（32bit）中的bit[23:16]。读此寄存器，返回最后一次当前时间设置值的bit[23:16]。 */
    } bits;
    unsigned int    u32;
}HI6551_RTCLR_A2_T;    /* RTCLR_A加载2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtclr_a3                   : 8; /* [7..0] 可设置当前时间值（32bit）中的bit[31:24]。读此寄存器，返回最后一次当前时间设置值的bit[31:24]。 */
    } bits;
    unsigned int    u32;
}HI6551_RTCLR_A3_T;    /* RTCLR_A加载3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtccr_a                    : 1; /* [0..0] 读RTC控制位0：读RTC当前值，返回值全部为0；1：读RTC当前值，返回值为实际当前值； */
        unsigned int    reserved                   : 7; /* [7..1] 保留。 */
    } bits;
    unsigned int    u32;
}HI6551_RTCCTRL_A_T;    /* RTCCTRL_A控制寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtcmr_a_a0                 : 8; /* [7..0] 可设置闹铃时间值（32bit）中的bit[7:0]。读此寄存器，返回最后一次闹铃时间设置值的bit[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_RTCMR_A_A0_T;    /* RTC_A_A比较0寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtcmr_a_a1                 : 8; /* [7..0] 可设置闹铃时间值（32bit）中的bit[15:8]。读此寄存器，返回最后一次闹铃时间设置值的bit[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_RTCMR_A_A1_T;    /* RTC_A_A比较1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtcmr_a_a2                 : 8; /* [7..0] 可设置闹铃时间值（32bit）中的bit[23:16]。读此寄存器，返回最后一次闹铃时间设置值的bit[23:16]。 */
    } bits;
    unsigned int    u32;
}HI6551_RTCMR_A_A2_T;    /* RTC_A_A比较2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtcmr_a_a3                 : 8; /* [7..0] 可设置闹铃时间值（32bit）中的bit[31:24]。读此寄存器，返回最后一次闹铃时间设置值的bit[31:24]。 */
    } bits;
    unsigned int    u32;
}HI6551_RTCMR_A_A3_T;    /* RTC_A_A比较3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtcmr_a_b0                 : 8; /* [7..0] 可设置闹铃时间值（32bit）中的bit[7:0]。读此寄存器，返回最后一次闹铃时间设置值的bit[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_RTCMR_A_B0_T;    /* RTC_A_B比较0寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtcmr_a_b1                 : 8; /* [7..0] 可设置闹铃时间值（32bit）中的bit[15:8]。读此寄存器，返回最后一次闹铃时间设置值的bit[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_RTCMR_A_B1_T;    /* RTC_A_B比较1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtcmr_a_b2                 : 8; /* [7..0] 可设置闹铃时间值（32bit）中的bit[23:16]。读此寄存器，返回最后一次闹铃时间设置值的bit[23:16]。 */
    } bits;
    unsigned int    u32;
}HI6551_RTCMR_A_B2_T;    /* RTC_A_B比较2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtcmr_a_b3                 : 8; /* [7..0] 可设置闹铃时间值（32bit）中的bit[31:24]。读此寄存器，返回最后一次闹铃时间设置值的bit[31:24]。 */
    } bits;
    unsigned int    u32;
}HI6551_RTCMR_A_B3_T;    /* RTC_A_B比较3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtcmr_a_c0                 : 8; /* [7..0] 可设置闹铃时间值（32bit）中的bit[7:0]。读此寄存器，返回最后一次闹铃时间设置值的bit[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_RTCMR_A_C0_T;    /* RTC_A_C比较0寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtcmr_a_c1                 : 8; /* [7..0] 可设置闹铃时间值（32bit）中的bit[15:8]。读此寄存器，返回最后一次闹铃时间设置值的bit[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_RTCMR_A_C1_T;    /* RTC_A_C比较1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtcmr_a_c2                 : 8; /* [7..0] 可设置闹铃时间值（32bit）中的bit[23:16]。读此寄存器，返回最后一次闹铃时间设置值的bit[23:16]。 */
    } bits;
    unsigned int    u32;
}HI6551_RTCMR_A_C2_T;    /* RTC_A_C比较2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtcmr_a_c3                 : 8; /* [7..0] 可设置闹铃时间值（32bit）中的bit[31:24]。读此寄存器，返回最后一次闹铃时间设置值的bit[31:24]。 */
    } bits;
    unsigned int    u32;
}HI6551_RTCMR_A_C3_T;    /* RTC_A_C比较3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtcmr_a_d0                 : 8; /* [7..0] 可设置闹铃时间值（32bit）中的bit[7:0]。读此寄存器，返回最后一次闹铃时间设置值的bit[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_RTCMR_A_D0_T;    /* RTC_A_D比较0寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtcmr_a_d1                 : 8; /* [7..0] 可设置闹铃时间值（32bit）中的bit[15:8]。读此寄存器，返回最后一次闹铃时间设置值的bit[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_RTCMR_A_D1_T;    /* RTC_A_D比较1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtcmr_a_d2                 : 8; /* [7..0] 可设置闹铃时间值（32bit）中的bit[23:16]。读此寄存器，返回最后一次闹铃时间设置值的bit[23:16]。 */
    } bits;
    unsigned int    u32;
}HI6551_RTCMR_A_D2_T;    /* RTC_A_D比较2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtcmr_a_d3                 : 8; /* [7..0] 可设置闹铃时间值（32bit）中的bit[31:24]。读此寄存器，返回最后一次闹铃时间设置值的bit[31:24]。 */
    } bits;
    unsigned int    u32;
}HI6551_RTCMR_A_D3_T;    /* RTC_A_D比较3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtcmr_a_e0                 : 8; /* [7..0] 可设置闹铃时间值（32bit）中的bit[7:0]。读此寄存器，返回最后一次闹铃时间设置值的bit[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_RTCMR_A_E0_T;    /* RTC_A_E比较0寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtcmr_a_e1                 : 8; /* [7..0] 可设置闹铃时间值（32bit）中的bit[15:8]。读此寄存器，返回最后一次闹铃时间设置值的bit[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_RTCMR_A_E1_T;    /* RTC_A_E比较1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtcmr_a_e2                 : 8; /* [7..0] 可设置闹铃时间值（32bit）中的bit[23:16]。读此寄存器，返回最后一次闹铃时间设置值的bit[23:16]。 */
    } bits;
    unsigned int    u32;
}HI6551_RTCMR_A_E2_T;    /* RTC_A_E比较2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtcmr_a_e3                 : 8; /* [7..0] 可设置闹铃时间值（32bit）中的bit[31:24]。读此寄存器，返回最后一次闹铃时间设置值的bit[31:24]。 */
    } bits;
    unsigned int    u32;
}HI6551_RTCMR_A_E3_T;    /* RTC_A_E比较3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtcmr_a_f0                 : 8; /* [7..0] 可设置闹铃时间值（32bit）中的bit[7:0]。读此寄存器，返回最后一次闹铃时间设置值的bit[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_RTCMR_A_F0_T;    /* RTC_A_F比较0寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtcmr_a_f1                 : 8; /* [7..0] 可设置闹铃时间值（32bit）中的bit[15:8]。读此寄存器，返回最后一次闹铃时间设置值的bit[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_RTCMR_A_F1_T;    /* RTC_A_F比较1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtcmr_a_f2                 : 8; /* [7..0] 可设置闹铃时间值（32bit）中的bit[23:16]。读此寄存器，返回最后一次闹铃时间设置值的bit[23:16]。 */
    } bits;
    unsigned int    u32;
}HI6551_RTCMR_A_F2_T;    /* RTC_A_F比较2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtcmr_a_f3                 : 8; /* [7..0] 可设置闹铃时间值（32bit）中的bit[31:24]。读此寄存器，返回最后一次闹铃时间设置值的bit[31:24]。 */
    } bits;
    unsigned int    u32;
}HI6551_RTCMR_A_F3_T;    /* RTC_A_F比较3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtccr_na0                  : 8; /* [7..0] 读此寄存器，返回当前RTC值（32bit）中的bit[7:0]。注： 改组RTC仅做计数用，无ALARM功能，不对用户开放。 */
    } bits;
    unsigned int    u32;
}HI6551_RTCCR_NA0_T;    /* RTC_NA数据0寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtccr_na1                  : 8; /* [7..0] 读此寄存器，返回当前RTC值（32bit）中的bit[15:8]。注： 改组RTC仅做计数用，无ALARM功能，不对用户开放。 */
    } bits;
    unsigned int    u32;
}HI6551_RTCCR_NA1_T;    /* RTC_NA数据1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtccr_na2                  : 8; /* [7..0] 读此寄存器，返回当前RTC值（32bit）中的bit[23:16]。注： 改组RTC仅做计数用，无ALARM功能，不对用户开放。 */
    } bits;
    unsigned int    u32;
}HI6551_RTCCR_NA2_T;    /* RTC_NA数据2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtccr_na3                  : 8; /* [7..0] 读此寄存器，返回当前RTC值（32bit）中的bit[31:24]。注： 改组RTC仅做计数用，无ALARM功能，不对用户开放。 */
    } bits;
    unsigned int    u32;
}HI6551_RTCCR_NA3_T;    /* RTC_NA数据3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtclr_na0                  : 8; /* [7..0] 可设置当前时间值（32bit）中的bit[7:0]。读此寄存器，返回最后一次当前时间设置值的bit[7:0]。注： 改组RTC仅做计数用，无ALARM功能，不对用户开放。 */
    } bits;
    unsigned int    u32;
}HI6551_RTCLR_NA0_T;    /* RTCLR_NA加载0寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtclr_na1                  : 8; /* [7..0] 可设置当前时间值（32bit）中的bit[15:8]。读此寄存器，返回最后一次当前时间设置值的bit[15:8]。注： 改组RTC仅做计数用，无ALARM功能，不对用户开放。 */
    } bits;
    unsigned int    u32;
}HI6551_RTCLR_NA1_T;    /* RTCLR_NA加载1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtclr_na2                  : 8; /* [7..0] 可设置当前时间值（32bit）中的bit[23:16]。读此寄存器，返回最后一次当前时间设置值的bit[23:16]。注： 改组RTC仅做计数用，无ALARM功能，不对用户开放。 */
    } bits;
    unsigned int    u32;
}HI6551_RTCLR_NA2_T;    /* RTCLR_NA加载2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtclr_na3                  : 8; /* [7..0] 可设置当前时间值（32bit）中的bit[31:24]。读此寄存器，返回最后一次当前时间设置值的bit[31:24]。注： 改组RTC仅做计数用，无ALARM功能，不对用户开放。 */
    } bits;
    unsigned int    u32;
}HI6551_RTCLR_NA3_T;    /* RTCLR_NA加载3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtccr_na                   : 1; /* [0..0] 读RTC控制位0：读RTC当前值，返回值全部为0；1：读RTC当前值，返回值为实际当前值；注： 改组RTC仅做计数用，无ALARM功能，不对用户开放。 */
        unsigned int    reserved                   : 7; /* [7..1] 保留。 */
    } bits;
    unsigned int    u32;
}HI6551_RTCCTRL_NA_T;    /* RTCCTRL_NA控制寄存器。 */

typedef union
{
    struct
    {
        unsigned int    version                    : 8; /* [7..0] 版本寄存器，表示版本为V220。 */
    } bits;
    unsigned int    u32;
}HI6551_VERSION_T;    /* 版本寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved0                  : 8; /* [7..0] bit[7:4]：测试用 及备用未定；bit[3:0]: lvs的offset调整寄存器。 */
    } bits;
    unsigned int    u32;
}HI6551_RESERVED0_T;    /* 保留0寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved1                  : 8; /* [7..0] bit[7:2]:测试用 及备用未定；bit[1:0]:配置REF进入ECO模式。 */
    } bits;
    unsigned int    u32;
}HI6551_RESERVED1_T;    /* 保留1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved2                  : 8; /* [7..0] 测试用 及备用未定。 */
    } bits;
    unsigned int    u32;
}HI6551_RESERVED2_T;    /* 保留2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved3                  : 8; /* [7..0] 测试用 及备用未定。 */
    } bits;
    unsigned int    u32;
}HI6551_RESERVED3_T;    /* 保留3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved4                  : 8; /* [7..0] 测试用 及备用未定。 */
    } bits;
    unsigned int    u32;
}HI6551_RESERVED4_T;    /* 保留4寄存器。 */

typedef union
{
    struct
    {
        unsigned int    htol_mode                  : 4; /* [3..0] 0101：不进入老化模式 ；1010：进入老化测试模式 ；其他：无效 。注：老化测试专用。 */
        unsigned int    reserved                   : 4; /* [7..4] 保留。 */
    } bits;
    unsigned int    u32;
}HI6551_HTOL_MODE_T;    /* 老化测试模式配置寄存器。 */

typedef union
{
    struct
    {
        unsigned int    dac_on_sel                 : 1; /* [0..0] 1: 两个DAC的开关由dac_reg_onoff控制；0：两个DAC的开关由外边pin脚控制。 */
        unsigned int    aux_ibias_cfg              : 2; /* [2..1] AUXDAC偏置电流调节，11最大，00最小 */
        unsigned int    reserved                   : 5; /* [7..3] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_DAC_CTRL_T;    /* DAC控制寄存器 */

typedef union
{
    struct
    {
        unsigned int    soft_rst                   : 8; /* [7..0] 0x11:复位库仑计模块；0x1f:解除库仑计模块复位；写入其他无效。 */
    } bits;
    unsigned int    u32;
}HI6551_CHIP_SOFT_RST_T;    /* 芯片模块软复位寄存器。 */

typedef union
{
    struct
    {
        unsigned int    pdns_sel                   : 3; /* [2..0] 长按键Ns关机可配寄存器：000:6s;001:7s;010:8s;011:9s;100:10s;101:11s;110:8s;111:9s。 */
        unsigned int    pdns_mk                    : 1; /* [3..3] 长按键关机屏蔽寄存器：0：不屏蔽长按键关机；1：屏蔽长按键关机；注：在正常工作中，一直按下pwron_n ，此时才可以通过SSI将长按键关机屏蔽位置一，当pwron_n松开20ms后该位自动清除。 */
        unsigned int    vsys_uv_adj                : 1; /* [4..4] 2.7V/3V欠压保护调节：0：vsys低压检测点设为3.0V；1：vsys低压检测点设为2.7V； */
        unsigned int    nopwr_rc_off               : 1; /* [5..5] 关闭内部256K RC时钟:0:使能;1:关闭。 */
        unsigned int    reserved                   : 2; /* [7..6] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_NP_REG_ADJ_T;    /* 非下电调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    nopwr_vcoinsl              : 2; /* [1..0] 备用电池充电电压选择：00：2.5V；01：3.0V；1X：3.3V。 */
        unsigned int    nopwr_en_backup_chg        : 1; /* [2..2] 备用电池充电使能：0：不使能；1：使能。 */
        unsigned int    reserved                   : 5; /* [7..3] 保留 */
    } bits;
    unsigned int    u32;
}HI6551_NP_REG_CHG_T;    /* 备用电池非下电调节寄存器。 */

typedef union
{
    struct
    {
        unsigned int    no_pwr_resved1             : 8; /* [7..0] 模拟预留：测试用及备用未定 */
    } bits;
    unsigned int    u32;
}HI6551_NP_RSVED1_T;    /* 非下电保留1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    no_pwr_resved2             : 8; /* [7..0] 模拟预留：测试用及备用未定 */
    } bits;
    unsigned int    u32;
}HI6551_NP_RSVED2_T;    /* 非下电保留2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    no_pwr_resved3             : 8; /* [7..0] 产品预留：测试用及备用，不对PMU内部工作状态产生任何作用，仅用于记录信息。 */
    } bits;
    unsigned int    u32;
}HI6551_NP_RSVED3_T;    /* 非下电保留3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    no_pwr_resved4             : 8; /* [7..0] 产品预留：测试用及备用，不对PMU内部工作状态产生任何作用，仅用于记录信息。 */
    } bits;
    unsigned int    u32;
}HI6551_NP_RSVED4_T;    /* 非下电保留4寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtc_clk_step_adj1          : 8; /* [7..0] 计时精度可以调节，调节范围+/-192ppm（63拍时钟），调节步进3.05ppm（1拍时钟），默认为0。 */
    } bits;
    unsigned int    u32;
}HI6551_RTC_ADJ1_T;    /* rtc_adj[7:0]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    rtc_clk_step_adj2          : 8; /* [7..0] 计时精度可以调节，调节范围+/-192ppm（63拍时钟），调节步进3.05ppm（1拍时钟），默认为0。 */
    } bits;
    unsigned int    u32;
}HI6551_RTC_ADJ2_T;    /* rtc_adj[15:8]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    eco_ctrl                   : 3; /* [2..0] bit[2]:0：不强制进入ECO模式；1：强制进入ECO模式。bit[1]:0：库仑计不跟随PMU进入ECO模式；1：库仑计跟随PMU进入ECO模式。仅发生在bit[2] == 0 时有效bit[0]:0：进入ECO模式使能信号不去抖；1：进入ECO模式使能信号去抖。仅发生在bit[1] ==1 时有效 */
        unsigned int    reflash_value_ctrl         : 1; /* [3..3] 0：所有状态都更新数据；1：仅ECO模式更新数据；默认为0。 */
        unsigned int    eco_filter_time            : 2; /* [5..4] 00：20ms；01：25ms；10：30ms；11：35ms；用于配置en_eco_mode去抖时长，默认为00。 */
        unsigned int    calibration_ctrl           : 1; /* [6..6] 1：强制进入校准状态；0：不校准；默认为0。 */
        unsigned int    coul_ctrl_onoff_reg        : 1; /* [7..7] 1：库仑计开启；0：库仑计关闭。默认为1。 */
    } bits;
    unsigned int    u32;
}HI6551_CLJ_CTRL_REG_T;    /* 寄存器控制寄存器。 */

typedef union
{
    struct
    {
        unsigned int    eco_reflash_time           : 8; /* [7..0] 此工作时间以一个单位代表110ms，为ECO_REFLASH时间；有效配置值为1~255，配置值大于255时，硬件直接饱和到255。默认为1。 */
    } bits;
    unsigned int    u32;
}HI6551_ECO_REFALSH_TIME_T;    /* ECO_REFLASH工作时间寄存器。 */

typedef union
{
    struct
    {
        unsigned int    cl_out0                    : 8; /* [7..0] 流入电量计数器[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_CL_OUT0_T;    /* cl_out[7:0]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    cl_out1                    : 8; /* [7..0] 流入电量计数器[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_CL_OUT1_T;    /* cl_out[15:8]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    cl_out2                    : 8; /* [7..0] 流入电量计数器[23:16]。 */
    } bits;
    unsigned int    u32;
}HI6551_CL_OUT2_T;    /* cl_out[23:16]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    cl_out3                    : 8; /* [7..0] 流入电量计数器[31:24]。 */
    } bits;
    unsigned int    u32;
}HI6551_CL_OUT3_T;    /* cl_out[31:24]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    cl_in0                     : 8; /* [7..0] 流出电量计数器[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_CL_IN0_T;    /* cl_in[7:0]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    cl_in1                     : 8; /* [7..0] 流出电量计数器[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_CL_IN1_T;    /* cl_in[15:8]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    cl_in2                     : 8; /* [7..0] 流出电量计数器[23:16]。 */
    } bits;
    unsigned int    u32;
}HI6551_CL_IN2_T;    /* cl_in[23:16]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    cl_in3                     : 8; /* [7..0] 流出电量计数器[31:24]。 */
    } bits;
    unsigned int    u32;
}HI6551_CL_IN3_T;    /* cl_in[31:24]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    chg_timer0                 : 8; /* [7..0] 放电时间计数器[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_CHG_TIMER0_T;    /* chg_timer[7:0]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    chg_timer1                 : 8; /* [7..0] 放电时间计数器[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_CHG_TIMER1_T;    /* chg_timer[15:8]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    chg_timer2                 : 8; /* [7..0] 放电时间计数器[23:16]。 */
    } bits;
    unsigned int    u32;
}HI6551_CHG_TIMER2_T;    /* chg_timer[23:16]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    chg_timer3                 : 8; /* [7..0] 放电时间计数器[31:24]。 */
    } bits;
    unsigned int    u32;
}HI6551_CHG_TIMER3_T;    /* chg_timer[31:24]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    load_timer0                : 8; /* [7..0] 充电时间计数器[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_LOAD_TIMER0_T;    /* load_timer[7:0]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    load_timer1                : 8; /* [7..0] 充电时间计数器[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_LOAD_TIMER1_T;    /* load_timer[15:8]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    load_timer2                : 8; /* [7..0] 充电时间计数器[23:16]。 */
    } bits;
    unsigned int    u32;
}HI6551_LOAD_TIMER2_T;    /* load_timer[23:16]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    load_timer3                : 8; /* [7..0] 充电时间计数器[31:24]。 */
    } bits;
    unsigned int    u32;
}HI6551_LOAD_TIMER3_T;    /* load_timer[31:24]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    off_timer0                 : 8; /* [7..0] 关机计数器[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_OFF_TIMER0_T;    /* off_timer[7:0]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    off_timer1                 : 8; /* [7..0] 关机计数器[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_OFF_TIMER1_T;    /* off_timer[15:8]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    off_timer2                 : 8; /* [7..0] 关机计数器[23:16]。 */
    } bits;
    unsigned int    u32;
}HI6551_OFF_TIMER2_T;    /* off_timer[23:16]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    off_timer3                 : 8; /* [7..0] 关机计数器[31:24]。 */
    } bits;
    unsigned int    u32;
}HI6551_OFF_TIMER3_T;    /* off_timer[31:24]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    cl_int0                    : 8; /* [7..0] 流入电量中断设置，配置一个值，当cl_out>cl_int时上报中断。 */
    } bits;
    unsigned int    u32;
}HI6551_CL_INT0_T;    /* cl_int[7:0]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    cl_int1                    : 8; /* [7..0] 流入电量中断设置，配置一个值，当cl_out>cl_int时上报中断。 */
    } bits;
    unsigned int    u32;
}HI6551_CL_INT1_T;    /* cl_int[15:8]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    cl_int2                    : 8; /* [7..0] 流入电量中断设置，配置一个值，当cl_out>cl_int时上报中断。 */
    } bits;
    unsigned int    u32;
}HI6551_CL_INT2_T;    /* cl_int[23:16]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    cl_int3                    : 8; /* [7..0] 流入电量中断设置，配置一个值，当cl_out>cl_int时上报中断。 */
    } bits;
    unsigned int    u32;
}HI6551_CL_INT3_T;    /* cl_int[31:24]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    v_int0                     : 8; /* [7..0] 电压中断比较值寄存器[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_V_INT0_T;    /* v_int[7:0]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    v_int1                     : 8; /* [7..0] 电压中断比较值寄存器[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_V_INT1_T;    /* v_int[15:8]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    offset_current0            : 8; /* [7..0] 电流校准[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_OFFSET_CURRENT0_T;    /* offset_current[7:0]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    offset_current1            : 8; /* [7..0] 电流校准[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_OFFSET_CURRENT1_T;    /* offset_current[15:8]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    offset_voltage0            : 8; /* [7..0] 电压校准[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_OFFSET_VOLTAGE0_T;    /* offset_voltage[7:0]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    offset_voltage1            : 8; /* [7..0] 电压校准[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_OFFSET_VOLTAGE1_T;    /* offset_voltage[15:8]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    ocv_data0                  : 8; /* [7..0] OCV电压采样值[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_OCV_DATA1_T;    /* ocv_data[7:0]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    ocv_data1                  : 8; /* [7..0] OCV电压采样值[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_OCV_DATA2_T;    /* ocv_data[15:8]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    v_out0_pre0                : 8; /* [7..0] 当前电池电压指示[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_V_OUT0_PRE0_T;    /* v_out_pre0[7:0]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    v_out1_pre0                : 8; /* [7..0] 当前电池电压指示[15:8]，最高位代表符号位]。 */
    } bits;
    unsigned int    u32;
}HI6551_V_OUT1_PRE0_T;    /* v_out_pre0[15:8]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    v_out0_pre1                : 8; /* [7..0] 当前电压前一次值[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_V_OUT0_PRE1_T;    /* v_out_pre1[7:0]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    v_out1_pre1                : 8; /* [7..0] 当前电压前一次值[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_V_OUT1_PRE1_T;    /* v_out_pre1[15:8]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    v_out0_pre2                : 8; /* [7..0] 当前电压前两次值[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_V_OUT0_PRE2_T;    /* v_out_pre2[7:0]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    v_out1_pre2                : 8; /* [7..0] 当前电压前两次值[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_V_OUT1_PRE2_T;    /* v_out_pre2[15:8]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    v_out0_pre3                : 8; /* [7..0] 当前电压前三次值[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_V_OUT0_PRE3_T;    /* v_out_pre3[7:0]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    v_out1_pre3                : 8; /* [7..0] 当前电压前三次值[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_V_OUT1_PRE3_T;    /* v_out_pre3[15:8]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    v_out0_pre4                : 8; /* [7..0] 当前电压前四次值[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_V_OUT0_PRE4_T;    /* v_out_pre4[7:0]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    v_out1_pre4                : 8; /* [7..0] 当前电压前四次值[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_V_OUT1_PRE4_T;    /* v_out_pre4[15:8]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    v_out0_pre5                : 8; /* [7..0] 当前电压前五次值[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_V_OUT0_PRE5_T;    /* v_out_pre5[7:0]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    v_out1_pre5                : 8; /* [7..0] 当前电压前五次值[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_V_OUT1_PRE5_T;    /* v_out_pre5[15:8]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    v_out0_pre6                : 8; /* [7..0] 当前电压前六次值[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_V_OUT0_PRE6_T;    /* v_out_pre6[7:0]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    v_out1_pre6                : 8; /* [7..0] 当前电压前六次值[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_V_OUT1_PRE6_T;    /* v_out_pre6[15:8]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    v_out0_pre7                : 8; /* [7..0] 当前电压前七次值[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_V_OUT0_PRE7_T;    /* v_out_pre7[7:0]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    v_out1_pre7                : 8; /* [7..0] 当前电压前七次值[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_V_OUT1_PRE7_T;    /* v_out_pre7[15:8]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    v_out0_pre8                : 8; /* [7..0] 当前电压前八次值[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_V_OUT0_PRE8_T;    /* v_out_pre8[7:0]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    v_out1_pre8                : 8; /* [7..0] 当前电压前八次值[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_V_OUT1_PRE8_T;    /* v_out_pre8[15:8]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    v_out0_pre9                : 8; /* [7..0] 当前电压前九次值[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_V_OUT0_PRE9_T;    /* v_out_pre9[7:0]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    v_out1_pre9                : 8; /* [7..0] 当前电压前九次值[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_V_OUT1_PRE9_T;    /* v_out_pre9[15:8]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    v_out0_pre10               : 8; /* [7..0] 当前电压前十次值[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_V_OUT0_PRE10_T;    /* v_out_pre10[7:0]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    v_out1_pre10               : 8; /* [7..0] 当前电压前十次值[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_V_OUT1_PRE10_T;    /* v_out_pre10[15:8]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    v_out0_pre11               : 8; /* [7..0] 当前电压前十一次值[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_V_OUT0_PRE11_T;    /* v_out_pre11[7:0]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    v_out1_pre11               : 8; /* [7..0] 当前电压前十一次值[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_V_OUT1_PRE11_T;    /* v_out_pre11[15:8]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    v_out0_pre12               : 8; /* [7..0] 当前电压前十二次值[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_V_OUT0_PRE12_T;    /* v_out_pre12[7:0]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    v_out1_pre12               : 8; /* [7..0] 当前电压前十二次值[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_V_OUT1_PRE12_T;    /* v_out_pre12[15:8]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    v_out0_pre13               : 8; /* [7..0] 当前电压前十三次值[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_V_OUT0_PRE13_T;    /* v_out_pre13[7:0]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    v_out1_pre13               : 8; /* [7..0] 当前电压前十三次值[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_V_OUT1_PRE13_T;    /* v_out_pre13[15:8]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    v_out0_pre14               : 8; /* [7..0] 当前电压前十四次值[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_V_OUT0_PRE14_T;    /* v_out_pre14[7:0]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    v_out1_pre14               : 8; /* [7..0] 当前电压前十四次值[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_V_OUT1_PRE14_T;    /* v_out_pre14[15:8]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    v_out0_pre15               : 8; /* [7..0] 当前电压前十五次值[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_V_OUT0_PRE15_T;    /* v_out_pre15[7:0]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    v_out1_pre15               : 8; /* [7..0] 当前电压前十五次值[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_V_OUT1_PRE15_T;    /* v_out_pre15[15:8]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    v_out0_pre16               : 8; /* [7..0] 当前电压前十六次值[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_V_OUT0_PRE16_T;    /* v_out_pre16[7:0]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    v_out1_pre16               : 8; /* [7..0] 当前电压前十六次值[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_V_OUT1_PRE16_T;    /* v_out_pre16[15:8]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    v_out0_pre17               : 8; /* [7..0] 当前电压前十七次值[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_V_OUT0_PRE17_T;    /* v_out_pre17[7:0]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    v_out1_pre17               : 8; /* [7..0] 当前电压前十七次值[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_V_OUT1_PRE17_T;    /* v_out_pre17[15:8]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    v_out0_pre18               : 8; /* [7..0] 当前电压前十八次值[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_V_OUT0_PRE18_T;    /* v_out_pre18[7:0]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    v_out1_pre18               : 8; /* [7..0] 当前电压前十八次值[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_V_OUT1_PRE18_T;    /* v_out_pre18[15:8]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    v_out0_pre19               : 8; /* [7..0] 当前电压前十九次值[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_V_OUT0_PRE19_T;    /* v_out_pre19[7:0]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    v_out1_pre19               : 8; /* [7..0] 当前电压前十九次值[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_V_OUT1_PRE19_T;    /* v_out_pre19[15:8]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    current0_pre0              : 8; /* [7..0] 当前电流指示[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_CURRENT0_PRE0_T;    /* current_pre0[7:0]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    current1_pre0              : 8; /* [7..0] 当前电流指示[15:8]，最高位代表符号位。. */
    } bits;
    unsigned int    u32;
}HI6551_CURRENT1_PRE0_T;    /* current_pre0[15:8]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    current0_pre1              : 8; /* [7..0] 当前电流前一次值[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_CURRENT0_PRE1_T;    /* current_pre1[7:0]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    current1_pre1              : 8; /* [7..0] 当前电流前一次值[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_CURRENT1_PRE1_T;    /* current_pre1[15:8]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    current0_pre2              : 8; /* [7..0] 当前电流前两次值[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_CURRENT0_PRE2_T;    /* current_pre2[7:0]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    current1_pre2              : 8; /* [7..0] 当前电流前两次值[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_CURRENT1_PRE2_T;    /* current_pre2[15:8]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    current0_pre3              : 8; /* [7..0] 当前电流前三次值[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_CURRENT0_PRE3_T;    /* current_pre3[7:0]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    current1_pre3              : 8; /* [7..0] 当前电流前三次值[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_CURRENT1_PRE3_T;    /* current_pre3[15:8]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    current0_pre4              : 8; /* [7..0] 当前电流前四次值[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_CURRENT0_PRE4_T;    /* current_pre4[7:0]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    current1_pre4              : 8; /* [7..0] 当前电流前四次值[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_CURRENT1_PRE4_T;    /* current_pre4[15:8]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    current0_pre5              : 8; /* [7..0] 当前电流前五次值[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_CURRENT0_PRE5_T;    /* current_pre5[7:0]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    current1_pre5              : 8; /* [7..0] 当前电流前五次值[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_CURRENT1_PRE5_T;    /* current_pre5[15:8]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    current0_pre6              : 8; /* [7..0] 当前电流前六次值[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_CURRENT0_PRE6_T;    /* current_pre6[7:0]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    current1_pre6              : 8; /* [7..0] 当前电流前六次值[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_CURRENT1_PRE6_T;    /* current_pre6[15:8]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    current0_pre7              : 8; /* [7..0] 当前电流前七次值[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_CURRENT0_PRE7_T;    /* current_pre7[7:0]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    current1_pre7              : 8; /* [7..0] 当前电流前七次值[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_CURRENT1_PRE7_T;    /* current_pre7[15:8]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    current0_pre8              : 8; /* [7..0] 当前电流前八次值[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_CURRENT0_PRE8_T;    /* current_pre8[7:0]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    current1_pre8              : 8; /* [7..0] 当前电流前八次值[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_CURRENT1_PRE8_T;    /* current_pre8[15:8]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    current0_pre9              : 8; /* [7..0] 当前电流前九次值[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_CURRENT0_PRE9_T;    /* current_pre9[7:0]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    current1_pre9              : 8; /* [7..0] 当前电流前九次值[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_CURRENT1_PRE9_T;    /* current_pre9[15:8]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    current0_pre10             : 8; /* [7..0] 当前电流前十次值[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_CURRENT0_PRE10_T;    /* current_pre10[7:0]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    current1_pre10             : 8; /* [7..0] 当前电流前十次值[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_CURRENT1_PRE10_T;    /* current_pre10[15:8]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    current0_pre11             : 8; /* [7..0] 当前电流前十一次值[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_CURRENT0_PRE11_T;    /* current_pre11[7:0]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    current1_pre11             : 8; /* [7..0] 当前电流前十一次值[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_CURRENT1_PRE11_T;    /* current_pre11[15:8]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    current0_pre12             : 8; /* [7..0] 当前电流前十二次值[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_CURRENT0_PRE12_T;    /* current_pre12[7:0]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    current1_pre12             : 8; /* [7..0] 当前电流前十二次值[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_CURRENT1_PRE12_T;    /* current_pre12[15:8]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    current0_pre13             : 8; /* [7..0] 当前电流前十三次值[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_CURRENT0_PRE13_T;    /* current_pre13[7:0]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    current1_pre13             : 8; /* [7..0] 当前电流前十三次值[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_CURRENT1_PRE13_T;    /* current_pre13[15:8]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    current0_pre14             : 8; /* [7..0] 当前电流前十四次值[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_CURRENT0_PRE14_T;    /* current_pre14[7:0]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    current1_pre14             : 8; /* [7..0] 当前电流前十四次值[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_CURRENT1_PRE14_T;    /* current_pre14[15:8]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    current0_pre15             : 8; /* [7..0] 当前电流前十五次值[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_CURRENT0_PRE15_T;    /* current_pre15[7:0]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    current1_pre15             : 8; /* [7..0] 当前电流前十五次值[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_CURRENT1_PRE15_T;    /* current_pre15[15:8]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    current0_pre16             : 8; /* [7..0] 当前电流前十六次值[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_CURRENT0_PRE16_T;    /* current_pre16[7:0]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    current1_pre16             : 8; /* [7..0] 当前电流前十六次值[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_CURRENT1_PRE16_T;    /* current_pre16[15:8]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    current0_pre17             : 8; /* [7..0] 当前电流前十七次值[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_CURRENT0_PRE17_T;    /* current_pre17[7:0]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    current1_pre17             : 8; /* [7..0] 当前电流前十七次值[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_CURRENT1_PRE17_T;    /* current_pre17[15:8]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    current0_pre18             : 8; /* [7..0] 当前电流前十八次值[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_CURRENT0_PRE18_T;    /* current_pre18[7:0]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    current1_pre18             : 8; /* [7..0] 当前电流前十八次值[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_CURRENT1_PRE18_T;    /* current_pre18[15:8]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    current0_pre19             : 8; /* [7..0] 当前电流前十九次值[7:0]。 */
    } bits;
    unsigned int    u32;
}HI6551_CURRENT0_PRE19_T;    /* current_pre19[7:0]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    current1_pre19             : 8; /* [7..0] 当前电流前十九次值[15:8]。 */
    } bits;
    unsigned int    u32;
}HI6551_CURRENT1_PRE19_T;    /* current_pre19[15:8]寄存器。 */

typedef union
{
    struct
    {
        unsigned int    cic_clk_inv_i              : 1; /* [0..0] 控制库仑计电流滤波电路第一级取反时钟。0：无效，1：有效 */
        unsigned int    cic_clk_inv_v              : 1; /* [1..1] 控制库仑计电压滤波电路第一级取反时钟。0：无效，1：有效 */
        unsigned int    adc_ana_v_output           : 1; /* [2..2] 复用CK32BC管脚，电压ADC模拟输出 */
        unsigned int    adc_ana_i_output           : 1; /* [3..3] 复用CK32C管脚，电流ADC模拟输出 */
        unsigned int    cali_en_i                  : 1; /* [4..4] 0:保持正常；1：电流始终不进行检测校准，默认为0 */
        unsigned int    cali_en_i_force            : 1; /* [5..5] 1：电流强制校准；0：电流不强制校准（默认），以上功能均在clj_debug[4]=0时候作用。 */
        unsigned int    cali_en_v_force            : 1; /* [6..6] 1：电压强制校准；0：电压不强制校准（默认），以上功能均在clj_debug[7]=0时候作用。 */
        unsigned int    cali_en_v                  : 1; /* [7..7] 0:保持正常；1：电压检测不进行校准。默认为0 */
    } bits;
    unsigned int    u32;
}HI6551_CLJ_DEBUG_T;    /* 库仑计DEBUG专用1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    state_test                 : 8; /* [7..0] 电流和电压检测跳转状态：[7:4]表示电流跳转状态，[3:0]表示电压跳转状态。 */
    } bits;
    unsigned int    u32;
}HI6551_STATE_TEST_T;    /* 库仑计DEBUG专用2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reg_data_clr               : 1; /* [0..0] 40组寄存器（电流，电压）检测值恢复原始值标志。0：不恢复初始值；1：恢复初始值 。 */
        unsigned int    clj_rw_inf1                : 7; /* [7..1] 产品软件专用，对库仑计可维可测，仅用于记录数据，不对库仑计状态产生任何作用。 */
    } bits;
    unsigned int    u32;
}HI6551_CLJ_RESERVED1_T;    /* 库仑计保留1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    clj_rw_inf2                : 8; /* [7..0] 产品软件专用，对库仑计可维可测，仅用于记录数据，不对库仑计状态产生任何作用。 */
    } bits;
    unsigned int    u32;
}HI6551_CLJ_RESERVED2_T;    /* 库仑计保留2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    clj_rw_inf3                : 8; /* [7..0] 产品软件专用，对库仑计可维可测，仅用于记录数据，不对库仑计状态产生任何作用。 */
    } bits;
    unsigned int    u32;
}HI6551_CLJ_RESERVED3_T;    /* 库仑计保留3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    clj_rw_inf4                : 8; /* [7..0] 产品软件专用，对库仑计可维可测，仅用于记录数据，不对库仑计状态产生任何作用。 */
    } bits;
    unsigned int    u32;
}HI6551_CLJ_RESERVED4_T;    /* 库仑计保留4寄存器。 */

typedef union
{
    struct
    {
        unsigned int    i_reserve_1                : 8; /* [7..0] 库仑计模拟寄存器保留。 */
    } bits;
    unsigned int    u32;
}HI6551_CLJ_RESERVED5_T;    /* 库仑计保留5寄存器。 */

typedef union
{
    struct
    {
        unsigned int    i_reserve_2                : 8; /* [7..0] 库仑计模拟寄存器保留。 */
    } bits;
    unsigned int    u32;
}HI6551_CLJ_RESERVED6_T;    /* 库仑计保留6寄存器。 */

typedef union
{
    struct
    {
        unsigned int    v_reserve_1                : 8; /* [7..0] 库仑计模拟寄存器保留。 */
    } bits;
    unsigned int    u32;
}HI6551_CLJ_RESERVED7_T;    /* 库仑计保留7寄存器。 */

typedef union
{
    struct
    {
        unsigned int    v_reserve_2                : 8; /* [7..0] 库仑计模拟寄存器保留。 */
    } bits;
    unsigned int    u32;
}HI6551_CLJ_RESERVED8_T;    /* 库仑计保留8寄存器。 */


/********************************************************************************/
/*    hi6551 函数（项目名_模块名_寄存器名_成员名_set)        */
/********************************************************************************/

#endif // __HI6551_H__

