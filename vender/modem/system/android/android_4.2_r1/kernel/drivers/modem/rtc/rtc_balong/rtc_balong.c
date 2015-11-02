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



/*lint -save -e537 */
#include <linux/platform_device.h>
#include <linux/rtc.h>
#include <linux/module.h>
#include <osl_bio.h>
#include <hi_rtc.h>
#include <drv_comm.h>
#include <bsp_memmap.h>
#include <bsp_icc.h>
#include "rtc_balong.h"

struct rtc_control g_rtc_ctrl =
{
	.rtc_base_addr = HI_RTC_REGBASE_ADDR_VIRT,
};

/*lint -save -e550*/
/**
 * balong_rtc_readtime-read the RTC time
 * @dev:	the rtc device.
 * @tm:		the RTC time pointer which point to the RTC time
 *
 */
s32 balong_rtc_readtime(struct device *dev, struct rtc_time *tm)
{
    rtc_time_to_tm(readl(g_rtc_ctrl.rtc_base_addr + HI_RTC_CCVR_OFFSET), tm);

	dev_dbg(dev, "%s: %4d-%02d-%02d %02d:%02d:%02d\n", "readtime",
		1900 + tm->tm_year, tm->tm_mon, tm->tm_mday,
		tm->tm_hour, tm->tm_min, tm->tm_sec);

	return BSP_OK;
}
/**
 * balong_rtc_settime-set the RTC time
 * @dev:	the RTC device.
 * @tm:		the RTC time which will be set
 *
 */
s32 balong_rtc_settime(struct device *dev, struct rtc_time *tm)
{
    u32 secs = 0;
    s32  ret = 0;
    u32 alarmtime = 0;
    u32 alarmenable = 0;
    u32 curtime = 0;/*当前的时间值*/
    ret  = rtc_valid_tm(tm);
    if (0 != ret)
    {
        rtc_print_error("RTC: rtc_valid_tm error!\n");
        return BSP_ERROR;
    }

    ret = rtc_tm_to_time(tm, (unsigned long *)&secs);
    if (ret != 0)
         return ret;
    /* store the new base time in rtc register */
    secs += 1;
    curtime = readl(g_rtc_ctrl.rtc_base_addr + HI_RTC_CCVR_OFFSET);
    alarmtime = readl(g_rtc_ctrl.rtc_base_addr + HI_RTC_CMR_OFFSET);
    writel( secs, g_rtc_ctrl.rtc_base_addr + HI_RTC_CLR_OFFSET);

    /*只有在设置了闹钟的时候才进行下面的重新设置闹钟时间动作*/
    alarmenable = readl(g_rtc_ctrl.rtc_base_addr + HI_RTC_CCR_OFFSET)&(RTC_BIT_IM);
    if ((!alarmenable)&&( curtime < alarmtime )) {
        /* disable interrupts */
        balong_alarm_irq_enable(dev, ALARM_DISABLED);

        alarmtime = secs + ( alarmtime - curtime);
        writel( alarmtime, g_rtc_ctrl.rtc_base_addr + HI_RTC_CMR_OFFSET);

        /* enable alarm interrupts */
        balong_alarm_irq_enable(dev, ALARM_ENABLE);
    }
    return BSP_OK;
}

/**
 * balong_alarm_irq_enable-enable or disenable RTC alarm irq
 * @dev:	the RTC device.
 * @enabled:		1: enable the RTC alrm irq
 *					0: disenable the RTC alarm irq
 *
 */
s32 balong_alarm_irq_enable(struct device *dev, unsigned int enabled)
{
	u32 ccr_value = 0;

	/* Clear any pending alarm interrupts. */
	readl(g_rtc_ctrl.rtc_base_addr + HI_RTC_EOI_OFFSET);
	ccr_value = readl(g_rtc_ctrl.rtc_base_addr + HI_RTC_CCR_OFFSET);

    /* if interupt enabled,unmask */
    /* if interupt unenabled,mask */
	if (ALARM_DISABLED == enabled)
    {
        writel(  ccr_value | RTC_BIT_IM, g_rtc_ctrl.rtc_base_addr + HI_RTC_CCR_OFFSET);
    }
	else
    {
        writel( ccr_value & (~RTC_BIT_IM), g_rtc_ctrl.rtc_base_addr + HI_RTC_CCR_OFFSET);
    }

	return BSP_OK;
}
/**
 * balong_rtc_readalarm-read rtc alarm
 */
s32 balong_rtc_readalarm(struct device *dev, struct rtc_wkalrm *alarm)
{
	rtc_time_to_tm(readl(g_rtc_ctrl.rtc_base_addr + HI_RTC_CMR_OFFSET), &alarm->time);
    alarm->pending = readl(g_rtc_ctrl.rtc_base_addr + HI_RTC_CCR_OFFSET) & RTC_BIT_AI;
    if(readl(g_rtc_ctrl.rtc_base_addr + HI_RTC_CCR_OFFSET)&RTC_BIT_IM)
    {
        alarm->enabled = ALARM_DISABLED;
    }
    else
    {
        alarm->enabled = ALARM_ENABLE;
    }

	return BSP_OK;
}
/**
 * balong_rtc_setalarm-set rtc alarm
 */
s32 balong_rtc_setalarm(struct device *dev, struct rtc_wkalrm *alarm)
{
    s32 ret = 0;
    u32 alarmtime = 0;
    u32 currenttime = 0;
    u32 ccr_value = 0;

    balong_alarm_irq_enable(dev, ALARM_DISABLED);

    ret = rtc_valid_tm(&alarm->time);
    if (0 != ret)
    {
	    rtc_print_error("balong rtc_valid_tm Fail!\n");
        return BSP_ERROR;
    }

    ret = rtc_tm_to_time(&alarm->time, (unsigned long *)&alarmtime);
    if (0 != ret)
    {
	    rtc_print_error("balong rtc_tm_to_time Fail!\n");
        return BSP_ERROR;
    }

    currenttime = readl(g_rtc_ctrl.rtc_base_addr + HI_RTC_CCVR_OFFSET);
    if(currenttime <= alarmtime)
    {
        writel( alarmtime, g_rtc_ctrl.rtc_base_addr + HI_RTC_CMR_OFFSET);
    }
    else
    {
        rtc_print_error("the alarm time to be set is error.\r\n");
    }

    ccr_value = readl(g_rtc_ctrl.rtc_base_addr + HI_RTC_CCR_OFFSET);
    writel( ccr_value|RTC_BIT_AI, g_rtc_ctrl.rtc_base_addr + HI_RTC_CCR_OFFSET);

    balong_alarm_irq_enable(dev, alarm->enabled);

	return BSP_OK;
}
/**
 * balong_rtc_alarmhandler-the rtc irq handler function
 */
s32  balong_rtc_alarmhandler(int irq, void *data)
{
	u32 int_status = 0;
    u32 events = 0;
	struct rtc_device *rtc = (struct rtc_device *)data;


	int_status = readl(g_rtc_ctrl.rtc_base_addr + HI_RTC_STAT_OFFSET);
    /*清中断*/
    readl(g_rtc_ctrl.rtc_base_addr + HI_RTC_EOI_OFFSET);

	if (!int_status)
    {
		return IRQ_NONE;
	}
	events = RTC_IRQF | RTC_AF;
	rtc_update_irq(rtc, 1, events);
    rtc_dbg("=========alram is handled==========\r\n");
	return IRQ_HANDLED;
}

/*RTC操作函数数据结构*/
static const struct rtc_class_ops balong_rtc_ops = {
	.read_time	= balong_rtc_readtime,
	.set_time	= balong_rtc_settime,
	.read_alarm	= balong_rtc_readalarm,
	.set_alarm	= balong_rtc_setalarm,
	.alarm_irq_enable = balong_alarm_irq_enable,
};

/**
 * balong_rtc_cb_settime-the icc callback function when ccore set the time through icc
 * @channel_id:	the icc channel id.
 * @len:		the type of the container struct this is embedded in.
 * @context:	the parameter pass from the register
 *
 */
static s32 balong_rtc_cb_settime(u32 channel_id , u32 len, void* context)
{
	s32 ret = 0;
	struct rtc_time tm = {0};
    rtc_dbg("balong_rtc_cb_settime is actived\r\n");
	ret = bsp_icc_read(channel_id, (u8*)&tm, len);
    rtc_dbg("%4d-%02d-%02d %02d:%02d:%02d\n",
            RTC_BASE_YEAR + tm.tm_year, tm.tm_mon, tm.tm_mday,
            tm.tm_hour, tm.tm_min, tm.tm_sec);
	if(len != (u32)ret)
	{
		rtc_print_error("balong_rtc_cb_settime error \r\n");
		return BSP_ERROR;
	}
	return balong_rtc_settime(NULL, &tm);
}

static s32 __devinit  balong_rtc_probe(struct platform_device *pdev)
{

    struct rtc_device *balongv7r2_rtc;
    u32 channel_id_set = ICC_CHN_IFC << 16 | IFC_RECV_FUNC_RTC_SETTIME;
    s32 ret = 0;

	balongv7r2_rtc = rtc_device_register("balongv7r2_rtc", &pdev->dev, &balong_rtc_ops,
			THIS_MODULE);
	if (IS_ERR(balongv7r2_rtc))
    {
		ret = PTR_ERR(balongv7r2_rtc);
		goto cleanup0;
	}
	platform_set_drvdata(pdev, balongv7r2_rtc);

	/* 清中断 */
	readl(g_rtc_ctrl.rtc_base_addr + HI_RTC_EOI_OFFSET);

	/*disable the alarm interupt */
	writel( 0, g_rtc_ctrl.rtc_base_addr + HI_RTC_CCR_OFFSET);
    ret = request_irq(RTC_INT_ID, (irq_handler_t)balong_rtc_alarmhandler, IRQF_DISABLED, "balongv7r2_rtc", (void *)balongv7r2_rtc);
    if (ret != 0)
		goto cleanup1;

    ret = bsp_icc_event_register(channel_id_set, (read_cb_func)balong_rtc_cb_settime, (void *)NULL, (write_cb_func)NULL, (void *)NULL);
    if(0 != ret)
		goto cleanup2;
    bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RTC,"RTC init OK!\r\n");
    return BSP_OK;

cleanup2:
	rtc_print_error("rtc register icc fail\r\n");
	rtc_device_unregister(balongv7r2_rtc);
    free_irq(RTC_INT_ID, balongv7r2_rtc);
    balongv7r2_rtc = NULL;
cleanup1:
	rtc_print_error("rtc request_irq fail\r\n");
	rtc_device_unregister(balongv7r2_rtc);
    balongv7r2_rtc = NULL;
cleanup0:
    rtc_print_error("rtc device register fail\r\n");
    balongv7r2_rtc = NULL;
    /*lint -save -e438*/
    return ret;
    /*lint -restore*/
}

static s32  balong_rtc_remove(struct platform_device *pdev)
{
    /*lint -save -e438*/
    u32 channel_id_set = ICC_CHN_IFC << 16 | IFC_RECV_FUNC_RTC_SETTIME;
	struct rtc_device *balongv7r2_rtc = (struct rtc_device *)platform_get_drvdata(pdev);

    bsp_icc_event_unregister(channel_id_set);
    free_irq(RTC_INT_ID, balongv7r2_rtc);
    rtc_device_unregister(balongv7r2_rtc);
    balongv7r2_rtc = NULL;

	return BSP_OK;
    /*lint -restore*/
}
static struct platform_device balong_rtc_device = {
        .name           = "balongv7r2_rtc"
};

static struct platform_driver balong_rtc_driver = {
	.driver = {
		.name = "balongv7r2_rtc",
		.owner = THIS_MODULE,
	},
	.probe	= balong_rtc_probe,
	.remove = __devexit_p(balong_rtc_remove),
};
s32 __init balong_rtc_init (void)
{
    int ret = 0;
    ret = platform_device_register(&balong_rtc_device);
    if(ret)
    {
        platform_device_unregister(&balong_rtc_device);
    }
    return platform_driver_register(&balong_rtc_driver);
}
static void __exit balong_rtc_exit(void)
{
	platform_device_unregister(&balong_rtc_device);
	platform_driver_unregister(&balong_rtc_driver);
}
/*
* adapt v9r1
*/
u32 balong_get_rtc_value (void)
{
    u32 rtc_value = 0;
	rtc_value = readl(g_rtc_ctrl.rtc_base_addr + HI_RTC_CCVR_OFFSET);
    return rtc_value;
}
/*lint -restore +e537*/
/*lint -save -e19*/
EXPORT_SYMBOL(balong_get_rtc_value);
module_init(balong_rtc_init);
module_exit(balong_rtc_exit);
MODULE_AUTHOR("HI6930 RTC Device Driver");
MODULE_DESCRIPTION("HI6930 RTC Driver");
/*lint -restore +e550 +e19*/

