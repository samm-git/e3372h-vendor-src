/*
 * Copyright 2011 Daniel Drown
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
 * config.h - configuration settings
 */
#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <netinet/in.h>
#include <sys/system_properties.h>

#define DEFAULT_IPV6_MTU "1500"
#define DEFAULT_IPV4_MTU "1480"
#define DEFAULT_IPV6_HOST_ID "::1:1:1:1"
#define DEFAULT_IPV4_VIRTUAL_ADDRESS "10.10.10.10"
#define DEFAULT_IPV4_VIRTUAL_GATEWAY "10.10.10.1"
#define DEFAULT_DNS64_DETECTION_HOSTNAME "ipv4.google.com"
/* 以下默认参数为软件大厦22楼MBB测试部CLAT-PLAT环境参数 */
#define DEFAULT_PLAT_IPV6_ADDRESS   "3001::"
#define DEFAULT_PLAT_IPV6_PREFIXLEN "96"
#define DEFAULT_CLAT_IPV6_ADDRESS   "2001:3333:4444:6666::" 
#define DEFAULT_CLAT_IPV6_PREFIXLEN "64"

/* begin 为RFC6052翻译算法扩展结构体，m00172998 20130725 */
struct ipv6_prefix
{
	/** IPv6 prefix. */
	struct in6_addr address;
	/** Number of bits from "addr" which represent the network. */
	unsigned char len;
};
union ipv4_address {
	unsigned int as32;
	unsigned char as8[4];
};
/* end 为RFC6052翻译算法扩展结构体，m00172998 20130725 */

struct clat_config {
  int16_t ipv6mtu, ipv4mtu;
  struct in6_addr ipv6_local_subnet;          /* useless if using RFC6052 m00172998 20130725 */  
  struct in6_addr ipv6_host_id;               /* useless if using RFC6052 m00172998 20130725 */  
  struct ipv6_prefix ipv4_converted_address;  /* used for RFC6052 added by m00172998 20130725 */  
  struct in_addr ipv4_local_subnet;
  struct in_addr ipv4_virtual_gatway;  
  struct in6_addr plat_subnet;                   /* useless if using RFC6052 m00172998 20130725 */   
  struct ipv6_prefix ipv4_translatable_address;  /* used for RFC6052 added by m00172998 20130725 */  
  char default_pdp_interface[PROP_VALUE_MAX];
  char *plat_from_dns64_hostname;
};

extern struct clat_config Global_Clatd_Config;

int read_config(const char *file, const char *uplink_interface, const char *plat_prefix);
void config_generate_local_ipv6_subnet(struct in6_addr *interface_ip);

#endif /* __CONFIG_H__ */
