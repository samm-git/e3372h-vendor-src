/******************************************************************************

                   版权所有 (C), 2001-2011,华为终端有限公司

 ******************************************************************************
  文 件 名   : reb_boot.h
  版 本 号   : 初稿
  生成日期   : 2013年11月16日
  最近修改   :
  修改历史   :
  1.日    期   : 2013年11月16日
    修改内容   : 创建文件

******************************************************************************/
/******************************问题单修改记录**********************************
日期             修改人         问题单号                 修改内容
******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/

 #define REB_BOOT_DEBUG
 #define Reb_Platform_V7R2 (YES)
/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/
typedef struct 
{       
    unsigned char     FileSysActiveProtectEnable;   /* 文件系统主动保护使能 */     
    unsigned char     BadImgResumFromOnlineEnable;   /* 镜像损坏后从Online分区恢复使能*/     
    unsigned char     BootNotOperaFileSysEnable;      /*系统启动过程不对文件系统操作使能*/     
    unsigned char     FastOffFailTrueShutdownEnable;    /*假关机失败进入真关机功能使能*/      
    unsigned char     SoftPowOffReliableEnable;          /*软件关机可靠性功能使能*/      
    unsigned char     ZoneWriteProtectEnable;          /*分区写越界保护使能*/       
    unsigned char     BadZoneReWriteEnable;            /* Flash病危块回写功能使能*/       
    unsigned char     BootCanFromkernelBEnable;  /*主镜像破坏后可以从备份镜像启动使能*/
    unsigned char     OnlineChangeNotUpgradEnable; /*Online分区变化不进行在线升级使能*/
    /*预留*/
    unsigned char     BadZoneScanEnable;  /* Flash病危块扫描功能使能*/
    unsigned char     reserved2;
    unsigned char     reserved3;
    unsigned char     reserved4;
    unsigned char     reserved5;
    unsigned char     reserved6;
    unsigned char     MmcReliabilityEnable;            /*协议MMC可靠性保护使能开关*/
    unsigned int      MmcReliabilityBitmap;     /* 通信协议协议MMC可靠性保护功能掩码 */
    unsigned int      DangerTime;                   /*频繁上下电危险期经验值 ，单位为秒*/       
    unsigned int      WaitTime;           /*应用通知到底软假关机后，定时器值单位为秒*/
    /*预留*/
    int         reserved7; 
    int         reserved8; 
    int         reserved9; 
    int         reserved10; 
    int         reserved11;       
}SOFT_RELIABLE_CFG_STRU;
