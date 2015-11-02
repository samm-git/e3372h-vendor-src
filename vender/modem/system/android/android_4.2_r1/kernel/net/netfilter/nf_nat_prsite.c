/* (C) 2001-2002 Magnus Boden <mb@ozaba.mine.nu>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Version: 0.0.7
 *
 * Thu 21 Mar 2002 Harald Welte <laforge@gnumonks.org>
 * 	- Port to newnat API
 *
 * This module currently supports DNAT:
 * iptables -t nat -A PREROUTING -d x.x.x.x -j DNAT --to-dest x.x.x.y
 *
 * and SNAT:
 * iptables -t nat -A POSTROUTING { -j MASQUERADE , -j SNAT --to-source x.x.x.x }
 *
 * It has not been tested with
 * -j SNAT --to-source x.x.x.x-x.x.x.y since I only have one external ip
 * If you do test this please let me know if it works or not.
 *
 */
/*
*/
#include <linux/module.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/tcp.h>

#include <linux/netfilter.h>
#include <linux/netfilter/nf_conntrack_prsite.h>
#include <net/checksum.h>
#include <net/tcp.h>


#define  IGNORESTRING  "/?updataredirect="
#define  SUFFIXLEN     8
#define  DATALENGTH    1500
MODULE_LICENSE("GPL");

MODULE_DESCRIPTION("http NAT helper");
MODULE_ALIAS("nf_nat_http");


#if 1
 #define PRSITE_DEBUGP(format, args...) printk(format, ## args)
#else
 #define PRSITE_DEBUGP(format, args...)
#endif
#if 1
 #define PRSITE_STATIC_DEBUGP(format, args...) do {printk(format, ## args);} while (0)
#else
 #define PRSITE_STATIC_DEBUGP(format, args...)
#endif


extern int (*prsite_add_hook)(char __user * optval);
extern int (*prsite_del_hook)(char __user * optval);


/*************** start of 添加静态IP地址，默认Computer。add by c60023298 ***************/

struct prsite_url_info g_stPrsiteUrlInfo = {0};


static struct user_agent_list g_browser_list[] =
{
    //Safari浏览器
    {"Safari", "",         0         },
    //IE浏览器
    {"MSIE", "",         0         },
    //Chrome浏览器
    {"Chrome","",          0         },
    //Opera浏览器
    //{"Opera", "OPR/",   12          },
    {"Opera","",          0         },
    //Firefox浏览器
    {"Firefox","",          0         },
    //UCWEB浏览器
    {"UCWEB","",          0         },
    //IE11
    {"rv:11.0","",          0         },
    {NULL, NULL ,      0         }
};

static char g_blackurlsuffix [][SUFFIXLEN] = 
{
    "gif","jpeg","tiff","ico",
    "xbm","xpm","png","erf",
    "jpg","jpe","rgb","svf",
    "tif","pict","bmp"
};





static int http_set_appdata(struct sk_buff *poldskb, void *pdata, int len)
{
    struct iphdr *iph = NULL;
    struct tcphdr *tcph = NULL;
    u_int32_t tcplen;
    u_int32_t datalen;
    struct sk_buff *newskb = NULL;
    unsigned int taddr = 0;           /* Temporary IP holder */
    short tport = 0;
    u_int32_t tseq = 0;
    struct ethhdr *peth = NULL;
    unsigned char t_hwaddr[ETH_ALEN] = {0};
    struct net_device *dev = NULL;
    struct sk_buff *pskb = NULL;
    int tmplen = 0;
    int tmptcplen = 0;


    if (NULL == poldskb || (NULL == pdata))
    {
        return 1;
    }
    
    pskb = skb_copy(poldskb, GFP_ATOMIC);
    
    if (NULL == pskb)
    {
        return 1;
    }
    iph = ip_hdr(pskb);
    
    if (NULL == iph)
    {
        kfree_skb(pskb);
        return 1;
    } 
    tcph = (struct tcphdr *)(pskb->data + iph->ihl * 4);
    if(NULL == tcph)
    {
        kfree_skb(pskb);
        return 1;
    }
#if 0    
    if (skb_shinfo(pskb)->frag_list)
    {
        newskb = skb_copy(pskb, GFP_ATOMIC);
        if (!newskb)
        {
            return 1;
        }

        kfree_skb(pskb);
        pskb = newskb;
    }
#endif
    if ((pskb->end - pskb->data - iph->ihl * 4 - tcph->doff * 4) < len)
    {   
        PRSITE_DEBUGP("data len %d expand len %d\n",
                       pskb->end - pskb->data - iph->ihl * 4 - tcph->doff * 4,len);
        struct sk_buff * skb2 = skb_copy_expand(pskb, 0, len, GFP_ATOMIC);
        if (NULL == skb2)
        {
            kfree_skb(pskb);
            return 1;
        }

        kfree_skb(pskb);
        pskb = skb2;
    }
    
    iph = ip_hdr(pskb);
    
    if (NULL == iph)
    {
        kfree_skb(pskb);
        return 1;
    } 
    tcph = (struct tcphdr *)(pskb->data + iph->ihl * 4);
    if(NULL == tcph)
    {
        kfree_skb(pskb);
        return 1;
    }

    tmplen = pskb->len - iph->ihl * 4 - sizeof(struct tcphdr);
    PRSITE_DEBUGP("tcp data %d \n",tmplen);
    memset(pskb->data + iph->ihl * 4 + sizeof(struct tcphdr),
           0, pskb->len - iph->ihl * 4 - sizeof(struct tcphdr));
      

    memcpy(pskb->data + iph->ihl * 4 + sizeof(struct tcphdr),
           pdata, len);

    tmptcplen = (tcph->doff * 4);
    tcph->doff	= sizeof(struct tcphdr) / 4;
    tport = tcph->source;
    tcph->source = tcph->dest;
    tcph->dest = tport;
    
    tseq = tcph->ack_seq;
  
    //tcph->ack_seq = htonl(ntohl(tcph->seq) + tcph->syn + tcph->fin +
	//			      pskb->len - ip_hdrlen(pskb) -
	//			      (tcph->doff << 2)); 
    tcph->ack_seq = htonl(ntohl(tcph->seq) + tcph->syn + tcph->fin +
				      pskb->len - ip_hdrlen(pskb) -
				      tmptcplen);                       
    tcph->seq = tseq;
    tcph->ack = 1;  
   // tcph->fin = 1;

    taddr = iph->saddr;
    iph->saddr = iph->daddr;
    iph->daddr = taddr;
    iph->ttl -= 1;

    iph->tot_len = htons(iph->ihl * 4 + sizeof(struct tcphdr) + len);
    iph->check = 0;
    iph->check = ip_fast_csum((unsigned char *)iph, iph->ihl);
    /* fix checksum information */
    if (tmplen > len)
    {
        tcplen  = pskb->len - iph->ihl * 4 - (tmplen - len);    
    }
    else
    {
       tcplen  = pskb->len - iph->ihl * 4;
    }
    
    datalen = tcplen - tcph->doff * 4;
    pskb->csum = csum_partial((char *)tcph + tcph->doff * 4, datalen, 0);

    tcph->check = 0;
    tcph->check = tcp_v4_check(tcplen, iph->saddr, iph->daddr,
                               csum_partial((char *)tcph, tcph->doff * 4,
                                            pskb->csum));

    //pskb->pkt_type = PACKET_OTHERHOST;
    pskb->pkt_type = PACKET_OUTGOING;
//===================================


    pskb->data = (unsigned char *)pskb->mac_header;
    pskb->tail = pskb->data + ETH_HLEN  + sizeof(struct iphdr) + sizeof(struct tcphdr) + len;
    //pskb->tail = pskb->data + ETH_HLEN  + (iph->ihl * 4) + sizeof(struct tcphdr) + len;
    pskb->len = pskb->tail - pskb->data;
    peth = pskb->mac_header;
    if (NULL == peth)
    {
        PRSITE_DEBUGP("eth_hdr error ======>\n");
        kfree_skb(pskb);
        return 1;
    }        
    if (pskb->tail > pskb->end)
    {
        PRSITE_DEBUGP("prsite deal with error,ignore this buf \n");
        kfree_skb(pskb);
        return 1;
    }
   
    memcpy(t_hwaddr, peth->h_dest, ETH_ALEN); 
    memcpy(peth->h_dest, peth->h_source, ETH_ALEN);  
    memcpy(peth->h_source, t_hwaddr, ETH_ALEN);
    
//==========================================    

    
    dev_queue_xmit(pskb);

    return 0;
}


/*增加强制门户节点*/
int prsite_add(char __user *optval)
{
    int ret = 0;
    struct affined_bind bind   = {0};

    PRSITE_DEBUGP("prsite_add ======>\n");

    if (copy_from_user(&bind, optval, sizeof(struct affined_bind)))
    {
        ret = 1;
        return ret;
    }

    if (strlen(bind.url) == 0)
    {
        g_stPrsiteUrlInfo.lEnable = 0;
        return ret;
    }
    else
    {
        g_stPrsiteUrlInfo.ul_lan_addr = ntohl(bind.addr);
        g_stPrsiteUrlInfo.ul_lan_mask = ntohl(bind.mask);
        strncpy(g_stPrsiteUrlInfo.ac_stb_url, bind.url, HTTP_URL_MAX);
        g_stPrsiteUrlInfo.lEnable = bind.flag;
    }
    PRSITE_DEBUGP("++++++ ul_lan_addr %u ul_lan_mask %u ac_stb_url %s g_stPrsiteUrlInfo.lEnable %d\n",
        g_stPrsiteUrlInfo.ul_lan_addr,g_stPrsiteUrlInfo.ul_lan_mask,g_stPrsiteUrlInfo.ac_stb_url,g_stPrsiteUrlInfo.lEnable);

   // PRSITE_STATIC_DEBUGP("\n\t add ipaddr <%u.%u.%u.%u>", NIPQUAD(g_stPrsiteUrlInfo.ul_lan_addr));
  //  PRSITE_STATIC_DEBUGP("\n\t add mask <%u.%u.%u.%u>", NIPQUAD(g_stPrsiteUrlInfo.ul_lan_mask));
    return ret;
}

/*删除强制门户节点*/
int prsite_del(char __user *optval)
{
    PRSITE_DEBUGP("prsite_del ======>\n");
    memset(&g_stPrsiteUrlInfo,0,sizeof(g_stPrsiteUrlInfo));
    return 0;
}


int isspace(int x)
{
    if(' ' == x || '\t' == x || '\n' == x || '\f' == x || '\b' == x || '\r' == x)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int isdigit(int x)
{
    if(x <= '9' && x >= '0')
    {
        return 1;
    }
    else 
    {
        return 0;
    }
}

//atoi实现原型
int s_atoi(const char *nptr)
{
    int c;              /* current char */
    int total;         /* current total */
    int sign;           /* if '-', then negative, otherwise positive */

    /* skip whitespace */
    while ( isspace((int)(unsigned char)*nptr) )
    {
        ++nptr;
    }
    c = (int)(unsigned char)*nptr++;
    sign = c;           /* save sign indication */
    if ('-' == c || '+' == c)
    {
        c = (int)(unsigned char)*nptr++;    /* skip sign */
    }

    total = 0;

    while (isdigit(c)) 
    {
        total = 10 * total + (c - '0');     /* accumulate digit */
        c = (int)(unsigned char)*nptr++;    /* get next char */
    }

    if ('-' == sign)
    {
        return -total;
    }
    else
    {
        return total;   /* return result, negated if necessary */
    }
}

int check_version_white_list(char *puser_agent,int version)
{
    int num;
    if ( NULL == puser_agent)
    {
        return 1;
    }
    num = s_atoi(puser_agent);
    PRSITE_DEBUGP("change to num = %d\n",num);
    if( num >= version)
    {
        return 0;
    }
    return 1;
}

static int checkuser_agent_version(char *puser_agent)
{
    int i,j = 0;
    char *temp = NULL;
    char str_version[32] = {0};
    temp = puser_agent;
    for (i = 0; NULL != g_browser_list[i].key_type; i++)
    {
        //关键字在浏览器列表中
        if (NULL != strstr(puser_agent,g_browser_list[i].key_type))
        {
            //判断版本号
            temp = strstr(puser_agent,g_browser_list[i].key_version);
            if (NULL != temp)
            {
                temp = temp + strlen(g_browser_list[i].key_version);
                while (*temp != ' ')
                {
                    if (j >= 31)
                    {
                        return 1;
                    }

                     str_version[j] = *temp;
                     j++;
                     temp++;
                }
                PRSITE_DEBUGP("str_version = %s\n",str_version);

                if(!check_version_white_list(str_version,g_browser_list[i].version_num))
                {
                    return 0;
                }
            }
            PRSITE_DEBUGP("key_type %s is not in list\n",g_browser_list[i].key_type);
        }
    }
    return 1;
}




static int http_changeto_newurl(struct sk_buff *pskb, char *url)
{
    char http_newdata[HTTP_RESPONSE_BUF_MAX + 1] = {0};
    PRSITE_DEBUGP("http_changeto_newurl ======>\n");
    /*
    HTTP/1.1 302 Redirection\r\n
    Content-length: 0\r\n
    Location: http://192.168.1.1/html/update.html?updataredirect=10.14.10.153/onlineupdate/\r\n
    Connection: Close\r\n\r\n
     */
    //snprintf(http_newdata,HTTP_RESPONSE_BUF_MAX + 1,
    //    "%s\r\nContent-length: 0\r\nLocation: http://%s?updataredirect=%s\r\nConnection: Close\r\n\r\n",
    //    "HTTP/1.1 302 Redirection",g_stPrsiteUrlInfo.ac_stb_url,url);
    snprintf(http_newdata,HTTP_RESPONSE_BUF_MAX + 1,
        "%s\r\nContent-length: 0\r\nLocation: http://%s?updataredirect=%s\r\nConnection: Close\r\n\r\n",
        "HTTP/1.1 307 Temporary Redirect",g_stPrsiteUrlInfo.ac_stb_url,url);

    PRSITE_DEBUGP("HTTP ALG: new http response \n%s\n", http_newdata);

    return http_set_appdata(pskb, http_newdata, strlen(http_newdata));
}


static int checkurlsuffix(char *purl)
{
    char *p = purl; 
    char *ptmp = NULL;
    if (0 == strcmp("/",purl))
    {
        PRSITE_DEBUGP("checkurlsuffix+++++++++++++++\n");
        return 0;
    }
    else
    {
        ptmp = strrchr(p,'.');
        if(NULL == ptmp)
        {
            return 1;
        }
        else
        {
            ptmp += 1;
            if(0 == strcmp(ptmp,"jsp")
               || 0 == strcmp(ptmp,"asp")
               || 0 == strcmp(ptmp,"htm")
               || 0 == strcmp(ptmp,"xhtml")
               || 0 == strcmp(ptmp,"aspx")
               || 0 == strcmp(ptmp,"php")
               || 0 == strcmp(ptmp,"cgi"))
            {
                return 0;
            }
            else
            {
                if(0 == strcmp(ptmp,"html"))
                {
                    if(strstr(purl, "library/test/success"))
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
                return 1;
            }
        }
    }
}


static int checkblacksuffix(char *purl)
{
    char *p = purl; 
    int i = 0;
    char *ptmp = NULL;
    if (0 == strcmp("/",purl))
    {
        return 0;
    }
    else
    {
        ptmp = strrchr(p,'.');
        if(NULL == ptmp)
        {
            return 0;
        }
        else
        {
            ptmp += 1;
            for(i = 0; i< sizeof(g_blackurlsuffix)/sizeof(g_blackurlsuffix[0]); i++)
            {
                if (0 == strcmp(ptmp,g_blackurlsuffix[i]))
                {
                    PRSITE_DEBUGP("+++++++++++++++blacksuffix %s ignore!!\n",g_blackurlsuffix[i]);
                    return 1;
                }                 
            }
        }
    }
    return 0;
}

static unsigned int http_prsite_info(unsigned char *pdata,unsigned char *purl,int useflag)
{
    unsigned char *p = NULL;
    unsigned char *ptmp = NULL;
    char uri[224]  = {0}; 
    char user_agent[224] = {0};   
    char host[64] = {0};
    int i = 0 ;
    p = pdata + 4;
    while (*p != ' ')
    {
        if (i >= 223)
        {
            return 1;
        }        
        uri[i] = *p;
        i++;
        p++;    
    }
    PRSITE_DEBUGP("uri %s\n", uri);
    if (strstr(uri,IGNORESTRING))
    {
        return 1;
    }
    
    if (checkblacksuffix(uri))
    {
        return 1;
    }
    
    if (2 == useflag)
    {
        if(checkurlsuffix(uri))
        {
            PRSITE_DEBUGP("checkurlsuffix not need======>\n");
            return 1;
        }
    }
    ptmp = strstr(p, "\r\nHost: "); 
    if (NULL == ptmp) 
    {
        return 1; 
    }
    ptmp = ptmp + strlen("\r\nHost: "); 
    i = 0;  
    while ((*ptmp != '\r') && (*ptmp != '\n'))
    {
        if (i >= 63)     
        {
            return 1; 
        }
        host[i] = *ptmp;   
        i++;
        ptmp++; 
    }
    PRSITE_DEBUGP("host %s\n", host);  
    ptmp = NULL;   
    ptmp = strstr(p, "\r\nUser-Agent: ");  
    if (NULL == ptmp) 
    {
        PRSITE_DEBUGP("User-Agent not need======>\n");
        return 1;     
    }
    ptmp = ptmp + strlen("\r\nUser-Agent: "); 

    i = 0;   
    while ((*ptmp != '\r') && (*ptmp != '\n'))  
    {
        if (i >= 223)  
        {
            user_agent[223] = 0;
            break;   
        }
        user_agent[i] = *ptmp;  
        i++;   
        ptmp++;
    }
     
    if(checkuser_agent_version(user_agent)) 
    {        
        PRSITE_DEBUGP("checkuser_agent,the boswer is not support======>\n");
        return 1; 
    }
    if (0 == strcmp("/",uri))   
    {
        snprintf(purl, HTTP_URL_MAX,"%s", host);   
    }    
    else  
    {     
        snprintf(purl, HTTP_URL_MAX,"%s%s", host, uri); 
    } 
    return 0;
}



unsigned int nf_prsite_in(struct sk_buff *skb, const struct net_device *pin, const struct net_device *pout)
{
    struct iphdr *iph = NULL;
    struct tcphdr *tcph = NULL;
    char data[DATALENGTH] = {0};
    unsigned char url[HTTP_URL_MAX] = {0};
    int useflag = g_stPrsiteUrlInfo.lEnable;
        
    if (!useflag)
    {
        //PRSITE_STATIC_DEBUGP("prsite is not enable\n");
        return NF_ACCEPT;
    }
    
    iph = ip_hdr(skb);

    if (NULL == iph)
    {
        return NF_ACCEPT;   
    }
    
    if (iph->frag_off & htons(IP_OFFSET))	
    {
        return NF_ACCEPT;
    }

    if (iph->protocol != IPPROTO_TCP)
    {
        return NF_ACCEPT;   
    }

    tcph = (void *)iph + iph->ihl * 4;

    if (tcph->dest != htons(80))
    {
        return NF_ACCEPT; 
    }
    
    if ((iph->saddr == htonl(0x7F000001))||(iph->daddr == htonl(0x7F000001)))
    {
        PRSITE_DEBUGP("lo accpet ======>\n");
        return NF_ACCEPT; 
    }
    
    if ((iph->saddr == g_stPrsiteUrlInfo.ul_lan_addr)
        ||(iph->daddr == g_stPrsiteUrlInfo.ul_lan_addr)) 
    {          
        return NF_ACCEPT;   
    }   
    if ((iph->saddr & g_stPrsiteUrlInfo.ul_lan_mask)== (iph->daddr & g_stPrsiteUrlInfo.ul_lan_mask))  
    {     
        //PRSITE_STATIC_DEBUGP("prsite ipaddr is local address(lan)\n");    
        return NF_ACCEPT;  
    }
    
    if (tcph->doff * 4 + iph->ihl * 4 == skb->len)
    {
  	    return NF_ACCEPT;
    }
    
    if (strncmp((void *)tcph + tcph->doff * 4, HTTP_PROTO_GET, strlen(HTTP_PROTO_GET)) != 0)
    {
        return NF_ACCEPT;
    }
    
    PRSITE_DEBUGP("skb->len %d - %d - %d = %d>\n", skb->len, tcph->doff * 4, iph->ihl * 4, skb->len - tcph->doff * 4 - iph->ihl * 4);
    if (skb->len - tcph->doff * 4 - iph->ihl * 4 < DATALENGTH - 1)
    {
        strncpy(data, (void *)tcph + tcph->doff * 4, skb->len - tcph->doff * 4 - iph->ihl * 4);
    }
    else
    {
        return NF_ACCEPT;
    }
   
    
    if (http_prsite_info(data,url,useflag) == 1)
    {
        return NF_ACCEPT;
    }

    if (http_changeto_newurl(skb, url) == 1)
    {
        return NF_ACCEPT;
    }
    
    /*重定向一次*/
    if (2 == useflag)
    {
        g_stPrsiteUrlInfo.lEnable = 0;
    }
    
    return NF_DROP;
}
EXPORT_SYMBOL_GPL(nf_prsite_in);

static void __exit nf_nat_http_fini(void)
{
    rcu_assign_pointer(prsite_add_hook, NULL);
    rcu_assign_pointer(prsite_del_hook, NULL);

    synchronize_rcu();
}

static int __init nf_nat_http_init(void)
{
    PRSITE_DEBUGP("nf_nat_http_init <*********** start ************>\n");
    BUG_ON(rcu_dereference(prsite_add_hook) != NULL);
    BUG_ON(rcu_dereference(prsite_del_hook) != NULL);
    
    rcu_assign_pointer(prsite_add_hook, prsite_add);
    rcu_assign_pointer(prsite_del_hook, prsite_del);
    
    return 0;
}

module_init(nf_nat_http_init);
module_exit(nf_nat_http_fini);
