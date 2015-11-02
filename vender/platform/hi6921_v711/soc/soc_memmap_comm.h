/*************************************************************************
*   版权所有(C) 2008-2013 深圳华为技术有限公司.
*
*   文 件 名 :  soc_memmap_comm.h
*
*   作    者 :  y00184236
*
*   描    述 :  k3v3 v7r2 common memory map
*
*************************************************************************/


#ifndef __SOC_MEMMAP_COMM_H__
#define __SOC_MEMMAP_COMM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "product_config.h"

/* 非2^n大小的宏，内核未定义，需要放到公共区 */

#ifndef SZ_3M
#define SZ_3M                           (0x00300000)
#endif

#ifdef __KERNEL__
#include <asm-generic/sizes.h>
#else
/************************************************************************
                * size *
************************************************************************/
#ifndef SZ_512
#define SZ_512                          (0x00000200)
#endif

#ifndef SZ_1K
#define SZ_1K                           (0x00000400)
#endif

#ifndef SZ_2K
#define SZ_2K                           (0x00000800)
#endif

#ifndef SZ_4K
#define SZ_4K                           (0x00001000)
#endif

#ifndef SZ_8K
#define SZ_8K                           (0x00002000)
#endif

#ifndef SZ_16K
#define SZ_16K                          (0x00004000)
#endif

#ifndef SZ_32K
#define SZ_32K                          (0x00008000)
#endif

#ifndef SZ_64K
#define SZ_64K                          (0x00010000)
#endif

#ifndef SZ_128K
#define SZ_128K                         (0x00020000)
#endif

#ifndef SZ_256K
#define SZ_256K                         (0x00040000)
#endif

#ifndef SZ_512K
#define SZ_512K                         (0x00080000)
#endif

#ifndef SZ_1M
#define SZ_1M                           (0x00100000)
#endif

#ifndef SZ_2M
#define SZ_2M                           (0x00200000)
#endif

#ifndef SZ_4M
#define SZ_4M                           (0x00400000)
#endif

#ifndef SZ_8M
#define SZ_8M                           (0x00800000)
#endif

#ifndef SZ_16M
#define SZ_16M                          (0x01000000)
#endif

#ifndef SZ_32M
#define SZ_32M                          (0x02000000)
#endif

#ifndef SZ_64M
#define SZ_64M                          (0x04000000)
#endif

#ifndef SZ_128M
#define SZ_128M                         (0x08000000)
#endif

#ifndef SZ_256M
#define SZ_256M                         (0x10000000)
#endif

#ifndef SZ_512M
#define SZ_512M                         (0x20000000)
#endif
#endif

/************************************************************************
                * IP BASE ADDR *
************************************************************************/

/* APP GIC */
#define HI_APP_GIC_BASE_ADDR                  0x4000000
#define HI_APP_GIC_SIZE                       SZ_8K

/* MDM GIC */
#define HI_MDM_GIC_BASE_ADDR                  0x8000000
#define HI_MDM_GIC_SIZE                       SZ_8K

/* M3 TCM0 */
#define HI_M3TCM0_MEM_ADDR                    0x10000000
#define HI_M3TCM0_MEM_SIZE                    SZ_32K

/* M3 TCM1 */
#define HI_M3TCM1_MEM_ADDR                    0x10008000
#define HI_M3TCM1_MEM_SIZE                    SZ_32K

/* SRAM */
#define HI_SRAM_MEM_SIZE                      SZ_128K

/* APP A9 L2 reg */
#define HI_APPA9_L2_REGBASE_ADDR              0x4FF00000
#define HI_APPA9_L2_REG_SIZE                  SZ_4K      /* 1M */

/* CS_SYS */
#define HI_CS_SYS_REGBASE_ADDR                0x80000000
#define HI_CS_SYS_REG_SIZE                    SZ_4M

/* PCIE PHY */
#define HI_PCIE_PHY_BASE_ADDR                 0x8FFC0000
#define HI_PCIE_PHY_SIZE                      SZ_256K

/* system controller */
#define HI_SYSCTRL_BASE_ADDR                  0x90000000
#define HI_SYSCTRL_REG_SIZE                   SZ_4K

#define HI_SYSCRG_BASE_ADDR                   HI_SYSCTRL_BASE_ADDR
#define HI_SYSSC_BASE_ADDR                    HI_SYSCTRL_BASE_ADDR
#define HI_SYSSC_AO_BASE_ADDR                    HI_SYSCTRL_BASE_ADDR
#define HI_SYSSC_PD_BASE_ADDR                    HI_SYSCTRL_BASE_ADDR
#define HI_PWRCTRL_BASE_ADDR                  HI_SYSCTRL_BASE_ADDR

/* wdog */
#define HI_WDT_BASE_ADDR                      0x90001000
#define HI_WDT_REG_SIZE                       SZ_4K

/* timer 0~7 */
#define HI_TIMER_00TO07_REGBASE_ADDR          0x90002000
#define HI_TIMER_00TO07_REG_SIZE              SZ_4K

/* timer 8~15 */
#define HI_TIMER_08TO15_REGBASE_ADDR          0x90003000
#define HI_TIMER_08TO15_REG_SIZE              SZ_4K

/* timer 16~23 */
#define HI_TIMER_16TO23_REGBASE_ADDR          0x90004000
#define HI_TIMER_16TO23_REG_SIZE              SZ_4K

#define HI_TIMER_00_REGBASE_ADDR             (HI_TIMER_00TO07_REGBASE_ADDR+0x00)
#define HI_TIMER_01_REGBASE_ADDR             (HI_TIMER_00TO07_REGBASE_ADDR+0x14)
#define HI_TIMER_02_REGBASE_ADDR             (HI_TIMER_00TO07_REGBASE_ADDR+0x28)
#define HI_TIMER_03_REGBASE_ADDR             (HI_TIMER_00TO07_REGBASE_ADDR+0x3C)
#define HI_TIMER_04_REGBASE_ADDR             (HI_TIMER_00TO07_REGBASE_ADDR+0x50)
#define HI_TIMER_05_REGBASE_ADDR             (HI_TIMER_00TO07_REGBASE_ADDR+0x64)
#define HI_TIMER_06_REGBASE_ADDR             (HI_TIMER_00TO07_REGBASE_ADDR+0x78)
#define HI_TIMER_07_REGBASE_ADDR             (HI_TIMER_00TO07_REGBASE_ADDR+0x8C)
#define HI_TIMER_08_REGBASE_ADDR             (HI_TIMER_08TO15_REGBASE_ADDR+0x00)
#define HI_TIMER_09_REGBASE_ADDR             (HI_TIMER_08TO15_REGBASE_ADDR+0x14)
#define HI_TIMER_10_REGBASE_ADDR             (HI_TIMER_08TO15_REGBASE_ADDR+0x28)
#define HI_TIMER_11_REGBASE_ADDR             (HI_TIMER_08TO15_REGBASE_ADDR+0x3C)
#define HI_TIMER_12_REGBASE_ADDR             (HI_TIMER_08TO15_REGBASE_ADDR+0x50)
#define HI_TIMER_13_REGBASE_ADDR             (HI_TIMER_08TO15_REGBASE_ADDR+0x64)
#define HI_TIMER_14_REGBASE_ADDR             (HI_TIMER_08TO15_REGBASE_ADDR+0x78)
#define HI_TIMER_15_REGBASE_ADDR             (HI_TIMER_08TO15_REGBASE_ADDR+0x8C)
#define HI_TIMER_16_REGBASE_ADDR             (HI_TIMER_16TO23_REGBASE_ADDR+0x00)
#define HI_TIMER_17_REGBASE_ADDR             (HI_TIMER_16TO23_REGBASE_ADDR+0x14)
#define HI_TIMER_18_REGBASE_ADDR             (HI_TIMER_16TO23_REGBASE_ADDR+0x28)
#define HI_TIMER_19_REGBASE_ADDR             (HI_TIMER_16TO23_REGBASE_ADDR+0x3C)
#define HI_TIMER_20_REGBASE_ADDR             (HI_TIMER_16TO23_REGBASE_ADDR+0x50)
#define HI_TIMER_21_REGBASE_ADDR             (HI_TIMER_16TO23_REGBASE_ADDR+0x64)
#define HI_TIMER_22_REGBASE_ADDR             (HI_TIMER_16TO23_REGBASE_ADDR+0x78)
#define HI_TIMER_23_REGBASE_ADDR             (HI_TIMER_16TO23_REGBASE_ADDR+0x8C)

/* RTC */
#define HI_RTC_REGBASE_ADDR                  0x90005000
#define HI_RTC_REG_SIZE                      SZ_4K

/*GPIO0*/
#define HI_GPIO0_REGBASE_ADDR                0x90006000
#define HI_GPIO0_REG_SIZE                    SZ_4K

/* UART0 */
#define HI_UART0_REGBASE_ADDR                0x90007000
#define HI_UART0_REG_SIZE                    SZ_4K

/*SPI Master 0*/
#define HI_SPI_MST0_REGBASE_ADDR             0x90008000
#define HI_SPI_MST0_REG_SIZE                 SZ_4K

/*SCI0*/
#define HI_SCI0_REGBASE_ADDR                 0x90009000
#define HI_SCI0_REG_SIZE                     SZ_4K

/*SCI1*/
#define HI_SCI1_REGBASE_ADDR                 0x9000A000
#define HI_SCI1_REG_SIZE                     SZ_4K

/* Tsensor */
#define HI_TSENSOR_REGBASE_ADDR              0x9000B000
#define HI_TSENSOR_REG_SIZE                  SZ_4K

/* bc_ctrl */
#define HI_BC_CTRL_REGBASE_ADDR              0x9000C000
#define HI_BC_CTRL_REG_SIZE                  SZ_4K

/* PMUSSI0 */
#define HI_PMUSSI0_REGBASE_ADDR              0x9000D000
#define HI_PMUSSI0_REG_SIZE                  SZ_4K

/* IOS */
#define HI_IOS_REGBASE_ADDR                  0x90010000
#define HI_IOS_REG_SIZE                      SZ_4K

/* GPIO1 */
#define HI_GPIO1_REGBASE_ADDR                0x90011000
#define HI_GPIO1_REG_SIZE                    SZ_4K

/* GPIO2 */
#define HI_GPIO2_REGBASE_ADDR                0x90012000
#define HI_GPIO2_REG_SIZE                    SZ_4K

/* GPIO3 */
#define HI_GPIO3_REGBASE_ADDR                0x90013000
#define HI_GPIO3_REG_SIZE                    SZ_4K

/* UART1 */
#define HI_UART1_REGBASE_ADDR                0x90016000
#define HI_UART1_REG_SIZE                    SZ_4K

/* UART2 */
#define HI_UART2_REGBASE_ADDR                0x90017000
#define HI_UART2_REG_SIZE                    SZ_4K

/* UART3 */
#define HI_UART3_REGBASE_ADDR                0x90018000
#define HI_UART3_REG_SIZE                    SZ_4K

/* I2C0 */
#define HI_I2C0_REGBASE_ADDR                 0x90019000
#define HI_I2C0_REG_SIZE                     SZ_4K

/* I2C1 */
#define HI_I2C1_REGBASE_ADDR                 0x9001A000
#define HI_I2C1_REG_SIZE                     SZ_4K

/* MDDRC */
#define HI_MDDRC_REGBASE_ADDR                0x90020000
#define HI_MDDRC_REG_SIZE                    SZ_4K

/* efuse */
#define HI_EFUSE_REGBASE_ADDR                0x90021000
#define HI_EFUSE_REG_SIZE                    SZ_4K

/*SPI Master 1*/
#define HI_SPI_MST1_REGBASE_ADDR             0x90023000
#define HI_SPI_MST1_REG_SIZE                 SZ_4K

/* EDMA CH16 */
#define HI_EDMA_REGBASE_ADDR                 0x90024000
#define HI_EDMA_REG_SIZE                     SZ_4K

/* DramTest/ASHB */
#define HI_ASHB_REGBASE_ADDR                 0x90025000
#define HI_ASHB_REG_SIZE                     SZ_4K

/* EDMA CH4 */
#define HI_EDMA_CH4_REGBASE_ADDR             0x90026000
#define HI_EDMA_CH4_REG_SIZE                 SZ_4K

/* ASHB_M3_MST */
#define HI_ASHB_M3_MST_BASE_ADDR             0x90028000
#define HI_ASHB_M3_MST_REG_SIZE              SZ_4K

/* APB_MIPI */
#define HI_APB_MIPI_REGBASE_ADDR             0x9002A000
#define HI_APB_MIPI_REG_SIZE                 SZ_4K

/* SYSREG_PD */
#define HI_SYSREG_PD_BASE_ADDR                  0x9002B000
#define HI_SYSREG_PD_REG_SIZE                 SZ_4K


/* BBP_APB */
#define HI_BBP_APB_REGBASE_ADDR              0x9002C000
#define HI_BBP_APB_REG_SIZE                  SZ_8K

/* PWM0 */
#define HI_PWM0_REGBASE_ADDR                 0x9002E000
#define HI_PWM0_REG_SIZE                     SZ_4K

/* PWM1 */
#define HI_PWM1_REGBASE_ADDR                 0x9002F000
#define HI_PWM1_REG_SIZE                     SZ_4K

/* TCSSI1 */
#define HI_TCSSI1_REGBASE_ADDR               0x90031000
#define HI_TCSSI1_REG_SIZE                   SZ_4K

/* UICC */
#define HI_UICC_REGBASE_ADDR                 0x90040000
#define HI_UICC_REG_SIZE                     SZ_256K

/* HSIC */
#define HI_HSIC_REGBASE_ADDR                 0x90080000
#define HI_HSIC_REG_SIZE                     SZ_128K

/* EMI */
#define HI_EMI_REGBASE_ADDR                  0x900A2000
#define HI_EMI_REG_SIZE                      SZ_4K

/* EMI MEM */
#define HI_EMIMEM_REGBASE_ADDR               0x900A3000
#define HI_EMIMEM_REG_SIZE                   SZ_4K

/* NANDC reg */
#define HI_NANDC_REGBASE_ADDR                0x900A4000
#define HI_NANDC_REG_SIZE                    SZ_4K

/* IPC */
#define HI_IPCM_REGBASE_ADDR                 0x900A5000
#define HI_IPCM_REG_SIZE                     SZ_4K

/* LTE SIO */
#define HI_LTESIO_REGBASE_ADDR               0x900A7000
#define HI_LTESIO_REG_SIZE                   SZ_4K

/* HS UART */
#define HI_HSUART_REGBASE_ADDR               0x900A8000
#define HI_HSUART_REG_SIZE                   SZ_4K

/* SOCP */
#define HI_SOCP_REGBASE_ADDR                 0x900A9000
#define HI_SOCP_REG_SIZE                     SZ_4K

/* CIPHER */
#define HI_CIPHER_BASE_ADDR                  0x900AB000
#define HI_CIPHER_REG_SIZE                   SZ_4K

/* IPF */
#define HI_IPF_REGBASE_ADDR                  0x900AC000
#define HI_IPF_REG_SIZE                      SZ_4K

/* MMC HOST0 */
#define HI_MMC_HOST0_REGBASE_ADDR            0x900AD000
#define HI_MMC_HOST0_REG_SIZE                SZ_4K

/* MMC HOST1 */
#define HI_MMC_HOST1_REGBASE_ADDR            0x900AE000
#define HI_MMC_HOST1_REG_SIZE                SZ_4K

/* SDDC */
#define HI_MMC_DEV_REGBASE_ADDR              0x900B0000
#define HI_MMC_DEV_REG_SIZE                  SZ_4K

/* GMAC */
#define HI_GMAC_REGBASE_ADDR                 0x900C0000
#define HI_GMAC_REG_SIZE                     SZ_64K

/* CICOM0 */
#define HI_CICOM0_REGBASE_ADDR               0x90100000
#define HI_CICOM0_REG_SIZE                   SZ_64K

/* CICOM1 */
#define HI_CICOM1_REGBASE_ADDR               0x90110000
#define HI_CICOM1_REG_SIZE                   SZ_64K

/* HDLC */
#define HI_HDLC_REGBASE_ADDR                 0x90120000
#define HI_HDLC_REG_SIZE                     SZ_64K

/* BBE16 VIC */
#define HI_BBE16VIC_REGBASE_ADDR             0x90131000
#define HI_BBE16VIC_REG_SIZE                 SZ_4K

/* UPACC */
#define HI_UPACC_BASE_ADDR                   0x90135000
#define HI_UPACC_REG_SIZE                    SZ_4K

/* BOOTROM */
#define HI_BOOTROM_REGBASE_ADDR              0x90140000
#define HI_BOOTROM_REG_SIZE                  SZ_64K

/* USB3 */
#define HI_USB3_REGBASE_ADDR                 0x90200000
#define HI_USB3_REG_SIZE                     SZ_1M

/* NAND MEM */
#define HI_NAND_MEM_BUFFER_ADDR              0xA0000000
#define HI_NAND_MEM_BUFFER_SIZE              SZ_512M

/* STM */
#define HI_STM_BASE_ADDR                     0xD0000000
#define HI_STM_SIZE                          SZ_16M

/* AMON_CPUSLOW */
#define HI_AMON_SOC_REGBASE_ADDR             0xD1000000
#define HI_AMON_SOC_REG_SIZE                 SZ_16K

/* AMON_CPUFAST */
#define HI_AMON_CPUFAST_REGBASE_ADDR         0xD1004000
#define HI_AMON_CPUFAST_REG_SIZE             SZ_16K

/* DSP */
#define HI_DSP0DMEM0_BASE_ADDR               0x48F80000
#define HI_DSP0DMEM0_SIZE                    SZ_256K

#define HI_DSP0DMEM1_BASE_ADDR               0x48FC0000
#define HI_DSP0DMEM1_SIZE                    SZ_256K

#define HI_DSP0IMEM0_BASE_ADDR               0x49000000
#define HI_DSP0IMEM0_SIZE                    SZ_256K

#define HI_DSP0IMEM1_BASE_ADDR               0x49040000
#define HI_DSP0IMEM1_SIZE                    SZ_256K

/* HIFI */
#define HI_HIFI2DMEM0_BASE_ADDR              0x4CFF0000
#define HI_HIFI2DMEM0_SIZE                   SZ_32K

#define HI_HIFI2DMEM1_BASE_ADDR              0x4CFFC000
#define HI_HIFI2DMEM1_SIZE                   SZ_16K

#define HI_HIFI2IMEM0_BASE_ADDR              0x4D000000
#define HI_HIFI2IMEM0_SIZE                   SZ_2K

#define HI_HIFI2IMEM1_BASE_ADDR              0x4D000800
#define HI_HIFI2IMEM1_SIZE                   SZ_2K

/* CoreSignt STM */
#define HI_CORESIGHT_STM_BASE_ADDR           0x80001000
#define HI_CORESIGHT_STM_SIZE                SZ_4K

/* CoreSignt ETF */
#define HI_CORESIGHT_ETF_BASE_ADDR           0x80002000
#define HI_CORESIGHT_ETF_SIZE                SZ_4K

/* CoreSignt CTI */
#define HI_CORESIGHT_CTI_BASE_ADDR           0x80003000
#define HI_CORESIGHT_CTI_SIZE                SZ_4K

/* CoreSignt FUNNEL */
#define HI_CORESIGHT_FUNNEL_BASE_ADDR        0x80004000
#define HI_CORESIGHT_FUNNEL_SIZE             SZ_4K

/* shared  DDR */
#define HI_SHARED_DDR_BASE_ADDR           (DDR_SHARED_MEM_ADDR)
#define HI_SHARED_DDR_SIZE                (DDR_SHARED_MEM_SIZE)

/* CoreSignt ETR */
#define HI_CORESIGHT_ETR_BASE_ADDR           0x80006000
#define HI_CORESIGHT_ETR_SIZE                SZ_4K

/* CoreSignt Core0 PTM */
#define HI_CORESIGHT_PTM0_BASE_ADDR          0x8011C000
#define HI_CORESIGHT_PTM0_SIZE               SZ_4K

/* CoreSignt Core1 PTM */
#define HI_CORESIGHT_PTM1_BASE_ADDR          0x8021C000
#define HI_CORESIGHT_PTM1_SIZE               SZ_4K

/*******************************************************************************/


#define HI_SYSSC_GU_BASE_ADDR                 0x27050000
#define HI_SYSSC_GU_BASE_SIZE                 SZ_4K

#define HI_BBPMASTER_REG_BASE_ADDR            HI_WBBP_REG_BASE_ADDR
#define HI_ZSP_AHB_REG_BASE_ADDR              0xFFFFFFFF            /* not found */
#define HI_ZSP_DTCM_REG_BASE_ADDR             0xFFFFFFFF            /* not found */

/*******************************************************************************/
#define HI_BBP_AXI_BASE_ADDR                  (0x20000000)
#define HI_BBP_AXI_SIZE                       SZ_16M

#define HI_BBP_SRC_BASE_ADDR                  (0x20000000)
#define HI_BBP_SRC_SIZE                        SZ_1M

#define HI_BBP_DMA_BASE_ADDR                  (0x20200000)
#define HI_BBP_DMA_SIZE                        SZ_1M

#define HI_BBP_DBG_BASE_ADDR                  (0x20300000)
#define HI_BBP_DBG_SIZE                        SZ_1M

#define HI_BBP_UL_BASE_ADDR                   (0x20600000)
#define HI_BBP_UL_SIZE                         SZ_1M

#define HI_BBP_INT_BASE_ADDR                  (0x20700000)
#define HI_BBP_INT_SIZE                        SZ_4K

#define HI_BBP_STU_BASE_ADDR                  (0x2070e000)
#define HI_BBP_STU_SIZE                        SZ_4K

#define HI_GBBP_REG_BASE_ADDR                 (0x20800000)
#define HI_GBBP_REG_SIZE                       SZ_512K

#define HI_GBBP1_REG_BASE_ADDR                (0x20880000)
#define HI_GBBP1_REG_SIZE                      SZ_512K

#define HI_WBBP_REG_BASE_ADDR                 (0x20900000)
#define HI_WBBP_REG_REG_SIZE                   SZ_1M
/*for bbp stress*/
#define HI_CQI_BASE_ADDR                      (0x20A00000)
#define HI_CQI_SIZE                            SZ_1M

#define HI_BBP_TSTU_BASE_ADDR				  (0x20d00000)
#define HI_BBP_TSTU_SIZE                       SZ_8K

/*for hifi bbit*/
#define SOC_BBP_TDS_BASE_ADDR                 HI_BBP_TSTU_BASE_ADDR

#define HI_CTU_BASE_ADDR                      (0x20f80000)
#define HI_CTU_SIZE                            SZ_32K

#define HI_BBP_CTU_BASE_ADDR                 HI_CTU_BASE_ADDR

#define HI_ABB_REG_BASE_ADDR               (0x20F8C000)
#define HI_ABB_REG_SIZE                       (0x2000)

/* PCIE DATA */
#define HI_PCIE_DATA_BASE_ADDR                (0x28000000)
#define HI_PCIE_DATA_SIZE                     (SZ_256M)

/* PCIE CFG */
#define HI_PCIE_CFG_BASE_ADDR                 (0x38000000)
#define HI_PCIE_CFG_SIZE                      (SZ_64K)

#define HI_BBP_SYSTIME_BASE_ADDR             HI_SYSCTRL_BASE_ADDR
#define HI_BBP_SYSTIME_SIZE                  HI_SYSCTRL_REG_SIZE

/*bbp lte drx*/
#define HI_BBP_LTEDRX_BASE_ADDR               (0x9002C000)

#define HI_GBBP_DRX_REG_BASE_ADDR             (0x9002c800)

#define HI_GBBP1_DRX_REG_BASE_ADDR	          (0x9002CC00)

/*bbp tds drx*/
#define HI_BBP_TDSDRX_BASE_ADDR               (0x9002D400)

/* HKADC SSI */
#define HI_HKADCSSI_REGBASE_ADDR              (0x9002D800)

/*COMM ON的基地址为(0x9002D800),根据luxiaohua要求，返回0x9002a000*/
#define HI_BBP_COMM_ON_BASE_ADDR              HI_APB_MIPI_REGBASE_ADDR

/*W DRX的基地址为(0x9002DC00),根据luxiaohua要求，返回0x9002a000*/
#define HI_WBBP_DRX_REG_BASE_ADDR	          HI_APB_MIPI_REGBASE_ADDR



#ifdef BSP_CONFIG_V7R2_SFT
/* CIPHER */
#undef HI_CIPHER_BASE_ADDR
#define HI_CIPHER_BASE_ADDR                   (0x220ab000)

/* IPF */
#undef HI_IPF_REGBASE_ADDR
#define HI_IPF_REGBASE_ADDR                   (0x220ac000)

/* BBP COMM */
#undef HI_BBP_COMM_ON_BASE_ADDR
#define HI_BBP_COMM_ON_BASE_ADDR              (0x20830000)

/*GBBP1 DRX BASE*/
#undef HI_GBBP1_DRX_REG_BASE_ADDR
#define HI_GBBP1_DRX_REG_BASE_ADDR	          (0x20890000)

/*GBBP DRX BASE*/
#undef HI_GBBP_DRX_REG_BASE_ADDR
#define HI_GBBP_DRX_REG_BASE_ADDR             (0x20810000)

/* CTU */
#ifdef FEATURE_MULTIMODE_GUL /* 多模 ，by 陆宏伟 */
#define HI_CTU_BASE_ADDR            0x20820000
#else /* 单模版本*/
#define HI_CTU_BASE_ADDR      		0x20f80000
#endif
#define HI_CTU_SIZE                           (SZ_32K)

/* CICOM0 */
#undef HI_CICOM0_REGBASE_ADDR
#define HI_CICOM0_REGBASE_ADDR                (0x27010000)

/* UPACC */
#undef HI_UPACC_BASE_ADDR
#define HI_UPACC_BASE_ADDR                    (0x27040000)

/* CICOM1 */
#undef HI_CICOM1_REGBASE_ADDR
#define HI_CICOM1_REGBASE_ADDR                (0x27070000)

/* HDLC */
#undef HI_HDLC_REGBASE_ADDR
#define HI_HDLC_REGBASE_ADDR                  (0x270C0000)

/* only for Acore multi mode */
#define HI_FPGA_RESERVED_SPACE_FOR_MODE_SW_BASE_ADDR   0x220a2000
#define HI_FPGA_RESERVED_SPACE_FOR_MODE_SW_SIZE        SZ_4K
#endif


#ifdef __cplusplus
}
#endif

#endif    /* __SOC_MEMMAP_COMM_H__ */

