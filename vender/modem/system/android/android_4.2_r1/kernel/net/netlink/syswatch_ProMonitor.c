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

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/skbuff.h>
#include <linux/moduleparam.h>
#include <linux/mutex.h>
#include <linux/wait.h>
#include <linux/miscdevice.h>
#include <linux/delay.h>
#include <linux/netlink.h>
#include <asm/uaccess.h>
#include <net/sock.h>

#define SYSWATCH_OK                    0
#define SYSWATCH_ERROR                (-1)
#define SYSWATCH_NAME_TOO_LONG        (-2)
#define SYSWATCH_MPQ_END              (-1)

#define MAX_MONITORED_PROCESSES_NUM    40        //max supported processes numbers
#define ONE_PROCESS_NAME_LEN            16        //max process name length
#define MONITORED_PROCESS_BUFF_LEN    (sizeof(struct monitor_cell) * (MAX_MONITORED_PROCESSES_NUM))
#define DEVICE_EVENT_REPORT_BUF_LEN    (sizeof(DEVICE_EVENT) + (ONE_PROCESS_NAME_LEN) + 1)


/*
cell of the 'monitor_process_queue'
each cell should be in one list, the list maybe used list or free list.
'next_idx' refers to the index of next cell in the list, and is '-1' if comes to the
end of the list
'comm' is the monitored process's name
why name this field 'comm'? this name comes from 'struct task_struct' 
*/
struct monitor_cell
{
    int  next_idx;
    char comm[ONE_PROCESS_NAME_LEN];
};

/* the queue of the 'monitor_cell'*/
struct monitor_process_queue
{
    int                curr_num; /* how many monitor_cell in the pool*/
    int                first_used_idx; /* the index of first used cell of the pool*/
    int                first_free_idx; /*the index of first unused cell of the pool*/
    spinlock_t          queue_lock;    /*lock for acess process queue*/
    struct monitor_cell *pool;
};


#define SYS_SEPRATOR    ' '

#define SYSWATCH_UTCPRINT printk

static struct monitor_process_queue syswatch_mpq = {0};

extern int device_event_handler_register(DEVICE_ID id, device_event_dispatch dispatcher);
extern int device_event_report(void *data, int len);



/*-------------------------------------------------
Func:	syswatch_show_monitored
Desc:	for TESTuse, print all used prcesses name
Para:	NA
return:	NA
--------------------------------------------------*/
static void syswatch_show_monitored(void)
{
#if 0
    int index = -1;

    printk("Begin----------------------------------\n");

    for(index = syswatch_mpq.first_used_idx; index != SYSWATCH_MPQ_END; index = syswatch_mpq.pool[index].next_idx)
    {
        printk("%s\n", syswatch_mpq.pool[index].comm);
    }

    printk("End----------------------------------\n");
#endif
    return;
}


/*-------------------------------------------------
Func:	syswatch_find_monitored_process
Desc:	find a specified process name from queue
Para:	name, specified process name 
		len, 	   length of string name
return:	monitor cell index of queue when the specified process exist
		SYSWATCH_MPQ_END when the specified process not exist
		SYSWATCH_NAME_TOO_LONG when para len not illegal
--------------------------------------------------*/
int syswatch_find_monitored_process(const char *name, unsigned int len)
{
    int index = SYSWATCH_MPQ_END;

    if (len >= ONE_PROCESS_NAME_LEN)
    {
        SYSWATCH_UTCPRINT("process name invalid:%d\n", len);
        return SYSWATCH_NAME_TOO_LONG;
    }

    index = syswatch_mpq.first_used_idx;

    while (index != SYSWATCH_MPQ_END)
    {
        if (!strncmp(syswatch_mpq.pool[index].comm, name, len))
        {
            // here the check is for case:
            // when the monitored processes include "rmdir",
            // and we want to find whether "rm" is included,
            // if we just compare the first two bytes, the result
            // will be wrong
            char *str = syswatch_mpq.pool[index].comm;

            if ((str[len] == ' ') || (str[len] == '\0'))
            {
                break;
            }
        }

        index = syswatch_mpq.pool[index].next_idx;
    }

    return index;
}


/*-------------------------------------------------
函数原型:static int syswatch_rm_monitored_process(char *name)；
描    述： 		从监控队列里删除一进程
输    入： 		要去除监控的进程名称
输    出： 		无
返 回 值： 	去除成功返回0
                其它返回-1
--------------------------------------------------*/
int syswatch_rm_monitored_process(const char *name)
{
    int index = -1;
    int tmpIndex;
    int ret = SYSWATCH_ERROR;

    //para check
    if(!name || (name[0] == '\0'))
    {
        SYSWATCH_UTCPRINT("Invalid parameters, return error\n");
        return ret;
    }

    /*Lock*/
    spin_lock(&syswatch_mpq.queue_lock);

    //safe check
    if(NULL == syswatch_mpq.pool)
    {
        SYSWATCH_UTCPRINT("syswatch not inited, return error\n");
        goto safe_exit;
    }

    index = syswatch_find_monitored_process(name, strlen(name));

    // find the target process in the moniroed processes
    if ((index >= 0) && (index < MAX_MONITORED_PROCESSES_NUM))
    {
        tmpIndex = syswatch_mpq.first_used_idx;

        if (index == syswatch_mpq.first_used_idx)
        {
            syswatch_mpq.first_used_idx = syswatch_mpq.pool[index].next_idx;
        }
        else
        {
            //find the previous cell
            while (tmpIndex != SYSWATCH_MPQ_END)
            {
                if (syswatch_mpq.pool[tmpIndex].next_idx == index)
                {
                    break;
                }

                tmpIndex = syswatch_mpq.pool[tmpIndex].next_idx;
            }

            syswatch_mpq.pool[tmpIndex].next_idx = syswatch_mpq.pool[index].next_idx;
        }

        memset(syswatch_mpq.pool[index].comm, 0, ONE_PROCESS_NAME_LEN);

        // add this back to free index list
        syswatch_mpq.pool[index].next_idx = syswatch_mpq.first_free_idx;
        syswatch_mpq.first_free_idx = index;

        --syswatch_mpq.curr_num;

        //syswatch_show_monitored();

        ret = SYSWATCH_OK;
    }

safe_exit:
    syswatch_show_monitored();
    spin_unlock(&syswatch_mpq.queue_lock);
    return ret;
}


/*-------------------------------------------------
函数原型：static int syswatch_put_process(const char * name, unsigned int length)
描    述： 		添加一个进程名称到链表中
输    入： 		进程名称，名称长度
输    出： 		无
返 回 值： 	设置成功返回0
                否则返回-1
--------------------------------------------------*/
static int syswatch_put_process(const char * name, unsigned int length)
{
    int index;
    // actually this check is also done in following syswatch_find_monitored_process
    // but here add some more debug information
    // so just keep it here
    if (length >= ONE_PROCESS_NAME_LEN)
    {
        SYSWATCH_UTCPRINT("process name too long, %d\n",length);
        return -1;
}

    // skip the duplicated element
    index = syswatch_find_monitored_process(name, length);
    if ((index >= 0) && (index < MAX_MONITORED_PROCESSES_NUM))
    {
        SYSWATCH_UTCPRINT("There is duplicated monitor process.\n");
        return -1;
    }


    // get the free cell index
    index = syswatch_mpq.first_free_idx;

    // adjust the next free index to the first one
    syswatch_mpq.first_free_idx = syswatch_mpq.pool[index].next_idx;

    // copy one process's name to the free cell
    strncpy(syswatch_mpq.pool[index].comm, name, length);
    // remember to add the ending NULL
    syswatch_mpq.pool[index].comm[length] = '\0';

    // add this cell to the used list
    syswatch_mpq.pool[index].next_idx = syswatch_mpq.first_used_idx;
    syswatch_mpq.first_used_idx = index;
    ++syswatch_mpq.curr_num;

    return 0;
}

/*-------------------------------------------------
函数原型： static int syswatch_set_monitored_processes(char *names)；
描    述： 		一次设置多个要监控的进程
输    入： 		多个进程名称组成的字符串，进程名称
                之间用空格隔开
输    出： 		无
返 回 值： 	设置成功返回0
                否则返回-1
--------------------------------------------------*/
static int syswatch_set_monitored_processes(const char *names)
{
    char *next = NULL, *pos = NULL;
    int ret   = SYSWATCH_ERROR;
    //int index = SYSWATCH_MPQ_END;
    int last_one_len;
    int tmpRet;

    pos = (char *)names;

    if (!names || (names[0] == '\0'))
    {
        SYSWATCH_UTCPRINT("Invalid parameters, return error\n");
        return ret;
    }

    if (!syswatch_mpq.pool)
    {
        return SYSWATCH_ERROR;
    }

    SYSWATCH_UTCPRINT("%s\n", names);

    /* Lock */
    spin_lock(&syswatch_mpq.queue_lock);

    // move forward all the ' '
    while (*pos == SYS_SEPRATOR)
    {
        ++pos;
    }

    while ((next = strchr(pos, SYS_SEPRATOR)))
    {
        if (syswatch_mpq.curr_num >= MAX_MONITORED_PROCESSES_NUM)
        {
            SYSWATCH_UTCPRINT("Cannot add any more monitored processes.\n");
            ret = SYSWATCH_ERROR;
            goto safe_exit;
        }

        tmpRet = syswatch_put_process(pos, (unsigned int)(next - pos));
        if(tmpRet != 0)
        {
            printk("syswatch_put_process failed\n");
        }

        // move forward all the ' '
        while (*next == SYS_SEPRATOR)
        {
            ++next;
        }

        pos = next;
    }

    // check whether there is trailing one
    if ((*pos != '\0') && (*pos != '\n'))
    {
        last_one_len = (int)strlen(pos);

        if (last_one_len >= ONE_PROCESS_NAME_LEN)
        {
            SYSWATCH_UTCPRINT("process name too long, %d bytes\n", last_one_len);
            ret = SYSWATCH_ERROR;
            goto safe_exit;
        }

        
        tmpRet = syswatch_put_process(pos, (unsigned int)last_one_len);
        if(tmpRet != 0)
        {
            printk("syswatch_put_process failed\n");
        }

    }

    ret = SYSWATCH_OK;
safe_exit:
    syswatch_show_monitored();
    
    /* Unlock */
    spin_unlock(&syswatch_mpq.queue_lock);
    return ret;
}



/*-------------------------------------------------
函数原型： static int syswatch_rm_all_monitored_processes(void)；
描    述： 		一次性清空监控队列
输    入： 		无
输    出： 		无
返 回 值： 	总返回0
--------------------------------------------------*/
#if 0

static int syswatch_rm_all_monitored_processes(void)
{
    int index;
    SYSWATCH_UTCPRINT("remove all processes");

    if(syswatch_mpq.pool)
    {
        /*Lock*/
        spin_lock(&syswatch_mpq.queue_lock);

        //Link the monitor cells
        for(index = 0; index < MAX_MONITORED_PROCESSES_NUM - 1; ++index)
        {
            syswatch_mpq.pool[index].next_idx = index + 1;
        }
        syswatch_mpq.pool[index].next_idx = SYSWATCH_MPQ_END;

        //init global states
        syswatch_mpq.first_used_idx = SYSWATCH_MPQ_END;
        syswatch_mpq.first_free_idx = 0;
        syswatch_mpq.curr_num = 0;

        /*Unlock*/
        spin_unlock(&syswatch_mpq.queue_lock);
    }
    return SYSWATCH_OK;
}
#endif
/*-------------------------------------------------
函数原型： static void syswatch_nl_input(struct sk_buff *__skb)；
描    述： 		此函数处理userspace下发的消息
输    入： 		封装了userspace消息体的sk_buff
输    出： 		无
返 回 值： 	无
--------------------------------------------------*/
static int syswatch_nl_input(void *buffer, int length)
{
    char *name = NULL;
    DEVICE_EVENT *pEvent = (DEVICE_EVENT*)buffer;
    int ret;
    int last_offset;
    
    /* safe checks */
    if(pEvent == NULL)
    {
        SYSWATCH_UTCPRINT("Invalid parameters, Null ptr\n");
        return SYSWATCH_ERROR;
    }
    
    if (length <= (int)sizeof(DEVICE_EVENT))
    {
        SYSWATCH_UTCPRINT("Invalid parameters, length too small: %d\n",length);
        return SYSWATCH_ERROR;
    }
    
    if (pEvent->device_id != DEVICE_ID_PROCESS)
    {
        SYSWATCH_UTCPRINT("Invalid parameters, event type:%d\n", pEvent->device_id);
        return SYSWATCH_ERROR;
    }
    
    name =  (char *)buffer + sizeof(DEVICE_EVENT);
    /* force to add a null symbol to the end */
    last_offset = length - sizeof(DEVICE_EVENT);
    last_offset -= 1;
    name[last_offset] = '\0';
    
    switch(pEvent->event_code)
    {
    case PROCESS_ADD_MONITORED:
        SYSWATCH_UTCPRINT("PROCESS_ADD_MONITORED\n");
        ret = syswatch_set_monitored_processes(name);
        break;
    case PROCESS_RM_MONITORED:
        SYSWATCH_UTCPRINT("PROCESS_RM_MONITORED\n");
        ret = syswatch_rm_monitored_process(name);
        break;
    default:
        SYSWATCH_UTCPRINT("NOT implemented event type: %d\n", pEvent->event_code);
        ret = SYSWATCH_ERROR;
    }
    
    return ret;
    
}



/*-----------------------------------------------------------------------------
Function:       syswatch_process_exit
Parameter:	    task--- 当前进程指针
  Description:    检查当前进程是否是被监视进程，并上报事件
  Return:         无
------------------------------------------------------------------------------*/  
void process_exit_monitor(struct task_struct *task)
{
    DEVICE_EVENT *pEvent = NULL;
    int index;
    unsigned int length;
    char *name = NULL;
    char buff[DEVICE_EVENT_REPORT_BUF_LEN];
    int ret;

    if (NULL == syswatch_mpq.pool)
    {
        return;
    }
    
    name = task->comm;
    length = strlen(name);
    
    if (length >= (ONE_PROCESS_NAME_LEN - 1))
    {
        length = ONE_PROCESS_NAME_LEN - 1;
    }

    /* Lock */
    spin_lock(&syswatch_mpq.queue_lock);
    
    index = syswatch_find_monitored_process(name, length);
    
    /* Unlock */
    spin_unlock(&syswatch_mpq.queue_lock);
    
    if ((index >= 0) && (index < MAX_MONITORED_PROCESSES_NUM))
    {
        //填充nl_dev_type
        memset(buff, 0, DEVICE_EVENT_REPORT_BUF_LEN);
        pEvent = (DEVICE_EVENT*)buff;
        pEvent->device_id = DEVICE_ID_PROCESS;
        pEvent->event_code = (int)PROCESS_REPORT_EXIT;
        strncpy(pEvent->data, name, length);
        pEvent->data[length] = '\0';
        pEvent->len = (int)length;

        //调用事件上报接口上报事件
        ret = device_event_report(pEvent, sizeof(DEVICE_EVENT) + length + 1);
        if (ret != 0)
        {
            printk("report event failed\n");
        }
            
    }
    else
    {
        //do nothing
    }

    return;
}



static void syswatch_exit(void)
{
    int ret;
    /* unregister event dispatcher first */
    ret = device_event_handler_register(DEVICE_ID_PROCESS, NULL);
    if (ret != 0)
    {
        printk("unregister event handler failed\n");
    }
    
    if(syswatch_mpq.pool)
    {
        kfree(syswatch_mpq.pool);
    }
    syswatch_mpq.pool = NULL;
    //swatch_inited = 0;
}

/*-------------------------------------------------
Func:	syswatch_init
Desc:	init syswatch module
Para:	NA
return:	SYSWATCH_OK when init success
		SYSWATCH_ERROR when failed to alloc memory
--------------------------------------------------*/
static int  syswatch_init(void)
{
    int index = 0;
    struct monitor_cell *monitored_processes = syswatch_mpq.pool;

    /* Alloc memory for syswatch queue*/
    if (NULL != monitored_processes)
    {
        return SYSWATCH_OK;
    }

    monitored_processes = (struct monitor_cell *)kzalloc(MONITORED_PROCESS_BUFF_LEN, GFP_ATOMIC);
    if (!monitored_processes)
    {
        SYSWATCH_UTCPRINT("Alloc memory for syswatch failed\n");
        return SYSWATCH_ERROR;
    }

    syswatch_mpq.pool = monitored_processes;



    //Link the monitor cells
    for (index = 0; index < MAX_MONITORED_PROCESSES_NUM - 1; ++index)
    {
        monitored_processes[index].next_idx = index + 1;
    }

    monitored_processes[index].next_idx = SYSWATCH_MPQ_END;

    //init global states
    spin_lock_init(&syswatch_mpq.queue_lock);
    syswatch_mpq.first_used_idx = SYSWATCH_MPQ_END;
    syswatch_mpq.first_free_idx = 0;
    syswatch_mpq.curr_num = 0;

    /* register callback function to netlink module */
    if (0 != device_event_handler_register(DEVICE_ID_PROCESS, syswatch_nl_input))
    {
        SYSWATCH_UTCPRINT("register event handler failed\n");
        syswatch_exit();
        return SYSWATCH_ERROR;
    }

    //swatch_inited = 1;
    return SYSWATCH_OK;
}



EXPORT_SYMBOL(process_exit_monitor);


module_init(syswatch_init);
module_exit(syswatch_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("jakio.chen");
MODULE_DESCRIPTION("process monitor----report event when specified processes exit");

