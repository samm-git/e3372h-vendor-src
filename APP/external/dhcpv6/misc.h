#ifndef __MISC_H__
#define __MISC_H__
/*
  <问题单号>     <作  者>    <修改时间>   <版本> <修改描述>
    2032203980  m00182044   2012/6/10 DHCP6c不能通过RA进行无状态IPv6地址配置 
	2012.11.16      y00186923                               9615平台代码差异合入
 */

/*BEGIN 2032203980  m00182044 2012-06-10 modify*/
#include "np_log.h"
/*BEGIN PN:2071008409 l00170266 for var problem modify */
#include "netprotoapi.h"
/*END PN:2071008409 l00170266 for var problem modify */

extern int exit_addrMonitor;
extern int exit_ok;
extern int logged;

#define     FUNC        __FUNCTION__
#define     FLINE       __LINE__

/* start of AU4D02274 by d00107688 2009-12-02 PPPoe+SLAAC方式无法获取DNS信息 */
#define DHCP6C_START_BY_RA      (0x01)
/* end of AU4D02274 by d00107688 2009-12-02 PPPoe+SLAAC方式无法获取DNS信息 */

#define IPV6_ADDR_MULTICAST         0x01
#define IPV6_ADDR_SCOPE_LINKLOCAL	0x02
#define IPV6_ADDR_SCOPE_GLOBAL		0x04

#define LOCAL_LOOPBACK_PREFIX   htonl(0x00000000)
#define LOCAL_ADDR_PREFIX       htonl(0xfe800000)
#define UNI_LOCAL_ADDR_PREFIX   htonl(0xfc000000)
#define MUTICAST_ADDR_PREFIX    htonl(0xff000000)

#define ARRAY_SIZE(x)       (sizeof(x)/sizeof(x[0]))


#define DHCP_IAPD_NUM_BASE                  (200)
#define DHCP_IANA_NUM_BASE                  (2000)

#define ACCEPT_RA_FILE_PATH                 "/proc/sys/net/ipv6/conf/%s/accept_ra"

#define ACCEPT_FORWARDING_FILE_PATH                 "/proc/sys/net/ipv6/conf/%s/forwarding"

#define ACCEPT_RA_AUTOCONF_FILE_PATH        "/proc/sys/net/ipv6/conf/%s/autoconf"


/* start of by d00107688 2010-01-26 增加支持是否允许添加默认路由 */
#define ACCEPT_RA_DLF_ROUTE_FILE_PATH       "/proc/sys/net/ipv6/conf/%s/accept_ra_defrtr"
/* end of by d00107688 2010-01-26 增加支持是否允许添加默认路由 */

#define INET6_ADDR_FILE                     "/proc/net/if_inet6"

#define INET6_ROUTE_FILE                    "/proc/net/ipv6_route"

/*BEGIN PN:2071008409 l00170266 for var problem modify */
#define WAN_DIAL6_FILE                      (ROUTER_VARPATH_PREFIX "/wan/%s/dial6")

#define WAN_NOTIFY_FILE                     (ROUTER_VARPATH_PREFIX "/wan/%s/notify6")

#define WAN_STATUS6_FILE                    (ROUTER_VARPATH_PREFIX "/wan/%s/status6")

/* 默认网关 */
#define WAN_GW6_FILE                        (ROUTER_VARPATH_PREFIX "/wan/%s/gateway6")

/* DNS信息 */
#define WAN_DNS6_FILE                       (ROUTER_VARPATH_PREFIX "/wan/%s/dns6")

/*MTU 信息*/
#define WAN_RA_MTU_FILE                   (ROUTER_VARPATH_PREFIX "/wan/%s/mtu6")

/*prefix ValidLifeTime 信息*/
#define WAN_PREFIX_VALID_TIME_FILE                   (ROUTER_VARPATH_PREFIX "/wan/%s/validtime6")

/*prefix PreferredLifeTime 信息*/
#define WAN_PREFIX_PREFER_TIME_FILE                   (ROUTER_VARPATH_PREFIX "/wan/%s/prefertime6")

/*RA里的 Managed Flag*/
#define WAN_RA_M_FLAG_FILE                   (ROUTER_VARPATH_PREFIX "/wan/%s/M_flag")
/*RA里的 Other Flag*/
#define WAN_RA_O_FLAG_FILE                   (ROUTER_VARPATH_PREFIX "/wan/%s/O_flag")

/* IP地址列表 */
#define WAN_IPADDR6_FILE                   (ROUTER_VARPATH_PREFIX "/wan/%s/ipaddr6")

#define WAN_PREFIX_FILE                     (ROUTER_VARPATH_PREFIX "/wan/%s/prefix6")

/* DHCP DSLITE 文件 */
#define WAN_DHCPC6_DSLITE                   (ROUTER_VARPATH_PREFIX "/wan/%s/dhcpdslite")

/* DHCP DSLITE 域名文件 */
#define WAN_DHCPC6_DSLITENAME               (ROUTER_VARPATH_PREFIX "/wan/%s/dhcpdslitename")

/* DHCP DNS 文件 */
#define WAN_DHCPC6_DNS                      (ROUTER_VARPATH_PREFIX "/wan/%s/dhcpdns6")

/* DHCP 前缀信息 */
#define WAN_DHCPC6_PREFIX                   (ROUTER_VARPATH_PREFIX "/wan/%s/dhcpprefix6")


#define WAN_DHCP6C_CFG                      (ROUTER_VARPATH_PREFIX "/wan/%s/dhcp6c.conf")


#define DHCP6C_PIDFILE                      (ROUTER_VARPATH_PREFIX "/wan/%s/dhcp6c.pid")
/*END PN:2071008409 l00170266 for var problem modify */
#define NP_DHCP_SERVER_FATAL(msg, args...)  NP_LOG_FATAL("dhcp", msg, ##args)
#define NP_DHCP_SERVER_ERROR(msg, args...)  NP_LOG_ERROR("dhcp", msg, ##args)
#define NP_DHCP_SERVER_WARN(msg, args...)   NP_LOG_WARN("dhcp", msg, ##args)
#define NP_DHCP_SERVER_INFO(msg, args...)   NP_LOG_INFO("dhcp", msg, ##args)
#define NP_DHCP_SERVER_DEBUG(msg, args...)  NP_LOG_DEBUG("dhcp", msg, ##args)

#define NIP6(addr) \
	ntohs((addr).s6_addr16[0]), \
	ntohs((addr).s6_addr16[1]), \
	ntohs((addr).s6_addr16[2]), \
	ntohs((addr).s6_addr16[3]), \
	ntohs((addr).s6_addr16[4]), \
	ntohs((addr).s6_addr16[5]), \
	ntohs((addr).s6_addr16[6]), \
	ntohs((addr).s6_addr16[7])
#define NIP6_FMT "%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x"
#define NIP6_SEQFMT "%04x%04x%04x%04x%04x%04x%04x%04x"



#define FUNCBEGIN()                         //loginfo(FUNC, "%d----start-------------", FLINE);
#define FUNCEND()                           //loginfo(FUNC, "%d ----end---------------\r\n", FLINE);

int ComparePrefix(struct in6_addr stAddr1, int iPrefixLen1,
            struct in6_addr stAddr2, int iPrefixLen2);

long SysUpTime(void);
int  IPv6AddrType(const struct in6_addr stAddr);

int SetAcceptRA(char *pszIfName, int iValue);

int SetAcceptForwarding(char *pszIfName, int iValue);
/*END 2032203980  m00182044 2012-06-10 modify*/

int SetAutoConf(char *pszIfName, int iValue);

int SetWanStatus(int iIfindex, int iValue);

void NotifyWANStatus(int iIfindex);

void WriteIPIntoFile(FILE *fp, struct in6_addr stAddr, int iPrefix);

void WriteIPLinkFlagIntoFile(FILE *fp, struct in6_addr stAddr, int iPrefixLen, int OnLinkFlag);

char *WriteDHCP6CCfg(char *pszIfname, int iIANAFlag, int iIAPDFlag, int iDsliteFlag);

int StartDhcp6c(int iIfindex, int iIANAFlag, int iIAPDFlag, int iDsliteFlag);
int StopDhcp6c(int iIfindex, int iFlags);

void loginfo(const char *fname, const char *fmt, ...);

int ExitProcessID(int iIfindex);
//int GetPrevProcessEixtStatus(int iIfindex);
int GetPrevProcessEixtStatus(char *pszIfname);

void ExitProcess(int signo);





#endif
