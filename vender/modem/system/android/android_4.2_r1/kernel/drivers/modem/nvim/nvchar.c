 /*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
 * foss@huawei.com
 *
 * If distributed as part of the Linux kernel, the following license terms
 * apply:
 *
 * * This program is free software; you can redistribute it and/or modify
 * * it under the terms of the GNU General Public License version 2 and 
 * * only version 2 as published by the Free Software Foundation.
 * *
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * * GNU General Public License for more details.
 * *
 * * You should have received a copy of the GNU General Public License
 * * along with this program; if not, write to the Free Software
 * * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Otherwise, the following license terms apply:
 *
 * * Redistribution and use in source and binary forms, with or without
 * * modification, are permitted provided that the following conditions
 * * are met:
 * * 1) Redistributions of source code must retain the above copyright
 * *    notice, this list of conditions and the following disclaimer.
 * * 2) Redistributions in binary form must reproduce the above copyright
 * *    notice, this list of conditions and the following disclaimer in the
 * *    documentation and/or other materials provided with the distribution.
 * * 3) Neither the name of Huawei nor the names of its contributors may 
 * *    be used to endorse or promote products derived from this software 
 * *    without specific prior written permission.
 * 
 * * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/*****************************************************************************/

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif


/*lint --e{533,830}*/
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>

#include "nv_comm.h"



#define NVCHAR_DEVICE_NAME    "nv"
#define NVCHAR_DEVICE_CLASS   "nv_class"
#define NVCHAR_MAX_ID         0xffff
#define IOCTRL_MAGIC           'c'


#define IOCTRL_NVFLUSH        _IOW(IOCTRL_MAGIC, 0, struct nv_data_stru)
#define IOCTRL_NVBACKUP       _IOW(IOCTRL_MAGIC, 1, struct nv_data_stru)
#define IOCTRL_NVREVERT       _IOR(IOCTRL_MAGIC, 2, struct nv_data_stru)
#define IOCTRL_NVSETDEFATULT  _IOW(IOCTRL_MAGIC, 2, struct nv_data_stru)


#if (FEATURE_ON == MBB_COMMON)
#define NV_MAX_USER_BUFF_LEN      1024  /* 修改为与应用匹配的最大用户buff尺寸 */
#else
#define NV_MAX_USER_BUFF_LEN      2048
#endif


struct nv_dev
{
    unsigned int nvid;
    struct cdev cdev;
};

#if (FEATURE_ON == MBB_COMMON)
typedef enum
{ 
    TYPE_MAC_ADDR = 0,   /*  mac地址*/ 
    TYPE_NDIS_STATE,  /*ndis的状态*/ 
    TYPE_SD_MODE,     /*SD MODE*/ 
    TYPE_APP_INFO,    /*登陆信息*/
    
    TYPE_SSID1_INFO,   /*SSID1*/ 
    TYPE_WIFI_PASSWD1_WPA, /*WIFI PASSWORD*/ 
    TYPE_WIFI_PASSWD1_WEP, /*WIFI PASSWORD*/ 
    
    TYPE_SSID2_INFO,   /*SSID2*/ 
    TYPE_WIFI_PASSWD2_WPA, /*WIFI PASSWORD*/ 
    TYPE_WIFI_PASSWD2_WEP, /*WIFI PASSWORD*/
    
    TYPE_APPIN_INFO,  /*仅 CPE使用,用于记录 WIFI WPS的PIN码*/ 
    TYPE_GPS_ONOFF, /*GPS是否使能*/
    
    TYPE_MULT_WIFI_BASIC, /*V7R2 WIFI SSID*/
    TYPE_MULT_WIFI_SEC,   /*V7R2 WIFI KEY*/
    TYPE_FACTORY_MODE,    /*工厂模式*/      
    TYPE_CPE_TELNET_SWITCH_NVIM_STRU,    /*此NV项用于控制CPE产品telnet功能是否打开*/

    /*以下为高通平台nv，balong不涉及*/
    TYPE_FOTA_PPP_APN,            /* 进行PPP拨号时的APN */
    TYPE_FOTA_PPP_USER_ID,        /* 进行PPP拨号所需的用户名 */
    TYPE_FOTA_PPP_PASSWORD,       /* 进行PPP拨号所需的密码 */
    TYPE_FOTA_PPP_AUTH,           /* 进行PPP拨号时所用的鉴权方式 */
    TYPE_FOTA_SERVER_IP,          /* FOTA服务器的IP */
    TYPE_FOTA_SERVER_PORT,        /* FOTA服务器的端口号 */
    TYPE_FOTA_MODE,               /* FOTA模式的设定 */
    TYPE_FOTA_DETECT_COUNT,       /* 本地计数器的值*/
    TYPE_FOTA_NWTIME,             /* 周期查询的基准时间 */
    TYPE_FOTA_TIMER,              /* 定时器的步长 */
    TYPE_FOTA_INTERVAL,           /* 手动版本查询的间隔时间 */
    TYPE_FOTA_SMS_CONFIG,         /* fota暂未使用，预留*/
    TYPE_FOTA_TIME_STAMP,         /* fota暂未使用，预留*/
    TYPE_FOTA_RSA_PUB_MODE,       /* fota暂未使用，预留*/
    TYPE_FOTA_RSA_PUB_EXP,        /* fota暂未使用，预留*/
    TYPE_FOTA_SMS_FLAG,  /* 既标识短信FOTA 标志位，也保存收到FOTA 短信后的重启次数 */
    /*以上为高通平台nv，balong不涉及*/

    TYPE_NV_FACTORY_INFO_I,              /*用于记录产线工位的检测结果*/
    TYPE_NV_HUAWEI_BT_INFO_I,       /*33 蓝牙设备的MAC等配置信息*/
    TYPE_DATA_LOCK_HASH_VAL,       /*终端厂家私有密码hash值32字节*/
    TYPE_WIFI_MAC_NUM,              /*35 wifi mac地址个数，TYPE_MAC_ADDR为 基mac地址，4字节整型数据*/
    TYPE_LAN_MAC,                   /*36 LAN mac地址，ascci字符格式存储*/
    TYPE_LAN_MAC_NUM,               /*37 LAN mac地址个数，4字节整型数据*/
    TYPE_TTS_CFG,                   /*38 TTS配置*/

    TYPE_NV_MAX, 
}TYPE_KERNEL_NV_INFO; 
/*lint -e64 -e156 */
unsigned long nv_index_table[TYPE_NV_MAX + 1] = \ 
{
    {NV_ID_MSP_MAC_ADDR},                        /* TYPE_MAC_ADDR   50014 */ 
    {NV_ID_DRV_USB_DYNAMIC_PID_TYPE_PARAM},      /* TYPE_NDIS_STATE   50091 */ 
    {NV_ID_DRV_WEBNAS_SD_WORK_MODE},             /* TYPE_SD_MODE   51 */ 
    {NULL},                                      /* TYPE_APP_INFO   50418 */ 
    {NULL},                                      /* TYPE_SSID1_INFO   50290*/   /*50290-50295为高通的NV，balong上不使用*/         
    {NULL},                                      /* TYPE_WIFI_PASSWD1_WPA   50291 */ 
    {NULL},                                      /* TYPE_WIFI_PASSWD1_WEP   50292 */ 
    {NULL},                                      /* TYPE_SSID2_INFO   50293 */ 
    {NULL},                                      /* TYPE_WIFI_PASSWD2_WPA   50294 */ 
    {NULL},                                      /* TYPE_WIFI_PASSWD2_WEP   50295 */ 
    {NV_ID_DRV_WPS_PIN},                         /* TYPE_APPIN_INFO   balong为0xCB8D*/ 
    {NULL},                                      /* TYPE_GPS_ONOFF   50280 */ 
    {NV_ID_DRV_MULTI_WIFI_STATUS_SSID},          /* TYPE_MULT_WIFI_BASIC   9111 */
    {NV_ID_DRV_MULTI_WIFI_KEY},                  /* TYPE_MULT_WIFI_SEC   9110 */
    {NV_ID_DRV_VER_FLAG},                        /* TYPE_FACTORY_MODE   36 */
    {NV_ID_TELNET_SWITCH_I},                     /* TYPE_CPE_TELNET_SWITCH_NVIM_STRU   50501 */
    /*以下为高通平台nv，balong不涉及*/
    {NULL},                                      /*TYPE_FOTA_PPP_APN*/ 
    {NULL},                                      /*TYPE_FOTA_PPP_USER_ID*/ 
    {NULL},                                      /*TYPE_FOTA_PPP_PASSWORD*/ 
    {NULL},                                      /*TYPE_FOTA_PPP_AUTH*/ 
    {NULL},                                      /*TYPE_FOTA_SERVER_IP*/ 
    {NULL},                                      /*TYPE_FOTA_SERVER_PORT*/ 
    {NULL},                                      /*TYPE_FOTA_MODE*/ 
    {NULL},                                      /*TYPE_FOTA_DETECT_COUNT*/ 
    {NULL},                                      /*TYPE_FOTA_NWTIME*/ 
    {NULL},                                      /*TYPE_FOTA_TIMER*/ 
    {NULL},                                      /*TYPE_FOTA_INTERVAL*/ 
    {NULL},                                      /*TYPE_FOTA_SMS_CONFIG*/ 
    {NULL},                                      /*TYPE_FOTA_TIME_STAMP*/ 
    {NULL},                                      /*TYPE_FOTA_RSA_PUB_MODE*/ 
    {NULL},                                      /*TYPE_FOTA_RSA_PUB_EXP*/ 
    {NULL},                                      /*TYPE_FOTA_SMS_FLAG*/ 
    /*以上为高通平台nv，balong不涉及*/

    {NV_ID_DRV_NV_FACTORY_INFO_I},               /* TYPE_NV_FACTORY_INFO_I   114 */
    {NV_ID_MSP_MAC_ADDR},                        /* TYPE_NV_HUAWEI_BT_INFO_I   50014*/
    {NV_ID_DATA_LOCK_I},                         /* TYPE_DATA_LOCK_HASH_VAL*/
    {NV_ID_DRV_MAC_NUM},                         /* TYPE_WIFI_MAC_NUM  50517*/
    {NV_ID_MSP_MAC_ADDR},                        /* TYPE_LAN_MAC       50014*/
    {NV_ID_DRV_MAC_NUM},                         /* TYPE_LAN_MAC_NUM   50517*/
    {NULL/*nv后续添加*/},                        /* TYPE_TTS_CFG*/
    {NULL},                                      /* TYPE_NV_MAX */
}; 
/*lint +e64 +e156 */

typedef struct
{
    unsigned char lanmac_num;        /* LAN_MAC/Cradle MAC个数*/
    unsigned char wifimac_num;       /* WIFI_MAC 个数*/
    unsigned char btmac_num;        /* BLUETOOTH_MAC个数*/
    unsigned char usbmac_num;       /* USB_MAC个数*/
    unsigned char reserve[4];          /*预留字段*/
}NV_MAC_NUM_STRU;

#endif

#if (FEATURE_ON == MBB_COMMON)
/* 修改为与应用匹配的结构体 */
struct nv_data_stru
{
    TYPE_KERNEL_NV_INFO type;
    unsigned int        len;
    unsigned int        rst;
    unsigned char       data[NV_MAX_USER_BUFF_LEN];
};
#else
struct nv_data_stru
{
    unsigned int nvid;
    unsigned int len;
    unsigned char  data[NV_MAX_USER_BUFF_LEN];
};

#endif

#define NV_HEAD_LEN            (sizeof(struct nv_data_stru) - NV_MAX_USER_BUFF_LEN)

static struct nv_dev nv_cdev = {0};
static unsigned int nvchar_major;
static struct class* nv_class;
/*lint -save -e438*/
/*lint -save -e745 -e601 -e49 -e65 -e64 -e533 -e830*/
ssize_t nvdev_open(struct inode * inode, struct file * file);
ssize_t nvdev_release(struct inode * inode, struct file * file);
ssize_t nvdev_read(struct file *file, char __user *buf, size_t count,
			loff_t *ppos);
ssize_t nvdev_write(struct file *file, const char __user *buf, size_t count,
			loff_t *ppos);

static struct file_operations nv_fops = {
    .owner   = THIS_MODULE,
    .read    = nvdev_read,
    .write   = nvdev_write,
    .open    = nvdev_open,
    .release = nvdev_release,
};

ssize_t nvdev_open(struct inode *inode, struct file *file)/*lint !e830*/
{
    struct nv_dev* dev;

    dev = (struct nv_dev*)container_of(inode->i_cdev,struct nv_dev,cdev);
    file->private_data = dev;
    return 0;
}
ssize_t nvdev_release(struct inode *inode, struct file *file)
{
    inode = inode;
    file  = file;
    return 0;
}

#if(FEATURE_ON == MBB_COMMON)
#define MAC_STR_LEN    (17)    /*XX:XX:XX:XX:XX:XX实际长度是12+5*/
#define MAC_HALF_NUM   (3 )  
#define MAC_ARRAY_LEN  (32) 
#define BITS_OF_BYTE   (8 )
#define BYTE_OF_NUM    (3 )    /*XX:的长度*/
/************************************************************************
func: 基MAC(mac_input)加offset后返回给mac_intput , mac格式: XX:XX:XX:XX:XX:XX
input: mac_input: 基MAC   nv_len: 上层传递下来要读取NV的长度
output: None
return: None
Example: 
AA:BB:CC:00:00:00 AA:BB:CC:00:00:01 AA:BB:CC:00:00:02 AA:BB:CC:00:00:03
AA:BB:CC:FF:FF:FE AA:BB:CC:FF:FF:FF AA:BB:CC:00:00:00 AA:BB:CC:00:00:01
AA:BB:CC:FF:FE:FE AA:BB:CC:FF:FE:FF AA:BB:CC:FF:FF:00 AA:BB:CC:FF:FF:01
************************************************************************/
/*lint -save -e160 -e506 -e522 -e527 */
void BaseMACToWLAN( char* mac_input, int offset, unsigned long nv_len )
{
    unsigned long temp = 0;
    char array_mac[MAC_ARRAY_LEN] = {0};
    int  cnt = 0;
    unsigned long low_mac = 0;
    char array_temp[4] = {0};

    if ( MAC_STR_LEN >= nv_len )
    {
        printk( KERN_ERR "%s, nv_len not enough: %u", __func__, nv_len );
    }

    strncpy( array_mac, mac_input, MAC_STR_LEN ); 
   
    for ( cnt = 0; cnt < MAC_HALF_NUM; cnt++ )
    {
        sscanf( ( char * )( array_mac + ( cnt + MAC_HALF_NUM ) * BYTE_OF_NUM ), "%X", &temp ); 
        
        low_mac |= (unsigned long)( temp << ( ( MAC_HALF_NUM - cnt - 1 ) * BITS_OF_BYTE ) ); 
    }

    low_mac += offset;

    for ( cnt = 0; cnt < MAC_HALF_NUM; cnt++ )
    {
        temp = (unsigned char)( low_mac >> ( ( MAC_HALF_NUM - cnt - 1 ) * BITS_OF_BYTE ) );
        memset( array_temp, 0, sizeof(array_temp) );
        sprintf( array_temp, "%02X", (unsigned char)temp );
        memcpy( ( array_mac + ( cnt + MAC_HALF_NUM ) * BYTE_OF_NUM ), array_temp, 2 ); 
    }

    /*保存末尾'\0'*/
    array_mac[MAC_STR_LEN] = 0;

    memcpy( mac_input, array_mac, MAC_STR_LEN + 1 );
}
/*lint -restore*/


/*****************************************************************************
 函 数 名  : get_mac_offset
 功能描述  : 获取设备第一个mac地址相对于基mac地址的偏移量
 输入参数  : type:设备类型，wifi，blooth，lan等；
             data:读取的nv 50517里的值
 输出参数  : 无
 返 回 值  : 偏移量
 说    明  : 偏移量的计算如下: nv 50014保存一个基mac地址BASE_MAC，nv50517保存每种设备
             mac的个数，然后如下计算:
             LAN_MAC :[BASE_MAC,BASE_MAC+ lanmac_num)
             WIFI_MAC:[BASE_MAC+ lanmac_num,BASE_MAC+ lanmac_num+ wifimac_num)
             BLUETOOTH_MAC:
                      [BASE_MAC+lanmac_num+wifimac_num,
                       BASE_MAC+lanmac_num+wifimac_num+ btmac_num)
             USB_MAC:
                      [BASE_MAC+lanmac_num+wifimac_num+ btmac_num,
                       BASE_MAC+lanmac_num+wifimac_num+ btmac_num+ usbmac_num)  
             
*****************************************************************************/
int get_mac_offset(TYPE_KERNEL_NV_INFO type,void * data)
{
    int offset = 0;
    NV_MAC_NUM_STRU * mac_temp = data;
    switch(type)
    {
        case TYPE_LAN_MAC:
            offset = 0;
            break;
        case TYPE_MAC_ADDR:
            offset = mac_temp->lanmac_num;
            break;
        case TYPE_NV_HUAWEI_BT_INFO_I:
            offset = mac_temp->wifimac_num + mac_temp->lanmac_num;
            break;       
        default:
            break;
    }
    return offset;

}

/*****************************************************************************
 函 数 名  : get_mac_num
 功能描述  : 获取输入设备mac 地址的个数
 输入参数  : type:设备类型，wifi，blooth，lan等；
             data:读取的nv 50517里的值
 输出参数  : 无
 返 回 值  : mac地址个数
 *****************************************************************************/
int get_mac_num(TYPE_KERNEL_NV_INFO type,void * data)
{
    int num = 0;
    NV_MAC_NUM_STRU * mac_temp = data;
    switch(type)
    {
        case TYPE_WIFI_MAC_NUM:
            num = mac_temp->wifimac_num;
            break;
        case TYPE_LAN_MAC_NUM:
            num = mac_temp->lanmac_num;
            break;
        default:
            break;
    }
    return num;
}
#endif

/*lint -restore +e745 +e601 +e49 +e65 +e64 +e533 +e830*/
/*
data stru:
    ---------------------------------------------
    | NV ID | data length |     data      |
    ---------------------------------------------
*/

/*lint -save --e{529,527}*/
ssize_t nvdev_read(struct file *file,char __user *buf, size_t count,
			loff_t *ppos)
{
    ssize_t ret;
    struct nv_data_stru* kbuf = NULL;
#if(FEATURE_ON == MBB_COMMON)
    unsigned long   k_nvid = 0;
    unsigned long   k_nvlen = 0;
    unsigned long   mac_num = 0;
    unsigned char   mac_offset = 0; 
    NV_MAC_NUM_STRU nv_mac_num ={0};
#endif

    if((NULL == buf)||(count <= NV_HEAD_LEN)||(count > NV_MAX_USER_BUFF_LEN))/*lint !e737*/
    {
        return BSP_ERR_NV_INVALID_PARAM;
    }

    kbuf = (struct nv_data_stru*)kmalloc(count+1,GFP_KERNEL);
    if(NULL == kbuf)
    {
        return BSP_ERR_NV_MALLOC_FAIL;
    }

    if(copy_from_user(kbuf,buf,count))
    {
        kfree(kbuf);
        return -1;
    }
#if(FEATURE_ON == MBB_COMMON)
    /*BEGIN EUAP 归一化*/

    if ( TYPE_NV_MAX <= kbuf->type )
    {
        printk( KERN_ERR "kbuf->type is err: %u", kbuf->type );
        kfree(kbuf);
        return -1;
    }
   
    k_nvid = nv_index_table[kbuf->type];
    
    if(TYPE_WIFI_MAC_NUM == kbuf->type || TYPE_LAN_MAC_NUM == kbuf->type)
    {
        k_nvlen = sizeof(NV_MAC_NUM_STRU);
    }
    else
    {
        k_nvlen = count-(sizeof(TYPE_KERNEL_NV_INFO) + sizeof(unsigned int) + sizeof(unsigned int));
    }
    
    /*表中NV不支持直接返回*/
    if((0 == k_nvlen) || ((unsigned long )NULL == k_nvid))
    {
        printk(KERN_ERR "The request nv is not support here or para is err, kbuf->type = %d,count = %d\n", \
                         kbuf->type,count);
        kfree(kbuf);
        return -1;
    }

    /*如果是读取mac地址，先获取mac偏移量*/
    if(TYPE_MAC_ADDR == kbuf->type || TYPE_NV_HUAWEI_BT_INFO_I == kbuf->type || TYPE_LAN_MAC == kbuf->type)
    {
        ret = (ssize_t)bsp_nvm_read(NV_ID_DRV_MAC_NUM,(u8*)&nv_mac_num,sizeof(NV_MAC_NUM_STRU));
        if(ret)
        {
            printk( KERN_ERR "[%s]nv read err!\n",__func__);
            kfree(kbuf);
            return ret;
        }      
        mac_offset = get_mac_offset(kbuf->type,&nv_mac_num);       
    }
        
    /* coverity[tainted_data] */
    ret = (ssize_t)bsp_nvm_read(k_nvid,kbuf->data,k_nvlen);
    kbuf->len = k_nvlen;
#else
    ret = (ssize_t)bsp_nvm_read(kbuf->nvid,kbuf->data,kbuf->len);
#endif
    if(ret)
    {
        kfree(kbuf);
        return ret;
    }


#if (FEATURE_ON == MBB_COMMON)
    kbuf->rst = NV_OK;
#endif



#if (FEATURE_ON == MBB_COMMON)
    if ( TYPE_MAC_ADDR == kbuf->type || TYPE_NV_HUAWEI_BT_INFO_I == kbuf->type || TYPE_LAN_MAC == kbuf->type )
    {
        BaseMACToWLAN( (char *)kbuf->data, mac_offset, k_nvlen );
    }

    else if(TYPE_WIFI_MAC_NUM == kbuf->type || TYPE_LAN_MAC_NUM == kbuf->type)
    {
        mac_num = get_mac_num(kbuf->type,kbuf->data);
        memset(kbuf->data,0x0,sizeof(kbuf->data));
        kbuf->data[0] = mac_num;
        kbuf->len = 1;
        printk("[%s]kbuf->type=%d,mac_num=%d\n",__func__,kbuf->type,kbuf->data[0]);
    }
#endif
    
    ret = (ssize_t)copy_to_user(buf,kbuf,count);
    if(ret)
    {
        kfree(kbuf);
        return ret;
    }

    kfree(kbuf);
    return (ssize_t)count;
}
/*lint -restore*/

/*lint -save --e{529,527}*/
ssize_t nvdev_write(struct file *file,const char __user *buf, size_t count,
			loff_t *ppos)
{
    ssize_t ret;
    struct nv_data_stru* kbuf = NULL;
#if(FEATURE_ON == MBB_COMMON)
    unsigned long k_nvid = 0;
    unsigned long k_nvlen = 0;
#endif

    if((NULL == buf)||(count <= NV_HEAD_LEN)||(count > NV_MAX_USER_BUFF_LEN))/*lint !e737*/
    {
        printk("invalid parameter!\n");
        return BSP_ERR_NV_INVALID_PARAM;
    }

    kbuf = (struct nv_data_stru*)kmalloc(count+1,GFP_KERNEL);
    if(NULL == kbuf)
    {
        return BSP_ERR_NV_MALLOC_FAIL;
    }

    if(copy_from_user(kbuf,buf,count))
    {
        printk("copy from user fail!\n");
        kfree(kbuf);
        return -1;
    }
#if(FEATURE_ON == MBB_COMMON)
    /*BEGIN EUAP 归一化*/

    if ( TYPE_NV_MAX <= kbuf->type )
    {
        printk( KERN_ERR "kbuf->nvid is err: %u", kbuf->type );
        kfree(kbuf);
        return -1;
    }
    
    k_nvid = nv_index_table[kbuf->type];
    k_nvlen = count-(sizeof(TYPE_KERNEL_NV_INFO) + sizeof(unsigned int) + sizeof(unsigned int));

    /*表中NV不支持直接返回*/
    if((0 == k_nvlen) ||((unsigned long )NULL == k_nvid))
    {
        printk(KERN_ERR "The request nv is not support here or para is err, kbuf->type = %d,count = %d\n", \
                         kbuf->type,count);
        kfree(kbuf);
        return -1;
    }

    /* coverity[tainted_data] */
    ret = (ssize_t)bsp_nvm_write(k_nvid,kbuf->data,k_nvlen);
#else
    ret = (ssize_t)bsp_nvm_write(kbuf->nvid,kbuf->data,kbuf->len);
#endif
    if(ret)
    {
        kfree(kbuf);
        return ret;
    }

    kfree(kbuf);
    return (ssize_t)count;
}
/*lint -restore*/
/*lint -save -e527 -e533*/
void nvchar_dev_setup_cdev(struct nv_dev* dev,struct file_operations* fops)
/*lint -restore*/
{
    int ret;
    unsigned int devno = MKDEV(nvchar_major,0);


    cdev_init(&dev->cdev,fops);
    dev->cdev.owner= THIS_MODULE;
    dev->cdev.ops = fops;

    ret = cdev_add(&dev->cdev,devno,1);
    if(ret)
    {
        printk("add nv fail!\n");
        return ;
    }

    nv_class = class_create(THIS_MODULE,NVCHAR_DEVICE_CLASS);
    if(IS_ERR(nv_class))
    {
        printk("class create failed!\n");
        return ;
    }

    device_create(nv_class, NULL, devno, NULL, NVCHAR_DEVICE_NAME);
}



/*nv device init*/
/*lint -save --e{529}*/
int nvchar_init(void)
{
    int ret;
    dev_t dev = 0;

    /*dynamic dev num use*/
    ret = alloc_chrdev_region(&dev,0,1,NVCHAR_DEVICE_NAME);
    nvchar_major = MAJOR(dev);

    if(ret)
    {
        printk("nvchar failed alloc :%d\n",nvchar_major);
        return ret;
    }
    /*setup dev*/
    nvchar_dev_setup_cdev(&nv_cdev,&nv_fops);

    printk("nvchar init end! major: 0x%x\n",nvchar_major);
    return NV_OK;
}
/*lint -restore*/


#if 0
void nvchar_exit(void)
{
    cdev_del(&(nv_cdev.cdev));
    class_destroy(nv_class);
    unregister_chrdev_region(MKDEV(nvchar_major,0),1);
}
#endif


MODULE_AUTHOR("Hisilicon Drive Group");
MODULE_DESCRIPTION("Balong NV driver for Hisilicon");
MODULE_LICENSE("GPL");

/*lint -restore*/

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
