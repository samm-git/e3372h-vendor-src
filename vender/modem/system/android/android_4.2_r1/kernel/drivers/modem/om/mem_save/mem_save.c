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
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/syscalls.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/poll.h>
#include <linux/personality.h>
#include <linux/file.h>
#include <linux/fdtable.h>
#include <linux/io.h>
#include <linux/fs.h>
#include <linux/rcupdate.h>
#include <linux/hrtimer.h>
#include <linux/statfs.h>

#include "bsp_memmap.h"

typedef enum
{
    DDR_TYPE_LPHY_SDR        = 0,
    DDR_TYPE_TLPHY_IMAGE     = 1,
    DDR_TYPE_LCS             = 2,
    DDR_TYPE_MAX             = 3,
}DDR_TYPE_ENUM;

static int is_ioremap[DDR_TYPE_MAX] = {0};
static u32 virt_addr[DDR_TYPE_MAX] = {0};


s32 addr_ioremap(DDR_TYPE_ENUM ddr_type, u32 phy_addr, u32 size)
{
    if(0 == is_ioremap[ddr_type])
    {
        virt_addr[ddr_type] = (u32)ioremap(phy_addr, size);
        if(0 == virt_addr[ddr_type])
        {

		    return -1;
    	}
        is_ioremap[ddr_type] = 1;
    }
    return 0;
    
}

/*****************************************************************************
* 函 数 名  : file_save
*
* 功能描述  : 将指定地址数据保存到文件中
*
* 输入参数  : u32 export_file_name     导出文件名称
*             u32 export_virt_addr     导出数据虚拟地址
*             u32 count                导出数据大小
*
* 返 回 值  : s32  返回值:0为成功，-1为失败。
*
* 修改记录  : 2012年11月27日   
*****************************************************************************/
long file_save(char* export_file_name, u32 export_virt_addr, u32 count)
{   
    long ret = 0;
    long len = 0;    
    unsigned int  fd = 0;
    
    mm_segment_t old_fs = get_fs();
    set_fs(KERNEL_DS);

    ret = sys_open(export_file_name, O_RDWR | O_CREAT, 0666);

    if(ret < 0)
    {
        printk("open file %s error,ret:%#x\n",  export_file_name, fd);
        
        /*sys_fsync(fd);*/

        set_fs(old_fs);

        return -1;
    }
	
    fd = (unsigned int)ret;
	
    len = sys_write(fd,(char*)export_virt_addr,count);

    if(count != (unsigned long)len)
    {
        ret = -1;
        printk("write err, writlen=0x%x, wantted len=0x%x\n", (u32)len, (u32)count);
    }
    else
    {
        ret = 0;
        printk("save to %s success\n, pls use lsz %s to upload to securecrt\n", export_file_name, export_file_name);
    }

    sys_fsync(fd);

    set_fs(old_fs);

    return ret;

}


/*****************************************************************************
* 函 数 名  : file_save_tlphy_image
*
* 功能描述  : 将内存tlphy_image数据保存到文件中
*
* 输入参数  : u32 offset_addr          基于TLPHT_IMAGE基址的偏移地址
*             u32 data_size            TLPHT_IMAGE大小
*
* 返 回 值  : s32  返回值:0为成功，-1为失败。
*
* 修改记录  : 2012年11月27日   
*****************************************************************************/
long file_save_tlphy_image(u32 offset_addr, u32 data_size)
{
    unsigned count = 0;
    unsigned export_phy_addr = 0;
    unsigned export_virt_addr = 0;
    char export_file_name[] = "tlphy_image_data.bin"; 
    
    if(offset_addr + data_size > DDR_TLPHY_IMAGE_SIZE)
    {
	    printk("%s: parameter error.\n", __FUNCTION__);
	    return -1;
    }

    if(0 != addr_ioremap(DDR_TYPE_TLPHY_IMAGE, DDR_TLPHY_IMAGE_ADDR, DDR_TLPHY_IMAGE_SIZE))
    {
	    printk("%s: cannot map IO.\n", __FUNCTION__);
        return -1;
    }
    
    count = data_size;
    export_phy_addr = DDR_TLPHY_IMAGE_ADDR + offset_addr;
    printk("export data address is %#x, and the size is %#x.\n", \
        export_phy_addr, data_size);

    export_virt_addr = virt_addr[DDR_TYPE_TLPHY_IMAGE] + offset_addr;    
    printk("DDR_TLPHY_IMAGE_ADDR = %#x, DDR_TLPHY_ADDR_VIRT = %#x, DDR_TLPHY_IMAGE_SIZE = %#x.\n", \
        DDR_TLPHY_IMAGE_ADDR, virt_addr[DDR_TYPE_TLPHY_IMAGE], DDR_TLPHY_IMAGE_SIZE);

    return file_save(export_file_name, export_virt_addr, count);
}


/*****************************************************************************
* 函 数 名  : file_save_lcs
*
* 功能描述  : 将内存lcs数据保存到文件中
*
* 输入参数  : u32 offset_addr          基于TLPHT_IMAGE基址的偏移地址
*             u32 data_size            TLPHT_IMAGE大小
*
* 返 回 值  : s32  返回值:0为成功，-1为失败。
*
* 修改记录  : 2012年11月27日   
*****************************************************************************/
long file_save_lcs(u32 offset_addr, u32 data_size)
{
    unsigned count = 0;
    unsigned export_phy_addr = 0;
    unsigned export_virt_addr = 0;
    char export_file_name[] = "lcs_data.bin"; 
    
    if(offset_addr + data_size > DDR_LCS_SIZE)
    {
	    printk("%s: parameter error.\n", __FUNCTION__);
	    return -1;
    }

    if(0 != addr_ioremap(DDR_TYPE_LCS, DDR_LCS_ADDR, DDR_LCS_SIZE))
    {
	    printk("%s: cannot map IO.\n", __FUNCTION__);
        return -1;
    }
    
    count = data_size;
    export_phy_addr = DDR_LCS_ADDR + offset_addr;
    printk("export data address is %#x, and the size is %#x.\n", \
        export_phy_addr, data_size);

    export_virt_addr = virt_addr[DDR_TYPE_LCS] + offset_addr;    
    printk("DDR_LCS_ADDR = %#x, DDR_LCS_ADDR_VIRT = %#x, DDR_LCS_SIZE = %#x.\n", \
        DDR_LCS_ADDR, virt_addr[DDR_TYPE_LCS], DDR_LCS_SIZE);

    return file_save(export_file_name, export_virt_addr, count);
}


/*****************************************************************************
* 函 数 名  : file_save_lphy_sdr
*
* 功能描述  : 将lphy内存数据保存到文件中
*
* 输入参数  : u32 offset_addr          基于lphy基址的偏移地址
*             u32 data_size            lphy大小
*             u32 anten_num            采数天线编号
*
* 返 回 值  : s32  返回值:0为成功，-1为失败。
*
* 修改记录  : 2012年11月27日   
*****************************************************************************/
long file_save_lphy_sdr(u32 offset_addr, u32 data_size, u32 anten_num)
{
    unsigned count = 0;
    unsigned export_phy_addr = 0;
    unsigned export_virt_addr = 0;
    char export_file_name[60] = "";
    
    if(offset_addr + data_size > DDR_LPHY_SDR_SIZE)
    {
	    printk("%s: parameter error.\n", __FUNCTION__);
	    return -1;
    }

    if(0 != addr_ioremap(DDR_TYPE_LPHY_SDR, DDR_LPHY_SDR_ADDR, DDR_LPHY_SDR_SIZE))
    {
	    printk("%s: cannot map IO.\n", __FUNCTION__);
        return -1;
    }
    
    count = data_size;
    export_phy_addr = DDR_LPHY_SDR_ADDR + offset_addr;
    snprintf(export_file_name, 60,"lphy_sdr_data_anten%d.bin", anten_num);
    printk("export data address is %#x, and the size is %#x.\n", \
        export_phy_addr, data_size);

    export_virt_addr = virt_addr[DDR_TYPE_LPHY_SDR] + offset_addr;    
    printk("DDR_LPHY_SDR_ADDR = %#x, DDR_LPHY_SDR_ADDR_VIRT = %#x, DDR_LPHY_SDR_SIZE = %#x.\n", \
        DDR_LPHY_SDR_ADDR, virt_addr[DDR_TYPE_LPHY_SDR], DDR_LPHY_SDR_SIZE);

    return file_save(export_file_name, export_virt_addr, count);
}

