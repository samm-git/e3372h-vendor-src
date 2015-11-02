#ifndef REB_TASK_H
#define REB_TASK_H
/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2013-2015. All rights reserved.
 *
 * mobile@huawei.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
/******************************问题单修改记录**********************************
    日期          修改人              问题单号                   修改内容
******************************************************************************/
/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

void hard_timer_creat(unsigned int time_in_ms, unsigned int input_para );
void hard_timer_reset( unsigned int time_in_ms );
void hard_timer_delete(void);
int reb_nv_kernel_read(void);
void reb_for_power_off(void);
int reb_is_factory_mode(void);
unsigned long  reb_get_current_systime(void);
#endif
