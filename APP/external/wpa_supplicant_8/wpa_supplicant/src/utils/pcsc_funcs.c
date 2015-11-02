/*
 * WPA Supplicant / PC/SC smartcard interface for USIM, GSM SIM
 * Copyright (c) 2004-2007, 2012, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 *
 * This file implements wrapper functions for accessing GSM SIM and 3GPP USIM
 * cards through PC/SC smartcard library. These functions are used to implement
 * authentication routines for EAP-SIM and EAP-AKA.
 */
/******************************************************************************
修改日期        修改人        问题单号                 简要描述
2012/11/09      l00182072     2111300780        支持EAP-SIM特性
2013/11/07      l00182072     3102807932        RES的长度在4到16个字节变化，需要按照鉴权结果的格式进行解码
2014/07/02      s00208871     4070205391        增加CSIM卡类型处理
******************************************************************************/
#include "includes.h"
//#include <winscard.h>

#include "common.h"
#include "pcsc_funcs.h"
#include "dlfcn.h"

typedef int (*pAt_CommenCmd)(char* cmd, char* matchCmd,char* result);

pAt_CommenCmd at_common_cmd = NULL;
#if 1//(FEATURE_WIFI == FEATURE_ON)
// eap-sim-interface.cpp : Defines the entry point for the console application.
//

#define RETRY_COUNTER 3
#define GSM_AUTH_DATA_LENGTH 12
#define UMTS_AUTH_DATA_LENGTH 51
#define GSM_UMTS_RESULT_LENGTH 4

#define AT_CARDMODE "AT^CARDMODE"
#define AT_CARDMODE_MATCH "^CARDMODE"

#define AT_CPIN "AT+CPIN?"
#define AT_CPIN_MATCH "+CPIN"

#define AT_CPIN_VERIFY "AT+CPIN="
#define AT_CPIN_VERIFY_MATCH "+CPIN"
#define PIN_VERIFY_RESULT "OK"
#define SIM_PIN_READY_STATUS "+CPIN: READY"
#define SIM_PIN_LOCK_STATUS "+CPIN: SIM PIN"

#define AT_ISMI "AT+CIMI"
#define AT_ISMI_MATCH "+CIMI"

#define AT_CSIM_GSM_SELFILE "AT+CSIM=14,\"a0a40000027f20\""
#define AT_CSIM_GSM_SELFILE_MATCH "+CSIM"
#define AT_CSIM_GSM_GETFILE "AT+CSIM=10,\"00c00000"
#define AT_CSIM_GSM_GETFILE_MATCH "+CSIM"
#define AT_CSIM_GSM_SIM_AUTH "AT+CSIM=42,\"a088000010"
#define AT_CSIM_GSM_USIM_AUTH "AT+CSIM=44,\"008800801110"
#define AT_CSIM_GSM_AUTH_MATCH "+CSIM"
#define GSM_AUTH_SIM_SUCCESS_TOKEN "9F0C"
#define GSM_AUTH_USIM_SUCCESS_TOKEN "610E"
#define GSM_AUTH_RAND_LENGTH 16
#define GSM_AUTH_SRES_LENGTH 4
#define GSM_AUTH_KC_LENGTH 8

#define AT_CSIM_UMTS_SELFILE "AT+CSIM=14,\"00a40804027fff\""
#define AT_CSIM_UMTS_SELFILE_MATCH "+CSIM"
#define AT_CSIM_UMTS_GETFILE "AT+CSIM=10,\"00c00000"
#define AT_CSIM_UMTS_GETFILE_MATCH "+CSIM"
#define AT_CSIM_UMTS_AUTH "AT+CSIM=78,\"008800812210"
#define AT_CSIM_UMTS_AUTH_MATCH "+CSIM"
#define UMTS_AUTH_SYNC_FAIL_TOKEN "6110"
#define UMTS_AUTH_RAND_LENGTH 16
#define UMTS_AUTH_AUTN_LENGTH 16

#define UMTS_AUTH_SUCCESS_TOKEN 0x61
#define RES_MAX_LEN    16
#define UMTS_CK_LENGTH 16
#define UMTS_IK_LENGTH 16


#define UMTS_AUTS_LENGTH 14
#define UMTS_AUTH_RESULT_TAG_LENGTH 1

/* BEGIN PN: 2111300780, Modified by l00182072, 2012/11/9*/
#define AT_CRSM_READ_MNC "AT+CRSM=176,28589,0,0,4"   
#define AT_CRSM_CMD_MATCH "+CRSM"
#define AT_CMD_LEN 128
#define AT_CMD_RESPONSE 256
#define POS_MNC_LEN 9

#define  UMTS_AUTH_TOKEN        "DB"
#define  UMTS_AUTH_TOKEN_LEN    1

typedef enum { SCARD_GSM_SIM, SCARD_USIM } sim_types;
struct scard_data {
	sim_types sim_type;
	int data;
};

int get_at_reply_len(char*buf, int len);
/* END   PN: 2111300780, Modified by l00182072, 2012/11/9*/

struct scard_data *scard;
char cmd[128] = {'\0'};
char result[256] = {'\0'};
char tempBuf[256] = {'\0'};
char tempBuf2[256] = {'\0'};
    
//int at_common_cmd(const char* cmd, const char* matchCmd, char* result);
int hex2num(const char c);
int hex2byte(const char *hex);
int string2hex(const char *srcStr,char *result,int length);
int hex2string(const char *srcStr,char *result,int length);

void set_pin_test(const char *pin);
void get_imsi_test();
void gsm_auth_test(char* rand);
void umts_auth_test(char* rand, char* autn);
void gsm_auth_adv_test(char* rand);
void umts_auth_adv_test(char* rand, char* autn);
char testText[] = {'1','2','3','4','5','6','7','8','9','a','b','c','\0'};

void *pDLLHandle = NULL;

struct scard_data * scard_init(scard_sim_type sim_type, const char *reader)
{

/*Sim card already init ,fake one.*/
    static int debug_flag = 0;
	struct scard_data *scard;
	scard = os_zalloc(sizeof(*scard));
	if (scard == NULL)
		return NULL;
        
    /* BEGIN PN: 2111300780, Modified by l00182072, 2012/11/9*/
    pDLLHandle = dlopen("libwebsdk.so",RTLD_NOW);
    if(NULL == pDLLHandle)
    {
        wpa_printf(MSG_ERROR, "dlopen error!\n");
        os_free(scard);
        return NULL;
    }
    else
    {
        wpa_printf(MSG_ERROR, "dlopen ok!\n");
    }

    at_common_cmd = dlsym(pDLLHandle,"AT_CommenCmd");
    if(NULL == at_common_cmd)
    {
        wpa_printf(MSG_ERROR, "get at_common_cmd func pointer failed!!!\n");
        dlclose(pDLLHandle);
        os_free(scard);
        return NULL;
    }
    else
    {
        wpa_printf(MSG_ERROR, "get at_common_cmd func pointer ok!\n");
    }
    scard->data = 1;
    /* END   PN: 2111300780, Modified by l00182072, 2012/11/9*/

    return scard;
}

void scard_deinit(struct scard_data *scard)
{
    /*Sim card already deinit ,fake one.*/
    if (scard == NULL)
        return;
    
    if(pDLLHandle != NULL)
        dlclose(pDLLHandle);
    
    os_free(scard);
}
int testAll()
{

	char token[256] = {'\0'};
	char sres[5] = {'\0'};
	char kc[9] = {'\0'};
	char imsi[33] = {'\0'};
	char res[5] = {'\0'};
	char ik[17] = {'\0'};
	char ck[17] = {'\0'};
	char auts[15] = {'\0'};
	char * autn = "1111111111118881";

	size_t len = 0;

	//scard_set_pin(scard,"1234");
	set_pin_test("1234");
	//scard_get_imsi(scard,imsi,&len);
	get_imsi_test();
	//printf("imsi:%s len:%d \n",imsi,len);

	memcpy(token, "2011040800000000", strlen("2011040800000000") + 1);
	//scard_gsm_auth(scard,(const unsigned char*)token,(unsigned char*)sres,(unsigned char*)kc);
	gsm_auth_test(token);
	gsm_auth_adv_test("31323334353637383930313233343536");
	umts_auth_adv_test("31323334353637383930313233343536","31323334353637383930313233343536");
	//scard_umts_auth(scard,(const unsigned char*)token,(unsigned char*)autn,(unsigned char*)res,&len,(unsigned char*)ik,(unsigned char*)ck,(unsigned char*)auts);
	umts_auth_test(token,autn);
	return 0;
}


void set_pin_test(const char *pin)
{
       int retVal = 0;
       
	retVal = scard_set_pin(scard,pin);

       if(0 == retVal)
       {
            
            wpa_printf(MSG_ERROR, "set_pin success! \n");
       }
       else
       {
            
            wpa_printf(MSG_ERROR, "set_pin failed! \n");
        }
}

void get_imsi_test()
{	
	char imsi[33] = {'\0'};
	size_t len;
	scard_get_imsi(scard,imsi,&len);
       
       wpa_printf(MSG_ERROR, "imsi:%s len:%d \n",imsi,len);
}

void gsm_auth_test(char* rand)
{
	char sres[5] = {'\0'};
	char kc[9] = {'\0'};
	scard_gsm_auth(scard,(const unsigned char*)rand,(unsigned char*)sres,(unsigned char*)kc);

	char a,b;
	int i = 0;
	printf("\nSRES:");
	for(i = 0; i < 4; i++)
	{
		a = (sres[i] >> 4) & 0xF;
		b = sres[i] & 0xF;

		if(a >= 10)
			a = a - 10 + 'A';
		else
			a = a + '0';

		if(b >= 10)
			b = b - 10 + 'A';
		else
			b = b + '0';
		printf("%c%c",a,b);
	}

	printf("\nKc:");
	for(i = 0; i < 8; i++)
	{
		a = (kc[i] >> 4) & 0xF;
		b = kc[i] & 0xF;

		if(a >= 10)
			a = a - 10 + 'A';
		else
			a = a + '0';

		if(b >= 10)
			b = b - 10 + 'A';
		else
			b = b + '0';
		printf("%c%c",a,b);
	}
	printf("\nsres: %s kc:%s %s%s \n",sres,kc,sres,kc);
}

void umts_auth_test(char* rand, char* autn)
{
	char res[9] = {'\0'};
	char ik[17] = {'\0'};
	char ck[17] = {'\0'};
	char auts[15] = {'\0'};
	size_t len = 0;

	scard_umts_auth(scard,(const unsigned char*)rand,(unsigned char*)autn,(unsigned char*)res,&len,(unsigned char*)ik,(unsigned char*)ck,(unsigned char*)auts);
	char a,b;
	int i = 0;
	printf("\nSRES:");
	for(i = 0; i < 8; i++)
	{
		a = (res[i] >> 4) & 0xF;
		b = res[i] & 0xF;

		if(a >= 10)
			a = a - 10 + 'A';
		else
			a = a + '0';

		if(b >= 10)
			b = b - 10 + 'A';
		else
			b = b + '0';
		printf("%c%c",a,b);
	}

	printf("\nik:");
	for(i = 0; i < 16; i++)
	{
		a = (ik[i] >> 4) & 0xF;
		b = ik[i] & 0xF;

		if(a >= 10)
			a = a - 10 + 'A';
		else
			a = a + '0';

		if(b >= 10)
			b = b - 10 + 'A';
		else
			b = b + '0';
		printf("%c%c",a,b);
	}

	printf("\nck:");
	for(i = 0; i < 16; i++)
	{
		a = (ck[i] >> 4) & 0xF;
		b = ck[i] & 0xF;

		if(a >= 10)
			a = a - 10 + 'A';
		else
			a = a + '0';

		if(b >= 10)
			b = b - 10 + 'A';
		else
			b = b + '0';
		printf("%c%c",a,b);
	}

	printf("\nauts:");
	for(i = 0; i < 14; i++)
	{
		a = (auts[i] >> 4) & 0xF;
		b = auts[i] & 0xF;

		if(a >= 10)
			a = a - 10 + 'A';
		else
			a = a + '0';

		if(b >= 10)
			b = b - 10 + 'A';
		else
			b = b + '0';
		printf("%c%c",a,b);
	}
}

void gsm_auth_adv_test(char* rand)
{
    char tempRand[60];

    hex2string(rand, tempRand, strlen(rand));
    gsm_auth_test(tempRand);
}

void umts_auth_adv_test(char* rand, char* autn)
{
    char tempRand[60];
    char tempAutn[60];

    hex2string(rand, tempRand, strlen(rand));
    hex2string(autn, tempAutn, strlen(autn));
    umts_auth_test(tempRand,tempAutn);
}

int string2hex(const char *srcStr,char *result,int length)
{
	int len = 0;
	int i = 0;
	unsigned char a,b;

	if(NULL == srcStr || NULL == result)
	{
		return -1;
	}

	while(i < length)
	{
		a = (srcStr[i] >> 4) & 0xF;
		b = srcStr[i] & 0xF;

		if(a >= 10)
			a = a - 10 + 'A';
		else
			a = a + '0';

		if(b >= 10)
			b = b - 10 + 'A';
		else
			b = b + '0';

		result[2*i] = a;
		result[2*i + 1] = b;
		i++;
	}

	result[2*i] = '\0';

	return 0;
}

int hex2string(const char *srcStr,char *result,int length)
{
	int len = 0;
	int i = 0;
	unsigned int a;

	if(NULL == srcStr || NULL == result)
	{
		return -1;
	}

	if((length % 2) != 0)
		return -1;

	len = length / 2;

	while(i < len)
	{	
		a = hex2byte(&srcStr[2*i]);
		result[i] = a;
		i++;
	}

	result[i] = '\0';

	return 0;
}

/**
 * scard_set_pin - Set PIN (CHV1/PIN1) code for accessing SIM/USIM commands
 * @scard: Pointer to private data from scard_init()
 * @pin: PIN code as an ASCII string (e.g., "1234")
 * Returns: 0 on success, -1 on failure
 * modify history : new function
 * data   : 2011.12.19
 * author   : w00176398 
 */ 
#define SET_PIN_SUCCESS 0
#define SET_PIN_FAIL -1

int scard_set_pin(struct scard_data *scard, const char *pin)
{
	int retVal = -1;
	int i = 0;

       
	wpa_printf(MSG_DEBUG, "scard_set_pin()++\n");
    
	if(NULL == pin)
		return SET_PIN_FAIL;
       memset(cmd,'\0',sizeof(cmd));
	memcpy(cmd, AT_CPIN_VERIFY,strlen(AT_CPIN_VERIFY)+1);
    strcat(cmd,"\"");
    strcat(cmd,pin);
    strcat(cmd,"\"");
        
    /* BEGIN PN: 2111300780, Modified by l00182072, 2012/11/9*/
    retVal = at_common_cmd(AT_CPIN, AT_CPIN_MATCH, result);
    if(0 == retVal)
    {
        if(0 == memcmp(SIM_PIN_READY_STATUS, result, strlen(SIM_PIN_READY_STATUS) + 1))
        { 
            wpa_printf(MSG_ERROR, "scard_set_pin(): SIM PIN READY\n");
            return SET_PIN_SUCCESS;
        }
        else if(0 == memcmp(SIM_PIN_LOCK_STATUS, result, strlen(SIM_PIN_LOCK_STATUS) + 1))
        {  
            wpa_printf(MSG_ERROR, "scard_set_pin(): SIM PIN LOCK\n");
            /*retVal = at_common_cmd(cmd, AT_CPIN_VERIFY_MATCH, result);*/
        }
    }
    else
    {
        /*retVal = at_common_cmd(cmd, AT_CPIN_VERIFY_MATCH, result);*/
        wpa_printf(MSG_ERROR, "scard_set_pin(): AT+CPIN? failed\n");
    }

    return SET_PIN_FAIL;
    /* END   PN: 2111300780, Modified by l00182072, 2012/11/9*/
}

/**
 * scard_get_imsi - Read IMSI from SIM/USIM card
 * @scard: Pointer to private data from scard_init()
 * @imsi: Buffer for IMSI
 * @len: Length of imsi buffer; set to IMSI length on success
 * Returns: 0 on success, -1 if IMSI file cannot be selected, -2 if IMSI file
 * selection returns invalid result code
 * modify history : new function
 * data   : 2011.12.19
 * author   : w00176398 
 */
 #define GET_IMSI_SUCCESS 0
 #define GET_IMSI_FAIL -1
 #define GET_IMSI_INVALID_RESULT -1


/* BEGIN PN: 2111300780,Added  by l00182072, 2012/11/9*/
int wpa_scard_get_imsi(char *imsi, size_t *len)
{
	int retVal = 0;
    int tempLen = 0;

    wpa_printf(MSG_DEBUG, "wpa_scard_get_imsi()++\n");  

    if(NULL == imsi || NULL == len)
    {        
	    wpa_printf(MSG_ERROR, "wpa_scard_get_imsi(): imsi or len is null pointer\n");
        return GET_IMSI_FAIL;
    }
       
	memcpy(cmd, AT_ISMI,strlen(AT_ISMI)+1);
	retVal = at_common_cmd(cmd, AT_ISMI_MATCH, result);

	if(0 == retVal)
	{
		tempLen = strlen(result);
        *len = tempLen;
		memcpy(imsi, result, tempLen);
		imsi[tempLen] = '\0';
	}
	else
	{
	       
		wpa_printf(MSG_ERROR, "wpa_scard_get_imsi(cmd:%s): send get-imsi cmd failed ret:%d\n",
            cmd,retVal);
        return GET_IMSI_FAIL;
	}
    
    
	wpa_printf(MSG_DEBUG, "wpa_scard_get_imsi()--\n");
	return GET_IMSI_SUCCESS;
}
/* END   PN: 2111300780,Added  by l00182072, 2012/11/9*/
 
int scard_get_imsi(struct scard_data *scard, char *imsi, size_t *len)
{
	int retVal = 0;
    int tempLen = 0;

    wpa_printf(MSG_DEBUG, "scard_get_imsi()++\n");  

    if(NULL == imsi || NULL == len)
    {        
	    wpa_printf(MSG_ERROR, "scard_get_imsi(): imsi or len is null pointer\n");
        return GET_IMSI_FAIL;
    }
       
	memcpy(cmd, AT_ISMI, strlen(AT_ISMI) + 1);
	retVal = at_common_cmd(cmd, AT_ISMI_MATCH, result);

	if(0 == retVal)
	{
		tempLen = strlen(result);
        *len = tempLen;
		memcpy(imsi, result, tempLen);
		imsi[tempLen] = '\0';
	}
	else
	{
	       
		wpa_printf(MSG_ERROR, "scard_get_imsi(cmd:%s): send get-imsi cmd failed ret:%d\n",
            cmd,retVal);
        return GET_IMSI_FAIL;
	}
    
    
	/* BEGIN PN: 2111300780, Modified by l00182072, 2012/11/9*/
	wpa_printf(MSG_DEBUG, "scard_get_imsi()--\n");
	/* END   PN: 2111300780, Modified by l00182072, 2012/11/9*/
	return GET_IMSI_SUCCESS;
}

int get_length_from_str(const char* srcStr, char token, char* result)
{
	int len = 0;
	
	if((NULL == srcStr) || (NULL == result))
	{
		result = NULL;
		return -1;
	}
	
	if(strchr(srcStr,token))
	{
		memcpy(result,strchr(srcStr,token),strlen(strchr(srcStr,token)) + 1);
		len = strlen(result);

		result[0] = result[len-3];
		result[1] = result[len-2];
		result[2] = '\0';
	}
	else
	{
		result = NULL;
		return -1;
	}
	return 0;
}

enum card_type
{
	CARD_NO = 0,
	CARD_SIM = 1,
	CARD_USIM = 2,
	CARD_CSIM = 3
};

enum card_type get_card_type()
{
	int iret = 0;
	enum card_type  ctype;
	const char *nptr = NULL;
  
	ctype = CARD_NO;
	memset(result, 0, sizeof(result));
  
	iret = at_common_cmd(AT_CARDMODE, AT_CARDMODE_MATCH, result);
	wpa_printf(MSG_ERROR, "%s:ret:%d, result:%s",__FUNCTION__, iret, result);
	if(0 == iret)
	{   
      /*  +1是为了跳过字符':', result的buffer长度足够，不会越界 */
      nptr = result + strlen(AT_CARDMODE_MATCH) + 1;
      ctype = (enum card_type)atoi(nptr);
      if( CARD_CSIM < ctype) {
          wpa_printf(MSG_ERROR, "invalid card type:%d\n", ctype);
          ctype = CARD_NO;
      }  
	}
  return ctype;
}

/**
 * scard_gsm_auth - Run GSM authentication command on SIM card
 * @scard: Pointer to private data from scard_init()
 * @_rand: 16-byte RAND value from HLR/AuC
 * @sres: 4-byte buffer for SRES
 * @kc: 8-byte buffer for Kc
 * Returns: 0 on success, -1 if SIM/USIM connection has not been initialized,
 * -2 if authentication command execution fails, -3 if unknown response code
 * for authentication command is received, -4 if reading of response fails,
 * -5 if if response data is of unexpected length
 *
 * This function performs GSM authentication using SIM/USIM card and the
 * provided RAND value from HLR/AuC. If authentication command can be completed
 * successfully, SRES and Kc values will be written into sres and kc buffers.
 * modify history : new function
 * data   : 2011.12.19
 * author   : w00176398 
 */

#define GSM_AUTH_SUCCESS_CODE 0
#define GSM_AUTH_UNINIT_CODE -1
#define GSM_AUTH_EXC_FAIL_CODE -2
#define GSM_AUTH_UNKNOWN_RSP_CODE -3
#define GSM_AUTH_READ_RSP_FAIL_CODE -4
#define GSM_AUTH_UNEXCEPT_LENGTH_CODE -5
/* BEGIN PN: 2111300780, Modified by l00182072, 2012/11/9*/
int scard_gsm_auth(struct scard_data *scard, const unsigned char *_rand,unsigned char *sres, unsigned char *kc)
{
	int retVal = 0;
	enum card_type cardType = CARD_NO;
	
	int len = 0;
	char tempRand[GSM_AUTH_RAND_LENGTH*2 + 1] = {'\0'};
	int dataLen = 0;

	cardType = get_card_type();

       
	wpa_printf(MSG_DEBUG, "scard_gsm_auth()++\n");

	if(CARD_NO == cardType)
	{
		
		wpa_printf(MSG_DEBUG, "No card insert!!!\n");
		return GSM_AUTH_EXC_FAIL_CODE;
	}
	/******************Select File******************/
  if(CARD_USIM == cardType || CARD_CSIM == cardType)
  {
    memcpy(cmd, AT_CSIM_UMTS_SELFILE,strlen(AT_CSIM_UMTS_SELFILE)+1);
  }
  else
  {
    memcpy(cmd, AT_CSIM_GSM_SELFILE,strlen(AT_CSIM_GSM_SELFILE)+1);
  }
    
	retVal = at_common_cmd(cmd, AT_CSIM_GSM_SELFILE_MATCH, result);
	if(retVal != 0)
	{
	       
		wpa_printf(MSG_ERROR, "scard_gsm_auth(cmd:%s): send select file cmd failed\n",cmd);
		return GSM_AUTH_EXC_FAIL_CODE;
	}

	/******************send auth data******************/
	string2hex((const char*)_rand,tempRand,GSM_AUTH_RAND_LENGTH);

	if(CARD_SIM == cardType)
	{
		memcpy(cmd, AT_CSIM_GSM_SIM_AUTH,strlen(AT_CSIM_GSM_SIM_AUTH)+1);
	}
	else if(CARD_USIM == cardType || CARD_CSIM == cardType)
	{
		memcpy(cmd, AT_CSIM_GSM_USIM_AUTH,strlen(AT_CSIM_GSM_USIM_AUTH)+1);
	}
	
	strcat(cmd,(const char *)tempRand);
	strcat(cmd,"\"");
	retVal = at_common_cmd(cmd, AT_CSIM_GSM_GETFILE_MATCH, result);
    if(retVal != 0)
	{
	       
		wpa_printf(MSG_ERROR, "scard_gsm_auth(cmd:%s): send auth data cmd failed\n",cmd);
		return GSM_AUTH_READ_RSP_FAIL_CODE;
	}

	/******************get auth data******************/
	get_length_from_str(result,',',tempBuf);
    
	memcpy(tempBuf2,strchr(result,'"') + 1,GSM_UMTS_RESULT_LENGTH);
	tempBuf2[GSM_UMTS_RESULT_LENGTH] = '\0';

       
	wpa_printf(MSG_DEBUG, "scard_gsm_auth(): auth data length: restult: \n");

	if(CARD_SIM == cardType)
	{
		if(memcmp(tempBuf2, GSM_AUTH_SIM_SUCCESS_TOKEN, 4))
		{
	       	
			wpa_printf(MSG_ERROR, "scard_gsm_auth(SIM cmd:%s): get gsm auth data error,result:%s\n",cmd,result);
			return GSM_AUTH_UNEXCEPT_LENGTH_CODE;
		}
	}
	else	 if(CARD_USIM == cardType || CARD_CSIM == cardType)
	{
		if(memcmp(tempBuf2, GSM_AUTH_USIM_SUCCESS_TOKEN, 4))
		{
	       	
			wpa_printf(MSG_ERROR, "scard_gsm_auth(USIM cmd:%s): get gsm auth data error,result:%s\n",cmd,result);
			return GSM_AUTH_UNEXCEPT_LENGTH_CODE;
		}
	}
		
	memcpy(cmd, AT_CSIM_GSM_GETFILE,strlen(AT_CSIM_GSM_GETFILE)+1);
	strcat(cmd,tempBuf);
	strcat(cmd,"\"");
	retVal = at_common_cmd(cmd, AT_CSIM_GSM_GETFILE_MATCH, result);
	if(retVal != 0)
	{
	       
		wpa_printf(MSG_ERROR, "scard_gsm_auth(cmd:%s): send get auth data cmd failed\n",cmd);
		return GSM_AUTH_READ_RSP_FAIL_CODE;
	}

	memcpy(tempBuf,strchr(result,'"') + 1,strlen(strchr(result,'"')));
	len = strlen(tempBuf);
	tempBuf[len - 1] = '\0';
#if 0
    if((len - 1) != (GSM_AUTH_DATA_LENGTH*2 + 4))
	{      
	       
		wpa_printf(MSG_ERROR, "scard_gsm_auth(): Error dataLen:%d != gsm auth data length:%d \n",len - 1,GSM_AUTH_DATA_LENGTH*2 + 4);
		return GSM_AUTH_UNEXCEPT_LENGTH_CODE;
	}
#endif	   
	hex2string(tempBuf,tempBuf2,len - 1);

       
	wpa_printf(MSG_DEBUG, "scard_gsm_auth():original auth data: after conver auth data:\n");
    if(CARD_SIM == cardType)
    {
        memcpy(sres,tempBuf2,GSM_AUTH_SRES_LENGTH);
        //*(sres + GSM_AUTH_SRES_LENGTH) = '\0';
    	memcpy(kc,tempBuf2+GSM_AUTH_SRES_LENGTH,GSM_AUTH_KC_LENGTH);
    	//*(kc + GSM_AUTH_KC_LENGTH) = '\0';
    }
    else if(CARD_USIM == cardType || CARD_CSIM == cardType)
    {
    	memcpy(sres,tempBuf2 + 1,GSM_AUTH_SRES_LENGTH);
        //*(sres + GSM_AUTH_SRES_LENGTH) = '\0';
    	memcpy(kc,tempBuf2+GSM_AUTH_SRES_LENGTH + 1 + 1,GSM_AUTH_KC_LENGTH);
        //*(kc + GSM_AUTH_KC_LENGTH) = '\0';
    }
       
	wpa_printf(MSG_DEBUG, "scard_gsm_auth()--\n");
	return GSM_AUTH_SUCCESS_CODE;
}
/* END   PN: 2111300780, Modified by l00182072, 2012/11/9*/

/**
 * scard_umts_auth - Run UMTS authentication command on USIM card
 * @scard: Pointer to private data from scard_init()
 * @_rand: 16-byte RAND value from HLR/AuC
 * @autn: 16-byte AUTN value from HLR/AuC
 * @res: 16-byte buffer for RES
 * @res_len: Variable that will be set to RES length
 * @ik: 16-byte buffer for IK
 * @ck: 16-byte buffer for CK
 * @auts: 14-byte buffer for AUTS
 * Returns: 0 on success, -1 on failure, or -2 if USIM reports synchronization
 * failure
 *
 * This function performs AKA authentication using USIM card and the provided
 * RAND and AUTN values from HLR/AuC. If authentication command can be
 * completed successfully, RES, IK, and CK values will be written into provided
 * buffers and res_len is set to length of received RES value. If USIM reports
 * synchronization failure, the received AUTS value will be written into auts
 * buffer. In this case, RES, IK, and CK are not valid.
 * modify history : new function
 * data   : 2011.12.19
 * author   : w00176398 
 */
#define UMTS_AUTH_SUCCESS_CODE    0
#define UMTS_AUTH_FAIL_CODE       -1
#define UMTS_AUTH_SYNC_FAIL_CODE  -2

/* success token的长度，取决于res的length，res的长度范围是4到16字节 */
static int valid_auth_succ_token(char *tag, int len)
{
  /* 继承之前的数组定义长度 */
  char tag_hex[256] = {0};
  
  hex2string(tag, tag_hex, len);
  if (UMTS_AUTH_SUCCESS_TOKEN == tag_hex[0]){
    /*长度的变化范围是49字节到61字节*/
    if (0x31 <= tag_hex[1] && 0x3d >= tag_hex[1]) {
      return 0;
    }
  }
  wpa_printf(MSG_ERROR, "invalid auth token:0x%x%x", tag_hex[0], tag_hex[1]);
  return (-1);
}
static int get_umts_auth_results(unsigned char *pos, int len, unsigned char *res, size_t *res_len,unsigned char *ck, unsigned char *ik)
{
  unsigned char *end = NULL;
  
  end = pos + len;
  /* 6 bytes are 0xdb(1byte) + 1 byte res length + minimum of res(4bytes) */
  if (len < (6 + UMTS_CK_LENGTH + UMTS_IK_LENGTH) || pos[0] != 0xdb) {
	  wpa_printf(MSG_DEBUG, "bad auth result format: length %d, tag:0x%x", len, pos[0]);
    return -1;
  }
  /* 跳过起始标志0xdb */
	pos += 1;
	/* RES */
	if (pos[0] > RES_MAX_LEN || pos + pos[0] > end) {
		wpa_printf(MSG_DEBUG, "invalid RES len:%d", pos[0]);
		return -1;
	}
	*res_len = *pos++;
	os_memcpy(res, pos, *res_len);
	pos += *res_len;
	wpa_hexdump(MSG_DEBUG, "RES", res, *res_len);

	/* CK */
	if (pos[0] != UMTS_CK_LENGTH || pos + UMTS_CK_LENGTH > end) {
		wpa_printf(MSG_DEBUG, "invalid CK len:%d", pos[0]);
		return -1;
	}
	pos++;
	os_memcpy(ck, pos, UMTS_CK_LENGTH);
	pos += UMTS_CK_LENGTH;
	wpa_hexdump(MSG_DEBUG, "CK", ck, UMTS_CK_LENGTH);

	/* IK */
	if (pos[0] != UMTS_IK_LENGTH || pos + UMTS_IK_LENGTH > end) {
		wpa_printf(MSG_DEBUG, "invalid IK len:%d", pos[0]);
		return -1;
	}
	pos++;
	os_memcpy(ik, pos, UMTS_IK_LENGTH);
	pos += UMTS_IK_LENGTH;
	wpa_hexdump(MSG_DEBUG, "IK", ik, UMTS_IK_LENGTH);

	return 0;
}
/* BEGIN PN: 2111300780, Modified by l00182072, 2012/11/9*/
int scard_umts_auth(struct scard_data *scard, const unsigned char *_rand,const unsigned char *autn,unsigned char *res, size_t *res_len,unsigned char *ik, unsigned char *ck, unsigned char *auts)
{
	int retVal = 0;
	int len = 0;
	char tempRand[UMTS_AUTH_RAND_LENGTH*2 + 1] = {'\0'};
	char tempAutn[UMTS_AUTH_AUTN_LENGTH*2 + 1] = {'\0'};
	int resultFlag = 0;
	int dataLen = 0;
       
	wpa_printf(MSG_DEBUG, "umts_auth begin\n");
	/******************Select File******************/
	/*add by s00208871 */
	memset(cmd, 0, sizeof(cmd));
	memset(result, 0, sizeof(result));
	/*add over*/
	memcpy(cmd, AT_CSIM_UMTS_SELFILE, strlen(AT_CSIM_UMTS_SELFILE) + 1);
	retVal = at_common_cmd(cmd, AT_CSIM_UMTS_SELFILE_MATCH, result);
	if(retVal != 0)
	{
		wpa_printf(MSG_ERROR, "umts_auth:select file failed, ret:%d\n", retVal);
		return UMTS_AUTH_FAIL_CODE;
	} 
	get_length_from_str(result, ',' ,tempBuf);	
	memcpy(cmd, AT_CSIM_UMTS_GETFILE, strlen(AT_CSIM_UMTS_GETFILE) + 1);
	strcat(cmd, tempBuf);
	strcat(cmd, "\"");
	retVal = at_common_cmd(cmd, AT_CSIM_UMTS_GETFILE_MATCH, result);
	if(retVal != 0)
	{
		wpa_printf(MSG_ERROR, "umts_auth: get file failed, ret:%d\n", retVal);
		return UMTS_AUTH_FAIL_CODE;
	} 

	/******************send auth data******************/
	string2hex((const char*)_rand,tempRand,UMTS_AUTH_RAND_LENGTH);
	string2hex((const char*)autn,tempAutn,UMTS_AUTH_AUTN_LENGTH);

	/*add by s00208871 */
	memset(cmd, 0, sizeof(cmd));
	memset(result, 0, sizeof(result));
	/*add over*/

	memcpy(cmd, AT_CSIM_UMTS_AUTH,strlen(AT_CSIM_UMTS_AUTH)+1);
	strcat(cmd,(const char *)tempRand);
	strcat(cmd, "10");
	strcat(cmd, (const char *)tempAutn);
	strcat(cmd, "\"");
	retVal = at_common_cmd(cmd, AT_CSIM_UMTS_GETFILE_MATCH, result);
	if(retVal != 0) {
		wpa_printf(MSG_ERROR, "umts_auth:get file failed, ret:%d\n", retVal);
		return UMTS_AUTH_FAIL_CODE;
	} 
	
	/******************get auth data from result******************/
	/* tempBuf保存了鉴权返回的长度字符串 */
	get_length_from_str(result, ',', tempBuf);
  /* tempBuf2保存了AT命令返回的前4个字节 */     
	memcpy(tempBuf2, strchr(result,'"') + 1, GSM_UMTS_RESULT_LENGTH);
	tempBuf2[GSM_UMTS_RESULT_LENGTH] = '\0';

	if(0 == memcmp(tempBuf2, UMTS_AUTH_SYNC_FAIL_TOKEN, GSM_UMTS_RESULT_LENGTH))
	{
	  resultFlag = 1;
	  wpa_printf(MSG_ERROR, "umts_auth:sysnc fail\n");
	}
  /* 根据返回的字符串的取值，决定是否需要读入sim卡获得鉴权结果 */
	if((0 == memcmp(tempBuf2, UMTS_AUTH_SYNC_FAIL_TOKEN, GSM_UMTS_RESULT_LENGTH))
    || (0 == valid_auth_succ_token(tempBuf2, GSM_UMTS_RESULT_LENGTH)))
	{
	  wpa_printf(MSG_DEBUG, "read auth results from sim card");
	  memcpy(cmd, AT_CSIM_UMTS_GETFILE, strlen(AT_CSIM_UMTS_GETFILE) + 1);
	  strcat(cmd, tempBuf);
	  strcat(cmd, "\"");
	  retVal = at_common_cmd(cmd, AT_CSIM_UMTS_GETFILE_MATCH, result);
	  if(retVal != 0)
	  {
	    wpa_printf(MSG_ERROR, "umts_auth: get file failed, ret:%d\n", retVal);
		  return UMTS_AUTH_FAIL_CODE;
	  }
	}
	else if(0 == strncasecmp(tempBuf2, UMTS_AUTH_TOKEN, UMTS_AUTH_TOKEN_LEN))
	{
	  wpa_printf(MSG_DEBUG, "do not read auth results from sim card");
	} else {
	  wpa_printf(MSG_ERROR, "umts_auth:bad auth result:%s", result);
	  return UMTS_AUTH_FAIL_CODE;
  }
  
  /* 先读取AT命令返回的长度 */
  len = get_at_reply_len(result, strlen(result));
  if (0 == len || len > (int)strlen(result)) {
    wpa_printf(MSG_ERROR, "bad auth results:%s\n", result);
    return UMTS_AUTH_FAIL_CODE;
  }
  memset(tempBuf, 0, sizeof(tempBuf));
  memcpy(tempBuf, strchr(result,'"') + 1, len);  
  /* 将鉴权结果转换为hex编码,保存在tempBuf2 */
  hex2string(tempBuf, tempBuf2, len);
  len = len/2;
  if(1 == resultFlag) 
  {
    /* 跳过类型码0xdc和长度0x0e，共2个字节 */
    memcpy(auts, tempBuf2 + UMTS_AUTH_RESULT_TAG_LENGTH + 1, UMTS_AUTS_LENGTH);
    return UMTS_AUTH_SYNC_FAIL_CODE;
  } else {
    retVal = get_umts_auth_results((unsigned char *)tempBuf2, len, res, res_len, ck, ik);
    if (0 == retVal) {
      wpa_printf(MSG_DEBUG, "umts_auth:success\n");
      return UMTS_AUTH_SUCCESS_CODE;
    } else {
      wpa_printf(MSG_ERROR, "umts_auth:bad auth result:%s", tempBuf2);
      return UMTS_AUTH_FAIL_CODE;
    }
  }
}


int get_at_reply_len(char *buf, int len)
{
	int i = 0;
    int j = 0;
	char data_len[AT_CMD_RESPONSE];
	
	memset(data_len, 0, sizeof(data_len));
	while ((i < len) && (' ' != buf[i])) 
	{
		i++;	
	}
	if (i < len)
	{
		if(' ' == buf[i])
		{
			while ((i < len) && (',' != buf[i]))
			{
				data_len[j] = buf[i];
				j++;
				i++;
			}

			if (i < len )
			{
				wpa_printf(MSG_DEBUG, "data len:%s\n", data_len);
				return atoi(data_len);
			}
		}
	}

	return (0);
}


int scard_get_mnc_len(struct scard_data *scard)
{

    int   retVal = -1;
    char  cmd[AT_CMD_LEN];
    char  result[AT_CMD_RESPONSE];
    char  mnc_len_bit = 0;
    char* response = NULL;
    unsigned long   len_of_response = 0;
    int   mnc_len = 2; //default is 2 since MNC is 2 in most cases
    int   i = 0;
  
    memset(cmd,'\0',sizeof(cmd));
    memset(result,'\0',sizeof(result));
	memcpy(cmd, AT_CRSM_READ_MNC, strlen(AT_CRSM_READ_MNC) + 1); 
    retVal = at_common_cmd(cmd, AT_CRSM_CMD_MATCH, result);
	wpa_printf(MSG_DEBUG, "return of Read is %d, result is %s \n",retVal,result);

    //if any abnormal exist, use default value of MNC
    if(0 != retVal) 
    {
		wpa_printf(MSG_DEBUG, "CRSM failed, use default value of MNC \n");
        return mnc_len;
    }

    if(NULL != (response = strchr(result,',')))
    {
        response++;
        if(NULL != (response = strchr(response,',')))
        {
            response++;
            len_of_response = strlen(response);
        	wpa_printf(MSG_DEBUG, "len is %d,response is %s \n",len_of_response,response);

        for(i = 0; '\0' != response[i]; i++)
        {
             	wpa_printf(MSG_DEBUG, "response %d is : %c \n",i,response[i]);

        }
            if(POS_MNC_LEN < len_of_response)
            {
                /*lint -e661*/
                mnc_len_bit = response[POS_MNC_LEN - 1];
                mnc_len = ((mnc_len_bit & 1)? 3 : 2);
                /*lint +e661*/
            }
        }
    }
  
	return mnc_len;
    
}
/* END   PN: 2111300780, Modified by l00182072, 2012/11/9*/
#else
typedef enum { SCARD_GSM_SIM, SCARD_USIM } sim_types;
struct scard_data {
	SCARDCONTEXT ctx;
	SCARDHANDLE card;
	DWORD protocol;
	sim_types sim_type;
	int pin1_required;
};

/* See ETSI GSM 11.11 and ETSI TS 102 221 for details.
 * SIM commands:
 * Command APDU: CLA INS P1 P2 P3 Data
 *   CLA (class of instruction): A0 for GSM, 00 for USIM
 *   INS (instruction)
 *   P1 P2 P3 (parameters, P3 = length of Data)
 * Response APDU: Data SW1 SW2
 *   SW1 SW2 (Status words)
 * Commands (INS P1 P2 P3):
 *   SELECT: A4 00 00 02 <file_id, 2 bytes>
 *   GET RESPONSE: C0 00 00 <len>
 *   RUN GSM ALG: 88 00 00 00 <RAND len = 10>
 *   RUN UMTS ALG: 88 00 81 <len=0x22> data: 0x10 | RAND | 0x10 | AUTN
 *	P1 = ID of alg in card
 *	P2 = ID of secret key
 *   READ BINARY: B0 <offset high> <offset low> <len>
 *   READ RECORD: B2 <record number> <mode> <len>
 *	P2 (mode) = '02' (next record), '03' (previous record),
 *		    '04' (absolute mode)
 *   VERIFY CHV: 20 00 <CHV number> 08
 *   CHANGE CHV: 24 00 <CHV number> 10
 *   DISABLE CHV: 26 00 01 08
 *   ENABLE CHV: 28 00 01 08
 *   UNBLOCK CHV: 2C 00 <00=CHV1, 02=CHV2> 10
 *   SLEEP: FA 00 00 00
 */

/* GSM SIM commands */
#define SIM_CMD_SELECT			0xa0, 0xa4, 0x00, 0x00, 0x02
#define SIM_CMD_RUN_GSM_ALG		0xa0, 0x88, 0x00, 0x00, 0x10
#define SIM_CMD_GET_RESPONSE		0xa0, 0xc0, 0x00, 0x00
#define SIM_CMD_READ_BIN		0xa0, 0xb0, 0x00, 0x00
#define SIM_CMD_READ_RECORD		0xa0, 0xb2, 0x00, 0x00
#define SIM_CMD_VERIFY_CHV1		0xa0, 0x20, 0x00, 0x01, 0x08

/* USIM commands */
#define USIM_CLA			0x00
#define USIM_CMD_RUN_UMTS_ALG		0x00, 0x88, 0x00, 0x81, 0x22
#define USIM_CMD_GET_RESPONSE		0x00, 0xc0, 0x00, 0x00

#define SIM_RECORD_MODE_ABSOLUTE 0x04

#define USIM_FSP_TEMPL_TAG		0x62

#define USIM_TLV_FILE_DESC		0x82
#define USIM_TLV_FILE_ID		0x83
#define USIM_TLV_DF_NAME		0x84
#define USIM_TLV_PROPR_INFO		0xA5
#define USIM_TLV_LIFE_CYCLE_STATUS	0x8A
#define USIM_TLV_FILE_SIZE		0x80
#define USIM_TLV_TOTAL_FILE_SIZE	0x81
#define USIM_TLV_PIN_STATUS_TEMPLATE	0xC6
#define USIM_TLV_SHORT_FILE_ID		0x88
#define USIM_TLV_SECURITY_ATTR_8B	0x8B
#define USIM_TLV_SECURITY_ATTR_8C	0x8C
#define USIM_TLV_SECURITY_ATTR_AB	0xAB

#define USIM_PS_DO_TAG			0x90

#define AKA_RAND_LEN 16
#define AKA_AUTN_LEN 16
#define AKA_AUTS_LEN 14
#define RES_MAX_LEN 16
#define IK_LEN 16
#define CK_LEN 16


/* GSM files
 * File type in first octet:
 * 3F = Master File
 * 7F = Dedicated File
 * 2F = Elementary File under the Master File
 * 6F = Elementary File under a Dedicated File
 */
#define SCARD_FILE_MF		0x3F00
#define SCARD_FILE_GSM_DF	0x7F20
#define SCARD_FILE_UMTS_DF	0x7F50
#define SCARD_FILE_GSM_EF_IMSI	0x6F07
#define SCARD_FILE_GSM_EF_AD	0x6FAD
#define SCARD_FILE_EF_DIR	0x2F00
#define SCARD_FILE_EF_ICCID	0x2FE2
#define SCARD_FILE_EF_CK	0x6FE1
#define SCARD_FILE_EF_IK	0x6FE2

#define SCARD_CHV1_OFFSET	13
#define SCARD_CHV1_FLAG		0x80





#ifdef __MINGW32_VERSION
/* MinGW does not yet support WinScard, so load the needed functions
 * dynamically from winscard.dll for now. */

static HINSTANCE dll = NULL; /* winscard.dll */

static const SCARD_IO_REQUEST *dll_g_rgSCardT0Pci, *dll_g_rgSCardT1Pci;
#undef SCARD_PCI_T0
#define SCARD_PCI_T0 (dll_g_rgSCardT0Pci)
#undef SCARD_PCI_T1
#define SCARD_PCI_T1 (dll_g_rgSCardT1Pci)


static WINSCARDAPI LONG WINAPI
(*dll_SCardEstablishContext)(IN DWORD dwScope,
			     IN LPCVOID pvReserved1,
			     IN LPCVOID pvReserved2,
			     OUT LPSCARDCONTEXT phContext);
#define SCardEstablishContext dll_SCardEstablishContext

static long (*dll_SCardReleaseContext)(long hContext);
#define SCardReleaseContext dll_SCardReleaseContext

static WINSCARDAPI LONG WINAPI
(*dll_SCardListReadersA)(IN SCARDCONTEXT hContext,
			 IN LPCSTR mszGroups,
			 OUT LPSTR mszReaders,
			 IN OUT LPDWORD pcchReaders);
#undef SCardListReaders
#define SCardListReaders dll_SCardListReadersA

static WINSCARDAPI LONG WINAPI
(*dll_SCardConnectA)(IN SCARDCONTEXT hContext,
		     IN LPCSTR szReader,
		     IN DWORD dwShareMode,
		     IN DWORD dwPreferredProtocols,
		     OUT LPSCARDHANDLE phCard,
		     OUT LPDWORD pdwActiveProtocol);
#undef SCardConnect
#define SCardConnect dll_SCardConnectA

static WINSCARDAPI LONG WINAPI
(*dll_SCardDisconnect)(IN SCARDHANDLE hCard,
		       IN DWORD dwDisposition);
#define SCardDisconnect dll_SCardDisconnect

static WINSCARDAPI LONG WINAPI
(*dll_SCardTransmit)(IN SCARDHANDLE hCard,
		     IN LPCSCARD_IO_REQUEST pioSendPci,
		     IN LPCBYTE pbSendBuffer,
		     IN DWORD cbSendLength,
		     IN OUT LPSCARD_IO_REQUEST pioRecvPci,
		     OUT LPBYTE pbRecvBuffer,
		     IN OUT LPDWORD pcbRecvLength);
#define SCardTransmit dll_SCardTransmit

static WINSCARDAPI LONG WINAPI
(*dll_SCardBeginTransaction)(IN SCARDHANDLE hCard);
#define SCardBeginTransaction dll_SCardBeginTransaction

static WINSCARDAPI LONG WINAPI
(*dll_SCardEndTransaction)(IN SCARDHANDLE hCard, IN DWORD dwDisposition);
#define SCardEndTransaction dll_SCardEndTransaction


static int mingw_load_symbols(void)
{
	char *sym;

	if (dll)
		return 0;

	dll = LoadLibrary("winscard");
	if (dll == NULL) {
		wpa_printf(MSG_DEBUG, "WinSCard: Could not load winscard.dll "
			   "library");
		return -1;
	}

#define LOADSYM(s) \
	sym = #s; \
	dll_ ## s = (void *) GetProcAddress(dll, sym); \
	if (dll_ ## s == NULL) \
		goto fail;

	LOADSYM(SCardEstablishContext);
	LOADSYM(SCardReleaseContext);
	LOADSYM(SCardListReadersA);
	LOADSYM(SCardConnectA);
	LOADSYM(SCardDisconnect);
	LOADSYM(SCardTransmit);
	LOADSYM(SCardBeginTransaction);
	LOADSYM(SCardEndTransaction);
	LOADSYM(g_rgSCardT0Pci);
	LOADSYM(g_rgSCardT1Pci);

#undef LOADSYM

	return 0;

fail:
	wpa_printf(MSG_DEBUG, "WinSCard: Could not get address for %s from "
		   "winscard.dll", sym);
	FreeLibrary(dll);
	dll = NULL;
	return -1;
}


static void mingw_unload_symbols(void)
{
	if (dll == NULL)
		return;

	FreeLibrary(dll);
	dll = NULL;
}

#else /* __MINGW32_VERSION */

#define mingw_load_symbols() 0
#define mingw_unload_symbols() do { } while (0)

#endif /* __MINGW32_VERSION */


static int _scard_select_file(struct scard_data *scard, unsigned short file_id,
			      unsigned char *buf, size_t *buf_len,
			      sim_types sim_type, unsigned char *aid,
			      size_t aidlen);
static int scard_select_file(struct scard_data *scard, unsigned short file_id,
			     unsigned char *buf, size_t *buf_len);
static int scard_verify_pin(struct scard_data *scard, const char *pin);
static int scard_get_record_len(struct scard_data *scard,
				unsigned char recnum, unsigned char mode);
static int scard_read_record(struct scard_data *scard,
			     unsigned char *data, size_t len,
			     unsigned char recnum, unsigned char mode);


static int scard_parse_fsp_templ(unsigned char *buf, size_t buf_len,
				 int *ps_do, int *file_len)
{
	unsigned char *pos, *end;

	if (ps_do)
		*ps_do = -1;
	if (file_len)
		*file_len = -1;

	pos = buf;
	end = pos + buf_len;
	if (*pos != USIM_FSP_TEMPL_TAG) {
		wpa_printf(MSG_DEBUG, "SCARD: file header did not "
			   "start with FSP template tag");
		return -1;
	}
	pos++;
	if (pos >= end)
		return -1;
	if ((pos + pos[0]) < end)
		end = pos + 1 + pos[0];
	pos++;
	wpa_hexdump(MSG_DEBUG, "SCARD: file header FSP template",
		    pos, end - pos);

	while (pos + 1 < end) {
		wpa_printf(MSG_MSGDUMP, "SCARD: file header TLV 0x%02x len=%d",
			   pos[0], pos[1]);
		if (pos + 2 + pos[1] > end)
			break;

		switch (pos[0]) {
		case USIM_TLV_FILE_DESC:
			wpa_hexdump(MSG_MSGDUMP, "SCARD: File Descriptor TLV",
				    pos + 2, pos[1]);
			break;
		case USIM_TLV_FILE_ID:
			wpa_hexdump(MSG_MSGDUMP, "SCARD: File Identifier TLV",
				    pos + 2, pos[1]);
			break;
		case USIM_TLV_DF_NAME:
			wpa_hexdump(MSG_MSGDUMP, "SCARD: DF name (AID) TLV",
				    pos + 2, pos[1]);
			break;
		case USIM_TLV_PROPR_INFO:
			wpa_hexdump(MSG_MSGDUMP, "SCARD: Proprietary "
				    "information TLV", pos + 2, pos[1]);
			break;
		case USIM_TLV_LIFE_CYCLE_STATUS:
			wpa_hexdump(MSG_MSGDUMP, "SCARD: Life Cycle Status "
				    "Integer TLV", pos + 2, pos[1]);
			break;
		case USIM_TLV_FILE_SIZE:
			wpa_hexdump(MSG_MSGDUMP, "SCARD: File size TLV",
				    pos + 2, pos[1]);
			if ((pos[1] == 1 || pos[1] == 2) && file_len) {
				if (pos[1] == 1)
					*file_len = (int) pos[2];
				else
					*file_len = ((int) pos[2] << 8) |
						(int) pos[3];
				wpa_printf(MSG_DEBUG, "SCARD: file_size=%d",
					   *file_len);
			}
			break;
		case USIM_TLV_TOTAL_FILE_SIZE:
			wpa_hexdump(MSG_MSGDUMP, "SCARD: Total file size TLV",
				    pos + 2, pos[1]);
			break;
		case USIM_TLV_PIN_STATUS_TEMPLATE:
			wpa_hexdump(MSG_MSGDUMP, "SCARD: PIN Status Template "
				    "DO TLV", pos + 2, pos[1]);
			if (pos[1] >= 2 && pos[2] == USIM_PS_DO_TAG &&
			    pos[3] >= 1 && ps_do) {
				wpa_printf(MSG_DEBUG, "SCARD: PS_DO=0x%02x",
					   pos[4]);
				*ps_do = (int) pos[4];
			}
			break;
		case USIM_TLV_SHORT_FILE_ID:
			wpa_hexdump(MSG_MSGDUMP, "SCARD: Short File "
				    "Identifier (SFI) TLV", pos + 2, pos[1]);
			break;
		case USIM_TLV_SECURITY_ATTR_8B:
		case USIM_TLV_SECURITY_ATTR_8C:
		case USIM_TLV_SECURITY_ATTR_AB:
			wpa_hexdump(MSG_MSGDUMP, "SCARD: Security attribute "
				    "TLV", pos + 2, pos[1]);
			break;
		default:
			wpa_hexdump(MSG_MSGDUMP, "SCARD: Unrecognized TLV",
				    pos, 2 + pos[1]);
			break;
		}

		pos += 2 + pos[1];

		if (pos == end)
			return 0;
	}
	return -1;
}


static int scard_pin_needed(struct scard_data *scard,
			    unsigned char *hdr, size_t hlen)
{
	if (scard->sim_type == SCARD_GSM_SIM) {
		if (hlen > SCARD_CHV1_OFFSET &&
		    !(hdr[SCARD_CHV1_OFFSET] & SCARD_CHV1_FLAG))
			return 1;
		return 0;
	}

	if (scard->sim_type == SCARD_USIM) {
		int ps_do;
		if (scard_parse_fsp_templ(hdr, hlen, &ps_do, NULL))
			return -1;
		/* TODO: there could be more than one PS_DO entry because of
		 * multiple PINs in key reference.. */
		if (ps_do > 0 && (ps_do & 0x80))
			return 1;
		return 0;
	}

	return -1;
}


static int scard_get_aid(struct scard_data *scard, unsigned char *aid,
			 size_t maxlen)
{
	int rlen, rec;
	struct efdir {
		unsigned char appl_template_tag; /* 0x61 */
		unsigned char appl_template_len;
		unsigned char appl_id_tag; /* 0x4f */
		unsigned char aid_len;
		unsigned char rid[5];
		unsigned char appl_code[2]; /* 0x1002 for 3G USIM */
	} *efdir;
	unsigned char buf[127];
	size_t blen;

	efdir = (struct efdir *) buf;
	blen = sizeof(buf);
	if (scard_select_file(scard, SCARD_FILE_EF_DIR, buf, &blen)) {
		wpa_printf(MSG_DEBUG, "SCARD: Failed to read EF_DIR");
		return -1;
	}
	wpa_hexdump(MSG_DEBUG, "SCARD: EF_DIR select", buf, blen);

	for (rec = 1; rec < 10; rec++) {
		rlen = scard_get_record_len(scard, rec,
					    SIM_RECORD_MODE_ABSOLUTE);
		if (rlen < 0) {
			wpa_printf(MSG_DEBUG, "SCARD: Failed to get EF_DIR "
				   "record length");
			return -1;
		}
		blen = sizeof(buf);
		if (rlen > (int) blen) {
			wpa_printf(MSG_DEBUG, "SCARD: Too long EF_DIR record");
			return -1;
		}
		if (scard_read_record(scard, buf, rlen, rec,
				      SIM_RECORD_MODE_ABSOLUTE) < 0) {
			wpa_printf(MSG_DEBUG, "SCARD: Failed to read "
				   "EF_DIR record %d", rec);
			return -1;
		}
		wpa_hexdump(MSG_DEBUG, "SCARD: EF_DIR record", buf, rlen);

		if (efdir->appl_template_tag != 0x61) {
			wpa_printf(MSG_DEBUG, "SCARD: Unexpected application "
				   "template tag 0x%x",
				   efdir->appl_template_tag);
			continue;
		}

		if (efdir->appl_template_len > rlen - 2) {
			wpa_printf(MSG_DEBUG, "SCARD: Too long application "
				   "template (len=%d rlen=%d)",
				   efdir->appl_template_len, rlen);
			continue;
		}

		if (efdir->appl_id_tag != 0x4f) {
			wpa_printf(MSG_DEBUG, "SCARD: Unexpected application "
				   "identifier tag 0x%x", efdir->appl_id_tag);
			continue;
		}

		if (efdir->aid_len < 1 || efdir->aid_len > 16) {
			wpa_printf(MSG_DEBUG, "SCARD: Invalid AID length %d",
				   efdir->aid_len);
			continue;
		}

		wpa_hexdump(MSG_DEBUG, "SCARD: AID from EF_DIR record",
			    efdir->rid, efdir->aid_len);

		if (efdir->appl_code[0] == 0x10 &&
		    efdir->appl_code[1] == 0x02) {
			wpa_printf(MSG_DEBUG, "SCARD: 3G USIM app found from "
				   "EF_DIR record %d", rec);
			break;
		}
	}

	if (rec >= 10) {
		wpa_printf(MSG_DEBUG, "SCARD: 3G USIM app not found "
			   "from EF_DIR records");
		return -1;
	}

	if (efdir->aid_len > maxlen) {
		wpa_printf(MSG_DEBUG, "SCARD: Too long AID");
		return -1;
	}

	os_memcpy(aid, efdir->rid, efdir->aid_len);

	return efdir->aid_len;
}


/**
 * scard_init - Initialize SIM/USIM connection using PC/SC
 * @sim_type: Allowed SIM types (SIM, USIM, or both)
 * @reader: Reader name prefix to search for
 * Returns: Pointer to private data structure, or %NULL on failure
 *
 * This function is used to initialize SIM/USIM connection. PC/SC is used to
 * open connection to the SIM/USIM card and the card is verified to support the
 * selected sim_type. In addition, local flag is set if a PIN is needed to
 * access some of the card functions. Once the connection is not needed
 * anymore, scard_deinit() can be used to close it.
 */
struct scard_data * scard_init(scard_sim_type sim_type, const char *reader)
{
	long ret;
	unsigned long len, pos;
	struct scard_data *scard;
#ifdef CONFIG_NATIVE_WINDOWS
	TCHAR *readers = NULL;
#else /* CONFIG_NATIVE_WINDOWS */
	char *readers = NULL;
#endif /* CONFIG_NATIVE_WINDOWS */
	unsigned char buf[100];
	size_t blen;
	int transaction = 0;
	int pin_needed;

	wpa_printf(MSG_DEBUG, "SCARD: initializing smart card interface");
	if (mingw_load_symbols())
		return NULL;
	scard = os_zalloc(sizeof(*scard));
	if (scard == NULL)
		return NULL;

	ret = SCardEstablishContext(SCARD_SCOPE_SYSTEM, NULL, NULL,
				    &scard->ctx);
	if (ret != SCARD_S_SUCCESS) {
		wpa_printf(MSG_DEBUG, "SCARD: Could not establish smart card "
			   "context (err=%ld)", ret);
		goto failed;
	}

	ret = SCardListReaders(scard->ctx, NULL, NULL, &len);
	if (ret != SCARD_S_SUCCESS) {
		wpa_printf(MSG_DEBUG, "SCARD: SCardListReaders failed "
			   "(err=%ld)", ret);
		goto failed;
	}

#ifdef UNICODE
	len *= 2;
#endif /* UNICODE */
	readers = os_malloc(len);
	if (readers == NULL) {
		wpa_printf(MSG_INFO, "SCARD: malloc failed\n");
		goto failed;
	}

	ret = SCardListReaders(scard->ctx, NULL, readers, &len);
	if (ret != SCARD_S_SUCCESS) {
		wpa_printf(MSG_DEBUG, "SCARD: SCardListReaders failed(2) "
			   "(err=%ld)", ret);
		goto failed;
	}
	if (len < 3) {
		wpa_printf(MSG_WARNING, "SCARD: No smart card readers "
			   "available.");
		goto failed;
	}
	wpa_hexdump_ascii(MSG_DEBUG, "SCARD: Readers", (u8 *) readers, len);
	/*
	 * readers is a list of available readers. The last entry is terminated
	 * with double null.
	 */
	pos = 0;
#ifdef UNICODE
	/* TODO */
#else /* UNICODE */
	while (pos < len) {
		if (reader == NULL ||
		    os_strncmp(&readers[pos], reader, os_strlen(reader)) == 0)
			break;
		while (pos < len && readers[pos])
			pos++;
		pos++; /* skip separating null */
		if (pos < len && readers[pos] == '\0')
			pos = len; /* double null terminates list */
	}
#endif /* UNICODE */
	if (pos >= len) {
		wpa_printf(MSG_WARNING, "SCARD: No reader with prefix '%s' "
			   "found", reader);
		goto failed;
	}

#ifdef UNICODE
	wpa_printf(MSG_DEBUG, "SCARD: Selected reader='%S'", &readers[pos]);
#else /* UNICODE */
	wpa_printf(MSG_DEBUG, "SCARD: Selected reader='%s'", &readers[pos]);
#endif /* UNICODE */

	ret = SCardConnect(scard->ctx, &readers[pos], SCARD_SHARE_SHARED,
			   SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1,
			   &scard->card, &scard->protocol);
	if (ret != SCARD_S_SUCCESS) {
		if (ret == (long) SCARD_E_NO_SMARTCARD)
			wpa_printf(MSG_INFO, "No smart card inserted.");
		else
			wpa_printf(MSG_WARNING, "SCardConnect err=%lx", ret);
		goto failed;
	}

	os_free(readers);
	readers = NULL;

	wpa_printf(MSG_DEBUG, "SCARD: card=0x%x active_protocol=%lu (%s)",
		   (unsigned int) scard->card, scard->protocol,
		   scard->protocol == SCARD_PROTOCOL_T0 ? "T0" : "T1");

	ret = SCardBeginTransaction(scard->card);
	if (ret != SCARD_S_SUCCESS) {
		wpa_printf(MSG_DEBUG, "SCARD: Could not begin transaction: "
			   "0x%x", (unsigned int) ret);
		goto failed;
	}
	transaction = 1;

	blen = sizeof(buf);

	scard->sim_type = SCARD_GSM_SIM;
	if (sim_type == SCARD_USIM_ONLY || sim_type == SCARD_TRY_BOTH) {
		wpa_printf(MSG_DEBUG, "SCARD: verifying USIM support");
		if (_scard_select_file(scard, SCARD_FILE_MF, buf, &blen,
				       SCARD_USIM, NULL, 0)) {
			wpa_printf(MSG_DEBUG, "SCARD: USIM is not supported");
			if (sim_type == SCARD_USIM_ONLY)
				goto failed;
			wpa_printf(MSG_DEBUG, "SCARD: Trying to use GSM SIM");
			scard->sim_type = SCARD_GSM_SIM;
		} else {
			wpa_printf(MSG_DEBUG, "SCARD: USIM is supported");
			scard->sim_type = SCARD_USIM;
		}
	}

	if (scard->sim_type == SCARD_GSM_SIM) {
		blen = sizeof(buf);
		if (scard_select_file(scard, SCARD_FILE_MF, buf, &blen)) {
			wpa_printf(MSG_DEBUG, "SCARD: Failed to read MF");
			goto failed;
		}

		blen = sizeof(buf);
		if (scard_select_file(scard, SCARD_FILE_GSM_DF, buf, &blen)) {
			wpa_printf(MSG_DEBUG, "SCARD: Failed to read GSM DF");
			goto failed;
		}
	} else {
		unsigned char aid[32];
		int aid_len;

		aid_len = scard_get_aid(scard, aid, sizeof(aid));
		if (aid_len < 0) {
			wpa_printf(MSG_DEBUG, "SCARD: Failed to find AID for "
				   "3G USIM app - try to use standard 3G RID");
			os_memcpy(aid, "\xa0\x00\x00\x00\x87", 5);
			aid_len = 5;
		}
		wpa_hexdump(MSG_DEBUG, "SCARD: 3G USIM AID", aid, aid_len);

		/* Select based on AID = 3G RID from EF_DIR. This is usually
		 * starting with A0 00 00 00 87. */
		blen = sizeof(buf);
		if (_scard_select_file(scard, 0, buf, &blen, scard->sim_type,
				       aid, aid_len)) {
			wpa_printf(MSG_INFO, "SCARD: Failed to read 3G USIM "
				   "app");
			wpa_hexdump(MSG_INFO, "SCARD: 3G USIM AID",
				    aid, aid_len);
			goto failed;
		}
	}

	/* Verify whether CHV1 (PIN1) is needed to access the card. */
	pin_needed = scard_pin_needed(scard, buf, blen);
	if (pin_needed < 0) {
		wpa_printf(MSG_DEBUG, "SCARD: Failed to determine whether PIN "
			   "is needed");
		goto failed;
	}
	if (pin_needed) {
		scard->pin1_required = 1;
		wpa_printf(MSG_DEBUG, "PIN1 needed for SIM access (retry "
			   "counter=%d)", scard_get_pin_retry_counter(scard));
	}

	ret = SCardEndTransaction(scard->card, SCARD_LEAVE_CARD);
	if (ret != SCARD_S_SUCCESS) {
		wpa_printf(MSG_DEBUG, "SCARD: Could not end transaction: "
			   "0x%x", (unsigned int) ret);
	}

	return scard;

failed:
	if (transaction)
		SCardEndTransaction(scard->card, SCARD_LEAVE_CARD);
	os_free(readers);
	scard_deinit(scard);
	return NULL;
}


/**
 * scard_set_pin - Set PIN (CHV1/PIN1) code for accessing SIM/USIM commands
 * @scard: Pointer to private data from scard_init()
 * @pin: PIN code as an ASCII string (e.g., "1234")
 * Returns: 0 on success, -1 on failure
 */
int scard_set_pin(struct scard_data *scard, const char *pin)
{
	if (scard == NULL)
		return -1;

	/* Verify whether CHV1 (PIN1) is needed to access the card. */
	if (scard->pin1_required) {
		if (pin == NULL) {
			wpa_printf(MSG_DEBUG, "No PIN configured for SIM "
				   "access");
			return -1;
		}
		if (scard_verify_pin(scard, pin)) {
			wpa_printf(MSG_INFO, "PIN verification failed for "
				"SIM access");
			return -1;
		}
	}

	return 0;
}


/**
 * scard_deinit - Deinitialize SIM/USIM connection
 * @scard: Pointer to private data from scard_init()
 *
 * This function closes the SIM/USIM connect opened with scard_init().
 */
void scard_deinit(struct scard_data *scard)
{
	long ret;

	if (scard == NULL)
		return;

	wpa_printf(MSG_DEBUG, "SCARD: deinitializing smart card interface");
	if (scard->card) {
		ret = SCardDisconnect(scard->card, SCARD_UNPOWER_CARD);
		if (ret != SCARD_S_SUCCESS) {
			wpa_printf(MSG_DEBUG, "SCARD: Failed to disconnect "
				   "smart card (err=%ld)", ret);
		}
	}

	if (scard->ctx) {
		ret = SCardReleaseContext(scard->ctx);
		if (ret != SCARD_S_SUCCESS) {
			wpa_printf(MSG_DEBUG, "Failed to release smart card "
				   "context (err=%ld)", ret);
		}
	}
	os_free(scard);
	mingw_unload_symbols();
}


static long scard_transmit(struct scard_data *scard,
			   unsigned char *_send, size_t send_len,
			   unsigned char *_recv, size_t *recv_len)
{
	long ret;
	unsigned long rlen;

	wpa_hexdump_key(MSG_DEBUG, "SCARD: scard_transmit: send",
			_send, send_len);
	rlen = *recv_len;
	ret = SCardTransmit(scard->card,
			    scard->protocol == SCARD_PROTOCOL_T1 ?
			    SCARD_PCI_T1 : SCARD_PCI_T0,
			    _send, (unsigned long) send_len,
			    NULL, _recv, &rlen);
	*recv_len = rlen;
	if (ret == SCARD_S_SUCCESS) {
		wpa_hexdump(MSG_DEBUG, "SCARD: scard_transmit: recv",
			    _recv, rlen);
	} else {
		wpa_printf(MSG_WARNING, "SCARD: SCardTransmit failed "
			   "(err=0x%lx)", ret);
	}
	return ret;
}


static int _scard_select_file(struct scard_data *scard, unsigned short file_id,
			      unsigned char *buf, size_t *buf_len,
			      sim_types sim_type, unsigned char *aid,
			      size_t aidlen)
{
	long ret;
	unsigned char resp[3];
	unsigned char cmd[50] = { SIM_CMD_SELECT };
	int cmdlen;
	unsigned char get_resp[5] = { SIM_CMD_GET_RESPONSE };
	size_t len, rlen;

	if (sim_type == SCARD_USIM) {
		cmd[0] = USIM_CLA;
		cmd[3] = 0x04;
		get_resp[0] = USIM_CLA;
	}

	wpa_printf(MSG_DEBUG, "SCARD: select file %04x", file_id);
	if (aid) {
		wpa_hexdump(MSG_DEBUG, "SCARD: select file by AID",
			    aid, aidlen);
		if (5 + aidlen > sizeof(cmd))
			return -1;
		cmd[2] = 0x04; /* Select by AID */
		cmd[4] = aidlen; /* len */
		os_memcpy(cmd + 5, aid, aidlen);
		cmdlen = 5 + aidlen;
	} else {
		cmd[5] = file_id >> 8;
		cmd[6] = file_id & 0xff;
		cmdlen = 7;
	}
	len = sizeof(resp);
	ret = scard_transmit(scard, cmd, cmdlen, resp, &len);
	if (ret != SCARD_S_SUCCESS) {
		wpa_printf(MSG_WARNING, "SCARD: SCardTransmit failed "
			   "(err=0x%lx)", ret);
		return -1;
	}

	if (len != 2) {
		wpa_printf(MSG_WARNING, "SCARD: unexpected resp len "
			   "%d (expected 2)", (int) len);
		return -1;
	}

	if (resp[0] == 0x98 && resp[1] == 0x04) {
		/* Security status not satisfied (PIN_WLAN) */
		wpa_printf(MSG_WARNING, "SCARD: Security status not satisfied "
			   "(PIN_WLAN)");
		return -1;
	}

	if (resp[0] == 0x6e) {
		wpa_printf(MSG_DEBUG, "SCARD: used CLA not supported");
		return -1;
	}

	if (resp[0] != 0x6c && resp[0] != 0x9f && resp[0] != 0x61) {
		wpa_printf(MSG_WARNING, "SCARD: unexpected response 0x%02x "
			   "(expected 0x61, 0x6c, or 0x9f)", resp[0]);
		return -1;
	}
	/* Normal ending of command; resp[1] bytes available */
	get_resp[4] = resp[1];
	wpa_printf(MSG_DEBUG, "SCARD: trying to get response (%d bytes)",
		   resp[1]);

	rlen = *buf_len;
	ret = scard_transmit(scard, get_resp, sizeof(get_resp), buf, &rlen);
	if (ret == SCARD_S_SUCCESS) {
		*buf_len = resp[1] < rlen ? resp[1] : rlen;
		return 0;
	}

	wpa_printf(MSG_WARNING, "SCARD: SCardTransmit err=0x%lx\n", ret);
	return -1;
}


static int scard_select_file(struct scard_data *scard, unsigned short file_id,
			     unsigned char *buf, size_t *buf_len)
{
	return _scard_select_file(scard, file_id, buf, buf_len,
				  scard->sim_type, NULL, 0);
}


static int scard_get_record_len(struct scard_data *scard, unsigned char recnum,
				unsigned char mode)
{
	unsigned char buf[255];
	unsigned char cmd[5] = { SIM_CMD_READ_RECORD /* , len */ };
	size_t blen;
	long ret;

	if (scard->sim_type == SCARD_USIM)
		cmd[0] = USIM_CLA;
	cmd[2] = recnum;
	cmd[3] = mode;
	cmd[4] = sizeof(buf);

	blen = sizeof(buf);
	ret = scard_transmit(scard, cmd, sizeof(cmd), buf, &blen);
	if (ret != SCARD_S_SUCCESS) {
		wpa_printf(MSG_DEBUG, "SCARD: failed to determine file "
			   "length for record %d", recnum);
		return -1;
	}

	wpa_hexdump(MSG_DEBUG, "SCARD: file length determination response",
		    buf, blen);

	if (blen < 2 || (buf[0] != 0x6c && buf[0] != 0x67)) {
		wpa_printf(MSG_DEBUG, "SCARD: unexpected response to file "
			   "length determination");
		return -1;
	}

	return buf[1];
}


static int scard_read_record(struct scard_data *scard,
			     unsigned char *data, size_t len,
			     unsigned char recnum, unsigned char mode)
{
	unsigned char cmd[5] = { SIM_CMD_READ_RECORD /* , len */ };
	size_t blen = len + 3;
	unsigned char *buf;
	long ret;

	if (scard->sim_type == SCARD_USIM)
		cmd[0] = USIM_CLA;
	cmd[2] = recnum;
	cmd[3] = mode;
	cmd[4] = len;

	buf = os_malloc(blen);
	if (buf == NULL)
		return -1;

	ret = scard_transmit(scard, cmd, sizeof(cmd), buf, &blen);
	if (ret != SCARD_S_SUCCESS) {
		os_free(buf);
		return -2;
	}
	if (blen != len + 2) {
		wpa_printf(MSG_DEBUG, "SCARD: record read returned unexpected "
			   "length %ld (expected %ld)",
			   (long) blen, (long) len + 2);
		os_free(buf);
		return -3;
	}

	if (buf[len] != 0x90 || buf[len + 1] != 0x00) {
		wpa_printf(MSG_DEBUG, "SCARD: record read returned unexpected "
			   "status %02x %02x (expected 90 00)",
			   buf[len], buf[len + 1]);
		os_free(buf);
		return -4;
	}

	os_memcpy(data, buf, len);
	os_free(buf);

	return 0;
}


static int scard_read_file(struct scard_data *scard,
			   unsigned char *data, size_t len)
{
	unsigned char cmd[5] = { SIM_CMD_READ_BIN /* , len */ };
	size_t blen = len + 3;
	unsigned char *buf;
	long ret;

	cmd[4] = len;

	buf = os_malloc(blen);
	if (buf == NULL)
		return -1;

	if (scard->sim_type == SCARD_USIM)
		cmd[0] = USIM_CLA;
	ret = scard_transmit(scard, cmd, sizeof(cmd), buf, &blen);
	if (ret != SCARD_S_SUCCESS) {
		os_free(buf);
		return -2;
	}
	if (blen != len + 2) {
		wpa_printf(MSG_DEBUG, "SCARD: file read returned unexpected "
			   "length %ld (expected %ld)",
			   (long) blen, (long) len + 2);
		os_free(buf);
		return -3;
	}

	if (buf[len] != 0x90 || buf[len + 1] != 0x00) {
		wpa_printf(MSG_DEBUG, "SCARD: file read returned unexpected "
			   "status %02x %02x (expected 90 00)",
			   buf[len], buf[len + 1]);
		os_free(buf);
		return -4;
	}

	os_memcpy(data, buf, len);
	os_free(buf);

	return 0;
}


static int scard_verify_pin(struct scard_data *scard, const char *pin)
{
	long ret;
	unsigned char resp[3];
	unsigned char cmd[5 + 8] = { SIM_CMD_VERIFY_CHV1 };
	size_t len;

	wpa_printf(MSG_DEBUG, "SCARD: verifying PIN");

	if (pin == NULL || os_strlen(pin) > 8)
		return -1;

	if (scard->sim_type == SCARD_USIM)
		cmd[0] = USIM_CLA;
	os_memcpy(cmd + 5, pin, os_strlen(pin));
	os_memset(cmd + 5 + os_strlen(pin), 0xff, 8 - os_strlen(pin));

	len = sizeof(resp);
	ret = scard_transmit(scard, cmd, sizeof(cmd), resp, &len);
	if (ret != SCARD_S_SUCCESS)
		return -2;

	if (len != 2 || resp[0] != 0x90 || resp[1] != 0x00) {
		wpa_printf(MSG_WARNING, "SCARD: PIN verification failed");
		return -1;
	}

	wpa_printf(MSG_DEBUG, "SCARD: PIN verified successfully");
	return 0;
}


int scard_get_pin_retry_counter(struct scard_data *scard)
{
	long ret;
	unsigned char resp[3];
	unsigned char cmd[5] = { SIM_CMD_VERIFY_CHV1 };
	size_t len;
	u16 val;

	wpa_printf(MSG_DEBUG, "SCARD: fetching PIN retry counter");

	if (scard->sim_type == SCARD_USIM)
		cmd[0] = USIM_CLA;
	cmd[4] = 0; /* Empty data */

	len = sizeof(resp);
	ret = scard_transmit(scard, cmd, sizeof(cmd), resp, &len);
	if (ret != SCARD_S_SUCCESS)
		return -2;

	if (len != 2) {
		wpa_printf(MSG_WARNING, "SCARD: failed to fetch PIN retry "
			   "counter");
		return -1;
	}

	val = WPA_GET_BE16(resp);
	if (val == 0x63c0 || val == 0x6983) {
		wpa_printf(MSG_DEBUG, "SCARD: PIN has been blocked");
		return 0;
	}

	if (val >= 0x63c0 && val <= 0x63cf)
		return val & 0x000f;

	wpa_printf(MSG_DEBUG, "SCARD: Unexpected PIN retry counter response "
		   "value 0x%x", val);
	return 0;
}


/**
 * scard_get_imsi - Read IMSI from SIM/USIM card
 * @scard: Pointer to private data from scard_init()
 * @imsi: Buffer for IMSI
 * @len: Length of imsi buffer; set to IMSI length on success
 * Returns: 0 on success, -1 if IMSI file cannot be selected, -2 if IMSI file
 * selection returns invalid result code, -3 if parsing FSP template file fails
 * (USIM only), -4 if IMSI does not fit in the provided imsi buffer (len is set
 * to needed length), -5 if reading IMSI file fails.
 *
 * This function can be used to read IMSI from the SIM/USIM card. If the IMSI
 * file is PIN protected, scard_set_pin() must have been used to set the
 * correct PIN code before calling scard_get_imsi().
 */
int scard_get_imsi(struct scard_data *scard, char *imsi, size_t *len)
{
	unsigned char buf[100];
	size_t blen, imsilen, i;
	char *pos;

	wpa_printf(MSG_DEBUG, "SCARD: reading IMSI from (GSM) EF-IMSI");
	blen = sizeof(buf);
	if (scard_select_file(scard, SCARD_FILE_GSM_EF_IMSI, buf, &blen))
		return -1;
	if (blen < 4) {
		wpa_printf(MSG_WARNING, "SCARD: too short (GSM) EF-IMSI "
			   "header (len=%ld)", (long) blen);
		return -2;
	}

	if (scard->sim_type == SCARD_GSM_SIM) {
		blen = (buf[2] << 8) | buf[3];
	} else {
		int file_size;
		if (scard_parse_fsp_templ(buf, blen, NULL, &file_size))
			return -3;
		blen = file_size;
	}
	if (blen < 2 || blen > sizeof(buf)) {
		wpa_printf(MSG_DEBUG, "SCARD: invalid IMSI file length=%ld",
			   (long) blen);
		return -3;
	}

	imsilen = (blen - 2) * 2 + 1;
	wpa_printf(MSG_DEBUG, "SCARD: IMSI file length=%ld imsilen=%ld",
		   (long) blen, (long) imsilen);
	if (blen < 2 || imsilen > *len) {
		*len = imsilen;
		return -4;
	}

	if (scard_read_file(scard, buf, blen))
		return -5;

	pos = imsi;
	*pos++ = '0' + (buf[1] >> 4 & 0x0f);
	for (i = 2; i < blen; i++) {
		unsigned char digit;

		digit = buf[i] & 0x0f;
		if (digit < 10)
			*pos++ = '0' + digit;
		else
			imsilen--;

		digit = buf[i] >> 4 & 0x0f;
		if (digit < 10)
			*pos++ = '0' + digit;
		else
			imsilen--;
	}
	*len = imsilen;

	return 0;
}


/**
 * scard_get_mnc_len - Read length of MNC in the IMSI from SIM/USIM card
 * @scard: Pointer to private data from scard_init()
 * Returns: length (>0) on success, -1 if administrative data file cannot be
 * selected, -2 if administrative data file selection returns invalid result
 * code, -3 if parsing FSP template file fails (USIM only), -4 if length of
 * the file is unexpected, -5 if reading file fails, -6 if MNC length is not
 * in range (i.e. 2 or 3), -7 if MNC length is not available.
 *
 */
int scard_get_mnc_len(struct scard_data *scard)
{
	unsigned char buf[100];
	size_t blen;
	int file_size;

	wpa_printf(MSG_DEBUG, "SCARD: reading MNC len from (GSM) EF-AD");
	blen = sizeof(buf);
	if (scard_select_file(scard, SCARD_FILE_GSM_EF_AD, buf, &blen))
		return -1;
	if (blen < 4) {
		wpa_printf(MSG_WARNING, "SCARD: too short (GSM) EF-AD "
			   "header (len=%ld)", (long) blen);
		return -2;
	}

	if (scard->sim_type == SCARD_GSM_SIM) {
		file_size = (buf[2] << 8) | buf[3];
	} else {
		if (scard_parse_fsp_templ(buf, blen, NULL, &file_size))
			return -3;
	}
	if (file_size == 3) {
		wpa_printf(MSG_DEBUG, "SCARD: MNC length not available");
		return -7;
	}
	if (file_size < 4 || file_size > (int) sizeof(buf)) {
		wpa_printf(MSG_DEBUG, "SCARD: invalid file length=%ld",
			   (long) file_size);
		return -4;
	}

	if (scard_read_file(scard, buf, file_size))
		return -5;
	buf[3] = buf[3] & 0x0f; /* upper nibble reserved for future use  */
	if (buf[3] < 2 || buf[3] > 3) {
		wpa_printf(MSG_DEBUG, "SCARD: invalid MNC length=%ld",
			   (long) buf[3]);
		return -6;
	}
	wpa_printf(MSG_DEBUG, "SCARD: MNC length=%ld", (long) buf[3]);
	return buf[3];
}


/**
 * scard_gsm_auth - Run GSM authentication command on SIM card
 * @scard: Pointer to private data from scard_init()
 * @_rand: 16-byte RAND value from HLR/AuC
 * @sres: 4-byte buffer for SRES
 * @kc: 8-byte buffer for Kc
 * Returns: 0 on success, -1 if SIM/USIM connection has not been initialized,
 * -2 if authentication command execution fails, -3 if unknown response code
 * for authentication command is received, -4 if reading of response fails,
 * -5 if if response data is of unexpected length
 *
 * This function performs GSM authentication using SIM/USIM card and the
 * provided RAND value from HLR/AuC. If authentication command can be completed
 * successfully, SRES and Kc values will be written into sres and kc buffers.
 */
int scard_gsm_auth(struct scard_data *scard, const unsigned char *_rand,
		   unsigned char *sres, unsigned char *kc)
{
	unsigned char cmd[5 + 1 + 16] = { SIM_CMD_RUN_GSM_ALG };
	int cmdlen;
	unsigned char get_resp[5] = { SIM_CMD_GET_RESPONSE };
	unsigned char resp[3], buf[12 + 3 + 2];
	size_t len;
	long ret;

	if (scard == NULL)
		return -1;

	wpa_hexdump(MSG_DEBUG, "SCARD: GSM auth - RAND", _rand, 16);
	if (scard->sim_type == SCARD_GSM_SIM) {
		cmdlen = 5 + 16;
		os_memcpy(cmd + 5, _rand, 16);
	} else {
		cmdlen = 5 + 1 + 16;
		cmd[0] = USIM_CLA;
		cmd[3] = 0x80;
		cmd[4] = 17;
		cmd[5] = 16;
		os_memcpy(cmd + 6, _rand, 16);
	}
	len = sizeof(resp);
	ret = scard_transmit(scard, cmd, cmdlen, resp, &len);
	if (ret != SCARD_S_SUCCESS)
		return -2;

	if ((scard->sim_type == SCARD_GSM_SIM &&
	     (len != 2 || resp[0] != 0x9f || resp[1] != 0x0c)) ||
	    (scard->sim_type == SCARD_USIM &&
	     (len != 2 || resp[0] != 0x61 || resp[1] != 0x0e))) {
		wpa_printf(MSG_WARNING, "SCARD: unexpected response for GSM "
			   "auth request (len=%ld resp=%02x %02x)",
			   (long) len, resp[0], resp[1]);
		return -3;
	}
	get_resp[4] = resp[1];

	len = sizeof(buf);
	ret = scard_transmit(scard, get_resp, sizeof(get_resp), buf, &len);
	if (ret != SCARD_S_SUCCESS)
		return -4;

	if (scard->sim_type == SCARD_GSM_SIM) {
		if (len != 4 + 8 + 2) {
			wpa_printf(MSG_WARNING, "SCARD: unexpected data "
				   "length for GSM auth (len=%ld, expected 14)",
				   (long) len);
			return -5;
		}
		os_memcpy(sres, buf, 4);
		os_memcpy(kc, buf + 4, 8);
	} else {
		if (len != 1 + 4 + 1 + 8 + 2) {
			wpa_printf(MSG_WARNING, "SCARD: unexpected data "
				   "length for USIM auth (len=%ld, "
				   "expected 16)", (long) len);
			return -5;
		}
		if (buf[0] != 4 || buf[5] != 8) {
			wpa_printf(MSG_WARNING, "SCARD: unexpected SREC/Kc "
				   "length (%d %d, expected 4 8)",
				   buf[0], buf[5]);
		}
		os_memcpy(sres, buf + 1, 4);
		os_memcpy(kc, buf + 6, 8);
	}

	wpa_hexdump(MSG_DEBUG, "SCARD: GSM auth - SRES", sres, 4);
	wpa_hexdump(MSG_DEBUG, "SCARD: GSM auth - Kc", kc, 8);

	return 0;
}


/**
 * scard_umts_auth - Run UMTS authentication command on USIM card
 * @scard: Pointer to private data from scard_init()
 * @_rand: 16-byte RAND value from HLR/AuC
 * @autn: 16-byte AUTN value from HLR/AuC
 * @res: 16-byte buffer for RES
 * @res_len: Variable that will be set to RES length
 * @ik: 16-byte buffer for IK
 * @ck: 16-byte buffer for CK
 * @auts: 14-byte buffer for AUTS
 * Returns: 0 on success, -1 on failure, or -2 if USIM reports synchronization
 * failure
 *
 * This function performs AKA authentication using USIM card and the provided
 * RAND and AUTN values from HLR/AuC. If authentication command can be
 * completed successfully, RES, IK, and CK values will be written into provided
 * buffers and res_len is set to length of received RES value. If USIM reports
 * synchronization failure, the received AUTS value will be written into auts
 * buffer. In this case, RES, IK, and CK are not valid.
 */
int scard_umts_auth(struct scard_data *scard, const unsigned char *_rand,
		    const unsigned char *autn,
		    unsigned char *res, size_t *res_len,
		    unsigned char *ik, unsigned char *ck, unsigned char *auts)
{
	unsigned char cmd[5 + 1 + AKA_RAND_LEN + 1 + AKA_AUTN_LEN] =
		{ USIM_CMD_RUN_UMTS_ALG };
	unsigned char get_resp[5] = { USIM_CMD_GET_RESPONSE };
	unsigned char resp[3], buf[64], *pos, *end;
	size_t len;
	long ret;

	if (scard == NULL)
		return -1;

	if (scard->sim_type == SCARD_GSM_SIM) {
		wpa_printf(MSG_ERROR, "SCARD: Non-USIM card - cannot do UMTS "
			   "auth");
		return -1;
	}

	wpa_hexdump(MSG_DEBUG, "SCARD: UMTS auth - RAND", _rand, AKA_RAND_LEN);
	wpa_hexdump(MSG_DEBUG, "SCARD: UMTS auth - AUTN", autn, AKA_AUTN_LEN);
	cmd[5] = AKA_RAND_LEN;
	os_memcpy(cmd + 6, _rand, AKA_RAND_LEN);
	cmd[6 + AKA_RAND_LEN] = AKA_AUTN_LEN;
	os_memcpy(cmd + 6 + AKA_RAND_LEN + 1, autn, AKA_AUTN_LEN);

	len = sizeof(resp);
	ret = scard_transmit(scard, cmd, sizeof(cmd), resp, &len);
	if (ret != SCARD_S_SUCCESS)
		return -1;

	if (len <= sizeof(resp))
		wpa_hexdump(MSG_DEBUG, "SCARD: UMTS alg response", resp, len);

	if (len == 2 && resp[0] == 0x98 && resp[1] == 0x62) {
		wpa_printf(MSG_WARNING, "SCARD: UMTS auth failed - "
			   "MAC != XMAC");
		return -1;
	} else if (len != 2 || resp[0] != 0x61) {
		wpa_printf(MSG_WARNING, "SCARD: unexpected response for UMTS "
			   "auth request (len=%ld resp=%02x %02x)",
			   (long) len, resp[0], resp[1]);
		return -1;
	}
	get_resp[4] = resp[1];

	len = sizeof(buf);
	ret = scard_transmit(scard, get_resp, sizeof(get_resp), buf, &len);
	if (ret != SCARD_S_SUCCESS || len > sizeof(buf))
		return -1;

	wpa_hexdump(MSG_DEBUG, "SCARD: UMTS get response result", buf, len);
	if (len >= 2 + AKA_AUTS_LEN && buf[0] == 0xdc &&
	    buf[1] == AKA_AUTS_LEN) {
		wpa_printf(MSG_DEBUG, "SCARD: UMTS Synchronization-Failure");
		os_memcpy(auts, buf + 2, AKA_AUTS_LEN);
		wpa_hexdump(MSG_DEBUG, "SCARD: AUTS", auts, AKA_AUTS_LEN);
		return -2;
	} else if (len >= 6 + IK_LEN + CK_LEN && buf[0] == 0xdb) {
		pos = buf + 1;
		end = buf + len;

		/* RES */
		if (pos[0] > RES_MAX_LEN || pos + pos[0] > end) {
			wpa_printf(MSG_DEBUG, "SCARD: Invalid RES");
			return -1;
		}
		*res_len = *pos++;
		os_memcpy(res, pos, *res_len);
		pos += *res_len;
		wpa_hexdump(MSG_DEBUG, "SCARD: RES", res, *res_len);

		/* CK */
		if (pos[0] != CK_LEN || pos + CK_LEN > end) {
			wpa_printf(MSG_DEBUG, "SCARD: Invalid CK");
			return -1;
		}
		pos++;
		os_memcpy(ck, pos, CK_LEN);
		pos += CK_LEN;
		wpa_hexdump(MSG_DEBUG, "SCARD: CK", ck, CK_LEN);

		/* IK */
		if (pos[0] != IK_LEN || pos + IK_LEN > end) {
			wpa_printf(MSG_DEBUG, "SCARD: Invalid IK");
			return -1;
		}
		pos++;
		os_memcpy(ik, pos, IK_LEN);
		pos += IK_LEN;
		wpa_hexdump(MSG_DEBUG, "SCARD: IK", ik, IK_LEN);

		return 0;
	}

	wpa_printf(MSG_DEBUG, "SCARD: Unrecognized response");
	return -1;
}
#endif
int scard_supports_umts(struct scard_data *scard)
{
	return scard->sim_type == SCARD_USIM;
}
