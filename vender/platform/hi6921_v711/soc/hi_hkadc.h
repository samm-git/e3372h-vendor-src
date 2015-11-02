/*************************************************************************
*   版权所有(C) 2008-2013, 深圳华为技术有限公司.
*
*   文 件 名 :  hi_hkadc.h
*
*
*   描    述 :
*
*************************************************************************/
#ifndef __HI_HKADC_H__
#define __HI_HKADC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* this macro is used to avoid the BBP bug */
#define BSP_BBP_HKADC_BUG

enum HKADC_CHANNEL_ID{
    HKADC_CHANNEL_MIN = 0,
    HKADC_CHANNEL_0 = 0,
    HKADC_CHANNEL_1,
    HKADC_CHANNEL_2,
    HKADC_CHANNEL_3,
    HKADC_CHANNEL_4,
    HKADC_CHANNEL_5,
    HKADC_CHANNEL_6,
    HKADC_CHANNEL_7,
    HKADC_CHANNEL_8,
    HKADC_CHANNEL_9,
    HKADC_CHANNEL_MAX = 9, 
    //zhaodandan,based on the revise recording of 2014/04/18 ,reduce 2  HKADC_CHANNEL
};

/*hardware version */
typedef enum _ehard_ware_version
{
    HW_VERSION_BIG_LOW = HKADC_CHANNEL_4,	/* big version low byte*/
	HW_VERSION_BIG_HIGH= HKADC_CHANNEL_5,	/* big version high byte*/
    HW_VERSION_PRODUCT = HKADC_CHANNEL_9,	/* sub version */
    HW_VERSION_UDP = HKADC_CHANNEL_6	/* identify udp*/  //zhaodandan, from channel_11 to channel_6
}hard_ware_version;


#ifdef __cplusplus
}
#endif

#endif

