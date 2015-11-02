#ifndef _XT_DNS_FLT_H
#define _XT_DNS_FLT_H

#ifdef __KERNEL__

#define DNS_PORT    53

struct stDnsMsgHdr
{
    uint16_t u16ID;
    uint16_t u16Coms;
    uint16_t u16QDcnt;
    uint16_t u16ANcnt;    
    uint16_t u16NScnt;
    uint16_t u16ARcnt;
}  __attribute__((packed));

#define     URL_FLT_DNS_QUERY_CODE      0x0
#define     URL_FLT_DNS_RESP_CODE       0x1

#endif

#define     URL_STRING_LEN              256

typedef struct URLString
{
    unsigned char acURL[URL_STRING_LEN];
    uint16_t      u16Inv;
}URL_STRING_ST;



#if 0
#define DEBUGPC(str, args...)        \
do{                              \
    printk("[%s %s:%d]  ", __FILE__, __FUNCTION__, __LINE__); \
    printk("\033[1;33;40m");   \
    printk(str, ## args);      \
    printk("\033[0;37;40m\n"); \
}while(0) 
#else
#define DEBUGPC(str, args...)
#endif 


#endif
/*Hisilicon Linux add end*/