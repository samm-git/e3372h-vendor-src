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
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/interrupt.h>
#include <linux/init.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
#include <linux/wakelock.h>
#include <linux/icmp.h>
#include <linux/udp.h>
#include <linux/ip.h>
#include <linux/in.h>


#if (FEATURE_ON == MBB_FEATURE_FASTIP)
#include <linux/if.h>
#include <linux/if_vlan.h>
#include <linux/fastip/fastip.h>
#include <linux/ip.h>
#endif /* MBB_FEATURE_FASTIP */

static struct net_device *hw_rmnet_dev;

#if (FEATURE_ON == MBB_FEATURE_FASTIP)
fastip_t  __attribute__ ((aligned(32))) *rmnet2_fastip_handle  = NULL;
#endif /* MBB_FEATURE_FASTIP */

#if (FEATURE_ON == MBB_FEATURE_FASTIP)
static void fastip_detach_rmnet2(fastip_t *ci, void *arg)
{
    rmnet2_fastip_handle = NULL;
    return;
}

#endif /* MBB_FEATURE_FASTIP */

static int hw_rmnet_open(struct net_device *dev)
{
	netif_start_queue(dev);
	return 0;
}

static int hw_rmnet_stop(struct net_device *dev)
{
	netif_stop_queue(dev);
	return 0;
}


static int hw_rmnet_xmit(struct sk_buff *skb, struct net_device *dev)
{
	/* do nothing just  to release skb */
	dev_kfree_skb_irq(skb);
	return 0;
}


static const struct net_device_ops hw_rmnet_ops = {
	.ndo_open = hw_rmnet_open,
	.ndo_stop = hw_rmnet_stop,
	.ndo_start_xmit = hw_rmnet_xmit,/*lint !e64*/
	.ndo_set_mac_address = eth_mac_addr,
	.ndo_validate_addr	= eth_validate_addr,	
	.ndo_change_mtu = eth_change_mtu,
};

static void __init hw_rmnet_setup(struct net_device *dev)
{
	ether_setup(dev);
    dev->netdev_ops = &hw_rmnet_ops;
	random_ether_addr(dev->dev_addr);  
}


static int __init hw_rmnet_init(void)
{
	int ret;
	struct net_device *dev;
#if (FEATURE_ON == MBB_FEATURE_FASTIP)
    int msglevel = 1;
#endif
	dev = alloc_netdev(0, "rmnet2", hw_rmnet_setup);

	if (!dev)
       {
   	    return -ENOMEM;
       }
       dev_net_set(dev, &init_net);
	ret = register_netdev(dev);
	if (ret) 
       {
		free_netdev(dev);
		return ret;
	}
	hw_rmnet_dev=dev;
#if (FEATURE_ON == MBB_FEATURE_FASTIP)
    if (!rmnet2_fastip_handle) 
    {
        printk("rmnet2 fastip init\n");
        printk("rmnet2 fastip devname %s\n",dev->name);
        rmnet2_fastip_handle = fastip_attach(dev->name, &msglevel, fastip_detach_rmnet2, NULL);
        if (rmnet2_fastip_handle)
        {
            printk("\n rmnet2_fastip_handle attach ok !!!!!! cih = 0x%x \n", rmnet2_fastip_handle );
        }
        
        if (fastip_dev_register(rmnet2_fastip_handle, dev, FALSE, TRUE) != FASTIP_OK) 
        {
            printk("rmnet2 fastip_dev_register() failed\n");
        }
        else
        {
            printk("rmnet2 fastip register ok\n");
        }
    }
#endif /* MBB_FEATURE_FASTIP */
	return 0;
}
static void __exit hw_rmnet_cleanup(void)
{
#if (FEATURE_ON == MBB_FEATURE_FASTIP)
    fastip_dev_unregister(rmnet2_fastip_handle, hw_rmnet_dev);
    rmnet2_fastip_handle = NULL;
#endif /* MBB_FEATURE_FASTIP */
	unregister_netdev(hw_rmnet_dev);
	free_netdev(hw_rmnet_dev);
}

module_init(hw_rmnet_init);
module_exit(hw_rmnet_cleanup);
