/* $Id: iptcrdr.h,v 1.14 2007/12/18 10:14:12 nanard Exp $ */
/* MiniUPnP project
 * http://miniupnp.free.fr/ or http://miniupnp.tuxfamily.org/
 * (c) 2006 Thomas Bernard 
 * This software is subject to the conditions detailed
 * in the LICENCE file provided within the distribution */
 
   /**********************问题单修改记录******************************************
    日期              修改人         问题单号           修改内容
  
 2012.03.14        z00203875     2031401440    修改打LOG方式
 2012.04.03        z00203875     2032802867    无法在不删除规则的
                                                     情况下禁用规则  
 2014.06.10        y00248130     4062600669    PCP特性
******************************************************************************/

#ifndef __IPTCRDR_H__
#define __IPTCRDR_H__

#include "commonrdr.h"

void reload_port_mapping_rules(void);

int
add_redirect_rule2(const char * ifname, unsigned short eport,
                   const char * iaddr, unsigned short iport, int proto,
				   const char * desc, int enabled);

int
add_filter_rule2(const char * ifname, const char * iaddr,
                 unsigned short eport, unsigned short iport,
                 int proto, const char * desc);

int
delete_redirect_and_filter_rules(unsigned short eport, int proto);

/* for debug */
int
list_redirect_rule(const char * ifname);

int
get_redirect_rule2(const char * ifname, unsigned short eport, int proto,
                  char * iaddr, int iaddrlen, unsigned short * iport,
                  char * desc, int desclen,
                  char * rhost, int rhostlen,
                  unsigned int * timestamp,
                  u_int64_t * packets, u_int64_t * bytes, int *penabled);

#ifdef MBB_FEATURE_PCP
void update_all_rules_to_pcp_client(void);
#endif

#endif

