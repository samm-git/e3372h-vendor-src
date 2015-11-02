#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/icmp6.h>
#include <linux/sockios.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <net/if_arp.h>

#include <features.h>
#include <common.h>

#include "sockutils.h"
#include "ra.h"
#include "misc.h"
/*
  <问题单号>     <作  者>    <修改时间>   <版本> <修改描述>
    2121101411  s00201037  2012/12/25        fortify清零
    2032203980  m00182044   2012/6/10 DHCP6c不能通过RA进行无状态IPv6地址配置 
 */



#if (defined (__UCLIBC__) && (__UCLIBC_MAJOR__ == 0) && (__UCLIBC_MINOR__ == 9) && (__UCLIBC_SUBLEVEL__ == 28))
inline struct cmsghdr * fixed_uclibc_cmsg_nxthdr(void *__ctl, __kernel_size_t __size,
					       struct cmsghdr *__cmsg)
{
	struct cmsghdr * __ptr;

	__ptr = (struct cmsghdr*)(((unsigned char *) __cmsg) +  CMSG_ALIGN(__cmsg->cmsg_len));
	if ((unsigned long)((char*)(__ptr+1) - (char *) __ctl) > __size)
		return (struct cmsghdr *)0;

	return __ptr;
}
#endif


int CreateICMPv6Socket(void)
{
    int iSockfd = -1;
    iSockfd = socket(AF_INET6, SOCK_RAW, IPPROTO_ICMPV6);
    if (0 > iSockfd)
    {
        perror("create socket for icmpv6 error");
        return (-1);
    }

    return iSockfd;
}

int SetICMPv6SockOpt(int iSockfd)
{
    int val;
    int ret = -1;
    struct icmp6_filter stIcmpFlt;

    /* 接收其他报文信息 */
    val = 1;
    ret = setsockopt(iSockfd, IPPROTO_IPV6, IPV6_RECVPKTINFO, &val, sizeof(val));
    if (0 > ret)
    {
        perror("setsockopt (IPV6_RECVPKTINFO) error ");
        return (-1);
    }

    /* 设置成2表示由内核完成校验和检查???? */
#if 0
    val = 2;
    ret = setsockopt(iSockfd, SOL_IPV6, IPV6_CHECKSUM, &val, sizeof(val));
    if (0 > ret)
    {
        perror("setsockopt (IPV6_CHECKSUM) error");
        return (-1);
    }
#endif

    /* 控制本socket发送出去的报文的跳数为255 */
    val = 255;
    ret = setsockopt(iSockfd, IPPROTO_IPV6, IPV6_UNICAST_HOPS, &val, sizeof(val));
    if (0 > ret)
    {
        perror("setsockopt (IPV6_UNICAST_HOPS) error");
        return (-1);
    }

    /* 设置本socket发送出去的所有多播报文的跳数为255 */
    val = 255;
    ret = setsockopt(iSockfd, IPPROTO_IPV6, IPV6_MULTICAST_HOPS, &val, sizeof(val));
    if (0 > ret)
    {
        perror("setsockopt (IPV6_MULTICAST_HOPS) error");
        return (-1);
    }

    /* 设置本属性从而告诉recvmsg函数将收到的报文的hop也带上来给用户态，本属性默认没开启从而用户态收不到对应的hop */
    val = 1;
    ret = setsockopt(iSockfd, IPPROTO_IPV6, IPV6_RECVHOPLIMIT, &val, sizeof(val));
    if (0 > ret)
    {
        perror("setsockopt (IPV6_RECVHOPLIMIT) error");
        return (-1);
    }

    /* 设置RAW过滤，只接收RA报文 */
    /*BEGIN 2032203980  m00182044 2012-06-10 modify*/
	#if 1
    /*END 2032203980  m00182044 2012-06-10 modify*/
	ICMP6_FILTER_SETBLOCKALL(&stIcmpFlt);
    ICMP6_FILTER_SETPASS(ND_ROUTER_ADVERT, &stIcmpFlt);
    ret = setsockopt(iSockfd, IPPROTO_ICMPV6, ICMP6_FILTER, &stIcmpFlt, sizeof(stIcmpFlt));
    if (0 > ret)
    {
        perror("setsockopt (ICMP6_FILTER) error");
        return (-1);
    }
	#endif

    // TODO: set socket no-block
    
    return 0;
}

//#define INET6_ADDR_FILE           "/proc/net/if_inet6"

int IsHaveValidLinkAddress(char *pszIfname)
{
    FILE *fp = NULL;
    char  aucAddr[8][5] = {0};
    int ifindex = -1;
    int iPrefixLen = -1;
    int iScope;
    int iFlg;
    int iRet = 0;
    char acIfName[IFNAMSIZ] = {0};
    char acIpv6Addr[64] = {0};
    int iIfidx = -1;
    struct in6_addr stAddr;

    if (!pszIfname)
    {
        loginfo(FUNC, "args NULL\r\n\r\n");
        return (iRet);
    }
    
    iIfidx = if_nametoindex(pszIfname);
    if (0 >= iIfidx)
    {
        loginfo(FUNC, "if_nametoindex error");
        return (iRet);
    }

    fp = fopen(INET6_ADDR_FILE, "r");
    if (!fp)
    {
        loginfo(FUNC, "open file %s error", INET6_ADDR_FILE);
        return (iRet);
    }
    
    while (1)
    {
        if (EOF == fscanf(fp, "%4s%4s%4s%4s%4s%4s%4s%4s %7x %7x %7x %7x %20s\n",
            aucAddr[0], aucAddr[1], aucAddr[2], aucAddr[3], aucAddr[4],
            aucAddr[5], aucAddr[6], aucAddr[7], &ifindex, &iPrefixLen,
            &iScope, &iFlg, acIfName))
        {
            loginfo(FUNC, "read file [%s] over", INET6_ADDR_FILE);
            break;
        }

        if (ifindex != iIfidx)
        {
            loginfo(FUNC, "skip %s", acIfName);
            continue;
        }

        snprintf(acIpv6Addr, 64, "%s:%s:%s:%s:%s:%s:%s:%s",
            aucAddr[0], aucAddr[1], aucAddr[2], aucAddr[3], aucAddr[4],
            aucAddr[5], aucAddr[6], aucAddr[7]);

        if (0 >= inet_pton(AF_INET6, acIpv6Addr, &stAddr))
        {
            loginfo(FUNC, "address error");
            continue;
        }

        if (iFlg & IFA_F_TENTATIVE)
        {
            loginfo(FUNC, "address is temp , can not use......\r\n\r\n");
            continue;
        }
        
        if (IN6_IS_ADDR_LINKLOCAL(&stAddr))
        {
            loginfo(FUNC, "get link local address or multicast address [%s]", acIpv6Addr);
            iRet = 1;
            break;
        }
    }

    fclose(fp);
    
    return (iRet);
}



int GetLinkAddress(struct in6_addr *pstAddr, int iIfIdx)
{
    FILE *fp = NULL;
    char  aucAddr[8][5] = {0};
    int ifindex = -1;
    int iPrefixLen = -1;
    int iScope;
    int iFlg;
    char acIfName[20] = {0};
    char acIpv6Addr[64] = {0};
    struct in6_addr stAddr;

    if (!pstAddr)
    {
        loginfo(FUNC, "nothing to do");
        return (0);
    }

    fp = fopen(INET6_ADDR_FILE, "r");
    if (!fp)
    {
        loginfo(FUNC, "open file %s error", INET6_ADDR_FILE);
        return (-1);
    }
    
    while (1)
    {
        if (EOF == fscanf(fp, "%4s%4s%4s%4s%4s%4s%4s%4s %7x %7x %7x %7x %20s\n",
            aucAddr[0], aucAddr[1], aucAddr[2], aucAddr[3], aucAddr[4],
            aucAddr[5], aucAddr[6], aucAddr[7], &ifindex, &iPrefixLen,
            &iScope, &iFlg, acIfName))
        {
            loginfo(FUNC, "read file [%s] over", INET6_ADDR_FILE);
            break;
        }

        if (ifindex != iIfIdx)
        {
            loginfo(FUNC, "skip %s", acIfName);
            continue;
        }

        snprintf(acIpv6Addr, 64, "%s:%s:%s:%s:%s:%s:%s:%s",
            aucAddr[0], aucAddr[1], aucAddr[2], aucAddr[3], aucAddr[4],
            aucAddr[5], aucAddr[6], aucAddr[7]);

        if (0 >= inet_pton(AF_INET6, acIpv6Addr, &stAddr))
        {
            loginfo(FUNC, "address error");
            continue;
        }
        
        if (iFlg & IFA_F_TENTATIVE)
        {
            loginfo(FUNC, "address is temp , can not use......\r\n\r\n");
            continue;
        }
        
        if (IN6_IS_ADDR_LINKLOCAL(&stAddr))
        {
            loginfo(FUNC, "get link local address or multicast address [%s]", acIpv6Addr);
            memcpy(pstAddr, &stAddr, sizeof(stAddr));
            break;
        }
    }

    fclose(fp);
    
    return (0);
}


static int GetHWAddress(struct sockaddr *pstSockAddr, int iIfindex)
{
    struct ifreq stReq;
    int iSockfd;
    char acIfName[IFNAMSIZ] = {0};

    if (!pstSockAddr)
    {
        loginfo(FUNC, "args NULL");
        return (-1);
    }

    if (NULL == if_indextoname(iIfindex, acIfName))
    {
        loginfo(FUNC, "if_indextoname error");
        return (-1);
    }

    iSockfd = socket(AF_INET6, SOCK_DGRAM, 0);
    if (0 > iSockfd)
    {
        loginfo(FUNC, "Error: socket error");
        return (-1);
    }

    memset(pstSockAddr, 0, sizeof(struct sockaddr));
    memset(&stReq, 0, sizeof(struct ifreq));
    strncpy(stReq.ifr_name, acIfName, (IFNAMSIZ - 1));

    if (0 > ioctl(iSockfd, SIOCGIFHWADDR, &stReq))
    {
        loginfo(FUNC, "Get hwaddress error");
        close(iSockfd);
        return (-1);
    }

    close(iSockfd);

    memcpy(pstSockAddr, &stReq.ifr_hwaddr, sizeof(struct sockaddr));

    return (0);
}


int BindICMPSocket(int iSockfd, int iIfx)
{
    struct sockaddr_in6 stAddr;

    memset(&stAddr, 0, sizeof(stAddr));
    stAddr.sin6_family = AF_INET6;
    GetLinkAddress(&stAddr.sin6_addr, iIfx);
    stAddr.sin6_scope_id = iIfx;

    if (0 > bind(iSockfd, (struct sockaddr *)&stAddr, sizeof(stAddr)))
    {
        loginfo(FUNC, "Error: please check it.. [%s]\r\n\r\n\r\n\r\n", strerror(errno));
        return (-1);
    }

    return (0);
}


/* 构造一个RS报文 */
int GenerateRSPacket(unsigned char *pszBuff, int *iLen, int iIfIndex)
{
    struct nd_router_solicit *pstAddr = NULL;
    struct nd_opt_hdr *pstOptHdr = NULL;
    struct sockaddr    stSockAddr;
    int iPacketLen = 0;
    unsigned char *ptr = NULL;

    if ((!pszBuff) || (!iLen) || (0 == iLen))
    {
        loginfo(FUNC, "args error");
        return (-1);
    }

    memset(pszBuff, 0, *iLen);

    pstAddr = (struct nd_router_solicit *)pszBuff;
    pstAddr->nd_rs_type = (ND_ROUTER_SOLICIT);
    pstAddr->nd_rs_code = 0;
    
    iPacketLen += sizeof(*pstAddr);

    if (0 != GetHWAddress(&stSockAddr, iIfIndex))
    {
        loginfo(FUNC, "Get hw address error");
        return (-1);
    }

    /* ppp接口，ND与IPv6 Over PPP都没有指出这种情况下RS应该填充什么option */
    if (ARPHRD_PPP == stSockAddr.sa_family)
    {
        
    }
    else if (ARPHRD_ETHER == stSockAddr.sa_family)
    {
        pstOptHdr = (struct nd_opt_hdr *)(pstAddr + 1);
        pstOptHdr->nd_opt_type = 1;
        pstOptHdr->nd_opt_len = 1;
        memcpy((pstOptHdr + 1), &stSockAddr.sa_data, 6);
        iPacketLen += 8;   
    }
    
    *iLen = iPacketLen;

    return (0);
}

// TODO:发送给FF02::2之前检查是否本接口加入了这个组
int SendRSPacket(int iSockfd, int iIfIndex)
{
    struct msghdr stMsgHdr;
    struct sockaddr_in6 stDstAddr;
    struct iovec stIOVec;
    unsigned char acBuff[MAX_BUFFER];
    int iPacketLen = MAX_BUFFER;

    
    memset(&stIOVec, 0, sizeof(struct iovec));
    stIOVec.iov_base = (caddr_t)acBuff;
    stIOVec.iov_len = 0;
    
    memset(&stMsgHdr, 0, sizeof(struct msghdr));
    stMsgHdr.msg_name = (void *)&stDstAddr;
    stMsgHdr.msg_namelen = sizeof(struct sockaddr_in6);
    stMsgHdr.msg_iov = &stIOVec;
    stMsgHdr.msg_iovlen = 1;
    memset(&stDstAddr, 0, sizeof(struct in6_addr));

    stDstAddr.sin6_family = AF_INET6;
    stDstAddr.sin6_port = htons(IPPROTO_ICMPV6);
    stDstAddr.sin6_scope_id = iIfIndex;
    if (0 >= inet_pton(AF_INET6, ALL_ROUTE_GROUP, (void *)&stDstAddr.sin6_addr))
    {
        loginfo(FUNC, "inet_pton error");
        return (-1);
    }

    if (0 != GenerateRSPacket(acBuff, &iPacketLen, iIfIndex))
    {
        loginfo(FUNC, "Generate packet for RS error");
        return (-1);
    }
    stIOVec.iov_len = iPacketLen;

    return sendmsg(iSockfd, &stMsgHdr, 0);
}






#define FREE_RES_EXIT()    \
{free(pszCmsgPtr); return (-1);}

/* 参数1: 要从该socket接收报文，参数2: 要将接收到的报文的源地址拷贝回去，参数3: 从该接口索引接收报文
 * 参数4，5: 该报文内容要存放的位置和长度
 */
int RecvRAPacket(int iSockfd, struct sockaddr_in6 *pstFrom, int iRcvDevIfx,
    void *pvBuffer, int iBuffLen, int iAccept)
{
    struct  msghdr          stMsgHdr;
    struct  cmsghdr         *pstCmsgHdr = NULL;  
    struct  in6_pktinfo     *pstPktInfo = NULL;
    struct  iovec           stIov;
    struct  sockaddr_in6    stFrom;
    struct  icmp6_hdr       *pstIcmpHdr = NULL;  
#if 0
    ND_OPT_FMT_LINK_ST      *pstLink    = NULL;
#endif
#if 0
    char                    pszBuff[MAX_BUFFER];
#endif
    int                     *piHopLimit;
    unsigned char           *pszCmsgPtr = NULL;
    int                     iLen = -1;

		loginfo(FUNC, "Enter \r\n");
    if ((!pvBuffer) || (0 >= iBuffLen))
    {
        loginfo(FUNC, "args ERROR\r\n");
        return (-1);
    }

    /* 收到的辅助数据会存放在这个里面 */
    iLen = CMSG_SPACE(sizeof(struct in6_pktinfo)) +
            CMSG_SPACE(sizeof(*piHopLimit));
    pszCmsgPtr = (unsigned char *)malloc(iLen * sizeof(unsigned char));
    if (!pszCmsgPtr)
    {
        perror("malloc error");
        return (-1);
    }

    memset(pvBuffer, 0, iBuffLen);
    memset(&stFrom, 0, sizeof(struct sockaddr_in6));
    memset(pszCmsgPtr, 0, iLen);
    memset(&stIov, 0, sizeof(struct iovec));    
    memset(&stMsgHdr, 0, sizeof(struct msghdr));
#if 0
    stIov.iov_len = MAX_BUFFER;
    stIov.iov_base = (caddr_t)pszBuff;
#else    
    stIov.iov_len = iBuffLen;
    stIov.iov_base = (caddr_t)pvBuffer;
#endif
    stMsgHdr.msg_name = (struct sockaddr *)&stFrom;
    stMsgHdr.msg_namelen = sizeof(struct sockaddr_in6);
    stMsgHdr.msg_iov = &stIov;
    stMsgHdr.msg_iovlen = 1;
    stMsgHdr.msg_control = (caddr_t)pszCmsgPtr;
    stMsgHdr.msg_controllen = iLen;

    iLen = recvmsg(iSockfd, &stMsgHdr, 0);

    if ((0 >= iLen) 
            || (iLen < sizeof(struct icmp6_hdr))
            || !iAccept)
    {
        perror("recvmsg error");
        FREE_RES_EXIT();
    }

#if 0
    printf("received: %d bytes\r\n", iLen);
    int x;
    for (x = 0; x < iLen; x++)
    {
        printf("%02x ", *((unsigned char *)pvBuffer+x));
    }
    printf("\r\n");
#endif

    /* 如果空间不够，那么内核会设置本标志告知用户态 */
    if (stMsgHdr.msg_flags & MSG_TRUNC)
    {
        loginfo(FUNC, "Error: no enough space");
        FREE_RES_EXIT();
    }
    
    /* RFC 4861 6.1.2 IP Source Address is a link-local address. */
#if 0
    if ((stFrom.sin6_addr.s6_addr32[0] & htonl(0xFFC00000)) 
            != htonl(0xFE800000))
#else
    if (!IN6_IS_ADDR_LINKLOCAL(&stFrom.sin6_addr))
#endif
    {
        if (NULL != pstPktInfo)
        {
        loginfo(FUNC, "packet src %08x address is not a link-local address", 
            pstPktInfo->ipi6_addr.s6_addr32[0]);
        }            
        FREE_RES_EXIT();
    }
    
    if (pstFrom)
    {
        memcpy(pstFrom, &stFrom, sizeof(struct sockaddr_in6));
    }

#if (defined (__UCLIBC__) && (__UCLIBC_MAJOR__ == 0) && (__UCLIBC_MINOR__ == 9) && (__UCLIBC_SUBLEVEL__ == 28))
    for (pstCmsgHdr = CMSG_FIRSTHDR(&stMsgHdr);
            NULL != pstCmsgHdr;
            pstCmsgHdr = fixed_uclibc_cmsg_nxthdr(stMsgHdr.msg_control, 
                stMsgHdr.msg_controllen, pstCmsgHdr))
#else
    for (pstCmsgHdr = CMSG_FIRSTHDR(&stMsgHdr);
            NULL != pstCmsgHdr;
            pstCmsgHdr = CMSG_NXTHDR(&stMsgHdr, pstCmsgHdr))
#endif
    {
        if (IPPROTO_IPV6 != pstCmsgHdr->cmsg_level)
        {
            loginfo(FUNC, "notice: cmsghdr level error\r\n");
            continue;
        }
        switch(pstCmsgHdr->cmsg_type)
        {
            case IPV6_HOPLIMIT:
                if (CMSG_LEN(sizeof(int)) != pstCmsgHdr->cmsg_len)
                {
                    loginfo(FUNC, "cmsg_len error\r\n");
                    FREE_RES_EXIT();
                }
                piHopLimit = (int *)(CMSG_DATA(pstCmsgHdr));
                if (255 != *piHopLimit)
                {
                    loginfo(FUNC, "hop count %d error, must 255\r\n", *piHopLimit);
                    FREE_RES_EXIT();
                }
                break;
            case IPV6_PKTINFO:
                if (CMSG_LEN(sizeof(struct in6_pktinfo)) != pstCmsgHdr->cmsg_len)
                {
                    loginfo(FUNC, "CMSG_LEN is not in6_pktinfo size");
                    FREE_RES_EXIT();
                }
                pstPktInfo = (struct in6_pktinfo *)(CMSG_DATA(pstCmsgHdr));
                if (0 < iRcvDevIfx)
                {
                    if (iRcvDevIfx != pstPktInfo->ipi6_ifindex)
                    {
                        loginfo(FUNC, "packet not come from ifindex [%d] come from: [%d]\r\n", 
                            iRcvDevIfx, pstPktInfo->ipi6_ifindex);
                        FREE_RES_EXIT();
                    }
                }
                break;
            default:
                continue;
        }
    }

#if 0
    pstIcmpHdr = (struct icmp6_hdr *)pszBuff;
#else
    pstIcmpHdr = (struct icmp6_hdr *)pvBuffer;
#endif
    if ((0 != pstIcmpHdr->icmp6_code)
        || (16 > iLen)
        || (ND_ROUTER_ADVERT != pstIcmpHdr->icmp6_type))
    {
        loginfo(FUNC, "error: valid RA error");
        FREE_RES_EXIT();
    }


#if 0
    pstLink = BuildTLVList(pszBuff, iLen);
    if (!pstLink)
    {
        LOG_ERR("parse RA message error");
        FREE_RES_EXIT();
    }
    /* 将数据包的源地址拷贝回去 */
    memcpy(&pstLink->stFrom, &stFrom, sizeof(struct sockaddr_in6));
#endif

    if (pszCmsgPtr)
    {
        free(pszCmsgPtr);
        pszCmsgPtr = NULL;
    }

#if 0
    return pstLink;
#endif

    return iLen;
}
#undef FREE_RES_EXIT


#if 0
int SetIfUp(char *pszDev)
{
    struct ifreq stReq;
    int iSockfd = -1;

    if (!pszDev)
    {
        return (-1);
    }

    iSockfd = socket(AF_INET6, SOCK_DGRAM, 0);
    if (0 > iSockfd)
    {
        perror("socket()");
        return (-1);
    }

    memset(&stReq, 0, sizeof(struct ifreq));
    strcpy(stReq.ifr_name, pszDev);
    if (0 > ioctl(iSockfd, SIOCGIFFLAGS, &stReq))
    {
        perror("ioctl error");
        close(iSockfd);
        return (-1);
    }

    if (stReq.ifr_flags & IFF_UP)
    {
        printf("dev: %s already UP\r\n", pszDev);
        close(iSockfd);
        return (0);
    }

    stReq.ifr_flags |= IFF_UP;
    if (0 > ioctl(iSockfd, SIOCSIFFLAGS, &stReq))
    {
        perror("ioctl error");
        close(iSockfd);
        return (-1);
    }

    close(iSockfd);
    return (0);
}
#endif

