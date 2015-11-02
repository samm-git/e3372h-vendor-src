#ifndef __TLV_H__
#define __TLV_H__

#include <netinet/icmp6.h>
#include <netinet/in.h>
//#include "ra.h"

#define OPT_MAX_LEN     (2048)

#if 0
typedef struct _TagTLVData
{
    uint8_t         ucType;
    uint8_t         ucOptLen;
    unsigned char   uacPtr[OPT_MAX_LEN];    
}TLV_DATA_ST;
#else
typedef struct _TagTLVData
{
    unsigned char *pucData;
}TLV_DATA_ST;
#endif


typedef struct __tagTLV
{
    TLV_DATA_ST stTLVData;
    struct __tagTLV *pstNext;
}ND_OPT_FMT_ST;

#define ucType      stTLVData.pucData[0]
#define ucOptLen    stTLVData.pucData[1] 


typedef struct __tagTLV_LINK
{    
    struct sockaddr_in6         stFrom;             /* 报文的源地址，网络字节序 */
    struct  nd_router_advert    stNDRAHdr;
    ND_OPT_FMT_ST               *pstOpt;
}ND_OPT_FMT_LINK_ST;


ND_OPT_FMT_LINK_ST *BuildTLVList(void *pvMsg, struct sockaddr_in6 stFrom, int iLen);
void    DumpTLVOpt(ND_OPT_FMT_LINK_ST *pstMsg);
void    DumpWholeTLVList(ND_OPT_FMT_LINK_ST *pstMsg);
void    FreeTLVList(ND_OPT_FMT_LINK_ST **pstMsg);

#endif
