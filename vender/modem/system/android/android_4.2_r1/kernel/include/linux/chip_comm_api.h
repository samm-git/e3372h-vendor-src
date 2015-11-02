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
#ifndef __CHIP_COMM_API_H__
#define __CHIP_COMM_API_H__

/*监听的设备*/
#define CHIP_COMM_BCM_DEV           "usbcomm0"
#define CHIP_COMM_ACORE_DEV         "usb3"

/**************************************************
                ENUM DEFINITION
**************************************************/
typedef enum
{
    CHIP_COMM_SUCC,
    CHIP_COMM_FAIL,
} CHIP_COMM_ERR_EN;

/*消息传送模式: 同步、异步*/
typedef enum
{
    CHIP_COMM_ASYNC,
    CHIP_COMM_SYNC,
} CHIP_COMM_MSG_MODE_EN;

/*消息流向: 从BCM发往A核、从A核发往BCM*/
typedef enum
{
    CHIP_COMM_BCM2ACORE,
    CHIP_COMM_ACORE2BCM,
} CHIP_COMM_MSG_FLOW_EN;


/* 注册的消息类型在此添加，请不要在此注册同一处理模块的多个id，
   此处仅分发到驱动各个模块，同一模块的不同消息id可以放到buffer中，
   在模块内部再解析*/
   
/*id规则: 0x@@##$$$$
          @@  表示去向: 01 表示从BCM发送到balong A核
                        10 表示从balong A核发送到BCM
          ##  表示相关领域
          $$$$表示消息索引*/
typedef enum
{
    /***********  本模块内部使用  **********/
    DECT_SERVER_STATUS          = 0x01000000,   /*用于通路测试，两端均可以发送*/
    
    /*************  voip相关  **************/
    EQUIP_TEST_SLIC             = 0x01010001,   /*SLIC产线测试*/


    DRV_RHPC_R2M_REQUEST        = 0x01020001,  /* Driver RHPC request Router->Modem*/
    DRV_RHPC_M2R_REQUEST        = 0x10020001,  /* Driver RHPC request Modem->Router*/

    /***********  BCM读取A核NV  ***********/
    
    
} CHIP_COMM_MSG_TYPE_EN;

/*message process call-back function*/
/*对于各模块的注册函数来说:
  void *req_buf -- 入参，接收的消息体
  int  req_len  -- 入参，接收的消息长度
  void *rsp_buf -- 出参，响应的消息体
  int  *rsp_len -- 出参，响应的消息长度*/
typedef int (*CHIP_COMM_MSG_PROC)(void *req_buf, int req_len, void *rsp_buf, int *rsp_len);


typedef struct
{
    /*消息流向*/
    CHIP_COMM_MSG_FLOW_EN msg_flow;

    /*消息类型*/
    CHIP_COMM_MSG_TYPE_EN msg_type;

    /*传输模式*/
    CHIP_COMM_MSG_MODE_EN msg_mode;
} CHIP_COMM_INFO_ST;

typedef struct
{
    CHIP_COMM_MSG_TYPE_EN msg_type;
    CHIP_COMM_MSG_PROC    msg_proc_func;
} CHIP_COMM_PROC_ST;


/**************************************************
                FUNCTION DEFINITION
**************************************************/

/******************************************************************************
  函数名称  : chip_comm_send_msg
  功能描述  : 发送消息到对端芯片
  输入参数  : CHIP_COMM_INFO_ST *msg_info 发送的消息的一些控制信息
              void *send_buf      发送的消息体，
              int send_len        发送的消息长度
              int recv_len        接收的响应的消息长度，如果是异步消息，填0

  调用函数  :
  被调函数  : 
  输出参数  : void *recv_buf      对端响应的消息体,如果是异步消息,填NULL
  返 回 值  : CHIP_COMM_SUCC, CHIP_COMM_FAIL
  
  修改历史  :

******************************************************************************/
extern int chip_comm_send_msg
(
    CHIP_COMM_INFO_ST *msg_info,
    void *send_buf, int send_len,
    void *recv_buf, int recv_len    
);

/******************************************************************************
  函数名称  : chip_comm_detect_peer
  功能描述  : 检验对端server是否OK，此接口最多阻塞1秒, 此函数可在系统启动时使用
  输入参数  : CHIP_COMM_MSG_FLOW_EN msg_flow  消息流向，告知检测那个server

  调用函数  :
  被调函数  : 
  输出参数  : 无
  返 回 值  : CHIP_COMM_SUCC, CHIP_COMM_FAIL
  
  修改历史  :

******************************************************************************/
extern int chip_comm_detect_peer(CHIP_COMM_MSG_FLOW_EN msg_flow);

/******************************************************************************
  函数名称  : chip_comm_server_stop
  功能描述  : 停止双机通信的server端, 注意: 此函数调用最大可能阻塞
              CHIP_COMM_SRV_BLOCK_TIMEOUT秒
  输入参数  : 无

  调用函数  :
  被调函数  : 
  输出参数  : 无
  返 回 值  : CHIP_COMM_SUCC, CHIP_COMM_FAIL
  
  修改历史  :

******************************************************************************/
extern int chip_comm_server_stop(void);

/******************************************************************************
  函数名称  : chip_comm_BCM_server_start
  功能描述  : BCM芯片调用此函数启动双机通信的router侧的server端
  输入参数  : 无

  调用函数  :
  被调函数  : 此函数在usb0注册完成时调用
  输出参数  : 无
  返 回 值  : CHIP_COMM_SUCC, CHIP_COMM_FAIL
  
  修改历史  :

******************************************************************************/
extern int chip_comm_BCM_server_start(void);

/******************************************************************************
  函数名称  : chip_comm_Acore_server_start
  功能描述  : Balong芯片A核调用此函数启动双机通信的modem侧的server端
  输入参数  : 无

  调用函数  :
  被调函数  : 此函数在usb0注册完成时调用
  输出参数  : 无
  返 回 值  : CHIP_COMM_SUCC, CHIP_COMM_FAIL
  
  修改历史  :

******************************************************************************/
extern int chip_comm_Acore_server_start(void);


#endif //end of __CHIP_COMM_API_H__
