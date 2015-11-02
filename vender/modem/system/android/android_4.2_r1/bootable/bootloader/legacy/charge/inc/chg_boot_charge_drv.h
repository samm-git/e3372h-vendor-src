/******************************************************************************
                  版权所有 (C), 2001-2012,华为终端有限公司
 ******************************************************************************
  文 件 名   : chg_charge_drv.h
  版 本 号   : 1.0
  生成日期   : 10/30/2012
  最近修改   :
  功能描述   : BQ24192 Charge IC Driver
  函数列表   : 包含的函数名列表
******************************************************************************/

/**********************问题单修改记录******************************************
日    期              修改人         问题单号           修改内容
 2012.x.x           xxxxx          DTSxxxx               xxx
******************************************************************************/
#ifndef CHG_BOOT_CHARGE_DRV_H
#define CHG_BOOT_CHARGE_DRV_H
 /*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/

 #include "chg_boot_chip_platform.h"

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/
/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/
/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/
/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/
/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/
/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/
/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
 /* BEGIN: PN:dongle121,chendongguo, 2013/6/26 added/deleted/modified*/
#define    I2C_RW_PASS   (0)
#define    I2C_RW_FAIL  (-1)
 /* END: PN:dongle121,chendongguo, 2013/6/26 added/deleted/modified*/


/*充电电流相关宏*/
#define CHG_CURRENT_2048MA_B7       (2048)
#define CHG_CURRENT_1024MA_B6       (1024)
#define CHG_CURRENT_512MA_B5        (512)
#define CHG_CURRENT_256MA_B4        (256)
#define CHG_CURRENT_128MA_B3        (128)
#define CHG_CURRENT_64MA_B2         (64)


#define CHG_IINPUT_LIMIT_3000MA     (3000)
#define CHG_IINPUT_LIMIT_2000MA     (2000)
#define CHG_IINPUT_LIMIT_1500MA     (1500)
#define CHG_IINPUT_LIMIT_1200MA     (1200)
#define CHG_IINPUT_LIMIT_1000MA     (1000)
#define CHG_IINPUT_LIMIT_900MA      (900)
#define CHG_IINPUT_LIMIT_500MA      (500)
#define CHG_IINPUT_LIMIT_150MA      (150)
#define CHG_IINPUT_LIMIT_100MA      (100)

#define CHG_TERM_CURRENT_1024MA     (1024)
#define CHG_TERM_CURRENT_512MA      (512)
#define CHG_TERM_CURRENT_256MA      (256)
#define CHG_TERM_CURRENT_128MA      (128)

#define CHG_PRECHG_CURRENT_1024MA   (1024)
#define CHG_PRECHG_CURRENT_512MA    (512)
#define CHG_PRECHG_CURRENT_256MA    (256)
#define CHG_PRECHG_CURRENT_128MA    (128)

#define CHG_BAT_VREG_512MV_B7       (512)
#define CHG_BAT_VREG_256MV_B6       (256)
#define CHG_BAT_VREG_128MV_B5       (128)
#define CHG_BAT_VREG_64MV_B4        (64)
#define CHG_BAT_VREG_32MV_B3        (32)
#define CHG_BAT_VREG_16MV_B2        (16)

#define CHG_VINDPM_640MV_B6         (640)
#define CHG_VINDPM_320MV_B5         (320)
#define CHG_VINDPM_160MV_B4         (160)
#define CHG_VINDPM_80MV_B3          (80)


#define CHG_BOOSTV_OFFSET_512MV     (512)
#define CHG_BOOSTV_OFFSET_256MV     (256)
#define CHG_BOOSTV_OFFSET_128MV     (128)
#define CHG_BOOSTV_OFFSET_64MV      (64)

#define CHG_BCOLD_THOLD_MINUS20DEGC (-20)
#define CHG_BCOLD_THOLD_MINUS10DEGC (-10)
#define CHG_BHOT_THOLD_PLUS55DEGC   (55)
#define CHG_BHOT_THOLD_PLUS60DEGC   (60)
#define CHG_BHOT_THOLD_PLUS65DEGC   (65)

/*******************  REGISTER INFORMATION ************************************/
/****************0x00 Status/Control Register (Read/Write)*********************/
/*Memory Location: 00, Reset State: x1xx 0xxx*/
#define BQ24192_INPUT_CTL_REG        0x00
/*All 8 Register bits and relevant mask value*/
#define BQ24192_EN_HIZ_BITPOS        0x7
#define BQ24192_EN_HIZ              (0x1 << BQ24192_EN_HIZ_BITPOS)
#define BQ24192_DIS_HIZ             (0x0 << BQ24192_EN_HIZ_BITPOS)
#define BQ24192_EN_HIZ_MASK         (0x1 << BQ24192_EN_HIZ_BITPOS)

#define BQ24192_DPM_3_VOL_BITPO      0x6
#define BQ24192_DPM_VOL_640MV       (0x1 << BQ24192_DPM_3_VOL_BITPO)

#define BQ24192_DPM_2_VOL_BITPO      0x5
#define BQ24192_DPM_VOL_320MV       (0x1 << BQ24192_DPM_2_VOL_BITPO)

#define BQ24192_DPM_1_VOL_BITPO      0x4
#define BQ24192_DPM_VOL_160MV       (0x1 << BQ24192_DPM_1_VOL_BITPO)

#define BQ24192_DPM_0_VOL_BITPO      0x3
#define BQ24192_DPM_VOL_80MV        (0x1 << BQ24192_DPM_0_VOL_BITPO)
#define BQ24192_DPM_VOL_OFFSET       3880
#define BQ24192_DPM_VOL_MAX          5080
#define BQ24192_DPM_VOL_MASK        (0xF  << BQ24192_DPM_0_VOL_BITPO)

#define BQ24192_IINLIMIT_BITPOS      0
#define BQ24192_IINLIMIT_100        (0x0 << BQ24192_IINLIMIT_BITPOS)
#define BQ24192_IINLIMIT_150        (0x1 << BQ24192_IINLIMIT_BITPOS)
#define BQ24192_IINLIMIT_500        (0x2 << BQ24192_IINLIMIT_BITPOS)
#define BQ24192_IINLIMIT_900        (0x3 << BQ24192_IINLIMIT_BITPOS)
#define BQ24192_IINLIMIT_1200       (0x4 << BQ24192_IINLIMIT_BITPOS)

#define BQ24296_IINLIMIT_1000       (0x4 << BQ24192_IINLIMIT_BITPOS)
#define BQ24192_IINLIMIT_1500       (0x5 << BQ24192_IINLIMIT_BITPOS)
#define BQ24192_IINLIMIT_2000       (0x6 << BQ24192_IINLIMIT_BITPOS)
#define BQ24192_IINLIMIT_3000       (0x7 << BQ24192_IINLIMIT_BITPOS)
#define BQ24192_IINLIMIT_MASK       (0x7 << BQ24192_IINLIMIT_BITPOS)


/**************0x01 BAT/SUPPLY STATUS Register (Read/Write)********************/
/*Memory Location: 01, Reset State: xxxx 0xxx*/
#define BQ24192_POWER_ON_CFG_REG     0x01
/*All 8 Register bits and relevant mask value*/
#define BQ24192_RESET_BITPOS         0x7
#define BQ24192_RESET_ALL           (0x1 << BQ24192_RESET_BITPOS)
#define BQ24192_RESET_ALL_MASK      (0x1 << BQ24192_RESET_BITPOS)

#define BQ24192_TMR_RST_BITPOS       0x6
#define BQ24192_TMR_RST             (0x1 << BQ24192_TMR_RST_BITPOS)
#define BQ24192_TMR_RST_MASK        (0x1 << BQ24192_TMR_RST_BITPOS)

#define BQ24192_CHG_CFG_BITPOS       0x4
#define BQ24192_CHG_CFG_DIS         (0x0 << BQ24192_CHG_CFG_BITPOS)
#define BQ24192_CHG_CFG_BAT         (0x1 << BQ24192_CHG_CFG_BITPOS)
#define BQ24192_CHG_CFG_OTG         (0x2 << BQ24192_CHG_CFG_BITPOS)
#define BQ24192_CHG_CFG_MASK        (0x3 << BQ24192_CHG_CFG_BITPOS)

#define BQ24192_SYS_MIN_2_BITPOS     0x3
#define BQ24192_SYS_MIN_400MV       (0x1 << BQ24192_SYS_MIN_2_BITPOS)

#define BQ24192_SYS_MIN_1_BITPOS     0x2
#define BQ24192_SYS_MIN_200MV       (0x1 << BQ24192_SYS_MIN_1_BITPOS)

#define BQ24192_SYS_MIN_0_BITPOS     0x1
#define BQ24192_SYS_MIN_100MV       (0x1 << BQ24192_SYS_MIN_0_BITPOS)
#define BQ24192_SYS_MIN_MASK        (0x7 << BQ24192_SYS_MIN_0_BITPOS)
#define BQ24192_SYS_MIN_OFFSET       3000

#define BQ24192_BOOST_LIM_BITPOS     0x0
#define BQ24192_BOOST_LIM_500MA     (0x0 << BQ24192_BOOST_LIM_BITPOS)
#define BQ24192_BOOST_LIM_1300MA    (0x1 << BQ24192_BOOST_LIM_BITPOS)
#define BQ24296_BOOST_LIM_1000MA    (0x0 << BQ24192_BOOST_LIM_BITPOS)
#define BQ24296_BOOST_LIM_1500MA    (0x1 << BQ24192_BOOST_LIM_BITPOS)
#define BQ24192_BOOST_LIM_MASK      (0x1 << BQ24192_BOOST_LIM_BITPOS)


/****************0x02 CONTROL Register (Read/Write)****************************/
/*Memory Location: 02, Reset State: 1000 1100*/
#define BQ24192_CHG_CUR_CTL_REG      0x02
/*All the 8bits and relevant mask value*/
#define BQ24192_ICHARGE_5_BITPOS     0x7
#define BQ24192_ICHARGE_2048        (0x1 << BQ24192_ICHARGE_5_BITPOS)

#define BQ24192_ICHARGE_4_BITPOS     0x6
#define BQ24192_ICHARGE_1024        (0x1 << BQ24192_ICHARGE_4_BITPOS)

#define BQ24192_ICHARGE_3_BITPOS     0x5
#define BQ24192_ICHARGE_512         (0x1 << BQ24192_ICHARGE_3_BITPOS)

#define BQ24192_ICHARGE_2_BITPOS     0x4
#define BQ24192_ICHARGE_256         (0x1 << BQ24192_ICHARGE_2_BITPOS)

#define BQ24192_ICHARGE_1_BITPOS     0x3
#define BQ24192_ICHARGE_128         (0x1 << BQ24192_ICHARGE_1_BITPOS)

#define BQ24192_ICHARGE_0_BITPOS     0x2
#define BQ24192_ICHARGE_64          (0x1 << BQ24192_ICHARGE_0_BITPOS)
#define BQ24192_ICHARGE_OFFSET       512
#define BQ24192_ICHARGE_MAX          4544
#define BQ24192_ICHARGE_MASK        (0x1F << BQ24192_ICHARGE_0_BITPOS)
#define BQ24296_BCOLD_BITPOS        0x1
#define BQ24296_BCOLD_MASK          (0x1 << BQ24296_BCOLD_BITPOS)
#define BQ24296_BCOLD0_N10DEGC      (0x0 << BQ24296_BCOLD_BITPOS)
#define BQ24296_BCOLD1_N20DEGC      (0x1 << BQ24296_BCOLD_BITPOS)

#define BQ24192_FORCE_20PCT_BITPOS   0x0
#define BQ24192_FORCE_20PCT_EN      (0x1 << BQ24192_FORCE_20PCT_BITPOS)
#define BQ24192_FORCE_20PCT_DIS     (0x0 << BQ24192_FORCE_20PCT_BITPOS)
#define BQ24192_FORCE_20PCT_MASK    (0x1 << BQ24192_FORCE_20PCT_BITPOS)


/***************0x03 BAT_VOL/CTL Register (Read/Write)*************************/
/*Memory Location: 03, Reset State: 0001 0100*/
#define BQ24192_IPRECHG_TERM_CTL_REG 0x03
/*All the 8bits and relevant mask value*/
#define BQ24192_IPRECHG_3_BITPOS     0x7
#define BQ24192_IPRECHG_1024        (0x1 << BQ24192_IPRECHG_3_BITPOS)

#define BQ24192_IPRECHG_2_BITPOS     0x6
#define BQ24192_IPRECHG_512         (0x1 << BQ24192_IPRECHG_2_BITPOS)

#define BQ24192_IPRECHG_1_BITPOS     0x5
#define BQ24192_IPRECHG_256         (0x1 << BQ24192_IPRECHG_1_BITPOS)

#define BQ24192_IPRECHG_0_BITPOS     0x4
#define BQ24192_IPRECHG_128         (0x1 << BQ24192_IPRECHG_0_BITPOS)
#define BQ24192_IPRECHG_OFFSET       128
#define BQ24192_IPRECHG_MAX          2048
#define BQ24192_IPRECHG_MASK        (0xF << BQ24192_IPRECHG_0_BITPOS)

#define BQ24192_ITERM_3_BITPOS       0x3
#define BQ24192_ITERM_1024          (0x1 << BQ24192_ITERM_3_BITPOS)

#define BQ24192_ITERM_2_BITPOS       0x2
#define BQ24192_ITERM_512           (0x1 << BQ24192_ITERM_2_BITPOS)

#define BQ24192_ITERM_1_BITPOS       0x1
#define BQ24192_ITERM_256           (0x1 << BQ24192_ITERM_1_BITPOS)

#define BQ24192_ITERM_0_BITPOS       0x0
#define BQ24192_ITERM_128           (0x1 << BQ24192_ITERM_0_BITPOS)
#define BQ24192_ITERM_OFFSET         128
#define BQ24192_ITERM_MAX            2048
#define BQ24192_ITERM_MASK          (0xF << BQ24192_ITERM_0_BITPOS)


/*******************0x04 Vendor Register (Read only)***************************/
/*Memory Location: 04, Reset State: 0100 0000*/
#define BQ24192_CHG_VOLT_CTL_REG     0x04
/*All the 8bits and relevant mask value*/
#define BQ24192_BAT_REG_VOL_5_BITPOS 0x7
#define BQ24192_BAT_REG_VOL_512     (0x1 << BQ24192_BAT_REG_VOL_5_BITPOS)

#define BQ24192_BAT_REG_VOL_4_BITPOS 0x6
#define BQ24192_BAT_REG_VOL_256     (0x1 << BQ24192_BAT_REG_VOL_4_BITPOS)

#define BQ24192_BAT_REG_VOL_3_BITPOS 0x5
#define BQ24192_BAT_REG_VOL_128     (0x1 << BQ24192_BAT_REG_VOL_3_BITPOS)

#define BQ24192_BAT_REG_VOL_2_BITPOS 0x4
#define BQ24192_BAT_REG_VOL_64      (0x1 << BQ24192_BAT_REG_VOL_2_BITPOS)

#define BQ24192_BAT_REG_VOL_1_BITPOS 0x3
#define BQ24192_BAT_REG_VOL_32      (0x1 << BQ24192_BAT_REG_VOL_1_BITPOS)

#define BQ24192_BAT_REG_VOL_0_BITPOS 0x2
#define BQ24192_BAT_REG_VOL_16      (0x1 << BQ24192_BAT_REG_VOL_0_BITPOS)
#define BQ24192_BAT_REG_VOL_OFFSET   3504
#define BQ24192_BAT_REG_VOL_MAX      4400
#define BQ24192_BAT_REG_VOL_MASK    (0x3F << BQ24192_BAT_REG_VOL_0_BITPOS)

#define BQ24192_BATLOW_VOL_BITPOS    0x1
#define BQ24192_BATLOW_VOL          (0x1 << BQ24192_BATLOW_VOL_BITPOS)
#define BQ24192_BATLOW_VOL_MASK     (0x1 << BQ24192_BATLOW_VOL_BITPOS)

#define BQ24192_RECHG_VOL_BITPOS     0x0
#define BQ24192_RECHG_VOL           (0x1 << BQ24192_RECHG_VOL_BITPOS)
#define BQ24192_RECHG_VOL_MASK      (0x1 << BQ24192_RECHG_VOL_BITPOS)


/*****0x05 BATTERY_CUTOFF_CURRENT/FAST_CHG_CURRENT Register (Read / Write)*****/
/*Memory Location: 05, Reset State: 0011 0010*/
#define BQ24192_TERM_TIMER_CTL_REG   0x05
/*All the 8bits and relevant mask value*/
#define BQ24192_TERM_CUR_BITPOS      0x7
#define BQ24192_FBD_TERM_CUR        (0x0 << BQ24192_TERM_CUR_BITPOS)
#define BQ24192_EN_TERM_CUR         (0x1 << BQ24192_TERM_CUR_BITPOS)
#define BQ24192_TERM_CUR_MASK       (0x1 << BQ24192_TERM_CUR_BITPOS)

#define BQ24192_TERM_STAT_BITPOS     0x6
#define BQ24192_EN_MATCH_ITERM      (0x0 << BQ24192_TERM_STAT_BITPOS)
#define BQ24192_DIS_MATCH_ITERM     (0x1 << BQ24192_TERM_STAT_BITPOS)
#define BQ24192_MATCH_ITERM_MASK    (0x1 << BQ24192_TERM_STAT_BITPOS)

#define BQ24192_WATCHDOG_TMR_BITPOS  0x4
#define BQ24192_WATCHDOG_TMR_DIS    (0x0 << BQ24192_WATCHDOG_TMR_BITPOS)
#define BQ24192_WATCHDOG_TMR_40     (0x1 << BQ24192_WATCHDOG_TMR_BITPOS)
#define BQ24192_WATCHDOG_TMR_80     (0x2 << BQ24192_WATCHDOG_TMR_BITPOS)
#define BQ24192_WATCHDOG_TMR_160    (0x3 << BQ24192_WATCHDOG_TMR_BITPOS)
#define BQ24192_WATCHDOG_TMR_MASK   (0x3 << BQ24192_WATCHDOG_TMR_BITPOS)

#define BQ24192_CHG_TIMER_EN_BITPOS  0x3
#define BQ24192_CHG_TIMER_EN        (0x1 << BQ24192_CHG_TIMER_BITPOS)
#define BQ24192_CHG_TIMER_DIS       (0x0 << BQ24192_CHG_TIMER_BITPOS)
#define BQ24192_CHG_TIMER_EN_MASK   (0x1 << BQ24192_CHG_TIMER_BITPOS)

#define BQ24192_CHG_TIMER_BITPOS     0x1
#define BQ24192_CHG_TIMER_5HRS      (0x00 << BQ24192_CHG_TIMER_BITPOS)
#define BQ24192_CHG_TIMER_8HRS      (0x01 << BQ24192_CHG_TIMER_BITPOS)
#define BQ24192_CHG_TIMER_12HRS     (0x10 << BQ24192_CHG_TIMER_BITPOS)
#define BQ24192_CHG_TIMER_20HRS     (0x11 << BQ24192_CHG_TIMER_BITPOS)
#define BQ24192_CHG_TIMER_MASK      (0x11 << BQ24192_CHG_TIMER_BITPOS)


/************0x06 DPM STATUS Register (Read / Write)***************************/
/*Memory Location: 06, Reset State: xx00 0000*/
#define BQ24192_IRCOMP_THERM_CTL_REG 0x06
/*All the 8bits and relevant mask value*/
#define BQ24192_IR_BATCOMP_2_BITPOS  0x7
#define BQ24192_IR_BATCOMP_40MOHM   (0x1 << BQ24192_IR_BATCOMP_2_BITPOS)

#define BQ24192_IR_BATCOMP_1_BITPOS  0x6
#define BQ24192_IR_BATCOMP_20MOHM   (0x1 << BQ24192_IR_BATCOMP_1_BITPOS)

#define BQ24192_IR_BATCOMP_0_BITPOS  0x5
#define BQ24192_IR_BATCOMP_10MOHM   (0x1 << BQ24192_IR_BATCOMP_0_BITPOS)
#define BQ24192_IR_BATCOMP_OFFSET    0
#define BQ24192_IR_BATCOMP_MASK     (0x7 << BQ24192_IR_BATCOMP_0_BITPOS)

#define BQ24192_IR_VCLAMP_2_BITPOS   0x4
#define BQ24192_IR_VCLAMP_64        (0x1 << BQ24192_IR_VCLAMP_2_BITPOS)

#define BQ24192_IR_VCLAMP_1_BITPOS   0x3
#define BQ24192_IR_VCLAMP_32        (0x1 << BQ24192_IR_VCLAMP_1_BITPOS)

#define BQ24192_IR_VCLAMP_0_BITPOS   0x2
#define BQ24192_IR_VCLAMP_16        (0x1 << BQ24192_IR_VCLAMP_0_BITPOS)
#define BQ24192_IR_VCLAMP_OFFSET     0
#define BQ24192_IR_VCLAMP_MASK      (0x7 << BQ24192_IR_VCLAMP_0_BITPOS)

#define BQ24192_TERM_REG_BITPOS      0x0
#define BQ24192_TERM_REG_60         (0x0 << BQ24192_TERM_REG_BITPOS)
#define BQ24192_TERM_REG_80         (0x1 << BQ24192_TERM_REG_BITPOS)
#define BQ24192_TERM_REG_100        (0x2 << BQ24192_TERM_REG_BITPOS)
#define BQ24192_TERM_REG_120        (0x3 << BQ24192_TERM_REG_BITPOS)
#define BQ24192_TERM_REG_MASK       (0x3 << BQ24192_TERM_REG_BITPOS)



/************************0x06 (Read / Write) **********************************/
/************* Boost Voltage/Thermal Regulation Control Register **************/
/*Memory Location: 06, Reset State: OB0111 0011 or 0x73*/
#define BQ24x96_BOOSTV_THERMREG_REG 0x06
/*All the 8bits and relevant mask value*/
#define BQ24296_BOOSTV_BITPOS       0x4
#define BQ24296_BOOSTV_MASK         (0xF << BQ24296_BOOSTV_BITPOS)
#define BQ24296_BOOSTV_512MV        (0x8 << BQ24296_BOOSTV_BITPOS)
#define BQ24296_BOOSTV_256MV        (0x4 << BQ24296_BOOSTV_BITPOS)
#define BQ24296_BOOSTV_128MV        (0x2 << BQ24296_BOOSTV_BITPOS)
#define BQ24296_BOOSTV_64MV         (0x1 << BQ24296_BOOSTV_BITPOS)
#define BQ24296_BOOSTV_OFFSET       4550
#define BQ24296_BOOSTV_MAX          5510

#define BQ24296_BHOT_BITPOS         0x2
#define BQ24296_BHOT_MASK           (0x3 << BQ24296_BHOT_BITPOS)
#define BQ24296_BHOT1_55DEGC        (0x0 << BQ24296_BHOT_BITPOS)
#define BQ24296_BHOT0_60DEGC        (0x1 << BQ24296_BHOT_BITPOS)
#define BQ24296_BHOT2_65DEGC        (0x2 << BQ24296_BHOT_BITPOS)
#define BQ24296_BHOT_DISABLE        (0x3 << BQ24296_BHOT_BITPOS)

#define BQ24x96_TREG_BITPOS         0x0
#define BQ24x96_TREG_MASK           (0x3 << BQ24x96_TREG_BITPOS)
#define BQ24x96_TREG_60DEGC         (0x0 << BQ24x96_TREG_BITPOS)
#define BQ24x96_TREG_80DEGC         (0x1 << BQ24x96_TREG_BITPOS)
#define BQ24x96_TREG_100DEGC        (0x2 << BQ24x96_TREG_BITPOS)
#define BQ24x96_TREG_120DEGC        (0x3 << BQ24x96_TREG_BITPOS)


/*********0x07 Safety Timer / NTC Monitor Register (Read / Write)**************/
/*Memory Location: 07, Reset State: 1001 1xxx*/
#define BQ24192_MISC_OP_CTL_REG      0x07
/*All the 8bits and relevant mask value*/
#define BQ24192_DPDM_EN_BITPOS       0x7
#define BQ24192_DPDM_EN             (0x1 << BQ24192_DPDM_EN_BITPOS)
#define BQ24192_DPDM_DIS            (0x0 << BQ24192_DPDM_EN_BITPOS)
#define BQ24192_DPDM_MASK           (0x1 << BQ24192_DPDM_EN_BITPOS)

#define BQ24192_TMR2X_EN_BITPOS      0x6
#define BQ24192_TMR2X_EN            (0x1 << BQ24192_TMR2X_EN_BITPOS)
#define BQ24192_TMR2X_DIS           (0x0 << BQ24192_TMR2X_EN_BITPOS)
#define BQ24192_TMR2X_MASK          (0x1 << BQ24192_TMR2X_EN_BITPOS)

#define BQ24192_BATFET_OFF_BITPOS    0x5
#define BQ24192_BATFET_OFF_EN       (0x1 << BQ24192_BATFET_OFF_BITPOS)
#define BQ24192_BATFET_OFF_DIS      (0x0 << BQ24192_BATFET_OFF_BITPOS)
#define BQ24192_BATFET_OFF_MASK     (0x1 << BQ24192_BATFET_OFF_BITPOS)

#define BQ24192_INT_ON_CHGFLT_BITPOS 0x1
#define BQ24192_INT_ON_CHGFLT_EN    (0x1 << BQ24192_INT_ON_CHGFLT_BITPOS)
#define BQ24192_INT_ON_CHGFLT_DIS   (0x0 << BQ24192_INT_ON_CHGFLT_BITPOS)
#define BQ24192_INT_ON_CHGFLT_MASK  (0x1 << BQ24192_INT_ON_CHGFLT_BITPOS)

#define BQ24192_INT_ON_BATFLT_BITPOS 0x0
#define BQ24192_INT_ON_BATFLT_EN    (0x1 << BQ24192_INT_ON_BATFLT_BITPOS)
#define BQ24192_INT_ON_BATFLT_DIS   (0x0 << BQ24192_INT_ON_BATFLT_BITPOS)
#define BQ24192_INT_ON_BATFLT_MASK  (0x1 << BQ24192_INT_ON_BATFLT_BITPOS)


/*********0x08 System State Register Register (Read Only)**************/
/*Memory Location: 08, Reset State: xxxx xxxx*/
#define BQ24192_SYS_STAT_REG         0x08
/*All the 8bits and relevant mask value*/
#define BQ24192_VBUS_STAT_BITPOS     0x6
#define BQ24192_VBUS_STAT_UNKNOWN   (0x0 << BQ24192_VBUS_STAT_BITPOS)
#define BQ24192_VBUS_STAT_USB       (0x1 << BQ24192_VBUS_STAT_BITPOS)
#define BQ24192_VBUS_STAT_ADAPTER   (0x2 << BQ24192_VBUS_STAT_BITPOS)
#define BQ24192_VBUS_STAT_OTG       (0x3 << BQ24192_VBUS_STAT_BITPOS)
#define BQ24192_VBUS_STAT_MASK      (0x3 << BQ24192_VBUS_STAT_BITPOS)

#define BQ24192_CHRG_STAT_BITPOS     0x4
#define BQ24192_CHRG_STAT_NOCHG     (0x0 << BQ24192_CHRG_STAT_BITPOS)
#define BQ24192_CHRG_STAT_PRECHG    (0x1 << BQ24192_CHRG_STAT_BITPOS)
#define BQ24192_CHRG_STAT_FASTCHG   (0x2 << BQ24192_CHRG_STAT_BITPOS)
#define BQ24192_CHRG_STAT_CHGDONE   (0x3 << BQ24192_CHRG_STAT_BITPOS)
#define BQ24192_CHRG_STAT_MASK      (0x3 << BQ24192_CHRG_STAT_BITPOS)

#define BQ24192_DPM_STAT_BITPOS      0x3
#define BQ24192_DPM_STAT            (0x1 << BQ24192_DPM_STAT_BITPOS)
#define BQ24192_NODPM_STAT          (0x0 << BQ24192_DPM_STAT_BITPOS)
#define BQ24192_DPM_STAT_MASK       (0x1 << BQ24192_DPM_STAT_BITPOS)

#define BQ24192_PG_STAT_BITPOS       0x2
#define BQ24192_PG_STAT             (0x1 << BQ24192_PG_STAT_BITPOS)
#define BQ24192_NOPG_STAT           (0x0 << BQ24192_PG_STAT_BITPOS)
#define BQ24192_PG_STAT_MASK        (0x1 << BQ24192_PG_STAT_BITPOS)

#define BQ24192_THERM_STAT_BITPOS    0x1
#define BQ24192_THERM_STAT_NORMAL   (0x1 << BQ24192_THERM_STAT_BITPOS)
#define BQ24192_THERM_STAT_THERMAL  (0x0 << BQ24192_THERM_STAT_BITPOS)
#define BQ24192_THERM_STAT_MASK     (0x1 << BQ24192_THERM_STAT_BITPOS)

#define BQ24192_VSYS_STAT_BITPOS     0x0
#define BQ24192_VSYS_STAT_INSYSMIN  (0x1 << BQ24192_VSYS_STAT_BITPOS)
#define BQ24192_VSYS_STAT_NOSYSMIN  (0x0 << BQ24192_VSYS_STAT_BITPOS)
#define BQ24192_VSYS_STAT_MASK      (0x1 << BQ24192_VSYS_STAT_BITPOS)

/*********0x09 Fault Register Register (Read Only)**************/
/*Memory Location: 09, Reset State: xxxx xxxx*/
#define BQ24192_FAULT_REG            0x09
/*All the 8bits and relevant mask value*/
#define BQ24192_WATCHDOG_FLT_BITPOS  0x7
#define BQ24192_WATCHDOG_EXP        (0x1 << BQ24192_WATCHDOG_FLT_BITPOS)
#define BQ24192_WATCHDOG_NORMAL     (0x0 << BQ24192_WATCHDOG_FLT_BITPOS)
#define BQ24192_WATCHDOG_FLT_MASK   (0x1 << BQ24192_WATCHDOG_FLT_BITPOS)

#define BQ24192_BOOST_FLT_BITPOS     0x6
#define BQ24192_BOOST_FLT           (0x1 << BQ24192_BOOST_FLT_BITPOS)
#define BQ24192_BOOST_NORMAL        (0x0 << BQ24192_BOOST_FLT_BITPOS)
#define BQ24192_BOOST_FLT_MASK      (0x1 << BQ24192_BOOST_FLT_BITPOS)

#define BQ24192_CHRG_FLT_BITPOS      0x4
#define BQ24192_CHRG_NORMAL         (0x0 << BQ24192_CHRG_FLT_BITPOS)
#define BQ24192_INPUT_FLT           (0x1 << BQ24192_CHRG_FLT_BITPOS)
#define BQ24192_THERMAL_SHUTDOWM    (0x2 << BQ24192_CHRG_FLT_BITPOS)
#define BQ24192_CHRG_TIMER_EXP      (0x3 << BQ24192_CHRG_FLT_BITPOS)
#define BQ24192_CHRG_FLT_MASK       (0x3 << BQ24192_CHRG_FLT_BITPOS)

#define BQ24192_BAT_FLT_BITPOS       0x3
#define BQ24192_BAT_OVP             (0x1 << BQ24192_BAT_FLT_BITPOS)
#define BQ24192_BAT_NORMAL          (0x0 << BQ24192_BAT_FLT_BITPOS)
#define BQ24192_BAT_FLT_MASK        (0x1 << BQ24192_BAT_FLT_BITPOS)


#define BQ24192_NTC_FLT_BITPOS       0x0
#define BQ24192_NTC_NORMAL          (0x0 << BQ24192_NTC_FLT_BITPOS)
#define BQ24192_TS1_COLD            (0x1 << BQ24192_NTC_FLT_BITPOS)
#define BQ24192_TS1_HOT             (0x2 << BQ24192_NTC_FLT_BITPOS)
#define BQ24192_TS2_COLD            (0x3 << BQ24192_NTC_FLT_BITPOS)
#define BQ24192_TS2_HOT             (0x4 << BQ24192_NTC_FLT_BITPOS)
#define BQ24192_BOTH_COLD           (0x5 << BQ24192_NTC_FLT_BITPOS)
#define BQ24192_BOTH_HOT            (0x6 << BQ24192_NTC_FLT_BITPOS)
#define BQ24192_NTC_FLT_MASK        (0x7 << BQ24192_NTC_FLT_BITPOS)
#define BQ24196_NTC_NORMAL          (0x0 << BQ24192_NTC_FLT_BITPOS)
#define BQ24196_TS_COLD_VAL         (0x5 << BQ24192_NTC_FLT_BITPOS)
#define BQ24196_TS_HOT_VAL          (0x6 << BQ24192_NTC_FLT_BITPOS)
#define BQ24296_NTC_NORMAL          (0x0 << BQ24192_NTC_FLT_BITPOS)
#define BQ24296_TS_HOT_VAL          (0x1 << BQ24192_NTC_FLT_BITPOS)
#define BQ24296_TS_COLD_VAL         (0x2 << BQ24192_NTC_FLT_BITPOS)
#define BQ24x96_NTC_FLT_MASK        (0x7 << BQ24192_NTC_FLT_BITPOS)

/*********0x0A Vender Register Register (Read Only)**************/

/*Memory Location: 0A, Fix State: 0010 1011*/
#define BQ24x96_REVISION_REG        0x0A
/*BQ24296/BQ24297 Vendor/Part/Revision Reg Info.*/
#define BQ2429x_PART_NR_BITPOS      0x5
#define BQ2429x_PART_NR_MASK        (0x7 << BQ2429x_PART_NR_BITPOS)
#define BQ24296_PART_NR_VALUE       (0x1 << BQ2429x_PART_NR_BITPOS)
#define BQ24297_PART_NR_VALUE       (0x3 << BQ2429x_PART_NR_BITPOS)

#define BQ2429x_REVISION_BITPOS     0x0
#define BQ2429x_REVISION_MASK       (0x7 << BQ2429x_REVISION_BITPOS)
#define BQ2429x_REVISION_1P0        (0x0 << BQ2429x_REVISION_BITPOS)
#define BQ2429x_REVISION_1P1        (0x1 << BQ2429x_REVISION_BITPOS)

/*BQ24296/BQ24297 Vendor/Part/Revision Reg Info.*/
#define BQ2419x_PART_NR_BITPOS      0x3
#define BQ2419x_PART_NR_MASK        (0x7 << BQ2419x_PART_NR_BITPOS)
#define BQ24190_PART_NR_VALUE       (0x4 << BQ2419x_PART_NR_BITPOS)
#define BQ24196_PART_NR_VALUE       (0x5 << BQ2419x_PART_NR_BITPOS)

#define BQ2419x_REVISION_BITPOS     0x0
#define BQ2419x_REVISION_MASK       (0x7 << BQ2419x_REVISION_BITPOS)
#define BQ2419x_REVISION_VAL        (0x3 << BQ2419x_REVISION_BITPOS)

#if 0 /*PN:dongle121, chendongguo, 2013/6/27 deleted*/
#define BQ24192_VENDER_REG           0x6B
#endif

/*****************************内部结构体定义********************************/
typedef enum
{
    CHG_STAT_UNKNOWN,
    CHG_STAT_USB_HOST,
    CHG_STAT_ADAPTER_PORT,
    CHG_STAT_OTG
}chg_dcdc_vbus_stat;

typedef enum
{
    CHG_STAT_NOT_CHARGING,
    CHG_STAT_PRE_CHARGING,
    CHG_STAT_FAST_CHARGING,
    CHG_STAT_CHARGE_DONE
}chg_dcdc_chrg_stat;

typedef enum
{
    CHG_WATCHDOG_NORMAL,
    CHG_WATCHDOG_EXP
}chg_dcdc_watchdog_fault;

typedef enum
{
    CHG_FAULT_NORMAL,
    CHG_FAULT_INPUT_FLT,
    CHG_FAULT_THERMAL_SHUTDOWN,
    CHG_FAULT_SAFTY_TIMER_EXP
}chg_dcdc_chrg_fault;

typedef enum
{
    CHG_BAT_NORMAL,
    CHG_BAT_OVP
}chg_dcdc_bat_fault;

/*充电芯片电池状态的数据结构类型定义*/
typedef enum
{
    CHG_BATT_NORMAL,
    CHG_BATT_OVP,
    CHG_BATT_ABSENT,
    CHG_BATT_INVALID
}chg_dcdc_batt_status;

/*充电CE状态*/
typedef enum
{
    CHG_CONFIG_CHG_DIS,
    CHG_CONFIG_CHG_BAT,
    CHG_CONFIG_CHG_OTG,
}chg_dcdc_ce_status;

/*充电芯片所有状态的数据结构类型定义*/
typedef struct
{
    chg_dcdc_vbus_stat          chg_vbus_stat;
    chg_dcdc_chrg_stat          chg_chrg_stat;
    chg_dcdc_watchdog_fault     chg_watchdog_fault;
    chg_dcdc_chrg_fault         chg_chrg_fault;
    chg_dcdc_bat_fault          chg_bat_fault;
    chg_dcdc_ce_status          chg_ce_stat;
}chg_status_type;

/*充电芯片控制相关的数据结构类型定义*/
typedef enum
{
    LOW_CTRL_VAL,
    HIGH_CTRL_VAL
}
chg_charge_control_value;

typedef enum
{
    CHG_5HOUR_SAFETY_TIMER,
    CHG_8HOUR_SAFETY_TIMER,
    CHG_12HOUR_SAFETY_TIMER,
    CHG_20HOUR_SAFETY_TIMER,
    CHG_DISABLE_SAFETY_TIMER
}
chg_safety_timer_value;

typedef enum
{
    CHG_DISABLE_WATCHDOG_TIMER,
    CHG_40S_WATCHDOG_TIMER,
    CHG_80S_WATCHDOG_TIMER,
    CHG_160S_WATCHDOG_TIMER
}
chg_watchdog_timer_value;

typedef enum
{
    CHG_STOP_COMPLETE,
    CHG_STOP_TIMEOUT,
    CHG_STOP_INVALID_TEMP,
    CHG_STOP_BY_SW,
    CHG_STOP_SUPPLY_ERR,
    CHG_STOP_BAT_ERR,
    CHG_STOP_OTHER_REASON
}
chg_stop_reason;

typedef enum
{
    CHG_SUPPLY_PREFER_IN,
    CHG_SUPPLY_PREFER_USB
}
chg_supply_prefer_value;
typedef enum
{
    CHG_CHIP_PN_BQ24196,
    CHG_CHIP_PN_BQ24296,
    CHG_CHIP_PN_INVALID
}
chg_chip_part_nr;

/******************************************************************************
Function:       chg_boot_set_cur_level
Description:    设定充电芯片对电池的充电电流
Input:          充电电流值
Output:         N/A
Return:         TRUE     : 函数执行成功
                FALSE    : 函数执行失败
Others:         N/A
******************************************************************************/
boolean chg_boot_set_cur_level(uint32_t chg_cur);

/******************************************************************************
  Function      chg_boot_set_supply_limit
  Description   设定输入电流限制
  Input         输入电流值
  Output        N/A
  Return        TRUE      : 函数执行成功
                FALSE     : 函数执行失败
  Others        N/A
******************************************************************************/
boolean chg_boot_set_supply_limit(uint32_t lmt_val);

/******************************************************************************
  Function      chg_boot_set_term_current
  Description   设置充电截止电流
  Input         截止电流值
  Output        N/A
  Return        TRUE      : 函数执行成功
                FALSE     : 函数执行失败
  Others        N/A
******************************************************************************/
boolean chg_boot_set_term_current(uint32_t term_val);

/******************************************************************************
  Function      chg_boot_set_charge_enable
  Description   设定是否使能充电
  Input         en_val:TRUE   表明使能充电
                       FALSE  表明禁止充电
  Output        N/A
  Return        TRUE      : 函数执行成功
                FALSE     : 函数执行失败
  Others        N/A
******************************************************************************/
boolean chg_boot_set_charge_enable(boolean enable);

/******************************************************************************
  Function      chg_boot_set_vreg_level
  Description   设置恒压充电电压
  Input         充电恒压电压值
  Output        N/A
  Return        TRUE      : 函数执行成功
                FALSE     : 函数执行失败
  Others        N/A
******************************************************************************/
boolean chg_boot_set_vreg_level(uint32_t vreg_val);

/******************************************************************************
  Function      chg_boot_set_suspend_mode
  Description   设置充电IC为suspend模式
  Input         hz_mode: TRUE   表明设定充电芯片进入suspend模式
                         FALSE  表明设定芯片退出suspend模式
  Output        N/A
  Return        TRUE      : 函数执行成功
                FALSE     : 函数执行失败
  Others        N/A
******************************************************************************/
boolean chg_boot_set_suspend_mode(boolean enable);

/******************************************************************************
  Function      chg_boot_set_supply_prefer
  Description   设定充电芯片输入端的优先级
  Input         chg_cur: CHG_SUPPLY_PREFER_IN   表明输入优先级为IN优先
                         CHG_SUPPLY_PREFER_USB  表明输入优先级为USB优先
  Output        N/A
  Return        TRUE      : 函数执行成功
                FALSE     : 函数执行失败
  Others        N/A
******************************************************************************/
boolean chg_boot_set_supply_prefer(chg_supply_prefer_value sup_val);

/******************************************************************************
  Function      chg_boot_set_te_enable
  Description   设定是否使能截止电流
  Input         chg_cur: TRUE   表明使能充电截止电流，充电电流达到设定的
                                截止值后充电就会停止
                         FALSE  表明禁止充电截止电流
  Output        N/A
  Return        TRUE      : 函数执行成功
                FALSE     : 函数执行失败
  Others        N/A
******************************************************************************/
boolean chg_boot_set_te_enable(boolean enable);

/******************************************************************************
  Function      chg_boot_set_ts_enable
  Description   设定芯片温度保护功能使能
  Input         chg_cur: TRUE   使能芯片自带的温度保护功能
                         FALSE  禁用芯片自带的温度保护功能
  Output        N/A
  Return        TRUE      : 函数执行成功
                FALSE     : 函数执行失败
  Others        N/A
******************************************************************************/
boolean chg_boot_set_ts_enable(boolean enable);

/******************************************************************************
  Function      chg_boot_set_charge_mode
  Description   设定芯片是否进行涓流充电
  Input         chg_cur: TRUE   使能LOW_CHG功能，芯片以100mA小电流对
                         电池进行充电
                         FALSE  禁用LOW_CHG功能，芯片以0x05寄存器设定
                         的充电电流对电池进行充电
  Output        N/A
  Return        TRUE      : 函数执行成功
                FALSE     : 函数执行失败
  Others        N/A
******************************************************************************/
boolean chg_boot_set_charge_mode(boolean enable);

/******************************************************************************
  Function      chg_boot_set_stat_enable
  Description   设定芯片是否使能STAT比特位
  Input         chg_cur: TRUE   使能STAT比特位
                         FALSE  禁用STAT比特位
  Output        N/A
  Return        TRUE      : 函数执行成功
                FALSE     : 函数执行失败
  Others        N/A
******************************************************************************/
boolean chg_boot_set_stat_enable(boolean enable);

/******************************************************************************
  Function      chg_boot_set_dpm_val
  Description   设定DPM值，即当充电器输入电压小于该设定值之后，芯片会避免
                输入电压的进一步下降(避免输入电流无限制增大导致损坏充电设备)
                从而逐渐减小甚至停止对电池进行充电，以全部电流用于给系统供电
                目前DPM设定值为4.2V
  Input         N/A
  Output        N/A
  Return        TRUE      : 函数执行成功
                FALSE     : 函数执行失败
  Others        N/A
******************************************************************************/
boolean chg_boot_set_dpm_val(uint32_t dpm_val);

/******************************************************************************
  Function      chg_boot_set_safety_timer_enable
  Description   设定是否使能充电芯片的安全计时器功能
  Input         chg_cur: 00 (01,10) 使能安全计时器，芯片默认使能，并以27min
                         对快速充电时间进行限制(6h,9h)
                         11  禁用安全计时器
  Output        N/A
  Return        TRUE      : 函数执行成功
                FALSE     : 函数执行失败
  Others        N/A
******************************************************************************/
boolean chg_boot_set_safety_timer_enable(chg_safety_timer_value tmr_val);

/******************************************************************************
  Function      chg_boot_set_tmr_rst
  Description   对充电芯片执行踢狗操作
  Input         N/A
  Output        N/A
  Return        TRUE      : 函数执行成功
                FALSE     : 函数执行失败
  Others        N/A
******************************************************************************/
boolean chg_boot_set_tmr_rst(void);

/******************************************************************************
  Function      chg_boot_reset_all_reg
  Description   重置所有BQ24192寄存器至默认配置
  Input         N/A
  Output        N/A
  Return        TRUE      : 函数执行成功
                FALSE     : 函数执行失败
  Others        N/A
******************************************************************************/
boolean chg_boot_reset_all_reg(void);

/******************************************************************************
  Function      chg_boot_get_IC_status
  Description   获取充电IC状态，读取充电芯片所有寄存器的值，并识别其中的只读
                比特位，写入chg_status_type结构体中，该结构体能体现所有充电芯片
                状态
  Input         N/A
  Output        chg_status_type: 该结构中，
                chg_dcdc_status           表明总体充电器状态，
                chg_dcdc_fault_status     表明充电是否出现错误，
                chg_dcdc_in_supply_status 表明IN输入源是否正常，
                chg_dcdc_usb_supply_status表明USB输入源是否正常，
                chg_dcdc_batt_status      表明电池是否正常
  Return        TRUE      : 函数执行成功
                FALSE     : 函数执行失败
  Others        N/A
******************************************************************************/
boolean chg_boot_get_IC_status(chg_status_type *chg_stat_ptr);

/******************************************************************************
  Function      chg_boot_is_charger_present
  Description   外电源是否在位
  Input         N/A
  Output        N/A
  Return        FALSE     : 外电源不在位
                TRUE      : 外电源在位
  Others        N/A
******************************************************************************/
boolean chg_boot_is_charger_present(void);

/******************************************************************************
  Function      chg_boot_is_IC_charging
  Description   充电IC是否在充电
  Input         N/A
  Output        N/A
  Return        TRUE     : 正在充电
                FALSE    : 停止充电
  Others        N/A
******************************************************************************/
boolean chg_boot_is_IC_charging(void);

/******************************************************************************
  Function      chg_boot_get_stop_charging_reason
  Description   获取充电停止原因
  Input         N/A
  Output        N/A
  Return        充电停止原因chg_stop_reason:
                CHG_STOP_COMPLETE,
                CHG_STOP_TIMEOUT,
                CHG_STOP_INVALID_TEMP,
                CHG_STOP_BY_SW
  Others        N/A
******************************************************************************/
chg_stop_reason chg_boot_get_stop_charging_reason(void);

/******************************************************************************
  Function      chg_boot_set_chip_cd
  Description   设置芯片片选使能
  Input         拉低芯片对应的CD的GPIO表示使能芯片，拉高表示关闭芯片
  Output        N/A
  Return        TRUE      : 函数执行成功
                FALSE     : 函数执行失败
  Others        N/A
******************************************************************************/
boolean chg_boot_set_chip_cd(boolean enable);


/******************************************************************************
  Function      chg_boot_set_boost_therm_protect_threshold
  Description   设置充电芯片BOOST模式下温度保护门限值
  Input         temp      : 待设置的门限值温度, 0 表示禁用保护功能。
  Output        N/A
  Return        TRUE      : 函数执行成功
                FALSE     : 函数执行失败
  Others        该接口目前仅BQ24296芯片支持，其他芯片直接返回TRUE.
******************************************************************************/
boolean chg_boot_set_boost_therm_protect_threshold(int32_t temp);

/******************************************************************************
  Function      chg_boot_set_boost_volt
  Description   设置充电芯片BOOST模式电压值
  Input         待设置的Boost模式电压值
  Output        N/A
  Return        TRUE      : 函数执行成功
                FALSE     : 函数执行失败
  Others        该接口目前仅BQ24296芯片支持，其他芯片返回TRUE.
******************************************************************************/
boolean chg_boot_set_boost_volt(uint32_t boostv);

/******************************************************************************
  Function      chg_boot_get_part_nr
  Description   获取充电芯片的型号，目前支持bq24196与bq24296
  Input         N/A
  Output        pn        : 返回的芯片型号
  Return        TRUE      : 函数执行成功
                FALSE     : 函数执行失败
  Others        N/A
******************************************************************************/
boolean chg_boot_get_part_nr(chg_chip_part_nr *pn);

/******************************************************************************
  Function      chg_boot_chip_init
  Description   充电芯片初始化
  Input         N/A
  Output        N/A
  Return        TRUE      : 函数执行成功
                FALSE     : 函数执行失败
  Others        N/A
******************************************************************************/
boolean chg_boot_chip_init(void);

#endif
