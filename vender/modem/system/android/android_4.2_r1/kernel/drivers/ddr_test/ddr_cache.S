/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2013-2015. All rights reserved.
 *
 * mobile@huawei.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#include <linux/linkage.h>

.equ CACHE_LINE,64

.text

	/* void arch_flush_cache_range(addr_t start, size_t len); */
ENTRY(arch_clean_cache_range)
0:
	mcr		p15, 0, r0, c7, c10, 1		// clean cache to PoC by MVA
	add		r0, r0, #CACHE_LINE
	subs	r1, r1, #CACHE_LINE
	bhs		0b
	
	mov		r0, #0
	mcr		p15, 0, r0, c7, c10, 4		// data sync barrier (formerly drain write buffer)

	bx		lr
END(arch_clean_cache_range)

ENTRY(arch_clean_cacheline)
	mcr		p15, 0, r0, c7, c10, 1		// clean cache to PoC by MVA
	mov		r0, #0
	mcr		p15, 0, r0, c7, c10, 4		// data sync barrier (formerly drain write buffer)

	bx		lr
END(arch_clean_cacheline)

	/* void arch_flush_invalidate_cache_range(addr_t start, size_t len); */
ENTRY(arch_clean_invalidate_cache_range)
0:
	mcr		p15, 0, r0, c7, c14, 1		// clean & invalidate cache to PoC by MVA
	add		r0, r0, #CACHE_LINE
	subs	r1, r1, #CACHE_LINE
	bhs		0b

	mov		r0, #0
	mcr		p15, 0, r0, c7, c10, 4		// data sync barrier (formerly drain write buffer)

	bx		lr
END(arch_clean_invalidate_cache_range)

ENTRY(arch_clean_invalidate_cacheline)
	mcr		p15, 0, r0, c7, c14, 1		// clean & invalidate cache to PoC by MVA
	mov		r0, #0
	mcr		p15, 0, r0, c7, c10, 4		// data sync barrier (formerly drain write buffer)

	bx		lr
END(arch_clean_invalidate_cacheline)


	/* void arch_invalidate_cache_range(addr_t start, size_t len); */
ENTRY(arch_invalidate_cache_range)
0:
	/* invalidate cache line */
	mcr		p15, 0, r0, c7, c6, 1
	add		r0, r0, #CACHE_LINE
	subs	r1, r1, #CACHE_LINE
	bhs		0b
	mov		r0, #0
	/* data sync barrier (formerly drain write buffer*/
	mcr		p15, 0, r0, c7, c10, 4
	bx		lr
END(arch_invalidate_cache_range)

ENTRY(arch_invalidate_cacheline)
	/* invalidate cache line */
	mcr		p15, 0, r0, c7, c6, 1
	mov		r0, #0
	/* data sync barrier (formerly drain write buffer*/
	mcr		p15, 0, r0, c7, c10, 4
	bx		lr
END(arch_invalidate_cacheline)
	/* void arch_sync_cache_range(addr_t start, size_t len); */
ENTRY(arch_sync_cache_range)
	push    { r14 }
	bl      arch_clean_cache_range

	mov     r0, #0
	mcr     p15, 0, r0, c7, c5, 0       // invalidate icache to PoU

	pop     { pc }
END(arch_sync_cache_range)

ENTRY(arch_sync_cacheline)
	push    { r14 }
	bl      arch_clean_cacheline

	mov     r0, #0
	mcr     p15, 0, r0, c7, c5, 0       // invalidate icache to PoU

	pop     { pc }
END(arch_sync_cacheline)
