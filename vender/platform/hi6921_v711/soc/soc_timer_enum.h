#ifndef __SOC_TIMER_ENUM__
#define __SOC_TIMER_ENUM__
/*用于在drv_timer.h中区分不同产品形态*/
#define   CCPU_DSP_DRX_TIMER          17
#define   ACPU_SOFTTIMER_WAKE         1
#define   CCPU_SOFTTIMER_WAKE         2
#define   ACPU_WDT_TIMER              3
#define   CCPU_WDT_TIMER              4
/*V7R2上 时间戳不用timer提供*/
#define   STAMP_TIMER                 1000
#define   USB_TIMER                   6
#define   CCPU_SOFTTIMER_NOWAKE       7
#define   ACPU_SOFTTIMER_NOWAKE       11
#define   DSP_TIMER2                  9
#define   MCU_TIMER                   10
#define   HIFI_TIMER2                 8
#define   UDELAY_TIMER                12
#define   CCPU_SYSTIMER               13
#define   ACPU_SYSTIMER               14
#define   ACPU_SOURCE_TIMER           15
#define   ACPU_VIEW_TIMER             16
#define   CCPU_VIEW_TIMER             16
#define   HIFI_TIMER1                 0
#define   ACPU_OSA_TIMER              18
#define   CCPU_OSA_TIMER              19
#define   CCPU_DRX1                   20
#define   DSP_TIMER1                  21
#define   CCPU_DRX2                   22
#define   CCPU_DRX_TIMER              23
#define   ACPU_OM_TIMER               5

#endif

