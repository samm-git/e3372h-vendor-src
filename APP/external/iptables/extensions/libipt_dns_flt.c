/*Hisilicon Linux Add by f00166181 2011-12-26*/
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>

#include <xtables.h>
#include <linux/netfilter/xt_dns_flt.h>

#define IPTABLES_VERSION "1.4.11.1"

/* Function which prints out usage message. */
static void help(void)
{
    printf("dns_flt match v%s options:\n"
                "[!] --str string	Match DNS name.\n",
                IPTABLES_VERSION);
}

static const struct option opts[] = {
    { "str", 1, 0, '1' },
    {.name = NULL}
};

static void parse_dns_str(const char *dnsString, char *pstResult, int len)
{
    size_t iLen = 0;
    if((NULL != dnsString) && (NULL != pstResult) && ('\0' != *dnsString))
    {
        iLen = strlen(dnsString);
        if (iLen >= (size_t)len)
        {
            xtables_error(PARAMETER_PROBLEM, "string too long, must not longer than 256");
        }

        memset(pstResult, 0, (size_t)len);
        strncpy(pstResult, dnsString, (size_t)iLen);
    }
    else
    {
        xtables_error(PARAMETER_PROBLEM, "--str must with a string");
    }
}

/* Function which parses command options; returns true if it
   ate an option */
static int parse(int c, 
                        char ** argv, 
                        int invert, 
                        unsigned int *flags,
                        const void *entry,
                        struct xt_entry_match ** match)//lint !e830
{
    if(NULL == (*match))
    {
        xtables_error(OTHER_PROBLEM,"*match is NULL");
    }
    
    URL_STRING_ST *dnsinfo = (URL_STRING_ST *)((*match)->data); //lint !e826
    
    switch (c) 
    {
        case '1':
            if (*flags)
            {
                xtables_error(PARAMETER_PROBLEM,"Only one `--str' allowed");
            }
            xtables_check_inverse(optarg, &invert, &optind, 0, argv);
            parse_dns_str(argv[optind-1], (char*)(&dnsinfo->acURL), URL_STRING_LEN);
            if (invert)
            {
                dnsinfo->u16Inv = 1;
            }
            *flags = 1;
            break;
        default:
            return 0;
	}
	return 1;
}//lint !e818

static void print_dns_flt(char *pstStr, int invert)
{
    if (invert)
    {   
        printf("! ");
    }

    if (pstStr)
    {
        printf("%s ", pstStr);
    }
}

/* Final check; must have specified --mss. */
static void final_check(unsigned int flags)
{
    if (!flags)
    {   
        xtables_error(PARAMETER_PROBLEM,"dns_flt match: You must specify `--str'");
    }
}

/* Prints out the matchinfo. */
static void print(const void *ip,
                          const struct xt_entry_match *match,
                          int numeric)
{
    const URL_STRING_ST *dnsinfo = (const URL_STRING_ST *)match->data;//lint !e826
    printf("dns_flt match ");
    print_dns_flt((char*)(dnsinfo->acURL), dnsinfo->u16Inv);
}

/* Saves the union ipt_matchinfo in parsable form to stdout. */
static void save(const void *ip, const struct xt_entry_match *match)
{
    const  URL_STRING_ST *dnsinfo = (const URL_STRING_ST *)match->data;//lint !e826
    printf("--str ");
    print_dns_flt((char*)(dnsinfo->acURL), dnsinfo->u16Inv);
}

static struct xtables_match dns_flt = 
{  
	.name		= "dns_flt",
	.version	= XTABLES_VERSION,
	.size		= XT_ALIGN(sizeof(URL_STRING_ST)),
	.userspacesize	= XT_ALIGN(sizeof(URL_STRING_ST)),
	.help		= &help,
	.parse		= &parse,
	.final_check	= &final_check,
	.print		= &print,
	.save		= &save,
	.extra_opts	= opts 
}; //lint !e785

void _init(void) //lint -esym(714, ipt_dns_flt_init)
{
    xtables_register_match(&dns_flt);
}
/*Hisilicon Linux Add END*/

