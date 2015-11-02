/******************************************************************************

                  版权所有 (C), 2001-2014, 华为技术有限公司

 ******************************************************************************
  文 件 名   : dload_debug.h
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
#ifndef _HUAWEI_DLOAD_DEBUG_H_
#define _HUAWEI_DLOAD_DEBUG_H_


#ifdef  __cplusplus
#if  __cplusplus
    extern "C"{
#endif
#endif

#define     MBB_DLOAD_LOGREC_TOCONSOLE  (1)
#define     MBB_DLOAD_LOGREC_TOFLASH    (1)
//#define     MBB_DLOAD_LOGREC_TOMMC      (1)
#define     MBB_DLOAD_FLASHLOG_PATH     "/data/huawei_dload/"
#define     MBB_DLOAD_MMCLOG_PATH       "/mnt/sdcard/dload_log/"

#define     DLOAD_LOGREC_LINE_LENGTH    (2048)
#define     DLOAD_LOGREC_FILENAME_A     "dload_logA.txt"
#define     DLOAD_LOGREC_FILENAME_B     "dload_logB.txt"
#define     DLOAD_LOGREC_PATH_LENGTH    (128)
#define     DLOAD_LOGREC_MAX_FILESIZE   (128 * 1024)

enum dload_debug_lev
{
    DEBUG_LEV_CRIL,
    DEBUG_LEV_ERR,
    DEBUG_LEV_INFO
}debug_lev_e;

#define  DLOAD_DEBUG_LEVEL  (DEBUG_LEV_INFO)

/***********************************************************************
  函数名称  :    dload_printf
  函数描述  :    输出log接口 
  函数入参  :    printf 格式
  函数输出  :    NA
  函数返回值:    DLOAD_OK: 函数成功
                 DLOAD_ERR: 函数失败
************************************************************************/
int dload_printf(int debuglev, const char *format, ...);

#ifdef  __cplusplus
#if __cplusplus
    }
#endif
#endif

#endif /*_HUAWEI_DLOAD_FLASH_H_*/
