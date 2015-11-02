/* $Id: upnpglobalvars.c,v 1.25 2011/05/27 21:36:22 nanard Exp $ */
/* MiniUPnP project
 * http://miniupnp.free.fr/ or http://miniupnp.tuxfamily.org/
 * (c) 2006-2010 Thomas Bernard 
 * This software is subject to the conditions detailed
 * in the LICENCE file provided within the distribution */

   /**********************问题单修改记录******************************************
    日期              修改人         问题单号           修改内容

 2012.08.29        z00203875     2082304944    UPnP认证测试
 2012.11.16        y00186923                         9615平台代码差异合入
******************************************************************************/


#include <sys/types.h>
#include <netinet/in.h>

#include "config.h"
#include "upnpglobalvars.h"

/*BEGIN PN:2071008409 l00170266 for var problem modify 20120725 */
#define UPNP_PID  (ROUTER_VARPATH_PREFIX "/upnp/pid")
#define UPNP_MINISSDPDSOCK (ROUTER_VARPATH_PREFIX "/run/minissdpd.sock")
/*END PN:2071008409 l00170266 for var problem modify 20120725 */

/* network interface for internet */
const char * ext_if_name = 0;

/* file to store leases */
#ifdef ENABLE_LEASEFILE
const char* lease_file = 0;
#endif

/* forced ip address to use for this interface
 * when NULL, getifaddr() is used */
const char * use_ext_ip_addr = 0;

/* LAN address */
/*const char * listen_addr = 0;*/

unsigned long downstream_bitrate = 0;
unsigned long upstream_bitrate = 0;

/* startup time */
time_t startup_time = 0;

int runtime_flags = 0;
/*BEGIN PN:2071008409 l00170266 for var problem modify 20120725 */
const char * pidfilename = UPNP_PID;
/*END PN:2071008409 l00170266 for var problem modify 20120725 */

char uuidvalue[] = "uuid:00000000-0000-0000-0000-000000000000";
/* BEGIN 2082304944 zhoujianchun 00203875 2012.8.29 added */
char uuidvalue_root[] = "uuid:00000000-0000-0000-0000-000000000000";

char uuidvalue_wan[] = "uuid:00000000-0000-0000-0000-000000000001";
char uuidvalue_wan_common[] = "uuid:00000000-0000-0000-0000-000000000001";

char uuidvalue_wan_conn[] = "uuid:00000000-0000-0000-0000-000000000002";
char uuidvalue_wan_ipconn[] = "uuid:00000000-0000-0000-0000-000000000002";

char *uuid_value[MAX_KNOWN_SERVICE] = {0};
/* END   2082304944 zhoujianchun 00203875 2012.8.29 added */
char serialnumber[SERIALNUMBER_MAX_LEN] = "00000000";

char modelnumber[MODELNUMBER_MAX_LEN] = "1";

/* presentation url :
 * http://nnn.nnn.nnn.nnn:ppppp/  => max 30 bytes including terminating 0 */
char presentationurl[PRESENTATIONURL_MAX_LEN];

/* UPnP permission rules : */
struct upnpperm * upnppermlist = 0;
unsigned int num_upnpperm = 0;

#ifdef ENABLE_NATPMP
/* NAT-PMP */
#if 0
unsigned int nextnatpmptoclean_timestamp = 0;
unsigned short nextnatpmptoclean_eport = 0;
unsigned short nextnatpmptoclean_proto = 0;
#endif
#endif

/* For automatic removal of expired rules (with LeaseDuration) */
unsigned int nextruletoclean_timestamp = 0;

#ifdef USE_PF
const char * queue = 0;
const char * tag = 0;
#endif

#ifdef USE_NETFILTER
/* chain name to use, both in the nat table
 * and the filter table */
const char * miniupnpd_nat_chain = "MINIUPNPD";
const char * miniupnpd_forward_chain = "MINIUPNPD";
#endif
#ifdef ENABLE_NFQUEUE
int nfqueue = -1;
int n_nfqix = 0;
unsigned nfqix[MAX_LAN_ADDR];
#endif
struct lan_addr_list lan_addrs;

#ifdef ENABLE_IPV6
/* ipv6 address used for HTTP */
char ipv6_addr_for_http_with_brackets[64];
#endif

/* Path of the Unix socket used to communicate with MiniSSDPd */
/*BEGIN PN:2071008409 l00170266 for var problem modify 20120725 */
const char * minissdpdsocketpath = UPNP_MINISSDPDSOCK;
/*END PN:2071008409 l00170266 for var problem modify 20120725 */

/* BOOTID.UPNP.ORG and CONFIGID.UPNP.ORG */
unsigned int upnp_bootid = 1;
unsigned int upnp_configid = 1337;

#ifdef ENABLE_6FC_SERVICE
int ipv6fc_firewall_enabled = 1;
int ipv6fc_inbound_pinhole_allowed = 1;
#endif

