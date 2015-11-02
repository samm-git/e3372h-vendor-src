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
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/cdev.h>
#include <asm/io.h>
#include <asm/system.h>
#include <asm/uaccess.h>
#include <osl_types.h>
#include "kdf.h"
#include "balong_kdf_ioctrl.h"

/*lint --e{21,774,831} */
//extern void kdf_reg_init(void);
extern int kdf_init(void);
extern int kdf_hash_make(void * sha_s_addr, u32 sha_s_len);
int kdfdev_open(struct inode * inode, struct file * fp);
int kdfdev_release(struct inode * inode, struct file * fp);
long kdfdev_unlocked_ioctl(struct file * fp, unsigned int cmd, unsigned long arg);

static const struct file_operations kdf_fops =
{
	.owner = THIS_MODULE,
	.open  = kdfdev_open,
	.unlocked_ioctl = kdfdev_unlocked_ioctl,
	.release = kdfdev_release,
};
struct cdev chardev;
static int kdfdev_major = KDFDEV_MAJOR;
struct kdf_dev_mem * kdf_devp = NULL;
module_param(kdfdev_major, int, S_IRUGO);

int kdfdev_open(struct inode * inode, struct file * fp)
{
	unsigned int dev_num = iminor(inode);
	if(dev_num >= KDFDEV_NR_DEVS)
	{
		return -ENODEV;
	}
	fp->private_data = kdf_devp;

	return 0;
}

int kdfdev_release(struct inode * inode, struct file * fp)
{
	return 0;
}
/*lint -save -e26*/
/*lint --e{718,746,737,731}*/
long kdfdev_unlocked_ioctl(struct file * fp, unsigned int cmd, unsigned long arg)
{
	int err = 0;
	int ret = 0;
	u32 regval1 = 0;
	u32 regval2 = 0;
	u32 data_size = 0;

	if(_IOC_TYPE(cmd) != KDFDEV_IOC_MAGIC)
	{
		return -EINVAL;
	}
	if(_IOC_NR(cmd) > KDFDEV_NR_DEVS)
	{
		return -EINVAL;
	}
	if(_IOC_DIR(cmd) & _IOC_READ)
	{
		if(0 != access_ok(VERIFY_WRITE, (void *)arg, _IOC_SIZE(cmd)))
			return -EFAULT;
	}
	else if(_IOC_DIR(cmd) & _IOC_WRITE)
	{
		if(0 != access_ok(VERIFY_WRITE, (void *)arg, _IOC_SIZE(cmd)))
			return -EFAULT;
	}

	if(KDF_IOC_GETSTATUS == cmd)
	{
		regval1 = readl(CIPHER_REGBASE_ADDR + CIPHER_CHNBDQ_RWPTR(KDF_CHN_NUM));/*lint !e666*/
		regval2 = readl(CIPHER_REGBASE_ADDR + CIPHER_CHN_ENABLE(KDF_CHN_NUM));/*lint !e666*/
		ret = ((regval1 & 0x3FF) == ((regval1 & 0x3FF0000) >> 16)) && (0 == regval2 & (0x1UL << 31)) ? 1 : 0; /*lint !e514*/
		err = (int)copy_to_user((int*)arg, (void*)&ret, sizeof(int));
	}
	else if(KDF_IOC_SETDATA == cmd)
	{
		data_size = *(u32*)arg;
		err = (int)copy_from_user((void*)(fp->private_data),(void*)(arg + sizeof(u32)), data_size);
		err = (int)kdf_hash_make(fp->private_data, data_size);
	}
	else if(KDF_IOC_SETKEY == cmd)
	{
		err = (int)copy_from_user((void*)(CIPHER_REGBASE_ADDR + CIPHER_KEYRAM_OFFSET + KDF_SHA_KEY_LENGTH * KEY_INDEX),(void*)arg, KDF_SHA_KEY_LENGTH);
	}
	else if(KDF_IOC_GETHASH == cmd)
	{
		err = (int)copy_to_user((void*)arg, (void*)(CIPHER_REGBASE_ADDR + CIPHER_KEYRAM_OFFSET + KDF_SHA_KEY_LENGTH * KEY_INDEX), KDF_SHA_KEY_LENGTH);
	}
	else
	{
		err = -1;
	}
	return err;
}
/*lint -restore +e26*/

int kdfdev_init(void)
{
	int result = 0;

	if(kdf_init())
		return -1;

	dev_t devno = MKDEV(kdfdev_major,0);/*lint !e701*/
	if(kdfdev_major)
	{
		result = register_chrdev_region(devno, KDFDEV_NR_DEVS, "kdfdev");
	}
	else
	{
		result = alloc_chrdev_region(&devno, 0, KDFDEV_NR_DEVS, "kdfdev");
		kdfdev_major = MAJOR(devno);
	}
	if(result < 0)
	{
		return result;
	}

	kdf_devp = kmalloc(KDFDEV_NR_DEVS * sizeof(struct kdf_dev_mem), GFP_KERNEL);
	if(!kdf_devp)
	{ 
		unregister_chrdev_region(devno, KDFDEV_NR_DEVS);
		return -ENOMEM;
	}
	memset(kdf_devp, 0, (KDFDEV_NR_DEVS * sizeof(struct kdf_dev_mem)));

	kdf_devp->size = KDFDEV_MEM_SIZE;
	kdf_devp->buf = kmalloc(KDFDEV_MEM_SIZE, GFP_KERNEL);
	if(!(kdf_devp->buf))
	{
		kfree(kdf_devp);
		kdf_devp = NULL;
		unregister_chrdev_region(devno, KDFDEV_NR_DEVS);
		return -ENOMEM;
	}
	memset(kdf_devp->buf, 0, KDFDEV_MEM_SIZE);

	cdev_init(&chardev, &kdf_fops);
	chardev.owner = THIS_MODULE;
	/*chardev.ops =  &kdf_fops;*/

	return cdev_add(&chardev, MKDEV(kdfdev_major, 0), KDFDEV_NR_DEVS);/*lint !e701*/
}

void kdfdev_exit(void)
{
	cdev_del(&chardev);
	kfree(kdf_devp->buf);
	kfree(kdf_devp);
	unregister_chrdev_region(MKDEV(kdfdev_major, 0), KDFDEV_NR_DEVS);/*lint !e701*/
}

MODULE_AUTHOR("W00228729");
MODULE_LICENSE("GPL");
