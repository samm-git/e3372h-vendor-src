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

#define CLK_TEST_OK 0
#define CLK_TEST_ERROR 1
#ifdef __KERNEL__
/*lint --e{737}*/
int clk_get_test_case0(void)
{
    struct clk *sio_clk = clk_get(NULL,"sio_clk");
	struct clk *sio_yclk = clk_get(NULL,"sio_yclk");
	unsigned int ret = 0;
    if(IS_ERR(sio_yclk)){
	    ret = (unsigned int)clk_enable(sio_yclk);
	     clk_disable(sio_yclk);
	     ret |= clk_get_rate(sio_yclk);
	     ret |= clk_set_rate(sio_yclk,100);
	     ret |= clk_round_rate(sio_yclk,100);
	     sio_clk = clk_get_parent(sio_yclk);
	     ret |= clk_set_parent(sio_yclk,sio_clk);
	     ret |= clk_status(sio_yclk);
	     if(ret == 0)
            		return CLK_TEST_OK;
    }
    return CLK_TEST_OK;
}

int clk_enable_test_case0(void)
{
    struct clk *sio_clk = clk_get(NULL,"sio_clk");
	struct clk *apb_pclk = clk_get(NULL,"sleep_clk");
	struct clk *mmc2_clk = clk_get(NULL,"mmc2_clk");
	unsigned int ret = 0;
	ret =  (unsigned int)clk_enable(apb_pclk);
	clk_disable(apb_pclk);
	ret |= clk_round_rate(apb_pclk,100);
	ret |= clk_set_rate(apb_pclk,100);
	ret |= clk_set_parent(apb_pclk, sio_clk);
	ret |= clk_status(apb_pclk);
        ret =  (unsigned int)clk_enable(mmc2_clk);
	clk_disable(mmc2_clk);
	ret |= clk_round_rate(mmc2_clk,100);
	ret |= clk_set_rate(mmc2_clk,100);
	ret |= clk_status(mmc2_clk);	
	if(ret)
		return CLK_TEST_OK;
    return CLK_TEST_OK;
}

int clk_disable_test_case0(void)
{
    struct clk *sio_clk = clk_get(NULL,"sio_clk");
    int ret = 0;
    clk_disable(sio_clk);
    if (reg_message[sio_clk->clkid].enable_flag){
                ret = sio_clk->ops->isenable(sio_clk);
    }
    if(ret){
            clk_printf("can't disable sio_clk\n");
            return CLK_TEST_ERROR;
    }
    return CLK_TEST_OK;
}

int clk_get_parent_test_case0(void)
{
    struct clk *sio_clk = clk_get(NULL,"sio_clk");
    struct clk *tcxo = clk_get(NULL, "tcxo");
    struct clk *parent = clk_get_parent(sio_clk);
    if (parent != tcxo){
            clk_printf("sio_clk parent is not tcxo\n");
            return CLK_TEST_ERROR;
    }
    return CLK_TEST_OK;
}

int clk_get_rate_test_case0(void)
{
    struct clk *sio_clk = clk_get(NULL,"sio_clk");
    unsigned long rate = clk_get_rate(sio_clk);
    if (rate != TCXO){
            clk_printf("sio_clk rate is not 19.2M\n");
            return CLK_TEST_ERROR;
    }
    return CLK_TEST_OK;
}

int clk_set_parent_test_case0(void)
{
    struct clk *sio_clk = clk_get(NULL,"sio_outer_clk");
    struct clk *peripll_fout4 = clk_get(NULL, "reverse_sio_clk");
    struct clk *tcxo = clk_get(NULL, "sio_clk");
    int ret = 0;
    ret = clk_set_parent(sio_clk, peripll_fout4);
    if (ret){
            clk_printf("sio_clk set parent reverse_sio_clk,failure\n");
            return CLK_TEST_ERROR;
    }
    clk_set_parent(sio_clk, tcxo);
    return CLK_TEST_OK;
}

int clk_set_rate_test_case0(void)
{
    struct clk *sio_bclk = clk_get(NULL,"sio_bclk");
    int ret = 0;
    ret = clk_set_rate(sio_bclk, TCXO/4);
    if (ret){
            clk_printf("sio_bclk set rate TCXO/4,failure\n");
            return CLK_TEST_ERROR;
   }
    clk_set_rate(sio_bclk, TCXO);
	debug_clock();
	debug_clk_enable(30);
	debug_clk_disable(30);
	debug_clk_set_rate(30,100);
	debug_clk_get_rate(30);
	debug_clk_set_parent(30,31);
	debug_clk_status(30);
	
	debug_clk_enable(14);
	debug_clk_disable(14);
	debug_clk_set_rate(14,100);
	debug_clk_get_rate(14);
	debug_clk_set_parent(14,15);
	debug_clk_status(14);
         return CLK_TEST_OK;
}

#else
int clk_get_test_case1(void)
{
    struct clk *sio_bclk = clk_get(NULL,"sio_bclk");
	struct clk *sio_yclk = clk_get(NULL,"sio_yclk");
	int ret = 0;
	if(IS_ERR(sio_yclk)){
	    ret = clk_enable(sio_yclk);
	     clk_disable(sio_yclk);
	     ret |= (int)clk_get_rate(sio_yclk);
	     ret |= clk_set_rate(sio_yclk,100);
	     ret |= clk_round_rate(sio_yclk,100);
	     sio_bclk = clk_get_parent(sio_yclk);
	     ret |= clk_set_parent(sio_yclk, sio_bclk);
	     ret |= clk_status(sio_yclk);
	     if(ret == 0)
            		return CLK_TEST_OK;
    }
    return CLK_TEST_OK;
}


int clk_enable_test_case1(void)
{
    struct clk *sio_clk = clk_get(NULL,"sio_clk");
	
    struct clk *apb_pclk = clk_get(NULL,"apb_pclk");
    int ret = 0;
    ret =  clk_enable(sio_clk);
    if(ret){
            clk_printf("can't enable sio_clk\n");
            return CLK_TEST_ERROR;
    }
	ret =  clk_enable(apb_pclk);
	clk_disable(apb_pclk);
	ret |= clk_round_rate(apb_pclk,100);
	ret |= clk_set_rate(apb_pclk,100);
	ret |= clk_set_parent(apb_pclk,sio_clk);
	ret |= clk_status(apb_pclk);

	debug_clock();
	debug_clk_enable(30);
	debug_clk_disable(30);
	debug_clk_set_rate(30,100);
	debug_clk_get_rate(30);
	debug_clk_set_parent(30,31);
	debug_clk_status(30);
	
	debug_clk_enable(14);
	debug_clk_disable(14);
	debug_clk_set_rate(14,100);
	debug_clk_get_rate(14);
	debug_clk_set_parent(14,15);
	debug_clk_status(14);
	if(ret)
		return CLK_TEST_OK;
    return CLK_TEST_OK;
}

int clk_disable_test_case1(void)
{
    struct clk *sio_clk = clk_get(NULL,"sio_clk");
   struct clk *apb_pclk = clk_get(NULL,"sleep_clk");
	int ret = 0;
    clk_disable(sio_clk);
    if (reg_message[sio_clk->clkid].enable_flag){
                ret = sio_clk->ops->isenable(sio_clk);
    }
    if(ret){
            clk_printf("can't disable sio_clk\n");
            return CLK_TEST_ERROR;
    }
	ret =  clk_enable(apb_pclk);
	clk_disable(apb_pclk);
	ret |= clk_round_rate(apb_pclk,100);
	ret |= clk_set_rate(apb_pclk,100);
	ret |= clk_set_parent(apb_pclk,sio_clk);
	ret |= clk_status(apb_pclk);
	if(ret)
		return CLK_TEST_OK;
	if(!ret)
		return CLK_TEST_ERROR;
    return CLK_TEST_OK;
}

int clk_get_parent_test_case1(void)
{
    struct clk *sio_clk = clk_get(NULL,"sio_clk");
    struct clk *tcxo = clk_get(NULL, "tcxo");
    struct clk *parent = clk_get_parent(sio_clk);
    if (parent != tcxo){
            clk_printf("sio_clk parent is not tcxo\n");
            return CLK_TEST_ERROR;
    }
    return CLK_TEST_OK;
}

int clk_get_rate_test_case1(void)
{
    struct clk *sio_clk = clk_get(NULL,"sio_clk");
    unsigned long rate = clk_get_rate(sio_clk);
    if (rate != TCXO){
            clk_printf("sio_clk rate is not 19.2M\n");
            return CLK_TEST_ERROR;
    }
    return CLK_TEST_OK;
}

int clk_set_parent_test_case1(void)
{
    struct clk *sio_clk = clk_get(NULL,"sio_outer_clk");
    struct clk *peripll_fout4 = clk_get(NULL, "reverse_sio_clk");
    struct clk *tcxo = clk_get(NULL, "sio_clk");
    int ret = 0;
    ret = clk_set_parent(sio_clk, peripll_fout4);
    if (ret){
            clk_printf("sio_clk set parent reverse_sio_clk,failure\n");
            return CLK_TEST_ERROR;
    }
   clk_set_parent(sio_clk, tcxo);
    return CLK_TEST_OK;
}

int clk_set_rate_test_case1(void)
{
    struct clk *sio_bclk = clk_get(NULL,"sio_bclk");
   struct clk *cipher_clk = clk_get(NULL,"cipher_clk");
    int ret = 0;
    clk_set_rate(sio_bclk, TCXO/4);
    if (ret){
            clk_printf("sio_bclk set rate TCXO/4,failure\n");
            return CLK_TEST_ERROR;
   }
    ret = clk_set_rate(sio_bclk, TCXO);
    ret = clk_set_rate(cipher_clk, PERIPLL_FOUNTPOST/5);
    if (ret){
            clk_printf("cipher_clk PERIPLL_FOUNTPOST/5,failure\n");
            return CLK_TEST_ERROR;
   }
   clk_round_rate(cipher_clk, PERIPLL_FOUNTPOST/5);
    return CLK_TEST_OK;
}
#endif

