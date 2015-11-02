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
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/gpio.h>
#include <linux/i2c/pca953x.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include "bsp_om.h"
//#include "bsp_hardtimer.h"

#define GPIO_I2C_EXPANDER_ADP 0
#define GPIO_I2C_EXPANDER_NUM 1

#define  gpio_print_error(fmt, ...)    (bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_GPIO, "[gpio_ex]: <%s> <%d> "fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__))
#define  gpio_print_info(fmt, ...)     (bsp_trace(BSP_LOG_LEVEL_ERROR,  BSP_MODU_GPIO, "[gpio_ex]: "fmt, ##__VA_ARGS__))

int	gpio_i2c_expander_setup(struct i2c_client *client, unsigned gpio, unsigned ngpio, void *context);
int	gpio_i2c_expander_teardown(struct i2c_client *client, unsigned gpio, unsigned ngpio, void *context);

static struct pca953x_platform_data gpio_i2c_expander_data[GPIO_I2C_EXPANDER_NUM] = {
		{
        	.gpio_base = 128,
        	.invert = 0,
        	.irq_base = -1,
        	.context = NULL,
        	.setup = gpio_i2c_expander_setup,
        	.teardown = gpio_i2c_expander_teardown,
		},
#if 0 
		{/*v711 UDP E5*/
        	.gpio_base = 136,
        	.invert = 0,
        	.irq_base = -1,
        	.context = NULL,
        	.setup = gpio_i2c_expander_setup,
        	.teardown = gpio_i2c_expander_teardown,
		},
		{  /* V711 E5 */
        	.gpio_base = 144,
        	.invert = 0,
        	.irq_base = -1,
        	.context = NULL,
        	.setup = gpio_i2c_expander_setup,
        	.teardown = gpio_i2c_expander_teardown,
		},
#endif

};

static struct i2c_board_info expander_board_info[GPIO_I2C_EXPANDER_NUM] = {
		{
        	.type = "pca9574",
        	.addr = 0x20,
        	.platform_data = &gpio_i2c_expander_data[0],
		},
#if 0 
		{/* v711 UDP E5*/
        	.type = "pca9574",
        	.addr = 0x21,
        	.platform_data = &gpio_i2c_expander_data[1],
		},
		{/*v711 E5*/
        	.type = "pca9555",
        	.addr = 0x24,
        	.platform_data = &gpio_i2c_expander_data[2],
		},
#endif
};

struct i2c_client *gpio_expander_client[GPIO_I2C_EXPANDER_NUM] = {0};

int	gpio_i2c_expander_setup(struct i2c_client *client, unsigned gpio, unsigned ngpio, void *context)
{
    int ret = 0;
    unsigned int i = 0;

    for(i = 0;i < ngpio;i++)
    {
        ret = gpio_request(gpio + i, "gpio_expander");
        if(ret < 0)
        {
            gpio_print_error( "gpio%d request is fail, ret = %d.\n", gpio + i, ret);
            return -1;
        }

    }
    
    return 0;
    
}

int	gpio_i2c_expander_teardown(struct i2c_client *client, unsigned gpio, unsigned ngpio, void *context)
{
    unsigned int i = 0;

    for(i = 0;i < ngpio;i++)
    {
        gpio_free(gpio + i);
    }

    return 0;
    
}

/* read gpio expander reg */
int gpio_expander_register_debug(unsigned int num, int reg)
{
	int ret;

    struct i2c_client *client = NULL;

    if(num >= GPIO_I2C_EXPANDER_NUM || (reg >= 8))
    {
        gpio_print_error("para is error, num = %d, reg = 0x%x.\n", num, reg);
        return -1;
    }

    client = gpio_expander_client[num];

	if (num < 2)
		ret = i2c_smbus_read_byte_data(client, reg);
	else
		ret = i2c_smbus_read_word_data(client, reg << 1);

	if (ret < 0)
    {
		gpio_print_error("failed reading register.\n");
		return ret;
	}

	gpio_print_info("reg(0x%x) is 0x%x.\n", reg, ret);
    
	return 0;
}

/* just for v7r2 test */
/*void gpio_i2c_expander_reset(void)
{

    int ret = 0;
    
    ret = gpio_request(GPIO_2_24, "gpio_expander");
    if(ret < 0)
    {
        gpio_print_error( "gpio_request is fail, ret = %d.\n", ret);
        return ;
    }
    
    gpio_direction_output(GPIO_2_24, 0);
    
    mdelay(10);
    
    gpio_direction_output(GPIO_2_24, 1);
    
}*/

/*i2c gpio expander*/
static int __devinit gpio_i2c_expander_init(void)
{
    unsigned int i = 0;

	struct i2c_adapter *adapter = NULL;

    //gpio_i2c_expander_reset();/* just for v7r2 test*/

	adapter = i2c_get_adapter(GPIO_I2C_EXPANDER_ADP);
	if (!adapter)
    {
		gpio_print_error("i2c_get_adapter failed.\n");
		return -1;
	}
    
    for(i = 0;i < GPIO_I2C_EXPANDER_NUM;i++)
    {
    	gpio_expander_client[i] = i2c_new_device(adapter, &expander_board_info[i]);
    	if (!gpio_expander_client[i]) {
    		gpio_print_error("i2c_new_device failed, i = %d, addr = 0x%x.\n", i, expander_board_info[i].addr);
    		return -1;
    	}
    }

#if 0
    ret = i2c_register_board_info(GPIO_I2C_EXPANDER_ADP, expander_board_info, sizeof(expander_board_info)/sizeof(struct i2c_board_info));
    if(ret < 0)
    {
        gpio_print_error("i2c_register_board_info is fail,ret = %d.\n", ret);
        return -1;
    }
#endif

	gpio_print_info("gpio ex init ok.\n");
    
	return 0;
}


static void __exit gpio_i2c_expander_exit(void)
{

}


subsys_initcall(gpio_i2c_expander_init);
module_exit(gpio_i2c_expander_exit);
MODULE_DESCRIPTION("balong GPIO I2C Expander device.");

