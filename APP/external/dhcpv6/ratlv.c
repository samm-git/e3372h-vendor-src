#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ratlv.h"
#include "ra.h"
#include "misc.h"


/* 根据收到的报文来构造整个的TLV链表 */
ND_OPT_FMT_LINK_ST *BuildTLVList(void *pvMsg, struct sockaddr_in6 stFrom, int iLen)
{
    ND_OPT_FMT_LINK_ST      *pstLink = NULL;
    ND_OPT_FMT_ST           *pstTLV = NULL;
    struct nd_router_advert *pstNDHdr = (struct nd_router_advert *)pvMsg;
    unsigned char           *ptr = (unsigned char *)(pstNDHdr + 1);
    unsigned char           *eptr = (unsigned char *)pvMsg + iLen;
    //uint8_t                  ucType;
    uint32_t                  uiLength;
    int                     err = 0;
        
    if ((!pvMsg) || (0 >= iLen))
    {
        loginfo(FUNC, "error: args NULL or length invalid\r\n");
        return (NULL);
    }

    pstLink = (ND_OPT_FMT_LINK_ST *)malloc(sizeof(ND_OPT_FMT_LINK_ST));
    if (!pstLink)
    {
        loginfo(FUNC, "error: malloc error");
        return (NULL);
    }
    memset(pstLink, 0, sizeof(ND_OPT_FMT_LINK_ST));
    memcpy(&pstLink->stFrom, &stFrom, sizeof(struct sockaddr_in6));
    memcpy(&pstLink->stNDRAHdr, pstNDHdr, sizeof(struct nd_router_advert));

    while ((ptr < eptr) && (!err))
    {
        //ucType = *(uint8_t *)ptr;
        uiLength = (*(uint8_t *)(ptr + 1)) * 8;      /* 长度以8字节为单位 */

        /* 6.1.2 All included options have a length that is greater than zero */
        if ((0 == uiLength) || (eptr < (ptr + uiLength)))
        {
            loginfo(FUNC, "length error %d", uiLength);
            err = 1;
            break;
        }
        pstTLV = (ND_OPT_FMT_ST *)malloc(sizeof(ND_OPT_FMT_ST));
        if (!pstTLV)
        {
            loginfo(FUNC, "malloc error");
            err = 1;
            break;
        }
        memset(pstTLV, 0, sizeof(ND_OPT_FMT_ST));
#if 1
        //pstTLV->stTLVData.ucType = ucType;
        //pstTLV->stTLVData.ucOptLen = uiLength;            /* 注意本变量存放的是所有的长度包括type和length本身 */
        pstTLV->stTLVData.pucData = 
            (unsigned char *)malloc((uiLength) * sizeof(unsigned char));
        if (!(pstTLV->stTLVData.pucData))
        {
            loginfo(FUNC, "malloc error malloc length: %0x", uiLength);
            err = 1;
            break;
        }
        memset(pstTLV->stTLVData.pucData, 0, (uiLength));
        memcpy(pstTLV->stTLVData.pucData, ptr, (uiLength));
#endif

        if (!pstLink->pstOpt)
        {
            pstLink->pstOpt = pstTLV;
        }
        else
        {
            pstTLV->pstNext = pstLink->pstOpt;
            pstLink->pstOpt = pstTLV;
        }

        ptr += uiLength;
    }

    if (err)
    {
        FreeTLVList(&pstLink);
        return (NULL);
    }

    return pstLink;
}

void    FreeTLVList(ND_OPT_FMT_LINK_ST **pstMsg)
{
    ND_OPT_FMT_ST           *pstTLV = NULL;
    ND_OPT_FMT_ST           *pstTemp = NULL;

    if (!(*pstMsg))
    {
        loginfo(FUNC, "args NULL\r\n");
        return ;
    }
    
    pstTLV = (*pstMsg)->pstOpt;
    for (; NULL != pstTLV; )
    {
        if (pstTLV->stTLVData.pucData)
        {
            free(pstTLV->stTLVData.pucData);
            pstTLV->stTLVData.pucData = NULL;
        }
        
        pstTemp = pstTLV;
        pstTLV = pstTLV->pstNext;
    
        free(pstTemp);
        pstTemp = NULL;
    }
    
    free(*pstMsg);
    *pstMsg = NULL;

    return ;
}


void DumpTLVOpt(ND_OPT_FMT_LINK_ST *pstMsg)
{    
    ND_OPT_FMT_ST           *pstTLV = NULL;
    uint32_t                 uiLen = 0;
    uint32_t                 uiAllLen = 0;
    
    if (!pstMsg)
    {
        loginfo(FUNC, "args NULL\r\n");
        return ;
    }
    pstTLV = pstMsg->pstOpt;
    while (pstTLV)
    {
        printf("option type:  %0x\r\n", pstTLV->ucType);
        uiAllLen = (pstTLV->ucOptLen * 8 - 2);
        printf("option length:%0x\r\n", uiAllLen);
        for (uiLen = 2; uiLen < (uiAllLen+2); uiLen++)
        {            
            if (((0 != (uiLen - 2)) && !((uiLen - 2) % 16)))
            {
                printf("\r\n");
            }
            printf("%02x ", (pstTLV->stTLVData.pucData[uiLen]));
        }

        printf("\r\n---------------------------\r\n");
        pstTLV = pstTLV->pstNext;
    }
}

void DumpWholeTLVList(ND_OPT_FMT_LINK_ST *pstMsg)
{    
    if (!pstMsg)
    {
        loginfo(FUNC, "args NULL");
        return ;
    }

    printf("\r\npacket from: "NIP6_FMT"\r\n", NIP6(pstMsg->stFrom.sin6_addr));
    printf("icmpheader\r\n");
    printf("type: %0x\r\n", pstMsg->stNDRAHdr.nd_ra_hdr.icmp6_type);
    printf("code: %0x\r\n", pstMsg->stNDRAHdr.nd_ra_hdr.icmp6_code);
    printf("checksum: %0x\r\n", pstMsg->stNDRAHdr.nd_ra_hdr.icmp6_cksum);

    printf("\r\nRA inform\r\n");
    printf("hoplimit: %0x\r\n", pstMsg->stNDRAHdr.nd_ra_curhoplimit);
    printf("M flag: %0x\r\n", 
        (pstMsg->stNDRAHdr.nd_ra_flags_reserved & ND_RA_FLAG_MANAGED ? 1:0));
    printf("O flag: %0x\r\n",
        (pstMsg->stNDRAHdr.nd_ra_flags_reserved & ND_RA_FLAG_OTHER ? 1:0));
    printf("route lifetime: %0x\r\n", ntohs(pstMsg->stNDRAHdr.nd_ra_router_lifetime));
    printf("reachable lifetime: %0x\r\n", ntohl(pstMsg->stNDRAHdr.nd_ra_reachable));
    printf("retrans lifetime: %0x\r\n", ntohl(pstMsg->stNDRAHdr.nd_ra_retransmit));

    printf("\r\noption inform\r\n");
    printf("---------------------------\r\n");
    
    DumpTLVOpt(pstMsg);

    return ;
}



