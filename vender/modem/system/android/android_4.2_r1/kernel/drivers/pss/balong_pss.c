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
#include <linux/module.h>
#include <linux/debugfs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/kallsyms.h>
#include <linux/device.h>
#include <linux/netfilter.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <linux/inetdevice.h>
#include <net/arp.h>
#include "balong_pss.h"

struct pss_man g_pss_man = {/*all init*/
	.seqnum = 1,
	.max = 0, 
	.cur_r = 0,
	.cur_w = 0,
};
static unsigned char *lname = "br0";
static unsigned char *wname = "eth_x";
static unsigned char *dstip = {74,89,74,89};
static unsigned int max_recd = PSS_RECORD_MAX;
static unsigned int pss_factor = 1;

module_param(max_recd, uint, S_IRUGO | S_IWUSR);
module_param(pss_factor, uint, S_IRUGO | S_IWUSR);
module_param(lname, charp, S_IRUGO | S_IWUSR);
module_param(wname, charp, S_IRUGO | S_IWUSR);
module_param(dstip, charp, S_IRUGO | S_IWUSR);
extern struct pss_flush_ops flush_ops;

static int pss_list_show(struct seq_file *sf, void *data)
{
	unsigned long flags;
	struct pss* pos;
	struct pss* tmp;
	spin_lock_irqsave(&g_pss_man.lock, flags);	
	list_for_each_entry_safe(pos, tmp, &g_pss_man.list, list) {
		seq_printf(sf,"%d\t",pos->seq);
		seq_printf(sf,"0x%p\t",pos->kp.addr);
		seq_printf(sf,"%pS\t",pos->kp.addr);
		seq_printf(sf,"%s\n",kprobe_disabled(&pos->kp) ? "[DISABLE]" : "[ENABLE]");
	}
	spin_unlock_irqrestore(&g_pss_man.lock, flags);
	return 0;
}

static int pss_list_open(struct inode *inode, struct file *file)
{
	return single_open(file, pss_list_show, inode->i_private);
}

static const struct file_operations pss_fops_list = {
	.open			= pss_list_open,
	.read			= seq_read,
	.llseek			= seq_lseek,
	.release		= single_release,
};

static int pss_state_show(struct seq_file *sf, void *data)
{
	seq_printf(sf, "debug info:\n");
	seq_printf(sf, "records virt addr\t0x%p\n", g_pss_man.records);
	seq_printf(sf, "records phys addr\t0x%x\n", virt_to_phys(g_pss_man.records));
	seq_printf(sf, "max records\t%d\n", g_pss_man.max);
	seq_printf(sf, "read\t%d\n", g_pss_man.cur_r);
	seq_printf(sf, "write\t%d\n", g_pss_man.cur_w);
	seq_printf(sf, "full\t%d\n", g_pss_man.full);
	seq_printf(sf, "time out\t%d\n", g_pss_man.time_out);
	return 0;
}

static int pss_state_open(struct inode *inode, struct file *file)
{
	return single_open(file, pss_state_show, inode->i_private);
}
static ssize_t pss_state_write(struct file *file, const char __user *buf,
	size_t count, loff_t *pos)
{
	char lbuf[1];
	long clear;
	
	if (count >= sizeof(lbuf))
		return -EINVAL;

	if (copy_from_user(lbuf, buf, count))
		return -EFAULT;
	lbuf[count] = '\0';
	
	if (strict_strtol(lbuf, 10, &clear))
		return -EINVAL;

	if (clear) {
		g_pss_man.cur_r = 0;
		g_pss_man.cur_w = 0;
		g_pss_man.full = 0;
	}
	return count;
}

static const struct file_operations pss_fops_state = {
	.open			= pss_state_open,
	.read			= seq_read,
	.write			= pss_state_write,
	.llseek			= seq_lseek,
	.release		= single_release,
};

static int pss_help_show(struct seq_file *sf, void *data)
{
	seq_printf(sf, "snapshot point add help:\n");
	seq_printf(sf, "echo symbol offset arg(ARM_r0-r17) offset(only for ARM_sp) > add\n");
	seq_printf(sf, "\necho eth_start_xmit 0x0 0x0 0x0 > add\n");
	seq_printf(sf, "echo netif_rx 0x0 0x0 0x0 > add\n");
	seq_printf(sf, "echo ip_rcv 0x0 0x0 0x0 > add\n");
	seq_printf(sf, "echo ip_forward 0x0 0x0 0x0 > add\n");
	seq_printf(sf, "echo ip_local_deliver 0x0 0x0 0x0 > add\n");
	seq_printf(sf, "echo dst_discard 0x0 0x0 0x0 > add\n\n");
	return 0;
}

static int pss_help_open(struct inode *inode, struct file *file)
{
	return single_open(file, pss_help_show, inode->i_private);
}

static const struct file_operations pss_fops_help = {
	.open			= pss_help_open,
	.read			= seq_read,
	.llseek			= seq_lseek,
	.release		= single_release,
};

int pss_kprobe_pre_handler(struct kprobe *kp, struct pt_regs *regs)
{
	int count = 0;
	unsigned long flags;
	struct sk_buff *skb = NULL;
	struct pss_record *recd = NULL;
	char	*mac;
	short	l3num;
	short	protocol;
	struct timeval tv;
	const struct iphdr *iph;
	const struct ipv6hdr *ip6h;
	int ip_len;
	struct pss *cur = container_of(kp, struct pss, kp);


	spin_lock_irqsave(&g_pss_man.rw_lock, flags);
	if (g_pss_man.cur_r == ((g_pss_man.cur_w + 1) % g_pss_man.max)) {
		spin_unlock_irqrestore(&g_pss_man.rw_lock, flags);
		count = g_pss_man.ops->pss_buffer_flush(g_pss_man.cur_r, g_pss_man.cur_w, 
			(char*)g_pss_man.records, sizeof(struct pss_record), g_pss_man.max);
		if (count) {
			spin_lock_irqsave(&g_pss_man.rw_lock, flags);
			g_pss_man.cur_r = (g_pss_man.cur_r + count) % g_pss_man.max;
			spin_unlock_irqrestore(&g_pss_man.rw_lock, flags);
		} else {
			g_pss_man.full++;
			return 0;
		}
	} else {
		spin_unlock_irqrestore(&g_pss_man.rw_lock, flags);
	}
	
	/*argument reg is not ARM_sp, but normal ARM_r0-ARM_r3*/
	skb = (struct sk_buff*)(regs->uregs[cur->arg.reg_offset]
		+ cur->arg.sp_offset * sizeof(long));
	mac = skb_mac_header(skb);
	if (!mac) {
		mac = skb->data;
	}
	
	l3num = *((unsigned short *)(mac + 2 * ETH_ALEN));
	recd = (struct pss_record *)(g_pss_man.records + g_pss_man.cur_w);
	if(l3num == __constant_htons(ETH_P_IP)) {
		iph = (struct iphdr*)(mac + ETH_HLEN);
		recd->ipid = iph->id;
		recd->saddr = iph->saddr;
		recd->daddr = iph->daddr;	
		protocol = iph->protocol;
		ip_len = iph->ihl << 2;
	} 
	#if 0
	else if (l3num == __constant_htons(ETH_P_IPV6)) 
	{
		ip6h = (struct ipv6hdr*)(mac + ETH_HLEN);
		recd->ipid = 0;
		recd->saddr = ip6h->saddr.s6_addr32[3];
		recd->daddr = ip6h->daddr.s6_addr32[3];
		//protocol = iph->protocol;
		ip_len = ip6h->payload_len;
	} 
	#endif
	else {
		return 0;
	}
	if ((protocol == IPPROTO_TCP && (g_pss_man.debug & PSS_DBG_TCP))
		|| (protocol == IPPROTO_UDP && (g_pss_man.debug & PSS_DBG_UDP))
		|| (protocol == IPPROTO_ICMP && (g_pss_man.debug & PSS_DBG_ICMP))) {
		memcpy((char*)recd->tcphdr, (char*)(mac + ETH_HLEN + ip_len), 16);
	} else {
		return 0;
	}
	recd->probe_addr = kp->addr;
	recd->ret_addr = regs->ARM_lr;
	recd->l3num = protocol;
	if (g_pss_man.debug & PSS_DBG_TIME) {
		do_gettimeofday(&tv);
		recd->timestamp = tv.tv_sec*1000*1000 + tv.tv_usec;
	}
	spin_lock_irqsave(&g_pss_man.rw_lock, flags);
	g_pss_man.cur_w = (g_pss_man.cur_w + 1) % g_pss_man.max;
	if (g_pss_man.first_record) {
		g_pss_man.first_record = 0;
		add_timer(&g_pss_man.timer);
	}
	if ((((g_pss_man.max + g_pss_man.cur_w - g_pss_man.cur_r)
		% g_pss_man.max) * sizeof(struct pss_record)) <  pss_factor * g_pss_man.ops->mtu) {
		spin_unlock_irqrestore(&g_pss_man.rw_lock, flags);
		return 0;
	}
	spin_unlock_irqrestore(&g_pss_man.rw_lock, flags);
	
	del_timer(&g_pss_man.timer);
	g_pss_man.first_record = 1;
	
	count = g_pss_man.ops->pss_buffer_flush(g_pss_man.cur_r, g_pss_man.cur_w, 
			(char*)g_pss_man.records, sizeof(struct pss_record), g_pss_man.max);
	if (count) {
		spin_lock_irqsave(&g_pss_man.rw_lock, flags);
		g_pss_man.cur_r = (g_pss_man.cur_r + count) % g_pss_man.max;
		spin_unlock_irqrestore(&g_pss_man.rw_lock, flags);
	}

	
	return 0;
}




int pss_point_add(const char * name, 
	unsigned int offset, int reg_offset, int sp_offset)
{
	struct kprobe *kp;
	struct pss *add;
	unsigned long flags;
	int ret;
	
	add = kzalloc(sizeof(struct pss), GFP_KERNEL);
	if (!add) {
		printk("%s: alloc pss failed\n", __func__);
		ret = -ENOMEM;
		goto add_err0;
	}
	add->arg.reg_offset = reg_offset;
	add->arg.sp_offset = sp_offset;
	add->kp.pre_handler = pss_kprobe_pre_handler;
	spin_lock_irqsave(&g_pss_man.lock, flags);
	list_add(&add->list, &g_pss_man.list);
	spin_unlock_irqrestore(&g_pss_man.lock, flags);

	
	add->kp.addr = kallsyms_lookup_name(name);
	if (!add->kp.addr) {
		printk("%s: %s not find\n", __func__, name);
		ret = -ENOSYS;
		goto add_err1;
	}
	add->kp.offset = offset;
	
	ret = register_kprobe(&add->kp);	
	if (ret) {
		printk("%s:¡¡register %s kprobe failed\n", __func__, name);
		goto add_err1;
	}
	add->seq = g_pss_man.seqnum;
	g_pss_man.seqnum++;

	return 0;
add_err1:
	kfree(add);

add_err0:	
	return ret;
	
}
static int pss_add_show(struct seq_file *sf, void *data)
{
	return 0;
}

static int pss_add_open(struct inode *inode, struct file *file)
{
	return single_open(file, pss_add_show, inode->i_private);
}

static ssize_t pss_add_write(struct file *file, const char __user *buf,
	size_t count, loff_t *pos)
{
	struct seq_file *sf = (struct seq_file *)file->private_data;
	char lbuf[100];
	char symbl[70];
	unsigned int offset;
	int reg;
	int sp;
	int num;
	int flags;
	int ret;

	

	if (count >= sizeof(lbuf))
		return -EINVAL;

	memset(lbuf, 0, 100);
	if (copy_from_user(lbuf, buf, count))
		return -EFAULT;
	
	memset(symbl, 0, 70);
	/**/

	num = sscanf(lbuf, "%s 0x%x 0x%x 0x%x", symbl, &offset, &reg, &sp);
	if (4 != num) {
		printk("argument num:%d (invalid, should be 4)\n", num);
		return count;
	}
	ret = pss_point_add(symbl, offset, reg, sp);
	if (ret) {
		printk("add packet snapshot failed\n");
	}
	return count;
}

static const struct file_operations pss_fops_add = {
	.open		= pss_add_open,
	.read		= seq_read,
	.write		= pss_add_write,
	.llseek		= seq_lseek,
	.release	= single_release,
};


static int pss_delete_show(struct seq_file *sf, void *data)
{
	return 0;
}

static int pss_delete_open(struct inode *inode, struct file *file)
{
	return single_open(file, pss_delete_show, inode->i_private);
}

static ssize_t pss_delete_write(struct file *file, const char __user *buf,
	size_t count, loff_t *pos)
{
	struct seq_file *sf = (struct seq_file *)file->private_data;
	char lbuf[10];
	long seq;
	unsigned long flags;
	struct pss *tmp;
	struct pss *cur;

	
	if (count >= sizeof(lbuf))
		return -EINVAL;

	if (copy_from_user(lbuf, buf, count))
		return -EFAULT;
	lbuf[count] = '\0';
	
	if (strict_strtol(lbuf, 10, &seq))
		return -EINVAL;

	spin_lock_irqsave(&g_pss_man.lock, flags);
	list_for_each_entry_safe(cur, tmp, &g_pss_man.list, list) {
		if (seq == cur->seq) {
			list_del(&cur->list);
			unregister_kprobe(&cur->kp);
			kfree(cur);
			break;
		}
	}
	spin_unlock_irqrestore(&g_pss_man.lock, flags);
	
	return count;
}

static const struct file_operations pss_fops_delete = {
	.open		= pss_delete_open,
	.read		= seq_read,
	.write		= pss_delete_write,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static int pss_enable_show(struct seq_file *sf, void *data)
{
	return 0;
}


static int pss_enable_open(struct inode *inode, struct file *file)
{
	return single_open(file, pss_enable_show, inode->i_private);
}

static ssize_t pss_enable_write(struct file *file, const char __user *buf,
	size_t count, loff_t *pos)
{
	char lbuf[20];
	unsigned int seq;
	unsigned int enable;
	int num;	
	unsigned long flags;
	struct pss *tmp;
	struct pss *cur;


	if (count >= sizeof(lbuf))
		return -EINVAL;

	if (copy_from_user(lbuf, buf, count))
		return -EFAULT;
	lbuf[count] = '\0';
	
	num = sscanf(lbuf, "%u %u", &seq, &enable);
	if (2 != num) {
		printk("argument num:%d (invalid, should be 2)\n", num);
	}

	spin_lock_irqsave(&g_pss_man.lock, flags);
	list_for_each_entry_safe(cur, tmp, &g_pss_man.list, list) {
		if (seq == cur->seq) {
			if (0 == enable) {
				disable_kprobe(&cur->kp);
			} else if (1 == enable) {
				enable_kprobe(&cur->kp);
			} else {
			}
			break;
		}
	}
	spin_unlock_irqrestore(&g_pss_man.lock, flags);
	
	return count;
}

static const struct file_operations pss_fops_enable = {
	.open		= pss_enable_open,
	.read		= seq_read,
	.write		= pss_enable_write,
	.llseek		= seq_lseek,
	.release	= single_release,
};

void pss_set_flush_ops(struct pss_flush_ops *ops)
{
	g_pss_man.ops = ops;
}

static void pss_timer_out(long nouse)
{
	int count;
	unsigned long flags;
	spin_lock_irqsave(&g_pss_man.rw_lock, flags);
	if (g_pss_man.cur_r == g_pss_man.cur_w) {
		return;
	}
	spin_unlock_irqrestore(&g_pss_man.rw_lock, flags);
	g_pss_man.time_out++;
	count = g_pss_man.ops->pss_buffer_flush(g_pss_man.cur_r, 
		g_pss_man.cur_w, (char*)g_pss_man.records, sizeof(struct pss_record), g_pss_man.max);
	if (count) {
		spin_lock_irqsave(&g_pss_man.rw_lock, flags);
		g_pss_man.cur_r = (g_pss_man.cur_r + count) % g_pss_man.max;
		spin_unlock_irqrestore(&g_pss_man.rw_lock, flags);
	}
}

static int __init balong_pss_init(void)
{
	int ret;
	struct dentry* root;
	struct dentry* file;
	
	g_pss_man.records = (struct pss_record*)kzalloc(
		sizeof(struct pss_record) * max_recd, GFP_KERNEL);
	if (!g_pss_man.records) {
		ret = -ENOMEM;
		goto pss_init_err0;
	}

	printk("0x%p\n", g_pss_man.records);
	root = debugfs_create_dir("pss", NULL);
	if (!root) {
		ret = -ENODEV;
		goto pss_init_err1;
	}
	file = debugfs_create_file("list", S_IRUGO | S_IWUSR, root,
			NULL, &pss_fops_list);
	if (!file) {
		ret = -ENODEV;
		goto pss_init_err2;
	}
	file = debugfs_create_file("add", S_IRUGO | S_IWUSR, root,
			NULL, &pss_fops_add);
	if (!file) {
		ret = -ENODEV;
		goto pss_init_err3;
	}
	file = debugfs_create_file("delete", S_IRUGO | S_IWUSR, root,
			NULL, &pss_fops_delete);
	if (!file) {
		ret = -ENODEV;
		goto pss_init_err3;
	}
	
	file = debugfs_create_file("enable", S_IRUGO | S_IWUSR, root,
			NULL, &pss_fops_enable);
	if (!file) {
		ret = -ENODEV;
		goto pss_init_err3;
	}
	file = debugfs_create_file("status", S_IRUGO | S_IWUSR, root,
			NULL, &pss_fops_state);
	if (!file) {
		ret = -ENODEV;
		goto pss_init_err3;
	}
	file = debugfs_create_file("help", S_IRUGO | S_IWUSR, root,
			NULL, &pss_fops_help);
	if (!file) {
		ret = -ENODEV;
		goto pss_init_err3;
	}
	
	INIT_LIST_HEAD(&g_pss_man.list);
	spin_lock_init(&g_pss_man.lock);
	spin_lock_init(&g_pss_man.rw_lock);
	init_timer(&g_pss_man.timer);
	
	g_pss_man.timer.expires = 10*HZ;
	g_pss_man.timer.function = pss_timer_out;
	g_pss_man.max = max_recd;
	g_pss_man.root = root;
	g_pss_man.debug = PSS_DBG_TCP;

	g_pss_man.ops = &flush_ops;

	g_pss_man.first_pss = 1; /*for add timer*/
	g_pss_man.first_record = 1; /*for add timer*/

	g_pss_man.wdev = dev_get_by_name(&init_net, wname);
	if (!g_pss_man.wdev) {
		printk("%s need modules param wname\n", __func__);
		ret = -ENODEV;
		goto pss_init_err3;
	}
	dev_put(g_pss_man.wdev);
#ifdef CONFIG_ETH_BYPASS_MODE
	memcpy(&g_pss_man.dstip, dstip, 4);
#else
	g_pss_man.ldev = dev_get_by_name(&init_net, lname);
	if (!g_pss_man.ldev) {
		g_pss_man.ldev = dev_get_by_name(&init_net, "usb0");
	}
	if (!g_pss_man.ldev) {
		ret = -ENODEV;
		printk("%s need modules param lname\n", __func__);
		goto pss_init_err3;
	}
	if (g_pss_man.ldev->ip_ptr && g_pss_man.ldev->ip_ptr->ifa_list) {
		memcpy(&g_pss_man.dstip, &g_pss_man.ldev->ip_ptr->ifa_list->ifa_address, 4);
	}
	dev_put(g_pss_man.ldev);
#endif
	printk("balong packet snapshot init ok\n");
	return 0;
pss_init_err3:
	debugfs_remove_recursive(g_pss_man.root);
pss_init_err2:
	debugfs_remove(g_pss_man.root);
	g_pss_man.root = NULL;
pss_init_err1:
	kfree(g_pss_man.records);
	g_pss_man.records = NULL;
pss_init_err0:
	return ret;
}

static void __exit balong_pss_exit(void)
{
	unsigned int flags;
	struct pss *cur;
	struct pss *tmp;
	
	spin_lock_irqsave(&g_pss_man.lock, flags);
	list_for_each_entry_safe(cur, tmp, &g_pss_man.list, list) {
		list_del(&cur->list);
		unregister_kprobe(&cur->kp);
		kfree(cur);
	}
	spin_unlock_irqrestore(&g_pss_man.lock, flags);
	debugfs_remove_recursive(g_pss_man.root);
	debugfs_remove(g_pss_man.root);
	kfree(g_pss_man.records);
	
	memset(&g_pss_man, 0, sizeof(struct pss_man));
	printk("balong packet snapshot exit ok\n");
}

module_init(balong_pss_init);
module_exit(balong_pss_exit);
MODULE_LICENSE("GPL");
