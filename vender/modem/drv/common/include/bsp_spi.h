/*************************************************************************
*   版权所有(C) 1987-2004, 深圳华为技术有限公司.
*
*   文 件 名 :  bsp_spi.h
*
*
*   描    述 :  SPI模块用户接口文件
*
*   修改记录 :  2011年6月9日  v1.00  xuwenfang  创建
*
*************************************************************************/

#ifndef _BSP_SPI_H_
#define _BSP_SPI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "osl_types.h"
#include "product_config.h"

/*函数声明*/
void bsp_spi_init (void);
s32 spi_recv (u32 spiNo, u32 cs, u16* prevData, u32 recvSize,u16* psendData,u32 sendSize );
s32 spi_send (u32 spiNo, u32 cs, u16* pData, u32 ulLen);
#endif
