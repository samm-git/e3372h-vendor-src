/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2013-2015. All rights reserved.
 *
 * mobile@huawei.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
 /*----------------------------------------------*
 * 宏定义                        *
 *----------------------------------------------*/
 /*假关机过放保护周期默认60分钟+10秒,以便与充电模块岔开*/
#define FAST_ON_OFF_CYCLE (3610 * 1000)

/*进入假关机后首次电池电压监测*/
#define FAST_ON_OFF_FST_CYCLE (20 * 1000)

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/
void reb_write_partition_beyond(const long long from, unsigned int* cnt_addr, 
    const char* part_name, unsigned int part_size );
void reb_sys_start_write_protect(char *name);
void reb_file_sync(void);
int reb_stop_write_file(void);
void reb_fast_onoff_inspect_callback(void * para);
/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/
typedef struct __SOFT_RELIABLE_CFG_STRU
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
    unsigned char     BadZoneScanEnable;   /* Flash病危块扫描功能使能*/    
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
/*----------------------------------------------*
 * 外部变量                                     *
 *----------------------------------------------*/
extern SOFT_RELIABLE_CFG_STRU g_kernel_soft_reliable_info;
