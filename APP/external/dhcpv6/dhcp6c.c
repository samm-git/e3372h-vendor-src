/*	$KAME: dhcp6c.c,v 1.164 2006/01/10 02:46:09 jinmei Exp $	*/
/*
 * Copyright (C) 1998 and 1999 WIDE Project.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the project nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE PROJECT AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE PROJECT OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
/*
  <问题单号>     <作  者>    <修改时间>   <版本> <修改描述>
    2032203980  m00182044   2012/6/10 DHCP6c不能通过RA进行无状态IPv6地址配置
    3082101267  y00188255  2013/8/24        修改所有获取时间的方式为单板上电时间
    3082010042  y00188255  2013/08/23       消除fortify报警
    3112001144  s00216562   2013/11/21       pppoe支持ipv6
    4031801302  s00216562   2014/03/18       动态ip支持ipv6
    4032807844  s00216562   2014/04/22       单板接入仅支持ipv6的服务器，动态ip拨号失败
    4091702971  f00178256   2014/09/20       offload双栈切换为offload ipv4单栈 br0仍能获取IPv6地址
 */
#include <sys/types.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <sys/queue.h>
#include <errno.h>
#include <limits.h>
#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif
#include <net/if.h>
#ifdef __FreeBSD__
#include <net/if_var.h>
#endif

#include <netinet/in.h>
#ifdef __KAME__
#include <net/if_dl.h>
#include <netinet6/in6_var.h>
#endif

#include <arpa/inet.h>
#include <netdb.h>

#include <signal.h>
#include <stdio.h>
#include <stdarg.h>
#include <syslog.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <err.h>
#include <ifaddrs.h>

#include <dhcp6.h>
#include <config.h>
#include <common.h>
#include <timer.h>
#include <dhcp6c.h>
#include <control.h>
#include <dhcp6_ctl.h>
#include <dhcp6c_ia.h>
#include <prefixconf.h>
#include <auth.h>

/* start of by d00107688 2009-10-20 检测WAN地址 */
#include "addrstate.h"
#include "ra.h"
#include "misc.h"
/* end of by d00107688 2009-10-20 检测WAN地址 */

/* BEGIN: Added by z67728, 2009/11/21 */
/* END:   Added by z67728, 2009/11/21 */

static int debug = 0;
//int exit_ok = 0;
static sig_atomic_t sig_flags = 0;
#define SIGF_TERM 0x1
#define SIGF_HUP 0x2
#define SIGF_USR1 0x4
/*新增信号60，用于退出dhcp6c，但不进行上报*/
#define SIGEXEXITDHCP6C 60
#define SIGF_EXITDHCP6C 0x8

#ifdef SUPPORT_ATP_IPV6_ALWAYS_SEND_RS
/* BEGIN: Added by z67728, 2011/4/15 电信需求:
    1. wan侧为存DHCP方式时也需要发送RS,获取RA后添加默认路由.
    2. 不使用RA生成无状态地址.
    3. 路由更新保持原有机制,不使用RA中的RouteLife配置项.
    4. 删除WAN DHCPv6 模式中的静态路由CMS配置项.
    5. RS发完后,无RA响应时,使用接口路由作为默认路由.*/
#define IPV6_CONFIG_FOLDER                  ("/proc/sys/net/ipv6/conf/")
#define IPV6_CONFIG_FILE_ACCEPT_RA          ("/accept_ra")
#define IPV6_CONFIG_FILE_ACCEPT_RA_PINFO    ("/accept_ra_pinfo")
#define IPV6_CONFIG_FILE_ACCEPT_RA_DEFRTR   ("/accept_ra_defrtr")
#define IPV6_CONFIG_FILE_ACCEPT_RA_RTR_PREF ("/accept_ra_rtr_pref")
/* END:   Added by z67728, 2011/4/15 */
#endif

const dhcp6_mode_t dhcp6_mode = DHCP6_MODE_CLIENT;

int sock;	/* inbound/outbound udp port */
int rtsock;	/* routing socket */
int ctlsock = -1;		/* control TCP port */
char *ctladdr = DEFAULT_CLIENT_CONTROL_ADDR;
char *ctlport = DEFAULT_CLIENT_CONTROL_PORT;

#define DEFAULT_KEYFILE SYSCONFDIR "/dhcp6cctlkey"
#define CTLSKEW 300
/*  Status Code define*/
#define STCode_NoPrefixAvail        (0x01)
#define STCode_NOADDRSAVAIL         (0x02)

char *conffile = NULL;
//static char *conffile = DHCP6C_CONF;

static const struct sockaddr_in6 *sa6_allagent;
static struct duid client_duid;

/* start of by d00107688 2009-10-27 增加vendor class的标签 */
static struct vendor_class g_stVC;
/* end of by d00107688 2009-10-27 增加vendor class的标签 */


static char *pid_file = DHCP6C_PIDFILE;

static char *ctlkeyfile = DEFAULT_KEYFILE;
static struct keyinfo *ctlkey = NULL;
static int ctldigestlen;

static int infreq_mode = 0;

int g_lSignalPipev6[2];


/* start of by d00107688 2009-10-20 支持IPv6 处理路由通告 */
int g_iEnableRA = 0;                        /* 默认不开启RA */
int g_iEnableAutoConf = 0;                  /* 默认不开启自动配置 */
int g_iIAPDFlags = 0;                       /* 是否开启IAPD */
int g_iIANAFlags = 0;                       /* 是否开启IANA */
int g_iDsliteFlags = 0;                     /* 是否开启Dslite */
int g_iDftRt = 1;                           /* SLAAC时是否接收默认路由信息,默认为1表示开启，为0表示关闭 */
int g_iDhcpStartFlag = 0;
int g_iReadDialFile = 0; 
int g_iIfindex = 0;
/* end of by d00107688 2009-10-20 支持IPv6 处理路由通告 */


static inline int get_val32 __P((char **, int *, u_int32_t *));
static inline int get_ifname __P((char **, int *, char *, int));

static void usage __P((void));

#if 0
static void client6_init __P((void));
#else
static void client6_init __P((char *));
#endif

static void client6_startall __P((int));
static void free_resources __P((struct dhcp6_if *));
static void client6_mainloop __P((void));
static int client6_do_ctlcommand __P((char *, ssize_t));
static void client6_reload __P((void));
static int client6_ifctl __P((char *ifname, u_int16_t));
static void check_exit __P((void));
static void process_signals __P((void));
static struct dhcp6_serverinfo *find_server __P((struct dhcp6_event *,
						 struct duid *));
static struct dhcp6_serverinfo *select_server __P((struct dhcp6_event *));
//static void client6_recv __P((void));
static void client6_recv __P((int));
static int client6_recvadvert __P((struct dhcp6_if *, struct dhcp6 *,
				   ssize_t, struct dhcp6_optinfo *));
static int client6_recvreply __P((struct dhcp6_if *, struct dhcp6 *,
				  ssize_t, struct dhcp6_optinfo *));
static void client6_signal __P((int));
static struct dhcp6_event *find_event_withid __P((struct dhcp6_if *,
						  u_int32_t));
static int construct_confdata __P((struct dhcp6_if *, struct dhcp6_event *));
static int construct_reqdata __P((struct dhcp6_if *, struct dhcp6_optinfo *,
    struct dhcp6_event *));
static void destruct_iadata __P((struct dhcp6_eventdata *));
static void tv_sub __P((struct timeval *, struct timeval *, struct timeval *));
static struct dhcp6_timer *client6_expire_refreshtime __P((void *));
static int process_auth __P((struct authparam *, struct dhcp6 *dh6, ssize_t,
    struct dhcp6_optinfo *));
static int set_auth __P((struct dhcp6_event *, struct dhcp6_optinfo *));

struct dhcp6_timer *client6_timo __P((void *));
int client6_start __P((struct dhcp6_if *));
static void info_printf __P((const char *, ...));

extern int client6_script __P((char *, int, struct dhcp6_optinfo *));

#define MAX_ELAPSED_TIME 0xffff

/* BEGIN: Added by z67728, 2010/1/16 PN:AU4D02303 comment : Add a signal for dhcp6c declien packet.*/
#ifndef DHCP6_DECLIEN
#define DHCP6_DECLIEN
#endif

#ifdef DHCP6_DECLIEN

#define DHCP6_DEBUG     (1)
#define DHCP6_CHECK_NUM (1)

#if DHCP6_DEBUG
#define dhcp6_debug(format,arg...) printf("\r\n"format"FILE[%s] FUNC[%s] LINE[%d] \r\n",## arg,__FILE__,__func__,__LINE__) 
#define FUNSTART  printf("\r\n ************* Function : %s start .FILE[%s] LINE[%u] ************** \r\n",__func__,__FILE__,__LINE__)
#define FUNEND    printf("\r\n ************* Function : %s end .FILE[%s] LINE[%u] **************** \r\n",__func__,__FILE__,__LINE__)
#else
#define dhcp6_debug(format,arg...)
#define FUNSTART
#define FUNEND
#endif

enum pointer_type_e{
    POINTER_TYPE_IAPD = 1,
    POINTER_TYPE_IANA = 2,
    POINTER_TYPE_BUFF
};

enum addr_type_e{
    ADDR_TYPE_STATELESS = 0,
    ADDR_TYPE_DHCPV6    = 1,
    ADDR_TYPE_NOTYPE
};

enum return_type_e{
    RETURN_FAIL = -1,
    RETURN_TRUE = 0
};

enum decline_switch_flag_e{
    DECLINE_SWITCH_DISABLE = 0,
    DECLINE_SWITCH_ENABLE  = 1
};

struct iana_addr_st{
    struct in6_addr addr;
    unsigned long   uliaid;
    unsigned long   ulT1;
    unsigned long   ulT2;
    unsigned long   ulPreferLifetime;
    unsigned long   ulValidLifetime;
};

struct iapd_addr_st{
    struct dhcp6_prefix prefix;
    unsigned long       uliaid;
    unsigned long       ulT1;
    unsigned long       ulT2;
};

struct iana_option_st{
    unsigned short  optiontype;
    unsigned short  optionlength;
    unsigned long   iaid;
    unsigned long   t1;
    unsigned long   t2;
    unsigned short  subtype;
    unsigned short  sublength;
    struct in6_addr addr;
    unsigned long   preferlifetime;
    unsigned long   validlifetime;
};

struct duid_option_st{
    unsigned short optiontype;
    unsigned short optionleng;
    char cztembuff[16];
};

struct ifinfo_st{
    struct ifinfo_st *next;
    struct ifinfo_st *pre;
    
    char *ifname;
    unsigned long ullinkid;

    struct duid clientID;	/* DUID */
	struct duid serverID;	/* DUID */

    union {
        struct iana_addr_st ifdhcpaddr;
        struct iapd_addr_st ifdhcppref;
    }iainfo;

    unsigned long uliatype;
    
#define iana iainfo.ifdhcpaddr
#define iapd iainfo.ifdhcppref
};

static struct ifinfo_st *g_pstifinfoheader = NULL;
static int g_ldeclineswitch = DECLINE_SWITCH_DISABLE;

static int client6_duplicateifname __P((struct ifinfo_st *pstaddr,const char *ifname));
static int client6_freeifname __P((struct ifinfo_st *pstaddr));
static int client6_addnewaddrnode __P((void* srcpara,const char *ifname,unsigned long ullinkid,struct duid* pclientID,struct duid* pserverID,unsigned long ulpointertype));
static int client6_delcurrentaddrnode __P((struct ifinfo_st *pstaddr));
static void client6_freechain __P((struct ifinfo_st **pstifinfoheader));
static struct ifinfo_st * client6_findaddrfromchain __P((struct in6_addr* paddr));
static void client6_addrduplicatesignal __P((int));
static int client6_senddeclinepacket __P((struct ifinfo_st *pstaddrinfo));
static int client6_initifstatuemachine __P((struct ifinfo_st *pstaddrinfo));
static void client6_addressduplicateprocess __P(());
static int client6_declinefunctioninterface __P((struct in6_addr *pstipv6addr,long lmode,const char * pcinterface));
static int client6_generatenewstatelessaddress __P((struct in6_addr *pstipv6addr,const char * pcifname));

static int
client6_duplicateifname(struct ifinfo_st *pstaddr,const char *ifname)
{
    FUNSTART;
        
    if ( !pstaddr || !ifname )
    {
        dhcp6_debug("%s","Input parameter meet error !");
        return RETURN_FAIL;
    }

    pstaddr->ifname = (char*)malloc(strlen(ifname) + 1);
    if ( NULL == pstaddr->ifname )
    {
        dhcp6_debug("%s","malloc meet error !");
        return RETURN_FAIL;
    }
    else
    {
        memset(pstaddr->ifname,0,(strlen(ifname) + 1));
        strcpy(pstaddr->ifname,ifname);
    }

    FUNEND;
    
    return RETURN_TRUE;
}

static int
client6_freeifname(struct ifinfo_st *pstaddr)
{
    FUNSTART;
    
    if ( !pstaddr )
    {
        dhcp6_debug("%s","Input parameter meet error !");
        return RETURN_FAIL;
    }

    if ( NULL != pstaddr->ifname )
    {
        free(pstaddr->ifname);
        pstaddr->ifname = NULL;
    }

    FUNEND;
    
    return RETURN_TRUE;
}

static int
client6_addnewaddrnode(void* srcpara,const char *ifname,unsigned long ullinkid, struct duid* pclientID,struct duid* pserverID,unsigned long ulpointertype)
{
    int                 ret        = RETURN_TRUE;
    int                 loopi      = 0;
    struct ifinfo_st   *pstnewaddr = NULL;

    FUNSTART;
        
    if ( !srcpara || !ifname || !pclientID || !pserverID )
    {
        dhcp6_debug("%s","Input parameter meet error !");
        return RETURN_FAIL;
    }

    if ( POINTER_TYPE_IAPD != ulpointertype && POINTER_TYPE_IANA != ulpointertype )
    {
        dhcp6_debug("%s","Input parameter meet error !");
        return RETURN_FAIL;
    }

    do
    {        
        pstnewaddr = (struct ifinfo_st *)malloc(sizeof(*pstnewaddr));
        if (NULL == pstnewaddr )
        {
            dhcp6_debug("%s","malloc meet error !");
            break;
        }
        else
        {
            memset(pstnewaddr,0,sizeof(*pstnewaddr));

            ret = client6_duplicateifname(pstnewaddr,ifname);
            if ( RETURN_TRUE != ret )
            {
                dhcp6_debug("%s","FUN[ client6_duplicateifname ] meet error !");
                break;
            }

            pstnewaddr->ullinkid = ullinkid;
            
            if ( POINTER_TYPE_IAPD == ulpointertype )
            {
                memcpy(&pstnewaddr->iapd,(struct iapd_addr_st *)srcpara,sizeof(struct iapd_addr_st));
                pstnewaddr->uliatype = POINTER_TYPE_IAPD;
            }
            else
            {
                memcpy(&pstnewaddr->iana,(struct iana_addr_st *)srcpara,sizeof(struct iana_addr_st));                
                pstnewaddr->uliatype = POINTER_TYPE_IANA;
            }
            

            if ( duidcpy(&pstnewaddr->clientID,pclientID) )
            {
                dhcp6_debug("%s","duidcpy client id meet error !");
                break;
            }

            if ( duidcpy(&pstnewaddr->serverID,pserverID) )
            {
                dhcp6_debug("%s","duidcpy server id meet error !");
                break;
            }

            pstnewaddr->next = g_pstifinfoheader;
            if ( NULL != g_pstifinfoheader )
            {
                g_pstifinfoheader->pre = pstnewaddr;
            }

            g_pstifinfoheader = pstnewaddr;
        }

        FUNEND;
            
        return RETURN_TRUE;
    } while ( 1 );

    if ( NULL != pstnewaddr )
    { 
        if ( NULL != pstnewaddr->ifname )
        {
            client6_freeifname(pstnewaddr);
        }

        duidfree(&pstnewaddr->serverID);
        duidfree(&pstnewaddr->clientID);
        
        free(pstnewaddr);
    }
    
    return RETURN_FAIL;
}

static int
client6_delcurrentaddrnode(struct ifinfo_st *pstaddr)
{
    struct ifinfo_st *psttemaddr = NULL;

    FUNSTART;
    
    if ( !pstaddr )
    {
        dhcp6_debug("%s","Input parameter meet error !");
        return RETURN_FAIL;
    }

    psttemaddr = pstaddr;
    if ( NULL != psttemaddr->next && NULL != psttemaddr->pre )
    {
        psttemaddr->next->pre = psttemaddr->pre;
        psttemaddr->pre->next = psttemaddr->next;
    }
    else if ( NULL != psttemaddr->next && NULL == psttemaddr->pre )
    {
        /* header of chain */
        g_pstifinfoheader = psttemaddr->next;
    }
    else if ( NULL == psttemaddr->next && NULL != psttemaddr->pre )
    {
        /* tail of chain */
        psttemaddr->pre->next = NULL;
    }
    else if ( NULL == psttemaddr->next && NULL == psttemaddr->pre )
    {
        /* only one node in the chain */
        g_pstifinfoheader = NULL;
    }
    else
    {
        dhcp6_debug("%s","No this type !");
        return RETURN_FAIL;
    }

    client6_freeifname(psttemaddr);
    duidfree(&psttemaddr->serverID);
    duidfree(&psttemaddr->clientID);
    
    free(psttemaddr);

    FUNEND;
        
    return RETURN_TRUE;
}

static void 
client6_freechain(struct ifinfo_st **pstifinfoheader)
{
    struct ifinfo_st *psttemaddr = NULL;

    FUNSTART;
    
    if ( !(*pstifinfoheader) )
    {
        dhcp6_debug("%s","Input parameter meet error !");
        return;
    }

    psttemaddr       = *pstifinfoheader;
    *pstifinfoheader = (*pstifinfoheader)->next;

    do
    {
        client6_freeifname(psttemaddr);
        
        duidfree(&psttemaddr->serverID);
        duidfree(&psttemaddr->clientID);
        
        free(psttemaddr);

        psttemaddr = *pstifinfoheader;
        if ( NULL != *pstifinfoheader )
        {
            *pstifinfoheader = (*pstifinfoheader)->next;
        }
        
    } while ( psttemaddr );

    FUNEND;
    
    return;
}

static struct ifinfo_st *
client6_findaddrfromchain(struct in6_addr* paddr)
{
    struct ifinfo_st *psttemaddr = NULL;

    FUNSTART;
    
    if ( !paddr )
    {
        dhcp6_debug("%s","Input parameter meet error !");
        return NULL;
    }

    if ( NULL != g_pstifinfoheader )
    {
        psttemaddr = g_pstifinfoheader;

        do
        {
            if ( POINTER_TYPE_IANA == psttemaddr->uliatype && 0 == memcmp(&psttemaddr->iana.addr.s6_addr,paddr->s6_addr,16) )
            {
                FUNEND;
                return psttemaddr;
            }

            psttemaddr = psttemaddr->next;
        } while ( psttemaddr );
    }

    FUNEND;
    
    return NULL;
}

static struct ifinfo_st *
client6_findPrefixfromchain(struct in6_addr* pstPrefix)
{
    struct ifinfo_st *psttemaddr = NULL;

    FUNSTART;
    
    if ( !pstPrefix )
    {
        dhcp6_debug("%s","Input parameter meet error !");
        return NULL;
    }

    if ( NULL != g_pstifinfoheader )
    {
        psttemaddr = g_pstifinfoheader;

        do
        {
            if ( POINTER_TYPE_IAPD == psttemaddr->uliatype && 0 == strncasecmp(&psttemaddr->iapd.prefix.addr.s6_addr,pstPrefix->s6_addr,8) )
            {
                FUNEND;
                return psttemaddr;
            }

            psttemaddr = psttemaddr->next;
        } while ( psttemaddr );
    }

    FUNEND;
    return NULL;
}

static int
client6_setlistvalue(struct dhcp6_list * pstlistheader,unsigned long ultype, struct dhcp6_if * pstif,struct duid* pstserverID,struct duid* pstclientID)
{
    int                   lret = RETURN_FAIL;
    struct dhcp6_listval *iav  = NULL;
    struct dhcp6_listval *siav = NULL;
    struct ia_conf       *iac  = NULL;
    struct iana_addr_st   staddpara;
    struct iapd_addr_st   stpfxpara;

    FUNSTART;
    
    if ( !pstlistheader || !pstif || !pstserverID )
    {
        dhcp6_debug("%s","Input parameter meet error !");
        return RETURN_FAIL;
    }

    memset(&staddpara,0,sizeof(struct iana_addr_st));
    memset(&stpfxpara,0,sizeof(struct iapd_addr_st));
    
    for ( iav = TAILQ_FIRST(pstlistheader); iav; iav = TAILQ_NEXT(iav, link)) {
        if ((iac = find_iaconf(&pstif->iaconf_list, ultype,
		    iav->val_ia.iaid)) == NULL) {
			continue;
		}
                        
        for (siav = TAILQ_FIRST(&iav->sublist); siav;siav = TAILQ_NEXT(siav, link)) {
            switch (siav->type) {
                case DHCP6_LISTVAL_STATEFULADDR6:
                    memcpy(&staddpara.addr,&siav->val_statefuladdr6.addr,sizeof(struct in6_addr));
                    staddpara.ulPreferLifetime = siav->val_statefuladdr6.pltime;
                    staddpara.ulValidLifetime  = siav->val_statefuladdr6.vltime;
                    staddpara.uliaid           = iav->val_ia.iaid;
                    staddpara.ulT1             = siav->val_statefuladdr6.pltime / 2;
                    if ( 0x3fffffff > siav->val_statefuladdr6.pltime )
                    {                        
                        staddpara.ulT2 = (siav->val_statefuladdr6.pltime * 4) / 5;
                    }
                    else
                    {
                        staddpara.ulT2 = (siav->val_statefuladdr6.pltime / 5) * 4;
                    }
                     
                    dhcp6_debug(" Parameter list : \r\n Addr[%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x] \r\n PreferLifetime[%08x] \r\n ValidLifetime[%08x] \r\n IAID[%08x]",
                        ntohs(staddpara.addr.s6_addr16[0]),
                        ntohs(staddpara.addr.s6_addr16[1]),
                        ntohs(staddpara.addr.s6_addr16[2]),
                        ntohs(staddpara.addr.s6_addr16[3]),
                        ntohs(staddpara.addr.s6_addr16[4]),
                        ntohs(staddpara.addr.s6_addr16[5]),
                        ntohs(staddpara.addr.s6_addr16[6]),
                        ntohs(staddpara.addr.s6_addr16[7]),
                        staddpara.ulPreferLifetime,
                        staddpara.ulValidLifetime,
                        staddpara.uliaid);

                    lret = client6_addnewaddrnode(&staddpara,pstif->ifname,pstif->linkid,pstclientID,pstserverID,POINTER_TYPE_IANA);
                    if ( RETURN_TRUE != lret )
                    {
                        dhcp6_debug("%s","FUN[client6_addnewaddrnode] meet error !");
                        return RETURN_FAIL;
                    }
                    
                    break;
                case DHCP6_LISTVAL_PREFIX6:
                    memcpy(&stpfxpara.prefix.addr,&siav->val_prefix6.addr,sizeof(struct in6_addr));
                    stpfxpara.prefix.pltime = siav->val_prefix6.pltime;
                    stpfxpara.prefix.vltime = siav->val_prefix6.vltime;
                    stpfxpara.prefix.plen   = siav->val_prefix6.plen;
                    stpfxpara.uliaid        = iav->val_ia.iaid;
                    stpfxpara.ulT1          = siav->val_prefix6.pltime / 2;
                    if ( 0x3fffffff > siav->val_prefix6.pltime )
                    {                        
                        stpfxpara.ulT2 = (siav->val_prefix6.pltime * 4) / 5;
                    }
                    else
                    {
                        stpfxpara.ulT2 = (siav->val_prefix6.pltime / 5) * 4;
                    }
                    
                    dhcp6_debug(" Parameter list : \r\n Addr[%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x] \r\n PreferLifetime[%08x] \r\n ValidLifetime[%08x] \r\n LEN[%04x] \r\n IAID[%08x]",
                        ntohs(stpfxpara.prefix.addr.s6_addr16[0]),
                        ntohs(stpfxpara.prefix.addr.s6_addr16[1]),
                        ntohs(stpfxpara.prefix.addr.s6_addr16[2]),
                        ntohs(stpfxpara.prefix.addr.s6_addr16[3]),
                        ntohs(stpfxpara.prefix.addr.s6_addr16[4]),
                        ntohs(stpfxpara.prefix.addr.s6_addr16[5]),
                        ntohs(stpfxpara.prefix.addr.s6_addr16[6]),
                        ntohs(stpfxpara.prefix.addr.s6_addr16[7]),
                        stpfxpara.prefix.pltime,
                        stpfxpara.prefix.vltime,
                        stpfxpara.prefix.plen,
                        stpfxpara.uliaid);

                    lret = client6_addnewaddrnode(&stpfxpara,pstif->ifname,pstif->linkid,pstclientID,pstserverID,POINTER_TYPE_IAPD);
                    if ( RETURN_TRUE != lret )
                    {
                        dhcp6_debug("%s","FUN[client6_addnewaddrnode] meet error !");
                        return RETURN_FAIL;
                    }
                    
    				break;
                default :
                    break;
            }
        }
    }

    FUNEND;

    return RETURN_TRUE;
}

static int client6_senddeclinepacket(struct ifinfo_st *pstaddrinfo)
{    
	char                  buf[BUFSIZ] = {0};
    char                 *temppointer = NULL;
	ssize_t               len         = 0;
    int                   loopi       = 0;
	struct dhcp6         *dh6         = NULL;
	struct sockaddr_in6   dst;
    struct iana_option_st stianaoption;

    FUNSTART;
    
    /* init */
    memset(&dst ,0,sizeof(struct sockaddr_in6));
    memset(&stianaoption,0,sizeof(struct iana_option_st));
    
    if ( NULL == pstaddrinfo ){
        dhcp6_debug("%s","Input parameter meet error !");
        return RETURN_FAIL;
    }

    /* Create dhcp packet header */
    temppointer = buf;
    
    dh6 = (struct dhcp6 *)buf;

    dh6->dh6_xid     = 0;
    dh6->dh6_msgtype = DH6_DECLINE;

    len = sizeof(*dh6);

    temppointer += len;

    /* Copy Client Duid */
    *((unsigned short *)temppointer) = htons(DH6OPT_CLIENTID);
    temppointer = temppointer + 2;

    *((unsigned short *)temppointer) = htons(pstaddrinfo->clientID.duid_len);
    temppointer = temppointer + 2;

    memcpy(temppointer,pstaddrinfo->clientID.duid_id,pstaddrinfo->clientID.duid_len);
    temppointer = temppointer + pstaddrinfo->clientID.duid_len;

    len += 2 + 2 + pstaddrinfo->clientID.duid_len;

    /* Copy Server Duid */
    *((unsigned short *)temppointer) = htons(DH6OPT_SERVERID);
    temppointer = temppointer + 2;
    
    *((unsigned short *)temppointer) = htons(pstaddrinfo->serverID.duid_len);
    temppointer = temppointer + 2;

    memcpy(temppointer,pstaddrinfo->serverID.duid_id,pstaddrinfo->serverID.duid_len);
    temppointer = temppointer + pstaddrinfo->serverID.duid_len;

    len += 2 + 2 + pstaddrinfo->serverID.duid_len;
    
    /* Copy IANA Option */
    stianaoption.optiontype     = htons(3);
    stianaoption.optionlength   = htons(40);
    stianaoption.iaid           = htonl(pstaddrinfo->iana.uliaid);
    stianaoption.t1             = htonl(pstaddrinfo->iana.ulT1);
    stianaoption.t2             = htonl(pstaddrinfo->iana.ulT2);
    stianaoption.subtype        = htons(5);
    stianaoption.sublength      = htons(24);
    stianaoption.preferlifetime = htonl(pstaddrinfo->iana.ulPreferLifetime);
    stianaoption.validlifetime  = htonl(pstaddrinfo->iana.ulValidLifetime);
    memcpy(&stianaoption.addr,&pstaddrinfo->iana.addr,sizeof(struct in6_addr));

    memcpy(temppointer,(char*)&stianaoption,sizeof(stianaoption));

    len += sizeof(stianaoption);

    temppointer += len;

    /* Set dest */
    dst = *sa6_allagent;
	dst.sin6_scope_id = pstaddrinfo->ullinkid;
    
    /* Send packet */
    if (sendto(sock, buf, len, 0, (struct sockaddr *)&dst,sysdep_sa_len((struct sockaddr *)&dst)) == -1) {
		dhcp6_debug("Send decline packet failed: %s", strerror(errno));
        return RETURN_FAIL;
	}

    FUNEND;

    return RETURN_TRUE;
}

static int 
client6_initifstatuemachine(struct ifinfo_st *pstaddrinfo)
{
    struct dhcp6_if *ifp     = NULL;
    struct ia_conf  *iac     = NULL; 
    struct ia       *ia      = NULL;
    struct ia       *ia_next = NULL;

    FUNSTART;
    
    if ( NULL == pstaddrinfo || NULL == pstaddrinfo->ifname ){
        dhcp6_debug("%s","Input parameter meet error !");
        return RETURN_FAIL;
    }

    dhcp6_debug("if name : %s .",pstaddrinfo->ifname);

    for (ifp = dhcp6_if; ifp; ifp = ifp->next) {
        if ( 0 != strcmp(pstaddrinfo->ifname,ifp->ifname) ) {
            continue; /* XXX: try to recover? */
        }

        remove_all_ia(ifp,pstaddrinfo->iana.uliaid);
        break;
    }

    FUNEND;
    
    return RETURN_TRUE;
}

static int 
client6_generatenewstatelessaddress(struct in6_addr *pstipv6addr,const char * pcifname)
{
    char czTempaddr[128] = {0};
    char czTempBuff[128] = {0};
    long lRandomNum      = 0;
    long lLoopi          = 0;
    struct in6_addr stTemaddr;
    
    if ( !pstipv6addr || !pcifname )
    {
        dhcp6_debug("%s","Input parameter meet error !");
        return RETURN_FAIL;
    }

    FUNSTART;
    
    memset(&stTemaddr,0,sizeof(struct in6_addr));
    
    /* generate and set new stateless address */
    for ( lLoopi = 0 ; lLoopi < 4 ; lLoopi++ )
    {        
        if ( 3 == lLoopi )
        {
            lRandomNum = time(0) + lLoopi;
            srand((unsigned long)lRandomNum);
            stTemaddr.s6_addr[11] = (rand() % 256);
            dhcp6_debug("Position : 11 , value : %x .",stTemaddr.s6_addr[11]);
        }
        else
        {            
            lRandomNum = time(0) + lLoopi;
            srand((unsigned long)lRandomNum);
            
            lRandomNum = (rand() % 16);
            if ( 8 > lRandomNum )
            {
                lRandomNum += 8;
            }

            stTemaddr.s6_addr[lRandomNum] = (rand() % 256);

            dhcp6_debug("Position : %d , value : %x .",lRandomNum,stTemaddr.s6_addr[lRandomNum]);
        }
    }

    inet_ntop(AF_INET6, &stTemaddr, czTempaddr,128);
    snprintf(czTempBuff, 127, "ifconfig %s add %s/128",pcifname,czTempaddr);
    dhcp6_debug("CMD[%s]",czTempBuff);
    system(czTempBuff);

    memset(czTempaddr,0,sizeof(czTempaddr));
    memset(czTempBuff,0,sizeof(czTempBuff));
    
    memcpy(&stTemaddr,pstipv6addr,sizeof(struct in6_addr));

    /* delete duplicate address : ifconfig interface del address/64 */
    inet_ntop(AF_INET6, &stTemaddr, czTempaddr,128);
    snprintf(czTempBuff, 127, "ifconfig %s del %s/128",pcifname,czTempaddr);
    dhcp6_debug("CMD[%s]",czTempBuff);
    system(czTempBuff);

    FUNEND;
    
    return RETURN_TRUE;
}

static int 
client6_declinefunctioninterface(struct in6_addr *pstipv6addr,long lmode,const char * pcinterface)
{
    struct ifinfo_st *pstifinfo = NULL;
    
    if ( NULL == pstipv6addr || NULL == pcinterface )
    {
        dhcp6_debug("%s","Input parameter meet error !");
        return RETURN_FAIL;
    }

    FUNSTART;
    
    switch ( lmode )
    {
        case ADDR_TYPE_DHCPV6 : 
            dhcp6_debug("%s","mode is ADDR_TYPE_DHCPV6 !");
            pstifinfo = client6_findaddrfromchain(pstipv6addr);
            if ( NULL != pstifinfo )
            {
                /* send decline packet */
                client6_senddeclinepacket(pstifinfo);

                /* resend solicit packet */
                client6_initifstatuemachine(pstifinfo);

                /* delete current node */
                client6_delcurrentaddrnode(pstifinfo);
            }
            else
            {
                dhcp6_debug("%s","No this address !");
                return RETURN_FAIL;
            }
            break;
        case ADDR_TYPE_STATELESS :
            dhcp6_debug("%s","mode is ADDR_TYPE_STATELESS !");
            if ( NULL != (pstifinfo = client6_findPrefixfromchain(pstipv6addr)) )
            {
                client6_generatenewstatelessaddress(pstipv6addr,pcinterface);
            }
            else
            {
                dhcp6_debug("%s","No this prefix !");
                return RETURN_FAIL;
            }
            break;
        default:
            dhcp6_debug("%s","No this mode !");
            return RETURN_FAIL;
    }

    FUNEND;
    
    return RETURN_TRUE;
}

static void 
client6_addressduplicateprocess()
{
    FILE         *pFile          = NULL;
    char         *pcAddrFile     = "/proc/net/if_inet6";
    char          czTemBuff[128] = {0};
    char          czTemName[128] = {0};
    char          czTemAddr[128] = {0};
    char          czFilAddr[128] = {0};
    long          lRet           = 0;
    unsigned long ulTemLength    = 0;
    unsigned long ulLoopi        = 0;
    unsigned long ulLoopj        = 0;
    unsigned long ulTemFlag      = 0;
    struct in6_addr staddress;

    FUNSTART;
    
    memset(&staddress,0,sizeof(struct in6_addr));

    sleep(3);

    pFile = fopen(pcAddrFile,"r");
    if ( NULL != pFile )
    {
        while ( fgets(czTemBuff,128,pFile) && NULL != g_pstifinfoheader)
        {
            sscanf(czTemBuff,"%s %x %x %x %x %s",czTemAddr,&ulTemFlag,&ulTemFlag,&ulTemFlag,&ulTemFlag,czTemName);

            ulTemLength = strlen(czTemAddr);

            for (ulLoopi = 0, ulLoopj = 0; ulLoopi < ulTemLength; ulLoopi++, ulLoopj++)
            {
                if ((ulLoopi != 0)
                    && (ulLoopi % 4 == 0))
                {
                    czFilAddr[ulLoopj] = ':';
                    ulLoopj++;
                }
                czFilAddr[ulLoopj] = czTemAddr[ulLoopi];
            }

            dhcp6_debug("Addr[%s] , flag[%#02x] ifname[%s] .",czFilAddr,ulTemFlag,czTemName);
            if ( inet_pton(AF_INET6, czFilAddr, &staddress) > 0 && !strcmp(czTemName,"nas0") && 0xc0 == ulTemFlag && !IN6_IS_ADDR_LINKLOCAL(&staddress) )
            {
                /* 无状态地址重复 */
                lRet = client6_declinefunctioninterface(&staddress,ADDR_TYPE_STATELESS,"nas0");
                if ( RETURN_TRUE != lRet )
                {                    
                    /* 有状态地址重复 */
                    client6_declinefunctioninterface(&staddress,ADDR_TYPE_DHCPV6,"nas0");
                }
            }
        }
        fclose(pFile);
    }

    FUNEND;
    
    return;
}

static void
client6_addrduplicatesignal(sig)
    int sig;
{
    printf("\r\n *************** Dhcp6c receive signal : SIGUSR2 . ******************\r\n");
    if ( DECLINE_SWITCH_ENABLE == g_ldeclineswitch )
    {        
        printf("\r\n Decline switch : DECLINE_SWITCH_DISABLE \r\n");
        g_ldeclineswitch = DECLINE_SWITCH_DISABLE;
    }
    else
    {
        printf("\r\n Decline switch : DECLINE_SWITCH_ENABLE \r\n");
        g_ldeclineswitch = DECLINE_SWITCH_ENABLE;
    }
    
    return;
}
#endif
/* END:   Added by z67728, 2010/1/16 */

/* BEGIN: Added by z67728, 2011/4/19 电信需求修改. */
#ifdef SUPPORT_ATP_IPV6_ALWAYS_SEND_RS
/*****************************************************************************
 函 数 名  : client6_set_krnl_parameter
 功能描述  : 根据标志位设置内核中的对应配置项。
             配置文件夹:
             /proc/sys/net/ipv6/conf/
             配置项包含:
             accept_ra          : 是否接收RA报文.               1 : 是. 0 : 否
             accept_ra_pinfo    : 是否使用RA前缀生成无状态地址. 1 : 是. 0 : 否
             accept_ra_defrtr   : 是否使用RA生成默认路由.       1 : 是. 0 : 否
             accept_ra_rtr_pref : 设置路由优先级.               1 : medium. 0 : 
 输入参数  : const char *pcInterface  
             const char *pcParaName   
             int         iParameter   
 输出参数  : 无
 返 回 值  : static void
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2011年4月19日
    作    者   : z67728
    修改内容   : 新生成函数

*****************************************************************************/
static void client6_set_krnl_parameter
(
    const char *pcInterface,
    const char *pcParaName,
    int         iParameter
)
{
    char acFile[128]      = {0};
    char acTemBuffer[256] = {0};

    if ( NULL == pcInterface || NULL == pcParaName )
    {
        return;
    }
    
    /* 修改指定wan接口的RA配置项 */
    if ( strlen(pcInterface) )
    {
        /* 修改指定wan接口的RA配置项,该wan接口不能使用RA前缀生成无状态地址. */
        snprintf(acFile,
                 sizeof(acFile),
                 "%s%s%s",
                 IPV6_CONFIG_FOLDER,
                 pcInterface,
                 pcParaName);

        snprintf(acTemBuffer,sizeof(acTemBuffer),"echo %d > %s",
                 iParameter,
                 acFile);
        system(acTemBuffer);
    }
    
    return;
}
#endif
/* END:   Added by z67728, 2011/4/19 */
int main(int argc, char **argv)
{
	int ch, pid;
	char *progname;
	FILE *pidfp;
    int iCnt = 0;
/* start of by d00107688 2009-10-20 支持IPv6 处理路由通告 */
    char acPidFile[32] = {0};
/* end of by d00107688 2009-10-20 支持IPv6 处理路由通告 */
#ifdef SUPPORT_ATP_IPV6_ALWAYS_SEND_RS
/* BEGIN: Added by z67728, 2011/4/15 电信需求修改. */
    char acInterface[16]  = {0};
/* END:   Added by z67728, 2011/4/15 */
#endif
	struct dhcp6_if *ifp;

    /*start: 用于查询组件版本号(dns atpv),请不要修改或删除*/
    /*if ((argc == 2) && (NULL != argv[1]) && (0 == strcmp(argv[1],ATP_VERSION_CMD_KEY)))
    {
       printf("\r\n%s.\n", DHCP6C_MODULE_VERSION);    	    		
       exit(0);
    }*/
    /*end */ 
    
#ifndef HAVE_ARC4RANDOM
	srandom(time(NULL) & getpid());
#endif

	if ((progname = strrchr(*argv, '/')) == NULL)
		progname = *argv;
	else
		progname++;

	while ((ch = getopt(argc, argv, "c:dDfiRAINPSFGZk:p:")) != -1) {
		switch (ch) {
		case 'c':
			conffile = optarg;
			break;
		case 'd':
			debug = 1;
			break;
		case 'D':
			debug = 2;
			break;
		case 'f':
			foreground++;
			break;
		case 'i':
			infreq_mode = 1;
			break;
        case 'P':
            g_iIAPDFlags = 1;
            break;
        case 'N':
            g_iIANAFlags = 1;
            break;
        case 'S':
            g_iDsliteFlags = 1;
            break;
		case 'k':
			ctlkeyfile = optarg;
			break;
		case 'p':
			pid_file = optarg;
			break;
/* start of by d00107688 2009-10-20 支持IPv6 处理路由通告 */
        case 'R':
            g_iEnableRA = 1;
            break;
        case 'A':
            g_iEnableAutoConf = 1;
            break;
        case 'F':
            g_iReadDialFile = 1;
            break;
        case 'G':
            g_iDftRt = 0;
            break;
/* end of by d00107688 2009-10-20 支持IPv6 处理路由通告 */
/* BEGIN: Added by z67728, 2010/2/2 decline process .*/
#ifdef DHCP6_DECLIEN
        case 'Z' :
            dhcp6_debug("%s","Set decline switch on .");
            g_ldeclineswitch = DECLINE_SWITCH_ENABLE;
            break;
#else
        case 'Z' :
            dhcp6_debug("%s","Don't include decline switch .");
            break;
#endif
/* END:   Added by z67728, 2010/2/2 */
		default:
			usage();
			exit(0);
		}
	}
	argc -= optind;
	argv += optind;

	if (argc == 0) 
    {
		usage();
		exit(0);
	}
#if 0 
	if (foreground == 0)
	{
		openlog(progname, LOG_NDELAY|LOG_PID, LOG_DAEMON);
	}
    
	setloglevel(debug);
#endif
#ifdef SUPPORT_ATP_IPV6_ALWAYS_SEND_RS
    /* BEGIN: Added by z67728, 2011/4/15 电信需求修改. */
    if ( NULL != argv[0] )
    {
        snprintf(acInterface,sizeof(acInterface),"%s",argv[0]);
    }
    /* END:   Added by z67728, 2011/4/15 */
#endif

    /*BEGIN 2031405958 n00202065 2012-3-13 added*/
    while (argc-- > 0) 
    { 
		if ((ifp = ifinit(argv[0])) == NULL) 
        {
			dprintf(LOG_ERR, FNAME, "failed to initialize [%s]",
			    argv[0]);
			exit(1);
		}                
		argv++;
    }
    for (ifp = dhcp6_if; ifp; ifp = ifp->next)
    {
        snprintf(acPidFile, 32, DHCP6C_PIDFILE, ifp->ifname);
        pid = getpid();
        if ((pidfp = fopen(acPidFile, "w")) != NULL) 
        {
            fprintf(pidfp, "%d\n", pid);
            fclose(pidfp);
        }
    }
	/*END 2031405958 n00202065 2012-3-13  added*/

    /* 因为需要先等待上一个进程退出，否则可能当上一次进程没有退出的时候，那么将会使得
       * 上一个进程的清理工作导致覆盖本进程的初始化工作
       */
    /*BEGIN 2032203980  m00182044 2012-06-10 modify*/
       for (ifp = dhcp6_if; ifp; ifp = ifp->next)
        {
    iCnt = 0;
    g_iIfindex = if_nametoindex(ifp->ifname);
    while (!GetPrevProcessEixtStatus(ifp->ifname))
    {
        fprintf(stderr, 
            "prev process is running on [%s], so sleeping....\r\n\r\n\r\n\r\n",
            ifp->ifname);
        
        sleep(1);
#if 1
        if (6 <= iCnt++)
        {
            fprintf(stderr, "have wait %d seconds, so force to running...", iCnt);
            ExitProcessID(g_iIfindex);
            GetPrevProcessEixtStatus(ifp->ifname);
            break;
        }
#endif
    }
    loginfo(FUNC, "get lock ok, begin to run......");

    while (!IsHaveValidLinkAddress(ifp->ifname))
    {
        loginfo(FUNC, "link-local address is not valid.\r\n\r\n\r\n");
        sleep(1);
    }
    
    socketpair(AF_UNIX, SOCK_STREAM, 0, g_lSignalPipev6);
/* 考虑到DUID的方面，可能需要先将接口进行初始化 */
#if 0
	client6_init();
#else    
    client6_init(ifp->ifname);
#endif
    /*BEGIN 2031405958 n00202065 2012-3-13 deleted*/
    /*END 2031405958 n00202065 2012-3-13  deleted*/
    
#if 0
        if (foreground == 0 && infreq_mode == 0) {
            if (daemon(0, 0) < 0)
                err(1, "daemon");
        }
#endif
        }
    /*END 2032203980  m00182044 2012-06-10 modify*/

    
/* start of by d00107688 2009-10-20 支持IPv6 处理路由通告 */
    for (ifp = dhcp6_if; ifp; ifp = ifp->next)
    { 
#if 0
        // 起线程，监听WAN 的IP地址start pthread for netlink
        StartMonitorWanPthread(ifp->ifname, g_iEnableRA, 
                    g_iIAPDFlags, g_iReadDialFile);
#endif
    /*BEGIN 2031405958 n00202065 2012-3-13 deleted*/
    /*END 2031405958 n00202065 2012-3-13  deleted*/

#ifdef SUPPORT_ATP_IPV6_ALWAYS_SEND_RS
        /* BEGIN: Added by z67728, 2011/4/15 电信需求:
           1. wan侧为存DHCP方式时也需要发送RS,获取RA后添加默认路由.     OK
           2. 不使用RA生成无状态地址.                                   OK
           3. 路由更新保持原有机制,不使用RA中的RouteLife配置项.         OK
           4. 删除WAN DHCPv6 模式中的静态路由CMS配置项.
              代码中默认路由和地址模式复用,当wan地址为静态地址时,默认路由配置项仍然需要.
              因此,只需要在网页上修改显示方式.当ipv6地址类型不为static类型时,不显示默认
              路由配置项.                                               OK
           5. RS发完后,无RA响应时,使用接口路由作为默认路由.             OK    */
        client6_set_krnl_parameter(acInterface,IPV6_CONFIG_FILE_ACCEPT_RA_PINFO,g_iEnableRA);

        /*BEGIN 2032203980  m00182044 2012-06-10 modify*/
        SetAcceptForwarding(ifp->ifname, 2);
        StartRAPthread(ifp->ifname, g_iIANAFlags, g_iIAPDFlags, g_iDsliteFlags, g_iDftRt);
#else
        if (g_iEnableRA)
        {
            SetAcceptForwarding(ifp->ifname, 2);
            StartRAPthread(ifp->ifname, g_iIANAFlags, g_iIAPDFlags, g_iDsliteFlags, g_iDftRt);
           
        }
#endif
        /*END 2032203980  m00182044 2012-06-10 modify*/
        /* END:   Added by z67728, 2011/4/15 */

        // 起线程，监听WAN 的IP地址start pthread for netlink
        StartMonitorWanPthread(ifp->ifname, g_iEnableRA, 
                    g_iIANAFlags, g_iIAPDFlags, g_iDsliteFlags, g_iReadDialFile);
        
        if (!g_iEnableRA)
        {
            conffile = WriteDHCP6CCfg(ifp->ifname, g_iIANAFlags, g_iIAPDFlags, g_iDsliteFlags);
            if (!conffile)
            {
                dprintf(LOG_ERR, FNAME, "error: config file error");
                exit(-127);
            }
        }
    }
    
/* end of by d00107688 2009-10-20 支持IPv6 处理路由通告 */


#if 0
if (infreq_mode == 0 && (cfparse(conffile)) != 0) {
		dprintf(LOG_ERR, FNAME, "failed to parse configuration file");
		//exit(1);
	}
#endif

	//client6_startall(0);
	client6_mainloop();
#ifdef DHCP6_DECLIEN
    if ( DECLINE_SWITCH_ENABLE == g_ldeclineswitch )
    {        
        client6_freechain(&g_pstifinfoheader);
    }
#endif
	exit(0);
}

static void
usage()
{

	fprintf(stderr, "usage: dhcp6c [-c configfile] [-dDIfira] "
	    "[-p pid-file] interface [interfaces...]\n");
}

/*------------------------------------------------------------*/



int sigsegvprocess(int signo)
{
    loginfo(FUNC, "\r\n\r\n\r\ncatch SIGSEGV\r\n\r\n");
    ExitProcessID(g_iIfindex);
    exit(-127);
}

#if 0
void client6_init()
#else
void client6_init(char *pszIfname)
#endif
{
	struct addrinfo hints, *res;
	static struct sockaddr_in6 sa6_allagent_storage;
	int error, on = 1;

    static int iCreate = 0;


#if 0
	/* get our DUID */
	if (get_duid(DUID_FILE, &client_duid)) {
		dprintf(LOG_ERR, FNAME, "failed to get a DUID");
		exit(1);
	}
#else
    if (Client_get_duid(&client_duid, pszIfname)) {
        dprintf(LOG_ERR, FNAME, "failed to get a DUID");
        exit(1);
    }
#endif

    if (Client_get_vendorclass(&g_stVC))
    {
        dprintf(LOG_ERR, FNAME, "failed to get vendor class");
    }

#if 0
	if (dhcp6_ctl_authinit(ctlkeyfile, &ctlkey, &ctldigestlen) != 0) {
		dprintf(LOG_NOTICE, FNAME,
		    "failed initialize control message authentication");
		/* run the server anyway */
	}
#endif


#if 0
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = PF_INET6;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;
	hints.ai_flags = AI_PASSIVE;
	error = getaddrinfo(NULL, DH6PORT_DOWNSTREAM, &hints, &res);
	if (error) {
		dprintf(LOG_ERR, FNAME, "getaddrinfo: %s",
		    gai_strerror(error));
		exit(1);
	}
	sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (sock < 0) {
		dprintf(LOG_ERR, FNAME, "socket");
		exit(1);
	}
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEPORT,
		       &on, sizeof(on)) < 0) {
		dprintf(LOG_ERR, FNAME,
		    "setsockopt(SO_REUSEPORT): %s", strerror(errno));
		exit(1);
	}
#ifdef IPV6_RECVPKTINFO
	if (setsockopt(sock, IPPROTO_IPV6, IPV6_RECVPKTINFO, &on,
		       sizeof(on)) < 0) {
		dprintf(LOG_ERR, FNAME,
			"setsockopt(IPV6_RECVPKTINFO): %s",
			strerror(errno));
		exit(1);
	}
#else
	if (setsockopt(sock, IPPROTO_IPV6, IPV6_PKTINFO, &on,
		       sizeof(on)) < 0) {
		dprintf(LOG_ERR, FNAME,
		    "setsockopt(IPV6_PKTINFO): %s",
		    strerror(errno));
		exit(1);
	}
#endif
	if (setsockopt(sock, IPPROTO_IPV6, IPV6_MULTICAST_LOOP, &on,
		       sizeof(on)) < 0) {
		dprintf(LOG_ERR, FNAME,
		    "setsockopt(sock, IPV6_MULTICAST_LOOP): %s",
		    strerror(errno));
		exit(1);
	}
#ifdef IPV6_V6ONLY
	if (setsockopt(sock, IPPROTO_IPV6, IPV6_V6ONLY,
	    &on, sizeof(on)) < 0) {
		dprintf(LOG_ERR, FNAME, "setsockopt(IPV6_V6ONLY): %s",
		    strerror(errno));
		exit(1);
	}
#endif

    {
        struct sockaddr_in6 *pst;
        pst = (struct sockaddr_in6 *)(res->ai_addr);
        loginfo(FUNC, "create socket in [%0x]\r\n\r\n\r\n\r\n", pst->sin6_scope_id);
    }


	/*
	 * According RFC3315 2.2, only the incoming port should be bound to UDP
	 * port 546.  However, to have an interoperability with some servers,
	 * the outgoing port is also bound to the DH6PORT_DOWNSTREAM.
	 */
	if (bind(sock, res->ai_addr, res->ai_addrlen) < 0) {
		dprintf(LOG_ERR, FNAME, "bind: %s", strerror(errno));
		exit(1);
	}
	freeaddrinfo(res);

#else
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = PF_INET6;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;
	hints.ai_flags = AI_PASSIVE;
	error = getaddrinfo(NULL, DH6PORT_DOWNSTREAM, &hints, &res);
	if (error) {
		dprintf(LOG_ERR, FNAME, "getaddrinfo: %s",
		    gai_strerror(error));
		exit(1);
	}
	sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (sock < 0) {
		dprintf(LOG_ERR, FNAME, "socket");
		exit(1);
	}
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEPORT,
		       &on, sizeof(on)) < 0) {
		dprintf(LOG_ERR, FNAME,
		    "setsockopt(SO_REUSEPORT): %s", strerror(errno));
		exit(1);
	}
#ifdef IPV6_RECVPKTINFO
	if (setsockopt(sock, IPPROTO_IPV6, IPV6_RECVPKTINFO, &on,
		       sizeof(on)) < 0) {
		dprintf(LOG_ERR, FNAME,
			"setsockopt(IPV6_RECVPKTINFO): %s",
			strerror(errno));
		exit(1);
	}
#else
	if (setsockopt(sock, IPPROTO_IPV6, IPV6_PKTINFO, &on,
		       sizeof(on)) < 0) {
		dprintf(LOG_ERR, FNAME,
		    "setsockopt(IPV6_PKTINFO): %s",
		    strerror(errno));
		exit(1);
	}
#endif
	if (setsockopt(sock, IPPROTO_IPV6, IPV6_MULTICAST_LOOP, &on,
		       sizeof(on)) < 0) {
		dprintf(LOG_ERR, FNAME,
		    "setsockopt(sock, IPV6_MULTICAST_LOOP): %s",
		    strerror(errno));
		exit(1);
	}
#ifdef IPV6_V6ONLY
	if (setsockopt(sock, IPPROTO_IPV6, IPV6_V6ONLY,
	    &on, sizeof(on)) < 0) {
		dprintf(LOG_ERR, FNAME, "setsockopt(IPV6_V6ONLY): %s",
		    strerror(errno));
		exit(1);
	}
#endif

#if 0
    if (setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, 
        pszIfname, strlen(pszIfname) + 1) < 0)
    {
        loginfo(FUNC, "bind to device error......\r\n\r\n\r\n");
    }
#endif


    int iIfx;
    int iPort = atoi(DH6PORT_DOWNSTREAM);
    struct sockaddr_in6 stSockaddr;
    iIfx = if_nametoindex(pszIfname);
    if (0 >= iIfx)
    {
        loginfo(FUNC, "Error: if_nametoindex error [%s]\r\n\r\n", strerror(errno));
        close(sock);
        exit(1);
    }

    memset(&stSockaddr,0, sizeof(stSockaddr));
    stSockaddr.sin6_family = AF_INET6;
    stSockaddr.sin6_port = htons(iPort);
    stSockaddr.sin6_scope_id = iIfx;
    GetLinkAddress(&stSockaddr.sin6_addr, iIfx);

    loginfo(FUNC, "bind at: [%s: %s] index: %d port: %d", 
        addr2str((struct sockaddr *)&stSockaddr), 
        (DH6PORT_DOWNSTREAM), iIfx,
        ntohs(stSockaddr.sin6_port));
    
    if (bind(sock, (struct sockaddr *)&stSockaddr, sizeof(stSockaddr)) < 0) {
        dprintf(LOG_ERR, FNAME, "bind: %s", strerror(errno));
        
        if (setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, 
            pszIfname, strlen(pszIfname) + 1) < 0)
        {
            loginfo(FUNC, "bind to device error......\r\n\r\n\r\n");
        }
        
        if (bind(sock, res->ai_addr, res->ai_addrlen) < 0)
        {
            dprintf(LOG_ERR, FNAME, "bind: %s", strerror(errno));
        }

        //exit(1);
    }

	freeaddrinfo(res);

	/*
	 * According RFC3315 2.2, only the incoming port should be bound to UDP
	 * port 546.  However, to have an interoperability with some servers,
	 * the outgoing port is also bound to the DH6PORT_DOWNSTREAM.
	 */
#endif

	/* open a routing socket to watch the routing table */
	if ((rtsock = socket(PF_ROUTE, SOCK_RAW, 0)) < 0) {
		dprintf(LOG_ERR, FNAME, "open a routing socket: %s",
		    strerror(errno));
        exit(1);
	}

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = PF_INET6;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;
	error = getaddrinfo(DH6ADDR_ALLAGENT, DH6PORT_UPSTREAM, &hints, &res);
	if (error) {
		dprintf(LOG_ERR, FNAME, "getaddrinfo: %s",
		    gai_strerror(error));
		exit(1);
	}
	memcpy(&sa6_allagent_storage, res->ai_addr, res->ai_addrlen);
	sa6_allagent = (const struct sockaddr_in6 *)&sa6_allagent_storage;
	freeaddrinfo(res);

	/* set up control socket */
	if (ctlkey == NULL)
		dprintf(LOG_NOTICE, FNAME, "skip opening control port");
	else if (dhcp6_ctl_init(ctladdr, ctlport,
	    DHCP6CTL_DEF_COMMANDQUEUELEN, &ctlsock)) {
		dprintf(LOG_ERR, FNAME,
		    "failed to initialize control channel");
		exit(1);
	}

	if (signal(SIGHUP, client6_signal) == SIG_ERR) {
		dprintf(LOG_WARNING, FNAME, "failed to set signal: %s",
		    strerror(errno));
		exit(1);
	}
	if (signal(SIGTERM, client6_signal) == SIG_ERR) {
		dprintf(LOG_WARNING, FNAME, "failed to set signal: %s",
		    strerror(errno));
		exit(1);
	}

    if (signal(SIGEXEXITDHCP6C, client6_signal) == SIG_ERR) {
        dprintf(LOG_WARNING, FNAME, "failed to set signal: %s",
            strerror(errno));
        exit(1);
    }
    if (signal(SIGUSR1, client6_signal) == SIG_ERR)
    {
        dprintf(LOG_ERR, FNAME, "signal usr1 for log ra error");
        return ;
    }
#ifdef DHCP6_DECLIEN
    /* BEGIN: Added by z67728, 2010/1/16 PN:AU4D02303 comment : Add a signal for dhcp6c declien packet. */    
    if (signal(SIGUSR2, client6_addrduplicatesignal) == SIG_ERR)
    {
        dprintf(LOG_ERR, FNAME, "signal usr2 for log ra error");
        return ;
    }
    /* END:   Added by z67728, 2010/1/16 */
#endif
    if (signal(SIGQUIT, ExitProcess) == SIG_ERR)
    {
        dprintf(LOG_ERR, FNAME, "signal quit for log ra error");
        return ;
    }

#if 0
    if (signal(SIGSEGV, sigsegvprocess) == SIG_ERR)
    {
        dprintf(LOG_ERR, FNAME, "signal sigsegv error");
        return ;
    }
#endif
}

int
client6_start(ifp)
	struct dhcp6_if *ifp;
{
	struct dhcp6_event *ev;
    struct dhcp6_event *ev_next;

	/* make sure that the interface does not have a timer */
	if (ifp->timer != NULL) {
		dprintf(LOG_DEBUG, FNAME,
		    "removed existing timer on %s", ifp->ifname);
		dhcp6_remove_timer(&ifp->timer);
	}

#if 0
    /* 插拔网线的时候solicit消息比release快 */    
    if (g_iDhcpStartFlag)
    {
        loginfo(FUNC, "begin to mangle release event...");
        for (ev = TAILQ_FIRST(&ifp->event_list); ev; ev = ev_next)
        {
            ev_next = TAILQ_NEXT(ev, link);
            /* 马上发送release消息 */
            if (DHCP6S_RELEASE == ev->state)
            {
                loginfo(FUNC, "state [RELEASE] send release packet ......");
                client6_send(ev);
                dhcp6_remove_event(ev);
            }
        }
    }
#endif

	/* create an event for the initial delay */
	if ((ev = dhcp6_create_event(ifp, DHCP6S_INIT)) == NULL) {
		dprintf(LOG_NOTICE, FNAME, "failed to create an event");
		return (-1);
	}
	TAILQ_INSERT_TAIL(&ifp->event_list, ev, link);

	if ((ev->authparam = new_authparam(ifp->authproto,
	    ifp->authalgorithm, ifp->authrdm)) == NULL) {
		dprintf(LOG_WARNING, FNAME, "failed to allocate "
		    "authentication parameters");
		dhcp6_remove_event(ev);
		return (-1);
	}

	if ((ev->timer = dhcp6_add_timer(client6_timo, ev)) == NULL) {
		dprintf(LOG_NOTICE, FNAME, "failed to add a timer for %s",
		    ifp->ifname);
		dhcp6_remove_event(ev);
		return (-1);
	}
	dhcp6_reset_timer(ev);

	return (0);
}

static void
client6_startall(isrestart)
	int isrestart;
{
	struct dhcp6_if *ifp;

	for (ifp = dhcp6_if; ifp; ifp = ifp->next) {
		if (isrestart &&ifreset(ifp)) {
            loginfo(FUNC, "failed to reset %s", ifp->ifname);
			dprintf(LOG_NOTICE, FNAME, "failed to reset %s",
			    ifp->ifname);
			continue; /* XXX: try to recover? */
		}
		if (client6_start(ifp))
		{
            loginfo(FUNC, "client_start error");
			exit(1); /* initialization failure.  we give up. */
		}
	}
}

static void
free_resources(freeifp)
	struct dhcp6_if *freeifp;
{
	struct dhcp6_if *ifp;

	for (ifp = dhcp6_if; ifp; ifp = ifp->next) {
		struct dhcp6_event *ev, *ev_next;

		if (freeifp != NULL && freeifp != ifp)
			continue;

		/* release all IAs as well as send RELEASE message(s) */
		release_all_ia(ifp);

		/*
		 * Cancel all outstanding events for each interface except
		 * ones being released.
		 */
		for (ev = TAILQ_FIRST(&ifp->event_list); ev; ev = ev_next) {
			ev_next = TAILQ_NEXT(ev, link);

			if (ev->state == DHCP6S_RELEASE)
				continue; /* keep it for now */

			dhcp6_remove_event(ev);
		}
	}


#if 0
    /* 清除掉所有的DNS信息 */
    CleanAllDHCPInfo();
#endif

}

static void
check_exit()
{
	struct dhcp6_if *ifp;

	if (!exit_ok)
		return;

	for (ifp = dhcp6_if; ifp; ifp = ifp->next) {
		/*
		 * Check if we have an outstanding event.  If we do, we cannot
		 * exit for now.
		 */
		if (!TAILQ_EMPTY(&ifp->event_list))
			return;
	}

	/* We have no existing event.  Do exit. */
	dprintf(LOG_INFO, FNAME, "exiting");

	exit(0);
}

static void
process_signals()
{
	if ((sig_flags & SIGF_TERM)) {
        exit_addrMonitor = 1;
        if (send(g_lSignalPipev6[1], &sig_flags, sizeof(sig_flags), MSG_DONTWAIT) < 0) 
        {
    		loginfo(FUNC, "send failed\n");
    	}
		free_resources(NULL);
		//unlink(pid_file);
		check_exit();
	}
	if ((sig_flags & SIGF_HUP)) {
		dprintf(LOG_INFO, FNAME, "restarting");
		free_resources(NULL);
		client6_startall(1);
	}
    if((sig_flags & SIGF_EXITDHCP6C))
    {
        exit_ok = 1;
        check_exit();
    }
	sig_flags = 0;
}

int Dhcp6cGetIndex(void)
{
    int iIfindex;
    struct dhcp6_if *ifp = NULL;
    
	for (ifp = dhcp6_if; ifp; ifp = ifp->next) 
    {
        iIfindex = if_nametoindex(ifp->ifname);
        if (0 < iIfindex)
        {
            return iIfindex;
        }
	}

    return (-1);
}


int Dhcp6cUpdateInfo(void)
{
    int iCnt = 0;
    struct dhcp6_if *ifp = NULL;

    FUNCBEGIN();
    
    /*BEGIN 2032203980  m00182044 2012-06-10 modify*/
    for (ifp = dhcp6_if; ifp; ifp = ifp->next) 
    {
        iCnt += WriteAllDHCPInfo(ifp->ifid, ifp->ifname);
        BSP_SYS_WakeupMonitorTask(ifp->ifid);
    }
    /*END 2032203980  m00182044 2012-06-10 modify*/

    FUNCEND();
    
    return iCnt;
}

/* 启动DHCP6C客户端 */
int Dhcp6cStart(int iIfindex, int iIANAFlag, int iIAPDFlag, int iDsliteFlag)
{
    char acIfname[IFNAMSIZ] = {0};
    struct dhcp6_if *ifp = NULL;

    FUNCBEGIN();
    
    if (NULL == if_indextoname(iIfindex, acIfname))
    {
        loginfo(FUNC, "get name from index [%d] error", iIfindex);
        return (-1);
    }

#if 0
    ifp = find_ifconfbyname(acIfname);
    if (ifp)
    {
        log(FUNC, "begin to release resource.........");
        free_resources(ifp);
    }
#endif

    conffile = WriteDHCP6CCfg(acIfname, iIANAFlag, iIAPDFlag, iDsliteFlag);

    if ((conffile) && (0 != cfparse(conffile)))
    {
        loginfo(FUNC, "notice.... parse config [%s] error", conffile);
        return (-1);
    }

    loginfo(FUNC, "config file: [%s]", conffile);

    client6_startall(1);

    g_iDhcpStartFlag = 1;

    FUNCEND();

    return (0);
}

int Dhcp6cStop(int iIfindex, int iFlags)
{
    char acIfname[IFNAMSIZ] = {0};
    struct dhcp6_if *ifp = NULL;

    FUNCBEGIN();
    
    if (NULL == if_indextoname(iIfindex, acIfname))
    {
        loginfo(FUNC, "get name from index [%d] error", iIfindex);
        return (-1);
    }

    ifp = find_ifconfbyname(acIfname);
    if (ifp)
    {
        loginfo(FUNC, "begin to release for dev [%s]", ifp->ifname);
        
        free_resources(ifp);
    }

    CleanAllDHCPInfo();
    WriteAllDHCPInfo(iIfindex, NULL);

    g_iDhcpStartFlag = 0;
#ifdef DHCP6_DECLIEN
    /* BEGIN: Added by z67728, 2010/2/1 free chain .*/
    if ( DECLINE_SWITCH_ENABLE == g_ldeclineswitch )
    {        
        client6_freechain(&g_pstifinfoheader);
    }
    /* END:   Added by z67728, 2010/2/1 */
#endif
    FUNCEND();

    return (0);
}
int StartDhcp6c(int iIfindex, int iIANAFlag, int iIAPDFlag, int iDsliteFlag)
{
#if (defined FEATURE_HUAWEI_MBB_WIFIOFFLOAD_IPV6 ) || (defined MBB_FEATURE_PPP6C) || (defined MBB_FEATURE_CRADLE_IPV6)        
    return Dhcp6cStart(iIfindex, iIANAFlag, iIAPDFlag, iDsliteFlag);
#else
    return 0;
#endif 
}

int StopDhcp6c(int iIfindex, int iFlags)
{
#if (defined FEATURE_HUAWEI_MBB_WIFIOFFLOAD_IPV6 ) || (defined MBB_FEATURE_PPP6C) || (defined MBB_FEATURE_CRADLE_IPV6)    
    return Dhcp6cStop(iIfindex, iFlags);
#else
    return 0;
#endif 
}



static void
client6_mainloop()
{
	struct timeval *w;
    static struct timeval t;
	int ret, maxsock;
	fd_set r;

	while(1) {

        //loginfo(FUNC, "main loop here........");
        
		if (sig_flags)
		{
            if (sig_flags & SIGF_USR1)
            {
                logged = !logged;
                if (logged)
                {                    
                    setloglevel(2);
                }
                else
                {
                    setloglevel(0);
                }
                sig_flags = 0;
            }
            else
            {
                process_signals();
            }
		}

        //loginfo(FUNC, "Check the timer.....");
        
		w = dhcp6_check_timer();

        /* 注意这里: dhcp6_check_timer 中如果没有定时器返回那么返回NULL从而导致无限等待但是这样子是不行的所以这里改成
            * 过一秒钟轮询一次
            */
        
        t.tv_sec = 1;
        t.tv_usec = 0;
        /* 根据定时器检查返回结果处理t，如果定时器检查结果返回了定时器，那么此时
            * 需要根据返回的结果来决定t的值，如果返回的时间大于1秒钟，那么就将定时器时间设置为t
            * 否则，如果返回的时间小于1秒钟，那么就以返回的定时器为标准
            */
        if (w)
        {
            if (0 >= w->tv_sec)
            {
                t.tv_sec = 0;
                t.tv_usec = w->tv_usec;
            }
        }
        w = &t;

        //loginfo(FUNC, "select will wait: %d second", w->tv_sec);

#if 0
        if (!g_iDhcpStartFlag)
        {
            log(FUNC, "dhcp not enable so will still keep sleep......");
            //sleep(1);
            //continue;
        }
#endif

		FD_ZERO(&r);
		FD_SET(sock, &r);
		maxsock = sock;
		if (ctlsock >= 0) {
			FD_SET(ctlsock, &r);
			maxsock = (sock > ctlsock) ? sock : ctlsock;
			(void)dhcp6_ctl_setreadfds(&r, &maxsock);
		}

		ret = select(maxsock + 1, &r, NULL, NULL, w);

		switch (ret) {
		case -1:
			if (errno != EINTR) {
				dprintf(LOG_ERR, FNAME, "select: %s",
				    strerror(errno));
				//exit(1);
			}
			continue;
		case 0:	/* timeout */
			break;	/* dhcp6_check_timer() will treat the case */
		default:
			break;
		}
		if (FD_ISSET(sock, &r))
		{
            loginfo(FUNC, "good, get packet now...");
			client6_recv(g_iDhcpStartFlag);
		}
		if (ctlsock >= 0) {
			if (FD_ISSET(ctlsock, &r)) {
				(void)dhcp6_ctl_acceptcommand(ctlsock,
				    client6_do_ctlcommand);
			}
			(void)dhcp6_ctl_readcommand(&r);
		}

        /*BEGIN 2032203980  m00182044 2012-06-10 modify*/
        if (!exit_ok)
        {
           Dhcp6cUpdateInfo() ;
        }
        /*END 2032203980  m00182044 2012-06-10 modify*/

        if (exit_ok)
        {
            exit(0);
        }
	}
}

static inline int
get_val32(bpp, lenp, valp)
	char **bpp;
	int *lenp;
	u_int32_t *valp;
{
	char *bp = *bpp;
	int len = *lenp;
	u_int32_t i32;

	if (len < sizeof(*valp))
		return (-1);

	memcpy(&i32, bp, sizeof(i32));
	*valp = ntohl(i32);

	*bpp = bp + sizeof(*valp);
	*lenp = len - sizeof(*valp);

	return (0);
}

static inline int
get_ifname(bpp, lenp, ifbuf, ifbuflen)
	char **bpp;
	int *lenp;
	char *ifbuf;
	int ifbuflen;
{
	char *bp = *bpp;
	int len = *lenp, ifnamelen;
	u_int32_t i32;

	if (get_val32(bpp, lenp, &i32))
		return (-1);
	ifnamelen = (int)i32;

	if (*lenp < ifnamelen || ifnamelen > ifbuflen)
		return (-1);

	memset(ifbuf, 0, sizeof(ifbuf));
	memcpy(ifbuf, *bpp, ifnamelen);
	if (ifbuf[ifbuflen - 1] != '\0')
		return (-1);	/* not null terminated */

	*bpp = bp + sizeof(i32) + ifnamelen;
	*lenp = len - (sizeof(i32) + ifnamelen);

	return (0);
}

static int
client6_do_ctlcommand(buf, len)
	char *buf;
	ssize_t len;
{
	struct dhcp6ctl *ctlhead;
	u_int16_t command, version;
	u_int32_t p32, ts, ts0;
	int commandlen;
	char *bp;
	char ifname[IFNAMSIZ];
	time_t now;

	memset(ifname, 0, sizeof(ifname));

	ctlhead = (struct dhcp6ctl *)buf;

	command = ntohs(ctlhead->command);
	commandlen = (int)(ntohs(ctlhead->len));
	version = ntohs(ctlhead->version);
	if (len != sizeof(struct dhcp6ctl) + commandlen) {
		dprintf(LOG_ERR, FNAME,
		    "assumption failure: command length mismatch");
		return (DHCP6CTL_R_FAILURE);
	}

	/* replay protection and message authentication */
    /* BEGIN 3082101267 y00188255 2013-8-24  Modified */
	if ((now = (time_t)dhcp_get_cur_time()) < 0) {
		dprintf(LOG_ERR, FNAME, "failed to get current time: %s",
		    strerror(errno));
		return (DHCP6CTL_R_FAILURE);
	}
    /* END 3082101267 y00188255 2013-8-24  Modified */
	ts0 = (u_int32_t)now;
	ts = ntohl(ctlhead->timestamp);
	if (ts + CTLSKEW < ts0 || (ts - CTLSKEW) > ts0) {
		dprintf(LOG_INFO, FNAME, "timestamp is out of range");
		return (DHCP6CTL_R_FAILURE);
	}

	if (ctlkey == NULL) {	/* should not happen!! */
		dprintf(LOG_ERR, FNAME, "no secret key for control channel");
		return (DHCP6CTL_R_FAILURE);
	}
	if (dhcp6_verify_mac(buf, len, DHCP6CTL_AUTHPROTO_UNDEF,
	    DHCP6CTL_AUTHALG_HMACMD5, sizeof(*ctlhead), ctlkey) != 0) {
		dprintf(LOG_INFO, FNAME, "authentication failure");
		return (DHCP6CTL_R_FAILURE);
	}

	bp = buf + sizeof(*ctlhead) + ctldigestlen;
	commandlen -= ctldigestlen;

	if (version > DHCP6CTL_VERSION) {
		dprintf(LOG_INFO, FNAME, "unsupported version: %d", version);
		return (DHCP6CTL_R_FAILURE);
	}

	switch (command) {
	case DHCP6CTL_COMMAND_RELOAD:
		if (commandlen != 0) {
			dprintf(LOG_INFO, FNAME, "invalid command length "
			    "for reload: %d", commandlen);
			return (DHCP6CTL_R_DONE);
		}
		client6_reload();
		break;
	case DHCP6CTL_COMMAND_START:
		if (get_val32(&bp, &commandlen, &p32))
			return (DHCP6CTL_R_FAILURE);
		switch (p32) {
		case DHCP6CTL_INTERFACE:
			if (get_ifname(&bp, &commandlen, ifname,
			    sizeof(ifname))) {
				return (DHCP6CTL_R_FAILURE);
			}
			if (client6_ifctl(ifname, DHCP6CTL_COMMAND_START))
				return (DHCP6CTL_R_FAILURE);
			break;
		default:
			dprintf(LOG_INFO, FNAME,
			    "unknown start target: %ul", p32);
			return (DHCP6CTL_R_FAILURE);
		}
		break;
	case DHCP6CTL_COMMAND_STOP:
		if (commandlen == 0) {
			exit_ok = 1;
			free_resources(NULL);
			//unlink(pid_file);
			check_exit();
		} else {
			if (get_val32(&bp, &commandlen, &p32))
				return (DHCP6CTL_R_FAILURE);

			switch (p32) {
			case DHCP6CTL_INTERFACE:
				if (get_ifname(&bp, &commandlen, ifname,
				    sizeof(ifname))) {
					return (DHCP6CTL_R_FAILURE);
				}
				if (client6_ifctl(ifname,
				    DHCP6CTL_COMMAND_STOP)) {
					return (DHCP6CTL_R_FAILURE);
				}
				break;
			default:
				dprintf(LOG_INFO, FNAME,
				    "unknown start target: %ul", p32);
				return (DHCP6CTL_R_FAILURE);
			}
		}
		break;
	default:
		dprintf(LOG_INFO, FNAME,
		    "unknown control command: %d (len=%d)",
		    (int)command, commandlen);
		return (DHCP6CTL_R_FAILURE);
	}

  	return (DHCP6CTL_R_DONE);
}

static void
client6_reload()
{
	/* reload the configuration file */
	if (cfparse(conffile) != 0) {
		dprintf(LOG_WARNING, FNAME,
		    "failed to reload configuration file");
		return;
	}

	dprintf(LOG_NOTICE, FNAME, "client reloaded");

	return;
}

static int
client6_ifctl(ifname, command)
	char *ifname;
	u_int16_t command;
{
	struct dhcp6_if *ifp;

	if ((ifp = find_ifconfbyname(ifname)) == NULL) {
		dprintf(LOG_INFO, FNAME,
		    "failed to find interface configuration for %s",
		    ifname);
		return (-1);
	}

	dprintf(LOG_DEBUG, FNAME, "%s interface %s",
	    command == DHCP6CTL_COMMAND_START ? "start" : "stop", ifname);

	switch(command) {
	case DHCP6CTL_COMMAND_START:
		free_resources(ifp);
		if (client6_start(ifp)) {
			dprintf(LOG_NOTICE, FNAME, "failed to restart %s",
			    ifname);
			return (-1);
		}
		break;
	case DHCP6CTL_COMMAND_STOP:
		free_resources(ifp);
		if (ifp->timer != NULL) {
			dprintf(LOG_DEBUG, FNAME,
			    "removed existing timer on %s", ifp->ifname);
			dhcp6_remove_timer(&ifp->timer);
		}
		break;
	default:		/* impossible case, should be a bug */
		dprintf(LOG_ERR, FNAME, "unknown command: %d", (int)command);
		break;
	}

	return (0);
}

static struct dhcp6_timer *
client6_expire_refreshtime(arg)
	void *arg;
{
	struct dhcp6_if *ifp = arg;

	dprintf(LOG_DEBUG, FNAME,
	    "information refresh time on %s expired", ifp->ifname);

	dhcp6_remove_timer(&ifp->timer);
	client6_start(ifp);

	return (NULL);
}

struct dhcp6_timer *
client6_timo(arg)
	void *arg;
{
	struct dhcp6_event *ev = (struct dhcp6_event *)arg;
	struct dhcp6_if *ifp;
	int state = ev->state;
	static int iCnt = 0;
	
	ifp = ev->ifp;
	ev->timeouts++;

	/*
	 * Unless MRC is zero, the message exchange fails once the client has
	 * transmitted the message MRC times.
	 * [RFC3315 14.]
	 */
	if (ev->max_retrans_cnt && ev->timeouts >= ev->max_retrans_cnt) {
		dprintf(LOG_INFO, FNAME, "no responses were received");
		dhcp6_remove_event(ev);

		if (state == DHCP6S_RELEASE)
		{
            loginfo(FUNC, "state is DHCP6S_RELEASE");
			check_exit();
		}
        
		return (NULL);
	}

	switch(ev->state) {
	case DHCP6S_INIT:
/* start of by d00107688 AU4D02431 2010-03-15  IPOE-PD方式，网关获取到前缀后插拔网线，抓包发现网关先申请前缀信息，然后才发出上一次的release报文 */
        {
            struct dhcp6_event *pstEv = NULL;
            struct dhcp6_event *pstEvNext = NULL;
			int iFlag = 0;

            for (pstEv = TAILQ_FIRST(&ifp->event_list); pstEv; pstEv = pstEvNext)
            {
                pstEvNext = TAILQ_NEXT(pstEv, link);
                /* 马上发送release消息 */
                if (DHCP6S_RELEASE == pstEv->state)
                {
					iFlag = 1;
                    //fprintf(stderr, "%s %d so begin to send release packet\r\n", __FUNCTION__, __LINE__);
                    dprintf(LOG_ERR, FNAME, "%s %d so begin to send release packet\r\n", __FUNCTION__, __LINE__);
                    client6_send(pstEv);
                    iCnt += 1;
					if (4 <= iCnt)
					{
                        //fprintf(stderr, "send %d times\r\n", iCnt);
                        dprintf(LOG_ERR, FNAME, "send %d times\r\n", iCnt);
						pstEv->timeouts = pstEv->max_retrans_cnt;
                        struct timeval t;
                        t.tv_sec = 0;
                        t.tv_usec = 0;
                        dhcp6_set_timer(&t, pstEv->timer);
                    }
				}
            }
            
            if (iFlag)
            {
                dhcp6_reset_timer(ev);
                return (ev->timer);
            }
			
        }
        
        //fprintf(stderr, "now, begin to send solicite......\r\n\r\n");  
            dprintf(LOG_ERR, FNAME, "now, begin to send solicite......\r\n\r\n");
		iCnt = 0;
/* end of by d00107688 AU4D02431 2010-03-15  IPOE-PD方式，网关获取到前缀后插拔网线，抓包发现网关先申请前缀信息，然后才发出上一次的release报文 */
		
		ev->timeouts = 0; /* indicate to generate a new XID. */
		if ((ifp->send_flags & DHCIFF_INFO_ONLY) || infreq_mode)
			ev->state = DHCP6S_INFOREQ;
		else {
			ev->state = DHCP6S_SOLICIT;
			if (construct_confdata(ifp, ev)) {
				dprintf(LOG_ERR, FNAME, "can't send solicit");
				exit(1); /* XXX */
			}
		}
		dhcp6_set_timeoparam(ev); /* XXX */
		/* fall through */
	case DHCP6S_REQUEST:
	case DHCP6S_RELEASE:
	case DHCP6S_INFOREQ:
		client6_send(ev);
		break;
	case DHCP6S_RENEW:
	case DHCP6S_REBIND:
		if (!TAILQ_EMPTY(&ev->data_list))
			client6_send(ev);
		else {
			dprintf(LOG_INFO, FNAME,
			    "all information to be updated was canceled");   
            loginfo(FUNC, "rebind over...no ia exist... so clean all dhcp info\r\n\r\n\r\n\r\n");
            CleanAllDHCPInfo();  
            
            //WriteAllDHCPInfo(ifp->ifid, NULL);
			dhcp6_remove_event(ev);
			return (NULL);
		}
		break;
	case DHCP6S_SOLICIT:
		if (ev->servers) {
			/*
			 * Send a Request to the best server.
			 * Note that when we set Rapid-commit in Solicit,
			 * but a direct Reply has been delayed (very much),
			 * the transition to DHCP6S_REQUEST (and the change of
			 * transaction ID) will invalidate the reply even if it
			 * ever arrives.
			 */
			ev->current_server = select_server(ev);
			if (ev->current_server == NULL) {
				/* this should not happen! */
				dprintf(LOG_NOTICE, FNAME,
				    "can't find a server");
				exit(1); /* XXX */
			}
			if (duidcpy(&ev->serverid,
			    &ev->current_server->optinfo.serverID)) {
				dprintf(LOG_NOTICE, FNAME,
				    "can't copy server ID");
				return (NULL); /* XXX: better recovery? */
			}
			ev->timeouts = 0;
			ev->state = DHCP6S_REQUEST;
			dhcp6_set_timeoparam(ev);

			if (ev->authparam != NULL)
				free(ev->authparam);
			ev->authparam = ev->current_server->authparam;
			ev->current_server->authparam = NULL;

			if (construct_reqdata(ifp,
			    &ev->current_server->optinfo, ev)) {
				dprintf(LOG_NOTICE, FNAME,
				    "failed to construct request data");
				break;
			}
		}
        loginfo(FUNC, "now, begin to send packet.....");
		client6_send(ev);
		break;
	}

	dhcp6_reset_timer(ev);

	return (ev->timer);
}

static int
construct_confdata(ifp, ev)
	struct dhcp6_if *ifp;
	struct dhcp6_event *ev;
{
	struct ia_conf *iac;
	struct dhcp6_eventdata *evd = NULL;
	struct dhcp6_list *ial = NULL, pl;
	struct dhcp6_ia iaparam;

	TAILQ_INIT(&pl);	/* for safety */

	for (iac = TAILQ_FIRST(&ifp->iaconf_list); iac;
	    iac = TAILQ_NEXT(iac, link)) {
		/* ignore IA config currently used */
		if (!TAILQ_EMPTY(&iac->iadata))
			continue;

		evd = NULL;
		if ((evd = malloc(sizeof(*evd))) == NULL) {
			dprintf(LOG_NOTICE, FNAME,
			    "failed to create a new event data");
			goto fail;
		}
		memset(evd, 0, sizeof(evd));

		memset(&iaparam, 0, sizeof(iaparam));
		iaparam.iaid = iac->iaid;
		switch (iac->type) {
		case IATYPE_PD:
			ial = NULL;
			if ((ial = malloc(sizeof(*ial))) == NULL)
				goto fail;
			TAILQ_INIT(ial);

			TAILQ_INIT(&pl);
			dhcp6_copy_list(&pl,
			    &((struct iapd_conf *)iac)->iapd_prefix_list);
			if (dhcp6_add_listval(ial, DHCP6_LISTVAL_IAPD,
			    &iaparam, &pl) == NULL) {
				goto fail;
			}
			dhcp6_clear_list(&pl);

			evd->type = DHCP6_EVDATA_IAPD;
			evd->data = ial;
			evd->event = ev;
			evd->destructor = destruct_iadata;
			TAILQ_INSERT_TAIL(&ev->data_list, evd, link);
			break;
		case IATYPE_NA:
			ial = NULL;
			if ((ial = malloc(sizeof(*ial))) == NULL)
				goto fail;
			TAILQ_INIT(ial);

			TAILQ_INIT(&pl);
			dhcp6_copy_list(&pl,
			    &((struct iana_conf *)iac)->iana_address_list);
			if (dhcp6_add_listval(ial, DHCP6_LISTVAL_IANA,
			    &iaparam, &pl) == NULL) {
				goto fail;
			}
			dhcp6_clear_list(&pl);

			evd->type = DHCP6_EVDATA_IANA;
			evd->data = ial;
			evd->event = ev;
			evd->destructor = destruct_iadata;
			TAILQ_INSERT_TAIL(&ev->data_list, evd, link);
			break;
		default:
			dprintf(LOG_ERR, FNAME, "internal error");
			exit(1);
		}
	}

	return (0);

  fail:
	if (evd)
		free(evd);
	if (ial)
		free(ial);
	dhcp6_remove_event(ev);	/* XXX */
	
	return (-1);
}

static int
construct_reqdata(ifp, optinfo, ev)
	struct dhcp6_if *ifp;
	struct dhcp6_optinfo *optinfo;
	struct dhcp6_event *ev;
{
	struct ia_conf *iac;
	struct dhcp6_eventdata *evd = NULL;
	struct dhcp6_list *ial = NULL;
	struct dhcp6_ia iaparam;

	/* discard previous event data */
	dhcp6_remove_evdata(ev);

	if (optinfo == NULL)
		return (0);

	for (iac = TAILQ_FIRST(&ifp->iaconf_list); iac;
	    iac = TAILQ_NEXT(iac, link)) {
		struct dhcp6_listval *v;

		/* ignore IA config currently used */
		if (!TAILQ_EMPTY(&iac->iadata))
			continue;

		memset(&iaparam, 0, sizeof(iaparam));
		iaparam.iaid = iac->iaid;

		ial = NULL;
		evd = NULL;

		switch (iac->type) {
		case IATYPE_PD:
			if ((v = dhcp6_find_listval(&optinfo->iapd_list,
			    DHCP6_LISTVAL_IAPD, &iaparam, 0)) == NULL)
				continue;

			if ((ial = malloc(sizeof(*ial))) == NULL)
				goto fail;

			TAILQ_INIT(ial);
			if (dhcp6_add_listval(ial, DHCP6_LISTVAL_IAPD,
			    &iaparam, &v->sublist) == NULL) {
				goto fail;
			}

			if ((evd = malloc(sizeof(*evd))) == NULL)
				goto fail;
			memset(evd, 0, sizeof(*evd));
			evd->type = DHCP6_EVDATA_IAPD;
			evd->data = ial;
			evd->event = ev;
			evd->destructor = destruct_iadata;
			TAILQ_INSERT_TAIL(&ev->data_list, evd, link);
			break;
		case IATYPE_NA:
			if ((v = dhcp6_find_listval(&optinfo->iana_list,
			    DHCP6_LISTVAL_IANA, &iaparam, 0)) == NULL)
				continue;

			if ((ial = malloc(sizeof(*ial))) == NULL)
				goto fail;

			TAILQ_INIT(ial);
			if (dhcp6_add_listval(ial, DHCP6_LISTVAL_IANA,
			    &iaparam, &v->sublist) == NULL) {
				goto fail;
			}

			if ((evd = malloc(sizeof(*evd))) == NULL)
				goto fail;
			memset(evd, 0, sizeof(*evd));
			evd->type = DHCP6_EVDATA_IANA;
			evd->data = ial;
			evd->event = ev;
			evd->destructor = destruct_iadata;
			TAILQ_INSERT_TAIL(&ev->data_list, evd, link);
			break;
		default:
			dprintf(LOG_ERR, FNAME, "internal error");
			exit(1);
		}
	}

	return (0);

  fail:
	if (evd)
		free(evd);
	if (ial)
		free(ial);
	dhcp6_remove_event(ev);	/* XXX */
	
	return (-1);
}

static void
destruct_iadata(evd)
	struct dhcp6_eventdata *evd;
{
	struct dhcp6_list *ial;

	if (evd->type != DHCP6_EVDATA_IAPD && evd->type != DHCP6_EVDATA_IANA) {
		dprintf(LOG_ERR, FNAME, "assumption failure %d", evd->type);
		exit(1);
	}

	ial = (struct dhcp6_list *)evd->data;
	dhcp6_clear_list(ial);
	free(ial);
}

static struct dhcp6_serverinfo *
select_server(ev)
	struct dhcp6_event *ev;
{
	struct dhcp6_serverinfo *s;

	/*
	 * pick the best server according to RFC3315 Section 17.1.3.
	 * XXX: we currently just choose the one that is active and has the
	 * highest preference.
	 */
	for (s = ev->servers; s; s = s->next) {
		if (s->active) {
			dprintf(LOG_DEBUG, FNAME, "picked a server (ID: %s)",
			    duidstr(&s->optinfo.serverID));
			return (s);
		}
	}

	return (NULL);
}

static void
client6_signal(sig)
	int sig;
{

	switch (sig) {
	case SIGTERM:
		sig_flags |= SIGF_TERM;
		break;
	case SIGHUP:
		sig_flags |= SIGF_HUP;
		break;
    case SIGUSR1:
        sig_flags |= SIGF_USR1;
        break;
    case SIGEXEXITDHCP6C:
        sig_flags |= SIGF_EXITDHCP6C;
        break;
	}
}

void
client6_send(ev)
	struct dhcp6_event *ev;
{
	struct dhcp6_if *ifp;
	char buf[BUFSIZ];
	struct sockaddr_in6 dst;
	struct dhcp6 *dh6;
	struct dhcp6_optinfo optinfo;
	ssize_t optlen, len;
	struct dhcp6_eventdata *evd;

    /* BEGIN 3082101267 y00188255 2013/8/24  Added */
    struct timespec ts;
    /* END 3082101267 y00188255 2013-8-24  Added */

	ifp = ev->ifp;

	dh6 = (struct dhcp6 *)buf;
	memset(dh6, 0, sizeof(*dh6));

	switch(ev->state) {
	case DHCP6S_SOLICIT:
		dh6->dh6_msgtype = DH6_SOLICIT;
		break;
	case DHCP6S_REQUEST:
		dh6->dh6_msgtype = DH6_REQUEST;
		break;
	case DHCP6S_RENEW:
		dh6->dh6_msgtype = DH6_RENEW;
		break;
	case DHCP6S_REBIND:
		dh6->dh6_msgtype = DH6_REBIND;
		break;
	case DHCP6S_RELEASE:
		dh6->dh6_msgtype = DH6_RELEASE;
		break;
	case DHCP6S_INFOREQ:
		dh6->dh6_msgtype = DH6_INFORM_REQ;
		break;
	default:
		dprintf(LOG_ERR, FNAME, "unexpected state");
		exit(1);	/* XXX */
	}

	if (ev->timeouts == 0) {
		/*
		 * A client SHOULD generate a random number that cannot easily
		 * be guessed or predicted to use as the transaction ID for
		 * each new message it sends.
		 *
		 * A client MUST leave the transaction-ID unchanged in
		 * retransmissions of a message. [RFC3315 15.1]
		 */
#ifdef HAVE_ARC4RANDOM
		ev->xid = arc4random() & DH6_XIDMASK;
#else
		ev->xid = random() & DH6_XIDMASK;
#endif
		dprintf(LOG_DEBUG, FNAME, "a new XID (%x) is generated",
		    ev->xid);
	}
	dh6->dh6_xid &= ~ntohl(DH6_XIDMASK);
	dh6->dh6_xid |= htonl(ev->xid);
	len = sizeof(*dh6);

	/*
	 * construct options
	 */
	dhcp6_init_options(&optinfo);

	/* server ID */
	switch (ev->state) {
	case DHCP6S_REQUEST:
	case DHCP6S_RENEW:
	case DHCP6S_RELEASE:
		if (duidcpy(&optinfo.serverID, &ev->serverid)) {
			dprintf(LOG_ERR, FNAME, "failed to copy server ID");
			goto end;
		}
		break;
	}

	/* client ID */
	if (duidcpy(&optinfo.clientID, &client_duid)) {
		dprintf(LOG_ERR, FNAME, "failed to copy client ID");
		goto end;
	}

/* start of by d00107688 for support vendor class */
    if (VendorClassCopy(&optinfo.stVC, &g_stVC))  
    {
        dprintf(LOG_ERR, FNAME, "failed to copy vendor class");
    }
/* end of by d00107688 for support vendor class */

	/* rapid commit (in Solicit only) */
	if (ev->state == DHCP6S_SOLICIT &&
	    (ifp->send_flags & DHCIFF_RAPID_COMMIT)) {
		optinfo.rapidcommit = 1;
	}

	/* elapsed time */
    /* BEGIN 3082101267 y00188255 2013-8-24  Modified */
	if (ev->timeouts == 0) {
		clock_gettime(CLOCK_MONOTONIC, &ts);
    	ev->tv_start.tv_sec = ts.tv_sec;
        ev->tv_start.tv_usec = ts.tv_nsec / 1000;
		optinfo.elapsed_time = 0;
	} else {
		struct timeval now, tv_diff;
		long et;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        now.tv_sec = ts.tv_sec;
        now.tv_usec = ts.tv_nsec / 1000;
		tv_sub(&now, &ev->tv_start, &tv_diff);

		/*
		 * The client uses the value 0xffff to represent any elapsed
		 * time values greater than the largest time value that can be
		 * represented in the Elapsed Time option.
		 * [RFC3315 22.9.]
		 */
		if (tv_diff.tv_sec >= (MAX_ELAPSED_TIME / 100) + 1) {
			/*
			 * Perhaps we are nervous too much, but without this
			 * additional check, we would see an overflow in 248
			 * days (of no responses). 
			 */
			et = MAX_ELAPSED_TIME;
		} else {
			et = tv_diff.tv_sec * 100 + tv_diff.tv_usec / 10000;
			if (et >= MAX_ELAPSED_TIME)
				et = MAX_ELAPSED_TIME;
		}
		optinfo.elapsed_time = (int32_t)et;
	}
    /* END 3082101267 y00188255 2013-8-24  Modified */
	/* option request options */
	if (ev->state != DHCP6S_RELEASE &&
	    dhcp6_copy_list(&optinfo.reqopt_list, &ifp->reqopt_list)) {
		dprintf(LOG_ERR, FNAME, "failed to copy requested options");
		goto end;
	}

	/* configuration information specified as event data */
	for (evd = TAILQ_FIRST(&ev->data_list); evd;
	     evd = TAILQ_NEXT(evd, link)) {
		switch(evd->type) {
		case DHCP6_EVDATA_IAPD:
			if (dhcp6_copy_list(&optinfo.iapd_list,
			    (struct dhcp6_list *)evd->data)) {
				dprintf(LOG_ERR, FNAME,
				    "failed to add an IAPD");
				goto end;
			}
			break;
		case DHCP6_EVDATA_IANA:
			if (dhcp6_copy_list(&optinfo.iana_list,
			    (struct dhcp6_list *)evd->data)) {
				dprintf(LOG_ERR, FNAME,
				    "failed to add an IAPD");
				goto end;
			}
			break;
		default:
			dprintf(LOG_ERR, FNAME, "unexpected event data (%d)",
			    evd->type);
			exit(1);
		}
	}

	/* authentication information */
	if (set_auth(ev, &optinfo)) {
		dprintf(LOG_ERR, FNAME,
		    "failed to set authentication option");
		goto end;
	}

	/* set options in the message */
	if ((optlen = dhcp6_set_options(dh6->dh6_msgtype,
	    (struct dhcp6opt *)(dh6 + 1),
	    (struct dhcp6opt *)(buf + sizeof(buf)), &optinfo)) < 0) {
		dprintf(LOG_ERR, FNAME, "failed to construct options");
		goto end;
	}
	len += optlen;

	/* calculate MAC if necessary, and put it to the message */
	if (ev->authparam != NULL) {
		switch (ev->authparam->authproto) {
		case DHCP6_AUTHPROTO_DELAYED:
			if (ev->authparam->key == NULL)
				break;

			if (dhcp6_calc_mac((char *)dh6, len,
			    optinfo.authproto, optinfo.authalgorithm,
			    optinfo.delayedauth_offset + sizeof(*dh6),
			    ev->authparam->key)) {
				dprintf(LOG_ERR, FNAME,
				    "failed to calculate MAC");
				goto end;
			}
			break;
		default:
			break;	/* do nothing */
		}
	}

	/*
	 * Unless otherwise specified in this document or in a document that
	 * describes how IPv6 is carried over a specific type of link (for link
	 * types that do not support multicast), a client sends DHCP messages
	 * to the All_DHCP_Relay_Agents_and_Servers.
	 * [RFC3315 Section 13.]
	 */
	dst = *sa6_allagent;
	dst.sin6_scope_id = ifp->linkid;


    loginfo(FUNC, "send %s to %s",
	    dhcp6msgstr(dh6->dh6_msgtype), addr2str((struct sockaddr *)&dst));
    
	if (sendto(sock, buf, len, 0, (struct sockaddr *)&dst,
	    sysdep_sa_len((struct sockaddr *)&dst)) == -1) {
		dprintf(LOG_ERR, FNAME,
		    "transmit failed: %s", strerror(errno));
		goto end;
	}

	dprintf(LOG_DEBUG, FNAME, "send %s to %s",
	    dhcp6msgstr(dh6->dh6_msgtype), addr2str((struct sockaddr *)&dst));

  end:
	dhcp6_clear_options(&optinfo);
	return;
}

/* result will be a - b */
static void
tv_sub(a, b, result)
	struct timeval *a, *b, *result;
{
	if (a->tv_sec < b->tv_sec ||
	    (a->tv_sec == b->tv_sec && a->tv_usec < b->tv_usec)) {
		result->tv_sec = 0;
		result->tv_usec = 0;

		return;
	}

	result->tv_sec = a->tv_sec - b->tv_sec;
	if (a->tv_usec < b->tv_usec) {
		result->tv_usec = a->tv_usec + 1000000 - b->tv_usec;
		result->tv_sec -= 1;
	} else
		result->tv_usec = a->tv_usec - b->tv_usec;

	return;
}

//static void client6_recv()
static void client6_recv(int iAccept)
{
	char rbuf[BUFSIZ], cmsgbuf[BUFSIZ];
	struct msghdr mhdr;
	struct iovec iov;
	struct sockaddr_storage from;
	struct dhcp6_if *ifp;
	struct dhcp6opt *p, *ep;
	struct dhcp6_optinfo optinfo;
	ssize_t len;
	struct dhcp6 *dh6;
	struct cmsghdr *cm;
	struct in6_pktinfo *pi = NULL;

	memset(&iov, 0, sizeof(iov));
	memset(&mhdr, 0, sizeof(mhdr));

	iov.iov_base = (caddr_t)rbuf;
	iov.iov_len = sizeof(rbuf);
	mhdr.msg_name = (caddr_t)&from;
	mhdr.msg_namelen = sizeof(from);
	mhdr.msg_iov = &iov;
	mhdr.msg_iovlen = 1;
	mhdr.msg_control = (caddr_t)cmsgbuf;
	mhdr.msg_controllen = sizeof(cmsgbuf);
	if ((len = recvmsg(sock, &mhdr, 0)) < 0) {
		dprintf(LOG_ERR, FNAME, "recvmsg: %s", strerror(errno));
		return;
	}


#if 0
    // TODO: 允许走下去，因为一般情况下只剩下收到release报文的ACK的了这种情况下可以避免
    // TODO:不会发送多次，否则将会重发release
    if (!iAccept)
    {
        dprintf(LOG_ERR, FNAME, "drop packet");
        return ;
    }
#endif

	/* detect receiving interface */
	for (cm = (struct cmsghdr *)CMSG_FIRSTHDR(&mhdr); cm;
	     cm = (struct cmsghdr *)CMSG_NXTHDR(&mhdr, cm)) {
		if (cm->cmsg_level == IPPROTO_IPV6 &&
		    cm->cmsg_type == IPV6_PKTINFO &&
		    cm->cmsg_len == CMSG_LEN(sizeof(struct in6_pktinfo))) {
			pi = (struct in6_pktinfo *)(CMSG_DATA(cm));
		}
	}
	if (pi == NULL) {
		dprintf(LOG_NOTICE, FNAME, "failed to get packet info");
		return;
	}

	if ((ifp = find_ifconfbyid((unsigned int)pi->ipi6_ifindex)) == NULL) {
        
        loginfo(FUNC, "packet come form [%0x] ", pi->ipi6_ifindex);
        
		dprintf(LOG_INFO, FNAME, "unexpected interface (%d)",
		    (unsigned int)pi->ipi6_ifindex);
		return;
	}


	if (len < sizeof(*dh6)) {
		dprintf(LOG_INFO, FNAME, "short packet (%d bytes)", len);
		return;
	}

	dh6 = (struct dhcp6 *)rbuf;

	dprintf(LOG_DEBUG, FNAME, "receive %s from %s on %s",
	    dhcp6msgstr(dh6->dh6_msgtype),
	    addr2str((struct sockaddr *)&from), ifp->ifname);

	/* get options */
	dhcp6_init_options(&optinfo);
	p = (struct dhcp6opt *)(dh6 + 1);
	ep = (struct dhcp6opt *)((char *)dh6 + len);
	if (dhcp6_get_options(p, ep, &optinfo) < 0) {
		dprintf(LOG_INFO, FNAME, "failed to parse options");
		return;
	}

	switch(dh6->dh6_msgtype) {
	case DH6_ADVERTISE:
        loginfo(FUNC, "process avertise packet......");
		(void)client6_recvadvert(ifp, dh6, len, &optinfo);
		break;
	case DH6_REPLY:
        loginfo(FUNC, "process reply packet...");
		(void)client6_recvreply(ifp, dh6, len, &optinfo);
		break;
	default:
		dprintf(LOG_INFO, FNAME, "received an unexpected message (%s) "
		    "from %s", dhcp6msgstr(dh6->dh6_msgtype),
		    addr2str((struct sockaddr *)&from));
		break;
	}

	dhcp6_clear_options(&optinfo);
	return;
}

/* start of AU4D02348 by d00107688  网关做DHCPv6 Client，收到statue code为NoPrefixAvail或者unspecFail的advertise，仍然再次发起request */

/* 查找是否某一个iapd/iana存在状态码, 返回1表示找到 */
static int dhcp6_find_status_code(struct dhcp6_optinfo *pstoptinfo, 
	dhcp6_eventdata_t type, void *val, int option)
{
    struct dhcp6_list *head;
    struct dhcp6_list *sublist;
	struct dhcp6_listval *lv;

    if (!pstoptinfo)
    {
        return (0);
    }
    if (DHCP6_EVDATA_IAPD == type)
    {
        head = &pstoptinfo->iapd_list;
    }
    else if (DHCP6_EVDATA_IANA == type)
    {
        head = &pstoptinfo->iana_list;
    }
    else
    {
        return (0);
    }

    //dprintf(LOG_DEBUG, FNAME, "begin to found advertise contains status");

    /* 遍历所有iapd/iana列表 */
	for (lv = TAILQ_FIRST(head); lv; lv = TAILQ_NEXT(lv, link)) 
    {
        /* 遍历每一个链表下的子节点 */
        sublist = &lv->sublist;
		if (dhcp6_find_listval(sublist, DHCP6_LISTVAL_STCODE, val, 0)) 
	    {
            //fprintf(stderr, "status code: %d found\r\n", *(int *)val);
			dprintf(LOG_DEBUG, FNAME, "advertise contains status");
			return (1);
		}
	}

	return (0);
}
/* end of AU4D02348 by d00107688  网关做DHCPv6 Client，收到statue code为NoPrefixAvail或者unspecFail的advertise，仍然再次发起request */




static int
client6_recvadvert(ifp, dh6, len, optinfo)
	struct dhcp6_if *ifp;
	struct dhcp6 *dh6;
	ssize_t len;
	struct dhcp6_optinfo *optinfo;
{
	struct dhcp6_serverinfo *newserver, **sp;
	struct dhcp6_event *ev;
	struct dhcp6_eventdata *evd;
	struct authparam *authparam = NULL, authparam0;
    int iflag = 0;
	/* find the corresponding event based on the received xid */
	ev = find_event_withid(ifp, ntohl(dh6->dh6_xid) & DH6_XIDMASK);
	if (ev == NULL) {
		dprintf(LOG_INFO, FNAME, "XID mismatch");
		return (-1);
	}

	/* packet validation based on Section 15.3 of RFC3315. */
	if (optinfo->serverID.duid_len == 0) {
		dprintf(LOG_INFO, FNAME, "no server ID option");
		return (-1);
	} else {
		dprintf(LOG_DEBUG, FNAME, "server ID: %s, pref=%d",
		    duidstr(&optinfo->serverID),
		    optinfo->pref);
	}
	if (optinfo->clientID.duid_len == 0) {
		dprintf(LOG_INFO, FNAME, "no client ID option");
		return (-1);
	}
	if (duidcmp(&optinfo->clientID, &client_duid)) {
		dprintf(LOG_INFO, FNAME, "client DUID mismatch");
		return (-1);
	}

	/* validate authentication */
	authparam0 = *ev->authparam;
	if (process_auth(&authparam0, dh6, len, optinfo)) {
		dprintf(LOG_INFO, FNAME, "failed to process authentication");
		return (-1);
	}

	/*
	 * The requesting router MUST ignore any Advertise message that
	 * includes a Status Code option containing the value NoPrefixAvail
	 * [RFC3633 Section 11.1].
	 * Likewise, the client MUST ignore any Advertise message that includes
	 * a Status Code option containing the value NoAddrsAvail. 
	 * [RFC3315 Section 17.1.3].
	 * We only apply this when we are going to request an address or
	 * a prefix.
	 */
	for (evd = TAILQ_FIRST(&ev->data_list); evd;
	    evd = TAILQ_NEXT(evd, link)) {
		u_int16_t stcode;
		char *stcodestr;

		switch (evd->type) {
		case DHCP6_EVDATA_IAPD:
			stcode = DH6OPT_STCODE_NOPREFIXAVAIL;
			stcodestr = "NoPrefixAvail";
			break;
		case DHCP6_EVDATA_IANA:
			stcode = DH6OPT_STCODE_NOADDRSAVAIL;
			stcodestr = "NoAddrsAvail";
			break;
		default:
			continue;
		}
		if (dhcp6_find_listval(&optinfo->stcode_list,
		    DHCP6_LISTVAL_STCODE, &stcode, 0)) {
			dprintf(LOG_INFO, FNAME,
			    "advertise contains %s status", stcodestr);
			return (-1);
		}
        /* start of AU4D02348 by d00107688  网关做DHCPv6 Client，收到statue code为NoPrefixAvail或者unspecFail的advertise，仍然再次发起request */
        if (dhcp6_find_status_code(optinfo, evd->type, &stcode, 0))
        {
            if (DH6OPT_STCODE_NOPREFIXAVAIL == stcode)
            {
                iflag |= STCode_NoPrefixAvail;
            }
            else if (DH6OPT_STCODE_NOADDRSAVAIL == stcode)
            {
                iflag |= STCode_NOADDRSAVAIL;
            }
        }

        if ((STCode_NoPrefixAvail|STCode_NOADDRSAVAIL) == iflag)
        {
            dprintf(LOG_INFO, FNAME, "advertise contains %s status", stcodestr);
            return (-1);
        }
        
        /* end of AU4D02348 by d00107688  网关做DHCPv6 Client，收到statue code为NoPrefixAvail或者unspecFail的advertise，仍然再次发起request */
	}

	if (ev->state != DHCP6S_SOLICIT ||
	    (ifp->send_flags & DHCIFF_RAPID_COMMIT) || infreq_mode) {
		/*
		 * We expected a reply message, but do actually receive an
		 * Advertise message.  The server should be configured not to
		 * allow the Rapid Commit option.
		 * We process the message as if we expected the Advertise.
		 * [RFC3315 Section 17.1.4]
		 */
		dprintf(LOG_INFO, FNAME, "unexpected advertise");
		/* proceed anyway */
	}

	/* ignore the server if it is known */
	if (find_server(ev, &optinfo->serverID)) {
		dprintf(LOG_INFO, FNAME, "duplicated server (ID: %s)",
		    duidstr(&optinfo->serverID));
		return (-1);
	}

	/* keep the server */
	if ((newserver = malloc(sizeof(*newserver))) == NULL) {
		dprintf(LOG_WARNING, FNAME,
		    "memory allocation failed for server");
		return (-1);
	}
	memset(newserver, 0, sizeof(*newserver));

	/* remember authentication parameters */
	newserver->authparam = ev->authparam;
	newserver->authparam->flags = authparam0.flags;
	newserver->authparam->prevrd = authparam0.prevrd;
	newserver->authparam->key = authparam0.key;

	/* allocate new authentication parameter for the soliciting event */
	if ((authparam = new_authparam(ev->authparam->authproto,
	    ev->authparam->authalgorithm, ev->authparam->authrdm)) == NULL) {
		dprintf(LOG_WARNING, FNAME, "memory allocation failed "
		    "for authentication parameters");
		free(newserver);
		return (-1);
	}
	ev->authparam = authparam;

	/* copy options */
	dhcp6_init_options(&newserver->optinfo);
	if (dhcp6_copy_options(&newserver->optinfo, optinfo)) {
		dprintf(LOG_ERR, FNAME, "failed to copy options");
		if (newserver->authparam != NULL)
			free(newserver->authparam);
		free(newserver);
		return (-1);
	}
	if (optinfo->pref != DH6OPT_PREF_UNDEF)
		newserver->pref = optinfo->pref;
	newserver->active = 1;
	for (sp = &ev->servers; *sp; sp = &(*sp)->next) {
		if ((*sp)->pref != DH6OPT_PREF_MAX &&
		    (*sp)->pref < newserver->pref) {
			break;
		}
	}
	newserver->next = *sp;
	*sp = newserver;

	if (newserver->pref == DH6OPT_PREF_MAX) {
		/*
		 * If the client receives an Advertise message that includes a
		 * Preference option with a preference value of 255, the client
		 * immediately begins a client-initiated message exchange.
		 * [RFC3315 Section 17.1.2]
		 */
		ev->current_server = newserver;
		if (duidcpy(&ev->serverid,
		    &ev->current_server->optinfo.serverID)) {
			dprintf(LOG_NOTICE, FNAME, "can't copy server ID");
			return (-1); /* XXX: better recovery? */
		}
		if (construct_reqdata(ifp, &ev->current_server->optinfo, ev)) {
			dprintf(LOG_NOTICE, FNAME,
			    "failed to construct request data");
			return (-1); /* XXX */
		}

		ev->timeouts = 0;
		ev->state = DHCP6S_REQUEST;

		free(ev->authparam);
		ev->authparam = newserver->authparam;
		newserver->authparam = NULL;

		client6_send(ev);

		dhcp6_set_timeoparam(ev);
		dhcp6_reset_timer(ev);
	} else if (ev->servers->next == NULL) {
		struct timeval *rest, elapsed, tv_rt, tv_irt, timo;

		/*
		 * If this is the first advertise, adjust the timer so that
		 * the client can collect other servers until IRT elapses.
		 * XXX: we did not want to do such "low level" timer
		 *      calculation here.
		 */
		rest = dhcp6_timer_rest(ev->timer);
		tv_rt.tv_sec = (ev->retrans * 1000) / 1000000;
		tv_rt.tv_usec = (ev->retrans * 1000) % 1000000;
		tv_irt.tv_sec = (ev->init_retrans * 1000) / 1000000;
		tv_irt.tv_usec = (ev->init_retrans * 1000) % 1000000;
		timeval_sub(&tv_rt, rest, &elapsed);
		if (TIMEVAL_LEQ(elapsed, tv_irt))
			timeval_sub(&tv_irt, &elapsed, &timo);
		else
			timo.tv_sec = timo.tv_usec = 0;

		dprintf(LOG_DEBUG, FNAME, "reset timer for %s to %d.%06d",
		    ifp->ifname, (int)timo.tv_sec, (int)timo.tv_usec);

		dhcp6_set_timer(&timo, ev->timer);
	}

	return (0);
}

static struct dhcp6_serverinfo *
find_server(ev, duid)
	struct dhcp6_event *ev;
	struct duid *duid;
{
	struct dhcp6_serverinfo *s;

	for (s = ev->servers; s; s = s->next) {
		if (duidcmp(&s->optinfo.serverID, duid) == 0)
			return (s);
	}

	return (NULL);
}

static int
client6_recvreply(ifp, dh6, len, optinfo)
	struct dhcp6_if *ifp;
	struct dhcp6 *dh6;
	ssize_t len;
	struct dhcp6_optinfo *optinfo;
{
	struct dhcp6_listval *lv;
	struct dhcp6_event *ev;
	int state;

    FUNCBEGIN();

	/* find the corresponding event based on the received xid */
	ev = find_event_withid(ifp, ntohl(dh6->dh6_xid) & DH6_XIDMASK);
	if (ev == NULL) {
		dprintf(LOG_INFO, FNAME, "XID mismatch");
		return (-1);
	}

	state = ev->state;
	if (state != DHCP6S_INFOREQ &&
	    state != DHCP6S_REQUEST &&
	    state != DHCP6S_RENEW &&
	    state != DHCP6S_REBIND &&
	    state != DHCP6S_RELEASE &&
	    (state != DHCP6S_SOLICIT ||
	     !(ifp->send_flags & DHCIFF_RAPID_COMMIT))) {
		dprintf(LOG_INFO, FNAME, "unexpected reply");
		return (-1);
	}

	/* A Reply message must contain a Server ID option */
	if (optinfo->serverID.duid_len == 0) {
		dprintf(LOG_INFO, FNAME, "no server ID option");
		return (-1);
	}

	/*
	 * DUID in the Client ID option (which must be contained for our
	 * client implementation) must match ours.
	 */
	if (optinfo->clientID.duid_len == 0) {
		dprintf(LOG_INFO, FNAME, "no client ID option");
		return (-1);
	}
	if (duidcmp(&optinfo->clientID, &client_duid)) {
		dprintf(LOG_INFO, FNAME, "client DUID mismatch");
		return (-1);
	}

	/* validate authentication */
	if (process_auth(ev->authparam, dh6, len, optinfo)) {
		dprintf(LOG_INFO, FNAME, "failed to process authentication");
		return (-1);
	}

	/*
	 * If the client included a Rapid Commit option in the Solicit message,
	 * the client discards any Reply messages it receives that do not
	 * include a Rapid Commit option.
	 * (should we keep the server otherwise?)
	 * [RFC3315 Section 17.1.4]
	 */
	if (state == DHCP6S_SOLICIT &&
	    (ifp->send_flags & DHCIFF_RAPID_COMMIT) &&
	    !optinfo->rapidcommit) {
		dprintf(LOG_INFO, FNAME, "no rapid commit");
		return (-1);
	}

	/*
	 * The client MAY choose to report any status code or message from the
	 * status code option in the Reply message.
	 * [RFC3315 Section 18.1.8]
	 */
	for (lv = TAILQ_FIRST(&optinfo->stcode_list); lv;
	     lv = TAILQ_NEXT(lv, link)) {
		dprintf(LOG_INFO, FNAME, "status code: %s",
		    dhcp6_stcodestr(lv->val_num16));
	}

    /* 处理DNS信息 */  
    loginfo(FUNC, "process dns info.....");
    if (DHCP6S_RELEASE != state)
    {
        ProcessDHCPDNSInfo(optinfo);
        ProcessDHCPDsliteInfo(optinfo);
    }
    loginfo(FUNC, "..............process over. so begin to print it....");
	if (!TAILQ_EMPTY(&optinfo->dns_list)) {        
		struct dhcp6_listval *d;
		int i = 0;
		for (d = TAILQ_FIRST(&optinfo->dns_list); d;
		     d = TAILQ_NEXT(d, link), i++) {
			info_printf("nameserver[%d] %s",
			    i, in6addr2str(&d->val_addr6, 0));
		}
	}
    loginfo(FUNC, "process dns info over...");

	if (!TAILQ_EMPTY(&optinfo->dnsname_list)) {
		struct dhcp6_listval *d;
		int i = 0;

		for (d = TAILQ_FIRST(&optinfo->dnsname_list); d;
		     d = TAILQ_NEXT(d, link), i++) {
			info_printf("Domain search list[%d] %s",
			    i, d->val_vbuf.dv_buf);
		}
	}

	if (!TAILQ_EMPTY(&optinfo->ntp_list)) {
		struct dhcp6_listval *d;
		int i = 0;

		for (d = TAILQ_FIRST(&optinfo->ntp_list); d;
		     d = TAILQ_NEXT(d, link), i++) {
			info_printf("NTP server[%d] %s",
			    i, in6addr2str(&d->val_addr6, 0));
		}
	}

	if (!TAILQ_EMPTY(&optinfo->sip_list)) {
		struct dhcp6_listval *d;
		int i = 0;

		for (d = TAILQ_FIRST(&optinfo->sip_list); d;
		     d = TAILQ_NEXT(d, link), i++) {
			info_printf("SIP server address[%d] %s",
			    i, in6addr2str(&d->val_addr6, 0));
		}
	}

	if (!TAILQ_EMPTY(&optinfo->sipname_list)) {
		struct dhcp6_listval *d;
		int i = 0;

		for (d = TAILQ_FIRST(&optinfo->sipname_list); d;
		     d = TAILQ_NEXT(d, link), i++) {
			info_printf("SIP domain name[%d] %s",
			    i, d->val_vbuf.dv_buf);
		}
	}
    
	if (!TAILQ_EMPTY(&optinfo->dslite_list)) {
		struct dhcp6_listval *d;
		int i = 0;

		for (d = TAILQ_FIRST(&optinfo->dslite_list); d;
		     d = TAILQ_NEXT(d, link), i++) {
			info_printf("DS-Lite address[%d] %s",
			    i, in6addr2str(&d->val_addr6, 0));
		}
	}

	if (!TAILQ_EMPTY(&optinfo->dslitename_list)) {
		struct dhcp6_listval *d;
		int i = 0;

		for (d = TAILQ_FIRST(&optinfo->dslitename_list); d;
		     d = TAILQ_NEXT(d, link), i++) {
			info_printf("DS-Lite domain name[%d] %s",
			    i, d->val_vbuf.dv_buf);
		}
	}

	/*
	 * Call the configuration script, if specified, to handle various
	 * configuration parameters.
	 */
	if (ifp->scriptpath != NULL && strlen(ifp->scriptpath) != 0) {
		dprintf(LOG_DEBUG, FNAME, "executes %s", ifp->scriptpath);
		client6_script(ifp->scriptpath, state, optinfo);
	}

	/*
	 * Set refresh timer for configuration information specified in
	 * information-request.  If the timer value is specified by the server
	 * in an information refresh time option, use it; use the protocol
	 * default otherwise.
	 */
	if (state == DHCP6S_INFOREQ) {
		int64_t refreshtime = DHCP6_IRT_DEFAULT;

		if (optinfo->refreshtime != DH6OPT_REFRESHTIME_UNDEF)
			refreshtime = optinfo->refreshtime;

		ifp->timer = dhcp6_add_timer(client6_expire_refreshtime, ifp);
		if (ifp->timer == NULL) {
			dprintf(LOG_WARNING, FNAME,
			    "failed to add timer for refresh time");
		} else {
			struct timeval tv;

			tv.tv_sec = (long)refreshtime;
			tv.tv_usec = 0;

			if (tv.tv_sec < 0) {
				/*
				 * XXX: tv_sec can overflow for an
				 * unsigned 32bit value.
				 */
				dprintf(LOG_WARNING, FNAME,
				    "refresh time is too large: %lu",
				    (u_int32_t)refreshtime);
				tv.tv_sec = 0x7fffffff;	/* XXX */
			}

			dhcp6_set_timer(&tv, ifp->timer);
		}
	} else if (optinfo->refreshtime != DH6OPT_REFRESHTIME_UNDEF) {
		/*
		 * draft-ietf-dhc-lifetime-02 clarifies that refresh time
		 * is only used for information-request and reply exchanges.
		 */
		dprintf(LOG_INFO, FNAME,
		    "unexpected information refresh time option (ignored)");
	}

	/* update stateful configuration information */
	if (state != DHCP6S_RELEASE) {
	/*更换顺序,先生成IANA地址, 后续的判断有了IANA地址, 
	WAN接口就不用PD地址了, 专门给LAN侧用*/
		update_ia(IATYPE_NA, &optinfo->iana_list, ifp,
		    &optinfo->serverID, ev->authparam);
		update_ia(IATYPE_PD, &optinfo->iapd_list, ifp,
		    &optinfo->serverID, ev->authparam);
	}

#ifdef DHCP6_DECLIEN
    if ( DECLINE_SWITCH_ENABLE == g_ldeclineswitch )
    {        
        if ( state == DHCP6S_REQUEST ) {
            int iret = 0;
            
            iret = client6_setlistvalue(&optinfo->iapd_list,IATYPE_PD,ifp,&optinfo->serverID,&optinfo->clientID);
            if ( RETURN_TRUE != iret )
            {
                dhcp6_debug("%s","FUN[client6_setlistvalue] meet error !");
            }

            iret = client6_setlistvalue(&optinfo->iana_list,IATYPE_NA,ifp,&optinfo->serverID,&optinfo->clientID);
            if ( RETURN_TRUE != iret )
            {
                dhcp6_debug("%s","FUN[client6_setlistvalue] meet error !");
            }
        }
    }
#endif    

	dhcp6_remove_event(ev);

	if (state == DHCP6S_RELEASE) {
		/*
		 * When the client receives a valid Reply message in response
		 * to a Release message, the client considers the Release event
		 * completed, regardless of the Status Code option(s) returned
		 * by the server.
		 * [RFC3315 Section 18.1.8]
		 */
		check_exit();
	}

	dprintf(LOG_DEBUG, FNAME, "got an expected reply, sleeping.");

	if (infreq_mode) {
		exit_ok = 1;
		free_resources(NULL);
		//unlink(pid_file);
		check_exit();
	}

#ifdef DHCP6_DECLIEN
/* BEGIN: Added by z67728, 2010/1/29 process wan dad .*/
    if ( DECLINE_SWITCH_ENABLE == g_ldeclineswitch )
    {        
        if ( state == DHCP6S_REQUEST )
        {
            int lLoop = 0;
            for ( lLoop = 0 ; lLoop < DHCP6_CHECK_NUM ; lLoop++ )
            {            
                client6_addressduplicateprocess();
            }
        }
    }
/* END:   Added by z67728, 2010/1/29 */
#endif
    FUNCEND();
	return (0);
}

static struct dhcp6_event *
find_event_withid(ifp, xid)
	struct dhcp6_if *ifp;
	u_int32_t xid;
{
	struct dhcp6_event *ev;

	for (ev = TAILQ_FIRST(&ifp->event_list); ev;
	     ev = TAILQ_NEXT(ev, link)) {
		if (ev->xid == xid)
			return (ev);
	}

	return (NULL);
}

static int
process_auth(authparam, dh6, len, optinfo)
	struct authparam *authparam;
	struct dhcp6 *dh6;
	ssize_t len;
	struct dhcp6_optinfo *optinfo;
{
	struct keyinfo *key = NULL;
	int authenticated = 0;

	switch (optinfo->authproto) {
	case DHCP6_AUTHPROTO_UNDEF:
		/* server did not provide authentication option */
		break;
	case DHCP6_AUTHPROTO_DELAYED:
		if ((optinfo->authflags & DHCP6OPT_AUTHFLAG_NOINFO)) {
			dprintf(LOG_INFO, FNAME, "server did not include "
			    "authentication information");
			break;
		}

		if (optinfo->authalgorithm != DHCP6_AUTHALG_HMACMD5) {
			dprintf(LOG_INFO, FNAME, "unknown authentication "
			    "algorithm (%d)", optinfo->authalgorithm);
			break;
		}

		if (optinfo->authrdm != DHCP6_AUTHRDM_MONOCOUNTER) {
			dprintf(LOG_INFO, FNAME,"unknown RDM (%d)",
			    optinfo->authrdm);
			break;
		}

		/*
		 * Replay protection.  If we do not know the previous RD value,
		 * we accept the message anyway (XXX).
		 */
		if ((authparam->flags & AUTHPARAM_FLAGS_NOPREVRD)) {
			dprintf(LOG_WARNING, FNAME, "previous RD value is "
			    "unknown (accept it)");
		} else {
			if (dhcp6_auth_replaycheck(optinfo->authrdm,
			    authparam->prevrd, optinfo->authrd)) {
				dprintf(LOG_INFO, FNAME,
				    "possible replay attack detected");
				break;
			}
		}

		/* identify the secret key */
		if ((key = authparam->key) != NULL) {
			/*
			 * If we already know a key, its identification should
			 * match that contained in the received option.
			 * (from Section 21.4.5.1 of RFC3315)
			 */
			if (optinfo->delayedauth_keyid != key->keyid ||
			    optinfo->delayedauth_realmlen != key->realmlen ||
			    memcmp(optinfo->delayedauth_realmval, key->realm,
			    key->realmlen) != 0) {
				dprintf(LOG_INFO, FNAME,
				    "authentication key mismatch");
				break;
			}
		} else {
			key = find_key(optinfo->delayedauth_realmval,
			    optinfo->delayedauth_realmlen,
			    optinfo->delayedauth_keyid);
			if (key == NULL) {
				dprintf(LOG_INFO, FNAME, "failed to find key "
				    "provided by the server (ID: %x)",
				    optinfo->delayedauth_keyid);
				break;
			} else {
				dprintf(LOG_DEBUG, FNAME, "found key for "
				    "authentication: %s", key->name);
			}
			authparam->key = key;
		}

		/* check for the key lifetime */
		if (dhcp6_validate_key(key)) {
			dprintf(LOG_INFO, FNAME, "key %s has expired",
			    key->name);
			break;
		}

		/* validate MAC */
		if (dhcp6_verify_mac((char *)dh6, len, optinfo->authproto,
		    optinfo->authalgorithm,
		    optinfo->delayedauth_offset + sizeof(*dh6), key) == 0) {
			dprintf(LOG_DEBUG, FNAME, "message authentication "
			    "validated");
			authenticated = 1;
		} else {
			dprintf(LOG_INFO, FNAME, "invalid message "
			    "authentication");
		}

		break;
	default:
		dprintf(LOG_INFO, FNAME, "server sent unsupported "
		    "authentication protocol (%d)", optinfo->authproto);
		break;
	}

	if (authenticated == 0) {
		if (authparam->authproto != DHCP6_AUTHPROTO_UNDEF) {
			dprintf(LOG_INFO, FNAME, "message not authenticated "
			    "while authentication required");

			/*
			 * Right now, we simply discard unauthenticated
			 * messages.
			 */
			return (-1);
		}
	} else {
		/* if authenticated, update the "previous" RD value */
		authparam->prevrd = optinfo->authrd;
		authparam->flags &= ~AUTHPARAM_FLAGS_NOPREVRD;
	}

	return (0);
}

static int
set_auth(ev, optinfo)
	struct dhcp6_event *ev;
	struct dhcp6_optinfo *optinfo;
{
	struct authparam *authparam = ev->authparam;

	if (authparam == NULL)
		return (0);

	optinfo->authproto = authparam->authproto;
	optinfo->authalgorithm = authparam->authalgorithm;
	optinfo->authrdm = authparam->authrdm;

	switch (authparam->authproto) {
	case DHCP6_AUTHPROTO_UNDEF: /* we simply do not need authentication */
		return (0);
	case DHCP6_AUTHPROTO_DELAYED:
		if (ev->state == DHCP6S_INFOREQ) {
			/*
			 * In the current implementation, delayed
			 * authentication for Information-request and Reply
			 * exchanges doesn't work.  Specification is also
			 * unclear on this usage.
			 */
			dprintf(LOG_WARNING, FNAME, "delayed authentication "
			    "cannot be used for Information-request yet");
			return (-1);
		}

		if (ev->state == DHCP6S_SOLICIT) {
			optinfo->authflags |= DHCP6OPT_AUTHFLAG_NOINFO;
			return (0); /* no auth information is needed */
		}

		if (authparam->key == NULL) {
			dprintf(LOG_INFO, FNAME,
			    "no authentication key for %s",
			    dhcp6_event_statestr(ev));
			return (-1);
		}

		if (dhcp6_validate_key(authparam->key)) {
			dprintf(LOG_INFO, FNAME, "key %s is invalid",
			    authparam->key->name);
			return (-1);
		}

		if (get_rdvalue(optinfo->authrdm, &optinfo->authrd,
		    sizeof(optinfo->authrd))) {
			dprintf(LOG_ERR, FNAME, "failed to get a replay "
			    "detection value");
			return (-1);
		}

		optinfo->delayedauth_keyid = authparam->key->keyid;
		optinfo->delayedauth_realmlen = authparam->key->realmlen;
		optinfo->delayedauth_realmval =
		    malloc(optinfo->delayedauth_realmlen);
		if (optinfo->delayedauth_realmval == NULL) {
			dprintf(LOG_ERR, FNAME, "failed to allocate memory "
			    "for authentication realm");
			return (-1);
		}
		memcpy(optinfo->delayedauth_realmval, authparam->key->realm,
		    optinfo->delayedauth_realmlen);

		break;
	default:
		dprintf(LOG_ERR, FNAME, "unsupported authentication protocol "
		    "%d", authparam->authproto);
		return (-1);
	}

	return (0);
}

static void
info_printf(const char *fmt, ...)
{
	va_list ap;
	char logbuf[LINE_MAX];

	va_start(ap, fmt);
	vsnprintf(logbuf, sizeof(logbuf), fmt, ap);

	dprintf(LOG_DEBUG, FNAME, "%s", logbuf);
	if (infreq_mode)
		printf("%s\n", logbuf);

	return;
}
