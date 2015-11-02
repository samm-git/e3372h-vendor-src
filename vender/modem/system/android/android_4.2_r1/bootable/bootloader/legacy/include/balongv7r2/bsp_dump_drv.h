/*************************************************************************
*   版权所有(C) 2008-2012, 深圳华为技术有限公司.
*
*   文 件 名 :  bsp_dump.h
*
*   作    者 :  z67193
*
*   描    述 :  临终遗言功能头文件
*
*   修改记录 :  2012年04月14日  v1.00  z67193  创建
*************************************************************************/

#ifndef	__BSP_DUMP_DRV_H__
#define __BSP_DUMP_DRV_H__

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "product_config.h"
#include <bsp_memmap.h>
#ifndef __ASSEMBLY__

#endif/*#ifndef __ASSEMBLY__*/

/**************************************************************************
  宏定义
**************************************************************************/

/*全局内存分配*/
#define DUMP_MEM_BASE                       DDR_MNTN_ADDR_VIRT
#define DUMP_MEM_TOTAL_SIZE                 DDR_MNTN_SIZE
#define DUMP_GLOBAL_INFO_ADDR               DUMP_MEM_BASE
#define DUMP_GLOBAL_INFO_SIZE               0x100
#define DUMP_GLOBAL_MAP_ADDR                (DUMP_MEM_BASE+DUMP_GLOBAL_INFO_SIZE)
#define DUMP_GLOBAL_MAP_SIZE                0x28
#define DUMP_LOAD_INFO_ADDR                 (DUMP_GLOBAL_MAP_ADDR+DUMP_GLOBAL_MAP_SIZE)
#define DUMP_LOAD_INFO_SIZE                 (0x30)
#define DUMP_APP_SECTION_ADDR               (DUMP_MEM_BASE+0x200)
#define DUMP_APP_SECTION_SIZE               0x2FE00
#define DUMP_COMM_SECTION_ADDR              (DUMP_MEM_BASE+0x30000)
#define DUMP_COMM_SECTION_SIZE              0x40000
#define DUMP_M3_SECTION_ADDR              (DUMP_MEM_BASE+0x30000+0x40000)
#define DUMP_M3_SECTION_SIZE               0x10000
#define DUMP_FASTBOOT_ADDR                  (DUMP_M3_SECTION_ADDR + DUMP_M3_SECTION_SIZE)
#define DUMP_FASTBOOT_SIZE                   0x100
#define DUMP_EXT_SECTION_ADDR            (DUMP_FASTBOOT_ADDR+ DUMP_FASTBOOT_SIZE)


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif    /* End #define __BSP_DUMP_H__ */




