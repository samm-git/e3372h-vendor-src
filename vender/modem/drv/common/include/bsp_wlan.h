/*
 *  Copyright (C), 2011-2013, Huawei Technologies Co., Ltd.
 *
 * File name     : bsp_wlan.h
 * Created       : 2014-07-10
 * Last Modified : wlan对外的接口
 * Description   :  vic
 * Modification  : Create file
 *
 */


#ifndef _BSP_WLAN_H_
#define _BSP_WLAN_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 函数名称  : void wlan_set_led_stus(int flag)
 功能描述  : 获取log打印配置
 输入参数  : NA
 输出参数  : NA
 返 回 值  : BOOT
*****************************************************************************/
void wlan_set_led_flag(int flag);

#ifdef __cplusplus
}
#endif

#endif /*  _BSP_WLAN_H_*/

