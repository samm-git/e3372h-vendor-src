/*******************************************************************************
* Copyright (C), 2008-2011, HUAWEI Tech. Co., Ltd.
*
* Module name: Efuse
*
* Description: Burn nand flash info into efuse
*
* Version: v1.0
*
* Filename:    efuse_nandc.c
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
/*lint --e{537}*/
#include <hi_syscrg_interface.h>
#include <hi_efuse.h>
#include "boot/boot.h"
#include "bsp_nandc.h"
#include <bsp_efuse.h>
#include <balongv7r2/efuse_nandc.h>


extern int bsp_get_nand_info(struct nand_spec *spec);

#if defined(CONFIG_EFUSE_NANDC_GROUP2)
/******************************************************************************
* 函 数 名: do_burn
* 功能描述: 将flash spec信息烧写到efuse
* 输入参数: @spec:存放flash info
* 输出参数: 无
* 返 回 值: 0 - 成功； 非0 - 失败
* 函数说明:
*******************************************************************************/
s32 do_burn(u32 value, NANDINFO_GROUP_ID nandc_info_group_id)
{
	s32 ret = ERROR;
    u32 read_value = 0;
    u32 nandc_info_mask = 0;

    if(nandc_info_group_id >= EFUSE_NANDINFO_GROUP_MAX)
    {
		PRINT_ERR("ERROR: invalid group id %d\n", nandc_info_group_id);
        return ERROR;
    }

	/* write info */
	ret = bsp_efuse_write(&value, EFUSE_GROUP_ID_FOR_NANDC, 1); 
	if(ret)
	{
		PRINT_ERR("EFuse write failed, ret = %d\n", ret);
		return ret;
	}

    /* read and compare */
    ret = bsp_efuse_read(&read_value, EFUSE_GROUP_ID_FOR_NANDC, 1); 
    if(ret)
	{
		PRINT_ERR("EFuse read failed, ret = %d\n", ret);
		return ret;
	}

    nandc_info_mask = (EFUSE_NANDINFO_GROUP_0 == nandc_info_group_id) ? NAND_INFO_MASK_GROUP0 : NAND_INFO_MASK_GROUP1;

    if((read_value & nandc_info_mask) != value)
    {
        PRINT_ERR("EFuse value cmp error, write value 0x%x, read value 0x%x, read_value & nandc_info_mask %d\n",
            value, read_value, read_value & nandc_info_mask);
		return ERROR;
    }

    PRINT_MSG("SUCCESS: write nand info successful, efuse value 0x%x\n", value);

	return OK;
}

/******************************************************************************
* 函 数 名: get_nandc_info_from_efuse
* 功能描述: 从efuse中获取nandc info
* 输入参数: @efuse_value - efuse value
            @group_id - 表示获取efuse中第几组nandc info，合法值为0或1
* 输出参数: @nandc_spec - 保存获取到的nandc info
* 返 回 值: 0 - 成功； 非0 - 失败
* 函数说明:
*******************************************************************************/
s32 get_nandc_info_from_efuse(struct nand_spec *spec_real, HI_HW_CFG_T efuse_value, NANDINFO_GROUP_ID group_id)
{
	u32 page_size;
	u32 ecc_type;
	u32 page_num_per_block;
	u32 addr_num;
    
    /* 参数检查 */
    if((!spec_real) || (group_id >= EFUSE_NANDINFO_GROUP_MAX))
    {
		PRINT_ERR("ERROR: invalid argument\n");
        return ERROR;
    }
    
	if(EFUSE_NANDINFO_GROUP_1 == group_id)
	{
		page_size = efuse_value.bits.nf_page_size1;
		ecc_type  = efuse_value.bits.nf_ecc_type1;
		page_num_per_block = efuse_value.bits.nf_block_size1;
		addr_num = efuse_value.bits.nf_addr_num1;
	}
	else /* EFUSE_NANDINFO_GROUP_0 == group_id */
	{
		page_size = efuse_value.bits.nf_page_size;
		ecc_type  = efuse_value.bits.nf_ecc_type;
		page_num_per_block = efuse_value.bits.nf_block_size;
		addr_num = efuse_value.bits.nf_addr_num;
	}


	/* page size */
	switch(page_size)
	{
		case NANDC_PAGE_SIZE_2K_EFUSE:
			spec_real->pagesize = NANDC_SIZE_2K;
			break;
		case NANDC_PAGE_SIZE_4K_EFUSE:
			spec_real->pagesize = NANDC_SIZE_4K;
			break;
		default:
			PRINT_ERR("wrong efuse pagesize: %d\n", page_size);
			return ERROR;
	};

	/* ecc type */
	switch(ecc_type)
	{
		case NANDC_ECCTYPE_ECC_4BIT_EFUSE:
			spec_real->ecctype = NANDC_ECC_4BIT;
			break;
		case NANDC_ECCTYPE_ECC_8BIT_EFUSE:
			spec_real->ecctype = NANDC_ECC_8BIT;
			break;
		default:
			PRINT_ERR("wrong efuse ecctype: %d\n", ecc_type);
			return ERROR;
	};

	/* page num per block */
	switch(page_num_per_block)
	{
		case NANDC_PAGENUM_PER_BLOCK_64_EFUSE:
			spec_real->pagenumperblock = NANDC_BLOCK_64PAGE;
			break;
		case NANDC_PAGENUM_PER_BLOCK_128_EFUSE:
			spec_real->pagenumperblock = NANDC_BLOCK_128PAGE;
			break;
		default:
			PRINT_ERR("wrong efuse pagenum: %d\n", page_num_per_block);
			return ERROR;
	};

	/* addr cycle */
	switch(addr_num)
	{
		case NANDC_ADDRCYCLE_4_EFUSE:
			spec_real->addrnum = NANDC_ADDRCYCLE_4;
			break;
		case NANDC_ADDRCYCLE_5_EFUSE:
			spec_real->addrnum = NANDC_ADDRCYCLE_5;
			break;
		default:
			PRINT_ERR("wrong efuse addrnum: %d\n", addr_num);
			return ERROR;
	};

	/* done */
	return OK;
}
/******************************************************************************
* 函 数 名: set_nandc_info_of_efuse
* 功能描述: 根据nandc_spec设置要烧写到efuse中的value
* 输入参数: @group_id - 表示填充efuse中第几组nandc info，合法值为0或1
            @spec_real - 保存获取到的nandc info
* 输出参数: @efuse_value - efuse value
* 返 回 值: 0 - 成功； 非0 - 失败
* 函数说明:
*******************************************************************************/
s32 set_nandc_info_of_efuse(struct nand_spec *spec_real, HI_HW_CFG_T *efuse_value, NANDINFO_GROUP_ID group_id)
{
	u32 page_size;
	u32 ecc_type;
	u32 page_num_per_block;
	u32 addr_num;

    /* 参数判断 */
    if((!spec_real) || (!efuse_value) || (group_id >= EFUSE_NANDINFO_GROUP_MAX))
    {
		PRINT_ERR("ERROR: invalid argument\n");
        return ERROR;
    }

	/* page size */
	switch(spec_real->pagesize)
	{
		case NANDC_SIZE_2K :
			page_size = NANDC_PAGE_SIZE_2K_EFUSE;
			break;
		case NANDC_SIZE_4K :
			page_size = NANDC_PAGE_SIZE_4K_EFUSE;
			break;
		default:
			PRINT_ERR("err pagesize: %d\n", spec_real->pagesize);
			return ERROR;
	};

	/* ecc type */
	switch(spec_real->ecctype)
	{
		case NANDC_ECC_4BIT:
			ecc_type = NANDC_ECCTYPE_ECC_4BIT_EFUSE;
			break;
		case NANDC_ECC_8BIT:
			ecc_type = NANDC_ECCTYPE_ECC_8BIT_EFUSE;
			break;
		default:
			PRINT_ERR("err ecctype: %d\n", spec_real->ecctype);
			return ERROR;
	};

	/* page num per block */
	switch(spec_real->pagenumperblock)
	{
		case NANDC_BLOCK_64PAGE :
			page_num_per_block = NANDC_PAGENUM_PER_BLOCK_64_EFUSE;
			break;
		case NANDC_BLOCK_128PAGE :
			page_num_per_block = NANDC_PAGENUM_PER_BLOCK_128_EFUSE;
			break;
		default:
			PRINT_ERR("err blocksize: %d\n", spec_real->pagenumperblock);
			return ERROR;
	};

	/* addr cycle */
	switch(spec_real->addrnum)
	{
		case NANDC_ADDRCYCLE_4 :
			addr_num = NANDC_ADDRCYCLE_4_EFUSE;
			break;
		case NANDC_ADDRCYCLE_5 :
			addr_num = NANDC_ADDRCYCLE_5_EFUSE;
			break;
		default:
			PRINT_ERR("err addrnum: %d\n", spec_real->addrnum);
			return ERROR;
	};

	if(EFUSE_NANDINFO_GROUP_1 == group_id)
	{
		efuse_value->bits.nf_page_size1 = page_size;
		efuse_value->bits.nf_ecc_type1 = ecc_type;
		efuse_value->bits.nf_block_size1 = page_num_per_block;
		efuse_value->bits.nf_addr_num1 = addr_num;
        efuse_value->bits.nf_ctrl_ena1 = 1;
	}
	else /* EFUSE_NANDINFO_GROUP_0 == group_id */
	{
		efuse_value->bits.nf_page_size = page_size;
		efuse_value->bits.nf_ecc_type = ecc_type;
		efuse_value->bits.nf_block_size = page_num_per_block;
		efuse_value->bits.nf_addr_num = addr_num;
        efuse_value->bits.nf_ctrl_ena0 = 1;
	}

    /* successed */
    return OK;
}

/******************************************************************************
* 函 数 名: is_nand_info_same
* 功能描述: 比较spec_real保存的nand info和efuse_value中第group_id中保存的是否一致
* 输入参数: @spec_real - 保存nand info
*           @efuse_value - efuse值
*           @group_id - efuse中第几组nand info有效
* 输出参数: @result - 比较结果
* 返 回 值: 0 - 成功； 非0 - 失败
* 函数说明:
*******************************************************************************/
s32 is_nand_info_same(struct nand_spec *spec_real, HI_HW_CFG_T efuse_value, NANDINFO_GROUP_ID group_id, COMPARE_RESULT *result)
{
	struct nand_spec spec_from_efuse;
    s32 ret = OK;

    /* 参数检查 */
    if((!spec_real) || (group_id >= EFUSE_NANDINFO_GROUP_MAX) || (!result))
    {
		PRINT_ERR("ERROR: invalid argument\n");
        return ERROR;
    }
    
    /* 解析第group_id组nandc规格信息 */
    ret = get_nandc_info_from_efuse(&spec_from_efuse, efuse_value, group_id);
    if(ret)
    {
		PRINT_ERR("ERROR:get_nandc_info_from_efuse failed, ret %d\n", ret);
        return ret;
    }

    /* 如果二者一样，不需要烧写，直接返回 */
    if((spec_from_efuse.pagesize == spec_real->pagesize)
        && (spec_from_efuse.addrnum == spec_real->addrnum)
        && (spec_from_efuse.ecctype == spec_real->ecctype)
        && (spec_from_efuse.pagenumperblock == spec_real->pagenumperblock))
    {
		PRINT_MSG("Nand info already saved in efuse, no need to burn, group_id %d\n", group_id);
        *result = RESULT_SAME;
    }
    else
    {
        *result = RESULT_DIFFERENT;
    }

    return OK;
}

/******************************************************************************
* 函 数 名: burn_efuse_nandc
* 功能描述: 将flash spec info烧写进efuse
* 输入参数: 无
* 输出参数: 无
* 返 回 值: 0 - 成功； 非0 - 失败
* 函数说明:
*******************************************************************************/
s32 burn_efuse_nandc(void)
{
	struct nand_spec spec_from_nandc_drv;
	struct nand_spec spec_from_efuse;
    COMPARE_RESULT result = RESULT_DIFFERENT;
    HI_HW_CFG_T efuse_value;
    NANDINFO_GROUP_ID group_id = EFUSE_NANDINFO_GROUP_0;
    s32 ret = OK;

    memset((void *)&spec_from_nandc_drv, 0, sizeof(struct nand_spec));
    memset((void *)&spec_from_efuse, 0, sizeof(struct nand_spec));
    
    /* 读取efuse值 */
	ret = bsp_efuse_read((u32 *)&efuse_value, EFUSE_GROUP_ID_FOR_NANDC, 1);
	if(ret)
	{
		PRINT_ERR("Efuse read info error, ret %d\n", ret);
		return ret;
	}
    
	/* 获取nandc规格信息 */
	ret = bsp_get_nand_info(&spec_from_nandc_drv);
	if(OK != ret)
	{
		PRINT_ERR("Get flash info failed, ret %d\n", ret);
		return ret;
	}    

	PRINT_MSG("page size	:0x%x\n", spec_from_nandc_drv.pagesize);
	PRINT_MSG("page number  :0x%x\n", spec_from_nandc_drv.pagenumperblock);
	PRINT_MSG("addr cycle	:0x%x\n", spec_from_nandc_drv.addrnum);
	PRINT_MSG("ecc  type	:0x%x\n", spec_from_nandc_drv.ecctype);

    /* 两组efuse都被烧写了，则直接返回 */
	if(efuse_value.bits.nf_ctrl_ena1)
	{
        ret = is_nand_info_same(&spec_from_nandc_drv, efuse_value, EFUSE_NANDINFO_GROUP_1, &result);
    	if(ret)
    	{
    		PRINT_ERR("is_nand_info_same error, ret %d\n", ret);
    		return ret;
    	}

        if(RESULT_SAME == result)
        {
            /* 二者一样，返回OK */
    		PRINT_MSG("Nand info already saved(group 1), no need to burn.\n");
            return OK;
        }
        else
        {
            /* 二者不一样，返回ERROR */
    		PRINT_ERR("ERROR: Two groups already burn, no space to use.\n");
            return ERROR;
        }
	}
    
    if(!efuse_value.bits.nf_ctrl_ena0)  /* 两组都没有写 */
    {
        group_id = EFUSE_NANDINFO_GROUP_0;
    }    
    else    /* 只有第0组nand info被烧写 */
    {
        ret = is_nand_info_same(&spec_from_nandc_drv, efuse_value, EFUSE_NANDINFO_GROUP_0, &result);
    	if(ret)
    	{
    		PRINT_ERR("is_nand_info_same error, ret %d\n", ret);
    		return ret;
    	}
        
        /* 二者一样，直接返回 */
        if(RESULT_SAME == result)
        {
    		PRINT_MSG("Nand info already saved(group 0), no need to burn.\n");
            return OK;
        }

        /* 二者不一样，make要烧写efuse的值 */
        group_id = EFUSE_NANDINFO_GROUP_1;
    }
    
    efuse_value.u32 = 0;  /* efuse同一bit不能重复写1，此处要赋值为0 */
    ret = set_nandc_info_of_efuse(&spec_from_nandc_drv, &efuse_value, group_id);
    if(ret)
    {
    	PRINT_ERR("ERROR: set_nandc_info_of_efuse failed, ret = %d\n", ret);
        return ERROR;
    }

	/* 将最终的efuse值烧写进efuse */
	ret = do_burn(efuse_value.u32, group_id);
	if(OK != ret)
	{
		PRINT_ERR("Burn flash info failed, ret = %d\n", ret);
		return ret;
	}
	return OK;
}

#elif defined(CONFIG_EFUSE_NANDC_GROUP1)

/******************************************************************************
* 函 数 名: do_burn
* 功能描述: 将flash spec信息烧写到efuse
* 输入参数: @spec:存放flash info
* 输出参数: 无
* 返 回 值: 0 - 成功； 非0 - 失败
* 函数说明:
*******************************************************************************/
static s32 do_burn(struct nand_spec *spec)
{
	s32 ret = ERROR;
	u32 efuse_nand_info_addr = EFUSE_NANDC_GROUP_OFFSET;
    u32 read_value;
    u32 value = 0;

	/* param error */
	if(!spec)
	{
		PRINT_MSG("Param failed\n");
		goto erro;
	}

	/* make value */
    switch(spec->pagenumperblock)       /* add page num per block */
    {
        case NANDC_BLOCK_64PAGE:
            value |= EFUSE_NANDC_BLOCKSIZE_64PAGE;
            break;
        case NANDC_BLOCK_128PAGE:
            value |= EFUSE_NANDC_BLOCKSIZE_128PAGE;
            break;
        default:
			break;
    }

	PRINT_MSG("Add block size: 0x%x\n", value);

    /*lint -save -e30, -e142*/
	switch(spec->ecctype)         		/* add ecc type */
	{
	    case NANDC_ECC_NONE:
            value |= EFUSE_NANDC_ECCTYPE_NONE;
			break;
		case NANDC_ECC_1BIT:
            value |= EFUSE_NANDC_ECCTYPE_1BIT;
			break;
		case NANDC_ECC_4BIT:
			value |= EFUSE_NANDC_ECCTYPE_4BIT;
			break;
		case NANDC_ECC_8BIT:
			value |= EFUSE_NANDC_ECCTYPE_8BIT;
			break;
		default:
			break;
	}
    /*lint -restore*/

	PRINT_MSG("Add ecc type: 0x%x\n", value);

	switch(spec->pagesize)				/* add page size */
    {
        case NANDC_SIZE_HK:
			value |= EFUSE_NANDC_PAGESIZE_hK;
			break;
		case NANDC_SIZE_2K:
			value |= EFUSE_NANDC_PAGESIZE_2K;
			break;
		case NANDC_SIZE_4K:
			value |= EFUSE_NANDC_PAGESIZE_4K;
			break;
		case NANDC_SIZE_8K:
			value |= EFUSE_NANDC_PAGESIZE_8K;
			break;
		default:
			break;
	}

	PRINT_MSG("Add page size: 0x%x\n", value);

    switch(spec->addrnum)               /* add addr num */
    {
        case NANDC_ADDRCYCLE_4:
            value |= EFUSE_NANDC_ADDRNUM_4CYCLE;
            break;
        case NANDC_ADDRCYCLE_5:
            value |= EFUSE_NANDC_ADDRNUM_5CYCLE;
            break;
        default:
			break;
    }

	PRINT_MSG("Add addr num: 0x%x\n", value);

	/* set burn flag */
	value |= EFUSE_NANDC_BURNFLAG_HADBURN;

	PRINT_MSG("Add burn flag: 0x%x\n", value);

    /* set configure nandc from efuse */
    value |= EFUSE_CONF_FROM_EFUSE;

	PRINT_MSG("Add conf flag: 0x%x\n", value);

	/* write info */
	ret = bsp_efuse_write(&value, efuse_nand_info_addr, EFUSE_NANDC_GROUP_LENGTH);
	if(ret)
	{
		PRINT_MSG("EFuse write failed\n");
		goto erro;
	}

    /* read and compare */
    ret = bsp_efuse_read(&read_value, efuse_nand_info_addr, EFUSE_NANDC_GROUP_LENGTH);
    if(ret)
	{
		PRINT_MSG("EFuse read failed\n");
		goto erro;
	}

    if(read_value != value)
    {
        PRINT_MSG("EFuse value cmp error, write value: 0x%x, read value 0x%x\n", value, read_value);
        ret = ERROR;
		goto erro;
    }

    PRINT_MSG("SUCCESS: write nandc info successful\n");

	return OK;
erro:
	return ret;
}


/******************************************************************************
* 函 数 名: check_flag
* 功能描述: 检查buffer是否已经烧写了flash info
* 输入参数: @buffer: efuse中nandc info所在group的值
* 输出参数: 无
* 返 回 值: 1 - 已烧写；0 - 未烧写
* 函数说明:
*******************************************************************************/
static s32 check_flag(u32 buffer)
{
	if(buffer & EFUSE_NANDC_BURNFLAG_HADBURN)
	{
		return EFUSE_NANDC_HAD_BURN;
	}
	else
	{
		return EFUSE_NANDC_NOT_BURN;
	}
}

/******************************************************************************
* 函 数 名: is_burn
* 功能描述: 检查efuse是否已经烧写了flash info
* 输入参数: @burn_flag: 烧写标志。1 - 已烧写；0 - 未烧写
*			@buffer: efuse中nandc info所在group的值
* 输出参数: 无
* 返 回 值: 0 - 成功； 非0 - 失败
* 函数说明: 检查
*******************************************************************************/
static s32 is_burn(u32 *burn_flag)
{
	s32 ret;
	u32 efuse_nand_info_addr = EFUSE_NANDC_GROUP_OFFSET;
    u32 buffer = 0;

	ret = bsp_efuse_read(&buffer, efuse_nand_info_addr, EFUSE_NANDC_GROUP_LENGTH);
	if(ret)
	{
		PRINT_MSG("Efuse read info error\n");
		goto erro;
	}

	PRINT_MSG("After read: 0x%x\n", buffer);

	ret = check_flag(buffer);
	if(EFUSE_NANDC_NOT_BURN == ret)
	{
		*burn_flag = EFUSE_NANDC_NOT_BURN;
	}
	else
	{
		*burn_flag = EFUSE_NANDC_HAD_BURN;
	}

	return OK;
erro:
	return ret;
}

/******************************************************************************
* 函 数 名: burn_efuse_nandc
* 功能描述: 将flash spec info烧写进efuse
* 输入参数: 无
* 输出参数: 无
* 返 回 值: 0 - 成功； 非0 - 失败
* 函数说明:
*******************************************************************************/
s32 burn_efuse_nandc(void)
{
	s32 ret = ERROR;
	u32 burn_flag = 0;  /* 1 - already burn; 0 - not burn */
	struct nand_spec *spec;

	/* check whether nand info already burn */
    /* coverity[no_effect_test] */
	ret = is_burn(&burn_flag);
	if(ret)
	{
		PRINT_MSG("Check efuse info error\n");
		goto erro;
	}
	if(burn_flag)
	{
		PRINT_MSG("Efuse nandc info already burn\n");
		return OK;
	}

	/* get flash info  */
	spec = (struct nand_spec *)alloc(sizeof(struct nand_spec));
	if(!spec)
	{
		PRINT_MSG("Malloc memory failed\n");
		goto erro;
	}

	ret = bsp_get_nand_info(spec);
	if(OK != ret)
	{
		PRINT_MSG("Get flash info failed\n");
		goto erro;
	}

	PRINT_MSG("After get spec\n");
	PRINT_MSG("spec->pagesize			: 0x%x\n", spec->pagesize);
	PRINT_MSG("spec->pagenumperblock	: 0x%x\n", spec->pagenumperblock);
	PRINT_MSG("spec->addrnum			: 0x%x\n", spec->addrnum);
	PRINT_MSG("spec->ecctype			: 0x%x\n", spec->ecctype);


	/* burn flash info into efuse */
	ret = do_burn(spec);
	if(OK != ret)
	{
		PRINT_MSG("Burn flash info failed\n");
		goto erro;
	}

	return OK;

erro:
	return ret;
}

#else
s32 burn_efuse_nandc(void)
{
	PRINT_MSG("ERROR: no platform found!\n");
    return ERROR;
}

#endif
#ifdef __cplusplus
}
#endif


