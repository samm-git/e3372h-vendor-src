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
#ifndef _SECURE_IMAGE_H_
#define _SECURE_IMAGE_H_

#ifdef __cplusplus /* __cplusplus */
extern "C"
{
#endif /* __cplusplus */

#define EMMC_BLOCK_SIZE         512


#define PTN_VRL_P_NAME			    "block2mtd: /dev/block/mmcblk0p1"
#define PTN_VRL_M_NAME			    "block2mtd: /dev/block/mmcblk0p2"
#define PTN_MODEM_NAME			    "block2mtd: /dev/block/mmcblk0p26"
#define PTN_MODEM_DSP_NAME			"block2mtd: /dev/block/mmcblk0p27"

#define CCORE_IMAGE_NAME            "VXWORKS"
#define DSP_IMAGE_NAME              "DSP"

#define SECBOOT_MODEM_VRL_OFFSET    27

#define SECBOOT_PTN_VRL_P_START		(128 * 2)
#define SECBOOT_PTN_VRL_M_START		(256 * 2)
#define SECBOOT_PTN_VRLP_SIZE		(128 * 2)
#define SECBOOT_PTN_VRLM_SIZE		(128 * 2)

#define SECBOOT_PTN_MODEM_START		(212*1024*2)
#define SECBOOT_PTN_MODEM_SIZE		(60*1024*2)

#define MODEM_TDS_SIZE		  		(0x40000)		/* 0.25 MB*/
#define MODEM_DSP_SIZE				(0x180000)		/* 1.5 MB*/
	
/*primary vrl + secondary vrl = 4k*/
#define SECBOOT_VRL_SIZE		(0x1000)
#define SECBOOT_VRL_MAXNUM	    32

#define sec_print_err(fmt, ...)   (printk(KERN_ERR "[sec]: <%s> "fmt, __FUNCTION__, ##__VA_ARGS__))

#define SEC_OK      			(0)
#define SEC_ERROR  				(-1)

struct modem_head
{
	unsigned int image_length;
    unsigned int image_addr;
};

s32 load_modem_image(void);
	
#ifdef __cplusplus /* __cplusplus */
}
#endif /* __cplusplus */

#endif /* _SECURE_IMAGE_H_ */

