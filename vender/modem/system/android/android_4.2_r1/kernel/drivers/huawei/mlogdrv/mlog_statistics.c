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
#include <linux/types.h>
#include <linux/termios.h>
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/string.h>
#include <linux/interrupt.h>
#include <linux/notifier.h>
#include <linux/reboot.h>
#include <linux/syscalls.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/bitops.h>
#include <linux/mutex.h>
#include <linux/compat.h>
#include <asm/io.h>
#include <asm/uaccess.h>
//#include <mach/huawei_smem.h>
#include <linux/mlog_lib.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#ifdef CONFIG_HAS_WAKELOCK     
#include <linux/wakelock.h>
#endif
#include <bsp_sram.h>

#define FLASH_BLOCK_128K  (128 * 1024)
#define SHIFT_128KB  (17)    /*128k 移位 bit*/
#define FLASH_BLOCK_256K  (256 * 1024)
#define SHIFT_256KB  (18)    /*256k 移位 bit*/
#define MAX_ITEM_CNT (100)   /*队列中保持200个统计item*/
#define MAX_FLASH_ITEM_CNT (300)   /*队列中保持FLASH 个统计item*/
#define MLOG_DELAY_TIME_MS (10)  /*每次延迟time 发送统计信息，减少系统负荷*/
#define MLOG_DELAY_TIMES (10)
#define MLOG_FLASH_DELAY_TIME_MS (10)  /*每次延迟time 发送统计信息，减少系统负荷*/
#define MLOG_REBOOT_DELAY_TIME_MS (500)
#define MLOG_SECONDS_IN_SIX_MINUTE (60 * 6)
#define MLOG_INFORNMATION_STAT_INTERVAL (360)      /*unit : s*/
#define MLOG_SECONDS_IN_ONE_MINUTE (60)
#define FLASH_MLOG_OWN_PARTION_NAME  "mlog"
#define FLASH_MLOG_SHARE_PARTION_NAME  "bbou"
#define SHUTDOWN_FLAG    (0x0001)
#define SYNC_FLAG   (0x5f5f)
enum
{
    TYPE_FLASH_WRITE,
    TYPE_FLASH_ERASE,
    TYPE_TYPE_MAX
};

/*for comm statistics interface */
struct mlog_comm_info_t
{
    char* item_name;
    char* item_value;
    struct mlog_comm_info_t* next;
} ;

/*just for flash statistics interface*/
struct mlog_flash_info
{
    unsigned int blockid;
    unsigned int erase_num;
    unsigned int write_num;
    struct mlog_flash_info* next;
};

yaffs_dev_statis_info_t g_yaffs_dev_statis_info;          /*记录增量*/
yaffs_dev_statis_info_t g_yaffs_dev_statis_info_old;    /*记录当前的数据*/

/*for record the power time*/
static unsigned int new_last_times = 0;
static unsigned int old_last_times = 0;
static unsigned int rest_last_times = 0;

static DEFINE_RAW_SPINLOCK(mlog_flash_statistics_lock);
static DEFINE_RAW_SPINLOCK(mlog_comm_statistics_lock);

#if defined(CONFIG_HAS_WAKELOCK) 
struct wake_lock mlog_sync_lock;
#endif

static int g_upgrade_mode = 0;

/*统计信息驱动队列，统计flash*/
static unsigned int g_flash_list_cnt = 0;
static struct mlog_flash_info* head = NULL;
static struct mlog_flash_info* tail = NULL;

/*统计信息驱动队列，通用的统计队列*/
static unsigned int g_comm_list_cnt = 0;
static struct mlog_comm_info_t* comm_head = NULL;
static struct mlog_comm_info_t* comm_tail = NULL;

struct timer_list mlog_poweron_record_timer;

char yaffs_dev_param[][MAX_ITEM_LEN] =
{
    {"n_page_writes"},
    {"n_page_reads"},
    {"n_erasures"},
    {"n_erase_failures"},
    {"n_gc_copies"},
    {"all_gcs"},
    {"passive_gc_count"},
    {"oldest_dirty_gc_count"},
    {"n_gc_blocks"},
    {"bg_gcs"},
    {"n_retired_writes"},
    {"n_retired_blocks"},
    {"n_ecc_fixed"},
    {"n_ecc_unfixed"},
    {"n_tags_ecc_fixed"},
    {"n_tags_ecc_unfixed"},
    {"n_deletions"},
    {"n_unmarked_deletions"},
    {"refresh_count"},
    {"cache_hits"}
};

extern unsigned int g_mlog_drv_debug_level;
extern unsigned int g_partition_shared;
extern struct timer_list mlog_stat_timer;

int yaffs_info_statistics(void);
void mlog_set_statis_info(char* item_name, unsigned int item_value);
void  mlog_statistic_info_init(void);
void mlog_flash_list_init(void);
void mlog_comm_list_init(void ) ;
#if 0 
static void mlog_poweron_record(void);
#endif 

static void mlog_poweron_time_stat(unsigned long lparam);
static void mlog_comm_clear_item(void);
static void mlog_flash_clear_item(void);

void  smem_statistic_info_process(void);

void mlog_process_common_statistics_info(void);
void mlog_process_flash_statistics_info(void);
void mlog_process_yaffs_statistics_info(void);
int mlog_is_upgrade_mode(void);
static int mlog_notify_sys_shutdown(struct notifier_block *this, 
    unsigned long code,void *unused);
void  sys_shutdown_mog_report(void);
void mlog_sync(void);
extern int mlog_server_is_initd(void);
extern int mlog_stat_item_send(mlog_server_stat_info* stat_info) ;

static struct notifier_block mlog_sys_shutdown_notifier = {
    .notifier_call = mlog_notify_sys_shutdown,
};
/*****************************************************************************
 函 数 名  : mlog_sync
 功能描述  : 同步未刷新的消息，刷新到flash
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2014年6月18日
    作    者   : liufang
    修改内容   : 新生成函数

*****************************************************************************/
void mlog_sync(void)
{   
    
    mlog_server_stat_info  stat_info;
    printk(KERN_EMERG "--- sync mlog msg ---\n");
    snprintf(stat_info.item_name, MAX_ITEM_LEN - 1,
                 "%s.%s", "EVNET", "shutdown_flag");
    snprintf(stat_info.item_value, MAX_ITEM_LEN - 1,
                 "%d", SYNC_FLAG);
    stat_info.item_flags = SHUTDOWN_FLAG;
    mlog_stat_item_send(&stat_info);
    return;
}

/*****************************************************************************
 函 数 名  : mlog_notify_sys_shutdown
 功能描述  : 通知链处理函数
 输入参数  : struct notifier_block *this  
             unsigned long code           
             void *unused                 
 输出参数  : 无
 返 回 值  : static
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2014年6月18日
    作    者   : liufang
    修改内容   : 新生成函数

*****************************************************************************/
static int mlog_notify_sys_shutdown(struct notifier_block *this, 
    unsigned long code,void *unused)
{

sys_shutdown_mog_report();

}


void  sys_shutdown_mog_report(void)
{
    int upgrade_flag = 0 ;
    huawei_smem_info *smem_data = NULL;
    smem_data = (huawei_smem_info *)SRAM_DLOAD_ADDR;
    upgrade_flag = smem_data->smem_dload_flag;

    if (SMEM_DLOAD_FLAG_NUM == upgrade_flag )
    {
        printk(KERN_EMERG "--- It is in upgrade mode ---\n");
        return 0;
    }

    if (0 == mlog_server_is_initd())
    {
        printk(KERN_EMERG "--- mlog server is not inited ---\n");
        return 0;
    }

 #ifdef CONFIG_HAS_WAKELOCK
    wake_lock(&mlog_sync_lock);          
#endif   
     printk(KERN_EMERG "--- sys_shutdown_mog_report start ---\n");
   /*del stat report timer */
    if(timer_pending(&mlog_stat_timer))
    {
        del_timer_sync(&mlog_stat_timer);
    }
 
/*读取modem的共享内存信息*/
    smem_statistic_info_process();
  
/*将最新的开机时间的统计发送给mlog service*/    
    mlog_poweron_time_stat(0);
/*将mlog_comm_get_item队列里的item发送给mlog service*/
    mlog_process_common_statistics_info();

/*将yaffs2的统计信息发送给mlog service*/
    mlog_process_yaffs_statistics_info();
/*让app 立刻产生同步消息*/
    mlog_sync();

#ifdef CONFIG_HAS_WAKELOCK
    wake_unlock(&mlog_sync_lock);           /*no release because reboot soonly*/
/*uplayer maybe sent reboot command when recognize AT shutown command timeout*/
#if 0
    wake_lock_destroy(&mlog_sync_lock);
#endif 
#endif
    msleep(MLOG_DELAY_TIME_MS * MLOG_DELAY_TIMES);
    sys_sync();

    msleep(MLOG_REBOOT_DELAY_TIME_MS);
    printk(KERN_EMERG "--- sys_shutdown_mog_report end ---\n");

    return 0;
}
/*****************************************************************************
 函 数 名  : mlog_register_reboot_notifier
 功能描述  : 注册内核通知链
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2014年6月18日
    作    者   : liufang
    修改内容   : 新生成函数

*****************************************************************************/
void mlog_register_reboot_notifier(void)
{
    register_reboot_notifier(&mlog_sys_shutdown_notifier);
}

void mlog_unregister_reboot_notifier(void)
{
    unregister_reboot_notifier(&mlog_sys_shutdown_notifier);
}


/*****************************************************************************
函数名称  : mlog_set_startup_mode
功能描述  :
输入参数  :
输出参数  :
返 回 值  : No
修改历史  :
*****************************************************************************/
void mlog_set_startup_mode(void)
{
    int upgrade_flag = 0 ;
    huawei_smem_info *smem_data = NULL;
    smem_data = (huawei_smem_info *)SRAM_DLOAD_ADDR;
    upgrade_flag = smem_data->smem_dload_flag;

    if (SMEM_DLOAD_FLAG_NUM == upgrade_flag )
    {
        g_upgrade_mode = 1;
    }
}


/*****************************************************************************
 函 数 名  : mlog_is_upgrade_mode
 功能描述  : 判断当前是否为升级模式
 输入参数  : void
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年6月10日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
int mlog_is_upgrade_mode(void)
{
    return  g_upgrade_mode;
}


/*****************************************************************************
函数名称  : mlog_statistic_info_init
功能描述  :
输入参数  :
输出参数  :
返 回 值  : No
修改历史  :
*****************************************************************************/
void  mlog_statistic_info_init(void)
{
#ifdef CONFIG_HAS_WAKELOCK
    wake_lock_init(&mlog_sync_lock, WAKE_LOCK_SUSPEND, "mlog_sync_lock");
#endif
    mlog_comm_list_init();
    mlog_flash_list_init();
#if 0
    /*record the share mem static info */
    mlog_poweron_record();
#endif

    init_timer(&mlog_poweron_record_timer);
    mlog_poweron_record_timer.expires = jiffies + MLOG_INFORNMATION_STAT_INTERVAL * HZ;
    mlog_poweron_record_timer.function = mlog_poweron_time_stat;
    mlog_poweron_record_timer.data = 0;
    add_timer(&mlog_poweron_record_timer);
}

void mlog_statistic_info_destory(void)
{   
    #ifdef CONFIG_HAS_WAKELOCK
        wake_lock_destroy(&mlog_sync_lock);
    #endif
    if (timer_pending(&mlog_poweron_record_timer))
    {
        del_timer_sync(&mlog_poweron_record_timer);
    }
    mlog_flash_clear_item();
    mlog_comm_clear_item();

}
#if 0
/*****************************************************************************
 函 数 名  : flash_efs_stat
 功能描述  :
 输入参数  : 无
 输出参数  : 无
 返 回 值  :无
 调用函数  :
 被调函数  :
  修改历史      :
  1.日    期   : 2014年5月28日
    修改内容   : 新生成函数
*****************************************************************************/
int flash_efs_stat(void)
{
    static huawei_smem_info_vendor2* p_smem_data = NULL;
    uint32 index = 0;
    uint32 blockid = 0;
    uint32 erasetimes = 0;
    uint32 writetimes = 0;
    mlog_server_stat_info  stat_info;

    memset((void*)&stat_info, 0, sizeof(mlog_server_stat_info));
    if (NULL == p_smem_data)
    {
        p_smem_data = (huawei_smem_info_vendor2*)SMEM_HUAWEI_ALLOC(SMEM_ID_VENDOR2,
                      SMEM_VENDOR2_RESERVED_LEN);
        if (NULL == p_smem_data)
        {
            printk(KERN_ERR "flash_erase_stat,SMEM_HUAWEI_ALLOC fail !\n");
            return -1;
        }
    }
    for (index = 0; index < FLASH_EFS_STAT_NUMBER; index++)
    {
        if (FLASH_ERASE_WRITE_MAGIC == p_smem_data->flash_efs_stat_info[index].flags)
        {
            blockid = p_smem_data->flash_efs_stat_info[index].blockid;
            erasetimes = p_smem_data->flash_efs_stat_info[index].erasetimes;
            writetimes = p_smem_data->flash_efs_stat_info[index].writetimes;


            snprintf(stat_info.item_name, MAX_ITEM_LEN - 1,
                     "%s.BLOCK%ld.earsetimes", "FLASH", blockid);
            snprintf(stat_info.item_value, MAX_ITEM_LEN - 1,
                     "%ld", erasetimes);

            if (g_mlog_drv_debug_level & FLASH_DEBUG_LEVEL)
            {
                printk(KERN_EMERG "%s:item_name %s ,item_value %s\n",
                       __func__, stat_info.item_name, stat_info.item_value);
            }
            mlog_stat_item_send(&stat_info);

            snprintf(stat_info.item_name, MAX_ITEM_LEN - 1,
                     "%s.BLOCK%ld.writetimes", "FLASH", blockid);
            snprintf(stat_info.item_value, MAX_ITEM_LEN - 1,
                     "%ld", writetimes);

            if (g_mlog_drv_debug_level & FLASH_DEBUG_LEVEL)
            {
                printk(KERN_EMERG "%s:item_name %s ,item_value %s\n",
                       __func__, stat_info.item_name, stat_info.item_value);
            }
            mlog_stat_item_send(&stat_info);

            /*clear the share mem*/
            p_smem_data->flash_efs_stat_info[index].blockid = 0;
            p_smem_data->flash_efs_stat_info[index].erasetimes  = 0;
            p_smem_data->flash_efs_stat_info[index].writetimes = 0;
            p_smem_data->flash_efs_stat_info[index].flags = 0;
            p_smem_data->flash_efs_operation_blocks = 0;
        }

    }

    return 0;
}
#endif 

/*****************************************************************************
函数名称  : mlog_poweron_time_stat
功能描述  :
输入参数  :
输出参数  :
返 回 值  :
修改历史  :
*****************************************************************************/
static void mlog_poweron_time_stat(unsigned long lparam)
{
    struct timespec uptime;

    do_posix_clock_monotonic_gettime(&uptime);
    monotonic_to_bootbased(&uptime);

    new_last_times = uptime.tv_sec;


    mlog_set_statis_info("power_last_times",
                         ( new_last_times - old_last_times  + rest_last_times) / MLOG_SECONDS_IN_SIX_MINUTE) ;

    rest_last_times = ( new_last_times - old_last_times + rest_last_times )
                      % MLOG_SECONDS_IN_SIX_MINUTE ;

    if (g_mlog_drv_debug_level & COMM_DEBUG_LEVEL)
    {
        printk(KERN_ERR "new_last_times is %d, old_last_times is %d\n\n",
               new_last_times, old_last_times);
    }

    old_last_times = new_last_times;
    mod_timer(&mlog_poweron_record_timer, jiffies + MLOG_INFORNMATION_STAT_INTERVAL * HZ);
}

#if 0 
/*****************************************************************************
函数名称  : mlog_poweron_record
功能描述  :
输入参数  :
输出参数  :
返 回 值  :
修改历史  :
*****************************************************************************/
static void mlog_poweron_record()
{
    /*获取共享内存的值，判断重启做的行为*/
    huawei_smem_info* smem_data = NULL;
    huawei_smem_vendor1_info* vendor1_smem_data = NULL;

    smem_data = (huawei_smem_info*)SMEM_HUAWEI_ALLOC(SMEM_ID_VENDOR0,
                sizeof(huawei_smem_info ));
    if (NULL == smem_data)
    {
        printk(KERN_ERR "smem_data is NULL\n");
        return;
    }

    vendor1_smem_data =
        (huawei_smem_vendor1_info*)SMEM_HUAWEI_ALLOC(SMEM_ID_VENDOR1,
                SMEM_VENDOR1_RESERVED_LEN);
    if (NULL == vendor1_smem_data)
    {
        printk(KERN_ERR "vendor1 smem_data is NULL\n");
        return;
    }

    /**************************************************************************************************
        1.开机时不会置smem_rest_flags标志位，不会走到第一条分支
        2.软件重启后会设置smem_rest_flags标志位为软件重启(正常或者异常)，也不会检测到硬件重启
    ***************************************************************************************************/
    if (SMEM_HARDWARE_REST == vendor1_smem_data->smem_reset_flags)
    {
        printk(KERN_ERR "normal hardware reset.\n");
        mlog_print(MLOG_POWERON_RESTART, mlog_lv_info, "normal hardware reset.\n");
    }
    else if (NORMAL_RESTART == smem_data->smem_huawei_poweroff_chg)
    {
        if (SMEM_SOFTWARE_ABNORMAL_RESET == vendor1_smem_data->smem_reset_flags)
        {
            mlog_set_statis_info("abnormal_reboot_times", 1);
            mlog_print(MLOG_POWERON_RESTART, mlog_lv_info, "abnormall software reset.\n");
        }
        else if (SMEM_SOFTWARE_NORMAL_RESET == vendor1_smem_data->smem_reset_flags)
        {
            printk(KERN_ERR "normal software reset.\n");
            mlog_print(MLOG_POWERON_RESTART, mlog_lv_info, "normal software reset.\n");
        }
    }
    else if (POWER_ON == smem_data->smem_huawei_poweroff_chg)
    {
        mlog_set_statis_info("on_times", 1);
        printk(KERN_ERR "power on by key.\n");
        mlog_print(MLOG_POWERON_RESTART, mlog_lv_info, "power on by key.\n");

    }
    else if (POWER_OFF_CHARGER == smem_data->smem_huawei_poweroff_chg)
    {
        mlog_set_statis_info("on_times", 1);
        printk(KERN_ERR "power on by charger.\n");
        mlog_print(MLOG_POWERON_RESTART, mlog_lv_info, "power on by charger.\n");
    }

    /*default:hardware reset*/
    vendor1_smem_data->smem_reset_flags = SMEM_HARDWARE_REST;
}
#endif 


/*****************************************************************************
函数名称  : smem_statistic_info_process
功能描述  :
输入参数  :
输出参数  :
返 回 值  :
修改历史  :
*****************************************************************************/
void  smem_statistic_info_process(void)
{
    huawei_smem_info *smem_data = NULL;
    smem_data = (huawei_smem_info *)SRAM_DLOAD_ADDR;
    if (NULL == smem_data)
    {
        printk(KERN_ERR "SRAM_DLOAD_ADDR smem_data is NULL\n");
        return;
    }
    if (1 == smem_data->smem_update_times)
    {
        printk(KERN_ERR "smem_update_times is 0x%lx\n",
               smem_data->smem_update_times);
        mlog_set_statis_info("update_times",
                             smem_data->smem_update_times);
        smem_data->smem_update_times = 0;
    }
}

/*****************************************************************************
函数名称  : mlog_comm_insert_item
功能描述  :
输入参数  :
输出参数  :
返 回 值  :
修改历史  :
*****************************************************************************/
int mlog_comm_insert_item(struct mlog_comm_info_t* item_list)
{

    if (NULL == item_list ||
        NULL == item_list->item_name ||
        NULL == item_list->item_value)
    {
        return -1;
    }

    if ( g_comm_list_cnt > MAX_ITEM_CNT)
    {
        if (g_mlog_drv_debug_level & COMM_DEBUG_LEVEL)
        {
            printk(KERN_EMERG "%s:item num %d is over\n",
                   __func__, g_comm_list_cnt);
        }
        return -1;
    }

    g_comm_list_cnt++;

    if (NULL == comm_head && NULL == comm_tail)
    {
        comm_head =  item_list;
        comm_tail = item_list;
        comm_tail->next = NULL;
    }
    else
    {
        comm_tail->next = item_list;
        comm_tail = item_list;
        comm_tail->next = NULL;
    }
    return 0;
}
/*****************************************************************************
 函 数 名  : mlog_comm_get_item
 功能描述  : 通用统计队列出队列函数
 输入参数  : void
 输出参数  : 无
 返 回 值  : struct
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年6月10日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
struct mlog_comm_info_t* mlog_comm_get_item(void)
{
    struct mlog_comm_info_t* item = NULL;

    if (NULL == comm_head)
    {
        return NULL;
    }

    item = comm_head;

    if (comm_tail == comm_head)
    {
        comm_tail = NULL;
    }
    comm_head = comm_head->next;

    g_comm_list_cnt--;

    return item;
}
/*****************************************************************************
 函 数 名  : mlog_comm_list_init
 功能描述  : 通用统计初始化
 输入参数  : 无
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年6月10日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
void mlog_comm_list_init()
{
    comm_head = NULL;
    comm_tail = NULL;
    g_comm_list_cnt = 0;
}

/*****************************************************************************
 函 数 名  : mlog_flash_list_init
 功能描述  : flash统计初始化函数
 输入参数  : void
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年6月10日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
void mlog_flash_list_init(void)
{
    head = NULL;
    tail = NULL;
    g_flash_list_cnt = 0;
}
/*****************************************************************************
 函 数 名  : mlog_flash_insert_item
 功能描述  : flash统计插入队列操作
 输入参数  : struct mlog_flash_info* item_list
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年6月10日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
int mlog_flash_insert_item(struct mlog_flash_info* item_list)
{
    if (NULL == item_list)
    {
        return -1;
    }

    if ( g_flash_list_cnt > MAX_FLASH_ITEM_CNT)
    {
        if (g_mlog_drv_debug_level & FLASH_DEBUG_LEVEL)
        {
            printk(KERN_EMERG "%s:item num %d is over\n",
                   __func__, g_flash_list_cnt);
        }
        return -1;
    }

    g_flash_list_cnt++;

    if (NULL == head && NULL == tail)
    {
        head =  item_list;
        tail = item_list;
        tail->next = NULL;
    }
    else
    {
        tail->next = item_list;
        tail = item_list;
        tail->next = NULL;
    }
    return 0;
}
/*****************************************************************************
 函 数 名  : mlog_flash_get_item
 功能描述  : flash的出队列操作
 输入参数  : void
 输出参数  : 无
 返 回 值  : struct
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年6月10日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
struct mlog_flash_info* mlog_flash_get_item(void)
{
    struct mlog_flash_info* item = NULL;

    if (NULL == head)
    {
        return NULL;
    }

    item = head;

    if (tail == head)
    {
        tail = NULL;
    }
    head = head->next;

    g_flash_list_cnt--;

    return item;
}

/*****************************************************************************
 函 数 名  : mlog_flash_get_tail
 功能描述  : flash获取上次插入队列尾节点
 输入参数  : void
 输出参数  : 无
 返 回 值  : struct
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年7月01日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
struct mlog_flash_info* mlog_flash_get_tail(void)
{
    struct mlog_flash_info* item = NULL;

    if (NULL == tail)
    {
        return NULL;
    }

    item = tail;
    return item;
}
/*****************************************************************************
 函 数 名  : mlog_flash_clear_item
 功能描述  : flash队列清除
 输入参数  : void
 输出参数  : 无
 返 回 值  : struct
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年6月10日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
static void mlog_flash_clear_item(void)
{
    struct mlog_flash_info*  flash_item_info = NULL;
    while (1)
    {
        raw_spin_lock(&mlog_flash_statistics_lock);
        flash_item_info = mlog_flash_get_item();
        raw_spin_unlock(&mlog_flash_statistics_lock);

        if (NULL == flash_item_info)
        {
            break;
        }

        if (g_mlog_drv_debug_level & FLASH_DEBUG_LEVEL)
        {
            printk(KERN_EMERG "clean blk_id = %d,write = %d, erase = %d \n",
                   flash_item_info->blockid, flash_item_info->write_num, flash_item_info->erase_num);
        }

        kfree(flash_item_info);
        flash_item_info = NULL;
        msleep(MLOG_FLASH_DELAY_TIME_MS);
    }
}

/*****************************************************************************
 函 数 名  : mlog_comm_clear_item
 功能描述  : comm队列清除
 输入参数  : void
 输出参数  : 无
 返 回 值  : struct
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年6月10日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
static void mlog_comm_clear_item(void)
{
    struct mlog_comm_info_t* mlog_comm_info = NULL;
    unsigned long flags = 0;

    while (1)
    {
        raw_spin_lock_irqsave(&mlog_comm_statistics_lock, flags);
        mlog_comm_info = mlog_comm_get_item();
        raw_spin_unlock_irqrestore(&mlog_comm_statistics_lock, flags);
        if (NULL == mlog_comm_info)
        {
            break;
        }

        if (g_mlog_drv_debug_level & COMM_DEBUG_LEVEL)
        {
            printk(KERN_EMERG "item = %s,value = %s\n",
                   mlog_comm_info->item_name, mlog_comm_info->item_value);
        }
        kfree(mlog_comm_info->item_name);
        kfree(mlog_comm_info->item_value);
        kfree(mlog_comm_info);
        mlog_comm_info = NULL;
        msleep(MLOG_DELAY_TIME_MS);
    }
}

/*****************************************************************************
 函 数 名  : mlog_set_flash_statistics
 功能描述  : flash的统计接口函数
 输入参数  : unsigned int blocksize
             unsigned long long addr
             unsigned int len
             unsigned int times
             unsigned int type
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年6月10日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
void mlog_set_flash_statistics(unsigned int blocksize, unsigned long long addr,
                unsigned int len, unsigned int times, unsigned int type, char* partition_name)
{
    struct mlog_flash_info*  flash_item_info = NULL;
    struct mlog_flash_info* old_flash_item_info = NULL;
    unsigned long long addr_tmp = 0;
    unsigned int len_tmp = 0 ;
    unsigned char shift = 0;
    int ret = -1;

    if (mlog_is_upgrade_mode())
    {
        return;
    }

    if (type >= TYPE_TYPE_MAX)
    {
        printk(KERN_EMERG "%s input type error \n", __func__);
        return;
    }

    if (0 == strncmp(partition_name,
                     FLASH_MLOG_OWN_PARTION_NAME, strlen(partition_name)))
    {
        return;
    }


    if (g_partition_shared)
    {
        if (0 == strncmp(partition_name,
                         FLASH_MLOG_SHARE_PARTION_NAME, strlen(partition_name)))
        {
            return;
        }
    }

    addr_tmp = addr;
    len_tmp = len;

    if (FLASH_BLOCK_128K == blocksize)
    {
        shift = SHIFT_128KB;
    }

    if (FLASH_BLOCK_256K == blocksize)
    {
        shift = SHIFT_256KB;
    }

    for (; addr_tmp < addr + len; addr_tmp += blocksize)
    {   
        raw_spin_lock(&mlog_flash_statistics_lock);
        old_flash_item_info = mlog_flash_get_tail();
        if ( NULL != old_flash_item_info )
        {
            if ( addr_tmp >> shift == old_flash_item_info->blockid )
            {
                if (TYPE_FLASH_WRITE == type)
                {
                    old_flash_item_info->write_num += times;
                }
                if (TYPE_FLASH_ERASE == type)
                {
                    old_flash_item_info->erase_num += times;
                }
                raw_spin_unlock(&mlog_flash_statistics_lock);
                continue;
            }
        }
        raw_spin_unlock(&mlog_flash_statistics_lock);
        
        flash_item_info =
            (struct mlog_flash_info*)kmalloc(sizeof(struct mlog_flash_info), GFP_ATOMIC);

        if (NULL == flash_item_info)
        {
            printk(KERN_EMERG "%s %d flash_item_info is null \n", __func__, __LINE__);
            return;
        }

        memset(flash_item_info, 0, sizeof(struct mlog_flash_info));

        flash_item_info->blockid = addr_tmp >> shift;
        if (TYPE_FLASH_WRITE == type)
        {
            flash_item_info->write_num = times;
        }
        else if (TYPE_FLASH_ERASE == type)
        {
            flash_item_info->erase_num = times;
        }

        raw_spin_lock(&mlog_flash_statistics_lock);
        ret = mlog_flash_insert_item(flash_item_info);
        raw_spin_unlock(&mlog_flash_statistics_lock);
        if ( ret < 0)
        {
            if (NULL != flash_item_info)
            {
                kfree(flash_item_info);
            }
        }
    }
    return;
}
/*****************************************************************************
 函 数 名  : mlog_set_statis_info
 功能描述  : 通用的统计函数，各模块直接调用即可
 输入参数  : char* item_name
             unsigned int item_value
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年6月10日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
void mlog_set_statis_info(char* item_name, unsigned int item_value)
{
    struct mlog_comm_info_t* ptr = NULL;
    unsigned int name_len = 0;
    unsigned int value_len = 0;
    unsigned long flags = 0;
    char buf[MAX_ITEM_LEN] = {0};
    int ret = -1;

    if (NULL == item_name )
    {
        printk(KERN_EMERG "%s: input error\n", __func__);
        return;
    }

    if (mlog_is_upgrade_mode())
    {
        return;
    }


    name_len = strlen(item_name);

    snprintf(buf, MAX_ITEM_LEN - 1, "%d", item_value);
    value_len = strlen(buf);

    if (name_len > (MAX_ITEM_LEN - 1) ||
        value_len > (MAX_ITEM_LEN - 1))
    {
        printk(KERN_EMERG "%s: name len %d or value_len %d is over\n",
               __func__, name_len, value_len);
        return;
    }

    if (in_interrupt())
    {
        ptr = (struct mlog_comm_info_t*) kmalloc(sizeof(struct mlog_comm_info_t), GFP_ATOMIC);

        if ( NULL == ptr)
        {
            printk(KERN_EMERG "%s: ptr alloc fail\n", __func__);
            return;
        }
        memset(ptr, 0, sizeof(struct mlog_comm_info_t));

        ptr->item_name = (char*)kmalloc(name_len + 1, GFP_ATOMIC);

        if ( NULL == ptr->item_name)
        {
            printk(KERN_EMERG "%s: ptr alloc fail\n", __func__);
            goto fail;
        }

        ptr->item_value = (char*)kmalloc(value_len + 1, GFP_ATOMIC);

        if ( NULL == ptr->item_value)
        {
            printk(KERN_EMERG "%s: ptr alloc fail\n", __func__);
            goto fail;
        }

    }
    else
    {
        ptr = (struct mlog_comm_info_t*) kmalloc(sizeof(struct mlog_comm_info_t), GFP_KERNEL);

        if ( NULL == ptr)
        {
            printk(KERN_EMERG "%s: ptr alloc fail\n", __func__);
            return;
        }
        memset(ptr, 0, sizeof(struct mlog_comm_info_t));

        ptr->item_name = (char*)kmalloc(name_len + 1, GFP_KERNEL);

        if ( NULL == ptr->item_name)
        {
            printk(KERN_EMERG "%s: ptr alloc fail\n", __func__);
            goto fail;
        }

        ptr->item_value = (char*)kmalloc(value_len + 1, GFP_KERNEL);

        if ( NULL == ptr->item_value)
        {
            printk(KERN_EMERG "%s: ptr alloc fail\n", __func__);
            goto fail;
        }
    }

    memset(ptr->item_name, 0, name_len + 1);
    memset(ptr->item_value, 0, value_len + 1);

    memcpy(ptr->item_name, item_name, name_len);
    memcpy(ptr->item_value, buf, value_len);

    if (g_mlog_drv_debug_level & COMM_DEBUG_LEVEL)
    {
        printk(KERN_EMERG "%s %s %s\n",
               ptr->item_name, ptr->item_value, __FUNCTION__);
    }

    raw_spin_lock_irqsave(&mlog_comm_statistics_lock, flags);
    ret = mlog_comm_insert_item(ptr);
    raw_spin_unlock_irqrestore(&mlog_comm_statistics_lock, flags);

    if (ret < 0)
    {
        goto fail;
    }

    return;
fail:
    if (NULL != ptr->item_name)
    {
        kfree(ptr->item_name);
    }

    if (NULL != ptr->item_value)
    {
        kfree(ptr->item_value);
    }

    if (NULL != ptr)
    {
        kfree(ptr);
    }
}

/*****************************************************************************
 函 数 名  : mlog_process_yaffs_statistics_info
 功能描述  : yaffs信息统计处理函数
 输入参数  : void
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年6月10日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
void mlog_process_yaffs_statistics_info(void)
{
    /*report the yaffs stat info*/
    yaffs_dev_stat_t* yaffs_dev_stat_p = NULL;
    mlog_server_stat_info  stat_info;
    int i  = 0 ;
    int n = 0;
    int stat_item_value = -1;

    memset((void*)&stat_info, 0, sizeof(mlog_server_stat_info));

    yaffs_dev_stat_p = g_yaffs_dev_statis_info.yaffs_info_stat;
    yaffs_info_statistics();

    for (i = 0; i < MAX_FS_PARTITI_COUNT; i++)
    {
        if (yaffs_dev_stat_p->flags)
        {
            for (n = 0; n < sizeof(yaffs_dev_param) / MAX_ITEM_LEN; n++)
            {
                snprintf(stat_info.item_name, MAX_ITEM_LEN - 1,
                         "%s.%s.%s", "YAFFS", yaffs_dev_stat_p->name, yaffs_dev_param[n]);

                stat_item_value = *(unsigned int*)((char*)yaffs_dev_stat_p +
                                                   sizeof(yaffs_dev_stat_p->name) + n * sizeof(unsigned int) );
                snprintf(stat_info.item_value, MAX_ITEM_LEN - 1,
                         "%d", stat_item_value);

                mlog_stat_item_send(&stat_info);
            }
            msleep(MLOG_FLASH_DELAY_TIME_MS);
        }
        yaffs_dev_stat_p++ ;
    }
}

/*****************************************************************************
 函 数 名  : mlog_process_flash_statistics_info
 功能描述  : flash信息统计处理函数
 输入参数  : void
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年6月10日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
void mlog_process_flash_statistics_info(void)
{
    struct mlog_flash_info*  flash_item_info = NULL;
    mlog_server_stat_info  stat_info;

    memset((void*)&stat_info, 0, sizeof(mlog_server_stat_info));
#if 0 
    /*process efs */
    flash_efs_stat();
#endif 
    /*process mtd */
    while (1)
    {
        raw_spin_lock(&mlog_flash_statistics_lock);
        flash_item_info = mlog_flash_get_item();
        raw_spin_unlock(&mlog_flash_statistics_lock);

        if (NULL == flash_item_info)
        {
            break;
        }

        snprintf(stat_info.item_name, MAX_ITEM_LEN - 1,
                 "%s.BLOCK%d.earsetimes", "FLASH", flash_item_info->blockid);
        snprintf(stat_info.item_value, MAX_ITEM_LEN - 1,
                 "%d", flash_item_info->erase_num);
        mlog_stat_item_send(&stat_info);

        snprintf(stat_info.item_name, MAX_ITEM_LEN - 1,
                 "%s.BLOCK%d.writetimes", "FLASH", flash_item_info->blockid);
        snprintf(stat_info.item_value, MAX_ITEM_LEN - 1,
                 "%d", flash_item_info->write_num);
        mlog_stat_item_send(&stat_info);

        if (g_mlog_drv_debug_level & FLASH_DEBUG_LEVEL)
        {
            printk(KERN_EMERG "blk_id = %d,write = %d, erase = %d \n",
                   flash_item_info->blockid, flash_item_info->write_num,
                   flash_item_info->erase_num);
        }

        kfree(flash_item_info);
        flash_item_info = NULL;
        msleep(MLOG_FLASH_DELAY_TIME_MS);
    }
    return;
}
/*****************************************************************************
 函 数 名  : mlog_process_common_statistics_info
 功能描述  : 通用的统计处理函数
 输入参数  : 无
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年6月10日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
void mlog_process_common_statistics_info(void)
{

    struct mlog_comm_info_t* mlog_comm_info = NULL;
    unsigned long flags = 0;
    mlog_server_stat_info  stat_info;

    memset((void*)&stat_info, 0, sizeof(mlog_server_stat_info));
    while (1)
    {
        raw_spin_lock_irqsave(&mlog_comm_statistics_lock, flags);
        mlog_comm_info = mlog_comm_get_item();
        raw_spin_unlock_irqrestore(&mlog_comm_statistics_lock, flags);
        if (NULL == mlog_comm_info)
        {
            break;
        }

        snprintf(stat_info.item_name, MAX_ITEM_LEN - 1,
                 "%s.%s", "EVNET", mlog_comm_info->item_name);
        snprintf(stat_info.item_value, MAX_ITEM_LEN - 1,
                 "%s", mlog_comm_info->item_value);
        mlog_stat_item_send(&stat_info);

        if (g_mlog_drv_debug_level & COMM_DEBUG_LEVEL)
        {
            printk(KERN_EMERG "item = %s,value = %s\n",
                   mlog_comm_info->item_name, mlog_comm_info->item_value);
        }
        kfree(mlog_comm_info->item_name);
        kfree(mlog_comm_info->item_value);
        kfree(mlog_comm_info);
        mlog_comm_info = NULL;
        msleep(MLOG_DELAY_TIME_MS);
    }
}
