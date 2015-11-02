/******************************************************************************

                  版权所有 (C), 2001-2014, 华为技术有限公司

 ******************************************************************************
  文 件 名   : dload_debug.c
  版 本 号   : 初稿
  作    者   : q00208481
  生成日期   : 2013年4月12日
  最近修改   :
  功能描述   :
  函数列表   :
  修改历史   :
  1.日    期   : 2013年4月12日
    作    者   : q00208481
    修改内容   : 创建文件
    
                                   
******************************************************************************/

#include    <stdio.h>
#include    <string.h>
#include    <errno.h>
#include    <stdarg.h>
#include    <sys/time.h>
#include    <sys/stat.h>
#include    <sys/types.h>
#include    <unistd.h>

#include    "dload_debug.h"
#include    "dload_comm.h"

#ifdef  __cplusplus
#if  __cplusplus
    extern "C"{
#endif
#endif

/***********************************************************************
  函数名称  :    dload_printf_to_file
  函数描述  :    输出log到sd卡或flash 
  函数入参  :    printf 格式
  函数输出  :    NA
  函数返回值:    DLOAD_OK: 函数成功
                 DLOAD_ERR: 函数失败
************************************************************************/
/*lint -e528*/
static int dload_printf_to_file(const char *path, int tv_sec, int tv_usec, const char *buf)
{
    FILE    *pFile = NULL;
    int     file_length = 0;
    char    filea_name[DLOAD_LOGREC_PATH_LENGTH] = {0};
    char    fileb_name[DLOAD_LOGREC_PATH_LENGTH] = {0};
    int     ret = 0;
    int     status = 0;

    ret = access(path, F_OK);
    if(ret)
    {
        /* 目录不存在，则创建目录 */
        status = mkdir(path, S_IRWXU);
        if(-1 == status)
        {
            printf("[DLOAD LOGREC] mkdir %s error.\n", path, strerror(errno));
            return DLOAD_ERR;
        }
    }

    /*获取file A长度*/
    /*lint -e119*/
    snprintf(filea_name, DLOAD_LOGREC_PATH_LENGTH, "%s%s", path, DLOAD_LOGREC_FILENAME_A);
    /*lint +e119*/
    pFile = fopen(filea_name, "a+");
    if (NULL == pFile)
    {
        printf("[DLOAD LOGREC] fopen %s, %s\n", filea_name, strerror(errno));
        return DLOAD_ERR;
    }

    ret = fseek(pFile, 0, SEEK_END);
    if(0 != ret)
    {
        fclose(pFile);

        printf("[DLOAD LOGREC] fseek %s, %s\n", filea_name, strerror(errno));
        return DLOAD_ERR;       
    }
    
    file_length = ftell(pFile);
    if(0 > file_length)
    {
        fclose(pFile);

        printf("[DLOAD LOGREC] Get length %s, %s\n", filea_name, strerror(errno));
        return DLOAD_ERR;
    }

    /*如果超长: A->B, new A*/
    if (DLOAD_LOGREC_MAX_FILESIZE <= file_length)
    {
        fclose(pFile);
        /*lint -e119*/
        snprintf(fileb_name, DLOAD_LOGREC_PATH_LENGTH, "%s%s", path, DLOAD_LOGREC_FILENAME_B);
        /*lint +e119*/
        ret = rename(filea_name, fileb_name);
        if (0 != ret)
        {
            printf("[DLOAD LOGREC] Rename %s to %s, %s\n", filea_name, fileb_name, strerror(errno));
            return DLOAD_ERR;
        }

        pFile = NULL;
        pFile = fopen(filea_name, "a+");
        if (NULL == pFile)
        {
            printf("[DLOAD LOGREC] Open %s, %s\n", filea_name, strerror(errno));
            return DLOAD_ERR;
        }
    }

    ret = fprintf(pFile, "[%d.%d]", tv_sec, tv_usec);
    if (0 > ret)
    {
        fclose(pFile);
        printf("[DLOAD LOGREC] fprintf %s, %s\n", filea_name, strerror(errno));
        return DLOAD_ERR;
    }

    ret = fprintf(pFile, "%s", buf);
    if (0 > ret)
    {
        fclose(pFile);

        printf("[DLOAD LOGREC] fvprintf %s, %s\n", filea_name, strerror(errno));
        return DLOAD_ERR;
    }

    fclose(pFile);
    return DLOAD_OK;
}

/*lint +e528*/
/***********************************************************************
  函数名称  :    dload_printf
  函数描述  :    输出log接口 
  函数入参  :    printf 格式
  函数输出  :    NA
  函数返回值:    DLOAD_OK: 函数成功
                 DLOAD_ERR: 函数失败
************************************************************************/
int dload_printf(int debuglev, const char *format, ...)
{
    /*lint -e530*/
    va_list para_list;
    /*lint +e530*/
    int     ret = 0;
    char    tmp_buf[DLOAD_LOGREC_LINE_LENGTH] = {0};
    struct timeval  now_time = {0};

    if (debuglev > DLOAD_DEBUG_LEVEL)
    {
        return DLOAD_OK;
    }
    /*lint -e528*/
#if ((defined MBB_DLOAD_LOGREC_TOCONSOLE) || (defined MBB_DLOAD_LOGREC_TOFLASH) || (defined MBB_DLOAD_LOGREC_TOMMC))
    gettimeofday(&now_time, NULL);

    /*lint -e530*/
    va_start(para_list, format);
    /*lint +e530*/
    /*lint -e119*/
    vsnprintf(tmp_buf, DLOAD_LOGREC_LINE_LENGTH, format, para_list);
    /*lint +e119*/
    va_end(para_list);
#endif

    /*lint -e528*/
#ifdef  MBB_DLOAD_LOGREC_TOCONSOLE
    printf("[%d.%d]%s", (int)now_time.tv_sec, (int)now_time.tv_usec, tmp_buf);
#endif
    
#ifdef  MBB_DLOAD_LOGREC_TOFLASH
    ret = dload_printf_to_file(MBB_DLOAD_FLASHLOG_PATH, now_time.tv_sec, now_time.tv_usec, tmp_buf);
#endif

#ifdef  MBB_DLOAD_LOGREC_TOMMC
    ret = dload_printf_to_file(MBB_DLOAD_MMCLOG_PATH, now_time.tv_sec, now_time.tv_usec, tmp_buf);
#endif

    return ret;
}

#ifdef  __cplusplus
#if  __cplusplus
    }
#endif
#endif
