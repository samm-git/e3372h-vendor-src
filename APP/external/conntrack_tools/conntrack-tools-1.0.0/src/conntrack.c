/*
 * (C) 2005-2008 by Pablo Neira Ayuso <pablo@netfilter.org>
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * Note:
 *	Yes, portions of this code has been stolen from iptables ;)
 *	Special thanks to the the Netfilter Core Team.
 *	Thanks to Javier de Miguel Rodriguez <jmiguel at talika.eii.us.es>
 *	for introducing me to advanced firewalling stuff.
 *
 *						--pablo 13/04/2005
 *
 * 2005-04-16 Harald Welte <laforge@netfilter.org>: 
 * 	Add support for conntrack accounting and conntrack mark
 * 2005-06-23 Harald Welte <laforge@netfilter.org>:
 * 	Add support for expect creation
 * 2005-09-24 Harald Welte <laforge@netfilter.org>:
 * 	Remove remaints of "-A"
 * 2007-04-22 Pablo Neira Ayuso <pablo@netfilter.org>:
 * 	Ported to the new libnetfilter_conntrack API
 * 2008-04-13 Pablo Neira Ayuso <pablo@netfilter.org>:
 *	Way more flexible update and delete operations
 */

#include "conntrack.h"

#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>
#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif
#include <signal.h>
#include <string.h>
#include <netdb.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <libnetfilter_conntrack/libnetfilter_conntrack.h>

/* These are the template objects that are used to send commands. */
static struct {
	struct nf_conntrack *ct;
	struct nf_expect *exp;
	/* Expectations require the expectation tuple and the mask. */
	struct nf_conntrack *exptuple, *mask;
} tmpl;

static int alloc_tmpl_objects(void)
{
	tmpl.ct = nfct_new();
	tmpl.exptuple = nfct_new();
	tmpl.mask = nfct_new();
	tmpl.exp = nfexp_new();

	return tmpl.ct != NULL && tmpl.exptuple != NULL &&
	       tmpl.mask != NULL && tmpl.exp != NULL;
}

static void free_tmpl_objects(void)
{
	if (tmpl.ct)
		nfct_destroy(tmpl.ct);
	if (tmpl.exptuple)
		nfct_destroy(tmpl.exptuple);
	if (tmpl.mask)
		nfct_destroy(tmpl.mask);
	if (tmpl.exp)
		nfexp_destroy(tmpl.exp);
}

enum ct_command {
	CT_NONE		= 0,

	CT_LIST_BIT 	= 0,
	CT_LIST 	= (1 << CT_LIST_BIT),

	CT_CREATE_BIT	= 1,
	CT_CREATE	= (1 << CT_CREATE_BIT),

	CT_UPDATE_BIT	= 2,
	CT_UPDATE	= (1 << CT_UPDATE_BIT),

	CT_DELETE_BIT	= 3,
	CT_DELETE	= (1 << CT_DELETE_BIT),

	CT_GET_BIT	= 4,
	CT_GET		= (1 << CT_GET_BIT),

	CT_FLUSH_BIT	= 5,
	CT_FLUSH	= (1 << CT_FLUSH_BIT),

	CT_EVENT_BIT	= 6,
	CT_EVENT	= (1 << CT_EVENT_BIT),

	CT_VERSION_BIT	= 7,
	CT_VERSION	= (1 << CT_VERSION_BIT),

	CT_HELP_BIT	= 8,
	CT_HELP		= (1 << CT_HELP_BIT),

	EXP_LIST_BIT 	= 9,
	EXP_LIST 	= (1 << EXP_LIST_BIT),

	EXP_CREATE_BIT	= 10,
	EXP_CREATE	= (1 << EXP_CREATE_BIT),

	EXP_DELETE_BIT	= 11,
	EXP_DELETE	= (1 << EXP_DELETE_BIT),

	EXP_GET_BIT	= 12,
	EXP_GET		= (1 << EXP_GET_BIT),

	EXP_FLUSH_BIT	= 13,
	EXP_FLUSH	= (1 << EXP_FLUSH_BIT),

	EXP_EVENT_BIT	= 14,
	EXP_EVENT	= (1 << EXP_EVENT_BIT),

	CT_COUNT_BIT	= 15,
	CT_COUNT	= (1 << CT_COUNT_BIT),

	EXP_COUNT_BIT	= 16,
	EXP_COUNT	= (1 << EXP_COUNT_BIT),

	X_STATS_BIT	= 17,
	X_STATS		= (1 << X_STATS_BIT),
};
/* If you add a new command, you have to update NUMBER_OF_CMD in conntrack.h */

enum ct_options {
	CT_OPT_ORIG_SRC_BIT	= 0,
	CT_OPT_ORIG_SRC 	= (1 << CT_OPT_ORIG_SRC_BIT),

	CT_OPT_ORIG_DST_BIT	= 1,
	CT_OPT_ORIG_DST		= (1 << CT_OPT_ORIG_DST_BIT),

	CT_OPT_ORIG		= (CT_OPT_ORIG_SRC | CT_OPT_ORIG_DST),

	CT_OPT_REPL_SRC_BIT	= 2,
	CT_OPT_REPL_SRC		= (1 << CT_OPT_REPL_SRC_BIT),

	CT_OPT_REPL_DST_BIT	= 3,
	CT_OPT_REPL_DST		= (1 << CT_OPT_REPL_DST_BIT),

	CT_OPT_REPL		= (CT_OPT_REPL_SRC | CT_OPT_REPL_DST),

	CT_OPT_PROTO_BIT	= 4,
	CT_OPT_PROTO		= (1 << CT_OPT_PROTO_BIT),

	CT_OPT_TUPLE_ORIG	= (CT_OPT_ORIG | CT_OPT_PROTO),
	CT_OPT_TUPLE_REPL	= (CT_OPT_REPL | CT_OPT_PROTO),

	CT_OPT_TIMEOUT_BIT	= 5,
	CT_OPT_TIMEOUT		= (1 << CT_OPT_TIMEOUT_BIT),

	CT_OPT_STATUS_BIT	= 6,
	CT_OPT_STATUS		= (1 << CT_OPT_STATUS_BIT),

	CT_OPT_ZERO_BIT		= 7,
	CT_OPT_ZERO		= (1 << CT_OPT_ZERO_BIT),

	CT_OPT_EVENT_MASK_BIT	= 8,
	CT_OPT_EVENT_MASK	= (1 << CT_OPT_EVENT_MASK_BIT),

	CT_OPT_EXP_SRC_BIT	= 9,
	CT_OPT_EXP_SRC		= (1 << CT_OPT_EXP_SRC_BIT),

	CT_OPT_EXP_DST_BIT	= 10,
	CT_OPT_EXP_DST		= (1 << CT_OPT_EXP_DST_BIT),

	CT_OPT_MASK_SRC_BIT	= 11,
	CT_OPT_MASK_SRC		= (1 << CT_OPT_MASK_SRC_BIT),

	CT_OPT_MASK_DST_BIT	= 12,
	CT_OPT_MASK_DST		= (1 << CT_OPT_MASK_DST_BIT),

	CT_OPT_NATRANGE_BIT	= 13,
	CT_OPT_NATRANGE		= (1 << CT_OPT_NATRANGE_BIT),

	CT_OPT_MARK_BIT		= 14,
	CT_OPT_MARK		= (1 << CT_OPT_MARK_BIT),

	CT_OPT_ID_BIT		= 15,
	CT_OPT_ID		= (1 << CT_OPT_ID_BIT),

	CT_OPT_FAMILY_BIT	= 16,
	CT_OPT_FAMILY		= (1 << CT_OPT_FAMILY_BIT),

	CT_OPT_SRC_NAT_BIT	= 17,
	CT_OPT_SRC_NAT		= (1 << CT_OPT_SRC_NAT_BIT),

	CT_OPT_DST_NAT_BIT	= 18,
	CT_OPT_DST_NAT		= (1 << CT_OPT_DST_NAT_BIT),

	CT_OPT_OUTPUT_BIT	= 19,
	CT_OPT_OUTPUT		= (1 << CT_OPT_OUTPUT_BIT),

	CT_OPT_SECMARK_BIT	= 20,
	CT_OPT_SECMARK		= (1 << CT_OPT_SECMARK_BIT),

	CT_OPT_BUFFERSIZE_BIT	= 21,
	CT_OPT_BUFFERSIZE	= (1 << CT_OPT_BUFFERSIZE_BIT),

	CT_OPT_ANY_NAT_BIT	= 22,
	CT_OPT_ANY_NAT		= (1 << CT_OPT_ANY_NAT_BIT),

	CT_OPT_ZONE_BIT		= 23,
	CT_OPT_ZONE		= (1 << CT_OPT_ZONE_BIT),
};
/* If you add a new option, you have to update NUMBER_OF_OPT in conntrack.h */

/* Update this mask to allow to filter based on new options. */
#define CT_COMPARISON (CT_OPT_PROTO | CT_OPT_ORIG | CT_OPT_REPL | \
		       CT_OPT_MARK | CT_OPT_SECMARK |  CT_OPT_STATUS | \
		       CT_OPT_ID | CT_OPT_ZONE)

static const char *optflags[NUMBER_OF_OPT] = {
	[CT_OPT_ORIG_SRC_BIT] 	= "src",
	[CT_OPT_ORIG_DST_BIT]	= "dst",
	[CT_OPT_REPL_SRC_BIT]	= "reply-src",
	[CT_OPT_REPL_DST_BIT]	= "reply-dst",
	[CT_OPT_PROTO_BIT]	= "protonum",
	[CT_OPT_TIMEOUT_BIT]	= "timeout",
	[CT_OPT_STATUS_BIT]	= "status",
	[CT_OPT_ZERO_BIT]	= "zero",
	[CT_OPT_EVENT_MASK_BIT]	= "event-mask",
	[CT_OPT_EXP_SRC_BIT]	= "tuple-src",
	[CT_OPT_EXP_DST_BIT]	= "tuple-dst",
	[CT_OPT_MASK_SRC_BIT]	= "mask-src",
	[CT_OPT_MASK_DST_BIT]	= "mask-dst",
	[CT_OPT_NATRANGE_BIT]	= "nat-range",
	[CT_OPT_MARK_BIT]	= "mark",
	[CT_OPT_ID_BIT]		= "id",
	[CT_OPT_FAMILY_BIT]	= "family",
	[CT_OPT_SRC_NAT_BIT]	= "src-nat",
	[CT_OPT_DST_NAT_BIT]	= "dst-nat",
	[CT_OPT_OUTPUT_BIT]	= "output",
	[CT_OPT_SECMARK_BIT]	= "secmark",
	[CT_OPT_BUFFERSIZE_BIT]	= "buffer-size",
	[CT_OPT_ANY_NAT_BIT]	= "any-nat",
	[CT_OPT_ZONE_BIT]	= "zone",
};

static struct option original_opts[] = {
	{"dump", 2, 0, 'L'},
	{"create", 2, 0, 'I'},
	{"delete", 2, 0, 'D'},
	{"update", 2, 0, 'U'},
	{"get", 2, 0, 'G'},
	{"flush", 2, 0, 'F'},
	{"event", 2, 0, 'E'},
	{"counter", 2, 0, 'C'},
	{"stats", 0, 0, 'S'},
	{"version", 0, 0, 'V'},
	{"help", 0, 0, 'h'},
	{"orig-src", 1, 0, 's'},
	{"src", 1, 0, 's'},
	{"orig-dst", 1, 0, 'd'},
	{"dst", 1, 0, 'd'},
	{"reply-src", 1, 0, 'r'},
	{"reply-dst", 1, 0, 'q'},
	{"protonum", 1, 0, 'p'},
	{"timeout", 1, 0, 't'},
	{"status", 1, 0, 'u'},
	{"zero", 0, 0, 'z'},
	{"event-mask", 1, 0, 'e'},
	{"tuple-src", 1, 0, '['},
	{"tuple-dst", 1, 0, ']'},
	{"mask-src", 1, 0, '{'},
	{"mask-dst", 1, 0, '}'},
	{"nat-range", 1, 0, 'a'},	/* deprecated */
	{"mark", 1, 0, 'm'},
	{"secmark", 1, 0, 'c'},
	{"id", 2, 0, 'i'},		/* deprecated */
	{"family", 1, 0, 'f'},
	{"src-nat", 2, 0, 'n'},
	{"dst-nat", 2, 0, 'g'},
	{"output", 1, 0, 'o'},
	{"buffer-size", 1, 0, 'b'},
	{"any-nat", 2, 0, 'j'},
	{"zone", 1, 0, 'w'},
	{0, 0, 0, 0}
};

static const char *getopt_str = "L::I::U::D::G::E::F::hVs:d:r:q:"
				"p:t:u:e:a:z[:]:{:}:m:i:f:o:n::"
				"g::c:b:C::Sj::w:";

/* Table of legal combinations of commands and options.  If any of the
 * given commands make an option legal, that option is legal (applies to
 * CMD_LIST and CMD_ZERO only).
 * Key:
 *  0  illegal
 *  1  compulsory
 *  2  optional
 *  3  undecided, see flag combination checkings in generic_opt_check()
 */

static char commands_v_options[NUMBER_OF_CMD][NUMBER_OF_OPT] =
/* Well, it's better than "Re: Linux vs FreeBSD" */
{
          /*   s d r q p t u z e [ ] { } a m i f n g o c b j w*/
/*CT_LIST*/   {2,2,2,2,2,0,2,2,0,0,0,0,0,0,2,0,2,2,2,2,2,0,2,2},
/*CT_CREATE*/ {3,3,3,3,1,1,2,0,0,0,0,0,0,2,2,0,0,2,2,0,0,0,0,2},
/*CT_UPDATE*/ {2,2,2,2,2,2,2,0,0,0,0,0,0,0,2,2,2,2,2,2,0,0,0,0},
/*CT_DELETE*/ {2,2,2,2,2,2,2,0,0,0,0,0,0,0,2,2,2,2,2,2,0,0,0,2},
/*CT_GET*/    {3,3,3,3,1,0,0,0,0,0,0,0,0,0,0,2,0,0,0,2,0,0,0,0},
/*CT_FLUSH*/  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
/*CT_EVENT*/  {2,2,2,2,2,0,0,0,2,0,0,0,0,0,2,0,0,2,2,2,2,2,2,2},
/*VERSION*/   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
/*HELP*/      {0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
/*EXP_LIST*/  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0},
/*EXP_CREATE*/{1,1,2,2,1,1,2,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
/*EXP_DELETE*/{1,1,2,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
/*EXP_GET*/   {1,1,2,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
/*EXP_FLUSH*/ {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
/*EXP_EVENT*/ {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
/*CT_COUNT*/  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
/*EXP_COUNT*/ {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
/*X_STATS*/   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

static const int cmd2type[][2] = {
	['L']	= { CT_LIST, 	EXP_LIST },
	['I']	= { CT_CREATE,	EXP_CREATE },
	['D']	= { CT_DELETE,	EXP_DELETE },
	['G']	= { CT_GET,	EXP_GET },
	['F']	= { CT_FLUSH,	EXP_FLUSH },
	['E']	= { CT_EVENT,	EXP_EVENT },
	['V']	= { CT_VERSION,	CT_VERSION },
	['h']	= { CT_HELP,	CT_HELP },
	['C']	= { CT_COUNT,	EXP_COUNT },
};

static const int opt2type[] = {
	['s']	= CT_OPT_ORIG_SRC,
	['d']	= CT_OPT_ORIG_DST,
	['r']	= CT_OPT_REPL_SRC,
	['q']	= CT_OPT_REPL_DST,
	['{']	= CT_OPT_MASK_SRC,
	['}']	= CT_OPT_MASK_DST,
	['[']	= CT_OPT_EXP_SRC,
	[']']	= CT_OPT_EXP_DST,
	['n']	= CT_OPT_SRC_NAT,
	['g']	= CT_OPT_DST_NAT,
	['m']	= CT_OPT_MARK,
	['c']	= CT_OPT_SECMARK,
	['i']	= CT_OPT_ID,
	['j']	= CT_OPT_ANY_NAT,
	['w']	= CT_OPT_ZONE,
};

static const int opt2family_attr[][2] = {
	['s']	= { ATTR_ORIG_IPV4_SRC,	ATTR_ORIG_IPV6_SRC },
	['d']	= { ATTR_ORIG_IPV4_DST,	ATTR_ORIG_IPV6_DST },
	['r']	= { ATTR_REPL_IPV4_SRC, ATTR_REPL_IPV6_SRC },
	['q']	= { ATTR_REPL_IPV4_DST, ATTR_REPL_IPV6_DST },
	['{']	= { ATTR_ORIG_IPV4_SRC,	ATTR_ORIG_IPV6_SRC },
	['}']	= { ATTR_ORIG_IPV4_DST,	ATTR_ORIG_IPV6_DST },
	['[']	= { ATTR_ORIG_IPV4_SRC, ATTR_ORIG_IPV6_SRC },
	[']']	= { ATTR_ORIG_IPV4_DST, ATTR_ORIG_IPV6_DST },
};

static const int opt2attr[] = {
	['s']	= ATTR_ORIG_L3PROTO,
	['d']	= ATTR_ORIG_L3PROTO,
	['r']	= ATTR_REPL_L3PROTO,
	['q']	= ATTR_REPL_L3PROTO,
	['m']	= ATTR_MARK,
	['c']	= ATTR_SECMARK,
	['i']	= ATTR_ID,
	['w']	= ATTR_ZONE,
};

static char exit_msg[NUMBER_OF_CMD][64] = {
	[CT_LIST_BIT] 		= "%d flow entries have been shown.\n",
	[CT_CREATE_BIT]		= "%d flow entries have been created.\n",
	[CT_UPDATE_BIT]		= "%d flow entries have been updated.\n",
	[CT_DELETE_BIT]		= "%d flow entries have been deleted.\n",
	[CT_GET_BIT] 		= "%d flow entries have been shown.\n",
	[CT_EVENT_BIT]		= "%d flow events have been shown.\n",
	[EXP_LIST_BIT]		= "%d expectations have been shown.\n",
	[EXP_DELETE_BIT]	= "%d expectations have been shown.\n",
};

static const char usage_commands[] =
	"Commands:\n"
	"  -L [table] [options]\t\tList conntrack or expectation table\n"
	"  -G [table] parameters\t\tGet conntrack or expectation\n"
	"  -D [table] parameters\t\tDelete conntrack or expectation\n"
	"  -I [table] parameters\t\tCreate a conntrack or expectation\n"
	"  -U [table] parameters\t\tUpdate a conntrack\n"
	"  -E [table] [options]\t\tShow events\n"
	"  -F [table]\t\t\tFlush table\n"
	"  -C [table]\t\t\tShow counter\n"
	"  -S\t\t\t\tShow statistics\n";

static const char usage_tables[] =
	"Tables: conntrack, expect\n";

static const char usage_conntrack_parameters[] =
	"Conntrack parameters and options:\n"
	"  -n, --src-nat ip\t\t\tsource NAT ip\n"
	"  -g, --dst-nat ip\t\t\tdestination NAT ip\n"
	"  -j, --any-nat ip\t\t\tsource or destination NAT ip\n"
	"  -m, --mark mark\t\t\tSet mark\n"
	"  -c, --secmark secmark\t\t\tSet selinux secmark\n"
	"  -e, --event-mask eventmask\t\tEvent mask, eg. NEW,DESTROY\n"
	"  -z, --zero \t\t\t\tZero counters while listing\n"
	"  -o, --output type[,...]\t\tOutput format, eg. xml\n";

static const char usage_expectation_parameters[] =
	"Expectation parameters and options:\n"
	"  --tuple-src ip\tSource address in expect tuple\n"
	"  --tuple-dst ip\tDestination address in expect tuple\n"
	"  --mask-src ip\t\tSource mask address\n"
	"  --mask-dst ip\t\tDestination mask address\n";

static const char usage_parameters[] =
	"Common parameters and options:\n"
	"  -s, --orig-src ip\t\tSource address from original direction\n"
	"  -d, --orig-dst ip\t\tDestination address from original direction\n"
	"  -r, --reply-src ip\t\tSource addres from reply direction\n"
	"  -q, --reply-dst ip\t\tDestination address from reply direction\n"
	"  -p, --protonum proto\t\tLayer 4 Protocol, eg. 'tcp'\n"
	"  -f, --family proto\t\tLayer 3 Protocol, eg. 'ipv6'\n"
	"  -t, --timeout timeout\t\tSet timeout\n"
	"  -u, --status status\t\tSet status, eg. ASSURED\n"
	"  -w, --zone value\t\tSet conntrack zone\n"
	"  -b, --buffer-size\t\tNetlink socket buffer size\n"
	;

#define OPTION_OFFSET 256

static struct nfct_handle *cth, *ith;
static struct option *opts = original_opts;
static unsigned int global_option_offset = 0;

#define ADDR_VALID_FLAGS_MAX   2
static unsigned int addr_valid_flags[ADDR_VALID_FLAGS_MAX] = {
	CT_OPT_ORIG_SRC | CT_OPT_ORIG_DST,
	CT_OPT_REPL_SRC | CT_OPT_REPL_DST,
};

static LIST_HEAD(proto_list);

static unsigned int options;

void register_proto(struct ctproto_handler *h)
{
	if (strcmp(h->version, VERSION) != 0) {
		fprintf(stderr, "plugin `%s': version %s (I'm %s)\n",
			h->name, h->version, VERSION);
		exit(1);
	}
	list_add(&h->head, &proto_list);
}

extern struct ctproto_handler ct_proto_unknown;

static struct ctproto_handler *findproto(char *name, int *pnum)
{
	struct ctproto_handler *cur;
	struct protoent *pent;
	int protonum;

	/* is it in the list of supported protocol? */
	list_for_each_entry(cur, &proto_list, head) {
		if (strcmp(cur->name, name) == 0) {
			*pnum = cur->protonum;
			return cur;
		}
	}
	/* using the protocol name for an unsupported protocol? */
	if ((pent = getprotobyname(name))) {
		*pnum = pent->p_proto;
		return &ct_proto_unknown;
	}
	/* using a protocol number? */
	protonum = atoi(name);
	if (protonum > 0 && protonum <= IPPROTO_MAX) {
		/* try lookup by number, perhaps this protocol is supported */
		list_for_each_entry(cur, &proto_list, head) {
			if (cur->protonum == protonum) {
				*pnum = protonum;
				return cur;
			}
		}
		*pnum = protonum;
		return &ct_proto_unknown;
	}

	return NULL;
}

static void
extension_help(struct ctproto_handler *h, int protonum)
{
	const char *name;

	if (h == &ct_proto_unknown) {
		struct protoent *pent;

		pent = getprotobynumber(protonum);
		if (!pent)
			name = h->name;
		else
			name = pent->p_name;
	} else {
		name = h->name;
	}

	fprintf(stdout, "Proto `%s' help:\n", name);
	h->help();
}

static void __attribute__((noreturn))
exit_tryhelp(int status)
{
	fprintf(stderr, "Try `%s -h' or '%s --help' for more information.\n",
			PROGNAME, PROGNAME);
	exit(status);
}

static void free_options(void)
{
	if (opts != original_opts) {
		free(opts);
		opts = original_opts;
		global_option_offset = 0;
	}
}

void __attribute__((noreturn))
exit_error(enum exittype status, const char *msg, ...)
{
	va_list args;

	free_options();
	va_start(args, msg);
	fprintf(stderr,"%s v%s (conntrack-tools): ", PROGNAME, VERSION);
	vfprintf(stderr, msg, args);
	fprintf(stderr, "\n");
	va_end(args);
	if (status == PARAMETER_PROBLEM)
		exit_tryhelp(status);
	/* release template objects that were allocated in the setup stage. */
	free_tmpl_objects();
	exit(status);
}

static int bit2cmd(int command)
{
	int i;

	for (i = 0; i < NUMBER_OF_CMD; i++)
		if (command & (1<<i))
			break;

	return i;
}

int generic_opt_check(int local_options, int num_opts,
		      char *optset, const char *optflg[],
		      unsigned int *coupled_flags, int coupled_flags_size,
		      int *partial)
{
	int i, matching = -1, special_case = 0;

	for (i = 0; i < num_opts; i++) {
		if (!(local_options & (1<<i))) {
			if (optset[i] == 1)
				exit_error(PARAMETER_PROBLEM, 
					   "You need to supply the "
					   "`--%s' option for this "
					   "command", optflg[i]);
		} else {
			if (optset[i] == 0)
				exit_error(PARAMETER_PROBLEM, "Illegal "
					   "option `--%s' with this "
					   "command", optflg[i]);
		}
		if (optset[i] == 3)
			special_case = 1;
	}

	/* no weird flags combinations, leave */
	if (!special_case || coupled_flags == NULL)
		return 1;

	*partial = -1;
	for (i=0; i<coupled_flags_size; i++) {
		/* we look for an exact matching to ensure this is correct */
		if ((local_options & coupled_flags[i]) == coupled_flags[i]) {
			matching = i;
			break;
		}
		/* ... otherwise look for the first partial matching */
		if ((local_options & coupled_flags[i]) && *partial < 0) {
			*partial = i;
		}
	}

	/* we found an exact matching, game over */
	if (matching >= 0)
		return 1;

	/* report a partial matching to suggest something */
	return 0;
}

static struct option *
merge_options(struct option *oldopts, const struct option *newopts,
	      unsigned int *option_offset)
{
	unsigned int num_old, num_new, i;
	struct option *merge;

	for (num_old = 0; oldopts[num_old].name; num_old++);
	for (num_new = 0; newopts[num_new].name; num_new++);

	global_option_offset += OPTION_OFFSET;
	*option_offset = global_option_offset;

	merge = malloc(sizeof(struct option) * (num_new + num_old + 1));
	if (merge == NULL)
		return NULL;

	memcpy(merge, oldopts, num_old * sizeof(struct option));
	for (i = 0; i < num_new; i++) {
		merge[num_old + i] = newopts[i];
		merge[num_old + i].val += *option_offset;
	}
	memset(merge + num_old + num_new, 0, sizeof(struct option));

	return merge;
}

/* From linux/errno.h */
#define ENOTSUPP        524     /* Operation is not supported */

/* Translates errno numbers into more human-readable form than strerror. */
static const char *
err2str(int err, enum ct_command command)
{
	unsigned int i;
	struct table_struct {
		enum ct_command act;
		int err;
		const char *message;
	} table [] =
	  { { CT_LIST, ENOTSUPP, "function not implemented" },
	    { 0xFFFF, EINVAL, "invalid parameters" },
	    { CT_CREATE, EEXIST, "Such conntrack exists, try -U to update" },
	    { CT_CREATE|CT_GET|CT_DELETE, ENOENT, 
		    "such conntrack doesn't exist" },
	    { CT_CREATE|CT_GET, ENOMEM, "not enough memory" },
	    { CT_GET, EAFNOSUPPORT, "protocol not supported" },
	    { CT_CREATE, ETIME, "conntrack has expired" },
	    { EXP_CREATE, ENOENT, "master conntrack not found" },
	    { EXP_CREATE, EINVAL, "invalid parameters" },
	    { ~0U, EPERM, "sorry, you must be root or get "
		    	   "CAP_NET_ADMIN capability to do this"}
	  };

	for (i = 0; i < sizeof(table)/sizeof(struct table_struct); i++) {
		if ((table[i].act & command) && table[i].err == err)
			return table[i].message;
	}

	return strerror(err);
}

#define PARSE_STATUS 0
#define PARSE_EVENT 1
#define PARSE_OUTPUT 2
#define PARSE_MAX 3

enum {
	_O_XML	= (1 << 0),
	_O_EXT	= (1 << 1),
	_O_TMS	= (1 << 2),
	_O_ID	= (1 << 3),
	_O_KTMS	= (1 << 4),
};

enum {
	CT_EVENT_F_NEW	= (1 << 0),
	CT_EVENT_F_UPD	= (1 << 1),
	CT_EVENT_F_DEL 	= (1 << 2),
	CT_EVENT_F_ALL	= CT_EVENT_F_NEW | CT_EVENT_F_UPD | CT_EVENT_F_DEL,
};

static struct parse_parameter {
	const char	*parameter[6];
	size_t  size;
	unsigned int value[6];
} parse_array[PARSE_MAX] = {
	{ {"ASSURED", "SEEN_REPLY", "UNSET", "FIXED_TIMEOUT", "EXPECTED"}, 5,
	  { IPS_ASSURED, IPS_SEEN_REPLY, 0, IPS_FIXED_TIMEOUT, IPS_EXPECTED} },
	{ {"ALL", "NEW", "UPDATES", "DESTROY"}, 4,
	  { CT_EVENT_F_ALL, CT_EVENT_F_NEW, CT_EVENT_F_UPD, CT_EVENT_F_DEL } },
	{ {"xml", "extended", "timestamp", "id", "ktimestamp"}, 5, 
	  { _O_XML, _O_EXT, _O_TMS, _O_ID, _O_KTMS },
	},
};

static int
do_parse_parameter(const char *str, size_t str_length, unsigned int *value, 
		   int parse_type)
{
	size_t i;
	int ret = 0;
	struct parse_parameter *p = &parse_array[parse_type];

	if (strncasecmp(str, "SRC_NAT", str_length) == 0) {
		fprintf(stderr, "WARNING: ignoring SRC_NAT, "
				"use --src-nat instead\n");
		return 1;
	}

	if (strncasecmp(str, "DST_NAT", str_length) == 0) {
		fprintf(stderr, "WARNING: ignoring DST_NAT, "
				"use --dst-nat instead\n");
		return 1;
	}

	for (i = 0; i < p->size; i++)
		if (strncasecmp(str, p->parameter[i], str_length) == 0) {
			*value |= p->value[i];
			ret = 1;
			break;
		}
	
	return ret;
}

static void
parse_parameter(const char *arg, unsigned int *status, int parse_type)
{
	const char *comma;

	while ((comma = strchr(arg, ',')) != NULL) {
		if (comma == arg 
		    || !do_parse_parameter(arg, comma-arg, status, parse_type))
			exit_error(PARAMETER_PROBLEM,"Bad parameter `%s'", arg);
		arg = comma+1;
	}

	if (strlen(arg) == 0
	    || !do_parse_parameter(arg, strlen(arg), status, parse_type))
		exit_error(PARAMETER_PROBLEM, "Bad parameter `%s'", arg);
}

static void
add_command(unsigned int *cmd, const int newcmd)
{
	if (*cmd)
		exit_error(PARAMETER_PROBLEM, "Invalid commands combination");
	*cmd |= newcmd;
}

static unsigned int
check_type(int argc, char *argv[])
{
	char *table = NULL;

	/* Nasty bug or feature in getopt_long ? 
	 * It seems that it behaves badly with optional arguments.
	 * Fortunately, I just stole the fix from iptables ;) */
	if (optarg)
		return 0;
	else if (optind < argc && argv[optind][0] != '-' 
			&& argv[optind][0] != '!')
		table = argv[optind++];
	
	if (!table)
		return 0;
		
	if (strncmp("expect", table, strlen(table)) == 0)
		return 1;
	else if (strncmp("conntrack", table, strlen(table)) == 0)
		return 0;
	else
		exit_error(PARAMETER_PROBLEM, "unknown type `%s'", table);

	return 0;
}

static void set_family(int *family, int new)
{
	if (*family == AF_UNSPEC)
		*family = new;
	else if (*family != new)
		exit_error(PARAMETER_PROBLEM, "mismatched address family");
}

struct addr_parse {
	struct in_addr addr;
	struct in6_addr addr6;
	unsigned int family;
};

static int
parse_inetaddr(const char *cp, struct addr_parse *parse)
{
	if (inet_aton(cp, &parse->addr))
		return AF_INET;
#ifdef HAVE_INET_PTON_IPV6
	else if (inet_pton(AF_INET6, cp, &parse->addr6) > 0)
		return AF_INET6;
#endif
	return AF_UNSPEC;
}

union ct_address {
	uint32_t v4;
	uint32_t v6[4];
};

static int
parse_addr(const char *cp, union ct_address *address)
{
	struct addr_parse parse;
	int ret;

	if ((ret = parse_inetaddr(cp, &parse)) == AF_INET)
		address->v4 = parse.addr.s_addr;
	else if (ret == AF_INET6)
		memcpy(address->v6, &parse.addr6, sizeof(parse.addr6));

	return ret;
}

static void
nat_parse(char *arg, struct nf_conntrack *obj, int type)
{
	char *colon, *error;
	union ct_address parse;

	colon = strchr(arg, ':');

	if (colon) {
		uint16_t port;

		*colon = '\0';

		port = (uint16_t)atoi(colon+1);
		if (port == 0) {
			if (strlen(colon+1) == 0) {
				exit_error(PARAMETER_PROBLEM,
					   "No port specified after `:'");
			} else {
				exit_error(PARAMETER_PROBLEM,
					   "Port `%s' not valid", colon+1);
			}
		}

		error = strchr(colon+1, ':');
		if (error)
			exit_error(PARAMETER_PROBLEM,
				   "Invalid port:port syntax");

		if (type == CT_OPT_SRC_NAT)
			nfct_set_attr_u16(tmpl.ct, ATTR_SNAT_PORT, ntohs(port));
		else if (type == CT_OPT_DST_NAT)
			nfct_set_attr_u16(tmpl.ct, ATTR_DNAT_PORT, ntohs(port));
		else if (type == CT_OPT_ANY_NAT) {
			nfct_set_attr_u16(tmpl.ct, ATTR_SNAT_PORT, ntohs(port));
			nfct_set_attr_u16(tmpl.ct, ATTR_DNAT_PORT, ntohs(port));
		}
	}

	if (parse_addr(arg, &parse) == AF_UNSPEC) {
		if (strlen(arg) == 0) {
			exit_error(PARAMETER_PROBLEM, "No IP specified");
		} else {
			exit_error(PARAMETER_PROBLEM,
					"Invalid IP address `%s'", arg);
		}
	}

	if (type == CT_OPT_SRC_NAT || type == CT_OPT_ANY_NAT)
		nfct_set_attr_u32(tmpl.ct, ATTR_SNAT_IPV4, parse.v4);
	else if (type == CT_OPT_DST_NAT || type == CT_OPT_ANY_NAT)
		nfct_set_attr_u32(tmpl.ct, ATTR_DNAT_IPV4, parse.v4);
}

static void
usage(char *prog)
{
	fprintf(stdout, "Command line interface for the connection "
			"tracking system. Version %s\n", VERSION);
	fprintf(stdout, "Usage: %s [commands] [options]\n", prog);

	fprintf(stdout, "\n%s", usage_commands);
	fprintf(stdout, "\n%s", usage_tables);
	fprintf(stdout, "\n%s", usage_conntrack_parameters);
	fprintf(stdout, "\n%s", usage_expectation_parameters);
	fprintf(stdout, "\n%s\n", usage_parameters);
}

static unsigned int output_mask;

static int 
filter_nat(const struct nf_conntrack *obj, const struct nf_conntrack *ct)
{
	int check_srcnat = options & CT_OPT_SRC_NAT ? 1 : 0;
	int check_dstnat = options & CT_OPT_DST_NAT ? 1 : 0;
	int has_srcnat = 0, has_dstnat = 0;
	uint32_t ip;
	uint16_t port;

	if (options & CT_OPT_ANY_NAT)
		check_srcnat = check_dstnat = 1;

	if (check_srcnat) {
		int check_address = 0, check_port = 0;

		if (nfct_attr_is_set(obj, ATTR_SNAT_IPV4)) {
			check_address = 1;
			ip = nfct_get_attr_u32(obj, ATTR_SNAT_IPV4);
			if (nfct_getobjopt(ct, NFCT_GOPT_IS_SNAT) &&
			    ip == nfct_get_attr_u32(ct, ATTR_REPL_IPV4_DST))
				has_srcnat = 1;
		}
		if (nfct_attr_is_set(obj, ATTR_SNAT_PORT)) {
			int ret = 0;

			check_port = 1;
			port = nfct_get_attr_u16(obj, ATTR_SNAT_PORT);
			if (nfct_getobjopt(ct, NFCT_GOPT_IS_SPAT) &&
			    port == nfct_get_attr_u16(ct, ATTR_REPL_PORT_DST))
				ret = 1;

			/* the address matches but the port does not. */
			if (check_address && has_srcnat && !ret)
				has_srcnat = 0;
			if (!check_address && ret)
				has_srcnat = 1;
		}
		if (!check_address && !check_port &&
		    (nfct_getobjopt(ct, NFCT_GOPT_IS_SNAT) ||
		     nfct_getobjopt(ct, NFCT_GOPT_IS_SPAT)))
		  	has_srcnat = 1;
	}
	if (check_dstnat) {
		int check_address = 0, check_port = 0;

		if (nfct_attr_is_set(obj, ATTR_DNAT_IPV4)) {
			check_address = 1;
			ip = nfct_get_attr_u32(obj, ATTR_DNAT_IPV4);
			if (nfct_getobjopt(ct, NFCT_GOPT_IS_DNAT) &&
			    ip == nfct_get_attr_u32(ct, ATTR_REPL_IPV4_SRC))
				has_dstnat = 1;
		}
		if (nfct_attr_is_set(obj, ATTR_DNAT_PORT)) {
			int ret = 0;

			check_port = 1;
			port = nfct_get_attr_u16(obj, ATTR_DNAT_PORT);
			if (nfct_getobjopt(ct, NFCT_GOPT_IS_DPAT) &&
			    port == nfct_get_attr_u16(ct, ATTR_REPL_PORT_SRC))
				ret = 1;

			/* the address matches but the port does not. */
			if (check_address && has_dstnat && !ret)
				has_dstnat = 0;
			if (!check_address && ret)
				has_dstnat = 1;
		}
		if (!check_address && !check_port &&
		    (nfct_getobjopt(ct, NFCT_GOPT_IS_DNAT) ||
		     nfct_getobjopt(ct, NFCT_GOPT_IS_DPAT)))
			has_dstnat = 1;
	}
	if (options & CT_OPT_ANY_NAT)
		return !(has_srcnat || has_dstnat);
	else if ((options & CT_OPT_SRC_NAT) && (options & CT_OPT_DST_NAT))
		return !(has_srcnat && has_dstnat);
	else if (options & CT_OPT_SRC_NAT)
		return !has_srcnat;
	else if (options & CT_OPT_DST_NAT)
		return !has_dstnat;

	return 0;
}

static int counter;
static int dump_xml_header_done = 1;

static void __attribute__((noreturn))
event_sighandler(int s)
{
	if (dump_xml_header_done == 0) {
		printf("</conntrack>\n");
		fflush(stdout);
	}

	fprintf(stderr, "%s v%s (conntrack-tools): ", PROGNAME, VERSION);
	fprintf(stderr, "%d flow events have been shown.\n", counter);
	nfct_close(cth);
	exit(0);
}

static int event_cb(enum nf_conntrack_msg_type type,
		    struct nf_conntrack *ct,
		    void *data)
{
	char buf[1024];
	struct nf_conntrack *obj = data;
	unsigned int op_type = NFCT_O_DEFAULT;
	unsigned int op_flags = 0;

	if (filter_nat(obj, ct))
		return NFCT_CB_CONTINUE;

	if (options & CT_COMPARISON &&
	    !nfct_cmp(obj, ct, NFCT_CMP_ALL | NFCT_CMP_MASK))
		return NFCT_CB_CONTINUE;

	if (output_mask & _O_XML) {
		op_type = NFCT_O_XML;
		if (dump_xml_header_done) {
			dump_xml_header_done = 0;
			printf("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
			       "<conntrack>\n");
		}
	} 
	if (output_mask & _O_EXT)
		op_flags = NFCT_OF_SHOW_LAYER3;
	if (output_mask & _O_TMS) {
		if (!(output_mask & _O_XML)) {
			struct timeval tv;
			gettimeofday(&tv, NULL);
			printf("[%-8ld.%-6ld]\t", tv.tv_sec, tv.tv_usec);
		} else
			op_flags |= NFCT_OF_TIME;
	}
	if (output_mask & _O_KTMS)
		op_flags |= NFCT_OF_TIMESTAMP;
	if (output_mask & _O_ID)
		op_flags |= NFCT_OF_ID;

	nfct_snprintf(buf, sizeof(buf), ct, type, op_type, op_flags);

	printf("%s\n", buf);
	fflush(stdout);

	counter++;

	return NFCT_CB_CONTINUE;
}

static int dump_cb(enum nf_conntrack_msg_type type,
		   struct nf_conntrack *ct,
		   void *data)
{
	char buf[1024];
	struct nf_conntrack *obj = data;
	unsigned int op_type = NFCT_O_DEFAULT;
	unsigned int op_flags = 0;

	if (filter_nat(obj, ct))
		return NFCT_CB_CONTINUE;

	if (options & CT_COMPARISON &&
	    !nfct_cmp(obj, ct, NFCT_CMP_ALL | NFCT_CMP_MASK))
		return NFCT_CB_CONTINUE;

	if (output_mask & _O_XML) {
		op_type = NFCT_O_XML;
		if (dump_xml_header_done) {
			dump_xml_header_done = 0;
			printf("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
			       "<conntrack>\n");
		}
	}
	if (output_mask & _O_EXT)
		op_flags = NFCT_OF_SHOW_LAYER3;
	if (output_mask & _O_KTMS)
		op_flags |= NFCT_OF_TIMESTAMP;
	if (output_mask & _O_ID)
		op_flags |= NFCT_OF_ID;

	nfct_snprintf(buf, sizeof(buf), ct, NFCT_T_UNKNOWN, op_type, op_flags);
	printf("%s\n", buf);

	counter++;

	return NFCT_CB_CONTINUE;
}

static int delete_cb(enum nf_conntrack_msg_type type,
		     struct nf_conntrack *ct,
		     void *data)
{
	int res;
	char buf[1024];
	struct nf_conntrack *obj = data;
	unsigned int op_type = NFCT_O_DEFAULT;
	unsigned int op_flags = 0;

	if (filter_nat(obj, ct))
		return NFCT_CB_CONTINUE;

	if (options & CT_COMPARISON &&
	    !nfct_cmp(obj, ct, NFCT_CMP_ALL | NFCT_CMP_MASK))
		return NFCT_CB_CONTINUE;

	res = nfct_query(ith, NFCT_Q_DESTROY, ct);
	if (res < 0)
		exit_error(OTHER_PROBLEM,
			   "Operation failed: %s",
			   err2str(errno, CT_DELETE));

	if (output_mask & _O_XML)
		op_type = NFCT_O_XML;
	if (output_mask & _O_EXT)
		op_flags = NFCT_OF_SHOW_LAYER3;
	if (output_mask & _O_ID)
		op_flags |= NFCT_OF_ID;

	nfct_snprintf(buf, sizeof(buf), ct, NFCT_T_UNKNOWN, op_type, op_flags);
	printf("%s\n", buf);

	counter++;

	return NFCT_CB_CONTINUE;
}

static int print_cb(enum nf_conntrack_msg_type type,
		    struct nf_conntrack *ct,
		    void *data)
{
	char buf[1024];
	unsigned int op_type = NFCT_O_DEFAULT;
	unsigned int op_flags = 0;

	if (output_mask & _O_XML)
		op_type = NFCT_O_XML;
	if (output_mask & _O_EXT)
		op_flags = NFCT_OF_SHOW_LAYER3;
	if (output_mask & _O_ID)
		op_flags |= NFCT_OF_ID;

	nfct_snprintf(buf, sizeof(buf), ct, NFCT_T_UNKNOWN, op_type, op_flags);
	printf("%s\n", buf);

	return NFCT_CB_CONTINUE;
}

static int update_cb(enum nf_conntrack_msg_type type,
		     struct nf_conntrack *ct,
		     void *data)
{
	int res;
	struct nf_conntrack *obj = data, *tmp;

	if (filter_nat(obj, ct))
		return NFCT_CB_CONTINUE;

	if (nfct_attr_is_set(obj, ATTR_ID) && nfct_attr_is_set(ct, ATTR_ID) &&
	    nfct_get_attr_u32(obj, ATTR_ID) != nfct_get_attr_u32(ct, ATTR_ID))
	    	return NFCT_CB_CONTINUE;

	if (options & CT_OPT_TUPLE_ORIG && !nfct_cmp(obj, ct, NFCT_CMP_ORIG))
		return NFCT_CB_CONTINUE;
	if (options & CT_OPT_TUPLE_REPL && !nfct_cmp(obj, ct, NFCT_CMP_REPL))
		return NFCT_CB_CONTINUE;

	tmp = nfct_new();
	if (tmp == NULL)
		exit_error(OTHER_PROBLEM, "out of memory");

	nfct_copy(tmp, ct, NFCT_CP_ORIG);
	nfct_copy(tmp, obj, NFCT_CP_META);

	res = nfct_query(ith, NFCT_Q_UPDATE, tmp);
	if (res < 0) {
		nfct_destroy(tmp);
		exit_error(OTHER_PROBLEM,
			   "Operation failed: %s",
			   err2str(errno, CT_UPDATE));
	}
	nfct_callback_register(ith, NFCT_T_ALL, print_cb, NULL);

	res = nfct_query(ith, NFCT_Q_GET, tmp);
	if (res < 0) {
		nfct_destroy(tmp);
		/* the entry has vanish in middle of the update */
		if (errno == ENOENT) {
			nfct_callback_unregister(ith);
			return NFCT_CB_CONTINUE;
		}
		exit_error(OTHER_PROBLEM,
			   "Operation failed: %s",
			   err2str(errno, CT_UPDATE));
	}
	nfct_destroy(tmp);
	nfct_callback_unregister(ith);

	counter++;

	return NFCT_CB_CONTINUE;
}

static int dump_exp_cb(enum nf_conntrack_msg_type type,
		      struct nf_expect *exp,
		      void *data)
{
	char buf[1024];

	nfexp_snprintf(buf,sizeof(buf), exp, NFCT_T_UNKNOWN, NFCT_O_DEFAULT, 0);
	printf("%s\n", buf);
	counter++;

	return NFCT_CB_CONTINUE;
}

static int event_exp_cb(enum nf_conntrack_msg_type type,
			struct nf_expect *exp, void *data)
{
	char buf[1024];

	nfexp_snprintf(buf,sizeof(buf), exp, type, NFCT_O_DEFAULT, 0);
	printf("%s\n", buf);
	counter++;

	return NFCT_CB_CONTINUE;
}

static int count_exp_cb(enum nf_conntrack_msg_type type,
			struct nf_expect *exp,
			void *data)
{
	counter++;
	return NFCT_CB_CONTINUE;
}

#ifndef CT_STATS_PROC
#define CT_STATS_PROC "/proc/net/stat/nf_conntrack"
#endif

/* As of 2.6.29, we have 16 entries, this is enough */
#ifndef CT_STATS_ENTRIES_MAX
#define CT_STATS_ENTRIES_MAX 64
#endif

/* maximum string length currently is 13 characters */
#ifndef CT_STATS_STRING_MAX
#define CT_STATS_STRING_MAX 64
#endif

static int display_proc_conntrack_stats(void)
{
	int ret = 0;
	FILE *fd;
	char buf[4096], *token, *nl;
	char output[CT_STATS_ENTRIES_MAX][CT_STATS_STRING_MAX];
	unsigned int value[CT_STATS_ENTRIES_MAX], i, max;

	fd = fopen(CT_STATS_PROC, "r");
	if (fd == NULL)
		return -1;

	if (fgets(buf, sizeof(buf), fd) == NULL) {
		ret = -1;
		goto out_err;
	}

	/* trim off trailing \n */
	nl = strchr(buf, '\n');
	if (nl != NULL) {
		*nl = '\0';
		nl = strchr(buf, '\n');
	}
	token = strtok(buf, " ");
	for (i=0; token != NULL && i<CT_STATS_ENTRIES_MAX; i++) {
		strncpy(output[i], token, CT_STATS_STRING_MAX);
		output[i][CT_STATS_STRING_MAX-1]='\0';
		token = strtok(NULL, " ");
	}
	max = i;

	if (fgets(buf, sizeof(buf), fd) == NULL) {
		ret = -1;
		goto out_err;
	}

	nl = strchr(buf, '\n');
	while (nl != NULL) {
		*nl = '\0';
		nl = strchr(buf, '\n');
	}
	token = strtok(buf, " ");
	for (i=0; token != NULL && i<CT_STATS_ENTRIES_MAX; i++) { 
		value[i] = (unsigned int) strtol(token, (char**) NULL, 16);
		token = strtok(NULL, " ");
	}

	for (i=0; i<max; i++)
		printf("%-10s\t\t%-8u\n", output[i], value[i]);

out_err:
	fclose(fd);
	return ret;
}

static struct ctproto_handler *h;

int main(int argc, char *argv[])
{
	int c, cmd;
	unsigned int type = 0, event_mask = 0, l4flags = 0, status = 0;
	int res = 0, partial;
	size_t socketbuffersize = 0;
	int family = AF_UNSPEC;
	int l3protonum, protonum = 0;
	union ct_address ad;
	unsigned int command = 0;

	/* we release these objects in the exit_error() path. */
	if (!alloc_tmpl_objects())
		exit_error(OTHER_PROBLEM, "out of memory");

	register_tcp();
	register_udp();
	register_udplite();
	register_sctp();
	register_dccp();
	register_icmp();
	register_icmpv6();
	register_gre();
	register_unknown();

	/* disable explicit missing arguments error output from getopt_long */
	opterr = 0;

	while ((c = getopt_long(argc, argv, getopt_str, opts, NULL)) != -1) {
	switch(c) {
		/* commands */
		case 'L':
		case 'I':
		case 'D':
		case 'G':
		case 'F':
		case 'E':
		case 'V':
		case 'h':
		case 'C':
			type = check_type(argc, argv);
			add_command(&command, cmd2type[c][type]);
			break;
		case 'U':
			type = check_type(argc, argv);
			if (type == 0)
				add_command(&command, CT_UPDATE);
			else
				exit_error(PARAMETER_PROBLEM, 
					   "Can't update expectations");
			break;
		case 'S':
			add_command(&command, X_STATS);
			break;
		/* options */
		case 's':
		case 'd':
		case 'r':
		case 'q':
			options |= opt2type[c];

			l3protonum = parse_addr(optarg, &ad);
			if (l3protonum == AF_UNSPEC) {
				exit_error(PARAMETER_PROBLEM,
					   "Invalid IP address `%s'", optarg);
			}
			set_family(&family, l3protonum);
			if (l3protonum == AF_INET) {
				nfct_set_attr_u32(tmpl.ct,
						  opt2family_attr[c][0],
						  ad.v4);
			} else if (l3protonum == AF_INET6) {
				nfct_set_attr(tmpl.ct,
					      opt2family_attr[c][1],
					      &ad.v6);
			}
			nfct_set_attr_u8(tmpl.ct, opt2attr[c], l3protonum);
			break;
		case '{':
		case '}':
		case '[':
		case ']':
			options |= opt2type[c];
			l3protonum = parse_addr(optarg, &ad);
			if (l3protonum == AF_UNSPEC) {
				exit_error(PARAMETER_PROBLEM,
					   "Invalid IP address `%s'", optarg);
			}
			set_family(&family, l3protonum);
			if (l3protonum == AF_INET) {
				nfct_set_attr_u32(tmpl.mask, 
						  opt2family_attr[c][0],
						  ad.v4);
			} else if (l3protonum == AF_INET6) {
				nfct_set_attr(tmpl.mask,
					      opt2family_attr[c][1],
					      &ad.v6);
			}
			nfct_set_attr_u8(tmpl.mask,
					 ATTR_ORIG_L3PROTO, l3protonum);
			break;
		case 'p':
			options |= CT_OPT_PROTO;
			h = findproto(optarg, &protonum);
			if (!h)
				exit_error(PARAMETER_PROBLEM,
					   "`%s' unsupported protocol",
					   optarg);

			opts = merge_options(opts, h->opts, &h->option_offset);
			if (opts == NULL)
				exit_error(OTHER_PROBLEM, "out of memory");

			nfct_set_attr_u8(tmpl.ct, ATTR_L4PROTO, protonum);
			break;
		case 't':
			options |= CT_OPT_TIMEOUT;
			nfct_set_attr_u32(tmpl.ct, ATTR_TIMEOUT, atol(optarg));
			nfexp_set_attr_u32(tmpl.exp,
					   ATTR_EXP_TIMEOUT, atol(optarg));
			break;
		case 'u':
			options |= CT_OPT_STATUS;
			parse_parameter(optarg, &status, PARSE_STATUS);
			nfct_set_attr_u32(tmpl.ct, ATTR_STATUS, status);
			break;
		case 'e':
			options |= CT_OPT_EVENT_MASK;
			parse_parameter(optarg, &event_mask, PARSE_EVENT);
			break;
		case 'o':
			options |= CT_OPT_OUTPUT;
			parse_parameter(optarg, &output_mask, PARSE_OUTPUT);
			break;
		case 'z':
			options |= CT_OPT_ZERO;
			break;
		case 'n':
		case 'g':
		case 'j': {
			char *tmp = NULL;

			options |= opt2type[c];

			if (optarg)
				continue;
			else if (optind < argc && argv[optind][0] != '-'
				 && argv[optind][0] != '!')
				tmp = argv[optind++];

			if (tmp == NULL)
				continue;

			set_family(&family, AF_INET);
			nat_parse(tmp, tmpl.ct, opt2type[c]);
			break;
		}
		case 'w':
			options |= opt2type[c];
			nfct_set_attr_u16(tmpl.ct,
					  opt2attr[c],
					  strtoul(optarg, NULL, 0));
			break;
		case 'i':
		case 'm':
		case 'c':
			options |= opt2type[c];
			nfct_set_attr_u32(tmpl.ct,
					  opt2attr[c],
					  strtoul(optarg, NULL, 0));
			break;
		case 'a':
			fprintf(stderr, "WARNING: ignoring -%c, "
					"deprecated option.\n", c);
			break;
		case 'f':
			options |= CT_OPT_FAMILY;
			if (strncmp(optarg, "ipv4", strlen("ipv4")) == 0)
				set_family(&family, AF_INET);
			else if (strncmp(optarg, "ipv6", strlen("ipv6")) == 0)
				set_family(&family, AF_INET6);
			else
				exit_error(PARAMETER_PROBLEM,
					   "`%s' unsupported protocol",
					   optarg);
			break;
		case 'b':
			socketbuffersize = atol(optarg);
			options |= CT_OPT_BUFFERSIZE;
			break;
		case '?':
			if (optopt)
				exit_error(PARAMETER_PROBLEM,
					   "option `%s' requires an "
					   "argument", argv[optind-1]);
			else
				exit_error(PARAMETER_PROBLEM,
					   "unknown option `%s'", 
					   argv[optind-1]);
			break;
		default:
			if (h && h->parse_opts 
			    &&!h->parse_opts(c - h->option_offset, tmpl.ct,
			    		     tmpl.exptuple, tmpl.mask,
					     &l4flags))
				exit_error(PARAMETER_PROBLEM, "parse error");
			break;
		}
	}

	/* default family */
	if (family == AF_UNSPEC)
		family = AF_INET;

	/* we cannot check this combination with generic_opt_check. */
	if (options & CT_OPT_ANY_NAT &&
	   ((options & CT_OPT_SRC_NAT) || (options & CT_OPT_DST_NAT))) {
		exit_error(PARAMETER_PROBLEM, "cannot specify `--src-nat' or "
					      "`--dst-nat' with `--any-nat'");
	}
	cmd = bit2cmd(command);
	res = generic_opt_check(options, NUMBER_OF_OPT,
				commands_v_options[cmd], optflags,
				addr_valid_flags, ADDR_VALID_FLAGS_MAX,
				&partial);
	if (!res) {
		switch(partial) {
		case -1:
		case 0:
			exit_error(PARAMETER_PROBLEM, "you have to specify "
						      "`--src' and `--dst'");
			break;
		case 1:
			exit_error(PARAMETER_PROBLEM, "you have to specify "
						      "`--reply-src' and "
						      "`--reply-dst'");
			break;
		}
	}
	if (!(command & CT_HELP) && h && h->final_check)
		h->final_check(l4flags, cmd, tmpl.ct);

	switch(command) {

	case CT_LIST:
		cth = nfct_open(CONNTRACK, 0);
		if (!cth)
			exit_error(OTHER_PROBLEM, "Can't open handler");

		if (options & CT_COMPARISON && 
		    options & CT_OPT_ZERO)
			exit_error(PARAMETER_PROBLEM, "Can't use -z with "
						      "filtering parameters");

		nfct_callback_register(cth, NFCT_T_ALL, dump_cb, tmpl.ct);

		if (options & CT_OPT_ZERO)
			res = nfct_query(cth, NFCT_Q_DUMP_RESET, &family);
		else
			res = nfct_query(cth, NFCT_Q_DUMP, &family);

		if (dump_xml_header_done == 0) {
			printf("</conntrack>\n");
			fflush(stdout);
		}

		nfct_close(cth);
		break;

	case EXP_LIST:
		cth = nfct_open(EXPECT, 0);
		if (!cth)
			exit_error(OTHER_PROBLEM, "Can't open handler");

		nfexp_callback_register(cth, NFCT_T_ALL, dump_exp_cb, NULL);
		res = nfexp_query(cth, NFCT_Q_DUMP, &family);
		nfct_close(cth);
		break;
			
	case CT_CREATE:
		if ((options & CT_OPT_ORIG) && !(options & CT_OPT_REPL))
		    	nfct_setobjopt(tmpl.ct, NFCT_SOPT_SETUP_REPLY);
		else if (!(options & CT_OPT_ORIG) && (options & CT_OPT_REPL))
			nfct_setobjopt(tmpl.ct, NFCT_SOPT_SETUP_ORIGINAL);

		cth = nfct_open(CONNTRACK, 0);
		if (!cth)
			exit_error(OTHER_PROBLEM, "Can't open handler");

		res = nfct_query(cth, NFCT_Q_CREATE, tmpl.ct);
		if (res != -1)
			counter++;
		nfct_close(cth);
		break;

	case EXP_CREATE:
		nfexp_set_attr(tmpl.exp, ATTR_EXP_MASTER, tmpl.ct);
		nfexp_set_attr(tmpl.exp, ATTR_EXP_EXPECTED, tmpl.exptuple);
		nfexp_set_attr(tmpl.exp, ATTR_EXP_MASK, tmpl.mask);

		cth = nfct_open(EXPECT, 0);
		if (!cth)
			exit_error(OTHER_PROBLEM, "Can't open handler");

		res = nfexp_query(cth, NFCT_Q_CREATE, tmpl.exp);
		nfct_close(cth);
		break;

	case CT_UPDATE:
		cth = nfct_open(CONNTRACK, 0);
		/* internal handler for delete_cb, otherwise we hit EILSEQ */
		ith = nfct_open(CONNTRACK, 0);
		if (!cth || !ith)
			exit_error(OTHER_PROBLEM, "Can't open handler");

		nfct_callback_register(cth, NFCT_T_ALL, update_cb, tmpl.ct);

		res = nfct_query(cth, NFCT_Q_DUMP, &family);
		nfct_close(ith);
		nfct_close(cth);
		break;
		
	case CT_DELETE:
		cth = nfct_open(CONNTRACK, 0);
		ith = nfct_open(CONNTRACK, 0);
		if (!cth || !ith)
			exit_error(OTHER_PROBLEM, "Can't open handler");

		nfct_callback_register(cth, NFCT_T_ALL, delete_cb, tmpl.ct);

		res = nfct_query(cth, NFCT_Q_DUMP, &family);
		nfct_close(ith);
		nfct_close(cth);
		break;

	case EXP_DELETE:
		nfexp_set_attr(tmpl.exp, ATTR_EXP_EXPECTED, tmpl.ct);

		cth = nfct_open(EXPECT, 0);
		if (!cth)
			exit_error(OTHER_PROBLEM, "Can't open handler");

		res = nfexp_query(cth, NFCT_Q_DESTROY, tmpl.exp);
		nfct_close(cth);
		break;

	case CT_GET:
		cth = nfct_open(CONNTRACK, 0);
		if (!cth)
			exit_error(OTHER_PROBLEM, "Can't open handler");

		nfct_callback_register(cth, NFCT_T_ALL, dump_cb, tmpl.ct);
		res = nfct_query(cth, NFCT_Q_GET, tmpl.ct);
		nfct_close(cth);
		break;

	case EXP_GET:
		nfexp_set_attr(tmpl.exp, ATTR_EXP_MASTER, tmpl.ct);

		cth = nfct_open(EXPECT, 0);
		if (!cth)
			exit_error(OTHER_PROBLEM, "Can't open handler");

		nfexp_callback_register(cth, NFCT_T_ALL, dump_exp_cb, NULL);
		res = nfexp_query(cth, NFCT_Q_GET, tmpl.exp);
		nfct_close(cth);
		break;

	case CT_FLUSH:
		cth = nfct_open(CONNTRACK, 0);
		if (!cth)
			exit_error(OTHER_PROBLEM, "Can't open handler");
		res = nfct_query(cth, NFCT_Q_FLUSH, &family);
		nfct_close(cth);
		fprintf(stderr, "%s v%s (conntrack-tools): ",PROGNAME,VERSION);
		fprintf(stderr,"connection tracking table has been emptied.\n");
		break;

	case EXP_FLUSH:
		cth = nfct_open(EXPECT, 0);
		if (!cth)
			exit_error(OTHER_PROBLEM, "Can't open handler");
		res = nfexp_query(cth, NFCT_Q_FLUSH, &family);
		nfct_close(cth);
		fprintf(stderr, "%s v%s (conntrack-tools): ",PROGNAME,VERSION);
		fprintf(stderr,"expectation table has been emptied.\n");
		break;
		
	case CT_EVENT:
		if (options & CT_OPT_EVENT_MASK) {
			unsigned int nl_events = 0;

			if (event_mask & CT_EVENT_F_NEW)
				nl_events |= NF_NETLINK_CONNTRACK_NEW;
			if (event_mask & CT_EVENT_F_UPD)
				nl_events |= NF_NETLINK_CONNTRACK_UPDATE;
			if (event_mask & CT_EVENT_F_DEL)
				nl_events |= NF_NETLINK_CONNTRACK_DESTROY;

			cth = nfct_open(CONNTRACK, nl_events);
		} else {
			cth = nfct_open(CONNTRACK,
					NF_NETLINK_CONNTRACK_NEW |
					NF_NETLINK_CONNTRACK_UPDATE |
					NF_NETLINK_CONNTRACK_DESTROY);
		}

		if (!cth)
			exit_error(OTHER_PROBLEM, "Can't open handler");

		if (options & CT_OPT_BUFFERSIZE) {
			size_t ret;
			ret = nfnl_rcvbufsiz(nfct_nfnlh(cth), socketbuffersize);
			fprintf(stderr, "NOTICE: Netlink socket buffer size "
					"has been set to %zu bytes.\n", ret);
		}
		signal(SIGINT, event_sighandler);
		signal(SIGTERM, event_sighandler);
		nfct_callback_register(cth, NFCT_T_ALL, event_cb, tmpl.ct);
		res = nfct_catch(cth);
		if (res == -1) {
			if (errno == ENOBUFS) {
				fprintf(stderr, 
					"WARNING: We have hit ENOBUFS! We "
					"are losing events.\nThis message "
					"means that the current netlink "
					"socket buffer size is too small.\n"
					"Please, check --buffer-size in "
					"conntrack(8) manpage.\n");
			}
		}
		nfct_close(cth);
		break;

	case EXP_EVENT:
		if (options & CT_OPT_EVENT_MASK) {
			unsigned int nl_events = 0;

			if (event_mask & CT_EVENT_F_NEW)
				nl_events |= NF_NETLINK_CONNTRACK_EXP_NEW;
			if (event_mask & CT_EVENT_F_UPD)
				nl_events |= NF_NETLINK_CONNTRACK_EXP_UPDATE;
			if (event_mask & CT_EVENT_F_DEL)
				nl_events |= NF_NETLINK_CONNTRACK_EXP_DESTROY;

			cth = nfct_open(CONNTRACK, nl_events);
		} else {
			cth = nfct_open(EXPECT,
					NF_NETLINK_CONNTRACK_EXP_NEW |
					NF_NETLINK_CONNTRACK_EXP_UPDATE |
					NF_NETLINK_CONNTRACK_EXP_DESTROY);
		}

		if (!cth)
			exit_error(OTHER_PROBLEM, "Can't open handler");
		signal(SIGINT, event_sighandler);
		signal(SIGTERM, event_sighandler);
		nfexp_callback_register(cth, NFCT_T_ALL, event_exp_cb, NULL);
		res = nfexp_catch(cth);
		nfct_close(cth);
		break;
	case CT_COUNT: {
#define NF_CONNTRACK_COUNT_PROC "/proc/sys/net/netfilter/nf_conntrack_count"
		FILE *fd;
		int count;
		fd = fopen(NF_CONNTRACK_COUNT_PROC, "r");
		if (fd == NULL) {
			exit_error(OTHER_PROBLEM, "Can't open %s",
				   NF_CONNTRACK_COUNT_PROC);
		}
		if (fscanf(fd, "%d", &count) != 1) {
			exit_error(OTHER_PROBLEM, "Can't read %s",
				   NF_CONNTRACK_COUNT_PROC);
		}
		fclose(fd);
		printf("%d\n", count);
		break;
	}
	case EXP_COUNT:
		cth = nfct_open(EXPECT, 0);
		if (!cth)
			exit_error(OTHER_PROBLEM, "Can't open handler");

		nfexp_callback_register(cth, NFCT_T_ALL, count_exp_cb, NULL);
		res = nfexp_query(cth, NFCT_Q_DUMP, &family);
		nfct_close(cth);
		printf("%d\n", counter);
		break;
	case X_STATS:
		if (display_proc_conntrack_stats() < 0)
			exit_error(OTHER_PROBLEM, "Can't open /proc interface");
		break;
	case CT_VERSION:
		printf("%s v%s (conntrack-tools)\n", PROGNAME, VERSION);
		break;
	case CT_HELP:
		usage(argv[0]);
		if (options & CT_OPT_PROTO)
			extension_help(h, protonum);
		break;
	default:
		usage(argv[0]);
		break;
	}

	if (res < 0)
		exit_error(OTHER_PROBLEM, "Operation failed: %s",
			   err2str(errno, command));

	free_tmpl_objects();
	free_options();

	if (command && exit_msg[cmd][0]) {
		fprintf(stderr, "%s v%s (conntrack-tools): ",PROGNAME,VERSION);
		fprintf(stderr, exit_msg[cmd], counter);
		if (counter == 0 && !(command & (CT_LIST | EXP_LIST)))
			return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
