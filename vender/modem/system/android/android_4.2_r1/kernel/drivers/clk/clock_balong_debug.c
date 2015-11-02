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
#include <bsp_clk.h>
#include "clock.h"

void clock_debug(void)
{
      int i = 0;
      unsigned int clkid = 0;

      unsigned int status_value = 0;
	struct clk_lookup *cl_lookups = &hi6930_clk_lookup[0];
	struct clk *clk = NULL;
       clk_printf ("name            rate           refcnt          status           enable reg        bit\n");
	while((cl_lookups[i].clk != NULL) ) {
		clk = cl_lookups[i].clk;
		clkid = clk->clkid;
             if (reg_message[clkid].enable_flag){
                 status_value = (unsigned int)clk->ops->isenable(clk);
                 clk_printf("%-15s %-15ld %-15d %-15x 0x%-15x %-15d\n",clk->name,clk->rate,clk->refcnt,status_value,reg_message[clkid].enable_reg,reg_message[clkid].enable_bit);
              }
             else{
                 status_value = 0;
                 clk_printf("%-15s %-15ld %-15d %-15d 0x%-15x %-15d\n",clk->name,clk->rate,clk->refcnt,status_value,reg_message[clkid].enable_reg,reg_message[clkid].enable_bit);
             }
		i++;
	}
      return;
}

void debug_clock(void)
{
	  struct clk *clk = NULL;
         struct clk_lookup *cl_lookups = &hi6930_clk_lookup[0];
         int i = 0;
	  while((cl_lookups[i].clk != NULL) ) {
		clk = cl_lookups[i].clk;
		clk_printf("clk_name:%-20s num: %-15d\n", clk->name, i);
		i++;
	  }
        clk_printf("enable:          void debug_clk_enable(unsigned int num)\n");
        clk_printf("disable:         void debug_clk_disable(unsigned int num)\n");
        clk_printf("check status:    void debug_clk_status(unsigned int num)\n");
        clk_printf("set rate :       void debug_clk_set_rate(unsigned int num, unsigned long rate)\n");
	 clk_printf("get rate :       void debug_clk_get_rate(unsigned int num)\n");
        clk_printf("set parent :     void debug_clk_set_parent(unsigned int num, unsigned int parent_num)\n");
        return;
}


void debug_clk_enable(unsigned int num)
{
        int ret = 0;
        const char *clk_name = NULL;
        struct clk *clk = NULL;
        clk_name = hi6930_clk_lookup[num].con_id;
        clk = clk_get(NULL, clk_name);
        if(IS_ERR(clk)){
                clk_printf("clk:%s is NULL, can't find it,please check!!\n", clk_name);
                return;
        }
        ret = clk_enable(clk);
        if(!ret){
                clk_printf("clk:%s enable OK!!\n", clk_name);
                return;
         }
        clk_printf("clk:%s enable failure!!\n", clk_name);
        return;
}
void debug_clk_disable(unsigned int num)
{
        const char *clk_name = NULL;
        struct clk *clk = NULL;
        clk_name = hi6930_clk_lookup[num].con_id;
        clk = clk_get(NULL, clk_name);
        if(IS_ERR(clk)){
                clk_printf("clk:%s is NULL, can't find it,please check!!\n", clk_name);
                return;
        }
        clk_disable(clk);
        return;
}
void debug_clk_status(unsigned int num)
{
        const char *clk_name = NULL;
        struct clk *clk = NULL;
        int ret = 0;
        clk_name = hi6930_clk_lookup[num].con_id;
        clk = clk_get(NULL, clk_name);
        ret = clk_status(clk);
        clk_printf("clk:%s status = %d\n", clk_name, ret);
        return;
}

void debug_clk_set_rate(unsigned int num, unsigned long rate)
{
        const char *clk_name = NULL;
        struct clk *clk = NULL;
        unsigned long ret = 0;
        clk_name = hi6930_clk_lookup[num].con_id;
        clk = clk_get(NULL, clk_name);
        if(IS_ERR(clk)){
                clk_printf("clk:%s is NULL, can't find it,please check!!\n", clk_name);
                return;
        }
        ret = (unsigned long)clk_set_rate(clk, rate);
        if (ret){
                clk_printf("clk:%s ---set rate :%ld, failure!!\n", clk_name, rate);
                return;
        }
        ret = clk_get_rate(clk);
        clk_printf("clk:%s ---rate is :%ld\n\n", clk_name, ret);
        return;
}

void debug_clk_get_rate(unsigned int num)
{
        const char *clk_name = NULL;
        struct clk *clk = NULL;
        unsigned long ret = 0;
        clk_name = hi6930_clk_lookup[num].con_id;
        clk = clk_get(NULL, clk_name);
        if(IS_ERR(clk)){
                clk_printf("clk:%s is NULL, can't find it,please check!!\n", clk_name);
                return;
        }
        ret = clk_get_rate(clk);
        clk_printf("clk:%s ---rate is :%ld\n\n", clk_name, ret);
        return;
}

void debug_clk_set_parent(unsigned int num, unsigned int parent_num)
{
        const char *clk_name = NULL;
        const char *parent_name = NULL;
        struct clk *clk = NULL;
        struct clk *parent_clk = NULL;
        unsigned long ret = 0;
        clk_name = hi6930_clk_lookup[num].con_id;
        clk = clk_get(NULL, clk_name);

        parent_name = hi6930_clk_lookup[parent_num].con_id;
        parent_clk = clk_get(NULL, parent_name);
        if(IS_ERR(clk)){
                clk_printf("clk:%s is NULL, can't find it,please check!!\n", clk_name);
                return;
        }
        if(IS_ERR(parent_clk)){
                clk_printf("clk:%s is NULL, can't find it,please check!!\n", parent_name);
                return;
        }
        ret = (unsigned long)clk_set_parent(clk, parent_clk);
        if (ret){
                clk_printf("clk:%s set parent clk :%s, failure!!\n", clk_name, parent_name);
                return;
        }
        parent_clk = clk_get_parent(clk);
		if(parent_clk == NULL)
			return;
        clk_printf("clk:%s ---parent is :%s\n\n", clk_name, parent_clk->name);
        return;
}


void debug_clk_show_reg(unsigned int reg_addr_offset)
{
    unsigned int reg_val;
	reg_val = readl(HI_SYSCTRL_BASE_ADDR_VIRT + reg_addr_offset);
    clk_printf("reg_addr_offset 0x%08x reg_val = 0x%08x\n", reg_addr_offset, reg_val);

    return ;
}

void debug_clk_wr_reg(unsigned int reg_addr_offset, unsigned int reg_val)
{
	writel(reg_val, HI_SYSCTRL_BASE_ADDR_VIRT + reg_addr_offset);
	clk_printf("wr reg_val 0x%08x to reg_addr_offset 0x%08x ok\n", reg_val, reg_addr_offset);

    return ;
}


void debug_clk_set_mmc0_cclk_to_4m(void)
{
    int Ret;

    /* 设置refclk的父时钟为480M */
    struct clk *pstru_clk_mmc0_refclk = NULL;
	struct clk *pstru_clk_peripll_fout1 = NULL;
	struct clk *pstru_clk_mmc0_phase_clk = NULL;
	struct clk *pstru_clk_mmc0_cclk = NULL;

	pstru_clk_mmc0_refclk = clk_get(NULL, "mmc0_refclk");
	if (NULL == pstru_clk_mmc0_refclk)
	{
	    clk_printf("clk_get mmc0_refclk err\n");
		return ;
	}
    clk_printf("clk_get mmc0_refclk ok\n");


	pstru_clk_peripll_fout1 = clk_get(NULL, "peripll_fout1");
	if (NULL == pstru_clk_peripll_fout1)
	{
	    clk_printf("clk_get peripll_fout1 err\n");
		return ;
	}
    clk_printf("clk_get peripll_fout1 ok\n");

	Ret = clk_set_parent(pstru_clk_mmc0_refclk, pstru_clk_peripll_fout1);
	if (Ret)
	{
	    clk_printf("clk_set_parent err Ret = %d\n", Ret);
		return ;
	}
    clk_printf("clk_set_parent  ok\n");

    /* 设置mmc0 phase clk 到 60M */
	pstru_clk_mmc0_phase_clk = clk_get(NULL, "mmc0_phase_clk");
	if (NULL == pstru_clk_mmc0_phase_clk)
	{
	    clk_printf("clk_get mmc0_phase_clk err\n");
		return ;
	}
	Ret = clk_set_rate(pstru_clk_mmc0_phase_clk, 60000000);
	if (Ret)
	{
	    clk_printf("clk_set_rate to 60M Err Ret = %d\n", Ret);
		return ;
	}
    clk_printf("clk_set_rate to 60M ok\n");

    /* 设置mmc0 cclk 到 4M */
	pstru_clk_mmc0_cclk = clk_get(NULL, "mmc0_cclk");
	if (NULL == pstru_clk_mmc0_cclk)
	{
	    clk_printf("clk_get mmc0_cclk err\n");
		return ;
	}
	Ret = clk_set_rate(pstru_clk_mmc0_cclk, 4000000);
	if (Ret)
	{
	    clk_printf("clk_set_rate to 4M Err Ret = %d\n", Ret);
		return ;
	}
    clk_printf("clk_set_rate to 4M ok\n");

	
	
	

    return ;
}

void debug_clk_set_mmc0_cclk_to_100m(void)
{
    int Ret;

    /* 设置refclk的父时钟为600M */
    struct clk *pstru_clk_mmc0_refclk = NULL;
	struct clk *pstru_clk_dsp_pll = NULL;
	struct clk *pstru_clk_mmc0_phase_clk = NULL;
	struct clk *pstru_clk_mmc0_cclk = NULL;

	pstru_clk_mmc0_refclk = clk_get(NULL, "mmc0_refclk");
	if (NULL == pstru_clk_mmc0_refclk)
	{
	    clk_printf("clk_get mmc0_refclk err\n");
		return ;
	}
    clk_printf("clk_get mmc0_refclk ok\n");


	pstru_clk_dsp_pll = clk_get(NULL, "dsp_pll");
	if (NULL == pstru_clk_dsp_pll)
	{
	    clk_printf("clk_get dsp_pll err\n");
		return ;
	}
    clk_printf("clk_get dsp_pll ok\n");

	Ret = clk_set_parent(pstru_clk_mmc0_refclk, pstru_clk_dsp_pll);
	if (Ret)
	{
	    clk_printf("clk_set_parent err Ret = %d\n", Ret);
		return ;
	}
    clk_printf("clk_set_parent  ok\n");

    /* 设置mmc0 phase clk 到 600M */
	pstru_clk_mmc0_phase_clk = clk_get(NULL, "mmc0_phase_clk");
	if (NULL == pstru_clk_mmc0_phase_clk)
	{
	    clk_printf("clk_get mmc0_phase_clk err\n");
		return ;
	}
	Ret = clk_set_rate(pstru_clk_mmc0_phase_clk, 600000000);
	if (Ret)
	{
	    clk_printf("clk_set_rate to 600M Err Ret = %d\n", Ret);
		return ;
	}
    clk_printf("clk_set_rate to 600M ok\n");

    /* 设置mmc0 cclk 到 100M */
	pstru_clk_mmc0_cclk = clk_get(NULL, "mmc0_cclk");
	if (NULL == pstru_clk_mmc0_cclk)
	{
	    clk_printf("clk_get mmc0_cclk err\n");
		return ;
	}
	Ret = clk_set_rate(pstru_clk_mmc0_cclk, 100000000);
	if (Ret)
	{
	    clk_printf("clk_set_rate to 100M Err Ret = %d\n", Ret);
		return ;
	}
    clk_printf("clk_set_rate to 100M ok\n");

	
	
	

    return ;
}

#ifdef __KERNEL__

#ifdef CONFIG_DEBUG_FS

/*for debugfs ---begin*/
static int clock_debug_rate_set(void *data, u64 val)
{
	struct clk *clock = data;
	int ret;

	/* Only increases to max rate will succeed, but that's actually good
	 * for debugging purposes so we don't check for error. */
		ret = clk_set_rate(clock, val);
	if (ret != 0)
		clk_printf("%s clk_set_rate %ld failed\n",clock->name, val);
	return ret;
}
static int clock_debug_rate_get(void *data, u64 *val)
{
	struct clk *clock = data;
	*val = clk_get_rate(clock);
	return 0;
}
DEFINE_SIMPLE_ATTRIBUTE(clock_rate_fops, clock_debug_rate_get,
			clock_debug_rate_set, "%llu\n");

static int clock_debug_enable_set(void *data, u64 val)
{
	struct clk *clock = data;
	int rc = 0;
	unsigned int clkid = clock->clkid;
      if(!reg_message[clkid].enable_flag)
            return rc;
	if (val)
		rc = clock->ops->enable(clock);
	else
		clock->ops->disable(clock);

	return rc;
}

static int clock_debug_status_get(void *data, u64 *val)
{
	struct clk *clock = data;
	unsigned int clkid = clock->clkid;

       if(!reg_message[clkid].enable_flag)
            return 0;
	*val = clock->ops->isenable(clock);

	return 0;
}

DEFINE_SIMPLE_ATTRIBUTE(clock_enable_fops, clock_debug_status_get,
			clock_debug_enable_set, "%llu\n");

static struct dentry *debugfs_base;
int __init clock_debug_init(void)
{
	debugfs_base = debugfs_create_dir("clk", NULL);
	if (!debugfs_base){
	      clk_printf("@@@@@@@@clock_debug_init_failure");
		return -ENOMEM;
	}
	return 0;
}
int __init clock_debug_add(struct clk *clock)
{
	char temp[50], *ptr;
	struct dentry *clk_dir;

	if (!debugfs_base)
		return -ENOMEM;

	strncpy(temp, clock->name, ARRAY_SIZE(temp)-1);
	for (ptr = temp; *ptr; ptr++)
		*ptr = tolower(*ptr);

	clk_dir = debugfs_create_dir(temp, debugfs_base);
	if (!clk_dir)
		return -ENOMEM;

	if (!debugfs_create_file("rate", S_IRUGO | S_IWUSR, clk_dir,
				clock, &clock_rate_fops))
		goto error;

	if (!debugfs_create_file("enable", S_IRUGO | S_IWUSR, clk_dir,
				clock, &clock_enable_fops))
		goto error;
	return 0;
error:
	debugfs_remove_recursive(clk_dir);
	return -ENOMEM;
}

/*******************************************
*clk debugfs init
********************************************/
static int __init clock_late_init(void)
{
	struct clk_lookup *cl_lookups = &hi6930_clk_lookup[0];
       int i = 0;
	clock_debug_init();
	while((cl_lookups[i].clk != NULL) ) {
		clock_debug_add(cl_lookups[i].clk);
		i++;
      }
	return 0;
}

late_initcall(clock_late_init);
#endif

#endif /*__KERNEL__*/
