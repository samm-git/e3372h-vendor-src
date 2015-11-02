/******************************************************************************
  Copyright, 1988-2013, Huawei Tech. Co., Ltd.
  File name:      mbb_common.c
  Author:         徐超 x00202188
  Version:        V1.0 
  Date:           徐超     初版作成      2013-11-25
  Description:    MBB通用驱动实现文件。
                  本驱动是为了将MBB这边比较零散不方便单独写驱动处理的东西统一到一个驱动中来处理
  Others:         本文件中所写代码仅适用于MBB通用驱动相关功能使用，请勿将其他无关代码
                  写入本文件。
******************************************************************************/
/**********************问题单修改记录******************************************
 日    期          修改人         问题单号           修改内容
******************************************************************************/


#include <linux/errno.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include "bsp_ipc.h"
#include "bsp_icc.h"
#include "bsp_sram.h"
#include <mbb_common.h>
#include <linux/platform_device.h> 
#if ( FEATURE_ON == MBB_MLOG )
#include <linux/mlog_lib.h>
#include <linux/string.h>
#endif
#include "drv_ipc_enum.h"
struct mbb_common_struct mbb_common_config;

#ifdef CONFIG_USB_GADGET_SNPS_DWC_OTG
extern void dwc_otg_enter_suspend(void);
extern void dwc_otg_exit_suspend(void);
#endif
#if ( FEATURE_ON == MBB_MLOG )
typedef struct
{
    uint32 one_class_temp_start_time;
    uint32 one_class_temp_last_time;
    uint32 two_class_temp_start_time;
    uint32 two_class_temp_last_time;
    uint32 dropline_class_temp_start_time;
    uint32 dropline_class_temp_last_time;
} ThermStatTime_t;

ThermStatTime_t  g_ThermStatTime = {0, 0, 0, 0, 0, 0};

typedef enum _THERM_STATE_TYPE
{
    THERM_STATE_NORMAL = 0,
    THERM_STATE_LIMIT_1 = 1 ,  /*20M*/
    THERM_STATE_LIMIT_2 = 2 ,  /*2M*/
    THERM_STATE_EMERG_1 = 3 ,  /*fly mode*/
    THERM_STATE_EMERG_2 = 4 ,  /*shutdown*/
    THERM_STATE_MAX = 0xFFFF,  /*stop*/
} THERM_STATE_TYPE;

THERM_STATE_TYPE current_therm_state = THERM_STATE_MAX;
THERM_STATE_TYPE pre_therm_state = THERM_STATE_MAX;

#define SIX_MINUTE_TICKS   (HZ * 60 * 6)

void  mstat_thermal_protection(char* p);

#endif
#if ( FEATURE_ON == MBB_MLOG )
/********************************************************
*函数名   : mbb_common_mlog_recv
*函数功能 : 接收C核需要发到mobile log模块的日志
*输入参数 : u32 channel_id , u32 len, void* context
*输出参数 : 无
*返回值   : 执行成功返回0，失败返回非0值
*修改历史 :
*           2014-2-18 徐超 初版作成
********************************************************/
static int mbb_common_mlog_recv( u32 channel_id , u32 len, void* context )
{
    int rt = 0;
    int read_len = 0;
    stMbbCommonMlog mlogbuf;
    mlogbuf.len = 0;
    mlogbuf.loglv = 0;

    read_len = bsp_icc_read(channel_id, (u8*)&mlogbuf, len);
    if((read_len > len) || (read_len < 0))
    {
        printk(KERN_ERR "[Mbb Common Drv] mlogbuf.len =(%d),read len(%d),len(%d), msg: %s.\n", mlogbuf.len, read_len,len, mlogbuf.msg);
        return -1;
    }

    printk(KERN_INFO "[Mbb Common Drv] module=(%s),mlogbuf.len =(%d),read len(%d),len(%d),lv=(%d),msg: %s.\n",
        mlogbuf.module, mlogbuf.len, read_len, len, mlogbuf.loglv, mlogbuf.msg);
#if (FEATURE_ON == MBB_MLOG)
    mlog_print(mlogbuf.module, mlogbuf.loglv, "%s", mlogbuf.msg);
#endif
#if ( FEATURE_ON == MBB_MLOG)
    mstat_thermal_protection(mlogbuf.module);
#endif
    return 0;
}
#endif

#if ( FEATURE_ON == MBB_MLOG)
void  mstat_thermal_protection(char* p)
{
    char* tp_sign = "TP_CLASS_" ;
    char* tp_class = NULL;
    char* tp_string = p ;
    unsigned int last_times = 0;
    unsigned int tp_class_max_size = 0 ;

    if ( NULL == p)
    {
        return;
    }

    if ( 0 != strncmp(tp_string, tp_sign, strlen(tp_sign)))
    {
        return;
    }

    tp_class = p + strlen(tp_sign) ;
    tp_class_max_size = MLOG_MAX_MD_LEN - strlen(tp_sign) - 1;

    printk("%s tp_class_max_size= %d  %s %d \n" , tp_class, tp_class_max_size, __FUNCTION__, __LINE__);
    if (0 == strncmp(tp_class, "ONE", min(strlen(tp_sign), tp_class_max_size )))
    {
        current_therm_state = THERM_STATE_LIMIT_1;
        if (pre_therm_state != current_therm_state)
        {
            mlog_set_statis_info("one_class_temp_prot_count", 1);
            g_ThermStatTime.one_class_temp_start_time = jiffies;
        }
    }

    if (0 == strncmp(tp_class, "TWO", min(strlen(tp_sign), tp_class_max_size )))
    {
        current_therm_state = THERM_STATE_LIMIT_2;
        if (pre_therm_state != current_therm_state)
        {
            mlog_set_statis_info("two_class_temp_prot_count", 1);
            g_ThermStatTime.two_class_temp_start_time = jiffies;
        }
    }

    if (0 == strncmp(tp_class, "DROPLINE", min(strlen(tp_sign), tp_class_max_size)))
    {
        current_therm_state = THERM_STATE_EMERG_1;
        if (pre_therm_state != current_therm_state)
        {
            mlog_set_statis_info("dropline_class_temp_prot_count", 1);
            g_ThermStatTime.dropline_class_temp_start_time = jiffies;
        }
    }

    if (0 == strncmp(tp_class, "NORMAL", min(strlen(tp_sign), tp_class_max_size)))
    {
        printk("f00174127 debug %s  %s %d \n" , tp_class, __FUNCTION__, __LINE__);
        current_therm_state = THERM_STATE_NORMAL;
    }

    if (0 == strncmp(tp_class, "POWEROFF", min(strlen(tp_sign), tp_class_max_size)))
    {
        printk("f00174127 debug %s  %s %d \n" , tp_class, __FUNCTION__, __LINE__);
        current_therm_state = THERM_STATE_EMERG_2;
    }

    if (pre_therm_state != current_therm_state)
    {
        if (THERM_STATE_LIMIT_1 == pre_therm_state)
        {
            /*获取持续时间*/
            g_ThermStatTime.one_class_temp_last_time = \
                    g_ThermStatTime.one_class_temp_last_time \
                    + jiffies - g_ThermStatTime.one_class_temp_start_time;

            /*持续时间转换成6分钟*/
            last_times =  g_ThermStatTime.one_class_temp_last_time / SIX_MINUTE_TICKS;

            /*保存剩余的ticks*/
            g_ThermStatTime.one_class_temp_last_time = \
                    g_ThermStatTime.one_class_temp_last_time % SIX_MINUTE_TICKS;

            /*写入统计信息*/
            mlog_set_statis_info("one_class_temp_prot_time", last_times);
        }
        else if (THERM_STATE_LIMIT_2 == pre_therm_state)
        {
            /*获取持续时间*/
            g_ThermStatTime.two_class_temp_last_time = \
                    g_ThermStatTime.two_class_temp_last_time \
                    + jiffies - g_ThermStatTime.two_class_temp_start_time;

            /*持续时间转换成6分钟*/
            last_times = g_ThermStatTime.two_class_temp_last_time / SIX_MINUTE_TICKS;
            mlog_set_statis_info("two_class_temp_prot_time", last_times);

            /*保存剩余的ticks*/
            g_ThermStatTime.two_class_temp_last_time = \
                    g_ThermStatTime.two_class_temp_last_time % SIX_MINUTE_TICKS;

            /*写入统计信息*/
            mlog_set_statis_info("two_class_temp_prot_time", last_times);

        }
        else if (THERM_STATE_EMERG_1 == pre_therm_state)
        {
            /*获取持续时间*/
            g_ThermStatTime.dropline_class_temp_last_time = \
                    g_ThermStatTime.dropline_class_temp_last_time \
                    + jiffies - g_ThermStatTime.dropline_class_temp_start_time;

            /*持续时间转换成6分钟*/
            last_times = g_ThermStatTime.dropline_class_temp_last_time / SIX_MINUTE_TICKS;

            /*保存剩余的ticks*/
            g_ThermStatTime.dropline_class_temp_last_time = \
                    g_ThermStatTime.dropline_class_temp_last_time % SIX_MINUTE_TICKS;
            mlog_set_statis_info("dropline_class_temp_prot_time", last_times);
        }
    }
    pre_therm_state = current_therm_state;
}
#endif


/********************************************************
*函数名   : mbb_common_ipc_send
*函数功能 : 向C核发送消息
*输入参数 : u32 flag
*输出参数 : 无
*返回值   : 执行成功返回0，失败返回非0值
*修改历史 :
*           2013-11-25 徐超 初版作成
********************************************************/
void mbb_common_ipc_send(u32 flag)
{
    SRAM_SMALL_SECTIONS * sram_mem = ((SRAM_SMALL_SECTIONS * )SRAM_SMALL_SECTIONS_ADDR);
    sram_mem->SRAM_MBB_COMMON = flag;
    bsp_ipc_int_send(IPC_CORE_CCORE,IPC_CCPU_INT_SRC_ACPU_MBB_COMM);
}


/********************************************************
*函数名   : Mbb_Common_Probe
*函数功能 : 
*输入参数 : struct platform_device *dev
*输出参数 : 无
*返回值   : 执行成功返回0，失败返回非0值
*修改历史 :
*           2013-11-25 徐超 初版作成
********************************************************/
static int Mbb_Common_Probe(struct platform_device *dev)
{

    int ret = 0;
#if ( FEATURE_ON == MBB_MLOG )
    u32 channel_id = ICC_CHN_IFC << 16 | IFC_RECV_FUNC_MLOG; /* 用于接收C核的mobile log消息 */
#endif

    printk(KERN_ERR "\r\n [Mbb Common Drv] Mbb_Common_Probe!\n");

    if(NULL == dev )
    {
        printk(KERN_ERR "\r\n [Mbb Common Drv]Mbb_Common_Probe: dev is NULL!\r\n");
        return -1;
    }
    
#if ( FEATURE_ON == MBB_MLOG )
    /* 注册用于接收C核的mobile log消息的函数 */
    ret = bsp_icc_event_register(channel_id, (read_cb_func)mbb_common_mlog_recv, NULL, NULL, NULL);
    if(ret != 0)
    {
        printk(KERN_ERR "\r\n [Mbb Common Drv] bsp_icc_event_register error，ret=%d.\n", ret);
    }
#endif

    return ret;
}
/********************************************************
*函数名   : Mbb_Common_Remove
*函数功能 : 
*输入参数 : 
*输出参数 : 无
*返回值   : 执行成功返回0，失败返回非0值
*修改历史 :
*           2013-11-25 徐超 初版作成
********************************************************/
static int Mbb_Common_Remove(struct platform_device *dev)
{
    int ret = 0;

    printk(KERN_ERR "\r\n [Mbb Common Drv] Mbb_Common_Remove!\n");
    
    if(NULL == dev )
    {
        printk(KERN_ERR "\r\n [Mbb Common Drv]Mbb_Common_Remove: dev is NULL!\r\n");
        return -1;
    }

    return ret;
}

#ifdef CONFIG_PM
static int Mbb_Common_Prepare(struct device *pdev)
{
    return 0;
}

static void Mbb_Common_Complete(struct device *pdev)
{
    return ;
}

/********************************************************
*函数名   : Mbb_Common_Suspend
*函数功能 : Mbb_Common函数，用于处理待机休眠时的一些事务
*输入参数 : 
*输出参数 : 无
*返回值   : 执行成功返回0，失败返回非0值
*修改历史 :
*           2013-11-25 徐超 初版作成
********************************************************/
static int Mbb_Common_Suspend(struct device *dev)
{
    int ret = 0;

    printk(KERN_INFO "\r\n [Mbb Common Drv] Mbb_Common_Suspend!\n");
    
    if(NULL == dev )
    {
        printk(KERN_ERR "\r\n [Mbb Common Drv]Mbb_Common_Suspend: dev is NULL!\r\n");
        return -1;
    }
#ifdef CONFIG_USB_GADGET_SNPS_DWC_OTG
    dwc_otg_enter_suspend();
#endif 

    /* 将A核要进入待机的消息通知C核*/
    mbb_common_ipc_send(MBB_SUSPEND);
    return ret;
}

/********************************************************
*函数名   : Mbb_Common_Resume
*函数功能 : Mbb_Common函数，用于处理待机唤醒时的一些事务
*输入参数 : 
*输出参数 : 无
*返回值   : 执行成功返回0，失败返回非0值
*修改历史 :
*           2013-11-25 徐超 初版作成
********************************************************/
static int Mbb_Common_Resume(struct device *dev)
{
    int ret = 0;

    printk(KERN_INFO "\r\n [Mbb Common Drv] Mbb_Common_Resume!\n");
    
    if(NULL == dev )
    {
        printk(KERN_ERR "\r\n [Mbb Common Drv]Mbb_Common_Resume: dev is NULL!\r\n");
        return -1;
    }
    /* 将A核被唤醒的消息通知C核*/
#ifdef CONFIG_USB_GADGET_SNPS_DWC_OTG
    dwc_otg_exit_suspend();
#endif 

    mbb_common_ipc_send(MBB_RESUME);
    return ret;
}

const struct dev_pm_ops Mbb_Common_dev_Pm_Ops =
{
    .suspend    =   Mbb_Common_Suspend,
    .resume     =   Mbb_Common_Resume,
    .prepare    =   Mbb_Common_Prepare,
    .complete   =   Mbb_Common_Complete,
};

#define MBB_COMMON_DEV_PM_OPS (&Mbb_Common_dev_Pm_Ops)

#else

#define MBB_COMMON_DEV_PM_OPS NULL

#endif

static struct platform_driver Mbb_Common_drv = {
    .probe = Mbb_Common_Probe,
    .remove = Mbb_Common_Remove,
    .driver = {  
        .name  = "Mbb_Common",  
        .owner = THIS_MODULE, 
        .pm    = MBB_COMMON_DEV_PM_OPS,
    },  
};

static struct platform_device Mbb_Common_dev = {
    .name           = "Mbb_Common",
    .id             = 1,
    .dev ={
        .platform_data = &mbb_common_config,
    },
};

static int __init mbb_common_init(void)
{
    int ret = 0;
    
    ret = platform_device_register(&Mbb_Common_dev);
    if (ret < 0)
    {
        printk(KERN_ERR "\r\n [Mbb Common Drv] platform_device_register [Mbb Common Module] failed.\r\n");
        return ret;
    }
    
    ret = platform_driver_register(&Mbb_Common_drv);
    if (0 > ret)
    {
        platform_device_unregister(&Mbb_Common_dev);
        printk(KERN_ERR "\r\n [Mbb Common Drv] platform_driver_register [Mbb Common Module] failed.\r\n");
        return ret;
    }
    return ret;
}

static void __exit mbb_common_exit(void)
{
    platform_driver_unregister(&Mbb_Common_drv);

    platform_device_unregister(&Mbb_Common_dev);
}

module_init(mbb_common_init);
module_exit(mbb_common_exit);

MODULE_AUTHOR("MBB.Huawei Device");
MODULE_DESCRIPTION("Mbb Common Driver");
MODULE_LICENSE("GPL");

