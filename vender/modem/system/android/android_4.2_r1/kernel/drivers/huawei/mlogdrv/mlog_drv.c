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
#include <linux/sched.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/time.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/poll.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/seq_file.h>
#include <linux/time.h>
#include <asm/uaccess.h>
#include <asm/io.h>

#include <linux/mlog_lib.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/debugfs.h>
#include <linux/workqueue.h>
#include "bsp_nvim.h"
#define  MLOG_BUF_LEN  (8 * 1024)



#define MLOG_MAX_MD_LEN  16
#define MLOG_MAX_LV_LEN  16
#define MLOG_MAX_TM_LEN  20
#define MLOG_MAX_HEAD_LEN  (MLOG_MAX_MD_LEN + MLOG_MAX_LV_LEN + MLOG_MAX_TM_LEN)
#define MLOG_MAX_LOG_LEN 100
#define MLOG_PRINT_BUF_LEN (MLOG_MAX_HEAD_LEN + MLOG_MAX_LOG_LEN)
#define MLOG_STAT_INTERVAL 30   /*unit: s*/
#define MLOG_TICKS_IN_SIX_MINUTE   (HZ * 60 * 6)
#define   NV_ID_SOFT_FACTORY_MLOG_CFG  (36)
static char  mlog_buf[MLOG_BUF_LEN];
static char *log_buf = mlog_buf;
static int   log_buf_len = MLOG_BUF_LEN;
static unsigned int log_rd_off;
static unsigned int log_wr_off;
#define LOG_BUF_MASK (log_buf_len - 1)
#define LOG_BUF(idx) (log_buf[(idx) & LOG_BUF_MASK])
static char print_buf[MLOG_PRINT_BUF_LEN];
extern unsigned int g_partition_shared;
extern int mlog_stat_item_send(mlog_server_stat_info *stat_info);

struct timer_list mlog_stat_timer;
struct work_struct stat_work;
static struct dentry *den = NULL;
static struct dentry *parent = NULL;
uint32 g_mlog_drv_debug_level;
static int proc_init_ok = FALSE;
static DEFINE_RAW_SPINLOCK(mlog_log_buf_lock);
DECLARE_WAIT_QUEUE_HEAD(mlog_log_wait);
void mlog_stat_timer_function(unsigned long lparam);
void stat_report_work(struct work_struct *work);
extern int yaffs_info_statistics(void);
extern void mlog_process_common_statistics_info(void);
extern void mlog_process_flash_statistics_info(void);
extern void mlog_process_yaffs_statistics_info(void);
extern void mlog_stat_netlink_init(void) ;
extern void mlog_statistic_info_init(void);
extern void mlog_statistic_info_destory(void);
extern void  smem_statistic_info_process(void);
extern int mlog_is_upgrade_mode(void);

extern void mlog_set_startup_mode(void);
 

extern void mlog_register_reboot_notifier(void);

extern void mlog_unregister_reboot_notifier(void);


/*****************************************************************************
 函数名称  : int mlog_is_factory_mode(void)
 功能描述  : 
 输入参数  : No
 输出参数  : No
 返 回 值  : No
 修改历史  : 
             1. 2014-09-23 :  00206465 qiaoyichuan created
*****************************************************************************/
int mlog_is_factory_mode(void)
{
    int ret = 0;
    static int mlog_ftm_mod = -1;
    if (-1 == mlog_ftm_mod)
    {
        ret = bsp_nvm_read(NV_ID_SOFT_FACTORY_MLOG_CFG, (unsigned char *)(& mlog_ftm_mod),
                           sizeof( unsigned int));
        if (0 != ret)
        {
            printk("get nv factory err \r\n" );
            return -1;
        }
    }
    return mlog_ftm_mod;
}
/*****************************************************************************
函数名称  : mlog_log_write_char
功能描述  :
输入参数  :
输出参数  :
返 回 值  :
修改历史  :
*****************************************************************************/
static void mlog_log_write_char(char c)
{
    LOG_BUF(log_wr_off) = c;
    log_wr_off++;
    if (log_wr_off - log_rd_off > log_buf_len)
    {
        log_rd_off = log_wr_off - log_buf_len;
    }
}

/*****************************************************************************
函数名称  : mlog_log_open
功能描述  :
输入参数  :
输出参数  :
返 回 值  :
修改历史  :
*****************************************************************************/
static int mlog_log_open(struct inode * inode, struct file * file)
{
    return 0;
}

/*****************************************************************************
函数名称  : mlog_log_release
功能描述  :
输入参数  :
输出参数  :
返 回 值  : No
修改历史  :
*****************************************************************************/
static int mlog_log_release(struct inode * inode, struct file * file)
{
    return 0;
}

/*****************************************************************************
函数名称  : mlog_log_read
功能描述  :
输入参数  :
输出参数  :
返 回 值  : No
修改历史  :
*****************************************************************************/
static ssize_t mlog_log_read(struct file *file, char __user *buf,size_t count, loff_t *ppos)
{
    ssize_t ret = -EINVAL;
    int error = 0;
    int i = 0;
    int bwr_eq_rd = 0;
    char c;
    DEFINE_WAIT(wait);

    for (;;) {
        prepare_to_wait(&mlog_log_wait, &wait, TASK_INTERRUPTIBLE);

        raw_spin_lock_irq(&mlog_log_buf_lock);
        bwr_eq_rd = (log_wr_off == log_rd_off);
        raw_spin_unlock_irq(&mlog_log_buf_lock);
        if (!bwr_eq_rd)
        {
            break;
        }

        if (file->f_flags & O_NONBLOCK)
        {
            ret = -EAGAIN;
            break;
        }

        if (signal_pending(current))
        {
            ret = -EINTR;
            break;
        }
        schedule();
    }
    finish_wait(&mlog_log_wait, &wait);

    if (bwr_eq_rd)
    {
        return ret;
    }

    raw_spin_lock_irq(&mlog_log_buf_lock);
    i = 0;

    while (!error && (log_wr_off != log_rd_off) && i < count) {
        c = LOG_BUF(log_rd_off);
        log_rd_off++;
        error = __put_user(c,buf);
        buf++;
        i++;
    }

    if (!error) {
        ret = i;
    }
    raw_spin_unlock_irq(&mlog_log_buf_lock);

    return ret;
}

/*****************************************************************************
函数名称  : mlog_log_poll
功能描述  :
输入参数  :
输出参数  :
返 回 值  : No
修改历史  :
*****************************************************************************/
static unsigned int mlog_log_poll(struct file *file, poll_table *wait)
{
    unsigned int ret = 0;

    poll_wait(file, &mlog_log_wait, wait);

    raw_spin_lock_irq(&mlog_log_buf_lock);

    if (log_wr_off != log_rd_off) {
        ret = POLLIN | POLLRDNORM;
    }

    raw_spin_unlock_irq(&mlog_log_buf_lock);

    return ret;
}

/*****************************************************************************
函数名称  : mlog_time_show
功能描述  :
输入参数  :
输出参数  :
返 回 值  : No
修改历史  :
*****************************************************************************/
static int mlog_time_show(struct seq_file *m, void *v)
{
    struct timespec uptime;

    do_posix_clock_monotonic_gettime(&uptime);
    monotonic_to_bootbased(&uptime);
    seq_printf(m, "%lu\n",
        (unsigned long) (uptime.tv_sec * MSEC_PER_SEC + uptime.tv_nsec / NSEC_PER_MSEC));
    return 0;
}
/*****************************************************************************
函数名称  : mlog_time_open
功能描述  :
输入参数  :
输出参数  :
返 回 值  :
修改历史  :
*****************************************************************************/
static int mlog_time_open(struct inode * inode, struct file * file)
{
    return single_open(file, mlog_time_show, NULL);
}

static const struct file_operations proc_mlog_log_operations = {
    .read        = mlog_log_read,
    .poll        = mlog_log_poll,
    .open        = mlog_log_open,
    .release    = mlog_log_release,
};

/*提供用户态获取于内核相同的时间*/
static const struct file_operations proc_mlog_time_operations = {
       .open        = mlog_time_open,
       .read        = seq_read,
       .llseek      = seq_lseek,
       .release     = single_release,
};
static int mlog_print_debug_set(void *data, u64 val)
{
    g_mlog_drv_debug_level = (int32_t)val;

    printk("[KERNEL] g_huawei_drv_debug = 0x%lx\n", g_mlog_drv_debug_level);

    return 0;
}

static int mlog_print_debug_get(void *data, u64 *val)
{

    *val = g_mlog_drv_debug_level;

    return 0;
}



DEFINE_SIMPLE_ATTRIBUTE(mlog_print_debug_fops, mlog_print_debug_get,
    mlog_print_debug_set, "%llu\n");
/*****************************************************************************
函数名称  : proc_mlog_log_init
功能描述  :
输入参数  :
输出参数  :
返 回 值  : No
修改历史  :
*****************************************************************************/
static int __init proc_mlog_log_init(void)
{
    struct proc_dir_entry *p = NULL;

    if(0 == mlog_is_factory_mode())
    {
        printk("LOG module will do nothing due to factory mode cuerrrently\n");
        return 0;
    }

    p = proc_create("mobilelog", S_IRUSR, NULL, &proc_mlog_log_operations);
    if(NULL == p)
    {
        printk(KERN_ERR "create /proc/mobilelog failure !\n");
        return -1;
    }

    p = proc_create("mlog_time", S_IRUSR, NULL, &proc_mlog_time_operations);
    if(NULL == p)
    {
        remove_proc_entry("mobilelog",  NULL );
        printk(KERN_ERR "create /proc/mlog_time failure !\n");
        return -1;
    }
   /*for debug level */
    parent = debugfs_create_dir("mlog", NULL);  
    if (!parent)  
    {
        printk(KERN_ERR "debugfs mlog error !\n");
        return -1;     
    }
    den = debugfs_create_file("mlog_print", 0644, parent, &g_mlog_drv_debug_level,     //codecheck no
        &mlog_print_debug_fops);

    INIT_WORK(&stat_work, stat_report_work);
    init_timer(&mlog_stat_timer);

    mlog_set_startup_mode();

    if (mlog_is_upgrade_mode())
    {
        printk("LOG module will do nothing due to update mode cuerrrently\n");
        return 0 ;
    }

    mlog_statistic_info_init();
    mlog_stat_netlink_init();
    mlog_stat_timer.function = mlog_stat_timer_function;  
    mlog_stat_timer.data = (unsigned int)(&mlog_stat_timer);
    mlog_stat_timer.expires = jiffies + MLOG_STAT_INTERVAL * HZ;       
    add_timer(&mlog_stat_timer);
#if 0 
    mlog_register_reboot_notifier();
#endif

    proc_init_ok = TRUE;
    printk(KERN_ERR"init stat time %s %d \n", __FUNCTION__,__LINE__);


    return 0;
}

void mlog_process_statistic_info(void) 
{

  /*some info acqu need trigger*/
  smem_statistic_info_process();


  /*report the stat info to uplayer*/
  mlog_process_common_statistics_info();
  mlog_process_flash_statistics_info();
  mlog_process_yaffs_statistics_info();
  
}

/*****************************************************************************
函数名称  : stat_report_work
功能描述  :
输入参数  :
输出参数  :
返 回 值  : No
修改历史  :
*****************************************************************************/
void stat_report_work(struct work_struct *work)
{
  mlog_process_statistic_info();
}


/*****************************************************************************
函数名称  : mlog_stat_timer_function
功能描述  :
输入参数  :
输出参数  :
返 回 值  : No
修改历史  :
*****************************************************************************/
void mlog_stat_timer_function(unsigned long lparam)
{   
    schedule_work(&stat_work);
    mod_timer(&mlog_stat_timer,  jiffies + MLOG_STAT_INTERVAL * HZ);
}


/*****************************************************************************
函数名称  : proc_mlog_log_exit
功能描述  :
输入参数  :
输出参数  :
返 回 值  : No
修改历史  :
*****************************************************************************/
static  void __exit proc_mlog_log_exit(void)
{
    if(timer_pending(&mlog_stat_timer))
    {
        del_timer_sync(&mlog_stat_timer);
    }

    mlog_statistic_info_destory();
    if (den )
    {
        debugfs_remove(den);
    }
    if (parent )
    {
    debugfs_remove(parent);
    }
   
    remove_proc_entry("mlog_time",  NULL );
    remove_proc_entry("mobilelog",  NULL );
#if 0
    mlog_unregister_reboot_notifier();
#endif
}

typedef struct
{
    char         *mloglv_str;
    mlog_lv_eum   mloglv;
}mlog_lv_info_t;

static mlog_lv_info_t mlog_lv_str[] =
{
    {"NONE" , mlog_lv_none  },
    {"FATAL", mlog_lv_fatal },
    {"ERROR", mlog_lv_error },
    {"WARN" , mlog_lv_warn  },
    {"INFO" , mlog_lv_info  },
    {NULL   , mlog_lv_none  }
};

/*****************************************************************************
函数名称  : mlog_get_log_lv_string
功能描述  :
输入参数  :
输出参数  :
返 回 值  : No
修改历史  :
*****************************************************************************/
static char *mlog_get_log_lv_string(mlog_lv_eum mlog_lv)
{
    int i = 0;

    while(mlog_lv_str[i].mloglv_str != 0 )
    {
        if(mlog_lv_str[i].mloglv == mlog_lv)
        {
            break;
        }
        i++;
    }

    return mlog_lv_str[i].mloglv_str;
}

/*****************************************************************************
函数名称  : mlog_print
功能描述  :
输入参数  :
输出参数  :
返 回 值  : No
修改历史  :
*****************************************************************************/
asmlinkage void mlog_print(char *module, mlog_lv_eum loglv,const char *fmt,...)
{
    int printed_len = 0;
    unsigned long flags;
    char *p ;
    unsigned wake_up = 0;
    va_list args;
    struct timespec uptime;

    if((NULL == module) || ( NULL == fmt))
    {
        return;
    }
    /*PROC未创建成功，返回*/	
    if(FALSE == proc_init_ok)
    {
        return;
    }

    local_irq_save(flags);
    if (!raw_spin_trylock(&mlog_log_buf_lock))
    {
        local_irq_restore(flags);
        return;
    }
    do_posix_clock_monotonic_gettime(&uptime);
    monotonic_to_bootbased(&uptime);
    snprintf(print_buf, MLOG_MAX_HEAD_LEN , "[%011lu][%s][%s]:", 
        ((uptime.tv_sec * 1000) + (uptime.tv_nsec / 1000000)),/*将秒和纳秒转换成毫秒*/
        module,mlog_get_log_lv_string(loglv));
    printed_len = strlen(print_buf);
    va_start(args, fmt);
    vsnprintf(print_buf + printed_len , MLOG_MAX_LOG_LEN - 1, fmt, args);
    printed_len = strlen(print_buf);
    va_end(args);
    //若调用者log行尾未未增加换行，帮其增加
    if('\n' != print_buf[printed_len - 1])
    {
        print_buf[printed_len] = '\n';
        printed_len++;
        print_buf[printed_len] = '\0';
    }
    printed_len++;

    p = print_buf;
    for (; *p; p++) {
        mlog_log_write_char(*p);
    }

    wake_up = (log_wr_off == log_rd_off);
    if (!wake_up) {
         wake_up_interruptible(&mlog_log_wait);
    }

    raw_spin_unlock(&mlog_log_buf_lock);
    local_irq_restore(flags);

    return;
}
EXPORT_SYMBOL(mlog_print);

module_init(proc_mlog_log_init);
module_exit(proc_mlog_log_exit);


