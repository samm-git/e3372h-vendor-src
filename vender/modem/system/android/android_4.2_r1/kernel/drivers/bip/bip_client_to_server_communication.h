/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2013-2015. All rights reserved.
 *
 * mobile@huawei.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/semaphore.h>
#include <linux/slab.h>
#include <linux/miscdevice.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/kthread.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/device.h>
#include <linux/workqueue.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>
#include <linux/netlink.h>
#include <linux/irq.h>
#include <linux/gpio.h>
#include <linux/mutex.h>
#include <linux/netlink.h>

#include "drv_bip.h"

/*BIP Buffer 最大长度*/
#define BIP_MAX_BUFFER_LENGTH               1500

/*BIP APN 最小长度*/
#define BIP_MIN_NETWORK_ACCESS_NAME_LENGTH  1

/*BIP APN 最大长度*/
#define BIP_MAX_NETWORK_ACCESS_NAME_LENGTH  99

/*BIP IP地址 最大长度*/
#define BIP_MAX_IPADDRESS_LENGTH            16

/*BIP 拨号用户名 最大长度*/
#define BIP_MAX_DIAL_USERNAME_LEN           255

/*BIP 拨号密码 最大长度*/
#define BIP_MAX_DIAL_PASSWORD_LEN           255

/*BIP 协议定义 Channel Data最大长度*/
#define BIP_MAX_CHANNEL_DATA_LENGTH         255

/*定义BIP Client和BIP设备 之间 通信的设备ID*/
#define DEVICE_ID_BIP                       17

typedef unsigned char       SI_UINT8;
typedef unsigned long       SI_UINT32;
typedef signed long         SI_INT32;
typedef unsigned short      SI_UINT16;

typedef void BSP_VOID;
typedef void SI_VOID;


typedef enum
{
    BIP_NTO_CREATE_SOCKET =  1,
    BIP_TO_CREATE_SOCKET =  2,
    BIP_HAVE_CREATED_SOCKET = 3,
    BIP_CREATE_SOCKET_STATE_BUTT
} BIP_SOCKET_STATE_EN;


/*BIP Client和 BIP设备通信 消息枚举*/   
typedef enum
{ 
    UICC_INFORM = -1,/*通知BIP Client 取数据*/
    OTA_TO_UICC = 0, /*BIP Client 写入 BIP字符设备 数据*/
    UICC_TO_OTA = 1, /*BIP Client 读取 BIP字符设备 数据*/
    UICC_COMPLETE = 0xff /*BIP业务完成*/
}BIP_EVENT;


#define BIP_SOCKET_CREATE_NOK   0
#define BIP_SOCKET_CREATE_OK     1

/*PACKET DATA Structure to find BIP Data */ 
typedef struct
{  
    BSP_U32               BipServerAddr;
    BSP_U16               BipServerPort;
    BSP_U16               BipClientPort;
    BSP_U8                BipSocketStatus;
}BIP_PACKET_HEADER;
/**************************************************************************
  函数声明
**************************************************************************/

/*****************************************************************************
函 数 名  : BipDeviceOpen
功能描述  : 打开BIP字符设备
输入参数  : struct inode *inode, struct file *file
输出参数  : 无
调用函数  : 无
被调函数  : 外部接口
History     :
1.日    期  : 
  修改内容  : Create
*****************************************************************************/
static int BipDeviceOpen(struct inode *inode, struct file *file);

/*****************************************************************************
函 数 名  : BipDeviceRelease
功能描述  : 关闭BIP字符设备
输入参数  : struct inode *inode, struct file *file
输出参数  : 无
调用函数  : 无
被调函数  : 外部接口
History     :
1.日    期  : 
  修改内容  : Create
*****************************************************************************/
static int BipDeviceRelease(struct inode *inode, struct file *file);

/*****************************************************************************
函 数 名  : BipDeviceIoctl
功能描述  : BIP字符设备读写操作
输入参数  : struct inode *inode, struct file *file
输出参数  : 无
调用函数  : 无
被调函数  : 外部接口
History     :
1.日    期  : 
  修改内容  : Create
*****************************************************************************/
static long BipDeviceIoctl(struct file *file, unsigned int  cmd, unsigned long);


/*****************************************************************************
函 数 名  :BipDeviceInit
功能描述  : 注册BIP字符设备
输入参数  :无
输出参数  : 无
调用函数  : 无
被调函数  : 外部接口
History     :
1.日    期  : 
  修改内容  : Create
*****************************************************************************/
BSP_S32 BipDeviceInit(void);

/*****************************************************************************
函 数 名  :BipClientRegisterFuncInit
功能描述  : A核初始化:BIP 核间通信
输入参数  :无
输出参数  : 无
调用函数  : 无
被调函数  : 外部接口
History     :
1.日    期  : 
  修改内容  : Create
*****************************************************************************/
void __init BipClientRegisterFuncInit(void);

/*****************************************************************************
函 数 名  :BipDeviceReceiveDataFromBipClient
功能描述  : 从BIP Client接收到BIP消息，进行处理
输入参数  : BIP_Command_Event_STRU *stru
输出参数  : 无
返 回 值  : 无
被调函数  : 外部接口
History     :
1.日    期  : 
  修改内容  : Create
*****************************************************************************/
BSP_S32 BipDeviceReceiveDataFromBipClient( BIP_Command_Event_STRU *pDst);

/*****************************************************************************
函 数 名  :BipDeviceSendDataToModem
功能描述  : BIP字符设备收到Bip Cliennt的消息，通过核间通信，向STK模块发送数据
输入参数  : BIP_Command_Event_STRU *stru
输出参数  : 无
返 回 值  : BSP_OK:    操作成功
             BSP_ERROR: 操作失败
调用函数  : 无
被调函数  : 外部接口
History     :
1.日    期  : 
  修改内容  : Create
 *****************************************************************************/
BSP_S32 BipDeviceSendDataToModem( BIP_Command_Event_STRU *stru);

/*****************************************************************************
函 数 名  : BipDeviceReceiveDataFromModem
功能描述  : 接收来自M核的 BIP消息,并通知Bip Client Fetch数据
功能描述  : 打印M核BIP模块，传输过来的数据
输入参数  : BSP_VOID *pMsgBody，接收的消息实体
             BSP_U32 u32Len,消息长度
输出参数  : 无
返 回 值  : BSP_OK:    操作成功
             BSP_ERROR: 操作失败
调用函数  : 无
被调函数  : 外部接口
History     :
1.日    期  : 
  修改内容  : Create
*****************************************************************************/
BSP_S32 BipDeviceReceiveDataFromModem(SI_VOID *pMsgBody, BSP_U32 u32Len);

/*****************************************************************************
函 数 名  :BipDeviceNotifyDataToBipClient
功能描述  : 给应用Bip Client发送消息，通知有数据到来，来取数据
输入参数  : BIP_Command_Event_STRU *stru
输出参数  : 无
返 回 值  : 无
调用函数  : 无
被调函数  : 外部接口
History     :
1.日    期  : 
  修改内容  : Create
 *****************************************************************************/
BSP_S32 BipDeviceNotifyDataToBipClient( BIP_Command_Event_STRU *pDst, BIP_EVENT event_code);

/*****************************************************************************
函 数 名  :BipDeviceShowInfo
功能描述  : 打印M核STK模块，传输过来的数据
输入参数  : BIP_Command_Event_STRU *stru
输出参数  : 无
返 回 值  :无
调用函数  : 无
被调函数  : 外部接口
History     :
1.日    期  : 
  修改内容  : Create
*****************************************************************************/
void BipDeviceShowInfo (BIP_Command_Event_STRU *Data);

#if (FEATURE_ON == MBB_FEATURE_BIP_TEST)

/*****************************************************************************
函 数 名  :BipClientTestOpenChannel
功能描述  : 模拟Bip Client 回复Open Channel回应
输入参数  :  无
输出参数  : 无
返 回 值  :无
调用函数  : 无
被调函数  : 外部接口
History     :
1.日    期  : 
  修改内容  : Create
*****************************************************************************/
void BipClientTestOpenChannel(BSP_U8 ResultValue);

/*****************************************************************************
函 数 名  :BipClientTestCloseChannel
功能描述  : 模拟Bip Client 回复Close Channel回应
输入参数  :  无
输出参数  : 无
返 回 值  :无
调用函数  : 无
被调函数  : 外部接口
History     :
1.日    期  : 
  修改内容  : Create
 *****************************************************************************/
void BipClientTestCloseChannel(void);

/*****************************************************************************
函 数 名  :BipClientTestSendData
功能描述  : 模拟Bip Client 回复Send Datal回应
输入参数  :  无
输出参数  : 无
返 回 值  :无
调用函数  : 无
被调函数  : 外部接口
History     :
1.日    期  : 
  修改内容  : Create
 *****************************************************************************/
void BipClientTestSendData(BSP_U8 dataLen);

/*****************************************************************************
函 数 名  :BipClientTestSendData
功能描述  : 模拟Bip Client 回复Receive Datal回应
输入参数  :  无
输出参数  : 无
返 回 值  :无
调用函数  : 无
被调函数  : 外部接口
History     :
1.日    期  : 
  修改内容  : Create
 *****************************************************************************/
void BipClientTestReceiveData( BSP_U8 dataLen, BSP_U8 LeftDataLen );

/*****************************************************************************
函 数 名  :BipClientTestSendData
功能描述  : 模拟Bip Client 回复Get Channel Statusl回应
输入参数  :  无
输出参数  : 无
返 回 值  :无
调用函数  : 无
被调函数  : 外部接口
History     :
1.日    期  : 
  修改内容  : Create
 *****************************************************************************/
void BipClientTestGetChannelStatus(BSP_U8 ChannelStatus);

/*****************************************************************************
函 数 名  :BipClientTestSendData
功能描述  : 模拟Bip Client 回复Get Channel Statusl回应
输入参数  :  无
输出参数  : 无
返 回 值  :无
调用函数  : 无
被调函数  : 外部接口
History     :
1.日    期  : 
  修改内容  : Create
 *****************************************************************************/
void BipClientTestGetChannelStatusFail(void);

/*****************************************************************************
函 数 名  :BipClientTestSendData
功能描述  : 模拟Bip Client 下发Channel Statusl Event
输入参数  :  无
输出参数  : 无
返 回 值  :无
调用函数  : 无
被调函数  : 外部接口
History     :
1.日    期  : 
  修改内容  : Create
 *****************************************************************************/
void BipClientTestChannelStatusEvent(BSP_U8 ChannelStatus);
#endif /*(FEATURE_ON == MBB_FEATURE_BIP_TEST)*/


