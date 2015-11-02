/* Kernel module to match the port-ranges, trigger related port-ranges,
 * and alters the destination to a local IP address.
 *
 * Copyright (C) 2003, CyberTAN Corporation
 * All Rights Reserved.
 *
 * Description:
 *   This is kernel module for port-triggering.
 *
 *   The module follows the Netfilter framework, called extended packet 
 *   matching modules. 
 */
/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.
when       who     what, where, why
--------   ---     ----------------------------------------------------------
===========================================================================*/ 
/*
* Authors:        wangjianping 
                  zhangguangwei
*/
/*
*  Fixes::       wangjianping   ip filter bug
                 zhangguangwei  miss alignment bug
*/
#include <linux/types.h>
#include <linux/ip.h>
#include <linux/timer.h>
#include <linux/module.h>
#include <linux/netfilter.h>
#include <linux/netdevice.h>
#include <linux/if.h>
#include <linux/inetdevice.h>
#include <net/protocol.h>
#include <net/checksum.h>
#include <linux/list.h>
#include<linux/tcp.h>

#include <linux/netfilter/x_tables.h>
#include <linux/netfilter_ipv4.h>
#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_conntrack_core.h>
#include <net/netfilter/nf_conntrack_helper.h>
#include <net/netfilter/nf_conntrack_tuple.h>

#ifdef CONFIG_NF_NAT_NEEDED
#include <net/netfilter/nf_nat_rule.h>
#else
#include <linux/netfilter_ipv4/ip_nat_rule.h>
#endif

#include <linux/netfilter_ipv4/ipt_TRIGGER.h>

/* This rwlock protects the main hash table, protocol/helper/expected
 *    registrations, conntrack timers*/

DEFINE_RWLOCK(trigger_conntrack_lock);


/* Return pointer to first true entry, if any, or NULL.  A macro
   required to allow inlining of cmpfn. */
#define LIST_FIND(head, cmpfn, type, args ...)		\
({							\
	const struct list_head *__i, *__j = NULL;	\
							\
	list_for_each(__i, (head))			\
		if (cmpfn((const type)__i , ## args)) {	\
			__j = __i;			\
			break;				\
		}					\
	(type)__j;					\
})



#if 0
#define DEBUGP printk
#else
#define DEBUGP(format, args...)
#endif

struct ipt_trigger {
    struct list_head list;		/* Trigger list */
    struct timer_list timeout;	/* Timer for list destroying */	
    u_int32_t srcip;		/* Outgoing source address */
    u_int32_t dstip;		/* Outgoing destination address */
    u_int16_t mproto;		/* Trigger protocol */
    u_int16_t rproto;		/* Related protocol */
    struct ipt_trigger_ports ports;	/* Trigger and related ports */
    u_int8_t reply;			/* Confirm a reply connection */
};

LIST_HEAD(trigger_list);

static void trigger_refresh(struct ipt_trigger *trig, unsigned long extra_jiffies)
{
    DEBUGP("%s: \n", __FUNCTION__);
    
    NF_CT_ASSERT(trig);
    
    write_lock_bh(&trigger_conntrack_lock);

    /* Need del_timer for race avoidance (may already be dying). */
    if (del_timer(&trig->timeout)) {
        trig->timeout.expires = jiffies + extra_jiffies;
        add_timer(&trig->timeout);
    }

	write_unlock_bh(&trigger_conntrack_lock);
}

static void __del_trigger(struct ipt_trigger *trig)
{
    DEBUGP("%s: \n", __FUNCTION__);
    
    NF_CT_ASSERT(trig);
    /* delete from 'trigger_list' */
    list_del(&trig->list);
    kfree(trig);
}

static void trigger_timeout(unsigned long ul_trig)
{
    struct ipt_trigger *trig= (void *) ul_trig;

    DEBUGP("trigger list %p timed out\n", trig);
    
    write_lock_bh(&trigger_conntrack_lock);
    __del_trigger(trig);
	write_unlock_bh(&trigger_conntrack_lock);
}

    static unsigned int
add_new_trigger(struct ipt_trigger *trig)
{
    struct ipt_trigger *new;

    DEBUGP("!!!!!!!!!!!! %s !!!!!!!!!!!\n", __FUNCTION__);
    
    write_lock_bh(&trigger_conntrack_lock);
    new = (struct ipt_trigger *)
        kmalloc(sizeof(struct ipt_trigger), GFP_ATOMIC);

    if (!new) {
        write_unlock_bh(&trigger_conntrack_lock);
        DEBUGP("%s: OOM allocating trigger list\n", __FUNCTION__);
        return -ENOMEM;
    }

    memset(new, 0, sizeof(*trig));
    INIT_LIST_HEAD(&new->list);
    memcpy(new, trig, sizeof(*trig));

    /* add to global table of trigger */
    list_add(&new->list, &trigger_list);
    /* add and start timer if required */
    init_timer(&new->timeout);
    new->timeout.data = (unsigned long)new;
    new->timeout.function = trigger_timeout;
    new->timeout.expires = jiffies + (TRIGGER_TIMEOUT * HZ);
    add_timer(&new->timeout);

	write_unlock_bh(&trigger_conntrack_lock);

    return 0;
}

static inline int trigger_out_matched(const struct ipt_trigger *i,
    const u_int16_t proto, const u_int16_t dport, const u_int16_t proto_in)
{
    DEBUGP("%s: i=%p, proto= %d, dport=%d.\n", __FUNCTION__, i, proto, dport);
    DEBUGP("%s: Got one, mproto= %d, mport[0..1]=%d, %d.\n", __FUNCTION__, 
        i->mproto, i->ports.mport[0], i->ports.mport[1]);

	u_int16_t mproto = i->mproto;
	u_int16_t rproto = i->rproto;

	if (!mproto)
		mproto = proto;

    #if 0
    printk("trigger out: protocol: (%d) %s\r\n", proto, proto == IPPROTO_TCP ? "TCP":"UDP");
    printk("i->mproto :            (%d) %s\r\n", i->mproto, i->mproto == IPPROTO_TCP ? "TCP":"UDP");
    #endif

    return ((mproto == proto) && (i->ports.mport[0] <= dport) 
        && (i->ports.mport[1] >= dport) && (rproto == proto_in));
}

static unsigned int
trigger_out(struct sk_buff *skb,
    const struct net_device *in,
    const struct net_device *out,
        unsigned int hooknum,
    const void *targinfo )
{
    const struct ipt_trigger_info *info = targinfo;
    struct ipt_trigger trig, *found;
    const struct iphdr *iph = ip_hdr(skb);
    struct tcphdr *tcph = (void *)iph + iph->ihl*4; 	/* Might be TCP, UDP */
   /* <BEGIN BB5D01723 caoxuewen 2009/7/02 */
	int index = 0;
   /* BB5D01723 caoxuewen 2009/7/02 END> */

    u_int16_t mproto = 0;

    DEBUGP("############# %s ############\n", __FUNCTION__);

    #if 0
    printk("\r\n************TRIGGER_OUT***************\r\n");
    printk(" info->mproto: (%d) %s\r\n", info->proto, (info->proto == IPPROTO_TCP ? "TCP":"UDP"));
    printk(" info->rproto: (%d)%s\r\n", info->rproto, (info->rproto == IPPROTO_TCP ? "TCP":"UDP"));
    printk("info->mport[0...1]: %d - %d\r\n", info->ports.mport[0], info->ports.mport[1]);
    /*printk("info->rport[0...1]: %d - %d\r\n", info->ports.rport[0], info->ports.rport[1]);*/
    printk("real port:          %d\r\n",  ntohs(tcph->dest));
    printk("real protocol:      %d (%s)\r\n", iph->protocol, iph->protocol == IPPROTO_TCP ? "TCP":"UDP");
    printk("___________________________________________\r\n");
    #endif
    
    /* Check if the trigger range has already existed in 'trigger_list'. */
    /* 首先，在全局链表中查找是否存在该skb满足的规则 */
    found = LIST_FIND(&trigger_list, trigger_out_matched,
        struct ipt_trigger *, iph->protocol, ntohs(tcph->dest), info->rproto);

    /* 如果发现存在该skb满足的规则 */
    if (found) {

        #if 0
        printk("find the list ................\r\n");
        #endif
        
        /* Yeah, it exists. We need to update(delay) the destroying timer. 更新计数器 */
        trigger_refresh(found, TRIGGER_TIMEOUT * HZ);
        /* In order to allow multiple hosts use the same port range, we update
           the 'saddr' after previous trigger has a reply connection. */
        /* 如果发现reply已经被置1了，那么表示已经有其它主机给使用该规则了，于是我们就将
          * 源IP改成本skb中的源IP，否则外面的包将还是会送到found中存放的以前的IP
          */
        if (found->reply)
            found->srcip = iph->saddr;
            /*found->srcip = HW_GET_UNALIGNED((void*)&(iph->saddr));*/
        DEBUGP("find found src ip :%u.%u.%u.%u\r\n",NIPQUAD(found->srcip));
    }
    else {

		mproto = info->proto;
		
    	if (!mproto)
		mproto = iph->protocol;

        if ((iph->protocol == mproto) 
            && (info->ports.mport[0] <= ntohs(tcph->dest))
            && (info->ports.mport[1] >= ntohs(tcph->dest))) {
            
            memset(&trig, 0, sizeof(trig));
            trig.srcip = iph->saddr;
            /*trig.srcip = HW_GET_UNALIGNED((void*)&(iph->saddr));*/
            DEBUGP("add new src ip :%u.%u.%u.%u\r\n",NIPQUAD(iph->saddr));
            /*DEBUGP("add new src ip :%u.%u.%u.%u\r\n",NIPQUAD(HW_GET_UNALIGNED((void*)&(iph->saddr))));*/
			trig.mproto = info->proto;
			trig.rproto = info->rproto;
			#if 0
            if (info->proto) {
                trig.mproto = info->proto;
            } else {
                trig.mproto = iph->protocol;
            }
            #if 1 //def ATP_SUPPORT_PRTT_RPROTOCOL
            if (info->rproto) 
            {
                trig.rproto = info->rproto;
            }
            else {
                trig.rproto = info->proto;
            }
            #else
            trig.rproto = info->proto;
            #endif
            #endif

            
            memcpy(&trig.ports, &info->ports, sizeof(struct ipt_trigger_ports));
            add_new_trigger(&trig); /* Add the new 'trig' to list 'trigger_list'. */
        }
    }
    #if 0
    else {
        printk("not find the list, build one ......\r\n");
        /* Create new trigger */
        memset(&trig, 0, sizeof(trig));
        trig.srcip = iph->saddr;

        /* 以前的方法是表示进来的协议应该是配置的协议类型，出去的不管，也就是如果配置了规则，
          * 比如 trigger-proto tcp 那么其实表示的是你进来的应该是TCP协议，而不是说出去的是TCP
          * 协议，所以，不管是出去的是UDP还是TCP协议，只要是从规则中对应端口出去的，那么就可以
          * 导致该协议的触发，从而打开对应端口。
          */
        #if 0
        trig.mproto = iph->protocol;
        trig.rproto = info->proto;
        #else
        if (info->proto) {
            printk("info->proto:  %d(%s)\r\n", info->proto, info->proto == IPPROTO_TCP ? "TCP":"UDP");
            trig.mproto = info->proto;
        }
        else {
            printk("line: %d\r\n", __LINE__);
            trig.mproto = iph->protocol;
        }
        #if 1 //def ATP_SUPPORT_PRTT_RPROTOCOL
        if (info->rproto) {
            printk("line: %d\r\n", __LINE__);
            trig.rproto = info->rproto;
        }
        else {
            printk("line: %d\r\n", __LINE__);
            trig.rproto = info->proto;
        }
        #else
        printk("line: %d\r\n", __LINE__);
        trig.rproto = info->proto;
        #endif
        
        #endif

        printk("\r\n#################################r\n");
        printk("build result: \r\n");
        printk(" trig->mproto: (%d) %s\r\n", trig.mproto, (trig.mproto == IPPROTO_TCP ? "TCP":"UDP"));
        printk(" trig->rproto: (%d)%s\r\n", trig.rproto, (trig.rproto == IPPROTO_TCP ? "TCP":"UDP"));
        printk("info->mport[0...1]: %d - %d\r\n", trig.ports.mport[0], trig.ports.mport[1]);
        printk("info->rport[0...1]: %d - %d\r\n", trig.ports.rport[0], trig.ports.rport[1]);
        printk("#####################################\r\n");

        
        memcpy(&trig.ports, &info->ports, sizeof(struct ipt_trigger_ports));
        add_new_trigger(&trig);	/* Add the new 'trig' to list 'trigger_list'. */
    }
    #endif

    return XT_CONTINUE;	/* We don't block any packet. */
}

static inline int trigger_in_matched(const struct ipt_trigger *i,
    const u_int16_t proto, const u_int16_t dport)
{

    u_int16_t rproto = i->rproto;

   /* <BEGIN BB5D01723 caoxuewen 2009/7/02 */
    int index = 0;
   /* BB5D01723 caoxuewen 2009/7/02 END> */

    if (!rproto)
        rproto = proto;

    #if 0
    printk("\r\n++++++++++++trigger_in_matched+++++++++++++++++\r\n");
    printk("trigger in match:  \r\n");
    printk(" i->mproto:     (%d) %s\r\n", i->mproto, (i->mproto == IPPROTO_TCP ? "TCP":"UDP"));
    printk(" i->rproto:     (%d) %s\r\n", i->rproto, (i->rproto == IPPROTO_TCP ? "TCP":"UDP"));
    printk(" real protocol: (%d) %s\r\n", proto, proto == IPPROTO_TCP ? "tcp":"udp");
    printk(" real port:      %d\r\n", dport);
    printk("i->mport[0...1]: %d - %d\r\n", i->ports.mport[0], i->ports.mport[1]);
    /*printk("i->rport[0...1]: %d - %d\r\n", i->ports.rport[0], i->ports.rport[1]);*/
    printk("\r\n===============================================\r\n");
    #endif
    
   /* <BEGIN BB5D01723 caoxuewen 2009/7/02 */
#if 0
    return ((rproto == proto) && (i->ports.rport[0] <= dport) 
        && (i->ports.rport[1] >= dport));
#endif
    for (index = 0; index < PT_OPEN_PORT_RANGE_MAX; index++)
	{
		if (i->ports.stOpenPort[index].ulStartPort == 0)
		{
			continue;
		}

		if ((rproto == proto) && (i->ports.stOpenPort[index].ulStartPort <= dport) 
            && (i->ports.stOpenPort[index].ulEndPort >= dport))
		{
			return 1;
		}
	}
    
    return 0;
   /* BB5D01723 caoxuewen 2009/7/02 END> */
}

static unsigned int
trigger_in(struct sk_buff *skb,
    const struct net_device *in,
    const struct net_device *out,
    unsigned int hooknum,
    const void *targinfo )
{
    struct ipt_trigger *found = NULL;
    const struct iphdr *iph = ip_hdr(skb);
    struct tcphdr *tcph = (void *)iph + iph->ihl*4;	/* Might be TCP, UDP */

    /* Check if the trigger-ed range has already existed in 'trigger_list'. 
      * 首先去找一把，看进来的报文是否满足要求
      */
    found = LIST_FIND(&trigger_list, trigger_in_matched,
        struct ipt_trigger *, iph->protocol, ntohs(tcph->dest));
    if (found) {
        DEBUGP("############# %s ############\n", __FUNCTION__);
        /* Yeah, it exists. We need to update(delay) the destroying timer. */
        trigger_refresh(found, TRIGGER_TIMEOUT * HZ);
        return NF_ACCEPT;	/* Accept it, or the imcoming packet could be 
                               dropped in the FORWARD chain */
    }

    return XT_CONTINUE;	/* Our job is the interception. */
}

/* 主要是因为进来的包是首先发起连接，所以需要对其进行DNAT，否则就不知道进来的包具体发给哪一个了 */
    static unsigned int
trigger_dnat(struct sk_buff *skb,
    const struct net_device *in,
    const struct net_device *out,
    unsigned int hooknum,
    const void *targinfo )
{
    struct ipt_trigger *found;
    const struct iphdr *iph = ip_hdr(skb);
    struct tcphdr *tcph = (void *)iph + iph->ihl*4;	    /* Might be TCP, UDP */
    struct nf_conn *ct;
    enum ip_conntrack_info ctinfo;
	struct nf_nat_ipv4_range newrange;
	const struct nf_nat_ipv4_multi_range_compat *mr;

    
    NF_CT_ASSERT(hooknum == NF_IP_PRE_ROUTING);
    
	mr = targinfo;

	DEBUGP("src ip :%u.%u.%u.%u,dst ip :%u.%u.%u.%u\r\n",NIPQUAD(mr->range[0].min_ip),NIPQUAD(mr->range[0].max_ip));

	DEBUGP("############# %s ############  11111 \n", __FUNCTION__);

    /* Check if the trigger-ed range has already existed in 'trigger_list'. */
    found = LIST_FIND(&trigger_list, trigger_in_matched,
        struct ipt_trigger *, iph->protocol, ntohs(tcph->dest));

    /* 如果没有找到对应的规则，那么就继续给下一个iptables规则处理该skb */
    if (!found || !found->srcip)
        return XT_CONTINUE;	/* We don't block any packet. */

    DEBUGP("############# %s ############  2222 \n", __FUNCTION__);
    found->reply = 1;	/* Confirm there has been a reply connection. */
    
    ct = nf_ct_get(skb, &ctinfo);

    /* 显然应该是外面首先连接进来 */
    NF_CT_ASSERT(ct && (ctinfo == IP_CT_NEW));

    DEBUGP("%s: got \r\n", __FUNCTION__);
  //  DUMP_TUPLE(&ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple);

  DEBUGP("src ip :%u.%u.%u.%u,hooknum:%u\r\n",NIPQUAD(found->srcip),hooknum);

	/* Transfer from original range. */
	newrange = ((struct nf_nat_ipv4_range)
		{ mr->range[0].flags | NF_NAT_RANGE_MAP_IPS,
		  found->srcip, found->srcip,
		  mr->range[0].min, mr->range[0].max });
	DEBUGP("src ip :%u.%u.%u.%u,dst ip :%u.%u.%u.%u adfsadfasdfasfasfas\r\n",NIPQUAD(newrange.min_ip),NIPQUAD(newrange.max_ip));

    /* Hand modified range to generic setup. */
    return nf_nat_setup_info(ct, &newrange, NF_NAT_MANIP_DST);
}

static unsigned int
trigger_target(struct sk_buff *skb,struct xt_action_param *tgpar )
{
    const struct net_device *in = tgpar->in;
    const struct net_device *out = tgpar->out;
    unsigned int hooknum = tgpar->hooknum;
    const void *targinfo = tgpar->targinfo;

    const struct ipt_trigger_info *info = targinfo;
    const struct iphdr *iph = ip_hdr(skb);

    DEBUGP("%s: type = %s\n", __FUNCTION__, 
        (info->type == IPT_TRIGGER_DNAT) ? "dnat" :
        (info->type == IPT_TRIGGER_IN) ? "in" : "out"); 

    /* The Port-trigger only supports TCP and UDP. */
    if ((iph->protocol != IPPROTO_TCP) && (iph->protocol != IPPROTO_UDP))
        return XT_CONTINUE;

    if (info->type == IPT_TRIGGER_OUT)
        return trigger_out(skb, in, out, hooknum, targinfo);
    else if (info->type == IPT_TRIGGER_IN)
        return trigger_in(skb, in, out, hooknum, targinfo);
    else if (info->type == IPT_TRIGGER_DNAT)
        return trigger_dnat(skb, in, out, hooknum, targinfo);

    return XT_CONTINUE;
}

    static int
trigger_check(const struct xt_tgchk_param *par)
#if 0
const char *tablename,
    const void *e,
    const struct xt_target *target,
    void *targinfo,
    unsigned int hook_mask)
#endif
{
    const char *tablename = par->table;
    const void *e = par->entryinfo;
    const struct xt_target *target = par->target;
    void *targinfo = par->targinfo;
    unsigned int hook_mask = par->hook_mask;
    
    const struct ipt_trigger_info *info = targinfo;
    struct list_head *cur_item, *tmp_item;

    if ((strcmp(tablename, "mangle") == 0)) {
        DEBUGP("trigger_check: bad table `%s'.\n", tablename);
        return -EINVAL;
    }

#if 0
    if (targinfosize != IPT_ALIGN(sizeof(*info))) {
        DEBUGP("trigger_check: size %u.\n", targinfosize);
        return 0;
    }
#endif
    if (hook_mask & ~((1 << NF_INET_PRE_ROUTING) | (1 << NF_INET_FORWARD))) {
        DEBUGP("trigger_check: bad hooks %x.\n", hook_mask);
        return -EINVAL;
    }
    if (info->proto) {
        if (info->proto != IPPROTO_TCP && info->proto != IPPROTO_UDP) {
            DEBUGP("trigger_check: bad proto %d.\n", info->proto);
            return -EINVAL;
        }
    }

    /* 增加了针对related协议分类的判断 */
    #if 1 //def ATP_SUPPORT_PRTT_RPROTOCOL
    if (info->rproto) {
        if (info->rproto != IPPROTO_TCP && info->rproto != IPPROTO_UDP) {
            DEBUGP("trigger_check: bad proto %d.\n", info->rproto);
            return -EINVAL;
        }
    }
    #endif

    
    if (info->type == IPT_TRIGGER_OUT) {
   /* <BEGIN BB5D01723 caoxuewen 2009/7/02 */
        if (!info->ports.mport[0]) {
            DEBUGP("trigger_check: Try 'iptbles -j PORT_TRIGGERING -h' for help.\n");
            return -EINVAL;
        }
    }

	 if (info->type == IPT_TRIGGER_OUT)
 	 {
		DEBUGP("i->--open-port %d-%d,%d-%d,%d-%d,%d-%d,%d-%d", 
							info->ports.stOpenPort[0].ulStartPort,info->ports.stOpenPort[0].ulEndPort,
							info->ports.stOpenPort[1].ulStartPort,info->ports.stOpenPort[1].ulEndPort,
							info->ports.stOpenPort[2].ulStartPort,info->ports.stOpenPort[2].ulEndPort,
							info->ports.stOpenPort[3].ulStartPort,info->ports.stOpenPort[3].ulEndPort,
							info->ports.stOpenPort[4].ulStartPort,info->ports.stOpenPort[4].ulEndPort);
 	 }
   /* BB5D01723 caoxuewen 2009/7/02 END> */

    /* Empty the 'trigger_list' */
    list_for_each_safe(cur_item, tmp_item, &trigger_list) {
        struct ipt_trigger *trig = (void *)cur_item;

        DEBUGP("%s: list_for_each_safe(): %p.\n", __FUNCTION__, trig);
        del_timer(&trig->timeout);
        __del_trigger(trig);
    }

    return 0;
}

static struct xt_target porttriggering_tg_reg __read_mostly = { 
    .name = "PORT_TRIGGERING",
	.family		= NFPROTO_IPV4,
    .target = trigger_target, 
    .targetsize = sizeof(struct ipt_trigger_info),
	/*.table		= "filter",
	.hooks		= (1 << NF_INET_LOCAL_IN) | (1 << NF_INET_FORWARD) |
			  (1 << NF_INET_LOCAL_OUT),*/
    .checkentry = trigger_check,
    .me = THIS_MODULE,
};

static int __init porttriggering_tg_init(void)
{
    return xt_register_target(&porttriggering_tg_reg);
}

static void __exit porttriggering_tg_exit(void)
{
    xt_unregister_target(&porttriggering_tg_reg);
}

module_init(porttriggering_tg_init);
module_exit(porttriggering_tg_exit);
/*Hisilicon Linux add end*/
