/*************************************************************************
*   版权所有(C) 2008-2013 深圳华为技术有限公司.
*
*   文 件 名 :  soc_interrupts_app.h
*
*
*   描    述 :  v7r2 m3 中断map
*
*************************************************************************/
#ifndef __SOC_INTERRUPTS_M3_H__
#define __SOC_INTERRUPTS_M3_H__

#ifdef __cplusplus
extern "C" {
#endif


#define  M3_IPCM_INT0_2                 16  /* IPCM给M3的核间通信，对应IPCM中断0[2] */
#define  M3_IPCM_INT1_2                 17  /* IPCM给M3的核间通信，对应IPCM中断1[2] */
#define  M3_APPA9_EVENTO_INT            18  /* APP A9 EVENTO信号 */
#define  M3_MDMA9_EVENTO_INT            19  /* MDM A9 EVENTO信号 */
#define  M3_RTC_INT                     20  /* RTC模块输出中断 */
#define  M3_WDT_INT                     21  /* WatchDog模块输出中断 */
#define  M3_TSENSOR_INT                 22  /* Tsensor中断 */
#define  M3_LTE_ARM_CLK_SWITCH_INT      23  /* LTE_BBP_ON到ARM中断 */
#define  M3_LTE_ARM_WAKEUP_INT          24  /* LTE_BBP_ON到ARM中断 */
#define  M3_LTE_ARM_PUB_INT             25  /* LTE_BBP_ON到ARM中断 */
#define  M3_G1_INT_BBP_TO_CPU           26  /* GBBP1_ON到ARM中断 */
#define  M3_G1_INT_BBP_TO_DSP           27  /* GBBP1_ON到ARM中断 */
#define  M3_G1_INT_BBP_TO_CPU_32K       28  /* GBBP1_ON到ARM中断 */
#define  M3_G1_INT_BBP_TO_DSP_32K       29  /* GBBP1_ON到ARM中断 */
#define  M3_G2_INT_BBP_TO_CPU           30  /* GBBP2_ON到ARM中断 */
#define  M3_G2_INT_BBP_TO_DSP           31  /* GBBP2_ON到ARM中断 */
#define  M3_G2_INT_BBP_TO_CPU_32K       32  /* GBBP2_ON到ARM中断 */
#define  M3_G2_INT_BBP_TO_DSP_32K       33  /* GBBP2_ON到ARM中断 */
#define  M3_IPCM_INT0_4                 34  /* IPCM给M3的核间通信，对应IPCM中断0[4] */
#define  M3_TIMER0_INT                  35  /* TIMER0中断 */
#define  M3_TIMER1_INT                  36  /* TIMER1中断 */
#define  M3_TIMER2_INT                  37  /* TIMER2中断 */
#define  M3_TIMER3_INT                  38  /* TIMER3中断 */
#define  M3_TIMER4_INT                  39  /* TIMER4中断 */
#define  M3_TIMER5_INT                  40  /* TIMER5中断 */
#define  M3_TIMER6_INT                  41  /* TIMER6中断 */
#define  M3_TIMER7_INT                  42  /* TIMER7中断 */
#define  M3_TIMER8_INT                  43  /* TIMER8中断 */
#define  M3_TIMER9_INT                  44  /* TIMER9中断 */
#define  M3_TIMER10_INT                 45  /* TIMER10中断 */
#define  M3_TIMER11_INT                 46  /* TIMER11中断 */
#define  M3_TIMER12_INT                 47  /* TIMER12中断 */
#define  M3_TIMER13_INT                 48  /* TIMER13中断 */
#define  M3_TIMER14_INT                 49  /* TIMER14中断 */
#define  M3_TIMER15_INT                 50  /* TIMER15中断 */
#define  M3_SCI0_INT                    51  /* SIM卡0中断 */
#define  M3_SCI1_INT                    52  /* SIM卡1中断 */
#define  M3_DW_SSI0_INT                 53  /* SPI0中断 */
#define  M3_DW_SSI1_INT                 54  /* SPI1中断 */
#define  M3_RESERVE2                    55  /* 保留 */
#define  M3_TCSSI1_INT                  56  /* 单线串行ssi1中断 */
#define  M3_USB3_PME_INT                57  /* USB3_PD_PME_INT | USB3_PMU_PME_INT ,USB3的power management event */
#define  M3_MIPI_INT                    58  /* MIPI中断 */
#define  M3_HS_UART_INT                 59  /* 高速串口0中断 */
#define  M3_UART0_INT                   60  /* 串口0中断 */
#define  M3_UART1_INT                   61  /* 串口1中断 */
#define  M3_UART2_INT                   62  /* 串口2中断 */
#define  M3_UART3_INT                   63  /* 串口3中断 */
#define  M3_NANDC_INT                   64  /* Nandflash控制器中断 */
#define  M3_MMC0_INT                    65  /* MobileStorageController0中断 */
#define  M3_MMC1_INT                    66  /* MobileStorageController1中断 */
#define  M3_RESERVE3                    67  /* 保留 */
#define  M3_SDCC_INT                    68  /* SDIO slave中断 */
#define  M3_I2C0_INT                    69  /* I2C0中断 */
#define  M3_GPIO_GROUP0_INT             70  /* M3 GPIO group 0 复合中断,由系统控制器m3_gpio0_en控制使能GPIO0的32个输出中断 */
#define  M3_GPIO_GROUP1_INT             71  /* M3 GPIO group 1 复合中断,由系统控制器m3_gpio1_en控制使能GPIO1的32个输出中断 */
#define  M3_GPIO_GROUP2_INT             72  /* M3 GPIO group 2 复合中断,由系统控制器m3_gpio2_en控制使能GPIO2的32个输出中断 */
#define  M3_GPIO_GROUP3_INT             73  /* M3 GPIO group 3 复合中断,由系统控制器m3_gpio3_en控制使能GPIO3的32个输出中断 */
#define  M3_I2C1_INT                    74  /* I2C1中断 */
#define  M3_IPCM_APP_INT0               75  /* IPC APP中断 0*/
#define  M3_IPCM_APP_INT1               76  /* IPC APP中断 1*/
#define  M3_EDMAC_INT0                  77  /* EDMAC中断0 */
#define  M3_EDMAC_INT1                  78  /* EDMAC中断1 */
#define  M3_AMON_INT                    79  /* AMON_FAST_INT|AMON_SOC_INT */
#define  M3_HSIC_INT                    80  /* HSIC中断 */
#define  M3_APPA9_PMUIREQ_INT           81  /* APPA9 PMU 中断 */
#define  M3_MDMA9_PMUIREQ_INT           82  /* MDMA9 PMU 中断 */
#define  M3_HiFi_GPIO2_INT              83  /* HIFI GPIO[2] */
#define  M3_DSP0_GPIO2_INT              84  /* DSP0 GPIO[2] */
#define  M3_PMU_INT                     85  /* PMU中断，极性可由系统控制器配置取反 */
#define  M3_USB3_OTG_INT                86  /* USB3 OTG 中断 */
#define  M3_X2H_X2P_ERR                 87  /* dw_axi_x2h和dw_axi_x2p外部地址译码错误中断 */
#define  M3_AXI_ERR                     88  /* dw_axi_cpu/glb/bbphy/dsp桥的地址译码错误中断 */
#define  M3_USBOTG_BC_INT               89  /* bc_ctrl中断 */
#define  M3_SYS_CONTROL_INT4            90  /* 软中断4，系统控制器配置 */
#define  M3_SYS_CONTROL_INT5            91  /* 软中断5，系统控制器配置 */
#define  M3_TIMER16_INT                 92  /* TIMER16中断 */
#define  M3_TIMER17_INT                 93  /* TIMER17中断 */
#define  M3_TIMER18_INT                 94  /* TIMER18中断 */
#define  M3_TIMER19_INT                 95  /* TIMER19中断 */
#define  M3_TIMER20_INT                 96  /* TIMER20中断 */
#define  M3_TIMER21_INT                 97  /* TIMER21中断 */
#define  M3_TIMER22_INT                 98  /* TIMER22中断 */
#define  M3_TIMER23_INT                 99  /* TIMER23中断 */
#define  M3_RESERVE6                    100 /* 保留 */
#define  M3_CPU_AXI_DLOCK_INT           101 /* dw_axi_cpu互联死锁中断 */
#define  M3_GLb_AXI_DLOCK_INT           102 /* dw_axi_glb互联死锁中断 */
#define  M3_MST_AXI_DLOCK_INT           103 /* dw_axi_mst互联死锁中断 */
#define  M3_AMON_AXI_DLOCK_INT          104 /* dw_axi_amon互联死锁中断 */
#define  M3_BBPHY_AXI_DLOCK_INT         105 /* dw_axi_bbphy互联死锁中断 */
#define  M3_SDCC_HCLK_WKUP_INT          106 /* SDIO slave唤醒中断,上升沿触发，不需要清中断源，IP自动拉高和拉低； */
#define  M3_TDS_STU_ARM_INT             107 /* TDS_BBP_OFF到ARM中断 */
#define  M3_RESERVE7                    108 /* 保留 */
#define  M3_TDS_DRX_ARM_CLKSWITCH_INT   109 /* TDS_BBP_ON到ARM中断 */
#define  M3_TDS_DRX_ARM_WAKEUP_INT      110 /* TDS_BBP_ON到ARM中断 */
#define  M3_WAKEUP_INT                  111 /* 组合唤醒中断，中断类型、触发类型和是否需要清除中断源需要看源的中断类型。具体参考《M3唤醒中断源》表单描述 */
#define  M3_USBOTG_PHY_INT              112 /* 给M3的USBOTG_PHY_INT */
#define  M3_APPA9_L2CC_INT              113 /* APPA9 L2组合中断 */
#define  M3_IPCM_MDM_INT0               114 /* IPC MDM中断 0*/
#define  M3_SDCC_PULL_INT               115 /* sdcc data3上拉中断，中断源和中断状态在系统控制器处理 */
#define  M3_USB3_INT                    116 /* USB3中断 */
#define  M3_IPCM_MDM_INT1               117 /* IPC MDM中断 1*/
#define  M3_APPA9_ARM_WD_SRST_REQ       118 /* APP A9软复位请求 */
#define  M3_MODA9_ARM_WD_SRST_REQ       119 /* Modem A9软复位请求 */
#define  M3_W_ARM_INT_02                120 /* WBBP_ON到ARM中断 */
#define  M3_W_ARM_INT_03                121 /* WBBP_ON到ARM中断 */
#define  M3_OTDOA_INT                   122 /* WBBP_OFF到ARM中断 */
#define  M3_CTU_INT_W                   123 /* CTU BBP_OFF到ARM中断 */
#define  M3_CTU_INT_G1                  124 /* CTU BBP_OFF到ARM中断 */
#define  M3_CTU_INT_G2                  125 /* CTU BBP_OFF到ARM中断 */
#define  M3_CTU_INT_LTE                 126 /* CTU BBP_OFF到BBE16中断 */
#define  M3_CTU_INT_TDS                 127 /* CTU BBP_OFF到BBE16中断 */
#define  M3_PCIE0_LINK_DOWN_INT         128 /* PCIE 控制器 LINK DOWN 中断 */
#define  M3_PCIE0_EDMA_INT              129 /* PCIE 控制器 EDMA 中断 */
#define  M3_PCIE0_PM_INT                130 /* PCIE 控制器 PM 中断 */
#define  M3_PCIE0_RADM_INTA             131 /* PCIE 控制器 RADM 中断 A */
#define  M3_PCIE0_RADM_INTB             132 /* PCIE 控制器 RADM 中断 B */
#define  M3_PCIE0_RADM_INTC             133 /* PCIE 控制器 RADM 中断 C */
#define  M3_PCIE0_RADM_INTD             134 /* PCIE 控制器 RADM 中断 D */
#define  M3_RESERVE9                    135 /* 保留 */
#define  M3_RESERVE10                   136 /* 保留 */
#define  M3_EDMAC_CH4_INT0              137 /* 4通道EDMAC中断0 */
#define  M3_EDMAC_CH4_INT1              138 /* 4通道EDMAC中断1 */
#define  M3_EDMAC_CH4_INT2              139 /* 4通道EDMAC中断2 */
#define  M3_IPF_INT0                    140 /* 保留 */
#define  M3_IPF_INT1                    141 /* 保留 */
#define  M3_SOCP_INT0                   142 /* SOCP Core0中断*/
#define  M3_SOCP_INT1                   143 /* SOCP Core1中断 */

/* for setting wake_src in acore/ccore */
#define M3_DRX_TIMER_INT M3_TIMER23_INT

#ifdef __cplusplus
}
#endif

#endif /* __SOC_INTERRUPTS_M3_H__ */

