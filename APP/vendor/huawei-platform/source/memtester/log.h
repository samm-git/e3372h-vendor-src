/* < DTS2013093005579 xiaohui 20131009 begin */

#ifndef __MEMTESTER_LOG_H__
#define __MEMTESTER_LOG_H__/*lint !e750*/

int log_pool_init( void );/*lint !e752*/
char *log_buffer_get(int user);/*lint !e752*/
int log_flush_buf( int buffered, int user  );/*lint !e752*/
void log_put( int buffered, char *buffer, int newline, const char *fmt, ... );/*lint !e752*/

#endif
/* DTS2013093005579 xiaohui 20131009 end > */

