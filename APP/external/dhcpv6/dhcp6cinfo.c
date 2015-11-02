#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/if.h>
#include <errno.h>

#include <sys/queue.h>
#include "common.h"
#include <dhcp6.h>
#include "config.h"
#include <dhcp6c.h>
#include <dhcp6c_ia.h>
#include <prefixconf.h>
#include <errno.h>


#include "dhcp6cinfo.h"
#include "misc.h"
/*
  <问题单号>     <作  者>    <修改时间>   <版本> <修改描述>
    2121101411  s00201037  2012/12/25        fortify清零
    2032203980  m00182044   2012/6/10 DHCP6c不能通过RA进行无状态IPv6地址配置 
 */

DHCP6C_UPDATE_INFO_ST g_stDHCP6CUpdateInfo;


int IsOverByType(INFO_TYPE_EN enType)
{
    switch(enType)
    {
        case en_type_dns:
            return (MAX_DNS_NUM <= g_stDHCP6CUpdateInfo.iNewDnsNum);
        case en_type_iapd:
            return (MAX_IAPD_NUM <= g_stDHCP6CUpdateInfo.iNewPrefixNum);
        case en_type_iana:
            return (0 <= g_stDHCP6CUpdateInfo.iNewIANANum);
        case en_type_dslite:
            return (MAX_DSLITE_NUM <= g_stDHCP6CUpdateInfo.iNewDsliteNum);
        case en_type_dslitename:
            return (MAX_DSLITE_NUM <= g_stDHCP6CUpdateInfo.iNewDsliteNameNum);
        default:
            break;
    }
    return (1);
}


int GetNumByType(INFO_TYPE_EN enType)
{
    switch(enType)
    {
        case en_type_dns:
            return (g_stDHCP6CUpdateInfo.iNewDnsNum);
        case en_type_iapd:
            return (g_stDHCP6CUpdateInfo.iNewPrefixNum);
        case en_type_iana:
            return (g_stDHCP6CUpdateInfo.iNewIANANum);
        case en_type_dslite:
            return (g_stDHCP6CUpdateInfo.iNewDsliteNum);
        case en_type_dslitename:
            return (g_stDHCP6CUpdateInfo.iNewDsliteNameNum);
        default:
            break;
    }
    
    return (0xff);
}

void SetFlagByType(INFO_TYPE_EN enType, INFO_CHANGE_EN enFlag)
{
    switch(enType)
    {
        case en_type_dns:
            g_stDHCP6CUpdateInfo.iDNSFlag |= enFlag;
            break;
        case en_type_iapd:
            g_stDHCP6CUpdateInfo.iPrefixFlag |= enFlag;
            break;
        case en_type_iana:
            g_stDHCP6CUpdateInfo.iIANAFlag |= enFlag;
            break;
        case en_type_dslite:
            g_stDHCP6CUpdateInfo.iDsliteFlag |= enFlag;
            break;
        case en_type_dslitename:
            g_stDHCP6CUpdateInfo.iDsliteNameFlag |= enFlag;
            break;
        default:
            break;
    }
    return ;
}

void *FindNodeByType(INFO_TYPE_EN enType, void *pvData)
{
    DHCP6C_INFO_LIST *pstNode = NULL;
    
    switch(enType)
    {
        case en_type_dns:
        {
            pstNode = g_stDHCP6CUpdateInfo.pstDNSList;
            for (; pstNode; pstNode = pstNode->pstNext)
            {
                if (0 == memcmp(&pstNode->dns.stAddr, (struct in6_addr *)pvData,
                    sizeof(struct in6_addr)))
                {
                    return (void *)pstNode;
                }
            }
            return (NULL);
        }
        break;
        case en_type_iapd:
        {
            struct dhcp6_prefix *pst = (struct dhcp6_prefix *)pvData;
            pstNode = g_stDHCP6CUpdateInfo.pstIAPDList;
            for (; pstNode; pstNode = pstNode->pstNext)
            {
                if (0 == ComparePrefix(pstNode->iapd.stAddr, pstNode->iapd.iPrefixLen,
                    pst->addr, pst->plen))
                {
                    return (void *)pstNode;
                }
            }
            loginfo(FUNC, "can not found:"NIP6_FMT, NIP6(pst->addr));
            return (NULL);
        }
        break;
        case en_type_iana:
        {
            struct dhcp6_statefuladdr *pst = (struct dhcp6_statefuladdr *)pvData;
            pstNode = g_stDHCP6CUpdateInfo.pstIANAList;
            for (; pstNode; pstNode = pstNode->pstNext)
            {
                if (0 == memcmp(&pstNode->iana.stAddr, &pst->addr,
                    sizeof(struct in6_addr)))
                {
                    return (void *)pstNode;
                }
            }
            return (NULL);
        }
        break;
        case en_type_dslite:
        {
            pstNode = g_stDHCP6CUpdateInfo.pstDsliteList;
            for (; pstNode; pstNode = pstNode->pstNext)
            {
                if (0 == memcmp(&pstNode->dslite.stAddr, (struct in6_addr *)pvData,
                    sizeof(struct in6_addr)))
                {
                    return (void *)pstNode;
                }
            }
            return (NULL);
        }
        break;
        case en_type_dslitename:
        {
            pstNode = g_stDHCP6CUpdateInfo.pstDsliteNameList;
            for (; pstNode; pstNode = pstNode->pstNext)
            {
                if (0 == strcmp(pstNode->data.stDsliteName.acDslite, (char *)pvData))
                {
                    return (void *)pstNode;
                }
            }
            return (NULL);
        }
        break;
    }

    return (NULL);
}


int ClearDHCP6CInfoList(INFO_TYPE_EN enType)
{
    DHCP6C_INFO_LIST *pstCur = NULL;
    DHCP6C_INFO_LIST *pstPrev = NULL;
    int i = 0;

    FUNCBEGIN();

    switch(enType)
    {
        case en_type_dns:
            pstCur = g_stDHCP6CUpdateInfo.pstDNSList;
            break;
        case en_type_iapd:
            pstCur = g_stDHCP6CUpdateInfo.pstIAPDList;
            break;
        case en_type_iana:
            pstCur = g_stDHCP6CUpdateInfo.pstIANAList;
            break;
        case en_type_dslite:
            pstCur = g_stDHCP6CUpdateInfo.pstDsliteList;
            break;
        case en_type_dslitename:
            pstCur = g_stDHCP6CUpdateInfo.pstDsliteNameList;
            break;
        default:
            return (-1);
    }
    
    for (; pstCur; )
    {
        i += 1;
        pstPrev = pstCur;
        pstCur = pstCur->pstNext;
        free(pstPrev);
    }
    
    switch(enType)
    {
        case en_type_dns:
            g_stDHCP6CUpdateInfo.pstDNSList = NULL;
            g_stDHCP6CUpdateInfo.iNewDnsNum = 0;
            if (i)
            {
                g_stDHCP6CUpdateInfo.iDNSFlag |= en_changed;
            }
            break;
        case en_type_iapd:
            g_stDHCP6CUpdateInfo.pstIAPDList = NULL;
            g_stDHCP6CUpdateInfo.iNewPrefixNum = 0;
            if (i)
            {
                g_stDHCP6CUpdateInfo.iPrefixFlag |= en_changed;
            }
            break;
        case en_type_iana:
            g_stDHCP6CUpdateInfo.pstIANAList = NULL;
            g_stDHCP6CUpdateInfo.iNewIANANum = 0;
            if (i)
            {
                g_stDHCP6CUpdateInfo.iIANAFlag |= en_changed;
            }
            break;
        case en_type_dslite:
            g_stDHCP6CUpdateInfo.pstDsliteList = NULL;
            g_stDHCP6CUpdateInfo.iNewDsliteNum = 0;
            if (i)
            {
                g_stDHCP6CUpdateInfo.iDsliteFlag |= en_changed;
            }
            break;
        case en_type_dslitename:
            g_stDHCP6CUpdateInfo.pstDsliteNameList = NULL;
            g_stDHCP6CUpdateInfo.iNewDsliteNameNum = 0;
            if (i)
            {
                g_stDHCP6CUpdateInfo.iDsliteNameFlag |= en_changed;
            }
            break;
        default:
            return (-1);
    }

    FUNCEND();

    return (0);
}


int RemoveFromList(INFO_TYPE_EN enType, void *pvData)
{
    DHCP6C_INFO_LIST *pstCur = NULL;    
    DHCP6C_INFO_LIST *pstPrev = NULL;
    
    if (!pvData)
    {
        fprintf(stderr, "args NULL......");
        return (-1);
    }

    switch(enType)
    {
        case en_type_dns:       /* 待实现 */
            return (-1);
        case en_type_iapd:
        {
            struct dhcp6_prefix  *pstPrefix = (struct dhcp6_prefix *)pvData;
            pstCur = FindNodeByType(en_type_dns, pstPrefix);
            if (NULL == pstCur)
            {
                loginfo(FUNC, "can not found the iapd for"NIP6_FMT,  NIP6(pstPrefix->addr));
                return (-1);
            }

            loginfo(FUNC, "found the node... so begin to delete the iapd......\r\n\r\n\r\n");
            
            /* 头结点 */
            if (pstCur == g_stDHCP6CUpdateInfo.pstIAPDList)
            {
                loginfo(FUNC, "head node........\r\n\r\n\r\n");
                g_stDHCP6CUpdateInfo.pstIAPDList = g_stDHCP6CUpdateInfo.pstIAPDList->pstNext;
                g_stDHCP6CUpdateInfo.iNewPrefixNum -= 1;
                free(pstCur);
            }
            else
            {
                pstCur = g_stDHCP6CUpdateInfo.pstIAPDList;
                for (; pstCur; pstCur = pstCur->pstNext)
                {
                    if (0 == ComparePrefix(pstCur->iapd.stAddr, pstCur->iapd.iPrefixLen,
                    pstPrefix->addr, pstPrefix->plen))
                    {
                        if (NULL != pstPrev)
                        {
                            pstPrev->pstNext = pstCur->pstNext;                        
                            g_stDHCP6CUpdateInfo.iNewPrefixNum -= 1;
                            free(pstCur);
                            break;
                        }
                    }
                    pstPrev = pstCur;
                }
            }
            loginfo(FUNC, "Remove iapd from list over.....\r\n\r\n");
            //SetFlagByType(en_type_iapd,en_changed);
            return (0);
        }
        break;
        case en_type_iana:      /* 待实现 */
            return (-1);
        case en_type_dslite:      /* 待实现 */
            return (-1);
        case en_type_dslitename:      /* 待实现 */
            return (-1);
        default:
            return (-1);
    }
}


int AddIntoList(INFO_TYPE_EN enType, void *pvData)
{
    static int iInit = 0;
    DHCP6C_INFO_LIST *p = NULL;    
    DHCP6C_INFO_LIST *pTmp = NULL;

    if (!pvData)
    {
        fprintf(stderr, "args NULL......");
        return (-1);
    }

    if (!iInit)
    {
        memset(&g_stDHCP6CUpdateInfo, 0, sizeof(DHCP6C_UPDATE_INFO_ST));
        iInit = 1;
    }

    if (IsOverByType(enType))
    {
        loginfo(FUNC, "already reached the max limit...");
        return (-1);
    }
        
    p = (DHCP6C_INFO_LIST *)malloc(sizeof(DHCP6C_INFO_LIST));
    if (!p)
    {
        fprintf(stderr, "notice: malloc error at [%s %d]", 
            __FUNCTION__, __LINE__);
        return (-1);
    }
    
    memset(p, 0, sizeof(DHCP6C_INFO_LIST));
    
    switch(enType)
    {
        case en_type_dns:
        {
            struct in6_addr stAddr;
            memcpy(&stAddr, pvData, sizeof(struct in6_addr));
            memcpy(&p->dns.stAddr, pvData, sizeof(struct in6_addr));
/* start of AU4D02265 by d00107688 2009-11-30 网关获取的IPv6的主备DNS地址显示是反的 */
#if 0
            p->pstNext = g_stDHCP6CUpdateInfo.pstDNSList;
            g_stDHCP6CUpdateInfo.pstDNSList = p;            
#else
            if (NULL == g_stDHCP6CUpdateInfo.pstDNSList)
            {
                g_stDHCP6CUpdateInfo.pstDNSList = p;
                p->pstNext = NULL;
            }
            else
            {
                for (pTmp = g_stDHCP6CUpdateInfo.pstDNSList;
                        NULL != pTmp->pstNext; pTmp = pTmp->pstNext)
                ;
                pTmp->pstNext = p;
                p->pstNext = NULL;
            }
#endif
/* end of AU4D02265 by d00107688 2009-11-30 网关获取的IPv6的主备DNS地址显示是反的 */
            g_stDHCP6CUpdateInfo.iNewDnsNum += 1;
        }
        break;
        case en_type_iapd:
        {
            struct dhcp6_prefix *pst = (struct dhcp6_prefix *)pvData;
            memcpy(&p->iapd.stAddr, &pst->addr, sizeof(struct in6_addr));
            p->iapd.uiPlTime = pst->pltime;
            p->iapd.uiVlTime = pst->vltime;
            p->iapd.iPrefixLen = pst->plen;
            p->pstNext = g_stDHCP6CUpdateInfo.pstIAPDList;
            g_stDHCP6CUpdateInfo.pstIAPDList = p;
            g_stDHCP6CUpdateInfo.iNewPrefixNum += 1;
            break;
        }
        case en_type_iana:
        {
            struct dhcp6_statefuladdr *pst = (struct dhcp6_statefuladdr *)pvData;
            memcpy(&p->iana.stAddr, &pst->addr, sizeof(struct in6_addr));
            p->iana.uiPlTime = pst->pltime;
            p->iana.uiVlTime = pst->vltime;
            p->pstNext = g_stDHCP6CUpdateInfo.pstIANAList;
            g_stDHCP6CUpdateInfo.pstIANAList = p;
            g_stDHCP6CUpdateInfo.iNewIANANum += 1;
            break;
        }        
        case en_type_dslite:
        {
            memcpy(&p->dslite.stAddr, pvData, sizeof(struct in6_addr));

            if (NULL == g_stDHCP6CUpdateInfo.pstDsliteList)
            {
                g_stDHCP6CUpdateInfo.pstDsliteList = p;
                p->pstNext = NULL;
            }
            else
            {
                for (pTmp = g_stDHCP6CUpdateInfo.pstDsliteList;
                        NULL != pTmp->pstNext; pTmp = pTmp->pstNext)
                ;
                pTmp->pstNext = p;
                p->pstNext = NULL;
            }

            g_stDHCP6CUpdateInfo.iNewDsliteNum += 1;
            break;
        }
        case en_type_dslitename:
        {
            strcpy(p->data.stDsliteName.acDslite, pvData);

            if (NULL == g_stDHCP6CUpdateInfo.pstDsliteNameList)
            {
                g_stDHCP6CUpdateInfo.pstDsliteNameList = p;
                p->pstNext = NULL;
            }
            else
            {
                for (pTmp = g_stDHCP6CUpdateInfo.pstDsliteNameList;
                        NULL != pTmp->pstNext; pTmp = pTmp->pstNext)
                ;
                pTmp->pstNext = p;
                p->pstNext = NULL;
            }

            g_stDHCP6CUpdateInfo.iNewDsliteNameNum += 1;
            break;
        }   
        default:
            loginfo(FUNC, "unknown type");
            return (-1);
    }

    return (0);
}


int ProcessDHCPIAPD(struct dhcp6_prefix  *pstPrefix, int iSet)
{    
    DHCP6C_INFO_LIST stPrefix;

    if (!pstPrefix)
    {
        loginfo(FUNC, "args NULL");
        return (-1);
    }

    loginfo(FUNC, "Now, begin to process dhcp iapd ......\r\n\r\n");

    /* 增加节点 */
    if (iSet)
    {
        // TODO: 目前来说，如果一个IA带有多个前缀，那么WAN侧也只要一个前缀
        // TODO: 后续如果要支持多个前缀的话，那么需要将本函数提到update_prefix函数的上一层也就是update_ia
        if (NULL == FindNodeByType(en_type_iapd, (void *)pstPrefix))
        {
            loginfo(FUNC, "add prefix:["NIP6_FMT"]", NIP6(pstPrefix->addr));
            ClearDHCP6CInfoList(en_type_iapd);
            AddIntoList(en_type_iapd, (void *)pstPrefix);        
            SetFlagByType(en_type_iapd, en_changed);
        }
    }
    else
    {
        loginfo(FUNC, "Remove iapd" NIP6_FMT " from iapd list...\r\n\r\n", NIP6(pstPrefix->addr));
        if (0 == RemoveFromList(en_type_iapd, (void *)pstPrefix))
        {
            SetFlagByType(en_type_iapd, en_changed);
        }
    }

    loginfo(FUNC, "process dhcp iapd over........\r\n\r\n\r\n");

    return (0);
}

int ProcessDHCPDNSInfo(struct dhcp6_optinfo *pstOption)
{
    struct dhcp6_listval *d;
    int j;

    FUNCBEGIN();
    
    if (!pstOption)
    {
        fprintf(stderr, "args NULL");
        return (-1);
    }

    loginfo(FUNC, "now, begin to process dns info......");
    
    if (!TAILQ_EMPTY(&pstOption->dns_list))
    {
        j = 0;
        /*BEGIN 2032203980  m00182044 2012-06-10 modify*/
        /*找到了一个有改变，则把老的dns列表清空，因为下面要重加一遍*/
        for (d = TAILQ_FIRST(&pstOption->dns_list); d;
            d = TAILQ_NEXT(d, link)) 
        {
            if (NULL == FindNodeByType(en_type_dns, &d->val_addr6))
            {
                j++;
                loginfo(FUNC, "dns info changed....");
                break;
            }
        }
        /*先清空旧的dns列表信息*/
        if (j)
        {
            if (0 != GetNumByType(en_type_dns))
            {
                loginfo(FUNC, "dns info clear old first......");
                ClearDHCP6CInfoList(en_type_dns);
            }
        }
        /*加新的dns信息到链表*/
        j = 0;
        for (d = TAILQ_FIRST(&pstOption->dns_list); d;
            d = TAILQ_NEXT(d, link)) 
        {
            if (NULL == FindNodeByType(en_type_dns, &d->val_addr6))
            {
                AddIntoList(en_type_dns, (void *)&d->val_addr6);
                j += 1;
            }
        }
        /*END 2032203980  m00182044 2012-06-10 modify*/
             
        if (j)
        {
            loginfo(FUNC, "dns info have changed......");
            SetFlagByType(en_type_dns, en_changed);
        }
    }
    else            /* 如果服务器下发的为0，那么这种情况下就需要清掉DNS列表，然后报告给WAN */
    {
        loginfo(FUNC, "empty the list......");
        
        if (0 != GetNumByType(en_type_dns))
        {
            ClearDHCP6CInfoList(en_type_dns);
            SetFlagByType(en_type_dns, en_changed);
        }
    }

    FUNCEND();

    return (0);
}

int ProcessDHCPDsliteInfo(struct dhcp6_optinfo *pstOption)
{
    struct dhcp6_listval *d;
    int j;

    FUNCBEGIN();
    
    if (!pstOption)
    {
        fprintf(stderr, "args NULL");
        return (-1);
    }

    loginfo(FUNC, "now, begin to process dslite info......");
    
    if (!TAILQ_EMPTY(&pstOption->dslite_list))
    {
        j = 0;
		for (d = TAILQ_FIRST(&pstOption->dslite_list); d;
		     d = TAILQ_NEXT(d, link)) 
        {
            if (NULL == FindNodeByType(en_type_dslite, &d->val_addr6))
            {
                AddIntoList(en_type_dslite, (void *)&d->val_addr6);
                j += 1;
            }
        }
             
        if (j)
        {
            loginfo(FUNC, "dslite info have changed......");
            SetFlagByType(en_type_dslite, en_changed);
        }
    }
    else            /* 如果服务器下发的为0，那么这种情况下就需要清掉Dslite列表，然后报告给WAN */
    {
        loginfo(FUNC, "empty the list......");
        
        if (0 != GetNumByType(en_type_dslite))
        {
            ClearDHCP6CInfoList(en_type_dslite);
            SetFlagByType(en_type_dslite, en_changed);
        }
    }

    if (!TAILQ_EMPTY(&pstOption->dslitename_list))
    {
        j = 0;
        for (d = TAILQ_FIRST(&pstOption->dslitename_list); d;
             d = TAILQ_NEXT(d, link)) 
        {
            if ((strlen(d->val_vbuf.dv_buf) < 64)
                && (NULL == FindNodeByType(en_type_dslitename, d->val_vbuf.dv_buf)))
            {
                AddIntoList(en_type_dslitename, (void *)d->val_vbuf.dv_buf);
                j += 1;
            }
        }
             
        if (j)
        {
            loginfo(FUNC, "dslitename info have changed......");
            SetFlagByType(en_type_dslitename, en_changed);
        }
    }
    else            /* 如果服务器下发的为0，那么这种情况下就需要清掉Dslite列表，然后报告给WAN */
    {
        loginfo(FUNC, "empty the list......");
        
        if (0 != GetNumByType(en_type_dslitename))
        {
            ClearDHCP6CInfoList(en_type_dslitename);
            SetFlagByType(en_type_dslitename, en_changed);
        }
    }

    FUNCEND();

    return (0);
}


void CleanAllDHCPInfo(void)
{
    FUNCBEGIN();

    loginfo(FUNC, "Clear all dhcp info......\r\n\r\n");
    
    ClearDHCP6CInfoList(en_type_dns);
    ClearDHCP6CInfoList(en_type_iapd);
    ClearDHCP6CInfoList(en_type_iana);
    ClearDHCP6CInfoList(en_type_dslite);
    ClearDHCP6CInfoList(en_type_dslitename);

    FUNCEND();
}


int WriteAllDHCPInfo(int iIfindex, char *pszIfname)
{
    DHCP6C_INFO_LIST *pstNode = NULL;
    int iCnt = 0;
    char acIfname[IFNAMSIZ] = {0};
    char acFilePath[64] = {0};
    FILE *fp = NULL;
    int lLen = 0;
    char acName[64] = {0};

    FUNCBEGIN();

    if (!if_indextoname(iIfindex, acIfname))
    {
        if ((NULL == pszIfname) || ('\0' == pszIfname[0]))
        {
            fprintf(stderr, "if_indextoname error");
            return (0);
        }
        
        strncpy(acIfname, pszIfname, (IFNAMSIZ - 1));

        iIfindex = if_nametoindex(acIfname);
        if (0 >= iIfindex)
        {
            fprintf(stderr, "if_nametoindex error");
            return (0);
        }
    }
    
    if (g_stDHCP6CUpdateInfo.iDNSFlag & en_changed)
    {
        snprintf(acFilePath, 63, WAN_DHCPC6_DNS, acIfname);
        fp = fopen(acFilePath, "w");
        {
            if (NULL == fp)
            {
                fp = fopen(acFilePath, "w");
            }
            if (NULL == fp)
            {
                fprintf(stderr, "open file: [%s] error", acFilePath);
            }
        }
        if (fp)
        {
            pstNode = g_stDHCP6CUpdateInfo.pstDNSList;
            if (!pstNode)
            {
            }
            else
            {
                for (; pstNode; pstNode = pstNode->pstNext)
                {
                    WriteIPIntoFile(fp, pstNode->dns.stAddr, -1);
                }
            }
            fclose(fp);            
            iCnt += 1;
        }
        g_stDHCP6CUpdateInfo.iDNSFlag &= ~en_changed;
    }

#if 0
    if (g_stDHCP6CUpdateInfo.iIANAFlag & en_changed)
    {
        snprintf(acFilePath, 63, WAN_DHCPC6_DNS, acIfname);
        fp = fopen(acFilePath, "w");
        {
            if (NULL == fp)
            {
                fp = fopen(acFilePath, "w");
            }
            if (NULL == fp)
            {
                fprintf(stderr, "open file: [%s] error", acFilePath);
            }
        }
        if (fp)
        {
            pstNode = g_stDHCP6CUpdateInfo.pstDNSList;
            if (!pstNode)
            {
            }
            else
            {
                for (; pstNode; pstNode = pstNode->pstNext)
                {
                    WriteIPIntoFile(fp, pstNode->dns.stAddr, -1);
                }
            }
            fclose(fp);            
            iCnt += 1;
        }
        g_stDHCP6CUpdateInfo.iIANAFlag &= ~en_changed;
    }
#endif
    if (g_stDHCP6CUpdateInfo.iPrefixFlag & en_changed)
    {
        snprintf(acFilePath, 63, WAN_DHCPC6_PREFIX, acIfname);
        fp = fopen(acFilePath, "w");
        {
            if (NULL == fp)
            {
                fp = fopen(acFilePath, "w");
            }
            if (NULL == fp)
            {
                fprintf(stderr, "open file: [%s] error", acFilePath);
            }
        }
        if (fp)
        {
            pstNode = g_stDHCP6CUpdateInfo.pstIAPDList;
            if (!pstNode)
            {
            }
            else
            {
                for (; pstNode; pstNode = pstNode->pstNext)
                {
                    WriteIPIntoFile(fp, pstNode->iapd.stAddr, 
                            pstNode->iapd.iPrefixLen);
                }
            }
            fclose(fp);            
            iCnt += 1;
        }
        
        g_stDHCP6CUpdateInfo.iPrefixFlag &= ~en_changed;
    }

    if (g_stDHCP6CUpdateInfo.iDsliteFlag & en_changed)
    {
        snprintf(acFilePath, 63, WAN_DHCPC6_DSLITE, acIfname);
        fp = fopen(acFilePath, "w");
        {
            if (NULL == fp)
            {
                usleep(100000);
                fp = fopen(acFilePath, "w");
            }
            if (NULL == fp)
            {
                fprintf(stderr, "open file: [%s] error", acFilePath);
            }
        }
        if (fp)
        {
            pstNode = g_stDHCP6CUpdateInfo.pstDsliteList;
            if (!pstNode)
            {
            }
            else
            {
                for (; pstNode; pstNode = pstNode->pstNext)
                {
                    WriteIPIntoFile(fp, pstNode->dslite.stAddr, -1);
                }
            }
            fclose(fp);            
            iCnt += 1;
        }
        g_stDHCP6CUpdateInfo.iDsliteFlag &= ~en_changed;
    }

    if (g_stDHCP6CUpdateInfo.iDsliteNameFlag & en_changed)
        {
            snprintf(acFilePath, 63, WAN_DHCPC6_DSLITENAME, acIfname);
            fp = fopen(acFilePath, "w");
            {
                if (NULL == fp)
                {
                    usleep(100000);
                    fp = fopen(acFilePath, "w");
                }
                if (NULL == fp)
                {
                    fprintf(stderr, "open file: [%s] error", acFilePath);
                }
            }
            if (fp)
            {
                pstNode = g_stDHCP6CUpdateInfo.pstDsliteNameList;
                if (!pstNode)
                {
                }
                else
                {
                    for (; pstNode; pstNode = pstNode->pstNext)
                    {
                        strcpy(acName, pstNode->data.stDsliteName.acDslite);
                        lLen = strlen(acName);
                        if ((lLen > 0) && (acName[lLen - 1] == '.'))
                        {
                            acName[lLen - 1] = 0;
                        }
                        fprintf(fp, "%s\n", acName);
                    }
                }
                fclose(fp);            
                iCnt += 1;
            }
            g_stDHCP6CUpdateInfo.iDsliteNameFlag &= ~en_changed;
        }


    if (iCnt)
    {
        NotifyWANStatus(iIfindex);
    }

    FUNCEND();

    return iCnt;
}

