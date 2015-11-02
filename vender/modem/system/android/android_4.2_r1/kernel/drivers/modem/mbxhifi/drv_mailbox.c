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


/*****************************************************************************
  1 头文件包含
*****************************************************************************/
#include "drv_mailbox.h"
#include "drv_mailbox_cfg.h"
#include "bsp_nvim.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifndef BSP_CONFIG_HI3630
#ifdef CONFIG_HIFI


/*****************************************************************************
  2 函数声明
*****************************************************************************/
unsigned long mailbox_send_msg(
                unsigned long            mailcode,
                void                    *data,
                unsigned long            length);

unsigned long mailbox_reg_msg_cb(
                unsigned long             mailcode,
                mb_msg_cb                 func,
                void                     *data);

unsigned long mailbox_read_msg_data(
                void                   *mail_handle,
                 char                  *buff,
                unsigned long          *size);
/*****************************************************************************
  3 函数实现
*****************************************************************************/

/*****************************************************************************
 函 数 名  : DRV_MAILBOX_SENDMAIL
 功能描述  : 邮箱用户发送邮件数据
 输入参数  : MailCode   -- 邮编, 指定邮件发送目标
             pData      -- 指向邮件数据的指针
             Length     -- 待发送数据长度, 单位byte
 输出参数  : 无
 返 回 值  : 正确:MAILBOX_OK / 错误:MAILBOX_ERRO或者其他错误码
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年9月28日
    作    者   : 莫南 00176101
    修改内容   : 新生成函数

*****************************************************************************/
unsigned long DRV_MAILBOX_SENDMAIL(
                unsigned long           MailCode,
                void                   *pData,
                unsigned long           Length)
{
    return mailbox_send_msg(MailCode, pData, Length);
}

/*****************************************************************************
 函 数 名  : DRV_MAILBOX_REGISTERRECVFUNC
 功能描述  : 注册邮件数据接收函数
 输入参数  : MailCode   -- 数据接收函数处理的对象邮编
             pFun       -- 邮件数据接收处理函数指针
             UserHandle -- 用户定义句柄, 数据接收函数被调用时会作为入参, 无则填空
 输出参数  : 无
 返 回 值  : 正确:MAILBOX_OK / 错误:MAILBOX_ERRO或者其他错误码
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年9月29日
    作    者   : 莫南 00176101
    修改内容   : 新生成函数

*****************************************************************************/
unsigned long DRV_MAILBOX_REGISTERRECVFUNC(
                unsigned long           MailCode,
                mb_msg_cb               pFun,
                void                   *UserHandle)
{
    return mailbox_reg_msg_cb(MailCode, pFun, UserHandle);
}

/*****************************************************************************
 函 数 名  : DRV_MAILBOX_READMAILDATA
 功能描述  : 邮箱用户在数据接收回调函数中调用, 从邮箱中读取一封最先到达的邮件
 输入参数  : MailHandle -- 邮箱句柄, 数据接收回调函数入参
             pData      -- 保存待读出数据的缓存地址
             *pSize     -- 缓存长度, 单位byte, 需要大于待读取邮件长度
 输出参数  : *pSize     -- 实际读取长度, 单位byte
 返 回 值  : 正确:MAILBOX_OK / 错误:MAILBOX_ERRO或者其他错误码
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年10月26日
    作    者   : 莫南 00176101
    修改内容   : 新生成函数

*****************************************************************************/
unsigned long DRV_MAILBOX_READMAILDATA(
                void                   *MailHandle,
                unsigned char          *pData,
                unsigned long          *pSize)
{
    return mailbox_read_msg_data(MailHandle, (char *)pData, pSize);
}

#define HIFI_MBX_OFFSET    0xa0000000
/*****************************************************************************
 函 数 名  : drv_hifi_fill_mb_info
 功能描述  : 邮箱信息设置接口
 输入参数  : addr 段信息地址(由HIFI加载模块传入)
 输出参数  : 
 返 回 值  : void
 调用函数  : V9R1中在fastboot中调用，V7R2中与DSP一起加载调用
 被调函数  :

 修改历史      :
  1.日    期   : 2013年11月4日
    作    者   : 崔军强 64416
    修改内容   : 新生成函数

*****************************************************************************/
void drv_hifi_fill_mb_info(unsigned int addr)
{
    CARM_HIFI_DYN_ADDR_SHARE_STRU *pdata;

    pdata = (CARM_HIFI_DYN_ADDR_SHARE_STRU*)addr;
    
    pdata->stCarmHifiMB.uwHifi2CarmMailBoxLen   = MAILBOX_QUEUE_SIZE(HIFI, CCPU, MSG);
    pdata->stCarmHifiMB.uwCarm2HifiMailBoxLen   = MAILBOX_QUEUE_SIZE(CCPU, HIFI, MSG);
    pdata->stCarmHifiMB.uwHifiCarmHeadAddr      = HIFI_MBX_OFFSET + MAILBOX_HEAD_ADDR(HIFI, CCPU, MSG);
    pdata->stCarmHifiMB.uwHifiCarmBodyAddr      = HIFI_MBX_OFFSET + MAILBOX_QUEUE_ADDR(HIFI, CCPU, MSG);
    pdata->stCarmHifiMB.uwCarmHifiHeadAddr      = HIFI_MBX_OFFSET + MAILBOX_HEAD_ADDR(CCPU, HIFI, MSG);
    pdata->stCarmHifiMB.uwCarmHifiBodyAddr      = HIFI_MBX_OFFSET + MAILBOX_QUEUE_ADDR(CCPU, HIFI, MSG);
    pdata->stCarmHifiMB.uwProtectWord           = HIFI_MB_ADDR_PROTECT;

    pdata->stAarmHifiMB.uwHifi2AarmMailBoxLen   = MAILBOX_QUEUE_SIZE(HIFI, ACPU, MSG);
    pdata->stAarmHifiMB.uwAarm2HifiMailBoxLen   = MAILBOX_QUEUE_SIZE(ACPU, HIFI, MSG);
    pdata->stAarmHifiMB.uwHifiAarmHeadAddr      = HIFI_MBX_OFFSET + MAILBOX_HEAD_ADDR(HIFI, ACPU, MSG);
    pdata->stAarmHifiMB.uwHifiAarmBodyAddr      = HIFI_MBX_OFFSET + MAILBOX_QUEUE_ADDR(HIFI, ACPU, MSG);
    pdata->stAarmHifiMB.uwAarmHifiHeadAddr      = HIFI_MBX_OFFSET + MAILBOX_HEAD_ADDR(ACPU, HIFI, MSG);
    pdata->stAarmHifiMB.uwAarmHifiBodyAddr      = HIFI_MBX_OFFSET + MAILBOX_QUEUE_ADDR(ACPU, HIFI, MSG);
    pdata->stAarmHifiMB.uwProtectWord           = HIFI_MB_ADDR_PROTECT;
    
    pdata->uwNvBaseAddrPhy = HIFI_MBX_OFFSET + SHD_DDR_V2P(NV_GLOBAL_CTRL_INFO_ADDR);
    pdata->uwNvBaseAddrVirt = HIFI_MBX_OFFSET + NV_GLOBAL_CTRL_INFO_ADDR;
    
    pdata->uwProtectWord = HIFI_MB_ADDR_PROTECT;
    
    return ;
}
#else
/*****************************************************************************
 函 数 名  : DRV_MAILBOX_SENDMAIL
 功能描述  : 邮箱用户发送邮件数据
 输入参数  : MailCode   -- 邮编, 指定邮件发送目标
             pData      -- 指向邮件数据的指针
             Length     -- 待发送数据长度, 单位byte
 输出参数  : 无
 返 回 值  : 正确:MAILBOX_OK / 错误:MAILBOX_ERRO或者其他错误码
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年9月28日
    作    者   : 莫南 00176101
    修改内容   : 新生成函数

*****************************************************************************/
unsigned long DRV_MAILBOX_SENDMAIL(
                unsigned long           MailCode,
                void                   *pData,
                unsigned long           Length)
{
    return MAILBOX_OK;
}

/*****************************************************************************
 函 数 名  : DRV_MAILBOX_REGISTERRECVFUNC
 功能描述  : 注册邮件数据接收函数
 输入参数  : MailCode   -- 数据接收函数处理的对象邮编
             pFun       -- 邮件数据接收处理函数指针
             UserHandle -- 用户定义句柄, 数据接收函数被调用时会作为入参, 无则填空
 输出参数  : 无
 返 回 值  : 正确:MAILBOX_OK / 错误:MAILBOX_ERRO或者其他错误码
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年9月29日
    作    者   : 莫南 00176101
    修改内容   : 新生成函数

*****************************************************************************/
unsigned long DRV_MAILBOX_REGISTERRECVFUNC(
                unsigned long           MailCode,
                mb_msg_cb               pFun,
                void                   *UserHandle)
{
    return MAILBOX_OK;
}

/*****************************************************************************
 函 数 名  : DRV_MAILBOX_READMAILDATA
 功能描述  : 邮箱用户在数据接收回调函数中调用, 从邮箱中读取一封最先到达的邮件
 输入参数  : MailHandle -- 邮箱句柄, 数据接收回调函数入参
             pData      -- 保存待读出数据的缓存地址
             *pSize     -- 缓存长度, 单位byte, 需要大于待读取邮件长度
 输出参数  : *pSize     -- 实际读取长度, 单位byte
 返 回 值  : 正确:MAILBOX_OK / 错误:MAILBOX_ERRO或者其他错误码
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年10月26日
    作    者   : 莫南 00176101
    修改内容   : 新生成函数

*****************************************************************************/
unsigned long DRV_MAILBOX_READMAILDATA(
                void                   *MailHandle,
                unsigned char          *pData,
                unsigned long          *pSize)
{
    *pSize = 0;
    return MAILBOX_OK;
}
/*****************************************************************************
 函 数 名  : drv_hifi_fill_mb_info
 功能描述  : 邮箱信息设置接口
 输入参数  : addr 段信息地址(由HIFI加载模块传入)
 输出参数  : 
 返 回 值  : void
 调用函数  : V9R1中在fastboot中调用，V7R2中与DSP一起加载调用
 被调函数  :

 修改历史      :
  1.日    期   : 2013年11月4日
    作    者   : 崔军强 64416
    修改内容   : 新生成函数

*****************************************************************************/
void drv_hifi_fill_mb_info(unsigned int addr)
{
    return ;
}
#endif
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

