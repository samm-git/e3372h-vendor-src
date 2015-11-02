
#include <features.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <resolv.h>
#include <netdb.h>
#include <ctype.h>
#include <arpa/nameser.h>
#include <sys/utsname.h>
#include <sys/un.h>
#include <time.h>


//__UCLIBC_MUTEX_EXTERN(__resolv_lock);
#define ATP_NAME_RESOLVE_HAS_IPV6

#define MAX_RECURSE 5
#define REPLY_TIMEOUT 2
#define MAX_RETRIES 2
#define MAX_SERVERS 3
#define MAX_SEARCH 4

#define MAX_ALIASES	5

/* 1:ip + 1:full + MAX_ALIASES:aliases + 1:NULL */
#define 	ALIAS_DIM		(2 + MAX_ALIASES + 1)

#if 0
#define PACKETSZ            512
#define MAXDNAME            1025
#define NAMESERVER_PORT     53
#define	HFIXEDSZ            12
#endif

#undef DEBUG
//#define DEBUG
/* #define DEBUG */

#ifdef DEBUG
#define DPRINTF(X,args...) fprintf(stderr, X, ##args)
#else
#define DPRINTF(X,args...)
#endif /* DEBUG */


/* Global stuff (stuff needing to be locked to be thread safe)... */

/* Structs */
struct resolv_header {
	int id;
	int qr,opcode,aa,tc,rd,ra,rcode;
	int qdcount;
	int ancount;
	int nscount;
	int arcount;
};

struct resolv_question {
	char * dotted;
	int qtype;
	int qclass;
};

struct resolv_answer {
	char * dotted;
	int atype;
	int aclass;
	int ttl;
	int rdlength;
	unsigned char * rdata;
	int rdoffset;
	char* buf;
	size_t buflen;
	size_t add_count;
};

/* function prototypes */
static int handy_dns_lookup(const char * name, int type,
						unsigned char ** outpacket, struct resolv_answer * a,
						char* wanaddr);

static int handy_encode_dotted(const char * dotted, unsigned char * dest, int maxlen);
static int handy_decode_dotted(const unsigned char * message, int offset,
						   char * dest, int maxlen);
static int handy_length_dotted(const unsigned char * message, int offset);
static int handy_encode_header(struct resolv_header * h, unsigned char * dest, int maxlen);
static int handy_decode_header(unsigned char * data, struct resolv_header * h);
static int handy_encode_question(struct resolv_question * q,
							 unsigned char * dest, int maxlen);
static int handy_decode_answer(unsigned char * message, int offset,
						   struct resolv_answer * a);
static int handy_length_question(unsigned char * message, int offset);

static int handy_encode_header(struct resolv_header *h, unsigned char *dest, int maxlen)
{
	if (maxlen < HFIXEDSZ)
		return -1;

	dest[0] = (h->id & 0xff00) >> 8;
	dest[1] = (h->id & 0x00ff) >> 0;
	dest[2] = (h->qr ? 0x80 : 0) |
		((h->opcode & 0x0f) << 3) |
		(h->aa ? 0x04 : 0) |
		(h->tc ? 0x02 : 0) |
		(h->rd ? 0x01 : 0);
	dest[3] = (h->ra ? 0x80 : 0) | (h->rcode & 0x0f);
	dest[4] = (h->qdcount & 0xff00) >> 8;
	dest[5] = (h->qdcount & 0x00ff) >> 0;
	dest[6] = (h->ancount & 0xff00) >> 8;
	dest[7] = (h->ancount & 0x00ff) >> 0;
	dest[8] = (h->nscount & 0xff00) >> 8;
	dest[9] = (h->nscount & 0x00ff) >> 0;
	dest[10] = (h->arcount & 0xff00) >> 8;
	dest[11] = (h->arcount & 0x00ff) >> 0;

	return HFIXEDSZ;
}

static int handy_decode_header(unsigned char *data, struct resolv_header *h)
{
	h->id = (data[0] << 8) | data[1];
	h->qr = (data[2] & 0x80) ? 1 : 0;
	h->opcode = (data[2] >> 3) & 0x0f;
	h->aa = (data[2] & 0x04) ? 1 : 0;
	h->tc = (data[2] & 0x02) ? 1 : 0;
	h->rd = (data[2] & 0x01) ? 1 : 0;
	h->ra = (data[3] & 0x80) ? 1 : 0;
	h->rcode = data[3] & 0x0f;
	h->qdcount = (data[4] << 8) | data[5];
	h->ancount = (data[6] << 8) | data[7];
	h->nscount = (data[8] << 8) | data[9];
	h->arcount = (data[10] << 8) | data[11];

	return HFIXEDSZ;
}

/* Encode a dotted string into nameserver transport-level encoding.
   This routine is fairly dumb, and doesn't attempt to compress
   the data */

static int handy_encode_dotted(const char *dotted, unsigned char *dest, int maxlen)
{
	int used = 0;

	while (dotted && *dotted) {
		char *c = strchr(dotted, '.');
		int l = c ? c - dotted : strlen(dotted);

		if (l >= (maxlen - used - 1))
			return -1;

		dest[used++] = l;
		memcpy(dest + used, dotted, l);
		used += l;

		if (c)
			dotted = c + 1;
		else
			break;
	}

	if (maxlen < 1)
		return -1;

	dest[used++] = 0;

	return used;
}

/* Decode a dotted string from nameserver transport-level encoding.
   This routine understands compressed data. */

static int handy_decode_dotted(const unsigned char *data, int offset,
					char *dest, int maxlen)
{
	int l;
	int measure = 1;
	int total = 0;
	int used = 0;

	if (!data)
		return -1;

	while ((l=data[offset++])) {
		if (measure)
		    total++;
		if ((l & 0xc0) == (0xc0)) {
			if (measure)
				total++;
			/* compressed item, redirect */
			offset = ((l & 0x3f) << 8) | data[offset];
			measure = 0;
			continue;
		}

		if ((used + l + 1) >= maxlen)
			return -1;

		memcpy(dest + used, data + offset, l);
		offset += l;
		used += l;
		if (measure)
			total += l;

		if (data[offset] != 0)
			dest[used++] = '.';
		else
			dest[used++] = '\0';
	}

	/* The null byte must be counted too */
	if (measure) {
	    total++;
	}

	DPRINTF("Total decode len = %d\n", total);

	return total;
}

static int handy_length_dotted(const unsigned char *data, int offset)
{
	int orig_offset = offset;
	int l;

	if (!data)
		return -1;

	while ((l = data[offset++])) {

		if ((l & 0xc0) == (0xc0)) {
			offset++;
			break;
		}

		offset += l;
	}

	return offset - orig_offset;
}

static int handy_encode_question(struct resolv_question *q,
					  unsigned char *dest, int maxlen)
{
	int i;

	i = handy_encode_dotted(q->dotted, dest, maxlen);
	if (i < 0)
		return i;

	dest += i;
	maxlen -= i;

	if (maxlen < 4)
		return -1;

	dest[0] = (q->qtype & 0xff00) >> 8;
	dest[1] = (q->qtype & 0x00ff) >> 0;
	dest[2] = (q->qclass & 0xff00) >> 8;
	dest[3] = (q->qclass & 0x00ff) >> 0;

	return i + 4;
}

static int handy_length_question(unsigned char *message, int offset)
{
	int i;

	i = handy_length_dotted(message, offset);
	if (i < 0)
		return i;

	return i + 4;
}

static int handy_decode_answer(unsigned char *message, int offset,
					struct resolv_answer *a)
{
	char temp[256];
	int i;

	i = handy_decode_dotted(message, offset, temp, sizeof(temp));
	if (i < 0)
		return i;

	message += offset + i;

	a->dotted = strdup(temp);
	a->atype = (message[0] << 8) | message[1];
	message += 2;
	a->aclass = (message[0] << 8) | message[1];
	message += 2;
	a->ttl = (message[0] << 24) |
		(message[1] << 16) | (message[2] << 8) | (message[3] << 0);
	message += 4;
	a->rdlength = (message[0] << 8) | message[1];
	message += 2;
	a->rdata = message;
	a->rdoffset = offset + i + RRFIXEDSZ;

	DPRINTF("i=%d,rdlength=%d\n", i, a->rdlength);

	return i + RRFIXEDSZ + a->rdlength;
}

//__UCLIBC_MUTEX_STATIC(mylock, PTHREAD_MUTEX_INITIALIZER);

static int handy_dns_lookup(const char *name, int type,
				 unsigned char **outpacket, struct resolv_answer *a,
				 char* wanaddr)
{
	int i, j, len, fd, pos, rc;
	struct timeval tv;
	fd_set fds;
	struct resolv_header h;
	struct resolv_question q;
	struct resolv_answer ma;
    struct sockaddr_in  bindAddr;
	int first_answer = 1;
	int retries = 0;
	unsigned char * packet = malloc(PACKETSZ);
	char *dns, *lookup = malloc(MAXDNAME);
    int local_id = -1;
	struct sockaddr_in sa;
#ifdef ATP_NAME_RESOLVE_HAS_IPV6
	int v6;
	struct sockaddr_in6 sa6;
    struct sockaddr_in6  bindAddr6;
    int record_flag = 0;
#endif

	fd = -1;

	if (!packet || !lookup)
	    goto fail;

	DPRINTF("Looking up type %d answer for '%s'\n", type, name);
    local_id = time(NULL);

    ++local_id;
    local_id &= 0xffff;

	while (retries < MAX_RETRIES) {
		if (fd != -1)
			close(fd);

		memset(packet, 0, PACKETSZ);

		memset(&h, 0, sizeof(h));

		//++local_id;
		//local_id &= 0xffff;
		h.id = local_id;
#ifdef ATP_NAME_RESOLVE_HAS_IPV6
        if (NULL == wanaddr)
        {
            if (T_AAAA == type)
            {
                if (!record_flag)
                {
    		        dns = "::1";
                }
                else
                {
                    dns = "127.0.0.1";
                }
            }
            else
            {
                if (!record_flag)
                {
                    dns = "127.0.0.1";
                }
                else
                {
    		        dns = "::1";
                }
            }
        }
        else if (inet_pton(AF_INET6, wanaddr, &sa6.sin6_addr) > 0)
        {
            dns = "::1";
        }
        else
        {
            dns = "127.0.0.1";
        }
#else
        dns = "127.0.0.1";
#endif
        DPRINTF("got server %s\n", dns);


		h.qdcount = 1;
		h.rd = 1;

		DPRINTF("encoding header %d\n", h.rd);

		i = handy_encode_header(&h, packet, PACKETSZ);
		if (i < 0)
			goto fail;

		strncpy(lookup,name,MAXDNAME);
		DPRINTF("lookup name: %s\n", lookup);
		q.dotted = (char *)lookup;
		q.qtype = type;
		q.qclass = 1; /* CLASS_IN C_IN*/

		j = handy_encode_question(&q, packet+i, PACKETSZ-i);
		if (j < 0)
			goto fail;

		len = i + j;

		DPRINTF("On try %d, sending query to port %d of machine %s\n",
				retries+1, NAMESERVER_PORT, dns);

#ifdef ATP_NAME_RESOLVE_HAS_IPV6
		v6 = inet_pton(AF_INET6, dns, &sa6.sin6_addr) > 0;
		fd = socket(v6 ? AF_INET6 : AF_INET, SOCK_DGRAM, IPPROTO_UDP);
#else
		fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
#endif
		if (fd < 0) {
			retries++;
            if ((retries >= MAX_RETRIES) && (!record_flag))
            {
                retries = 0;
                record_flag = 1;
            }
		    continue;
		}

		/* Connect to the UDP socket so that asyncronous errors are returned */
#ifdef ATP_NAME_RESOLVE_HAS_IPV6
		if (v6) {
            memset((void *)(&bindAddr6), 0, sizeof(bindAddr6));
            bindAddr6.sin6_family = AF_INET6;
            if ((NULL == wanaddr) || (inet_pton(AF_INET6, wanaddr, &bindAddr6.sin6_addr) <= 0))
            {
                DPRINTF("Bind any addr!\n");
            }
            if (bind(fd, (struct sockaddr *)(&bindAddr6), sizeof(bindAddr6)) < 0)
            {
                DPRINTF("Bind addr %s failed!\n", wanaddr);
                goto fail;
            }
		    sa6.sin6_family = AF_INET6;
		    sa6.sin6_port = htons(NAMESERVER_PORT);
		    /* sa6.sin6_addr is already here */
		    rc = connect(fd, (struct sockaddr *) &sa6, sizeof(sa6));
		} else {
#endif
		    sa.sin_family = AF_INET;
		    sa.sin_port = htons(NAMESERVER_PORT);
        #if 0
		    __UCLIBC_MUTEX_LOCK(__resolv_lock);
		    /* 'dns' is really __nameserver[] which is a global that
		       needs to hold __resolv_lock before access!! */
		    sa.sin_addr.s_addr = inet_addr(dns);
		    __UCLIBC_MUTEX_UNLOCK(__resolv_lock);
        #else
            // TODO: Doing binding
            memset((void *)(&bindAddr), 0, sizeof(bindAddr));
            bindAddr.sin_family = AF_INET;
            //bindAddr.sin_addr.s_addr = localAddr;
            if ((NULL == wanaddr) || (inet_pton(AF_INET, wanaddr, &bindAddr.sin_addr) <= 0))
            {
                bindAddr.sin_addr.s_addr = 0;
            }
            if (bind(fd, (struct sockaddr *)(&bindAddr), sizeof(bindAddr)) < 0)
            {
                DPRINTF("Bind addr %s failed!\n", inet_ntoa(bindAddr.sin_addr));
                goto fail;
            }
            sa.sin_addr.s_addr = inet_addr(dns);
        #endif
		    rc = connect(fd, (struct sockaddr *) &sa, sizeof(sa));
#ifdef ATP_NAME_RESOLVE_HAS_IPV6
		}
#endif
		if (rc < 0) {
		    if (errno == ENETUNREACH) {
				/* routing error, presume not transient */
				//goto fail;
				retries++;
		    } else {
				/* retry */
				retries++;
		    }
            if ((retries >= MAX_RETRIES) && (!record_flag))
            {
                retries = 0;
                record_flag = 1;
            }
			continue;
		}

		DPRINTF("Transmitting packet of length %d, id=%d, qr=%d\n",
				len, h.id, h.qr);

		send(fd, packet, len, 0);

		FD_ZERO(&fds);
		FD_SET(fd, &fds);
		tv.tv_sec = REPLY_TIMEOUT;
		tv.tv_usec = 0;
		if (select(fd + 1, &fds, NULL, NULL, &tv) <= 0) {
		    DPRINTF("Timeout\n");

			/* timed out, so retry send and receive,
			 * to next nameserver on queue */
			//goto fail;
			goto again;
		}

		len = recv(fd, packet, 512, 0);
		if (len < HFIXEDSZ) {
			/* too short ! */
			goto again;
		}

		handy_decode_header(packet, &h);

		DPRINTF("id = %d, qr = %d\n", h.id, h.qr);

		if ((h.id != local_id) || (!h.qr)) {
			/* unsolicited */
			goto again;
		}


		DPRINTF("Got response %s\n", "(i think)!");
		DPRINTF("qrcount=%d,ancount=%d,nscount=%d,arcount=%d\n",
				h.qdcount, h.ancount, h.nscount, h.arcount);
		DPRINTF("opcode=%d,aa=%d,tc=%d,rd=%d,ra=%d,rcode=%d\n",
				h.opcode, h.aa, h.tc, h.rd, h.ra, h.rcode);

		if ((h.rcode) || (h.ancount < 1)) {
			/* negative result, not present */
			goto again;
		}

		pos = HFIXEDSZ;

		for (j = 0; j < h.qdcount; j++) {
			DPRINTF("Skipping question %d at %d\n", j, pos);
			i = handy_length_question(packet, pos);
			DPRINTF("Length of question %d is %d\n", j, i);
			if (i < 0)
				goto again;
			pos += i;
		}
		DPRINTF("Decoding answer at pos %d\n", pos);

		first_answer = 1;
		for (j=0;j<h.ancount;j++,pos += i)
		{
			i = handy_decode_answer(packet, pos, &ma);

			if (i<0) {
				DPRINTF("failed decode %d\n", i);
				goto again;
			}

			if ( first_answer )
			{
				ma.buf = a->buf;
				ma.buflen = a->buflen;
				ma.add_count = a->add_count;
				memcpy(a, &ma, sizeof(ma));
				//if (a->atype != T_SIG && (0 == a->buf || (type != T_A && type != T_AAAA)))
				if (a->atype != 24 && (0 == a->buf || (type != 1 && type != 28)))
				{
					break;
				}
				if (a->atype != type)
				{
					free(a->dotted);
					 a->dotted = NULL;
					continue;
				}
				a->add_count = h.ancount - j - 1;
				if ((a->rdlength + sizeof(struct in_addr*)) * a->add_count > a->buflen)
				{
					break;
				}
				a->add_count = 0;
				first_answer = 0;
			}
			else
			{
				free(ma.dotted);
				if (ma.atype != type)
				{
					continue;
				}
				if (a->rdlength != ma.rdlength)
				{
					free(a->dotted);
					 a->dotted = NULL;
					DPRINTF("Answer address len(%u) differs from original(%u)\n",
							ma.rdlength, a->rdlength);
					goto again;
				}
				memcpy(a->buf + (a->add_count * ma.rdlength), ma.rdata, ma.rdlength);
				++a->add_count;

                if (a->add_count >= 7)
                {
                    DPRINTF("8 records at most.\n");
                    break;
                }
			}
		}

		DPRINTF("Answer name = |%s|\n", a->dotted);
		DPRINTF("Answer type = |%d|\n", a->atype);

		close(fd);

		if (outpacket)
			*outpacket = packet;
		else
			free(packet);
		free(lookup);

		return (len);				/* success! */

	again:
        retries++;

        if ((retries >= MAX_RETRIES) && (!record_flag))
        {
            retries = 0;
            record_flag = 1;
        }
	}

 fail:
	if (fd != -1)
	    close(fd);
	if (lookup)
	    free(lookup);
	if (packet)
	    free(packet);
	return -1;
}

static int gethostbyname_handy_r(const char * name,
					struct hostent * result_buf,
					char * buf, size_t buflen,
					struct hostent ** result,
					char* wanaddr)
{
    struct in_addr *in;
    struct in_addr **addr_list;
    char **alias;
    unsigned char *packet;
    struct resolv_answer a;
    int i;

    *result=NULL;
    if (!name)
        return EINVAL;

    if (buflen < sizeof(*in))
        return ERANGE;
    in=(struct in_addr*)buf;
    buf+=sizeof(*in);
    buflen-=sizeof(*in);

    if (buflen < sizeof(*addr_list)*2)
        return ERANGE;
    addr_list=(struct in_addr**)buf;
    buf+=sizeof(*addr_list)*2;
    buflen-=sizeof(*addr_list)*2;

    addr_list[0] = in;
    addr_list[1] = 0;

    if (buflen < sizeof(char *)*(ALIAS_DIM))
        return ERANGE;
    alias=(char **)buf;
    buf+=sizeof(char **)*(ALIAS_DIM);
    buflen-=sizeof(char **)*(ALIAS_DIM);

    if (buflen<256)
        return ERANGE;
    strncpy(buf, name, buflen);

    alias[0] = buf;
    alias[1] = NULL;

    /* First check if this is already an address */
    if (inet_aton(name, in)) {
        result_buf->h_name = buf;
        result_buf->h_addrtype = AF_INET;
        result_buf->h_length = sizeof(*in);
        result_buf->h_addr_list = (char **) addr_list;
        result_buf->h_aliases = alias;
        *result=result_buf;
        return NETDB_SUCCESS;
    }

    for (;;) {
        a.buf = buf;
        a.buflen = buflen;
        a.add_count = 0;

        i = handy_dns_lookup(name, 1, &packet, &a, wanaddr);

        if (i < 0) {
            DPRINTF("handy_dns_lookup\n");
            return TRY_AGAIN;
        }

        if ((a.rdlength + sizeof(struct in_addr*)) * a.add_count + 256 > buflen)
        {
            free(a.dotted);
            free(packet);
            DPRINTF("buffer too small for all addresses\n");
            return ERANGE;
        }
        else if(a.add_count > 0)
        {
            memmove(buf - sizeof(struct in_addr*)*2, buf, a.add_count * a.rdlength);
            addr_list = (struct in_addr**)(buf + a.add_count * a.rdlength);
            addr_list[0] = in;
            for (i = a.add_count-1; i>=0; --i)
                addr_list[i+1] = (struct in_addr*)(buf - sizeof(struct in_addr*)*2 + a.rdlength * i);
            addr_list[a.add_count + 1] = 0;
            buflen -= (((char*)&(addr_list[a.add_count + 2])) - buf);
            buf = (char*)&addr_list[a.add_count + 2];
        }

        strncpy(buf, a.dotted, buflen);
        free(a.dotted);

        if (a.atype == 1) { /* ADDRESS T_A=1 */
            memcpy(in, a.rdata, sizeof(*in));
            result_buf->h_name = buf;
            result_buf->h_addrtype = AF_INET;
            result_buf->h_length = sizeof(*in);
            result_buf->h_addr_list = (char **) addr_list;
#ifdef __UCLIBC_MJN3_ONLY__
#warning TODO -- generate the full list
#endif
            result_buf->h_aliases = alias; /* TODO: generate the full list */
            free(packet);
            break;
        } else {
            free(packet);
            return TRY_AGAIN;
        }
    }

    *result=result_buf;
    return NETDB_SUCCESS;
}

struct hostent *atp_gethostbyname(const char *name, unsigned int localAddr)
{
	static struct hostent h;
	static char buf[sizeof(struct in_addr) +
					sizeof(struct in_addr *)*2 +
					sizeof(char *)*(ALIAS_DIM) + 384/*namebuffer*/ + 32/* margin */];
	struct hostent *hp;
    struct in_addr wanin;

    wanin.s_addr = localAddr;

	gethostbyname_handy_r(name, &h, buf, sizeof(buf), &hp, inet_ntoa(wanin));

	return hp;
}

int handy_gethostbyname2_r(const char *name, int family,
					 struct hostent * result_buf,
					 char * buf, size_t buflen,
					 struct hostent ** result,
					 char * bindaddr)
{
	struct in6_addr *in;
	struct in6_addr **addr_list;
	unsigned char *packet;
	struct resolv_answer a;
	int i;
	//int nest = 0;
	/* 20120904 Delete by weiqingchuan 61642 for Compiling Warning */
    //struct in_addr in4;
    int lRet = 1;
    char cmd[32] = {0};
    FILE *fp = NULL;
    *result=NULL;
    packet = NULL;

	if (family == AF_INET)
	{
        fp = fopen("/var/dns/ipv4","r");
        if (fp != NULL)
        {
            fgets(cmd, 32, fp);
            lRet = atoi(cmd);
            fclose(fp);
        }
        if (lRet != 1)
        {
            return EINVAL;
        }
		return gethostbyname_handy_r(name, result_buf, buf, buflen, result, bindaddr);
	}
     
	if (family != AF_INET6)
		return EINVAL;

	if (!name)
		return EINVAL;

    /* 未开启IPV6功能直接返回 */
    fp = fopen("/proc/sys/net/ipv6/conf/all/enable","r");
    if (fp != NULL)
    {
        fgets(cmd, 32, fp);
        lRet = atoi(cmd);
        fclose(fp);
    }
    if (lRet != 1)
    {
        return EINVAL;
    }

    fp = fopen("/var/dns/ipv6","r");
    if (fp != NULL)
    {
        fgets(cmd, 32, fp);
        lRet = atoi(cmd);
        fclose(fp);
    }
    if (lRet != 1)
    {
        return EINVAL;
    }

	if (buflen < sizeof(*in))
		return ERANGE;
	in=(struct in6_addr*)buf;
	buf+=sizeof(*in);
	buflen-=sizeof(*in);

	if (buflen < sizeof(*addr_list)*2)
		return ERANGE;
	addr_list=(struct in6_addr**)buf;
	buf+=sizeof(*addr_list)*2;
	buflen-=sizeof(*addr_list)*2;

	addr_list[0] = in;
	addr_list[1] = 0;

	if (buflen<256)
		return ERANGE;
	strncpy(buf, name, buflen);

	/* First check if this is already an address */
	if (inet_pton(AF_INET6, name, in)) {
	    result_buf->h_name = buf;
	    result_buf->h_addrtype = AF_INET6;
	    result_buf->h_length = sizeof(*in);
	    result_buf->h_addr_list = (char **) addr_list;
	    *result=result_buf;
	    return NETDB_SUCCESS;
	}

	memset((char *) &a, '\0', sizeof(a));

	for (;;) {
        a.buf = buf;
        a.buflen = buflen;

		i = handy_dns_lookup(buf, T_AAAA, &packet, &a, bindaddr);

		if (i < 0) {
			return TRY_AGAIN;
		}

        if ((a.rdlength + sizeof(struct in6_addr*)) * a.add_count + 256 > buflen)
        {
            if(a.dotted != NULL)
            {
			 free(a.dotted);
			 a.dotted = NULL;
	     }
           
	     if(packet != NULL)
            {
			 free(packet);
			 packet = NULL;
	     }
            DPRINTF("buffer too small for all addresses\n");
            return ERANGE;
        }
        else if(a.add_count > 0)
        {
            memmove(buf - sizeof(struct in6_addr*)*2, buf, a.add_count * a.rdlength);
            addr_list = (struct in6_addr**)(buf + a.add_count * a.rdlength);
            addr_list[0] = in;
            for (i = a.add_count-1; i>=0; --i)
                addr_list[i+1] = (struct in6_addr*)(buf - sizeof(struct in6_addr*)*2 + a.rdlength * i);
            addr_list[a.add_count + 1] = 0;
            buflen -= (((char*)&(addr_list[a.add_count + 2])) - buf);
            buf = (char*)&addr_list[a.add_count + 2];
        }

		if(a.dotted != NULL)
		{
			strncpy(buf, a.dotted, buflen);
			free(a.dotted);
		 	a.dotted = NULL;
		}

		if (a.atype == T_AAAA) {	/* ADDRESS */
			memcpy(in, a.rdata, sizeof(*in));
			result_buf->h_name = buf;
			result_buf->h_addrtype = AF_INET6;
			result_buf->h_length = sizeof(*in);
			result_buf->h_addr_list = (char **) addr_list;
			if(packet != NULL)
	            {
				 free(packet);
				 packet = NULL;
		     }
			break;
		} else {
			if(packet != NULL)
	            {
				 free(packet);
				 packet = NULL;
		     }
			return TRY_AGAIN;
		}
	}

	*result=result_buf;
	return NETDB_SUCCESS;
}

struct hostent *ATP_GPL_Gethostbyname(const char *name, int family, char* bindaddr)
{
	static struct hostent h;
	static char buf[sizeof(struct in6_addr) +
					sizeof(struct in6_addr *)*2 +
					sizeof(char *)*(ALIAS_DIM) + 384/*namebuffer*/ + 32/* margin */];
	struct hostent *hp;

	handy_gethostbyname2_r(name, family, &h, buf, sizeof(buf), &hp, bindaddr);

	return hp;
}


#if 0
int main(int argc, char *argv[])
{
    struct hostent *res;

    printf("Bind with %s resolve for %s\n", argv[1], argv[2]);
    res = atp_gethostbyname(argv[1], inet_addr(argv[2]));
    if (NULL == res)
    {
        printf("Resolve failed!\n");
    }
    else
    {
        printf("Resolve OK!\n");
    }

    return 0;
}
#endif

