/*	$KAME: prefixconf.c,v 1.33 2005/09/16 11:30:15 suz Exp $	*/

/*
 * Copyright (C) 2002 WIDE Project.
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
 /******************************************************************************
  历史记录      :
3082101267  y00188255  2013/8/24        修改所有获取时间的方式为单板上电时间
*********************************************************************************/
 
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/queue.h>
#include <sys/ioctl.h>

#include <net/if.h>
#ifdef __FreeBSD__
#include <net/if_var.h>
#endif

#include <net/if_arp.h>

#include <netinet/in.h>

#ifdef __KAME__
#include <netinet6/in6_var.h>
#include <netinet6/nd6.h>
#endif

#include <errno.h>
#include <syslog.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "dhcp6.h"
#include "config.h"
#include "common.h"
#include "timer.h"
#include "dhcp6c_ia.h"
#include "prefixconf.h"

#include "dhcp6cinfo.h"

#include "ifaddrs.h"

#include "misc.h"

TAILQ_HEAD(siteprefix_list, siteprefix);
struct iactl_pd {
	struct iactl common;
	struct pifc_list *pifc_head;
	struct siteprefix_list siteprefix_head;
};
#define iacpd_ia common.iactl_ia
#define iacpd_callback common.callback
#define iacpd_isvalid common.isvalid
#define iacpd_duration common.duration
#define iacpd_renew_data common.renew_data
#define iacpd_rebind_data common.rebind_data
#define iacpd_reestablish_data common.reestablish_data
#define iacpd_release_data common.release_data
#define iacpd_cleanup common.cleanup

struct siteprefix {
	TAILQ_ENTRY (siteprefix) link;

	struct dhcp6_prefix prefix;
	time_t updatetime;
	struct dhcp6_timer *timer;
	struct iactl_pd *ctl;
	TAILQ_HEAD(, dhcp6_ifprefix) ifprefix_list; /* interface prefixes */
};

struct dhcp6_ifprefix {
	TAILQ_ENTRY(dhcp6_ifprefix) plink;

	/* interface configuration */
	struct prefix_ifconf *ifconf;

/* start of by d00107688 支持根据前缀给接口生成地址，而不需要从配置文件中指定 */
    struct dhcp6_if *pstIfp;
/* end of by d00107688 支持根据前缀给接口生成地址，而不需要从配置文件中指定 */

	/* interface prefix parameters */
	struct sockaddr_in6 paddr;
	int plen;

	/* address assigned on the interface based on the prefix */
	struct sockaddr_in6 ifaddr;
};

static struct siteprefix *find_siteprefix __P((struct siteprefix_list *,
    struct dhcp6_prefix *, int));
static void remove_siteprefix __P((struct siteprefix *));
static int isvalid __P((struct iactl *));
static u_int32_t duration __P((struct iactl *));
static void cleanup __P((struct iactl *));
static int renew_prefix __P((struct iactl *, struct dhcp6_ia *,
    struct dhcp6_eventdata **, struct dhcp6_eventdata *));
static void renew_data_free __P((struct dhcp6_eventdata *));

static struct dhcp6_timer *siteprefix_timo __P((void *));

static int add_ifprefix __P((struct siteprefix *,
    struct dhcp6_prefix *, struct prefix_ifconf *));

extern struct dhcp6_timer *client6_timo __P((void *));
static int pd_ifaddrconf __P((ifaddrconf_cmd_t, struct dhcp6_ifprefix *ifpfx));

static int if_prefixaddradd(struct dhcp6_ifprefix *ifpfx);

static int add_ifprefix2(
    struct dhcp6_if *pstIfp,
    struct siteprefix *siteprefix,
	struct dhcp6_prefix *prefix)
{
    struct dhcp6_ifprefix *ifpfx = NULL;
    struct in6_addr *a;
    u_long sla_id;
    char acIfID[32] = {0};
    char acTmp[10] = {0};
    int iIdLen = 0;
    int iHwType = 0;
    char *sp;
    int i;
    int iLft = 0;

    if (!pstIfp)
    {
        dprintf(LOG_ERR, FNAME, "args NULL: ifp can not be NULL\r\n\r\n");
        return (-1);
    }

    iIdLen = 32;
    if (0 > GetInterfaceID(pstIfp->ifname, acIfID, &iIdLen, &iHwType))
    {
        dprintf(LOG_ERR, FNAME, "Error: get interface id for [%s] error\r\n", pstIfp->ifname);
        return (-1);
    }

    if ((ifpfx = malloc(sizeof(*ifpfx))) == NULL) {
        dprintf(LOG_NOTICE, FNAME,
            "failed to allocate memory for ifprefix");
        return (-1);
    }
    memset(ifpfx, 0, sizeof(*ifpfx));

    ifpfx->ifconf = NULL;

    ifpfx->pstIfp = pstIfp;

    ifpfx->paddr.sin6_family = AF_INET6;
#ifdef HAVE_SA_LEN
    ifpfx->paddr.sin6_len = sizeof(struct sockaddr_in6);
#endif
    ifpfx->paddr.sin6_addr = prefix->addr;
    ifpfx->plen = 128;

    dprintf(LOG_ERR, FNAME, "ifid: %02x %02x %02x %02x %02x %02x %02x %02x\r\n",
        acIfID[0], acIfID[1], acIfID[2], acIfID[3], acIfID[4],
        acIfID[5], acIfID[6], acIfID[7]);

    /* 如果前缀为64bit，那么就是用MAC地址来生成该地址，否则的话，如果大于64bit那么不处理 */
    if (64 >= prefix->plen)
    {
        if (ARPHRD_ETHER == iHwType)
        {
            memcpy(acTmp, acIfID, 3);
            acTmp[3] = 0xff;
            acTmp[4] = 0xfe;
            acTmp[0] ^= 2;
            memcpy(acTmp+5, acIfID+3, 3);
        }
        else
        {
            memcpy(acTmp, acIfID, 8);
        }
        
        memcpy(ifpfx->paddr.sin6_addr.s6_addr + 8, 
            acTmp, 8);
    }
    else
    {
        i = prefix->plen / 8;
        iLft = prefix->plen % 8;
        ifpfx->paddr.sin6_addr.s6_addr[i] &= (0xff << (8 - i));
        memset(ifpfx->paddr.sin6_addr.s6_addr[i + 1], 0, (16 - i -1));
    }
    
    /* configure the corresponding address */
    ifpfx->ifaddr = ifpfx->paddr;

    int iRet = 0;
    iRet = if_prefixaddradd(ifpfx);
    if (iRet < 0)
    {
        if (EEXIST != iRet)
        {
            goto bad;
        }
    }

    /* TODO: send a control message for other processes */

    TAILQ_INSERT_TAIL(&siteprefix->ifprefix_list, ifpfx, plink);

    return (0);

  bad:
    if (ifpfx)
        free(ifpfx);
    return (-1);
}

int
update_prefix(ia, pinfo, pifc, dhcpifp, ctlp, callback)
	struct ia *ia;
	struct dhcp6_prefix *pinfo;
	struct pifc_list *pifc;
	struct dhcp6_if *dhcpifp;
	struct iactl **ctlp;
	void (*callback)__P((struct ia *));
{
	struct iactl_pd *iac_pd = (struct iactl_pd *)*ctlp;
	struct siteprefix *sp;
	struct prefix_ifconf *pif;
	int spcreate = 0;
	struct timeval timo;
	
	struct ifaddrs *ifa, *ifap;
	struct sockaddr_in6 *sin6;
	struct in6_addr addr6;

	/*
	 * A client discards any addresses for which the preferred
         * lifetime is greater than the valid lifetime.
	 * [RFC3315 22.6] 
	 */
	if (pinfo->vltime != DHCP6_DURATION_INFINITE &&
	    (pinfo->pltime == DHCP6_DURATION_INFINITE ||
	    pinfo->pltime > pinfo->vltime)) {
		dprintf(LOG_INFO, FNAME, "invalid prefix %s/%d: "
		    "pltime (%lu) is larger than vltime (%lu)",
		    in6addr2str(&pinfo->addr, 0), pinfo->plen,
		    pinfo->pltime, pinfo->vltime);
		return (-1);
	}

	if (iac_pd == NULL) {
		if ((iac_pd = malloc(sizeof(*iac_pd))) == NULL) {
			dprintf(LOG_NOTICE, FNAME, "memory allocation failed");
			return (-1);
		}
		memset(iac_pd, 0, sizeof(*iac_pd));
		iac_pd->iacpd_ia = ia;
		iac_pd->iacpd_callback = callback;
		iac_pd->iacpd_isvalid = isvalid;
		iac_pd->iacpd_duration = duration;
		iac_pd->iacpd_cleanup = cleanup;
		iac_pd->iacpd_renew_data =
		    iac_pd->iacpd_rebind_data =
		    iac_pd->iacpd_release_data =
		    iac_pd->iacpd_reestablish_data = renew_prefix;

		iac_pd->pifc_head = pifc;
		TAILQ_INIT(&iac_pd->siteprefix_head);
		*ctlp = (struct iactl *)iac_pd;
	}

	/* search for the given prefix, and make a new one if it fails */
	if ((sp = find_siteprefix(&iac_pd->siteprefix_head, pinfo, 1)) == NULL) {
		if ((sp = malloc(sizeof(*sp))) == NULL) {
			dprintf(LOG_NOTICE, FNAME, "memory allocation failed");
			return (-1);
		}
		memset(sp, 0, sizeof(*sp));
		sp->prefix.addr = pinfo->addr;
		sp->prefix.plen = pinfo->plen;
		sp->ctl = iac_pd;
		TAILQ_INIT(&sp->ifprefix_list);

		TAILQ_INSERT_TAIL(&iac_pd->siteprefix_head, sp, link);

		spcreate = 1;
	}

	/* update the timestamp of update */
    /* BEGIN 3082101267 y00188255 2013-8-24  Modified */
	sp->updatetime = (time_t)dhcp_get_cur_time();
    /* END 3082101267 y00188255 2013-8-24  Modified */
	/* update the prefix according to pinfo */
	sp->prefix.pltime = pinfo->pltime;
	sp->prefix.vltime = pinfo->vltime;
	dprintf(LOG_DEBUG, FNAME, "%s a prefix %s/%d pltime=%lu, vltime=%lu",
	    spcreate ? "create" : "update",
	    in6addr2str(&pinfo->addr, 0), pinfo->plen,
	    pinfo->pltime, pinfo->vltime);

    /* 先将前缀添加进去 */
    if (0 != pinfo->vltime)
    {
        ProcessDHCPIAPD(pinfo, 1);
        /*记下这个prefix的prefer time和valid time的值，存到文件*/
        SetFileValue(WAN_PREFIX_VALID_TIME_FILE, dhcpifp->ifname, pinfo->vltime);
        SetFileValue(WAN_PREFIX_PREFER_TIME_FILE, dhcpifp->ifname, pinfo->pltime);
    }
    else
    {
        ProcessDHCPIAPD(pinfo, 0);
    }

#if 0
	/* update prefix interfaces if necessary */
	if (sp->prefix.vltime != 0 && spcreate) {
		for (pif = TAILQ_FIRST(iac_pd->pifc_head); pif;
		    pif = TAILQ_NEXT(pif, link)) {
			/*
			 * The requesting router MUST NOT assign any delegated
			 * prefixes or subnets from the delegated prefix(es) to
			 * the link through which it received the DHCP message
			 * from the delegating router.
			 * [RFC3633 Section 12.1]
			 */
			if (strcmp(pif->ifname, dhcpifp->ifname) == 0) {
				dprintf(LOG_INFO, FNAME,
				    "skip %s as a prefix interface",
				    dhcpifp->ifname);
				continue;
			}
			add_ifprefix(sp, pinfo, pif);
		}
	}
#else
if (sp->prefix.vltime != 0 && spcreate)
    {
/*判断WAN口是否有RA前缀生成的地址, 若已有,则不要用PD的了*/
        {
		if (getifaddrs(&ifap) < 0) {
			dprintf(LOG_ERR, FNAME, "getifaddrs failed: %s",
			    strerror(errno));
			return -1;
		}
		memset(&addr6, 0, sizeof(struct in6_addr));
		for (ifa = ifap; ifa; ifa = ifa->ifa_next) {
			if (strcmp(ifa->ifa_name, dhcpifp->ifname) != 0)
				continue;
			if (ifa->ifa_addr == NULL)
				continue;
			if (ifa->ifa_addr->sa_family != AF_INET6)
				continue;
			sin6 = (struct sockaddr_in6 *)ifa->ifa_addr;
			if (IN6_IS_ADDR_LINKLOCAL(&sin6->sin6_addr))
				continue;
			memcpy(&addr6, &sin6->sin6_addr, sizeof(struct in6_addr));
		}
		freeifaddrs(ifap);
	}
	if (IN6_IS_ADDR_LINKLOCAL(&addr6)  || IN6_IS_ADDR_UNSPECIFIED(&addr6))
	{
            add_ifprefix2(dhcpifp, sp, pinfo);
         }
    }
#endif
	/*
	 * If the new vltime is 0, this prefix immediately expires.
	 * Otherwise, set up or update the associated timer.
	 */
	switch (sp->prefix.vltime) {
	case 0:
		remove_siteprefix(sp);
		break;
	case DHCP6_DURATION_INFINITE:
		if (sp->timer)
			dhcp6_remove_timer(&sp->timer);
		break;
	default:
		if (sp->timer == NULL) {
			sp->timer = dhcp6_add_timer(siteprefix_timo, sp);
			if (sp->timer == NULL) {
				dprintf(LOG_NOTICE, FNAME,
				    "failed to add prefix timer");
				remove_siteprefix(sp); /* XXX */
				return (-1);
			}
		}
		/* update the timer */
		timo.tv_sec = sp->prefix.vltime;
		timo.tv_usec = 0;

		dhcp6_set_timer(&timo, sp->timer);
		break;
	}

	return (0);
}

static struct siteprefix *
find_siteprefix(head, prefix, match_plen)
	struct siteprefix_list *head;
	struct dhcp6_prefix *prefix;
	int match_plen;
{
	struct siteprefix *sp;

	for (sp = TAILQ_FIRST(head); sp; sp = TAILQ_NEXT(sp, link)) {
		if (!IN6_ARE_ADDR_EQUAL(&sp->prefix.addr, &prefix->addr))
			continue;
		if (match_plen == 0 || sp->prefix.plen == prefix->plen)
			return (sp);
	}

	return (NULL);
}

static void
remove_siteprefix(sp)
	struct siteprefix *sp;
{
	struct dhcp6_ifprefix *ip;

	dprintf(LOG_DEBUG, FNAME, "remove a site prefix %s/%d",
	    in6addr2str(&sp->prefix.addr, 0), sp->prefix.plen);

	if (sp->timer)
		dhcp6_remove_timer(&sp->timer);

    /* 删除前缀 */
    ProcessDHCPIAPD(&sp->prefix, 0);

	/* remove all interface prefixes */
	while ((ip = TAILQ_FIRST(&sp->ifprefix_list)) != NULL) {        
		TAILQ_REMOVE(&sp->ifprefix_list, ip, plink);        
		pd_ifaddrconf(IFADDRCONF_REMOVE, ip);
		free(ip);
	}

	TAILQ_REMOVE(&sp->ctl->siteprefix_head, sp, link);
	free(sp);
}

static int
isvalid(iac)
	struct iactl *iac;
{
	struct iactl_pd *iac_pd = (struct iactl_pd *)iac;

	if (TAILQ_EMPTY(&iac_pd->siteprefix_head))
		return (0);	/* this IA is invalid */
	return (1);
}

static u_int32_t
duration(iac)
	struct iactl *iac;
{
	struct iactl_pd *iac_pd = (struct iactl_pd *)iac;
	struct siteprefix *sp;
	u_int32_t base = DHCP6_DURATION_INFINITE, pltime, passed;
	time_t now;

	/* Determine the smallest period until pltime expires. */
    /* BEGIN 3082101267 y00188255 2013-8-24  Modified */
    now = (time_t)dhcp_get_cur_time();
    /* END 3082101267 y00188255 2013-8-24  Modified */
	for (sp = TAILQ_FIRST(&iac_pd->siteprefix_head); sp;
	    sp = TAILQ_NEXT(sp, link)) {
		passed = now > sp->updatetime ?
		    (u_int32_t)(now - sp->updatetime) : 0;
		pltime = sp->prefix.pltime > passed ?
		    sp->prefix.pltime - passed : 0;

		if (base == DHCP6_DURATION_INFINITE || pltime < base)
			base = pltime;
	}

	return (base);
}

static void
cleanup(iac)
	struct iactl *iac;
{
	struct iactl_pd *iac_pd = (struct iactl_pd *)iac;
	struct siteprefix *sp;

	while ((sp = TAILQ_FIRST(&iac_pd->siteprefix_head)) != NULL) {
		TAILQ_REMOVE(&iac_pd->siteprefix_head, sp, link);
		remove_siteprefix(sp);
	}

	free(iac);
}

static int
renew_prefix(iac, iaparam, evdp, evd)
	struct iactl *iac;
	struct dhcp6_ia *iaparam;
	struct dhcp6_eventdata **evdp, *evd;
{
	struct iactl_pd *iac_pd = (struct iactl_pd *)iac;
	struct siteprefix *sp;
	struct dhcp6_list *ial = NULL, pl;

	TAILQ_INIT(&pl);
	for (sp = TAILQ_FIRST(&iac_pd->siteprefix_head); sp;
	    sp = TAILQ_NEXT(sp, link)) {
		if (dhcp6_add_listval(&pl, DHCP6_LISTVAL_PREFIX6,
		    &sp->prefix, NULL) == NULL)
			goto fail;
	}

	if ((ial = malloc(sizeof(*ial))) == NULL)
		goto fail;
	TAILQ_INIT(ial);
	if (dhcp6_add_listval(ial, DHCP6_LISTVAL_IAPD, iaparam, &pl) == NULL)
		goto fail;
	dhcp6_clear_list(&pl);

	evd->type = DHCP6_EVDATA_IAPD;
	evd->data = (void *)ial;
	evd->privdata = (void *)evdp;
	evd->destructor = renew_data_free;

	return (0);

  fail:
	dhcp6_clear_list(&pl);
	if (ial)
		free(ial);
	return (-1);
}

static void
renew_data_free(evd)
	struct dhcp6_eventdata *evd;
{
	struct dhcp6_list *ial;

	if (evd->type != DHCP6_EVDATA_IAPD) {
		dprintf(LOG_ERR, FNAME, "assumption failure");
		exit(1);
	}

	if (evd->privdata)
		*(struct dhcp6_eventdata **)evd->privdata = NULL;
	ial = (struct dhcp6_list *)evd->data;
	dhcp6_clear_list(ial);
	free(ial);
}

static struct dhcp6_timer *
siteprefix_timo(arg)
	void *arg;
{
	struct siteprefix *sp = (struct siteprefix *)arg;
	struct ia *ia;
	void (*callback)__P((struct ia *));

	dprintf(LOG_DEBUG, FNAME, "prefix timeout for %s/%d",
	    in6addr2str(&sp->prefix.addr, 0), sp->prefix.plen);

	ia = sp->ctl->iacpd_ia;
	callback = sp->ctl->iacpd_callback;

	if (sp->timer)
		dhcp6_remove_timer(&sp->timer);

	remove_siteprefix(sp);

	(*callback)(ia);

	return (NULL);
}





static int
add_ifprefix(siteprefix, prefix, pconf)
	struct siteprefix *siteprefix;
	struct dhcp6_prefix *prefix;
	struct prefix_ifconf *pconf;
{
	struct dhcp6_ifprefix *ifpfx = NULL;
	struct in6_addr *a;
	u_long sla_id;
	char *sp;
	int b, i;

	if ((ifpfx = malloc(sizeof(*ifpfx))) == NULL) {
		dprintf(LOG_NOTICE, FNAME,
		    "failed to allocate memory for ifprefix");
		return (-1);
	}
	memset(ifpfx, 0, sizeof(*ifpfx));

	ifpfx->ifconf = pconf;

	ifpfx->paddr.sin6_family = AF_INET6;
#ifdef HAVE_SA_LEN
	ifpfx->paddr.sin6_len = sizeof(struct sockaddr_in6);
#endif
	ifpfx->paddr.sin6_addr = prefix->addr;
	ifpfx->plen = prefix->plen + pconf->sla_len;
	/*
	 * XXX: our current implementation assumes ifid len is a multiple of 8
	 */
	if ((pconf->ifid_len % 8) != 0) {
		dprintf(LOG_ERR, FNAME,
		    "assumption failure on the length of interface ID");
		goto bad;
	}

	if (ifpfx->plen + pconf->ifid_len < 0 ||
	    ifpfx->plen + pconf->ifid_len > 128) {
		dprintf(LOG_INFO, FNAME,
			"invalid prefix length %d + %d + %d",
			prefix->plen, pconf->sla_len, pconf->ifid_len);
		goto bad;
	}

	/* copy prefix and SLA ID */
	a = &ifpfx->paddr.sin6_addr;
	b = prefix->plen;

    for (i = 0, b = prefix->plen; b > 0; b -= 8, i++)
		a->s6_addr[i] = prefix->addr.s6_addr[i];
    
	sla_id = htonl(pconf->sla_id);
	sp = ((char *)&sla_id + 3);
	i = (128 - pconf->ifid_len) / 8;
	for (b = pconf->sla_len; b > 7; b -= 8, sp--)
		a->s6_addr[--i] = *sp;
	if (b)
		a->s6_addr[--i] |= *sp;

	/* configure the corresponding address */
	ifpfx->ifaddr = ifpfx->paddr;
	for (i = 15; i >= pconf->ifid_len / 8; i--)
		ifpfx->ifaddr.sin6_addr.s6_addr[i] = pconf->ifid[i];
	if (pd_ifaddrconf(IFADDRCONF_ADD, ifpfx))
		goto bad;

	/* TODO: send a control message for other processes */

	TAILQ_INSERT_TAIL(&siteprefix->ifprefix_list, ifpfx, plink);

	return (0);

  bad:
	if (ifpfx)
		free(ifpfx);
	return (-1);
}

#ifndef ND6_INFINITE_LIFETIME
#define ND6_INFINITE_LIFETIME 0xffffffff
#endif

#if 0
static int if_prefixaddradd(struct dhcp6_ifprefix *ifpfx)
{
	struct prefix_ifconf *pconf;
	pconf = ifpfx->ifconf;
    return (ifaddrconfAdd(&ifpfx->ifaddr.sin6_addr, pconf->ifname));
}
static int
pd_ifaddrconf(cmd, ifpfx)
	ifaddrconf_cmd_t cmd;
	struct dhcp6_ifprefix *ifpfx;
{
	struct prefix_ifconf *pconf;

	pconf = ifpfx->ifconf;

	return (ifaddrconf(cmd, pconf->ifname, &ifpfx->ifaddr, ifpfx->plen, 
	    ND6_INFINITE_LIFETIME, ND6_INFINITE_LIFETIME));
}

#else
static int if_prefixaddradd(struct dhcp6_ifprefix *ifpfx)
{
	struct dhcp6_if *pstIfp;
	pstIfp = ifpfx->pstIfp;
    
    return (ifaddrconfAdd(&ifpfx->ifaddr.sin6_addr, pstIfp->ifname));
}
#endif

static int
pd_ifaddrconf(ifaddrconf_cmd_t cmd,
	struct dhcp6_ifprefix *ifpfx)
{
	struct dhcp6_if *pstIfp;

	pstIfp = ifpfx->pstIfp;

    return (ifaddrconf(cmd, pstIfp->ifname, &ifpfx->ifaddr, 128, 
        ND6_INFINITE_LIFETIME, ND6_INFINITE_LIFETIME));
}
