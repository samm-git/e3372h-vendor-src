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
#include "drv_mailbox_debug.h"
#include "drv_mailbox_gut.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifdef CONFIG_HIFI
/*lint --e{713, 732, 737, 569, 830}*/
/*****************************************************************************
    可维可测信息中包含的C文件编号宏定义
*****************************************************************************/
#undef  _MAILBOX_FILE_
#define _MAILBOX_FILE_   "msg"
/*****************************************************************************
  2 全局变量定义
*****************************************************************************/
/*****************************************************************************
 函 数 名  : mailbox_msg_receiver
 功能描述  : 核间邮件数据接收回调函数的注册
 输入参数  : void *mb_buf     -- 邮箱物理通道句柄。
             void *handle     -- 用户句柄.
             void *data       -- 用户数据.
 输出参数  : 无
 返 回 值  : MAILBOX_OK, 异常返回值
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年9月29日
    作    者   : 莫南 00176101
    修改内容   : 新生成函数

*****************************************************************************/

void mailbox_msg_receiver(void *mb_buf, void *handle, void *data)
{
    struct mb_queue * queue;  /*邮箱buffer临时句柄，用于传给用户回调*/
    struct mb_buff  * mbuf = ( struct mb_buff  *)mb_buf;
    mb_msg_cb  func = (mb_msg_cb)handle;

    queue = &mbuf->usr_queue;
    if (func) {
        func(data, (void *)queue, queue->size);
    } else {
        mailbox_logerro_p1(MAILBOX_ERR_GUT_READ_CALLBACK_NOT_FIND, mbuf->mailcode);
    }
}

MAILBOX_EXTERN unsigned long mailbox_reg_msg_cb(
                unsigned long             mailcode,
                mb_msg_cb                 func,
                void                     *data)
{
    /*[false alarm]:误报*/
    return mailbox_register_cb(mailcode, mailbox_msg_receiver, func, data);
}

/*****************************************************************************
 函 数 名  : mailbox_try_send_msg
 功能描述  : 进行一次邮件发送动作
 输入参数  :
             unsigned long      mailcode        -- 邮箱逻辑通道号
             unsigned char      *pdata           -- 指向邮件数据的指针。
             unsigned long      length/byte     -- 数据或者消息总长度 字节为单位。
 输出参数  : 无
 返 回 值  : unsigned long
             成功OK, 异常返回值
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年9月28日
    作    者   : 莫南 00176101
    修改内容   : 新生成函数

*****************************************************************************/
MAILBOX_EXTERN unsigned long mailbox_try_send_msg(
                unsigned long            mailcode,
                void                    *pdata,
                unsigned long            length)
{
    struct mb_buff      *mb_buf = MAILBOX_NULL;
    struct mb_queue     *queue  = MAILBOX_NULL;
    unsigned long        ret_val    = MAILBOX_OK;

   if ((0 == pdata) || (0 == length)) {
        /*[false alarm]:误报*/
        return mailbox_logerro_p1(MAILBOX_ERRO, mailcode);
   }
    /*获取邮箱buffer*/
    ret_val = (unsigned long)mailbox_request_buff(mailcode, &mb_buf);
    if (MAILBOX_OK != ret_val) {
        goto exit_out;
    }

    /*填充用户数据*/
    queue = &mb_buf->usr_queue;
    if ( length != (unsigned long)mailbox_write_buff( queue, pdata, length)) {
         ret_val = mailbox_logerro_p1(MAILBOX_FULL, mailcode);
         goto exit_out;
    }

    /*封信*/
    ret_val = mailbox_sealup_buff( mb_buf,  length);
    if (MAILBOX_OK == ret_val) {
         /*发送邮件*/
        ret_val = mailbox_send_buff(mb_buf);
    }

exit_out:
    /*释放邮箱buffer*/
    if (MAILBOX_NULL != mb_buf) {
        mailbox_release_buff(mb_buf);
    }

    return ret_val;
}


/****************************************************************************
 函 数 名  : mailbox_read_msg_data
 功能描述  : 邮箱用户在数据接收回调函数中调用, 从邮箱中读取一封最先到达的邮件
 输入参数  : mail_handle -- 邮箱句柄, 数据接收回调函数入参
             buff      -- 保存待读出数据的缓存地址
             *size     -- 缓存长度, 单位byte, 需要大于待读取邮件长度
 输出参数  : *size     -- 实际读取长度, 单位byte
 返 回 值  : MAILBOX_OK / MAILBOX_ERRO
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年10月26日
    作    者   : 莫南 00176101
    修改内容   : 新生成函数

*****************************************************************************/
MAILBOX_GLOBAL unsigned long mailbox_read_msg_data(
                void                   *mail_handle,
                char                   *buff,
                unsigned long          *size)
{
    struct mb_queue *pMailQueue = (struct mb_queue *)mail_handle;

    if ((MAILBOX_NULL == pMailQueue) || (MAILBOX_NULL == buff) || (MAILBOX_NULL == size)) {
        /*[false alarm]:误报*/
        return mailbox_logerro_p1(MAILBOX_ERR_GUT_INPUT_PARAMETER, 0);
    }

    if (pMailQueue->size  >  *size) {
        /*[false alarm]:误报*/
        return mailbox_logerro_p1(MAILBOX_ERR_GUT_USER_BUFFER_SIZE_TOO_SMALL, *size);
    }

    /*检查用户传回的邮箱数据队列句柄的有效性*/
    if ((0 == pMailQueue->length) ||
        ((pMailQueue->front - pMailQueue->base) >  pMailQueue->length ) ||
        ((pMailQueue->rear - pMailQueue->base) >  pMailQueue->length )) {
        /*[false alarm]:误报*/
        return mailbox_logerro_p1(MAILBOX_CRIT_GUT_INVALID_USER_MAIL_HANDLE, pMailQueue);
    }

    *size =  mailbox_read_buff(pMailQueue, buff, pMailQueue->size);

    return MAILBOX_OK;
}

/*****************************************************************************
 函 数 名  : mailbox_send_msg
 功能描述  : 提供给各模块进行核间数据发送的接口。
 输入参数  :
             unsigned long       mailcode      -- 邮件的id号, 逻辑通道号。
             unsigned char      *data           -- 指向邮件数据的指针。
             unsigned long       length/byte     -- 数据或者消息总长度 字节为单位。
 输出参数  : 无
 返 回 值  : unsigned long
             成功OK, 异常返回值
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年9月28日
    作    者   : 莫南 00176101
    修改内容   : 新生成函数

*****************************************************************************/
MAILBOX_EXTERN unsigned long mailbox_send_msg(
                unsigned long            mailcode,
                void                    *data,
                unsigned long            length)
{
    unsigned long  ret_val;
    unsigned long  try_go_on = MAILBOX_TRUE;
    unsigned long  try_times = 0;

/* Modified by c64416 for hifi mailbox, 2013/09/24, begin */
#if 0
    ret_val= BSP_CPU_StateGet(mailbox_get_dst_id(mailcode));
    if(!ret_val)
    {
        return MAILBOX_TARGET_NOT_READY;
    }
#else
    if(mailbox_get_dst_id(mailcode) != MAILBOX_CPUID(HIFI))
    {
        return MAILBOX_TARGET_NOT_READY;
    }
#endif
/* Modified by c64416 for hifi mailbox, 2013/09/24, end */

    ret_val = mailbox_try_send_msg(mailcode, data, length);

    if (MAILBOX_FALSE == mailbox_int_context()) {
        /*发送满等待轮询尝试*/
        while (MAILBOX_FULL == ret_val) {
            mailbox_delivery(mailbox_get_channel_id(mailcode));
            try_go_on = mailbox_scene_delay(MAILBOX_DELAY_SCENE_MSG_FULL, &try_times);

            if (MAILBOX_TRUE == try_go_on) {
                ret_val = mailbox_try_send_msg(mailcode, data, length);
            } else {
                break;
            }
        }
    }

    if (MAILBOX_OK != ret_val) {
        /*mailbox_show(mailcode,0);*/
        /*mailbox_assert(ret_val);*/
        if (MAILBOX_FULL != ret_val) {
            ret_val = MAILBOX_ERRO;
        }
        return ret_val;
    }

    return ret_val;
}

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

