/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2013-2015. All rights reserved.
 *
 * mobile@huawei.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#ifndef _FASTIP_TYPEDEFS_H_
#define _FASTIP_TYPEDEFS_H_

#ifndef _UINT8_DEFINED
#define _UINT8_DEFINED
#ifndef TYPEDEF_UINT8
typedef unsigned char	uint8;
#endif
#endif

#ifndef _UINT16_DEFINED
#define _UINT16_DEFINED
#ifndef TYPEDEF_UINT16
typedef unsigned short	uint16;
#endif
#endif

#ifndef _UINT32_DEFINED
#define _UINT32_DEFINED
#ifndef TYPEDEF_UINT32
typedef unsigned int	uint32;
#endif
#endif

#ifndef _UINT64_DEFINED
#define _UINT64_DEFINED
#ifndef TYPEDEF_UINT64
typedef unsigned long long uint64;
#endif
#endif

#ifndef TYPEDEF_UINTPTR
typedef unsigned int	uintptr;
#endif

#ifndef _INT8_DEFINED
#define _INT8_DEFINED
#ifndef TYPEDEF_INT8
typedef signed char	int8;
#endif
#endif

#ifndef _INT16_DEFINED
#define _INT16_DEFINED
#ifndef TYPEDEF_INT16
typedef signed short	int16;
#endif
#endif

#ifndef _INT32_DEFINED
#define _INT32_DEFINED
#ifndef TYPEDEF_INT32
typedef signed int	int32;
#endif
#endif

#ifndef _INT64_DEFINED
#define _INT64_DEFINED
#ifndef TYPEDEF_INT64
typedef signed long long int64;
#endif
#endif


#define FASTIPSWAP16(val) \
	((uint16)((((uint16)(val) & (uint16)0x00ffU) << 8) | \
		  (((uint16)(val) & (uint16)0xff00U) >> 8)))


#define FASTIPSWAP32(val) \
	((uint32)((((uint32)(val) & (uint32)0x000000ffU) << 24) | \
		  (((uint32)(val) & (uint32)0x0000ff00U) <<  8) | \
		  (((uint32)(val) & (uint32)0x00ff0000U) >>  8) | \
		  (((uint32)(val) & (uint32)0xff000000U) >> 24)))


#define FASTIPSWAP32BY16(val) \
	((uint32)((((uint32)(val) & (uint32)0x0000ffffU) << 16) | \
		  (((uint32)(val) & (uint32)0xffff0000U) >> 16)))


#define FASTIP_ETHER_ISBCAST(ea)	((((const uint8 *)(ea))[0] &		\
	                          ((const uint8 *)(ea))[1] &		\
				  ((const uint8 *)(ea))[2] &		\
				  ((const uint8 *)(ea))[3] &		\
				  ((const uint8 *)(ea))[4] &		\
				  ((const uint8 *)(ea))[5]) == 0xff)
#define FASTIP_ETHER_ISNULLADDR(ea)	((((const uint8 *)(ea))[0] |		\
				  ((const uint8 *)(ea))[1] |		\
				  ((const uint8 *)(ea))[2] |		\
				  ((const uint8 *)(ea))[3] |		\
				  ((const uint8 *)(ea))[4] |		\
				  ((const uint8 *)(ea))[5]) == 0)

#define FASTIP_ETHER_ISNULLDEST(da)	((((const uint16 *)(da))[0] |           \
				  ((const uint16 *)(da))[1] |           \
				  ((const uint16 *)(da))[2]) == 0)
				  
#define FASTIP_ETHER_ISNULLSRC(sa)	FASTIP_ETHER_ISNULLDEST(sa)

#define	fastip_ether_cmp(a, b)	(!(((short*)a)[0] == ((short*)b)[0]) | \
			 !(((short*)a)[1] == ((short*)b)[1]) | \
			 !(((short*)a)[2] == ((short*)b)[2]))


#define	fastip_ether_copy(s, d) { \
		((short*)d)[0] = ((short*)s)[0]; \
		((short*)d)[1] = ((short*)s)[1]; \
		((short*)d)[2] = ((short*)s)[2]; }
#ifndef hton16
#ifndef IL_BIGENDIAN
#define HTON16(i) FASTIPSWAP16(i)
#define	HTON32(i) FASTIPSWAP32(i)
#define	NTOH16(i) FASTIPSWAP16(i)
#define	NTOH32(i) FASTIPSWAP32(i)
#define LTOH16(i) (i)
#define LTOH32(i) (i)
#define HTOL16(i) (i)
#define HTOL32(i) (i)
#else /* IL_BIGENDIAN */
#define HTON16(i) (i)
#define	HTON32(i) (i)
#define	NTOH16(i) (i)
#define	NTOH32(i) (i)
#define	LTOH16(i) FASTIPSWAP16(i)
#define	LTOH32(i) FASTIPSWAP32(i)
#define HTOL16(i) FASTIPSWAP16(i)
#define HTOL32(i) FASTIPSWAP32(i)
#endif /* IL_BIGENDIAN */
#endif /* hton16 */

#define FASTIP_NUM_FFFF 0xffff
#define FASTIP_NUM_FF 0xff
#define FASTIP_NUM_36 36
#define FASTIP_NUM_32 32
#define FASTIP_NUM_22 22
#define FASTIP_NUM_18 18
#define FASTIP_NUM_0 0
#define FASTIP_NUM_1 1
#define FASTIP_NUM_2 2
#define FASTIP_NUM_3 3
#define FASTIP_NUM_4 4
#define FASTIP_NUM_5 5
#define FASTIP_NUM_6 6
#define FASTIP_NUM_7 7
#define FASTIP_NUM_8 8
#define FASTIP_NUM_9 9
/* IPV4 and IPV6 common */
#define FASTIP_IP_VER_OFFSET		0x0	/* offset to version field */
#define FASTIP_IP_VER_MASK		0xf0	/* version mask */
#define FASTIP_IP_VER_SHIFT		4	/* version shift */
#define FASTIP_IP_VER_4		4	/* version number for IPV4 */
#define FASTIP_IP_VER_6		6	/* version number for IPV6 */

#define FASTIP_IP_VER(ip_body) \
	((((uint8 *)(ip_body))[FASTIP_IP_VER_OFFSET] & FASTIP_IP_VER_MASK) >> FASTIP_IP_VER_SHIFT)
	
/* IPV6 field decodes */
#define FASTIP_IPV6_TRAFFIC_CLASS(ipv6_body) \
	(((((uint8 *)(ipv6_body))[0] & 0x0f) << 4) | \
	 ((((uint8 *)(ipv6_body))[1] & 0xf0) >> 4))
	 
#define FASTIP_IP_PROT_ICMP		0x1	/* ICMP protocol */
#define FASTIP_IP_PROT_IGMP		0x2	/* IGMP protocol */
#define FASTIP_IP_PROT_TCP		0x6	/* TCP protocol */
#define FASTIP_IP_PROT_UDP		0x11	/* UDP protocol type */
#define FASTIP_IP_PROT_ICMP6		0x3a	/* ICMPv6 protocol type */

#define FASTIP_IPV4_ADDR_LEN		4

#define FASTIP_IPV4_FRAG_RESV		0x8000	/* Reserved */
#define FASTIP_IPV4_FRAG_DONT		0x4000	/* Don't fragment */
#define FASTIP_IPV4_FRAG_MORE		0x2000	/* More fragments */
#define FASTIP_IPV4_FRAG_OFFSET_MASK	0x1fff	/* Fragment offset */

#define	FASTIP_IPV4_TOS_DSCP_MASK	0xfc	/* DiffServ codepoint mask */
#define	FASTIP_IPV4_TOS_DSCP_SHIFT	2	/* DiffServ codepoint shift */

/* IPV4 field offsets */
#define FASTIP_IPV4_VER_HL_OFFSET      0       /* version and ihl byte offset */

#define FASTIP_IPV4_HLEN_MASK		0x0f	/* IPV4 header length mask */
#define FASTIP_IPV4_TOS_OFFSET         1       /* type of service offset */

#define FASTIP_IPV4_HLEN(ptr)	(4 * (((uint8 *)(ptr))[FASTIP_IPV4_VER_HL_OFFSET] & FASTIP_IPV4_HLEN_MASK))
#define	FASTIP_IPV4_TOS(ipv4_body)	(((uint8 *)(ipv4_body))[FASTIP_IPV4_TOS_OFFSET])

#define FASTIP_OK        0    /* Success */
#define FASTIP_ERROR    -1    /* Error generic */
#define FASTIP_BADARG     -2
#define FASTIP_NOTFOUND   -3

#define FASTIP_MAXBRCHOT		4
#define FASTIP_MAXBRCHOTIF		4

#define TRUE  1
#define FALSE 0

#define FASTIP_TCP_SRC_PORT_OFFSET	0	/* TCP source port offset */
#define FASTIP_TCP_DEST_PORT_OFFSET	2	/* TCP dest port offset */
#define FASTIP_TCP_HLEN_OFFSET		12	/* HLEN and reserved bits offset */
#define FASTIP_TCP_FLAGS_OFFSET	13	/* FLAGS and reserved bits offset */
#define FASTIP_TCP_CHKSUM_OFFSET	16	/* TCP body checksum offset */

#define FASTIP_TCP_FLAG_URG            0x0020
#define FASTIP_TCP_FLAG_ACK            0x0010
#define FASTIP_TCP_FLAG_PSH            0x0008
#define FASTIP_TCP_FLAG_RST            0x0004
#define FASTIP_TCP_FLAG_SYN            0x0002
#define FASTIP_TCP_FLAG_FIN            0x0001
#define FASTIP_TCP_HLEN_MASK           0xf000
#define FASTIP_TCP_HLEN_SHIFT          12

#define FASTIP_UDP_DEST_PORT_OFFSET	2	/* UDP dest port offset */
#define FASTIP_UDP_LEN_OFFSET		4	/* UDP length offset */
#define FASTIP_UDP_CHKSUM_OFFSET	6	/* UDP body checksum offset */

#define FASTIP_UDP_HDR_LEN	8	/* UDP header length */
#define FASTIP_UDP_PORT_LEN	2	/* UDP port length */

/* ether types */
#define FASTIP_ETHER_TYPE_MIN		0x0600		/* Anything less than MIN is a length */
#define	FASTIP_ETHER_TYPE_IP		0x0800		/* IP */
#define FASTIP_ETHER_TYPE_ARP		0x0806		/* ARP */
#define FASTIP_ETHER_TYPE_8021Q	0x8100		/* 802.1Q */
#define	FASTIP_ETHER_TYPE_IPV6		0x86dd		/* IPv6 */
#define	FASTIP_ETHER_TYPE_802_1X	0x888e		/* 802.1x */

#define FASTIP_VLAN_PRI_MASK		7	/* 3 bits of priority */
#define FASTIP_VLAN_PRI_SHIFT		13

#define	FASTIP_ETHER_ADDR_LEN		6
#define	FASTIP_ETHER_TYPE_LEN		2
#define	FASTIP_ETHER_CRC_LEN		4
#define FASTIP_ETHER_ADDR_STR_LEN	18	
#define	FASTIP_ETHER_HDR_LEN		(FASTIP_ETHER_ADDR_LEN * 2 + FASTIP_ETHER_TYPE_LEN)

#define	FASTIP_VLAN_TAG_LEN		4
#define	FASTIP_VLAN_TAG_OFFSET		(2 * FASTIP_ETHER_ADDR_LEN)	

#define FASTIP_VLAN_TPID		0x8100	

#define	FASTIP_ETHERVLAN_HDR_LEN	(FASTIP_ETHER_HDR_LEN + FASTIP_VLAN_TAG_LEN)

#define	FASTIP_IPV4_TOS_PREC_MASK	0xe0	/* Historical precedence mask */
#define	FASTIP_IPV4_TOS_PREC_SHIFT	5	/* Historical precedence shift */

#define FASTIP_MAC_BYTE_MAX            (6)


struct fastip_ether_addr
{
    unsigned char octet[FASTIP_MAC_BYTE_MAX];
};

#ifndef _FASTIP_ETHER_ADDR
#define _FASTIP_ETHER_ADDR
typedef struct fastip_ether_addr FASTIP_ETHER_ADDR;
#endif

struct fastip_tcp_hdr
{
	uint16	src_port;	/* Source Port Address */
	uint16	dst_port;	/* Destination Port Address */
	uint32	seq_num;	/* TCP Sequence Number */
	uint32	ack_num;	/* TCP Sequence Number */
	uint16	hdrlen_rsvd_flags;	/* Header length, reserved bits and flags */
	uint16	tcpwin;		/* TCP window */
	uint16	chksum;		/* Segment checksum with pseudoheader */
	uint16	urg_ptr;	/* Points to seq-num of byte following urg data */
};

struct fastip_ipv4_hdr {
	uint8	version_ihl;		/* Version and Internet Header Length */
	uint8	tos;			/* Type Of Service */
	uint16	tot_len;		/* Number of bytes in packet (max 65535) */
	uint16	id;
	uint16	frag;			/* 3 flag bits and fragment offset */
	uint8	ttl;			/* Time To Live */
	uint8	prot;			/* Protocol */
	uint16	hdr_chksum;		/* IP header checksum */
	uint8	src_ip[FASTIP_IPV4_ADDR_LEN];	/* Source IP Address */
	uint8	dst_ip[FASTIP_IPV4_ADDR_LEN];	/* Destination IP Address */
};

struct fastip_ethervlan_header {
	uint8	ether_dhost[FASTIP_ETHER_ADDR_LEN];
	uint8	ether_shost[FASTIP_ETHER_ADDR_LEN];
	uint16	vlan_type;		
	uint16	vlan_tag;		
	uint16	ether_type;
};
#endif /* _TYPEDEFS_H_ */

