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
#ifndef _NANDC_DEF_H_
#define _NANDC_DEF_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <bsp_nandc.h>

#define nandc_latch_none                    (0)
#define nandc_read_latch_start              (1)
#define nandc_read_latch_done               (2)
#define nandc_write_latch_start             (3)
#define nandc_write_latch_done              (4)

#define NANDC_SPARE_LENGTH_16BYTE           (16)
#define NANDC_SPARE_LENGTH_32BYTE           (32)
#define NANDC_SPARE_LENGTH_64BYTE           (64)
#define NANDC_SPARE_LENGTH_128BYTE          (128)
#define NANDC_SPARE_LENGTH_218BYTE          (218)
#define NANDC_SPARE_LENGTH_224BYTE          (224)
#define NANDC_SPARE_LENGTH_256BYTE          (256)
#define NANDC_SPARE_LENGTH_400BYTE          (400)
#define NANDC_SPARE_LENGTH_436BYTE          (436)
#define NANDC_SPARE_LENGTH_448BYTE          (448)
#define NANDC_SPARE_LENGTH_512BYTE          (512)
#define NANDC_SPARE_LENGTH_640BYTE          (640)

#define nandc_size_1Gbit                    (128 << 20) /*128MB*/

#define NANDC_ADDR_INVALID                  (0xFFFFFFFF)

#define NANDC_READID_SIZE	                    (8)

/***********NANDC ctrl option bit value start*****************************************
---------------------------------------------------------------------------
*  bit 31:5   --    reserved
---------------------------------------------------------------------------
*  bit 4:3     RW    read type      ->   read  data or oob
---------------------------------------------------------------------------
*  bit 2:2     RW    ECC             ->  use default ecc or forbid it (no ecc)
 --------------------------------------------------------------------------
*  bit 1:1     RW    WAIT           ->   waite operation complete: int mode or poll mode
--------------------------------------------------------------------------
*  bit 0:0     RW    DMA            ->   dma mode or normal mode
*****************************************************************/
#define NANDC_OPTION_DMA_DISABLE            (0x0000)
#define NANDC_OPTION_DMA_ENABLE             (0x0001)

#define NANDC_OPTION_WAIT_POLL              (0x0000)
#define NANDC_OPTION_WAIT_INT               (0x0002)


#define NANDC_OPTION_ECC_DEFAULT            (0x0000)
#define NANDC_OPTION_ECC_FORBID             (0x0004)

#define NANDC_OPTION_DATA_OOB               (0x0000)
#define NANDC_OPTION_OOB_ONLY               (0x0007)
#define NANDC_OPTION_DATA_ONLY              (0x0010)

/***********NANDC ctrl option bit value end*****************************************/

#define NANDC_GOOD_BLOCK                    (0)
#define NANDC_BAD_BLOCK                     (1)

#define NANDC_EARSED_BLOCK                  (1)
#define NANDC_DIRTY_BLOCK                   (0)


#define NAND_CMD_NOP		                (0)

#define nandc_bits_enum_start               (0)
#define nandc_bits_cmd                      (1)
#define nandc_bits_cmd2                     (2)
#define nandc_bits_cmd3                     (3)
#define nandc_bits_bus_width                (4)
#define nandc_bits_addr_high                (5)
#define nandc_bits_addr_low                 (6)
#define nandc_bits_addr_cycle               (7)
#define nandc_bits_chip_select              (8)
#define nandc_bits_operation                (9)
#define nandc_bits_op_status                (10)
#define nandc_bits_int_status               (11)
#define nandc_bits_data_num                 (12)
#define nandc_bits_ecc_type                 (13)
#define nandc_bits_ecc_select               (14)
#define nandc_bits_page_size                (15)
#define nandc_bits_op_mode                  (16)
#define nandc_bits_int_enable               (17)
#define nandc_bits_int_clear                (18)
#define nandc_bits_segment                  (19)
#define nandc_bits_ecc_result               (20)
#define nandc_bits_nf_status                (21)
#define nandc_bits_async_interface_type     (22)
#define nandc_bits_randomizer               (23)
#define nandc_bits_read_id_en               (24)
#define nandc_bits_rw_reg_en                (25)
#define nandc_bits_ecc_class                (26)
#define nandc_bits_randomizer_en            (27)
#define nandc_bits_cs0_done                 (28)
#define nandc_bits_ahb_op_err               (29)
#define nandc_bits_wr_lock_err              (30)
#define nandc_bits_dma_done                 (31)
#define nandc_bits_dma_err                  (32)
#define nandc_bits_dma_start                (33)
/*
#define nandc_bits_dma_rw_enable            (34)
#define nandc_bits_burst_enable             (35)
#define nandc_bits_dma_addr_num             (36)
#define nandc_bits_dma_cs                   (37)
#define nandc_bits_dma_wr                   (38)
#define nandc_bits_dma_rd_oob               (39)
*/
#define nandc_bits_base_addr_data           (40)
#define nandc_bits_base_addr_oob            (41)
#define nandc_bits_dma_len                  (42)
#define nandc_bits_errnum0buf0              (43)
#define nandc_bits_rb_status                (44)
#define nandc_bits_base_addr_d1             (45)
#define nandc_bits_base_addr_d2             (46)
#define nandc_bits_base_addr_d3             (47)
#define nandc_bits_oob_len_sel              (48)
#define nandc_bits_dma_ctrl					(49)

#define NANDC_BITS_OP_DONE_EN               (50)
#define NANDC_BITS_CS_DONE_EN               (51)
#define NANDC_BITS_ERR_VALID_EN             (52)
#define NANDC_BITS_ERR_INVALID_EN           (53)
#define NANDC_BITS_AHD_OP_EN                (54)
#define NANDC_BITS_WR_LOCK_EN               (55)
#define NANDC_BITS_DMA_DONE_EN              (56)
#define NANDC_BITS_DMA_ERR_EN               (57)

#define nandc_bits_enum_end                 (0xff)   /*don't add after this*/

#define nandc_alige_size(p, a)              (((p)+  ((a) - 1)) & ~ ((a) - 1))

#define nandc_reg_desc_max                  (8)

#define nandc_op_boot                       (0)
#define nandc_op_normal                     (1)

#define nandc_page_hk                       (0)
#define nandc_page_2k                       (1)
#define nandc_page_4k                       (2)
#define nandc_page_8k                       (3)
#define nandc_page_16k                      (4)


#define nandc_bus_08                        (0)
#define nandc_bus_16                        (1)

#define nandc_cs_00                         (0)
#define nandc_cs_01                         (1)
#define nandc_cs_02                         (2)
#define nandc_cs_03                         (3)

#define nandc6_not_read_id                  (0)
#define nandc6_read_id                      (1)

#define nandc6_rw_reg                       (0)
#define nandc6_rw_data                      (1)


#define nandc_status_op_done		        (1)
#define nandc_status_int_done		        (1)
#define nandc_status_dma_done		        (0)

#define NANDC_COLUMN_SHIFT                  (16) /* when configure nfc_addrl, low 16 bits is for column address */
#define NAND_ONCHIP_ECC                     (0x80)

#ifdef __cplusplus
}
#endif

#endif  /*_NANDC_DEF_H_*/



