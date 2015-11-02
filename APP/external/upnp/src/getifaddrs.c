/*
 * Copyright (c) 2006 WIDE Project. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the project nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE PROJECT AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE PROJECT OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/* getifaddrs -- get names and addresses of all network interfaces
   Copyright (C) 2002, 2003 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#include <net/if.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#ifdef __linux__
#include <linux/types.h>
#include <linux/rtnetlink.h>
#endif
#include "ifaddrs.h"

/* We could do this _much_ better. kame racoon in its current form
 * will esentially die at frequent changes of address configuration.
 */


static int parse_rtattr(struct rtattr *tb[], int max, struct rtattr *rta, int len)
{
	while (RTA_OK(rta, len)) {
		if (rta->rta_type <= max)
			tb[rta->rta_type] = rta;
		rta = RTA_NEXT(rta,len);
	}
	return 0;
}

static void recvaddrs(int fd, struct ifaddrs **ifa, __u32 seq)
{
	char	buf[8192];
	struct sockaddr_nl nladdr;
	struct iovec iov = { buf, sizeof(buf) };
	struct ifaddrmsg *m;
	struct rtattr * rta_tb[IFA_MAX+1];
	struct ifaddrs *I;

	while (1) {
		int status;
		struct nlmsghdr *h;

		struct msghdr msg = {
			(void*)&nladdr, sizeof(nladdr),
			&iov,	1,
			NULL,	0,
			0
		};

		status = recvmsg(fd, &msg, 0);

		if (status < 0)
			continue;

		if (status == 0)
			return;

		if (nladdr.nl_pid) /* Message not from kernel */
			continue;

		h = (struct nlmsghdr*)buf;
		while (NLMSG_OK(h, status)) {
			if (h->nlmsg_seq != seq)
				goto skip_it;

			if (h->nlmsg_type == NLMSG_DONE)
				return;

			if (h->nlmsg_type == NLMSG_ERROR)
				return;

			if (h->nlmsg_type != RTM_NEWADDR)
				goto skip_it;

			m = NLMSG_DATA(h);

			if (m->ifa_family != AF_INET &&
			    m->ifa_family != AF_INET6)
				goto skip_it;

			if (m->ifa_flags&IFA_F_TENTATIVE)
				goto skip_it;

			memset(rta_tb, 0, sizeof(rta_tb));
			parse_rtattr(rta_tb, IFA_MAX, IFA_RTA(m), h->nlmsg_len - NLMSG_LENGTH(sizeof(*m)));

			if (rta_tb[IFA_LOCAL] == NULL)
				rta_tb[IFA_LOCAL] = rta_tb[IFA_ADDRESS];
			if (rta_tb[IFA_LOCAL] == NULL)
				goto skip_it;
			I = malloc(sizeof(struct ifaddrs));
			if (!I)
				return;
			memset(I, 0, sizeof(*I));
			I->ifa_ifindex = m->ifa_index;
			I->ifa_addr = (struct sockaddr*)&I->ifa_addrbuf;
			I->ifa_addr->sa_family = m->ifa_family;
			if (m->ifa_family == AF_INET) {
				struct sockaddr_in *sin = (void*)I->ifa_addr;
				memcpy(&sin->sin_addr, RTA_DATA(rta_tb[IFA_LOCAL]), 4);
			} else {
				struct sockaddr_in6 *sin = (void*)I->ifa_addr;
				memcpy(&sin->sin6_addr, RTA_DATA(rta_tb[IFA_LOCAL]), 16);
				if (IN6_IS_ADDR_LINKLOCAL(&sin->sin6_addr))
					sin->sin6_scope_id = I->ifa_ifindex;

			}
			I->ifa_next = *ifa;
			*ifa = I;

skip_it:
			h = NLMSG_NEXT(h, status);
		}
		if (msg.msg_flags & MSG_TRUNC)
			continue;
	}
	return;
}

int getifaddrs(struct ifaddrs **ifa0)
{
	struct {
		struct nlmsghdr nlh;
		struct rtgenmsg g;
	} req;
	struct sockaddr_nl nladdr;
	struct sockaddr *sa;
	static __u32 seq = 0;
	struct ifaddrs *i;
	int fd;

	fd = socket(PF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
	if (fd < 0)
		return -1;

	memset(&nladdr, 0, sizeof(nladdr));
	nladdr.nl_family = AF_NETLINK;

	req.nlh.nlmsg_len = sizeof(req);
	req.nlh.nlmsg_type = RTM_GETADDR;
	req.nlh.nlmsg_flags = NLM_F_ROOT|NLM_F_MATCH|NLM_F_REQUEST;
	req.nlh.nlmsg_pid = 0;
	req.nlh.nlmsg_seq = ++seq;
	req.g.rtgen_family = AF_UNSPEC;

	if (sendto(fd, (void*)&req, sizeof(req), 0, (struct sockaddr*)&nladdr, sizeof(nladdr)) < 0) {
		close(fd);
		return -1;
	}

	*ifa0 = NULL;

	recvaddrs(fd, ifa0, seq);

	close(fd);

	fd = socket(AF_INET, SOCK_DGRAM, 0);

	for (i=*ifa0; i; i = i->ifa_next) {
		struct ifreq ifr;
		ifr.ifr_ifindex = i->ifa_ifindex;
		if(ioctl(fd, SIOCGIFNAME, (void*)&ifr)){
			break;
		}
		else
		{
			memcpy(i->ifa_name, ifr.ifr_name, 16);
		}
		
	}
	close(fd);

	return 0;
}

void freeifaddrs(struct ifaddrs *ifa0)
{
        struct ifaddrs *i;

        while (ifa0) {
                i = ifa0;
                ifa0 = i->ifa_next;
                free(i);
        }
}
