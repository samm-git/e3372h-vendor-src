/*************************************************************************
*   版权所有(C) 1987-2011, 深圳华为技术有限公司.
*
*   文 件 名 :  drv_clk.h
*
*   作    者 :  xujingcui
*
*   描    述 :  本文件命名为"drv_clk", 给出V7R2底软和协议栈之间的CLK_API接口
*
*   修改记录 :  2013年1月18日  v1.00 xujingcui创建
*************************************************************************/
#ifndef __DRV_TCXO_H__
#define __DRV_TCXO_H__

#include <drv_dpm.h>
typedef BSP_S32 (*PWC_TCXO_FUNCPTR)(int);

/***************************************************************************
 函 数 名  : BSP_TCXO_RfclkStatus
 功能描述  : 获取RF CLK状态
 输入参数  :
		enModemId  Modem ID
 输出参数  : 无
 返 回 值  : PWRCTRL_COMM_ON         使能;
             PWRCTRL_COMM_OFF        禁止;
             RET_ERR                 参数异常
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年6月24日
    作    者   : 周珊 212992
    修改内容   : 新生成函数
**************************************************************************/
 int BSP_TCXO_RfclkStatus(PWC_COMM_MODEM_E enModemId);
#define DRV_TCXO_RFCLKSTATUS(enModemId) BSP_TCXO_RfclkStatus(enModemId)
/*****************************************************************************
 函 数 名  : DRV_TCXO_RfclkEnable
 功能描述  : 通信模块控制RF 19.2M时钟使能
 输入参数  : 无
 输出参数  : 无
 返 回 值  : RET_OK         成功
             RET_ERR        失败
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年6月24日
    作    者   : 周珊 212992
    修改内容   : 新生成函数

*****************************************************************************/
 int BSP_TCXO_RfclkEnable(PWC_COMM_MODEM_E  enModemId, PWC_COMM_MODE_E enModeType);
#define DRV_TCXO_RFCLKENABLE(enModemId, enModeType) BSP_TCXO_RfclkEnable(enModemId, enModeType)
/*****************************************************************************
 函 数 名  : BSP_TCXO_RfclkDisable
 功能描述  : 通信模块控制RF 19.2M时钟去使能
 输入参数  : 无
 输出参数  : 无
 返 回 值  : RET_OK         成功
             RET_ERR        失败
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年6月24日
    作    者   : 周珊 212992
    修改内容   : 新生成函数

*****************************************************************************/
 int BSP_TCXO_RfclkDisable(PWC_COMM_MODEM_E  enModemId, PWC_COMM_MODE_E enModeType);
#define DRV_TCXO_RFCLKDISABLE(enModemId, enModeType) BSP_TCXO_RfclkDisable(enModemId, enModeType)
/***************************************************************************
 函 数 名  : DRV_TCXO_GETSTATUS
 功能描述  : 获取TCXO状态
 输入参数  :
		enModemId  Modem ID
 输出参数  : 无
 返 回 值  : PWRCTRL_COMM_ON         使能;
             PWRCTRL_COMM_OFF        禁止;
             RET_ERR                 参数异常
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年4月19日
    作    者   : 王振伟 00176398
    修改内容   : 新生成函数
**************************************************************************/
 int bsp_tcxo_getstatus(PWC_COMM_MODEM_E enModemId);
#define DRV_TCXO_GETSTATUS(enModemId)   bsp_tcxo_getstatus(enModemId)
/***************************************************************************
 函 数 名  : DRV_TCXO_ENABLE
 功能描述  : 使能TCXO
 输入参数  : enModemId  Modem ID
	         enModeType    Mode type
 输出参数  : 无
 返 回 值  : RET_OK         成功
             RET_ERR        失败
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年4月19日
    作    者   : 王振伟 00176398
    修改内容   : 新生成函数
**************************************************************************/
 int bsp_tcxo_enable(PWC_COMM_MODEM_E  enModemId, PWC_COMM_MODE_E enModeType);
#define DRV_TCXO_ENABLE(enModemId, enModeType)  bsp_tcxo_enable(enModemId, enModeType)
/***************************************************************************
 函 数 名  : DRV_TCXO_DISABLE
 功能描述  : 关闭TCXO
 输入参数  : enModemId  Modem ID
	         enModeType    Mode type
 输出参数  : 无
 返 回 值  : RET_OK         成功
             RET_ERR        失败
	         RET_ERR_NOT_FOUND 没有投票使能
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年4月19日
    作    者   : 王振伟 00176398
    修改内容   : 新生成函数
**************************************************************************/
 int bsp_tcxo_disable(PWC_COMM_MODEM_E  enModemId, PWC_COMM_MODE_E enModeType);
#define DRV_TCXO_DISABLE(enModemId, enModeType) bsp_tcxo_disable(enModemId, enModeType)
/***************************************************************************
 函 数 名  : DRV_TCXO_TIMER_START
 功能描述  : TCXO 定时器启动
 输入参数  : enModemId  Modem ID
    	     enModeType    Mode type
    	     routine:回调函数  注:回调函数在中断上下文中执行
             arg:回调参数
             timerValue:计数值 单位ms
 输出参数  : 无
 返 回 值  : RET_OK         成功
             RET_ERR        失败
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年4月19日
    作    者   : 王振伟 00176398
    修改内容   : 新生成函数
**************************************************************************/
 int bsp_tcxo_timer_start(PWC_COMM_MODEM_E  enModemId, PWC_COMM_MODE_E enModeType, PWC_TCXO_FUNCPTR routine, int arg, unsigned int timerValue);
#define DRV_TCXO_TIMER_START(enModemId, enModeType, routine, arg, timerValue)   bsp_tcxo_timer_start(enModemId, enModeType, routine, arg, timerValue)





#endif
