/******************************************************************************/
/*  Copyright (C), 2007-2013, Hisilicon Technologies Co., Ltd. */
/******************************************************************************/
/* File name     : hi_syssc_interface.h */
/* Version       : 2.0 */
/* Author        : w00221604*/
/* Created       : 2013-04-18*/
/* Last Modified : */
/* Description   : */
/* Function List : */
/* History       : */
/* 1 Date        : */
/* Author        : w00221604*/
/* Modification  : Create file */
/******************************************************************************/
#ifndef __HI_SYSSC_INTER_H__
#define __HI_SYSSC_INTER_H__

#include "hi_base.h"
#include "hi_syssc.h"

/********************************************************************************/
/*    syssc 函数（hi_syssc_模块名_功能)        */
/********************************************************************************/
static __inline__ void hi_syssc_wdt_reboot(void)
{
    set_hi_sc_ctrl1_pmu_rstout_n(0);
}

static __inline__ void hi_syssc_wdt_enble(void)
{
	set_hi_sc_ctrl2_wdt_en_ctrl(0x6920);
	set_hi_sc_ctrl2_wdt_clk_en(1);
}

/********************************************************************************/
/*    syssc 函数（AXI Monitor模块，启动、停止)                                  */
/********************************************************************************/
/* 启动CPUFAST, 如果该启动位为1，需要先写0，再写1 */
static __inline__ void hi_syssc_amon_cpufast_start(void)
{
    if(get_hi_sc_ctrl3_amon_cpufast_monitor_start())
    {
        set_hi_sc_ctrl3_amon_cpufast_monitor_start(0);
    }
    set_hi_sc_ctrl3_amon_cpufast_monitor_start(1);
}

/* 启动SOC, 如果该启动位为1，需要先写0，再写1 */
static __inline__ void hi_syssc_amon_soc_start(void)
{
    if(get_hi_sc_ctrl3_amon_soc_monitor_start())
    {
        set_hi_sc_ctrl3_amon_soc_monitor_start(0);
    }
    set_hi_sc_ctrl3_amon_soc_monitor_start(1);
}

/* 停止CPUFAST */
static __inline__ void hi_syssc_amon_cpufast_stop(void)
{
    set_hi_sc_ctrl3_amon_cpufast_monitor_start(0);
}

/* 停止SOC */
static __inline__ void hi_syssc_amon_soc_stop(void)
{
    set_hi_sc_ctrl3_amon_soc_monitor_start(0);
}

static __inline__ void hi_syssc_pmu_reboot(void)
{
    set_hi_sc_ctrl1_pmu_rstout_n(0);
}

static __inline__ void hi_syssc_pmu_hold_down(void)
{
    set_hi_sc_ctrl1_pmu_hold(0);
}

static __inline__ void hi_syssc_pcm_mode_master(void)
{
    set_hi_sc_ctrl3_sio_master_mode(1);
}

static __inline__ void hi_syssc_pcm_mode_slave(void)
{
    set_hi_sc_ctrl3_sio_master_mode(0);
}

static __inline__ void hi_syssc_mmc_voltage_18v(unsigned int enable)
{
    set_hi_sc_ctrl3_mmc_msc(!!enable);
}

/********************************************************************************/
/*    syssc 函数（Hsic模块，init/unreset*/
/********************************************************************************/
static __inline__ void hi_syssc_hsic_config_settings(void)
{
    /* Adjust HSIC signals */
    set_hi_sc_ctrl34_txrputune(0x3);
    set_hi_sc_ctrl34_txrpdtune(0x3);
    set_hi_sc_ctrl34_txsrtune(0xF);
    /* Force Host Mode */
    set_hi_sc_ctrl34_utmiotg_iddig(0);
    /* dppulldown,dmpulldown clear */
    set_hi_sc_ctrl34_dmpulldown(1);
    set_hi_sc_ctrl34_dppulldown(1);
}
static __inline__ void hi_syssc_hsic_phy_reset(void)
{
    set_hi_sc_ctrl34_hsic_portreset(1);
}

static __inline__ void hi_syssc_hsic_phy_unreset(void)
{
    set_hi_sc_ctrl34_hsic_portreset(0);
}


/********************************************************************************/
/*    syssc 函数（USB模块)                                  */
/********************************************************************************/
/* ref_ssp_en */
static __inline__ void hi_syssc_usb_ref_ssp(unsigned int enable)
{
}

/* usb2.0 phy powerdown, enable = 1: powerdown / enable = 0: powerup */
static __inline__ void hi_syssc_usb_powerdown_hsp(unsigned int enable)
{
}

/* usb3.0 phy powerdown, enable = 1: powerdown / enable = 0: powerup */
static __inline__ void hi_syssc_usb_powerdown_ssp(unsigned int enable)
{
}

/* vbusvldext and vbusvldextsel enable */
static __inline__ void hi_syssc_usb_vbusvldext(unsigned int enable)
{
    set_hi_sc_ctrl37_vbusvldextsel(!!enable);
    set_hi_sc_ctrl37_vbusvldext(!!enable);
}

/* override_en and override_value*/
static __inline__ void hi_syssc_usb_override(unsigned int enable)
{
    set_hi_sc_ctrl41_usb3_pmu_powerpresent_override_en(!!enable);

    set_hi_sc_ctrl41_usb3_pmu_avalid_override_en(!!enable);
    set_hi_sc_ctrl41_usb3_pmu_bvalid_override_en(!!enable);
    set_hi_sc_ctrl41_usb3_pmu_vbusvalid_override_en(!!enable);

    set_hi_sc_ctrl41_usb3_pmu_powerpresent_override(!!enable);

    set_hi_sc_ctrl41_usb3_pmu_avalid_override(!!enable);
    set_hi_sc_ctrl41_usb3_pmu_bvalid_override(!!enable);
    set_hi_sc_ctrl41_usb3_pmu_vbusvalid_override(!!enable);
}

static __inline__ void hi_syssc_usb_txpreempamptune(unsigned int txpreempamptune)
{
    set_hi_sc_ctrl38_txpreempamptune(txpreempamptune);
}

static __inline__ void hi_syssc_usb_iddig(unsigned int value)
{
    set_hi_sc_ctrl41_usb3_pmu_iddig_override(value);
}

static __inline__ void hi_syssc_usb_iddig_en(unsigned int value)
{
    set_hi_sc_ctrl41_usb3_pmu_iddig_override_en(value?1:0);
}

static __inline__ void hi_syssc_usb_phy3_init(void)
{
    set_hi_sc_ctrl39_los_level4_3(0x1);
    set_hi_sc_ctrl39_pcs_tx_deemph_3p5db(0x15);
    set_hi_sc_ctrl39_pcs_tx_deemph_6db(0x20);
    set_hi_sc_ctrl39_pcs_tx_swing_full(0x71);
    set_hi_sc_ctrl39_lane0_tx_term_offset(0x0);

    set_hi_sc_ctrl39_tx_vboost_lvl(0x7);
    set_hi_sc_ctrl38_los_bias(3);
}

static __inline__ void hi_syssc_ctrl3_mmc1_sample_tuning_enable(unsigned int enable)
{
    set_hi_sc_ctrl3_mmc1_sample_tuning_enable(!!enable);
}

static __inline__ void hi_syssc_ctrl3_mmc0_sample_tuning_enable(unsigned int enable)
{
    set_hi_sc_ctrl3_mmc0_sample_tuning_enable(!!enable);
}
/********************************************************************************/
/*    syssc 函数（EBI模块)  */
/********************************************************************************/
static __inline__ void hi_syssc_ebi_mode_normal(void)
{
    set_hi_sc_ctrl32_ebi_normal_mode(1);
}

/*********  pm start **********/


/* a9热启动地址控制。*/
#define A9_BOOT_ALIGN_64K  0
#define A9_BOOT_ALIGN_32K  1
#define A9_BOOT_ALIGN_16K  2
#define A9_BOOT_ALIGN_8K   3
static __inline__ void hi_syssc_set_appa9_boot_addr(unsigned int boot_addr)
{
    /* bit [31..13] */
    set_hi_sc_ctrl5_sw_appa9_boot_addr(boot_addr>>13);
    set_hi_sc_ctrl7_appa9_remap_size(A9_BOOT_ALIGN_16K);
}

/*v7r2 包括对齐单位的设置 */
static __inline__ void hi_syssc_set_mdma9_boot_addr(unsigned int boot_addr)
{
    /* bit [31..13] */
    set_hi_sc_ctrl6_sw_modema9_boot_addr(boot_addr>>13);
    set_hi_sc_ctrl7_mdma9_remap_size(A9_BOOT_ALIGN_16K);
}


/*********  pm end   **********/

/* edma peri dma_sel */
/* 互联优先级配置，将 edma_ch4 优先级降到最低 */
static __inline__ void edma_ch4_set_axi_mst_min(void)
{
    set_hi_sc_ctrl72_axi_mst_priority_m2(0);
}

#define edma_dw_ssi0_dma_sel(edma_id)     set_hi_sc_ctrl54_dw_ssi0_dma_sel(edma_id)         
#define edma_dw_ssi1_dma_sel(edma_id)     set_hi_sc_ctrl54_dw_ssi1_dma_sel(edma_id)         
#define edma_sio_dma_sel(edma_id)         set_hi_sc_ctrl54_sio_dma_sel(edma_id)             
#define edma_hs_uart_dma_sel(edma_id)     set_hi_sc_ctrl54_hs_uart_dma_sel(edma_id)         
#define edma_uart0_dma_sel(edma_id)       set_hi_sc_ctrl54_uart0_dma_sel(edma_id)           
#define edma_uart1_dma_sel(edma_id)       set_hi_sc_ctrl54_uart1_dma_sel(edma_id)           
#define edma_uart2_dma_sel(edma_id)       set_hi_sc_ctrl54_uart2_dma_sel(edma_id)           
#define edma_sci0_dma_sel(edma_id)        set_hi_sc_ctrl54_sci0_dma_sel(edma_id)            
#define edma_sci1_dma_sel(edma_id)        set_hi_sc_ctrl54_sci1_dma_sel(edma_id)            
#define edma_bbp_dbg_dma_sel(edma_id)     set_hi_sc_ctrl54_bbp_dbg_dma_sel(edma_id)         
#define edma_bbp_grif_dma_sel(edma_id)    set_hi_sc_ctrl54_bbp_grif_dma_sel(edma_id)        
#define edma_amon_soc_dma_sel(edma_id)    set_hi_sc_ctrl54_amon_soc_dma_sel(edma_id)        
#define edma_amon_fast_dma_sel(edma_id)   set_hi_sc_ctrl54_amon_fast_dma_sel(edma_id)       
/* edma peri dma_sel   end */

/*gu bbp wakeup status get*/
#define HI_SC_GUBBP_WAKEUP_STAT_OFFSET 0x530
#define GUBBP_WAKEUP_STAT_MASK 0x7
static __inline__ unsigned int hi_get_gubbp_wakeup_status(void)
{
#if 0
	unsigned int temp = 0;
    temp =  (*(volatile unsigned *) (HI_AP_SYSCTRL_BASE_ADDR_VIRT +  HI_SC_GUBBP_WAKEUP_STAT_OFFSET));

	return (temp & GUBBP_WAKEUP_STAT_MASK);
#endif
    return 0;
}
/*设置spi三线模式*/
static __inline__ void hi_spi_three_wire_set(unsigned int spi_no)
{
    unsigned int reg;
    /*三线模式*/
    reg = readl(HI_SYSCTRL_BASE_ADDR+HI_SC_CTRL3_OFFSET);
    if(spi_no)
        reg &= ~0x100;
    else
        reg &= ~0x80;    
    writel(reg,HI_SYSCTRL_BASE_ADDR+HI_SC_CTRL3_OFFSET);
}


#endif

