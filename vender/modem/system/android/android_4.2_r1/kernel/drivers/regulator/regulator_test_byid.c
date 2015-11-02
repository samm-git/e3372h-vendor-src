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
#ifdef __cplusplus
extern "C"
{
#endif

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/err.h>
#include <linux/mutex.h>
#include <linux/suspend.h>
#include <linux/delay.h>
#include <linux/regulator/consumer.h>
#include <linux/regulator/driver.h>
#include <linux/regulator/machine.h>
#include <regulator_balong.h>

struct regulator {
	struct device *dev;
	struct list_head list;
	int uA_load;
	int min_uV;
	int max_uV;
	char *supply_name;
	struct device_attribute dev_attr;
	struct regulator_dev *rdev;
	struct dentry *debugfs;
};
extern struct regulator_dev *get_regulator_by_id(int id);


s32 g_regu_pr_err_flag = 0;
/*是否打印该ID信息*/
void regu_dbg_flag(u32 flag)
{
	g_regu_pr_err_flag = flag;
}

void regulator_print_status(int id, int type)
{
	struct regulator *regulator = ERR_PTR(-EINVAL);
	int ret = 0;
	id = (type << REGULATOR_SHIFT) | id;
	regulator = kmalloc(sizeof(struct regulator), GFP_KERNEL);
	if (!regulator)
	{
		regu_pr_err("regulator malloc failed---->id is :%d\n", id);
		return;
	}
	regulator->rdev = get_regulator_by_id(id);
	if (IS_ERR(regulator->rdev))
	{
		regu_pr_err("get regulator failed---->id is :%d\n", id);
		return;
	}
	regu_pr_err("******************regulator id %d's status*******************\n\r", id);
	regu_pr_err("regulator id------------>%d\n", regulator->rdev->desc->id);
	regu_pr_err("regulator name---------->%s\n", regulator->rdev->desc->name);
	regu_pr_err("regulator use_count----->%d\n", regulator->rdev->use_count);
	ret = regulator_is_enabled(regulator);
	regu_pr_err("regulator status-------->%d\n", ret);
	regu_pr_err("regulator n_vol--------->%d\n", regulator->rdev->desc->n_voltages);
	ret = regulator_get_voltage(regulator);
	regu_pr_err("regulator voltage------->%d\n", ret);
	regu_pr_err("******************regulator status end*******************\n\r");
	kfree(regulator);
}


void regulator_status(int id, int type)
{
	struct regulator *regulator = ERR_PTR(-EINVAL);
	int status = 0, voltage = 0;
	id = (type << REGULATOR_SHIFT) | id;
	regulator = kmalloc(sizeof(struct regulator), GFP_KERNEL);
	if (!regulator)
	{
		regu_pr_err("regulator malloc failed---->id is :%d\n", id);
		return;
	}
	regulator->rdev = get_regulator_by_id(id);
	if (IS_ERR(regulator->rdev))
	{
		regu_pr_info("get regulator failed---->id is :%d\n", id);
		return;
	}
	status = regulator_is_enabled(regulator);
	voltage = regulator_get_voltage(regulator);

	pr_err("id\t name\t\t use_count\t status\t voltage\t\n");
	pr_err("%-d\t %-s\t %-d\t\t %-d\t %-d\t\n", regulator->rdev->desc->id, regulator->rdev->desc->name, regulator->rdev->use_count,
											status, voltage);
	kfree(regulator);
}
void regulator_get_current_status(void)
{
	int id = 0;
	regu_pr_err("******************regulator status*******************\n", id);
	for(id = HI6451_BUCK1; id <= HI6451_DR3; id++)
	{
		regulator_status(id, REGULATOR_TYPE_HI6451);
	}
	for(id = HI6551_BUCK1; id <= HI6551_LVS10; id++)
	{
		regulator_status(id, REGULATOR_TYPE_HI6551);
	}
	for(id = P531_MTCMOS1; id <= P531_MTCMOS10; id++)
	{
		regulator_status(id, REGULATOR_TYPE_P531_MTCMOS);
	}
	for(id = V7R2_MTCMOS1; id <= V7R2_MTCMOS13; id++)
	{
		regulator_status(id, REGULATOR_TYPE_V7R2_MTCMOS);
	}
}


/*
	返回为0 ----未使能
	返回大于0-----已使能
*/
int  regulator_is_enabled_byid(int id, int type)
{
	struct regulator *regulator = ERR_PTR(-EINVAL);
	int ret;
	id = (type << REGULATOR_SHIFT) | id;
	regulator = kmalloc(sizeof(struct regulator), GFP_KERNEL);
	if (!regulator)
	{
		regu_pr_err("regulator malloc failed---->id is :%d\n", id);
		ret = -EINVAL;
		goto out;
	}
	regulator->rdev = get_regulator_by_id(id);
	if (IS_ERR(regulator->rdev))
	{
		regu_pr_err("get regulator failed---->id is :%d\n", id);
		ret = -EINVAL;
		goto out;
	}
	ret = regulator_is_enabled(regulator);

		regu_pr_err("ret>>>>>>>>>>>> %d <<<<<<<<<<<<<<\n", ret);

out:
	if (regulator)
	{
		kfree(regulator);
	}
	return ret;
}
/*
	返回0 ------使能成功
	返回非0-----使能失败
*/
int regulator_enable_byid(int id, int type)
{
	struct regulator *regulator = ERR_PTR(-EINVAL);
	int ret;
	id = (type << REGULATOR_SHIFT) | id;
	regulator = kmalloc(sizeof(struct regulator), GFP_KERNEL);
	if (!regulator)
	{
		regu_pr_err("regulator malloc failed---->id is :%d\n", id);
		ret = -EINVAL;
		goto out;
	}

	regulator->rdev= get_regulator_by_id(id);
	if (IS_ERR(regulator->rdev))
	{
		regu_pr_err("get regulator failed---->id is :%d\n", id);
		ret = -EINVAL;
		goto out;
	}
	ret = regulator_enable(regulator);
	if (ret)
	{
		regu_pr_err("enable regulator failed---->id is :%d\n", id);
	}
	regu_pr_err("ret>>>>>>>>>>>> %d <<<<<<<<<<<<<<\n", ret);
	
out:
	if (regulator)
	{
		kfree(regulator);
	}
	return ret;
}

/*
	 返回0 ------- 去使能成功
	 返回非0 -------去使能失败
*/
int regulator_forcedisable_byid(int id, int type)
{
	struct regulator *regulator = ERR_PTR(-EINVAL);
	int ret;
	id = (type << REGULATOR_SHIFT) | id;
	regulator = kmalloc(sizeof(struct regulator), GFP_KERNEL);
	if (!regulator)
	{
		regu_pr_err("regulator malloc failed---->id is :%d\n", id);
		ret = -EINVAL;
		goto out;
	}

	regulator->rdev= get_regulator_by_id(id);
	if (IS_ERR(regulator->rdev))
	{
		regu_pr_err("get regulator failed---->id is :%d\n", id);
		ret = -EINVAL;
		goto out;
	}
	ret = regulator_force_disable(regulator);
	if (ret)
	{
		regu_pr_err("disable regulator failed---->id is :%d\n", id);
	}
	regu_pr_err("ret>>>>>>>>>>>> %d <<<<<<<<<<<<<<\n", ret);
	
out:
	if (regulator)
	{
		kfree(regulator);
	}
	return ret;
}
int regulator_disable_byid(int id, int type)
{
	struct regulator *regulator = ERR_PTR(-EINVAL);
	int ret;
	id = (type << REGULATOR_SHIFT) | id;
	regulator = kmalloc(sizeof(struct regulator), GFP_KERNEL);
	if (!regulator)
	{
		regu_pr_err("regulator malloc failed---->id is :%d\n", id);
		ret = -EINVAL;
		goto out;
	}

	regulator->rdev= get_regulator_by_id(id);
	if (IS_ERR(regulator->rdev))
	{
		regu_pr_err("get regulator failed---->id is :%d\n", id);
		ret = -EINVAL;
		goto out;
	}
	ret = regulator_disable(regulator);
	if (ret)
	{
		regu_pr_err("disable regulator failed---->id is :%d\n", id);
	}
	regu_pr_err("ret>>>>>>>>>>>> %d <<<<<<<<<<<<<<\n", ret);

out:
	if (regulator)
	{
		kfree(regulator);
	}
	return ret;
}
int regulator_get_voltage_byid(int id, int type)
{
	struct regulator *regulator = ERR_PTR(-EINVAL);
	int ret;
	id = (type << REGULATOR_SHIFT) | id;
	regulator = kmalloc(sizeof(struct regulator), GFP_KERNEL);
	if (!regulator)
	{
		regu_pr_err("regulator malloc failed---->id is :%d\n", id);
		ret = -EINVAL;
		goto out;
	}

	regulator->rdev= get_regulator_by_id(id);
	if (IS_ERR(regulator->rdev))
	{
		regu_pr_err("get regulator failed---->id is :%d\n", id);
		ret = -EINVAL;
		goto out;
	}
	ret = regulator_get_voltage(regulator);
	if (!ret)
	{
		regu_pr_err("get regulator voltage failed---->id is :%d\n", id);
	}
	regu_pr_err("ret>>>>>>>>>>>> %d <<<<<<<<<<<<<<\n", ret);
	
out:
	if (regulator)
	{
		kfree(regulator);
	}
	return ret;
}

int regulator_set_voltage_byid(int id, int min_uV, int max_uV, int type)
{
	struct regulator *regulator = ERR_PTR(-EINVAL);
	int ret;
	id = (type << REGULATOR_SHIFT) | id;
	regulator = kmalloc(sizeof(struct regulator), GFP_KERNEL);
	if (!regulator)
	{
		regu_pr_err("regulator malloc failed---->id is :%d\n", id);
		ret = -EINVAL;
		goto out;
	}

	regulator->rdev= get_regulator_by_id(id);
	if (IS_ERR(regulator->rdev))
	{
		regu_pr_err("get regulator failed---->id is :%d\n", id);
		ret = -EINVAL;
		goto out;
	}
	if ((min_uV > max_uV) || (min_uV < 0) || max_uV < 0)
	{
		regu_pr_err("regulator uV is error---->id is :%d,---->min_uV is : %d,---->max_uV is : %d\n", id, min_uV, max_uV);
		ret = -EINVAL;
		goto out;
	}
	ret = regulator_set_voltage(regulator, min_uV, max_uV);
	if (ret < 0)
	{
		regu_pr_err("set regulator voltage failed---->id is :%d\n", id);
	}
	regu_pr_err("ret>>>>>>>>>>>> %d <<<<<<<<<<<<<<\n", ret);
	
out:
	if (regulator)
	{
		kfree(regulator);
	}
	return ret;
}
#ifdef __cplusplus
}
#endif
