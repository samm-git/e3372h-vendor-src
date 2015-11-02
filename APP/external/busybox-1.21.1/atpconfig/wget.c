/* vi: set sw=4 ts=4: */
/*
 * wget - retrieve a file using HTTP or FTP
 *
 * Chip Rosenthal Covad Communications <chip@laserlink.net>
 *
 */

#include <getopt.h>	/* for struct option */
#include "libbb.h"
#include "atpcommon.h"

//usage:#define wget_trivial_usage 
//usage:	    "[OPTION]... HOST"

//usage:#define wget_full_usage 
//usage:	    "wget download and upload a file via HTTP\n\n" 
//usage:	    "Options:\n" 
//usage:       "\t-g, --download         Download\n" 
//usage:       "\t-s, --upload           Upload\n" 
//usage:       "\t-v, --verbose          Verbose\n" 
//usage:       "\t-u, --username         Username to be used\n" 
//usage:       "\t-p, --password         Password to be used\n" 
//usage:       "\t-l, --local            Local file path\n" 
//usage:       "\t-r, --remote           Remote file path\n" 
//usage:       "\t-P, --port             Port to be used, optional\n" 
//usage:       "\t-B, --bind             Bind local ip, optional\n" 
//usage:       "\t-A, --addr             Remote resolved ip, optional\n" 
//usage:       "\t-b, --begin            Transfer start position\n" 
//usage:       "\t-e, --len              Transfer length\n" 
//usage:       "\t-m, --max              Max transfer size\n" 
//usage:       "\t-c, --compress         Compress downloaded file" 
        
        
#if ENABLE_FEATURE_ATP_WGET_AUTHENTICATION
#include "openssl/md5.h"
#endif

#if ENABLE_FEATURE_ATP_WGET_HTTPS
#include "openssl/ssl.h"

static smallint https_mode;

/*START ADD by c00164495 for HG622u 对于只允许通过域名访问的服务器下载失败 2011-02-22*/
char *pcDomain = NULL;	//保存远端服务器域名
/*END ADD by c00164495 for HG622u 对于只允许通过域名访问的服务器下载失败 2011-02-22*/
#endif

struct host_info {
	// May be used if we ever will want to free() all xstrdup()s...
	/* char *allocated; */
    char *host;
	const char *path;
	char *user;
    char *password;
    char *localFile;
    int  port;
    smallint is_https;
};


/* Globals (can be accessed from signal handlers) */
struct globals {
	off_t content_len;        /* Content-length of the file */
	off_t beg_range;          /* Range at which continue begins */
#if ENABLE_FEATURE_ATP_WGET_STATUSBAR
	off_t lastsize;
	off_t totalsize;
	off_t transferred;        /* Number of bytes transferred so far */
	const char *curfile;      /* Name of current file being transferred */
	unsigned lastupdate_sec;
	unsigned start_sec;
#endif
	smallint chunked;             /* chunked transfer encoding */
};
#define G (*(struct globals*)(bb_common_bufsiz1 + sizeof(struct atp_trans_globals)))
struct BUG_G_too_big {
        char BUG_G_too_big[sizeof(G) <= COMMON_BUFSIZE ? 1 : -1];
};
#define content_len     (G.content_len    )
#define beg_range       (G.beg_range      )
#define lastsize        (G.lastsize       )
#define totalsize       (G.totalsize      )
#define transferred     (G.transferred    )
#define curfile         (G.curfile        )
#define lastupdate_sec  (G.lastupdate_sec )
#define start_sec       (G.start_sec      )
#define chunked         (G.chunked        )
#define INIT_G() do { } while (0)

static const char keywords[] ALIGN1 =
	"content-length\0""transfer-encoding\0""chunked\0""location\0www-authenticate\0";
enum {
	KEY_content_length = 1, KEY_transfer_encoding, KEY_chunked, KEY_location,
    KEY_authenticate
};

//#if ENABLE_FEATURE_ATP_WGET_HTTPS
//static smallint https_mode;
//#endif

#if ENABLE_FEATURE_ATP_WGET_STATUSBAR
enum {
	STALLTIME = 5                   /* Seconds when xfer considered "stalled" */
};

static int getttywidth(void)
{
	int width;
	get_terminal_width_height(0, &width, NULL);
	return width;
}

static void progressmeter(int flag)
{
	/* We can be called from signal handler */
	int save_errno = errno;
	off_t abbrevsize;
	unsigned since_last_update, elapsed;
	unsigned ratio;
	int barlength, i;

	if (flag == -1) { /* first call to progressmeter */
		start_sec = monotonic_sec();
		lastupdate_sec = start_sec;
		lastsize = 0;
		totalsize = content_len + beg_range; /* as content_len changes.. */
	}

	ratio = 100;
	if (totalsize != 0 && !chunked) {
		/* long long helps to have it working even if !LFS */
		ratio = (unsigned) (100ULL * (transferred+beg_range) / totalsize);
		if (ratio > 100) ratio = 100;
	}

	fprintf(stderr, "\r%-20.20s%4d%% ", curfile, ratio);

	barlength = getttywidth() - 49;
	if (barlength > 0) {
		/* god bless gcc for variable arrays :) */
		i = barlength * ratio / 100;
		{
			char buf[i+1];
			memset(buf, '*', i);
			buf[i] = '\0';
			fprintf(stderr, "|%s%*s|", buf, barlength - i, "");
		}
	}
	i = 0;
	abbrevsize = transferred + beg_range;
	while (abbrevsize >= 100000) {
		i++;
		abbrevsize >>= 10;
	}
	/* see http://en.wikipedia.org/wiki/Tera */
	fprintf(stderr, "%6d%c ", (int)abbrevsize, " kMGTPEZY"[i]);

// Nuts! Ain't it easier to update progress meter ONLY when we transferred++?

	elapsed = monotonic_sec();
	since_last_update = elapsed - lastupdate_sec;
	if (transferred > lastsize) {
		lastupdate_sec = elapsed;
		lastsize = transferred;
		if (since_last_update >= STALLTIME) {
			/* We "cut off" these seconds from elapsed time
			 * by adjusting start time */
			start_sec += since_last_update;
		}
		since_last_update = 0; /* we are un-stalled now */
	}
	elapsed -= start_sec; /* now it's "elapsed since start" */

	if (since_last_update >= STALLTIME) {
		fprintf(stderr, " - stalled -");
	} else {
		off_t to_download = totalsize - beg_range;
		if (transferred <= 0 || (int)elapsed <= 0 || transferred > to_download || chunked) {
			fprintf(stderr, "--:--:-- ETA");
		} else {
			/* to_download / (transferred/elapsed) - elapsed: */
			int eta = (int) ((unsigned long long)to_download*elapsed/transferred - elapsed);
			/* (long long helps to have working ETA even if !LFS) */
			i = eta % 3600;
			fprintf(stderr, "%02d:%02d:%02d ETA", eta / 3600, i / 60, i % 60);
		}
	}

	if (flag == 0) {
		/* last call to progressmeter */
		alarm(0);
		transferred = 0;
		fputc('\n', stderr);
	} else {
		if (flag == -1) {
			/* first call to progressmeter */
			struct sigaction sa;
			sa.sa_handler = progressmeter;
			sigemptyset(&sa.sa_mask);
			sa.sa_flags = SA_RESTART;
			sigaction(SIGALRM, &sa, NULL);
		}
		alarm(1);
	}

	errno = save_errno;
}
/* Original copyright notice which applies to the CONFIG_FEATURE_WGET_STATUSBAR stuff,
 * much of which was blatantly stolen from openssh.  */
/*-
 * Copyright (c) 1992, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. <BSD Advertising Clause omitted per the July 22, 1999 licensing change
 *		ftp://ftp.cs.berkeley.edu/pub/4bsd/README.Impt.License.Change>
 *
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */
#else /* FEATURE_WGET_STATUSBAR */

static ALWAYS_INLINE void progressmeter(int flag) { }

#endif

//close_and_delete_outfile(localFile);
#define close_delete_and_die(s...) { \
	bb_error_msg_and_die(s); }

#define close_and_die(s...) {   \
	close(localFile);           \
	bb_error_msg_and_die(s); }

/* Read NMEMB bytes into PTR from STREAM.  Returns the number of bytes read,
 * and a short count if an eof or non-interrupt error is encountered.  */
static size_t safe_fread(void *ptr, size_t nmemb, FILE *stream)
{
	size_t ret;
	char *p = (char*)ptr;

	do {
		clearerr(stream);
		ret = fread(p, 1, nmemb, stream);
		p += ret;
		nmemb -= ret;
	} while (nmemb && ferror(stream) && errno == EINTR);

	return p - (char*)ptr;
}

/* Read a line or SIZE-1 bytes into S, whichever is less, from STREAM.
 * Returns S, or NULL if an eof or non-interrupt error is encountered.  */
static char *safe_fgets(char *s, int size, FILE *stream)
{
#if ENABLE_FEATURE_ATP_WGET_HTTPS
    if (https_mode > 0) {
        close_delete_and_die("https not supported");
    }
    return NULL;
#else
	char *ret;

	do {
		clearerr(stream);
		ret = fgets(s, size, stream);
	} while (ret == NULL && ferror(stream) && errno == EINTR);

	return ret;
#endif
}

#if ENABLE_FEATURE_ATP_WGET_HTTPS
// 认证对方证书的示例代码
#if 0
static int verify_callback(int ok, X509_STORE_CTX *store)
{
    char data[256];

    if (!ok)
    {
        X509 *cert = X509_STORE_CTX_get_current_cert(store);
        int  depth = X509_STORE_CTX_get_error_depth(store);
        int  err   = X509_STORE_CTX_get_error(store);

        fprintf(stderr, "-Error with certificate at depth: %i\n", depth);
        X509_NAME_oneline(X509_get_issuer_name(cert), data, 256);
        fprintf(stderr, "   issuer   = %s\n", data);
        X509_NAME_oneline(X509_get_subject_name(cert), data, 256);
        fprintf(stderr, "   subject  = %s\n", data);
        fprintf(stderr, "   err      = %i:%s\n", err, X509_verify_cert_error_string(err));

        // 忽略证书还没有生效的错误
        if () {
        }
    }

    return ok;
}
#endif

#ifdef BUILD_USE_POLARSSL
static SSL_CTX *setup_client_ctx(void)
{
    SSL_CTX *ctx;

    /*
     *  0: No ssl; 1: Any except V2
     *  2: SSLv2; 3: SSLv3; 4: TLSv1
     *  5: Any version
     */
    if (3 == https_mode)
    {
        ctx = SSL_CTX_new(SSL_MINOR_VERSION_0, SSL_IS_CLIENT);
    }
    else    // Default TLS
    {
        ctx = SSL_CTX_new(SSL_MINOR_VERSION_1, SSL_IS_CLIENT);
    }
    SSL_CTX_set_verify(ctx, SSL_VERIFY_NONE);
    ctx->ciphers = ssl_default_ciphers;
    return ctx;
}
#else
static SSL_CTX *setup_client_ctx(void)
{
    SSL_CTX *ctx;
    SSL_METHOD *method;

    /*
     *  0: No ssl; 1: Any except V2
     *  2: SSLv2; 3: SSLv3; 4: TLSv1
     *  5: Any version
     */
    method = NULL;
    if ((1 == https_mode) || (5 == https_mode)) {
    #ifdef BUILD_USE_CYASSL
        method = TLSv1_client_method();
    #else
        method = SSLv23_method();
    #endif
    }
#if 0 //disable SSL V2   
     else if (2 == https_mode) {
        method = SSLv2_method();
    }
#endif    
    else if (3 == https_mode) {
    #ifdef BUILD_USE_CYASSL
        method = SSLv3_client_method();
    #else
        method = SSLv3_method();
    #endif
    } else if (4 == https_mode) {
    #ifdef BUILD_USE_CYASSL
        method = TLSv1_client_method();
    #else
        method = TLSv1_method();
    #endif
    }
    if (NULL == method) {
        return NULL;
    }
    ctx = SSL_CTX_new(method);
    if (NULL == ctx) {
        return NULL;
    }

    SSL_CTX_set_verify(ctx, SSL_VERIFY_NONE, NULL);
#if 0
    // 加载本地的根证书用于认证对方
    if (SSL_CTX_load_verify_locations(ctx, CAFILE, CADIR) != 1) {
        SSL_CTX_free(ctx);
        return NULL;
    }

    SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, verify_callback);
    SSL_CTX_set_verify_depth(ctx, 4);

    // 更改加密套，一般不需要
    if (SSL_CTX_set_cipher_list(ctx, CIPHER_LIST) != 1) {
        SSL_CTX_free(ctx);
        return NULL;
    }
#endif
    if (1 == https_mode) {
        // 排除SSLv2
    #ifndef BUILD_USE_CYASSL
        SSL_CTX_set_options(ctx, SSL_OP_ALL|SSL_OP_NO_SSLv2);
    #endif
    }

    return ctx;
}
#endif

static int open_socket(len_and_sockaddr *lsa, SSL **retssl, FILE **fIn, FILE **fOut,
                                    tr143_diag_state_t *pstDiagState, bool bDiagFlag)
{
    int     fd;
    SSL     *ssl;
    SSL_CTX *ctx;

	/* glibc 2.4 seems to try seeking on it - ??! */
	/* hopefully it understands what ESPIPE means... */
    xfunc_error_retval = ATP_TRANS_TIMEOUT;
    fd = xbind_connect(lsa, g_pcLocalIP);
    xfunc_error_retval = ATP_TRANS_SYS_ERR;

    // SSL not enabled
    if (https_mode <= 0) {
        (*fIn) = fdopen(fd, "r");
        (*fOut) = fdopen(fd, "w");
        if (((*fIn) == NULL) || ((*fOut) == NULL))
        {
                  SetDiagState(pstDiagState, "Error_InitConnectionFailed", bDiagFlag);
		    bb_perror_msg_and_die("fdopen");
        }
        return fd;
    }

    // Setup ssl obj
    ctx = setup_client_ctx();
    if (!(ssl = SSL_new(ctx)))
    {
        SetDiagState(pstDiagState, "Error_InitConnectionFailed", bDiagFlag);
        bb_perror_msg_and_die("ssl obj");
    }
    SSL_set_fd(ssl, fd);

    // Do ssl negotiation
#ifdef BUILD_USE_POLARSSL
    if (SSL_connect(fd, ctx, ssl) <= 0)
    {
        SetDiagState(pstDiagState, "Error_InitConnectionFailed", bDiagFlag);
        bb_perror_msg_and_die("ssl connect");
    }
#else
    if (SSL_connect(ssl) <= 0)
    {
        SetDiagState(pstDiagState, "Error_InitConnectionFailed", bDiagFlag);
        bb_perror_msg_and_die("ssl connect");
    }
#endif

    (*retssl) = ssl;

	return fd;
}
#else
static int open_socket(len_and_sockaddr *lsa, FILE **fIn, FILE **fOut,  tr143_diag_state_t *pstDiagState, bool bDiagFlag)
{
    int fd;

	/* glibc 2.4 seems to try seeking on it - ??! */
	/* hopefully it understands what ESPIPE means... */
    xfunc_error_retval = ATP_TRANS_TIMEOUT;
    fd = xbind_connect(lsa, g_pcLocalIP);
	(*fIn) = fdopen(fd, "r");
    (*fOut) = fdopen(fd, "w");
	if (((*fIn) == NULL) || ((*fOut) == NULL))
	{
             SetDiagState(pstDiagState, "Error_InitConnectionFailed", bDiagFlag);
		bb_perror_msg_and_die("fdopen");
	}
    xfunc_error_retval = ATP_TRANS_SYS_ERR;

	return fd;
}
#endif

static void parse_url(char *src_url, struct host_info *h)
{
	char *url, *p, *sp;

	/* h->allocated = */ url = xstrdup(src_url);

	if (strncmp(url, "https://", 8) == 0) {
		h->port = bb_lookup_port("https", "tcp", 443);
		h->host = url + 8;
		h->is_https = 0;
	} else if (strncmp(url, "http://", 7) == 0) {
		h->port = bb_lookup_port("http", "tcp", 80);
		h->host = url + 6;
		h->is_https = 1;
	} else
		bb_error_msg_and_die("not an https or http url: %s", url);

	// FYI:
	// "Real" wget 'http://busybox.net?var=a/b' sends this request:
	//   'GET /?var=a/b HTTP 1.0'
	//   and saves 'index.html?var=a%2Fb' (we save 'b')
	// wget 'http://busybox.net?login=john@doe':
	//   request: 'GET /?login=john@doe HTTP/1.0'
	//   saves: 'index.html?login=john@doe' (we save '?login=john@doe')
	// wget 'http://busybox.net#test/test':
	//   request: 'GET / HTTP/1.0'
	//   saves: 'index.html' (we save 'test')
	//
	// We also don't add unique .N suffix if file exists...
	sp = strchr(h->host, '/');
	p = strchr(h->host, '?'); if (!sp || (p && sp > p)) sp = p;
	p = strchr(h->host, '#'); if (!sp || (p && sp > p)) sp = p;
	if (!sp) {
		h->path = "";
	} else if (*sp == '/') {
		*sp = '\0';
		h->path = sp + 1;
	} else { // '#' or '?'
		// http://busybox.net?login=john@doe is a valid URL
		// memmove converts to:
		// http:/busybox.nett?login=john@doe...
		memmove(h->host - 1, h->host, sp - h->host);
		h->host--;
		sp[-1] = '\0';
		h->path = sp;
	}

	sp = strrchr(h->host, '@');
	h->user = NULL;
	if (sp != NULL) {
		h->user = h->host;
		*sp = '\0';
		h->host = sp + 1;
	}

	sp = h->host;
}


#if ENABLE_FEATURE_ATP_WGET_HTTPS
typedef struct tag_ssl_line_buffer
{
    char    *pcBuf;
    int     lTotalLen;
    int     lReadLen;
} ssl_line_buffer;
static void ssl_fgets(char *buf, size_t bufsiz, void *fp, SSL *ssl)
{
    ssl_line_buffer *linebuf;
    char *pcLineEnd;
    char backup;
    int readLen;
    if (https_mode <= 0) {
        if (fgets(buf, bufsiz, (FILE *)fp) == NULL)
		    close_delete_and_die("fgets");
        return;
    }

    linebuf = (ssl_line_buffer *)fp;
    linebuf->pcBuf[linebuf->lReadLen] = '\0';

check_line_end:
    if ((linebuf->lTotalLen - linebuf->lReadLen) < 1) {
        close_delete_and_die("ssl fgets");
    }

    pcLineEnd = strstr(linebuf->pcBuf, "\r\n");
    if (NULL != pcLineEnd)
    {
        pcLineEnd += 2;
        backup = (*pcLineEnd);
        (*pcLineEnd) = '\0';
        snprintf(buf, bufsiz, "%s", linebuf->pcBuf);
        (*pcLineEnd) = backup;
        linebuf->lReadLen -= (pcLineEnd - linebuf->pcBuf);
        memcpy((void *)(linebuf->pcBuf), (void *)pcLineEnd,
               (linebuf->lReadLen));
        return;
    }

    readLen = SSL_read(ssl, (void *)(linebuf->pcBuf + linebuf->lReadLen), ((linebuf->lTotalLen - linebuf->lReadLen) - 1));
    if (readLen < 0) {
        close_delete_and_die("ssl read");
    }

    linebuf->lReadLen += readLen;
    linebuf->pcBuf[linebuf->lReadLen] = '\0';
    goto check_line_end;
}

static char *gethdr(char *buf, size_t bufsiz, void *fp, SSL *ssl)
#else
static char *gethdr(char *buf, size_t bufsiz, FILE *fp /*, int *istrunc*/)
#endif
{
	char *s, *hdrval;
	int c;

	/* *istrunc = 0; */

	/* retrieve header line */
#if ENABLE_FEATURE_ATP_WGET_HTTPS
    ssl_fgets(buf, bufsiz, fp, ssl);
#else
	if (fgets(buf, bufsiz, fp) == NULL)
		return NULL;
#endif

    //printf("Got header:\n%s\n", buf);

	/* see if we are at the end of the headers */
	for (s = buf; *s == '\r'; ++s)
		continue;
	if (*s == '\n')
		return NULL;

	/* convert the header name to lower case */
	for (s = buf; isalnum(*s) || *s == '-' || *s == '.'; ++s)
		*s = tolower(*s);

	/* verify we are at the end of the header name */
	if (*s != ':')
		bb_error_msg_and_die("bad header line: %s", buf);

	/* locate the start of the header value */
	*s++ = '\0';
	hdrval = skip_whitespace(s);

	/* locate the end of header */
	while (*s && *s != '\r' && *s != '\n')
		++s;

	/* end of header found */
	if (*s) {
		*s = '\0';
		return hdrval;
	}

	/* Rats! The buffer isn't big enough to hold the entire header value. */
#if ENABLE_FEATURE_ATP_WGET_HTTPS
    if (https_mode <= 0) {
        while (c = getc((FILE *)fp), c != EOF && c != '\n')
		continue;
    }
#else
	while (c = getc(fp), c != EOF && c != '\n')
		continue;
#endif
	/* *istrunc = 1; */
	return hdrval;
}

#if ENABLE_FEATURE_ATP_WGET_AUTHENTICATION
#define HTTP_HD_AUTHORIZATION           "Authorization"
#define HTTP_HD_AUTHORIZATION_SIZE      (13)

#define HTTPAUTH_TYPE_BASIC_STR         "Basic"     // Case-insensitive
#define HTTPAUTH_TYPE_BASIC_STR_LEN     5           // Const length, calculated manually for efficiency
#define HTTPAUTH_TYPE_DIGEST_STR        "Digest"
#define HTTPAUTH_TYPE_DIGEST_STR_LEN    6           // Const length, calculated manually for efficiency

#define HTTPAUTH_DEFAULT_REALM          "Huawei"

#define HASHLEN 16
typedef char HASH[HASHLEN];
#define HASHHEXLEN 32
typedef char HASHHEX[HASHHEXLEN+1];

/*
 *  Base64-encode character string
 *  oops... isn't something similar in uuencode.c?
 *  It would be better to use already existing code
 */
static char *base64enc(unsigned char *p, char *buf, int len) {

        char al[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
                    "0123456789+/";
		char *s = buf;

        while(*p) {
				if (s >= buf+len-4)
					bb_error_msg_and_die("buffer overflow");
                *(s++) = al[(*p >> 2) & 0x3F];
                *(s++) = al[((*p << 4) & 0x30) | ((*(p+1) >> 4) & 0x0F)];
                *s = *(s+1) = '=';
                *(s+2) = 0;
                if (! *(++p)) break;
                *(s++) = al[((*p << 2) & 0x3C) | ((*(p+1) >> 6) & 0x03)];
                if (! *(++p)) break;
                *(s++) = al[*(p++) & 0x3F];
        }

		return buf;
}

#define HTTPDigest_SkipSpace(x) while ((isspace((unsigned char)(*(x)))) && ('\0' != (*(x)))) {(x)++;} \
                                        if ('\0' == (*(x))) {bb_error_msg_and_die("syntax error");}

// All the names are case insensitive
static const char *HTTPDigest_FieldNames[] =
{
    "realm",                // The coresponding value is case-sensitive
    "nonce",                // The coresponding value is case-sensitive
    "qop",                  // The coresponding value is case-sensitive
    "username",             // The coresponding value is case-sensitive
    "uri",                  // The coresponding value is case-sensitive
    "cnonce",               // The coresponding value is case-sensitive
    "nc",                   // The coresponding value is case-sensitive
    "response",             // The coresponding value is case-sensitive
    "algorithm",            // The coresponding value is case-sensitive
    "opaque",               // The coresponding value is case-sensitive
    "stale",                // TRUE: digest is OK, but username/password not OK;
                            // FALSE: username/password not OK
    NULL
};

/*
 *  Local types
 */
typedef enum tagHTTPDigest_FieldIndex
{
    HTTPDigest_Field_realm,
    HTTPDigest_Field_nonce,
    HTTPDigest_Field_qop,
    HTTPDigest_Field_username,
    HTTPDigest_Field_uri,
    HTTPDigest_Field_cnonce,
    HTTPDigest_Field_nc,
    HTTPDigest_Field_response,
    HTTPDigest_Field_algorithm,
    //lint -save -e749
    HTTPDigest_Field_opaque,
    //lint -restore
    HTTPDigest_Field_End
//lint -save -e751
} HTTPDigest_FieldIndex;
//lint -restore
typedef char *HTTPDigest_Fields[HTTPDigest_Field_End];

#define HTTPAuth_FieldNameMaxLen           32
#define HTTPAuth_FieldValueMaxLen          128

static void CvtHex(HASH Bin, HASHHEX Hex)
{
    unsigned short i;
    unsigned char j;

    //lint -save -e734
    for (i = 0; i < HASHLEN; i++)
    {
        // Higher 4 bits
        //lint -save -e702
        j = (Bin[i] >> 4) & 0xf;
        //lint -restore
        if (j <= 9)
        {
            Hex[i*2] = (j + '0');
        }
        else
        {
            Hex[i*2] = (j + 'a' - 10);
        }

        // Lower 4 bits
        j = Bin[i] & 0xf;
        if (j <= 9)
        {
            Hex[i*2+1] = (j + '0');
        }
        else
        {
            Hex[i*2+1] = (j + 'a' - 10);
        }
    };
    Hex[HASHHEXLEN] = '\0';
    //lint -restore
}

/* calculate H(A1) as per spec */
static void DigestCalcHA1(
            const char   *pszAlg,
            const char   *pszUserName,
            const char   *pszRealm,
            const char   *pszPassword,
            const char   *pszNonce,
            const char   *pszCNonce,
            HASHHEX     SessionKey)
{
    MD5_CTX Md5Ctx;
    HASH HA1;

    MD5_Init(&Md5Ctx);
    MD5_Update(&Md5Ctx, (unsigned char *)pszUserName, (unsigned int)strlen(pszUserName));
    MD5_Update(&Md5Ctx, (unsigned char *)":", 1);
    MD5_Update(&Md5Ctx, (unsigned char *)pszRealm, (unsigned int)strlen(pszRealm));
    MD5_Update(&Md5Ctx, (unsigned char *)":", 1);
    MD5_Update(&Md5Ctx, (unsigned char *)pszPassword, (unsigned int)strlen(pszPassword));
    MD5_Final((unsigned char *)HA1, &Md5Ctx);

    // Calculate SessionKey if use the MD5-sess algorithm
#ifdef WIN32
    if ((NULL != pszAlg) && (0 == stricmp(pszAlg, "md5-sess")))
#else
    if ((NULL != pszAlg) && (0 == strcasecmp(pszAlg, "md5-sess")))
#endif
    {
        MD5_Init(&Md5Ctx);
        MD5_Update(&Md5Ctx, (unsigned char *)HA1, HASHLEN);
        MD5_Update(&Md5Ctx, (unsigned char *)":", 1);
        MD5_Update(&Md5Ctx, (unsigned char *)pszNonce, (unsigned int)strlen(pszNonce));
        MD5_Update(&Md5Ctx, (unsigned char *)":", 1);
        MD5_Update(&Md5Ctx, (unsigned char *)pszCNonce, (unsigned int)strlen(pszCNonce));
        MD5_Final((unsigned char *)HA1, &Md5Ctx);
    };

    CvtHex(HA1, SessionKey);
}

/* calculate request-digest/response-digest as per HTTP Digest spec */
static void DigestCalcResponse(
        HASHHEX      HA1,           /* H(A1) */
        const char   *pszNonce,     /* nonce from server */
        const char   *pszNonceCount,/* 8 hex digits */
        const char   *pszCNonce,    /* client nonce */
        const char   *pszQop,       /* qop-value: "", "auth", "auth-int" */
        const char   *pszMethod,    /* method from the request */
        const char   *pszDigestUri, /* requested URL */
        HASHHEX      HEntity,       /* H(entity body) if qop="auth-int" */
        HASHHEX     Response       /* request-digest or response-digest */
        )
{
    MD5_CTX Md5Ctx;
    HASH HA2;
    HASH RespHash;
    HASHHEX HA2Hex;

    // Calculate H(A2)
    MD5_Init(&Md5Ctx);
    MD5_Update(&Md5Ctx, (unsigned char *)pszMethod, (unsigned int)strlen(pszMethod));
    MD5_Update(&Md5Ctx, (unsigned char *)":", 1);
    MD5_Update(&Md5Ctx, (unsigned char *)pszDigestUri, (unsigned int)strlen(pszDigestUri));
#ifdef WIN32
    if ((NULL != pszQop) && (0 == stricmp(pszQop, "auth-int")))
#else
    if ((NULL != pszQop) && (0 == strcasecmp(pszQop, "auth-int")))
#endif
    {
        MD5_Update(&Md5Ctx, (unsigned char *)":", 1);
        MD5_Update(&Md5Ctx, (unsigned char *)HEntity, HASHHEXLEN);
    };
    MD5_Final((unsigned char *)HA2, &Md5Ctx);
    CvtHex(HA2, HA2Hex);

    // Calculate response
    MD5_Init(&Md5Ctx);
    MD5_Update(&Md5Ctx, (unsigned char *)HA1, HASHHEXLEN);
    MD5_Update(&Md5Ctx, (unsigned char *)":", 1);
    MD5_Update(&Md5Ctx, (unsigned char *)pszNonce, (unsigned int)strlen(pszNonce));
    MD5_Update(&Md5Ctx, (unsigned char *)":", 1);
    if ((NULL != pszQop) && ('\0' != (*pszQop))) // If qop exist, need to digest nc and cnonce
    {
        MD5_Update(&Md5Ctx, (unsigned char *)pszNonceCount, (unsigned int)strlen(pszNonceCount));
        MD5_Update(&Md5Ctx, (unsigned char *)":", 1);
        MD5_Update(&Md5Ctx, (unsigned char *)pszCNonce, (unsigned int)strlen(pszCNonce));
        MD5_Update(&Md5Ctx, (unsigned char *)":", 1);
        MD5_Update(&Md5Ctx, (unsigned char *)pszQop, (unsigned int)strlen(pszQop));
        MD5_Update(&Md5Ctx, (unsigned char *)":", 1);
    };
    MD5_Update(&Md5Ctx, (unsigned char *)HA2Hex, HASHHEXLEN);
    MD5_Final((unsigned char *)RespHash, &Md5Ctx);
    CvtHex(RespHash, Response);
}

/*Start of MNT 2008-12-4 for NETFile WEB Server对接 by z65940*/
static int digest_parse_auth_header(
                                            const char          *pcSrc,
                                            HTTPDigest_Fields   pstDigestFields)
{
    const char *pcPos;
    unsigned char idx;
    int bQuote;

    char pcKey[HTTPAuth_FieldNameMaxLen];
    char pcValue[HTTPAuth_FieldValueMaxLen];

    if ((NULL == pcSrc) || (NULL == pstDigestFields))
    {
        return -1;
    }
    //lint -save -e682
    memset(pstDigestFields, 0, HTTPDigest_Field_End*sizeof(char *));
    //lint -restore
    pcPos = pcSrc;
    while ('\0' != (*pcPos))
    {
        // Parse field name
        HTTPDigest_SkipSpace(pcPos);
        idx = 0;
        while (('\0' != (*pcPos)) && ('=' != (*pcPos)) && (!isspace(*pcPos)) && (idx < HTTPAuth_FieldNameMaxLen))
        {
            pcKey[idx] = (*pcPos);
            idx++;
            pcPos++;
        }
        if (('\0' == (*pcPos)) && (idx >= HTTPAuth_FieldNameMaxLen))
        {
            return -1;
        }
        pcKey[idx] = '\0';

        if ('=' == (*pcPos))
        {
            pcPos++;
        }

        // Parse the field value
        HTTPDigest_SkipSpace(pcPos);
        bQuote = 0;
        if ('"' == (*pcPos))            // Skip the "
        {
            pcPos++;
            bQuote = 1;
        }
        idx = 0;
        if (1 == bQuote)
        {
        	while (('\0' != (*pcPos)) && ('"' != (*pcPos)) &&
                   (idx < HTTPAuth_FieldValueMaxLen))
            {
                pcValue[idx] = (*pcPos);
                idx++;
                pcPos++;
            }
            if ('"' == (*pcPos))
            {
                pcPos++;
            }
            while (isspace(*pcPos))
            {
                pcPos++;
            }
        }
        else
        {
        	// Find the ending ' or space
            while (('\0' != (*pcPos)) && (',' != (*pcPos)) &&
                        (!isspace(*pcPos)) && (idx < HTTPAuth_FieldValueMaxLen))
            {
                pcValue[idx] = (*pcPos);
                idx++;
                pcPos++;
            }
        }
        if (idx >= HTTPAuth_FieldValueMaxLen)
        {
            return -1;
        }
        pcValue[idx] = '\0';

        if (',' == (*pcPos))
        {
            pcPos++;
        }

        idx = 0;
        for (idx = 0; idx < HTTPDigest_Field_End; idx++)
        {
            if (NULL != pstDigestFields[idx])
            {
                continue;
            }

            if (0 == strncasecmp(HTTPDigest_FieldNames[idx], pcKey, strlen(HTTPDigest_FieldNames[idx])))
            {
                pstDigestFields[idx] = strdup(pcValue);
                if (NULL == pstDigestFields[idx])
                {
                    return -1;
                }
                break;
            }
        }

        if ('\0' == (*pcPos))
        {
            break;
        }
    }

    if (NULL == pstDigestFields[HTTPDigest_Field_algorithm])
    {
        pstDigestFields[HTTPDigest_Field_algorithm] = strdup("MD5");
    }
    /*End of MNT 2008-12-4 for NETFile WEB Server对接 by z65940*/

    return 0;
}

static char *http_auth_gen_nonce(void)
{
    char    acTimeStr[32];
    long    ulNow;
    HASHHEX pcNonce;

    HASH acHash;
    MD5_CTX stMd5Ctx;
    char acRand[32];

    // Time as the seed of random data
    time((time_t *)(&ulNow));
    sprintf(acTimeStr, "%ld", ulNow);
    srand((unsigned int)ulNow);

    MD5_Init(&stMd5Ctx);
    MD5_Update(&stMd5Ctx, (unsigned char *)acTimeStr, (unsigned int)strlen(acTimeStr));
    MD5_Update(&stMd5Ctx, (unsigned char *)":", 1);
    sprintf(acRand, "%ld", (long)rand());

    MD5_Update(&stMd5Ctx, (unsigned char *)acRand, (unsigned int)strlen(acRand));
    MD5_Update(&stMd5Ctx, (unsigned char *)":", 1);
    MD5_Update(&stMd5Ctx, (unsigned char *)HTTPAUTH_DEFAULT_REALM,
                       (unsigned char)strlen(HTTPAUTH_DEFAULT_REALM));

    MD5_Final((unsigned char *)acHash, &stMd5Ctx);

    CvtHex(acHash, pcNonce);

    return xstrdup(pcNonce);
}

static char *build_auth_header(
                            const char              *challenge,
                            struct host_info        *target,
                            int                     trans_dir)
{
    char    buf[512];
    char    valueBuf[256];
    int     len;
    HTTPDigest_Fields digestFields;
    char    nonceCnt[12];      // nonce count value is 8LHEX
    char    *pcNonce;
    HASHHEX hSessionKey;
    HASHHEX Response;

    // Basic authorization
    if (0 == strncasecmp(challenge, HTTPAUTH_TYPE_BASIC_STR, HTTPAUTH_TYPE_BASIC_STR_LEN))
    {
        len = snprintf(buf, sizeof(buf), "%s ", HTTPAUTH_TYPE_BASIC_STR);
        snprintf(valueBuf, sizeof(valueBuf), "%s:%s", target->user, target->password);
        base64enc(valueBuf, (buf + len), (sizeof(buf) - len));
        return xstrdup(buf);
    }

    // Must be Digest authorization
    if (0 != strncasecmp(challenge, HTTPAUTH_TYPE_DIGEST_STR, HTTPAUTH_TYPE_DIGEST_STR_LEN))
        bb_error_msg_and_die("no response from server");

    // Parse
    challenge += HTTPAUTH_TYPE_DIGEST_STR_LEN;
    if (digest_parse_auth_header(challenge, digestFields) < 0)
        bb_error_msg_and_die("parse auth header error");

    // Check required fields
    if ((NULL == digestFields[HTTPDigest_Field_realm])  ||
        (NULL == digestFields[HTTPDigest_Field_nonce]) )
        bb_error_msg_and_die("lack fields");

    snprintf(nonceCnt, sizeof(nonceCnt), "%08x", 1);
    pcNonce = http_auth_gen_nonce();
    DigestCalcHA1(digestFields[HTTPDigest_Field_algorithm],
                        target->user,
                        digestFields[HTTPDigest_Field_realm],
                        target->password,
                        digestFields[HTTPDigest_Field_nonce],
                        pcNonce,
                        hSessionKey);
    DigestCalcResponse(
                        hSessionKey,
                        digestFields[HTTPDigest_Field_nonce],
                        nonceCnt,
                        pcNonce,
                        digestFields[HTTPDigest_Field_qop],
                        (2 == trans_dir) ? "POST" : "GET",
                        target->path,
                        "",     // auth-int not implemented yet, Entity should be an empty string
                        Response);

    //2 Build username, realm, nonce and uri fields
    len = snprintf(buf, sizeof(buf),
                   "%s %s=\"%s\", %s=\"%s\", %s=\"%s\", %s=\"%s\", ",
                   HTTPAUTH_TYPE_DIGEST_STR,
                   HTTPDigest_FieldNames[HTTPDigest_Field_username], target->user,
                   HTTPDigest_FieldNames[HTTPDigest_Field_realm], digestFields[HTTPDigest_Field_realm],
                   HTTPDigest_FieldNames[HTTPDigest_Field_nonce], digestFields[HTTPDigest_Field_nonce],
                   HTTPDigest_FieldNames[HTTPDigest_Field_uri], target->path);

    //2 Build the qop, nc and cnonce fields
    if (NULL != digestFields[HTTPDigest_Field_qop])    // Need to fill the nc, cnonce and qop fields
    {
        len += snprintf((buf + len), (sizeof(buf) - len), "%s=%s, %s=\"%s\", %s=\"%s\", ",
                HTTPDigest_FieldNames[HTTPDigest_Field_nc], nonceCnt,
                HTTPDigest_FieldNames[HTTPDigest_Field_cnonce], pcNonce,
                HTTPDigest_FieldNames[HTTPDigest_Field_qop], digestFields[HTTPDigest_Field_qop]);
    }

    //2 Build the algorithm field
    if (NULL != digestFields[HTTPDigest_Field_algorithm])    // Need to fill the algorithm field
    {
        len += snprintf((buf + len), (sizeof(buf) - len), "%s=\"%s\", ",
                HTTPDigest_FieldNames[HTTPDigest_Field_algorithm], digestFields[HTTPDigest_Field_algorithm]);
    }

	/*Start of MNT 2008-12-4 for NETFile WEB Server对接 by z65940*/
	if (NULL != digestFields[HTTPDigest_Field_opaque])    // Need to fill the algorithm field
    {
        len += snprintf((buf + len), (sizeof(buf) - len), "%s=\"%s\", ",
                HTTPDigest_FieldNames[HTTPDigest_Field_opaque], digestFields[HTTPDigest_Field_opaque]);
    }
	/*End of MNT 2008-12-4 for NETFile WEB Server对接 by z65940*/

    // Build the response field
    snprintf((buf + len), (sizeof(buf) - len), "%s=\"%s\"",
                HTTPDigest_FieldNames[HTTPDigest_Field_response], Response);

    return xstrdup(buf);
}

#endif

#define HTTP_UPLOAD_BOUNDARY        "HuaweiBusyboxHttpUploader"
#define HTTP_UPLOAD_BOUNDARY_LEN    (25)

#define HTTP_HEADER_MAX_LEN         (1024 * 4)
/*
 *  Prefix:
 *
 -----------------------------7d61ffc140e5a

 Content-Disposition: form-data; name="Huawei"; filename="curcfg.xml"
 Content-Type: application/octet-stream
 -----------------------------7d61ffc140e5a--
 */
static void http_upload(struct host_info             *target,
                          struct len_and_sockaddr    *lsa, tr143_diag_state_t *pstDiagState, bool bDiagFlag)
{
    int try = 5, status;
    char buf[512];
    char *s;
    int  authCnt;
    int  fileLen;
    int  localFile;             /* local file descriptor            */
    const char *pcFileName;
#if ENABLE_FEATURE_ATP_WGET_AUTHENTICATION
    char *pcAuthHeader = NULL;
#endif
    struct stat statbuf;

    int fd;
#if ENABLE_FEATURE_ATP_WGET_HTTPS
    int readLen;
    SSL *ssl        = NULL;
    //BIO *bio        = NULL;
    int biolen = 0;
    int headerlen = 0;
    int size = 0;
    char *bio        = NULL;
    ssl_line_buffer linebuf;
#endif
    FILE *sockOut = NULL;			/* socket write to web/ftp server			*/
    FILE *sockIn  = NULL;			/* socket read from web/ftp server			*/

#if defined(SUPPORT_ATP_TR143_UPLOAD_TCP) || defined(SUPPORT_ATP_TR143_UPLOAD)  
       unsigned int ulIfBytesBtimeSent = 0;
       unsigned int ulIfBytesEtimeSent = 0;
       ATP_UTIL_INTERFACE_STAT_ST stIfcStats;
   
       memset(&stIfcStats, 0, sizeof(ATP_UTIL_INTERFACE_STAT_ST));    
#endif
   if(TRUE == bDiagFlag)
    {
        fileLen = atoi(target->localFile);
        size = fileLen;
    }
    else
    {
        xfunc_error_retval = ATP_TRANS_FILE_ERR;
        localFile = xopen(target->localFile, O_RDONLY);
        if (fstat(localFile, &statbuf) < 0) {
            close_and_die("local file does not exist");
        }
        fileLen = (int)statbuf.st_size;
        xfunc_error_retval = ATP_TRANS_SYS_ERR;
        close(localFile);
    }

    pcFileName = strrchr(target->localFile, '/');
    if (NULL == pcFileName)
        pcFileName = target->localFile;
    else
        pcFileName += 1;

    // Boundary length and actual file length
    fileLen += HTTP_UPLOAD_BOUNDARY_LEN + 4 + 60 + strlen(pcFileName) +
               42 + HTTP_UPLOAD_BOUNDARY_LEN + 8;

    authCnt = 0;
    fd = -1;
    /*
     *  HTTP Upload session
     */
    do {

        if (! --try)
            bb_error_msg_and_die("too many redirections");

        /*
         * Open socket to http server
         */
        if (sockIn) fclose(sockIn);
        if (sockOut) fclose(sockOut);
        if (-1 != fd)
            close(fd);
#ifdef SUPPORT_ATP_TR143_UPLOAD_TCP
        if(TRUE == bDiagFlag)
        {
            GetCurDateTime(pstDiagState->szTcpOpenRequestTime, ATP_STRING_LEN_64);
        }
#endif
#if ENABLE_FEATURE_ATP_WGET_HTTPS
        fd = open_socket(lsa, &ssl, &sockIn, &sockOut, pstDiagState,bDiagFlag);
#else
        fd = open_socket(lsa, &sockIn, &sockOut, pstDiagState, bDiagFlag);
#endif

        if(TRUE == bDiagFlag)
        {
          #ifdef SUPPORT_ATP_TR143_UPLOAD_TCP
            GetCurDateTime(pstDiagState->szTcpOpenResponseTime, ATP_STRING_LEN_64);
          #endif
        }
       else
        {
             localFile = xopen(target->localFile, O_RDONLY);
        }

        /*Start of MNT 2008-10-13 14:40 for 传输超时检测 by z65940*/
        g_TimeoutCnt = ATP_PACK_TIMEOUT_D;
        /*End of MNT 2008-10-13 14:40 for by z65940*/
#if defined(SUPPORT_ATP_TR143_UPLOAD_TCP) || defined(SUPPORT_ATP_TR143_UPLOAD)       
         if(TRUE == bDiagFlag)
         {
               GetCurDateTime(pstDiagState->szRomTime, ATP_STRING_LEN_64);
         }
#endif
#if ENABLE_FEATURE_ATP_WGET_HTTPS
        // HTTP模式
        if (https_mode <= 0) {
            /*
             * Send HTTP request.
             */
            fprintf(sockOut, "POST %s HTTP/1.1\r\n", target->path);
			/*START ADD by c00164495 for HG622u 对于只允许通过域名访问的服务器下载失败 2011-02-22*/
            if (NULL != pcDomain)
            {
                fprintf(sockOut, "Host: %s\r\nUser-Agent: Wget\r\n", pcDomain);
            }
            else
            {
                fprintf(sockOut, "Host: %s:%d\r\nUser-Agent: Wget\r\n",
                        target->host, target->port);
            }

            fprintf(sockOut, "Content-Type: multipart/form-data; boundary=%s\r\nCache-Control:no-cache\r\n", HTTP_UPLOAD_BOUNDARY);

        #if ENABLE_FEATURE_ATP_WGET_AUTHENTICATION
            if (NULL != pcAuthHeader) {
                fprintf(sockOut, "Authorization: %s\r\n", pcAuthHeader);
            }
        #endif
            if (NULL != pcDomain)
            {
                fprintf(sockOut, "Host: %s\r\nUser-Agent: Wget\r\nContent-Length: %d\r\n\r\n",
                        pcDomain, fileLen);
            }
            else
            {
                fprintf(sockOut, "Host: %s:%d\r\nUser-Agent: Wget\r\nContent-Length: %d\r\n\r\n",
                        target->host, target->port, fileLen);
            }
			/*END ADD by c00164495 for HG622u 对于只允许通过域名访问的服务器下载失败 2011-02-22*/
            //fprintf(sfp, "Connection: close\r\n\r\n");

            // HTTP_UPLOAD_BOUNDARY_LEN+4+60+strlen(filename)
            fprintf(sockOut, "--%s\r\nContent-Disposition: form-data; name=\"Huawei\"; filename=\"%s\"\r\n", HTTP_UPLOAD_BOUNDARY, pcFileName);
            // 42 Bytes
            fprintf(sockOut, "Content-Type: application/octet-stream\r\n\r\n");

            fflush(sockOut);

            /*
        	 * Transfer file
        	 */
            /*Start of MNT 2008-10-13 15:24 for 传输超时检测  by z65940*/
              

           if(TRUE == bDiagFlag)
           {
              #if defined(SUPPORT_ATP_TR143_UPLOAD_TCP) || defined(SUPPORT_ATP_TR143_UPLOAD) 
                GetCurDateTime(pstDiagState->szBomTime, ATP_STRING_LEN_64);  
                ATP_UTIL_GetIfcStatInfo(pstDiagState->szIfName, &stIfcStats);    
                ulIfBytesBtimeSent= stIfcStats.ulBytesSent;
               if(diag_copy_fd_with_timeout( fileno(sockOut), size) == -1)
               {
                 SetUpDiagState(pstDiagState, "Error_NoTransferComplete", bDiagFlag); 
                 exit(ATP_TRANS_SYS_ERR);
        	 }
             #endif
           }
           else
           {
        #if 0
        	if (bb_copyfd_eof(localFile, fileno(sockOut)) == -1) {
        #else
/*
1082704676 2011-11-02 for by j00196195
需要传入文件路径，因为文件句柄已经失效
*/		
        	if (atp_copy_fd_with_timeout(localFile, fileno(sockOut), 0 , NULL) == -1) {
        #endif
            /*End of MNT 2008-10-13 15:24 by z65940*/
        		exit(ATP_TRANS_SYS_ERR);
        	}
            close(localFile);
            localFile = -1;
          }

            // HTTP_UPLOAD_BOUNDARY_LEN+8
            fprintf(sockOut, "\r\n--%s--\r\n", HTTP_UPLOAD_BOUNDARY);
            fflush(sockOut);
        } else {
            //bio = BIO_new(BIO_s_mem());
            biolen = HTTP_HEADER_MAX_LEN;
            headerlen = 0;
            bio = (char *)malloc(biolen);
            if (NULL == bio) {
                close_delete_and_die("bio new err");
            }

            /*
             * Send HTTP request.
             */
            biolen -= snprintf((bio + headerlen), biolen, "POST %s HTTP/1.1\r\n", target->path);
            headerlen = HTTP_HEADER_MAX_LEN - biolen;
			/*START ADD by c00164495 for HG622u 对于只允许通过域名访问的服务器下载失败 2011-02-22*/
            if (NULL != pcDomain)
            {
                biolen -= snprintf((bio + headerlen), biolen, "Host: %s\r\nUser-Agent: Wget\r\n", pcDomain);
            }
            else
            {
                biolen -= snprintf((bio + headerlen), biolen, "Host: %s:%d\r\nUser-Agent: Wget\r\n",
                        target->host, target->port);
            }
			/*END ADD by c00164495 for HG622u 对于只允许通过域名访问的服务器下载失败 2011-02-22*/
            headerlen = HTTP_HEADER_MAX_LEN - biolen;

            biolen -= snprintf((bio + headerlen), biolen, "Content-Type: multipart/form-data; boundary=%s\r\nCache-Control:no-cache\r\n", HTTP_UPLOAD_BOUNDARY);
            headerlen = HTTP_HEADER_MAX_LEN - biolen;

        #if ENABLE_FEATURE_ATP_WGET_AUTHENTICATION
            if (NULL != pcAuthHeader) {
                biolen -= snprintf((bio + headerlen), biolen, "Authorization: %s\r\n", pcAuthHeader);
                headerlen = HTTP_HEADER_MAX_LEN - biolen;
            }
        #endif
			/*START ADD by c00164495 for HG622u 对于只允许通过域名访问的服务器下载失败 2011-02-22*/
            if (NULL != pcDomain)
            {
                biolen -= snprintf((bio + headerlen), biolen, "Host: %s\r\nUser-Agent: Wget\r\nContent-Length: %d\r\n\r\n",
                       pcDomain, fileLen);
            }
            else
            {
                biolen -= snprintf((bio + headerlen), biolen, "Host: %s:%d\r\nUser-Agent: Wget\r\nContent-Length: %d\r\n\r\n",
                       target->host, target->port, fileLen);
            }
			/*END ADD by c00164495 for HG622u 对于只允许通过域名访问的服务器下载失败 2011-02-22*/
            headerlen = HTTP_HEADER_MAX_LEN - biolen;

            //fprintf(sfp, "Connection: close\r\n\r\n");

            // HTTP_UPLOAD_BOUNDARY_LEN+4+60+strlen(filename)
            biolen -= snprintf((bio + headerlen), biolen, "--%s\r\nContent-Disposition: form-data; name=\"Huawei\"; filename=\"%s\"\r\n", HTTP_UPLOAD_BOUNDARY, pcFileName);
            headerlen = HTTP_HEADER_MAX_LEN - biolen;
            // 42 Bytes
            biolen -= snprintf((bio + headerlen), biolen, "Content-Type: application/octet-stream\r\n\r\n");
            headerlen = HTTP_HEADER_MAX_LEN - biolen;

            SSL_write(ssl, (const void *)bio, headerlen);

            /*
        	 * Transfer file
        	 */
        	while (1) {
                /*Start of MNT 2008-10-13 14:40 for 传输超时检测 by z65940*/
                g_TimeoutCnt = ATP_PACK_TIMEOUT_D;
                /*End of MNT 2008-10-13 14:40 for by z65940*/
                readLen = safe_read(localFile, bio, HTTP_HEADER_MAX_LEN);
                if (!readLen) { /* eof - all done */
        			break;
        		}
                SSL_write(ssl, bio, readLen);
            }
            close(localFile);
            localFile = -1;

            // HTTP_UPLOAD_BOUNDARY_LEN+8
            headerlen = 0;
            biolen = HTTP_HEADER_MAX_LEN;
            biolen -= snprintf((bio + headerlen), biolen, "\r\n--%s--\r\n", HTTP_UPLOAD_BOUNDARY);
            headerlen = HTTP_HEADER_MAX_LEN - biolen;
            readLen = SSL_write(ssl, bio, headerlen);

            free((void *)bio);
            bio = NULL;
        }
#else
        /*
         * Send HTTP request.
         */
        fprintf(sockOut, "POST %s HTTP/1.1\r\n", target->path);
		/*START ADD by c00164495 for HG622u 对于只允许通过域名访问的服务器下载失败 2011-02-22*/
        if (NULL != pcDomain)
        {
            fprintf(sockOut, "Host: %s\r\nUser-Agent: Wget\r\n", pcDomain);
        }
        else
        {
            fprintf(sockOut, "Host: %s:%d\r\nUser-Agent: Wget\r\n",
                    target->host, target->port);
        }
		/*END ADD by c00164495 for HG622u 对于只允许通过域名访问的服务器下载失败 2011-02-22*/

        fprintf(sockOut, "Content-Type: multipart/form-data; boundary=%s\r\nCache-Control:no-cache\r\n", HTTP_UPLOAD_BOUNDARY);

    #if ENABLE_FEATURE_ATP_WGET_AUTHENTICATION
        if (NULL != pcAuthHeader) {
            fprintf(sockOut, "Authorization: %s\r\n", pcAuthHeader);
        }
    #endif
		/*START ADD by c00164495 for HG622u 对于只允许通过域名访问的服务器下载失败 2011-02-22*/
        if (NULL != pcDomain)
        {
            fprintf(sockOut, "Host: %s\r\nUser-Agent: Wget\r\nContent-Length: %d\r\n\r\n",
                    pcDomain, fileLen);
        }
        else
        {
            fprintf(sockOut, "Host: %s:%d\r\nUser-Agent: Wget\r\nContent-Length: %d\r\n\r\n",
                    target->host, target->port, fileLen);
        }
		/*END ADD by c00164495 for HG622u 对于只允许通过域名访问的服务器下载失败 2011-02-22*/

        //fprintf(sfp, "Connection: close\r\n\r\n");

        // HTTP_UPLOAD_BOUNDARY_LEN+4+60+strlen(filename)
        fprintf(sockOut, "--%s\r\nContent-Disposition: form-data; name=\"Huawei\"; filename=\"%s\"\r\n", HTTP_UPLOAD_BOUNDARY, pcFileName);
        // 42 Bytes
        fprintf(sockOut, "Content-Type: application/octet-stream\r\n\r\n");

        fflush(sockOut);

        /*
    	 * Transfer file
    	 */
        /*Start of MNT 2008-10-13 15:26 for 传输超时检测  by z65940*/
         if(TRUE == bDiagFlag)
         {
           #if defined(SUPPORT_ATP_TR143_UPLOAD_TCP) || defined(SUPPORT_ATP_TR143_UPLOAD)
                GetCurDateTime(pstDiagState->szBomTime, ATP_STRING_LEN_64);  
                ATP_UTIL_GetIfcStatInfo(pstDiagState->szIfName, &stIfcStats);    
                ulIfBytesBtimeSent= stIfcStats.ulBytesSent; 
               if(diag_copy_fd_with_timeout( fileno(sockOut), size) == -1)
               {
                 SetUpDiagState(pstDiagState, "Error_NoTransferComplete", bDiagFlag); 
                 exit(ATP_TRANS_SYS_ERR);
        	 }
            #endif
         }
         else
         {            
/*
1082704676 2011-11-02 for by j00196195
需要传入文件路径，因为文件句柄已经失效
*/	
    	if (atp_copy_fd_with_timeout(localFile, fileno(sockOut), 0, NULL) == -1) {
        /*End of MNT 2008-10-13 15:26 by z65940*/
    		exit(ATP_TRANS_SYS_ERR);
    	}
        close(localFile);
        localFile = -1;
    	  }

        // HTTP_UPLOAD_BOUNDARY_LEN+8
        fprintf(sockOut, "\r\n--%s--\r\n", HTTP_UPLOAD_BOUNDARY);
        fflush(sockOut);
#endif

        // HTTPS时，先创建BIO用户缓冲
#if ENABLE_FEATURE_ATP_WGET_HTTPS
        if (NULL != bio) {
            free((void *)bio);
            bio = NULL;
        }
        if (https_mode > 0) {
            bio = (char *)malloc(HTTP_HEADER_MAX_LEN);
            if (NULL == bio) {
                close_delete_and_die("bio new err");
            }
            linebuf.pcBuf = bio;
            linebuf.lTotalLen = HTTP_HEADER_MAX_LEN;
            linebuf.lReadLen = 0;
        }
#endif

    /*Start of MNT 2008-10-13 14:40 for 传输超时检测 by z65940*/
    g_TimeoutCnt = ATP_PACK_TIMEOUT_D;  // 收取每个包最长45秒
    /*End of MNT 2008-10-13 14:40 for by z65940*/

        /*
         * Retrieve HTTP response line and check for "200" status code.
         */
read_response:
    #if ENABLE_FEATURE_ATP_WGET_HTTPS
        if (https_mode <= 0) {
            if (fgets(buf, sizeof(buf), sockIn) == NULL) {
                SetUpDiagState(pstDiagState, "Error_NoResponse", bDiagFlag); 
                close_delete_and_die("no response from server");
            }
        }
        else {
            ssl_fgets(buf, sizeof(buf), (void *)(&linebuf), ssl);
        }
    #else
        if (fgets(buf, sizeof(buf), sockIn) == NULL) {
            SetUpDiagState(pstDiagState, "Error_NoResponse", bDiagFlag); 
            bb_error_msg_and_die("no response from server");
        }
    #endif

        for (s = buf ; *s != '\0' && !isspace(*s) ; ++s)
        ;
        for ( ; isspace(*s) ; ++s)
        ;
        switch (status = atoi(s)) {
            case 0:
            case 100:
            #if ENABLE_FEATURE_ATP_WGET_HTTPS
                if (https_mode <= 0) {
                    while (gethdr(buf, sizeof(buf), (void *)sockIn, ssl) != NULL);
                } else {
                    while (gethdr(buf, sizeof(buf), (void *)(&linebuf), ssl) != NULL);
                }
            #else
                while (gethdr(buf, sizeof(buf), sockIn) != NULL) ;
            #endif
                goto read_response;
            case 200:
                break;
            case 300:   /* redirection */
            case 301:
            case 302:
            case 303:
                authCnt = 0;
                break;
            case 206:
				if (NULL != g_pcTransBegin)
					break;
				/*FALLTHRU*/
            case 401:
                authCnt++;
                break;
            case 404:
                xfunc_error_retval = ATP_TRANS_FILE_ERR;
                SetUpDiagState(pstDiagState, "Error_NoResponse", bDiagFlag); 
                close_delete_and_die("404 error");
                break;
                /*FALLTHRU*/
            default:
                /* Show first line only and kill any ESC tricks */
				buf[strcspn(buf, "\n\r\x1b")] = '\0';
                            SetUpDiagState(pstDiagState, "Error_NoResponse", bDiagFlag); 
				bb_error_msg_and_die("server returned error: %s", buf);
        }

        /*Start of MNT 2008-10-13 14:40 for 传输超时检测 by z65940*/
        g_TimeoutCnt = ATP_PACK_TIMEOUT_D;  // 收取每个包最长45秒
        /*End of MNT 2008-10-13 14:40 for by z65940*/

        /*
         * Retrieve HTTP headers.
         */
    #if ENABLE_FEATURE_ATP_WGET_HTTPS
        if (https_mode <= 0) {
            s = gethdr(buf, sizeof(buf), (void *)sockIn, ssl);
        } else {
            s = gethdr(buf, sizeof(buf), (void *)(&linebuf), ssl);
        }
        while (s != NULL) {
    #else
        while ((s = gethdr(buf, sizeof(buf), sockIn)) != NULL) {
    #endif
            /* gethdr did already convert the "FOO:" string to lowercase */
			smalluint key = index_in_strings(keywords, *&buf) + 1;
        #if ENABLE_FEATURE_ATP_WGET_AUTHENTICATION
            if (key == KEY_authenticate) {
                //unsigned long value;
                //if (safe_strtoul(s, &value));
                // Auth error
                xfunc_error_retval = ATP_TRANS_AUTH_ERR;
                if ((0 == authCnt)          || (authCnt > 2) ||
                    (NULL == target->user)  || (NULL == target->password))
                {
                    bb_error_msg_and_die("auth error");
                }
                authCnt++;
				/*Start of MNT 2008-12-4 for NETFile WEB Server对接 by z65940*/
				if ((NULL == pcAuthHeader) || (NULL == strstr(pcAuthHeader, HTTPAUTH_TYPE_DIGEST_STR)))
				{
					pcAuthHeader = build_auth_header(s, target, 2);
				}
				/*End of MNT 2008-12-4 for NETFile WEB Server对接 by z65940*/
                xfunc_error_retval = ATP_TRANS_SYS_ERR;
            #if ENABLE_FEATURE_ATP_WGET_HTTPS
                if (https_mode <= 0) {
                    s = gethdr(buf, sizeof(buf), (void *)sockIn, ssl);
                } else {
                    s = gethdr(buf, sizeof(buf), (void *)(&linebuf), ssl);
                }
            #endif
                continue;
            }
        #endif
            if (key == KEY_location) {
                if (s[0] == '/')
                    target->path = xstrdup(s+1);
                else {
                    parse_url(s, target);
                    free(lsa);
					lsa = xhost2sockaddr(target->host, target->port);
                    //bb_lookup_host(s_in, target->host);
                    //s_in->sin_port = target->port;
                    break;
                }
            }
        #if ENABLE_FEATURE_ATP_WGET_HTTPS
            if (https_mode <= 0) {
                s = gethdr(buf, sizeof(buf), (void *)sockIn, ssl);
            } else {
                s = gethdr(buf, sizeof(buf), (void *)(&linebuf), ssl);
            }
        #endif
        }

    #if ENABLE_FEATURE_ATP_WGET_HTTPS
        if (NULL != bio) {
            free((void *)bio);
            bio = NULL;
        }
        if (https_mode > 0) {
            linebuf.pcBuf = NULL;
            linebuf.lTotalLen = 0;
            linebuf.lReadLen = 0;
        }
    #endif
    } while(status >= 300);
   #if defined(SUPPORT_ATP_TR143_UPLOAD_TCP) || defined(SUPPORT_ATP_TR143_UPLOAD)
       if(TRUE == bDiagFlag)
       {
            GetCurDateTime(pstDiagState->szEomTime, ATP_STRING_LEN_64);
            ATP_UTIL_GetIfcStatInfo(pstDiagState->szIfName, &stIfcStats);    
             ulIfBytesEtimeSent = stIfcStats.ulBytesSent;
             pstDiagState->ulTotalBytesSent = ulIfBytesEtimeSent - ulIfBytesBtimeSent;
             SetUpDiagState(pstDiagState, "Completed", bDiagFlag); 
       }
 #endif
}

static void http_download(struct host_info              *target,
	                         struct len_and_sockaddr    *lsa, tr143_diag_state_t *pstDiagState, bool bDiagFlag)
{
    int try=5, status;
    char buf[512];
    char *s;
    int skipBytes;
    int curWrittenLen;
    int curSkipped;
    int got_clen = 0;			/* got content-length: from server	*/
#if ENABLE_FEATURE_ATP_WGET_ZIP
    gzFile localFile;
#else
    int localFile;			    /* local file						*/
#endif

#if ENABLE_FEATURE_ATP_WGET_AUTHENTICATION
    char *pcAuthHeader = NULL;
#endif
    int authCnt;

#if ENABLE_FEATURE_ATP_WGET_HTTPS
    SSL *ssl        = NULL;
    int biolen = 0;
    int headerlen = 0;
    char *bio        = NULL;
    ssl_line_buffer linebuf;
    //BIO *bio        = NULL;
    //BUF_MEM *pstBuf = NULL;
#endif
    FILE *sockOut   = NULL;			/* socket write to web/ftp server			*/
    FILE *sockIn    = NULL;			/* socket read from web/ftp server			*/

    unsigned int ulBytesRecvCount = 0;
#if defined(SUPPORT_ATP_TR143_DOWNLOAD_TCP) ||defined(SUPPORT_ATP_TR143_DOWNLOAD)
    unsigned int ulExpectBytesRecv = 0;    
    unsigned int ulIfBytesBtimeRecv = 0;
    unsigned int ulIfBytesEtimeRecv = 0;
    ATP_UTIL_INTERFACE_STAT_ST stIfcStats;
    
    memset(&stIfcStats, 0, sizeof(ATP_UTIL_INTERFACE_STAT_ST));
#endif
    authCnt = 0;
    /*
     *  HTTP Download session
     */
    do {
        got_clen = 0;
        chunked = 0;

        if (! --try)
            close_delete_and_die("too many redirections");
        /*
         * Open socket to http server
         */
        if (sockIn) fclose(sockIn);
        if (sockOut) fclose(sockOut);
#ifdef SUPPORT_ATP_TR143_DOWNLOAD_TCP
        if(TRUE == bDiagFlag)
        {
            GetCurDateTime(pstDiagState->szTcpOpenRequestTime, ATP_STRING_LEN_64);
        }
#endif
#if ENABLE_FEATURE_ATP_WGET_HTTPS
        open_socket(lsa, &ssl, &sockIn, &sockOut, pstDiagState, bDiagFlag);
#else                   
        open_socket(lsa, &sockIn, &sockOut, pstDiagState, bDiagFlag); 
#endif
#ifdef SUPPORT_ATP_TR143_DOWNLOAD_TCP
        if(TRUE == bDiagFlag)
        {
            GetCurDateTime(pstDiagState->szTcpOpenResponseTime, ATP_STRING_LEN_64);
        }
#endif
        /*Start of MNT 2008-10-13 14:40 for 传输超时检测 by z65940*/
        g_TimeoutCnt = ATP_PACK_TIMEOUT_D;
        /*End of MNT 2008-10-13 14:40 for by z65940*/
#if defined(SUPPORT_ATP_TR143_DOWNLOAD_TCP) ||defined(SUPPORT_ATP_TR143_DOWNLOAD)
      if(TRUE == bDiagFlag)
      {        
         GetCurDateTime(pstDiagState->szRomTime, ATP_STRING_LEN_64);
      }
#endif
#if ENABLE_FEATURE_ATP_WGET_HTTPS

        // HTTP模式
        if (https_mode <= 0) {
            /*
             * Send HTTP request.
             */
            ulBytesRecvCount += (unsigned int)fprintf(sockOut, "GET %s HTTP/1.1\r\n", target->path);
            /*START ADD by c00164495 for HG622u 对于只允许通过域名访问的服务器下载失败 2011-02-22*/
            if (NULL != pcDomain)
            {
                ulBytesRecvCount += (unsigned int)fprintf(sockOut, "Host: %s\r\nUser-Agent: Wget\r\n", pcDomain);
            }
            else
            {
                ulBytesRecvCount += (unsigned int)fprintf(sockOut, "Host: %s:%d\r\nUser-Agent: Wget\r\n", target->host, target->port);
            }
            /*END ADD by c00164495 for HG622u 对于只允许通过域名访问的服务器下载失败 2011-02-22*/
#if ENABLE_FEATURE_ATP_WGET_AUTHENTICATION
            if (NULL != pcAuthHeader) {
                ulBytesRecvCount += (unsigned int)fprintf(sockOut, "Authorization: %s\r\n", pcAuthHeader);
            }
#endif

            //g_pcTransBegin
            if ((NULL != g_pcTransEnd) && (g_lTransEnd >= 0)) {
                if (NULL == g_pcTransBegin) {
                   g_pcTransBegin = "";
                }
                ulBytesRecvCount += (unsigned int)fprintf(sockOut, "Range: bytes=%s-%d\r\n", g_pcTransBegin, g_lTransEnd);
            }
            else if ((NULL != g_pcTransBegin) && (NULL == g_pcTransEnd)) {
                ulBytesRecvCount += (unsigned int)fprintf(sockOut, "Range: bytes=%s-\r\n", g_pcTransBegin);
            }
   
            ulBytesRecvCount += (unsigned int)fprintf(sockOut, "Connection: close\r\n\r\n");
            fflush(sockOut);
        } else {

            //bio = BIO_new(BIO_s_mem());
            biolen = HTTP_HEADER_MAX_LEN;
            headerlen = 0;
            bio = (char *)malloc(biolen);
            if (NULL == bio) {
                close_delete_and_die("bio new err");
            }

            biolen -= snprintf((bio + headerlen), biolen, "GET %s HTTP/1.1\r\n", target->path);
            headerlen = HTTP_HEADER_MAX_LEN - biolen;
            /*START ADD by c00164495 for HG622u 对于只允许通过域名访问的服务器下载失败 2011-02-22*/
            if (NULL != pcDomain)
            {
                biolen -= snprintf((bio + headerlen), biolen, "Host: %s\r\nUser-Agent: Wget\r\n", pcDomain);
            }
            else
            {
                biolen -= snprintf((bio + headerlen), biolen, "Host: %s:%d\r\nUser-Agent: Wget\r\n", target->host, target->port);
            }
            /*END ADD by c00164495 for HG622u 对于只允许通过域名访问的服务器下载失败 2011-02-22*/
            headerlen = HTTP_HEADER_MAX_LEN - biolen;

        #if ENABLE_FEATURE_ATP_WGET_AUTHENTICATION
            if (NULL != pcAuthHeader) {
                biolen -= snprintf((bio + headerlen), biolen, "Authorization: %s\r\n", pcAuthHeader);
                headerlen = HTTP_HEADER_MAX_LEN - biolen;
            }
        #endif

            //g_pcTransBegin
            if ((NULL != g_pcTransEnd) && (g_lTransEnd >= 0)) {
                if (NULL == g_pcTransBegin) {
                    g_pcTransBegin = "";
                }
                biolen -= snprintf((bio + headerlen), biolen, "Range: bytes=%s-%d\r\n", g_pcTransBegin, g_lTransEnd);
            }
            else if ((NULL != g_pcTransBegin) && (NULL == g_pcTransEnd)) {
                biolen -= snprintf((bio + headerlen), biolen, "Range: bytes=%s-\r\n", g_pcTransBegin);
            }
            headerlen = HTTP_HEADER_MAX_LEN - biolen;

            biolen -= snprintf((bio + headerlen), biolen, "Connection: close\r\n\r\n");
            headerlen = HTTP_HEADER_MAX_LEN - biolen;

            SSL_write(ssl, (const void *)bio, headerlen);
            ulBytesRecvCount += (unsigned int)headerlen;
   
        }
#else
        /*
         * Send HTTP request.
         */
        ulBytesRecvCount += (unsigned int)fprintf(sockOut, "GET %s HTTP/1.1\r\n", target->path);
        /*START ADD by c00164495 for HG622u 对于只允许通过域名访问的服务器下载失败 2011-02-22*/
        if (NULL != pcDomain)
        {
           ulBytesRecvCount += (unsigned int)fprintf(sockOut, "Host: %s\r\nUser-Agent: Wget\r\n", pcDomain);
        }
        else
        {
           ulBytesRecvCount +=  (unsigned int)fprintf(sockOut, "Host: %s:%d\r\nUser-Agent: Wget\r\n", target->host, target->port);
        }
        /*END ADD by c00164495 for HG622u 对于只允许通过域名访问的服务器下载失败 2011-02-22*/

#if ENABLE_FEATURE_ATP_WGET_AUTHENTICATION
        if (NULL != pcAuthHeader) {
            ulBytesRecvCount += (unsigned int)fprintf(sockOut, "Authorization: %s\r\n", pcAuthHeader);
        }
#endif

        //g_pcTransBegin
        if ((NULL != g_pcTransEnd) && (g_lTransEnd >= 0)) {
            if (NULL == g_pcTransBegin) {
                g_pcTransBegin = "";
            }
            ulBytesRecvCount += (unsigned int)fprintf(sockOut, "Range: bytes=%s-%d\r\n", g_pcTransBegin, g_lTransEnd);
        }
        else if ((NULL != g_pcTransBegin) && (NULL == g_pcTransEnd)) {
            ulBytesRecvCount += (unsigned int)fprintf(sockOut, "Range: bytes=%s-\r\n", g_pcTransBegin);
        }

        ulBytesRecvCount += (unsigned int)fprintf(sockOut, "Connection: close\r\n\r\n");
        fflush(sockOut);
#endif
#if defined(SUPPORT_ATP_TR143_DOWNLOAD_TCP) ||defined(SUPPORT_ATP_TR143_DOWNLOAD)
        if(TRUE == bDiagFlag)
        {               
            ulExpectBytesRecv +=  ulBytesRecvCount;
        }
#endif
        // HTTPS时，先创建BIO用户缓冲
#if ENABLE_FEATURE_ATP_WGET_HTTPS
        if (NULL != bio) {
            free((void *)bio);
            bio = NULL;
        }
        if (https_mode > 0) {
            bio = (char *)malloc(HTTP_HEADER_MAX_LEN);
            if (NULL == bio) {
                close_delete_and_die("bio new err");
            }
            linebuf.pcBuf = bio;
            linebuf.lTotalLen = HTTP_HEADER_MAX_LEN;
            linebuf.lReadLen = 0;
        }
#endif

        /*Start of MNT 2008-10-13 14:40 for 传输超时检测 by z65940*/
        g_TimeoutCnt = ATP_PACK_TIMEOUT_D;
        /*End of MNT 2008-10-13 14:40 for by z65940*/

       /*
        * Retrieve HTTP response line and check for "200" status code.
        */
 #if defined(SUPPORT_ATP_TR143_DOWNLOAD_TCP) ||defined(SUPPORT_ATP_TR143_DOWNLOAD)
      if(TRUE == bDiagFlag)
      { 
          GetCurDateTime(pstDiagState->szBomTime, ATP_STRING_LEN_64);  
          ATP_UTIL_GetIfcStatInfo(pstDiagState->szIfName, &stIfcStats);    
          ulIfBytesBtimeRecv = stIfcStats.ulBytesReceived; 
      }
#endif
read_response:
    #if ENABLE_FEATURE_ATP_WGET_HTTPS
        if (https_mode <= 0) {
            if (fgets(buf, sizeof(buf), sockIn) == NULL) {
                SetDownDiagState(pstDiagState, "Error_NoResponse", bDiagFlag);   
                close_delete_and_die("no response from server");
            }
        }
        else {
            ssl_fgets(buf, sizeof(buf), (void *)(&linebuf), ssl);
        }
    #else
        if (fgets(buf, sizeof(buf), sockIn) == NULL) {
            SetDownDiagState(pstDiagState, "Error_NoResponse", bDiagFlag);   
            close_delete_and_die("no response from server");
        }
    #endif

        s = buf;
		s = skip_non_whitespace(s);
		s = skip_whitespace(s);
        // FIXME: no error check
		// xatou wouldn't work: "200 OK"
        switch (status = atoi(s)) {
            case 0:
            case 100:
        #if ENABLE_FEATURE_ATP_WGET_HTTPS
            if (https_mode <= 0) {
                while (gethdr(buf, sizeof(buf), (void *)sockIn, ssl) != NULL);
            } else {
                while (gethdr(buf, sizeof(buf), (void *)(&linebuf), ssl) != NULL);
            }
        #else
                while (gethdr(buf, sizeof(buf), sockIn) != NULL);
        #endif
                goto read_response;
            case 200:
                break;
            case 300:   /* redirection */
            case 301:
            case 302:
            case 303:
                authCnt = 0;
                break;
            case 206:
                if (0 <= g_lTransEnd)
                    break;
                /*FALLTHRU*/
            case 404:
                xfunc_error_retval = ATP_TRANS_FILE_ERR;
                SetDownDiagState(pstDiagState, "Error_NoResponse", bDiagFlag);   
                close_delete_and_die("404 error");
                break;
            case 401:
                authCnt++;
                break;
                /*FALLTHRU*/
            default:
                /* Show first line only and kill any ESC tricks */
				buf[strcspn(buf, "\n\r\x1b")] = '\0';
                            SetDownDiagState(pstDiagState, "Error_NoResponse", bDiagFlag);   
				bb_error_msg_and_die("server returned error: %s", buf);
        }

        /*
         * Retrieve HTTP headers.
         */
    #if ENABLE_FEATURE_ATP_WGET_HTTPS
        if (https_mode <= 0) {
            s = gethdr(buf, sizeof(buf), (void *)sockIn, ssl);
        } else {
            s = gethdr(buf, sizeof(buf), (void *)(&linebuf), ssl);
        }
        while (s != NULL) {
    #else
        while ((s = gethdr(buf, sizeof(buf), sockIn)) != NULL) {
    #endif
            /* gethdr did already convert the "FOO:" string to lowercase */
			smalluint key = index_in_strings(keywords, *&buf) + 1;
        #if ENABLE_FEATURE_ATP_WGET_AUTHENTICATION
            if (key == KEY_authenticate) {
            //if (strcasecmp(buf, "WWW-Authenticate") == 0) {
                //unsigned long value;
                //if (safe_strtoul(s, &value));
                // Auth error
                xfunc_error_retval = ATP_TRANS_AUTH_ERR;
                if ((0 == authCnt)          || (authCnt > 2) ||
                    (NULL == target->user)  || (NULL == target->password))
                {
                    close_delete_and_die("auth error");
                }
                authCnt++;
				/*Start of MNT 2008-12-4 for NETFile WEB Server对接 by z65940*/
				if ((NULL == pcAuthHeader) || (NULL == strstr(pcAuthHeader, HTTPAUTH_TYPE_DIGEST_STR)))
				{
					pcAuthHeader = build_auth_header(s, target, 1);
				}
				/*End of MNT 2008-12-4 for NETFile WEB Server对接 by z65940*/
                xfunc_error_retval = ATP_TRANS_SYS_ERR;
            #if ENABLE_FEATURE_ATP_WGET_HTTPS
                if (https_mode <= 0) {
                    s = gethdr(buf, sizeof(buf), (void *)sockIn, ssl);
                } else {
                    s = gethdr(buf, sizeof(buf), (void *)(&linebuf), ssl);
                }
            #endif
                continue;
            }
        #endif
            if (key == KEY_content_length) {
                content_len = BB_STRTOOFF(s, NULL, 10);                
              #if defined(SUPPORT_ATP_TR143_DOWNLOAD_TCP) ||defined(SUPPORT_ATP_TR143_DOWNLOAD)
                if(TRUE == bDiagFlag)
                {
                    ulExpectBytesRecv += (unsigned int)content_len;
                }
               #endif
                if (errno || (content_len < 0) ||
                    ((g_lMaxTrans > 0) && (content_len > g_lMaxTrans))) {
                    xfunc_error_retval = ATP_TRANS_FILE_ERR;
                    SetDownDiagState(pstDiagState, "Error_NoResponse", bDiagFlag);   
                    close_delete_and_die("file too large");
				}
                got_clen = 1;
            #if ENABLE_FEATURE_ATP_WGET_HTTPS
                if (https_mode <= 0) {
                    s = gethdr(buf, sizeof(buf), (void *)sockIn, ssl);
                } else {
                    s = gethdr(buf, sizeof(buf), (void *)(&linebuf), ssl);
                }
            #endif
                continue;
            }
            if (key == KEY_transfer_encoding) {
                if (index_in_strings(keywords, str_tolower(s)) + 1 != KEY_chunked){
                    SetDownDiagState(pstDiagState, "Error_NoResponse", bDiagFlag);  
                    close_delete_and_die("server wants to do %s transfer encoding", s);
                }
                chunked = got_clen = 1;
            }
            if (key == KEY_location) {
                if (s[0] == '/')
                    target->path = xstrdup(s+1);
                else {
                    parse_url(s, target);
                    //server.host = target.host;
					//server.port = target.port;
                    free(lsa);
					lsa = xhost2sockaddr(target->host, (int)(target->port));

                    //bb_lookup_host(s_in, target->host);
                    //s_in->sin_port = target->port;
                    break;
                }
            }
        #if ENABLE_FEATURE_ATP_WGET_HTTPS
            if (https_mode <= 0) {
                s = gethdr(buf, sizeof(buf), (void *)sockIn, ssl);
            } else {
                s = gethdr(buf, sizeof(buf), (void *)(&linebuf), ssl);
            }
        #endif
        }

    #if ENABLE_FEATURE_ATP_WGET_HTTPS
        if (status >= 300)
        {
            if (NULL != bio) {
                free((void *)bio);
                bio = NULL;
            }
            if (https_mode > 0) {
                linebuf.pcBuf = NULL;
                linebuf.lTotalLen = 0;
                linebuf.lReadLen = 0;
            }
        }
    #endif
    } while(status >= 300);

    /*Start of MNT 2008-10-13 14:40 for 传输超时检测 by z65940*/
    g_TimeoutCnt = -1;
    /*End of MNT 2008-10-13 14:40 for by z65940*/

    /*
	 * Open the download local file stream
	 */
#if ENABLE_FEATURE_ATP_WGET_ZIP
    if (!g_bCompress) {
    	localFile = (gzFile)xopen(target->localFile, O_WRONLY | O_CREAT | O_TRUNC);
    } else {
        localFile = gzopen(target->localFile, "wb9");
        if (NULL == localFile) {
            SetDownDiagState(pstDiagState, "Error_NoResponse", bDiagFlag);  
            bb_error_msg_and_die("open zip file");
        }
    }
#else
    localFile = xopen(target->localFile, O_WRONLY | O_CREAT | O_TRUNC);
#endif

    skipBytes = 0;
    if (NULL != g_pcTransBegin)
    {
        skipBytes = atoi(g_pcTransBegin);
    }
    curSkipped    = 0;
    curWrittenLen = 0;

    /*
	 * Retrieve file
	 */
    if (chunked)
		goto get_clen;
#if ENABLE_FEATURE_ATP_WGET_HTTPS
    else if (https_mode > 0) {
        // 先把多余的一部分报文写入到文件中
        if ((NULL != linebuf.pcBuf) && (linebuf.lReadLen > 0))
        {
            if ((g_lTransEnd >= 0) && (206 != status))  // For fileservers do not understand range
            //if (1)
            {
                if (skipBytes > 0)  // Non-zero offset
                {
                    if (skipBytes < linebuf.lReadLen)   // more data need to write
                    {
                        if ((0 == g_lTransEnd) || (g_lTransEnd > linebuf.lReadLen))
                        {
                            // Write partly
                            atp_write(localFile, (linebuf.pcBuf + skipBytes), (linebuf.lReadLen - skipBytes));
                            curWrittenLen += (linebuf.lReadLen - skipBytes);
                        }
                        else
                        {
                            // Write wholely
                            atp_write(localFile, (linebuf.pcBuf + skipBytes), (g_lTransEnd - skipBytes));
                        #if ENABLE_FEATURE_ATP_WGET_ZIP
                            if (!g_bCompress) {
                                close((int)localFile);
                            } else {
                                gzclose(localFile);
                            }
                        #else
                            close(localFile);
                        #endif
                            xfunc_error_retval = ATP_TRANS_OK;
                            close_delete_and_die("ok");
                        }
                    }
                    // Start offset too big, nothing need to write
                }
                else    // Zero start offset
                {
                    if ((0 == g_lTransEnd) || (g_lTransEnd > linebuf.lReadLen)) // More data need to write
                    {
                        atp_write(localFile, linebuf.pcBuf, linebuf.lReadLen);
                        curWrittenLen += linebuf.lReadLen;
                        ulBytesRecvCount += (unsigned int)linebuf.lReadLen;
                    }
                    else // Enough data, just exit
                    {
                        atp_write(localFile, linebuf.pcBuf, g_lTransEnd);
                    #if ENABLE_FEATURE_ATP_WGET_ZIP
                        if (!g_bCompress) {
                            close((int)localFile);
                        } else {
                            gzclose(localFile);
                        }
                    #else
                        close(localFile);
                    #endif
                        xfunc_error_retval = ATP_TRANS_OK;
                        close_delete_and_die("ok");
                    }
                }
                curSkipped += linebuf.lReadLen;
            }
            else
            {
                atp_write(localFile, linebuf.pcBuf, linebuf.lReadLen);
                ulBytesRecvCount += (unsigned int)linebuf.lReadLen;
            }
            content_len -= linebuf.lReadLen;
        }
    }
#endif

    while (1) {
		while (content_len > 0 || !got_clen) {
			int n;
			unsigned rdsz = sizeof(buf);

			if (content_len < sizeof(buf) && (chunked || got_clen))
				rdsz = (unsigned)content_len;
            //printf("%d %d\n", content_len, rdsz);
            /*Start of MNT 2008-10-13 14:40 for 传输超时检测 by z65940*/
            g_TimeoutCnt = ATP_PACK_TIMEOUT_D;
            /*End of MNT 2008-10-13 14:40 for by z65940*/
        #if ENABLE_FEATURE_ATP_WGET_HTTPS
            if (https_mode <= 0) {
                n = safe_fread(buf, rdsz, sockIn);
            } else {
                n = SSL_read(ssl, buf, (int)rdsz);
            }
        #else
			n = safe_fread(buf, rdsz, sockIn);
        #endif
            /*Start of MNT 2008-10-13 14:40 for 传输超时检测 by z65940*/
            g_TimeoutCnt = -1;
            /*End of MNT 2008-10-13 14:40 for by z65940*/
			if (n <= 0) {
				if (ferror(sockIn)) {
					/* perror will not work: ferror doesn't set errno */
                                   SetDownDiagState(pstDiagState, "Error_TransferFailed", bDiagFlag);  
					bb_error_msg_and_die(bb_msg_read_error);
				}
				break;
			}
            //printf("Write content len %d %d %d:%d:%d bytes to file now\n", content_len, n, g_lTransEnd, status, skipBytes);
            if ((g_lTransEnd >= 0) && (206 != status))
            //if (1)
            {
                if (skipBytes >= (curSkipped + n))
                {
                    if (got_clen)
				    {
				        content_len -= n;
                    }
                    curSkipped += n;
                    // Need to skip
                    continue;
                }

                // Calculate bytes to write
                int datalen;
                datalen = n;
                if (skipBytes > curSkipped)
                {
                    datalen = ((n + curSkipped) - skipBytes);
                }
                //printf("Datalen: %d:%d.\n", datalen, curWrittenLen);

                if ((0 == g_lTransEnd) || ((datalen + curWrittenLen) < ((g_lTransEnd - skipBytes) + 1)))
                {
                    if (skipBytes > curSkipped)
                    {
                        atp_write(localFile, buf + (skipBytes - curSkipped), datalen);
                    }
                    else
                    {
                        atp_write(localFile, buf, datalen);
                    }
                    ulBytesRecvCount += (unsigned int)datalen;
                }
                else
                {
                    // Data enough, just break
                    if (skipBytes > curSkipped)
                    {
                        atp_write(localFile, buf + (skipBytes - curSkipped), (((g_lTransEnd - skipBytes) + 1) - curWrittenLen));
                    }
                    else
                    {
                        atp_write(localFile, buf, (((g_lTransEnd - skipBytes) + 1) - curWrittenLen));
                    }
                    break;
                }
                curWrittenLen += datalen;
                curSkipped    += datalen;
            }
			else
            {
                atp_write(localFile, buf, n);
                ulBytesRecvCount += (unsigned int)n;
            }
#if ENABLE_FEATURE_ATP_WGET_STATUSBAR
			transferred += n;
#endif
			if (got_clen)
				content_len -= n;
		}

#if 0
#if ENABLE_FEATURE_ATP_WGET_HTTPS
        if (https_mode > 0) {
            SSL_clear(ssl);
        }
#endif
#endif
		if (!chunked)
			break;

		safe_fgets(buf, sizeof(buf), sockIn); /* This is a newline */
 get_clen:
		safe_fgets(buf, sizeof(buf), sockIn);
		content_len = STRTOOFF(buf, NULL, 16);
       #if defined(SUPPORT_ATP_TR143_DOWNLOAD_TCP) ||defined(SUPPORT_ATP_TR143_DOWNLOAD)
          if(TRUE == bDiagFlag)
          {
              ulBytesRecvCount += (unsigned int)content_len;
              ulExpectBytesRecv += (unsigned int)content_len;
          }
        #endif
        // 判断文件长度
        if (g_lMaxTrans > 0) {
            g_lMaxTrans -= content_len;
            if (g_lMaxTrans < 0) {
                xfunc_error_retval = ATP_TRANS_FILE_ERR;
                close_delete_and_die("file too large");
            }
        }
		/* FIXME: error check? */
		if (content_len == 0)
			break; /* all done! */
	}
    
#if defined(SUPPORT_ATP_TR143_DOWNLOAD_TCP) ||defined(SUPPORT_ATP_TR143_DOWNLOAD)
 if(TRUE == bDiagFlag)
 {
    ulBytesRecvCount += (unsigned int)curWrittenLen;    
    pstDiagState->ulTextBytesReceived = ulBytesRecvCount;
    if(ulExpectBytesRecv != ulBytesRecvCount)
    {        
        SetDownDiagState(pstDiagState, "Error_TransferFailed", bDiagFlag);  
    }
    else
    {
        GetCurDateTime(pstDiagState->szEomTime, ATP_STRING_LEN_64);
        ATP_UTIL_GetIfcStatInfo(pstDiagState->szIfName, &stIfcStats);    
        ulIfBytesEtimeRecv = stIfcStats.ulBytesReceived; 
        pstDiagState->ulTotalBytesReceived = ulIfBytesEtimeRecv - ulIfBytesBtimeRecv;    
        SetDownDiagState(pstDiagState, "Completed", bDiagFlag);  
    }
 }
#endif

#if ENABLE_FEATURE_ATP_WGET_STATUSBAR
	progressmeter(1);
#endif

#if ENABLE_FEATURE_ATP_WGET_ZIP
    if (!g_bCompress) {
        close((int)localFile);
    } else {
        gzclose(localFile);
    }
#else
    close(localFile);
#endif
}

int wget_main(int argc, char **argv);
int wget_main(int argc, char **argv)
{
    unsigned long opt;
	struct host_info target;
    struct len_and_sockaddr *lsa;

    int trans_dir = 0;          /* 0: Error; 1: Download; 2: Upload */
    const char *pcRemotePort    = NULL;
    const char *pcRemoteIp      = NULL;
    const char *pcMaxTrans      = NULL;
#if ENABLE_FEATURE_ATP_WGET_HTTPS
    const char *pcSslVersion    = NULL;
    const char *pcSslCert       = NULL;
#endif
#ifdef ENABLE_FEATURE_IPV6
    int iResult = 0;
    char buffer[ATP_LEN_64] = {0};
    int iHttpFlag = 0;
    char acHost[128] = {0};
    struct sockaddr_in6 sin6;
#endif
#if defined(SUPPORT_ATP_TR143_DOWNLOAD_TCP) ||defined(SUPPORT_ATP_TR143_DOWNLOAD) || defined(SUPPORT_ATP_TR143_UPLOAD_TCP) || defined(SUPPORT_ATP_TR143_UPLOAD)  
    int               mpid =0;
    const char    *pcIfName = NULL;   
    FILE            *pfDiagPid = NULL;
#endif

    bool bDiagFlag = FALSE;
    tr143_diag_state_t  stDiagState;
    memset(&stDiagState,0,sizeof(tr143_diag_state_t));
    
    /*
     * Default values
     */
    xfunc_error_retval  = ATP_TRANS_SYS_ERR;
    g_pcLocalIP     = NULL;
    g_pcTransBegin  = NULL;
    g_pcTransEnd    = NULL;
    g_lTransEnd     = -1;
    g_lMaxTrans     = -1;

#if ENABLE_FEATURE_ATP_WGET_ZIP
    g_bCompress     = 0;
#endif

#if ENABLE_FEATURE_ATP_WGET_HTTPS
    https_mode      = -1;
#endif

    memset((void *)(&target), 0, sizeof(target));

	/*
	 * Crack command line.
	 */
	/*下载的两边路径都是文件名，上传的时候，
    本地是文件名，那边是路径名，例如 test/handy/ 或者 test/handy/*/
#if ENABLE_FEATURE_ATP_WGET_LONG_OPTIONS
    applet_long_options = bb_transtool_long_options;
#endif
    opt_complementary = "=1"; /* must have 1 params */

#if ENABLE_FEATURE_ATP_WGET_ZIP
    #define ZIP_OPT_CHAR    "c"
#else
    #define ZIP_OPT_CHAR
#endif
    /*START ADD by c00164495 for HG622u 对于只允许通过域名访问的服务器下载失败 2011-02-22*/
#if ENABLE_FEATURE_ATP_WGET_HTTPS
	/*增加-D参数，表示远端服务器域名。
	 *ps：增加新的参数一定要放在参数列表的最后，不然getopt32函数处理会有问题*/
   #if defined(SUPPORT_ATP_TR143_DOWNLOAD_TCP) ||defined(SUPPORT_ATP_TR143_DOWNLOAD) || defined(SUPPORT_ATP_TR143_UPLOAD_TCP) || defined(SUPPORT_ATP_TR143_UPLOAD)  
    opt = getopt32(argv, "gsvu:p:r:l:P:B:A:b:e:m:"ZIP_OPT_CHAR"a:z:D:dI:",
  #else
    opt = getopt32(argv, "gsvu:p:r:l:P:B:A:b:e:m:"ZIP_OPT_CHAR"a:z:D:",
 #endif
#else
 #if defined(SUPPORT_ATP_TR143_DOWNLOAD_TCP) ||defined(SUPPORT_ATP_TR143_DOWNLOAD) || defined(SUPPORT_ATP_TR143_UPLOAD_TCP) || defined(SUPPORT_ATP_TR143_UPLOAD)  
    opt = getopt32(argv, "gsvu:p:r:l:P:B:A:b:e:m:D:"ZIP_OPT_CHAR"dI:",
 #else
    opt = getopt32(argv, "gsvu:p:r:l:P:B:A:b:e:m:D:"ZIP_OPT_CHAR,
  #endif
#endif
    
                                &target.user,
                                &target.password,
                                &target.path,
                                &target.localFile,
                                &pcRemotePort,
                                &g_pcLocalIP,
                                &pcRemoteIp,
                                &g_pcTransBegin,
                                &g_pcTransEnd,
                            #if ENABLE_FEATURE_ATP_WGET_HTTPS
                                &pcMaxTrans,
                                &pcSslVersion,
                                &pcSslCert,
                            #else
                                &pcMaxTrans,
                            #endif
                            #if defined(SUPPORT_ATP_TR143_DOWNLOAD_TCP) ||defined(SUPPORT_ATP_TR143_DOWNLOAD) || defined(SUPPORT_ATP_TR143_UPLOAD_TCP) || defined(SUPPORT_ATP_TR143_UPLOAD)   
                                 &pcDomain,
                                 &pcIfName
                            #else   
                                 &pcDomain
                             #endif    
                                );
    /*END ADD by c00164495 for HG622u 对于只允许通过域名访问的服务器下载失败 2011-02-22*/
#undef ZIP_OPT_CHAR
 #if defined(SUPPORT_ATP_TR143_DOWNLOAD_TCP) ||defined(SUPPORT_ATP_TR143_DOWNLOAD) || defined(SUPPORT_ATP_TR143_UPLOAD_TCP) || defined(SUPPORT_ATP_TR143_UPLOAD)  
  if(0 == strcmp(argv[2],"-d") )
   {
        bDiagFlag = TRUE;                
        snprintf(stDiagState.szIfName, sizeof(stDiagState.szIfName), "%s", pcIfName);        
        mpid = (uint16_t) getpid();
   }
#endif  
#if ENABLE_FEATURE_ATP_WGET_HTTPS
    if (NULL != pcSslVersion)
    {
        /*
         *  0: No ssl; 1: Any except V2
         *  2: SSLv2; 3: SSLv3; 4: TLSv1
         *  5: Any version
         */
        https_mode = (smallint)atoi(pcSslVersion);
    }
#endif

#if ENABLE_FEATURE_ATP_WGET_HTTPS
    if (https_mode > 0) {
        target.port = bb_lookup_port(pcRemotePort, "tcp", 443);
    } else {
        target.port = bb_lookup_port(pcRemotePort, "tcp", 80);
    }
#else
    target.port = bb_lookup_port(pcRemotePort, "tcp", 80);
#endif

    if (NULL != pcMaxTrans) {
        g_lMaxTrans = atoi(pcMaxTrans);
    }

    // 有下载长度
    if (NULL != g_pcTransEnd) {
        // 只能为正数
        g_lTransEnd = atoi(g_pcTransEnd);
        if (g_lTransEnd <= 0)
                bb_show_usage();

        // 有下载起始位置，则计算下载结束位置
        if (NULL != g_pcTransBegin) {
            // 加上偏移量
            g_lTransEnd +=  atoi(g_pcTransBegin);
            g_lTransEnd -= 1;
        }
    }
    // 没有下载长度
    else {
        // 有下载开始位置，没有下载长度
        if (NULL != g_pcTransBegin) {   // Download until end
            g_lTransEnd = 0;
        }
    }

#if ENABLE_FEATURE_ATP_WGET_ZIP
    if (opt & TRANS_TOOL_OPT_ZIP) {
        g_bCompress = 1;
    }
#endif

	if (opt & TRANS_TOOL_OPT_DOWNLOAD) {
		trans_dir = 1;
        #if defined(SUPPORT_ATP_TR143_DOWNLOAD_TCP) ||defined(SUPPORT_ATP_TR143_DOWNLOAD)
             if(TRUE == bDiagFlag)
             {
                stDiagState.ulTransDir = TRANS_TOOL_OPT_DOWNLOAD;       
               if ((pfDiagPid = fopen (TR143_DOWNDIAG_PID_FILE , "w")) != NULL) 
               {
                   fprintf(pfDiagPid,"%d\n", mpid);               
                }
                (void)fclose(pfDiagPid);  
             }
         #endif 
	}
    if (opt & TRANS_TOOL_OPT_UPLOAD) {
		trans_dir = 2;
        #if defined(SUPPORT_ATP_TR143_UPLOAD_TCP) || defined(SUPPORT_ATP_TR143_UPLOAD)  
             if(TRUE == bDiagFlag)
             {
                stDiagState.ulTransDir = TRANS_TOOL_OPT_UPLOAD;       
               if ((pfDiagPid = fopen (TR143_UPDIAG_PID_FILE , "w")) != NULL) 
               {
                   fprintf(pfDiagPid,"%d\n", mpid);               
                }
                (void)fclose(pfDiagPid);  
             }
         #endif 
	}
	if ((0 == trans_dir) || (NULL == target.localFile))
		bb_show_usage();
    argv += optind;

#ifdef ENABLE_FEATURE_IPV6
    if (inet_pton(AF_INET6, argv[0], &sin6.sin6_addr) > 0)
    {
        sprintf(acHost, "[%s]", argv[0]);
        target.host = acHost;
    }
    else
#endif
    {
        target.host = argv[0];
    }

	/* We want to do exactly _one_ DNS lookup, since some
	 * sites (i.e. ftp.us.debian.org) use round-robin DNS
	 * and we want to connect to only one IP... */
	if (NULL == pcRemoteIp) {
		// User has not resolved the host, we need to use host to resolve name
		pcRemoteIp = argv[0];
	}
    #ifdef ENABLE_FEATURE_IPV6
    iResult = ATP_TransHostname2IP(pcRemoteIp,buffer,sizeof(buffer),&iHttpFlag);
    if(0 == iResult)
    {
        printf("Please check your Http URL %s! \n", pcRemoteIp);
        return ;
    }
    else
    {
        printf("The IP is [%s]\n",buffer);
        lsa = xhost2sockaddr(buffer, target.port);
    }
    #else
    lsa = xhost2sockaddr(pcRemoteIp, target.port);
    #endif

#if ENABLE_FEATURE_ATP_WGET_HTTPS
    // Need to init ssl library
    //if (!THREAD_setup() || ! SSL_library_init())
#ifndef BUILD_USE_POLARSSL
    if ((https_mode > 0) && (!SSL_library_init()))
    {
        close_delete_and_die("ssl\n");
    }
#endif
#endif

    /*Start of MNT 2008-10-13 14:39 for 传输超时检测 by z65940*/
    g_TimeoutCnt = -1;
    atp_setup_alarm();
    /*End of MNT 2008-10-13 14:39 for by z65940*/

    if (1 == trans_dir)
        http_download(&target, lsa, &stDiagState, bDiagFlag);
    else
        http_upload(&target, lsa, &stDiagState, bDiagFlag);

	exit(ATP_TRANS_OK);
}

