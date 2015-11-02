/* $Id: getconnstatus.c,v 1.4 2011/05/23 20:22:41 nanard Exp $ */
/* MiniUPnP project
 * http://miniupnp.free.fr/ or http://miniupnp.tuxfamily.org/
 * (c) 2011 Thomas Bernard 
 * This software is subject to the conditions detailed
 * in the LICENCE file provided within the distribution */
 
   /**********************问题单修改记录******************************************
    日期              修改人         问题单号           修改内容
  
 2012.03.14        z00203875     2031401440    修改打LOG方式
 2012.03.26        z00203875     2032603705    LOG打到SDT中
******************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include "getconnstatus.h"
#include "getifaddr.h"
#include "config.h"
#include "wanapi.h" /* for wan status structure */
#define STATUS_UNCONFIGURED (0)
#define STATUS_CONNECTING (1)
#define STATUS_CONNECTED (2)
#define STATUS_PENDINGDISCONNECT (3)
#define STATUS_DISCONNECTING (4)
#define STATUS_DISCONNECTED (5)

#ifndef USE_MOCKER
#include "appinterface.h"
#endif

/**
 * get the connection status
 * return values :
 *  0 - Unconfigured
 *  1 - Connecting
 *  2 - Connected
 *  3 - PendingDisconnect
 *  4 - Disconnecting
 *  5 - Disconnected */
int
get_wan_connection_status(const char * ifname)
{
    NP_UPNP_INFO("enter get_wan_connection_status\n");
	char addr[INET_ADDRSTRLEN];
	int r;
    static int i = 0;

#ifdef USE_MOCKER
    if(i%2 == 0)
        r = 5;
    else
        r = 2;
    i++;
    r = 4;
#else

    r = MgntGetWanState();
    NP_UPNP_DEBUG("MgntGetWanState() returns %d\n", r);
    switch(r)
    {
        case WAN_STATUS_UNCONFIGURED:
            NP_UPNP_DEBUG("MgntGetWanState() returns WAN_STATUS_UNCONFIGURED\n");
            NP_UPNP_DEBUG("WAN_STATUS_UNCONFIGURED maps to UPNP_UNCONFIGURED\n");
            r = UPNP_UNCONFIGURED;
            break;

        case WAN_STATUS_CONNECTING:
            NP_UPNP_DEBUG("MgntGetWanState() returns WAN_STATUS_CONNECTING\n");
            NP_UPNP_DEBUG("WAN_STATUS_CONNECTING maps to UPNP_CONNECTING\n");
            r = UPNP_CONNECTING;
            break;

        case WAN_STATUS_AUTHENTICATING:
            NP_UPNP_DEBUG("MgntGetWanState() returns WAN_STATUS_AUTHENTICATING\n");
            NP_UPNP_DEBUG("WAN_STATUS_AUTHENTICATING maps to UPNP_CONNECTING\n");
            r = UPNP_CONNECTING;
            break;

        case WAN_STATUS_CONNECTED:
            NP_UPNP_DEBUG("MgntGetWanState() returns WAN_STATUS_CONNECTED\n");
            NP_UPNP_DEBUG("WAN_STATUS_CONNECTED maps to UPNP_CONNECTED\n");
            r = UPNP_CONNECTED;
            break;

        case WAN_STATUS_DISCONNECTING:
            NP_UPNP_DEBUG("MgntGetWanState() returns WAN_STATUS_DISCONNECTING\n");
            NP_UPNP_DEBUG("WAN_STATUS_CONNECTED maps to UPNP_DISCONNECTINGD\n");
            r = UPNP_DISCONNECTING;
            break;

        case WAN_STATUS_TIMEOUT:
            NP_UPNP_DEBUG("MgntGetWanState() returns WAN_STATUS_TIMEOUT\n");
            NP_UPNP_DEBUG("WAN_STATUS_CONNECTED maps to UPNP_DISCONNECTED\n");
            r = UPNP_DISCONNECTED;
            break;

        case WAN_STATUS_DISCONNECTED:
            NP_UPNP_DEBUG("MgntGetWanState() returns WAN_STATUS_DISCONNECTED\n");
            NP_UPNP_DEBUG("WAN_STATUS_DISCONNECTED maps to UPNP_DISCONNECTED\n");
            r = UPNP_DISCONNECTED;
            break;

        case WAN_STATUS_PENDING:
            NP_UPNP_DEBUG("MgntGetWanState() returns WAN_STATUS_PENDING\n");
            NP_UPNP_DEBUG("WAN_STATUS_PENDING maps to UPNP_DISCONNECTED\n");
            r = UPNP_DISCONNECTED;
            break;

        default:
            NP_UPNP_DEBUG("MgntGetWanState() returns WAN_STATUS_UNCONFIGRUED\n");
            NP_UPNP_DEBUG("WAN_STATUS_UNCONFIGURED maps to UPNP_UNCONFIGURED\n");
            r = UPNP_UNCONFIGURED;
            break;
    }
#endif
	NP_UPNP_DEBUG("get_wan_conncetion_status = %d\n", r);
	NP_UPNP_INFO("leave get_wan_connection_status\n");
	return r;
}

/**
 * return the same value as get_wan_connection_status()
 * as a C string */
const char *
get_wan_connection_status_str(const char * ifname)
{
	int status;
	const char * str = NULL;

	status = get_wan_connection_status(ifname);
	switch(status) {
	case 0:
		str = "Unconfigured";
		break;
	case 1:
		str = "Connecting";
		break;
	case 2:
		str = "Connected";
		break;
	case 3:
		str = "PendingDisconnect";
		break;
	case 4:
		str = "Disconnecting";
		break;
	case 5:
		str = "Disconnected";
		break;
	default:
		str = "Unconfigured";
		break;
	}
	return str;
}

