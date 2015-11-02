/* MD5: b4c8399d725f9fd96c3c9b205069a357*/
#if !defined(__PRODUCT_CONFIG_H__)
#define __PRODUCT_CONFIG_H__

#ifndef BALONG_OBUILD_VERSION
#define BALONG_OBUILD_VERSION 10 
#endif 

#ifndef PRODUCT_NAME
#define PRODUCT_NAME "v711_udp" 
#endif 

#ifndef PRODUCT_CFG_CHIP_SOLUTION_NAME
#define PRODUCT_CFG_CHIP_SOLUTION_NAME "Balong" 
#endif 

#ifndef BUILD_TYPE
#define BUILD_TYPE RELEASE 
#endif 

#ifndef PRODUCT_VERSION
#define PRODUCT_VERSION V700R110C30B210 
#endif 

#ifndef PRODUCT_VERSION_STR
#define PRODUCT_VERSION_STR "V700R110C30B210" 
#endif 

#ifndef PRODUCT_FULL_VERSION_STR
#define PRODUCT_FULL_VERSION_STR "Balong V700R110C30B210" 
#endif 

#ifndef PRODUCT_FILE_VERSION_STR
#define PRODUCT_FILE_VERSION_STR "2.1.6.8" 
#endif 

#ifndef PRODUCT_DLOAD_SOFTWARE_VER		
#define PRODUCT_DLOAD_SOFTWARE_VER		 "21.210.00.00.030" 
#endif 

#ifndef FEATURE_OBJ_CMP 				
#endif 

#ifndef FEATURE_OBJ_DUMP				
#endif 

#ifndef USE_USBLOADER_MERGE 			
#define USE_USBLOADER_MERGE 			 
#endif 

#ifndef BSP_ENBALE_PACK_IMAGE			
#define BSP_ENBALE_PACK_IMAGE			 
#endif 

#ifndef BSP_USB_BURN					
#define BSP_USB_BURN					 
#endif 

#ifndef PLATFORM
#define PLATFORM hi6921_v711 
#endif 

#ifndef FEATURE_MULTIMODE_GUL
#define FEATURE_MULTIMODE_GUL 
#endif 

#ifndef OS_PATH
#define OS_PATH config/product/hi6921_v711_udp/os 
#endif 

#ifndef OS_IMG_FILE
#define OS_IMG_FILE VXWORKS.6.8.3.IMG.wpj 
#endif 

#ifndef OS_LIB
#define OS_LIB VXWORKS.6.8.3.LIB.RELEASE 
#endif 

#ifndef FTP_REPLY_TIME_OUT
#define FTP_REPLY_TIME_OUT 0 
#endif 

#ifndef OS_LINUX_PRODUCT_NAME
#define OS_LINUX_PRODUCT_NAME balongv7r2_defconfig 
#endif 

#ifndef OS_ANDROID_PRODUCT_NAME
#define OS_ANDROID_PRODUCT_NAME balongv7r2 
#endif 

#ifndef PRODUCT_CONTROL_VERSION
#define PRODUCT_CONTROL_VERSION "121" 
#endif 

#ifndef PRODUCT_HISILICON_VERSION
#define PRODUCT_HISILICON_VERSION hi6921 
#endif 

#ifndef PRODUCT_NV_SPLITFILE
#define PRODUCT_NV_SPLITFILE binfilesplit "0" 
#endif 

#ifndef PRODUCT_NV_FILE_MAX_SIZE
#define PRODUCT_NV_FILE_MAX_SIZE binfileLimitSize "100" 
#endif 

#ifndef PRODUCT_NV_BIN_MODEM_NUNBER
#define PRODUCT_NV_BIN_MODEM_NUNBER bin_modem_number "1" 
#endif 

#ifndef PRODUCT_NV_XNV_MODEM_NUNBER
#define PRODUCT_NV_XNV_MODEM_NUNBER xnv_modem_number "1" 
#endif 

#ifndef LNV_PRO_NAME
#define LNV_PRO_NAME hi6921_v711_udp 
#endif 

#ifndef XTENSA_CORE				
#define XTENSA_CORE				 V7R2_bbe16 
#endif 

#ifndef XTENSA_SYSTEM			
#define XTENSA_SYSTEM			 $(ROOT_XTENSA_PATH_W)/XtDevTools/install/builds/RD-2012.5-linux/V7R2_bbe16/config 
#endif 

#ifndef LPHY_UNI_DTCM_BASE	
#define LPHY_UNI_DTCM_BASE	 0x48f80000 
#endif 

#ifndef LPHY_UNI_ITCM_BASE	
#define LPHY_UNI_ITCM_BASE	 0x49000000 
#endif 

#ifndef LPHY_PUB_DTCM_BASE		
#define LPHY_PUB_DTCM_BASE		 0x48f80000 
#endif 

#ifndef LPHY_PUB_ITCM_BASE		
#define LPHY_PUB_ITCM_BASE		 0x49000000 
#endif 

#ifndef LPHY_PRV_DTCM_BASE		
#define LPHY_PRV_DTCM_BASE		 0x48fc0000 
#endif 

#ifndef LPHY_PRV_ITCM_BASE		
#define LPHY_PRV_ITCM_BASE		 0x49038000 
#endif 

#ifndef LPHY_UNI_DTCM_SIZE		
#define LPHY_UNI_DTCM_SIZE		 0x70000 
#endif 

#ifndef LPHY_UNI_ITCM_SIZE		
#define LPHY_UNI_ITCM_SIZE		 0x70000 
#endif 

#ifndef LPHY_PUB_DTCM_SIZE		
#define LPHY_PUB_DTCM_SIZE		 0x40000 
#endif 

#ifndef LPHY_PUB_ITCM_SIZE		
#define LPHY_PUB_ITCM_SIZE		 0x38000 
#endif 

#ifndef LPHY_PRV_DTCM_SIZE		
#define LPHY_PRV_DTCM_SIZE		 0x30000 
#endif 

#ifndef LPHY_PRV_ITCM_SIZE		
#define LPHY_PRV_ITCM_SIZE		 0x38000 
#endif 

#ifndef TL_PHY_LALG_PATH    	
#define TL_PHY_LALG_PATH    	 include/phy 
#endif 

#ifndef TL_PHY_ASIC_V711  	
#define TL_PHY_ASIC_V711  	 
#endif 

#ifndef TL_PHY_6930         	
#define TL_PHY_6930         	 
#endif 

#ifndef TL_PHY_FEATURE_LTE_LCS
#endif 

#ifndef GU_INC_PATH
#define GU_INC_PATH COMM_CODE_GU/Balong_GU_Inc/GUL_CS 
#endif 

#ifndef GU_PRODUCT_VERSION
#define GU_PRODUCT_VERSION VERSION_V7R1 
#endif 

#ifndef RAT_GU
#define RAT_GU 0 
#endif 

#ifndef RAT_GUL
#define RAT_GUL 1 
#endif 

#ifndef RAT_MODE
#define RAT_MODE RAT_GUL 
#endif 

#ifndef GU_RAT_MODE
#define GU_RAT_MODE RAT_GUL 
#endif 

#ifndef PLATFORM_HISI_BALONG
#define PLATFORM_HISI_BALONG hi6921_v711 
#endif 

#ifndef TTF_SKB_EXP
#endif 

#ifndef GU_FEATURE_CONFIG_PATH
#define GU_FEATURE_CONFIG_PATH config/product/hi6921_v711_udp/include_gu 
#endif 

#ifndef HIFI_INCLUDE
#define HIFI_INCLUDE 
#endif 

#ifndef HIFI_LINK_DIR_NAME
#define HIFI_LINK_DIR_NAME hifi_ld_V7R2_SFT 
#endif 

#ifndef BSP_CONFIG_EDA
#endif 

#ifndef BSP_CONFIG_EMU
#endif 

#ifndef BSP_CONFIG_NOT_DDR_BYPASSPLL
#endif 

#ifndef BSP_CONFIG_V711_ASIC
#define BSP_CONFIG_V711_ASIC 
#endif 

#ifndef BSP_CONFIG_BOARD_UDP
#define BSP_CONFIG_BOARD_UDP 
#endif 

#ifndef BSP_HAS_SEC_FEATURE
#endif 

#ifndef CONFIG_FASTBOOT_DEBUG
#endif 

#ifndef CONFIG_FASTBOOT_UART_NUM
#define CONFIG_FASTBOOT_UART_NUM 0 
#endif 

#ifndef CONFIG_COMPRESS_CCORE_IMAGE
#define CONFIG_COMPRESS_CCORE_IMAGE 
#endif 

#ifndef CONFIG_MODULE_VIC
#endif 

#ifndef CONFIG_PCIE_CFG
#define CONFIG_PCIE_CFG 
#endif 

#ifndef CONFIG_CIPHER
#define CONFIG_CIPHER 
#endif 

#ifndef CONFIG_AT_UART
#define CONFIG_AT_UART 
#endif 

#ifndef CONFIG_CSHELL
#define CONFIG_CSHELL 
#endif 

#ifndef CONFIG_IPF
#define CONFIG_IPF 
#endif 

#ifndef CONFIG_MODULE_BUSSTRESS
#endif 

#ifndef CONFIG_ICC
#define CONFIG_ICC 
#endif 

#ifndef CONFIG_BALONG_RTC
#endif 

#ifndef CONFIG_HI6551_RTC
#endif 

#ifndef CONFIG_HI6559_RTC
#define CONFIG_HI6559_RTC 
#endif 

#ifndef CONFIG_MEM
#define CONFIG_MEM 
#endif 

#ifndef CONFIG_MDDR_WINBOND
#endif 

#ifndef CONFIG_CPUFREQ
#define CONFIG_CPUFREQ 
#endif 

#ifndef CONFIG_REGULATOR
#define CONFIG_REGULATOR 
#endif 

#ifndef CONFIG_CCORE_WDT
#define CONFIG_CCORE_WDT 
#endif 

#ifndef CONFIG_ACORE_WDT
#define CONFIG_ACORE_WDT 
#endif 

#ifndef CONFIG_PMIC_HI6451
#endif 

#ifndef CONFIG_PMIC_HI6551
#endif 

#ifndef CONFIG_PMIC_HI6559
#define CONFIG_PMIC_HI6559 
#endif 

#ifndef CONFIG_COUL
#endif 

#ifndef CONFIG_SPI
#endif 

#ifndef CONFIG_SFLASH
#endif 

#ifndef CONFIG_ABB
#define CONFIG_ABB 
#endif 

#ifndef CONFIG_DSP
#define CONFIG_DSP 
#endif 

#ifndef CONFIG_HIFI
#define CONFIG_HIFI 
#endif 

#ifndef CONFIG_HKADC
#define CONFIG_HKADC 
#endif 

#ifndef CONFIG_ONOFF
#define CONFIG_ONOFF 
#endif 

#ifndef CONFIG_BBP_INT
#define CONFIG_BBP_INT 
#endif 

#ifndef FEATURE_WIFI_ON
#endif 

#ifndef FEATURE_BCM_VERSION
#define FEATURE_BCM_VERSION 43241 
#endif 

#ifndef CONFIG_NVIM
#define CONFIG_NVIM 
#endif 

#ifndef FEATURE_NV_FLASH_ON
#define FEATURE_NV_FLASH_ON 
#endif 

#ifndef FEATURE_NV_EMMC_ON
#endif 

#ifndef FEATURE_NV_LFILE_ON
#endif 

#ifndef FEATURE_NV_RFILE_ON
#endif 

#ifndef FEATURE_UPGRADE_TL
#define FEATURE_UPGRADE_TL 
#endif 

#ifndef FEATURE_TLPHY_MAILBOX
#define FEATURE_TLPHY_MAILBOX 
#endif 

#ifndef CONFIG_PASTAR
#endif 

#ifndef CONFIG_PASTAR_V200
#endif 

#ifndef CONFIG_MIPI
#endif 

#ifndef ENABLE_SHELL_SYM
#define ENABLE_SHELL_SYM 
#endif 

#ifndef CCORE_COMPONENTS
#define CCORE_COMPONENTS bsp_ccore.o gumsp_mcore.o nas_ccore.o rabm_ccore.o gas_ccore.o was_ccore.o ttf_ccore.o ascomm_ccore.o pscomm_ccore.o taf_ccore.o tlas_ccore.o tlnas_ccore.o lmsp_ccore.o gudsp_mcore.o gudsp_comm_RT.o gudsp_mcore_RT.o os_ccore.o 
#endif 

#ifndef CCORE_MODEM1_COMPONENTS
#define CCORE_MODEM1_COMPONENTS bsp_ccore1.o nas1_ccore.o rabm1_ccore.o gas1_ccore.o taf1_ccore.o ascomm1_ccore.o pscomm1_ccore.o gumsp1_mcore.o lmsp1_ccore.o gudsp1_mcore.o gudsp1_mcore_RT.o ttf1_ccore.o 
#endif 

#ifndef CCORE_MODEMUNION_COMPONENTS
#define CCORE_MODEMUNION_COMPONENTS modemcore1_union.o 
#endif 

#ifndef BUILD_DRV_ONLY
#endif 

#ifndef ENABLE_TEST_CODE
#endif 

#ifndef ENABLE_BUILD_VARS
#define ENABLE_BUILD_VARS 
#endif 

#ifndef ENABLE_BUILD_OM
#define ENABLE_BUILD_OM 
#endif 

#ifndef ENABLE_BUILD_SYSVIEW
#define ENABLE_BUILD_SYSVIEW 
#endif 

#ifndef ENABLE_BUILD_CPUVIEW
#define ENABLE_BUILD_CPUVIEW 
#endif 

#ifndef ENABLE_BUILD_MEMVIEW
#define ENABLE_BUILD_MEMVIEW 
#endif 

#ifndef ENABLE_BUILD_AMON
#define ENABLE_BUILD_AMON 
#endif 

#ifndef ENABLE_BUILD_UTRACE
#define ENABLE_BUILD_UTRACE 
#endif 

#ifndef ENABLE_BUILD_SOCP
#define ENABLE_BUILD_SOCP 
#endif 

#ifndef CONFIG_CCORE_CPU_IDLE
#define CONFIG_CCORE_CPU_IDLE 
#endif 

#ifndef CONFIG_BSP_TEST_CCORE
#endif 

#ifndef CONFIG_BALONG_CCLK
#define CONFIG_BALONG_CCLK 
#endif 

#ifndef FEATURE_E5_ONOFF
#endif 

#ifndef CONFIG_CCORE_PM
#define CONFIG_CCORE_PM 
#endif 

#ifndef CONFIG_MODULE_IPC
#define CONFIG_MODULE_IPC 
#endif 

#ifndef CONFIG_MODULE_TIMER
#define CONFIG_MODULE_TIMER 
#endif 

#ifndef CONFIG_MODULE_SYNC
#define CONFIG_MODULE_SYNC 
#endif 

#ifndef CONFIG_LEDS_CCORE
#endif 

#ifndef CONFIG_HAS_CCORE_WAKELOCK
#define CONFIG_HAS_CCORE_WAKELOCK 
#endif 

#ifndef CONFIG_CCORE_BALONG_PM
#define CONFIG_CCORE_BALONG_PM 
#endif 

#ifndef CONFIG_BALONG_EDMA
#define CONFIG_BALONG_EDMA 
#endif 

#ifndef CONFIG_I2C_TEST	
#endif 

#ifndef CONFIG_IPF_AD_RPRT_FIX
#define CONFIG_IPF_AD_RPRT_FIX 
#endif 

#ifndef CONFIG_IPF_SCH_ALL_DESC
#define CONFIG_IPF_SCH_ALL_DESC 
#endif 

#ifndef CONFIG_FB_SPI_BALONG
#endif 

#ifndef CONFIG_FB_EMI_BALONG
#endif 

#ifndef CONFIG_FB_1_4_5_INCH_BALONG
#endif 

#ifndef CONFIG_FB_2_4_INCH_BALONG
#endif 

#ifndef CONFIG_SLIC
#define CONFIG_SLIC m 
#endif 

#ifndef CONFIG_WM8990
#endif 

#ifndef CONFIG_GPIO_EXPANDER
#define CONFIG_GPIO_EXPANDER 
#endif 

#ifndef CONFIG_TEMPERATURE_PROTECT
#define CONFIG_TEMPERATURE_PROTECT 
#endif 

#ifndef CONFIG_ANTEN
#endif 

#ifndef CONFIG_EFUSE
#define CONFIG_EFUSE 
#endif 

#ifndef CONFIG_BURN_EFUSE_NANDC
#endif 

#ifndef CONFIG_DDM
#define CONFIG_DDM 
#endif 

#ifndef CONFIG_TSENSOR
#define CONFIG_TSENSOR 
#endif 

#ifndef CONFIG_DESIGNWARE_I2C
#define CONFIG_DESIGNWARE_I2C 
#endif 

#ifndef CONFIG_USB_DWC3_VBUS_DISCONNECT
#define CONFIG_USB_DWC3_VBUS_DISCONNECT 
#endif 

#ifndef USB3_SYNOPSYS_PHY
#define USB3_SYNOPSYS_PHY 
#endif 

#ifndef CONFIG_USB_FORCE_HIGHSPEED
#define CONFIG_USB_FORCE_HIGHSPEED 
#endif 

#ifndef CONFIG_PWC_MNTN_CCORE
#define CONFIG_PWC_MNTN_CCORE 
#endif 

#ifndef CONFIG_TCXO_BALONG
#endif 

#ifndef FEATURE_SIM_NOTIFY
#endif 

#ifndef CONFIG_CCORE_I2C
#endif 

#ifndef CONFIG_VFP
#define CONFIG_VFP 
#endif 

#ifndef CONFIG_BALONG_DPM
#define CONFIG_BALONG_DPM 
#endif 

#ifndef CONFIG_BALONG_HPM_TEMP
#define CONFIG_BALONG_HPM_TEMP 
#endif 

#ifndef CONFIG_M2M_HSUART
#define CONFIG_M2M_HSUART 
#endif 

#ifndef PRODUCT_CFG_PTABLE_VER_NAME
#define PRODUCT_CFG_PTABLE_VER_NAME "HI6921_V711_UDP" 
#endif 

#ifndef PRODUCT_CFG_PTABLE_VER_ID			
#define PRODUCT_CFG_PTABLE_VER_ID			 "ptable 1.00" 
#endif 

#ifndef PRODUCT_CFG_FLASH_TOTAL_SIZE
#define PRODUCT_CFG_FLASH_TOTAL_SIZE 0x10000000 
#endif 

#ifndef PRODUCT_CFG_FLASH_M3BOOT_LEN     	
#define PRODUCT_CFG_FLASH_M3BOOT_LEN     	 0x40000 
#endif 

#ifndef PRODUCT_CFG_FLASH_FASTBOOT_LEN
#define PRODUCT_CFG_FLASH_FASTBOOT_LEN 0xC0000 
#endif 

#ifndef PRODUCT_CFG_FLASH_NV_LTE_LEN
#define PRODUCT_CFG_FLASH_NV_LTE_LEN 0x400000 
#endif 

#ifndef PRODUCT_CFG_FLASH_NV_IMG_LEN
#define PRODUCT_CFG_FLASH_NV_IMG_LEN 0x400000 
#endif 

#ifndef PRODUCT_CFG_FLASH_NV_DLD_LEN      	
#define PRODUCT_CFG_FLASH_NV_DLD_LEN      	 0x200000 
#endif 

#ifndef PRODUCT_CFG_FLASH_NV_FACTORY_LEN   	
#define PRODUCT_CFG_FLASH_NV_FACTORY_LEN   	 0x200000 
#endif 

#ifndef PRODUCT_CFG_FLASH_OEMINFO_LEN
#define PRODUCT_CFG_FLASH_OEMINFO_LEN 0x200000 
#endif 

#ifndef PRODUCT_CFG_FLASH_YAFFS_USERDATA_LEN
#define PRODUCT_CFG_FLASH_YAFFS_USERDATA_LEN 0x600000 
#endif 

#ifndef PRODUCT_CFG_FLASH_YAFFS_ONLINE_LEN
#define PRODUCT_CFG_FLASH_YAFFS_ONLINE_LEN 0 
#endif 

#ifndef PRODUCT_CFG_FLASH_BOOTIMG_LEN	
#define PRODUCT_CFG_FLASH_BOOTIMG_LEN	 0x600000 
#endif 

#ifndef PRODUCT_CFG_FLASH_BOOTIMGBAK_LEN	
#define PRODUCT_CFG_FLASH_BOOTIMGBAK_LEN	 0 
#endif 

#ifndef PRODUCT_CFG_FLASH_LOGO_LEN
#define PRODUCT_CFG_FLASH_LOGO_LEN 0x100000 
#endif 

#ifndef PRODUCT_CFG_FLASH_DSP_LEN  		
#define PRODUCT_CFG_FLASH_DSP_LEN  		 0x400000 
#endif 

#ifndef PRODUCT_CFG_FLASH_MISC_LEN
#define PRODUCT_CFG_FLASH_MISC_LEN 0x100000 
#endif 

#ifndef PRODUCT_CFG_FLASH_CCORE_LEN  	
#define PRODUCT_CFG_FLASH_CCORE_LEN  	 0x1000000 
#endif 

#ifndef PRODUCT_CFG_FLASH_M3IMAGE_LEN 	
#define PRODUCT_CFG_FLASH_M3IMAGE_LEN 	 0x100000 
#endif 

#ifndef PRODUCT_CFG_FLASH_RESERVE1_LEN
#define PRODUCT_CFG_FLASH_RESERVE1_LEN 0 
#endif 

#ifndef PRODUCT_CFG_FLASH_RESERVE2_LEN
#define PRODUCT_CFG_FLASH_RESERVE2_LEN 0 
#endif 

#ifndef PRODUCT_CFG_FLASH_RESERVE3_LEN
#define PRODUCT_CFG_FLASH_RESERVE3_LEN 0 
#endif 

#ifndef PRODUCT_CFG_FLASH_OM_LEN
#define PRODUCT_CFG_FLASH_OM_LEN 0xC00000 
#endif 

#ifndef PRODUCT_CFG_FLASH_YAFFS_APP_LEN
#define PRODUCT_CFG_FLASH_YAFFS_APP_LEN 0x0 
#endif 

#ifndef PRODUCT_CFG_FLASH_YAFFS_WEBUI_LEN 	
#define PRODUCT_CFG_FLASH_YAFFS_WEBUI_LEN 	 0x0 
#endif 

#ifndef PRODUCT_CFG_FLASH_SYSIMG_LEN
#define PRODUCT_CFG_FLASH_SYSIMG_LEN 0x1400000 
#endif 

#ifndef PRODUCT_CFG_FLASH_ISO_LEN
#define PRODUCT_CFG_FLASH_ISO_LEN 0x1000000 
#endif 

#ifndef PRODUCT_CFG_FLASH_CACHE_LEN   	
#define PRODUCT_CFG_FLASH_CACHE_LEN   	 0x400000 
#endif 

#ifndef PRODUCT_CFG_FLASH_RECOVERY_LEN
#define PRODUCT_CFG_FLASH_RECOVERY_LEN 0x400000 
#endif 

#ifndef FLASH_PTABLE_OFFSET						
#define FLASH_PTABLE_OFFSET						 0x1F800 
#endif 

#ifndef MCORE_TEXT_START_ADDR
#define MCORE_TEXT_START_ADDR 0x50D10000 
#endif 

#ifndef FASTBOOT_ENTRY
#define FASTBOOT_ENTRY 0x4fe00000 
#endif 

#ifndef FASTBOOT_DDR_ENTRY
#define FASTBOOT_DDR_ENTRY 0x57d00000 
#endif 

#ifndef PRODUCT_CFG_KERNEL_ENTRY 		
#define PRODUCT_CFG_KERNEL_ENTRY 		 0x55E08000 
#endif 

#ifndef PRODUCT_KERNEL_PARAMS_PHYS		
#define PRODUCT_KERNEL_PARAMS_PHYS		 0x55E00100 
#endif 

#ifndef ONCHIP_FASTBOOT_ADDR
#define ONCHIP_FASTBOOT_ADDR 0x57900000 
#endif 

#ifndef RTX_KERNEL_ENTRY
#define RTX_KERNEL_ENTRY 0x10000000 
#endif 

#ifndef HI_SRAM_MEM_ADDR
#define HI_SRAM_MEM_ADDR 0x4fe00000 
#endif 

#ifndef DDR_MEM_ADDR       		
#define DDR_MEM_ADDR       		 0x50000000 
#endif 

#ifndef DDR_MEM_SIZE       		
#define DDR_MEM_SIZE       		 0x8D00000 
#endif 

#ifndef DDR_APP_ACP_ADDR
#define DDR_APP_ACP_ADDR (DDR_MEM_ADDR) 
#endif 

#ifndef DDR_APP_ACP_SIZE
#define DDR_APP_ACP_SIZE 0x000000 
#endif 

#ifndef DDR_GU_ADDR
#define DDR_GU_ADDR ((DDR_APP_ACP_ADDR) + (DDR_APP_ACP_SIZE)) 
#endif 

#ifndef DDR_GU_SIZE
#define DDR_GU_SIZE 0xB00000 
#endif 

#ifndef DDR_GU_ADDR_VIRT			
#define DDR_GU_ADDR_VIRT			 0xF3000000 
#endif 

#ifndef DDR_TLPHY_IMAGE_ADDR     	
#define DDR_TLPHY_IMAGE_ADDR     	 ((DDR_GU_ADDR) + (DDR_GU_SIZE)) 
#endif 

#ifndef DDR_TLPHY_IMAGE_SIZE     	
#define DDR_TLPHY_IMAGE_SIZE     	 0x200000 
#endif 

#ifndef DDR_MCORE_ADDR
#define DDR_MCORE_ADDR (( DDR_TLPHY_IMAGE_ADDR ) + (DDR_TLPHY_IMAGE_SIZE)) 
#endif 

#ifndef DDR_MCORE_SIZE
#define DDR_MCORE_SIZE 0x4600000 
#endif 

#ifndef DDR_SHARED_MEM_ADDR
#define DDR_SHARED_MEM_ADDR ((DDR_MCORE_ADDR) + (DDR_MCORE_SIZE)) 
#endif 

#ifndef DDR_SHARED_MEM_SIZE
#define DDR_SHARED_MEM_SIZE 0x300000 
#endif 

#ifndef DDR_MNTN_ADDR
#define DDR_MNTN_ADDR ((DDR_SHARED_MEM_ADDR) + (DDR_SHARED_MEM_SIZE)) 
#endif 

#ifndef DDR_MNTN_SIZE
#define DDR_MNTN_SIZE 0x100000 
#endif 

#ifndef DDR_LPHY_SDR_ADDR		
#define DDR_LPHY_SDR_ADDR		 ((DDR_MNTN_ADDR) + (DDR_MNTN_SIZE)) 
#endif 

#ifndef DDR_LPHY_SDR_SIZE     	
#define DDR_LPHY_SDR_SIZE     	 0x200000 
#endif 

#ifndef DDR_LCS_ADDR
#define DDR_LCS_ADDR ((DDR_LPHY_SDR_ADDR) + (DDR_LPHY_SDR_SIZE)) 
#endif 

#ifndef DDR_LCS_SIZE
#define DDR_LCS_SIZE 0x280000 
#endif 

#ifndef CORESHARE_MEM_TENCILICA_MULT_BAND_SIZE
#define CORESHARE_MEM_TENCILICA_MULT_BAND_SIZE 0x8000 
#endif 

#ifndef CORESHARE_MEM_TENCILICA_MULT_BAND_ADDR
#define CORESHARE_MEM_TENCILICA_MULT_BAND_ADDR DDR_SHARED_MEM_ADDR 
#endif 

#ifndef DDR_HIFI_ADDR
#define DDR_HIFI_ADDR ((DDR_LCS_ADDR) + (DDR_LCS_SIZE)) 
#endif 

#ifndef DDR_HIFI_SIZE
#define DDR_HIFI_SIZE 0x200000 
#endif 

#ifndef DDR_RESERVED_ADDR
#define DDR_RESERVED_ADDR ((DDR_HIFI_ADDR) + (DDR_HIFI_SIZE)) 
#endif 

#ifndef DDR_RESERVED_SIZE
#define DDR_RESERVED_SIZE 0x80000 
#endif 

#ifndef DDR_ACORE_ADDR
#define DDR_ACORE_ADDR ((DDR_RESERVED_ADDR) + (DDR_RESERVED_SIZE)) 
#endif 

#ifndef DDR_ACORE_SIZE
#define DDR_ACORE_SIZE 0x1F00000 
#endif 

#ifndef DDR_SOCP_ADDR
#define DDR_SOCP_ADDR ((DDR_ACORE_ADDR) + (DDR_ACORE_SIZE)) 
#endif 

#ifndef DDR_SOCP_SIZE
#define DDR_SOCP_SIZE 0x300000 
#endif 

#ifndef DDR_MDM_ACP_ADDR
#define DDR_MDM_ACP_ADDR ((DDR_SOCP_ADDR) + (DDR_SOCP_SIZE)) 
#endif 

#ifndef DDR_MDM_ACP_SIZE
#define DDR_MDM_ACP_SIZE 0x0 
#endif 

#ifndef GLOBAL_AXI_TEMP_PROTECT_ADDR
#define GLOBAL_AXI_TEMP_PROTECT_ADDR 0x4FE1FF1C 
#endif 

#ifndef GLOBAL_AXI_TEMP_PROTECT_SIZE
#define GLOBAL_AXI_TEMP_PROTECT_SIZE (128) 
#endif 

#ifndef THUMB_COMPILE
#define THUMB_COMPILE 
#endif 

#ifndef FEATURE_ON
#define FEATURE_ON 1 
#endif 

#ifndef FEATURE_OFF
#define FEATURE_OFF 0 
#endif 

#ifndef MEMORY_SIZE_32M
#define MEMORY_SIZE_32M 1 
#endif 

#ifndef MEMORY_SIZE_64M
#define MEMORY_SIZE_64M 2 
#endif 

#ifndef MEMORY_SIZE_128M
#define MEMORY_SIZE_128M 3 
#endif 

#ifndef MEMORY_SIZE_256M
#define MEMORY_SIZE_256M 4 
#endif 

#ifndef MEMORY_SIZE_512M
#define MEMORY_SIZE_512M 5 
#endif 

#ifndef PS_PTL_VER_DANAMIC
#define PS_PTL_VER_DANAMIC 20 
#endif 

#ifndef PS_PTL_VER_PRE_R99
#define PS_PTL_VER_PRE_R99 (-2) 
#endif 

#ifndef PS_PTL_VER_R99
#define PS_PTL_VER_R99 (-1) 
#endif 

#ifndef PS_PTL_VER_R3
#define PS_PTL_VER_R3 0 
#endif 

#ifndef PS_PTL_VER_R4
#define PS_PTL_VER_R4 1 
#endif 

#ifndef PS_PTL_VER_R5
#define PS_PTL_VER_R5 2 
#endif 

#ifndef PS_PTL_VER_R6
#define PS_PTL_VER_R6 3 
#endif 

#ifndef PS_PTL_VER_R7
#define PS_PTL_VER_R7 4 
#endif 

#ifndef PS_PTL_VER_R8
#define PS_PTL_VER_R8 5 
#endif 

#ifndef PS_PTL_VER_R9
#define PS_PTL_VER_R9 6 
#endif 

#ifndef PS_UE_REL_VER
#define PS_UE_REL_VER PS_PTL_VER_R9 
#endif 

#ifndef FEATURE_UE_MODE_G
#define FEATURE_UE_MODE_G FEATURE_ON 
#endif 

#ifndef FEATURE_UE_MODE_W
#define FEATURE_UE_MODE_W FEATURE_ON 
#endif 

#ifndef FEATURE_CS
#define FEATURE_CS FEATURE_ON 
#endif 

#ifndef FEATURE_GFAX
#define FEATURE_GFAX FEATURE_OFF 
#endif 

#ifndef FEATURE_WFAX
#define FEATURE_WFAX FEATURE_OFF 
#endif 

#ifndef FEATURE_GCBS
#define FEATURE_GCBS FEATURE_ON 
#endif 

#ifndef FEATURE_WCBS
#define FEATURE_WCBS FEATURE_ON 
#endif 

#ifndef FEATURE_AP
#define FEATURE_AP FEATURE_OFF 
#endif 

#ifndef BALONG_UDP_V1R1
#define BALONG_UDP_V1R1 FEATURE_OFF 
#endif 

#ifndef BALONG_UDP_V1R2
#define BALONG_UDP_V1R2 FEATURE_OFF 
#endif 

#ifndef FEATURE_PTABLE_UDP
#define FEATURE_PTABLE_UDP FEATURE_ON 
#endif 

#ifndef FEATURE_STICK								
#define FEATURE_STICK								 FEATURE_OFF 
#endif 

#ifndef FEATURE_E5
#define FEATURE_E5 FEATURE_OFF 
#endif 

#ifndef FEATURE_E5_UDP
#define FEATURE_E5_UDP FEATURE_OFF 
#endif 

#ifndef FEATURE_PC_VOICE
#define FEATURE_PC_VOICE FEATURE_OFF 
#endif 

#ifndef FEATURE_HILINK
#define FEATURE_HILINK FEATURE_OFF 
#endif 

#ifndef FEATURE_SEC_BOOT
#define FEATURE_SEC_BOOT FEATURE_OFF 
#endif 

#ifndef FEATURE_LTE
#define FEATURE_LTE FEATURE_ON 
#endif 

#ifndef FEATURE_CSD
#define FEATURE_CSD FEATURE_OFF 
#endif 

#ifndef FEATURE_IPV6
#define FEATURE_IPV6 FEATURE_ON 
#endif 

#ifndef FEATURE_PPP
#define FEATURE_PPP FEATURE_ON 
#endif 

#ifndef FEATURE_PPPOE
#define FEATURE_PPPOE FEATURE_OFF 
#endif 

#ifndef FEATURE_VCOM_EXT
#define FEATURE_VCOM_EXT FEATURE_OFF 
#endif 

#ifndef FEATURE_MERGE_OM_CHAN						
#define FEATURE_MERGE_OM_CHAN						 FEATURE_OFF 
#endif 

#ifndef FEATURE_UPDATEONLINE
#define FEATURE_UPDATEONLINE FEATURE_OFF 
#endif 

#ifndef FEATURE_WIFI
#define FEATURE_WIFI FEATURE_OFF 
#endif 

#ifndef FEATURE_HIFI
#define FEATURE_HIFI FEATURE_OFF 
#endif 

#ifndef FEATURE_SDIO
#define FEATURE_SDIO FEATURE_OFF 
#endif 

#ifndef FEATURE_KEYBOARD
#define FEATURE_KEYBOARD FEATURE_OFF 
#endif 

#ifndef FEATURE_CHARGE
#define FEATURE_CHARGE FEATURE_OFF 
#endif 

#ifndef FEATURE_ICC_DEBUG
#define FEATURE_ICC_DEBUG FEATURE_ON 
#endif 

#ifndef FEATURE_POWER_ON_OFF
#define FEATURE_POWER_ON_OFF FEATURE_OFF 
#endif 

#ifndef FEATURE_OLED
#define FEATURE_OLED FEATURE_OFF 
#endif 

#ifndef FEATURE_TFT
#define FEATURE_TFT FEATURE_OFF 
#endif 

#ifndef FEATURE_MMI_TEST
#define FEATURE_MMI_TEST FEATURE_OFF 
#endif 

#ifndef FEATURE_DL_E_CELL_FACH
#define FEATURE_DL_E_CELL_FACH FEATURE_ON 
#endif 

#ifndef FEATURE_INTERPEAK
#define FEATURE_INTERPEAK FEATURE_OFF 
#endif 

#ifndef FEATURE_INTERPEAK_MINI
#define FEATURE_INTERPEAK_MINI FEATURE_OFF 
#endif 

#ifndef IPWEBS_FEATURE_E5
#define IPWEBS_FEATURE_E5 FEATURE_OFF 
#endif 

#ifndef IPWEBS_FEATURE_WIRELESS_DONGLE
#define IPWEBS_FEATURE_WIRELESS_DONGLE FEATURE_OFF 
#endif 

#ifndef IPWEBS_FEATURE_LIGHTNING_CARD
#define IPWEBS_FEATURE_LIGHTNING_CARD FEATURE_OFF 
#endif 

#ifndef FEATURE_WEBNAS
#define FEATURE_WEBNAS FEATURE_OFF 
#endif 

#ifndef FEATURE_WIRELESS_DONGLE
#define FEATURE_WIRELESS_DONGLE FEATURE_OFF 
#endif 

#ifndef FEATURE_PMU_PROTECT
#define FEATURE_PMU_PROTECT FEATURE_OFF 
#endif 

#ifndef FEATURE_VXWORKS_TCPIP
#define FEATURE_VXWORKS_TCPIP FEATURE_OFF 
#endif 

#ifndef FEATURE_M2
#define FEATURE_M2 FEATURE_OFF 
#endif 

#ifndef FEATURE_MEMORY_SIZE
#define FEATURE_MEMORY_SIZE MEMORY_SIZE_64M 
#endif 

#ifndef FEATURE_ECM_RNDIS
#define FEATURE_ECM_RNDIS FEATURE_OFF 
#endif 

#ifndef FEATURE_RNIC
#define FEATURE_RNIC FEATURE_OFF 
#endif 

#ifndef FEATURE_NFEXT
#define FEATURE_NFEXT FEATURE_OFF 
#endif 

#ifndef FEATURE_UL_E_CELL_FACH
#define FEATURE_UL_E_CELL_FACH FEATURE_ON 
#endif 

#ifndef FEATURE_BREATH_LIGHT
#define FEATURE_BREATH_LIGHT FEATURE_OFF 
#endif 

#ifndef FEATURE_LEDSTATUS
#define FEATURE_LEDSTATUS FEATURE_OFF 
#endif 

#ifndef FEATURE_E5_LED
#define FEATURE_E5_LED FEATURE_OFF 
#endif 

#ifndef FEATURE_RTC
#define FEATURE_RTC FEATURE_OFF 
#endif 

#ifndef FEATURE_SMALL_MAILBOX
#define FEATURE_SMALL_MAILBOX FEATURE_OFF 
#endif 

#ifndef FEATURE_RECONFIG
#define FEATURE_RECONFIG FEATURE_ON 
#endif 

#ifndef FEATURE_HWENCRY_REWORK
#define FEATURE_HWENCRY_REWORK FEATURE_OFF 
#endif 

#ifndef FEATURE_MMU_BIG
#define FEATURE_MMU_BIG FEATURE_OFF 
#endif 

#ifndef FEATURE_MMU_MEDIUM
#define FEATURE_MMU_MEDIUM FEATURE_ON 
#endif 

#ifndef FEATURE_MMU_SMALL
#define FEATURE_MMU_SMALL FEATURE_OFF 
#endif 

#ifndef FEATURE_TTFMEM_CACHE
#define FEATURE_TTFMEM_CACHE FEATURE_OFF 
#endif 

#ifndef FEATURE_SDUPDATE
#define FEATURE_SDUPDATE FEATURE_OFF 
#endif 

#ifndef FEATURE_SECURITY_SHELL          			
#define FEATURE_SECURITY_SHELL          			 FEATURE_ON 
#endif 

#ifndef FEATURE_EPAD
#define FEATURE_EPAD FEATURE_OFF 
#endif 

#ifndef FEATURE_HSIC_SLAVE
#define FEATURE_HSIC_SLAVE FEATURE_OFF 
#endif 

#ifndef FEATURE_MANUFACTURE_LOG
#define FEATURE_MANUFACTURE_LOG FEATURE_OFF 
#endif 

#ifndef FEATURE_MEM_MONITOR
#define FEATURE_MEM_MONITOR FEATURE_ON 
#endif 

#ifndef FEATURE_DRV_REPLAY_DUMP						
#define FEATURE_DRV_REPLAY_DUMP						 FEATURE_OFF 
#endif 

#ifndef FEATURE_SKB_EXP
#define FEATURE_SKB_EXP FEATURE_OFF 
#endif 

#ifndef FEATURE_MULTI_FS_PARTITION
#define FEATURE_MULTI_FS_PARTITION FEATURE_OFF 
#endif 

#ifndef FEATURE_MULTI_MODEM
#define FEATURE_MULTI_MODEM FEATURE_OFF 
#endif 

#ifndef FEATURE_SOCP_CHANNEL_REDUCE
#define FEATURE_SOCP_CHANNEL_REDUCE FEATURE_OFF 
#endif 

#ifndef FEATURE_SOCP_DECODE_INT_TIMEOUT
#define FEATURE_SOCP_DECODE_INT_TIMEOUT FEATURE_ON 
#endif 

#ifndef FEATURE_UE_MODE_TDS
#define FEATURE_UE_MODE_TDS FEATURE_ON 
#endif 

#ifndef FEATURE_ETWS
#define FEATURE_ETWS FEATURE_ON 
#endif 

#ifndef FEATURE_AGPS
#define FEATURE_AGPS FEATURE_OFF 
#endif 

#ifndef FEATRUE_XML_PARSER
#define FEATRUE_XML_PARSER FEATURE_OFF 
#endif 

#ifndef FEATURE_PTM
#define FEATURE_PTM FEATURE_ON 
#endif 

#ifndef CONFIG_HISI_PTM
#define CONFIG_HISI_PTM FEATURE_OFF 
#endif 

#ifndef FEATURE_COMPRESS_WRITE_LOG_FILE
#define FEATURE_COMPRESS_WRITE_LOG_FILE FEATURE_OFF 
#endif 

#ifndef FEATURE_CBT_LOG
#define FEATURE_CBT_LOG FEATURE_OFF 
#endif 

#ifndef FEATURE_IMS
#define FEATURE_IMS FEATURE_ON 
#endif 

#ifndef FEATURE_SAMPLE_LTE_CHAN 			
#define FEATURE_SAMPLE_LTE_CHAN 			 FEATURE_OFF 
#endif 

#ifndef FEATURE_AT_HSUART
#define FEATURE_AT_HSUART FEATURE_OFF 
#endif 

#ifndef NAS_FEATURE_SMS_NVIM_SMSEXIST
#define NAS_FEATURE_SMS_NVIM_SMSEXIST FEATURE_ON 
#endif 

#ifndef NAS_FEATURE_SMS_FLASH_SMSEXIST
#define NAS_FEATURE_SMS_FLASH_SMSEXIST FEATURE_ON 
#endif 

#ifndef FEATURE_AT_HSIC
#define FEATURE_AT_HSIC FEATURE_OFF 
#endif 

#ifndef FEATURE_RMNET_CUSTOM
#define FEATURE_RMNET_CUSTOM FEATURE_ON 
#endif 

#ifndef GAS_PTL_VER_PRE_R99
#define GAS_PTL_VER_PRE_R99 (PS_PTL_VER_PRE_R99) 
#endif 

#ifndef GAS_PTL_VER_R99
#define GAS_PTL_VER_R99 (PS_PTL_VER_R99) 
#endif 

#ifndef GAS_PTL_VER_R3
#define GAS_PTL_VER_R3 (PS_PTL_VER_R3) 
#endif 

#ifndef GAS_PTL_VER_R4
#define GAS_PTL_VER_R4 (PS_PTL_VER_R4) 
#endif 

#ifndef GAS_PTL_VER_R5
#define GAS_PTL_VER_R5 (PS_PTL_VER_R5) 
#endif 

#ifndef GAS_PTL_VER_R6
#define GAS_PTL_VER_R6 (PS_PTL_VER_R6) 
#endif 

#ifndef GAS_PTL_VER_R7
#define GAS_PTL_VER_R7 (PS_PTL_VER_R7) 
#endif 

#ifndef GAS_PTL_VER_R8
#define GAS_PTL_VER_R8 (PS_PTL_VER_R8) 
#endif 

#ifndef GAS_PTL_VER_R9
#define GAS_PTL_VER_R9 (PS_PTL_VER_R9) 
#endif 

#ifndef GAS_UE_REL_VER
#define GAS_UE_REL_VER (GAS_PTL_VER_R9) 
#endif 

#ifndef FEATURE_BBP_MASTER
#define FEATURE_BBP_MASTER FEATURE_ON 
#endif 

#ifndef FEATURE_BBPMST_FREQUENCY_REDUCE
#define FEATURE_BBPMST_FREQUENCY_REDUCE FEATURE_OFF 
#endif 

#ifndef TTF_CF_FEATURE
#define TTF_CF_FEATURE FEATURE_OFF 
#endif 

#ifndef FEATURE_TTF_MEM_DEBUG
#define FEATURE_TTF_MEM_DEBUG FEATURE_ON 
#endif 

#ifndef FEATURE_TTF_RLC_PIGGY_BACKED
#define FEATURE_TTF_RLC_PIGGY_BACKED FEATURE_OFF 
#endif 

#ifndef FEATURE_W_R99_DL_DUAL_MAILBOX
#define FEATURE_W_R99_DL_DUAL_MAILBOX FEATURE_ON 
#endif 

#ifndef FEATURE_ASSEMBLY_MEM_CPY
#define FEATURE_ASSEMBLY_MEM_CPY FEATURE_ON 
#endif 

#ifndef FEATURE_HSPA_PERF_IMP
#define FEATURE_HSPA_PERF_IMP FEATURE_ON 
#endif 

#ifndef FEATURE_RACH_NO_ACK_DEBUG
#define FEATURE_RACH_NO_ACK_DEBUG FEATURE_OFF 
#endif 

#ifndef FEATURE_RLC_REASSEMBLE
#define FEATURE_RLC_REASSEMBLE FEATURE_ON 
#endif 

#ifndef FEATURE_TCP_ACK_IN_FRONT
#define FEATURE_TCP_ACK_IN_FRONT FEATURE_ON 
#endif 

#ifndef FEATURE_CST_ASYN_OR_NOT_TRANS
#define FEATURE_CST_ASYN_OR_NOT_TRANS FEATURE_OFF 
#endif 

#ifndef FEATURE_CIPHER_MASTER
#define FEATURE_CIPHER_MASTER FEATURE_ON 
#endif 

#ifndef FEATURE_HDLC_ENHANCE
#define FEATURE_HDLC_ENHANCE FEATURE_ON 
#endif 

#ifndef FEATURE_ACPU_STAT
#define FEATURE_ACPU_STAT FEATURE_OFF 
#endif 

#ifndef FEATURE_ACPU_FC_POINT_REG
#define FEATURE_ACPU_FC_POINT_REG FEATURE_OFF 
#endif 

#ifndef SC_CTRL_MOD_6620_FPGA
#define SC_CTRL_MOD_6620_FPGA 1 
#endif 

#ifndef SC_CTRL_MOD_6620_SFT
#define SC_CTRL_MOD_6620_SFT 2 
#endif 

#ifndef SC_CTRL_MOD_6758_FPGA
#define SC_CTRL_MOD_6758_FPGA 3 
#endif 

#ifndef SC_CTRL_MOD_6758_SFT
#define SC_CTRL_MOD_6758_SFT 4 
#endif 

#ifndef SC_CTRL_MOD_6930_FPGA
#define SC_CTRL_MOD_6930_FPGA 5 
#endif 

#ifndef SC_CTRL_MOD_6930_SFT
#define SC_CTRL_MOD_6930_SFT 6 
#endif 

#ifndef SC_CTRL_MOD_3630_UDP
#define SC_CTRL_MOD_3630_UDP 7 
#endif 

#ifndef SC_CTRL_MOD
#define SC_CTRL_MOD (SC_CTRL_MOD_6930_SFT) 
#endif 

#ifndef FEATURE_EDMAC
#define FEATURE_EDMAC FEATURE_OFF 
#endif 

#ifndef ZSP_DSP_CHIP_BB_TYPE		
#define ZSP_DSP_CHIP_BB_TYPE		 7 
#endif 

#ifndef ZSP_DSP_PRODUCT_FORM		
#define ZSP_DSP_PRODUCT_FORM		 4 
#endif 

#ifndef BOARD
#define BOARD ASIC 
#endif 

#ifndef FEATURE_DC_DPA			
#define FEATURE_DC_DPA			 FEATURE_ON 
#endif 

#ifndef FEATURE_DC_UPA
#define FEATURE_DC_UPA FEATURE_OFF 
#endif 

#ifndef FEATURE_DC_MIMO
#define FEATURE_DC_MIMO FEATURE_OFF 
#endif 

#ifndef FEATURE_HARQ_OUT
#define FEATURE_HARQ_OUT FEATURE_OFF 
#endif 

#ifndef FEATURE_UPHY_SIO_WPLL_ELUSION_ON		
#define FEATURE_UPHY_SIO_WPLL_ELUSION_ON		 FEATURE_OFF 
#endif 

#ifndef FEATURE_RFIC_RESET_GPIO_ON			
#define FEATURE_RFIC_RESET_GPIO_ON			 FEATURE_OFF 
#endif 

#ifndef FEATURE_EXTERNAL_32K_CLK			
#define FEATURE_EXTERNAL_32K_CLK			 FEATURE_ON 
#endif 

#ifndef FEATURE_MULTI_RFIC_MEAS			
#define FEATURE_MULTI_RFIC_MEAS			 FEATURE_ON 
#endif 

#ifndef FEATURE_SRAM_400K
#define FEATURE_SRAM_400K FEATURE_OFF 
#endif 

#ifndef FEATURE_TEMP_DSP_CORE_POWER_DOWN
#define FEATURE_TEMP_DSP_CORE_POWER_DOWN FEATURE_OFF 
#endif 

#ifndef FEATURE_TUNER				
#define FEATURE_TUNER				 FEATURE_ON 
#endif 

#ifndef FEATURE_TEMP_MULTI_MODE_LP
#define FEATURE_TEMP_MULTI_MODE_LP FEATURE_ON 
#endif 

#ifndef FEATURE_VIRTUAL_BAND			
#define FEATURE_VIRTUAL_BAND			 FEATURE_OFF 
#endif 

#ifndef FEATURE_POWER_DRV
#define FEATURE_POWER_DRV FEATURE_OFF 
#endif 

#ifndef FEATURE_SIM_NOTIFY
#define FEATURE_SIM_NOTIFY FEATURE_OFF 
#endif 

#ifndef FEATURE_HW_CODEC
#define FEATURE_HW_CODEC FEATURE_ON 
#endif 

#ifndef FEATURE_VSIM
#define FEATURE_VSIM FEATURE_ON 
#endif 

#ifndef FEATURE_VOICE_UP
#define FEATURE_VOICE_UP FEATURE_OFF 
#endif 

#ifndef FEATURE_ANT_SHARE				
#define FEATURE_ANT_SHARE				 FEATURE_OFF 
#endif 

#ifndef FEATURE_DSDS
#define FEATURE_DSDS FEATURE_OFF 
#endif 

#ifndef FEATURE_SOCP_ON_DEMAND			
#define FEATURE_SOCP_ON_DEMAND			 FEATURE_OFF 
#endif 

#ifndef FEATURE_POWER_TIMER
#define FEATURE_POWER_TIMER FEATURE_ON 
#endif 

#ifndef FEATURE_CL_INTERWORK
#define FEATURE_CL_INTERWORK FEATURE_OFF 
#endif 

#ifndef FEATURE_BSP_LCH_OM
#define FEATURE_BSP_LCH_OM FEATURE_ON 
#endif 

#ifndef FEATURE_HISOCKET				
#define FEATURE_HISOCKET				 FEATURE_ON 
#endif 

#ifndef FEATURE_DFS_SYNC
#define FEATURE_DFS_SYNC FEATURE_ON 
#endif 

#ifndef FEATURE_GU_DSP_SPLIT
#define FEATURE_GU_DSP_SPLIT FEATURE_OFF 
#endif 

#ifndef FEATURE_MULTI_CHANNEL		
#define FEATURE_MULTI_CHANNEL		 FEATURE_OFF 
#endif 

#ifdef OBC_IMAGE_MODEM
#endif 

#ifdef OBC_IMAGE_APP
#endif 

#ifdef OBC_IMAGE_BOOTROM
#endif 

#ifdef OBC_IMAGE_BOOTLOADER
#endif 

#endif /*__PRODUCT_CONFIG_H__*/ 
