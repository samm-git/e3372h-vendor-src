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
#ifndef __HI6551_COUL_H__
#define __HI6551_COUL_H__

#include "bsp_om.h"

typedef enum{
    BATT_CAPACITY = 0,
    BATT_VOLTAGE,
    BATT_CURRENT,
    BATT_VOLT_CALI,
    BATT_CHARGE_TIME,
    BATT_DISCHARGE_TIME,
    BATT_INFO_BUILT
}COUL_BATT_INFO;

typedef enum{
    COUL_POWER_OFF = 0,
    COUL_POWER_ON,
    COUL_POWER_BUILT
}COUL_POWER_CTRL;

typedef enum{
    COUL_REFLASH_ALL = 0,
    COUL_REFLASH_ECO,
    COUL_REFLASH_BUILT
}COUL_REFLASH_CTRL;

typedef enum{
    COUL_ECO_EXIT = 0,
    COUL_ECO_FORCE,
    COUL_ECO_BUILT
}COUL_ECO_CTRL;

typedef enum{
    COUL_CALI_OFF = 0,
    COUL_CALI_ON,
    COUL_CALI_BUILT
}COUL_CALI_CTRL;

#define COUL_CTRL_ONOFF_MASK  (1<<7)
#define COUL_CTRL_CALI_MASK   (1<<6)
#define COUL_CTRL_REFLASH_MASK (1<<3)
#define COUL_CTRL_ECO_MASK      (1<<2)
#define COUL_CTRL_FILETR_MASK        (3<<4)/*bit4,bit5*/
#define COUL_CTRL_FILETR_OFFSET        4/*bit4,bit5*/

typedef unsigned long coul_irqflags_t;

#define coul_dbg(fmt, ...)    (bsp_trace(BSP_LOG_LEVEL_DEBUG, BSP_MODU_COUL, "[coul]: <%s> "fmt"", __FUNCTION__, ##__VA_ARGS__))
#define coul_err(fmt, ...)    (bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_COUL, "[coul]: <%s> "fmt"", __FUNCTION__, ##__VA_ARGS__))
#define coul_info(fmt, ...)   (bsp_trace(BSP_LOG_LEVEL_INFO, BSP_MODU_COUL, "[coul]: <%s> "fmt"", __FUNCTION__, ##__VA_ARGS__))

#endif
