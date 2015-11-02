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
#include <bsp_wdt.h>
#include <hi_wdt.h>
#include <linux/types.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <osl_spinlock.h>

s32 hi6930_wdt_case01(void);
s32 hi6930_wdt_case02(void);
s32 hi6930_wdt_case03(void);
s32 hi6930_wdt_case04(void);
s32 hi6930_wdt_case05(void);
void bsp_wdt_print_debug(void);
void bsp_wdt_test(void);

/*初始化WDT，使能WDT，去使能WDT*/
s32 hi6930_wdt_case01(void)
{
	u32 timeout = 0;
	/* 使能复位和中断 */
	bsp_wdt_start();
	/*设置超时时间*/
	bsp_wdt_set_timeout(30);	
	/* 获取看门狗剩余超时时间 */
	bsp_wdt_get_timeleft(&timeout);
	printk("TIMEOUT = %d\n", timeout);  
	/*stop watchdog*/
	if (0 == bsp_wdt_stop())
	{
  	  printk("***************hi6930_wdt_case01 pass*******************\n");   
   	}
   else
   	{
	  printk("***************hi6930_wdt_case01 fail*******************\n");
	  return WDT_ERROR;
   	}
	return WDT_OK;
}

/*初始化WDT，使能WDT，WDT一直运行*/
s32 hi6930_wdt_case02(void)
{
	u32 timeout = 0;
	/* 使能复位和中断 */
	bsp_wdt_start();
	/*设置超时时间*/
	bsp_wdt_set_timeout(30);	
	/* 获取看门狗剩余超时时间 */
	bsp_wdt_get_timeleft(&timeout);
	printk("TIMEOUT = %d\n", timeout);  
	/*watchdog running*/
	printk("***************hi6930_wdt_case02 pass*******************\n"); 
	return WDT_OK;
}

/*使能WDT，产生中断(A核)，复位*/
s32 hi6930_wdt_case03(void)
{
	/* 并使能复位和中断 */
	bsp_wdt_start();
	/*设置超时间隔*/
	bsp_wdt_set_timeout(5);

	 printk("***************hi6930_wdt_case03 pass*******************\n"); 
	 return WDT_OK;
}

/*测试回调函数*/
s32 hi6930_wdt_case04(void)
{
   	wdt_timeout_cb wdt_hook = (void*)1;
   	bsp_wdt_register_hook(wdt_hook);
   	bsp_wdt_unregister_hook();
   	printk("***************hi6930_wdt_case04 pass*******************\n"); 
   	return WDT_OK;
}

/*低功耗接口*/
s32 hi6930_wdt_case05(void)
{
	bsp_wdt_start();
	/*设置计数时间60s*/
	bsp_wdt_suspend(60);
	/*计数时间恢复为30s*/
	bsp_wdt_resume();
	bsp_wdt_stop();
	printk("***************hi6930_wdt_case05 pass*******************\n"); 
	return WDT_OK;
}




void bsp_wdt_test(void)
{
	 spinlock_t lock;
    wdt_err("enter wdt test\n");
    spin_lock_init(&lock);
    spin_lock(&lock);
    spin_lock(&lock);
    
}


/*************************************wdt auto test start********************************************/
/* wdt nv must be enable*/
int bsp_wdt_start_test001(void)
{
	int timeleft = 0;
	bsp_wdt_start();

	bsp_wdt_get_timeleft(&timeleft);
	if (timeleft <= 30)
	{
		return BSP_OK;
	}
	else
	{
		return BSP_ERROR;
	}
}

int bsp_wdt_stop_test002(void)
{
	int timeleft = 0;
	bsp_wdt_start();
	bsp_wdt_stop();
	bsp_wdt_get_timeleft(&timeleft);
	if ((timeleft < 30) && (timeleft > 0))
	{
		wdt_err("%d\n", timeleft);
		return BSP_ERROR;
	}
	else
	{
		return BSP_OK;
	}
}

int bsp_wdt_keeplive_test003(void)
{
	int timeleft = 0;
	bsp_wdt_start();
	bsp_wdt_keepalive();
	bsp_wdt_get_timeleft(&timeleft);
	if ((timeleft > 20) && (timeleft <=30))
	{
		return BSP_OK;
	}
	else
	{
		return BSP_ERROR;
	}
}

int bsp_wdt_set_timeout_test004(void)
{
	int timeleft = 0;
	bsp_wdt_start();
	bsp_wdt_set_timeout(60);
	bsp_wdt_get_timeleft(&timeleft);
	if ((timeleft > 30) && (timeleft <=60))
	{
		return BSP_OK;
	}
	else
	{
		return BSP_ERROR;
	}
}

int bsp_wdt_register_hook_test005(void)
{
	int timeleft = 0;
	bsp_wdt_register_hook(&timeleft);
	bsp_wdt_reboot_register_hook(&timeleft);
	return BSP_OK;
}

int bsp_wdt_unregister_hook_test006(void)
{
	bsp_wdt_unregister_hook();
	bsp_wdt_reboot_unregister_hook();
	return BSP_OK;
}
/*************************************wdt auto test end********************************************/
