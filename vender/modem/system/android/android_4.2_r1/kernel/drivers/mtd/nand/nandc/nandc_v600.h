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
#ifndef _NANDC_v600_H_
#define _NANDC_v600_H_

#ifdef __cplusplus
extern "C"
{
#endif


#define NANDC6_NAME                             "NANDC_V6.00"   /* TODO:   rename to "hinandv4" */
#define NANDC6_BUFSIZE_BASIC                    (8192)
#define NANDC6_BUFSIZE_EXTERN                   (1024)
#define NANDC6_BUFSIZE_TOTAL                    (NANDC6_BUFSIZE_BASIC + NANDC6_BUFSIZE_EXTERN)
#define NANDC6_REG_SIZE                         (0x00D8 + 0x04)
#define NANDC6_IRQ                              (106)

#define NANDC6_REG_BASE_ADDR                    (NANDC_REG_BASE_ADDR)
#define NANDC6_BUFFER_BASE_ADDR                 (NANDC_BUFFER_BASE_ADDR)

#define NANDC6_MAX_CHIPS                        NANDC_MAX_CHIPS
/*********** nandc6_REG_CONF_OFFSET *****************************************
-------------------------------------------------------------------------
*  bit 15:14 RW    nf_mode       -> 00: async nand interface
                                    01: toggle nand
                                    10: onfi V2.3 nand
                                    11: onfi V3.0 nand
--------------------------------------------------------------------------
*  bit 13    RW    randomizer_en -> 0: randomizer unenable
									1: randomizer enable
--------------------------------------------------------------------------
*  bit 12    -     reserve
--------------------------------------------------------------------------
*  bit 11:9  RW    ecc_type      -> 000: no ecc
                                    001: 1bit ecc
                                    010: 4bit ecc
                                    011: 8bit ecc
                                    100: 24bit for 1KB
                                    101: 40bit ecc
                                    110: 64bit ecc
---------------------------------------------------------------------------
*  bit 8      RW   rb_sel       ->  0: use share ready/busy signal
                                    1: use independent ready/busy signal
---------------------------------------------------------------------------
*  bit 7      RW   cs_ctrl      ->  0:  busy is 0
                                    1:  busy is 1
---------------------------------------------------------------------------
*  bit 6:5   --    reserved
---------------------------------------------------------------------------
*  bit 4     RW    bus_width    ->  0:8bit
                                    1:16bit
 --------------------------------------------------------------------------
*  bit 3:1   RW   pagesize      ->  000: reserve ;
                                    001: 2KByte  ;
                                    010: 4KByte  ;
                                    011: 8KByte  ;
                                    100: 16KByte ;
                                    101~111: reserve
--------------------------------------------------------------------------
*  bit 0     RW    opmode       ->  0:boot mode ;
                                    1: normal mode ;
******************************************************************************/

/* nandc register func values */
#define nandc6_async_nand                    (0x0)
#define nandc6_toggle                        (0x1)
#define nandc6_onfi_v2                       (0x2)
#define nandc6_onfi_v3                       (0x3)

#define nandc6_open_randomizer               (0x0)
#define nandc6_close_randomizer              (0x1)

#define nandc6_cs0_int                       (0x0)
#define nandc6_cs0_no_int                    (0x1)

#define nandc6_rw_no_int 					 (0x0)
#define nandc6_rw_int                        (0x1)

#define nandc6_wr_lock_no_int                (0x0)
#define nandc6_wr_lock_int                   (0x1)

#define nandc6_dma_no_int                    (0x0)
#define nandc6_dma_int                       (0x1)

#define nandc6_dma_err_no_int                (0x0)
#define nandc6_dma_err_int                   (0x1)

#define nandc6_dma_over                      (0x0)
#define nandc6_dma_start                     (0x1)

#define nandc6_dma_rd_enable 				 (0x0)
#define nandc6_dma_wr_enable 				 (0x1)

#define nandc6_burst4_enable 				 (0x1)
#define nandc6_burst8_enable 				 (0x2)
#define nandc6_burst16_enable				 (0x4)

#define nandc6_dmaaddr5						 (0x0)
#define nandc6_dmaaddr4						 (0x1)

#define nandc6_dmacs0                        (0x0)
#define nandc6_dmacs1                        (0x1)
#define nandc6_dmacs2                        (0x2)
#define nandc6_dmacs3                        (0x3)

#define nandc6_wr_all                        (0x0)
#define nandc_wr_onlydata                    (0x1)

#define nandc6_rd_page 						 (0x0)
#define nandc_rd_oob_only                    (0x1)

#define nandc6_rb_ready                      (0x0)
#define nandc6_enhanced_clear                (0x1)

#define nandc6_oob_len_8or16                 (0x0)
#define nandc6_oob_len_32                    (0x1)

#define nandc6_segment_first                 (0x0)
#define nandc6_segment_last                  (0x1)


#define nandc6_ecc_err_none                  (0x0)
#define nandc6_ecc_err_valid                 (0x1)
#define nandc6_ecc_err_invalid               (0x2)


#define nandc6_ecc_data                      (0x05)    /*000101*/
#define nandc6_ecc_oob                       (0x0a)    /*001010*/
#define nandc6_ecc_all                       (0x0f)    /*001111*/
#define nandc6_noecc_all                     (0x00)


/* nandc6 cmd */
#define nandc6_op_read_id                    (0x66)   /*0110 0110   cmd1_en + addr_en + wait_ready_en  + read_data_en*/
#define nandc6_op_read_start                 (0x6e)
#define nandc6_op_read_continue              (0x02)
#define nandc6_op_read_end                   (0x02)
#define nandc6_op_wirte_start                (0x70)
#define nandc6_op_wirte_continue             (0x10)
#define nandc6_op_wirte_end                  (0x1d)
#define nandc6_op_erase                      (0x6d)
#define nandc6_op_reset                      (0x44)   /*0100 0100   cmd1_en + wait_ready_en*/


#define nandc6_dma_read                      (0x38)   /* (0x70)   */
#define nandc6_dma_read_oob_only             (0x838)  /* (0x1070) */
#define nandc6_dma_write                     (0x39)   /* (0x72)   */

#define NANDC6_INT_OP_DONE_EN                   (1)
#define NANDC6_INT_OP_DONE_DIS                  (0)
#define NANDC6_INT_CS_DONE_EN                   (0xF)
#define NANDC6_INT_CS_DONE_DIS                  (0)
#define NANDC6_INT_ERR_INVALID_EN               (1)
#define NANDC6_INT_ERR_INVALID_DIS              (0)
#define NANDC6_INT_ERR_VALID_EN                 (1)
#define NANDC6_INT_ERR_VALID_DIS                (0)
#define NANDC6_INT_AHB_OP_EN                    (1)
#define NANDC6_INT_AHB_OP_DIS                   (0)
#define NANDC6_INT_WR_LOCK_EN                   (1)
#define NANDC6_INT_WR_LOCK_DIS                  (0)
#define NANDC6_INT_DMA_DONE_EN                  (1)
#define NANDC6_INT_DMA_DONE_DIS                 (0)
#define NANDC6_INT_DMA_ERR_EN                   (1)
#define NANDC6_INT_DMA_ERR_DIS                  (0)

#define nandc6_int_enable_all                (0x7ff)
#define nandc6_int_disable_all               (0x000)
#define nandc6_int_clear_all                 (0x7ff)



#ifdef __cplusplus
}
#endif

#endif  /*_NANDC_v600_H_*/



