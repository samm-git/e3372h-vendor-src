/*
 * Copyright 2012 Daniel Drown
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * clatd.c - tun interface setup and main event loop
 *  日期                 单号               修改人           修改原因
 * 2014-01-07    3110710642     n00202065      解决CLATD不支持udp分片问题
 */
#include <poll.h>
#include <signal.h>
#include <time.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <netinet/ip6.h>
#include <netinet/icmp6.h>
#include <linux/icmp.h>

#include <linux/capability.h>
#include <sys/uio.h>
#include <linux/prctl.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <linux/if_ether.h>

#include <private/android_filesystem_config.h>

#include "ipv4.h"
#include "ipv6.h"
#include "translate.h"
#include "clatd.h"
#include "config.h"
#include "logging.h"
#include "setif.h"
#include "setroute.h"
#include "mtu.h"
#include "getaddr.h"
#include "dump.h"

#define DEVICENAME6 "clat6"
#define DEVICENAME4 "clat4"
#define CLAT_FILE_PID            ("/var/clat/pid")
#ifdef ATP
#define RUNPATH "/system/atp/sbin/"
#else
#define RUNPATH "busybox "
#endif

int forwarding_fd = -1;
volatile sig_atomic_t running = 1;

struct tun_data {
  char device6[IFNAMSIZ], device4[IFNAMSIZ];
  int fd6, fd4;
};

/* function: clat_delete_route
 * delete route info for clat4/clat6
 * added by m00172998 20130725
 */
void clat_delete_route(struct tun_data *tunnel)
{
  char acCmd[128];   
  int ret;  

  /* delete ipv6 route */  
  memset(acCmd, 0, sizeof(acCmd));  
  snprintf(acCmd, sizeof(acCmd), RUNPATH "ifconfig %s 0.0.0.0", tunnel->device6);
  logmsg(ANDROID_LOG_FATAL,"%s",acCmd); 
  ret = system(acCmd);
  logmsg(ANDROID_LOG_FATAL,"%s %d",strerror(errno),ret);   
    
  deconfigure_tun_ipv6(tunnel);

    /* delete nat rules */  
  memset(acCmd, 0, sizeof(acCmd));  
  snprintf(acCmd, sizeof(acCmd), "iptables -t nat -D POSTROUTING -o %s -j SNAT --to-source %s", 
            tunnel->device4, inet_ntoa(Global_Clatd_Config.ipv4_local_subnet));  
  logmsg(ANDROID_LOG_FATAL,"%s",acCmd); 
  ret = system(acCmd);
  logmsg(ANDROID_LOG_FATAL,"%s %d",strerror(errno),ret);   

  /* delete ipv4 route */
  memset(acCmd, 0, sizeof(acCmd));  
  snprintf(acCmd, sizeof(acCmd), RUNPATH "ifconfig %s 0.0.0.0", tunnel->device4);
  logmsg(ANDROID_LOG_FATAL,"%s",acCmd); 
  ret = system(acCmd);
  logmsg(ANDROID_LOG_FATAL,"%s %d",strerror(errno),ret);         
    
  memset(acCmd, 0, sizeof(acCmd));   
  snprintf(acCmd, sizeof(acCmd), RUNPATH "route del -host %s dev %s", 
            inet_ntoa(Global_Clatd_Config.ipv4_virtual_gatway), tunnel->device4);   
  logmsg(ANDROID_LOG_FATAL,"%s",acCmd);    
  ret = system(acCmd);   
  logmsg(ANDROID_LOG_FATAL, RUNPATH "%s %d",strerror(errno),ret);   
 
  memset(acCmd, 0, sizeof(acCmd));   
  snprintf(acCmd, sizeof(acCmd), RUNPATH "route del default gw %s dev %s", 
            inet_ntoa(Global_Clatd_Config.ipv4_virtual_gatway), tunnel->device4); 
  logmsg(ANDROID_LOG_FATAL,"%s",acCmd);  
  ret = system(acCmd);
  logmsg(ANDROID_LOG_FATAL,"%s %d",strerror(errno),ret);  

  /* SNAT规则仍由cms进程flush */   
   
  memset(&Global_Clatd_Config, '\0', sizeof(Global_Clatd_Config));  
  return;  
}

/* function: clat_save_pid
 * record pid for clatd
 * added by m00172998 20130725
 */
void clat_save_pid(void)
{
    int fd = 0;
    char acCmd[10] = {0};

    snprintf(acCmd, sizeof(acCmd), "%d", getpid());
    if (0 != access("/var/clat", F_OK))
    {
        mkdir("/var/clat", 0755);
    }
    fd = open(CLAT_FILE_PID, O_CREAT | O_WRONLY | O_TRUNC, 0755);
    if(-1 != fd)
    {
        write(fd, acCmd, strlen(acCmd));
        close(fd);
    }    
    return;
}
/* function: set_forwarding
 * enables/disables ipv6 forwarding
 */
void set_forwarding(int fd, const char *setting) {
  /* we have to forward packets from the WAN to the tun interface */
  if(write(fd, setting, strlen(setting)) < 0) {
    logmsg(ANDROID_LOG_FATAL,"set_forwarding(%s) failed: %s", setting, strerror(errno));
    exit(1);
  }
}

/* function: stop_loop
 * signal handler: stop the event loop
 */
void stop_loop(int signal) {
  running = 0;
}

/* function: tun_open
 * tries to open the tunnel device
 */
int tun_open() {
  int fd;

  fd = open("/dev/tun", O_RDWR);
  if(fd < 0) {
    fd = open("/dev/net/tun", O_RDWR);
  }

  return fd;
}

/* function: tun_alloc
 * creates a tun interface and names it
 * dev - the name for the new tun device
 */
int tun_alloc(char *dev, int fd) {
  struct ifreq ifr;
  int err;

  memset(&ifr, 0, sizeof(ifr));

  ifr.ifr_flags = IFF_TUN;
  if( *dev ) {
    strncpy(ifr.ifr_name, dev, IFNAMSIZ);
    ifr.ifr_name[IFNAMSIZ-1] = '\0';
  }

  if( (err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0 ){
    close(fd);
    return err;
  }
  strcpy(dev, ifr.ifr_name);
  return 0;
}

/* function: deconfigure_tun_ipv6
 * removes the ipv6 route
 * tunnel - tun device data
 */
void deconfigure_tun_ipv6(const struct tun_data *tunnel) {
  int status;

#ifdef FEATURE_CLAT_USE_RFC6052
  status = if_route(tunnel->device6, AF_INET6, &Global_Clatd_Config.ipv4_converted_address.address,
      Global_Clatd_Config.ipv4_converted_address.len, NULL, 1, 0, ROUTE_DELETE);
#else
  status = if_route(tunnel->device6, AF_INET6, &Global_Clatd_Config.ipv6_local_subnet,
      128, NULL, 1, 0, ROUTE_DELETE);
#endif  
  if(status < 0) {
    logmsg(ANDROID_LOG_WARN,"deconfigure_tun_ipv6/if_route(6) failed: %s",strerror(-status));
  }
}

/* function: configure_tun_ipv6
 * configures the ipv6 route
 * note: routes a /128 out of the (assumed routed to us) /64 to the CLAT interface
 * tunnel - tun device data
 */
void configure_tun_ipv6(const struct tun_data *tunnel) {
  struct in6_addr local_nat64_prefix_6;
  int status;

#ifdef FEATURE_CLAT_USE_RFC6052  
  status = if_route(tunnel->device6, AF_INET6, &Global_Clatd_Config.ipv4_converted_address.address,
      Global_Clatd_Config.ipv4_converted_address.len, NULL, 1, 0, ROUTE_CREATE);
#else   /* use SNAT */
  status = if_route(tunnel->device6, AF_INET6, &Global_Clatd_Config.ipv6_local_subnet,
      128, NULL, 1, 0, ROUTE_CREATE);  
#endif
  if(status < 0) {
    logmsg(ANDROID_LOG_FATAL,"configure_tun_ipv6/if_route(6) failed: %s",strerror(-status));
    exit(1);
  }
}

/* function: interface_poll
 * polls the uplink network interface for address changes
 * tunnel - tun device data
 */
void interface_poll(const struct tun_data *tunnel) {
  union anyip *interface_ip;

  interface_ip = getinterface_ip(Global_Clatd_Config.default_pdp_interface, AF_INET6);
  if(!interface_ip) {
    logmsg(ANDROID_LOG_WARN,"unable to find an ipv6 ip on interface %s",Global_Clatd_Config.default_pdp_interface);
    return;
  }

  config_generate_local_ipv6_subnet(&interface_ip->ip6);

  if(!IN6_ARE_ADDR_EQUAL(&interface_ip->ip6, &Global_Clatd_Config.ipv6_local_subnet)) {
    char from_addr[INET6_ADDRSTRLEN], to_addr[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET6, &Global_Clatd_Config.ipv6_local_subnet, from_addr, sizeof(from_addr));
    inet_ntop(AF_INET6, &interface_ip->ip6, to_addr, sizeof(to_addr));
    logmsg(ANDROID_LOG_WARN, "clat subnet changed from %s to %s", from_addr, to_addr);

    // remove old route
    deconfigure_tun_ipv6(tunnel);

    // add new route, start translating packets to the new prefix
    memcpy(&Global_Clatd_Config.ipv6_local_subnet, &interface_ip->ip6, sizeof(struct in6_addr));
    configure_tun_ipv6(tunnel);
  }

  free(interface_ip);
}

/* function: configure_tun_ip
 * configures the ipv4 and ipv6 addresses on the tunnel interface
 * tunnel - tun device data
 */
void configure_tun_ip(const struct tun_data *tunnel) {
  int status;
  char acCmd[128];   
  int ret;

  if((status = if_up(tunnel->device6, Global_Clatd_Config.ipv6mtu)) < 0) {
    logmsg(ANDROID_LOG_FATAL,"configure_tun_ip/if_up(6) failed: %s",strerror(-status));
    exit(1);
  }
  configure_tun_ipv6(tunnel);
  
  /* setup default ipv4 route */
  memset(acCmd, 0, sizeof(acCmd));  
  snprintf(acCmd, sizeof(acCmd), RUNPATH "ifconfig %s %s netmask 255.255.255.255 mtu %d", 
            tunnel->device4, inet_ntoa(Global_Clatd_Config.ipv4_local_subnet), Global_Clatd_Config.ipv4mtu);
  logmsg(ANDROID_LOG_FATAL,"%s",acCmd); 
  ret = system(acCmd);
  logmsg(ANDROID_LOG_FATAL,"%s %d",strerror(errno),ret);         
    
  memset(acCmd, 0, sizeof(acCmd));   
  snprintf(acCmd, sizeof(acCmd), RUNPATH "route add -host %s dev %s", 
            inet_ntoa(Global_Clatd_Config.ipv4_virtual_gatway), tunnel->device4);   
  logmsg(ANDROID_LOG_FATAL,"%s",acCmd);    
  ret = system(acCmd);   
  logmsg(ANDROID_LOG_FATAL,"%s %d",strerror(errno),ret);   
 
  memset(acCmd, 0, sizeof(acCmd));   
  snprintf(acCmd, sizeof(acCmd), RUNPATH "route add default gw %s dev %s", 
            inet_ntoa(Global_Clatd_Config.ipv4_virtual_gatway), tunnel->device4); 
  logmsg(ANDROID_LOG_FATAL,"%s",acCmd);  
  ret = system(acCmd);
  logmsg(ANDROID_LOG_FATAL,"%s %d",strerror(errno),ret);   
  
#ifdef FEATURE_CLAT_USE_RFC6052  
  NULL;  
#else  /* use SNAT */
  memset(acCmd, 0, sizeof(acCmd));        
  snprintf(acCmd, sizeof(acCmd), "iptables -t nat -A POSTROUTING -o %s -j SNAT --to-source %s", 
            tunnel->device4, inet_ntoa(Global_Clatd_Config.ipv4_local_subnet));   
  logmsg(ANDROID_LOG_FATAL,"%s",acCmd);  
  ret = system(acCmd); 
  logmsg(ANDROID_LOG_FATAL,"%s %d",strerror(errno),ret);   
  
  memset(acCmd, 0, sizeof(acCmd));
  snprintf(acCmd, sizeof(acCmd), "conntrack -F");      
  logmsg(ANDROID_LOG_FATAL,"%s",acCmd);  
  ret = system(acCmd);   
  logmsg(ANDROID_LOG_FATAL,"%s %d",strerror(errno),ret);          
#endif    

  return;    
}

/* function: drop_root
 * drops root privs but keeps the needed capability
 */
void drop_root() {
  #if 0 
  gid_t groups[] = { AID_INET };
  if(setgroups(sizeof(groups)/sizeof(groups[0]), groups) < 0) {
    logmsg(ANDROID_LOG_FATAL,"drop_root/setgroups failed: %s",strerror(errno));
    exit(1);
  }

  prctl(PR_SET_KEEPCAPS, 1, 0, 0, 0);

  if(setgid(AID_CLAT) < 0) {
    logmsg(ANDROID_LOG_FATAL,"drop_root/setgid failed: %s",strerror(errno));
    exit(1);
  }
  if(setuid(AID_CLAT) < 0) {
    logmsg(ANDROID_LOG_FATAL,"drop_root/setuid failed: %s",strerror(errno));
    exit(1);
  }

  struct __user_cap_header_struct header;
  struct __user_cap_data_struct cap;
  memset(&header, 0, sizeof(header));
  memset(&cap, 0, sizeof(cap));

  header.version = _LINUX_CAPABILITY_VERSION;
  header.pid = 0; // 0 = change myself
  cap.effective = cap.permitted = (1 << CAP_NET_ADMIN);

  if(capset(&header, &cap) < 0) {
    logmsg(ANDROID_LOG_FATAL,"drop_root/capset failed: %s",strerror(errno));
    exit(1);
  }  
  #endif      
}

/* function: display_clat_parameter_all
 * display Global_Clatd_Config on terminal
 * added by m00172998 20130725   
 */
void display_clat_parameter_all(void)
{      
  char ipv6_local_subnet[INET6_ADDRSTRLEN] = {0};
  char ipv6_host_id[INET6_ADDRSTRLEN] = {0};
  char plat_subnet[INET6_ADDRSTRLEN] = {0};  
  char ipv4_local_subnet[16] = {0};
  char ipv4_virtual_gateway[16] = {0};
  char ipv4_converted_address[INET6_ADDRSTRLEN] = {0};  
  
  inet_ntop(AF_INET6, &Global_Clatd_Config.ipv6_local_subnet, ipv6_local_subnet, sizeof(ipv6_local_subnet));   
  inet_ntop(AF_INET6, &Global_Clatd_Config.ipv6_host_id, ipv6_host_id, sizeof(ipv6_host_id));      
  inet_ntop(AF_INET6, &Global_Clatd_Config.plat_subnet, plat_subnet, sizeof(plat_subnet));  
  inet_ntop(AF_INET6, &Global_Clatd_Config.ipv4_converted_address.address, ipv4_converted_address, sizeof(ipv4_converted_address));    
  snprintf(ipv4_local_subnet, sizeof(ipv4_local_subnet), "%s", inet_ntoa(Global_Clatd_Config.ipv4_local_subnet)); 
  snprintf(ipv4_virtual_gateway, sizeof(ipv4_virtual_gateway), "%s", inet_ntoa(Global_Clatd_Config.ipv4_virtual_gatway));    
  
  #define IP_LF "\r\n"    
  printf(IP_LF"Global_Clatd_Config display below :"IP_LF\
            "ipv6_mtu=%d"IP_LF\
            "ipv4_mtu=%d"IP_LF\
            "ipv6_local_subnet=%s"IP_LF\
            "ipv6_host_id=%s"IP_LF\
            "ipv4_converted_address=%s"IP_LF\
            "ipv4_converted_prefixlen=%d"IP_LF\
            "ipv4_local_subnet=%s"IP_LF\
            "ipv4_virtual_gateway=%s"IP_LF\
            "ipv4_translatable_address=%s"IP_LF\
            "nat64-discovery-heuristic-hostname=%s"IP_LF\
            IP_LF,
            Global_Clatd_Config.ipv6mtu,
            Global_Clatd_Config.ipv4mtu,
            ipv6_local_subnet,
            ipv6_host_id,
            ipv4_converted_address,
            Global_Clatd_Config.ipv4_converted_address.len,
            ipv4_local_subnet,
            ipv4_virtual_gateway,
            plat_subnet,
            Global_Clatd_Config.plat_from_dns64_hostname); 
    return;        
}
/* function: configure_interface
 * reads the configuration and applies it to the interface
 * uplink_interface - network interface to use to reach the ipv6 internet
 * plat_prefix      - PLAT prefix to use
 * tunnel           - tun device data
 */
void configure_interface(const char *config_file, const char *uplink_interface, const char *plat_prefix, struct tun_data *tunnel) {
  int error;

  if(!read_config(config_file, uplink_interface, plat_prefix)) {
    logmsg(ANDROID_LOG_FATAL,"read_config failed");
    exit(1);
  }

  if(Global_Clatd_Config.ipv6mtu > MAXMTU) {
    logmsg(ANDROID_LOG_WARN,"Max MTU is %d, requested %d", MAXMTU, Global_Clatd_Config.ipv6mtu);
    Global_Clatd_Config.ipv6mtu = MAXMTU;
  }
  if(Global_Clatd_Config.ipv6mtu <= 0) {
    Global_Clatd_Config.ipv6mtu = getifmtu(Global_Clatd_Config.default_pdp_interface);
    logmsg(ANDROID_LOG_WARN,"ifmtu < 0 , set to %d",Global_Clatd_Config.ipv6mtu);
  }
  if(Global_Clatd_Config.ipv6mtu < 1280) {
    logmsg(ANDROID_LOG_WARN,"mtu too small = %d , set to 1280 !", Global_Clatd_Config.ipv6mtu);
    Global_Clatd_Config.ipv6mtu = 1280;
  }

  if(Global_Clatd_Config.ipv4mtu <= 0 || (Global_Clatd_Config.ipv4mtu > Global_Clatd_Config.ipv6mtu - 20)) {
    Global_Clatd_Config.ipv4mtu = Global_Clatd_Config.ipv6mtu-20;
    logmsg(ANDROID_LOG_WARN,"ipv4mtu now set to = %d",Global_Clatd_Config.ipv4mtu);
  }

  display_clat_parameter_all();

  error = tun_alloc(tunnel->device6, tunnel->fd6);
  if(error < 0) {
    logmsg(ANDROID_LOG_FATAL,"tun_alloc failed: %s",strerror(errno));
    exit(1);
  }

  error = tun_alloc(tunnel->device4, tunnel->fd4);
  if(error < 0) {
    logmsg(ANDROID_LOG_FATAL,"tun_alloc/4 failed: %s",strerror(errno));
    exit(1);
  }
  logmsg(ANDROID_LOG_FATAL,"tunnel device clat4 and clat6 creat succeed");  

  configure_tun_ip(tunnel);
}

/* function: packet_handler
 * takes a tun header and a packet and sends it down the stack
 * tunnel     - tun device data
 * tun_header - tun header
 * packet     - packet
 * packetsize - size of packet
 */
void packet_handler(const struct tun_data *tunnel, struct tun_pi *tun_header, const char *packet,
                    size_t packetsize) {
  int fd;
  int iov_len = 0;

  // Allocate buffers for all packet headers.
  struct tun_pi tun_targ;
  char iphdr[sizeof(struct ip6_hdr) + sizeof(struct ip6_frag)];
  char transporthdr[MAX_TCP_HDR];
  char icmp_iphdr[sizeof(struct ip6_hdr)];
  char icmp_transporthdr[MAX_TCP_HDR];

  // iovec of the packets we'll send. This gets passed down to the translation functions.
  clat_packet out = {
    { &tun_targ, sizeof(tun_targ) },  // Tunnel header.
    { iphdr, 0 },                     // IP header.
    { transporthdr, 0 },              // Transport layer header.
    { icmp_iphdr, 0 },                // ICMP error inner IP header.
    { icmp_transporthdr, 0 },         // ICMP error transport layer header.
    { NULL, 0 },                      // Payload. No buffer, it's a pointer to the original payload.
  };

  if(tun_header->flags != 0) {
    logmsg(ANDROID_LOG_WARN,"packet_handler: unexpected flags = %d", tun_header->flags);
  }

  if(ntohs(tun_header->proto) == ETH_P_IP) {
    fd = tunnel->fd6;
    fill_tun_header(&tun_targ, ETH_P_IPV6);
    iov_len = ipv4_packet(out, CLAT_POS_IPHDR, packet, packetsize);
  } else if(ntohs(tun_header->proto) == ETH_P_IPV6) {
    fd = tunnel->fd4;
    fill_tun_header(&tun_targ, ETH_P_IP);
    iov_len = ipv6_packet(out, CLAT_POS_IPHDR, packet, packetsize);
  } else {
    logmsg(ANDROID_LOG_WARN,"packet_handler: unknown packet type = %x",tun_header->proto);
  }

  if (iov_len > 0) {
    writev(fd, out, iov_len);
  }
}

/* function: read_packet
 * reads a packet from the tunnel fd and passes it down the stack
 * active_fd - tun file descriptor marked ready for reading
 * tunnel    - tun device data
 */
void read_packet(int active_fd, const struct tun_data *tunnel) {
  ssize_t readlen;
  char packet[PACKETLEN];

  // in case something ignores the packet length
  memset(packet, 0, PACKETLEN);

  readlen = read(active_fd,packet,PACKETLEN);

  if(readlen < 0) {
    logmsg(ANDROID_LOG_WARN,"read_packet/read error: %s", strerror(errno));
    return;
  } else if(readlen == 0) {
    logmsg(ANDROID_LOG_WARN,"read_packet/tun interface removed");
    running = 0;
  } else {
    struct tun_pi tun_header;
    ssize_t header_size = sizeof(struct tun_pi);

    if(readlen < header_size) {
      logmsg(ANDROID_LOG_WARN,"read_packet/short read: got %ld bytes", readlen);
      return;
    }

    packet_handler(tunnel, (struct tun_pi *) packet, packet + header_size, readlen - header_size);
  }
}

/* function: event_loop
 * reads packets from the tun network interface and passes them down the stack
 * tunnel - tun device data
 */
void event_loop(const struct tun_data *tunnel) {
  struct pollfd wait_fd[2];

  logmsg(ANDROID_LOG_WARN,"enter event_loop");  
  
  wait_fd[0].fd = tunnel->fd6;
  wait_fd[0].events = POLLIN;
  wait_fd[0].revents = 0;
  wait_fd[1].fd = tunnel->fd4;
  wait_fd[1].events = POLLIN;
  wait_fd[1].revents = 0;

  while(running) {
    if(poll(wait_fd, 2, NO_TRAFFIC_INTERFACE_POLL_FREQUENCY*1000) == -1) {
      if(errno != EINTR) {
        logmsg(ANDROID_LOG_WARN,"event_loop/poll returned an error: %s",strerror(errno));
      }
    } else {
      int i;
      for(i = 0; i < 2; i++) {
        if((wait_fd[i].revents & POLLIN) != 0) {
          read_packet(wait_fd[i].fd,tunnel);
        }
      }
    }    
  }
}

/* function: print_help
 * in case the user is running this on the command line
 */
void print_help() {
  printf("android-clat arguments:\n");
  printf("-i [uplink interface]\n");
  printf("-p [plat prefix]\n");
}

/* function: main
 * allocate and setup the tun device, then run the event loop
 */
int main(int argc, char **argv) {
  struct tun_data tunnel;
  int opt;
  char *uplink_interface = NULL, *plat_prefix = NULL;
  char *config_file = "/var/clat/clatd.conf";

  strcpy(tunnel.device6, DEVICENAME6);
  strcpy(tunnel.device4, DEVICENAME4);

  while((opt = getopt(argc, argv, "i:p:c:h")) != -1) {
    switch(opt) {
      case 'i':
        uplink_interface = optarg;
        break;
      case 'p':
        plat_prefix = optarg;
        break;
      case 'c':
        config_file = optarg;
        break;
      case 'h':
      default:
        print_help();
        exit(1);
        break;
    }
  }

  if(uplink_interface == NULL) {
    logmsg(ANDROID_LOG_FATAL, "clatd called without an interface");
    printf("I need an interface\n");
    exit(1);
  }
  logmsg(ANDROID_LOG_INFO, "Starting clat version %s on %s", CLATD_VERSION, uplink_interface);

  // open the tunnel device before dropping privs
  tunnel.fd6 = tun_open();
  if(tunnel.fd6 < 0) {
    logmsg(ANDROID_LOG_FATAL, "tun_open failed: %s", strerror(errno));
    exit(1);
  }

  tunnel.fd4 = tun_open();
  if(tunnel.fd4 < 0) {
    logmsg(ANDROID_LOG_FATAL, "tun_open4 failed: %s", strerror(errno));
    exit(1);
  }
#if 0
  // open the forwarding configuration before dropping privs
  forwarding_fd = open("/proc/sys/net/ipv6/conf/all/forwarding", O_RDWR);
  if(forwarding_fd < 0) {
    logmsg(ANDROID_LOG_FATAL,"open /proc/sys/net/ipv6/conf/all/forwarding failed: %s",
           strerror(errno));
    exit(1);
  }

  // run under a regular user
  drop_root();

  // When run from netd, the environment variable ANDROID_DNS_MODE is set to
  // "local", but that only works for the netd process itself.
  unsetenv("ANDROID_DNS_MODE");
#endif
  configure_interface(config_file, uplink_interface, plat_prefix, &tunnel);
#if 0
  set_forwarding(forwarding_fd,"1\n");
#endif
  // Loop until someone sends us a signal or brings down the tun interface.
  if(signal(SIGTERM, stop_loop) == SIG_ERR) {
    logmsg(ANDROID_LOG_FATAL, "sigterm handler failed: %s", strerror(errno));
    exit(1);
  }
  clat_save_pid();  
  event_loop(&tunnel);

  clat_delete_route(&tunnel);  
  logmsg(ANDROID_LOG_INFO,"Shutting down clat on %s", uplink_interface);

  return 0;
}