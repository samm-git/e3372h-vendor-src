/*************************************************************************
CAUTION : This file is Auto Generated by VBA based on 《V711 MBB MACRO_auto_generator.xlsm》.
          So, don't modify this file manually!
****************************************************************************/
#ifndef __MBB_CONFIG_H__
#define __MBB_CONFIG_H__


#ifdef __cplusplus
extern "C"
{
#endif


/******************************* MBB 基础宏定义 *******************************/
#ifndef FEATURE_ON
#define FEATURE_ON 1
#endif

#ifndef FEATURE_OFF
#define FEATURE_OFF 0
#endif

#ifndef YES
#define YES 1
#endif

#ifndef NO
#define NO 0
#endif

#define MBB_COMMON FEATURE_ON  /* 根宏：通用宏开关 */


/****************************************************************************/
/*************************** MBB 特性宏定义开始 *****************************/
/****************************************************************************/
/***************************** 一级宏定义 ***********************************/
#if (FEATURE_ON == MBB_COMMON)
#define    MBB_KEY                             FEATURE_ON         /* 一级宏：按键 */
#define    MBB_MLOG                            FEATURE_ON         /* 一级宏：mobile log */
#define    MBB_CHARGE                          FEATURE_ON         /* 一级宏：充电 */
#define    MBB_THERMAL_PROTECT                 FEATURE_ON         /* 一级宏：整机温度保护高级路由降速方案 */
#define    MBB_THERMAL_PROTECT_SAR             FEATURE_ON         /* 一级宏：整机温度保护降最大发射功率方案 */
#define    MBB_WIFI                            FEATURE_OFF        /* 一级宏：wifi */
#define    MBB_FAST_ON_OFF                     FEATURE_OFF        /* 一级宏：快速开关机 */
#define    MBB_WPG_COMMON                      FEATURE_ON         /* 一级宏：无线协议代码控制 */
#define    MBB_TOUCHSCREEN_MELFAS              FEATURE_OFF        /* 一级宏：touch驱动 */
#define    MBB_FEATURE_SD_POLLING              FEATURE_OFF        /* 一级宏：SD卡轮询检测 */
#define    MBB_FEATURE_PHY_NUM                 FEATURE_ON         /* 一级宏：硬件版本号适配 */
#define    MBB_REB                             FEATURE_ON         /* 一级宏：软件可靠性 */
#define    MBB_CTF_COMMON                      FEATURE_ON         /* 一级宏：CTF特性宏。由于BCM提供的ctf.ko中MBB_CTF_COMMON和MBB_CTF_WIFI_IPV6都是打开的，因此我们代码中这两个宏也应该同时打开或关闭 */
#define    MBB_FEATURE_CAT_MODULE_SET          FEATURE_OFF        /* 一级宏：单板CAT等级 */
#define    MBB_SIMLOCK_THREE                   FEATURE_ON         /* 一级宏：simlock 3.0 */
#define    MBB_USB                             FEATURE_ON         /* 一级宏：USB */
#define    MBB_DLOAD                           FEATURE_ON         /* 一级宏：升级 */
#define    MBB_LED                             FEATURE_ON         /* 一级宏：LED */
#define    MBB_LCD                             FEATURE_OFF        /* 一级宏：LCD */
#define    MBB_OLED                            FEATURE_OFF        /* 一级宏：OLED */
#define    MBB_SLIC                            FEATURE_OFF        /* 一级宏：SLIC特性宏 */
#define    MBB_FEATURE_32K_CLK_SRC             FEATURE_OFF        /* 一级宏：2K时钟源分频 */
#define    MBB_FEATURE_FASTIP                  FEATURE_OFF        /* 一级宏：FastIP */
#define    MBB_ANTENNA                         FEATURE_OFF        /* 一级宏：天线切换 */
#define    MBB_FEATURE_ETH                     FEATURE_OFF        /* 一级宏：ethernet */
#define    MBB_ECCSTATUS_OPTIMIZE              FEATURE_ON         /* 一级宏：ECC状态获取 文件系统和硬件自检功能需开启 */
#define    MBB_SOFT_POWER_OFF                  FEATURE_OFF        /* 一级宏：软关机功能宏 */
#define    MBB_HWTEST                          FEATURE_ON         /* 一级宏：硬件自检功能 */
#define    MBB_FEATURE_FILE_OPERATION          FEATURE_ON         /* 一级宏：MLOG文件读取工具宏 */
#endif

#if (FEATURE_OFF == MBB_COMMON)
#define    MBB_KEY                             FEATURE_OFF        /* 一级宏：按键 */
#define    MBB_MLOG                            FEATURE_OFF        /* 一级宏：mobile log */
#define    MBB_CHARGE                          FEATURE_OFF        /* 一级宏：充电 */
#define    MBB_THERMAL_PROTECT                 FEATURE_OFF        /* 一级宏：整机温度保护高级路由降速方案 */
#define    MBB_THERMAL_PROTECT_SAR             FEATURE_OFF        /* 一级宏：整机温度保护降最大发射功率方案 */
#define    MBB_WIFI                            FEATURE_OFF        /* 一级宏：wifi */
#define    MBB_FAST_ON_OFF                     FEATURE_OFF        /* 一级宏：快速开关机 */
#define    MBB_WPG_COMMON                      FEATURE_OFF        /* 一级宏：无线协议代码控制 */
#define    MBB_TOUCHSCREEN_MELFAS              FEATURE_OFF        /* 一级宏：touch驱动 */
#define    MBB_FEATURE_SD_POLLING              FEATURE_OFF        /* 一级宏：SD卡轮询检测 */
#define    MBB_FEATURE_PHY_NUM                 FEATURE_OFF        /* 一级宏：硬件版本号适配 */
#define    MBB_REB                             FEATURE_OFF        /* 一级宏：软件可靠性 */
#define    MBB_CTF_COMMON                      FEATURE_OFF        /* 一级宏：CTF特性宏。由于BCM提供的ctf.ko中MBB_CTF_COMMON和MBB_CTF_WIFI_IPV6都是打开的，因此我们代码中这两个宏也应该同时打开或关闭 */
#define    MBB_FEATURE_CAT_MODULE_SET          FEATURE_OFF        /* 一级宏：单板CAT等级 */
#define    MBB_SIMLOCK_THREE                   FEATURE_OFF        /* 一级宏：simlock 3.0 */
#define    MBB_USB                             FEATURE_OFF        /* 一级宏：USB */
#define    MBB_DLOAD                           FEATURE_OFF        /* 一级宏：升级 */
#define    MBB_LED                             FEATURE_OFF        /* 一级宏：LED */
#define    MBB_LCD                             FEATURE_OFF        /* 一级宏：LCD */
#define    MBB_OLED                            FEATURE_OFF        /* 一级宏：OLED */
#define    MBB_SLIC                            FEATURE_OFF        /* 一级宏：SLIC特性宏 */
#define    MBB_FEATURE_32K_CLK_SRC             FEATURE_OFF        /* 一级宏：2K时钟源分频 */
#define    MBB_FEATURE_FASTIP                  FEATURE_OFF        /* 一级宏：FastIP */
#define    MBB_ANTENNA                         FEATURE_OFF        /* 一级宏：天线切换 */
#define    MBB_FEATURE_ETH                     FEATURE_OFF        /* 一级宏：ethernet */
#define    MBB_ECCSTATUS_OPTIMIZE              FEATURE_OFF        /* 一级宏：ECC状态获取 文件系统和硬件自检功能需开启 */
#define    MBB_SOFT_POWER_OFF                  FEATURE_OFF        /* 一级宏：软关机功能宏 */
#define    MBB_HWTEST                          FEATURE_OFF        /* 一级宏：硬件自检功能 */
#define    MBB_FEATURE_FILE_OPERATION          FEATURE_OFF        /* 一级宏：MLOG文件读取工具宏 */
#endif


/********************************* 二级宏定义 *******************************/
#if (FEATURE_ON == MBB_KEY)
#define    MBB_POWER_KEY                       FEATURE_ON         /* 二级宏：power按键 */
#define    MBB_MENU_KEY                        FEATURE_ON         /* 二级宏：menu按键 */
#define    MBB_RESET_KEY                       FEATURE_ON         /* 二级宏：reset按键 */
#define    MBB_WLAN_KEY                        FEATURE_OFF        /* 二级宏：wlan按键 */
#endif

#if (FEATURE_OFF == MBB_KEY)
#define    MBB_POWER_KEY                       FEATURE_OFF        /* 二级宏：power按键 */
#define    MBB_MENU_KEY                        FEATURE_OFF        /* 二级宏：menu按键 */
#define    MBB_RESET_KEY                       FEATURE_OFF        /* 二级宏：reset按键 */
#define    MBB_WLAN_KEY                        FEATURE_OFF        /* 二级宏：wlan按键 */
#endif

#if (FEATURE_ON == MBB_CHARGE)
#define    MBB_CHG_PLATFORM_V7R2               FEATURE_ON         /* 二级宏：V7R2特性宏 */
#define    MBB_CHG_PLATFORM_9X25               FEATURE_OFF        /* 二级宏：9X25特性宏 */
#define    MBB_CHG_BAT_KNOCK_DOWN              FEATURE_ON         /* 二级宏：是否可拆卸电池,ON:是；OFF:不是；不定义：不带电池 */
#define    MBB_CHG_LCD                         FEATURE_OFF        /* 二级宏：LCD产品特性宏 */
#define    MBB_CHG_LED                         FEATURE_ON         /* 二级宏：LED产品特性宏 */
#define    MBB_CHG_OLED                        FEATURE_OFF        /* 二级宏：OLED产品特性宏 */
#define    MBB_CHG_TOUCH                       FEATURE_ON         /* 二级宏：TOHCH产品特性宏 */
#define    MBB_CHG_LINUX                       FEATURE_ON         /* 二级宏：LINUX系统特性宏 */
#define    MBB_CHG_VXWORKS                     FEATURE_OFF        /* 二级宏：LINUX系统特性宏 */
#define    MBB_CHG_REX                         FEATURE_OFF        /* 二级宏：REX系统特性宏 */
#define    MBB_CHG_EXTCHG                      FEATURE_OFF        /* 二级宏：对外充电特性宏 */
#define    MBB_CHG_WIRELESS                    FEATURE_OFF        /* 二级宏：无线充电特性宏 */
#define    MBB_CHG_MC2                         FEATURE_ON         /* 二级宏：MC2.0特性宏 */
#define    MBB_CHG_COMPENSATE                  FEATURE_ON         /* 二级宏：工厂补电特性宏 */
#define    MBB_CHG_BQ24196                     FEATURE_ON         /* 二级宏：BQ24196充电芯片特性宏 */
#define    MBB_CHG_BQ24161                     FEATURE_OFF        /* 二级宏：BQ24161充电芯片特性宏 */
#define    MBB_CHG_MAX8903                     FEATURE_OFF        /* 二级宏：MAX8903充电芯片特性宏 */
#define    MBB_CHG_SCHARGER                    FEATURE_OFF        /* 二级宏：scharger充电芯片特性宏 */
#define    MBB_CHG_WARM_CHARGE                 FEATURE_ON         /* 二级宏：高温充电特性宏 */
#define    MBB_CHG_MC                          FEATURE_ON         /* 二级宏：MC特性宏 */
#define    MBB_CHG_POWER_SUPPLY                FEATURE_ON         /* 二级宏：power supply特性宏 */
#define    MBB_CHG_COULOMETER                  FEATURE_OFF        /* 二级宏：库仑计特性宏 */
#define    MBB_CHG_BQ27510                     FEATURE_OFF        /* 二级宏：外置库仑计特性宏 */
#define    MBB_CHG_HIGH_VOLT_BATT              FEATURE_ON         /* 二级宏：高压电池特性宏 */
#define    MBB_CHG_BAT_CAPABILITY               3000              /* 二级宏：电池容量 */
#endif

#if (FEATURE_OFF == MBB_CHARGE)
#define    MBB_CHG_PLATFORM_V7R2               FEATURE_OFF        /* 二级宏：V7R2特性宏 */
#define    MBB_CHG_PLATFORM_9X25               FEATURE_OFF        /* 二级宏：9X25特性宏 */
#define    MBB_CHG_BAT_KNOCK_DOWN              FEATURE_OFF        /* 二级宏：是否可拆卸电池,ON:是；OFF:不是；不定义：不带电池 */
#define    MBB_CHG_LCD                         FEATURE_OFF        /* 二级宏：LCD产品特性宏 */
#define    MBB_CHG_LED                         FEATURE_OFF        /* 二级宏：LED产品特性宏 */
#define    MBB_CHG_OLED                        FEATURE_OFF        /* 二级宏：OLED产品特性宏 */
#define    MBB_CHG_TOUCH                       FEATURE_OFF        /* 二级宏：TOHCH产品特性宏 */
#define    MBB_CHG_LINUX                       FEATURE_OFF        /* 二级宏：LINUX系统特性宏 */
#define    MBB_CHG_VXWORKS                     FEATURE_OFF        /* 二级宏：LINUX系统特性宏 */
#define    MBB_CHG_REX                         FEATURE_OFF        /* 二级宏：REX系统特性宏 */
#define    MBB_CHG_EXTCHG                      FEATURE_OFF        /* 二级宏：对外充电特性宏 */
#define    MBB_CHG_WIRELESS                    FEATURE_OFF        /* 二级宏：无线充电特性宏 */
#define    MBB_CHG_MC2                         FEATURE_OFF        /* 二级宏：MC2.0特性宏 */
#define    MBB_CHG_COMPENSATE                  FEATURE_OFF        /* 二级宏：工厂补电特性宏 */
#define    MBB_CHG_BQ24196                     FEATURE_OFF        /* 二级宏：BQ24196充电芯片特性宏 */
#define    MBB_CHG_BQ24161                     FEATURE_OFF        /* 二级宏：BQ24161充电芯片特性宏 */
#define    MBB_CHG_MAX8903                     FEATURE_OFF        /* 二级宏：MAX8903充电芯片特性宏 */
#define    MBB_CHG_SCHARGER                    FEATURE_OFF        /* 二级宏：scharger充电芯片特性宏 */
#define    MBB_CHG_WARM_CHARGE                 FEATURE_OFF        /* 二级宏：高温充电特性宏 */
#define    MBB_CHG_MC                          FEATURE_OFF        /* 二级宏：MC特性宏 */
#define    MBB_CHG_POWER_SUPPLY                FEATURE_OFF        /* 二级宏：power supply特性宏 */
#define    MBB_CHG_COULOMETER                  FEATURE_OFF        /* 二级宏：库仑计特性宏 */
#define    MBB_CHG_BQ27510                     FEATURE_OFF        /* 二级宏：外置库仑计特性宏 */
#define    MBB_CHG_HIGH_VOLT_BATT              FEATURE_OFF        /* 二级宏：高压电池特性宏 */
#define    MBB_CHG_BAT_CAPABILITY              FEATURE_OFF        /* 二级宏：电池容量 */
#endif

#if (FEATURE_ON == MBB_WIFI)
#define    MBB_WIFI_CHIP1                      FEATURE_OFF           /* 二级宏：该宏控制编译哪款芯片.芯片目录命名规则为厂商+编号如：rtl8129, bcm43217,这是和对应的驱动目录名称要一样的。 */
#define    MBB_WIFI_CHIP2                      FEATURE_OFF        /* 二级宏：该宏控制编译哪款芯片.芯片目录命名规则为厂商+编号如：rtl8129, bcm43217,这是和对应的驱动目录名称要一样的。 */
#endif

#if (FEATURE_OFF == MBB_WIFI)
#define    MBB_WIFI_CHIP1                      FEATURE_OFF        /* 二级宏：该宏控制编译哪款芯片.芯片目录命名规则为厂商+编号如：rtl8129, bcm43217,这是和对应的驱动目录名称要一样的。 */
#define    MBB_WIFI_CHIP2                      FEATURE_OFF        /* 二级宏：该宏控制编译哪款芯片.芯片目录命名规则为厂商+编号如：rtl8129, bcm43217,这是和对应的驱动目录名称要一样的。 */
#endif

#if (FEATURE_ON == MBB_WPG_COMMON)
#define    MBB_FEATURE_UNI_PS_CALL             FEATURE_OFF        /* 二级宏：拨号 */
#define    MBB_FEATURE_BIP                     FEATURE_ON         /* 二级宏：BIP特性 */
#define    MBB_FEATURE_BIP_TEST                FEATURE_ON         /* 二级宏：BIP特性测试宏 */
#define    MBB_FEATURE_MPDP                    FEATURE_OFF        /* 二级宏：MPDP特性测试宏 */
#define    MBB_FEATURE_GATEWAY                 FEATURE_OFF        /* 二级宏：适配网关特性测试宏 */
#define    MBB_FEATURE_AT_CMD_FILTER           FEATURE_ON         /* 二级宏：屏蔽AT命令宏 */
#define    MBB_TELSTRA_CUSTOMSIZE              FEATURE_OFF        /* 二级宏：澳大利亚Telstra运营商定制 */
#define    MBB_PCVOICE                         FEATURE_OFF        /* 二级宏：PC语音 */
#endif

#if (FEATURE_OFF == MBB_WPG_COMMON)
#define    MBB_FEATURE_UNI_PS_CALL             FEATURE_OFF        /* 二级宏：拨号 */
#define    MBB_FEATURE_BIP                     FEATURE_OFF        /* 二级宏：BIP特性 */
#define    MBB_FEATURE_BIP_TEST                FEATURE_OFF        /* 二级宏：BIP特性测试宏 */
#define    MBB_FEATURE_MPDP                    FEATURE_OFF        /* 二级宏：MPDP特性测试宏 */
#define    MBB_FEATURE_GATEWAY                 FEATURE_OFF        /* 二级宏：适配网关特性测试宏 */
#define    MBB_FEATURE_AT_CMD_FILTER           FEATURE_OFF        /* 二级宏：屏蔽AT命令宏 */
#define    MBB_TELSTRA_CUSTOMSIZE              FEATURE_OFF        /* 二级宏：澳大利亚Telstra运营商定制 */
#define    MBB_PCVOICE                         FEATURE_OFF        /* 二级宏：PC语音 */
#endif

#if (FEATURE_ON == MBB_CTF_COMMON)
#define    MBB_CTF_WIFI_IPV6                   FEATURE_ON         /* 二级宏：CTF是否支持IPv6 */
#define    MBB_CTF_WAN_QUEUE                   FEATURE_ON         /* 二级宏：WAN */
#endif

#if (FEATURE_OFF == MBB_CTF_COMMON)
#define    MBB_CTF_WIFI_IPV6                   FEATURE_OFF        /* 二级宏：CTF是否支持IPv6 */
#define    MBB_CTF_WAN_QUEUE                   FEATURE_OFF        /* 二级宏：WAN */
#endif

#if (FEATURE_ON == MBB_USB)
#define    MBB_USB_MULTICONFIG                 FEATURE_OFF        /* 二级宏：USB */
#define    MBB_USB_SECURITY                    FEATURE_ON         /* 二级宏：USB */
#define    MBB_USB_SD                          FEATURE_OFF        /* 二级宏：USB */
#define    MBB_USB_CPE                         FEATURE_OFF        /* 二级宏：USB */
#define    MBB_USB_E5                          FEATURE_ON         /* 二级宏：USB */
#define    MBB_USB_HILINK                      FEATURE_OFF        /* 二级宏：USB */
#define    MBB_USB_STICK                       FEATURE_OFF        /* 二级宏：USB */
#define    MBB_USB_WINGLE                      FEATURE_OFF        /* 二级宏：USB */
#define    MBB_USB_CAPABILITY_THREE            FEATURE_OFF        /* 二级宏：USB */
#define    MBB_USB_CHARGE                      FEATURE_ON         /* 二级宏：USB */
#endif

#if (FEATURE_OFF == MBB_USB)
#define    MBB_USB_MULTICONFIG                 FEATURE_OFF        /* 二级宏：USB */
#define    MBB_USB_SECURITY                    FEATURE_OFF        /* 二级宏：USB */
#define    MBB_USB_SD                          FEATURE_OFF        /* 二级宏：USB */
#define    MBB_USB_CPE                         FEATURE_OFF        /* 二级宏：USB */
#define    MBB_USB_E5                          FEATURE_OFF        /* 二级宏：USB */
#define    MBB_USB_HILINK                      FEATURE_OFF        /* 二级宏：USB */
#define    MBB_USB_STICK                       FEATURE_OFF        /* 二级宏：USB */
#define    MBB_USB_WINGLE                      FEATURE_OFF        /* 二级宏：USB */
#define    MBB_USB_CAPABILITY_THREE            FEATURE_OFF        /* 二级宏：USB */
#define    MBB_USB_CHARGE                      FEATURE_OFF        /* 二级宏：USB */
#endif

#if (FEATURE_ON == MBB_DLOAD)
#define    MBB_DLOAD_BALONG                    FEATURE_ON         /* 二级宏：balong和高通差异 */
#define    MBB_DLOAD_LED                       FEATURE_ON         /* 二级宏：升级代码led闪灯 */
#define    MBB_DLOAD_LCD                       FEATURE_OFF        /* 二级宏：升级代码lcd显示 */
#define    MBB_DLOAD_BBOU                      FEATURE_ON         /* 二级宏：在线升级 */
#define    MBB_DLOAD_JFFS2                     FEATURE_ON         /* 二级宏：分区为jffs2文件系统(如webui分区） */
#define    MBB_DLOAD_SDUP                      FEATURE_ON         /* 二级宏：SD卡升级 */
#define    MBB_DLOAD_HILINK                    FEATURE_OFF        /* 二级宏：hilink升级 */
#define    MBB_DLOAD_WINGLE                    FEATURE_OFF        /* 二级宏：wingle升级 */
#define    MBB_DLOAD_SOLUTION                  FEATURE_OFF        /* 二级宏：solution升级 */
#define    MBB_DLOAD_STICK                     FEATURE_OFF        /* 二级宏：STICK升级 */
#define    MBB_DLOAD_MCPEUP                    FEATURE_OFF        /* 二级宏：AP+Modem形态CPE升级 */
#define    MBB_DLOAD_E5                        FEATURE_ON         /* 二级宏：E5升级 */
#define    MBB_DLOAD_AUTO_UPDATE               FEATURE_ON         /* 二级宏：自动恢复升级宏 */
#endif

#if (FEATURE_OFF == MBB_DLOAD)
#define    MBB_DLOAD_BALONG                    FEATURE_OFF        /* 二级宏：balong和高通差异 */
#define    MBB_DLOAD_LED                       FEATURE_OFF        /* 二级宏：升级代码led闪灯 */
#define    MBB_DLOAD_LCD                       FEATURE_OFF        /* 二级宏：升级代码lcd显示 */
#define    MBB_DLOAD_BBOU                      FEATURE_OFF        /* 二级宏：在线升级 */
#define    MBB_DLOAD_JFFS2                     FEATURE_OFF        /* 二级宏：分区为jffs2文件系统(如webui分区） */
#define    MBB_DLOAD_SDUP                      FEATURE_OFF        /* 二级宏：SD卡升级 */
#define    MBB_DLOAD_HILINK                    FEATURE_OFF        /* 二级宏：hilink升级 */
#define    MBB_DLOAD_WINGLE                    FEATURE_OFF        /* 二级宏：wingle升级 */
#define    MBB_DLOAD_SOLUTION                  FEATURE_OFF        /* 二级宏：solution升级 */
#define    MBB_DLOAD_STICK                     FEATURE_OFF        /* 二级宏：STICK升级 */
#define    MBB_DLOAD_MCPEUP                    FEATURE_OFF        /* 二级宏：AP+Modem形态CPE升级 */
#define    MBB_DLOAD_E5                        FEATURE_OFF        /* 二级宏：E5升级 */
#define    MBB_DLOAD_AUTO_UPDATE               FEATURE_OFF        /* 二级宏：自动恢复升级宏 */
#endif

#if (FEATURE_ON == MBB_LED)
#define    MBB_LED_DR                          FEATURE_ON         /* 二级宏：DR灯 */
#define    MBB_LED_GPIO                        FEATURE_ON         /* 二级宏：GPIO灯 */
#endif

#if (FEATURE_OFF == MBB_LED)
#define    MBB_LED_DR                          FEATURE_OFF        /* 二级宏：DR灯 */
#define    MBB_LED_GPIO                        FEATURE_OFF        /* 二级宏：GPIO灯 */
#endif

#if (FEATURE_ON == MBB_FEATURE_FASTIP)
#define    MBB_FEATURE_FASTIP_IPV6             FEATURE_ON         /* 二级宏：FastIP 对IPv6的支持 */
#define    MBB_FEATURE_FASTIP_WAN_QUEUE        FEATURE_ON         /* 二级宏：FastIP 使能WAN侧队列 */
#endif

#if (FEATURE_OFF == MBB_FEATURE_FASTIP)
#define    MBB_FEATURE_FASTIP_IPV6             FEATURE_OFF        /* 二级宏：FastIP 对IPv6的支持 */
#define    MBB_FEATURE_FASTIP_WAN_QUEUE        FEATURE_OFF        /* 二级宏：FastIP 使能WAN侧队列 */
#endif

#if (FEATURE_ON == MBB_FEATURE_ETH)
#define    MBB_FEATURE_ETH_PHY                 FEATURE_OFF        /* 二级宏：RGMII口接PHY芯片 */
#define    MBB_FEATURE_ETH_SWITCH              FEATURE_OFF        /* 二级宏：RGMII口接SWITCH芯片 */
#define    MBB_FEATURE_ETH_WAN_MIRROR          FEATURE_OFF        /* 二级宏：镜像抓包 */
#define    MBB_ETH_STMMAC_TX_THREAD            FEATURE_OFF        /* 二级宏：STMMAC的网口驱动增加fastip后，需打开此宏用线程和队列机制处理TX发包 */
#endif

#if (FEATURE_OFF == MBB_FEATURE_ETH)
#define    MBB_FEATURE_ETH_PHY                 FEATURE_OFF        /* 二级宏：RGMII口接PHY芯片 */
#define    MBB_FEATURE_ETH_SWITCH              FEATURE_OFF        /* 二级宏：RGMII口接SWITCH芯片 */
#define    MBB_FEATURE_ETH_WAN_MIRROR          FEATURE_OFF        /* 二级宏：镜像抓包 */
#define    MBB_ETH_STMMAC_TX_THREAD            FEATURE_OFF        /* 二级宏：STMMAC的网口驱动增加fastip后，需打开此宏用线程和队列机制处理TX发包 */
#endif



/****************************** 三级宏定义 *************************************/
#if (FEATURE_ON == MBB_TELSTRA_CUSTOMSIZE)
#define    MBB_T3402_TIMER_CTL                 FEATURE_ON         /* 三级宏：T3402超时后才能enable LTE */
#define    MBB_DIAL_REJ_50_51_CTL              FEATURE_ON         /* 三级宏：LTE注册网络以#50或#51拒绝，UE不应发起另一种类型的PDN承载 */
#endif

#if (FEATURE_OFF == MBB_TELSTRA_CUSTOMSIZE)
#define    MBB_T3402_TIMER_CTL                 FEATURE_OFF        /* 三级宏：T3402超时后才能enable LTE */
#define    MBB_DIAL_REJ_50_51_CTL              FEATURE_OFF        /* 三级宏：LTE注册网络以#50或#51拒绝，UE不应发起另一种类型的PDN承载 */
#endif



/******************************************************************************/
/***************************** MBB 特性宏定义结束 *****************************/
/******************************************************************************/


#ifdef __cplusplus
}
#endif
#endif /*__MBB_CONFIG_H__ */
