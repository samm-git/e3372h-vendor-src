 /*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
 * foss@huawei.com
 *
 * If distributed as part of the Linux kernel, the following license terms
 * apply:
 *
 * * This program is free software; you can redistribute it and/or modify
 * * it under the terms of the GNU General Public License version 2 and 
 * * only version 2 as published by the Free Software Foundation.
 * *
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * * GNU General Public License for more details.
 * *
 * * You should have received a copy of the GNU General Public License
 * * along with this program; if not, write to the Free Software
 * * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Otherwise, the following license terms apply:
 *
 * * Redistribution and use in source and binary forms, with or without
 * * modification, are permitted provided that the following conditions
 * * are met:
 * * 1) Redistributions of source code must retain the above copyright
 * *    notice, this list of conditions and the following disclaimer.
 * * 2) Redistributions in binary form must reproduce the above copyright
 * *    notice, this list of conditions and the following disclaimer in the
 * *    documentation and/or other materials provided with the distribution.
 * * 3) Neither the name of Huawei nor the names of its contributors may 
 * *    be used to endorse or promote products derived from this software 
 * *    without specific prior written permission.
 * 
 * * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */
/*lint --e{537}*/
#ifdef __VXWORKS__
#include <string.h>
#include <stdio.h>
#endif
#include "product_config.h"

#ifdef __KERNEL__
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/ctype.h>

#if (FEATURE_ON == MBB_DLOAD)
#include <linux/delay.h>
#include <linux/sched.h>
#endif /* MBB_DLOAD */

#endif

#include "hi_base.h"
#include "hi_syssc.h"
#include "hi_syscrg.h"

#include "osl_types.h"
#include "osl_bio.h"

#include "bsp_version.h"
#include "drv_version.h"
#include "bsp_memmap.h"
#include "drv_dload.h"
#include "bsp_wdt.h"
#include "drv_onoff.h"
#include "hi_syssc_interface.h"
#ifdef __VXWORKS__
#include "bbp_wcdma_interface.h"
#include "gbbp_interface.h"
#endif

/*BEGIN DTS2013080708856 W00183456 20130807 added*/
#include "bsp_sram.h"
/*END DTS2013080708856 W00183456 20130807 added*/
#include "drv_onoff.h"

/******************************************************************************************************
     问题单号                       时间                   修改人                        说明
DTS2013090310311             2013-09-04          wangli 00249966   增加at^dloadinfo?命令从OEMINFO分区读取ISO
DTS2013111809248             2013-12-10          wangli 00249966   清零编译告警
DTS2013103008391             2014-01-02          miaoshiyang 0023250  升级阶段流量清空处理
DTS2014012403055             2014-01-24          yaozhanwei 00216577  AT^PRODTYPE命令根据硬件HKADC规划调整
DTS2014072905310             2014-07-29          修改各个产品形态的at^prodtype?的返回值
DTS2014080805340             2014-08-12          liuqiang 00204556           修改AT^HWVER返回值不带子版本号
DTS2014082900291             2014-08-29          zhangxiangzhuang 00278248          使用NV50018进行软件版本号定制
DTS2014082602217             2014-09-10          c00227249          增加升级信息AT查询与设置
DTS2014091204377             2014-09-10          c00227249          AT^GODLOAD 重复发送，会创建多次工作队列，导致死机
DTS2014091603716             2014-09-16          x00205748          支持采用海思射频开关的新CPE单板
*****************************************************************************************************/


#ifndef OK
#define OK 0
#endif
#ifndef ERROR
#define ERROR  (-1)
#endif

#ifdef __KERNEL__
#if (FEATURE_ON == MBB_DLOAD)
/* set 2.5s delay before restart */
static struct delayed_work restart_process;
static bool restart_workqueue_create_flag = false;
static struct workqueue_struct* restart_workqueue = NULL;
#define HUAWEI_UPDATE_START_DELAY_TIME  2500
#endif /* MBB_DLOAD */
#endif


/* 通过HKADC标示的产品形态，(0:E5,1:stick,2:Phone,3:pad,4:cpe,5:mod,6~9:resv)*/
enum hkac_product_type
{
	HKADC_PRODUCT_TYPE_E5 = 0,
	HKADC_PRODUCT_TYPE_STICK,
	HKADC_PRODUCT_TYPE_PHONE,
	HKADC_PRODUCT_TYPE_PAD,
	HKADC_PRODUCT_TYPE_CPE,
	HKADC_PRODUCT_TYPE_MOD,
    HKADC_PRODUCT_TYPE_CPE_EXT = 8,
};


/*****************************************************************************
*
*兼容以的接口
*
*****************************************************************************/
#ifdef __VXWORKS__
VERSIONINFO  verCheckInfo[ VER_INFO_NUM ];

/*****************************************************************************
 函 数 名  : bsp_version_memversion_ctrl
 功能描述  : 组件版本读写接口。
 输入参数  : pcData：当Mode为读的时候，为调用者待保存返回的组件版本信息的内存地址；
                     当Mode为写的时候，为调用者准备写入的组件版本信息的地址。
             ucLength：当Mode为读的时候，为调用者待保存返回的组件版本信息的内存大小；
                       当Mode为写的时候，为调用者准备写入的组件版本信息的字符数（不包括'\0'）。
             ucType：版本信息ID
             ucMode：0：读取指定ID的组件版本信息；1：写入指定ID的组件版本信息。
 输出参数  : 无。
 返 回 值  : 0:  操作成功；
             -1：操作失败。
*****************************************************************************/
/*lint -save -e958*/
int bsp_version_memversion_ctrl(char *pcData, unsigned char ucLength, COMP_TYPE_I ucType, unsigned char ucMode)
/*lint -save restore*/
{
	/*lint -save -e958*/
	u8 *lpv_VerTmp;
    u8 liv_Index;
    u8 ucLen;
	/*lint -save restore*/
    if(ucType >= VER_INFO_NUM)
    {
        ver_print_error("Version Info Type Error\n");
        return ERROR;
    }

    if (ucMode > VERIONWRITEMODE)
    {
        ver_print_error("Control Mode Error\n");
        return ERROR;
    }

    ucLen = ucLength;
    if((VERIONWRITEMODE == ucMode)&&(ucLen > VER_MAX_LENGTH))
    {
        ver_print_error("Version Info Length Error\n");
        ucLen = VER_MAX_LENGTH;
    }

    if((VERIONWRITEMODE == ucMode)&&(0 == ucLen))
    {
        /*lint -save -e18 -e718 -e746*/
        ucLen = (u8)(strlen((const char *)pcData)+1);
        /*lint -restore*/
        if(ucLen >= VER_MAX_LENGTH)
        {
            ucLen = VER_MAX_LENGTH;
        }
    }

    if(NULL == pcData)
    {
        ver_print_error("Data Buffer Pointer Null\n");
        return ERROR;
    }

    lpv_VerTmp = (u8 *)verCheckInfo[ucType].CompVer;
    for(liv_Index = 0; liv_Index < ucLen; liv_Index++)
    {
        if(ucMode)
        {
            *(lpv_VerTmp + liv_Index) = *((u8 *)pcData + liv_Index);
        }
        else
        {
            *((u8 *)pcData + liv_Index) = *(lpv_VerTmp + liv_Index);
        }
    }

    return  OK;
}


/********************************************************************************************************************
 函 数 名  : bsp_version_init
 功能描述  :
 输出参数  :
 返 回 值  :
 注意事项  ：
********************************************************************************************************************/
void bsp_version_init(void)
{
    u32 i;
    /*BEGIN DTS2013092201594 yaozhanwei 2013-05-25 added for Product Out-Name*/
#if (FEATURE_ON == MBB_COMMON)
    BSP_CHAR *pProductName = NULL;
#endif
    /*END DTS2013092201594 yaozhanwei 2013-05-25 added for Product Out-Name*/
    
    /*init*/
    memset((void *)verCheckInfo, 0, sizeof (verCheckInfo));
    for( i = 0; i < VER_INFO_NUM; i++)
    {
        verCheckInfo[i].CompId = (u8)i;
    }

    bsp_version_memversion_ctrl((char *)PRODUCT_VERSION_STR, 0 , VER_VXWORKS,(unsigned char)VERIONWRITEMODE);

    /*BEGIN DTS2013092201594 yaozhanwei 2013-05-25 modified for Product Out-Name*/
#if (FEATURE_ON == MBB_COMMON)
    /*Read Product ID version infomation*/
    pProductName = (BSP_CHAR *)bsp_version_get_product_out_name();
    if(NULL == pProductName)
    {
        bsp_version_memversion_ctrl((char *)PRODUCT_VERSION_STR ,0 , VER_PRODUCT_ID,(unsigned char)VERIONWRITEMODE);
    }
    else
    {
        bsp_version_memversion_ctrl(pProductName , 0,
                                (unsigned char)VER_PRODUCT_ID,
                                (unsigned char)VERIONWRITEMODE);
    }
#else
    /*Read Product ID version infomation*/
    bsp_version_memversion_ctrl((char *)PRODUCT_VERSION_STR ,0 , VER_PRODUCT_ID,(unsigned char)VERIONWRITEMODE);
#endif
    /*END DTS2013092201594 yaozhanwei 2013-05-25 modified for Product Out-Name*/

#if (FEATURE_ON == MBB_COMMON)
    BSP_CHAR *pSoftwareVer = NULL;
    pSoftwareVer = bsp_version_get_firmware();

    if(NULL == pSoftwareVer)
    {
        bsp_version_memversion_ctrl((char *)PRODUCT_DLOAD_SOFTWARE_VER, 0, VER_SOFTWARE,(unsigned char)VERIONWRITEMODE);
    }
    else
    {
        bsp_version_memversion_ctrl((char *)pSoftwareVer, 0, VER_SOFTWARE,(unsigned char)VERIONWRITEMODE);
    }
#else
    /*Read product version infomation*/
    bsp_version_memversion_ctrl((char *)PRODUCT_DLOAD_SOFTWARE_VER, 0, VER_SOFTWARE,(unsigned char)VERIONWRITEMODE);
#endif

    /*Read hardware version infomation*/
	if(NULL != bsp_version_get_hardware())
	{
		bsp_version_memversion_ctrl((char *)bsp_version_get_hardware(), 0, VER_HARDWARE,(unsigned char)VERIONWRITEMODE);
	}
    #if (FEATURE_ON == MBB_COMMON)
    /*Read hardware version without subversion infomation*/
    if(NULL != bsp_version_get_hardware_no_subver())
    {
        bsp_version_memversion_ctrl(
           (char *)bsp_version_get_hardware_no_subver(), 0, VER_HARDWARE_NO_SUB,(unsigned char)VERIONWRITEMODE);
    }
    #endif
}

/*****************************************************************************
 函 数 名  : bsp_version_get_verall
 功能描述  : 查询所有组件的版本号。
 输入参数  : ppVersionInfo：待保存的版本信息地址。
 输出参数  : ucLength：待返回的数据的字节数。
 返 回 值  : 0:  操作成功；
             -1：操作失败。
*****************************************************************************/
int bsp_version_get_verall (void ** ppVersionInfo, unsigned int * ulLength)
{
    if ((NULL == ppVersionInfo) || (NULL == ulLength))
    {
        return ERROR;
    }

    *ppVersionInfo = verCheckInfo;
    *ulLength = sizeof(verCheckInfo);
    return OK;
}

/*****************************************************************************
 函 数 名  : bsp_version_write_chip_version
 功能描述  : 将芯片版本号写入全局变量中
 输入参数  : 无
 输出参数  : none
 返 回 值  : void
*****************************************************************************/

static void bsp_version_write_chip_version(u32 addr,COMP_TYPE_I comp_type)
{
	u8 soc_version_info[VER_MAX_LENGTH] = {0};
	s32 ret=OK;

	/* coverity[secure_coding] */
	sprintf((char *)soc_version_info,"%#x",addr);

	ret = bsp_version_memversion_ctrl((char*)soc_version_info,
                    VER_MAX_LENGTH, comp_type, VERIONWRITEMODE);
    if( OK != ret )
    {
        ver_print_error(" Write %d Version Info fail. \n", comp_type);
    }

}

/*****************************************************************************
 函 数 名  : BSP_InitPlatformVerInfo
 功能描述  : 初始化芯片的版本号
 输入参数  : 无
 输出参数  : none
 返 回 值  : void
*****************************************************************************/
void BSP_InitPlatformVerInfo(void)
{
	u32 addr;
	u32 product_type=0xff;
	COMP_TYPE_I comp_type;

	/*获取硬件版本号*/
	product_type = bsp_version_get_board_type();
	if(HW_VER_INVALID==product_type){
		ver_print_error("get board chip type fail!\n");
	}
	if(HW_VER_PRODUCT_P531_FPGA == product_type){
		comp_type=VER_SOC;
	}
	else{
		comp_type=VER_ASIC;
	}
	addr=get_hi_version_id_version_id();
	bsp_version_write_chip_version(addr,comp_type);


	if(HW_VER_PRODUCT_P531_FPGA == product_type||HW_VER_PRODUCT_UDP == product_type||HW_VER_K3V3_UDP == product_type){

		bsp_version_write_chip_version(HI_GBBP_REG_BASE_ADDR_VIRT+HAL_GBBP_VERSION_ADDR,VER_GBBP);

		bsp_version_write_chip_version(HI_WBBP_REG_BASE_ADDR_VIRT+(WBBP_VERSION_ADDR-SOC_BBP_WCDMA_BASE_ADDR),VER_WBBP);
	}
   return;
}

#endif

int BSP_GetProductName (char * pProductName, unsigned int ulLength)
{
	static bool b_geted=false;
	static char* pout_name=NULL;

	if((BSP_NULL == pProductName) || (0 == ulLength))
	{
		return ERROR;
	}

	if(!b_geted){
		b_geted=true;
		pout_name = bsp_version_get_product_out_name();
	}

	if(!pout_name)
	{
		ver_print_error("get product out name failed !\n");
		return ERROR;
	}

    /*lint -save -e732  -e713*/
	strncpy(pProductName, pout_name, StrParamType(ulLength));
    /*lint -restore*/
	return OK;
}

/*****************************************************************************
* 函数	: bsp_get_board_mode_type
* 功能	: get board type(GUL or LTE)
* 输入	: void
* 输出	: void
* 返回	: BOARD_TYPE_E
*****************************************************************************/
BOARD_TYPE_E bsp_get_board_mode_type(void)
{
#if(defined(FEATURE_MULTIMODE_GUL))
	return BOARD_TYPE_GUL;
#else
	return BOARD_TYPE_LTE_ONLY;
#endif
}

bool bsp_board_type_is_gutl(void)
{
	return (BOARD_TYPE_GUL == bsp_get_board_mode_type() ? (bool)1 : (bool)0);
}

BSP_S32 BSP_DLOAD_GetISOVer(char *pVersionInfo,BSP_U32 ulLength)
{
	static bool b_geted = false;
/* Begin DTS2013090310311 wangli 2013-09-04 modified */
#if (FEATURE_ON == MBB_DLOAD)
    static char iso_ver[VERSION_ISO_MAX_LEN];
#else
    static char* iso_ver=NULL;
#endif 

#if (FEATURE_ON == MBB_DLOAD)
if(VERSION_ISO_MAX_LEN>ulLength||NULL==pVersionInfo)
#else
    if(VERSION_MAX_LEN>ulLength||NULL==pVersionInfo)
#endif
/* End DTS2013090310311 wangli 2013-09-04 modified */
    {
		ver_print_error("pVersionInfo = %d, ulLength = %d error.\n",(int)pVersionInfo,(int)ulLength);
		return ERROR;
	}

	if(!b_geted){
		b_geted=true;
/* Begin DTS2013090310311 wangli 2013-09-04 modified */
#if (FEATURE_ON == MBB_DLOAD)
        bsp_get_iso_version(iso_ver, VERSION_ISO_MAX_LEN);
#else
        iso_ver = bsp_version_get_iso();
#endif
/* End DTS2013090310311 wangli 2013-09-04 modified */
	}

	if(!iso_ver)
	{
		ver_print_error("get iso version fail.\n");
		return ERROR;
	}

    /*lint -save -e732 -e713 */
	strncpy(pVersionInfo,iso_ver,StrParamType(ulLength));
    /*lint -restore*/
	return OK;
}

/*****************************************************************************
 函 数 名  : BSP_MNTN_GetProductIdInter
 功能描述  : 产品名称完整版本读接口。
 输入参数  : pProductIdInter：为调用者待保存返回的产品名称完整版本的内存首地址；
             ulLength       ：为调用者待保存返回的产品名称完整版本的内存大小；
 输出参数  : 无。
 返 回 值  : 0:  操作成功；
             -1：操作失败。
*****************************************************************************/
int	BSP_MNTN_GetProductIdInter(char * pProductIdInter, unsigned int ulLength)
{
	static bool b_geted=false;
	static char* inner_name=NULL;

	if(!b_geted){
		b_geted=true;
		inner_name = bsp_version_get_product_inner_name();
	}

	if(!inner_name)
	{
		ver_print_error("get product inner name failed !\n");
		return ERROR;
	}

    /*lint -save -e732 -e713*/
	strncpy(pProductIdInter,inner_name,StrParamType(ulLength));
    /*lint -restore*/
	return OK;
}

/*****************************************************************************
* 函 数 名  : BSP_GetProductInnerName
* 功能描述  : 获取内部产品名称
* 输入参数  : char* pProductIdInter,    字符串指针
*             BSP_U32 ulLength,         缓冲区长度
* 输出参数  : 无
* 返 回 值  : 0：正确，非0: 失败
* 修改记录  :
*****************************************************************************/
BSP_S32 BSP_GetProductInnerName (char * pProductIdInter, unsigned int ulLength)
{

	static bool b_geted=false;
	static char* inner_name=NULL;

	if(!b_geted){
		b_geted=true;
		inner_name = bsp_version_get_product_inner_name();
	}

	if(!inner_name)
	{
		ver_print_error("get product inner name failed !\n");
		return ERROR;
	}

    /*lint -save -e732 -e713*/
	strncpy(pProductIdInter,inner_name,StrParamType(ulLength));
    /*lint -restore*/
	return OK;
}
/*****************************************************************************
* 函 数 名  : BSP_HwGetHwVersion
* 功能描述  : 获取硬件版本名称
* 输入参数  : char* pHwVersion,字符串指针，保证不小于32字节
* 输出参数  : 无
* 返 回 值  : 无
* 修改记录  :
*****************************************************************************/
BSP_S32 BSP_HwGetHwVersion (char* pFullHwVersion, BSP_U32 ulLength)
{
	static bool b_geted=false;
	static char* hw_version=NULL;

	if(!b_geted){
		b_geted=true;
		hw_version = bsp_version_get_hardware();
	}

	if(!hw_version)
	{
		ver_print_error("get product hardware version failed !\n");
		return ERROR;
	}

    /*lint -save -e732 -e713*/
	strncpy(pFullHwVersion,hw_version,StrParamType(ulLength));
    /*lint -restore*/
	return OK;
}

/*****************************************************************************
 函 数 名  : bsp_version_get_platform_info
 功能描述  : 获取芯片的版本号
 输入参数  : 无
 输出参数  : Platform_info:芯片的版本号
 返 回 值  : void
*****************************************************************************/
static void bsp_version_get_platform_info(unsigned long *platform_info)
{
	/*board FPGA*/
	u32 product_type = 0xff;
	product_type = bsp_version_get_board_type();
	if(HW_VER_INVALID==product_type){
		ver_print_error("get board chip type fail!\n");
		return ;
	}

	switch(product_type){
	case HW_VER_PRODUCT_P531_FPGA:
	case HW_VER_K3V3_FPGA:
		*platform_info=VER_SOC;
		break;
	default:
		*platform_info=VER_ASIC;
	}

}
/*****************************************************************************
 函 数 名  : DRV_GET_PLATFORM_INFO
 功能描述  : 获取芯片的版本号
 输入参数  : 无
 输出参数  : u32PlatformInfo:芯片的版本号
 返 回 值  : void
*****************************************************************************/
void DRV_GET_PLATFORM_INFO(unsigned long *u32PlatformInfo)
{
	static bool b_geted=false;
	static unsigned long platform_info = 0xffffffff;

	if(!b_geted){
		b_geted=true;
		bsp_version_get_platform_info(&platform_info);
	}

	*u32PlatformInfo = platform_info;
	return ;
}

/*****************************************************************************
* 函 数 名  : BSP_OM_GetChipType
*
* 功能描述  : 获取芯片类型
*
* 输入参数  : void
*
* 输出参数  : 无
*
* 返 回 值  : 芯片类型
*             PV500_CHIP:PV500芯片
*             V7R1_CHIP: V7R1芯片
*
* 其它说明  : 无
*
*****************************************************************************/
BSP_CHIP_TYPE_E BSP_OM_GetChipType(void)
{
	return V7R2_CHIP;
}

/*************************************************
 函 数 名   : BSP_MNTN_ProductTypeGet
 功能描述: 返回当前产品类型
 输入参数: 无
 输出参数: 无
 返 回 值   :
 		0:STICK
         1:MOD
         2:E5
         3:CPE
         4:HILINK
         5:WINGLE
*************************************************/
BSP_U32 BSP_MNTN_ProductTypeGet(void)
{
    unsigned int id = 0xffffffff;
	eProductType product_type = PRODUCT_TYPE_STICK;	/* default to stick */

	id = (unsigned int)bsp_version_get_boardid();

	if(HW_VER_INVALID==id){
		ver_print_error("[bsp_get_board_chip_type]:get hw version failed!hw_ver:0x%x\n",id);
		return product_type;
	}

	/* UDP默认返回stick形态 */
	if(HW_VER_UDP_MASK == (id & HW_VER_UDP_MASK))
	{
		return product_type;
	}
    /*BEGIN DTS2013092201594 yaozhanwei 2013-05-25 modified for product type*/
	/* 其他产品版，根据hakdc获取的形态返回 */
	switch((enum hkac_product_type)((id & HW_VER_PRODUCT_TYPE_MASK) >> HW_VER_PRODUCT_TYPE_OS))
	{
    case HKADC_PRODUCT_TYPE_E5:
		product_type = PRODUCT_TYPE_E5;
		break;

    case HKADC_PRODUCT_TYPE_STICK:
#if (FEATURE_ON == MBB_COMMON)
#if defined(BSP_CONFIG_BOARD_STICK)
        product_type = PRODUCT_TYPE_STICK;
        break;
#endif
#if defined(BSP_CONFIG_BOARD_HILINK)
        product_type = PRODUCT_TYPE_HILINK;
        break;
#endif
#if defined(BSP_CONFIG_BOARD_WINGLE)
        product_type = PRODUCT_TYPE_WINGLE;
        break;
#endif
#else
        product_type = PRODUCT_TYPE_STICK;
        break;

	case HKADC_PRODUCT_TYPE_PHONE:
		product_type = PRODUCT_TYPE_PHONE;
		break;

	case HKADC_PRODUCT_TYPE_PAD:
		product_type = PRODUCT_TYPE_PAD;
		break;
#endif
	case HKADC_PRODUCT_TYPE_MOD:
		product_type = PRODUCT_TYPE_MOD;
		break;

	case HKADC_PRODUCT_TYPE_CPE: /* fall through */
    case HKADC_PRODUCT_TYPE_CPE_EXT:
		product_type = PRODUCT_TYPE_CPE;
		break;

	default:/* default to stick */
		product_type = PRODUCT_TYPE_STICK;
		break;
	}

	return (BSP_U32)product_type;
}

/*****************************************************************************
 函 数 名  : BSP_MNTN_GetHwGpioInfo
 功能描述  : Get flash infomation
 输入参数  : 无。
 输出参数  : 无。
 返 回 值  : 无。
*****************************************************************************/
int BSP_MNTN_GetHwGpioInfo(unsigned char *pGpioInfo, unsigned long usLength )
{
	///*lint -save -e438 -e830*/
	//pGpioInfo = pGpioInfo;
	//usLength = usLength;
	///*lint -restore*/
    return 0;
}

/***********************************************************************/

#ifdef __KERNEL__

/*****************************************************************************
* 函 数 名  : BSP_GetBuildVersion
*
* 功能描述  :
*
* 输入参数  :
* 输出参数  : 无
*
* 返 回 值  : 无
*
* 修改记录  : 2011-3-29 wuzechun creat
*
*****************************************************************************/
UE_SW_BUILD_VER_INFO_STRU* BSP_GetBuildVersion(void)
{
#define MAX_VER_SECTION 8
#define VER_PART_LEN 3
#define VERC_PART_LEN 2

    static UE_SW_BUILD_VER_INFO_STRU st = {0,0,0,0,0,0,0,{0},{0}};
	static bool binited = false;
	int ret;

    char * pCur, * pFind;
    char au8Args[MAX_VER_SECTION] = {0};
	u32 verno = 0;

	if(!binited)
	{
		pCur = PRODUCT_VERSION_STR;
	    pFind = pCur;

        memset(&st, 0, sizeof(UE_SW_BUILD_VER_INFO_STRU));

	    while (NULL != (pCur = strchr(pCur, 'V')) )
	    {
	        pCur++;

	        if (isdigit(*(pCur)))
	        {
	            memset(au8Args, 0, MAX_VER_SECTION);
	            strncpy(au8Args, pCur, VER_PART_LEN);
				ret = strict_strtoul(au8Args, 10, (unsigned long *)&verno);
				if(ret)
				{
					ver_print_error("kstrtouint error:ret %#x, au8Args:%s\n", ret, au8Args);
					return (&st);
				}
	            st.ulVVerNo = (u16)verno;
	            break;
	        }
	    }
	    pCur = pFind;
	    while (NULL != (pCur = strchr(pFind, 'R')) )
	    {
	        pCur++;

	        if (isdigit(*(pCur-2)) && isdigit(*(pCur)))
	        {
	            memset(au8Args, 0, MAX_VER_SECTION);
	            strncpy(au8Args, pCur, VER_PART_LEN);
				ret = strict_strtoul(au8Args, 10, (unsigned long *)&verno);
				if(ret)
				{
					ver_print_error("kstrtouint error:ret %#x, au8Args:%s\n", ret, au8Args);
					return (&st);
				}
	            st.ulRVerNo = (u16)verno;
	            break;
	        }
	    }
	    pCur = pFind;
	    while (NULL != (pCur = strchr(pFind, 'C')) )
	    {
	        pCur++;

	        if (isdigit(*(pCur-2)) && isdigit(*(pCur)))
	        {
	            memset(au8Args, 0, MAX_VER_SECTION);
	            strncpy(au8Args, pCur, VERC_PART_LEN);
	            pCur += VERC_PART_LEN;
				ret = strict_strtoul(au8Args, 10, (unsigned long *)&verno);
				if(ret)
				{
					ver_print_error("kstrtouint error:ret %#x, au8Args:%s\n", ret, au8Args);
					return (&st);
				}
	            st.ulCVerNo = (u16)verno;
	            break;
	        }
	    }
	    pCur = pFind;
	    while (NULL != (pCur = strchr(pFind, 'B')) )
	    {
	        pCur++;

	        if (isdigit(*(pCur-2)) && isdigit(*(pCur)))
	        {
	            memset(au8Args, 0, MAX_VER_SECTION);
	            strncpy(au8Args, pCur, VER_PART_LEN);
				ret = strict_strtoul(au8Args, 10, (unsigned long *)&verno);
				if(ret)
				{
					ver_print_error("kstrtouint error:ret %#x, au8Args:%s\n", ret, au8Args);
					return (&st);
				}
	            st.ulBVerNo = (u16)verno;
	            break;
	        }
	    }
	    pCur = pFind;
	    while (NULL != (pCur = strstr(pFind, "SPC")) )
	    {
	        pCur+=3;

	        if (isdigit(*(pCur-4)) && isdigit(*(pCur)))
	        {
	            memset(au8Args, 0, MAX_VER_SECTION);
	            strncpy(au8Args, pCur, VER_PART_LEN);
				ret = strict_strtoul(au8Args, 10, (unsigned long *)&verno);
				if(ret)
				{
					ver_print_error("kstrtouint error:ret %#x, au8Args:%s\n", ret, au8Args);
					return (&st);
				}
	            st.ulSpcNo = (u16)verno;
	            break;
	        }
	    }

		st.ulProductNo = BSP_MNTN_ProductTypeGet();

        /* coverity[buffer_size_warning] */
		strncpy(st.acBuildDate, __DATE__, BUILD_DATE_LEN);

        /* coverity[buffer_size_warning] */
		strncpy(st.acBuildTime, __TIME__, BUILD_TIME_LEN);
	}

	return (&st);
}

/*****************************************************************************
* 函 数 名  : BSP_DLOAD_GetCurMode
*
* 功能描述  : 设置当前工作模式
*
* 输入参数  : void
* 输出参数  : DLOAD_MODE_NORMAL     :正常模式
*             DLOAD_MODE_DATA       :数据模式
*             DLOAD_MODE_DOWNLOAD   :下载模式
*
* 返 回 值  : 无
*
* 其它说明  : AT模块调用
*             正常模式支持的有:
*                 BSP_DLOAD_GetCurMode(DLOAD_MODE_DOWNLOAD)
*             下载模式支持的有:
*                 BSP_DLOAD_GetCurMode(DLOAD_MODE_DATA)
*
*****************************************************************************/
void BSP_DLOAD_SetCurMode(DLOAD_MODE_E eDloadMode)
{
	/*先打桩，后面产品线实现*/
	return ;
}

/*****************************************************************************
* 函 数 名  : BSP_DLOAD_GetCurMode
*
* 功能描述  : 获取当前工作模式
*
* 输入参数  : void
* 输出参数  : 无
*
* 返 回 值  : DLOAD_MODE_NORMAL     :正常模式
*             DLOAD_MODE_DATA       :数据模式
*             DLOAD_MODE_DOWNLOAD   :下载模式
*
* 其它说明  : 无
*
*****************************************************************************/
DLOAD_MODE_E BSP_DLOAD_GetCurMode(void)
{
	/*暂时打桩，由产品线实现*/
/*BEGIN_DTS2013082809601_s00184745_2013-08-28_modified*/
#if (FEATURE_ON == MBB_DLOAD)
    return DLOAD_MODE_NORMAL;
#else
    return DLOAD_MODE_MAX;
#endif
/*END_DTS2013082809601_s00184745_2013-08-28_modified*/
}
/*****************************************************************************
* 函 数 名  : BSP_DLOAD_SetSoftLoad
*
* 功能描述  : 设置启动标志
*
* 输入参数  : BSP_BOOL bSoftLoad  :
*             BSP_FALSE :从vxWorks启动
*             BSP_TRUE  :从bootrom启动
* 输出参数  : 无
*
* 返 回 值  : 无
*
* 其它说明  : 无
*
*****************************************************************************/
void BSP_DLOAD_SetSoftLoad (BSP_BOOL bSoftLoad)
{
/*BEGIN DTS2013080708856 W00183456 20130807 Modified*/
#if (FEATURE_ON == MBB_DLOAD)
    huawei_smem_info *smem_data = NULL;
    smem_data = (huawei_smem_info *)SRAM_DLOAD_ADDR;
    
    if (NULL == smem_data)
    {
        ver_print_error("Dload smem_data malloc fail!\n");
        return;
    }
    
    if(BSP_TRUE == bSoftLoad)
    {
        smem_data->smem_dload_flag = SMEM_DLOAD_FLAG_NUM;
    }
    else
    {
        smem_data->smem_dload_flag = 0;
    }
    
    if(SMEM_SWITCH_PUCI_FLAG_NUM == smem_data->smem_switch_pcui_flag)
    {
        smem_data->smem_switch_pcui_flag = 0;
    }
    return;
#else
	/*TODO:先打桩，后面产品线实现*/
	return;
#endif 
/*END DTS2013080708856 W00183456 20130807 Modified*/
}

/*****************************************************************************
* 函 数 名  : BSP_DLOAD_GetSoftLoad
*
* 功能描述  : 获取启动标志
*
* 输入参数  : void
* 输出参数  : 无
*
* 返 回 值  : BSP_TRUE  :从bootrom启动
*             BSP_FALSE :从vxWorks启动
*
* 其它说明  : 无
*
*****************************************************************************/
BSP_BOOL BSP_DLOAD_GetSoftLoad (void)
{
/*BEGIN DTS2013080708856 W00183456 20130807 modified*/
#if (FEATURE_ON == MBB_DLOAD)
    huawei_smem_info *smem_data = NULL;
    smem_data = (huawei_smem_info *)SRAM_DLOAD_ADDR;
    
    if (NULL == smem_data)
    {
        ver_print_error("Dload smem_data malloc fail!\n");
        return BSP_FALSE;
    }

    if(SMEM_DLOAD_FLAG_NUM == smem_data->smem_dload_flag )
    {
        return BSP_TRUE;
    }
    else
    {
        return BSP_FALSE;
    }
#else
	/*TODO:先打桩，后面产品线实现*/
	return BSP_TRUE;
#endif
/*END DTS2013080708856 W00183456 20130807 Modified*/
}

/*****************************************************************************
* 函 数 名  : BSP_DLOAD_SendData
*
* 功能描述  : 通过虚拟串口向PC端发送数据
*
* 输入参数  : pBuf      :数据缓冲区
*             u32Len    :数据缓冲区长度
* 输出参数  : pBuf      :数据缓冲区
*
* 返 回 值  : 无
*
* 其它说明  : pBuf必须保证cache line(32字节)对齐
*
*****************************************************************************/
BSP_S32 BSP_DLOAD_SendData(char *pBuf, BSP_U32 u32Len)
{
	/*TODO:*/
	return OK;
}

/*****************************************************************************
* 函 数 名  : BSP_DLOAD_AtProcReg
*
* 功能描述  : 注册AT处理函数
*
* 输入参数  : pFun
* 输出参数  : 无
*
* 返 回 值  : DLOAD_OK:成功
*
* 其它说明  : 无
*
*****************************************************************************/

BSP_S32 BSP_DLOAD_AtProcReg (BSP_DLOAD_AtCallBack pFun)
{
    return DLOAD_OK;
}

/*****************************************************************************
* 函 数 名  : BSP_DLOAD_GetDloadVer
*
* 功能描述  : 查询下载协议版本号。该信息BSP固定写为2.0。
*
* 输入参数  :
				char *str  :字符串缓冲区
*             		BSP_S32 len      :字符串长度
* 输出参数  : BSP_S8 *str   :字符串缓冲区
*
* 返 回 值  : DLOAD_ERROR   :输入参数非法
*             其它          :返回字符串长度
*
* 其它说明  : AT模块调用
*             正常模式支持
*             下载模式支持
*****************************************************************************/
BSP_S32 BSP_DLOAD_GetDloadVer(char *str, BSP_U32 len)
{

    ver_print_info("getDloadVersion\r\n");

    if((NULL == str) || ((UINT32)len < sizeof(DLOAD_VERSION)))
    {
        ver_print_error("param error!\n");
        return VER_ERROR;
    }

    memset(str, 0x0, (UINT32)len);
    memcpy(str, DLOAD_VERSION, sizeof(DLOAD_VERSION));

    return VER_OK;
}

int BSP_DLOAD_GetDloadInfo(unsigned char atCmdBuf[], unsigned int dloadType)
{
	int ret=VER_OK;
	static bool b_geted=false;
    static struct dload_info_type dload_info={"\0","\0","\0"};

	if(!b_geted){
		b_geted=true;
		ret=bsp_dload_get_dload_info(&dload_info);
		if(VER_OK!=ret){
			ver_print_error("get dload info failed !\n");
			return VER_ERROR;
		}
	}
	/* coverity[secure_coding] */
	sprintf((char *)atCmdBuf,"\r\n\r\nswver:%s\r\n\
	    \r\niso:%s\r\n\
	    \r\nproduct name:%s\r\n\
	    \r\ndload type:%d\r\n\
	    \r\nOK\r\n",dload_info.software_ver,dload_info.iso_ver,dload_info.product_name,dloadType);

	return VER_OK;
}

/*****************************************************************************
* 函 数 名  : BSP_DLOAD_GetDloadType
*
* 功能描述  : 获取下载类型
*
* 输入参数  : void
* 输出参数  : 无
*
* 返 回 值  : 0 :正常模式
*             1 :强制模式
*
* 其它说明  : AT模块调用
*             正常模式支持
*             下载模式支持
*
*****************************************************************************/
BSP_S32 BSP_DLOAD_GetDloadType(void)
{
    if(DLOAD_MODE_NORMAL == BSP_DLOAD_GetCurMode())
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
/*****************************************************************************
* 函 数 名  : BSP_DLOAD_GetProductId
*
* 功能描述  : 获取产品名称字符串
*
* 输入参数  : BSP_S8 *str   :字符串缓冲区
*             BSP_S32 len   :字符串长度
* 输出参数  : BSP_S8 *str   :字符串缓冲区
*
* 返 回 值  : DLOAD_ERR_INVALID_PARA    :输入参数非法
*             VER_ERROR                :失败
*             其它                      :返回字符串长度
*
* 其它说明  : AT模块调用
*             正常模式支持
*             下载模式支持
*
*****************************************************************************/
BSP_S32 BSP_DLOAD_GetProductId(char *str, BSP_U32 len)
{
	static bool b_geted=false;
	static char* product_id=NULL;

    if((NULL == str) || (len == 0))
    {
        ver_print_error("param error!\n");
        return VER_ERROR;
    }

	if(!b_geted){
		b_geted=true;
		product_id = bsp_version_get_dloadid();
	}

	if(!product_id)
	{
		ver_print_error("get product dload ID failed !\n");
		return VER_ERROR;
	}

    /*lint -save -e732 -e713 */
	strncpy(str,product_id,len);
    /*lint -restore*/
	return VER_OK;
}

/*****************************************************************************
* 函 数 名  	: BSP_DLOAD_GetSoftwareVer
* 功能描述  	: 获取软件版本号
* 输入参数  	:
	BSP_S8 *str   	: 字符串缓冲区
*     BSP_S32 len   : 字符串长度

* 输出参数  	:
	BSP_S8 *str   	: 字符串缓冲区

* 返 回 值  	:
	BSP_ERROR    : 输入参数非法
*             其它 	: 返回字符串长度
* 其它说明  	:  AT模块调用
*             正常模式支持
*             下载模式支持
*****************************************************************************/
BSP_S32 BSP_DLOAD_GetSoftwareVer(char *str, BSP_U32 len)
{
	static bool b_geted=false;
	static char* software_ver=NULL;

    if((NULL == str) || (len == 0))
    {
        ver_print_error("param error!\n");
        return VER_ERROR;
    }


	if(!b_geted){
		b_geted=true;
		software_ver = bsp_version_get_firmware();
	}

	if(!software_ver)
	{
		ver_print_error("get software version failed !\n");
		return VER_ERROR;
	}

    /*lint -save -e732 -e713*/
	strncpy(str,software_ver,len);
    /*lint -restore*/
	return VER_OK;

}


#if (FEATURE_ON == MBB_DLOAD)
#ifdef __KERNEL__
/*****************************************************************
* 函 数 名  	: huawei_set_upinfo
* 功能描述  	: 检测set flag info
* 输入参数  	: 无
* 输出参数  	: 无
* 返 回 值  	: void
******************************************************************/
BSP_S32 huawei_set_upinfo(char* pData)
{
    (void)huawei_set_update_info(pData);
    return VER_OK;
}

void huawei_get_upinfo_times(BSP_U32* times)
{
    (void)huawei_get_update_info_times(times);
    return;
}
BSP_S32 huawei_get_spec_upinfo(char* pData, BSP_U32 pDataLen, BSP_U32 num)
{
    BSP_S32 ret = VER_ERROR;
    ret = huawei_get_spec_num_upinfo(pData, pDataLen, num);
    return ret;
}
#endif  /* __KERNEL__*/
/*****************************************************************
* 函 数 名  	: restart_func
* 功能描述  	: MBB 调用海思重启接口，延时2.5s
* 输入参数  	: 工作队列
* 输出参数  	: 无
* 返 回 值  	: void
******************************************************************/
static void restart_func(struct work_struct* data)
{
    /* 调用海思reboot接口 */
    printk(KERN_ERR "######ready to restart!######\n"); 
    DRV_SHUT_DOWN(DRV_SHUTDOWN_RESET);
    return;
}
#endif  /* MBB_DLOAD*/

/*****************************************************************
* 函 数 名  	: BSP_OM_SoftReboot
* 功能描述  	: 重启单板
* 输入参数  	:

* 输出参数  	:
* 返 回 值  	:

* 其它说明  : AT模块调用
*             正常模式支持
*             下载模式支持
******************************************************************/
void BSP_OM_SoftReboot(void)
{
/*BEGIN: DTS2013090702179 xuchao x00202188 2013-09-09 Modified*/
#if(FEATURE_ON == MBB_COMMON)
    power_on_wdt_cnt_set();
#endif
/*END  : DTS2013090702179 xuchao x00202188 2013-09-09 Modified*/

#if (FEATURE_ON == MBB_DLOAD)
    /* set 2.5s delay before restart */
    if(false == restart_workqueue_create_flag)
    {
        restart_workqueue = create_workqueue("huaweirestart");
        if(NULL == restart_workqueue)
        {
            printk(KERN_ERR "restart create_workqueue err and restart now!\n");
            DRV_SHUT_DOWN(DRV_SHUTDOWN_RESET);
            return;
        }
        INIT_DELAYED_WORK(&restart_process, restart_func);

        restart_workqueue_create_flag = true;
    }
    else
    {
        printk(KERN_ERR "restart create_workqueue have created!\n");
    }

    queue_delayed_work(restart_workqueue, 
        &restart_process, msecs_to_jiffies(HUAWEI_UPDATE_START_DELAY_TIME));
#else
    DRV_SHUT_DOWN(DRV_SHUTDOWN_RESET);
#endif /* MBB_DLOAD */
    return;
}

/*****************************************************************************
* 函 数 名  : BSP_DLOAD_GetNVBackupFlag
*
* 功能描述  : 获取升级前是否进行NV备份标志
*
* 输入参数  : 无
*
* 输出参数  : 无
*
* 返 回 值  : BSP_TRUE  :备份NV
*            BSP_FALSE :不备份NV
*
* 其它说明  : 此接口只对非一键式升级方式（SD升级/在线升级）有效，一键式升级会发AT命令设置
*
*****************************************************************************/
BSP_BOOL BSP_DLOAD_GetNVBackupFlag(void)
{
	return 0;
}

/* BEGIN DTS2013070403965 s00184745 Added 2013-07-04 */
#if (FEATURE_ON == MBB_COMMON)/*lint !e553*/
/*****************************************************************
* 函 数 名  	: BSP_DLOAD_BaselineVer
* 功能描述  	: 查询海思基线版本
* 输入参数  	:

* 输出参数  	:
* 返 回 值  	:

* 其它说明  : AT模块调用
*             
******************************************************************/
BSP_S32 BSP_DLOAD_BaselineVer(BSP_CHAR *str, BSP_U32 len)
{
    static bool b_geted = false;
    static char* base_software_ver = NULL;

    if((NULL == str) || (len <= 0))
    {
        ver_print_error("param error!\n");
        return VER_ERROR;
    }

    if(!b_geted)
    {
        b_geted = true;
        base_software_ver = bsp_version_get_baseline();
    }

    if(!base_software_ver)
    {
        ver_print_error("get software version failed !\n");
        return VER_ERROR;
    }

    strncpy(str,base_software_ver,len);
    return VER_OK;
}
#endif
/* END DTS2013070403965 s00184745 Added 2013-07-04 */
#endif
