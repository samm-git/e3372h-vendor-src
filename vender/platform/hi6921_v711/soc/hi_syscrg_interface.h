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
#ifndef __HI_SYSCRG_INTER_H__
#define __HI_SYSCRG_INTER_H__

#ifdef _ASSEMBLE_M3_
    #define UNRESET_APPCORE    \
    ldr   R0, =CRG_CTRL25      ;\
    ldr   R1, =0x4000          ;\
    str   R1, [R0]
#else

#include "hi_base.h"
#include "hi_syscrg.h"

/*此宏用于时钟模块*/
#ifndef CONFIG_V711_CLK_CRG
#define CONFIG_V711_CLK_CRG
#endif
/********************************************************************************/
/*    syscrg 函数（hi_syscrg_模块名_功能)        */
/********************************************************************************/
static __inline__ void hi_syscrg_wdt_clk_enble(void)
{
    set_hi_crg_clken2_wdog_clk_en(1);
}

static __inline__ void hi_syscrg_wdt_clk_disable(void)
{
	set_hi_crg_clkdis2_wdog_clk_dis(1);
}

/* efuse clk 使能 */
static __inline__ void hi_syscrg_efuse_clk_enble(void)
{
    set_hi_crg_clken1_efuse_clk_en(1);
}

/* efuse clk 关闭 */
static __inline__ void hi_syscrg_efuse_clk_disable(void)
{
    set_hi_crg_clkdis1_efuse_clk_dis(1);
}

/* Nandc 时钟使能 */
static __inline__ void hi_syscrg_nandc_clk_enable(void)
{
    set_hi_crg_clken1_nandc_clk_en(1);
}
/* emi 时钟使能 */
static __inline__ void hi_syscrg_emi_clk_enable(void)
{
    set_hi_crg_clken1_emi_clk_en(1);
}
/* emi 时钟关闭 */
static __inline__ void hi_syscrg_emi_clk_disable(void)
{
    set_hi_crg_clkdis1_emi_clk_dis(1);
}
/* spi0 时钟使能 */
static __inline__ void hi_syscrg_spi0_clk_enable(void)
{
    set_hi_crg_clken1_dw_ssi0_clk_en(1);
}
/* spi0 时钟关闭 */
static __inline__ void hi_syscrg_spi0_clk_disable(void)
{
    set_hi_crg_clkdis1_dw_ssi0_clk_dis(1);
}
/* spi1 时钟使能 */
static __inline__ void hi_syscrg_spi1_clk_enable(void)
{
    set_hi_crg_clken1_dw_ssi1_clk_en(1);
}
/* spi1 时钟关闭 */
static __inline__ void hi_syscrg_spi1_clk_disable(void)
{
    set_hi_crg_clkdis1_dw_ssi1_clk_dis(1);
}
/********************************************************************************/
/*    syscrg 函数（AXI Monitor模块，时钟使能与软复位)                           */
/********************************************************************************/
/* AXI Monitor CPUFAST 时钟使能 */
static __inline__ void hi_syscrg_amon_cpufast_clk_enable(void)
{
    set_hi_crg_clken1_amon_cpufast_clk_en(1);
}

/* AXI Monitor SOC 时钟使能 */
static __inline__ void hi_syscrg_amon_soc_clk_enable(void)
{
    set_hi_crg_clken1_amon_soc_clk_en(1);
}

/* AXI Monitor CPUFAST 时钟去使能 */
static __inline__ void hi_syscrg_amon_cpufast_clk_disable(void)
{
    set_hi_crg_clkdis1_amon_cpufast_clk_dis(1);
}

/* AXI Monitor SOC 时钟去使能 */
static __inline__ void hi_syscrg_amon_soc_clk_disable(void)
{
    set_hi_crg_clkdis1_amon_soc_clk_dis(1);
}

static __inline__ unsigned int hi_syscrg_amon_cpufast_get_clk_status(void)
{
    return get_hi_crg_clkstat1_amon_cpufast_clk_status();
}

static __inline__ unsigned int hi_syscrg_amon_soc_get_clk_status(void)
{
    return get_hi_crg_clkstat1_amon_soc_clk_status();
}

/* AXI Monitor CPUFAST 软复位 */
static __inline__ void hi_syscrg_amon_cpufast_soft_reset(void)
{
    set_hi_crg_srsten1_amon_cpufast_srst_en(1);
}

/* AXI Monitor SOC 软复位 */
static __inline__ void hi_syscrg_amon_soc_soft_reset(void)
{
    set_hi_crg_srsten1_amon_soc_srst_en(1);
}

/* AXI Monitor CPUFAST 解复位 */
static __inline__ void hi_syscrg_amon_cpufast_rls_reset(void)
{
    set_hi_crg_srstdis1_amon_cpufast_srst_dis(1);
}

/* AXI Monitor SOC 解复位 */
static __inline__ void hi_syscrg_amon_soc_rls_reset(void)
{
    set_hi_crg_srstdis1_amon_soc_srst_dis(1);
}

/********************************************************************************/
/*    hsic 复位、解复位、开钟、关钟函数*/
/********************************************************************************/

/* hsic 复位*/
static __inline__ void hi_syscrg_hsic_reset(void)
{
    /* Reset HSIC Controller */
    set_hi_crg_srsten2_usbhsic_ctrl_srst_en(1);
}

/*    hsic 解复位函数*/
static __inline__ void hi_syscrg_hsic_unreset(void)
{

    /* Unreset HSIC Controller */
    set_hi_crg_srstdis2_usbhsic_ctrl_srst_dis(1);
}

/* hsic_phy 复位*/
static __inline__ void hi_syscrg_hsic_phy_reset(void)
{
    /* Reset HSIC PHY Por */
    set_hi_crg_srsten2_usbhsic_phy_srst_en(1);
}

/*    hsic_phy 解复位函数*/
static __inline__ void hi_syscrg_hsic_phy_unreset(void)
{
    /* Unreset HSIC PHY Por */
    set_hi_crg_srstdis2_usbhsic_phy_srst_dis(1);
}

/*    hsic开关钟函数*/
static __inline__ void hi_syscrg_hsic_clock_disable(void)
{
    set_hi_crg_clkdis2_usbhsic_clk_dis(1);
}

static __inline__ void hi_syscrg_hsic_clock_enable(void)
{
    set_hi_crg_clken2_usbhsic_clk_en(1);
}



/*EMI 软复位*/
static __inline__ void hi_syscrg_emi_soft_reset_en(void)

{
    set_hi_crg_srsten1_emi_srst_en(1);
}

static __inline__ void hi_syscrg_emi_soft_reset_dis(void)
{
    set_hi_crg_srstdis1_emi_srst_dis(1);
}

/* sio clk正向 */
static __inline__ void hi_syscrg_sio_clk_positive(void)
{
    set_hi_crg_clk_sel1_sio_clk_sel(0);
}

/* sio clk反向 */
static __inline__ void hi_syscrg_sio_clk_negative(void)
{
    set_hi_crg_clk_sel1_sio_clk_sel(1);
}

static __inline__ void hi_syscrg_pcm_clk_sel(void)
{

}
static __inline__ void hi_syscrg_mmc_mmc0_unreset(void)
{
	set_hi_crg_srsten2_mmc0_srst_en(1);

    set_hi_crg_srstdis2_mmc0_srst_dis(1);
}
static __inline__ void hi_syscrg_mmc_mmc1_unreset(void)
{
	set_hi_crg_srsten2_mmc1_srst_en(1);

    set_hi_crg_srstdis2_mmc1_srst_dis(1);
}

static __inline__ void hi_syscrg_mmc_mmc1_phase_div_set(unsigned int value)
{
	set_hi_crg_clkdiv3_mmc1_phase_clk_div(value);
}

static __inline__ void hi_syscrg_mmc_mmc1_cclk_div_set(unsigned int value)

{
	set_hi_crg_clkdiv3_mmc1_cclk_div(value);
}

static __inline__ void hi_crg_clkdiv3_mmc0_cclk_div_set(unsigned int value)
{
	set_hi_crg_clkdiv3_mmc0_cclk_div(value);
}
static __inline__ void hi_syscrg_mmc_ctrl_mmc0_sample_sel(unsigned int value)
{
	set_hi_crg_mmc_ctrl_mmc0_sample_sel(value);

}


static __inline__ void hi_syscrg_mmc_ctrl_mmc0_drv_sel(unsigned int value)
{
	set_hi_crg_mmc_ctrl_mmc0_drv_sel(value);
}

static __inline__ void hi_syscrg_mmc_ctrl_mmc1_sample_sel(unsigned int value)
{
	set_hi_crg_mmc_ctrl_mmc1_sample_sel(value);

}

static __inline__ void hi_syscrg_mmc_ctrl_mmc1_drv_sel(unsigned int value)
{
	set_hi_crg_mmc_ctrl_mmc1_drv_sel(value);
}

static __inline__ void hi_syscrg_mmc_ctrl_mmc1_clk_bypass(unsigned int value)
{
	set_hi_crg_mmc_ctrl_mmc1_clk_bypass(value);
}

static __inline__ void hi_syscrg_mmc_ctrl_mmc0_clk_bypass(unsigned int value)
{
	set_hi_crg_mmc_ctrl_mmc0_clk_bypass(value);
}
static __inline__ void hi_crg_mmc_ctrl_mmc1_sample_ctrl(unsigned int value)
{
	set_hi_crg_mmc_ctrl_mmc1_sample_ctrl(value);
}

static __inline__ void hi_crg_mmc_ctrl_mmc0_sample_ctrl(unsigned int value)
{
	set_hi_crg_mmc_ctrl_mmc0_sample_ctrl(value);
}

/* reset gmac*/
static __inline__ void hi_syscrg_gmac_reset(void)
{
    set_hi_crg_srstdis2_gmac_srst_dis(1);
}


/********************************************************************************/
/*    syscrg 函数（usb模块，解复位)                           */
/********************************************************************************/
static __inline__ void hi_syscrg_usb_bc_clk_on(void)
{
    set_hi_crg_clken2_usb_bc_clk_en(1);
}

static __inline__ void hi_syscrg_usb_bc_clk_off(void)
{
   set_hi_crg_clkdis2_usb_bc_clk_dis(1);
}

/* release controller and PHY */
static __inline__ void hi_syscrg_usb_release(void)
{
    /* unreset the bc controller */
	set_hi_crg_srstdis2_usbotg_bc_srst_dis(1);

    /* enable usb controller clock */
    set_hi_crg_clken2_usbotg_clk_en(1);

    /* unreset usb phy */
    set_hi_crg_srstdis2_usbotg_phy_srst_dis(1);

    /* unreset usb otg ctrl */
    set_hi_crg_srstdis2_usbotg_ctrl_srst_dis(1);

    /* unreset usb otg aux */
    set_hi_crg_srstdis2_usbotg_ctrl_vaux_srst_dis(1);

    /* unreset usb otg vcc */
    set_hi_crg_srstdis2_usbotg_ctrl_vcc_srst_dis(1);

}

/* reset controller and PHY */
static __inline__ void hi_syscrg_usb_reset(void)
{
    /* reset usb otg vcc */
	set_hi_crg_srsten2_usbotg_ctrl_vcc_srst_en(1);

    /* reset usb otg aux */
	set_hi_crg_srsten2_usbotg_ctrl_vaux_srst_en(1);

    /* reset usb otg ctrl */
	set_hi_crg_srsten2_usbotg_ctrl_srst_en(1);

    /* reset usb phy */
	set_hi_crg_srsten2_usbotg_phy_srst_en(1);

    /* disable usb controller clock */
	set_hi_crg_clkdis2_usbotg_clk_dis(1);
}

/* get clk/mtcmos status  ---- start */
static __inline__ unsigned int get_hi_crg_clkstat1(void)
{
#if defined(__KERNEL__)
	return readl((const volatile void *)(HI_SYSCRG_BASE_ADDR+HI_CRG_CLKSTAT1_OFFSET));
#else
	return readl((HI_SYSCRG_BASE_ADDR+HI_CRG_CLKSTAT1_OFFSET));
#endif
}static __inline__ unsigned int get_hi_crg_clkstat2(void)
{
#if defined(__KERNEL__)
	return readl((const volatile void *)(HI_SYSCRG_BASE_ADDR+HI_CRG_CLKSTAT2_OFFSET));
#else
	return readl((HI_SYSCRG_BASE_ADDR+HI_CRG_CLKSTAT2_OFFSET));
#endif
}

static __inline__ unsigned int get_hi_crg_clkstat3(void)
{
#if defined(__KERNEL__)
	return readl((const volatile void *)(HI_SYSCRG_BASE_ADDR+HI_CRG_CLKSTAT3_OFFSET));
#else
	return readl((HI_SYSCRG_BASE_ADDR+HI_CRG_CLKSTAT3_OFFSET));
#endif
}
static __inline__ unsigned int get_hi_crg_clkstat4(void)
{
#if defined(__KERNEL__)
	return readl((const volatile void *)(HI_SYSCRG_BASE_ADDR+HI_CRG_CLKSTAT4_OFFSET));
#else
	return readl((HI_SYSCRG_BASE_ADDR+HI_CRG_CLKSTAT4_OFFSET));
#endif
}
static __inline__ unsigned int get_hi_crg_clkstat5(void)
{
#if defined(__KERNEL__)
	return readl((const volatile void *)(HI_SYSCRG_BASE_ADDR+HI_CRG_CLKSTAT5_OFFSET));
#else
	return readl((HI_SYSCRG_BASE_ADDR+HI_CRG_CLKSTAT5_OFFSET));
#endif
}

static __inline__ unsigned int get_hi_crg_clkstat5_twbbp_tbbp_122m_clk_stat(void)
{
	return get_hi_crg_clkstat5_pdtw_tbbp_122m_clk_status();
}

static __inline__ unsigned int get_hi_crg_clkstat5_twbbp_tbbp_245m_clk_stat(void)
{
	return get_hi_crg_clkstat5_pdtw_tbbp_245m_clk_status();
}
#endif//_ASSEMBLE_M3_
#endif
