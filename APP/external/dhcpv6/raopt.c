#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/icmp6.h>
#include <linux/if.h>
#include "ra.h"
#include "raopt.h"
#include "misc.h"
/*
  <问题单号>     <作  者>    <修改时间>   <版本> <修改描述>
    2121101411  s00201037  2012/12/25        fortify清零
    2032203980  m00182044   2012/6/10 DHCP6c不能通过RA进行无状态IPv6地址配置 
    3123107867  y00188255    2014/1/15        前缀到期后删除wan0上对应的ipv6地址
    4012701909  y00188255    2014/1/27        消除coverity报警
    3121105278  s00216562    2014/2/22        单板PPPoE IPv6拨号不支持M=0、O=0和M=0、O=1两种网络的拨号

 */

#define BUFFER_LENTH_64  64
#define BUFFER_LENTH_256 256 
#define BYTE_LENTH  8
#define WANNAME "wan0"

#define LONG_MAX 4
extern int g_iEnableRA;
static int GetPrefixNum(RA_INFO_LIST_ST *pstList);

static int GetDNSNum(RA_INFO_LIST_ST *pstList);

static int GetDftRtNum(RA_INFO_LIST_ST *pstList);


static RA_PREFIX_ST *FindPrefix(RA_INFO_LIST_ST *pstList,
    struct in6_addr stAddr, unsigned char ucPrefixLen);

static RA_DNS_INFO_ST *FindDNS(RA_INFO_LIST_ST *pstList,
    struct in6_addr stAddr);

static long TimeSub(long lCurTime, long lOldTime);


static int GetPrefixNum(RA_INFO_LIST_ST *pstList)
{
    if (!pstList)
    {
        loginfo(FUNC, "args NULL");
        return (0);
    }
    return pstList->iPrefixNum;
}

static int GetDNSNum(RA_INFO_LIST_ST *pstList)
{
    if (!pstList)
    {
        loginfo(FUNC, "args NULL");
        return (0);
    }
    return pstList->iDnsNum;
}

static int GetDftRtNum(RA_INFO_LIST_ST *pstList)
{
    if (!pstList)
    {
        loginfo(FUNC, "args NULL");
        return (0);
    }
    return pstList->iDftRtNum;
}




/* 从前缀列表中获取对应的前缀是否存在 */
static RA_PREFIX_ST *FindPrefix(RA_INFO_LIST_ST *pstList,
    struct in6_addr stAddr, uint8_t ucPrefixLen)
{
    RA_PREFIX_ST *p = NULL;
    if ((!pstList) || (0 >= GetPrefixNum(pstList)))
    {
        return (NULL);
    }

    p = pstList->pstPrexList;
    while (p)
    {
        if (0 == ComparePrefix(stAddr, ucPrefixLen, 
                            p->stPrefix, p->iPrefixLen))
        {
            return p;
        }
        p = p->pstNext;
    }

    return (NULL);
}

/* 从DNS列表中获取是否DNS存在 */
static RA_DNS_INFO_ST *FindDNS(RA_INFO_LIST_ST *pstList, struct in6_addr stAddr)
{
    RA_DNS_INFO_ST *p = NULL;
    if ((!pstList) || (0 >= GetDNSNum(pstList)))
    {
        return (NULL);
    }

    p = pstList->pstDNSList;
    while (p)
    {
        if (!memcmp(&stAddr, &p->stDnsAddr, sizeof(struct in6_addr)))
        {
            return (p);
        }
        p = p->pstNext;
    }

    return (NULL);
}

static RA_DEFAULT_ROUTE_ST *FindDftRoute(RA_INFO_LIST_ST *pstList, 
        struct in6_addr stAddr)
{
    RA_DEFAULT_ROUTE_ST *p = NULL;
    if ((!pstList) || (0 >= GetDftRtNum(pstList)))
    {
        return (NULL);
    }

    p = pstList->pstDftRtList;
    while (p)
    {
        if (!memcmp(&stAddr, &p->stDftRouteAddr, sizeof(struct in6_addr)))
        {
            return (p);
        }
        p = p->pstNext;
    }

    return (NULL);
}


/* 返回两个时间的差值，其中lCurTime为当前的sysinfo获取到的，lOldTime为之前保存的时间 */
static long TimeSub(long lCurTime, long lOldTime)
{
    long x = 0;
    if (lCurTime < lOldTime)
    {
        x = LONG_MAX - lOldTime + lCurTime;
        fprintf(stderr, "Notice: time overflow...");
    }
    else
    {
        x = lCurTime - lOldTime;
    }

    return (x);
}


/*****************************************************************************************************************/

static int WriteAllRADft(RA_INFO_LIST_ST *pstList);
static int FlushAllRADftFile(RA_INFO_LIST_ST *pstList);

// TODO: 如果默认路由的时间发生了改变，那么也需要标志出来
int ProcessRADefaultRoute(RA_INFO_LIST_ST *pstList,
    void *pvNDHdr, struct sockaddr_in6 stFrom)
{
    RA_DEFAULT_ROUTE_ST *p = NULL;
    struct nd_router_advert *pstNDHdr;
    uint32_t         uiDftLft = 0;
    int              iNew = 0;
    
    if (!pvNDHdr || !pstList)
    {
        loginfo(FUNC, "Args NULL");
        return (-1);
    }

    pstNDHdr = (struct nd_router_advert *)pvNDHdr;
    uiDftLft = ntohs(pstNDHdr->nd_ra_router_lifetime);

    p = FindDftRoute(pstList, stFrom.sin6_addr);
    /* 新节点，需要初始化 */
    if (!p && uiDftLft)
    {
        if (MAX_DFTRT_NUM <= GetDftRtNum(pstList))
        {
            loginfo(FUNC, "too many default route");
            return (-1);
        }

        loginfo(FUNC, "New default route node [" NIP6_FMT "] lifetime: %d", 
            NIP6(stFrom.sin6_addr), uiDftLft);
        
        p = malloc(sizeof(RA_DEFAULT_ROUTE_ST)); 
        if (!p)
        {
            loginfo(FUNC, "malloc error");
            return (-1);
        }
        memset(p, 0, sizeof(RA_DEFAULT_ROUTE_ST));
        iNew = 1;
    }

    if (p)
    {
        p->lUpTime = SysUpTime();
        memcpy(&p->stDftRouteAddr, &stFrom.sin6_addr, sizeof(struct in6_addr));
        p->uiLft = uiDftLft;
        // TODO: 包括linux/icmpv6.h头文件，将默认路由优先级记录下来
        // TODO: 但是目前的uclib库的linux/icmpv6.h并不支持icmpv6头文件
        //p->uiPrio = 0;
        if (iNew)
        {
            p->pstNext = pstList->pstDftRtList;
            pstList->pstDftRtList = p;
            pstList->iDftRtNum += 1;
            p->iFlg = EN_NEW;
        }
        else
        {
            p->iFlg |= EN_MODIFY;
        }
    }

    return (0);
}

int UpdateDftRt(RA_INFO_LIST_ST *pstList)
{
    RA_DEFAULT_ROUTE_ST *pstCur = NULL;
    RA_DEFAULT_ROUTE_ST *pstPrev = NULL;
    RA_DEFAULT_ROUTE_ST *pstTmpList = NULL;
    long ulCurrTime = SysUpTime();
    int iCnt = 0;

    //if (0 != GetDftRtNum(pstList))
    {
        pstCur = pstList->pstDftRtList;
        pstPrev = NULL;
        for (; pstCur; )
        {
            /* 过期的节点 */
            ulCurrTime = TimeSub(ulCurrTime, pstCur->lUpTime);
            
            if ((LIFETIME_TIMEOUT == pstCur->uiLft)
                || (ulCurrTime >= pstCur->uiLft))
            {
                if (NULL == pstPrev)
                {
                    pstList->pstDftRtList = pstCur->pstNext;
                    if (!(pstCur->iFlg & EN_SEND))
                    {
                        free(pstCur);
                    }
                    else
                    {
                        pstCur->pstNext = pstTmpList;
                        pstTmpList = pstCur;
                    }
                    pstPrev = NULL;
                    pstCur = pstList->pstDftRtList;
                }
                else
                {
                    pstPrev->pstNext = pstCur->pstNext;
                    if (!(pstCur->iFlg & EN_SEND))
                    {
                        free(pstCur);
                    }
                    else
                    {
                        pstCur->pstNext = pstTmpList;
                        pstTmpList = pstCur;
                    }
                    pstCur = pstPrev->pstNext;
                }

                pstList->iDftRtNum -= 1;
            }
            else
            {
                pstPrev = pstCur;
                pstCur = pstCur->pstNext;
            }
        }
    }

    // TODO: 通知默认路由失效
    if (pstTmpList)
    {
        iCnt = 1;
    }

    if (pstTmpList)
    {
        while (pstTmpList)
        {
            pstPrev = pstTmpList;
            pstTmpList = pstTmpList->pstNext;
            free(pstPrev);
        }
    }

    // TODO:告知所有的没有通知的默认路由
    /* 如果有些节点过期了，那么需要刷新，从而告知有效的 */
    /* 先检测有没有新节点 */
    // TODO: 如果节点标志为EN_MODIFY 那么也需要通知
    for (pstCur = pstList->pstDftRtList; pstCur; pstCur = pstCur->pstNext)
    {
        if (pstCur->iFlg & EN_NEW)
        {
            iCnt = 1;
            break;
        }
    }

    /* 走到了这里有几种可能: 1 有节点过期了，2 新增了节点，或者1，2都满足 */
    if (iCnt)
    {
        WriteAllRADft(pstList);
        return (1);
    }

    return (0);
}


/* 清除所有的默认路由信息 */
int ClearDftRoute(RA_INFO_LIST_ST *pstList)
{
    RA_DEFAULT_ROUTE_ST *p = NULL;

    if (!pstList)
    {
        loginfo(FUNC, "dft list is NULL");
        return (0);
    }

#if 0
    p = pstList->pstDftRtList;
    for (; p; p = p->pstNext)
    {
        if (p->iFlg & EN_SEND)
        {
            // TODO: 通知LAN侧，或者发消息 清掉对应文件
        }
    }
#endif

    FlushAllRADftFile(pstList);
    // TODO: 通知LAN侧，或者发消息 清掉对应文件
    
    p = pstList->pstDftRtList;
    while (pstList->pstDftRtList)
    {
        p = pstList->pstDftRtList;
        pstList->pstDftRtList = pstList->pstDftRtList->pstNext;
        free(p);
    }

    pstList->pstDftRtList = NULL;

    return 0;
}

/* 将所有默认路由写到文件中去 */
// TODO: 将时间也写入
static int WriteAllRADft(RA_INFO_LIST_ST *pstList)
{
    FILE *fp = NULL;
    char acFilePath[64] = {0};
    char acIfname[IFNAMSIZ] = {0};
    RA_DEFAULT_ROUTE_ST *pstCur = NULL;

    if (!pstList)
    {
        loginfo(FUNC, "args NULL");
        return (-1);
    }
    
    if (!if_indextoname(pstList->iIfindex, acIfname))
    {
        loginfo(FUNC, "if_indextoname error");
        return (-1);
    }
    
    snprintf(acFilePath, 63, WAN_GW6_FILE, acIfname);
    fp = fopen(acFilePath, "w");
    if (!fp)
    {
        fp = fopen(acFilePath, "w");
        if (!fp)
        {
            fprintf(stderr, "open file [%s] error", acFilePath);
            return (-1);
        }
    }

    for (pstCur = pstList->pstDftRtList; pstCur; pstCur = pstCur->pstNext)
    {
        WriteIPIntoFile(fp, pstCur->stDftRouteAddr, -1);
        
        if (pstCur->iFlg & EN_NEW)
        {
            pstCur->iFlg &= ~EN_NEW;
        }
        if (pstCur->iFlg & EN_MODIFY)
        {
            pstCur->iFlg &= ~EN_MODIFY;
        }
        pstCur->iFlg |= EN_PROCESSED;
        pstCur->iFlg |= EN_SEND;
    }

    fclose(fp);

    return (0);
}

static int FlushAllRADftFile(RA_INFO_LIST_ST *pstList)
{
    FILE *fp = NULL;
    char acFilePath[64] = {0};
    char acIfname[IFNAMSIZ] = {0};

    if (!pstList)
    {
        loginfo(FUNC, "args NULL");
        return (-1);
    }
    
    if (!if_indextoname(pstList->iIfindex, acIfname))
    {
        loginfo(FUNC, "if_indextoname error");
        return (-1);
    }
    
    snprintf(acFilePath, 63, WAN_IPADDR6_FILE, acIfname);
    fp = fopen(acFilePath, "w");
    if (!fp)
    {
        loginfo(FUNC, "open file [%s] error", acFilePath);
        return (-1);
    }

    fclose(fp);

    return (0);
}


/*****************************************************************************************************************/

static int WriteAllRAPrefix(RA_INFO_LIST_ST *pstList);
static int FlushAllRAPrefixFile(RA_INFO_LIST_ST *pstList);


int ProcessRAPrefixOpt(RA_INFO_LIST_ST *pstList,
            void *pvPrefixOpt, struct sockaddr_in6 stFrom)
{
    struct nd_opt_prefix_info *pstPrefix = NULL;
    RA_PREFIX_ST *pstPrefixNode = NULL; 
    int iNew = 0;
    uint32_t uiPerTime, uiValidTime;
    struct in6_addr stAddr;
    uint32_t uiLocalPrefix;

    if ((!pvPrefixOpt) || (!pstList))
    {
        loginfo(FUNC, "args NULL");
        return (-1);
    }

    pstPrefix = (struct nd_opt_prefix_info *)pvPrefixOpt;

    
    /* 如果该前缀不能用于自动地址配置，那么不需要保留 */
    if (!(pstPrefix->nd_opt_pi_flags_reserved & ND_OPT_PI_FLAG_AUTO))
    {
        loginfo(FUNC, "prefix can not used to autoconf [%0x]", 
            pstPrefix->nd_opt_pi_flags_reserved);
        return (-1);
    }

    uiPerTime = ntohl(pstPrefix->nd_opt_pi_preferred_time);
    uiValidTime = ntohl(pstPrefix->nd_opt_pi_valid_time);

    /* 如果perferred lifetime大于valid lifetime，那么也是非法，丢弃该前缀 */
    if (uiPerTime > uiValidTime)
    {
        loginfo(FUNC, "lifetime error");
        return (-1);
    }
    memcpy(&stAddr, &pstPrefix->nd_opt_pi_prefix, sizeof(struct in6_addr));

    

    /* 前缀必须符合要求 */
    #if 1
    if (IN6_IS_ADDR_LINKLOCAL(&stAddr)
        || IN6_IS_ADDR_MULTICAST(&stAddr))
    {
        loginfo(FUNC, "error: address type error");
        return (-1);
    }
    #else
    if (IPV6_ADDR_SCOPE_GLOBAL != IPv6AddrType(stAddr))
    {
        LOG_ERR("Error: address type error");
        return (-1);
    }
    #endif

    pstPrefixNode = FindPrefix(pstList,
        pstPrefix->nd_opt_pi_prefix, pstPrefix->nd_opt_pi_prefix_len);
    /* 新节点 */
    if (!pstPrefixNode)
    {
        if (MAX_PREFIX_NUM <= GetPrefixNum(pstList))
        {
            loginfo(FUNC, "prefix list is full");
            return (-1);
        }
        
        loginfo(FUNC, "New prefix ["NIP6_FMT"] perfer lifetime: %d  valid lifetime: %d", 
            NIP6(stAddr), uiPerTime, uiValidTime);

        /* 对于新节点来说如果时间为0，那么丢弃该前缀 */
        if ((LIFETIME_TIMEOUT == uiPerTime) || (LIFETIME_TIMEOUT == uiValidTime))
        {
            loginfo(FUNC, "New node, lifetime is 0");
            return (0);
        }
        
        pstPrefixNode = (RA_PREFIX_ST *)malloc(sizeof(RA_PREFIX_ST));
        if (!pstPrefixNode)
        {
            loginfo(FUNC, "malloc error");
            return (-1);
        }
        memset(pstPrefixNode, 0, sizeof(RA_PREFIX_ST));
        pstPrefixNode->pstNext = NULL;
        iNew = 1;
        pstPrefixNode->iFlg |= EN_NEW;
    }
    else            // TODO: 判断是否该节点的时间发生了改变，如果改变也需要通知WAN
    {
        pstPrefixNode->iFlg |= EN_MODIFY;
    }

    /* 赋值 */
    {
       pstPrefixNode->bAutoConf = (pstPrefix->nd_opt_pi_flags_reserved & ND_OPT_PI_FLAG_AUTO);
       pstPrefixNode->bOnLink = (pstPrefix->nd_opt_pi_flags_reserved & ND_OPT_PI_FLAG_ONLINK);
       pstPrefixNode->uiLft = uiPerTime;
       pstPrefixNode->uiVlft = uiValidTime;
       memcpy(&pstPrefixNode->stPrefix, &pstPrefix->nd_opt_pi_prefix, sizeof(struct in6_addr));
       pstPrefixNode->iPrefixLen = pstPrefix->nd_opt_pi_prefix_len;
       memcpy(&pstPrefixNode->stSrc, &stFrom, sizeof(struct sockaddr_in6));
       pstPrefixNode->lUpTime = SysUpTime();
    }

    /* 如果是新节点，那么需要加入链表 */
    if (iNew)
    {
        pstPrefixNode->pstNext = pstList->pstPrexList;
        pstList->pstPrexList = pstPrefixNode;
        pstList->iPrefixNum += 1;
    }

    return 0;    
}

/* 删除wan0接口上对应前缀生成的ipv6地址 */
static void delete_address_by_prefix(struct in6_addr *prefix, int prefixlen)
{
    char tmpbuf[BUFFER_LENTH_64] = {0};
    char addr6[BUFFER_LENTH_64] = {0};
    char buff[BUFFER_LENTH_256] = {0};
    char ifname[BUFFER_LENTH_64] = {0};
    struct in6_addr staddr6;
    FILE* pstfile = NULL;
    int addrlen = 0;
    int i = 0;
    int j = 0;

    if (NULL == prefix)
    {
        loginfo(FUNC, "prefix is NULL");
        return;
    }
    
    pstfile = fopen("/proc/net/if_inet6", "r");
    if (NULL != pstfile)
    {
        while (fgets(buff, BUFFER_LENTH_256, pstfile))
        {
            memset(tmpbuf, 0, BUFFER_LENTH_64);
            memset(ifname, 0, BUFFER_LENTH_64);
            // coverity[secure_coding]
            sscanf(buff, "%s %s %s %s %s %s\n", tmpbuf, ifname, ifname, ifname, ifname, ifname);
            if (0 == strcmp(ifname, WANNAME))
            {
                addrlen = strlen(tmpbuf);
                /* 为IPV6地址增加:分隔开 */
                for (i = 0, j = 0; i < addrlen; i++, j++)
                {
                    if ((i != 0) && (i % 4 == 0))
                    {
                        addr6[j] = ':';
                        j++;
                    }
                    addr6[j] = tmpbuf[i];
                }
                if (inet_pton(AF_INET6, addr6, &staddr6) > 0)
                {
                    /* 将前缀位数长度换算为字节数 */
                    addrlen = prefixlen / BYTE_LENTH;
                    if (0 == memcmp(&staddr6, prefix, addrlen))
                    {
                        /* 找到wan0上指定前缀对应的ipv6地址则删除 */
                        memset(buff, 0, BUFFER_LENTH_256);
                        memset(addr6, 0, BUFFER_LENTH_64);
                        inet_ntop(AF_INET6, &staddr6, addr6, INET6_ADDRSTRLEN);
                        snprintf(buff, BUFFER_LENTH_256, "busybox ifconfig %s del %s/%d", WANNAME, addr6, prefixlen);
                        loginfo(FUNC, " cmd is %s ", buff);
                        // coverity[tainted_string]
                        system(buff);
                        break;                       
                    }
                }
            }
        }
        fclose(pstfile);
    }
}

int UpdateRAPrefix(RA_INFO_LIST_ST *pstList)
{
    RA_PREFIX_ST *pstCurPrefixNode = NULL;
    RA_PREFIX_ST *pstPrePrefixNode = NULL;
    RA_PREFIX_ST *pstTmpPrefixList = NULL;
    long   ulCurrTime = SysUpTime();
    int iCnt = 0;
    
    if (0 != GetPrefixNum(pstList))
    {
        pstCurPrefixNode = pstList->pstPrexList;
        pstPrePrefixNode = NULL;
        while (NULL != pstCurPrefixNode)
        {
            ulCurrTime = TimeSub(ulCurrTime, pstCurPrefixNode->lUpTime);
            /* 如果生命周期为0或者已经老化掉了,那么就需要从链表中删除掉这些节点信息 */
            if ((LIFETIME_TIMEOUT == pstCurPrefixNode->uiLft)
                || ((INFINITY_LFT != pstCurPrefixNode->uiLft)
                    && (ulCurrTime >= pstCurPrefixNode->uiVlft)))
            {
                if (pstList->pstPrexList == pstCurPrefixNode)
                {
                    pstList->pstPrexList = pstCurPrefixNode->pstNext;
                    
                    if (!(pstCurPrefixNode->iFlg & EN_SEND))
                    {
                        free(pstCurPrefixNode);
                    }
                    else
                    {
                        /* 加入临时链表 */
                        pstCurPrefixNode->pstNext = pstTmpPrefixList;
                        pstTmpPrefixList = pstCurPrefixNode;
                    }
                    
                    pstCurPrefixNode = pstList->pstPrexList;
                    pstPrePrefixNode = pstCurPrefixNode;
                }
                else
                {
                   pstPrePrefixNode->pstNext = pstCurPrefixNode->pstNext;
                   if (!(pstCurPrefixNode->iFlg & EN_SEND))
                   {
                       free(pstCurPrefixNode);
                   }
                   else     /* 下发给了LAN因此需要通知到LAN告知这些已经过期 */
                   {
                        /* 加入临时链表 */
                       pstCurPrefixNode->pstNext = pstTmpPrefixList;
                       pstTmpPrefixList = pstCurPrefixNode;
                   }
                   
                   pstCurPrefixNode = pstPrePrefixNode->pstNext;
                }

                /* 节点总数减少 */
                pstList->iPrefixNum -= 1;
            }            
            else
            {
                pstPrePrefixNode = pstCurPrefixNode;
                pstCurPrefixNode = pstCurPrefixNode->pstNext;
            }
        }
    }

    /* 这个链表中都是过期的因此要告知LAN侧 */
    if (pstTmpPrefixList)
    {
        iCnt = 1;
        // TODO: 与LAN侧通讯告知这些前缀全部过期
    }

    /* 释放临时链表 */
    if (pstTmpPrefixList)
    {
        for (; NULL != pstTmpPrefixList;)
        {
            /* 释放过期节点并删除wan0上生成的对应ipv6地址 */
            delete_address_by_prefix(&pstTmpPrefixList->stPrefix, pstTmpPrefixList->iPrefixLen);
            pstPrePrefixNode = pstTmpPrefixList;
            pstTmpPrefixList = pstTmpPrefixList->pstNext;
            free(pstPrePrefixNode);
            
        }
    }
    
    /* 如果有些节点过期了，那么需要刷新，从而告知有效的 */
    /* 先检测有没有新节点 */
    for (pstCurPrefixNode = pstList->pstPrexList; 
                pstCurPrefixNode; pstCurPrefixNode = pstCurPrefixNode->pstNext)
    {
        // TODO: 如果节点不是新节点但是时间改变了也需要通知
        if (pstCurPrefixNode->iFlg & EN_NEW)
        {
            iCnt = 1;
            break;
        }
    }
                
    // TODO: 将剩余的没有下发给LAN的参数下发给LAN
    /* 走到了这里有几种可能: 1 有节点过期了，2 新增了节点，或者1，2都满足 */
    if (iCnt)
    {
        WriteAllRAPrefix(pstList);
        // TODO: 通知LAN侧
        return (1);
    }

    return (0);
}


/* 清除所有的RA信息，其中pstList给出的是链表 */
int ClearRAPrefix(RA_INFO_LIST_ST *pstList)
{
    RA_PREFIX_ST *p = NULL;

    if (!pstList)
    {
        loginfo(FUNC, "list is NULL");
        return (0);
    }

#if 0
    p = pstList->pstPrexList;
    for (; p; p = p->pstNext)
    {
        if (p->iFlg & EN_SEND)
        {
            // TODO: 通知LAN侧，或者发消息
        }
    }
#endif

    FlushAllRAPrefixFile(pstList);
    // TODO: 通知LAN侧，或者发消息


    p = pstList->pstPrexList;
    while (pstList->pstPrexList)
    {
        p = pstList->pstPrexList;
        pstList->pstPrexList = pstList->pstPrexList->pstNext;
        free(p);
    }

    pstList->pstPrexList = NULL;

    return 0;
}

static int WriteAllRAPrefix(RA_INFO_LIST_ST *pstList)
{
    FILE *fp = NULL;
    char acFilePath[64] = {0};
    char acIfname[IFNAMSIZ] = {0};
    RA_PREFIX_ST *pstCur = NULL;

    if (!pstList)
    {
        loginfo(FUNC, "args NULL");
        return (-1);
    }
    
    if (!if_indextoname(pstList->iIfindex, acIfname))
    {
        loginfo(FUNC, "if_indextoname error");
        return (-1);
    }
    
    snprintf(acFilePath, 63, WAN_PREFIX_FILE, acIfname);
    fp = fopen(acFilePath, "w");
    if (!fp)
    {
        fp = fopen(acFilePath, "w");
        if (!fp)
        {
            fprintf(stderr, "open file [%s] error", acFilePath);
            return (-1);
        }
    }

    for (pstCur = pstList->pstPrexList; 
            pstCur; pstCur = pstCur->pstNext)
    {
        /*把ra prefix和onlink 记入文件, 供cms读取, 方便wan的源IP路由里加入ra前缀的条目*/
        WriteIPLinkFlagIntoFile(fp, pstCur->stPrefix, pstCur->iPrefixLen, pstCur->bOnLink);
        
        if (pstCur->iFlg & EN_NEW)
        {
            pstCur->iFlg &= ~EN_NEW;
        }
        if (pstCur->iFlg & EN_MODIFY)
        {
            pstCur->iFlg &= ~EN_MODIFY;
        }
        pstCur->iFlg |= EN_PROCESSED;
        pstCur->iFlg |= EN_SEND;
    }

    fclose(fp);

    return (0);
}

static int FlushAllRAPrefixFile(RA_INFO_LIST_ST *pstList)
{
    FILE *fp = NULL;
    char acFilePath[64] = {0};
    char acIfname[IFNAMSIZ] = {0};

    if (!pstList)
    {
        loginfo(FUNC, "args NULL");
        return (-1);
    }
    
    if (!if_indextoname(pstList->iIfindex, acIfname))
    {
        loginfo(FUNC, "if_indextoname error");
        return (-1);
    }
    
    snprintf(acFilePath, 63, WAN_PREFIX_FILE, acIfname);
    fp = fopen(acFilePath, "w");
    if (!fp)
    {
        loginfo(FUNC, "open file [%s] error", acFilePath);
        return (-1);
    }

    fclose(fp);

    return (0);
}


/*****************************************************************************************************************/

static int WriteAllRADNS(RA_INFO_LIST_ST *pstList);
static int FlushAllRDNSFile(RA_INFO_LIST_ST *pstList);


// TODO: 注意，rfc5006有说明，因为DNS是有时间限制的，因此主机可以主动发送RS请求从而触发RA使得DNS不会过期
// TODO: 目前暂不支持
// TODO: 如果DNS的老化时间改变了也需要通知WAN
int ProcessRADNS(RA_INFO_LIST_ST *pstList,
            void *pvPrefixOpt, struct sockaddr_in6 stFrom)
{
    struct nd_opt_dns_info *pstDNSOptHdr = (struct nd_opt_dns_info *)pvPrefixOpt;
    RA_DNS_INFO_ST *pstDNSInfo = NULL; 
    RA_DNS_INFO_ST *pstPrev = NULL; 
    struct in6_addr stAddr;
    uint32_t    uiTime = 0;
    unsigned char  *p = NULL;
    int i = 0;
    int iNew = 0;

    if ((!pvPrefixOpt) || (!pstList))
    {
        loginfo(FUNC, "args NULL");
        return (-1);
    }

    /* 最小长度为3 */
    if (RA_RDDNS_LEN_MIN > pstDNSOptHdr->nd_opt_dns_len)
    {
        loginfo(FUNC, "error dns option len must longger than 3");
        return (-1);
    }

    uiTime = ntohl(pstDNSOptHdr->nd_opt_dns_lifetime);


    p = (unsigned char *)(pstDNSOptHdr + 1);
    /* 之所以需要减去1是因为需要减去option头部的大小为1 */
    for (i = 0; i < (pstDNSOptHdr->nd_opt_dns_len - 1); )
    {
        memcpy(&stAddr, p, sizeof(struct in6_addr));
        pstDNSInfo = FindDNS(pstList, stAddr);
        if (!pstDNSInfo)
        {
            /* 如果时间为0，而且是新节点那么不管该地址，可能不同的RA通报了不同的DNS */
            if (LIFETIME_TIMEOUT == uiTime)
            {
                loginfo(FUNC, "new node, lifetime is 0");
                continue;
            }

            /* 节点总数不能超过最大值 */
            if (MAX_RDDNS_NUM <= GetDNSNum(pstList))
            {
                i += 2;
                p += (2 * IPV6_ADDR_LEN);
                continue;
            }
                        
            pstDNSInfo = (RA_DNS_INFO_ST *)malloc(sizeof(RA_DNS_INFO_ST));
            if (!pstDNSInfo)
            {
                loginfo(FUNC, "malloc error");
                return (-1);
            }
            memset(pstDNSInfo, 0, sizeof(RA_DNS_INFO_ST));            
            pstDNSInfo->iFlg |= EN_NEW;            
            iNew = 1;
        }
        else
        {            
            pstDNSInfo->iFlg |= EN_MODIFY;
        }

        /* 更新内容 */
        {
            memcpy(&pstDNSInfo->stDnsAddr, &stAddr, sizeof(struct in6_addr));
            pstDNSInfo->lUpTime = SysUpTime();
            pstDNSInfo->uiLft = uiTime;
            memcpy(&pstDNSInfo->stSrc, &stFrom, sizeof(struct sockaddr_in6));
        }

        if (iNew)
        {
#if 0
            pstDNSInfo->pstNext = pstList->pstDNSList;
            pstList->pstDNSList = pstDNSInfo;
#else
            pstPrev = pstList->pstDNSList;
            if(pstPrev != NULL)
            {
                for (pstPrev = pstList->pstDNSList; NULL != pstPrev->pstNext;
                    pstPrev = pstPrev->pstNext)
                {
                    continue;
                }
            }
            if (NULL == pstPrev)
            {
                pstDNSInfo->pstNext = pstList->pstDNSList;
                pstList->pstDNSList = pstDNSInfo;
            }
            else
            {
                pstPrev->pstNext = pstDNSInfo;
                pstDNSInfo->pstNext = NULL;
            }
#endif
            pstList->iDnsNum += 1;
        }

        i += 2;
        p += IPV6_ADDR_LEN;
    }

    return (0);
}


int UpdateDNSInfo(RA_INFO_LIST_ST *pstList)
{
    RA_DNS_INFO_ST *pstCurDNSNode = NULL;
    RA_DNS_INFO_ST *pstPreDNSNode = NULL;
    RA_DNS_INFO_ST *pstTmpDNSList = NULL;
    long   ulCurrTime = SysUpTime();
    int iCnt = 0;
    
    if (0 != GetDNSNum(pstList))
    {
        pstCurDNSNode = pstList->pstDNSList;
        pstPreDNSNode = NULL;
        while (NULL != pstCurDNSNode)
        {
            ulCurrTime = TimeSub(ulCurrTime, pstCurDNSNode->lUpTime);
            /* 如果生命周期为0或者已经老化掉了,那么就需要从链表中删除掉这些节点信息 */
            if ((LIFETIME_TIMEOUT == pstCurDNSNode->uiLft)
                || ((INFINITY_LFT != pstCurDNSNode->uiLft)
                    && (ulCurrTime >= pstCurDNSNode->uiLft)))
            {
                if (pstList->pstDNSList == pstCurDNSNode)
                {
                    pstList->pstDNSList = pstCurDNSNode->pstNext;
                    
                    if (!(pstCurDNSNode->iFlg & EN_SEND))
                    {
                        free(pstCurDNSNode);
                    }
                    else            /* 这些信息被发送给了LAN侧或者给了DNS，那么这种情况下需要通知LAN侧或者DNS删除这些信息 */
                    {
                        if (NULL != pstPreDNSNode)
                        {
                        /* 加入临时链表 */
                        pstPreDNSNode->pstNext = pstTmpDNSList;
                        pstTmpDNSList = pstCurDNSNode;
                        }
                    }
                    
                    pstCurDNSNode = pstList->pstDNSList;
                    pstPreDNSNode = pstCurDNSNode;
                }
                else
                {
                    if (NULL != pstPreDNSNode)
                {
                   pstPreDNSNode->pstNext = pstCurDNSNode->pstNext;
                   if (!(pstCurDNSNode->iFlg & EN_SEND))
                   {
                       free(pstCurDNSNode);
                   }
                   else     /* 下发给了LAN,因此需要通知LAN这些已经过期 */
                   {
                       /* 加入临时链表 */
                       pstCurDNSNode->pstNext = pstTmpDNSList;
                       pstTmpDNSList = pstCurDNSNode;
                   }
                   pstCurDNSNode = pstPreDNSNode->pstNext;
                    }
                }

                /* 节点总数减少 */
                pstList->iDnsNum -= 1;
            }            
            else
            {
                pstPreDNSNode = pstCurDNSNode;
                pstCurDNSNode = pstCurDNSNode->pstNext;
            }
        }
    }

    /* 这个链表中都是过期的因此要告知LAN侧 */
    if (pstTmpDNSList)
    {
        iCnt = 1;
        // TODO: 与LAN侧通讯告知这些前缀全部过期
    }
    
    /* 释放临时链表 */
    if (pstTmpDNSList)
    {
        for (; NULL != pstTmpDNSList;)
        {
            pstPreDNSNode = pstTmpDNSList;
            pstTmpDNSList = pstTmpDNSList->pstNext;
            free(pstPreDNSNode);
        }
    }

/* 如果有些节点过期了，那么需要刷新，从而告知有效的 */
/* 先检测有没有新节点 */
    if (0 != GetDNSNum(pstList))
    {
        pstCurDNSNode = pstList->pstDNSList;
        pstPreDNSNode = NULL;
        while (NULL != pstCurDNSNode)
        {
            // TODO: 如果节点不是新节点但是时间改变了也需要通知
            if (pstCurDNSNode->iFlg & EN_NEW)
            {
                iCnt = 1;
                break;
            }
            pstPreDNSNode = pstCurDNSNode;
            pstCurDNSNode = pstCurDNSNode->pstNext;
        }
    }

    // TODO: 将剩余的没有下发给LAN的参数下发给LAN
    if (iCnt)
    {
        WriteAllRADNS(pstList);
        return (1);
    }

    return (0);
}


/* 清除所有的DNS信息 */
int ClearDNSInfo(RA_INFO_LIST_ST *pstList)
{
    RA_DNS_INFO_ST *p = NULL;

    if (!pstList)
    {
        loginfo(FUNC, "dns list is NULL");
        return (0);
    }

#if 0
    for (; p; p = p->pstNext)
    {
        if (p->iFlg & EN_SEND)
        {
            // TODO: 通知LAN侧，或者发消息
        }
    }
#endif
 
    p = pstList->pstDNSList;
    while (pstList->pstDNSList)
    {
        p = pstList->pstDNSList;
        pstList->pstDNSList = pstList->pstDNSList->pstNext;
        free(p);
    }

    pstList->pstDNSList = NULL;

    return 0;
}

static int WriteAllRADNS(RA_INFO_LIST_ST *pstList)
{
    FILE *fp = NULL;
    char acFilePath[64] = {0};
    char acIfname[IFNAMSIZ] = {0};
    RA_DNS_INFO_ST *pstCur = NULL;

    FUNCBEGIN();

    if (!pstList)
    {
        loginfo(FUNC, "args NULL");
        return (-1);
    }
    
    if (!if_indextoname(pstList->iIfindex, acIfname))
    {
        loginfo(FUNC, "if_indextoname error");
        return (-1);
    }
    
    snprintf(acFilePath, 63, WAN_DNS6_FILE, acIfname);
    fp = fopen(acFilePath, "w");
    if (!fp)
    {
        fp = fopen(acFilePath, "w");
        if (!fp)
        {
            fprintf(stderr, "open file [%s] error", acFilePath);
            return (-1);
        }
    }

    for (pstCur = pstList->pstDNSList; 
            pstCur; pstCur = pstCur->pstNext)
    {
        WriteIPIntoFile(fp, pstCur->stDnsAddr, -1);
        
        if (pstCur->iFlg & EN_NEW)
        {
            pstCur->iFlg &= ~EN_NEW;
        }
        if (pstCur->iFlg & EN_MODIFY)
        {
            pstCur->iFlg &= ~EN_MODIFY;
        }
        
        pstCur->iFlg |= EN_PROCESSED;
        pstCur->iFlg |= EN_SEND;
    }

    fclose(fp);

    FUNCEND();

    return (0);
}

static int FlushAllRDNSFile(RA_INFO_LIST_ST *pstList)
{
    FILE *fp = NULL;
    char acFilePath[64] = {0};
    char acIfname[IFNAMSIZ] = {0};

    FUNCBEGIN();

    if (!pstList)
    {
        loginfo(FUNC, "args NULL");
        return (-1);
    }
    
    if (!if_indextoname(pstList->iIfindex, acIfname))
    {
        loginfo(FUNC, "if_indextoname error");
        return (-1);
    }
    
    snprintf(acFilePath, 63, WAN_DNS6_FILE, acIfname);
    fp = fopen(acFilePath, "w");
    if (!fp)
    {
        loginfo(FUNC, "open file [%s] error", acFilePath);
        return (-1);
    }

    fclose(fp);

    FUNCEND();

    return (0);
}

static int ProcessRaMtu(RA_INFO_LIST_ST *pstList, void *pvPrefixOpt)
{
    struct nd_opt_mtu *pstRaMtu = pvPrefixOpt;
    FILE *fp = NULL;
    char acIfname[IFNAMSIZ] = {0};

    if (!pstList || !pvPrefixOpt)
    {
        loginfo(FUNC, "NULL parameter");
        return -1;
    }
    
    pstList->iMtu = ntohl(pstRaMtu->nd_opt_mtu_mtu);
    loginfo(FUNC, "RA options contains MTU=%d", pstList->iMtu);

    if (!if_indextoname(pstList->iIfindex, acIfname))
    {
        loginfo(FUNC, "if_indextoname error");
        return (-1);
    }
    
    SetFileValue(WAN_RA_MTU_FILE, acIfname, pstList->iMtu);
    return 0;
}

static int ProcessRaMOFlags(RA_INFO_LIST_ST *pstList)
{
    FILE *fp = NULL;
    char acIfname[IFNAMSIZ] = {0};

    if (!pstList)
    {
        loginfo(FUNC, "NULL parameter");
        return -1;
    }
    
    loginfo(FUNC, "RA options contains M=%d, O=%d", pstList->iManaged, pstList->iOtherInfo);

    if (!if_indextoname(pstList->iIfindex, acIfname))
    {
        loginfo(FUNC, "if_indextoname error");
        return (-1);
    }
    
    SetFileValue(WAN_RA_M_FLAG_FILE, acIfname, pstList->iManaged==ND_RA_FLAG_MANAGED);
    SetFileValue(WAN_RA_O_FLAG_FILE, acIfname, pstList->iOtherInfo==ND_RA_FLAG_OTHER);
    return 0;
}

/*****************************************************************************************************************/


int ProcessRAOption(ND_OPT_FMT_LINK_ST *pstOptList, RA_INFO_LIST_ST *pstList)
{
    static  int         iInitFlg = 0;
    ND_OPT_FMT_ST       *pstTLV = NULL;
    int                 err = 1;    
    static int iOldMflg, iOldOflg;

    FUNCBEGIN();
    
    if ((!pstOptList) || (!pstList))
    {
        loginfo(FUNC, "args NULL");
        return (-1);
    }

    if (!IN6_IS_ADDR_LINKLOCAL(&pstOptList->stFrom.sin6_addr))
    {
        loginfo(FUNC, "RA doesn't come from link-local address");
        return (-1);
    }

    pstTLV = pstOptList->pstOpt;
    if (!pstTLV)
    {
        loginfo(FUNC, "no option for this message");
        return (0);
    }

    ProcessRADefaultRoute(pstList, &pstOptList->stNDRAHdr,  pstOptList->stFrom);

    /* 处理具体的option 
       * 注意: 如果全局链表满了，那么这种情况下就不允许往里面添加了而只允许更新。
       */
    for (; NULL != pstTLV; pstTLV = pstTLV->pstNext)
    {        
        switch(pstTLV->ucType)
        {
            case ND_OPT_PREFIX_INFORMATION:
                ProcessRAPrefixOpt(pstList, (void *)(pstTLV->stTLVData.pucData), pstOptList->stFrom);
                break;
            case ND_OPT_RDNSS:
                ProcessRADNS(pstList, (void *)(pstTLV->stTLVData.pucData), pstOptList->stFrom);
                break;
            case ND_OPT_SOURCE_LINKADDR:
                break;
            case ND_OPT_MTU:
                ProcessRaMtu(pstList, (void *)(pstTLV->stTLVData.pucData));
                break;
            case ND_OPT_RIO:
            default:
                continue;
        }
    }

    /* 保存上一次的状态 */
    iOldMflg = pstList->iManaged;
    iOldOflg = pstList->iOtherInfo;
    
    /* 更新M,O的状态 */
    pstList->iManaged = (pstOptList->stNDRAHdr.nd_ra_flags_reserved 
                                & ND_RA_FLAG_MANAGED );    
    pstList->iOtherInfo = (pstOptList->stNDRAHdr.nd_ra_flags_reserved 
                                    & ND_RA_FLAG_OTHER);

    /*记录当前M, O标记到文件，供cms等读取*/
    ProcessRaMOFlags(pstList);
    
    if (iOldMflg != pstList->iManaged)
    {
        pstList->iMOChg |= EN_M_CHANGED;
    }
    if (iOldOflg != pstList->iOtherInfo)
    {
        pstList->iMOChg |= EN_O_CHANGED;
    }

    FUNCEND();

    return (0);
}




/* 注意，启动DHCP是以M还是O标志开始取决于收到的第一个RA */
int UpdateStatus(INTERFACE_RA_ST *pstList)
{
    int iCon = 0;
    
    FUNCBEGIN();
    /*BEGIN 2032203980  m00182044 2012-06-10 modify*/

    /*如果没收到ra, 不启动dhcp进程*/
    if (!(pstList->iFlags & en_ra_rcv))
    {
        return 0;
    }

    if ((!(pstList->stRAData.iDhcpStatus & enMDHCP))
        && (!(pstList->stRAData.iDhcpStatus & enODHCP)))
    {
    /*已经不看M,O标记了，都会启动;
    置一个位，让后续销毁函数能释放资源
    也让后续再进到这个函数，不会重复去调用StartDhcp6c*/
    pstList->stRAData.iDhcpStatus |= enMDHCP;
    iCon = 1;
    #if 0
        if(pstList->stRAData.iManaged)
        {
            pstList->stRAData.iDhcpStatus |= enMDHCP;
            iCon = 1;
        }
        else if (pstList->stRAData.iOtherInfo)
        {
            pstList->stRAData.iDhcpStatus |= enODHCP;
            iCon = 1;
        }
    #endif
    
        loginfo(FUNC, "notice: dhcp status:　[%0x]",
            pstList->stRAData.iDhcpStatus);
    }

    // TODO: 启动DHCP流程
        loginfo(FUNC, "now, start dhcp client iCon:%d.......",iCon);

/* start of AU4D02274 by d00107688 2009-12-02 PPPoe+SLAAC方式无法获取DNS信息 */

/*与数通ME60对接改动:
即使WAN传进来是IANA 和IAPD都申请
但还要看RA的M标记位, 若M=0则只请求IAPD, 
若M=1, 才请求IANA + IAPD
也不再看O标记，总是去请求DNS
*/
    if (iCon)
    {
	loginfo(FUNC, "pstList->stRAData.iManaged:%d.......",pstList->stRAData.iManaged);
    /*END 2032203980  m00182044 2012-06-10 modify*/
    /* BEGIN: Added by z67728, 2011/4/20 电信需求修改. */
#ifdef SUPPORT_ATP_IPV6_ALWAYS_SEND_RS
        if ( g_iEnableRA )
        {
            if (pstList->stRAData.iManaged)
            {
                StartDhcp6c(pstList->iIfindex, 
                    pstList->iIANAFlag, pstList->iIAPDFlag, pstList->iDsliteFlag);
            }
            else
            {
                StartDhcp6c(pstList->iIfindex, 0, pstList->iIAPDFlag, pstList->iDsliteFlag);
            }
        }
#else
        if (pstList->stRAData.iManaged)
        {
            StartDhcp6c(pstList->iIfindex, 
                pstList->iIANAFlag, pstList->iIAPDFlag, pstList->iDsliteFlag);
        }
        else if(pstList->stRAData.iOtherInfo)
        {
            StartDhcp6c(pstList->iIfindex, 0, 0, pstList->iDsliteFlag);
        }
        else
        {
            loginfo(FUNC, "Don't start dhcp6c!!");
        }
#endif
    /* END:   Added by z67728, 2011/4/20 */
    }
/* start of AU4D02274 by d00107688 2009-12-02 PPPoe+SLAAC方式无法获取DNS信息 */

    FUNCEND();

    return (0);
}


int UpdateByIfindex(int iIfindex)
{
    INTERFACE_RA_ST *p = NULL;

    FUNCBEGIN();
    
    p = FindInterfaceByIfindex(iIfindex);
    if (p)
    {
        UpdateDftRt(&p->stRAData);
        UpdateRAPrefix(&p->stRAData);
        UpdateDNSInfo(&p->stRAData);
        //UpdateStatus(p);
    }

    FUNCEND();
    
    return (0);
}

/* 更新消息处理 */
int UpdateRAInfo(INTERFACE_RA_ST *pstList)
{
    int iNum = 0;

    FUNCBEGIN();
    
    iNum += UpdateDftRt(&pstList->stRAData);
    
    //if (g_stRAinfoList.iAssignPrefix)
    {   
        iNum += UpdateRAPrefix(&pstList->stRAData);
    }
    
    //if (g_stRAinfoList.iAssignDNS)
    {
        iNum += UpdateDNSInfo(&pstList->stRAData);
    }
    
    UpdateStatus(pstList);

    // TODO: 如果WAN没有UP，那么不要更新
    if (iNum)
    {
        NotifyWANStatus(pstList->iIfindex);
        BSP_SYS_WakeupMonitorTask();
    }

    FUNCEND();

    return (0);
}


