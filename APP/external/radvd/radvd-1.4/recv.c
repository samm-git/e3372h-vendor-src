/*
 *   $Id: recv.c,v 1.1 2009/08/22 07:51:04 y42304 Exp $
 *
 *   Authors:
 *    Pedro Roque		<roque@di.fc.ul.pt>
 *    Lars Fenneberg		<lf@elemental.net>	 
 *
 *   This software is Copyright 1996,1997 by the above mentioned author(s), 
 *   All Rights Reserved.
 *
 *   The license which is distributed with this software in the file COPYRIGHT
 *   applies to this software. If your distribution is missing this file, you
 *   may request it from <pekkas@netcore.fi>.
 *
 */

#include <config.h>
#include <includes.h>
#include <radvd.h>

#include <features.h>

#define RADVD_DEBUG 0

/* BEGIN: Added by z67728, 2009/12/3 修改消息查询不能获取下一条消息问题. */
#if (defined (__UCLIBC__) && (__UCLIBC_MAJOR__ == 0) && (__UCLIBC_MINOR__ == 9) && (__UCLIBC_SUBLEVEL__ == 28))
struct cmsghdr * Test_cmsg_nxthdr(void *__ctl, __kernel_size_t __size, struct cmsghdr *__cmsg)
{
	struct cmsghdr * __ptr;

	__ptr = (struct cmsghdr*)(((unsigned char *) __cmsg) +  CMSG_ALIGN(__cmsg->cmsg_len));
	if ((unsigned long)((char*)(__ptr+1) - (char *) __ctl) > __size)
    {
#if RADVD_DEBUG
        printf("\r\n cmsg + cmsg - mhdr : %u , all msg length : %d .\r\n",(unsigned long)((char*)(__ptr+1) - (char *) __ctl),__size);
#endif
        return (struct cmsghdr *)0;
    }   

	return __ptr;
}
#endif
/* END:   Added by z67728, 2009/12/3 */

/* BEGIN: Added by z67728, 2009/12/5 计算校验和 */
typedef struct tag_ICMP_RS_PACK_CHECKSUM_S
{
    unsigned short  usType;               /* icmp type : 58 */
    unsigned short  usLength;             /* icmp packet length */
    struct in6_addr	sin6_saddr;           /* IPv6 src address */
    struct in6_addr	sin6_daddr;           /* IPv6 ds address */
    unsigned char  uszTemBuff[2048];     /* icmp packet content */
}ICMP_RS_PACK_CHECKSUM_S;
/* END:   Added by z67728, 2009/12/5 */

/*****************************************************************************
 函 数 名  : IcmpRsCheckSum
 功能描述  : 计算ICMP报文校验和
 输入参数  : ICMP_RS_PACK_CHECKSUM_S * pstCheckSum  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2009年12月7日
    作    者   : z67728
    修改内容   : 新生成函数

*****************************************************************************/
static int 
IcmpRsCheckSum(ICMP_RS_PACK_CHECKSUM_S * pstCheckSum)
{
    unsigned long  ulCheckSumResult = 0;
    unsigned short usTem            = 0;
    long           lLoopi           = 0;
    
    if ( NULL == pstCheckSum )
    {
        return -1;
    }
    
#if RADVD_DEBUG    
    printf("\r\n Type   : %#x \r\n",pstCheckSum->usType);
    printf("\r\n length : %#x \r\n",pstCheckSum->usLength);
    printf("\r\n Src add : %x %x %x %x  \r\n",htonl(pstCheckSum->sin6_saddr.s6_addr32[0]),htonl(pstCheckSum->sin6_saddr.s6_addr32[1]),htonl(pstCheckSum->sin6_saddr.s6_addr32[2]),htonl(pstCheckSum->sin6_saddr.s6_addr32[3]));
    printf("\r\n DS  add : %x %x %x %x  \r\n",htonl(pstCheckSum->sin6_daddr.s6_addr32[0]),htonl(pstCheckSum->sin6_daddr.s6_addr32[1]),htonl(pstCheckSum->sin6_daddr.s6_addr32[2]),htonl(pstCheckSum->sin6_daddr.s6_addr32[3]));

    for ( lLoopi = 0 ; lLoopi < pstCheckSum->usLength; lLoopi++ )
    {
        if ( 0 == lLoopi % 16 && 0 != lLoopi )
        {
            printf("\r\n");
        }
        
        printf("%02x ",pstCheckSum->uszTemBuff[lLoopi]);    
    }
#endif
    
    ulCheckSumResult += pstCheckSum->usType;
    ulCheckSumResult += pstCheckSum->usLength;

#if RADVD_DEBUG
    printf("\r\n Type length checksum : %#x \r\n",ulCheckSumResult);
#endif

    for ( lLoopi = 0 ; lLoopi < 8 ; lLoopi++ )
    {        
        ulCheckSumResult += htons(pstCheckSum->sin6_saddr.s6_addr16[lLoopi]);
        ulCheckSumResult += htons(pstCheckSum->sin6_daddr.s6_addr16[lLoopi]);
    }
    
#if RADVD_DEBUG
    printf("\r\n addr checksum : %#x \r\n",ulCheckSumResult);
#endif

    if ( NULL != pstCheckSum->uszTemBuff )
    {
        for ( lLoopi = 0 ; lLoopi < pstCheckSum->usLength ; )
        {
            usTem = (((unsigned short)pstCheckSum->uszTemBuff[lLoopi])<<8)&0xff00 | pstCheckSum->uszTemBuff[lLoopi + 1];
#if RADVD_DEBUG            
            printf("\r\n usTem : %#x \r\n",usTem);
#endif
            ulCheckSumResult += usTem;
            lLoopi += 2;
        }
    }

#if RADVD_DEBUG
    printf("\r\n data checksum : %#x \r\n",ulCheckSumResult);
#endif

    ulCheckSumResult += ulCheckSumResult>>16; 
    ulCheckSumResult &= 0xffff;

#if RADVD_DEBUG
    printf("\r\n RS Icmp checksum : %#x \r\n",ulCheckSumResult);
#endif

    if ( 0xffff == ulCheckSumResult )
    {
        return 0;
    }
    else
    {
        return -1;
    }
    
}

int
recv_rs_ra(int sock, unsigned char *msg, struct sockaddr_in6 *addr,
                 struct in6_pktinfo **pkt_info, int *hoplimit)
{                 
	struct msghdr mhdr;
	struct cmsghdr *cmsg;
	struct iovec iov;
	static unsigned char *chdr = NULL;
	static unsigned int chdrlen = 0;
	int len;
	fd_set rfds;

    /* BEGIN: Added by z67728, 2009/12/3 ready认证Request sent to loopback address, 收到目的地址为换回地址的报文直接丢弃.*/
    int            i          = 0;
    unsigned long  ulSum      = 0;
    unsigned short usCheckSum = 0;
    ICMP_RS_PACK_CHECKSUM_S stCheckSum;

    memset(&stCheckSum,0,sizeof(ICMP_RS_PACK_CHECKSUM_S));
    /* END:   Added by z67728, 2009/12/3 */
    
	if( ! chdr )
	{
		chdrlen = CMSG_SPACE(sizeof(struct in6_pktinfo)) +
				CMSG_SPACE(sizeof(int));
		if ((chdr = malloc(chdrlen)) == NULL) {
			flog(LOG_ERR, "recv_rs_ra: malloc: %s", strerror(errno));
			return -1;
		}
	}
    
	FD_ZERO( &rfds );
	FD_SET( sock, &rfds );

	if( select( sock+1, &rfds, NULL, NULL, NULL ) < 0 )
	{
		if (errno != EINTR)
			flog(LOG_ERR, "select: %s", strerror(errno));
		return -1;
	}

    /* BEGIN: Added by z67728, 2009/11/28 */
    memset(msg,0,MSG_SIZE);
    /* END:   Added by z67728, 2009/11/28 */
    
	iov.iov_len = MSG_SIZE;
	iov.iov_base = (caddr_t) msg;

	memset(&mhdr, 0, sizeof(mhdr));
	mhdr.msg_name = (caddr_t)addr;
	mhdr.msg_namelen = sizeof(*addr);
	mhdr.msg_iov = &iov;
	mhdr.msg_iovlen = 1;
	mhdr.msg_control = (void *)chdr;
	mhdr.msg_controllen = chdrlen;
#if RADVD_DEBUG
    printf("\r\n Receive message ! \r\n");
#endif
	len = recvmsg(sock, &mhdr, 0);
	if (len < 0)
	{
		if (errno != EINTR)
			flog(LOG_ERR, "recvmsg: %s", strerror(errno));
		return len;
	}

    stCheckSum.usLength = len;
    memcpy(stCheckSum.uszTemBuff,msg,len);
#if RADVD_DEBUG
    /* BEGIN: Added by z67728, 2009/11/28 */
    printf("\r\n **************** ICMP packet, length : %u **************** \r\n",len);
    for ( i = 0; i < len ; i++ )
    {
        if ( 0 == i % 16 && i != 0 )
        {
            printf("\r\n");
        }
        
        printf("%02x ",stCheckSum.uszTemBuff[i]);
    }

    printf("\r\n ******************** MSG : length[%d] flag[%0x] chdrlen[%d] ********************** \r\n",mhdr.msg_controllen,mhdr.msg_flags,chdrlen);
    
    for ( i = 0; i < chdrlen ; i++ )
    {
        if ( 0 == i % 16 && i != 0 )
        {
            printf("\r\n");
        }
        
        printf("%02x ",chdr[i]);
    }
    /* END:   Added by z67728, 2009/11/28 */
#endif    
	*hoplimit = 0;
    ulSum     = 0;
    
#if (defined (__UCLIBC__) && (__UCLIBC_MAJOR__ == 0) && (__UCLIBC_MINOR__ == 9) && (__UCLIBC_SUBLEVEL__ == 28))
    /* 当前工具链内联函数CMSG_NXTHDR无法找到下一条消息，使用linux内核代码替换 */
    for (cmsg = CMSG_FIRSTHDR(&mhdr); cmsg; cmsg = Test_cmsg_nxthdr(mhdr.msg_control,mhdr.msg_controllen, cmsg))
#else
    for (cmsg = CMSG_FIRSTHDR(&mhdr); cmsg; cmsg = CMSG_NXTHDR(&mhdr, cmsg))
#endif
    {
#if RADVD_DEBUG    
        printf("\r\n Recive RS cmsg->cmsg_type : %d . \r\n",cmsg->cmsg_type);
        if ( 8 == cmsg->cmsg_type )
        {
            printf("\r\n message value : %u . \r\n",*(int *)CMSG_DATA(cmsg));
        }
#endif
        if (cmsg->cmsg_level != IPPROTO_IPV6)
            continue;

        switch(cmsg->cmsg_type)
        {
#ifdef IPV6_HOPLIMIT
              case IPV6_HOPLIMIT:
                if ((cmsg->cmsg_len == CMSG_LEN(sizeof(int))) && 
                    (*(int *)CMSG_DATA(cmsg) >= 0) && 
                    (*(int *)CMSG_DATA(cmsg) < 256))
                {
                  *hoplimit = *(int *)CMSG_DATA(cmsg);
                }
                else
                {
                  flog(LOG_ERR, "received a bogus IPV6_HOPLIMIT from the kernel! len=%d, data=%d",
                  	cmsg->cmsg_len, *(int *)CMSG_DATA(cmsg));
                  return (-1);	
                }  
                break;
#endif /* IPV6_HOPLIMIT */
            case IPV6_PKTINFO:
                if ((cmsg->cmsg_len == CMSG_LEN(sizeof(struct in6_pktinfo))) &&
                ((struct in6_pktinfo *)CMSG_DATA(cmsg))->ipi6_ifindex)
                {
                    *pkt_info = (struct in6_pktinfo *)CMSG_DATA(cmsg);
#if RADVD_DEBUG
                    for ( i = 0 ; i < sizeof(struct in6_pktinfo) ; i++ )
                    {
                        if ( i % 16 == 0 && i != 0 )
                        {
                            printf("\r\n");
                        }

                        printf("%02x ",((unsigned char*)(*pkt_info))[i]);
                    }
#endif
                    /* BEGIN: Added by z67728, 2009/12/3 ready认证Request sent to loopback address, 收到目的地址为换回地址的报文直接丢弃. */
                    for ( i = 0 ; i < sizeof(struct in6_addr) ; i++ )
                    {
                        ulSum += ((unsigned char*)(*pkt_info))[i];
                    }

                    if ( 1 == ulSum && 1 == ((unsigned char*)(*pkt_info))[i - 1] )
                    {
#if RADVD_DEBUG                    
                        printf("\r\n *********** Lookback address ! *************** \r\n");
#else                        
                        flog(LOG_WARNING, "received lookback address from the kernel! len=%d, index=%d", 
                        cmsg->cmsg_len, ((struct in6_pktinfo *)CMSG_DATA(cmsg))->ipi6_ifindex);
#endif
                        return (-1);
                    }
                    /* END:   Added by z67728, 2009/12/3 */
                }
                else
                {
                    flog(LOG_ERR, "received a bogus IPV6_PKTINFO from the kernel! len=%d, index=%d", 
                    cmsg->cmsg_len, ((struct in6_pktinfo *)CMSG_DATA(cmsg))->ipi6_ifindex);
                    return (-1);
                } 
                break;
        }
    }

    stCheckSum.usType = 58;
    memcpy(&stCheckSum.sin6_saddr,&addr->sin6_addr,sizeof(struct in6_addr));
    memcpy(&stCheckSum.sin6_daddr,&((*pkt_info)->ipi6_addr),sizeof(struct in6_addr));
    /* 检验RS报文校验和 */
    if ( 0 > IcmpRsCheckSum(&stCheckSum) )
    {
        printf("\r\n Packet checksum meet error ! \r\n");
        return -1;
    }
    
	dlog(LOG_DEBUG, 4, "recvmsg len=%d", len);

	return len;
}

