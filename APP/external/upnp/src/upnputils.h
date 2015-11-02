/* $Id: upnputils.h,v 1.1 2011/05/15 08:58:41 nanard Exp $ */
/* MiniUPnP project
 * http://miniupnp.free.fr/ or http://miniupnp.tuxfamily.org/
 * (c) 2011 Thomas Bernard
 * This software is subject to the conditions detailed
 * in the LICENCE file provided within the distribution */
   /**********************问题单修改记录******************************************
    日期              修改人         问题单号           修改内容
  
 2012.08.29        z00203875     2082304944    UPnP认证测试
******************************************************************************/

#ifndef __UPNPUTILS_H__
#define __UPNPUTILS_H__
/* BEGIN 2082304944 zhoujianchun 00203875 2012.8.29 added */
#define MAX_DATE_LEN 64
/* END 2082304944 zhoujianchun 00203875 2012.8.29 added */
/**
 * convert a struct sockaddr to a human readable string.
 * [ipv6]:port or ipv4:port
 * return the number of characters used (as snprintf)
 */
int
sockaddr_to_string(const struct sockaddr * addr, char * str, size_t size);
/* BEGIN 2082304944 zhoujianchun 00203875 2012.8.29 added */
/* date_buf should be more than 32 bytes long */
int get_formatted_date(char *date_buf);
/* END 2082304944 zhoujianchun 00203875 2012.8.29 added */
#endif

