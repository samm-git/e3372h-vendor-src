/*
 *   $Id: pathnames.h,v 1.1 2009/08/22 07:51:00 y42304 Exp $
 *
 *   Authors:
 *    Pedro Roque		<roque@di.fc.ul.pt>
 *    Lars Fenneberg		<lf@elemental.net>	 
 *
 *   This software is Copyright 1996,1997 by the above mentioned author(s), 
 *   All Rights Reserved.
 *
 *   The license which is distributed with this software in the file COPYRIGHT
 *   applies to this software. If your distribution is missing this file, you
 *   may request it from <pekkas@netcore.fi>.
 *
 */
 /**********************问题单修改记录******************************************
    日期              修改人         问题单号           修改内容
 2012.11.16        y00186923                         9615平台代码差异合入
******************************************************************************/
#ifndef PATHNAMES_H
#define PATHNAMES_H

#ifndef PATH_RADVD_CONF
#define PATH_RADVD_CONF "/etc/radvd.conf"
#endif

/*BEGIN PN:2071008409 l00170266 for var problem modify 20120725 */
#ifndef PATH_RADVD_PID
#define PATH_RADVD_PID (ROUTER_VARPATH_PREFIX "/run/radvd.pid")
#endif

#ifndef PATH_RADVD_LOG
#define PATH_RADVD_LOG (ROUTER_VARPATH_PREFIX "/log/radvd.log")
#endif
/*END PN:2071008409 l00170266 for var problem modify 20120725 */

#define PATH_PROC_NET_IF_INET6 "/proc/net/if_inet6"
#define PATH_PROC_NET_IGMP6 "/proc/net/igmp6"

#ifdef __linux__
#define SYSCTL_IP6_FORWARDING CTL_NET, NET_IPV6, NET_IPV6_CONF, NET_PROTO_CONF_ALL, NET_IPV6_FORWARDING
#define PROC_SYS_IP6_FORWARDING "/proc/sys/net/ipv6/conf/all/forwarding"
#define PROC_SYS_IP6_LINKMTU "/proc/sys/net/ipv6/conf/%s/mtu"
#define PROC_SYS_IP6_CURHLIM "/proc/sys/net/ipv6/conf/%s/hop_limit"
#define PROC_SYS_IP6_BASEREACHTIME_MS "/proc/sys/net/ipv6/neigh/%s/base_reachable_time_ms"
#define PROC_SYS_IP6_BASEREACHTIME "/proc/sys/net/ipv6/neigh/%s/base_reachable_time"
#define PROC_SYS_IP6_RETRANSTIMER_MS "/proc/sys/net/ipv6/neigh/%s/retrans_time_ms"
#define PROC_SYS_IP6_RETRANSTIMER "/proc/sys/net/ipv6/neigh/%s/retrans_time"
#else /* BSD */
#define SYSCTL_IP6_FORWARDING CTL_NET, PF_INET6, IPPROTO_IPV6, IPV6CTL_FORWARDING
#endif

#endif
