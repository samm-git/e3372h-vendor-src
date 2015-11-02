#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>

#include <iptables.h>
#include <linux/netfilter/xt_dns_flt.h>
#define IPTABLES_VERSION "1.4.11.1"


/* Function which prints out usage message. */
static void help(void)
{
	printf(
        "dns_flt match v%s options:\n"
        "[!] --str string	Match DNS name.\n",
        IPTABLES_VERSION);
}

static struct option opts[] = {
	{ "str", 1, 0, '1' },
	{0}
};

/* Initialize the match. */
static void init(struct ipt_entry_match *m, unsigned int *nfcache)
{
	*nfcache |= NFC_IP_PROTO_UNKNOWN;
}


static void parse_dns_str(const char *dnsString, char *pstResult, int len)
{
    int iLen = 0;

    if (!dnsString || !pstResult || ('\0' == *dnsString))
    {
        xtables_error(PARAMETER_PROBLEM, "--str must with a string");
    }

    iLen = strlen(dnsString);
    if (iLen >= len)
    {
        xtables_error(PARAMETER_PROBLEM, "string too long, must not longer than 256");
    }

    memset(pstResult, 0, len);
    strncpy(pstResult, dnsString, iLen);
}

/* Function which parses command options; returns true if it
   ate an option */
static int parse(int c, char **argv, int invert, unsigned int *flags,
      const struct ipt_entry *entry,
      struct ipt_entry_match **match)
{
	 URL_STRING_ST *dnsinfo =
		(URL_STRING_ST *)(*match)->data;

	switch (c) {
	case '1':
		if (*flags)
			xtables_error(PARAMETER_PROBLEM,
				   "Only one `--str' allowed");
        xtables_check_inverse(optarg, &invert, &optind, 0, argv);
		parse_dns_str(argv[optind-1], &dnsinfo->acURL, URL_STRING_LEN);
		if (invert)
			dnsinfo->u16Inv = 1;
		*flags = 1;
		break;
	default:
		return 0;
	}
	return 1;
}

static void print_dns_flt(char *pstStr, int invert)
{
	if (invert)
		printf("! ");

    if (pstStr)
		printf("%s ", pstStr);
}

/* Final check; must have specified --mss. */
static void final_check(unsigned int flags)
{
	if (!flags)
		xtables_error(PARAMETER_PROBLEM,
			   "dns_flt match: You must specify `--str'");
}

/* Prints out the matchinfo. */
static void
print(const struct ipt_ip *ip,
      const struct ipt_entry_match *match,
      int numeric)
{
	const URL_STRING_ST *dnsinfo =
		(const struct URL_STRING_ST *)match->data;

	printf("dns_flt match ");
	print_dns_flt(dnsinfo->acURL, dnsinfo->u16Inv);
}

/* Saves the union ipt_matchinfo in parsable form to stdout. */
static void
save(const struct ipt_ip *ip, const struct ipt_entry_match *match)
{
	const  URL_STRING_ST *dnsinfo =
		(const URL_STRING_ST *)match->data;

	printf("--str ");
	print_dns_flt(dnsinfo->acURL, dnsinfo->u16Inv);
}


static struct xtables_match dns_flt_reg = {
	.name 		= "dns_flt",
        .version 	= XTABLES_VERSION,
	//.family		= NFPROTO_IPV6,
	.size		= XT_ALIGN(sizeof(URL_STRING_ST)),
	.userspacesize 	= XT_ALIGN(sizeof(URL_STRING_ST)),
	.help 		= &help,
	//.init 		= init,
	.parse 		= &parse,
    .final_check = &final_check,
	.print 		= &print,
	.save 		= &save,
	.extra_opts 	= opts,
};

void _init(void)
{
	xtables_register_match(&dns_flt_reg);
}

