/*************************************************************************
*   版权所有(C) 1987-2011, 深圳华为技术有限公司.
*
*   文 件 名 :  DrvInterface.h
*
*   作    者 :  yangzhi
*
*   描    述 :  本文件命名为"DrvInterface.h", 给出V7R1底软和协议栈之间的API接口统计
*
*   修改记录 :  2011年1月18日  v1.00  yangzhi创建
*************************************************************************/

#ifndef __DRV_CHG_H__
#define __DRV_CHG_H__

#include "drv_comm.h"
#include "product_config.h"

/*************************CHG模块START********************************/

/*****************************************************************************
 函 数 名  : BSP_CHG_GetCbcState
 功能描述  : 返回电池状态和电量
 输入参数  :pusBcs 0:电池在供电 1:与电池连接电池未供电 2:没有与电池连接
                          pucBcl  0:电量不足或没有与电池连接
 输出参数  : pusBcs 0:电池在供电 1:与电池连接电池未供电 2:没有与电池连接
                          pucBcl  0:电量不足或没有与电池连接
 返回值：   0 操作成功
                         -1操作失败

*****************************************************************************/
extern int BSP_CHG_GetCbcState(unsigned char *pusBcs,unsigned char *pucBcl);
extern int DRV_CHG_GET_CBC_STATE(unsigned char *pusBcs,unsigned char *pucBcl);

typedef enum CHARGING_STATE_ENUM_tag
{
        CHARGING_INIT = -1,
        NO_CHARGING_UP = 0,  /*开机未充电*/
        CHARGING_UP ,              /*开机正充电*/
        NO_CHARGING_DOWN ,  /*关机未充电*/
        CHARGING_DOWN         /*关机未充电*/
}CHARGING_STATE_ENUM;

typedef enum BATT_LEVEL_ENUM_tag
{
        BATT_INIT = -2,
        BATT_LOW_POWER =-1,    /*电池低电*/
        BATT_LEVEL_0,                 /*0格电池电量*/
        BATT_LEVEL_1,                  /*1格电池电量*/
        BATT_LEVEL_2,                   /*2格电池电量*/
        BATT_LEVEL_3,                    /*3格电池电量*/
        BATT_LEVEL_4,                    /*4格电池电量*/
        BATT_LEVEL_MAX
}BATT_LEVEL_ENUM;

/*电池高温上报事件*/
#if (MBB_CHARGE == FEATURE_ON)
typedef enum _TEMP_EVENT
{
   TEMP_BATT_LOW,    /* 电池低温事件 */
   TEMP_BATT_HIGH,   /* 电池高温事件 */
   TEMP_BATT_NORMAL, /* 电池高温解除事件 */
   TEMP_BATT_MAX     /* 如果事件值不小于此值，则为非法事件*/
}TEMP_EVENT;
#endif
typedef struct BATT_STATE_tag
{
    CHARGING_STATE_ENUM  charging_state;
    BATT_LEVEL_ENUM           battery_level;
#if (MBB_CHARGE == FEATURE_ON)
    TEMP_EVENT      batt_temp_state;
#endif
}BATT_STATE_T;
/**********Charge Module Return Code Macros**********/
#define CHG_OK                 0
#define CHG_ERROR              (-1)
#if (MBB_CHARGE == FEATURE_OFF)
/*AT 命令枚举类型*/
typedef enum
{
    CHG_AT_BATTERY_CHECK,        /* Battery校准*/
    CHG_AT_BATTERY_LEVEL,        /* Battery电量 */
    CHG_AT_BATTERY_ADC,          /* Battery Temperature保护*/
    CHG_AT_BATTERY_CAPACITY,     /* Battery Capacity*/
    CHG_AT_BATTERY_INVALID
} ENUM_CHG_ATCM_TYPE;
#endif
typedef enum ENUM_POWER_ON_MODE_tag
{
    POWER_ON_INVALID = 0,          	/* 无效开机模式*/
    POWER_ON_NORMAL,       	 	/* 正常开机模式*/
    POWER_ON_CHARGING,       	/* 关机充电模式*/
    POWER_ON_MAX
} ENUM_POWER_ON_MODE;
/**********************************************************************
函 数 名  :chg_get_batt_level
功能描述  : 获取当前电池电量格数的接口函数
输入参数  : 无。
输出参数  : 无。
返 回 值  : 电池电量格数
注意事项  : 无。
***********************************************************************/
#if (MBB_CHARGE == FEATURE_ON)
extern BATT_LEVEL_ENUM chg_get_batt_level(void);
unsigned char chg_is_powdown_charging (void);
#endif


#define  NV_BATT_VOLT_CALI_I           (58639)
#define  TBAT_CHECK_INVALID            (0xFFFF)
#if (MBB_CHARGE == FEATURE_ON)
/*电池校准参数数据结构*/
typedef struct
{
    unsigned short min_value;
    unsigned short max_value;
}VBAT_CALIBRATION_TYPE;
/*AT命令读取电池电压枚举类型*/
typedef enum
{
    CHG_AT_BATTERY_LEVEL,        /* Battery Voltage after Calibration*/
    CHG_AT_BATTERY_ADC,          /* Battery Voltage before Calibration*/
    CHG_AT_BATTERY_CAPACITY,     /* Battery Capacity*/
    CHG_AT_BATTERY_CHECK,        /* Battery Calibration*/
    CHG_AT_BATTERY_INVALID
} ENUM_CHG_ATCM_TYPE;
typedef enum
{
    CHG_AT_CHARGE_DISALBE,        /* disable charge*/
    CHG_AT_CHARGE_ENABLE,         /* enable charge*/
    CHG_AT_DISCHARGE_DISALBE,     /* disable discharge*/
    CHG_AT_DISCHARGE_ENABLE,      /* enable discharge*/
    CHG_AT_START_CHARGE_SPLY,     /* start charge supply*/
    CHG_AT_CHARGE_MODE_INVALID
} ENUM_CHG_MODE_TYPE;
typedef enum 
{
    NO_BATT_MODE = 0,          /*无电池*/
    KNOCK_DOWN_BATT_MODE = 1,  /*可拆卸电池*/
    FIX_BATT_MODE = 2,         /*一体化电池*/  
    INVALID_BATT_MODE  
    }BATT_FIX_MODE;
/*查询充电状态和模式*/
typedef struct
{
    int chg_state;
    int chg_mode;
}CHG_TCHRENABLE_TYPE;
/*****************************************************************************
 函 数 名  : app_get_battery_state
 功能描述  : Oled调用此函数获取充电状态以及电池电量，
 输入参数  : 无
 输出参数  : BATT_STATE_T *battery_state，输出电池格数和充电状态
 返 回 值  : 0 成功, -1 失败
 调用函数  :
 被调函数  :
 注      意: 使用power supply机制时此函数不在对应用开放,非LINUX系统需要打开此函数，同时在定义的地方
             去掉static声明；
 修改历史      :
  1.日    期   : 2012年10月30日
    作    者   : chendongguo c00182736
    修改内容   : 新生成函数

*****************************************************************************/
int app_get_battery_state(BATT_STATE_T *battery_state);
/**********************************************************************
函 数 名  : chg_tbat_read
功能描述  : AT读接口
输入参数  : 操作方式:读(校准前/后)电池电量还是读电压校准值
输出参数  : 电池电量/电池电压校准值
返 回 值  : 是否操作成功
注意事项  : 
***********************************************************************/
int chg_tbat_read(ENUM_CHG_ATCM_TYPE atID, void *pItem);
/**********************************************************************
函 数 名  : chg_tbat_write
功能描述  : AT写接口:设置电池电量校准值
输入参数  : 4.2V和3.4V对应的ADC采样值
输出参数  : 无
返 回 值  : 是否操作成功
注意事项  : 无
***********************************************************************/
int chg_tbat_write(ENUM_CHG_ATCM_TYPE atID, void *pItem);
/**********************************************************************
函 数 名  : chg_tbat_charge_mode_set
功能描述  : 设置电池充电或者放电
输入参数  : 命令参数
输出参数  : none
返 回 值  : CHG_OK:success,CHG_ERROR:fail
注意事项  : 
***********************************************************************/
int chg_tbat_charge_mode_set(ENUM_CHG_MODE_TYPE cmd, unsigned int arg);
/**********************************************************************
函 数 名  :  chg_tbat_status_get
功能描述  :  TBAT AT^TCHRENABLE?是否需要补电
输入参数  : 无
输出参数  : 无
返 回 值      : 1:需要补电
			    0:不需要补电
注意事项  : 无
***********************************************************************/
int chg_tbat_is_batt_status_ok(void);
/**********************************************************************
函 数 名  : chg_tbat_get_charging_status
功能描述  : 查询当前充电状态
输入参数  : none
输出参数  : none
返 回 值  : 1:charging,0:no
注意事项  : 
***********************************************************************/
int chg_tbat_get_charging_status(void);
/**********************************************************************
函 数 名  : chg_tbat_get_charging_status
功能描述  : 查询当前充电模式
输入参数  : none
输出参数  : none
返 回 值  : 0:非充电；1：涓充；2：快充
注意事项  : 
***********************************************************************/
int chg_tbat_get_charging_mode(void);
/**********************************************************************
函 数 名  : chg_tbat_get_discharging_status
功能描述  : 查询当前是否是suspend模式
输入参数  : none
输出参数  : none
返 回 值  : 1:yes,0:no
注意事项  : 
***********************************************************************/
int chg_tbat_get_discharging_status(void);
/**********************************************************************
函 数 名  : chg_tbat_sfeature_inquiry_extchg
功能描述  : 查询是否支持对外充电
输入参数  : none
输出参数  : none
返 回 值  : 1:yes,0:no
注意事项  : 
***********************************************************************/
int chg_tbat_sfeature_inquiry_extchg(void);
/**********************************************************************
函 数 名  : chg_tbat_sfeature_inquiry_wireless
功能描述  : 查询是否支持无线充电
输入参数  : none
输出参数  : none
返 回 值  : 1:yes,0:no
注意事项  : 
***********************************************************************/
int chg_tbat_sfeature_inquiry_wireless(void);
/**********************************************************************
函 数 名  : chg_tbat_sfeature_inquiry_battery
功能描述  : 查询电池电压典型值
输入参数  : none
输出参数  : none
返 回 值  : 
注意事项  : 
***********************************************************************/
int chg_tbat_sfeature_inquiry_battery(void);
/**********************************************************************
函 数 名  : chg_MNTN_get_batt_state
功能描述  : AT读接口
输入参数  : 查询是否是可拆卸电池
输出参数  : none
返 回 值  : 1:可拆卸，2：不可拆卸
注意事项  : 
***********************************************************************/
int chg_MNTN_get_batt_state(void);
/**********************************************************************
函 数 名  : chg_wireless_mmi_test
功能描述  : AT读接口，无线充电电路检测
输入参数  : none
输出参数  : none
返 回 值  : 
注意事项  : 
***********************************************************************/
int chg_wireless_mmi_test(void);
/**********************************************************************
函 数 名  : chg_extchg_mmi_test
功能描述  : AT读接口，对外充电电路检测
输入参数  : none
输出参数  : none
返 回 值  : 
注意事项  : 
***********************************************************************/
int chg_extchg_mmi_test(void);
/**********************************************************************
函 数 名  : chg_extchg_mmi_test
功能描述  : AT读接口，获取AT^TCHRENABLE设置的结果
输入参数  : none
输出参数  : none
返 回 值  : 
注意事项  : 
***********************************************************************/
int chg_tbat_get_tchrenable_status(CHG_TCHRENABLE_TYPE *tchrenable_state);
/************************************************************************  
函 数 名  : chg_getCbcState
功能描述  : 在线升级AT^CBC
         pucBcs
            0: 电池在供电
            1: 与电池连接电池未供电
            2: 没有与电池连接
         pucBcl
            0:电量不足或没有与电池连接
             100: 剩余电量百分比
输入参数  : none
输出参数  : pucBcs，pucBcl
返 回 值  : 
注意事项  : 0 Success, -1 failed  
 ***********************************************************************/
int chg_getCbcState(unsigned char  *pucBcs ,unsigned char   *pucBcl);
#else

typedef enum
{
    CHG_AT_CHARGE_MODE_INVALID
} ENUM_CHG_MODE_TYPE;

/*查询充电状态和模式*/
typedef struct
{
    int chg_state;
    int chg_mode;
}CHG_TCHRENABLE_TYPE;
/*****************************************************************************
 函 数 名  : app_get_battery_state
 功能描述  : Oled调用此函数获取充电状态以及电池电量，
 输入参数  : 无
 输出参数  : BATT_STATE_T *battery_state，输出电池格数和充电状态
 返 回 值  : 0 成功, -1 失败
 调用函数  :
 被调函数  :
 注      意: 使用power supply机制时此函数不在对应用开放,非LINUX系统需要打开此函数，同时在定义的地方
             去掉static声明；
 修改历史      :
  1.日    期   : 2012年10月30日
    作    者   : chendongguo c00182736
    修改内容   : 新生成函数

*****************************************************************************/
static INLINE int app_get_battery_state(BATT_STATE_T *battery_state)
{
    return 0;
}
/**********************************************************************
函 数 名  : chg_tbat_read
功能描述  : AT读接口
输入参数  : 操作方式:读(校准前/后)电池电量还是读电压校准值
输出参数  : 电池电量/电池电压校准值
返 回 值  : 是否操作成功
注意事项  : 
***********************************************************************/
static INLINE int chg_tbat_read(ENUM_CHG_ATCM_TYPE atID, void *pItem)
{
    return 0;
}
/**********************************************************************
函 数 名  : chg_tbat_write
功能描述  : AT写接口:设置电池电量校准值
输入参数  : 4.2V和3.4V对应的ADC采样值
输出参数  : 无
返 回 值  : 是否操作成功
注意事项  : 无
***********************************************************************/
static INLINE int chg_tbat_write(ENUM_CHG_ATCM_TYPE atID, void *pItem)
{
    return 0;
}
/**********************************************************************
函 数 名  : chg_tbat_charge_mode_set
功能描述  : 设置电池充电或者放电
输入参数  : 命令参数
输出参数  : none
返 回 值  : CHG_OK:success,CHG_ERROR:fail
注意事项  : 
***********************************************************************/
static INLINE int chg_tbat_charge_mode_set(ENUM_CHG_MODE_TYPE cmd, unsigned int arg)
{
    return 0;
}
/**********************************************************************
函 数 名  :  chg_tbat_status_get
功能描述  :  TBAT AT^TCHRENABLE?是否需要补电
输入参数  : 无
输出参数  : 无
返 回 值      : 1:需要补电
			    0:不需要补电
注意事项  : 无
***********************************************************************/
static INLINE int chg_tbat_is_batt_status_ok(void)
{
    return 0;
}
/**********************************************************************
函 数 名  : chg_tbat_get_charging_status
功能描述  : 查询当前充电状态
输入参数  : none
输出参数  : none
返 回 值  : 1:charging,0:no
注意事项  : 
***********************************************************************/
static INLINE int chg_tbat_get_charging_status(void)
{
    return 0;
}
/**********************************************************************
函 数 名  : chg_tbat_get_charging_status
功能描述  : 查询当前充电模式
输入参数  : none
输出参数  : none
返 回 值  : 0:非充电；1：涓充；2：快充
注意事项  : 
***********************************************************************/
static INLINE int chg_tbat_get_charging_mode(void)
{
    return 0;
}
/**********************************************************************
函 数 名  : chg_tbat_get_discharging_status
功能描述  : 查询当前是否是suspend模式
输入参数  : none
输出参数  : none
返 回 值  : 1:yes,0:no
注意事项  : 
***********************************************************************/
static INLINE int chg_tbat_get_discharging_status(void)
{
    return 0;
}
/**********************************************************************
函 数 名  : chg_tbat_sfeature_inquiry_extchg
功能描述  : 查询是否支持对外充电
输入参数  : none
输出参数  : none
返 回 值  : 1:yes,0:no
注意事项  : 
***********************************************************************/
static INLINE int chg_tbat_sfeature_inquiry_extchg(void)
{
    return 0;
}
/**********************************************************************
函 数 名  : chg_tbat_sfeature_inquiry_wireless
功能描述  : 查询是否支持无线充电
输入参数  : none
输出参数  : none
返 回 值  : 1:yes,0:no
注意事项  : 
***********************************************************************/
static INLINE int chg_tbat_sfeature_inquiry_wireless(void)
{
    return 0;
}
/**********************************************************************
函 数 名  : chg_tbat_sfeature_inquiry_battery
功能描述  : 查询电池电压典型值
输入参数  : none
输出参数  : none
返 回 值  : 
注意事项  : 
***********************************************************************/
static INLINE int chg_tbat_sfeature_inquiry_battery(void)
{
    return 0;
}
/**********************************************************************
函 数 名  : chg_MNTN_get_batt_state
功能描述  : AT读接口
输入参数  : 查询是否是可拆卸电池
输出参数  : none
返 回 值  : 1:可拆卸，2：不可拆卸
注意事项  : 
***********************************************************************/
static INLINE int chg_MNTN_get_batt_state(void)
{
    return 0;
}
/**********************************************************************
函 数 名  : chg_wireless_mmi_test
功能描述  : AT读接口，无线充电电路检测
输入参数  : none
输出参数  : none
返 回 值  : 
注意事项  : 
***********************************************************************/
static INLINE int chg_wireless_mmi_test(void)
{
    return 0;
}
/**********************************************************************
函 数 名  : chg_extchg_mmi_test
功能描述  : AT读接口，对外充电电路检测
输入参数  : none
输出参数  : none
返 回 值  : 
注意事项  : 
***********************************************************************/
static INLINE int chg_extchg_mmi_test(void)
{
    return 0;
}
/**********************************************************************
函 数 名  : chg_extchg_mmi_test
功能描述  : AT读接口，获取AT^TCHRENABLE设置的结果
输入参数  : none
输出参数  : none
返 回 值  : 
注意事项  : 
***********************************************************************/
static INLINE int chg_tbat_get_tchrenable_status(CHG_TCHRENABLE_TYPE *tchrenable_state)
{
    return 0;
}
/************************************************************************  
函 数 名  : chg_getCbcState
功能描述  : 在线升级AT^CBC
         pucBcs
            0: 电池在供电
            1: 与电池连接电池未供电
            2: 没有与电池连接
         pucBcl
            0:电量不足或没有与电池连接
             100: 剩余电量百分比
输入参数  : none
输出参数  : pucBcs，pucBcl
返 回 值  : 
注意事项  : 0 Success, -1 failed  
 ***********************************************************************/
static INLINE int chg_getCbcState(unsigned char  *pucBcs ,unsigned char   *pucBcl)
{
    return 0;
}
#endif
/*****************************************************************************
 函 数 名  : BSP_CHG_GetBatteryState
 功能描述  :获取底层电池状态信息
 输入参数  :battery_state 电量信息
 输出参数  :battery_state 电量信息
 返回值：   0 操作成功
                         -1操作失败

*****************************************************************************/
extern int BSP_CHG_GetBatteryState(BATT_STATE_T *battery_state);
extern int DRV_CHG_GET_BATTERY_STATE(BATT_STATE_T *battery_state);

/*****************************************************************************
 函 数 名  : BSP_CHG_ChargingStatus
 功能描述  :查询目前是否正在充电中
 输入参数  :无
 输出参数  :无
 返回值：   0 未充电
                          1 充电中
*****************************************************************************/
extern int BSP_CHG_ChargingStatus(void);
extern int DRV_CHG_GET_CHARGING_STATUS(void);

/*****************************************************************************
 函 数 名  : BSP_CHG_StateSet
 功能描述  :使能或者禁止充电
 输入参数  :ulState      0:禁止充电
 						1:使能充电
 输出参数  :无
 返回值：    无
*****************************************************************************/
extern void DRV_CHG_STATE_SET(unsigned long ulState);

/*****************************************************************************
 函 数 名  : BSP_CHG_StateGet
 功能描述  :查询
 输入参数  :
 输出参数  :无
 返回值：    无
*****************************************************************************/
extern BSP_S32 BSP_CHG_StateGet(void);
#define DRV_CHG_STATE_GET()    BSP_CHG_StateGet()

/*****************************************************************************
 函 数 名  : BSP_CHG_Sply
 功能描述  :查询
 输入参数  :
 输出参数  :无
 返回值：    无
*****************************************************************************/
extern BSP_S32 BSP_CHG_Sply(void);
#define DRV_CHG_BATT_SPLY()    BSP_CHG_Sply()


/*****************************************************************************
* 函 数 名  : BSP_TBAT_Read
* 功能描述  :
* 输入参数  :
* 输出参数  :
* 返 回 值  :
* 修改记录  :
*****************************************************************************/
extern BSP_S32 BSP_TBAT_Read(ENUM_CHG_ATCM_TYPE atID, void *pItem);

/*****************************************************************************
* 函 数 名  : BSP_TBAT_Write
* 功能描述  :
* 输入参数  :
* 输出参数  :
* 返 回 值  :
*****************************************************************************/
extern BSP_S32 BSP_TBAT_Write(ENUM_CHG_ATCM_TYPE atID, void *pItem);

/**********************************************************************
函 数 名      : BSP_TBAT_CHRStGet
功能描述  :  TBAT AT^TCHRENABLE?是否需要补电

输入参数  : 无
输出参数  : 无
返 回 值      : 1:需要补电
			      0:不需要补电
注意事项  : 无
***********************************************************************/
extern BSP_S32 BSP_TBAT_CHRStGet(void);

/**********************************************************************
函 数 名      : BSP_TBAT_CHRSuply
功能描述  :  TBAT AT^TCHRENABLE=4补电实现
            			需要提供补电成功LCD显示图样
输入参数  : 无
输出参数  : 无
返 回 值      :
注意事项  : 无
***********************************************************************/
extern BSP_S32 BSP_TBAT_CHRSply(void);

/**********************************************************************
函 数 名      : BSP_ONOFF_StartupModeGet
功能描述  :  A核判断开关机模式

输入参数  : 无
输出参数  : 无
返 回 值      :
注意事项  : 无
***********************************************************************/
ENUM_POWER_ON_MODE BSP_ONOFF_StartupModeGet( void );
/*****************************************************************************
* 函 数 名  : BSP_PMU_BattCali
*
* 功能描述  : 电池校准前减小单板电流接口，给AT调用
*
* 输入参数  :
* 输出参数  :
*
* 返 回 值  :
*
* 其它说明  :
*
*****************************************************************************/
extern void BSP_PMU_BattCali(void);


/*****************************************************************************
* 函 数 名  : BSP_MNTN_GetBattState
*
* 功能描述  : 获取电池在位状态
*
* 输入参数  : 无
* 输出参数  : 无
*
* 返 回 值  : 成功：0，1，2
              失败：负数
*
*
*****************************************************************************/
static INLINE BSP_S32 BSP_MNTN_GetBattState(void)
{
	return 0;
}
#define DRV_MNTN_GET_BATT_STATE() BSP_MNTN_GetBattState()


/*************************CHG模块 END*********************************/

#endif

