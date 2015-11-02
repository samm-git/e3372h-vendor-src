/*
 * Copyright 2011 Daniel Drown
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * logging.c - print a log message
 */

#include <stdarg.h>
#include <android/log.h>

#include "logging.h"
#include "debug.h"

/* function: logmsg
 * prints a log message to android's log buffer
 * prio - the log message priority
 * fmt  - printf format specifier
 * ...  - printf format arguments
 */
void logmsg(int prio, const char *fmt, ...) 
{
#if 0
  va_list ap;

  va_start(ap, fmt);
  __android_log_vprint(prio, "clatd", fmt, ap);
  va_end(ap);
#endif  
    /* 改造logmsg函数打印到终端 m00172998 20130725 */ 
    char buf[2048] = {0};
    va_list args;
    
    va_start (args, fmt);
    vsnprintf (buf, sizeof (buf), fmt, args);
    va_end (args);
    
    printf("\r\n[clat_debug] %s\r\n", buf);
    return;
}

/* function: logmsg_dbg
 * prints a log message to android's log buffer if CLAT_DEBUG is set
 * prio - the log message priority
 * fmt  - printf format specifier
 * ...  - printf format arguments
 */
void logmsg_dbg(int prio, const char *fmt, ...) 
{
#if CLAT_DEBUG
    #if 0
        va_list ap;

        va_start(ap, fmt);
        __android_log_vprint(prio, "clatd", fmt, ap);
        va_end(ap);
    #endif
    /* 改造logmsg函数打印到终端 m00172998 20130725 */ 
    char buf[2048] = {0};
    va_list args;
    
    va_start (args, fmt);
    vsnprintf (buf, sizeof (buf), fmt, args);
    va_end (args);
    
    printf("\r\n[clat_packet] %s\r\n", buf);
    return;

#endif
}
