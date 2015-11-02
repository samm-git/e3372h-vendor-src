/*
  The oSIP library implements the Session Initiation Protocol (SIP -rfc3261-)
  Copyright (C) 2001,2002,2003  Aymeric MOIZARD jack@atosc.org
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/



/******************************************************************************
    <问题单号>        <作  者>  <修改时间>   <版本>      <修改描述>         
    3082101267  00201037   2013/8/23               时间整改
******************************************************************************/


#include <osip2/internal.h>
#include <osip2/osip.h>
#include <sys/sysinfo.h>
#include <time.h>

#include "fsm.h"

void
add_gettimeofday (struct timeval *atv, int ms)
{
  int m;
  atv->tv_usec += ms * 1000;
  m = atv->tv_usec / 1000000;
  atv->tv_usec = atv->tv_usec % 1000000;
  atv->tv_sec += m;
}

void
min_timercmp (struct timeval *tv1, struct timeval *tv2)
{
  if (tv2->tv_sec == -1)
    return;
  if (timercmp (tv1, tv2, >))
    {
      /* replace tv1 with tv2 info */
      tv1->tv_sec = tv2->tv_sec;
      tv1->tv_usec = tv2->tv_usec;
    }
}
/*BEGIN 3082101267 00201037 20130823 modified*/

int osip_gettimeofday ( struct timeval * tv , struct timezone * tz )
{
    struct sysinfo info = {0};
    int ret = 0;
    ret = sysinfo(&info);  
    if (0 == ret)
    {
        tv->tv_sec = info.uptime;
        tv->tv_usec = 0;
        return 0;
    }
    return -1;
}

time_t osip_time(time_t *t)
{
    struct sysinfo info = {0};
    int ret = 0;
    ret = sysinfo(&info);  
    if (0 == ret)
    {
        if (t)
        {
            *t = (time_t)info.uptime;
        }
        return (time_t)info.uptime;
    }
    return -1;
}

/*END 3082101267 00201037 20130823 modified*/

#ifdef WIN32

#include <time.h>
#include <sys/timeb.h>

int
gettimeofday (struct timeval *tp, void *tz)
{
  struct _timeb timebuffer;

  _ftime (&timebuffer);
  tp->tv_sec = timebuffer.time;
  tp->tv_usec = timebuffer.millitm * 1000;
  return 0;
}

#endif
