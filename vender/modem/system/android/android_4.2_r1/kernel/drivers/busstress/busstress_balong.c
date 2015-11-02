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
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/semaphore.h>
#include <linux/string.h>
#include "DrvInterface.h"
#include <linux/semaphore.h>
#include <linux/kthread.h>
#include <linux/jiffies.h>
#include "bsp_sram.h"

#define SENCE_NUM (4)
#define MODULE_NUM (20)

char* g_testSenceName[SENCE_NUM] = {"S1","S2","S3","S4"};
int   module_test_switch_stat[MODULE_NUM] = {0};

static unsigned long next = 1;
static unsigned __alloc_next = SRAM_BUSSTRESS_ADDR;

void *alloc(signed sz)
{
    void *ptr;
    
    ptr = (void*) __alloc_next;
    __alloc_next = (__alloc_next + sz + 31) & (~31);

    return ptr;
}

/* RAND_MAX assumed to be 32767 */
static int myrand(void) {
   next = next * 1103515245 + 12345;
   return((unsigned)(next/65536) % 32768);
}

static void mysrand(unsigned seed) {
   next = seed;
}

int memcpyTestProcess(unsigned int uiSrc,unsigned int uiDst,int iSize)
{
	int iRet = 0;

	mysrand(uiSrc);
	memset((void*)uiSrc,myrand(),iSize);
	memset((void*)uiDst,0,iSize);
	memcpy((void*)uiDst,(void*)uiSrc,iSize);
	iRet = memcmp((void*)uiSrc,(void*)uiDst,iSize);
	if(0 != iRet)
	{
		printk("memcmp fail,iRet:0x%x\n",iRet);
		return iRet;
	}

	return 0;
}


int get_test_switch_stat(int module_id)
{
	return module_test_switch_stat[module_id];
}

int set_test_switch_stat(int module_id, int run_or_stop)
{
	module_test_switch_stat[module_id] = run_or_stop;
	return 0;
}
