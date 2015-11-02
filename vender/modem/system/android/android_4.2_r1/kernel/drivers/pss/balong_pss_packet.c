/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
 * foss@huawei.com
 *
 * If distributed as part of the Linux kernel, the following license terms
 * apply:
 *
 * * This program is free software; you can redistribute it and/or modify
 * * it under the terms of the GNU General Public License version 2 and 
 * * only version 2 as published by the Free Software Foundation.
 * *
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * * GNU General Public License for more details.
 * *
 * * You should have received a copy of the GNU General Public License
 * * along with this program; if not, write to the Free Software
 * * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Otherwise, the following license terms apply:
 *
 * * Redistribution and use in source and binary forms, with or without
 * * modification, are permitted provided that the following conditions
 * * are met:
 * * 1) Redistributions of source code must retain the above copyright
 * *    notice, this list of conditions and the following disclaimer.
 * * 2) Redistributions in binary form must reproduce the above copyright
 * *    notice, this list of conditions and the following disclaimer in the
 * *    documentation and/or other materials provided with the distribution.
 * * 3) Neither the name of Huawei nor the names of its contributors may 
 * *    be used to endorse or promote products derived from this software 
 * *    without specific prior written permission.
 * 
 * * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <net/ip.h>
#include <net/arp.h>
#include <linux/udp.h>
#include <linux/module.h>
#include <linux/etherdevice.h>
#include <linux/inetdevice.h>
#include "balong_pss.h"
#include "bsp_ncm.h"
#define PSS_SKB_LEN    (1512)
#define MACHDR_LEN      (14)
#define IPHDR_LEN       (20)
#define UDPHDR_LEN      (8)
#define PKTHDR_LEN      (MACHDR_LEN + IPHDR_LEN + UDPHDR_LEN)
#define DATA_LEN        (PSS_SKB_LEN - MACHDR_LEN - IPHDR_LEN - UDPHDR_LEN)
#define SKB_PRINT_LEN   (60)
#define PSS_SKB_DATA_LEN    (1460)
#define IP_ADDR_LEN     4
#define ETH_HTONS(x)    ((((x) & 0x00ff) << 8) | \
                        (((x) & 0xff00) >> 8))

#define MAC_TYPE        0x0800
#define IP_HDR_LEN      20
#define PSS_MAGIC      "BSP4"


/****************************build packet**********************************/
#pragma pack(push)
#pragma pack(1)
typedef struct ipv4_tcp_packet {
    struct ethhdr mac_pss;
    struct iphdr ip_pss;
    struct udphdr udp_pss;
}ipv4_udp_pkt_t;
#pragma pack(pop)

typedef struct udp_param {
    unsigned short src_port;
    unsigned short dst_port;
}udp_param_t;

unsigned char srcmac[ETH_ALEN] = {0x00,0Xb4,0x12,0x34,0x56,0x78};
unsigned char dstmac[ETH_ALEN] = {};
unsigned char srcip[IP_ADDR_LEN] = {192,168,74,89};/*192.168.J.Y*/
unsigned char dstip[IP_ADDR_LEN] = {};

static struct udp_param udp_test = {
    .src_port = 6001,
    .dst_port = 6002,
};


unsigned short ip_chksum(unsigned char *data, unsigned short len)
{
    unsigned int chksum  = 0;
    unsigned short *buffer = (unsigned short *)data;;

    while(len > 1)
    {
        chksum += *buffer++;
        len     -= sizeof(unsigned short);
    }

    if (len == 1)
    {
        chksum += *(unsigned char*)buffer;
    }
    chksum  = (chksum >> 16) + (chksum & 0xffff);
    chksum += (chksum >> 16);

    return (unsigned short)(~chksum);
}

static void pss_build_pkthdr(ipv4_udp_pkt_t *packet, unsigned short len)
{
    static unsigned short id_pss = 45557;
        
    /*mac header*/
    memcpy(packet->mac_pss.h_dest, g_pss_man.wdev->dev_addr, ETH_ALEN);
    memcpy(packet->mac_pss.h_source, srcmac, ETH_ALEN);
    packet->mac_pss.h_proto = ETH_HTONS(MAC_TYPE);

    /*ipv4 header*/
    packet->ip_pss.version = 4;
    packet->ip_pss.ihl = 5;
    packet->ip_pss.tos = 0x00;
    packet->ip_pss.tot_len = ETH_HTONS(len + sizeof(struct iphdr)+ sizeof(struct udphdr));
    packet->ip_pss.id = ETH_HTONS(id_pss);
    id_pss++;
    packet->ip_pss.frag_off = ETH_HTONS(IP_DF);
    packet->ip_pss.ttl = 0xff;
    packet->ip_pss.protocol = IPPROTO_UDP;
    packet->ip_pss.check = 0;

    memcpy(&packet->ip_pss.saddr, srcip, IP_ADDR_LEN);
    memcpy(&packet->ip_pss.daddr, &g_pss_man.dstip, IP_ADDR_LEN);

    packet->ip_pss.check = ip_chksum(&packet->ip_pss, IP_HDR_LEN);
    /*udp header*/
    packet->udp_pss.source  = ETH_HTONS(udp_test.src_port);
    packet->udp_pss.dest    = ETH_HTONS(udp_test.dst_port);
    packet->udp_pss.len     = ETH_HTONS(len + sizeof(struct udphdr));
    packet->udp_pss.check   = 0;    
}

static void pss_build_pkt(unsigned char* data, unsigned int len)
{
    pss_build_pkthdr((ipv4_udp_pkt_t *)data, len);
}


static struct sk_buff* packet_alloc_skb(unsigned int len)
{
	struct sk_buff* skb;

	skb = dev_alloc_skb(PSS_SKB_LEN);
	if (!skb) {
		return NULL;
	}
	pss_build_pkt(skb->data, len);
	skb_put(skb, PKTHDR_LEN);
	return skb;	
}


static int netif_rx_any(struct sk_buff *skb)
{
	int ret;
	if (in_irq() || irqs_disabled())
		ret = netif_rx(skb);
	else
		ret = netif_rx_ni(skb);
	return ret;
}

unsigned int pss_upload(unsigned int read, 
		unsigned int write, char *buf_base, unsigned int size, unsigned int max)
{
    struct sk_buff *skb;
	unsigned int rec_in_pkt;
	unsigned int total;
	unsigned int i = 0;
	char* tmp = buf_base + read * size;
	unsigned int count = 0;
	unsigned int read_cur = read;
	int ret;

	rec_in_pkt = g_pss_man.ops->mtu / size;
	total = (write + max - read ) % max;
	count = total / rec_in_pkt;
	while(i < count) {
		skb = packet_alloc_skb(rec_in_pkt * size);
		if (!skb) {
			break;
		}
		if(read_cur + rec_in_pkt <= (max - 1)) {
			memcpy(skb->data + skb->len, tmp, rec_in_pkt * size);
			tmp += rec_in_pkt * size;
			read_cur += rec_in_pkt;		
		} else {
			memcpy(skb->data + skb->len, tmp, (max - read_cur) * size);
			memcpy(skb->data + skb->len + (max - read_cur) * size, 
				buf_base, (read_cur + rec_in_pkt - max) * size);			
			tmp = buf_base +  (read_cur + rec_in_pkt - max)* size;
			read_cur = read_cur + rec_in_pkt - max;
		
		}
		skb_put(skb, rec_in_pkt * size);
		memset(skb->data + skb->len, PSS_MAGIC, strlen(PSS_MAGIC));
		skb_put(skb, strlen(PSS_MAGIC));
#ifdef CONFIG_ETH_BYPASS_MODE
		ret = bsp_ncm_write(0, skb, g_pss_man.wdev);
#else
		skb->protocol =  eth_type_trans(skb, g_pss_man.wdev);
		ret = netif_rx_any(skb);
#endif
		if (ret) {
			printk("%s: flush failed\n", __func__);
			break;
		}
		i++;
		
	}
	
	return i * rec_in_pkt;
}	



struct pss_flush_ops flush_ops = {
	.mtu = PSS_SKB_DATA_LEN,
	.pss_buffer_flush = pss_upload,
};

