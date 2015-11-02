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

/* this function is used to check whether the parent clock
is this clock's source, and get the clksel*/
static struct clksel *hi6930_getclksel_by_parent(struct clk *clk,
	struct clk *parent_clk)
{
	struct clksel *clks = clk->sel_parents;

	if (clks) {
		while (clks->sel_parent) {
			if (clks->sel_parent == parent_clk)
				break;
			clks++;
		}

		if (!clks->sel_parent) {
			return NULL;
		}
	}

	return clks;
}

/* this function is used to find the right parent which can provide the rate */
static struct clk *hi6930_clk_find_parent_from_selparents(struct clk *clk, unsigned long rate)
{
    struct clksel *clks = NULL;
    struct clk *clkparent = NULL;
    for (clks = clk->sel_parents; clks->sel_parent; clks++) {
    	/* this rate can be brought by this parent */
    	if (rate <= clks->parent_max && rate >= clks->parent_min) {
    		clkparent = clks->sel_parent;
    		break; /* Found the requested parent */
    	}
    }
	return clkparent;
}

int hi6930_clk_enable (struct clk *clk)
{
       /*lint -e550*/
        unsigned int clkid = 0;
        unsigned int clkcon = 0;
        clkid = clk->clkid;

        /*enable clk*/
        if (!reg_message[clkid].enable_flag){
                clk_printf("clock: %s has no enable register!!!\n", clk->name);
                return 0;
        }
        if (clk->ops->isenable(clk) == 1){
                return 0;
        }
        clkcon = ((unsigned int)0x1 << reg_message[clkid].enable_bit);/* [false alarm]:误报 */
        writel(clkcon, HI_SYSCRG_BASE_ADDR_VIRT + reg_message[clkid].enable_reg);
         return 0;
}
void hi6930_clk_disable (struct clk *clk)
{
        unsigned int clkid = 0;
        unsigned int clkcon = 0;
        clkid = clk->clkid;

        /*disable clk*/
        if (!reg_message[clkid].enable_flag){
                clk_printf("clock: %s has no disable register!!!\n", clk->name);
                return;
        }
        clkcon = ((unsigned int)0x1 << reg_message[clkid].enable_bit);/* [false alarm]:误报 */
        writel(clkcon, HI_SYSCRG_BASE_ADDR_VIRT + 0x4 + reg_message[clkid].enable_reg);
         return;
}
int hi6930_clk_round_rate(struct clk *clk, unsigned long rate)
{
	return 0;
}

/*for clock div N + 1*/
int hi6930_clk_set_rate(struct clk *clk, unsigned long rate)
{
        struct clk *clkparent = NULL;
        unsigned int div = 0;
        unsigned int clkid = 0;
        unsigned int regvalue = 0;
        int ret = 0;
        clkid = clk->clkid;
        if (!clk->parent){
                 clk_printf("clock: %s has no parent,can't set rate!!!\n", clk->name);
                 return -1;
        }
         /*find the right rate from its mulparent*/
        if (NULL != clk->sel_parents){
                clkparent = hi6930_clk_find_parent_from_selparents(clk, rate);
                if (clkparent == clk->parent)
                         return 0;
                if (!clk->ops || !clk->ops->set_parent){
                         clk_printf("clock: %s has no ops or ops->set_parent!!!\n", clk->name);
                         return -1;
                }
                ret = clk->ops->set_parent(clk, clkparent);
                clk->rate = clk->parent->rate;
                return ret;
        }

        /*set clock div by register,div must in the range of bits*/
        if (reg_message[clkid].div_reg){
                div = clk->parent->rate / rate;
                if(clk->parent->rate % rate)
                    div = div + 1;
                if (div >= REG_MIN_VALUE && div <= (reg_message[clkid].div + 1)){
                        regvalue = readl(HI_SYSCRG_BASE_ADDR_VIRT + reg_message[clkid].div_reg);
                        regvalue &= (~(reg_message[clkid].div << reg_message[clkid].div_bit));
                        regvalue |= ((div - 0x1) << reg_message[clkid].div_bit);/* [false alarm]:误报 */
                        writel(regvalue, HI_SYSCRG_BASE_ADDR_VIRT + reg_message[clkid].div_reg);
                        clk->rate = clk->parent->rate / div;
                        return 0;
                }
        }
        clk_printf("clock: %s clk_set_rate failure!!!\n", clk->name);
        return -1;
}

/*for clock div N */
int hi6930_clk_set_rate_n(struct clk *clk, unsigned long rate)
{
        struct clk *clkparent = NULL;
        unsigned int div = 0;
        unsigned int clkid = 0;
        unsigned int regvalue = 0;
        int ret = 0;
        clkid = clk->clkid;
        if (!clk->parent){
                 clk_printf("clock: %s has no parent,can't set rate!!!\n", clk->name);
                 return -1;
        }
         /*find the right rate from its mulparent*/
        if (NULL != clk->sel_parents){
                clkparent = hi6930_clk_find_parent_from_selparents(clk, rate);
                if (clkparent == clk->parent)
                         return 0;
                if (!clk->ops || !clk->ops->set_parent){
                         clk_printf("clock: %s has no ops or ops->set_parent!!!\n", clk->name);
                         return -1;
                }
                ret = clk->ops->set_parent(clk, clkparent);
                clk->rate = clk->parent->rate;
                return ret;
        }

        /*set clock div by register,div must in the range of bits*/
        if (reg_message[clkid].div_reg){
                div = clk->parent->rate / rate;
                if(clk->parent->rate % rate)
                    div = div + 1;
                if (div >= REG_MIN_VALUE && div <= reg_message[clkid].div){
                        regvalue = readl(HI_SYSCRG_BASE_ADDR_VIRT + reg_message[clkid].div_reg);
                        regvalue &= (~(reg_message[clkid].div << reg_message[clkid].div_bit));
                        regvalue |= (div << reg_message[clkid].div_bit);/* [false alarm]:误报 */
                        writel(regvalue, HI_SYSCRG_BASE_ADDR_VIRT + reg_message[clkid].div_reg);
                        clk->rate = clk->parent->rate / div;
                        return 0;
                }
        }
        clk_printf("clock: %s clk_set_rate failure!!!\n", clk->name);
        return -1;
}


int hi6930_clk_set_parent(struct clk *clk, struct clk *parent)
{
        struct clksel *clks = NULL;
        unsigned int clkid = 0;
        unsigned int regvalue = 0;
        clkid = clk->clkid;

        if (clk->parent == NULL){
                clk_printf("clock: %s parent is NULL,so can't set parent!!!\n", clk->name);
                return -1;
         }
         /*the future parent is its current parent,oprate sucess*/
        if (!reg_message[clkid].sel_reg) {
                if (clk->parent == parent) {
                        return 0;
                }
                clk_printf("clock: %s has no multiparent!!!\n", clk->name);
                return -1;
        }

        /*search input parent from array*/
        clks = hi6930_getclksel_by_parent(clk, parent);
        if (!clks){
                clk_printf("parent: %s,is no the parent of %s!!!\n", parent->name, clk->name);
                return -1;
        }

        /* set the register's bit to get the clock source */
        if(!reg_message[clkid].sel_reg){
                clk_printf("clock: %s has no set parent register!!!\n", clk->name);
                return -1;
        }
        regvalue = readl(HI_SYSCRG_BASE_ADDR_VIRT + reg_message[clkid].sel_reg);
        regvalue &= (~(reg_message[clkid].sel << reg_message[clkid].sel_bit));
        regvalue |= ((unsigned int)clks->sel_val << reg_message[clkid].sel_bit);/* [false alarm]:误报 */
        writel(regvalue, HI_SYSCRG_BASE_ADDR_VIRT + reg_message[clkid].sel_reg);
        clk->parent = parent;
        clk->rate = parent->rate;
        return 0;
}

int hi6930_clk_isenable(struct clk *clk)
{
        unsigned int clkid = 0;
        unsigned int clkcon = 0;
        clkid = clk->clkid;
        if (reg_message[clkid].enable_flag){
            clkcon = readl(HI_SYSCRG_BASE_ADDR_VIRT + 0x8 + reg_message[clkid].enable_reg);
            if (clkcon &((unsigned int)(0x1) << reg_message[clkid].enable_bit)) {
                return 1;
            }
            else{
                return 0;
            }
        }
        clk_printf("clock: %s has no status register!!!\n", clk->name);
        return -1;
}
/*时钟的通用ops*/


struct clk_ops clock_ops = {
    hi6930_clk_isenable,
	hi6930_clk_enable,
	hi6930_clk_disable,
	hi6930_clk_round_rate,
	hi6930_clk_set_rate,
	hi6930_clk_set_parent,
};

struct clk_ops clock_ops_div_n = {
    hi6930_clk_isenable,
	hi6930_clk_enable,
	hi6930_clk_disable,
	hi6930_clk_round_rate,
	hi6930_clk_set_rate_n,
	hi6930_clk_set_parent,
};

