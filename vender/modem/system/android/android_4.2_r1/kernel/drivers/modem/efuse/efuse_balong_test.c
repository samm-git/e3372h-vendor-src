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
#include <linux/types.h>
#include <linux/efuse/efuse_balong.h>

#define EFUSE_OK               (0)
#define EFUSE_ERROR            (-1)

int test_bsp_efuse_read_para_error(void);
int test_bsp_efuse_write_para_error(void);
int bsp_efuse_write_zero(void);
int efuse_test(void);

int test_bsp_efuse_read_para_error(void)
{
    if(EFUSE_ERROR == bsp_efuse_read(NULL, 0, 1))
    {
        return EFUSE_OK;
    }

    return EFUSE_ERROR;
}

int test_bsp_efuse_write_para_error(void)
{
    if(EFUSE_ERROR == bsp_efuse_write(NULL, 0, 1))
    {
        return EFUSE_OK;
    }

    return EFUSE_ERROR;
}

int bsp_efuse_write_zero(void)
{
    u32 value = 0;
    if(EFUSE_ERROR == bsp_efuse_write(&value, 0, 1))
    {
        return EFUSE_ERROR;
    }

    return EFUSE_OK;
}

extern void bsp_efuse_show(void);
int bsp_efuse_test(void)
{
    test_bsp_efuse_read_para_error();
    test_bsp_efuse_write_para_error();
    
	bsp_efuse_show();
    bsp_efuse_write_zero();
	
	return EFUSE_OK;
}

