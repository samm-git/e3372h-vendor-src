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
/***************************************************************************************************************
    问题单号               时间               修改人              说明
  DTS2014022608254        2014-02-27      ankang 00252937      AT^SD命令完善
  DTS2014071902709        2014-07-19      liuhuan 00219313     CS工位检查SD卡不在位状态失败，AT^SD=0返回值有问题 
  DTS2014122501147        2014-12-25      fangwentao fwx241803    休眠唤醒后sd卡子设备号变化导致上层无法找到设备问题
****************************************************************************************************************/
/*lint -save -e18 -e26 -e64 -e115 -e119 -e123 -e529 -e537 -e569 -e570 -e713 -e718 -e732 -e746 -e747 -e958*/
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include <osl_types.h>
#include <linux/scatterlist.h>
#include <linux/mmc/host.h>
#include <linux/mmc/mmc.h>
#include <linux/mmc/ioctl.h>
#include <linux/mmc/sd.h>
#include <linux/mmc/card.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/genhd.h>
#include <linux/major.h>
#include <linux/uaccess.h>



#define SDMMC_BLOCK_BYTE 	(512)
#define SDMMC_ONE_BLOCK 	(1)
enum mmc_at_process_status {
	SDMMC_PROCESS_OVER_OR_UNDO = 0,
	SDMMC_PROCESS_IN,
	SDMMC_PROCESS_ERROR,
};

enum mmc_at_process_error { 
	SDMMC_NOT_IN = 0,
	SDMMC_INIT_FAIL,
	SDMMC_PARAMETER_ERROR,
	SDMMC_ADDR_ERROR,
	SDMMC_OTHER_ERROR,
	SDMMC_NO_ERROR_PARA
};
enum mmc_at_process_data {
	SDMMC_DATA0 = 0,
	SDMMC_DATA1,
	SDMMC_DATA2,
	SDMMC_DATA3,
	SDMMC_DATA_BUTT,
};
/*Command*/
enum mmc_at_process_command {
	SDMMC_AT_FORMAT = 0,
	SDMMC_AT_ERASE_ALL,
	SDMMC_AT_ERASE_ADDR,
	SDMMC_AT_WRTIE_ADDR,
	SDMMC_AT_READ,
	SDMMC_AT_BUTT,
};

static unsigned int mmc_at_status;

#ifdef CONFIG_MMC_MSHCI_BALONG
struct mmc_host* mmc_get_host(void);
#endif

#if (FEATURE_ON == MBB_USB_SD)
int sd_block_minors = -1;    // sd card dynamic minors
EXPORT_SYMBOL(sd_block_minors);
#endif

int mmc_get_status(void)
{
	struct gendisk *disk;
	int partno = 0;
    #if (FEATURE_ON == MBB_USB_SD)
    /* use dynamic minors instead of 0 */
    disk = get_gendisk(MKDEV(MMC_BLOCK_MAJOR, sd_block_minors), &partno);
    #else
	disk = get_gendisk(MKDEV(MMC_BLOCK_MAJOR, 0), &partno);
    #endif
	if (!disk) {
		return 0;
	}
	return 1;
}

int mmc_get_dev_idex(void)
{
	struct gendisk *disk;
	int partno = 0;
	int ret;
    #if (FEATURE_ON == MBB_USB_SD)
    /* use dynamic minors instead of 0 */
    disk = get_gendisk(MKDEV(MMC_BLOCK_MAJOR, sd_block_minors), &partno);
    #else
	disk = get_gendisk(MKDEV(MMC_BLOCK_MAJOR, 0), &partno);
    #endif
	if (!disk) {
		return -ENOMEDIUM;
	}
	ret = disk->disk_name[6] - '0';/*lint !e409*/
	return ret;
}
unsigned long mmc_get_capacity(void)
{
	struct gendisk *disk;
	int partno = 0;
	unsigned long ret;
    #if (FEATURE_ON == MBB_USB_SD)
    /* use dynamic minors instead of 0 */
    disk = get_gendisk(MKDEV(MMC_BLOCK_MAJOR, sd_block_minors), &partno);
    #else
	disk = get_gendisk(MKDEV(MMC_BLOCK_MAJOR, 0), &partno);
    #endif
	if (!disk) {
		return 0;
	}
	ret = get_capacity(disk);
	return ret;
}

unsigned long mmc_at_prepare(unsigned int tpye, 
	unsigned long addr, char * buf, unsigned long *err, unsigned int *at_status)
{
	int status;
	unsigned int capacity;
	unsigned long ret = -1;/* [false alarm]:fortify disable */

	if (!err || !buf) {
		goto out;
	}
	/*not support */
	if (SDMMC_PROCESS_IN == mmc_at_status) {
		*err = SDMMC_OTHER_ERROR;
		goto out;
	}
	*at_status = SDMMC_PROCESS_IN;
	if (tpye >= SDMMC_AT_BUTT) {
		*err = SDMMC_PARAMETER_ERROR;
		*at_status = SDMMC_PROCESS_ERROR;
		goto out;
	}
	status = mmc_get_status();
	/*sd card not present*/
	if (!status) {
		*err = SDMMC_NOT_IN;
		*at_status = SDMMC_PROCESS_ERROR;
		goto out;
	}
	capacity = mmc_get_capacity();
	switch (tpye) {
		case SDMMC_AT_ERASE_ADDR:
			break;
		case SDMMC_AT_WRTIE_ADDR:
			break;
		case SDMMC_AT_READ:
			if ((addr + SDMMC_ONE_BLOCK) > capacity) {
				*err = SDMMC_ADDR_ERROR;
				*at_status = SDMMC_PROCESS_ERROR;
				goto out;
			}
			break;
		default:
			break;

	}
	return 0;
out:
	return ret;
}

int mmc_at_erase(	struct block_device * blk_dev, unsigned int from, unsigned int to)
{
	struct mmc_ioc_cmd cmd;
	int ret = 0;

	memset(&cmd, 0, sizeof(struct mmc_ioc_cmd));
	cmd.opcode = SD_ERASE_WR_BLK_START;
	cmd.arg = from;
	cmd.flags = MMC_RSP_SPI_R1 | MMC_RSP_R1 | MMC_CMD_AC;
	ret = ioctl_by_bdev(blk_dev, MMC_IOC_CMD, (unsigned long)(&cmd));
	if (ret) {
		goto out;
	}
    memset(&cmd, 0, sizeof(struct mmc_ioc_cmd));
	cmd.opcode = SD_ERASE_WR_BLK_END;
	cmd.arg = to;
	cmd.flags = MMC_RSP_SPI_R1 | MMC_RSP_R1 | MMC_CMD_AC;
	ret = ioctl_by_bdev(blk_dev, MMC_IOC_CMD, (unsigned long)(&cmd));
	if (ret) {
		goto out;
	}
    memset(&cmd, 0, sizeof(struct mmc_ioc_cmd));
	cmd.opcode = MMC_ERASE;
	cmd.arg = MMC_ERASE_ARG;
	cmd.flags = MMC_RSP_SPI_R1B | MMC_RSP_R1B | MMC_CMD_AC;
	ret = ioctl_by_bdev(blk_dev, MMC_IOC_CMD, (unsigned long)(&cmd));
out:	
	return ret;
}


int mmc_at_rw(struct block_device * blk_dev, unsigned int from,
	unsigned char *buf, int write, int data)
{
	struct mmc_ioc_cmd cmd;
	int ret = 0;
	unsigned char check_data[5] = {0x00, 0x55, 0xAA, 0xFF, 0x00};
	
	memset(&cmd, 0, sizeof(struct mmc_ioc_cmd));
	if (write) {
		if (data > 4) {
			data = 4;
		}
		memset(buf, check_data[data], SDMMC_BLOCK_BYTE);
		cmd.write_flag = 1;
		cmd.opcode = MMC_WRITE_BLOCK;
	} else {
		cmd.write_flag = 0;
		cmd.opcode = MMC_READ_SINGLE_BLOCK;	
	}
	cmd.blksz = SDMMC_BLOCK_BYTE;
	cmd.flags = MMC_RSP_R1 | MMC_CMD_ADTC;
	cmd.blocks = 1;
	cmd.arg = from;
	mmc_ioc_cmd_set_data(cmd, buf);
	
	ret = ioctl_by_bdev(blk_dev, MMC_IOC_CMD, (unsigned long)(&cmd));
	return ret;
}


/*****************************************************************************
* 函 数 名  : DRV_SD_SG_INIT_TABLE
*
* 功能描述  : SD多块数据传输sg list初始化
*
* 输入参数  : const void *buf		待操作的buffer地址
				  unsigned int buflen	待操作的buffer大小，小于32K, 大小为512B的整数倍
				  					大于32K, 大小为32KB的整数倍，最大buffer为128K
* 输出参数  : NA
*
* 返 回 值  :  0 : 成功;  其它:失败
* 其它说明  : NA
*
*****************************************************************************/
int DRV_SD_SG_INIT_TABLE(const void *buf,unsigned int buflen)
{
    return 0;
}


int DRV_SD_TRANSFER(struct scatterlist *sg, 
	unsigned dev_addr,unsigned blocks, unsigned blksz, int wrflags)
{
	return 0;
}

/*****************************************************************************
* 函 数 名  : DRV_SD_MULTI_TRANSFER
*
* 功能描述  : SD多块数据传输
*
* 输入参数  : unsigned dev_addr	待写入的SD block地址
				  unsigned blocks		待写入的block 个数
				  unsigned blksz		每个block 的大小，单位字节
				  int write			读写标志位，写:1;	读:0
* 输出参数  : NA
*
* 返 回 值  :  0 : 成功;  其它:失败
* 其它说明  : NA
*
*****************************************************************************/
int DRV_SD_MULTI_TRANSFER(unsigned dev_addr, 
	unsigned blocks,unsigned blksz,int write)
{
    return 0;
}

/*****************************************************************************
* 函 数 名  : DRV_SD_GET_CAPACITY
* 功能描述  : 卡容量查询
* 输入参数  : 无
* 输出参数  : 无
* 返 回 值  : 0 : 失败；>0: 卡容量
*****************************************************************************/
unsigned int DRV_SD_GET_CAPACITY(void)
{
	unsigned int ret;
	ret = mmc_get_capacity();
	return ret;
}

/*****************************************************************************
* 函 数 名  : DRV_SD_GET_STATUS
* 功能描述  : 卡在位查询
* 输入参数  : 无
* 输出参数  : 无
* 返 回 值  : 0 : 在位；-1: 不在位
*****************************************************************************/
int DRV_SD_GET_STATUS(void)
{	
	int ret;
	ret= mmc_get_status();
	return ret ? 0 : -1;
}


int DRV_SDMMC_GET_STATUS(void)
{
	int ret;
	ret = mmc_get_status();
	return ret;
}

unsigned long DRV_SDMMC_GET_OPRT_STATUS(void)
{
	return mmc_at_status;
}


/*****************************************************************************
 函 数 名  : BSP_SDMMC_ATProcess
 功能描述  : at^sd,SD卡操作，写，擦除，格式化操作
             0:
 输入参数  : 操作类型 ulOp:
            0  格式化SD卡内
            1  擦除整个SD卡内容；
            2  用于指定地址内容的擦除操作，指定擦除的内容长度为512字节。擦除后的地址中写全1
            3  写数据到SD卡的指定地址中，需要带第二个和第三个参数
            4  读数据到SD卡的指定地址中

            ulAddr < address >  地址，以512BYTE为一个单位，用数字n表示

            ulData
             < data >            数据内容，表示512BYTE的内容，每个字节的内容均相同。
             0       字节内容为0x00
             1       字节内容为0x55
             2       字节内容为0xAA
             3       字节内容为0xFF

 输出参数  : pulErr
 返 回 值  : 0 ：OK  非 0 ：Error

            具体的错误值填充在*pulErr中
            0 表示SD卡不在位
            1 表示SD卡初始化失败
            2 表示<opr>参数非法，对应操作不支持(该错误由AT使用,不需要底软使用)
            3 表示<address>地址非法，超过SD卡本身容量
            4 其他未知错误
*****************************************************************************/

unsigned long  DRV_SDMMC_AT_PROCESS(unsigned int type,
                unsigned long addr,  unsigned long data, 
                unsigned char *buf, unsigned long *err)
{
	unsigned long ret = 0;
	int partno = 0;
	struct gendisk *disk = NULL;
	struct block_device * blk_dev = NULL;
	unsigned int capacity = 0;
	unsigned int from = 0;
	unsigned int to = 0;
#ifdef CONFIG_MMC_MSHCI_BALONG
    unsigned int nr = 0;
    struct mmc_host *host = NULL;
#endif
	
	/*parameter check*/
	ret = mmc_at_prepare(type,addr, buf, err, &mmc_at_status);
	if (ret) {
#if (FEATURE_ON == MBB_COMMON)
        return ret;
#else
        goto out;
#endif
	}

    #if (FEATURE_ON == MBB_USB_SD)
    /* use dynamic minors instead of 0 */
    disk = get_gendisk(MKDEV(MMC_BLOCK_MAJOR, sd_block_minors), &partno);
    #else
	disk = get_gendisk(MKDEV(MMC_BLOCK_MAJOR, 0), &partno);
    #endif
	if (!disk) {
		goto out;
	}
	blk_dev = bdget_disk(disk, partno);
	if (!blk_dev) {
		goto out;
	}
    /*在进行下面的sd卡操作前，首先要打开该设备，建立block_device和gendisk的关联，防止为空造成单板重启*/
    ret  = blkdev_get(blk_dev, FMODE_WRITE | FMODE_READ, NULL);
    if (ret)
    {
        goto out;
    }
#ifdef CONFIG_MMC_MSHCI_BALONG
    host = mmc_get_host();
    if (!host || !host->card) {
        printk(KERN_ERR"host = 0x%x  host->card = 0x%x \n", host, host->card);
        ret = (unsigned int)-1;
        goto out;
    }
#endif
	capacity = mmc_get_capacity();

	switch (type) {
		/*erase not yet suport*/
		case SDMMC_AT_ERASE_ADDR:
#ifdef CONFIG_MMC_MSHCI_BALONG
			from = addr;
			nr = 0x1;
			mmc_claim_host(host);
			ret = mmc_erase(host->card, from, nr, 0x0);
			mmc_release_host(host);
#else
            from = addr;
            to = from+ 0x1;
            ret = mmc_at_erase(blk_dev, from, to);	
#endif
			break;
		case SDMMC_AT_FORMAT:
		case SDMMC_AT_ERASE_ALL:
#ifdef CONFIG_MMC_MSHCI_BALONG
			from = 0x0;
			nr = capacity - from;
			mmc_claim_host(host);
			ret = mmc_erase(host->card, from, nr, 0x0);
			mmc_release_host(host);
#else
            from = 0x1;
            to = from + capacity - 1;
            ret = mmc_at_erase(blk_dev, from, to);	
#endif
			break;
		case SDMMC_AT_WRTIE_ADDR:
			ret = mmc_at_rw(blk_dev, 0x1, buf, 0x1, data);
			break;
		case SDMMC_AT_READ:
			ret = mmc_at_rw(blk_dev, 0x1, buf, 0x0, 0x0);
			break;
		case SDMMC_AT_BUTT:
			break;
		default:
			break;
	}

out:
	if (ret) {
		*err = SDMMC_OTHER_ERROR;
		mmc_at_status= SDMMC_PROCESS_ERROR;
	} else {
		*err = SDMMC_NO_ERROR_PARA;
		mmc_at_status = SDMMC_PROCESS_OVER_OR_UNDO;
        /*在at命令处理结束时，关闭该设备，恢复原始状态*/
        blkdev_put(blk_dev, FMODE_WRITE | FMODE_READ);
	}
	return ret;
}

unsigned int at_test(unsigned int type, unsigned long addr, unsigned long data)
{
	unsigned char buf[SDMMC_BLOCK_BYTE];
	unsigned long err;
	unsigned int ret;
	ret = DRV_SDMMC_AT_PROCESS(type, addr, data, buf, &err);
	print_hex_dump(KERN_INFO, "", DUMP_PREFIX_OFFSET, 16, 1,
		buf, SDMMC_BLOCK_BYTE, 0);
	return ret;
}
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
/*lint -restore*/

