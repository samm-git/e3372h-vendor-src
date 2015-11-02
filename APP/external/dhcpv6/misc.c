/**********************************************************************************************
  <问题单号>         <作  者>    <修改时间>   <版本> <修改描述>
  2121101411  s00201037   2012/12/25           fortify清零
  2032203980  m00182044   2012/6/10            DHCP6c不能通过RA进行无状态IPv6地址配置 
  N/A	              y00186923   2012.11.16           9615平台代码差异合入
  3050907167  z00186446   2013/15/2            BCM43241在9625上面的接口适配
  3082010042  y00188255  2013/08/23            消除fortify报警
  3112001144  s00216562   2013/11/21           pppoe支持ipv6
 **********************************************************************************************/
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/sysinfo.h>
#include <linux/if.h>
#include <syslog.h>
#include <errno.h>
#ifdef MBB_ROUTER_ANDROID
#include <sys/ipc.h>
#endif
#include <linux/msg.h>

#include "misc.h"
#include "dhcp6c.h"

#define LINE_MAX 4096

/*BEGIN 2032203980  m00182044 2012-06-10 modify*/
#include "wanapi.h"
/*END 2032203980  m00182044 2012-06-10 modify*/

int exit_addrMonitor = 0;
int exit_ok = 0;
int logged = 0;

static unsigned int g_uiWanChangeCnt = 1;


static int SetProcValue(char *pszFilePath, char *pszIfName, int iValue);

//#define SEND_NA_PD

/********************************************************************************************************
注释开始，该引用方式不再用，而是在writeDHCP6CCfg中动态写，
此处仅保留格式作参考

适合于纯粹的DHCP+IANA，或者 SLAAC的单纯M 
const char *pszIANACfg = "interface %s\n"
                         "{\n"                         
                         "    send ia-na %d, domain-name;\n"
                         "    request domain-name-servers, domain-name;\n"
                         "};\n"
                         "id-assoc na %d \n"
                         "{\n"
                         "};\n";


适合DHCP+IAPD 或者 SLAAC+IAPD
const char *pszIAPDCfg =    "interface %s\n"
                            "{\n"
                            "    send ia-pd %d, domain-name;\n"
                            "    request domain-name-servers, domain-name;\n"
                            "};\n"
                            "id-assoc pd %d \n"
                            "{\n"
                            "};\n";


适合于 SLAAC+IANA+IAPD 或者 DHCP+IANA+IAPD
const char *pszIANAPDCfg =  "interface %s\n"
                            "{\n"
                            "    send ia-na %d, ia-pd %d, domain-name;\n"
                            "    request domain-name-servers, domain-name;\n"
                            "};\n"
                            "id-assoc na %d \n"
                            "{\n"
                            "};\n"
                            "id-assoc pd %d \n"
                            "{\n"
                            "};\n";

适合于单纯的SLAAC Other
const char *pszNoIAPDCfg = "interface %s\n"
                           "{\n"
                           "    information-only;\n"
                           "    request domain-name-servers, domain-name;\n"
                           "};\n";

注释结束
********************************************************************************************************/

/* 分为如下几种:
 * 1 纯粹的SLAACM
 * 2 SLAAC+IAPD(只有在M置位的情况下)
 * 3 纯粹的SLAAC Other
 * 4 DHCP+IANA
 * 5 DHCP+IAPD
 */


/* 返回0表示两个前缀相等 */
#if 0
int ComparePrefix(struct in6_addr stAddr1, uint8_t ucPrefixLen1,
        struct in6_addr stAddr2, uint8_t ucPrefixLen2)
#endif

int ComparePrefix(struct in6_addr stAddr1, int iPrefixLen1,
            struct in6_addr stAddr2, int iPrefixLen2)
{
    int i = (iPrefixLen1 % 8);
    unsigned char x = (iPrefixLen1 / 8);
    unsigned char c1, c2;
    
    if (iPrefixLen1 != iPrefixLen2)
    {
        return (-1);
    }

    if ( 0 != memcmp(&stAddr1, &stAddr2, x))
    {
        return (-1);
    }
    if (i)
    {
        c1 = *((unsigned char *)&stAddr1 + x);
        c2 = *((unsigned char *)&stAddr2 + x);

        if ((c1 ^ c2) >= (1 << (8 - i)))
        {
            return (-1);
        }
    }

    return (0);
}


long SysUpTime(void)
{
/*	Android不支持sysinfo
    struct sysinfo stSysInfo;

    if (0 != sysinfo(&stSysInfo))
    {
        return (0);
    }

    return stSysInfo.uptime;
*/
	FILE *tfp;
	long uptime = 0;
	if ((tfp = fopen("/proc/uptime", "r")) != NULL) {
		if (fscanf(tfp, "%ld", &uptime) != 1)
		{
			uptime = 0;
		}
		fclose(tfp);
	}
	return uptime;
}


struct 
{
    uint32_t    uiPrefix;
    int         iAddrType;
}AddrType[] =
{
    {0xff000000, IPV6_ADDR_MULTICAST},
    {0xfe800000, IPV6_ADDR_SCOPE_LINKLOCAL},
    {0xfc000000, IPV6_ADDR_SCOPE_LINKLOCAL},
};

int  IPv6AddrType(const struct in6_addr stAddr)
{
    uint32_t uiPrefix;
    uint32_t uiAddrPrefix;
    int i = 0;

    uiPrefix = stAddr.s6_addr32[0];

    for (; i < ARRAY_SIZE(AddrType); i++)
    {
        uiAddrPrefix = htonl(AddrType[i].uiPrefix);
        if (uiAddrPrefix == (uiPrefix & uiAddrPrefix))
        {
            return AddrType[i].uiPrefix;
        }
    }

    return IPV6_ADDR_SCOPE_GLOBAL;
}


void loginfo(const char *fname, const char *fmt, ...)
{
	va_list ap;
	char logbuf[LINE_MAX];
	int printfname = 1;

	va_start(ap, fmt);
	vsnprintf(logbuf, sizeof(logbuf), fmt, ap);
    va_end(ap);

	if (*fname == '\0')
		printfname = 0;

   // if (logged)
    {
/*BEGIN 2032203980  m00182044 2012-06-10 modify*/
#if 0
        fprintf(stderr, "[pid: %d  %s]    %s", getpid(), fname, logbuf);
        fprintf(stderr, "\r\n");
#endif
        NP_DHCP_SERVER_DEBUG("dhcp6 [pid: %d  %s]    %s", getpid(), fname, logbuf);
/*END 2032203980  m00182044 2012-06-10 modify*/
    }

#if 0
	if (logged) {
		time_t now;
		struct tm *tm_now;
		const char *month[] = {
			"Jan", "Feb", "Mar", "Apr", "May", "Jun",
			"Jul", "Aug", "Sep", "Oct", "Nov", "Dec",
		};

		if ((now = time(NULL)) < 0)
		{
            return ;
		}
        
		tm_now = localtime(&now);
		fprintf(stderr, "%3s/%02d/%04d %02d:%02d:%02d: %s%s%s\n",
		    month[tm_now->tm_mon], tm_now->tm_mday,
		    tm_now->tm_year + 1900,
		    tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec,
		    fname, printfname ? ": " : "",
		    logbuf);
	}
    else
		syslog(level, "%s%s%s", fname, printfname ? ": " : "", logbuf);
#endif
}


int SetFileValue(char *pszFilePath, char *pszIfName, int iValue)
{
    char acPath[64] = {0};
    FILE *fp = NULL;

    FUNCBEGIN();
    
    if (!pszIfName || !pszFilePath)
    {
        return (-1);
    }

    snprintf(acPath, 64, pszFilePath, pszIfName);

    fp = fopen(acPath, "w");
    if (NULL == fp)
    {
        fp = fopen(acPath, "w");
        if (NULL == fp)
        {
            perror("open file error");
            return (-1);
        }
    }

    fprintf(fp, "%d", iValue);

    fclose(fp);

    FUNCEND();

    return 0;
}

/*BEGIN 2032203980  m00182044 2012-06-10 modify*/
int GetFileValue(char *pszFilePath, char *pszIfName, int *iValue)
{
    char acPath[64] = {0};
    char acBuff[64] = {0};
    FILE *fp = NULL;

    FUNCBEGIN();
    
    if (!pszIfName || !pszFilePath)
    {
        return (-1);
    }

    snprintf(acPath, 64, pszFilePath, pszIfName);

    fp = fopen(acPath, "r");
    if (NULL == fp)
    {
        fp = fopen(acPath, "r");
        if (NULL == fp)
        {
            perror("open file error");
            return (-1);
        }
    }

    fread(acBuff, sizeof(acBuff), 1, fp);

    *iValue= atoi(acBuff);

    if (0 > *iValue)
    {
        fclose(fp);
        return -1;
    }

    fclose(fp);

    FUNCEND();

    return 0;
}
/*END 2032203980  m00182044 2012-06-10 modify*/

/* start of by d00107688 2010-01-26 增加支持是否允许添加默认路由 */
int SetAcceptDftRoute(char *pszIfName, int iValue)
{
    loginfo(FUNC, "set accept_ra with %d\r\n\r\n\r\n\r\n", iValue);
    return SetFileValue(ACCEPT_RA_DLF_ROUTE_FILE_PATH, pszIfName, iValue);
}
/* end of by d00107688 2010-01-26 增加支持是否允许添加默认路由 */


int SetAcceptRA(char *pszIfName, int iValue)
{
    loginfo(FUNC, "set accept_ra with %d\r\n\r\n\r\n\r\n", iValue);
    return SetFileValue(ACCEPT_RA_FILE_PATH, pszIfName, iValue);
}

/*BEGIN 2032203980  m00182044 2012-06-10 modify*/
int SetAcceptForwarding(char *pszIfName, int iValue)
{
    loginfo(FUNC, "set forwarding with %d\r\n\r\n\r\n\r\n", iValue);
    return SetFileValue(ACCEPT_FORWARDING_FILE_PATH, pszIfName, iValue);
}
/*END 2032203980  m00182044 2012-06-10 modify*/

int SetAutoConf(char *pszIfName, int iValue)
{
    loginfo(FUNC, "set autoconf with %d\r\n\r\n\r\n\r\n", iValue);
    return SetFileValue(ACCEPT_RA_AUTOCONF_FILE_PATH, pszIfName, iValue);
}


int SetWanStatus(int iIfindex, int iValue)
{
    char acIfName[IFNAMSIZ] = {0};
    if (!if_indextoname(iIfindex, acIfName))
    {
        fprintf(stderr, "get wan name by index[%d] error", iIfindex);
        return (-1);
    }
    
    return SetFileValue(WAN_STATUS6_FILE, acIfName, iValue);
}


/*BEGIN 2032203980  m00182044 2012-06-10 modify*/
int GetWanStatus(int iIfindex)
{
    char acIfName[IFNAMSIZ] = {0};
    int iValue = 0;
    if (!if_indextoname(iIfindex, acIfName))
    {
        fprintf(stderr, "get wan name by index[%d] error", iIfindex);
        return (-1);
    }

    GetFileValue(WAN_STATUS6_FILE, acIfName,&iValue);
    
    return iValue;
}
/*END 2032203980  m00182044 2012-06-10 modify*/

void NotifyWANStatus(int iIfindex)
{
    char acIfName[IFNAMSIZ] = {0};
    if (!if_indextoname(iIfindex, acIfName))
    {
        fprintf(stderr, "get wan name by index[%d] error", iIfindex);
        return ;
    }

    SetFileValue(WAN_NOTIFY_FILE, acIfName, g_uiWanChangeCnt);
    
    g_uiWanChangeCnt++;

    return ;
}


void WriteIPIntoFile(FILE *fp, struct in6_addr stAddr, int iPrefix)
{
    char acLine[256] = {0};
    char acAddr[64] = {0};

    FUNCBEGIN();
    
    if (!fp)
    {
        return;
    }

    if (NULL != inet_ntop(AF_INET6, (void *)&stAddr, acAddr, 63))
    {
        if (-1 == iPrefix)
        {
            snprintf(acLine, 255, "%s",  acAddr);
        }
        else
        {
            snprintf(acLine, 255, "%s/%d",  acAddr, iPrefix);
        }

        fprintf(fp, "%s\n", acLine);
    }

    FUNCEND();

    return ;
}

void WriteIPLinkFlagIntoFile(FILE *fp, struct in6_addr stAddr, int iPrefixLen, int OnLinkFlag)
{
    char acLine[256] = {0};
    char acAddr[64] = {0};
    /*BEGIN 2032203980  m00182044 2012-06-10 modify*/
    FILE *pstFile = NULL;
    /*END 2032203980  m00182044 2012-06-10 modify*/
    
    if (!fp)
    {
        return;
    }

    if (NULL != inet_ntop(AF_INET6, (void *)&stAddr, acAddr, 63))
    {
        if (-1 == iPrefixLen)
        {
            snprintf(acLine, 255, "%s",  acAddr);
        }
        else
        {
            snprintf(acLine, 255, "%s/%d %d",  acAddr, iPrefixLen, OnLinkFlag);
        }

        fprintf(fp, "%s\n", acLine);
        /*BEGIN 2032203980  m00182044 2012-06-10 modify*/
        /*清空全部wan路由防止路由冲突*/
        snprintf(acLine, sizeof(acLine), "ip -6 route flush table main");
        system(acLine);
        snprintf(acLine, sizeof(acLine), "ip -6 route del %s/%d dev br0",  acAddr, iPrefixLen);
        system(acLine);
        snprintf(acLine, sizeof(acLine), "ip -6 route add %s/%d dev br0",  acAddr, iPrefixLen);
        system(acLine);
        snprintf(acLine, sizeof(acLine), WAN_GW6_FILE, "wan0");
        pstFile = fopen(acLine, "r");
        if (NULL != pstFile)
        {
            fgets(acLine, 256, pstFile);
            sscanf(acLine, "%s\n", acAddr);
            fclose(pstFile);
        }
        snprintf(acLine, sizeof(acLine), "route -A inet6 add ::/0 gw %s dev wan0",  acAddr);
        system(acLine);
        /*END 2032203980  m00182044 2012-06-10 modify*/
    }
    return ;
}


char *WriteDHCP6CCfg(char *pszIfname, int iIANAFlag, int iIAPDFlag, int iDsliteFlag)
{
    FILE *fp = NULL;
    int iIfindex = -1;
    /* 实际上对于多线程来说，应该尽量避免使用静态变量，但是因为DHCP只可能被启动一次因此这里可以保证本文件内容不会被覆盖改写 */
    static char acFilePath[64] = {0};

    FUNCBEGIN();

    if (!pszIfname)
    {
        fprintf(stderr, "args NULL");
        return NULL;
    }

    snprintf(acFilePath, 63, WAN_DHCP6C_CFG, pszIfname);

    fp = fopen(acFilePath, "w");
    if (!fp)
    {
        loginfo(FUNC, "open file [%s] error", acFilePath);
        return NULL;
    }

    iIfindex = if_nametoindex(pszIfname);

    /* 文件格式形如：
    interface br0
    {
        send ia-na 201, iapd 2001;
        request ds-lite-address, ds-lite-domain-name, domain-name-servers;
    };
    id-assoc na 201
    {
    };
    id-assoc pd 2001
    {
    };
    */
    fprintf(fp, "interface %s\n{\n", pszIfname);
    if (iIANAFlag || iIAPDFlag)
    {
        fprintf(fp, "    send ");
    }
    else
    {
        fprintf(fp, "    information-only");
    }
    if (iIANAFlag && iIAPDFlag)
    {
        fprintf(fp, "ia-na %d, ", DHCP_IANA_NUM_BASE + iIfindex);
        fprintf(fp, "ia-pd %d", DHCP_IAPD_NUM_BASE + iIfindex);
    }
    else
    {
        if (iIANAFlag)
        {
            fprintf(fp, "ia-na %d", DHCP_IANA_NUM_BASE + iIfindex);
        }
        if (iIAPDFlag)
        {
            fprintf(fp, "ia-pd %d", DHCP_IAPD_NUM_BASE + iIfindex);
        }
    }
    fprintf(fp, ";\n    request ");
    if (iDsliteFlag)
    {
        fprintf(fp, "ds-lite-address, ds-lite-domain-name, ");
    }
    fprintf(fp, "domain-name-servers;\n};\n");
    if (iIANAFlag)
    {
        fprintf(fp, "id-assoc na %d \n{\n};\n", DHCP_IANA_NUM_BASE + iIfindex);
    }
    if (iIAPDFlag)
    {
        fprintf(fp, "id-assoc pd %d \n{\n};\n", DHCP_IAPD_NUM_BASE + iIfindex);
    }

    fclose(fp);

    FUNCEND();

    return acFilePath;
}


void ExitProcess(int signo)
{
    FUNCBEGIN();    
    loginfo(FUNC, "Now, begin to exit the process...\r\n\r\n\r\n\r\n");
    
#if 0
    int iIfindex = Dhcp6cGetIndex();
        if (0 >= iIfindex)
    {
        StopWanProcessByIndex(iIfindex);
    }
#endif


    exit_addrMonitor = 1;
    //exit_ok = 1;

    FUNCEND();
    
    //ExitProcessID(iIfindex);

    //exit(0);
}



int ExitProcessID(int iIfindex)
{
    key_t key;
    int iRet = -1;
    
    key = ftok("/proc/locks", iIfindex);
    if (0 > key)
    {
        loginfo(FUNC, "error: ftok error: %s", strerror(errno));
        return (0);
    }
    iRet = msgget(key, 0);
    if (0 < iRet)
    {
        msgctl(iRet, IPC_RMID, NULL);
    }

    return (0);
}

/* 判断给定的进程是否还存在 */
int GetPrevProcessEixtStatus(char *pszIfname)
{
    key_t key;
    int iRet = -1;
    int iIfindex = -1;

    iIfindex = if_nametoindex(pszIfname);
    
    key = ftok("/proc/locks", iIfindex);
    if (0 > key)
    {
        loginfo(FUNC, "error: ftok error: %s", strerror(errno));
        return (0);
    }
    iRet = msgget(key, IPC_CREAT|IPC_EXCL);
    if (0 > iRet) //((-1 == iRet) && (EEXIST == errno))
    {
        loginfo(FUNC, "key: %8x message id: %08x msgget error: %s", 
            key, iRet, strerror(errno));
        return (0);
    }
    else
    {
        loginfo(FUNC, "key: %8x message id: %08x msgget error: %s", 
            key, iRet, strerror(errno));
        return (1);
    }
}


