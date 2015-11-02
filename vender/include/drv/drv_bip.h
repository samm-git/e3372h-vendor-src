/*************************************************************************
*   版权所有(C) 1987-2009, 深圳华为技术有限公司.
*
*   文 件 名 :  bsp_bip.h
*
*
*   描    述 :  本文件命名为"bsp_bip.h", 实现BIP驱动MODEM侧与APP侧BIP驱动通信的功能
*
*   修改记录 :
*************************************************************************/

#ifndef _BSP_BIP_H_
#define _BSP_BIP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "DrvInterface.h"

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

/*BIP UE和SIM卡之间 传输Channel Data最大长度*/
#define BIP_MAX_CHANNEL_DATA_SIZE           237

/*BIP 支持的最大Channel个数,为了表示数组的下标，Channel个数加1*/
#define BIP_CHANNEL_MAX_NUM 2


enum 
{
    BIP_ERROR = -1,   /* Function Call was failed. */
    BIP_SUCCESS = 0,    /* Function Call was successful. */
};

/*Channel对应的Socket状态枚举*/
typedef enum 
{
    CHANNEL_CLOSE = 0,
    CHANNEL_OPEN = 1,
    CHANNEL_OPENING,
}CHANNEL_STATUS; 

/*BIP 字符设备和 BIP Client之间，事件枚举*/
typedef enum 
{
    BIP_OPEN_CHANNEL_REQ = 0x01,
    BIP_OPEN_CHANNEL_RSP,

    BIP_CLOSE_CHANNEL_REQ,
    BIP_CLOSE_CHANNEL_RSP,

    BIP_RECEIVE_DATA_REQ,
    BIP_RECEIVE_DATA_RSP,

    BIP_SEND_DATA_REQ,
    BIP_SEND_DATA_RSP,

    BIP_GET_CHANNEL_STATUS_REQ,
    BIP_GET_CHANNEL_STATUS_RSP,

    BIP_CHANNEL_STATUS_REQ,
    BIP_CHANNEL_STATUS_RSP,

    BIP_DATA_AVAILABLE_NOTIFY,
    BIP_CHANNEL_STATUS_NOTIFY,

    BIP_EVENT_BUTT
}huawei_bip_command_enum;

/*BIP Open Channel 连接模式枚举*/
typedef enum
{
    DEMAND_LINK = 1,
    IMMEDIATE_LINK = 2,
    BACKGROUND_LINK = 3
}bip_connect_mode_enum;

/*BIP Send Data 发送模式枚举*/
typedef enum
{
    STORE_DATA_TX_BUFFER = 0,
    SEND_DATA_IMMEDIATELY = 1
}bip_send_data_mode_enum;

/*BIP Channel异常 自动连接模式枚举*/
typedef enum
{
    NO_AUTOMATIC_RECONNECTION = 0,
    AUTOMATIC_RECONNECTION = 1
}bip_auto_reconnection_enum;

/*BIP Rx buffer 状态枚举*/
typedef enum
{
    BIP_TRANS_UDP = 1,
    BIP_TRANS_TCP = 2,  /*TCP client remote server */
}bip_trans_protocal_enum;

/*open channel, bip_drv --> bipclient request*/   
typedef struct
{
    BSP_U8                ConnectMode;
    BSP_U8                AutoConnect;
    BSP_U8                BearerDesp;
    BSP_U8                TransProtocolType;
    BSP_U16               BufferSize;
    BSP_U16               PortNum;
    BSP_U8                IPAddrType;
    BSP_U8                IPAddrLen;
    BSP_U8                IPAddr[BIP_MAX_IPADDRESS_LENGTH];
    BSP_U8                ApnLen;
    BSP_U8                ApnName[BIP_MAX_NETWORK_ACCESS_NAME_LENGTH];
    BSP_U8                UserNameCode;
    BSP_U8                UserNameLen;
    BSP_U8                UserName[BIP_MAX_DIAL_USERNAME_LEN];
    BSP_U8                PasswordCode;
    BSP_U8                PasswordLen;
    BSP_U8                Password[BIP_MAX_DIAL_PASSWORD_LEN];
    BSP_U8                BearerLen;
    BSP_U8                BearerPara[BIP_MAX_CHANNEL_DATA_LENGTH];
}OPEN_CHANNEL_REQ;

/*open channel, bipclient --> bip_drv response */  
typedef struct
{
    BSP_U8                ChannelStatus;
    BSP_U8                Reserved;
    BSP_U16               BufferSize;
    BSP_U16               SourcePort;
    BSP_U8                IPAddrType;
    BSP_U8                IPAddrLen;
    BSP_U8                IPAddr[BIP_MAX_IPADDRESS_LENGTH];
    BSP_U32               SocketNum;
}OPEN_CHANNEL_RSP;

/*close channel, bip_drv --> bipclient request */
typedef struct
{
    BSP_U32                 Reserved;
}CLOSE_CHANNEL_REQ;

 /*close channel, bipclient --> bip_drv response */
 typedef struct
{
    BSP_U8                 ChannelStatus;
    BSP_U8                 Reserved;
    BSP_U16                reservedAlign;
    BSP_U32                SocketNum;
}CLOSE_CHANNEL_RSP;
 
 /*RECEIVE DATA available, bip_drv --> bipclient request */ 
 typedef struct
 {
     BSP_U8               ReqDataLen;
     BSP_U8               Reserved;
     BSP_U16              xReserved;
 }RECEIVE_DATA_REQ;
 
 /*RECEIVE DATA available, bipclient --> bip_drv response */ 
 typedef struct
 {
     BSP_U8               LeftDataLen;
     BSP_U8               DataLen;
     BSP_U8               ChannelData[BIP_MAX_CHANNEL_DATA_LENGTH];
 }RECEIVE_DATA_RSP;

/*Send DATA, bip_drv --> bipclient request */ 
typedef struct
{ 
    BSP_U8               SendDataMode;
    BSP_U8               Reserved;
    BSP_U16              ChannleDataLen;
    BSP_U8               ChannelData[BIP_MAX_CHANNEL_DATA_LENGTH];
}SEND_DATA_REQ;

/*Send DATA, bipclient --> bip_drv response */ 
typedef struct
{
    BSP_U16              IdleChannleDataLen;
    BSP_U16              Reserved;
}SEND_DATA_RSP;

/*channel status, bip_drv --> bipclient request*/
typedef struct
{
    BSP_U32                 Reserved;
}CHANNEL_STATUS_REQ;

/*channel status, bipclient --> bip_drv response */
typedef struct
{
    BSP_U8                 ChannelStatus;
    BSP_U8                 Reserved;
    BSP_U16                reservedAlign;
    BSP_U32                SocketNum;
}CHANNEL_STATUS_RSP;

/*Channel Status Event download, bipclient --> bip_drv notify */
typedef struct
{
   BSP_U16                AvailableDataLen;
   BSP_U16                Reserved;
}DATA_AVAILABLE_NOTIFY;

/*Channel Status Event download, bipclient --> bip_drv notify */   
typedef struct
{
    BSP_U8                ChannelStatus;
    BSP_U8                Reserved;
    BSP_U8                IPAddrType;
    BSP_U8                IPAddrLen;
    BSP_U8                IPAddr[BIP_MAX_IPADDRESS_LENGTH];
    BSP_U32               SocketNum;
}CHANNEL_STATUS_NOTIFY;

/*BIP Client和 BIP字符设备通信 统一结构*/   
typedef struct
{
    BSP_U8                CmdType;
    BSP_U8                ResultValue;
    BSP_U8                ChannelNum;
    BSP_U8                Reserved;

    union
    {
        OPEN_CHANNEL_REQ  open_channel_req;
        OPEN_CHANNEL_RSP  open_channel_rsp;
        CLOSE_CHANNEL_REQ close_channel_req;
        CLOSE_CHANNEL_RSP close_channel_rsp;
        RECEIVE_DATA_REQ  receive_data_req;
        RECEIVE_DATA_RSP  receive_data_rsp;
        SEND_DATA_REQ     send_data_req;
        SEND_DATA_RSP     send_data_rsp;
        CHANNEL_STATUS_REQ channel_status_req;
        CHANNEL_STATUS_RSP channel_status_rsp;
        DATA_AVAILABLE_NOTIFY data_available_event;
        CHANNEL_STATUS_NOTIFY channel_status_event;
    } CmdEventStru;
}BIP_Command_Event_STRU;

/*测试结果返回给AT，以便于进行AT上报，有利于实现自动化测试设计*/
typedef struct
{
    BSP_U8                ucCmdType;
    BSP_U8                ucResultCodeLen;  /*指示结果码的个数*/
    BSP_U8                ucExtendInfoLen;  /*指示额外信息的长度数*/
    BSP_U8                aucResultCode[3];    /*最多支持三个结果码*/
    BSP_U16               ausExtInfoData[3];    /*最多支持三个附加信息*/
}BIP_TestCmdResult_STRU;

void bip_event_receiver(void *data, BSP_S32 len);
void AT_BipTestResultReport(void *data, BSP_U32 len);

BSP_U16 SI_STK_DataSendBipEvent(BSP_U16 DataLen, BSP_U8 *pData);
BSP_U16 SI_STK_DataSendBipTestEvent(BSP_U16 DataLen, BSP_U8 *pData);

BSP_U16 STUB_USIMM_SatDataInd(BSP_U8   ucCmdType, BSP_U16 usDataLen, BSP_U8 *pucData);


#ifdef __cplusplus
}
#endif


#endif /* #ifndef _BSP_BIP_H_ */


