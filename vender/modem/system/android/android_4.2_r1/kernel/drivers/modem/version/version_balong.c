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
#endif

#ifdef __KERNEL__
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/slab.h>

#include <linux/mtd/flash_huawei_dload.h>

#endif

#include <product_config.h>
#include <hi_syssc_interface.h>
#include <osl_types.h>
#include <bsp_sram.h>
#include <drv_nv_id.h>
#include <drv_nv_def.h>
#include <comm_nv_def.h>
#include <bsp_version.h>
#include <bsp_nvim.h>



#if (FEATURE_ON == MBB_COMMON)
#include "SysNvId.h"
#endif


/*
*--------------------------------------------------------------*
* 宏定义                                                       *
*--------------------------------------------------------------*
*/



#define HUAWEI_DLOAD_CDROM_VERSION_LEN           128
#define HUAWEI_DLOAD_NAME_MAX_LEN    256
/* iso_info_flag_type结构体用sizeof()计算空间与实际所占空间的差值 */
#define DIFFERENCE 3 


/*全局变量，保存产品信息,version 初始化配置nv值*/
PRODUCT_INFO_NV_STRU huawei_product_info =
{
	0
};


/*****************************************************************************
* 函 数	: bsp_version_default_set
* 功 能	: 读取NV中的版本号信息
* 输 入	: 无
* 输 出	: 无
* 返 回	: 无
* 作 者	:z00228752
* 说 明	:先打桩成 UDP单板，等hkadc及硬件ID号划分完成后，再适配
*****************************************************************************/
/*lint -save -e958*/
static __inline__ int bsp_version_default_set(void)
{
    u32 iret;
	u32 hw_ver;/*硬件版本号*/

	hw_ver=*(u32 *)HW_VERSION_ADDR;/*get hardware version*/

    /*read nv,get the exc protect setting*/
    /*lint -save -e26 -e119*/
    iret = bsp_nvm_read(NV_ID_DRV_NV_VERSION_INIT,(u8*)&huawei_product_info,sizeof(PRODUCT_INFO_NV_STRU));
	/*lint -restore*/

    if(NV_OK != iret)
    {
        ver_print_error("version init read nv error,not set,use the default config!\n");
        return VER_ERROR;
    }

	/*未查找到hwVer*/
	if(hw_ver!=huawei_product_info.index){
		ver_print_error("product info is not defined, pls check product_info_tbl!HKADC read:0x%x,nv read:0x%x\n",hw_ver,huawei_product_info.index);
		huawei_product_info.index = hw_ver;
		return VER_ERROR;
	}

	return VER_OK;

}

/*****************************************************************************
* 函 数	: bsp_productinfo_init
* 功 能	: 全局变量初始化
* 输 入	: 无
* 输 出	: 无
* 返 回	: 0获取成功/-1获取失败
* 作 者	:z00228752
* 说 明	:先打桩成 UDP单板，等hkadc及硬件ID号划分完成后，再适配
*****************************************************************************/
int bsp_productinfo_init(void)
{
	int iret=0;

	/*TODO:后续如果有子版本号，需要适配*/

    /*清除局部结构变量的内存，即初始化为全0*/
    memset((void *)(&huawei_product_info), 0, sizeof(PRODUCT_INFO_NV_STRU));
	iret = bsp_version_default_set();
	if(VER_ERROR==iret){
		ver_print_error("bsp version version_default_set fail!\n");
	}

	ver_print_error("bsp version init OK!\n");
	return iret;
}

/*****************************************************************************
* 函 数	: bsp_version_get_hardware
* 功 能	: 获取硬件版本号
* 输 入	: 无
* 输 出	: 无
* 返 回	: 硬件版本号
* 作 者	:z00228752
* 说 明	:暂时未填加子版本号
*****************************************************************************/
char * bsp_version_get_hardware(void)
{
	unsigned int len;
	static bool b_geted=false;
	static char hardware_version[VERSION_MAX_LEN];
	char hardware_sub_ver = 0;

	/*单板为硬核，软核，或者UDP*/
	if((HW_VER_INVALID == huawei_product_info.index))
	{
		ver_print_error("init error\n");
    	return NULL;
	}

	if(!b_geted){
        /*lint -save -e18 -e718 -e746*/
		len = (unsigned int)(strlen(huawei_product_info.hwVer) + strlen(" Ver.X"));
        /*lint -restore*/

	    hardware_sub_ver = ((char)huawei_product_info.hwIdSub % 3)+'A';

		(void)memset((void *)hardware_version, 0, MemparamType(len));
		/* coverity[secure_coding] */
		strncat(strncat(hardware_version, huawei_product_info.hwVer, strlen(huawei_product_info.hwVer)), " Ver.", strlen(" Ver."));
		*((hardware_version + len) - 1) = hardware_sub_ver;
		*(hardware_version + len) = 0;

		b_geted=true;
	}
	/*子版本号待确认*/

	return (char *)hardware_version;

}
#if (FEATURE_ON == MBB_COMMON)
/*****************************************************************************
* 函 数  : bsp_version_get_hardware_no_subver
* 功 能  : 获取无子版本号的硬件版本号
* 输 入  : 无
* 输 出  : 无
* 返 回  : 无子版本号的硬件版本号
* 作 者  :d00223866
* 说 明  :
*****************************************************************************/
char * bsp_version_get_hardware_no_subver(void)
{
    int len;
    static bool b_geted = false;
    static char hardware_version_no_subver[VERSION_MAX_LEN];
    //char hardware_sub_ver = 0;

    /*单板为硬核，软核，或者UDP*/
    if((HW_VER_INVALID == huawei_product_info.index))/*lint !e10*/
    {
            ver_print_error("init error\n");
            return NULL;
    }
    /*如果没有读取过版本号*/
    if(!b_geted)
    {
        /*统计字符标记*/
        len = strlen(huawei_product_info.hwVer);/*lint !e10*/

        memset((void *)hardware_version_no_subver, 0, len);/*lint !e160 !e506*/
        /* coverity[secure_coding] */
        strncat(hardware_version_no_subver, huawei_product_info.hwVer, strlen(huawei_product_info.hwVer));/*lint !e10*/
        *(hardware_version_no_subver + len) = 0;
        /*获取版本号标记置为true*/
        b_geted = true;
    }
    /*返回硬件版本号，不带子版本号*/
    return (char *)hardware_version_no_subver;

}
#endif
/*****************************************************************************
* 函 数	: bsp_get_product_inner_name
* 功 能	: 获取内部产品名称
* 输 入	: 无
* 输 出	: 无
* 返 回	: 内部产品名称字符串指针
* 作 者	:z00228752
* 说 明	:获取到的指针不可以释放
*****************************************************************************/
char * bsp_version_get_product_inner_name(void)
{
	unsigned int len;
	static bool b_geted=false;
	static char product_inner_name[VERSION_MAX_LEN];

	if( HW_VER_INVALID == huawei_product_info.index)
	{
		return NULL;
	}

	if(!b_geted){
		len = (unsigned int)(strlen(huawei_product_info.name)+ strlen(huawei_product_info.namePlus));

		(void)memset((void*)product_inner_name, 0, MemparamType(len));

		/* coverity[secure_coding] */
		strncat(strncat(product_inner_name,huawei_product_info.name, strlen(huawei_product_info.name)), huawei_product_info.namePlus, strlen(huawei_product_info.namePlus));

		*(product_inner_name+len) = 0;
		b_geted=true;
	}

	return  (char*)product_inner_name;

}

/*****************************************************************************
* 函 数	: bsp_get_product_out_name
* 功 能	: 获取外部产品名称
* 输 入	: 无
* 输 出	: 无
* 返 回	: 外部产品名称字符串指针
* 作 者	:z00228752
* 说 明	:获取到的指针不可以释放
*****************************************************************************/
char * bsp_version_get_product_out_name(void)
{
	if(HW_VER_INVALID == huawei_product_info.index)
	{
		return NULL;
	}

	return (char *)huawei_product_info.productId;
}

/*****************************************************************************
* 函 数	: bsp_get_pcb_version
* 功 能	: 获取PCB
* 输 入	: 无
* 输 出	: 无
* 返 回	: 获取PCB字符串指针
* 作 者	:z00228752
* 说 明	:获取到的指针不可以释放
*****************************************************************************/
char * bsp_version_get_pcb(void)
{
	if(HW_VER_INVALID == huawei_product_info.index)
	{
		return NULL;
	}

	return (char *)huawei_product_info.hwVer;
}

/*****************************************************************************
* 函 数	: bsp_get_dloadid_version
* 功 能	: 获取dloadid,升级中使用的名称
* 输 入	: 无
* 输 出	: 无
* 返 回	: dloadid字符串指针
* 作 者	:z00228752
* 说 明	:获取到的指针不可以释放
*****************************************************************************/
char * bsp_version_get_dloadid(void)
{
	if(HW_VER_INVALID == huawei_product_info.index)
	{
		return NULL;
	}

	return (char *)huawei_product_info.dloadId;
}

/*****************************************************************************
* 函 数	: bsp_get_build_date_time
* 功 能	: 获取编译日期和时间
* 输 入	: 无
* 输 出	: 无
* 返 回	: 编译日期和时间
* 作 者	:z00228752
* 说 明	:获取到的指针不可以释放
*****************************************************************************/
char * bsp_version_get_build_date_time(void)
{
	static char * build_date   = __DATE__ ", " __TIME__;
	return build_date;
}

/*****************************************************************************
* 函 数	: bsp_get_chip_version
* 功 能	: 获取芯片版本号
* 输 入	: 无
* 输 出	: 无
* 返 回	: 芯片版本号
* 作 者	:z00228752
* 说 明	:获取到的指针不可以释放
*****************************************************************************/
char * bsp_version_get_chip(void)
{
	return (char *)PRODUCT_CFG_CHIP_SOLUTION_NAME;
}

/*****************************************************************************
* 函 数	: bsp_get_firmware_version
* 功 能	: 获取软件版本号
* 输 入	: 无
* 输 出	: 无
* 返 回	: 软件版本号
* 作 者	:z00228752
* 说 明	:获取到的指针不可以释放
*****************************************************************************/
char * bsp_version_get_firmware(void)
{
	u32 iret = VER_OK;
	static NV_SW_VER_STRU nv_sw_ver={0};

	/*读取NV项中的软件版本号*/
    /*lint -save -e26 -e119*/
	iret=bsp_nvm_read(NV_ID_DRV_NV_DRV_VERSION_REPLACE_I, \
				(u8 *)&nv_sw_ver,
				sizeof(NV_SW_VER_STRU));
    /*lint -restore*/

    if(NV_OK != iret){/*如果获取失败，则返回默认版本号*/
		ver_print_info("get NV_SW_VERSION_REPLACE failed!\n");
		return (char*)PRODUCT_DLOAD_SOFTWARE_VER;
	}
	else{
		if(0 == nv_sw_ver.nvStatus){
			ver_print_info("nv_sw_ver.nvStatus :%d\n",nv_sw_ver.nvStatus);
			return (char*)PRODUCT_DLOAD_SOFTWARE_VER;
		}
	}

	return (char *)nv_sw_ver.nv_version_info;
}

/*****************************************************************************
* 函 数	: bsp_get_version_hash
* 功 能	: 获取软件中心库的commit_ID
* 输 入	: 无
* 输 出	: 无
* 返 回	: commit_ID
* 作 者	:z00228752
* 说 明	:获取到的指针不可以释放
*****************************************************************************/
char * bsp_version_get_hash(void)
{
	return CFG_PRODUCT_TAG_ID;
}

/*****************************************************************************
* 函 数	: bsp_get_iso_version
* 功 能	: 获取iso_version
* 输 入	: 无
* 输 出	: 无
* 返 回	: iso_version字符串的指针
* 作 者	:z00228752
* 说 明	:获取到的指针不可以释放
*****************************************************************************/
char * bsp_version_get_iso(void)
{
	/**TODO:  暂时先返回这个数值,后续看isover如何存储*/

	#define ISO_VER "00.000.00.000.00"
	return (char *)ISO_VER;
}

#ifdef __KERNEL__
#if (FEATURE_ON == MBB_DLOAD)
#define PAGE_SIZE_MAX 4096
s32 huawei_get_spec_num_upinfo(char* str, s32 str_len, s32 num)
{
    rec_updinfo_st rec_info = {0};
    char *format_str = "%s%s\r\n\r\n%s%s\r\n\r\n%s%s\r\n\r\n%s%d\r\n\r\n%s%d";
    
    if(NULL == str)
    {
        return VER_ERROR;
    }
    /* get update info from oeminfo area */
    (void)flash_get_share_region_info( RGN_RECORD_UPINFO_FLAG, 
                        (void *)(&rec_info), sizeof(rec_updinfo_st));

    if(DLOAD_RECORD_UPDATE_MAGIC != rec_info.magic_num)
    {
        /* oeminfo is empty, return */
        return VER_ERROR;
    }
    if(num > rec_info.upinfo_record_times)
    {
        /* para err */
        return VER_ERROR;
    }
    snprintf(str, str_len, format_str, 
                "bswver: ", 
                rec_info.upswver_info[num - 1].bswver,
                "aswver: ",
                rec_info.upswver_info[num - 1].aswver,
                "uptime: ",
                rec_info.upswver_info[num - 1].uptime,
                "uptype: ",
                rec_info.upswver_info[num - 1].uptype,
                "upstatus: ",
                rec_info.upswver_info[num - 1].upstatus
            );
    return VER_OK;
    
}

void huawei_get_update_info_times(s32* times)
{
    rec_updinfo_st rec_info = {0};
    if(NULL == times)
    {
        return;
    }

    /* get update info from oeminfo area */
    (void)flash_get_share_region_info(RGN_RECORD_UPINFO_FLAG, 
                        (void *)(&rec_info), sizeof(rec_updinfo_st));
    if(DLOAD_RECORD_UPDATE_MAGIC != rec_info.magic_num)
    {
        /* oeminfo is empty, return 0 */
        *times = 0;
    }
    else
    {
        *times = rec_info.upinfo_record_times;
    }
    return;
}

void huawei_set_update_info(char* str)
{
    rec_updinfo_st rec_info = {0};
    update_info_st update_info = {{0}, {0}, 0};
    uint8 i = 0;
    uint32 cur_times = 0;
    int8* firmware_ver = NULL;
    int8 ret_buf[PAGE_SIZE_MAX] = {0};
    if(NULL == str)
    {
        ver_print_error("str is null\n");
        return;
    }
    memcpy(&update_info, str, sizeof(update_info_st));
    /* get update info from oeminfo area */
    (void)flash_get_share_region_info( RGN_RECORD_UPINFO_FLAG, 
                        (void *)(&rec_info), sizeof(rec_updinfo_st));

    /* check the magic */
    if(DLOAD_RECORD_UPDATE_MAGIC != rec_info.magic_num)
    {
        rec_info.total_times = 1;
        rec_info.upinfo_record_times = 1;
        cur_times = rec_info.upinfo_record_times - 1;
        ver_print_error("rec_info.magic_num is not magic\n");
    }
    else
    {
        /* if the current update status is 1, update this struct */
        if(UPDATE_BEGIN_STATUS == rec_info.upswver_info[rec_info.upinfo_record_times - 1].upstatus)
        {
            cur_times = rec_info.upinfo_record_times - 1;  // 4 = 5 - 1
            ver_print_error("repeat at ,times is %d\n", rec_info.upinfo_record_times);
        }
        else
        {
            if(RECORD_MAX_TIMES <= rec_info.upinfo_record_times)
            {
                /* << ,set the back info to front */
                for(i = 0; i < RECORD_MAX_TIMES - 1; i++)
                {
                    rec_info.upswver_info[i] = rec_info.upswver_info[i + 1];
                }
                (void)memset(&rec_info.upswver_info[RECORD_MAX_TIMES - 1], 0, sizeof(updswver_info_st));
                rec_info.upinfo_record_times = RECORD_MAX_TIMES;
                cur_times = RECORD_MAX_TIMES - 1;   /* set cur_times -> 10 - 1 */
            }
            else
            {
                cur_times = rec_info.upinfo_record_times;
                rec_info.upinfo_record_times++;
                ver_print_error("update at ,times is %d\n", rec_info.upinfo_record_times);
            }
        }
    }

    /* update the target software version */
    (void)strncpy(rec_info.upswver_info[cur_times].aswver, update_info.ver_info, VERSION_LENGTH - 1);

    /* update the current software version */
    firmware_ver = bsp_version_get_firmware();
    if(NULL != firmware_ver)
    {
        ver_print_error("the before version :%s\n", rec_info.upswver_info[cur_times].bswver);
        (void)memset(rec_info.upswver_info[cur_times].bswver, 0, VERSION_LENGTH);
        (void)strncpy(rec_info.upswver_info[cur_times].bswver, firmware_ver, strlen(firmware_ver));
        ver_print_error("the current version :%s\n", rec_info.upswver_info[cur_times].bswver);
    }

    /* update the current upgrade UTC time */
    (void)strncpy(rec_info.upswver_info[cur_times].uptime, update_info.time_info, VERSION_LENGTH - 1);
    
    /* update the tools type */
    rec_info.upswver_info[cur_times].uptype = update_info.tooltype;

    /* update the record times */
    rec_info.upswver_info[cur_times].upstatus = UPDATE_BEGIN_STATUS;
    
    /* update the magic number */
    rec_info.magic_num = DLOAD_RECORD_UPDATE_MAGIC;
    memcpy(ret_buf, &rec_info, sizeof(rec_updinfo_st));

    (void)flash_update_share_region_info(RGN_RECORD_UPINFO_FLAG, (void *)ret_buf);
    return;
}
#endif /* MBB_DLOAD */
#endif /* __KERNEL__*/

/*****************************************************************************
* 函 数	: bsp_get_iso_version
* 功 能	: 获取iso_version
* 输 入	: 
str:保存后台版本号
           len:需要获取的后台号版本号长度
* 输 出	: 无
* 返 回	: 无
* 作 者	:  w00183456
* 说 明	:获取到的指针不可以释放
*****************************************************************************/
void bsp_get_iso_version(char *str,int len)
{
#ifdef __KERNEL__
#if (FEATURE_ON == MBB_DLOAD)
    unsigned int   verLen = 0;
    unsigned char   bufTmp[HUAWEI_DLOAD_CDROM_VERSION_LEN] = {0};
    iso_info_flag_type  *iso_version_info = NULL;
    char  iso_ver[HUAWEI_DLOAD_CDROM_VERSION_LEN] = "00.00.00.000.00";

    /*入参判断*/
    if(NULL == str || len < HUAWEI_DLOAD_CDROM_VERSION_LEN)
    {
        ver_print_error("Input parameter error!.\r\n");
        return ;
    }
    
    /*获取单板软件版本信息字符串信息*/ 
    verLen = HUAWEI_DLOAD_CDROM_VERSION_LEN;
    memset(bufTmp, 0, HUAWEI_DLOAD_CDROM_VERSION_LEN);

    /* 获取ISO版本号 */
    /*lint -e429*/
    if (true == flash_get_share_region_info(RGN_ISOHD_FLAG,bufTmp,verLen))
    {
        iso_version_info = (iso_info_flag_type*)bufTmp;
        if (iso_version_info->in_use_flag == FLASH_OEM_REGION_IN_USE_FLAG 
            && iso_version_info->iso_info_magic == DLOAD_ISO_VERSION_MAGIC)
        {
            memset(str, 0, len);
            /*  此处为3是根据iso_info_flag_type结构体原因所致*/
            snprintf((char*)str, HUAWEI_DLOAD_CDROM_VERSION_LEN, "%s", \
                    bufTmp + sizeof(iso_info_flag_type) - DIFFERENCE);
             ver_print_error("flash_get_share_region_info ture. \r\n");
        }
        else
        {
            memset(str, 0, len);
            snprintf((char*)str, HUAWEI_DLOAD_CDROM_VERSION_LEN, "%s", iso_ver);
            ver_print_error("flash_get_share_region_info ture iso null.\r\n");
        }
    }
    else
    {
        memset(str, 0, len);
        snprintf((char*)str, HUAWEI_DLOAD_CDROM_VERSION_LEN, "%s", iso_ver);
        ver_print_error("flash_get_share_region_info false.\r\n");
    }
#endif
#else
    char  iso_ver[HUAWEI_DLOAD_CDROM_VERSION_LEN] = "00.00.00.000.00";

    /*入参判断*/
    if(NULL == str || len < HUAWEI_DLOAD_CDROM_VERSION_LEN)
    {
        ver_print_error("Input parameter error!.\r\n");
        return ;
    }

    memset(str, 0, len);
    snprintf((char*)str, HUAWEI_DLOAD_CDROM_VERSION_LEN, "%s", iso_ver);
    ver_print_error("ccore default!.\r\n");
#endif
    return ;
}


/*****************************************************************************
* 函 数	: bsp_get_webui_version
* 功 能	: 获取webui_version
* 输 入	: 无
* 输 出	: 无
* 返 回	: webui_version字符串的指针
* 作 者	:z00228752
* 说 明	:获取到的指针不可以释放
*****************************************************************************/
char * bsp_version_get_webui(void)
{
	/**TODO:  同isover*/
	return NULL;
}

/*****************************************************************************
* 函 数	: bsp_version_get_release
* 功 能	: 获取全版本号
* 输 入	: 无
* 输 出	: 无
* 返 回	: 全版本号字符串的指针
* 作 者	:z00228752
* 说 明	:获取到的指针不可以释放
*****************************************************************************/
char * bsp_version_get_release(void)
{
	/**TODO:  */
	return PRODUCT_VERSION_STR;
}

/*****************************************************************************
* 函 数	: bsp_version_get_boardid
* 功 能	: 获得硬件版本号索引
* 输 入	: 无
* 输 出	: 无
* 返 回	: 硬件版本号索引字符串的指针
* 作 者	:z00228752
* 说 明	:获取到的指针不可以释放
*****************************************************************************/
int bsp_version_get_boardid(void)
{
    int hw_ver=0;
	hw_ver=*(int *)HW_VERSION_ADDR;/*get hardware version*/

	return hw_ver;
}
/*****************************************************************************
* 函数	: bsp_get_board_chip_type
* 功能	: get board type and chip type
* 输入	: void
* 输出	: void
* 返回	: BOARD_TYPE_E
*****************************************************************************/
u32 bsp_version_get_board_type(void)
{
    unsigned int type = 0xffffffff;
	type = (u32)bsp_version_get_boardid();

	if(HW_VER_INVALID==type){
		ver_print_error("[bsp_get_board_chip_type]:get hw version failed!hw_ver:0x%x\n",type);
		return type;
	}

	/*若为udp，只返回UDP硬件版本号，屏蔽射频扣板信息*/
	if(HW_VER_UDP_MASK==(type & HW_VER_UDP_MASK)){
		return HW_VER_PRODUCT_UDP;
	}

	/*若为k3v3UDP,只返回UDP硬件版本号*/
	if(HW_VER_K3V3_UDP_MASK == (type & HW_VER_K3V3_UDP_MASK)){
		return HW_VER_K3V3_UDP;
	}

	/*若为k3v3FPGA,只返回FPGA硬件版本号*/
	if(HW_VER_K3V3_FPGA_MASK == (type & HW_VER_K3V3_FPGA_MASK)){
		return HW_VER_K3V3_FPGA;
	}

	if(HW_VER_V711_UDP_MASK == (type & HW_VER_V711_UDP_MASK)){
		return HW_VER_V711_UDP;
	}

	return type;
}
u32 bsp_version_get_chip_type(void)
{
	return get_hi_version_id_version_id();
}



#if (FEATURE_ON == MBB_COMMON)/*lint !e553*/
/*****************************************************************************
* 函 数 名  : bsp_version_get_baseline
*
* 功能描述  : 获取海思基线版本号
*
* 输入参数  : 无
* 输出参数  :
*
 返 回 值  : 软件版本号字符串指针
* 修改记录  :
*
*****************************************************************************/
char * bsp_version_get_baseline(void)
{
      /* 返回V7R2基线版本 */
    return (char*)PRODUCT_VERSION_STR;
}
#endif 



#if (FEATURE_ON == MBB_COMMON)
/*****************************************************************************
* 函 数	: bsp_get_factory_mode
* 功 能	: 查询当前版本是否烧片版本，如果是，输出 FACTORY_MODE_MAGIC_STR 信息
* 输 入	: 无
* 输 出	: 无
* 返 回	: 外部产品名称字符串指针
* 作 者	: y00216577
* 说 明	: 1: 正常模式
          0: 产线模式
*****************************************************************************/
int bsp_get_factory_mode(void)
{
    #define FACTORY_MODE_MAGIC_STR  "+=+=+==factory_mode+=+=+==\n"
    int ret = -1;
    FACTORY_MODE_TYPE  real_factory_mode;

    /* 读错误，认为是正常模式 */
    ret = NVM_Read(en_NV_Item_SW_VERSION_FLAG, &real_factory_mode, sizeof(FACTORY_MODE_TYPE));/*lint !e18*/
    if(0 != ret)        
    {
        printk("DR_NV_Read error, default to normal mode\n");
        return 1;
    }

    if(1 == real_factory_mode.ulFactoryMode)
    {
        printk("normal mode\n");
        return 1;
    }
    else
    {
        printk(FACTORY_MODE_MAGIC_STR);
        return 0;
    }
}
#endif


#ifdef __KERNEL__

/*****************************************************************************
* 函 数	: bsp_dload_get_dload_version
* 功 能	: 获取下载协议版本号
* 输 入	: 无
* 输 出	: 无
* 返 回	:下载协议版本号字符串的指针
* 作 者	:z00228752
* 说 明	:获取到的指针不可以释放
*****************************************************************************/
char* bsp_dload_get_dload_version(void)
{
	return (char *)DLOAD_VERSION;
}

/*****************************************************************************
* 函 数	: get_current_dload_info
* 功 能	: 获取dload_info(升级版本号)
* 输 入	: 无
* 输 出	: 无
* 返 回	:获取dload_info字符串的指针
* 作 者	:z00228752
* 说 明	:获取到的指针不可以释放
*****************************************************************************/
int bsp_dload_get_dload_info(struct dload_info_type* dload_info)
{
	char *dload_iso = NULL;
	char *product_name = NULL;
	char *firmware = NULL;

	/*获取iso version*/
	dload_iso = bsp_version_get_iso();
	if(NULL == dload_iso){
		ver_print_error("get iso ver is null!\n");
		return VER_ERROR;
	}
	/* coverity[secure_coding] */
	strncat(dload_info->iso_ver,dload_iso,strlen(dload_iso));

	/*获取dload id(升级中使用的产品名)*/
	product_name = bsp_version_get_dloadid();
	if(NULL == product_name){
		ver_print_error("get product name is null\n");
		return VER_ERROR;
	}
	/* coverity[secure_coding] */
	strncat(dload_info->product_name,product_name,strlen(product_name));

	/*获取software_version*/
	firmware = bsp_version_get_firmware();
	if(NULL == firmware){
		ver_print_error("get firmware version is null\n");
		return VER_ERROR;
	}
	/* coverity[secure_coding] */
	strncat(dload_info->software_ver,firmware,strlen(firmware));

	return VER_OK;

}
/*需要保证在nv模块初始化之后*/
module_init(bsp_productinfo_init);

#endif
/*****************************************************************************
* 函数	: bsp_get_board_actual_type
* 功能	: get board actual type 
* 输入	: void
* 输出	: void
* 返回	: BOARD_ACTUAL_TYPE_E       BBIT/SFT/ASIC
*
* 其它       : 无
*
*****************************************************************************/
BOARD_ACTUAL_TYPE_E bsp_get_board_actual_type(void)
{
	u32 chip_ver = 0;

	chip_ver = bsp_version_get_chip_type();

	switch(chip_ver){
		case CHIP_VER_P531_ASIC:
		case CHIP_VER_P532_ASIC:
			return BOARD_TYPE_BBIT;

		case CHIP_VER_HI6950_ASIC:			
		case CHIP_VER_HI3630_ASIC:
		case CHIP_VER_HI6921_ASIC:
		case CHIP_VER_HI6930_ASIC:
			return BOARD_TYPE_ASIC;

		case CHIP_VER_HI6950_SFT:
			return BOARD_TYPE_SFT;

		default:
			return BOARD_TYPE_MAX;		
	}
}

/*****************************************************************************
* 函 数	: bsp_version_debug
* 功 能	: 用于调试查看版本号相关信息
* 输 入	: 无
* 输 出	: 无
* 返 回	:获取dload_info字符串的指针
* 作 者	:z00228752
* 说 明	:获取到的指针不可以释放
*****************************************************************************/
int bsp_version_debug(void)
{
	int ret=0;
#ifdef __KERNEL__
	struct dload_info_type info={"\0","\0","\0"};
#endif

	/*判断version初始化是否成功*/
    if(huawei_product_info.index == HW_VER_INVALID){
		ver_print_error("huawei_product_info init failed!HW_VERSION_ADDR:0x%x\n",*(u32 *)HW_VERSION_ADDR);
		return VER_ERROR;
	}
	ver_print_error("\n***************************************************\n");
	ver_print_error("\n*************huawei_product_info*******************\n");
	ver_print_error("\n***************************************************\n");
	ver_print_error("Hardware index	:0x%x\n",huawei_product_info.index);
	ver_print_error("hw_Sub_ver		:0x%x\n",huawei_product_info.hwIdSub);
	ver_print_error("Inner name 	:%s\n",huawei_product_info.name);
	ver_print_error("name plus 		:%s\n",huawei_product_info.namePlus);
	ver_print_error("HardWare ver	:%s\n",huawei_product_info.hwVer);
	ver_print_error("DLOAD_ID		:%s\n",huawei_product_info.dloadId);
	ver_print_error("Out name		:%s\n",huawei_product_info.productId);

	ver_print_error("\n***************************************************\n");
	ver_print_error("\n*************from func get*******************\n");
	ver_print_error("\n***************************************************\n");
	ver_print_error("Board ID   	:0x%x\n",bsp_version_get_boardid());
	ver_print_error("Chip Type   	:0x%x\n",bsp_version_get_chip_type());
	ver_print_error("HardWare ver	:%s\n",bsp_version_get_hardware());
	ver_print_error("Inner name		:%s\n",bsp_version_get_product_inner_name());
	ver_print_error("Out name		:%s\n",bsp_version_get_product_out_name());
	ver_print_error("PCB_ver		:%s\n",bsp_version_get_pcb());
	ver_print_error("DLOAD_ID		:%s\n",bsp_version_get_dloadid());
	ver_print_error("Build_time		:%s\n",bsp_version_get_build_date_time());
	ver_print_error("Chip_ver		:%s\n",bsp_version_get_chip());
	ver_print_error("Firmware		:%s\n",bsp_version_get_firmware());
	ver_print_error("CommitID		:%s\n",bsp_version_get_hash());
	ver_print_error("ISO_ver		:%s\n",bsp_version_get_iso());
	ver_print_error("WebUI			:%s\n",bsp_version_get_webui());
	ver_print_error("Release_ver	:%s\n",bsp_version_get_release());

#ifdef __KERNEL__

	ver_print_error("dload_version	:%s\n",bsp_dload_get_dload_version());
	ret=bsp_dload_get_dload_info(&info);
#endif
	return VER_OK|ret;
}
/*lint -restore*/

