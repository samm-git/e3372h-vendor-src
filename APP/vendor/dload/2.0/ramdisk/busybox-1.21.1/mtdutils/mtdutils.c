/******************************************************************************

                  版权所有 (C), 2001-2014, 华为技术有限公司

 ******************************************************************************
  文 件 名   : mtdutils.c
  版 本 号   : 初稿
  作    者   : q00208481
  生成日期   : 2013年05月18日
  最近修改   :
  功能描述   : Flash压力测试接口
  函数列表   :
  修改历史   :
  1.日    期   : 2013年05月18日
    作    者   : q00208481
    修改内容   : 创建文件
     问题单号                       时间                   修改人                        说明
DTS2013122503592             2013-12-25                m00230250         优化分区表发生变化应用层升级代码与底层mtd设备驱动针对yaffs分区的处理
DTS2014090106291             2014-09-02                c00227249         同步升级归一化修改                                   
******************************************************************************/
/*BEGIN DTS2013052007435 QIUJIAN 00208481 2013-05-20 ADDED*/
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
#include "dload_ioctl.h"

#define     FLASH_CODEWORD_SHIFT    (9)
#define     FLASH_CW_OOBSIZE_4096   (20)
#define     FLASH_CW_OOBSIZE_2048   (16)

#define     FILE_NAME_LENGTH        (128)
#define     FILE_DATA_LENGTH        (512)

#define     MTD_ERROR               (-1)
#define     MTD_OK                  (0)

#define     FLASH_TEST_NUMBER_A     (0X55)
#define     FLASH_TEST_NUMBER_B     (0XAA)
#define     STRTOL_BASE_10          (10)
#define     WRITE_TURN_MOD          (2)

#define     MTDUTILS_CMD_PARAM_MIN  (2)
#define     MTDUTILS_CMD_PARAM_3    (3)
#define     MTDUTILS_CMD_PARAM_4    (4)
#define     MTDUTILS_CMD_PARAM_5    (5)
#define     MTDUTILS_CMD_PARAM_6    (6)

#define     FLASH_PAGE_SIZE_2048    (2048)
#define     FLASH_PAGE_SIZE_4096    (4096)


#define     COMMAND_TYPE_GET_FLASHINFO  (0)
#define     COMMAND_TYPE_GET_MTDINFO    (1)
#define     COMMAND_TYPE_QUERY_BAD      (2)
#define     COMMAND_TYPE_MARK_BAD       (3)
#define     COMMAND_TYPE_ERASE_DATA     (4)
#define     COMMAND_TYPE_ERASE_FORCE_DATA   (5)
#define     COMMAND_TYPE_READ_DATA          (6)
#define     COMMAND_TYPE_WRITE_DATA         (7)
#define     COMMAND_TYPE_INVIABLE           (8)

#define     MTD_NAME_LENGTH                 (3)

#define     OOB_SIZE_USE                    (16)

typedef enum
{
    MTD_UNKNOW_FS,
    MTD_NO_FS,    
    MTD_YAFFS2_FS,
    MTD_JIFFS2_FS,
    MTD_FAT32_FS,
    MTD_MAX_FS
}mtd_fs_type;

/*****************************************************************************
*Function:         printf_usage
*
*Description:      print usage
*Input:            func_name: this function name
*Output:           NA
*Return:           0:  not yaffs
*                  1:  is yaffs
*
*Others:           
*******************************************************************************/
int printf_usage(char *func_name)
{
    printf("Usage:\n");
    printf("Get flash info:       --> ");
    printf("%s %d;\n", func_name, COMMAND_TYPE_GET_FLASHINFO);
    printf("Get a partition info: --> ");
    printf("%s %d mtd_name;\n", 
        func_name, COMMAND_TYPE_GET_MTDINFO);
    printf("Query bad block:      --> ");
    printf("%s %d mtd_name start_block end_block;\n", 
        func_name, COMMAND_TYPE_QUERY_BAD);
    printf("Mark bad block:       --> ");
    printf("%s %d mtd_name start_blcok end_block;\n", 
        func_name, COMMAND_TYPE_MARK_BAD);
    printf("Erase block:          --> ");
    printf("%s %d mtd_name start_block end_block;\n", 
        func_name, COMMAND_TYPE_ERASE_DATA);
    printf("Erase block force:    --> ");
    printf("%s %d mtd_name start_block end_block;\n", 
        func_name, COMMAND_TYPE_ERASE_FORCE_DATA);
    printf("Read data from flash: --> ");
    printf("%s %d mtd_name start_block end_block save_file;\n", 
        func_name, COMMAND_TYPE_READ_DATA);
    printf("Write data to flash:  --> ");
    printf("%s %d mtd_name start_block end_block write_times;\n", 
        func_name, COMMAND_TYPE_WRITE_DATA);

    return 0;
}

/*******************************************************************
  函数名称  : dload_mtd_fs_type
  函数描述  : 获取平台信息
  函数入参  : mtd_name : mtd名称
  函数输出  : NA
  函数返回值: mtd_fs_type : 文件系统类型
********************************************************************/
STATIC mtd_fs_type dload_mtd_fs_type(char *mtd_name)
{
    mtd_fs_type fs_type = MTD_NO_FS;
    
    fs_type = (mtd_fs_type)dload_get_data(DLOAD_FLASH_MODULE, GET_MTD_FS_TYPE_MSG_ID, mtd_name, strlen(mtd_name) + 1);
    if(MTD_UNKNOW_FS > fs_type || MTD_MAX_FS <= fs_type)
    {
        return MTD_NO_FS;
    }
    
    return fs_type;
}

/*****************************************************************************
*Function:         mtd_fd_type_check
*
*Description:      check partition is yaffs or not
*Input:            mtd_name: mtd device name
*Output:           NA
*Return:           0:  not yaffs
*                  1:  is yaffs
*
*Others:           
*******************************************************************************/
mtd_fs_type mtd_fd_type_check(char *mtd_name)
{
    FILE        *filefd = NULL;
    char        mtdname[FILE_NAME_LENGTH] = {0};
    char        partname[FILE_NAME_LENGTH] = {0};
    int         mtdsize = 0;
    int         erasesize = 0;
    char        buf[FILE_DATA_LENGTH] = {0};
    int         mtdlength = 0;
    
    mtd_fs_type mtd_type = MTD_NO_FS;
    
    filefd = fopen("/proc/mtd", "r");
    if (NULL == filefd)
    {
        printf("fopen %s error: %s\n", "/proc/mtd", strerror(errno));
        return MTD_ERROR;
    }

    while(NULL != fgets(buf, sizeof(buf) - 1, filefd))
    {
        if (strncmp(buf, "mtd", MTD_NAME_LENGTH))
        {
            continue;
        }
        
        sscanf(buf, "%s %x %x %s\n", mtdname, &mtdsize, &erasesize, partname);
/*lint -e40 -e409*/
        mtdlength = strlen(mtdname);
        mtdname[mtdlength - 1] = 0;
        if (!strcmp(mtdname, mtd_name))
        {
            mtd_type = dload_mtd_fs_type(mtd_name);

            printf("[%s]: %d\n", __func__, mtd_type);
            break;
        }
    }

/*lint +e40 +e409*/
    return mtd_type;
}

/*****************************************************************************
*Function:         flash_get_info
*
*Description:      get flash info
*Input:            NA
*Output:           NA
*Return:           MTD_ERROR:  failure
*                  MTD_OK:     success
*
*Others:           
*******************************************************************************/
int flash_get_info(void)
{
    mtd_info_t  info = {0};
    int         mtdfd = -1;
    int         ret = 0;
    FILE        *filefd = NULL;
    char        buf[FILE_DATA_LENGTH] = {0};
    char        mtdname[FILE_NAME_LENGTH] = {0};
    char        partname[FILE_NAME_LENGTH] = {0};
    int         startaddr = 0;
    int         endaddr = 0;
    int         startblock = 0;
    int         endblock = 0;
    int         mtdsize = 0;
    int         erasesize = 0;
    int         flashtotal = 0;

    mtdfd = open("/dev/mtd0", O_RDWR);
    if (mtdfd < 0)
    {
        printf("open %s error: %s\n", "/dev/mtd0", strerror(errno));
        return MTD_ERROR;
    }

    ret = ioctl(mtdfd, MEMGETINFO, &info);
    if (0 != ret)
    {
        printf("get meminfo faild, %s\n", strerror(errno)); 
        close(mtdfd);
        return MTD_ERROR;
    }

    close(mtdfd);

    filefd = fopen("/proc/mtd", "r");
    if (NULL == filefd)
    {
        printf("fopen %s error: %s\n", "/proc/mtd", strerror(errno));
        return MTD_ERROR;
    }

    while(NULL != fgets(buf, sizeof(buf) - 1, filefd))
    {
        if (strncmp(buf, "mtd", MTD_NAME_LENGTH))
        {
            continue;
        }

        sscanf(buf, "%s %x %x %s\n", mtdname, &mtdsize, &erasesize, partname);

        flashtotal += mtdsize;
        startaddr = endaddr;
        endaddr += mtdsize;
        startblock = startaddr / erasesize;
        endblock = endaddr / erasesize;

        printf("%s address[%08x -- %08x] block[%04d -- %04d] %s\n", 
            mtdname, startaddr, endaddr - 1, startblock, endblock - 1, partname);
    }

    printf("FLASH INFO: \n");
    printf("totalsize: %d, erasesize: %d, writesize: %d, oobsize: %d\n", 
        flashtotal, info.erasesize, info.writesize, info.oobsize);
    
    fclose(filefd);
    return MTD_OK;
}

/*****************************************************************************
*Function:         mtd_get_info
*
*Description:      get flash a mtd device info
*Input:            mtd_name:  mtd device name
*Output:           NA
*Return:           MTD_ERROR:  failure
*                  MTD_OK:     success
*
*Others:           
*******************************************************************************/
int mtd_get_info(char *mtd_name)
{
    char    mtdfile[FILE_NAME_LENGTH] = {0};
    int     mtdfd = -1;
    mtd_info_t  info = {0};
    int     ret = 0;

    if (NULL == mtd_name)
    {
        printf("mtd_get_info param error\n");
        return MTD_ERROR;
    }
    
    strcpy(mtdfile, "/dev/");
    strcat(mtdfile, mtd_name);

    mtdfd = open(mtdfile, O_RDONLY);
    if (mtdfd < 0)
    {
        printf("open %s error: %s\n", mtdfile, strerror(errno));
        return MTD_ERROR;
    }
    /* get partition info
     */
    ret = ioctl(mtdfd, MEMGETINFO, &info);
    if (0 != ret)
    {
        printf("get meminfo faild, %s\n", strerror(errno)); 
        close(mtdfd);
        return MTD_ERROR;
    }

    close(mtdfd);
    
    printf("%s info: \n", mtd_name);
    printf("mtdsize: %d, erasesize: %d, writesize: %d, oobsize: %d\n", 
        info.size, info.erasesize, info.writesize, info.oobsize);
    printf("block number: [0 -- %d]\n", info.size / info.erasesize - 1);

    return MTD_OK;
}

/*****************************************************************************
*Function:         mtd_query_badblock
*
*Description:      query bad block from start_block to end_block
*Input:            mtd_name:  mtd device name
*                  start_block: query start block
*                  end_block: query end block
*Output:           NA
*Return:           MTD_ERROR:  failure
*                  MTD_OK:     success
*
*Others:           
*******************************************************************************/
int mtd_query_badblock(char *mtd_name, unsigned int start_block, 
    unsigned int end_block)
{
    char    mtdfile[FILE_NAME_LENGTH] = {0};
    int     mtdfd = -1;
    int     ret = 0;
    int     totalbad = 0;
    
    unsigned int    curblock = start_block;
    unsigned long long  curaddr = 0;
    mtd_info_t  info = {0};

    if (NULL == mtd_name || start_block > end_block)
    {
        printf("mtd_query_badblock param error\n");
        return MTD_ERROR;
    }

    strcpy(mtdfile, "/dev/");
    strcat(mtdfile, mtd_name);

    mtdfd = open(mtdfile, O_RDONLY);
    if (mtdfd < 0)
    {
        printf("open %s error: %s\n", mtdfile, strerror(errno));
        return MTD_ERROR;
    }

    ret = ioctl(mtdfd, MEMGETINFO, &info);
    if (0 != ret)
    {
        printf("get meminfo faild, %s\n", strerror(errno)); 
        close(mtdfd);
        return MTD_ERROR;
    }
    /* check end_block is over 
     */
    if (end_block * info.erasesize >= info.size)
    {
        printf("block number %d over, max %d\n", 
            end_block, info.size / info.erasesize - 1);
        return MTD_ERROR;
    }

    printf("Bad block in [%d -- %d]: ", start_block, end_block);
    while(curblock <= end_block)
    {
        curaddr = (unsigned long long)curblock * (unsigned long long)info.erasesize;
        ret = ioctl(mtdfd, MEMGETBADBLOCK, &curaddr);
        if(ret < 0)
        {
            printf("GET MEMGETBADBLOCK error, offset: %lld\n", curaddr);
            close(mtdfd);
            return MTD_ERROR;
        }
        /* bad block 
         */
        if(1 == ret)
        {
            printf("%d ", curblock);
            totalbad++;
        }

        curblock++;
    }

    if (!totalbad)
    {
        printf("None\n");
    }
    else
    {
        printf("\n");
    }
    close(mtdfd);
    return MTD_OK;
}

/*****************************************************************************
*Function:         mtd_mark_badblock
*
*Description:      mark block as a bad block
*Input:            mtd_name:  mtd device name
*                  block: the block to mark as bad
*Output:           NA

*Return:           MTD_ERROR:  failure
*                  MTD_OK:     success
*
*Others:           
*******************************************************************************/
int mtd_mark_badblock(char *mtd_name, unsigned int start_block,
        unsigned int end_block)
{
    char    mtdfile[FILE_NAME_LENGTH] = {0};
    int     mtdfd = -1;
    int     ret = 0;
    
    unsigned int    curblock = start_block;
    unsigned long long  curaddr = 0;
    mtd_info_t  info = {0};

    if (NULL == mtd_name)
    {
        printf("mtd_mark_badblock param error\n");
        return MTD_ERROR;
    }

    strcpy(mtdfile, "/dev/");
    strcat(mtdfile, mtd_name);

    mtdfd = open(mtdfile, O_RDONLY);
    if (mtdfd < 0)
    {
        printf("open %s error: %s\n", mtdfile, strerror(errno));
        return MTD_ERROR;
    }

    ret = ioctl(mtdfd, MEMGETINFO, &info);
    if (0 != ret)
    {
        printf("get meminfo faild, %s\n", strerror(errno)); 
        return MTD_ERROR;
    }
    /* check end_block is over 
     */
    if (end_block * info.erasesize >= info.size)
    {
        printf("block number %d over, max %d\n", 
            end_block, info.size / info.erasesize - 1);
        return MTD_ERROR;
    }

    while(curblock <= end_block)
    {
        curaddr = (unsigned long long)curblock * (unsigned long long)info.erasesize;
        ret = ioctl(mtdfd, MEMSETBADBLOCK, &curaddr);
        if(ret < 0)
        {
            printf("GET MEMGETBADBLOCK error, offset: %lld\n", curaddr);
            close(mtdfd);
            return MTD_ERROR;
        }

        curblock++;
    }
    
    return MTD_OK;
}

/*****************************************************************************
*Function:         mtd_erase_block
*
*Description:      erase block from start_block to end_block
*Input:            mtd_name:  mtd device name
*                  start_block: query start block
*                  end_block: query end block
*Output:           NA
*Return:           MTD_ERROR:  failure
*                  MTD_OK:     success
*
*Others:           
*******************************************************************************/
int mtd_erase_block(char *mtd_name, unsigned int start_block, 
    unsigned int end_block, int isforce)
{
    char    mtdfile[FILE_NAME_LENGTH] = {0};
    int     mtdfd = -1;
    int     ret = 0;
    
    unsigned int    curblock = start_block;
    unsigned long long  curaddr = 0;
    struct erase_info_user erase_info;
    mtd_info_t  info = {0};

    if (NULL == mtd_name || start_block > end_block)
    {
        printf("mtd_erase_block param error\n");
        return MTD_ERROR;
    }

    strcpy(mtdfile, "/dev/");
    strcat(mtdfile, mtd_name);

    mtdfd = open(mtdfile, O_RDWR);
    if (mtdfd < 0)
    {
        printf("open %s error: %s\n", mtdfile, strerror(errno));
        return MTD_ERROR;
    }
    /* get info 
     */
    ret = ioctl(mtdfd, MEMGETINFO, &info);
    if (0 != ret)
    {
        printf("get meminfo faild, %s\n", strerror(errno)); 
        close(mtdfd);
        return MTD_ERROR;
    }
    /* check address is over 
     */
    if (end_block * info.erasesize >= info.size)
    {
        printf("block number %d over, max %d\n", 
            end_block, info.size / info.erasesize - 1);
        return MTD_ERROR;
    }

    while(curblock <= end_block)
    {
        /* if not force erase, skip bad block
         */
        curaddr = (unsigned long long)curblock * (unsigned long long)info.erasesize;
        if (!isforce)
        {
            ret = ioctl(mtdfd, MEMGETBADBLOCK, &curaddr);
            if(ret < 0)
            {
                printf("GET MEMGETBADBLOCK error, offset: %lld\n", curaddr);
                close(mtdfd);
                return MTD_ERROR;
            }
            /* bad block 
             */
            if(1 == ret)
            {
                curblock++;
                continue;
            }
        }
        /* ok erase 
         */
        erase_info.start = curaddr;
        erase_info.length = info.erasesize;
        ret = ioctl(mtdfd, MEMERASE, &erase_info);
        if(ret < 0)
        {
            printf("erase %d block error : %s\n", curblock, strerror(errno));
            close(mtdfd);
            return MTD_ERROR;
        }
        curblock++;
    }

    return MTD_OK;
}

/*****************************************************************************
*Function:         mtd_read_block
*
*Description:      read block from start_block to end_block
*Input:            mtd_name:  mtd device name
*                  start_block: query start block
*                  end_block: query end block
*                  filename: save data to 
*Output:           NA
*Return:           MTD_ERROR:  failure
*                  MTD_OK:     success
*
*Others:           
*******************************************************************************/
int mtd_read_block(char *mtd_name, unsigned int start_block, 
    unsigned int end_block, char *filename)
{
    char    mtdfile[FILE_NAME_LENGTH] = {0};
    int     mtdfd = -1;
    int     ret = 0;
    char    *databuf = NULL;
    int     blocksize = 0;
    int     sparesize = 0;
    FILE    *destfp = NULL;
    mtd_fs_type mtd_fstype = MTD_NO_FS;
    
    unsigned int    curblock = start_block;
    //unsigned long long  curaddr = 0;
    //struct erase_info_user erase_info;
    mtd_info_t  info = {0};

    if (NULL == mtd_name || NULL == filename
        || start_block > end_block)
    {
        printf("mtd_read_block param error\n");
        return MTD_ERROR;
    }

    strcpy(mtdfile, "/dev/");
    strcat(mtdfile, mtd_name);

    mtdfd = open(mtdfile, O_RDONLY);
    if (mtdfd < 0)
    {
        printf("open %s error: %s\n", mtdfile, strerror(errno));
        return MTD_ERROR;
    }

    ret = ioctl(mtdfd, MEMGETINFO, &info);
    if (0 != ret)
    {
        printf("get meminfo faild, %s\n", strerror(errno)); 
        close(mtdfd);
        return MTD_ERROR;
    }
    /* check end_block is over 
     */
    if (end_block * info.erasesize >= info.size)
    {
        printf("block number %d over, max %d\n", 
            end_block, info.size / info.erasesize - 1);
        return MTD_ERROR;
    }
    /* get spare size 
     */
    if(FLASH_PAGE_SIZE_4096 == info.writesize)
    {
        sparesize = ((info.writesize >> FLASH_CODEWORD_SHIFT) 
                        * FLASH_CW_OOBSIZE_4096);
    }
    else
    {
        sparesize = ((info.writesize >> FLASH_CODEWORD_SHIFT) 
                        * FLASH_CW_OOBSIZE_2048);
    }

    destfp = fopen(filename, "w");
    if (NULL == destfp)
    {
        printf("fopen error: %s\n", strerror(errno));
        close(mtdfd);
        return MTD_ERROR;
    }
    /* get block data size 
     */
    mtd_fstype = mtd_fd_type_check(mtd_name);
    if (MTD_YAFFS2_FS == mtd_fstype)
    {
        blocksize = (info.erasesize / info.writesize) * sparesize + info.erasesize; 
    }
    else if(MTD_UNKNOW_FS == mtd_fstype)
    {
        printf("[%s]:mtd_fstype get error.\n", __func__);
        close(mtdfd);
        return MTD_ERROR;
    }
    else
    {
        blocksize = info.erasesize;
    }
    
    databuf = malloc(blocksize);
    if (NULL == databuf)
    {
        printf("read block malloc error\n");
        close(mtdfd);
        fclose(destfp);
        return MTD_ERROR;
    }

    while(curblock <= end_block)
    {      
/*lint -e119*/
        /* read a block, need kernel read main and oob 
         */
        ret = pread(mtdfd, databuf, blocksize, curblock * info.erasesize);
        if (ret != blocksize)
        {
            printf("read error: %s\n", strerror(errno));
            goto out;
        }
/*lint +e119*/
        ret = fwrite(databuf, 1, blocksize, destfp);
        if (ret != blocksize)
        {
            printf("fwrite error: %s\n", strerror(errno));
            goto out;
        }
        curblock++;
    }

out:
    free(databuf);
    close(mtdfd);
    fclose(destfp);

    return MTD_OK;
}

/*****************************************************************************
*Function:         mtd_read_block
*
*Description:      read block from start_block to end_block
*Input:            mtd_name:  mtd device name
*
*                  start_block: query start block
*                  end_block: query end block
*                  time: write times 
*Output:           NA
*
*Return:           MTD_ERROR:  failure
*                  MTD_OK:     success
*
*Others:           
*******************************************************************************/
int mtd_write_block(char *mtd_name, unsigned int start_block, 
    unsigned int end_block, int times)
{
    char    mtdfile[FILE_NAME_LENGTH] = {0};
    int     mtdfd = -1;
    mtd_info_t  info = {0};
    int     ret = 0;
    char    *databufA = NULL;
    char    *databufB = NULL;
    char    *databuf = NULL;
    char    *readbuf = NULL;
    int     blocksize = 0;
    int     sparesize = 0;
    int     i = 0;
    int     end = 0;
    unsigned int    curblock = start_block;
    unsigned long long  curaddr = 0;
    struct erase_info_user erase_info;
    int     badblock_bef = 0;
    int     badblock_aft = 0;
    int     j = 0;
    mtd_fs_type mtd_fstype = MTD_NO_FS;

    if (NULL == mtd_name || start_block > end_block 
        || times <= 0)
    {
        printf("mtd_write_block param error\n");
        return MTD_ERROR;
    }

    strcpy(mtdfile, "/dev/");
    strcat(mtdfile, mtd_name);

    mtdfd = open(mtdfile, O_RDWR);
    if (mtdfd < 0)
    {
        printf("open %s error: %s\n", mtdfile, strerror(errno));
        return MTD_ERROR;
    }

    ret = ioctl(mtdfd, MEMGETINFO, &info);
    if (0 != ret)
    {
        printf("get meminfo faild, %s\n", strerror(errno)); 
        close(mtdfd);
        return MTD_ERROR;
    }
    /* check end_block is over 
     */
    if (end_block * info.erasesize >= info.size)
    {
        printf("block number %d over, max %d\n", 
            end_block, info.size / info.erasesize - 1);
        return MTD_ERROR;
    }

    if(FLASH_PAGE_SIZE_4096 == info.writesize)
    {
        sparesize = ((info.writesize >> FLASH_CODEWORD_SHIFT) 
                        * FLASH_CW_OOBSIZE_4096);
    }
    else
    {
        sparesize = ((info.writesize >> FLASH_CODEWORD_SHIFT) 
                        * FLASH_CW_OOBSIZE_2048);
    }
    
    mtd_fstype = mtd_fd_type_check(mtd_name);
    if (MTD_YAFFS2_FS == mtd_fstype)
    {
        blocksize = (info.erasesize / info.writesize) * sparesize + info.erasesize; 
    }
    else if(MTD_UNKNOW_FS == mtd_fstype)
    {
        printf("[%s]:mtd_fstype get error.\n", __func__);
        close(mtdfd);
        return MTD_ERROR;
    }
    else
    {
        blocksize = info.erasesize;
    }

    /* malloc 3 block memory 
     */
    databufA = malloc(blocksize);
    if (NULL == databufA)
    {
        printf("databufA malloc error\n");
        close(mtdfd);
        return MTD_ERROR;
    }
    memset(databufA, FLASH_TEST_NUMBER_A, blocksize);
    
    databufB = malloc(blocksize);
    if (NULL == databufB)
    {
        printf("databufB malloc error\n");
        free(databufA);
        close(mtdfd);
        return MTD_ERROR;
    }
    memset(databufB, FLASH_TEST_NUMBER_B, blocksize);

    readbuf = malloc(blocksize);
    if (NULL == readbuf)
    {
        printf("readbuf malloc error\n");
        free(databufA);
        free(databufB);
        close(mtdfd);        
        return MTD_ERROR;
    }

    curblock = start_block;
    while(curblock <= end_block)
    { 
        /* check bad block 
         */
        curaddr = (unsigned long long)curblock * (unsigned long long)info.erasesize;
        ret = ioctl(mtdfd, MEMGETBADBLOCK, &curaddr);
        if(ret < 0)
        {
            printf("GET MEMGETBADBLOCK error, offset: %lld\n", curaddr);
            goto out;
        }  
        if (1 == ret)
        {
            badblock_bef++;
        }
        curblock++;
    }

    /* write times */
    printf("****************Flash Test Begin****************\n");
    for (i = 0; i < times; i++)
    {
        printf("----------------Time %d Begin----------------\n", i);
        if (0 == i % WRITE_TURN_MOD)
        {
            curblock = start_block;
            end = end_block + 1;
        }
        else
        {
            curblock = end_block;
            end = start_block - 1;
        }
        /* write every block 
         */
        while(curblock != end)
        {      
            /* check bad block 
             */
            curaddr = (unsigned long long)curblock * (unsigned long long)info.erasesize;
            ret = ioctl(mtdfd, MEMGETBADBLOCK, &curaddr);
            if(ret < 0)
            {
                printf("GET MEMGETBADBLOCK error, offset: %lld\n", curaddr);
                goto out;
            }
            /* bad block 
             */
            if(1 == ret)
            {
                if (0 == i % WRITE_TURN_MOD)
                {
                    curblock++;
                }
                else
                {
                    curblock--;
                }
                continue;
            }

            /* erase 
             */
            erase_info.start = curaddr;
            erase_info.length = info.erasesize;
            ret = ioctl(mtdfd, MEMERASE, &erase_info);
            if(ret < 0)
            {
                ret = ioctl(mtdfd, MEMSETBADBLOCK, &curaddr);
                if(ret < 0)
                {
                    printf("GET MEMSETBADBLOCK error, offset: %lld\n", curaddr);
                    goto out;
                }
            }
            printf("Block %d Erasing...\n", curblock);
            /* turn buffer
             */
            if (databuf == databufA)
            {
                databuf = databufB;
            }
            else
            {
                databuf = databufA;
            }
/*lint -e119*/
            /* write 
             */
            ret = pwrite(mtdfd, databuf, blocksize, curblock * info.erasesize);
            if (ret != blocksize)
            {
                printf("pwrite error: %s\n", strerror(errno));
                goto out;
            }
            printf("Block %d Write 0X%X\n", curblock, databuf[0]);

            /* read 
             */
            ret = pread(mtdfd, readbuf, blocksize, curblock * info.erasesize);
            /* if block ecc error, continue */
            if (ret != blocksize && ( - EBADMSG ) == ret)
            {
                ret = ioctl(mtdfd, MEMSETBADBLOCK, &curaddr);
                if(ret < 0)
                {
                    printf("[GET MEMSETBADBLOCK error, offset: %lld\n]", curaddr);
                    goto out;
                }
                printf("Block %d Test FAILD\n", curblock);
                if (0 == i % WRITE_TURN_MOD)
                {
                    curblock++;
                }
                else
                {
                    curblock--;
                }
                continue;
            }
            else if (ret != blocksize)
            {
                printf("pread error: %s\n", strerror(errno));
                goto out;
            }

/*lint +e119*/
            /* check, yaffs partiton available data only 
             * main + OOB_SIZE_USE
             */
            if (MTD_YAFFS2_FS == mtd_fstype)
            {
                j = 0;
                while(j < blocksize)
                {
                    ret = memcmp(readbuf + j, 
                            databuf + j, info.writesize + OOB_SIZE_USE);
                    if (0 != ret)
                    {
                        break;   
                    }
                    j += (info.writesize + sparesize);
                }
            }
            else
            {
                ret = memcmp(readbuf, databuf, blocksize);
            }
            
            if (ret)
            {
                /* mark block as a bad block 
                 */
                
                ret = ioctl(mtdfd, MEMSETBADBLOCK, &curaddr);
                if(ret < 0)
                {
                    printf("[GET MEMSETBADBLOCK error, offset: %lld\n]", curaddr);
                    goto out;
                }
                
                printf("Block %d Test FAILD\n", curblock);
            }
            else
            {
                printf("Block %d Test PASS\n", curblock);
            }

            if (0 == i % WRITE_TURN_MOD)
            {
                curblock++;
            }
            else
            {
                curblock--;
            }
        }
        printf("------------------Time %d End----------------\n", i);
    }

    printf("****************Flash Test End****************\n");

    curblock = start_block;
    while(curblock <= end_block)
    { 
        /* check bad block after test */
        curaddr = (unsigned long long)curblock * (unsigned long long)info.erasesize;
        ret = ioctl(mtdfd, MEMGETBADBLOCK, &curaddr);
        if(ret < 0)
        {
            printf("GET MEMGETBADBLOCK error, offset: %lld\n", curaddr);
            goto out;
        }  
        if (1 == ret)
        {
            badblock_aft++;
        }
        curblock++;
    }
    printf("Bad Block: before %d, after %d\n", 
            badblock_bef, badblock_aft);
out:
    free(databufA);
    free(databufB);
    free(readbuf);
    close(mtdfd);

    return MTD_OK;
}

/*****************************************************************************
*Function:         mtdutils_main
*
*Description:      flash test main function
*Input:            argc: 
*                  argv:
*
*Output:           NA
*Return:           MTD_ERROR:  failure
*                  MTD_OK:     success
*
*Others:           MTD_OK
*******************************************************************************/
int mtdutils_main(int argc, char *argv[])
{
    int             ret = 0;
    int             comtype = COMMAND_TYPE_INVIABLE;
    int             cmd_para = 0;
    int             startblock = 0;
    int             endblock = 0;

    if (argc < MTDUTILS_CMD_PARAM_MIN)
    {
        printf_usage(argv[0]);
        return MTD_ERROR;
    }
    
    cmd_para = strtol(argv[1], NULL, STRTOL_BASE_10);
    if (LONG_MIN == cmd_para 
        || LONG_MAX == cmd_para)
    {
        printf("param %s error\n", argv[1]);
        return MTD_ERROR;
    }
    comtype = cmd_para;

    /* get start block and end block 
     */
    if (argc >= MTDUTILS_CMD_PARAM_5)
    {
        startblock = strtol(argv[MTDUTILS_CMD_PARAM_3], NULL, STRTOL_BASE_10);
        if (LONG_MIN == startblock 
            || LONG_MAX == startblock)
        {
            printf("param %s error\n", argv[MTDUTILS_CMD_PARAM_3]);
            return MTD_ERROR;
        }
        endblock = strtol(argv[MTDUTILS_CMD_PARAM_4], NULL, STRTOL_BASE_10);
        if (LONG_MIN == endblock 
            || LONG_MAX == endblock)
        {
            printf("param %s error\n", argv[MTDUTILS_CMD_PARAM_4]);
            return MTD_ERROR;
        }
    }
    
    switch(comtype)
    {
        /* get flash info
         */
        case (COMMAND_TYPE_GET_FLASHINFO):
        {
            ret = flash_get_info();
        }
        break;
        /* get partition info
         */
        case (COMMAND_TYPE_GET_MTDINFO):
        {
            if (argc != MTDUTILS_CMD_PARAM_3)
            {
                printf_usage(argv[0]);
                return MTD_ERROR;
            }
            ret = mtd_get_info(argv[MTDUTILS_CMD_PARAM_MIN]);
        }
        break;
        /* query bad block 
         */
        case (COMMAND_TYPE_QUERY_BAD):
        {
            if (argc != MTDUTILS_CMD_PARAM_5)
            {
                printf_usage(argv[0]);
                return MTD_ERROR;
            }

            ret = mtd_query_badblock(argv[MTDUTILS_CMD_PARAM_MIN], 
                startblock, endblock);
        }
        break;
        /* mark bad block 
         */
        case (COMMAND_TYPE_MARK_BAD):
        {
            if (argc != MTDUTILS_CMD_PARAM_5)
            {
                printf_usage(argv[0]);
                return MTD_ERROR;
            }
            ret = mtd_mark_badblock(argv[MTDUTILS_CMD_PARAM_MIN], 
                startblock, endblock);
        }
        break;
        /* erase block 
         */
        case (COMMAND_TYPE_ERASE_DATA):
        {
            if (argc != MTDUTILS_CMD_PARAM_5)
            {
                printf_usage(argv[0]);
                return MTD_ERROR;
            }
            ret = mtd_erase_block(argv[MTDUTILS_CMD_PARAM_MIN], 
                startblock, endblock, 0);
        }
        break;
        /* force erase block 
         */        
        case (COMMAND_TYPE_ERASE_FORCE_DATA):
        {
            if (argc != MTDUTILS_CMD_PARAM_5)
            {
                printf_usage(argv[0]);
                return MTD_ERROR;
            }
            ret = mtd_erase_block(argv[MTDUTILS_CMD_PARAM_MIN], 
                startblock, endblock, 1);
        }
        break;
        /* read block 
         */
        case (COMMAND_TYPE_READ_DATA):
        {
            if (argc != MTDUTILS_CMD_PARAM_6)
            {
                printf_usage(argv[0]);
                return MTD_ERROR;
            }
            ret = mtd_read_block(argv[MTDUTILS_CMD_PARAM_MIN], 
                startblock, endblock, argv[MTDUTILS_CMD_PARAM_5]);
        }
        break;
        /* write block 
         */
        case (COMMAND_TYPE_WRITE_DATA):
        {
            if (argc != MTDUTILS_CMD_PARAM_6)
            {
                printf_usage(argv[0]);
                return MTD_ERROR;
            }
            ret = mtd_write_block(argv[MTDUTILS_CMD_PARAM_MIN], 
                startblock, endblock, atoi(argv[MTDUTILS_CMD_PARAM_5]));
        }
        break;

        default:
        {
            printf("command type error\n");
            return MTD_ERROR;
        }
    }

    if (0 != ret)
    {
        printf("COMMAND EXECUTE ERROR\n");
    }

    return ret;
}
/*END DTS2013052007435 QIUJIAN 00208481 2013-05-20 ADDED*/

