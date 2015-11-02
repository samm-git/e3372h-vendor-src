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
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include <linux/slab.h>
#include <linux/err.h>
#include <bsp_acm.h>
#include <bsp_ncm.h>
#include <drv_icc.h>
#include "udi_balong.h"

#ifdef CONFIG_NCM_MBIM_SUPPORT /* M-PDP */
#define  MAX_EX_NCM_INST_NUM 7
#endif

static BSP_S32 udiAdpNcmInit(VOID);
static BSP_S32 udiAdpIccInit(VOID);
static BSP_S32 udiAdpAcmInit(VOID);
static BSP_S32 udiAdpIccInit(VOID);



/* 各模块特性值定义 */
#define UDI_USB_ACM_CAPA		(UDI_CAPA_BLOCK_READ | UDI_CAPA_BLOCK_WRITE | UDI_CAPA_READ_CB | UDI_CAPA_WRITE_CB)
#define UDI_USB_NCM_NDIS_CAPA	(UDI_CAPA_READ_CB | UDI_CAPA_BUFFER_LIST)
#define UDI_USB_NCM_CTRL_CAPA	(UDI_CAPA_READ_CB | UDI_CAPA_CTRL_OPT)
#define UDI_ICC_CAPA				(UDI_CAPA_BLOCK_READ | UDI_CAPA_BLOCK_WRITE | UDI_CAPA_READ_CB | UDI_CAPA_WRITE_CB)
#define UDI_UART_CAPA			(UDI_CAPA_BLOCK_READ | UDI_CAPA_BLOCK_WRITE)


/* 各模块初始化函数定义 */
VOID* g_udiInitFuncTable[(BSP_U32)UDI_DEV_MAX+1] =
{
	/* ACM Init */
	udiAdpAcmInit,

	/* NCM Init */
	udiAdpNcmInit,

	/* ICC Init */
	udiAdpIccInit,

	/* Must Be END */
	NULL
};

/*** eric **/

static BSP_S32 udiAcmAdpOpen(UDI_OPEN_PARAM *param, UDI_HANDLE handle)
{
	//BSP_U8* pstrName;
	BSP_U32 u32Type;
	BSP_S32 s32Fd;

	UDI_UNUSED_PARAM(handle);

	u32Type = UDI_GET_DEV_TYPE(param->devid);
	s32Fd = bsp_acm_open(u32Type);

	if (IS_ERR((void*)s32Fd))
	{
		return ERROR;
	}

	(BSP_VOID)BSP_UDI_SetPrivate(param->devid, (VOID*)s32Fd);
	return OK;
}

static BSP_S32 udiAdpAcmInit(VOID)
{
	UDI_DRV_INTEFACE_TABLE* pDrvInterface = NULL;
	BSP_U32 u32Cnt;

	/* 构造回调函数指针列表 */
	pDrvInterface = kmalloc(sizeof(UDI_DRV_INTEFACE_TABLE), GFP_KERNEL);
	if (NULL == pDrvInterface)
	{
		printk(KERN_ERR "BSP_MODU_UDI NO Mem, line:%d\n", __LINE__);
		return ERROR;
	}
	memset((VOID*)pDrvInterface, 0, sizeof(UDI_DRV_INTEFACE_TABLE));

	/* 只设置需要实现的几个接口 */
	pDrvInterface->udi_open_cb = udiAcmAdpOpen;
	pDrvInterface->udi_close_cb = (UDI_CLOSE_CB_T)bsp_acm_close;
	pDrvInterface->udi_write_cb = (UDI_WRITE_CB_T)bsp_acm_write;
	pDrvInterface->udi_read_cb = (UDI_READ_CB_T)bsp_acm_read;
	pDrvInterface->udi_ioctl_cb = (UDI_IOCTL_CB_T)bsp_acm_ioctl;

	/*  ACM 都使用同一套驱动函数指针 */
	for (u32Cnt = UDI_USB_ACM_CTRL; u32Cnt < UDI_USB_ACM_MAX; u32Cnt++)
	{
		(BSP_VOID)BSP_UDI_SetCapability((UDI_DEVICE_ID)UDI_BUILD_DEV_ID(UDI_DEV_USB_ACM, u32Cnt), UDI_USB_ACM_CAPA);
		(BSP_VOID)BSP_UDI_SetInterfaceTable((UDI_DEVICE_ID)UDI_BUILD_DEV_ID(UDI_DEV_USB_ACM, u32Cnt), pDrvInterface);
	}

	return OK;
}

/**************************************************************************
  NCM 适配实现
**************************************************************************/
#define BSP_NCM_MAX_DEV_NUM  4 /* 最大NCM设备数量，用作数组定义*/
static BSP_U32 sg_chnNcmDataHandleTbl[BSP_NCM_MAX_DEV_NUM] = {0};
#define UDI2NCM_ID(udi_id) ((udi_id) / 2)

UDI_HANDLE BSP_UDI_NCM_DataChnToHandle(BSP_U32 u32Chn)
{
    return sg_chnNcmDataHandleTbl[u32Chn];
}

static BSP_S32 udiNcmAdpOpen(UDI_OPEN_PARAM *param, UDI_HANDLE handle)
{
	BSP_U32 u32NcmDevId = 0;
	BSP_U32 u32Ret;
	BSP_U32 u32ChanId;

	UDI_UNUSED_PARAM(handle);

	u32ChanId = UDI_GET_DEV_TYPE(param->devid);
	u32NcmDevId = UDI2NCM_ID(u32ChanId);

	u32Ret = bsp_ncm_open(NCM_DEV_DATA_TYPE, u32NcmDevId);
	(BSP_VOID)BSP_UDI_SetPrivate(param->devid, (VOID*)u32NcmDevId);
    sg_chnNcmDataHandleTbl[u32NcmDevId] = handle;    /*lint !e732*/
	return (BSP_S32)u32Ret;
}

static BSP_S32 udiNcmAdpCtrlOpen(UDI_OPEN_PARAM *param, UDI_HANDLE handle)
{
	BSP_U32 u32NcmDevId;
	BSP_U32 u32Ret;
	/*modify bsp_ncm_open ,add by wangweichao*/
	BSP_U32 u32ChanId;

	UDI_UNUSED_PARAM(handle);

	u32ChanId = UDI_GET_DEV_TYPE(param->devid);
	u32NcmDevId = UDI2NCM_ID(u32ChanId);

	u32Ret = bsp_ncm_open(NCM_DEV_CTRL_TYPE, u32NcmDevId);
	(BSP_VOID)BSP_UDI_SetPrivate(param->devid, (VOID*)u32NcmDevId);
	return (BSP_S32)u32Ret;
}

static BSP_S32 udiNcmAdpClose(VOID* pPrivate)
{
	return (BSP_S32)bsp_ncm_close(NCM_DEV_DATA_TYPE, (BSP_U32)pPrivate);
}

static BSP_S32 udiNcmAdpCtrlClose(VOID* pPrivate)
{
	return (BSP_S32)bsp_ncm_close(NCM_DEV_CTRL_TYPE, (BSP_U32)pPrivate);
}

static BSP_S32 udiNcmAdpWrite(VOID* pPrivate, VOID* pMemObj, BSP_S32 s32Size)
{
	UDI_UNUSED_PARAM(s32Size);
	return (BSP_S32)bsp_ncm_write((BSP_U32)pPrivate, pMemObj, NULL);
}

static BSP_S32 udiAdpNcmInit(VOID)
{
	BSP_U32 i;
	UDI_DRV_INTEFACE_TABLE* pDrvDataInterface = NULL;
	UDI_DRV_INTEFACE_TABLE* pDrvCtrlInterface = NULL;
#ifdef CONFIG_NCM_MBIM_SUPPORT  /* M-PDP */
	UDI_DEVICE_ID ExNcmDevIdArray[MAX_EX_NCM_INST_NUM] = { 
	  UDI_NCM_NDIS1_ID, UDI_NCM_NDIS2_ID, UDI_NCM_NDIS3_ID, UDI_NCM_NDIS4_ID,
	  UDI_NCM_NDIS5_ID, UDI_NCM_NDIS6_ID, UDI_NCM_NDIS7_ID};
	BSP_U32 Index = 0;
	UDI_DRV_INTEFACE_TABLE *ExNcmDevIntf[MAX_EX_NCM_INST_NUM] = {NULL};
#endif

	/* 构造回调函数指针列表(数据通道) */
	pDrvDataInterface = kmalloc(sizeof(UDI_DRV_INTEFACE_TABLE), GFP_KERNEL);
	if (NULL == pDrvDataInterface)
	{
		printk(KERN_ERR "BSP_MODU_UDI NO Mem, line:%d\n", __LINE__);
		goto ADP_NCM_INIT_ERR;
	}
	memset((VOID*)pDrvDataInterface, 0, sizeof(UDI_DRV_INTEFACE_TABLE));

	/* 只设置需要实现的几个接口(数据通道) */
	pDrvDataInterface->udi_open_cb = (UDI_OPEN_CB_T)udiNcmAdpOpen;
	pDrvDataInterface->udi_close_cb = (UDI_CLOSE_CB_T)udiNcmAdpClose;
	pDrvDataInterface->udi_write_cb = (UDI_WRITE_CB_T)udiNcmAdpWrite;
	pDrvDataInterface->udi_ioctl_cb = (UDI_IOCTL_CB_T)bsp_ncm_ioctl;

	/* 设置函数指针(数据通道) */
	for(i=0; i<UDI_USB_NCM_BOTTOM/2; i++)
	{
		(BSP_VOID)BSP_UDI_SetCapability(UDI_NCM_NDIS_ID, UDI_USB_NCM_NDIS_CAPA);
		(BSP_VOID)BSP_UDI_SetInterfaceTable(UDI_NCM_NDIS_ID, pDrvDataInterface);
	}

	/* 构造回调函数指针列表(控制通道) */
	pDrvCtrlInterface = kmalloc(sizeof(UDI_DRV_INTEFACE_TABLE), GFP_KERNEL);
	if (NULL == pDrvCtrlInterface)
	{
		printk(KERN_ERR "BSP_MODU_UDI NO Mem, line:%d\n", __LINE__);
		goto ADP_NCM_INIT_ERR;
	}
	memset((VOID*)pDrvCtrlInterface, 0, sizeof(UDI_DRV_INTEFACE_TABLE));

	/* 构造回调函数指针列表(控制通道) */
	pDrvCtrlInterface->udi_open_cb = (UDI_OPEN_CB_T)udiNcmAdpCtrlOpen;
	pDrvCtrlInterface->udi_close_cb = (UDI_CLOSE_CB_T)udiNcmAdpCtrlClose;
	pDrvCtrlInterface->udi_write_cb = (UDI_WRITE_CB_T)udiNcmAdpWrite;
	pDrvCtrlInterface->udi_ioctl_cb = (UDI_IOCTL_CB_T)bsp_ncm_ioctl;

	/* 设置函数指针(数据通道) */
	for(i=0; i<UDI_USB_NCM_BOTTOM/2; i++)
	{
		(BSP_VOID)BSP_UDI_SetCapability(UDI_NCM_CTRL_ID, UDI_USB_NCM_CTRL_CAPA);
		(BSP_VOID)BSP_UDI_SetInterfaceTable(UDI_NCM_CTRL_ID, pDrvCtrlInterface);
	}

#ifdef CONFIG_NCM_MBIM_SUPPORT /* M-PDP */
	for (Index = 0; Index < MAX_EX_NCM_INST_NUM; Index++)
	{
	  /* 构造回调函数指针列表(数据通道) */
	  ExNcmDevIntf[Index] = kmalloc(sizeof(UDI_DRV_INTEFACE_TABLE), GFP_KERNEL);
	  if (NULL == ExNcmDevIntf[Index])
	  {
	    printk(KERN_ERR "WinBlue %s, alloc drv intf %u failed\n", __FUNCTION__, Index);
	    goto ADP_NCM_INIT_ERR;
	  }    

	  /* 只设置需要实现的几个接口(数据通道) */
	  ExNcmDevIntf[Index]->udi_open_cb = (UDI_OPEN_CB_T)udiNcmAdpOpen;
	  ExNcmDevIntf[Index]->udi_close_cb = (UDI_CLOSE_CB_T)udiNcmAdpClose;
	  ExNcmDevIntf[Index]->udi_write_cb = (UDI_WRITE_CB_T)udiNcmAdpWrite;
	  ExNcmDevIntf[Index]->udi_ioctl_cb = (UDI_IOCTL_CB_T)bsp_ncm_ioctl;

	  /* 设置函数指针(数据通道) */
	  (BSP_VOID)BSP_UDI_SetCapability(ExNcmDevIdArray[Index], UDI_USB_NCM_NDIS_CAPA);
	  (BSP_VOID)BSP_UDI_SetInterfaceTable(ExNcmDevIdArray[Index], ExNcmDevIntf[Index]);
	}
#endif

	return OK;

ADP_NCM_INIT_ERR:
	if (NULL != pDrvDataInterface)
	{
		/* coverity[dead_error_condition] */
		kfree(pDrvDataInterface);
	}
	/* coverity[dead_error_condition] */
	if (NULL != pDrvCtrlInterface)
	{
		/* coverity[dead_error_line] */
		kfree(pDrvCtrlInterface);
	}

#ifdef CONFIG_NCM_MBIM_SUPPORT /* M-PDP */
	printk(KERN_ERR "WinBlue %s free extra ncm dev info\n", __FUNCTION__);
	for (Index = 0; Index < MAX_EX_NCM_INST_NUM; Index++)
	{
	  if (NULL != ExNcmDevIntf[Index])
	  {
	    kfree(ExNcmDevIntf[Index]);
	  }
	}
#endif

	return ERROR;
}


/**************************************************************************
  ICC 适配实现
**************************************************************************/
static BSP_U32 sg_chnHandleTbl[ICC_CHAN_NUM_MAX] = {0};

UDI_HANDLE BSP_UDI_ICC_ChnToHandle(BSP_U32 u32Chn)
{
	return sg_chnHandleTbl[u32Chn];
}

static BSP_S32 udiIccAdpOpen(UDI_OPEN_PARAM *param, UDI_HANDLE handle)
{
	BSP_U32 u32ChanId;
	ICC_CHAN_ATTR_S *pstOpenParam;

	u32ChanId = UDI_GET_DEV_TYPE(param->devid);
	(BSP_VOID)BSP_UDI_SetPrivate(param->devid, (VOID*)u32ChanId);

	/* 从param 中解析出各个参数 */
	pstOpenParam = (ICC_CHAN_ATTR_S *)param->pPrivate;
	sg_chnHandleTbl[u32ChanId] = handle;     /*lint !e732*/

	return BSP_ICC_Open(u32ChanId, pstOpenParam);
}

static BSP_S32 udiAdpIccInit(VOID)
{
	UDI_DRV_INTEFACE_TABLE* pDrvInterface = NULL;
	BSP_U32 u32Cnt;

	/* 构造回调函数指针列表 */
	pDrvInterface = kmalloc(sizeof(UDI_DRV_INTEFACE_TABLE), GFP_KERNEL);
	if (NULL == pDrvInterface)
	{
		printk(KERN_ERR "BSP_MODU_UDI NO Mem, line:%d\n", __LINE__);
		return ERROR;
	}
	memset((VOID*)pDrvInterface, 0, sizeof(UDI_DRV_INTEFACE_TABLE));

	/* 只设置需要实现的几个接口 */
	pDrvInterface->udi_open_cb = (UDI_OPEN_CB_T)udiIccAdpOpen;
	pDrvInterface->udi_close_cb = (UDI_CLOSE_CB_T)BSP_ICC_Close;
	pDrvInterface->udi_write_cb = (UDI_WRITE_CB_T)BSP_ICC_Write;
	pDrvInterface->udi_read_cb = (UDI_READ_CB_T)BSP_ICC_Read;
	pDrvInterface->udi_ioctl_cb = (UDI_IOCTL_CB_T)BSP_ICC_Ioctl;

	/* 几个 ICC 都使用同一套驱动函数指针 */
	for (u32Cnt = 0; u32Cnt < ICC_CHAN_NUM_MAX; u32Cnt++)
	{
		(BSP_VOID)BSP_UDI_SetCapability((UDI_DEVICE_ID)UDI_BUILD_DEV_ID(UDI_DEV_ICC, u32Cnt), UDI_ICC_CAPA);
		(BSP_VOID)BSP_UDI_SetInterfaceTable((UDI_DEVICE_ID)UDI_BUILD_DEV_ID(UDI_DEV_ICC, u32Cnt), pDrvInterface);
	}

	return OK;
}


#ifdef __cplusplus
}
#endif /* __cplusplus */




