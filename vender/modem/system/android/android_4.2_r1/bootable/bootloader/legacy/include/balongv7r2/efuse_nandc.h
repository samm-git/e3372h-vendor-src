/*******************************************************************************
* Copyright (C), 2008-2011, HUAWEI Tech. Co., Ltd.
*
* Module name: Efuse
*
* Description: Burn nand flash info into efuse
*
* Version: v1.0
*
* Filename:    efuse_nandc.h
* Description: Get nand info and burn into efuse
*
* Function List:
*
* History:
* 1.date:2013-06-06
* question number:
* modify reasion:         create
*******************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif

#ifndef __EFUSE_BURN_H__
#define __EFUSE_BURN_H__

#include <types.h>

#define EFUSE_NANDC_DEBUG

#ifdef EFUSE_NANDC_DEBUG
#define PRINT_MSG(fmt,...)  /*lint -save -e515 */ cprintf(fmt, ##__VA_ARGS__) /*lint -restore */ 
#define PRINT_ERR(fmt,...)  /*lint -save -e515 */ cprintf("[EFUSE_NANDC] <%s> "fmt, __FUNCTION__, ##__VA_ARGS__) /*lint -restore */ 
#else
#define PRINT_MSG(fmt,...)
#endif

#if defined(CONFIG_EFUSE_NANDC_GROUP2)

typedef enum
{
    EFUSE_NANDINFO_GROUP_0 = 0,
    EFUSE_NANDINFO_GROUP_1,
    EFUSE_NANDINFO_GROUP_MAX,
}NANDINFO_GROUP_ID;

typedef enum
{
    RESULT_SAME = 0,
    RESULT_DIFFERENT,
}COMPARE_RESULT;


#define NAND_INFO_MASK_GROUP0 0x00770020
#define NAND_INFO_MASK_GROUP1 0x00001FC0

#define NANDC_ECCTYPE_ECC_4BIT		4
#define NANDC_ECCTYPE_ECC_8BIT		8

/* macro for efuse reg bit value for nandc */
#define NANDC_PAGE_SIZE_2K_EFUSE			1
#define NANDC_PAGE_SIZE_4K_EFUSE			3

#define NANDC_ECCTYPE_ECC_4BIT_EFUSE		1
#define NANDC_ECCTYPE_ECC_8BIT_EFUSE		3

#define NANDC_ADDRCYCLE_4_EFUSE				0
#define NANDC_ADDRCYCLE_5_EFUSE				1

#define NANDC_PAGENUM_PER_BLOCK_64_EFUSE  	0
#define NANDC_PAGENUM_PER_BLOCK_128_EFUSE 	1


#elif defined(CONFIG_EFUSE_NANDC_GROUP1)
/**
 * Follow configuration is just for v7r2 e5 dcm.
 */

#define EFUSE_NANDC_GROUP_END_BIT  		511 
#define EFUSE_BITS_NUMBER_PER_GROUP     32
#define EFUSE_NANDC_GROUP_START_BIT 	(EFUSE_NANDC_GROUP_END_BIT - EFUSE_BITS_NUMBER_PER_GROUP + 1) /* 480 */

#define EFUSE_NANDC_GROUP_OFFSET   		(EFUSE_NANDC_GROUP_START_BIT / EFUSE_BITS_NUMBER_PER_GROUP)
#define EFUSE_NANDC_GROUP_LENGTH   		((EFUSE_NANDC_GROUP_END_BIT - EFUSE_NANDC_GROUP_START_BIT + 1) / EFUSE_BITS_NUMBER_PER_GROUP)    			/* 1 group */

#define EFUSE_NANDC_BLOCKSIZE_START_BIT 496
#define EFUSE_NANDC_BLOCKSIZE_SHIFT 	(EFUSE_NANDC_BLOCKSIZE_START_BIT - EFUSE_NANDC_GROUP_START_BIT)
#define EFUSE_NANDC_BLOCKSIZE_64PAGE    (0 << EFUSE_NANDC_BLOCKSIZE_SHIFT)
#define EFUSE_NANDC_BLOCKSIZE_128PAGE   (1 << EFUSE_NANDC_BLOCKSIZE_SHIFT)

#define EFUSE_NANDC_ECCTYPE_START_BIT 	497 
#define EFUSE_NANDC_ECCTYPE_SHIFT		(EFUSE_NANDC_ECCTYPE_START_BIT - EFUSE_NANDC_GROUP_START_BIT)
#define EFUSE_NANDC_ECCTYPE_NONE	    (0 << EFUSE_NANDC_ECCTYPE_SHIFT)
#define EFUSE_NANDC_ECCTYPE_1BIT	    (1 << EFUSE_NANDC_ECCTYPE_SHIFT)
#define EFUSE_NANDC_ECCTYPE_4BIT	    (1 << EFUSE_NANDC_ECCTYPE_SHIFT) 
#define EFUSE_NANDC_ECCTYPE_8BIT		(3 << EFUSE_NANDC_ECCTYPE_SHIFT)

#define EFUSE_NANDC_PAGESIZE_START_BIT 	 500
#define EFUSE_NANDC_PAGESIZE_SHIFT		(EFUSE_NANDC_PAGESIZE_START_BIT - EFUSE_NANDC_GROUP_START_BIT)
#define EFUSE_NANDC_PAGESIZE_hK			(0 << EFUSE_NANDC_PAGESIZE_SHIFT)
#define EFUSE_NANDC_PAGESIZE_2K			(1 << EFUSE_NANDC_PAGESIZE_SHIFT)
#define EFUSE_NANDC_PAGESIZE_4K			(3 << EFUSE_NANDC_PAGESIZE_SHIFT) 
#define EFUSE_NANDC_PAGESIZE_8K			(2 << EFUSE_NANDC_PAGESIZE_SHIFT) 

#define EFUSE_NANDC_ADDRNUM_START_BIT 	502 
#define EFUSE_NANDC_ADDRNUM_SHIFT		(EFUSE_NANDC_ADDRNUM_START_BIT - EFUSE_NANDC_GROUP_START_BIT)
#define EFUSE_NANDC_ADDRNUM_4CYCLE		(0 << EFUSE_NANDC_ADDRNUM_SHIFT)
#define EFUSE_NANDC_ADDRNUM_5CYCLE		(1 << EFUSE_NANDC_ADDRNUM_SHIFT)

#define EFUSE_CONF_FLAG_BIT             505 
#define EFUSE_CONF_FLAG_SHIFT           (EFUSE_CONF_FLAG_BIT - EFUSE_NANDC_GROUP_START_BIT)
#define EFUSE_CONF_FROM_EFUSE           (1 << EFUSE_CONF_FLAG_SHIFT)
#define EFUSE_CONF_FROM_PIN             (0 << EFUSE_CONF_FLAG_SHIFT)

#define EFUSE_NANDC_BURNFLAG_BIT	    EFUSE_CONF_FLAG_BIT 
#define EFUSE_NANDC_BURNFLAG_SHIFT		(EFUSE_NANDC_BURNFLAG_BIT - EFUSE_NANDC_GROUP_START_BIT)
#define EFUSE_NANDC_BURNFLAG_NOTBURN	EFUSE_CONF_FROM_PIN
#define EFUSE_NANDC_BURNFLAG_HADBURN	EFUSE_CONF_FROM_EFUSE
/* end check position */

#define EFUSE_NANDC_HAD_BURN            1
#define EFUSE_NANDC_NOT_BURN            0
#else
/* no platform */
#endif
s32 burn_efuse_nandc(void);

#endif /* __EFUSE_BURN_H__ */

#ifdef __cplusplus
}
#endif
