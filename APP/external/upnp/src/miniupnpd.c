/* $Id: miniupnpd.c,v 1.138 2011/05/27 21:58:12 nanard Exp $ */
/* MiniUPnP project
 * http://miniupnp.free.fr/ or http://miniupnp.tuxfamily.org/
 * (c) 2006-2011 Thomas Bernard
 * This software is subject to the conditions detailed
 * in the LICENCE file provided within the distribution */
 
   /**********************问题单修改记录******************************************
    日期              修改人         问题单号           修改内容
  
 2012.03.14        z00203875     2031401440    修改打LOG方式  
 2012.03.23        z00203875     2032102417    退出时清除规则
 2012.04.27        z00203875     2041707023    Add LAN IP Restriction
 2012.05.16        z00203875     2051600916    Disable WAN Port Listen
 2012.07.06        z00203875     2061908201    图标低概率不出现
 2012.08.29        z00203875     2082304944    UPnP认证测试
 2012/8/1          h00195629     2071909199    wan和lanip冲突处理
 2012.11.16        y00186923                         9615平台代码差异合入
  2012.12.24        y44942        2121101411    ??Fortify??
 2012.12.22        s00210128     2122200601    优化log打印和xml读取
 2013.08.23        y00248130     3082101267    修改时间的获取方式，使之不受时间管理特性的影响
 2014.06.10        y00248130     4062600669    PCP特性
******************************************************************************/

#include "config.h"

/* Experimental support for NFQUEUE interfaces */
#ifdef ENABLE_NFQUEUE
/* apt-get install libnetfilter-queue-dev */
#include <netinet/ip.h>
#include <netinet/udp.h>
//#include <linux/netfilter_ipv4.h>  /* Defines verdicts (NF_ACCEPT, etc) */
#include <linux/netfilter.h>
#include <libnetfilter_queue/libnetfilter_queue.h>
#include <linux/netfilter/nfnetlink_queue.h>
#endif

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
/*BEGIN 2041707023 zhoujianchun 00203875 2012.4.27 added */
#include <strings.h>
/*END   2041707023 zhoujianchun 00203875 2012.4.27 added */
#include <errno.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>
/*BEGIN 2041707023 zhoujianchun 00203875 2012.4.27 added */
#include <sys/ioctl.h>
/*END   2041707023 zhoujianchun 00203875 2012.4.27 added */
#include <sys/socket.h>
/*BEGIN 2041707023 zhoujianchun 00203875 2012.4.27 added */
#include <net/if.h>
/*END   2041707023 zhoujianchun 00203875 2012.4.27 added */
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/file.h>
#include <sys/time.h>
#include <time.h>
#include <signal.h>
#include <errno.h>
#include <sys/param.h>
#if defined(sun)
#include <kstat.h>
#else
/* for BSD's sysctl */
//#include <sys/sysctl.h>
#endif

/* unix sockets */
#ifdef USE_MINIUPNPDCTL
#include <sys/un.h>
#endif

/*BEGIN 3082101267 y00248130 2013-8-23 added/modified*/
#include <sys/sysinfo.h> 
/* END  3082101267 y00248130 2013-8-23 added/modified*/

/*BEGIN 2041707023 zhoujianchun 00203875 2012.4.27 added */
#include "upnppermissions.h"
/*END   2041707023 zhoujianchun 00203875 2012.4.27 added */

#include "upnpglobalvars.h"
#include "upnphttp.h"
#include "upnpdescgen.h"
#include "miniupnpdpath.h"
#include "getifaddr.h"
#include "upnpsoap.h"
#include "options.h"
#include "minissdp.h"
#include "upnpredirect.h"
#include "miniupnpdtypes.h"
#include "daemonize.h"
#include "upnpevents.h"
#ifdef ENABLE_NATPMP
#include "natpmp.h"
#endif
#include "commonrdr.h"
#include "upnputils.h"
#ifdef USE_IFACEWATCHER
#include "ifacewatcher.h"
#endif

#include "upnpdescstrings.h"

#ifndef DEFAULT_CONFIG
#define DEFAULT_CONFIG "/etc/miniupnpd.conf"
#endif

#ifndef USE_MOCKER
#include "appinterface.h"
#endif

#include "xmlapi.h"

#ifdef MBB_FEATURE_PCP
#include "appinterface.h"
#include "pcp_lib.h"
#include "wanapi.h"
#endif

/*BEGIN PN:2071008409 l00170266 for var problem modify 20120725 */
#define UPNP_KILL_CMD ("/system/bin/kill `/system/bin/cat" ROUTER_VARPATH_PREFIX "/upnp/pid`")
#define UPNP_MKDIR_ROUTER_DIR_CMD ("/system/bin/mkdir " ROUTER_VARPATH_PREFIX)
#define UPNP_MKDIR_DIR_CMD ("/system/bin/mkdir " ROUTER_VARPATH_PREFIX "/upnp")
#define UPNP_MINIUPNPCTL (ROUTER_VARPATH_PREFIX "/run/miniupnpd.ctl")
/*END PN:2071008409 l00170266 for var problem modify 20120725 */

/*BEGIN 2041707023 zhoujianchun 00203875 2012.4.27 added */
#define IP_ADDR_LEN 4
#define IP_ADDR_STRING_LEN 32 /*dotted decimal notation, 32 is enough */ 
#define MAX_PORT_NUMBER 65535
#define MIN_PORT_NUMBER 0
/*END   2041707023 zhoujianchun 00203875 2012.4.27 added */

void load_upnp_cfg(UPNP_CFG_ST *cfg);

#ifdef USE_MINIUPNPDCTL
struct ctlelem {
	int socket;
	LIST_ENTRY(ctlelem) entries;
};
#endif

#ifdef ENABLE_NFQUEUE
/* globals */
static struct nfq_handle *nfqHandle;
static struct sockaddr_in ssdp;

/* prototypes */
static int nfqueue_cb( struct nfq_q_handle *qh, struct nfgenmsg *nfmsg, struct nfq_data *nfa, void *data) ;
int identify_ip_protocol (char *payload);
int get_udp_dst_port (char *payload);
#endif

/* variables used by signals */
static volatile int quitting = 0;
volatile int should_send_public_address_change_notif = 0;
#ifdef MBB_FEATURE_PCP
volatile sig_atomic_t should_update_pcp = 0; 
#endif
/*BEGIN 2041707023 zhoujianchun 00203875 2012.4.27 added */
int build_permission_rule(void);
/*END   2041707023 zhoujianchun 00203875 2012.4.27 added */

/*BEGIN 3082101267 y00248130 2013-8-23 added/modified*/
time_t get_boot_time(void);  
/* END  3082101267 y00248130 2013-8-23 added/modified*/

/* OpenAndConfHTTPSocket() :
 * setup the socket used to handle incoming HTTP connections. */
static int
OpenAndConfHTTPSocket(unsigned short port)
{
	int s;
	int i = 1;
    /*BEGIN 2051600916 zhoujianchun 00203875 2012.5.16 added */
    const char *ifname = "br0";
    int sock_fd = 0;
    struct ifreq ifr;
    int ifrlen = 0;
    struct sockaddr_in * addr = NULL;
    ifrlen = sizeof(ifr);
    int ret = 0;
    /*END   2051600916 zhoujianchun 00203875 2012.5.16 added */

#ifdef ENABLE_IPV6
	struct sockaddr_in6 listenname;
#else
	struct sockaddr_in listenname;
#endif
	socklen_t listenname_len;

	if( (s = socket(
#ifdef ENABLE_IPV6
	                PF_INET6,
#else
	                PF_INET,
#endif
	                SOCK_STREAM, 0)) < 0)
	{
		NP_UPNP_ERROR("socket(http): %s\n", strerror(errno));
		return -1;
	}

	if(setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(i)) < 0)
	{
		NP_UPNP_ERROR("setsockopt(http, SO_REUSEADDR): %s\n", strerror(errno));
	}
#if 0
	/* enable this to force IPV6 only for IPV6 socket.
	 * see http://www.ietf.org/rfc/rfc3493.txt section 5.3 */
	if(setsockopt(s, IPPROTO_IPV6, IPV6_V6ONLY, &i, sizeof(i)) < 0)
	{
		NP_UPNP_ERROR("setsockopt(http, IPV6_V6ONLY): %\n", strerror(errno));
	}
#endif
    /*BEGIN 2051600916 zhoujianchun 00203875 2012.5.16 added */
    sock_fd = socket(PF_INET, SOCK_DGRAM, 0);
    if(0 > sock_fd)
    {
        NP_UPNP_ERROR("%s\n", strerror(errno));
        close(s);
        return -1;
    }
    strncpy(ifr.ifr_name, ifname, IFNAMSIZ);
    ret = ioctl(sock_fd, SIOCGIFADDR, &ifr, &ifrlen);
    if(0 > ret)
    {
        NP_UPNP_ERROR("%s\n", strerror(errno));
        close(sock_fd);
        close(s);
        return -1;
    }
    addr = (struct sockaddr_in *)&(ifr.ifr_addr);
    char *tmp_address = (char*)addr;
    struct in_addr address;
    memcpy(&(address), tmp_address + IP_ADDR_LEN, IP_ADDR_LEN);
    
    close(sock_fd);
    /*END   2051600916 zhoujianchun 00203875 2012.5.16 added */

#ifdef ENABLE_IPV6
	memset(&listenname, 0, sizeof(struct sockaddr_in6));
	listenname.sin6_family = AF_INET6;
	listenname.sin6_port = htons(port);
	listenname.sin6_addr = in6addr_any;
	listenname_len =  sizeof(struct sockaddr_in6);
#else
	listenname.sin_family = AF_INET;
	listenname.sin_port = htons(port);
    /*BEGIN 2051600916 zhoujianchun 00203875 2012.5.16 modified */
	listenname.sin_addr = address;
    /*END   2051600916 zhoujianchun 00203875 2012.5.16 modified */
	listenname_len = sizeof(struct sockaddr_in);
#endif

	if(bind(s, (struct sockaddr *)&listenname, listenname_len) < 0)
	{
		NP_UPNP_ERROR("bind(http): %s\n", strerror(errno));
		close(s);
		return -1;
	}

	if(listen(s, 5) < 0)
	{
		NP_UPNP_ERROR("listen(http): %s\n", strerror(errno));
		close(s);
		return -1;
	}

	return s;
}
#ifdef ENABLE_NFQUEUE

int identify_ip_protocol(char *payload) {
    return payload[9];
}


/*
 * This function returns the destination port of the captured packet UDP
 */
int get_udp_dst_port(char *payload) {
        char *pkt_data_ptr = NULL;
        pkt_data_ptr = payload + sizeof(struct ip);

    /* Cast the UDP Header from the raw packet */
    struct udphdr *udp = (struct udphdr *) pkt_data_ptr;

    /* get the dst port of the packet */
    return(ntohs(udp->dest));

}
static int
OpenAndConfNFqueue(){

        struct nfq_q_handle *myQueue;
        struct nfnl_handle *netlinkHandle;

        int fd = 0, e = 0;

	inet_pton(AF_INET, "239.255.255.250", &(ssdp.sin_addr));

        //Get a queue connection handle from the module
        if (!(nfqHandle = nfq_open())) {
		NP_UPNP_ERROR("Error in nfq_open(): %s\n", strerror(errno));
                return -1;
        }

        //Unbind the handler from processing any IP packets
        //      Not totally sure why this is done, or if it's necessary...
        if ((e = nfq_unbind_pf(nfqHandle, AF_INET)) < 0) {
		NP_UPNP_ERROR("Error in nfq_unbind_pf(): %s\n", strerror(errno));
                return -1;
        }

        //Bind this handler to process IP packets...
        if (nfq_bind_pf(nfqHandle, AF_INET) < 0) {
		NP_UPNP_ERROR("Error in nfq_bind_pf(): %s\n", strerror(errno));
                return -1;
        }

        //      Install a callback on queue -Q
        if (!(myQueue = nfq_create_queue(nfqHandle,  nfqueue, &nfqueue_cb, NULL))) {
		NP_UPNP_ERROR("Error in nfq_create_queue(): %s\n", strerror(errno));
                return -1;
        }

        //      Turn on packet copy mode
        if (nfq_set_mode(myQueue, NFQNL_COPY_PACKET, 0xffff) < 0) {
		NP_UPNP_ERROR("Error setting packet copy mode (): %s\n", strerror(errno));
                return -1;
        }

        netlinkHandle = nfq_nfnlh(nfqHandle);
        fd = nfnl_fd(netlinkHandle);

	return fd;

}


static int nfqueue_cb(
                struct nfq_q_handle *qh,
                struct nfgenmsg *nfmsg,
                struct nfq_data *nfa,
                void *data) {

	char	*pkt;
	struct nfqnl_msg_packet_hdr *ph;
	ph = nfq_get_msg_packet_hdr(nfa);

	if ( ph ) {

		int id = 0, size = 0;
		id = ntohl(ph->packet_id);

		size = nfq_get_payload(nfa, &pkt);

    		struct ip *iph = (struct ip *) pkt;

		int id_protocol = identify_ip_protocol(pkt);

		int dport = get_udp_dst_port(pkt);

		int x = sizeof (struct ip) + sizeof (struct udphdr);
	
		/* packets we are interested in are UDP multicast to 239.255.255.250:1900	
		 * and start with a data string M-SEARCH
		 */
		if ( (dport == 1900) && (id_protocol == IPPROTO_UDP) 
			&& (ssdp.sin_addr.s_addr == iph->ip_dst.s_addr) ) {
		
			/* get the index that the packet came in on */
			u_int32_t idx = nfq_get_indev(nfa);
			int i = 0;
			for ( ;i < n_nfqix ; i++) {
				if ( nfqix[i] == idx ) {

					struct udphdr *udp = (struct udphdr *) (pkt + sizeof(struct ip));

					char *dd = pkt + x;
					
					struct sockaddr_in sendername;
					sendername.sin_family = AF_INET;
					sendername.sin_port = udp->source;
					sendername.sin_addr.s_addr = iph->ip_src.s_addr;

					/* printf("pkt found %s\n",dd);*/
					ProcessSSDPData (sudp, dd, size - x,
					                 &sendername, (unsigned short) 5555);
				}
			}
		}
		
		nfq_set_verdict(qh, id, NF_ACCEPT, 0, NULL);

	} else {
		NP_UPNP_ERROR("nfq_get_msg_packet_hdr failed\n");
		return 1; // from nfqueue source: 0 = ok, >0 = soft error, <0 hard error
	}

	return 0;
}

static void ProcessNFQUEUE(int fd){
	char buf[4096];

	socklen_t len_r;
	struct sockaddr_in sendername;
	len_r = sizeof(struct sockaddr_in);

        int res = recvfrom(fd, buf, sizeof(buf), 0,
			(struct sockaddr *)&sendername, &len_r);

	nfq_handle_packet(nfqHandle, buf, res);
}
#endif

/* Functions used to communicate with miniupnpdctl */
#ifdef USE_MINIUPNPDCTL
static int
OpenAndConfCtlUnixSocket(const char * path)
{
	struct sockaddr_un localun;
	int s;
	s = socket(AF_UNIX, SOCK_STREAM, 0);
	localun.sun_family = AF_UNIX;
	strncpy(localun.sun_path, path,
	          sizeof(localun.sun_path));
	if(bind(s, (struct sockaddr *)&localun,
	        sizeof(struct sockaddr_un)) < 0)
	{
		NP_UPNP_ERROR("bind(sctl): %s\n", strerror(errno));
		close(s);
		s = -1;
	}
	else if(listen(s, 5) < 0)
	{
		NP_UPNP_ERROR("listen(sctl): %s\n", strerror(errno));
		close(s);
		s = -1;
	}
	return s;
}

static void
write_upnphttp_details(int fd, struct upnphttp * e)
{
	char buffer[256];
	int len;
	write(fd, "HTTP :\n", 7);
	while(e)
	{
		len = snprintf(buffer, sizeof(buffer),
		               "%d %d %s req_buf=%p(%dbytes) res_buf=%p(%dbytes alloc)\n",
		               e->socket, e->state, e->HttpVer,
		               e->req_buf, e->req_buflen,
		               e->res_buf, e->res_buf_alloclen);
		write(fd, buffer, len);
		e = e->entries.le_next;
	}
}

static void
write_ctlsockets_list(int fd, struct ctlelem * e)
{
	char buffer[256];
	int len;
	write(fd, "CTL :\n", 6);
	while(e)
	{
		len = snprintf(buffer, sizeof(buffer),
		               "struct ctlelem: socket=%d\n", e->socket);
		write(fd, buffer, len);
		e = e->entries.le_next;
	}
}

static void
write_option_list(int fd)
{
	char buffer[256];
	int len;
	int i;
	write(fd, "Options :\n", 10);
	for(i=0; i<num_options; i++)
	{
		len = snprintf(buffer, sizeof(buffer),
		               "opt=%02d %s\n",
		               ary_options[i].id, ary_options[i].value);
		write(fd, buffer, len);
	}
}

static void
write_command_line(int fd, int argc, char * * argv)
{
	char buffer[256];
	int len;
	int i;
	write(fd, "Command Line :\n", 15);
	for(i=0; i<argc; i++)
	{
		len = snprintf(buffer, sizeof(buffer),
		               "argv[%02d]='%s'\n",
		                i, argv[i]);
		write(fd, buffer, len);
	}
}

#endif

/* Handler for the SIGTERM signal (kill) 
 * SIGINT is also handled */
static void
sigterm(int sig)
{
	/*int save_errno = errno;*/
	signal(sig, SIG_IGN);	/* Ignore this signal while we are quitting */

	quitting = 1;
	/*errno = save_errno;*/
}

/* Handler for the SIGUSR1 signal indicating public IP address change. */
static void
sigusr1(int sig)
{
	should_send_public_address_change_notif = 1;
}

#ifdef MBB_FEATURE_PCP
/*****************************************************************************
 函数名称  : sigusr2
 功能描述  : 其它进程通过向miniupnpd发送SIGUSR2信号来通知miniupnpd向pcp client进程
             发送现有的规则
 输入参数  : sig
 输出参数  : No
 返 回 值  : No
*****************************************************************************/
static void
sigusr2(int sig)
{
    should_update_pcp = 1;
}
#endif

/* record the startup time, for returning uptime */
static void
set_startup_time(int sysuptime)
{
    /*BEGIN 3082101267 y00248130 2013-8-23 added/modified*/
    startup_time = get_boot_time();
    /* END  3082101267 y00248130 2013-8-23 added/modified*/
    
	if(sysuptime)
	{
		/* use system uptime instead of daemon uptime */
#if defined(__linux__)
		char buff[64];
		int uptime = 0, fd;
		fd = open("/proc/uptime", O_RDONLY);
		if(fd < 0)
		{
			NP_UPNP_ERROR("open(\"/proc/uptime\" : %s\n", strerror(errno));
		}
		else
		{
			memset(buff, 0, sizeof(buff));
			if(read(fd, buff, sizeof(buff) - 1) < 0)
			{
				NP_UPNP_ERROR("read(\"/proc/uptime\" : %s\n", strerror(errno));
			}
			else
			{
				uptime = atoi(buff);
				NP_UPNP_DEBUG("system uptime is %d seconds\n", uptime);
			}
			close(fd);
			startup_time -= uptime;
		}
#elif defined(SOLARIS_KSTATS)
		kstat_ctl_t *kc;
		kc = kstat_open();
		if(kc != NULL)
		{
			kstat_t *ksp;
			ksp = kstat_lookup(kc, "unix", 0, "system_misc");
			if(ksp && (kstat_read(kc, ksp, NULL) != -1))
			{
				void *ptr = kstat_data_lookup(ksp, "boot_time");
				if(ptr)
					memcpy(&startup_time, ptr, sizeof(startup_time));
				else
					NP_UPNP_ERROR("cannot find boot_time kstat\n");
			}
			else
				NP_UPNP_ERROR("cannot open kstats for unix/0/system_misc: %s\n", strerror(errno));
			kstat_close(kc);
		}
#else
		struct timeval boottime;
		size_t size = sizeof(boottime);
		int name[2] = { CTL_KERN, KERN_BOOTTIME };
		if(sysctl(name, 2, &boottime, &size, NULL, 0) < 0)
		{
			NP_UPNP_ERROR("sysctl(\"kern.boottime\") failed\n");
		}
		else
		{
			startup_time = boottime.tv_sec;
		}
#endif
	}
}

/* structure containing variables used during "main loop"
 * that are filled during the init */
struct runtime_vars {
	/* LAN IP addresses for SSDP traffic and HTTP */
	/* moved to global vars */
	int port;	/* HTTP Port */
	int notify_interval;	/* seconds between SSDP announces */
	/* unused rules cleaning related variables : */
	int clean_ruleset_threshold;	/* threshold for removing unused rules */
	int clean_ruleset_interval;		/* (minimum) interval between checks */
};

/* parselanaddr()
 * parse address with mask
 * ex: 192.168.1.1/24
 * When MULTIPLE_EXTERNAL_IP is enabled, the ip address of the
 * external interface associated with the lan subnet follows.
 * ex : 192.168.1.1/24 81.21.41.11
 *
 * return value : 
 *    0 : ok
 *   -1 : error */
static int
parselanaddr(struct lan_addr_s * lan_addr, const char * str)
{
	const char * p;
	int nbits = 24;	/* by default, networks are /24 */
	int n;
	p = str;
	while(*p && *p != '/' && !isspace(*p))
		p++;
	n = p - str;
	if(*p == '/')
	{
		nbits = atoi(++p);
		while(*p && !isspace(*p))
			p++;
	}
	if(n>15)
	{
		fprintf(stderr, "Error parsing address/mask : %s\n", str);
		return -1;
	}
	memcpy(lan_addr->str, str, n);
	lan_addr->str[n] = '\0';
	if(!inet_aton(lan_addr->str, &lan_addr->addr))
	{
		fprintf(stderr, "Error parsing address/mask : %s\n", str);
		return -1;
	}
	lan_addr->mask.s_addr = htonl(nbits ? (0xffffffff << (32 - nbits)) : 0);
#ifdef MULTIPLE_EXTERNAL_IP
	/* skip spaces */
	while(*p && isspace(*p))
		p++;
	if(*p) {
		/* parse the exteral ip address to associate with this subnet */
		n = 0;
		while(p[n] && !isspace(*p))
			n++;
		if(n<=15) {
			memcpy(lan_addr->ext_ip_str, p, n);
			lan_addr->ext_ip_str[n] = '\0';
			if(!inet_aton(lan_addr->ext_ip_str, &lan_addr->ext_ip_addr)) {
				/* error */
				fprintf(stderr, "Error parsing address : %s\n", lan_addr->ext_ip_str);
			}
		}
	}
#endif
	return 0;
}

/* init phase :
 * 1) read configuration file
 * 2) read command line arguments
 * 3) daemonize
 * 4) open syslog
 * 5) check and write pid file
 * 6) set startup time stamp
 * 7) compute presentation URL
 * 8) set signal handlers */
static int
init(int argc, char * * argv, struct runtime_vars * v)
{
	int i;
	int pid;
	int debug_flag = 0;
	int options_flag = 0;
	int openlog_option;
	struct sigaction sa;
	/*const char * logfilename = 0;*/
	const char * presurl = 0;
	const char * optionsfile = DEFAULT_CONFIG;
	struct lan_addr_s * lan_addr;
	struct lan_addr_s * lan_addr2;

	/* only print usage if -h is used */
	for(i=1; i<argc; i++)
	{
		if(0 == strcmp(argv[i], "-h"))
			goto print_usage;
	}
	/* first check if "-f" option is used */
	for(i=2; i<argc; i++)
	{
		if(0 == strcmp(argv[i-1], "-f"))
		{
			optionsfile = argv[i];
			options_flag = 1;
			break;
		}
	}

	/* set initial values */
	SETFLAG(ENABLEUPNPMASK);

	LIST_INIT(&lan_addrs);
	v->port = -1;
	v->notify_interval = 30;	/* seconds between SSDP announces */
	v->clean_ruleset_threshold = 20;
	v->clean_ruleset_interval = 0;	/* interval between ruleset check. 0=disabled */

    /*BEGIN 2041707023 zhoujianchun 00203875 2012.4.27 added */
    build_permission_rule();
    /*END   2041707023 zhoujianchun 00203875 2012.4.27 added */


	/* read options file first since
	 * command line arguments have final say */
	if(readoptionsfile(optionsfile) < 0)
	{
		/* only error if file exists or using -f */
		if(access(optionsfile, F_OK) == 0 || options_flag)
			fprintf(stderr, "Error reading configuration file %s\n", optionsfile);
	}
	else
	{
		for(i=0; i<num_options; i++)
		{
			switch(ary_options[i].id)
			{
			case UPNPEXT_IFNAME:
				ext_if_name = ary_options[i].value;
				break;
			case UPNPEXT_IP:
				use_ext_ip_addr = ary_options[i].value;
				break;
			case UPNPLISTENING_IP:
				lan_addr = (struct lan_addr_s *) malloc(sizeof(struct lan_addr_s));
				if (lan_addr == NULL)
				{
					fprintf(stderr, "malloc(sizeof(struct lan_addr_s)): %m");
					break;
				}
				if(parselanaddr(lan_addr, ary_options[i].value) != 0)
				{
					fprintf(stderr, "can't parse \"%s\" as valid lan address\n", ary_options[i].value);
					free(lan_addr);
					break;
				}
				LIST_INSERT_HEAD(&lan_addrs, lan_addr, list);
				break;
			case UPNPPORT:
				v->port = atoi(ary_options[i].value);
				break;
			case UPNPBITRATE_UP:
				upstream_bitrate = strtoul(ary_options[i].value, 0, 0);
				break;
			case UPNPBITRATE_DOWN:
				downstream_bitrate = strtoul(ary_options[i].value, 0, 0);
				break;
			case UPNPPRESENTATIONURL:
				presurl = ary_options[i].value;
				break;
#ifdef USE_NETFILTER
			case UPNPFORWARDCHAIN:
				miniupnpd_forward_chain = ary_options[i].value;
				break;
			case UPNPNATCHAIN:
				miniupnpd_nat_chain = ary_options[i].value;
				break;
#endif
			case UPNPNOTIFY_INTERVAL:
				v->notify_interval = atoi(ary_options[i].value);
				break;
			case UPNPSYSTEM_UPTIME:
				if(strcmp(ary_options[i].value, "yes") == 0)
					SETFLAG(SYSUPTIMEMASK);	/*sysuptime = 1;*/
				break;
			case UPNPPACKET_LOG:
				if(strcmp(ary_options[i].value, "yes") == 0)
					SETFLAG(LOGPACKETSMASK);	/*logpackets = 1;*/
				break;
			case UPNPUUID:
				strncpy(uuidvalue+5, ary_options[i].value,
				        strlen(uuidvalue+5) + 1);
				break;
			case UPNPSERIAL:
				strncpy(serialnumber, ary_options[i].value, SERIALNUMBER_MAX_LEN);
				serialnumber[SERIALNUMBER_MAX_LEN-1] = '\0';
				break;				
			case UPNPMODEL_NUMBER:
				strncpy(modelnumber, ary_options[i].value, MODELNUMBER_MAX_LEN);
				modelnumber[MODELNUMBER_MAX_LEN-1] = '\0';
				break;
			case UPNPCLEANTHRESHOLD:
				v->clean_ruleset_threshold = atoi(ary_options[i].value);
				break;
			case UPNPCLEANINTERVAL:
				v->clean_ruleset_interval = atoi(ary_options[i].value);
				break;
#ifdef USE_PF
			case UPNPQUEUE:
				queue = ary_options[i].value;
				break;
			case UPNPTAG:
				tag = ary_options[i].value;
				break;
#endif
#ifdef ENABLE_NATPMP
			case UPNPENABLENATPMP:
				if(strcmp(ary_options[i].value, "yes") == 0)
					SETFLAG(ENABLENATPMPMASK);	/*enablenatpmp = 1;*/
				else
					if(atoi(ary_options[i].value))
						SETFLAG(ENABLENATPMPMASK);
					/*enablenatpmp = atoi(ary_options[i].value);*/
				break;
#endif
#ifdef PF_ENABLE_FILTER_RULES
			case UPNPQUICKRULES:
				if(strcmp(ary_options[i].value, "no") == 0)
					SETFLAG(PFNOQUICKRULESMASK);
				break;
#endif
			case UPNPENABLE:
				if(strcmp(ary_options[i].value, "yes") != 0)
					CLEARFLAG(ENABLEUPNPMASK);
				break;
			case UPNPSECUREMODE:
				if(strcmp(ary_options[i].value, "yes") == 0)
					SETFLAG(SECUREMODEMASK);
				break;
#ifdef ENABLE_LEASEFILE
			case UPNPLEASEFILE:
				lease_file = ary_options[i].value;
				break;
#endif
			case UPNPMINISSDPDSOCKET:
				minissdpdsocketpath = ary_options[i].value;
				break;
			default:
				fprintf(stderr, "Unknown option in file %s\n",
				        optionsfile);
			}
		}
	}

	/* command line arguments processing */
	for(i=1; i<argc; i++)
	{
		if(argv[i][0]!='-')
		{
			fprintf(stderr, "Unknown option: %s\n", argv[i]);
		}
		else switch(argv[i][1])
		{
		case 'o':
			if(i+1 < argc)
				use_ext_ip_addr = argv[++i];
			else
				fprintf(stderr, "Option -%c takes one argument.\n", argv[i][1]);
			break;
		case 't':
			if(i+1 < argc)
				v->notify_interval = atoi(argv[++i]);
			else
				fprintf(stderr, "Option -%c takes one argument.\n", argv[i][1]);
			break;
		case 'u':
			if(i+1 < argc)
				strncpy(uuidvalue+5, argv[++i], strlen(uuidvalue+5) + 1);
			else
				fprintf(stderr, "Option -%c takes one argument.\n", argv[i][1]);
			break;
		case 's':
			if(i+1 < argc)
				strncpy(serialnumber, argv[++i], SERIALNUMBER_MAX_LEN);
			else
				fprintf(stderr, "Option -%c takes one argument.\n", argv[i][1]);
			serialnumber[SERIALNUMBER_MAX_LEN-1] = '\0';
			break;
		case 'm':
			if(i+1 < argc)
				strncpy(modelnumber, argv[++i], MODELNUMBER_MAX_LEN);
			else
				fprintf(stderr, "Option -%c takes one argument.\n", argv[i][1]);
			modelnumber[MODELNUMBER_MAX_LEN-1] = '\0';
			break;
#ifdef ENABLE_NATPMP
		case 'N':
			/*enablenatpmp = 1;*/
			SETFLAG(ENABLENATPMPMASK);
			break;
#endif
		case 'U':
			/*sysuptime = 1;*/
			SETFLAG(SYSUPTIMEMASK);
			break;
		/*case 'l':
			logfilename = argv[++i];
			break;*/
		case 'L':
			/*logpackets = 1;*/
			SETFLAG(LOGPACKETSMASK);
			break;
		case 'S':
			SETFLAG(SECUREMODEMASK);
			break;
		case 'i':
			if(i+1 < argc)
				ext_if_name = argv[++i];
			else
				fprintf(stderr, "Option -%c takes one argument.\n", argv[i][1]);
			break;
#ifdef USE_PF
		case 'q':
			if(i+1 < argc)
				queue = argv[++i];
			else
				fprintf(stderr, "Option -%c takes one argument.\n", argv[i][1]);
			break;
		case 'T':
			if(i+1 < argc)
				tag = argv[++i];
			else
				fprintf(stderr, "Option -%c takes one argument.\n", argv[i][1]);
			break;
#endif
		case 'p':
			if(i+1 < argc)
				v->port = atoi(argv[++i]);
			else
#ifdef ENABLE_NFQUEUE
		case 'Q':
			if(i+1<argc)
			{
				nfqueue = atoi(argv[++i]);
			}
			else
				fprintf(stderr, "Option -%c takes one argument.\n", argv[i][1]);
			break;
		case 'n':
			if (i+1 < argc) {
				i++;
				if(n_nfqix < MAX_LAN_ADDR) {
					nfqix[n_nfqix++] = if_nametoindex(argv[i]);
				} else {
					fprintf(stderr,"Too many nfq interfaces. Ignoring %s\n", argv[i]);
				}
			} else {
				fprintf(stderr, "Option -%c takes one argument.\n", argv[i][1]);
			}
			break;
#endif
				fprintf(stderr, "Option -%c takes one argument.\n", argv[i][1]);
			break;
		case 'P':
			if(i+1 < argc)
				pidfilename = argv[++i];
			else
				fprintf(stderr, "Option -%c takes one argument.\n", argv[i][1]);
			break;
		case 'd':
			debug_flag = 1;
			break;
		case 'w':
			if(i+1 < argc)
				presurl = argv[++i];
			else
				fprintf(stderr, "Option -%c takes one argument.\n", argv[i][1]);
			break;
		case 'B':
			if(i+2<argc)
			{
				downstream_bitrate = strtoul(argv[++i], 0, 0);
				upstream_bitrate = strtoul(argv[++i], 0, 0);
			}
			else
				fprintf(stderr, "Option -%c takes two arguments.\n", argv[i][1]);
			break;
		case 'a':
			if(i+1 < argc)
			{
				i++;
				lan_addr = (struct lan_addr_s *) malloc(sizeof(struct lan_addr_s));
				if (lan_addr == NULL)
				{
					fprintf(stderr, "malloc(sizeof(struct lan_addr_s)): %m");
					break;
				}
				if(parselanaddr(lan_addr, argv[i]) != 0)
				{
					fprintf(stderr, "can't parse \"%s\" as valid lan address\n", argv[i]);
					free(lan_addr);
					break;
				}
				/* check if we already have this address */
				for(lan_addr2 = lan_addrs.lh_first; lan_addr2 != NULL; lan_addr2 = lan_addr2->list.le_next)
				{
					if (0 == strncmp(lan_addr2->str, lan_addr->str, 15))
						break;
				}
				if (lan_addr2 == NULL)
					LIST_INSERT_HEAD(&lan_addrs, lan_addr, list);
			}
			else
				fprintf(stderr, "Option -%c takes one argument.\n", argv[i][1]);
			break;
		case 'f':
			i++;	/* discarding, the config file is already read */
			break;
		default:
			fprintf(stderr, "Unknown option: %s\n", argv[i]);
		}
	}
	if(!ext_if_name || !lan_addrs.lh_first)
	{
		/* bad configuration */
		goto print_usage;
	}

	if(debug_flag)
	{
		pid = getpid();
	}
	else
	{
#ifdef USE_DAEMON
		daemon(1, 1);
		pid = getpid();
#else
		pid = daemonize();
#endif
	}


	if(checkforrunning(pidfilename) < 0)
	{
		NP_UPNP_ERROR("MiniUPnPd is already running. EXITING\n");
		return 1;
	}	

	set_startup_time(GETFLAG(SYSUPTIMEMASK));

	/* presentation url */
	if(presurl)
	{
		strncpy(presentationurl, presurl, PRESENTATIONURL_MAX_LEN);
		presentationurl[PRESENTATIONURL_MAX_LEN-1] = '\0';
	}
	else
	{
		snprintf(presentationurl, PRESENTATIONURL_MAX_LEN,
		         "http://%s/", lan_addrs.lh_first->str);
		         /*"http://%s:%d/", lan_addrs.lh_first->str, 80);*/
	}

	/* set signal handler */
	memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_handler = sigterm;

	if (sigaction(SIGTERM, &sa, NULL))
	{
		NP_UPNP_ERROR("Failed to set %s handler. EXITING\n", "SIGTERM");
		return 1;
	}
	if (sigaction(SIGINT, &sa, NULL))
	{
		NP_UPNP_ERROR("Failed to set %s handler. EXITING\n", "SIGINT");
		return 1;
	}

	if(signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
		NP_UPNP_ERROR("Failed to ignore SIGPIPE signals\n");
	}

	sa.sa_handler = sigusr1;
	if (sigaction(SIGUSR1, &sa, NULL))
	{
		NP_UPNP_DEBUG("Failed to set %s handler\n", "SIGUSR1");
	}

#ifdef MBB_FEATURE_PCP
    sa.sa_handler = sigusr2;
    if (sigaction(SIGUSR2, &sa, NULL))
    {
        NP_UPNP_ERROR("Failed to set SIGUSR2 handler.\n");
    }
#endif

	if(init_redirect() < 0)
	{
		NP_UPNP_ERROR("Failed to init redirection engine. EXITING\n");
		return 1;
	}

	writepidfile(pidfilename, pid);

#ifdef ENABLE_LEASEFILE
	/*remove(lease_file);*/
	NP_UPNP_DEBUG("Reloading rules from lease file\n");
	reload_from_lease_file();
#endif

	return 0;
print_usage:
	fprintf(stderr, "Usage:\n\t"
	        "%s [-f config_file] [-i ext_ifname] [-o ext_ip]\n"
#ifndef ENABLE_NATPMP
			"\t\t[-a listening_ip] [-p port] [-d] [-L] [-U] [-S]\n"
#else
			"\t\t[-a listening_ip] [-p port] [-d] [-L] [-U] [-S] [-N]\n"
#endif
			/*"[-l logfile] " not functionnal */
			"\t\t[-u uuid] [-s serial] [-m model_number] \n"
			"\t\t[-t notify_interval] [-P pid_filename]\n"
			"\t\t[-B down up] [-w url]\n"
#ifdef USE_PF
                        "\t\t[-q queue] [-T tag]\n"
#endif
#ifdef ENABLE_NFQUEUE
                        "\t\t[-Q queue] [-n name]\n"
#endif
	        "\nNotes:\n\tThere can be one or several listening_ips.\n"
	        "\tNotify interval is in seconds. Default is 30 seconds.\n"
			"\tDefault pid file is '%s'.\n"
			"\tDefault config file is '%s'.\n"
			"\tWith -d miniupnpd will run as a standard program.\n"
			"\t-L sets packet log in pf and ipf on.\n"
			"\t-S sets \"secure\" mode : clients can only add mappings to their own ip\n"
			"\t-U causes miniupnpd to report system uptime instead "
			"of daemon uptime.\n"
#ifdef ENABLE_NATPMP
			"\t-N enable NAT-PMP functionnality.\n"
#endif
			"\t-B sets bitrates reported by daemon in bits per second.\n"
			"\t-w sets the presentation url. Default is http address on port 80\n"
#ifdef USE_PF
			"\t-q sets the ALTQ queue in pf.\n"
			"\t-T sets the tag name in pf.\n"
#endif
#ifdef ENABLE_NFQUEUE
                        "\t-Q sets the queue number that is used by NFQUEUE.\n"
                        "\t-n sets the name of the interface(s) that packets will arrive on.\n"
#endif
			"\t-h prints this help and quits.\n"
	        "", argv[0], pidfilename, DEFAULT_CONFIG);
	return 1;
}

/* === main === */
/* process HTTP or SSDP requests */
int
main(int argc, char * * argv)
{
    NP_UPNP_INFO("#########################################\n");
    NP_UPNP_INFO(VERSION_INFO);
    NP_UPNP_INFO("#########################################\n");
    NP_UPNP_INFO("ENTER MAIN OF UPNP, FLUSH UPNP_PREROUTING CHAIN\n");
    system("/system/bin/iptables -t nat -F UPNP_PREROUTING 2>/dev/null");
    //system("/system/bin/kill -9 `/system/bin/ps|/system/bin/grep miniupnpd|/system/bin/grep -v grep|/system/bin/awk '{print $2}'`");
    /*BEGIN PN:2071008409 l00170266 for var problem modify 20120725 */
    system(UPNP_KILL_CMD);
    system(UPNP_MKDIR_ROUTER_DIR_CMD);
    system(UPNP_MKDIR_DIR_CMD);
    /*END PN:2071008409 l00170266 for var problem modify 20120725 */

	int i;
	int shttpl = -1;	/* socket for HTTP */
	int sudp = -1;		/* IP v4 socket for receiving SSDP */
#ifdef ENABLE_IPV6
	int sudpv6 = -1;	/* IP v6 socket for receiving SSDP */
#endif
#ifdef ENABLE_NATPMP
	int * snatpmp;
#endif
#ifdef ENABLE_NFQUEUE
	int nfqh = -1;
#endif
#ifdef USE_IFACEWATCHER
	int sifacewatcher = -1;
#endif

	int * snotify;
	int addr_count;
	LIST_HEAD(httplisthead, upnphttp) upnphttphead;
	struct upnphttp * e = 0;
	struct upnphttp * next;
	fd_set readset;	/* for select() */
#ifdef ENABLE_EVENTS
	fd_set writeset;
#endif
	struct timeval timeout, timeofday, lasttimeofday = {0, 0};
	int max_fd = -1;
#ifdef USE_MINIUPNPDCTL
	int sctl = -1;
	LIST_HEAD(ctlstructhead, ctlelem) ctllisthead;
	struct ctlelem * ectl;
	struct ctlelem * ectlnext;
#endif
	struct runtime_vars v;
	/* variables used for the unused-rule cleanup process */
	struct rule_state * rule_list = 0;
	struct timeval checktime = {0, 0};
	struct lan_addr_s * lan_addr;
#ifdef MBB_FEATURE_PCP
    int wan_status;
#endif

    /*BEGIN 3082101267 y00248130 2013-8-23 added/modified*/
    struct timespec ts;
    int ret = -1;
    time_t curr_time;
    /* END  3082101267 y00248130 2013-8-23 added/modified*/

#ifndef USE_MOCKER
    if(-1 == init_notify_mode())
    {
        NP_UPNP_DEBUG("init_notify_mode() returns -1\n");
        return -1;
    }
#endif

	if(init(argc, argv, &v) != 0)
		return 1;
    NP_UPNP_DEBUG("ext_if_name is %s\n", ext_if_name);
    /* BEGIN 2082304944 zhoujianchun 00203875 2012.8.29 added */
    memcpy(uuidvalue_root, uuidvalue, strlen(uuidvalue) - 1);
    memcpy(uuidvalue_wan, uuidvalue, strlen(uuidvalue) - 1);
    memcpy(uuidvalue_wan_conn, uuidvalue, strlen(uuidvalue) - 1);
    NP_UPNP_DEBUG("uuidvalue is %s\n", uuidvalue);
    NP_UPNP_DEBUG("uuidvalue_root is %s\n", uuidvalue_root);
    NP_UPNP_DEBUG("uuidvalue_wan is %s\n", uuidvalue_wan);
    NP_UPNP_DEBUG("uuidvalue_wan_conn is %s\n", uuidvalue_wan_conn);
    uuid_value[UUID_IDX_ROOT] = uuidvalue_root;
    uuid_value[UUID_IDX_IGD] = uuidvalue_root;
    uuid_value[UUID_IDX_WAN_CONN] = uuidvalue_wan_conn;
    uuid_value[UUID_IDX_WAN] = uuidvalue_wan;
    uuid_value[UUID_IDX_WAN_COMMON] = uuidvalue_wan;
    uuid_value[UUID_IDX_WAN_IPCONN] = uuidvalue_wan_conn;
    uuid_value[UUID_IDX_L3F] = uuidvalue_root;
    uuid_value[UUID_IDX_IPV6FW] = uuidvalue_root;

    memcpy(uuidvalue, uuidvalue_root, strlen(uuidvalue));
    /* END   2082304944 zhoujianchun 00203875 2012.8.29 added */
    load_upnp_cfg(&upnp_cfg_st);

#ifdef FEATURE_HUAWEI_MBB_PNPX
    strncpy(hardware_id, "VEN_2EB6&amp;DEV_1A03&amp;SUBSYS_01&amp;REV_0000 "
            "VEN_0033&amp;DEV_0008&amp;REV_01", sizeof(hardware_id) - 1);
    at_sku sku_info;
    memset(&sku_info, 0, sizeof(sku_info));
    if(0 == AT_sku(AT_CMD_TYPE_CHECK, NULL, &sku_info))
    {
        NP_UPNP_INFO("AT_sku returns zero.\n");
        switch((int)sku_info.AtExeResult)
        {
            case AT_CMD_RES_SUCCESS:
                {
                    NP_UPNP_INFO("AT_CMD_RES_SUCCESS\n");
                    switch(sku_info.n)
                    {
                        case V_MOBILE_WIFI: /* 0, Vodafone Mobile Wi-Fi */
                            NP_UPNP_INFO("Mobile Wi-Fi\n");
                            strncpy(hardware_id, "VEN_2EB6&amp;DEV_1A03&amp;SUBSYS_01&amp;REV_0000 "
                                    "VEN_0033&amp;DEV_0008&amp;REV_01", sizeof(hardware_id) - 1);
                            break;
                        case P_WIFI: /* 1, Pocket Wi-Fi */
                            NP_UPNP_INFO("Pocket Wi-Fi\n");
                            strncpy(hardware_id, "VEN_2EB6&amp;DEV_1A03&amp;SUBSYS_02&amp;REV_0000 "
                                    "VEN_0033&amp;DEV_0008&amp;REV_01", sizeof(hardware_id) - 1);
                            break;
                        case E_NONE: /* 2, Enterprise None P&P */
                            NP_UPNP_INFO("Enterprise None P&P\n");
                            strncpy(hardware_id, "VEN_2EB6&amp;DEV_1A03&amp;SUBSYS_04&amp;REV_0000 "
                                    "VEN_0033&amp;DEV_0008&amp;REV_01", sizeof(hardware_id) - 1);
                            break;
                        case C_JOHNS: /* 3, VHA Crazy Johns */
                            NP_UPNP_INFO("VHA Crazy Johns\n");
                            strncpy(hardware_id, "VEN_2EB6&amp;DEV_1A03&amp;SUBSYS_03&amp;REV_0000 "
                                    "VEN_0033&amp;DEV_0008&amp;REV_01", sizeof(hardware_id) - 1);
                            break;
                        case C_QUICKSTART: /* 4, Crazy Johns Quick Start */
                            NP_UPNP_INFO("Crazy Johns Quick Start\n");
                            strncpy(hardware_id, "VEN_2EB6&amp;DEV_1A03&amp;SUBSYS_03&amp;REV_0000 "
                                    "VEN_0033&amp;DEV_0008&amp;REV_01", sizeof(hardware_id) - 1);
                            break;
                    }
                    break;
                }
            default:
                {
                    NP_UPNP_WARN("AT CMD AT+SKU return error, use default sku value.\n");
                    break;
                }
        }
    }
    else
    {
        NP_UPNP_WARN("AT_sku return none zero, use default sku value.\n");
    }

#endif
	/* count lan addrs */
	addr_count = 0;
	for(lan_addr = lan_addrs.lh_first; lan_addr != NULL; lan_addr = lan_addr->list.le_next)
		addr_count++;

    NP_UPNP_DEBUG("addr_count is %d\n", addr_count);

	snotify = (int*) malloc(addr_count * sizeof(int));
	memset(snotify, 0, sizeof(snotify));
#ifdef ENABLE_NATPMP
	snatpmp = (int*) malloc(addr_count * sizeof(int));
	for(i = 0; i < addr_count; i++)
		snatpmp[i] = -1;
#endif

	LIST_INIT(&upnphttphead);
#ifdef USE_MINIUPNPDCTL
	LIST_INIT(&ctllisthead);
#endif

	if(
#ifdef ENABLE_NATPMP
        !GETFLAG(ENABLENATPMPMASK) &&
#endif
        !GETFLAG(ENABLEUPNPMASK) ) {
		NP_UPNP_ERROR("Why did you run me anyway?\n");
        free(snotify);
#ifdef ENABLE_NATPMP
        free(snatpmp);
#endif
		return 0;
	}

	if(GETFLAG(ENABLEUPNPMASK))
	{

		/* open socket for HTTP connections. Listen on the 1st LAN address */
		shttpl = OpenAndConfHTTPSocket((v.port > 0) ? v.port : 0);
		if(shttpl < 0)
		{
			NP_UPNP_ERROR("Failed to open socket for HTTP. EXITING\n");
            free(snotify);
#ifdef ENABLE_NATPMP
            free(snatpmp);
#endif
			return 1;
		}
		if(v.port <= 0) {
			struct sockaddr_in sockinfo;
			socklen_t len = sizeof(struct sockaddr_in);
			if (getsockname(shttpl, (struct sockaddr *)&sockinfo, &len) < 0) {
				NP_UPNP_ERROR("getsockname(): %s\n", strerror(errno));
                close(shttpl);
                free(snotify);
#ifdef ENABLE_NATPMP
                free(snatpmp);
#endif
				return 1;
			}
			v.port = ntohs(sockinfo.sin_port);
		}
		NP_UPNP_DEBUG("HTTP listening on port %d\n", v.port);
#ifdef ENABLE_IPV6
		if(find_ipv6_addr(NULL, ipv6_addr_for_http_with_brackets, sizeof(ipv6_addr_for_http_with_brackets)) > 0) {
			NP_UPNP_DEBUG("HTTP IPv6 address given to control points : %s\n",
			       ipv6_addr_for_http_with_brackets);
		} else {
			memcpy(ipv6_addr_for_http_with_brackets, "[::1]", 6);
			NP_UPNP_WARN("no HTTP IPv6 address\n");
		}
#endif

		/* open socket for SSDP connections */
		sudp = OpenAndConfSSDPReceiveSocket(0);
		if(sudp < 0)
		{
			NP_UPNP_INFO("Failed to open socket for receiving SSDP. Trying to use MiniSSDPd\n");
			if(SubmitServicesToMiniSSDPD(lan_addrs.lh_first->str, v.port) < 0) {
				NP_UPNP_ERROR("Failed to connect to MiniSSDPd. EXITING\n");
                close(shttpl);
                free(snotify);
#ifdef ENABLE_NATPMP
                free(snatpmp);
#endif
				return 1;
			}
		}
#ifdef ENABLE_IPV6
		sudpv6 = OpenAndConfSSDPReceiveSocket(1);
		if(sudpv6 < 0)
		{
			NP_UPNP_DEBUG("Failed to open socket for receiving SSDP (IP v6).\n");
		}
#endif

		/* open socket for sending notifications */
		if(OpenAndConfSSDPNotifySockets(snotify) < 0)
		{
			NP_UPNP_ERROR("Failed to open sockets for sending SSDP notify "
		                "messages. EXITING\n");
            close(shttpl);
            close(sudp);
            free(snotify);
#ifdef ENABLE_NATPMP
            free(snatpmp);
#endif
			return 1;
        }
        /* BEGIN 2082304944 zhoujianchun 00203875 2012.8.29 added */
	    if (GETFLAG(ENABLEUPNPMASK))
	    {
		    if(0 > SendSSDPGoodbye(snotify, addr_count))
		    {
			    NP_UPNP_ERROR("Failed to broadcast good-bye notifications\n");
		    }
        }
        /* END   2082304944 zhoujianchun 00203875 2012.8.29 added */
#ifdef USE_IFACEWATCHER
		/* open socket for kernel notifications about new network interfaces */
		if (sudp >= 0)
		{
			sifacewatcher = OpenAndConfInterfaceWatchSocket();
			if (sifacewatcher < 0)
			{
				NP_UPNP_ERROR("Failed to open socket for receiving network interface notifications\n");
			}
		}
#endif
	}

#ifdef ENABLE_NATPMP
	/* open socket for NAT PMP traffic */
	if(GETFLAG(ENABLENATPMPMASK))
	{
		if(OpenAndConfNATPMPSockets(snatpmp) < 0)
		{
			NP_UPNP_ERROR("Failed to open sockets for NAT PMP.\n");
		} else {
			NP_UPNP_DEBUG("Listening for NAT-PMP traffic on port %u\n",
			       NATPMP_PORT);
		}
#if 0
		ScanNATPMPforExpiration();
#endif
	}
#endif

	/* for miniupnpdctl */
#ifdef USE_MINIUPNPDCTL
    /*BEGIN PN:2071008409 l00170266 for var problem modify 20120725 */
	sctl = OpenAndConfCtlUnixSocket(UPNP_MINIUPNPCTL);
    /*END PN:2071008409 l00170266 for var problem modify 20120725 */
#endif

#ifdef ENABLE_NFQUEUE
	if ( nfqueue != -1 && n_nfqix > 0) {
		nfqh = OpenAndConfNFqueue();
		if(nfqh < 0) {
			NP_UPNP_ERROR("Failed to open fd for NFQUEUE.\n");
            close(shttpl);
            close(sudp);
            free(snotify);
#ifdef ENABLE_NATPMP
            free(snatpmp);
#endif
			return 1;
		} else {
			NP_UPNP_DEBUG("Opened NFQUEUE %d\n",nfqueue);
		}
	}
#endif
	/* main loop */
	while(!quitting)
	{
        NP_UPNP_DEBUG("main loop heart beat ");
        NP_UPNP_DEBUG(VERSION_INFO);
		/* Correct startup_time if it was set with a RTC close to 0 */
        /*BEGIN 3082101267 y00248130 2013-8-23 added/modified*/
        curr_time = get_boot_time();
        if ((startup_time < 60 * 60 * 24) && (curr_time > 60 * 60 * 24))  /*以天为单位重新初始化startup_time*/  
        {
            set_startup_time(GETFLAG(SYSUPTIMEMASK));
        } 
        /* END  3082101267 y00248130 2013-8-23 added/modified*/
		/* send public address change notifications if needed */
		if(should_send_public_address_change_notif)
		{
			NP_UPNP_DEBUG("should send external iface address change notification(s)\n");
#ifdef ENABLE_NATPMP
			if(GETFLAG(ENABLENATPMPMASK))
				SendNATPMPPublicAddressChangeNotification(snatpmp, addr_count);
#endif
#ifdef ENABLE_EVENTS
			if(GETFLAG(ENABLEUPNPMASK))
			{
				upnp_event_var_change_notify(EWanIPC);
			}
#endif

#ifdef MBB_FEATURE_PCP
            update_all_rules_to_pcp_client();
#endif 

			should_send_public_address_change_notif = 0;
		}

#ifdef MBB_FEATURE_PCP
        if (1 == should_update_pcp)
        {
            update_all_rules_to_pcp_client();
            should_update_pcp = 0;
        }
#endif
		/* Check if we need to send SSDP NOTIFY messages and do it if
		 * needed */
        /*BEGIN 3082101267 y00248130 2013-8-23 added/modified*/
        ret = clock_gettime(CLOCK_MONOTONIC, &ts);
        if (ret < 0)
        {
            NP_UPNP_ERROR("clock_gettime(): %s\n", strerror(errno));
            timeout.tv_sec = v.notify_interval;
            timeout.tv_usec = 0;
            timeofday.tv_sec = 0;
            timeofday.tv_usec = 0;
        }
        else
        {
            timeofday.tv_sec = ts.tv_sec;
            timeofday.tv_usec = ts.tv_nsec / 1000;    /*ns转换为us*/        
        /* END  3082101267 y00248130 2013-8-23 added/modified*/
			/* the comparaison is not very precise but who cares ? */
			if(timeofday.tv_sec >= (lasttimeofday.tv_sec + v.notify_interval))
			{
				if (GETFLAG(ENABLEUPNPMASK))
					SendSSDPNotifies2(snotify,
				                  (unsigned short)v.port,
                                  /*BEGIN 2061908201 zhoujianchun 00203875 2012.07.06 modified */
                                  1800);/* follow protocol's advice */
                                  /*END   2061908201 zhoujianchun 00203875 2012.07.06 modified */
				memcpy(&lasttimeofday, &timeofday, sizeof(struct timeval));
				timeout.tv_sec = v.notify_interval;
				timeout.tv_usec = 0;
			}
			else
			{
				timeout.tv_sec = lasttimeofday.tv_sec + v.notify_interval
				                 - timeofday.tv_sec;
				if(timeofday.tv_usec > lasttimeofday.tv_usec)
				{
					timeout.tv_usec = 1000000 + lasttimeofday.tv_usec
					                  - timeofday.tv_usec;
					timeout.tv_sec--;
				}
				else
				{
					timeout.tv_usec = lasttimeofday.tv_usec - timeofday.tv_usec;
				}
			}
		}
		/* remove unused rules */
		if( v.clean_ruleset_interval
		  && (timeofday.tv_sec >= checktime.tv_sec + v.clean_ruleset_interval))
		{
			if(rule_list)
			{
				remove_unused_rules(rule_list);
				rule_list = NULL;
			}
			else
			{
				rule_list = get_upnp_rules_state_list(v.clean_ruleset_threshold);
			}
			memcpy(&checktime, &timeofday, sizeof(struct timeval));
		}
		/* Remove expired port mappings, based on UPnP IGD LeaseDuration
		 * or NAT-PMP lifetime) */
		if(nextruletoclean_timestamp
		  && ((unsigned int)(timeofday.tv_sec) >= nextruletoclean_timestamp))
		{
			NP_UPNP_DEBUG("cleaning expired Port Mappings\n");
			get_upnp_rules_state_list(0);
		}
		if(nextruletoclean_timestamp
		  && (unsigned int)(timeout.tv_sec) >= (nextruletoclean_timestamp - timeofday.tv_sec))
		{
			timeout.tv_sec = nextruletoclean_timestamp - timeofday.tv_sec;
			timeout.tv_usec = 0;
			NP_UPNP_DEBUG("setting timeout to %u sec\n",
			       (unsigned)timeout.tv_sec);
		}
#ifdef ENABLE_NATPMP
#if 0
		/* Remove expired NAT-PMP mappings */
		while(nextnatpmptoclean_timestamp
		     && (timeofday.tv_sec >= nextnatpmptoclean_timestamp + startup_time))
		{
			if(CleanExpiredNATPMP() < 0) {
				break;
			}
		}
		if(nextnatpmptoclean_timestamp
		  && timeout.tv_sec >= (nextnatpmptoclean_timestamp + startup_time - timeofday.tv_sec))
		{
			timeout.tv_sec = nextnatpmptoclean_timestamp + startup_time - timeofday.tv_sec;
			timeout.tv_usec = 0;
		}
#endif
#endif

		/* select open sockets (SSDP, HTTP listen, and all HTTP soap sockets) */
		FD_ZERO(&readset);

		if (sudp >= 0) 
		{
			FD_SET(sudp, &readset);
			max_fd = MAX( max_fd, sudp);
#ifdef USE_IFACEWATCHER
			if (sifacewatcher >= 0)
			{
				FD_SET(sifacewatcher, &readset);
				max_fd = MAX(max_fd, sifacewatcher);
			}
#endif
		}
		if (shttpl >= 0) 
		{
			FD_SET(shttpl, &readset);
			max_fd = MAX( max_fd, shttpl);
		}
#ifdef ENABLE_IPV6
		if (sudpv6 >= 0)
		{
			FD_SET(sudpv6, &readset);
			max_fd = MAX( max_fd, sudpv6);
		}
#endif

#ifdef ENABLE_NFQUEUE
		if (nfqh >= 0) 
		{
			FD_SET(nfqh, &readset);
			max_fd = MAX( max_fd, nfqh);
		}
#endif

		i = 0;	/* active HTTP connections count */
		for(e = upnphttphead.lh_first; e != NULL; e = e->entries.le_next)
		{
			if((e->socket >= 0) && (e->state <= 2))
			{
				FD_SET(e->socket, &readset);
				max_fd = MAX( max_fd, e->socket);
				i++;
			}
		}
		/* for debug */
#ifdef DEBUG
		if(i > 1)
		{
			NP_UPNP_DEBUG("%d active incoming HTTP connections\n", i);
		}
#endif
#ifdef ENABLE_NATPMP
		for(i=0; i<addr_count; i++) {
			if(snatpmp[i] >= 0) {
				FD_SET(snatpmp[i], &readset);
				max_fd = MAX( max_fd, snatpmp[i]);
			}
		}
#endif
#ifdef USE_MINIUPNPDCTL
		if(sctl >= 0) {
			FD_SET(sctl, &readset);
			max_fd = MAX( max_fd, sctl);
		}
		
		for(ectl = ctllisthead.lh_first; ectl; ectl = ectl->entries.le_next)
		{
			if(ectl->socket >= 0) {
				FD_SET(ectl->socket, &readset);
				max_fd = MAX( max_fd, ectl->socket);
			}
		}
#endif

#ifdef ENABLE_EVENTS
		FD_ZERO(&writeset);
		upnpevents_selectfds(&readset, &writeset, &max_fd);
#endif

#ifdef ENABLE_EVENTS
		if(select(max_fd+1, &readset, &writeset, 0, &timeout) < 0)
#else
		if(select(max_fd+1, &readset, 0, 0, &timeout) < 0)
#endif
		{
			if(quitting) goto shutdown;
			if(errno == EINTR) continue; /* interrupted by a signal, start again */
			NP_UPNP_ERROR("select(all): %s\n", strerror(errno));
			NP_UPNP_ERROR("Failed to select open sockets. EXITING\n");
            close(shttpl);
            close(sudp);
            free(snotify);
#ifdef ENABLE_NATPMP
            free(snatpmp);
#endif
			return 1;	/* very serious cause of error */
		}
#ifdef USE_MINIUPNPDCTL
		for(ectl = ctllisthead.lh_first; ectl;)
		{
			ectlnext =  ectl->entries.le_next;
			if((ectl->socket >= 0) && FD_ISSET(ectl->socket, &readset))
			{
				char buf[256];
				int l;
				l = read(ectl->socket, buf, sizeof(buf));
				if(l > 0)
				{
					/*write(ectl->socket, buf, l);*/
					write_command_line(ectl->socket, argc, argv);
					write_option_list(ectl->socket);
					write_permlist(ectl->socket, upnppermlist, num_upnpperm);
					write_upnphttp_details(ectl->socket, upnphttphead.lh_first);
					write_ctlsockets_list(ectl->socket, ctllisthead.lh_first);
					write_ruleset_details(ectl->socket);
#ifdef ENABLE_EVENTS
					write_events_details(ectl->socket);
#endif
					/* close the socket */
					close(ectl->socket);
					ectl->socket = -1;
				}
				else
				{
					close(ectl->socket);
					ectl->socket = -1;
				}
			}
			if(ectl->socket < 0)
			{
				LIST_REMOVE(ectl, entries);
				free(ectl);
			}
			ectl = ectlnext;
		}
		if((sctl >= 0) && FD_ISSET(sctl, &readset))
		{
			int s;
			struct sockaddr_un clientname;
			struct ctlelem * tmp;
			socklen_t clientnamelen = sizeof(struct sockaddr_un);
			s = accept(sctl, (struct sockaddr *)&clientname,
			           &clientnamelen);
			NP_UPNP_DEBUG("sctl! : '%s'\n", clientname.sun_path);
			tmp = malloc(sizeof(struct ctlelem));
			tmp->socket = s;
			LIST_INSERT_HEAD(&ctllisthead, tmp, entries);
		}
#endif
#ifdef ENABLE_EVENTS
		upnpevents_processfds(&readset, &writeset);
#endif
#ifdef ENABLE_NATPMP
		/* process NAT-PMP packets */
		for(i=0; i<addr_count; i++)
		{
			if((snatpmp[i] >= 0) && FD_ISSET(snatpmp[i], &readset))
			{
				ProcessIncomingNATPMPPacket(snatpmp[i]);
			}
		}
#endif
		/* process SSDP packets */
		if(sudp >= 0 && FD_ISSET(sudp, &readset))
		{
			ProcessSSDPRequest(sudp, (unsigned short)v.port);
		}
#ifdef ENABLE_IPV6
		if(sudpv6 >= 0 && FD_ISSET(sudpv6, &readset))
		{
			ProcessSSDPRequest(sudpv6, (unsigned short)v.port);
		}
#endif
#ifdef USE_IFACEWATCHER
		/* process kernel notifications */
		if (sifacewatcher >= 0 && FD_ISSET(sifacewatcher, &readset))
			ProcessInterfaceWatchNotify(sifacewatcher);
#endif

		/* process active HTTP connections */
		/* LIST_FOREACH macro is not available under linux */
		for(e = upnphttphead.lh_first; e != NULL; e = e->entries.le_next)
		{
			if(  (e->socket >= 0) && (e->state <= 2)
				&&(FD_ISSET(e->socket, &readset)) )
			{
				Process_upnphttp(e);
			}
		}
		/* process incoming HTTP connections */
		if(shttpl >= 0 && FD_ISSET(shttpl, &readset))
		{
			int shttp;
			socklen_t clientnamelen;
#ifdef ENABLE_IPV6
			struct sockaddr_storage clientname;
			clientnamelen = sizeof(struct sockaddr_storage);
#else
			struct sockaddr_in clientname;
			clientnamelen = sizeof(struct sockaddr_in);
#endif
			shttp = accept(shttpl, (struct sockaddr *)&clientname, &clientnamelen);
			if(shttp<0)
			{
				NP_UPNP_ERROR("accept(http): %s\n", strerror(errno));
			}
			else
			{
				struct upnphttp * tmp = 0;
				char addr_str[64];

				sockaddr_to_string((struct sockaddr *)&clientname, addr_str, sizeof(addr_str));
				NP_UPNP_DEBUG("HTTP connection from %s\n", addr_str);
				/* Create a new upnphttp object and add it to
				 * the active upnphttp object list */
				tmp = New_upnphttp(shttp);
				if(tmp)
				{
#ifdef ENABLE_IPV6
					if(clientname.ss_family == AF_INET)
					{
						tmp->clientaddr = ((struct sockaddr_in *)&clientname)->sin_addr;
					}
					else if(clientname.ss_family == AF_INET6)
					{
						struct sockaddr_in6 * addr = (struct sockaddr_in6 *)&clientname;
						if(IN6_IS_ADDR_V4MAPPED(&addr->sin6_addr))
						{
							memcpy(&tmp->clientaddr,
							       &addr->sin6_addr.s6_addr[12],
							       4);
						}
						else
						{
							tmp->ipv6 = 1;
							memcpy(&tmp->clientaddr_v6,
							       &addr->sin6_addr,
							       sizeof(struct in6_addr));
						}
					}
#else
					tmp->clientaddr = clientname.sin_addr;
#endif
					LIST_INSERT_HEAD(&upnphttphead, tmp, entries);
				}
				else
				{
					NP_UPNP_ERROR("New_upnphttp() failed\n");
					close(shttp);
				}
			}
		}
#ifdef ENABLE_NFQUEUE
		/* process NFQ packets */
		if(nfqh >= 0 && FD_ISSET(nfqh, &readset))
		{
			ProcessNFQUEUE(nfqh);
		}
#endif
		/* delete finished HTTP connections */
		for(e = upnphttphead.lh_first; e != NULL; )
		{
			next = e->entries.le_next;
			if(e->state >= 100)
			{
				LIST_REMOVE(e, entries);
				Delete_upnphttp(e);
			}
			e = next;
		}

	}	/* end of main loop */

shutdown:
	/* close out open sockets */
	while(upnphttphead.lh_first != NULL)
	{
		e = upnphttphead.lh_first;
		LIST_REMOVE(e, entries);
		Delete_upnphttp(e);
	}

	if (sudp >= 0) close(sudp);
	if (shttpl >= 0) close(shttpl);
#ifdef ENABLE_IPV6
	if (sudpv6 >= 0) close(sudpv6);
	//if (shttplv6 >= 0) close(shttplv6);
#endif
#ifdef USE_IFACEWATCHER
	if(sifacewatcher >= 0) close(sifacewatcher);
#endif
#ifdef ENABLE_NATPMP
	for(i=0; i<addr_count; i++) {
		if(snatpmp[i]>=0)
		{
			close(snatpmp[i]);
			snatpmp[i] = -1;
		}
	}
#endif
#ifdef USE_MINIUPNPDCTL
	if(sctl>=0)
	{
		close(sctl);
		sctl = -1;
        /*BEGIN PN:2071008409 l00170266 for var problem modify 20120725 */
		if(unlink(UPNP_MINIUPNPCTL) < 0)
        /*END PN:2071008409 l00170266 for var problem modify 20120725 */
		{
			NP_UPNP_ERROR("unlink() %s\n", strerror(errno));
		}
	}
#endif

#ifdef MBB_FEATURE_PCP
    wan_status = MgntGetWanState();
    if (WAN_STATUS_CONNECTED == wan_status)
    {
        send_msg_to_pcp_client(ATP_MSG_UPNP_CLOSE, NULL);
    }
#endif

	/*if(SendSSDPGoodbye(snotify, v.n_lan_addr) < 0)*/
	if (GETFLAG(ENABLEUPNPMASK))
	{
		if(SendSSDPGoodbye(snotify, addr_count) < 0)
		{
			NP_UPNP_ERROR("Failed to broadcast good-bye notifications\n");
		}
		for(i=0; i<addr_count; i++)
			close(snotify[i]);
	}

	if(unlink(pidfilename) < 0)
	{
		NP_UPNP_ERROR("Failed to remove pidfile %s: %s\n", pidfilename, strerror(errno));
	}

	/* delete lists */
	while(lan_addrs.lh_first != NULL)
	{
		lan_addr = lan_addrs.lh_first;
		LIST_REMOVE(lan_addrs.lh_first, list);
		free(lan_addr);
	}

#ifdef ENABLE_NATPMP
	free(snatpmp);
#endif
	free(snotify);
	//closelog();	
	freeoptions();
    /* BEGIN 2032102417 zhoujianchun 203875 2012.03.23 modified */
    NP_UPNP_DEBUG("process will terminates, flush portmapping rule in nat table.\n");
    system("/system/bin/iptables -t nat -F UPNP_PREROUTING 2>/dev/null");
    /* END   2032102417 zhoujianchun 203875 2012.03.23 modified */
    
    /* BEGIN 2071909199 h00195629 20120801 added */
    system("/system/bin/iptables -t filter -F UPNP_FORWARD 2>/dev/null");
    /* END 2071909199 h00195629 20120801 added */
	
	return 0;
}
/*BEGIN 2041707023 zhoujianchun 00203875 2012.4.27 added */
int build_permission_rule(void)
{
    const char *ifname = "br0";
    int sock_fd;
    struct ifreq ifr;
    int ifrlen;
    struct sockaddr_in * addr;
    ifrlen = sizeof(ifr);

    if(NULL == ifname || '\0' == ifname[0])
    {
        return -1;
    }

    upnppermlist = 
        (struct upnpperm *)(malloc(sizeof(struct upnpperm) * 2));/* two rules */
    if(NULL == upnppermlist)
    {
        NP_UPNP_ERROR("malloc memory for upnpperm struct failed\n");
        return -1;
    }

    bzero(upnppermlist, sizeof(struct upnpperm));

    upnppermlist->type = UPNPPERM_ALLOW;
    upnppermlist->eport_min = MIN_PORT_NUMBER;
    upnppermlist->eport_max = MAX_PORT_NUMBER;
    upnppermlist->iport_min = MIN_PORT_NUMBER;
    upnppermlist->iport_max = MAX_PORT_NUMBER;

    struct upnpperm *deny_rule_pointer = upnppermlist + 1;/* points to the second rule */
    deny_rule_pointer->type = UPNPPERM_DENY;
    deny_rule_pointer->eport_min = MIN_PORT_NUMBER;
    deny_rule_pointer->eport_max = MAX_PORT_NUMBER;
    deny_rule_pointer->iport_min = MIN_PORT_NUMBER;
    deny_rule_pointer->iport_max = MAX_PORT_NUMBER; /* max port number */
    bzero(&(deny_rule_pointer->address), sizeof(struct in_addr));
    bzero(&(deny_rule_pointer->mask), sizeof(struct in_addr));

    num_upnpperm = 2;/* two rules, one to deny all, another to allow specified */

    sock_fd = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock_fd < 0)
    {
        NP_UPNP_ERROR("%s\n", strerror(errno));
        return -1;
    }
    strncpy(ifr.ifr_name, ifname, IFNAMSIZ);

    /* get net mask */
    if(ioctl(sock_fd, SIOCGIFNETMASK, &ifr, &ifrlen) < 0)
    {
        NP_UPNP_ERROR("%s\n", strerror(errno));
        close(sock_fd);
        return -1;
    }
    addr = (struct sockaddr_in *)&(ifr.ifr_addr);
    char *address = (char*)addr;
    memcpy(&(upnppermlist->mask), address + IP_ADDR_LEN, IP_ADDR_LEN);

    /* get ip address */
    if(ioctl(sock_fd, SIOCGIFADDR, &ifr, &ifrlen) < 0)
    {
        NP_UPNP_ERROR("%s\n", strerror(errno));
        close(sock_fd);
        return -1;
    }
    addr = (struct sockaddr_in *)&(ifr.ifr_addr);
    memcpy(&(upnppermlist->address), address + IP_ADDR_LEN, IP_ADDR_LEN);
    
    close(sock_fd);

    /* for debug only */

    char ip_str[IP_ADDR_STRING_LEN] = {0};
    char mask_str[IP_ADDR_STRING_LEN] = {0};
    inet_ntop(AF_INET, &(upnppermlist->address), ip_str, IP_ADDR_STRING_LEN);
    inet_ntop(AF_INET, &(upnppermlist->mask), mask_str, IP_ADDR_STRING_LEN);
    NP_UPNP_DEBUG("permission rule is %d, (%d, %d),(%d, %d),%s, %s\n",
            upnppermlist->type, upnppermlist->eport_min, 
            upnppermlist->eport_max, upnppermlist->iport_min, 
            upnppermlist->iport_max, ip_str, mask_str);

    inet_ntop(AF_INET, &(deny_rule_pointer->address), ip_str, IP_ADDR_STRING_LEN);
    inet_ntop(AF_INET, &(deny_rule_pointer->mask), mask_str, IP_ADDR_STRING_LEN);
    NP_UPNP_DEBUG("permission rule is %d, (%d, %d),(%d, %d),%s, %s\n",
            deny_rule_pointer->type, deny_rule_pointer->eport_min, 
            deny_rule_pointer->eport_max, deny_rule_pointer->iport_min, 
            deny_rule_pointer->iport_max, ip_str, mask_str);
    
    return 0;
}
/*END   2041707023 zhoujianchun 00203875 2012.4.27 added */

/* BEGIN 2122200601 s00210128 2012-12-22 modified */
void load_upnp_cfg(UPNP_CFG_ST *cfg)
{
    NODEPTR head = NULL;

    if(XML_OK != TSP_XML_ParseFile(UPNP_CONFIG_FILE, &head))
    {
        NP_UPNP_DEBUG("upnp config file parse failed, will use default value.\n");
        if(XML_OK != TSP_XML_ParseFile(OLD_UPNP_CONFIG_FILE, &head))
        {
            NP_UPNP_DEBUG("old upnp config file parse failed, will use default value.\n");
        }
    }

    LOAD_CONFIG_ITEM_STRING_STATIC(head, cfg->rootdev_friendlyname,
        "upnp_cfg.rootdev_friendlyname", ROOTDEV_FRIENDLYNAME, NP_UPNP_DEBUG);
    LOAD_CONFIG_ITEM_STRING_STATIC(head, cfg->rootdev_manufacturer,
        "upnp_cfg.rootdev_manufacturer", ROOTDEV_MANUFACTURER, NP_UPNP_DEBUG);
    LOAD_CONFIG_ITEM_STRING_STATIC(head, cfg->rootdev_manufacturerurl,
        "upnp_cfg.rootdev_manufacturerurl", ROOTDEV_MANUFACTURERURL, NP_UPNP_DEBUG);
    LOAD_CONFIG_ITEM_STRING_STATIC(head, cfg->rootdev_modelname,
        "upnp_cfg.rootdev_modelname", ROOTDEV_MODELNAME, NP_UPNP_DEBUG);
    LOAD_CONFIG_ITEM_STRING_STATIC(head, cfg->rootdev_modeldescription,
        "upnp_cfg.rootdev_modeldescription", ROOTDEV_MODELDESCRIPTION, NP_UPNP_DEBUG);
    LOAD_CONFIG_ITEM_STRING_STATIC(head, cfg->rootdev_modelurl,
        "upnp_cfg.rootdev_modelurl", ROOTDEV_MODELURL, NP_UPNP_DEBUG);
    LOAD_CONFIG_ITEM_STRING_STATIC(head, cfg->rootdev_modelnumber,
        "upnp_cfg.rootdev_modelnumber", ROOTDEV_MODELNUMBER, NP_UPNP_DEBUG);

    LOAD_CONFIG_ITEM_STRING_STATIC(head, cfg->wandev_friendlyname,
        "upnp_cfg.wandev_friendlyname", WANDEV_FRIENDLYNAME, NP_UPNP_DEBUG);
    LOAD_CONFIG_ITEM_STRING_STATIC(head, cfg->wandev_manufacturer,
        "upnp_cfg.wandev_manufacturer", WANDEV_MANUFACTURER, NP_UPNP_DEBUG);
    LOAD_CONFIG_ITEM_STRING_STATIC(head, cfg->wandev_manufacturerurl,
        "upnp_cfg.wandev_manufacturerurl", WANDEV_MANUFACTURERURL, NP_UPNP_DEBUG);
    LOAD_CONFIG_ITEM_STRING_STATIC(head, cfg->wandev_modelname,
        "upnp_cfg.wandev_modelname", WANDEV_MODELNAME, NP_UPNP_DEBUG);
    LOAD_CONFIG_ITEM_STRING_STATIC(head, cfg->wandev_modeldescription,
        "upnp_cfg.wandev_modeldescription", WANDEV_MODELDESCRIPTION, NP_UPNP_DEBUG);
    LOAD_CONFIG_ITEM_STRING_STATIC(head, cfg->wandev_modelurl,
        "upnp_cfg.wandev_modelurl", WANDEV_MODELURL, NP_UPNP_DEBUG);
    LOAD_CONFIG_ITEM_STRING_STATIC(head, cfg->wandev_modelnumber,
        "upnp_cfg.wandev_modelnumber", WANDEV_MODELNUMBER, NP_UPNP_DEBUG);
    LOAD_CONFIG_ITEM_STRING_STATIC(head, cfg->wandev_upc,
        "upnp_cfg.wandev_upc", WANDEV_UPC, NP_UPNP_DEBUG);

    LOAD_CONFIG_ITEM_STRING_STATIC(head, cfg->wancdev_friendlyname,
        "upnp_cfg.wancdev_friendlyname", WANCDEV_FRIENDLYNAME, NP_UPNP_DEBUG);
    LOAD_CONFIG_ITEM_STRING_STATIC(head, cfg->wancdev_manufacturer,
        "upnp_cfg.wancdev_manufacturer", WANCDEV_MANUFACTURER, NP_UPNP_DEBUG);
    LOAD_CONFIG_ITEM_STRING_STATIC(head, cfg->wancdev_manufacturerurl,
        "upnp_cfg.wancdev_manufacturerurl", WANCDEV_MANUFACTURERURL, NP_UPNP_DEBUG);
    LOAD_CONFIG_ITEM_STRING_STATIC(head, cfg->wancdev_modelname,
        "upnp_cfg.wancdev_modelname", WANCDEV_MODELNAME, NP_UPNP_DEBUG);
    LOAD_CONFIG_ITEM_STRING_STATIC(head, cfg->wancdev_modeldescription,
        "upnp_cfg.wancdev_modeldescription", WANCDEV_MODELDESCRIPTION, NP_UPNP_DEBUG);
    LOAD_CONFIG_ITEM_STRING_STATIC(head, cfg->wancdev_modelurl,
        "upnp_cfg.wancdev_modelurl", WANCDEV_MODELURL, NP_UPNP_DEBUG);
    LOAD_CONFIG_ITEM_STRING_STATIC(head, cfg->wancdev_modelnumber,
        "upnp_cfg.wancdev_modelnumber", WANCDEV_MODELNUMBER, NP_UPNP_DEBUG);
    LOAD_CONFIG_ITEM_STRING_STATIC(head, cfg->wancdev_upc,
        "upnp_cfg.wancdev_upc", WANCDEV_UPC, NP_UPNP_DEBUG);

    TSP_XML_FreeNode(head);
    return;
}
/* END 2122200601 s00210128 2012-12-22 modified */


/*BEGIN 3082101267 y00248130 2013-8-23 added/modified*/
/*****************************************************************************
 函数名称  : get_boot_time
 功能描述  : 对sysinfo进行封装，获取到开机到当前的时间，用于替换time
 输入参数  : No
 输出参数  : No
 返 回 值  : No
 修改历史  : 
             1. 2013-8-23 :  y00248130 created
*****************************************************************************/
time_t get_boot_time(void)
{
    int ret;
    struct sysinfo sys_info;
    memset(&sys_info, 0, sizeof(sys_info));
    ret = sysinfo(&sys_info);
    if (0 == ret)
    {
        return (time_t)(sys_info.uptime);
    }
    else
    {
        return (time_t)(0);
    }
}
/* END  3082101267 y00248130 2013-8-23 added/modified*/