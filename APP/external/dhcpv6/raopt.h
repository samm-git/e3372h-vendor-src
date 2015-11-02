#ifndef __OPT_H__
#define __OPT_H__

#include <netinet/in.h>
#include <sys/types.h>
#include "ratlv.h"

#define PREFIX_TO_LAN       (1)
#define PREFIX_NO_TO_LAN    (0)

#define MAX_DFTRT_NUM       (16)
#define MAX_PREFIX_NUM      (16)
#define MAX_RDDNS_NUM       (16)

#define ND_OPT_RIO          (24)        /* rfc 4191 */
#define ND_OPT_RDNSS        (25)
#define INFINITY_LFT        (0xffffffff)

#define LIFETIME_TIMEOUT    (0)

/* DNS选项的最小长度 */
#define RA_RDDNS_LEN_MIN    (3)

#define IPV6_ADDR_LEN       (16)

typedef enum 
{
    EN_NEW          = (0x1),            /* 新节点 */
    EN_MODIFY       = (0x1 << 1),       /* 该节点不是新的，被修改 */
    EN_NO_MODIFY    = (0x1 << 2),       /* 节点内容没变 */
    EN_PROCESSED    = (0x1 << 3),       /* 节点被处理过了 */
    EN_SEND         = (0x1 << 4)        /* 该节点被发送给LAN侧了 */
}EN_NODE_FLAGS;

typedef enum
{
    EN_M_CHANGED = 1,
    EN_O_CHANGED = 2,
}EN_M_O_CHG_FLG;


typedef enum
{
    enNothing           = 0,            /* 初始化状态 */
    enMDHCP             = (1 << 1),     /* 通过M启动了DHCP */
    enODHCP             = (1 << 2),     /* 通过O启动了DHCP */
}EN_STATUS;


/* 前缀信息
 * stSrc 并不单独从option中拿出来而是每一个结构比如前缀,DNS等一个是因为考虑到可能多个路由器下发同一个前缀或者DNS信息
 * 同时也为了方便对每一个前缀或者DNS信息进行操作
 */
typedef struct __tagPrefix
{
    struct __tagPrefix  *pstNext;
    int                 bOnLink;            /* 这个值实际上没有什么用 */
    int                 bAutoConf;          /* 这个值实际上也可以不需要，留着这两个是便于扩展 */
    uint32_t            uiLft;
    uint32_t            uiVlft;    
    struct in6_addr     stPrefix;           /* 前缀，网络字节序 */
    uint8_t             iPrefixLen;         
#if 0
    struct timeval      stUpdateT;          /* 该option最后被更新时间 */
#else
    long       lUpTime;           /* 该option最后被更新时间 */
#endif
#if 0
    int                 iIfIndex;           /* 从哪个接口收到的 */
#endif
    int                 iGiveLan;           /* 是否下发给了LAN侧 */
    struct sockaddr_in6 stSrc;              /* 该前缀的来源(路由器的源地址) 已经是网络字节序的 */
    int                 iFlg;               /* 该节点的标志，如果新的RA更新了该节点但是还没有被处理那么就是新的 */
}RA_PREFIX_ST;



/*
 * 0                   1                   2                   3
 * 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |     Type      |     Length    |           Reserved            |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |                           Lifetime                            |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |                                                               |
 * :            Addresses of IPv6 Recursive DNS Servers            :
 * |                                                               |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
struct nd_opt_dns_info     /* dns information */
{
    uint8_t   nd_opt_dns_type;
    uint8_t   nd_opt_dns_len;    
    uint16_t  nd_opt_dns_reserved;
    uint32_t  nd_opt_dns_lifetime;
};


/* DNS信息 */
typedef struct __tagDnsInfo
{
    struct  __tagDnsInfo    *pstNext;
    struct  in6_addr        stDnsAddr;
    uint32_t                uiLft;
#if 0
        struct timeval      stUpdateT;          /* 该option最后被更新时间 */
#else
        long       lUpTime;           /* 该option最后被更新时间 */
#endif
#if 0
        int                 iIfIndex;           /* 从哪个接口收到的 */
#endif
    struct sockaddr_in6     stSrc;              /* 该前缀的来源(路由器的源地址) 已经是网络字节序的 */
    int                     iFlg;               /* 该节点的标志，如果更新了该节点但是还没有被处理那么就是新的 */

}RA_DNS_INFO_ST;


/* 默认路由信息 */
typedef struct _TagDefaultRoute
{
    struct _TagDefaultRoute *pstNext;
    struct in6_addr         stDftRouteAddr;     /* 默认路由地址 */
    uint32_t                uiLft;              /* 默认路由存活时间 */
    uint8_t                 uiPrio;             /* 默认路由优先级 */
    long                    lUpTime;           /* 该option最后被更新时间 */
    int                     iFlg;               /* 该路由标志 */
}RA_DEFAULT_ROUTE_ST;

/*iAssignPrefix, iAssignDNS用来预留，后续可能需要在配置WAN的时候就指定是否该条WAN的信息下发给LAN侧  */
typedef struct __tagRAInfoList
{
    int             iPrefixNum;             /* 前缀的数目 */
    int             iDnsNum;                /* DNS的数目 */
    int             iDftRtNum;              /* 默认路由总数 */
    int             iManaged;               /* M 标志 */
    int             iOtherInfo;             /* O 标志，存放的是最近收到的最后一个RA中的对应字段 */
    int             iMOChg;                 /* M, O标记是否改变了 */
    int             iDhcpStatus;            /* DHCP状态标志 */
    int             iAssignPrefix;          /* 获取的前缀信息是否下发 */
    int             iAssignDNS;             /* 获取的DNS信息是否下发 */
    int             iIfindex;               /* 这些消息是在哪个接口上收到的 */
    int             iMtu;    /*下发的MTU值*/
    RA_PREFIX_ST    *pstPrexList;
    RA_DNS_INFO_ST  *pstDNSList;
    RA_DEFAULT_ROUTE_ST *pstDftRtList; 
}RA_INFO_LIST_ST;

int ClearRAPrefix(RA_INFO_LIST_ST *pstList);
int ClearDNSInfo(RA_INFO_LIST_ST *pstList);
int ClearDftRoute(RA_INFO_LIST_ST *pstList);
int ProcessRAOption(ND_OPT_FMT_LINK_ST *pstOptList, RA_INFO_LIST_ST *pstList);


#endif
