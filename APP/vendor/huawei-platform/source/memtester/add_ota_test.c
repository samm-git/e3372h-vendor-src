/* < DTS2013093005579 xiaohui 20131009 begin */
/* < DTS2013112300820 xiaohui 20131123 begin */

#include <stdio.h>
//#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <fcntl.h>

/*lint -e451 */
#include <errno.h>
#include <stdarg.h>
#include <string.h>
/*lint +e451 */

/*lint -e613*/

#include "log.h"
#include "add_ota_test.h"
#include "common.h"
static const int MAX_ARGS = 100;
static const char *COMMAND_FILE = "/mobilelog/ota_para.cfg";
FILE *cmd_pipe;/*lint !e752*/

// Read a file into memory; optionally (retouch_flag == RETOUCH_DO_MASK) mask
// the retouched entries back to their original value (such that SHA-1 checks
// don't fail due to randomization); store the file contents and associated
// metadata in *file.
//
// Return 0 on success.
int LoadFileContents(const char* filename, FileContents* file)
{
    file->data = NULL;

    if (stat(filename, &file->st) != 0)
    {
        printf("failed to stat \"%s\": %s\n", filename, strerror(errno));
        return -1;
    }

    file->size = (size_t)file->st.st_size;
    file->data = malloc(file->size + 1 + 1 );
    if ( NULL == file->data )
    {
        printf("malloc error:%s,line:%d\n" , __func__ , __LINE__ ) ;
        return -1 ;
    }
    memset( file->data , 0 , file->size + 1 + 1 ) ;
    FILE* f = fopen(filename, "rb");
    if (f == NULL)
    {
        printf("failed to open \"%s\": %s\n", filename, strerror(errno));
        free(file->data);
        file->data = NULL;
        return -1;
    }

    size_t bytes_read = fread(file->data, 1, file->size, f);
    if (bytes_read != file->size)
    {
        printf("short read of \"%s\" (%ld bytes of %ld)\n",
               filename, (long)bytes_read, (long)file->size);
        fclose(f);
        free(file->data);
        file->data = NULL;
        return -1;
    }
    if ( ' ' != file->data[file->size-1] )
    {
        printf("end char is not space\n" );
        file->data[file->size]  = ' ' ;
    }
    fclose(f);
    printf("read config:<%s>\n" , file->data ) ;
    return 0;
}


int get_args(int *argc, char ***argv)
{
    FileContents cur_file_content;
    int i;
    if ( 0 == LoadFileContents(COMMAND_FILE, &cur_file_content) )
    {
        const char *arg;
        char *argv0 = (*argv)[0] ;
//not need the cmd_pipe to print message delete by ouyangxun
#if 0
        char *cmd_fd_s = (*argv)[*argc-1];
        int cmd_fd = atoi( cmd_fd_s );
        cmd_pipe = fdopen(cmd_fd, "wb");
        setlinebuf(cmd_pipe);
#endif

        *argv = (char **) malloc(sizeof(char *) * MAX_ARGS);
        (*argv)[0] =  argv0;  // use the same program name
        arg = strtok((char*)cur_file_content.data, " ") ;
        (*argv)[1] =  strdup(arg);

        for (*argc = 2; *argc < MAX_ARGS; ++*argc)
        {
            if ((arg = strtok(NULL, " ")) == NULL)
            {
                break;
            }
            (*argv)[*argc] = strdup(arg);  // Strip newline.
        }

        printf("Got arguments from %s\n", COMMAND_FILE);
    }
    printf("paras:\n" ) ;
    for ( i = 0 ; i < *argc; i ++ )
    {
        printf("%d:%s\n" , i ,   (*argv)[i] ) ;
    }
    return 0 ;
}

/*===========================================================================
FUNCTION  int fprintf(char* fmt, ...)
DESCRIPTION
  this is the same with printf,but print current time.

PARAMETER
  char* fmt:the formation of string.
  ....:this is the para of the fmt.

RETURN VALUE
  int:return vfprintf's value,please reference the standard manual,the return value can be ignore.

NOTICE
  None.
===========================================================================*/
int timestamp_print11111(FILE *stream, char* fmt, ...)       //一个简单的类似于printf的实现
{
    char    datetime[1024 ] = {'\0'} ;
    int ret;
    va_list ap;
    struct timeval t;
    gettimeofday(&t, NULL);
    struct tm * ptm = localtime(&t.tv_sec);

    memset( &ap , 0 , sizeof( va_list ) ) ;

    sprintf(datetime , "[%.4d-%.2d-%.2d %.2d:%.2d:%.2d %.3ld]" ,
            ptm->tm_year + 1900 , /*         ——年*/
            ptm->tm_mon + 1,/*         ——月*/
            ptm->tm_mday,/*         ——日*/
            ptm->tm_hour,/*         ——时*/
            ptm->tm_min,/*         ——分*/
            ptm->tm_sec,/*         ——秒*/
            t.tv_usec / 1000) ;/*         ——毫秒  */
    fprintf(stream, "%s ", datetime);
    va_start(ap, fmt);
    ret = fprintf(stream, fmt, ap);
    va_end(ap);
    return (ret);
}

int memtester_print1111( char*fmt, ...)
{
    int ret;
    va_list ap;

    memset( &ap , 0 , sizeof( va_list ) ) ;

    va_start(ap, fmt);
    ret = fprintf(stdout, fmt, ap);
    va_end(ap);
    return ret ;
}

int timestamp_print_time(FILE *stream, int thread_id )        //一个简单的类似于printf的实现
{
    char    datetime[1024 ] = {'\0'} ;
    struct timeval t;
    gettimeofday(&t, NULL);
    struct tm * ptm = localtime(&t.tv_sec);

    if ( thread_id == -1 )
    {
        sprintf(datetime , "[all][%.4d-%.2d-%.2d %.2d:%.2d:%.2d %.3ld]" ,
                ptm->tm_year + 1900 , /*         ——年*/
                ptm->tm_mon + 1,/*         ——月*/
                ptm->tm_mday,/*         ——日*/
                ptm->tm_hour,/*         ——时*/
                ptm->tm_min,/*         ——分*/
                ptm->tm_sec,/*         ——秒*/
                t.tv_usec / 1000) ;/*         ——毫秒  */
    }
    else
    {
        sprintf(datetime , "[%d][%.4d-%.2d-%.2d %.2d:%.2d:%.2d %.3ld]" ,
                thread_id,
                ptm->tm_year + 1900 , /*         ——年*/
                ptm->tm_mon + 1,/*         ——月*/
                ptm->tm_mday,/*         ——日*/
                ptm->tm_hour,/*         ——时*/
                ptm->tm_min,/*         ——分*/
                ptm->tm_sec,/*         ——秒*/
                t.tv_usec / 1000) ;/*         ——毫秒  */
    }
    fprintf(stream, "%s ", datetime);
    return 0 ;
}

int timestamp_print_time_buf(char *buf, int thread_id )        //一个简单的类似于printf的实现
{
    extern int g_log_buffered;

    struct timeval t;
    gettimeofday(&t, NULL);
    struct tm * ptm = localtime(&t.tv_sec);

    log_put( g_log_buffered, buf, 1, "[%d][%.4d-%.2d-%.2d %.2d:%.2d:%.2d %.3ld]\n" ,
             thread_id,
             ptm->tm_year + 1900 , /*         ——年*/
             ptm->tm_mon + 1,/*         ——月*/
             ptm->tm_mday,/*         ——日*/
             ptm->tm_hour,/*         ——时*/
             ptm->tm_min,/*         ——分*/
             ptm->tm_sec,/*         ——秒*/
             t.tv_usec / 1000) ;/*         ——毫秒  */

    return 0 ;
}

int timestamp_print_time_mini(char *buf, int thread_id )        //一个简单的类似于printf的实现
{
    extern int g_log_buffered;

    struct timeval t;
    gettimeofday(&t, NULL);
    struct tm * ptm = localtime(&t.tv_sec);

    log_put( g_log_buffered, buf, 1, "[%d][%.2d:%.2d:%.2d]\n" ,
             thread_id,
             ptm->tm_hour,/*         ——时*/
             ptm->tm_min,/*         ——分*/
             ptm->tm_sec/*         ——秒*/
           ) ;

    return 0 ;
}

/* DTS2013112300820 xiaohui 20131009 end > */
/* DTS2013093005579 xiaohui 20131009 end > */

