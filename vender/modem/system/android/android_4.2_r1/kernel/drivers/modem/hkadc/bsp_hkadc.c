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
/*lint --e{537} */

#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/delay.h>
#include <linux/sched.h>

#include <osl_types.h>
#include <osl_bio.h>
#include <osl_sem.h>

#include <bsp_om.h>
#include <bsp_icc.h>
#include <bsp_hkadc.h>
#include "bsp_memmap.h" 
#include "drv_temp_cfg.h"
#include "bsp_shared_ddr.h"
#define TEMPERATURE_MAGIC         0x5A5A5A5A

 /******************************问题单修改记录**********************************
    日期       修改人       问题单号                 修改内容
2014-02-13    l00212897    DTS2014021305922        库仑计代码上库
******************************************************************************/


static int bsp_hkadc_acore_init(void);

static u16 g_hkadc_voltage;
static osl_sem_id g_hkadc_icc_sem_id;
static osl_sem_id g_hkadc_value_sem_id;

int bsp_hkadc_convert(enum HKADC_CHANNEL_ID channel, u16* value)
{
    int ret;
    u32 hkadc_channel_id = (u32)channel;
    u32 icc_channel_id = ICC_CHN_MCORE_ACORE << 16 | MCORE_ACORE_FUNC_HKADC;

    osl_sem_down(&g_hkadc_icc_sem_id);

    ret = bsp_icc_send(ICC_CPU_MCU, icc_channel_id,
        (u8*)&hkadc_channel_id, sizeof(hkadc_channel_id));
    if (ret != (int)sizeof(hkadc_channel_id))
    {
        osl_sem_up(&g_hkadc_icc_sem_id);
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_HKADC, "icc send error, error code: 0x%x\r\n", ret);
        return ret;
    }

    /*coverity[lock] */
    osl_sem_down(&g_hkadc_value_sem_id);

    if (0xFFFF == g_hkadc_voltage)
    {
        ret = -1;
    }
    else
    {
        ret = 0;
        *value = g_hkadc_voltage;
    }

    osl_sem_up(&g_hkadc_icc_sem_id);

    /*coverity[missing_unlock] */
    return ret;
}

static int bsp_hkadc_icc_callback(u32 icc_channel_id , u32 len, void* context)
{
    int ret;
    u16 voltage = 0;

    ret = bsp_icc_read(icc_channel_id, (u8*)&voltage, len);
    if (ret != (int)sizeof(voltage))
    {
        g_hkadc_voltage = 0xFFFF;
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_HKADC, "hkadc icc read error, error code: 0x%x\r\n", ret);
    }
    else
    {
        g_hkadc_voltage = voltage;
    }

    osl_sem_up(&g_hkadc_value_sem_id);

    return 0;
}

static int bsp_hkadc_acore_init(void)
{
    int ret = 0;
    u32 icc_channel_id = ICC_CHN_MCORE_ACORE << 16 | MCORE_CCORE_FUNC_HKADC;

    osl_sem_init(1, &g_hkadc_icc_sem_id);
    osl_sem_init(0, &g_hkadc_value_sem_id);

    ret |= bsp_icc_event_register(icc_channel_id,
        (read_cb_func)bsp_hkadc_icc_callback, NULL, NULL, NULL);
    if (ret)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_HKADC, "hkadc init error, error code: 0x%x\r\n", ret);
    }

    return ret;
}

int bsp_hkadc_convert_test(int channel)
{
    int ret;
    u16 value = 0;

    ret = bsp_hkadc_convert((enum HKADC_CHANNEL_ID)channel, &value);
    if (ret)
        printk(KERN_ERR"fail to convert, return value 0x%x\n", ret);
    else
        printk(KERN_ERR"%d, channel %d\n", value, channel);

    return ret;
}

int bsp_hkadc_test(u32 count)
{
    int ret;
    u32 i, channel;

    u16 value = 0;

    for (i = 0; i <= count; i++)
    {
        for (channel = (u32)HKADC_CHANNEL_MIN; channel <= (u32)HKADC_CHANNEL_MAX; channel++)
        {
            ret = bsp_hkadc_convert((enum HKADC_CHANNEL_ID)channel, &value);
            if (ret)
                return ret;
            else
                printk(KERN_ERR"%d\n", value);
        }
        printk(KERN_ERR"\n");
    }
    return 0;
}

short bsp_hkadc_get_temp(int chan)
{
    short temp = 0;
    DRV_HKADC_DATA_AREA *p_area = (DRV_HKADC_DATA_AREA *)SHM_MEM_TEMPERATURE_ADDR;
    if(NULL == p_area)
    {
        pr_err("p_area is NULL!!!\n");
        return -1;
    }
    if(TEMPERATURE_MAGIC != p_area->magic_start 
        || (TEMPERATURE_MAGIC != p_area->magic_end))
    {
        pr_err("temp mem is writed by others.\n");
        return -1;
    }
    if((chan > 11) || (chan < 0))
    {
        pr_err("hkadc channel %d is error!!\n", chan);
        return -1;
    }
    temp = p_area->chan_out[chan].temp_l;
    pr_info("The temperature of channel %d is %d'C.\n", chan, temp);
    return temp;
}
short bsp_hkadc_get_volt(int chan)
{
    unsigned short volt = 0;
    DRV_HKADC_DATA_AREA *p_area = (DRV_HKADC_DATA_AREA *)SHM_MEM_TEMPERATURE_ADDR;
    if(NULL == p_area)
    {
        pr_err("p_area is NULL!!!\n");
        return -1;
    }
    if(TEMPERATURE_MAGIC != p_area->magic_start 
        || (TEMPERATURE_MAGIC != p_area->magic_end))
    {
        pr_err("tem mem is writed by others.\n");
        return -1;
    }
    if((chan > 11) || (chan < 0))
    {
        pr_err("hkadc channel %d is error!!\n", chan);
        return -1;
    }
    volt = p_area->chan_out[chan].volt_l;
    pr_info("The voltage of channel %d is %dmV.\n", chan, volt);
    return volt;
}
#if (MBB_COMMON == FEATURE_ON)
fs_initcall(bsp_hkadc_acore_init);
#else
module_init(bsp_hkadc_acore_init);
#endif
MODULE_AUTHOR("z00227143@huawei.com");
MODULE_DESCRIPTION("HIS Balong V7R2 HKADC");
MODULE_LICENSE("GPL");


