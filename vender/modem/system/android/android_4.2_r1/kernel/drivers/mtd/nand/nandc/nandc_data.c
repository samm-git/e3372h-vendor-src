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
/*lint --e{409,601,745,49,718,746,56,830,601,516}*/
#ifdef __cplusplus
extern "C"
{
#endif

#include "nandc_inc.h"

 /*******************************************************************************
 * FUNC NAME:
 * nandc_data_transmit_page() -
 *
 * PARAMETER:
 * @host - [input]the main structure.
 * @pagemap - [input]data map in one page
 * @times - [input]the times it needs to transfer data between nand controller buffer
 *                 and ram buffer.because Hisi nand controller buffer is 2k~3k large,when
 *                 we do operations to nand flash chip with 4k page,we need 2 times data
 *                 transmission between nand controller buffer and nand flash chip,as the
 *                 same,when we do operation to nand flash chip with 8k page,we need 3 times
 *                 data transmission between nand controller buffer and nand flash chip.
 * @access - [input] NAND_CMD_READSTART : reading operation to nand flash chip.
 *                   NAND_CMD_PAGEPROG  : writing operation to nand flash chip.
 *
 * DESCRIPTION:
 * This functions transfers data between ram and nand controller buffer.
 * reading operation : from nand controller buffer to ram buffer.
 *   nand flash chip           nand controller buffer     ram buffer
 *   ----------------          ----------------         ----------------
 *  |                |  ----> |  host->bufbase | --->  | host->databuf  |
 *   ----------------          ----------------         ----------------
 *
 * writing operation : from ram buffer to nand controllers buffer.
 *   nand flash chip           nand controller buffer     ram buffer
 *   ----------------          ----------------         ----------------
 *  |                |  <---- |  host->bufbase | <---  | host->databuf  |
 *   ----------------          ----------------         ----------------
 * for __KERNEL__ platform :host->databuf == host->bufbase
 * for other platform eg : __FASTBOOT__ ,__VXWORKS__,__RVDS__
 * host->databuf != host->bufbase
 *
 * CALL FUNC:
 * () -
 *
 ********************************************************************************/
u32 nandc_data_transmit_page(struct nandc_host *host, struct nandc_pagemap *pagemap, u32 times, u32 access)
{/*lint !e578 */
    u32 bufoffset;
    u32 ramoffset;
    u32 copylen;
    u32 i;

    if(host->databuf == host->bufbase)
    {
        return NANDC_OK;
    }

    /*copy page data*/
    copylen = pagemap[times].data.lens;

    if((0 != copylen) && (NANDC_NULL != host->databuf))
    {
		bufoffset = pagemap[times].data.addr;
        i = 0;
        ramoffset = 0;
        while(i < times)
        {
            ramoffset += pagemap[i].data.lens;
            i++;
        }

        if(NAND_CMD_READSTART == access)
        {
            memcpy((void*)((u32)host->databuf + ramoffset), (const void*)((u32)(host->bufbase) + bufoffset), copylen);
        }
        else /*NAND_CMD_PAGEPROG*/
        {
            memcpy((void*)((u32)(host->bufbase) + bufoffset),(const void*)((u32)host->databuf + ramoffset),  copylen);
        }
    }

    /*copy oob data*/
    copylen     =   pagemap[times].oob.lens;
	bufoffset   =   pagemap[times].oob.addr ;

    if((0 != copylen)&&(NANDC_NULL != host->oobbuf))
    {
        i = 0;
        ramoffset = 0;
        while(i < times)
        {
            ramoffset += pagemap[i].oob.lens;
            i++;
        }

        if(NAND_CMD_READSTART == access)
        {
            memcpy((void*)((u32)host->oobbuf + ramoffset), (const void*)((u32)host->bufbase + bufoffset), copylen);
#ifdef __KERNEL__
			memcpy((void*)((u32)host->databuf + host->nandchip->spec.pagesize), (const void*)((u32)host->bufbase + bufoffset), copylen);
#endif
		}
        else /*NAND_CMD_PAGEPROG*/
        {
#ifdef __KERNEL__
			/* change for 4K Nand */
            memcpy(host->oobbuf + ramoffset, (const void*)((u32)host->databuf + host->nandchip->spec.pagesize), copylen);
#endif
            memcpy((void*)((u32)host->bufbase + bufoffset), (const void*)((u32)host->oobbuf + ramoffset), copylen);
        }
    }
    else if((0 != copylen)&&(NAND_CMD_READSTART != access))
    {
        /*纯数据写(不填充spare区 )模式下确保坏块标志不为0*/
        memset((void*)((u32)host->bufbase + bufoffset), 0xff , copylen);

    }

    return NANDC_OK;
}

 /*******************************************************************************
 * FUNC NAME:
 * nandc_data_transmit_raw() -
 *
 * PARAMETER:
 * @host - [input]the main structure.
 * @datasize - [input]the length of raw data and oob data.
 * @bufoffset - [input]offset form host->databuf's base address.
 * @access - [input] NAND_CMD_READSTART : reading operation to nand flash chip.
 *                   NAND_CMD_PAGEPROG  : writing operation to nand flash chip.
 *
 * DESCRIPTION:
 * This functions transfers data between ram and nand controller buffer.
 * reading operation : from nand controller buffer to ram buffer.
 *   nand flash chip           nand controller buffer     ram buffer
 *   ----------------          ----------------         ----------------
 *  |                |  ----> |  host->bufbase | --->  | host->databuf  |
 *   ----------------          ----------------         ----------------
 *
 * writing operation : from ram buffer to nand controllers buffer.
 *   nand flash chip           nand controller buffer     ram buffer
 *   ----------------          ----------------         ----------------
 *  |                |  <---- |  host->bufbase | <---  | host->databuf  |
 *   ----------------          ----------------         ----------------
 * for __KERNEL__ platform :host->databuf == host->bufbase
 * for other platform eg : __FASTBOOT__ ,__VXWORKS__,__RVDS__,__BOOTLOADER__
 * host->databuf != host->bufbase
 *
 * CALL FUNC:
 * () -
 *
 ********************************************************************************/
u32 nandc_data_transmit_raw(struct nandc_host *host, u32 datasize, u32 bufoffset, u32 access)
{/*lint !e578 */
    if(host->databuf == host->bufbase)
    {
        return NANDC_OK;
    }

    if(NAND_CMD_READSTART == access)
    {
        memcpy(host->databuf + bufoffset, host->bufbase, datasize);
    }
    else /*NAND_CMD_PAGEPROG*/
    {
        memcpy( host->bufbase, host->databuf + bufoffset, datasize);
    }

    return NANDC_OK;
}

#ifdef __cplusplus
}
#endif


