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
#ifndef _NANDC_MCO_H_
#define _NANDC_MCO_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define NANDC_TRUE                         1
#define NANDC_FALSE                        0
#define NANDC_NULL                         0

#define NANDC_INVALIED_ECC_ERROR           0x40         
#define NANDC_DMA_ERROR                    0x400

#define NANDC_OK                           0
#define NANDC_ERROR                        1
#define NANDC_E_NOMEM                      2
#define NANDC_E_PARAM                      3
#define NANDC_E_NOFIND                     4
#define NANDC_E_STATUS                     5
#define NANDC_E_TIMEOUT                    6
#define NANDC_E_INIT                       7
#define NANDC_E_READID                     8
#define NANDC_E_NULL                       9
#define NANDC_E_ERASE                      10
#define NANDC_E_ADDR_OVERSTEP              11
#define NANDC_E_READ                       12
#define NANDC_E_ECC                        13
#define NANDC_E_CRC                        14

#define NAND_SPEC_ALREADY_SAVED         0x5A5A5A5A            

/*
 * Tracing flags.
 * The flags masked in NANDC_TRACE_ALL|NANDC_TRACE_NORMAL are always traced.
 */

#define NANDC_TRACE_ASSERT		        0x00000001
#define NANDC_TRACE_OS			        0x00000002
#define NANDC_TRACE_CTRL			    0x00000004
#define NANDC_TRACE_CHIP			    0x00000008
#define NANDC_TRACE_HOST		        0x00000010
#define NANDC_TRACE_MTD		            0x00000020
#define NANDC_TRACE_CONF	            0x00000040
#define NANDC_TRACE_INIT		        0x00000080
#define NANDC_TRACE_NATIVE		        0x00000100
#define NANDC_TRACE_NAND		        0x00000200
#define NANDC_TRACE_PORT                0x00000400
#define NANDC_TRACE_PTABLE              0x00000800
#define NANDC_TRACE_ALL		            0x0FFFFFFF
#define NANDC_TRACE_ERRO			    0x00000000
#define NANDC_TRACE_WARNING		        0x10000000
#define NANDC_TRACE_NORMAL		        0x20000000
#define NANDC_TRACE_LEVEL			    0xF0000000

#define HICHAR_NULL                     ""

#ifdef NANDC_NAND_SIZE_WIDTH64
/*typedef u64                           FSZ;*/
/*flash size*/
#define FSZ                             long long unsigned int
/*flash size wildcard character*/
#define FWC                             "016ll"
#else
/*typedef u32                           FSZ;*/
/*flash size*/
#define FSZ                             long unsigned int
/*flash size wildcard character*/
#define FWC                             "08l"
#endif



#define NANDC_TRACE(mask, p)    \
/*lint -save -e778*/    /*do*/{                                                                                        \
        if (((mask) & nandc_trace_mask & NANDC_TRACE_ALL) &&                                                       \
        ((s32)((mask) &  (NANDC_TRACE_LEVEL)) <= (s32)((nandc_trace_mask) & (NANDC_TRACE_LEVEL)))) \
        {                                                                                           \
            /* coverity[printf_arg_mismatch] */         \
            hiout(p);                                                                           \
        }                                                                                           \
    } /*while(0)*//*lint -restore */


#define NANDC_DO_ASSERT(a, msg, p)    \
/*lint -save -e548 -e506*/ do{                                                                                     \
    if(!(a))                                                                            \
    {                                                                                   \
        if(msg)hiout(("%s",(char*)msg));                                                       \
        hiout((NANDC_NULL_ASSERT_FORMAT ,p, __FILE__, __LINE__));                       \
        hiassert(0);                                                                    \
    }\
/*lint -restore */ }while(0)


#define NANDC_REJECT_NULL(p)    \
 /*lint -save -e548*/ /*do*/{                                                                                \
    if(NANDC_NULL == (void*)(p))                                                           \
    {                                                                                   \
        NANDC_DO_ASSERT(0, NANDC_NULL_PARAM_FORMAT, 0);                                 \
        goto ERRO;                                                                      \
    }                                                                                   \
 /*lint -restore */ }/*while(0)*/

#define NANDC_CHECK_PAGESIZE(pagesize)      ((pagesize==NANDC_SIZE_HK)||(pagesize==NANDC_SIZE_2K)||(pagesize==NANDC_SIZE_4K)\
                                                                                    ||(pagesize==NANDC_SIZE_8K))?pagesize:NANDC_NULL

#define NANDC_CHECK_ERASESIZE(erasesize)     ((erasesize==NANDC_SIZE_HK*NANDC_BLOCK_64PAGE)||(erasesize==NANDC_SIZE_2K*NANDC_BLOCK_64PAGE)||\
                                                                      (erasesize==NANDC_SIZE_4K*NANDC_BLOCK_64PAGE)||(erasesize==NANDC_SIZE_8K*NANDC_BLOCK_128PAGE))?erasesize:NANDC_NULL

#define NANDC_CHECK_BUSWIDE(buswidth)       buswidth

#define NANDC_CHECK_CHIPSIZE(chipsize)      chipsize;

#define NANDC_CHECK_SPARESIZE(sparesize)    sparesize;

#define NANDC_CHECK_ECCTYPE(ecctype)        ecctype;


#define NANDC_SET_REG_BITS(bitfunc, value)     \
  /*do*/{if(bitfunc.bitset)                                                                \
        {                                                                                  \
            if(32 == (bitfunc).bitset->length)                                             \
                NANDC_REG_WRITE32((u32)host->regbase + ((bitfunc).reg_offset), value);    \
            else                                                                           \
                /* coverity[bit_and_with_zero] */                                           \
                NANDC_REG_SETBIT32((u32)host->regbase + ((bitfunc).reg_offset),            \
                                                            (bitfunc).bitset->offset ,      \
                                                            (bitfunc).bitset->length,       \
                                                            value);                         \
        }}/*while(0)*/

#define NANDC_WRITE_REG_BITS(bitfunc, value)     \
    /*do*/{if(bitfunc.bitset)                                                                 \
                    NANDC_REG_WRITEBIT32((u32)host->regbase + ((bitfunc).reg_offset),        \
                                                            (bitfunc).bitset->offset ,        \
                                                            (bitfunc).bitset->length,         \
                                                            value);                           \
        }/*while(0)*/


#define NANDC_GET_REG_BITS(bitfunc)     \
    ((bitfunc.bitset)?                                                                       \
                    NANDC_REG_GETBIT32((u32)host->regbase + ((bitfunc).reg_offset),          \
                                                            (bitfunc).bitset->offset ,        \
                                                            (bitfunc).bitset->length):0 )

__inline static void nandc_byte_cpy(char* dst, const char* src,  u32 size)
{
	while(size)
	{
		*dst++ = *src++;
		size--;
	}
}

 #define ADDR_DATA_SWAP32(a,b) \
    do{\
                        *(u32*)(a) = (*(u32*)(a)) ^ (*(u32*)(b)); \
                        *(u32*)(b) = (*(u32*)(a)) ^ (*(u32*)(b)); \
                        *(u32*)(a) = (*(u32*)(a)) ^ (*(u32*)(b)); \
    }while(0)

#define HI_NOTUSED(p)   ((void)(p)); ((p)=(p))

#define PARTITION_NAME_TEMPLATE "partition00"

#define NANDC_MS_TO_COUNT(ms)       ms

#ifdef __cplusplus
}
#endif

#endif  /*_NANDC_MCO_H_*/



