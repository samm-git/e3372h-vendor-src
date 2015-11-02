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
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <drv_comm.h>
#include <soc_memmap.h>
#include <osl_bio.h>
#include <hi_cipher.h>
#include <bsp_memmap.h>
#include "kdf.h"

extern int kdf_hash_make(void * sha_s_addr, u32 sha_s_len);
extern int kdf_init(void);
extern void kdf_reg_init(void);

s32 BSP_KDF_003_001(void);
s32 BSP_KDF_005_001(void);
int KDF_TEST_LHK(void);


#define KEY_TOTAL_SIZE        (32*16)
#define KDF_SHA_S_LENGTH      0x7
#define KDF_DESTKEY_OFF       0x10
#define KDF_DESTKEY_LENGTH    0x10
#define KDF_SHA_KEY_LENGTH    0x20

static u8 u32GoldenKey[KDF_SHA_KEY_LENGTH] = 
{
    0xbd, 0xef, 0x4e, 0xd7, 0x1a, 0x1d, 0x3a, 0x15, 
    0x00, 0x90, 0x4d, 0x83, 0xa5, 0xe8, 0x32, 0xf7, 
    0xe3, 0xe0, 0xfb, 0x1c, 0x43, 0x07, 0xcd, 0x94, 
    0xc6, 0xe6, 0x2d, 0x6a, 0x63, 0x7b, 0x76, 0x41 
};

static u8 u32GoldenS1[KDF_SHA_S_LENGTH] = 
{
    0x15,0x05,0x00,0x01,0x02,0x00,0x01
};
#if 0
static u8 u32GoldenS2[KDF_SHA_S_LENGTH] = 
{
    0x15,0x04,0x00,0x01,0x02,0x00,0x01
};

static u8 u32GoldenS3[KDF_SHA_S_LENGTH] = 
{
    0x15,0x03,0x00,0x01,0x02,0x00,0x01
};
#endif
static u8 u32GoldenResult1[KDF_DESTKEY_LENGTH] = 
{
    0xf8,0xc0,0x2a,0x2f,0x03,0x11,0x02,0x2f, 
    0x8d,0x37,0xec,0xe9,0xf2,0x0c,0x07,0x11
};

BSP_S32 cipher_memcmp_ext(const void* mem1,const void* mem2,int size)
{
	BSP_S32 i = 0;

	for(i = 0; i < size; i++)
	{
		if(*((char*)mem1+i) != *((char*)mem2+i))
		{
			//printk("diff_pos:%d,addr1(0x%x):%x,addr2(0x%x):%x\n",i,(int)((char*)mem1+i),(int)*((char*)mem1+i),(int)((char*)mem2+i),(int)*((char*)mem2+i),0);
			return BSP_ERROR;
		}
	}
	return BSP_OK;
}
#if 0
static inline void set_key_test(u8 * key_addr, u32 key_len, u32 index)
{
	if(index > 15)
		return;
	memcpy((void *)(HI_CIPHER_BASE_ADDR_VIRT + HI_KEY_RAM_OFFSET + index * KDF_SHA_KEY_LENGTH), key_addr, key_len);
}
#endif

s32 BSP_KDF_003_001(void)
{
    s32  s32Ret = BSP_OK;
    void *pDestAddr;
	s32 i = 0;
	u32 result_hash[KDF_DESTKEY_LENGTH / sizeof(u32)] = {0};

	if(kdf_init())
	{
		return -1;
	}
	kdf_reg_init();

    /*把key写入0号索引位置*/
    for(i=0; i<KDF_SHA_KEY_LENGTH/4; i++)
    {        
        *((BSP_U32 *)(HI_CIPHER_BASE_ADDR_VIRT + HI_KEY_RAM_OFFSET + 4*i))  \
        = (((BSP_U32)u32GoldenKey[4*i])<<24) | (((BSP_U32)u32GoldenKey[4*i+1])<<16) \
          | (((BSP_U32)u32GoldenKey[4*i+2])<<8) | ((BSP_U32)u32GoldenKey[4*i+3]);
    }

	if(kdf_hash_make(u32GoldenS1, KDF_SHA_S_LENGTH))
		return -2;
	pDestAddr = (void *)(HI_CIPHER_BASE_ADDR_VIRT + HI_KEY_RAM_OFFSET + KDF_DESTKEY_OFF);
	for(i = 0; i < 256 / 32; i++)
	{
		printk("%X\n", *((u32*)pDestAddr + i));
	}
	for(i = 0; i < KDF_DESTKEY_LENGTH / sizeof(u32); i++)
		result_hash[i] = CIPHER_BSWAP32(*((u32 *)pDestAddr + i));
	s32Ret = cipher_memcmp_ext((void *)result_hash, (void *)u32GoldenResult1, KDF_DESTKEY_LENGTH);
	return s32Ret;
}

s32 BSP_KDF_005_001(void)
{
    s32 i;
    s32 s32Ret;
    
    for(i=0; i < 100; i++)
    {
        s32Ret = BSP_KDF_003_001();

        if(BSP_OK != s32Ret)
        {
            return s32Ret;
        }
    }
	return 0;
 }

extern int kdf_hash_setkey(void * key_addr);
extern int kdf_result_hash_get(void * out_hash);
#if 0
int KDF_TEST_LHK(void)
{
	s32 i = 0;
	u32 hashrelt[8] = {0,0,0,0,0,0,0,0};
	u32 src_data[1024] = {0};
	u32 pDestAddr = (HI_CIPHER_BASE_ADDR_VIRT + HI_KEY_RAM_OFFSET);
	u8 right_hash[32] = {0x65,0xb9,0x30,0x04,0x64,0x47,0x0a,0x9a,0xe4,0xc2,0x30,0xf9,0xd0,0xf3,0xe4,0x03,0x60,0xc7,0x56,0x20,0xf1,0x9f,0x89,0x64,0xf5,0x29,0xc7,0x78,0x2e,0xc8,0x88,0xc8};
	
	printk("%X\n\n",HI_CIPHER_BASE_ADDR_VIRT);

	memset(src_data, 0, sizeof(src_data));
	for(i = 0; i < 1024; i++)
		src_data[i] = i;
#if 1
	if(kdf_hash_setkey((void*)hashrelt))
		return -1;
	for(i = 0; i < 256 / 32; i++)
	{
		printk("%X\n", *((u32*)(pDestAddr + i * 4)));
	}
	printk("\n\n");
#endif

#if 0
	memset((void*)pDestAddr, 0, 32);
	for(i = 0; i < 256 / 32; i++)
	{
		printk("%X\n", *((u32*)pDestAddr + i));
	}
	printk("\n\n");
#endif
	
	kdf_hash_make(src_data, 512);
	kdf_result_hash_get(hashrelt);
	for(i = 0; i < 8; i++)
		printk("%X\n", *(hashrelt + i));

	kdf_hash_setkey(hashrelt);
	kdf_hash_make(((u8*)src_data) + 512, 512);
	kdf_result_hash_get(hashrelt);
	for(i = 0; i < 8; i++)
		printk("%X\n", *(hashrelt + i));

	kdf_hash_setkey(hashrelt);
	kdf_hash_make(((u8*)src_data) + 1024, 512);
	kdf_result_hash_get(hashrelt);
	for(i = 0; i < 8; i++)
		printk("%X\n", *(hashrelt + i));

	kdf_hash_setkey(hashrelt);
	kdf_hash_make(((u8*)src_data) + 512 + 1024, 512);
	kdf_result_hash_get(hashrelt);
	for(i = 0; i < 8; i++)
		printk("%X\n", *(hashrelt + i));
#if 0
	kdf_hash_setkey(hashrelt);
	kdf_hash_make(((u8*)src_data) + 1024 + 1024, 512);
	kdf_result_hash_get(hashrelt);
	for(i = 0; i < 8; i++)
		printk("%X\n", *(hashrelt + i));

	kdf_hash_setkey(hashrelt);
	kdf_hash_make(((u8*)src_data) + 512 + 2 * 1024, 512);
	kdf_result_hash_get(hashrelt);
	for(i = 0; i < 8; i++)
		printk("%X\n", *(hashrelt + i));

	kdf_hash_setkey(hashrelt);
	kdf_hash_make(((u8*)src_data) + 3 * 1024, 512);
	kdf_result_hash_get(hashrelt);
	for(i = 0; i < 8; i++)
		printk("%X\n", *(hashrelt + i));

	kdf_hash_setkey(hashrelt);
	kdf_hash_make(((u8*)src_data) + 512 + 3 * 1024, 512);
	kdf_result_hash_get(hashrelt);
	for(i = 0; i < 8; i++)
		printk("%X\n", *(hashrelt + i));
#endif
	for(i = 0; i < 8; i++)
		hashrelt[i] = CIPHER_BSWAP32(hashrelt[i]);
	if(cipher_memcmp_ext(hashrelt,right_hash,32))
		printk("KDF_TEST_LHK hash ERR\n");
	
	return 0;
}
#endif
