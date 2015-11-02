/******************************************************************************

                  版权所有 (C), 2001-2011, 华为终端(东莞)有限公司

 ******************************************************************************
  文 件 名   : chg_boot.h
  版 本 号   : 初稿
  作    者   : chendongguo
  生成日期   : 2013年6月28日
  最近修改   :
  功能描述   : BOOT涓流充电头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2013年6月28日
    作    者   : chendongguo
    修改内容   : 创建文件

******************************************************************************/
/******************************修改记录***************************************************************************************
    问题单号         时间          修改人            说明
******************************************************************************************************************************/
#ifndef CHG_BOOT_CHARGE_H
#define CHG_BOOT_CHARGE_H

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "chg_boot_charge_drv.h"
#include "chg_boot_chip_platform.h"
#include "product_config.h"

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

#define  BOOT_BATT_SHORTED_CHECK_TIME    (3)    //   连续检测三次，共36s 
#define  BOOT_PLUS_CHARGING_ON_TIME      (10)
#define  BOOT_PLUS_CHARGING_ON_MAX_TIME  (25)
#define  BOOT_PLUS_CHARGING_OFF_TIME     (1)
#define  BOOT_TRICKLE_CHARGING_TIME      (10)
#define  BATT_NOT_PRESENCE_TEMP          (-30)

#define  BATT_STOP_CHARGINE_LOW_TEMP     (0)
#define  BATT_STOP_CHARGINE_HIGH_TEMP    (55)

#define  BATT_RECHARGINE_LOW_TEMP        (3)
#define  BATT_RECHARGINE_HIGH_TEMP       (52)

#define  BOOT_TRICKLE_CHARGE_TIME        (180)

#define BAT_CHECK_JUDGMENT_VOLT          (3000)
#define BAT_CHECK_HIGH_VOLT              (3800)

#define BAT_CHECK_POWERON_VOLT           (3450)
#define BAT_CHECK_OPEN_VOLT              (3500)
#define BAT_CHECK_SHORTED_VOLT           (1000)
#define BAT_CHECK_SHORTED_TIMES          (3)

//定义电池检测的返回状态

#if 0 /*PN:dongle121, chendongguo, 2013/6/26 deleted*/
typedef enum
{
    CHARGE_BATTERY_GOOD = 0, /*电池/charge 在位，并且电池是好的 voltage >= 3.0 */
    CHARGE_BATTERY_ONLY,      /*电池在位(no charge)，并且达到开机门限( >= 3.4v)*/
    CHARGE_BATTERY_LOWER,   /* 弱电池( no charge ) ==> power off. < 3.4 */
    CHARGE_BATTERY_BAD,           /*坏电池   */
    BATTERY_BAD_WITHOUT_CHARGE,/*电源拔出坏电池状态*//*须评估是否要增加*/
    CHARGE_BATTERY_MAX,
}CHG_CHARGE_BATT_CHECK_TYPE;
#endif


/* BEGIN: PN:dongle121,chendongguo, 2013/6/26 modified*/
/*待巴隆代码删除后打开*/
#if (MBB_CHG_PLATFORM_V7R2 == FEATURE_OFF)
typedef enum
{
    CHARGE_BATTERY_GOOD = 0,/*电池/charge 在位，并且电池是好的 voltage >= 3.0. go on  **/
    CHARGE_BATTERY_ONLY,    /*电池在位(no charge)，并且达到开机门限( >= 3.4v), go on*/
    CHARGE_BATTERY_LOWER,  /* 弱电池(no charger) ==> power off. < 3.4   */
    CHARGE_BATTERY_LOWER_WITH_CHARGE, /* 弱电池(charger) ==> power off. < 3.4   */
    CHARGE_BATTERY_BAD,     /*坏电池: charger ==> Wait  show message, */
    BATTERY_BAD_WITHOUT_CHARGE,/* 坏电池时拔出charger */
    CHARGE_BATTERY_OVER_TEMP, /* 过温 wait resume */
    CHARGE_BATTERY_MAX
}CHG_CHARGE_BATT_CHECK_TYPE;
#endif
/* END: PN:dongle121,chendongguo, 2013/6/26 modified*/

/*函数列表*/
/***********************************************************************************
函 数 名  :chg_boot_is_batt_over_discharge
功能描述  :查询电池是否过放
输入参数  :none
输出参数  :无
返 回 值  : TRUE:过放，FALSE:非过放
注意事项  : 
***********************************************************************************/
boolean chg_boot_is_batt_over_discharge(void);
/***********************************************************************************
函 数 名  :chg_boot_batt_shorted_check
功能描述  :检测电池是否短路
输入参数  :无
输出参数  :无
返 回 值  : TRUE：电池短路
                  FALSE：电池未短路
注意事项  : 本函数依赖与ADC电池电压检测模块提供的电压读取接口和驱动适配层提供的充电芯片使能接口
***********************************************************************************/
boolean chg_boot_batt_shorted_check( void );

/***********************************************************************************
函 数 名  :chg_boot_batt_presence_check
功能描述  :检测电池是否在位
输入参数  :无
输出参数  :无
返 回 值  : TRUE：    电池在位
                  FALSE：电池不在位
注意事项  : 本函数依赖与ADC电池电压检测模块提供的电压读取接口和驱动适配层提供的充电芯片使能接口
***********************************************************************************/
boolean chg_boot_batt_presence_check( void );

/***********************************************************************************
函 数 名  :chg_boot_batt_unpresence_handle
功能描述  :检测到电池不在位时处理
输入参数  :无
输出参数  :无
返 回 值  : TRUE：    电池在位
                  FALSE：电池不在位
注意事项  : 本函数依赖与ADC电池电压检测模块提供的电压读取接口和驱动适配层提供的充电芯片使能接口
***********************************************************************************/
void chg_boot_batt_unpresence_handle( void );

/***********************************************************************************
函 数 名  :chg_boot_batt_over_temp_check
功能描述  :检测电池是否过温
输入参数  :无
输出参数  :无
返 回 值  : FALSE： 电池温度正常
                  TRUE：  电池过温
注意事项  : 本函数依赖与ADC电池电压检测模块提供的电压读取接口和驱动适配层提供的充电芯片使能接口
***********************************************************************************/
boolean chg_boot_batt_over_temp_check( void );

/***********************************************************************************
函 数 名  :chg_boot_low_power_trickle_charg_check
功能描述  :电池短路和过放检测
输入参数  :无
输出参数  :
返 回 值  : (rex)无/(其他)有电池状态
注意事项  : 本函数依赖与ADC电池电压检测模块提供的电压读取接口和驱动适配层提供的充电芯片使能接口
***********************************************************************************/
#if (MBB_CHG_PLATFORM_V7R2 == FEATURE_OFF)
/*9X25平台使用此函数接口*/
void chg_boot_low_power_trickle_charg_check(bl_shared_data_type *bl_shared_data);
#else
void chg_boot_low_power_trickle_charg_check( void );
#endif/*MBB_CHG_PLATFORM_V7R2 == NO*/
/**********************************************************************
函 数 名  : void load_factory_mode_flag_init(void)
功能描述  : 读取工厂模式标志NV参数
输入参数  : none
输出参数  : 无。
返 回 值  : 无。
注意事项  : 无。
***********************************************************************/
extern void load_ftm_mode_init(void);
/**********************************************************************
函 数 名      : chg_basic_init
功能描述  :  开机初始化:
                          充电芯片+双核通信
输入参数  : 无
输出参数  : 无
返 回 值      : 无
注意事项  : 在预充电部分调用
                         可根据具体平台添加删减
***********************************************************************/
extern void chg_basic_init( void );

/**********************************************************************
函 数 名      : void load_on_off_mode_parameter(void)
功能描述  :  读取硬件测试开机模式NV
输入参数  :none
输出参数  : 无。
返 回 值      : 无。
注意事项  : 无。
***********************************************************************/
extern void load_on_off_mode_parameter(void);

/**********************************************************************
函 数 名  : boolean chg_is_exception_poweroff_poweron_mode(void)
功能描述  :  判断目前是否为异常关机后,
                          下次插入电源后进入开机模式使能开机使能模式
输入参数  : 无。
输出参数  : 无。
返 回 值  : 是否为异常关机后，下次插入电源后进入开机模式使能
           TRUE : 是
           FALSE: 否
注意事项  : 无。
***********************************************************************/
extern boolean chg_is_exception_poweroff_poweron_mode(void);
/**********************************************************************
函 数 名      : load_batt_calib_para
功能描述  : 从NV(0xE50F)中获取电池校准参数
输入参数  : None
输出参数  : None
返 回 值     : None
注意事项  : None
***********************************************************************/
extern void load_batt_calib_para(void);
#endif /*CHG_BOOT_H*/
