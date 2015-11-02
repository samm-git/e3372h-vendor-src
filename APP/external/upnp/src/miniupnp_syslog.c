#include <stdio.h>
#include <stdarg.h>
#include "miniupnp_syslog.h"
int global_log_level = 0;

/*Begin:added by c00127420 for upnp log to QXDM 1092205664*/
#define MAX_MSG_LEN  512

#define UPNP_MSG_HIGH(x_fmt, a) \
    printf (x_fmt, a)
#define UPNP_MSG_ERROR(x_fmt, a) \
    printf (x_fmt, a)

static void upnp_format_msg(char* buf,UPNP_LOG_LEVEL lv, const char * msg, va_list va)
{
    int length = 0;
	length = vsnprintf(buf, MAX_MSG_LEN, msg, va);
}

static void print_upnp_log(char* buf,UPNP_LOG_LEVEL lv, const char * msg, va_list va)
{
    switch (lv)
    {
        case UPNP_LOG_LV_WARN:
			upnp_format_msg(buf,lv,msg,va);
            UPNP_MSG_HIGH("%s\n",buf);
            break;
		case UPNP_LOG_LV_ERROR:
			upnp_format_msg(buf,lv,msg,va);
			UPNP_MSG_ERROR("%s\n",buf);
			break;
        default:
			upnp_format_msg(buf,lv,msg,va);
            UPNP_MSG_HIGH("%s\n",buf);
            break;
    }
}
/*End:added by c00127420 for upnp log to QXDM 1092205664*/

int syslog(int log_level, const char * format, ...)
{
	va_list ap;
	char   buf[MAX_MSG_LEN];
	int     retval = 0;

	if (log_level < global_log_level)
		return 0;

/*Begin:modified by c00127420 for upnp log to QXDM 1092205664*/
	switch(log_level)
	{
		case LOG_ERR:
			log_level = UPNP_LOG_LV_ERROR;
			break;
		case LOG_INFO:
		case LOG_WARNING:
		case LOG_NOTICE:
		case LOG_DEBUG:
			log_level = UPNP_LOG_LV_WARN;
			break;
		default:
			log_level = UPNP_LOG_LV_WARN;
			break;
	}

	va_start(ap, format);
	print_upnp_log(buf,log_level,format,ap);
	va_end(ap);
/*End:modified by c00127420 for upnp log to QXDM 1092205664*/	
	return 1;
}
