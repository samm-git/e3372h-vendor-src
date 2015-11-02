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

#include "osl_types.h"
#include "osl_bio.h"
#include "osl_irq.h"
#include "osl_module.h"
#include "hi_vic.h"
#include "vic_balong.h"
#include "bsp_vic.h"
#include "drv_version.h"

#ifdef __KERNEL__
#define VIC_PRINT(a)   printk(a)
#else
#define VIC_PRINT(a)   logMsg(a, 0, 0, 0, 0, 0, 0)
#endif


bsp_vic_st gstVicInfo[BALONG_VIC_NUM];
u32 gGuVicFlag = 0;

void gu_vic_disable(u32 vicnum, int level);

/*****************************************************************
 * Function name	 : bsp_vic_inthandler 
 * Author			 : luting 00168886
 * Created		 : 2013-01-24
 * Description		 : vic irq handler
 *****************************************************************/
OSL_IRQ_FUNC(irqreturn_t, bsp_vic_inthandler, intLvl, dev)
{
	int newLevel = 0;
	u32 isr = 0;
	bsp_vic_st* pVic = (bsp_vic_st*)dev;

	isr = readl(pVic->vic_base_addr + pVic->vic_final_status);

	if(isr == 0)
	{
		return IRQ_NONE;
	}

	for(;;)
	{
		if (0x1 == (isr & 0x1))
		{
			break;
		}
		isr = isr >> 1;
		newLevel++;
	}

	if(NULL != pVic->victable[newLevel].routine)
	{
		pVic->victable[newLevel].routine(pVic->victable[newLevel].arg);
	}

	return IRQ_HANDLED;
}

/*****************************************************************
 * Function name	 : bsp_vic_init
 * Author			 : luting 00168886
 * Created             : 2013-01-24
 * Description		 : init vic module
 *****************************************************************/
s32 __init bsp_vic_init(void)
{
    u32 i, j;

	if(BOARD_TYPE_GUL == bsp_get_board_mode_type())
	{
		gGuVicFlag = 1;
	}


		
	gstVicInfo[0].vic_base_addr = BSP_VIC_BASE;
	gstVicInfo[0].vic_irq_num = INT_LVL_VIC;
	gstVicInfo[0].vic_inten = 0x0;
	gstVicInfo[0].vic_intdisable = 0x0;
	gstVicInfo[0].vic_final_status = 0x30;
	
	
	if(1 == gGuVicFlag)
	{
	    gstVicInfo[1].vic_base_addr = BSP_GU_VIC_BASE;
	    gstVicInfo[1].vic_irq_num = INT_GU_LVL_VIC;
		gstVicInfo[1].vic_inten = 0x10;
		gstVicInfo[1].vic_intdisable = 0x14;
		gstVicInfo[1].vic_final_status = 0x0;		
	}
	
	for(j = 0; j <= gGuVicFlag; j++)
	{
		for(i=0; i<INT_VIC_MAX; i++)
		{
			gstVicInfo[j].victable[i].routine = NULL;
			gstVicInfo[j].victable[i].arg = 0;
		}
		writel(0x0, gstVicInfo[j].vic_base_addr + gstVicInfo[j].vic_inten);

		if(request_irq(gstVicInfo[j].vic_irq_num, (irq_handler_t)bsp_vic_inthandler, 0, "vic", &gstVicInfo[j]) != OK)
		{
			VIC_PRINT("bsp_vic_init error \n");
			return ERROR;
		}
	}

	VIC_PRINT("bsp_vic_init ok \n");
	return OK;
}

/*****************************************************************
 * Function name	 : bsp_vic_enable 
 * Author			 : luting 00168886
 * Created		 : 2013-01-24
 * Description		 : enable vic irq
 *****************************************************************/
s32 bsp_vic_enable(int level)
{
    u32 ulValue = 0;
    int key = 0;
	u32 vicnum = 0;

	if((level < 0) || (level >= (INT_GU_VIC_BASE_LVL + gGuVicFlag * INT_VIC_MAX)))
	{
		VIC_PRINT("bsp_vic_enable input error \n");
		return 	ERROR;
	}
	if(level >= INT_GU_VIC_BASE_LVL)
	{
		vicnum = 1;
		level -= INT_GU_VIC_BASE_LVL;
	}

	local_irq_save(key);

	ulValue = readl(gstVicInfo[vicnum].vic_base_addr + gstVicInfo[vicnum].vic_inten);
    ulValue |= (1<<level);
	writel(ulValue, gstVicInfo[vicnum].vic_base_addr + gstVicInfo[vicnum].vic_inten);

	local_irq_restore(key);

    return OK;
}

/*****************************************************************
 * Function name	 : bsp_vic_disable 
 * Author			 : luting 00168886
 * Created		 : 2013-01-24
 * Description		 : disable vic irq
 *****************************************************************/
s32 bsp_vic_disable (int level)
{

    u32 ulValue = 0;
    int key = 0;
	u32 vicnum = 0;
	
	if((level < 0) || (level >= (INT_GU_VIC_BASE_LVL + gGuVicFlag * INT_VIC_MAX)))
	{
		VIC_PRINT("bsp_vic_disable input error \n");
		return 	ERROR;
	}
	if(level >= INT_GU_VIC_BASE_LVL)
	{
		vicnum = 1;
		level -= INT_GU_VIC_BASE_LVL;
		gu_vic_disable(vicnum, level);
		return OK;	
	}

	local_irq_save(key);

	ulValue = readl(gstVicInfo[vicnum].vic_base_addr + gstVicInfo[vicnum].vic_intdisable);
    ulValue &= ~(1<<level);
	writel(ulValue, gstVicInfo[vicnum].vic_base_addr + gstVicInfo[vicnum].vic_intdisable);

	local_irq_restore(key);

    return OK;	
}

void gu_vic_disable(u32 vicnum, int level)
{
    u32 ulValue = 0;
    int key = 0;
	
	local_irq_save(key);

    ulValue = (1<<level);
	writel(ulValue, gstVicInfo[vicnum].vic_base_addr + gstVicInfo[vicnum].vic_intdisable);
}
/*****************************************************************
 * Function name     : bsp_vic_connect 
 * Author        	   : luting 00168886
 * Created              : 2013-01-24
 * Description          : register vic irq handler
 *****************************************************************/
s32 bsp_vic_connect(int level, vicfuncptr routine, s32 parameter)
{
	u32 vicnum = 0;

	if((level < 0) || (level >= (INT_GU_VIC_BASE_LVL + gGuVicFlag * INT_VIC_MAX)))
	{
		VIC_PRINT("bsp_vic_connect input error \n");
		return 	ERROR;
	}
	if(level >= INT_GU_VIC_BASE_LVL)
	{
		vicnum = 1;
		level -= INT_GU_VIC_BASE_LVL;
	}

    gstVicInfo[vicnum].victable[level].routine = routine;
    gstVicInfo[vicnum].victable[level].arg = parameter;

	return OK;
}


/****************************************************************
 * Function name     : bsp_vic_disconnect
 * Author        	   : luting 00168886
 * Created              : 2013-01-24
 * Description          : unregister vic irq handler
 *****************************************************************/
s32 bsp_vic_disconnect(int level)
{    
	u32 vicnum = 0;
	
	if((level < 0) || (level >= (INT_GU_VIC_BASE_LVL + gGuVicFlag * INT_VIC_MAX)))
	{
		VIC_PRINT("bsp_vic_disconnect input error \n");
		return 	ERROR;
	}
	if(level >= INT_GU_VIC_BASE_LVL)
	{
		vicnum = 1;
		level -= INT_GU_VIC_BASE_LVL;
	}

    gstVicInfo[vicnum].victable[level].routine = NULL;
    gstVicInfo[vicnum].victable[level].arg = 0;

	return OK;
}

module_init(bsp_vic_init);
EXPORT_SYMBOL(bsp_vic_enable);
EXPORT_SYMBOL(bsp_vic_disable);
EXPORT_SYMBOL(bsp_vic_connect);
EXPORT_SYMBOL(bsp_vic_disconnect);

