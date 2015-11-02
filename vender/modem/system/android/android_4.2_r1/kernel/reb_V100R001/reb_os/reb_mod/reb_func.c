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
/******************************问题单修改记录**********************************
    日期          修改人             问题单号                    修改内容
******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "../reb_config.h"
#include <linux/sched.h>
#include <linux/syscalls.h>
#include <linux/export.h>
#include <linux/notifier.h>
#include "../os_adapter/reb_os_adapter.h"
#include "reb_func.h"
#if ( YES == Reb_Platform_9x25)
#include "../platform_adapter/9x25/reb_9x25_platform.h"
#endif
#if ( YES == Reb_Platform_V7R2)
#include "../platform_adapter/v7r2/reb_v7r2_platform.h"
#include "../../../../../../include/drv/drv_fastOnOff.h"
#endif
/*----------------------------------------------*
 * 宏定义                                       *
*----------------------------------------------*/
/*4.0V~4.2V之间电量太少,从4.0V开始跟踪*/
#define VOLT_INSPECT_UPPER_LIMIT (4000)

/*低于假关机低电关机门限10个小时*/
#define UPPER_LIMIT_TIME (10 * 3600 * 1000)

/*假关机下检测电池电压的最大次数*/
#define VOLT_INSPECT_MAX_TIME (10)

/*电池低电电压值*/
#define LOW_POWER_BAT_LVL (3550)

/*假关机下检测电池电压的最小刻度:56mV,以对应3000mAh电池
3.7V-3.6V之间的50mA*/
#define VOLT_INSPECT_VOLT_DELTA_UNIT (56)

/*假关机下检测电池电压基准电池容量 3000mAh*/
#define VOLT_INSPECT_BASE_VOLT_CAPABILITY (3000)
/*----------------------------------------------*
 * 内部类型说明                                 *
 *----------------------------------------------*/
typedef struct __volt_and_elec
{
    int volt_upside; /*电池点*/
    int elect;       /*当前组的电池点到下一个组的
                      电池点需要耗的电量,单位mAh*/
}volt_and_elec;

typedef enum
{
    INVALID_VOLT = -1,       /*无效电池电压,继续检测*/
    BAT_OVER_USE = 0,        /*电池过放,需要关机*/
    CONTINUE_INSPECT = 1,    /*继续检测*/
    STOP_INSPECT = 2,        /*停止检测*/
}BAT_VOLT_STA;

/*----------------------------------------------*
 * 外部函数原型声明                             *
 *----------------------------------------------*/
#if (FEATURE_ON == MBB_FAST_ON_OFF)
#if (MBB_CHARGE == FEATURE_ON)
extern int chg_get_batt_volt_value(void);
extern unsigned int chg_is_bat_only(void);
extern int get_low_bat_level(void);
#endif
#endif

extern reb_ret_val is_dload_mod(void);
/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

#if (FEATURE_ON == MBB_FAST_ON_OFF)
#if (MBB_CHARGE == FEATURE_ON)
int reb_fast_onoff_event_deal(struct notifier_block *nb, unsigned long val, void *data);
void reb_for_fast_onoff_stop(void);
#endif
#endif
/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/
extern struct blocking_notifier_head g_fast_on_off_notifier_list;
#if ( YES == Reb_Platform_V7R2 )
extern reb_hard_timer_callback g_hard_timer_call_func;
#endif
/*kernel 阶段读取的NV50442的值*/
SOFT_RELIABLE_CFG_STRU g_kernel_soft_reliable_info = {0};
/*关机同步文件系统前，停止写标志*/
static bool g_stop_write = FALSE;

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/
#if (FEATURE_ON == MBB_FAST_ON_OFF)
#if (MBB_CHARGE == FEATURE_ON)
/*假关机时的电池电压,因为存在假关机前虚低的问题,该变量
引申为假关机20秒后,电池电压稳定时的值,是判断是否56mv过放的基准*/
static int volt_of_fast_off = 0;

/*假关机时定时的次数*/
static unsigned int volt_inspect_cnt = 0;

/*假关机时电池电压监控值*/
static int volt_inspect_record[VOLT_INSPECT_MAX_TIME + 1] = {0};


/*定义假关机事件通知链回调结构体*/
struct notifier_block fast_on_event_notifier = {
    .notifier_call = reb_fast_onoff_event_deal,
};

/*过放的电压最小刻度与产品电池容量对应*/
unsigned int bat_overuse_delt
    = ( VOLT_INSPECT_BASE_VOLT_CAPABILITY * VOLT_INSPECT_VOLT_DELTA_UNIT )
    / ( MBB_CHG_BAT_CAPABILITY ); 
#endif
#endif
/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/
/*3000mAh电池,电压与电量对照表,取3个放电曲线的最小值*/
/*并且我们认为在每0.1V刻度内电量与电压时近似线性的关系*/
volt_and_elec volt_arry[] =
{
    { 4100, 180 },  /*4.1V-4.0V之间(至少)有200mAh电量*/
    { 4000, 360 },  /*4.0V-3.9V之间(至少)有360mAh电量*/
    { 3900, 400 },  /*3.9V-3.8V之间(至少)有400mAh电量*/
    { 3800, 680 },  /*3.8V-3.7V之间(至少)有680mAh电量*/
    { 3700, 900 },  /*3.7V-3.6V之间(至少)有900mAh电量*/
    { 3600, 280 },  /*3.6V-3.5V之间(至少)有280mAh电量*/
};

/*************************************************************
  函数名称  : reb_reliability_nv_cfg_kernel_init
  功能描述  : 读取nv 50442 的值，获取可靠性配置信息
  输入参数  : 无
  输出参数  : 无
  返 回 值  :无
*************************************************************/
int reb_reliability_nv_cfg_kernel_init(void)
{
    return reb_nv_kernel_read();
}
EXPORT_SYMBOL(reb_reliability_nv_cfg_kernel_init);


#if (FEATURE_ON == MBB_FAST_ON_OFF)
#if (MBB_CHARGE == FEATURE_ON)
/*****************************************************************************
 函 数 名  : reset_bat_inspect_para
 功能描述  : 检测电池参数重置
 输入参数  : none
 输出参数  : none
 返 回 值  : none
 注意事项  : 1\每次进入假关机 2\真关机之前 3\电池电压跌到4.1V以下
*****************************************************************************/
void reset_bat_inspect_para(void)
{
    reb_msg( REB_MSG_INFO, "REB_DFT:Fast on/off reset all inspect para\r\n");

    /*重置电压检测次数*/
    volt_inspect_cnt = 0;

    /*重置关机时刻电池电压*/
    volt_of_fast_off = 0;

    /*重置电池电压记录*/
    memset( volt_inspect_record, 0, sizeof(volt_inspect_record) );
}
#if 0
/*****************************************************************************
 函 数 名  : get_low_bat_level
 功能描述  : 获取产品电池低电电压值(该接口暂时打桩,后面要放到充放电模块中)
 输入参数  : none
 输出参数  : none  
 返 回 值  : 电池电压值
*****************************************************************************/
int get_low_bat_level(void)
{
    return LOW_POWER_BAT_LVL;

    /*要在充电模块初始化以后调用*/
    //return g_chgBattVoltProtect.VbatLevelLow_MAX;
}
#endif
#ifdef REB_OS_DEBUG
/*****************************************************************************
 函 数 名  : show_bat_inspect_val
 功能描述  : 显示记录的电池电压
 输入参数  : none
 输出参数  : none
 返 回 值  : none
*****************************************************************************/
void show_bat_inspect_val(void)
{
    unsigned int idx = 0;
    if ( bat_overuse_delt )
    {
        reb_msg( REB_MSG_DEBUG, "REB_DFT:bat_overuse_delt is %umv\r\n",
            bat_overuse_delt );
    }
    
    if ( 0 == volt_inspect_cnt )
    {
        reb_msg( REB_MSG_DEBUG, "REB_DFT:No volt inspect record\r\n");
        return;
    }

    reb_msg( REB_MSG_DEBUG, "REB_DFT:Total %u inspect record\r\n", volt_inspect_cnt );

    for ( idx = 0; idx < volt_inspect_cnt; idx++ )
    {
        reb_msg( REB_MSG_DEBUG, "REB_DFT:The %dth inspect record is %dmV\r\n",\
            idx, volt_inspect_record[idx] );
    
    }
}
#endif
/*****************************************************************************
 函 数 名  : is_current_normal
 功能描述  : 查看功耗是否正常
 输入参数  : none
 输出参数  : none
 返 回 值  : BAT_VOLT_STA-当前电池状态
 注意事项  : 1- 对于电量消耗慢的(半个小时在10mv以内),下次定时的时间要加倍，并最终
             要清除该定时
             2- 对于电量消耗快的(可以多确定几次,并最终真关机)
*****************************************************************************/
BAT_VOLT_STA is_current_normal(void)
{
    /*获取当前电池电压*/
    int cur_volt = chg_get_batt_volt_value();

    int low_power_batt_volt = get_low_bat_level();

    /*电池电压高于4.0V时,保存最近一次电压值,否则为0*/
    static int last_volt = 0;

    /*如果没有处于假关机,直接返回电池OK*/
    if ( FASTONOFF_MODE_SET != fastOnOffGetFastOnOffMode() )
    {
        reb_msg( REB_MSG_ERR, "REB_DFT:Fast on/off Not in fast off mode\r\n");
        return STOP_INSPECT;
    }

    /*对于检测电池电压的方案:可能存在假关机前虚低,进入假关机后电压正常的现象*/
    if ( cur_volt > volt_of_fast_off )
    {
        /*可能电压不准*/
        reb_msg( REB_MSG_ERR, "REB_DFT:Fast on/off Invalid bat volt: %d\r\n",
            cur_volt );
        /*低于3.6V不进行保护,以避免与假关机3.55V保护冲突*/
        if ( ( low_power_batt_volt + bat_overuse_delt ) >= cur_volt ) 
        {
            reb_msg( REB_MSG_ERR, "REB_DFT:Fast on/off lower than %dmv, stop...\r\n",
                ( low_power_batt_volt + bat_overuse_delt ) );
            return STOP_INSPECT;
        }
        /*此处volt_of_fast_off也会被清零*/
        reset_bat_inspect_para();
        volt_of_fast_off = cur_volt; /*刷新volt_of_fast_off*/
        return INVALID_VOLT;
    }
    reb_msg( REB_MSG_ERR, "REB_DFT:Fast on/off cur bat volt: %d, cnt: %ust\r\n",
        cur_volt,  ( volt_inspect_cnt + 1 ) );

    /*记录本次采集值(0~9)*/
    if ( volt_inspect_cnt < VOLT_INSPECT_MAX_TIME )
    {
        volt_inspect_record[volt_inspect_cnt] = cur_volt;
    }

    /*电池电压还在4.0V以上,等到4.0V*/
    if ( cur_volt > VOLT_INSPECT_UPPER_LIMIT )
    {
        /*记录该次电压*/
        last_volt = cur_volt;
        reb_msg( REB_MSG_INFO, "REB_DFT:Fast on/off higner than 4.0V wait...\r\n" );
    }
    else
    {
        /*电池电压降到4.0V以下,清除检测次数*/
        if ( last_volt > VOLT_INSPECT_UPPER_LIMIT )
        {
            /*此处volt_of_fast_off也会被清零*/
            reset_bat_inspect_para();
            last_volt = 0;
            volt_of_fast_off = cur_volt;
            return INVALID_VOLT;
        }
        
        /*(0~9)10次之内,包含10次*/
        if ( volt_inspect_cnt < VOLT_INSPECT_MAX_TIME )
        {
            /*电池电压消耗超过56mv*/
            if ( ( volt_of_fast_off - cur_volt ) >= bat_overuse_delt )
            {
                /*(0~9)10次之内电压降低超过bat_overuse_delt,准备关机*/
                reb_msg( REB_MSG_ERR, "REB_DFT:Fast on/off Delt > %umv, go to power down\r\n",
                    bat_overuse_delt );
                return BAT_OVER_USE;
            }
        }
    }

    if ( ( volt_inspect_cnt + 1 ) >= VOLT_INSPECT_MAX_TIME )
    {
        /*如果超过(0~9)10次都没有超过56mV,停止检测*/
        reb_msg( REB_MSG_INFO, "REB_DFT:Fast on/off stop inspect\r\n");
        return STOP_INSPECT;
    }
    else
    {
        reb_msg( REB_MSG_DEBUG, "REB_DFT:Fast on/off continue inspect\r\n");
        return CONTINUE_INSPECT;
    }
}
/*****************************************************************************
 函 数 名  : reb_for_fast_onoff_start
 功能描述  : 启动定时,看是否进入低功耗,如果没有,则进行真关机
 输入参数  : none
 输出参数  : none
 返 回 值  : none
*****************************************************************************/
void reb_for_fast_onoff_start(void)
{
    /*获取假关机时刻的电池电压*/
    volt_of_fast_off = chg_get_batt_volt_value();
    reb_msg( REB_MSG_INFO, "REB_DFT:Fast on/off current bat volt %umv\r\n",
        volt_of_fast_off );
    
    /*重置假关机下电池检测参数*/
    reset_bat_inspect_para();
#if ( YES == Reb_Platform_V7R2 )
    /*设置定时回调函数*/
    g_hard_timer_call_func = reb_fast_onoff_inspect_callback;
#endif
    /*为防止假关机时V虚低,20秒以后进行首次监测,并更新volt_of_fast_off*/
    hard_timer_creat( FAST_ON_OFF_FST_CYCLE, REB_HARD_TIMER_OUT_EVENT );

    return;
}

/*****************************************************************************
 函 数 名  : reb_for_fast_onoff_stop
 功能描述  : 快速开机/(由于功耗正常)停止过放检测
 输入参数  : none
 输出参数  : none  
 返 回 值  : none
*****************************************************************************/
void reb_for_fast_onoff_stop(void)
{
    /*重置假关机下电池检测参数*/
    reset_bat_inspect_para();
#if ( YES == Reb_Platform_V7R2 )
    g_hard_timer_call_func = NULL;
    hard_timer_delete();
#endif
#if ( YES == Reb_Platform_9x25 )
    reb_send_msg_to_main_task(REB_HARD_TIMER_STOP_EVENT);
#endif
    return;
}

/*****************************************************************************
 函 数 名  : reb_fast_onoff_event_deal
 功能描述  : 可靠性快速开关机事件处理
 输入参数  : none
 输出参数  : none  
 返 回 值  : none
*****************************************************************************/
int reb_fast_onoff_event_deal(struct notifier_block *nb, unsigned long val, void *data)
{
    int ret_val = 0;
    reb_msg( REB_MSG_ERR, "REB_DFT:reb_fast_onoff_event_deal %d\r\n", val );

    switch (val)
    {
        /*退出假关机状态*/
        case FASTONOFF_MODE_CLR:
            reb_for_fast_onoff_stop();
            break;
        /*进入假关机状态*/
        case FASTONOFF_MODE_SET:
            /*不允许待机*/
            reb_lock_sleep( 0 );
            if ( 0 == chg_is_bat_only() )
            {
                /*假关机充电不进行保护*/
                reb_msg( REB_MSG_ERR, "REB_DFT:Fast on/off Not Bat only!\r\n" );
            }
            else
            {
                reb_msg( REB_MSG_INFO, "REB_DFT:Fast on/off YES Bat only!\r\n" );
                reb_for_fast_onoff_start();
            }
            /*允许待机*/
            reb_unlock_sleep( 0 );
            break;
        default:
            ret_val = reb_err;
            break;
    }

    return ret_val;

}
/*****************************************************************************
 函 数 名  : reb_fast_onoff_inspect_callback
 功能描述  : Hard timer超时后检测电池电压
 输入参数  : none
 输出参数  : none
 返 回 值  : none
*****************************************************************************/
void reb_fast_onoff_inspect_callback(void * para)
{
    BAT_VOLT_STA result = INVALID_VOLT;

    /*加锁保护,防止进入待机*/
    reb_lock_sleep( 0 );

    result = is_current_normal();
#ifdef REB_OS_DEBUG
    reb_msg( REB_MSG_DEBUG, "REB_DFT:Fast on/off %s return %d\r\n",
        __func__, result );
#endif

    switch (result)
    {
        case INVALID_VOLT:
            /*重置定时器*/
#if ( YES == Reb_Platform_V7R2 )
            hard_timer_reset( FAST_ON_OFF_CYCLE );
#endif
#if ( YES == Reb_Platform_9x25 )
           reb_send_msg_to_main_task(REB_HARD_TIMER_OUT_EVENT);
#endif
            break;
        case CONTINUE_INSPECT:  /*继续检测*/
            volt_inspect_cnt++;
#if ( YES == Reb_Platform_V7R2 )
            /*重置定时器*/
            hard_timer_reset( FAST_ON_OFF_CYCLE );
#endif
#if ( YES == Reb_Platform_9x25 )
           reb_send_msg_to_main_task(REB_HARD_TIMER_OUT_EVENT);
#endif
            break;

        case BAT_OVER_USE:
            /*重置假关机下电池检测参数*/
            reb_for_fast_onoff_stop();
            reb_send_msg_to_main_task(REB_FAST_OFF_BAT_OVERUSE);
            reb_delay_ms( 1000 );/*延迟1秒*/
            reb_for_power_off();
            break;
        case STOP_INSPECT:      /*要停止检测*/
            /*重置假关机下电池检测参数*/
            reb_for_fast_onoff_stop();
            break;
        default:
            reb_msg( REB_MSG_ERR, "REB_DFT:Fast on/off is_current_normal ret err\r\n");
            break;
    }

    /*解除锁保护*/
    reb_unlock_sleep( 0 );
}

/*****************************************************************************
 函 数 名  : reb_fast_off_inspect_init
 功能描述  : 快速关机后测电池电压模块初始化
 输入参数  : none
 输出参数  : none
 返 回 值  : none
*****************************************************************************/
void reb_fast_off_inspect_init(void)
{
    int ret_val = 0;

    /*查看假关机失败进入真关机功能是否开启*/
    if ( g_kernel_soft_reliable_info.FastOffFailTrueShutdownEnable )
    {
        /*注册快速开关机事件通知链*/
        ret_val = blocking_notifier_chain_register(&g_fast_on_off_notifier_list, &fast_on_event_notifier);
        if(ret_val)
        {
            reb_msg( REB_MSG_ERR, "REB_DFT:reb_fast_off_inspect_init fail\r\n");
        }
    }
}
#endif
#endif


/*****************************************************************************
 函 数 名  : reb_write_partition_beyond
 功能描述  : MTD flash分区是否写越界
 输入参数  : from-起始地址;cnt_addr-要写的字节数的(栈)地址;
             part_name-该flash分区名字;part_size-该flash分区尺寸
 输出参数  : none
 返 回 值  : none
*****************************************************************************/
void reb_write_partition_beyond(const long long from, unsigned int* cnt_addr,
                const char* part_name, unsigned int part_size )
{
    long long to = 0;

    if ( NULL == cnt_addr )
    {
        reb_msg( REB_MSG_ERR, "REB_DFT: cnt_addr is NULL \r\n" );
        return;
    }

    to = from + *cnt_addr;
    /*根据NV配置决定是否开启*/
    if (g_kernel_soft_reliable_info.ZoneWriteProtectEnable)
    {
        if ( to > part_size )
        {
            *cnt_addr = 0;
            if ( part_name )
            {
                reb_msg( REB_MSG_ERR, 
                                    "REB_DFT: Write partition:%s ERROR\r\n", part_name );
            }
            reb_msg( REB_MSG_ERR, "REB_DFT: From:0x%08llX\r\n", from );
            reb_msg( REB_MSG_ERR, "REB_DFT: To  :0x%08llX\r\n", to  );
        }

        /*通知主控任务*/
        reb_send_msg_to_main_task( REB_PRT_BYND_EVENT );
    }
    else /*分区防护功能未开启*/
    {
        /*不做任何处理*/
        return;
    }
}
/*****************************************************************************
 函 数 名  : reb_inspect_free_memory
 功能描述  : 监控内存是否足够
 输入参数  : from-起始地址;cnt_addr-要写的字节数的(栈)地址;the_mtd-该flash分区信息
 输出参数  : none
 返 回 值  : none
*****************************************************************************/
void reb_inspect_free_memory(void)
{
    return;
}

/*****************************************************************************
 函数名称  : void reb_sys_start_write_protect(void)
 功能描述  : 用于判断并启动开机文件写的预警处理
 输入参数  : none
 输出参数  : none
 返 回 值  : none
*****************************************************************************/
void reb_sys_start_write_protect(char *name)
{
    static bool  write_protect = TRUE;
    unsigned long current_time = 0;
    reb_ret_val ret_val = reb_err;

    /*升级模式该功能不生效*/
    ret_val = is_dload_mod();
    if ( ( reb_err == ret_val ) || ( reb_true == ret_val ) )
    {
        /*如果是下载模式该功能不生效*/
        return;        
    }

    if(0 == reb_is_factory_mode())
    {
        return;
    }
    /*根据NV配置决定是否开启*/
    if ( g_kernel_soft_reliable_info.BootNotOperaFileSysEnable)
    {
        if (FALSE == write_protect)
        {
            reb_msg( REB_MSG_DEBUG, 
                                 "Reb_DFT it should not start write protect function\n");
            return;
        }
        /*获取当前开机时间*/
        current_time = reb_get_current_systime();
        if (current_time < g_kernel_soft_reliable_info.DangerTime)
        {
            /*打印出警告*/
            reb_msg( REB_MSG_ERR, "Reb_DFT write name %s\r\n", name);
        }
        else
        {
            write_protect = FALSE;
            reb_msg( REB_MSG_INFO, "Reb_DFT the write timer forbiden has passed\n");
        }

    }
    else /*开机文件写保护功能开启*/
    {
        /*不做任何处理*/
        return;
    }
    return;
}
/*****************************************************************************
 函数名称  :  void reb_file_sync(void)
 功能描述  : 用于关机或者重启时文件系统的同步
 输入参数  : none
 输出参数  : none
 返 回 值  : none
*****************************************************************************/
void reb_file_sync(void)
{
    if(0 == reb_is_factory_mode())
    {
        return;
    }
    /*根据NV配置决定是否开启*/
    if (g_kernel_soft_reliable_info.FileSysActiveProtectEnable)
    {
        g_stop_write = TRUE;
        reb_msg( REB_MSG_DEBUG, "Reb_DFT Syncing filesystems...\n");
        sys_sync();
        reb_msg( REB_MSG_DEBUG, "Reb_DFT done.\n");
        reb_delay_ms(500); /*延迟500ms*/
    }
    return ;
}

/*****************************************************************************
 函数名称  : bool reb_stop_write(void)
 功能描述  : 获取是否可以写FLASH的标志
 输入参数  : None
 输出参数  : None
 返 回 值  : g_stop_write
 修改历史  : 
             1. 2013-11-24 :  00206465 qiaoyichuan created
*****************************************************************************/
int reb_stop_write_file(void)
{
    return g_stop_write;
}


