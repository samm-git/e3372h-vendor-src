/*
 * (C) 2005-2011 by Pablo Neira Ayuso <pablo@netfilter.org>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include "internal/internal.h"

const uint32_t attr_grp_bitmask[ATTR_GRP_MAX][__NFCT_BITSET] = {
	[ATTR_GRP_ORIG_IPV4] = {
		[0] = (1 << ATTR_ORIG_IPV4_SRC) |
		      (1 << ATTR_ORIG_IPV4_DST) |
		      (1 << ATTR_ORIG_L3PROTO),
	},
	[ATTR_GRP_REPL_IPV4] = {
		[0] = (1 << ATTR_REPL_IPV4_SRC) |
		      (1 << ATTR_REPL_IPV4_DST) |
		      (1 << ATTR_REPL_L3PROTO),
	},
	[ATTR_GRP_ORIG_IPV6] = {
		[0] = (1 << ATTR_ORIG_IPV6_SRC) |
		      (1 << ATTR_ORIG_IPV6_DST) |
		      (1 << ATTR_ORIG_L3PROTO),
	},
	[ATTR_GRP_REPL_IPV6] = {
		[0] = (1 << ATTR_REPL_IPV6_SRC) |
		      (1 << ATTR_REPL_IPV6_DST) |
		      (1 << ATTR_REPL_L3PROTO),
	},
	[ATTR_GRP_ORIG_PORT] = {
		[0] = (1 << ATTR_ORIG_PORT_SRC) |
		      (1 << ATTR_ORIG_PORT_DST) |
		      (1 << ATTR_ORIG_L4PROTO),
	},
	[ATTR_GRP_REPL_PORT] = {
		[0] = (1 << ATTR_REPL_PORT_SRC) |
		      (1 << ATTR_REPL_PORT_DST) |
		      (1 << ATTR_REPL_L4PROTO),
	},
	[ATTR_GRP_ICMP] = {
		[0] = (1 << ATTR_ICMP_CODE) |
		      (1 << ATTR_ICMP_TYPE) |
		      (1 << ATTR_ICMP_ID),
	},
	[ATTR_GRP_MASTER_IPV4] = {
		[1] = (1 << (ATTR_MASTER_IPV4_SRC - 32)) |
		      (1 << (ATTR_MASTER_IPV4_DST - 32)) |
		      (1 << (ATTR_MASTER_L3PROTO - 32)),
	},
	[ATTR_GRP_MASTER_IPV6] = {
		[1] = (1 << (ATTR_MASTER_IPV6_SRC - 32)) |
		      (1 << (ATTR_MASTER_IPV6_DST - 32)) |
		      (1 << (ATTR_MASTER_L3PROTO - 32)),
	},
	[ATTR_GRP_MASTER_PORT] = {
		[1] = (1 << (ATTR_MASTER_PORT_SRC - 32)) |
		      (1 << (ATTR_MASTER_PORT_DST - 32)) |
		      (1 << (ATTR_MASTER_L4PROTO - 32)),
	},
	[ATTR_GRP_ORIG_COUNTERS] = {
		[0] = (1 << (ATTR_ORIG_COUNTER_PACKETS)) |
		      (1 << (ATTR_ORIG_COUNTER_BYTES)),
	},
	[ATTR_GRP_REPL_COUNTERS] = {
		[0] = (1 << (ATTR_REPL_COUNTER_PACKETS)) |
		      (1 << (ATTR_REPL_COUNTER_BYTES)),
	},
};
