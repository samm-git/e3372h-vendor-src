/* vi: set sw=4 ts=4: */
/*
 * ftpget
 *
 * Mini implementation of FTP to retrieve a remote file.
 *
 * Copyright (C) 2002 Jeff Angielski, The PTR Group <jeff@theptrgroup.com>
 * Copyright (C) 2002 Glenn McGrath
 *
 * Based on wget.c by Chip Rosenthal Covad Communications
 * <chip@laserlink.net>
 *
 * Licensed under GPLv2 or later, see file LICENSE in this tarball for details.
 */
//usage:#define ftpget_trivial_usage 
//usage:       "[options] remote-host"
//usage:#define ftpget_full_usage 
//usage:       "Download or upload via FTP.\n\n" 
//usage:       "\t-g, --download         Download\n" 
//usage:       "\t-s, --upload           Upload\n" 
//usage:       "\t-v, --verbose          Verbose\n" 
//usage:       "\t-u, --username         Username to be used\n" 
//usage:       "\t-p, --password         Password to be used\n" 
//usage:       "\t-l, --local            Local file path\n" 
//usage:       "\t-r, --remote           Remote file path\n" 
//usage:       "\t-P, --port             Port to be used, optional\n" 
//usage:       "\t-B, --bind             Bind local ip, optional\n" 
//usage:       "\t-A, --addr             Remote resolved ip, optional\n" 
//usage:       "\t-b, --begin            Transfer start position\n" 
//usage:       "\t-e, --len              Transfer length\n" 
//usage:       "\t-m, --max              Max transfer size\n" 
//usage:       "\t-c, --compress         Compress downloaded file" 
        

//usage:#define ftpput_trivial_usage ftpget_trivial_usage
//usage:#define ftpput_full_usage ftpget_full_usage


#include <getopt.h>
#include "libbb.h"
#include "atpcommon.h"



typedef struct ftp_host_info_s {
	const char *user;
	const char *password;
	struct len_and_sockaddr *lsa;
} ftp_host_info_t;

static smallint verbose_flag;
/*Start of Maintain 2008-3-1 14:28 for 修改Busybox中的传输工具，满足cwmp传输需求 by z65940*/
#if 0
#else
static FILE *control_stream_in = NULL;
#endif
/*End of Maintain 2008-3-1 14:28 for 修改Busybox中的传输工具，满足cwmp传输需求 by z65940*/
#ifdef ENABLE_FEATURE_IPV6
int iFTPflag = 0;
#endif
static void ftp_die(const char *msg, const char *remote) ;
static void ftp_die(const char *msg, const char *remote)
{
	/* Guard against garbage from remote server */
	const char *cp = remote;
  
	while (*cp >= ' ' && *cp < '\x7f') cp++;

    bb_error_msg_and_die("unexpected server response%s%s: %.*s",
			msg ? " to " : "", msg ? msg : "",
			(int)(cp - remote), remote);

}


static int ftpcmd(const char *s1, const char *s2, FILE *stream, char *buf)
{
	unsigned n;
	if (verbose_flag) {
		bb_error_msg("cmd %s %s", s1, s2);
	}

    /*Start of MNT 2008-10-13 14:40 for 传输超时检测 by z65940*/
    g_TimeoutCnt = 20;  // 每个命令的超时检测
    /*End of MNT 2008-10-13 14:40 for by z65940*/

	if (s1) {
		if (s2) {
			fprintf(stream, "%s %s\r\n", s1, s2);
		} else {
			fprintf(stream, "%s\r\n", s1);
		}
        /*Start of Maintain 2008-3-1 15:47 for 修改Busybox中的传输工具，满足cwmp传输需求 by z65940*/
        fflush(stream);
        /*End of Maintain 2008-3-1 15:47 for 修改Busybox中的传输工具，满足cwmp传输需求 by z65940*/
	}
	do {
		char *buf_ptr;

        /*Start of Maintain 2008-3-1 15:26 for 修改Busybox中的传输工具，满足cwmp传输需求 by z65940*/
    #if 0
		if (fgets(buf, 510, stream) == NULL) {
    #else
		if (fgets(buf, 510, control_stream_in) == NULL) {
    #endif
        /*End of Maintain 2008-3-1 15:26 for 修改Busybox中的传输工具，满足cwmp传输需求 by z65940*/
			bb_perror_msg_and_die("fgets");
		}
		buf_ptr = strstr(buf, "\r\n");
		if (buf_ptr) {
			*buf_ptr = '\0';
		}
	} while (!isdigit(buf[0]) || buf[3] != ' ');

	buf[3] = '\0';
	n = xatou(buf);
	buf[3] = ' ';

    /*Start of MNT 2008-10-13 14:40 for 传输超时检测 by z65940*/
    g_TimeoutCnt = -1;  // 每个命令的超时检测
    /*End of MNT 2008-10-13 14:40 for by z65940*/

	return n;
}

static int xconnect_ftpdata(ftp_host_info_t *server, char *buf)
{
	char *buf_ptr;
	unsigned short port_num;

	/* Response is "NNN garbageN1,N2,N3,N4,P1,P2[)garbage]
	 * Server's IP is N1.N2.N3.N4 (we ignore it)
	 * Server's port for data connection is P1*256+P2 */
	#ifdef ENABLE_FEATURE_IPV6
	if(iFTPflag == ATP_IPV6_IP)
	{
        buf_ptr = strrchr(buf, '|');
    	if (buf_ptr) *buf_ptr = '\0';
        buf_ptr = strrchr(buf, '|');
    	if (buf_ptr) *buf_ptr = '\0';
    	port_num = xatoul_range(buf_ptr + 1, 0, 65535);
    }
    else
    {
    #endif
        buf_ptr = strrchr(buf, ')');
    	if (buf_ptr) *buf_ptr = '\0';

    	buf_ptr = strrchr(buf, ',');
    	*buf_ptr = '\0';
    	port_num = xatoul_range(buf_ptr + 1, 0, 255);

    	buf_ptr = strrchr(buf, ',');
    	*buf_ptr = '\0';
    	port_num += xatoul_range(buf_ptr + 1, 0, 255) * 256;
    #ifdef ENABLE_FEATURE_IPV6
    }
    #endif
    set_nport(&(server->lsa->u.sa), htons(port_num));
    /*Start of Maintain 2008-3-1 16:11 for 修改Busybox中的传输工具，满足cwmp传输需求 by z65940*/
#if 0
	return xconnect_stream(server->lsa);
#else
	return xbind_connect(server->lsa, g_pcLocalIP);
#endif
    /*End of Maintain 2008-3-1 16:11 for 修改Busybox中的传输工具，满足cwmp传输需求 by z65940*/
}

/*Start of 维护管理组 2008-4-15 16:55 for 修改ftp支持远程路径 by handy*/
#if 0
static FILE *ftp_login(ftp_host_info_t *server)
#else
static FILE *ftp_login(ftp_host_info_t *server, char *pcRemotePath, tr143_diag_state_t  *pstDiagState, bool bDiagFlag)
#endif
/*End of 维护管理组 2008-4-15 16:55 for 修改ftp支持远程路径 by handy*/
{
	FILE *control_stream;
	char buf[512];
/*Start of Maintain 2008-3-1 15:21 for 修改Busybox中的传输工具，满足cwmp传输需求 by z65940*/
    int  sock;
/*End of Maintain 2008-3-1 15:21 for 修改Busybox中的传输工具，满足cwmp传输需求 by z65940*/

/*Start of 维护管理组 2008-4-15 16:56 for 修改ftp支持远程路径 by handy*/
    char *pcName;
/*End of 维护管理组 2008-4-15 16:56 for 修改ftp支持远程路径 by handy*/

	/* Connect to the command socket */
    /*Start of Maintain 2008-3-1 15:5 for 修改Busybox中的传输工具，满足cwmp传输需求 by z65940*/
#if 0
	control_stream = fdopen(xconnect_stream(server->lsa), "r+");
	if (control_stream == NULL) {
		/* fdopen failed - extremely unlikely */
		bb_perror_nomsg_and_die();
	}
#else
    // If error, default to be connection timeout
    xfunc_error_retval = ATP_TRANS_TIMEOUT;
    // This function will bind if g_pcLocalIP is not NULL
    sock = xbind_connect(server->lsa, g_pcLocalIP);
    // Some system can not read and write one socket at the same time
	control_stream = fdopen(sock, "w");
    control_stream_in = fdopen(sock, "r");
	if ((control_stream == NULL) || (control_stream_in == NULL)){
		/* fdopen failed - extremely unlikely */
		bb_perror_nomsg_and_die();
	}

    // Restore to system error
    xfunc_error_retval = ATP_TRANS_SYS_ERR;
#endif
    /*End of Maintain 2008-3-1 15:5 for 修改Busybox中的传输工具，满足cwmp传输需求 by z65940*/

	if (ftpcmd(NULL, NULL, control_stream, buf) != 220) {
              SetDiagState(pstDiagState, "Error_InitConnectionFailed", bDiagFlag);
		ftp_die(NULL, buf);
	}

	/*  Login to the server */
	switch (ftpcmd("USER", server->user, control_stream, buf)) {
	case 230:
		break;
	case 331:
		if (ftpcmd("PASS", server->password, control_stream, buf) != 230) {
                SetDiagState(pstDiagState, "Error_LoginFailed", bDiagFlag);
            /*Start of Maintain 2008-3-1 16:25 for 修改Busybox中的传输工具，满足cwmp传输需求 by z65940*/
            xfunc_error_retval = ATP_TRANS_AUTH_ERR;
            /*End of Maintain 2008-3-1 16:25 for 修改Busybox中的传输工具，满足cwmp传输需求 by z65940*/
			ftp_die("PASS", buf);
		}
		break;
	default:
              SetDiagState(pstDiagState, "Error_PasswordRequestFailed", bDiagFlag);
		ftp_die("USER", buf);
	}

	if(ftpcmd("TYPE I", NULL, control_stream, buf) != 200)
	{
              SetDiagState(pstDiagState, "Error_NoTransferMode", bDiagFlag);
		ftp_die("TYPE I", buf);
	}

    /*Start of 维护管理组 2008-4-15 16:56 for 修改ftp支持远程路径 by handy*/
    pcName = strrchr(pcRemotePath, '/');
    if (NULL != pcName)
    {
        (*pcName) = '\0';
    }
    /*Modified by lvxin00135113@20110104 ftp client登入后先执行PWD命令，而不再执行CWD*/
#if 1
    if ((NULL != pcName) && ('\0' != pcRemotePath[0]))
    {
        int iRet = 0;
        iRet = ftpcmd("CWD", pcRemotePath, control_stream, buf);

        /*路径错误，修改问题单1082903922 */
        if( 5 == ( iRet / 100 ) )
        {
             ftp_die("CWD", buf);
        }
    }
    else
    {
        //ftpcmd("CWD", "/", control_stream, buf);
    }
#else
    if ((NULL != pcName) && ('\0' != pcRemotePath[0]))
    {
        ftpcmd("PWD", pcRemotePath, control_stream, buf);
    }
    else
    {
        ftpcmd("PWD", "/", control_stream, buf);
    }
#endif
  /*Modified by lvxin00135113@20110104 ftp client登入后先执行PWD命令，而不再执行CWD*/

    if (NULL != pcName)     // 恢复最后一个目录的/
    {
        (*pcName) = '/';
    }
    /*End of 维护管理组 2008-4-15 16:56 for 修改ftp支持远程路径 by handy*/

	return control_stream;
}

#if !ENABLE_ATP_FTPGET
int ftp_receive(ftp_host_info_t *server, FILE *control_stream,
		const char *local_path, const char *server_path, tr143_diag_state_t  *pstDiagState, bool bDiagFlag);
#else
static
int ftp_receive(ftp_host_info_t *server, FILE *control_stream,
		const char *local_path, const char *server_path, tr143_diag_state_t  *pstDiagState, bool bDiagFlag)
{
	char buf[512];
/* I think 'filesize' usage here is bogus. Let's see... */
	off_t filesize = -1;
//#define filesize ((off_t)-1)
	int fd_data;
#if ENABLE_FEATURE_ATP_FTP_ZIP
	gzFile fd_local = NULL;
#else
    int fd_local = -1;
#endif
    int transsize;
    int lBytesRecv = 0;
#if defined(SUPPORT_ATP_TR143_DOWNLOAD_TCP) ||defined(SUPPORT_ATP_TR143_DOWNLOAD)
    unsigned int ulIfBytesBtimeRecv = 0;
    unsigned int ulIfBytesEtimeRecv = 0;
    ATP_UTIL_INTERFACE_STAT_ST stIfcStats;

    memset(&stIfcStats, 0, sizeof(ATP_UTIL_INTERFACE_STAT_ST));
#endif
	/* Connect to the data socket */
    #ifdef ENABLE_FEATURE_IPV6
    if(iFTPflag == ATP_IPV6_IP)
    {
        if (ftpcmd("EPSV", NULL, control_stream, buf) != 229) {
              SetDownDiagState(pstDiagState, "Error_NoPASV", bDiagFlag);
		ftp_die("EPSV", buf);
        }
    }
    else
    {
    #endif
        if (ftpcmd("PASV", NULL, control_stream, buf) != 227) {
              SetDownDiagState(pstDiagState, "Error_NoPASV", bDiagFlag);
		ftp_die("PASV", buf);
        }
    #ifdef ENABLE_FEATURE_IPV6
	}
    #endif
    #ifdef SUPPORT_ATP_TR143_DOWNLOAD_TCP
        if(TRUE == bDiagFlag)
        {
              GetCurDateTime(pstDiagState->szTcpOpenRequestTime, ATP_STRING_LEN_64);
        }
    #endif
	fd_data = xconnect_ftpdata(server, buf);

       if(TRUE == bDiagFlag)
       {
            if(fd_data < 0)
            {
                  SetDownDiagState(pstDiagState, "Error_NoResponse", bDiagFlag);
                  //conncet fail
                  return ATP_TRANS_SYS_ERR;
            }
         #ifdef SUPPORT_ATP_TR143_DOWNLOAD_TCP
            GetCurDateTime(pstDiagState->szTcpOpenResponseTime, ATP_STRING_LEN_64);
         #endif
       }
	if (ftpcmd("SIZE", server_path, control_stream, buf) == 213) {
		filesize = BB_STRTOOFF(buf + 4, NULL, 10);
		if (errno || filesize < 0)
		{
                     SetDownDiagState(pstDiagState, "Error_IncorrectSize", bDiagFlag);
			ftp_die("SIZE", buf);
		}
	}
    else
    {
        SetDownDiagState(pstDiagState, "Error_IncorrectSize", bDiagFlag);
        return ATP_TRANS_FILE_ERR;
    }
    if (filesize < g_lTransEnd) {
        SetDownDiagState(pstDiagState, "Error_IncorrectSize", bDiagFlag);
        xfunc_error_retval = ATP_TRANS_FILE_ERR;
        ftp_die("SIZE", buf);
    }
    transsize = filesize;

	if (g_lTransEnd >= 0) {
        // 如果指定了传输起始偏移量，则指向该地址
        if (NULL != g_pcTransBegin) {
    		sprintf(buf, "REST %s", g_pcTransBegin);

            transsize = atoi(g_pcTransBegin);

            // 如果没有指定下载长度，则下载后续所有字节
            if (NULL == g_pcTransEnd) {
                transsize = filesize - transsize;
                filesize = -1;
            }
            else if (0 == g_lTransEnd) {
                g_lTransEnd = transsize + 1;
                transsize = 1;
            }
            else {
                // 重新计算长度
                g_lTransEnd -= transsize;
                g_lTransEnd += 1;
                transsize = g_lTransEnd;
            }
        } else {
            // 如果没有指定起始偏移量，则下载末尾的g_pcTransBegin字节
            sprintf(buf, "REST %d", (int)(filesize - (off_t)g_lTransEnd));
            transsize = (int)g_lTransEnd;
        }
        filesize = transsize;

        if (ftpcmd(buf, NULL, control_stream, buf) != 350) {
            SetDownDiagState(pstDiagState, "Error_ Timeout", bDiagFlag);
			ftp_die("REST", buf);
		}
	}
    else {  // 没有分段传输，则下载所有字节
        // 判断文件大小是否过大
        filesize = -1;
    }

    // 不使能压缩时，可以直接判断文件是否过大
    if ((g_lMaxTrans > 0) && (transsize > g_lMaxTrans)) {
        xfunc_error_retval = ATP_TRANS_FILE_ERR;
        bb_error_msg_and_die("File too big\n");
    }
  #if defined(SUPPORT_ATP_TR143_DOWNLOAD_TCP) ||defined(SUPPORT_ATP_TR143_DOWNLOAD)
     if(TRUE == bDiagFlag)
     {
         GetCurDateTime(pstDiagState->szRomTime, ATP_STRING_LEN_64);
         ATP_UTIL_GetIfcStatInfo(pstDiagState->szIfName, &stIfcStats);
         ulIfBytesBtimeRecv = stIfcStats.ulBytesReceived;
     }
  #endif
	if (ftpcmd("RETR", server_path, control_stream, buf) > 150) {
        SetDownDiagState(pstDiagState, "Error_ Timeout", bDiagFlag);
        xfunc_error_retval = ATP_TRANS_FILE_ERR;
		ftp_die("RETR", buf);
	}

	/* only make a local file if we know that one exists on the remote server */
	if (fd_local <= 0) {
        xfunc_error_retval = ATP_TRANS_FILE_ERR;
    #if ENABLE_FEATURE_ATP_FTP_ZIP
        if (g_bCompress) {
            fd_local = gzopen(local_path, "wb9");
        } else {
            fd_local = (gzFile)xopen(local_path, O_CREAT | O_TRUNC | O_WRONLY);
        }
        if ((int)fd_local <= 0) {
            bb_error_msg_and_die("Open file err\n");
        }
    #else
		fd_local = xopen(local_path, O_CREAT | O_TRUNC | O_WRONLY);
    #endif
        xfunc_error_retval = ATP_TRANS_SYS_ERR;
	}
  #if defined(SUPPORT_ATP_TR143_DOWNLOAD_TCP) ||defined(SUPPORT_ATP_TR143_DOWNLOAD)
     if(TRUE == bDiagFlag)
     {
          GetCurDateTime(pstDiagState->szBomTime, ATP_STRING_LEN_64);
     }
   #endif
	/* Copy the file */
#if ENABLE_FEATURE_ATP_FTP_ZIP
    if (g_bCompress) {
/*
1082704676 2011-11-02 for by j00196195
需要传入文件路径，因为文件句柄已经失效
*/
        lBytesRecv = atp_copy_to_zipfile(fd_data, fd_local, filesize, (char *)local_path);
        if (lBytesRecv <= 0){
            SetDownDiagState(pstDiagState, "Error_ Timeout", bDiagFlag);
/*
1082704676 2011-11-02 for by j00196195
以前的代码，出错后忘记关闭文件句柄
*/
        		close((int)fd_local);
            return ATP_TRANS_SYS_ERR;
        }
        gzclose(fd_local);
        if (-1 != filesize) {
            // Abort the current transfer now
            transsize = 0;
            transsize = ftpcmd("ABOR", NULL, control_stream, buf);
            if (226 == transsize)
            {
                close(fd_data);
            #if defined(SUPPORT_ATP_TR143_DOWNLOAD_TCP) ||defined(SUPPORT_ATP_TR143_DOWNLOAD)
                if(TRUE == bDiagFlag)
                {
                    pstDiagState->ulTextBytesReceived = (unsigned int)lBytesRecv;
                    GetCurDateTime(pstDiagState->szEomTime, ATP_STRING_LEN_64);
                    ATP_UTIL_GetIfcStatInfo(pstDiagState->szIfName, &stIfcStats);
                    ulIfBytesEtimeRecv = stIfcStats.ulBytesReceived;
                    pstDiagState->ulTotalBytesReceived = ulIfBytesEtimeRecv - ulIfBytesBtimeRecv;
                    SetDownDiagState(pstDiagState, "Completed", bDiagFlag);
                }
            #endif
                ftpcmd("QUIT", NULL, control_stream, buf);
	            return EXIT_SUCCESS;
            }
        }
    } else {
        if (filesize != -1) {
            /*Start of MNT 2008-10-13 15:8 for 传输超时检测 by z65940*/
/*
1082704676 2011-11-02 for by j00196195
需要传入文件路径，因为文件句柄已经失效
*/
            /* 20120904 Modify by weiqingchuan 61642 for Compiling Warning */
            lBytesRecv = (int)atp_copy_fd_with_timeout(fd_data, (int)fd_local, filesize, (char *)local_path);
    		if (lBytesRecv <= 0){
            /*End of MNT 2008-10-13 15:8 by z65940*/
			SetDownDiagState(pstDiagState, "Error_ Timeout", bDiagFlag);
/*
1082704676 2011-11-02 for by j00196195
以前的代码，出错后忘记关闭文件句柄
*/
        		close((int)fd_local);
    			return ATP_TRANS_SYS_ERR;
        }
            // Abort the current transfer now
            transsize = 0;
            transsize = ftpcmd("ABOR", NULL, control_stream, buf);
            if (226 == transsize)
            {
                close((int)fd_local);
                close(fd_data);
            #if defined(SUPPORT_ATP_TR143_DOWNLOAD_TCP) ||defined(SUPPORT_ATP_TR143_DOWNLOAD)
                if(TRUE == bDiagFlag)
                {
                    pstDiagState->ulTextBytesReceived = (unsigned int)lBytesRecv;
                    GetCurDateTime(pstDiagState->szEomTime, ATP_STRING_LEN_64);
                    ATP_UTIL_GetIfcStatInfo(pstDiagState->szIfName, &stIfcStats);
                    ulIfBytesEtimeRecv = stIfcStats.ulBytesReceived;
                    pstDiagState->ulTotalBytesReceived = ulIfBytesEtimeRecv - ulIfBytesBtimeRecv;
                    SetDownDiagState(pstDiagState, "Completed", bDiagFlag);
                }
           #endif
                ftpcmd("QUIT", NULL, control_stream, buf);
	            return EXIT_SUCCESS;
            }
    	} else {
            /*Start of MNT 2008-10-13 15:8 for 传输超时检测 by z65940*/
/*
1082704676 2011-11-02 for by j00196195
需要传入文件路径，因为文件句柄已经失效
*/
            /* 20120904 Modify by weiqingchuan 61642 for Compiling Warning */
            lBytesRecv = (int)atp_copy_fd_with_timeout(fd_data, (int)fd_local, 0, (char *)local_path);
    		if (lBytesRecv <= 0)
            /*End of MNT 2008-10-13 15:8 by z65940*/
        {
                     SetDownDiagState(pstDiagState, "Error_ Timeout", bDiagFlag);
/*
1082704676 2011-11-02 for by j00196195
以前的代码，出错后忘记关闭文件句柄
*/
       	 	close((int)fd_local);
    			return ATP_TRANS_SPACE_FULL_ERR;
        }
    	}
        close((int)fd_local);
    }
#else
	if (filesize != -1) {
        /*Start of MNT 2008-10-13 15:9 for 传输超时检测 by z65940*/
/*
1082704676 2011-11-02 for by j00196195
需要传入文件路径，因为文件句柄已经失效
*/
        /* 20120904 Modify by weiqingchuan 61642 for Compiling Warning */
        lBytesRecv = (int)atp_copy_fd_with_timeout(fd_data, fd_local, filesize, (char *)local_path);
		if (lBytesRecv <= 0){
        /*End of MNT 2008-10-13 15:9 by z65940*/
                     SetDownDiagState(pstDiagState, "Error_ Timeout", bDiagFlag);
/*
1082704676 2011-11-02 for by j00196195
以前的代码，出错后忘记关闭文件句柄
*/
       	 	close((int)fd_local);
			return ATP_TRANS_SYS_ERR;
    	}
        // Abort the current transfer now
        transsize = 0;
        transsize = ftpcmd("ABOR", NULL, control_stream, buf);
        if (226 == transsize)
        {
            close((int)fd_local);
            close(fd_data);
         #if defined(SUPPORT_ATP_TR143_DOWNLOAD_TCP) ||defined(SUPPORT_ATP_TR143_DOWNLOAD)
            if(TRUE == bDiagFlag)
            {
                pstDiagState->ulTextBytesReceived = (unsigned int)lBytesRecv;
                GetCurDateTime(pstDiagState->szEomTime, ATP_STRING_LEN_64);
                ATP_UTIL_GetIfcStatInfo(pstDiagState->szIfName, &stIfcStats);
                ulIfBytesEtimeRecv = stIfcStats.ulBytesReceived;
                pstDiagState->ulTotalBytesReceived = ulIfBytesEtimeRecv - ulIfBytesBtimeRecv;
                SetDownDiagState(pstDiagState, "Completed", bDiagFlag);
              }
         #endif
            ftpcmd("QUIT", NULL, control_stream, buf);
            return EXIT_SUCCESS;
        }
	} else {
        /*Start of MNT 2008-10-13 15:9 for 传输超时检测 by z65940*/
/*
1082704676 2011-11-02 for by j00196195
需要传入文件路径，因为文件句柄已经失效
*/
        /* 20120904 Modify by weiqingchuan 61642 for Compiling Warning */
        lBytesRecv = (int)atp_copy_fd_with_timeout(fd_data, fd_local, 0, (char *)local_path);
		if (lBytesRecv <= 0){
        /*End of MNT 2008-10-13 15:9 by z65940*/
                     SetDownDiagState(pstDiagState, "Error_ Timeout", bDiagFlag);
/*
1082704676 2011-11-02 for by j00196195
以前的代码，出错后忘记关闭文件句柄
*/
       	 	close((int)fd_local);
			return ATP_TRANS_SYS_ERR;
    	}
	}
    close(fd_local);
#endif
#if defined(SUPPORT_ATP_TR143_DOWNLOAD_TCP) ||defined(SUPPORT_ATP_TR143_DOWNLOAD)
 if(TRUE == bDiagFlag)
 {
    pstDiagState->ulTextBytesReceived = (unsigned int)lBytesRecv;
    GetCurDateTime(pstDiagState->szEomTime, ATP_STRING_LEN_64);
    ATP_UTIL_GetIfcStatInfo(pstDiagState->szIfName, &stIfcStats);
    ulIfBytesEtimeRecv = stIfcStats.ulBytesReceived;
    pstDiagState->ulTotalBytesReceived = ulIfBytesEtimeRecv - ulIfBytesBtimeRecv;
    SetDownDiagState(pstDiagState, "Completed", bDiagFlag);
 }
#endif
	/* close it all down */
	close(fd_data);
    /*Start of MNT 2008-8-27 14:58 for 与WindowsServer 2003对接 by z65940*/
#if 0
	if (ftpcmd(NULL, NULL, control_stream, buf) != 226) {
#else
    transsize = ftpcmd(NULL, NULL, control_stream, buf);
	if ((transsize != 226) && (transsize != 225)) {
#endif
    /*End of MNT 2008-8-27 14:58 by z65940*/
		ftp_die(NULL, buf);
	}
	ftpcmd("QUIT", NULL, control_stream, buf);

	return EXIT_SUCCESS;
}
#endif



#if !CONFIG_ATP_FTPPUT
int ftp_send(ftp_host_info_t *server, FILE *control_stream,
		  const char *local_path, const char *server_path, tr143_diag_state_t  *pstDiagState, bool bDiagFlag);
#else
static
int ftp_send(ftp_host_info_t *server, FILE *control_stream,
		 const char *local_path, const char *server_path, tr143_diag_state_t  *pstDiagState, bool bDiagFlag)
{
    
    struct stat sbuf;
    char buf[512];
    int fd_data;

    /* 20120904 Modify by weiqingchuan 61642 for Compiling Warning */
    int fd_local = 0;
    int response;
         
#if defined(SUPPORT_ATP_TR143_UPLOAD_TCP) || defined(SUPPORT_ATP_TR143_UPLOAD)
       unsigned int ulIfBytesBtimeSent = 0;
       unsigned int ulIfBytesEtimeSent = 0;
      ATP_UTIL_INTERFACE_STAT_ST stIfcStats;
    memset(&stIfcStats, 0, sizeof(ATP_UTIL_INTERFACE_STAT_ST));
#endif

	/*  Connect to the data socket */
    #ifdef ENABLE_FEATURE_IPV6
    if(iFTPflag == ATP_IPV6_IP)
    {
        if (ftpcmd("EPSV", NULL, control_stream, buf) != 229) {
              SetUpDiagState(pstDiagState,  "Error_NoPASV", bDiagFlag);
		ftp_die("EPSV", buf);
        }
    }
    else
    {
    #endif
        if (ftpcmd("PASV", NULL, control_stream, buf) != 227) {
              SetUpDiagState(pstDiagState,  "Error_NoPASV", bDiagFlag);
		ftp_die("PASV", buf);
        }
    #ifdef ENABLE_FEATURE_IPV6
    }
    #endif

    #ifdef SUPPORT_ATP_TR143_UPLOAD_TCP
        if(TRUE == bDiagFlag)
        {
              GetCurDateTime(pstDiagState->szTcpOpenRequestTime, ATP_STRING_LEN_64);
        }
    #endif
	fd_data = xconnect_ftpdata(server, buf);

       if(TRUE == bDiagFlag)
       {
             if(fd_data < 0)
             {
                  SetUpDiagState(pstDiagState, "Error_NoResponse", bDiagFlag);
                  //conncet fail
                  return ATP_TRANS_SYS_ERR;
             }
          #ifdef SUPPORT_ATP_TR143_UPLOAD_TCP
            GetCurDateTime(pstDiagState->szTcpOpenResponseTime, ATP_STRING_LEN_64);
          #endif
         xfunc_error_retval = ATP_TRANS_SYS_ERR;
            sprintf(buf, "ALLO %"OFF_FMT"u", atoi(local_path));
            response = ftpcmd(buf, NULL, control_stream, buf);
		switch (response) {
		case 200:
		case 202:
        /* 某些FTP Server不识别ALLO命令*/
        default:
			break;
		}
         #if defined(SUPPORT_ATP_TR143_UPLOAD_TCP) || defined(SUPPORT_ATP_TR143_UPLOAD)
            GetCurDateTime(pstDiagState->szRomTime, ATP_STRING_LEN_64);
         #endif
       }
       else
       {
            fd_local = STDIN_FILENO;
	     if (NOT_LONE_DASH(local_path)) {
               xfunc_error_retval = ATP_TRANS_FILE_ERR;
		    fd_local = xopen(local_path, O_RDONLY);
               xfunc_error_retval = ATP_TRANS_SYS_ERR;
		    fstat(fd_local, &sbuf);
		    sprintf(buf, "ALLO %"OFF_FMT"u", sbuf.st_size);
                  response = ftpcmd(buf, NULL, control_stream, buf);
		    switch (response) {
		    case 200:
		    case 202:
                  /* 某些FTP Server不识别ALLO命令*/
                  default:
			break;
            #if 0
		default:
			close(fd_local);
			ftp_die("ALLO", buf);
			break;
            #endif
                /*end of 某些FTP Server不识别ALLO命令 */
		    }
	     }
       }

	response = ftpcmd("STOR", server_path, control_stream, buf);
	switch (response)
	{
    	case 125:
    	case 150:
        {
          #if defined(SUPPORT_ATP_TR143_UPLOAD_TCP) || defined(SUPPORT_ATP_TR143_UPLOAD)
              if(TRUE == bDiagFlag)
              {
                    GetCurDateTime(pstDiagState->szBomTime, ATP_STRING_LEN_64);
                    ATP_UTIL_GetIfcStatInfo(pstDiagState->szIfName, &stIfcStats);
                    ulIfBytesBtimeSent = stIfcStats.ulBytesSent;
               }
           #endif
	    }
		break;
	default:
		close(fd_local);
        xfunc_error_retval = ATP_TRANS_FILE_ERR;
              SetUpDiagState(pstDiagState, "Error_NoSTOR", bDiagFlag);
		ftp_die("STOR", buf);
	}

	/* transfer the file  */
    /*Start of MNT 2008-10-13 15:9 for 传输超时检测 by z65940*/
#if defined(SUPPORT_ATP_TR143_UPLOAD_TCP) || defined(SUPPORT_ATP_TR143_UPLOAD)
    if(TRUE == bDiagFlag)
    {
        if(diag_copy_fd_with_timeout(fd_data,  atoi(local_path)) == -1)
        {
            SetUpDiagState(pstDiagState, "Error_NoTransferComplete", bDiagFlag);
            exit(ATP_TRANS_SYS_ERR);
        }
    }
    else
#endif
    {
      /*
        1082704676 2011-11-02 for by j00196195
        需要传入文件路径，因为文件句柄已经失效
        */
        /* 20120904 Modify by weiqingchuan 61642 for Compiling Warning */
        if (atp_copy_fd_with_timeout(fd_local, fd_data, 0, (char*)NULL) == -1)
        {
		    exit(ATP_TRANS_SYS_ERR);
        }
	}
	/* close it all down */
	close(fd_data);
	if (ftpcmd(NULL, NULL, control_stream, buf) != 226)
	{
        SetUpDiagState(pstDiagState, "Error_NoTransferComplete", bDiagFlag);
		ftp_die("close", buf);
	}
#if defined(SUPPORT_ATP_TR143_UPLOAD_TCP) || defined(SUPPORT_ATP_TR143_UPLOAD)
    if(TRUE == bDiagFlag)
    {
         GetCurDateTime(pstDiagState->szEomTime, ATP_STRING_LEN_64);
         ATP_UTIL_GetIfcStatInfo(pstDiagState->szIfName, &stIfcStats);
         ulIfBytesEtimeSent = stIfcStats.ulBytesSent;
         pstDiagState->ulTotalBytesSent = ulIfBytesEtimeSent - ulIfBytesBtimeSent;
         SetUpDiagState(pstDiagState, "Completed", bDiagFlag);
    }
 #endif
	ftpcmd("QUIT", NULL, control_stream, buf);

	return EXIT_SUCCESS;
}

#endif



/*Start of Maintain 2008-3-1 14:28 for 修改Busybox中的传输工具，满足cwmp传输需求 by z65940*/
#if 0
#define FTPGETPUT_OPT_CONTINUE	1
#define FTPGETPUT_OPT_VERBOSE	2
#define FTPGETPUT_OPT_USER	4
#define FTPGETPUT_OPT_PASSWORD	8
#define FTPGETPUT_OPT_PORT	16

#if ENABLE_FEATURE_FTPGETPUT_LONG_OPTIONS
static const char ftpgetput_longopts[] ALIGN1 =
	"continue\0" Required_argument "c"
	"verbose\0"  No_argument       "v"
	"username\0" Required_argument "u"
	"password\0" Required_argument "p"
	"port\0"     Required_argument "P"
	;
#endif
#else

#if ENABLE_FEATURE_ATP_FTPGETPUT_LONG_OPTIONS
static const char ftpgetput_longopts[] ALIGN1 =
	"continue\0" Required_argument "c"
	"verbose\0"  No_argument       "v"
	"username\0" Required_argument "u"
	"password\0" Required_argument "p"
	"port\0"     Required_argument "P"
	;
#endif
#endif
/*End of Maintain 2008-3-1 14:28 for 修改Busybox中的传输工具，满足cwmp传输需求 by z65940*/
/*
static void * thread_start(void *arg)
{
    arg = arg;
    while(1)
    {       
        int iRet = 0;
        iRet = system("echo 1 > /proc/sys/vm/drop_caches");
        sleep(5);
    }

    return NULL;
}

void create_thread(void)
{
    pthread_t   thread_pid;
    pthread_create(&thread_pid, NULL, &thread_start, NULL);
    return;
}
*/
int ftpgetput_main(int argc, char **argv);
int ftpgetput_main(int argc, char **argv)
{
	/* content-length of the file */
	unsigned opt;
	const char *port = "ftp";
	/* socket to ftp server */
	FILE *control_stream;
	/* continue previous transfer (-c) */
	ftp_host_info_t *server;
    /*Start of Maintain 2008-3-1 14:26 for 修改Busybox中的传输工具，满足cwmp传输需求 by z65940*/
    char *pcLocalFile = NULL;
    char *pcRemoteFile = NULL;
    char *pcPtr =NULL ;
    char *pcRemoteFileName =NULL ;
    char *pcRemoteIP =NULL ;
    /*End of Maintain 2008-3-1 14:26 for 修改Busybox中的传输工具，满足cwmp传输需求 by z65940*/
    const char *pcMaxTrans      = NULL;
    #ifdef ENABLE_FEATURE_IPV6
    int iResult = 0;
    char buffer[ATP_LEN_64] = {0};
    #endif
    tr143_diag_state_t  stDiagState;
    bool bDiagFlag = FALSE;
    memset(&stDiagState,0,sizeof(tr143_diag_state_t));
#if defined(SUPPORT_ATP_TR143_DOWNLOAD_TCP) ||defined(SUPPORT_ATP_TR143_DOWNLOAD) ||defined(SUPPORT_ATP_TR143_UPLOAD_TCP) || defined(SUPPORT_ATP_TR143_UPLOAD)
    int               mpid =0;
    char            *pcIfName;
    FILE            *pfDiagPid = NULL;
#endif
#if ENABLE_ATP_FTPGET && !CONFIG_ATP_FTPPUT
# define ftp_action ftp_send
#elif ENABLE_ATP_FTPGET && !CONFIG_ATP_FTPPUT
# define ftp_action ftp_receive
#else
	int (*ftp_action)(ftp_host_info_t *, FILE *, const char *, const char *,  tr143_diag_state_t  *, bool ) = ftp_send;
	/* Check to see if the command is ftpget or ftput */
	if (applet_name[3] == 'g') {
		ftp_action = ftp_receive;
	}
#endif

    /*Start of MNT 2008-10-13 14:39 for 传输超时检测 by z65940*/
    g_TimeoutCnt = -1;
    atp_setup_alarm();
    /*End of MNT 2008-10-13 14:39 for by z65940*/

	/* Set default values */
	server = xmalloc(sizeof(*server));
	server->user = "anonymous";
	server->password = "busybox@";

    g_pcLocalIP     = NULL;
    g_pcTransBegin  = NULL;
    g_pcTransEnd    = NULL;
    g_lTransEnd     = -1;
    g_lMaxTrans     = -1;
#if ENABLE_FEATURE_ATP_FTP_ZIP
    g_bCompress     = 0;
    #define ZIP_OPT_CHAR    "c"
#else
    #define ZIP_OPT_CHAR
#endif

	/*
	 * Decipher the command line
	 */
#if ENABLE_FEATURE_ATP_FTPGETPUT_LONG_OPTIONS
    /*Start of Maintain 2008-3-1 12:14 for 修改Busybox中的传输工具，满足cwmp传输需求 by z65940*/
#if 0
	applet_long_options = ftpgetput_longopts;
#else
	applet_long_options = bb_transtool_long_options;
#endif
    /*End of Maintain 2008-3-1 12:14 for 修改Busybox中的传输工具，满足cwmp传输需求 by z65940*/
#endif
    /*Start of Maintain 2008-3-1 12:16 for 修改Busybox中的传输工具，满足cwmp传输需求 by z65940*/
#if 0
    opt_complementary = "=3"; /* must have 3 params */
	opt = getopt32(argv, "cvu:p:P:", &server->user, &server->password, &port);
#else
    // Default error code
    xfunc_error_retval = ATP_TRANS_SYS_ERR;
    opt_complementary = "=1"; /* must have 1 params */
	//opt = getopt32(argv, "cvu:p:P:", &server->user, &server->password, &port);
 #if defined(SUPPORT_ATP_TR143_DOWNLOAD_TCP) ||defined(SUPPORT_ATP_TR143_DOWNLOAD) || defined(SUPPORT_ATP_TR143_UPLOAD_TCP) || defined(SUPPORT_ATP_TR143_UPLOAD)
    opt = getopt32(argv, "gsvu:p:r:l:P:B:A:b:e:m:"ZIP_OPT_CHAR"dI:",
                            &server->user,
                            &server->password,
                            &pcRemoteFile,
                            &pcLocalFile,
                            &port,
                            &g_pcLocalIP,
                            &pcRemoteIP,
                            &g_pcTransBegin,
                            &g_pcTransEnd,
                            &pcMaxTrans,
                            &pcIfName);
   if(0 == strcmp(argv[2],"-d") )
   {
        bDiagFlag = TRUE;
        snprintf(stDiagState.szIfName, sizeof(stDiagState.szIfName), "%s", pcIfName);
        mpid = (uint16_t) getpid();
   }
 #else
    opt = getopt32(argv, "gsvu:p:r:l:P:B:A:b:e:m:"ZIP_OPT_CHAR,
                            &server->user,
                            &server->password,
                            &pcRemoteFile,
                            &pcLocalFile,
                            &port,
                            &g_pcLocalIP,
                            &pcRemoteIP,
                            &g_pcTransBegin,
                            &g_pcTransEnd,
                            &pcMaxTrans);
 #endif
#endif

#undef ZIP_OPT_CHAR
    /*End of Maintain 2008-3-1 12:16 for 修改Busybox中的传输工具，满足cwmp传输需求 by z65940*/
	argv += optind;

#if ENABLE_FEATURE_ATP_FTP_ZIP
    if (opt & TRANS_TOOL_OPT_ZIP) {
        g_bCompress = 1;
    }
#endif

    if (NULL != pcMaxTrans)
    {
        g_lMaxTrans = atoi(pcMaxTrans);
    }
    
    if (NULL == pcLocalFile) {
        bb_show_usage();
    }

    // 有下载长度
    if (NULL != g_pcTransEnd) {
        // 只能为正数
        g_lTransEnd = atoi(g_pcTransEnd);
        if (g_lTransEnd <= 0)
                bb_show_usage();

        // 有下载起始位置，则计算下载结束位置
        if (NULL != g_pcTransBegin) {
            // 加上偏移量
            g_lTransEnd +=  atoi(g_pcTransBegin);
            g_lTransEnd -= 1;
        }
    }
    // 没有下载长度
    else {
        // 有下载开始位置，没有下载长度
        if (NULL != g_pcTransBegin) {   // Download until end
            g_lTransEnd = 0;
        }
    }

	/* Process the non-option command line arguments */
	if (opt & TRANS_TOOL_OPT_VERBOSE) {
		verbose_flag = 1;
	}
    /*create_thread();*/

	/* We want to do exactly _one_ DNS lookup, since some
	 * sites (i.e. ftp.us.debian.org) use round-robin DNS
	 * and we want to connect to only one IP... */
    /*Start of Maintain 2008-3-1 14:58 for 修改Busybox中的传输工具，满足cwmp传输需求 by z65940*/
#if 0
	server->lsa = xhost2sockaddr(argv[0], bb_lookup_port(port, "tcp", 21));
#else
    ftp_action = ftp_send;  // Upload
   #if defined(SUPPORT_ATP_TR143_UPLOAD_TCP) || defined(SUPPORT_ATP_TR143_UPLOAD)
    if(TRUE == bDiagFlag)
    {
        stDiagState.ulTransDir = TRANS_TOOL_OPT_UPLOAD;
        if ((pfDiagPid = fopen (TR143_UPDIAG_PID_FILE , "w")) != NULL)
        {
             fprintf(pfDiagPid,"%d\n", mpid);
         }
        (void)fclose(pfDiagPid);
    }
    #endif
    if (opt & TRANS_TOOL_OPT_DOWNLOAD) {
		ftp_action = ftp_receive;
    #if defined(SUPPORT_ATP_TR143_DOWNLOAD_TCP) ||defined(SUPPORT_ATP_TR143_DOWNLOAD)
              if(TRUE == bDiagFlag)
              {
                   stDiagState.ulTransDir = TRANS_TOOL_OPT_DOWNLOAD;
                   if ((pfDiagPid = fopen (TR143_DOWNDIAG_PID_FILE , "w")) != NULL)
                   {
                        fprintf(pfDiagPid,"%d\n", mpid);
                   }
                   (void)fclose(pfDiagPid);
              }
    #endif
	}
    if (!(opt & TRANS_TOOL_OPT_REMOTEIP)) {
		// User has not resolved the host, we need to use host to resolve name
		pcRemoteIP = argv[0];
	}
    #ifdef ENABLE_FEATURE_IPV6
    iResult = ATP_TransHostname2IP(pcRemoteIP,buffer,sizeof(buffer),&iFTPflag);
    if(0 == iResult)
    {
        printf("Please check your FTP URL! \n");
        /* 20120904 Add by weiqingchuan 61642 for Compiling Warning */
        return 0;
    }
    else
    {
        printf("The IP is [%s]\n",buffer);
        server->lsa = xhost2sockaddr(buffer, bb_lookup_port(port, "tcp", 21));
    }
    #else
    server->lsa = xhost2sockaddr(pcRemoteIP, bb_lookup_port(port, "tcp", 21));
    #endif
#endif
    /*End of Maintain 2008-3-1 14:58 for 修改Busybox中的传输工具，满足cwmp传输需求 by z65940*/
	if (verbose_flag) {
		printf("Connecting to %s (%s)\n", argv[0],
			xmalloc_sockaddr2dotted(&server->lsa->u.sa));
	}

	/*  Connect/Setup/Configure the FTP session */
    /*Start of 维护管理组 2008-4-15 16:55 for 修改ftp支持远程路径 by handy*/
#if 0
    control_stream = ftp_login(server);
#else
    /*去掉路径开头可能多余的/字符*/
    pcPtr = pcRemoteFile;
    while ('/' == (*pcPtr))
    {
        pcPtr++;
    }

    #if 0
    if ('/' == pcRemoteFile[0])
    {
        pcRemoteFile += 1;
    }
    #else
    if(pcPtr != pcRemoteFile)
    {
        pcRemoteFile = pcPtr;
    }
    #endif
    control_stream   = ftp_login(server, pcRemoteFile, &stDiagState, bDiagFlag);
    pcRemoteFileName = strrchr(pcRemoteFile, '/');
    if (NULL != pcRemoteFileName)
    {
        pcRemoteFile = (pcRemoteFileName + 1);
    }
    // 如果远程文件名是空串，则使用本地的文件名
    if ((NULL == pcRemoteFile) || ('\0' == (*pcRemoteFile))) {
        pcRemoteFile = strrchr(pcLocalFile, '/');
        if (NULL != pcRemoteFile) {
            pcRemoteFile += 1;
        } else {
            pcRemoteFile = pcLocalFile;
        }
    }
#endif
    /*End of 维护管理组 2008-4-15 16:55 for 修改ftp支持远程路径 by handy*/


    /*Start of Maintain 2008-3-1 14:49 for 修改Busybox中的传输工具，满足cwmp传输需求 by z65940*/
#if 0
	return ftp_action(server, control_stream, argv[1], argv[2]);
#else
    // Default return error code
    xfunc_error_retval = ATP_TRANS_SYS_ERR;

    
    return ftp_action(server, control_stream, pcLocalFile, pcRemoteFile, &stDiagState, bDiagFlag);
#endif
    /*End of Maintain 2008-3-1 14:49 for 修改Busybox中的传输工具，满足cwmp传输需求 by z65940*/
}
