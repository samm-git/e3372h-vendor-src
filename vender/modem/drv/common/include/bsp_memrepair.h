/*************************************************************************
*   版权所有(C) 2008-2013, 深圳华为技术有限公司.
*
*   文 件 名 :  bbp_balong.c
*
*
*   描    述    :  本文件主要实现modem memrepair
*
*   修改记录 :  2014年3月12日 创建
*************************************************************************/
#ifndef __BSP_MEMREPAIR_H__
#define __BSP_MEMREPAIR_H__

#define MEMRAIR_WAIT_TIMEOUT    (10)
#define MR_POWERON_FLAG         (0xFAFAFAFA)
#define MODEM_MEMREPAIR_DSP_BIT (13)
#define MODEM_MEMREPAIR_IRM_BIT (14)

#define MEMREPAIR_FAIL 0xffffffff
typedef enum{
	MODEM_MEMREPAIR_DSP = 0,
	MODEM_MEMREPAIR_IRM = 1,
	MODEM_MEMREPAIR_DBG = 2,
	MODEM_MEMREPAIR_BUTT
}modem_memrepair_ip;

int bsp_modem_memrepair(modem_memrepair_ip module);
int bsp_modem_is_need_memrepair(void);
int bsp_get_memrepair_time(void);

#endif
