#ifndef __HI_PCIE_INTERFACE_H__
#define __HI_PCIE_INTERFACE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "hi_base.h"
#include "hi_syssc.h"
#include "hi_syscrg.h"
#include "soc_interrupts_app.h"

/******************************* Configuration ********************************/

#define BALONG_PCI_VENDOR_ID        (0x19E5)
#define BALONG_PCI_DEVICE_ID        (0x6921)

#define BALONG_PCI_INNER_CLK

#define BALONG_PCIE_INT_LVL_MSI_BASE    (INT_LVL_PCIE0_RADM_D + 1)

#define HI_PCIE_SC_BASE_ADDR            (HI_SYSREG_PD_BASE_ADDR)
#define HI_PCIE_SC_BASE_ADDR_VIRT       (HI_SYSREG_PD_BASE_ADDR_VIRT)

#define HI_PCIE_SC_ASSERT_INT_OFFSET    (HI_SC_CTRL108_OFFSET)

/* PCIE0 */
#define BALONG_PCIE0_EXISTS

#define BALONG_PCIE0_RC_CFG_ADDR        (0x38000000)
#define BALONG_PCIE0_RC_CFG_SIZE        (SZ_64K)

#define BALONG_PCIE0_DEVICE_CFG_ADDR    (0x30000000)
#define BALONG_PCIE0_DEVICE_CFG_SIZE    (SZ_16M)

#define BALONG_PCIE0_TYPE0_CONFIG_ADDR  (0x30000000)
#define BALONG_PCIE0_TYPE0_CONFIG_SIZE  (0x00200000)

#define BALONG_PCIE0_TYPE1_CONFIG_ADDR  (BALONG_PCIE0_TYPE0_CONFIG_ADDR + BALONG_PCIE0_TYPE0_CONFIG_SIZE)
#define BALONG_PCIE0_TYPE1_CONFIG_SIZE  (0x00E00000)

#define BALONG_PCIE0_IO_BASE    (BALONG_PCIE0_TYPE1_CONFIG_ADDR + BALONG_PCIE0_TYPE1_CONFIG_SIZE)
#define BALONG_PCIE0_IO_SIZE    (SZ_16M)

#define BALONG_PCIE0_MEM_BASE   (0x28000000)
#define BALONG_PCIE0_MEM_SIZE   (SZ_128M)

#define BALONG_PCIE0_MSI_PCI_LOWER_ADDR (BALONG_PCIE0_RC_CFG_ADDR)
#define BALONG_PCIE0_MSI_PCI_UPPER_ADDR (0x0)

#define BALONG_PCIE0_MSI_PCI_SIZE       (BALONG_PCIE0_RC_CFG_SIZE)

#define BALONG_PCIE0_MSI_CPU_LOWER_ADDR (BALONG_PCIE0_RC_CFG_ADDR)
#define BALONG_PCIE0_MSI_CPU_UPPER_ADDR (0x0)

#define BALONG_PCIE0_MAX_DMA_CHANNEL_NUM  (7)   /* 8 channels */

#define BALONG_PCIE0_PERST_GPIO         (GPIO_2_24)

#define BALONG_PCIE0_INT_LVL_LINK_DOWN  (INT_LVL_PCIE0_LINK_DOWN)
#define BALONG_PCIE0_INT_LVL_EDMA       (INT_LVL_PCIE0_EDMA)
#define BALONG_PCIE0_INT_LVL_PM         (INT_LVL_PCIE0_PM)
#define BALONG_PCIE0_INT_LVL_RADM_A     (INT_LVL_PCIE0_RADM_A)
#define BALONG_PCIE0_INT_LVL_RADM_B     (INT_LVL_PCIE0_RADM_B)
#define BALONG_PCIE0_INT_LVL_RADM_C     (INT_LVL_PCIE0_RADM_C)
#define BALONG_PCIE0_INT_LVL_RADM_D     (INT_LVL_PCIE0_RADM_D)
#define BALONG_PCIE0_INT_LVL_MSI        (INT_LVL_PCIE0_RADM_B)


/******************************* Definition ********************************/

static __inline__ void hi_pcie_dbi_enable(u32 __pcie_id)
{
    set_hi_sc_ctrl58_pcie_slv_awmisc_info(0x200000);
    set_hi_sc_ctrl59_pcie_slv_armisc_info(0x200000);
}

static __inline__ void hi_pcie_dbi_disable(u32 __pcie_id)
{
    set_hi_sc_ctrl58_pcie_slv_awmisc_info(0);
    set_hi_sc_ctrl59_pcie_slv_armisc_info(0);
}

static __inline__ void hi_pcie_set_work_mode(u32 __pcie_id, u32 __pcie_work_mode)
{
    set_hi_sc_ctrl58_pcie_device_type(__pcie_work_mode);
}

static __inline__ void hi_pcie_ltssm_enable(u32 __pcie_id)
{
    set_hi_sc_ctrl65_pcie_app_ltssm_enable(1);
}

static __inline__ void hi_pcie_ltssm_disable(u32 __pcie_id)
{
    set_hi_sc_ctrl65_pcie_app_ltssm_enable(0);
}

static __inline__ u32 hi_pcie_get_link_status(u32 __pcie_id)
{
    return readl(HI_SYSSC_BASE_ADDR_VIRT + HI_SC_STAT71_OFFSET);
}

static __inline__ u32 hi_pcie_is_linkup(u32 __pcie_id)
{
    return (get_hi_sc_stat70_pcie_rdlh_link_up() & get_hi_sc_stat70_pcie_xmlh_link_up());
}

static __inline__ void hi_pcie_linkdown_interrupt_clear(u32 __pcie_id)
{
    set_hi_sc_ctrl107_pcie_bridge_flush_not_clr(1);
    set_hi_sc_ctrl107_pcie_link_req_rst_not_clr(1);

    set_hi_sc_ctrl107_pcie_bridge_flush_not_clr(0);
    set_hi_sc_ctrl107_pcie_link_req_rst_not_clr(0);
}

static __inline__ void hi_soc_peri_usb_unreset(u32 __pcie_id)
{
    set_hi_crg_srstdis2_soc_peri_usb_pd_srst_dis(1);
}

static __inline__ void hi_pcie_core_unreset(u32 __pcie_id)
{
    set_hi_crg_srstdis2_pcie_ctrl_srst_dis(1);
}

static __inline__ void hi_pcie_core_reset(u32 __pcie_id)
{
    set_hi_crg_srsten2_pcie_ctrl_srst_req(1);
}

static __inline__ void hi_pcie_phy_unreset(u32 __pcie_id)
{
    set_hi_sc_ctrl109_phy_test_powerdown(0);
    set_hi_sc_ctrl110_phy_reset(0);
    set_hi_crg_srstdis2_pcie_phy_srst_dis(1);
}

static __inline__ void hi_pcie_phy_reset(u32 __pcie_id)
{
    set_hi_crg_srsten2_pcie_phy_srst_req(1);
}

static __inline__ u32 hi_pcie_is_under_reset(u32 __pcie_id)
{
    return (get_hi_sc_stat75_reserved() & (0x1 << 19));
}

static __inline__ void hi_pcie_perst_set(u32 __pcie_id, u32 __pcie_value)
{
    set_hi_sc_ctrl113_pcie_perst_out_n(__pcie_value);
    set_hi_sc_ctrl113_pcie_perst_oe(1);
}

static __inline__ void hi_pcie_phy_init(u32 __pcie_id)
{
    writel(0x1846C24A, HI_PCIE_SC_BASE_ADDR_VIRT + HI_SC_CTRL111_OFFSET);
    writel(0x15003FFF, HI_PCIE_SC_BASE_ADDR_VIRT + HI_SC_CTRL112_OFFSET);
}

static __inline__ void hi_pcie_set_apps_pm_xmt_turnoff(u32 __pcie_id, u32 __pcie_value)
{
    set_hi_sc_ctrl65_pcie_apps_pm_xmt_turnoff(__pcie_value);
}

static __inline__ void hi_pcie_set_app_ready_entr_l23(u32 __pcie_id, u32 __pcie_value)
{
    set_hi_sc_ctrl65_pcie_app_ready_entr_l23(__pcie_value);
}

static __inline__ void hi_pcie_set_apps_pm_xmt_pme(u32 __pcie_id, u32 __pcie_value)
{
    set_hi_sc_ctrl65_pcie_apps_pm_xmt_pme(__pcie_value);
}

static __inline__ u32 hi_pcie_get_radm_pm_to_ack_reg(u32 __pcie_id)
{
    return get_hi_sc_stat71_radm_pm_to_ack_reg();
}

static __inline__ void hi_pcie_inner_clk_enable(u32 __pcie_id)
{
    /* PHY port signal, switch to inner clock */
    set_hi_sc_ctrl110_phy_ref_use_pad(0);

    /* ICG1 clock enable */
    set_hi_sc_ctrl110_ref_alt_clk_gt_en(1);

    /* ICG2 clock anto gate enable signal, 0 -- disable auto gate(force enable), 1 -- enable auto gate */
    set_hi_sc_ctrl108_pcie0_phy_gatedclock_en(0);

    /* the control from pad: 0 -- disable, 1 -- enable */
    set_hi_sc_ctrl108_pcie0_pad_phy_mask(1);
    /* the control from inner: 0 -- disable, 1 -- enable */
    set_hi_sc_ctrl108_pcie0_inner_phy_mask(0);
}

static __inline__ void hi_pcie_output_clk_enable(u32 __pcie_id)
{
    /* select the control of PCIe IO, 0 -- software(refclk_oen_cfg), 1 -- hardware */
    set_hi_sc_ctrl108_pcie0_refclk_oen_sel(1);

    /* the control of PCIe IO by software, 1 -- enable output */
    /* if the PCIe IO is controlled by hardware, this bit is useless */
    set_hi_sc_ctrl108_pcie0_refclk_oen_cfg(1);

    /* the control of auto gate: 0 -- disable(force to output), 1 -- enable */
    set_hi_sc_ctrl108_pcie0_out_gatedclock_en(0);

    /* the control from pad: 0 -- disable, 1 -- enable */
    set_hi_sc_ctrl108_pcie0_pad_out_mask(1);
    /* the control from inner: 0 -- disable, 1 -- enable */
    set_hi_sc_ctrl108_pcie0_inner_out_mask(0);
}

static __inline__ void hi_pcie_l1ss_auto_gate_enable(u32 __pcie_id)
{
    /* ICG2 clock anto gate enable signal, 0 -- disable auto gate(force enable), 1 -- enable auto gate */
    set_hi_sc_ctrl108_pcie0_phy_gatedclock_en(1);
    /* the control of auto gate: 0 -- disable auto gate(force to output), 1 -- enable auto gate */
    set_hi_sc_ctrl108_pcie0_out_gatedclock_en(1);
}

static __inline__ void hi_pcie_app_clk_req_n(u32 __pcie_id, u32 __request)
{
    if (__request)
    {
        /* 0 -- request clock */
        set_hi_sc_ctrl59_pcie_app_clk_req_n(0);
        set_hi_sc_ctrl65_pcie_app_req_exit_l1(1);
    }
    else
    {
        set_hi_sc_ctrl59_pcie_app_clk_req_n(1);
        set_hi_sc_ctrl65_pcie_app_req_exit_l1(0);
    }
}


#ifdef __cplusplus
}
#endif

#endif

