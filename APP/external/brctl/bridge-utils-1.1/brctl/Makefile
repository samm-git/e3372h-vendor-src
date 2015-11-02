
KERNEL_HEADERS=-I/usr/src/linux/include

LOCAL_PATH:= $(shell pwd)

#CC=arm-none-linux-gnueabi-gcc
CFLAGS= -Wall -DFEATURE_E5832 -DFEATURE_EC5825 -DWEB_UI_NEW -DWEB_SOURCE_US_CRICKET -DFORBID_ROAMING_DIAL -DFEATURE_HUAWEI_MAC_UPDATE -DFEATURE_EC5805 -DFEATURE_FIELD_TEST -DFEATURE_EC5805_FLUX_COLLECT -DFEATURE_EC5825_CDMA_SMS -DFEATURE_PHONEBOOK -DFEATURE_CDMA_OTA -DFEATURE_REDIRECT
LDFLAGS=-L/shared/bcm/c00127420/EC5805_b108/LINUX/project/platform-msm7625/sysroot-target/lib -L/shared/bcm/c00127420/EC5805_b108/LINUX/project/platform-msm7625/sysroot-target/usr/lib -Wl,-rpath-link -Wl,/shared/bcm/c00127420/EC5805_b108/LINUX/project/platform-msm7625/sysroot-target/usr/lib
INCLUDE=-I../libbridge $(KERNEL_HEADERS) 
INCLUDE += -I$(LOCAL_PATH)/../../../../../netplatforminc
LIBS= -L ../libbridge -lbridge 

prefix=/usr
exec_prefix=${prefix}
bindir=${exec_prefix}/bin
sbindir=${exec_prefix}/sbin
mandir=${prefix}/man

INSTALL=/usr/bin/install -c


common_SOURCES= brctl_cmd.c brctl_disp.c 
brctl_SOURCES=  brctl.c $(common_SOURCES)

common_OBJECTS= $(common_SOURCES:.c=.o)
brctl_OBJECTS= $(brctl_SOURCES:.c=.o)

OBJECTS= $(common_OBJECTS) $(brctl_OBJECTS)

PROGRAMS= brctl 


all:	$(PROGRAMS)

install: $(PROGRAMS)
	mkdir -p $(DESTDIR)$(sbindir)
	$(INSTALL) -m 755 $(PROGRAMS) $(DESTDIR)$(sbindir)

brctl:	$(brctl_OBJECTS) ../libbridge/libbridge.a
	$(CC) $(LDFLAGS) $(brctl_OBJECTS) $(LIBS) -o brctl
	cp -f $(PROGRAMS) $(TARGETDIR)
%.o: %.c brctl.h
	$(CC) $(CFLAGS) $(INCLUDE) -c $< 

clean:
	rm -f *.o brctl core

