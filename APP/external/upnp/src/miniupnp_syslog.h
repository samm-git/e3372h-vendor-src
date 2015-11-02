#define LOG_ERR 1
#define LOG_INFO 2
#define LOG_WARNING 3
#define LOG_NOTICE 4
#define LOG_DEBUG 5


extern int global_log_level;
/*Begin:added by c0127420 for UPNP debug 1092205664*/
typedef enum _UPNP_LOG_LEVEL
{
    UPNP_LOG_LV_NONE  = 0,
    UPNP_LOG_LV_FATAL = 1,
    UPNP_LOG_LV_ERROR = 2,
    UPNP_LOG_LV_WARN  = 4,
    UPNP_LOG_LV_INFO  = 8,
    UPNP_LOG_LV_DEBG  = 16
} UPNP_LOG_LEVEL;
/*End:added by c0127420 for UPNP debug 1092205664*/
int syslog(int log_level, const char * format, ...);
