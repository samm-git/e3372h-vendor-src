/******************************************************************************
  Copyright, 1988-2013, Huawei Tech. Co., Ltd.
  File name:      antenna.c
  Author:         张祥壮 z00278248
  Version:        V1.0 
  Date:              2014-6-27
  Description:    天线切换驱动程序，实现天线类型查询、设置
  Others:        
******************************************************************************/
/**********************问题单修改记录******************************************
 日    期          修改人         问题单号           修改内容
******************************************************************************/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/stat.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/interrupt.h>
#include <linux/timer.h>
#include <linux/irq.h>
#include <linux/gpio.h>
#include <linux/gpio_keys.h>
#include <bsp_softtimer.h>
#include <linux/jiffies.h>
#include "linux/wakelock.h"
#include <linux/wait.h>
#include <linux/poll.h>
#include <linux/sched.h>

static DECLARE_WAIT_QUEUE_HEAD(ant1_wait);
static DECLARE_WAIT_QUEUE_HEAD(ant2_wait);
static DEFINE_SPINLOCK(ant_check_lock); /* ant操作锁 */
static DEFINE_SPINLOCK(set_trigger_lock); /* ant操作锁 */

static int g_ant1_flag = 1;
static int g_ant2_flag = 1;

#define ANT1_GPIO_CHECK  GPIO_1_7    /*(主天线)天线1检测gpio口*/
#define ANT1_GPIO_SET  GPIO_2_17        /*(主天线)天线1 设置gpio口*/
#define ANT2_GPIO_CHECK  GPIO_2_19    /*(分集天线)天线2 检测gpio口*/
#define ANT2_GPIO1_SET  GPIO_1_30       /* (分集天线)天线2 设置gpio口1，低电平配置为内置天线*/
#define ANT2_GPIO2_SET  GPIO_2_4        /*(分集天线)天线2 设置gpio口2，高电平配置为内置天线*/

#define LEVEL_HIGH 1
#define LEVEL_LOW 0

#define ANT1 0
#define ANT2 1
#define ANTM 2

#define TIME_DELAY  50    /*防抖延时   单位ms*/

#define MSG_CONTAINER_LEN   100
static char msg_container[MSG_CONTAINER_LEN];
static const char*container_ant_msg =  "  current_antenna  %s  \n  outside_antenna  %s  \n";

#define ANTENNA_1_NAME   "antenna1"      /*主集天线命名*/
#define ANTENNA_2_NAME   "antenna2"      /* 分集天线命名*/

#define ANT_CHAR1 "1"
#define ANT_CHAR0 "0" 

static char ant1_current[2] = "0";    /*默认当前使用内置天线*/
static char ant1_outside[2] = "0";    /*外置天线1未插入*/
static char ant2_current[2] = "0";    /*默认当前使用内置天线2*/
static char ant2_outside[2] = "0";    /*外置天线2未插入*/

struct timer_list ant_timer1;
struct timer_list ant_timer2;

struct ant_gpio
{
    const char *name;
    unsigned int gpio_no;
};

/*gpio口全局变量定义*/
struct ant_gpio gpio_info[] =
{
    {
        .name = "ANT1_GPIO_CHECK",
        .gpio_no = ANT1_GPIO_CHECK,
    },
            
    {
        .name = "ANT2_GPIO_CHECK",
        .gpio_no = ANT2_GPIO_CHECK,
    },
            
    {
        .name = "ANT1_GPIO_SET",
        .gpio_no = ANT1_GPIO_SET,
    },
            
    {
        .name = "ANT2_GPIO1_SET",
        .gpio_no = ANT2_GPIO1_SET,
    },
            
    {
        .name = "ANT2_GPIO2_SET",
        .gpio_no = ANT2_GPIO2_SET,
    }
};

/*****************************************************************************
函数名称  : ant1_poll
功能描述  :
输入参数  :struct file *file, poll_table *wait
输出参数  :
返 回 值  : 
修改历史  :
*****************************************************************************/
static unsigned int ant1_poll(struct file *file, poll_table *wait)
{
    unsigned int ret = 0;

    poll_wait(file, &ant1_wait, wait);

    if (g_ant1_flag) 
    {
        ret = POLLIN | POLLRDNORM;
    }
    
    return ret;
}
/*****************************************************************************
函数名称  : ant2_poll
功能描述  :
输入参数  :struct file *file, poll_table *wait
输出参数  :
返 回 值  : 
修改历史  :
*****************************************************************************/
static unsigned int ant2_poll(struct file *file, poll_table *wait)
{
    unsigned int ret = 0;

    poll_wait(file, &ant2_wait, wait);

    if (g_ant2_flag) 
    {
        ret = POLLIN | POLLRDNORM;
    }
    
    return ret;
}
/********************************************************
*函数名   : ant_outside_check
*函数功能 :外置天线是否插入检测
*输入参数 : char*ant_outside,unsigned int gpio_no
*输出参数 : 无
*返回值   : 无
*修改历史 :
*           2014-6-27 张祥壮初版作成
********************************************************/
static void ant_outside_check(char*ant_outside,unsigned int gpio_no)
{
    int ret = 0;

    spin_lock(&ant_check_lock);
    /*查询检测外置天线插入，设置ant_outside*/
    ret = gpio_get_value(gpio_no);
    if(ret)
    {
        /*外置天线未插入*/
        strncpy(ant_outside,ANT_CHAR0,sizeof(ANT_CHAR0));
    }
    else
    {
        /*外置天线插入*/
        strncpy(ant_outside,ANT_CHAR1,sizeof(ANT_CHAR1));
    }
    
    spin_unlock(&ant_check_lock);
    return;
}

/********************************************************
*函数名   : gpio_set_trigger
*函数功能 : gpio 中断触发方式设置
*输入参数 : unsigned int gpio_no
*输出参数 : 无
*返回值   :无
*修改历史 :
*           2014-6-27 张祥壮初版作成
********************************************************/
static void gpio_set_trigger(unsigned int gpio_no)
{
    int ret = 0;
    
    spin_lock(&set_trigger_lock);
    ret = gpio_get_value(gpio_no);
    if(ret)
    {
        /*设置低电平触发*/
        gpio_int_trigger_set(gpio_no, IRQ_TYPE_LEVEL_LOW);
    }

    else
    {
        /*设置高电平触发*/
        gpio_int_trigger_set(gpio_no, IRQ_TYPE_LEVEL_HIGH);
    }
    
    spin_unlock(&set_trigger_lock);
    return;
}

/********************************************************
*函数名   : gpio_request_irq
*函数功能 : gpio 中断申请
*输入参数 : struct ant_gpio *gpio_data,irq_handler_t handler
*输出参数 : 无
*返回值   :执行成功返回0，失败返回-1
*修改历史 :
*           2014-6-27 张祥壮初版作成
********************************************************/
static int gpio_request_irq( struct ant_gpio *gpio_data,irq_handler_t handler)
{
    int ret = 0;

    /*检测gpio口需设置为输入模式*/
    gpio_direction_input(gpio_data->gpio_no);

    /*外置天线1插入检测中断设置*/
    gpio_int_mask_set(gpio_data->gpio_no);

    gpio_set_trigger(gpio_data->gpio_no);

    gpio_set_function(gpio_data->gpio_no, GPIO_INTERRUPT);

    ret = request_irq((unsigned int)gpio_to_irq(gpio_data->gpio_no), handler, 
                    IRQF_NO_SUSPEND | IRQF_SHARED, gpio_data->name, gpio_data);
    if (ret) 
    {
        printk(KERN_ERR"%s: %s  request_irq  failed.\n", __func__,gpio_data->name);
        return -1;
    }

    gpio_int_state_clear(gpio_data->gpio_no);
    gpio_int_unmask_set(gpio_data->gpio_no);

    return 0;
}


/********************************************************
*函数名   : ant_timer1_fn
*函数功能 : 定时器1超时回调函数，防抖
*输入参数 : unsigned long _data
*输出参数 : 无
*返回值   :无
*修改历史 :
*           2014-6-27 张祥壮初版作成
********************************************************/
static void ant_timer1_fn(unsigned long _data)
{
    ant_outside_check(ant1_outside,ANT1_GPIO_CHECK);

    gpio_set_trigger(ANT1_GPIO_CHECK);    

    /*中断使能*/
    gpio_int_unmask_set(ANT1_GPIO_CHECK);
    
    g_ant1_flag = 1;
    wake_up_interruptible(&ant1_wait);
}

/********************************************************
*函数名   : ant_timer2_fn
*函数功能 : 定时器2超时回调函数，防抖
*输入参数 : unsigned long _data
*输出参数 : 无
*返回值   :无
*修改历史 :
*           2014-6-27 张祥壮初版作成
********************************************************/
static void ant_timer2_fn(unsigned long _data)
{
    ant_outside_check(ant2_outside,ANT2_GPIO_CHECK);

    gpio_set_trigger(ANT2_GPIO_CHECK);

    /*中断使能*/
    gpio_int_unmask_set(ANT2_GPIO_CHECK);

    g_ant2_flag = 1;
    wake_up_interruptible(&ant2_wait);
}
/********************************************************
*函数名   : ant1_gpio_isr
*函数功能 :外置天线1插入检测中断回调函数
*输入参数 : int irq, void *dev_id
*输出参数 : 无
*返回值   :IRQ_NONE表示中断非此gpio产生，
                      IRQ_HANDLED表示中断处理成功
*修改历史 :
*           2014-6-27 张祥壮初版作成
********************************************************/
static irqreturn_t ant1_gpio_isr(int irq, void *dev_id)
{
    int ret = 0;
    ret = gpio_int_state_get(ANT1_GPIO_CHECK);
    if(!ret)
    {
        return IRQ_NONE;
    }

    /*中断屏蔽*/
    gpio_int_mask_set(ANT1_GPIO_CHECK);
    gpio_int_state_clear(ANT1_GPIO_CHECK);
    
    /* 延时防抖定时启动*/
    mod_timer(&ant_timer1, jiffies + msecs_to_jiffies(TIME_DELAY));

    return IRQ_HANDLED;
}

/********************************************************
*函数名   : ant2_gpio_isr
*函数功能 :外置天线2插入检测中断回调函数
*输入参数 : int irq, void *dev_id
*输出参数 : 无
*返回值   :IRQ_NONE表示中断非此gpio产生，
                      IRQ_HANDLED表示中断处理成功
*修改历史 :
*           2014-6-27 张祥壮初版作成
********************************************************/
static irqreturn_t ant2_gpio_isr(int irq, void *dev_id)
{
    int ret = 0;
    ret = gpio_int_state_get(ANT2_GPIO_CHECK);
    if(!ret)
    {
        return IRQ_NONE;
    }

    /*中断屏蔽*/
    gpio_int_mask_set(ANT2_GPIO_CHECK);
    gpio_int_state_clear(ANT2_GPIO_CHECK);
    
    /*延时防抖定时启动*/
    mod_timer(&ant_timer2, jiffies + msecs_to_jiffies(TIME_DELAY));

    return IRQ_HANDLED;
}
/********************************************************
*函数名   : gpio_init
*函数功能 : gpio口初始化设置
*输入参数 : 无
*输出参数 : 无
*返回值   : 执行成功返回0，失败返回-1
*修改历史 :
*           2014-6-27 张祥壮初版作成
********************************************************/
static int gpio_init(void)
{
    int error = 0;
    int i = 0;
    int j = 0;

    for(i = 0;i < (int)(ARRAY_SIZE(gpio_info));i++)
    {
        error = gpio_request(gpio_info[i].gpio_no,gpio_info[i].name);
        if(error) 
        {
            printk(KERN_ERR "%s: request gpio %s FAIL!\n", __func__, gpio_info[i].name);
            goto exit;
        }
    }

    error = gpio_request_irq(&gpio_info[0],ant1_gpio_isr);
    if(error)
    {
        goto exit;
    }

    error = gpio_request_irq(&gpio_info[1],ant2_gpio_isr);
    if(error)
    {
        free_irq((unsigned int)gpio_to_irq(ANT1_GPIO_CHECK),&gpio_info[0]);
        goto exit;
    }

    /*初始使用内置天线*/
    (void)gpio_direction_output(ANT1_GPIO_SET, LEVEL_LOW);
    (void)gpio_direction_output(ANT2_GPIO1_SET, LEVEL_LOW);
    (void)gpio_direction_output(ANT2_GPIO2_SET, LEVEL_HIGH);

    return 0;

exit:
        for(j = i;j > 0;j--)
        {
            gpio_free(gpio_info[j - 1].gpio_no);
        }

    return -1;
}
/********************************************************
*函数名   : fill_container
*函数功能 : 查询输出字符串填充
*输入参数 : char *current_ant,char *outside_ant
*输出参数 : 无
*返回值   : 执行成功返回0，失败返回-1
*修改历史 :
*           2014-6-27 张祥壮初版作成
********************************************************/
static int fill_container(char *current_ant,char *outside_ant)
{   
    int len = 0;

    (void)memset(msg_container, 0x0, MSG_CONTAINER_LEN);

    len = snprintf(msg_container, MSG_CONTAINER_LEN, container_ant_msg, current_ant, outside_ant);
    if(len < 0)
    {
        printk(KERN_ERR"%s: fill  failed.\n", __func__);
        return  -1;
    }

    return 0;
}
/********************************************************
*函数名   : ant1_read
*函数功能 :查询天线1状态函数
*输入参数 : struct file *file, char __user *buf,size_t count, loff_t *ppos
*输出参数 : 天线1状态
*返回值   : 执行成功返回，失败返回-1
*修改历史 :
*           2014-6-27 张祥壮初版作成
********************************************************/
static ssize_t ant1_read(struct file *file, char __user *buf,size_t count, loff_t *ppos)
{
    int ret = 0;
    int len = 0;

    g_ant1_flag = 0;

    ant_outside_check(ant1_outside,ANT1_GPIO_CHECK);

    ret = fill_container(ant1_current,ant1_outside);
    if(ret)
    {
        return  -1;
    }
    
    len = strlen(msg_container);    
    if(*ppos > len )
    {
        return  0;
    }
    if(count > len - *ppos)
    {
        count = len - *ppos;
    }

    ret = copy_to_user(buf, msg_container, count);
    if(0 != ret)
    {
        printk(KERN_ERR "ant1_read Failed!\n");
    }
    
    *ppos += count;
    
    return count;
}
/********************************************************
*函数名   : ant1_write
*函数功能 :设置天线1的类型为内置或外置
*输入参数 : struct file *file, const char __user *buffer, size_t count, loff_t *ppos
*输出参数 : 无
*返回值   : 执行成功返回写入字符串长度，失败返回负值
*修改历史 :
*           2014-6-27 张祥壮初版作成
********************************************************/
static ssize_t ant1_write(struct file *file, const char __user *buffer, size_t count, loff_t *ppos)
{
    if(copy_from_user(ant1_current,(void*)buffer,1))
    {
        printk(KERN_ERR "ant1_write Failed!\n");
        return - EFAULT;
    }
    ant1_current[1] = '\0';

    if(0 == strncmp(ant1_current,ANT_CHAR1,sizeof(ANT_CHAR1)))
    {
        /*设置当前天线为外置天线*/
        (void)gpio_direction_output(ANT1_GPIO_SET, LEVEL_HIGH);
        printk(KERN_INFO "%s: CURRENT_ANT1_OUTSIDE set .\n",  __func__);
    }

    else if(0 == strncmp(ant1_current,ANT_CHAR0,sizeof(ANT_CHAR0)))
    {
        /*设置当前天线为内置天线*/
        (void)gpio_direction_output(ANT1_GPIO_SET, LEVEL_LOW);
        printk(KERN_INFO "%s: CURRENT_ANT1_INSIDE set .\n",  __func__);
    }

    else
    {
        /*命令参数输入有误,当前天线设置为内置*/
        strncpy(ant1_current,ANT_CHAR0,sizeof(ANT_CHAR0));
        (void)gpio_direction_output(ANT1_GPIO_SET, LEVEL_LOW);
        printk(KERN_ERR"%s: input wrong command .\n", __func__);
        return - EIO;
    }

    return count;
}
/********************************************************
*函数名   : ant2_read
*函数功能 :查询天线2状态函数
*输入参数 : struct file *file, char __user *buf,size_t count, loff_t *ppos
*输出参数 : 天线2状态
*返回值   : 执行成功返回，失败返回-1
*修改历史 :
*           2014-6-27 张祥壮初版作成
********************************************************/
static ssize_t ant2_read(struct file *file, char __user *buf,size_t count, loff_t *ppos)
{
    int ret = 0;
    int len = 0;

    g_ant2_flag = 0;

    ant_outside_check(ant2_outside,ANT2_GPIO_CHECK);

    ret = fill_container(ant2_current,ant2_outside);
    if(ret)
    {
        return  -1;
    }
    
    len = strlen(msg_container);    
    if(*ppos > len )
    {
        return  0;
    }
    if(count > len - *ppos)
    {
        count = len - *ppos;
    }

    ret = copy_to_user(buf, msg_container, count);
    if(0 != ret)
    {
        printk(KERN_ERR "ant1_read Failed!\n");
    }
    
    *ppos += count;

    return count;
}
/********************************************************
*函数名   : ant2_write
*函数功能 :设置天线2的类型为内置或外置
*输入参数 : struct file *file, const char __user *buffer, size_t count, loff_t *ppos
*输出参数 : 无
*返回值   : 执行成功返回写入字符串长度，失败返回负值
*修改历史 :
*           2014-6-27 张祥壮初版作成
********************************************************/
static ssize_t ant2_write(struct file *file, const char __user *buffer, size_t count, loff_t *ppos)
{
    int ret = 0;

    ret = copy_from_user(ant2_current,(void*)buffer,1);
    if(ret)
    {
        printk(KERN_ERR "ant2_write Failed!\n");
        return - EFAULT;
    }
    ant2_current[1] = '\0';

    if(0 == strncmp(ant2_current,ANT_CHAR1,sizeof(ANT_CHAR1)))
    {
        /*设置当前天线为外置天线*/
        (void)gpio_direction_output(ANT2_GPIO1_SET, LEVEL_HIGH);
        (void)gpio_direction_output(ANT2_GPIO2_SET, LEVEL_LOW);
        printk(KERN_INFO "%s: CURRENT_ANT2_OUTSIDE set .\n", __func__);
    }

    else if(0 == strncmp(ant2_current,ANT_CHAR0,sizeof(ANT_CHAR0)))
    {
        /*设置当前天线为内置天线*/
        (void)gpio_direction_output(ANT2_GPIO1_SET, LEVEL_LOW);
        (void)gpio_direction_output(ANT2_GPIO2_SET, LEVEL_HIGH);
        printk(KERN_INFO "%s: CURRENT_ANT2_INSIDE set .\n", __func__);
    }

    else
    {
        /*命令参数输入有误，当前天线设置为内置*/
        strncpy(ant2_current,ANT_CHAR0,sizeof(ANT_CHAR0));
        (void)gpio_direction_output(ANT2_GPIO1_SET, LEVEL_LOW);
        (void)gpio_direction_output(ANT2_GPIO2_SET, LEVEL_HIGH);
        printk(KERN_ERR"%s: input wrong command .\n", __func__);
        return - EIO;
    }

    return count;
}

static const struct file_operations ant1_operations = {
    .read        = ant1_read,
    .write        = ant1_write,
    .poll        = ant1_poll,
};

static const struct file_operations ant2_operations = {
    .read        = ant2_read,
    .write        = ant2_write,
    .poll        = ant2_poll,
};

/********************************************************
*函数名   : antenna_proc_init
*函数功能 : 模块初始化函数
*输入参数 : 无
*输出参数 : 无
*返回值   : 执行成功返回0，失败返回非0值
*修改历史 :
*           2014-6-27 张祥壮初版作成
********************************************************/
static int __init antenna_proc_init(void)
{
    int ret = 0;
    static struct proc_dir_entry *antenna1_entry = NULL;
    static struct proc_dir_entry *antenna2_entry = NULL;

    antenna1_entry = proc_create(ANTENNA_1_NAME, S_IRUSR, NULL, &ant1_operations);
    if(NULL == antenna1_entry)
    {
        printk(KERN_ERR "can't create /proc/%s \n",ANTENNA_1_NAME);
        return - EFAULT;
    }

    antenna2_entry = proc_create(ANTENNA_2_NAME, S_IRUSR, NULL, &ant2_operations);
    if(NULL == antenna2_entry)
    {
        remove_proc_entry(ANTENNA_2_NAME, NULL);
        printk(KERN_ERR "can't create /proc/%s \n",ANTENNA_2_NAME);
        return - EFAULT;
    }

    ret = gpio_init();
    if(ret)
    {
        remove_proc_entry(ANTENNA_1_NAME, NULL);
        remove_proc_entry(ANTENNA_2_NAME, NULL);
        return -1;
    }

    ant_outside_check(ant1_outside,ANT1_GPIO_CHECK);
    ant_outside_check(ant2_outside,ANT2_GPIO_CHECK);

    setup_timer(&ant_timer1, ant_timer1_fn, 0);
    setup_timer(&ant_timer2, ant_timer2_fn, 0);
    return 0;

}
/********************************************************
*函数名   : antenna_proc_exit
*函数功能 : 模块初始化函数
*输入参数 : 无
*输出参数 : 无
*返回值   : 无
*修改历史 :
*           2014-6-27 张祥壮初版作成
********************************************************/
static void __exit antenna_proc_exit(void)
{
    int i = 0;

    del_timer_sync(&ant_timer1);
    del_timer_sync(&ant_timer2);
    
    free_irq((unsigned int)gpio_to_irq(ANT1_GPIO_CHECK),&gpio_info[0]);
    free_irq((unsigned int)gpio_to_irq(ANT2_GPIO_CHECK),&gpio_info[1]);

    /*gpio口释放*/
    for(i = 0;i < (int)(ARRAY_SIZE(gpio_info));i++)
    {
        gpio_free(gpio_info[i].gpio_no);
    }
    
    remove_proc_entry(ANTENNA_1_NAME, NULL);
    remove_proc_entry(ANTENNA_2_NAME, NULL);
}

/********************************************************
*函数名   : anten_switch_set
*函数功能 : 天线切换外部接口
*输入参数 : unsigned long ant_type,unsigned long in_or_out
*输出参数 : 无
*返回值   : 执行成功返回0，失败返回非0值
*修改历史 :
*           2014-6-27 张祥壮初版作成
********************************************************/
int anten_switch_set(unsigned long ant_type,unsigned long in_or_out)
{
    switch(ant_type)
    {
        case ANT1:
                if(0 == in_or_out)
                {
                        /*主集天线设置为内置天线*/
                        (void)gpio_direction_output(ANT1_GPIO_SET, LEVEL_LOW);
                        strncpy(ant1_current,ANT_CHAR0,sizeof(ANT_CHAR0));

                }
                else if(1 == in_or_out)
                {
                        /*主集天线设置为外置天线*/
                        (void)gpio_direction_output(ANT1_GPIO_SET, LEVEL_HIGH);
                        strncpy(ant1_current,ANT_CHAR1,sizeof(ANT_CHAR1));

                }
                else
                {
                        goto exit;
                }
        break;

        case ANT2:
                if(0 == in_or_out)
                {
                        /*分集天线设置为内置天线*/
                        (void)gpio_direction_output(ANT2_GPIO2_SET, LEVEL_HIGH);
                        (void)gpio_direction_output(ANT2_GPIO1_SET, LEVEL_LOW);
                        strncpy(ant2_current,ANT_CHAR0,sizeof(ANT_CHAR0));
                }    
                else if(1 == in_or_out)
                {
                        /*分集天线设置为外置天线*/
                        (void)gpio_direction_output(ANT2_GPIO1_SET, LEVEL_HIGH);
                        (void)gpio_direction_output(ANT2_GPIO2_SET, LEVEL_LOW);
                        strncpy(ant2_current,ANT_CHAR1,sizeof(ANT_CHAR1));
                }
                else
                {
                        goto exit;
                }
        break;

        case ANTM:
                if(0 == in_or_out)
                {
                        /*主、分集天线设置为内置天线*/
                        (void)gpio_direction_output(ANT1_GPIO_SET, LEVEL_LOW);
                        (void)gpio_direction_output(ANT2_GPIO2_SET, LEVEL_HIGH);
                        (void)gpio_direction_output(ANT2_GPIO1_SET, LEVEL_LOW);
                        strncpy(ant1_current,ANT_CHAR0,sizeof(ANT_CHAR0));
                        strncpy(ant2_current,ANT_CHAR0,sizeof(ANT_CHAR0));
                }    
                else if(1 == in_or_out)
                {
                        /*主、分集天线设置为外置天线*/
                        (void)gpio_direction_output(ANT1_GPIO_SET, LEVEL_HIGH);
                        (void)gpio_direction_output(ANT2_GPIO2_SET, LEVEL_LOW);
                        (void)gpio_direction_output(ANT2_GPIO1_SET, LEVEL_HIGH);
                        strncpy(ant1_current,ANT_CHAR1,sizeof(ANT_CHAR1));
                        strncpy(ant2_current,ANT_CHAR1,sizeof(ANT_CHAR1));
                }
                else
                {
                        goto exit;
                }
        break;

        default:
                        printk(KERN_ERR "[%s] ant_type = %ld is  wrong parameters \n",__func__,ant_type);
                        return - 1;
        break;
    }

return 0;

exit:
        printk(KERN_ERR "[%s] in_or_out = %ld is  wrong parameters \n",__func__,in_or_out);
        return - 1;
}
/********************************************************
*函数名   : anten_switch_get
*函数功能 : 天线查询外部接口
*输入参数 : unsigned long ant_type
*输出参数 : 无
*返回值   : 执行成功返回0，失败返回非0值
*修改历史 :
*           2014-6-27 张祥壮初版作成
********************************************************/
int anten_switch_get(unsigned long ant_type)
{
    int ucData = 0;

    switch(ant_type)
    {
        case ANT1:
                if(0 == strncmp(ant1_current,ANT_CHAR0,sizeof(ANT_CHAR0)))
                {
                        /*主集天线为内置天线*/
                        ucData = 0;
                }
                else if(0 == strncmp(ant1_current,ANT_CHAR1,sizeof(ANT_CHAR1)))
                {
                        /*主集天线为外置天线*/
                        ucData = 1;
                }
                else
                {
                        ucData = - 1;
                        printk(KERN_ERR "[%s] ant1_current = [%s] is  wrong parameters \n",__func__,ant1_current);
                }
        break;

        case ANT2:

                if(0 == strncmp(ant2_current,ANT_CHAR0,sizeof(ANT_CHAR0)))
                {
                        /*分集天线为内置天线*/
                        ucData = 0;
                }
                else if(0 == strncmp(ant2_current,ANT_CHAR1,sizeof(ANT_CHAR1)))
                {
                        /*分集天线为外置天线*/
                        ucData = 1;
                }
                else
                {
                        ucData = - 1;
                        printk(KERN_ERR "[%s] ant2_current = [%s] is  wrong parameters \n",__func__,ant2_current);
                }
        break;

        default:
                /*要查询的天线类型参数有误*/
                ucData = - 1;
                printk(KERN_ERR "[%s] ant_type = %ld is  wrong parameters \n",__func__,ant_type);
        break;
    }

    return ucData;
}


module_init(antenna_proc_init);
module_exit(antenna_proc_exit);

MODULE_AUTHOR("MBB.Huawei Device");
MODULE_DESCRIPTION("Mbb Common Driver");
MODULE_LICENSE("GPL");




