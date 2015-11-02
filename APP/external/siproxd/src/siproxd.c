/* -*- Mode: C; c-basic-offset: 3 -*-
    Copyright (C) 2002-2005  Thomas Ries <tries@gmx.net>

    This file is part of Siproxd.
    
    Siproxd is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
    
    Siproxd is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with Siproxd; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
*/
   /**********************问题单修改记录******************************************
    日期              修改人         问题单号           修改内容
 2012.11.16        y00186923                         9615平台代码差异合入
 2013.08.24        s00201037      3061302357    添加siproxd的二进制交付
 2013.10.15        s00201037      3092906251    增加sipdns域名解析
******************************************************************************/
#include "config.h"

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <net/if.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#ifdef _SOLARIS2
# include <sys/sockio.h>
#endif

#ifdef	HAVE_GETOPT_H
#include <getopt.h>
#endif
/*BEGIN 3061302357 s00201037 2013-6-13 added*/  
#include "versionbin.h"
/*END 3061302357 s00201037 2013-6-13 added*/  
#include <osipparser2/osip_parser.h>

#include "siproxd.h"
#include "log.h"
/*BEGIN PN:2071008409 l00170266 for var problem modify 20120725 */
#include "netprotoapi_public.h"
#define DNSMASQ_BIND (ROUTER_VARPATH_PREFIX "/wan/dnsmasq/bind")
#define PPPOEPROXY_CONF (ROUTER_VARPATH_PREFIX "/pppoeproxy.conf")
#define ALG_MAC (ROUTER_VARPATH_PREFIX "/alg/mac")
#define ALG_IPRULE (ROUTER_VARPATH_PREFIX "/alg/iprule")
#define ALG_IPROUTE (ROUTER_VARPATH_PREFIX "/alg/iproute")
/*BEGIN 3092906251 s00201037 2013-10-15 added*/  
#define DHCPS_CONF_FILE   ROUTER_VARPATH_PREFIX "/dhcp/dhcps/config"
/*END 3092906251 s00201037 2013-10-15 added*/  
/*END PN:2071008409 l00170266 for var problem modify 20120725 */

static char const ident[]="$Id: siproxd.c,v 1.2 2009/02/27 02:34:20 l41296 Exp $";

/* configuration storage */
struct siproxd_config configuration;
/*BEGIN 3092906251 s00201037 2013-10-15 added*/
unsigned int lansrcip = 0;
int lansrcport = 0;
unsigned int br0ip = 0;
unsigned int br0mask = 0;
/*END 3092906251 s00201037 2013-10-15 added*/
/* Global File instance on pw file */
FILE *siproxd_passwordfile;

/* -h help option text */
static const char str_helpmsg[] =
PACKAGE "-" VERSION "-" BUILDSTR " (c) 2002-2005 Thomas Ries\n"
"\nUsage: siproxd [options]\n\n"
"options:\n"
#ifdef	HAVE_GETOPT_LONG
"       -h, --help                 help\n"
"       -d, --debug <pattern>      set debug-pattern\n"
"       -c, --config <cfgfile>     use the specified config file\n"
"       -p, --pid-file <pidfile>   create pid file <pidfile>\n"
#else
"       -h              help\n"
"       -d <pattern>    set debug-pattern\n"
"       -c <cfgfile>    use the specified config file\n"
"       -p <pidfile>    create pid file <pidfile>\n"
#endif
"";


#ifdef SUPPORT_BRIDGE_BIND
/*****************  SipReadIpAddr()中赋值 ************************************/
int g_lOutIFaceIsProxy = 0;  /* 判断OUTBOUND WAN接口是否为PPPOE代理   是＝1   不是＝0 */

char g_acWanBindEth[SIP_BIND_LEN] = {0};  /* 判断OUTBOUND WAN接口绑定的LAN侧接口 */

char g_acEthBind[SIP_BIND_LEN] = {0};     /* 已经被绑定的Eth接口列表 */

/* 判断Lan侧br0维护地址 */
char  g_acBrIpAddr[SIP_IPADDR] = "192.168.1.1";
char  g_acBrMask[SIP_IPADDR] = "255.255.255.0";

/* 判断Lan侧PPPoe代理维护地址 */
char  g_acPPPoEIpAddr[SIP_IPADDR] = "192.168.239.1";
char  g_acPPPoEMask[SIP_IPADDR] = "255.255.255.0";
/*****************  SipReadIpAddr()中赋值结束 ********************************/

/* 判断报文方向，0  LAN侧发向WAN侧报文     1  WAN侧发向LAN侧报文 
int g_lPacketDir = 0;
*/
#endif

/*
 * module local data
 */
static  int dmalloc_dump=0;
static  int exit_program=0;

/*
 * local prototypes
 */
static void sighandler(int sig);
/*BEGIN 3092906251 s00201037 2013-10-15 added*/
void GetBr0Info()
{
    FILE *fp = NULL;
    char acBuffer[BUFLENTH] = {0};
    char *planip = NULL;
    char *planmask = NULL;
    fp = fopen (DHCPS_CONF_FILE,"r");
    if (NULL == fp)
    {
        return;
    }
    
    while (fgets(acBuffer, BUFLENTH, fp)) 
    {
        planip = strstr(acBuffer,"Option router ");
        planmask = strstr(acBuffer,"Option subnet ");

        if (planip)
        {
            planip = planip + strlen("Option router ") ;   
            br0ip = htonl(inet_addr(planip));
        }
        if (planmask)
        {
            planmask = planmask + strlen("Option subnet ") ;
            br0mask = htonl(inet_addr(planmask));
        }
        if (br0ip && br0mask)
        {
            break;
        }
        planip = NULL;
        planmask = NULL;
    }
    fclose(fp);
}
/*END 3092906251 s00201037 2013-10-15 added*/
int main (int argc, char *argv[]) 
{
   int sts;
   int i;
   int access;
   char buff [BUFFER_SIZE];
   sip_ticket_t ticket;
   /* start of AU4D00875 by d00107688 to support bind 2008-10-15*/
   int readfd = -1;
   /* end of AU4D00875 by d00107688 to support bind 2008-10-15*/

   extern char *optarg;		/* Defined in libc getopt and unistd.h */
   int ch1;
   
   char configfile[64]="siproxd";	/* basename of configfile */
   int  config_search=1;		/* search the config file */
   int  cmdline_debuglevel=0;
   char *pidfilename=NULL;
   struct sigaction act;

   log_set_stderr(1);

/*
 * setup signal handlers
 */
   act.sa_handler=sighandler;
   sigemptyset(&act.sa_mask);
   act.sa_flags=SA_RESTART;
   if (sigaction(SIGTERM, &act, NULL)) {
      ERROR("Failed to install SIGTERM handler");
   }
   if (sigaction(SIGINT, &act, NULL)) {
      ERROR("Failed to install SIGINT handler");
   }
   if (sigaction(SIGUSR2, &act, NULL)) {
      ERROR("Failed to install SIGUSR2 handler");
   }
   if (sigaction(SIGUSR1, &act, NULL)) {
      printf("Failed to install SIGUSR1 handler\r\n");
   }


/*
 * prepare default configuration
 */
   make_default_config();

   log_set_pattern(configuration.debuglevel);      

/*
 * open a the pwfile instance, so we still have access after
 * we possibly have chroot()ed to somewhere.
 */
   if (configuration.proxy_auth_pwfile) {
      siproxd_passwordfile = fopen(configuration.proxy_auth_pwfile, "r");
   } else {
      siproxd_passwordfile = NULL;
   }

/*
 * parse command line
 */
{
#ifdef	HAVE_GETOPT_LONG
   int option_index = 0;
   static struct option long_options[] = {
      {"help", no_argument, NULL, 'h'},
      {"config", required_argument, NULL, 'c'},
      {"debug", required_argument, NULL, 'd'},
      {"pid-file", required_argument, NULL,'p'},
      /*BEGIN 3061302357 s00201037 2013-6-13 added*/   
      {"version", no_argument, NULL, 'v'},
      /*END 3061302357 s00201037 2013-6-13 added*/  
      {0,0,0,0}
   };
    /*BEGIN 3061302357 s00201037 2013-6-13 added*/  
    while ((ch1 = getopt_long(argc, argv, "hc:d:p:v",
                  long_options, &option_index)) != -1) {
    /*END 3061302357 s00201037 2013-6-13 added*/  				  
#else	/* ! HAVE_GETOPT_LONG */
    while ((ch1 = getopt(argc, argv, "hc:d:p:")) != -1) {
#endif
      switch (ch1) {
      case 'h':	/* help */
         DEBUGC(DBCLASS_CONFIG,"option: help");
         fprintf(stderr,str_helpmsg);
         exit(0);
	 break;
      /*BEGIN 3061302357 s00201037 2013-6-13 added*/   
     case 'v':/*version*/
        printf("siproxd, %s\n", SIP_MODULE_VERSION);
        printf("BUILTTIME is: %s %s\n",__DATE__,__TIME__);
        exit(0);
     /*END 3061302357 s00201037 2013-6-13 added*/   

      case 'c':	/* load config file */
         DEBUGC(DBCLASS_CONFIG,"option: config file=%s",optarg);
         i=sizeof(configfile)-1;
         strncpy(configfile,optarg,i-1);
	 configfile[i]='\0';
	 config_search=0;
	 break; 

      case 'd':	/* set debug level */
         DEBUGC(DBCLASS_CONFIG,"option: set debug level: %s",optarg);
	 cmdline_debuglevel=atoi(optarg);
         log_set_pattern(cmdline_debuglevel);
	 break;

      case 'p':
	 pidfilename = optarg;
	 break;

      default:
         DEBUGC(DBCLASS_CONFIG,"no command line options");
	 break; 
      }
   }
}

/*
 * Init stuff
 */
   INFO(PACKAGE"-"VERSION"-"BUILDSTR" "UNAME" starting up");

   /* read the config file */
   if (read_config(configfile, config_search) == STS_FAILURE) exit(1);

   /* if a debug level > 0 has been given on the commandline use its
      value and not what is in the config file */
   if (cmdline_debuglevel != 0) {
      configuration.debuglevel=cmdline_debuglevel;
   }

   /* set debug level as desired */
   log_set_pattern(configuration.debuglevel);
   log_set_listen_port(configuration.debugport);

   /* change user and group IDs */
#if 0 //该函数中进行chroot setuid会导致创建socket时权限不够而失败
   secure_enviroment();
#endif

   /* daemonize if requested to */
   if (configuration.daemonize) {
      DEBUGC(DBCLASS_CONFIG,"daemonizing");
      if (fork()!=0) exit(0);
      setsid();
      if (fork()!=0) exit(0);

      log_set_stderr(0);
      INFO("daemonized, pid=%i", getpid());
   }

   /* write PID file of main thread */
   if (pidfilename == NULL) pidfilename = configuration.pid_file;
   if (pidfilename) {
      FILE *pidfile;
      DEBUGC(DBCLASS_CONFIG,"creating PID file [%s]", pidfilename);
      sts=unlink(configuration.pid_file);
      if ((sts==0) ||(errno == ENOENT)) {
         if ((pidfile=fopen(pidfilename, "w"))) {
            fprintf(pidfile,"%i\n",(int)getpid());
            fclose(pidfile);
         } else {
            WARN("couldn't create new PID file: %s", strerror(errno));
         }
      } else {
         WARN("couldn't delete old PID file: %s", strerror(errno));
      }
   }

   /* initialize the RTP proxy */
   sts=rtpproxy_init();
   if (sts != STS_SUCCESS) {
      ERROR("unable to initialize RTP proxy - aborting"); 
      exit(1);
   }

#ifdef SUPPORT_BRIDGE_BIND
    printf("Sip support bind. \r\n");
    SipReadIpAddr();
#else
    printf("Sip not support bind. \r\n");
#endif
   /* init the oSIP parser */
   parser_init();

   /* listen for incoming messages */
   sts=sipsock_listen();
   if (sts == STS_FAILURE) {
      /* failure to allocate SIP socket... */
      ERROR("unable to bind to SIP listening socket - aborting"); 
      exit(1);
   }

   /* initialize the registration facility */
   register_init();
   /*BEGIN 3092906251 s00201037 2013-10-15 added*/
   /*获得br0的ip地址和掩码*/
   GetBr0Info();
   /*END 3092906251 s00201037 2013-10-15 added*/
/*
 * silence the log - if so required...
 */
   log_set_silence(configuration.silence_log);

   INFO(PACKAGE"-"VERSION"-"BUILDSTR" "UNAME" started");

/*
 * Main loop
 */
   while (!exit_program) {

      DEBUGC(DBCLASS_BABBLE,"going into sipsock_wait\n");
   /* start of AU4D00875 by d00107688 to support bind 2008-10-15*/
      while (sipsock_wait(&readfd)<=0) {
   /* end of AU4D00875 by d00107688 to support bind 2008-10-15*/
         /* got no input, here by timeout. do aging */
         register_agemap();

         /* TCP log: check for a connection */
         log_tcp_connect();

         /* dump memory stats if requested to do so */
         if (dmalloc_dump) {
            dmalloc_dump=0;
#ifdef DMALLOC
            INFO("SIGUSR2 - DMALLOC statistics is dumped");
            dmalloc_log_stats();
            dmalloc_log_unfreed();
#else
            INFO("SIGUSR2 - DMALLOC support is not compiled in");
#endif
         }

         if (exit_program) goto exit_prg;
      }

      /* got input, process */
      DEBUGC(DBCLASS_BABBLE,"back from sipsock_wait");

   /* start of AU4D00875 by d00107688 to support bind 2008-10-15*/
      i=sipsock_read(readfd, &buff, sizeof(buff)-1, &ticket.from, &ticket.protocol);
   /* end of AU4D00875 by d00107688 to support bind 2008-10-15*/
      buff[i]='\0';

      /* evaluate the access lists (IP based filter)*/
      access=accesslist_check(ticket.from);
      if (access == 0) {
         DEBUGC(DBCLASS_ACCESS,"access for this packet was denied");
         continue; /* there are no resources to free */
      }
      
#ifdef SUPPORT_BRIDGE_BIND
      sts = parseBind(ticket.from);
      if (sts == 0)
      {
          printsip("Sip: Not Allowed by Bind from ip[%s]. \r\n", 
              inet_ntoa(ticket.from.sin_addr));
          continue;
      }
#endif

      /* integrity checks */
      sts=security_check_raw(buff, i);
      if (sts != STS_SUCCESS) {
         DEBUGC(DBCLASS_SIP,"security check (raw) failed");
         continue; /* there are no resources to free */
      }

      /* init sip_msg */
      sts=osip_message_init(&ticket.sipmsg);
      ticket.sipmsg->message=NULL;
      if (sts != 0) {
         ERROR("osip_message_init() failed... this is not good");
	 continue; /* skip, there are no resources to free */
      }

      /*
       * RFC 3261, Section 16.3 step 1
       * Proxy Behavior - Request Validation - Reasonable Syntax
       * (parse the received message)
       */
      sts=osip_message_parse(ticket.sipmsg, buff);
      if (sts != 0) {
         ERROR("osip_message_parse() failed... this is not good");
         DUMP_BUFFER(-1, buff, i);
         goto end_loop; /* skip and free resources */
      }

      /* integrity checks - parsed buffer*/
      sts=security_check_sip(&ticket);
      if (sts != STS_SUCCESS) {
         ERROR("security_check_sip() failed... this is not good");
         DUMP_BUFFER(-1, buff, i);
         goto end_loop; /* skip and free resources */
      }

      /*
       * RFC 3261, Section 16.3 step 2
       * Proxy Behavior - Request Validation - URI scheme
       * (check request URI and refuse with 416 if not understood)
       */
      /* NOT IMPLEMENTED */

      /*
       * RFC 3261, Section 16.3 step 3
       * Proxy Behavior - Request Validation - Max-Forwards check
       * (check Max-Forwards header and refuse with 483 if too many hops)
       */
      {
      osip_header_t *max_forwards;
      int forwards_count = DEFAULT_MAXFWD;

      osip_message_get_max_forwards(ticket.sipmsg, 0, &max_forwards);
      if (max_forwards && max_forwards->hvalue) {
         forwards_count = atoi(max_forwards->hvalue);
      }

      DEBUGC(DBCLASS_PROXY,"checking Max-Forwards (=%i)",forwards_count);
      if (forwards_count <= 0) {
         DEBUGC(DBCLASS_SIP, "Forward count reached 0 -> 483 response");
         sip_gen_response(&ticket, 483 /*Too many hops*/);
         goto end_loop; /* skip and free resources */
      }

      }

      /*
       * RFC 3261, Section 16.3 step 4
       * Proxy Behavior - Request Validation - Loop Detection check
       * (check for loop and return 482 if a loop is detected)
       */
      if (check_vialoop(&ticket) == STS_TRUE) {
         /* make sure we don't end up in endless loop when detecting
          * an loop in an "loop detected" message - brrr */
         if (MSG_IS_RESPONSE(ticket.sipmsg) && 
             MSG_TEST_CODE(ticket.sipmsg, 482)) {
            DEBUGC(DBCLASS_SIP,"loop in loop-response detected, ignoring");
         } else {
            DEBUGC(DBCLASS_SIP,"via loop detected, ignoring request");
            sip_gen_response(&ticket, 482 /*Loop detected*/);
         }
         goto end_loop; /* skip and free resources */
      }

      /*
       * RFC 3261, Section 16.3 step 5
       * Proxy Behavior - Request Validation - Proxy-Require check
       * (check Proxy-Require header and return 420 if unsupported option)
       */
      /* NOT IMPLEMENTED */

      /*
       * RFC 3261, Section 16.5
       * Proxy Behavior - Determining Request Targets
       */
      /* NOT IMPLEMENTED */

      DEBUGC(DBCLASS_SIP,"received SIP type %s:%s",
	     (MSG_IS_REQUEST(ticket.sipmsg))? "REQ" : "RES",
             (MSG_IS_REQUEST(ticket.sipmsg) ?
                ((ticket.sipmsg->sip_method)?
                   ticket.sipmsg->sip_method : "NULL") :
                ((ticket.sipmsg->reason_phrase) ? 
                   ticket.sipmsg->reason_phrase : "NULL")));
              
      /*
       * if an REQ REGISTER, check if it is directed to myself,
       * or am I just the outbound proxy but no registrar.
       * - If I'm the registrar, register & generate answer
       * - If I'm just the outbound proxy, register, rewrite & forward
       */
      /* 注册报文 */
      if (MSG_IS_REGISTER(ticket.sipmsg) && 
          MSG_IS_REQUEST(ticket.sipmsg)) {
         if (access & ACCESSCTL_REG) {
            osip_uri_t *url;
            struct in_addr addr1, addr2, addr3;
            int dest_port;

            url = osip_message_get_uri(ticket.sipmsg);
            dest_port= (url->port)?atoi(url->port):SIP_PORT;

            if ( (get_ip_by_host(url->host, &addr1) == STS_SUCCESS) &&
                 (get_ip_by_ifname(configuration.inbound_if,&addr2) ==
                  STS_SUCCESS) &&
                 (get_ip_by_ifname(configuration.outbound_if,&addr3) ==
                  STS_SUCCESS)) {

               if ((configuration.sip_listen_port == dest_port) &&
                   ((memcmp(&addr1, &addr2, sizeof(addr1)) == 0) ||
                    (memcmp(&addr1, &addr3, sizeof(addr1)) == 0))) {
                  /* I'm the registrar, send response myself */
                  printsip("Sip:main(), register_client(registrar).\r\n");
                  sts = register_client(&ticket, 0);
                  sts = register_response(&ticket, sts);
               } else {
                  /* I'm just the outbound proxy */
                  DEBUGC(DBCLASS_SIP,"proxying REGISTER request to:%s",
                         url->host);
                  printsip("Sip:main(), register_client(proxy).\r\n");
                  sts = register_client(&ticket, 1);
                  sts = proxy_request(&ticket);
               }
            } else {
               if (MSG_IS_REQUEST(ticket.sipmsg)) {
                  sip_gen_response(&ticket, 408 /*request timeout*/);
               }
            }
	 } else {
            WARN("non-authorized registration attempt from %s",
	         utils_inet_ntoa(ticket.from.sin_addr));
	 }

      /*
       * check if outbound interface is UP.
       * If not, send back error to UA and
       * skip any proxying attempt
       */
      } else if (get_ip_by_ifname(configuration.outbound_if,NULL) !=
                 STS_SUCCESS) {
         DEBUGC(DBCLASS_SIP, "got a %s to proxy, but outbound interface "
                "is down", (MSG_IS_REQUEST(ticket.sipmsg))? "REQ" : "RES");

         if (MSG_IS_REQUEST(ticket.sipmsg))
            sip_gen_response(&ticket, 408 /*request timeout*/);
      
      /*
       * MSG is a request, add current via entry,
       * do a lookup in the URLMAP table and
       * send to the final destination
       */
      } else if (MSG_IS_REQUEST(ticket.sipmsg)) {
         if (access & ACCESSCTL_SIP) {
            sts = proxy_request(&ticket);
	 } else {
            INFO("non-authorized request received from %s",
	         utils_inet_ntoa(ticket.from.sin_addr));
	 }

      /*
       * MSG is a response, remove current via and
       * send to the next VIA in chain
       */
      } else if (MSG_IS_RESPONSE(ticket.sipmsg)) {
         if (access & ACCESSCTL_SIP) {
            sts = proxy_response(&ticket);
	 } else {
            INFO("non-authorized response received from %s",
	         utils_inet_ntoa(ticket.from.sin_addr));
	 }
	 
      /*
       * unsupported message
       */
      } else {
         ERROR("received unsupported SIP type %s %s",
	       (MSG_IS_REQUEST(ticket.sipmsg))? "REQ" : "RES",
	       ticket.sipmsg->sip_method);
      }


/*
 * free the SIP message buffers
 */
      end_loop:
      osip_message_free(ticket.sipmsg);

   } /* while TRUE */
   exit_prg:

   /* dump current known SIP registrations */
   register_shut();
   INFO("properly terminating siproxd");

   /* remove PID file */
   if (pidfilename) {
      DEBUGC(DBCLASS_CONFIG,"deleting PID file [%s]", pidfilename);
      sts=unlink(pidfilename);
      if (sts != 0) {
         WARN("couldn't delete old PID file: %s", strerror(errno));
      }
   }

   /* END */
   return 0;
} /* main */

/*
 * Signal handler
 *
 * this one is called asynchronously whevener a registered
 * signal is applied. Just set a flag and don't do any funny
 * things here.
 */
static void sighandler(int sig) {
   if (sig==SIGTERM) exit_program=1;
   if (sig==SIGINT)  exit_program=1;
   if (sig==SIGUSR2) dmalloc_dump=1;
#ifdef SUPPORT_BRIDGE_BIND
   if (sig==SIGUSR1) SipReadIpAddr();
#endif
   return;
}

#ifdef SUPPORT_BRIDGE_BIND
/*****************************************************************
  函数名称：SipReadIpAddr
  功能描述：读取新的配置关系，由进程外部发送  SIGUSR1信号调用
  输入参数：

  输出参数：无
  返 回 值：
  
  其它说明：
    依赖外部配置文件
    1. /var/wan/ppp_8_35_1/config   WAN模块的配置文件
       读取其中的  proxy  代理信息
                   lan    该WAN绑定的eth接口信息
                   
    2. /var/wan/dnsmasq/bind        WAN模块配置文件
       保存了所有已经被绑定的  eth接口信息

    3. /var/pppoeproxy.conf         如果1中开启了ppp代理读取该ppp代理信息
       读取其中   
          ServerAddr   ppp代理LAN侧的维护地址  默认 "192.168.239.1"
       将掩码写定为    "255.255.255.0"

    4. ioctl()                      如果1中没有开启ppp代理
       通过系统函数获取  br0  维护地址和掩码
*****************************************************************/
void SipReadIpAddr()
{
    FILE* f = NULL;
    char* pTmp = NULL;
    int lRet = 0;
    
    char acLine[128] = {0};
    char acOption[32] = {0};
    char acTemp[32] = {0};
    char acValue[SIP_BIND_LEN] = {0};

    struct ifreq  stIfr;
    int lSockfd;

    /* 判断WAN接口是否为PPP代理 */
    /*BEGIN PN:2071008409 l00170266 for var problem modify 20120725 */
    sprintf(acLine, "%s/wan/%s/config", ROUTER_VARPATH_PREFIX, configuration.outbound_if);
    /*END PN:2071008409 l00170266 for var problem modify 20120725 */
    
    f = fopen(acLine, "r");
    if (NULL == f)
    {
        printsip("Sip: open wan config fail.\r\n");
        return;
    }
    while (fgets(acLine, 128, f))
    {
        sscanf(acLine, "%s %s %s", 
            acOption, acTemp, acValue);
        if (0 == strcmp(acOption, "proxy"))
        {
            g_lOutIFaceIsProxy = atoi(acValue);
        }
        else if(0 == strcmp(acOption, "lan"))
        {
            strcpy(g_acWanBindEth, acValue);
        }
    }
    fclose(f);
    printsip("Sip: Outbound:[%s] pppproxy:[%d]\r\n", 
        configuration.outbound_if, g_lOutIFaceIsProxy);

    /* 读取所有被配置了绑定的Eth接口 */
    /*BEGIN PN:2071008409 l00170266 for var problem modify 20120725 */
    f = fopen(DNSMASQ_BIND, "r");
    /*END PN:2071008409 l00170266 for var problem modify 20120725 */
    if (NULL == f)
    {
        printsip("Sip: open bind failed.\r\n");
        return;
    }
    fgets(g_acEthBind, 128, f);
    fclose(f);
    printsip("Sip: All Binded Eht IF:[%s].\r\n", g_acEthBind);

    /* Wan接口是PPP代理，读取配置文件中的ppp代理维护地址 */
    if (1 == g_lOutIFaceIsProxy)
    {
        /*BEGIN PN:2071008409 l00170266 for var problem modify 20120725 */
        f = fopen(PPPOEPROXY_CONF, "r");
        /*END PN:2071008409 l00170266 for var problem modify 20120725 */
        
        if (NULL == f)
        {
            printsip("Sip: pppoeproxy.conf Open Fail \r\n");
            return;
        }
        while(fgets(acLine, 128, f))
        {
            if(acLine[strlen(acLine)-1] == '\n')
            {
                acLine[strlen(acLine)-1] = '\0';//去掉'\n'
            }
            if (strstr(acLine, "ServerAddr") != NULL)
            {
                pTmp = strchr(acLine, '=');
                pTmp++;
                strcpy(g_acPPPoEIpAddr, pTmp);
                lRet = 1;
                break;
            }        
        }
        if (0 == lRet)
        {   /* 读取不到，赋给默认值 */
            strcpy(g_acPPPoEIpAddr, "192.168.239.1");
            printsip("Sip: Couldn't find ServerAddr, Default PPPoE IpAddr:[%s]\r\n",
                g_acPPPoEIpAddr);
        }
        else
        {
            printsip("Sip: PPPoE IpAddr:[%s]\r\n", g_acPPPoEIpAddr);
        }
        fclose(f);
    }
    else  /* 不是PPP代理，读取Lan侧的br0维护地址 */
    {
        if ((lSockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            strcpy(g_acBrIpAddr, "192.168.1.1");
            strcpy(g_acBrMask, "255.255.255.0");
            printsip("Sip: Sock creat fail give br default value.\r\n");
            return;
        }
        strcpy(stIfr.ifr_name, "br0");
        /* get interface addr */
        if(ioctl(lSockfd, SIOCGIFADDR, &stIfr) != 0) 
        {
            strcpy(g_acBrIpAddr, "192.168.1.1");
            printsip("Sip: ioctl fail give br default addr.\r\n");
        }
        else
        {
            strcpy(g_acBrIpAddr, inet_ntoa(((struct sockaddr_in *)(&(stIfr.ifr_addr)))->sin_addr));
        }
        
        /* get mask addr */
        if(ioctl(lSockfd, SIOCGIFNETMASK, &stIfr) != 0) 
        {
            strcpy(g_acBrMask, "255.255.255.0");
            printsip("Sip: ioctl fail give br default mask.\r\n");
        }
        else
        {
            strcpy(g_acBrMask, inet_ntoa(((struct sockaddr_in *)(&(stIfr.ifr_netmask)))->sin_addr));
        }
        close(lSockfd);
        printsip("Sip: ioctl br0 addr:[%s], mask:[%s]\r\n", g_acBrIpAddr, g_acBrMask);
    }    
    return;    
}


/*****************************************************************
  函数名称：IpisInNet
  功能描述：判断Ip地址是否在Net维护的网段内
  输入参数：pszIp         待判断的ip地址
            pszNetIp      网段的地址
            lNetMask      网段的掩码
  输出参数：无
  返 回 值：1：在维护网段内    0：不在维护的网段内
  
  其它说明：

*****************************************************************/
int IpisInNet(char* pszIp, char* pszNetIp, char* pszNetMask)
{
    unsigned long int ulIp;
    unsigned long int ulNetip;
    unsigned long int ulNetMask;
    if ((NULL == pszIp) || (NULL == pszNetIp) || (NULL == pszNetMask))
    {
        printsip("Sip:IpisInNet(), input para is NULL\r\n");
        return 0;
    }
    printsip("Sip:IpisInNet(), ip:[%s], Netip:[%s], Mask:[%s]", pszIp, pszNetIp, pszNetMask);
    
    ulIp = ntohl(inet_addr(pszIp));
    ulNetip = ntohl(inet_addr(pszNetIp));
    ulNetMask = ntohl(inet_addr(pszNetMask));

    if ((ulIp & ulNetMask) == (ulNetip & ulNetMask))
    {
        printsip("In Net.\r\n");
        return 1;
    }
    else
    {
        printsip(" Not in Net.\r\n");
        return 0;
    }
}


/*****************************************************************
  函数名称：parseBind
  功能描述：判断绑定关系是否合法
  输入参数：from          报文源地址参数

  输出参数：无
  返 回 值：1：符合绑定关系，可以进一步处理    0：不符合绑定关系，丢弃报文
  
  其它说明：
            对WAN侧的报文IP 地址直接返回
*****************************************************************/
int parseBind(struct sockaddr_in from) 
{
    FILE* f = NULL;
    char acSrcIp[16] = {0};              /* 输入报文的源IP */
    char acSipOutIf[32] = {0};           /* Sip Wan侧OUTBOUND接口 */

    char acArpIp[128] = {0};
    char acArpMac[128] = {0};
    char acBrctlMac[128] = {0};
    char acLan[16] = {0};
    
    char acLine[128] = {0};
    char acTemp[128] = {0};
    char acSource[128] = {0};
    char acTable[128] = {0};
    char acDest[128] = {0};
    char acIfc[32] = {0};   
    int lBrctlPort = 0;
    int lRet = 0;                        /* 使用前先置0 */
    int num = 0;

    struct sockaddr_in stDstAddr;
    int sock = 0;
    
    strcpy(acSrcIp, inet_ntoa(from.sin_addr));
    strcpy(acSipOutIf, configuration.outbound_if);
    
    printsip("Sip: parseBind Src ip:[%s], outboud_if:[%s]\r\n", acSrcIp, acSipOutIf);

    if (g_lOutIFaceIsProxy == 0)
    {   /* 不是ppp代理 */
        /* 不是LAN侧报文，不做处理 */
        lRet = IpisInNet(acSrcIp, g_acBrIpAddr, g_acBrMask);
        if (lRet == 0)
        {
            if (IpisInNet(acSrcIp, g_acPPPoEIpAddr, g_acPPPoEMask))
            {
                printsip("Sip: parseBind(), Cur OutIface not proxy, pkt is Lan\r\n");
                // g_lPacketDir = 0;
                return lRet;
            }
            printsip("Sip: ip:[%s] is not Lan Ip. \r\n", acSrcIp);
            // g_lPacketDir = 1;
            return 1;
        }
        // g_lPacketDir = 0;
        /* Wan接口和Eth接口均没有绑定，则符合条件 */
        if (('\0' == g_acWanBindEth[0]) && ('\0' == g_acEthBind[0]))
        {
            printsip("Sip: Bind OK Wan && Lan has No Bind. \r\n");
            return 1;
        }

        /* 读取该报文的Eth接口 */
        /* 根据ARP表查找MAC地址 */
        lRet = 0;
        num = 0;
        f = fopen("/proc/net/arp", "r");
        if (NULL == f)
        {
            printsip("sip: Bind Nok open arp failed.\r\n");
            return lRet;
        }
    
        while (fgets(acLine, 128, f))
        {
            num++;
            if (1 == num)
            {
                continue;
            }
            sscanf(acLine, "%s %s %s %s", acArpIp, acTemp, acTemp, acArpMac);
            if ((0 == strcmp(acArpIp, acSrcIp))
                && (0 == strcmp(acTemp, "0x2")))
            {
                lRet = 1;
                break;
            }
        }    
        fclose(f);
    
        if (0 == lRet)
        {
            printsip("Sip: Bind Nok no arp.\r\n");
            /* 没有arp，发送一个udp请求，获得mac地址，以查找绑定关系 */
            sock=socket (PF_INET, SOCK_DGRAM, IPPROTO_UDP);
            if (sock < 0) 
            {
               printsip("Sip: socket() call failed \r\n");
               return lRet;
            }
            stDstAddr.sin_family = AF_INET;
            inet_aton(acSrcIp, &stDstAddr.sin_addr);
            stDstAddr.sin_port= htons(1234);
            sendto(sock, "hello world", strlen("hello world"), 0,
                         (const struct sockaddr *)&stDstAddr,
                         (socklen_t)sizeof(stDstAddr));
            printsip("Send Pkt to %s\r\n", acSrcIp);
            close(sock);
            return lRet;
        }
    
        /* 根据MAC地址表查找端口 */
        lRet = 0;
        num = 0;
        /*BEGIN PN:2071008409 l00170266 for var problem modify 20120725 */
        sprintf(acLine, "brctl forwan br0 > %s", ALG_MAC);
        system(acLine);
        f = fopen(ALG_MAC, "r");
        /*END PN:2071008409 l00170266 for var problem modify 20120725 */
        if (NULL == f)
        {
            printsip("sip: Bind Nok open mac failed.\r\n");
            return lRet;
        }    
        while (fgets(acLine, 128, f))
        {
            acLan[0] = '\0';
            num++;
            if (1 == num)
            {
                if (NULL != strstr(acLine, "lanname"))
                {
                    continue;
                }
                else
                {
                    /* 没有扩展命令<brctl forwan br0>则认为绑定成功 */
                    lRet = 1;
                    break;
                }
            }
            /* MAC地址表较多时不再查找，直接返回OK，认为绑定成功 */
            if (num > 256)
            {
                lRet = 1;
                break;
            }
            
            sscanf(acLine, "%d %s %s\n", &lBrctlPort, acBrctlMac, acLan);
            if (0 == strcmp(acBrctlMac, acArpMac))
            {
                lRet = 1;
                break;
            }
        }    
        fclose(f);
    
        if (0 == lRet)
        {
            printsip("Sip: Bind Nok brctl Tbl no mac = ArpTblMac:[%s].\r\n", acArpMac);
            return lRet;
        }
    
        printsip("Sip: WanBindEth[%s] lan[%s] \r\n", g_acWanBindEth, acLan);
        
        if ('\0' != g_acWanBindEth[0])
        {
            /* 当该LAN接口在当前WAN的绑定列表中，或者当前LAN接口不在任何绑定列表中，符合条件 */
            if ((NULL != strstr(g_acWanBindEth, acLan))
                || (NULL == strstr(g_acEthBind, acLan)))
            {
                lRet = 1;
                printsip("Sip: Bind Ok, WanBindEth[%s], Lan[%s].\r\n", g_acWanBindEth, acLan);
                return lRet;
            }
            else
            {
                lRet = 0;
                printsip("Sip: Bind Nok, WanBindEth[%s], Lan[%s].\r\n", g_acWanBindEth, acLan);
                return lRet;
            }
        }
        else
        {
            if(NULL != strstr(g_acEthBind, acLan))
            {
                lRet = 0;
                printsip("Sip: Bind Nok, EthBind[%s], Lan[%s]. \r\n", g_acEthBind, acLan);
                return lRet;
            }
            else
            {
                lRet = 1;
                printsip("Sip: Bind Ok, EthBind[%s], Lan[%s]. \r\n", g_acEthBind, acLan);
                return lRet;
            }            
        } 
    }
    else
    {   /* 是ppp代理 */
        /* 不是LAN侧报文，不做处理 */
        lRet = IpisInNet(acSrcIp, g_acPPPoEIpAddr, g_acPPPoEMask);
        if (lRet == 0)
        {
            if (IpisInNet(acSrcIp, g_acBrIpAddr, g_acBrMask))
            {
                printsip("Sip: parseBind(), Cur OutIface proxy, pkt is Lan\r\n");
                // g_lPacketDir = 0;
                return lRet;
            }
            printsip("Sip: ip:[%s] is not Lan Ip \r\n", acSrcIp);
            // g_lPacketDir = 1;
            return 1;
        }
        // g_lPacketDir = 0;
        /* 为PPPoE Proxy 查找是否有源地址路由,Lan侧源地址 */
        num = 0;
        lRet = 0;
        /*BEGIN PN:2071008409 l00170266 for var problem modify 20120725 */
        sprintf(acLine, "ip rule > %s", ALG_IPRULE);
        system(acLine);
        f = fopen(ALG_IPRULE, "r");
        /*END PN:2071008409 l00170266 for var problem modify 20120725 */
        if (NULL == f)
        {
            printsip("Sip: Bind Nok open iprule failed.\r\n");
            return lRet;
        }

        while (fgets(acLine, 128, f))
        {
            sscanf(acLine, "%s %s %s %s %s", 
                acTemp, acTemp, acSource, acTemp, acTable);
            if (0 == strcmp(acSource, acSrcIp))
            {
                lRet = 1;
                break;
            }
        }    
        fclose(f);

        /* 有源地址路由的情况 */
        if (1 == lRet)
        {
            /* default dev ppp_8_35_1 scope link */
            lRet = 0;
            num = 0;
            /*BEGIN PN:2071008409 l00170266 for var problem modify 20120725 */
            sprintf(acLine, "ip route show table %s > %s", acTable, ALG_IPROUTE);
            system(acLine);
            f = fopen(ALG_IPROUTE, "r");
            /*END PN:2071008409 l00170266 for var problem modify 20120725 */
            if (NULL == f)
            {
                printsip("Sip: Bind Nok open iproute failed.\r\n");
                return lRet;
            }

            while (fgets(acLine, 128, f))
            {
                sscanf(acLine, "%s %s %s", acDest, acTemp, acIfc);
                if ((0 == strcmp(acDest, "default"))
                    && (0 == strcmp(acIfc, acSipOutIf)))
                {
                    lRet = 1;
                    break;
                }
            }    
            fclose(f);

            if (0 == lRet)
            {
                printsip("Sip: Bind Nok no default route.\r\n");
                return lRet;
            }
            else
            {
                printsip("Sip: Bind Ok [%s] route dev [%s]. \r\n", acDest, acIfc);
                return lRet;
            }
        }
        else
        {
            printsip("Sip: Bind Nok PPP Proxy has no src:[%s] route. \r\n", acSrcIp);
            return lRet;
        }
    }
}
#endif
