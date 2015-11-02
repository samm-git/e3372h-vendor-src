#ifndef __SOCK_UTILS_H__
#define __SOCK_UTILS_H__

#include <netinet/in.h>

#define  MAX_BUFFER         (2048)
#define  ALL_ROUTE_GROUP    "ff02::2"
#define IFA_F_TENTATIVE     0X40

int IsHaveValidLinkAddress(char *pszIfname);
int GetLinkAddress(struct in6_addr *pstAddr, int iIfIdx);
int CreateICMPv6Socket(void);
int SetICMPv6SockOpt(int iSockfd);
int RecvRAPacket(int iSockfd, struct sockaddr_in6 *pstFrom, 
    int iRcvDevIfx, void *pvBuffer, int iBuffLen, int iAccept);


#endif
