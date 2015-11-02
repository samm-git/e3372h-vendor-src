/* < DTS2013093005579 xiaohui 20131009 begin */
/* < DTS2013112300820 xiaohui 20131123 begin */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
/*lint -e451 */
#include <string.h>
#include <stdarg.h>
/*lint +e451 */

/*lint -e620*/
#include "log.h"

#define TEST_LINE_LEN  (40ul)
#define TEST_LOG_HEADER (200ul)
#define TEST_BUF_SIZE (2048ul)

#define LOG_BUFFER_TEMP_SIZE (256)

typedef struct s_log_buffer
{
    size_t buf_size;
    char *buf;
}t_log_buffer;

t_log_buffer *log_pool = NULL;

pthread_mutex_t printer_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t printer_nb_mutex = PTHREAD_MUTEX_INITIALIZER;

extern size_t get_tests_count( void );
extern int thread_count;
extern FILE *cmd_pipe;/*lint !e752*/

int get_user_count()
{
    return thread_count;
}

int log_pool_init( void )
{
    size_t user_count = (size_t)get_user_count();
    size_t i;
    size_t buf_size;
    char *buf;

    log_pool = (t_log_buffer*)malloc( sizeof(t_log_buffer) * user_count );
    if ( !log_pool )
    {
        return -1;
    }

    memset((void*)log_pool, 0 , sizeof(t_log_buffer)*user_count );

    buf_size = TEST_LOG_HEADER + TEST_LINE_LEN * get_tests_count();

    if ( buf_size < TEST_BUF_SIZE )
    {
        buf_size = TEST_BUF_SIZE;
    }

    for ( i = 0; i < user_count; i++ )
    {
        buf = (char*)malloc(buf_size);
        if (!buf)
        {
            return -1;
        }

        memset( (void*)buf, 0 , buf_size );

        log_pool[i].buf = buf;
        log_pool[i].buf_size = buf_size;
    }

    return 0;
}

void log_pool_destroy( void )
{
    int user_count = get_user_count();
    int i;

    for ( i = 0; i < user_count; i++ )
    {
        free(log_pool[i].buf);
    }
    free( log_pool );

}

char *log_buffer_get( int user)
{

    if ( user >= get_user_count() )
    {
        return NULL;
    }

    return log_pool[user].buf;

}

int log_flush_buf( int buffered, int user  )
{
    int times, i;
    char backup;
    char *pos;
    char *buf;
    size_t buf_size;


    if ( user >= get_user_count() )
    {
        return -1;
    }

    pthread_mutex_lock(&printer_mutex);

    if ( buffered )
    {
        i = user;

        buf = log_pool[i].buf;
        buf_size = log_pool[i].buf_size;
        pos = buf;

        times = (int)(( buf_size + LOG_BUFFER_TEMP_SIZE - 1 ) / LOG_BUFFER_TEMP_SIZE);


        for ( i = 1; i < times; i++ )
        {
            backup = buf[LOG_BUFFER_TEMP_SIZE*i];
            buf[LOG_BUFFER_TEMP_SIZE*i] = 0;
            printf( "%s", pos );
            buf[LOG_BUFFER_TEMP_SIZE*i] = backup;
            pos = &buf[LOG_BUFFER_TEMP_SIZE*i];
        }
        printf( "%s", pos );
        memset( buf, 0, buf_size);
    }

    fflush(stdout);

    pthread_mutex_unlock(&printer_mutex);

    return 0;
}

void log_put( int buffered, char *buffer, int newline, const char *fmt, ... )
{
    char log_line[256];
    va_list ap;

    memset( &ap , 0 , sizeof( va_list ) ) ;

    /*lint -e64 */
    va_start(ap, fmt);
    memset( log_line, 0 , sizeof(log_line) );

    if ( buffered )
    {
        vsnprintf(log_line, 256, fmt, ap);
        strcat( buffer , log_line );
    }
    else
    {

        pthread_mutex_lock(&printer_nb_mutex);

        vfprintf( stdout, fmt, ap );

        if ( newline )
        {
            //printf("ui_print\n");
        }

        sprintf(log_line, "%s\n", fmt );
        printf("%s\n", log_line);
        pthread_mutex_unlock(&printer_nb_mutex);
    }

    /*lint +e64 */
    va_end(ap);

}
/* DTS2013112300820 xiaohui 20131009 end > */
/* DTS2013093005579 xiaohui 20131009 end > */

