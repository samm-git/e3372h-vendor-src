/*
 *   This software is Copyright 2011 by Sean Groarke <sgroarke@gmail.com>
 *   All rights reserved.
 *
 *   This file is part of npd6.
 *
 *   npd6 is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   npd6 is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with npd6.  If not, see <http://www.gnu.org/licenses/>.
 */

/* $Id: ip6.c 81 2011-10-07 11:45:42Z npd6Project@gmail.com $
 * $HeadURL: https://npd6.googlecode.com/svn/trunk/ip6.c $
  <问题单号>        <作  者>    <修改时间>   <版本> <修改描述>
  2121101411  s00201037   2012/12/25          fortify清零
  2081504600  m00182044   2012/8/15           添加组播刷新功能，防止驱动异常导致ns处理失败
  3051406497  h00192681   2013/05/14          9X25主线合入路由多WAN框架代码
  3082010042  y00188255   2013/08/23          消除fortify报警
  4032401612  y00248130   2014/03/24          优化代码结构
 */

#include "includes.h"
#include "npd6.h"
#include "search.h"
#include "wanapi_public.h"

#ifndef CMSG_SPACE
#define	CMSG_SPACE(l) \
	((unsigned int)_CMSG_HDR_ALIGN(sizeof (struct cmsghdr) + (l)))
#endif
#ifndef CMSG_LEN
#define	CMSG_LEN(l) \
	((unsigned int)_CMSG_DATA_ALIGN(sizeof (struct cmsghdr)) + (l))
#endif

unsigned char               nabuff[MAX_PKT_BUFF];
unsigned char               nsbuff[MAX_PKT_BUFF];
char __attribute__((aligned(8))) chdr[CMSG_SPACE(sizeof(struct in6_pktinfo))];
extern unsigned char   msgdata[MAX_MSG_SIZE *2];

/*****************************************************************************
 * processNS
 *  Takes a received Neighbor Solicitation and handles it. Main logic is:
 *      - bit of extra validation.
 *      - determine who it's asking about.
 *      - see if that matches the prefix we are looking after.
 *      - if it does, send a Neighbor Advertisement.
 *
 *  For more, see the inline comments - There's a lot going on here.
 *
 * Inputs:
 *  char *msg
 *      The received NS.
 *  int len
 *      The length of the received (candidate) NS.
 *      *** This has already been sanity checked back in the callers ***
 *
 * Outputs:
 *  Potentially, sends the Neighbor Advertisement.
 *
 * Return:
 *      void
 *
 */
void processNS( unsigned char *msg,
                unsigned int len)
{
    struct in6_pktinfo          *pkt_info;
    struct nd_neighbor_advert   *nad;
    struct cmsghdr              *cmsg;
    struct nd_opt_hdr           *opthdr;
    // Offsets into the received packet
    struct icmp6_hdr  			*icmph =
    (struct icmp6_hdr *)(msg + ETH_HLEN + sizeof( struct ip6_hdr));
    struct nd_neighbor_solicit      ns;
    struct ip6_hdr 				ip6h ;
    // String representations of the various addresses
    char                        targetaddr_str[INET6_ADDRSTRLEN + 2];
    char                        prefixaddr_str[INET6_ADDRSTRLEN + 2];
    char                        srcaddr_str[INET6_ADDRSTRLEN + 2];
    char                        dstaddr_str[INET6_ADDRSTRLEN + 2];
    // Extracted from the received packet
    struct in6_addr             *srcaddr;
    struct in6_addr             *dstaddr;
    struct in6_addr             *targetaddr;
    unsigned int                multicastNS;
    /*BEGIN 2081504600 majiangtao 2012-08-15 added*/
    char switchifname[IFNAMSIZ] = {0};
    /*END 2081504600 majiangtao 2012-08-15 added*/
    
    // For outgoing NA
    struct in6_addr             srcLinkAddr = IN6ADDR_ANY_INIT;
    
    struct sockaddr_in6         sockaddr;
    
    size_t                      iovlen=0;
    struct iovec                iov;
    
    struct msghdr               mhdr;
    ssize_t                     err;
	
    

    // Validate ICMP packet type, to ensure filter was correct
    // In theory not required, as the filter CAN'T be wrong...!
    memcpy(&ip6h, (msg + ETH_HLEN), sizeof(struct ip6_hdr));
    memcpy(&ns, (struct nd_neighbor_solicit *)(msg + ETH_HLEN + sizeof( struct ip6_hdr)), sizeof(struct nd_neighbor_solicit));
    if ( icmph->icmp6_type == ND_NEIGHBOR_SOLICIT )
    {
        flog(LOG_DEBUG2, "Confirmed packet as icmp6 Neighbor Solicitation.");
        srcaddr = &ip6h.ip6_src;
        dstaddr = &ip6h.ip6_dst;
        if (debug)
        {
            print_addr(srcaddr, srcaddr_str);
            print_addr(dstaddr, dstaddr_str);
            flog( LOG_DEBUG, "src addr = %s", srcaddr_str);
            flog( LOG_DEBUG, "dst addr = %s", dstaddr_str);
        }
    }
    else
    {
        flog(LOG_ERR, "Received impossible packet... filter failed. Oooops.");
        return;
    }

    // Bug 27 - Handle DAD NS as per RFC4862, 5.4.3
    if ( IN6_IS_ADDR_UNSPECIFIED(srcaddr) )
    { 
        flog(LOG_DEBUG, "Unspecified src addr - DAD activity. Ignoring NS.");
        return;
    }
        
    // Based upon the dstaddr, record if this was a unicast or multicast NS.
    // If unicast, we'll use that later when we decide whether to add the
    // target link-layer option to any outgoing NA.
    if ( IN6_IS_ADDR_MULTICAST(dstaddr) )
    {
        // This was a multicast NS
        flog(LOG_DEBUG2, "Multicast NS");
        multicastNS = 1;
    }else
    {
        // This was a unicast NS
        flog(LOG_DEBUG2, "Unicast NS");
        /*BEGIN 2081504600 majiangtao 2012-08-15 added*/
		/* Set allmulti on the interface */
        sleep(1);
        while (0 > npd6getwan(switchifname))
        {
            sleep(1);
        }
        if_allmulti(switchifname, TRUE);
        /*END 2081504600 majiangtao 2012-08-15 added*/
        multicastNS=0;
    }

    // Within the NS, who are they looking for?
    targetaddr = (struct in6_addr *)&(ns.nd_ns_target);
    if (debug)
    {
        print_addr16(targetaddr, targetaddr_str);
        flog(LOG_DEBUG, "NS target addr: %s", targetaddr_str);
    }

    // If tgt-addr == dst-addr then ignore this, as the automatic mechanisms
    // will reply themselves - we don't need to.
    if ( nsIgnoreLocal && (!memcmp(targetaddr, dstaddr, sizeof(struct in6_addr))) )
    {
        flog(LOG_DEBUG, "tgt==dst - Ignore.");
        return;
    }
    // Check for black or white listing compliance

    switch (listType) {
        case NOLIST:
            flog(LOG_DEBUG2, "Neither white nor black listing in operation.");
            break;

        case BLACKLIST:
            // If active and tgt is in the list, bail.
            if ( tfind( (void *)targetaddr, &lRoot, tCompare) )
            {
                if (!debug)
                {
                    print_addr16(targetaddr, targetaddr_str);
                }
                flog(LOG_INFO, "NS for blacklisted addr: %s", targetaddr_str);
                return;
            }
            break;

        case WHITELIST:
            // If active and tgt is NOT in the list, bail.
            if ( !tfind( (void *)targetaddr, &lRoot, tCompare) )
            {
                if (!debug)
                {
                    print_addr16(targetaddr, targetaddr_str);
                }
                flog(LOG_INFO, "NS for addr not whitelisted: %s", targetaddr_str);
                return;
            }
            break;
    }

    // Does it match our configured prefix that we're interested in?
    if (( !addr6match( targetaddr, NULL, prefixaddrlen) )
        && (!processLocalNS(targetaddr, multicastNS,srcLinkAddr)))
    {
        flog(LOG_DEBUG, "Target/:prefix - send NS to lan update neighbour list.");
        return;
    }
    else
    {
        flog(LOG_DEBUG, "Target:prefix - Build NA response.");
        // If configured, log target to list
		
        if (collectTargets)
        {
            storeTarget( targetaddr );
        }
        // Start building up the header for the packet
        memset(&sockaddr, 0, sizeof(struct sockaddr_in6));
        sockaddr.sin6_family = AF_INET6;
        sockaddr.sin6_port = htons(IPPROTO_ICMPV6);

        // Set the destination of the NA
        memcpy(&sockaddr.sin6_addr, srcaddr, sizeof(struct in6_addr));

        // Set up the NA itself
        memset( nabuff, 0, sizeof(nabuff) );
        nad = (struct nd_neighbor_advert *)nabuff;
        nad->nd_na_type = ND_NEIGHBOR_ADVERT;
        nad->nd_na_code = 0;
        nad->nd_na_cksum = 0;
        if (naRouter)
        {
            nad->nd_na_flags_reserved |= ND_NA_FLAG_SOLICITED | ND_NA_FLAG_ROUTER;
        }
        else
        {
            nad->nd_na_flags_reserved |= ND_NA_FLAG_SOLICITED;
        }
            
        memcpy(&(nad->nd_na_target), targetaddr, sizeof(struct in6_addr) );
		
        flog(LOG_INFO, "multicastNS:%d naLinkOptFlag:%d", multicastNS, naLinkOptFlag);
        if (multicastNS || naLinkOptFlag)
        {
            // If the NS that came in was to a multicast address
            // or if we have forced the option for all packets anyway
            // then add a target link-layer option to the outgoing NA.
            // Per rfc, we must add dest link-addr option for NSs that came
            // to the multicast group addr.
            opthdr = (struct nd_opt_hdr *)(nabuff + sizeof(struct nd_neighbor_advert));
            opthdr->nd_opt_type = ND_OPT_TARGET_LINKADDR;
            opthdr->nd_opt_len = 1; // Units of 8-octets
            memcpy((unsigned char *)(opthdr + 1), (unsigned char *)linkAddr, 6);
            // Build the io vector
            iovlen = sizeof(struct nd_neighbor_advert) + sizeof(struct nd_opt_hdr) + ETH_ALEN;
            iov.iov_len = iovlen;
            iov.iov_base = (caddr_t) nabuff;
        } else
        {
            // The NS was unicast AND the config option was unset.
            // Build the io vector
            iovlen = sizeof(struct nd_neighbor_advert);
            iov.iov_len = iovlen;
            iov.iov_base = (caddr_t) nabuff;
        }
        
        // Build the cmsg
        memset(chdr, 0, sizeof(chdr) );
        cmsg = (struct cmsghdr *) chdr;
        cmsg->cmsg_len = CMSG_LEN(sizeof(struct in6_pktinfo) );
        cmsg->cmsg_level = IPPROTO_IPV6;
        cmsg->cmsg_type = IPV6_PKTINFO;
        pkt_info = (struct in6_pktinfo *)CMSG_DATA(cmsg);
        // Set src (sending) addr and outgoing i/f for the datagram            
        memcpy(&pkt_info->ipi6_addr, &srcLinkAddr, sizeof(struct in6_addr) );
        pkt_info->ipi6_ifindex = interfaceIdx;        
        
        // Build the mhdr
        memset(&mhdr, 0, sizeof(mhdr) );
        mhdr.msg_name = (caddr_t)&sockaddr;
        mhdr.msg_namelen = sizeof(sockaddr);
        mhdr.msg_iov = &iov;
        mhdr.msg_iovlen = 1;
        mhdr.msg_control = (void *) cmsg;
        mhdr.msg_controllen = sizeof(chdr);

        flog(LOG_DEBUG2, "Outbound message built");

        err = sendmsg( sockicmp, &mhdr, 0);
        if (err < 0)
            flog(LOG_ERR, "sendmsg returned with error %d = %s", errno, strerror(errno));
        else
            flog(LOG_DEBUG2, "sendmsg completed OK data len:%d", err);
        
    }
}

/*****************************************************************************
 * processNSLocal
 *  Takes a received Neighbor Solicitation and handles it. Main logic is:
 *      - bit of extra validation.
 *      - determine who it's asking about.
 *      - see if that matches the prefix we are looking after.
 *      - if it does, send a Neighbor Advertisement.
 *
 *  For more, see the inline comments - There's a lot going on here.
 *
 * Inputs:
 *  char *msg
 *      The received NS.
 *  int len
 *      The length of the received (candidate) NS.
 *      *** This has already been sanity checked back in the callers ***
 *
 * Outputs:
 *  Potentially, sends the Neighbor Advertisement.
 *
 * Return:
 *      void
 *
 */
int processNSLocal( unsigned char *msg,
                unsigned int len)
{
    struct in6_pktinfo          *pkt_info;
    struct nd_neighbor_advert   *nad;
    struct cmsghdr              *cmsg;
    struct nd_opt_hdr           *opthdr;
    // Offsets into the received packet
    struct icmp6_hdr  			*icmph =
    (struct icmp6_hdr *)(msg + ETH_HLEN + sizeof( struct ip6_hdr));
    struct nd_neighbor_solicit      ns;
    struct ip6_hdr 				ip6h ;
    // String representations of the various addresses
    char                        targetaddr_str[INET6_ADDRSTRLEN + 2];
    char                        prefixaddr_str[INET6_ADDRSTRLEN + 2];
    char                        srcaddr_str[INET6_ADDRSTRLEN + 2];
    char                        dstaddr_str[INET6_ADDRSTRLEN + 2];
    // Extracted from the received packet
    struct in6_addr             *srcaddr;
    struct in6_addr             *dstaddr;
    struct in6_addr             *targetaddr;
    unsigned int                multicastNS;
    
    // For outgoing NA
    struct in6_addr             srcLinkAddr = IN6ADDR_ANY_INIT;
    
    struct sockaddr_in6         sockaddr;
    
    size_t                      iovlen=0;
    struct iovec                iov;
    
    struct msghdr               mhdr;
    ssize_t                     err;
	
    

    // Validate ICMP packet type, to ensure filter was correct
    // In theory not required, as the filter CAN'T be wrong...!
    memcpy(&ip6h, (msg + ETH_HLEN), sizeof(struct ip6_hdr));
    memcpy(&ns, (struct nd_neighbor_solicit *)(msg + ETH_HLEN + sizeof( struct ip6_hdr)), sizeof(struct nd_neighbor_solicit));
    if ( icmph->icmp6_type == ND_NEIGHBOR_SOLICIT )
    {
        flog(LOG_DEBUG2, "Confirmed packet as icmp6 Neighbor Solicitation.");
        srcaddr = &ip6h.ip6_src;
        dstaddr = &ip6h.ip6_dst;
        if (debug)
        {
            print_addr(srcaddr, srcaddr_str);
            print_addr(dstaddr, dstaddr_str);
            flog( LOG_DEBUG, "src addr = %s", srcaddr_str);
            flog( LOG_DEBUG, "dst addr = %s", dstaddr_str);
        }
    }
    else
    {
        flog(LOG_ERR, "Received impossible packet... filter failed. Oooops.");
        return 0;
    }

    if (IN6_IS_ADDR_LINKLOCAL(srcaddr) )
    {
        flog(LOG_DEBUG, "Local link addr. Ignoring NS.");
        return 0;
    }
    
    // Bug 27 - Handle DAD NS as per RFC4862, 5.4.3
    if ( IN6_IS_ADDR_UNSPECIFIED(srcaddr) )
    { 
        flog(LOG_DEBUG, "Unspecified src addr - DAD activity. Ignoring NS.");
        return 0;
    }
        
    // Based upon the dstaddr, record if this was a unicast or multicast NS.
    // If unicast, we'll use that later when we decide whether to add the
    // target link-layer option to any outgoing NA.
    if ( IN6_IS_ADDR_MULTICAST(dstaddr) )
    {
        // This was a multicast NS
        flog(LOG_DEBUG2, "Multicast NS ignore");
        multicastNS = 1;
        return 0;
    }else
    {
        // This was a unicast NS
        flog(LOG_DEBUG2, "Unicast NS");
        multicastNS=0;
    }

    // Within the NS, who are they looking for?
    targetaddr = (struct in6_addr *)&(ns.nd_ns_target);
    if (debug)
    {
        print_addr16(targetaddr, targetaddr_str);
        flog(LOG_DEBUG, "NS target addr: %s", targetaddr_str);
    }
 
    // Does it match our configured prefix that we're interested in?
    if (!addr6match( targetaddr, NULL, prefixaddrlen))
    {
        flog(LOG_DEBUG, "Target/:prefix - send NS to lan update neighbour list.");
        return 0;
    }
    else
    {
        flog(LOG_DEBUG, "Target/:prefi lan match success.");
        return 1;
    }
}
/******************************************************************************
  函数名称  : BSP_SYS_WanGetSwitchName
  功能描述  : 获取当前连接的wan口名称
  输入参数  : 
              1.  *pWanName:
 
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  :  0: 成功
              其它: 失败

  修改历史      :

     修改内容   : 完成初稿

******************************************************************************/
int npd6getwan(char *pWanName)
{
    char acCmd[128] = {0};
    char acinterface[64] = {0};
    FILE* pstFile  = NULL;
    unsigned int lStatus = DISABLE_FEATURE;
    unsigned int i = 0;

    if (NULL == pWanName)
    {
        return -1;
    }

    *pWanName = '\0';
    for (i = 0; i < WMR_WANTYPE_ALL; i++)
    {
        switch (i)
        {
        case WMR_WANTYPE_UMTS:
            strcpy(acinterface, WAN_3G_NAME);
            break;
        case WMR_WANTYPE_WIFI:
            strcpy(acinterface, WAN_STA_NAME);
            break;
        case WMR_WANTYPE_CRADLE:
            strcpy(acinterface, WAN_CRADLE_NAME);
            break;
        default:
            strcpy(acinterface, WAN_3G_NAME);
            break;
        }

        sprintf(acCmd, "/var/wan/%s/switch", acinterface);
        pstFile = fopen(acCmd, "r");
        if (NULL != pstFile)
        {
            if (fgets(acCmd, 127, pstFile))
            {
                lStatus = atoi(acCmd);
                if ((ENABLE_FEATURE < lStatus) || (DISABLE_FEATURE >= lStatus))
                {
                    lStatus = DISABLE_FEATURE;
                }
                else
                {
                    fclose(pstFile);
                    strcpy(pWanName, acinterface);
                    return i;
                }
            }

            fclose(pstFile);
        }
    }

    return -1;
}

/*****************************************************************************
 * addr6match
 *      Compare two binary ipv6 addresses and see if they match
 *      in the first N bits.
 *
 * Inputs:
 *  a1 & a2 are the addresses to be compared, in form in6_addr.
 *  bits is the number of bits to compare, starting from the left.
 *
 * Outputs:
 *  void
 *
 * Return:
 *      1 if we match, else 0.
 *
 */
int addr6match( struct in6_addr *a1, struct in6_addr *a2, int bits)
{
    int idx, bdx;
    FILE *fp = NULL;
    char wanname[64] = {0};
    char acCmd[256] = {0};
    char acLine[256] = {0};
    char acAddr[256] = {0};
    char tmpacAddr[256] = {0};
    char aclinkaddr[12] = {0};
    char *pcEnd = NULL;
    int ulRaLen = 0;
    struct in6_addr addr;
	
    bits = 116;//终端E5 IPV6前缀长度
    getLinkaddress("br0", aclinkaddr);
    if (0 <= npd6getwan(wanname))
    {
        /*更新RA的前缀信息*/
        snprintf(acCmd, 256, "/var/wan/%s/prefix6", wanname);
        fp = fopen(acCmd, "r");
        if (NULL != fp)
        {
            while (fgets(acLine, 256, fp))
            {
                sscanf(acLine, "%s %*d\n", acAddr);
                pcEnd = strchr(acAddr, '/');
                if (NULL != pcEnd)
                {
                    *pcEnd = '\0';
                    pcEnd++;
                    ulRaLen = atoi(pcEnd);
                }
                if (0 >= strlen(acAddr))
                {
                    fclose(fp);
                    flog(LOG_DEBUG2, "Match failed acAddr NULL");
                    return 0;
                }
                prefixaddrlen = 64;
                if (strcmp(prefixaddrstr, acAddr))
                {
                    snprintf(acCmd, sizeof(acCmd), "ip -6 route add %s/%d dev br0",  acAddr, prefixaddrlen);
                    system(acCmd);
                    flog(LOG_DEBUG2, "addr6match acCmd:%s", acCmd);
                }
                strncpy(prefixaddrstr, acAddr, sizeof(prefixaddrstr) - 1);
                prefixaddrstr[sizeof(prefixaddrstr) - 1] = '\0';
                snprintf(tmpacAddr, 256 - strlen(acAddr), "%02x%02x:%02x%02x:%02x%02x::",
                	aclinkaddr[0], aclinkaddr[1], aclinkaddr[2],aclinkaddr[3], aclinkaddr[4], aclinkaddr[5]);
                memcpy((acAddr + strlen(acAddr) - 1) , tmpacAddr, strlen(tmpacAddr));
                flog(LOG_DEBUG2, "addr6match acAddr:%s", acAddr);
                inet_pton(AF_INET6, acAddr, &addr);
                if (a1)
                {
                	for (bdx=1,idx=0; bdx<=bits; bdx+=8, idx++)
                	{

                		flog(LOG_DEBUG2, "Match s:%x--d:%x", 
                				 a1->s6_addr[idx], addr.s6_addr[idx]);
                		if ( a1->s6_addr[idx] != addr.s6_addr[idx])
                		{
                		      if (addr6matchex(a1))
                                {
                                    fclose(fp);
                                    flog(LOG_DEBUG2, "Target and matched ex up to bit position %d", bits);
                                    return 1;
                                }      
                			flog(LOG_DEBUG2, "Match failed at bit position %d", bdx);
                			fclose(fp);
                			return 0;
                		}
                	}
                }
            }
            fclose(fp);
        }
    }

    flog(LOG_DEBUG2, "Target and prefix matched up to bit position %d", bits);

    return 1;
}

int addr6matchex( struct in6_addr *a1)
{
    int idx, bdx;
    char acCmd[256] = {0};
    char acLine[256] = {0};
    char acAddr[256] = {0};
    struct in6_addr addr;
    FILE *fp = NULL;

    sprintf(acCmd, "ip -6 neigh show dev br0");

    /* Execute the command e.g "ip -6 neigh show" */
    if ((fp = popen(acCmd, "r")) == NULL) 
    {
    	flog(LOG_DEBUG2, "open cmd:%s failure", acCmd);
    	return 0;
    }
    while (fgets(acLine, 256, fp))
    {
        sscanf(acLine, "%s %*s\n", acAddr);
        strcpy(prefixaddrstr, acAddr);
        flog(LOG_DEBUG2, "addr6matchex acAddr:%s", acAddr);
        inet_pton(AF_INET6, acAddr, &addr);
        if (a1)
        {
        	for (bdx=65,idx=8; bdx<=128; bdx+=8, idx++)
        	{

        		flog(LOG_DEBUG2, "Match s:%x--d:%x", 
        				 a1->s6_addr[idx], addr.s6_addr[idx]);
        		if ( a1->s6_addr[idx] != addr.s6_addr[idx])
        		{
                        continue;
        		}
        	}
            pclose(fp);
            return 1;
        }
    }
    flog(LOG_DEBUG2, "Match failed at bit position %d", bdx);
    pclose(fp);
    return 0;
}

/*****************************************************************************
 * processLocalNS
 *      send neigh solict to lan bridge.
 *
 * Inputs:
 *  targetaddrare the addresses sendto, in form in6_addr.
 *  multicastNS multicast flag.
 *
 * Outputs:
 *  int
 *
 * Return:
 *      1 if we match, else 0.
 *
 */
int processLocalNS(struct in6_addr *targetaddr,  unsigned int  multicastNS,
                                struct in6_addr srcLinkAddr)
{
        size_t                      iovlen=0;
        struct iovec                iov;
        int                          msglen = 0;
        struct msghdr               mhdr;
        ssize_t                     err = 0;
        struct nd_opt_hdr           *opthdr;
        struct sockaddr_in6         sockaddr;
        struct nd_neighbor_solicit *nas;
        struct in6_pktinfo          *pkt_info;
        struct nd_neighbor_advert   *nad;
        struct cmsghdr              *cmsg;
    // Start building up the header for the packet
        
        memset(&sockaddr, 0, sizeof(struct sockaddr_in6));
        sockaddr.sin6_family = AF_INET6;
        sockaddr.sin6_port = htons(IPPROTO_ICMPV6);

        // Set the destination of the NA
        memcpy(&sockaddr.sin6_addr, targetaddr, sizeof(struct in6_addr));

        // Set up the NA itself
        memset( nsbuff, 0, sizeof(nsbuff) );
        nas = (struct nd_neighbor_solicit *)nsbuff;
        nas->nd_ns_type = ND_NEIGHBOR_SOLICIT;
        nas->nd_ns_code = 0;
        nas->nd_ns_cksum = 0;
        if (naRouter)
        {
            nas->nd_ns_reserved |= ND_NA_FLAG_SOLICITED | ND_NA_FLAG_ROUTER;
        }
        else
        {
            nas->nd_ns_reserved |= ND_NA_FLAG_SOLICITED;
        }
            
        memcpy(&(nas->nd_ns_target), targetaddr, sizeof(struct in6_addr) );
		
        flog(LOG_INFO, "multicastNS:%d naLinkOptFlag:%d", multicastNS, naLinkOptFlag);
        if (multicastNS || naLinkOptFlag)
        {
            // If the NS that came in was to a multicast address
            // or if we have forced the option for all packets anyway
            // then add a target link-layer option to the outgoing NA.
            // Per rfc, we must add dest link-addr option for NSs that came
            // to the multicast group addr.
            opthdr = (struct nd_opt_hdr *)(nsbuff + sizeof(struct nd_neighbor_solicit));
            opthdr->nd_opt_type = ND_OPT_SOURCE_LINKADDR;
            opthdr->nd_opt_len = 1; // Units of 8-octets
            memcpy((unsigned char *)(opthdr + 1), (unsigned char *)linkAddr, 6);
            // Build the io vector
            iovlen = sizeof(struct nd_neighbor_advert) + sizeof(struct nd_opt_hdr) + ETH_ALEN;
            iov.iov_len = iovlen;
            iov.iov_base = (caddr_t) nsbuff;
        } else
        {
            // The NS was unicast AND the config option was unset.
            // Build the io vector
            iovlen = sizeof(struct nd_neighbor_solicit);
            iov.iov_len = iovlen;
            iov.iov_base = (caddr_t) nsbuff;
        }
        
        // Build the cmsg
        memset(chdr, 0, sizeof(chdr) );
        cmsg = (struct cmsghdr *) chdr;
        cmsg->cmsg_len = CMSG_LEN(sizeof(struct in6_pktinfo) );
        cmsg->cmsg_level = IPPROTO_IPV6;
        cmsg->cmsg_type = IPV6_PKTINFO;
        pkt_info = (struct in6_pktinfo *)CMSG_DATA(cmsg);
        // Set src (sending) addr and outgoing i/f for the datagram            
        memcpy(&pkt_info->ipi6_addr, &srcLinkAddr, sizeof(struct in6_addr) );
        pkt_info->ipi6_ifindex = laninterfaceIdx;        
        
        // Build the mhdr
        memset(&mhdr, 0, sizeof(mhdr) );
        mhdr.msg_name = (caddr_t)&sockaddr;
        mhdr.msg_namelen = sizeof(sockaddr);
        mhdr.msg_iov = &iov;
        mhdr.msg_iovlen = 1;
        mhdr.msg_control = (void *) cmsg;
        mhdr.msg_controllen = sizeof(chdr);

        flog(LOG_DEBUG2, "Outbound message built");

        err = sendmsg( sockicmp, &mhdr, 0);
        if (err < 0)
        {
            flog(LOG_ERR, "sendmsg returned with error %d = %s", errno, strerror(errno));
            err = 0;
        }
        else
        {
            flog(LOG_DEBUG2, "sendmsg completed OK data len:%d", err);
            msglen = get_rx(msgdata, lansockpkt, 5);
            if (msglen <= 0)
            {
                return msglen;
            }
            err = processNSLocal(msgdata, msglen);
            
        }
        
    return err;
}


