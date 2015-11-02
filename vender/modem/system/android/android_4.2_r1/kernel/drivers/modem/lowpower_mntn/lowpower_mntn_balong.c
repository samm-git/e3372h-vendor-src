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
#include <linux/string.h>
#include <linux/suspend.h>
#include <bsp_lowpower_mntn.h>
#include <bsp_hardtimer.h>
#include <bsp_dump.h>

#define  mntn_printf(fmt, ...)    (bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_WAKELOCK, "[Lowpower_mntn]: <%s> "fmt"", __FUNCTION__, ##__VA_ARGS__))
struct notifier_block modem_lowpower_mntn_notify = {0};
#ifdef CONFIG_HISI_BALONG_MODEM
struct lower_power_pmntn_ctrl{
	void __iomem *pmctrl_base;
	void __iomem *pericrg_base;
	}lp_ctrl;
#endif
/*记录低功耗共享地址*/
unsigned int g_lowpower_shared_addr = 0;


/* STAMP */
#define STAMP_START_ADDR            (0+16)
#define PM_ENTER_COUNT              (4+STAMP_START_ADDR)
#define PM_DPM_FAIL_COUNT           (4+PM_ENTER_COUNT)
/*start form pm_suspend()*/
#define STAMP_PM_SUSPEND_START      (4+PM_DPM_FAIL_COUNT)
#define STAMP_DPM_SUSPEND_FAIL      (4+STAMP_PM_SUSPEND_START)
#define STAMP_AFTER_DPM_SUSPEND     (4+STAMP_DPM_SUSPEND_FAIL)  /* dpm_suspend may fail and goto resume without being stamped.*/
#define STAMP_AFTER_UART_SUSPEND    (4+STAMP_AFTER_DPM_SUSPEND)
/* pm enter */
#define STAMP_PM_ENTER_START        (4+STAMP_AFTER_UART_SUSPEND)
#define STAMP_AFTER_DISABLE_GIC     (4+STAMP_PM_ENTER_START)
#define STAMP_AFTER_BAK_GIC         (4+STAMP_AFTER_DISABLE_GIC)
#define STAMP_AFTER_UTRACE_SUSPEND  (4+STAMP_AFTER_BAK_GIC)
#define STAMP_AFTER_TCXO_SUSPEND    (4+STAMP_AFTER_UTRACE_SUSPEND)
#define STAMP_AFTER_PIN_POWERDOWN   (4+STAMP_AFTER_TCXO_SUSPEND)

/*in sleep ASM power down*/
#define STAMP_SLEEP_ASM_ENTER       (4+STAMP_AFTER_PIN_POWERDOWN)
#define STAMP_BAK_COREG_BEGIN       (4+STAMP_SLEEP_ASM_ENTER)
#define STAMP_BAK_COREG_END         (4+STAMP_BAK_COREG_BEGIN)
#define STAMP_BAK_MMUREG_BEGIN      (4+STAMP_BAK_COREG_END)
#define STAMP_BAK_MMUREG_END        (4+STAMP_BAK_MMUREG_BEGIN)
#define STAMP_BEFORE_SEND_IPC       (4+STAMP_BAK_MMUREG_END)
#define STAMP_AFTER_SEND_IPC        (4+STAMP_BEFORE_SEND_IPC)

#define STAMP_AFTER_WFI_NOP         (4+STAMP_AFTER_SEND_IPC)   /* after wfi, should not be stamped */

/*in sleep ASM power up*/
#define STAMP_PWRUP_CODE_BEGIN      (4+STAMP_AFTER_WFI_NOP)
#define STAMP_RSTR_MMUREG_BEGIN     (4+STAMP_PWRUP_CODE_BEGIN)
#define STAMP_RSTR_MMUREG_END       (4+STAMP_RSTR_MMUREG_BEGIN)
#define STAMP_RSTR_COREG_BEGIN      (4+STAMP_RSTR_MMUREG_END)
#define STAMP_RSTR_COREG_END        (4+STAMP_RSTR_COREG_BEGIN)
/* out from sleep ASM,in pm enter */
#define STAMP_SLEEP_ASM_OUT         (4+STAMP_RSTR_COREG_END)
#define STAMP_AFTER_PIN_NORMAL      (4+STAMP_SLEEP_ASM_OUT)
#define STAMP_AFTER_TCXO_RESUME     (4+STAMP_AFTER_PIN_NORMAL)
#define STAMP_AFTER_UTRACE_RESUME   (4+STAMP_AFTER_TCXO_RESUME)
#define STAMP_AFTER_RSTR_GIC        (4+STAMP_AFTER_UTRACE_RESUME)
#define STAMP_AFTER_ENABLE_GIC      (4+STAMP_AFTER_RSTR_GIC)
#define STAMP_PM_ENTER_END          (4+STAMP_AFTER_ENABLE_GIC)
/* out from pm enter, in pm suspend*/
#define STAMP_AFTER_UART_RESUME     (4+STAMP_PM_ENTER_END)
#define STAMP_BEFORE_DPM_RESUME     (4+STAMP_AFTER_UART_RESUME)
#define STAMP_AFTER_DPM_RESUME      (4+STAMP_BEFORE_DPM_RESUME) /* after dpm_resume, pm_suspend return */


void print_ccpu_pm_info(void)
{
    printk("soft wakelock:        0x%x\n",*((u32 *)g_lowpower_shared_addr));
    printk("[SLEEP COUNT] pm enter cnt: 0x%x, dpm fail cnt: 0x%x\n"\
        ,*((u32 *)(g_lowpower_shared_addr+PM_ENTER_COUNT)) \
        ,*((u32 *)(g_lowpower_shared_addr+PM_DPM_FAIL_COUNT))\
        );
    printk("[STAMP SLICE] sleep start: 0x%x, sleep end: 0x%x, wake start: 0x%x, wake start: 0x%x \n"\
        ,*((u32 *)(g_lowpower_shared_addr+STAMP_PM_SUSPEND_START)) \
        ,*((u32 *)(g_lowpower_shared_addr+STAMP_AFTER_SEND_IPC))   \
        ,*((u32 *)(g_lowpower_shared_addr+STAMP_PWRUP_CODE_BEGIN)) \
        ,*((u32 *)(g_lowpower_shared_addr+STAMP_AFTER_DPM_RESUME)) \
        );
}
int ccpu_lp_mntn_notify(struct notifier_block *nb, unsigned long event, void *dummy)
{
	char* dump_base = NULL;
	unsigned int dump_length = 0;
    if(event == PM_SUSPEND_PREPARE){
        if(!g_lowpower_shared_addr){

            /*获取DUMP 低功耗共享内存地址*/
            if(BSP_OK != bsp_dump_get_buffer(DUMP_SAVE_MOD_DRX_CCORE, &dump_base, &dump_length)){
                printk("*******!get dump buffer failed!******* \n\n");
                return 0;
            }
            g_lowpower_shared_addr = (unsigned int)dump_base;
        }
    }
	if ((event == PM_POST_SUSPEND)&&(g_lowpower_shared_addr))
	{
	    printk("\n############ Ccpu Debug Info,Current Slice 0x%x ############\n",bsp_get_slice_value());

		/*此处用于增加CCPU的打印信息函数*/
		print_ccpu_pm_info();
	}
	return 0;
}
#ifdef CONFIG_HISI_BALONG_MODEM

void bsp_modem_error_handler(u32  p1,  void* p2, void* p3, void* p4)
{
	unsigned long regval = 0;
	regval = readl_relaxed(lp_ctrl.pmctrl_base + 0x3A0);
	mntn_printf("pmc offset:0x3A0    :PERI INTO MASK = 0x%x\n", regval);
	regval = readl_relaxed(lp_ctrl.pmctrl_base + 0x3A4);
	mntn_printf("pmc offset :0x3A4  :NOC TIMEOUT INTSTAT = 0x%x\n", regval);
	regval = readl_relaxed(lp_ctrl.pmctrl_base + 0x380);
	mntn_printf("pmc offset:0x380     :NOC_POWER_IDLEREQ = 0x%x\n", regval);
	regval = readl_relaxed(lp_ctrl.pmctrl_base + 0x384);
	mntn_printf("pmc offset:0x384     :NOC_POWER_IDLEACK = 0x%x\n", regval);
	regval = readl_relaxed(lp_ctrl.pmctrl_base + 0x388);
	mntn_printf("pmc offset:0x388     :NOC_POWER_IDLE = 0x%x\n", regval);
	regval = readl_relaxed(lp_ctrl.pericrg_base  + 0x68);
	mntn_printf("pericrg offset:0x68    :MODEM NOC perrststat0 = 0x%x\n", regval);
}
#endif
static int __init bsp_lowpower_mntn_init_acore(void)
{
    modem_lowpower_mntn_notify.notifier_call = ccpu_lp_mntn_notify;
	register_pm_notifier(&modem_lowpower_mntn_notify);
#ifdef CONFIG_HISI_BALONG_MODEM
	lp_ctrl.pmctrl_base = ioremap(HI_NOC_PMC_REG_ADDR,HI_NOC_PMC_REG_SIZE);
	lp_ctrl.pericrg_base = ioremap(HI_LP_PERI_CRG_REG_ADDR,HI_LP_PERI_CRG_REG_SIZE);
#endif
	return 0;
}
module_init(bsp_lowpower_mntn_init_acore);

