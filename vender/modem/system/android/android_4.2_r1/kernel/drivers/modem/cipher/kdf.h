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
#ifndef _CIPHER_DRV_SHAONLY_H
#define _CIPHER_DRV_SHAONLY_H

#include <osl_types.h>
#include <soc_memmap.h>
#include <bsp_memmap.h>
#include <hi_cipher.h>

#ifdef __cplusplus
extern "C" {
#endif
 
/* 错误码 */
#define CIPHER_SUCCESS   0


#define CHN_IV_SEL                 0x0
#define CHN_RDQ_CTRL               0x1
#define CHN_RDQCTRL_BIT            0x2
#define CHN_USRFIELDLEN_BIT        0x5

#define CIPHER_KEY_NUM             0x10
#define CIPHER_KEY_LEN             0x20
#define CIPHER_TIMEOUT_TICKS       100
#define CIPHER_WAIT_THREHLD        20

#define CIPHER_BSWAP32(val) \
        (((val) >> 24) | (((val) >> 8) & 0x0000ff00) | \
        (((val) << 8) & 0x00ff0000) | ((val) << 24))

#define KDF_CHN_NUM            0x4
#define KDF_CHN_USRFIELD_LEN   0x0
#define KDF_SHA_KEY_LENGTH     0x20
#define KDF_SHA_S_LENGTH_MAX   0x200

#define CIPHER_ALIGN_LEN  7

typedef enum tagKDF_SHA_KEY_SOURCE_E
{
    SHA_KEY_SOURCE_DDR,             /*sha_key来源于input point指定的地址空间*/
    SHA_KEY_SOURCE_KEYRAM,          /*sha_key来源于内部KeyRam*/
    SHA_KEY_SOURCE_FOLLOW,          /*sha_key来源于上次KDF操作的sha_key*/
    SHA_KEY_SOURCE_RESULT,          /*sha_key来源于上一次KDF操作的结果*/
    SHA_KEY_SOURCE_MAX
}KDF_SHA_KEY_SOURCE_E;

typedef enum tagKDF_SHA_S_SOURCE_E
{
    SHA_S_SOURCE_DDR,               /*sha_s来源于input point指定的地址空间*/
    SHA_S_SOURCE_KEYRAM,            /*sha_s来源于内部KeyRam*/
    SHA_S_SOURCE_MAX
}KDF_SHA_S_SOURCE_E;

/* CIPHER通知类型 */
typedef enum tagCIPHER_NOTIFY_STAT_E
{
    CIPHER_STAT_OK = 0,           /* 成功完成 */
    CIPHER_STAT_CHECK_ERR = 1,    /* 完整性检查错误 */
    CIPHER_STAT_BUTTOM
}CIPHER_NOTIFY_STAT_E;

/* KDF使用的BD结构*/
typedef struct tagKDF_BD_SCPT_S
{
    u32 u32KdfConfig;
    u32 u32Address;
}KDF_BD_SCPT_S;

/* KDF使用的RD结构*/
typedef struct tagKDF_RD_SCPT_S
{
    u32 u32Address;
    u32 u32KdfConfig;
}KDF_RD_SCPT_S;

typedef struct tagKDF_BDCONFIG_INFO_S
{
    KDF_SHA_KEY_SOURCE_E enShaKeySource;    /*sha_key来源*/
    KDF_SHA_S_SOURCE_E   enShaSSource;      /*sha_s来源*/
    u32                  u32ShaKeyIndex;    /*sha_key来源于KeyRam时，其在keyRam中的索引*/
    u32                  u32ShaSIndex;      /*sha_s来源于KeyRam时，其在keyRam中的索引*/
    u32                  u32Length;         /*KDF运算作为sha_s的长度*/
    u32                  u32DestIndex;      /*sha_result_sn在key_ram中的位置*/
    void *               pAddress;          /*目的(源)地址*/
}KDF_BDCONFIG_INFO_S;

#define KEY_INDEX    0
#define BD_RD_NUM    2
#define	BDQ_SIZE	 (sizeof(KDF_BD_SCPT_S)* BD_RD_NUM)
#define	RDQ_SIZE	 (sizeof(KDF_RD_SCPT_S)* BD_RD_NUM)

struct kdf_s
{
	u32 bdq_virt_addr;
	u32 bdq_real_addr;
	u32 rdq_virt_addr;
	u32 rdq_real_addr;
	u32 brdq_length;
	u32 sha_s_buff_real_addr;
	u32 sha_s_buff_virt_addr;
	u32 buffer_virt_addr;
	u32 buffer_real_addr;
	struct clk * cifer_clk;
};
 
/**************************************************************************
  宏定义
**************************************************************************/
/* 注意CIPHER错误码要为负值 */
#define CIPHER_ERROR_BASE               0x10004000

typedef enum tagCIPHER_ERR_CODE_E
{
    CIPHER_ERR_CODE_NULL_PTR = 1,
    CIPHER_ERR_CODE_NO_MEM ,
    CIPHER_ERR_CODE_NOT_INIT ,
    CIPHER_ERR_CODE_FIFO_FULL ,
    CIPHER_ERR_CODE_INVALID_CHN ,
    CIPHER_ERR_CODE_INVALID_OPT ,
    CIPHER_ERR_CODE_ALIGN_ERROR ,
    CIPHER_ERR_CODE_PURGING ,
    CIPHER_ERR_CODE_TIME_OUT,
    CIPHER_ERR_CODE_INVALID_ENUM,
    CIPHER_ERR_CODE_INVALID_RD,
    CIPHER_ERR_CODE_RD_NULL,
    CIPHER_ERR_CODE_INVALID_KEY,
    CIPHER_ERR_CODE_CHECK_ERROR,
    CIPHER_ERR_CODE_BDLEN_ERROR,
    CIPHER_ERR_CODE_INVALID_NUM,
    CIPHER_ERR_CODE_NO_KEY,
    CIPHER_ERR_CODE_KEYLEN_ERROR
}CIPHER_ERR_CODE_E;

#define CIPHER_NULL_PTR      ((s32)(CIPHER_ERROR_BASE|CIPHER_ERR_CODE_NULL_PTR))
#define CIPHER_NO_MEM        ((s32)(CIPHER_ERROR_BASE|CIPHER_ERR_CODE_NO_MEM))
#define CIPHER_NOT_INIT      ((s32)(CIPHER_ERROR_BASE|CIPHER_ERR_CODE_NOT_INIT))
#define CIPHER_FIFO_FULL     ((s32)(CIPHER_ERROR_BASE|CIPHER_ERR_CODE_FIFO_FULL))
#define CIPHER_INVALID_CHN   ((s32)(CIPHER_ERROR_BASE|CIPHER_ERR_CODE_INVALID_CHN))
#define CIPHER_INVALID_OPT   ((s32)(CIPHER_ERROR_BASE|CIPHER_ERR_CODE_INVALID_OPT))
#define CIPHER_ALIGN_ERROR   ((s32)(CIPHER_ERROR_BASE|CIPHER_ERR_CODE_ALIGN_ERROR))
#define CIPHER_PURGING       ((s32)(CIPHER_ERROR_BASE|CIPHER_ERR_CODE_PURGING))
#define CIPHER_TIME_OUT      ((s32)(CIPHER_ERROR_BASE|CIPHER_ERR_CODE_TIME_OUT))
#define CIPHER_INVALID_ENUM  ((s32)(CIPHER_ERROR_BASE|CIPHER_ERR_CODE_INVALID_ENUM))
#define CIPHER_INVALID_RD    ((s32)(CIPHER_ERROR_BASE|CIPHER_ERR_CODE_INVALID_RD))
#define CIPHER_RDQ_NULL      ((s32)(CIPHER_ERROR_BASE|CIPHER_ERR_CODE_RD_NULL))
#define CIPHER_INVALID_KEY   ((s32)(CIPHER_ERROR_BASE|CIPHER_ERR_CODE_INVALID_KEY))
#define CIPHER_CHECK_ERROR   ((s32)(CIPHER_ERROR_BASE|CIPHER_ERR_CODE_CHECK_ERROR))
#define CIPHER_BDLEN_ERROR   ((s32)(CIPHER_ERROR_BASE|CIPHER_ERR_CODE_BDLEN_ERROR))
#define CIPHER_INVALID_NUM   ((s32)(CIPHER_ERROR_BASE|CIPHER_ERR_CODE_INVALID_NUM))
#define CIPHER_NO_KEY        ((s32)(CIPHER_ERROR_BASE|CIPHER_ERR_CODE_NO_KEY))
#define CIPHER_KEYLEN_ERROR  ((s32)(CIPHER_ERROR_BASE|CIPHER_ERR_CODE_KEYLEN_ERROR))
#define CIPHER_UNKNOWN_ERROR ((s32)(CIPHER_ERROR_BASE|0xff))

#ifdef __cplusplus
}
#endif

#endif
