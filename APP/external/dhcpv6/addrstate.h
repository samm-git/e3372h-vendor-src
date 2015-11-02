#ifndef __ADDR_STATE_H__
#define __ADDR_STATE_H__

#include <netinet/in.h>
//#include <sys/types.h>
#include <asm/types.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <pthread.h>


#define WAN_FIRST_GET_IP6_ADDR      (1)
#define INTERFACE_NO_G_ADDR         (0)

#define UNINIT                      (0)
#define INIT                        (1)

#define ADDR_MAX_NUM                (16)


#define NIP6(addr) \
	ntohs((addr).s6_addr16[0]), \
	ntohs((addr).s6_addr16[1]), \
	ntohs((addr).s6_addr16[2]), \
	ntohs((addr).s6_addr16[3]), \
	ntohs((addr).s6_addr16[4]), \
	ntohs((addr).s6_addr16[5]), \
	ntohs((addr).s6_addr16[6]), \
	ntohs((addr).s6_addr16[7])
#define NIP6_FMT "%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x"
#define NIP6_SEQFMT "%04x%04x%04x%04x%04x%04x%04x%04x"


typedef enum
{    
    en_item_free = 0,
    en_item_used = 1,
    en_item_new  = (1 << 2),            /* 新节点需要更新 */
}ADDR_ITEM_STATUS_EN;

/* 状态 */
typedef enum
{
    en_no_addr      = 0,
    en_one_addr     = 1,
    en_more_addr    = 2,
}ADDR_STATE_EN;


/* WAN状态 */
typedef enum
{
    en_wan_down = 0,
    en_wan_up   = 1,
}WAN_STATE_EN;

/* 是否下发 */
typedef enum
{
    en_not_assign   = 0,
    en_do_assign    = 1,
}ADDR_ASSIGN_EN;


typedef struct netlinksock
{
    int                 iSockfd;
    int                 iProtocol;
    int                 iIfIdx;         /* 接口ID */
    uint32_t            iSeq;
    struct sockaddr_nl  stLocalAddr;
    struct sockaddr_nl  stPeerAddr;
}NETLINK_SOCKET_ST;


typedef struct AddrSaveItem
{
    int iFlg;
    int iPrefixLen;
    struct in6_addr stAddr;
}ADDR_ITEM_ST;

typedef struct AddrCnt
{
    int iCnt;
}IPV6_ADDR_CNT_ST;

typedef struct _TagNlReq
{
    struct nlmsghdr     stNlMsgHdr;
    struct ifaddrmsg    stAddr;
    char   acBuff[256]; 
}ADDR_DEL_REQ_ST;

typedef struct _TagNlRouteReq
{
    struct nlmsghdr     stNlMsgHdr;
    struct rtmsg        stRt;
    char                acBuff[256]; 
}ROUTE_DEL_REQ_ST;

typedef struct AddrState
{
    pthread_t tidp;                         /* 线程ID */
    int     iInit;                          /* 是否是直接读取文件 */
    int     ifindex;                        /* 接口ID */
    int     iGlobalAddrCnt;                 /* 全局地址个数 */ 
    int     iWanState;                      /* WAN状态 */
    int     iReadDialFile;                  /* 是否读取dial6文件, 大于0表示不读取 */
    unsigned int  uiDialCmd;                /* 总是存放着上一次的拨号状态，因为会每隔1s就去查询WAN的拨号状态因此
                                                                * 如果本值没有改变，那么就可以继续做以前的事情
                                                                */
#if 0
    int     iState;                         /* 状态 */
#endif
    int     iEnableRA;                      /* 有没有使能RA，因为后续需要根据这个来清除状态 */
    int     iIANAFlag;                      /* 是否申请地址 */
    int     iIAPDFlag;                      /* 有没有配置IAPD */
    int     iDsliteFlag;                    /* 有没有配置Dslite */
    int     iAssignPrefix;                  /* 是否下发前缀 */
    int     iAssignDns;                     /* 是否下发DNS信息 */
    NETLINK_SOCKET_ST stStateMonitor;       /* 状态检测相关 */
    ADDR_ITEM_ST astAddr[ADDR_MAX_NUM];     /* 所有地址 */
    struct AddrState *pstNext;
    //IPV6_ADDR_CNT_ST stAddrCnt;
}ADDRSTATE_ST;

int StartMonitorWanPthread(char *pszIfName, int iEnableRA, 
    int iIANAFlag, int iIAPDFlag, int iDsliteFlag, int iReadDialFile);
int StopMonitorWanPthread(char *pszIfName);
int DelAllGlobalAddrs(ADDRSTATE_ST *pstState);
void StopWanProcessByIndex(int iIfindex);


#endif
