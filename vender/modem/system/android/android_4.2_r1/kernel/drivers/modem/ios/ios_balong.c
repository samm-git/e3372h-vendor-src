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
/*lint -save -e537*/
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/module.h>
#include <linux/clk.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/debugfs.h>
#include <linux/seq_file.h>

#include "product_config.h"
#include "bsp_ipc.h"
#include "bsp_version.h"
#include "bsp_om.h"
#include "bsp_reg_def.h"
#include "ios_list.h"
#include "soc_memmap.h"


#if defined(BSP_CONFIG_P531_ASIC)
#include "ios_drv_macro.h"

#elif (defined(BSP_CONFIG_V7R2_SFT) || defined(BSP_CONFIG_V7R2_ASIC))
#include "ios_ao_drv_macro.h"
#include "ios_pd_drv_macro.h"

#endif
/*lint -restore*/

#define  ios_print_error(fmt, ...)    (bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_GPIO, "[ios]: <%s> <%d> "fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__))
#define  ios_print_info(fmt, ...)     (bsp_trace(BSP_LOG_LEVEL_ERROR,  BSP_MODU_GPIO, "[ios]: "fmt, ##__VA_ARGS__))

#ifdef __cplusplus
extern "C" {
#endif

static void mmc0_to_gpio_save(void)
{
#ifdef BSP_CONFIG_V7R2_ASIC
	u32 product_type = bsp_version_get_board_type();

    if(HW_VER_PRODUCT_UDP ==product_type)
    {
/*配置MMC0（6个PIN）（SD MASTER/SDIO SLAVE）*/
    /*gpio1[7]管脚复用配置保存*/
    add_ios_list(IOS_PD_MF_CTRL1);
    add_ios_list(IOS_PD_AF_CTRL3);
    /*gpio1[7]管脚Drive电流配置保存*/
    add_ios_list(IOS_PD_IOM_CTRL21);

    /*gpio1[8]管脚复用配置保存*/
    add_ios_list(IOS_PD_MF_CTRL1);
    add_ios_list(IOS_PD_AF_CTRL3);
    /*gpio1[8]管脚上下拉配置保存*/
    add_ios_list(IOS_PD_IOM_CTRL22);

    /*gpio1[9]管脚复用配置保存*/
    add_ios_list(IOS_PD_MF_CTRL1);
    add_ios_list(IOS_PD_AF_CTRL3);
    /*gpio1[9]管脚上下拉配置保存*/
    add_ios_list(IOS_PD_IOM_CTRL23);

    /*gpio1[10]管脚复用配置保存*/
    add_ios_list(IOS_PD_MF_CTRL1);
    add_ios_list(IOS_PD_AF_CTRL3);
    /*gpio1[10]管脚上下拉配置保存*/
    add_ios_list(IOS_PD_IOM_CTRL24);

    /*gpio1[11]管脚复用配置保存*/
    add_ios_list(IOS_PD_MF_CTRL1);
    add_ios_list(IOS_PD_AF_CTRL3);
    /*gpio1[11]管脚上下拉配置保存*/
    add_ios_list(IOS_PD_IOM_CTRL25);

    /*gpio1[12]管脚复用配置保存*/
    add_ios_list(IOS_PD_MF_CTRL1);
    add_ios_list(IOS_PD_AF_CTRL3);
    /*gpio1[12]管脚上下拉配置保存*/
    add_ios_list(IOS_PD_IOM_CTRL26);
	}
#endif

}

void mmc0_to_gpio_mux(void)
{
#ifdef BSP_CONFIG_V7R2_ASIC
	u32 product_type = bsp_version_get_board_type();

    if(HW_VER_PRODUCT_UDP ==product_type)
    {        
    	gpio_request(GPIO_1_7, "GPIO_1_7");
    	gpio_direction_input(GPIO_1_7);
    	gpio_free(GPIO_1_7);

        gpio_request(GPIO_1_8, "GPIO_1_8");
    	gpio_direction_input(GPIO_1_8);
    	gpio_free(GPIO_1_8);
        
        gpio_request(GPIO_1_9, "GPIO_1_9");
    	gpio_direction_input(GPIO_1_9);
    	gpio_free(GPIO_1_9);

      	gpio_request(GPIO_1_10, "GPIO_1_10");
    	gpio_direction_input(GPIO_1_10);
    	gpio_free(GPIO_1_10);

    	gpio_request(GPIO_1_11, "GPIO_1_11");
    	gpio_direction_input(GPIO_1_11);
    	gpio_free(GPIO_1_11);

    	gpio_request(GPIO_1_12, "GPIO_1_12");
    	gpio_direction_input(GPIO_1_12);
    	gpio_free(GPIO_1_12);
        
        bsp_ipc_spin_lock(IPC_SEM_GPIO);
        
    	/*gpio1[7]管脚复用配置*/
    	SET_IOS_GPIO1_7_CTRL1_1;
    	CLR_IOS_MMC0_CLK_CTRL1_1;
    	CLR_IOS_JTAG1_CTRL1_1;
    	CLR_IOS_PCM_CTRL2_2;
    	/*gpio1[7]管脚Drive电流配置*/
    	PDSET_IOS_PD_IOM_CTRL21;

    	/*gpio1[8]管脚复用配置*/
    	SET_IOS_GPIO1_8_CTRL1_1;
    	CLR_IOS_MMC0_CTRL1_1;
    	CLR_IOS_JTAG1_CTRL1_1;
    	CLR_IOS_PCM_CTRL2_2;
    	/*gpio1[8]管脚上下拉配置*/
    	PDSET_IOS_PD_IOM_CTRL22;    	

    	/*gpio1[9]管脚复用配置*/
    	SET_IOS_GPIO1_9_CTRL1_1;
    	CLR_IOS_MMC0_CTRL1_1;
    	CLR_IOS_JTAG1_CTRL1_1;
    	CLR_IOS_PCM_CTRL2_2;
    	/*gpio1[9]管脚上下拉配置*/
    	PDSET_IOS_PD_IOM_CTRL23;    	

    	/*gpio1[10]管脚复用配置*/
    	SET_IOS_GPIO1_10_CTRL1_1;
    	CLR_IOS_MMC0_CTRL1_1;
    	CLR_IOS_JTAG1_CTRL1_1;
    	CLR_IOS_PCM_CTRL2_2;
    	/*gpio1[10]管脚上下拉配置*/
    	PDSET_IOS_PD_IOM_CTRL24;

    	/*gpio1[11]管脚复用配置*/
    	SET_IOS_GPIO1_11_CTRL1_1;
    	CLR_IOS_MMC0_CTRL1_1;
    	CLR_IOS_JTAG1_CTRL1_1;
    	/*gpio1[11]管脚上下拉配置*/
    	PDSET_IOS_PD_IOM_CTRL25;

    	/*gpio1[12]管脚复用配置*/
    	SET_IOS_GPIO1_12_CTRL1_1;
    	CLR_IOS_MMC0_CTRL1_1;
    	CLR_IOS_JTAG1_CTRL1_1;
    	/*gpio1[12]管脚上下拉配置*/
    	PDSET_IOS_PD_IOM_CTRL26;
        
        mmc0_to_gpio_save();
        
		bsp_ipc_spin_unlock(IPC_SEM_GPIO);

    }
#elif defined(BSP_CONFIG_V711_ASIC) || defined(BSP_CONFIG_V711_PORTING)
	mmc0_to_gpio_save();
#endif


}

void gpio_to_mmc0_mux(void)
{
#ifdef BSP_CONFIG_V7R2_ASIC
	u32 product_type = bsp_version_get_board_type();
    if(HW_VER_PRODUCT_UDP == product_type)
    {
        bsp_ipc_spin_lock(IPC_SEM_GPIO);
        
    	PUSET_IOS_PD_IOM_CTRL22;
    	PUSET_IOS_PD_IOM_CTRL23;
    	PUSET_IOS_PD_IOM_CTRL24;
    	PUSET_IOS_PD_IOM_CTRL25;
    	PUSET_IOS_PD_IOM_CTRL26;

    	SET_IOS_MMC0_CLK_CTRL1_1;
    	SET_IOS_MMC0_CTRL1_1;
        
    	CLR_IOS_GPIO1_7_CTRL1_1;
    	CLR_IOS_GPIO1_8_CTRL1_1;
    	CLR_IOS_GPIO1_9_CTRL1_1;
    	CLR_IOS_GPIO1_10_CTRL1_1;
    	CLR_IOS_GPIO1_11_CTRL1_1;
    	CLR_IOS_GPIO1_12_CTRL1_1;

        bsp_ipc_spin_unlock(IPC_SEM_GPIO);

    }
#endif

}

MODULE_AUTHOR("l00225826@huawei.com");
MODULE_DESCRIPTION("HIS Balong V7R2 IO PAD");
MODULE_LICENSE("GPL");

#ifdef __cplusplus
}
#endif


