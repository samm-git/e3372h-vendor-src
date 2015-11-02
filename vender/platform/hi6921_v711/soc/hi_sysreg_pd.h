/******************************************************************************/
/*  Copyright (C), 2007-2014, Hisilicon Technologies Co., Ltd. */
/******************************************************************************/
/* File name     : hi_sysreg_pd.h */
/* Version       : 2.0 */
/* Author        : xxx*/
/* Created       : 2014-02-19*/
/* Last Modified : */
/* Description   :  The C union definition file for the module sysreg_pd*/
/* Function List : */
/* History       : */
/* 1 Date        : */
/* Author        : xxx*/
/* Modification  : Create file */
/******************************************************************************/

#ifndef __HI_SYSREG_PD_H__
#define __HI_SYSREG_PD_H__

/*
 * Project: hi
 * Module : sysreg_pd
 */

#ifndef HI_SET_GET
#define HI_SET_GET(a0,a1,a2,a3,a4)
#endif

/********************************************************************************/
/*    sysreg_pd 寄存器偏移定义（项目名_模块名_寄存器名_OFFSET)        */
/********************************************************************************/
#define    HI_SC_CTRL58_OFFSET                               (0x4E8) /* PCIE0系统控制寄存器0寄存器。 */
#define    HI_SC_CTRL59_OFFSET                               (0x4EC) /* PCIE0系统控制寄存器1寄存器。 */
#define    HI_SC_CTRL60_OFFSET                               (0x4F0) /* PCIE0系统控制寄存器2寄存器。 */
#define    HI_SC_CTRL61_OFFSET                               (0x4F4) /* PCIE0系统控制寄存器3寄存器。 */
#define    HI_SC_CTRL62_OFFSET                               (0x4F8) /* PCIE0系统控制寄存器4寄存器。 */
#define    HI_SC_CTRL63_OFFSET                               (0x4FC) /* PCIE0系统控制寄存器5寄存器。 */
#define    HI_SC_CTRL64_OFFSET                               (0x500) /* PCIE0系统控制寄存器6寄存器。 */
#define    HI_SC_CTRL65_OFFSET                               (0x504) /* PCIE0系统控制寄存器7寄存器。 */
#define    HI_SC_CTRL66_OFFSET                               (0x508) /* PCIE0系统控制寄存器8寄存器。 */
#define    HI_SC_CTRL67_OFFSET                               (0x50C) /* PCIE0系统控制寄存器9寄存器。 */
#define    HI_SC_CTRL73_OFFSET                               (0x524) /* func mbist使能寄存器0。 */
#define    HI_SC_CTRL74_OFFSET                               (0x528) /* func mbist使能寄存器1。 */
#define    HI_SC_CTRL75_OFFSET                               (0x52C) /* func mbist使能寄存器2。 */
#define    HI_SC_CTRL76_OFFSET                               (0x530) /* func mbist使能寄存器3。 */
#define    HI_SC_CTRL77_OFFSET                               (0x534) /* func mbist使能寄存器4。 */
#define    HI_SC_CTRL78_OFFSET                               (0x538) /* func mbist使能寄存器5。 */
#define    HI_SC_CTRL79_OFFSET                               (0x53C) /* func mbist使能寄存器6。 */
#define    HI_SC_CTRL80_OFFSET                               (0x540) /* func mbist使能寄存器7。 */
#define    HI_SC_CTRL82_OFFSET                               (0x548) /* appa9 gpio1中断按位使能寄存器。 */
#define    HI_SC_CTRL83_OFFSET                               (0x54C) /* appa9 gpio2中断按位使能寄存器。 */
#define    HI_SC_CTRL84_OFFSET                               (0x550) /* appa9 gpio3中断按位使能寄存器。 */
#define    HI_SC_CTRL86_OFFSET                               (0x558) /* mdma9 gpio1中断按位使能寄存器。 */
#define    HI_SC_CTRL87_OFFSET                               (0x55C) /* mdma9 gpio2中断按位使能寄存器。 */
#define    HI_SC_CTRL88_OFFSET                               (0x560) /* mdma9 gpio3中断按位使能寄存器。 */
#define    HI_SC_CTRL89_OFFSET                               (0x564) /* hifi gpio0中断按位使能寄存器。 */
#define    HI_SC_CTRL90_OFFSET                               (0x568) /* hifi gpio1中断按位使能寄存器。 */
#define    HI_SC_CTRL91_OFFSET                               (0x56C) /* hifi gpio2中断按位使能寄存器。 */
#define    HI_SC_CTRL92_OFFSET                               (0x570) /* hifi gpio3中断按位使能寄存器。 */
#define    HI_SC_CTRL93_OFFSET                               (0x574) /* dsp0 gpio0中断按位使能寄存器。 */
#define    HI_SC_CTRL94_OFFSET                               (0x578) /* dsp0 gpio1中断按位使能寄存器。 */
#define    HI_SC_CTRL95_OFFSET                               (0x57C) /* dsp0 gpio2中断按位使能寄存器。 */
#define    HI_SC_CTRL96_OFFSET                               (0x580) /* dsp0 gpio3中断按位使能寄存器。 */
#define    HI_SC_CTRL97_OFFSET                               (0x584) /* func mbist控制寄存器。 */
#define    HI_SC_CTRL99_OFFSET                               (0x58C) /* M3 GPIO1中断使能寄存器。 */
#define    HI_SC_CTRL100_OFFSET                              (0x590) /* M3 GPIO2中断使能寄存器。 */
#define    HI_SC_CTRL101_OFFSET                              (0x594) /* M3 GPIO3中断使能寄存器。 */
#define    HI_SC_CTRL102_OFFSET                              (0x598) /* 保留寄存器。 */
#define    HI_SC_CTRL106_OFFSET                              (0x5A8) /* PCIE0系统控制寄存器10寄存器。 */
#define    HI_SC_CTRL107_OFFSET                              (0x5AC) /* PCIE0系统控制寄存器11寄存器。 */
#define    HI_SC_CTRL108_OFFSET                              (0x5B0) /* PCIE和GMAC参考时钟配置寄存器 */
#define    HI_SC_CTRL109_OFFSET                              (0x5B4) /* PCIE PHY系统控制寄存器0寄存器。 */
#define    HI_SC_CTRL110_OFFSET                              (0x5B8) /* PCIE PHY系统控制寄存器1寄存器。 */
#define    HI_SC_CTRL111_OFFSET                              (0x5BC) /* PCIE PHY系统控制寄存器2寄存器。 */
#define    HI_SC_CTRL112_OFFSET                              (0x5C0) /* PCIE PHY系统控制寄存器3寄存器。 */
#define    HI_SC_CTRL113_OFFSET                              (0x5C4) /* PCIE0系统控制寄存器12寄存器 */
#define    HI_SC_CTRL114_OFFSET                              (0x5C8) /* PCIE0系统控制寄存器13寄存器 */
#define    HI_SC_CTRL115_OFFSET                              (0x5CC) /* PCIE0系统控制寄存器14寄存器 */
#define    HI_SC_CTRL116_OFFSET                              (0x5D0) /* PCIE0系统控制寄存器15寄存器 */
#define    HI_SC_CTRL117_OFFSET                              (0x5D4) /* PCIE0系统控制寄存器16寄存器 */
#define    HI_SC_CTRL118_OFFSET                              (0x5D8) /* PCIE0系统控制寄存器17寄存器 */
#define    HI_SC_CTRL119_OFFSET                              (0x5DC) /* PCIE0系统控制寄存器18寄存器 */
#define    HI_SC_CTRL120_OFFSET                              (0x5E0) /* PCIE0系统控制寄存器19寄存器 */


#ifndef __ASSEMBLY__

/********************************************************************************/
/*    sysreg_pd 寄存器定义（项目名_模块名_寄存器名_T)        */
/********************************************************************************/
typedef union
{
    struct
    {
        unsigned int    pcie_slv_awmisc_info       : 22; /* [21..0] PCIe（X1）控制器。bit[4:0]：AXI transaction’s TYPE.bit[5]：AXI Transaction’s BCM；bit[6]；AXI Transaction’s EP；bit[7]：AXI Transaction’s TD；bit[9:8]：AXI Transaction’s ATTR；bit[12:10]：AXI Transaction’s TC；bit[20:13]：AXI Transaction’s MSG code；bit[21]：是否使能选择配置的传输类型。0：选择通过地址映射计算的类型；1：选择寄存器配置的类型，也就是本寄存器的[4:0]。 */
        unsigned int    pcie_slv_resp_err_map      : 6; /* [27..22] PCIe（X1）控制器。 Slave响应的几种Error情况：bit[22]： CPL UR；bit[23]： Not used；bit[24]： CPL CRS；bit[25]： CPL Poisoned；bit[26]： CPL ECRC；bit[27]： CPL Abort (MLF or UC)。- UR = Unsupported request- CRS = Completion retry request- UC = Unexpected completion- MLF = Malformed */
        unsigned int    pcie_device_type           : 4; /* [31..28] PCIe（X1）控制器的类型。0000： PCI Express Endpoint device；0001： Legacy PCI Express Endpoint device；0100： Root Port of PCI Express Root Complex；其他：保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL58_T;    /* PCIE0系统控制寄存器0寄存器。 */

typedef union
{
    struct
    {
        unsigned int    pcie_slv_armisc_info       : 22; /* [21..0] PCIe（X1）控制器。[4:0]：AXI transaction’s TYPE.bit[5]：AXI Transaction’s BCM；bit[6]：AXI Transaction’s EP；bit[7]：AXI Transaction’s TD；bit[9:8]：AXI Transaction’s ATTR；bit[12:10]：AXI Transaction’s TC；bit[20:13]：AXI Transaction’s MSG code；bit[21]：是否使能选择配置的传输类型；0：选择通过地址映射计算的类型；1：选择寄存器配置的类型，也就是本寄存器的[4:0]。 */
        unsigned int    reserved                   : 1; /* [22..22] 保留 */
        unsigned int    pcie_app_clk_req_n         : 1; /* [23..23] PCIe（X1）控制器 应用层请求时钟有效0：申请时钟；1：不申请时钟； */
        unsigned int    pcie_ven_msg_code          : 8; /* [31..24] PCIe（X1）控制器Vendor定义的Message TLP的Message Core。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL59_T;    /* PCIE0系统控制寄存器1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    pcie_mstr_bmisc_info       : 14; /* [13..0] PCIe（X1）控制器AXI Master 写响应的misc信息bit[1:0]：AXI response’s attributesbit[2]；AXI response with Bad EOT assigned to signal native PCIecore to drop this responsebit[5:3]： AXI response’s TCbit[6]：AXI response’s BCMbit[7]：AXI response’s EPbit[8]：AXI response’s TDbit[11:9]：AXI response’s Function number                           bit[13：12]：Master Response Error Map. */
        unsigned int    pcie_mstr_rmisc_info       : 12; /* [25..14] PCIe（X1）控制器AXI Master 读响应的misc信息bit[15:14]：AXI response’s attributes；bit[16]：AXI response with Bad EOT； assigned to signal native PCIecore to drop this response；bit[19:17]：AXI response’s TC；bit[20]：AXI response’s BCM；bit[21]：AXI response’s EP；bit[22]：AXI response’s TD；bit[25:23]：AXI response’s Function number。 */
        unsigned int    pcie_ven_msi_req           : 1; /* [26..26] PCIe（X1）控制器。表明应用层是否请求发送MSI。0：无请求；1：请求。 */
        unsigned int    pcie_ven_msi_vector        : 5; /* [31..27] PCIe（X1）控制器当多Message模式使能的时候，用来调整MSI数据寄存器的最低5bits； */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL60_T;    /* PCIE0系统控制寄存器2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    pcie_ven_msg_req           : 1; /* [0..0] PCIe（X1）控制器。应用层请求发送一个Vendor 定义的Message，一旦req有效，必须要保持到ven_msg_grant有效。0：不请求；1：请求。 */
        unsigned int    pcie_ven_msg_fmt           : 2; /* [2..1] PCIe（X1）控制器。Vendor 定义的Message TLP的Format bit域。 */
        unsigned int    pcie_ven_msg_type          : 5; /* [7..3] PCIe（X1）控制器。Vendor 定义的Message TLP的Type bit域。 */
        unsigned int    pcie_ven_msg_td            : 1; /* [8..8] PCIe（X1）控制器。Vendor 定义的Message TLP的Digest bit域。 */
        unsigned int    pcie_ven_msg_ep            : 1; /* [9..9] PCIe（X1）控制器。Vendor 定义的Message TLP的EP。 */
        unsigned int    pcie_ven_msg_attr          : 2; /* [11..10] PCIe（X1）控制器。Vendor 定义的Message TLP的Attributes域。 */
        unsigned int    pcie_ven_msg_len           : 10; /* [21..12] PCIe（X1）控制器。Vendor 定义的Message TLP的Length域。 */
        unsigned int    pcie_ven_msg_tag           : 8; /* [29..22] PCIe（X1）控制器。Vendor 定义的Message TLP的Tag。 */
        unsigned int    reserved                   : 2; /* [31..30] 保留，读反馈0，写无效。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL61_T;    /* PCIE0系统控制寄存器3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    pice_ven_msg_data_low      : 32; /* [31..0] PCIe（X1）控制器。Vendor定义的Message TLP的Message 数据的低32bit。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL62_T;    /* PCIE0系统控制寄存器4寄存器。 */

typedef union
{
    struct
    {
        unsigned int    pice_ven_msg_data_high     : 32; /* [31..0] PCIe（X1）控制器。Vendor定义的Message TLP的Message 数据的高32bit。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL63_T;    /* PCIE0系统控制寄存器5寄存器。 */

typedef union
{
    struct
    {
        unsigned int    slot_led_blink_period      : 32; /* [31..0] Slot状态指示LED闪烁周期控制：LED灯闪烁周期＝该寄存器的值×Period_of_pcie_clk(8ns)。. */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL64_T;    /* PCIE0系统控制寄存器6寄存器。 */

typedef union
{
    struct
    {
        unsigned int    pcie_app_init_rst          : 1; /* [0..0] PCIe（X1）控制器。应用层请求发送一个Hot Reset给下游端口设备。0：无效；1：有效。 */
        unsigned int    pcie_app_req_entr_l1       : 1; /* [1..1] PCIe（X1）控制器。应用层请求进入L1状态；0：无效；1：有效。 */
        unsigned int    pcie_app_ready_entr_l23    : 1; /* [2..2] PCIe（X1）控制器。应用层准备进入L23状态；0：无效；1：有效。 */
        unsigned int    pcie_app_req_exit_l1       : 1; /* [3..3] PCIe（X1）控制器。应用层请求退出L1状态；0：无效；1：有效。 */
        unsigned int    pcie_app_req_retry_en      : 1; /* [4..4] PCIe（X1）控制器。当这个信号有效，Core完成输入配置请求。0：无效；1：有效。 */
        unsigned int    pcie_sys_int               : 1; /* [5..5] PCIe（X1）控制器。当sys_int从低变高的时候，Core产生一个Assert_INTx Message。当sys_int从高变低的时候，Core产生一个Deassert_INTx Message。 */
        unsigned int    pcie_outband_pwrup_cmd     : 1; /* [6..6] PCIe（X1）控制器。应用层用来将PMC状态机从低功耗状态唤醒；需要有效一个cycle。0：无效；1：有效。 */
        unsigned int    pcie_app_unlock_msg        : 1; /* [7..7] PCIe（X1）控制器。应用层请求产生一个Unlock Message。有效一个cycle，Core不会反馈确认信息。0：无效；1：有效。 */
        unsigned int    pcie_apps_pm_xmt_turnoff   : 1; /* [8..8] PCIe（X1）控制器。应用层请求产生一个PM_Turm_off Message。有效一个cycle。Core不会反馈确认信息。0：无效；1：有效。 */
        unsigned int    pcie_apps_pm_xmt_pme       : 1; /* [9..9] PCIe（X1）控制器。应用逻辑将Core从低功耗状态唤醒。0：无效；1：有效。 */
        unsigned int    pcie_sys_aux_pwr_det       : 1; /* [10..10] PCIe（X1）控制器。辅助电源侦测，表明是否存在辅助电源。0：不存在；1：存在。 */
        unsigned int    pcie_app_ltssm_enable      : 1; /* [11..11] PCIe（X1）控制器。应用层需要在复位保持LTSSM在侦测状态，驱动这个信号为0，直到应用层准备好。 */
        unsigned int    pcie_cfg_pwr_ctrler_ctrl_pol : 1; /* [12..12] PCIe（X1）控制器。Socket电源控制极性控制。0:低有效；1:高有效。 */
        unsigned int    reserved_2                 : 1; /* [13..13] 保留 */
        unsigned int    pcie_sys_mrl_sensor_state  : 1; /* [14..14] PCIe（X1）控制器。表明MRL传感器状况（如果有MRL传感器的话）；0：MRL关；1：MRL开。 */
        unsigned int    pcie_sys_pwr_fault_det     : 1; /* [15..15] PCIe（X1）控制器。表明电源控制器在插槽中侦测到电源故障；0：未侦测到；1：侦测到。 */
        unsigned int    pcie_sys_mrl_sensor_chged  : 1; /* [16..16] PCIe（X1）控制器。表明MRL sensor的状态是否改变；0：未改变；1：改变。 */
        unsigned int    reserved_1                 : 1; /* [17..17] 保留 */
        unsigned int    pcie_sys_cmd_cpled_int     : 1; /* [18..18] PCIe（X1）控制器。命令完成中断。表明Hot-Plug控制器完成一个命令。0：中断无效；1：中断有效。 */
        unsigned int    pcie_sys_eml_interlock_engaged : 1; /* [19..19] PCIe DM控制器输入端信号。表明是否使用了机电互锁机制。               0：未使用                         1：使用 */
        unsigned int    reserved_0                 : 1; /* [20..20] 保留 */
        unsigned int    pcie_int_ctrl              : 8; /* [28..21] PCIe（X1）控制器中断屏蔽，高有效[0]:对应cfg_aer_rc_err_int[1]:对应ven_msi_int[2]:对应misc_int[3]:对应cfg_pme_int[4]:选择中断inta共用中断脚[5]:选择中断intb或者MSI共用中断脚[6]:选择中断intc共用中断脚[7]:选择中断intd共用中断脚 */
        unsigned int    pcie_linkdown_auto_rstn_disable : 1; /* [29..29] LINKDOWN事件自动启动链路训练禁止控制。0：允许重启链路训练；1：禁止重启链路训练。 */
        unsigned int    pcie_err_bresp_enable      : 1; /* [30..30] PCIe控制器AXI slave接口写反馈响应(BRESP）错误上报使能控制。0：BRESP固定为2'h0；1：如实上报BRESP。 */
        unsigned int    cipe_err_rresp_enable      : 1; /* [31..31] PCIe控制器AXI slave接口读响应(RRESP）错误上报使能控制信号。0：RRESP固定为2'h0；1：如实上报RRESP。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL65_T;    /* PCIE0系统控制寄存器7寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 3; /* [2..0] 保留 */
        unsigned int    cfg_l1_aux_clk_switch_core_clk_gate_en : 1; /* [3..3] 辅助时钟(auxclk)切换信号的一个使能控制分量。当设备状态为D0或D1，链路状态为L1时，该信号控制PCIe core工作时钟是否切换至辅助时钟。0：不切换至辅助时钟；1：切换至辅助时钟。 */
        unsigned int    cfg_l1_mac_powerdown_override_to_p2_en : 1; /* [4..4] 在链路状态为L1期间，在即将移除参考时钟（refclk）时，该信号控制是否使用P2替代P1。0：不替代；1：替代。 */
        unsigned int    pcie_state2_sel            : 2; /* [6..5] PCIe状态寄存器2寄存器（PERI_PCIE_STAT2）输入端MUX选择信号 */
        unsigned int    pcie_state3_sel            : 2; /* [8..7] PCIe状态寄存器3寄存器（PERI_PCIE_STAT3）输入端MUX选择信号 */
        unsigned int    reserved_0                 : 23; /* [31..9] 保留 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL66_T;    /* PCIE0系统控制寄存器8寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 32; /* [31..0] 保留 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL67_T;    /* PCIE0系统控制寄存器9寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 32; /* [31..0] func mbist使能寄存器。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL73_T;    /* func mbist使能寄存器0。 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 32; /* [31..0] func mbist使能寄存器。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL74_T;    /* func mbist使能寄存器1。 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 32; /* [31..0] func mbist使能寄存器。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL75_T;    /* func mbist使能寄存器2。 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 32; /* [31..0] func mbist使能寄存器。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL76_T;    /* func mbist使能寄存器3。 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 32; /* [31..0] func mbist使能寄存器。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL77_T;    /* func mbist使能寄存器4。 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 32; /* [31..0] func mbist使能寄存器。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL78_T;    /* func mbist使能寄存器5。 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 32; /* [31..0] func mbist使能寄存器。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL79_T;    /* func mbist使能寄存器6。 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 32; /* [31..0] func mbist使能寄存器。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL80_T;    /* func mbist使能寄存器7。 */

typedef union
{
    struct
    {
        unsigned int    appa9_gpio1_en             : 32; /* [31..0] 每1bit控制1bit中断。0：中断使能；1：中断不使能； */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL82_T;    /* appa9 gpio1中断按位使能寄存器。 */

typedef union
{
    struct
    {
        unsigned int    appa9_gpio2_en             : 32; /* [31..0] 每1bit控制1bit中断。0：中断使能；1：中断不使能； */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL83_T;    /* appa9 gpio2中断按位使能寄存器。 */

typedef union
{
    struct
    {
        unsigned int    appa9_gpio3_en             : 32; /* [31..0] 每1bit控制1bit中断。0：中断使能；1：中断不使能； */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL84_T;    /* appa9 gpio3中断按位使能寄存器。 */

typedef union
{
    struct
    {
        unsigned int    mdma9_gpio1_en             : 32; /* [31..0] 每1bit控制1bit中断。0：中断使能；1：中断不使能； */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL86_T;    /* mdma9 gpio1中断按位使能寄存器。 */

typedef union
{
    struct
    {
        unsigned int    mdma9_gpio2_en             : 32; /* [31..0] 每1bit控制1bit中断。0：中断使能；1：中断不使能； */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL87_T;    /* mdma9 gpio2中断按位使能寄存器。 */

typedef union
{
    struct
    {
        unsigned int    mdma9_gpio3_en             : 32; /* [31..0] 每1bit控制1bit中断。0：中断使能；1：中断不使能； */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL88_T;    /* mdma9 gpio3中断按位使能寄存器。 */

typedef union
{
    struct
    {
        unsigned int    hifi_gpio0_en              : 32; /* [31..0] 每1bit控制1bit中断。0：中断使能；1：中断不使能； */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL89_T;    /* hifi gpio0中断按位使能寄存器。 */

typedef union
{
    struct
    {
        unsigned int    hifi_gpio1_en              : 32; /* [31..0] 每1bit控制1bit中断。0：中断使能；1：中断不使能； */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL90_T;    /* hifi gpio1中断按位使能寄存器。 */

typedef union
{
    struct
    {
        unsigned int    hifi_gpio2_en              : 32; /* [31..0] 每1bit控制1bit中断。0：中断使能；1：中断不使能； */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL91_T;    /* hifi gpio2中断按位使能寄存器。 */

typedef union
{
    struct
    {
        unsigned int    hifi_gpio3_en              : 32; /* [31..0] 每1bit控制1bit中断。0：中断使能；1：中断不使能； */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL92_T;    /* hifi gpio3中断按位使能寄存器。 */

typedef union
{
    struct
    {
        unsigned int    dsp0_gpio0_en              : 32; /* [31..0] 每1bit控制1bit中断。0：中断使能；1：中断不使能； */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL93_T;    /* dsp0 gpio0中断按位使能寄存器。 */

typedef union
{
    struct
    {
        unsigned int    dsp0_gpio1_en              : 32; /* [31..0] 每1bit控制1bit中断。0：中断使能；1：中断不使能； */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL94_T;    /* dsp0 gpio1中断按位使能寄存器。 */

typedef union
{
    struct
    {
        unsigned int    dsp0_gpio2_en              : 32; /* [31..0] 每1bit控制1bit中断。0：中断使能；1：中断不使能； */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL95_T;    /* dsp0 gpio2中断按位使能寄存器。 */

typedef union
{
    struct
    {
        unsigned int    dsp0_gpio3_en              : 32; /* [31..0] 每1bit控制1bit中断。0：中断使能；1：中断不使能； */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL96_T;    /* dsp0 gpio3中断按位使能寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 32; /* [31..0] 每1bit控制1bit中断。0：中断使能；1：中断不使能； */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL97_T;    /* func mbist控制寄存器。 */

typedef union
{
    struct
    {
        unsigned int    cm3_gpio1_en               : 32; /* [31..0] M3 GPIO1中断使能寄存器。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL99_T;    /* M3 GPIO1中断使能寄存器。 */

typedef union
{
    struct
    {
        unsigned int    cm3_gpio2_en               : 32; /* [31..0] M3 GPIO2中断使能寄存器。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL100_T;    /* M3 GPIO2中断使能寄存器。 */

typedef union
{
    struct
    {
        unsigned int    cm3_gpio3_en               : 32; /* [31..0] M3 GPIO3中断使能寄存器。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL101_T;    /* M3 GPIO3中断使能寄存器。 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 32; /* [31..0] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL102_T;    /* 保留寄存器。 */

typedef union
{
    struct
    {
        unsigned int    cfg_aer_rc_err_msi_mask    : 1; /* [0..0] cfg_aer_rc_err_msi中断屏蔽 */
        unsigned int    cfg_sys_err_rc_mask        : 1; /* [1..1] cfg_sys_err_rc中断屏蔽 */
        unsigned int    radm_correctable_err_mask  : 1; /* [2..2] radm_correctable_err中断屏蔽 */
        unsigned int    radm_nonfatal_err_mask     : 1; /* [3..3] radm_nonfatal_err中断屏蔽 */
        unsigned int    radm_fatal_err_mask        : 1; /* [4..4] radm_fatal_err中断屏蔽 */
        unsigned int    radm_pm_pme_mask           : 1; /* [5..5] radm_pm_pme中断屏蔽 */
        unsigned int    radm_pm_to_ack_mask        : 1; /* [6..6] radm_pm_to_ack中断屏蔽 */
        unsigned int    ven_msg_int_mask           : 1; /* [7..7] ven_msg_int中断屏蔽 */
        unsigned int    radm_cpl_timeout_mask      : 1; /* [8..8] radm_cpl_timeout中断屏蔽 */
        unsigned int    radm_msg_unlock_mask       : 1; /* [9..9] radm_msg_unlock中断屏蔽 */
        unsigned int    cfg_pme_msi_mask           : 1; /* [10..10] cfg_pme_msi中断屏蔽 */
        unsigned int    bridge_flush_not_mask      : 1; /* [11..11] bridge_flush_not中断屏蔽 */
        unsigned int    link_req_rst_not_mask      : 1; /* [12..12] link_req_rst_not中断屏蔽 */
        unsigned int    pcie_p2_exit_reg_mask      : 1; /* [13..13] pcie从P2低功耗退出中断屏蔽 */
        unsigned int    reserved_1                 : 7; /* [20..14]  */
        unsigned int    pcie_radm_pm_turnoff_mask  : 1; /* [21..21] PCIe控制器radm_pm_turnoff中断屏蔽 */
        unsigned int    reserved_0                 : 10; /* [31..22] 保留 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL106_T;    /* PCIE0系统控制寄存器10寄存器。 */

typedef union
{
    struct
    {
        unsigned int    pcie_cfg_aer_rc_msi_clr    : 1; /* [0..0] cfg_aer_rc_err_msi中断清除 */
        unsigned int    pcie_cfg_sys_err_rc_clr    : 1; /* [1..1] cfg_sys_err_rc中断清除 */
        unsigned int    pcie_radm_corretable_err_clr : 1; /* [2..2] radm_correctable_err中断清除 */
        unsigned int    pcie_radm_nonfatal_err_clr : 1; /* [3..3] radm_nonfatal_err中断清除 */
        unsigned int    pcie_radm_fatal_err_clr    : 1; /* [4..4] radm_fatal_err中断清除 */
        unsigned int    pcie_radm_pm_pme_clr       : 1; /* [5..5] radm_pm_pme中断清除 */
        unsigned int    pcie_radm_pm_to_ack_clr    : 1; /* [6..6] radm_pm_to_ack中断清除 */
        unsigned int    pcie_ven_msg_int_clr       : 1; /* [7..7] ven_msg_int中断清除 */
        unsigned int    pcie_radm_cpl_timeout_clr  : 1; /* [8..8] radm_cpl_timeout中断清除 */
        unsigned int    pcie_radm_msg_unlock_clr   : 1; /* [9..9] radm_msg_unlock中断清除 */
        unsigned int    pcie_cfg_pme_msi_clr       : 1; /* [10..10] cfg_pme_msi中断清除 */
        unsigned int    pcie_bridge_flush_not_clr  : 1; /* [11..11] bridge_flush_not中断清除 */
        unsigned int    pcie_link_req_rst_not_clr  : 1; /* [12..12] link_req_rst_not中断清除 */
        unsigned int    pcie_p2_exit_int_clr       : 1; /* [13..13] PCIec从P2低功耗退出中断清除 */
        unsigned int    pcie_slv_err_int_clr       : 1; /* [14..14] PCIe控制器AXI slave错误中断清除 */
        unsigned int    pcie_ven_msi_func_num      : 3; /* [17..15] PCIe控制器Vendor MSI功能号 */
        unsigned int    pcie_ven_msg_func_num      : 3; /* [20..18] PCIe控制器Vendor Message功能号 */
        unsigned int    pcie_radm_pm_turnoff_clr   : 1; /* [21..21] PCIe控制器radm_pm_turnoff中断清除 */
        unsigned int    reserved                   : 10; /* [31..22] 保留 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL107_T;    /* PCIE0系统控制寄存器11寄存器。 */

typedef union
{
    struct
    {
        unsigned int    pcie0_intx_req             : 1; /* [0..0]  */
        unsigned int    pcie0_msi_req              : 1; /* [1..1]  */
        unsigned int    reserved_5                 : 2; /* [3..2] 保留。 */
        unsigned int    pcie0_pad_phy_mask         : 1; /* [4..4]  */
        unsigned int    pcie0_pad_out_mask         : 1; /* [5..5]  */
        unsigned int    reserved_4                 : 2; /* [7..6] 保留。 */
        unsigned int    pcie0_inner_phy_mask       : 1; /* [8..8]  */
        unsigned int    pcie0_inner_out_mask       : 1; /* [9..9]  */
        unsigned int    reserved_3                 : 2; /* [11..10] 保留。 */
        unsigned int    pcie0_out_gatedclock_en    : 1; /* [12..12]  */
        unsigned int    pcie0_phy_gatedclock_en    : 1; /* [13..13]  */
        unsigned int    reserved_2                 : 2; /* [15..14] 保留。 */
        unsigned int    pcie0_refclk_oen_sel       : 1; /* [16..16] PCIE0参考时钟输出使能选择。0：来自系统控制器配置（pcie0_refclk_oen_cfg）；1：来自PCIE控制器输出。 */
        unsigned int    pcie0_refclk_oen_cfg       : 1; /* [17..17] PCIE0参考时钟输出使能。0：不使能；1：使能。 */
        unsigned int    gmac_refclk_sel            : 1; /* [18..18] 输出给gmac对接器件的参考时钟。0：25MHz；1:125MHz。 */
        unsigned int    reserved_1                 : 1; /* [19..19] 保留。 */
        unsigned int    reserved_0                 : 12; /* [31..20]  */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL108_T;    /* PCIE和GMAC参考时钟配置寄存器 */

typedef union
{
    struct
    {
        unsigned int    pipe_tx2rx_loopbk          : 1; /* [0..0] PHY输入端口信号，PIPE发送（TX）环回到接收（RX） */
        unsigned int    pipe_pclkreq_n             : 2; /* [2..1] 时钟请求，低有效。0:No clock removal1: Clock removal for executing L1 with clock PM2: Clock removal for executeing L1 with substates3: Clock removal for executing L1 with substates */
        unsigned int    pipe_pclkreq_n_sel         : 1; /* [3..3] pipe_pclkreq_n[0] MUX选择。0: 选用mac_pclkreq_n1: 选用pipe_pclkreq_n[0] */
        unsigned int    pcs_clk_req                : 1; /* [4..4] 外部逻辑请求PHY mpll_dword_clk。0: 无请求1: 请求 */
        unsigned int    pcs_common_clocks          : 1; /* [5..5] 为接收数据通路选择公共时钟模式。0: 不使用公共时钟1: 使用公共时钟 */
        unsigned int    pipe_port_sel              : 1; /* [6..6] PIPE使能请求。0: 禁能PIPE1: 使能PIPE */
        unsigned int    reserved_2                 : 1; /* [7..7] 保留。 */
        unsigned int    pipe_rx_elecidle_disable   : 1; /* [8..8] 禁能PIPE接收器电器空闲检测电路。0: 使能1: 禁能 */
        unsigned int    pipe_tx_common_mode_disable : 1; /* [9..9] 禁能发送(TX)共模保持电路。0: 使能1: 禁能 */
        unsigned int    pipe_clkreq_n_cfg          : 1; /* [10..10] 软件配置的phy_clkreq_n，在pipe_clkreq_n_sel为1时有效。0: 不请求时钟1: 请求时钟 */
        unsigned int    pipe_clkreq_n_sel          : 1; /* [11..11] phy_clkreq_n信号来源选择。0: 使用逻辑产生的pipe_clkreq_n1: 使用软件配置的pipe_clkreq_n_cfg */
        unsigned int    reserved_1                 : 4; /* [15..12] 保留。 */
        unsigned int    phy_res_ack_in_cfg         : 1; /* [16..16] 软件配置的phy_res_ack_in信号。 */
        unsigned int    phy_res_ack_in_sel         : 1; /* [17..17] phy_res_ack_in信号来源选择。0: 使用逻辑产生的phy_res_req_out1: 使用软件配置的phy_res_ack_in_cfg */
        unsigned int    phy_res_req_in             : 1; /* [18..18] PHY输入端口信号。 */
        unsigned int    phy_rtune_req              : 1; /* [19..19] PHY输入端口信号。 */
        unsigned int    phy_test_burnin            : 1; /* [20..20] PHY输入端口信号。 */
        unsigned int    phy_test_bypass            : 1; /* [21..21] PHY输入端口信号。 */
        unsigned int    phy_test_powerdown         : 1; /* [22..22] PHY输入端口信号。 */
        unsigned int    reserved_0                 : 9; /* [31..23] 保留 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL109_T;    /* PCIE PHY系统控制寄存器0寄存器。 */

typedef union
{
    struct
    {
        unsigned int    phy_mpll_multiplier        : 7; /* [6..0] PHY输入端口信号。 */
        unsigned int    phy_ref_clkdiv2            : 1; /* [7..7] PHY输入端口信号。 */
        unsigned int    phy_ref_use_pad            : 1; /* [8..8] PHY输入端口信号。 */
        unsigned int    ref_alt_clk_gt_en          : 1; /* [9..9] ref_alt_clk时钟门控使能。0: 不使能1: 使能 */
        unsigned int    reserved_1                 : 6; /* [15..10] 保留。 */
        unsigned int    phy_reset                  : 1; /* [16..16] PHY复位信号，高有效，在phy_reset_sel为0时有效。 */
        unsigned int    phy_reset_sel              : 1; /* [17..17] phy_reset信号来源选择。0: 使用软件配置的phy_reset1:  使用取反后的pipe_reset_n */
        unsigned int    pipe_reset_n_cfg           : 1; /* [18..18] 软件配置的pipe_reset_n，低有效，在pipe_reset_sel为1时有效。 */
        unsigned int    pipe_reset_sel             : 1; /* [19..19] pipe_reset_n信号来源选择。0: 使用取反后的pipe_reset_n1: 使用软件配置的phy_reset */
        unsigned int    reserved_0                 : 12; /* [31..20] 保留 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL110_T;    /* PCIE PHY系统控制寄存器1寄存器。 */

typedef union
{
    struct
    {
        unsigned int    phy_los_bias               : 3; /* [2..0] PHY输入端口信号。 */
        unsigned int    phy_los_level              : 5; /* [7..3] PHY输入端口信号。 */
        unsigned int    phy_rx_eq                  : 3; /* [10..8] PHY输入端口信号。 */
        unsigned int    pcs_tx_deemph_gen2_3p5db   : 6; /* [16..11] PHY输入端口信号。 */
        unsigned int    pcs_tx_deemph_gen2_6db     : 6; /* [22..17] PHY输入端口信号。 */
        unsigned int    reserved_1                 : 1; /* [23..23] 保留 */
        unsigned int    pcs_tx_deemph_gen1         : 6; /* [29..24] PHY输入端口信号。 */
        unsigned int    reserved_0                 : 2; /* [31..30] 保留 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL111_T;    /* PCIE PHY系统控制寄存器2寄存器。 */

typedef union
{
    struct
    {
        unsigned int    pcs_tx_swing_full          : 7; /* [6..0] PHY输入端口信号。 */
        unsigned int    pcs_tx_swing_low           : 7; /* [13..7] PHY输入端口信号。 */
        unsigned int    phy_tx_term_offset         : 5; /* [18..14] PHY输入端口信号。 */
        unsigned int    reserved_2                 : 5; /* [23..19] 保留。 */
        unsigned int    phy_tx_vboost_lvl          : 3; /* [26..24] PHY输入端口信号。 */
        unsigned int    reserved_1                 : 1; /* [27..27] 保留 */
        unsigned int    vreg_bypass                : 1; /* [28..28] PHY输入端口信号。 */
        unsigned int    reserved_0                 : 3; /* [31..29] 保留 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL112_T;    /* PCIE PHY系统控制寄存器3寄存器。 */

typedef union
{
    struct
    {
        unsigned int    pcie_perst_out_n           : 1; /* [0..0] PCIe PERST输出，在pcie_perst_oe为高时生效。PCIe PERST#为低有效复位信号。 */
        unsigned int    pcie_perst_oe              : 1; /* [1..1] PCIe PERST输出使能，高有效。 */
        unsigned int    pcie_perst_in_n_ctrl       : 2; /* [3..2] 对pcie_ClK_RST模块pcie_perst_in_n进行门控的控制信号。00:门控到0；01:门控到1；10:门控到0；11:使用原值；注:1：该字段决定PCIe模块是否受PERST#复位引脚输入信号的影响。注2：该字段不影响pcie_perst_oe和pcie_perst_out_n的值，即不影响PERST#复位引脚输出信号。注3：在RC模式，该字段设为01。注4：在EP模式，该字段设为11。 */
        unsigned int    mac_phy_pclkreq_n_0_ctrl   : 1; /* [4..4] 对mac_phy_pclkreq_n[0]进行门控的控制信号：0:门控到0；1:使用原值。 */
        unsigned int    mac_phy_pclkreq_n_1_ctrl   : 1; /* [5..5] 对mac_phy_pclkreq_n[1]进行门控的控制信号：0:门控到0；1:使用原值。 */
        unsigned int    cfg_l1sub_en_ctrl          : 2; /* [7..6] 对cfg_l1sub_en进行门控的控制信号：00:使用原值；01:门控到1；10:门控到0；11:门控到0。 */
        unsigned int    clkreq_n_ctrl0             : 2; /* [9..8] 对clkreq_n进行门控的控制信号：00:使用原值；01:门控到1；10:门控到0；11:门控到0。注1：clkreq_n经此门控后驱动CLKREQ#引脚。 */
        unsigned int    clkreq_n_ctrl1             : 2; /* [11..10] 对clkreq_n进行门控的控制信号：00:使用原值；01:门控到1；10:门控到0；11:门控到0。注1：clkreq_n经此门控后驱动CLKREQ#引脚。 */
        unsigned int    rc_wake_n_ctrl             : 1; /* [12..12] 对rc_wake_n进行门控的控制信号：0:使用原值；1:门控到1； */
        unsigned int    ep_wake_n_ctrl             : 1; /* [13..13] 对ep_wake_n进行门控的控制信号：0:使用原值；1:门控到1； */
        unsigned int    obff_enc_owre_wake_ctrl    : 1; /* [14..14] 对obff_enc_owre_wake进行门控的控制信号：0:使用原值；1:门控到1； */
        unsigned int    dwc_pcie_dm_wake_n_ctrl    : 2; /* [16..15] 对dwc_pcie_dm_wake_n进行门控的控制信号：00:使用原值；01:门控到1；10:门控到0；11:门控到0； */
        unsigned int    reserved                   : 15; /* [31..17] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL113_T;    /* PCIE0系统控制寄存器12寄存器 */

typedef union
{
    struct
    {
        unsigned int    obff_dec_app_init          : 1; /* [0..0] 应用逻辑对obff_dec_filter内部状态机进行初始化的使能控制信号。0:不使能；1:使能；注1：该信号与obff_dec_app_idle，obff_dec_app_obff和obff_dec_app_cpu_act配合，将obff_dec_filter内部状态机初始化到相应的状态。 */
        unsigned int    obff_dec_app_idle          : 1; /* [1..1] 应用逻辑对obff_dec_filter内部状态机进行初始化的状态选择信号，在obff_dec_app_init为1时生效。0:无效；1:选择IDLE状态； */
        unsigned int    obff_dec_app_obff          : 1; /* [2..2] 应用逻辑对obff_dec_filter内部状态机进行初始化的状态选择信号，在obff_dec_app_init为1时生效。0:无效；1:选择OBFF状态； */
        unsigned int    obff_dec_app_cpu_act       : 1; /* [3..3] 应用逻辑对obff_dec_filter内部状态机进行初始化的状态选择信号，在obff_dec_app_init为1时生效。0:无效；1:选择CPU_ACT状态； */
        unsigned int    obff_enc_app_owre_idle     : 1; /* [4..4] 应用逻辑请求obff_enc在WAKE#引脚上发送IDLE obff消息，上升沿有效。注1：软件在需要发送IDLE obff消息时，需在该位上产生一个上升沿。 */
        unsigned int    obff_enc_app_owre_obff     : 1; /* [5..5] 应用逻辑请求obff_enc在WAKE#引脚上发送OBFF obff消息，上升沿有效。注1：软件在需要发送OBFF obff消息时，需在该位上产生一个上升沿。 */
        unsigned int    obff_enc_app_owre_cpu_act  : 1; /* [6..6] 应用逻辑请求obff_enc在WAKE#引脚上发送CPU_ACTIVE obff消息，上升沿有效。注1：软件在需要发送CPU_ACTIVE obff消息时，需在该位上产生一个上升沿。 */
        unsigned int    app_obff_idle_msg_req      : 1; /* [7..7] 应用逻辑请求发送IDLE obff消息TLP包，上升沿有效。注1：软件在需要发送IDLE obff消息时，需在该位上产生一个上升沿。 */
        unsigned int    app_obff_obff_msg_req      : 1; /* [8..8] 应用逻辑请求发送OBFF obff消息TLP包，上升沿有效。注1：软件在需要发送OBFF obff消息时，需在该位上产生一个上升沿。 */
        unsigned int    app_obff_cpu_active_msg_req : 1; /* [9..9] 应用逻辑请求发送CPU_ACTIVE obff消息TLP包，上升沿有效。注1：软件在需要发送CPU_ACTIVE obff消息时，需在该位上产生一个上升沿。 */
        unsigned int    app_ltr_msg_req            : 1; /* [10..10] 应用逻辑请求发送LTR消息TLP包，上升沿有效。注1：软件在需要发送LTR消息时，需在该位上产生一个上升沿。 */
        unsigned int    reserved                   : 21; /* [31..11] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL114_T;    /* PCIE0系统控制寄存器13寄存器 */

typedef union
{
    struct
    {
        unsigned int    obff_cfg_wk_max_pls_wdt    : 8; /* [7..0] 配置OBFF译码器/编码器使用的active-inactive脉冲的最大宽度。注1：单位为19.2MHz时钟的一个周期。 */
        unsigned int    obff_cfg_wk_min_pls_wdt    : 8; /* [15..8] 配置OBFF译码器/编码器使用的active-inactive脉冲的最小宽度。注1：单位为19.2MHz时钟的一个周期。 */
        unsigned int    obff_cfg_wk_max_f2f_wdt    : 8; /* [23..16] 配置OBFF译码器/编码器使用的最大的下降沿到下降沿的宽度。注1：单位为19.2MHz时钟的一个周期。 */
        unsigned int    obff_cfg_wk_min_f2f_wdt    : 8; /* [31..24] 配置OBFF译码器/编码器使用的最小的下降沿到下降沿的宽度。注1：单位为19.2MHz时钟的一个周期。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL115_T;    /* PCIE0系统控制寄存器14寄存器 */

typedef union
{
    struct
    {
        unsigned int    obff_dec_idle_intr_mask    : 1; /* [0..0] obff_dec_idle中断屏蔽位，低有效。0:屏蔽；1:不屏蔽； */
        unsigned int    obff_dec_obff_intr_mask    : 1; /* [1..1] obff_dec_obff中断屏蔽位，低有效。0:屏蔽；1:不屏蔽； */
        unsigned int    obff_dec_cpu_active_intr_mask : 1; /* [2..2] obff_dec_cpu_active中断屏蔽位，低有效。0:屏蔽；1:不屏蔽； */
        unsigned int    obff_dec_err_intr_mask     : 1; /* [3..3] obff_dec_err中断屏蔽位，低有效。0:屏蔽；1:不屏蔽； */
        unsigned int    obff_msg_idle_intr_mask    : 1; /* [4..4] obff_msg_idle中断屏蔽位，低有效。0:屏蔽；1:不屏蔽； */
        unsigned int    obff_msg_obff_intr_mask    : 1; /* [5..5] obff_msg_obff中断屏蔽位，低有效。0:屏蔽；1:不屏蔽； */
        unsigned int    obff_msg_cpu_active_intr_mask : 1; /* [6..6] obff_msg_cpu_active中断屏蔽位，低有效。0:屏蔽；1:不屏蔽； */
        unsigned int    radm_msg_ltr_intr_mask     : 1; /* [7..7] radm_msg_ltr中断屏蔽位，低有效。0:屏蔽；1:不屏蔽； */
        unsigned int    cfg_msi_mask_chg_intr_mask : 1; /* [8..8] cfg_msi_mask_chg中断屏蔽位，低有效。0:屏蔽；1:不屏蔽； */
        unsigned int    radm_qoverflow_intr_mask   : 1; /* [9..9] radm_qoverflow中断屏蔽位，低有效。0:屏蔽；1:不屏蔽； */
        unsigned int    amba_order_mgr_wdt_int_intr_mask : 1; /* [10..10] amba_order_mgr_wdt_int中断屏蔽位，低有效。0:屏蔽；1:不屏蔽； */
        unsigned int    pcie_clkreq_in_n_intr_mask : 1; /* [11..11] pcie_clkreq_in_n中断屏蔽位，低有效。0:屏蔽；1:不屏蔽； */
        unsigned int    pcie_clkreq_in_n_inv_intr_mask : 1; /* [12..12] pcie_clkreq_in_n_inv中断屏蔽位，低有效。0:屏蔽；1:不屏蔽； */
        unsigned int    pcie_v721_intr_mask        : 1; /* [13..13] pcie_v721中断屏蔽位，低有效。0:屏蔽；1:不屏蔽； */
        unsigned int    reserved_1                 : 2; /* [15..14] 保留。 */
        unsigned int    obff_dec_idle_intr_clear   : 1; /* [16..16] obff_dec_idle中断清除位，上升沿有效。注1:该位在写1后不会自清0，需要软件显式写0。 */
        unsigned int    obff_dec_obff_intr_clear   : 1; /* [17..17] obff_dec_obff中断清除位，上升沿有效。注1:该位在写1后不会自清0，需要软件显式写0。 */
        unsigned int    obff_dec_cpu_active_intr_clear : 1; /* [18..18] obff_dec_cpu_active中断清除位，上升沿有效。注1:该位在写1后不会自清0，需要软件显式写0。 */
        unsigned int    obff_dec_err_intr_clear    : 1; /* [19..19] obff_dec_err中断清除位，上升沿有效。注1:该位在写1后不会自清0，需要软件显式写0。 */
        unsigned int    obff_msg_idle_intr_clear   : 1; /* [20..20] obff_msg_idle中断清除位，上升沿有效。注1:该位在写1后不会自清0，需要软件显式写0。 */
        unsigned int    obff_msg_obff_intr_clear   : 1; /* [21..21] obff_msg_obff中断清除位，上升沿有效。注1:该位在写1后不会自清0，需要软件显式写0。 */
        unsigned int    obff_msg_cpu_active_intr_clear : 1; /* [22..22] obff_msg_cpu_active中断清除位，上升沿有效。注1:该位在写1后不会自清0，需要软件显式写0。 */
        unsigned int    radm_msg_ltr_intr_clear    : 1; /* [23..23] radm_msg_ltr中断清除位，上升沿有效。注1:该位在写1后不会自清0，需要软件显式写0。 */
        unsigned int    cfg_msi_mask_chg_intr_clear : 1; /* [24..24] cfg_msi_mask_chg中断清除位，上升沿有效。注1:该位在写1后不会自清0，需要软件显式写0。 */
        unsigned int    radm_qoverflow_intr_clear  : 1; /* [25..25] radm_qoverflow中断清除位，上升沿有效。注1:该位在写1后不会自清0，需要软件显式写0。 */
        unsigned int    reserved_0                 : 6; /* [31..26] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL116_T;    /* PCIE0系统控制寄存器15寄存器 */

typedef union
{
    struct
    {
        unsigned int    cfg_msi_pending            : 32; /* [31..0] 软件配置的MSI能力结构Vector Interrupt Pending寄存器的值。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL117_T;    /* PCIE0系统控制寄存器16寄存器 */

typedef union
{
    struct
    {
        unsigned int    app_ltr_msg_latency        : 32; /* [31..0] 应用请求发送的LTR消息。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL118_T;    /* PCIE0系统控制寄存器17寄存器 */

typedef union
{
    struct
    {
        unsigned int    cfg_pwr_budget_data_reg    : 32; /* [31..0] 功耗预算相关寄存器。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL119_T;    /* PCIE0系统控制寄存器18寄存器 */

typedef union
{
    struct
    {
        unsigned int    app_ltr_msg_func_num       : 3; /* [2..0] LTR消息功能号。 */
        unsigned int    reserved_1                 : 1; /* [3..3] 保留。 */
        unsigned int    cfg_pwr_budget_func_num    : 3; /* [6..4] 功耗预算相关寄存器字段。 */
        unsigned int    slv_awmisc_info_atu_bypass : 1; /* [7..7] AXI slave写请求内部ATU旁路。0：不旁路；1：旁路。 */
        unsigned int    slv_armisc_info_atu_bypass : 1; /* [8..8] AXI slave读请求内部ATU旁路。0：不旁路；1：旁路。 */
        unsigned int    app_clk_pm_en              : 1; /* [9..9] Clock PM使能。0：不使能；1：使能。 */
        unsigned int    reserved_0                 : 22; /* [31..10] 保留。 */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL120_T;    /* PCIE0系统控制寄存器19寄存器 */


/********************************************************************************/
/*    sysreg_pd 函数（项目名_模块名_寄存器名_成员名_set)        */
/********************************************************************************/
HI_SET_GET(hi_sc_ctrl58_pcie_slv_awmisc_info,pcie_slv_awmisc_info,HI_SC_CTRL58_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL58_OFFSET)
HI_SET_GET(hi_sc_ctrl58_pcie_slv_resp_err_map,pcie_slv_resp_err_map,HI_SC_CTRL58_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL58_OFFSET)
HI_SET_GET(hi_sc_ctrl58_pcie_device_type,pcie_device_type,HI_SC_CTRL58_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL58_OFFSET)
HI_SET_GET(hi_sc_ctrl59_pcie_slv_armisc_info,pcie_slv_armisc_info,HI_SC_CTRL59_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL59_OFFSET)
HI_SET_GET(hi_sc_ctrl59_reserved,reserved,HI_SC_CTRL59_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL59_OFFSET)
HI_SET_GET(hi_sc_ctrl59_pcie_app_clk_req_n,pcie_app_clk_req_n,HI_SC_CTRL59_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL59_OFFSET)
HI_SET_GET(hi_sc_ctrl59_pcie_ven_msg_code,pcie_ven_msg_code,HI_SC_CTRL59_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL59_OFFSET)
HI_SET_GET(hi_sc_ctrl60_pcie_mstr_bmisc_info,pcie_mstr_bmisc_info,HI_SC_CTRL60_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL60_OFFSET)
HI_SET_GET(hi_sc_ctrl60_pcie_mstr_rmisc_info,pcie_mstr_rmisc_info,HI_SC_CTRL60_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL60_OFFSET)
HI_SET_GET(hi_sc_ctrl60_pcie_ven_msi_req,pcie_ven_msi_req,HI_SC_CTRL60_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL60_OFFSET)
HI_SET_GET(hi_sc_ctrl60_pcie_ven_msi_vector,pcie_ven_msi_vector,HI_SC_CTRL60_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL60_OFFSET)
HI_SET_GET(hi_sc_ctrl61_pcie_ven_msg_req,pcie_ven_msg_req,HI_SC_CTRL61_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL61_OFFSET)
HI_SET_GET(hi_sc_ctrl61_pcie_ven_msg_fmt,pcie_ven_msg_fmt,HI_SC_CTRL61_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL61_OFFSET)
HI_SET_GET(hi_sc_ctrl61_pcie_ven_msg_type,pcie_ven_msg_type,HI_SC_CTRL61_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL61_OFFSET)
HI_SET_GET(hi_sc_ctrl61_pcie_ven_msg_td,pcie_ven_msg_td,HI_SC_CTRL61_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL61_OFFSET)
HI_SET_GET(hi_sc_ctrl61_pcie_ven_msg_ep,pcie_ven_msg_ep,HI_SC_CTRL61_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL61_OFFSET)
HI_SET_GET(hi_sc_ctrl61_pcie_ven_msg_attr,pcie_ven_msg_attr,HI_SC_CTRL61_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL61_OFFSET)
HI_SET_GET(hi_sc_ctrl61_pcie_ven_msg_len,pcie_ven_msg_len,HI_SC_CTRL61_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL61_OFFSET)
HI_SET_GET(hi_sc_ctrl61_pcie_ven_msg_tag,pcie_ven_msg_tag,HI_SC_CTRL61_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL61_OFFSET)
HI_SET_GET(hi_sc_ctrl61_reserved,reserved,HI_SC_CTRL61_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL61_OFFSET)
HI_SET_GET(hi_sc_ctrl62_pice_ven_msg_data_low,pice_ven_msg_data_low,HI_SC_CTRL62_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL62_OFFSET)
HI_SET_GET(hi_sc_ctrl63_pice_ven_msg_data_high,pice_ven_msg_data_high,HI_SC_CTRL63_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL63_OFFSET)
HI_SET_GET(hi_sc_ctrl64_slot_led_blink_period,slot_led_blink_period,HI_SC_CTRL64_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL64_OFFSET)
HI_SET_GET(hi_sc_ctrl65_pcie_app_init_rst,pcie_app_init_rst,HI_SC_CTRL65_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL65_OFFSET)
HI_SET_GET(hi_sc_ctrl65_pcie_app_req_entr_l1,pcie_app_req_entr_l1,HI_SC_CTRL65_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL65_OFFSET)
HI_SET_GET(hi_sc_ctrl65_pcie_app_ready_entr_l23,pcie_app_ready_entr_l23,HI_SC_CTRL65_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL65_OFFSET)
HI_SET_GET(hi_sc_ctrl65_pcie_app_req_exit_l1,pcie_app_req_exit_l1,HI_SC_CTRL65_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL65_OFFSET)
HI_SET_GET(hi_sc_ctrl65_pcie_app_req_retry_en,pcie_app_req_retry_en,HI_SC_CTRL65_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL65_OFFSET)
HI_SET_GET(hi_sc_ctrl65_pcie_sys_int,pcie_sys_int,HI_SC_CTRL65_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL65_OFFSET)
HI_SET_GET(hi_sc_ctrl65_pcie_outband_pwrup_cmd,pcie_outband_pwrup_cmd,HI_SC_CTRL65_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL65_OFFSET)
HI_SET_GET(hi_sc_ctrl65_pcie_app_unlock_msg,pcie_app_unlock_msg,HI_SC_CTRL65_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL65_OFFSET)
HI_SET_GET(hi_sc_ctrl65_pcie_apps_pm_xmt_turnoff,pcie_apps_pm_xmt_turnoff,HI_SC_CTRL65_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL65_OFFSET)
HI_SET_GET(hi_sc_ctrl65_pcie_apps_pm_xmt_pme,pcie_apps_pm_xmt_pme,HI_SC_CTRL65_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL65_OFFSET)
HI_SET_GET(hi_sc_ctrl65_pcie_sys_aux_pwr_det,pcie_sys_aux_pwr_det,HI_SC_CTRL65_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL65_OFFSET)
HI_SET_GET(hi_sc_ctrl65_pcie_app_ltssm_enable,pcie_app_ltssm_enable,HI_SC_CTRL65_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL65_OFFSET)
HI_SET_GET(hi_sc_ctrl65_pcie_cfg_pwr_ctrler_ctrl_pol,pcie_cfg_pwr_ctrler_ctrl_pol,HI_SC_CTRL65_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL65_OFFSET)
HI_SET_GET(hi_sc_ctrl65_reserved_2,reserved_2,HI_SC_CTRL65_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL65_OFFSET)
HI_SET_GET(hi_sc_ctrl65_pcie_sys_mrl_sensor_state,pcie_sys_mrl_sensor_state,HI_SC_CTRL65_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL65_OFFSET)
HI_SET_GET(hi_sc_ctrl65_pcie_sys_pwr_fault_det,pcie_sys_pwr_fault_det,HI_SC_CTRL65_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL65_OFFSET)
HI_SET_GET(hi_sc_ctrl65_pcie_sys_mrl_sensor_chged,pcie_sys_mrl_sensor_chged,HI_SC_CTRL65_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL65_OFFSET)
HI_SET_GET(hi_sc_ctrl65_reserved_1,reserved_1,HI_SC_CTRL65_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL65_OFFSET)
HI_SET_GET(hi_sc_ctrl65_pcie_sys_cmd_cpled_int,pcie_sys_cmd_cpled_int,HI_SC_CTRL65_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL65_OFFSET)
HI_SET_GET(hi_sc_ctrl65_pcie_sys_eml_interlock_engaged,pcie_sys_eml_interlock_engaged,HI_SC_CTRL65_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL65_OFFSET)
HI_SET_GET(hi_sc_ctrl65_reserved_0,reserved_0,HI_SC_CTRL65_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL65_OFFSET)
HI_SET_GET(hi_sc_ctrl65_pcie_int_ctrl,pcie_int_ctrl,HI_SC_CTRL65_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL65_OFFSET)
HI_SET_GET(hi_sc_ctrl65_pcie_linkdown_auto_rstn_disable,pcie_linkdown_auto_rstn_disable,HI_SC_CTRL65_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL65_OFFSET)
HI_SET_GET(hi_sc_ctrl65_pcie_err_bresp_enable,pcie_err_bresp_enable,HI_SC_CTRL65_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL65_OFFSET)
HI_SET_GET(hi_sc_ctrl65_cipe_err_rresp_enable,cipe_err_rresp_enable,HI_SC_CTRL65_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL65_OFFSET)
HI_SET_GET(hi_sc_ctrl66_reserved_1,reserved_1,HI_SC_CTRL66_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL66_OFFSET)
HI_SET_GET(hi_sc_ctrl66_cfg_l1_aux_clk_switch_core_clk_gate_en,cfg_l1_aux_clk_switch_core_clk_gate_en,HI_SC_CTRL66_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL66_OFFSET)
HI_SET_GET(hi_sc_ctrl66_cfg_l1_mac_powerdown_override_to_p2_en,cfg_l1_mac_powerdown_override_to_p2_en,HI_SC_CTRL66_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL66_OFFSET)
HI_SET_GET(hi_sc_ctrl66_pcie_state2_sel,pcie_state2_sel,HI_SC_CTRL66_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL66_OFFSET)
HI_SET_GET(hi_sc_ctrl66_pcie_state3_sel,pcie_state3_sel,HI_SC_CTRL66_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL66_OFFSET)
HI_SET_GET(hi_sc_ctrl66_reserved_0,reserved_0,HI_SC_CTRL66_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL66_OFFSET)
HI_SET_GET(hi_sc_ctrl67_reserved,reserved,HI_SC_CTRL67_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL67_OFFSET)
HI_SET_GET(hi_sc_ctrl73_reserved,reserved,HI_SC_CTRL73_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL73_OFFSET)
HI_SET_GET(hi_sc_ctrl74_reserved,reserved,HI_SC_CTRL74_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL74_OFFSET)
HI_SET_GET(hi_sc_ctrl75_reserved,reserved,HI_SC_CTRL75_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL75_OFFSET)
HI_SET_GET(hi_sc_ctrl76_reserved,reserved,HI_SC_CTRL76_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL76_OFFSET)
HI_SET_GET(hi_sc_ctrl77_reserved,reserved,HI_SC_CTRL77_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL77_OFFSET)
HI_SET_GET(hi_sc_ctrl78_reserved,reserved,HI_SC_CTRL78_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL78_OFFSET)
HI_SET_GET(hi_sc_ctrl79_reserved,reserved,HI_SC_CTRL79_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL79_OFFSET)
HI_SET_GET(hi_sc_ctrl80_reserved,reserved,HI_SC_CTRL80_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL80_OFFSET)
HI_SET_GET(hi_sc_ctrl82_appa9_gpio1_en,appa9_gpio1_en,HI_SC_CTRL82_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL82_OFFSET)
HI_SET_GET(hi_sc_ctrl83_appa9_gpio2_en,appa9_gpio2_en,HI_SC_CTRL83_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL83_OFFSET)
HI_SET_GET(hi_sc_ctrl84_appa9_gpio3_en,appa9_gpio3_en,HI_SC_CTRL84_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL84_OFFSET)
HI_SET_GET(hi_sc_ctrl86_mdma9_gpio1_en,mdma9_gpio1_en,HI_SC_CTRL86_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL86_OFFSET)
HI_SET_GET(hi_sc_ctrl87_mdma9_gpio2_en,mdma9_gpio2_en,HI_SC_CTRL87_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL87_OFFSET)
HI_SET_GET(hi_sc_ctrl88_mdma9_gpio3_en,mdma9_gpio3_en,HI_SC_CTRL88_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL88_OFFSET)
HI_SET_GET(hi_sc_ctrl89_hifi_gpio0_en,hifi_gpio0_en,HI_SC_CTRL89_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL89_OFFSET)
HI_SET_GET(hi_sc_ctrl90_hifi_gpio1_en,hifi_gpio1_en,HI_SC_CTRL90_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL90_OFFSET)
HI_SET_GET(hi_sc_ctrl91_hifi_gpio2_en,hifi_gpio2_en,HI_SC_CTRL91_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL91_OFFSET)
HI_SET_GET(hi_sc_ctrl92_hifi_gpio3_en,hifi_gpio3_en,HI_SC_CTRL92_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL92_OFFSET)
HI_SET_GET(hi_sc_ctrl93_dsp0_gpio0_en,dsp0_gpio0_en,HI_SC_CTRL93_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL93_OFFSET)
HI_SET_GET(hi_sc_ctrl94_dsp0_gpio1_en,dsp0_gpio1_en,HI_SC_CTRL94_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL94_OFFSET)
HI_SET_GET(hi_sc_ctrl95_dsp0_gpio2_en,dsp0_gpio2_en,HI_SC_CTRL95_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL95_OFFSET)
HI_SET_GET(hi_sc_ctrl96_dsp0_gpio3_en,dsp0_gpio3_en,HI_SC_CTRL96_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL96_OFFSET)
HI_SET_GET(hi_sc_ctrl97_reserved,reserved,HI_SC_CTRL97_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL97_OFFSET)
HI_SET_GET(hi_sc_ctrl99_cm3_gpio1_en,cm3_gpio1_en,HI_SC_CTRL99_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL99_OFFSET)
HI_SET_GET(hi_sc_ctrl100_cm3_gpio2_en,cm3_gpio2_en,HI_SC_CTRL100_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL100_OFFSET)
HI_SET_GET(hi_sc_ctrl101_cm3_gpio3_en,cm3_gpio3_en,HI_SC_CTRL101_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL101_OFFSET)
HI_SET_GET(hi_sc_ctrl102_reserved,reserved,HI_SC_CTRL102_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL102_OFFSET)
HI_SET_GET(hi_sc_ctrl106_cfg_aer_rc_err_msi_mask,cfg_aer_rc_err_msi_mask,HI_SC_CTRL106_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL106_OFFSET)
HI_SET_GET(hi_sc_ctrl106_cfg_sys_err_rc_mask,cfg_sys_err_rc_mask,HI_SC_CTRL106_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL106_OFFSET)
HI_SET_GET(hi_sc_ctrl106_radm_correctable_err_mask,radm_correctable_err_mask,HI_SC_CTRL106_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL106_OFFSET)
HI_SET_GET(hi_sc_ctrl106_radm_nonfatal_err_mask,radm_nonfatal_err_mask,HI_SC_CTRL106_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL106_OFFSET)
HI_SET_GET(hi_sc_ctrl106_radm_fatal_err_mask,radm_fatal_err_mask,HI_SC_CTRL106_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL106_OFFSET)
HI_SET_GET(hi_sc_ctrl106_radm_pm_pme_mask,radm_pm_pme_mask,HI_SC_CTRL106_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL106_OFFSET)
HI_SET_GET(hi_sc_ctrl106_radm_pm_to_ack_mask,radm_pm_to_ack_mask,HI_SC_CTRL106_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL106_OFFSET)
HI_SET_GET(hi_sc_ctrl106_ven_msg_int_mask,ven_msg_int_mask,HI_SC_CTRL106_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL106_OFFSET)
HI_SET_GET(hi_sc_ctrl106_radm_cpl_timeout_mask,radm_cpl_timeout_mask,HI_SC_CTRL106_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL106_OFFSET)
HI_SET_GET(hi_sc_ctrl106_radm_msg_unlock_mask,radm_msg_unlock_mask,HI_SC_CTRL106_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL106_OFFSET)
HI_SET_GET(hi_sc_ctrl106_cfg_pme_msi_mask,cfg_pme_msi_mask,HI_SC_CTRL106_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL106_OFFSET)
HI_SET_GET(hi_sc_ctrl106_bridge_flush_not_mask,bridge_flush_not_mask,HI_SC_CTRL106_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL106_OFFSET)
HI_SET_GET(hi_sc_ctrl106_link_req_rst_not_mask,link_req_rst_not_mask,HI_SC_CTRL106_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL106_OFFSET)
HI_SET_GET(hi_sc_ctrl106_pcie_p2_exit_reg_mask,pcie_p2_exit_reg_mask,HI_SC_CTRL106_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL106_OFFSET)
HI_SET_GET(hi_sc_ctrl106_reserved_1,reserved_1,HI_SC_CTRL106_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL106_OFFSET)
HI_SET_GET(hi_sc_ctrl106_pcie_radm_pm_turnoff_mask,pcie_radm_pm_turnoff_mask,HI_SC_CTRL106_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL106_OFFSET)
HI_SET_GET(hi_sc_ctrl106_reserved_0,reserved_0,HI_SC_CTRL106_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL106_OFFSET)
HI_SET_GET(hi_sc_ctrl107_pcie_cfg_aer_rc_msi_clr,pcie_cfg_aer_rc_msi_clr,HI_SC_CTRL107_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL107_OFFSET)
HI_SET_GET(hi_sc_ctrl107_pcie_cfg_sys_err_rc_clr,pcie_cfg_sys_err_rc_clr,HI_SC_CTRL107_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL107_OFFSET)
HI_SET_GET(hi_sc_ctrl107_pcie_radm_corretable_err_clr,pcie_radm_corretable_err_clr,HI_SC_CTRL107_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL107_OFFSET)
HI_SET_GET(hi_sc_ctrl107_pcie_radm_nonfatal_err_clr,pcie_radm_nonfatal_err_clr,HI_SC_CTRL107_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL107_OFFSET)
HI_SET_GET(hi_sc_ctrl107_pcie_radm_fatal_err_clr,pcie_radm_fatal_err_clr,HI_SC_CTRL107_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL107_OFFSET)
HI_SET_GET(hi_sc_ctrl107_pcie_radm_pm_pme_clr,pcie_radm_pm_pme_clr,HI_SC_CTRL107_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL107_OFFSET)
HI_SET_GET(hi_sc_ctrl107_pcie_radm_pm_to_ack_clr,pcie_radm_pm_to_ack_clr,HI_SC_CTRL107_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL107_OFFSET)
HI_SET_GET(hi_sc_ctrl107_pcie_ven_msg_int_clr,pcie_ven_msg_int_clr,HI_SC_CTRL107_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL107_OFFSET)
HI_SET_GET(hi_sc_ctrl107_pcie_radm_cpl_timeout_clr,pcie_radm_cpl_timeout_clr,HI_SC_CTRL107_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL107_OFFSET)
HI_SET_GET(hi_sc_ctrl107_pcie_radm_msg_unlock_clr,pcie_radm_msg_unlock_clr,HI_SC_CTRL107_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL107_OFFSET)
HI_SET_GET(hi_sc_ctrl107_pcie_cfg_pme_msi_clr,pcie_cfg_pme_msi_clr,HI_SC_CTRL107_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL107_OFFSET)
HI_SET_GET(hi_sc_ctrl107_pcie_bridge_flush_not_clr,pcie_bridge_flush_not_clr,HI_SC_CTRL107_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL107_OFFSET)
HI_SET_GET(hi_sc_ctrl107_pcie_link_req_rst_not_clr,pcie_link_req_rst_not_clr,HI_SC_CTRL107_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL107_OFFSET)
HI_SET_GET(hi_sc_ctrl107_pcie_p2_exit_int_clr,pcie_p2_exit_int_clr,HI_SC_CTRL107_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL107_OFFSET)
HI_SET_GET(hi_sc_ctrl107_pcie_slv_err_int_clr,pcie_slv_err_int_clr,HI_SC_CTRL107_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL107_OFFSET)
HI_SET_GET(hi_sc_ctrl107_pcie_ven_msi_func_num,pcie_ven_msi_func_num,HI_SC_CTRL107_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL107_OFFSET)
HI_SET_GET(hi_sc_ctrl107_pcie_ven_msg_func_num,pcie_ven_msg_func_num,HI_SC_CTRL107_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL107_OFFSET)
HI_SET_GET(hi_sc_ctrl107_pcie_radm_pm_turnoff_clr,pcie_radm_pm_turnoff_clr,HI_SC_CTRL107_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL107_OFFSET)
HI_SET_GET(hi_sc_ctrl107_reserved,reserved,HI_SC_CTRL107_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL107_OFFSET)
HI_SET_GET(hi_sc_ctrl108_pcie0_intx_req,pcie0_intx_req,HI_SC_CTRL108_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL108_OFFSET)
HI_SET_GET(hi_sc_ctrl108_pcie0_msi_req,pcie0_msi_req,HI_SC_CTRL108_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL108_OFFSET)
HI_SET_GET(hi_sc_ctrl108_reserved_5,reserved_5,HI_SC_CTRL108_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL108_OFFSET)
HI_SET_GET(hi_sc_ctrl108_pcie0_pad_phy_mask,pcie0_pad_phy_mask,HI_SC_CTRL108_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL108_OFFSET)
HI_SET_GET(hi_sc_ctrl108_pcie0_pad_out_mask,pcie0_pad_out_mask,HI_SC_CTRL108_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL108_OFFSET)
HI_SET_GET(hi_sc_ctrl108_reserved_4,reserved_4,HI_SC_CTRL108_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL108_OFFSET)
HI_SET_GET(hi_sc_ctrl108_pcie0_inner_phy_mask,pcie0_inner_phy_mask,HI_SC_CTRL108_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL108_OFFSET)
HI_SET_GET(hi_sc_ctrl108_pcie0_inner_out_mask,pcie0_inner_out_mask,HI_SC_CTRL108_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL108_OFFSET)
HI_SET_GET(hi_sc_ctrl108_reserved_3,reserved_3,HI_SC_CTRL108_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL108_OFFSET)
HI_SET_GET(hi_sc_ctrl108_pcie0_out_gatedclock_en,pcie0_out_gatedclock_en,HI_SC_CTRL108_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL108_OFFSET)
HI_SET_GET(hi_sc_ctrl108_pcie0_phy_gatedclock_en,pcie0_phy_gatedclock_en,HI_SC_CTRL108_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL108_OFFSET)
HI_SET_GET(hi_sc_ctrl108_reserved_2,reserved_2,HI_SC_CTRL108_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL108_OFFSET)
HI_SET_GET(hi_sc_ctrl108_pcie0_refclk_oen_sel,pcie0_refclk_oen_sel,HI_SC_CTRL108_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL108_OFFSET)
HI_SET_GET(hi_sc_ctrl108_pcie0_refclk_oen_cfg,pcie0_refclk_oen_cfg,HI_SC_CTRL108_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL108_OFFSET)
HI_SET_GET(hi_sc_ctrl108_gmac_refclk_sel,gmac_refclk_sel,HI_SC_CTRL108_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL108_OFFSET)
HI_SET_GET(hi_sc_ctrl108_reserved_1,reserved_1,HI_SC_CTRL108_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL108_OFFSET)
HI_SET_GET(hi_sc_ctrl108_reserved_0,reserved_0,HI_SC_CTRL108_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL108_OFFSET)
HI_SET_GET(hi_sc_ctrl109_pipe_tx2rx_loopbk,pipe_tx2rx_loopbk,HI_SC_CTRL109_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL109_OFFSET)
HI_SET_GET(hi_sc_ctrl109_pipe_pclkreq_n,pipe_pclkreq_n,HI_SC_CTRL109_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL109_OFFSET)
HI_SET_GET(hi_sc_ctrl109_pipe_pclkreq_n_sel,pipe_pclkreq_n_sel,HI_SC_CTRL109_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL109_OFFSET)
HI_SET_GET(hi_sc_ctrl109_pcs_clk_req,pcs_clk_req,HI_SC_CTRL109_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL109_OFFSET)
HI_SET_GET(hi_sc_ctrl109_pcs_common_clocks,pcs_common_clocks,HI_SC_CTRL109_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL109_OFFSET)
HI_SET_GET(hi_sc_ctrl109_pipe_port_sel,pipe_port_sel,HI_SC_CTRL109_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL109_OFFSET)
HI_SET_GET(hi_sc_ctrl109_reserved_2,reserved_2,HI_SC_CTRL109_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL109_OFFSET)
HI_SET_GET(hi_sc_ctrl109_pipe_rx_elecidle_disable,pipe_rx_elecidle_disable,HI_SC_CTRL109_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL109_OFFSET)
HI_SET_GET(hi_sc_ctrl109_pipe_tx_common_mode_disable,pipe_tx_common_mode_disable,HI_SC_CTRL109_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL109_OFFSET)
HI_SET_GET(hi_sc_ctrl109_pipe_clkreq_n_cfg,pipe_clkreq_n_cfg,HI_SC_CTRL109_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL109_OFFSET)
HI_SET_GET(hi_sc_ctrl109_pipe_clkreq_n_sel,pipe_clkreq_n_sel,HI_SC_CTRL109_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL109_OFFSET)
HI_SET_GET(hi_sc_ctrl109_reserved_1,reserved_1,HI_SC_CTRL109_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL109_OFFSET)
HI_SET_GET(hi_sc_ctrl109_phy_res_ack_in_cfg,phy_res_ack_in_cfg,HI_SC_CTRL109_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL109_OFFSET)
HI_SET_GET(hi_sc_ctrl109_phy_res_ack_in_sel,phy_res_ack_in_sel,HI_SC_CTRL109_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL109_OFFSET)
HI_SET_GET(hi_sc_ctrl109_phy_res_req_in,phy_res_req_in,HI_SC_CTRL109_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL109_OFFSET)
HI_SET_GET(hi_sc_ctrl109_phy_rtune_req,phy_rtune_req,HI_SC_CTRL109_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL109_OFFSET)
HI_SET_GET(hi_sc_ctrl109_phy_test_burnin,phy_test_burnin,HI_SC_CTRL109_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL109_OFFSET)
HI_SET_GET(hi_sc_ctrl109_phy_test_bypass,phy_test_bypass,HI_SC_CTRL109_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL109_OFFSET)
HI_SET_GET(hi_sc_ctrl109_phy_test_powerdown,phy_test_powerdown,HI_SC_CTRL109_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL109_OFFSET)
HI_SET_GET(hi_sc_ctrl109_reserved_0,reserved_0,HI_SC_CTRL109_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL109_OFFSET)
HI_SET_GET(hi_sc_ctrl110_phy_mpll_multiplier,phy_mpll_multiplier,HI_SC_CTRL110_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL110_OFFSET)
HI_SET_GET(hi_sc_ctrl110_phy_ref_clkdiv2,phy_ref_clkdiv2,HI_SC_CTRL110_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL110_OFFSET)
HI_SET_GET(hi_sc_ctrl110_phy_ref_use_pad,phy_ref_use_pad,HI_SC_CTRL110_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL110_OFFSET)
HI_SET_GET(hi_sc_ctrl110_ref_alt_clk_gt_en,ref_alt_clk_gt_en,HI_SC_CTRL110_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL110_OFFSET)
HI_SET_GET(hi_sc_ctrl110_reserved_1,reserved_1,HI_SC_CTRL110_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL110_OFFSET)
HI_SET_GET(hi_sc_ctrl110_phy_reset,phy_reset,HI_SC_CTRL110_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL110_OFFSET)
HI_SET_GET(hi_sc_ctrl110_phy_reset_sel,phy_reset_sel,HI_SC_CTRL110_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL110_OFFSET)
HI_SET_GET(hi_sc_ctrl110_pipe_reset_n_cfg,pipe_reset_n_cfg,HI_SC_CTRL110_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL110_OFFSET)
HI_SET_GET(hi_sc_ctrl110_pipe_reset_sel,pipe_reset_sel,HI_SC_CTRL110_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL110_OFFSET)
HI_SET_GET(hi_sc_ctrl110_reserved_0,reserved_0,HI_SC_CTRL110_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL110_OFFSET)
HI_SET_GET(hi_sc_ctrl111_phy_los_bias,phy_los_bias,HI_SC_CTRL111_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL111_OFFSET)
HI_SET_GET(hi_sc_ctrl111_phy_los_level,phy_los_level,HI_SC_CTRL111_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL111_OFFSET)
HI_SET_GET(hi_sc_ctrl111_phy_rx_eq,phy_rx_eq,HI_SC_CTRL111_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL111_OFFSET)
HI_SET_GET(hi_sc_ctrl111_pcs_tx_deemph_gen2_3p5db,pcs_tx_deemph_gen2_3p5db,HI_SC_CTRL111_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL111_OFFSET)
HI_SET_GET(hi_sc_ctrl111_pcs_tx_deemph_gen2_6db,pcs_tx_deemph_gen2_6db,HI_SC_CTRL111_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL111_OFFSET)
HI_SET_GET(hi_sc_ctrl111_reserved_1,reserved_1,HI_SC_CTRL111_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL111_OFFSET)
HI_SET_GET(hi_sc_ctrl111_pcs_tx_deemph_gen1,pcs_tx_deemph_gen1,HI_SC_CTRL111_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL111_OFFSET)
HI_SET_GET(hi_sc_ctrl111_reserved_0,reserved_0,HI_SC_CTRL111_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL111_OFFSET)
HI_SET_GET(hi_sc_ctrl112_pcs_tx_swing_full,pcs_tx_swing_full,HI_SC_CTRL112_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL112_OFFSET)
HI_SET_GET(hi_sc_ctrl112_pcs_tx_swing_low,pcs_tx_swing_low,HI_SC_CTRL112_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL112_OFFSET)
HI_SET_GET(hi_sc_ctrl112_phy_tx_term_offset,phy_tx_term_offset,HI_SC_CTRL112_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL112_OFFSET)
HI_SET_GET(hi_sc_ctrl112_reserved_2,reserved_2,HI_SC_CTRL112_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL112_OFFSET)
HI_SET_GET(hi_sc_ctrl112_phy_tx_vboost_lvl,phy_tx_vboost_lvl,HI_SC_CTRL112_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL112_OFFSET)
HI_SET_GET(hi_sc_ctrl112_reserved_1,reserved_1,HI_SC_CTRL112_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL112_OFFSET)
HI_SET_GET(hi_sc_ctrl112_vreg_bypass,vreg_bypass,HI_SC_CTRL112_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL112_OFFSET)
HI_SET_GET(hi_sc_ctrl112_reserved_0,reserved_0,HI_SC_CTRL112_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL112_OFFSET)
HI_SET_GET(hi_sc_ctrl113_pcie_perst_out_n,pcie_perst_out_n,HI_SC_CTRL113_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL113_OFFSET)
HI_SET_GET(hi_sc_ctrl113_pcie_perst_oe,pcie_perst_oe,HI_SC_CTRL113_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL113_OFFSET)
HI_SET_GET(hi_sc_ctrl113_pcie_perst_in_n_ctrl,pcie_perst_in_n_ctrl,HI_SC_CTRL113_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL113_OFFSET)
HI_SET_GET(hi_sc_ctrl113_mac_phy_pclkreq_n_0_ctrl,mac_phy_pclkreq_n_0_ctrl,HI_SC_CTRL113_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL113_OFFSET)
HI_SET_GET(hi_sc_ctrl113_mac_phy_pclkreq_n_1_ctrl,mac_phy_pclkreq_n_1_ctrl,HI_SC_CTRL113_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL113_OFFSET)
HI_SET_GET(hi_sc_ctrl113_cfg_l1sub_en_ctrl,cfg_l1sub_en_ctrl,HI_SC_CTRL113_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL113_OFFSET)
HI_SET_GET(hi_sc_ctrl113_clkreq_n_ctrl0,clkreq_n_ctrl0,HI_SC_CTRL113_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL113_OFFSET)
HI_SET_GET(hi_sc_ctrl113_clkreq_n_ctrl1,clkreq_n_ctrl1,HI_SC_CTRL113_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL113_OFFSET)
HI_SET_GET(hi_sc_ctrl113_rc_wake_n_ctrl,rc_wake_n_ctrl,HI_SC_CTRL113_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL113_OFFSET)
HI_SET_GET(hi_sc_ctrl113_ep_wake_n_ctrl,ep_wake_n_ctrl,HI_SC_CTRL113_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL113_OFFSET)
HI_SET_GET(hi_sc_ctrl113_obff_enc_owre_wake_ctrl,obff_enc_owre_wake_ctrl,HI_SC_CTRL113_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL113_OFFSET)
HI_SET_GET(hi_sc_ctrl113_dwc_pcie_dm_wake_n_ctrl,dwc_pcie_dm_wake_n_ctrl,HI_SC_CTRL113_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL113_OFFSET)
HI_SET_GET(hi_sc_ctrl113_reserved,reserved,HI_SC_CTRL113_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL113_OFFSET)
HI_SET_GET(hi_sc_ctrl114_obff_dec_app_init,obff_dec_app_init,HI_SC_CTRL114_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL114_OFFSET)
HI_SET_GET(hi_sc_ctrl114_obff_dec_app_idle,obff_dec_app_idle,HI_SC_CTRL114_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL114_OFFSET)
HI_SET_GET(hi_sc_ctrl114_obff_dec_app_obff,obff_dec_app_obff,HI_SC_CTRL114_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL114_OFFSET)
HI_SET_GET(hi_sc_ctrl114_obff_dec_app_cpu_act,obff_dec_app_cpu_act,HI_SC_CTRL114_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL114_OFFSET)
HI_SET_GET(hi_sc_ctrl114_obff_enc_app_owre_idle,obff_enc_app_owre_idle,HI_SC_CTRL114_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL114_OFFSET)
HI_SET_GET(hi_sc_ctrl114_obff_enc_app_owre_obff,obff_enc_app_owre_obff,HI_SC_CTRL114_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL114_OFFSET)
HI_SET_GET(hi_sc_ctrl114_obff_enc_app_owre_cpu_act,obff_enc_app_owre_cpu_act,HI_SC_CTRL114_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL114_OFFSET)
HI_SET_GET(hi_sc_ctrl114_app_obff_idle_msg_req,app_obff_idle_msg_req,HI_SC_CTRL114_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL114_OFFSET)
HI_SET_GET(hi_sc_ctrl114_app_obff_obff_msg_req,app_obff_obff_msg_req,HI_SC_CTRL114_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL114_OFFSET)
HI_SET_GET(hi_sc_ctrl114_app_obff_cpu_active_msg_req,app_obff_cpu_active_msg_req,HI_SC_CTRL114_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL114_OFFSET)
HI_SET_GET(hi_sc_ctrl114_app_ltr_msg_req,app_ltr_msg_req,HI_SC_CTRL114_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL114_OFFSET)
HI_SET_GET(hi_sc_ctrl114_reserved,reserved,HI_SC_CTRL114_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL114_OFFSET)
HI_SET_GET(hi_sc_ctrl115_obff_cfg_wk_max_pls_wdt,obff_cfg_wk_max_pls_wdt,HI_SC_CTRL115_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL115_OFFSET)
HI_SET_GET(hi_sc_ctrl115_obff_cfg_wk_min_pls_wdt,obff_cfg_wk_min_pls_wdt,HI_SC_CTRL115_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL115_OFFSET)
HI_SET_GET(hi_sc_ctrl115_obff_cfg_wk_max_f2f_wdt,obff_cfg_wk_max_f2f_wdt,HI_SC_CTRL115_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL115_OFFSET)
HI_SET_GET(hi_sc_ctrl115_obff_cfg_wk_min_f2f_wdt,obff_cfg_wk_min_f2f_wdt,HI_SC_CTRL115_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL115_OFFSET)
HI_SET_GET(hi_sc_ctrl116_obff_dec_idle_intr_mask,obff_dec_idle_intr_mask,HI_SC_CTRL116_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL116_OFFSET)
HI_SET_GET(hi_sc_ctrl116_obff_dec_obff_intr_mask,obff_dec_obff_intr_mask,HI_SC_CTRL116_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL116_OFFSET)
HI_SET_GET(hi_sc_ctrl116_obff_dec_cpu_active_intr_mask,obff_dec_cpu_active_intr_mask,HI_SC_CTRL116_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL116_OFFSET)
HI_SET_GET(hi_sc_ctrl116_obff_dec_err_intr_mask,obff_dec_err_intr_mask,HI_SC_CTRL116_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL116_OFFSET)
HI_SET_GET(hi_sc_ctrl116_obff_msg_idle_intr_mask,obff_msg_idle_intr_mask,HI_SC_CTRL116_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL116_OFFSET)
HI_SET_GET(hi_sc_ctrl116_obff_msg_obff_intr_mask,obff_msg_obff_intr_mask,HI_SC_CTRL116_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL116_OFFSET)
HI_SET_GET(hi_sc_ctrl116_obff_msg_cpu_active_intr_mask,obff_msg_cpu_active_intr_mask,HI_SC_CTRL116_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL116_OFFSET)
HI_SET_GET(hi_sc_ctrl116_radm_msg_ltr_intr_mask,radm_msg_ltr_intr_mask,HI_SC_CTRL116_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL116_OFFSET)
HI_SET_GET(hi_sc_ctrl116_cfg_msi_mask_chg_intr_mask,cfg_msi_mask_chg_intr_mask,HI_SC_CTRL116_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL116_OFFSET)
HI_SET_GET(hi_sc_ctrl116_radm_qoverflow_intr_mask,radm_qoverflow_intr_mask,HI_SC_CTRL116_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL116_OFFSET)
HI_SET_GET(hi_sc_ctrl116_amba_order_mgr_wdt_int_intr_mask,amba_order_mgr_wdt_int_intr_mask,HI_SC_CTRL116_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL116_OFFSET)
HI_SET_GET(hi_sc_ctrl116_pcie_clkreq_in_n_intr_mask,pcie_clkreq_in_n_intr_mask,HI_SC_CTRL116_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL116_OFFSET)
HI_SET_GET(hi_sc_ctrl116_pcie_clkreq_in_n_inv_intr_mask,pcie_clkreq_in_n_inv_intr_mask,HI_SC_CTRL116_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL116_OFFSET)
HI_SET_GET(hi_sc_ctrl116_pcie_v721_intr_mask,pcie_v721_intr_mask,HI_SC_CTRL116_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL116_OFFSET)
HI_SET_GET(hi_sc_ctrl116_reserved_1,reserved_1,HI_SC_CTRL116_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL116_OFFSET)
HI_SET_GET(hi_sc_ctrl116_obff_dec_idle_intr_clear,obff_dec_idle_intr_clear,HI_SC_CTRL116_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL116_OFFSET)
HI_SET_GET(hi_sc_ctrl116_obff_dec_obff_intr_clear,obff_dec_obff_intr_clear,HI_SC_CTRL116_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL116_OFFSET)
HI_SET_GET(hi_sc_ctrl116_obff_dec_cpu_active_intr_clear,obff_dec_cpu_active_intr_clear,HI_SC_CTRL116_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL116_OFFSET)
HI_SET_GET(hi_sc_ctrl116_obff_dec_err_intr_clear,obff_dec_err_intr_clear,HI_SC_CTRL116_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL116_OFFSET)
HI_SET_GET(hi_sc_ctrl116_obff_msg_idle_intr_clear,obff_msg_idle_intr_clear,HI_SC_CTRL116_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL116_OFFSET)
HI_SET_GET(hi_sc_ctrl116_obff_msg_obff_intr_clear,obff_msg_obff_intr_clear,HI_SC_CTRL116_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL116_OFFSET)
HI_SET_GET(hi_sc_ctrl116_obff_msg_cpu_active_intr_clear,obff_msg_cpu_active_intr_clear,HI_SC_CTRL116_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL116_OFFSET)
HI_SET_GET(hi_sc_ctrl116_radm_msg_ltr_intr_clear,radm_msg_ltr_intr_clear,HI_SC_CTRL116_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL116_OFFSET)
HI_SET_GET(hi_sc_ctrl116_cfg_msi_mask_chg_intr_clear,cfg_msi_mask_chg_intr_clear,HI_SC_CTRL116_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL116_OFFSET)
HI_SET_GET(hi_sc_ctrl116_radm_qoverflow_intr_clear,radm_qoverflow_intr_clear,HI_SC_CTRL116_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL116_OFFSET)
HI_SET_GET(hi_sc_ctrl116_reserved_0,reserved_0,HI_SC_CTRL116_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL116_OFFSET)
HI_SET_GET(hi_sc_ctrl117_cfg_msi_pending,cfg_msi_pending,HI_SC_CTRL117_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL117_OFFSET)
HI_SET_GET(hi_sc_ctrl118_app_ltr_msg_latency,app_ltr_msg_latency,HI_SC_CTRL118_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL118_OFFSET)
HI_SET_GET(hi_sc_ctrl119_cfg_pwr_budget_data_reg,cfg_pwr_budget_data_reg,HI_SC_CTRL119_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL119_OFFSET)
HI_SET_GET(hi_sc_ctrl120_app_ltr_msg_func_num,app_ltr_msg_func_num,HI_SC_CTRL120_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL120_OFFSET)
HI_SET_GET(hi_sc_ctrl120_reserved_1,reserved_1,HI_SC_CTRL120_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL120_OFFSET)
HI_SET_GET(hi_sc_ctrl120_cfg_pwr_budget_func_num,cfg_pwr_budget_func_num,HI_SC_CTRL120_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL120_OFFSET)
HI_SET_GET(hi_sc_ctrl120_slv_awmisc_info_atu_bypass,slv_awmisc_info_atu_bypass,HI_SC_CTRL120_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL120_OFFSET)
HI_SET_GET(hi_sc_ctrl120_slv_armisc_info_atu_bypass,slv_armisc_info_atu_bypass,HI_SC_CTRL120_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL120_OFFSET)
HI_SET_GET(hi_sc_ctrl120_app_clk_pm_en,app_clk_pm_en,HI_SC_CTRL120_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL120_OFFSET)
HI_SET_GET(hi_sc_ctrl120_reserved_0,reserved_0,HI_SC_CTRL120_T,HI_SYSREG_PD_BASE_ADDR, HI_SC_CTRL120_OFFSET)

#endif

#endif // __HI_SYSREG_PD_H__

