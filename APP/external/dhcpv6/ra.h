#ifndef __RA_H__
#define __RA_H__

#include <pthread.h>
#include "raopt.h"

#if 0
#define     log_error(str, args ...)                                  \
{                                                                   \
    printf("\r\n[%-16s : %-5d %8x] ", __FUNCTION__, __LINE__, pthread_self());          \
    printf(str, ## args);                                           \
    printf("\r\n\r\n");                                             \
}
#endif



#define MAX_RS_SEND_NUM             (3)

#define PREFIX_UPDATE_TIME          (4)

#define RS_LOOP_SEND_TIME           (2)

#define RS_NUM_MAX_VALUE            (4)

typedef enum
{
    en_rs_send      = 1,               /* 有发送RS出去 */
    en_ra_rcv       = (1 << 1),        /* 收到了RA，此时就不需要发送RS了 */
    en_st_start     = (1 << 2),         /* 是否继续运行 */
}EN_FLAGS;


typedef struct _TagInterfaceRA
{
    struct  _TagInterfaceRA *pstNext;
    pthread_t               tidp;                /* 线程ID */
    int                     iSockfd;
    int                     iIfindex;
    int                     iIANAFlag;        /* 是否开启IANA */
    int                     iIAPDFlag;        /* 有没有开启IAPD */
    int                     iDsliteFlag;      /* 有没有开启Dslite */
    /* start of by d00107688 2010-01-26 增加支持是否允许添加默认路由 */
    int                     iDftRt;           /* 是否开启RA时候的默认路由学习 */    
    /* start of by d00107688 2010-01-26 增加支持是否允许添加默认路由 */
    int                     iState;              /* 当前RA的状态 */
    //int                     iDhcpState;          /* 当前的DHCPC状态 */
    int                     iRSNum;              /* 发送的RS数目 */
    int                     iFlags;              /* 标志 */
    RA_INFO_LIST_ST         stRAData;            /* 实际上就是RA_INFO_LIST_ST */
}INTERFACE_RA_ST;

int StartRAPthread(char *pszIfName, int iIANAFlag, int iIAPDFlag, int iDsliteFlag, int iDftRT);
int ReInitInterfacebyIfIndex(int iIfIndex);
int StopRunningRA(int iIfindex);
int StartRunningRA(int iIfindex);
int UpdateRAInfoByWanMonitor(int iIfindex);

#endif
