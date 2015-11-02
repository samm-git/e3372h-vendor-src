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




#ifndef __RTC_BALONG_H__
#define __RTC_BALONG_H__

#include <bsp_om.h>
#include <hi_bbstar.h>
#include <bsp_pmu.h>
#include <product_config.h>
#include <linux/rtc.h>



#define ALARM_ENABLE   (1)
#define ALARM_DISABLED (0)

#define RTC_INT_ID      36
#define RTC_BASE_YEAR   1900 /*RTC base year*/
#define HI6559_RTC_DEVICE_NUM   6
#define RTC_OK          0
#define RTC_ERROR       -1

struct hi6559_alarm_para
{
    s32 alarm_id;
    struct rtc_device *rtc_dev;
};

/*RTC 全局变量结构体*/
struct rtc_control
{
    u16 rtc_cur_addr;               /*当前值寄存器*/
    u16 rtc_clr_addr;               /*加载值寄存器*/
    u32 alarm_irq[HI6559_RTC_DEVICE_NUM];
    u16 alarm_base_addr[HI6559_RTC_DEVICE_NUM];
};

#define  hi6559_rtc_print_error(fmt,...)      (bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_HI6559_RTC, "[hi6559_rtc]: <%s> <%d>"fmt"\n", __FUNCTION__, __LINE__, ##__VA_ARGS__))
#define  hi6559_rtc_print_dbg(fmt,...)		       (bsp_trace(BSP_LOG_LEVEL_DEBUG, BSP_MODU_HI6559_RTC, "[hi6559_rtc]: <%s> <%d>"fmt"\n", __FUNCTION__, __LINE__, ##__VA_ARGS__))


int hi6559_rtc_readtime(struct device *dev, struct rtc_time *tm);
int hi6559_rtc_settime(struct device *dev, struct rtc_time *tm);
int hi6559_alarm_irq_enable(struct device *dev, unsigned int enabled);
int hi6559_rtc_readalarm(struct device *dev, struct rtc_wkalrm *alarm);
int hi6559_rtc_setalarm(struct device *dev, struct rtc_wkalrm *alarm);
void hi6559_rtc_alarmhandler(void *data);
u32 hi6559_get_rtc_value (void);
u32 hi6559_rtc_write_reg(u16 addr, u32 pValue);
u32 hi6559_rtc_read_reg(u16 addr);
u32 hi6559_rtc_show_level(void);
#endif
