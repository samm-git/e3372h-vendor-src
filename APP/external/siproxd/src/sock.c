/*
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
 2013.10.15        s00201037      3092906251    增加sipdns域名解析
******************************************************************************/
#include "config.h"

#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#include <osipparser2/osip_parser.h>

#include "siproxd.h"
#include "log.h"

static char const ident[]="$Id: sock.c,v 1.3 2009/02/27 02:34:21 l41296 Exp $";


/* configuration storage */
extern struct siproxd_config configuration;
/*BEGIN 3092906251 s00201037 2013-10-15 added*/
extern unsigned int lansrcip;
extern int lansrcport;
/*END 3092906251 s00201037 2013-10-15 added*/
/* socket used for sending SIP datagrams */
int sip_udp_socket=0;
   /* start of AU4D00875 by d00107688 to support bind 2008-10-15*/
int sip_udp_wan_socket = 0;
int g_lsockFlg = 0;

int wanport = 56005;
   /* end of AU4D00875 by d00107688 to support bind 2008-10-15*/

#ifdef SUPPORT_BRIDGE_BIND
extern char g_acPPPoEIpAddr[SIP_IPADDR];
extern char g_acPPPoEMask[SIP_IPADDR];
extern char g_acBrIpAddr[SIP_IPADDR];
extern char g_acBrMask[SIP_IPADDR];
#endif


/*
 * binds to SIP UDP socket for listening to incoming packets
 *
 * RETURNS
 *	STS_SUCCESS on success
 *	STS_FAILURE on error
 */
int sipsock_listen (void) {
   struct in_addr ipaddr;
   
   /* start of AU4D00875 by d00107688 to support bind 2008-10-15*/
   struct in_addr wanipaddr;
   /* end of AU4D00875 by d00107688 to support bind 2008-10-15*/

    int i = 0;
   memset(&ipaddr, 0, sizeof(ipaddr));
   /* start of AU4D00875 by d00107688 to support bind 2008-10-15*/
   memset(&wanipaddr, 0, sizeof(wanipaddr));


    if (STS_FAILURE == get_ip_by_ifname(configuration.outbound_if, &wanipaddr))
    {
        ERROR("%s ip: %s", configuration.inbound_if, utils_inet_ntoa(wanipaddr));
        ERROR("can not get the ip , please check it.......");
    }
    else
    {
        g_lsockFlg = 1;
    }
   /* end of AU4D00875 by d00107688 to support bind 2008-10-15*/
   /* start of AU4D00875 by d00107688 to support bind 2008-10-15*/
    if (1 == g_lsockFlg)
    {
   sip_udp_socket = sockbind(ipaddr, configuration.sip_listen_port, 1);
   sip_udp_wan_socket = sockbind(wanipaddr, wanport, 1);
       if (0 == sip_udp_wan_socket)
       {
            for (i = 0; i < 5; i++)
            {
                wanport += (i * 10 + i);
                sip_udp_wan_socket = sockbind(wanipaddr, wanport, 1);
                if (0 == sip_udp_wan_socket)
                {
                    ERROR("bind in port: %d error", wanport);
                    continue;
                }
                else
                {
                    break;
                }
            }
       }
       if ((0 == sip_udp_socket) || (0 == sip_udp_wan_socket)) 
       {
            ERROR("socket bind error");
            return STS_FAILURE; /* failure */
       }
    }
    else
    {
        sip_udp_socket = sockbind(ipaddr, configuration.sip_listen_port, 1);   
   if (sip_udp_socket == 0) return STS_FAILURE; /* failure*/
    }

   INFO("bound to port %i", configuration.sip_listen_port);
   DEBUGC(DBCLASS_NET,"bound socket %i",sip_udp_socket);
   /* end of AU4D00875 by d00107688 to support bind 2008-10-15*/
   return STS_SUCCESS;
}

/*
 * Wait for incoming SIP message. After a 2 sec timeout
 * this function returns with sts=0
 *
 * RETURNS >0 if data received, =0 if nothing received /T/O), -1 on error
 */
   /* start of AU4D00875 by d00107688 to support bind 2008-10-15*/
int sipsock_wait(int *fd) {
   int sts;
   fd_set fdset;
   struct timeval timeout;
   
   /* start of AU4D00875 by d00107688 to support bind 2008-10-15*/
   int max_fd = 0;
   static int counter = 0;
   /* end of AU4D00875 by d00107688 to support bind 2008-10-15*/

   timeout.tv_sec=2;
   timeout.tv_usec=0;

   FD_ZERO(&fdset);

   /* start of AU4D00875 by d00107688 to support bind 2008-10-15*/
   if (1 == g_lsockFlg)
   {
       FD_SET (sip_udp_socket, &fdset);
       FD_SET (sip_udp_wan_socket, &fdset);
       max_fd = (sip_udp_wan_socket > sip_udp_socket? sip_udp_wan_socket : sip_udp_socket);
   }
   else
   {
       FD_SET (sip_udp_socket, &fdset);
       max_fd = sip_udp_socket;
   }

   #if 0
   sts=select (sip_udp_socket+1, &fdset, NULL, NULL, &timeout);
   #else
   sts=select (max_fd + 1, &fdset, NULL, NULL, &timeout);
   #endif
   /* WARN on failures */
   if (sts<0) {
      /* WARN on failure, except if it is an "interrupted system call"
         as it will result by SIGINT, SIGTERM */
      if (errno != 4) {
         WARN("select() returned error [%i:%s]",errno, strerror(errno));
      } else {
         DEBUGC(DBCLASS_NET,"select() returned error [%i:%s]",
                errno, strerror(errno));
      }
   }

   /* 考虑到目前siproxd的处理流程, 因为目前select之后siproxd只能read一个报文，因此如果在
    * select 中有两个数据可读，那么我们只能选出一个可读的。这里为了保证公平性因此需要在这里进行
    * 轮询
    */
    if (0 == counter)
    {
   
        if (FD_ISSET(sip_udp_socket, &fdset))
        {
            *fd = sip_udp_socket;
        }
        else if (FD_ISSET(sip_udp_wan_socket, &fdset))
        {
            *fd = sip_udp_wan_socket;
        }
        counter = 1;
    }
    else 
    {
        if (FD_ISSET(sip_udp_wan_socket, &fdset))
        {
            *fd = sip_udp_wan_socket;
        }
        else if (FD_ISSET(sip_udp_socket, &fdset))
        {
            *fd = sip_udp_socket;
        }
        counter = 0;
    }
   /* end of AU4D00875 by d00107688 to support bind 2008-10-15*/
   return sts;
}

/*
 * read a message from SIP listen socket (UDP datagram)
 *
 * RETURNS number of bytes read
 *         from is modified to return the sockaddr_in of the sender
 */
int sipsock_read(int fd, void *buf, size_t bufsize,
                 struct sockaddr_in *from, int *protocol) {
   int count;
   socklen_t fromlen;
   /* start of AU4D00875 by d00107688 to support bind 2008-10-15*/
    if (fd < 0)
    {
        count = 0;
        return count;
    }
   /* end of AU4D00875 by d00107688 to support bind 2008-10-15*/
   fromlen=sizeof(struct sockaddr_in);
   *protocol = PROTO_UDP; /* up to now, unly UDP */
   #if 0
   count=recvfrom(sip_udp_socket, buf, bufsize, 0,
                  (struct sockaddr *)from, &fromlen);
   #else
   /*BEGIN 3092906251 s00201037 2013-10-15 added*/   
   count=recvfrom(fd, buf, bufsize, 0,
                  from, &fromlen);
   //   count=recvfrom(fd, buf, bufsize, 0,
     //             (struct sockaddr *)from, &fromlen);
   /*END 3092906251 s00201037 2013-10-15 added*/	 
   #endif

   if (count<0) {
      WARN("recvfrom() returned error [%s]",strerror(errno));
      *protocol = PROTO_UNKN;
   }

   DEBUGC(DBCLASS_NET,"received UDP packet from %s, count=%i",
          utils_inet_ntoa(from->sin_addr), count);
   DUMP_BUFFER(DBCLASS_NETTRAF, buf, count);
   /*BEGIN 3092906251 s00201037 2013-10-15 added*/
   lansrcport = 0;
   lansrcport = ntohs(from->sin_port);
   lansrcip = htonl(inet_addr(utils_inet_ntoa(from->sin_addr)));
   /*END 3092906251 s00201037 2013-10-15 added*/

   return count;
}
#ifdef SUPPORT_BRIDGE_BIND

/*
 * sends an UDP datagram to the specified destination
 *
 * RETURNS
 *	STS_SUCCESS on success
 *	STS_FAILURE on error
 *  函数修改说明:
 *      修改人:    l67187
 *      修改目的:  解决代理和绑定时没有默认路由，sip alg无法工作的问题
 *      修改方案:  在报文发送时，bind上网关发送侧的ip地址，使用网关源地址路由
 *      具体修改:  原alg  使用  初始化时创建的 sip_udp_socket，绑定了空地址
 *                 无法再次绑定，修改后，在该函数中不在使用  sip_udp_socket发送报文
 *                 而是重新创建一个socket发送，之后就关闭释放资源
 */
int sipsock_send(struct in_addr addr, int port, int protocol,
                 char *buffer, int size) {
   
   struct sockaddr_in dst_addr;
   struct sockaddr_in stLocalAddr;
   int sock;
   int sts = -1;
   
   /* first time: allocate a socket for sending 
   if (sip_udp_socket == 0) {
      ERROR("SIP socket not allocated");
      return STS_FAILURE;
   }
*/
   if (buffer == NULL) {
      ERROR("sipsock_send got NULL buffer");
      return STS_FAILURE;
   }

   if (protocol != PROTO_UDP) {
      ERROR("sipsock_send: only UDP supported by now");
      return STS_FAILURE;
   }

    if (1 == g_lsockFlg)
    {

        
        dst_addr.sin_family = AF_INET;
        memcpy(&dst_addr.sin_addr.s_addr, &addr, sizeof(struct in_addr));
        dst_addr.sin_port= htons(port);

        if ((1 == IpisInNet(utils_inet_ntoa(addr), g_acBrIpAddr, g_acBrMask))
        || (1 == IpisInNet(utils_inet_ntoa(addr), g_acPPPoEIpAddr, g_acPPPoEMask)))
        {
            
            sts = sendto(sip_udp_socket, buffer, size, 0,
                 (const struct sockaddr *)&dst_addr,
                 (socklen_t)sizeof(dst_addr));
        }
        else  /* wan 侧 */
        {
            // TODO: 这里需要优化，目前PPPoE代理的时候没有默认路由
            // 因此需要进行判断，如果没有默认路由那么就需要使用使用sip_udp_wan_socket
            // 发包，否则一直使用 sip_udp_socket 发包
            // if (没有默认路由)
            if (0 != sip_udp_wan_socket)
            {

                sts = sendto(sip_udp_wan_socket, buffer, size, 0,
                     (const struct sockaddr *)&dst_addr,
                     (socklen_t)sizeof(dst_addr));
            }
            else if (0 == sip_udp_wan_socket)
            {
                sts = sendto(sip_udp_socket, buffer, size, 0,
                     (const struct sockaddr *)&dst_addr,
                     (socklen_t)sizeof(dst_addr));
            }
            else
            {
                ERROR("no proper socket fd to send out");
            }
        }
        
        if (sts == -1) {
           if (errno != ECONNREFUSED) {
              ERROR("sendto() [%s:%i size=%i] call failed: %s",
                    utils_inet_ntoa(addr),
                    port, size, strerror(errno));
              return STS_FAILURE;
           }
           DEBUGC(DBCLASS_BABBLE,"sendto() [%s:%i] call failed: %s",
                  utils_inet_ntoa(addr), port, strerror(errno));
        }
    }
    else
    {
   sock=socket (PF_INET, SOCK_DGRAM, IPPROTO_UDP);
   if (sock < 0) {
      printsip("Sip: socket() call failed: %s",strerror(errno));
      return STS_FAILURE;
   }

   dst_addr.sin_family = AF_INET;
   memcpy(&dst_addr.sin_addr.s_addr, &addr, sizeof(struct in_addr));
   dst_addr.sin_port= htons(port);

   DEBUGC(DBCLASS_NET,"send UDP packet to %s: %i", utils_inet_ntoa(addr),port);
   
   printsip("Sip:send UDP packet to %s: %i\r\n", utils_inet_ntoa(addr),port);
   DUMP_BUFFER(DBCLASS_NETTRAF, buffer, size);


    /* 本地发送源 */
    bzero(&stLocalAddr, 0);
    /* 目的ip为LAN侧ip */
    if ((1 == IpisInNet(utils_inet_ntoa(addr), g_acBrIpAddr, g_acBrMask))
        || (1 == IpisInNet(utils_inet_ntoa(addr), g_acPPPoEIpAddr, g_acPPPoEMask)))
    {
        get_ip_by_ifname(configuration.inbound_if, &stLocalAddr.sin_addr);
    }
    else
    {
        get_ip_by_ifname(configuration.outbound_if, &stLocalAddr.sin_addr);
    }
/*    
    if (g_lPacketDir == 0)
    {
        get_ip_by_ifname(configuration.outbound_if,&stLocalAddr.sin_addr);
//    stLocalAddr.sin_port = htons(atoi(pszLocalPort));
    }
    else
    {
        get_ip_by_ifname(configuration.inbound_if,&stLocalAddr.sin_addr);
//    stLocalAddr.sin_port = htons(atoi(pszLocalPort));
    }
*/
    stLocalAddr.sin_family = AF_INET;
    
    if (bind(sock, (struct sockaddr *)&stLocalAddr, sizeof(stLocalAddr)) < 0)
    {
    	perror("bind");
    }
    else
    {
        printsip("Sip: bind src ip:[%s]\r\n", inet_ntoa(stLocalAddr.sin_addr));
    }
   
    // if (0 != sip_udp_socket)
        #if 0
    if ((1 == IpisInNet(utils_inet_ntoa(addr), g_acBrIpAddr, g_acBrMask))
        || (1 == IpisInNet(utils_inet_ntoa(addr), g_acPPPoEIpAddr, g_acPPPoEMask)))
        #else
        if (0)
        #endif
    {
        sts = sendto(sock, buffer, size, 0,
             (const struct sockaddr *)&dst_addr,
             (socklen_t)sizeof(dst_addr));
   }
   else if (0 != sip_udp_socket)
   {
       sts = sendto(sip_udp_socket, buffer, size, 0,
                 (const struct sockaddr *)&dst_addr,
                 (socklen_t)sizeof(dst_addr));
   }
   else
   {
        printf("no proper socket found \r\n");
   }
   if (sts == -1) {
      if (errno != ECONNREFUSED) {
         ERROR("sendto() [%s:%i size=%i] call failed: %s",
               utils_inet_ntoa(addr),
               port, size, strerror(errno));
         close(sock);
         return STS_FAILURE;
      }
      DEBUGC(DBCLASS_BABBLE,"sendto() [%s:%i] call failed: %s",
             utils_inet_ntoa(addr), port, strerror(errno));
   }
   close(sock);
    }
   return STS_SUCCESS;
}
#else
/*
 * sends an UDP datagram to the specified destination
 *
 * RETURNS
 *	STS_SUCCESS on success
 *	STS_FAILURE on error
 */
int sipsock_send(struct in_addr addr, int port, int protocol,
                 char *buffer, int size) {
   struct sockaddr_in dst_addr;
   int sts;

   /* first time: allocate a socket for sending */
   if (sip_udp_socket == 0) {
      ERROR("SIP socket not allocated");
      return STS_FAILURE;
   }

   if (buffer == NULL) {
      ERROR("sipsock_send got NULL buffer");
      return STS_FAILURE;
   }

   if (protocol != PROTO_UDP) {
      ERROR("sipsock_send: only UDP supported by now");
      return STS_FAILURE;
   }

   dst_addr.sin_family = AF_INET;
   memcpy(&dst_addr.sin_addr.s_addr, &addr, sizeof(struct in_addr));
   dst_addr.sin_port= htons(port);

   DEBUGC(DBCLASS_NET,"send UDP packet to %s: %i", utils_inet_ntoa(addr),port);
   DUMP_BUFFER(DBCLASS_NETTRAF, buffer, size);

   sts = sendto(sip_udp_socket, buffer, size, 0,
                (const struct sockaddr *)&dst_addr,
                (socklen_t)sizeof(dst_addr));
   
   if (sts == -1) {
      if (errno != ECONNREFUSED) {
         ERROR("sendto() [%s:%i size=%i] call failed: %s",
               utils_inet_ntoa(addr),
               port, size, strerror(errno));
         return STS_FAILURE;
      }
      DEBUGC(DBCLASS_BABBLE,"sendto() [%s:%i] call failed: %s",
             utils_inet_ntoa(addr), port, strerror(errno));
   }

   return STS_SUCCESS;
}
#endif


/*
 * generic routine to allocate and bind a socket to a specified
 * local address and port (UDP)
 * errflg !=0 log errors, ==0 don't
 *
 * RETURNS socket number on success, zero on failure
 */
int sockbind(struct in_addr ipaddr, int localport, int errflg) {
   struct sockaddr_in my_addr;
   int sts;
   int sock;
   int flags;
   unsigned char on = 0;

   memset(&my_addr, 0, sizeof(my_addr));

   my_addr.sin_family = AF_INET;
   memcpy(&my_addr.sin_addr.s_addr, &ipaddr, sizeof(struct in_addr));
   my_addr.sin_port = htons(localport);

   sock=socket (PF_INET, SOCK_DGRAM, IPPROTO_UDP);
   if (sock < 0) {
      ERROR("socket() call failed: %s",strerror(errno));
      return 0;
   }
    /* 重用地址，将来便于扩展，以后可能当用户选中接口为auto的时候可以考虑将所有接口全部传入从而一个一个发送尝试 */
    {
        on = 1;
        if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (void *)&on, sizeof(int)) < 0)
        {
            ERROR("set socket reuse addr error : %s", strerror(errno));
        }
   }

   sts=bind(sock, (struct sockaddr *)&my_addr, sizeof(my_addr));
   if (sts != 0) {
      if (errflg) ERROR("bind failed: %s",strerror(errno));
      close(sock);
      return 0;
   }

   /*
    * It has been seen on linux 2.2.x systems that for some
    * reason (bug?) inside the RTP relay, select()
    * claims that a certain file descriptor has data available to
    * read, a subsequent call to read() or recv() then does block!!
    * So lets make the FD's we are going to use non-blocking, so
    * we will at least survive and not run into a deadlock.
    *
    * There is a way to (more or less) reproduce this effect:
    * Make a local UA to local UA call and then very quickly do
    * HOLD/unHOLD, several times.
    */
   flags = fcntl(sock, F_GETFL);
   if (flags < 0) {
      ERROR("fcntl(F_SETFL) failed: %s",strerror(errno));
      close(sock);
      return 0;
   }
   if (fcntl(sock, F_SETFL, (long) flags | O_NONBLOCK) < 0) {
      ERROR("fcntl(F_SETFL) failed: %s",strerror(errno));
      close(sock);
      return 0;
   }
   printsip("Sip: Func(sockbind) ipaddr:[%s],localport:[%d], errflg:[%d] \r\n", 
       inet_ntoa(ipaddr), localport, errflg);
   return sock;
}
