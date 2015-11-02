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
/*********************************问题单修改记录*******************************
  问题单号               修改人         修改时间           修改说明
******************************************************************************/
#include <linux/if_vlan.h>
#include <linux/if_ether.h>
#include <linux/rwlock_types.h>
#include <linux/rwlock.h>
#include <linux/timer.h>
#include <linux/skbuff.h>
#include <linux/phy.h>
#include <linux/ethtool.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/netlink.h>
#include "mbb_net.h"
#include "SysNvId.h"
#include <bsp_nvim.h>

/*MBB_FEATURE_ETH_PHY与MBB_FEATURE_ETH_SWITCH不能同时打开*/
#if (FEATURE_ON == MBB_FEATURE_ETH_PHY)
#if (FEATURE_ON == MBB_FEATURE_ETH_SWITCH)
Pls_check_MBB_FEATURE_ETH_SWITCH_or_MBB_FEATURE_ETH_PHY
#endif
#endif

#if (FEATURE_ON == MBB_CTF_COMMON)
#include <linux/fake/typedefs.h>
#include <linux/fake/osl.h>
#include <linux/fake/linux_osl.h>
#include <linux/fake/ctf/hndctf.h>

static ctf_t *eth_cih __attribute__ ((aligned(32))) = NULL;/* ctf instance handle */
static osl_t *eth_fake_osh = NULL;
unsigned int g_ifctf = 0;
#endif

#if (FEATURE_ON == MBB_ETH_PHY_LOWPOWER)
#define   REPORT_ETH_REMOVE      (0)
#define   REPORT_ETH_INSERT      (1)
#define   REPORT_POWEROFF_EN     (2)
#define   REPORT_POWEROFF_DIS    (3)
#define   ETH_ROUTE_STATE_UNKNOWN    (0xf)
#endif

#if ((FEATURE_ON == MBB_FEATURE_FASTIP) && (FEATURE_ON == MBB_ETH_STMMAC_TX_THREAD))
#include <linux/if.h>
#include <linux/if_vlan.h>
#include <linux/fastip/fastip.h>
#include <linux/ip.h>
fastip_t  __attribute__ ((aligned(32))) *eth_fastip_handle  = NULL;
#endif

#if (FEATURE_ON == MBB_FEATURE_ETH_SWITCH)
#define   WAN_PORT_INDEX     3
#define   NET_DEVICE_NAME   "et0"
#else
#define   NET_DEVICE_NAME   "eth0"
#endif

#define ETH_TRAFFIC_TIMES   3

/*多长时间查询端口的状态，单位为秒*/
#define PHY_LINK_TIME           1

/*组播地址的首字节为0x01*/
#define MUTICAST_MAC            0x01

/*目标端口的偏移量*/
#define DPORT_BYMASS             36

/*约定组播升级的目的端口为13456*/
#define MUTICAST_DPORT_1        0x34 
#define MUTICAST_DPORT_2        0x90

/*组播升级的MAGIC_NUMBER的偏移量*/
#define MAGIC_NUMBER_BYMASS     46 

/*组播报文魔数字*/
#define MAGIC_NUMBER            0xB0A555AA

#define MAC_CLONE_OFF           "OFF"
#define SZ_MAC_LENGTH           20   
#define MAC_CLONE_ENABLE        1
#define MAC_CLONE_DISABLE       0

/*ifctf的文件内容最大长度*/
#define IF_CTF_LENGTH           1
/*十进制*/
#define DECIMAL                 10

#define SYS_NODE_INT_BUF_LEN    6

#define LAN_PORT_1              1
#define LAN_PORT_2              2
#define LAN_PORT_3              3
#define LAN_PORT_4              4
#define NV_MAC_LENGTH           32

/*读写锁，避免全局变量被并发访问*/
rwlock_t mac_clone_lock; 

/*保存从用户空间传入的冒号分隔形式的用于克隆MAC地址*/
char sz_clone_macaddr[SZ_MAC_LENGTH] = {"00:00:00:00:00:00"};
char clone_mac_addr[ETH_ALEN] = {0};
char clone_init_addr[ETH_ALEN] = {0};
int  mac_clone_flag = MAC_CLONE_DISABLE;

/*标志当前是否检测到组播升级 0表示未检测到 1表示检测到*/
int g_muticast_flag = 0;

/*标志当前LAN侧是否存在流量*/
int g_traffic_flag = 0;

#if (FEATURE_ON == MBB_FEATURE_ETH_SWITCH)
extern void rtk_reset_allport(int wan_exclude);
extern int rtk_poll_port_status(unsigned int *all_port_status);
#endif
/*查询eth端口状态的work*/
struct delayed_work s_ethstatusqueue;

static struct class *lan_class;
static struct device *lan_dev;

static int translate_mac_address(char *adr_str, char *adr_dst);
static void mbb_device_event_report(int event);

/*****************************************************************************
函数名：   get_aneg_speed
功能描述:  获取CPE 网口指定端口速率
输入参数:  端口索引
返回值：  速率值(1000/100/10) 
*****************************************************************************/
static int get_aneg_speed(int portIndex)
{
    struct phy_device *phydev = mbb_get_phy_device();
    int retv = 0;

    if (NULL != phydev)
    {
        if (phydev->drv->read_status)
        {
            phydev->drv->read_status(phydev);
        }

        if (phydev->link)
        {
            retv = phydev->speed;
        }
    }

    return retv;
}

/*****************************************************************************
函数名：   PhyATQryPortPara
功能描述:  装备单网口信息获取接口
输入参数:  端口索引
返回值：  NET_RET_OK(0)为ok 
*****************************************************************************/
int PhyATQryPortPara(NET_PORT_ST *PortPara)
{
#if (FEATURE_ON == MBB_FEATURE_ETH_PHY)
    if (NULL == PortPara)
    {
        return NET_RET_FAIL;
    }
    
    PortPara->total_port = LAN_PORT_1;
    PortPara->port_index = 0;
    PortPara->port_rate = get_aneg_speed(PortPara->port_index);

    return NET_RET_OK;
#else
    return NET_RET_FAIL;
#endif
}

/*****************************************************************************
函数名：   mbb_check_net_upgrade
功能描述:  判断报文是否是组播升级报文
输入参数:  skb报文
返回值：  NET_RET_OK(0)为ok 
*****************************************************************************/
int mbb_check_net_upgrade(struct sk_buff *skb)
{
    int iret = 0;
    unsigned short vlanid = 0;
    int vlanLen = 0;

    if (NULL == skb)
    {
        return NET_RET_FAIL;
    }

    /*在此判断一下是否为组播升级的报文*/
    if (0 == g_muticast_flag)
    {
#if (FEATURE_ON == MBB_FEATURE_ETH_SWITCH)
        vlanLen = VLAN_HLEN;
#endif
    /*如果第一个地址为组播地址*/
        if (MUTICAST_MAC == (skb->data[0] & MUTICAST_MAC))
        {
            /*比较端口号  两个字节*/
            if ((MUTICAST_DPORT_1 == skb->data[DPORT_BYMASS + vlanLen])
            && (MUTICAST_DPORT_2 == skb->data[DPORT_BYMASS + vlanLen + 1]))
            {
                /*比较魔数字 四个字节*/
                if (MAGIC_NUMBER == (*((unsigned int *)(skb->data + MAGIC_NUMBER_BYMASS + vlanLen))))
                {
                    printk("the packet is for muticast\n");
                    mbb_device_event_report(CRADLE_MUTI_CAST);

                    g_muticast_flag = 1;
                }
            }
        }
    }

    return NET_RET_OK;
}

/*****************************************************************************
函数名：   mbb_mac_clone_rx_restore
功能描述:  MAC层接收PPP报文时mac clone
输入参数:  skb报文
返回值：  NET_RET_OK(0)为ok 
*****************************************************************************/
int mbb_mac_clone_rx_restore(struct sk_buff *skb)
{
    int L3Offset = ETH_HLEN;

    if (NULL == skb)
    {
        return NET_RET_FAIL;
    }
    
    read_lock(&mac_clone_lock);

    if (MAC_CLONE_ENABLE == mac_clone_flag)
    {
#if (FEATURE_ON == MBB_FEATURE_ETH_SWITCH)
        L3Offset = VLAN_ETH_HLEN - sizeof(__be16);
#endif 

#if (FEATURE_ON == MBB_FEATURE_ETH_PHY)
        L3Offset = ETH_HLEN - sizeof(__be16);
#endif  

        /*如果是PPP发现报文或者会话报文就用克隆的MAC地址去替换*/
        if (((cpu_to_be16(ETH_P_PPP_DISC) == (*(unsigned short *)(skb->data + L3Offset)))
        || (cpu_to_be16(ETH_P_PPP_SES) == (*(unsigned short *)(skb->data + L3Offset))))
        && (0 == strncmp(skb->data, clone_mac_addr, ETH_ALEN)))
        {
            //LAN_DEBUG("mac clone in rx\r\n");
            memcpy(skb->data, clone_init_addr, ETH_ALEN); 
        }
    }

    read_unlock(&mac_clone_lock);

    return NET_RET_OK;
}

/*****************************************************************************
函数名：   mbb_mac_clone_tx_save
功能描述:  MAC层发送PPP报文时mac clone
输入参数:  skb报文
返回值：  NET_RET_OK(0)为ok 
*****************************************************************************/
int mbb_mac_clone_tx_save(struct sk_buff *skb)
{
    int L3Offset = ETH_HLEN;

    if (NULL == skb)
    {
        return NET_RET_FAIL;
    }

    read_lock(&mac_clone_lock);  

    if (MAC_CLONE_ENABLE == mac_clone_flag)
    {
#if (FEATURE_ON == MBB_FEATURE_ETH_SWITCH)
        L3Offset = VLAN_ETH_HLEN - sizeof(__be16);
#endif 

#if (FEATURE_ON == MBB_FEATURE_ETH_PHY)
        L3Offset = ETH_HLEN - sizeof(__be16);
#endif

        /*如果是PPP发现报文或者会话报文就用克隆的MAC地址去替换*/
        if ((cpu_to_be16(ETH_P_PPP_DISC) == (*(unsigned short *)(skb->data + L3Offset)))
        || (cpu_to_be16(ETH_P_PPP_SES) == (*(unsigned short *)(skb->data + L3Offset))))
        {           
            //LAN_DEBUG("mac clone in tx\r\n");
            memcpy(clone_init_addr, skb->data + ETH_ALEN, ETH_ALEN);  
            memcpy(skb->data + ETH_ALEN, clone_mac_addr, ETH_ALEN);
        }
    }

    read_unlock(&mac_clone_lock);

    return NET_RET_OK;
}

static int wan_mirror_port = 0;
static int wan_mirror_enable = 0;

extern struct net init_net;
#if (FEATURE_ON == MBB_FEATURE_ETH_WAN_MIRROR)
#if ((FEATURE_ON == MBB_CTF_COMMON) || (FEATURE_ON == MBB_FEATURE_FASTIP)) 
extern void set_ctf_wan_mirror_flags(int value);
#endif
#endif

/*****************************************************************************
函数名：   RNIC_WANMirror
功能描述:  打开WAN口镜像
输入参数:  enable使能与否
                        port端口号
返回值：  NET_RET_OK(0)为ok 
*****************************************************************************/
int RNIC_WANMirror(unsigned int enable, unsigned int port)
{   
#if (FEATURE_ON == MBB_FEATURE_ETH_SWITCH)
    if (port < LAN_PORT_1 || port > LAN_PORT_4)
    {        
        printk("port is illegal\n");
        return NET_RET_FAIL;
    }
#endif        

    wan_mirror_port = port;
    wan_mirror_enable = enable;
    
#if (FEATURE_ON == MBB_FEATURE_ETH_WAN_MIRROR)
#if ((FEATURE_ON == MBB_CTF_COMMON) || (FEATURE_ON == MBB_FEATURE_FASTIP))
    set_ctf_wan_mirror_flags(enable);
#endif
#endif
    return NET_RET_OK;
}

int RNIC_WanMirrorStatus()
{
    return wan_mirror_enable;
}

/*****************************************************************************
函数名：   RNIC_Map_To_Lan_Forward
功能描述:  WAN口镜像
输入参数:  skb报文
返回值：  NET_RET_OK(0)为ok 
*****************************************************************************/
int RNIC_Map_To_Lan_Forward(struct sk_buff *skb)
{
    struct net_device *dev = NULL;
    struct sk_buff *skb2 = NULL;
    int ret = 0;

#if (FEATURE_ON == MBB_FEATURE_ETH_SWITCH) 
    int vlan_id = 0;
#endif

    if (!wan_mirror_enable)
    {
        return NET_RET_FAIL;
    }

    if (NULL == skb)
    {
        return NET_RET_FAIL;
    }  

    /*打开mirror需要关闭ctf*/
#if (FEATURE_ON == MBB_CTF_COMMON)
    if (!g_ifctf)
    {
        g_ifctf = 1;
    }
#endif    
            
    skb2 = skb_copy(skb, GFP_ATOMIC);
    if(skb2 == NULL)
    {
        printk("\r\n%s %d skb_copy failed",__FUNCTION__,__LINE__);
        return NET_RET_FAIL;
    }

#if (FEATURE_ON == MBB_FEATURE_ETH_SWITCH)
    /*获取镜像的lan口设备节点*/
    switch(wan_mirror_port)
    {
        case LAN_PORT_1:
            dev = __dev_get_by_name(&init_net, "eth0.2");
            vlan_id = VLAN_PORT0;
            break;
        case LAN_PORT_2:
            dev = __dev_get_by_name(&init_net, "eth0.3");            
            vlan_id = VLAN_PORT1;
            break;
        case LAN_PORT_3:
            dev = __dev_get_by_name(&init_net, "eth0.4");
            vlan_id = VLAN_PORT2;
            break;
        case LAN_PORT_4:
            dev = __dev_get_by_name(&init_net, "eth0");
            vlan_id = VLAN_WAN_PORT;
            break;
        default:
            printk("\r\nwan mirror get dst failed");
            kfree_skb(skb2);
            return NET_RET_FAIL;
    }
#endif

#if (FEATURE_ON == MBB_FEATURE_ETH_PHY) 
    dev = __dev_get_by_name(&init_net, "eth0");
#endif

    if(dev)
    {    
        unsigned long flags;
        
        /*发送前的准备工作*/
        skb2->dev = dev;

        /*ether协议头*/
        skb2->protocol = htons(ETH_P_802_3);

        /*伪造组播报文，防止PC wireshark抓不到报文*/
        skb2->data[0] |= 0x01;

#if (FEATURE_ON == MBB_FEATURE_ETH_SWITCH)        
        skb2 = __vlan_put_tag(skb2, vlan_id);
#endif

        local_irq_save(flags);
        local_irq_enable();
        dev_queue_xmit(skb2);
        local_irq_restore(flags);
                 
        return NET_RET_OK;
    }
    else
    {
        printk("\r\n%s %d didn't find port %d dev", __FUNCTION__, __LINE__, wan_mirror_port);
        kfree_skb(skb2);

        return NET_RET_FAIL;
    }

}

/*****************************************************************************
函数名：   get_carrier_state
功能描述: net_state节点处理函数
输入参数:  
返回值：
*****************************************************************************/
static ssize_t get_carrier_state(struct device *dev, struct device_attribute *attr, char *buf)
{
    int port_line_state = CRADLE_REMOVE;

#if (FEATURE_ON == MBB_FEATURE_ETH_PHY) 
    struct phy_device *phydev = mbb_get_phy_device();

    if (NULL != phydev)
    {
        if (phydev->drv->read_status)
        {
            phydev->drv->read_status(phydev);
        }

        if (phydev->link)
        {
            port_line_state = CRADLE_INSERT;
        }    
    }
#endif    

#if (FEATURE_ON == MBB_FEATURE_ETH_SWITCH) 
    LAN_ETH_STATE_INFO_ST wan_status;

    rtk_get_port_status_info(&wan_status, WAN_PORT_INDEX, 1);

    if (wan_status.link_state)
    {
        port_line_state = CRADLE_INSERT;
    }
#endif

    return snprintf(buf, SYS_NODE_INT_BUF_LEN, "%0x\n", port_line_state); 
}
 
static DEVICE_ATTR(net_state, S_IRUGO, get_carrier_state, NULL);
 
/*****************************************************************************
函数名：   set_clone_mac
功能描述: 文件clone_mac状态发生变化时，调用此函数判断是否开启MAC地址克隆功能
输入参数:  
返回值：
*****************************************************************************/
static ssize_t set_clone_mac(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
    char sz_macaddr[SZ_MAC_LENGTH] = {0};

    strncpy(sz_clone_macaddr, buf, SZ_MAC_LENGTH - 1);
    strncpy(sz_macaddr, sz_clone_macaddr, SZ_MAC_LENGTH - 1);
    write_lock(&mac_clone_lock);
    mac_clone_flag = MAC_CLONE_DISABLE; /*先设置为0*/

    if (strncmp(buf, MAC_CLONE_OFF, strlen(MAC_CLONE_OFF)))
    {
        /*将冒号间隔的字符串格式MAC地址转换为6字节格式*/
        if (0 == translate_mac_address(sz_macaddr, clone_mac_addr))
        {
            mac_clone_flag = MAC_CLONE_ENABLE;
        }  
    }

    write_unlock(&mac_clone_lock);

    return count;
}

/*****************************************************************************
函数名：  get_clone_mac
功能描述: 获取设置的clone mac
输入参数:  
返回值：
*****************************************************************************/
static ssize_t get_clone_mac(struct device *dev, struct device_attribute *attr, char *buf)
{
    return snprintf(buf, SZ_MAC_LENGTH, "%s\n", sz_clone_macaddr);
}

static DEVICE_ATTR(clone_mac, S_IRUGO | S_IWUSR, get_clone_mac, set_clone_mac);
 
#if 0 
/*全局变量 用来控制下行数据是否走ctf模块 0表示走 1表示不走*/
int g_if_eth_ctf;

 /*用来给应用控制是否走ctf*/
static ssize_t set_if_ctf(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
    char temp_if_ctf[IF_CTF_LENGTH + 1] = {0};

    strncpy(temp_if_ctf, buf, IF_CTF_LENGTH);
    temp_if_ctf[IF_CTF_LENGTH] = '\0';
    g_if_eth_ctf = simple_strtol(temp_if_ctf, NULL, DECIMAL);

    printk("g_if_eth_ctf %d\n",g_if_eth_ctf);

    return count;
}
 
static ssize_t get_if_ctf(struct device *dev, struct device_attribute *attr, char *buf)
{
    return sprintf(buf, "%d\n", g_if_eth_ctf);
}
 
static DEVICE_ATTR(if_ctf, S_IRUGO | S_IWUSR, get_if_ctf, set_if_ctf);
#endif


/*****************************************************************************
函数名：  get_muticast_flag
功能描述: 组播升级报文标识
输入参数:  
返回值：
*****************************************************************************/
static ssize_t get_muticast_flag(struct device *dev, struct device_attribute *attr, char *buf)
{
    printk("get_muticast_flag = %d\n", g_muticast_flag);
    return snprintf(buf, SYS_NODE_INT_BUF_LEN, "%0x\n", g_muticast_flag);
}
 
static DEVICE_ATTR(muticast_flag, S_IRUGO | S_IWUSR, get_muticast_flag, NULL);

/*****************************************************************************
函数名：  get_traffic_flag
功能描述: 网口流量标识
输入参数:  
返回值：
*****************************************************************************/
static ssize_t get_traffic_flag(struct device *dev, struct device_attribute *attr, char *buf)
{
    printk("get_traffic_flag = %d\n", g_traffic_flag);
    return snprintf(buf, SYS_NODE_INT_BUF_LEN, "%0x\n", g_traffic_flag);
}
 
static DEVICE_ATTR(traffic_flag, S_IRUGO, get_traffic_flag, NULL);

/*****************************************************************************
函数名：  mbb_eth_state_report
功能描述: DEVICE_ID_CRADLE消息沙包
输入参数:  
返回值
*****************************************************************************/
void mbb_eth_state_report(int new_state)
{
    if (new_state)
    {
        mbb_device_event_report(CRADLE_INSERT);
    }
    else
    {
        mbb_device_event_report(CRADLE_REMOVE);
    } 
}

/*****************************************************************************
函数名：  mbb_eth_traffic_status
功能描述:  网口LED控制函数
输入参数:  all_port_status所有端口状态
返回值：
*****************************************************************************/
void mbb_eth_traffic_status(unsigned int all_port_status)
{
    DEVICE_EVENT stusbEvent;
    struct net_device *pstDev =  dev_get_by_name(&init_net, NET_DEVICE_NAME);
    static int eth_rx_packets = 0;
    static int eth_tx_packets = 0;
    static int blink_delay_times = 0;
    static int on_delay_times = 0;

   /* 获取不到网口设备*/
    if (NULL == pstDev)
    {       
        return;
    }

    stusbEvent.device_id = DEVICE_ID_TRAFFIC;
    stusbEvent.len = 0;

     /*所有端口未连接*/
    if (!all_port_status)
    {       
        if (ETH_TRAFFIC_OFF != g_traffic_flag)
        {
            stusbEvent.event_code = ETH_TRAFFIC_OFF;
            device_event_report(&stusbEvent, sizeof(DEVICE_EVENT));
            blink_delay_times = 0;
            on_delay_times = 0;

            g_traffic_flag = ETH_TRAFFIC_OFF;
        }
        dev_put(pstDev);
        return;
    }

    /*有数据变化*/
    if ( (pstDev->stats.rx_packets != eth_rx_packets)  
        || (pstDev->stats.tx_packets != eth_tx_packets))
    {
        /*假如原来状态为不闪烁*/
        if (ETH_TRAFFIC_BLINK != g_traffic_flag)
        {
            /*防止状态切换太快，消息太多*/
            if (blink_delay_times++ >= ETH_TRAFFIC_TIMES)
            {
                stusbEvent.event_code = ETH_TRAFFIC_BLINK;
                device_event_report(&stusbEvent, sizeof(DEVICE_EVENT));
                blink_delay_times = 0;
                on_delay_times = 0;

                g_traffic_flag = ETH_TRAFFIC_BLINK;
                LAN_DEBUG("eth report lan ETH_TRAFFIC_BLINK\r\n");
            }
        }

        eth_rx_packets = pstDev->stats.rx_packets;
        eth_tx_packets = pstDev->stats.tx_packets;
    }
    else   /* 无数据变化*/
    {
        if (ETH_TRAFFIC_ON != g_traffic_flag)/*假如原来状态为闪烁*/
        {
            /*防止状态切换太快，消息太多*/
            if (on_delay_times++ >= ETH_TRAFFIC_TIMES)
            {
                stusbEvent.event_code = ETH_TRAFFIC_ON;
                device_event_report(&stusbEvent, sizeof(DEVICE_EVENT));
                blink_delay_times = 0;
                on_delay_times = 0;

                g_traffic_flag = ETH_TRAFFIC_ON;
                LAN_DEBUG("eth report lan ETH_TRAFFIC_ON\r\n");
            }
            
        }
    }

    dev_put(pstDev);
}
/*****************************************************************************
函数名：  eth_port_status_poll
功能描述:  端口状态查询函数
输入参数:  
返回值：
*****************************************************************************/
static void eth_port_status_poll(struct work_struct *work)
{
    unsigned int all_port_status = 0;

   /*对于多网口在这里上报网口状态，单网口在phy.c的状态机中上报*/
#if (FEATURE_ON == MBB_FEATURE_ETH_SWITCH)
    int phylinkstate = 0;
    DEVICE_EVENT stusbEvent;
    
    phylinkstate = rtk_poll_port_status(&all_port_status);

    switch (phylinkstate)
    {
        case ETH_LAN_DOWN:
        case ETH_LAN_UP:    
            stusbEvent.device_id = DEVICE_ID_ETH;
            stusbEvent.len = 0;
            stusbEvent.event_code = 0;

            LAN_DEBUG("eth report lan change\r\n");

            device_event_report(&stusbEvent,sizeof(DEVICE_EVENT));
            break;
        case ETH_WAN_DOWN:
            mbb_eth_state_report(0);
            break;
        case ETH_WAN_UP:
            mbb_eth_state_report(1);   
            break;
        default:
            break;
    }
#endif

#if (FEATURE_ON == MBB_FEATURE_ETH_PHY)
    struct phy_device *phydev = mbb_get_phy_device();

    if (NULL == phydev)
    {
        return;
    }   

    all_port_status = phydev->link;
#endif

    /*网口流量的状态*/
    mbb_eth_traffic_status(all_port_status);

    schedule_delayed_work(&s_ethstatusqueue, PHY_LINK_TIME * HZ);  
}

/*****************************************************************************
函数名：  hw_net_sysfs_init
功能描述: lan_usb节点创建
输入参数:  
返回值：NET_RET_OK(0)为ok 
*****************************************************************************/
int hw_net_sysfs_init(void)
{
    int err;
    static int sysfs_init = 0;

    /*防止多次初始化*/
    if (sysfs_init > 0)
    {
        return NET_RET_OK;
    }
    
    lan_class = class_create(THIS_MODULE, "lan_usb");
    lan_dev = device_create(lan_class, NULL, MKDEV(0, 0), NULL, "lan");
    
    err = device_create_file(lan_dev, &dev_attr_net_state);
    if(err)
    {
        printk("et_net_fs_init create lan_usb file error\n");
        return err;
    }
    err = device_create_file(lan_dev, &dev_attr_clone_mac);
    if(err)
    {
        printk("et_net_fs_init create clone_mac file error\n");
        return err;
    }

#if 0
    err = device_create_file(lan_dev, &dev_attr_if_ctf);
    if(err)
    {
        printk("et_net_fs_init create if_ctf file error\n");
        return err;
    }
#endif

    err = device_create_file(lan_dev, &dev_attr_muticast_flag);
    if(err)
    {
        printk("et_net_fs_init create muticast_flag file error\n");
        return err;
    }

    err = device_create_file(lan_dev, &dev_attr_traffic_flag);
    if(err)
    {
        printk("et_net_fs_init create traffic_flag file error\n");
        return err;
    }
    
    rwlock_init(&mac_clone_lock);

    INIT_DELAYED_WORK(&s_ethstatusqueue, eth_port_status_poll);
    schedule_delayed_work(&s_ethstatusqueue, PHY_LINK_TIME * HZ);

    sysfs_init++;
    
    return NET_RET_OK;
}

/*****************************************************************************
函数名：  hw_net_sysfs_init
功能描述: lan_usb节点删除
输入参数:  
返回值：NET_RET_OK(0)为ok 
*****************************************************************************/
void hw_net_sysfs_uninit(void)
{
    device_remove_file(lan_dev, &dev_attr_net_state);

    device_remove_file(lan_dev, &dev_attr_clone_mac);

#if 0
    device_remove_file(lan_dev, &dev_attr_if_ctf);
#endif

    device_remove_file(lan_dev, &dev_attr_muticast_flag);

    device_remove_file(lan_dev, &dev_attr_traffic_flag);

    device_destroy(lan_class, MKDEV(0, 0));
    
    class_destroy(lan_class);
    return;
}

#define MAC_ADDR_LEN    17
#define HALF_BYTE_LEN   4
#define ASCI_A          'A'        
#define ASCI_F          'F'
#define ASCI_a          'a'
#define ASCI_f          'f'
#define ASCI_0          '0'
#define ASCI_9          '9'
#define CHAR_NUM1       1
#define CHAR_NUM2       2
#define CHAR_NUM3       3

/*****************************************************************************
函数名：  translate_mac_address
功能描述: 用于将冒号间隔的字符串格式MAC地址转换为6字节格式

输入参数:  
返回值：NET_RET_OK(0)为ok 
*****************************************************************************/
static int translate_mac_address(char *adr_str, char *adr_dst)
{
    int ret = 0;
    int i = 0, j = 0;
    int data;
    unsigned char c = 0;

    if (!adr_dst)
    {
        return NET_RET_FAIL;
    }
    
    if (!adr_str)
    {
        return NET_RET_FAIL;
    }
    
    data = 0;
    i = 0;

    while(i < MAC_ADDR_LEN)
    {
        c = adr_str[i];
        data = data << HALF_BYTE_LEN;
        j = i % CHAR_NUM3;
        if(CHAR_NUM2 == j)
        {
            if(':' == c)
            { 
                i++;
                continue;
            }
            else
            {
                ret = NET_RET_FAIL;
                break;
            } 
        }
        if(ASCI_0 <= c && ASCI_9 >= c)
        {
            data += c - ASCI_0;
        }    
        else if(ASCI_A <= c && ASCI_F >= c)
        {
            data += c - ASCI_A + DECIMAL;
        }    
        else if(ASCI_a <= c && ASCI_f >= c)
        {
            data += c - ASCI_a + DECIMAL;
        }    
        else
        {
            ret = NET_RET_FAIL;
            break;
        }
        if(CHAR_NUM1 == j)
        {
            adr_str[i / CHAR_NUM3] = data;
            data = 0;
        }
        i++;
    }

    if(NET_RET_FAIL != ret)
    {
        memcpy(adr_dst, adr_str, ETH_ALEN);
    }
    else
    {
        printk("%s: error mac addr\n", __FUNCTION__);
    }

    return ret;
}

/*****************************************************************************
函数名：  mbb_device_event_report
功能描述: DEVICE_ID_CRADLE消息沙包
输入参数:  
返回值
*****************************************************************************/
static void mbb_device_event_report(int event)
{
    DEVICE_EVENT stusbEvent;
    stusbEvent.device_id = DEVICE_ID_CRADLE;
    stusbEvent.len = 0;
    stusbEvent.event_code = event;

    LAN_DEBUG("eth report device event:%d\r\n", event);

    device_event_report(&stusbEvent,sizeof(DEVICE_EVENT));
}

#if (FEATURE_ON == MBB_CTF_COMMON)
static void eth_ctf_detach(ctf_t *ci, void *arg)
{
    eth_cih = NULL;
    return;
}

unsigned int eth_ctf_forward(struct sk_buff *skb)
{
    /* use slow path if ctf is disabled */
    if (!CTF_ENAB(eth_cih))
    {
        return (BCME_ERROR);
    }
    /* try cut thru first */
    if (BCME_ERROR != ctf_forward(eth_cih, skb, skb->dev))
    {
        return (BCME_OK);
    }

    /* clear skipct flag before sending up */
    PKTCLRSKIPCT(NULL /* et->osh*/, skb);

    return (BCME_ERROR);
}

/*****************************************************************************
函数名：  mbb_ctf_forward
功能描述: 报文通过ctf转发
输入参数:  
返回值
*****************************************************************************/
int mbb_ctf_forward(struct sk_buff *skb,  struct net_device *dev)
{
    if (NULL == skb || NULL == dev)
    {
        printk("mbb_ctf_forward skb or dev is null\n");
        return NET_RET_FAIL;
    }   
        
    if (0 == g_ifctf)
    {
        /*对skb->dev, protocol等赋值*/
        skb->protocol = eth_type_trans(skb,  dev);

        /*ether header*/
        skb_push(skb, ETH_HLEN);

        if (eth_cih && (BCME_ERROR != eth_ctf_forward(skb)))
        {
            return NET_RET_OK;
        }
    }

    return NET_RET_FAIL;
}

/*****************************************************************************
函数名：  mbb_ctf_init
功能描述: ctf设备注册
输入参数:  
返回值
*****************************************************************************/
void mbb_ctf_init(struct net_device *net)
{
    unsigned int msglevel = 1;

    if (NULL != eth_cih || NULL == net)
    {
        return;
    }

    LAN_DEBUG("Init CTF for eth.\n");
    eth_fake_osh = (osl_t *)0xdeadbeaf; /* watch here, it might die */
    eth_cih = ctf_attach(eth_fake_osh, net->name, &msglevel, eth_ctf_detach, NULL /* et*/ );
    if (eth_cih)
    {
        LAN_DEBUG("Successful attach eth_cih = 0x%08x \n", (unsigned int)eth_cih);
    }

    if ((ctf_dev_register(eth_cih, net, FALSE) != BCME_OK) ||
        (ctf_enable(eth_cih, net, TRUE,NULL) != BCME_OK)) 
    {
        printk("ctf_dev_register() failed for eth.\n");
    }
    else
    {
        printk("Register CTF for eth successful.\n");
    }
}


/*****************************************************************************
函数名：  mbb_ctf_exit
功能描述: ctf设备退出
输入参数:  
返回值
*****************************************************************************/
void mbb_ctf_exit(struct net_device *net)
{
    if (NULL == eth_cih || NULL == net)
    {
        return;
    }
    
    ctf_dev_unregister(eth_cih, net);
    eth_cih = NULL;
}

EXPORT_SYMBOL(mbb_ctf_forward);
EXPORT_SYMBOL(mbb_ctf_init);
EXPORT_SYMBOL(mbb_ctf_exit);
#endif

#if ((FEATURE_ON == MBB_FEATURE_FASTIP) && (FEATURE_ON == MBB_ETH_STMMAC_TX_THREAD))
static void fastip_detach_eth(fastip_t *ci, void *arg)
{
    eth_fastip_handle = NULL;
    return;
}

int fastip_forward_eth(struct sk_buff *skb)
{
    /* use slow path if fastip is disabled */
    if (!FASTIP_ENAB(eth_fastip_handle))
    {
        return (FASTIP_ERROR);
    }

    /* try fastip first */
    if (fastip_forward(eth_fastip_handle, skb, skb->dev) != FASTIP_ERROR)
    {
        return (FASTIP_OK);
    }
    /* clear skipct flag before sending up */
    FASTIPCLRSKIPCT(skb);

    return (FASTIP_ERROR);
}
int mbb_fastip_forward(struct sk_buff *skb,  struct net_device *dev)
{
    if (NULL == skb || NULL == dev)
    {
        LAN_DEBUG("mbb_ctf_forward skb or dev is null\n");
        return NET_RET_FAIL;
    }   

    /*对skb->dev, protocol等赋值*/
    skb->protocol = eth_type_trans(skb,  dev);

    /*ether header*/
    skb_push(skb, ETH_HLEN);

    if (eth_fastip_handle && fastip_forward_eth(skb) != FASTIP_ERROR)
    {
        /* 统计网卡接收数据信息 */
        return NET_RET_OK;
    }

    return NET_RET_FAIL;
}

void mbb_fastip_init(struct net_device *net)
{
    unsigned int msglevel = 1;

    if ( NULL == net)
    {
        return;
    }
    if (!eth_fastip_handle) 
    {
        LAN_DEBUG( "eth fastip init\n");
        LAN_DEBUG( "eth fastip devname %s\n", net->name);
        eth_fastip_handle = fastip_attach(net->name, &msglevel, fastip_detach_eth, NULL);
        if (eth_fastip_handle)
        {
            LAN_DEBUG( "\n eth_fastip_handle attach ok !!!!!! cih = 0x%x \n",
                                            eth_fastip_handle );
        }
        if (fastip_dev_register(eth_fastip_handle, net, FALSE, TRUE) != FASTIP_OK) 
        {
            LAN_DEBUG( "eth fastip_dev_register() failed\n");
        }
        else
        {
            LAN_DEBUG( "eth fastip register ok\n");
        }
    }

}
void mbb_fastip_exit(struct net_device *net)
{
    if (NULL == eth_fastip_handle || NULL == net)
    {
        return;
    }
    fastip_dev_unregister(eth_fastip_handle, net);
    eth_fastip_handle = NULL;
    LAN_DEBUG( "eth fastip exit\n");
}
EXPORT_SYMBOL(mbb_fastip_forward);
EXPORT_SYMBOL(mbb_fastip_init);
EXPORT_SYMBOL(mbb_fastip_exit);
#endif /* MBB_FEATURE_FASTIP */

/*************************************************
函数名:       int et_reset_linkstate(int mode)
功能描述:    重置网口link状态，使其重新获取IP  
输入参数:        int mode, 0 重启port1-port4 1 重启port1-port3
返回值:       0 处理成功， 非0  处理异常     
*************************************************/
int et_reset_linkstate(int mode)
{
    struct phy_device *phydev = mbb_get_phy_device();
    int regvalue;

    /*组播升级状态不做reset*/
    if (g_muticast_flag)
    {
        return NET_RET_FAIL;
    }
    
#if (FEATURE_ON == MBB_FEATURE_ETH_PHY)   
    if (NULL != phydev)
    { 
        regvalue = phy_read(phydev, MII_BMCR);

        regvalue |= BMCR_RESET;
        phy_write(phydev, MII_BMCR, regvalue);

        return NET_RET_OK;
    }
#endif

#if (FEATURE_ON == MBB_FEATURE_ETH_SWITCH)   
    rtk_reset_allport(mode);
    return NET_RET_OK;
#endif

    return NET_RET_FAIL;

}

/*******************************************************************************
 函数名称  : mbb_get_eth_macAddr
 功能描述  : 设置出厂MAC, dev->dev_addr
 输入参数  :eth_macAddr
 输出参数  : 无
 返 回 值     :  0 : 成功,  -1 : 失败
*******************************************************************************/
int mbb_get_eth_macAddr(char *eth_macAddr)
{
    char baseMacaddr[NV_MAC_LENGTH] = {0};
    
    if (NULL == eth_macAddr)
    {
        return -1;
    }
   
    if (0 != NVM_Read(en_NV_Item_WIFI_MAC_ADDR,  baseMacaddr, sizeof(baseMacaddr)))
    {
        LAN_DEBUG("NVM Read MAC addr fail\n");
        return -1;
    }
    else
    {      
        if (0 != translate_mac_address(baseMacaddr, eth_macAddr))
        {
            LAN_DEBUG("factory macAddr format err\n");
            return -1;
        }  

       LAN_DEBUG("Get ETH MacAddr:%X:%X:%X:%X:%X:%X\n", 
            eth_macAddr[0], eth_macAddr[1], eth_macAddr[2], eth_macAddr[3], eth_macAddr[4],  eth_macAddr[5]);

        return 0;
    }  
}

EXPORT_SYMBOL(PhyATQryPortPara);
EXPORT_SYMBOL(mbb_check_net_upgrade);
EXPORT_SYMBOL(mbb_mac_clone_rx_restore);
EXPORT_SYMBOL(mbb_mac_clone_tx_save);
EXPORT_SYMBOL(RNIC_WANMirror);
EXPORT_SYMBOL(RNIC_Map_To_Lan_Forward);
EXPORT_SYMBOL(mbb_eth_state_report);
EXPORT_SYMBOL(et_reset_linkstate);

