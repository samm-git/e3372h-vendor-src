#include "libbb.h"
#include "atpcommon.h"

#if ENABLE_GETOPT_LONG
const char bb_transtool_long_options[] ALIGN1 =
    "download\0" No_argument        "g"
	"upload\0"   No_argument        "s"
	"verbose\0"  No_argument        "v"
	"username\0" Required_argument  "u"
	"password\0" Required_argument  "p"
	"local\0"    Required_argument  "l"
	"remote\0"   Required_argument  "r"
	"port\0"     Required_argument  "P"
	"bind\0"     Required_argument  "B"
	"addr\0"     Required_argument  "A"
    ;
#endif

#if 0
const char *g_pcLocalIP     = NULL;
const char *g_pcTransBegin  = NULL;
int        g_lTransEnd      = -1;
int        g_lMaxTrans      = -1;
#endif
#ifdef ENABLE_FEATURE_IPV6
extern int iFTPflag;
#endif

/* 20120904 Add by weiqingchuan 61642 for Compiling Warning */
extern struct hostent *ATP_GPL_Gethostbyname(const char *name, int family, char* bindaddr);

int xbind_connect(const len_and_sockaddr *lsa, const char *ip)
{
	int fd;
    /*Start of MNT 2008-10-13 14:40 for 传输超时检测 by z65940*/
    g_TimeoutCnt = ATP_CONNECT_TIMEOUT_D;  // 每个命令的超时检测
    /*End of MNT 2008-10-13 14:40 for by z65940*/

    #if 0
    /*Added by yehuisheng00183935@20110806 修改IPv6链路地址访问问题--使用链路地址时，必须指定参数"sin6_scope_id"*/
    if ( AF_INET6 == lsa->u.sa.sa_family )
    {
        if (IN6_IS_ADDR_LINKLOCAL(&(((struct sockaddr_in6 *)(&(lsa->u.sa)))->sin6_addr)))
        {
            ((struct sockaddr_in6 *)(&(lsa->u.sa)))->sin6_scope_id = if_nametoindex("br0");
        }
    }
    /*Added by yehuisheng00183935@20110806 修改IPv6链路地址访问问题--使用链路地址时，必须指定参数"sin6_scope_id"*/
    #endif
    if (NULL == ip)
    {
        // No bind, the same as xconnect_stream
        fd = xconnect_stream(lsa);
        /*Start of MNT 2008-10-13 14:40 for 传输超时检测 by z65940*/
        g_TimeoutCnt = -1;
        /*End of MNT 2008-10-13 14:40 for by z65940*/
        return fd;
    }

    // Bind to specified local interface
    //fd = create_and_bind_stream_or_die(ip, get_nport(&(lsa->u.sa)));
    fd = create_and_bind_stream_or_die(ip, 0);
    xconnect(fd, &(lsa->u.sa), lsa->len);
    /*Start of MNT 2008-10-13 14:40 for 传输超时检测 by z65940*/
    g_TimeoutCnt = -1;
    /*End of MNT 2008-10-13 14:40 for by z65940*/
	return fd;
}

#if ENABLE_FEATURE_ATP_WGET_ZIP || ENABLE_FEATURE_ATP_FTP_ZIP || ENABLE_FEATURE_ATP_MCAST_ZIP
void atp_zip_write(gzFile fd, const void *buf, size_t count)
{
    int bytesWrite;

    if (!g_bCompress) {
        xwrite((int)fd, buf, count);
        return;
    }

    // Add to zip file
    bytesWrite = gzwrite(fd, buf, count);
    if ((size_t)bytesWrite != count) {
        bb_error_msg_and_die("zip write");
    }
}

#if 1
/*
1082704676 2011-11-02 for by j00196195
需要传入文件路径，因为文件句柄已经失效
*/
int atp_copy_to_zipfile(int src_fd, gzFile dst_file, int filesize, char* dst_path)
#else
int atp_copy_to_zipfile(int src_fd, gzFile dst_file, int filesize)
#endif
{
    int status = -1;
	off_t total = 0;
#if CONFIG_FEATURE_COPYBUF_KB <= 4
	char buffer[CONFIG_FEATURE_COPYBUF_KB * 1024];
	int buffer_size = sizeof(buffer);
#else
	char *buffer;
	int buffer_size;

	/* We want page-aligned buffer, just in case kernel is clever
	 * and can do page-aligned io more efficiently */
	buffer = mmap(NULL, CONFIG_FEATURE_COPYBUF_KB * 1024,
			PROT_READ | PROT_WRITE,
			MAP_PRIVATE | MAP_ANON,
			/* ignored: */ -1, 0);
	buffer_size = CONFIG_FEATURE_COPYBUF_KB * 1024;
	if (buffer == MAP_FAILED) {
		buffer = alloca(4 * 1024);
		buffer_size = 4 * 1024;
	}
#endif

	if ((src_fd < 0) || (NULL == dst_file))
		goto out;

    if (filesize <= 0) {
        filesize = buffer_size;
        status = 1; /* copy until eof */
    }

	while (1) {
		ssize_t rd;

        /*Start of MNT 2008-10-13 14:40 for 传输超时检测 by z65940*/
        g_TimeoutCnt = ATP_PACK_TIMEOUT_D;  // 收取每个包最长45秒
        /*End of MNT 2008-10-13 14:40 for by z65940*/
		rd = safe_read(src_fd, buffer, filesize > buffer_size ? buffer_size : filesize);
        /*Start of MNT 2008-10-13 14:40 for 传输超时检测 by z65940*/
        g_TimeoutCnt = -1;
        /*End of MNT 2008-10-13 14:40 for by z65940*/

		if (!rd) { /* eof - all done */
			status = 0;
			break;
		}
		if (rd < 0) {
			bb_perror_msg(bb_msg_read_error);
			break;
		}
/*Start of 1082704676 2011-11-02 for by j00196195*/
#if 1
	if( dst_path != NULL )
	{
		struct stat ptr_tmp;
		/* 使用stat函数确认文件路径是否存在，以此达到校验u盘是否被拔掉。
			这里没有使用fstat，因为在HFS文件系统中，拔掉u盘后，文件句柄已经失效了。
		*/
		if( stat(dst_path ,&ptr_tmp) < 0 )
		{
			status = 2;/*设置为返回值，区别于其他的错误类型。*/
			break;
		}
	}
#endif
/*End of 1082704676 2011-11-02 for by j00196195*/


		atp_zip_write(dst_file, (const void *)buffer, rd);
		total += rd;
		if (status < 0) { /* if we aren't copying till EOF... */
			filesize -= rd;
			if (!filesize) {
				/* 'size' bytes copied - all done */
				status = 0;
				break;
			}
		}
	}
 out:

#if CONFIG_FEATURE_COPYBUF_KB > 4
	if (buffer_size != 4 * 1024)
		munmap(buffer, buffer_size);
#endif
/*Start of 1082704676 2011-11-02 for by j00196195*/
#if 1
	/*有风险恰好文件写完成却没有校验，所以在写成功后再校验一次。*/
	if ( (status != 2) && (dst_path != NULL) )
	{
		struct stat ptr_tmp;
		if( stat(dst_path ,&ptr_tmp) < 0 )
		{
			status = 2;/*设置为返回值，区别于其他的错误类型。*/
		}
		sync();
	}
#endif
/*End of 1082704676 2011-11-02 for by j00196195*/

	return status ? -1 : total;
}
#endif

/*Start of MNT 2008-10-13 14:36 for 传输超时检测 by z65940*/
static void transfer_timeout(void)
{
    //fprintf(stderr, "g_TimeoutCnt: %d\n", g_TimeoutCnt);

    if (g_TimeoutCnt < 0)
    {
        alarm(1);
        return;
    }
    else if (g_TimeoutCnt > 0)
    {
        g_TimeoutCnt -= 1;
        alarm(1);
        return;
    }

    // Timeout
    xfunc_error_retval = ATP_TRANS_FILE_ERR;
    bb_error_msg_and_die("Transfer timeout!\n");
    exit(ATP_TRANS_TIMEOUT);
}

// Setup signals
/* 20120904 Modify by weiqingchuan 61642 for Compiling Warning */
void atp_setup_alarm(void)
{
    struct sigaction sa;

    memset(&sa, 0, sizeof(sa));
    sa.sa_flags = SA_RESTART;

    //sa.sa_handler = (void (*)(int)) finish_detection;
    //sigaction(SIGINT, &sa, NULL);

    sa.sa_handler = (void (*)(int))transfer_timeout;
    sigaction(SIGALRM, &sa, NULL);
    alarm(1);
}

#if 1
/*
1082704676 2011-11-02 for by j00196195
需要传入文件路径，因为文件句柄已经失效
*/
off_t atp_copy_fd_with_timeout(int src_fd, int dst_fd, off_t size, char* dst_path)
#else
off_t atp_copy_fd_with_timeout(int src_fd, int dst_fd, off_t size)
#endif
{
	int status = -1;
	off_t total = 0;
#if CONFIG_FEATURE_COPYBUF_KB <= 4
	char buffer[CONFIG_FEATURE_COPYBUF_KB * 1024];
	enum { buffer_size = sizeof(buffer) };
#else
	char *buffer;
	int buffer_size;

	/* We want page-aligned buffer, just in case kernel is clever
	 * and can do page-aligned io more efficiently */
	buffer = mmap(NULL, CONFIG_FEATURE_COPYBUF_KB * 1024,
			PROT_READ | PROT_WRITE,
			MAP_PRIVATE | MAP_ANON,
			/* ignored: */ -1, 0);
	buffer_size = CONFIG_FEATURE_COPYBUF_KB * 1024;
	if (buffer == MAP_FAILED) {
		buffer = alloca(4 * 1024);
		buffer_size = 4 * 1024;
	}
#endif

	if (src_fd < 0)
		goto out;

	if (!size) {
		size = buffer_size;
		status = 1; /* copy until eof */
	}

	while (1) {
		ssize_t rd;

        g_TimeoutCnt = ATP_PACK_TIMEOUT_D;
		rd = safe_read(src_fd, buffer, size > buffer_size ? buffer_size : size);
        g_TimeoutCnt = -1;

		if (!rd) { /* eof - all done */
			status = 0;
			break;
		}
		if (rd < 0) {
			bb_perror_msg(bb_msg_read_error);
			break;
		}

/*Start of 1082704676 2011-11-02 for by j00196195*/
#if 1
	if( dst_path != NULL )
	{
		struct stat ptr_tmp;
		/* 使用stat函数确认文件路径是否存在，以此达到校验u盘是否被拔掉。
			这里没有使用fstat，因为在HFS文件系统中，拔掉u盘后，文件句柄已经失效了。
		*/
		if( stat(dst_path ,&ptr_tmp) < 0 )
		{
			status = 2;/*设置为返回值，区别于其他的错误类型。*/
			break;
		}
	}
#endif
/*End of 1082704676 2011-11-02 for by j00196195*/

		/* dst_fd == -1 is a fake, else... */
		if (dst_fd >= 0)
		{
            g_TimeoutCnt = ATP_PACK_TIMEOUT_D;  // 发送每个包最长45秒
			ssize_t wr = 0; /* 20120904 Modify by weiqingchuan 61642 for Compiling Warning */
			wr = full_write(dst_fd, buffer, rd);
            g_TimeoutCnt = -1;
			if (wr < rd)
			{
				bb_perror_msg(bb_msg_write_error);
				break;
			}
		}
		total += rd;
		if (status < 0) { /* if we aren't copying till EOF... */
			size -= rd;
			if (!size) {
				/* 'size' bytes copied - all done */
				status = 0;
				break;
			}
		}
	}
 out:

#if CONFIG_FEATURE_COPYBUF_KB > 4
	if (buffer_size != 4 * 1024)
		munmap(buffer, buffer_size);
#endif

/*Start of 1082704676 2011-11-02 for by j00196195*/
#if 1
	/*有风险恰好文件写完成却没有校验，所以在写成功后再校验一次。*/
	if ( (status != 2) && (dst_path != NULL) )
	{
		struct stat ptr_tmp;
		if( stat(dst_path ,&ptr_tmp) < 0 )
		{
			status = 2;/*设置为返回值，区别于其他的错误类型。*/
		}
		sync();
	}
#endif
/*End of 1082704676 2011-11-02 for by j00196195*/

	return status ? -1 : total;
}

/*End of MNT 2008-10-13 14:36 for by z65940*/
#ifdef ENABLE_FEATURE_IPV6
int ATP_TransHostname2IP(char * hostname,char * buffer,int len,int * pIPflag)
{
    struct sockaddr_in sin;
	struct sockaddr_in6 sin6;
    struct hostent* h = NULL;

    if(NULL == hostname)
    {
        return 0;
    }
    if (inet_pton(AF_INET6, hostname, &sin6.sin6_addr) > 0)
    {
        memset(buffer,0,len);
        strcpy(buffer,hostname);
        *pIPflag = ATP_IPV6_IP;
        return 1;
    }
    else if (inet_pton(AF_INET, hostname, &sin.sin_addr) > 0)
    {
        memset(buffer,0,len);
        strcpy(buffer,hostname);
        *pIPflag = ATP_IPV4_IP;
        return 1;
    }
    /* 20120904 Modify by weiqingchuan 61642 for Compiling Warning */
    h = ATP_GPL_Gethostbyname(hostname, AF_INET6, (char*)NULL);
    if (h == NULL)
    {
        /* 20120904 Modify by weiqingchuan 61642 for Compiling Warning */
        h = ATP_GPL_Gethostbyname(hostname, AF_INET, (char*)NULL);
    }
    else
    {
        memcpy(sin6.sin6_addr.s6_addr, (char*)*h->h_addr_list, 16);
        memset(buffer,0,len);
        inet_ntop(AF_INET6, &sin6.sin6_addr, buffer, INET6_ADDRSTRLEN);
        *pIPflag = ATP_IPV6_IP;
        return 1;
    }
    if (h == NULL)
    {
        return 0;
    }
    else
    {
        sin.sin_addr.s_addr = *(int*)*h->h_addr_list;
        memset(buffer,0,len);
        strncpy(buffer ,inet_ntoa(sin.sin_addr),INET_ADDRSTRLEN);
        *pIPflag = ATP_IPV4_IP;
        return 1;
    }
}
#endif

#if defined(SUPPORT_ATP_TR143_DOWNLOAD_TCP) ||defined(SUPPORT_ATP_TR143_DOWNLOAD)
static int DownDiagResultSave(tr143_diag_state_t *pstDiagState)
{
    // printf("\r\n do store \r\n");
     FILE  *pfTr143Stats = NULL;
     unlink(TR143_DOWNDIAG_PID_FILE);
     pfTr143Stats = fopen (TR143_DOWNDIAG_RESULT_FILE , "w");
     if (pfTr143Stats == NULL)
     {
       return ATP_TRANS_SYS_ERR;
     }
     rewind(pfTr143Stats);
     fprintf(pfTr143Stats, "State = %s \n", pstDiagState->szState);
     fprintf(pfTr143Stats, "Rtime = %s \n", pstDiagState->szRomTime);
     fprintf(pfTr143Stats, "Btime = %s \n", pstDiagState->szBomTime);
     fprintf(pfTr143Stats, "Etime = %s \n", pstDiagState->szEomTime);
     fprintf(pfTr143Stats, "TextbytesRecv = %u \n", pstDiagState->ulTextBytesReceived);
     fprintf(pfTr143Stats, "TotalbytesRecv= %u \n", pstDiagState->ulTotalBytesReceived);
 #ifdef SUPPORT_ATP_TR143_DOWNLOAD_TCP
     fprintf(pfTr143Stats, "TcpRequestTime = %s \n", pstDiagState->szTcpOpenRequestTime);
     fprintf(pfTr143Stats, "TcpResponseTime = %s \n", pstDiagState->szTcpOpenResponseTime);
 #endif
     fclose(pfTr143Stats);
     //free(pstDiagState);
     return EXIT_SUCCESS;
}

void SetDownDiagState(tr143_diag_state_t *pstDiagState, char * pcState, bool bFlag)
{
     if(TRUE == bFlag)
     {
          snprintf(pstDiagState->szState, ATP_STRING_LEN_32, "%s", pcState);
          DownDiagResultSave(pstDiagState);
     }
}
#endif

#if defined(SUPPORT_ATP_TR143_UPLOAD_TCP) || defined(SUPPORT_ATP_TR143_UPLOAD)
static int UpDiagResultSave(tr143_diag_state_t *pstDiagState)
{
    // printf("\r\n do store \r\n");
     FILE  *pfTr143Stats = NULL;
     unlink(TR143_UPDIAG_PID_FILE);
     pfTr143Stats = fopen (TR143_UPDIAG_RESULT_FILE, "w");
     if (pfTr143Stats == NULL)
     {
       return ATP_TRANS_SYS_ERR;
     }
     rewind(pfTr143Stats);
     fprintf(pfTr143Stats, "State = %s \n", pstDiagState->szState);
     fprintf(pfTr143Stats, "Rtime = %s \n", pstDiagState->szRomTime);
     fprintf(pfTr143Stats, "Btime = %s \n", pstDiagState->szBomTime);
     fprintf(pfTr143Stats, "Etime = %s \n", pstDiagState->szEomTime);
     fprintf(pfTr143Stats, "TotabytesSent = %u \n", pstDiagState->ulTotalBytesSent);
 #ifdef SUPPORT_ATP_TR143_UPLOAD_TCP
     fprintf(pfTr143Stats, "TcpRequestTime = %s \n", pstDiagState->szTcpOpenRequestTime);
     fprintf(pfTr143Stats, "TcpResponseTime = %s \n", pstDiagState->szTcpOpenResponseTime);
 #endif
     fclose(pfTr143Stats);
     //free(pstDiagState);
     return EXIT_SUCCESS;
}

void SetUpDiagState(tr143_diag_state_t *pstDiagState, char * pcState, bool bFlag)
{
     if(TRUE == bFlag)
     {
          snprintf(pstDiagState->szState, ATP_STRING_LEN_32, "%s", pcState);
          UpDiagResultSave(pstDiagState);
     }
}

int diag_copy_fd_with_timeout( int dst_fd, int size)
{
	int status = -1;
	int total = 0;
       ssize_t wr;
#if CONFIG_FEATURE_COPYBUF_KB <= 4
	char buffer[CONFIG_FEATURE_COPYBUF_KB * 1024];
	enum { buffer_size = sizeof(buffer)-4  };
#else
	char *buffer;
	int buffer_size;
	/* We want page-aligned buffer, just in case kernel is clever
	 * and can do page-aligned io more efficiently */
	buffer = mmap(NULL, (CONFIG_FEATURE_COPYBUF_KB * 1024),
			PROT_READ | PROT_WRITE,
			MAP_PRIVATE | MAP_ANON,
			/* ignored: */ -1, 0);
	buffer_size = CONFIG_FEATURE_COPYBUF_KB * 1024;
	if (buffer == MAP_FAILED) {
		buffer = alloca(4 * 1024);
		buffer_size = 4 * 1024;
	}
#endif
      if (!size)
      {
	 return status;
      }
      if(size >= buffer_size)
      {
        memset(buffer, 'a', buffer_size);
        while(size >= buffer_size)
        {
           if (dst_fd >= 0) {
            g_TimeoutCnt = ATP_PACK_TIMEOUT_D;  // 发送每个包最长45秒
	     wr = full_write(dst_fd, buffer, buffer_size);
            g_TimeoutCnt = -1;
			if (wr < buffer_size) {
				bb_perror_msg(bb_msg_write_error);
				return status;
			}
		}
		total += wr;
              size -= wr;
        }
      }
      memset(buffer, '\0', buffer_size);
      memset(buffer, 'a', size);
      g_TimeoutCnt = ATP_PACK_TIMEOUT_D;  // 发送每个包最长45秒
      wr = full_write(dst_fd, buffer, size);
      g_TimeoutCnt = -1;
      if (wr < size)
      {
	    bb_perror_msg(bb_msg_write_error);
           return status;
	}
       total += wr;

#if CONFIG_FEATURE_COPYBUF_KB > 4
	if (buffer_size != 4 * 1024)
		munmap(buffer, buffer_size);
#endif
	return  total;
}
#endif


#if defined(SUPPORT_ATP_TR143_DOWNLOAD_TCP) ||defined(SUPPORT_ATP_TR143_DOWNLOAD) || defined(SUPPORT_ATP_TR143_UPLOAD_TCP) || defined(SUPPORT_ATP_TR143_UPLOAD)
int GetCurDateTime(char *pcBuf, int ulBufLen)
{
    struct tm   *curdate;
    struct timeval  stTimeVal;
   int       ret =0;

    if(NULL == pcBuf)
    {
        return EXIT_SUCCESS;
    }
    if ((ret = gettimeofday(&stTimeVal, NULL)) < 0)
    {
        return EXIT_SUCCESS;
    }
    curdate = gmtime((time_t *)(&(stTimeVal.tv_sec)));
    /*End of ATP网络协议组 2010-4-1 10:38 by h00163136*/
    //strftime(pcBuf, ulBufLen, "%a, %d %b %Y %H:%M:%S", curdate);
    strftime(pcBuf, ulBufLen, "%Y-%m-%dT%H:%M:%S", curdate);
    snprintf(pcBuf,ulBufLen, "%s.%d",pcBuf, (int)stTimeVal.tv_usec);

    return EXIT_SUCCESS;
}

void SetDiagState(tr143_diag_state_t *pstDiagState, char * pcState, bool bFlag)
{
      if(TRANS_TOOL_OPT_DOWNLOAD == pstDiagState->ulTransDir)
      {
            SetDownDiagState(pstDiagState, pcState, bFlag);
      }
      else
      {
           SetUpDiagState(pstDiagState, pcState, bFlag);
      }
}

void ATP_UTIL_GetIfcStatInfo(char *pszIfcName, ATP_UTIL_INTERFACE_STAT_ST *pstState)
{
    int iCount = 0;
    char acCol[17][32];
    char *pszChar = NULL;
    FILE *fs      = NULL;

    char acLine[512], acBuf[512];

    memset(acCol, 0, sizeof(acCol));
    memset(acLine, 0, sizeof(acLine));
    memset(acBuf,  0, sizeof(acBuf));

    fs = fopen("/proc/net/dev", "r");

    if (NULL == fs)
    {
        return;
    }

    if ((NULL == pszIfcName) || (NULL == pstState))
    {
        fclose(fs);
        return;
    }
#ifdef ATP_PCLINT
    pszIfcName = pszIfcName;
#endif

    while (NULL != fgets(acLine, sizeof(acLine), fs) )
    {
        /* 忽略文件开始2行 */
        if ((iCount++) < 2 )
        {
            continue;
        }

        /* normally line will have the following example value
           "eth0: 19140785 181329 0 0 0 0 0 0 372073009 454948 0 0 0 0 0 0"
           but when the number is too big then line will have the following example value
           "eth0:19140785 181329 0 0 0 0 0 0 372073009 454948 0 0 0 0 0 0"
           so to make the parsing correctly, the following codes are added
           to insert space between ':' and number */
        pszChar = strchr(acLine, ':');
        if (NULL != pszChar)
        {
            pszChar++;
        }
        if ((NULL != pszChar) && isdigit(*pszChar) )
        {
            //strcpy(acBuf, pszChar);
            snprintf(acBuf, sizeof(acBuf), "%s", pszChar);
            *pszChar = ' ';
           strcpy(++pszChar, acBuf);
        }

        /* 如果接口被找到，则存储统计值 */
        if (NULL != strstr(acLine, pszIfcName) )
        {
            sscanf(acLine, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
                   acCol[0], acCol[1], acCol[2], acCol[3], acCol[4], acCol[5],
                   acCol[6], acCol[7], acCol[8], acCol[9], acCol[10], acCol[11],
                   acCol[12],acCol[13], acCol[14], acCol[15], acCol[16]);
            pstState->ulBytesReceived        = strtoul(acCol[1],0,10);
            pstState->ulPacketsReceived      = strtoul(acCol[2],0,10);
            pstState->ulPacketsReceivedErr   = strtoul(acCol[3],0,10);
            pstState->ulPacketsReceivedDrops = strtoul(acCol[4],0,10);
            pstState->ulBytesSent            = strtoul(acCol[9],0,10);
            pstState->ulPacketsSent          = strtoul(acCol[10],0,10);
            pstState->ulPacketsSentErr       = strtoul(acCol[11],0,10);
            pstState->ulPacketsSentDrops     = strtoul(acCol[12],0,10);

            break;
        }
    }
    fclose(fs);
}

#endif
