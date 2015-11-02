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
#include <icc_balong.h>

#ifdef __cplusplus
extern "C"
{
#endif

extern struct icc_init_info g_icc_init_info[];

#define ICC_DEFAULT_SUB_CHANNEL   (0)

#ifndef CONFIG_ICC   /* 打桩 */

s32 bsp_icc_send(u32 cpuid,u32 channel_id,u8 *buffer,u32 data_len)
{
	bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_ICC, "[icc]: <%s> is stub\n", __FUNCTION__);
	return data_len;
}

s32 bsp_icc_send_sync(u32 cpuid,u32 channel_id,u8 * data,u32 data_len,s32 timeout)
{
	bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_ICC, "[icc]: <%s> is stub\n", __FUNCTION__);
	return data_len;	
}

s32 bsp_icc_read(u32 channel_id,u8 * buf,u32 buf_len)
{
	bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_ICC, "[icc]: <%s> is stub\n", __FUNCTION__);
	return buf_len;	
}

s32 bsp_icc_event_register(u32 channel_id, read_cb_func read_cb, void *read_context,
                                              write_cb_func write_cb, void *write_context)
{
	bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_ICC, "[icc]: <%s> is stub\n", __FUNCTION__);
	return ICC_OK;
}

s32 bsp_icc_event_unregister(u32 channel_id)
{
	bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_ICC, "[icc]: <%s> is stub\n", __FUNCTION__);
	return ICC_OK;
}

s32 bsp_icc_init(void)
{
	bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_ICC, "[icc]: <%s> is stub\n", __FUNCTION__);
	return ICC_OK;
}

void bsp_icc_release(void)
{
	bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_ICC, "[icc]: <%s> is stub\n", __FUNCTION__);
	return;
}

s32 bsp_icc_suspend(void)
{
	bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_ICC, "[icc]: <%s> is stub\n", __FUNCTION__);
	return ICC_OK;
}
void icc_dump_hook(void)
{
	bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_ICC, "[icc]: <%s> is stub\n", __FUNCTION__);
	return;
}

BSP_S32 BSP_ICC_Open(BSP_U32 u32ChanId, ICC_CHAN_ATTR_S *pChanAttr)
{
	bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_ICC, "[icc]: <%s> is stub\n", __FUNCTION__);
	return ICC_OK;
}

BSP_S32 BSP_ICC_Close(BSP_U32 u32ChanId)
{
	bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_ICC, "[icc]: <%s> is stub\n", __FUNCTION__);
	return ICC_OK;
}

BSP_S32 BSP_ICC_Write(BSP_U32 u32ChanId, BSP_U8* pData, BSP_S32 s32Size)
{
	bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_ICC, "[icc]: <%s> is stub\n", __FUNCTION__);
	return s32Size;
}

BSP_S32 BSP_ICC_Read(BSP_U32 u32ChanId, BSP_U8* pData, BSP_S32 s32Size)
{
	bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_ICC, "[icc]: <%s> is stub\n", __FUNCTION__);
	return s32Size;
}

BSP_S32 BSP_ICC_Ioctl(BSP_U32 u32ChanId, BSP_U32 cmd, BSP_VOID *param)
{
	bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_ICC, "[icc]: <%s> is stub\n", __FUNCTION__);
	return ICC_OK;
}

BSP_S32 BSP_ICC_Debug_Register(BSP_U32 u32ChanId, FUNCPTR_1 debug_routine, int param)
{
	bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_ICC, "[icc]: <%s> is stub\n", __FUNCTION__);
	return ICC_OK;
}

#elif (defined(__KERNEL__) || defined(__VXWORKS__))  /* CONFIG_ICC */
s32 icc_read_cb_wraper(u32 channel_id , u32 len, void* context);
s32 icc_write_cb_wraper(u32 real_channel_id , void* context);

s32 icc_read_cb_wraper(u32 channel_id , u32 len, void* context)
{
	icc_read_cb cb = (icc_read_cb)context;

	if(cb)
	{
		return (s32)cb(GET_CHN_ID(channel_id),(s32)len);
	}

	return ICC_OK;
}

s32 icc_write_cb_wraper(u32 real_channel_id , void* context)
{
	icc_write_cb cb = (icc_write_cb)context;
	if(cb)
	{
		return (s32)cb(real_channel_id);
	}

	return ICC_OK;
}

BSP_S32 BSP_ICC_Open(BSP_U32 u32ChanId, ICC_CHAN_ATTR_S *pChanAttr)
{
	u32 func_id = ICC_DEFAULT_SUB_CHANNEL; /* 除ifc通道外，其他通道只有一个回调 */
	u32 channel_id = u32ChanId << 16 | func_id;
	u32 i = 0;
	u32 real_channel_size = bsp_icc_channel_size_get();

	/* coverity[REVERSE_INULL] */
	if(!pChanAttr)
	{
		icc_print_error("pChanAttr is null!\n");
		goto out;
	}
	else if(pChanAttr->u32FIFOOutSize != pChanAttr->u32FIFOInSize || u32ChanId >= ICC_CHN_ID_MAX)
	{
		icc_print_error("invalid param u32ChanId[%d],pChanAttr[0x%x],fifo_in[0x%x],fifo_out[0x%x]\n",
			u32ChanId, (s32)pChanAttr, pChanAttr->u32FIFOInSize, pChanAttr->u32FIFOOutSize);
		goto out;
	}

	for(i = 0; i < real_channel_size; i++)
	{
		if(u32ChanId == g_icc_init_info[i].real_channel_id)
		{
			break;
		}
	}

	if(i >= real_channel_size)
	{
		icc_print_error("channel_id[%d] cannot find in g_icc_init_info array\n", u32ChanId);
		goto out;
	}
	else
	{
		if(pChanAttr->u32FIFOInSize > g_icc_init_info[i].fifo_size)/*lint !e574 */
		{
			icc_print_error("user fifo_size(%d) > fifo_size(%d) defined in bsp(g_icc_init_info)\n",
					 pChanAttr->u32FIFOInSize, g_icc_init_info[i].fifo_size);
			goto out;
		}
	}

	return (BSP_S32)bsp_icc_event_register(channel_id, icc_read_cb_wraper, (void*)pChanAttr->read_cb, icc_write_cb_wraper, (void*)pChanAttr->write_cb);

out:
	return BSP_ERR_ICC_INVALID_PARAM;
}

BSP_S32 BSP_ICC_Close(BSP_U32 u32ChanId)
{
	bsp_icc_channel_uninit((u32)u32ChanId);
	return ICC_OK;
}
BSP_S32 BSP_ICC_Write(BSP_U32 u32ChanId, BSP_U8* pData, BSP_S32 s32Size)
{
	u32 channel_id = (u32ChanId << 16 | (u16)ICC_DEFAULT_SUB_CHANNEL);
	
	if(!pData || u32ChanId >= ICC_CHN_ID_MAX)
	{
		icc_print_error("invalid param[%d], pData[0x%x]\n", u32ChanId, (u32)pData);
		return BSP_ERR_ICC_INVALID_PARAM;
	}

	return (BSP_S32)bsp_icc_send(ICC_SEND_CPU, channel_id, (u8*)pData, (u32)s32Size);
}

BSP_S32 BSP_ICC_Read(BSP_U32 u32ChanId, BSP_U8* pData, BSP_S32 s32Size)
{
	u32 channel_id = (u32ChanId << 16 | (u16)ICC_DEFAULT_SUB_CHANNEL);
	
	if(!pData || u32ChanId >= ICC_CHN_ID_MAX)
	{
		icc_print_error("invalid param[%d], pData[0x%x]\n", u32ChanId, (u32)pData);
		return BSP_ERR_ICC_INVALID_PARAM;
	}

	return (BSP_S32)bsp_icc_read(channel_id, (u8*)pData, (u32)s32Size);
}

BSP_S32 BSP_ICC_Ioctl(BSP_U32 u32ChanId, BSP_U32 cmd, BSP_VOID *param)
{
	u32 channel_id = (u32ChanId << 16 | (u16)ICC_DEFAULT_SUB_CHANNEL);/* [false alarm]:屏蔽Fortify错误 */
	s32 ret = ICC_OK;
	
	if(u32ChanId >= ICC_CHN_ID_MAX)
	{
		icc_print_error("invalid param[%d].\n", u32ChanId);
		return BSP_ERR_ICC_INVALID_PARAM;
	}
	switch(cmd)
	{
		case ICC_IOCTL_SET_READ_CB:
		{
			ret = (s32)bsp_icc_event_register(channel_id, icc_read_cb_wraper ,(void*)param, NULL, NULL);
			break;
		}
		case ICC_IOCTL_SET_WRITE_CB:
		{
			ret = (s32)bsp_icc_event_register(channel_id, NULL, NULL, icc_write_cb_wraper ,(void*)param);
			break;
		}
		case ICC_IOCTL_GET_STATE:
		{
			ret = (s32)bsp_icc_channel_status_get((u32)u32ChanId, (u32*)param);
			break;
		}
		case ICC_IOCTL_SET_EVENT_CB:
		{
			break;
		}
		default:
		{
			icc_print_error("channel[%d] invalid cmd[%d].\n", u32ChanId, cmd);
			ret = BSP_ERR_ICC_INVALID_PARAM;
			break;
		}
	}
	
	return ret;
}
BSP_S32 BSP_ICC_Debug_Register(BSP_U32 u32ChanId, FUNCPTR_1 debug_routine, int param)
{
	return bsp_icc_debug_register(u32ChanId,debug_routine,param);
}

#endif /* CONFIG_ICC */

#ifdef __cplusplus
}
#endif
