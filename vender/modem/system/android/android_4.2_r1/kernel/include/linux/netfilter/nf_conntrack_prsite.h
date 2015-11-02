/*
 * 2015-05-19 - changed http prsite. (C) Huawei Technologies. The contribution by Huawei Technologies is under GPLv2
*/
#ifndef _NF_NAT_PRSITE_H_
#define _NF_NAT_PRSITE_H_

#include <linux/timer.h>
#include <linux/spinlock.h>
#include <linux/list.h>
#include <asm/atomic.h>
#include <linux/netfilter.h>
#include <linux/netfilter/nf_conntrack_tuple_common.h>
#include <net/netfilter/nf_conntrack.h>

#define HTTP_PORT 80

/* start of 防止用户态配置下来的字符串长度加上 http:// */ 
#define HTTP_URL_MAX (256 + 32)
/* end of 防止用户态配置下来的字符串长度加上 http://  */
#define HTTP_TRACE_CHECK_TIMEOUT 5 //minutes
#define HTTP_TRACE_TIMEOUT 30 //minutes
#define HTTP_PROTO_HEAD_BUF_MAX 16
#define HTTP_RESPONSE_BUF_MAX (HTTP_URL_MAX + 256)
#define AFFINED_ADDR_BUF_MAX 16

#define AFFINED_ADDR_STATIC_BASE 8

#define HTTP_PROTO_NAME "HTTP"
#define HTTP_PROTO_GET  "GET"



struct prsite_url_info
{
    int          lEnable;
    unsigned int ul_lan_addr;
    unsigned int ul_lan_mask;
    char         ac_stb_url[HTTP_URL_MAX];
    char         ac_computer_url[HTTP_URL_MAX];
    char         ac_wifi_url[HTTP_URL_MAX];
};

//for ioctl
struct affined_bind
{
    unsigned int addr;
    unsigned int mask;
    unsigned int flag;
    char         url[HTTP_URL_MAX];
};
struct user_agent_list
{
    char *key_type;
    char *key_version;
    int version_num;
};

extern unsigned int nf_prsite_in(struct sk_buff *skb, const struct net_device *pin, const struct net_device *pout);

#endif
