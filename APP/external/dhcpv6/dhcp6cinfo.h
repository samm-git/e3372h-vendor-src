#ifndef __DHCP6C_INFO_H__
#define __DHCP6C_INFO_H__

#include <sys/types.h>
#include <netinet/in.h>

#define MAX_DNS_NUM             (2)
#define MAX_IAPD_NUM            (4)
#define MAX_DSLITE_NUM          (2)

/* 信息类型 */
typedef enum
{
    en_type_dns  = (1),
    en_type_iapd = (2),
    en_type_iana = (3),
    en_type_dslite = (4),
    en_type_dslitename = (5),
}INFO_TYPE_EN;


typedef enum
{
    en_changed = 1,
    //en_prefixchanged = (1 << 1),
    //en_ianachanged = (1 << 2),
}INFO_CHANGE_EN;

typedef struct _TagPrefix
{
    uint32_t uiPlTime;
    uint32_t uiVlTime;
    struct in6_addr stAddr;
    int iPrefixLen;
}IA_PD_IETM_ST;


typedef struct _TagIP6Addr
{
    struct in6_addr stAddr;
    uint32_t uiPlTime;
    uint32_t uiVlTime;
}IA_NA_ITEM_ST;


typedef struct _TagDNS6Addr
{
    struct in6_addr stAddr;
}DNS_ITEM_ST;

typedef struct _TagDslite
{
    struct in6_addr stAddr;
}DSLITE_ITEM_ST;

typedef struct _TagDsliteName
{
    char acDslite[64];
}DSLITENAME_ITEM_ST;


typedef struct _TagDHCPInfoList
{
    struct _TagDHCPInfoList *pstNext;
    //int iType;
    //int iFlag;
    union
    {
        IA_PD_IETM_ST stIAPD;
        IA_NA_ITEM_ST stIANA;
        DNS_ITEM_ST   stDNS;
        DSLITE_ITEM_ST stDslite;
        DSLITENAME_ITEM_ST stDsliteName;
    }data;
}DHCP6C_INFO_LIST;

#define     iapd        data.stIAPD
#define     iana        data.stIANA
#define     dns         data.stDNS
#define     dslite      data.stDslite


typedef struct _TagDHCPInfo
{
    int iNewDnsNum;                         /* 当前节点个数 */
    int iDNSFlag;
    int iNewPrefixNum;                      /* 当前前缀个数 */
    int iPrefixFlag;
    int iNewIANANum;                        /* 当前IANA数目 */
    int iIANAFlag;
    int iNewDsliteNum;
    int iDsliteFlag;
    int iNewDsliteNameNum;
    int iDsliteNameFlag;
    DHCP6C_INFO_LIST *pstDNSList;           /* DNS 列表 */
    DHCP6C_INFO_LIST *pstIAPDList;          /* IAPD列表 */
    DHCP6C_INFO_LIST *pstIANAList;          /* IANA列表 */
    DHCP6C_INFO_LIST *pstDsliteList;        /* DSLITE列表 */
    DHCP6C_INFO_LIST *pstDsliteNameList;    /* DSLITE域名列表 */
}DHCP6C_UPDATE_INFO_ST;


#if 0
typedef struct _TagDHCPInfo
{
    int iOldDnsNum;                 /* 上一次的节点个数 */
    int iNewDnsNum;                 /* 当前节点个数 */
    int iDNSFlag;
    int iOldPrefixNum;              /* 上一次前缀个数 */
    int iNewPrefixNum;              /* 当前前缀个数 */
    int iPrefixFlag;
    int iOldIANANum;                /* 上一次IANA个数 */
    int iNewIANANum;                /* 当前IANA数目 */
    int iIANAFlag;
    DHCP6C_DNS_ST *pstDNSList;      /* DNS 列表 */
    IA_PD_ST *pstIAPDList;          /* IAPD列表 */
    IA_NA_ST *pstIANAList;          /* IANA列表 */
}DHCP6C_UPDATE_INFO_ST;
#endif

void CleanAllDHCPInfo(void);
int WriteAllDHCPInfo(int iIfindex, char *pszIfname);
int ProcessDHCPDNSInfo(struct dhcp6_optinfo *pstOption);
int ProcessDHCPDsliteInfo(struct dhcp6_optinfo *pstOption);
int ProcessDHCPIAPD(struct dhcp6_prefix  *pstPrefix, int iSet);




#endif
