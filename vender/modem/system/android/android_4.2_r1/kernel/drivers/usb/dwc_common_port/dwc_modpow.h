/*
 * Synopsys Portability Library Software and documentation
 * (hereinafter, "Software") is an Unsupported proprietary work of
 * Synopsys, Inc. unless otherwise expressly agreed to in writing
 * between Synopsys and you.
 * 
 * The Software IS NOT an item of Licensed Software or Licensed Product
 * under any End User Software License Agreement or Agreement for
 * Licensed Product with Synopsys or any supplement thereto. You are
 * permitted to use and redistribute this Software in source and binary
 * forms, with or without modification, provided that redistributions
 * of source code must retain this notice. You may not view, use,
 * disclose, copy or distribute this file or any information contained
 * herein except pursuant to this license grant from Synopsys. If you
 * do not agree with this notice, including the disclaimer below, then
 * you are not authorized to use the Software.
 * 
 * Alternatively, this software may be distributed under the terms of the
 * GNU General Public License ("GPL") version 2 as published by the Free
 * Software Foundation.
 * 
 * THIS SOFTWARE IS BEING DISTRIBUTED BY SYNOPSYS SOLELY ON AN "AS IS"
 * BASIS AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE HEREBY DISCLAIMED. IN NO EVENT SHALL
 * SYNOPSYS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 */
/*
 * dwc_modpow.h
 * See dwc_modpow.c for license and changes
 */
#ifndef _DWC_MODPOW_H
#define _DWC_MODPOW_H

#ifdef __cplusplus
extern "C" {
#endif

#include "dwc_os.h"

/** @file
 *
 * This file defines the module exponentiation function which is only used
 * internally by the DWC UWB modules for calculation of PKs during numeric
 * association.  The routine is taken from the PUTTY, an open source terminal
 * emulator.  The PUTTY License is preserved in the dwc_modpow.c file.
 *
 */

typedef uint32_t BignumInt;
typedef uint64_t BignumDblInt;
typedef BignumInt *Bignum;

/* Compute modular exponentiaion */
extern Bignum dwc_modpow(void *mem_ctx, Bignum base_in, Bignum exp, Bignum mod);

#ifdef __cplusplus
}
#endif

#endif /* _LINUX_BIGNUM_H */
