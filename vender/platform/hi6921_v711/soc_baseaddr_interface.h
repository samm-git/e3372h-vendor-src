/******************************************************************************

                 版权所有 (C), 2001-2012, 华为技术有限公司

 ******************************************************************************
  文 件 名   : soc_baseaddr_interface.h
  版 本 号   : 初稿
  作    者   : Excel2Code
  生成日期   : 2012-12-08 16:50:26
  最近修改   :
  功能描述   : 接口头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2012年12月8日
    修改内容   : 从《Hi6620V100 SOC寄存器手册_BaseAddr.xml》自动生成

******************************************************************************/

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/

#ifndef __SOC_BASEADDR_INTERFACE_H__
#define __SOC_BASEADDR_INTERFACE_H__

#include "soc_memmap_comm.h"
#include "bsp_memmap.h"
#include "hi_bbp_systime.h"
#include "hi_timer.h"
#ifdef __cplusplus
    #if __cplusplus
        extern "C" {
    #endif
#endif



/*****************************************************************************
  2 宏定义
*****************************************************************************/
/* 寄存器说明： */

#define SOC_BBP_COMM_BASE_ADDR                        HI_CTU_BASE_ADDR

/* 寄存器说明： */
#define SOC_BBP_COMM_ON_BASE_ADDR                     HI_BBP_COMM_ON_BASE_ADDR

/* 寄存器说明： */
#define SOC_BBP_GSM_BASE_ADDR                         HI_GBBP_REG_BASE_ADDR

/* 寄存器说明： */
#define SOC_BBP_GSM_ON_BASE_ADDR                        HI_GBBP_DRX_REG_BASE_ADDR


#define SOC_BBP_GSM1_BASE_ADDR                         HI_GBBP1_REG_BASE_ADDR

/* 寄存器说明： */
#define SOC_BBP_GSM1_ON_BASE_ADDR                        HI_GBBP1_DRX_REG_BASE_ADDR


/* 寄存器说明： */
#define SOC_BBP_WCDMA_BASE_ADDR                       HI_WBBP_REG_BASE_ADDR

/* 寄存器说明：*/
#define SOC_BBP_WCDMA_ON_BASE_ADDR                    HI_WBBP_DRX_REG_BASE_ADDR

/* 寄存器说明： */
#define SOC_UPACC_BASE_ADDR           	              HI_UPACC_BASE_ADDR
#define SOC_AO_SCTRL_SC_SLICER_COUNT0_ADDR(x) (HI_BBP_SYSTIME_BASE_ADDR_VIRT+HI_BBP_SYSTIME_ABS_TIMER_L_OFFSET)

/*HIFI timer使能计数，V7R2 HIFI使用的timer已切换为32k，不需要上层操作，故打桩*/
#define SOC_AO_SCTRL_SC_TIMER_EN1_ADDR(x)

/*HIFI timer时钟使能*/
#define SOC_AO_SCTRL_SC_PERIPH_CLKEN4_ADDR(x) (HI_SYSCRG_BASE_ADDR+0x18)

/*获取timer基地址*/
#define SOC_HIFI_Timer00_BASE_ADDR HI_TIMER_00_REGBASE_ADDR
#define SOC_HIFI_Timer08_BASE_ADDR HI_TIMER_08_REGBASE_ADDR

/*ipc基址*/
#define SOC_IPC_S_BASE_ADDR HI_IPCM_REGBASE_ADDR

/*DMA基址*/
#define SOC_AP_DMAC_BASE_ADDR HI_EDMA_REGBASE_ADDR

/*DMA基址*/
#define SOC_UART3_BASE_ADDR HI_UART3_REGBASE_ADDR


#define SOC_AO_SCTRL_SC_SLICER_COUNT0_ADDR(x) (HI_BBP_SYSTIME_BASE_ADDR_VIRT+HI_BBP_SYSTIME_ABS_TIMER_L_OFFSET)

/*HIFI timer使能计数，V7R2 HIFI使用的timer已切换为32k，不需要上层操作，故打桩*/
#define SOC_AO_SCTRL_SC_TIMER_EN1_ADDR(x)

/*HIFI timer时钟使能*/
#define SOC_AO_SCTRL_SC_PERIPH_CLKEN4_ADDR(x) (HI_SYSCRG_BASE_ADDR+0x18)

/*获取timer基地址*/
#define SOC_HIFI_Timer00_BASE_ADDR HI_TIMER_00_REGBASE_ADDR
#define SOC_HIFI_Timer08_BASE_ADDR HI_TIMER_08_REGBASE_ADDR

/*ipc基址*/
#define SOC_IPC_S_BASE_ADDR HI_IPCM_REGBASE_ADDR

/*DMA基址*/
#define SOC_AP_DMAC_BASE_ADDR HI_EDMA_REGBASE_ADDR

/*DMA基址*/
#define SOC_UART3_BASE_ADDR HI_UART3_REGBASE_ADDR



/*****************************************************************************
  3 枚举定义
*****************************************************************************/



/*****************************************************************************
  4 消息头定义
*****************************************************************************/


/*****************************************************************************
  5 消息定义
*****************************************************************************/



/*****************************************************************************
  6 STRUCT定义
*****************************************************************************/



/*****************************************************************************
  7 UNION定义
*****************************************************************************/





/*****************************************************************************
  8 OTHERS定义
*****************************************************************************/



/*****************************************************************************
  9 全局变量声明
*****************************************************************************/


/*****************************************************************************
  10 函数声明
*****************************************************************************/


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of soc_baseaddr_interface.h */

