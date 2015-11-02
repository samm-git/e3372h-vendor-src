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
#ifndef	__BALONG_GPIO_H
#define	__BALONG_GPIO_H
#ifdef __cplusplus
extern "C"
{
#endif

#include <linux/types.h>
#include "bsp_memmap.h"
#include "hi_gpio.h"


#define gpio_get_value	__gpio_get_value
#define gpio_set_value	__gpio_set_value
#define gpio_cansleep	__gpio_cansleep
#define gpio_to_irq		__gpio_to_irq

#define GPIO_OK        0
#define GPIO_ERROR     -1

#define ARCH_NR_GPIOS     (GPIO_TOTAL_PINS_NUM)

/* GPIO TLMM: Function -- GPIO specific */

/* GPIO TLMM: Direction */
enum {
	GPIO_INPUT = 0,
	GPIO_OUTPUT = 1,
};

/* GPIO TLMM: Pullup/Pulldown */
enum {
	GPIO_NO_PULL,
	GPIO_PULL_DOWN,
	GPIO_KEEPER,
	GPIO_PULL_UP,
};

/*GPIO function*/
enum{
	GPIO_NORMAL= 0,
	GPIO_INTERRUPT = 1,
};

/*interrupt mask*/
enum {
	GPIO_INT_ENABLE = 0,
	GPIO_INT_DISABLE = 1,
};

/*interrupt level trigger*/
enum{
	GPIO_INT_TYPE_LEVEVL = 0,
	GPIO_INT_TYPE_EDGE = 1,
};

/*interrupt polarity*/
enum{
	GPIO_INT_POLARITY_FAL_LOW = 0,
	GPIO_INT_POLARITY_RIS_HIGH = 1,
};

/*************************************************
  Function:       gpio_direction_get
  Description:    Get GPIO Direction value,
  				  include GPIO_INPUT and GPIO_OUTPUT
  Calls:
  Called By:
  Table Accessed: NONE
  Table Updated:  NONE
  Input:          unsigned gpio, such as GPIO_*_**

  Return:         GPIO_INPUT, GPIO_OUTPUT
  Others:
************************************************/
extern int gpio_direction_get(unsigned gpio);

/*************************************************
  Function:       gpio_set_function
  Description:    Set GPIO function
  Calls:
  Called By:
  Table Accessed: NONE
  Table Updated:  NONE
  Input:          unsigned gpio, int mode, include
  	              GPIO_NORMAL,
				  GPIO_INTERRUPT,

  Return:         NONE
  Others:
************************************************/
extern void gpio_set_function(unsigned gpio, int mode);

/*************************************************
  Function:       gpio_int_mask_set
  Description:    Mask GPIO interrupt
  Calls:
  Called By:
  Table Accessed: NONE
  Table Updated:  NONE
  Input:          unsigned gpio, include GPIO_INT_ENABLE
  				  and GPIO_INT_DISABLE
  Return:         NONE
  Others:mask bit refer to GPIO_INT_ENABLE, GPIO_INT_DISABLE
************************************************/
extern void gpio_int_mask_set(unsigned gpio);

/*************************************************
  Function:       gpio_int_unmask_set
  Description:    unMask GPIO interrupt
  Called By:
  Table Accessed: NONE
  Table Updated:  NONE
  Input:          unsigned gpio

  Return:        NONE
  Others:mask bit refer to GPIO_INT_ENABLE, GPIO_INT_DISABLE
************************************************/
extern void gpio_int_unmask_set(unsigned gpio);

/*************************************************
  Function:       gpio_int_state_clear
  Description:    get GPIO interrupt status
  Calls:
  Called By:
  Table Accessed: NONE
  Table Updated:  NONE
  Input:          unsigned gpio

  Return:         NONE
  Others:
************************************************/
extern void gpio_int_state_clear(unsigned gpio);

/*************************************************
  Function:       gpio_int_state_get
  Description:    get GPIO interrupt  status
  Calls:
  Called By:
  Table Accessed: NONE
  Table Updated:  NONE
  Input:          unsigned gpio

  Return:         NONE
  Others:
************************************************/
extern int gpio_int_state_get(unsigned gpio);

/*************************************************
  Function:       gpio_raw_int_state_get
  Description:    Clear GPIO raw interrupt
  Calls:
  Called By:
  Table Accessed: NONE
  Table Updated:  NONE
  Input:          unsigned gpio

  Return:         NONE
  Others:clear refer to
************************************************/
extern int gpio_raw_int_state_get(unsigned gpio);

/*************************************************
  Function:       gpio_int_trigger_set
  Description:    set GPIO Interrupt Triger style
  Calls:
  Called By:
  Table Accessed: NONE
  Table Updated:  NONE
  Input:          unsigned gpio, int trigger_type
				  IRQ_TYPE_EDGE_RISING,
				  IRQ_TYPE_EDGE_FALLING,
				  IRQ_TYPE_LEVEL_HIGH,
				  IRQ_TYPE_LEVEL_LOW
  Output:         NONE
  Return:         OSAL_OK : successfully
                  OSAL_ERROR: fail
  Others:
************************************************/
extern void gpio_int_trigger_set(unsigned gpio, int trigger_type);

#include <asm-generic/gpio.h>
#ifdef __cplusplus
} /* allow C++ to use these headers */
#endif /* __cplusplus */

#endif
