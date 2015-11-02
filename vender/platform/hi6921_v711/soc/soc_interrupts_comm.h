/*
This file contains I/O address and related constants for the balongv7r2 interrupts level, comm.
*/

#ifndef __SOC_INTERRUPTS_COMM_H
#define __SOC_INTERRUPTS_COMM_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************
                  * interrupts level comm*
***************************************************************************/

#define GIC_PPI_START                         27
#define GIC_SPI_START                         32
#define SGI_INT_MAX                           16
#define INT_LVL_MAX                          224

#define INT_LVL_RTC                           36
#define INT_LVL_WDT                           37
#define INT_LVL_UPACC_DSP                     38
#define INT_LVL_UICC                          39
#define INT_LVL_UPACC_BBP_I1_E                40
#define INT_LVL_UPACC_BBP_I2                  41
#define INT_LVL_EDMAC2                        42
#define INT_LVL_CIPHER                        43
#define INT_LVL_IPF0                          44
#define INT_LVL_IPF1                          45
#define INT_LVL_SOCP0                         46
#define INT_LVL_SOCP1                         47
#define INT_LVL_LTESIO_RX                     48
#define INT_LVL_LTESIO_SIO                    49
#define INT_LVL_LTESIO_TX                     50
#define INT_LVL_CICOM0_UL                     51
#define INT_LVL_CICOM0_DL                     52
#define INT_LVL_CICOM1_UL                     53
#define INT_LVL_CICOM1_DL                     54
#define INT_LVL_UPACC_ARM                     55
#define INT_LVL_HDLC_FRM                      56
#define INT_LVL_HDLC_DEF                      57
#define INT_LVL_CS_ETR_0_FULL                 58
#define INT_LVL_TSENSOR                       59
#define INT_LVL_BBPTIMER                      60
#define INT_LVL_BBPDLTB                       61
#define INT_LVL_LTE_BBP_CLK_SWITCH            62
#define INT_LVL_LTE_BBP_WAKEUP                63
#define INT_LVL_LTE_BBP_PUB                   64
#define INT_LVL_LTE_BBP2APPA9_PUB             65
#define INT_LVL_LTE_BBP_POSITION_PULSE        66
#define INT_LVL_LTE_BBP_DL_DMA_PULSE          67
#define INT_LVL_G1_BBP_TO_CPU                 68
#define INT_LVL_G1_BBP_TO_DSP                 69
#define INT_LVL_G1_BBP_TO_CPU_32K             70
#define INT_LVL_G1_BBP_TO_DSP_32K             71
#define INT_LVL_G2_BBP_TO_CPU                 72
#define INT_LVL_G2_BBP_TO_DSP                 73
#define INT_LVL_G2_BBP_TO_CPU_32K             74
#define INT_LVL_G2_BBP_TO_DSP_32K             75

#define INT_LVL_TIMER0                        77
#define INT_LVL_TIMER1                        78
#define INT_LVL_TIMER2                        79
#define INT_LVL_TIMER3                        80
#define INT_LVL_TIMER4                        81
#define INT_LVL_TIMER5                        82
#define INT_LVL_TIMER6                        83
#define INT_LVL_TIMER7                        84
#define INT_LVL_TIMER8                        85
#define INT_LVL_TIMER9                        86
#define INT_LVL_TIMER10                       87
#define INT_LVL_TIMER11                       88
#define INT_LVL_TIMER12                       89
#define INT_LVL_TIMER13                       90
#define INT_LVL_TIMER14                       91
#define INT_LVL_TIMER15                       92
#define INT_LVL_SCI0                          93
#define INT_LVL_SCI1                          94
#define INT_LVL_SPI0                          95
#define INT_LVL_SPI1                          96

#define INT_LVL_TCSSI1                        98
#define INT_LVL_USB3_PME                      99
#define INT_LVL_MIPI                         100
#define INT_LVL_HSUART                       101
#define INT_LVL_UART0                        102
#define INT_LVL_UART1                        103
#define INT_LVL_UART2                        104
#define INT_LVL_UART3                        105
#define INT_LVL_NANDC                        106
#define INT_LVL_MMC0                         107
#define INT_LVL_MMC1                         108

#define INT_LVL_SDCC                         110
#define INT_LVL_I2C0                         111
#define INT_LVL_I2C1                         116

#define INT_LVL_EDMAC0                       119
#define INT_LVL_EDMAC1                       120
#define INT_LVL_AMON                         121
#define INT_LVL_HSIC                         122
#define INT_LVL_APP_PMU                      123
#define INT_LVL_MDM_PMU                      124

#define INT_LVL_PMU                          127
#define INT_LVL_USB3_OTG                     128
#define INT_LVL_X2H_X2P_ERR                  129
#define INT_LVL_AXI_ERR                      130
#define INT_LVL_USBOTG_BC                    131

#define INT_LVL_TIMER16                      134
#define INT_LVL_TIMER17                      135
#define INT_LVL_TIMER18                      136
#define INT_LVL_TIMER19                      137
#define INT_LVL_TIMER20                      138
#define INT_LVL_TIMER21                      139
#define INT_LVL_TIMER22                      140
#define INT_LVL_TIMER23                      141

#define INT_LVL_CPU_AXI_LOCK                 143
#define INT_LVL_GLB_AXI_LOCK                 144
#define INT_LVL_MST_AXI_LOCK                 145
#define INT_LVL_AMON_AXI_LOCK                146
#define INT_LVL_BBPPHY_AXI_LOCK              147
#define INT_LVL_SDCC_HCLK_WKUP               148
#define INT_LVL_TDSSTU                       149

#define INT_LVL_TDS_DRX_ARM_CLKSWITCH        151
#define INT_LVL_TDS_DRX_WAKEUP               152
#define INT_LVL_APP_L2CC                     155

#define INT_LVL_SDCC_PULL                    157
#define INT_LVL_USB3                         158

#define INT_LVL_APP_SRST_REQ                 160
#define INT_LVL_MDM_SRST_REQ                 161
#define INT_LVL_W_ARM_01                     162
#define INT_LVL_W_ARM_02                     163
#define INT_LVL_W_ARM_03                     164
#define INT_LVL_W_ARM_04                     165
#define INT_LVL_W_ARM_05                     166
#define INT_LVL_W_ARM_06                     167
#define INT_LVL_W_ARM_07                     168
#define INT_LVL_W_ARM_08                     169
#define INT_LVL_W_ARM_09                     170
#define INT_LVL_W_ARM_10                     171
#define INT_LVL_W_ARM_11                     172
#define INT_LVL_OTDOA                        173
#define INT_LVL_CTU_W                        174
#define INT_LVL_CTU_G1                       175
#define INT_LVL_CTU_G2                       176
#define INT_LVL_W_BBE16_13                   177
#define INT_LVL_W_BBE16_14                   178

#define INT_LVL_EDMAC_CH4_0                  182
#define INT_LVL_EDMAC_CH4_1                  183
#define INT_LVL_EDMAC_CH4_2                  184

#define INT_LVL_MERGE0                       186
#define INT_LVL_MERGE1                       187
#define INT_LVL_MERGE2                       188
#define INT_LVL_MERGE3                       189
#define INT_LVL_CTU_LTE                      190
#define INT_LVL_CTU_TDS                      191
#define INT_LVL_PCIE0_LINK_DOWN              192
#define INT_LVL_PCIE0_EDMA                   193
#define INT_LVL_PCIE0_PM                     194
#define INT_LVL_PCIE0_RADM_A                 195
#define INT_LVL_PCIE0_RADM_B                 196
#define INT_LVL_PCIE0_RADM_C                 197
#define INT_LVL_PCIE0_RADM_D                 198
#define INT_LVL_GMAC_SBD                     206
#define INT_LVL_GMAC_PMT                     207

/*GU INT*/
#define	INT_LVL_GBBP_DSP                  INT_LVL_G1_BBP_TO_DSP
#define INT_LVL_GBBP_GSML_NODRX_INTR      INT_LVL_G1_BBP_TO_DSP
#define INT_LVL_GBBP_AWAKE_DSP            INT_LVL_G1_BBP_TO_DSP_32K
#define INT_LVL_GBBP_GSML_RESERVED_INTR   INT_LVL_G1_BBP_TO_DSP_32K
#define INT_LVL_GBBP                      INT_LVL_G1_BBP_TO_CPU
#define INT_LVL_GBBP_AWAKE                INT_LVL_G1_BBP_TO_CPU_32K 
#define INT_LVL_GBBP1_DSP                 INT_LVL_G2_BBP_TO_DSP
#define INT_LVL_GBBP1_AWAKE_DSP           INT_LVL_G2_BBP_TO_DSP_32K
#define INT_LVL_GBBP1                     INT_LVL_G2_BBP_TO_CPU
#define INT_LVL_GBBP1_AWAKE               INT_LVL_G2_BBP_TO_CPU_32K
#define INT_LVL_WBBP_0MS                  INT_LVL_W_ARM_01
#define INT_LVL_WBBP_AWAKE                INT_LVL_W_ARM_02
#define INT_LVL_WBBP_SWITCH               INT_LVL_W_ARM_03
#define INT_LVL_WBBP1_0MS                 INT_LVL_W_ARM_04
#define INT_LVL_WBBP_CARD2_0MS_INTR       INT_LVL_W_ARM_04
#define INT_LVL_WBBP_SEARCH               INT_LVL_W_ARM_05
#define INT_LVL_WBBP_SEARCH_INTR          INT_LVL_W_ARM_05
#define INT_LVL_WBBP_RAKE                 INT_LVL_W_ARM_06
#define INT_LVL_WBBP_RAKE_INTR            INT_LVL_W_ARM_06
#define INT_LVL_WBBP_DECODE               INT_LVL_W_ARM_07
#define INT_LVL_WBBP_DECODE_INTR          INT_LVL_W_ARM_07
#define INT_LVL_WBBP_TIME                 INT_LVL_W_ARM_08
#define INT_LVL_WBBP_CARD1_TIME_INTR      INT_LVL_W_ARM_08
#define INT_LVL_WBBP1_TIME                INT_LVL_W_ARM_09
#define INT_LVL_WBBP_CARD2_TIME_INTR      INT_LVL_W_ARM_09
#define INT_LVL_WBBP_MULTI_SEARCH         INT_LVL_W_ARM_10
#define INT_LVL_WBBP_MULTI_SEARCH_INTR    INT_LVL_W_ARM_10
#define INT_LVL_WBBP_BBPMST               INT_LVL_W_ARM_11
#define INT_LVL_BBPMASTER                 INT_LVL_W_ARM_11
#define INT_LVL_CTU_INT_G                 INT_LVL_CTU_G1
#define INT_LVL_CTU_INT_W                 INT_LVL_CTU_W
#define INT_LVL_INT12_G2                  1000000

#define INT_LVL_GU_UPACC_DSP              INT_LVL_UPACC_DSP
#define INT_LVL_UPACC_INTR                INT_LVL_UPACC_ARM

#define INT_LVL_GU_HDLC_FRM               INT_LVL_HDLC_FRM
#define INT_LVL_GU_HDLC_DEF               INT_LVL_HDLC_DEF
#define INT_LVL_CICOM_DL                  INT_LVL_CICOM0_DL
#define INT_LVL_CICOM_UL                  INT_LVL_CICOM0_UL
#define INT_LVL_GU_CICOM1_DL              INT_LVL_CICOM1_DL
#define INT_LVL_GU_CICOM1_UL              INT_LVL_CICOM1_UL



/*LTE INT*/
#define INT_LVL_LTE_ARM_POSITION_INT            1000000
#define INT_LVL_LTE_ARM_DL_DMA_INT              1000000
#define INT_LVL_LTE_ARM_CLK_SWITCH_INT          1000000
#define INT_LVL_LTE_ARM_WAKEUP_INT              INT_LVL_LTE_BBP_WAKEUP
#define INT_LVL_LTE_ARM_PUB_INT                 1000000
#define INT_LVL_LTE_APP_ARM_PUB_INT             1000000
#define INT_LVL_LTE_ARM_POSITION_INTS           1000000
#define INT_LVL_LTE_ARM_DL_DMA_INTS             1000000
#define INT_LVL_TDS_STU_ARM_INT                 1000000
#define INT_LVL_TDS_DRX_ARM_CLKSWITCH_INT       1000000
#define INT_LVL_TDS_DRX_ARM_WAKEUP_INT          INT_LVL_TDS_DRX_WAKEUP
#define INT_LVL_VIC_CIPHER                      1000000
#define INT_LVL_VIC_IPF_INTR0                   1000000
#define INT_LVL_VIC_IPF_INTR1                   1000000
#define INT_LVL_VIC_SOCP_INTR0                  1000000
#define INT_LVL_VIC_SOCP_INTR1                  1000000
#define INT_LVL_CTU_INT_LTE                     1000000
#define INT_LVL_CTU_INT_TDS                     1000000

#ifdef __cplusplus
}
#endif

#endif    /* __SOC_INTERRUPTS_COMM_H */

