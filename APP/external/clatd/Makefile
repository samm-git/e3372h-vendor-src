
#KERNEL_HEADERS=-I/usr/src/linux/include

#CC=arm-none-linux-gnueabi-gcc
CC = gcc
CFLAGS= -Wall -DFEATURE_E5832 -DFEATURE_EC5825 -DWEB_UI_NEW -DWEB_SOURCE_US_CRICKET -DFORBID_ROAMING_DIAL -DFEATURE_HUAWEI_MAC_UPDATE -DFEATURE_EC5805 -DFEATURE_FIELD_TEST -DFEATURE_EC5805_FLUX_COLLECT -DFEATURE_EC5825_CDMA_SMS -DFEATURE_PHONEBOOK -DFEATURE_CDMA_OTA -DFEATURE_REDIRECT
LDFLAGS=-L/shared/bcm/c00127420/EC5805_b108/LINUX/project/platform-msm7625/sysroot-target/lib -L/shared/bcm/c00127420/EC5805_b108/LINUX/project/platform-msm7625/sysroot-target/usr/lib -Wl,-rpath-link -Wl,/shared/bcm/c00127420/EC5805_b108/LINUX/project/platform-msm7625/sysroot-target/usr/lib
INCLUDE=-Iexternal/libnl-headers  -I./include
#LIBS= -L ../libbridge -lbridge 

prefix=/usr
exec_prefix=${prefix}
bindir=${exec_prefix}/bin
sbindir=${exec_prefix}/sbin
mandir=${prefix}/man

INSTALL=/usr/bin/install -c


common_SOURCES= clatd.c dump.c checksum.c translate.c ipv4.c ipv6.c config.c dns64.c logging.c getaddr.c getroute.c netlink_callbacks.c netlink_msg.c setif.c setroute.c mtu.c

common_OBJECTS= $(common_SOURCES:.c=.o)

OBJECTS= $(common_OBJECTS)

PROGRAMS= clatd 


all:	$(PROGRAMS)

install: $(PROGRAMS)
	mkdir -p $(DESTDIR)$(sbindir)
	$(INSTALL) -m 755 $(PROGRAMS) $(DESTDIR)$(sbindir)

clatd:	$(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) -o clatd

%.o: %.c 
	$(CC) $(CFLAGS) $(INCLUDE) -c $< 

clean:
	rm -f *.o brctl core

