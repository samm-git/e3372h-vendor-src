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
#include "huawei_proc.h"

#define MAX_MSG_LENGTH 128      /*定义数据包最大长度*/    
static char msg[MAX_MSG_LENGTH];
int g_ssid2_pdev = -1;   /*初始化为-1，读取文件/proc/wifinet/ssid2中的值，并赋给g_ssid2_pdev*/

static struct proc_dir_entry *root_entry = NULL;
static struct proc_dir_entry *subroot_entry = NULL;

static int g_wifi_proc_inited = 0;

/*字符串与整数转换*/
typedef unsigned int dsat_num_item_type;
typedef  unsigned char      byte;  
typedef enum
{                       
  ATOI_OK,              /*  conversion complete             */
  ATOI_NO_ARG,          /*  nothing to convert              */
  ATOI_OUT_OF_RANGE     /*  character or value out of range */
} atoi_enum_type;
#define MAX_VAL_NUM_ITEM 0xFFFFFFFF /*  Max value of a numeric AT parm     */
#define  UPCASE( c ) ( ((c) >= 'a' && (c) <= 'z') ? ((c) - 0x20) : (c) )
#define DEC 10
/*****************************************************************************
 函 数 名  : huawei_atoi
 功能描述  : 实现字符串按进制转换为整数
 输入参数  : val_arg_ptr ----- 指针用于存储转换结果
             s----- 原始的字符串
             r------进制
 输出参数  : 无
 返 回 值  : 成功或失败


修改内容   : 新生成函数

*****************************************************************************/
atoi_enum_type huawei_atoi
(
    dsat_num_item_type *val_arg_ptr,     
    const byte *s,     
    unsigned int r    
)
{
    atoi_enum_type err_ret = ATOI_NO_ARG;
    byte c;
    dsat_num_item_type val, val_lim, dig_lim;
    
    val = 0;
    val_lim = (dsat_num_item_type) ((unsigned int)MAX_VAL_NUM_ITEM / r);
    dig_lim = (dsat_num_item_type) ((unsigned int)MAX_VAL_NUM_ITEM % r);

    while ( (c = *s++) != '\0')
    {
        if (c != ' ')
        {
            c = (byte) UPCASE (c);
            if (c >= '0' && c <= '9')
            {
                c -= '0';
            }
            else if (c >= 'A')
            {
                c -= 'A' - DEC;
            }
            else
            {
                err_ret = ATOI_OUT_OF_RANGE;  /*  char code too small */
                break;
            }
        
            if (c >= r || val > val_lim
                    || (val == val_lim && c > dig_lim))
            {
                err_ret = ATOI_OUT_OF_RANGE;  /*  char code too large */
                break;
            }
            else
            {
                err_ret = ATOI_OK;            /*  arg found: OK so far*/
                val = (dsat_num_item_type) (val * r + c);
            }
        }
        *val_arg_ptr =  val;
    }
  
    return err_ret;

}

/*****************************************************************************
 函 数 名  : wifinet_read
 功能描述  : ssid2文件读内核空间数据到用户空间
 输入参数  : page ----- 数据写入的位置
             start----- 
             off------
             count-----可以写入的最大字节数
             eof------文件结束参数
             data------私有数据指针
 输出参数  : 
 返 回 值  : 


修改内容   : 新生成函数

*****************************************************************************/
static int wifinet_read(char *page, char **start, off_t off, int count, int *eof, void *data)
{
    int len = strlen(msg);

    if (off >= len)
    {
        return 0;
    }

    if (count > len - off)
    {
        count = len - off;
    }

    memcpy(page + off, msg + off, count);      /*读取内核数据到用户空间*/
    return off + count;
}
/*****************************************************************************
 函 数 名  : wifinet_write
 功能描述  : 向ssid2写数据
 输入参数  : file ----- 打开的文件结构
             buffer----- 传递的字符串数据
             len------buffer中数据写入的多少
             data------指向私有数据的指针
 输出参数  : 
 返 回 值  : 
 修改内容   : 新生成函数
*****************************************************************************/

static int wifinet_write(struct file *file, const char __user *buffer, unsigned long len, void *data)
{

    if (len >= MAX_MSG_LENGTH) 
    {
        //printk("wifinet_write: len(%d) is large!\n",len);
        return -ENOSPC;
    }

    if (copy_from_user(msg, buffer, len ))        
    {
        printk("wifinet_write: copy_from_user error!\n");
        return -EFAULT;
    }

    msg[len] = '\0';

    huawei_atoi(&g_ssid2_pdev, msg, 10);   /*msg所指向的字符串转化为10进制数赋给g_ssid2_pdev，
										与Daemon_Enable_SSID2_Access函数中SSID2_Ifx数值进制保持一致*/

    return len;
}
/*****************************************************************************
 函 数 名  : wifiproc_init
 功能描述  : 创建/proc/wifinet/ssid2文件并注册该文件读写函数
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 修改内容  : 新生成函数
*****************************************************************************/
int wifiproc_init(void)
{
    int error = 0;
    struct path proc_path;

    if(1 == g_wifi_proc_inited)
    {
        return 0;
    }
    g_wifi_proc_inited = 1;
	
    error = kern_path("/proc/wifinet", LOOKUP_FOLLOW, &proc_path);
    if (error)
    {
        printk(KERN_DEBUG "kern_path /proc/wifinet don't exist !\n");
        root_entry = proc_mkdir("wifinet", NULL);
        if (NULL == root_entry) 
        {
            printk(KERN_ERR "Can't create /proc/wifinet !\n");
            return -1;
        }
    }
    else
    {
        printk(KERN_DEBUG "kern_path /proc/wifinet  exist !\n");
    }

    error = kern_path("/proc/wifinet/ssid2", LOOKUP_FOLLOW, &proc_path);
    if (error)
    {
        printk(KERN_DEBUG "kern_path /proc/wifinet/ssid2 don't exist !\n");

        if (NULL != root_entry)
        {
            /*创建/proc/wifinet/ssid2文件，实现用户空间和内核空间的数据交互*/
            subroot_entry = create_proc_entry("ssid2", 0666, root_entry); 
        }

        if (NULL != subroot_entry) 
        {
            subroot_entry->read_proc = wifinet_read;
            subroot_entry->write_proc = wifinet_write;
            return 0;
        }
        else
        {
            printk(KERN_ERR "Can't create /proc/wifinet/ssid2!\n");
            remove_proc_entry("wifinet", NULL);
            return -1;
        
        }
    }
    else
    {
        printk(KERN_DEBUG "kern_path /proc/wifinet/ssid2  exist !\n");
        return -1;
    }
       
}
/*****************************************************************************
 函 数 名  : wifiproc_exit
 功能描述  : 删去/proc/wifinet/ssid2文件和/proc/wifinet/目录
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 无
 修改内容  : 新生成函数
*****************************************************************************/

void wifiproc_exit(void)
{
    remove_proc_entry("ssid2", root_entry);
    remove_proc_entry("wifinet", NULL);
    g_wifi_proc_inited = 0;
}


