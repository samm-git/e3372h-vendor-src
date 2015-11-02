/* $Id: upnputils.c,v 1.3 2011/05/20 09:42:23 nanard Exp $ */
/* MiniUPnP project
 * http://miniupnp.free.fr/ or http://miniupnp.tuxfamily.org/
 * (c) 2006-2011 Thomas Bernard
 * This software is subject to the conditions detailed
 * in the LICENCE file provided within the distribution */
   /**********************问题单修改记录******************************************
    日期              修改人         问题单号           修改内容
  
 2012.08.29        z00203875     2082304944    UPnP认证测试
******************************************************************************/

#include "config.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#ifdef AF_LINK
#include <net/if_dl.h>
#endif
/* BEGIN 2082304944 zhoujianchun 00203875 2012.8.29 added */
#include <time.h>
#include <string.h>
/* END 2082304944 zhoujianchun 00203875 2012.8.29 added */
#include "upnputils.h"

int
sockaddr_to_string(const struct sockaddr * addr, char * str, size_t size)
{
	char buffer[64];
	unsigned short port = 0;
	int n = -1;

	switch(addr->sa_family)
	{
	case AF_INET6:
		inet_ntop(addr->sa_family,
		          &((struct sockaddr_in6 *)addr)->sin6_addr,
		          buffer, sizeof(buffer));
		port = ntohs(((struct sockaddr_in6 *)addr)->sin6_port);
		n = snprintf(str, size, "[%s]:%hu", buffer, port);
		break;
	case AF_INET:
		inet_ntop(addr->sa_family,
		          &((struct sockaddr_in *)addr)->sin_addr,
		          buffer, sizeof(buffer));
		port = ntohs(((struct sockaddr_in *)addr)->sin_port);
		n = snprintf(str, size, "%s:%hu", buffer, port);
		break;
#ifdef AF_LINK
	case AF_LINK:
		{
			struct sockaddr_dl * sdl = (struct sockaddr_dl *)addr;
			n = snprintf(str, size, "index=%hu type=%d %s",
			             sdl->sdl_index, sdl->sdl_type,
			             link_ntoa(sdl));
		}
		break;
#endif
	default:
		n = snprintf(str, size, "unknown address family %d", addr->sa_family);
#if 0
		n = snprintf(str, size, "unknown address family %d "
		             "%02x %02x %02x %02x %02x %02x %02x %02x",
		             addr->sa_family,
		             addr->sa_data[0], addr->sa_data[1], (unsigned)addr->sa_data[2], addr->sa_data[3],
		             addr->sa_data[4], addr->sa_data[5], (unsigned)addr->sa_data[6], addr->sa_data[7]);
#endif
	}
	return n;
}
/* BEGIN 2082304944 zhoujianchun 00203875 2012.8.29 added */
int get_formatted_date(char *buf)
{
    const char *rfc1123_fmt = "%a, %d %b %Y %H:%M:%S GMT";
    time_t now = 0;
    char date[MAX_DATE_LEN] = {0};

    if(NULL == buf)
    {
        NP_UPNP_ERROR("call get_formatted_date with null pointer.\n");
        return -1;
    }

    now = time(NULL);
    strftime(date, sizeof(date), rfc1123_fmt, gmtime(&now));
    strncpy(buf, date, strlen(date) + 1);

    return 0;
}
/* END 2082304944 zhoujianchun 00203875 2012.8.29 added */
