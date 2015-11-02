/* $Id: iptcrdr.c,v 1.39 2011/03/09 15:27:42 nanard Exp $ */
/* MiniUPnP project
 * http://miniupnp.free.fr/ or http://miniupnp.tuxfamily.org/
 * (c) 2006-2011 Thomas Bernard
 * This software is subject to the conditions detailed
 * in the LICENCE file provided within the distribution
 */


 /**********************问题单修改记录******************************************
    日期              修改人         问题单号           修改内容
  
 2012.03.07        z00203875     2030707506    限制16条端口映射规则
 2012.03.14        z00203875     2031401440    修改打LOG方式
 2012.04.03        z00203875     2032802867    无法在不删除规则的
                                                     情况下禁用规则      
 2012.12.24        y44942        2121101411    消除Fortify告警
 2013.08.22        y00248130     3082010042    消除Fortify告警
 2014.06.10        y00248130     4062600669    PCP特性
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "config.h"
#include "ipnat_rdr.h"
#include "upnpglobalvars.h"
#include "getifaddr.h"
#include "firewallapi.h"

#ifdef MBB_FEATURE_PCP
#include "appinterface.h"
#include "pcp_lib.h"
#include "wanapi.h"
#define MAX_RULE_NUM 128  // TODO: 再看一下改成128有什么影响没
#else
#define MAX_RULE_NUM 16
#endif

#define DESC_MAX_LEN (64)

#ifdef MBB_FEATURE_PCP
extern void add_pcp_rule(unsigned short eport, int proto, unsigned int lifetime, int enabled,
                    const char *local_ip, unsigned short iport);
#endif

/* dummy init and shutdown functions */

int init_redirect(void)
{
    return 0;
}

void shutdown_redirect(void)
{
    return;
}

/* convert an ip address to string */
static int snprintip(char * dst, size_t size, unsigned int ip)
{
    return snprintf(dst, size,
           "%u.%u.%u.%u", ip >> 24, (ip >> 16) & 0xff,
           (ip >> 8) & 0xff, ip & 0xff);
}

/* netfilter cannot store redirection descriptions, so we use our
 * own structure to store them */
struct rdr_desc {
    struct rdr_desc * next;
    unsigned short eport;
    short proto;
    char eaddr[16];
    char iaddr[16];
    unsigned short iport;
    char ifname[16];
	/* BEGIN 2032802867 zhoujianchun 203875 2012.04.03 added */
    int enabled;
	/* END 2032802867 zhoujianchun 203875 2012.04.03 added */
    char str[DESC_MAX_LEN];
};

/* pointer to the chained list where descriptions are stored */
static struct rdr_desc * rdr_desc_list = 0;

/* begin 2030707506 zhoujianchun 00203875 2010-03-07 added for UPnP */

/* after delete the rule number 17, reload other rules */
void reload_port_mapping_rules(void)
{
    NP_UPNP_DEBUG("reload_port_mapping_rules\n");
    struct rdr_desc *p = rdr_desc_list;
    NAT_PORTMAP_INFO_ST portmap_info;

    DelUPnPPortMapRule(); /* delete all rules in nat table first */

    while(NULL != p)
    {
        memset(&portmap_info, 0, sizeof(portmap_info));
        portmap_info.portmap_index = 0;
        portmap_info.portmap_enabled = ENABLE_FEATURE;
        if(NULL != p->str)
        {
            strncpy((char *)portmap_info.portmap_name, p->str, NAME_LEN_MAX);
        }
        else
        {
            strncpy((char *)portmap_info.portmap_name, "none", NAME_LEN_MAX);
        }
        if(NULL != p->eaddr)
        {
            strncpy((char *)portmap_info.remote_host_addr, p->eaddr, IPV4_ADDRESS_LEN);
        }
        else
        {
            strncpy((char *)portmap_info.remote_host_addr, "0.0.0.0", IPV4_ADDRESS_LEN);
        }
        if(NULL != p->iaddr)
        {
            strncpy((char *)portmap_info.internal_client_addr, p->iaddr, IPV4_ADDRESS_LEN);
        }
        else
        {
            /* internal client address can not be null */
            p = p->next;
            continue;
        }
        strncpy((char *)portmap_info.protocol, ((IPPROTO_TCP == p->proto) ? "tcp" : "udp"), IP_PROTO_LEN);
        portmap_info.external_port = p->eport;
        portmap_info.external_port_end = p->eport;
        portmap_info.internal_port = p->iport;

        /* BEGIN 2032802867 zhoujianchun 203875 2012.04.03 added */
        if(1 == p->enabled)
        /* BEGIN 2032802867 zhoujianchun 203875 2012.04.03 added */
        {
            AddUPnPPortMapRule(&portmap_info);
        }

        p = p->next;
    }
    return;
}


/* check if there are more than MAX_RULE_NUM entries in rdr_desc_list, if so, delete the oldest one */
static void check_and_delete_last_entry(void)
{
    int i = 0;
    struct rdr_desc * p = NULL;
#ifdef MBB_FEATURE_PCP
    int wan_status;
    PARA_TO_PCP_PROC_ST pcp_para;
#endif

    for(p = rdr_desc_list; NULL != p; p = p->next)
    {
        i++;
        if( MAX_RULE_NUM == i && NULL != p->next)
        {
#ifdef MBB_FEATURE_PCP
            wan_status = MgntGetWanState();
            if (WAN_STATUS_CONNECTED == wan_status)
            {
                memset(&pcp_para, 0, sizeof(pcp_para));
                //eport是pcp的iport
                fill_pcp_process_para(&pcp_para, 0, NULL, p->next->eport, p->next->proto, 0, 0, NULL);
                send_msg_to_pcp_client(ATP_MSG_UPNP_MAX_RULE, &pcp_para);
            }
#endif
            free(p->next);
            p->next = NULL;
            reload_port_mapping_rules();
            break;
        }
    }
    return;
}
/* end 2030707506 zhoujianchun 00203875 2010-03-07 added for UPnP */
/* add a description to the list of redirection descriptions */
static void
add_redirect_desc(unsigned short eport, int proto, const char * eaddr, const char * iaddr, 
unsigned short iport, const char * ifname, const char * desc, int enabled)
{
    NP_UPNP_INFO("enter add_redirect_desc, enabled arg is %d\n", enabled);

    struct rdr_desc * p;
    size_t l;
    /* set a default description if none given */
    if(!desc)
        desc = "miniupnpd";
    l = strlen(desc) + 1;
    p = (struct rdr_desc *)malloc(sizeof(struct rdr_desc) + l);
    if(p)
    {
        p->next = rdr_desc_list;
        p->eport = eport;
        p->proto = (short)proto;
        p->enabled = enabled;
        memcpy(p->eaddr, eaddr, 16);
        memcpy(p->iaddr, iaddr, 16);
        p->iport = iport;
        memcpy(p->ifname, ifname, 16);
        memcpy(p->str, desc, (sizeof(p->str) < l)?sizeof(p->str):l); 
        p->str[sizeof(p->str) - 1] = '\0'; 
        rdr_desc_list = p;
    }
/* begin 2030707506 zhoujianchun 00203875 2010-03-07 added for UPnP */
    check_and_delete_last_entry();
/* end 2030707506 zhoujianchun 00203875 2010-03-07 added for UPnP */
#ifdef DEBUG
    list_redirect_rule(ext_if_name);
#endif
}

/* delete a description from the list */
static void
del_redirect_desc(unsigned short eport, int proto)
{
    struct rdr_desc * p, * last;
    p = rdr_desc_list;
    last = 0;
    while(p)
    {
        if(p->eport == eport && p->proto == proto)
        {
            if(!last)
                rdr_desc_list = p->next;
            else
                last->next = p->next;
            free(p);
            return;
        }
        last = p;
        p = p->next;
    }
}

/* go through the list to find the description */
static void
get_redirect_desc(unsigned short eport, int proto,
                  char * desc, int desclen)
{
    struct rdr_desc * p;
    if(!desc || (desclen == 0))
        return;
    for(p = rdr_desc_list; p; p = p->next)
    {
        if(p->eport == eport && p->proto == (short)proto)
        {
            strncpy(desc, p->str, desclen);
            return;
        }
    }
    /* if no description was found, return miniupnpd as default */
    strncpy(desc, "miniupnpd", desclen);
}

static int
get_redirect_desc_by_index(int index, unsigned short * eport, int * proto,
                  char * desc, int desclen)
{
    int i = 0;
    struct rdr_desc * p;
    if(!desc || (desclen == 0))
        return -1;
    for(p = rdr_desc_list; p; p = p->next, i++)
    {
        if(i == index)
        {
            *eport = p->eport;
            *proto = (int)p->proto;
            strncpy(desc, p->str, desclen);
            return 0;
        }
    }
    return -1;
}
/* BEGIN 2032802867 zhoujianchun 203875 2012.04.03 added */
int
add_redirect_rule2(const char * rhost, unsigned short eport,
                   const char * iaddr, unsigned short iport, int proto,
                   const char * desc, int enabled)
{
    NP_UPNP_INFO("enter add_redirect_rule2, enabled arg is %d\n", enabled);
    char eaddr[16] = {0};
    /*BEGIN 3082010042 y00248130 2013-8-22 added/modified*/
    int r = -1;     
    /* END  3082010042 y00248130 2013-8-22 added/modified*/
    char rule[256]={0};

    /*if (getifaddr(ifname, eaddr, 16) != 0) return -1;
    sprintf(rule, "rdr %s %s port %d -> %s port %d %s", 
        ifname, eaddr, eport, iaddr, iport, 
        (proto == IPPROTO_TCP)? "tcp":"udp");
    */

    /* this line cause warning:
     * missing initializer(near initialization for 'portmap_info.portmap_enabled')
     */
    //NAT_PORTMAP_INFO_ST portmap_info = {0};
    
    NAT_PORTMAP_INFO_ST portmap_info;
    memset(&portmap_info, 0, sizeof(portmap_info));
    portmap_info.portmap_index = 0;
    portmap_info.portmap_enabled = ENABLE_FEATURE;
    if(NULL != desc)
    {
        strncpy((char *)portmap_info.portmap_name, desc, NAME_LEN_MAX);
    }
    if(NULL != rhost)
    {
        strncpy((char *)portmap_info.remote_host_addr, rhost, IPV4_ADDRESS_LEN);
    }

    if(NULL != iaddr)
    {
        strncpy((char *)portmap_info.internal_client_addr, iaddr, IPV4_ADDRESS_LEN);
    }
    else
    {
        NP_UPNP_DEBUG("internal addr is null, return without rule adding.\n");
        return -1;

    }
    strncpy((char *)portmap_info.protocol, ((IPPROTO_TCP == proto) ? "tcp" : "udp"), IP_PROTO_LEN);
    portmap_info.external_port = eport;
    portmap_info.external_port_end = eport;
    portmap_info.internal_port = iport;
    
    if(1 == enabled)
    {
        r = AddUPnPPortMapRule(&portmap_info);
    }

    /*BEGIN 3082010042 y00248130 2013-8-22 added/modified*/
    if (0 == r)
    {   
        char none[16] = "none";
        NP_UPNP_DEBUG("AddUPnPPortMapRule success.\n");
        add_redirect_desc(eport, proto, eaddr, iaddr, iport, 
                (char *)none, 
                (char *)desc, enabled);
    }
    else
    {
        NP_UPNP_ERROR("AddUPnPPortMapRule fail!\n");
    }
    /* END  3082010042 y00248130 2013-8-22 added/modified*/
    return r;
}
/* END 2032802867 zhoujianchun 203875 2012.04.03 added */
int
add_filter_rule2(const char * ifname, const char * iaddr,
                 unsigned short eport, unsigned short iport,
                 int proto, const char * desc)
{
    return 0;
}

/* get_redirect_rule() 
 * returns -1 if the rule is not found */
int
get_redirect_rule(const char * ifname, unsigned short eport, int proto,
                  char * iaddr, int iaddrlen, unsigned short * iport,
                  char * desc, int desclen,
                  char * rhost, int rhostlen,
                  unsigned int * timestamp,
                  u_int64_t * packets, u_int64_t * bytes, int *penabled)
{
    struct rdr_desc * p;
    int enabled_tmp;

    short protoTmp = proto;

    for(p = rdr_desc_list; p; p = p->next)
    {
        NP_UPNP_DEBUG("~~~~~export =%d, %d, proto=%d, %d, ifname=%s, %s\n", 
                p->eport, eport, p->proto, proto, ifname, p->ifname);

        if(p->eport == eport)
        {
            NP_UPNP_DEBUG("eport is same\n");

            if(p->proto == protoTmp) 
            {
                NP_UPNP_DEBUG("proto is same\n");

                //if(!strcmp(ifname, p->ifname))
                {
                    //printf("ifname is same\n");
                    strncpy(iaddr, p->iaddr, iaddrlen);
                    *iport = p->iport;
                    /* set current enable flag, return old enable flag */
                    enabled_tmp = p->enabled;
                    p->enabled = *penabled;
                    *penabled = enabled_tmp;
                    strncpy(desc, p->str, desclen);
                    if(NULL != packets)
                    {
                        *packets = 0;
                    }
                    if(NULL != bytes)
                    {
                        *bytes = 0;
                    }
                    return 0;
                }
            }
        }
    }
    return -1;
}

int
get_redirect_rule2(const char * ifname, unsigned short eport, int proto,
                  char * iaddr, int iaddrlen, unsigned short * iport,
                  char * desc, int desclen,
                  char * rhost, int rhostlen,
                  unsigned int * timestamp,
                  u_int64_t * packets, u_int64_t * bytes, int *penabled)
{
    struct rdr_desc * p;

    short protoTmp = proto;

    for(p = rdr_desc_list; p; p = p->next)
    {
        NP_UPNP_DEBUG("~~~~~export =%d, %d, proto=%d, %d, ifname=%s, %s\n", 
                p->eport, eport, p->proto, proto, ifname, p->ifname);

        if(p->eport == eport)
        {
            NP_UPNP_DEBUG("eport is same\n");

            if(p->proto == protoTmp) 
            {
                NP_UPNP_DEBUG("proto is same\n");

                //if(!strcmp(ifname, p->ifname))
                {
                    //printf("ifname is same\n");
                    strncpy(iaddr, p->iaddr, iaddrlen);
                    *iport = p->iport;
                    *penabled = p->enabled;
                    strncpy(desc, p->str, desclen);
                    if(NULL != packets)
                    {
                        *packets = 0;
                    }
                    if(NULL != bytes)
                    {
                        *bytes = 0;
                    }
                    return 0;
                }
            }
        }
    }
    return -1;
}

/* get_redirect_rule_by_index() 
 * return -1 when the rule was not found */
int
get_redirect_rule_by_index(int index,
                           char * ifname, unsigned short * eport,
                           char * iaddr, int iaddrlen, unsigned short * iport,
                           int * proto, char * desc, int desclen,
                           char * rhost, int rhostlen,
                           unsigned int * timestamp,
                           u_int64_t * packets, u_int64_t * bytes, int *penabled)
{
    NP_UPNP_INFO("enter get_redirect_rule_by_index\n");
    NP_UPNP_INFO("index is %d\n", index);
#ifdef DEBUG
    list_redirect_rule(ext_if_name);
#endif
    int i = 0;
    struct rdr_desc * p;
    if(!desc || (desclen == 0))
        return -1;
    for(p = rdr_desc_list; p; p = p->next, i++)
    {
        if(i == index)
        {
            NP_UPNP_DEBUG("**** i == index ****\n");

            if (ifname != NULL)
                strncpy(ifname, p->ifname, 16);
            if (eport != NULL)
                *eport = p->eport;
            if (iaddr != NULL)
                strncpy(iaddr, p->iaddr, iaddrlen);
            if (iport != NULL)
                *iport = p->iport;
            if (proto != NULL)
                *proto = (int)p->proto;
            if (desc != NULL)
                strncpy(desc, p->str, desclen);
            if(penabled != NULL)
                *penabled = p->enabled;
            
            return 0;
        }
    }
    return -1;
}

/* delete_redirect_and_filter_rules()
 */
int
delete_redirect_and_filter_rules(unsigned short eport, int proto)
{
    int r = -1;
    struct rdr_desc * p, * last;
    char rule[256];
    int is_nat_flashed = 0;
    
    p = rdr_desc_list;
    last = 0;
    while(p)
    {
        if(p->eport == eport && p->proto == proto)
        {
            if(!last)
                rdr_desc_list = p->next;
            else
                last->next = p->next;
            snprintf(rule, sizeof(rule),"rdr %s %s port %d -> %s port %d %s",
                p->ifname, p->eaddr, p->eport, p->iaddr, p->iport, 
                (p->proto == IPPROTO_TCP)? "tcp":"udp");
            NP_UPNP_DEBUG("%s\n", rule);
            DelUPnPPortMapRule();
            is_nat_flashed = 1;
            r = 0;
        }
        last = p;
        p = p->next;
    }
    if(1 == is_nat_flashed)
    {
        p = rdr_desc_list;
        while(p)
        {
            NAT_PORTMAP_INFO_ST portmap_info;
            memset(&portmap_info, 0, sizeof(portmap_info));
            portmap_info.portmap_index = 0;
            portmap_info.portmap_enabled = ENABLE_FEATURE;
            if(NULL != p->str)
            {
                strncpy((char *)portmap_info.portmap_name, p->str, NAME_LEN_MAX);
            }
            if(NULL != p->eaddr)
            {
                strncpy((char *)portmap_info.remote_host_addr, p->eaddr, IPV4_ADDRESS_LEN);
            }
            if(NULL != p->iaddr)
            {
                strncpy((char *)portmap_info.internal_client_addr, p->iaddr, IPV4_ADDRESS_LEN);
            }
            strncpy((char *)portmap_info.protocol, ((6 == p->proto) ? "tcp" : "udp"), IP_PROTO_LEN);
            portmap_info.external_port = p->eport;
            portmap_info.external_port_end = p->eport;
            portmap_info.internal_port = p->iport;
    
            if(1 == p->enabled)
            {
                AddUPnPPortMapRule(&portmap_info);
            }

            p = p->next;
        }
    }

    del_redirect_desc(eport, proto);
    
    return r;
}

/* return an (malloc'ed) array of "external" port for which there is
 * a port mapping. number is the size of the array */
unsigned short *
get_portmappings_in_range(unsigned short startport, unsigned short endport,
                          int proto, unsigned int * number)
{
    unsigned short * array;
    unsigned int capacity;
    unsigned short eport;
    struct rdr_desc * p;

    *number = 0;
    capacity = 128;
    eport = 0;
    array = (unsigned short *)calloc(capacity, sizeof(unsigned short));
    if(!array)
    {
        NP_UPNP_ERROR("get_portmappings_in_range() : calloc error\n");
        return NULL;
    }

    for(p = rdr_desc_list; p; p = p->next)
    {
        if ((p->proto == proto) && (startport <= eport && eport <= endport))
        {
            array[*number] = eport;
            (*number)++;
            if(*number >= capacity)
            {
                 /* need to increase the capacity of the array */
                array = (unsigned short *)realloc(array, sizeof(unsigned short)*capacity);
                if(!array)
                {
                    NP_UPNP_ERROR("get_portmappings_in_range() : realloc(%lu) error\n", sizeof(unsigned short)*capacity);
                    *number = 0;
                    break;
                }
            }
        }
    }

    return array; 
}

int
list_redirect_rule(const char * ifname)
{
    NP_UPNP_DEBUG("************************************\n");
    struct rdr_desc * p;
    for(p = rdr_desc_list; p; p = p->next)
    {
        NP_UPNP_DEBUG("rdr %s %s port %d -> %s port %d %s enabled=%d\n", 
            p->ifname, p->eaddr, p->eport, p->iaddr, p->iport,
            (p->proto == IPPROTO_TCP)?"tcp":"udp", p->enabled);
    }
    NP_UPNP_DEBUG("************************************\n");
    
    return 0;
}


#ifdef MBB_FEATURE_PCP
/*****************************************************************************
 函数名称  : update_all_rules_to_pcp_client
 功能描述  : 将所有的规则发送到pcp client
 输入参数  : No
 输出参数  : No
 返 回 值  : No
*****************************************************************************/
void update_all_rules_to_pcp_client(void)
{      
    int wan_status;
    struct rdr_desc * p = NULL;

    NP_UPNP_DEBUG("update_all_rules_to_pcp_client entered.\n");
    wan_status = MgntGetWanState();

    if (WAN_STATUS_CONNECTED == wan_status)
    {
        for (p = rdr_desc_list; NULL != p; p = p->next)
        {
            add_pcp_rule(p->eport, p->proto, 0, p->enabled, p->iaddr, p->iport);
        }
    }
}
#endif

