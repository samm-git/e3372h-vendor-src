#ifndef __SOC_TIMER_H__
#define __SOC_TIMER_H__

#include <hi_base.h>
#include <bsp_memmap.h>
#include <soc_timer_enum.h>
#ifdef __KERNEL__
#include <soc_interrupts_app.h>
#include <soc_clk_app.h>
#elif defined(__VXWORKS__)
#include <soc_interrupts_mdm.h>
#include <soc_clk_mdm.h>
#elif defined(__CMSIS_RTOS)
#include <soc_interrupts_m3.h>
#include <soc_clk.h>
#endif

/*封装udelay延时函数使用的timer基址*/
#define UDELAY_TIMER_ADDR   HI_TIMER_12_REGBASE_ADDR_VIRT
#define UDELAY_TIMER_CLK    19200000

#define CCORE_SYS_TIMER_CLK              HI_TIMER_CLK
#define CCORE_SYS_TIMER_BASE_ADDR        HI_TIMER_13_REGBASE_ADDR_VIRT
#define CCORE_SYS_TIMER_INT_LVL          INT_LVL_TIMER13

#define UDELAY_CURTIME_ADDR   (UDELAY_TIMER_ADDR+0X4)
#define PM_MCU_TIMER_BASE      HI_TIMER_10_REGBASE_ADDR
#define PM_MCU_TIMER_CLK       HI_TCXO_CLK
#define PM_MCU_TIMER_INT_LVL   M3_TIMER10_INT

#ifndef __ASSEMBLY__
/*1代表非唤醒特性,0代表唤醒特性*/
static inline int check_timer_type(unsigned int type)
{
	if((type!=1)&&(type!=0))
		return (-1);
	else
		return 0;
}
static inline unsigned int  __bsp_get_slice_value_hrt(void)
{
	return 0xFFFFFFFF - (*(volatile unsigned *)(UDELAY_CURTIME_ADDR));
}

#define platform_timer_rest_time(i,j) bsp_get_timer_rest_time(i,j)

#ifndef get_timer_slice_delta
#define get_timer_slice_delta(begin,end) ((end>=begin)?(end-begin):(0xFFFFFFFF-begin+end))
#endif

static inline void __udelay_soc(unsigned us)
{
	unsigned int begin = 0;
	unsigned int end=0;
	unsigned int delta = 0;
	delta = us*(UDELAY_TIMER_CLK/1000000);
	begin = *(volatile unsigned *)(UDELAY_CURTIME_ADDR);
	do{
		end = *(volatile unsigned *)(UDELAY_CURTIME_ADDR);
	}while(get_timer_slice_delta(end,begin)<delta);
}


#endif

#ifdef __KERNEL__

#define ST_WAKE_HARDTIMER_ADDR           HI_TIMER_01_REGBASE_ADDR_VIRT
#define ST_NORMAL_HARDTIMER_ADDR         HI_TIMER_11_REGBASE_ADDR_VIRT
#define WAKE_TIMER_CLK                   HI_TIMER1_CLK
#define NOWAKE_TIMER_CLK                 HI_TIMER11_CLK
#define WAKE_TIMER_IRQ_ID                INT_LVL_TIMER1
#define NORMAL_TIMER_IRQ_ID              INT_LVL_TIMER11
#define wake_timer_array \
{\
	ACORE_SOFTTIMER_ID,\
	ACORE_WDT_TIMER_ID,\
	TIMER_ACPU_OSA_ID\
}

#elif defined(__VXWORKS__)

#define ST_WAKE_HARDTIMER_ADDR           HI_TIMER_02_REGBASE_ADDR_VIRT
#define ST_NORMAL_HARDTIMER_ADDR         HI_TIMER_07_REGBASE_ADDR_VIRT
#define WAKE_TIMER_CLK                   HI_TIMER2_CLK
#define NOWAKE_TIMER_CLK                 HI_TIMER7_CLK
#define WAKE_TIMER_IRQ_ID                INT_LVL_TIMER2
#define NORMAL_TIMER_IRQ_ID              INT_LVL_TIMER7
#define wake_timer_array \
{\
	TIMER_CCPU_DSP_DRX_PROT_ID,\
	CCORE_WDT_TIMER_ID,\
	TIMER_CCPU_OSA_ID,\
	CCORE_SOFTTIMER_ID,\
	TIMER_DSP_TIMER2_ID,\
	TIMER_DSP_TIMER1_ID\
}

#endif
#define WAKE_SOURCE_CLK      (32*1024)
#define NOWAKE_SOURCE_CLK    (32*1024)
#define SOFTTIMER_MAX_LENGTH             (0xFFFFFFFF>>15)
#define SOFTTIMER_MAX_LENGTH_NORMAL      (0xFFFFFFFF>>15)


#endif/*__SOC_TIMER_H__*/

