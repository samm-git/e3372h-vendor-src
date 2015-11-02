 /*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
 * foss@huawei.com
 *
 * If distributed as part of the Linux kernel, the following license terms
 * apply:
 *
 * * This program is free software; you can redistribute it and/or modify
 * * it under the terms of the GNU General Public License version 2 and 
 * * only version 2 as published by the Free Software Foundation.
 * *
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * * GNU General Public License for more details.
 * *
 * * You should have received a copy of the GNU General Public License
 * * along with this program; if not, write to the Free Software
 * * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Otherwise, the following license terms apply:
 *
 * * Redistribution and use in source and binary forms, with or without
 * * modification, are permitted provided that the following conditions
 * * are met:
 * * 1) Redistributions of source code must retain the above copyright
 * *    notice, this list of conditions and the following disclaimer.
 * * 2) Redistributions in binary form must reproduce the above copyright
 * *    notice, this list of conditions and the following disclaimer in the
 * *    documentation and/or other materials provided with the distribution.
 * * 3) Neither the name of Huawei nor the names of its contributors may 
 * *    be used to endorse or promote products derived from this software 
 * *    without specific prior written permission.
 * 
 * * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */
/*lint -save -e19 -e123 -e537 -e713*/
/******************************************************************************************************
    问题单号                       时间                   修改人                        说明
DTS2013111808849               2013-11-18          ankang 00252937                hilink PNP
DTS2013111907319               2013-11-19          zhangkuo 00248699   coverity,fortify告警清零  
DTS2013111808826               2013-11-22          liulimin 00193392              无线充电USB代码上库
DTS2013111202833               2013-11-22          ligang 00212897              PMU误报中断处理
DTS2013111809086               2013-11-25          liulimin 00193392     插拔USB死机，问题原因为切换端口的work没有执行完，
                                                                         单板收到拔出中断，销毁gadget，引起空指针
DTS2013111809248               2013-12-04           huangfuli 00122846     平台coverity,fortify 报告清零。																		 
DTS2013122404067               2013-12-26           ankang 00252937      Setport 去掉RNDIS口
DTS2014012201641               2014-01-21           ankang 00252937      实现at^getportmode和at^dialmode命令
DTS2014021906342               2014-04-03           hepanzhou00230714    网关优化
DTS2014081408221               2014-08-14           hepanzhou00230714     fortify警告清零
DTS2014102905845               2014-10-30           h00122846             USB归一化代码升级
******************************************************************************************************/
#include "drv_usb.h"
#include "bsp_usb.h"
#include "usb_vendor.h"
#if (FEATURE_ON == MBB_USB)
#include "mbb_usb_adp.h"
#if USB_IS_SUPPORT_NV
#include "drv_nvim.h"
#include "bsp_nvim.h"
#include "drv_nv_id.h"
#endif
#endif
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/module.h>

static unsigned g_usb_enum_done_cur = 0;
static unsigned g_usb_enum_dis_cur = 0;
#if (FEATURE_ON == MBB_USB)
#define MAX_EPIN_NUM            15
#define MAX_EPOUT_NUM           15
#endif

#if (FEATURE_ON == MBB_COMMON)
#define ERROR (-1)
#endif

static struct notifier_block gs_adp_usb_nb;
static struct notifier_block *gs_adp_usb_nb_ptr = NULL;
static int g_usb_enum_done_notify_complete = 0;
static int g_usb_disable_notify_complete = 0;
static USB_CTX_S g_usb_ctx = {{0},{0},{0}};

#if (FEATURE_ON == MBB_USB)

const unsigned char gDevProfileSupported[] = 
{
    USB_IF_PROTOCOL_3G_DIAG, 
#if (FEATURE_OFF == MBB_USB_E5)
    USB_IF_PROTOCOL_MODEM,
    USB_IF_PROTOCOL_3G_MODEM,
#endif
    USB_IF_PROTOCOL_CTRL, 
    USB_IF_PROTOCOL_PCUI,
    USB_IF_PROTOCOL_DIAG,
    USB_IF_PROTOCOL_3G_GPS,
    USB_IF_PROTOCOL_GPS,
    USB_IF_PROTOCOL_BLUETOOTH,
    USB_IF_PROTOCOL_NCM,
    USB_IF_PROTOCOL_CDROM,
#if (FEATURE_ON == MBB_USB_SD)    
    USB_IF_PROTOCOL_SDRAM,
#endif
};
#endif

/*****************************************************************************
 函 数 名  : BSP_USB_SetPid
 功能描述  :
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 无
*****************************************************************************/
int BSP_USB_SetPid(unsigned char u2diagValue)
{
    return OK;
}

/*****************************************************************************
函数名：   BSP_USB_PortTypeQuery
功能描述:  查询当前的设备枚举的端口形态值
输入参数： stDynamicPidType  端口形态
输出参数： stDynamicPidType  端口形态
返回值：   0:    查询成功
           其他：查询失败
*****************************************************************************/
unsigned int BSP_USB_PortTypeQuery(DRV_DYNAMIC_PID_TYPE_STRU *pstDynamicPidType)
{
#if (FEATURE_ON == MBB_USB)
/* BEGIN PN: DTS2013091401665, add by h00122846, 2013/09/14 */
    unsigned  int ret = 0;
    if (NULL != pstDynamicPidType)
    {
        memset(pstDynamicPidType, 0, sizeof(DRV_DYNAMIC_PID_TYPE_STRU));
        ret = NVM_Read(NV_ID_DRV_USB_DYNAMIC_PID_TYPE_PARAM, (void*)pstDynamicPidType,sizeof(DRV_DYNAMIC_PID_TYPE_STRU));
    }
   
    return ret;
/* END PN: DTS2013091401665, add by h00122846, 2013/09/14 */
#else
    return OK;
#endif	
}

/*****************************************************************************
函数名：   BSP_USB_PortTypeValidCheck
功能描述:  提供给上层查询设备端口形态配置合法性接口
           1、端口为已支持类型，2、包含PCUI口，3、无重复端口，4、端点数不超过16，
           5、第一个设备不为MASS类
输入参数： pucPortType  端口形态配置
           ulPortNum    端口形态个数
返回值：   0:    端口形态合法
           其他：端口形态非法
*****************************************************************************/
unsigned int BSP_USB_PortTypeValidCheck(unsigned char *pucPortType, unsigned long ulPortNum)
{
#if (FEATURE_ON == MBB_USB)
/* BEGIN PN: DTS2013101203547,Added by h00122846, 2013/10/14*/
    uint32_t cnt = 0, type = 0, dev_loc = 0;
    uint32_t ep_in_cnt = 0, ep_out_cnt = 0;
    bool pcui_present =  false;
    uint8_t port_exist[256] = {0};

    printk(KERN_ERR "BSP_USB_PortTypeValidCheck");

    if ((NULL == pucPortType) || (0 == ulPortNum) || (DYNAMIC_PID_MAX_PORT_NUM < ulPortNum))
    {
        printk(KERN_ERR "%s: invalid param buf[%p] num[%ld]\n", __FUNCTION__, pucPortType, ulPortNum);
        return 1;
    }

    type = pucPortType[0];

    /* 检查第一个设备不能为MASS设备或空设备 */
    if ((USB_IF_PROTOCOL_CDROM == type) || (USB_IF_PROTOCOL_SDRAM == type) ||
        (USB_IF_PROTOCOL_VOID == type))
    {
        printk(KERN_ERR "%s: First device is Mass Storage device!\r\n", __FUNCTION__);
        return 1;
    }

#ifdef CONFIG_BALONG_RNDIS        // Hilink形态不支持Setport
    return 1;
#else
    /* 检查切换后的设备形态 */
    for (cnt = 0; cnt < ulPortNum; cnt++)
    {
        type = pucPortType[cnt];

        if (0 < port_exist[type])
        {
            printk(KERN_ERR "%s: Port type repeat\n", __FUNCTION__);
            return 1;
        }

#if (FEATURE_ON == MBB_USB_E5)
        /* E5产品不能带Modem设备 */
        if ((USB_IF_PROTOCOL_3G_MODEM == type) || (USB_IF_PROTOCOL_MODEM == type))
        {
            printk(KERN_ERR "%s: There is MODEM with E5!\r\n", __FUNCTION__);
            return 1;
        }
#endif
        /* 检查PCUI端口是否存在 */
        if ((USB_IF_PROTOCOL_3G_PCUI == type) || (USB_IF_PROTOCOL_PCUI == type))
        {
            pcui_present = true;
        }

        /* 检查设备形态是否有效支持 */
        for (dev_loc = 0; dev_loc < sizeof(gDevProfileSupported); dev_loc++)
        {
            if (type == gDevProfileSupported[dev_loc])
            {
                break;
            }
        }

        if (dev_loc == sizeof(gDevProfileSupported))
        {
            printk(KERN_ERR "%s: Port type isn't supported by current device!\r\n", __FUNCTION__);
            printk(KERN_ERR "%s: dev_loc = [%d], type = [%d]\r\n", __FUNCTION__, dev_loc, type);
            return 1;
        }

        /* 统计特定设备形态需要的USB端点数目 */
        if ((USB_IF_PROTOCOL_NCM  == type) || (USB_IF_PROTOCOL_MODEM   == type) ||
            (USB_IF_PROTOCOL_NDIS == type) || (USB_IF_PROTOCOL_RNDIS   == type) ||
            (USB_IF_PROTOCOL_PCSC == type) || (USB_IF_PROTOCOL_3G_NDIS == type) ||
            (USB_IF_PROTOCOL_3G_MODEM == type))
        {
            ep_in_cnt  += 2;    /* Bulk IN + Interrupt IN */
            ep_out_cnt += 1;    /* Bulk OUT */
        }
        else
        {
            ep_in_cnt  += 1;    /* Bulk IN */
            ep_out_cnt += 1;    /* Bulk OUT */
        }

        port_exist[type]++;
    }

    /* 无PCUI端口，则判为非法 */
    if (false == pcui_present)
    {
        printk(KERN_ERR "%s: No PCUI!\r\n", __FUNCTION__);
        return 1;
    }

    /* 判断需要的USB端点数是否超过USB IP的支持 */
    if ((ep_in_cnt > MAX_EPIN_NUM) || (ep_out_cnt > MAX_EPOUT_NUM))
    {
        printk(KERN_ERR "%s: Too many ports-ep_in_cnt[%d], ep_out_cnt[%d]\n", __FUNCTION__,
                ep_in_cnt, ep_out_cnt);
        return 1;
    }
    return 0;
#endif /*#ifdef MBB_USB_RNDIS*/
    /* 设备形态参数正常 */
/* END PN: DTS2013101203547,Added by h00122846, 2013/10/14*/
#else	
    return OK;
#endif
}

/*****************************************************************************
函数名：   BSP_USB_GetAvailabePortType
功能描述:  提供给上层查询当前设备支持端口形态列表接口
输入参数： ulPortMax    协议栈支持最大端口形态个数
输出参数:  pucPortType  支持的端口形态列表
           pulPortNum   支持的端口形态个数
返回值：   0:    获取端口形态列表成功
           其他：获取端口形态列表失败
*****************************************************************************/
unsigned int BSP_USB_GetAvailabePortType(unsigned char *pucPortType,
                            unsigned long *pulPortNum, unsigned long ulPortMax)
{
#if (FEATURE_ON == MBB_USB)
/* BEGIN PN: DTS2013101203547,Added by h00122846, 2013/10/14*/
    unsigned int i = 0;
    unsigned int cnt = 0;
    unsigned int j = 0;

    if ((NULL == pucPortType) || (NULL == pulPortNum) || (0 == ulPortMax))
    {
        return 1;
    }  

    cnt = sizeof(gDevProfileSupported) / sizeof(char);
    
    printk(KERN_ERR "aTm support  fds\n");
    if (cnt > ulPortMax)
    {
        return 1;
    }

    for (i = 0; i < cnt; i++)
    {
        pucPortType[j++] = gDevProfileSupported[i];
    }

    *pulPortNum = j;
 /* END PN: DTS2013101203547,Added by h00122846, 2013/10/14*/
#endif  
    return OK;
}

/*****************************************************************************
 函 数 名  : BSP_USB_ATProcessRewind2Cmd
 功能描述  : rewind2 CMD 处理
 输入参数  : pData：数据
 输出参数  : 无
 返 回 值  : false(0):处理失败
             tool(1):处理成功
*****************************************************************************/
int BSP_USB_ATProcessRewind2Cmd(unsigned char *pData)
{
    return OK;
}

/*****************************************************************************
 函 数 名  : BSP_USB_GetDiagModeValue
 功能描述  : 获得设备类型
 输入参数  : 无
 输出参数  : ucDialmode:  0 - 使用Modem拨号; 1 - 使用NDIS拨号; 2 - Modem和NDIS共存
              ucCdcSpec:   0 - Modem/NDIS都符合CDC规范; 1 - Modem符合CDC规范;
                           2 - NDIS符合CDC规范;         3 - Modem/NDIS都符合CDC规范
 返 回 值  : VOS_OK/VOS_ERR
*****************************************************************************/
BSP_S32 BSP_USB_GetDiagModeValue(unsigned char *pucDialmode,
                             unsigned char *pucCdcSpec)
{
#if (FEATURE_ON == MBB_USB)
    unsigned int ret = 1;

    printk(KERN_ERR" BSP_USB_GetDiagModeValue \n");

    ret = Query_USB_PortType(pucDialmode,pucCdcSpec);
    if ( 0 != ret )
    {
        return 1; 
    }
#endif    
    return 0;
}

/*****************************************************************************
 函 数 名  :     BSP_USB_GetPortMode
 功能描述  :  获取端口形态模式，网关对接需求，打桩。
 输入参数  :  PsBuffer   保存查询到的端口名称及端口上报的顺序
                            Length     记录*PsBuffer中字符串的长度
 输出参数  : 
 返 回 值  :      成功返回0，失败返回1
*****************************************************************************/
unsigned char BSP_USB_GetPortMode(char*PsBuffer, unsigned long*Length )
{
#if (FEATURE_ON == MBB_USB)
    printk(KERN_ERR"BSP_USB_GetPortMode Enter!\r\n");
    unsigned char ret = 1;

    ret = Check_EnablePortName(PsBuffer,Length);
    if( 0 != ret )
    {
        return 1;
    }
#endif
    return 0;
}

/*****************************************************************************
 函 数 名  : BSP_USB_GetU2diagDefaultValue
 功能描述  : 获得端口默认u2diag值
 输入参数  : 无
 输出参数  : 无
 返回值：   u2diag值

*****************************************************************************/
BSP_U32 BSP_USB_GetU2diagDefaultValue(void)
{
    return 0;
}

/*****************************************************************************
 函 数 名  : ErrlogRegFunc
 功能描述  : USB MNTN注册异常日志接口，Porting项目中打桩
             Added by c00204787 for errorlog,20120211
 输入参数  : 无
 输出参数  : 无
 返回值    ：无

*****************************************************************************/
void MNTN_ERRLOG_REG_FUNC(MNTN_ERRLOGREGFUN pRegFunc)
{
    return ;
}

/*****************************************************************************
 函 数 名  : BSP_USB_UdiagValueCheck
 功能描述  : 本接口用于检查NV项中USB形态值的合法性
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 0：OK
             -1：ERROR
*****************************************************************************/
int BSP_USB_UdiagValueCheck(unsigned long DiagValue)
{
    return OK;
}

/*****************************************************************************
 函 数 名  : BSP_USB_GetLinuxSysType
 功能描述  : 本接口用于检查PC侧是否为Linux，以规避Linux后台二次拨号失败的问题.
 输入参数  : 无。
 输出参数  : 无。
 返 回 值  : 0：Linux；
            -1：非Linux。
*****************************************************************************/
int BSP_USB_GetLinuxSysType(void)
{
    return -1;
}

/********************************************************
函数说明： 返回当前设备列表中支持(sel=1)或者不支持(sel=0)PCSC的设备形态值
函数功能:
输入参数：sel
          0: 通过参数dev_type返回当前不带PCSC是设备形态值
          1：通过参数dev_type返回当前带PCSC是设备形态值
输出参数：dev_type 写入需要的设备形态值，如果没有则不写入值。
          NV中存储的设备形态值
输出参数：pulDevType 与ulCurDevType对应的设备形态值，如果没有返回值1。
返回值：
          0：查询到相应的设备形态值；
          1：没有查询到响应的设备形态值。
********************************************************/
int BSP_USB_PcscInfoSet(unsigned int  ulSel, unsigned int  ulCurDevType, unsigned int *pulDevType)
{
    return 0;
}

/********************************************************
函数说明：协议栈注册USB使能通知回调函数
函数功能:
输入参数：pFunc: USB使能回调函数指针
输出参数：无
输出参数：无
返回值  ：0：成功
          1：失败
********************************************************/
unsigned int BSP_USB_RegUdiEnableCB(USB_UDI_ENABLE_CB_T pFunc)
{
    if (g_usb_enum_done_cur >= USB_ENABLE_CB_MAX)
    {
        printk("BSP_USB_RegUdiEnableCB error:0x%x", (unsigned)pFunc);

        return 1;

    }

    g_usb_ctx.udi_enable_cb[g_usb_enum_done_cur] = pFunc;
    g_usb_enum_done_cur++;

	if (g_usb_enum_done_notify_complete)
    {
    	if (pFunc)
        	pFunc();
    }

    return 0;
}

/********************************************************
函数说明：协议栈注册USB去使能通知回调函数
函数功能:
输入参数：pFunc: USB去使能回调函数指针
输出参数：无
输出参数：无
返回值  ：0：成功
          1：失败
********************************************************/
unsigned int BSP_USB_RegUdiDisableCB(USB_UDI_DISABLE_CB_T pFunc)
{
    if (g_usb_enum_dis_cur >= USB_ENABLE_CB_MAX)
    {
        printk("BSP_USB_RegUdiDisableCB error:0x%x", (unsigned)pFunc);
        return (unsigned int)ERROR;
    }

    g_usb_ctx.udi_disable_cb[g_usb_enum_dis_cur] = pFunc;
    g_usb_enum_dis_cur++;

    return OK;
}

unsigned int BSP_USB_RegIpsTraceCB(USB_IPS_MNTN_TRACE_CB_T pFunc)
{
    if (!pFunc)
    {
        return 1;
    }

    return 0;
}

/********************************************************
函数说明：协议栈注册HSIC使能通知回调函数
函数功能:
输入参数：pFunc: HSIC使能回调函数指针
输出参数：无
输出参数：无
返回值  ：0：成功
          1：失败
********************************************************/
unsigned int BSP_HSIC_RegUdiEnableCB(HSIC_UDI_ENABLE_CB_T pFunc)
{
    return OK;
}

/********************************************************
函数说明：协议栈注册HSIC去使能通知回调函数
函数功能:
输入参数：pFunc: HSIC去使能回调函数指针
输出参数：无
输出参数：无
返回值：  0：成功
          1：失败
********************************************************/
unsigned int BSP_HSIC_RegUdiDisableCB(HSIC_UDI_DISABLE_CB_T pFunc)
{
    return OK;
}

unsigned long USB_ETH_DrvSetDeviceAssembleParam(
    unsigned long ulEthTxMinNum,
    unsigned long ulEthTxTimeout,
    unsigned long ulEthRxMinNum,
    unsigned long ulEthRxTimeout)
{
    return 0;
}

unsigned long USB_ETH_DrvSetHostAssembleParam(unsigned long ulHostOutTimeout)
{
#ifdef CONFIG_BALONG_NCM
    /* the interface don't include net_id, so we just set net_id:0 */
    (void)ncm_set_host_assemble_param(0, ulHostOutTimeout);
#endif
    return 0;
}

int USB_otg_switch_get(UINT8 *pvalue)
{
    return 0;
}

int USB_otg_switch_set(UINT8 value)
{
    return 0;
}

int USB_otg_switch_signal_set(UINT8 group,UINT8 pin, UINT8 value )
{
    return 0;
}

/********************************************************
函数说明：协议栈查询HSIC枚举状态
函数功能:
输入参数：无
输出参数：无
输出参数：无
返回值：  1：枚举完成
          0：枚举未完成
********************************************************/
unsigned int BSP_GetHsicEnumStatus(void)
{
    return OK;
}

/*****************************************************************************
 函 数 名  : BSP_USB_HSIC_SupportNcm
 功能描述  : 本接口用于查询HSIC是否支持NCM
 输入参数  : NA
 输出参数  : NA
 返 回 值  : 1-- 支持NCM
             0--不支持NCM
*****************************************************************************/
int BSP_USB_HSIC_SupportNcm(void)
{
    return 0;
}

/********************************************************
函数说明：TTF查询预申请SKB Num
函数功能:
输入参数：无
输出参数：无
输出参数：无
返回值：  SKB Num
********************************************************/
BSP_U32 BSP_AcmPremallocSkbNum(void)
{
    return 0;
}

/********************************************************
函数名：   BSP_UDI_FindVcom
功能描述： 查询当前设备形态下指定的UDI虚拟串口端口是否存在
输入参数： UDI_DEVICE_ID枚举值，即待查询的虚拟串口端口ID
输出参数： 无
返回值：
           0：当前设备形态不支持查询的虚拟串口端口；
           1：当前设备形态支持查询的虚拟串口端口。
注意事项： 无
********************************************************/
int BSP_UDI_FindVcom(UDI_DEVICE_ID enVCOM)
{
    return 0;
}

/*****************************************************************************
* 函 数 名  : DRV_USB_RegEnumDoneForMsp
* 功能描述  : 提供给MSP注册USB枚举完成后通知函数
* 输入参数  : pFunc:枚举完成回调函数指针
* 输出参数  : 无
* 返 回 值  : 0: 成功注册,等待枚举完成通知;
*             1: USB已经枚举完成,可以直接初始化USB部分;
*             -1: 失败, 非USB形态,没有USB驱动
*****************************************************************************/
signed int BSP_USB_RegEnumDoneForMsp(void *pFunc)
{
    /* we suggest to use the new interface for usb insert/remove */
    return BSP_USB_RegUdiEnableCB(pFunc);
}

/*****************************************************************************
* 函 数 名  : BSP_USB_RegEnumDoneForPs
* 功能描述  : 提供给PS注册USB枚举完成后通知函数
* 输入参数  : pFunc:枚举完成回调函数指针
* 输出参数  : 无
* 返 回 值  : 0: 成功注册,等待枚举完成通知;
*             1: USB已经枚举完成,可以直接初始化USB部分;
*             -1: 失败,非USB形态,没有USB驱动
*****************************************************************************/
signed int BSP_USB_RegEnumDoneForPs(void *pFunc)
{
    /* we suggest to use the new interface for usb insert/remove */
    return BSP_USB_RegUdiEnableCB(pFunc);
}

/*****************************************************************************
 函 数 名  : BSP_USB_RndisAppEventDispatch
 功能描述  : 本接口用于通知APP 相应的USB插拔事件
 输出参数  : usb事件
 返 回 值  : 无
*****************************************************************************/
void BSP_USB_RndisAppEventDispatch(unsigned ulStatus)
{
    return ;
}

/************************************************************************
 * FUNCTION
 *       rndis_app_event_dispatch
 * DESCRIPTION
 *       闪电卡版本控制应用进行拨号或断开拨号连接
 * INPUTS
 *       进行拨号或断开拨号指示
 * OUTPUTS
 *       NONE
 *************************************************************************/
VOID rndis_app_event_dispatch(unsigned int ulStatus)
{
    return ;
}

/*****************************************************************************
 函 数 名  : BSP_USB_NASSwitchGatewayRegExtFunc
 功能描述  : 本接口用于NAS注册切换网关通知回调函数
 输入参数  : 回调接口。
 输出参数  : 无。
 返 回 值  : 0：成功
             零:失败
*****************************************************************************/
int BSP_USB_NASSwitchGatewayRegFunc(USB_NET_DEV_SWITCH_GATEWAY switchGwMode)
{
#if (FEATURE_ON == MBB_USB)
    usb_adp_ctx_t* ctx = NULL;

    ctx = usb_get_adp_ctx();
    if (NULL == ctx)
    {
        printk(KERN_ERR"NASS set gateway func fail \n");
        return ERROR;
    }
    ctx->usb_switch_gatway_mode_cb = switchGwMode;
#endif
    return OK;
}

/*****************************************************************************
 函 数 名  : bsp_usb_register_enablecb
 功能描述  : 本接口用于Cshell通知USB插入回调函数
 输入参数  : 回调接口。
 输出参数  : 无。
 返 回 值  : 0：成功
             零:失败
*****************************************************************************/
int bsp_usb_register_enablecb(USB_UDI_ENABLE_CB_T pFunc)
{
    /* we suggest to use the new interface for usb insert/remove */
    return BSP_USB_RegUdiEnableCB(pFunc);
}

/*****************************************************************************
 函 数 名  : bsp_usb_register_disablecb
 功能描述  : 本接口用于Cshell通知USB拔出回调函数
 输入参数  : 回调接口。
 输出参数  : 无。
 返 回 值  : 0：成功
             零:失败
*****************************************************************************/
int bsp_usb_register_disablecb(USB_UDI_DISABLE_CB_T pFunc)
{
    /* we suggest to use the new interface for usb insert/remove */
    return BSP_USB_RegUdiDisableCB(pFunc);
}

unsigned long USB_ETH_DrvSetRxFlowCtrl(unsigned long ulParam1, unsigned long ulParam2)
{
    return 0;
}

unsigned long USB_ETH_DrvClearRxFlowCtrl(unsigned long ulParam1, unsigned long ulParam2)
{
    return 0;
}

/************************************************************************
 * FUNCTION
 *       rndis_app_event_dispatch
 * DESCRIPTION
 *       闪电卡版本控制应用进行拨号或断开拨号连接
 * INPUTS
 *       进行拨号或断开拨号指示
 * OUTPUTS
 *       NONE
 *************************************************************************/
VOID DRV_AT_SETAPPDAILMODE(unsigned int ulStatus)
{

}

int l2_notify_register(FUNC_USB_LP_NOTIFY pUSBLPFunc)
{
    return 0;
}

static int gs_usb_adp_notifier_cb(struct notifier_block *nb,
            unsigned long event, void *priv)
{
    int loop;

    switch (event) {

    case USB_BALONG_DEVICE_INSERT:
        g_usb_disable_notify_complete = 0;
        break;
    case USB_BALONG_ENUM_DONE:
        /* enum done */
        g_usb_disable_notify_complete = 0;
        if (!g_usb_enum_done_notify_complete) {
            for(loop = 0; loop < USB_ENUM_DONE_CB_BOTTEM; loop++)
            {
                if(g_usb_ctx.enum_done_cbs[loop])
                    g_usb_ctx.enum_done_cbs[loop]();
            }

            for(loop = 0; loop < USB_ENABLE_CB_MAX; loop++)
            {
                if(g_usb_ctx.udi_enable_cb[loop])
                    g_usb_ctx.udi_enable_cb[loop]();
            }
        }
        g_usb_enum_done_notify_complete = 1;
        break;
    case USB_BALONG_DEVICE_DISABLE:
    case USB_BALONG_DEVICE_REMOVE:
        /* notify other cb */
        g_usb_enum_done_notify_complete = 0;
        if (!g_usb_disable_notify_complete) {
            for(loop = 0; loop < USB_ENABLE_CB_MAX; loop++)
            {
                if(g_usb_ctx.udi_disable_cb[loop])
                    g_usb_ctx.udi_disable_cb[loop]();
            }
            g_usb_disable_notify_complete = 1;
        }
        break;
    default:
        break;
    }
    return 0;
}


int __init adp_usb_init(void)
{
    /* we just regist once, and don't unregist any more */
    if (!gs_adp_usb_nb_ptr) {
        gs_adp_usb_nb_ptr = &gs_adp_usb_nb;
        gs_adp_usb_nb.priority = USB_NOTIF_PRIO_ADP;
        gs_adp_usb_nb.notifier_call = gs_usb_adp_notifier_cb;
        bsp_usb_register_notify(gs_adp_usb_nb_ptr);
    }
    return 0;
}
module_init(adp_usb_init);
/*lint -restore*/