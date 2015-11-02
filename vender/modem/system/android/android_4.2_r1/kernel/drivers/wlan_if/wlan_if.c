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

/*===========================================================================

                        linux 系统头文件

===========================================================================*/
#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/mm.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/skbuff.h>
#include <linux/netlink.h>
#include <linux/moduleparam.h>
#include <net/sock.h>
#include <linux/delay.h>
#include <linux/mutex.h>
#include <linux/spinlock.h>

#ifdef CONFIG_HAS_WAKELOCK
#include <linux/wakelock.h>  /* 采用android标准休眠接口 */
#endif

/*===========================================================================

                        依赖平台的头文件

===========================================================================*/

#include "bsp_softtimer.h"    
#include "wlan_if.h"
#include "wlan_drv_if.h"
#include "wlan_utils.h"

/*===========================================================================

                        公用数据结构声明(linux系统的)
===========================================================================*/
#define  WIFI_AT_DEVICE             "wifi_at_dev"
#define  WIFI_DELAY_FOR_NL_SEND     msecs_to_jiffies(1)

#define  WIFI_SHUTDOWN_SH           "/system/bin/%s/exe/wifi_poweroff_%u.sh"
#define  WIFI_STARTUP_SH            "/system/bin/%s/exe/wifi_poweron_%u.sh"
#define  WIFI_STARTUP_VSDB_SH       "/system/bin/%s/exe/wifi_poweron_%u_vsdb.sh"

#define  WIFI_MAX_EVENT_QUEUE_LEN       (20)   /* 队列最大长度 */
#define  USER_WIFI_TIMEOUT_EVENT        (1)    /* netlink 事件头 */
#define  WIFI_USER_ID_MAX               (0xFFFFFFFF) /* netlink用户进程初始值 */
#define  WIFI_WL_ARGS_MAX               (3)  /* wl命令参数最大个数 */
#define  WIFI_MICROSECONDS              (1000) /* 1秒= 1000毫秒 */
#define  WIFI_COMMON_LEN                (64)
#define  CHIP_STUS_CHECK_INTERVAL       (5 * HZ)  /* WiFi芯片状态检测时间 */
#define  CHIP_STUS_CHECK_TIMES          (3)  /* WiFi芯片状态检测次数 */
#define  USER_WIFI_FIRMWARE_DATA_DEAD_EVENT  (32)  /* 数据通路、命令控制通路异常事件 */


#define ERROR         (-1)
#define OK            0

typedef enum 
{
    USER_WIFI_TMR_AP  = 0,  /* 超时子事件，ap超时 */
    USER_WIFI_TMR_STA = 1,  /* 超时子事件，sta超时 */
    USER_WIFI_TMR_MAX,
}WIFI_USER_TMR_ID;


/* 用户层事件，如10分钟wifi超时 */
typedef struct _wifi_user_event
{
    unsigned int eventId;
    unsigned int eventVal;
} WIFI_USER_EVENT;

/* 事件节点 */
typedef struct _wifi_event_node 
{
    struct list_head list;
    int len;
    char data[0];
}WIFI_EVENT_NODE_STRU;


/* linux 消息队列 */
typedef struct _wifi_msg_ctrl
{
    struct sock   *dev_sock;  //netlink socket;
    unsigned int  user_pid;   //user task pid
    spinlock_t queue_lock;  /*lint !e43*/
    struct delayed_work event_work;        
    struct list_head event_queue;
    unsigned int queue_len;
} WIFI_MSG_CTRL_STRU;

static struct mutex wlan_ioc_mutex;
static struct mutex wlan_pwr_mutex;

WLAN_STATUS_STU wlan_status = {0};    /* 用来记录5秒时间内错误的状态 */
static WLAN_STATUS_STU wlan_count = {0};    /* 用来记录错误的次数 */
spinlock_t wlan_status_lock;

/* 消息处理函数声明 */
static inline WIFI_EVENT_NODE_STRU *wifi_event_dequeue(WIFI_MSG_CTRL_STRU *pCtrl);
static inline int wifi_eventq_is_empty(WIFI_MSG_CTRL_STRU *pCtrl);/*lint !e402*/
static WIFI_EVENT_NODE_STRU *wifi_event_node_alloc(WIFI_MSG_CTRL_STRU *pCtrl,void * data, int len);
static inline void wifi_event_enqueue(WIFI_MSG_CTRL_STRU *pCtrl,WIFI_EVENT_NODE_STRU *pNode);
static inline void wifi_event_node_free(WIFI_EVENT_NODE_STRU *pNode);
static void wifi_event_send(struct work_struct *work);

/* 应用层解析函数声明 */
static int wifi_dev_register(void);  /*lint !e402*/
static ssize_t wifi_at_device_store(struct device *dev, struct device_attribute *attr,const char *buf, size_t count); /*lint !e1075 !e31 !e10*/
static void wifi_netlink_chunnel_uninit(void);
int wifi_event_send_tasklet_mode(void *data, int len);


#define WIFI_MSG_CTRL_INIT(pCtrl)  \
{\
    INIT_LIST_HEAD(&(pCtrl->event_queue));    \
    pCtrl->queue_len = 0; \
    INIT_DELAYED_WORK(&(pCtrl->event_work), wifi_event_send); \
    spin_lock_init(&(pCtrl->queue_lock)); \
    pCtrl->dev_sock = netlink_kernel_create(&init_net,NETLINK_USER_WIFI_EVENT, \
    0,(void*)wifi_event_input,NULL,THIS_MODULE); \
    pCtrl->user_pid = (unsigned int)(WIFI_USER_ID_MAX); \
}

#define WIFI_MSG_CTRL_UNINIT(pCtrl)  \
{\
    cancel_delayed_work_sync(&(pCtrl->event_work)); \
    if (pCtrl->dev_sock) \
    {\
        netlink_kernel_release(pCtrl->dev_sock); \
    }\
    pCtrl->user_pid = (unsigned int)(WIFI_USER_ID_MAX); \
}

#define WIFI_EVENT_PATH_IS_READY(pCtrl)  (pCtrl->user_pid != (unsigned int)WIFI_USER_ID_MAX)


DEVICE_ATTR(wifi_at_dev, 0666, NULL, wifi_at_device_store); /*lint !e65 !e133 !e10*/
DECLARE_TASKLET(wifi_send_event_name, wifi_event_send,0); /*lint !e64  !e628*/


/* 设备节点变量 */

static struct platform_device *wifi_platform_device = NULL;
static WIFI_MSG_CTRL_STRU  *wifi_user_msg_block = NULL;

static struct attribute *wifi_dev_attributes[] = {
    &dev_attr_wifi_at_dev.attr,
    NULL
};

static const struct attribute_group wifi_dev_group = {
    .attrs = wifi_dev_attributes,
};

typedef enum 
{
    TIMER_INACTIVE = 0,   /* 删除后状态 */
    TIMER_ACTIVE  = 1,     /* 激活状态，添加到list后状态 */
}timer_status;

typedef struct 
{
    struct softtimer_list timer_list;
    timer_status   timer_stus;
}wifi_timer;

typedef struct _wlan_pid
{
    int pid;
    const struct pid *p_pid;
}wlan_pid_t;


/* 目前用到的两个计时器 */
static wifi_timer wlan_timer[USER_WIFI_TMR_MAX];

/* WiFi芯片状态检测定时器 */
struct timer_list wlan_stus_detect_timer;


/*产品ID对应值*/
int g_product_index = 0;
EXPORT_SYMBOL(g_product_index);

/* 低功耗相关 */
#ifdef CONFIG_HAS_WAKELOCK
/* wakelock 变量 */
static struct wake_lock wlan_wakelock;
#define  WIFI_WAKELOCK_NAME         "wlan_if"
#define  WIFI_WAKE_TIME   5          /* 事件发送后，延时5s */
#endif

#ifdef CONFIG_USB_GADGET_SNPS_DWC_OTG
extern void dwc_otg_wifi_pwctrl_down(void);
#endif
/* for sdio interface */
#ifdef CONFIG_MMC
extern int mshci_wifi_power_on(void);
extern int mshci_wifi_power_off(void);
#endif
#define MAX_RETRY  10
static int wlan_pwroff_status = 0; /* 默认开启 */
static int wlan_ioctl_status = 0; /* ioctl 通路是否返回,0表示应用层的wl处理完成 */

wlan_pid_t g_wlan_msg_monitor = {-1, NULL}; /* msg_monitor 进程的 pid */



/*===========================================================================

                         依赖具体芯片平台的数据结构定义、外部引用
===========================================================================*/


/*===========================================================================

                        函数实现部分

===========================================================================*/

/*****************************************************************************
 函数名称  : int wlan_get_poweroff_status()
 功能描述  : 获取当前的关机状态
 输入参数  : NA
 输出参数  : NA
 返 回 值  : 0:不处于关机状态
             1:正在关机
*****************************************************************************/
int wlan_get_poweroff_status()
{
    return wlan_pwroff_status;
}
EXPORT_SYMBOL(wlan_get_poweroff_status);

/*****************************************************************************
 函数名称  : int wlan_set_poweroff_status(int poweroff_status)
 功能描述  : 设置关机状态
 输入参数  : NA
 输出参数  : NA
 返 回 值  : NA
*****************************************************************************/
int wlan_set_poweroff_status(int poweroff_status)
{
    mutex_lock(&wlan_pwr_mutex);
    wlan_pwroff_status = poweroff_status;
    PLAT_WLAN_INFO("poweroff_status = %d",poweroff_status);
    mutex_unlock(&wlan_pwr_mutex);
    return 0;
}
EXPORT_SYMBOL(wlan_set_poweroff_status);

/*****************************************************************************
 函数名称  : int wlan_get_ioctl_status()
 功能描述  : 获取当前的ioctl通道状态
 输入参数  : NA
 输出参数  : NA
 返 回 值  : 0:不处于关机状态
             1:正在关机
*****************************************************************************/
int wlan_get_ioctl_status() 
{
    PLAT_WLAN_INFO("wlan_ioctl_status = %d",wlan_ioctl_status);

    return wlan_ioctl_status;
}
EXPORT_SYMBOL(wlan_get_ioctl_status);

/*****************************************************************************
 函数名称  : int wlan_count_ioctl_status()
 功能描述  : 加1或者减1，魔术字0xff设置wlan_ioctl_status = 0 
 输入参数  : NA
 输出参数  : NA
 返 回 值  : NA
*****************************************************************************/
int wlan_count_ioctl_status(int ioctl_step) 
{
    mutex_lock(&wlan_ioc_mutex);
    
    if (0xff == ioctl_step)
    {
        wlan_ioctl_status = 0;
    }
    else 
    {
        wlan_ioctl_status += (ioctl_step);
    }
    mutex_unlock(&wlan_ioc_mutex);
    return 0;
}
EXPORT_SYMBOL(wlan_count_ioctl_status);


/*****************************************************************
Function: wifi_set_product_id
Description:
    通过sh脚本，向内核传递产品id号，用于RSSI补偿。
Input:
    product_id
Output:
    N/A
Return:
*******************************************************************/
void wifi_set_product_id(int product_id)
{
    g_product_index = product_id;
    return;
}
EXPORT_SYMBOL(wifi_set_product_id);

/*****************************************************************
Function: wlan_stus_timer_function
Description:
    WiFi状态检测函数的定时器的回调函数。
Input:
    N/A
Output:
    N/A
Return:
*******************************************************************/

static void wlan_stus_timer_function()
{
    int i = 0;
    unsigned long flgs = 0;
    int reset_flag = 0;
    WIFI_USER_EVENT event= {0};
    unsigned int *stus = &wlan_status;
    unsigned int *cnt = &wlan_count;
    int stulen = sizeof(WLAN_STATUS_STU) / sizeof(unsigned int);
    spin_lock_irqsave(&wlan_status_lock, flgs);
    /* 以下代码是循环检测结构体的成员变量，成员变量必须为unsigned int类型。
       如果5秒内成员变量的status状态大于0，count计数一次 */
    for(i = 0;i < stulen;i++)
    {
        if(0 < *(stus + i))
        {
            (*(cnt + i))++; 
            *(stus + i) = 0;
        }
        else
        {
            *(cnt + i) = 0;
        }
        if(CHIP_STUS_CHECK_TIMES <= *(cnt + i))
        {  
            reset_flag = 1;
            break;
        }
        //PLAT_WLAN_INFO("wlan_count:%d is %d",i,*(cnt + i));
    }
    spin_unlock_irqrestore(&wlan_status_lock, flgs);

    if(reset_flag)
    {   
        memset(&wlan_count, 0x00, sizeof(WLAN_STATUS_STU));
        event.eventId = USER_WIFI_FIRMWARE_DATA_DEAD_EVENT;     /* 命令通路和控制通路使用这一个事件，上层处理方式一样 */
        wifi_event_send_tasklet_mode((void *)&event,sizeof(WIFI_USER_EVENT)); 
        PLAT_WLAN_ERR("FW error,WiFi chip will restart!!!");
    }   
    /* 循环启动定时器 */
    mod_timer(&wlan_stus_detect_timer, jiffies + CHIP_STUS_CHECK_INTERVAL);
    return;
}

/*****************************************************************
Function: wlan_stus_timer_init
Description:
    WiFi状态检测函数的定时器的初始化函数。
Input:
    N/A
Output:
    N/A
Return:
*******************************************************************/

static void wlan_stus_timer_start()
{
    init_timer(&wlan_stus_detect_timer);
    wlan_stus_detect_timer.function = wlan_stus_timer_function;  
    wlan_stus_detect_timer.data = NULL;
    wlan_stus_detect_timer.expires = CHIP_STUS_CHECK_INTERVAL;
    mod_timer(&wlan_stus_detect_timer, jiffies + CHIP_STUS_CHECK_INTERVAL);
    PLAT_WLAN_INFO("wlan_stus_timer_started!!!!");
    return;
}

/***********************************************************************************
 Function:         wifi_timer_delete
 Description:      删除的timer
 Calls:
 Input:      USER_WIFI_TIMEID_MAX:删除所有的；其余的值，删除指定的
 Output:            NA
 Return:            NULL or node
                  
 ************************************************************************************/
static void wifi_timer_delete(WIFI_USER_TMR_ID timer_id )
{
    if(  USER_WIFI_TMR_MAX <= timer_id )
    {
        PLAT_WLAN_INFO(" timer id error.id = %d",timer_id);
        return;
    }
    
    PLAT_WLAN_INFO(" enter, wlan_timer[%d].timer_stus = %d",timer_id,
                               wlan_timer[timer_id].timer_stus);
       
    if( TIMER_INACTIVE == wlan_timer[timer_id].timer_stus )
    {
        PLAT_WLAN_INFO(" timer TIMER_INACTIVE,id = %d",timer_id);
        return;
    }
    (void)bsp_softtimer_delete(&wlan_timer[timer_id].timer_list);

    (void)bsp_softtimer_free(&wlan_timer[timer_id].timer_list);

    wlan_timer[timer_id].timer_stus = TIMER_INACTIVE;
    PLAT_WLAN_INFO("exit, timer delete success,wlan_timer[%d].timer_stus = %d",
                             timer_id,wlan_timer[timer_id].timer_stus);
}

/***********************************************************************************
 Function:          wifi_timer_callback
 Description:      计时超时执行函数
 Calls:
 Input:              data: 
 Output:            NA
 Return:            NA                  
 ************************************************************************************/
static irqreturn_t wifi_timer_callback(unsigned int data)
{
    WIFI_USER_EVENT event;
    WIFI_USER_TMR_ID timer_id = (WIFI_USER_TMR_ID)data;
    
    PLAT_WLAN_INFO("enter,timer_id = %d",timer_id);
    
#ifdef CONFIG_HAS_WAKELOCK
    wake_lock_timeout(&wlan_wakelock, WIFI_WAKE_TIME*HZ); /*extend 5s*/
#endif

    if(  USER_WIFI_TMR_MAX <= timer_id )
    {
        PLAT_WLAN_INFO("param error  %d",timer_id);
        return IRQ_NONE;
    }    
    wifi_timer_delete(timer_id);

    event.eventId = USER_WIFI_TIMEOUT_EVENT;
    event.eventVal = timer_id;
    (void)wifi_event_send_tasklet_mode((void *)&event,sizeof(WIFI_USER_EVENT));
    PLAT_WLAN_INFO("event.eventId = %d, event.eventVal= %d",event.eventId, event.eventVal);    

    return IRQ_NONE;
}


/***********************************************************************************
 Function:          wifi_start_timer
 Description:     启动一个定时器
 Calls:
 Input:              seconds: time out value，等于0时，表示删除定时器
 Output:            NA
 Return:            0/-1                  
 ************************************************************************************/
static int wifi_timer_start(unsigned int seconds,WIFI_USER_TMR_ID timer_id)
{
    s32 ret = 0;
    if(  USER_WIFI_TMR_MAX <= timer_id )
    {
        PLAT_WLAN_INFO(" timeid is over range,time id = %d",timer_id);
        return ERROR;
    }
    PLAT_WLAN_INFO("set timer to %d seconds,time id = %d",seconds,timer_id);

    /* 一类timer只有一个实例,再次配置时，把之前的删除掉,重新计时 */
    wifi_timer_delete(timer_id);
    
    if ( seconds > 0)
    {
        /* 配置softtimer_list */
        wlan_timer[timer_id].timer_list.func = wifi_timer_callback; /*lint !e64*/
        wlan_timer[timer_id].timer_list.para = timer_id;   /* 用id作为子事件参数 */
        wlan_timer[timer_id].timer_list.timeout = seconds * WIFI_MICROSECONDS;
        wlan_timer[timer_id].timer_list.wake_type = SOFTTIMER_WAKE;

        ret = bsp_softtimer_create(&(wlan_timer[timer_id].timer_list));     
        if( ret )
        {
            PLAT_WLAN_INFO(" timer creat fail");
            return ERROR;
        }
        bsp_softtimer_add(&(wlan_timer[timer_id].timer_list));
        wlan_timer[timer_id].timer_stus = TIMER_ACTIVE;
        PLAT_WLAN_INFO("timer add success, time = %d seconds,wlan_timer[%d].timer_stus = %d",
                      seconds,timer_id,wlan_timer[timer_id].timer_stus);
    }
    return OK;
}

/*****************************************************************************
 函数名称  : static int wifi_powerup_rtl_8192()
 功能描述  : RTL8192 启动 wifi
 输入参数  : NA
 输出参数  : NA
 返 回 值  : 返回启动结果
*****************************************************************************/
static int wifi_powerup_rtl_8192(void)
{
    int ret = 0;
   
#ifdef CONFIG_MMC
    ret = mshci_wifi_power_on();
    if (0 != ret)
    {
        PLAT_WLAN_ERR("wifi power up fail");
    }
#endif

    return ret;
}

/*****************************************************************************
 函数名称  : static int wifi_powerup_bcm_43241()
 功能描述  : BCM43241 启动 wifi
 输入参数  : NA
 输出参数  : NA
 返 回 值  : 返回启动结果
*****************************************************************************/
static int wifi_powerup_bcm_43241(unsigned int chip_name, char *chip_supplier)
{
    int ret = 0;
    char wifi_run_sh[WIFI_COMMON_LEN] = {NULL};    /*执行的SH脚本*/

    wlan_set_poweroff_status(0);/* 1 表示正在关机，ioctl通路关闭；0 开启 */
    
    snprintf(wifi_run_sh, WIFI_COMMON_LEN, WIFI_STARTUP_SH, chip_supplier, chip_name);
    PLAT_WLAN_INFO("wifi_at_dev:wifi_run_sh is %s!", wifi_run_sh);
    ret = wlan_run_cmd(wifi_run_sh);
    if(0 != ret)
    {
        PLAT_WLAN_ERR("ret = %d, run cmd:%s", (int)ret,  wifi_run_sh);
    }

    return ret;
}

/*****************************************************************************
函数名称  : static int wifi_powerup()
功能描述  : wifi上电
输入参数  : NA
输出参数  : NA
返 回 值  : 返回启动结果
*****************************************************************************/
static int wifi_powerup(unsigned int chip_name, char *chip_supplier)
{
    int ret = 0;
        
    switch(chip_name)
    {
        case WIFI_CHIP_NAME_8192:
            ret = wifi_powerup_rtl_8192();
            break;
        case WIFI_CHIP_NAME_43241:
            ret = wifi_powerup_bcm_43241(chip_name, chip_supplier);
            break;
        default:
        {
            PLAT_WLAN_ERR("no process of this chip:%d ", chip_name);    
            break;
        }
    }

    return ret;
}

/*****************************************************************************
函数名称  : static int wifi_shutdown_rtl_8192()
功能描述  : RTL8192  wifi 下电
输入参数  : NA
输出参数  : NA
返 回 值  : 返回启动结果
*****************************************************************************/
static int wifi_shutdown_rtl_8192(void)
{
    int ret = 0;
    
#ifdef CONFIG_MMC
    ret = mshci_wifi_power_off();
    if (0 != ret)
    {
        PLAT_WLAN_ERR("wifi power down fail");
    }
#endif

    return ret;
}

/*****************************************************************************
函数名称  : static int wifi_shutdown_rtl_8192()
功能描述  : RTL8192  wifi 下电
输入参数  : NA
输出参数  : NA
返 回 值  : 返回启动结果
*****************************************************************************/
static int wifi_shutdown_bcm_43241(unsigned int chip_name, char *chip_supplier)
{
    int ret = 0;
    int index = 0;
    char wifi_run_sh[WIFI_COMMON_LEN] = {NULL};    /*执行的SH脚本*/

    wlan_set_poweroff_status(1);/* 1 表示正在关机，ioctl通路关闭；0 开启 */

    snprintf(wifi_run_sh, WIFI_COMMON_LEN, WIFI_SHUTDOWN_SH, chip_supplier, chip_name);
    PLAT_WLAN_INFO("wifi_at_dev:wifi_run_sh is %s!", wifi_run_sh);
    
    for(;index < MAX_RETRY;index++)
    {
        int ioctl_status = wlan_get_ioctl_status();
        PLAT_WLAN_ERR("ioctl_status = %d ",ioctl_status);
        if (0 == ioctl_status)
        {
         break;
        }
        msleep(500); 
    }

    PLAT_WLAN_ERR("index = %d ", index);
    /* 等待了最大延时5s，ioctl还是未知，执行下电操作 */
#ifdef CONFIG_USB_GADGET_SNPS_DWC_OTG
    /* call hisc power off */
    dwc_otg_wifi_pwctrl_down();
#endif
    ret = wlan_run_cmd(wifi_run_sh);
    if(0 != ret)
    {
        PLAT_WLAN_ERR("ret = %d, run cmd:%s", (int)ret,  wifi_run_sh);
    }

    return ret;
}

/*****************************************************************************
函数名称  : static int wifi_shutdown()
功能描述  : wifi 下电
输入参数  : NA
输出参数  : NA
返 回 值  : 返回启动结果
*****************************************************************************/
static int wifi_shutdown(unsigned int chip_name, char *chip_supplier)
{
    int ret = 0;
 
    switch(chip_name)
    {
        case WIFI_CHIP_NAME_8192:
             ret = wifi_shutdown_rtl_8192();
             break;
        case WIFI_CHIP_NAME_43241:
            ret = wifi_shutdown_bcm_43241(chip_name, chip_supplier);
            break;
        default:
        {
            PLAT_WLAN_ERR("no process of this chip:%d ", chip_name);
            break;
        }
    }  
    
    return ret;
}

 /***********************************************************************************
 Function:          wifi_at_device_store
 Description:      负责解析应用层传递的命令字符
 Calls:
 Input:              pCtrl: control block
                Data: date buffer
                len: data len
 Output:            NA
 Return:            NULL or node
                  
 ************************************************************************************/
static ssize_t wifi_at_device_store (struct device *dev,struct device_attribute *attr,const char *buf, size_t count) /*lint !e31 !e10*/
{
    unsigned int ap_seconds = 0;
    int cmd = 0;

    unsigned int chip_name = 0;  /*芯片名称，例如43241*/
    char chip_supplier[WIFI_COMMON_LEN] = {0};    /*芯片供应商名称，例如:wifi_brcm*/
    char wifi_run_sh[WIFI_COMMON_LEN] = {NULL};    /*执行的SH脚本*/
    
    if ((NULL == dev) || (NULL == attr) || (NULL == buf))
    {
        PLAT_WLAN_INFO("wifi_at_dev: invalid para!");
        return count;
    }
    PLAT_WLAN_INFO("receive msg from user is %s", buf);
    
    if(sscanf(buf,"%d",&cmd) != 1)
    {
        PLAT_WLAN_ERR(" wifi_at_dev: %s is error!",buf);
        return count;
    }
    switch(cmd)
    {
        case WIFI_AP_SHUTDOWN_TIMEOUT_SET:        /* AP 自动关闭计时 */
        {
            (void)sscanf(buf,WIFI_AP_SHUTDOWN_TIMEOUT_SET_STR,&ap_seconds) ;
            if(0 != wifi_timer_start(ap_seconds,USER_WIFI_TMR_AP))
            {
                PLAT_WLAN_ERR("wifi_at_dev: failed to execute program wifi_ap_start_timer! "); 
            }
            break; 
        }
        case WIFI_STA_SCAN_TIMEOUT_SET:  
        {
            (void)sscanf(buf,WIFI_TIMER_SET_CMD_STR, &ap_seconds);
            if(0 != wifi_timer_start(ap_seconds,USER_WIFI_TMR_STA))
            {
                PLAT_WLAN_ERR("failed to execute wifi_start_timer!");
            }
            break;
        }
        case WIFI_STARTUP_SET:
        {
             /*获取供应商ID和芯片型号ID*/
            if(sscanf(buf,WIFI_STARTUP_SET_STR,&chip_name,chip_supplier) != 2)
            {
                PLAT_WLAN_ERR("wifi_at_dev: %s is error!",buf);
                return count;
            }
            
            (void)wifi_powerup(chip_name, chip_supplier);          
            break;
        }
        case WIFI_STARTUP_VSDB_SET:
        {
            wlan_set_poweroff_status(0);/* 1 表示正在关机，ioctl通路关闭；0 开启 */
            /*获取供应商ID和芯片型号ID*/
            if(sscanf(buf,WIFI_STARTUP_VSDB_SET_STR,&chip_name,chip_supplier) != 2)
            {
                PLAT_WLAN_INFO("wifi_at_dev: %s is error!",buf);
                return count;
            }
            snprintf(wifi_run_sh, WIFI_COMMON_LEN, WIFI_STARTUP_VSDB_SH, chip_supplier, chip_name);
            PLAT_WLAN_INFO("[info][WIFI_STARTUP_VSDB_SET]wifi_at_dev:wifi_run_sh is %s!",wifi_run_sh);
            (void)wlan_run_cmd(wifi_run_sh);
            break;
        }
        case WIFI_SHUTDOWN_SET:
        {
            /*获取供应商ID和芯片型号ID*/
            if(sscanf(buf,WIFI_SHUTDOWN_SET_STR,&chip_name,chip_supplier) != 2)
            {
                PLAT_WLAN_INFO("wifi_at_dev: %s is error!",buf);
                return count;
            }
           
            (void)wifi_shutdown(chip_name, chip_supplier);
            break;
        }
        case WIFI_MONITOR_PID_SET:
        {
            (void)sscanf(buf,WIFI_MONITOR_PID_SET_STR,&g_wlan_msg_monitor.pid) ;
            
            rcu_read_lock();
            g_wlan_msg_monitor.p_pid = get_pid(find_vpid(g_wlan_msg_monitor.pid));    
            rcu_read_unlock();
            PLAT_WLAN_INFO("wifi MONITOR_PID = %d, p = %p", g_wlan_msg_monitor.pid, g_wlan_msg_monitor.p_pid);
            break;
        }
        case WIFI_LOG_LEVEL_SET:
        {
            int tmp_val = 0;
            (void)sscanf(buf,WIFI_LOG_LEVEL_SET_STR,&tmp_val) ;           
            (void)wlan_set_log_flag(tmp_val);
            break;
        }   
        default:
        {
            PLAT_WLAN_INFO("wifi_at_dev: %s is not support!", buf);
            break;
        }
    }
    return count;
}

/***********************************************************************************
 Function:          wifi_dev_register
 Description:      sysfs文件注册
 Calls:
 Input:              pCtrl: control block
                Data: date buffer
                len: data len
 Output:            NA
 Return:            NULL or node
                  
************************************************************************************/
static int wifi_dev_register(void) /*lint !e10*/
{
    int ret = ERROR;

    wifi_platform_device = platform_device_register_simple(WIFI_AT_DEVICE,-1, NULL, 0);

    if( NULL == wifi_platform_device )
    {
        PLAT_WLAN_INFO("create wifi_platform_device failed.");
        return ERROR;
    }

    ret = sysfs_create_group(&wifi_platform_device->dev.kobj, &wifi_dev_group);
    if (OK != ret)
    {
        PLAT_WLAN_INFO(" create sdio sys filesystem node failed.");
        return ERROR;
    }

    return OK;
}

 /*****************************************************************************
  函数名称  : WLAN_RETURN_TYPE wifi_at_device_unregister(void)
  功能描述  : 释放wifi_platform_device设备占用资源
  输入参数  : NA
  输出参数  : NA
  返 回 值  : WLAN_RETURN_TYPE
 *****************************************************************************/
 static WLAN_RETURN_TYPE wifi_at_device_unregister(void)
 {   
     ASSERT_NULL_POINTER(wifi_platform_device, WLAN_SUCCESS);
     sysfs_remove_group(&wifi_platform_device->dev.kobj, &wifi_dev_group);
     platform_device_unregister(wifi_platform_device);
     wifi_platform_device = NULL;
     return WLAN_SUCCESS;
 }

 /***********************************************************************************
 Function:          wifi_event_node_alloc
 Description:      Construct a buffer node
 Calls:
 Input:              pCtrl: control block
                Data: date buffer
                len: data len
 Output:            NA
 Return:            NULL or node
                  
 ************************************************************************************/
static WIFI_EVENT_NODE_STRU *wifi_event_node_alloc(WIFI_MSG_CTRL_STRU *pCtrl,void * data, int len)
{
    WIFI_EVENT_NODE_STRU *pNode = NULL;

    PLAT_WLAN_INFO("enter");

    if ( WIFI_MAX_EVENT_QUEUE_LEN < pCtrl->queue_len)
    {
        PLAT_WLAN_INFO(" max queue len is reached! ");
        return NULL;
    }
    
    
    pNode = (WIFI_EVENT_NODE_STRU *)kmalloc(((unsigned int)len + sizeof(WIFI_EVENT_NODE_STRU)), GFP_ATOMIC); /*lint !e628*/
    if (NULL == pNode) 
    {
        PLAT_WLAN_INFO("kmalloc failed! ");
        return NULL;
    }

    pNode->len = len;
    memcpy(pNode->data, data, (unsigned int)len);
    
    return pNode;
}

/***********************************************************************************
Function:           wifi_event_node_free
Description:       free the node
Calls:
Input:             pNote
Output:            NA
Return:            NA
                 
************************************************************************************/
static inline void wifi_event_node_free(WIFI_EVENT_NODE_STRU *pNode)
{
    PLAT_WLAN_INFO("enter");
    if (NULL != pNode)
    {
        kfree(pNode);
    }
}

/***********************************************************************************
Function:           wifi_event_enqueue
Description:       inset the note into queue
Calls:
Input:             pCtrl: control block
             pNote: wifi event note
Output:            NA
Return:            NA
                 
************************************************************************************/

static inline void wifi_event_enqueue(WIFI_MSG_CTRL_STRU *pCtrl,WIFI_EVENT_NODE_STRU *pNode)
{
    unsigned long flags = 0;

    PLAT_WLAN_INFO("enter");

    spin_lock_irqsave(&(pCtrl->queue_lock), flags); /*lint !e628 !e160 !e666 !e534 !e530 !e830 !e10 !e550 !e522 */
    list_add_tail(&pNode->list, &(pCtrl->event_queue));
    pCtrl->queue_len++;
    spin_unlock_irqrestore(&(pCtrl->queue_lock), flags); /*lint !e530 */
}

/***********************************************************************************
Function:           wifi_event_dequeue
Description:      dequeue a note for queue
Calls:
Input:             pCtrl: control block
Output:            NA
Return:            event node or NULL
                 
************************************************************************************/
static inline WIFI_EVENT_NODE_STRU *wifi_event_dequeue(WIFI_MSG_CTRL_STRU *pCtrl) /*lint !e533*/
{
    WIFI_EVENT_NODE_STRU *pNode = NULL;
    unsigned long flags = 0;

    PLAT_WLAN_INFO("enter");

    spin_lock_irqsave(&(pCtrl->queue_lock), flags);/*lint !e666 !e160 !e534 !e530 !e830 !e10 !e550 !e522 */
    pNode = list_first_entry(&(pCtrl->event_queue), WIFI_EVENT_NODE_STRU, list); /*lint !e515 !e533 !e160 !e102 !e15 !e10 !e42*/
    if (NULL == pNode)
    {
        spin_unlock_irqrestore(&(pCtrl->queue_lock), flags);/*lint !e530 */
        return NULL;
    }
    
    list_del(&pNode->list);    /*lint !e413*/
    pCtrl->queue_len--;    
    spin_unlock_irqrestore(&(pCtrl->queue_lock), flags); /*lint !e530 */
    
    return pNode;/*lint !e533 */
}/*lint !e529 */


/***********************************************************************************
Function:          wifi_eventq_is_empty
Description:      check the event queue weather is empty
Calls:
Input:              control block
Output:            NA
Return:            TRUE : empty
               FALSE: not empty
              
************************************************************************************/
static inline int wifi_eventq_is_empty(WIFI_MSG_CTRL_STRU *pCtrl) /*lint !e10*/
{   /*lint !e10*/
    unsigned long flags = 0;/*lint !e578*/
    int ret = 0;
    
    spin_lock_irqsave(&(pCtrl->queue_lock), flags);    /*lint !e666 !e160 !e534 !e530 !e830 !e10 !e550 !e522 !e119 */
    ret = list_empty(&(pCtrl->event_queue));
    spin_unlock_irqrestore(&(pCtrl->queue_lock),flags);/*lint !e530 */
    return ret;  /*lint !e64 */
}

/***********************************************************************************
 Function:          wifi_event_input
 Description:      use to proces msg from APP
 Calls:
 Input:         pCtrl: control block
                _skb: date
 Output:            NA
 Return:            NA                  
 ************************************************************************************/
static void wifi_event_input(struct sk_buff *__skb)/*lint !e527*/
{
    struct sk_buff *skb = NULL;
    struct nlmsghdr *nlh = NULL;
    WIFI_MSG_CTRL_STRU *pCtrl = NULL; /*lint !e578*/

    PLAT_WLAN_INFO("enter");

    if (unlikely (NULL == (pCtrl = wifi_user_msg_block)))/*lint !e730 !e628*/
    {
        PLAT_WLAN_INFO("pCtrl is NULL!");
        return;
    }

    skb = skb_get(__skb);
    if (skb->len < NLMSG_SPACE(0))
    {
        PLAT_WLAN_INFO("skb->len isn't enough!");    
        return;
    }

    nlh = nlmsg_hdr(skb);

    /* check nlh */
    if ( NULL == nlh)
    {
        PLAT_WLAN_INFO("nlh is NULL!");
        return;
    }

    if (!WIFI_EVENT_PATH_IS_READY(pCtrl))
    {
        PLAT_WLAN_INFO("record user_pid = 0x%u!",nlh->nlmsg_pid);
        // Store the user space daemon pid
        pCtrl->user_pid = nlh->nlmsg_pid;
        
        // Send device events, reported before daemon start
        schedule_delayed_work(&(pCtrl->event_work),WIFI_DELAY_FOR_NL_SEND);    /*lint !e534*/
    }
}


/***********************************************************************************
 Function:          wifi_event_send
 Description:      send event to user space
 Calls:
 Input:             workqueue
 Output:            NA
 Return:            NA
                  
 ************************************************************************************/
static void wifi_event_send(struct work_struct *work)
{
    WIFI_MSG_CTRL_STRU *pCtrl = NULL; /*lint !e578*/
    WIFI_EVENT_NODE_STRU *pNode = NULL; /*lint !e578*/
    struct sk_buff *skb = NULL;
    struct nlmsghdr *nlh = NULL;    
    int msgSize = 0;
    int ret = 0;

    PLAT_WLAN_INFO("enter");
    
    /*restore the contrl block*/        
    pCtrl = wifi_user_msg_block;

    while (!wifi_eventq_is_empty(pCtrl))
    {
        pNode = wifi_event_dequeue(pCtrl);
        if (NULL == pNode)
        {
            PLAT_WLAN_INFO("dequeue a null node");
            break;
        }

        PLAT_WLAN_INFO("dequeue a node");

        /*
         * node->len: size of (node->data), is event message length
         * size: size of (skb) 
         * size = sizeof(*nlh) + align pad + node->len + aligh pad
         * 
         * alloc skb here
         * But, NOT release skb here
         * 
         */
        msgSize = NLMSG_SPACE(pNode->len);
        skb = alloc_skb((unsigned int)msgSize, GFP_ATOMIC);
        if (NULL == skb)
        {
            PLAT_WLAN_INFO("alloc skb failed");
            return;
        }

        // Use "size - sizeof(*nlh)" here (incluing align pads)
        nlh = nlmsg_put(skb, 0, 0, 0, (unsigned int)msgSize - sizeof(*nlh), 0);

        NETLINK_CB(skb).pid = 0;        /*lint !e545 */
        NETLINK_CB(skb).dst_group = 0;  /*lint !e545 */
        memcpy(NLMSG_DATA(nlh), pNode->data, (unsigned int)pNode->len);

        // Release this event entry
        wifi_event_node_free(pNode);

        // Send message to user space
        ret = netlink_unicast(pCtrl->dev_sock, skb, pCtrl->user_pid, 0);
        if (ret < 0) 
        {
            PLAT_WLAN_INFO("msg send failed!!!");
            return;
        }
        
        PLAT_WLAN_INFO("msg send over");    
    }
}

/***********************************************************************************
 Function:          wifi_event_send_tasklet_mode
 Description:      report a message  to user space
 Calls:
 Input:             data: data buffer
               len: date len
 Output:            NA
 Return:            OK or ERROR
 ************************************************************************************/
int wifi_event_send_tasklet_mode(void *data, int len) /*lint !e529!e629*/
{   /*lint !e629*/
    WIFI_MSG_CTRL_STRU *pCtrl = NULL;    /*lint !e578*/
    WIFI_EVENT_NODE_STRU *pNode = NULL; /*lint !e578*/

    PLAT_WLAN_INFO("enter");

    /*restore the contrl block*/        
    pCtrl = wifi_user_msg_block;

    /*check paramate*/
    if (unlikely(NULL == pCtrl) || (!WIFI_EVENT_PATH_IS_READY(pCtrl))) /*lint !e730 */
    {
        PLAT_WLAN_INFO("msg path isn't ready");
        return ERROR;
    }

    pNode = wifi_event_node_alloc(pCtrl,data, len);
    if (unlikely(NULL == pNode)) /*lint !e730 */
    {
        PLAT_WLAN_INFO("alloc node failed");
        return  ERROR;
    }
    
    wifi_event_enqueue(pCtrl,pNode);

    /* Only put event into buffer queue */
    tasklet_schedule(&wifi_send_event_name);    

    return OK;        
}

/*****************************************************************************
 函数名称  : WLAN_RETURN_TYPE wlan_event_report(WIFI_USER_EVENT *event) 
 功能描述  : 上报netlink事件
 输入参数  : event:上报消息
 输出参数  : NA
 返 回 值  : WLAN_RETURN_TYPE
*****************************************************************************/
WLAN_RETURN_TYPE wlan_event_report(WLAN_USER_EVENT *event) /*lint -e529 */
{
    WIFI_MSG_CTRL_STRU *pCtrl = wifi_user_msg_block;
    WIFI_EVENT_NODE_STRU *pNode = NULL; /*lint !e578*/
    
    ASSERT_NULL_POINTER(pCtrl, WLAN_POINTER_NULL);
    ASSERT_NULL_POINTER(event, WLAN_POINTER_NULL);
    PLAT_WLAN_INFO("Enter, eventId = %d, eventVal = %d",
                    (int)event->eventId, (int)event->eventVal);
   
    pNode = wifi_event_node_alloc(pCtrl,(void *)event, sizeof(*event));
    if (unlikely(NULL == pNode)) /*lint !e730 */
    {
        PLAT_WLAN_INFO("alloc node failed");
        return  ERROR;
    }
    
    wifi_event_enqueue(pCtrl,pNode);

    /* Only put event into buffer queue */
    tasklet_schedule(&wifi_send_event_name); 
    return WLAN_SUCCESS;
}
EXPORT_SYMBOL(wlan_event_report);

/*****************************************************************************
 函数名称  : int wifi_netlink_chunnel_init(void)
 功能描述  : netlink client初始化
             initialization a netlink for wifi pass event to user APP
 输入参数  : NA
 输出参数  : NA
 返 回 值  : WLAN_RETURN_TYPE
*****************************************************************************/
static int wifi_netlink_chunnel_init(void)
{
    WIFI_MSG_CTRL_STRU *pCtrl = NULL; /*lint !e578*/
    
    PLAT_WLAN_INFO("enter");
    pCtrl = (WIFI_MSG_CTRL_STRU*) kmalloc(sizeof(WIFI_MSG_CTRL_STRU), GFP_ATOMIC);
    if (NULL == pCtrl)
    {
        PLAT_WLAN_INFO("kmalloc failed");
        return ERROR;
    }

    /*init netlink and other resource*/
    WIFI_MSG_CTRL_INIT(pCtrl);  /*lint !e69 !e534 !e133 !e43 !e611*/

    if (NULL == pCtrl->dev_sock)
    {
        /*if failed, release the resource*/
        WIFI_MSG_CTRL_UNINIT(pCtrl);  /*lint !e534*/
        kfree(pCtrl);        
        PLAT_WLAN_INFO("WIFI_MSG_CTRL_INIT failed!");    
        return ERROR;
    }

    /*record the contrl block*/        
    wifi_user_msg_block = pCtrl;
    PLAT_WLAN_INFO("exit");
    return OK;
}

/*****************************************************************************
 函数名称  : void wifi_netlink_chunnel_uninit(void)
 功能描述  : netlink client资源释放
 输入参数  : NA
 输出参数  : NA
 返 回 值  : NA
*****************************************************************************/
static void wifi_netlink_chunnel_uninit(void)
{
    WIFI_MSG_CTRL_STRU *pCtrl = NULL; /*lint !e578*/

    PLAT_WLAN_INFO("enter");

    /*restore the contrl block*/        
    pCtrl = wifi_user_msg_block;

    if (pCtrl)
    {
        WIFI_MSG_CTRL_UNINIT(pCtrl); /*lint !e534*/
        kfree(pCtrl);
        wifi_user_msg_block = NULL;        
    }
    PLAT_WLAN_INFO("exit");
}

 /*****************************************************************************
  函数名称  : WLAN_RETURN_TYPE wlan_signal_report(WiFi_SIGNAL_EVENT_ENUM signal)
  功能描述  : 向应用层发送信号
  输入参数  : NA
  输出参数  : NA
  返 回 值  : 发送信号是否成功
 *****************************************************************************/
 WLAN_RETURN_TYPE wlan_signal_report(WLAN_SIGNAL_EVENT_ENUM signal) /*lint -e529 */
 {
     int ret = 0;
     ASSERT_NULL_POINTER(g_wlan_msg_monitor.p_pid, WLAN_POINTER_NULL);
     
     ret = kill_pid((struct pid *)g_wlan_msg_monitor.p_pid, signal, 1);
     
     PLAT_WLAN_INFO("kill signal = %d, ret = %d, pid = %d", signal, ret, g_wlan_msg_monitor.pid);
     return ((0 == ret) ? WLAN_SUCCESS : WLAN_EFFECT_FAILURE);
 }
 EXPORT_SYMBOL(wlan_signal_report);


 /*****************************************************************************
  函数名称  : int wlan_if_init(void)
  功能描述  : 驱动适配层初始化
  输入参数  : NA
  输出参数  : NA
  返 回 值  : WLAN_RETURN_TYPE
 *****************************************************************************/
static int __init wlan_if_init(void)
{
    int ret = 0;
    
    ret = wifi_dev_register();
    
    ret |= wifi_netlink_chunnel_init(); 

    /* init timer */
    memset(wlan_timer,0x0,sizeof(wlan_timer));
    
#ifdef CONFIG_HAS_WAKELOCK
    wake_lock_init(&wlan_wakelock,WAKE_LOCK_SUSPEND,WIFI_WAKELOCK_NAME);
#endif    
    
    spin_lock_init(&wlan_status_lock);
    wlan_stus_timer_start();
    
    mutex_init(&wlan_ioc_mutex);
    mutex_init(&wlan_pwr_mutex);

    return ret;
}

/*****************************************************************************
 函数名称  : int wlan_if_exit(void)
 功能描述  : 驱动适配层初始化
 输入参数  : NA
 输出参数  : NA
 返 回 值  : WLAN_RETURN_TYPE
*****************************************************************************/
static void __exit wlan_if_exit(void)
{
    /* wifi at dev unregister */
    (void)wifi_at_device_unregister();
    
    /* netlink uninit */
    wifi_netlink_chunnel_uninit();

    /* del all timers */
    wifi_timer_delete(USER_WIFI_TMR_AP);
    wifi_timer_delete(USER_WIFI_TMR_STA);

    del_timer_sync(&wlan_stus_detect_timer);
    
#ifdef CONFIG_HAS_WAKELOCK
    wake_lock_destroy(&wlan_wakelock);
#endif
}

module_init(wlan_if_init); /*lint !e529*/
module_exit(wlan_if_exit); /*lint !e529*/


/*===========================================================================

                       导出符号列表

===========================================================================*/

EXPORT_SYMBOL(wifi_event_send_tasklet_mode);

EXPORT_SYMBOL(wlan_status);

MODULE_AUTHOR("Hisilicon Drive Group");
MODULE_DESCRIPTION(WIFI_AT_DEVICE);
MODULE_LICENSE("GPL");

