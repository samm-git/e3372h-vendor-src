/* $Id: upnphttp.c,v 1.64 2011/11/18 11:21:17 nanard Exp $ */
/* Project :  miniupnp
 * Website :  http://miniupnp.free.fr/ or http://miniupnp.tuxfamily.org/
 * Author :   Thomas Bernard
 * Copyright (c) 2005-2011 Thomas Bernard
 * This software is subject to the conditions detailed in the
 * LICENCE file included in this distribution.
 * */
 
   /**********************问题单修改记录******************************************
    日期              修改人         问题单号           修改内容
  
 2012.03.14        z00203875     2031401440    修改打LOG方式
 2012.03.26        z00203875     2032603705    LOG打到SDT中    
 2012.08.29        z00203875     2082304944    UPnP认证测试
 2012.09.29        z00203875     2092807655    WIN7系统图标无拨号菜单
 2013.08.22        y00248130     3082010042    解决Fortify告警
******************************************************************************/
 
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/param.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include "config.h"
#include "upnphttp.h"
#include "upnpdescgen.h"
#include "miniupnpdpath.h"
#include "upnpsoap.h"
#include "upnpevents.h"
/* BEGIN 2082304944 zhoujianchun 00203875 2012.8.29 added */
#include "upnputils.h"
/* END   2082304944 zhoujianchun 00203875 2012.8.29 added */

struct upnphttp * 
New_upnphttp(int s)
{
	struct upnphttp * ret;
	if(s<0)
		return NULL;
	ret = (struct upnphttp *)malloc(sizeof(struct upnphttp));
	if(ret == NULL)
		return NULL;
	memset(ret, 0, sizeof(struct upnphttp));
	ret->socket = s;
	return ret;
}

void
CloseSocket_upnphttp(struct upnphttp * h)
{
	if(close(h->socket) < 0)
	{
		NP_UPNP_ERROR("CloseSocket_upnphttp: close(%d): %s\n", h->socket, strerror(errno));
	}
	h->socket = -1;
	h->state = 100;
}

void
Delete_upnphttp(struct upnphttp * h)
{
	if(h)
	{
		if(h->socket >= 0)
			CloseSocket_upnphttp(h);
		if(h->req_buf)
			free(h->req_buf);
		if(h->res_buf)
			free(h->res_buf);
		free(h);
	}
}

/* parse HttpHeaders of the REQUEST */
static void
ParseHttpHeaders(struct upnphttp * h)
{
	char * line;
	char * colon;
	char * p;
	int n;
	line = h->req_buf;
	/* TODO : check if req_buf, contentoff are ok */
	while(line < (h->req_buf + h->req_contentoff))
	{
		colon = strchr(line, ':');
		if(colon)
		{
			if(strncasecmp(line, "Content-Length", 14)==0)
			{
				p = colon;
				while(*p < '0' || *p > '9')
					p++;
				h->req_contentlen = atoi(p);
				/*printf("*** Content-Lenght = %d ***\n", h->req_contentlen);
				printf("    readbufflen=%d contentoff = %d\n",
					h->req_buflen, h->req_contentoff);*/
			}
			else if(strncasecmp(line, "SOAPAction", 10)==0)
			{
				p = colon;
				n = 0;
				while(*p == ':' || *p == ' ' || *p == '\t')
					p++;
				while(p[n]>=' ')
				{
					n++;
				}
				if((p[0] == '"' && p[n-1] == '"')
				  || (p[0] == '\'' && p[n-1] == '\''))
				{
					p++; n -= 2;
				}
				h->req_soapAction = p;
				h->req_soapActionLen = n;
			}
#ifdef ENABLE_EVENTS
			else if(strncasecmp(line, "Callback", 8)==0)
			{
				p = colon;
				while(*p != '<' && *p != '\r' )
					p++;
				n = 0;
				while(p[n] != '>' && p[n] != '\r' )
					n++;
				h->req_Callback = p + 1;
				h->req_CallbackLen = MAX(0, n - 1);
			}
			else if(strncasecmp(line, "SID", 3)==0)
			{
				p = colon + 1;
				while(isspace(*p))
					p++;
				n = 0;
				while(!isspace(p[n]))
					n++;
				h->req_SID = p;
				h->req_SIDLen = n;
			}
			/* Timeout: Seconds-nnnn */
/* TIMEOUT
Recommended. Requested duration until subscription expires,
either number of seconds or infinite. Recommendation
by a UPnP Forum working committee. Defined by UPnP vendor.
 Consists of the keyword "Second-" followed (without an
intervening space) by either an integer or the keyword "infinite". */
			else if(strncasecmp(line, "Timeout", 7)==0)
			{
				p = colon + 1;
				while(isspace(*p))
					p++;
				if(strncasecmp(p, "Second-", 7)==0) {
					h->req_Timeout = atoi(p+7);
				}
			}
#endif
		}
		while(!(line[0] == '\r' && line[1] == '\n'))
			line++;
		line += 2;
	}
}

/* very minimalistic 404 error message */
static void
Send404(struct upnphttp * h)
{
/*
	static const char error404[] = "HTTP/1.1 404 Not found\r\n"
		"Connection: close\r\n"
		"Content-type: text/html\r\n"
		"\r\n"
		"<HTML><HEAD><TITLE>404 Not Found</TITLE></HEAD>"
		"<BODY><H1>Not Found</H1>The requested URL was not found"
		" on this server.</BODY></HTML>\r\n";
	int n;
	n = send(h->socket, error404, sizeof(error404) - 1, 0);
	if(n < 0)
	{
	}*/
	static const char body404[] =
		"<HTML><HEAD><TITLE>404 Not Found</TITLE></HEAD>"
		"<BODY><H1>Not Found</H1>The requested URL was not found"
		" on this server.</BODY></HTML>\r\n";
	h->respflags = FLAG_HTML;
	BuildResp2_upnphttp(h, 404, "Not Found",
	                    body404, sizeof(body404) - 1);
	SendRespAndClose_upnphttp(h);
}

/* very minimalistic 501 error message */
static void
Send501(struct upnphttp * h)
{
/*
	static const char error501[] = "HTTP/1.1 501 Not Implemented\r\n"
		"Connection: close\r\n"
		"Content-type: text/html\r\n"
		"\r\n"
		"<HTML><HEAD><TITLE>501 Not Implemented</TITLE></HEAD>"
		"<BODY><H1>Not Implemented</H1>The HTTP Method "
		"is not implemented by this server.</BODY></HTML>\r\n";
	int n;
	n = send(h->socket, error501, sizeof(error501) - 1, 0);
	if(n < 0)
	{
	}
*/
	static const char body501[] = 
		"<HTML><HEAD><TITLE>501 Not Implemented</TITLE></HEAD>"
		"<BODY><H1>Not Implemented</H1>The HTTP Method "
		"is not implemented by this server.</BODY></HTML>\r\n";
	h->respflags = FLAG_HTML;
	BuildResp2_upnphttp(h, 501, "Not Implemented",
	                    body501, sizeof(body501) - 1);
	SendRespAndClose_upnphttp(h);
}

static const char *
findendheaders(const char * s, int len)
{
	while(len-->0)
	{
		if(s[0]=='\r' && s[1]=='\n' && s[2]=='\r' && s[3]=='\n')
			return s;
		s++;
	}
	return NULL;
}

#ifdef HAS_DUMMY_SERVICE
static void
sendDummyDesc(struct upnphttp * h)
{
	static const char xml_desc[] = "<?xml version=\"1.0\"?>\r\n"
		"<scpd xmlns=\"urn:schemas-upnp-org:service-1-0\">"
		" <specVersion>"
		"    <major>1</major>"
		"    <minor>0</minor>"
		"  </specVersion>"
		"  <actionList />"
		"  <serviceStateTable />"
		"</scpd>\r\n";
	BuildResp_upnphttp(h, xml_desc, sizeof(xml_desc)-1);
	SendRespAndClose_upnphttp(h);
}
#endif

/* Sends the description generated by the parameter */
static void
sendXMLdesc(struct upnphttp * h, char * (f)(int *))
{
	char * desc;
	int len;
	desc = f(&len);
	if(!desc)
	{
		static const char error500[] = "<HTML><HEAD><TITLE>Error 500</TITLE>"
		   "</HEAD><BODY>Internal Server Error</BODY></HTML>\r\n";
		NP_UPNP_ERROR("Failed to generate XML description\n");
		h->respflags = FLAG_HTML;
		BuildResp2_upnphttp(h, 500, "Internal Server Error",
		                    error500, sizeof(error500)-1);
	}
	else
	{
		BuildResp_upnphttp(h, desc, len);
	}
	SendRespAndClose_upnphttp(h);
	free(desc);
}

/* ProcessHTTPPOST_upnphttp()
 * executes the SOAP query if it is possible */
static void
ProcessHTTPPOST_upnphttp(struct upnphttp * h)
{
	if((h->req_buflen - h->req_contentoff) >= h->req_contentlen)
	{
		if(h->req_soapAction)
		{
			/* we can process the request */
			NP_UPNP_DEBUG("SOAPAction: %.*s", h->req_soapActionLen, h->req_soapAction);
			ExecuteSoapAction(h, 
				h->req_soapAction,
				h->req_soapActionLen);
		}
		else
		{
			static const char err400str[] =
				"<html><body>Bad request</body></html>";
			NP_UPNP_DEBUG("No SOAPAction in HTTP headers\n");
			h->respflags = FLAG_HTML;
			BuildResp2_upnphttp(h, 400, "Bad Request",
			                    err400str, sizeof(err400str) - 1);
			SendRespAndClose_upnphttp(h);
		}
	}
	else
	{
		/* waiting for remaining data */
		h->state = 1;
	}
}

#ifdef ENABLE_EVENTS
/**
 * returns 0 if the callback header value is not valid
 * 1 if it is valid.
 */
static int
checkCallbackURL(struct upnphttp * h)
{
	char addrstr[48];
	int ipv6;
	const char * p;
	int i;

	if(!h->req_Callback || h->req_CallbackLen < 8)
		return 0;
	if(memcmp(h->req_Callback, "http://", 7) != 0)
		return 0;
	ipv6 = 0;
	i = 0;
	p = h->req_Callback + 7;
	if(*p == '[') {
		p++;
		ipv6 = 1;
		while(*p != ']' && i < (int)(sizeof(addrstr)-1)
		      && p < (h->req_Callback + h->req_CallbackLen))
			addrstr[i++] = *(p++);
	} else {
		while(*p != '/' && *p != ':' && i < (int)(sizeof(addrstr)-1)
		      && p < (h->req_Callback + h->req_CallbackLen))
			addrstr[i++] = *(p++);
	}
	addrstr[i] = '\0';
	if(ipv6) {
		struct in6_addr addr;
		if(inet_pton(AF_INET6, addrstr, &addr) <= 0)
			return 0;
#ifdef ENABLE_IPV6
		if(!h->ipv6
		  || (0!=memcmp(&addr, &(h->clientaddr_v6), sizeof(struct in6_addr))))
			return 0;
#else
		return 0;
#endif
	} else {
		struct in_addr addr;
		if(inet_pton(AF_INET, addrstr, &addr) <= 0)
			return 0;
#ifdef ENABLE_IPV6
		if(h->ipv6) {
			if(!IN6_IS_ADDR_V4MAPPED(&(h->clientaddr_v6)))
				return 0;
			if(0!=memcmp(&addr, ((const char *)&(h->clientaddr_v6) + 12), 4))
				return 0;
		} else {
			if(0!=memcmp(&addr, &(h->clientaddr), sizeof(struct in_addr)))
				return 0;
		}
#else
		if(0!=memcmp(&addr, &(h->clientaddr), sizeof(struct in_addr)))
			return 0;
#endif
	}
	return 1;
}

static void
ProcessHTTPSubscribe_upnphttp(struct upnphttp * h, const char * path)
{
	const char * sid;
    /* BEGIN 2082304944 zhoujianchun 00203875 2012.8.29 added */
    int i = 0;
    int has_nt = 0;
    char *ptr = h->req_buf;
    /* END   2082304944 zhoujianchun 00203875 2012.8.29 added */
	NP_UPNP_DEBUG("ProcessHTTPSubscribe %s\n", path);
	NP_UPNP_DEBUG("Callback '%.*s' Timeout=%d\n",
	       h->req_CallbackLen, h->req_Callback, h->req_Timeout);
	NP_UPNP_DEBUG("SID '%.*s'\n", h->req_SIDLen, h->req_SID);
    /* BEGIN 2082304944 zhoujianchun 00203875 2012.8.29 added */
    for(i = 0; i < h->req_contentoff; i++)
    {
        /* BEGIN 2092807655 zhoujianchun 00203875 2012.9.28 modified */
        if((0 == strncasecmp(ptr + i, "NT:", strlen("NT:")))
                &&(('\r' == (char)(*(ptr + i - 1))) || ('\n' == (char)(*(ptr + i - 1)))))
        /* END  2092807655 zhoujianchun 00203875 2012.9.28 modified */
        {
            has_nt = 1;
            ptr = ptr + i + strlen("NT:");
            while(isspace((int)*ptr))
            {
                ptr++;
            }
            if(0 != strncasecmp(ptr, "upnp:event", strlen("upnp:event")))
            {
                NP_UPNP_ERROR("Subscrible with wrong NT format, should be \"NT: upnp:event\".\n");
		        BuildResp2_upnphttp(h, 412, "Precondition Failed", 0, 0);
		        SendRespAndClose_upnphttp(h);
                return;
            }
            break;
        }
    }
    if(h->req_SID && (h->req_Callback || has_nt))
    {
		BuildResp2_upnphttp(h, 400, "Incompatible header fields", 0, 0);
		SendRespAndClose_upnphttp(h);
    }
    /* END   2082304944 zhoujianchun 00203875 2012.8.29 added */
	if(!h->req_Callback && !h->req_SID) {
		/* Missing or invalid CALLBACK : 412 Precondition Failed.
		 * If CALLBACK header is missing or does not contain a valid HTTP URL,
		 * the publisher must respond with HTTP error 412 Precondition Failed*/
		BuildResp2_upnphttp(h, 412, "Precondition Failed", 0, 0);
		SendRespAndClose_upnphttp(h);
	} else {
	/* - add to the subscriber list
	 * - respond HTTP/x.x 200 OK 
	 * - Send the initial event message */
/* Server:, SID:; Timeout: Second-(xx|infinite) */
	/* Check that the callback URL is on the same IP as
	 * the request, and not on the internet, nor on ourself (DOS attack ?) */
		if(h->req_Callback) {
			if(checkCallbackURL(h)) {
				sid = upnpevents_addSubscriber(path, h->req_Callback,
				                               h->req_CallbackLen, h->req_Timeout);
				h->respflags = FLAG_TIMEOUT;
				if(sid) {
					NP_UPNP_DEBUG("generated sid=%s\n", sid);
					h->respflags |= FLAG_SID;
					h->req_SID = sid;
					h->req_SIDLen = strlen(sid);
				}
				BuildResp_upnphttp(h, 0, 0);
			} else {
				NP_UPNP_WARN("Invalid Callback in SUBSCRIBE %.*s\n",
	       		       h->req_CallbackLen, h->req_Callback);
				BuildResp2_upnphttp(h, 412, "Precondition Failed", 0, 0);
			}
		} else {
			/* subscription renew */
			/* Invalid SID
412 Precondition Failed. If a SID does not correspond to a known,
un-expired subscription, the publisher must respond
with HTTP error 412 Precondition Failed. */
			if(renewSubscription(h->req_SID, h->req_SIDLen, h->req_Timeout) < 0) {
				BuildResp2_upnphttp(h, 412, "Precondition Failed", 0, 0);
			} else {
				h->respflags = FLAG_TIMEOUT;
				BuildResp_upnphttp(h, 0, 0);
			}
		}
		SendRespAndClose_upnphttp(h);
	}
}

static void
ProcessHTTPUnSubscribe_upnphttp(struct upnphttp * h, const char * path)
{
    /* BEGIN 2082304944 zhoujianchun 00203875 2012.8.29 added */
    int i = 0;
    char *ptr = NULL;
    ptr = h->req_buf;
    /* END   2082304944 zhoujianchun 00203875 2012.8.29 added */
	NP_UPNP_DEBUG("ProcessHTTPUnSubscribe %s\n", path);
	NP_UPNP_DEBUG("SID '%.*s'\n", h->req_SIDLen, h->req_SID);
    /* BEGIN 2082304944 zhoujianchun 00203875 2012.8.29 added */
    for(i = 0; i < h->req_contentoff; i++)
    {
        if((0 == strncasecmp(ptr + i, "NT", strlen("NT"))) 
            || (0 == strncasecmp(ptr + i, "CALLBACK", strlen("CALLBACK"))))
        {
            /* BEGIN 2092807655 zhoujianchun 00203875 2012.9.28 modified */
            if(('\r' == (char)(*(ptr + i - 1))) || ('\n' == (char)(*(ptr + i - 1))))
            {
                NP_UPNP_ERROR("UnSubscribe with NT or CALLBACK element is not allowed.\n");
		        BuildResp2_upnphttp(h, 400, "Incompatible header fields", 0, 0);
		        SendRespAndClose_upnphttp(h);
                return;
            }
            /* END   2092807655 zhoujianchun 00203875 2012.9.28 modified */
        }
    }
    /* END   2082304944 zhoujianchun 00203875 2012.8.29 added */
	/* Remove from the list */
	if(upnpevents_removeSubscriber(h->req_SID, h->req_SIDLen) < 0) {
		BuildResp2_upnphttp(h, 412, "Precondition Failed", 0, 0);
	} else {
		BuildResp_upnphttp(h, 0, 0);
	}
	SendRespAndClose_upnphttp(h);
}
#endif

/* Parse and process Http Query 
 * called once all the HTTP headers have been received. */
static void
ProcessHttpQuery_upnphttp(struct upnphttp * h)
{
	char HttpCommand[16];
	char HttpUrl[128];
	char * HttpVer;
	char * p;
	int i;
	p = h->req_buf;
	if(!p)
		return;
	for(i = 0; i<15 && *p != ' ' && *p != '\r'; i++)
		HttpCommand[i] = *(p++);
	HttpCommand[i] = '\0';
	while(*p==' ')
		p++;
	for(i = 0; i<127 && *p != ' ' && *p != '\r'; i++)
		HttpUrl[i] = *(p++);
	HttpUrl[i] = '\0';
	while(*p==' ')
		p++;
	HttpVer = h->HttpVer;
	for(i = 0; i<15 && *p != '\r'; i++)
		HttpVer[i] = *(p++);
	HttpVer[i] = '\0';
	NP_UPNP_DEBUG("HTTP REQUEST : %s %s (%s)\n",
	       HttpCommand, HttpUrl, HttpVer);
	ParseHttpHeaders(h);
	if(strcmp("POST", HttpCommand) == 0)
	{
		h->req_command = EPost;
		ProcessHTTPPOST_upnphttp(h);
	}
	else if(strcmp("GET", HttpCommand) == 0)
	{
		h->req_command = EGet;
		if(strcasecmp(ROOTDESC_PATH, HttpUrl) == 0)
		{
			sendXMLdesc(h, genRootDesc);
		}
		else if(strcasecmp(WANIPC_PATH, HttpUrl) == 0)
		{
			sendXMLdesc(h, genWANIPCn);
		}
		else if(strcasecmp(WANCFG_PATH, HttpUrl) == 0)
		{
			sendXMLdesc(h, genWANCfg);
		}
#ifdef HAS_DUMMY_SERVICE
		else if(strcasecmp(DUMMY_PATH, HttpUrl) == 0)
		{
			sendDummyDesc(h);
		}
#endif
#ifdef ENABLE_L3F_SERVICE
		else if(strcasecmp(L3F_PATH, HttpUrl) == 0)
		{
			sendXMLdesc(h, genL3F);
		}
#endif
#ifdef ENABLE_6FC_SERVICE
		else if(strcasecmp(WANIP6FC_PATH, HttpUrl) == 0)
		{
			sendXMLdesc(h, gen6FC);
		}
#endif
#ifdef ENABLE_DP_SERVICE
		else if(strcasecmp(DP_PATH, HttpUrl) == 0)
		{
			sendXMLdesc(h, genDP);
		}
#endif
		else
		{
			NP_UPNP_WARN("%s not found, responding ERROR 404\n", HttpUrl);
			Send404(h);
		}
	}
#ifdef ENABLE_EVENTS
	else if(strcmp("SUBSCRIBE", HttpCommand) == 0)
	{
		h->req_command = ESubscribe;
		ProcessHTTPSubscribe_upnphttp(h, HttpUrl);
	}
	else if(strcmp("UNSUBSCRIBE", HttpCommand) == 0)
	{
		h->req_command = EUnSubscribe;
		ProcessHTTPUnSubscribe_upnphttp(h, HttpUrl);
	}
#else
	else if(strcmp("SUBSCRIBE", HttpCommand) == 0)
	{
		NP_UPNP_WARN("SUBSCRIBE not implemented. ENABLE_EVENTS compile option disabled\n");
		Send501(h);
	}
#endif
	else
	{
		NP_UPNP_WARN("Unsupported HTTP Command %s\n", HttpCommand);
		Send501(h);
	}
}


void
Process_upnphttp(struct upnphttp * h)
{
	char buf[2048];
	int n;
    /*BEGIN 3082010042 y00248130 2013-8-22 added/modified*/
    char * p_return = NULL;     
    /* END  3082010042 y00248130 2013-8-22 added/modified*/
	if(!h)
		return;
	switch(h->state)
	{
	case 0:
		n = recv(h->socket, buf, 2048, 0);
		if(n<0)
		{
			NP_UPNP_ERROR("recv (state0): %s\n", strerror(errno));
			h->state = 100;
		}
		else if(n==0)
		{
			NP_UPNP_WARN("HTTP Connection closed unexpectedly\n");
			h->state = 100;
		}
		else
		{
			const char * endheaders;
			/* if 1st arg of realloc() is null,
			 * realloc behaves the same as malloc() */
			h->req_buf = (char *)realloc(h->req_buf, n + h->req_buflen + 1);
			memcpy(h->req_buf + h->req_buflen, buf, n);
			h->req_buflen += n;
			h->req_buf[h->req_buflen] = '\0';
			/* search for the string "\r\n\r\n" */
			endheaders = findendheaders(h->req_buf, h->req_buflen);
			if(endheaders)
			{
				h->req_contentoff = endheaders - h->req_buf + 4;
				ProcessHttpQuery_upnphttp(h);
			}
		}
		break;
	case 1:
		n = recv(h->socket, buf, 2048, 0);
		if(n<0)
		{
			NP_UPNP_ERROR("recv (state1): %s\n", strerror(errno));
			h->state = 100;
		}
		else if(n==0)
		{
			NP_UPNP_WARN("HTTP Connection closed inexpectedly\n");
			h->state = 100;
		}
		else
		{
			/*fwrite(buf, 1, n, stdout);*/	/* debug */

            /*BEGIN 3082010042 y00248130 2013-8-22 added/modified*/
            p_return = (char *)realloc(h->req_buf, (n + h->req_buflen + 1));
            if (NULL == p_return)
            {
                NP_UPNP_ERROR("cannot alloc mem for request.\n");
                return;
            }
            h->req_buf = p_return;
            *(h->req_buf + h->req_buflen + n) = '\0';
            /* END  3082010042 y00248130 2013-8-22 added/modified*/
			memcpy(h->req_buf + h->req_buflen, buf, n);
			h->req_buflen += n;
			if((h->req_buflen - h->req_contentoff) >= h->req_contentlen)
			{
				ProcessHTTPPOST_upnphttp(h);
			}
		}
		break;
	default:
		NP_UPNP_WARN("Unexpected state: %d\n", h->state);
	}
}
/* BEGIN 2082304944 zhoujianchun 00203875 2012.8.29 added */
static const char httpresphead_content_language[] =
	"%s %d %s\r\n"
	"Content-Type: %s\r\n"
	"Connection: close\r\n"
	//"Connection: Keep-Alive\r\n"
	"Content-Length: %d\r\n"
	"Server: " MINIUPNPD_SERVER_STRING "\r\n"
    "CONTENT-LANGUAGE: en\r\n"
    "EXT:\r\n"
    "DATE: %s\r\n"
	;	/*"\r\n";*/
/* END   2082304944 zhoujianchun 00203875 2012.8.29 added */
static const char httpresphead[] =
	"%s %d %s\r\n"
	//"Content-Type: text/xml; charset=\"utf-8\"\r\n"
	//"Content-Type: %s; charset=\"utf-8\"\r\n"
	"Content-Type: %s\r\n"
	"Connection: close\r\n"
	//"Connection: Keep-Alive\r\n"
	"Content-Length: %d\r\n"
	"Server: " MINIUPNPD_SERVER_STRING "\r\n"
    /* BEGIN 2082304944 zhoujianchun 00203875 2012.8.29 added */
    "EXT:\r\n"
    "DATE: %s\r\n"
    /* END   2082304944 zhoujianchun 00203875 2012.8.29 added */
	;	/*"\r\n";*/
/*
		"<?xml version=\"1.0\"?>\n"
		"<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" "
		"s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">"
		"<s:Body>"

		"</s:Body>"
		"</s:Envelope>";
*/
/* with response code and response message
 * also allocate enough memory */

void
BuildHeader_upnphttp(struct upnphttp * h, int respcode,
                     const char * respmsg,
                     int bodylen)
{
	int templen;
    /* BEGIN 2082304944 zhoujianchun 00203875 2012.8.29 added */
    int i = 0;
    char has_accept_language = 0;
    char *ptr = h->req_buf;
    char date[MAX_DATE_LEN] = {0};
    NP_UPNP_DEBUG("h->req_buf is :%s\n", h->req_buf);
    NP_UPNP_DEBUG("h->req_contentoff is :%d\n", h->req_contentoff);
    NP_UPNP_DEBUG("h->req_buflen is :%d\n", h->req_buflen);
    get_formatted_date(date);
    for(i = 0; i < h->req_contentoff; i++)
    {
        if(0 == strncasecmp(ptr + i, "ACCEPT-LANGUAGE", strlen("ACCEPT-LANGUAGE")))
        {
            /* BEGIN 2092807655 zhoujianchun 00203875 2012.9.28 modified */
            if(('\r' == (char)(*(ptr + i - 1))) || ('\n' == (char)(*(ptr + i - 1))))
            {
                has_accept_language = 1;
            }
            /* END   2092807655 zhoujianchun 00203875 2012.9.28 modified */
        }
    }
    /* END   2082304944 zhoujianchun 00203875 2012.8.29 added */
	if(!h->res_buf)
	{
    /* BEGIN 2082304944 zhoujianchun 00203875 2012.8.29 added */
        if(1 == has_accept_language)
        {
            NP_UPNP_DEBUG("HAS ACCEPT-LANGUAGE\n");
            templen = sizeof(httpresphead_content_language) + 256 + bodylen;
        }
        else
        {
            NP_UPNP_DEBUG("DO NOT HAVE ACCEPT-LANGUAGE\n");
            templen = sizeof(httpresphead) + 256 + bodylen;
        }
    /* END   2082304944 zhoujianchun 00203875 2012.8.29 added */
		h->res_buf = (char *)malloc(templen);
		if(!h->res_buf)
		{
			NP_UPNP_ERROR("malloc error in BuildHeader_upnphttp()\n");
			return;
		}
		h->res_buf_alloclen = templen;
	}
	h->res_buflen = snprintf(h->res_buf, h->res_buf_alloclen,
    /* BEGIN 2082304944 zhoujianchun 00203875 2012.8.29 modified */
	                         has_accept_language ? httpresphead_content_language : httpresphead, h->HttpVer,
	                         respcode, respmsg,
	                         (h->respflags&FLAG_HTML)?"text/html":"text/xml; charset=\"utf-8\"",
							 bodylen, date);
	/* Additional headers */
#ifdef ENABLE_EVENTS
	if(h->respflags & FLAG_TIMEOUT) {
		h->res_buflen += snprintf(h->res_buf + h->res_buflen,
		                          h->res_buf_alloclen - h->res_buflen,
		                          "Timeout: Second-");
		if(h->req_Timeout) {
			h->res_buflen += snprintf(h->res_buf + h->res_buflen,
			                          h->res_buf_alloclen - h->res_buflen,
			                          "%d\r\n", h->req_Timeout);
		} else {
			h->res_buflen += snprintf(h->res_buf + h->res_buflen,
			                          h->res_buf_alloclen - h->res_buflen,
			                          "1800\r\n");
		}
	}
	if(h->respflags & FLAG_SID) {
		h->res_buflen += snprintf(h->res_buf + h->res_buflen,
		                          h->res_buf_alloclen - h->res_buflen,
		                          "SID: %s\r\n", h->req_SID);
	}
#endif
    /* END   2082304944 zhoujianchun 00203875 2012.8.29 modified */
	h->res_buf[h->res_buflen++] = '\r';
	h->res_buf[h->res_buflen++] = '\n';
	if(h->res_buf_alloclen < (h->res_buflen + bodylen))
	{
		char * tmp;
		tmp = (char *)realloc(h->res_buf, (h->res_buflen + bodylen));
		if(tmp)
		{
			h->res_buf = tmp;
			h->res_buf_alloclen = h->res_buflen + bodylen;
		}
		else
		{
			NP_UPNP_ERROR("realloc error in BuildHeader_upnphttp()\n");
		}
	}
}

void
BuildResp2_upnphttp(struct upnphttp * h, int respcode,
                    const char * respmsg,
                    const char * body, int bodylen)
{
	BuildHeader_upnphttp(h, respcode, respmsg, bodylen);
	if(body)
		memcpy(h->res_buf + h->res_buflen, body, bodylen);
	h->res_buflen += bodylen;
}

/* responding 200 OK ! */
void
BuildResp_upnphttp(struct upnphttp * h,
                        const char * body, int bodylen)
{
	BuildResp2_upnphttp(h, 200, "OK", body, bodylen);
}

void
SendRespAndClose_upnphttp(struct upnphttp * h)
{
#ifdef DEBUG_SHOW_HTTP_PACKET
    NP_UPNP_INFO("enter SendRespAndClose_upnp_http.\n");
    NP_UPNP_DEBUG("res_buflen is %d\n", h->res_buflen);
    NP_UPNP_DEBUG("resp buf is:\n");
    (h->res_buf)[h->res_buflen] = '\0';
    NP_UPNP_DEBUG("%s\n", h->res_buf);
    NP_UPNP_DEBUG("end of res_buf\n");
#endif
	char * p;
	ssize_t n;
	size_t len;
	p = h->res_buf;
	len = h->res_buflen;
	while (len > 0)
	{
		n = send(h->socket, p, len, 0);
		if(n<0)
		{
			NP_UPNP_ERROR("send(res_buf): %s\n", strerror(errno));
			if (errno != EINTR)
				break; /* avoid infinite loop */
		}
		else if(n == 0)
		{
			NP_UPNP_ERROR("send(res_buf): %zd bytes sent (out of %zu)\n",
							n, len);
			break;
		}
		else
		{
			p += n;
			len -= n;
		}
	}
	CloseSocket_upnphttp(h);
}

