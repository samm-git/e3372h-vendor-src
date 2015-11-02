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

/*#define MODULE_MAIN          NAND*/
#define SUB_MODULE              NANDC_TRACE_PTABLE

#include "ptable_com.h"
#include "ptable_inc.h"
#include <boot/flash.h>
#include <boot/boot.h>

struct ptentry_ex pt_find;

/******************************************************************************************
 * FUNC NAME:
 * @flash_find_ptn() - external API:
 *
 * PARAMETER:
 * @name   -[input] which partition to be find for fastboot
 *
 * RETURN:
 *    partition info of struct ptentry
 *
 * DESCRIPTION:
 *     find fastboot partition informatin from flash partition table in AXI memory
 *
 * CALL FUNC:
 *****************************************************************************************/
struct ptentry_ex *flash_find_ptnex(const char *name)
{
    struct ST_PART_TBL *part = NULL;

    part = (struct ST_PART_TBL *)SHM_MEM_PTABLE_ADDR;

    while(0 != strcmp(PTABLE_END_STR, part->name))
    {
        if((0 == strcmp(name ,  part->name)) && (DATA_VALID == ptable_get_validity(part)))
        {
            pt_find.ptn.flags = 0;
            pt_find.ptn.length = part->capacity;
            pt_find.ptn.start = part->offset;
            pt_find.ptn.property = part->property;
			pt_find.entry = part->entry;
			pt_find.loadaddr = part->loadaddr;
			pt_find.image = part->image;
			pt_find.property = part->property;

            /* coverity[secure_coding] */
            strcpy(pt_find.ptn.name, part->name);

            return &pt_find;
        }

        part++;
    }

    return 0;
}

struct ptentry *flash_find_ptn(const char *name)
{
	struct ptentry_ex * ex = flash_find_ptnex(name);

	if(ex)
		return &ex->ptn;
	else
		return 0;
}

#ifdef __cplusplus
}
#endif




