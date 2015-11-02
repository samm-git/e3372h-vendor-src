/*************************************************************************
*   版权所有(C) 2008-2013, 深圳华为技术有限公司.
*
*   文 件 名 :  bsp_tuner.h
*
*   作    者 :  z00227143
*
*   描    述 :
*
*************************************************************************/
#ifndef __BSP_TUNER_H__
#define __BSP_TUNER_H__


#ifdef __cplusplus
extern "C" {
#endif

#define NV_GU_RF_FEND_MIPI_INIT_CONFIG_ID (10046)


void bsp_tuner_init(void);
int bsp_tuner_resume(struct dpm_device *dev);


#ifdef __cplusplus
}
#endif

#endif

