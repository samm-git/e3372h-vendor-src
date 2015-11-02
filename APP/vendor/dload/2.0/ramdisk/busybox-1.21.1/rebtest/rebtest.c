/******************************************************************************

                  版权所有 (C), 2013-2023, 华为技术有限公司

 ******************************************************************************
  文 件 名   : reb_test.c
  版 本 号   : 初稿
  作    者   :  qiaoyichuan   00206465
  生成日期   : 2013年11月15 日
  最近修改   :
  功能描述   : 可靠性测试接口
  函数列表   :
  修改历史   :
  1.日    期   : 2013年11月15 日
    作    者   : qiaoyichuan   00206465
    修改内容   : 创建文件
    2013.11.28     q00206465       DTS2013112803803       增加MTD写FLASH接口
    2013.11.30     q00206465       DTS2013120200913       增加MTD读FLASH接口
    2013.12.2      q00206465       DTS2013120201310       增加MTD获取分区信息接口   
    2013.12.2      q00206465       DTS2013120200993       增加正常释放内存的测试接口   
******************************************************************************/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <mtd/mtd-user.h>
#include <sys/param.h>

#include "libbb.h"

/******************************内部宏定义***********************************/
/*可靠性测试设备路径*/
#define     REB_TEST_FILE   "/dev/reb_test"
#define     LOG_BUF_LEN   (100)

#define     FILE_NAME_LENGTH        (128)

#define     REB_TEST_ERROR               (-1)
#define     REB_TEST_OK                       (0)

#define     FLASH_TEST_NUMBER     (0X55)
#define     STRTOL_BASE_10             (10)

/*参数个数*/
#define     REB_TEST_CMD_PARAM_MIN    (2)
#define     REB_TEST_CMD_PARAM_3         (3)
#define     REB_TEST_CMD_PARAM_4         (4)
#define     REB_TEST_CMD_PARAM_5         (5)
/*BEGIN DTS2013120200913 00206465 qiaoyichuan 2013-12-2 added*/
#define     REB_TEST_CMD_PARAM_6         (6)
/*END DTS2013120200913 00206465 qiaoyichuan 2013-12-2 added*/

/*BEGIN DTS2013120200993  00206465 qiaoyichuan 2013-11-30 modified*/
/*IOCTL 消息类型*/
#define     REB_MEM_DOUBLE_FREE          (0x0A)
#define     REB_MEM_ABNORMAL_FREE     (0x0B)
#define     REB_ABNORMAL_RESTART       (0x0C)
#define     REB_MEM_NORMAL_FREE         (0x0D)

#define     REB_MEM_DOUBLE_FREE_SET         (0)
#define     REB_MEM_ABNORMAL_FREE_SET   (1)
#define     REB_MEM_NORMAL_FREE_SET        (2)
/* END  DTS2013120200993  00206465 qiaoyichuan 2013-11-30 modified*/

/*可靠性测试类型*/
#define     COMMAND_TYPE_FILE_SYSTEM_PROTECT  (0)
#define     COMMAND_TYPE_FAST_POWEROFF    (1)
#define     COMMAND_TYPE_MEM_FREE      (2)
#define     COMMAND_TYPE_POWEROFF       (3)
#define     COMMAND_TYPE_NV_TEST     (4)
#define     COMMAND_TYPE_FLASH_WRITE_BEYOND   (5)
#define     COMMAND_TYPE_FLASH_ERASE          (6)
#define     COMMAND_TYPE_ABNORMAL_RESTART       (7)
/*BEGIN DTS2013112803803 00206465 qiaoyichuan 2013-11-28 added*/
#define     COMMAND_TYPE_FLASH_WRITE   (8)
/*BEGIN DTS2013120200913 00206465 qiaoyichuan 2013-11-30 added*/
#define     COMMAND_TYPE_FLASH_READ     (9)
/*BEGIN DTS2013120201310 00206465 qiaoyichuan 2013-12-2 added*/
#define     COMMAND_TYPE_GET_MTDINFO    (10)
#define     COMMAND_TYPE_INVIABLE           (11)
/* END  DTS2013120201310 00206465 qiaoyichuan 2013-12-2 added*/
/* END  DTS2013120200913 00206465 qiaoyichuan 2013-11-30 added*/
/* END  DTS2013112803803 00206465 qiaoyichuan 2013-11-28 added*/
#define     REB_TWO     (2)

/****************************函数原型声明************************************/
void reb_printf(char *func_name);
int reb_test_file_system_protect(void);
int reb_fast_poweroff_protect(bool lock);
int reb_mem_free_test(int func);
void reb_poweroff_test(void);
int reb_nv_test( unsigned int start_block, unsigned int end_block, char *mtd_name);
int reb_erase_block(unsigned int start_block, unsigned int end_block, char *mtd_name );
int reb_flash_write_beyond_test(char *mtd_name);
int reb_abnormal_restart(void);
/*BEGIN DTS2013112803803 00206465 qiaoyichuan 2013-11-28 added*/
int reb_flash_write_test( unsigned int start_block,
                    unsigned int end_block, char *mtd_name);
/* END  DTS2013112803803 00206465 qiaoyichuan 2013-11-28 added*/
/*BEGIN DTS2013120200913 00206465 qiaoyichuan 2013-11-30 added*/
int reb_read_block(unsigned int start_block, 
    unsigned int end_block, char *mtd_name, char *filename);
/* END  DTS2013120200913 00206465 qiaoyichuan 2013-11-30 added*/
/*BEGIN DTS2013120201310 00206465 qiaoyichuan 2013-12-2 added*/
int reb_mtd_get_info(char *mtd_name);
/* END  DTS2013120201310 00206465 qiaoyichuan 2013-12-2 added*/
int rebtest_main(int argc, char *argv[]);

/*******************************函数实现**************************************/

/*****************************************************************************
 函数名称  : int printf_usage(char *func_name)
 功能描述  : 提示用法
 输入参数  : func_name
 输出参数  : None
 返 回 值  : None
 修改历史  :
             1. 2013-11-16 :  00206465 qiaoyichuan created
*****************************************************************************/
void reb_printf(char *func_name)
{
    printf("Usage:\n");
    printf("file system protect test:       --> ");
    printf("%s %d\n", func_name, COMMAND_TYPE_FILE_SYSTEM_PROTECT);
    printf("fast poweroff test: --> ");
    printf("%s %d vote\n",
           func_name, COMMAND_TYPE_FAST_POWEROFF);
    printf("mem free test :      --> ");
    printf("%s %d function\n", func_name, COMMAND_TYPE_MEM_FREE);
    printf("poweroff test:       --> ");
    printf("%s %d \n", func_name, COMMAND_TYPE_POWEROFF);
    printf("nv recovery test:          --> ");
    printf("%s %d start_block end_block mtd_name\n",
           func_name, COMMAND_TYPE_NV_TEST);
    printf("flash write beyond test:    --> ");
    printf("%s %d   mtd_name\n",
           func_name, COMMAND_TYPE_FLASH_WRITE_BEYOND);
    printf("erase some area test: --> ");
    printf("%s %d start_block end_block mtd_name \n",
           func_name, COMMAND_TYPE_FLASH_ERASE);
    printf("abnormal restart test: --> ");
    printf("%s %d\n", func_name, COMMAND_TYPE_ABNORMAL_RESTART);
    /*BEGIN DTS2013112803803 00206465 qiaoyichuan 2013-11-28 added*/
    printf("write mtd  test: --> ");
    printf("%s %d start_block end_block mtd_name \n",
           func_name, COMMAND_TYPE_FLASH_WRITE);
    /* END  DTS2013112803803 00206465 qiaoyichuan 2013-11-28 added*/
    /*BEGIN DTS2013120200913 00206465 qiaoyichuan 2013-12-2 added*/
    printf("read mtd test: --> ");
    printf("%s %d start_block end_block mtd_name filename \n",
           func_name, COMMAND_TYPE_FLASH_READ);
    /* END  DTS2013120200913 00206465 qiaoyichuan 2013-12-2 added*/
    /*BEGIN DTS2013120201310 00206465 qiaoyichuan 2013-12-2 added*/
    printf("Get a partition info: --> ");
    printf("%s %d mtd_name;\n", 
        func_name, COMMAND_TYPE_GET_MTDINFO);
    /* END  DTS2013120201310 00206465 qiaoyichuan 2013-12-2 added*/
    return ;
}

/*BEGIN DTS2013120201310 00206465 qiaoyichuan 2013-12-2 added*/
/*****************************************************************************
 函数名称  : int reb_mtd_get_info(char *mtd_name)
 功能描述  : 获取分区信息
 输入参数  : mtd_name
 输出参数  : None
 返 回 值  :REB_TEST_ERROR
                         REB_TEST_OK
 修改历史  : 
             1. 2013-12-2 :  00206465 qiaoyichuan created
*****************************************************************************/
int reb_mtd_get_info(char *mtd_name)
{
    char    mtdfile[FILE_NAME_LENGTH] = {0};
    int     mtdfd = -1;
    mtd_info_t  info = {0};
    int     ret = 0;

    if (NULL == mtd_name)
    {
        printf("mtd_get_info param error\n");
        return REB_TEST_ERROR;
    }
    
    strcpy(mtdfile, "/dev/");
    strcat(mtdfile, mtd_name);

    mtdfd = open(mtdfile, O_RDONLY);
    if (mtdfd < 0)
    {
        printf("open %s error\n", mtdfile);
        return REB_TEST_ERROR;
    }
    /*获取分区信息*/
    ret = ioctl(mtdfd, MEMGETINFO, &info);
    if (0 != ret)
    {
        printf("get meminfo faild\n"); 
        close(mtdfd);
        return REB_TEST_ERROR;
    }

    close(mtdfd);
    /*打印分区信息*/
    printf("%s info: \n", mtd_name);
    printf("mtdsize: %d, erasesize: %d, writesize: %d, oobsize: %d\n", 
        info.size, info.erasesize, info.writesize, info.oobsize);
    printf("block number: [0 -- %d]\n", info.size / info.erasesize - 1);

    return REB_TEST_OK;
}
/* END  DTS2013120201310 00206465 qiaoyichuan 2013-12-2 added*/
/*****************************************************************************
 函数名称  : int reb_test_file_system_protect(void)
 功能描述  : 测试在文件写的时候，关机或者重启时
                               文件主动保护功能
 输入参数  : None
 输出参数  : None
 返 回 值  :   REB_TEST_ERROR
                            REB_TEST_OK
 修改历史  :
             1. 2013-11-13 :  00206465 qiaoyichuan created
*****************************************************************************/
int reb_test_file_system_protect(void)
{

    FILE* fd = NULL;
    char logTmpBuf[LOG_BUF_LEN] = {0};

    memset((void*)logTmpBuf, '*', LOG_BUF_LEN);

    /*若文件不存在，建立*/
    if ( NULL == (fd = fopen("/data/rebtest", "ab+")))
    {
        printf("Reb_DFT open file error\n");
        return REB_TEST_ERROR;
    }
    /*向文件中写并且不关闭*/
    if (strlen(logTmpBuf) != fwrite((const void *)logTmpBuf, 1,
                                    (size_t)strlen(logTmpBuf), (FILE *)fd))
    {
        printf("Reb_DFT write file error\n" );
        return REB_TEST_ERROR;
    }

    return REB_TEST_OK;
}

/*****************************************************************************
 函数名称  : int reb_fast_poweroff_protect(bool lock)
 功能描述  : 在假关机时，投票模拟假关机失败
 输入参数  : lock
 输出参数  : None
 返 回 值  :   REB_TEST_ERROR
                             REB_TEST_OK
 修改历史  :
             1. 2013-11-13 :  00206465 qiaoyichuan created
*****************************************************************************/
int reb_fast_poweroff_protect(bool lock)
{
    /*如果为1，投反对票，为0 投赞成票*/
    if (TRUE == lock)
    {
        system("echo reb_test > /sys/power/wake_lock");
        printf("Reb_DFT fast poweroff fail \n");
    }
    else if (FALSE == lock)
    {
        system("echo reb_test > /sys/power/wake_unlock");
        printf("Reb_DFT fast poweroff will success\n");
    }
    else
    {
        printf("Reb_DFT the argv is error\n");
        return REB_TEST_ERROR;
    }
    return REB_TEST_OK;
}

/*****************************************************************************
 函数名称  : int reb_mem_free_test(int func)
 功能描述  : 重复释放内存或者释放异常地址的内存
 输入参数  : func
 输出参数  : None
 返 回 值  : No
 修改历史  :
             1. 2013-11-13 :  00206465 qiaoyichuan created
*****************************************************************************/
int reb_mem_free_test(int func)
{
    int fd = REB_TEST_ERROR;
    int ret = REB_TEST_OK;

    /*打开字符设备*/
    fd = open(REB_TEST_FILE, O_RDWR);
    if (fd < 0)
    {
        printf("Reb_DFT open reb test moudle fail \n");
        return REB_TEST_ERROR;
    }
    switch (func)
    {
            /*重复释放内存*/
        case REB_MEM_DOUBLE_FREE_SET:
        {
            ret = ioctl(fd, REB_MEM_DOUBLE_FREE, 0);
            if (0 != ret)
            {
                printf("Reb_DFT mem double free ioctl fail \n");
                close(fd);
                return REB_TEST_ERROR;
            }
            break;
        }
        /*释放异常内存*/
        case REB_MEM_ABNORMAL_FREE_SET:
        {
            ret = ioctl(fd, REB_MEM_ABNORMAL_FREE, 0);
            if (0 != ret)
            {
                printf("Reb_DFT mem abnormal ioctl fail \n");
                close(fd);
                return REB_TEST_ERROR;
            }
            break;
        }
        /*BEGIN DTS2013120200993  00206465 qiaoyichuan 2013-11-30 added*/
        /*正常释放内存*/
        case REB_MEM_NORMAL_FREE_SET:
        {
            ret = ioctl(fd, REB_MEM_NORMAL_FREE, 0);
            if (0 != ret)
            {
                printf("Reb_DFT mem normal ioctl fail \n");
                close(fd);
                return REB_TEST_ERROR;
            }
            break;
        }
        /* END  DTS2013120200993  00206465 qiaoyichuan 2013-11-30 added*/

        default:
            break;
    }
    close(fd);
    return REB_TEST_OK;
}



/*****************************************************************************
 函数名称  : int reb_poweroff_test(void)
 功能描述  : 关机过程中异常，测试是否还能关机
 输入参数  : None
 输出参数  : None
 返 回 值  : No
 修改历史  :
             1. 2013-11-13 :  00206465 qiaoyichuan created
*****************************************************************************/
void reb_poweroff_test(void)
{
    /*关机过程中，删除appvcom,模拟无法关机*/
    printf("Reb_DFT reb_poweroff_test\n");
    system("rm /dev/appvcom1");
    system("rm /dev/appvcom");
}

/*****************************************************************************
 函数名称  : int reb_nv_test( unsigned int start_block,unsigned int end_block,char *mtd_name)
 功能描述  : 标记NV分区某些块为坏块，测试NV分区恢复机制
 输入参数  : mtd_name  start_block end_block
 输出参数  : No
 返 回 值  : REB_TEST_ERROR
                          REB_TEST_OK
 修改历史  :
             1. 2013-11-14 :  00206465 qiaoyichuan created
*****************************************************************************/
int reb_nv_test( unsigned int start_block, unsigned int end_block, char *mtd_name)
{
    char    mtdfile[FILE_NAME_LENGTH] = {0};
    int     mtdfd = REB_TEST_ERROR;
    int     ret = REB_TEST_OK;

    unsigned int    curblock = start_block;
    unsigned long long  curaddr = 0;
    mtd_info_t  info = {0};

    /*入参判断*/
    if (NULL == mtd_name || start_block > end_block)
    {
        printf("Reb_DFT mtd_mark_badblock param error\n");
        return REB_TEST_ERROR;
    }

    strcpy(mtdfile, "/dev/");
    strcat(mtdfile, mtd_name);

    /*打开对应的MTD 设备*/
    mtdfd = open(mtdfile, O_RDONLY);
    if (mtdfd < 0)
    {
        printf("Reb_DFT open %s error\n", mtdfile);
        return REB_TEST_ERROR;
    }

    /*获取所写分区信息*/
    ret = ioctl(mtdfd, MEMGETINFO, &info);
    if (0 != ret)
    {
        printf("Reb_DFT get meminfo faild\n");
        close(mtdfd);
        return REB_TEST_ERROR;
    }
    /* 判断所输入的block是否超出分区范围 */
    if (end_block * info.erasesize >= info.size)
    {
        printf("Reb_DFT block number %d over, max %d\n",
               end_block, info.size / info.erasesize - 1);
        return REB_TEST_ERROR;
    }

    /* 将输入的块标记为坏块*/
    while (curblock <= end_block)
    {
        curaddr = (unsigned long long)curblock * (unsigned long long)info.erasesize;
        ret = ioctl(mtdfd, MEMSETBADBLOCK, &curaddr);
        if (ret < 0)
        {
            printf("Reb_DFT GET MEMGETBADBLOCK error, offset: %lld\n", curaddr);
            close(mtdfd);
            return REB_TEST_ERROR;
        }

        curblock++;
    }

    return REB_TEST_OK;
}


/*****************************************************************************
 函数名称  : int reb_erase_block(char *mtd_name, unsigned int start_block,
 功能描述  :
 输入参数  : No
 输出参数  : No
 返 回 值  : No
 修改历史  :
             1. 2013-11-15 :  00206465 qiaoyichuan created
*****************************************************************************/
int reb_erase_block(unsigned int start_block, unsigned int end_block, char *mtd_name )
{
    char    mtdfile[FILE_NAME_LENGTH] = {0};
    int     mtdfd = REB_TEST_ERROR;
    int     ret = REB_TEST_OK;

    unsigned int    curblock = start_block;
    unsigned long long  curaddr = 0;
    struct erase_info_user erase_info;
    mtd_info_t  info = {0};

    /*入参判断*/
    if (NULL == mtd_name || start_block > end_block)
    {
        printf("Reb_DFT mtd_erase_block param error\n");
        return REB_TEST_ERROR;
    }

    strcpy(mtdfile, "/dev/");
    strcat(mtdfile, mtd_name);

    /*打开对应的MTD 设备*/
    mtdfd = open(mtdfile, O_RDWR);
    if (mtdfd < 0)
    {
        printf("Reb_DFT open %s error\n", mtdfile);
        return REB_TEST_ERROR;
    }

    /*获取所写分区信息*/
    ret = ioctl(mtdfd, MEMGETINFO, &info);
    if (0 != ret)
    {
        printf("Reb_DFT get meminfo faild\n");
        close(mtdfd);
        return REB_TEST_ERROR;
    }

    /* 判断所输入的block是否超出分区范围 */
    if (end_block * info.erasesize >= info.size)
    {
        printf("Reb_DFT block number %d over, max %d\n",
               end_block, info.size / info.erasesize - 1);
        return REB_TEST_ERROR;
    }

    /* 将输入的块擦除*/
    while (curblock <= end_block)
    {
        printf("Reb_DFT erase %d block \n", curblock);
        curaddr = (unsigned long long)curblock * (unsigned long long)info.erasesize;
        erase_info.start = curaddr;
        erase_info.length = info.erasesize;
        ret = ioctl(mtdfd, MEMERASE, &erase_info);
        if (ret < 0)
        {
            printf("Reb_DFT erase %d block error\n", curblock);
            close(mtdfd);
            return REB_TEST_ERROR;
        }
        curblock++;
    }

    return REB_TEST_OK;
}



/*****************************************************************************
 函数名称  : int reb_flash_write_beyond_test( unsigned int start_block, unsigned int end_block,
                                 char *mtd_name, )
 功能描述  : 指定分区指定BLOCK指定写
 输入参数  : start_block  end_block mtd_name
 输出参数  : No
 返 回 值  : No
 修改历史  :
             1. 2013-11-15 :  00206465 qiaoyichuan created
*****************************************************************************/
int reb_flash_write_beyond_test(char *mtd_name)
{
    char    mtdfile[FILE_NAME_LENGTH] = {0};
    int     mtdfd = REB_TEST_ERROR;
    mtd_info_t  info = {0};
    int     ret = REB_TEST_OK;
    char    *databuf = NULL;
    int     blocksize = 0;
    int     end = 0;
    unsigned long long  curaddr = 0;
    unsigned int    curblock = 0;
    unsigned int    startblock = 0;
    struct erase_info_user erase_info;

    /*入参判断*/
    if (NULL == mtd_name)
    {
        printf("Reb_DFT mtd_write_block param error\n");
        return REB_TEST_ERROR;
    }

    strcpy(mtdfile, "/dev/");
    strcat(mtdfile, mtd_name);

    /*打开对应的MTD 设备*/
    mtdfd = open(mtdfile, O_RDWR);
    if (mtdfd < 0)
    {
        printf("Reb_DFT open %s error\n", mtdfile);
        return REB_TEST_ERROR;
    }

    /*获取所写分区信息*/
    ret = ioctl(mtdfd, MEMGETINFO, &info);
    if (0 != ret)
    {
        printf("Reb_DFT get meminfo faild\n");
        close(mtdfd);
        return REB_TEST_ERROR;
    }

    blocksize = info.erasesize;

    /* 申请2个blocksize大小的内存 */
    databuf = malloc(REB_TWO * blocksize);
    if (NULL == databuf)
    {
        printf("Reb_DFT databuf malloc error\n");
        close(mtdfd);
        return REB_TEST_ERROR;
    }
    memset(databuf, FLASH_TEST_NUMBER, REB_TWO * blocksize);

    /* 计算起始和结束的block*/
    curblock = info.size / info.erasesize - 1;
    end = curblock + 1;

    startblock = curblock;
    /* 写数据到指定的block中*/
    while (curblock != end)
    {
        /*先擦后写*/
        curaddr = (unsigned long long)curblock * (unsigned long long)info.erasesize;
        erase_info.start = curaddr;
        erase_info.length = info.erasesize;
        ret = ioctl(mtdfd, MEMERASE, &erase_info);
        if (ret < 0)
        {
            /*擦除失败，标记为坏块*/
            ret = ioctl(mtdfd, MEMSETBADBLOCK, &curaddr);
            if (ret < 0)
            {
                printf("GET MEMSETBADBLOCK error, offset: %lld\n", curaddr);
                free(databuf);
                close(mtdfd);
                return REB_TEST_ERROR;
            }
        }
        printf("Block %d Erasing...\n", curblock);
        curblock++;
    }

    /*向分区中写两个块大小的内容*/
    ret = pwrite(mtdfd, databuf, REB_TWO * blocksize, startblock * info.erasesize);
    if (ret != blocksize)
    {
        printf("Reb_DFT pwrite error\n");
        free(databuf);
        close(mtdfd);
        return REB_TEST_ERROR;
    }
    printf("Reb_DFT Block %d Write 0x%x\n", startblock, databuf[0]);

    free(databuf);
    close(mtdfd);

    return REB_TEST_OK;

}
/*BEGIN DTS2013112803803 00206465 qiaoyichuan 2013-11-28 added*/
/*****************************************************************************
 函数名称  : int reb_flash_write_test( unsigned int start_block, unsigned int end_block,
                                 char *mtd_name, )
 功能描述  : 指定分区指定BLOCK指定写
 输入参数  : start_block  end_block mtd_name
 输出参数  : No
 返 回 值  : No
 修改历史  :
             1. 2013-11-15 :  00206465 qiaoyichuan created
*****************************************************************************/
int reb_flash_write_test( unsigned int start_block,
                    unsigned int end_block, char *mtd_name)
{
    char    mtdfile[FILE_NAME_LENGTH] = {0};
    int     mtdfd = REB_TEST_ERROR;
    mtd_info_t  info = {0};
    int     ret = REB_TEST_OK;
    char    *databuf = NULL;
    int     blocksize = 0;
    int     end = 0;
    unsigned long long  curaddr = 0;
    unsigned int    curblock = 0;
    struct erase_info_user erase_info;

    /*入参判断*/
    if (NULL == mtd_name)
    {
        printf("Reb_DFT mtd_write_block param error\n");
        return REB_TEST_ERROR;
    }

    strcpy(mtdfile, "/dev/");
    strcat(mtdfile, mtd_name);

    /*打开对应的MTD 设备*/
    mtdfd = open(mtdfile, O_RDWR);
    if (mtdfd < 0)
    {
        printf("Reb_DFT open %s error\n", mtdfile);
        return REB_TEST_ERROR;
    }

    /*获取所写分区信息*/
    ret = ioctl(mtdfd, MEMGETINFO, &info);
    if (0 != ret)
    {
        printf("Reb_DFT get meminfo faild\n");
        close(mtdfd);
        return REB_TEST_ERROR;
    }

    /* 判断所输入的block是否超出分区范围 */
    if (end_block * info.erasesize >= info.size)
    {
        printf("Reb_DFT block number %d over, max %d\n",
               end_block, info.size / info.erasesize - 1);
        return REB_TEST_ERROR;
    }

    blocksize = info.erasesize;

    /* 申请1 个blocksize大小的内存 */
    databuf = malloc( blocksize);
    if (NULL == databuf)
    {
        printf("Reb_DFT databuf malloc error\n");
        close(mtdfd);
        return REB_TEST_ERROR;
    }
    memset(databuf, FLASH_TEST_NUMBER,  blocksize);

    /* 计算起始和结束的block*/
    curblock = start_block;
    end = end_block + 1;

    /* 写数据到指定的block中*/
    while (curblock != end)
    {
        /*先擦后写*/
        curaddr = (unsigned long long)curblock * (unsigned long long)info.erasesize;
        erase_info.start = curaddr;
        erase_info.length = info.erasesize;
        ret = ioctl(mtdfd, MEMERASE, &erase_info);
        if (ret < 0)
        {
            /*擦除失败，标记为坏块*/
            ret = ioctl(mtdfd, MEMSETBADBLOCK, &curaddr);
            if (ret < 0)
            {
                printf("GET MEMSETBADBLOCK error, offset: %lld\n", curaddr);
                free(databuf);
                close(mtdfd);
                return REB_TEST_ERROR;
            }
        }
        printf("Block %d Erasing...\n", curblock);

        /*向分区中写一个块大小的内容*/
        ret = pwrite(mtdfd, databuf, blocksize, curblock * info.erasesize);
        if (ret != blocksize)
        {
            printf("Reb_DFT pwrite error\n");
            free(databuf);
            close(mtdfd);
            return REB_TEST_ERROR;
        }
        printf("Reb_DFT Block %d Write 0x%x\n", curblock, databuf[0]);
        curblock++;
    }
    free(databuf);
    close(mtdfd);

    return REB_TEST_OK;
}
/* END  DTS2013112803803 00206465 qiaoyichuan 2013-11-28 added*/

/*****************************************************************************
 函数名称  : void reb_abnormal_restart(void)
 功能描述  :
 输入参数  : No
 输出参数  : No
 返 回 值  : No
 修改历史  :
             1. 2013-11-19 :  00206465 qiaoyichuan created
*****************************************************************************/
int reb_abnormal_restart(void)
{
    int fd = REB_TEST_ERROR;
    int ret = REB_TEST_OK;

    /*打开字符设备*/
    fd = open(REB_TEST_FILE, O_RDWR);
    if (fd < 0)
    {
        printf("Reb_DFT open reb test moudle fail \n");
        return REB_TEST_ERROR;
    }

    /*下发异常重启命令*/
    ret = ioctl(fd, REB_ABNORMAL_RESTART, 0);
    if (0 != ret)
    {
        printf("Reb_DFT abnormal restart ioctl fail \n");
        close(fd);
        return REB_TEST_ERROR;
    }
    close(fd);
    return REB_TEST_OK;
}

/*BEGIN DTS2013120200913 00206465 qiaoyichuan 2013-12-2 added*/
/*****************************************************************************
 函数名称  : int reb_read_block(unsigned int start_block, unsigned int end_block,
                                        char *mtd_name, char *filename)
 功能描述  : 读取FLASH内容到文件中
 输入参数  : start_block end_block  mtd_name  filename
 输出参数  : No
 返 回 值  : REB_TEST_OK  REB_TEST_ERROR
 修改历史  : 
             1. 2013-11-29 :  00206465 qiaoyichuan created
*****************************************************************************/
int reb_read_block(unsigned int start_block, 
    unsigned int end_block, char *mtd_name, char *filename)
{
    char    mtdfile[FILE_NAME_LENGTH] = {0};
    int     mtdfd = -1;
    int     ret = 0;
    char    *databuf = NULL;
    int     blocksize = 0;
    FILE    *destfp = NULL;
    
    unsigned int    curblock = start_block;
    mtd_info_t  info = {0};

    /*入参判断*/
    if (NULL == mtd_name || NULL == filename
        || start_block > end_block)
    {
        printf("Reb_DFT mtd_read_block param error\n");
        return REB_TEST_ERROR;
    }

    strcpy(mtdfile, "/dev/");
    strcat(mtdfile, mtd_name);
    /*打开MTD设备*/
    mtdfd = open(mtdfile, O_RDONLY);
    if (mtdfd < 0)
    {
        printf("Reb_DFT open %s error\n", mtdfile);
        return REB_TEST_ERROR;
    }
    /*获取分区信息*/
    ret = ioctl(mtdfd, MEMGETINFO, &info);
    if (0 != ret)
    {
        printf("Reb_DFT get meminfo faild\n"); 
        close(mtdfd);
        return REB_TEST_ERROR;
    }
    /*判断是否超出分区范围*/
    if (end_block * info.erasesize >= info.size)
    {
        printf("Reb_DFT block number %d over, max %d\n", 
            end_block, info.size / info.erasesize - 1);
        return REB_TEST_ERROR;
    }


    destfp = fopen(filename, "w");
    if (NULL == destfp)
    {
        printf("Reb_DFT fopen error\n");
        close(mtdfd);
        return REB_TEST_ERROR;
    }

    blocksize = info.erasesize;
    databuf = malloc(blocksize);
    if (NULL == databuf)
    {
        printf("Reb_DFT read block malloc error\n");
        close(mtdfd);
        fclose(destfp);
        return REB_TEST_ERROR;
    }
    /*读取FLASH某些block的内容到指定文件中*/
    while(curblock <= end_block)
    {      
        /*lint -e119*/
        ret = pread(mtdfd, databuf, blocksize, curblock * info.erasesize);
        if (ret != blocksize)
        {
            printf("Reb_DFT read error\n");
            goto out;
        }
        /*lint +e119*/
        ret = fwrite(databuf, 1, blocksize, destfp);
        if (ret != blocksize)
        {
            printf("Reb_DFT fwrite error\n");
            goto out;
        }
        curblock++;
    }

out:
    free(databuf);
    close(mtdfd);
    fclose(destfp);

    return REB_TEST_OK;
}
/* END  DTS2013120200913 00206465 qiaoyichuan 2013-12-2 added*/
/*****************************************************************************
*Function:         rebtest_main
*
*Description:      flash test main function
*Input:            argc:
*                  argv:
*
*Output:           NA
*Return:           REB_TEST_ERROR:  failure
*                  REB_TEST_OK:     success
*
*Others:           REB_TEST_OK
*******************************************************************************/
int rebtest_main(int argc, char *argv[])
{
    int             ret = REB_TEST_OK;
    int             comtype = COMMAND_TYPE_INVIABLE;
    int             cmd_para = 0;
    int             cmd_para1 = 0;
    int             cmd_para2 = 0;

    /*入参个数判断*/
    if (REB_TEST_CMD_PARAM_MIN > argc)
    {
        reb_printf(argv[0]);
        return REB_TEST_ERROR;
    }

    /*入参处理*/
    cmd_para = strtol(argv[1], NULL, STRTOL_BASE_10);
    if (LONG_MIN == cmd_para
            || LONG_MAX == cmd_para)
    {
        printf("Reb_DFT param %s error\n", argv[1]);
        return REB_TEST_ERROR;
    }
    comtype = cmd_para;

    if (REB_TEST_CMD_PARAM_3 == argc)
    {
        cmd_para1 = strtol(argv[REB_TEST_CMD_PARAM_MIN], NULL, STRTOL_BASE_10);
        if (LONG_MIN == cmd_para1 || LONG_MAX == cmd_para1)
        {
            printf("Reb_DFT param1 %s error\n", argv[REB_TEST_CMD_PARAM_MIN]);
            return REB_TEST_ERROR;
        }
    }
    else if (REB_TEST_CMD_PARAM_4 <= argc)
    {
        cmd_para1 = strtol(argv[REB_TEST_CMD_PARAM_MIN], NULL, STRTOL_BASE_10);
        if (LONG_MIN == cmd_para1 || LONG_MAX == cmd_para1)
        {
            printf("Reb_DFT param1 %s error\n", argv[REB_TEST_CMD_PARAM_MIN]);
            return REB_TEST_ERROR;
        }
        cmd_para2 = strtol(argv[REB_TEST_CMD_PARAM_3], NULL, STRTOL_BASE_10);
        if (LONG_MIN == cmd_para2 || LONG_MAX == cmd_para2)
        {
            printf("Reb_DFT param2 %s error\n", argv[REB_TEST_CMD_PARAM_3]);
            return REB_TEST_ERROR;
        }
    }
    else
    {
        //donothing
    }

    switch (comtype)
    {
            /*关机文件系统保护*/
        case (COMMAND_TYPE_FILE_SYSTEM_PROTECT):
        {
            if (argc != REB_TEST_CMD_PARAM_MIN)
            {
                reb_printf(argv[0]);
                return REB_TEST_ERROR;
            }
            ret = reb_test_file_system_protect();
        }
        break;

        /*假关机失败进入真关机*/
        case (COMMAND_TYPE_FAST_POWEROFF):
        {
            if (argc != REB_TEST_CMD_PARAM_3)
            {
                reb_printf(argv[0]);
                return REB_TEST_ERROR;
            }
            ret = reb_fast_poweroff_protect(cmd_para1);
        }
        break;

        /*内存重复释放或异常释放*/
        case (COMMAND_TYPE_MEM_FREE):
        {
            if (argc != REB_TEST_CMD_PARAM_3)
            {
                reb_printf(argv[0]);
                return REB_TEST_ERROR;
            }
            ret = reb_mem_free_test(cmd_para1);
        }
        break;

        /* 关机可靠性测试*/
        case (COMMAND_TYPE_POWEROFF):
        {
            if (argc != REB_TEST_CMD_PARAM_MIN)
            {
                reb_printf(argv[0]);
                return REB_TEST_ERROR;
            }
            reb_poweroff_test();
        }
        break;

        /* NV 恢复测试*/
        case (COMMAND_TYPE_NV_TEST):
        {
            if (argc != REB_TEST_CMD_PARAM_5)
            {
                reb_printf(argv[0]);
                return REB_TEST_ERROR;
            }
            ret = reb_nv_test(cmd_para1, cmd_para2, argv[REB_TEST_CMD_PARAM_4]);
        }
        break;

        /* 分区写越界测试*/
        case (COMMAND_TYPE_FLASH_WRITE_BEYOND):
        {
            if (argc != REB_TEST_CMD_PARAM_3)
            {
                reb_printf(argv[0]);
                return REB_TEST_ERROR;
            }
            ret = reb_flash_write_beyond_test(argv[REB_TEST_CMD_PARAM_MIN]);
        }
        break;

        /* 分区擦除接口*/
        case (COMMAND_TYPE_FLASH_ERASE):
        {
            if (argc != REB_TEST_CMD_PARAM_5)
            {
                reb_printf(argv[0]);
                return REB_TEST_ERROR;
            }
            ret = reb_erase_block( cmd_para1, cmd_para2, argv[REB_TEST_CMD_PARAM_4]);
        }
        break;

        /* 异常重启测试*/
        case (COMMAND_TYPE_ABNORMAL_RESTART):
        {
            if (argc != REB_TEST_CMD_PARAM_MIN)
            {
                reb_printf(argv[0]);
                return REB_TEST_ERROR;
            }
            ret = reb_abnormal_restart();
        }
        break;

        /*BEGIN DTS2013112803803 00206465 qiaoyichuan 2013-11-28 added*/
        /* 分区写接口*/
        case (COMMAND_TYPE_FLASH_WRITE):
        {
            if (argc != REB_TEST_CMD_PARAM_5)
            {
                reb_printf(argv[0]);
                return REB_TEST_ERROR;
            }
            ret = reb_flash_write_test( cmd_para1, cmd_para2, argv[REB_TEST_CMD_PARAM_4]);
        }
        break;
        /* END  DTS2013112803803 00206465 qiaoyichuan 2013-11-28 added*/
        /*BEGIN DTS2013120200913 00206465 qiaoyichuan 2013-12-2 added*/
        /* 分区读接口*/
        case (COMMAND_TYPE_FLASH_READ):
        {
            if (argc != REB_TEST_CMD_PARAM_6)
            {
                reb_printf(argv[0]);
                return REB_TEST_ERROR;
            }
            ret = reb_read_block( cmd_para1, cmd_para2, 
                    argv[REB_TEST_CMD_PARAM_4],argv[REB_TEST_CMD_PARAM_5]);
        }
        break;
        /* END  DTS2013120200913 00206465 qiaoyichuan 2013-12-2 added*/

        /*BEGIN DTS2013120201310 00206465 qiaoyichuan 2013-12-2 added*/
        case (COMMAND_TYPE_GET_MTDINFO):
        {
            if (argc != REB_TEST_CMD_PARAM_3)
            {
                reb_printf(argv[0]);
                return REB_TEST_ERROR;
            }
            ret = reb_mtd_get_info(argv[REB_TEST_CMD_PARAM_MIN]);
        }
        break;
        /* END  DTS2013120201310 00206465 qiaoyichuan 2013-12-2 added*/

        default:
        {
            printf("Reb_DFT command type error\n");
            return REB_TEST_ERROR;
        }
    }

    if (0 != ret)
    {
        printf("Reb_DFT COMMAND EXECUTE ERROR\n");
    }

    return ret;

}

