/***************************************************************************************************
  Copyright, 1988-2013, Huawei Tech. Co., Ltd.
  File name:      mbb_config.h
  Author:         daizhicheng 00183683
  Version:        V1.0 
  Date:           2013-05-24
  Description:    为了通过宏来隔离Hisi与MBB的代码
                  宏定义原则如下：
                  1.宏开关在打开与关闭的情况下能保证代码编译通过，并且能保证特性能真正打开与关闭
                  2.宏的命名请在各领域拉通，各领域统一管理，使用前统一申请
  Others: 
                  1：MBB定义一个根宏：MBB通用宏（MBB_COMMON）
                     根宏是最顶级的宏，关闭这两个宏，编译出来的就是纯基线代码
                  2：一级宏：包括特性宏（USB特性宏--MBB_USB，不同的特性有不同的特性宏）
                  3：二级宏：相当于一级特性宏的子特性宏（比如充电特性下会有无线充电与对外充电等子特性）
                  4：其它详细说明请仔细阅读《MBB产品宏管理规范》，如有不明之处，请联系Author
  Warning:        
                  1: 请勿使用除#if、#endif、#ifdef、#ifndef、#define之外的关键字定义或判断宏
                  2：请勿使用使用“与”、“或”以及#else，请使用#if实现多宏嵌套
                  3：请勿将define分成多行，必须写在一行内
******************************************************************************************************/
/******************************************************************************************************
    问题单号                       时间                   修改人                        说明
DTS2013060607539               2013-06-06          daizhicheng 00183683        增加mbb_config.h头文件
DTS2013070203191               2013-07-02          xuchao 00202188             增加头文件警告性限制说明
*****************************************************************************************************/
#ifndef __MBB_CONFIG_H__
#define __MBB_CONFIG_H__

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef FEATURE_ON
#define FEATURE_ON 1
#endif
#ifndef FEATURE_OFF
#define FEATURE_OFF 0
#endif

#define MBB_COMMON FEATURE_ON    /* MBB通用宏 */

/***********************宏定义开始******************************/
/*******************************************************************
                        一级宏定义
********************************************************************/
#if (FEATURE_ON == MBB_COMMON)
#define    MBB_KEY                      FEATURE_ON      /* 一级宏：按键 */
#define    MBB_MLOG                     FEATURE_ON      /* 一级宏：MobileLog(仅C核使用) */
#define    MBB_CHARGE                   FEATURE_ON      /* 一级宏：充电 */
#define    MBB_THERMAL_PROTECT          FEATURE_ON      /* 一级宏：温度保护 */
#define    MBB_WIFI                     FEATURE_ON      /* 一级宏：WiFi */
#define    MBB_FAST_ON_OFF              FEATURE_OFF     /* 一级宏：快速开关机 */
#define    MBB_WPG_COMMON               FEATURE_ON      /* 一级宏：无线协议代码控制宏 */
#define    MBB_TOUCHSCREEN_MELFAS       FEATURE_OFF  	/* 一级宏：touch驱动 */
#define    MBB_FEATURE_SD_POLLING       FEATURE_OFF     /* 一级宏：SD卡轮询检测宏 */
#define    MBB_FEATURE_PHY_NUM          FEATURE_ON      /* 一级宏：硬件版本号适配 */
#define    MBB_REB                      FEATURE_ON      /* 一级宏：软件可靠性 */
#define    MBB_CTF_COMMON               FEATURE_ON     /* 一级宏：CTF 暂时关闭，调试通过后打开 */
#define    MBB_FEATURE_CAT_MODULE_SET   FEATURE_OFF     /* 一级宏：单板CAT等级 */
#define    MBB_SIMLOCK_THREE            FEATURE_ON      /* 一级宏：simlock 3.0 宏 */
#define    MBB_DLOAD                    FEATURE_ON      /*一级宏：升级 */
#define    MBB_LED                      FEATURE_OFF      /* 一级宏 :LED*/
#define    MBB_LCD                      FEATURE_OFF      /* 一级宏 :LCD*/
#define    MBB_OLED                     FEATURE_ON      /* 一级宏 :0.96 OLED*/
#define    MBB_FEATURE_32K_CLK_SRC      FEATURE_OFF     /* 一级宏：32K时钟源分频 */
#define    MBB_FEATURE_FASTIP           FEATURE_OFF     /* 一级宏：FastIP */
#define    MBB_ANTENNA                  FEATURE_OFF     /* 一级宏：天线切换 */
#define    MBB_SLIC	                    FEATURE_OFF     /* 一级宏：定义slic驱动一级宏 */
#define    MBB_FEATURE_ETH		      	FEATURE_OFF     /* 一级宏：ethernet */
#endif/* MBB_COMMON == FEATURE_ON */

#if (FEATURE_OFF == MBB_COMMON)
#define    MBB_KEY                      FEATURE_OFF     /* 一级宏：按键 */
#define    MBB_MLOG                     FEATURE_OFF     /* 一级宏：MobileLog(仅C核使用) */
#define    MBB_CHARGE                   FEATURE_OFF     /* 一级宏：充电 */
#define    MBB_THERMAL_PROTECT          FEATURE_OFF     /* 一级宏：温度保护 */
#define    MBB_WIFI                     FEATURE_OFF     /* 一级宏：WiFi */
#define    MBB_FAST_ON_OFF              FEATURE_ON      /* 一级宏：快速开关机 */
#define    MBB_WPG_COMMON               FEATURE_OFF     /* 一级宏：无线协议代码控制宏 */
#define    MBB_TOUCHSCREEN_MELFAS       FEATURE_OFF  	/* 一级宏：touch驱动 */
#define    MBB_FEATURE_SD_POLLING       FEATURE_OFF     /* 一级宏：SD卡轮询检测宏 */
#define    MBB_FEATURE_PHY_NUM          FEATURE_OFF     /* 一级宏：硬件版本号适配 */
#define    MBB_REB                      FEATURE_OFF     /* 一级宏：软件可靠性 */
#define    MBB_CTF_COMMON               FEATURE_OFF     /* 一级宏：CTF */
#define    MBB_FEATURE_CAT_MODULE_SET   FEATURE_OFF     /* 一级宏：单板CAT等级 */
#define    MBB_SIMLOCK_THREE            FEATURE_OFF     /* 一级宏：simlock 3.0 宏 */
#define    MBB_DLOAD                    FEATURE_OFF     /*一级宏：升级 */
#define    MBB_LED                      FEATURE_OFF     /* 一级宏 :LED*/
#define    MBB_FEATURE_32K_CLK_SRC      FEATURE_OFF     /* 一级宏：32K时钟源分频 */
#define    MBB_LCD                      FEATURE_OFF      /* 一级宏 :LCD*/
#define    MBB_OLED                     FEATURE_OFF      /* 一级宏 :0.96 OLED*/
#define    MBB_FEATURE_FASTIP           FEATURE_OFF     /* 一级宏：FastIP */
#define    MBB_ANTENNA                  FEATURE_OFF     /* 一级宏：天线切换 */
#define    MBB_SLIC	                    FEATURE_OFF     /* 一级宏：定义slic驱动一级宏 */
#define    MBB_FEATURE_ETH		      	FEATURE_OFF     /* 一级宏：ethernet */
#endif/* MBB_COMMON == FEATURE_OFF */

/*******************************************************************
                        二级宏定义
********************************************************************/
#if (FEATURE_ON == MBB_KEY)
#define    MBB_POWER_KEY                FEATURE_ON      /* 二级宏：power按键 */
#define    MBB_MENU_KEY                 FEATURE_OFF     /* 二级宏：menu按键 */
#define    MBB_RESET_KEY                FEATURE_ON      /* 二级宏：reset按键 */
#define    MBB_WLAN_KEY                 FEATURE_OFF     /* 二级宏：wlan按键 */
#endif

#if (FEATURE_OFF == MBB_KEY)
#define    MBB_POWER_KEY                FEATURE_OFF     /* 二级宏：power按键 */
#define    MBB_MENU_KEY                 FEATURE_OFF     /* 二级宏：menu按键 */
#define    MBB_RESET_KEY                FEATURE_OFF     /* 二级宏：reset按键 */
#define    MBB_WLAN_KEY                 FEATURE_OFF     /* 二级宏：wlan按键 */
#endif


#if (MBB_CHARGE == FEATURE_ON)
#define    MBB_CHG_PLATFORM_V7R2        FEATURE_ON      /* 二级宏：V7R2特性宏 */
#define    MBB_CHG_PLATFORM_9X25        FEATURE_OFF     /* 二级宏：9X25特性宏 */
#define    MBB_CHG_BAT_KNOCK_DOWN       FEATURE_OFF     /* 二级宏：是否可拆卸电池,FEATURE_ON:是；FEATURE_OFF:不是；不定义：不带电池 */
#define    MBB_CHG_LCD                  FEATURE_OFF     /* 二级宏：LCD产品特性宏 */
#define    MBB_CHG_LED                  FEATURE_OFF     /* 二级宏：LED产品特性宏 */
#define    MBB_CHG_OLED                 FEATURE_ON      /* 二级宏：OLED产品特性宏 */
#define    MBB_CHG_TOUCH                FEATURE_OFF      /* 二级宏：TOHCH产品特性宏 */
#define    MBB_CHG_LINUX                FEATURE_ON      /* 二级宏：LINUX系统特性宏 */
#define    MBB_CHG_VXWORKS              FEATURE_OFF     /* 二级宏：LINUX系统特性宏 */
#define    MBB_CHG_REX                  FEATURE_OFF     /* 二级宏：REX系统特性宏 */
#define    MBB_CHG_EXTCHG               FEATURE_OFF      /* 二级宏：对外充电特性宏 */
#define    MBB_CHG_WIRELESS             FEATURE_OFF      /* 二级宏：无线充电特性宏 */
#define    MBB_CHG_MC2                  FEATURE_ON      /* 二级宏：MC2.0特性宏 */
#define    MBB_CHG_COMPENSATE           FEATURE_ON      /* 二级宏：工厂补电特性宏 */
#define    MBB_CHG_BQ24196              FEATURE_ON      /* 二级宏：BQ24196充电芯片特性宏 */
#define    MBB_CHG_BQ24161              FEATURE_OFF     /* 二级宏：BQ24161充电芯片特性宏 */
#define    MBB_CHG_MAX8903              FEATURE_OFF     /* 二级宏：MAX8903充电芯片特性宏 */
#define    MBB_CHG_SCHARGER             FEATURE_OFF     /* 二级宏：scharger充电芯片特性宏 */
#define    MBB_CHG_WARM_CHARGE          FEATURE_ON      /* 二级宏：高温充电特性宏 */
#define    MBB_CHG_MC                   FEATURE_ON      /* 二级宏：MC2.0特性宏 */
#define    MBB_CHG_POWER_SUPPLY         FEATURE_ON      /* 二级宏：power supply特性宏 */
#define    MBB_CHG_COULOMETER           FEATURE_OFF     /* 二级宏：库仑计特性宏 */
#define    MBB_CHG_BQ27510              FEATURE_OFF     /* 二级宏：外置库仑计特性宏 */
#define    MBB_CHG_HIGH_VOLT_BATT       FEATURE_OFF     /* 二级宏：高压电池特性宏 */
#define    MBB_CHG_BAT_CAPABILITY       (3000)          /* 二级宏：电池容量 */
#endif

#if (MBB_CHARGE == FEATURE_OFF)
#define    MBB_CHG_PLATFORM_V7R2        FEATURE_OFF    /* 二级宏：V7R2特性宏 */
#define    MBB_CHG_PLATFORM_9X25        FEATURE_OFF    /* 二级宏：9X25特性宏 */
#define    MBB_CHG_BAT_KNOCK_DOWN       FEATURE_OFF    /* 二级宏：是否可拆卸电池,FEATURE_ON:是；FEATURE_OFF:不是；不定义：不带电池 */
#define    MBB_CHG_LCD                  FEATURE_OFF    /* 二级宏：LCD产品特性宏 */
#define    MBB_CHG_LED                  FEATURE_OFF    /* 二级宏：LED产品特性宏 */
#define    MBB_CHG_OLED                 FEATURE_OFF      /* 二级宏：OLED产品特性宏 */
#define    MBB_CHG_TOUCH                FEATURE_OFF    /* 二级宏：TOHCH产品特性宏 */
#define    MBB_CHG_LINUX                FEATURE_OFF    /* 二级宏：LINUX系统特性宏 */
#define    MBB_CHG_VXWORKS              FEATURE_OFF    /* 二级宏：LINUX系统特性宏 */
#define    MBB_CHG_REX                  FEATURE_OFF    /* 二级宏：REX系统特性宏 */
#define    MBB_CHG_EXTCHG               FEATURE_OFF    /* 二级宏：对外充电特性宏 */
#define    MBB_CHG_WIRELESS             FEATURE_OFF    /* 二级宏：无线充电特性宏 */
#define    MBB_CHG_MC2                  FEATURE_OFF    /* 二级宏：MC2.0特性宏 */
#define    MBB_CHG_COMPENSATE           FEATURE_OFF    /* 二级宏：工厂补电特性宏*/
#define    MBB_CHG_BQ24196              FEATURE_OFF    /* 二级宏：BQ24196充电芯片特性宏 */
#define    MBB_CHG_BQ24161              FEATURE_OFF    /* 二级宏：BQ24161充电芯片特性宏 */
#define    MBB_CHG_MAX8903              FEATURE_OFF    /* 二级宏：MAX8903充电芯片特性宏 */
#define    MBB_CHG_SCHARGER             FEATURE_OFF    /* 二级宏：scharger充电芯片特性宏 */
#define    MBB_CHG_WARM_CHARGE          FEATURE_OFF    /* 二级宏：高温充电特性宏 */
#define    MBB_CHG_MC                   FEATURE_OFF    /* 二级宏：MC2.0特性宏 */
#define    MBB_CHG_POWER_SUPPLY         FEATURE_OFF    /* 二级宏：power supply特性宏 */
#define    MBB_CHG_COULOMETER           FEATURE_OFF    /* 二级宏：库仑计特性宏 */
#define    MBB_CHG_BQ27510              FEATURE_OFF    /* 二级宏：外置库仑计特性宏 */
#define    MBB_CHG_HIGH_VOLT_BATT       FEATURE_OFF    /* 二级宏：高压电池特性宏 */
#define    MBB_CHG_BAT_CAPABILITY       (3000)         /* 二级宏：电池容量 */
#endif


/* WiFi二级宏:该宏控制编译哪款芯片
 * 芯片目录命名规则为厂商+编号如：rtl8129, bcm43217,这是和对应的驱动目录名称要一样的。
 */
#if (FEATURE_ON == MBB_WIFI)
#define    MBB_WIFI_CHIP1               bcm43241
#define    MBB_WIFI_CHIP2               FEATURE_OFF
#endif

#if (FEATURE_OFF == MBB_WIFI)
#define    MBB_WIFI_CHIP1               FEATURE_OFF
#define    MBB_WIFI_CHIP2               FEATURE_OFF
#endif

/*无线协议模块宏定义开始*/
#if (FEATURE_ON == MBB_WPG_COMMON)
#define    MBB_FEATURE_UNI_PS_CALL      FEATURE_OFF    /* 二级宏：拨号 */
#define    MBB_FEATURE_BIP              FEATURE_OFF    /* 二级宏：BIP特性 */
#define    MBB_FEATURE_BIP_TEST         FEATURE_OFF    /* 二级宏：BIP特性测试宏 */
#define    MBB_FEATURE_MPDP             FEATURE_OFF    /* 二级宏：MPDP特性测试宏 */
#define    MBB_FEATURE_GATEWAY          FEATURE_OFF    /* 二级宏：适配网关特性测试宏 */
#define    MBB_FEATURE_AT_CMD_FILTER    FEATURE_ON     /* 二级宏：屏蔽AT命令宏 */
#endif

#if (FEATURE_OFF == MBB_WPG_COMMON)
#define    MBB_FEATURE_UNI_PS_CALL      FEATURE_OFF    /* 二级宏：拨号 */
#define    MBB_FEATURE_BIP              FEATURE_OFF    /* 二级宏：BIP特性 */
#define    MBB_FEATURE_BIP_TEST         FEATURE_OFF    /* 二级宏：BIP特性测试宏 */
#define    MBB_FEATURE_MPDP             FEATURE_OFF    /* 二级宏：MPDP特性测试宏 */
#define    MBB_FEATURE_GATEWAY          FEATURE_OFF    /* 二级宏：适配网关特性测试宏 */
#define    MBB_FEATURE_AT_CMD_FILTER    FEATURE_OFF    /* 二级宏：屏蔽AT命令宏 */
#endif
/*无线协议模块宏定义结束*/

#if (MBB_CTF_COMMON == FEATURE_ON)
/*由于BCM提供的ctf.ko中MBB_CTF_COMMON和MBB_CTF_WIFI_IPV6都是打开的，因此我们代码中这两个宏也应该同时打开或关闭*/
#define    MBB_CTF_WIFI_IPV6            FEATURE_ON     /* 二级宏：CTF是否支持IPv6 */
#define    MBB_CTF_WAN_QUEUE            FEATURE_ON
#endif /* MBB_CTF_COMMON == FEATURE_ON */

#if (MBB_CTF_COMMON == FEATURE_OFF)
/*由于BCM提供的ctf.ko中MBB_CTF_COMMON和MBB_CTF_WIFI_IPV6都是打开的，因此我们代码中这两个宏也应该同时打开或关闭*/
#define    MBB_CTF_WIFI_IPV6            FEATURE_OFF    /* 二级宏：CTF是否支持IPv6 */
#define    MBB_CTF_WAN_QUEUE            FEATURE_OFF
#endif /* MBB_CTF_COMMON == FEATURE_OFF */

#if (FEATURE_ON == MBB_DLOAD)
#define MBB_DLOAD_BALONG     FEATURE_ON/*二级宏：区分balong和高通的差异*/
#define MBB_DLOAD_LED       FEATURE_OFF      /*二级宏，升级代码led闪灯*/
#define MBB_DLOAD_LCD       FEATURE_OFF     /*二级宏，升级代码lcd显示所用 */
#define MBB_DLOAD_BBOU       FEATURE_ON/*二级宏：在线升级*/
#define MBB_DLOAD_SDUP      FEATURE_OFF    /*二级宏：SD卡升级*/
#define MBB_DLOAD_MCPEUP    FEATURE_OFF      /*二级宏: AP+Modem形态CPE升级*/
#define MBB_DLOAD_HILINK    FEATURE_OFF     /*二级宏: hilink升级*/
#define MBB_DLOAD_SOLUTION  FEATURE_OFF     /*二级宏: solution升级*/
#define MBB_DLOAD_STICK     FEATURE_OFF     /*二级宏: STICK升级*/
#define MBB_DLOAD_WINGLE    FEATURE_OFF     /*二级宏: WINGLE升级*/
#define MBB_DLOAD_E5        FEATURE_OFF     /*二级宏:  E5升级*/
#define MBB_DLOAD_JFFS2     FEATURE_ON     /*二级宏: 分区为jffs2文件系统(如webui分区）*/
#endif

#if (FEATURE_OFF == MBB_DLOAD)
#define MBB_DLOAD_BALONG    FEATURE_OFF     /*二级宏：balong和高通差异*/
#define MBB_DLOAD_LED       FEATURE_OFF     /*二级宏，升级代码led闪灯*/
#define MBB_DLOAD_LCD       FEATURE_OFF     /*二级宏，升级代码lcd显示所用 */
#define MBB_DLOAD_BBOU      FEATURE_OFF     /*二级宏: 在线升级*/
#define MBB_DLOAD_SDUP      FEATURE_OFF     /*二级宏：SD卡升级*/
#define MBB_DLOAD_MCPEUP    FEATURE_OFF     /*二级宏: AP+Modem形态CPE升级*/
#define MBB_DLOAD_HILINK    FEATURE_OFF     /*二级宏: hilink升级*/
#define MBB_DLOAD_SOLUTION  FEATURE_OFF     /*二级宏: solution升级*/
#define MBB_DLOAD_STICK     FEATURE_OFF     /*二级宏: STICK升级*/
#define MBB_DLOAD_WINGLE    FEATURE_OFF     /*二级宏: WINGLE升级*/
#define MBB_DLOAD_E5        FEATURE_OFF     /*二级宏: E5升级*/
#define MBB_DLOAD_JFFS2     FEATURE_OFF     /*二级宏: 分区为jffs2文件系统(如webui分区）*/
#endif

#if (FEATURE_ON == MBB_LED)
#define    MBB_LED_DR                   FEATURE_ON       /*二级宏：DR灯*/
#define    MBB_LED_GPIO                 FEATURE_ON      /*二级宏：GPIO灯*/
#endif
#if (FEATURE_OFF == MBB_LED)
#define    MBB_LED_DR                   FEATURE_OFF     /*二级宏：DR灯*/
#define    MBB_LED_GPIO                 FEATURE_OFF     /*二级宏：GPIO灯*/
#endif                      
#if (FEATURE_ON == MBB_FEATURE_ETH)
#define    MBB_FEATURE_ETH_PHY      	FEATURE_OFF     /* 二级宏：RGMII口接PHY芯片 */
#define    MBB_FEATURE_ETH_SWITCH      	FEATURE_OFF     /* 二级宏：RGMII口接SWITCH芯片 */
#endif
#if (FEATURE_OFF == MBB_FEATURE_ETH)
#define    MBB_FEATURE_ETH_PHY      	FEATURE_OFF     /* 二级宏：RGMII口接PHY芯片 */
#define    MBB_FEATURE_ETH_SWITCH      	FEATURE_OFF     /* 二级宏：RGMII口接SWITCH芯片 */
#endif
#if (FEATURE_ON == MBB_FEATURE_FASTIP)
#define    MBB_FEATURE_FASTIP_IPV6             FEATURE_ON  /* 二级宏：FastIP 对IPv6的支持 */
#define    MBB_FEATURE_FASTIP_WAN_QUEUE        FEATURE_ON  /* 二级宏：FastIP 使能WAN侧队列 */
#endif

#if (FEATURE_OFF == MBB_FEATURE_FASTIP)
#define    MBB_FEATURE_FASTIP_IPV6             FEATURE_OFF /* 二级宏：FastIP 对IPv6的支持 */
#define    MBB_FEATURE_FASTIP_WAN_QUEUE        FEATURE_OFF /* 二级宏：FastIP 使能WAN侧队列 */
#endif
/*******************************************************************
                        三级宏定义
********************************************************************/

/***********************宏定义完毕******************************/

/*BEGIN DTS2013072504345  l00212897 2013/07/25 modified*/
#ifndef YES
#define YES  1
#endif
#ifndef NO
#define NO   0
#endif
/*END DTS2013072504345  l00212897 2013/07/25 modified*/

/**************************************MBB_COMMON == FEATURE_ON****************************************************/
#if (MBB_COMMON == FEATURE_ON)         /* 根宏：根宏已经定义 */ 
/*******************************************************************/



/* BEGIN: Add for PN: DTS2013070304541  by h00122846,2013/7/4 */
/*******************************************************************
           USB模块，新加宏定义者请按照本文件风格统一添加
*******************************************************************/
/*******************************************************************
                        一级宏定义
********************************************************************/
#if (FEATURE_ON == MBB_COMMON)
#define MBB_USB            FEATURE_ON      
#endif

#if (FEATURE_OFF == MBB_COMMON)
#define MBB_USB            FEATURE_OFF     
#endif

/*******************************************************************
                        二级宏定义
********************************************************************/
#if (FEATURE_ON == MBB_USB)
#define MBB_USB_MULTICONFIG         FEATURE_OFF
#define MBB_USB_SECURITY            FEATURE_ON
#define MBB_USB_SD                  FEATURE_ON

#define MBB_USB_CPE                 FEATURE_OFF
#define MBB_USB_E5                  FEATURE_ON
#define MBB_USB_HILINK              FEATURE_OFF
#define MBB_USB_STICK               FEATURE_OFF
#define MBB_USB_WINGLE              FEATURE_OFF

#define MBB_USB_CAPABILITY_THREE    FEATURE_OFF
#define MBB_USB_CHARGE              FEATURE_ON
#endif

#if (FEATURE_OFF == MBB_USB)
#define MBB_USB_MULTICONFIG         FEATURE_OFF
#define MBB_USB_SECURITY            FEATURE_OFF
#define MBB_USB_SD                  FEATURE_OFF

#define MBB_USB_CPE                 FEATURE_OFF
#define MBB_USB_E5                  FEATURE_OFF
#define MBB_USB_HILINK              FEATURE_OFF
#define MBB_USB_STICK               FEATURE_OFF
#define MBB_USB_WINGLE              FEATURE_OFF

#define MBB_USB_CAPABILITY_THREE    FEATURE_OFF
#define MBB_USB_CHARGE              FEATURE_OFF
#endif



#endif /* MBB_COMMON == FEATURE_ON */
#ifdef __cplusplus
}
#endif

#endif /*__MBB_CONFIG_H__ */
