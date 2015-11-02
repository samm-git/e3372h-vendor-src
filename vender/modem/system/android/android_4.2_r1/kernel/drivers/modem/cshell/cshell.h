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
#ifndef    _CSHELL_H_
#define    _CSHELL_H_

/**************************************************************************
                                              头文件定义
**************************************************************************/

#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/semaphore.h>


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/**************************************************************************
                                               宏定义
**************************************************************************/
#define ACM_CHN_CSHELL                  2
#define CSHELL_BUFFER_SIZE              8192
#define CSHELL_ICC_CHANNEL_FIFO_SIZE    8192
#define CSHELL_ICC_CHANNEL_ID           31
#define CSHELL_ICC_CHANNEL_INVALID_ID   0xFFFF
#define CSHELL_SEND_BUFFER_NUM          8
#define CSHELL_OK                       0
#define CSHELL_ERROR                    (-1)
#define CHAR_BIT_NUM                    8
#define RECV_CMD_STR_LEN                256

enum usb_filenode_open_list
{
	USB_CSHELL,
	FILE_CSHELL,
	AUART_CSHELL,
	DIAL_CSHELL,
	ALL_CSHELL,
	INVAL_CSHELL
};

/**************************************************************************
                                                    STRUCT定义
**************************************************************************/
typedef struct cshell_mem_handle
{
    u8  *   buf;
    u32     buf_size;
    u32     buf_valid;
    u32     index;
    struct  cshell_mem_handle *next;
}cshell_mem_handle_t;

typedef struct
{
    u32                 icc_chan_opened;
    u32                 icc_channel_id ;
    u32                 acm_channel_id;
    s32                 cshell_acm_fd;
    u32                 valid;
    cshell_mem_handle_t recv_mem;
    cshell_mem_handle_t send_mem[CSHELL_SEND_BUFFER_NUM];
    struct semaphore    cshell_send_sem;
    struct semaphore    cshell_recv_sem;
    u32                 cshell_send_index;
    u8 *                recv_buf;
    u8 *                send_buf;
    struct task_struct *send_task;  //A->PC
    struct task_struct *recv_task;  //A->ICC

    u32                 reset_infor_fail_times;
    u8                  acshell_permit;        /* 0:not permit to send log to acore; others, permit*/
    spinlock_t          cshell_spin_loc_permit;/* lock to pretect  acshell_permit*/
    u8                  ccshell_work_flg;      /* 1:ccore working; 0:ccore stop */
    u8                  recv_cmd_str[RECV_CMD_STR_LEN];

    u8*                 usb_send_buf;
    u32                 usb_send_buf_size;
    u32                 usb_send_buf_r;
    u32                 usb_send_buf_w;
    struct semaphore    cshell_usb_send_sem;
    struct task_struct *send_task_usb;

}cshell_ctx_t;

struct cshell_debug_log
{
	u32 usb_buf_drop_times;
	u32 send_mem_drop_times;
	u32 auart_send_cmd_times;
	u32 bluetooth_send_cmd_times;
};

/**************************************************************************
                                                    函数声明
**************************************************************************/
int cshell_set_bit(int num_from_zero);
int cshell_clear_bit(int num_from_zero);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _CSHELL_H_ */

